# Introduction

ZZZ is an OEM of mobile self propelled, off-highway, work vehicles.  The 4WS Sweeper Vehicle is a proven production unit which is currently operated via hardwired relay logic.

ZZZ has determined that reliance on hard wired relay logic is no longer a viable approach for this product and has contacted STW requesting a hardware and software solution to replace the current relay logic.

Assumptions  
The following assumptions are utilized throughout this document::

1. All requirements identified below are driven from the Functional Requirements outlined in the initial Statement of Work document for this project:  
   [SOW - ZZZ CORP](https://docs.google.com/document/d/1jJ9iLbOfMjGz0JBu7Xj2nEy5MyuOZMADzSY-G2Ge2aE/edit?usp=sharing)  
2. TBD

# 4WS \- Auxiliary Device Functions

## FUNCTION 1: Drum Boom Lift Control

### *Functionality*

The Drum Boom Control Module shall control the lifting and lowering of the machine “drum boom” through the use of hydraulic control valves and hardware or CAN switch inputs.

| ID | Description |
| :---- | :---- |
| FR-1.1 | The control module shall command the machine to perform “Lift” or “Lower” operations based on operator inputs. |
| FR-1.2 | The control module shall prevent simultaneous activation of “Lift” and “Lower” operations. |
| FR-1.3 | The control module shall utilize a “drum boom lift limit” switch to prevent boom lift activation beyond a certain range. |
| FR-1.4 | The control module shall utilize a “drum boom lower limit” switch to prevent boom lower activation beyond a certain range. |
| FR-1.5 | The control module shall invert the default button mapping for lift and lower commands when the inversion flag is set to true. |

## 

## FUNCTION 2: Sweeper Drum Control

### *Functionality*

The Sweeper Drum Control shall read the operator requested Sweeper Drum commands and convert the requested Sweeper Drum Speed to a PWM output to control the rotational speed of the Sweeper Drum

| ID | Description |
| :---- | :---- |
| FR-2.1 | The control module shall read the operator requested Sweeper Drum Speed Request from the operator joystick via CAN |
| FR-2.2 | The control module shall read the PTO Engaged condition from the Engine via CAN |
| FR-2.3 | The control module shall apply necessary scaling and clamping conditions to the Drum Speed Request command and bind the speed within minimum and maximum limits. |
| FR-2.4 | The control module shall command the Sweeper Drum Target Speed to zero when the PTO Engaged signal read from the Engine is false. |
| FR-2.5 | The control module shall ramp the Sweeper Drum Target Speed command from the precious value towards the operator requested Sweeper Drum Target Speed |
| FR-2.6 | The control module shall control the speed of the Sweeper Drum and convert the final Sweeper Drum Speed Command into a bounded proportional PWM output that drives the Sweeper Drum coil(s) |

4WS \- Chassis Functions

## FUNCTION 3: Engine Start Control

### *Functionality*

The Engine Start Control Module shall control the engine start operation based on operator ignition input and valid system permissive conditions.  The module shall inhibit engine start-up under unsafe conditions.

| ID | Description |
| :---- | :---- |
| FR-3.1 | The control module shall read the ignition start hardware input and acquire all additional inputs from internal control modules |
| FR-3.2 | The control module shall read in Engine Speed from the Engine (CAN), determine Engine OFF Status is TRUE when engine speed is \< 450 RPM, transmit the Engine ON/OFF Status to TBD (led output mounted on joystick, triggered with joystick send msg??) |
| FR-3.3 | The control module shall output the Engine Start Signal to the machine and set to ON only when the ignition start is requested, the Engine is OFF, the joystick is in a valid neutral state, PTO Engaged is FALSE, and operator presence is TRUE. |
| FR-3.4 | The control module shall compute Neutral Safe as TRUE only when the engine is OFF, the joystick is in a valid neutral state, PTO Engaged is FALSE, and Sweeper Drum rotation is less than 10 RPM; otherwise Neutral Safe shall be FALSE |
| FR-3.5 | The control module shall transmit a CAN message containing the Neutral Safe status |

## FUNCTION 4: Engine Throttle Control

### *Functionality*

The Throttle Control Module shall govern the vehicle's engine speed, measured as crankshaft rpm, by calculating new “Engine Requested Speed” values based on hardware input received from the operator and output via CAN based J1939 speed control messages.

| ID | Description |
| :---- | :---- |
| FR-4.1 | The control module shall interpret the assigned Joystick buttons for incremental speed increase and decrease as Speed Increment and Speed Decrement Signals |
| FR-4.2 | The control module shall, upon ECU Program Start, command the throttle speed request to be a calibrated minimum setpoint and shall hold this request for 6 seconds before accepting operator adjustments.  |
| FR-4.3 | The control module shall increase or decrease, depending on driving input, the engine RPM target at a rate of 100RPM per 100 ms. |
| FR-4.4 | The control module shall ensure mutual exclusivity when handling increment or decrement signals. |
| FR-4.5 | The control module shall output the updated Engine Request Speed to CAN after applying configurable ramping profiles to ensure smooth engine RPM adjustment |
| FR-4.6 | The control module shall set Engine Request Speed and Engine Override Control to zero when the engine is detected to be in OFF for an excess of 6 seconds |
| FR-4.7 | The control module shall populate and transmit all defined TSC1 message signals include the computed Engine Requested Speed. |

## FUNCTION 5: Vehicle FNR Motion Control

### *Functionality*

The FNR Control Module primarily controls vehicle propulsion in the forward and reverse directions by manipulating the control signal to an electronically controlled, bi-directional, proportional control valve (EFC) responsible for vehicle driving wheel rotation direction and speed.

| ID | Description |
| :---- | :---- |
| FR-5.1 | The control module shall calculate the Wheel Speed (in MPH) based on hardware feedback provided by a wheel mounted Speed Sensor (Frequency Signal). |
| FR-5.2 | The control module shall read the Main Grip Axis position of the joystick via CAN |
| FR-5.3 | The control module shall Enable EFC (permit movement in any direction) only when: Engine is ON ECU Application Program has been running for \> 4 seconds |
| FR-5.4 | The control module shall calculate the Target Speed command based on the Main Grip Axis position of the joystick. |
| FR-5.5 | The control module shall utilize the Main Grip Axis position of the joystick to determine “position state” of the joystick \- Forward, Reverse, or Neutral using predefined ranges / values. |
| FR-5.6 | The control module shall output via CAN message the status of Neutral State to identify when the joystick is in a neutral state. |
| FR-5.7 | The control module shall ramp the target speed command from previous value to new value using pre-configured maximum rates of change |
| FR-5.8 | The control module shall output to dual EFC valves (Forward valve and Reverse valve) based on the target speed command and the state of the joystick |
| FR-5.9 | The control module shall ensure mutual exclusivity between outputs to the forward valve and reverse valve |

4WS \- Software Helper Functions

## FUNCTION 6: PID Output Calculation

### *Functionality*

The PID Calculation Module determines a bounded actuator command by summing proportional, integral, and derivative responses to the difference between setpoint and measured feedback each control cycle. 

| ID | Description |
| :---- | :---- |
| FR-6.1 | The control module shall execute the PID output calculation as a cyclical function at the input parameter control rate.  |
| FR-6.2 | The control module shall compute control error as the difference between setpoint and measured feedback. |
| FR-6.3 | The control module shall compute proportional, integral, and derivative terms using input parameter gains and the control period.  |
| FR-6.4 | The control module shall sum the proportional, integral, and derivative terms to produce a control output.  |
| FR-6.5 | The control module shall limit the control output within input minimum and maximum bounds.  |

## FUNCTION 7: RAMP Value Calculation

### *Functionality*

The Ramp Value Calculation Module shall be used to control how quickly an actuator command is allowed to change over a specified time. 

| ID | Description |
| :---- | :---- |
| FR-7.1 | The control module shall execute the ramp value calculation as a cyclic function using the provided execution period input parameter.  |
| FR-7.2 | The control module shall compute an output that transitions linearly from the current output toward the command target value input parameter.  |
| FR-7.3 | The control module shall limit the rate of change of the output per execution cycle using the ramp rate and execution period input parameters.  |
| FR-7.4 | The control module shall update the ramped output each cycle until the output reaches the commanded target value and shall hold the output constant once reached.  |
| FR-7.5 | The control module shall constrain the ramped output within the provided minimum and maximum limit input parameters prior to output. |

## FUNCTION 8: Toggle Button Behavior

### *Functionality*

The Toggle Button Module shall debounce a momentary button press and toggle a latched output once per press, forcing a safe state when faulted. 

| ID | Description |
| :---- | :---- |
| FR-8.1 | The control module shall execute cyclic toggle logic that measures continuous button press time and shall toggle the latched output state only when the press duration is greater than or equal to the configured debounce time.  |
| FR-8.2 | The control module shall allow only one toggle per button press, shall require a button release before accepting a new press, and shall provide the latched state as the toggle output.  |

# **Functional Requirements Addendum — Joystick Control Mapping**

**Parent document:** `1_functional_requirements.md` **Scope:** ZZZ 4WS Sweeper Vehicle — operator joystick input assignments **Status:** Draft for review

## **Purpose**

The base Functional Requirements Document assigns behavior to *signals* (e.g. "Sweeper Drum Speed Request", "Neutral State") but does not define which *physical joystick control* produces each signal. This addendum closes that gap by consolidating the joystick input map into a single reference, derived from the joystick reference graphics (front/side views and the X/Y button grid) and reconciled against the existing FR-x.y requirements.

Grid coordinates below reference the joystick front-view button grid map (columns X0–X4, rows Y0–Y3).

## **Button-to-Function Map**

| Grid cell | Physical control | Assigned function | Related FRD requirements |
| ----- | ----- | ----- | ----- |
| X0,Y0 | Single button, upper row | PTO engage / disengage | FR-2.2, FR-3.3, FR-3.4 |
| X1,Y0 | Two-position rocker, upper row | Drum Boom Lift **and** Lower (both directions on the one rocker) | FR-1.1, FR-1.2, FR-1.3, FR-1.4, FR-1.5 |
| X2,Y0 | Top-center position | Unused | — |
| X3,Y0 | Linear slider / mini-joystick (0–100 range) | Sweeper Drum rotational speed command | FR-2.1, FR-2.3 |
| X4,Y0 | Single button, upper row | **Unassigned** (spare input — see AR-1) | — |
| X1,Y1 | Single button, lower row | Momentary throttle "bump": \+100 RPM per activation | FR-4.1, FR-4.3 |
| X2,Y2 | Index-finger trigger, rear face of grip | Sweeper Drum engage (hold-to-run) — see AR-2 | FR-2.4, FR-2.5, FR-7.x |
| X2,Y3 | Handle pivot datum | Vehicle FNR motion — joystick Y-axis | FR-5.2, FR-5.4, FR-5.5 |

### **Notes on selected controls**

**X1,Y0 — Drum Boom rocker.** Lift and Lower are commanded from a single two-position rocker rather than two independent buttons. This physically reinforces FR-1.2 (mutual exclusivity of Lift and Lower), since the rocker cannot occupy both positions simultaneously. The FR-1.5 inversion flag remaps which rocker direction commands Lift vs. Lower.

**X3,Y0 — Sweeper Drum speed slider.** This control behaves as a proportional linear input across a 0–100 range (a mini-joystick / slider), not a momentary button. It provides the operator "requested Sweeper Drum Speed" consumed by FR-2.1 and scaled/clamped per FR-2.3.

**X2,Y3 — FNR pivot datum.** Forward / Neutral / Reverse vehicle propulsion is mapped to the fore-aft pivot of the joystick handle (the joystick Y-axis), not to any button. This is the physical origin of the Main Grip Axis position referenced throughout Function 5\. Per the CAN interface, this axis is transmitted as a 10-bit value; the governing PGN is expected to be **64983 or 64982** (*to be confirmed against the joystick supplier's J1939 signal definition — see AR-3*).

## **Requirement Gaps Identified**

The following gaps were surfaced during the joystick mapping review and are logged here as new addendum requirements (AR-x) pending incorporation into the parent FRD.

### **AR-1: Spare Joystick Input (X4,Y0)**

| ID | Description |
| ----- | ----- |
| AR-1.1 | The joystick provides a physical button at grid position X4,Y0 that is currently unassigned. The control module shall reserve this input as a spare with no commanded function until a function is formally assigned. |
| AR-1.2 | The control module shall not produce any vehicle actuation as a result of X4,Y0 activation in its unassigned state. |

### **AR-2: Sweeper Drum Engage Trigger (X2,Y2)**

The base FRD (FR-2.4, FR-2.5) describes zeroing and ramping of the Sweeper Drum but does not identify the operator control that *gates* engagement, nor the release behavior. This requirement makes the trigger-driven engage/disengage behavior explicit.

| ID | Description |
| ----- | ----- |
| AR-2.1 | The control module shall treat the index-finger trigger (X2,Y2) as the Sweeper Drum engage command: the drum shall be commanded to run only while the trigger is depressed. |
| AR-2.2 | Upon trigger depression from a stopped (0 RPM) state, the control module shall apply a soft-start ramp to bring the Sweeper Drum from 0 RPM toward the commanded speed, using the ramp function (Function 7). |
| AR-2.3 | Upon trigger release, the control module shall ramp the Sweeper Drum from its current speed back to 0 RPM rather than stopping instantaneously. |
| AR-2.4 | Trigger-driven engagement shall remain subordinate to existing permissives — the drum shall not run when PTO Engaged is FALSE (per FR-2.4), regardless of trigger state. |

### **AR-3: FNR Axis CAN Definition (X2,Y3)**

| ID | Description |
| ----- | ----- |
| AR-3.1 | The control module shall read the vehicle FNR command from the joystick Y-axis (handle fore-aft pivot) as a 10-bit value received via CAN. |
| AR-3.2 | The governing J1939 PGN for the FNR axis shall be confirmed against the joystick supplier's signal definition. Candidate PGNs are 64983 and 64982; the correct PGN and signal start bit/length are **TBD** pending supplier documentation. |

## **Open Items Summary**

The following remain unresolved and are carried forward:

1. **X4,Y0 function** — physical spare input exists; no function assigned (AR-1).  
2. **FNR PGN confirmation** — 64983 vs. 64982, plus signal layout, unconfirmed (AR-3.2).  
3. **Engine ON/OFF indicator output** — carried over from the base FRD (FR-3.2 TBD): the joystick-mounted LED / status output destination is still undefined.  
4. **Assumption \#2** — the base FRD lists a bare "TBD" under Assumptions; content still required.

