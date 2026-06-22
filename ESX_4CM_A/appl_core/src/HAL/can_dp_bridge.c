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


//PLATFORM
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_can.h"
#include "x_osf.h"

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

    //TODO_SGC implement can init.  Follow pattern from user manual example in 19.5.1
    // 1. configure a can bus -- x_can_bus_init
    // 2. create object handle for init'd can bus -- x_can_obj_init

    T_x_osf_can t_OsfCanSettings;           // settings object
    T_x_can_bus_config t_CanBusConfig =     // config object, initialize some of the fields
    {
        .u32_DataBitrate = 0u,
        .u16_DataSamplePoint = 0u,
        .u16_Protocol = X_CAN_BUS_PROTOCOL_CAN,
        .pr_FailureCallback = NULL,
        .pv_CallbackParameter = NULL
    };

    return s16_error;
}

sint16 update_canInputs(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC implement input updater
    // 3. query the number of rx'd CAN messages in the buffer with -- x_can_obj_get_buf_count
    // 4. read CAN messages from buffer -- x_can_obj_get_msg
    // 5. send CAN message with -- x_can_obj_send_msg
    // 6. OPT, check bus status -- x_can_bus_get_status
    // 7. OPT, check CAN message object status -- x_can_obj_get_status

    return s16_error;
}

sint16 update_canOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC implement output updater
    // 1. configure a can bus -- x_can_bus_init
    // 2. create object handle for init'd can bus -- x_can_obj_init
    // 3. query the number of rx'd CAN messages in the buffer with -- x_can_obj_get_buf_count
    // 4. read CAN messages from buffer -- x_can_obj_get_msg
    // 5. send CAN message with -- x_can_obj_send_msg
    // 6. OPT, check bus status -- x_can_bus_get_status
    // 7. OPT, check CAN message object status -- x_can_obj_get_status

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


