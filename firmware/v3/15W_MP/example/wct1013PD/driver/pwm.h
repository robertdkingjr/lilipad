/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __PWM_H__
#define __PWM_H__
    
#define PWM_INVERTER_INDEX   0
#define PWM_INVERTER_CHANNEL_TRIGGER    0
#define PWM_INVERTER_CHANNEL_LEFT_HIGH    2
#define PWM_INVERTER_CHANNEL_RIGHT_HIGH   4

//#define PWM_INVERTER_RIGHT_INDEX  0
//#define PWM_INVERTER_CHANNEL_RIGHT_HIGH  2
//#define PWM_INVERTER_CHANNEL_RIGHT_LOW   4

#define PWM_Q_MEASURE_INDEX       1      //actually using PWM1,
#define PWM_Q_MEASURE_CHANNEL_TRIGGER    0
#define PWM_Q_MEASURE_CHANNEL_HIGH       2
#define PWM_Q_MEASURE_CHANNEL_LOW        4

#define PWM_D_DCDC_BUCK_INDEX      2
#define PWM_D_DCDC_CHANNLE_BUCK_HIGH    0

#define PWM_D_DCDC_CLK      200000UL

typedef enum
{
	PWM_MODE_NORMAL = 0,
	PWM_MODE_COUNT
}PWM_MODE_E;

void PWM_SetFreq(uint8 byId, uint32 dwPeriod);
void PWM_SetChannelPairs(uint8 byId, uint8 byChannel, uint32 dwStartPeriod, uint32 dwStopPeriod);
void PWM_SetChannelDuty(uint8 byId, uint8 byChannel, uint32 dwPeriod);
void PWM_Start(uint8 byId);
void PWM_Stop(uint8 byId);
void PWM_StopBridge(uint8 byChannel);
void PWM_ClearChannelFlags(uint8 byId, uint8 byChannel);
boolean PWM_CheckChannelFlags(uint8 byId, uint8 byChannel);
void PWM_EnableTrigger(uint8 byId, uint8 byChannel, uint8 byIsEn);
boolean PWM_CheckOverFlowFlag(uint8 byId);
void PWM_ClearOverFlowFlag(uint8 byId);
boolean PWM_CheckRunning(uint8 byId);
void PWM_Init(uint16 wDeadTimeNs);
void PWM_SetMode(uint8 byId, PWM_MODE_E mode);
void PWM_FracOn(uint8 byId, uint8 byIsEn);
void PWM_DisablebyFaultInput(uint8 byIsEn);
boolean PWM_CheckFaultFlag(void);
void PWM_ClearFaultFlag(void);

//Optimization for speed performance
#define PWM_Update_D_DCDC_Channels(buckDuty) \
		PWMA_SM2FRACVAL3 = buckDuty<<11; \
		PWMA_SM2VAL3 = buckDuty>>5; \
		PWMA_MCTRL |= PWMA_MCTRL_LDOK_2;

#define PWM_UpdateFreq(pwmIndex, period) \
		if(PWM_INVERTER_INDEX == pwmIndex) \
		{\
			PWMA_SM0INIT = 0;\
			PWMA_SM0VAL1 = (period >> 5);\
			PWMA_SM0FRACVAL1 = (period & 0x1f) << 11;\
		}

#define PWM_UpdateChannelPairs(pwmIndex, pwmChIndex, pwmH, pwmL) \
		if( (PWM_INVERTER_INDEX == pwmIndex) && (PWM_INVERTER_CHANNEL_LEFT_HIGH == pwmChIndex) )\
		{\
			PWMA_SM0VAL2 = (pwmH >> 5);\
			PWMA_SM0FRACVAL2 = (pwmH & 0x1f) << 11;\
			PWMA_SM0VAL3 = (pwmL >> 5);\
			PWMA_SM0FRACVAL3 = (pwmL & 0x1f) << 11;\
		}\
		if( (PWM_INVERTER_INDEX == pwmIndex) && (PWM_INVERTER_CHANNEL_RIGHT_HIGH == pwmChIndex) )\
		{\
			PWMA_SM0VAL4 = (pwmH >> 5);\
			PWMA_SM0FRACVAL4 = (pwmH & 0x1f) << 11;\
			PWMA_SM0VAL5 = (pwmL >> 5);\
			PWMA_SM0FRACVAL5 = (pwmL & 0x1f) << 11;\
		}

#define PWM_InvalidUpdate(pwmIndex) \
		if( PWM_INVERTER_INDEX == pwmIndex ) \
		{\
			PWMA_MCTRL |= PWMA_MCTRL_CLDOK_0;\
		}

#define PWM_ValidUpdate(pwmIndex) \
		if( PWM_INVERTER_INDEX == pwmIndex ) \
		{\
			PWMA_MCTRL |= PWMA_MCTRL_LDOK_0;\
		}

#endif
