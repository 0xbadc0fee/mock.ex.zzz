//----------------------------------------------------------------------------------------------------------------------

/*
 * \file            boom_control.c
 * \brief           <description>
 *
 *  Created on: Jun 17, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"

//STW
#include "stwerrors.h"
#include "stwtypes.h"

//PROJECT
#include "boom_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Module Global Variables --------------------------------------------------------------------------------------- */
static T_BoomControl mt_boom;
static uint8 u8_invertLimitSwitches = 0;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

/**
 * \brief  Initialize AgvWork - Boom Control
 *
 * \param _ui
 * \param _nvmBoom
 *
 * \return s16_Error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 init_boomControl(T_CANDevices *_can_dev, T_Config_Boom *_nvmBoom)
{
    sint16 s16_error = C_NO_ERR;

    // populate local copy of NVM elements
    mt_boom.pt_nvmBoom = _nvmBoom;   //!< Boom config struct

    // FR-1.1 Map boom lift & boom lower commands to operator inputs
    if (mt_boom.pt_nvmBoom->u8_invertRocker == 0)
    {
        mt_boom.pu8_liftCommand = &_can_dev->t_joystickJSLX.u8_b2_state;
        mt_boom.pu8_lowerCommand = &_can_dev->t_joystickJSLX.u8_b3_state;
    }
    else
    // FR-1.5 Invert button mapping for boom lift & boom lower if NVM invert config is set
    {
        mt_boom.pu8_liftCommand = &_can_dev->t_joystickJSLX.u8_b3_state;
        mt_boom.pu8_lowerCommand = &_can_dev->t_joystickJSLX.u8_b2_state;
    }

    if (mt_boom.pt_nvmBoom->u8_invertLimitSwitchPos != 0)
    {
        // TODO_SGC map NVM config'd HW limit switches to local var
        u8_invertLimitSwitches = TRUE;
    }


    return s16_error;
}

/**
 * \brief Update AgvWork - Boom Control
 *
 * This function contains cyclical logic for AgvWork - Boom Control.
 *
 * Primary logic is to operator the boom arm that carries the rotating drum sweeper.
 * Operation is based on joystick commands, limit switch orientation, joystick rocker configuration
 * and operator presence.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function executed properly
 */
sint16 update_boomControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_boom_limit_lo = 0;
    float32 f32_boom_limit_hi = 0;

    uint8 u8_boom_lift_output = FALSE;
    uint8 u8_boom_lower_output = FALSE;

    // TODO_SGC map NVM config'd CAN joystick inputs to local control var
    mt_boom.u8_liftBoomCommand = *(mt_boom.pu8_liftCommand);
    mt_boom.u8_lowerBoomCommand = *(mt_boom.pu8_lowerCommand);

    // FR-1.3, FR-1.4 Utilize cylinder mounted lo / hi limit switches
    if(u8_invertLimitSwitches == TRUE)
    {
        get_inputValue("BOOM_LIMIT_1", &f32_boom_limit_hi);
        get_inputValue("BOOM_LIMIT_2", &f32_boom_limit_lo);
    }
    else
    {
        get_inputValue("BOOM_LIMIT_2", &f32_boom_limit_hi);
        get_inputValue("BOOM_LIMIT_1", &f32_boom_limit_lo);
    }

    // FR-1.1 Control lift / lower outpus based on operator input commands
    if(mt_boom.u8_liftBoomCommand)
    {
        // FR-1.2 Prevent simultaneous activation
        u8_boom_lower_output = FALSE;
        if(f32_boom_limit_hi)
            u8_boom_lift_output = FALSE;
        else
            u8_boom_lift_output = TRUE;
    }
    else if (mt_boom.u8_lowerBoomCommand)
    {
        // FR-1.2 Prevent simultaneous activation
        u8_boom_lift_output = FALSE;
        if(f32_boom_limit_lo)
            u8_boom_lower_output = FALSE;
        else
            u8_boom_lower_output = TRUE;
    }
    else
    {
        u8_boom_lift_output = FALSE;
        u8_boom_lower_output = FALSE;
    }

    //set the hardware output

    set_outputValue("BOOM_LIFT_COIL", (float32)u8_boom_lift_output);
    set_outputValue("BOOM_LOWER_COIL", (float32)u8_boom_lower_output);

    return s16_error;
}


//EOF
