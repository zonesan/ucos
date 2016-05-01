/****************************************************************************
*                  Copyright (c) 2006 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File        : mpeg4_encoder.c                     
*                                                                         
* Description : 
*             
* Date        : Feb 8, 2006                                                           
****************************************************************************/
#define MPEG4_ENCODER_GLOBAL


#include "mpeg4_encoder.h"
#include "2risc_memory_map.h"
#include "mpeg4_intr.h"    // by gb

#include "cctl.h"

#include "../video/VIN_API.h"
#include "../bsp/bsp.h"

#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"
#include "two_risc_video.h"
#include "VIN_SET.h"
#include "Sc.h"

//#include "../application/two_risc_display.h"
#include"../inc/video_cfg.h"
#ifdef MV_ENABLE
unsigned int Mv_count;
unsigned int MbCnt;
unsigned int MvFrameCnt;
#endif
int flag_mv_enable=0;

tMPEG4_ENCODER_PARMS mp4EncodeParams;


void mpeg4PerformInited(void)
{
    static int inited = 0;

    if(inited)
        return;
    
    inited = 1;

#ifdef DVR    
    rVCODEC_M4_TOP_CTL = 0x5a;
#else
    rVCODEC_M4_TOP_CTL = 0x5a;
#endif

    //*(volatile unsigned *)0xa8120400 = 0x0000001b;  
}
// TODO: delete
// int Count_CPU_RunTime[4],Count_CPU_RunTime0=0,Count_CPU_RunTime1=0,Count_CPU_RunTime2=0;

/**********************************************************************************************
* mpeg4EncoderSoftReset
*
* Description: Set MPEG-4 soft reset register to reset all status flip-flops before 
*			   encoding process
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderSoftReset(void)
{
    int value;

    //CctlVcodecClkSet();

    mpeg4PerformInited();
    //rVCODEC_M4_TOP_CTL = 0xaa;

    //*(volatile unsigned *)0xa8120400 = 0x0000001b;    

    // Set MPEG-4 soft reset register. 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_SOFT_RESET, rM4EN_SOFT_RST);
    // value = rM4EN_SOFT_RST;
    // *( volatile unsigned *)0xa8120000 = rM4EN_SOFT_RST;          
}

/**********************************************************************************************
* mpeg4EncoderSetCtrl
*
* Description: Set MPEG-4 encoder control registers
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderSetCtrl(ptMPEG4_ENCODER_PARMS pParams)
{
    int value;
        
    // System control register.  
    value = (rM4EN_MV_INTR_MASK<<8) | (rM4EN_FIX_REC_BUF<<7) | (rM4EN_BIG_ENDIAN_VLC<<6) |
            (rM4EN_EXT_STALL_ENABLE<<5) | (rM4EN_FIELD_SRC<<4) | (rM4EN_ENC_FINISH_MASK<<3) |
            (rM4EN_VOP_FINISH_MASK<<2) | (rM4EN_BIG_ENDIAN_SRC<<1) | rM4EN_CLK_INV_IF;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL, value);
    //*( volatile unsigned *)0xa8120008 = value;
        //open by hjm
    writeMpeg4EncoderReg(eMPEG4_ENCODER_STUFFING_CODE, 0x1ff);    // gb added on July 21 
    //*(volatile unsigned int *)0xa812000c = 0x144;           

    // Image width and height. must be multiple of 16.
    writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_WIDTH, pParams->image_width);
    //*( volatile unsigned *)0xa8120010 = pParams->image_width;

extern int gCameraSelectDigital;

    if((gImgEncodeMode == ENCODEBOTH)||(gImage_Mode == MERGED_IMAGE) || (gtVinFormat.interlaced == FALSE))
        writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_HEIGHT, pParams->image_height);
    else
        writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_HEIGHT, pParams->image_height / 2);
    //*( volatile unsigned *)0xa8120014 = pParams->image_height;
  
    // Time_increment_resolution register 
    value = pParams->frame_rate;  // number of ticks within one second 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_INCREMENT_RESOLUTION, value);
    //*( volatile unsigned *)0xa8120018 = value;
      
    // VOL control register 
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_SHORT_HDR_H263<<2) | (rM4EN_RESYNC_DISABLE_H263<<1) | rM4EN_QUAT_TYPE_H263;
    else
        value = (rM4EN_SHORT_HDR<<2) | (rM4EN_RESYNC_DISABLE<<1) | rM4EN_QUAT_TYPE;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOL_CONTROL, value);
     //*( volatile unsigned *)0xa812001c = value;
     
    // VOP control register 1
    //value = (rM4EN_AP_MODE_OFF<<8) | (rM4EN_HALF_EN_REG<<7) | 
    //        (rM4EN_ROUNDING_CONTROL<<6) | (rM4EN_REVERSE<<3) | rM4EN_INTRA_DC_VLC_THR;
    /* Kevin's email on 20061017 */
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_AP_MODE_OFF_H263<<8) | (rM4EN_HALF_EN_REG<<7) | 
                (rM4EN_ROUNDING_CONTROL_H263<<6) | rM4EN_INTRA_DC_VLC_THR_H263;    
    else
        value = (rM4EN_AP_MODE_OFF<<8) | (rM4EN_HALF_EN_REG<<7) | 
                (rM4EN_ROUNDING_CONTROL<<6) | rM4EN_INTRA_DC_VLC_THR;    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL1, value);
    //*( volatile unsigned *)0xa8120020 = value;
    
    // VOP control register 2, first frame should be I frame 
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED_H263<<2);
    else
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED<<2);
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL2, value);
    //*( volatile unsigned *)0xa8120024 = value;
    
    
    // time_increment register 
    value = 0x0;   
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_TIME_INCREMENT, value);
    //*( volatile unsigned *)0xa8120028 = value;

    /*
    // BITSTREAM FRAME WORD COUNT REGISTER
    value = (rM4EN_HEADER_EXT<<16) | rM4EN_FRAME_WORD_CNT;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT, value);
    //*( volatile unsigned *)0xa812002c = value;
    */
    
    //MPEG4_ENCODER_MARKER_CONTROL   register
    value = rM4EN_HEADER_EXT<<16 | rM4EN_VP_WORD;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_MARKER_CONTROL, value);
    //*( volatile unsigned *)0xa8120030 = value;
    // Set source image initial address 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y_INIT_ADDR, pParams->src_Y);
    //*( volatile unsigned *)0xa8120040 = pParams->src_Y;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB_INIT_ADDR, pParams->src_Cb);
    //*( volatile unsigned *)0xa8120044 = pParams->src_Cb;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR_INIT_ADDR, pParams->src_Cr);
    //*( volatile unsigned *)0xa8120048 = pParams->src_Cr;
    // Set reconstruct image initial address 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y0_INIT_ADDR, pParams->recon_Y0);
    //*( volatile unsigned *)0xa812004c = pParams->recon_Y0;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB0_INIT_ADDR, pParams->recon_Cb0);
    //*( volatile unsigned *)0xa8120050 = pParams->recon_Cb0;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR0_INIT_ADDR, pParams->recon_Cr0);
    //*( volatile unsigned *)0xa8120054 = pParams->recon_Cr0;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y1_INIT_ADDR, pParams->recon_Y1);
    //*( volatile unsigned *)0xa8120058 = pParams->recon_Y1;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB1_INIT_ADDR, pParams->recon_Cb1);
    //*( volatile unsigned *)0xa812005c = pParams->recon_Cb1;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR1_INIT_ADDR, pParams->recon_Cr1);
    //*( volatile unsigned *)0xa8120060 = pParams->recon_Cr1;

    // Set output bitstream initial address 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_OUTPUT_INIT_ADDR, pParams->out_bits);
    //*( volatile unsigned *)0xa8120064 = pParams->out_bits;
    
}

/**********************************************************************************************
* mpeg4EncoderSetCtrl1
*
* Description: Set MPEG-4 encoder control registers
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderSetCtrl1(ptMPEG4_ENCODER_PARMS pParams)
{
    int value;
        
    // System control register.  
    value = (rM4EN_MV_INTR_MASK<<8) | (rM4EN_FIX_REC_BUF<<7) | (rM4EN_BIG_ENDIAN_VLC<<6) |
            (rM4EN_EXT_STALL_ENABLE<<5) | (rM4EN_FIELD_SRC<<4) | (rM4EN_ENC_FINISH_MASK<<3) |
            (rM4EN_VOP_FINISH_MASK<<2) | (rM4EN_BIG_ENDIAN_SRC<<1) | rM4EN_CLK_INV_IF;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_SYSTEM_CONTROL, value);
    //*( volatile unsigned *)0xa8120008 = value;
        //open by hjm
    writeMpeg4EncoderReg(eMPEG4_ENCODER_STUFFING_CODE, 0x1ff);    // gb added on July 21 
    //*(volatile unsigned int *)0xa812000c = 0x144;           

    // Image width and height. must be multiple of 16.
    writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_WIDTH, pParams->image_width);
    //*( volatile unsigned *)0xa8120010 = pParams->image_width;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_HEIGHT, pParams->image_height);
    //*( volatile unsigned *)0xa8120014 = pParams->image_height;
  
    // Time_increment_resolution register 
    value = pParams->frame_rate;  // number of ticks within one second 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_INCREMENT_RESOLUTION, value);
    //*( volatile unsigned *)0xa8120018 = value;
      
    // VOL control register 
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_SHORT_HDR_H263<<2) | (rM4EN_RESYNC_DISABLE_H263<<1) | rM4EN_QUAT_TYPE_H263;
    else
        value = (rM4EN_SHORT_HDR<<2) | (rM4EN_RESYNC_DISABLE<<1) | rM4EN_QUAT_TYPE;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOL_CONTROL, value);
    //*( volatile unsigned *)0xa812001c = value;
     
    // VOP control register 1
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_AP_MODE_OFF_H263<<8) | (rM4EN_HALF_EN_REG<<7) | 
                (rM4EN_ROUNDING_CONTROL_H263<<6) | (rM4EN_REVERSE<<3) | rM4EN_INTRA_DC_VLC_THR_H263;    
    else
        value = (rM4EN_AP_MODE_OFF<<8) | (rM4EN_HALF_EN_REG<<7) | 
                (rM4EN_ROUNDING_CONTROL<<6) | (rM4EN_REVERSE<<3) | rM4EN_INTRA_DC_VLC_THR;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL1, value);
    //*( volatile unsigned *)0xa8120020 = value;
    
    // VOP control register 2, first frame should be I frame 
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED_H263<<2);
    else
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED<<2);
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL2, value);
    //*( volatile unsigned *)0xa8120024 = value;
    
    
    // time_increment register 
    value = 0x0;   
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_TIME_INCREMENT, value);
    //*( volatile unsigned *)0xa8120028 = value;

    /*
    // BITSTREAM FRAME WORD COUNT REGISTER
    value = (rM4EN_HEADER_EXT<<16) | rM4EN_FRAME_WORD_CNT;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT, value);
    //*( volatile unsigned *)0xa812002c = value;
    */
    
    //MPEG4_ENCODER_MARKER_CONTROL   register
    value = rM4EN_HEADER_EXT<<16 | rM4EN_VP_WORD;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_MARKER_CONTROL, value);
    //*( volatile unsigned *)0xa8120030 = value;
    // Set source image initial address 
    //writeMpeg4EncoderReg(eMPEG4_ENCODER_Y_INIT_ADDR, pParams->src_Y);
    //*( volatile unsigned *)0xa8120040 = pParams->src_Y;
    
    //writeMpeg4EncoderReg(eMPEG4_ENCODER_CB_INIT_ADDR, pParams->src_Cb);
    //*( volatile unsigned *)0xa8120044 = pParams->src_Cb;
    
    //writeMpeg4EncoderReg(eMPEG4_ENCODER_CR_INIT_ADDR, pParams->src_Cr);
    //*( volatile unsigned *)0xa8120048 = pParams->src_Cr;
    // Set reconstruct image initial address 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y0_INIT_ADDR, pParams->recon_Y0);
    //*( volatile unsigned *)0xa812004c = pParams->recon_Y0;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB0_INIT_ADDR, pParams->recon_Cb0);
    //*( volatile unsigned *)0xa8120050 = pParams->recon_Cb0;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR0_INIT_ADDR, pParams->recon_Cr0);
    //*( volatile unsigned *)0xa8120054 = pParams->recon_Cr0;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y1_INIT_ADDR, pParams->recon_Y1);
    //*( volatile unsigned *)0xa8120058 = pParams->recon_Y1;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB1_INIT_ADDR, pParams->recon_Cb1);
    //*( volatile unsigned *)0xa812005c = pParams->recon_Cb1;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR1_INIT_ADDR, pParams->recon_Cr1);
    //*( volatile unsigned *)0xa8120060 = pParams->recon_Cr1;

    // Set output bitstream initial address 
    //writeMpeg4EncoderReg(eMPEG4_ENCODER_OUTPUT_INIT_ADDR, pParams->out_bits);
    //*( volatile unsigned *)0xa8120064 = pParams->out_bits;
    
}

/**********************************************************************************************
* mpeg4EncoderStart
*
* Description: Start MPEG-4 encoder
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderStart(void)
{       
    int  MC_Counter=0, Value=0, Bit0, Bit9, Bit0_Counter=0 , Bit9_Counter = 0,value;
    const int Bit0_Mask=0x1, Bit9_Mask = 0x200; 
    
    // Set MPEG-4 encoder start registers
    // TODO: need delete
    // This value recode the starting point of the Coding process
    // Count_CPU_RunTime1=GetCount();
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_START, rM4EN_ENC_START);
    //*( volatile unsigned *)0xa8120004 = rM4EN_ENC_START;
    
    
    //    To watch the bit 0xa8020030.0 & the bit 0xa8020030.9 
    
    /*while (1)
    {
        Value= *(volatile unsigned *)(0xa8020030);
        Bit0 = Value & Bit0_Mask;
        Bit9 = Value & Bit9_Mask;
        while(Bit0 == (Value & Bit0_Mask))
         {
            Value= *(volatile unsigned *)(0xa8020030);
            Bit0 = Value & Bit0_Mask;
        }
            Bit0_Counter++;
            
        while(Bit9 == (Value & Bit9_Mask))
         {
            Value= *(volatile unsigned *)(0xa8020030);
            Bit9 = Value & Bit9_Mask;
        }
        
            Bit9_Counter++;    
        
    }*/
    
}

/**********************************************************************************************
* mpeg4EncoderUpdate
*
* Description: Update MPEG-4 encoder parameters to encode a new frame
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderUpdate(ptMPEG4_ENCODER_PARMS pParams)
{
    int value;
	
    // Update VOP Control Register2 
    // VOP control register 2 
    if(gEncodeMode == H263_ENCODE)
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED_H263<<2);
    else
        value = (rM4EN_MODULO_TIME_BASE<<9) | (pParams->quant<<4) | (rM4EN_VOP_CODED<<2);

    // all i frame
    
    if (pParams->isPFrame)
        value |= rM4EN_VOP_TYPE;

    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL2, value);
    //*( volatile unsigned *)0xa8120024 = value;


    // Update VOP time_increment . Kevin's email on 20061017
    value = readMpeg4EncoderReg(eMPEG4_ENCODER_VOP_TIME_INCREMENT);
    value++;
    if(value == 30)
        value = 0;		
    writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_TIME_INCREMENT, value);
    //*( volatile unsigned *)0xa8120028 = value;

    //====================================
    // Update frame rate. Added by hjm
//	writeMpeg4EncoderReg(eMPEG4_ENCODER_INCREMENT_RESOLUTION, 
//											pParams->frame_rate);
    // Update Width and Height. Added by hjm
    // Image width and height. must be multiple of 16.
    //  writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_WIDTH, pParams->image_width);
    //*( volatile unsigned *)0xa8120010 = pParams->image_width;
    
   //	writeMpeg4EncoderReg(eMPEG4_ENCODER_IMAGE_HEIGHT, pParams->image_height);
    //*( volatile unsigned *)0xa8120014 = pParams->image_height;
    //=====================================

    // Set new source image initial address 
    writeMpeg4EncoderReg(eMPEG4_ENCODER_Y_INIT_ADDR, pParams->src_Y);
    //*( volatile unsigned *)0xa8120040 = pParams->src_Y;
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CB_INIT_ADDR, pParams->src_Cb);
    //*( volatile unsigned *)0xa8120044 = pParams->src_Cb;
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_CR_INIT_ADDR, pParams->src_Cr);
    //*( volatile unsigned *)0xa8120048 = pParams->src_Cr;

    // Set new output bitstream initial address  
    writeMpeg4EncoderReg(eMPEG4_ENCODER_OUTPUT_INIT_ADDR, pParams->out_bits);
    //*( volatile unsigned *)0xa8120064 = pParams->out_bits;
   if(flag_mv_enable)
         EnableMotionDetect();
   else
        DisableMotionDetect();
    
}

/**********************************************************************************************
* mpeg4EncoderUpdate
*
* Description: Start MPEG-4 encoder to encode another new frame
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderVopStart(void)
{
    int value;

    // Set MPEG-4 VOP start registers, Start encode next VOP 
    value = rM4EN_VOP_START;
    // TODO: need delete
    // Count_CPU_RunTime1=GetCount();
    writeMpeg4EncoderReg(eMPEG4_ENCODER_START, value);
    //*( volatile unsigned *)0xa8120004 = value;
}

/**********************************************************************************************
* mpeg4EncoderLastStart
*
* Description: Start MPEG-4 encoder to encode the last frame
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderLastVopStart(void)
{
    int value;

    // Set MPEG-4 VOP start registers, Start encode the lasr VOP 
    value = rM4EN_ENC_LAST_VOP;

    writeMpeg4EncoderReg(eMPEG4_ENCODER_START, value);
    //*( volatile unsigned *)0xa8120004 = value;
}

extern unsigned int *gpMpegH263UcosToLinuxShareBuf;
/**********************************************************************************************
* mpeg4EncoderInit
*
* Description: Initialize MPEG-4 encoder 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderInit(void)
{
    int i=0, j=0,COUNT=0;
    //int MV_TEST[350],MvTest = 0;

    // set vcodec clock(encode & decode share the same function!)
    CctlVcodecClkSet();

    // Enable mpeg encode block
    CctlVencBlockEn();

    // hardware reset mpeg encode
    CctlVencReset();

    /* David add Jan 23, 2007 */
    //*(volatile int *)0xa8010038 = 0x180000;
    //*(volatile int *)0xa8010038 = 0x0;
	
    // encoder soft reset
    mpeg4EncoderSoftReset();
    //for(i = 0; i < 1000000; i ++);	

    gMpegEncodeOutBufNo = 0;

    // init global parameters
    gIPFrameCount = 0;
    gI_Frame_Occur_Period = gIntraFrameRate;    // July 20, 2006. Every 5 frames, there will be one I frame!

    // init MPEG4 encoder parameters here
    //mp4EncodeParams.quant =gVideoQuality;
    mp4EncodeParams.frame_rate=  gFrameRate;
    mp4EncodeParams.image_height = gImageHeight;
    mp4EncodeParams.image_width = gImageWidth;
    //mp4EncodeParams.src_Y = ( int )VIN_Y_memory_1 | 0xa0000000;
    //mp4EncodeParams.src_Cb = (int)VIN_V_memory_1 | 0xa0000000;
    //mp4EncodeParams.src_Cr = (int)VIN_U_memory_1 | 0xa0000000;

    mp4EncodeParams.out_bits = ((int)gpMpegH263UcosToLinuxShareBuf) & 0x1fffffff;    // use physical address
    
    mp4EncodeParams.isPFrame =  0;    // the first frame must be I frame
    mp4EncodeParams.recon_Y0 =  RECON_Y0_ADDR;
    mp4EncodeParams.recon_Cb0 = RECON_V0_ADDR;
    mp4EncodeParams.recon_Cr0 = RECON_U0_ADDR;
    mp4EncodeParams.recon_Y1 =  RECON_Y1_ADDR;
    mp4EncodeParams.recon_Cb1 = RECON_V1_ADDR;
    mp4EncodeParams.recon_Cr1 = RECON_U1_ADDR;
    // ...
    
    // set encoder control registers
    mpeg4EncoderSetCtrl(&mp4EncodeParams);

   if(flag_mv_enable)
         EnableMotionDetect();
   else
        DisableMotionDetect();
   
    // Start encoder
    mpeg4EncoderStart();    
}

/**********************************************************************************************
* mpeg4EncoderInit1
*
* Description: Initialize MPEG-4 encoder 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
extern unsigned int AnalogCameraRatio;
void mpeg4EncoderInit1(void)
{
    int i=0, j=0,COUNT=0;
    //int MV_TEST[350],MvTest = 0;

    // set vcodec divide value
    CctlVcodecClkSet();

    // Enable VENC block
    CctlVencBlockEn();
	
    // encoder soft reset
    mpeg4EncoderSoftReset();

    gMpegEncodeOutBufNo = 0;

    // init global parameters
    gIPFrameCount = 0;
    gI_Frame_Occur_Period = gIntraFrameRate;    // July 20, 2006. Every 5 frames, there will be one I frame!

    // init MPEG4 encoder parameters here
    //mp4EncodeParams.quant =gVideoQuality;                  
    mp4EncodeParams.frame_rate=  gFrameRate;

    mp4EncodeParams.image_height = gImageHeight;
    mp4EncodeParams.image_width = gImageWidth;

/*
    switch(AnalogCameraRatio) {
        case 0:
            mp4EncodeParams.image_height = gImageHeight;
            mp4EncodeParams.image_width = gImageWidth;
            break;

        case 1:
            mp4EncodeParams.image_height = gImageHeight / 2;
            mp4EncodeParams.image_width = gImageWidth /2;
            break;

        case 2:    
            mp4EncodeParams.image_height = gImageHeight / 4;
            mp4EncodeParams.image_width = gImageWidth /4;
            break;

        default:
            mp4EncodeParams.image_height = gImageHeight;
            mp4EncodeParams.image_width = gImageWidth;
            break;            
    }
*/
    
    //mp4EncodeParams.src_Y = ( int )VIN_Y_memory_1 | 0xa0000000;
    //mp4EncodeParams.src_Cb = (int)VIN_V_memory_1 | 0xa0000000;
    //mp4EncodeParams.src_Cr = (int)VIN_U_memory_1 | 0xa0000000;

    mp4EncodeParams.out_bits = ((int)gpMpegH263UcosToLinuxShareBuf) & 0x1fffffff;    // use physical address
    
    mp4EncodeParams.isPFrame =  0;    // the first frame must be I frame
    mp4EncodeParams.recon_Y0 =  RECON_Y0_ADDR;
    mp4EncodeParams.recon_Cb0 = RECON_V0_ADDR;
    mp4EncodeParams.recon_Cr0 = RECON_U0_ADDR;
    mp4EncodeParams.recon_Y1 =  RECON_Y1_ADDR;
    mp4EncodeParams.recon_Cb1 = RECON_V1_ADDR;
    mp4EncodeParams.recon_Cr1 = RECON_U1_ADDR;
    // ...

    
    // set encoder control registers
    mpeg4EncoderSetCtrl(&mp4EncodeParams);

    // Start encoder
    mpeg4EncoderStart();                
}

/**********************************************************************************************
* MVExtractInterruptHandler
*
* Description: On receiving MV Interrupt, extract MPEG-4 Encoder Motion Vector
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void MVExtractInterruptHandler(void)
{
    // Read MV information register to check current macroblock ID and motion vector

    /*****************************************************************************************
    *????Here, I don't know the use of this instruction. and when we have read out the current * 
    *Macroblock ID and motion vector, then take them for what use?                             *
    *******************************************************************************************/                         

    // Read MV interrupt register to check MV updating overflow and clear MV interrupt register. 
    
    readMpeg4EncoderReg(eMPEG4_ENCODER_MV_INTERRUPT);   //Just read it?
    
}



/**********************************************************************************************
* HwMpeg4EncoderLastVopStart
*
* Description: Start MPEG-4 encoder to encode the last new frame
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/

void HwMpeg4EncoderLastVopStart(void)
{
    int value;

    // Set MPEG-4 VOP start registers, Start encode next VOP 
    
    value = (rM4EN_ENC_LAST_VOP<<2) | rM4EN_VOP_START;
    
    
    writeMpeg4EncoderReg(eMPEG4_ENCODER_START, value);
    //*( volatile unsigned *)0xa8120004 = value;
}

/**********************************************************************************************
* mpeg4EncoderInterruptHandler
*
* Description: On receiving vop_finish interrupt, update encode parameters and start
*               encoding a new frame 
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
**********************************************************************************************/
void mpeg4EncoderInterruptHandler(void)
{
    int regValue, code_word_cnt;
    static int j = 0;
    /************Below is the origin code writen by Daniel******/

    // check encoded word number              
    regValue = readMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT);
    code_word_cnt = regValue & 0x0ffff;
        
    /*// update encode parameters
    mpeg4EncoderUpdate(&mp4EncodeParams);

    // start encoding a new frame
    mpeg4EncoderVopStart();*/
    /*************************my code************************/
    //printf("OK,MP4_intr succeed!!\n");
    //printf("MP4 encoder frame: No. %d\n!", gIPFrameCount);
    //printf("Frame_Word_Count: 0x%x\n", *(volatile unsigned*)0xa812002c);
    // clear interrupt
    // writeMpeg4EncoderReg(eMPEG4_ENCODER_INTERRUPT, 0x0001);
    mpeg4EncodeIntrClr();

    if ((++gIPFrameCount % gI_Frame_Occur_Period) != 0)
        mp4EncodeParams.isPFrame =  1; 
    else
        mp4EncodeParams.isPFrame = 0;
    
    mp4EncodeParams.src_Y = mp4EncodeParams.src_Y + FRAME_LENGTH;
    mp4EncodeParams.src_Cb = mp4EncodeParams.src_Cb + FRAME_LENGTH;
    mp4EncodeParams.src_Cr = mp4EncodeParams.src_Cr + FRAME_LENGTH;
    mp4EncodeParams.out_bits += (readMpeg4EncoderReg(eMPEG4_ENCODER_FRAME_WORD_COUNT) & 0xffff)*4;
    j += code_word_cnt *4;
    mpeg4EncoderUpdate( &mp4EncodeParams);


    /* by gb on May.27 */
    if (gIPFrameCount < 59) {
        if (gIPFrameCount % 30 == 0)
            writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL2, 0x3f4);
        delay(50,50);
        mpeg4EncoderVopStart(); 
    }

    if (gIPFrameCount == 59) {
//        if (gIPFrameCount % 30 == 0)
//            writeMpeg4EncoderReg(eMPEG4_ENCODER_VOP_CONTROL2, 0x3f4);
              delay(50,50);
        HwMpeg4EncoderLastVopStart();
    }
    
    // mpeg4EncoderIntrClr();
    // mpeg4EncoderVopStart(); 
}

