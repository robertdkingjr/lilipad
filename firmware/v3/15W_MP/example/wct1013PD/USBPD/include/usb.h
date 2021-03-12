/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __USB_H__
#define __USB_H__

#include "defines.h"
#include "usb_osa.h"
#include "usb_misc.h"
#include "usb_spec.h"

/*!
 * @addtogroup usb_drv
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Defines USB stack major version */
#define USB_STACK_VERSION_MAJOR (0x01U)
/*! @brief Defines USB stack minor version */
#define USB_STACK_VERSION_MINOR (0x00U)
/*! @brief Defines USB stack bugfix version */
#define USB_STACK_VERSION_BUGFIX (0x00U)

/*! @brief USB stack version definition */
#define USB_MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/*! @brief USB error code */
typedef enum _usb_status
{
    kStatus_USB_Success = 0x00U, /*!< Success */
    kStatus_USB_Error,           /*!< Failed */

    kStatus_USB_Busy,                       /*!< Busy */
    kStatus_USB_InvalidHandle,              /*!< Invalid handle */
    kStatus_USB_InvalidParameter,           /*!< Invalid parameter */
    kStatus_USB_InvalidRequest,             /*!< Invalid request */
    kStatus_USB_ControllerNotFound,         /*!< Controller cannot be found */
    kStatus_USB_InvalidControllerInterface, /*!< Invalid controller interface */

    kStatus_USB_NotSupported,   /*!< Configuration is not supported */
    kStatus_USB_Retry,          /*!< Enumeration get configuration retry */
    kStatus_USB_TransferStall,  /*!< Transfer stalled */
    kStatus_USB_TransferFailed, /*!< Transfer failed */
    kStatus_USB_AllocFail,      /*!< Allocation failed */
    kStatus_USB_LackSwapBuffer, /*!< Insufficient swap buffer for KHCI */
    kStatus_USB_TransferCancel, /*!< The transfer cancelled */
    kStatus_USB_BandwidthFail,  /*!< Allocate bandwidth failed */
    kStatus_USB_MSDStatusFail,  /*!< For MSD, the CSW status means fail */
    kStatus_USB_EHCIAttached,
    kStatus_USB_EHCIDetached
} usb_status_t;

/*! @brief USB host handle type define */
typedef void *usb_host_handle;

/*! @brief USB device handle type define. For device stack it is the whole device handle; for host stack it is the
 * attached device instance handle*/
typedef void *usb_device_handle;

/*! @brief USB OTG handle type define */
typedef void *usb_otg_handle;

/*! @brief USB controller ID */
typedef enum _usb_controller_index
{
    kUSB_ControllerKhci0 = 0U, /*!< KHCI 0U */
    kUSB_ControllerKhci1 = 1U, /*!< KHCI 1U, Currently, there are no platforms which have two KHCI IPs, this is reserved
                                  to be used in the future. */
    kUSB_ControllerEhci0 = 2U, /*!< EHCI 0U */
    kUSB_ControllerEhci1 = 3U, /*!< EHCI 1U, Currently, there are no platforms which have two EHCI IPs, this is reserved
                                  to be used in the future. */

    kUSB_ControllerLpcIp3511Fs0 = 4U, /*!< LPC USB IP3511 FS controller 0 */
    kUSB_ControllerLpcIp3511Fs1 =
        5U, /*!< LPC USB IP3511 FS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

    kUSB_ControllerLpcIp3511Hs0 = 6U, /*!< LPC USB IP3511 HS controller 0 */
    kUSB_ControllerLpcIp3511Hs1 =
        7U, /*!< LPC USB IP3511 HS controller 1, there are no platforms which have two IP3511 IPs, this is reserved
              to be used in the future. */

    kUSB_ControllerOhci0 = 8U, /*!< OHCI 0U */
    kUSB_ControllerOhci1 = 9U, /*!< OHCI 1U, Currently, there are no platforms which have two OHCI IPs, this is reserved
                                  to be used in the future. */

    kUSB_ControllerIp3516Hs0 = 10U, /*!< IP3516HS 0U */
    kUSB_ControllerIp3516Hs1 =
        11U  /*!< IP3516HS 1U, Currently, there are no platforms which have two IP3516HS IPs, this is reserved
           to be used in the future. */
} usb_controller_index_t;

/**
* @brief USB stack version fields
*/
typedef struct _usb_version
{
    uint8_t major;  /*!< Major */
    uint8_t minor;  /*!< Minor */
    uint8_t bugfix; /*!< Bug fix */
} usb_version_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*! @} */

#endif /* __USB_H__ */
