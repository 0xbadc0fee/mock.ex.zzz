//----------------------------------------------------------------------------------------------------------------------
/*
 * FNR_control.h
 *
 *  Created on: Jun 24, 2026
 *      Author: silas.curfman
 */

//----------------------------------------------------------------------------------------------------------------------


/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* -- Defines ------------------------------------------------------------------------------------------------------- */
#ifndef APPL_CORE_SRC_AGVCHASSIS_FNR_CONTROL_H_
#define APPL_CORE_SRC_AGVCHASSIS_FNR_CONTROL_H_

/* -- Types --------------------------------------------------------------------------------------------------------- */
typedef struct
{
        // local control variables
        uint8 u8_joystick_standby_status;  //!< Joystick in standby state when y-axis @ 0 position and all buttons inactive

        // TX can variables

        // RX can variables


}T_FNRControl;
/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------------------------------------------------- */
sint16 init_FNRControl();
sint16 update_FNRControl();
sint16 get_standbyStatus(uint8 *_standby_status);
/* -- Implementation ------------------------------------------------------------------------------------------------ */

#endif /* APPL_CORE_SRC_AGVCHASSIS_FNR_CONTROL_H_ */
