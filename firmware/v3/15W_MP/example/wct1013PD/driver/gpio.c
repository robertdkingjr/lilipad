/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "defines.h"
#include "cpu.h"
#include "appcfg.h"

#include "systemDebug.h"

#include "gpio.h"

typedef struct
{
	uint16* pwPort;
	uint8  bit;
}GPIO_CTRL_T;

GPIO_CTRL_T gGPIO_Ctrl[] =
{
#if !DIGITAL_DCDC
	{(uint16*)FGPIOF_BASE, 0},  //GPIO_DCDC_EN
#endif
	{(uint16*)FGPIOE_BASE, 4},  //GPIO_LED1
	{(uint16*)FGPIOE_BASE, 5},  //GPIO_LED2
	{(uint16*)FGPIOE_BASE, 1},  //GPIO_PWM_LEFT_HIGH
	{(uint16*)FGPIOE_BASE, 0},  //GPIO_PWM_RIGHT_HIGH
	{(uint16*)FGPIOC_BASE, 6},  //GPIO_DDMTOGGLE
	{(uint16*)FGPIOF_BASE, 6},  //GPIO_FSKTOGGLE
	{(uint16*)FGPIOF_BASE, 1},  //GPIO_RAIL_DIS
	{(uint16*)FGPIOC_BASE, 7},  //GPIO_DRV_EN_L
	{(uint16*)FGPIOC_BASE, 8},  //GPIO_DRV_EN_H
	{(uint16*)FGPIOC_BASE, 9},  //GPIO_CTRL_DN0
	{(uint16*)FGPIOC_BASE, 10}, //GPIO_CTRL_DN1
	{(uint16*)FGPIOC_BASE, 11}, //GPIO_CTRL_DP0
	{(uint16*)FGPIOC_BASE, 12}, //GPIO_CTRL_DP1
	{(uint16*)FGPIOC_BASE, 13}, //GPIO_nINT
	{(uint16*)FGPIOC_BASE, 15}, //GPIO_I2C0SCL
	{(uint16*)FGPIOC_BASE, 14}  //GPIO_I2C0SDA
};

void GPIO_Init(void)
{
	GPIOA_PUR    = 0x0000U;
	GPIOA_DDR    = 0x0000U;
	GPIOA_PER    = 0x00FFU;
	GPIOA_PPMODE = 0x00FFU;               
	
	GPIOB_PUR    = 0x0000U;
	GPIOB_DDR    = 0x00E0U;
	GPIOB_PER    = 0x001FU;
	GPIOB_PPMODE = 0x001FU;
	GPIOB_PUS    = 0x0000U;
	
	GPIOC_PUR    = 0xC000U;
	GPIOC_DR     = 0x0000U;
	GPIOC_DDR    = 0xFFF8U;
	GPIOC_PER    = 0xC03FU;
	GPIOC_PPMODE = 0xFFFFU;
	GPIOC_PUS    = 0xC000U;
	
	GPIOD_PUR    = 0x0000U;
	GPIOD_PER    = 0x001FU;
	GPIOD_PPMODE = 0x00FFU;
	GPIOD_DRIVE  = 0x0000U;
	
	GPIOE_PUR    = 0x0030U;
	GPIOE_DDR    = 0x00FFU;
	GPIOE_PER    = 0x000FU;
	GPIOE_PPMODE = 0x003FU;

	GPIOF_PUR    = 0x0000U;
	GPIOF_DR     = 0x0020U;
	GPIOF_DDR    = 0x01FFU;
	GPIOF_PER    = 0x000CU;   //F2 F3 are configured as XBAR_OUT
	GPIOF_PPMODE = 0x00FFU;
}

void GPIO_SetMode(GPIO_ID_E byId, GPIO_MODE_E mode, boolean bOut)
{
	uint16* ptr;
	
	DBG_Assert(byId >= GPIO_MAX, 0, byId);
	
	ptr = gGPIO_Ctrl[byId].pwPort;

	if(GPIO_MODE_PER == mode)
	{
		//GPIOx_PER
		ptr[3] |= (1U<<gGPIO_Ctrl[byId].bit);
	}
	else
	{
		//GPIOx_PER
		ptr[3] &= ~(1U<<gGPIO_Ctrl[byId].bit);
		if(bOut)
		{
			//GPIOx_DDR
			ptr[2] |= (1U<<gGPIO_Ctrl[byId].bit);
		}
		else
		{
			//GPIOx_DDR
			ptr[2] &= ~(1U<<gGPIO_Ctrl[byId].bit);
		}
	}
}

void GPIO_SetOutput(GPIO_ID_E byId, GPIO_OUT_E output)
{
	uint16* ptr;
		
	DBG_Assert(byId >= GPIO_MAX, 0, byId);
	
	ptr = gGPIO_Ctrl[byId].pwPort;

	if(GPIO_SET == output)
	{
		//GPIOx_DR
		ptr[1] |= (1U<<gGPIO_Ctrl[byId].bit);
	}
	else if(GPIO_CLR == output)
	{
		//GPIOx_DR
		ptr[1] &= ~(1U<<gGPIO_Ctrl[byId].bit);
	}
	else if(GPIO_TOG == output)
	{
		//GPIOx_DR
		ptr[1] ^= (1U<<gGPIO_Ctrl[byId].bit);
	}
}

boolean GPIO_GetInput(GPIO_ID_E byId)
{
	boolean ret = 0;
	uint16* ptr;
		
	DBG_Assert(byId >= GPIO_MAX, 0, byId);
	
	ptr = gGPIO_Ctrl[byId].pwPort;
	
	//GPIOx_DR
	if(ptr[1] & (1U<<gGPIO_Ctrl[byId].bit))
	{
		ret = 1;
	}
	
	return ret;
}
