/**
 *   @file  qmss_acc.h
 *
 *   @brief   
 *      This is the Queue Manager accumulator header file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2016, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/


#ifndef QMSS_ACC_H_
#define QMSS_ACC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* QMSS LLD includes */
/* The ifndef QMSS_DRV_H avoids warning from static analysis tool
 * for include recursion, if qmss_acc is included normally via
 * qmss_drv.h.  Not deleting code to avoid breaking backwards compat 
 * for those who use qmss_acc.h directly.
 */
#ifndef QMSS_DRV_H_
#include <ti/drv/qmss/qmss_drv.h>
#endif

/**
@addtogroup QMSS_LLD_SYMBOL
@{
*/

/** QMSS accumulator return and Error Codes */
/** QMSS accumulator idle return code */
#define QMSS_ACC_IDLE                               0
/** QMSS accumulator successful return code */
#define QMSS_ACC_SOK                                1
/** QMSS accumulator invalid command return code */
#define QMSS_ACC_INVALID_COMMAND                    2
/** QMSS accumulator invalid channel return code */
#define QMSS_ACC_INVALID_CHANNEL                    3
/** QMSS accumulator channel not active return code */
#define QMSS_ACC_CHANNEL_NOT_ACTIVE                 4
/** QMSS accumulator channel already active */
#define QMSS_ACC_CHANNEL_ALREADY_ACTIVE             5
/** QMSS accumulator invalid queue number */
#define QMSS_ACC_INVALID_QUEUE_NUMBER               6
/** QMSS accumulator invalid entry count */
#define QMSS_ACC_INVALID_MAXENTRIES                 7

/**
@}
*/

/**
@addtogroup QMSS_LLD_ENUM
@{
*/

/** 
 * @brief Accumulator channel handle
 */
typedef int32_t Qmss_AccChHandle;

/** 
 * @brief Accumulator channel commands
 */
typedef enum
{
    /** Accumulator command to disable channel */
    Qmss_AccCmd_DISABLE_CHANNEL = 0x80,
    /** Accumulator command to enable channel */
    Qmss_AccCmd_ENABLE_CHANNEL = 0x81,
    /** Accumulator command to configure timer constant */
    Qmss_AccCmd_CONFIG_TIMER_CONSTANT = 0x82,
    /** Accumulator command to configure reclamation queue */
    Qmss_AccCmd_CONFIG_RECLAIM_QUEUE = 0x83,
    /** Accumulator command to configure diversion queue */
    Qmss_AccCmd_CONFIG_DIVERSION_QUEUE = 0x84,
    /** Accumulator command to configure diversion queue */
    Qmss_AccCmd_CONFIG_DDR_BARRIER_QUEUE = 0x85,
    /** Accumulator command to configure diversion queue */
    Qmss_AccCmd_CONFIG_MSMC_BARRIER_QUEUE = 0x86
}Qmss_AccCmdType;

/** 
 * @brief Accumulator configuration - interrupt pacing mode
 */
typedef enum
{
    /** Interrupt on entry threshold count only */
    Qmss_AccPacingMode_NONE = 0,
    /** Time delay since last interrupt */
    Qmss_AccPacingMode_LAST_INTERRUPT,
    /** Time delay since first new packet */
    Qmss_AccPacingMode_FIRST_NEW_PACKET,
    /** Time delay since last new packet */
    Qmss_AccPacingMode_LAST_NEW_PACKET
}Qmss_AccPacingMode;

/** 
 * @brief Accumulator configuration - list entry size
 */
typedef enum
{
    /** 'D' register only (4 byte entries)
     * Word 0 : Packet Descriptor Pointer 
     */
    Qmss_AccEntrySize_REG_D = 0,
    /** 'C,D' registers (8 byte entries) 
     * Word 0 : Packet Length (as reported by queue manager)
     * Word 1 : Packet Descriptor Pointer
     */
    Qmss_AccEntrySize_REG_CD,
    /** 'A,B,C,D' registers (16 byte entries) 
     * Word 0 : Packet Count on Queue (when read)
     * Word 1 : Byte Count on Queue (when read)
     * Word 2 : Packet Length (as reported by queue manager)
     * Word 3 : Packet Descriptor Pointer
     */
    Qmss_AccEntrySize_REG_ABCD
}Qmss_AccEntrySize;

/** 
 * @brief Accumulator configuration - list count mode
 */
typedef enum
{
    /** NULL Terminate Mode - The last list entry is used to store a NULL pointer 
     * record (NULL terminator) to mark the end of list. In either case there is room for one less 
     * list entry in a page than is actually specified by the host.
     */
    Qmss_AccCountMode_NULL_TERMINATE = 0,
    /** Entry Count Mode - The first list entry is used to store the total list entry 
     * count (not including the length entry). 
     */
    Qmss_AccCountMode_ENTRY_COUNT
}Qmss_AccCountMode;


/** 
 * @brief Accumulator configuration - Multi-Queue Mode
 */
typedef enum
{
    /** Single Queue Mode - The channel monitors a single queue. */
    Qmss_AccQueueMode_SINGLE_QUEUE = 0,
    /** Multi-Queue Mode - The channel monitors up to 32 queues starting at the supplied base queue index. */
    Qmss_AccQueueMode_MULTI_QUEUE
}Qmss_AccQueueMode;

/**
@}
*/

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/

/** 
 * @brief Accumulator Command interface structure
 */
typedef struct
{
    /** Accumulator channel affected (0-47) */
    uint8_t               channel;
    /** Accumulator channel command - Qmss_AccCmd_ENABLE_CHANNEL : Enable channel 
     * Qmss_AccCmd_DISABLE_CHANNEL : Disable channel */
    Qmss_AccCmdType     command;
    /** This field specifies which queues are to be included in the queue group. 
     * Bit 0 corresponds to the base queue index, and bit 31 corresponds to the base 
     * queue index plus 31. For any bit set in this mask, the corresponding queue index 
     * is included in the monitoring function.
     *
     * This field is ignored in single-queue mode.*/
    uint32_t              queueEnMask;
    /** Physical pointer to list ping/pong buffer. NULL when channel disabled */
    uint32_t              listAddress;
    /** Queue Manager and Queue Number index to monitor. This serves as a base queue index when the 
     * channel in multi-queue mode, and must be a multiple of 32 when multi-queue mode is enabled. */
    uint16_t              queMgrIndex;
    /** Max entries per list buffer page */
    uint16_t              maxPageEntries;
    /** Number of timer ticks to delay interrupt */
    uint16_t              timerLoadCount;
    /** Interrupt pacing mode. Specifies when the interrupt should be trigerred */
    Qmss_AccPacingMode  interruptPacingMode;
    /** List entry size. Specifies the size of each data entry */
    Qmss_AccEntrySize   listEntrySize;
    /** List count Mode. The number of entries in the list */
    Qmss_AccCountMode   listCountMode; 
    /** Queue mode. Moitor single or multiple queues */
    Qmss_AccQueueMode   multiQueueMode;
} Qmss_AccCmdCfg;

/** 
@} 
*/

/* Internal data structure to write accumulator command */
typedef struct
{
    /* Channel, Command */
    uint32_t              word0;
    /* Queue enable mask */
    uint32_t              word1; 
    /* List buffer physical address */
    uint32_t              word2; 
    /* Queue manager and base index, Maximum page entries */
    uint32_t              word3; 
    /* Timer Load Count, Configuration */
    uint32_t              word4; 
} Qmss_AccCmd;

/* Exported functions */
extern Qmss_AccChHandle Qmss_openAccumulatorChSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, uint8_t channel);
extern Qmss_AccChHandle Qmss_openAccumulatorCh (Qmss_PdspId pdspId, uint8_t channel);
extern Qmss_Result      Qmss_cfgAccumulatorCh (Qmss_AccChHandle handle, Qmss_AccCmdCfg *cfg);
extern Qmss_Result Qmss_programAccumulatorSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, Qmss_AccCmdCfg *cfg);
extern Qmss_Result Qmss_programAccumulator (Qmss_PdspId pdspId, Qmss_AccCmdCfg *cfg);

extern Qmss_Result Qmss_stopAccumulatorCh (Qmss_AccChHandle handle);
extern Qmss_Result Qmss_closeAccumulatorCh (Qmss_AccChHandle handle);
extern Qmss_Result Qmss_disableAccumulator (Qmss_PdspId pdspId, uint8_t channel);
extern Qmss_Result Qmss_disableAccumulatorSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, uint8_t channel);
extern Qmss_Result Qmss_configureAccTimerSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, uint16_t timerConstant);
extern Qmss_Result Qmss_configureAccTimer (Qmss_PdspId pdspId, uint16_t timerConstant);
extern Qmss_Result Qmss_programReclaimQueueSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, Qmss_QueueHnd hnd);
extern Qmss_Result Qmss_programReclaimQueue (Qmss_PdspId pdspId, Qmss_QueueHnd hnd);
extern Qmss_Result Qmss_programDiversionQueueSubSys (Qmss_SubSysHnd subSysHnd, Qmss_PdspId pdspId, Qmss_QueueHnd divQ,
                                                     Qmss_QueueHnd divCompletionQ);
extern Qmss_Result Qmss_programDiversionQueue (Qmss_PdspId pdspId, Qmss_QueueHnd divQ,
                                               Qmss_QueueHnd divCompletionQ);

/* The following APIs only work on K2 devices, but no #ifdef to avoid
 * new DEVICE/SOC define requirement for applications */
Qmss_Result Qmss_programDDRBarrierQueueSubSys (Qmss_SubSysHnd subSysHnd,
                                               Qmss_PdspId pdspId, 
                                               Qmss_QueueHnd ddrInfBarrierQ,
                                               Qmss_QueueHnd ddrNetCPBarrierQ);
Qmss_Result Qmss_programDDRBarrierQueue (Qmss_PdspId pdspId, 
                                         Qmss_QueueHnd ddrInfBarrierQ,
                                         Qmss_QueueHnd ddrNetCPBarrierQ);
Qmss_Result Qmss_programMSMCBarrierQueueSubSys (Qmss_SubSysHnd subSysHnd,
                                                Qmss_PdspId pdspId, 
                                                Qmss_QueueHnd msmcInfBarrierQ,
                                                Qmss_QueueHnd msmcNetCPBarrierQ);
Qmss_Result Qmss_programMSMCBarrierQueue (Qmss_PdspId pdspId, 
                                          Qmss_QueueHnd msmcInfBarrierQ,
                                          Qmss_QueueHnd msmcNetCPBarrierQ);

#ifdef __cplusplus
}
#endif

#endif /* QMSS_ACC_H_ */

