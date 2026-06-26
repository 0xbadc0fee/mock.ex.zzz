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

    //TODO_SGC Safe to start when joystick_standby and operator_standby TRUE
    // FR-XX Compute Safe-to-Start Precondition(s)
    // precond: Joystick in neutral pos
    // precond: Engine is off / less than xxx rpm
    // precond: operator seat switch is true
    // precond: PTO is off

    // 1. Collect or compute all standby statuses

    // 1.1 Joystick
    get_standbyStatus(&mt_engine.u8_joystick_standby);

    // 1.2 Operator Presence

    // 1.3 Engine RPM

    // 1.4 Power Take Off

    // 2. Compute overall safe_start_standby value (0 = safe)
    mt_engine.u8_safe_start_standby = 0;
    mt_engine.u8_safe_start_standby += mt_engine.u8_joystick_standby;
    mt_engine.u8_safe_start_standby += mt_engine.u8_opp_present_standby;
    mt_engine.u8_safe_start_standby += mt_engine.u8_engineRpm_standby;
    mt_engine.u8_safe_start_standby += mt_engine.u8_pto_standby;

    return s16_error;
}

sint16 update_engineStarterControl(void)
{
    sint16 s16_error = C_NO_ERR;

    //uint8 u8_joystick_standby_status = 0; //!< local variable for joystick standby status reported by FNR
    //TODO_SGC Implement engineStarter updater

    // FR-XX Read HW input ignition key signal

    // FR-XX Read additionals inputs from internal modules

    // FR-XX Compute Safe-to-Start Precondition(s)
    // precond: Joystick in neutral pos
    // precond: Engine is off / less than xxx rpm
    // precond: operator seat switch is true
    // precond: PTO is off
    update_safeToStartStatus();

    // FR-XX Execute Engine Start when preconditions all met

    if (mt_engine.u8_start_key)
    {
        if (mt_engine.u8_safe_start_standby != 0)
        {
            //TODO_SGC engine start LOGIC
            mt_engine.u8_engine_start_cmd = FALSE;
            // set can tx value safe to start FALSE
        }
        else if (mt_engine.u8_safe_start_standby == 0)
            mt_engine.u8_engine_start_cmd = TRUE;
            // set can tx value safe to start FALSE

        else
            // ?? set warning, undefined start behavior??
            do {} while(0);
    }

    // FR-xx Transmit Safe-to-Start Status via CAN
    return s16_error;
}

void get_engineStatus();

void get_engineRuntime();

void check_engineStatus();
//EOF
