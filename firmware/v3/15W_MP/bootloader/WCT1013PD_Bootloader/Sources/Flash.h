/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef FLASH_H_
#define FLASH_H_

#include "derivative.h"

// Must disable flash cache before memory verification
#define disable_flash_cache()	FMC_PFB0CR |= (0x00F00000UL)
#define enable_flash_cache()	FMC_PFB0CR &= ~(0x00F00000UL)

// Flash hardware algorithm operation commands
#define FTFL_VERIFY_BLOCK			0x00u
#define FTFL_VERIFY_SECTION			0x01u
#define FTFL_PROGRAM_CHECK			0x02u
#define FTFL_READ_RESOURCE			0x03u
#define FTFL_PROGRAM_BYTE			0x04u	// reserved
#define FTFL_PROGRAM_WORD			0x05u   // reserved
#define FTFL_PROGRAM_LONGWORD		0x06u	// available in this code
#define FTFL_ERASE_BLOCK			0x08u	// available in this code
#define FTFL_ERASE_SECTOR			0x09u	// available in this code
#define FTFL_PROGRAM_SECTION		0x0Bu
#define FTFL_VERIFY_ALL_BLOCK		0x40u
#define FTFL_READ_ONCE				0x41u
#define FTFL_PROGRAM_ONCE			0x43u
#define FTFL_ERASE_ALL_BLOCK		0x44u
#define FTFL_SECURITY_BY_PASS		0x45u
#define FTFL_PROGRAM_PARTITION		0x80u
#define FTFL_SET_EERAM				0x81u

// Flash margin read settings
#define FTFL_MARGIN_NORMAL			0x0000u
#define FTFL_USER_MARGIN_LEVEL1		0x0001u
#define FTFL_USER_MARGIN_LEVEL0     0x0002u
#define FTFL_FACTORY_MARGIN_LEVEL0	0x0003u
#define FTFL_FACTORY_MARGIN_LEVEL1  0x0004u

// Types
typedef struct
{
	unsigned char ByteCount;	// number of parameters corresponding to a specific command (excluding addresses)
	unsigned char AddrAlignMask; //0x07 for phrase aligned, 0x03 for longword aligned
}TFTFL_CMDOBJ;

typedef struct
{
	unsigned int FTFL_FCNFG_FSTAT;		// STAT is LSB, CNFG is MSB
	unsigned int FTFL_FOPT_FSEC;
	unsigned int FTFL_FCCOB2_FCCOB3;	// FCCOB3:LSB, FCCOB2:MSB
	unsigned int FTFL_FCCOB0_FCCOB1;
	unsigned int FTFL_FCCOB6_FCCOB7;
	unsigned int FTFL_FCCOB4_FCCOB5;
	unsigned int FTFL_FCCOBA_FCCOBB;
	unsigned int FTFL_FCCOB8_FCCOB9;
	unsigned int FTFL_FPROT2_FPROT3;
	unsigned int FTFL_FPROT0_FPROT1;
	unsigned int FTFL_FDPROT_FEPROT;
}FTFL_REGISTER16_T;
extern FTFL_REGISTER16_T *FTFL_REG16;

// Prototypes
extern UWord16 hfm_command(UWord16 cmd, UWord32 addr, UWord16 *data, UWord16 cnt);
extern UWord8  FTFL_ProtectionCheck(void);

// variables
extern UWord16 *pStatus;


#endif /* FLASH_H_ */
