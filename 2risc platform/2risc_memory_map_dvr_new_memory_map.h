/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : risc.h                     
*                                                                         
* Description :    
*             
* Date        : Feb 13, 2006                                                           
***************************************************************************
*/


#include "video_cfg.h"

//#define VIDEO_UCOS_TO_LINUX_2M

// 2-cpu platform memory map, from zxiang's memory_map.h
// first & second interrupt vector table are common to risc0 & risc1
#define EXC000		(0x0)
#define EXC080		(0x80)
#define EXC100		(0x100)	
#define EXC180		(0x180)
#define EXC200		(0x200)
#define EXC280		(0x280)
#define EXC300		(0x300)
#define EXC380		(0x380)
#define EXC400		(0x400)

#define FIRST_VECTOR_B_ADDR    0x80000000

#define FIRST_VECTOR_EXC000		(FIRST_VECTOR_B_ADDR+0x0)
#define FIRST_VECTOR_EXC080		(FIRST_VECTOR_B_ADDR+0x80)
#define FIRST_VECTOR_EXC100		(FIRST_VECTOR_B_ADDR+0x100)	
#define FIRST_VECTOR_EXC180		(FIRST_VECTOR_B_ADDR+0x180)
#define FIRST_VECTOR_EXC200		(FIRST_VECTOR_B_ADDR+0x200)
#define FIRST_VECTOR_EXC280		(FIRST_VECTOR_B_ADDR+0x280)
#define FIRST_VECTOR_EXC300		(FIRST_VECTOR_B_ADDR+0x300)
#define FIRST_VECTOR_EXC380		(FIRST_VECTOR_B_ADDR+0x380)
#define FIRST_VECTOR_EXC400		(FIRST_VECTOR_B_ADDR+0x400)


#define SECOND_VECTOR_B_ADDR    0x80000450

#define SECOND_VECTOR_EXC000		(SECOND_VECTOR_B_ADDR+0x0)
#define SECOND_VECTOR_EXC080		(SECOND_VECTOR_B_ADDR+0x50)
#define SECOND_VECTOR_EXC100		(SECOND_VECTOR_B_ADDR+0x100)	
#define SECOND_VECTOR_EXC180		(SECOND_VECTOR_B_ADDR+0x150)
#define SECOND_VECTOR_EXC200		(SECOND_VECTOR_B_ADDR+0x200)
#define SECOND_VECTOR_EXC280		(SECOND_VECTOR_B_ADDR+0x250)
#define SECOND_VECTOR_EXC300		(SECOND_VECTOR_B_ADDR+0x300)
#define SECOND_VECTOR_EXC380		(SECOND_VECTOR_B_ADDR+0x350)
#define SECOND_VECTOR_EXC400		(SECOND_VECTOR_B_ADDR+0x400)

#define RISC0_THIRD_VECTOR_B_ADDR    0x80000900
#define RISC1_THIRD_VECTOR_B_ADDR    (LINUX_VECTOR_BASEADDR+0x450)

#define RISC0_FOURTH_VECTOR_B_ADDR    0x80001000
#define RISC1_FOURTH_VECTOR_B_ADDR    (LINUX_VECTOR_BASEADDR+0x600)

/*
* SDRAM map
*/
#define VECTOR_B_ADDR 		0x0
#define VECTOR_E_ADDR 		0x10000    //64K
#define LINUX_VECTOR_B_ADDR	0x10000
#define LINUX_VECTOR_E_ADDR	0x20000		//64k
#define BOOTLOADER_B_ADDR       0x20000
#define BOOTLOADER_E_ADDR	0x100000    //960K
#define UCOS_B_ADDR		0x100000
#define UCOS_E_ADDR		0xb00000     //10M for new memory map

#define UCOS_B_ADDR_RISC1		UCOS_E_ADDR
#define UCOS_E_ADDR_RISC1		(UCOS_B_ADDR_RISC1 + (UCOS_E_ADDR - UCOS_B_ADDR))


//#define UCOS_E_ADDR		0x600000     //6M
#define LINUX_B_ADDR		0x900000      
#define LINUX_E_ADDR		0x19d0000
#define ROMFS_B_ADDR		0x19d0000

#ifdef VIDEO_UCOS_TO_LINUX_2M
#define ROMFS_E_ADDR            0x1c94000  //4880K
#define CPU_SHARE_B_ADDR    0xa0020000 // for new memory map 
#else

#define ROMFS_E_ADDR            0x1e14000
#define CPU_SHARE_B_ADDR    0xa0020000 // for new memory map 

#endif

#define CPU_SHARE_E_ADDR	0x1fb6000 //1M+136k
#define Q_B_ADDR				0x1fb6000 //40k
#define Q_E_ADDR				0x1fc0000
#define ETHERNET_B_ADDR		0x1fc0000
#define ETHERNET_E_ADDR		0x1ff0000//0x2000000// 64*3K
#define ETHERNET1_B_ADDR		BOOTLOADER_B_ADDR //0x20000
#define ETHERNET1_E_ADDR		(BOOTLOADER_E_ADDR-0x10000) //64*13K
#define ETH_BLOCK_64K_NUM	16	

#define IDE_B_ADDR			ETHERNET_E_ADDR
#define IDE_E_ADDR			0x2000000

#define AUDIO_MAX_FRAME_LEN	80
#define AUDIO_MAX_FRAME_NUM	10
#define AUDIO_MAX_DATABUF_SIZE  3200
#define AUDIO_MAX_PARABUF_SIZE	0x80
#define AUDIO_UCOS_TO_LINUX_B_ADDR 	(CPU_SHARE_B_ADDR|0xa0000000)
#define AUDIO_UCOS_TO_LINUX_E_ADDR 	(AUDIO_UCOS_TO_LINUX_B_ADDR+AUDIO_MAX_DATABUF_SIZE)
#define AUDIO_LINUX_TO_UCOS_B_ADDR	AUDIO_UCOS_TO_LINUX_E_ADDR
#define AUDIO_LINUX_TO_UCOS_E_ADDR	(AUDIO_LINUX_TO_UCOS_B_ADDR+AUDIO_MAX_DATABUF_SIZE)
#define AUDIO_PARA_B_ADDR  	AUDIO_LINUX_TO_UCOS_E_ADDR
#define AUDIO_PARA_E_ADDR  	(AUDIO_PARA_B_ADDR+AUDIO_MAX_PARABUF_SIZE)
#define AUDIO_UCOS_TO_LINUX_CNT	0
#define AUDIO_LINUX_TO_UCOS_CNT	1

#define SLIC_MAX_DATABUF_SIZE	0x100
#define SLIC_UCOS_TO_LINUX_B_ADDR	AUDIO_PARA_E_ADDR
#define SLIC_UCOS_TO_LINUX_E_ADDR	(SLIC_UCOS_TO_LINUX_B_ADDR+SLIC_MAX_DATABUF_SIZE)

#ifdef VIDEO_UCOS_TO_LINUX_2M
#define VIDEO_MAX_DATABUF_SIZE	0x280000
#else
#define VIDEO_MAX_DATABUF_SIZE	0x92000
#endif

//#define VIDEO_UCOS_TO_LINUX_B_ADDR	0xa0080000 // for new memory map
//extern unsigned int gMpeg4EncodeReconAndVideoShareBuffer[];
#define VIDEO_UCOS_TO_LINUX_B_ADDR    ((unsigned int)gMpeg4EncodeReconAndVideoShareBuffer | 0xa0000000)

#define VIDEO_UCOS_TO_LINUX_E_ADDR	(VIDEO_UCOS_TO_LINUX_B_ADDR+VIDEO_MAX_DATABUF_SIZE)
#define VIDEO_LINUX_TO_UCOS_E_ADDR	(UCOS_E_ADDR_RISC1 | 0xa0000000) // for new memory map, the old one is "(VIDEO_LINUX_TO_UCOS_B_ADDR+VIDEO_MAX_DATABUF_SIZE)"
#define VIDEO_LINUX_TO_UCOS_B_ADDR	(VIDEO_LINUX_TO_UCOS_E_ADDR - VIDEO_MAX_DATABUF_SIZE)    // for new memory map, just for loopback

#define VIDEO_UCOS_TO_LINUX_CNT	2
#define VIDEO_LINUX_TO_UCOS_CNT	3

#define YUV_UCOS_TO_LINUX_CNT	4
#define YUV_LINUX_TO_UCOS_CNT 5

//motion add by zjb Dec 21st   


#define COMM_MAX_COUNTBUF_SIZE 0x500  //5 * 0x100
#define COMM_COUNTBUF_B_ADDR	   SLIC_UCOS_TO_LINUX_E_ADDR    // for new memory map
#define COMM_COUNTBUF_E_ADDR	   (COMM_COUNTBUF_B_ADDR+COMM_MAX_COUNTBUF_SIZE)
#define COMM_SEMAPHORE		0x1

#define COMM_MAX_DATABUF_SIZE 0xa0000 //640K
#define COMM_LINUX_TO_UCOS_B_ADDR	COMM_COUNTBUF_E_ADDR
#define COMM_LINUX_TO_UCOS_E_ADDR	(COMM_LINUX_TO_UCOS_B_ADDR+COMM_MAX_DATABUF_SIZE)
/**********************************************************************************/

#define AUDIO_MAX_VOICEBUF_SIZE  AUDIO_MAX_DATABUF_SIZE
#define AUDIO_MAX_KEYBUF_SIZE	SLIC_MAX_DATABUF_SIZE

#define AUDIO_UCOS_TO_LINUX_VOICEBUF_B_ADDR 	AUDIO_UCOS_TO_LINUX_B_ADDR
#define AUDIO_UCOS_TO_LINUX_VOICEBUF_E_ADDR 	AUDIO_UCOS_TO_LINUX_E_ADDR
#define AUDIO_LINUX_TO_UCOS_VOICEBUF_B_ADDR	AUDIO_LINUX_TO_UCOS_B_ADDR
#define AUDIO_LINUX_TO_UCOS_VOICEBUF_E_ADDR	AUDIO_LINUX_TO_UCOS_E_ADDR
#define AUDIO_UCOS_TO_LINUX_KEYBUF_B_ADDR	SLIC_UCOS_TO_LINUX_B_ADDR
#define AUDIO_UCOS_TO_LINUX_KEYBUF_E_ADDR	SLIC_UCOS_TO_LINUX_E_ADDR

/*********************************************************************************/




#define LINUX_VECTOR_BASEADDR	(LINUX_VECTOR_B_ADDR|0x80000000)
#define VECTOR_EXC000		(LINUX_VECTOR_BASEADDR+0x0)
#define VECTOR_EXC080		(LINUX_VECTOR_BASEADDR+0x80)
#define VECTOR_EXC100		(LINUX_VECTOR_BASEADDR+0x100)	
#define VECTOR_EXC180		(LINUX_VECTOR_BASEADDR+0x180)
#define VECTOR_EXC200		(LINUX_VECTOR_BASEADDR+0x200)
#define VECTOR_EXC280		(LINUX_VECTOR_BASEADDR+0x280)
#define VECTOR_EXC300		(LINUX_VECTOR_BASEADDR+0x300)
#define VECTOR_EXC380		(LINUX_VECTOR_BASEADDR+0x380)
#define VECTOR_EXC400		(LINUX_VECTOR_BASEADDR+0x400)


/*
* the Write Streams memory map (bufsize = 8/16/32/64/128/256/512)
*/
#define WR_STREAM_B_ADDR  0x0
#define WR_STREAM_E_ADDR 0x1000
#define VIN_Y_S_ADDR  (WR_STREAM_E_ADDR - 512)
#define VIN_U_S_ADDR     (VIN_Y_S_ADDR - 512)
#define VIN_V_S_ADDR  (VIN_U_S_ADDR - 512)
#define VSC_WR_Y_S_ADDR  (VIN_V_S_ADDR - 512)
#define VSC_WR_U_S_ADDR  (VSC_WR_Y_S_ADDR - 512)
#define VSC_WR_V_S_ADDR  (VSC_WR_U_S_ADDR - 512)
#define SOFT_WR_0_S_ADDR (VSC_WR_V_S_ADDR - 256) //2cpu communication
#define ETH0_RX_S_ADDR  (SOFT_WR_0_S_ADDR - 128)
#define ETH1_RX_S_ADDR  (ETH0_RX_S_ADDR - 128)
#define PPE_WR_S_ADDR  (ETH1_RX_S_ADDR - 64)
#define AUD_IN_CH0_S_ADDR (PPE_WR_S_ADDR - 32)
#define AUD_IN_CH1_S_ADDR (AUD_IN_CH0_S_ADDR - 32)
#define AUD_IN_CH2_S_ADDR (AUD_IN_CH1_S_ADDR - 32)
#define AUD_IN_CH3_S_ADDR (AUD_IN_CH2_S_ADDR - 32)
#define SEC_WR_S_ADDR  (AUD_IN_CH3_S_ADDR - 32)
#define SOFT_WR_1_S_ADDR (SEC_WR_S_ADDR - 32) //ucos use stream
#define SOFT_WR_2_S_ADDR (SOFT_WR_1_S_ADDR - 32)
#define SOFT_WR_3_S_ADDR (SOFT_WR_2_S_ADDR - 32)
#define SOFT_WR_4_S_ADDR (SOFT_WR_3_S_ADDR - 32)
#define SOFT_WR_5_S_ADDR (SOFT_WR_4_S_ADDR - 32)
#define SOFT_WR_6_S_ADDR (SOFT_WR_5_S_ADDR - 32)
#define SOFT_WR_7_S_ADDR (SOFT_WR_6_S_ADDR - 32)
#define SOFT_WR_8_S_ADDR (SOFT_WR_7_S_ADDR - 32)
#define SOFT_WR_9_S_ADDR (SOFT_WR_8_S_ADDR - 32)
#define SOFT_WR_10_S_ADDR  WR_STREAM_E_ADDR
#define SOFT_WR_11_S_ADDR  WR_STREAM_E_ADDR
#define SOFT_WR_12_S_ADDR  WR_STREAM_E_ADDR
#define SOFT_WR_13_S_ADDR WR_STREAM_E_ADDR
#define SOFT_WR_14_S_ADDR WR_STREAM_E_ADDR

#define DSP_WR_S_ADDR  WR_STREAM_E_ADDR
#define SIF_IN_S_ADDR  WR_STREAM_E_ADDR
#define PIF_IN_S_ADDR  WR_STREAM_E_ADDR

/*
* the Read Streams memory map (bufsize = 8/16/32/64/128/256/512)
*/
#define RD_STREAM_B_ADDR  0x0
#define RD_STREAM_E_ADDR 0x1000
#define VSC_RD_Y0_S_ADDR (RD_STREAM_E_ADDR - 512)
#define VSC_RD_U0_S_ADDR (VSC_RD_Y0_S_ADDR - 512)
#define VSC_RD_V0_S_ADDR (VSC_RD_U0_S_ADDR - 512)
#define VSC_RD_Y1_S_ADDR (VSC_RD_V0_S_ADDR - 512)
#define VSC_RD_U1_S_ADDR (VSC_RD_Y1_S_ADDR - 512)
#define VSC_RD_V1_S_ADDR (VSC_RD_U1_S_ADDR - 512)
#define VSC_RD_OSD_S_ADDR (VSC_RD_V1_S_ADDR - 256)
#define SOFT_RD_0_S_ADDR (VSC_RD_OSD_S_ADDR - 256) //2cpu communication
#define ETH0_TX_S_ADDR  (SOFT_RD_0_S_ADDR - 128)
#define ETH1_TX_S_ADDR  (ETH0_TX_S_ADDR - 128)
#define PPE_RD_S_ADDR  (ETH1_TX_S_ADDR - 64)
#define AUD_OUT_CH0_S_ADDR (PPE_RD_S_ADDR - 32)
#define AUD_OUT_CH1_S_ADDR (AUD_OUT_CH0_S_ADDR - 32)
#define AUD_OUT_CH2_S_ADDR (AUD_OUT_CH1_S_ADDR - 32)
#define AUD_OUT_CH3_S_ADDR (AUD_OUT_CH2_S_ADDR - 32)
#define SEC_RD_S_ADDR  (AUD_OUT_CH3_S_ADDR - 32)
#define SOFT_RD_1_S_ADDR (SEC_RD_S_ADDR - 8) //ppe and ide use stream

#define VOUT_Y_S_ADDR  VSC_RD_Y0_S_ADDR
#define VOUT_U_S_ADDR  VSC_RD_U0_S_ADDR
#define VOUT_V_S_ADDR  VSC_RD_V0_S_ADDR

#define SOFT_RD_2_S_ADDR  RD_STREAM_E_ADDR
#define SOFT_RD_3_S_ADDR  RD_STREAM_E_ADDR
#define SOFT_RD_4_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_5_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_6_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_7_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_8_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_9_S_ADDR RD_STREAM_E_ADDR
#define SOFT_RD_10_S_ADDR RD_STREAM_E_ADDR
#define SIF_OUT_S_ADDR  RD_STREAM_E_ADDR
#define PIF_OUT_S_ADDR  RD_STREAM_E_ADDR
#define DSP_RD_S_ADDR  RD_STREAM_E_ADDR

