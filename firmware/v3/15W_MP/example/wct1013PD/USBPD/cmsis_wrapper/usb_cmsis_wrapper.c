/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_osa.h"
#include "appcfg.h"
#include "wct_lib.h"

#include "hal.h"

#if USB_POWER_ENABLE

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ARM_DRIVER_OK                 0 ///< Operation succeeded 
#define ARM_DRIVER_ERROR             -1 ///< Unspecified error
#define ARM_DRIVER_ERROR_BUSY        -2 ///< Driver is busy
#define ARM_DRIVER_ERROR_TIMEOUT     -3 ///< Timeout occurred
#define ARM_DRIVER_ERROR_UNSUPPORTED -4 ///< Operation not supported
#define ARM_DRIVER_ERROR_PARAMETER   -5 ///< Parameter error
#define ARM_DRIVER_ERROR_SPECIFIC    -6 ///< Start of driver specific errors 

static cmsis_i2c_adapter_t s_I2CDriverInstance[CMSIS_DRIVER_WRAPPER_INSTANCE_COUNT];

typedef enum _cmsis_driver_state
{
    CMSIS_IDLE,
    CMSIS_TRANSFERING,
    CMSIS_TRANSFER_ERROR_DONE
} cmsis_driver_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int32_t CMSIS_PortControlInterfaceInit(usb_cmsis_wrapper_handle *wrapperHandle,
                                       uint8_t interface,
                                       void *interfaceConfig)
{
    cmsis_i2c_adapter_t *cmsisI2cAdapter = NULL;
    uint32_t j;
    uint8_t index = 0;

    for (; index < CMSIS_DRIVER_WRAPPER_INSTANCE_COUNT; index++)
    {
        if (s_I2CDriverInstance[index].occupied != 1)
        {
            uint8_t *buffer = (uint8_t *)&s_I2CDriverInstance[index];
            for (j = 0U; j < sizeof(cmsis_i2c_adapter_t); j++)
            {
                buffer[j] = 0x00U;
            }
            s_I2CDriverInstance[index].occupied = 1;
            cmsisI2cAdapter = &s_I2CDriverInstance[index];
            break;
        }
    }
    if (cmsisI2cAdapter == NULL)
    {
        return ARM_DRIVER_ERROR;
    }

    cmsisI2cAdapter->cmsisState = CMSIS_IDLE;

    /* initialize I2C: i2c speed */
    HAL_USBPDI2CInit();

    *wrapperHandle = cmsisI2cAdapter;
}

int32_t CMSIS_PortControlInterfaceDeinit(usb_cmsis_wrapper_handle wrapperHandle)
{
    cmsis_i2c_adapter_t *cmsisI2cAdapter = (cmsis_i2c_adapter_t *)wrapperHandle;

    /* deinit I2C */
    HAL_USBPDI2CDeinit();

    cmsisI2cAdapter->occupied = 0;
    return ARM_DRIVER_OK;
}

int32_t CMSIS_PortControlInterfaceWriteRegister(usb_cmsis_wrapper_handle wrapperHandle,
                                                uint32_t slave,
                                                uint32_t registerAddr,
                                                uint8_t registerLen,
                                                const uint8_t *data,
                                                uint32_t num)
{
    int32_t result;
    cmsis_i2c_adapter_t *cmsisI2cAdapter = (cmsis_i2c_adapter_t *)wrapperHandle;
    uint8_t index;
    //USB_OSA_SR_ALLOC();

    cmsisI2cAdapter->cmsisTry = CMSIS_TRANSFER_RETRY_COUNT;

    do
    {
        result = ARM_DRIVER_ERROR;
        //USB_OSA_ENTER_CRITICAL();
        if (HAL_USBPDI2CWrite(slave, registerAddr, registerLen, (uint8_t*)data, num) == 0)
        {
        	result = ARM_DRIVER_OK;
        }
        //USB_OSA_EXIT_CRITICAL();

        if (result == ARM_DRIVER_OK)
        {
            break;
        }
        else
        {
            /* deinit i2c -> release bus -> init i2c */
            HAL_I2C_Disable();
            HAL_I2C_ReleaseBus();
            HAL_I2C_Enable();
        }
    } while (--cmsisI2cAdapter->cmsisTry);

    return result;
}

int32_t CMSIS_PortControlInterfaceReadRegister(usb_cmsis_wrapper_handle wrapperHandle,
                                               uint32_t slave,
                                               uint32_t registerAddr,
                                               uint8_t registerLen,
                                               uint8_t *data,
                                               uint32_t num)
{
    int32_t result;
    cmsis_i2c_adapter_t *cmsisI2cAdapter = (cmsis_i2c_adapter_t *)wrapperHandle;
    //USB_OSA_SR_ALLOC();

    cmsisI2cAdapter->cmsisTry = CMSIS_TRANSFER_RETRY_COUNT;
    do
    {
        result = ARM_DRIVER_ERROR;
        //USB_OSA_ENTER_CRITICAL();
        if (HAL_USBPDI2CRead(slave, registerAddr, registerLen, data, num) == 0)
        {
        	result = ARM_DRIVER_OK;
        }
        //USB_OSA_EXIT_CRITICAL();

        if (result == ARM_DRIVER_OK)
        {
            break;
        }
        else
        {
            /* deinit i2c -> release bus -> init i2c */
            HAL_I2C_Disable();
            HAL_I2C_ReleaseBus();
            HAL_I2C_Enable();
        }
    } while (--cmsisI2cAdapter->cmsisTry);

    return result;
}

#endif

