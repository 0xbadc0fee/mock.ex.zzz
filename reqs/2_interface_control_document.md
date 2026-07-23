---
document: Interface Control Document (ICD)
project: ZZZ 4WS Sweeper Vehicle — Control Application
version: 0.1
status: Scaffold / Draft
authors:
  - Silas Curfman (STW Technic, L.P.)
  - Claude (Anthropic) — co-author
related_documents:
  - id: FRD
    title: Functional Requirements Document
    file: 1_functional_requirements_v2.md
  - id: SOW
    title: Statement of Work
    file: 0_statement_of_work.md
  - id: DBC
    title: CAN network database (authoritative CAN encoding source)
    file: ZZZ_CAN_1.dbc
    exports:
      - zzz_CAN1.xml
      - zzz_ObjectList.csv
date: 2026-07-21
controller: STW ESX (STW.4cm), node name ESX_4CM_A
bus: CAN1, J1939
conventions:
  bit_numbering: LSB0 (bit 0 = least significant bit of byte 0)
  byte_order: Intel (little-endian) unless stated
  spn_status_encoding: 2-bit J1939 status (0=disabled/false, 1=enabled/true, 2=error, 3=not available)
---

# Interface Control Document — ZZZ 4WS Sweeper Vehicle

> **Purpose and co-authorship.** This ICD defines the external interfaces of the
> ESX_4CM_A control application: the CAN/J1939 message interface and the hardware
> I/O interface. It is a companion to the FRD, which states *behavior*; this
> document states *interface encoding and channel allocation*. Co-authored by
> Claude (Anthropic) with the STW Technic engineering owner.

> **Authority and scope of this scaffold.** The CAN sections below are **derived
> from** the project DBC (`ZZZ_CAN_1.dbc`, via exports `zzz_CAN1.xml` and
> `zzz_ObjectList.csv`). Where this document and the DBC disagree, **the DBC is
> authoritative** for CAN signal encoding. The Hardware I/O section (§4) is **not**
> derivable from the DBC and is provided here as a structured stub to be populated
> from the ECU hardware allocation / pin map.

---

## 1. Scope and interface inventory

This ICD covers two interface classes for the `ESX_4CM_A` node:

| Interface class | Section | Source of truth |
| :---- | :---- | :---- |
| CAN / J1939 messages and signals | §2, §3 | `ZZZ_CAN_1.dbc` (authoritative) |
| Hardware discrete/analog/frequency I/O | §4 | ECU hardware allocation (TBD — stub) |

### 1.1 Node interface summary (from DBC)

| Node | Role | TX messages | RX messages |
| :---- | :---- | :---- | :---- |
| `ESX_4CM_A` | Controller (this application) | TSC1, AUXIO1 | BJM1, EEC1 |
| `Joystick_J1939` | Operator input device | EJM1, BJM1 | — |
| `Engine` | Engine ECU | IC1, ET1, EEC1 | TSC1 |

---

## 2. CAN message catalog (ESX_4CM_A interface)

Only messages that cross the `ESX_4CM_A` boundary are listed. PGNs are computed
from the 29-bit extended identifier in the DBC.

| Message | PGN | CAN ID (ext) | DLC | Dir (ESX) | Cycle | Description |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- |
| `BJM1` | 64982 | 0x0CFDD6FE | 8 | **RX** | 100 ms | Basic Joystick Message 1 |
| `EJM1` | 64983 | 0x0CFDD7FE | 8 | (not RX by ESX) | 100 ms | Extended Joystick Message 1 |
| `EEC1` | 61444 | 0x0CF004FE | 8 | **RX** | — | Electronic Engine Controller 1 |
| `TSC1` | 0 (00 00) | 0x0C0000FE | 8 | **TX** | — | Torque/Speed Control 1 |
| `AUXIO1` | 65241 | 0x00FED900 | 8 | **TX** | — | Auxiliary Input/Output Status 1 |

> **Note — BJM1 vs EJM1.** The application consumes the FNR and drum-speed axis
> values from **BJM1** (RX). EJM1 exists in the DBC but is not received by the ESX
> node in the current network model.

---

## 3. CAN signal definitions (FRD-referenced)

This section documents the specific signals cited by FRD requirements. It is a
*curated subset* of the 86-signal DBC — the DBC remains authoritative and complete.
Bit positions use LSB0 numbering within the 8-byte payload.

### 3.1 BJM1 (PGN 64982) — RX — joystick primary axes

| Signal | SPN | Start bit | Size | Type | Factor | Offset | Range | Unit | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- |
| `Joystick1XAxisNeutralPosStatus` | 2675 | 0 | 2 | status | 1 | 0 | 0–3 | — | FR-SD-01 (gating) |
| `Joystick1XAxisPos` | 2660 | 6 | 10 | unsigned | 0.1 | 0 | 0–102 | % | FR-SD-01 |
| `Joystick1YAxisNeutralPosStatus` | 2676 | 16 | 2 | status | 1 | 0 | 0–3 | — | FR-FNR-03 |
| `Joystick1YAxisPos` | 2661 | 22 | 10 | unsigned | 0.1 | 0 | 0–102 | % | FR-FNR-02 |

> **Critical interpretation note (raw vs. logical).** On the wire, both axis
> positions are **unsigned 0–102 %** (10-bit, factor 0.1). The FRD/SOW logical model
> for the FNR Y-axis is a **signed −100…0…+100** scale (full reverse / neutral /
> full forward). The application is responsible for mapping raw `Joystick1YAxisPos`
> (0–102 %) into the signed logical range, using `Joystick1YAxisNeutralPosStatus`
> (SPN 2676) to identify the dead-center neutral band. The same raw-unsigned pattern
> applies to the X-axis drum-speed request (0–102 % → 0–100 % request domain).
> This mapping is application logic, not a wire property — see FR-FNR-02/03.

### 3.2 EEC1 (PGN 61444) — RX — engine feedback

| Signal | SPN | Start bit | Size | Type | Factor | Offset | Range | Unit | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- |
| `EngSpeed` | 190 | 24 | 16 | unsigned | 0.125 | 0 | 0–8031.875 | rpm | FR-ES-02, FR-FNR-01, FR-TH-06 |

> Used for Engine OFF determination (< 450 rpm per FR-ES-02) and the EFC-enable
> gate (Engine ON per FR-FNR-01).

### 3.3 TSC1 (PGN 0) — TX — engine torque/speed command

TSC1 is transmitted by the ESX to command engine speed (throttle function). Signal
set is defined in the DBC; key request signals:

| Signal (DBC) | SPN | Notes | FRD ref |
| :---- | :---- | :---- | :---- |
| `EngOverrideCtrlMode` | 695 | Override control mode | FR-TH-05, FR-TH-06 |
| `ControlPurpose` | 3350 | TSC1 control-purpose mode | FR-TH-07 |
| *(requested speed signal per DBC)* | — | Computed Engine Requested Speed | FR-TH-05, FR-TH-07 |

> **[VERIFY]** Confirm the exact DBC signal name carrying "Engine Requested Speed"
> in TSC1 and populate the middle row. TSC1 signal layout is owned by the DBC.

### 3.4 AUXIO1 (PGN 65241) — TX — auxiliary status out (LED indication)

AUXIO1 is transmitted by the ESX to drive operator indication, including the
joystick-mounted status LED.

| Signal | SPN | Start bit | Size | Type | Range | Meaning | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- |
| `AuxiliaryIO_04` | 704 | 0 | 2 | status | 0–3 | **Neutral Safe** status → joystick top LED | FR-ES-06 |

> **Encoding of Neutral Safe (SPN 704).** 2-bit J1939 status field in byte 0.
> Per project decision: the LED-driving status is TRUE (1) when Neutral Safe is
> asserted, FALSE (0) otherwise. (The originally-described "bit 7 = 0, bit 8 =
> status" maps onto this 2-bit field; the DBC models it as the 2-bit SPN-704 status
> at start bit 0.) Remaining `AuxiliaryIO_xx` channels (SPN 701–716) and the two
> 16-bit `AuxiliaryIO_channel_1/2` (SPN 1083/1084) are available for future
> indication and are currently unassigned by the FRD.

---

## 4. Hardware I/O interface (STUB — not in DBC)

> **This section is intentionally incomplete.** None of the following signals are
> CAN; they are physical ECU channels and cannot be derived from the DBC. Populate
> from the ESX hardware allocation / pin map. Each row's **logical name** is the
> stable identifier that FRD requirements and application code should reference; the
> **channel/pin** binding is filled in from hardware design.

### 4.1 Digital inputs

| Logical name | Channel/Pin | Electrical | Active state | Description | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- |
| `DI_OperatorPresence` | TBD | 12 VDC discrete | 12 V = present (door closed) | Cab door-ajar switch → operator presence | FR-ES-03, §2.3 FRD |
| `DI_IgnitionStart` | TBD | 12 VDC discrete | 12 V = crank req | Ignition switch full-rotation start request | FR-ES-01, FR-ES-03 |
| `DI_BoomLiftLimit` | TBD | discrete | TBD | Drum-boom upper travel limit switch | FR-BM-03 |
| `DI_BoomLowerLimit` | TBD | discrete | TBD | Drum-boom lower travel limit switch | FR-BM-04 |

### 4.2 Frequency / pulse inputs

| Logical name | Channel/Pin | Type | Description | Calibration | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- |
| `FI_WheelSpeed` | TBD | tone-ring freq input (Hz) | Wheel-speed pulse train | `pulses_per_rev` (fixed); `wheel_circumference` (data-pool, adjustable) | FR-WS-01…05 |

### 4.3 Outputs (PWM / proportional / discrete)

| Logical name | Channel/Pin | Type | Fail-safe state | Description | FRD ref |
| :---- | :---- | :---- | :---- | :---- | :---- |
| `PO_EFC_Forward` | TBD | proportional PWM | de-energized | Forward EFC drive valve | FR-FNR-06 |
| `PO_EFC_Reverse` | TBD | proportional PWM | de-energized | Reverse EFC drive valve | FR-FNR-07 |
| `DO_EFC_Coast` | TBD | discrete/PWM | **de-energized (= brake)** | Free-wheeling bypass valve; energized only in NEUTRAL | FR-FNR-08, FR-FNR-10 |
| `PO_SweeperDrum` | TBD | proportional PWM | de-energized | Sweeper drum coil(s) drive | FR-SD-08 |
| `PO_BoomRaise` | TBD | discrete/PWM | de-energized | Boom raise valve | FR-BM-01 |
| `PO_BoomLower` | TBD | discrete/PWM | de-energized | Boom lower valve | FR-BM-01 |

> **Fail-safe note (FR-FNR-10).** `DO_EFC_Coast` de-energizes on signal loss or
> power loss. Because the coast valve must be energized to permit free-wheeling,
> its de-energized state stops recirculation flow and produces a hydraulic braking
> effect — the deliberate safe state.

---

## 5. Open items

| Item | Location | Needed |
| :---- | :---- | :---- |
| TSC1 requested-speed signal name | §3.3 | Confirm exact DBC signal for Engine Requested Speed |
| All hardware channel/pin bindings | §4 | Populate from ESX hardware allocation |
| Fail-safe states for boom/drum outputs | §4.3 | Confirm intended de-energized behavior |
| AuxiliaryIO channel assignments beyond SPN 704 | §3.4 | Assign or leave reserved |

---

## Appendix A — DBC export provenance

Derived from `ZZZ_CAN_1.dbc` exports: `zzz_CAN1.xml` (network/message/signal model,
86 signals) and `zzz_ObjectList.csv` (signal attribute list). Regenerate this ICD's
§2–§3 tables whenever the DBC changes; the DBC is the single source of truth for CAN
encoding.

*End of Interface Control Document, Version 0.1 (scaffold).*
