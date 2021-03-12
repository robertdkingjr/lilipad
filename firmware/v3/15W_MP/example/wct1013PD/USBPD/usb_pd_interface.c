/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_phy.h"
#include "usb_pd_timer.h"
#include "usb_pd_interface.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void PD_MsgInit(pd_instance_t *pdInstance);
void PD_MsgReceived(pd_instance_t *pdInstance, uint32_t msgLength, pd_status_t result);
void PD_MsgSendDone(pd_instance_t *pdInstance, pd_status_t result);
void PD_StackStateMachine(pd_instance_t *pdInstance);
void PD_ConnectSetPowerProgress(pd_instance_t *pdInstance, uint8_t state);
void HAL_EnableUSBPDIrq(boolean isEn);
pd_status_t PD_PhyControl(pd_instance_t *pdInstance, uint32_t control, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static pd_instance_t s_PDInstance[PD_CONFIG_MAX_PORT];

#if ((defined PD_CONFIG_PTN5100_PORT) && (PD_CONFIG_PTN5100_PORT))
static const pd_phy_api_interface_t s_PTN5100Interface = {
    PDphy_PTN5100Init, PDphy_PTN5100Deinit, PDphy_PTN5100Send, PDphy_PTN5100Receive, PDphy_PTN5100Control,
};
#endif /* PD_CONFIG_PTN5100_PORT */

#if ((defined PD_CONFIG_PTN5110_PORT) && (PD_CONFIG_PTN5110_PORT))
static const pd_phy_api_interface_t s_PTN5110Interface = {
    PDPTN5110_Init, PDPTN5110_Deinit, PDPTN5110_Send, PDPTN5110_Receive, PDPTN5110_Control,
};
#endif /* PD_CONFIG_PTN5110_PORT */

/*******************************************************************************
 * Code
 ******************************************************************************/
static pd_instance_t *PD_GetInstance(void)
{
    uint8_t i = 0;
    uint32_t j;
    USB_OSA_SR_ALLOC();
    USB_OSA_ENTER_CRITICAL();
    for (; i < PD_CONFIG_MAX_PORT; i++)
    {
        if (s_PDInstance[i].occupied != 1)
        {
            uint8_t *buffer = (uint8_t *)&s_PDInstance[i];
            for (j = 0U; j < sizeof(pd_instance_t); j++)
            {
                buffer[j] = 0x00U;
            }
            s_PDInstance[i].occupied = 1;
            USB_OSA_EXIT_CRITICAL();
            return &s_PDInstance[i];
        }
    }
    USB_OSA_EXIT_CRITICAL();
    return NULL;
}

static void PD_ReleaseInstance(pd_instance_t *pdInstance)
{
    USB_OSA_SR_ALLOC();
    USB_OSA_ENTER_CRITICAL();
    pdInstance->occupied = 0;
    USB_OSA_EXIT_CRITICAL();
}

static void PD_GetPhyInterface(uint8_t phyType, const pd_phy_api_interface_t **controllerTable)
{
#if ((defined PD_CONFIG_PTN5100_PORT) && (PD_CONFIG_PTN5100_PORT))
    if (phyType == kPD_PhyPTN5100)
    {
        *controllerTable = &s_PTN5100Interface;
    }
#endif
#if ((defined PD_CONFIG_PTN5110_PORT) && (PD_CONFIG_PTN5110_PORT))
    if (phyType == kPD_PhyPTN5110)
    {
        *controllerTable = &s_PTN5110Interface;
    }
#endif
}

pd_status_t PD_PhyControl(pd_instance_t *pdInstance, uint32_t control, void *param)
{
    if ((control == PD_PHY_UPDATE_STATE) ||
        (USB_OsaEventCheck(pdInstance->taskEventHandle, PD_TASK_EVENT_PHY_STATE_CHAGNE, NULL) ==
         kStatus_USB_OSA_Success))
    {
        USB_OsaEventClear(pdInstance->taskEventHandle, PD_TASK_EVENT_PHY_STATE_CHAGNE);
        pdInstance->phyInterface->pdPhyControl(pdInstance->pdPhyHandle, PD_PHY_UPDATE_STATE, NULL);
        HAL_EnableUSBPDIrq(1);
    }

    if (control != PD_PHY_UPDATE_STATE)
    {
        return pdInstance->phyInterface->pdPhyControl(pdInstance->pdPhyHandle, control, param);
    }
    else
    {
        return kStatus_PD_Success;
    }
}

pd_status_t PD_InstanceInit(pd_handle *pdHandle,
                            pd_stack_callback_t callbackFn,
                            pd_power_handle_callback_t *callbackFunctions,
                            void *callbackParam,
                            pd_instance_config_t *config)
{
    pd_instance_t *pdInstance;
    pd_status_t status = kStatus_PD_Success;
    pd_phy_config_t phyConfig;

    pdInstance = PD_GetInstance();
    if (pdInstance == NULL)
    {
        return kStatus_PD_Error;
    }

    /* get phy API table */
    pdInstance->phyInterface = NULL;
    PD_GetPhyInterface(config->phyType, &pdInstance->phyInterface);
    if ((pdInstance->phyInterface == NULL) || (pdInstance->phyInterface->pdPhyInit == NULL) ||
        (pdInstance->phyInterface->pdPhyDeinit == NULL) || (pdInstance->phyInterface->pdPhySend == NULL) ||
        (pdInstance->phyInterface->pdPhyReceive == NULL) || (pdInstance->phyInterface->pdPhyControl == NULL))
    {
        PD_ReleaseInstance(pdInstance);
        return kStatus_PD_Error;
    }
    pdInstance->phyType = config->phyType;
    pdInstance->pdCallback = callbackFn;
    pdInstance->callbackFns = callbackFunctions;
    pdInstance->callbackParam = callbackParam;
    pdInstance->revision = PD_CONFIG_REVISION;
    pdInstance->sendingMsgHeader.bitFields.specRevision = pdInstance->revision;
    pdInstance->initializeLabel = 0;
    pdInstance->waitTime = PD_WAIT_EVENT_TIME;
    pdInstance->curConnectState = TYPEC_DISABLED;
    pdInstance->partnerSourcePDOsCount = 0;

    if (kStatus_USB_OSA_Success != USB_OsaEventCreate(&(pdInstance->taskEventHandle), 0))
    {
        PD_ReleaseInstance(pdInstance);
        return kStatus_PD_Error;
    }

    /* initialize PHY */
    pdInstance->pdPhyHandle = NULL;
    pdInstance->pdConfig = config;
    if (pdInstance->pdConfig->deviceType == kDeviceType_NormalPowerPort)
    {
        pdInstance->pdPowerPortConfig = (pd_power_port_config_t *)pdInstance->pdConfig->deviceConfig;
        pdInstance->pendingSOP = kPD_MsgSOPMask;
    }
    else if (pdInstance->pdConfig->deviceType == kDeviceType_Cable)
    {
        pdInstance->pendingSOP = kPD_MsgSOPpMask | kPD_MsgSOPppMask;
    }
    else
    {
        pdInstance->pendingSOP = kPD_MsgSOPMask;
    }
    phyConfig.interface = config->phyInterface;
    phyConfig.interfaceParam = config->interfaceParam;
    status = pdInstance->phyInterface->pdPhyInit(pdInstance, &(pdInstance->pdPhyHandle), &phyConfig);
    if ((status != kStatus_PD_Success) || (pdInstance->pdPhyHandle == NULL))
    {
        USB_OsaEventDestroy(pdInstance->taskEventHandle);
        PD_ReleaseInstance(pdInstance);
        return kStatus_PD_Error;
    }
    /* initialize pd stack */
    PD_MsgInit(pdInstance);
    PD_TimerInit(pdInstance);

    PD_PhyControl(pdInstance, PD_PHY_GET_PHY_VENDOR_INFO, &pdInstance->phyInfo);

    *pdHandle = pdInstance;
    return kStatus_PD_Success;
}

pd_status_t PD_InstanceDeinit(pd_handle pdHandle)
{
    pd_instance_t *pdInstance = (pd_instance_t *)pdHandle;
    pd_status_t status = kStatus_PD_Success;

    USB_OsaEventDestroy(pdInstance->taskEventHandle);
    PD_ReleaseInstance(pdInstance);
    status = pdInstance->phyInterface->pdPhyDeinit(pdInstance->pdPhyHandle);
    PD_ReleaseInstance(pdInstance);

    return status;
}

void PD_InstanceTask(pd_handle pdHandle)
{
    pd_instance_t *pdInstance = (pd_instance_t *)pdHandle;

    if (pdHandle == NULL)
    {
        return;
    }
    PD_StackStateMachine(pdInstance);
}

void PD_Notify(pd_handle pdHandle, uint32_t event, void *param)
{
    pd_instance_t *pdInstance = (pd_instance_t *)pdHandle;

    switch (event)
    {
        case PD_PHY_EVENT_STATE_CHANGE:
            HAL_EnableUSBPDIrq(0);
            USB_OsaEventSet(pdInstance->taskEventHandle, PD_TASK_EVENT_PHY_STATE_CHAGNE);
            USB_OsaEventSet(pdInstance->taskEventHandle, PD_TASK_EVENT_TYPEC_STATE_PROCESS);
            break;

        case PD_PHY_EVENT_SEND_COMPLETE:
        {
            PD_MsgSendDone(pdInstance, (pd_status_t) * ((uint8_t *)param));
            break;
        }

        case PD_PHY_EVENT_FR_SWAP_SINGAL_RECEIVED:
        {
            if (pdInstance->inProgress != kVbusPower_InFRSwap)
            {
                PD_ConnectSetPowerProgress(pdInstance, kVbusPower_InFRSwap);
#if 0
                if (!PD_DpmCheckLessOrEqualVsafe5v(pdInstance))
                {
                    pdInstance->callbackFns->PD_SrcTurnOffVbus(pdInstance->callbackParam, kVbusPower_Stable);
                }
#endif
                USB_OsaEventSet(pdInstance->taskEventHandle, PD_TASK_EVENT_FR_SWAP_SINGAL);
            }
            break;
        }

        case PD_PHY_EVENT_RECEIVE_COMPLETE:
        {
            pd_phy_rx_result_t *rxResult = (pd_phy_rx_result_t *)param;
            pdInstance->receivedSop = rxResult->rxSop;
            PD_MsgReceived(pdInstance, rxResult->rxLength, (pd_status_t)rxResult->rxResultStatus);
            break;
        }

        case PD_PHY_EVENT_HARD_RESET_RECEIVED:
        {
            PD_ConnectSetPowerProgress(pdInstance, kVbusPower_InHardReset);
            pdInstance->hardResetReceived = 1;
            USB_OsaEventSet(pdInstance->taskEventHandle, PD_TASK_EVENT_RECEIVED_HARD_RESET);
            break;
        }

        case PD_PHY_EVENT_REQUEST_STACK_RESET:
            /* BootSoftRebootToMain(); */
            break;

        case PD_PHY_EVENT_VCONN_PROTECTION_FAULT:

            break;

        case PD_PHY_EVENT_FAULT_RECOVERY:
        {
            pd_phy_config_t phyConfig;

            pdInstance->phyInterface->pdPhyDeinit(pdInstance->pdPhyHandle);
            phyConfig.interface = pdInstance->pdConfig->phyInterface;
            phyConfig.interfaceParam = pdInstance->pdConfig->interfaceParam;
            pdInstance->phyInterface->pdPhyInit(pdInstance, &(pdInstance->pdPhyHandle), &phyConfig);
            break;
        }

        case PD_PHY_EVENT_VBUS_STATE_CHANGE:
            USB_OsaEventSet(pdInstance->taskEventHandle, PD_TASK_EVENT_OTHER);
            break;

        default:
            break;
    }
}

void PD_WaitUsec(uint32_t us)
{
    uint32_t usDelay;

    while (us--)
    {
        usDelay = 15;
        while (--usDelay)
        {
            asm(nop);
        }
    }
}
