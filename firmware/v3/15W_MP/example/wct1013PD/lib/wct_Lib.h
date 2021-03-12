/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#ifndef __WCT_LIB_H__
#define __WCT_LIB_H__


#define FREQUENCY_CONTROL                       FALSE
#define PHASE_CONTROL                           FALSE
#define DUTY_CONTROL                            TRUE
#define RAIL_CONTROL                            TRUE

#define LIB_MAX_DUTY_CYCLE   5000UL
#define LIB_MAX_PHASE_SHIFT 10000UL

#define LOW_POWER_MODE_TIME_INFINITE             0xFFFFU
#define FATAL_FAULT_TIMEOUT                     ( 60000UL * 255 )     // Report of '255' indicates fatal fault - don't retry


//For RUNTIME_PARAMS
#define MAX_POWER_LOSS_SAMPLE_HISTORY           50U
#define MAX_COIL_NUM_PER_DEVICE                 10U

//For FSK_CTRL_T
#define FSK_MAX_TX_DATA_NUM                     16U

//For RF_DATA_INTERRUPT_PARAMS
#define COMM_EDGE_BUFFER_SIZE                   20U

//For RF_DATA_PARAMS
#define MAX_PACKET_SIZE                         30U

//For RF_DATA_PARAMS & RF_DATA_INTERRUPT_PARAMS
#define NUM_COMM_CHANNELS_PER_DEVICE            4U

//For gDDM_RecordBuf
#define RECORD_BUF_LEN   				       (512U)

typedef enum
{
    RX_NONE = 0,
    RX_PREPARE_CHARGE,                     // can be seen as RX detected and give user indication
    RX_CHARGING,                           // real charging, but indication may be slow
    RX_CHARGED,
    RX_UNDEFINE,
    RX_FAULT
} RX_CHARGING_STATUS;

typedef enum
{
    RX_SUCCESS = 0,
    RX_WPC_EPT_UNKNOWN,
    RX_WPC_EPT_INTERNAL_FAULT,
    RX_WPC_EPT_OVER_TEMP,
    RX_WPC_EPT_OVER_VOLT,
    RX_WPC_EPT_OVER_CURRENT,
    RX_WPC_EPT_BATTERY_FAILURE,
    RX_WPC_EPT_NO_RESPONSE,
    RX_WPC_EPT_RESTART_POWERTRANSFER,
    RX_WPC_EPT_NEGOTIATION_FAILURE,
    RX_WPC_EPT_RESERVED,
    RX_WPC_PACKET_INCOMPATIBLE,
    RX_WPC_PACKET_POWER_BEYOND_CAPABILITY,
    RX_WPC_PACKET_RCVPWR_TIMEOUT
} RX_CHARGING_ERRORS;

typedef enum
{
    TX_ERROR_HALT = 0,
    TX_APP_HALT,
    TX_OBJECT_DETECTION,
    TX_COIL_SELECTION,
    TX_COIL_SELECTION_CFM,
    TX_DIGITAL_PING,
    TX_IDENTIFICATION,
    TX_CONFIGURATION,
    TX_NEGOTIATION,
    TX_CALIBRATION,
    TX_POWER_TRANSFER,
    TX_RENEGOTIATION,
    TX_RECHARGE_RETRY
} TX_CHARGING_STATUS;

typedef enum
{
    TX_SUCCESS = 0,
    TX_CHIP_ERROR,
    TX_PRE_FOD_ERROR,
    TX_FOD_ERROR,
    TX_QFOD_ERROR,
    TX_RUNTIME_PARAM_ERROR,
    TX_CHARGE_REPEATED_FAIL
} TX_CHARGING_ERRORS;

typedef enum
{
	FREQUENCY_CONTROL_ALGORITHM = 0,
	BRIDGE_CONTROL_ALGORITHM,
	PHASE_CONTROL_ALGORITHM,
	DUTY_CYCLE_CONTROL_ALGORITHM,
	RAIL_CONTROL_ALGORITHM
} CONTROL_ALGORITHM;

typedef enum
{
    WPC_CHARGING = 0,
    PMA_CHARGING
} CHARGING_TYPE;

typedef enum
{
	POWER_TYPE_ANALOG_PING = 0,
	POWER_TYPE_DIGITAL_PING
}WCT_POWER_TYPE_E;

typedef enum
{
    RECHARGETIME_RX_UNKNOWN = 0,
    RECHARGETIME_RX_CHARGE_COMPLETE,
    RECHARGETIME_RX_INTERNAL_FAULT,
    RECHARGETIME_RX_OVER_TEMP,
    RECHARGETIME_RX_OVER_VOLT,
    RECHARGETIME_RX_OVER_CURRENT,
    RECHARGETIME_RX_BATTERY_FAILURE,
    RECHARGETIME_RX_NO_RESPONSE,
    RECHARGETIME_RX_RESTART_POWERXFER,
    RECHARGETIME_RX_NEGOTIATION_FAILURE,
    RECHARGETIME_RX_POWER_BEYOND_CAPABILITY,           
    RECHARGETIME_TX_RCVPWR_TIMEOUT,
    RECHARGETIME_TX_PREFOD_ERROR,
    RECHARGETIME_TX_FOD_ERROR,
    RECHARGETIME_TX_QFOD_ERROR,
    RECHARGETIME_TX_CHARGE_REPEATED_FAIL
}E_RECHARGETIME_SETTYPE;

// RF Data Rx States
typedef enum
{
    RF_DATA_RX_STATE_IDLE = 0,
    RF_DATA_RX_STATE_PREAMBLE_WAIT,
    RF_DATA_RX_STATE_START_BIT_WAIT,
    RF_DATA_RX_STATE_DATA_RECEIVE,
    RF_DATA_RX_STATE_STOP_BIT_WAIT,
    RF_DATA_RX_STATE_BYTE_RECEIVED,
    RF_DATA_RX_STATE_PACKET_RECEIVED,
    RF_DATA_RX_STATE_ERROR
}RF_DATA_RX_STATE;


typedef enum
{
	CONTROL_TYPE_HALF_BRIDGE_FREQ = 0, 
	CONTROL_TYPE_FULL_BRIDGE_PHASE, 
	CONTROL_TYPE_FULL_BRIDGE_FREQ, 
	CONTROL_TYPE_MAX
} CONTROL_TYPE;


// Device Control Flags
typedef struct
{
	uint32 bPowering              : 1;    // are we supplying power?
	uint32 bCharged               : 1;    // indicates a battery charge is complete
	uint32 bDevicePresent         : 1;    // is the feedback signal present?
	uint32 bCalibrated            : 1;
	uint32 bFodSupported          : 1;
	uint32 bSendFSK               : 1;
	uint32 bSendFSKFinished       : 1;
	uint32 bStartBitReceived      : 1;
	uint32 bPingRetryAttempted    : 1;
	uint32 bCheckNegTimeOut       : 1;
	uint32 bSafeDPChecked         : 1;
	uint32 bNegFODFaultFlag       : 1;
	uint32 bNegPowerPacketFaultFlag  : 1;
	uint32 bFobActiveStatus       : 1;
	uint32 bOffAfterFSKFinished   : 1;
	uint32 bInitialQFactorMeasured  : 1;
	uint32 bMPLFlag                 : 1;
	uint32 bActivePowerProtection   : 1;
	uint32 bPreQfMeasured           : 1;
	uint32 bReserved                :13;
} DEVICE_CONTROL_FLAGS;

typedef struct
{
	uint16 wMaximumPowerHalfWatt;
	uint16 wGuaranteedPowerHalfWatt;
	uint8  byFSKParam;
	uint8  byPowerPacketType;
} POWER_CONTRACT;

typedef struct
{
    uint8                        byDeviceId;
    uint8                        byCoilIdIndex;
    
    uint16                       wPreviousAnalogPingAdcCounts[ MAX_COIL_NUM_PER_DEVICE ];
    uint8                        byAnalogPingDetect;

    uint8                        byMaxSignalCoilIndex;
    uint8                        byMaxSignalStrength;
    uint8                        byCoilSignalStrength[ MAX_COIL_NUM_PER_DEVICE ];
    
    uint16                       wMaxTxHalfWatts;
    CHARGING_TYPE                ChargeType;
    uint8						 byControlType;

    POWER_CONTRACT               tmpPowerContract;
	POWER_CONTRACT               PowerContract;

    // Reference time
    uint16                       wPingStartRefCount;
    uint16                       wStartBitReferenceCount;
    uint16                       wLastPacketRefCount;
    uint16                       wElapsedTimeFromStartBit;
    
    // Packet
    uint8                        byCommPacketTypeReceived;
    uint8                        byConfigPacketCount;

    // Control params
    uint8                        byActiveBridgeType;
    uint8                        byActiveCoilId;
    uint32                       dwActiveFreq;
	uint16                       wActiveRailMv;
	uint16                       wActivePhase;
	uint16                       wActiveDutyCycle;
	uint16                       wMaxVolLimit;
    CONTROL_ALGORITHM            eControlAlgorithm;
    uint8                        byPowerFactor;

    // PID params
    uint8                        byPidDelayTime;
    uint8                        byIterationNum;
    uint16                       wSetPoint;
    uint16                       byIterationCount;
    uint16                       wActiveTimeCount;
    uint16                       wActiveTimeThreshold;
    sint16                       iPreviousError;
    sint16                       iCurrentError;
    sint16                       iProportionalTerm;
    sint16                       iIntegralTerm;
    sint16                       iDerivativeTerm;
    sint16                       iPidOutput;

    //time out
    uint16                       wLastCEPMsgHeard;
    uint16                       wLastMsgHeaderHeard;
    uint16                       wOpStateTimeAccumMs;
    uint16                       wLastReceivedPowerMessage;
    uint32                       dwPowerXferRetryTimerMs;
    uint16                       wDigitalPingRetryTimerMs;
    uint16                       wLowPowerModeTimeMs;
    uint16                       wCalibrationTimeAccumMs;
    
    //Power Loss params
	uint16                       wMeasuredQFactor;
	uint16                       wRetryMeasuredQFactor;
	uint32                       dwRxReportedPower;
    
    uint16						 wMeasuredInputVoltageMs;
    uint16						 wMeasuredCoilCurrentMs;
    uint16						 wMeasuredInputCurrentMs;
    uint16 						 wMeasuredBatVoltageMs;
    uint32						 dwMeasuredInputPowerMs;
    uint16                       wCalculatedInputVoltage;
    uint16                       wCalculatedCoilCurrent;
    uint32                       wCalculatedInputCurrent;
    uint32                       dwCalculatedInputPower;
    
    uint32                       dwPowerLossIndicationToPwrCessationMs;
    uint32                       dwPowerLossFaultTimer;
    uint16                       wPowerLossDetectionThreshold;
    uint8                        byPowerLossFaultCount;
	uint8                        byNumFodTripsToIndication;
    
    uint16                       wFodBufferIndex;
    uint16                       wFodSampleCount;
    uint32                       dwCoilCurrentHistory[ MAX_POWER_LOSS_SAMPLE_HISTORY ];
    uint32                       dwInputCurrentHistory[ MAX_POWER_LOSS_SAMPLE_HISTORY ];
    uint32                       dwInputVoltageHistory[ MAX_POWER_LOSS_SAMPLE_HISTORY ];

    sint16                       swTxInternalLoss;
    sint16                       swParasiticLoss;
    sint16                       swNormalizationLoss;
    sint16                       swNormalizedParasiticLoss;
    
    DEVICE_CONTROL_FLAGS         btDeviceControlFlags;
    
    //Fast charging
    uint8                        byFastChargingState;
    
    //DDM
    uint8                        byDDMReStartNeeded;
    uint8                        byDDMTimeOutCnt;        
    uint8						 byDDMTryCnt;
    uint16                       wDDMLastPktTimerMs;
    uint16                       wDDMReStartTimerCntMs;

    //status
	TX_CHARGING_STATUS           TxChargingStatus;
	TX_CHARGING_ERRORS           TxChargingErrors;
    RX_CHARGING_STATUS           RxChargingStatus;
    RX_CHARGING_ERRORS           RxChargingErrors;
    
    //Charge counter, retry counter
    uint8                        byContinueChargeCnt;
    uint8                        byContinueChargeCoilId;
    uint8                        byPowerXferRetryCnt;
    
    uint8                        eOpState;
    uint8                        ePingState;
    uint8                        eCalibrationState;
    uint8                        eQFactorMeasureState;
    uint8                        ePowerSwitchState;
    
    //Calibration
    sint32                       sdwTransmittedPowerInLightMode;
    sint32                       sdwTransmittedPowerInConnectMode;
    uint16                       wReceivedPowerInLightMode;
    uint16                       wReceivedPowerInConnectMode;
    sint32                       sdwCalibrationCoeffA;
    sint32                       sdwCalibrationCoeffB;
    sint32                       sdwRawTransmitterPower;
    sint32                       sdwCalibratedTransmitterPower;
    
    uint8                        byFodWindowSize;
    uint8                        byFodWindowOffset;
    
    uint8                        byNotifyPacket;
    uint8                        byNotifyStartBit;    
	uint8                        byNotifyFSKFinished;
	uint8                        byNotifyDecoderIdle;
    uint8                        byNotifyPacketHeader;
    uint8                        byNotifyReserved;
    uint16                       byNotifyPacketTimeMark;
    uint16                       byNotifyStartTimeMark;
    uint16                       wChangeBitMap;
    
    uint16                       wMPLTargetPPT;    
    uint32                       dwQFactorAccum;    
    uint8                        byQfactorMeasureCnt;
    
    uint8                        byPreFodQfCoilIdIndex;
    uint16                       wPreFodQf;
    uint16                       wPreFODTryCnt;
    
    uint16                       wMaxCoilCurrentLimit;
} RUNTIME_PARAMS;

typedef struct
{
	uint8  byState;
	uint8  byModulated;
	uint8  byBitsCnt;
	uint8  byBitsPerUnit;
	uint8  byUnitIndex;
	uint8  bySendUnitNum;
	uint8  byWorkingCoilId;
	uint8  reserved;	
    uint32 dwModulateFreq;
    uint32 dwWorkingFreq;
    uint32 dwWorkingDuty;
    uint32 dwWorkingPhase;
	uint16 wSendUnitBuf[FSK_MAX_TX_DATA_NUM];
	uint16 wSendUnit;
}FSK_CTRL_T;

typedef struct
{
    uint8   byBufferReadPointer;
    uint8   byBufferWritePointer;
    uint16  wTimeBetweenActiveEdges[ COMM_EDGE_BUFFER_SIZE ];
	uint16  wTimeBetweenOppositeEdges[ COMM_EDGE_BUFFER_SIZE ];
    uint16  wRefTimeBetweenActiveEdges[ COMM_EDGE_BUFFER_SIZE ];
} RF_DATA_INTERRUPT_PARAMS;

typedef struct
{
    uint16 bRfDataRxBitTimeCentered   : 1;
    uint16 bLookingForFirstOddBitTimeOccurrence    : 1;
    uint16 bUnused1                   : 1;
    uint16 bUnused2                   : 1;
    uint16 bUnused3                   : 1;
    uint16 bUnused4                   : 1;
    uint16 bUnused5                   : 1;
    uint16 bUnused6                   : 1;
}COMM_FLAGS;

typedef struct
{
    RF_DATA_RX_STATE            eRfDataRxState;
    uint16                      wRfDataRxTransitionLastCounter;
    uint16                      wRfDataTimerCalibrationValue;
    uint16                      wStartBitReferenceCount;
    uint16                      wStopBitReferenceCount;
    uint16                      wRfDataRxShiftRegister;
    uint8                       byRfDataRxShiftCount;
    uint8                       byRfDataRxTransitionBitAvail;
    uint8                       byRfDataRxTransitionBitAvailCount;
    uint8                       byRfPreambleCount;
    COMM_FLAGS                  btCommFlags;
    uint8                       byNumBytesInPacket;
    uint8                       byDataPacketBufferIndex;
    uint8                       byDataPacketBuffer[ MAX_PACKET_SIZE ];
}RF_DATA_PARAMS;

typedef struct
{
    uint8  bySignalStrength;
    uint8  byMajorVer;
    uint8  byMinorVer;
    uint8  byExtIDExist;

    uint16 wManuufactureID;
    uint16 wReceivedPower;
    
    uint32 dwWPIDHigh;
    uint32 dwWPIDLow;
    uint32 dwDeviceID;
    uint32 dwExtIDHigh;
    uint32 dwExtIDLow;
    
    uint16 wHighWPIDCRC;
    uint16 wLowWPIDCRC;
    
    uint8  byCfgWinSize;
    uint8  byCfgWinOffset;
    uint8  bycfgFSKParam;
    uint8  byCfgNeg;
    
    uint8  byCfgCount;
    uint8  byCfgPowerClass;
    uint8  byCfgPropCtrl; 
    uint8  byEptReasonCode;
    
    uint8  bySRRequest;
    uint8  bySRParameter;        
    uint16 wCfgMaxHalfWatts;
    
    uint8  byQMode;
	uint8  byQFactor;
	uint8  byPowerPacketMode;    
	uint8  byGRRequest;

    sint8  sbyError;
    uint8  byChargeStatus;
    uint8  byHoldOffTime;
    uint8  byMsgBuffer[MAX_PACKET_SIZE];
}PACKET_RX_INFO_T;


typedef struct
{
	uint16  wHysCnt;
	uint16  wHysCntThre;
	uint16  wFallingEdgeCnt;
	uint16  wRisingEdgeCnt;	
	sint32  sdwMin;
	sint32  sdwMax;
}DDM_CH_PARAM;

typedef struct
{
	//Device control params
	sint16  *pswBufFir;
	sint16  *pswBufSec;
	sint16  *pswBufThir;
#ifdef _LDM_
	uint16   wReserved;
#endif
	uint16   wHalfBitPoint;
	uint16   wRefConut;
	uint16   wMiscCtrl;
	uint16   wTimePerSample;
	uint16   wRefCountPerSample;
	sint32   sdwWinSum0;
	sint32   sdwWinSum1;
	sint32   sdwMidValue0;
	sint32   sdwMidValue1;
	sint32   sdwValueThre;
	//Channel control params
	DDM_CH_PARAM tChCtrl[NUM_COMM_CHANNELS_PER_DEVICE];
}WPC_DDM_CTRL_T;

typedef boolean (*FODFUNC)(uint8 byDeviceId, uint32 dwTxInputPower, sint16 swTxInternalLoss, uint32 dwRxReportedPower, sint32 sdwCalibratedTransmitPower);

typedef struct
{
	uint32 bAnalogPingDisable :1;        //1: analog ping disabled;  0: analog ping enabled
	uint32 bFODEnable         :1;        //FOD feature enable/disable
	uint32 bRRQDEnable        :1;        //Rx remove quick deteciton enable/disable
	uint32 bSafeDPEnable      :1;        //Safe digital ping enable/disable
	uint32 bMVLEnable         :1;        //Max voltage limit enable/disable
	uint32 bFSKAppendHalfBit  :1;        //Add explicit half bit at end of FSK modulation waveform, enable/disable
	uint32 byKeyfobAvoidanceDisableCoil : 1;  //0: continue charging when keyFOB;   1: stop charging when KeyFOB 
	uint32 bFastChargingEnable :1;       //fast charging enable/disable
	uint32 bQfactorRetry      :1;        //using Q factor method for recharge retry
	uint32 bMPLEnable         :1;        //maximum transmitted power limit
	uint32 bRechargeRetryLowPowerEnable  :1;  //enable low power mode for recharge retry
	uint32 bActivePowerProtectionEnable  :1;  //Enable active power protection
	uint32 bPowerDownForMaxVrail :1;          //Enable power down when Vrail reaches its maximum voltage
	uint32 bNotResSens        :1;        //"not res sens" bit for Tx capability packet
	uint32 bBPPOnly           :1;        //Tx behaves as BPP transmitter
	uint32 bPreFODEnable      :1;        //Enable pre FOD protection
	uint32 bEnableDutyCycleControlForMinRailVol: 1;  //if set 1, when vRail down to gWCT_Params.wMinRailVoltageMv and 
	                                                 //Rx still asks to decrease the power, Tx will work in duty mode
	uint32 bRMVLEnable         :1;        //RX Max voltage limit through coil current limit enable/disable
	uint32 bReserved          :14; 
} WPC_CTRL;

typedef struct
{
    //debug log on-off
    uint16 bGeneralDbg        : 1;
    uint16 bStateMachineDbg   : 1;
    uint16 bCommPacketDbg     : 1;
    uint16 bPidDbg            : 1;
    uint16 bNegoDbg           : 1;
    uint16 bPowerlossDbg      : 1;
    uint16 bKeepLastAbnormDbg : 1;
    uint16 reserved           : 9;
} WCT_DBG_CONFIG;

typedef struct
{
	//General Configure
    uint8    byDeviceNum;                //Device number
    uint8    byCoilNumPerDevice;         //coil number for each device
	uint16   wManufacturerCode;	         //manufacture code
	uint16   wTxMaxPowerHalfWatts;       //Combine with byTxPowerClass
	uint16   wDeviceEnableFlag;	         //Enable device, one bit for one device, say bit 0 is for device 0.
	WPC_CTRL uCtrlBit;
    uint8    byTxPowerClass;             //transmit power class, the maximum transmit power = wTxMaxPowerHalfWatts * 10^byTxPowerClass / 2
    uint8    byChargingTryNumOnOneCoilThreshold;  //Tx enters failure protection mode if continues charging failure(Rx remove, FOD, etc.) number
                                                  //is greater than byChargingTryNumOnOneCoilThreshold. Set it to 255 to disable this feature
    uint8    byRxRemovedConfirmDPNum;    //Tx consider Rx is removed after byRxRemovedConfirmDPNum digital ping can't get SS packet, when Tx in fault waiting status 
	
	
    //Analog Ping Configuration
	uint8    byAnalogPingDetectAbsoluteValue;    //Analog ping trigger absolute change threshold. 
	uint8    byAnalogPingDetectThresholdPercent; //Analog ping trigger percent change threshold.
	                                             //Note: above two conditions should be both satisfied for analog ping to trigger digital ping
    
	//Timing Configuration
	uint8    byDigitalPingRetryInterval;         //in unit of 0.1 second. defines the interval of forced digital ping.
	uint16   wPingInterval;                      //in unit of ms. interval for each round digital ping
	uint16   wAnalogPingInterval;                //in unit of ms. interval of analog ping for each coil in one round analog ping
	uint16   wDigitalPingInterval;               //in unit of ms. interval of digital ping for each coil in one round digital ping
	uint16   wDigitalPingDuration;               //in unit of ms. Maximum digital ping duration.
	uint16   wNextPacketTimeOut;                 //in unit of ms. Timeout value for next packet in DP/ID/CFG phase.
	uint16   wFirstPacketDuration;               //in unit of ms. First packet maximum duration.
	uint16   wMaxPacketDuration;                 //in unit of ms. Maximum packet duration.
	uint16   wRPPTimeOut;                        //in unit of ms. Received power packet timeout.
	uint16   wCEPTimeOut;                        //in unit of ms. Control error packet timeout.
	uint16   wMsgHeaderTimeOut;                  //in unit of ms. Packet timeout.
	uint16   wTimeForWaitNextNegotiationPacket;  //in unit of ms. Packet timeout in negotiation phase.
	uint16   wQPrepareInterval;                  //in unit of ms. Interval between one round digital ping and QF_QMeasurePrepare
	uint16   wQMeasureInterval;                  //in unit of ms. Interval between each QF_QMeasure
	uint16   wRailSetupTime;                     //in unit of ms. Vrail setup time afte enabled.
	uint16   wAnalogPingPowerSetupTime;          //in unit of ms. Power rail for analog ping setup time after enabled.
	uint16   wRailDischargeTime;                 //in unit of ms. Vrail discharge time.
	uint16   wDDMStartDelayTimeAfterCharging;    //in unit of ms. Delay time for DDM start after charging started.
	uint16   wDDMRetryTimeout;                   //in unit of ms. DDM retry timeout value.
	uint16   wSendFSKDelay;                      //in unit of ms. Delay time to send FSK.
	uint16   wCalibrationTimeout;	             //in unit of ms. Timeout value in calibration phase.
	
    
    //Inverter Params
	uint16   wDefaultRailVoltageMv[MAX_COIL_NUM_PER_DEVICE];   //Default digital ping Vrail voltage for each coil
	uint16   wDigitalPingDuty;                                 //digital ping duty cycle.
	uint16   wDigitalPingPhase;                                //digital ping phase
    uint16   wMaxDuty;                                         //maximum duty cycle
    uint16   wMinDuty;                                         //minimum duty cycle
    uint16   wMaxPhase;                                        //maximum phase shift
    uint16   wMinPhase;                                        //minimum phase shift
    uint16   wMaxRailVoltageMv;                                //maximum Vrail voltage
    uint16   wMinRailVoltageMv;                                //minimum Vrail voltage
    uint32   dwDigitalPingFreq;                                //digital ping frequency
    uint32   dwMaxFreq;                                        //maximum frequency
    uint32   dwMinFreq;                                        //minimum frequency
    uint32   dwFobAvoidFreqency;                               //frequency for keyFOB
    uint8    byDigitalPinglBridgeType;                         //inverter bridge type. 1: full bridge; 0: half bridge 
    
    //FOD
    uint8    byNumFodTripsToIndication;                        //Default trips number for trigger FOD protection
    uint16   wBPPLPPowerLossThresholdInOperationMode;             //Default power loss threshold for LP Rx
    uint16   wEPPMPPowerLossThresholdInOperationMode;             //Default power loss threshold for MP Rx
    uint16   wEPPLPPowerLossThresholdInOperationMode;		   //Default power loss threshold for EPP 5W
    uint16   wPowerLossThresholdInCalibLightMode;              //Power loss threshold in calibration light mode
    uint16   wPowerLossThresholdInCalibConnectMode;            //Power loss threshold in calibraiton connect mode
    
    uint16   wPowerLossIndicationToPwrCessationMs;             //in unit of ms. Wait time before entering FOD protection
    uint16   wPowerLossThresholdForLegacyRx;                   //Power loss threshold for legacy receiver(V1.0), for rough FOD protection
    uint8    byDefaultWindowSize;                              //default window size
    uint8    byQfactorThresholdPercent;                        //Q factor threshold, in unit of percent
    uint8    byQfactorAdjsutPercent[MAX_COIL_NUM_PER_DEVICE];                           //Adjust value for the measure Q factor, in unit of percent
    uint8    byEffiThresholdPercentForLegacyRx;                //Efficiency threshold of legacy receivers(V1.0), for rough FOD protection
    FODFUNC  pFodExternalCheck;                                //External FOD function pointer. If none, Tx use internal default FOD algorithm
    
    //Calibration
    sint8    sbyMaxErrorForLightMode;
    sint8    sbyMinErrorForLightMode;
    sint8    sbyMaxErrorForConnectMode;
    sint8    sbyMinErrorForConnectMode;
    uint16   wLightModeMaxRecvPwrThreshPercent;                //in unit of percent, based on the negotiated maximum power
    uint16   wConnectModeMaxRecvPwrThreshPercent;              //in unit of percent, based on the negotiated maximum power
    uint16   wPowerDiffThresholdBetweenCalibrationLightAndConnect;  //Received power difference threshold for light and connect mode.
                                                                    //If less, Tx won't perform calibration algorithm.
    
    //PID
    uint8    byNumPidAdjustmentsPerActiveWindow;
    uint8    byIntervalBetweenPidAdjust;
    uint16   wOverCurrentLimitMa;
    uint16   wRailStepMv;
    uint16   wRailPidScaleFactor;
    uint16   wIntegralUpdateInterval;
    uint16   wDerivativeUpdateInterval;
    sint16   swIntegralUpperLimit;
    sint16   swIntegralLowerLimit;
    sint16   swPidUpperLimit;
    sint16   swPidLowerLimit;
    uint8    byDefaultPidHoldTimeMs;
	uint8    byMaxPidHoldTimeMs;
    uint8    byActiveTimeMs;
    uint8    bySettleTimeMs;
    uint8    byRailKp;             //PID parameters
    uint8    byRailKi;
    uint8    byRailKd;
    
    uint16   wDutyStep;
    uint16   wDutyPidScaleFactor;
    uint8    byDutyKp;
    uint8    byDutyKi;
    uint8    byDutyKd;
    
    //DDM
    uint8    byDDMThreshold;               //Modulation amplitude threshold, 1 means 1 ADC count
    uint8    byMaxPreambleDuty;            //Preamble high/low duty
    uint8    byMinPreambleDuty;            //Preamble high/low duty
    uint8    byMaxPreambleCount;
    uint8    byMinPreambleCount;           //Minumum preamble number
    uint16   wCommunicationRate;           //Communication bit rate
    uint8    wCommunicationRateTolerance;  //bit rate tolerance, in unit of percentage
    uint8    wCommunicationFailISRCount;
    
	//Rx remove quick detection
	uint16   wRRQDInputCurrentAbsoluteThreshold;
	uint16   wRRQDInputCurrentPercentThreshold;
	uint16   wRRQDCoilCurrentAbsoluteThreshold;
	uint16   wRRQDCoilCurrentPercentThreshold;
    
    //DBG
    WCT_DBG_CONFIG tDebugConfig;
    
    //MVL
    uint16   wMaxVolForLpPowerRx;
    uint16   wMaxVolForMpPowerRx;
    uint16   wLowLoadingThreshold;
    uint16   wHeavyLoadingThreshold;
    uint16   wMinPowerFactorForHeavyLoading;
    
    //reference timer
    uint16   wMaxDigitalPingTimeRefCounts;
	uint16   wFirstPacketTimeoutRefCounts;
	uint16   wNextPacketTimeoutRefCounts;
    uint32   dwCommReferenceTimerFreq;
    uint32   dwCommReferenceTimerMaxCount;
	
	//Safe digital ping 
    uint32   dwSafeDigitalPingFreq;
	uint16   wSafeDigitalPingCheckTime;	   //Safe digital ping duration
	uint16   wSafeDigitalPingDuty;
	uint16   wSafeDigitalPingPhase;
    uint8    bySafeDigitalPinglBridgeType;
    
    //Q factor retry
    uint16   wQfactorChangeThreshold;
    
    //Q factor stability
    uint8    byQfStableThreshold;
    uint8    byQfMeasureNum;
    
    //fast charge
    uint8    byFCSSThreshold;
    uint8    byFCIncPercentForLowSS;
    
    //active power protection
    uint16   wAPPDumpPowerAbsoluteThreshold;
    uint8    byAPPRollBackWin;
    uint8    byAPPDumpPowerPercentageThreshold;
    uint8    byAPPVolDumpScale;   //percentage
    uint8    byMaxRxReportedQFactor;
    uint8    byQfAveNumForRetry;
    uint8    byMinTxMeasuredQfToStopRetry;
    
    //MPL
    //for general Rx, MPL limit its maximum transmitted power = Rx guaranteed power * wMPLRxCoeffThre + wMPLRxOffsetThre
    uint16   wMPLHysteresis;       //hysteresis value to exit MPL state, general value
    uint16   wMPLRxOffsetThre;     //Rx static power consumption, general value
    uint16   wMPLRxCoeffThre;      //efficiency threshold for Rx,  = input/output, general value
	
	uint16   wPreFODQfThreshold;
    uint16   wPreFODTryNum;
    
    uint16   wCoilCurrentThresholdForLowLoading[MAX_COIL_NUM_PER_DEVICE];
}WCT_PARAM_T;

typedef struct
{
    uint8 bMajorVersion;
    uint8 bMinorVersion;
    uint8 bSubVersion;
}LIB_Version;

typedef struct
{
	sint16 swQuadCoefficient;
	uint16 wQuadExponent;
	sint16 swLinearCoefficient;
	uint16 wLinearExponent;
	sint16 swConstantCoefficient;
	sint16 swPowerLossCalibrationOffset;
} FOD_CHARACTERIZATION_PARAMS;

typedef struct
{
	sint16 swQuadCoefficient;
	uint16 wQuadExponent;
	sint16 swLinearCoefficient;
	uint16 wLinearExponent;
	sint16 swConstantCoefficient;
	uint16 wReserved;
} FOD_NORMALIZATION_PARAMS;

extern  WCT_PARAM_T gWCT_Params;
extern WPC_DDM_CTRL_T gWpcDDMCtrlParams[];
extern RF_DATA_PARAMS eRfDataParams[][NUM_COMM_CHANNELS_PER_DEVICE];
extern RF_DATA_INTERRUPT_PARAMS eRfDataInterruptParams[][NUM_COMM_CHANNELS_PER_DEVICE];
extern PACKET_RX_INFO_T gPKT_RxPktInfo[];
extern FSK_CTRL_T gFSK_Ctrl[];
extern RUNTIME_PARAMS gRunTimeParams[];
extern uint32 gDCD_PktChCnt[][NUM_COMM_CHANNELS_PER_DEVICE];

void WCT_LibInit(void);
uint16 WCT_Run( uint16 wTimePassedMs );
void WCT_Stop(void);
void WCT_ChargeSpecificCoil(uint8 byDeviceId, uint8 byCoilId, CHARGING_TYPE ChargeType);
CHARGING_TYPE WCT_GetChargingType(uint8 byDeviceId);
TX_CHARGING_STATUS WCT_GetTxStatus(uint8 byDeviceId);
TX_CHARGING_ERRORS WCT_GetTxError(uint8 byDeviceId);
RX_CHARGING_STATUS WCT_GetRxStatus(uint8 byDeviceId);
RX_CHARGING_ERRORS WCT_GetRxError(uint8 byDeviceId);
void WCT_CommAnalyse(uint8 byDeviceId);
void WCT_GetLibVer(LIB_Version *pLibVersion);
boolean FSK_IsBusy(uint8 byDeviceId);
void FSK_ISR(uint8 byDeviceId);

#endif
