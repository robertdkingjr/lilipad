/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "defines.h"
#include "cpu.h"

#include "appCfg.h"
#include "Wct_Lib.h"
#include "NVM.h"

#include "freemaster_cfg.h"

#include "adc.h"
#include "dma.h"
#include "pwm.h"
#include "timer.h"
#include "sim.h"
#include "flash.h"
#include "trigger.h"
#include "qsci.h"
#include "dac.h"
#include "gpio.h"
#include "cmp.h"
#include "flash.h"
#include "wdg.h"
#include "sim.h"
#include "occs.h"
#include "i2c.h"
#include "pmc.h"

#if DIGITAL_DCDC
#include "gflib.h"
#endif

#include "systemDebug.h"
#include "systemTimers.h"
#include "systemUSBPower.h"
#include "Wct_LibParams.h"
#include "hal.h"
#include "math.h"

#define HAL_PWM_DEAD_TIME                  (20UL)   //ns

#define HAL_RAIL_SENSE_SHIFT_BITS           10   //shift for better resolution
#define HAL_RAIL_SENSE_UP_RESISTOR     36000.0   //ohm
#define HAL_RAIL_SENSE_DOWN_RESISTOR    4700.0   //ohm
#define HAL_RAIL_SENSE_MULTI_F      ((HAL_RAIL_SENSE_UP_RESISTOR+HAL_RAIL_SENSE_DOWN_RESISTOR)/HAL_RAIL_SENSE_DOWN_RESISTOR)
#define HAL_RAIL_SENSE_MULTI_U      ((uint32)(HAL_RAIL_SENSE_MULTI_F*(1UL<<HAL_RAIL_SENSE_SHIFT_BITS)))

#define HAL_BATTERY_SENE_SHIFT_BITS         10      //shift for better resolution
#define HAL_BATTERY_SENSE_UP_RESISTOR     91000.0   //ohm
#define HAL_BATTERY_SENSE_DOWN_RESISTOR   13000.0   //ohm
#define HAL_BATTERY_SENSE_MULTI_F      ((HAL_BATTERY_SENSE_UP_RESISTOR+HAL_BATTERY_SENSE_DOWN_RESISTOR)/HAL_BATTERY_SENSE_DOWN_RESISTOR)
#define HAL_BATTERY_SENSE_MULTI_U  ((uint32)(HAL_BATTERY_SENSE_MULTI_F*(1UL<<HAL_BATTERY_SENE_SHIFT_BITS)))

#define HAL_INPUT_CURRENT_SENSE_SHIFT_BITS   10     //shift for better resolution
#define HAL_INPUT_CURRENT_SENSE_RESISTOR   0.01   //ohm
#define HAL_INPUT_CURRENT_AMPLIFY_MULTI     100.0   //current sensor gain
#define HAL_INPUT_CURRENT_MULTI_COEFF_U     ((uint32)((1UL<<HAL_INPUT_CURRENT_SENSE_SHIFT_BITS)/(HAL_INPUT_CURRENT_SENSE_RESISTOR*HAL_INPUT_CURRENT_AMPLIFY_MULTI)))

#define HAL_ANALOG_PING_PULSE_NUM       	 2U
#define HAL_ANALOG_PING_ADC_SAMPLE_NUM       8U
#define HAL_ANALOG_PING_FREQ            127772UL    //Hz
#define HAL_ANALOG_PING_DUTY            LIB_MAX_DUTY_CYCLE
#define HAL_ANALOG_PING_PHASE           (LIB_MAX_PHASE_SHIFT)
#define HAL_ANALOG_PING_ADC_TRIG_POS    1U

//shift value for better resolution, careful to set it to prevent 
//HAL_RESONANCE_CAPACITOR_RESISTANCE_WITHOUT_FREQ from overflow
#define HAL_COIL_CURRENT_SHIFT_BITS          10
#define HAL_RESONANCE_CAPACITOR    500.0     //nF
#define HAL_DDM_FREQ            127772.0     //Hz
#define HAL_DDM_SENSE_CAPACITOR   4700.0     //pF
#define HAL_DDM_SENSE_RESISTOR   51000.0     //ohm
#define HAL_DDM_UP_RESISTOR       3900.0     //ohm
#define HAL_DDM_DOWN_RESISTOR     5110.0     //ohm
#define HAL_SENSE_CAPACITOR_RESISTANCE      (1e12/(2*PI*HAL_DDM_FREQ*HAL_DDM_SENSE_CAPACITOR))
#define HAL_DDM_DIV_UP_RESISTANCE           (HAL_SENSE_CAPACITOR_RESISTANCE + HAL_DDM_SENSE_RESISTOR)
#define HAL_DDM_DIV_DOWN_RESISTANCE         ((HAL_DDM_UP_RESISTOR*HAL_DDM_DOWN_RESISTOR)/(HAL_DDM_UP_RESISTOR+HAL_DDM_DOWN_RESISTOR))
#define HAL_DDM_DIVIDER_FLOAT_MUL           ((HAL_DDM_DIV_UP_RESISTANCE/HAL_DDM_DIV_DOWN_RESISTANCE + 1) * (1<<HAL_COIL_CURRENT_SHIFT_BITS))
#define HAL_DDM_DIVIDER_NUM_MUL             ((uint32)(HAL_DDM_DIVIDER_FLOAT_MUL+0.5))
#define HAL_RESONANCE_CAPACITOR_RESISTANCE_WITHOUT_FREQ  (uint32)((1<<HAL_COIL_CURRENT_SHIFT_BITS)*1e9/(2*PI*HAL_RESONANCE_CAPACITOR))

#define HAL_VRAIL_VOL_STEP  2000U   //2V

#define HAL_QSCI_TX_BUFFER_SIZE		220u	//define size of TX buffer
volatile uint8 gHAL_byQSCITxBuffReadIndex = 0;	// Index into TX buffer of next byte to read
volatile uint8 gHAL_byQSCITxBuffWriteIndex = 0;	// Index into TX buffer of next byte to write
volatile uint8 gHAL_byQSCITxBuffer[HAL_QSCI_TX_BUFFER_SIZE];	// Uart Transmit buffer

#define HAL_LOW_POWER_CLOCK_SWITCH_TIME		6U //ms, set clock to 32K from PLL: 2ms, restore 100MHz IPBus from 32K clock: 4ms
#if DIGITAL_DCDC
#define HAL_LOW_POWER_RESTORE_TIME			60U //ms, including IRQ EN/DIS, ISR, etc. It depends on the code execution time under low speed clock.
#else
#define HAL_LOW_POWER_RESTORE_TIME			26U //ms
#endif
#define HAL_LOW_POWER_TIME_LIMIT			100U //ms, don't enter low power mode when sleep time is less than limit

#define HAL_ADC_AVERAGE_NUM        4
uint16 gHAL_wADCMiscData[NUM_DEVICES][HAL_ADC_AVERAGE_NUM][ADC_MISC_CHANNEL_NUM];
uint16 gHAL_wCoilCurrent[NUM_DEVICES];
volatile uint8 gHAL_MiscAdcContinuesConversionNum[NUM_DEVICES];

uint16 gHAL_wDDMOffsetAdcCnt[NUM_DEVICES], gHAL_wResonanceOffsetAdcCnt[NUM_DEVICES];

#define HAL_TEMPERATURE_TOP_R				5110.0	//ohm
#define HAL_TEMPERATURE_RESISTANCE_ROOM		10000.0	//ohm
#define HAL_TEMPERATURE_ROOM				(273.15+25.0)	//25 degrees Celsius
#define HAL_TEMPERATURE_B					3610.0	//B25/85
#define HAL_TEMPERATURE_K					273.15

//the gHAL_DDMRecordBuf must be RECORD_BUF_LEN*2 aligned to facilitate DDM operation.
#pragma define_section DMA_DATA ".dma_data"
#pragma section DMA_DATA begin
static volatile sint16 gHAL_DDMRecordBuf[NUM_DEVICES][RECORD_BUF_LEN / 2];
#pragma section DMA_DATA end

uint8 gHAL_byAdcTPOffset = 8;   //offset value for ADC trigger position when byDiv = 2
uint8 gHAL_byDBGDDMValleyPos;   //for debug purpose

//key fob
boolean gHAL_bFobActive;
boolean gHAL_bDBGFobType;
uint16 gHAL_wDBGFobDuration = 100;

//MP81 OCP
uint16 MP81OCPThreshold = 1900;

void HAL_Init( void )
{
	uint16 wIntPri;
	HAL_DisableIRQ();
	
	WDG_Init();
	
	SIM_Init();
	
	PMC_Init();

	GPIO_Init();
	
	OCCS_Init();
	
	PWM_Init(HAL_PWM_DEAD_TIME);
	
	ADC_Init();
	
	DMA_Init();
	
	TRG_Init();
	
	CMP_Init();
	
	DAC_Init();
	
	TMR_Init();
	
	I2C_Init();
	
	FLASH_Init();
	
	QSCI0_Init(19200ul);
#if !DIGITAL_DCDC
	//the QSC1 pin are reused for D_DCDC
	QSCI1_Init(19200ul);
#endif
	
	wIntPri  = 0;
	wIntPri |= (INTC_IPR2_TMRA_0_0 | INTC_IPR2_TMRA_0_1);  //TMRA0 INT PRIORITY 2
	wIntPri |= (INTC_IPR2_TMRA_2_1);                       //TMRA2 INT PRIORITY 1
#if DIGITAL_DCDC
	wIntPri |= (INTC_IPR2_TMRA_1_0);                       //D_DCDC TMRA1 INT PRIORITY 0
#else
	wIntPri |= (INTC_IPR2_ADC_CC0_0);                      //ADC12 ADCA INT PRIORITY 0
#endif
	INTC_IPR2 = wIntPri;
	
	wIntPri  = 0;
	wIntPri |= INTC_IPR3_DMACH0_0;                         //DMA CH0 INT PRIORITY 0
	INTC_IPR3 = wIntPri;
	
	INTC_IPR10 |= INTC_IPR10_PIT1_ROLLOVR_0;				//PIT1 INT PRIORITY 0
	
#if FREEMASTER_SUPPORTED
    // Initialise FreeMASTER tool interrupt
    FMSTR_Cfg();
#endif
    
#if DEBUG_CONSOLE_SUPPORTED && (QSCI_CONSOLE_INDEX == 0)
	INTC_IPR5 &= ~ INTC_IPR5_QSCI0_TDRE;
	INTC_IPR5 |= INTC_IPR5_QSCI0_TDRE_1;
#endif
    
#if DEBUG_CONSOLE_SUPPORTED && (QSCI_CONSOLE_INDEX == 1)
	INTC_IPR5 &= ~ INTC_IPR5_QSCI1_TDRE;
	INTC_IPR5 |= INTC_IPR5_QSCI1_TDRE_1;
#endif

	HAL_RestoreIRQ(1);  //enable IRQ within peripheral level
	
	__EI0();  //enable IRQ within CPU core level
}

//disable IRQ and return the IRQ status before disable
uint8 HAL_DisableIRQ(void)
{
	uint8 ret = 1;
	if(INTC_CTRL & BIT5)
	{
		ret = 0;
	}
	else
	{
		INTC_CTRL |= BIT5;
	}
	return ret;
}

//restore IRQ setting
void HAL_RestoreIRQ(uint8 bySts)
{
	if(bySts)
	{
		INTC_CTRL &= ~BIT5;
	}
}

void HAL_DelayUs(uint16 wWaitUs)
{
    TMR_WaitUs(wWaitUs);
}

void HAL_KickWatchDog(void)
{
	WDG_Kick();
}

//For debug purpose
void HAL_SetTP(uint8 byId, HAL_TP_TYPE_E eType)
{
	if(0 == byId)
	{
		GPIO_SetOutput(GPIO_DDMTOGGLE, (GPIO_OUT_E)eType);
	}
	else if(1 == byId)
	{
		GPIO_SetOutput(GPIO_FSKTOGGLE, (GPIO_OUT_E)eType);
	}
}

//to check if the PWM for the inverter is running
boolean HAL_CheckInterverRunning(uint8 byDeviceId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	return PWM_CheckRunning(PWM_INVERTER_INDEX);
}

#if DIGITAL_DCDC
//For D_DCDC, the ADCA starts sample at every D_DCDC PID control loop (50KHz)
//in 1ms tick ISR, HAL only reads the ADC result
void HAL_MiscADCRead(void)
{
	static uint8 rollCnt[NUM_DEVICES];
	uint8 index;
	uint8 byDeviceId;
	
	for(byDeviceId=0; byDeviceId<NUM_DEVICES; byDeviceId++)
	{
		index = rollCnt[byDeviceId];
		ADC_GetResult(ADC_MISC_INDEX, &gHAL_wADCMiscData[byDeviceId][index][0], ADC_MISC_CHANNEL_DUMMY);
	    ADC_GetResult(ADC_DMA_INDEX, &gHAL_wADCMiscData[byDeviceId][index][ADC_MISC_CHANNEL_DUMMY], ADC_MISC_CHANNEL_NUM-ADC_MISC_CHANNEL_DUMMY);
		index++;
		if(index >= HAL_ADC_AVERAGE_NUM)
		{
			index = 0;
		}
		rollCnt[byDeviceId] = index;
	}
	
	if(TRG_CheckADCBTriggerSource() == 0)
	{
		//Start ADCB to sample temperature when DDM stop
		ADC_StartConversion(ADC_DMA_INDEX);
	}
}

//ADC result (gHAL_wADCMiscData) is filled every 1ms, for some case we wish to sample the latest signal
//Force sample let the ADC sample HAL_ADC_AVERAGE_NUM ADC data set immediately to fill gHAL_wADCMiscData
void HAL_MiscADCForceSample(uint8 byDeviceId)
{
	gHAL_MiscAdcContinuesConversionNum[byDeviceId] = HAL_ADC_AVERAGE_NUM;
	while(gHAL_MiscAdcContinuesConversionNum[byDeviceId] != 0) {}
}

//Called by D_DCDC timer ISR (50KHz)
void HAL_D_DCDC_ADCHandler(uint8 byDeviceId)
{
	if(gHAL_MiscAdcContinuesConversionNum[byDeviceId])
	{
	    //force sample
		gHAL_MiscAdcContinuesConversionNum[byDeviceId]--;
		HAL_MiscADCRead();
	}
	ADC_StartConversion(ADC_MISC_INDEX);
}

//D_DCDC Ipeak current offset voltage
uint16 HAL_ReadIpeakOffsetVol(uint8 byDeviceId)
{
	uint32 dwVoltage = 0;

	dwVoltage = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_IPEAK_INDEX);
	
	dwVoltage  *= ADC_REF_VOL;
	dwVoltage >>= ADC_BITS;
	
	return ((uint16) dwVoltage);
}

#else
void HAL_MiscADCISRAck(uint8 byDeviceId)
{
	static uint8 rollCnt[NUM_DEVICES];
	uint8 index = rollCnt[byDeviceId];
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	ADC_GetResult(ADC_MISC_INDEX, gHAL_wADCMiscData[byDeviceId][index], ADC_MISC_CHANNEL_DUMMY);
	ADC_GetResult(ADC_DMA_INDEX, &gHAL_wADCMiscData[byDeviceId][index][ADC_MISC_CHANNEL_DUMMY], ADC_MISC_CHANNEL_NUM-ADC_MISC_CHANNEL_DUMMY);
    index++;
    if(index >= HAL_ADC_AVERAGE_NUM)
    {
    	index = 0;
    }
    rollCnt[byDeviceId] = index;
    
    if(gHAL_MiscAdcContinuesConversionNum[byDeviceId])
	{
        //force sample
    	gHAL_MiscAdcContinuesConversionNum[byDeviceId]--;
		ADC_StartConversion(ADC_MISC_INDEX);
	}
    
    if(TRG_CheckADCBTriggerSource() == 0)
	{
		//Sample temperature when DDM stop
		ADC_StartConversion(ADC_DMA_INDEX);
	}
}

void HAL_MiscADCForceSample(uint8 byDeviceId)
{
	gHAL_MiscAdcContinuesConversionNum[byDeviceId] = HAL_ADC_AVERAGE_NUM;
	ADC_StartConversion(ADC_MISC_INDEX);
	while(gHAL_MiscAdcContinuesConversionNum[byDeviceId] != 0) {}
}

//Set DAC(or PWM simulated DAC) output voltage
void HAL_SetVrailCtrlVoltage(uint8 byDeviceId, uint16 wCtrlVol)
{
	uint32 dutyCycle;
	
	DBG_Assert(wCtrlVol > HAL_CHIP_SUPPLY, 0, wCtrlVol);
	
	dutyCycle = wCtrlVol;
	dutyCycle *= TMR_PWM_MAX_DUTY;
	dutyCycle /= HAL_CHIP_SUPPLY;
	
	TMR_SetCompareValue(TMR_DAC_INDEX, dutyCycle);
}
#endif

uint16 HAL_GetMiscADCData(uint8 byDeviceId, uint8 byChannel)
{
	uint8 i;
	uint32 tmp = 0;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	DBG_Assert(byChannel >= ADC_MISC_CHANNEL_NUM, 0, byChannel);
	
	for(i=0; i<HAL_ADC_AVERAGE_NUM; i++)
	{
		tmp += gHAL_wADCMiscData[byDeviceId][i][byChannel];
	}
	tmp /= HAL_ADC_AVERAGE_NUM;
	return tmp;
}

void HAL_PrintChar(uint8 byChar)
{
    boolean sts = HAL_DisableIRQ();		
    gHAL_byQSCITxBuffer[gHAL_byQSCITxBuffWriteIndex] = byChar;
    gHAL_byQSCITxBuffWriteIndex++;
    if(gHAL_byQSCITxBuffWriteIndex >= HAL_QSCI_TX_BUFFER_SIZE)
    {
        gHAL_byQSCITxBuffWriteIndex = 0;
    }
    HAL_RestoreIRQ(sts);
    QSCI_EnableTXEmptyInterrupt(QSCI_CONSOLE_INDEX, 1);
}

void HAL_PrintCharISRAck(void)
{
	if( gHAL_byQSCITxBuffReadIndex == gHAL_byQSCITxBuffWriteIndex )
	{
	    //Have sent out all data in buffer, disable tx empty interrupt
		QSCI_EnableTXEmptyInterrupt(QSCI_CONSOLE_INDEX, 0);
	}
	else
	{
		QSCI_PutChar(QSCI_CONSOLE_INDEX, gHAL_byQSCITxBuffer[ gHAL_byQSCITxBuffReadIndex ]);
		gHAL_byQSCITxBuffReadIndex++;
		if ( gHAL_byQSCITxBuffReadIndex >= HAL_QSCI_TX_BUFFER_SIZE )
		{
			gHAL_byQSCITxBuffReadIndex = 0u;
		}
	}
}

void HAL_ReadFlashData(uint8 * pbyBuf, NVM_BYTE_POINTER pbyAddr, uint16 wLen)
{
	FLASH_ReadData(pbyBuf, pbyAddr, wLen);
}

void HAL_WriteFlashData(uint32 * pdwBuf, uint32 dwAddr, uint16 wLen)
{
	uint16 i;
	
	// Divide parameter size by 4 to calculate # of DWORDS
	wLen >>= 2;
	
	for(i=0; i<wLen; i++)
	{
	    //Flash operation requires IRQ disabled
		uint8 sts = HAL_DisableIRQ();
		FLASH_WriteData(dwAddr, *pdwBuf);
		HAL_RestoreIRQ(sts);
		dwAddr += 4;
		pdwBuf++;
	}
}

void HAL_EraseFlashSector(uint32 dwAddr)
{
    //Flash operation requires IRQ disabled
	uint8 sts = HAL_DisableIRQ();
	FLASH_SectorDataErase(dwAddr);
	HAL_RestoreIRQ(sts);
}

void HAL_PrepareChargingResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq)
{
    //Make the low side MOSFET ON, up side MOSFET OFF for both half-bridge
    
    //PWM driver requires the period minus 32
    //that means the integer counter minus 1 and fractional keeps original
	uint32 period = CPU_PWM_CLK/dwFreq-32;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);

	PWM_SetFreq(PWM_INVERTER_INDEX, period);
	PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH);
	PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH);
	
	//PWMA reset, PWMB set.  Add 64 to make compliance for PWM fractional and normal mode
	PWM_SetChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH, period+64, 0);
	//PWM_SetChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH, period+64, 0);
	
	HAL_SetChargingBridge(byDeviceId, byCoilId, 0);
	PWM_Start(PWM_INVERTER_INDEX);
}

void HAL_ChargeResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty)
{
    uint32 period = CPU_PWM_CLK / dwFreq - 32;
    uint32 dutyPeriod = period * dwDuty / (LIB_MAX_DUTY_CYCLE*2);
    
    DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
    
    PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH);
    PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH);
    
    while(0 == PWM_CheckChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH)) {}  //wait start of a PWM cycle
    PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH);
    PWM_ClearChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH);

    //Set pulse width and deadtime for left half-bridge
    PWM_SetChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH, 0, dutyPeriod);
      
    while(0 == PWM_CheckChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH)) {}  //wait start of pulse
    
    //set PWMA low for next PWM cycle, set PWMB high for next PWM cycle
    //Note: the setting takes effect at next PWM cycle.
    PWM_SetChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH, period+64, 0);
 
    while(0 == PWM_CheckChannelFlags(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH)) {}   //wait end of pulse
}

void HAL_StopChargingResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	PWM_Stop(PWM_INVERTER_INDEX);
	HAL_SetChargingBridge(byDeviceId, byCoilId, 1);
}

void HAL_PreapareMeasureResonancePeriod(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq)
{
	uint32 period = CPU_TMR_CLK / dwFreq;
	uint32 dacVol;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	TRG_SetTriggerSource(TRG_DST_TA0IN_TRIG, TRG_SRC_CMPC_OUT);  //Set timer TA0 source as CMPC output
	TMR_Enable(TMR_DDM_FRQ_INDEX, 0);
	TMR_SetMode(TMR_DDM_FRQ_INDEX, TMR_MODE_CAPTURE);  //timer is set to capture mode
	TMR_SetCompareValue(TMR_DDM_FRQ_INDEX, period);    //set the overflow value
	TMR_ClearCaptureFlag(TMR_DDM_FRQ_INDEX);
	TMR_ClearCompareFlag(TMR_DDM_FRQ_INDEX);
	
	dacVol = gHAL_wResonanceOffsetAdcCnt[byDeviceId];
	dacVol *= ADC_REF_VOL;
	dacVol >>= ADC_BITS;
	DAC_SetVoltage(DAC_Q_MVL_INDEX, dacVol);  //Set DAC output value for CMP usage
	
	CMP_SetChannel(CMP_FRQ_INDEX, CMP_POLARITY_POSITIVE, CMP_GENERAL_CHANNEL_12BIT_DAC_INDEX);
	CMP_SetChannel(CMP_FRQ_INDEX, CMP_POLARITY_NEGATIVE, CMP_FRQ_CHANNEL_RESONANCE_INDEX);
	CMP_Enable(CMP_FRQ_INDEX, 1);  //CMP enable
}

void HAL_StartMesaureResonancePeriod(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	TMR_Enable(TMR_DDM_FRQ_INDEX, 1);  //timer is in capture mode, start it
}

uint16 HAL_CheckResonancePeriod(uint8 byDeviceId, uint8 byCoilId)
{
	uint16 ret = 0;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	if(TMR_CheckCapture(TMR_DDM_FRQ_INDEX))
	{
	    //Timer get signal captured
		ret = TMR_GetCounterValue(TMR_DDM_FRQ_INDEX);
		TMR_ClearCaptureFlag(TMR_DDM_FRQ_INDEX);
	}
	else if(TMR_CheckCompare(TMR_DDM_FRQ_INDEX))
	{
	    //if compared, means timer overflow, a error occurs
		ret = 0xFFFF;  //0xFFFF is a code for overflow
		TMR_ClearCompareFlag(TMR_DDM_FRQ_INDEX);
	}

	return ret;
}

void HAL_StopMeasureResonancePeriod(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	TRG_SetTriggerSource(TRG_DST_TA0IN_TRIG, TRG_SRC_NONE);
	CMP_Enable(CMP_FRQ_INDEX, 0);
	TMR_Enable(TMR_DDM_FRQ_INDEX, 0);
}

uint32 HAL_ConvertPeriodToFreq(uint8 byDeviceId, uint8 byCoilId, uint32 dwPeriod)
{
    //dwPeriod is the value returned by HAL_CheckResonancePeriod when timer get signal captured
	uint32 freq = 0;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	if(dwPeriod)
	{
		freq = CPU_TMR_CLK/dwPeriod;
	}
	return freq;
}

void HAL_PreQMeasure(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint8 byTriggerPosPercent)
{
    //Set peripheral ready to Q factor measurement
    
	uint16 i;
	uint32 period = CPU_PWM_CLK/dwFreq - 32;
	uint32 tmp;
	DMA_CFG_T dmaCfg;
	uint32 dacVol;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);

	//set CMP	
	dacVol = gHAL_wResonanceOffsetAdcCnt[byDeviceId];
	dacVol *= ADC_REF_VOL;
	dacVol >>= ADC_BITS;
	DAC_SetVoltage(DAC_Q_MVL_INDEX, dacVol);
	
	CMP_SetChannel(CMP_FRQ_INDEX, CMP_POLARITY_POSITIVE, CMP_GENERAL_CHANNEL_12BIT_DAC_INDEX);
	CMP_SetChannel(CMP_FRQ_INDEX, CMP_POLARITY_NEGATIVE, CMP_FRQ_CHANNEL_RESONANCE_INDEX);
	CMP_Enable(CMP_FRQ_INDEX, 1);
	
	//select ADC trigger source
	HAL_SetADCMode(byDeviceId, byCoilId, HAL_DDM_MODE_FR_Q);   //let ADC sample free resonance signal
	TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_PWMA1_TRG0);  //ADC trigger source change
	
	//PWM setting
	PWM_Stop(PWM_Q_MEASURE_INDEX);
	PWM_SetMode(PWM_Q_MEASURE_INDEX, PWM_MODE_COUNT);
	PWM_SetFreq(PWM_Q_MEASURE_INDEX, period*2-1);  //make the period*2 as the overflow period
	PWM_EnableTrigger(PWM_Q_MEASURE_INDEX, PWM_Q_MEASURE_CHANNEL_TRIGGER, 1);
	
	//Set the PWM trigger position
	tmp = byTriggerPosPercent;
	tmp *= period;
	tmp /= 100;
	PWM_SetChannelDuty(PWM_Q_MEASURE_INDEX, PWM_Q_MEASURE_CHANNEL_TRIGGER, tmp);
	PWM_ClearOverFlowFlag(PWM_Q_MEASURE_INDEX);

	//buffer clear
	for(i=0; i<(RECORD_BUF_LEN/2); i++)
	{
		gHAL_DDMRecordBuf[byDeviceId][i] = 0xFFFF;
	}
	
	//DMA configuraiton & enable	
	dmaCfg.dwSrcAddr = ADC_GetResultRegAddr(ADC_DMA_INDEX);
	dmaCfg.dwDstAddr = ((uint32)(gHAL_DDMRecordBuf)) * 2;
	dmaCfg.dwSrcMod  = 0x0;
	dmaCfg.dwDstMod  = RECORD_BUF_LEN;
	dmaCfg.bySrcAddrFixed  = 1;
	dmaCfg.byDstAddrFixed  = 0;
	dmaCfg.dwTransferLen   = DMA_MAX_TRANFER_LENGTH;
	dmaCfg.bySizePerRequest = 2;	
	DMA_SetConfig(DMA_ADC_INDEX, &dmaCfg);
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
	DMA_EnableRequest(DMA_ADC_INDEX, 1);
}

boolean HAL_StartQMeasure(uint8 byDeviceId, uint8 byCoilId)
{
	boolean ret = TRUE;
	PWM_ClearOverFlowFlag(PWM_Q_MEASURE_INDEX);
	CMP_ClearRaisingFlag(CMP_FRQ_INDEX);
	
	/*
	 * After pulse issued, the resonance signal will goes up then down, after it
	 * cross zero point, the CMP raising flag is set.
	 */
	
	while(0 == CMP_CheckRaisingFlag(CMP_FRQ_INDEX))  //wait signal cross zero point
	{
	    //in case of error, SW may stuck, add overflow mechanism
		if(PWM_CheckOverFlowFlag(PWM_Q_MEASURE_INDEX))
		{
			ret = FALSE;   //error happens
			break;
		}
	}
	//Start PWM. PWM is initiated by CMP output, and PWM issue trigger signal to ADCB to sample resonance signal.
	if(ret)
	{
	    PWM_Start(PWM_Q_MEASURE_INDEX);
    }
	return ret;
}

uint16 HAL_CheckQMeasureFinished(uint8 byDeviceId, uint8 byCoilId, uint8 byNum)
{
	uint16 ret = 0;
	if(byNum > 1)
	{
	    //convert it to array index
		byNum--;
	}
	else
	{
		DBG_Assert(1, 0, byNum);
	}
	
	if(gHAL_DDMRecordBuf[byDeviceId][byNum] != 0xFFFF)
	{
	    //ADC sample finished
		ret = 1;
	}
	else
	{
	    //in case of error, add overflow mechanism to protect SW from stuck
		if(PWM_CheckOverFlowFlag(PWM_Q_MEASURE_INDEX))
		{
			PWM_ClearOverFlowFlag(PWM_Q_MEASURE_INDEX);
			ret = 0xFFFF;  //0xFFFF is a code of overflow, imply error happen
		}
	}
	return ret;
}

void HAL_StopQMeasure(uint8 byDeviceId, uint8 byCoilId)
{
	CMP_Enable(CMP_FRQ_INDEX, 0);
	TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_NONE);
	PWM_EnableTrigger(PWM_Q_MEASURE_INDEX, PWM_Q_MEASURE_CHANNEL_TRIGGER, 0);
	PWM_Stop(PWM_Q_MEASURE_INDEX);
	DMA_EnableRequest(DMA_ADC_INDEX, 0);
}

uint16* HAL_GetQMeasureData(uint8 byDeviceId, uint8 byCoilId, uint8 byDataNum)
{
	uint8 i;
	for(i=0; i<byDataNum; i++)
	{
		uint16 tmp = gHAL_DDMRecordBuf[byDeviceId][i] >> 3;  //ADC result is left shift 3 bits, right shift to restore
		if(tmp < gHAL_wResonanceOffsetAdcCnt[byDeviceId])
		{
		    //convert to real amplitude value
			tmp = gHAL_wResonanceOffsetAdcCnt[byDeviceId] - tmp;
		}
		else
		{
			tmp = 0;
		}
		gHAL_DDMRecordBuf[byDeviceId][i] = tmp;
	}
	return (uint16*)gHAL_DDMRecordBuf[byDeviceId];
}

uint16 HAL_GetQFacotrSenseCircuitADCCntOffset(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	//gHAL_wResonanceOffsetAdcCnt is sampled at beginning of Q factor measurement
	return gHAL_wResonanceOffsetAdcCnt[byDeviceId];
}

void HAL_ResonanceCircuitSenseOffsetInit(uint8 byDeviceId, uint8 byCoilId, HAL_DDM_MODE_E eMode)
{
	uint8 i;
	uint16 offset;
	uint16* pOffset;
	uint32 sum;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	//set ADC sample channel
	HAL_SetADCMode(byDeviceId, byCoilId, eMode);
	
	//select which offset value should be initialized
	switch(eMode)
	{
	case HAL_DDM_MODE_CHARGING:
		pOffset = &gHAL_wDDMOffsetAdcCnt[byDeviceId];
		break;
	case HAL_DDM_MODE_FR_Q:
		pOffset = &gHAL_wResonanceOffsetAdcCnt[byDeviceId];
		break;
	default:
		DBG_Assert(1, 0, eMode);
	}

	sum = 0;
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
	
	//Abandon first sample, first sample may not correct due to ADC channel switch
	ADC_StartConversion(ADC_DMA_INDEX);
	while( ADC_CheckConvertionFinished(ADC_DMA_INDEX) == 0 ) {}
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX); 
	
	//sample 4 times and average the result
	for(i=0; i<4; i++)
	{
		ADC_StartConversion(ADC_DMA_INDEX);		
		while( ADC_CheckConvertionFinished(ADC_DMA_INDEX) == 0 ) {}
		ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
		ADC_GetResult(ADC_DMA_INDEX, &offset, 1);
		sum += offset;
	}
	*pOffset = sum/4;
}

void HAL_SetADCMode(uint8 byDeviceId, uint8 byCoilId, HAL_DDM_MODE_E eMode)
{
    uint8 i;
    ADC_DMA_CHANNEL_E chIndex;
    
    DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
    
    switch(eMode)
    {
    case HAL_DDM_MODE_CHARGING:
        chIndex = ADC_DMA_CHANNEL_DDM_INDEX;
        break;
    case HAL_DDM_MODE_FR_Q:
        chIndex = ADC_DMA_CHANNEL_QVOLT_INDEX;
        break;
    default:
        DBG_Assert(1, 0, eMode);
    }
    
    //Set ADC channel according requirement
    ADC_SetChannelIndex(ADC_DMA_INDEX, chIndex);
}

uint16 HAL_SetTriggerPosition(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint8 byTrigPosPercent)
{
	uint32 trigPeriod = CPU_PWM_CLK / dwFreq - 32;
	trigPeriod *= byTrigPosPercent;
	trigPeriod /= 100;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	PWM_SetChannelDuty(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, trigPeriod);
}

void HAL_StartCharging(uint8 byDeviceId, uint8 byCoilId)
{
	uint8 sts;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	HAL_SetADCMode(byDeviceId, byCoilId, HAL_DDM_MODE_CHARGING);
	
	PWM_SetMode(PWM_INVERTER_INDEX, PWM_MODE_NORMAL);
	PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 1);
	
	
	//Set MP81 over current protection
	//Configure CMPC for MP81 input current protection
	CMP_SetInternalDAC(CMP_MP81_INDEX, MP81OCPThreshold);
	TRG_SetTriggerSource(TRG_DST_PWM_FAULT0, TRG_SRC_CMPD_OUT);  //Set PWM Fault0 source as CMPC output
	CMP_Enable(CMP_MP81_INDEX, 1);  //CMP enable
	//Configure PWM disable by fault input
	PWM_DisablebyFaultInput(1);
	PWM_ClearFaultFlag();
	
	//time critical, and two PWM module must start at same time, PWM driver also compromise for it.
	PWM_Start(PWM_INVERTER_INDEX);
}

void HAL_StopCharging(uint8 byDeviceId, uint8 byCoilId)
{
	uint8 sts;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 0);

	PWM_Stop(PWM_INVERTER_INDEX);
	
	//Stop MP81 over current protection
	PWM_DisablebyFaultInput(0);
	PWM_ClearFaultFlag();
	CMP_Enable(CMP_MP81_INDEX, 0);
	TRG_SetTriggerSource(TRG_DST_PWM_FAULT0, TRG_SRC_NONE); 
	
	//init the coil current value, it is only sampled when charging status..
	gHAL_wCoilCurrent[byDeviceId] = 0;
}
static sint16 HAL_CovertAdcCntToCelsisusDegree(uint16 adcValue)
{
	uint32 dwTemp;
	float32 dbtemp;
	
	//Convert temperature ADC counts to resistor value
	// Formula: Rt/(Rt+Rtop) = adccnt/(1<<ADC_BITS)
	//       => Rt = Rtop*adccnt/((1<<ADC_BITS)-adccnt)
	dwTemp = adcValue;
	dwTemp *= HAL_TEMPERATURE_TOP_R;
	dwTemp /= ((1UL<<ADC_BITS) - 1UL - adcValue);
	
	//Calculate temperature
	//Rt = R*exp(B*(1/T1-1/T2))
	//where T1 = HAL_TEMPERATURE_RESISTANCE_ROOM, Rt is the resistor value in HAL_TEMPERATURE_RESISTANCE_ROOM
	//      T2 = measured temperature, R is the resistor value in measured temperature.
	//      B  = HAL_TEMPERATURE_B
	//  =>  T2 = 1/(1/T1 - log(Rt/R)/B) = 1/(1/T1 + log(R/Rt)/B) 
	dbtemp = dwTemp / HAL_TEMPERATURE_RESISTANCE_ROOM;
	dbtemp = log(dbtemp);
	dbtemp /= HAL_TEMPERATURE_B;
	dbtemp += (1.0 / HAL_TEMPERATURE_ROOM);
	dbtemp = 1.0 / dbtemp;
	dbtemp -= HAL_TEMPERATURE_K;  //convert to celsius degree
	
	return (sint16)dbtemp;
}

sint16 HAL_GetCoilTemperature(uint8 byDeviceId, uint8 byCoilId)
{
	//current MPA11 HW only implement a NTC on board, no NTC on coils
	//so use the board NTC for all coils
	uint32 tmp;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	tmp = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_TEMP1_INDEX);
	return HAL_CovertAdcCntToCelsisusDegree(tmp);
}

//return maximum Celsius degree
sint16 HAL_GetTopTemperature(uint8 byDeviceId)
{
	uint16 temp[2], top_temp;
	uint8 i;
	uint32 dwTemp;
	float32 dbtemp;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	//temperature sensor is NTC	
	temp[0] = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_TEMP1_INDEX);
	temp[1] = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_TEMP2_INDEX);

	top_temp = temp[0];
	for(i = 0; i< 2; i++)
	{
		if(temp[i] < top_temp)
		{
			top_temp = temp[i];
		}
	}
	
	return HAL_CovertAdcCntToCelsisusDegree(top_temp);
}

uint16 HAL_GetRailVoltage(uint8 byDeviceId)
{
	uint32 dwVoltage = 0;
	
	ANALOG_CAL_PARAMS* pAnalogCalParams = NVM_GetAnalogCalibrationParam(byDeviceId);
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	dwVoltage = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_VRAILA_INDEX);
	
	//Calibration formula  CalibratedValue = sampledValue * calibrationCoeff >> NOM_CAL_SCALE_FACTOR_SIZE_IN_BITS
	dwVoltage *= pAnalogCalParams->wInputVoltageCalibration;
	dwVoltage >>= NOM_CAL_SCALE_FACTOR_SIZE_IN_BITS;
	
	dwVoltage  *= ADC_REF_VOL;
	dwVoltage >>= ADC_BITS;  //ADC result convert to mV
	dwVoltage  *= HAL_RAIL_SENSE_MULTI_U;
	dwVoltage >>= HAL_RAIL_SENSE_SHIFT_BITS;  //convert to real voltage
	return ((uint16) dwVoltage);
}

//Here batteryVoltage means the board input voltage
uint16 HAL_GetBatteryVoltage(void)
{
	uint32 dwVoltage = 0;

	dwVoltage = HAL_GetMiscADCData(0, ADC_MISC_CHANNEL_VBAT_INDEX);
	
	dwVoltage  *= ADC_REF_VOL;
	dwVoltage >>= ADC_BITS;      //ADC result convert to mV
	dwVoltage  *= HAL_BATTERY_SENSE_MULTI_U;
	dwVoltage >>= HAL_BATTERY_SENE_SHIFT_BITS;   //convert to real voltage
	
	return ((uint16) dwVoltage);
}

uint16 HAL_GetCoilCurrent(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	//Coil current is not a simple DC value which could be sampled by ADC easily.
	//Coil current is mathematical sampled(search) on a AC signal(DDM signal) 
	return gHAL_wCoilCurrent[byDeviceId];
}

uint16 HAL_GetInputCurrent(uint8 byDeviceId)
{
	uint32 dwRtn = 0;
	sint32 sdwOffset;
	ANALOG_CAL_PARAMS* pAnalogCalParams;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	dwRtn = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_INPUTCURRENT_INDEX);
		
	pAnalogCalParams = NVM_GetAnalogCalibrationParam(byDeviceId);
		
	// NOTE: The input current sensor output value (say inputCurrent=0) is
	// dependent upon the operating rail voltage value. Needs compensation such offset.
	sdwOffset = HAL_GetMiscADCData(byDeviceId, ADC_MISC_CHANNEL_VRAIL_INDEX);
	sdwOffset /= 10;
	sdwOffset *= pAnalogCalParams->sdwInputCurrentSlope;
	sdwOffset += pAnalogCalParams->sdwInputCurrentOffset;
	
	if(sdwOffset > 0)
	{
		sdwOffset >>= pAnalogCalParams->wInputCurrentNorm;
		// Remove calibration offset
		if (dwRtn > sdwOffset)
		{
			dwRtn -= sdwOffset;
		}
		else
		{
			dwRtn = 0;
		}
	}

	// Calibrate the ADC reading using Run-Time Params value
	dwRtn *= pAnalogCalParams->wInputCurrentCalibration;
	dwRtn >>= NOM_CAL_SCALE_FACTOR_SIZE_IN_BITS;
	
	dwRtn  *= ADC_REF_VOL;
	dwRtn >>= ADC_BITS;
	
	dwRtn *= HAL_INPUT_CURRENT_MULTI_COEFF_U;
	dwRtn >>= HAL_INPUT_CURRENT_SENSE_SHIFT_BITS;
	
	return ((uint16) dwRtn);
}

uint16 HAL_GetResonanceCapValue(uint8 byDeviceId, uint8 byCoilId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	return HAL_RESONANCE_CAPACITOR;
}

void HAL_EnableDDM(uint8 byDeviceId, uint8 byCoilId, uint8 byIsEn)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	if(byIsEn)
	{
		DMA_CFG_T dmaCfg;
		
		//if ADCB is not triggered by PWM, the ADCB will be triggered by SW every 1ms.
		//Thus we have chance to sample the temperature channel.
		if(TRG_CheckADCBTriggerSource() == 0)
		{
			TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_PWMA0_TRG0);
			//Wait current ADC sample finished
			while(ADC_CheckConvertionOngoing(ADC_DMA_INDEX)) {}
		}
		ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
		
		dmaCfg.dwSrcAddr = ADC_GetResultRegAddr(ADC_DMA_INDEX);
		dmaCfg.dwDstAddr = ((uint32)(gHAL_DDMRecordBuf)) * 2;
		dmaCfg.dwSrcMod  = 0x0;
		dmaCfg.dwDstMod  = RECORD_BUF_LEN;
		dmaCfg.bySrcAddrFixed  = 1;
		dmaCfg.byDstAddrFixed  = 0;
		dmaCfg.dwTransferLen   = DMA_MAX_TRANFER_LENGTH;
		dmaCfg.bySizePerRequest = 2;
		DMA_SetConfig(DMA_ADC_INDEX, &dmaCfg);
		
		//TMR_DDM_FRQ_INDEX is a multiplex timer, set it for DDM mode
		TMR_SetMode(TMR_DDM_FRQ_INDEX, TMR_MODE_COUNTER);		
		TMR_SetCompareValue(TMR_DDM_FRQ_INDEX, RECORD_BUF_LEN / 2U / 2U - 1U);
		TMR_Enable(TMR_DDM_FRQ_INDEX, 1);
		TRG_SetTriggerSource(TRG_DST_TA0IN_TRIG, TRG_SRC_PWMA0_TRG0);
		
		DMA_EnableRequest(DMA_ADC_INDEX, 1);
		PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 1);
	}
	else
	{
	    //Reset all peripherals DDM employs
		TRG_SetTriggerSource(TRG_DST_TA0IN_TRIG, TRG_SRC_NONE);
		TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_NONE);
		PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 0);
		TMR_Enable(TMR_DDM_FRQ_INDEX, 0);
		DMA_EnableRequest(DMA_ADC_INDEX, 0);
	}
}

uint16 HAL_AnalogPing(uint8 byDeviceId, uint8 byCoilId)
{
	uint8 sts;
	uint16 result = 0;
	uint8 i;
	uint32 period = CPU_PWM_CLK / HAL_ANALOG_PING_FREQ;

	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	gHAL_DDMRecordBuf[byDeviceId][HAL_ANALOG_PING_PULSE_NUM-1] = 0xFFFF;
	gHAL_DDMRecordBuf[byDeviceId][HAL_ANALOG_PING_ADC_SAMPLE_NUM-1] = 0xFFFF;

	HAL_EnableDDM(byDeviceId, byCoilId, 0);

	HAL_EnableCoils(byDeviceId, byCoilId, 1);
	
	//Set the PWM parameters for analog ping
	HAL_SetChargingFreqDutyPhase(byDeviceId, byCoilId, HAL_ANALOG_PING_FREQ, HAL_ANALOG_PING_DUTY, HAL_ANALOG_PING_PHASE);
	HAL_SetTriggerPosition(byDeviceId, byCoilId, HAL_ANALOG_PING_FREQ, HAL_ANALOG_PING_ADC_TRIG_POS);
	HAL_EnableDDM(byDeviceId, byCoilId, 1);
	
	sts = HAL_DisableIRQ();
	//time critical, when coil discharge is disabled, the coil selection circuit charge the resonance capacitor
	HAL_EnableCoilDischarge(byDeviceId, byCoilId, 0);	
	HAL_StartCharging(byDeviceId, byCoilId);
	
	//wait PWM pulse output finished
	while(0xFFFF == gHAL_DDMRecordBuf[byDeviceId][HAL_ANALOG_PING_PULSE_NUM-1]) {}
	
	PWM_UpdateChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH, period+64, 0);
	PWM_UpdateChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH,  period+64, 0);
	PWM_ValidUpdate(PWM_INVERTER_INDEX);
	HAL_RestoreIRQ(sts);
	
	//wait ADC sample finished
	while(0xFFFF == gHAL_DDMRecordBuf[byDeviceId][HAL_ANALOG_PING_ADC_SAMPLE_NUM-1]) {}

	HAL_StopCharging(byDeviceId, byCoilId);
	HAL_EnableCoilDischarge(byDeviceId, byCoilId, 1);
	HAL_EnableDDM(byDeviceId, byCoilId, 0);
	HAL_EnableCoils(byDeviceId, byCoilId, 0);

	//accumulate all ADC samples
	for(i=HAL_ANALOG_PING_PULSE_NUM; i<HAL_ANALOG_PING_ADC_SAMPLE_NUM; i++)
	{
		uint16 tmp = gHAL_DDMRecordBuf[byDeviceId][i] >> 3; //ADC result is left shifted 3 bits, restore it.
		if(tmp < gHAL_wDDMOffsetAdcCnt[byDeviceId])
		{
			result += (gHAL_wDDMOffsetAdcCnt[byDeviceId] - tmp);
		}
		else
		{
			result += (tmp - gHAL_wDDMOffsetAdcCnt[byDeviceId]);
		}
	}
	
	//due to library limitation, result=0 is not allowed
	//limit the minimum value to 10
	if(result < 10)
	{
		result = 10;
	}
	
	return result;
}

uint16 HAL_FindAdcTriggerPos(uint8 byDeviceId, uint8 byCoilId, uint8 byDiv, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase)
{
    //due to pre-driver and DDM signal sense circuit propagation delay, the real 
    //DDM signal at ADC port is not synced with PWM cycle, its delay depends on circuit design
    //For this case, set the delay to 5% of PWM cycle
	uint8 searchStartPoint = 5;
	uint8 i, valley= 25;
	uint16  adcRawCnt, coilCurrAdcCnt, min = 0xffff;
	uint32 tmp, curr_result, pre_result, vRail;
	
	if(byDiv == 1)
	{
		//HAL_SetTP(2, HAL_TP_TYPE_CLR);
	}
	else if(byDiv == 2)
	{
		//HAL_SetTP(2, HAL_TP_TYPE_SET);
	}
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	DBG_Assert(byDiv>3 , 0, byDiv);

	//Set trigger position and enable the trigger output
	HAL_SetTriggerPosition(byDeviceId, byCoilId, dwFreq, searchStartPoint);
	TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_PWMA0_TRG0);	
	PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 1);
	
	
    /*
     * search the valley of DDM signal. Note: DDM signal is AC signal.                 *
     * The valley should be located at 0~25% of PWM cycle. due to propagation delay,   *
     * set the start position from searchStartPoint, and enlarge the spectrum to 30%,  *
	 * for robustness.                                                                 *
	 */
	
	//init the first sample count
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
	while(0 == ADC_CheckConvertionFinished(ADC_DMA_INDEX)) {}
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
	ADC_GetResult(ADC_DMA_INDEX, &adcRawCnt, 1);
	pre_result = adcRawCnt;
	
	while(0 == ADC_CheckConvertionFinished(ADC_DMA_INDEX)) {}
	ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
	ADC_GetResult(ADC_DMA_INDEX, &adcRawCnt, 1);
	pre_result += adcRawCnt;
	pre_result >>= 1;  //average the first sampled count
	
	for(i=(searchStartPoint+1); i<(searchStartPoint+30); i++)
	{
		uint16 tmp;
		
		HAL_SetTriggerPosition(byDeviceId, byCoilId, dwFreq, i);

		ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
		while(0 == ADC_CheckConvertionFinished(ADC_DMA_INDEX)) {}
		ADC_ClearConvertionFinishedFlag(ADC_DMA_INDEX);
		ADC_GetResult(ADC_DMA_INDEX, &adcRawCnt, 1);
		curr_result = adcRawCnt;
		while(0 == ADC_CheckConvertionFinished(ADC_DMA_INDEX)) {}
		ADC_GetResult(ADC_DMA_INDEX, &adcRawCnt, 1);
		curr_result += adcRawCnt;
		curr_result >>= 1;

		tmp = (curr_result + pre_result)>>1;
		if(min > tmp)
		{
			min = tmp;
			valley = i;  //record the valley position
		}
		pre_result = curr_result;
	}
	
	PWM_EnableTrigger(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_TRIGGER, 0);
	TRG_SetTriggerSource(TRG_DST_ADCB_TRIG, TRG_SRC_NONE);
	
	gHAL_byDBGDDMValleyPos = valley;
	
	/*
	 * byDiv = 0;  only for coil current sample
	 * byDiv = 1;  for coil curren sample, and set DDM trigger position to valley point
	 * byDiv = 2;  for coil curren sample, and set DDM trigger position to right half-valley point
	 * byDiv = 3;  for coil curren sample, and set DDM trigger position to left half-valley point
	 */
	
	if(0 == byDiv)
	{
		byDiv = 1;
	}
	
	if(byDiv == 1)
	{
		HAL_SetTriggerPosition(byDeviceId, byCoilId, dwFreq, valley);
	}
	else if(byDiv == 2)
	{
	    //Note: gHAL_byAdcTPOffset is a value related with DDM sense circuit DC offset
	    //      if DC offset get higher, set a higher value(by ratio) to gHAL_byAdcTPOffset.
		HAL_SetTriggerPosition(byDeviceId, byCoilId, dwFreq, (50+gHAL_byAdcTPOffset+valley)/2);
	}
	else if(byDiv == 3)
	{
	    HAL_SetTriggerPosition(byDeviceId, byCoilId, dwFreq, valley/2);
	}
	
	if(gHAL_wDDMOffsetAdcCnt[byDeviceId] > min)
	{
	    //get the amplitude to the DDM signal
		coilCurrAdcCnt = gHAL_wDDMOffsetAdcCnt[byDeviceId] - min;
	}
	else
	{
		coilCurrAdcCnt = 0;
	}
	
	//calculate the coil current
	//Formula for full bridge, no phase shift, 50% duty cycle:
	//  Icoil(RMS) = (DDM peak value/1.414 * senseCircuitDecayRate - Vrail/2)/resonanceCapacitorResistance
    //Formula for half bridge, 50% duty cycle:
    //  Icoil(RMS) = (DDM peak value/1.414 * senseCircuitDecayRate)/resonanceCapacitorResistance
	vRail = HAL_GetRailVoltage(byDeviceId);
	vRail *= ((1UL<<HAL_COIL_CURRENT_SHIFT_BITS)/2);
	tmp   = coilCurrAdcCnt;
	tmp  *= ADC_REF_VOL;
	tmp >>= ADC_BITS;  //convert ADC count to mV
	
	tmp *= HAL_DDM_DIVIDER_NUM_MUL;
	if(tmp > vRail)
	{
		tmp -= vRail;  //convert to Peak voltage across resonance capacitor
	}
	else
	{
		tmp = 0;
	}
	tmp /= (HAL_RESONANCE_CAPACITOR_RESISTANCE_WITHOUT_FREQ/dwFreq);  //divide resonance capacitor resistance
	
	//  tmp / 1.414 = tmp * 724/1024
	tmp *= 724;
	tmp >>= 10;
	gHAL_wCoilCurrent[byDeviceId] = tmp;
	
	//get actually valley position upon PWM cycle
	valley -= searchStartPoint;
	if(valley > 25)
	{
		valley = 25;
	}
	
	//valley position indicate the power factor of system
	//simply make power factor = 100-4*valley, however, it is not an accurate power factor.
	return 100 - 4*valley;
}

void HAL_SetChargingBridge(uint8 byDeviceId, uint8 byCoilId, uint8 byBridge)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);

	if(byBridge)
	{
	    //set right half bridge pin to PWM output
		GPIO_SetMode(GPIO_PWM_RIGHT_HIGH, GPIO_MODE_PER, 0);
	}
	else
	{
	    //Half bridge is achieved by set the right half bridge pin to GPIO,
	    //set GPIO pin for high side MOSFET to low
	    //set GPIO pin for low side MOSFET to high
		PWM_StopBridge(PWM_INVERTER_CHANNEL_RIGHT_HIGH);
		GPIO_SetOutput(GPIO_PWM_RIGHT_HIGH, GPIO_CLR);
		GPIO_SetMode(GPIO_PWM_RIGHT_HIGH, GPIO_MODE_GP, 1);

	}
}

void HAL_EnableCoilDischarge(uint8 byDeviceId, uint8 byCoilId, uint8 byIsEn)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	//coil discharge operation must be when PWM is OFF
	DBG_Assert(0 != PWM_CheckRunning(PWM_INVERTER_INDEX), 0, 0);
	
	if(byIsEn)
	{
	    //coil discharge is achieved by set both pins for full bridge
	    //low side MOFSET as GPIO high,
		GPIO_SetMode(GPIO_PWM_LEFT_HIGH, GPIO_MODE_GP, 1);
		GPIO_SetMode(GPIO_PWM_RIGHT_HIGH, GPIO_MODE_GP, 1);
		GPIO_SetOutput(GPIO_PWM_LEFT_HIGH, GPIO_CLR);
		GPIO_SetOutput(GPIO_PWM_RIGHT_HIGH, GPIO_CLR);
	}
	else
	{
	    //set both pins for full bridge low side MOSFET to PWM output
		GPIO_SetMode(GPIO_PWM_LEFT_HIGH, GPIO_MODE_PER, 0);
		GPIO_SetMode(GPIO_PWM_RIGHT_HIGH, GPIO_MODE_PER, 0);
	}
}



void HAL_EnableChargingOnCoil(uint8 byDeviceId, uint8 byCoilId, boolean bIsEn)
{
	uint16 wRailVol;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	if(bIsEn)
	{
		//check rail voltage before start powering signal
		wRailVol = HAL_GetRailVoltage(byDeviceId);
		
		if(HAL_VRAIL_MAX_VOL_FOR_DP <= wRailVol)
		{
			//probably due to ambitious parameter or HW issue
			//Should not start charging otherwise may damage Rx
			DBG_Assert(1, 0, wRailVol);
		}
		else
		{
			HAL_EnableCoils(byDeviceId, byCoilId, 1);
			HAL_StartCharging(byDeviceId, byCoilId);
		}
	}
	else
	{
		HAL_StopCharging(byDeviceId, byCoilId);
		HAL_EnableCoils(byDeviceId, byCoilId, 0);
	}
}

void HAL_SetChargingFreqDutyPhase(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase)
{
	uint8 sts;
	uint32 period = CPU_PWM_CLK / dwFreq - 32;
	uint32 dutyPeriod, phasePeriod;
	uint32 pwmLHH, pwmLHL;
	uint32 pwmRHH, pwmRHL;
	
	uint16  pwmDeadTimePeriod = (CPU_PWM_CLK/1000000UL)*HAL_PWM_DEAD_TIME/1000UL;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	DBG_Assert(dwDuty > LIB_MAX_DUTY_CYCLE, 0, dwDuty);
	DBG_Assert(dwPhase > LIB_MAX_PHASE_SHIFT, 0, dwPhase);
	//Not support duty and phase control simultaneously 
	DBG_Assert( ((dwDuty != LIB_MAX_DUTY_CYCLE) && (dwPhase != LIB_MAX_PHASE_SHIFT)), 0, 0);
	DBG_Assert( (period/2) <= pwmDeadTimePeriod, 0, period);
	
	if(dwDuty != LIB_MAX_DUTY_CYCLE)
	{
		//Can't support duty control, convert it to phase control, since it use dual input MOSFET
		//pre-driver, otherwise there may be risk that right half bridge may got short after changing
		//the PWM parameters. Another reason is for DDM implementation convenience.
		dwPhase = dwDuty * LIB_MAX_PHASE_SHIFT / LIB_MAX_DUTY_CYCLE;
		dwPhase = LIB_MAX_PHASE_SHIFT -dwPhase;
		dwDuty = LIB_MAX_DUTY_CYCLE;
	}
	
	dutyPeriod  = period * dwDuty / (LIB_MAX_DUTY_CYCLE * 2);
	phasePeriod = period / 2 * dwPhase / LIB_MAX_PHASE_SHIFT;
		
	//Not support small duty or phase, otherwise there may be risk that half bridge got shorted.
	DBG_Assert(dutyPeriod < (pwmDeadTimePeriod+32), 0, 0);
	DBG_Assert(phasePeriod < (pwmDeadTimePeriod+32), 0, 0);
	
	//The PWM modules works in complementary mode, needs manually set the dead time	
	//Get the left PWM channel output params
	
	if( (period/2-dutyPeriod) < pwmDeadTimePeriod )
	{
		pwmLHH = 0;
		pwmLHL = period/2 - pwmDeadTimePeriod;
		pwmRHH = period/2;
		pwmRHL = period - pwmDeadTimePeriod;
	}
	else
	{
		pwmLHH = 0;
		pwmLHL = dutyPeriod;
		pwmRHH = period/2;
		pwmRHL = period/2 + dutyPeriod;
	}
	
	if(LIB_MAX_PHASE_SHIFT != dwPhase)
	{
		//Add the shift period, the duty cycle must be 50%

		pwmRHH += phasePeriod;
		if(pwmRHL > (period - phasePeriod))
		{
			pwmRHL = pwmRHL + phasePeriod - period;
		}
		else
		{
			pwmRHL += phasePeriod;
		}
	}
	
	sts = HAL_DisableIRQ();
	if(PWM_CheckRunning(PWM_INVERTER_INDEX))
	{
	    //wait a beginning of next PWM cycle
		PWM_ClearOverFlowFlag(PWM_INVERTER_INDEX);
		while(0 == PWM_CheckOverFlowFlag(PWM_INVERTER_INDEX)) {}
	}
	
	PWM_InvalidUpdate(PWM_INVERTER_INDEX);
	
	PWM_UpdateFreq(PWM_INVERTER_INDEX, period);

	PWM_UpdateChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_LEFT_HIGH, pwmLHH, pwmLHL);
	PWM_UpdateChannelPairs(PWM_INVERTER_INDEX, PWM_INVERTER_CHANNEL_RIGHT_HIGH,  pwmRHH, pwmRHL);

	//valid all PWM parameter in atomic operation for synchronization.
	PWM_ValidUpdate(PWM_INVERTER_INDEX);
	HAL_RestoreIRQ(sts);
}

//Select coil
//Here may add SW logic to prevent two or more coil are selected simultaneously. Not implemented yet.
void HAL_EnableCoils(uint8 byDeviceId, uint8 byCoilId, boolean bIsEn)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
}


void HAL_EnableWCT(uint8 byDeviceId, boolean bIsEn)
{
	uint8 byCoilId;
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	//reset all control signal at beginning
	GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_CLR);

	for(byCoilId=0; byCoilId<NUM_COILS_PER_DEVICE; byCoilId++)
	{
		if(FALSE == PWM_CheckRunning(PWM_INVERTER_INDEX))
		{
			HAL_EnableCoilDischarge(byDeviceId, byCoilId, 1);
		}
	}
	
	//Handler VBAT_SW, DCDC_EN...etc.
	if(bIsEn)
	{
		//voltage should be set before HAL_EnableWCT call
#if DIGITAL_DCDC
		HAL_D_DCDC_Init();
		HAL_D_DCDC_Start(byDeviceId);
#else
		TMR_Enable(TMR_DAC_INDEX, 1);
		ST_WaitMs(10);  //let PWM simulated DAC output get stable
		GPIO_SetOutput(GPIO_DCDC_EN, GPIO_SET);
#endif
		GPIO_SetOutput(GPIO_DRV_EN_L, GPIO_SET);
		GPIO_SetOutput(GPIO_DRV_EN_H, GPIO_SET);
		
		ST_WaitMs(10);  //wait Vrail get stable
		
		for(byCoilId=0; byCoilId<NUM_COILS_PER_DEVICE; byCoilId++)
		{
            HAL_ResonanceCircuitSenseOffsetInit(byDeviceId, byCoilId, HAL_DDM_MODE_CHARGING);
            HAL_ResonanceCircuitSenseOffsetInit(byDeviceId, byCoilId, HAL_DDM_MODE_FR_Q);
		}
	}
	else
	{
#if DIGITAL_DCDC
		HAL_D_DCDC_Stop(byDeviceId);
#else
		GPIO_SetOutput(GPIO_DCDC_EN, GPIO_CLR);
		TMR_Enable(TMR_DAC_INDEX, 0);
#endif
		
		GPIO_SetOutput(GPIO_DRV_EN_L, GPIO_CLR);
		GPIO_SetOutput(GPIO_DRV_EN_H, GPIO_CLR);
	}
}

uint32 HAL_GetFSKFreq(uint8 byDeviceId, uint8 byFSKParam, uint32 dwWorkingFreq)
{
	uint32 period = CPU_PWM_CLK / dwWorkingFreq;
	uint16 pwmResolution = 1000000UL/(CPU_PWM_CLK/1000000); //ps
	
	//depth = bit 0~1 of byFSKParam
	//polarity = bit 2 of byFSKParam
	uint8 polarity = (byFSKParam & 0x04) >> 2;
	uint8 depth    = byFSKParam & 0x03;
	sint16 adj = 0;
	uint32 freq = 0;
	
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	//see Qi spec for FSK depth definition
	switch(depth)
	{
	case 0:  //30.25~63.25ns
		adj = (31250UL+63250UL)/pwmResolution/2;
		break;
	case 1:  //61.5~94.5ns
		adj = (62500UL+94500UL)/pwmResolution/2;
		break;
	case 2:  //124~157ns
		adj = (124000UL+157000UL)/pwmResolution/2;
		break;
	case 3:  //249~282ns
		adj = (249000UL+282000UL)/pwmResolution/2;
		break;
	}

	if(polarity == 0)
	{
		adj = -adj;
	}
	
	period  += adj;
	freq = CPU_PWM_CLK/period;

	//For Star special request
	if((polarity == 1)&&(depth == 0))
	{
		freq = 126984UL;
	}
	
	return freq;
}

void HAL_FSKModulation(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase)
{
	DBG_Assert(0 == PWM_CheckRunning(PWM_INVERTER_INDEX), 0, 0);
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	//set new PWM parameter
	HAL_SetChargingFreqDutyPhase(byDeviceId, byCoilId, dwFreq, dwDuty, dwPhase);
}

uint16 HAL_GetRefTimer(void)
{
    //reference timer generally is a free running counter
    //Note: make the COMM_REFERENCE_TIME_FREQ and dwCommReferenceTimerMaxCount setting 
    //      consistent with the HW reference timer setting
	return TMR_GetCounterValue(TMR_COMM_REF_INDEX);
}

uint32 HAL_GetElapsedRefTime(uint32 dwTimeMark)
{
    //return the elapsed reference timer count
	uint32 currTime = HAL_GetRefTimer();
	uint32 elapsedTime;
	if(currTime > dwTimeMark)
	{
		elapsedTime = currTime - dwTimeMark;
	}
	else
	{
		elapsedTime = currTime + (gWCT_Params.dwCommReferenceTimerMaxCount - dwTimeMark);
	}
	return elapsedTime;
}

//requires to OFF all source to Vrail, and discharge Vrail
void HAL_PreparePowerSwitch(uint8 byDeviceId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
}

//Stop Vrail discharge, and ON the required power source to Vrail
void HAL_PowerSwitch(uint8 byDeviceId, WCT_POWER_TYPE_E ePowerType)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
}

sint16* HAL_GetDDMBuffer(uint8 byDeviceId)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	return (sint16*)gHAL_DDMRecordBuf[byDeviceId];
}

void HAL_DischargeRailVoltage(uint8 byDeviceId, boolean isEn)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	if(isEn)
	{
		GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_SET);
	}
	else
	{
		GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_CLR);
	}
}

void HAL_SetVrailVoltage(uint8 byDeviceId, uint16 wVoltage)
{
	ANALOG_CAL_PARAMS* pAnalogCalParams = NVM_GetAnalogCalibrationParam(byDeviceId);
	
	uint16 wOriginalVoltage;
	uint16   setVol, dutyCycle, setCtrlVol;
	sint32 sdwTemp;
	boolean bDisCharge = FALSE;
	
	DBG_Assert(wVoltage > HAL_VRAIL_MAX_VOL, 0, wVoltage);
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);

	setVol = wOriginalVoltage = HAL_GetRailVoltage(byDeviceId);
	if(wOriginalVoltage > (wVoltage+HAL_VRAIL_VOL_STEP) )
	{
		bDisCharge = TRUE;
		HAL_DischargeRailVoltage(byDeviceId, 1);
	}
	
	//Converge the DCDC output slowly to target voltage, to avoid surge current
	while(1)
	{
		if(wVoltage > wOriginalVoltage)
		{
			setVol += HAL_VRAIL_VOL_STEP;
			if(setVol > wVoltage)
			{
				setVol = wVoltage;
			}
		}
		else
		{
			if(setVol > HAL_VRAIL_VOL_STEP)
			{
				setVol -= HAL_VRAIL_VOL_STEP;
			}
			else
			{
				setVol = 0;
			}
			
			if(setVol < wVoltage)
			{
				setVol = wVoltage;
			}
		}
#if DIGITAL_DCDC
		HAL_D_DCDC_SetVoltage(byDeviceId, setVol);
#else
		//For analog buck/boost, convert the DCDC output voltage(Vo) to its control voltage(Vc)
		//Formula:
		//    Vo = (Vc*Slope + offset) >> Norm
		// => Vc = ((Vo << Norm) - Offset) / Slope
		sdwTemp = setVol;

		sdwTemp <<= pAnalogCalParams->wRailVoltageNorm;
		sdwTemp -= pAnalogCalParams->sdwRailVoltageOffset;
		sdwTemp /= pAnalogCalParams->sdwRailVoltageSlope;
		
		if(sdwTemp < 0)
		{
			sdwTemp = 0;
		}
		else if(sdwTemp > HAL_CHIP_SUPPLY)
		{
			sdwTemp = HAL_CHIP_SUPPLY;
		}
		
		setCtrlVol = sdwTemp;
		HAL_SetVrailCtrlVoltage(byDeviceId, setCtrlVol);
#endif
		
		if(setVol == wVoltage)
		{
			break;
		}
		else
		{
			HAL_DelayUs(200);
		}
	}
	
	
	if(bDisCharge)
	{
		HAL_DischargeRailVoltage(byDeviceId, 0);
	}
}

void HAL_SetLED(uint8 byDeviceId, HAL_LED_INDEX_E byLedId, HAL_LED_CTRL_TYPE_E CtrlType)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	
	if(byLedId == HAL_RED_LED_INDEX)
	{
		if(CtrlType == HAL_LED_ON)
		{
			GPIO_SetOutput(GPIO_LED1, GPIO_CLR);
		}
		else if(CtrlType == HAL_LED_OFF)
		{
			GPIO_SetOutput(GPIO_LED1, GPIO_SET);
		}
		else if(CtrlType == HAL_LED_TOG)
		{
			GPIO_SetOutput(GPIO_LED1, GPIO_TOG);
		}
	}
	else if(byLedId == HAL_GRN_LED_INDEX)
	{
		if(CtrlType == HAL_LED_ON)
		{
			GPIO_SetOutput(GPIO_LED2, GPIO_CLR);
		}
		else if(CtrlType == HAL_LED_OFF)
		{
			GPIO_SetOutput(GPIO_LED2, GPIO_SET);
		}
		else if(CtrlType == HAL_LED_TOG)
		{
			GPIO_SetOutput(GPIO_LED2, GPIO_TOG);
		}
	}
}

void HAL_SetLEDDim(uint8 byDeviceId, HAL_LED_INDEX_E byLedId, uint16 wDimIntensity)
{
	DBG_Assert(NUM_DEVICES <= byDeviceId, 0, byDeviceId);
	//Needs HW(PWM) support, not implemented yet.
}

boolean HAL_CheckFobActive(void)
{
	boolean ret = 0;
	
	// gHAL_bFobActive is a signal means active the key FOB feature
	// it may be a GPIO input signal, or a variable, set/cleared by other function.
	
	if(gHAL_bDBGFobType)
	{
		//gHAL_bFobActive is a trigger and keeps key FOB status some time
		static boolean bFodActiveStatus;
		static uint16 wFodDuration;
		static uint16 wFodStartTick;
	
		if(gHAL_bFobActive && (!bFodActiveStatus))
		{
			wFodStartTick = ST_GetTimerTick();
			wFodDuration = gHAL_wDBGFobDuration;
		}
		
		if(wFodDuration)
		{
			if(ST_GetElapasedTime(wFodStartTick) < wFodDuration)
			{
				ret = 1;
			}
			else
			{
				wFodDuration = 0;
			}
		}
		
		bFodActiveStatus = gHAL_bFobActive;	
	}
	else
	{
		//gHAL_bFobActive is a signal to keep key FOB status
		ret = gHAL_bFobActive;
	}

	return ret;
}

#if DIGITAL_DCDC

#define PWM_MAX_CNT   (CPU_PWM_CLK/PWM_D_DCDC_CLK)

#define SOFT_START_VOL_ADC_CNT_ADJUST  20    //For DCDC soft-start control, ADC raw data, approximately 140mV according to HW config

#define D_DCDC_SAMPLE_RES   	    0.015	//Current sense resistor value
#define D_DCDC_OVERCURRENT          8.5		//Current threshold for protection
#define D_DCDC_SAMPLE_MUL   	    8.73	//current OP(amplifier) gain
#define D_DCDC_OVERCURRENT_VOL      ((uint16)( D_DCDC_SAMPLE_RES * 1000 * D_DCDC_OVERCURRENT * D_DCDC_SAMPLE_MUL ))

#define CALCULATE_ADC_CNT  			2000UL
#define D_DCDC_VOL_SCALE_SHIFT_BIT  16

//Set the PID output spectrum to X*PWM_MAX_CNT/32768, PID output fractional value * 32768 = PWM duty
//Consider the fractional value is a U16 value, thus (uint16)(PID output value) = PWM duty.
#define D_DCDC_DUTY_TO_PIDFRAC16(x)   (x)

typedef struct
{
	//DCDC status flags
	boolean     bD_DCDC_Status;
	boolean     bD_DCDC_SoftStart;
	
	uint16 	    wSetVolAdcCnt;
	sint16      swRailDeltaAdcCnt;
	
	frac16_t    swPIDRes;
	frac16_t    swPrePIDRes;
	
	uint16 	    wPWMBuckDuty;
	
	//Over current protection counter
	uint16      wOverCurCnt;
} D_DCDC_RUNTIME_PARAMS;

typedef struct
{
	//PID parameters definition
	acc32_t  	sdwBuckKp;
	acc32_t  	sdwBuckKi;
	
	//The range of PID output
	frac16_t  	swPIDUpperLimit;
	frac16_t  	swPIDLowerLimit;
	
	//PID adjustment limitation
	frac16_t 	swResChangeUpperLimit;
	frac16_t 	swResChangeLowerLimit;
} D_DCDC_PID_PARAMS;

typedef struct
{
	uint32 		dwSoftStopVolAdcCnt;
	
	uint16      wSetVolScale;
	uint16      wReserved;
	
	//For DCDC mode PWM duty cycle limitation
	uint16 		wBuckPWMDutyMin;
	uint16 		wBuckPWMDutyMax;
	
	//For over current protection
	uint16	    wOverCurrentVol;
	uint16 	    wOverCurCntThreshold;
} D_DCDC_CTRL_PARAMS;

D_DCDC_RUNTIME_PARAMS gD_DCDC_RuntimeParams;
D_DCDC_PID_PARAMS gD_DCDC_PID_Params;
D_DCDC_CTRL_PARAMS gD_DCDC_CtrlParams;

GFLIB_CTRL_PI_P_AW_T_A32 gD_DCDC_Ctrl;

void HAL_D_DCDC_Init(void)
{
	//D_DCDC PID parameters initialization
	gD_DCDC_PID_Params.sdwBuckKp 				= ACC32(0.20);
	gD_DCDC_PID_Params.sdwBuckKi			    = ACC32(0.10);

	gD_DCDC_PID_Params.swPIDLowerLimit 			= FRAC16(0);
	gD_DCDC_PID_Params.swPIDUpperLimit 			= FRAC16(PWM_MAX_CNT*0.95/32768.0);
	
	gD_DCDC_PID_Params.swResChangeLowerLimit 	= FRAC16(-(PWM_MAX_CNT*0.05/32768.0));
	gD_DCDC_PID_Params.swResChangeUpperLimit 	= FRAC16(PWM_MAX_CNT*0.05/32768.0);
	
	//D_DCDC control parameters initialization
	gD_DCDC_CtrlParams.wBuckPWMDutyMin		    = (PWM_MAX_CNT*0.08);
	gD_DCDC_CtrlParams.wBuckPWMDutyMax 			= (PWM_MAX_CNT*0.95);
	
	//Over current protection parameters initialization
	gD_DCDC_CtrlParams.wOverCurrentVol 			= D_DCDC_OVERCURRENT_VOL;
	gD_DCDC_CtrlParams.wOverCurCntThreshold 	= 25;	//25*0.02ms
}

uint16 HAL_D_DCDC_VolToAdcCnt(uint8 byDeviceId)
{
	uint32 dwTemp;
	ANALOG_CAL_PARAMS* pAnalogCalParams = NVM_GetAnalogCalibrationParam(byDeviceId);

	//Use CALCULATE_ADC_CNT to get the real voltage
	dwTemp = CALCULATE_ADC_CNT;
	dwTemp *= pAnalogCalParams->wInputVoltageCalibration;
	dwTemp >>= NOM_CAL_SCALE_FACTOR_SIZE_IN_BITS;
	dwTemp *= ADC_REF_VOL;
	dwTemp >>= ADC_BITS;
	dwTemp *= HAL_RAIL_SENSE_MULTI_U;
	dwTemp >>= HAL_RAIL_SENSE_SHIFT_BITS;
	
	//Get ratio for adc_cnt to real voltage. Add D_DCDC_VOL_SCALE_SHIFT_BIT for better resolution
	dwTemp = (CALCULATE_ADC_CNT << D_DCDC_VOL_SCALE_SHIFT_BIT) / dwTemp;
	
	return (uint16)dwTemp;
}

void HAL_D_DCDC_SetVoltage(uint8 byDeviceId, uint16 wVoltage)
{
	uint32 dwTemp;
	if(0 == gD_DCDC_CtrlParams.wSetVolScale)
	{
		gD_DCDC_CtrlParams.wSetVolScale = HAL_D_DCDC_VolToAdcCnt(byDeviceId);
	}
	
	//Convert the target voltage to ADC cnt
	dwTemp = (uint32)wVoltage;
	dwTemp *= gD_DCDC_CtrlParams.wSetVolScale;
	dwTemp >>= D_DCDC_VOL_SCALE_SHIFT_BIT;
	gD_DCDC_RuntimeParams.wSetVolAdcCnt = dwTemp;
}

void HAL_D_DCDC_Start(uint8 byDeviceId)
{
	boolean sts;
	uint16 wIpOffsetVol;
	uint16 wIpLowThre;

	DBG_Assert(gD_DCDC_RuntimeParams.bD_DCDC_Status, 0, gD_DCDC_RuntimeParams.bD_DCDC_Status);
	
	gD_DCDC_CtrlParams.wSetVolScale = HAL_D_DCDC_VolToAdcCnt(byDeviceId);
	//softstop vol set to ~95% D_DCDC VolSet
	gD_DCDC_CtrlParams.dwSoftStopVolAdcCnt = ((uint32)gD_DCDC_RuntimeParams.wSetVolAdcCnt)*120/128;

	//Get the base level for Ipeak protection
	HAL_MiscADCForceSample(byDeviceId);
	wIpOffsetVol = HAL_ReadIpeakOffsetVol(byDeviceId);
	
	if(wIpOffsetVol > (gD_DCDC_CtrlParams.wOverCurrentVol+100))
	{
		wIpLowThre = wIpOffsetVol - gD_DCDC_CtrlParams.wOverCurrentVol;
	}
	else
	{
	    //Avoid saturation(low)
		wIpLowThre = 100;
	}
	
	//Set the upper and lower limits for CMP
	CMP_ClearRaisingFlag(CMP_D_DCDC_CURRENT_LOW);
	CMP_SetInternalDAC(CMP_D_DCDC_CURRENT_LOW, wIpLowThre);
	CMP_Enable(CMP_D_DCDC_CURRENT_LOW, 1);
	
	//Initialize the PI 
	gD_DCDC_Ctrl.a32PGain 					 = gD_DCDC_PID_Params.sdwBuckKp;
	gD_DCDC_Ctrl.a32IGain 					 = gD_DCDC_PID_Params.sdwBuckKi;
	gD_DCDC_Ctrl.f16LowerLim			     = gD_DCDC_PID_Params.swPIDLowerLimit;
	gD_DCDC_Ctrl.f16UpperLim 			     = gD_DCDC_PID_Params.swPIDUpperLimit;
	gD_DCDC_Ctrl.f32IAccK_1				     = 0;
	gD_DCDC_Ctrl.f16InErrK_1 				 = 0;
	GFLIB_CtrlPIpAWInit_F16(0, &gD_DCDC_Ctrl);
	
	//Initialize the flags
	gD_DCDC_RuntimeParams.swPrePIDRes    	 = 0;
	gD_DCDC_RuntimeParams.swPIDRes       	 = 0;
	gD_DCDC_RuntimeParams.wOverCurCnt    	 = 0;
	gD_DCDC_RuntimeParams.bD_DCDC_SoftStart  = TRUE;
	
	//Initialize the PWM
	gD_DCDC_RuntimeParams.wPWMBuckDuty 		 = gD_DCDC_CtrlParams.wBuckPWMDutyMin;
	
	//Make PWMB to output high to wake up the PWM driver at the beginning
	PWM_SetChannelPairs(PWM_D_DCDC_BUCK_INDEX, PWM_D_DCDC_CHANNLE_BUCK_HIGH, 0, 0);

	//Modules enable
	PWM_FracOn(PWM_D_DCDC_BUCK_INDEX, 1);
	PWM_Start(PWM_D_DCDC_BUCK_INDEX);

	ST_WaitMs(1);
	
	//Update the normal BUCK PWM duty cycle
	PWM_SetChannelPairs(PWM_D_DCDC_BUCK_INDEX, PWM_D_DCDC_CHANNLE_BUCK_HIGH, 0, gD_DCDC_RuntimeParams.wPWMBuckDuty);

	gD_DCDC_RuntimeParams.bD_DCDC_Status = TRUE;
}

void HAL_D_DCDC_Stop(uint8 byDeviceId)
{
	boolean sts;

	//Disable PWM fractional mode
    PWM_FracOn(PWM_D_DCDC_BUCK_INDEX, 0);
    //Stop PWM output
	PWM_Stop(PWM_D_DCDC_BUCK_INDEX);
	//Disable CMP module
    CMP_Enable(CMP_D_DCDC_CURRENT_LOW, 0);
	
	gD_DCDC_RuntimeParams.bD_DCDC_Status = FALSE;
}

//speed optimization by setting the code running in RAM
#pragma define_section XRAM_CODE ".code_in_ram"
#pragma section XRAM_CODE begin
void HAL_D_DCDC_Adjust(uint8 byDeviceId)
{
	uint16 tmp;
	uint16 wRailAdcCnt;
	frac16_t diff;
	
	//Run D_DCDC control only when D_DCDC status is true
	if(gD_DCDC_RuntimeParams.bD_DCDC_Status)
	{
		wRailAdcCnt = ADC_Read_D_DCDC_VolAdcCnt();
		
		//In soft-start state, set a fixed voltage adjustment step
		if(gD_DCDC_RuntimeParams.bD_DCDC_SoftStart)
		{
			if(wRailAdcCnt > gD_DCDC_CtrlParams.dwSoftStopVolAdcCnt)
			{
			    gD_DCDC_RuntimeParams.bD_DCDC_SoftStart = FALSE;
			}
			gD_DCDC_RuntimeParams.swRailDeltaAdcCnt = SOFT_START_VOL_ADC_CNT_ADJUST;
		}
		else
		{
		    //Normal PID
			gD_DCDC_RuntimeParams.swRailDeltaAdcCnt  = gD_DCDC_RuntimeParams.wSetVolAdcCnt-wRailAdcCnt;
		}
		
		
		//PID calculation
		gD_DCDC_RuntimeParams.swPIDRes = GFLIB_CtrlPIpAW_F16((gD_DCDC_RuntimeParams.swRailDeltaAdcCnt*10), 0, &gD_DCDC_Ctrl);
		
		//Limit the PID output changes
		diff = gD_DCDC_RuntimeParams.swPIDRes - gD_DCDC_RuntimeParams.swPrePIDRes;
		if(diff > gD_DCDC_PID_Params.swResChangeUpperLimit)
		{
			diff = gD_DCDC_PID_Params.swResChangeUpperLimit;
		}
		if(diff < gD_DCDC_PID_Params.swResChangeLowerLimit)
		{
			diff = gD_DCDC_PID_Params.swResChangeLowerLimit;
		}
		gD_DCDC_RuntimeParams.swPIDRes = gD_DCDC_RuntimeParams.swPrePIDRes + diff;
		gD_DCDC_RuntimeParams.swPrePIDRes = gD_DCDC_RuntimeParams.swPIDRes;
		
		//Translate the PID output to PWM duty cycle
		tmp = D_DCDC_DUTY_TO_PIDFRAC16(gD_DCDC_RuntimeParams.swPIDRes);
		if(tmp > gD_DCDC_CtrlParams.wBuckPWMDutyMax)
		{
			gD_DCDC_RuntimeParams.wPWMBuckDuty = gD_DCDC_CtrlParams.wBuckPWMDutyMax;
		}
		else
		{
			gD_DCDC_RuntimeParams.wPWMBuckDuty = tmp;
			if(gD_DCDC_RuntimeParams.wPWMBuckDuty < gD_DCDC_CtrlParams.wBuckPWMDutyMin)
			{
				gD_DCDC_RuntimeParams.wPWMBuckDuty = gD_DCDC_CtrlParams.wBuckPWMDutyMin;
			}
		}
		
		//Over current detection
		if(CMP_Check_D_DCDC_Status())
		{
			gD_DCDC_RuntimeParams.wOverCurCnt++;
			if(gD_DCDC_RuntimeParams.wOverCurCnt > gD_DCDC_CtrlParams.wOverCurCntThreshold)
			{
				HAL_D_DCDC_Stop(byDeviceId);
			}
			CMP_Clean_D_DCDC_Flag();
		}
		else
		{
			gD_DCDC_RuntimeParams.wOverCurCnt = 0;
			//Update the PWM value registers
			PWM_Update_D_DCDC_Channels(gD_DCDC_RuntimeParams.wPWMBuckDuty);
		}
	}
}
#pragma section XRAM_CODE end

#endif


boolean HAL_CheckDCDCError(uint8 byDeviceId)
{
    boolean ret = FALSE;
#if DIGITAL_DCDC
    ret = gD_DCDC_RuntimeParams.bD_DCDC_Status;
#else
    //Note: LTC3789 PGOOD is open drain output, schematic shows no external pull up resistor, 
    //      but a pull down resistor, and MCU internal pull-up is weak, so PG is not used.
    //ret = GPIO_GetInput(GPIO_DCDC_PG);
    ret = TRUE;
#endif
    return !ret;
}

boolean HAL_CheckMP81OverCurrent(uint8 byDeviceId)
{
	boolean ret = FALSE;
	if(PWM_CheckFaultFlag())
	{
		ret = TRUE;
	}
	return ret;
}

void HAL_EnterLowPowerMode(uint16 wStopTimeMs)
{
    // low power mode is application specific, carefully to set the 
    // modules function when enter & exit lower power mode
    
#if LOW_POWER_MODE_ENABLE
	uint16 wAdjustStopTime = 0;
	uint16 tmp = 0;
	uint8 sts;
	
	// Enter stop mode in the analog ping interval between adjacent ping round
	// Don't enter stop mode in the digital ping interval between coils, because discharge Vrail and DCDC DIS/EN need more time
	// DCDC converter is disabled in the analog ping interval
	// Adjust STOP duration by the specified overhead value
	if(wStopTimeMs > HAL_LOW_POWER_TIME_LIMIT)
	{
		DBG_Assert(wStopTimeMs < (HAL_LOW_POWER_CLOCK_SWITCH_TIME + HAL_LOW_POWER_RESTORE_TIME), 0, wStopTimeMs);
		wAdjustStopTime = wStopTimeMs - HAL_LOW_POWER_CLOCK_SWITCH_TIME - HAL_LOW_POWER_RESTORE_TIME;
	}
	else
	{
	    //Don't enter lower power mode when sleep time is less than limit
		wAdjustStopTime = 0;
	}
	
	if(wAdjustStopTime > 0)
	{
		boolean bDPPower;
		boolean bRailDis;
		
		//Check power source, discharge resistor...
#if DIGITAL_DCDC
		bDPPower = gD_DCDC_RuntimeParams.bD_DCDC_Status;
#else
		bDPPower = GPIO_GetInput(GPIO_DCDC_EN);
#endif
		bRailDis = GPIO_GetInput(GPIO_RAIL_DIS);
#if USB_POWER_ENABLE
		//Disable PPS if PD power is used
		if(kSourceCap_RangeVoltageCurrent == USBGetPDType())
		{
			USB_PDPowerSwitch(19000ul, 22000ul, 1);
		}
#endif
		sts = HAL_DisableIRQ();
		//Disable power, peripherals...
#if DIGITAL_DCDC
		HAL_D_DCDC_Stop(0);
#else
		GPIO_SetOutput(GPIO_DCDC_EN, GPIO_CLR);
#endif

		GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_SET);
		GPIO_SetOutput(GPIO_DRV_EN_L, GPIO_CLR);
		GPIO_SetOutput(GPIO_DRV_EN_H, GPIO_CLR);
		
		ADC_PowerDown();
		DAC_PowerDown();
		PWM_FracOn(PWM_INVERTER_INDEX, 0);
		PWM_FracOn(PWM_Q_MEASURE_INDEX, 0);
		WDG_Enable(0); //disable watch dog

		//Set timer to wake up system
		TMR_EnableInStopMode(TMR_LOW_POWER_INDEX, 1); //Allow timer(wake MCU from low power mode) to stay alive in stop mode
		
		if(bDPPower)
		{
			tmp += gWCT_Params.wRailSetupTime * 2;
		}
		DBG_Assert(wAdjustStopTime <= tmp, 0, 0);
		wAdjustStopTime -= tmp;
		TMR_SetCompareValue(TMR_LOW_POWER_INDEX, wAdjustStopTime);
		
		//Reduce frequency to enter low power mode
		OCCS_SetLowSpeed(); //2ms
		//Remove discharge resistor
		GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_CLR); 
		
		HAL_RestoreIRQ(sts);
		TMR_Enable(TMR_LOW_POWER_INDEX, 1);
		asm( STOP ); //Go to STOP mode
		//Note: code execution will resume here after wake timer interrupt
		sts = HAL_DisableIRQ();
		
		//Restore core clock to 100M
		OCCS_SetHighSpeed();  //4ms
		
		PWM_FracOn(PWM_INVERTER_INDEX, 1); // take 25us to power up
		PWM_FracOn(PWM_Q_MEASURE_INDEX, 1);
		DAC_PowerUp(); // take 12us to power up
		HAL_DelayUs(25);
		ADC_PowerUp();
		WDG_Kick();
		WDG_Enable(1);
		
		HAL_RestoreIRQ(sts);
		
		GPIO_SetOutput(GPIO_DRV_EN_L, GPIO_SET);
		GPIO_SetOutput(GPIO_DRV_EN_H, GPIO_SET);
		
		if(bRailDis)
		{
			GPIO_SetOutput(GPIO_RAIL_DIS, GPIO_SET);
		}
		
		if(bDPPower)
		{
			ST_WaitMs(gWCT_Params.wRailSetupTime);
#if DIGITAL_DCDC
			HAL_D_DCDC_Start(0);
#else
			GPIO_SetOutput(GPIO_DCDC_EN, GPIO_SET);
#endif
			ST_WaitMs(gWCT_Params.wRailSetupTime);
			ST_AddTickTimer(wStopTimeMs); //Adjust system time ticks
		}
		else
		{
			ST_AddTickTimer(wStopTimeMs); //Adjust system time ticks
		}
	}
#endif
}

#if USB_POWER_ENABLE
#define PTN5110_ADDR   0x51
volatile boolean gHAL_USBPDInterruptEnable = 0;

void HAL_USBPDI2CInit(void)
{
	//do nothing
}

void HAL_USBPDI2CDeinit(void)
{
	//do nothing
}

void HAL_I2C_Disable(void)  //TODO
{
	//do nothing
	I2C_Enable(0);
	I2C_ClearFlag();
}

void HAL_I2C_Enable(void)   //TODO
{
	//do nothing
	I2C_Enable(1);
}

void HAL_I2C_ReleaseBus(void)
{
    uint8_t i = 0;

    /* Config pin mux as gpio */
	GPIO_SetOutput(GPIO_I2C0SCL, GPIO_CLR);
	GPIO_SetOutput(GPIO_I2C0SDA, GPIO_SET);
	GPIO_SetMode(GPIO_I2C0SCL, GPIO_MODE_GP, 1);
	GPIO_SetMode(GPIO_I2C0SDA, GPIO_MODE_GP, 1);
	TMR_WaitUs(10);
	
	GPIO_SetOutput(GPIO_I2C0SCL, GPIO_SET);
	TMR_WaitUs(10);
	
    /* Drive SDA low first to simulate a start */
	GPIO_SetOutput(GPIO_I2C0SDA, GPIO_CLR);
	TMR_WaitUs(10);
	
	GPIO_SetOutput(GPIO_I2C0SCL, GPIO_CLR);
	TMR_WaitUs(10);
	
	//set SDA gpio input 
	GPIO_SetMode(GPIO_I2C0SDA, GPIO_MODE_GP, 0);
	TMR_WaitUs(10);

    /* Send 9 pulses on SCL and check if SDA get high */
    for (i = 0; i < 9; i++)
    {
    	if(GPIO_GetInput(GPIO_I2C0SDA))
    	{
    		break;
    	}

    	GPIO_SetOutput(GPIO_I2C0SCL, GPIO_SET);
    	TMR_WaitUs(10);

    	GPIO_SetOutput(GPIO_I2C0SCL, GPIO_CLR);
    	TMR_WaitUs(10);
    }

    /* Send stop */
    GPIO_SetOutput(GPIO_I2C0SDA, GPIO_CLR);
    GPIO_SetMode(GPIO_I2C0SDA, GPIO_MODE_GP, 1);
    TMR_WaitUs(10);
    
    GPIO_SetOutput(GPIO_I2C0SCL, GPIO_SET);
    TMR_WaitUs(10);

    GPIO_SetOutput(GPIO_I2C0SDA, GPIO_SET);
    TMR_WaitUs(10);
    
    GPIO_SetMode(GPIO_I2C0SCL, GPIO_MODE_PER, 0);
    GPIO_SetMode(GPIO_I2C0SDA, GPIO_MODE_PER, 0);
}

uint8_t HAL_USBPDI2CWrite(uint32_t slave, uint32_t registerAddr, uint8_t registerLen, uint8_t *data, uint32_t num)
{
    /* success - return 0; fail - return other values */
#define TX_DATA_NUM  10
    uint8 buf[TX_DATA_NUM], i;
    buf[0] = registerAddr;
    
    if(num > (TX_DATA_NUM-1))
    {
       while(1) {}
    }
    
    for(i=0; i<num; i++)
    {
       buf[1+i] = data[i];
    }
    
    return !I2C_Write(PTN5110_ADDR, buf, num+1);
}

uint8_t HAL_USBPDI2CRead(uint32_t slave, uint32_t registerAddr, uint8_t registerLen, uint8_t *data, uint32_t num)
{
    /* success - return 0; fail - return other values */
    uint8 buf = registerAddr;
    if(I2C_Write(PTN5110_ADDR, &buf, 1))
    {
       return !I2C_Read(PTN5110_ADDR, data, num);
    }
    return 1;
}

void HAL_USBPDInit(void)
{
	GPIO_SetMode(GPIO_nINT, GPIO_MODE_GP, 0);
	gHAL_USBPDInterruptEnable = FALSE;
}

void HAL_EnableUSBPDIrq(boolean isEn)
{
	gHAL_USBPDInterruptEnable = isEn;
}

void HAL_USBPDPolling(void)
{
	if(gHAL_USBPDInterruptEnable)
	{
		if (!GPIO_GetInput(GPIO_nINT))
		{
			USBPD_PHYIsr();
		}
	}
}


// for QC
void HAL_USBDataLineInit(void)
{
	//DP output, DN input
	GPIO_SetMode(GPIO_CTRL_DN_0, GPIO_MODE_GP, 0);
	GPIO_SetMode(GPIO_CTRL_DN_1, GPIO_MODE_GP, 0);
	GPIO_SetMode(GPIO_CTRL_DP_0, GPIO_MODE_GP, 1);
	GPIO_SetMode(GPIO_CTRL_DP_1, GPIO_MODE_GP, 1);
}

void HAL_USBDataLineDNInit(void)
{
	GPIO_SetMode(GPIO_CTRL_DN_0, GPIO_MODE_GP, 1);
	GPIO_SetMode(GPIO_CTRL_DN_1, GPIO_MODE_GP, 1);	
}
void HAL_SetDataPVol(HAL_USB_DL_VOL_E eVol)
{
	switch(eVol)
	{
	case HAL_USB_DATA_LINE_00:
		GPIO_SET_DP_COMB_00;
		break;
	case HAL_USB_DATA_LINE_06:
		GPIO_SET_DP_COMB_06;
		break;
	case HAL_USB_DATA_LINE_25:
		GPIO_SET_DP_COMB_25;
		break;
	case HAL_USB_DATA_LINE_33:
		GPIO_SET_DP_COMB_33;
		break;
	}
}

void HAL_SetDataNVol(HAL_USB_DL_VOL_E eVol)
{
	switch(eVol)
	{
	case HAL_USB_DATA_LINE_00:
		GPIO_SET_DN_COMB_00;
		break;
	case HAL_USB_DATA_LINE_06:
		GPIO_SET_DN_COMB_06;
		break;
	case HAL_USB_DATA_LINE_25:
		GPIO_SET_DN_COMB_25;
		break;
	case HAL_USB_DATA_LINE_33:
		GPIO_SET_DN_COMB_33;
		break;
	}
}

HAL_USB_DL_VOL_E HAL_CheckDataNVol(void)
{
	return HAL_USB_DATA_LINE_00;
}

#endif

