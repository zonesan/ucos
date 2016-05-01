/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : Sc.h                     
*                                                                         
* Description : Revised from zhong xiang stream_controller.h   
*             
* Date        : Nov 16, 2005                                                           
***************************************************************************
*/
#ifndef __SC_H__
#define __SC_H__

#ifdef   SC_GLOBALS
#define  SC_EXT
#else
#define  SC_EXT  extern
#endif

#include "risc.h"

#define  SC_NEW1_VERSION        // maybe for image 10 and above

#define  SC_NEW2_VERSION        // for revised image 20

#ifdef  SC_NEW1_VERSION
#define SC_RPTR_OFFSET    11
#define SC_WPTR_OFFSET    23
#else
#define SC_RPTR_OFFSET    12
#define SC_WPTR_OFFSET    24
#endif

#define SC_ONCHIP_RAM_SIZE  4096
 
/* Stream Controller Registers Specifications */
#define  SC_BASE_ADDR    BLOCK_BASE_ADDR(SC_BLOCK)

#define  rSC_CTL         *(volatile unsigned *)(SC_BASE_ADDR + 0x0)  // Stream controller control
#define  rSC_INTR_RS     *(volatile unsigned *)(SC_BASE_ADDR + 0x4)  // Read stream interrupt.  Interrupt happens when the stream status is DONE or UF or OV.
#define  rSC_INTR_WS     *(volatile unsigned *)(SC_BASE_ADDR + 0x8)  // Write stream interrupt. Interrupt happens when the stream status is DONE or UF or OV.
#define  rSC_IENA_RS     *(volatile unsigned *)(SC_BASE_ADDR + 0xC)  // Read stream interrupt enable. Set to enable the stream's interrupt
#define  rSC_IENA_WS     *(volatile unsigned *)(SC_BASE_ADDR + 0x10) // Write stream interrupt enable. Set to enable the stream's interrupt
#define  rSC_IENA_S2M    *(volatile unsigned *)(SC_BASE_ADDR + 0x1C) // write stream to SDRAM interrupt enable
#define  rSC_INTR_S2M    *(volatile unsigned *)(SC_BASE_ADDR + 0x20) // write stream to SDRAM interrupt

// These are read only registers with stream ID N using address 0x100 + N*4 
#define  SC_STATUS_BASE   0x100   
#define  rSC_STATUS(sID)    (*(volatile unsigned *)\
                             (SC_BASE_ADDR + SC_STATUS_BASE + (sID<<2)) & S_ST_MASK)


#define  rSTREAM_ADDR(sID, operation)      *(volatile unsigned *)\
                                            (V_BASE_ADDR + STREAM_B_ADDR + \
                                            (operation<<16) + (sID<<8))


/* Stream ID */
/* Mmemory Context type: Queue */
// Write Stream
#define  AUD_IN_CH0_S        0x0
#define  AUD_IN_CH1_S        0x1
#define  AUD_IN_CH2_S        0x2
#define  AUD_IN_CH3_S        0x3

// Read Stream
#define  AUD_OUT_CH0_S       0x20
#define  AUD_OUT_CH1_S       0x21
#define  AUD_OUT_CH2_S       0x22
#define  AUD_OUT_CH3_S       0x23

/* Mmemory Context type: Random */
// Write Stream
#define  SIF_IN_S            0x4       // Data from SIF
#define  PIF_IN_S            0x5       // Data from PIF
#define  ETH0_RX_S           0x6       // Packet data to Ethernet0 MAC
#define  ETH1_RX_S           0x7       // Packet data from Ethernet1 MAC 
#define  SEC_WR_S            0x8       // Security data output
#define  PPE_WR_S            0x9       // Security data output
#define  DSP_WR_S            0xA       // Security data output

// Read Stream
#define  SIF_OUT_S           0x24      // Data to SIF
#define  PIF_OUT_S           0x25      // Data to PIF
#define  ETH0_TX_S           0x26      // Packet data to Ethernet0 MAC
#define  ETH1_TX_S           0x27      // Packet data to Ethernet1 MAC 
#define  SEC_RD_S            0x28      // Security data output
#define  PPE_RD_S            0x29      // Security data output
#define  DSP_RD_S            0x2A      // Security data output

/* Mmemory Context type: Video */
// Write Stream
#define  VIN_Y_S             0xB
#define  VIN_U_S             0xC
#define  VIN_V_S             0xD
#define  VSC_WR_Y_S          0xE
#define  VSC_WR_U_S          0xF
#define  VSC_WR_V_S          0x10

// Read Stream
#define  VOUT_Y_S            0x2B
#define  VOUT_U_S            0x2C
#define  VOUT_V_S            0x2D
#define  VSC_RD_Y0_S         0x2E
#define  VSC_RD_U0_S         0x2F
#define  VSC_RD_V0_S         0x30
#define  VSC_RD_Y1_S         0x31
#define  VSC_RD_U1_S         0x32
#define  VSC_RD_V1_S         0x33
#define  VSC_RD_OSD_S        0x34

/* Soft Stream */
// Write Stream: ID from 0x11 to 0x1f
#define  SOFT_WR_0_S         0x11
#define  SOFT_WR_1_S         0x12
#define  SOFT_WR_2_S         0x13
#define  SOFT_WR_3_S         0x14
#define  SOFT_WR_4_S         0x15
#define  SOFT_WR_5_S         0x16
#define  SOFT_WR_6_S         0x17
#define  SOFT_WR_7_S         0x18
#define  SOFT_WR_8_S         0x19
#define  SOFT_WR_9_S         0x1A
#define  SOFT_WR_10_S        0x1B
#define  SOFT_WR_11_S        0x1C
#define  SOFT_WR_12_S        0x1D
#define  SOFT_WR_13_S        0x1E
#define  SOFT_WR_14_S        0x1F

// Read Stream:  ID from 0x35 to 0x3F
#define  SOFT_RD_0_S         0x35
#define  SOFT_RD_1_S         0x36
#define  SOFT_RD_2_S         0x37
#define  SOFT_RD_3_S         0x38
#define  SOFT_RD_4_S         0x39
#define  SOFT_RD_5_S         0x3A
#define  SOFT_RD_6_S         0x3B
#define  SOFT_RD_7_S         0x3C
#define  SOFT_RD_8_S         0x3D
#define  SOFT_RD_9_S         0x3E
#define  SOFT_RD_A_S         0x3F



/* Operation on the stream context */
#define  R_DATA	             0x0        // Read data from stream
#define  W_DATA              0x1        // Write data to stream 
// #define  R_CONTEXT_H         0x2     // Read Context MSB[63:32]
// #define  W_CONTEXT_H         0x3     // Write Context MSB[63:32]
#define  R_CONTEXT_L         0x4        // Read Context LSB[31:0]
#define  W_CONTEXT_L         0x5        // Write Context LSB[31:0]
#define  I_WP                0x6        // Increment Write pointer
#define  I_RP                0x7        // Increment Read pointer
#define  C_WnRP              0x8        // Clear both read and write pointer
#define  R_LAST_DATA         0x9        // Re-read the last word of data


/* Stream status */
#define  S_NEMPTY            0x0        // FIFO has data, not full, not empty
#define  S_EMPTY             0x1        // FIFO is empty
#define  S_FULL	             0x2        // FIFO is full

#ifdef   SC_NEW1_VERSION
#define  S_DONE	             0x3        // Transaction is done, stream is closed
#define  S_WR_DONE           0x4        // Last data is written to the stream buffer.
                                        // Buffer is not empty, 
                                        // i.e. needs ready to empty the buffer to change status to done.
#else
#define  S_DONE	             0x3        // Transaction is done, 'last' is set and FIFO is empty
#define  S_XFR_DONE          0x4        // Transaction is done, 'last' is set and FIFO is not empty
#endif

#define  S_OF                0x5        // FIFO overflow
#define  S_UF                0x6        // FIFO underflow
#define  S_CLOSE             0x7        // Stream is closed
#define  S_ST_MASK           0x7

/* Stream context */
typedef enum Stream_DataValues
{
	// bit[0]
	sBufUse_Internal  = 0x1,        // 1 - this stream is for internal use
	sBufUse_Sdram     = 0x0,        // 0 - this stream is for SDRAM transaction
	
	// bit[1]. It specifies the word size on the DBUS. 
	sDBSize_Byte	  = 0x0,        
	sDBSize_4Byte 	  = 0x2,
	
	// bit[4:2]. This specifies the buffer size. 
	// The buffer pointers needs to be aligned with the buffer size.
	sBufSize_8Byte	  = 0x0,        
	sBufSize_16Byte	  = 0x4,
	sBufSize_32Byte	  = 0x8,
	sBufSize_64Byte	  = 0xC,
	sBufSize_128Byte  = 0x10,
	sBufSize_256Byte  = 0x14,
#ifdef 	SC_NEW1_VERSION
	sBufSize_512Byte  = 0x18,
#endif
	
	// bit[6:5]. It specifies the unit for stream EMPTY status. 
	// A buffer with number of bytes less than this stream unit is treated EMPTY.
	sSBSize_4Byte	  = 0x0, 
	sSBSize_8Byte	  = 0x20,
	sSBSize_16Byte	  = 0x40,
	sSBSize_32Byte	  = 0x60,
	
	// bit[8:7]. It specifies the burst length to trigger request to MC.
	sMBSize_4Byte	  = 0x0, 
	sMBSize_8Byte	  = 0x80,
	sMBSize_16Byte	  = 0x100,
	sMBSize_32Byte	  = 0x180,
	
	// bit[10:9]. 
	sSwap_no	  = 0x0,     // (3,2,1,0)
	sSwap_2Byte	  = 0x200,   // (2,3,0,1)
	sSwap_2Word	  = 0x400,   // (1,0,3,2)
	sSwap_4Byte	  = 0x600,   // (0,1,2,3)

//#ifndef SC_NEW1_VERSION
        // bit[11] 
	sLast_Enable	  = 0x800,   // data in stream FIFO is the last data
	sLast_Disable 	  = 0x0      // data in stream FIFO is not the last data
//#endif
	
}Stream_DataValues;

/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
SC_EXT int flag_vin_y;	
SC_EXT int flag_vin_u;
SC_EXT int flag_vin_v;
SC_EXT int video_in_frame_count;

SC_EXT int flag_vsc_w_y;
SC_EXT int flag_vsc_w_u;
SC_EXT int flag_vsc_w_v;

SC_EXT int gQcifYUVAddr;;

SC_EXT int gImage_Mode;

SC_EXT volatile unsigned int display_no;  //wy:move it here to be a globle varible
/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
int ScInit(int sID, int buf_use, int db_size, \
           int size, int sb_size, int mb_size, \
           int swap, int last, int base_addr);           
int ScOpen(int sID);
int ScRd(int sID, int *data);
int ScWr(int sID, int data);
int ScClose(int sID);
int ScRdStatus(int sID);
int ScRdContext(int sID);
int ScRdBufSize(int sID);
int ScRdBaseAddr(int sID);
int ScRdRPTR(int sID);
int ScRdReadAddr(int sID);
int ScRdWPTR(int sID);    
int ScRdWriteAddr(int sID);       
int ScRdCount(int sID);
int ScRdBlankCount(int sID);
void  ScIntrEn(void);
void  ScIntrEnQcif(void);
void  ScIntrEnVga(void);
void  ScISRVga(void);
void  ScISR(void);
void  ScISRQcif(void);
void PrinfScContext(int sID);
void DebugScContext(int sID);

#endif

