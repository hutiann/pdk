/**
 * \file  omapl137/cppi41dma.c
 *
 * \brief This file contains the API definitions for initializing and
 *        configuring CPPI DMA engine.
 */

/*
* Copyright (C) 2017-2019 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
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
*/

/* HW Macros and Peripheral Defines */
#include "types.h"
#include "hw_types.h"
#include "hw_usb.h"
#include "debug.h"
#include "cppi41dma.h"
/* Driver APIs */
#include "musb.h"
#include "usb_osal.h"

/*****************************************************************************
**                 STATIC FUNCTIONs
******************************************************************************/

static void Cppi41DmaModeSet(uint16_t usbDevInst, endpointInfo *epInfo);
static void Cppi41DmaInitBdPool();
static uint32_t Cppi41DmaReadCompletionQueue(uint16_t usbDevInst,
													uint32_t queueNum);

static void Cppi41DmaConfigRxhostPacket(uint16_t usbDevInst, uint16_t chan,
										uint32_t rxSubmitqA, uint32_t rxSubmitqB);

static void Cppi41DmaConfigRxCompletionQueue(uint16_t usbDevInst,
												uint16_t rxChan,uint32_t rxCompq);
static void Cppi41DmaConfigTxCompletionQueue(uint16_t usbDevInst,
												uint16_t txChan,uint32_t txCompq);

static void Cppi41DmaClearINTD0Status(uint16_t usbDevInst);

static void Cppi41DmaControlScheduler(uint16_t usbDevInst,
												uint16_t enableDisable,
													uint16_t numOfentries);

static void Cppi41DmaProcessBD( uint16_t usbDevInst, hostPacketDesc *current_bd,
											uint16_t direction, uint8_t * buffadd,
												uint32_t length, uint32_t ulEndpoint);
static void cppi41DmaInitBuffer();

uint32_t (*Cppi41DmaPopQueue)(uint16_t, uint32_t) =
                                            Cppi41DmaReadCompletionQueue;

static void Cppi41DmaPushToTearDownQ(uint16_t usbDevInst,
                            uint32_t  queueNum, cppi41TearDownDesc *curr_tdd);
static uint32_t Cppi41DmaPopFromSubmitQ(uint16_t usbDevInst, uint32_t  queueNum);


static void Cppi41DmaConfigTDDescQueue(uint16_t usbDevInst,uint32_t tdDescq);

void setTxTearDown(uint16_t usbDevInst, uint32_t indexEndPoint);
void setRxTearDown(uint16_t usbDevInst, uint32_t indexEndPoint);

uint32_t getQLength(uint16_t usbDevInst, uint32_t queueNum);
uint32_t Cppi41DmaGetQLength(uint16_t usbDevInst, uint32_t  queueNum);

static uint32_t Cppi41DmaRecoverBD( hostPacketDesc* completed_bd );

/*****************************************************************************
**                  VARIABLES AND BUFFERS
******************************************************************************/

/*Link RAM 0 area declaration */
#pragma DATA_ALIGN(linking_ram0, 32);
static uint32_t linking_ram0[LINK_RAM_SIZE];

/*Descriptor region 0 declaration */
#pragma DATA_ALIGN(region0DescriptorSpace, 32);
uint32_t region0DescriptorSpace[DESC_REGION_SIZE];

/*CPPI info structure */
cppi41DmaInfo cppiInfo;

/*Data Buffers for DMA data transfer */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=(32)
uint8_t dBuffer[CPDMA_NUMOF_BUFFERS][USB_PACKET_LENGTH];
#elif defined __TMS470__ || defined _TMS320C6X
#pragma DATA_ALIGN(dBuffer, 32);
uint8_t dBuffer[CPDMA_NUMOF_BUFFERS][USB_PACKET_LENGTH];
#else
uint8_t dBuffer[CPDMA_NUMOF_BUFFERS][USB_PACKET_LENGTH]__attribute__((aligned(32)));
#endif

/*Buffer management structre */
typedef struct
{
	uint32_t *dataBuffer;
	uint16_t usedFlag;
	uint16_t nBlocks;
}dmaDataBuffer;

#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=(32)
dmaDataBuffer dmaBuffer[CPDMA_NUMOF_BUFFERS];
#elif defined __TMS470__ || defined _TMS320C6X
#pragma DATA_ALIGN(dmaBuffer, 32);
dmaDataBuffer dmaBuffer[CPDMA_NUMOF_BUFFERS];
#else
dmaDataBuffer dmaBuffer[CPDMA_NUMOF_BUFFERS]__attribute__((aligned(32)));
#endif

/* Converts from an endpoint specifier to the offset of the endpoint's
 * control/status registers. */
#define EP_OFFSET(Endpoint)     (Endpoint - 0x10)
#define QUEUE_OFFSET(queue) 	(queue << 4)
#define CHANNEL_OFFSET(chan)	(chan << 5)

/*****************************************************************************
**                   FUNCTION DEFINITIONS
******************************************************************************/

/**
* \brief	  This API initializes the DMA. This API should be called by the application
*		  API .  API populates cppi Info structure according to the information
*		  passed from the application. This information is used by this API and all other
*		  APIs. This API configures Channels queues and schedulers as per the
*		  information passed from the application.
*
* \param	  usbDevIns :- This is the USB device instance
*
*\param	  epInfo:- End point info structure
*
*\param	  numOfEndPoimts:- Total Number of endpoints to be configured(RX+TX)
*
* \return	  None.
*
**/
void Cppi41DmaInit(uint16_t usbDevInst, endpointInfo *epInfo,
						uint16_t numOfEndPoimts)
{
	/* This function need to be changed for porting to different SoC ,
	 * to initialise base addresses and other details. */

	uint32_t descAddress = 0;
	uint16_t i= 0;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*Populate the CPPI info structure */
	cppiInfo.head_bd = NULL;
	cppiInfo.tail_bd = NULL;

	/*TX Completion queues */
	cppiInfo.txCompletionq[0]= TX_COMPQ1;
	cppiInfo.txCompletionq[1]= TX_COMPQ2;
	cppiInfo.txCompletionq[2]= TX_COMPQ1;
	cppiInfo.txCompletionq[3]= TX_COMPQ2;

	/*RX Completion queues */
	cppiInfo.rxCompletionq[0]= RX_COMPQ1;
	cppiInfo.rxCompletionq[1]= RX_COMPQ2;
	cppiInfo.rxCompletionq[2]= RX_COMPQ1;
	cppiInfo.rxCompletionq[3]= RX_COMPQ2;

	/*TX submit queues */
	cppiInfo.txSubmitq[0][0]=TX_SUBMITQ1;
	cppiInfo.txSubmitq[0][1]=TX_SUBMITQ2;
	cppiInfo.txSubmitq[1][0]=TX_SUBMITQ3;
	cppiInfo.txSubmitq[1][1]=TX_SUBMITQ4;
	cppiInfo.txSubmitq[2][0]=TX_SUBMITQ5;
	cppiInfo.txSubmitq[2][1]=TX_SUBMITQ6;
	cppiInfo.txSubmitq[3][0]=TX_SUBMITQ7;
	cppiInfo.txSubmitq[3][1]=TX_SUBMITQ8;

	/*USB base address and OTG basse adress  for USB instance 0 */
	cppiInfo.usbInst[0].usbBaseAddress = USB0_BASE;
	cppiInfo.usbInst[0].otgBaseAddress = USB_OTGBASE;

	/*This function initializes the buffer Management system */
	cppi41DmaInitBuffer();

	/* This loop populates the RX and TX queues(both submission and completion queues),
	 * channels and dma mode for each end point */
	for(i= 0;i< numOfEndPoimts;i++)
	{
		if((epInfo + i)->direction == CPDMA_DIR_RX)
		{
			usbInstance->rxEndPoint[(epInfo + i)->endPoint].channel =
												   (epInfo + i)->endPoint - 1;

			usbInstance->rxEndPoint[(epInfo + i)->endPoint].complettionq =
									 cppiInfo.rxCompletionq[(epInfo + i)->endPoint - 1];

			usbInstance->rxEndPoint[(epInfo + i)->endPoint].submitq =
													(epInfo + i)->endPoint - 1;

            usbInstance->rxEndPoint[(epInfo + i)->endPoint].tearDownq =
                                                        USB0_TEARDOWNQ1;

			usbInstance->rxEndPoint[(epInfo + i)->endPoint].mode =
														(epInfo + i)->dmaMode;
		}
		else
		{
			usbInstance->txEndPoint[(epInfo + i)->endPoint].channel =
													(epInfo + i)->endPoint - 1;
			usbInstance->txEndPoint[(epInfo + i)->endPoint].complettionq =
									cppiInfo.txCompletionq[(epInfo + i)->endPoint - 1];

			usbInstance->txEndPoint[(epInfo + i)->endPoint].submitq =
									 cppiInfo.txSubmitq[(epInfo + i)->endPoint - 1][0];

            usbInstance->txEndPoint[(epInfo + i)->endPoint].tearDownq =
                                                        USB0_TEARDOWNQ1;

			usbInstance->txEndPoint[(epInfo + i)->endPoint].mode =
														(epInfo + i)->dmaMode;
		}
	}

	/* Set  mode for each end pont*/
	for(i = 0;i < numOfEndPoimts; i++)
		Cppi41DmaModeSet(usbDevInst, &epInfo[i]);

	/*Link Ram setup	 */
	HWREG(usbInstance->otgBaseAddress + CPDMA_LRAM_0_BASE) = (uint32_t)&linking_ram0[0];
	HWREG(usbInstance->otgBaseAddress + CPDMA_LRAM_0_SIZE) = sizeof(linking_ram0);
	HWREG(usbInstance->otgBaseAddress + CPDMA_LRAM_1_BASE) = 0x0;

	/*Queue region setup */
	/*The Descriptor region address is aligned for 64 Byte access */
	descAddress = (uint32_t)region0DescriptorSpace;
	/*Add 64 to the starting address */
	descAddress = descAddress + BYTE_ALIGNMENT;
	/*Align the strating adress */
	descAddress &= ~0x3F;
	cppiInfo.region0DescriptorAddress = (uint32_t *)descAddress;

	HWREG(usbInstance->otgBaseAddress + CPDMA_QUEUEMGR_REGION_0)=
									(uint32_t)cppiInfo.region0DescriptorAddress;
	/*queue manager cotrol setup */
	HWREG(usbInstance->otgBaseAddress + CPDMA_QUEUEMGR_REGION_0_CONTROL) =
										QUEUE_MGR_DESCSIZE | QUEUE_MGR_REGSIZE;

	/*initialize the  BD pool */
	Cppi41DmaInitBdPool();

	/*configure the RX channel for each end point */
	for(i=0;i<numOfEndPoimts;i++)
	{
		if((epInfo + i)->direction == CPDMA_DIR_RX)
			Cppi41DmaConfigRxhostPacket(usbDevInst, usbInstance
				->rxEndPoint[(epInfo + i)->endPoint].channel,
				usbInstance->rxEndPoint[(epInfo + i)->endPoint].submitq,
				usbInstance->rxEndPoint[(epInfo + i)->endPoint].submitq);

	}

	/*configure the RX and TX completion queues for each end point */
	for(i=0;i<numOfEndPoimts;i++)
	{
		if((epInfo + i)->direction == CPDMA_DIR_RX)
		{
			Cppi41DmaConfigRxCompletionQueue(usbDevInst, usbInstance
			->rxEndPoint[(epInfo + i)->endPoint].channel, usbInstance
							->rxEndPoint[(epInfo + i)->endPoint].complettionq);
		}
		else
		{
			Cppi41DmaConfigTxCompletionQueue(usbDevInst, usbInstance
			->txEndPoint[(epInfo + i)->endPoint].channel, usbInstance
							->txEndPoint[(epInfo + i)->endPoint].complettionq);
		}
	}

     /*configure the TD Free Decsriptor Queue Control */
    Cppi41DmaConfigTDDescQueue(usbDevInst, USB0_TEARDOWNQ1);

	/*Scheduler configuration */
	/*Set the oerder of each RX and TX chennel to Processed to scheduler table 0 */
	HWREG(usbInstance->otgBaseAddress + CPDMA_SCHED_TABLE_0) = SCHEDULE_TX_CHANNEL;
	HWREG(usbInstance->otgBaseAddress + CPDMA_SCHED_TABLE_1) = SCHEDULE_RX_CHANNEL;

	/*Specify the number of entries in the scheduler table 0 */
	HWREG(usbInstance->otgBaseAddress + CPDMA_SCHED_CONTROL_REG )=
								(ENABLE_CPPIDMA<< SCHEDULER_ENABLE_SHFT) |
											(NUM_OF_SCHEDULER_ENTRIES - 1);

}

/**
* \brief	  This API sets the DMA mode for each channel
*
* \param	  dmaMde :- mode for rx and Tx.
*
*\param	  epInfo:- End point info structure
*
* \return	  None.
*
**/
void Cppi41DmaModeSet(uint16_t usbDevInst, endpointInfo *epInfo)
{

	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/* Enable RNDIS from Global Level */
	if (epInfo->dmaMode == CPDMA_MODE_SET_RNDIS)
	{
		HWREG(usbInstance->otgBaseAddress + USB_0_CTRL)|=
												CPDMA_MODE_ENABLE_GLOBAL_RNDIS;
	}

	/* Disable RNDIS from Global Level	 */
	else
	{
		HWREG(usbInstance->otgBaseAddress + USB_0_CTRL)&=
												~CPDMA_MODE_ENABLE_GLOBAL_RNDIS;
	}

	/*Set the mode for RX and TX channel */
	if(epInfo->direction == CPDMA_DIR_TX)
	{
		HWREG(usbInstance->otgBaseAddress + USB_0_MODE)|=
					(epInfo->dmaMode << USB_TX_MODE_SHIFT(epInfo->endPoint));
	}

	else
	{
		HWREG(usbInstance->otgBaseAddress + USB_0_MODE)|=
					(epInfo->dmaMode<< USB_RX_MODE_SHIFT(epInfo->endPoint));
	}

	/* For GRNDIS mode, set the maximum packet length */
	if (epInfo->dmaMode == CPDMA_MODE_SET_GRNDIS)
	{
		HWREG(usbInstance->otgBaseAddress + USB_0_GEN_RNDIS_SIZE_EP1) =
			GRNDIS_MAX_PACKET_LENGTH;
	}
}


/**
* \brief	  This API enables the TX DMA for an endpoint. The API access the CSR
*		  register of  the particular endpoint to set appropriate bits. This API is
*		  called by the stack to enable DMA when it is requied.
*
* \param	  usbDevInst:- USB device instance
*
* \param  ulEndpoint:- Endpoint Number
*
* \return	  None.
*
**/
void enableCoreTxDMA(uint16_t usbDevInst, uint32_t ulEndpoint)
{
	uint32_t ulRegister;
	usbInstance *usbInstance;
    uint32_t timeout = 0xFFFF;
	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*Find out the TX CRS address */
	ulRegister = USB_O_TXCSRL1 + EP_OFFSET(ulEndpoint);

    /*Wait till completion of any previous transaction */
    do
    {
        --timeout;
    }
	while (((HWREGH(usbInstance->usbBaseAddress + ulRegister) & 0x2) == 0x02)&&timeout);

	/* Clear Autoset */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)&= CPDMA_TX_CLR_AUTO_SET;

	/* Set DMAReqEnab & DMAReqMode */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)|=CPDMA_TX_SET_REQ_ENABLE;

}


/**
* \brief	  This API enables the RX DMA for an endPoint.The API access the CSR
*		  register of  the particular endpoint to set appropriate bits. This API is
*		  called by the stack to enable DMA when it is requied.
*
* \param	  usbDevInst:- USB device instance
*
* \param  ulEndpoint:- Endpoint Number
*
* \return	  None.
*
**/
void enableCoreRxDMA(uint16_t usbDevInst, uint32_t ulEndpoint)
{
	uint32_t ulRegister;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*Find out the RX CSR  address */
	ulRegister = USB_O_RXCSRL1 + EP_OFFSET(ulEndpoint);

	/*Find out the index to usb intance end point array */
	ulEndpoint = USB_EP_TO_INDEX(ulEndpoint);

	/*Enable the scheduler with number entries */
	Cppi41DmaControlScheduler(usbDevInst, ENABLE_CPPIDMA, NUM_OF_SCHEDULER_ENTRIES);

	/*Configure the completion queue */
	Cppi41DmaConfigRxCompletionQueue(usbDevInst, usbInstance->rxEndPoint[ulEndpoint].channel,
									usbInstance->rxEndPoint[ulEndpoint].complettionq);

	/* Clear AUTOCLEAR and DMAReqMode */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)&=CPDMA_RX_CLR_AUTO_CLEAR;

	/* Set DMAReqEnab */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)|=CPDMA_RX_SET_REQ_ENABLE;
}

/**
* \brief	  This API disables the RX DMA for an endpoint. The API access the CSR
*		  register of  the particular endpoint to set appropriate bits. This API is
*		  Called by the stack to disable DMA when it is required
*
* \param	  usbDevInst:- USB device instance
*
* \param  ulEndpoint:- Endpoint Number
*
* \return	  None.
*
**/
void disableCoreRxDMA(uint16_t usbDevInst, uint32_t ulEndpoint)
{
	uint32_t ulRegister;
	usbInstance *usbInstance;
    uint32_t timeout = 0xFFFF;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*Find out the CSR offset */
	ulRegister = USB_O_RXCSRL1 + EP_OFFSET(ulEndpoint);

	/*wait till completion of any previos transaction */
    do
    {
       --timeout;
    }
	while (((HWREGH(usbInstance->usbBaseAddress + ulRegister) & 0x1) == 0x01)&&timeout);

	/* Clear DMAReqEnab */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)&= CPDMA_RX_CLR_REQ_ENABLE;
}

/**
* \brief	  This API enables the TX DMA for an endpont. The API access the CSR
*		  register of  the particular endpoint to set appropriate bits. This API is
*		  Called by the stack to disable DMA when it is required
*
* \param	  usbDevInst:- USB device instance
*
* \param  ulEndpoint:- Endpoint Number
*
* \return	  None.
*
**/
void disableCoreTxDMA(uint16_t usbDevInst, uint32_t ulEndpoint)
{
	uint32_t ulRegister;
	usbInstance *usbInstance;
    uint32_t timeout = 0xFFFF;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	ulRegister = USB_O_TXCSRL1 + EP_OFFSET(ulEndpoint);
    do
    {
        --timeout;
    }
	while (((HWREGH(usbInstance->usbBaseAddress + ulRegister) & 0x2) == 0x02)&&timeout);

	/* Clear AUTOSET */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)&=CPDMA_TX_CLR_AUTO_SET;

	/* Clear DMAReqEnab & DMAReqMode */
	HWREGH(usbInstance->usbBaseAddress + ulRegister)&=CPDMA_TX_CLR_REQ_ENABLE;
}

/**
* \brief	  This API reads the complettion queue. This is called from the stack when a completion
* 		   interrupt comes. RX/TX completion queue number can be passed as an argument to
*		   this function. The function returns completed BD address.
*
* \param	  usbDevInst:- USB device instance
*
* \param  queueNum:- The completeion queue number
*
* \return	  None.
*
**/
uint32_t Cppi41DmaReadCompletionQueue(uint16_t usbDevInst,
														uint32_t queueNum)
{
	uint32_t  descAddress;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*Read the completetion queue register */
	descAddress=(uint32_t)HWREG(usbInstance->otgBaseAddress +
							(CPDMA_QUEUE_REGISTER_D + QUEUE_OFFSET(queueNum)));
	/*Extract the BD address  */
	descAddress&=0xFFFFFFE0;
	return(descAddress);
}

/**
* \brief	This API  configures an RX channel submit queue
*
* \param	 usbDevInst:- The USB Device instance
*
* \param	 chan:- The channel Number
*
* \param  rxSubmitqA:- The queue register A value
*
* \param	  rxSubmitqB:- The queue register B value
*
* \return	  None.
*
**/
void Cppi41DmaConfigRxhostPacket(uint16_t usbDevInst, uint16_t chan,
							uint32_t rxSubmitqA, uint32_t rxSubmitqB)
{
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	HWREG(usbInstance->otgBaseAddress + (CPDMA_RX_CHANNEL_REG_A +
											CHANNEL_OFFSET(chan))) = (rxSubmitqA | rxSubmitqA << 16);
	HWREG(usbInstance->otgBaseAddress + (CPDMA_RX_CHANNEL_REG_B +
											CHANNEL_OFFSET(chan))) = (rxSubmitqB | rxSubmitqB << 16) ;
}

/**
* \brief	 Configure the RX completion queues
*
* \param	 usbDevInst:- The USB Device instance
*
* \param	 rxChan:- The channel Number
*
* \param  rxCompq:-RX Completion queue
*
* \return	  None.
*
**/
void Cppi41DmaConfigRxCompletionQueue(uint16_t usbDevInst,
								uint16_t rxChan,uint32_t rxCompq)
{
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	HWREG(usbInstance->otgBaseAddress + (CPDMA_RX_CHANNEL_CONFIG_REG +
				CHANNEL_OFFSET(rxChan))) = CPDMA_RX_CHANNEL_ENABLE	 | rxCompq;

}


/**
* \brief	 Configure the TX completion queues
*
* \param	 usbDevInst:- The USB Device instance
*
* \param	 txChan:- The channel Number
*
* \param  txCompq:-RX Completion queue
*
* \return	  None.
*
**/
void Cppi41DmaConfigTxCompletionQueue(uint16_t usbDevInst,
									uint16_t txChan, uint32_t txCompq)
{
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	HWREG(usbInstance->otgBaseAddress + (CPDMA_TX_CHANNEL_CONFIG_REG +
				CHANNEL_OFFSET(txChan))) = 	CPDMA_TX_CHANNEL_ENABLE	 | txCompq;
}



/**
* \brief	 Initializes the BD pool
*
* \param
*
* \return	  None.
*
**/
void Cppi41DmaInitBdPool()
{
	hostPacketDesc *temp_bd = NULL;
	uint32_t i;

	temp_bd = (hostPacketDesc *)cppiInfo.region0DescriptorAddress;
	for(i=0;i<MAX_BD_NUM; i++, temp_bd++)
	{
		if(!cppiInfo.head_bd)
		{
			cppiInfo.head_bd = temp_bd;
			cppiInfo.tail_bd = temp_bd;
			temp_bd->nextHBDptr = NULL;
		}
		else
		{
			cppiInfo.tail_bd->nextHBDptr = temp_bd;
			cppiInfo.tail_bd = cppiInfo.tail_bd->nextHBDptr;
			temp_bd->nextHBDptr = NULL;
		}
	}
	temp_bd = NULL;
}


/**
* \brief	This API will get a free BD from the BD pool
*
* \param
*
* \return	  None.
*
**/
hostPacketDesc * getFreeBd()
{
	hostPacketDesc *free_bd;

	free_bd = NULL;

	if(cppiInfo.head_bd)
	{
		free_bd = cppiInfo.head_bd;
		cppiInfo.head_bd = cppiInfo.head_bd->nextHBDptr;
		free_bd->nextHBDptr = NULL;
	}

	return free_bd;
}

/**
* \brief	 This API puts the free BD back to the BD pool
*
* \param
*
* \return	  None.
*
**/
void putFreeBd(hostPacketDesc *free_bd)
{
	if(cppiInfo.head_bd)
	{
		free_bd->nextHBDptr = cppiInfo.head_bd;
		cppiInfo.head_bd =free_bd;
	}
	else
	{
		cppiInfo.head_bd = free_bd;
		cppiInfo.head_bd->nextHBDptr = NULL;
	}
}

/**
* \brief	 API to submit a BD to the queue
*
* \param	  usbDevInst:- The USB device instance
*
* \param  queueNum:- The submit queue number
*
* \param curr_bd:- Address of BD to submit
*
* \return	  None.
*
**/
void pushToSubmitQ(uint16_t usbDevInst,
							uint32_t  queueNum, hostPacketDesc *curr_bd)
{

	uint32_t submitqAdd;
	uint32_t curr_bdAdd;
	hostPacketDesc *bdAddr;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*get the BD address */
	curr_bdAdd = (uint32_t) curr_bd;
	/* Add the length to it */
	curr_bdAdd |= SIZE_OF_SINGLE_BD;
	bdAddr = (hostPacketDesc *)curr_bdAdd;
	/*Get the submit queue address */
	submitqAdd = usbInstance->otgBaseAddress + (CPDMA_QUEUE_REGISTER_D +
														QUEUE_OFFSET(queueNum));
	/*submit the queue */
	HWREG(submitqAdd) = (uint32_t) bdAddr;

}

/**
* \brief	 This API gets a free BD , process it for TX transfer and submit it to the TX queue.
*		 The API can take care of packetizing the data according to USB transfer size and
*		 attach to the BDs. This API will work in both transparent mode and GRNDIS mode .
*
* \param	  lenght:- length of the data buffer
*
* \param	  buff :- Bata buffer
*
* \param	  endPoind :- The TX end point
*
* \return	  None.
*
**/

void doDmaTxTransfer(uint16_t usbDevInst, uint8_t *buff,
								uint32_t length, uint32_t endPoint)
{
	hostPacketDesc *current_bd = NULL;

	uint32_t numOfBlocks =0;
	uint32_t residue;
	uint32_t i;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	endPoint = USB_EP_TO_INDEX(endPoint);

	/*This code segment will take care of the transparent mode transaction */
	if(CPDMA_MODE_SET_TRANSPARENT == usbInstance->txEndPoint[endPoint].mode)
	{
		/* If the data length is less than the USB packet size */
		if(length < USB_PACKET_LENGTH)
		{
			/*get a free db from the BD pool	 */
			current_bd = getFreeBd();
			ASSERT(current_bd != NULL);

			/*Clean the cache so that buffer will have proper data */
			usb_osalCacheWb((void*)buff, length);

			/*This API will initialize the BD fields */
			Cppi41DmaProcessBD(usbDevInst, current_bd, CPDMA_DIR_TX, buff, length, endPoint);

			current_bd->packetId = EOP;

			/*Clean the cache so that bd  will have proper data */
			usb_osalCacheWb((void*)current_bd,sizeof(hostPacketDesc));

			/*Submit the BD to the queue for transaction	 */
			pushToSubmitQ(usbDevInst, usbInstance->txEndPoint[endPoint]
				.submitq, current_bd);
		}

		/*If the length is more than packet size, then packetize it  */
		else
		{
			numOfBlocks = length / USB_PACKET_LENGTH;
			residue = length -  (numOfBlocks * USB_PACKET_LENGTH);

			for(i=0; i < numOfBlocks; i++)
			{
				/*get a free db from the BD pool	 */
				current_bd = getFreeBd();
				ASSERT(current_bd != NULL);

				usb_osalCacheWb((void*)&buff[i*USB_PACKET_LENGTH],
					USB_PACKET_LENGTH);

				/*This API will initialize the BD fields */
				Cppi41DmaProcessBD(usbDevInst, current_bd, CPDMA_DIR_TX,
									&buff[i*USB_PACKET_LENGTH], USB_PACKET_LENGTH, endPoint);

				/*Set the packet id to identify the packet position */
				if(numOfBlocks == 1)
					current_bd->packetId = EOP;
				else if(i==0)
					current_bd->packetId = SOP;
				else if((numOfBlocks != 1) && (i < (numOfBlocks -1)))
					current_bd->packetId = MOP;
				else if(i == (numOfBlocks -1) && residue == 0)
					current_bd->packetId = EOP;
				else
					current_bd->packetId = MOP;

				usb_osalCacheWb((void*)current_bd,sizeof(hostPacketDesc));

				/*Submit the BD to the queue for transaction	 */
				pushToSubmitQ(usbDevInst, usbInstance->txEndPoint[endPoint]
					.submitq, current_bd);
			}

			/*If there are some remaining data then send it also */
			if(residue)
			{
				/*get a free db from the BD pool	 */
				current_bd = getFreeBd();
				ASSERT(current_bd != NULL);

				usb_osalCacheWb((void*)&buff[i*USB_PACKET_LENGTH], residue);

				/*This API will initialize the BD fields */
				Cppi41DmaProcessBD(usbDevInst, current_bd, CPDMA_DIR_TX,
									&buff[i*USB_PACKET_LENGTH], residue, endPoint);

				current_bd->packetId = EOP;

				/*This API will initialize the BD fields */
				usb_osalCacheWb((void*)current_bd,sizeof(hostPacketDesc));

				/*Submit the BD to the queue for transaction	 */
				pushToSubmitQ(usbDevInst, usbInstance->txEndPoint[endPoint]
					.submitq, current_bd);
			}

		}
	}

	/*This segment of the code will take care of GRNIS mode of transaction */
	else if(usbInstance->txEndPoint[endPoint].mode == CPDMA_MODE_SET_GRNDIS)
	{
		/*get a free db from the BD pool	 */
		current_bd = getFreeBd();
		ASSERT(current_bd != NULL);

		usb_osalCacheWb((void*)buff, length);

		/*This API will initialize the BD fields */
		Cppi41DmaProcessBD(usbDevInst, current_bd, CPDMA_DIR_TX, buff, length, endPoint);

		current_bd->packetId = EOP;

		usb_osalCacheWb((void*)current_bd,sizeof(hostPacketDesc));

		/*Submit the BD to the queue for transaction	 */
		pushToSubmitQ(usbDevInst, usbInstance->txEndPoint[endPoint]
			.submitq, current_bd);
	}

}


/**
* \brief	 This API gets a free BD , process it for RX transfer and submit it to the RX queue.
*		 This API will work in both transparent mode and GRNDIS mode. . Max transfer size
*		 of this rx transfer is max USB packet size
*
* \param	  lenght:- length of the data buffer
*
* \param	  buff :- Bata buffer
*
* \param	  endPoind :- The RX end point
*
* \return	  None.
*
**/

void doDmaRxTransfer(uint16_t usbDevInst, uint32_t length,
								uint8_t *buff, uint32_t endPoint)
{
	hostPacketDesc *current_bd = NULL;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	current_bd = getFreeBd();
	ASSERT(current_bd != NULL);

	endPoint = USB_EP_TO_INDEX(endPoint);

	/* Process the BD */
	Cppi41DmaProcessBD(usbDevInst, current_bd, CPDMA_DIR_RX, buff, length, endPoint);

	/* Clean the cache */
	usb_osalCacheWb((void*)current_bd,sizeof(hostPacketDesc));

	/*Submit the BD */
	pushToSubmitQ(usbDevInst, usbInstance->rxEndPoint[endPoint].submitq, current_bd);

}

/**
* \brief	 This API reads the TX completion queue and puts the free bd to the BD pool.
*		 Extracts the buffer address from the BD and frees it. This will return the
*		 current DMA status on TX operation to the caller. his API will be called on
*		reception of a completion interrupt
*
* \param	 usbDevIns:- The USB device instacnce
*
* \param	 ulEndpoint :- Endpoint Number*
*
* \return	  None.
*
**/

uint32_t dmaTxCompletion(uint16_t usbDevInst, uint32_t ulEndpoint )
{

	hostPacketDesc *completed_bd;
	uint32_t state;
	uint32_t packetid;
	usbInstance *usbInstance;
	usbInstance = &(cppiInfo.usbInst[usbDevInst]);


	ulEndpoint = USB_EP_TO_INDEX(ulEndpoint);


	/*read the compltetion queue */
	completed_bd = (hostPacketDesc *)Cppi41DmaReadCompletionQueue(usbDevInst, usbInstance
											->txEndPoint[ulEndpoint].complettionq);
	/*Get the packet ID to update the DMA status */
	packetid = completed_bd->packetId;
	if(packetid == EOP)
		state = DMA_TX_COMPLETED;
	else
		state = DMA_TX_IN_PROGRESS;

	usb_osalCacheInvalidate((void *)completed_bd->buffAdd, sizeof(completed_bd->buffAdd));

	cppiDmaFreenBuffer((uint32_t *)completed_bd->buffAdd);

	completed_bd->buffAdd = 0;

	/*put the free buffer */
	putFreeBd(completed_bd);

	return state;

}

/**
* \brief	 This API reads the RX completion queue and puts the free bd to the BD pool
*		Extracts the data buffer from the DB and pass it to the caller . Extracts the
*		data buffer from the DB and pass it to the caller . This API will be called on
*		reception of a completion interrupt
*
* \param	  usbDevInst:- The USB Intance  Number
*
* \param	 ulEndpoin:- End point Number
*
* \return	  Free BD.
*
**/
uint32_t dmaRxCompletion(uint16_t usbDevInst, uint32_t ulEndpoint, uint32_t *pkLength )
{
	uint32_t bufferAdd;
	uint32_t length;
	usbInstance *usbInstance;

	hostPacketDesc *rx_bd =NULL;;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	ulEndpoint = USB_EP_TO_INDEX(ulEndpoint);

	/*read the compltetion queue */
	rx_bd = (hostPacketDesc *)Cppi41DmaReadCompletionQueue(usbDevInst, usbInstance
												->rxEndPoint[ulEndpoint].complettionq);

	/*Fush the cache to update the BD */
	usb_osalCacheInvalidate((void *)rx_bd, sizeof(hostPacketDesc));

	bufferAdd = rx_bd->buffAdd;
	length = rx_bd->buffLength;
	*pkLength = length;

	/*Flush the cache to update the buffer */
	usb_osalCacheInvalidate((void *)bufferAdd, length);

	putFreeBd(rx_bd);

	return bufferAdd;
}


/**
* \brief	 This API gets the transfer completion interupt status. This API will be on
*		 reception of a USB interrupt to check the cause of interrupt.
*
* \param	 usbDevInst:- The USB Intance  Number
*
* \return	  None.
*
**/
uint32_t CppiDmaGetPendStatus(uint16_t usbDevInst)
{

#if defined (am335x_15x15) || defined(am335x)
	uint32_t pendReg2 = 0;
	uint32_t pendReg3 = 0;
#endif

	uint32_t pendReg0 = 0;

#if defined (am335x_15x15) || defined(am335x)
	pendReg2 = HWREG(USB_OTGBASE + CPDMA_PEND_2_REGISTER);
	pendReg3 = HWREG(USB_OTGBASE + CPDMA_PEND_3_REGISTER);

	pendReg2 = pendReg2 & CPDMA_TX_PENDING;
	pendReg3 = pendReg3 & CPDMA_RX_PENDING;

	pendReg0 = pendReg2 | pendReg3;

#else
	/*Read the pend register */
	pendReg0 = HWREG(USB_OTGBASE + CPDMA_PEND_0_REGISTER);
#endif
	return pendReg0;
}

/**
* \brief	 This API gets the starvation interupt status. This API will be on
*		 reception of a USB interrupt to check the cause of interrupt.
*		 This API will also disable the scheduler and clear the starvation
*		 interrupt status in order to avoid over flooding of interrupt
*
* \param	 usbDevInst:- The USB Intance  Number
*
* \return	  None.
*
**/
uint32_t CppiDmaGetINTD0Status(uint16_t usbDevInst)
{

	uint32_t intdReg0;
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	intdReg0 = HWREG(usbInstance->otgBaseAddress + CPDMA_INTD_0_REGISTER);

	if(intdReg0 == CPDMA_STAR_0_PEND ||intdReg0 == CPDMA_STAR_1_PEND)
	{
		Cppi41DmaControlScheduler(usbDevInst, ENABLE_CPPIDMA,
										(NUM_OF_SCHEDULER_ENTRIES / 2));
		Cppi41DmaClearINTD0Status(usbDevInst);
	}

	return intdReg0;
}

/**
* \brief	 This API clears the starvation interupt status
*
* \param	 usbDevInst:- The USB Intance  Number
*
* \return	  None.
*
**/
void Cppi41DmaClearINTD0Status(uint16_t usbDevInst)
{
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	HWREG(usbInstance->otgBaseAddress + CPDMA_CLEAR_INTD_0_STATUS) = CLAER_INTDO_STATUS;

}

/**
* \brief	 This API configures the scheduler
*
* \param	 Configuration entries for the channels
*
* \return	  None.
*
**/
void Cppi41DmaControlScheduler(uint16_t usbDevInst, uint16_t enableDisable,
												uint16_t numOfentries)
{
	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	HWREG(usbInstance->otgBaseAddress + CPDMA_SCHED_CONTROL_REG ) =(enableDisable
								<< SCHEDULER_ENABLE_SHFT) | (numOfentries - 1);

}

/**
* \brief	 This API to Process the BD
*
* \param	 current_bd :- the current BD Address
*
* \param	 direction:- The Transfer Direction
*
* \param	 buffadd:- The buffer address
*
* \param	 length:- Length of the buffer
*
* \return	  None.
*
**/
void Cppi41DmaProcessBD(uint16_t usbDevInst, hostPacketDesc *current_bd,
								 uint16_t direction, uint8_t * buffadd,
									   uint32_t length, uint32_t ulEndpoint)
{

	usbInstance *usbInstance;

	usbInstance = &(cppiInfo.usbInst[usbDevInst]);

	/*These values are fixed for a PD */
	current_bd->hPDword0.hostPktType = CPDMA_BD_PACKET_TYPE;
	current_bd->hPDword0.protSize=0;

	/* For TX BD, need to mention the lenght, for RX CPU will fill the feild */
	if(direction == CPDMA_DIR_TX)
		current_bd->hPDword0.pktLength= length;
	else
		current_bd->hPDword0.pktLength= 0;

	/* These vlaues are always initialized to zero */
	current_bd->hPDword1.dstTag=0;
	current_bd->hPDword1.srcSubChNum=0;
	current_bd->hPDword1.srcChNum=0;

	/*The channel Number */
	if(direction == CPDMA_DIR_TX)
		current_bd->hPDword1.srcPrtNum = usbInstance->txEndPoint[ulEndpoint]
																		.channel + 1; /*chan_num+1; */
	else
		current_bd->hPDword1.srcPrtNum = usbInstance->rxEndPoint[ulEndpoint]
																		.channel + 1; /*chan_num+1; */

	/*Completion Queues */
	if(direction == CPDMA_DIR_TX)
		current_bd->hPDword2.pktRetQueue=usbInstance->txEndPoint[ulEndpoint]
																		.complettionq; /*returnQueue; */
	else
		current_bd->hPDword2.pktRetQueue=usbInstance->rxEndPoint[ulEndpoint]
																		.complettionq; /*returnQueue; */

	current_bd->hPDword2.pktRetQM=0;
	/*The BD is on chip */
	current_bd->hPDword2.onChip=1;
	current_bd->hPDword2.retPolicy=0;
	current_bd->hPDword2.protoSpecific=0;
	current_bd->hPDword2.rsv=0;
	/*USB type packet */
	current_bd->hPDword2.pktType=5;
	current_bd->hPDword2.pktErr=0;
	current_bd->buffLength=length;
	current_bd->buffAdd=(uint32_t)buffadd;
	current_bd->nextHBDptr=0;
	current_bd->gBuffLength=current_bd->buffLength;
	current_bd->gBuffLength |= (uint32_t)(1<<31);
	current_bd->gBuffAdd=current_bd->buffAdd;
	current_bd->endPoint = (uint16_t)ulEndpoint;
	if(direction == CPDMA_DIR_TX)
		current_bd->channel = usbInstance->txEndPoint[ulEndpoint].channel;
	else
		current_bd->channel = usbInstance->rxEndPoint[ulEndpoint].channel;
	current_bd->devInst = (uint8_t)usbDevInst;

}


/**
* \brief	 This API initialize the bbuffer
*
* \param	 none
*
* \return	  None.
*
**/
void cppi41DmaInitBuffer()
{
	int i;

	for(i =0;i<CPDMA_NUMOF_BUFFERS;i++)
		{
			 dmaBuffer[i].dataBuffer =(uint32_t *)&dBuffer[i][0];
			 dmaBuffer[i].usedFlag = CPDMA_BUFFER_NOT_USED;
			 dmaBuffer[i].nBlocks = 0;
		}

}


/**
* \brief	 This API will allocate 1unit(512 bytes) of  buffer. This API is called
*		just before the RX and TX transfer. The API returns a pointer to the
*		buffer allocated , which can used by the caller for data transfer.
*
* \param	 none
*
* \return	  None.
*
**/
uint32_t * cppiDmaAllocBuffer()
{
	int i;

	for(i =0;i<CPDMA_NUMOF_BUFFERS;i++)
	{
		if(dmaBuffer[i].usedFlag ==  CPDMA_BUFFER_NOT_USED)
		{
			dmaBuffer[i].usedFlag =  CPDMA_BUFFER_USED;
			dmaBuffer[i].nBlocks = 1;
			return dmaBuffer[i].dataBuffer;
		}
	}

	return NULL;
}


/**
* \brief	 This API will free 1 unit of buffer. Caller needs to pass the buffer
*		address to be freed. Once buffer is freed, the usage flag will set
*		as BUFFER _NOT_USED
*
* \param	 dataBuffer:- Buffer address
*
* \return	  None.
*
**/
void cppiDmaFreeBuffer(uint32_t *dataBuffer)
{
	int i;

	for(i=0;i<CPDMA_NUMOF_BUFFERS;i++)
	{
		if(dmaBuffer[i].dataBuffer == dataBuffer)
		{
			dmaBuffer[i].usedFlag = CPDMA_BUFFER_NOT_USED;
			dmaBuffer[i].nBlocks = 0;
			break;
		}
	}

}

/**
* \brief	 This API will allocate N (N*512 bytes) number of buffers. This API is called
*		just before the RX and TX transfer. The API returns a pointer to the
*		buffer allocated , which can used by the caller for data transfer.
*
* \param	 numOfBlocks:- Number of buffers
*
* \return	  None.
*
**/
uint32_t * cppiDmaAllocnBuffer(uint32_t numOfBlocks)
{
	uint32_t nBlocks = numOfBlocks;
	uint32_t i,j;



	for(i =0;i<CPDMA_NUMOF_BUFFERS;i++)
	{
		if(dmaBuffer[i].usedFlag ==  CPDMA_BUFFER_NOT_USED)
			nBlocks--;
		else
			nBlocks = numOfBlocks;

		if(nBlocks == 0)
			break;

	}


	if(nBlocks == 0)
	{
		for(j = i; j>(i-numOfBlocks); j--)
		{
			dmaBuffer[j].usedFlag =  CPDMA_BUFFER_USED;
		}
		dmaBuffer[j+1].nBlocks = numOfBlocks;
		return dmaBuffer[j+1].dataBuffer;
	}
	else
		return NULL;

}


/**
* \brief	 This API will free N(N *512 Bytes) buffers. Caller needs to pass the buffer
*		address to be freed. Once buffer is freed, the usage flag will set
*		as BUFFER _NOT_USED.
*
* \param	 dataBuffer:- Buffer address
*
* \return	  None.
*
**/

void cppiDmaFreenBuffer(uint32_t *dataBuffer)
{
	int i,j;
	uint32_t nBlocks = 0;

	for(i=0;i<CPDMA_NUMOF_BUFFERS;i++)
	{
		if(dmaBuffer[i].dataBuffer == dataBuffer)
		{
			nBlocks= dmaBuffer[i].nBlocks;
			break;
		}
	}

	if(nBlocks)
	{
		for(j =i; j<(i+nBlocks); j++)
		{
			dmaBuffer[j].usedFlag = CPDMA_BUFFER_NOT_USED;
			dmaBuffer[i].nBlocks= 0;
		}

	}

}

/**
* \brief	 This API will Handle the starvation Error.  The API will enable the
*		 scheduler which is disabled due to the starvation interrupt.
*		 This will help the DMA to resume the operation after clearing the starvation
*		 interrupt.
*
* \param	 dusbDevInst:- USB device instance.
*
* \return	  None.
*
**/

void cppiDmaHandleError(uint32_t usbDevInst)
{
	Cppi41DmaControlScheduler(usbDevInst, ENABLE_CPPIDMA, NUM_OF_SCHEDULER_ENTRIES);

}

int Cppi41DmaRxChTeardown(uint16_t usbDevInst, uint32_t ulEndpoint )
{
    signed int TeardownStatus = 0x0;
    usbInstance *usbInstance = 0;
    volatile int TimeOut = 0xffff;
    uint32_t ulEndPointIndex = USB_EP_TO_INDEX(ulEndpoint);
    uint16_t ulRxChan = 0;
    hostPacketDesc* completed_bd = NULL;
    hPDWord0* DescHeaderPtr = NULL;

    int GlobalConfigReg = 0;


    if (usbDevInst > 1)
    {
        /* wrong device instance. can only be 0 or 1 */
        return 0;
    }

    if (ulEndPointIndex > (NUM_USB_EP/2))   /* outside of 15 USB endpoints */
    {
        /* wrong endpoint provided */
        return 0;
    }

    usbInstance = &(cppiInfo.usbInst[usbDevInst]);
    ulRxChan = usbInstance->rxEndPoint[ulEndPointIndex].channel;

    GlobalConfigReg = HWREG(usbInstance->otgBaseAddress +
                      (CPDMA_RX_CHANNEL_CONFIG_REG + CHANNEL_OFFSET(ulRxChan)));

    /* Set Completion Q related info - Default Q manager (0)and Default Completion Q
    Note that we have alerady done this for this endpoint ,but  these fields
    are Write only! */

    GlobalConfigReg |=
     ((usbInstance->rxEndPoint[ulEndPointIndex].complettionq
      << DMA_CH_TX_DEFAULT_QNUM_SHIFT) & DMA_CH_TX_DEFAULT_QNUM_MASK);

    /* Initiate channel teardown in gloabal CPPI Regs TxGCRn or RxGCRn */
    /* Disable the DMA channel and set the tear down bit*/

    /* Note CPPI Specification 4.7 clearly states that "host should not clear
       the enable bit when issuing the teardown command. Doing so may prevent
       the teardown operation from completing since the channel will be removed
       from transmit scheduling operations." - But for us this is what works!   */

    GlobalConfigReg &= ~DMA_CH_TX_ENABLE_MASK;
    GlobalConfigReg |= DMA_CH_TX_TEARDOWN_MASK;

    HWREG(usbInstance->otgBaseAddress + (CPDMA_RX_CHANNEL_CONFIG_REG +
     CHANNEL_OFFSET(ulRxChan))) = GlobalConfigReg;

    /* Now we wait for the Buffer descriptor and the Teardown Desc to appear
        in the completion Queue of the Rx endpoint */
    do
    {
      /* First the Rx BD would come to the completion Q then the TD desc */
      DescHeaderPtr =(hPDWord0*)Cppi41DmaPopFromSubmitQ( usbDevInst,
                     usbInstance->rxEndPoint[ulEndPointIndex].complettionq);
      if (NULL == DescHeaderPtr)
      {
        --TimeOut;

      }
      else if ( CPDMA_TEARDOWN_DESC_TYPE == DescHeaderPtr->hostPktType )
        {
            /* tear down descriptor has been received
            this function returns the number of  bytes recevied in the
            Rx BD*/
            if (0xFFFFFFFF == TeardownStatus )
            {
                TeardownStatus = 1;
            }
            break;
        }
      else if ( CPDMA_BD_PACKET_TYPE == DescHeaderPtr->hostPktType )
        {
            /* We have popped the completed packet - is this the same channel
            that we tore down ? */
            completed_bd = (hostPacketDesc*)DescHeaderPtr;
            TeardownStatus = DescHeaderPtr->pktLength ;
            continue;
        }
    } while((--TimeOut)>0);

    /* We have got TD in the completion Q */
    if (TimeOut)
    {
        /* Now we need to free the Rx BD - if not read the Submit Q */
        if ( !completed_bd )
        {
            completed_bd = (hostPacketDesc*)Cppi41DmaPopFromSubmitQ( usbDevInst,
                     usbInstance->rxEndPoint[ulEndPointIndex].submitq);
        }

        /* Now reuse the BD and buffer that we got in the rx completion q*/
        if (NULL!= completed_bd)
        {
            Cppi41DmaRecoverBD( completed_bd );
        }
    }
    return (TeardownStatus);
}


/**
+* \brief     This API will teardown a Transmit DMA channel.
+* /
+*  For
+*
+*
+* \param     usbDevInst:- USB device instance
+* \param     ulEndpoint:- endpoint whose rx channel needs to be torn down.
+*
+* \return      Teardown status or number of bytes transmitted by the torn channel
+*
+**/

int Cppi41DmaTxChTeardown(uint16_t usbDevInst, uint32_t ulEndpoint )
{
    signed int TeardownStatus = 0xFFFFFFFF;
    usbInstance *usbInstance = &(cppiInfo.usbInst[usbDevInst]);
    volatile int TimeOut = 0xffff;
    uint32_t ulEndPointIndex = USB_EP_TO_INDEX(ulEndpoint);
    uint16_t ulTxChan = usbInstance->txEndPoint[ulEndPointIndex].channel;

    int GlobalConfigReg =
    HWREG(usbInstance->otgBaseAddress + (CPDMA_TX_CHANNEL_CONFIG_REG +
     CHANNEL_OFFSET(ulTxChan)));
    hostPacketDesc* completed_bd = NULL;
    hPDWord0* DescHeaderPtr = NULL;
    /* Set Completion Q related info - Default Q manager (0)and Default Completion Q
    Note that we have alerady done this for this endpoint ,but  these fields
    are Write only! */

    GlobalConfigReg |=
     ((usbInstance->txEndPoint[ulEndPointIndex].complettionq
      << DMA_CH_TX_DEFAULT_QNUM_SHIFT) & DMA_CH_TX_DEFAULT_QNUM_MASK);

    /* Initiate channel teardown in gloabal CPPI Regs TxGCRn or RxGCRn */
    /* Disable the DMA channel and set the tear down bit*/

    /* Note CPPI Specification 4.7 clearly states "host should not clear
       the enable bit when issuing the teardown command. Doing so may prevent
       the teardown operation from completing since the channel will be removed
       from transmit scheduling operations." - But for us this is what works!   */

    GlobalConfigReg &= ~DMA_CH_TX_ENABLE_MASK;
    GlobalConfigReg |= DMA_CH_TX_TEARDOWN_MASK;

    HWREG(usbInstance->otgBaseAddress + (CPDMA_TX_CHANNEL_CONFIG_REG +
     CHANNEL_OFFSET(ulTxChan))) = GlobalConfigReg;

    /* Now we wait for the Buffer descriptor and the Teardown Desc to appear
        in the completion Queue of the Rx endpoint */
    do
    {
      /* Set the Teardown mask in USBnTDOWN register */
      USBDmaTxChTeardown ( USB0_BASE, USB_0_OTGBASE, ulEndpoint);

      /* First the Tx BD would come to the completion Q then the TD desc */
      DescHeaderPtr =(hPDWord0*)Cppi41DmaPopFromSubmitQ( usbDevInst,
                     usbInstance->txEndPoint[ulEndPointIndex].complettionq);

      if (NULL == DescHeaderPtr)
      {
        --TimeOut;
      }
      else if ( CPDMA_TEARDOWN_DESC_TYPE == DescHeaderPtr->hostPktType )
        {
            /* tear down descriptor has been received
            this function returns the number of  bytes recevied in the
            Rx BD*/
            if (0xFFFFFFFF == TeardownStatus )
            {
                TeardownStatus = 1;
            }
            break;
        }
      else if ( CPDMA_BD_PACKET_TYPE == DescHeaderPtr->hostPktType )
        {
            /* We have popped the completed packet - is this the same channel
            that we tore down ? */
            TeardownStatus = DescHeaderPtr->pktLength ;
            continue;
        }

    } while(--TimeOut);
    /* We have got TD in the completion Q */
    if (TimeOut)
    {
        /* Now we need to free the Tx BD - if not read the Submit Q */
        if ( !completed_bd )
        {
            completed_bd = (hostPacketDesc*)Cppi41DmaPopFromSubmitQ( usbDevInst,
                     usbInstance->txEndPoint[ulEndPointIndex].submitq);
        }
        /* Now reuse the BD and buffer that we got in the rx completion q*/
        if (NULL!= completed_bd)
        {
            Cppi41DmaRecoverBD( completed_bd );
        }
    }
    return (TeardownStatus);
}

/**
* \brief        Configures the TD Desc Queue Control
*
* \param     usbDevInst:- The USB Device instance
*
* \param     tdDescq:-TD Desc queue
*
* \return      None.
*
**/
void Cppi41DmaConfigTDDescQueue(uint16_t usbDevInst,
                                        uint32_t tdDescq)
{
    usbInstance *usbInstance;

    usbInstance = &(cppiInfo.usbInst[usbDevInst]);

    HWREG(usbInstance->otgBaseAddress + CPDMA_TD_FD_QUEUE_CONTROL_REG) = tdDescq;
}

/**
* \brief     API to submit a TDD to the queue
*
* \param      usbDevInst:- The USB device instance
*
* \param  queueNum:- The submit queue number
*
* \param curr_bd:- Address of TD to submit
*
* \return      None.
*
**/
void Cppi41DmaPushToTearDownQ(uint16_t usbDevInst,
                              uint32_t  queueNum, cppi41TearDownDesc *curr_tdd)
{

    uint32_t submitqAdd;
    usbInstance *usbInstance;
    uint32_t curr_tdAdd = 0;
    usbInstance = &(cppiInfo.usbInst[usbDevInst]);

    /*get the TD address */
    curr_tdAdd = (uint32_t) curr_tdd;
    /* Add the length to it */
    curr_tdAdd |= SIZE_OF_TEARDOWNDESC;

    submitqAdd = usbInstance->otgBaseAddress + (CPDMA_QUEUE_REGISTER_D +
                                                        QUEUE_OFFSET(queueNum));
    /*submit the queue */
    HWREG(submitqAdd) = (uint32_t) curr_tdAdd ;

}

/**
* \brief     API to submit a BD to the queue
*
* \param  usbDevInst:- The USB device instance
*
* \param  queueNum:- The submit queue number
*
* \return   BD in the submit Q.
*
**/
uint32_t Cppi41DmaPopFromSubmitQ(uint16_t usbDevInst, uint32_t  queueNum)
{
    uint32_t  descAddress;
    usbInstance *usbInstance;

    usbInstance = &(cppiInfo.usbInst[usbDevInst]);

    /*Read the completetion queue register */
    descAddress=(uint32_t)HWREG(usbInstance->otgBaseAddress +
                            (CPDMA_QUEUE_REGISTER_D + QUEUE_OFFSET(queueNum)));
    /*Extract the BD address  */
    descAddress&=0xFFFFFFE0;
    return(descAddress);
}

/**
* \brief     Initializes the TD pool
*
* \param
*
* \return      None.
*
**/

void Cppi41DmaInitTddPool(uint16_t usbDevInst )
{

    /* Since we do not have a different region space get the TD
    from the BD pool*/
    cppi41TearDownDesc *temp_bd = (cppi41TearDownDesc *)getFreeBd();
    ASSERT(temp_bd != NULL);

    temp_bd->teardown_info = (uint32_t)
        (CPDMA_DESC_TYPE_TD<<CPDMA_DESC_TYPE_TD_SHIFT);
    Cppi41DmaPushToTearDownQ(usbDevInst, USB0_TEARDOWNQ1, temp_bd);

}
/**
* \brief     API to get the queue packet count
*
* \param  usbDevInst:- The USB device instance
*
* \param  queueNum:- The queue number
*
* \return   Number of packets in submit Q.
*
**/
uint32_t Cppi41DmaGetQLength(uint16_t usbDevInst, uint32_t  queueNum)
{
    uint32_t  ulQLength;
    usbInstance *usbInstance;

    usbInstance = &(cppiInfo.usbInst[usbDevInst]);

    /*Read the completetion queue register */
    ulQLength=(uint32_t)(HWREG(usbInstance->otgBaseAddress +
                            (CPDMA_QUEUE_STATUS_A+ QUEUE_OFFSET(queueNum)))
                            &CPDMA_QUEUE_STATUS_A_COUNT_MASK);
    return(ulQLength);
}

/**
* \brief     API to Free the BDs once complete
*
* \param  completed_bd:- pointer to completed BD
*
*
*
* \return  state.
*
**/
uint32_t Cppi41DmaRecoverBD( hostPacketDesc* completed_bd )
{
    uint32_t state;
    uint32_t packetid;

    /*Get the packet ID to update the DMA status */
    packetid = completed_bd->packetId;
    /* state is just information for the caller and is curently unused*/
    if(packetid == EOP)
        state = DMA_TX_COMPLETED;
    else
        state = DMA_TX_IN_PROGRESS;

    usb_osalCacheInvalidate((void*)completed_bd->buffAdd, sizeof(completed_bd->buffAdd));

    cppiDmaFreenBuffer((uint32_t *)completed_bd->buffAdd);

    completed_bd->buffAdd = 0;

    /*put the free buffer */
    putFreeBd(completed_bd);

    return (state);
}
