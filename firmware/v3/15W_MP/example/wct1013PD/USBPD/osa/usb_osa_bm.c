/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2015-2016 Freescale Semiconductor, Inc.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#include "defines.h"
#include "usb.h"
#include "usb_osa.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_OSA_BM_EVENT_COUNT (4U)

/* BM Event status structure */
typedef struct _usb_osa_event_struct
{
    uint32_t value; /* Event mask */
    uint32_t flag;  /* Event flags, includes auto clear flag */
    uint8_t isUsed; /* Is used */
} usb_osa_event_struct_t;



/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern uint8 HAL_DisableIRQ(void);
extern void HAL_RestoreIRQ(uint8 bySts);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static usb_osa_event_struct_t s_UsbBmEventStruct[USB_OSA_BM_EVENT_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_OsaEnterCritical(uint32_t *sr)
{
	*sr = HAL_DisableIRQ();
}

void USB_OsaExitCritical(uint32_t sr)
{
    HAL_RestoreIRQ(sr);
}

usb_osa_status_t USB_OsaEventCreate(usb_osa_event_handle *handle, uint32_t flag)
{
	uint32_t i;
    usb_osa_event_struct_t *event = NULL;
    USB_OSA_SR_ALLOC();

    if (!handle)
    {
        return kStatus_USB_OSA_Error;
    }

    USB_OSA_ENTER_CRITICAL();
    for (i = 0; i < USB_OSA_BM_EVENT_COUNT; i++)
    {
        if (0 == s_UsbBmEventStruct[i].isUsed)
        {
            event = &s_UsbBmEventStruct[i];
            break;
        }
    }

    if (NULL == event)
    {
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Error;
    }

    event->value = 0U;
    event->flag = flag;
    event->isUsed = 1;
    *handle = event;
    USB_OSA_EXIT_CRITICAL();
    return kStatus_USB_OSA_Success;
}

usb_osa_status_t USB_OsaEventDestroy(usb_osa_event_handle handle)
{
    usb_osa_event_struct_t *event = (usb_osa_event_struct_t *)handle;
    USB_OSA_SR_ALLOC();

    if (handle)
    {
        USB_OSA_ENTER_CRITICAL();
        event->isUsed = 0;
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Success;
    }
    return kStatus_USB_OSA_Error;
}

usb_osa_status_t USB_OsaEventSet(usb_osa_event_handle handle, uint32_t bitMask)
{
    usb_osa_event_struct_t *event = (usb_osa_event_struct_t *)handle;
    USB_OSA_SR_ALLOC();

    if (handle)
    {
        USB_OSA_ENTER_CRITICAL();
        event->value |= bitMask;
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Success;
    }
    return kStatus_USB_OSA_Error;
}

usb_osa_status_t USB_OsaEventWait(
    usb_osa_event_handle handle, uint32_t bitMask, uint32_t flag, uint32_t timeout, uint32_t *bitSet)
{
    usb_osa_event_struct_t *event = (usb_osa_event_struct_t *)handle;
    uint32_t bits;
    USB_OSA_SR_ALLOC();

    if (handle)
    {
        USB_OSA_ENTER_CRITICAL();
        bits = event->value & bitMask;
        if (flag)
        {
            if (bits != bitMask)
            {
                USB_OSA_EXIT_CRITICAL();
                return kStatus_USB_OSA_TimeOut;
            }
        }
        else
        {
            if (!bits)
            {
                USB_OSA_EXIT_CRITICAL();
                return kStatus_USB_OSA_TimeOut;
            }
        }
        if (bitSet)
        {
            *bitSet = bits;
        }
        if (event->flag)
        {
            event->value &= ~bits;
        }
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Success;
    }
    return kStatus_USB_OSA_Error;
}

usb_osa_status_t USB_OsaEventCheck(usb_osa_event_handle handle, uint32_t bitMask, uint32_t *bitSet)
{
    usb_osa_event_struct_t *event = (usb_osa_event_struct_t *)handle;
    uint32_t bits;
    USB_OSA_SR_ALLOC();

    if (handle)
    {
        USB_OSA_ENTER_CRITICAL();
        bits = event->value & bitMask;

        if (!bits)
        {
            USB_OSA_EXIT_CRITICAL();
            return kStatus_USB_OSA_Error;
        }

        if (bitSet)
        {
            *bitSet = bits;
        }
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Success;
    }
    return kStatus_USB_OSA_Error;
}

usb_osa_status_t USB_OsaEventClear(usb_osa_event_handle handle, uint32_t bitMask)
{
    usb_osa_event_struct_t *event = (usb_osa_event_struct_t *)handle;
    uint32_t bits;
    USB_OSA_SR_ALLOC();

    if (handle)
    {
        USB_OSA_ENTER_CRITICAL();
        bits = event->value & bitMask;
        event->value &= ~bits;
        USB_OSA_EXIT_CRITICAL();
        return kStatus_USB_OSA_Success;
    }
    return kStatus_USB_OSA_Error;
}


