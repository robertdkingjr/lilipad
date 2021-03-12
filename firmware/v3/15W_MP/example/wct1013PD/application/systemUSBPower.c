/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
//----------------------
// APPLICATION INCLUDES
//----------------------
#include "defines.h"
#include "appCfg.h"
#include "wct_lib.h"

#include "usb_pd_config.h"
#include "usb_pd.h"

#include "hal.h"
#include "systemPrint.h"
#include "systemDebug.h"
#include "systemTimers.h"
#include "systemUSBPower.h"

#if USB_POWER_ENABLE

#define USB_PD_CAP_DEBUG  1
#define USB_QC_DEBUG 1

uint32 gUSBP_dwPowerCapability;
USB_POWER_TYPE_E gUSBP_ePowerType = USB_TYPE_INIT;
uint32 gUSBP_dwQCVol[4] = {5000ul, 9000ul, 12000ul, 20000ul};

typedef void (*usbpd_power_change_callback_t)(void);

typedef enum _pd_request_value_type
{
    kRequestPower_Current,
    kRequestPower_Power
} pd_request_value_type_t;


typedef struct _pd_power_cap
{
    uint32_t minVoltage; /* unit 1mV */
    uint32_t maxVoltage; /* unit 1mV */
    uint32_t currentOrPower; /* current (unit is 1mA) or power (unit is 1mW) according to valueType */
    pd_power_type_t powerType;
} pd_power_cap_t;

#define PD_DEMO_PORT_CHARGE_POWER (60000) /* mW */
#define PD_DEMO_PORT_IRQn (0)
#define PD_DEMO_PTN5110_SLAVE_ADDR (0x51)
#define PD_DEMO_I2C_INSTANCE (0)
#define PD_APP_MAX_SINK_CAPS_NUM (5)
#define PD_APP_PARTNER_NO_PD_TIME_OUT_VALUE (800ul) /* 0.5s */
#define PD_APP_REQUEST_TIME_OUT_VALUE (500ul) /* 0.5s */

pd_status_t PD_PowerSnkDrawTypeCVbus(void *callbackParam, uint8_t typecCurrentLevel, uint8_t powerProgress);
pd_status_t PD_PowerSnkDrawRequestVbus(void *callbackParam, pd_rdo_t rdo);
static pd_status_t PD_PowerSnkStopDrawVbus(void *callbackParam, uint8_t powerProgress);
static pd_status_t PD_PowerSnkGotoMinReducePower(void *callbackParam);

pd_power_type_t g_pd_power_type = kSourceCap_FixedVoltageCurrent;

typedef struct _pd_app
{
	pd_handle pdHandle;
	usbpd_power_change_callback_t callbackFn;
	pd_sink_pdo_t sinkcaps[PD_APP_MAX_SINK_CAPS_NUM];
	uint32_t sinkCapsNum;
	volatile uint32_t timerValue;
    uint32_t expectedMinVoltage;
    uint32_t expectedMaxVoltage;
    uint32_t expectedPower; /* mW */
	pd_source_pdo_t partnerSourceCaps[7];
	pd_power_cap_t partnerVbusPowers[7];
	uint32_t partnerSourceCapsNum;
	pd_rdo_t requiredRDO;
	pd_power_cap_t vbusPower;
	uint8_t bootTypeC5VDone;
	uint8_t bootNegotiationDone;
	uint8_t requestNegotiationDone;
	uint8_t reserved[1]; /* for 4 bytes align */
} pd_app_t;

static void USBPD_NegotiationChange(pd_app_t *pdAppInstance);

volatile pd_app_t g_PDApp;

static pd_power_handle_callback_t callbackFunctions = {
	NULL,  NULL,  NULL,
	NULL, PD_PowerSnkDrawTypeCVbus, PD_PowerSnkDrawRequestVbus,
	PD_PowerSnkStopDrawVbus, PD_PowerSnkGotoMinReducePower, NULL,
};

static pd_power_port_config_t s_PortPowerConfig = {
    NULL,                                               /* source caps */
    NULL,            							        /* self sink caps */
    0,                                                  /* source cap count */
    0,                                                  /* sink cap count */
    kPowerConfig_SinkOnly,                              /* typec role */
    kCurrent_StdUSB,                                    /* source: Rp current level */
    kTypecTry_None,                                     /* drp try function */
    kDataConfig_None,                                   /* data function */
    0,                                                  /* support vconn */
    0,                                                  /* reserved */
    NULL,
    NULL,
    NULL,
};

pd_instance_config_t g_PortPDConfig = {
    kDeviceType_NormalPowerPort, /* normal power port */
    PD_DEMO_PORT_IRQn,           /* interrupt number */
    kPD_PhyPTN5110,
    PD_DEMO_I2C_INSTANCE,
    PD_DEMO_PTN5110_SLAVE_ADDR,
    &s_PortPowerConfig,
};

/***************sink need implement follow vbus power related functions***************/

static pd_status_t PD_PowerSnkDrawTypeCVbus(void *callbackParam, uint8_t typecCurrentLevel, uint8_t powerProgress)
{
    /* Type-C 5V vbus is OK
     */
	pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;
	pdAppInstance->timerValue = PD_APP_PARTNER_NO_PD_TIME_OUT_VALUE;
	pdAppInstance->bootTypeC5VDone = 1;

	pdAppInstance->vbusPower.powerType = kSourceCap_FixedVoltageCurrent;
	pdAppInstance->vbusPower.maxVoltage = 5000;
	pdAppInstance->vbusPower.minVoltage = 5000;
	PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_SNK_TYPEC_CURRENT_CAP, &(pdAppInstance->vbusPower.currentOrPower));
	if (pdAppInstance->vbusPower.currentOrPower == kCurrent_1A5)
	{
		pdAppInstance->vbusPower.currentOrPower = 1500;
	}
	else if (pdAppInstance->vbusPower.currentOrPower == kCurrent_3A)
	{
		pdAppInstance->vbusPower.currentOrPower = 3000;
	}
	else
	{
		pdAppInstance->vbusPower.currentOrPower = 900;
	}

    return kStatus_PD_Success;
}

static void PD_PowerGetVbusVoltage(uint32_t *partnerSourceCaps, pd_rdo_t rdo, pd_power_cap_t *vbusPower)
{
    pd_source_pdo_t pdo;

    if (partnerSourceCaps == NULL)
    {
        return;
    }

    pdo.PDOValue = partnerSourceCaps[rdo.bitFields.objectPosition - 1];
    switch (pdo.commonPDO.pdoType)
    {
        case kPDO_Fixed:
            vbusPower->minVoltage = pdo.fixedPDO.voltage * 50ul;
            vbusPower->maxVoltage = pdo.fixedPDO.voltage * 50ul;
            vbusPower->currentOrPower = rdo.bitFields.operateValue * 10ul;
            vbusPower->powerType = kSourceCap_FixedVoltageCurrent; /* current */
            break;

        case kPDO_Battery:
            vbusPower->minVoltage = pdo.batteryPDO.minVoltage * 50ul;
            vbusPower->maxVoltage = pdo.batteryPDO.maxVoltage * 50ul;
            vbusPower->currentOrPower = rdo.bitFields.operateValue * 250ul;
            vbusPower->powerType = kSoruceCap_VariableVoltagePower; /* power */
            break;

        case kPDO_Variable:
            vbusPower->minVoltage = pdo.variablePDO.minVoltage * 50ul;
            vbusPower->maxVoltage = pdo.variablePDO.maxVoltage * 50ul;
            vbusPower->currentOrPower = rdo.bitFields.operateValue * 10ul;
            vbusPower->powerType = kSoruceCap_VariableVoltageCurrent; /* current */
            break;

        case kPDO_APDO:
            vbusPower->minVoltage = rdo.programRDOBitFields.outputVoltage * 20ul;
            vbusPower->maxVoltage = rdo.programRDOBitFields.outputVoltage * 20ul;
            vbusPower->currentOrPower = rdo.programRDOBitFields.operatingCurrent * 50ul; /* 1mA unit */
            vbusPower->powerType = kSourceCap_RangeVoltageCurrent; /* current */
            break;

        default:
            break;
    }
}

static pd_status_t PD_PowerSnkDrawRequestVbus(void *callbackParam, pd_rdo_t rdo)
{
	pd_power_cap_t vbusPower;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    PD_PowerGetVbusVoltage((uint32_t*)&pdAppInstance->partnerSourceCaps[0], pdAppInstance->requiredRDO, &pdAppInstance->vbusPower);

    return kStatus_PD_Success;
}

static pd_status_t PD_PowerSnkStopDrawVbus(void *callbackParam, uint8_t powerProgress)
{
	/* nothing need do, the board will power off */
    return kStatus_PD_Success;
}

static pd_status_t PD_PowerSnkGotoMinReducePower(void *callbackParam)
{
    /* in normal situation, only current is reduced,
       voltage don't change, so don't need any operation */
    return kStatus_PD_Success;
}

/***************sink need implement above vbus power related functions***************/
static void PD_CapsInit(pd_app_t *pdAppInstance, uint32_t minVol, uint32_t maxVol, uint32_t power)
{
    uint32_t tempValue;
    uint8_t objIndex;

    /* sink capabilities */
    /* PDO1: fixed:5.0V */
    pdAppInstance->sinkcaps[0].PDOValue = 0;
    pdAppInstance->sinkcaps[0].fixedPDO.fixedSupply = kPDO_Fixed;
    pdAppInstance->sinkcaps[0].fixedPDO.dualRoleData = 0;
    pdAppInstance->sinkcaps[0].fixedPDO.dualRolePower = 0;
    pdAppInstance->sinkcaps[0].fixedPDO.externalPowered = 0;
#if ((defined PD_CONFIG_REVISION) && (PD_CONFIG_REVISION >= PD_SPEC_REVISION_30))
    pdAppInstance->sinkcaps[0].fixedPDO.frSwapRequiredCurrent = kFRSwap_CurrentDefaultUSB;
#endif
    if (minVol > 5000)
    {
        pdAppInstance->sinkcaps[0].fixedPDO.higherCapability = 1;
    }
    pdAppInstance->sinkcaps[0].fixedPDO.usbCommunicationsCapable = 0;
    pdAppInstance->sinkcaps[0].fixedPDO.voltage = (5 * 1000 / 50);
    tempValue = power * 1000 / 5; /* mA */
    if (tempValue > 3000)
    {
        tempValue = 3000;
    }
    pdAppInstance->sinkcaps[0].fixedPDO.operateCurrent = (tempValue / PD_PDO_CURRENT_UNIT);

    objIndex = 1;
    if ((minVol <= 9000) && (maxVol >= 9000))
    {
    	/* PDO: fixed: 9V */
    	pdAppInstance->sinkcaps[objIndex].PDOValue = 0;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.fixedSupply = kPDO_Fixed;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.voltage = (9 * 1000 / 50);
		tempValue = power * 1000 / 9; /* mA */
		if (tempValue > 3000)
		{
			tempValue = 3000;
		}
		pdAppInstance->sinkcaps[objIndex].fixedPDO.operateCurrent = (tempValue / PD_PDO_CURRENT_UNIT);
		objIndex++;
    }
    
    if ((minVol <= 15000) && (maxVol >= 15000))
	{
		/* PDO: fixed: 15V */
		pdAppInstance->sinkcaps[objIndex].PDOValue = 0;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.fixedSupply = kPDO_Fixed;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.voltage = (15 * 1000 / 50);
		tempValue = power * 1000 / 15; /* mA */
		if (tempValue > 3000)
		{
			tempValue = 3000;
		}
		pdAppInstance->sinkcaps[objIndex].fixedPDO.operateCurrent = (tempValue / PD_PDO_CURRENT_UNIT);
		objIndex++;
	}

    if ((minVol <= 20000) && (maxVol >= 20000))
	{
		/* PDO: fixed: 20V */
		pdAppInstance->sinkcaps[objIndex].PDOValue = 0;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.fixedSupply = kPDO_Fixed;
		pdAppInstance->sinkcaps[objIndex].fixedPDO.voltage = (20 * 1000 / 50);
		tempValue = power * 1000 / 20; /* mA */
		if (tempValue > 5000)
		{
			tempValue = 5000;
		}
		pdAppInstance->sinkcaps[objIndex].fixedPDO.operateCurrent = (tempValue / PD_PDO_CURRENT_UNIT);
		objIndex++;
	}

    s_PortPowerConfig.sinkCaps = (uint32_t*)(&pdAppInstance->sinkcaps[0]);
    s_PortPowerConfig.sinkCapCount = objIndex;
    pdAppInstance->sinkCapsNum = objIndex;
}

static void PD_AppClearState(pd_app_t *pdAppInstance)
{
	uint32_t *dataPrt = (uint32_t*)&(pdAppInstance->partnerSourceCaps[0]);
	while (dataPrt < (uint32_t*)(&pdAppInstance->bootNegotiationDone))
	{
		*dataPrt = 0;
		dataPrt++;
	}
	pdAppInstance->requiredRDO.rdoVal = 0;
	pdAppInstance->timerValue = PD_APP_PARTNER_NO_PD_TIME_OUT_VALUE;
}

static void PD_AppReset(pd_app_t *pdAppInstance)
{
	PD_AppClearState(pdAppInstance);
	USBPD_NegotiationChange(pdAppInstance);
}

/***************sink power negotiation related functions below***************/

static pd_status_t PD_DpmReceivePartnerSrcCapsCallback(void *callbackParam, pd_capabilities_t *caps)
{
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;
    uint8_t index;

    pdAppInstance->partnerSourceCapsNum = caps->capabilitiesCount;
    for (index = 0; index < caps->capabilitiesCount; ++index)
    {
    	pd_source_pdo_t pdo;
        pdAppInstance->partnerSourceCaps[index].PDOValue = caps->capabilities[index];
        pdAppInstance->partnerVbusPowers;

		pdo.PDOValue = pdAppInstance->partnerSourceCaps[index].PDOValue;
		switch (pdo.commonPDO.pdoType)
		{
			case kPDO_Fixed:
				pdAppInstance->partnerVbusPowers[index].minVoltage = pdo.fixedPDO.voltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].maxVoltage = pdo.fixedPDO.voltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].currentOrPower = pdo.fixedPDO.maxCurrent * 10ul;
				pdAppInstance->partnerVbusPowers[index].powerType = kSourceCap_FixedVoltageCurrent; /* current */
				break;

			case kPDO_Battery:
				pdAppInstance->partnerVbusPowers[index].minVoltage = pdo.batteryPDO.minVoltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].maxVoltage = pdo.batteryPDO.maxVoltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].currentOrPower = pdo.batteryPDO.maxAllowPower * 250ul;
				pdAppInstance->partnerVbusPowers[index].powerType = kSoruceCap_VariableVoltagePower; /* power */
				break;

			case kPDO_Variable:
				pdAppInstance->partnerVbusPowers[index].minVoltage = pdo.variablePDO.minVoltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].maxVoltage = pdo.variablePDO.maxVoltage * 50ul;
				pdAppInstance->partnerVbusPowers[index].currentOrPower = pdo.variablePDO.maxCurrent * 10ul;
				pdAppInstance->partnerVbusPowers[index].powerType = kSoruceCap_VariableVoltageCurrent; /* current */
				break;

			case kPDO_APDO:
				pdAppInstance->partnerVbusPowers[index].minVoltage = pdo.apdoPDO.minVoltage * 100ul;
				pdAppInstance->partnerVbusPowers[index].maxVoltage = pdo.apdoPDO.maxVoltage * 100ul;
				pdAppInstance->partnerVbusPowers[index].currentOrPower = pdo.apdoPDO.maxCurrent * 50ul; /* 1mA unit */
				pdAppInstance->partnerVbusPowers[index].powerType = kSourceCap_RangeVoltageCurrent; /* current */
				break;

			default:
				break;
		}
    }

    return kStatus_PD_Success;
}

static pd_status_t PD_DemoFindPDO(pd_app_t *pdAppInstance, pd_rdo_t *rdo)
{
    uint32_t index;
    pd_source_pdo_t sourcePDO;
    volatile uint32_t requestCurrent;
    volatile uint32_t objVoltage;
    volatile uint32_t currentVoltage;

    if (pdAppInstance->partnerSourceCapsNum == 0)
    {
        return kStatus_PD_Error;
    }

    /* default rdo as 5V - 0.9A */
    rdo->bitFields.objectPosition = 1;
    rdo->bitFields.giveBack = 0;
    rdo->bitFields.capabilityMismatch = 1;
    rdo->bitFields.usbCommunicationsCapable = 0;
    rdo->bitFields.noUsbSuspend = 1;
    rdo->bitFields.operateValue = pdAppInstance->partnerSourceCaps[0].fixedPDO.maxCurrent;
    rdo->bitFields.maxOrMinOperateValue = rdo->bitFields.operateValue;

    return kStatus_PD_Success;
}

static pd_status_t PD_DpmSnkGetRequestRDOCallback(void *callbackParam, pd_rdo_t *rdo)
{
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    if (pdAppInstance->requiredRDO.rdoVal != 0)
    {
    	rdo->rdoVal = pdAppInstance->requiredRDO.rdoVal;
    }
    else
    {
        PD_DemoFindPDO(pdAppInstance, rdo);
        pdAppInstance->requiredRDO.rdoVal = rdo->rdoVal;
    }

    return kStatus_PD_Success;
}

static pd_status_t PD_DpmSnkRDOResultCallback(void *callbackParam, uint8_t success, uint8_t failResultType)
{
	if (success)
	{
		pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

		USBPD_NegotiationChange(pdAppInstance);
		pdAppInstance->bootNegotiationDone = 1;
		pdAppInstance->requestNegotiationDone = 1;
	}
    return kStatus_PD_Success;
}

static pd_status_t PD_DpmSoftResetCallback(void *callbackParam)
{
	return kStatus_PD_Success;
}

static pd_status_t PD_DpmDemoAppCallback(void *callbackParam, uint32_t event, void *param)
{
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;
    pd_status_t status = kStatus_PD_Success;

    switch (event)
    {
        case PD_DISCONNECTED:
        case PD_FUNCTION_DISABLED:
            /* need hard or software reset */
        	PD_AppReset(pdAppInstance);
            break;

        case PD_CONNECTED:
        case PD_CONNECT_ROLE_CHANGE:
        	/* nothing need do */
            break;

		/* hard reset */
		case PD_DPM_SNK_HARD_RESET_REQUEST:
		case PD_DPM_SRC_HARD_RESET_REQUEST:
			PD_AppReset(pdAppInstance);
			break;

        case PD_DPM_SNK_RECEIVE_PARTNER_SRC_CAP:
            status = PD_DpmReceivePartnerSrcCapsCallback(callbackParam, (pd_capabilities_t *)param);
            break;
        case PD_DPM_SNK_GET_RDO:
            status = PD_DpmSnkGetRequestRDOCallback(callbackParam, (pd_rdo_t *)param);
            break;
        case PD_DPM_SNK_RDO_SUCCESS:
            status = PD_DpmSnkRDOResultCallback(callbackParam, 1, 0);
            break;
        case PD_DPM_SNK_RDO_FAIL:
            status = PD_DpmSnkRDOResultCallback(callbackParam, 0, *((uint8_t *)param));
            break;

        case PD_DPM_SNK_GOTOMIN_SUCCESS:
        case PD_DPM_SNK_GOTOMIN_FAIL:
            break;

		/* soft reset */
		case PD_DPM_SOFT_RESET_SUCCESS:
		case PD_DPM_SOFT_RESET_REQUEST:
			status = PD_DpmSoftResetCallback(callbackParam);
			break;
		case PD_DPM_SOFT_RESET_FAIL:
			/* in normal situation, app don't need process it PD stack will do hard_reset if soft_reset fail. */
			break;

		default:
			break;
    }
    return status;;
}
/***************sink power negotiation related functions above***************/

static void PD_AppInit(pd_app_t *pdAppInstance, uint32_t minVol, uint32_t maxVol, uint32_t power)
{
    uint8_t index;

    PD_CapsInit(pdAppInstance, minVol, maxVol, power);
    pdAppInstance->pdHandle = NULL;
    pdAppInstance->expectedMinVoltage = minVol;
    pdAppInstance->expectedMaxVoltage = maxVol;
    pdAppInstance->expectedPower = power;
    PD_AppClearState(pdAppInstance);
    if (PD_InstanceInit(&pdAppInstance->pdHandle, PD_DpmDemoAppCallback, &callbackFunctions, pdAppInstance,
                        &g_PortPDConfig) != kStatus_PD_Success)
    {
        return;
    }
    HAL_EnableUSBPDIrq(TRUE);
    ST_WaitMs(2);   //wait here to let the polling ISR to get handled firstly
}

static void USBPD_Setup(uint32_t minVol, uint32_t maxVol, uint32_t requiredPower, usbpd_power_change_callback_t callbackFn)
{
	HAL_USBPDInit();
	PD_AppInit((pd_app_t*)&g_PDApp, minVol, maxVol, requiredPower);
	g_PDApp.callbackFn = callbackFn;
	/* wait boot up PD negotiation done */
	while ((!g_PDApp.bootNegotiationDone) &&
			(g_PDApp.timerValue != 0))
	{
		PD_InstanceTask(g_PDApp.pdHandle);
		HAL_KickWatchDog();
	}
	PD_InstanceTask(g_PDApp.pdHandle);
	PD_InstanceTask(g_PDApp.pdHandle);
#if USB_PD_CAP_DEBUG
	if ((g_PDApp.timerValue == 0) && (!g_PDApp.bootNegotiationDone))
	{
		APP_PrintString("\r\nUSB PD setup time out");
	}
#endif
}

void USBPD_TickHandler(void)  //1ms
{
	if (g_PDApp.timerValue > 0)
	{
		g_PDApp.timerValue--;
	}
	PD_TimerIsrFunction(g_PDApp.pdHandle);
}

void USBPD_Task(void)
{
	PD_InstanceTask(g_PDApp.pdHandle);
}

void USBPD_PHYIsr(void)
{
	PD_PTN5110IsrFunction(g_PDApp.pdHandle);
}

static void USBPD_NegotiationChange(pd_app_t *pdAppInstance)
{
	if(pdAppInstance->callbackFn)
	{
		pdAppInstance->callbackFn();
	}
}

static void USBPD_GetPartnerSourceCaps(pd_power_cap_t **vbusPowerArray, uint8_t *count)
{
	*vbusPowerArray = (pd_power_cap_t*)&g_PDApp.partnerVbusPowers[0];
	*count = g_PDApp.partnerSourceCapsNum;
}

static uint8_t USBPD_RequestPower(uint8_t requestIndex, uint32_t requestVoltage, uint32_t requestCurrentOrPower)
{
	pd_rdo_t rdo;

	if ((requestIndex == 0) || (requestIndex > g_PDApp.partnerSourceCapsNum))
	{
		return 0;
	}

	/* default rdo bits value */
	rdo.rdoVal = 0u;
	rdo.bitFields.giveBack = 0;
	rdo.bitFields.capabilityMismatch = 0;
	rdo.bitFields.usbCommunicationsCapable = 0;
	rdo.bitFields.noUsbSuspend = 1;
	rdo.bitFields.objectPosition = requestIndex;
	rdo.programRDOBitFields.unchunkedSupported = 0;

	switch (g_PDApp.partnerSourceCaps[requestIndex - 1].commonPDO.pdoType)
	{
		case kPDO_Fixed:
		case kPDO_Variable:
			rdo.bitFields.operateValue = requestCurrentOrPower / PD_PDO_CURRENT_UNIT;
			rdo.bitFields.maxOrMinOperateValue = rdo.bitFields.operateValue;
			break;

		case kPDO_Battery:
			rdo.bitFields.operateValue = requestCurrentOrPower / PD_PDO_POWER_UNIT;
			rdo.bitFields.maxOrMinOperateValue = rdo.bitFields.operateValue;
			break;

		case kPDO_APDO:
			rdo.programRDOBitFields.outputVoltage = requestVoltage / 20;
			rdo.programRDOBitFields.operatingCurrent = requestCurrentOrPower / 50;
			break;

		default:
			break;
	}

	g_PDApp.requiredRDO.rdoVal = rdo.rdoVal;
	g_PDApp.requestNegotiationDone = 0;
	g_PDApp.timerValue = PD_APP_REQUEST_TIME_OUT_VALUE;
	if (PD_Command(g_PDApp.pdHandle, PD_DPM_CONTROL_REQUEST, (pd_rdo_t*)&g_PDApp.requiredRDO) != kStatus_PD_Success)
	{
		return 0;
	}
	/* wait request done */
	while ((!g_PDApp.requestNegotiationDone) &&
			(g_PDApp.timerValue != 0))
	{
		PD_InstanceTask(g_PDApp.pdHandle);
	}
	PD_InstanceTask(g_PDApp.pdHandle);
	PD_InstanceTask(g_PDApp.pdHandle);

	return g_PDApp.requestNegotiationDone;
}

uint8_t USBPD_GetSupportState(void)
{
	if (g_PDApp.partnerSourceCapsNum > 0)
	{
		return 1;
	}
	return 0;
}

static void USB_PDCapChanged(void)
{

}

static void USB_SetQCVol(uint32 setVol)
{
	switch(setVol)
	{
	case 5000:
		HAL_SetDataPVol(HAL_USB_DATA_LINE_06);
		HAL_SetDataNVol(HAL_USB_DATA_LINE_00);
		break;
	case 9000:
		HAL_SetDataPVol(HAL_USB_DATA_LINE_33);
		HAL_SetDataNVol(HAL_USB_DATA_LINE_06);
		break;
	case 12000:
		HAL_SetDataPVol(HAL_USB_DATA_LINE_06);
		HAL_SetDataNVol(HAL_USB_DATA_LINE_06);
		break;
	case 20000:
		HAL_SetDataPVol(HAL_USB_DATA_LINE_33);
		HAL_SetDataNVol(HAL_USB_DATA_LINE_33);
		break;
	default:
	break;
	
	}
}

pd_power_cap_t *sourceCaps;
uint8_t sourceCapCount = 0;

void USB_PDPowerSwitch(uint32 dwTargetVol, uint32 dwMaxVol, boolean bNoPPS)
{
	uint8 index, validIndex=0xFF;
	uint32 requestVol=0, requestPower=0;
	sint32 volDiff, requestVolDiff = -((sint32)dwTargetVol);
#if USB_PD_CAP_DEBUG
	APP_PrintString("\r\nUSB PD CapCount:");
	APP_PrintDecChar(sourceCapCount);
#endif
	
	gUSBP_ePowerType = USB_TYPE_PD;
	for (index = 0; index < sourceCapCount; ++index)
	{
#if USB_PD_CAP_DEBUG
		APP_PrintString("\r\n  Index:");
		APP_PrintDecChar(index);
		APP_PrintString(", type:");
		APP_PrintDecChar(sourceCaps[index].powerType);
		APP_PrintString(", maxVol:");
		APP_PrintDoubleWordValue(sourceCaps[index].maxVoltage);
		APP_PrintString(", minVol:");
		APP_PrintDoubleWordValue(sourceCaps[index].minVoltage);
		APP_PrintString(", PC:");
		APP_PrintDoubleWordValue(sourceCaps[index].currentOrPower);			
		ST_WaitMs(100);
#endif
		switch (sourceCaps[index].powerType)
		{
		case kSourceCap_FixedVoltageCurrent:
			if(sourceCaps[index].minVoltage <= dwMaxVol)
			{
				volDiff = (sint32)sourceCaps[index].minVoltage - (sint32)dwTargetVol;
				if( ((volDiff > requestVolDiff) && (requestVolDiff < 0)) ||
				   ((volDiff < requestVolDiff) && (requestVolDiff > 0) && (volDiff >= 0)) )
				{
					requestVolDiff = volDiff;
					validIndex = index;
					requestVol = sourceCaps[index].minVoltage;
					requestPower = sourceCaps[index].currentOrPower;
					requestPower *= requestVol;
					requestPower /= 1000ul;
				}
			}
			break;
			
		case kSoruceCap_VariableVoltageCurrent:
		case kSoruceCap_VariableVoltagePower:
			if( (sourceCaps[index].minVoltage <= dwMaxVol) &&
					(sourceCaps[index].maxVoltage <= dwMaxVol) )
			{
				sint32 minDiff, maxDiff;
				minDiff = (sint32)sourceCaps[index].minVoltage - (sint32)dwTargetVol;
				maxDiff = (sint32)sourceCaps[index].maxVoltage - (sint32)dwTargetVol;
				
				if(sourceCaps[index].minVoltage >= dwTargetVol)
				{
					volDiff = maxDiff;
				}
				else
				{
					volDiff = minDiff;
				}
				
				if( ((volDiff > requestVolDiff) && (requestVolDiff < 0)) ||
					((volDiff < requestVolDiff) && (requestVolDiff > 0) && (volDiff >= 0)) )
				{
					requestVolDiff = volDiff;
					validIndex = index;
					requestVol = 0;
					if(sourceCaps[index].powerType == kSoruceCap_VariableVoltagePower)
					{
						requestPower = sourceCaps[index].currentOrPower;
					}
					else
					{
						requestPower  = sourceCaps[index].currentOrPower;
						requestPower *= sourceCaps[index].maxVoltage;
						requestPower /= 1000ul;
					}
				}
			}
			break;

		case kSourceCap_RangeVoltageCurrent:
			if (bNoPPS)
			{
				continue;
			}
			if( (sourceCaps[index].minVoltage <= dwTargetVol) &&
				(sourceCaps[index].maxVoltage >= dwTargetVol) && 
				(requestVolDiff != 0) )
			{
				validIndex = index;
				requestVol = dwTargetVol;
				requestVolDiff = 0;
				requestPower = sourceCaps[index].currentOrPower;
				requestPower *= requestVol;
				requestPower /= 1000ul;
			}
			else if(sourceCaps[index].maxVoltage <= dwTargetVol)
			{
				volDiff = (sint32)sourceCaps[index].maxVoltage - (sint32)dwTargetVol;
				if( ((volDiff > requestVolDiff) && (requestVolDiff < 0)) ||
					((volDiff < requestVolDiff) && (requestVolDiff > 0) && (volDiff >= 0)) )
				{
					requestVolDiff = volDiff;
					validIndex = index;
					requestVol = sourceCaps[index].maxVoltage;
					requestPower = sourceCaps[index].currentOrPower;
					requestPower *= requestVol;
					requestPower /= 1000ul;
				}
			}
			else if( (sourceCaps[index].minVoltage <= dwMaxVol) && 
					 (sourceCaps[index].minVoltage >= dwTargetVol) )
			{
				volDiff = (sint32)sourceCaps[index].minVoltage - (sint32)dwTargetVol;
				if( ((volDiff > requestVolDiff) && (requestVolDiff < 0)) ||
					((volDiff < requestVolDiff) && (requestVolDiff > 0) && (volDiff >= 0)) )
				{
					requestVolDiff = volDiff;
					validIndex = index;
					requestVol = sourceCaps[index].minVoltage;
					requestPower  = sourceCaps[index].currentOrPower;
					requestPower *= requestVol;
					requestPower /= 1000ul;
				}
			}				
			break;
		}
	}
#if USB_PD_CAP_DEBUG
	APP_PrintString("\r\n  ValidIndex:");
	APP_PrintDecChar(validIndex);
	APP_PrintString(", requestVol:");
	APP_PrintDoubleWordValue(requestVol);
	APP_PrintString(", requestPower:");
	APP_PrintDoubleWordValue(requestPower);
	ST_WaitMs(100);
#endif
	if(validIndex != 0xFF)
	{	
		USBPD_RequestPower((validIndex + 1), requestVol, sourceCaps[validIndex].currentOrPower);
		gUSBP_dwPowerCapability = requestPower;
		g_pd_power_type = sourceCaps[validIndex].powerType;
	}

}

void USB_PowerSetup(uint32 dwTargetVol, uint32 dwMaxVol)
{
	uint32_t voltage;
	
	DBG_Assert(dwTargetVol > dwMaxVol, 0, 0);
	DBG_Assert(dwMaxVol <= 5000ul, 0, 0);
	
	/* The voltage range that the device need for work/run.
	 * The PD partner can get this information to know the device's wanted voltage.
	 * it is better to be the actual values. */
	USBPD_Setup(5000ul, dwTargetVol, 60000ul, USB_PDCapChanged);

	/* set voltage alarm */
	voltage = (5000ul) | (20000ul << 16);
	PD_Control(g_PDApp.pdHandle, PD_CONTROL_INFORM_VBUS_VOLTAGE_RANGE, &voltage); 
		
	USBPD_GetPartnerSourceCaps(&sourceCaps, &sourceCapCount);

	//set the voltage as close as possible to dwRequestVol
	if(0 == sourceCapCount)
	{
		uint16 wInputVol = HAL_GetBatteryVoltage();

		if(wInputVol > 6500)
		{
			//seems not USB input
			gUSBP_ePowerType = USB_TYPE_NONE;
			gUSBP_dwPowerCapability = wInputVol * 3ul;	//assume maximum 3A current
			APP_PrintString("\r\nExternal power supply\r\n");
		}
		else
		{
	#if USB_QC_DEBUG
			APP_PrintString("\r\nMaybe QC adaptor\r\n");
	#endif
			HAL_USBDataLineInit();
			HAL_SetDataPVol(HAL_USB_DATA_LINE_06);
			ST_WaitMs(1600);
			if(HAL_USB_DATA_LINE_00 == HAL_CheckDataNVol())
			{	
				uint8 i, validIndex=0;
				gUSBP_ePowerType = USB_TYPE_QC;
				HAL_USBDataLineDNInit();
				for(i=0; i<(sizeof(gUSBP_dwQCVol)/sizeof(gUSBP_dwQCVol[0])); i++)
				{
					if(gUSBP_dwQCVol[i] <= dwMaxVol)
					{
#if USB_QC_DEBUG
						APP_PrintString("\r\nTry QC:");
						APP_PrintDoubleWordValue(gUSBP_dwQCVol[i]);
#endif
						USB_SetQCVol(gUSBP_dwQCVol[i]);
						ST_WaitMs(100);
						wInputVol = HAL_GetBatteryVoltage();
						if( (wInputVol < (gUSBP_dwQCVol[i]-1000ul)) || (wInputVol > (gUSBP_dwQCVol[i]+1000ul)))
						{
							if(i < 2)
							{
								gUSBP_ePowerType = USB_TYPE_NORMAL;
							}
							break;
						}
						else
						{
							validIndex = i;
							if(gUSBP_dwQCVol[i] >= dwTargetVol)
							{	
								break;
							}
						}
					}
					else
					{
						if(i < 2)
						{
							gUSBP_ePowerType = USB_TYPE_NORMAL;
						}
						break;
					}
				}
				
				USB_SetQCVol(gUSBP_dwQCVol[validIndex]);
				if(USB_TYPE_QC == gUSBP_ePowerType)
				{
#if USB_QC_DEBUG
					APP_PrintString("\r\nValid QC voltage:");
					APP_PrintDoubleWordValue(gUSBP_dwQCVol[validIndex]);
#endif
					gUSBP_dwPowerCapability = gUSBP_dwQCVol[validIndex] * 3ul / 2ul; //assume 1.5A current
				}
				else
				{
					DBG_Assert(USB_TYPE_NORMAL != gUSBP_ePowerType, 0, 0);
					APP_PrintString("\r\nNot QC adaptor!\r\n");
					gUSBP_dwPowerCapability = gUSBP_dwQCVol[0] * 1ul; //assume 1A current
				}
			}
			else
			{
				gUSBP_ePowerType = USB_TYPE_NORMAL;
				gUSBP_dwPowerCapability = 5000ul;  //assume maximum 1A current
				APP_PrintString("\r\nNormal USB power\r\n");
			}
		}
	}
	else
	{
		USB_PDPowerSwitch(dwTargetVol, dwMaxVol, 0);
	}
}

uint32 USB_GetPowerCap(void)
{
	return gUSBP_dwPowerCapability;
}

USB_POWER_TYPE_E USB_GetPowerType(void)
{
	return gUSBP_ePowerType;
}

pd_power_type_t USBGetPDType(void)
{
	return  g_pd_power_type;
}

#endif

