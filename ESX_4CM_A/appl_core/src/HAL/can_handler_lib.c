//----------------------------------------------------------------------------------------------------------------------

/*
 * can_handler_lib.c
 *
 *  Created on: Jun 22, 2026
 *      Author: silas.curfman
 */


//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
//STD
#include <stdbool.h>

//PLATFORM - STW
#include "stwtypes.h"
#include "stwerrors.h"
#include "x_stdtypes.h"
#include "x_can.h"
#include "x_osf.h"
#include "osy_dph_data_pool_protector.h"

//APPLICATION - ZZZ Mock Controller
#include "can_device_definition.h"
#include "j1939_data_pool.h"


/* -- Defines ------------------------------------------------------------------------------------------------------- */
/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

// Definition of application heap

// Global variables for main task configuration


/* -- Module Global Variables --------------------------------------------------------------------------------------- */
static bool maq_CanAvailable[X_CAN_COUNT];
T_CANDevices gt_can_devs;

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

sint16 init_canInterfaces(void)
{
    sint16 s16_Error = C_NO_ERR;
    T_x_osf_can t_CanSettings;
    T_x_can_bus_config t_CanBusConfig =
    {
       .u32_DataBitrate = 0u,                  // Not used
       .u16_DataSamplePoint = 0u,              // Not used
       .u16_Protocol = X_CAN_BUS_PROTOCOL_CAN, // Classic CAN protocol
       .pr_FailureCallback = NULL,             // Not used
       .pv_CallbackParameter = NULL            // Not used
    };

    for (uint16 u16_Channel = 0u; (u16_Channel < X_CAN_COUNT) && (s16_Error == C_NO_ERR); u16_Channel++)
    {
       s16_Error = x_osf_can_get_param(u16_Channel, &t_CanSettings);
       if (s16_Error == C_NO_ERR)
       {
          t_CanBusConfig.u32_Bitrate = t_CanSettings.u32_Bitrate;
          t_CanBusConfig.u16_SamplePoint = t_CanSettings.u16_SamplePoint;
          s16_Error = x_can_bus_init(u16_Channel, &t_CanBusConfig);
          if (s16_Error == C_NOACT)
          {
             // Channel is not available for this HW variant
             s16_Error = C_NO_ERR;
          }
          else
          {
             maq_CanAvailable[u16_Channel] = true;
          }
       }
    }

    return s16_Error;

}

sint16 update_canInputs(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(J1939_DATA_POOL_INDEX);

    #define DP2CNTRL(name, CNTRL_VALUE, DPL_VALUE) VAR_ASSIGN((CNTRL_VALUE), (DPL_VALUE));
    #include "can_in_map.def"  //FIXME undefined reference to 'VAR_ASSSIGN'
    #undef DP2CNTRL

    (void)osy_dph_unlock_data_pool(J1939_DATA_POOL_INDEX);

    return s16_error;

}

sint16 update_canOutputs(void)
{
    sint16 s16_error = C_NO_ERR;

    (void)osy_dph_lock_data_pool(J1939_DATA_POOL_INDEX);

    #define CNTRL2DP(name, CNTRL_VALUE, DPL_VALUE) VAR_ASSIGN((DPL_VALUE), (CNTRL_VALUE));
    //#include "can_out_map.def"
    #undef CNTRL2DP

    (void)osy_dph_unlock_data_pool(J1939_DATA_POOL_INDEX);

    return s16_error;
}

bool can_get_availability_state(const uint16 ou16_Channel)
{
    return maq_CanAvailable[ou16_Channel];
}

// EOF

//----------------------------------------------------------------------------------------------------------------------
/*!  \brief   Main routine */
//----------------------------------------------------------------------------------------------------------------------


