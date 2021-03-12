/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2016~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __PD_TIMER_H__
#define __PD_TIMER_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define mSec(MSEC) (MSEC)

typedef enum tTimer
{
    _tStartTimer = 0,
    tSenderResponseTimer = 0,
    tSourceCapabilityTimer = 1,
    tSinkWaitCapTimer = 2,
    tSinkRequestTimer = 3,
    tDrSwapWaitTimer = 4,
    tPrSwapWaitTimer = 5,
    tPSTransitionTimer = 6,
    tPSSourceOffTimer = 7,
    tPSSourceOnTimer = 8,
    tNoResponseTimer = 9, /* The NoResponseTimer is used by the Policy Engine in a Source or Sink to determine that its
                            Port Partner is not responding after a Hard Reset. */
    tSwapSourceStartTimer = 10, /* pr swap */
    tPSHardResetTimer = 11,     /* source hard_reset */
    tVconnOnTimer = 12,         /* vconn swap */
    tVDMResponseTimer = 13,
    tVDMModeEntryTimer = 14,
    tVDMModeExitTimer = 15,
    tSinkTxTimer = 16,

    tSrcRecoverTimer = 17,           /* hard_reset tSrcRecover */
    tMsgHardResetCompleteTimer = 18, /* MSG */
    tSinkPPSPeriodicTimer = 19,
    tSourcePPSCommTimer = 20,
    _tMaxPSMTimer = tMsgHardResetCompleteTimer,    /* The maximum PSM timer */
    tAMETimeoutTimer = 21,                         /* T_AME_TIMEOUT */
    _tMaxDpmTimer = tAMETimeoutTimer,              /* The maximum port timer */
    tPDDebounceTimer = 22,                         /* TypeC */
    tPDDebounce2Timer = 23,                        /* TypeC */
    tCCDebounceTimer = 24,                         /* TypeC */
    tCCDebounceTimer1 = 25,                        /* TypeC */
    tCCDebounceTimer2 = 26,                        /* TypeC */
    tDRPToggleTimer = 27,                          /* TypeC */
    tTypeCVbusMinDischargeTimer = 28,              /* TypeC */
    tTypeCVbusMaxDischargeTimer = 29,              /* TypeC */
    _tMaxTypeCTimer = tTypeCVbusMaxDischargeTimer, /* The maximum TypeC timer */
    tVDMBusyTimer = 30,
    tDelayTimer = 31, /* tSrcRecover, T_SRC_TRANSITION (DPM), T_ERROR_RECOVERY (CONNECT), */
    timrCCFilter = 32,

    tDRPTryWaitTimer = 33,
    tDRPTryTimer = 34,
    tBISTContModeTimer = 35,
    tFRSwapSignalTimer = 36,
    tVBUSNotStableTimer = 37,
    tTimerCount = 38,
    _tEnd = tTimerCount - 1

/* don't implement yet */
#if 0
    tUFPDebounceTimer = 35,
    tUSBSuspendTimer = 36, /* USB */
    tUSBTimer = 37,        /* USB */
    tHciSpDebounceTimer = 38,
    tHostifWakeupTimer = 39,
#endif
} tTimer_t;

#define PD_MAX_TIMER_COUNT (38)

#define T_PPS_TIMEOUT mSec(15000)       /* 15s */
#define T_PPS_REQUEST mSec(10000)       /* 10s */
#define T_HARD_RESET_COMPLETE mSec(5)   /* 5 maximum */
#define T_UFP_CONNECT_DEBOUNCE mSec(15) /* 15 maximum */
#define T_SWAP_SOURCE_START mSec(25)    /* 20 minimum, so we choose 25 to be safe. */
#define T_SRC_TRANSITION mSec(30)       /* 25 to 35, so we choose 30 to be safe. */
#define T_SENDER_RESPONSE mSec(23)      /* 24 to 30, so we choose 25 to be safe. */
#define T_VDM_SENDER_RESPONSE mSec(25)  /* 24 to 30, so we choose 25 to be safe. */
#define T_PS_HARD_RESET mSec(26)        /* 25 to 35, so we choose 26 to be safe. */
#define T_ERROR_RECOVERY mSec(30)       /* 25 minimum, so we choose 30 to be safe. */
#define T_BIST_CONT_MODE mSec(45)       /* 30 to 60, so we choose 45 to be safe. */
#define T_VDM_WAIT_MODE_ENTRY mSec(45)  /* 40 to 50, so we choose 45 to be safe. */
#define T_VDM_WAIT_MODE_EXIT mSec(45)   /* 40 to 50, so we choose 45 to be safe. */
#define T_VDM_BUSY mSec(55)             /* 50 minumum */
#define T_VCONN_SOURCE_ON mSec(100)     /* 100 maximum */
#define T_VCONN_PA_SOURCE_ON mSec(100)  /* 100 maximum */
#define T_SINK_REQUEST mSec(100)        /* 100 to no maximum */
#define T_DRSWAP_WAIT mSec(100)         /* 100 to no maximum */
#define T_PRSWAP_WAIT mSec(100)         /* 100 to no maximum */
#define T_PSM_RDY_EVAL_DELAY mSec(120)  /* Arbitary - not a USBPD timer */
#if (defined PD_CONFIG_TRY_SRC_SUPPORT) && (PD_CONFIG_TRY_SRC_SUPPORT)
#define T_SINK_WAIT_CAP mSec(310) /* 310 to 620 */
#else
#define T_SINK_WAIT_CAP mSec(400) /* 310 to 620 */
#endif
#define T_PS_SOURCE_ON mSec(400)        /* 390 to 480 */
#define T_PS_TRANSITION mSec(500)       /* 450 to 550 */
#define T_SWAP_RECOVER mSec(500)        /* 500 to 1000 */
#define T_SRC_RECOVER mSec(700)         /* 660 to 1000 */
#define T_PS_SOURCE_OFF mSec(800)       /* 750 to 920 */
#define T_AME_TIMEOUT mSec(900)         /* 1000 maximum */
#define T_HARD_RESET_MAX mSec(2000)     /* tSafe0V (max) + tSrcRecover (max) + tSrcTurnOn (max) */
#define T_NO_RESPONSE mSec(5000)        /* 4500 to 5500 */
#define T_HOSTIF_WAKEUP mSec(150)       /* 4500 to 5500 */
#define T_USBFET2_OVERLAP mSec(30)      /* */
#define T_USBFET_TRANSITION mSec(1)     /* */
#define T_AUTO_PR_SWAP_TEST mSec(45000) /* */

#if defined(COSIM_TIMERS)

#define T_VCONN_STABLE mSec(5)     /* 50ms minimum */
#define T_SEND_SOURCE_CAP mSec(75) /* 100 to 200 */
#define T_SRC_NEW_POWER mSec(2)    /* Temporary value to keep tests passing */

#else

#define T_VCONN_STABLE mSec(50)     /* 50ms minimum */
#define T_SEND_SOURCE_CAP mSec(150) /* 100 to 200 */
#if defined(USBPD_PS_RDY_DELAY)
#define T_SRC_NEW_POWER USBPD_PS_RDY_DELAY /* 285 ms max */
#else
#define T_SRC_NEW_POWER mSec(80) /* 285 ms max */
#endif

#endif

#define T_MIN_VBUS_DISCHARGE mSec(14)  /* corrosponds to vbus_present debounce time (tccDebounce) */
#define T_MAX_VBUS_DISCHARGE mSec(650) /* tVBUSOFF */

#define T_SINK_TX mSec(20) /* 20ms max */

#define T_CC_FILTER_MAX mSec(1)

#define T_VBUS_NOT_STABLE (3)

/*******************************************************************************
 * API
 ******************************************************************************/

void PD_TimerInit(pd_handle pdHandle);
uint8_t PD_TimerCheckInvalidOrTimeOut(pd_handle pdHandle, uint8_t timrName);
uint8_t PD_TimerCheckValidTimeOut(pd_handle pdHandle, uint8_t timrName);
pd_status_t PD_TimerStart(pd_handle pdHandle, uint8_t timrName, uint16_t timrTime);
pd_status_t PD_TimerClear(pd_handle pdHandle, uint8_t timrName);
void PD_TimerCancelAllTimers(pd_handle pdHandle, uint8_t begin, uint8_t end);

#endif
