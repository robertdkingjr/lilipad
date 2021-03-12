/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "defines.h"

#include "appCfg.h"
#include "Wct_Lib.h"
#include "NVM.h"

#include "freemaster.h"

#include "hal.h"

#include "systemTask.h"
#include "systemPrint.h"
#include "systemDisplay.h"
#include "systemTimers.h"
#include "systemCallback.h"
#include "systemDebug.h"
#include "systemProtection.h"
#include "systemUSBPower.h"


static void VERSION_Print(void)
{
    LIB_Version lib_version;
    
    //Device Name
    APP_PrintString("\r\n");
    APP_PrintString(DEVICE_ID_STRING);
    
    //Application SW version
    APP_PrintString("\n\rWCT MP App SW Version: ");
    APP_PrintDoubleWordValue((WCT_SW_VERSION & 0xf000u)>>12);
    APP_PrintString(".");
    APP_PrintDoubleWordValue((WCT_SW_VERSION & 0x0f00u)>>8);
    APP_PrintString(".");
    APP_PrintDoubleWordValue(WCT_SW_VERSION & 0x00ffu);

    //Application HW version
    APP_PrintString("\n\rWCT MP HW Version: ");
    APP_PrintDoubleWordValue((WCT_HW_VERSION & 0xf000u)>>12);
    APP_PrintString(".");
    APP_PrintDoubleWordValue((WCT_HW_VERSION & 0x0f00u)>>8);
    APP_PrintString(".");
    APP_PrintDoubleWordValue(WCT_HW_VERSION & 0x00ffu);

    //SW Library version
    WCT_GetLibVer(&lib_version);
    APP_PrintString("\n\rWTC MP Lib Version: ");
    APP_PrintDoubleWordValue(lib_version.bMajorVersion);
    APP_PrintChar('.');
    APP_PrintDoubleWordValue(lib_version.bMinorVersion);
    APP_PrintChar('.');
    APP_PrintDoubleWordValue(lib_version.bSubVersion);
    
    APP_PrintString("\r\n");
}

void main(void)
{
	uint8 inputVol = 19;
	
    HAL_Init();
    
    //Wait to build the battery/board input voltage
    ST_WaitMs(100);

#if USB_POWER_ENABLE
    USB_PowerSetup(19000ul, 22000ul);
    ST_WaitMs(100);
#endif
    
    VERSION_Print();

#if FREEMASTER_SUPPORTED
    FMSTR_Init();
#endif
    DISP_Init();
    
    if(FALSE == NVM_ReadParams(1u))
    {
        uint8 i;
        
        WCT_Stop();
        App_SetErr(APP_NVM_ERROR);
        for(i=0; i<NUM_DEVICES; i++)
        {
            // For APP_NVM_ERROR, doesn't support LED blink display,
            // since no chance to call DISP_Handler
            WCT_UpdateDevUsrIndication(i);
        }
 
        while(1)
		{
			HAL_KickWatchDog();

#if FREEMASTER_SUPPORTED
			FMSTR_Poll();
#endif
		}
    }
    else
    {
        WCT_LibInit();
    }

    ST_WaitMs(50);

    while (1)
    {
        AppTask_Poll();

#if FREEMASTER_SUPPORTED
        FMSTR_Poll();
#endif
        
#if USB_POWER_ENABLE
        if(USB_TYPE_PD == USB_GetPowerType())
        {
        	USBPD_Task();
        }
#endif
    }
}

