//----------------------------------------------------------------------------------------------------------------------

/*
 * can_handler_lib.c
 *
 *  Created on: Jun 22, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdbool.h>
#include "stwtypes.h"
#include "stwerrors.h"

//PLATFORM
#include "stwtypes.h"
#include "x_can.h"

//APPLICATION
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

// Definition of application heap

// Global variables for main task configuration


/* -- Module Global Variables --------------------------------------------------------------------------------------- */
static bool maq_CanAvailable[X_CAN_COUNT];
T_CANDevices gt_can_devs;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 init_canInterfaces(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC implement can init

    return s16_error;
}

sint16 update_canInputs(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC implement input updater

    return s16_error;
}

sint16 update_canOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC implement output updater

    return s16_error;
}

bool can_get_availability_state(const uint16 ou16_Channel)
{
    return maq_CanAvailable[ou16_Channel];
}

// EOF

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Main routine */
//----------------------------------------------------------------------------------------------------------------------


