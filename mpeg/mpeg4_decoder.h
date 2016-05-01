/*
*********************************************************************************************************                 
*   
* File:          mpeg4_decoder.H             
*                                                                         
* DESCRIPTION :                                                         
*                                                                           
*********************************************************************************************************
*/


#ifndef __MPEG4_DECODER_H
#define __MPEG4_DECODER_H

#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#include "def_type.h"
#include "risc.h"
#include "../inc/chip_cfg.h"

/*
*********************************************************************************************************
*                          The values is to be changed by users to meet different requirements
*********************************************************************************************************
*/
// MPEG-4 SOFT RESET REGISTER (0X00)
#define  	rM4DE_SOFT_RST			0x1         /* 0: Reset MPEG-4 decoder except registers */

// MPEG-4 START REGISTER (0X04) 
#define  	rM4DE_LAST_BLOCK		0x1         /* 0: Current bitstream block to be decode is not last bitstream 
													  block. After current bitstream block has been decoded 
													  completely, MPEG-4 decoder will wait for next block_start to 
													  decode next bitstream block 
												   1: Current bitstream block to be decode is last bitstream block. 
													  After current bitstream block has been decoded completely, 
													  MPEG-4 decoder will stop decode. */
#define  	rM4DE_BLOCK_START  		0x01         /* 0: do nothing 
												   1: start to decode bitstream in bitstream block. */
#define  	rM4DE_REO_START       	0x1         /* 0: do nothing 
												   1: Start Re-order function (start to decode next frame). */
#define  	rM4DE_MP4_START       	0x1         /* 0: do nothing 1: Start MPEG-4 decoder.  */
// Example: 
// Write 0x0F => Start to decode current bitstream block and the bitstream block is last one. 
// Write 0x07 => Start to decode current bitstream block and the bitstream block is not last one. 
// Write 0x02 => Start to decode next frame. 
// Write 0x0C => Start to decode next bitstream block and the bitstream block is last one. 



// MPEG-4 FUNCTION REGISTER (0X0C)
#define  	rM4DE_IPT_STR_INV    	0x1         /*  0: Little-endian byte order 1: Big-endian byte order */
#define  	rM4DE_DEBLK_EN    		0x1         /*  0: Disable de-blocking 1: Enable de-blocking  */
#define  	rM4DE_DEBLK_DIS    		0x0         /*  0: Disable de-blocking 1: Enable de-blocking  */
#define  	rM4DE_SEL_REO			0x0         /*  0: De-blocking path re-order 1: Non de-blocking path re-order*/
#define 	rM4DE_EN_REO			0x1         /* 0: Disable re-order 1: Enable re-order  */
#define 	rM4DE_REO_BYTE_SWP		0x1			/* 0: Little-endian byte order 1: Big-endian byte order */
#define		rM4DE_REO_YUV  			0x1			/* 0: YCbYCr 422 format 1: YCbCr 420 format */
#define		rM4DE_REO_FIELD			0x0			/* (422 format) 0: Progressive mode 1: Interlace mode */
#define		rM4DE_REO_TYPE			0x02			/* Output Order (422 format)
														00: YCbYCr 
													        01: YCrYCb 
														10: CbYCrY 
														11: CrYCbY */
												 
// MPEG-4 SOFT RESET REGISTER (0X20)

#define  	rM4DE_MP4_STOP			0x0         /* ??? 0: Stop MPEG-4 Decoder 1: Stop MPEG-4 Decoder */
extern unsigned int YUV_Decoder_I420[];
// Structure for read only registers

// MPEG4 INTERRUPT REGISTER (0X24) 
struct StructDecIntReg
{
	ULONG rev:29;		 
	ULONG block_end:1;	// All bitstream in current decoded bitstream block has been decoded 
	ULONG reo_okay:1;	// Re-order completes one Frame. 
	ULONG mp4_finish:1;	// MPEG-4 decoder operation complete 
};

// MPEG4 TIME INFORMATION REGISTER (0X30) 
struct StructDecTimeInfoReg
{
	ULONG rev:7;		 
	ULONG time_code_valid:1;	// Indicate the time_code is valid or not 
	ULONG m_base:8;	// indicates the number of seconds elapsed since the previous decoded frame 
	ULONG t_inc:16;	// Each frame has one vop_time_increment number which indicates display time. 
};

// MPEG4 TIME CODE REGISTER (0X34)
struct StructDecTimeCodeReg
{
	ULONG rev:15;		 
	ULONG time_code_hours:5;	// The value indicates the time elapsed since the decoder starts 
	ULONG time_code_minutes:6;	
	ULONG time_code_seconds:6;	
};

/*
*********************************************************************************************************
*                          MPEG4 encoder register definition
*********************************************************************************************************
*/
#define MPEG4_DECODER_BASE_ADDR    	BLOCK_BASE_ADDR(VCODEC_BLOCK)+0x200   // FIXME, need to use MPEG4_DECODE_BLOCK


enum MPEG4_DECODER_REG_ADDR
{
	eMP4_DEC_SOFT_RESET				= 0x0,  
	eMP4_DEC_START    				= 0x4, 
	eMP4_DEC_FUNCTION				= 0x0C,   
	eMP4_DEC_MOTION_COMP_BASE_ADDR	= 0x10,
	eMP4_DEC_OUT_BUF_BASE_ADDR    	= 0x14,  
	eMP4_DEC_BIT_BLK_INIT_ADDR		= 0x18,  
	eMP4_DEC_BIT_BLK_END_ADDR		= 0x1C,  
	eMP4_DEC_STOP					= 0x20,   
	eMP4_DEC_INTERRUPT				= 0x24,   
	eMP4_DEC_RESOLUTION				= 0x28,   
	eMP4_DEC_TIME_INCRE_RESOL		= 0x2C,   
	eMP4_DEC_TIME_INFO				= 0x30,   
	eMP4_DEC_TIME_CODE				= 0x34,   
	eMP4_DEC_MOT_COMP_CB0_OFFSET	= 0x40,
	eMP4_DEC_MOT_COMP_CR0_OFFSET	= 0x44,
	eMP4_DEC_MOT_COMP_Y1_OFFSET		= 0x48,
	eMP4_DEC_MOT_COMP_CB1_OFFSET	= 0x4C,
	eMP4_DEC_MOT_COMP_CR1_OFFSET	= 0x50,
	eMP4_DEC_OUT_BUF_CB_OFFSET		= 0x54,
	eMP4_DEC_OUT_BUF_CR_OFFSET		= 0x58,
};

#define writeMpeg4DecoderReg(addr, value)  (*(volatile unsigned *)(MPEG4_DECODER_BASE_ADDR + addr) = value)   
#define readMpeg4DecoderReg(addr)		   (*(volatile unsigned *)(MPEG4_DECODER_BASE_ADDR + addr))   


typedef struct MPEG4_DECODER_PARMS 
{ 
	ULONG ipt_iaddr;		// input bitstream block initial address
	ULONG ipt_eaddr;		// input bitstream block end address
	ULONG mc_iaddr;			// motion compensation base address
	ULONG mc_cb0_offset;	// motion compensation CB0 offset address
	ULONG mc_cr0_offset;	// motion compensation CR0 offset address
	ULONG mc_y1_offset;		// motion compensation Y1 offset address
	ULONG mc_cb1_offset;	// motion compensation CB1 offset address
	ULONG mc_cr1_offset;	// motion compensation CR1 offset address
	ULONG opt_iaddr;		// output buffer base address
	ULONG opt_cb_offset;	// output buffer CB offset address
	ULONG opt_cr_offset;	// output buffer CR offset address
	LONG vop_width;
	LONG vop_height;
	LONG time_inc_resolu; 
	LONG time_inc;
}tMPEG4_DECODER_PARMS, *ptMPEG4_DECODER_PARMS;

// FIXME, need to define these addresses
#define IPT_IADDR	YUV_Decoder_I420		//Input bitstream block initial address		
#define IPT_EADDR	YUV_Decoder_I420 + 0x13174	//Input bitstream block end address			
#define MC_IADDR	0xa0700000	//MPEG-4 MC Base Address   Start address for MPEG-4 decoder motion compensation.			
#define MC_CB0_OFFSET	0x0200	//MPEG-4 mc offset address for CB0			
#define MC_CR0_OFFSET	0x0300				
#define MC_Y1_OFFSET	0x0400				
#define MC_CB1_OFFSET	0x0600				
#define MC_CR1_OFFSET	0x0700				
#define OPT_IADDR		0xa0400000				
#define OPT_CB_OFFSET	0x02D				
#define OPT_CR_OFFSET	0x039	
#define BLOCK_END_INTERRUPT_MASK  0X04
#define REO_OKEY_INTERRUPT_MASK   0X02
#define OPT_ONE_FRAME_SPACE  0x4500


void mpeg4DecoderInit(void);

#endif

#endif

