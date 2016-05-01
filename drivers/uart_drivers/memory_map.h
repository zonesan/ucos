#ifndef __memory_map_h__
#define __memory_map_h__


#define V_BASE_ADDR	0xa0000000

/*
* Address table
*/

/* 
* Global adress space
* SDRAM/reg/queue/stream
*/
#define GLOBAL_B_ADDR 0x00000000
#define GLOBAL_E_ADDR 0x10000000

/* The SDRAM:64Mbytes */
#define SDRAM_B_ADDR 0x00000000
#define SDRAM_E_ADDR 0x04000000

/* The SDRAM queues
* bit[15:8]:queue ID 
* bit[19:16]:extended video stream context ID,bit16=1 for UV,bit16=0 for Y
*/
#define QUEUE_B_ADDR 0x04000000
#define QUEUE_E_ADDR 0x08000000
#define QUEUE_ADDR(queueID,videoID)	(V_BASE_ADDR+QUEUE_B_ADDR+(queueID<<8)+(videoID<<16))

/* The on chip registers 
* bit[20:16]:block ID
* bit[15:0]:local register address of each block
*/
#define REGS_B_ADDR 0x08000000
#define REGS_E_ADDR 0x0c000000

//Block ID
#define RISC_BLOCK	0x0
#define CCTL_BLOCK 	0x1
#define MC_BLOCK 	0x2
#define SIF_BLOCK 	0x3
#define SC_BLOCK	0x4
#define VIN_BLOCK	0x5
#define VOUT_BLOCK	0x6
#define AUD_BLOCK	0x7
#define DMAC_BLOCK	0x8
#define PPE_BLOCK	0x9
#define PIF_BLOCK	0xA
#define VSC_BLOCK	0xB
#define EMAC0_BLOCK	0xC
#define EMAC1_BLOCK	0xD
#define SEC_BLOCK	0xE
#define UART_BLOCK	0xF
#define SD0_BLOCK	0x10
#define IDE_BLOCK	0x11
#define VCODEC_BLOCK	0x12
#define SD1_BLOCK	0x13
#define BLOCK_BASE_ADDR(BLOCK_ID) (V_BASE_ADDR+REGS_B_ADDR+(BLOCK_ID<<16))

/* The streams
* bit[15:8]:stream ID
* bit[19:16]:steam operation code 
* bit[23]:stream write last */
#define STREAM_B_ADDR 0x0c000000
#define STREAM_E_ADDR 0x10000000

#define STREAM_ADDR(streamID,operation)	(V_BASE_ADDR+STREAM_B_ADDR+(operation<<16)+(streamID<<8))
#define STREAM_ADDR_NEW(streamID,operation,last) (STREAM_ADDR(streamID,operation)+last<<23)
/* 
* boot address space
* Boot space maps to boot sources
*/
#define BOOT_B_ADDR 0x1fc00000
#define BOOT_E_ADDR 0x20000000

/* 
* Internal SPRAM address:2Mbyte 
* RISC internal SPRAM 
*/
#define I_SPRAM_B_ADDR 0x1fff8000
#define I_SPRAM_E_ADDR 0x20000000

/* 
* External SPRAM address:2Mbyte 
* RISC external SPRAM
*/
#define E_SPRAM_B_ADDR 0x1fa00000
#define E_SPRAM_E_ADDR 0x1fc00000

/*
* SDRAM map
*/
#define VECTOR_B_ADDR 		0x0
#define VECTOR_E_ADDR 		0x10000    //64K
#define BOOTLOADER_B_ADDR    	0x10000
#define BOOTLOADER_E_ADDR	0x100000    //960K
#define UCOS_B_ADDR		0x100000
#define UCOS_E_ADDR		0x300000     //2M
#define LINUX_B_ADDR		0x300000      
#define LINUX_E_ADDR		0xE2A000	
#define ROMFS_B_ADDR		0xE2A000
#define ROMFS_E_ADDR		0x1500000	//7000K
#define QUEUES_B_ADDR		0x1500000
#define QUEUES_E_ADDR		0x2000000

#define AUDIO_B_ADDR		0x1500000
#define AUDIO_E_ADDR		0x1508000
#define CPU_SHARE_B_ADDR	0x1508000
#define CPU_SHARE_E_ADDR	0x1510000
#define ETHERNET_B_ADDR		0x1510000
#define ETHERNET_E_ADDR		0x1700000
#define VIDEO_B_ADDR		0x1700000
#define VIDEO_E_ADDR		0x1900000

/*
* Stream memory map
*/



#endif
