/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef BUFFER_H_
#define BUFFER_H_

#include "defines.h"

#define RX_DATA_SIZE        2048u //1024 //550

// Function prototypes
extern asm void mem_init(register char *mem, register char init, register UInt16 sz);
extern void cirBufferInit(void);
extern UInt16 char_to_int(char c);
extern char int_to_char(UInt16 i);
extern char *int_to_string (UInt16 i);
extern void sci_tx(char *msg);
extern void sci_tx_char(char msg);
extern asm UInt16 get_byte(register char **ptr);
extern asm char get_char(register char **ptr);
extern asm void mem_copy(long p_start,long x_start,unsigned int cnt);
extern asm void fisr_sci_rx_full(void);


// variables
extern char *data_in;      // pointer to a cell in the circular buffer where data is about to written in
extern char *data_out;		// pointer to a cell in the circular buffer where data is about to read  
extern char rx_data[RX_DATA_SIZE];	// a circular buffer to receive the code
extern UWord16 bytesum;	// checksum of a s-record
extern char hex_string[5]; // dedicated to a 4-character ASCII string
extern UWord16 status;		// status of the boot-loading process




#endif /* BUFFER_H_ */
