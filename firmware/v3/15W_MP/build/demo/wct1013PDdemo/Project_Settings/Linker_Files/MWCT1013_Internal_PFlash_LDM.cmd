## ###################################################################
##
##      * This software is provided AS IS and without warranty of any kind. 
##      * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
##      * If you have not entered into such terms, you have no license rights to use the code.
##      * Copyright 2012-2016 Freescale Semiconductor, Inc.
##      * Copyright 2017~2019 NXP.
##      * All rights reserved.
##
##     Processor : MWCT1013
##
##     Abstract  :
##
##     This file is used by the linker. It describes files to be linked,
##     memory ranges, stack size, etc. For detailed description about linker
##     command files see CodeWarrior documentation.
##
## ###################################################################

MEMORY {
        
        ## Program Memory space 
     	.p_interrupts_ROM     (RX)  : ORIGIN = 0x000000, LENGTH = 0x000200 # reserved for interrupt vectoring
	    .p_flash_config_DATA  (RX)  : ORIGIN = 0x000200, LENGTH = 0x000008 # 16 bytes reserved for IFR
     	.p_flash_ROM          (RX)  : ORIGIN = 0x000208, LENGTH = 0x01F000-0x208 # primary location for code to be run - to 0x0ffff
        .p_dflash_DATA        (RX)  : ORIGIN = 0x01F000, LENGTH = 0x0003FD  # reserved for EEPROM emulation (1 2048 byte sectors)  
        .xBootCfg             (RWX) : ORIGIN = 0x01F3FD, LENGTH = 0x0003 
        .p_RAM_code           (RX)  : ORIGIN = 0x060100, LENGTH = 0x0003E0 # (alias of 01E0 for data RAM space)  
#     	.p_external           (RX)  : ORIGIN = 0x020000, LENGTH = 0x040000 # 256k words not used in this device
#     	.p_ram_RAM_code       (RX)  : ORIGIN = 0x060200, LENGTH = 0x001600 # (alias of 0000 for data RAM space)
#     	.p_ram_RAM_data       (RX)  : ORIGIN = 0x061800, LENGTH = 0x001800 # (alias of 1800 for data RAM space)

#     	.p_unused1            (RX)  : ORIGIN = 0x064000, LENGTH = 0x004000 # unused address space
     	.p_boot_ROM           (RX)  : ORIGIN = 0x068000, LENGTH = 0x004000 # secondary program/data flash
#     	.p_unused2            (RX)  : ORIGIN = 0x06C000, LENGTH = 0x194000 # unused address space out to 0x1fffff (end of 21 bit program address space)
 

    	.p_RAM_data           (RX)  : ORIGIN = 0x0004E0,  LENGTH = 0x004000-0x4E0    # internal xRAM data mirror
                                                                       		         # for pROM-to-xRAM copy

		## Data Memory space 
		.x_ddm_data_RAM        (RW)  : ORIGIN = 0x000000, LENGTH = 0x000100 # for storage DDM data
#    	.x_internal_RAM_code   (RW)  : ORIGIN = 0x000400, LENGTH = 0x001400 # alias of 60000 for program space
    	.x_internal_RAM_data   (RW)  : ORIGIN = 0x0004E0, LENGTH = 0x004000-0x4E0 # alias of 60000 for program space
#    	.reserved_1            (RW)  : ORIGIN = 0x004000, LENGTH = 0x004000 # unused
#    	.x_boot_ROM            (RW)  : ORIGIN = 0x008000, LENGTH = 0x004000 # secondary (boot) program/data flash 
    	.x_platform            (RW)  : ORIGIN = 0x00C000, LENGTH = 0x002000 # on-platform peripheral address space
    	.x_peripherals         (RW)  : ORIGIN = 0x00E000, LENGTH = 0x002000 # off-platform perihperal address space
#    	.reserved_2            (RW)  : ORIGIN = 0x010000, LENGTH = 0x00E000
    	.x_EEE                 (RW)  : ORIGIN = 0x01E000, LENGTH = 0x000400 # max used is 1k 
    	.x_EEE_reserved        (RW)  : ORIGIN = 0x01E400, LENGTH = 0x001C00 # total ammount address space reserved for EEE is 8k
#    	.x_interrupts_ROM      (RW)  : ORIGIN = 0x020000, LENGTH = 0x000200 # reserved for interrupts
#    	.x_reserved_IFR        (RW)  : ORIGIN = 0x020200, LENGTH = 0x000008 # reserved for IFR
#    	.x_flash_ROM           (RW)  : ORIGIN = 0x020208, LENGTH = 0x00FDF8 # primary location for code to be run - to 0x2ffff
#    	.x_external            (RW)  : ORIGIN = 0x040000, LENGTH = 0x040000 # 256k words not used in this device
    	.x_EOnCE               (RW)  : ORIGIN = 0xFFFF00, LENGTH = 0x000100 # debugger and other uses at end of memory space (24 bit)

}

KEEP_SECTION { interrupt_vectors.text }
KEEP_SECTION { dflash.text }
KEEP_SECTION { flash_config.text }


SECTIONS {
		.ApplicationConfiguration :
       	{
       		# Store the application entry point
       		WRITEW(Finit_MWCT10xx_ISR_HW_RESET);
       		#Bootloader start delay in seconds
       		WRITEH(5);		// delay time, in seconds, should not exceeds 5s to support PD
       	} > .xBootCfg

        .interrupt_vectors :
        {
          # interrupt vectors
          * (interrupt_vectors.text)
        } > .p_interrupts_ROM

    .dflash :
    {
        * (dflash.text)
        
    } > .p_dflash_DATA

    .flash_config :
    {
        * (flash_config.text)
        
    } > .p_flash_config_DATA

        .ApplicationCode :
        {

              F_Pcode_start_addr = .;

              # .text sections
              * (.text)
              * (.init)
              * (rtlib.text)
              * (startup.text)
              * (interrupt_routines.text)
              * (fp_engine.text)
              * (ll_engine.text)
              * (user.text)
              * (.data.pmem)

              F_Pcode_end_addr = .;

              # save address where for the data start in pROM
              . = ALIGN(2);
              __pROM_code_start = .;

        } >.p_flash_ROM

        # AT sets the download address
        # the download stashes the data just after the program code
        # as determined by our saved pROM data start variable

        .code_in_p_flash_ROM : AT(__pROM_code_start)
        {
        	__xRAM_code_start = .;
        	* (.code_in_ram)
        	. = ALIGN(2);
            __xRAM_code_end = .;
            __code_size = __xRAM_code_end - __xRAM_code_start;
            __pROM_data_start = __pROM_code_start + __code_size;
        } >.p_RAM_code
        
       
        .data_in_p_flash_ROM : AT(__pROM_data_start)
        {
              # the data sections flashed to pROM
              # save data start so we can copy data later to xRAM

              __xRAM_data_start = .;
              # offset to ensure no vars with location zero
      
              . = . + 2;

              # .data sections
              * (.const.data.char)  # used if "Emit Separate Char Data Section" enabled
              * (.const.data)

              * (fp_state.data)
              * (rtlib.data)
              * (.data.char)        # used if "Emit Separate Char Data Section" enabled
              * (.data)
              . = ALIGN(2);
			  F__sinit__ = .;
		      STATICINIT
		      F__sinit__end = .;
              # save data end and calculate data block size
              . = ALIGN(2);
              __xRAM_data_end = .;
              __data_size = __xRAM_data_end - __xRAM_data_start;

        } > .p_RAM_data          # this section is designated as p-memory
                                 # with X flag in the memory map
                                 # the start address and length map to
                                 # actual internal xRAM
 		.DDMData:
        {
	         . = ALIGN(512);
	         * (.dma_data)
        } >.x_ddm_data_RAM
        
        
        .ApplicationData :
        {
              # save space for the pROM data copy
              . =  __data_size + .;

              # .bss sections
              * (rtlib.bss.lo)
              * (rtlib.bss)
              . = ALIGN(4);
              F_Xbss_start_addr = .;
              _START_BSS = .;
               * (.wct_debug_buffer)
              . = ALIGN(2);
              * (.bss.char)         # used if "Emit Separate Char Data Section" enabled
              * (.bss)
              . = ALIGN(2);         # used to ensure proper functionality of the zeroBSS hardware loop utility
              _END_BSS   = .;
              F_Xbss_length = _END_BSS - _START_BSS;

              /* SETUP the STACK address */
              . = ALIGN(4);
              _min_stack_size = 0x00000200;
              _stack_addr = .;
              _stack_end  = _stack_addr + _min_stack_size;
              . = _stack_end;
              
             /* Setup the HEAP address */
              _RAM_END_ADDR = 0x004000;
              _HEAP_ADDR = .;
              _HEAP_SIZE = _RAM_END_ADDR - _HEAP_ADDR; # all remaining RAM used as HEAP
              _HEAP_END = _HEAP_ADDR + _HEAP_SIZE;
              . = _HEAP_END;
              
              /* EXPORT HEAP and STACK runtime to libraries */
              F_heap_addr   = _HEAP_ADDR;
              F_heap_end    = _HEAP_END;
              F_Lstack_addr = _stack_addr;
              F_StackAddr = _stack_addr;
              F_StackEndAddr = _stack_end - 1;

              # runtime code __init_sections uses these globals:

              F_Ldata_size     = __data_size;
              F_Ldata_RAM_addr = __xRAM_data_start;
              F_Ldata_ROM_addr = __pROM_data_start;
                          
              F_Lcode_size     = __code_size;
              F_Lcode_RAM_addr = __xRAM_code_start - 0x060000;
              F_Lcode_ROM_addr = __pROM_code_start;

              F_pROM_to_pRAM   = 0x0001;
              F_pROM_to_xRAM   = 0x0001;

              F_start_bss   = _START_BSS;
              F_end_bss     = _END_BSS;

              __DATA_END=.;

        } > .x_internal_RAM_data

}
