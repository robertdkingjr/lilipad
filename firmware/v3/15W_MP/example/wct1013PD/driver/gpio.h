/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__


typedef enum
{
	GPIO_MODE_GP = 0,
	GPIO_MODE_PER
}GPIO_MODE_E;

//make below enum same as HAL_TP_TYPE_E
typedef enum
{
	GPIO_SET = 0,
	GPIO_CLR = 1,
	GPIO_TOG = 2
}GPIO_OUT_E;

typedef enum
{
#if !DIGITAL_DCDC
	GPIO_DCDC_EN,
#endif
	GPIO_LED1,
	GPIO_LED2,
	GPIO_PWM_LEFT_HIGH,
	GPIO_PWM_RIGHT_HIGH,
	GPIO_DDMTOGGLE,
	GPIO_FSKTOGGLE,
	GPIO_RAIL_DIS,
	GPIO_DRV_EN_L,
	GPIO_DRV_EN_H,
	GPIO_CTRL_DN_0,
	GPIO_CTRL_DN_1,
	GPIO_CTRL_DP_0,
	GPIO_CTRL_DP_1,
	GPIO_nINT,
	GPIO_I2C0SCL,
	GPIO_I2C0SDA,
	GPIO_MAX
}GPIO_ID_E;

void GPIO_Init(void);
void GPIO_SetMode(GPIO_ID_E byId, GPIO_MODE_E mode, boolean bOut);
void GPIO_SetOutput(GPIO_ID_E byId, GPIO_OUT_E output);
boolean GPIO_GetInput(GPIO_ID_E byId);


#define GPIO_SET_DN_COMB_00   GPIOC_DR &= ~(BIT9|BIT10)
#define GPIO_SET_DN_COMB_06   {uint16 tmp = GPIOC_DR; tmp &= ~(BIT9|BIT10); tmp |= BIT9; GPIOC_DR = tmp;}
#define GPIO_SET_DN_COMB_25   {uint16 tmp = GPIOC_DR; tmp &= ~(BIT9|BIT10); tmp |= BIT10; GPIOC_DR = tmp;}
#define GPIO_SET_DN_COMB_33   GPIOC_DR |= (BIT9|BIT10)

#define GPIO_SET_DP_COMB_00   GPIOC_DR &= ~(BIT11|BIT12)
#define GPIO_SET_DP_COMB_06   {uint16 tmp = GPIOC_DR; tmp &= ~(BIT11|BIT12); tmp |= BIT11; GPIOC_DR = tmp;}
#define GPIO_SET_DP_COMB_25   {uint16 tmp = GPIOC_DR; tmp &= ~(BIT11|BIT12); tmp |= BIT12; GPIOC_DR = tmp;}
#define GPIO_SET_DP_COMB_33   GPIOC_DR |= (BIT11|BIT12)


#endif
