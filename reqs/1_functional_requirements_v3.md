---
document: Functional Requirements Document (FRD)
project: ZZZ 4WS Sweeper Vehicle — Control Application
version: 3.0
status: Draft
supersedes: 2.0
authors:
  - Silas Curfman (STW Technic, L.P.)
  - Claude (Anthropic) — co-author
source_documents:
  - id: SOW
    title: Statement of Work
    file: 0_statement_of_work.md
related_documents:
  - id: ICD
    title: Interface Control Document
    file: 2_interface_control_document.md
  - id: DBC
    title: CAN network database (authoritative CAN encoding source)
    file: ZZZ_CAN_1.dbc
date: 2026-07-22
controller: STW ESX (STW.4cm), node ESX_4CM_A
bus: CAN1, J1939
---

# Functional Requirements Document — ZZZ 4WS Sweeper Vehicle

> **Co-authorship note.** Version 3 was co-authored by Claude (Anthropic) with the
> STW Technic engineering owner. It carries forward the Version 2 restructuring and
> resolves the open interface items by reference to the newly created ICD and the
> project DBC (`ZZZ_CAN_1.dbc`). Changes from V2 are listed in §8.

---

## 1. Introduction

### 1.1 Purpose

This document specifies the **system-level functional requirements** for the software
application that replaces the existing hardwired relay logic on the ZZZ Corp 4WS
Sweeper Vehicle. It is the technical counterpart to the business-level requirements
in the SOW. Requirements state **what the system must do**, not how it is implemented.

### 1.2 Scope

The application runs on a single STW ESX (`STW.4cm`) controller (node `ESX_4CM_A`)
and reproduces the vehicle and work-system behaviors currently provided by relay
logic, communicating over CAN / J1939. Fault handling, functional safety, RTOS,
braking/immobilizing, steering, and NVM/EEPROM handling are **out of scope** per the
SOW, except where a default or safe output state is called out below for behavioral
completeness.

### 1.3 How to read this document

Each requirement has a **stable identifier** of the form `FR-<FC>-<NN>`, where
`<FC>` is a **two- or three-letter** function code (see §1.5) and `<NN>` is a
zero-padded sequence number. Identifiers are **immutable**: once assigned, an ID is
never renumbered or reused, so downstream design, code, and tests can reference it
permanently. Withdrawn requirements are marked `WITHDRAWN` rather than deleted.

Each requirement uses a single normative **"shall"** and is written to be
independently verifiable. Requirement attributes appear in a fixed column order
(§1.4) so the tables are both human-readable and machine-parseable.

### 1.4 Requirement attribute conventions

| Attribute | Meaning |
| :---- | :---- |
| **ID** | Stable, immutable identifier (`FR-<FC>-<NN>`). |
| **Requirement** | Single-"shall" normative statement. |
| **Rationale / Source** | Origin: SOW section, joystick grid cell, ICD/DBC signal, or review decision. |
| **Verify** | Verification method: **T** = Test, **A** = Analysis, **I** = Inspection, **D** = Demonstration. |

Priority uses **MoSCoW** (`M` = Must, `S` = Should, `C` = Could) where a requirement
is not implicitly a Must.

Where a requirement references a CAN signal, the citation names the **logical signal
and SPN**; the authoritative encoding (PGN, bit position, scaling) lives in the ICD
(§ per that document) and ultimately in `ZZZ_CAN_1.dbc`. Hardware channels are named
by their **logical name** as defined in ICD §4.

### 1.5 Function code index

| Code | Function | Group |
| :---- | :---- | :---- |
| `BM` | Drum Boom Lift Control | Auxiliary device |
| `SD` | Sweeper Drum Control | Auxiliary device |
| `ES` | Engine Start Control | Chassis |
| `TH` | Engine Throttle Control | Chassis |
| `FNR` | Vehicle FNR Motion Control | Chassis |
| `WS` | Wheel Speed Sensing | Chassis |
| `PID` | PID Output Calculation | Helper |
| `RMP` | Ramp Value Calculation | Helper |
| `TGL` | Toggle Button Behavior | Helper |

### 1.6 References

| Ref | Document | Role |
| :---- | :---- | :---- |
| SOW | `0_statement_of_work.md` | Business requirements, vehicle description, joystick map |
| ICD | `2_interface_control_document.md` | CAN message/signal encoding and hardware I/O allocation |
| DBC | `ZZZ_CAN_1.dbc` | Authoritative CAN signal database (source of truth for encoding) |

> When this FRD and the ICD/DBC differ on **encoding**, the DBC governs. When they
> differ on **behavior**, this FRD governs.

---

## 2. System context and shared definitions

### 2.1 Operator input surface (joystick)

Vehicle and work-system control is translated by a single multi-axis joystick
(SureGrip JSM-style, node `Joystick_J1939`) with J1939 output. Each physical control
is identified by grid coordinate `Y<row>.X<col>`, where `Y0.X0` is top-left from the
operator's point of view and `Y3.X2` is the main grip axis at the base.

| Grid cell | Physical control | Function mapped | Group |
| :---- | :---- | :---- | :---- |
| Y0.X0 | Momentary button | PTO engage / disengage (toggle) | SD / ES |
| Y0.X1 | 3-position rocker | Boom: pos1 = Lower, pos2 = Neutral, pos3 = Raise | BM |
| Y0.X2 | *(unused)* | — | — |
| Y0.X3 | Friction-hold slider (0–100 %) | Sweeper drum speed request | SD |
| Y0.X4 | Momentary button | **Unassigned** — reserved for expansion | — |
| Y1.X1 | Momentary button | Throttle **increment** +100 RPM / press | TH |
| Y1.X3 | Momentary button | Throttle **decrement** −100 RPM / press | TH |
| Y2.X2 | Momentary trigger (index finger) | Sweeper drum engage (hold-to-run) | SD |
| Y3.X2 | Main grip axis, return-to-center | Vehicle FNR propulsion | FNR |

### 2.2 Main grip axis scale (logical vs. wire)

The FNR **logical** model for the main grip axis is a **signed scale**: `-100` =
full reverse, `0` = dead-center neutral, `+100` = full forward. On the **wire**, the
axis is transmitted as `Joystick1YAxisPos` (SPN 2661) in message `BJM1` (PGN 64982)
as an **unsigned 0–102 %** value. The application derives the signed logical value
from the raw value together with `Joystick1YAxisNeutralPosStatus` (SPN 2676). See
`FR-FNR-02`/`FR-FNR-03` and ICD §3.1.

### 2.3 Operator presence

Operator presence is derived from the **cab door-ajar switch** (logical input
`DI_OperatorPresence`, ICD §4.1). When the door is closed the switch is closed and
the input reads 12 VDC. This single signal is the operator-presence condition for all
logic requiring it.

### 2.4 Shared terms

| Term | Definition |
| :---- | :---- |
| **EFC** | Electronically-controlled, bi-directional proportional valve for driving-wheel direction and speed. |
| **Neutral band** | The narrow range of main-grip-axis values around `0` classified as NEUTRAL. |
| **Neutral Safe** | Composite TRUE/FALSE status defined in `FR-ES-04`. |
| **PTO Engaged** | Power-take-off state read from the Engine over CAN. |
| **App uptime** | Elapsed time in milliseconds since application start. |

---

## 3. Auxiliary Device Functions

### 3.1 Function BM — Drum Boom Lift Control

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-BM-01 | The system shall command Boom **Raise** or **Lower** based on the `Y0.X1` rocker position (pos1 = Lower, pos2 = Neutral, pos3 = Raise). | SOW joystick map | T |
| FR-BM-02 | The system shall prevent simultaneous activation of Raise and Lower outputs. | V1 FR-1.2 | T |
| FR-BM-03 | The system shall inhibit Raise when the drum-boom **lift-limit** switch (`DI_BoomLiftLimit`) is active. | V1 FR-1.3; ICD §4.1 | T |
| FR-BM-04 | The system shall inhibit Lower when the drum-boom **lower-limit** switch (`DI_BoomLowerLimit`) is active. | V1 FR-1.4; ICD §4.1 | T |
| FR-BM-05 | The system shall invert the default Raise/Lower mapping when the inversion flag is TRUE. | V1 FR-1.5 | T |

### 3.2 Function SD — Sweeper Drum Control

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-SD-01 | The system shall read the requested Sweeper Drum Speed from the `Y0.X3` slider (0–100 %) via CAN signal `Joystick1XAxisPos` (SPN 2660, BJM1). | V1 FR-2.1; ICD §3.1 | T |
| FR-SD-02 | The system shall read the PTO Engaged condition from the Engine via CAN. | V1 FR-2.2 | T |
| FR-SD-03 | The system shall scale and clamp the drum speed request within configured minimum and maximum limits. | V1 FR-2.3 | T |
| FR-SD-04 | The system shall command Sweeper Drum target speed to zero when PTO Engaged is FALSE, regardless of trigger or slider state. | V1 FR-2.4 | T |
| FR-SD-05 | The system shall command the drum to run **only while the `Y2.X2` trigger is depressed**. | SOW map; review | T |
| FR-SD-06 | On trigger depression from a stopped state, the system shall soft-start the drum by ramping from 0 toward the requested speed using Function RMP. | Review (was AR-2) | T |
| FR-SD-07 | On trigger release, the system shall ramp the drum from its current speed back to 0 rather than stopping instantaneously. | Review (was AR-2) | T |
| FR-SD-08 | The system shall convert the final drum speed command into a bounded proportional PWM output (`PO_SweeperDrum`) driving the drum coil(s). | V1 FR-2.6; ICD §4.3 | T |

### 3.3 Function TGL — Toggle Button Behavior *(helper)*

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-TGL-01 | The system shall toggle the latched output only when continuous press duration ≥ configured debounce time. | V1 FR-8.1 | T |
| FR-TGL-02 | The system shall allow only one toggle per press and shall require a release before accepting a new press. | V1 FR-8.2 | T |
| FR-TGL-03 | The system shall force the latched output to a defined safe state when faulted. | V1 FR-8 | T |

---

## 4. Chassis Functions

### 4.1 Function ES — Engine Start Control

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-ES-01 | The system shall read the ignition-start hardware input (`DI_IgnitionStart`) and acquire required inputs from other modules. | V1 FR-3.1; ICD §4.1 | T |
| FR-ES-02 | The system shall determine Engine OFF Status is TRUE when engine speed `EngSpeed` (SPN 190, EEC1) is < 450 RPM. | V1 FR-3.2; ICD §3.2 | T |
| FR-ES-03 | The system shall set the Engine Start output ON **only when** ignition start is requested **and** Engine is OFF **and** the joystick is in valid neutral **and** PTO Engaged is FALSE **and** operator presence is TRUE. | V1 FR-3.3 | T |
| FR-ES-04 | The system shall compute Neutral Safe as TRUE **only when** Engine is OFF **and** joystick is in valid neutral **and** PTO Engaged is FALSE **and** drum rotation < 10 RPM; otherwise FALSE. | V1 FR-3.4 | T |
| FR-ES-05 | The system shall transmit a CAN message containing Neutral Safe status. | V1 FR-3.5 | T |
| FR-ES-06 | The system shall transmit **Neutral Safe** status to the joystick-mounted operator LED via CAN signal `AuxiliaryIO_04` (SPN 704) in message `AUXIO1` (PGN 65241), set TRUE when Neutral Safe is asserted and FALSE otherwise. | Review (changed in V3); ICD §3.4 | T |

> **ES-05 vs ES-06.** FR-ES-05 is the general Neutral Safe broadcast; FR-ES-06 is the
> operator-indication path that drives the joystick LED via the AUXIO1 status field.
> They carry the same logical status for different consumers.

### 4.2 Function TH — Engine Throttle Control

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-TH-01 | The system shall interpret `Y1.X1` as Speed Increment (+100 RPM per complete press/release) and `Y1.X3` as Speed Decrement (−100 RPM per complete press/release). | V1 FR-4.1; SOW map | T |
| FR-TH-02 | On application start, the system shall command a calibrated minimum setpoint and hold it for 6 seconds before accepting operator adjustments. | V1 FR-4.2 | T |
| FR-TH-03 | The system shall change the engine RPM target at a rate of 100 RPM per 100 ms. | V1 FR-4.3 | T |
| FR-TH-04 | The system shall ensure mutual exclusivity when handling increment and decrement signals. | V1 FR-4.4 | T |
| FR-TH-05 | The system shall output the updated Engine Requested Speed to CAN (`TSC1`) after applying configurable ramping profiles. | V1 FR-4.5; ICD §3.3 | T |
| FR-TH-06 | The system shall set Engine Requested Speed and Engine Override Control to zero when the engine has been OFF for more than 6 seconds. | V1 FR-4.6 | T |
| FR-TH-07 | The system shall populate and transmit all defined `TSC1` signals, including the computed Engine Requested Speed. | V1 FR-4.7; ICD §3.3 | T |

### 4.3 Function FNR — Vehicle FNR Motion Control

Controls forward/neutral/reverse propulsion from the main grip axis (`Y3.X2`) by
driving the EFC valve set. Includes the free-wheeling neutral valve and dependence on
Function WS.

**Preconditions (enable gate).** No propulsion output shall be commanded unless the
EFC-enable conditions in `FR-FNR-01` hold. When disabled, the system shall command
the safe default output state defined in `FR-FNR-10`.

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-FNR-01 | The system shall enable EFC output **only when** Engine is ON **and** app uptime > 4000 ms; otherwise EFC output shall be disabled. | V1 FR-5.3 | T |
| FR-FNR-02 | The system shall read the main grip axis position from CAN signal `Joystick1YAxisPos` (SPN 2661, BJM1, PGN 64982) and derive the signed logical position (−100…+100) using `Joystick1YAxisNeutralPosStatus` (SPN 2676). | V1 FR-5.2; SOW §2.2; ICD §3.1 | T |
| FR-FNR-03 | The system shall classify axis position as FORWARD, NEUTRAL, or REVERSE using configured range thresholds, where the NEUTRAL band is a configured range about 0. | V1 FR-5.5 | T |
| FR-FNR-04 | The system shall compute a Target Speed command from the magnitude of the axis position. | V1 FR-5.4 | T |
| FR-FNR-05 | The system shall ramp the Target Speed from its previous value to the new value using Function RMP with configured maximum rates of change. | V1 FR-5.7 | T |
| FR-FNR-06 | In FORWARD, the system shall energize the forward EFC valve (`PO_EFC_Forward`) proportionally, de-energize the reverse valve, and de-energize the coast valve. | V1 FR-5.8; review; ICD §4.3 | T |
| FR-FNR-07 | In REVERSE, the system shall energize the reverse EFC valve (`PO_EFC_Reverse`) proportionally, de-energize the forward valve, and de-energize the coast valve. | V1 FR-5.8; review; ICD §4.3 | T |
| FR-FNR-08 | In NEUTRAL, the system shall de-energize both drive valves **and energize the coast (free-wheeling bypass) valve** (`DO_EFC_Coast`) to permit hydraulic recirculation so the vehicle coasts rather than hydraulically braking. | Review (new scope); ICD §4.3 | T |
| FR-FNR-09 | The system shall enforce that at most one of {forward, reverse} drive valves is energized at any time, and that neither drive valve is energized simultaneously with the coast valve. | V1 FR-5.9; review | T, A |
| FR-FNR-10 | On loss of valid axis signal or when EFC is disabled, the system shall command the **de-energized** output state for all three valves. Because the coast valve must be energized to permit free-wheeling, its de-energized state halts recirculation and produces a hydraulic braking effect — the deliberate safe state. | Review (resolved in V3) | T, A |
| FR-FNR-11 | The system shall decode the main grip axis from J1939 message `BJM1` (PGN 64982), signal `Joystick1YAxisPos` (SPN 2661), per the ICD/DBC encoding. | Review (resolved in V3); ICD §3.1 | I |
| FR-FNR-12 | The system shall transmit Neutral State status via CAN to indicate when the joystick is in the NEUTRAL band. | V1 FR-5.6 | T |

> **Valve output state table** (normative summary of FR-FNR-06…10)
>
> | State | `PO_EFC_Forward` | `PO_EFC_Reverse` | `DO_EFC_Coast` |
> | :---- | :---- | :---- | :---- |
> | FORWARD | proportional | off | off |
> | NEUTRAL | off | off | **energized** |
> | REVERSE | off | proportional | off |
> | DISABLED / FAULT | off | off | off (→ brake) |

### 4.4 Function WS — Wheel Speed Sensing

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-WS-01 | The system shall measure wheel-sensor pulse frequency from the ECU hardware frequency-input channel (`FI_WheelSpeed`). | V1 FR-5.1; review; ICD §4.2 | T |
| FR-WS-02 | The system shall compute wheel rotational rate using a **fixed** pulses-per-revolution constant (tone-ring tooth count). | Review (new scope) | T, A |
| FR-WS-03 | The system shall compute vehicle speed in MPH from wheel rotational rate and a **field-adjustable wheel circumference/diameter** calibration value. | Review (new scope) | T |
| FR-WS-04 | The system shall expose the wheel circumference/diameter calibration value in the openSYDE data pool so a technician can update it if the pulse-wheel/tire diameter changes. | Review (new scope) | I, D |
| FR-WS-05 | The system shall provide the computed vehicle speed (MPH) for use by other functions. | V1 FR-5.1 | T |

---

## 5. Helper Functions

### 5.1 Function PID — PID Output Calculation

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-PID-01 | The system shall execute the PID calculation cyclically at the input control rate. | V1 FR-6.1 | T |
| FR-PID-02 | The system shall compute error as setpoint minus measured feedback. | V1 FR-6.2 | T |
| FR-PID-03 | The system shall compute P, I, and D terms using input gains and the control period. | V1 FR-6.3 | T |
| FR-PID-04 | The system shall sum the P, I, and D terms into a control output. | V1 FR-6.4 | T |
| FR-PID-05 | The system shall clamp the control output within input min/max bounds. | V1 FR-6.5 | T |

### 5.2 Function RMP — Ramp Value Calculation

| ID | Requirement | Rationale / Source | Verify |
| :---- | :---- | :---- | :---- |
| FR-RMP-01 | The system shall execute the ramp calculation cyclically using the provided execution-period input. | V1 FR-7.1 | T |
| FR-RMP-02 | The system shall transition the output linearly from current toward the commanded target value. | V1 FR-7.2 | T |
| FR-RMP-03 | The system shall limit the per-cycle rate of change using the ramp-rate and execution-period inputs. | V1 FR-7.3 | T |
| FR-RMP-04 | The system shall hold the output constant once the commanded target is reached. | V1 FR-7.4 | T |
| FR-RMP-05 | The system shall constrain the ramped output within provided min/max limits prior to output. | V1 FR-7.5 | T |

---

## 6. Nonfunctional Requirements

| ID | Requirement | Verify |
| :---- | :---- | :---- |
| NFR-01 | Vehicle Application logic shall complete all operations within a span deviating no more than 500 ms from the same interactions on the existing relay design. | T |
| NFR-02 | Displacement-based interactions (distance per joystick action, drum rotations per ON time, boom travel per Lift command) shall complete within ±10 % of the original system for equivalent operator inputs. | T |

---

## 7. Traceability and open items

### 7.1 V1 → V3 identifier mapping

| V1 ID | V3 ID | V1 ID | V3 ID |
| :---- | :---- | :---- | :---- |
| FR-1.1…1.5 | FR-BM-01…05 | FR-5.1 | FR-WS-01/05 |
| FR-2.1…2.6 | FR-SD-01…08 | FR-5.2 | FR-FNR-02 |
| FR-3.1…3.5 | FR-ES-01…05 | FR-5.3 | FR-FNR-01 |
| FR-4.1…4.7 | FR-TH-01…07 | FR-5.4 | FR-FNR-04 |
| FR-6.1…6.5 | FR-PID-01…05 | FR-5.5 | FR-FNR-03 |
| FR-7.1…7.5 | FR-RMP-01…05 | FR-5.6 | FR-FNR-12 |
| FR-8.1…8.2 | FR-TGL-01…03 | FR-5.7 | FR-FNR-05 |
| AR-1 (spare) | §2.1 note (Y0.X4) | FR-5.8 | FR-FNR-06/07/08 |
| AR-2 (trigger) | FR-SD-05…07 | FR-5.9 | FR-FNR-09 |
| AR-3 (FNR axis) | FR-FNR-11 | — | — |

> **V2 → V3 note.** All `FR-FN-NN` identifiers from V2 are renamed to `FR-FNR-NN`
> in V3 (three-letter function code). No other identifiers changed. Because the
> `FR-FN-*` IDs existed only in the interim V2 draft and never in released code, this
> rename does not violate the immutability rule for fielded identifiers.

### 7.2 Open items register

| Item | Requirement | Decision needed |
| :---- | :---- | :---- |
| TSC1 requested-speed signal name | FR-TH-05/07; ICD §3.3 | Confirm exact DBC signal carrying Engine Requested Speed. |
| Hardware channel/pin bindings | ICD §4 | Populate logical→physical map from ESX hardware allocation. |
| Fail-safe states for boom/drum outputs | ICD §4.3 | Confirm intended de-energized behavior. |

> **Closed since V2:** Engine indicator destination (resolved → Neutral Safe via
> AUXIO1/SPN 704, FR-ES-06); FNR axis PGN/encoding (resolved → BJM1/64982/SPN 2661,
> FR-FNR-11); neutral/fault output default (resolved → de-energized, FR-FNR-10); SOW
> Assumptions placeholder (accepted as TBD/unused for this mock exercise).

---

## 8. Change log (V2 → V3)

| # | Change |
| :---- | :---- |
| 1 | FNR function code changed from `FN` to `FNR`; all `FR-FN-NN` → `FR-FNR-NN`. |
| 2 | Added §1.6 References and the ICD/DBC authority rule; requirements now cite CAN signals/SPNs with encoding delegated to ICD/DBC. |
| 3 | FR-ES-06 changed from "Engine ON/OFF indicator" to **Neutral Safe → joystick LED** via AUXIO1 (PGN 65241) / SPN 704. |
| 4 | FR-FNR-11 resolved: axis decode bound to BJM1 (PGN 64982) / `Joystick1YAxisPos` (SPN 2661). |
| 5 | FR-FNR-10 resolved: loss-of-signal / disabled default is **de-energized (brake)** for all valves. |
| 6 | §2.2 clarified the raw-unsigned (0–102 %) vs. logical-signed (−100…+100) axis distinction, referencing the neutral-status SPN. |
| 7 | SOW Assumptions open item closed (accepted TBD/unused for the mock). |
| 8 | ID convention generalized to two- or three-letter function codes (§1.3). |

---

*End of Functional Requirements Document, Version 3.0.*
