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

    //populate local copy of NVM params
    mt_sweeper.pt_nvmSweeper = _nvmSweeper;

    //populate local copy of RX HMI elements
    mt_sweeper.pu8_onOffCommand = &_can_dev->t_joystickJSLX.u8_b1_state;
    mt_sweeper.ps16_requestedSpeed = &_can_dev->t_joystickJSLX.s16xPos;

    return s16_error;

}

/**
 * \brief Update AgvWork - Sweeper Control
 *
 * Primary run logic for rotary sweeper.  When permissible, run sweeper at requested speed.
 *
 * Safe operation defined by operator presence, and min - max rotation speed.
 *
 * \return s16_error Error Code
 * \retval C_NO_ERR Function Executed Properly
 */
sint16 update_sweeperControl(void)
{

    sint16 s16_error = C_NO_ERR;

    // TODO-SGC get inputs joystick
    // TODO-SGC get inputs chassis / operator presence
    // TODO-SGC run sweeper at desired speed until shut off

    return s16_error;
}

//EOF
