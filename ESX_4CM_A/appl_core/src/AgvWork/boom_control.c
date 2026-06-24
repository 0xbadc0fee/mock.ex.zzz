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
#include "stwerrors.h"
#include "stwtypes.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */
/* -- Global Variables ---------------------------------------------------------------------------------------------- */
/* -- Module Global Variables --------------------------------------------------------------------------------------- */
static T_BoomControl mt_boom;

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
    return s16_error;
}


sint16 update_boomControl(void)
{
    sint16 s16_error = C_NO_ERR;

    float32 f32_boom_limit_1 = 0;
    float32 f32_boom_limit_2 = 0;

    uint8 u8_boom_lift = FALSE;
    uint8 u8_boom_lower = FALSE;

    // TODO_SGC get CAN joystick inputs
    // TODO_SGC get HW inputs chassis / operator presence / boom limit switches

    // TODO_SGC check NVM for axis invert flag if need to swap rocker lift & lower buttons
    // TODO_SGC operate BOOM lift lower between MIN & MAX limit switch range

    return s16_error;
}


//EOF
