
; -------------------------------------------------------
;
;       MWCT10xx_init.asm
;
;	 * This software is provided AS IS and without warranty of any kind. 
;	 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
;	 * If you have not entered into such terms, you have no license rights to use the code.
;	 * Copyright 2012-2016 Freescale Semiconductor, Inc.
;	 * Copyright 2017~2019 NXP.
;	 * All rights reserved.

;	sample code
;
;	description:  main entry point to C code.
;                 setup runtime for C and call main
;
; -------------------------------------------------------


 include "MWCT1013.inc"
 include "MWCT10xx_init.inc"

;===============================

; OMR mode bits

;===============================

NL_MODE                                                          EQU  $8000
CM_MODE                                                          EQU  $0100
XP_MODE                                                          EQU  $0080
R_MODE                                                           EQU  $0020
SA_MODE                                                          EQU  $0010
EX_MODE                                                          EQU  $0008

;===============================

; init

;===============================

        section startup

        XREF  F_stack_addr
        XREF  F_pROM_to_pRAM
        XREF  F_pROM_to_xRAM
        XREF  F_Ldata_size
        XREF  F_Ldata_ROM_addr
        XREF  F_Ldata_RAM_addr
        XREF  F_Lcode_size
        XREF  F_Lcode_ROM_addr
        XREF  F_Lcode_RAM_addr
        org   p:


        GLOBAL Finit_MWCT10xx_ISR_HW_RESET 

        SUBROUTINE "Finit_MWCT10xx_ISR_HW_RESET",Finit_MWCT10xx_ISR_HW_RESET,Finit_MWCT10xx_ISR_HW_RESET_END-Finit_MWCT10xx_ISR_HW_RESET

Finit_MWCT10xx_ISR_HW_RESET:

;
; setup the OMr with the values required by C
; modified by Richy
        bfset #NL_MODE,omr		; ensure NL=1  (enable nested loop)
        nop
        nop
        bfclr #(CM_MODE|XP_MODE|R_MODE|SA_MODE|EX_MODE),omr		; ensure CM=0  (optional for C)
                                                        		; ensure XP=0 to enable harvard architecture
                                                        		; ensure R=0  (required for C)
                                                        		; ensure SA=0 (required for C)
                                                        		; ensure EX=0 to disable external memory access


; setup the m01 register for linear addressing

        move.w  #-1,x0
        moveu.w x0,m01                  ; set the m register to linear addressing

        moveu.w hws,la                  ; clear the hardware stack
        moveu.w hws,la
        nop
        nop


CALLMAIN:                               ; initialize compiler environment

                                        ; initialize the Stack
        move.l  #>>F_Lstack_addr,r0
        bftsth  #$0001,r0
        bcc     noinc
        adda    #1,r0
noinc:
        tfra    r0,sp                   ; set stack pointer too
        move.w  #0,r1
        nop
        move.w  r1,x:(sp)               ; put zero at top of stack
        adda    #1,sp                   ; increment stack
        move.w  r1,x:(sp)               ; put another zero


; Stationery default routine
; 56800E LC register doesn't have 8191 element limitation of 56800
; so we can use hardware loop here

; zeroBSS hardware loop utility
        move.l  #>>F_Xbss_length,r2     ; move count
        move.l  #>>F_Xbss_start_addr,r1 ; start address is BSS start
        move.w  #0,x0                   ; zero value
        nop
        do      r2,>>end_zeroBSS        ; copy for r2 times
        nop
        move.w  x0,x:(r1)+              ; stash value at x address r1
end_zeroBSS:



; pROM-to-xRAM utility
        nop
        move.w  #F_pROM_to_xRAM,r0      ; check for the option
        tst.w   r0
        beq     end_prom2xram           ; if no pROM-to-xRAM, then exit
        nop
        move.l  #>>F_Ldata_size,r2      ; set data size
        move.l  #>>F_Ldata_ROM_addr,r3  ; src address -- pROM data start
        move.l  #>>F_Ldata_RAM_addr,r1  ; dest address -- xRAM data start
        do      r2,>>end_prom2xram      ; copy for r2 times
        move.w  p:(r3)+,x0              ; fetch value at address r3
        nop
        nop
        nop
        move.w  x0,x:(r1)+              ; stash value at address r1
end_prom2xram:
	
; pROM-to-pRAM utility
        nop
        move.w  #F_pROM_to_pRAM,r0      ; check for the option
        tst.w   r0
        beq     end_prom2pram           ; if no pROM-to-xRAM, then exit
        nop
        move.l  #>>F_Lcode_size,r2      ; set data size
        move.l  #>>F_Lcode_ROM_addr,r3  ; src address -- pROM data start
        move.l  #>>F_Lcode_RAM_addr,r1  ; dest address -- xRAM data start
        do      r2,>>end_prom2pram      ; copy for r2 times
        move.w  p:(r3)+,x0              ; fetch value at address r3
        nop
        nop
        nop
        move.w  x0,x:(r1)+              ; stash value at address r1
end_prom2pram:
		


; call main()

        move.w  #0,y0                   ; pass parameters to main()
        move.w  #0,R2
        move.w  #0,R3

        jsr     Fmain                   ; call the user program

loop_at_end_of_app:
		
        debughlt
        bra    loop_at_end_of_app

Finit_MWCT10xx_ISR_HW_RESET_END:

        endsec
