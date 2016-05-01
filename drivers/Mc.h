/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Mc.h                     
*                                                                         
* Description : Revised from zhong xiang stream_controller.h   
*             
* Date        : Nov 16, 2005                                                           
***************************************************************************
*/



#include "risc.h"

#define  MC_NEW1_VERSION            // for revised image 20

#ifdef  MC_NEW1_VERSION
#define MC_TYPE_OFFSET    50
#else
#define MC_TYPE_OFFSET    49
#endif

/* Memory Controller register specifications */
#define  MC_BASE_ADDR    BLOCK_BASE_ADDR(MC_BLOCK)

#define  rMC_S_CTL       *(volatile unsigned *)(MC_BASE_ADDR + 0x0)   // Stream request arbitration control
#define  rMC_M_CTL       *(volatile unsigned *)(MC_BASE_ADDR + 0x4)   // Memory request arbitration control
#define  rMC_AS_MASK     *(volatile unsigned *)(MC_BASE_ADDR + 0x8)   // Mask of soft read streams for auto-fetch.
                                                                      // the stream ID is from 0x35 to 0x3f
                                                                      // If set, the corresponding stream auto-fetch is enabled.                                                        
#define  rMC_SDR_CTL     *(volatile unsigned *)(MC_BASE_ADDR + 0xC)   // Sdram controller registers
#define  rMC_SDR_RF      *(volatile unsigned *)(MC_BASE_ADDR + 0x10)  // sdram refresh control
#define  rMC_SDR_MODE    *(volatile unsigned *)(MC_BASE_ADDR + 0x14)  // sdram mode register values
#define  rMC_STATUS_HI   *(volatile unsigned *)(MC_BASE_ADDR + 0x18)  // It show the data availability of each Sdram queue with memory context[63:32]
                                                                      // 1 - the SDRAM buffer has at least a burst of data
                                                                      // 0 - the SDRAM buffer doesn't have a burst of data                                                          
#define  rMC_STATUS_LO   *(volatile unsigned *)(MC_BASE_ADDR + 0x1C)  // It shows the data availability of each Sdram queue with memory context[31:0]
                                                                      // 1 - the SDRAM buffer has at least a burst of data
                                                                      // 0 - the SDRAM buffer doesn't have a burst of data    
#define  rMC_INTR_CTL    *(volatile unsigned *)(MC_BASE_ADDR + 0x20)  // It controls the interrupt from MC to CPU
#define  rMC_WSTATUS_HI   *(volatile unsigned *)(MC_BASE_ADDR + 0x28)  // It shows the fullness of each SDRAM queue w ith memory context[63:32]
                                                                       // 1 每 the SDRAM queue has at lease 4 burst of space left in the queue
                                                                       // 0 每 the SDRAM queue is full                                                     
#define  rMC_WSTATUS_LO   *(volatile unsigned *)(MC_BASE_ADDR + 0x2C)  // It shows the fullness of each SDRAM queue w ith memory context[31:0]
                                                                       // 1 每 the SDRAM queue has at lease 4 burst of space left in the queue
                                                                       // 0 每 the SDRAM queue is full 
// Set random address of the memory context
#define  MC_M_ASET_BASE   0x200           
#define  rMC_M_ASET(sID)  *(volatile unsigned *)(MC_BASE_ADDR + \
                                                 MC_M_ASET_BASE + (sID<<2))

// Set length of the random context
#define  MC_M_LSET_BASE   0x400     
#define  rMC_M_LSET(sID)  *(volatile unsigned *)(MC_BASE_ADDR + \
                                                 MC_M_LSET_BASE + (sID<<2))

/* 
the stream ID to memory ID map
    Address bit[7:2] is the stream ID.
    bit[5:0] is the memory context ID.
    bit[7:6] is the burst size. It is a copy of the MBSIZE from stream context.  
*/
#define  MC_SID_MAP_BASE       0x600     
#define  rMC_SID_MAP(sID)   *(volatile unsigned *)(MC_BASE_ADDR + \
                                                   MC_SID_MAP_BASE + (sID<<2))

/* 
the manipulation of the memory context
    Address bit[ 7:2] is the memory context ID.
    Address bit[11:8] is the operation on the memory context 
*/
#define  MC_CTXT_BASE	0x2000         // the momory context
#define  rMC_CTXT(mID, operation)   *(volatile unsigned *)(MC_BASE_ADDR + MC_CTXT_BASE + \
                                                          (operation<<8) + (mID<<2))

// these 8 registers store the extended context for the video context
#define  MC_VCTXT_BASE	0x3000 
#define  rMC_VCTXT(extended_videoID)	*(volatile unsigned *)(MC_BASE_ADDR + \
                                                               MC_VCTXT_BASE + (extended_videoID<<2))


#define  rQUEUE_ADDR(queueID, extended_videoID)	*(volatile unsigned *)(V_BASE_ADDR + \
                                                  QUEUE_B_ADDR + (queueID<<8) + (extended_videoID<<16))

/* memory context ID: not necessary, but typical memory context assignments */
//Queue
#define  AUD_IN_CH0_M          0
#define  AUD_IN_CH1_M          1
#define  AUD_IN_CH2_M          2
#define  AUD_IN_CH3_M          3
#define  AUD_OUT_CH0_M 	       4
#define  AUD_OUT_CH1_M 	       5
#define  AUD_OUT_CH2_M 	       6
#define  AUD_OUT_CH3_M 	       7
#define  ETH0_TXQ_M            36   //26-43?
#define  ETH1_TXQ_M            44   //44-45?
#define  ETH0_RXQ_M            46
#define  ETH1_RXQ_M            47
#define  SEC_TASKQ_M           48
#define  SEC_ENC_RESULTQ_M     49
#define  SEC_DEC_RESULTQ_M     50
#define  MESSAGEQ_M            59

//Random
#define  SIF_M                 8
#define  PIF_M                 9
#define  ETH0_RX_M             10
#define  ETH1_RX_M             11
#define  ETH0_TX_M             12
#define  ETH1_TX_M             13
#define  SEC_WR_M              14
#define  SEC_RD_M              15
#define  PPE_RD0_M             16
#define  PPE_RD1_M             17
#define  CPU0_M                60
#define  CPU1_M                61
#define  DMA_TASKQ_CPU0_M      62
#define  DMA_TASKQ_CPU1_M      63

//Video
#define  VIN_Y_M               18
#define  VIN_U_M               19
#define  VIN_V_M               20
#define  VOUT_Y_M              21
#define  VOUT_U_M              22
#define  VOUT_V_M              23
#define  VSC_RD_Y0_M           24
#define  VSC_RD_U0_M           25
#define  VSC_RD_V0_M           26
#define  VSC_RD_Y1_M           27
#define  VSC_RD_U1_M           28
#define  VSC_RD_V1_M           29
#define  VSC_RD_OSD_M          30
#define  VSC_WR_Y_M            33
#define  VSC_WR_U_M            34
#define  VSC_WR_V_M            35
#define  FREE_POINTERQ_M       51   //51-54
#define  PPE_RESULTQ_M         55   //55-58


/* the operation on the memory context: address bit[11:8] specifies the operation */
#define  R_CONTEXT_LO          0x0      // 0 - read  context bit[31: 0]. Read only
#define  R_CONTEXT_HI          0x1      // 1 - read  context bit[63:32]. Read only
#define  W_CONTEXT_LO          0x2      // 2 - write context bit[31: 0]. Write only
#define  W_CONTEXT_HI          0x3      // 3 - write context bit[63:32]. Write only
#define  R_QUEUE_COUNT         0x4      // 4 - read queue count. Read only
#define  I_QUEUE_WP            0x5      // 5 - increment queue write pointer. Write only
#define  D_QUEUE_WP            0x6      // 6 - decrement queue write pointer. Write only.
#define  I_QUEUE_RP            0x7      // 7 - increment queue read  pointer. Write only
#define  D_QUEUE_RP            0x8      // 8 - decrement queue read  pointer. Write only
#define  C_CONTEXT             0x9      // 9 - clear context to 0
#define  S_POINT_VALUE         0xA      // 10 - set pointer value (bit[25:0]. Read or write
#define  S_RCONTEXT_LEN        0xB      // 11 - set random context length.    Read or write


/* Memory Context */
// Type[1:0]
#define  RANDOM	               0x0
#define  QUEUE                 0x1
#define  VIDEO                 0x2

// Random Context:  
//              Type         : bit[51:50]
//              Count        : bit[39:26]
//              Base address : bit[25:0]

// Queue Context:
//              Type             : bit[50:49]
//              Status           : bit[48:46]
//              Buffer size      : bit[45:43]
//              Sub-buffer size  : bit[42:40]
//              Read pointer     : bit[39:26]
//              Base address     : bit[25:14]
//              Write pointer    : bit[13:0]
// Queue Status: bit[48:46]
#ifdef   MC_NEW1_VERSION

#define  Q_NEMPTY              0x1           // buffer has data but is not full
#define  Q_EMPTY               0x0           // buffer is empty
#define  Q_FULL	               0x2           // buffer is full
#define  Q_OF                  0x5           // buffer is overflow
#define  Q_UF                  0x6           // buffer is underflow

#else

#define  Q_NEMPTY              0x0           // buffer has data but is not full
#define  Q_EMPTY               0x1           // buffer is empty
#define  Q_FULL	               0x2           // buffer is full
#define  Q_DONE	               0x3           // buffer is done
#define  Q_OF                  0x5           // buffer is overflow
#define  Q_UF                  0x6           // buffer is underflow
#define  Q_CLOSE               0x7           // buffer is closed

#endif

// Queue Buffer size: bit[45:43]
#define  Q_BufSize_256Byte     0x0
#define  Q_BufSize_512Byte     0x1
#define  Q_BufSize_1KByte      0x2
#define  Q_BufSize_2KByte      0x3
#define  Q_BufSize_4KByte      0x4
#define  Q_BufSize_8KByte      0x5
#define  Q_BufSize_16KByte     0x6

// Queue Sub-buffer size: bit[42:40]
#define  Q_SBufSize_4Byte      0x0
#define  Q_SBufSize_8Byte      0x1
#define  Q_SBufSize_16Byte     0x2
#define  Q_SBufSize_32Byte     0x3
#define  Q_SBufSize_64Byte     0x4

#ifndef  MC_NEW1_VERSION

#define  Q_SBufSize_128Byte    0x5
#define  Q_SBufSize_256Byte    0x6

#endif

#define  Q_Context_HI(status, buf_size, subbuf_size, pRead)       ((1<<17) | (status<<14) | (buf_size<<11) | \
                                                                   (subbuf_size<<8) | (pRead>>6))
#define  Q_Context_LO(pRead, base_addr, pWr)      (pWr | (base_addr & 0x3ffc000) | (pRead<<26))                                                       

/* Video Context 
* Line count per frame:bit[49:38]
* Pixel count per line:bit[37:26]
* pixel pointer:bit[25:0]
*/

/* Extended Video Context */
#define  NO_SKIP           0    // no skip/dup. Display every line
#define  HALF_SCALING      1    // 1/2 or 2 scaling. Display every 2nd line
#define  QUARTER_SCALING   2    // 1/4 or 4 scaling. Display every 4th line
#define  EIGHTH_SCALING    3    // 1/8 or 8 scaling. Display every 8th line

#define  Y_DUP_DOWN_SCALING           0    // Y frame down scaling
#define  Y_DUP_UP_SCALING             1    // Y frame up scaling

#define  UV_DUP_DOWN_SCALING          0    // UV frame down scaling
#define  UV_DUP_UP_SCALING            1    // UV frame up scaling

/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
int McQueueInit(int mID, int status, int buf_size, \
                 int subbuf_size, int base_addr);
int McQueueReInit(int mID);                 
int McRandomInit(int mID, int count, int base_addr);
int McVideoInit(int mID, int line_count, int pixel_count, int pixel_pointer);
int McVideoReInit(int mID);
int McVideoExtendedInit(int extended_videoID, int frame_width, int frame_depth,\
                        int y_skip, int uv_skip, int y_dup, int uv_dup);
int McRandomSet(int sID, unsigned int addr, unsigned int len);
int McScMap(int mID, int sID);
int McRdQueue(int mID, int *data);
int McWrQueue(int mID, int data);
int McRdContextHi(int mID);
int McRdContextLo(int mID);
int McRdQueueCount(int mID);
int McRdRandomPointer(int mID);
int McRdRandomLen(int mID);
int McRdQueueStatus(int mID);
int McRdQueueBufsize(int mID);
int McRdQueueActualBufsize(int mID);
int McRdQueueSubBufsize(int mID);
int McRdQueueRPTR(int mID);
int McRdQueueReadAddr(int mID);
int McRdQueueWPTR(int mID);
int McRdQueueWriteAddr(int mID);
int McRdQueueBaseAddr(int mID);
int McRdRandomBaseAddr(int mID);
int McRdRandomCount(int mID);
int McRdVideoPointer(int mID);
int McRdVideoLineCount(int mID);
int McRdVideoPixelCount(int mID);
int McRdVideoFramWidth(int extended_videoID);
int McRdVideoFramDepth(int extended_videoID);
int McAutoFetchEn(int sID);
void  McIntrEn(void);
void  McISR(void);
void PrinfMcQueueContext(int mID);
void DebugMcQueueContext(int mID);
void PrinfMcRandomContext(int mID, int sID);
void PrinfMcVideoContext(int videoID, int extended_videoID);
