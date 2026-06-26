//-----------------------------------------------------------------------------
/*! \file       engine_starter_control.c
 \brief      <description>

 project     FloryTemplate_4CM
 copyright   STW Technic (c) 2026
 license     use only under terms of contract / confidential

 created     Jan 6, 2026 STW Technic
 */
//-----------------------------------------------------------------------------
/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdint.h>
#include "x_stdtypes.h"

//STW
#include "stwerrors.h"
#include "stwtypes.h"

//PROJECT
#include "engine_start_control.h"
#include "FNR_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ---------------------------------------------------------------------------- */
void check_engineStatus(void);
sint16 update_safeToStartStatus(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_EngineControl mt_engine;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

sint16 init_engineStarterControl(T_CANDevices *_can_devs)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC Implement engineStarter initializer

    return C_NO_ERR;
}

sint16 update_safeToStartStatus(void)
{
    sint16 s16_error = 0;

    return s16_error;
}

sint16 update_engineStarterControl(void)
{
    sint16 s16_error = C_NO_ERR;

    uint8 u8_joystick_standby_status = 0; //!< local variable for joystick standby status reported by FNR
    //TODO_SGC Implement engineStarter updater

    // FR-XX Read HW input ignition key signal

    // FR-XX Read additionals inputs from internal modules

    //get_standbyStatus(&u8)
    get_standbyStatus(u8_joystick_standby_status);

    // FR-XX Compute Safe-to-Start Precondition(s)
    // precond: Joystick in neutral pos
    // precond: Engine is off / less than xxx rpm
    // precond: operator seat switch is true
    // precond: PTO is off

    // FR-XX Execute Engine Start when preconditions all met

    // FR-xx Transmit Safe-to-Start Status via CAN
    return s16_error;
}

void get_engineStatus();

void get_engineRuntime();

void check_engineStatus();
//EOF
