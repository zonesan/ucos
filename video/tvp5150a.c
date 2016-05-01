/*
***************************************************************************
*                   Copyright (c) 2005 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : Tvp5150a.c             
*                                                                         
* Description : Functions for Tvp5150a      
*                                                
* By          :              
*
* Date        : Dec.13, 2007                                                           
***************************************************************************
*/
#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "comm.h"
#include "os_cpu.h"
#include "two_risc_comm.h"
#include "sun_type_defs.h"

#include "tvp5150a.h"
#include "iic.h"
#include "Camera.h"

//register configuration for NTSC
unsigned char tvp5150a_initialization_ntsc[] =
{
	//---------normal register setting-----------------
0x02,0x40,     //operation mode controls register.bit6 color burst reference for AGC enabled.the rest default
0x03,0x6F,     //miscellaneous control register.bit6 GPCL output 1.bit7 GPCL is output.
0x04,0x3c,     //autoswitch mask register.does not switch to NTSC443,PAL-N,PAL-M,SECAM.
0x07,0x60, // julim.luminance processing control register.default setting.
//0x09,0x8B,
0x09,0x80,             //brightness default setting.
0x0a,0x80, //julim  color saturation control register default setting.
0x0b,0x00, // julim hue defaulting setting.
//0x0c,0xA0,
0x0c,0x80,            //contrast default setting.
//julim0x0d,0x00,
0x0d,0x07, // output bt656. output and dataf rates select register.bit6 ITU-R BT.601 coding range
0x0f,0x02,             //configuration shared pins register.bit3 FID.bit1 GPCL or VBLK dependin gon bit 7 of register 03h
//julim        0x11,0xff,
//julim        0x12,0x02,
0x11,0x02,
0x12,0x00,
0x15,0x15,        //genlock and RTC register.bit5-4 standard.bit2-0 RTC output mode 1
//0x16,0x8f, //julim
//0x18,0xf0, //julim
//0x19,0x0f, //julim
0x1a,0x1c,     //chrominance control #1 register.bit4 color PLL reset
0x25,0x01,
0x27,0x60,      //test mode
0xc0,0xc0       //interrupt status register A

//		0x11,0x00 Active Video Cropping Start Pixel MSB REgister
//		0x12,0x00 Active Video Cropping Start Pixel LSB REgister
//		0x13,0x3f // Active Video Cropping Stop Pixel MSB REgister
//		0x14,0x03 // Active Video Cropping Stop Pixel LSB REgister

//		0x18,00 Vertical Blanking Start Register
//		0x19,00 Vertical Blanking Stop Register
};

//register configuration for PAL
unsigned char tvp5150a_initialization_pal[] =
{

};

//brightness
unsigned char tvp5150a_brightness_volatile_0[] =
{
0x09,0x40
};

unsigned char tvp5150a_brightness_volatile_1[] =
{
0x09,0x50
};

unsigned char tvp5150a_brightness_volatile_2[] =
{
0x09,0x60
};

unsigned char tvp5150a_brightness_volatile_3[] =
{
0x09,0x70
};

unsigned char tvp5150a_brightness_volatile_4[] =
{
0x09,0x80
};

unsigned char tvp5150a_brightness_volatile_5[] =
{
0x09,0x90
};

unsigned char tvp5150a_brightness_volatile_6[] =
{
0x09,0xa0
};

unsigned char tvp5150a_brightness_volatile_7[] =
{
0x09,0xb0
};

unsigned char tvp5150a_brightness_volatile_8[] =
{
0x09,0xc0
};

unsigned char tvp5150a_brightness_volatile_9[] =
{
0x09,0xd0
};

//contrast
unsigned char tvp5150a_contrast_volatile_0[] =
{
0x0c,0x40
};

unsigned char tvp5150a_contrast_volatile_1[] =
{
0x0c,0x50
};

unsigned char tvp5150a_contrast_volatile_2[] =
{
0x0c,0x60
};

unsigned char tvp5150a_contrast_volatile_3[] =
{
0x0c,0x70
};

unsigned char tvp5150a_contrast_volatile_4[] =
{
0x0c,0x80
};

unsigned char tvp5150a_contrast_volatile_5[] =
{
0x0c,0x90
};

unsigned char tvp5150a_contrast_volatile_6[] =
{
0x0c,0xa0
};

unsigned char tvp5150a_contrast_volatile_7[] =
{
0x0c,0xb0
};

unsigned char tvp5150a_contrast_volatile_8[] =
{
0x0c,0xc0
};

unsigned char tvp5150a_contrast_volatile_9[] =
{
0x0c,0xd0
};

//hue
unsigned char tvp5150a_hue_volatile_0[] =
{
0x0a,0xb0
};

unsigned char tvp5150a_hue_volatile_1[] =
{
0x0a,0xa0
};

unsigned char tvp5150a_hue_volatile_2[] =
{
0x0a,0x90
};

unsigned char tvp5150a_hue_volatile_3[] =
{
0x0a,0x80
};

unsigned char tvp5150a_hue_volatile_4[] =
{
0x0a,0x00
};

unsigned char tvp5150a_hue_volatile_5[] =
{
0x0a,0x10
};

unsigned char tvp5150a_hue_volatile_6[] =
{
0x0a,0x20
};

unsigned char tvp5150a_hue_volatile_7[] =
{
0x0a,0x30
};

unsigned char tvp5150a_hue_volatile_8[] =
{
0x0a,0x40
};

unsigned char tvp5150a_hue_volatile_9[] =
{
0x0a,0x50
};

//saturation
unsigned char tvp5150a_saturation_volatile_0[] =
{
0x0a,0x40
};

unsigned char tvp5150a_saturation_volatile_1[] =
{
0x0a,0x50
};

unsigned char tvp5150a_saturation_volatile_2[] =
{
0x0a,0x60
};

unsigned char tvp5150a_saturation_volatile_3[] =
{
0x0a,0x70
};

unsigned char tvp5150a_saturation_volatile_4[] =
{
0x0a,0x80
};

unsigned char tvp5150a_saturation_volatile_5[] =
{
0x0a,0x90
};

unsigned char tvp5150a_saturation_volatile_6[] =
{
0x0a,0xa0
};

unsigned char tvp5150a_saturation_volatile_7[] =
{
0x0a,0xb0
};

unsigned char tvp5150a_saturation_volatile_8[] =
{
0x0a,0xc0
};

unsigned char tvp5150a_saturation_volatile_9[] =
{
0x0a,0xd0
};

//sharpness
unsigned char tvp5150a_sharpness_volatile_0[] =
{
0x08,0x00
};

unsigned char tvp5150a_sharpness_volatile_1[] =
{
0x08,0x01
};

unsigned char tvp5150a_sharpness_volatile_2[] =
{
0x08,0x02
};

unsigned char tvp5150a_sharpness_volatile_3[] =
{
0x08,0x03
};


/*
*********************************************************************************************
*                                  SetTvp5150aBrightness
*
* Description:  set the brightness fo tvp51150a   
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void SetTvp5150aBrightness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetTVP5150aBrightness: gtCameraSetting.brightness = %d, ", gtCameraSetting.brightness);

    switch(gtCameraSetting.brightness) {
        case 0:     // 0
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_0) / sizeof ( tvp5150a_brightness_volatile_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_0[i], tvp5150a_brightness_volatile_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_1) / sizeof ( tvp5150a_brightness_volatile_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_1[i], tvp5150a_brightness_volatile_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_2) / sizeof ( tvp5150a_brightness_volatile_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_2[i], tvp5150a_brightness_volatile_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_3) / sizeof ( tvp5150a_brightness_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_3[i], tvp5150a_brightness_volatile_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_4) / sizeof ( tvp5150a_brightness_volatile_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_4[i], tvp5150a_brightness_volatile_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_5) / sizeof ( tvp5150a_brightness_volatile_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_5[i], tvp5150a_brightness_volatile_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_6) / sizeof ( tvp5150a_brightness_volatile_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_6[i], tvp5150a_brightness_volatile_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_7) / sizeof ( tvp5150a_brightness_volatile_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_7[i], tvp5150a_brightness_volatile_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_8) / sizeof ( tvp5150a_brightness_volatile_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_8[i], tvp5150a_brightness_volatile_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_9) / sizeof ( tvp5150a_brightness_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_9[i], tvp5150a_brightness_volatile_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(tvp5150a_brightness_volatile_9) / sizeof ( tvp5150a_brightness_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_brightness_volatile_9[i], tvp5150a_brightness_volatile_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  SetTvp5150aContrast
*
* Description:  set the contrast fo tvp51150a
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void SetTvp5150aContrast(void)
{
    int i;
    int reg_volatile_num;

    printf("SetTVP5150aContrast: gtCameraSetting.contrast = %d, ", gtCameraSetting.contrast);

    switch(gtCameraSetting.contrast) {
        case 0:     // 0
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_0) / sizeof ( tvp5150a_contrast_volatile_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_0[i], tvp5150a_contrast_volatile_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_1) / sizeof ( tvp5150a_contrast_volatile_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_1[i], tvp5150a_contrast_volatile_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_2) / sizeof ( tvp5150a_contrast_volatile_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_2[i], tvp5150a_contrast_volatile_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_3) / sizeof ( tvp5150a_contrast_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_3[i], tvp5150a_contrast_volatile_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_4) / sizeof ( tvp5150a_contrast_volatile_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_4[i], tvp5150a_contrast_volatile_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_5) / sizeof ( tvp5150a_contrast_volatile_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_5[i], tvp5150a_contrast_volatile_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_6) / sizeof ( tvp5150a_contrast_volatile_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_6[i], tvp5150a_contrast_volatile_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_7) / sizeof ( tvp5150a_contrast_volatile_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_7[i], tvp5150a_contrast_volatile_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_8) / sizeof ( tvp5150a_contrast_volatile_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_8[i], tvp5150a_contrast_volatile_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_9) / sizeof ( tvp5150a_contrast_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_9[i], tvp5150a_contrast_volatile_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(tvp5150a_contrast_volatile_9) / sizeof ( tvp5150a_contrast_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_contrast_volatile_9[i], tvp5150a_contrast_volatile_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  SetTvp5150aHue
*
* Description:  set the hue fo tvp51150a
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void SetTvp5150aHue(void)
{
    int i;
    int reg_volatile_num;

    printf("SetTVP5150aHue: gtCameraSetting.hue = %d, ", gtCameraSetting.hue);

    switch(gtCameraSetting.hue) {
        case 0:     // 0
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_0) / sizeof ( tvp5150a_hue_volatile_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_0[i], tvp5150a_hue_volatile_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_1) / sizeof ( tvp5150a_hue_volatile_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_1[i], tvp5150a_hue_volatile_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_2) / sizeof ( tvp5150a_hue_volatile_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_2[i], tvp5150a_hue_volatile_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_3) / sizeof ( tvp5150a_hue_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_3[i], tvp5150a_hue_volatile_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_4) / sizeof ( tvp5150a_hue_volatile_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_4[i], tvp5150a_hue_volatile_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_5) / sizeof ( tvp5150a_hue_volatile_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_5[i], tvp5150a_hue_volatile_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_6) / sizeof ( tvp5150a_hue_volatile_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_6[i], tvp5150a_hue_volatile_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_7) / sizeof ( tvp5150a_hue_volatile_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_7[i], tvp5150a_hue_volatile_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_8) / sizeof ( tvp5150a_hue_volatile_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_8[i], tvp5150a_hue_volatile_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_9) / sizeof ( tvp5150a_hue_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_9[i], tvp5150a_hue_volatile_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(tvp5150a_hue_volatile_9) / sizeof ( tvp5150a_hue_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_hue_volatile_9[i], tvp5150a_hue_volatile_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  SetTvp5150aSaturation
*
* Description:  set the saturation fo tvp51150a   
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void SetTvp5150aSaturation(void)
{
    int i;
    int reg_volatile_num;

    printf("SetTVP5150aSaturation: gtCameraSetting.saturation = %d, ", gtCameraSetting.saturation);

    switch(gtCameraSetting.saturation) {
        case 0:     // 0
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_0) / sizeof ( tvp5150a_saturation_volatile_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_0[i], tvp5150a_saturation_volatile_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_1) / sizeof ( tvp5150a_saturation_volatile_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_1[i], tvp5150a_saturation_volatile_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_2) / sizeof ( tvp5150a_saturation_volatile_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_2[i], tvp5150a_saturation_volatile_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_3) / sizeof ( tvp5150a_saturation_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_3[i], tvp5150a_saturation_volatile_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_4) / sizeof ( tvp5150a_saturation_volatile_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_4[i], tvp5150a_saturation_volatile_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_5) / sizeof ( tvp5150a_saturation_volatile_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_5[i], tvp5150a_saturation_volatile_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_6) / sizeof ( tvp5150a_saturation_volatile_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_6[i], tvp5150a_saturation_volatile_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_7) / sizeof ( tvp5150a_saturation_volatile_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_7[i], tvp5150a_saturation_volatile_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_8) / sizeof ( tvp5150a_saturation_volatile_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_8[i], tvp5150a_saturation_volatile_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_9) / sizeof ( tvp5150a_saturation_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_9[i], tvp5150a_saturation_volatile_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(tvp5150a_saturation_volatile_9) / sizeof ( tvp5150a_saturation_volatile_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_saturation_volatile_9[i], tvp5150a_saturation_volatile_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  SetTvp5150aSharpness
*
* Description:  set the sharpness fo tvp51150a   
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void SetTvp5150aSharpness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetTVP5150aSharpness: gtCameraSetting.sharpness = %d, ", gtCameraSetting.sharpness);

    switch(gtCameraSetting.sharpness) {
        case 0:     // 0
            reg_volatile_num = sizeof(tvp5150a_sharpness_volatile_0) / sizeof ( tvp5150a_sharpness_volatile_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_sharpness_volatile_0[i], tvp5150a_sharpness_volatile_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(tvp5150a_sharpness_volatile_1) / sizeof ( tvp5150a_sharpness_volatile_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_sharpness_volatile_1[i], tvp5150a_sharpness_volatile_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(tvp5150a_sharpness_volatile_2) / sizeof ( tvp5150a_sharpness_volatile_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_sharpness_volatile_2[i], tvp5150a_sharpness_volatile_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(tvp5150a_sharpness_volatile_3) / sizeof ( tvp5150a_sharpness_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_sharpness_volatile_3[i], tvp5150a_sharpness_volatile_3[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(tvp5150a_sharpness_volatile_3) / sizeof ( tvp5150a_sharpness_volatile_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(TVP5150A_IIC_ADDR, tvp5150a_sharpness_volatile_3[i], tvp5150a_sharpness_volatile_3[i+1], 0);
            break;
    }

}

/*
*********************************************************************************************
*                                  HwTvp5150aInitialize
*
* Description:  initialize tvp5150a.configue the registers for NTSC
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
int HwTvp5150aInitialize(BOOL video_system)
{
    unsigned int read_data, status = FAILED;
    int i;
    int reg_volatile_num;

    reg_volatile_num = sizeof(tvp5150a_initialization_ntsc) / sizeof ( tvp5150a_initialization_ntsc [0] );
    for (i=0; i<reg_volatile_num; i+=2) {
        if(IICWriteByteTVP(TVP5150A_IIC_ADDR, tvp5150a_initialization_ntsc[i], tvp5150a_initialization_ntsc[i+1], 0) == IIC_ERROR)
            return FAILED;  
    }
        
    reg_volatile_num = sizeof(tvp5150a_initialization_ntsc) / sizeof ( tvp5150a_initialization_ntsc [0] );
    for (i= 0; i<reg_volatile_num; i += 2) {
        if (IICReadByteTVP(TVP5150A_IIC_ADDR, tvp5150a_initialization_ntsc[i], &read_data, 0) == IIC_ERROR)
            return FAILED;
    }
        
    return SUCCESSFUL;
}

/*
*********************************************************************************************
*                                  HwTvp5150aInit
*
* Description:  initialize tvp5150a.configue the registers for NTSC,PAL,brightness,contrast,hue,saturation,sharpness   
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void HwTvp5150aInit(void)
{
    printf("Init TVP5150A ");
    if(HwTvp5150aInitialize(0/*gtVinFormat.format*/) == SUCCESSFUL)    //0: NTSC mode, 1: PAL mode         
        printf("ok\n");
    else 
        printf("failed\n");  

    SetTvp5150aBrightness();
    SetTvp5150aContrast();
    SetTvp5150aHue();
    SetTvp5150aSaturation();
    SetTvp5150aSharpness();
    
}

/*
*********************************************************************************************
*                                  GetTvp5150aID
*
* Description:  Read the ID registers of tvp5150a.    
*
* Arguments  : none.
*
* Return     : The device ID of TVP5150A .
*
* Note(s)    : none.
*********************************************************************************************
*/
int GetTvp5150aID(void)
{
    int device_id,data;
    
    if(IICReadByteTVP(TVP5150A_IIC_ADDR, REG_ID_MSB, &data, 0) < 0)
        return IIC_ERROR;
    
    device_id = data << 8;
    
    if(IICReadByteTVP(TVP5150A_IIC_ADDR, REG_ID_LSB, &data, 0) < 0)
        return IIC_ERROR;
    
    device_id |= (data & 0xff);
    
    printf("deviceid = 0x%x\n",device_id);
 
    return device_id;
}

/*
*********************************************************************************************
*                                  GetTvp5150aVStd
*
* Description:  Get the video standard of tvp5150a.    
*
* Arguments  : *vstd
*
* Return     : SUCCESSFUL or FAILED,
*                   vstd:0  NTSC
*                               1  PAL
*                               2  SECAM
*                               3  MODE ERROR
* Note(s)    : none.
*********************************************************************************************
*/

unsigned char GetTvp5150aVStd(unsigned int *vstd)
{
    Risc_Packet_t pkt;
    unsigned int *pInt;

    int reg_val;
    int lost_lock;
    int video_std;

    pInt = (unsigned int *)&pkt;
    pkt.start = RISC_PKT_START;
    pkt.main_type = tSys;
    pkt.sub_type = tGetCameraID;
    pkt.length = 4;
    pkt.status = 0;	
    pkt.end = RISC_PKT_END;	

    if(IICReadByteTVP(TVP5150A_IIC_ADDR, STATUS_REG_1, &reg_val,0) == IIC_ERROR)
        return FAILED;

    lost_lock = (reg_val >> 4) & 0x1;            //bit4 of STATUS_REG_1 indicates lost or no lost lock since status register #1 was last read
    if(lost_lock == 0)
        printf("analog camera connected!\n");
    else {
        printf("analog camera unconnected!\n");
        return FAILED;
    }

    if(IICReadByteTVP(TVP5150A_IIC_ADDR, STATUS_REG_5, &reg_val,0) == IIC_ERROR)
        return FAILED;

    video_std = (reg_val >> 1) & 0x7;
    switch(video_std)
    {
        case 0x0:
            printf("analog camera type is (M)NTSC.\n");
            *vstd = NTSC;
            break;
            
        case 0x1:
            printf("analog camera type is (B,G,H,I,N)PAL.\n");
            *vstd = PAL;
            break;
            
        case 0x2:
            printf("analog camera type is (M)PAL.\n");
            *vstd = PAL;
            break;
            
        case 0x3:
            printf("analog camera type is PAL-N.\n");
            *vstd = PAL;
            break;
            
        case 0x4:
            printf("analog camera type is NTSC 4.33.\n");
            *vstd = NTSC;
            break;
            
        case 0x5:
            printf("analog camera type is SECAM.\n");
            *vstd = 2;
            break;
            
        default:
            printf("autodetect camera video mode error=%d!\n",video_std);
            *vstd = 3;
            break;
    }

    if(*vstd == NTSC)
        *(pInt + 2) = 4;
    else if (*vstd == PAL)
        *(pInt + 2) = 3;
    else {
        printf("unknow video standard\n");
        return FAILED;
    }
    if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL) {    //send analog camera type to linux
        RiscFlagSet();    // Set risc1 flag to interrupt risc1     
    }else {
        printf("TwoRiscCommSendPacket error \n");
    }

    return SUCCESSFUL;
}

