/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
//----------------------
// APPLICATION INCLUDES
//----------------------
#include "derivative.h" 
#include "defines.h"
#include "platformhal.h"
#include "peripheral_cfg.h"
#include "Buffer.h"

//-----------------
// PRIVATE DEFINES
//-----------------
#define OSCTL_TEMP (OCCS_OSCTL1_INIT & 0x3fffu) /* keep internal oscillator enabled */
#define OSCTL_TEMP2 (OCCS_OSCTL1_INIT & 0xC000u) /* shut down internal oscillator if required */

#if OCCS_USE_FACTORY_TRIM_TEMP && OCCS_USE_FACTORY_TRIM_32K
#define TRIM_TEMP_MASK (OCCS_OSCTL2_FREQ_TRIM32K|OCCS_OSCTL2_TEMP_TRIM8M)
#elif OCCS_USE_FACTORY_TRIM_TEMP
#define TRIM_TEMP_MASK (OCCS_OSCTL2_TEMP_TRIM8M)
#elif OCCS_USE_FACTORY_TRIM_32K
#define TRIM_TEMP_MASK (OCCS_OSCTL2_FREQ_TRIM32K)
#else
#define TRIM_TEMP_MASK (0)
#endif

//-------------------
// PRIVATE VARIABLES
//-------------------


//-------------------
// PRIVATE FUNCTIONS
//-------------------

//-----------------------------------------------------------------------------
void OccsInit( void )
{
	UWord16 wtemp;
/* configure external oscillator and clock mode */
#ifdef OCCS_OSCTL1_INIT
    OCCS_OSCTL1 = OSCTL_TEMP;      /* OSCTL, even if PLL is not used */
#endif

/* setup the PLL according to appconfig.h values */
#ifdef OCCS_DIVBY_INIT
    OCCS_DIVBY = OCCS_DIVBY_INIT;  /* PLLDB, even if PLL is not used */
#endif

/* load factory trimming value of the internal relaxation oscillator? */
#if OCCS_USE_FACTORY_TRIM
/* re-trim rxosc frequency? */
#if defined(OCCS_RXOSC_RETRIM_CLK)
#if (OCCS_RXOSC_RETRIM_CLK) != 8000000
    wtemp = SIM_NVMOPT2H&0x3ff;

    #warning Retrim clock is not supported
#endif /* OCCS_RXOSC_RETRIM_CLK != 8000000 */
#endif /* OCCS_RXOSC_RETRIM_CLK defined */

    wtemp = OCCS_OSCTL1;
    wtemp &=  ~OCCS_OSCTL1_FREQ_TRIM8M;
    wtemp |= SIM_NVMOPT2H&SIM_NVMOPT2H_ROSC_8M_FTRIM;
    OCCS_OSCTL1 = wtemp;
#endif /* OCCS_USE_FACTORY_TRIM */

#if OCCS_USE_FACTORY_TRIM_TEMP || OCCS_USE_FACTORY_TRIM_32K
    wtemp = OCCS_OSCTL2;
#if OCCS_OSCTL2_INIT
    //clean both trim value
    wtemp &= ~(OCCS_OSCTL2_FREQ_TRIM32K|OCCS_OSCTL2_TEMP_TRIM8M);
    //copy trim value from INIT macro, when Factory trim is disabled
    wtemp |= (OCCS_OSCTL2_INIT & (~TRIM_TEMP_MASK&(OCCS_OSCTL2_FREQ_TRIM32K|OCCS_OSCTL2_TEMP_TRIM8M)));
#endif
    //clean trim value, which must be initialized
    wtemp &= ~TRIM_TEMP_MASK;
    //copy factory trim
    wtemp |= (SIM_NVMOPT2H&SIM_NVMOPT2H_ROSC_8M_TTRIM | SIM_NVMOPT2L&SIM_NVMOPT2L_ROSC_32K_FTRIM)&TRIM_TEMP_MASK;

    //Save initialization
    OCCS_OSCTL2 = wtemp;
    
#elif defined(OCCS_OSCTL2_INIT)
    OCCS_OSCTL2 = OCCS_OSCTL2_INIT;
#endif

#ifdef OCCS_CTRL_INIT
#if (OCCS_CTRL_INIT & 0x4u) 
    /* Wait about 10ms for external oscillator stabilization */
  /*lint -save  -e586 Disable MISRA rule (2.1) checking. Functionality is implemented using assembler. */
    asm {
      /*
       * Delay
       *   - requested                  : 10 ms @ 4MHz,
       *   - possible                   : 40000 c, 10000000 ns
       */
      move.w #0x4E1B, A                  /* (2 c: 500 ns) number of iterations */
      do A, label0                       /* (8 c: 2000 ns) repeat 19995x nop */
        nop                              /* (1 c: 250 ns) wait for 1 c */
      label0:
    }
    OCCS_OSCTL2 &= ~(OCCS_OSCTL2_COPD);/* Enable crystal oscillator */
    /* switch to external clock source (set PRESC=1) */
    OCCS_CTRL |= OCCS_CTRL_PRECS_0;
    /* Wait 6 NOP’s for the synchronizing circuit to change clocks */
    asm(nop);
    asm(nop);
    asm(nop);
    asm(nop);
    asm(nop);
    asm(nop);
    /* shut down internal oscillator if required */
    OCCS_OSCTL1 |= OSCTL_TEMP2;
#endif /* (OCCS_CTRL_INIT & 0x4) */


#if (OCCS_CTRL_INIT & 1u) /* PLL active ? */
    /* interrupts off and PLL bypassed */
    OCCS_CTRL = OCCS_CTRL_INIT & 0xfeu;

    // wait for PLL lock
    while( !( ( OCCS_STAT & OCCS_STAT_LCK1 ) && ( OCCS_STAT & OCCS_STAT_LCK0 ) ) ){};

    /* PLL is locked: final PLL setup */
    OCCS_CTRL =  OCCS_CTRL_INIT;

    // Wait for clock transition to complete
    while( ( OCCS_STAT & OCCS_STAT_ZSRCS ) != OCCS_STAT_ZSRCS_0 ){};

#else
    /* PLL is locked: final PLL setup */
    OCCS_CTRL =  OCCS_CTRL_INIT;

#endif

#endif
#ifdef OCCS_CLKCHK_INIT
    move.w  OCCS_CLKCHK_INIT,ArchIO.Pll.clkchk
    nop
    nop
#endif

#ifdef OCCS_CLKCHKR_INIT
    OCCS_CLKCHKR = OCCS_CLKCHKR_INIT;
#endif

}

void SimInit(void)
{
#ifdef SIM_CONROL_INIT
	SIM_CTRL = SIM_CONROL_INIT;
#endif
#ifdef SIM_CLKOSR_INIT
	SIM_CLKOUT = SIM_CLKOSR_INIT;
#endif

#ifdef SIM_GPSAL_INIT
	SIM_GPSAL = SIM_GPSAL_INIT;
#endif
#ifdef SIM_GPSBL_INIT
	SIM_GPSBL = SIM_GPSBL_INIT;
#endif
#ifdef SIM_GPSCL_INIT
	SIM_GPSCL = SIM_GPSCL_INIT;
#endif
#ifdef SIM_GPSCH_INIT
	SIM_GPSCH = SIM_GPSCH_INIT;
#endif
#ifdef SIM_GPSEL_INIT
	SIM_GPSEL = SIM_GPSEL_INIT;
#endif
#ifdef SIM_GPSFL_INIT
	SIM_GPSFL = SIM_GPSFL_INIT;
#endif
#ifdef SIM_GPSFH_INIT
	SIM_GPSFH = SIM_GPSFH_INIT;
#endif

#ifdef SIM_PCR_INIT
	SIM_PCR = SIM_PCR_INIT;
#endif
#ifdef SIM_PCE0_INIT
	SIM_PCE0 = SIM_PCE0_INIT;
#endif
#ifdef SIM_PCE1_INIT
	SIM_PCE1 = SIM_PCE1_INIT;
#endif
#ifdef SIM_PCE2_INIT
	SIM_PCE2 = SIM_PCE2_INIT;
#endif
#ifdef SIM_PCE3_INIT
	SIM_PCE3 = SIM_PCE3_INIT;
#endif
#ifdef SIM_SD0_INIT
	SIM_SD0 = SIM_SD0_INIT;
#endif
#ifdef SIM_SD1_INIT
	SIM_SD1 = SIM_SD1_INIT;
#endif
#ifdef SIM_SD2_INIT
	SIM_SD2 = SIM_SD2_INIT;
#endif
#ifdef SIM_SD3_INIT
	SIM_SD3 = SIM_SD3_INIT;
#endif
#ifdef SIM_IPS0_INIT
	SIM_IPS0 = SIM_IPS0_INIT;
#endif
}

void GpioInit(void)
{
	// init GPIO_A
#ifdef GPIO_A_PUR_INIT
	GPIOA_PUR = GPIO_A_PUR_INIT;
#endif
#ifdef GPIO_A_DR_INIT
	GPIOA_DR = GPIO_A_DR_INIT;
#endif
#ifdef GPIO_A_DDR_INIT
	GPIOA_DDR = GPIO_A_DDR_INIT;
#endif
#ifdef GPIO_A_PER_INIT
	GPIOA_PER = GPIO_A_PER_INIT;
#endif
#ifdef GPIO_A_IENR_INIT
	GPIOA_IENR = GPIO_A_IENR_INIT;
#endif
#ifdef GPIO_A_IPOLR_INIT
	GPIOA_IPOLR = GPIO_A_IPOLR_INIT;
#endif
#ifdef GPIO_A_PPMODE_INIT
	GPIOA_PPMODE = GPIO_A_PPMODE_INIT;
#endif
#ifdef GPIO_A_DRIVE_INIT
	GPIOA_DRIVE = GPIO_A_DRIVE_INIT;
#endif

	// init GPIO_B
#ifdef GPIO_B_PUR_INIT
	GPIOB_PUR = GPIO_B_PUR_INIT;
#endif
#ifdef GPIO_B_DR_INIT
	GPIOB_DR = GPIO_B_DR_INIT;
#endif
#ifdef GPIO_B_DDR_INIT
	GPIOB_DDR = GPIO_B_DDR_INIT;
#endif
#ifdef GPIO_B_PER_INIT
	GPIOB_PER = GPIO_B_PER_INIT;
#endif
#ifdef GPIO_B_IENR_INIT
	GPIOB_IENR = GPIO_B_IENR_INIT;
#endif
#ifdef GPIO_B_IPOLR_INIT
	GPIOB_IPOLR = GPIO_B_IPOLR_INIT;
#endif
#ifdef GPIO_B_PPMODE_INIT
	GPIOB_PPMODE = GPIO_B_PPMODE_INIT;
#endif
#ifdef GPIO_B_DRIVE_INIT
	GPIOB_DRIVE = GPIO_B_DRIVE_INIT;
#endif

	// init GPIO_C
#ifdef GPIO_C_PUR_INIT
	GPIOC_PUR = GPIO_C_PUR_INIT;
#endif
#ifdef GPIO_C_DR_INIT
	GPIOC_DR = GPIO_C_DR_INIT;
#endif
#ifdef GPIO_C_DDR_INIT
	GPIOC_DDR = GPIO_C_DDR_INIT;
#endif
#ifdef GPIO_C_PER_INIT
	GPIOC_PER = GPIO_C_PER_INIT;
#endif
#ifdef GPIO_C_IENR_INIT
	GPIOC_IENR = GPIO_C_IENR_INIT;
#endif
#ifdef GPIO_C_IPOLR_INIT
	GPIOC_IPOLR = GPIO_C_IPOLR_INIT;
#endif
#ifdef GPIO_C_PPMODE_INIT
	GPIOC_PPMODE = GPIO_C_PPMODE_INIT;
#endif
#ifdef GPIO_C_DRIVE_INIT
	GPIOC_DRIVE = GPIO_C_DRIVE_INIT;
#endif

	// init GPIO_D
#ifdef GPIO_D_PUR_INIT
	GPIOD_PUR = GPIO_D_PUR_INIT;
#endif
#ifdef GPIO_D_DR_INIT
	GPIOD_DR = GPIO_D_DR_INIT;
#endif
#ifdef GPIO_D_DDR_INIT
	GPIOD_DDR = GPIO_D_DDR_INIT;
#endif
#ifdef GPIO_D_PER_INIT
	GPIOD_PER = GPIO_D_PER_INIT;
#endif
#ifdef GPIO_D_IENR_INIT
	GPIOD_IENR = GPIO_D_IENR_INIT;
#endif
#ifdef GPIO_D_IPOLR_INIT
	GPIOD_IPOLR = GPIO_D_IPOLR_INIT;
#endif
#ifdef GPIO_D_PPMODE_INIT
	GPIOD_PPMODE = GPIO_D_PPMODE_INIT;
#endif
#ifdef GPIO_D_DRIVE_INIT
	GPIOD_DRIVE = GPIO_D_DRIVE_INIT;
#endif

	// init GPIO_E
#ifdef GPIO_E_PUR_INIT
	GPIOE_PUR = GPIO_E_PUR_INIT;
#endif
#ifdef GPIO_E_DR_INIT
	GPIOE_DR = GPIO_E_DR_INIT;
#endif
#ifdef GPIO_E_DDR_INIT
	GPIOE_DDR = GPIO_E_DDR_INIT;
#endif
#ifdef GPIO_E_PER_INIT
	GPIOE_PER = GPIO_E_PER_INIT;
#endif
#ifdef GPIO_E_IENR_INIT
	GPIOE_IENR = GPIO_E_IENR_INIT;
#endif
#ifdef GPIO_E_IPOLR_INIT
	GPIOE_IPOLR = GPIO_E_IPOLR_INIT;
#endif
#ifdef GPIO_E_PPMODE_INIT
	GPIOE_PPMODE = GPIO_E_PPMODE_INIT;
#endif
#ifdef GPIO_E_DRIVE_INIT
	GPIOE_DRIVE = GPIO_E_DRIVE_INIT;
#endif

	// init GPIO_F
#ifdef GPIO_F_PUR_INIT
	GPIOF_PUR = GPIO_F_PUR_INIT;
#endif
#ifdef GPIO_F_DR_INIT
	GPIOF_DR = GPIO_F_DR_INIT;
#endif
#ifdef GPIO_F_DDR_INIT
	GPIOF_DDR = GPIO_F_DDR_INIT;
#endif
#ifdef GPIO_F_PER_INIT
	GPIOF_PER = GPIO_F_PER_INIT;
#endif
#ifdef GPIO_F_IENR_INIT
	GPIOF_IENR = GPIO_F_IENR_INIT;
#endif
#ifdef GPIO_F_IPOLR_INIT
	GPIOF_IPOLR = GPIO_F_IPOLR_INIT;
#endif
#ifdef GPIO_F_PPMODE_INIT
	GPIOF_PPMODE = GPIO_F_PPMODE_INIT;
#endif
#ifdef GPIO_F_DRIVE_INIT
	GPIOF_DRIVE = GPIO_F_DRIVE_INIT;
#endif
}

void WatchdogTimerInit(void)
{
	// Enable Computer Operating Properly peripheral
	// NOTE: The watchdog timer clock and prescaler is configurable

	// Disable the timer
	COP_CTRL = 0u;
	// Set the timeout counter value
#ifdef COP_COPTO_INIT
	COP_TOUT = COP_COPTO_INIT;
#endif
	// Re-initialize the timer
#ifdef COP_COPCTL_INIT
	COP_CTRL = COP_COPCTL_INIT;
#endif
}

void TimerA0Init(void)
{
	//clock is enabled by SIM module

	// Configure control register - mask off source to keep timer from starting
	TMRA_0_CTRL = (QT_A0_CTRL_INIT & ~TMRA_0_CTRL_CM);
	TMRA_0_SCTRL = 0U;
	// Configure the timer compare values
	TMRA_0_COMP1 = QT_A0_CMP1_INIT;
	TMRA_0_COMP2 = 0U;

	// Set the initial timer value
	TMRA_0_LOAD = 0U;

	// Turn on the timer peripheral
	TMRA_0_ENBL |= TMRA_0_ENBL_ENBL_0;  

	// start the timer in application
	//TMRA_0_CTRL |= (QT_A0_CTRL_INIT & TMRA_0_CTRL_CM);

}

void TimerA1Init(void)
{
	//clock is enabled by SIM module

	// Configure control register - mask off source to keep timer from starting
	TMRA_1_CTRL = (QT_A1_CTRL_INIT & ~TMRA_1_CTRL_CM);
	TMRA_1_SCTRL = 0U;
	// Configure the timer compare values
	TMRA_1_COMP1 = QT_A1_CMP1_INIT;
	TMRA_1_COMP2 = 0U;

	// Set the initial timer value
	TMRA_1_LOAD = 0U;

	// Turn on the timer peripheral
	TMRA_0_ENBL |= TMRA_0_ENBL_ENBL_1;  

	// start the timer
	TMRA_1_CTRL |= (QT_A1_CTRL_INIT & TMRA_1_CTRL_CM);

}

void Qsci0Init( void )
{
	// Initialize the QSCI0 controller
	// This connects the system clock to the serial peripheral
	//clock enabled in SIM init


	// Hold QSCI0 in standby state during configuration
	QSCI0_CTRL1 |= QSCI0_CTRL1_RWU;       // 1=Standby state, 0=normal state

	// Configure baud rate
	QSCI0_RATE = SCI_0_SCIBR_INIT;

	// Configure the remainder of the QSCI0 module
	QSCI0_CTRL1 = SCI_0_SCICR_INIT;
	QSCI0_CTRL2 = SCI_0_SCICR2_INIT;
	QSCI0_CTRL3 = SCI_0_SCICR3_INIT;

	// Release QSCI1 from reset state
	QSCI0_CTRL1 &= ~QSCI0_CTRL1_RWU;       // 1=Standby state, 0=normal state

	// Init INTC
	/* INTC_IPR5: QSCI0_RCV=3,QSCI0_RERR=3 */
	INTC_IPR5 |= 0x00F0u;
	/* Fast interrupt 0 setting */
	INTC_FIM0 = 0x33u;  //Vector number for fast interrupt 0
	INTC_FIVAL0 = (volatile UWord32)fisr_sci_rx_full;      //Low address of interrupt routine
	INTC_FIVAH0 = ((volatile UWord32)fisr_sci_rx_full >> 16U) & 0xFFFFU;  //High address of interrupt routine
	/* Fast interrupt 1 setting */
	INTC_FIM1 = 0x34u;
	INTC_FIVAL1 = (volatile UWord32)fisr_sci_rx_full;
	INTC_FIVAH1 = ((volatile UWord32)fisr_sci_rx_full >> 16U) & 0xFFFFU;
	
}

void TimerA0Reset( void )
{
	TMRA_0_CTRL = 0U;
	TMRA_0_SCTRL = 0U;
	TMRA_0_COMP1 = 0U;
	TMRA_0_CSCTRL = 0U;
	TMRA_0_CNTR = 0U;
}

void TimerA1Reset( void )
{
	TMRA_1_CTRL = 0U;
	TMRA_1_SCTRL = 0U;
	TMRA_1_COMP1 = 0U;
	TMRA_1_CSCTRL = 0U;
	TMRA_1_CNTR = 0U;
}

void Qsci0Reset( void )
{
	QSCI0_CTRL1 = 0U;       
	QSCI0_RATE = 0U;
	QSCI0_CTRL2 = 0U;
	INTC_IPR5 &= ~INTC_IPR5_QSCI0_RCV;
	INTC_IPR5 &= ~INTC_IPR5_QSCI0_RERR;
	INTC_FIM0 = 0U;
	INTC_FIVAL0 = 0U;
	INTC_FIVAH0 = 0U;
	INTC_FIM1 = 0U;
	INTC_FIVAL1 = 0U;
	INTC_FIVAH1 = 0U;
}
//------------------
// PUBLIC VARIABLES
//------------------


//------------------
// PUBLIC FUNCTIONS
//------------------


//------------------------------------------------------------------------------
void HwInit(void)
{
	//init COP module
	WatchdogTimerInit();
	//init OCCS
	OccsInit();
	//init SIM module
	SimInit();
	//init GPIO modules
	GpioInit();
	
	//init Timer modules
	TimerA0Init();
	TimerA1Init();
	
	//init QSCI0
	Qsci0Init();

	ENABLE_CPU_INTERRUPTS;
     __EI0();

}
