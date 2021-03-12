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
       
        .pIntvectorBoot(RWX): ORIGIN = 0x00000000, LENGTH = 0x000000F0 # Reset and cop vectors
        .pFlashConfig  (RWX): ORIGIN = 0x00000200, LENGTH = 0x00000008 # Reserved for Flash IFR value
        .ppFlash       (RWX): ORIGIN = 0x00000208, LENGTH = 0x0001F1F5 # Primary flash for user code
        .DelayT  	   (RWX): ORIGIN = 0x0001F3FD, LENGTH = 0x00000003 # Bootloading delay time & user code start position
        .pFlash  	   (RWX): ORIGIN = 0x0001F400, LENGTH = 0x00000C00 # Primary flash for bootloader, 3Kwords
  
        .xRAM_bss      (RW) : ORIGIN = 0x00000000, LENGTH = 0x00000800 # 2Kwords for bss
        .xRAM_data     (RWX): ORIGIN = 0x00000800, LENGTH = 0x00000200 # 0.5Kwords for global variables
        .xRAM		   (RW) : ORIGIN = 0x00000A00, LENGTH = 0x00001600 # 5.5Kwords for heaps and stacks
		.pRAM_code	   (RWX): ORIGIN = 0x00062000, LENGTH = 0x00002000 # 8Kwords for code
		.xRAM_code     (RW) : ORIGIN = 0x00002000, LENGTH = 0x00002000 # mirror of .pRAM_code
		
}

KEEP_SECTION { interrupt_vectors.text,interrupt_vectors.text }
FORCE_ACTIVE { Ffisr_sci_rx_full,Frx_data }

SECTIONS {

        .interrupt_vectors :
        {
          F_vector_addr = .;
          # interrupt vectors
          * (interrupt_vectors.text)
        } > .pIntvectorBoot

        .delay_time :
        {
          WRITEW(0x0001F400);	// remember to change this value if different chip is used
          WRITEH(5);			// delay time, in seconds, should not exceeds 5s to support PD
        } > .DelayT

        .FlashConfig : 
        {
			WRITEH(0xFFFF);	# 0, back door key
			WRITEH(0xFFFF); # 1, back door key
			WRITEH(0xFFFF); # 2, back door key
			WRITEH(0xFFFF); # 3, back door key
			WRITEH(0xFFFF); # 4, p-flash protection bits
			WRITEH(0xFFFF); # 5, p-flash protection bits
	#		WRITEH(0xFFFF);	# 6, opt register and security byte
			WRITEH(0xFFBE);	# 6, opt register and security byte
			WRITEH(0xFFFF); # 7, eeprom and d-flash protection bits
		} > .pFlashConfig
		
        .ApplicationCode : 
        {
	    	_pFlash_code_start  = .;
	        # Note: The function _EntryPoint should be placed at the beginning
	        #       of the code
	        OBJECT (Finit_MWCT10xx_ISR_HW_RESET, MWCT10xx_init_asm.obj)
	        
	        
	        # Remaining .text sections
	        * (rtlib.text)
	        * (startup.text)
	        * (fp_engine.text)
	        * (user.text)
	        * (.text)
	
	        # save address for the data starting in pROM
	        Fpflash_mirror = .;
	        Fpflash_index  = .;
    	} > .pFlash

        .data_in_x_ram_bss : 
        {
	        # .bss sections
	        F_Xbss_start_addr = .;
	        _START_BSS = .;
	        OBJECT (Frx_data, Buffer_c.obj)
	        * (rtlib.bss.lo)
	        * (rtlib.bss)
	        * (.bss.char)         # used if "Emit Separate Char Data Section" enabled
	        * (.bss)
	        _END_BSS   = .;
	        F_Xbss_length = _END_BSS - _START_BSS;    
    	} > .xRAM_bss
    	
    	.prog_in_p_flash_ROM : AT(Fpflash_mirror) 
    	{
	        Fpram_start = .;
	        _pram_start = .;
	
	        * (interrupt_vectors.text)
	        * (pram_code.text)
	        
	        # save data end and calculate data block size
	        Fpram_end = .;
	        Fpram_size = Fpram_end - Fpram_start;
	        _pram_size = Fpram_size;
	        Fpflash_mirror2 = Fpflash_mirror + Fpram_size;
	        Fpflash_index = Fpflash_mirror + Fpram_size;
	        
    	} > .pRAM_code
    	
    	.data_in_p_flash_ROM : AT(Fpflash_mirror2) 
    	{
        
	        . = ALIGN(2);
	        Fxram_start = .;
	        * (SciDataSection.data)
	        * (.const.data.char)    # used if "Emit Separate Char Data Section" enabled
	        * (.const.data)
	        * (fp_state.data)
	        * (rtlib.data)
	        * (.data.char)          # used if "Emit Separate Char Data Section" enabled
	        * (.data)
	        
	        # save data end and calculate data block size
	        Fxram_end = .;
	        Fxram_size = Fxram_end - Fxram_start;
	        Fpflash_index = Fpflash_mirror2 + Fxram_size;
    	} > .xRAM_data
    	
    	.data_in_x_ram : 
    	{
	        /* Setup the HEAP address */
	        . = ALIGN(4);
	        _HEAP_ADDR = .;
	        _HEAP_SIZE = 0x0000;
	        _HEAP_END = _HEAP_ADDR + _HEAP_SIZE;
	        . = _HEAP_END;
	
	        /* SETUP the STACK address */
	        _min_stack_size = 0x0200;
	        _stack_addr = _HEAP_END;
	        _stack_end  = _stack_addr + _min_stack_size;
	        . = _stack_end;
	        
	        // store page buffer
	        * (flash_page.page)
	
	        __DATA_END=.;
    	} > .xRAM
    	
    	# Export HEAP and STACK to runtime libraries
	    F_heap_addr    = _HEAP_ADDR;
	    F_heap_end     = _HEAP_END;
	    F_Lstack_addr  = _HEAP_END;
	    F_StackAddr    = _HEAP_END;
	    F_StackEndAddr = _stack_end - 1;
	
	    # runtime code __init_sections uses these globals:
	    F_Ldata_size     = 0;
	    F_Ldata_RAM_addr = 0x0000;
	    F_Ldata_ROM_addr = 0;
	    F_xROM_to_xRAM   = 0x0000;
	    F_pROM_to_xRAM   = 0x0000;
	    F_start_bss      = _START_BSS;
	    F_end_bss        = _END_BSS;
		F_pflash_code_start = Fpflash_mirror;
		F_dram_code_start = _pram_start - 0x60000;
		F_dram_code_size  = _pram_size;
		F_pflash_data_start = Fpflash_mirror2;
		F_dram_data_start = Fxram_start;
		F_dram_data_size  =  Fxram_size;
		
		F_pflash_code_start_h = (F_pflash_code_start/65536)&0xffff;
		F_pflash_code_start_l = F_pflash_code_start &0xffff;
		F_dram_code_start_h = (F_dram_code_start/65536) &0xffff;
		F_dram_code_start_l = F_dram_code_start &0xffff;
		
		F_pflash_data_start_h = (F_pflash_data_start/65536)&0xffff;
		F_pflash_data_start_l = F_pflash_data_start &0xffff;
		F_dram_data_start_h = (F_dram_data_start/65536) &0xffff;
		F_dram_data_start_l = F_dram_data_start &0xffff;
}