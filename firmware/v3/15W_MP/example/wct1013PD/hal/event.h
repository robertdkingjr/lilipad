/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#ifndef __EVENT_H__
#define __EVENT_H__

void MWCT10xx_ISR_UNDEFINED(void);
void MWCT10xx_ISR_TMRA_0(void);
void MWCT10xx_ISR_DMA0(void);
void MWCT10xx_ISR_TMRA_2(void);
void MWCT10xx_ISR_TX_REG(void);
void MWCT10xx_ISR_RX_REG(void);
void MWCT10xx_ISR_QSCI0_TDRE(void);
void MWCT10xx_ISR_QSCI0_RCV(void);
void MWCT10xx_ISR_PIT1_ROLLOVER(void);
#if DIGITAL_DCDC
void MWCT10xx_ISR_TMRA_1(void);
#else
void MWCT10xx_ISR_ADC_CC0(void);
void MWCT10xx_ISR_QSCI1_TDRE(void);
void MWCT10xx_ISR_QSCI1_RCV(void);
#endif

#endif
