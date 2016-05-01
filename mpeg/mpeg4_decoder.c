/****************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : mpeg4_decoder.c                     
*                                                                         
* Description : 
*             
* Date        : Feb 17, 2006                                                           
****************************************************************************/



#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "../mpeg/mpeg4_decoder.h"
#include "two_risc_display.h"

#include "comm.h"

#include "cctl.h"

tMPEG4_DECODER_PARMS mp4DecodeParams;

#ifdef DVR
unsigned int gDecodeDeblocking = 0x0;    //the default is disable de-blocking for DVR
#else
unsigned int gDecodeDeblocking = 0x1;
#endif

/**********************************************************************************************
* mpeg4DecoderSoftReset
*
* Description: Set MPEG-4 soft reset register to reset all status flip-flops before 
*               decoding process
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderSoftReset(void)
{
    //*(volatile unsigned *)0xa8010030 = 0x80000000;    // CCTL
    //*(volatile unsigned *)0xa8120400 = 0x0000001b;
    
    // Set MPEG-4 soft reset register  
    writeMpeg4DecoderReg(eMP4_DEC_SOFT_RESET, rM4DE_SOFT_RST);
}

/**********************************************************************************************
* mpeg4DecoderSetFunction
*
* Description: Set MPEG-4 decoder function registers. The order of register 
*                setting is facultative
*
* Arguments  : 
*                ipt_iaddr:        input bitstream block initial address
*                ipt_eaddr:        input bitstream block end address
*                mc_iaddr:        motion compensation base address
*                mc_cb0_offset:    motion compensation CB0 offset address
*                mc_cr0_offset:    motion compensation CR0 offset address
*                mc_y1_offset:    motion compensation Y1 offset address
*                mc_cb1_offset:    motion compensation CB1 offset address
*                mc_cr10_offset:    motion compensation CR1 offset address
*                opt_iaddr:        output buffer base address
*                opt_cb_offset:    output buffer CB offset address
*                opt_cr_offset:    output buffer CR offset address
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderSetFunction(ptMPEG4_DECODER_PARMS pParams)
{
    int value;

    // Set input bitstream block initial and end address
    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, pParams->ipt_iaddr);
    //*( volatile unsigned *)0xa8120218 = pParams->ipt_iaddr;
    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, pParams->ipt_eaddr);
    //*( volatile unsigned *)0xa812021c = pParams->ipt_eaddr;
    
    // Set MC base and offset address
    writeMpeg4DecoderReg(eMP4_DEC_MOTION_COMP_BASE_ADDR, pParams->mc_iaddr);
    //*( volatile unsigned *)0xa8120210 = pParams->mc_iaddr;
    writeMpeg4DecoderReg(eMP4_DEC_MOT_COMP_CB0_OFFSET, pParams->mc_cb0_offset);
       //*( volatile unsigned *)0xa8120240 = pParams->mc_cb0_offset;    
    writeMpeg4DecoderReg(eMP4_DEC_MOT_COMP_CR0_OFFSET, pParams->mc_cr0_offset);
    //*( volatile unsigned *)0xa8120244 = pParams->mc_cr0_offset;
    writeMpeg4DecoderReg(eMP4_DEC_MOT_COMP_Y1_OFFSET, pParams->mc_y1_offset);
    //*( volatile unsigned *)0xa8120248 = pParams->mc_y1_offset;
    writeMpeg4DecoderReg(eMP4_DEC_MOT_COMP_CB1_OFFSET, pParams->mc_cb1_offset);
    //*( volatile unsigned *)0xa812024c = pParams->mc_cb1_offset;
    writeMpeg4DecoderReg(eMP4_DEC_MOT_COMP_CR1_OFFSET, pParams->mc_cr1_offset);
    //*( volatile unsigned *)0xa8120250 = pParams->mc_cr1_offset;

    // Set output buffer base and offset address
    writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_BASE_ADDR, pParams->opt_iaddr);
    //*( volatile unsigned *)0xa8120214 = pParams->opt_iaddr;
    writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_CB_OFFSET, pParams->opt_cb_offset);
    //*( volatile unsigned *)0xa8120254 = pParams->opt_cb_offset;
    writeMpeg4DecoderReg(eMP4_DEC_OUT_BUF_CR_OFFSET, pParams->opt_cr_offset);
    //*( volatile unsigned *)0xa8120258 = pParams->opt_cr_offset;

    // Set MPEG-4 decoder function register
    if (1 == gDecodeDeblocking) {
    value = (rM4DE_IPT_STR_INV<<8) | (rM4DE_DEBLK_EN<<7) | (rM4DE_SEL_REO<<6) |
            (rM4DE_EN_REO<<5) | (rM4DE_REO_BYTE_SWP<<4) | (rM4DE_REO_YUV<<3) |
            (rM4DE_REO_FIELD<<2) | rM4DE_REO_TYPE;
    } else if (0 == gDecodeDeblocking) {
        value = (rM4DE_IPT_STR_INV<<8) | (rM4DE_DEBLK_DIS<<7) | (rM4DE_SEL_REO<<6) |
                (rM4DE_EN_REO<<5) | (rM4DE_REO_BYTE_SWP<<4) | (rM4DE_REO_YUV<<3) |
                (rM4DE_REO_FIELD<<2) | rM4DE_REO_TYPE;
    } else {
        printf("\nmpeg4DecoderSetFunction:gDecodeDeblocking error!\n");
    }
    writeMpeg4DecoderReg(eMP4_DEC_FUNCTION, value);
    //*( volatile unsigned *)0xa812020c = value;
}

/**********************************************************************************************
* mpeg4DecoderStart
*
* Description: Start MPEG-4 decoder
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderStart(void)
{
    int value;

    // Set MPEG-4 decoder start registers
    //value = (rM4DE_LAST_BLOCK<<3)|(rM4DE_BLOCK_START<<2) | (rM4DE_REO_START<<1) | rM4DE_MP4_START;
    //writeMpeg4DecoderReg(eMP4_DEC_START, value);
    *( volatile unsigned *)0xa8120204 =0x0f;     //value;
    
}

/**********************************************************************************************
* mpeg4DecoderSetFunction
*
* Description: Set "New" input bitstream bolck initial address register and "New" input 
*                bitstream bolck end address register to arrange new bitstream block.
*
* Arguments  : 
*                ipt_iaddr:        input bitstream block initial address
*                ipt_eaddr:        input bitstream block end address
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderSetBlockFunc(ptMPEG4_DECODER_PARMS pParams)  
{
    // Set NEW input bitstream block initial and end address
    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_INIT_ADDR, pParams->ipt_iaddr);
    //*( volatile unsigned *)0xa8120218 = pParams->ipt_iaddr;
    writeMpeg4DecoderReg(eMP4_DEC_BIT_BLK_END_ADDR, pParams->ipt_eaddr);
    //*( volatile unsigned *)0xa812021c = pParams->ipt_eaddr;
    
}

/**********************************************************************************************
* mpeg4DecoderStart
*
* Description: Set MPEG-4 block_start register to start reading new bitstream block for 
*                decoding process. The interrupt register of block_end will be cleared at 
*                the same time.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderStartBlock(void)
{
    int value;

    // Set MPEG-4 decoder start registers
    value = (rM4DE_BLOCK_START<<2);
    writeMpeg4DecoderReg(eMP4_DEC_START, value);
    //value = rM4DE_BLOCK_START;
    //writeMpeg4DecoderReg(eMP4_DEC_START, value);
    //*( volatile unsigned *)0xa8120204 = value;
}

/**********************************************************************************************
* mpeg4DecoderSetFunction
*
* Description: After the interrupt register of reo_okay is pulled up, one of the output 
*                buffers will be filled with the new frame (VOP) 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderReadVideoInfo(ptMPEG4_DECODER_PARMS pParams)
{
    ULONG value;

    // Read VOP width and VOP height register for deciding display video size 
    value = readMpeg4DecoderReg(eMP4_DEC_RESOLUTION);
    pParams->vop_width = value>>16;
    pParams->vop_height = value & 0x0ffff;

    // Read VOP time increment resolution for display time control 
    value = readMpeg4DecoderReg(eMP4_DEC_TIME_INCRE_RESOL);
    pParams->time_inc_resolu = value & 0x0ffff;

    // Read VOP time increment for display time control. 
    value = readMpeg4DecoderReg(eMP4_DEC_TIME_INFO);

    
}

/**********************************************************************************************
* mpeg4DecoderStart
*
* Description: Set reo_start register to start decoding next frame. The interrupt register 
*                of reo_okay will be cleared at the same time.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderStartReOder(void)
{
    int value;

    // Set reo_start register
    value = (rM4DE_REO_START<<1);
    writeMpeg4DecoderReg(eMP4_DEC_START, value);
    //*( volatile unsigned *)0xa8120204 = value;
}

            

/**********************************************************************************************
* mpeg4DecoderInit
*
* Description: Initialize MPEG-4 decoder 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4DecoderInit(void)
{
    int i=1, FrameCount=0;

    // set vcodec clock(encode & decode share the same function!)
    CctlVcodecClkSet();

    // Enable VDEC block
    CctlVdecBlockEn();
    
    // decoder reset
    CctlVdecReset();
    logprintf(DEBUG_MESSAGE, MP4_DECODE, "S$,");
    mpeg4PerformInited();
    
//  mpeg4DecoderSoftReset();

    // init MPEG4 decoder parameters
    //mp4DecodeParams.ipt_iaddr = IPT_IADDR; 
    //mp4DecodeParams.ipt_eaddr = IPT_EADDR; 
    mp4DecodeParams.mc_iaddr = ((unsigned int)gMpeg4DecodeMcIaddr) & 0x1fffffff;

    mp4DecodeParams.mc_cb0_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT) >> 8; 
    mp4DecodeParams.mc_cr0_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4) >> 8;
    mp4DecodeParams.mc_y1_offset  = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4 + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4) >> 8; 
    mp4DecodeParams.mc_cb1_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4 + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4 + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT) >> 8;
    mp4DecodeParams.mc_cr1_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4 + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4 + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT / 4) >> 8;    

            switch(gMpeg4DecodeOutputBufNo) {
                case 0:				
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr0) & 0x1fffffff; 					
                    break;					
					
                case 1:
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr1) & 0x1fffffff; 					
                    break;	

                case 2:
                    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr2) & 0x1fffffff; 					
                    break;					
					
                default:
                    printf("*");					
                    break;
					
            }		
/*
    gMpeg4DecodeOutputBufNo = 0;
    mp4DecodeParams.opt_iaddr = ((unsigned int)gMpeg4DecodeOptIaddr0) & 0x1fffffff;
*/
	
    /* for 420 */
    mp4DecodeParams.opt_cb_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT) >> 8; 
    mp4DecodeParams.opt_cr_offset = (MAX_MPEG_DECODE_WIDTH * MAX_MPEG_DECODE_HEIGHT + MPEG_DECODE_OUTPUT_UV_SIZE) >> 8;

    /* for 422 
    mp4DecodeParams.opt_cb_offset = (640 * 480) >> 8; 
    mp4DecodeParams.opt_cr_offset = (640 * 480 + 640 * 480 / 2) >> 8;
    */

    // decoder Set Function
    mpeg4DecoderSetFunction(&mp4DecodeParams);

    // Start decoder
    //mpeg4DecoderStart();

    return;	
}
#endif
