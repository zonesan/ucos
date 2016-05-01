
/*
***************************************************************************
*                   Copyright (c) 2005 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : adv7171.c             
*                                                                         
* Description : Functions for ADV7181      
*                                                
* By          : paul             
*
* Date        : Feb 28, 2007                                                           
***************************************************************************
*/
/****************send analog camera type to linux code needs these header files********************/
#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "comm.h"
#include "os_cpu.h"
#include "two_risc_comm.h"
#include "sun_type_defs.h"
/****************************************************************************************/

#include "adv7181.h"
#include "iic.h"
#include "Camera.h"
unsigned char initialise_adv7181_ntsc[ ]=
{
0x07, 0xff, //Enable autodetection only.
0x17 ,0x41, //Set CSFM to SH1.
0x2B, 0xE2, //AGC flash tweak.
0x19, 0xFA, //Stronger dot crawl reduction.
0x3A, 0x16, //Power down ADC 1 and ADC 2.
0x50, 0x0A, //Set higher DNR threshold.
0x51, 0x24, //Turn off FSC detect for IN LOCK status.
0xC3, 0x09, //Man mux AIN6 to ADC0 (1001).
0xC4, 0x80, //Set setadc_sw_man_en = 1.
0xD2, 0x01, //AGC flash tweak.
0xD3, 0x01, //AGC flash tweak.
0xDB, 0x9B, //AGC flash tweak.
CONTRAST_ADJ__REG,         0xff, //Contrast Control, register 0x08
0x0E, 0x80, //ADI recommended programming sequence. This sequence must be followed exactly when setting up the decoder.
0x89, 0x0D, //Recommended setting.
0x8D, 0x9B, //Recommended setting.
0x8F, 0x48, //Recommended setting.
0xB5 ,0x8B, //Recommended setting.
0xD4, 0xFB, //Recommended setting.
0xD6, 0x6D, //Recommended setting.
0xE2, 0xAF, //Recommended setting.
0xE3, 0x00, //Recommended setting.
0xE4, 0xB5, //Recommended setting.
0xE8, 0xF3, //Recommended setting.
0x0E, 0x00 ,//Recommended setting.
//Pal total 22
//0x00, 0x80, //Force PAL input only mode.
/*0x07, 0x01, //Enable PAL autodetection only.
0x15, 0x00, //Slow down digital clamps.
0x17, 0x41, //Set CSFM to SH1.
0x19, 0xFA, //Stronger dot crawl reduction.
0x3A, 0x16, //Power down ADC 1 and ADC 2.
0x50, 0x0A, //Set higher DNR threshold.
0xC3, 0x09, //Man mux AIN1 to ADC0 (0101).
0xC4, 0x80, //Enable manual muxing
0x0E, 0x85, 
*///ADI recommended programming sequence. This sequence must be followed exactly when setting up the decoder.
/*0x50, 0x20, //Recommended setting.
0x52, 0x18, //Recommended setting.
0x58, 0xED, //Recommended setting.
0x77, 0xC5, //Recommended setting.
0x7C, 0x93, //Recommended setting.
0x7D, 0x00, //Recommended setting.
0xD0, 0x48, //Recommended setting.
0xD5, 0xA0, //Recommended setting.
0xD7, 0xEA, //Recommended setting.
0xE4, 0x3E, //Recommended setting.
0xEA, 0x0F, //Recommended setting.
0x0E, 0x00, *///Recommended setting.
/*0x31,0x12,
0x32,0x81,
0x33,0x84,
0x37,0xA0,
0xe5,0x00,
0xe6,0x03,
0xe7,0x85,
0x50,0x00,
0x51,0x00,
0x04,0x02,
0x08,0x60,
0x0a,0x18,
0x0e,0x05
*/
};


/*
unsigned char initialise_adv7181_ntsc[ADV_REGSIZE]=
{

IN_CTRL_REG,                  0x00, //CVBS input, register 0x00
//VIDEO_SEL_REG,                0x88, //Turn off HSYNC processor (SECAM only).
CONTRAST_ADJ__REG,            0xff, //Contrast Control, register 0x08
BRIGHT_ADJ_REG,               0xff, //Brightness Control, register 0x0A   
SHAPING_FLTR_CTRL1_REG,       0x41, //Set CSFM to SH1., register 0x17
MISC_GAIN_CTRL_REG,           0xE2, //AGC flash tweak., register 0x2B
ADC_CTL_REG,                  0x16, //Power down ADC 1 and ADC 2., register 0x3A
CTI_DNR_CTL4_REG,             0x04, //Set DNR threshold., register 0x50
LOCK_CNT_REG,                 0x24, //Turn off FSC detect for IN LOCK status., register 0x51 
ADC_SWITCH1_REG,              0x09, //Man mux AIN1 to ADC0, register 0xC3
ADC_SWITCH2_REG ,             0x80, //Set setadc_sw_man_en =1.
RES4_REG,                     0x01, //AGC flash tweak.
RES5_REG,                     0x01, //AGC flash tweak.
RES8_REG,                     0x9B, //AGC flash tweak.
//have introduced the foll 3 reg to remove the black bar 
NTSC_VBIT_BEG_REG,            0x00,
NTSC_VBIT_END_REG,            0x12,
NTSC_FBIT_TOG_REG,            0x80,
///////////////////////////////////////////////////////
//ADI RECOMMENDED SETIING
ADI_CTRL_REG ,                0x85, 
RES1_REG,                     0x0D, 
RES2_REG,                     0x9B, 
FREERUN_LEN1_REG,             0x48, 
RES3_REG,                     0x8B, 
RES6_REG,                     0xFB, 
RES7_REG,                     0x6D, 
SD_OFFSET_CR_REG,             0xAF, 
SD_SAT_CB_REG ,               0x00, 
SD_SAT_CR_REG,                0xB5, 
PAL_VBIT_BEG_REG,             0xF3, 
ADI_CTRL_REG ,                0x05 
};
*/

unsigned char initialise_adv7181_pal[ ]=
{
0x00 ,0x80 ,//Force PAL input only mode.
0x07, 0x01 ,//Enable PAL autodetection only.
0x15, 0x00 ,//Slow down digital clamps.
0x17, 0x41 ,//Set CSFM to SH1.
0x19 ,0xFA, //Stronger dot crawl reduction.
0x3A ,0x16, //Power down ADC 1 and ADC 2.
0x50, 0x0A ,//Set higher DNR threshold.
0xC3, 0x09, //Man mux AIN1 to ADC0 (1001).
0xC4 ,0x80 ,//Enable manual muxing
0x0E ,0x80 ,//ADI recommended programming sequence. This sequence must be followed exactly when setting up the decoder.
0x50 ,0x20 ,//Recommended setting.
0x52 ,0x18, //Recommended setting.
0x58 ,0xED ,//Recommended setting.
0x77 ,0xC5, //Recommended setting.
0x7C ,0x93, //Recommended setting.
0x7D ,0x00, //Recommended setting.
0xD0, 0x48, //Recommended setting.
0xD5 ,0xA0 ,//Recommended setting.
0xD7, 0xEA, //Recommended setting.
0xE4, 0x3E ,//Recommended setting.
0xEA ,0x0F ,//Recommended setting.
0x0E ,0x00 //Recommended setting.
};

//brightness
unsigned char adv7181_camera_init_brightness_0[ ]=
{
0x0a,0xF9,
};

unsigned char adv7181_camera_init_brightness_1[ ]=
{
0x0a,0xFb,
};

unsigned char adv7181_camera_init_brightness_2[ ]=
{
0x0a,0xFd,
};

unsigned char adv7181_camera_init_brightness_3[ ]=
{
0x0a,0xFf,
};

unsigned char adv7181_camera_init_brightness_4[ ]=
{
0x0a,0x0c,
};

unsigned char adv7181_camera_init_brightness_5[ ]=
{
0x0a,0x10,
};

unsigned char adv7181_camera_init_brightness_6[ ]=
{
0x0a,0x14,
};

unsigned char adv7181_camera_init_brightness_7[ ]=
{
0x0a,0x18,
};

unsigned char adv7181_camera_init_brightness_8[ ]=
{
0x0a,0x1c,
};

unsigned char adv7181_camera_init_brightness_9[ ]=
{
0x0a,0x20,
};

//contrast
unsigned char adv7181_camera_init_contrast_0[ ]=
{
0x08,0x9C,
};

unsigned char adv7181_camera_init_contrast_1[ ]=
{
0x08,0xA7,
};

unsigned char adv7181_camera_init_contrast_2[ ]=
{
0x08,0xB2,
};

unsigned char adv7181_camera_init_contrast_3[ ]=
{
0x08,0xBD,
};

unsigned char adv7181_camera_init_contrast_4[ ]=
{
0x08,0xC8,
};

unsigned char adv7181_camera_init_contrast_5[ ]=
{
0x08,0xD3,
};

unsigned char adv7181_camera_init_contrast_6[ ]=
{
0x08,0xDE,
};

unsigned char adv7181_camera_init_contrast_7[ ]=
{
0x08,0xE9,
};

unsigned char adv7181_camera_init_contrast_8[ ]=
{
0x08,0xF4,
};

unsigned char adv7181_camera_init_contrast_9[ ]=
{
0x08,0xFF,
};

//hue
unsigned char adv7181_camera_init_hue_0[ ]=
{
0x0b,0x88,
//0xe1,0xc0,
//0xe2,0xc0,
};

unsigned char adv7181_camera_init_hue_1[ ]=
{
0x0b,0x86,
//0xe1,0xb0,
//0xe2,0xb0,
};

unsigned char adv7181_camera_init_hue_2[ ]=
{
0x0b,0x84,
//0xe1,0xa0,
//0xe2,0xa0,
};

unsigned char adv7181_camera_init_hue_3[ ]=
{
0x0b,0x82,
//0xe1,0x90,
//0xe2,0x90,
};

unsigned char adv7181_camera_init_hue_4[ ]=
{
0x0b,0x00,
//0xe1,0x80,
//0xe2,0x80,
};

unsigned char adv7181_camera_init_hue_5[ ]=
{
0x0b,0x10,
//0xe1,0x60,
//0xe2,0x60,
};

unsigned char adv7181_camera_init_hue_6[ ]=
{
0x0b,0x20,
//0xe1,0x50,
//0xe2,0x50,
};

unsigned char adv7181_camera_init_hue_7[ ]=
{
0x0b,0x30,
//0xe1,0x40,
//0xe2,0x40,
};

unsigned char adv7181_camera_init_hue_8[ ]=
{
0x0b,0x40,
//0xe1,0x30,
//0xe2,0x30,
};

unsigned char adv7181_camera_init_hue_9[ ]=
{
0x0b,0x50,
//0xe1,0x20,
//0xe2,0x20,
};

//saturation
unsigned char adv7181_camera_init_saturation_0[ ]=
{
0xe3,0x4a,    //Cb channel
0xe4,0x50,    //Cr channel
};

unsigned char adv7181_camera_init_saturation_1[ ]=
{
0xe3,0x5a,
0xe4,0x60,
};

unsigned char adv7181_camera_init_saturation_2[ ]=
{
0xe3,0x6a,
0xe4,0x70,
};

unsigned char adv7181_camera_init_saturation_3[ ]=
{
0xe3,0x7a,
0xe4,0x80,
};

unsigned char adv7181_camera_init_saturation_4[ ]=
{
0xe3,0x8a,
0xe4,0x90,
};

unsigned char adv7181_camera_init_saturation_5[ ]=
{
0xe3,0x9a,
0xe4,0xa0,
};

unsigned char adv7181_camera_init_saturation_6[ ]=
{
0xe3,0xaa,
0xe4,0xb0,
};

unsigned char adv7181_camera_init_saturation_7[ ]=
{
0xe3,0xba,
0xe4,0xc0,
};

unsigned char adv7181_camera_init_saturation_8[ ]=
{
0xe3,0xca,
0xe4,0xd0,
};

unsigned char adv7181_camera_init_saturation_9[ ]=
{
0xe3,0xda,
0xe4,0xe0,
};

//sharpness
unsigned char adv7181_camera_init_sharpness_0[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_1[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_2[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_3[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_4[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_5[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_6[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_7[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_8[ ]=
{

};

unsigned char adv7181_camera_init_sharpness_9[ ]=
{

};

/*
*********************************************************************************************
*                                  HwAdv7181Brightness
*
* Description:

* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetAdv7181Brightness(void)
{    
    int i;
    int reg_volatile_num;

    printf("SetADV7181Brightness: gtCameraSetting.brightness = %d, ", gtCameraSetting.brightness);

    switch(gtCameraSetting.brightness) {
        case 0:     // 0
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_0) / sizeof ( adv7181_camera_init_brightness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_0[i], adv7181_camera_init_brightness_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_1) / sizeof ( adv7181_camera_init_brightness_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_1[i], adv7181_camera_init_brightness_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_2) / sizeof ( adv7181_camera_init_brightness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_2[i], adv7181_camera_init_brightness_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_3) / sizeof ( adv7181_camera_init_brightness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_3[i], adv7181_camera_init_brightness_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_4) / sizeof ( adv7181_camera_init_brightness_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_4[i], adv7181_camera_init_brightness_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_5) / sizeof ( adv7181_camera_init_brightness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_5[i], adv7181_camera_init_brightness_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_6) / sizeof ( adv7181_camera_init_brightness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_6[i], adv7181_camera_init_brightness_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_7) / sizeof ( adv7181_camera_init_brightness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_7[i], adv7181_camera_init_brightness_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_8) / sizeof ( adv7181_camera_init_brightness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_8[i], adv7181_camera_init_brightness_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_9) / sizeof ( adv7181_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_9[i], adv7181_camera_init_brightness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(adv7181_camera_init_brightness_9) / sizeof ( adv7181_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_brightness_9[i], adv7181_camera_init_brightness_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  HwAdv7181Contrast
*
* Description:

* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetAdv7181Contrast(void)
{    
    int i;
    int reg_volatile_num;

    printf("SetADV7181Contrast: gtCameraSetting.contrast = %d, ", gtCameraSetting.contrast);

    switch(gtCameraSetting.contrast) {
        case 0:     // 0
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_0) / sizeof ( adv7181_camera_init_contrast_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_0[i], adv7181_camera_init_contrast_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_1) / sizeof ( adv7181_camera_init_contrast_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_1[i], adv7181_camera_init_contrast_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_2) / sizeof ( adv7181_camera_init_contrast_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_2[i], adv7181_camera_init_contrast_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_3) / sizeof ( adv7181_camera_init_contrast_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_3[i], adv7181_camera_init_contrast_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_4) / sizeof ( adv7181_camera_init_contrast_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_4[i], adv7181_camera_init_contrast_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_5) / sizeof ( adv7181_camera_init_contrast_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_5[i], adv7181_camera_init_contrast_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_6) / sizeof ( adv7181_camera_init_contrast_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_6[i], adv7181_camera_init_contrast_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_7) / sizeof ( adv7181_camera_init_contrast_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_7[i], adv7181_camera_init_contrast_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_8) / sizeof ( adv7181_camera_init_contrast_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_8[i], adv7181_camera_init_contrast_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_9) / sizeof ( adv7181_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_9[i], adv7181_camera_init_contrast_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(adv7181_camera_init_contrast_9) / sizeof ( adv7181_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_contrast_9[i], adv7181_camera_init_contrast_9[i+1], 0);
            break;
    }
}


/*
*********************************************************************************************
*                                  HwAdv7181Hue
*
* Description:

* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetAdv7181Hue(void)
{    
    int i;
    int reg_volatile_num;

    printf("SetADV7181Hue: gtCameraSetting.hue = %d, ", gtCameraSetting.hue);

    switch(gtCameraSetting.hue) {
        case 0:     // 0
            reg_volatile_num = sizeof(adv7181_camera_init_hue_0) / sizeof ( adv7181_camera_init_hue_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_0[i], adv7181_camera_init_hue_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(adv7181_camera_init_hue_1) / sizeof ( adv7181_camera_init_hue_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_1[i], adv7181_camera_init_hue_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(adv7181_camera_init_hue_2) / sizeof ( adv7181_camera_init_hue_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_2[i], adv7181_camera_init_hue_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(adv7181_camera_init_hue_3) / sizeof ( adv7181_camera_init_hue_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_3[i], adv7181_camera_init_hue_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(adv7181_camera_init_hue_4) / sizeof ( adv7181_camera_init_hue_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_4[i], adv7181_camera_init_hue_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(adv7181_camera_init_hue_5) / sizeof ( adv7181_camera_init_hue_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_5[i], adv7181_camera_init_hue_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(adv7181_camera_init_hue_6) / sizeof ( adv7181_camera_init_hue_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_6[i], adv7181_camera_init_hue_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(adv7181_camera_init_hue_7) / sizeof ( adv7181_camera_init_hue_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_7[i], adv7181_camera_init_hue_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(adv7181_camera_init_hue_8) / sizeof ( adv7181_camera_init_hue_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_8[i], adv7181_camera_init_hue_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(adv7181_camera_init_hue_9) / sizeof ( adv7181_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_9[i], adv7181_camera_init_hue_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(adv7181_camera_init_hue_9) / sizeof ( adv7181_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_hue_9[i], adv7181_camera_init_hue_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  HwAdv7181Saturation
*
* Description:

* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetAdv7181Saturation(void)
{    
    int i;
    int reg_volatile_num;

    printf("SetADV7181Saturation: gtCameraSetting.saturation = %d, ", gtCameraSetting.saturation);

    switch(gtCameraSetting.saturation) {
        case 0:     // 0
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_0) / sizeof ( adv7181_camera_init_saturation_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_0[i], adv7181_camera_init_saturation_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_1) / sizeof ( adv7181_camera_init_saturation_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_1[i], adv7181_camera_init_saturation_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_2) / sizeof ( adv7181_camera_init_saturation_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_2[i], adv7181_camera_init_saturation_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_3) / sizeof ( adv7181_camera_init_saturation_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_3[i], adv7181_camera_init_saturation_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_4) / sizeof ( adv7181_camera_init_saturation_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_4[i], adv7181_camera_init_saturation_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_5) / sizeof ( adv7181_camera_init_saturation_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_5[i], adv7181_camera_init_saturation_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_6) / sizeof ( adv7181_camera_init_saturation_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_6[i], adv7181_camera_init_saturation_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_7) / sizeof ( adv7181_camera_init_saturation_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_7[i], adv7181_camera_init_saturation_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_8) / sizeof ( adv7181_camera_init_saturation_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_8[i], adv7181_camera_init_saturation_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_9) / sizeof ( adv7181_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_9[i], adv7181_camera_init_saturation_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(adv7181_camera_init_saturation_9) / sizeof ( adv7181_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_saturation_9[i], adv7181_camera_init_saturation_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  HwAdv7181Sharpness
*
* Description:

* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetAdv7181Sharpness(void)
{    
    int i;
    int reg_volatile_num;

    printf("SetADV7181Sharpness: gtCameraSetting.sharpness = %d, ", gtCameraSetting.sharpness);

    switch(gtCameraSetting.sharpness) {
        case 0:     // 0
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_0) / sizeof ( adv7181_camera_init_sharpness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_0[i], adv7181_camera_init_sharpness_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_1) / sizeof ( adv7181_camera_init_sharpness_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_1[i], adv7181_camera_init_sharpness_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_2) / sizeof ( adv7181_camera_init_sharpness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_2[i], adv7181_camera_init_sharpness_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_3) / sizeof ( adv7181_camera_init_sharpness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_3[i], adv7181_camera_init_sharpness_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_4) / sizeof ( adv7181_camera_init_sharpness_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_4[i], adv7181_camera_init_sharpness_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_5) / sizeof ( adv7181_camera_init_sharpness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_5[i], adv7181_camera_init_sharpness_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_6) / sizeof ( adv7181_camera_init_sharpness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_6[i], adv7181_camera_init_sharpness_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_7) / sizeof ( adv7181_camera_init_sharpness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_7[i], adv7181_camera_init_sharpness_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_8) / sizeof ( adv7181_camera_init_sharpness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_8[i], adv7181_camera_init_sharpness_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_9) / sizeof ( adv7181_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_9[i], adv7181_camera_init_sharpness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(adv7181_camera_init_sharpness_9) / sizeof ( adv7181_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(ADV7181_IIC_ADDR, adv7181_camera_init_sharpness_9[i], adv7181_camera_init_sharpness_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                  HwAdv7181Initialize
*
* Description:     This routine read the array of adv7181_init, then write the value in the array
                                to adv7181 chip through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
int HwADV7181Initialize(BOOL video_system)
{
    unsigned int read_data, status = FAILED;
    int i;
    int reg_volatile_num;

    if (video_system == 0) {    // NTSC
        reg_volatile_num = sizeof(initialise_adv7181_ntsc) / sizeof ( initialise_adv7181_ntsc [0] );
        for (i=0; i<reg_volatile_num; i+=2) {
//          if(HwI2cSingleWrite(ADV7181_WRITE_ADDR, initialise_adv7181_ntsc[i], initialise_adv7181_ntsc[i+1]) == FAILED)
            if(IICWriteByte(ADV7181_IIC_ADDR, initialise_adv7181_ntsc[i], initialise_adv7181_ntsc[i+1], 0) == IIC_ERROR)
                return FAILED;  
        }

        /* Read to verify! */
        for (i=0; i<reg_volatile_num; i+=2) {
            if(IICReadByteADV(ADV7181_IIC_ADDR, initialise_adv7181_ntsc[i], &read_data, 0) == IIC_ERROR)
            {
            }
            if(read_data == initialise_adv7181_ntsc[i+1])
            {
            }
        }
        
    } else if (video_system == 1) {     // PAL(ke neng you wen ti)
        reg_volatile_num = sizeof(initialise_adv7181_pal) / sizeof ( initialise_adv7181_pal [0] );
        for (i=0; i<reg_volatile_num; i+=2) {
//          if(HwI2cSingleWrite(ADV7181_WRITE_ADDR, initialise_adv7181_pal[i], initialise_adv7181_pal[i+1]) == FAILED)
            if(IICWriteByte(ADV7181_IIC_ADDR, initialise_adv7181_pal[i], initialise_adv7181_pal[i+1], 0) == IIC_ERROR)
                return FAILED;
        }
        
        /* Read to verify! */
        for (i=0; i<reg_volatile_num; i+=2) {
            if(IICReadByteADV(ADV7181_IIC_ADDR, initialise_adv7181_pal[i], &read_data, 0) == IIC_ERROR)
            {
            }
            if(read_data != initialise_adv7181_pal[i+1])
            {
            }
        }          
    } else
        return FAILED;

    return SUCCESSFUL;
}

void HwAdv7181Init(void)
{
    printf("Init adv7181 ");
    if(HwADV7181Initialize(0/*gtVinFormat.format*/) == SUCCESSFUL)    //0: NTSC mode, 1: PAL mode         
        printf("ok\n");
    else 
        printf("failed\n");  

    SetAdv7181Brightness();
    SetAdv7181Contrast();
    SetAdv7181Hue();
    SetAdv7181Saturation();
}

int GetAdv7181ID(void)
{
    int device_state_reg;

    if(IICReadByteADV(ADV7181_IIC_ADDR, 0x11, &device_state_reg, 0) < 0)
        return IIC_ERROR;
    printf("\nGetAdv7181ID: device_state_reg = 0x%x\n",device_state_reg);
    if(device_state_reg >= 0x0a && device_state_reg <= 0x20) {
        return 0x7181;
    }
    
    return 0;
}

unsigned char ADV7181AutodetectVMode(unsigned int *vmode)
{
    Risc_Packet_t pkt;
    unsigned int *pInt;
    int reg_val;
    int i;

    pInt = (unsigned int *)&pkt;
    pkt.start = RISC_PKT_START;
    pkt.main_type = tSys;
    pkt.sub_type = tGetCameraID;
    pkt.length = 4;
    pkt.status = 0;	
    pkt.end = RISC_PKT_END;	

    if(IICReadByteADV(ADV7181_IIC_ADDR, STATUS1_REG, &reg_val, 0) == IIC_ERROR)
        return FAILED;
    
    i = reg_val & 0x3;
    if(i == 1)
        printf("analog camera connected!\n");
    else if(i == 2) {
        printf("analog camera unconnected!\n");
        return FAILED;
    }else {
        printf("analog camera connected status error=%d\n",i);
        return FAILED;
    }
    
    i = (reg_val>>4) & 0x7;
    switch(i)
    {
        case 0:
            printf("analog camera type is NTSC-MJ.\n");
            *vmode = NTSC;
            break;
        case 1:
            printf("analog camera type is NTSC-443.\n");
            *vmode = NTSC;
            break;
        case 2:
            printf("analog camera type is PAL-M.\n");
            *vmode = PAL;
            break;
        case 3:
            printf("analog camera type is PAL-60.\n");
            *vmode = PAL;
            break;
        case 4:
            printf("analog camera type is PAL-BGHID.\n");
            *vmode = PAL;
            break;
        case 5:
            printf("analog camera type is SECAM.\n");
            *vmode = 2;
            break;
        case 6:
            printf("analog camera type is PAL-Combination N.\n");
            *vmode = PAL;
            break;
        case 7:
            printf("analog camera type is SECAM-525.\n");
            *vmode = 2;
            break;
        default:
            printf("autodetect camera video mode error=%d!\n", i);
            *vmode = 3;
            break;
    }
    
    if((*vmode == NTSC) || (*vmode == PAL)) {
        if(*vmode == NTSC)
            *(pInt + 2) = 4;
        else
            *(pInt + 2) = 3;
        if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL) {    //send analog camera type to linux
            RiscFlagSet();    // Set risc1 flag to interrupt risc1     
        }else {
            printf("TwoRiscCommSendPacket error \n");
        }
    }
    
    return SUCCESSFUL;
}
