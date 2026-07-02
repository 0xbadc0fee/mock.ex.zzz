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
#include "system.h"

//PROJECT
#include "engine_start_control.h"
#include "FNR_control.h"
#include "hw_inputs.h"
#include "hw_outputs.h"
#include "can_engine.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ---------------------------------------------------------------------------- */
// void check_engineStatus(void); UNUSED IN ZZZ MOCK EXERCISE
sint16 check_engineStandby(void);
sint16 update_safeToStartStatus(void);

/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_EngineControl mt_engine;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

sint16 init_engineStarterControl(T_CANDevices *_can_devs)
{
    sint16 s16_error = C_NO_ERR;

    //TODO_SGC Implement engineStarter initializer

    //populate local struct with RX EEC1 inputs
    mt_engine.pu16_engine_speed = &_can_devs->t_engine.u16_engineSpeed;

    return s16_error;
}

sint16 update_safeToStartStatus(void)
{
    sint16 s16_error = C_NO_ERR;
    float32 f32_value = 0.0F;

    // FR-3.1, FR-3.4 Collect or compute all standby statuses
    s16_error += get_standbyStatus(&mt_engine.u8_joystick_standby);

    // read operator presence
    get_inputValue("OPERATOR_PRESENT", &f32_value);

    // read engine rpm
    s16_error += check_engineStandby();

    // FR-3.2 Engine RPM < 450
    mt_engine.u8_opp_present_standby = (f32_value != FALSE) ? TRUE : FALSE;

    // FR-3.3 Compute overall safe_start_standby value (0 = safe)
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

    // FR-3.4 Compute safe to start status
    update_safeToStartStatus();

    // FR-3.3 Execute Engine Start when preconditions all met
    if (mt_engine.u8_start_key)
    {
        if (mt_engine.u8_safe_start_standby != 0)
        {
            //TODO_SGC engine start LOGIC
            mt_engine.u8_engine_start_cmd = FALSE;
            // TODO_sgc set can tx value safe to start FALSE
        }
        else if (mt_engine.u8_safe_start_standby == 0)
            mt_engine.u8_engine_start_cmd = TRUE;
            // TODO_SGC set can tx value safe to start FALSE
    }

    // FR-3.5 Transmit Safe-to-Start Status via CAN
    // TODO_SGC can message

    return s16_error;
}

/*  UNUSED IN ZZZ MOCK EXERCISE

void get_engineStatus();
void get_engineRuntime();
void check_engineStatus();

*/

sint16 check_engineStandby()
{
    sint16 s16_error = C_NO_ERR;

    uint16 u16_curr_engine_speed = *mt_engine.pu16_engine_speed;  //dereference struct member pointer for rpm comparison
    s16_error = (u16_curr_engine_speed < 450) ? C_NO_ERR : C_RANGE;

    return s16_error;
}
//EOF
