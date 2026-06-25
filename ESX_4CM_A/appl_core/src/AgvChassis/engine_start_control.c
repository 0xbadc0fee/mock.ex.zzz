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
#include "hw_inputs.h"
#include "hw_outputs.h"

/* -- Defines ------------------------------------------------------------------------------------------------------ */
/* -- Types -------------------------------------------------------------------------------------------------------- */
/* -- Function Prototypes ---------------------------------------------------------------------------- */
void check_engineStatus(void);
/* -- Module Global Variables -------------------------------------------------------------------------------------- */
static T_EngineControl mt_engine;

/* -- Implementation  ---------------------------------------------------------------------------------------------- */

sint16 init_engineStarterControl(T_CANDevices *_can_devs)
{
    //TODO_SGC Implement engineStarter initializer
    return C_NO_ERR;
}
;

sint16 update_engineStarterControl(void)
{
    sint16 s16_error = C_NO_ERR;
    //TODO_SGC Implement engineStarter updater

    return s16_error;
}

void get_engineStatus();

void get_engineRuntime();

void check_engineStatus();
//EOF
