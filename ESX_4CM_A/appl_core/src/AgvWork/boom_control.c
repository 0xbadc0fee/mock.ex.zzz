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

    // populate local copy of
    mt_boom.pu8_liftCommand = &_can_dev->t_joystickJSLX.u8_b2_state;
    mt_boom.pu8_lowerCommand = &_can_dev->t_joystickJSLX.u8_b3_state;

    // populate local NVM config
    mt_boom.pt_nvmBoom = _nvmBoom;   //!< Boom config flags, invert rocker

    return s16_error;
}


sint16 update_boomControl(void)
{
    sint16 s16_error = C_NO_ERR;

    // TODO_SGC get joystick inputs
    // TODO_SGC get inputs chassis / operator presence / boom limit switches
    // TODO_SGC check for axis invert flag if need to swap rocker lift & lower buttons
    // TODO_SGC operate boom lift lower between MIN & MAX limit switch range

    return s16_error;
}


//EOF
