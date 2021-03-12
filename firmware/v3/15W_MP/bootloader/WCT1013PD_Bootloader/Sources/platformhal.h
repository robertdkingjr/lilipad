/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef PLATFORMHAL_H_
#define PLATFORMHAL_H_

//-------------------
// PRIVATE FUNCTIONS
//-------------------
void OccsInit( void );
void SimInit( void );
void GpioInit( void );
void WatchdogTimerInit( void );
void TimerA0Init( void );
void TimerA1Init( void );
void Qsci0Init( void );
void TimerA0Reset( void );
void TimerA1Reset( void );
void Qsci0Reset( void );
//-------------------
// PUBLIC FUNCTIONS
//-------------------
void HwInit( void );

#endif /* PLATFORMHAL_H_ */
