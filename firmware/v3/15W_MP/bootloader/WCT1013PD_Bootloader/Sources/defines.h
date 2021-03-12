/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef DEFINES_H_
#define DEFINES_H_


typedef char                char_t;

#ifndef FALSE
  #define  FALSE  0U                   /* Boolean value FALSE. FALSE is defined always as a zero value. */
#endif
#ifndef TRUE
  #define  TRUE   1U                   /* Boolean value TRUE. TRUE is defined always as a non zero value. */
#endif

//#ifndef NULL
//  #define  NULL   0U
//#endif


/* SDK types definition */
typedef signed char    Word8;
typedef unsigned char  UWord8;
typedef short          Word16;
typedef unsigned int   UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;
typedef signed char    Int8;
typedef unsigned char  UInt8;
typedef int            Int16;
typedef unsigned int   UInt16;
typedef long           Int32;
typedef unsigned long  UInt32;
typedef __typeof__(sizeof(0)) ssize_t;
typedef int ibool;



#define __EI0() { asm(bfclr  #0x0300,SR); } /* Enable interrupts of level 0,1,2,3 */
#define __EI1() { asm(bfset  #0x0100,SR); asm(bfclr  #0x0200,SR);} /* Enable interrupts of level 1,2,3 */
#define __EI2() { asm(bfset  #0x0200,SR); asm(bfclr  #0x0100,SR);} /* Enable interrupts of level 2,3 */
#define __EI3() { asm(bfset  #0x0300,SR); } /* Enable interrupts of level 3 */
#define __EI(level)  __EI##level()     /* Enable interrupts of the given level */
#define __DI()  __EI3()                /* Disable interrupts, only level 3 is allowed */
#define DISABLE_CPU_INTERRUPTS                  ( INTC_CTRL |= INTC_CTRL_INT_DIS )
#define ENABLE_CPU_INTERRUPTS                   ( INTC_CTRL &= ~INTC_CTRL_INT_DIS )

#define Cpu_DisableInt()  __DI()       
#define Cpu_EnableInt()	__EI0()

#endif /* DEFINES_H_ */
