/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __PD_CONFIG_H__
#define __PD_CONFIG_H__

#include "usb_pd.h"

/*!
 * @brief PD PTN5110 PHY driver instance count, meantime it indicates PTN5110 PHY driver enable or disable.
 *        - if 0, PTN5110 driver is disable.
 *        - if greater than 0, PTN5110 driver is enable.
 */
#define PD_CONFIG_PTN5110_PORT (1)

/*!
 * @brief PD stack instance max count.
 */
#define PD_CONFIG_MAX_PORT (PD_CONFIG_PTN5110_PORT)

/*! @brief PD revision that this stack support */
#define PD_CONFIG_REVISION (PD_SPEC_REVISION_30)

/*!
 * @brief Enable PD3.0 PPS function.
 */
#define PD_CONFIG_PD3_PPS_ENABLE (1)

/*! @brief PD structured VDM version that this stack support */
#define PD_CONFIG_STRUCTURED_VDM_VERSION (PD_SPEC_STRUCTURED_VDM_VERSION_10)

/*!
 * @brief Enable CMSIS I2C driver.
 */
#define PD_CONFIG_CMSIS_I2C_INTERFACE (1)

/*!
 * @brief Enable CMSIS SPI driver.
 */
#define PD_CONFIG_CMSIS_SPI_INTERFACE (0)

/*!
 * @brief Enable PD stack source role function.
 */
#define PD_CONFIG_SOURCE_ROLE_ENABLE (0)

/*!
 * @brief Enable PD stack sink role function.
 */
#define PD_CONFIG_SINK_ROLE_ENABLE (1)

/*!
 * @brief Enable PD stack dual power role function.
 */
#define PD_CONFIG_DUAL_POWER_ROLE_ENABLE (0)

/*!
 * @brief Enable PD stack dual data role function.
 */
#define PD_CONFIG_DUAL_DATA_ROLE_ENABLE (0)

/*!
 * @brief Enable Vconn support (vconn_swap, vconn supply).
 */
#define PD_CONFIG_VCONN_SUPPORT (0)

/*!
 * @brief Enable Vconn discharge function.
 */
#define USBPD_ENABLE_VCONN_DISCHARGE (0)

/*!
 * @brief Enable vendor defined message function.
 */
#define PD_CONFIG_VENDOR_DEFINED_MESSAGE_ENABLE (0)

/*!
 * @brief Enable alternate mode function.
 */
#define PD_CONFIG_ALTERNATE_MODE_SUPPORT (0)

/*!
 * @brief Enable auto discovery cable plug function.
 */
#define PD_CONFIG_SRC_AUTO_DISCOVER_CABLE_PLUG (0)

/*!
 * @brief Enable cable communication function.
 */
#define PD_CONFIG_CABLE_COMMUNICATION_ENABLE (0)

/*!
 * @brief Config the detach detect way
 *  - PD_SINK_DETACH_ON_VBUS_ABSENT: detach is detected when vubs absent.
 *  - PD_SINK_DETACH_ON_CC_OPEN: detach is detected when CC is open.
 */
#define PD_CONFIG_SINK_DETACH_DETECT_WAY (PD_SINK_DETACH_ON_VBUS_ABSENT)
#define PD_SINK_DETACH_ON_VBUS_ABSENT (1)
#define PD_SINK_DETACH_ON_CC_OPEN (2)

/*!
 * @brief Enable PD3.0 extended message function.
 */
#define PD_CONFIG_EXTENDED_MSG_SUPPORT (0)

/*!
 * @brief Enable fast role swap function.
 */
#define PD_CONFIG_PD3_FAST_ROLE_SWAP_ENABLE (0)

/*!
 * @brief Enable PD3.0 AMS collision avoid function.
 */
#define PD_CONFIG_PD3_AMS_COLLISION_AVOID_ENABLE (0)

/*!
 * @brief Set the discharge time.
 * 1 - use the 14ms.
 * 0 - use the 650ms.
 */
#define PD_CONFIG_MIN_DISCHARGE_TIME_ENABLE (1)

/*!
 * @brief Enable Try.SNK function.
 */
#define PD_CONFIG_TRY_SNK_SUPPORT (0)

/*!
 * @brief Enable Try.SRC function.
 */
#define PD_CONFIG_TRY_SRC_SUPPORT (0)

/*!
 * @brief Enable sink accessory function.
 */
#define PD_CONFIG_SINK_ACCESSORY_SUPPORT (0)

/*!
 * @brief Enable audio accessory function.
 */
#define PD_CONFIG_AUDIO_ACCESSORY_SUPPORT (0)

/*!
 * @brief Enable debug accessory function.
 */
#define PD_CONFIG_DEBUG_ACCESSORY_SUPPORT (0)

/*!
 * @brief Enable compliance test function.
 */
#define PD_CONFIG_COMPLIANCE_TEST_ENABLE (0)

#endif
