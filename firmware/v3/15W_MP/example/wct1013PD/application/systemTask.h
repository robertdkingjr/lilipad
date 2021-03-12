/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SYSTEMTASK_H__
#define __SYSTEMTASK_H__

typedef enum
{
    APP_SUCCESS = 0,
    APP_NVM_ERROR,
    APP_OVP_ERROR,
    APP_UVP_ERROR,
    APP_OTP_ERROR,
    APP_MAX_ERROR
} APP_ERROR_TYPE;

void App_ClrErr(APP_ERROR_TYPE err);
void App_SetErr(APP_ERROR_TYPE err);
APP_ERROR_TYPE App_GetErr(void);
void AppTask_Poll(void);


#endif
