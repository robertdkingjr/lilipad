/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
//----------
// INCLUDES
//----------
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "platformhal.h"
#include "Bootloader_cfg.h"
#include "Flash.h"
#include "Buffer.h"


// --------------------------  Variables --------------------------------
UWord32  StartAddress;	// user code start address
UWord16  StartDelay;	// boot loader start delay

#pragma define_section SciDataSection "SciDataSection.data" RW
#pragma section SciDataSection begin

char StrCopyright[]     = "\r\nMWCT1013PD Serial Bootloader v1.0\r\n";
char StrWaitingSrec[]   = "Waiting for application S-Record.\r\n";
char StrLoaded[]        = "Download Complete\r\n";
char StrStarted[]       = "Application started!\r\n\0"; // NOTE: the last ending byte 0 is added 
														// to workaround the compiler issue with
														// even # of chars in the string that 
														// will not be padded with 0 by the compiler
char StrLock[]          = "Lock Flash?";
char StrNewLine[]       = "\r\n\0";
char StrErr[]           = "\r\nError: ";
#pragma section SciDataSection end
 
extern int _pflash_code_start_h,_pflash_code_start_l,_dram_code_start_h,_dram_code_start_l; // code/data start address in flash 
extern int _pflash_data_start_h,_pflash_data_start_l,_dram_data_start_h,_dram_data_start_l; // code/data start address in ram
extern unsigned int _dram_code_size,_dram_data_size;										// code/data size
extern unsigned int pmem_read(register UWord16 *addr);																							// NOTE: Functions that run in ram and global variables
																							// with initial values reside in flash, but run in ram.

UWord16 *pStatusLib;			   // pointer to the status of the execution of the bootloader
UWord16 srec_data[MAX_SREC_WORDS]; // a buffer to store one s-record

// ---------------------------- Function Prototypes -----------------------------
void boot_state_machine(void);	// bootloader state machine
void bootTimerCheck(void);		// check the time before starting the bootloader
void bootExit(void);			// exit bootloader and go to user's code
void srec_decode(void);			// s-record analyse function

void main(void)
{ 
	// initialize the platform
	HwInit();
	
	/***************************************************************************
	* 	Copy functions and data initial values from FLASH to RAM 
	***************************************************************************/
	
	mem_copy(((Word32)(&_pflash_code_start_h)<<16) + (Word32)(&_pflash_code_start_l),\
		   ((Word32)(&_dram_code_start_h)<<16) + (Word32)(&_dram_code_start_l),(UWord16)&_dram_code_size);
	mem_copy(((Word32)(&_pflash_data_start_h)<<16) + (Word32)(&_pflash_data_start_l),\
		   ((Word32)(&_dram_data_start_h)<<16) + (Word32)(&_dram_data_start_l),(UWord16)&_dram_data_size);

	Cpu_DisableInt(); 
	
	// access flash control registers with 16-bit addresses but not byte addresses
	FTFL_REG16 = (FTFL_REGISTER16_T*)((UWord32)(&FTFL_FSTAT)>>1);
	
	/****************************************************************************
	*    Read start delay if no delay jump to user application
	/****************************************************************************/
	StartDelay = pmem_read((UWord16 *)BOOT_START_DELAY_PLACE) & 0x00FFu;  
	
	if(!StartDelay)  
	{
	  bootExit();
	}
	else 
	{
	  // start to count 
	  Timer_Enable();
	}
	
	/*****************************************************************************
	* 	Initialize the circular buffer and the pointers
	*****************************************************************************/
	cirBufferInit();
	/*****************************************************************************
	* 	Fill the circular buffer with 0 
	*****************************************************************************/
	mem_init(rx_data, NULL, RX_DATA_SIZE);
	pStatusLib = &status;	// Status is updated in s-record decoding routing 
	pStatus = &status;	// Status is updated in flash erase/program routing
	
	/*****************************************************************************
	* 	Output a banner through SCI "MWCT1013 Serial Bootloader v1.0"
	*****************************************************************************/
	sci_tx(&StrCopyright[0]);
	
	// Now it is safe to enable interrupts
	Cpu_EnableInt();
	
	/*****************************************************************************
	* 	Start bootloader state machine
	*****************************************************************************/
	boot_state_machine();
	
}

#pragma optimization_level 4

#ifdef	DYNAMIC_BOOT
#pragma section pram_code begin
#endif
/*****************************************************************************
* void state_machine(void)
*
* Bootloader state machine which reads s-record from SCI, and decodes s-record
* as well as programs user's code/data into flash.
*
* In:  n/a
*
* Out: n/a
*****************************************************************************/

void boot_state_machine(void)
{
	UWord16 state;  // state of bootloader
	char *temp_ptr; // a temporary pointer to the circular buffer, this pointer is used to analyse the s-record
	Word16 i; 

	state = INITIAL_STATE;
	temp_ptr = rx_data;

	do{
		// State: Initial State
		if (state == INITIAL_STATE) 
		{
			status = 0u;					// clear status of the bootloader
			sci_tx(&StrWaitingSrec[0]); 
			sci_tx_char(XON);			// inform PC to transfer data
			state = WAIT_FOR_S;
		}

		if (data_in != temp_ptr) // SCI receiving new data ??
		{
			Timer_Disable(); // stop timer's counter

			// State: Wait for Header "S"
			if (state == WAIT_FOR_S) 
			{
				temp_ptr = data_out;	// make "temp_ptr" point at the very first new data in the buffer 
				if(get_char(&temp_ptr) == 'S')// get the data "temp_ptr" points at,and then increment "temp_ptr" 
				{ 
					state = WAIT_FOR_0; 
				}
				else 
				{ 
					get_char(&data_out); // synchronize  "data_out" with "temp_ptr" if the data drawn out is not 'S'
				}
			}
			// State: Wait for Header "0"
			else if (state == WAIT_FOR_0) 
			{
				if(get_char(&temp_ptr) == '0') 
				{ 
					state = WAIT_FOR_EOL; 
				}
				else 
				{
					get_char(&data_out);
					state = WAIT_FOR_S;
				}
			}
			// State: Wait for EOL
			else if (state == WAIT_FOR_EOL) 
			{
				if(get_char(&temp_ptr) == '\r') 
				{
					if (!(status & TX_XOFF)) 
					{
						sci_tx_char(XOFF);
						status |= TX_XOFF;
					}
					srec_decode();
					temp_ptr = data_out;
				}
			}
			else
			{
				;
			}
		}
		else 
		{
			if(status & TX_XOFF) 
			{
				sci_tx_char(XON);
				status &= ~TX_XOFF;
			}
		}
		// State: Error
		if(status & DOWNLOAD_ERROR) 
		{
			sci_tx(StrErr);
			sci_tx(int_to_string(status));
			state = INITIAL_STATE;
			sci_tx(StrNewLine);
		}
      
		bootTimerCheck();
      
	}while (status != DOWNLOAD_OK);
   
	sci_tx(StrNewLine); 
	sci_tx(&StrLoaded[0]);
  	
	bootExit();
} 

/***************************************************************************
* Functions:   bootTimerCheck()
*
* Description: Check TIMER specified in Timer bean
*
* Returns:     1 - timer is reached defined timevalue
*              0 - count in progress
*
* Notes:
*
**************************************************************************/
void bootTimerCheck(void)
{
    if((TMRA_1_SCTRL & TMRA_1_SCTRL_TCF) != 0u)
    {
        TMRA_1_SCTRL &= ~TMRA_1_SCTRL_TCF;
        GPIOE_DR ^= 0x01u;
        if (--StartDelay == 0u) 
        {
        	Timer_Disable();
            bootExit();	
        }
    }
}

/***************************************************************************
* Functions:   bootExit()
*
* Description: read the new appliaction start address
*              and start user's application

* Returns:     None
*
* Notes:
*
**************************************************************************/
#pragma optimization_level 0
void bootExit(void)
{
	int i=0;
	UInt16 securityWord = 0xFFBEu; // disable secure and enable back door key
    
    sci_tx(&StrStarted[0]);	// output a banner
    while(!(QSCI0_STAT & QSCI0_STAT_TIDLE)){}; // wait until the banner's completely transfered 
  
    // Disable flash cache
    disable_flash_cache();
   
    // now check if it is secured by bootloader
    i = (int)pmem_read(FTFL_CONFIG_SECURITY_ADDR);
    if(i == -1)
    {
    	// secured, un-secure it
    	fprogram(FTFL_CONFIG_SECURITY_ADDR,&securityWord,1u);  
    }
    // re-enable flash cache
    enable_flash_cache();
#ifndef	DYNAMIC_BOOT
	// Disable all interrupts
	Cpu_DisableInt();   
    // Disable timer
    Timer_Disable();

    // Restore Vector Base Address (VBA)
    INTC_VBA = (VBA_BASE_ADDRESS) >>(21u-INTC_VBA_BITS);
    
	/****************************************************************************
	/ read user start address
	/****************************************************************************/
	   
    securityWord = pmem_read((UWord16 *)BOOT_START_ADDRESS_PLACE_L);
    i = (int)pmem_read((UWord16 *)BOOT_START_ADDRESS_PLACE_H);
   
    StartAddress = (((UWord32)i) << 16) | securityWord;

    if(StartAddress != 0x1F400UL)
    {
		TimerA0Reset();
		TimerA1Reset();
		Qsci0Reset();
    }
    
    asm(move.l StartAddress, N);
    asm(jmp (N));
#else
    
	// Issue a software reset to restart the chip
    SIM_CTRL |=  SIM_CTRL_SWRst_MASK;
	while(1)
	{
		asm(nop);
	}
#endif    
}

#pragma optimization_level  4
/*****************************************************************************
* void srec_decode(void)
*
* Parse S-record data contained in buffer.
*
* In:  n/a
*
* Out: n/a
*****************************************************************************/
void srec_decode(void)
{
	char type;	// s-record type,an ASCII
	int i,j;	// i must be a signed value here
	UInt16 bytes, data, checksum;
	UInt32 addr; // the address in an s-record,which is a word address
	UInt16 wordsCount; // the length of the data(in words) in an s-record
   
	// Process S-record "type", "byte count", and "address" fields
	get_char(&data_out);		// 'S'
	type = get_char(&data_out); // type:0,3,7
	bytesum = 0u;
	bytes = get_byte(&data_out);// bytes number of this s-record: address+data+checksum
	wordsCount = (bytes-5u)>>1;  // 
	addr=0u;
	for (i=4; i>0; i--)  // get the start address of this s-record
	{
		addr <<= 8;
		addr += (UInt32)get_byte(&data_out);
	}

	// Process S-record "data" field (content depends on "type" field)
	if (type == '0') 
	{

		for (i=((int)bytes)-4-1; i>0; i--) 		// send out all the received bytes: PROG&DATA
		{
			sci_tx_char((char)get_byte(&data_out));
		}
        (*pStatusLib) |= RX_HEADER;
#ifndef	DYNAMIC_BOOT

        // sector by sector erase for p-flash
        for (i=0; i<NO_PFLASH_PAGES_ERASE;i++)  // 1024 words for one sector
        {
        	hfm_command(FTFL_ERASE_SECTOR, ((UInt32)i*FLASHP_PAGE_SIZE+PFLASH_START_ADDR), 0u, 0u);
   
           	if((*pStatusLib) & 0x0FFFu)
           	{
           		break;
           	}
        } 
        // sector by sector erase for d-flash
        for (i=0; i<NO_DFLASH_PAGES_ERASE;i++)  // 512 words for one sector
        {
        	hfm_command(FTFL_ERASE_SECTOR, ((UInt32)i*FLASHD_PAGE_SIZE+DFLASH_START_ADDR), 0, 0);
        	if((*pStatusLib) & 0x0FFF)
        	{
        		break;
        	}      	
        }     
        
#else
        // WARN: flash will be secured after erase block
        // need re-program security at 0x206 
        // mass erase p-flash/primary flash
        if(NO_PFLASH_PAGES_ERASE>0)
        {
        	hfm_command(FTFL_ERASE_BLOCK,PFLASH_ADDR_MASS_ERASE_ADDR,0,0);
        }
      
        // mass erase d-flash/secondary flash
        if(NO_DFLASH_PAGES_ERASE>0)
        {
        	hfm_command(FTFL_ERASE_BLOCK,DFLASH_ADDR_MASS_ERASE_ADDR,0,0);	 
        }
#endif
	}
	else if (type == '3') 
	{
		// NOTE: the following statement may miss the last byte!
		// ASSUME: CW will always pad the bytes in s-record to words
		j=0;
		for (i=((int)bytes)-4-1; i>0; i-=2) 
		{
			data = get_byte(&data_out);
			data += (i>1) ? (get_byte(&data_out) << 8): 0xFF00u;

			// store the data into srec_data
			srec_data[j++] = data;
			if ((*pStatusLib) & DOWNLOAD_ERROR)
			{
				break;
			}
		}
		if(IsInFlash(addr))
		{
			// Now program srec data in words
			fprogram(addr,&srec_data[0],wordsCount);
		}
	}
	else if (type == '7') 
	{	 
		(*pStatusLib) |= RX_END_OF_FILE; 
//		fprogram(BOOT_START_ADDRESS_PLACE, (&addr), 2);	// save new application start address read from S7 record into 
  														// BOOT_START_ADDRESS_PLACE
	}
	else  
	{ 
		(*pStatusLib) |= ERR_SREC_TYPE; 
	}

	// Process S-record "checksum" field (compare with calculated value).
	if (!((*pStatusLib) & DOWNLOAD_ERROR)) 
	{
		checksum = (~bytesum) & 0xFFu;
		if (checksum != get_byte(&data_out)) 
		{ 
			(*pStatusLib) |= ERR_SREC_CKSUM; 
		}
		// Discard EOL characters
		get_char(&data_out);
	}

	// Use "*" to indicate progress
#ifdef SEE_INDICATOR   
	sci_tx_char('*'); // one indicator stands for one record
#endif   
}

#ifdef	DYNAMIC_BOOT
#pragma section pram_code end
#endif

unsigned int pmem_read(register UWord16 *addr)
{
   register unsigned int data;

   asm(move.w  p:(addr)+,data);
   return data;
}
