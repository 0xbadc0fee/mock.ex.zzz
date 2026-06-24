//----------------------------------------------------------------------------------------------------------------------

/*
 * boom_control.h
 *
 *  Created on: Jun 17, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------
#ifndef APPL_CORE_SRC_AGVWORK_BOOM_CONTROL_H_
#define APPL_CORE_SRC_AGVWORK_BOOM_CONTROL_H_

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD

//PLATFORM - STW
#include "stwtypes.h"

//APPLICATION
#include "can_device_definition.h"
#include "hmi_definition.h"
#include "input_handler_lib.h"


/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define BOOM_LIFT (1u)
#define BOOM_LOWER (0u)

/* -- Types ------------------------------------------------------------------------------------------------------- */

/**
 * \brief NVM Configuration Parameter(s) for Boom Arm
 *
 */
typedef struct
{
        uint8 u8_invertRocker;
        uint8 u8_invertLimitSwitchPos;
}T_Config_Boom;

typedef struct
{
        // local variables
        uint8 u8_liftBoomCommand;
        uint8 u8_lowerBoomCommand;
        uint8 u8_limitLower;
        uint8 u8_limitUpper;

        // rx can variables
        uint8 *pu8_liftCommand;  //<! Boom Lift Command from joystick
        uint8 *pu8_lowerCommand; //<! Boom Lower Command from joystick


        // tx can variables
        // NA

        // nvm params
        T_Config_Boom *pt_nvmBoom;  //<! Boom configuration parameters

        // diag check points
        // NA

}T_BoomControl;

/* -- Global Variables ---------------------------------------------------------------------------------------------- */


/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_boomControl(T_CANDevices *_can_dev, T_Config_Boom *_nvmBoom);
sint16 update_boomControl(void);


#endif /* APPL_CORE_SRC_AGVWORK_BOOM_CONTROL_H_ */
