//----------------------------------------------------------------------------------------------------------------------

/*
 * FNR_control.c
 *
 *  Created on: Jun 24, 2026
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
#include "FNR_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

// Definition of application heap

// Global variables for main task configuration


/* -- Module Global Variables --------------------------------------------------------------------------------------- */
T_FNRControl mt_fnr_control;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 init_FNRControl()
{
    sint16 s16_error = 0;


    return s16_error;
}

sint16 update_FRNControl()
{

    sint16 s16_error = 0;

    return s16_error;
}

sint16 get_standbyStatus(uint8 *_standby_status)
{
    sint16 s16_error = 0;
    *(_standby_status) = mt_fnr_control.u8_joystick_standby_status;
    return s16_error;
}

