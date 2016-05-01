/*
*********************************************************************************************************                 
*   
* File:          mpeg4_encoder.H             
*                                                                         
* DESCRIPTION :                                                         
*                                                                           
*********************************************************************************************************
*/
#ifndef MPEG4_ENCODER_H
#define MPEG4_ENCODER_H

#ifdef MPEG4_ENCODER_GLOBAL
#define MPEG4_ENCODER_EXTERN 
#else
#define MPEG4_ENCODER_EXTERN    extern
#endif

#include "def_type.h"
#include "risc.h"
#include "../video_data/saved_vin_data.h"
#include "../inc/chip_cfg.h"
#include "../inc/video_cfg.h"

#include  "2risc_memory_map.h"

/*
*********************************************************************************************************
*                          The values is to be changed by users to meet different requirements
*********************************************************************************************************
*/
// MPEG-4 SOFT RESET REGISTER (0X00)
#define  rM4EN_SOFT_RST            0x1         /* 1: Reset MPEG-4 encoder except registers */

// MPEG-4 START REGISTER (0X04) 
#define  rM4EN_ENC_LAST_VOP     0x6         /* 1: Index the frame is last VOP(Frame) */
#define  rM4EN_VOP_START           0x2         /* 1: Start to encode next VOP(Frame), which is not first VOP(Frame) */
#define  rM4EN_ENC_START           0x1         /* 1: Start MPEG-4 encoder and encode first VOP(Frame) */
// Example: Write 0x05 => Start to encode first VOP and the first VOP is last VOP. 
//            Write 0x02 => Start to encode next VOP and the VOP is not last VOP. 

// MPEG-4 SYSTEM CONTROL REGISTER (0X08)   mv_intr_mask
// TODO: enable MV if needed
extern int flag_mv_enable;
//#define  rM4EN_MV_INTR_MASK        0x0         /*  0: Enable MV interrupt 1: Disable MV interrupt */
#define  rM4EN_MV_INTR_MASK        0x1         /*  0: Enable MV interrupt 1: Disable MV interrupt */

#define  rM4EN_FIX_REC_BUF         0x0         /*  0: Automatically reconstruct buffer(double buffer) select 
 													                          1: Fixedly reconstruct buffer(double buffer) select */

#define  rM4EN_BIG_ENDIAN_VLC      0x1        /*  0: Output bitstream little endian (Byte order) , 

                                                    1: Output bitstream Big endian (Byte order), for ipcam */

#define  rM4EN_EXT_STALL_ENABLE    0x0         /* 0: Disable external stall signal 1: Enable external stall signal */
#define  rM4EN_FIELD_SRC           0x0         /* 0: Source image is not single field 1: Source image is single field*/
#define  rM4EN_ENC_FINISH_MASK     0x0         /* 0: Enable enc_finish interrupt 1: Disable enc_finish interrupt */
#define  rM4EN_VOP_FINISH_MASK     0x0         /* 0: Enable vop_finish interrupt 1: Disable vop_finish interrupt */
#define  rM4EN_BIG_ENDIAN_SRC      0x1         /* 0: Source image little endian 1: Source image Big endian */
#define  rM4EN_CLK_INV_IF          0x0         /* 0: Encoded bitstream clock(bs_clk) not inverse 
                                                  1: Encoded bitstream clock (bs_clk) inverse */
// MPEG-4 STUFFING CODE REGISTER (0X0C) 
#define  rM4EN_VOP_WORD_END        0x0         /* 0: Bitstream of each VOP is byte(8 bits) aligned ending. 
                                                  1: Bitstream of each VOP is word(32 bits) aligned ending. */
#define  rM4EN_STUFFING_CODE       0x7F        /* The Stuffing code which is used to filled the bitstream when 
                                                  the bitstream is not word aligned ending */
// MPEG-4 VOL CONTROL REGISTER (0X1C) 
#define  rM4EN_SHORT_HDR           0x0         /* 0: Disable short header 1: Enable short header (suggestion disable) */
#define  rM4EN_RESYNC_DISABLE      0x0         /* 0: Enable resync marker 1: Disable resync marker. If short header is 
                                                      enabled, this register must set to 1 (suggestion disable) */
#define  rM4EN_QUAT_TYPE           0x0         /* 0: H.263 Method (Compliant with MPEG-4 Simple Profile) 
                                                  1: MPEG-2 Method If short header is enabled, this register 
                                                      must set to 0. (suggestion H.263 Method)  */

/*the following two macros is special for H.263 encode and the macros terminated by
*_h263 is for H263 encode 
*/
#define  rM4EN_SHORT_HDR_H263           0x1         /* 0: Disable short header 1: Enable short header (suggestion disable) */
#define  rM4EN_RESYNC_DISABLE_H263      0x1         /* 0: Enable resync marker 1: Disable resync marker. If short header is 
                                                      enabled, this register must set to 1 (suggestion disable) */
#define  rM4EN_QUAT_TYPE_H263           0x0         /* 0: H.263 Method (Compliant with MPEG-4 Simple Profile) 
                                                  1: MPEG-2 Method If short header is enabled, this register 
                                                      must set to 0. (suggestion H.263 Method)  */


/*there are TWO macros should be changed when different encode mode is choosen
*they are:rM4EN_AP_MODE_OFF
*              rM4EN_ROUNDING_CONTROL
*/
// MPEG-4 VOP CONTROL REGISTER 1(0X20)
#define  rM4EN_AP_MODE_OFF         0x0         /* 0: Enable 4MV mode 1: Disable 4MV mode If short header is enabled, 
                                                      this register must set to 1. (suggestion enable) */
#define  rM4EN_HALF_EN_REG         0x1         /*  0: Disable half pixel motion estimation 1: Enable half pixel 
                                                      motion estimation (suggestion enable) */ 
#define  rM4EN_ROUNDING_CONTROL    0x1         /* 0: Disable rounding control 1: Enable rounding control. If short 
                                                      header is enabled, this register must set to 0. (suggestion enable) */
#define  rM4EN_REVERSE             0x2         /* ???? test script bit4=1, why???? */
#define  rM4EN_INTRA_DC_VLC_THR    0x0         /*  The register indicates the DC VLC coding method according 
                                                    to quant parameter(QP) 
                                                   000: Use Intra DC VLC for entire VOP 
                                                   001: Switch to Intra AC VLC when QP >=13 
                                                   010: Switch to Intra AC VLC when QP >=15 
                                                   011: Switch to Intra AC VLC when QP >=17 
                                                   100: Switch to Intra AC VLC when QP >=19 
                                                   101: Switch to Intra AC VLC when QP >=21 
                                                   110: Switch to Intra AC VLC when QP >=23 
                                                   111: Use Intra AC VLC for entire VOP 
                                                    If short header is enabled, this register must set to 0. 
                                                    (suggestion set 0) */
#define  rM4EN_AP_MODE_OFF_H263         0x1         /* 0: Enable 4MV mode 1: Disable 4MV mode If short header is enabled, 
                                                      this register must set to 1. (suggestion enable) */
#define  rM4EN_ROUNDING_CONTROL_H263    0x0         /* 0: Disable rounding control 1: Enable rounding control. If short 
                                                      header is enabled, this register must set to 0. (suggestion enable) */
#define  rM4EN_INTRA_DC_VLC_THR_H263    0x0         /*  The register indicates the DC VLC coding method according 
                                                    to quant parameter(QP) 
                                                   000: Use Intra DC VLC for entire VOP 
                                                   001: Switch to Intra AC VLC when QP >=13 
                                                   010: Switch to Intra AC VLC when QP >=15 
                                                   011: Switch to Intra AC VLC when QP >=17 
                                                   100: Switch to Intra AC VLC when QP >=19 
                                                   101: Switch to Intra AC VLC when QP >=21 
                                                   110: Switch to Intra AC VLC when QP >=23 
                                                   111: Use Intra AC VLC for entire VOP 
                                                    If short header is enabled, this register must set to 0. 
                                                    (suggestion set 0) */


// MPEG-4 VOP CONTROL REGISTER 2(0X24) 
#define  rM4EN_MODULO_TIME_BASE    0x0         /*  0: modulo_time_base = 0 
                                                   1: modulo_time_base = 1 
                                                   2: modulo_time_base = 2 
                                                   3: modulo_time_base = 3 
                                                   4: modulo_time_base = 4 
                                                   5: modulo_time_base = 5 
                                                   6: modulo_time_base = 6 
                                                   7: modulo_time_base = 7 
                                                   The register indicates how many second elapsed since the previous 
                                                    encoded frame. The register is useless while short header is enabled. */
#define  rM4EN_VOP_QUANT           0x7         /* 5-bit unsigned integer taking values from 1 to 31 for quantisation*/
#define  rM4EN_VOP_CODED           0x1         /* 0: VOP not coded 1: VOP coded */
#define  rM4EN_VOP_TYPE            0x1         /* 0: I-VOP 1: P-VOP */

//for H263 encode mode add by xyx on Dec.4th 2007
#define  rM4EN_VOP_CODED_H263           0x1         /* 0: VOP not coded 1: VOP coded */

// MPEG-4 BITSTREAM FRAME WORD COUNT REGISTER (0X2C) 
#define  rM4EN_STUFFING_BYTE_CNT   0x0         /* 3-bit taking values from 0 to 3 which indicates the 
                                                  byte(8 bits) number of stuffing_code filled by encoder.*/

#define  rM4EN_FRAME_WORD_CNT      0x0fff      /* The word(32 bits) number of encoded bitstream 
                                                  within current encoded VOP */
// MPEG-4 BITSTREAM FRAME WORD COUNT REGISTER (0X30) 
#define  rM4EN_HEADER_EXT          0x0         /* 0: Disable header extension code 1: Enable header extension code */
#define  rM4EN_VP_WORD             0x0ffff     /* Maximum Video Packet Word Count */

// MPEG-4 INTERRUPT REGISTER (0X3C) 
#define  rM4EN_ENC_FINISH          0x1         /* 1: enable encode finish interrupt */
#define  rM4EN_VOP_FINISH          0x1         /* 1: enable VOP finish interrupt */

#define  FRAME_WORD_COUNT_MASK     0xffff

/*
*********************************************************************************************************
*                          MPEG4 encoder register definition
*********************************************************************************************************
*/
#define  MPEG4_ENCODER_BASE_ADDR  BLOCK_BASE_ADDR(VCODEC_BLOCK)  // FIXME, need to use MPEG4_ENCODE_BLOCK

#define  VCODEC_BASE_ADDR    BLOCK_BASE_ADDR(VCODEC_BLOCK)

#define  rVCODEC_M4_TOP_CTL       *(volatile unsigned *)(VCODEC_BASE_ADDR + 0x400)
#define  rVCODEC_M4E_RANGE_CTL    *(volatile unsigned *)(VCODEC_BASE_ADDR + 0x404)
#define  rVCODEC_M4D_RANGE_CTL    *(volatile unsigned *)(VCODEC_BASE_ADDR + 0x408)

extern unsigned int startup_picture_yuv[];

enum MPEG4_ENCODER_REG_ADDR
{
    eMPEG4_ENCODER_SOFT_RESET               = 0x0,  
    eMPEG4_ENCODER_START                    = 0x4, 
    eMPEG4_ENCODER_SYSTEM_CONTROL           = 0x8,   
    eMPEG4_ENCODER_STUFFING_CODE            = 0x0C,   
    eMPEG4_ENCODER_IMAGE_WIDTH              = 0x10,
    eMPEG4_ENCODER_IMAGE_HEIGHT             = 0x14,  
    eMPEG4_ENCODER_INCREMENT_RESOLUTION     = 0x18,  
    eMPEG4_ENCODER_VOL_CONTROL              = 0x1C,  
    eMPEG4_ENCODER_VOP_CONTROL1             = 0x20,   
    eMPEG4_ENCODER_VOP_CONTROL2             = 0x24,   
    eMPEG4_ENCODER_VOP_TIME_INCREMENT       = 0x28,   
    eMPEG4_ENCODER_FRAME_WORD_COUNT         = 0x2C,   
    eMPEG4_ENCODER_MARKER_CONTROL           = 0x30,   
    eMPEG4_ENCODER_MV_INFO                  = 0x34,   
    eMPEG4_ENCODER_MV_INTERRUPT             = 0x38,
    eMPEG4_ENCODER_INTERRUPT                = 0x3C,
    eMPEG4_ENCODER_Y_INIT_ADDR              = 0x40,   
    eMPEG4_ENCODER_CB_INIT_ADDR             = 0x44,   
    eMPEG4_ENCODER_CR_INIT_ADDR             = 0x48,     
    eMPEG4_ENCODER_Y0_INIT_ADDR             = 0x4C,   
    eMPEG4_ENCODER_CB0_INIT_ADDR            = 0x50,   
    eMPEG4_ENCODER_CR0_INIT_ADDR            = 0x54,   
    eMPEG4_ENCODER_Y1_INIT_ADDR             = 0x58,   
    eMPEG4_ENCODER_CB1_INIT_ADDR            = 0x5C,   
    eMPEG4_ENCODER_CR1_INIT_ADDR            = 0x60,   
    eMPEG4_ENCODER_OUTPUT_INIT_ADDR         = 0x64,
};

#define  writeMpeg4EncoderReg(addr, value)  (*(volatile unsigned *)(MPEG4_ENCODER_BASE_ADDR + addr) = value)   
#define  readMpeg4EncoderReg(addr)          (*(volatile unsigned *)(MPEG4_ENCODER_BASE_ADDR + addr))   


typedef struct MPEG4_ENCODER_PARMS 
{ 
    LONG frame_rate;
    LONG quant; 
    BOOL isPFrame;
    LONG image_width; 
    LONG image_height; 
    ULONG src_Y; 
    ULONG src_Cb; 
    ULONG src_Cr; 
    ULONG recon_Y0; 
    ULONG recon_Cb0; 
    ULONG recon_Cr0;             
    ULONG recon_Y1; 
    ULONG recon_Cb1; 
    ULONG recon_Cr1; 
    ULONG out_bits;
}tMPEG4_ENCODER_PARMS, *ptMPEG4_ENCODER_PARMS;


MPEG4_ENCODER_EXTERN int gIPFrameCount;
MPEG4_ENCODER_EXTERN int gI_Frame_Occur_Period;  // for example, if gI_Frame_Occur_Period = 5, then one I frame & four P frames;
                                                // if gI_Frame_Occur_Period = 1, then each frame is I frame.
                                                // gI_Frame_Occur_Period should not be zero!    


#define  FRAME_Y_SIZE    (MAX_MPEG_ENCODE_WIDTH * MAX_MPEG_ENCODE_HEIGHT)
#define  FRAME_UV_SIZE  (MAX_MPEG_ENCODE_WIDTH * MAX_MPEG_ENCODE_HEIGHT / 4)

#define  OUT_BITSTREAM_ADDR   0xA1000000

#ifdef NEW_MEMORY_MAP
extern unsigned int gMpeg4EncodeReconAndVideoShareBuffer[];
#define gMpeg4EncodeReconBase    ((unsigned int)gMpeg4EncodeReconAndVideoShareBuffer + VIDEO_MAX_DATABUF_SIZE)
#else
extern unsigned int gMpeg4EncodeReconBase[MPEG_ENCODE_RECON_BUFSIZE / sizeof(int)];
#endif

#define  RECON_Y0_ADDR  ( ((unsigned int)gMpeg4EncodeReconBase) & 0x1fffffff)    // use physical addr
#define  RECON_U0_ADDR  (RECON_Y0_ADDR +  FRAME_Y_SIZE)
#define  RECON_V0_ADDR  (RECON_U0_ADDR +  FRAME_UV_SIZE)
#define  RECON_Y1_ADDR  (RECON_V0_ADDR +  FRAME_UV_SIZE)
#define  RECON_U1_ADDR  (RECON_Y1_ADDR +  FRAME_Y_SIZE)
#define  RECON_V1_ADDR  (RECON_U1_ADDR +  FRAME_UV_SIZE)


#define  FRAME_RATE             30                                  //frame rate from the camera, and software controlled
#define INTRA_FRAME_RATE  30 
#define  VOP_FINISH_MASK        0x01
#define  ENC_FINISH_MASK        0x02
#define  FRAME_LENGTH           (FRAME_Y_SIZE * 3 / 2)
#define  MV_INTR_REGISTER_MASK  0x03

void mpeg4EncoderSoftReset(void);
void mpeg4EncoderSetCtrl(ptMPEG4_ENCODER_PARMS pParams);
void mpeg4EncoderSetCtrl1(ptMPEG4_ENCODER_PARMS pParams);
void mpeg4EncoderStart(void);
void mpeg4EncoderUpdate(ptMPEG4_ENCODER_PARMS pParams);
void mpeg4EncoderVopStart(void);
void mpeg4EncoderLastVopStart(void);
void mpeg4EncoderInterruptHandler(void);
void mpeg4EncoderInit(void);
void mpeg4EncoderInitQcif(void);
void mpeg4EncoderInit1(void);
void MVExtractInterruptHandler(void);
void EnableMotionDetect(void);
void DisableMotionDetect(void);




extern tMPEG4_ENCODER_PARMS mp4EncodeParams;

#endif
