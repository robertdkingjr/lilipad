/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#ifndef __HAL_H__
#define __HAL_H__

typedef enum
{
	HAL_DDM_MODE_CHARGING = 0,
	HAL_DDM_MODE_FR_Q
}HAL_DDM_MODE_E;

typedef enum
{
	HAL_TP_TYPE_SET = 0,
	HAL_TP_TYPE_CLR = 1,
	HAL_TP_TYPE_TOG = 2
}HAL_TP_TYPE_E;

typedef enum
{
	HAL_LED_ON = 0,
	HAL_LED_OFF,
	HAL_LED_TOG
}HAL_LED_CTRL_TYPE_E;

typedef enum
{
	HAL_RED_LED_INDEX = 0,
	HAL_GRN_LED_INDEX,
	HAL_LED_NUM
}HAL_LED_INDEX_E;

typedef enum
{
	HAL_USB_DATA_LINE_00,
	HAL_USB_DATA_LINE_06,
	HAL_USB_DATA_LINE_25,
	HAL_USB_DATA_LINE_33
}HAL_USB_DL_VOL_E;

#define HAL_CHIP_SUPPLY  3300UL
#define HAL_VRAIL_MAX_VOL    17000UL
#define HAL_VRAIL_MIN_VOL     3000UL
#define HAL_VRAIL_MAX_VOL_FOR_DP    10000UL

#define HAL_DIM_MAX_INTENSITY   10000UL

void HAL_Init(void);

uint8 HAL_DisableIRQ(void);
void HAL_RestoreIRQ(uint8 bySts);
void HAL_DelayUs(uint16 wWaitUs);
void HAL_KickWatchDog(void);
void HAL_SetTP(uint8 byId, HAL_TP_TYPE_E eType);
boolean HAL_CheckInterverRunning(uint8 byDeviceId);
uint16 HAL_GetMiscADCData(uint8 byDeviceId, uint8 byChannel);
void HAL_PrintChar(uint8 byChar);
void HAL_PrintCharISRAck(void);

void HAL_ReadFlashData(uint8 * pbyBuf, NVM_BYTE_POINTER pbyAddr, uint16 wLen);
void HAL_WriteFlashData(uint32 * pdwBuf, uint32 dwAddr, uint16 wLen);
void HAL_EraseFlashSector(uint32 dwAddr);

void HAL_PrepareChargingResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq);
void HAL_ChargeResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty);
void HAL_StopChargingResonanceCapacitor(uint8 byDeviceId, uint8 byCoilId);
void HAL_PreapareMeasureResonancePeriod(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq);

void HAL_StartMesaureResonancePeriod(uint8 byDeviceId, uint8 byCoilId);
uint16 HAL_CheckResonancePeriod(uint8 byDeviceId, uint8 byCoilId);
void HAL_StopMeasureResonancePeriod(uint8 byDeviceId, uint8 byCoilId);
uint32 HAL_ConvertPeriodToFreq(uint8 byDeviceId, uint8 byCoilId, uint32 period);

void HAL_PreQMeasure(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint8 byTriggerPosPercent);
uint8 HAL_StartQMeasure(uint8 byDeviceId, uint8 byCoilId);
uint16 HAL_CheckQMeasureFinished(uint8 byDeviceId, uint8 byCoilId, uint8 byNum);
void HAL_StopQMeasure(uint8 byDeviceId, uint8 byCoilId);
uint16* HAL_GetQMeasureData(uint8 byDeviceId, uint8 byCoilId, uint8 byDataNum);

uint16 HAL_GetQFacotrSenseCircuitADCCntOffset(uint8 byDeviceId, uint8 byCoilId);
void HAL_ResonanceCircuitSenseOffsetInit(uint8 byDeviceId, uint8 byCoilId, HAL_DDM_MODE_E eMode);
void HAL_SetADCMode(uint8 byDeviceId, uint8 byCoilId, HAL_DDM_MODE_E mode);
uint16 HAL_SetTriggerPosition(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint8 byTrigPosPercent);
void HAL_StartCharging(uint8 byDeviceId, uint8 byCoilId);
void HAL_StopCharging(uint8 byDeviceId, uint8 byCoilId);
sint16 HAL_GetTopTemperature(uint8 byDeviceId);
sint16 HAL_GetCoilTemperature(uint8 byDeviceId, uint8 byCoilId);
uint16 HAL_GetResonanceCapValue(uint8 byDeviceId, uint8 byCoilId);

//WCT HAL layer
uint16 HAL_GetRailVoltage(uint8 byDeviceId);
uint16 HAL_GetBatteryVoltage(void);
uint16 HAL_GetCoilCurrent(uint8 byDeviceId, uint8 byCoilId);
uint16 HAL_GetInputCurrent(uint8 byDeviceId);
void HAL_EnableDDM(uint8 byDeviceId, uint8 byCoilId, uint8 byIsEn);
uint16 HAL_AnalogPing(uint8 byDeviceId, uint8 byCoilId);
uint16 HAL_FindAdcTriggerPos(uint8 byDeviceId, uint8 byCoilId, uint8 byDiv, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase);
void HAL_SetChargingBridge(uint8 byDeviceId, uint8 byCoilId, uint8 byBridge);
void HAL_EnableCoilDischarge(uint8 byDeviceId, uint8 byCoilId, boolean bIsEn);
void HAL_EnableChargingOnCoil(uint8 byDeviceId, uint8 byCoilId, boolean bIsEn);
void HAL_SetChargingFreqDutyPhase(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase);
void HAL_EnableCoils(uint8 byDeviceId, uint8 byCoilId, boolean bIsEn);
void HAL_SetVrailVoltage(uint8 byDeviceId, uint16 wVoltage);
void HAL_EnableWCT(uint8 byDeviceId, boolean bIsEn);
uint32 HAL_GetFSKFreq(uint8 byDeviceId, uint8 byFSKParam, uint32 dwWorkingFreq);
void HAL_FSKModulation(uint8 byDeviceId, uint8 byCoilId, uint32 dwFreq, uint32 dwDuty, uint32 dwPhase);
uint16 HAL_GetRefTimer(void);
uint32 HAL_GetElapsedRefTime(uint32 dwTimeMark);

void HAL_PreparePowerSwitch(uint8 byDeviceId);
void HAL_PowerSwitch(uint8 byDeviceId, WCT_POWER_TYPE_E ePowerType);
sint16* HAL_GetDDMBuffer(uint8 byDeviceId);
void HAL_DischargeRailVoltage(uint8 byDeviceId, boolean bIsEn);

void HAL_SetLED(uint8 byDeviceId, HAL_LED_INDEX_E byLedId, HAL_LED_CTRL_TYPE_E CtrlType);
void HAL_SetLEDDim(uint8 byDeviceId, HAL_LED_INDEX_E byLedId, uint16 wDimIntensity);
void HAL_EnterLowPowerMode(uint16 wStopTimeMs);

boolean HAL_CheckFobActive(void);

boolean HAL_CheckDCDCError(uint8 byDeviceId);
void HAL_MiscADCForceSample(uint8 byDeviceId);

#if DIGITAL_DCDC
void HAL_MiscADCRead(void);
void HAL_D_DCDC_ADCHandler(uint8 byDeviceId);
uint16 HAL_ReadIpeakOffsetVol(uint8 byDeviceId);

void HAL_D_DCDC_Init(void);
uint16 HAL_D_DCDC_VolToAdcCnt(uint8 byDeviceId);
void HAL_D_DCDC_SetVoltage(uint8 byDeviceId, uint16 wVoltage);
void HAL_D_DCDC_Start(uint8 byDeviceId);
void HAL_D_DCDC_Stop(uint8 byDeviceId);
void HAL_D_DCDC_Adjust(uint8 byDeviceId);

#else
void HAL_MiscADCISRAck(uint8 byDeviceId);
void HAL_SetVrailCtrlVoltage(uint8 byDeviceId, uint16 wCtrlVol);
#endif


#if USB_POWER_ENABLE
void HAL_USBPDInit(void);
void HAL_EnableUSBPDIrq(boolean isEn);
void HAL_USBPDPolling(void);
void HAL_USBPDI2CInit(void);
void HAL_USBPDI2CDeinit(void);
void HAL_I2C_Disable(void);
void HAL_I2C_Enable(void);
void HAL_I2C_ReleaseBus(void);
uint8_t HAL_USBPDI2CWrite(uint32_t slave, uint32_t registerAddr, uint8_t registerLen, uint8_t *data, uint32_t num);
uint8_t HAL_USBPDI2CRead(uint32_t slave, uint32_t registerAddr, uint8_t registerLen, uint8_t *data, uint32_t num);

void HAL_USBDataLineInit(void);
void HAL_USBDataLineDNInit(void);
void HAL_SetDataPVol(HAL_USB_DL_VOL_E eVol);
void HAL_SetDataNVol(HAL_USB_DL_VOL_E eVol);
HAL_USB_DL_VOL_E HAL_CheckDataNVol(void);
#endif

boolean HAL_CheckMP81OverCurrent(uint8 byDeviceId);

#endif
