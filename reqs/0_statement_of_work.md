# **Statement of Work**

## Software Development \- Vehicle Control Application

## \[2026.01.01\]

| Customer: | ZZZ Corp (fictional) |
| ----: | :---- |
| **Contact(s):** | \[Name\] \+ \[Email\] |
| **Industry:** | Agriculture |
| **Machine / Model:**  | 4WS Sweeper |

# General

ZZZ is an OEM of mobile self propelled, off-highway, work vehicles.  The 4WS Sweeper Vehicle is a proven production unit which is currently operated via hardwired relay logic.

ZZZ has determined that reliance on hard wired relay logic is no longer a viable approach for this product and has contacted STW requesting a hardware and software solution to replace the current relay logic.

# Challenges

The primary concerns that will need to be addressed in any solutions are:

1. Existing vehicle control logic is not documented in a written source but rather implied from wiring and relay diagrams.  
2. On completion of the project, ZZZ Corp is requesting STW provide all relevant Software Development artifacts for use in repeating the application development process in-house on the remainder of their fleet.

# Recommended Solution

STW Technic is proposing an Engineering Services engagement in that will:

1. Define suitable controller hardware to replace signal handling the relay system is currently responsible for.  
2. Develop a software application for the selected hardware that will perform all the duties currently handled by the relay logic.  
3. Generate project documentation that will both validate the performance of the newly developed vehicle application as well as provide a roadmap for future development of similar applications.  

## Hardware

On review of the existing vehicle design; STW recommends replacing the hardware relay assembly with a single STW.4cm controller.

## Software

On STW’s recommendation, the customer has requested that the new vehicle application be produced as a C based application rather than a graphical IEC type application.  ZZZ Corp is relatively new to software development and has left it to STW to design the application as they see fit.

## Other

To meet the customers requirement for project artifacts and documentation that can serve as a roadmap for future development; STW will apply the document driven development process outlined in the following resources (SDLC V-Model Process):

* [FPC-145 Software Conversion Guide](https://docs.google.com/document/d/1Zu5lr45WqHaHK9xlcrc6Jy72ZBd9QmHx93benE2sjY4/edit?usp=sharing)  
  * [Software Development Process](https://docs.google.com/presentation/d/1qX7ly5FXWN_aSn4WYektgyszeitFX-DjHeluKPeyuKs/edit?usp=sharing)	  
  * [Developer Training - Using Architecture](https://docs.google.com/presentation/d/1Weo-7u-uZ0EZIv0m-1RNEmnwHeT1h2XeVP9xNOxfT0I/edit?usp=sharing)  
  * [Developer Training - Architecture Exploration](https://docs.google.com/presentation/d/1r_e-uvnmcD_mXsJl2Y3orEibpCEl9safjDPKM12aaU0/edit?usp=sharing)

Process generated and consumed documents will be captured and included with the final project deliverables.

# Project Initial Conditions

## Vehicle \- Physical

The 4WS Sweeper Vehicle is an operator controlled, wheeled vehicle with the following relevant features:

* Primary power provided by a Tier 4 Diesel Engine  
* Auxiliary power is provided by a Hydraulic Pump driven by the Engine and controlled by PTO  
* (2) Work devices are powered by the Auxiliary power system: a rotating drum sweeper and a boom for lifting and lowering the drum sweeper.

*4WS Sweeper Vehicle (fictional)*  
![4WS Vehicle Side View][./graphic_01_4WS_Sweeper_Vehicle.svg]

## Vehicle Operation \- Overall

All Operator interactions with the 4WS Vehicle can be defined in some combination of the following in-cab HMI components:

* Operate presence is sensed within the cab.  
* Operator controls running state of the diesel engine via an automotive style key and switch.  
* Operator control of both the vehicle and the work systems is translated by a single Multi-Axis Joystick with a number of buttons, rockers, sliders, and triggers attached at points along the joystick grip.


*Operator Controls*  
![4WS Cab Controls Detail View][./graphic_02_4WS_detailView_cabControls.svg]

## Vehicle Operation \- Joystick

Currently, operators control the behavior of the 4WS Sweeper Vehicle by manipulation of a joystick with the features described below.  Replication of the behaviors described below will be one of the thresholds for final acceptance testing on completion of the project.  An identical replacement for the original joystick (relay logic) has been sourced and comes supplied with J1939 capability for use in the new application.

The most conspicuous action of the joystick is a forward and backward control stick pivoted at its base that returns to center when released. Moving upward from the base toward the top, the additional features are:

* A momentary trigger operated by the index finger of the right hand  
* Four small circular momentary buttons on the hat of the joystick  
* One 3-position rocker switch on top of the joystick  
* One thumb-operated slider with a friction hold; oriented vertically with approximately 1 inch of travel between lowest and highest position

*Operator Joystick \- Overview*  
![two-view orthographic drawing (front view & right view) of Suregrip JSM style joystick.][./graphic_03_joystick_sideView_fwd_neutral_rev.svg]

The main grip axis uses a signed integer scale. \-100 represents full reverse, 0 represents dead center neutral, and \+100 represents full forward.

*Operator Joystick \- Y Axis FNR*
![rh side view of Suregrip JSM style joystick in Full Reverse, Neutral, & Full Forward states.][./graphic_04_joystick_twoView_front_and_right.svg]

Each physical control is identified by a grid coordinate. Y0.X0 is the top-left of the joystick from the operator point of view and Y3.X2 is the main grip axis at the base.

*Operator Joystick \- Button ID Grid*  
![Front-view grid-box layout of Suregrip JSM joystick custom inputs such as momentary buttons, sliders, & rockers.][./graphic_05_joystick_frontView_button_gridMap.svg]

The individual Joystick buttons or features and their expected vehicle behavior outputs are listed below.  Each is identified by its unique grid position illustrated in the diagram above.

- Y0.X0 — Momentary button: Acts as a toggle for PTO engagement and disengagement.  
- Y0.X1 — 3-position RTC rocker switch: Moving to position 1 lowers the sweeper boom; position 2 is neutral; position 3 raises the sweeper boom.  
- Y0.X3 — Friction hold slider: Defines requested sweeper speed; 0% at lowest travel represents minimum request, while 100% at highest travel represents maximum request.  
- Y0.X4 — Momentary button: Currently unassigned; maintained for future system expansions.  
- Y1.X1 — Momentary button: Increases engine RPM setpoint by 100 RPM for each complete press and release cycle.  
- Y1.X3 — Momentary button: Decreases engine RPM setpoint by 100 RPM for each complete press and release cycle.  
- Y2.X2 — Momentary trigger (index finger): While depressed, the sweeper ramps to requested duty cycle; rotation ceases and signal returns to zero upon release.  
- Y3.X2 — Main grip axis, return to center: Facilitates vehicle propulsion; pushing past center initiates forward travel, while pulling past center initiates reverse travel, with speed proportional to deflection.

## Vehicle Operation \- Misc

The ignition switch behaves identically to a traditional consumer automobile ignition. Partial key rotation powers 12V devices. Full rotation engages the starter motor. No custom behavior is required beyond the startup sequence constraints defined in ~~Section 9\.~~

The operator cab door is fitted with a door ajar warning switch assumed to be present on the target vehicle and its existing wiring. When the operator door is closed the switch is in closed position and the input signal reads 12VDC. This signal is used as the operator presence signal for all logic in this application that requires an operator presence condition.

# Project Scope

| In Scope |  | Out of Scope |  |
| ----- | :---- | ----- | :---- |
| Item 1 | Reverse Engineering, as needed, existing vehicle behavior into written specifications | Item 1 | RTOS |
| Item 2 | Platform configuration | Item 2 | Fault Handling |
| Item 3 | Architecture layout | Item 3 | Functional Safety |
| Item 4 | Module / Unit Design | Item 4 | Standards Compliance |
| Item 5 | Testing | Item 5 | Vehicle Braking / Immobilizing |
| Item 6 | Code Documentation | Item 6 | Vehicle Steering |
|  |  | Item 7 | NVM / EEPROM Handling |

# Functional Requirements

Include here any business level Functional Requirements.  System level Functional Requirements should be reserved for the Functional Requirements Document (FRD).  

| *INTERNAL NOTE*:  If helpful, distinguish between the two based on the following: BRD: Business FR’s ask what the stakeholders require FRD: Technical FR’s ask what the system requires NOTE: Both stop short of stating how they should be implemented. |
| :---- |

| ID | Requirement | Priority | Ref |
| ----- | ----- | ----- | ----- |
| B-FR-01 | The customer shall be able to deploy and update the Vehicle Control Application using standard tooling, without requiring STW engagement. |  |  |
| B-FR-02 | Equivalent Manual Operation:  The Vehicle Control Application shall cause the same vehicle behaviors and outputs driving by operator input as the currently designed relay system provides. |  |  |
| B-FR-03 | Equivalent Automatic Operation:  The Vehicle Control Application shall cause the same automated system behaviors as ensured by the current design, such as: Automatic start-up sequence Automatic shut-down sequence Automatic operator safety interlocking etc |  |  |
| B-FR-04 | The Vehicle Application Source Code shall use within its documentation a naming convention that clearly maps to existing naming used to describe the current Vehicle and its features. |  |  |
| B-FR-05 | The Vehicle Control Application shall receive and transmit all CAN/J1939 messages required to replicate the communication behavior of the existing relay-based design, including but not limited to engine control, status PGN’s, and joystick input messages |  |  |

# Nonfunctional Requirement

Where Functional Requirements fail on presence or absence of a feature, Nonfunctional Requirements identify qualities, attributes, or performance thresholds the system must meet. 

| ID | Requirement |
| ----- | ----- |
| NFR-01 | TIME: The Vehicle Application logic shall complete all operations within a span of time that deviates no more than 500ms from the same interactions when observed on the existing design. |
| NFR-02 | DISPLACEMENT: The Vehicle Application logic shall cause all displacement based interactions to complete their actions within plus or minus 10% of the same displacement that would have been observed in the original system.  For example, given the same operator inputs the new system should not deviate by more than 10% in: Vehicle distance traveled per joystick action Total rotations of drum sweeper for a given ON time Total height of boom traveled for a given Lift command. |

# Estimate of Required Effort

~~\[ Provide a detailed estimate of the effort and time to complete the project tasks\]~~  
~~Example:~~

* ~~Design: 2 days (completed by:)~~  
* ~~Application Development~~   
  * ~~Feature 1: 2-4 days~~  
  * ~~Feature 2: 4-5 days~~  
* ~~Testing:~~  
  * ~~1 day~~

~~TOTAL: 10 days~~

# Proposed Schedule

\[Describe milestones and timelines\]

# Project Prerequisites

\[Specify information or materials needed by STW Technic in order to develop the solution described in this document.  This may include documentation or specifications from the customer or existing source code\]

# Project Deliverables

\[Specify materials that will be delivered to the customer as an acceptable completion package.  This may include build instructions, source code, documentation, test reports, etc.\]

# Assumptions

\[Record any assumptions STW Technic team is making regarding the project and the review of the information provided by the customer. These assumptions must be corroborated and agreed on by the customer\]

Project Pricing Recommendation  
\[Put any pricing recommendations \- based on effort required, value add to customer, ask difficulty etc. for Sales and Engineering to consider when choosing a final pricing strategy.\]  

