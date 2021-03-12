/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SPI_IIC_ADAPTER_H__
#define __SPI_IIC_ADAPTER_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CMSIS_DRIVER_WRAPPER_INSTANCE_COUNT (1)

/*! @brief usb cmsis I2C/SPI driver handle */
typedef void *usb_cmsis_wrapper_handle;

/*!
 * @addtogroup usb_pd_cmsis_wrapper
 * @{
 */

/*! @brief CMSIS I2C/SPI transfer retry count */
#define CMSIS_TRANSFER_RETRY_COUNT (10)

typedef struct _cmsis_i2c_driver_adapter
{
    uint8_t occupied;
    uint8_t interface;
    volatile uint8_t cmsisState;
    volatile uint8_t cmsisResult;
    volatile uint8_t cmsisTry;
} cmsis_i2c_adapter_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize CMSIS driver adapter instance.
 *
 * This function return the #cmsis_driver_adapter_t instance, the other API use this as the parameter.
 *
 * @param[out] wrapperHandle   Return the handle.
 * @param[in] interface      the I2C/SPI interface, see #pd_phy_interface_t
 * @param[in] interfaceConfig it is not used yet.
 *
 * @retval ARM_DRIVER_ERROR    initialization success.
 * @retval other value         error code.
 */
int32_t CMSIS_PortControlInterfaceInit(usb_cmsis_wrapper_handle *wrapperHandle,
                                       uint8_t interface,
                                       void *interfaceConfig);

/*!
 * @brief De-initialize CMSIS driver adapter instance.
 *
 * @param[in] wrapperHandle    The handle from CMSIS_PortControlInterfaceInit
 *
 * @retval ARM_DRIVER_ERROR    initialization success.
 * @retval other value         error code.
 */
int32_t CMSIS_PortControlInterfaceDeinit(usb_cmsis_wrapper_handle wrapperHandle);

/*!
 * @brief Write data to slave.
 *
 * @param[in] wrapperHandle  The handle from CMSIS_PortControlInterfaceInit.
 * @param[in] slave          For I2C it is slave address, for SPI it not defined yet.
 * @param[in] registerAddr   The access register address.
 * @param[in] registerLen    The register addreess's length, normally it is one byte or two bytes.
 * @param[in] data           The data buffer.
 * @param[in] num            The data length.
 *
 * @retval ARM_DRIVER_ERROR    initialization success.
 * @retval other value         error code.
 */
int32_t CMSIS_PortControlInterfaceWriteRegister(usb_cmsis_wrapper_handle wrapperHandle,
                                                uint32_t slave,
                                                uint32_t registerAddr,
                                                uint8_t registerLen,
                                                const uint8_t *data,
                                                uint32_t num);

/*!
 * @brief Read data from slave.
 *
 * @param[in] wrapperHandle  The handle from CMSIS_PortControlInterfaceInit.
 * @param[in] slave          For I2C it is slave address, for SPI it not defined yet.
 * @param[in] registerAddr   The access register address.
 * @param[in] registerLen    The register addreess's length, normally it is one byte or two bytes.
 * @param[in] data           The data buffer.
 * @param[in] num            The data length.
 *
 * @retval ARM_DRIVER_ERROR    initialization success.
 * @retval other value         error code.
 */
int32_t CMSIS_PortControlInterfaceReadRegister(usb_cmsis_wrapper_handle wrapperHandle,
                                               uint32_t slave,
                                               uint32_t registerAddr,
                                               uint8_t registerLen,
                                               uint8_t *data,
                                               uint32_t num);

/*! @}*/

#endif
