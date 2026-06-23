//----------------------------------------------------------------------------------------------------------------------

/*
 * can_handler_lib.h
 *
 *  Created on: Jun 22, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------
#ifndef CAN_INIT_H_
#define CAN_INIT_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include <stdbool.h>

#include "can_device_definition.h"
#include "j1939_data_pool.h"

#include "SPN_Definitions.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */



/* -- Global Variables ---------------------------------------------------------------------------------------------- */
extern T_CANDevices gt_can_devs;

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_canInterfaces(void);
sint16 update_canInputs(void);
sint16 update_canOutputs(void);

bool can_get_availability_state(const uint16 ou16_Channel);
/* -- Implementation ------------------------------------------------------------------------------------------------ */



#endif // CAN_INIT_H
