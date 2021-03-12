/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "derivative.h"
#include "defines.h"
#include "Bootloader_cfg.h"
#include "Buffer.h"


// Function prototypes

// variables
char *data_in = 0;  // pointer to a cell in the circular buffer where data is about to written in
char *data_out;		// pointer to a cell in the circular buffer where data is about to read  
char rx_data[RX_DATA_SIZE];	// a circular buffer to receive the code
UWord16 bytesum;	// checksum of a s-record
char hex_string[5]; // dedicated to a 4-character ASCII string
UWord16 status = 0;		// status of the boot-loading process

/*****************************************************************************
* void mem_copy(long p_start,register long x_start,register unsigned int cnt)
*
* move a bulk of data from p-rom to x-ram
*
* In:  p_start - start address of the data in p-rom
* 	   x_start - start address of the data in x-ram
* 	   cnt - number of data(in words) to be moved	
* Out: n/a
*****************************************************************************/
asm void mem_copy(long p_start,long x_start,unsigned int cnt)
{
	   move.l  a10,r2
	   move.l  b10,r3
	   do      y0,>>end_prom2xram      // copy for 'cnt' times
	   move.w  p:(r2)+,x0              // fetch value at p-address
	   nop
	   nop
	   nop
	   move.w  x0,x:(r3)+              // stash value at x-address 
end_prom2xram:
	   nop
	   rts
}
/*****************************************************************************
* void mem_init(register char *mem, register char init, register UInt16 sz)
*
* Initialize a string with desired character.
*
* In:  mem - String to initialize
* 
* Out: n/a
*****************************************************************************/
asm void mem_init(register char *mem, register char init, register UInt16 sz)
{
        rep     sz
        move.bp init,x:(mem)+
        rts
}

/*****************************************************************************
 * 	Initialize R0,R1,M01, and N for circular buffer (including shadows)
 *****************************************************************************/
void cirBufferInit(void)
{
	asm{
	  swap shadows                    ;// Switch to shadow registers
      moveu.w #(RX_DATA_SIZE-1),M01   ;// Set buffer size
      moveu.w #rx_data,R0             ;// Set starting address
      moveu.w #0xE083U,R1           	  ;// Use R1 for SCI status register
      moveu.w #0,N                    ;// N is unused
      swap shadows                    ;// Switch back to normal registers      
	}
	data_out = rx_data;
	data_in = rx_data;
}


#pragma section pram_code begin

/*****************************************************************************
* void fisr_sci_rx(void)
*
* Fast Interrupt Service Routine for SCI Receive Data Full Interrupts
*
* In:  n/a
*
* Out: n/a
*****************************************************************************/
asm void fisr_sci_rx_full(void)
{
        move.w  x:(r1),y1           ;// SCISR must be read to clear RDRF flag
        move.w  x:(0xE084),y0    ;// Read SCIDR to get RX data
        bftstl  #0x0f00,y1          ;// Check SCISR for RX errors
        bcs     NO_RX_ERRORS        ;// Skip next 2 lines if no RX errors
        move.w  y1,x:(r1)           ;// Clear SCI RX error flags (OR,NF,PF,FE)
        bfset   #ERR_SCI_RX,status  ;// Track errors in "status"
        bra     EXIT_FISR           ;// Exit if RX errors detected
NO_RX_ERRORS:
        move.bp y0,x:(r0)+          ;// Write SCI RX data to circ buffer
        move.w  r0,data_in          ;// Put circ buffer index in "data_in"
EXIT_FISR:
        frtid                       ;// Delayed return from FISR
        nop                         ;// Stall
        nop                         ;// Stall
}

#pragma section pram_code end


#ifdef	DYNAMIC_BOOT
#pragma section pram_code begin
#endif
/*****************************************************************************
* UInt16 char_to_int(char c)
*
* Convert ASCII hexadecimal character into integer.
*
* In:  c - Character to convert
*
* Out: Returns integer representation of ASCII hexadecimal character.
*****************************************************************************/
UInt16 char_to_int(char c)
{
	UInt16 i = (UInt16) c;

	if ((c >= '1') && (c <= '9'))
	{
	  i -= '0';
	}
	else if ((c >= 'A') && (c <= 'F'))
	{  
	  i -= ('A' - 0xAu);
	}
	else
	{
	   i = 0u;
	}
	return (i);
}


/*****************************************************************************
* char int_to_char(UInt16 i)
* 
* Get ASCII character representation of a 4-bit integer value.
*
* In:  i - Integer to convert (4-bit)
* 
* Out: Returns ASCII value used to display 4-bit integer.
*
* Note: This function only uses i[3:0] in translation!
*****************************************************************************/
char int_to_char(UInt16 i)
{
   char j;

   i &= 0x000Fu;
   j = (char) ((i < 10u) ? (i + '0'): (i - 10u + 'A'));

   return (j);
}

/*****************************************************************************
* char *int_to_string (UInt16 i)
*
* Convert integer into 4-character ASCII Hex format.
*
* Input:  i - Integer to convert.
*
* Output: Returns string containing converted value.
*****************************************************************************/
char *int_to_string (UInt16 i)
{
   char *s = hex_string;
   int j;
   char count = 0;

   for (j=12; j>=0; j-=4)
   {
      s[count++] = int_to_char(0xFu & (i >> j));
   }
   s[count++] = '\0';
   
   return (hex_string);
}
/*****************************************************************************
* void sci_tx(char *msg)
*
* Transmit string characters until NULL character is reached.
*
* In:  *msg - Transmit Buffer
*
* Out: n/a
*****************************************************************************/
void sci_tx(char *msg)
{
   char count = 0;
   do 
   {
	   while(!(QSCI0_STAT & QSCI0_STAT_TDRE)){}
	   QSCI0_DATA = (uint16_t)msg[count++];
   }
   while (msg[count]);
}

/*****************************************************************************
* void sci_tx_char(char msg)
*
* Transmit single character over SCI port.
*
* In:  msg - Transmit Character
*
* Out: n/a
*****************************************************************************/
void sci_tx_char(char msg)
{
	while(!(QSCI0_STAT & QSCI0_STAT_TDRE)){}
	QSCI0_DATA = (uint16_t) msg;
}
/*****************************************************************************
* UInt16 get_byte(char **ptr)
*
* Get two characters from circular buffer and returns the ASCII hex value.
* Post increments character pointer using modulo arithmetic.  Accumulates 
* ASCII hex values in "bytesum" global.
* 
* In:   ptr - Pointer to location in circular buffer
*
* Out:  Returns ASCII hexadecimal value of two characters in buffer.
*****************************************************************************/
asm UInt16 get_byte(register char **ptr)
{
        adda    #2,sp                   ;//
        move.w  m01,x:(sp-1)            ;// Save M01 to stack
        moveu.w #(RX_DATA_SIZE-1),M01   ;// Set buffer size
        nop
        moveu.w x:(ptr),r0              ;// Get circular buffer pointer
        move.bp x:(r0)+,y0              ;// Get 1st character (high nibble)
        jsr     char_to_int             ;// Convert into integer
        asll.w  #<4,y0                  ;// Shift to account for high nibble
        move.w  y0,y1                   ;// Accumulate result into Y1
        move.bp x:(r0)+,y0              ;// Get 2nd character (low nibble)
        jsr     char_to_int             ;// Convert into integer
        add     y1,y0                   ;// Accumulate result into Y0 (return value)
        add.w   y0,bytesum              ;// Accumulate result into global
        move.w  r0,x:(ptr)              ;// Update circular buffer pointer
        move.w  #bytesum+2,x0
        moveu.w x:(sp-1),m01            ;// Restore M01
        suba    #2,sp                   ;// Restore stack pointer
        rts
}


/*****************************************************************************
* char get_char(char **ptr)
*
* Get a character from circular buffer.  Post increments character pointer 
* using modulo arithmetic.
*
* In:   ptr - Pointer to location in circular buffer
*
* Out:  Returns character in circular buffer.
*****************************************************************************/
asm char get_char(register char **ptr)
{
        adda    #2,sp                   ;//
        move.w  m01,x:(sp-1)            ;// Save M01 to stack
        moveu.w #(RX_DATA_SIZE-1),M01   ;// Set buffer size
        nop
        moveu.w x:(ptr),r0              ;// Get circular buffer pointer
        move.bp x:(r0)+,y0              ;// Get 1st character
        move.w  r0,x:(ptr)              ;// Save updated pointer
        moveu.w x:(sp-1),m01            ;// Restore M01
        suba    #2,sp                   ;// Restore stack pointer
        rts
}



#ifdef	DYNAMIC_BOOT
#pragma section pram_code end
#endif

