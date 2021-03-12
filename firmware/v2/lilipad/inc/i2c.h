//-----------------------------------------------------------------------------
// EFM8BB3_SMBus_Master.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SMBUS_MASTER_H_
#define SMBUS_MASTER_H_

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (SDA, SFR_P1, 2);              // SMBus on P1.2
SI_SBIT(SCL, SFR_P1, 3);               // and P1.3

#define LED_ON   0
#define LED_OFF  1

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY          10000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                  0x00 // SMBus WRITE command
#define  READ                   0x01 // SMBus READ command

// Device addresses (7 bits, LSB is a don't care)
#define  SLAVE_ADDR             (0x61 << 1) // Device address for slave target
#define  IDT_P9242_ADDR         (0x61 << 1) // Power TX
#define  IDT_P9221_ADDR         (0x61 << 1) // Power RX
#define  BQ25895_ADDR           (0x6A << 1) // Battery charger
#define  BQ27750_ADDR           (0x55 << 1) // Battery fuel gauge + protection

// V2.0 Additions
#define  STUSB5400_ADDR         (0x28 << 1) // USB-C controller
#define  I2C_MUX_ADDR 			(0x70 << 1) // MUX the P9242 + P9221 (both 0x61)

// Status vector - top 4 bits only
#define  SMB_MTSTA              0xE0 // (MT) start transmitted
#define  SMB_MTDB               0xC0 // (MT) data byte transmitted
#define  SMB_MRDB               0x80 // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint8_t SMB_DATA_IN;            // Global holder for SMBus data
                                       // All receive data is written here

extern volatile uint8_t SMB_DATA_OUT;           // Global holder for SMBus data.
                                       // All transmit data is read from here

#define P9242_NUM_COMMANDS 12
extern uint8_t* P9242_COMMANDS[P9242_NUM_COMMANDS];
#define P9221_NUM_COMMANDS 24
extern uint8_t* P9221_COMMANDS[P9221_NUM_COMMANDS];

extern uint8_t P9242_REG_DEV_ID_H[2];
extern uint8_t P9242_REG_DEV_ID_L[2];
extern volatile uint8_t SMB_DATA_ARRAY[2];         // Hold 2 bytes of data
extern volatile uint8_t* SMB_DATA_PTR;         //
extern volatile uint8_t SMB_DATA_INDEX;         // Index array
extern volatile uint8_t SMB_DATA_LEN;

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;            // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;            // Counter for the number of errors.


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
//void SMB_Write(uint8_t d);
void SMB_Write(uint8_t ix);
void SMB_Read(void);
void T0_Wait_ms(uint8_t ms);


#endif // SMBUS_MASTER_H_
