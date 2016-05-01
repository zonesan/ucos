#include "video_cfg.h"
#if INCLUDE_OV7720
#include"../inc/video_cfg.h"
//#include    "ftuart.h"
#include    "comm.h"
#include "../video/VIN_API.h"
#include "sun_type_defs.h"
#include "VIN_SET.h"

#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "iic.h"

#include "video_intr.h"

#include "OV7720.h"
#include "Camera.h"

extern unsigned int gFreqFromCrystal;

/*
*********************************************************************************************
*                                  SetOV7720FrameRate
*
* Description:     This routine read the array of OV7720_init, then write the value in the array
                                to OV7720 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void SetOV7720FrameRate(int frame_rate)
{
    int i;
 
    printf("\nSetOV7720FrameRate: gFrameRate = %d!\n", gFrameRate);

    if(gFrameRate >= 1 && gFrameRate <= 11)
        frame_rate = 7;
    else if(gFrameRate >= 12&& gFrameRate <= 22)
        frame_rate = 15;
    else if(gFrameRate >= 23)
        frame_rate = 30;
    switch(frame_rate)
    {
        case 7:
            for (i=0; i<sizeof(ov7720_camera_init_frame_rate_7_5fps)/sizeof(ov7720_camera_init_frame_rate_7_5fps [0]); i+=2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_frame_rate_7_5fps[i], ov7720_camera_init_frame_rate_7_5fps[i+1], 0);
            break;
            
        case 15:
            for (i=0; i<sizeof(ov7720_camera_init_frame_rate_15fps)/sizeof(ov7720_camera_init_frame_rate_15fps [0]); i+=2)
                IICWriteByte( OV7720_CAMERA_IIC_ADDR,ov7720_camera_init_frame_rate_15fps[i], ov7720_camera_init_frame_rate_15fps[i+1], 0);
            break;
            
        case 30:
            for (i=0; i<sizeof(ov7720_camera_init_frame_rate_30fps)/sizeof(ov7720_camera_init_frame_rate_30fps [0]); i+=2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_frame_rate_30fps[i], ov7720_camera_init_frame_rate_30fps[i+1], 0);
            break;
                    
        default:    // default 30 framerate
            for (i=0; i<sizeof(ov7720_camera_init_frame_rate_30fps)/sizeof(ov7720_camera_init_frame_rate_30fps [0]); i+=2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_frame_rate_30fps[i], ov7720_camera_init_frame_rate_30fps[i+1], 0);        
            break;
    }    
}

/*
*********************************************************************************************
*                                            SetOV7720Brightness    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Brightness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720brightness: gtCameraSetting.brightness = %d,\n ", gtCameraSetting.brightness);

    switch(gtCameraSetting.brightness) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_0) / sizeof ( ov7720_camera_init_brightness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_0[i], ov7720_camera_init_brightness_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_1) / sizeof ( ov7720_camera_init_brightness_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_1[i], ov7720_camera_init_brightness_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_2) / sizeof ( ov7720_camera_init_brightness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_2[i], ov7720_camera_init_brightness_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_3) / sizeof ( ov7720_camera_init_brightness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_3[i], ov7720_camera_init_brightness_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_4) / sizeof ( ov7720_camera_init_brightness_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_4[i], ov7720_camera_init_brightness_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_5) / sizeof ( ov7720_camera_init_brightness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_5[i], ov7720_camera_init_brightness_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_6) / sizeof ( ov7720_camera_init_brightness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_6[i], ov7720_camera_init_brightness_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_7) / sizeof (ov7720_camera_init_brightness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_7[i], ov7720_camera_init_brightness_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_8) / sizeof (ov7720_camera_init_brightness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_8[i], ov7720_camera_init_brightness_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_9) / sizeof (ov7720_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_9[i], ov7720_camera_init_brightness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7720_camera_init_brightness_9) / sizeof (ov7720_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_brightness_9[i], ov7720_camera_init_brightness_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7720Contrast    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Contrast(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720Contrast: gtCameraSetting.contrast = %d,\n ", gtCameraSetting.contrast);

    switch(gtCameraSetting.contrast) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_0) / sizeof ( ov7720_camera_init_contrast_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_0[i], ov7720_camera_init_contrast_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_1) / sizeof ( ov7720_camera_init_contrast_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_1[i], ov7720_camera_init_contrast_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_2) / sizeof ( ov7720_camera_init_contrast_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_2[i], ov7720_camera_init_contrast_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_3) / sizeof ( ov7720_camera_init_contrast_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_3[i], ov7720_camera_init_contrast_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_4) / sizeof ( ov7720_camera_init_contrast_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_4[i], ov7720_camera_init_contrast_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_5) / sizeof ( ov7720_camera_init_contrast_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_5[i], ov7720_camera_init_contrast_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_6) / sizeof ( ov7720_camera_init_contrast_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_6[i], ov7720_camera_init_contrast_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_7) / sizeof (ov7720_camera_init_contrast_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_7[i], ov7720_camera_init_contrast_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_8) / sizeof (ov7720_camera_init_contrast_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_8[i], ov7720_camera_init_contrast_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_9) / sizeof (ov7720_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_9[i], ov7720_camera_init_contrast_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7720_camera_init_contrast_9) / sizeof (ov7720_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_contrast_9[i], ov7720_camera_init_contrast_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7720Hue    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Hue(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720hue: gtCameraSetting.hue = %d,\n ", gtCameraSetting.hue);

    switch(gtCameraSetting.hue) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7720_camera_init_hue_0) / sizeof ( ov7720_camera_init_hue_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_0[i], ov7720_camera_init_hue_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7720_camera_init_hue_1) / sizeof ( ov7720_camera_init_hue_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_1[i], ov7720_camera_init_hue_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7720_camera_init_hue_2) / sizeof ( ov7720_camera_init_hue_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_2[i], ov7720_camera_init_hue_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7720_camera_init_hue_3) / sizeof ( ov7720_camera_init_hue_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_3[i], ov7720_camera_init_hue_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7720_camera_init_hue_4) / sizeof ( ov7720_camera_init_hue_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_4[i], ov7720_camera_init_hue_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7720_camera_init_hue_5) / sizeof ( ov7720_camera_init_hue_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_5[i], ov7720_camera_init_hue_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7720_camera_init_hue_6) / sizeof ( ov7720_camera_init_hue_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_6[i], ov7720_camera_init_hue_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7720_camera_init_hue_7) / sizeof (ov7720_camera_init_hue_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_7[i], ov7720_camera_init_hue_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7720_camera_init_hue_8) / sizeof (ov7720_camera_init_hue_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_8[i], ov7720_camera_init_hue_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7720_camera_init_hue_9) / sizeof (ov7720_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_9[i], ov7720_camera_init_hue_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7720_camera_init_hue_9) / sizeof (ov7720_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_hue_9[i], ov7720_camera_init_hue_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7720Saturation    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Saturation(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720saturation: gtCameraSetting.saturation = %d,\n ", gtCameraSetting.saturation);

    switch(gtCameraSetting.saturation) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_0) / sizeof ( ov7720_camera_init_saturation_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_0[i], ov7720_camera_init_saturation_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_1) / sizeof ( ov7720_camera_init_saturation_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_1[i], ov7720_camera_init_saturation_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_2) / sizeof ( ov7720_camera_init_saturation_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_2[i], ov7720_camera_init_saturation_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_3) / sizeof ( ov7720_camera_init_saturation_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_3[i], ov7720_camera_init_saturation_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_4) / sizeof ( ov7720_camera_init_saturation_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_4[i], ov7720_camera_init_saturation_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_5) / sizeof ( ov7720_camera_init_saturation_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_5[i], ov7720_camera_init_saturation_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_6) / sizeof ( ov7720_camera_init_saturation_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_6[i], ov7720_camera_init_saturation_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_7) / sizeof (ov7720_camera_init_saturation_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_7[i], ov7720_camera_init_saturation_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_8) / sizeof (ov7720_camera_init_saturation_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_8[i], ov7720_camera_init_saturation_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_9) / sizeof (ov7720_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_9[i], ov7720_camera_init_saturation_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7720_camera_init_saturation_9) / sizeof (ov7720_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_saturation_9[i], ov7720_camera_init_saturation_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7720Sharpness    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Sharpness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720sharpness: gtCameraSetting.sharpness = %d,\n ", gtCameraSetting.sharpness);

    switch(gtCameraSetting.sharpness) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_0) / sizeof ( ov7720_camera_init_sharpness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_0[i], ov7720_camera_init_sharpness_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_1) / sizeof ( ov7720_camera_init_sharpness_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_1[i], ov7720_camera_init_sharpness_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_2) / sizeof ( ov7720_camera_init_sharpness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_2[i], ov7720_camera_init_sharpness_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_3) / sizeof ( ov7720_camera_init_sharpness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_3[i], ov7720_camera_init_sharpness_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_4) / sizeof ( ov7720_camera_init_sharpness_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_4[i], ov7720_camera_init_sharpness_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_5) / sizeof ( ov7720_camera_init_sharpness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_5[i], ov7720_camera_init_sharpness_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_6) / sizeof ( ov7720_camera_init_sharpness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_6[i], ov7720_camera_init_sharpness_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_7) / sizeof (ov7720_camera_init_sharpness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_7[i], ov7720_camera_init_sharpness_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_8) / sizeof (ov7720_camera_init_sharpness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_8[i], ov7720_camera_init_sharpness_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_9) / sizeof (ov7720_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_9[i], ov7720_camera_init_sharpness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7720_camera_init_sharpness_9) / sizeof (ov7720_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_sharpness_9[i], ov7720_camera_init_sharpness_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7720Lightmode    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Lightmode(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7720Lightmode: gtCameraSetting.lightmode = %d,\n ", gtCameraSetting.lightmode);

    switch(gtCameraSetting.lightmode) 
    {
            case 0:         //normal light mode
                  reg_volatile_num = sizeof (ov7720_camera_lowlight_dis) /sizeof (ov7720_camera_lowlight_dis[0]);
                  for (i = 0; i<reg_volatile_num; i +=2)
                  IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_lowlight_dis[i],ov7720_camera_lowlight_dis[i+1],0);
                break;      
                
            case 1:         //the camera is set to be low mode
                  IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_lowlight_en[0],ov7720_camera_lowlight_en[1],0);
                break;
                
            default:
                printf("Error Lightmode.\n");
                break;
    }
}

/*
*********************************************************************************************
*                                            SetOV7720Frequency    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Frequency(void)
{
//    gtCameraSetting.frequency = 0;
    printf("SetOV7720Frequency: gtCameraSetting.frequency = %d,\n ", gtCameraSetting.frequency);

printf("gFreqFromCrystal=%d\n",gFreqFromCrystal);

    switch(gtCameraSetting.frequency) 
    {
          case 0:           //the max frame rate is 25fps  50Hz
                  if(gFreqFromCrystal == TRUE) {
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_50hz_external_clock[0],ov7720_camera_50hz_external_clock[1],0);
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_50hz_external_clock[2],ov7720_camera_50hz_external_clock[3],0);
                    }
                  else
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_50hz_internal_clock[0],ov7720_camera_50hz_internal_clock[1],0);
                break;

          case 1:           //the max frame rate is 30fps   60Hz
                  if(gFreqFromCrystal == TRUE) {
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_60hz_external_clock[0],ov7720_camera_60hz_external_clock[1],0);
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_60hz_external_clock[2],ov7720_camera_60hz_external_clock[3],0);
                    }
                  else
                         IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_60hz_internal_clock[0],ov7720_camera_60hz_internal_clock[1],0);
                break;
                
          default:
                  printf("Error Frequency.\n");
                break;
    }
    
}

/*
*********************************************************************************************
*                                            SetOV7720Homemode    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720Homemode(void)
{
//    gtCameraSetting.homemode = 0;
    printf("SetOV7720Homemode: gtCameraSetting.homemode = %d,\n ", gtCameraSetting.homemode);

    switch(gtCameraSetting.homemode) 
    {
          case 0:
                  IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_indoor[0],ov7720_camera_indoor[1],0);
                break;

          case 1:
                  IICWriteByte(OV7720_CAMERA_IIC_ADDR,ov7720_camera_outdoor[0],ov7720_camera_outdoor[1],0);
                break;
                
          default:
                break;
    }
    
}

/*
*********************************************************************************************
*                                            SetOV7720ImageMirror    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720ImageMirror(void)
{
    int read_data;
    
    printf("SetOV7720ImageMirror: gtCameraSetting.image_mirror = %d,\n ", gtCameraSetting.image_mirror);

    IICReadByte(OV7720_CAMERA_IIC_ADDR, 0x0c, &read_data, 0);

    if (1 == gtCameraSetting.image_mirror) {
        IICWriteByte(OV7720_CAMERA_IIC_ADDR, 0x0c, read_data | (1 << 6), 0);
    } else if (0 == gtCameraSetting.image_mirror) {
        IICWriteByte(OV7720_CAMERA_IIC_ADDR, 0x0c, read_data & (~(1 << 6)), 0);
    }
}

/*
*********************************************************************************************
*                                            SetOV7720ImageFlip    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

void SetOV7720ImageFlip(void)
{
    int read_data;
    
    printf("SetOV7720ImageFlip: gtCameraSetting.image_flip = %d,\n ", gtCameraSetting.image_flip);

    IICReadByte(OV7720_CAMERA_IIC_ADDR, 0x0c, &read_data, 0);

    if (1 == gtCameraSetting.image_flip) {
        IICWriteByte(OV7720_CAMERA_IIC_ADDR, 0x0c, read_data | (1 << 7), 0);
    } else if (0 == gtCameraSetting.image_flip) {
        IICWriteByte(OV7720_CAMERA_IIC_ADDR, 0x0c, read_data & (~(1 << 7)), 0);
    }
}

/*
*********************************************************************************************
*                                            SetOV7720SingleRegister    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

BOOL SetOV7720SingleRegister( BYTE reg, BYTE input)
{

    unsigned char status = FAILED;
    BOOL ret;
    
    static Risc_Packet_t pkt = {0};
    pkt.start = RISC_PKT_START;
    pkt.end = RISC_PKT_END;
    pkt.main_type = tVideo;
	   
    BYTE *p=NULL;
    int n=0;
    int iwCaseN = 0;//case calculate
    int totolN =0;
    int val =-1;
   // GetCamera_Sensor_Init(&p, &totolN);
    p = ov7720_camera_init;
    totolN = sizeof(ov7720_camera_init) / sizeof( ov7720_camera_init[0]);

    int iwRegN = -1;

    while(p < &ov7720_camera_init[totolN])
    {
        if((*p) == reg)
            iwRegN = n;
        p+=2;
        n+=2;
    }

    if(iwRegN == -1)
    {
        debug("\nSetCamSingleRegister: don't have this REG %x in the array!\n" , reg);
    }

    ret = IICWriteByte(OV7720_CAMERA_IIC_ADDR,  reg, input, 0);
	
    if(ret == IIC_OK) {
        if(iwRegN != -1)
        {	
            ov7720_camera_init[iwRegN + 1] = input;	///////////
        }

        IICReadByte(OV7720_CAMERA_IIC_ADDR, reg, &val, 0);

        pkt.sub_type = tGetCamReg;
        pkt.length = 8;
        *(int *)(pkt.message) = reg;
        *(int *)(((int *)pkt.message) + 1) = val;
	  
      	 if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1     
        } else {
            printf("\nSetCamSingleRegister: TwoRiscCommSendPacket error!\n");
        }

        debug("\nSetCamSingleRegister: setReg %x == %x  succeed! And read %d!\n", reg, input, val);

        return ret;
		
    } else {    // IIC_ERROR
        debug("\nSetCamSingleRegister: setReg %x == %x  failed!", reg, input);    
        return ret;
    }	          
}

/*
*********************************************************************************************
*                                            HwOV7720Init    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

VOID HwOV7720Init(VOID)
{
    int i;
    int value;
    int reg_volatile_num;
    ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
    printf("\r\nmode is%d\r\n", mode );    

    IICWriteByte(OV7720_CAMERA_IIC_ADDR, 0x12, 0x80, 0);
    for(i = 0; i<= 0xFFFF; i++);

    int reg_template_num = sizeof ( ov7720_camera_init) /sizeof ( ov7720_camera_init[0] );
    for (i=0; i<reg_template_num; i+=2)
    {
        IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init[i], ov7720_camera_init[i+1], 0);
 //     IICReadByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init[i], &value, 0);
    }
        IICReadByte(OV7720_CAMERA_IIC_ADDR, 0x1c, &value, 0);
        IICReadByte(OV7720_CAMERA_IIC_ADDR, 0x1d, &value, 0);

    SetOV7720FrameRate(gFrameRate);


    switch ( mode )
    {
        case VGA:
            printf("\r\nVGA\r\n");
            reg_volatile_num = sizeof( ov7720_camera_init_vga_volatile ) /sizeof ( ov7720_camera_init_vga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_vga_volatile[i],ov7720_camera_init_vga_volatile[i+1], 0);                                  
            break;

        case QVGA:
            printf("\r\nQVGA\r\n");
            reg_volatile_num = sizeof( ov7720_camera_init_qvga_volatile ) /sizeof(ov7720_camera_init_qvga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_qvga_volatile [i], ov7720_camera_init_qvga_volatile [i+1], 0);
            break;

        case QQVGA:
            printf("\r\nQQVGA\r\n");
            reg_volatile_num = sizeof( ov7720_camera_init_qqvga_volatile ) /sizeof(ov7720_camera_init_qqvga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_qqvga_volatile [i], ov7720_camera_init_qqvga_volatile [i+1], 0);
            break;


        case CIF:
            printf("\r\nCIF\r\n");
            reg_volatile_num = sizeof ( ov7720_camera_init_cif_volatile )/ sizeof ( ov7720_camera_init_cif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_cif_volatile[i], ov7720_camera_init_cif_volatile[i+1], 0);
            break;

        case QCIF:          
            printf("\r\nQCIF\r\n");
            reg_volatile_num = sizeof ( ov7720_camera_init_qcif_volatile ) / sizeof ( ov7720_camera_init_qcif_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_qcif_volatile [i], ov7720_camera_init_qcif_volatile [i+1], 0);                
            break;

        case NTSC_CIF:          
            printf("\r\nNTSC_CIF\r\n");
            reg_volatile_num = sizeof ( ov7720_camera_init_cif_volatile ) / sizeof ( ov7720_camera_init_cif_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_cif_volatile [i], ov7720_camera_init_cif_volatile [i+1], 0);                
            break;

        case NTSC_QCIF:          
            printf("\r\nNTSC_QCIF\r\n");
            reg_volatile_num = sizeof ( ov7720_camera_init_qcif_volatile ) / sizeof ( ov7720_camera_init_qcif_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_qcif_volatile [i], ov7720_camera_init_qcif_volatile [i+1], 0);                
            break;

        case SUB_QCIF:          
            printf("\r\nSUB_QCIF\r\n");
            reg_volatile_num = sizeof ( ov7720_camera_init_qqvga_volatile ) / sizeof ( ov7720_camera_init_qqvga_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_qqvga_volatile [i], ov7720_camera_init_qqvga_volatile [i+1], 0);                
            break;

        default:
            printf("\r\nVGA\r\n");
            reg_volatile_num = sizeof( ov7720_camera_init_vga_volatile ) /sizeof ( ov7720_camera_init_vga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7720_CAMERA_IIC_ADDR, ov7720_camera_init_vga_volatile[i],ov7720_camera_init_vga_volatile[i+1], 0);                                  
            break;
    }

    SetOV7720Brightness();
    SetOV7720Contrast();
    SetOV7720Hue();
    SetOV7720Saturation();
    SetOV7720Sharpness();
    
    SetOV7720Lightmode();
    SetOV7720Frequency();
//    SetOV7720Homemode();
    SetOV7720ImageFlip();
    SetOV7720ImageMirror();
    
    SendAllCamRegValToLinux();

}

/*
*********************************************************************************************
*                                            GetOV7720ID
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

int GetOV7720ID(void)
{
	int device_id,data,status;

	if(IICReadByte(OV7720_CAMERA_IIC_ADDR, REG_PID, &data, 0) < 0)
		return IIC_ERROR;
	device_id = data << 8;

	if(IICReadByte(OV7720_CAMERA_IIC_ADDR, REG_VER, &data, 0) < 0)
		return IIC_ERROR;
	device_id |= (data & 0xff);

	printf("deviceid = 0x%x\n",device_id);
	return device_id;
}

#endif

