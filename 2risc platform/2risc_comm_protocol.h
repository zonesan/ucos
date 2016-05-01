/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        :                                                      
***************************************************************************
*/



#ifndef __2RISC_COMM_PROTOCOL_H_
#define __2RISC_COMM_PROTOCOL_H_

//#define RPACKET_LEN  64
//#define RMESSAGE_LEN 52
#define RPACKET_LEN  32
#define RMESSAGE_LEN 20

/* Stream IDs */
#define RISC1_TX  0x11 //RISC1 -> Stream -> RISC0
#define RISC0_RX  0x11 //RISC1 -> Stream -> RISC0
#define RISC0_TX  0x35 //RISC0 -> Stream -> RISC1	
#define RISC1_RX  0x35 //RISC0 -> Stream -> RISC1	

#define AUD_AUTOFETCH_CH0_S    SOFT_RD_1_S
#define AUD_AUTOFETCH_CH1_S    SOFT_RD_2_S
#define AUD_AUTOFETCH_CH2_S    SOFT_RD_3_S
#define AUD_AUTOFETCH_CH3_S    SOFT_RD_4_S

// tmp use
#define AUDIO_OUT_CH0_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 2)
#define AUDIO_IN_CH0_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 4)

#define AUDIO_OUT_CH1_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 6)
#define AUDIO_IN_CH1_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 8)

#define AUDIO_OUT_CH2_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 10)
#define AUDIO_IN_CH2_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 12)

#define AUDIO_OUT_CH3_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 14)
#define AUDIO_IN_CH3_M_ADDR  (UCOS_E_ADDR - CHANNEL_BUFFER_SIZE * 16)

#define AUD_AUTOFETCH_CH0_S_ADDR    SOFT_RD_1_S_ADDR
#define AUD_AUTOFETCH_CH1_S_ADDR    SOFT_RD_2_S_ADDR
#define AUD_AUTOFETCH_CH2_S_ADDR    SOFT_RD_3_S_ADDR
#define AUD_AUTOFETCH_CH3_S_ADDR    SOFT_RD_4_S_ADDR
/*
#define AUDIO_OUT_CH0_M_ADDR  ((unsigned int)(two_risc_audio_output[0]) & 0x1fffffff)
#define AUDIO_OUT_CH1_M_ADDR  ((unsigned int)(two_risc_audio_output[1]) & 0x1fffffff)
#define AUDIO_OUT_CH2_M_ADDR  ((unsigned int)(two_risc_audio_output[2]) & 0x1fffffff)
#define AUDIO_OUT_CH3_M_ADDR  ((unsigned int)(two_risc_audio_output[3]) & 0x1fffffff)

#define AUDIO_IN_CH0_M_ADDR  ((unsigned int)(two_risc_audio_input[0]) & 0x1fffffff)
#define AUDIO_IN_CH1_M_ADDR  ((unsigned int)(two_risc_audio_input[1]) & 0x1fffffff)
#define AUDIO_IN_CH2_M_ADDR  ((unsigned int)(two_risc_audio_input[2]) & 0x1fffffff)
#define AUDIO_IN_CH3_M_ADDR  ((unsigned int)(two_risc_audio_input[3]) & 0x1fffffff)

#define AUD_AUTOFETCH_CH0_S_ADDR    SOFT_RD_1_S_ADDR
#define AUD_AUTOFETCH_CH1_S_ADDR    SOFT_RD_2_S_ADDR
#define AUD_AUTOFETCH_CH2_S_ADDR    SOFT_RD_3_S_ADDR
#define AUD_AUTOFETCH_CH3_S_ADDR    SOFT_RD_4_S_ADDR
*/

/* Risc communication semaphores */
#define SEM_LINUX_OK    0
#define SEM_DVR       1
#define SEM_UART0       3
#define SEM_VIDEOPHONE       1

#define SEM_UCOS_AUDIO_READY    0

#define LINUX_TO_UCOS_SEMAPHORE 1
#define UCOS_TO_LINUX_SEMAPHORE 2

typedef struct
{
	unsigned char start;
	unsigned char main_type;
	unsigned char sub_type;
	unsigned char length;
	unsigned int status;
	unsigned char message[RMESSAGE_LEN];
	unsigned int end;
}Risc_Packet_t;

#define RISC_PKT_START  ':'
#define RISC_PKT_END	0xffffffff

#define RISC_PKT_MAX_MAINTYPE 0x5
typedef enum tRiscPktMainType
{
	tRiscComm	= 0x1,	
	tAudio		= 0x2, 
	tSys		= 0x3,
	tVideo		= 0x4,
	tDVR		= 0x5,	
}tRiscPktMainType;

#endif

