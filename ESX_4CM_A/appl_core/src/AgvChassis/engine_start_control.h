//----------------------------------------------------------------------------------------------------------------------
/*
 * engine_start_control.h
 *
 *  Created on: Jun 24, 2026
 *      Author: silas.curfman
 */

//----------------------------------------------------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include "x_stdtypes.h"

//STW
#include "stwerrors.h"
#include "stwtypes.h"
#include "can_device_definition.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifndef APPL_CORE_SRC_AGVCHASSIS_ENGINE_START_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_ENGINE_START_CONTROL_H_

/* -- Types --------------------------------------------------------------------------------------------------------- */

typedef struct
{
//RX CAN Variables
    uint16 *pu16_engine_speed;    //!<RX Engine Speed

//TX CAN Variables

//Local variables
    uint8 u8_engine_start_cmd;
    uint32 u32_engine_start_time;
    uint8 u8_engine_status;
    uint8 u8_prev_engine_status;

} T_EngineControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_engineStarterControl(T_CANDevices *_can_devs);
sint16 update_engineStarterControl(void);
void get_engineStatus();
void get_engineRuntime();

/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* APPL_CORE_SRC_AGVCHASSIS_ENGINE_START_CONTROL_H_ */
