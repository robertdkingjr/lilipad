/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef BOOTLOADER_CFG_H_
#define BOOTLOADER_CFG_H_

//#define DYNAMIC_BOOT
#define SEE_INDICATOR

/*========================  Memory configuration for Bootloader ================================*/
// user p-flash address range
#define PFLASH_START_ADDR		0u
#define PFLASH_END_ADDR			0x001F3FFUL	// user code end address
#define PFLASH_ADDR_MASS_ERASE_ADDR  PFLASH_START_ADDR
#define PFLASH_SECTOR_SIZE		0x400u		// 1Kwords
#define NO_PFLASH_PAGES_ERASE	((PFLASH_END_ADDR - PFLASH_START_ADDR + 1)/PFLASH_SECTOR_SIZE)

// user d-flash address range
#define DFLASH_START_ADDR		0x68000L
#define DFLASH_END_ADDR			0x6BFFFL
#define DFLASH_ADDR_MASS_ERASE_ADDR	DFLASH_START_ADDR
#define DFLASH_SECTOR_SIZE		0x200		// 512Words
#define NO_DFLASH_PAGES_ERASE	((DFLASH_END_ADDR - DFLASH_START_ADDR + 1)/DFLASH_SECTOR_SIZE)

// user vector table configuration
#define VBA_BASE_ADDRESS		0u
#define INTC_VBA_BITS			13u
/*==============================================================================================*/

/*============================  Memory Map of the specific chip ================================*/
// Flash in program memory
#define PP_FLASH_START		0x0000000UL // program p-flash start address
#define PP_FLASH_END		0x001FFFFUL // program p-flash end address
#define PD_FLASH_START		0x0068000UL // program d-flash start address
#define PD_FLASH_END		0x006BFFFUL // program d-flash end address

// Flash in data memory 
#define DP_FLASH_START		0x0020000UL // data p-flash start address
#define DP_FLASH_END		0x003FFFFUL // data p-flash end address
#define DD_FLASH_START		0x0008000UL // data d-flash start address
#define DD_FLASH_END		0x000BFFFUL // data d-flash end address
/*==============================================================================================*/

// S-record mark for data memory 
#define DATA_MEM_ADDRESS_MASK 	0x02000000UL	// bit25 of the address in a s-record tells if the addr is d-flash or p-flash
// Flash address mark for d-flash
#define FTFL_DFLASH_ADDR_MARK	0x00800000UL    // bit23=1 for d-flash address in a flash erase/program command
// Flash configuration word
#define FTFL_CONFIG_SECURITY_ADDR	(unsigned int*)0x206u	// word address for security word,it's in p-flash
// Flash sector size for FlexNVM/data flash
#define FTFL_DFLASH_SECTOR_SIZE		0x00000400	// 1KBytes size
// Flash sector size for p-flash
#define FTFL_PFLASH_SECTOR_SIZE		0x00000800  // 2KBytes size

// Entrance of user code
#define BOOT_START_ADDRESS_PLACE	(PFLASH_END_ADDR - 2)
#define BOOT_START_ADDRESS_PLACE_L	(PFLASH_END_ADDR - 2)	
#define BOOT_START_ADDRESS_PLACE_H	(PFLASH_END_ADDR - 1)
// Boot delay constant place
#define BOOT_START_DELAY_PLACE		PFLASH_END_ADDR

// Flash parameters and functions
#define FLASHP_PAGE_SIZE			PFLASH_SECTOR_SIZE	// PFLASH size in words
#define FLASHD_PAGE_SIZE			DFLASH_SECTOR_SIZE  // DFLASH size in words
#define FLASH_CONFIG_PAGE			0x200	// 0x200~0x208 in p-flash are used for flash configuration
#define FLASH_SECURITY				0xBE	// enable flash security and back door key

#define fmerase(x)			hfm_command(FTFL_ERASE_BLOCK, (UWord32)FLASH_LO, 0, 1)
#define fperase(x)			hfm_command(FTFL_ERASE_SECTOR, (UWord32)(x), 0, 1)
#define fprogram(x,y,z)     hfm_command(FTFL_PROGRAM_LONGWORD, (UWord32)(x), (UWord16*)(y), (z))

// Timer disable
#define Timer_Disable()		TMRA_0_CTRL &= 0x1fffu
#define Timer_Enable()		TMRA_0_CTRL |= 0x2000u

#define IsInFlash(addr)		(((  (addr) >= PP_FLASH_START) && ((addr) <= PP_FLASH_END)) || (((addr) >= PD_FLASH_START) && ((addr) <= PD_FLASH_END)) \
							|| (((addr) >= (DP_FLASH_START | DATA_MEM_ADDRESS_MASK)) && ((addr) <= (DP_FLASH_END | DATA_MEM_ADDRESS_MASK))) \
							|| (((addr) >= (DD_FLASH_START | DATA_MEM_ADDRESS_MASK)) && ((addr) <= (DD_FLASH_END | DATA_MEM_ADDRESS_MASK))))

// Receiver Buffer Info, Status Flags, and Error Codes
#define ERR_SREC_CKSUM      0x0001u
#define ERR_SREC_TYPE       0x0002u
#define ERR_HFM_ACCERR      0x0004u
#define ERR_HFM_PVIOL       0x0008u
#define ERR_HFM_PROG        0x0010u
#define ERR_ADDR_START      0x0020u
#define ERR_SCI_RX          0x0040u
#define ERR_ADDR_RANGE      0x0080u		// out of given flash address range
#define TX_XOFF             0x1000u
#define RX_HEADER           0x2000u
#define RX_END_OF_FILE      0x4000u
#define DOWNLOAD_OK         (RX_HEADER|RX_END_OF_FILE)
#define DOWNLOAD_ERROR      0x00FFu
#define MAX_SREC_WORDS		128		

// ASCII Character Values
#define XON             17
#define XOFF            19
#define NEWLINE         10
#define RETURN          13

// Application State Machine
#define INITIAL_STATE       (0u)
#define WAIT_FOR_S          (1u)
#define WAIT_FOR_0          (2u)
#define WAIT_FOR_EOL        (3u)
#define ERROR_HANDLER       (4u)
#define APPLICATION_DONE    (5u)
#define PROCESS_DATA        (8u)

// Miscellaneous
#define bitset(x,y)     ((y) |= (x))
#define bitclr(x,y)     ((y) &= ~(x))

#pragma define_section pram_code "pram_code.text" RWX

#endif /* BOOTLOADER_CFG_H_ */
