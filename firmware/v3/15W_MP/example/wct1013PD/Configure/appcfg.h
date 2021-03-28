/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __APP_CFG_H__
#define __APP_CFG_H__

//---------------------------- Application Version Defines -----------------------------
#define WCT_SW_VERSION                     0x1000u //x.y.z: 4 bit x, 4 bit y, 8 bit z
#define WCT_HW_VERSION                     0x1000u //x.y.z: 4 bit x, 4 bit y, 8 bit z

#define LILIPAD_HW_VERSION  3  // Lilipad v3
#define LILIPAD_SW_VERSION  1

#define DEVICE_ID_STRING                   "MPA11"
#define NUM_DEVICES                        1u
#define NUM_COILS_PER_DEVICE               1u

#define NVM_BASE_ADDR                      ( 0x01F000UL << 1 )

#define FREEMASTER_SUPPORTED	           (FALSE)
#define DEBUG_CONSOLE_SUPPORTED            (TRUE)
#define LOW_POWER_MODE_ENABLE			   (FALSE)

#define DIGITAL_DCDC					   (FALSE)

#define USB_POWER_ENABLE                   (TRUE)

#define BOOTLOADER_USED					   (FALSE)
#define BOOTLOADER_PARAMS_ADDR			   ( 0x0001F3FCUL << 1 )

#endif
