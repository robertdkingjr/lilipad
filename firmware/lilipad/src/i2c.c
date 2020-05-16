/*
 * i2c.c
 *
 *  Created on: Apr 19, 2020
 *      Author: Robert King
 */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>
#include "InitDevice.h"
#include "i2c.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t SMB_DATA_IN;                   // Global holder for SMBus data
                                       // All receive data is written here

volatile uint8_t SMB_DATA_OUT;         // Global holder for SMBus data.
                                       // All transmit data is read from here


// Hardcoded I2C commands, use PTR to select and use them
uint8_t P9242_REG_DEV_ID_L[2] = {0x01, 0x04};
uint8_t P9242_REG_DEV_ID_H[2] = {0x01, 0x05};
uint8_t P9242_REG_FW_MAJOR_L[2] = {0x01, 0x0C};
uint8_t P9242_REG_FW_MAJOR_H[2] = {0x01, 0x0D};
uint8_t P9242_REG_FW_MINOR_L[2] = {0x01, 0x0E};
uint8_t P9242_REG_FW_MINOR_H[2] = {0x01, 0x0F};
uint8_t P9242_REG_SYSTEM_STATE[2] = {0x06, 0xE0};
uint8_t P9242_REG_SYSTEM_STATUS[2] = {0x06, 0xE1};
uint8_t P9242_REG_COIL_CURRENT_L[2] = {0x06, 0xE2};
uint8_t P9242_REG_COIL_CURRENT_H[2] = {0x06, 0xE3};
uint8_t P9242_REG_COIL_VOLTAGE_L[2] = {0x06, 0xE4};
uint8_t P9242_REG_COIL_VOLTAGE_H[2] = {0x06, 0xE5};

// Array of all commands
uint8_t* P9242_COMMANDS[P9242_NUM_COMMANDS] = {
		P9242_REG_DEV_ID_L,
		P9242_REG_DEV_ID_H,
		P9242_REG_FW_MAJOR_L,
		P9242_REG_FW_MAJOR_H,
		P9242_REG_FW_MINOR_L,
		P9242_REG_FW_MINOR_H,
		P9242_REG_SYSTEM_STATE,
		P9242_REG_SYSTEM_STATUS,
		P9242_REG_COIL_CURRENT_L,
		P9242_REG_COIL_CURRENT_H,
		P9242_REG_COIL_VOLTAGE_L,
		P9242_REG_COIL_VOLTAGE_H,
};


// Hardcoded I2C commands, use PTR to select and use them
uint8_t P9221_REG_PART_NUM_L[2] = {0x00, 0x00};
uint8_t P9221_REG_PART_NUM_H[2] = {0x00, 0x01};
uint8_t P9221_REG_FW_MAJOR_L[2] = {0x00, 0x04};
uint8_t P9221_REG_FW_MAJOR_H[2] = {0x00, 0x05};
uint8_t P9221_REG_FW_MINOR_L[2] = {0x00, 0x04};
uint8_t P9221_REG_FW_MINOR_H[2] = {0x00, 0x05};

uint8_t* P9221_COMMANDS[P9221_NUM_COMMANDS] = {
		P9221_REG_PART_NUM_L,
		P9221_REG_PART_NUM_H,
		P9221_REG_FW_MAJOR_L,
		P9221_REG_FW_MAJOR_H,
		P9221_REG_FW_MINOR_L,
		P9221_REG_FW_MINOR_H
};


uint8_t SMB_DATA_ARRAY[2];         // Default array to hold I2C transactions
volatile uint8_t* SMB_DATA_PTR = SMB_DATA_ARRAY;

volatile uint8_t SMB_DATA_INDEX;         // Index array
volatile uint8_t SMB_DATA_LEN;         // Num of bytes in next SMBUS transmission

uint8_t TARGET;                        // Target SMBus slave address

volatile bool SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bool SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                   // Counter for the number of errors.


//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMB_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : data
//
// Writes a single byte to the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Write outgoing data to the <SMB_DATA_OUT> variable
// 3) Call SMB_Write()
//
//-----------------------------------------------------------------------------
//void SMB_Write (volatile uint8_t d)
//void SMB_Write (uint8_t ix)
void SMB_Write (uint8_t len)
{
	// ORIGINAL METHOD: seems to get compiled out to last call data
//	SMB_DATA_OUT = d;              // Define next outgoing byte

//	 NEW METHOD: assign index for deferencing data array
//	SMB_DATA_INDEX = ix;

	SMB_DATA_LEN = len;

	while(SMB_BUSY);                    // Wait for SMBus to be free.
	SMB_BUSY = 1;                       // Claim SMBus (set to busy)
	SMB_RW = 0;                         // Mark this transfer as a WRITE
	SMB0CN0_STA = 1;                    // Start transfer
	while(SMB_BUSY);                    // Wait for transfer to complete

}

//-----------------------------------------------------------------------------
// SMB_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Reads a single byte from the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Call SMB_Write()
// 3) Read input data from <SMB_DATA_IN> variable
//
//-----------------------------------------------------------------------------
void SMB_Read (void)
{
   while(SMB_BUSY != 0);               // Wait for transfer to complete
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ
   SMB0CN0_STA = 1;                    // Start transfer

   while(SMB_BUSY);                    // Wait for transfer to complete
}

//-----------------------------------------------------------------------------
// T0_Wait_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t ms - number of milliseconds to wait
//                        range is full range of character: 0 to 255
//
// Configure Timer0 to wait for <ms> milliseconds using SYSCLK as its time
// base.
//
//-----------------------------------------------------------------------------
void T0_Wait_ms (uint8_t ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON0 |= 0x04;                     // Timer0 counts SYSCLKs

   while(ms)
   {
      TCON_TR0 = 0;                    // Stop Timer0
      TH0 = ((-(SYSCLK/1000)) >> 8);   // Overflow in 1ms
      TL0 = ((-(SYSCLK/1000)) & 0xFF);
      TCON_TF0 = 0;                    // Clear overflow indicator
      TCON_TR0 = 1;                    // Start Timer0
      while(!TCON_TF0);                // Wait for overflow
      ms--;                            // Update ms counter
   }

   TCON_TR0 = 0;                       // Stop Timer0
}
