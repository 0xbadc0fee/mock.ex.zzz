//----------------------------------------------------------------------------------------------------------------------

/*
 * sweeper_control.c
 *
 *  Created on: Jun 16, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <stdint.h>
#include "x_stdtypes.h"


#include "stwerrors.h"
#include "stwtypes.h"

#include "sweeper_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

// Definition of application heap

// Global variables for main task configuration


/* -- Module Global Variables --------------------------------------------------------------------------------------- */
static T_SweeperControl mt_sweeper;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

/**
 * \brief Initialize AgvWork - Sweeper Control
 *
 * This function initializes the AgvWork - Sweeper Control Logic.
 *
 * \param _nvmSweeper
 *
 * \return s16_Error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_sweeperControl(T_CANDevices *_can_dev, T_Config_Sweeper *_nvmSweeper)
{
    sint16 s16_error = C_NO_ERR;

    // FR-2.3 populate local copy of NVM params for scaling signal
    mt_sweeper.pt_nvmSweeper = _nvmSweeper;

    //populate local copy of RX HMI elements
    mt_sweeper.pu8_onOffCommand = &_can_dev->t_joystickJSLX.u8_b1_state;

    // FR-2.1 Read requested speed from joystick slider via CAN
    mt_sweeper.ps16_requestedSpeed = &_can_dev->t_joystickJSLX.s16xPos;

    // FR-2.3 Read PTO Engagement status from PGN 64932 (PTODE) SPN 3942
    // FR-2.3 Read PTO Enable status from PGN 64932 (PTODE) SPN 3944

    return s16_error;

}

/**
 * \brief Update AgvWork - Sweeper Control
 *
 * Primary run logic for rotary sweeper.  When permissible, run sweeper at requested speed.
 *
 * Safe operation defined by operator presence, and min - max rotation speed.
 * Max and Min RPM is configured from NVM values and output is clamped within range.
 * Ramping is applied to output signal to create soft start affect.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 update_sweeperControl(void)
{

    sint16 s16_error =              C_NO_ERR;

    sint16 s16_req_sweeper_speed =  SWEEPER_SAFE_STATE;
    uint8 u8_req_sweeper_run =      SWEEPER_OFF;
    float32 f32_op_present =        SWEEPER_SAFE_STATE;

    // local copy of requested Sweeper speed
    s16_req_sweeper_speed = *(mt_sweeper.ps16_requestedSpeed);
    // local copy of requested Sweeper on/off command
    u8_req_sweeper_run = *(mt_sweeper.pu8_onOffCommand);
    // local copy of operator presence
    get_inputValue("OPERATOR_PRESENT", &f32_op_present);

    // local copy of PTO status
    // TBD




    // FR-2.3 Clamp and scale logic per config params

    // FR-2.4 Override req speed to zero when PTO off

    // FR-2.5 Apply time based ramp to smooth speed changes
    // BLOCKED by FR-7 RAMP

    // FR-2.6 Output to HW as PWM signal appropriate to sweeper coil specs

    return s16_error;
}

//EOF
