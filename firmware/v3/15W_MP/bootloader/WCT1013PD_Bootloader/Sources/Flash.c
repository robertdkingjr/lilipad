/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#include "defines.h"
#include "Bootloader_cfg.h"
#include "Flash.h"

FTFL_REGISTER16_T *FTFL_REG16;	// Flash control module register
UWord16 *pStatus;				// pointer to record the status of Flash erase/program

// flash command array
TFTFL_CMDOBJ  FTFL_cmdTab[FTFL_ERASE_SECTOR+1] =
{
	{
		1, 0x03					// read1s block
	},
	{
		1, 0x07					// read1s section for p-flash, 0x03 for d-flash
	},	
	{
		5, 0x03					// program check
	},
	{
		5, 0x03					// read resource
	},
	{
		0, 0x03					// program byte/reserved
	},
	{
		0, 0x03					// program word/reserved
	},
	{
		4, 0x03					// program longword
	},
	{
		0, 0x03					// reserved
	},
	
	{
		0, 0x03					// erase flash block
	},
	{
		0, 0x07					// erase flash sector, 0x03 for d-flash
	},
};


// Local function prototypes
unsigned long MemoryRangeCheck(unsigned long targetAddr, unsigned char isInDataMem);
void FTFL_cmd(UWord16 cmd,UWord8 *pParams);

// Function definitions
#ifdef DYNAMIC_BOOT
#pragma section pram_code begin
#endif

/***************************************************************************//*!
*
* @brief   Flash protection detection
*
* @param   void
*
* @return  1: Protected. 0: Not protected
*
******************************************************************************
*
******************************************************************************/
UWord8 FTFL_ProtectionCheck(void)
{
	unsigned char bProtected = 0;
	if((FTFL_REG16->FTFL_FPROT0_FPROT1 != 0xFFFF)||(FTFL_REG16->FTFL_FPROT2_FPROT3 != 0xFFFF))
	{
		bProtected = 1;
	}
	return (bProtected);
}

/***************************************************************************//*!
*
* @brief   Check the memory address and return the address for flash hardware algorithm
* 		   Set variable "status" an error code if the address is out of range
*
* @param   UWord32: flash address which indicates a word
* 		   UWord8:  a flag indicating whether the address is in p-memory or d-memory. 0: d-memory, 1: p-memory
*
* @return  the address for flash hardware algorithm
*
******************************************************************************
*
******************************************************************************/
UWord32 MemoryRangeCheck(UWord32 targetAddr, UWord8 isInDataMem)
{	
	UWord32 ftfl_addr = targetAddr;
	UWord8  bIsDFlash = 0;
	
	if(!isInDataMem)
	{
		// in code/program memory

		if(ftfl_addr >= PP_FLASH_START && ftfl_addr <= PP_FLASH_END)
		{
			// is p-flash
			ftfl_addr -= PP_FLASH_START;	// calculate the offset to the FTFL memory base
		}
		else if(ftfl_addr >= PD_FLASH_START && ftfl_addr <= PD_FLASH_END && (PD_FLASH_END >0))
		{
			// is d-flash
			bIsDFlash = 1;
			ftfl_addr -= PD_FLASH_START;	// calculate the offset to the FTFL memory base			
		}
		else
		{
			// error: out of flash
			(*pStatus) |= ERR_ADDR_RANGE;
		}
	}
	else
	{
		// in data/x memory
		if((ftfl_addr >= DD_FLASH_START) && (ftfl_addr <= DD_FLASH_END) && (DD_FLASH_END >0))
		{
			bIsDFlash = 1;
	    	ftfl_addr -= DD_FLASH_START;	// calculate the offset to the FTFL memory base			
		}
		else
		if(ftfl_addr >= DP_FLASH_START && ftfl_addr <= DP_FLASH_END)
		{
	    	ftfl_addr -= DP_FLASH_START;	// calculate the offset to the FTFL memory base			
		}
		else
		{
			// target address is non-flash area, report error
			(*pStatus) |= ERR_ADDR_RANGE;
		}		
	}
	// Convert word address to byte address used by FTFL
    ftfl_addr <<= 1;
	
	// If it's d-flash, set address bit 23   
	if(bIsDFlash)		   		
	{
		ftfl_addr |= FTFL_DFLASH_ADDR_MARK;  			
	}
	
	return (ftfl_addr);
} 

/***************************************************************************//*!
*
* @brief   Preparing the parameters for a specific flash hardware algorithm command and
* 		   then invoking the execution function. Set the variable "status" an error code if
* 		   there's anything wrong during the command execution
*
* @param   cmd: the command in a flash hardware algorithm
* 		   addr: the byte address in a flash hardware algorithm
*		   *data: the word pointer to a data buffer which is to be programmed into flash
*		   cnt:  the length of this word buffer 	
* @return  the command
*
******************************************************************************
*
******************************************************************************/
UWord16 hfm_command(UWord16 cmd, UWord32 addr, UWord16 *data, UWord16 cnt)
{
   UWord32 ftfl_addr = (UWord32)addr & ~DATA_MEM_ADDRESS_MASK; // convert the address to byte address
   UWord8 *pDataByte = (UWord8*)data;	// convert word address to byte address
   UWord16 err;
   UWord8  bIsDFlash;
   UWord16 bByteCountLeft = cnt<<1; // number of data bytes needing to be programmed
   UWord8 byteCount2Shift;		// the address must be aligned, this variable tells how many bytes are shifted
   UWord8 cmdBytesParamCount;	// number of parameters corresponding to the command
   UWord8  cmdParams[11];		// parameters corresponding to the command
   UWord8  bProgBuf[11];			// a temporary buffer for converting word type data to byte type
   Word16 i;
   
    // Check the flash memory range 
	// Check if it is in the data flash memory and if it is a p-flash or d-flash		
	ftfl_addr = MemoryRangeCheck(ftfl_addr, ((UWord32)addr & DATA_MEM_ADDRESS_MASK)? 1: 0);

    // Check the flash block and determine the address alignment mask
    // phrase aligned or word aligned
	if((ftfl_addr & FTFL_DFLASH_ADDR_MARK) )
	{
		FTFL_cmdTab[cmd].AddrAlignMask = 0x03; // for some commands, default value is 0x07 for both Dflash and Pflash, so it's essential to change it 
	}
	// Align address to longword and calculate the byte count to move
	byteCount2Shift = ftfl_addr & FTFL_cmdTab[cmd].AddrAlignMask;
	ftfl_addr &= ~FTFL_cmdTab[cmd].AddrAlignMask;
	
	// Pad the bytes with all FFs in the prog buffer
	for(i = 0; i < byteCount2Shift; i++)
	{
		bProgBuf[i] = 0xFF;			
	}
	do
	{		
		// Start to fill prog buffer with 4 bytes in each round and fill the
		// command objects
		for(i=byteCount2Shift;i < 4; i++)
		{
			if(bByteCountLeft)
			{
				bProgBuf[i] = *pDataByte++;	
				bByteCountLeft--;					
			}
			else
			{
				bProgBuf[i] = 0xFF;
			}
		}
		
		
		// Fill in the address fields
		cmdParams[0] = ((UWord32)ftfl_addr >> 16) & 0xff;
		cmdParams[1] = ((UWord32)ftfl_addr >> 8) & 0xff;
		cmdParams[2] =  (UWord32)ftfl_addr  & 0xff;
		
		// Fill in the bytes fields
		cmdBytesParamCount = FTFL_cmdTab[cmd].ByteCount;
		for(i = cmdBytesParamCount-1; i >= 0; i--)
		{
			cmdParams[3+i] = bProgBuf[3-i];	// bytes swapped
		}	
		// Wait till the previous command complete
		while(!(FTFL_REG16->FTFL_FCNFG_FSTAT & FTFL_FSTAT_CCIF))
		{		
		}	
		// Error check
		if(FTFL_REG16->FTFL_FCNFG_FSTAT & FTFL_FSTAT_ACCERR)
		{
			// access error
			FTFL_REG16->FTFL_FCNFG_FSTAT =  (FTFL_FSTAT_FPVIOL | FTFL_FSTAT_ACCERR);
			(*pStatus) |= ERR_HFM_ACCERR;
		}
		if(FTFL_REG16->FTFL_FCNFG_FSTAT & FTFL_FSTAT_FPVIOL)
		{
			// protection error
			FTFL_REG16->FTFL_FCNFG_FSTAT =  (FTFL_FSTAT_FPVIOL | FTFL_FSTAT_ACCERR);
			(*pStatus) |= ERR_HFM_PVIOL;			
		}
		// Write FTFL registers and lauch the command
		FTFL_cmd(cmd,cmdParams);
			
		// Move to the next 4 data bytes
		ftfl_addr += 4;
		byteCount2Shift = 0;
		   
	}while(bByteCountLeft>0);
	
   return (cmd);
}


#ifndef	DYNAMIC_BOOT
#pragma section pram_code begin
#endif
/***************************************************************************//*!
*
* @brief   Execution of a flash command
*
* @param   cmd: the command in a flash hardware algorithm
*		   *pParams: the byte pointer to a byte buffer which includes the address and data
*		   	
* @return  NA
*
******************************************************************************
*
******************************************************************************/
void FTFL_cmd( UWord16 cmd, UWord8* pParams)
{
	FTFL_REG16->FTFL_FCCOB0_FCCOB1 = (((cmd&0x00FF)<<8)|pParams[0]);
	FTFL_REG16->FTFL_FCCOB2_FCCOB3 = ((unsigned int)pParams[1]<<8)|pParams[2];
	FTFL_REG16->FTFL_FCCOB4_FCCOB5 = ((unsigned int)pParams[3]<<8)|pParams[4];
	FTFL_REG16->FTFL_FCCOB6_FCCOB7 = ((unsigned int)pParams[5]<<8)|pParams[6];
	FTFL_REG16->FTFL_FCCOB8_FCCOB9 = ((unsigned int)pParams[7]<<8)|pParams[8];
	FTFL_REG16->FTFL_FCCOBA_FCCOBB = ((unsigned int)pParams[9]<<8)|pParams[10];

	
    /* clear CCIF bit to launch the command*/
	FTFL_REG16->FTFL_FCNFG_FSTAT = FTFL_FSTAT_CCIF;  	
//#ifdef	 DYNAMIC_BOOT
    /* For static bootloader, wait till command complete */
    /* check CCIF bit of the flash status register */
    while(!(FTFL_REG16->FTFL_FCNFG_FSTAT & FTFL_FSTAT_CCIF))
    {
        /* wait till CCIF bit is set */
    }
//#endif        
}
#pragma section pram_code end
