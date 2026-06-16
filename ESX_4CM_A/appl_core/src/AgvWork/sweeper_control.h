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
        uint8 u8_speedFeedback;  // iffy, FRD doesn't indicate that an encoder signal is available or freq counter

        //tx can variables
        // TODO_SGC sweeper control struct - can tx
        // no current tx in this example, if fictional joystick had LED feedback though
        // like our old CFBX model, then a TX message could be sent to the joystick when
        // sweeper drum was on and show up as a red LED on top of the JSL joystick.  Will
        // leave this for a later exercise though as I no longer have the PGN data for that
        // kind of message without getting in contact with SureGrip.

        //rx can variables
        // TODO_SGC sweeper control struct - can rx
        uint8 *pu8_requestedSpeed;  //!<Sweeper requested speed from joystick
        uint8 *pu8_onOffCommand;    //!<Sweeper On/Off command from joystick

        //nvm configuration parameters
        // TODO_SGC sweeper control struct - nvm params
        T_Config_Sweeper *pt_nvmSweeper;     //!<Sweeper configuration variables (NVM)

        // control checkpoints
        // TODO_SGC sweeper control struct - checkpoints
        // Leaving these for later exercise.

}T_SweeperControl;


/* -- Global Variables ---------------------------------------------------------------------------------------------- */
sint16 init_sweeperControl();
sint16 update_sweeperControl();

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_sweeperControl(T_Config_Sweeper *_nvmSweeper);
sint16 update_sweeperControl(void);


#endif /* APPL_CORE_SRC_AGVWORK_SWEEPER_CONTROL_H_ */
