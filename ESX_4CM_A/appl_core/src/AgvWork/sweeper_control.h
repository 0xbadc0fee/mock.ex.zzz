//----------------------------------------------------------------------------------------------------------------------

/*
 * sweeper_control.h
 *
 *  Created on: Jun 16, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_SWEEPER_CONTROL_H_
#define APPL_CORE_SRC_AGVwORK_SWEEPER_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
//#include "system.h"

#include "hmi_definition.h"
#include "can_device_definition.h"
#include "input_handler_lib.h"

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define SWEEPER_ON (1u)
#define SWEEPER_OFF (0u)

/* -- Types --------------------------------------------------------------------------------------------------------- */
/**
 * \brief Configuration Structure - ZZZ Sweeper Control
 *
 */
typedef struct
{
        sint16 s16_minRPM;
        sint16 s16_maxRPM;
}T_Config_Sweeper;

/**
 * \brief Control Structure - Sweeper Control
 *
 */
typedef struct
{
        // local control variables
        uint8 u8_onOffCommand;
        sint16 s16_speedCommand;
        sint16 s16_speedFeedback;  //<! iffy, FRD doesn't indicate that an encoder signal is available or freq counter

        // rx can variables
        sint16 *ps16_requestedSpeed;  //<! Sweeper requested speed from joystick
        uint8 *pu8_onOffCommand;    //<! Sweeper On/Off command from joystick

        // nvm configuration parameters
        T_Config_Sweeper *pt_nvmSweeper;     //<! Sweeper configuration variables (NVM)

}T_SweeperControl;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_sweeperControl(T_CANDevices *_can_dev, T_Config_Sweeper *_nvmSweeper);
sint16 update_sweeperControl(void);


#endif /* APPL_CORE_SRC_AGVWORK_SWEEPER_CONTROL_H_ */
