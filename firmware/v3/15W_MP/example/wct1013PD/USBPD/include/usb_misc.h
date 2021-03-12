/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2015-2016 Freescale Semiconductor, Inc.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __USB_MISC_H__
#define __USB_MISC_H__

#ifndef ENDIANNESS

#error ENDIANNESS should be defined, and then rebulid the project.

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Define USB printf */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

extern int DbgConsole_Printf(const char *fmt_s, ...);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#if defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE < 1)
#define usb_echo printf
#else
#define usb_echo DbgConsole_Printf
#endif

#if defined(__ICCARM__)

#ifndef STRUCT_PACKED
#define STRUCT_PACKED __packed
#endif

#ifndef STRUCT_UNPACKED
#define STRUCT_UNPACKED
#endif

#elif defined(__GNUC__)

#ifndef STRUCT_PACKED
#define STRUCT_PACKED
#endif

#ifndef STRUCT_UNPACKED
#define STRUCT_UNPACKED __attribute__((__packed__))
#endif

#elif defined(__CC_ARM)

#ifndef STRUCT_PACKED
#define STRUCT_PACKED _Pragma("pack(1U)")
#endif

#ifndef STRUCT_UNPACKED
#define STRUCT_UNPACKED _Pragma("pack()")
#endif

#endif

#define USB_SHORT_GET_LOW(x) (((uint16_t)x) & 0xFFU)
#define USB_SHORT_GET_HIGH(x) ((uint8_t)(((uint16_t)x) >> 8U) & 0xFFU)

#define USB_LONG_GET_BYTE0(x) ((uint8_t)(((uint32_t)(x))) & 0xFFU)
#define USB_LONG_GET_BYTE1(x) ((uint8_t)(((uint32_t)(x)) >> 8U) & 0xFFU)
#define USB_LONG_GET_BYTE2(x) ((uint8_t)(((uint32_t)(x)) >> 16U) & 0xFFU)
#define USB_LONG_GET_BYTE3(x) ((uint8_t)(((uint32_t)(x)) >> 24U) & 0xFFU)

#define USB_MEM4_ALIGN_MASK (0x03U)

/* accessory macro */
#define USB_MEM4_ALIGN(n) ((n + 3U) & (0xFFFFFFFCu))
#define USB_MEM32_ALIGN(n) ((n + 31U) & (0xFFFFFFE0u))
#define USB_MEM64_ALIGN(n) ((n + 63U) & (0xFFFFFFC0u))

/* big/little endian */
#define SWAP2BYTE_CONST(n) ((((n)&0x00FFU) << 8U) | (((n)&0xFF00U) >> 8U))
#define SWAP4BYTE_CONST(n) \
    ((((n)&0x000000FFU) << 24U) | (((n)&0x0000FF00U) << 8U) | (((n)&0x00FF0000U) >> 8U) | (((n)&0xFF000000U) >> 24U))

#define USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(n, m)      \
    {                                                    \
        *((uint8_t *)&(n)) = *((uint8_t *)&(m));         \
        *((uint8_t *)&(n) + 1) = *((uint8_t *)&(m) + 1); \
        *((uint8_t *)&(n) + 2) = *((uint8_t *)&(m) + 2); \
        *((uint8_t *)&(n) + 3) = *((uint8_t *)&(m) + 3); \
    }

#define USB_ASSIGN_VALUE_ADDRESS_SHORT_BY_BYTE(n, m)     \
    {                                                    \
        *((uint8_t *)&(n)) = *((uint8_t *)&(m));         \
        *((uint8_t *)&(n) + 1) = *((uint8_t *)&(m) + 1); \
    }

#define USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(n, m) \
    {                                                     \
        *((uint8_t *)&(n)) = (uint8_t)m;                  \
        *((uint8_t *)&(n) + 1) = (uint8_t)(m >> 8);       \
        *((uint8_t *)&(n) + 2) = (uint8_t)(m >> 16);      \
        *((uint8_t *)&(n) + 3) = (uint8_t)(m >> 24);      \
    }

#define USB_ASSIGN_MACRO_VALUE_ADDRESS_SHORT_BY_BYTE(n, m) \
    {                                                      \
        *((uint8_t *)&(n)) = (uint8_t)m;                   \
        *((uint8_t *)&(n) + 1) = (uint8_t)(m >> 8);        \
    }

#if (ENDIANNESS == USB_BIG_ENDIAN)

#define USB_SHORT_TO_LITTLE_ENDIAN(n) SWAP2BYTE_CONST(n)
#define USB_LONG_TO_LITTLE_ENDIAN(n) SWAP4BYTE_CONST(n)
#define USB_SHORT_FROM_LITTLE_ENDIAN(n) SWAP2BYTE_CONST(n)
#define USB_LONG_FROM_LITTLE_ENDIAN(n) SWAP2BYTE_CONST(n)

#define USB_SHORT_TO_BIG_ENDIAN(n) (n)
#define USB_LONG_TO_BIG_ENDIAN(n) (n)
#define USB_SHORT_FROM_BIG_ENDIAN(n) (n)
#define USB_LONG_FROM_BIG_ENDIAN(n) (n)

#define USB_LONG_TO_LITTLE_ENDIAN_ADDRESS(n, m)    \
    {                                              \
        m[3] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        m[2] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        m[1] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        m[0] = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(n)                                                  \
    ((uint32_t)((((uint8_t)n[3]) << 24U) | (((uint8_t)n[2]) << 16U) | (((uint8_t)n[1]) << 8U) | \
                (((uint8_t)n[0]) << 0U)))

#define USB_LONG_TO_BIG_ENDIAN_ADDRESS(n, m)       \
    {                                              \
        m[0] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        m[1] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        m[2] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        m[3] = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_BIG_ENDIAN_ADDRESS(n)                                                     \
    ((uint32_t)((((uint8_t)n[0]) << 24U) | (((uint8_t)n[1]) << 16U) | (((uint8_t)n[2]) << 8U) | \
                (((uint8_t)n[3]) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(n, m)  \
    {                                             \
        m[1] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        m[0] = (((uint16_t)(n)) & 0xFFU);         \
    }

#define USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(n) ((uint32_t)((((uint8_t)n[1]) << 8U) | (((uint8_t)n[0]) << 0U)))

#define USB_SHORT_TO_BIG_ENDIAN_ADDRESS(n, m)     \
    {                                             \
        m[0] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        m[1] = (((uint16_t)(n)) & 0xFFU);         \
    }

#define USB_SHORT_FROM_BIG_ENDIAN_ADDRESS(n) ((uint32_t)((((uint8_t)n[0]) << 8U) | (((uint8_t)n[1]) << 0U)))

#define USB_LONG_TO_LITTLE_ENDIAN_DATA(n, m)                         \
    {                                                                \
        *((uint8_t *)&(m) + 3) = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        *((uint8_t *)&(m) + 2) = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        *((uint8_t *)&(m) + 1) = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        *((uint8_t *)&(m) + 0) = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_LITTLE_ENDIAN_DATA(n)                                             \
    ((uint32_t)(((*((uint8_t *)&(n) + 3)) << 24U) | ((*((uint8_t *)&(n) + 2)) << 16U) | \
                ((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_DATA(n, m)                       \
    {                                                               \
        *((uint8_t *)&(m) + 1) = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        *((uint8_t *)&(m)) = ((((uint16_t)(n))) & 0xFFU);           \
    }

#define USB_SHORT_FROM_LITTLE_ENDIAN_DATA(n) ((uint32_t)(((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))))))

#else

#define USB_SHORT_TO_LITTLE_ENDIAN(n) (n)
#define USB_LONG_TO_LITTLE_ENDIAN(n) (n)
#define USB_SHORT_FROM_LITTLE_ENDIAN(n) (n)
#define USB_LONG_FROM_LITTLE_ENDIAN(n) (n)

#define USB_SHORT_TO_BIG_ENDIAN(n) SWAP2BYTE_CONST(n)
#define USB_LONG_TO_BIG_ENDIAN(n) SWAP4BYTE_CONST(n)
#define USB_SHORT_FROM_BIG_ENDIAN(n) SWAP2BYTE_CONST(n)
#define USB_LONG_FROM_BIG_ENDIAN(n) SWAP4BYTE_CONST(n)

#define USB_LONG_TO_LITTLE_ENDIAN_ADDRESS(n, m)    \
    {                                              \
        m[3] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        m[2] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        m[1] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        m[0] = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(n)                                                  \
    ((uint32_t)((((uint32_t)n[3]) << 24U) | (((uint32_t)n[2]) << 16U) | (((uint32_t)n[1]) << 8U) | \
                (((uint32_t)n[0]) << 0U)))

#define USB_LONG_TO_BIG_ENDIAN_ADDRESS(n, m)       \
    {                                              \
        m[0] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        m[1] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        m[2] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        m[3] = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_BIG_ENDIAN_ADDRESS(n)                                                     \
    ((uint32_t)((((uint8_t)n[0]) << 24U) | (((uint8_t)n[1]) << 16U) | (((uint8_t)n[2]) << 8U) | \
                (((uint8_t)n[3]) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(n, m)  \
    {                                             \
        m[1] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        m[0] = (((uint16_t)(n)) & 0xFFU);         \
    }

#define USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(n) ((uint16_t)((((uint16_t)n[1]) << 8U) | (((uint16_t)n[0]) << 0U)))

#define USB_SHORT_TO_BIG_ENDIAN_ADDRESS(n, m)     \
    {                                             \
        m[0] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        m[1] = (((uint16_t)(n)) & 0xFFU);         \
    }

#define USB_SHORT_FROM_BIG_ENDIAN_ADDRESS(n) ((uint32_t)((((uint8_t)n[0]) << 8U) | (((uint8_t)n[1]) << 0U)))

#define USB_LONG_TO_LITTLE_ENDIAN_DATA(n, m)                         \
    {                                                                \
        *((uint8_t *)&(m) + 3) = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
        *((uint8_t *)&(m) + 2) = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
        *((uint8_t *)&(m) + 1) = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
        *((uint8_t *)&(m) + 0) = (((uint32_t)(n)) & 0xFFU);          \
    }

#define USB_LONG_FROM_LITTLE_ENDIAN_DATA(n)                                             \
    ((uint32_t)(((*((uint8_t *)&(n) + 3)) << 24U) | ((*((uint8_t *)&(n) + 2)) << 16U) | \
                ((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_DATA(n, m)                       \
    {                                                               \
        *((uint8_t *)&(m) + 1) = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
        *((uint8_t *)&(m)) = ((((uint16_t)(n))) & 0xFFU);           \
    }

#define USB_SHORT_FROM_LITTLE_ENDIAN_DATA(n) ((uint32_t)(((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))))))

#endif

#endif /* __USB_MISC_H__ */
