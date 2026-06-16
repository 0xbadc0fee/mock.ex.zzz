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


/* -- Defines ------------------------------------------------------------------------------------------------------- */
#define SWEEPER_ON (1u)
#define SWEEPER_OFF (0u)

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
        uint8 u8_minSpeed;
        uint8 u8_maxSpeed;

}T_Config_Sweeper;

typedef struct
{
        //local control variables
        // TODO_SGC sweeper control struct - locals
        uint8 u8_onOffCommand;
        uint8 u8_speedCommand;
        uint8 u8_speedFeedback;

        //tx can variables
        // TODO_SGC sweeper control struct - can tx

        //rx can variables
        // TODO_SGC sweeper control struct - can rx
        uint8 *pu8_requestedSpeed;  //!<Sweeper requested speed from joystick
        uint8 *pu8_onOffCommand;    //!<Sweeper On/Off command from joystick

        //nvm configuration parameters
        // TODO_SGC sweeper control struct - nvm params
        T_Config_Sweeper *pt_nvmSweeper;     //!<Sweeper configuration variables (NVM)

        // control checkpoints
        // TODO_SGC sweeper control struct - checkpoints

}T_SweeperControl;


/* -- Global Variables ---------------------------------------------------------------------------------------------- */
sint16 init_sweeperControl();
sint16 update_sweeperControl();

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_sweeperControl(T_Config_Sweeper *_nvmSweeper);
sint16 update_sweeperControl(void);


#endif /* APPL_CORE_SRC_AGVWORK_SWEEPER_CONTROL_H_ */
