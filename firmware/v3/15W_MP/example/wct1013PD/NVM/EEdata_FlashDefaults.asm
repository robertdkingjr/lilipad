 ;/******************************************************************************
 ;*
 ;* This software is provided AS IS and without warranty of any kind.
 ;* Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 ;* Copyright 2012-2016 Freescale Semiconductor, Inc.
 ;* Copyright 2017~2019 NXP.
 ;* All rights reserved
 ;*******************************************************************************/
 
 
    SECTION dflash
    org p:
 ; DFLASH Parameter Initialization Table:
 ;  Coil Configuration 
 DC $0000    ;0x00
 DC $0000    ;0x02
 DC $F31C    ;0x04
 DC $0001    ;0x06
 DC $0041    ;0x08
 DC $0190    ;0x0A
 DC $0005    ;0x0C
 DC $0005    ;0x0E
 DC $1F40    ;0x10
 DC $4268    ;0x12
 DC $55F0    ;0x14
 DC $1F40    ;0x16
 DC $1388    ;0x18
 DC $003C    ;0x1A
 DC $F31C    ;0x1C
 DC $0001    ;0x1E
 DC $F31C    ;0x20
 DC $0001    ;0x22
 DC $03E8    ;0x24
 DC $61A8    ;0x26
 DC $1388    ;0x28
 DC $0000    ;0x2A
 DC $1194    ;0x2C
 DC $0000    ;0x2E
 DC $01C2    ;0x30
 DC $0258    ;0x32
 DC $01C2    ;0x34
 DC $0001    ;0x36
 DC $0000    ;0x38
 DC $9400    ;0x3A
 ;  Calibration configuration 
 DC $0EA6    ;0x3C
 DC $3B92    ;0x3E
 DC $ED20    ;0x40
 DC $FFFF    ;0x42
 DC $EEA1    ;0x44
 DC $FFFF    ;0x46
 DC $C266    ;0x48
 DC $00F0    ;0x4A
 DC $B1A8    ;0x4C
 DC $023B    ;0x4E
 DC $000A    ;0x50
 DC $0015    ;0x52
 DC $7D01    ;0x54
 DC $7E21    ;0x56
 DC $4DB0    ;0x58
 DC $001B    ;0x5A
 DC $BEB1    ;0x5C
 DC $0012    ;0x5E
 DC $004E    ;0x60
 DC $0000    ;0x62
 DC $472F    ;0x64
 DC $0021    ;0x66
 DC $5061    ;0x68
 DC $0014    ;0x6A
 DC $FFFE    ;0x6C
 DC $0000    ;0x6E
 DC $06AA    ;0x70
 DC $0000    ;0x72
 DC $C794    ;0x74
 DC $FFFE    ;0x76
 DC $0F7C    ;0x78
 DC $FFFE    ;0x7A
 DC $0000    ;0x7C
 DC $0000    ;0x7E
 DC $000D    ;0x80
 DC $0000    ;0x82
 DC $083E    ;0x84
 DC $0000    ;0x86
 DC $0000    ;0x88
 DC $0000    ;0x8A
 DC $A772    ;0x8C
 DC $FFFE    ;0x8E
 DC $0000    ;0x90
 DC $0000    ;0x92
 DC $0009    ;0x94
 DC $0000    ;0x96
 DC $6225    ;0x98
 DC $0001    ;0x9A
 DC $0ED0    ;0x9C
 DC $0000    ;0x9E
 DC $001C    ;0xA0
 DC $0145    ;0xA2
 DC $0000    ;0xA4
 DC $B800    ;0xA6
 
    ENDSEC 
 
     end
