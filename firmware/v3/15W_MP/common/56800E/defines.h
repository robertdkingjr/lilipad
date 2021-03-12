/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
 *******************************************************************************/

#ifndef __DEFINES_H__
#define __DEFINES_H__

//Note: This file defines mcu core-related types, macros...


//----------
// TYPEDEFS
//----------
typedef signed char       	int8_t;
typedef signed int        	int16_t;
typedef signed long int   	int32_t;

typedef unsigned char     	uint8_t;
typedef unsigned int     	uint16_t;
typedef unsigned long int   uint32_t;

typedef volatile uint8_t    vuint8_t;
typedef volatile uint16_t   vuint16_t;
typedef volatile uint32_t   vuint32_t;

// types for passing MISRA
typedef unsigned char     boolean;
typedef unsigned char     uint8;
typedef unsigned int      uint16;
typedef unsigned long     uint32;
typedef signed char       sint8;
typedef signed int        sint16;
typedef signed long       sint32;
typedef float             float32;

//---------
// DEFINES
//---------

#define FALSE       (0u)
#define TRUE        (1u)

#ifndef NULL
#define NULL        ((void*)0)
#endif

#define PI          (3.1415926)

#define BIT0    (0x1u)
#define BIT1    (0x2u)
#define BIT2    (0x4u)
#define BIT3    (0x8u)
#define BIT4    (0x10u)
#define BIT5    (0x20u)
#define BIT6    (0x40u)
#define BIT7    (0x80u)
#define BIT8    (0x100u)
#define BIT9    (0x200u)
#define BIT10   (0x400u)
#define BIT11   (0x800u)
#define BIT12   (0x1000u)
#define BIT13   (0x2000u)
#define BIT14   (0x4000u)
#define BIT15   (0x8000u)
#define BIT16   (0x10000u)
#define BIT17   (0x20000u)
#define BIT18   (0x40000u)
#define BIT19   (0x80000u)
#define BIT20   (0x100000u)
#define BIT21   (0x200000u)
#define BIT22   (0x400000u)
#define BIT23   (0x800000u)
#define BIT24   (0x1000000u)
#define BIT25   (0x2000000u)
#define BIT26   (0x4000000u)
#define BIT27   (0x8000000u)
#define BIT28   (0x10000000u)
#define BIT29   (0x20000000u)
#define BIT30   (0x40000000u)
#define BIT31   (0x80000000u)

#define NVM_BYTE_POINTER                        __pmem uint8 *

///////////////////////////////////////////////////////////////////////////////
// Interrupts 
///////////////////////////////////////////////////////////////////////////////
#define __EI0() do { asm(bfclr  #0x0300,SR); }while(0)                        /* Enable interrupts of level 0,1,2,3 */
#define __EI1() do { asm(bfset  #0x0100,SR); asm(bfclr  #0x0200,SR);}while(0) /* Enable interrupts of level 1,2,3 */
#define __EI2() do { asm(bfset  #0x0200,SR); asm(bfclr  #0x0100,SR);}while(0) /* Enable interrupts of level 2,3 */
#define __EI3() do { asm(bfset  #0x0300,SR); }while(0)                        /* Enable interrupts of level 3 */
#define __EI(level)  __EI##level()                                            /* Enable interrupts of the given level */
#define __DI()  __EI3()                                                       /* Disable interrupts, only level 3 is allowed */

#endif // __DEFINES_H__

