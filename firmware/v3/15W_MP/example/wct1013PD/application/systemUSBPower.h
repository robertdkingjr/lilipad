/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SYSTEMUSBPD_H__
#define __SYSTEMUSBPD_H__

#if USB_POWER_ENABLE

typedef enum 
{
	USB_TYPE_INIT = 0,
	USB_TYPE_NONE,
	USB_TYPE_NORMAL,
	USB_TYPE_QC,
	USB_TYPE_PD
}USB_POWER_TYPE_E;

typedef enum _pd_power_type
{
	kSourceCap_FixedVoltageCurrent,
	kSoruceCap_VariableVoltageCurrent,
	kSoruceCap_VariableVoltagePower,
	kSourceCap_RangeVoltageCurrent
} pd_power_type_t;

/* voltage unit - V; power unit - W */
void USBPD_TickHandler(void);
void USBPD_Task(void);
void USBPD_PHYIsr(void);
/* return value: 1 - support PD, 0 - don't support */
uint8_t USBPD_GetSupportState(void);

void USB_PowerSetup(uint32 dwTargetVol, uint32 dwMaxVol);
void USB_PDPowerSwitch(uint32 dwTargetVol, uint32 dwMaxVol, boolean bNoPPS);
uint32 USB_GetPowerCap(void);
USB_POWER_TYPE_E USB_GetPowerType(void);
pd_power_type_t USBGetPDType(void);

#endif

#endif

