
#ifndef _ov7720_H
#define _ov7720_H

#include "def_type.h"

#define REG_PID 0xA
#define REG_VER 0xB

#define OV7720_CAMERA_IIC_ADDR  0x21
#define OV7720_CAMERA_REG_SIZE  128*2

/*definiton for UT box*/
#define SETTING_FOR_UT

#ifdef SETTING_FOR_UT
//#define GAMMA_0_UT
#define GAMMA_1_UT
#endif

BYTE ov7720_camera_init_frame_rate_30fps [] =
{
0x11,0x01, //00/01/03/07 for 60/30/15/7.5fps
0x22,0x7f, //ff/7f/3f/1f for 60/30/15/7.5fps
0x23,0x03, //01/03/07/0f for 60/30/15/7.5fps
};

BYTE ov7720_camera_init_frame_rate_15fps [] =
{
0x11,0x03, //00/01/03/07 for 60/30/15/7.5fps
0x22,0x3f, //ff/7f/3f/1f for 60/30/15/7.5fps
0x23,0x07, //01/03/07/0f for 60/30/15/7.5fps
};

BYTE ov7720_camera_init_frame_rate_7_5fps [] =
{
0x11,0x07, //00/01/03/07 for 60/30/15/7.5fps
0x22,0x1f, //ff/7f/3f/1f for 60/30/15/7.5fps
0x23,0x0f, //01/03/07/0f for 60/30/15/7.5fps
};

BYTE ov7720_camera_init_vga_volatile []=
{
0x29,0xa0,
0x2c,0xf0,
0x65,0x20,
};

BYTE ov7720_camera_init_qvga_volatile []=
{
0x29,0x50,
0x2c,0x78,
0x65,0x2f,
};

BYTE ov7720_camera_init_qqvga_volatile []=
{
0x29,0x28,
0x2c,0x3c,
0x65,0x2f,
};

BYTE ov7720_camera_init_cif_volatile []=
{
0x29,0x58,
0x2c,0x90,
0x65,0x2f,
};

BYTE ov7720_camera_init_qcif_volatile []=
{
0x29,0x2c,
0x2c,0x48,
0x65,0x2f,
};

BYTE ov7720_camera_init[]=
{
0x3d,0x03,
0x17,0x22,
0x18,0xa4,
0x19,0x07,
0x1a,0xf0,
0x32,0x00, //0x00
0x29,0xa0,
0x2c,0xf0,
0x2a,0x02,
0x11,0x01, //00/01/03/07 for 60/30/15/7.5fps
0x12,0x20,
0x15,0x11,
0x42,0x7f,
0x4d,0x09,
0x63,0xe0,
0x64,0xff,
0x65,0x20,
0x66,0x00,
0x67,0x48,
0x13,0xf0,
0x0d,0x41, //51/61/71 for different AEC/AGC window
0x0f,0xc5,
0x14,0x11,
0x22,0x7f, //ff/7f/3f/1f for 60/30/15/7.5fps
0x23,0x07, //01/03/07/0f for 60/30/15/7.5fps
0x24,0x52, //0x40
0x25,0x42, //0x30
0x26,0xa1,
0x2b,0x9e, //00/9e for 60/50Hz
0x6b,0xaa,
0x13,0xff,
0x90,0x05,
0x91,0x01,
0x92,0x03,
0x93,0x00,
0x94,0xb0,
0x95,0x9d,
0x96,0x13,
0x97,0x16,
0x98,0x7b,
0x99,0x91,
0x9a,0x1e,
0x9b,0x08,
0x9c,0x20,
0x9e,0x81,
0xa6,0x07,
0xab,0x06,
#if !defined(SETTING_FOR_UT)
0x7e,0x0c, //GAMMA SETTING FOR IPCAM
0x7f,0x16,
0x80,0x2a,
0x81,0x4e,
0x82,0x61,
0x83,0x6f,
0x84,0x7b,
0x85,0x86,
0x86,0x8e,
0x87,0x97,
0x88,0xa4,
0x89,0xaf,
0x8a,0xc5,
0x8b,0xd7,
0x8c,0xe8,
0x8d,0x20,
#else
#ifdef GAMMA_0_UT
0x7e,0x04, //GAMMA 0 SETTING FOR UT
0x7f,0x08,
0x80,0x10,
0x81,0x20,
0x82,0x28,
0x83,0x30,
0x84,0x38,
0x85,0x40,
0x86,0x48,
0x87,0x50,
0x88,0x60,
0x89,0x70,
0x8a,0x90,
0x8b,0xB0,
0x8c,0xD0,
0x8d,0x40,
#endif
#ifdef GAMMA_1_UT
0x7e,0x08, //GAMMA 1 SETTING FOR UT
0x7f,0x10,
0x80,0x20,
0x81,0x28,
0x82,0x30,
0x83,0x38,
0x84,0x40,
0x85,0x48,
0x86,0x50,
0x87,0x60,
0x88,0x70,
0x89,0x90,
0x8a,0xB0,
0x8b,0xC0,
0x8c,0xD0,
0x8d,0x40,
#endif
#endif
0x0c,0x20, //color bar enable
0x66,0x10,
0x0d,0x41, //bypass pll
0x67,0x00, //yuv output by dsp
0x64,0xff, //enable FIFO selection
0x15,0x11, //change the data range
0x76,0x00,
0x77,0x10,
0x94,0xb0,
0x95,0x9d,
0x96,0x13,
0x97,0x16,
0x98,0x7b,
0x99,0x91,
0x9a,0x1e,

};

BYTE ov7720_camera_init_brightness_0 []=
{
//0xa6,0x07,
0xab,0x0e,
0x9b,0x40,
};

BYTE ov7720_camera_init_brightness_1 []=
{
//0xa6,0x07,
0xab,0x0e,
0x9b,0x20,
};

BYTE ov7720_camera_init_brightness_2 []=
{
//0xa6,0x07,
0xab,0x0e,
0x9b,0x10,
};

BYTE ov7720_camera_init_brightness_3 []=
{
//0xa6,0x07,
0xab,0x0e,
0x9b,0x00,
};

BYTE ov7720_camera_init_brightness_4 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x20,
};

BYTE ov7720_camera_init_brightness_5 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x30,
};

BYTE ov7720_camera_init_brightness_6 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x40,
};

BYTE ov7720_camera_init_brightness_7 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x50,
};

BYTE ov7720_camera_init_brightness_8 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x60,
};

BYTE ov7720_camera_init_brightness_9 []=
{
//0xa6,0x07,
0xab,0x06,
0x9b,0x70,
};

BYTE ov7720_camera_init_contrast_0 []=
{
//0xa6,0x07,
0x9c,0x08,
};

BYTE ov7720_camera_init_contrast_1 []=
{
//0xa6,0x07,
0x9c,0x0a,
};

BYTE ov7720_camera_init_contrast_2 []=
{
//0xa6,0x07,
0x9c,0x10,
};

BYTE ov7720_camera_init_contrast_3 []=
{
//0xa6,0x07,
0x9c,0x18,
};

BYTE ov7720_camera_init_contrast_4 []=
{
//0xa6,0x07,
0x9c,0x20,
};

BYTE ov7720_camera_init_contrast_5 []=
{
//0xa6,0x07,
0x9c,0x28,
};

BYTE ov7720_camera_init_contrast_6 []=
{
//0xa6,0x07,
0x9c,0x30,
};

BYTE ov7720_camera_init_contrast_7 []=
{
//0xa6,0x07,
0x9c,0x38,
};

BYTE ov7720_camera_init_contrast_8 []=
{
//0xa6,0x07,
0x9c,0x40,
};

BYTE ov7720_camera_init_contrast_9 []=
{
//0xa6,0x07,
0x9c,0x50,
};

BYTE ov7720_camera_init_hue_0 []=
{
//0xa6,0x07,
0xa9,0x50,
0xaa,0x00,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_1 []=
{
//0xa6,0x07,
0xa9,0x58,
0xaa,0x00,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_2 []=
{
//0xa6,0x07,
0xa9,0x60,
0xaa,0x00,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_3 []=
{
//0xa6,0x07,
0xa9,0x68,
0xaa,0x00,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_4 []=
{
//0xa6,0x07,
0xa9,0x70,
0xaa,0x00,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_5 []=
{
//0xa6,0x07,
0xa9,0x78,
0xaa,0x08,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_6 []=
{
//0xa6,0x07,
0xa9,0x80,
0xaa,0x10,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_7 []=
{
//0xa6,0x07,
0xa9,0x88,
0xaa,0x18,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_8 []=
{
//0xa6,0x07,
0xa9,0x90,
0xaa,0x20,
0xab,0x05,
};

BYTE ov7720_camera_init_hue_9 []=
{
//0xa6,0x07,
0xa9,0xa0,
0xaa,0x28,
0xab,0x05,
};

BYTE ov7720_camera_init_saturation_0 []=
{
//0xa6,0x07,
0xa7,0x10,
0xa8,0x08,
};

BYTE ov7720_camera_init_saturation_1 []=
{
//0xa6,0x07,
0xa7,0x20,
0xa8,0x10,
};

BYTE ov7720_camera_init_saturation_2 []=
{
//0xa6,0x07,
0xa7,0x28,
0xa8,0x20,
};

BYTE ov7720_camera_init_saturation_3 []=
{
//0xa6,0x07,
0xa7,0x30,
0xa8,0x30,
};

BYTE ov7720_camera_init_saturation_4 []=
{
//0xa6,0x07,
0xa7,0x40,
0xa8,0x40,
};

BYTE ov7720_camera_init_saturation_5 []=
{
//0xa6,0x07,
0xa7,0x50,
0xa8,0x50,
};

BYTE ov7720_camera_init_saturation_6 []=
{
//0xa6,0x07,
0xa7,0x60,
0xa8,0x60,
};

BYTE ov7720_camera_init_saturation_7 []=
{
//0xa6,0x07,
0xa7,0x80,
0xa8,0x80,
};

BYTE ov7720_camera_init_saturation_8 []=
{
//0xa6,0x07,
0xa7,0xa0,
0xa8,0xa0,
};

BYTE ov7720_camera_init_saturation_9 []=
{
//0xa6,0x07,
0xa7,0xc0,
0xa8,0xc0,
};

BYTE ov7720_camera_init_sharpness_0 []=
{
0xac,0xDF,
0x8f,0x01,
};

BYTE ov7720_camera_init_sharpness_1 []=
{
0xac,0xDF,
0x8f,0x02,
};

BYTE ov7720_camera_init_sharpness_2 []=
{
0xac,0xDF,
0x8f,0x03,
};

BYTE ov7720_camera_init_sharpness_3 []=
{
0xac,0xDF,
0x8f,0x04,
};

BYTE ov7720_camera_init_sharpness_4 []=
{
0xac,0xdF,
0x8f,0x06,
};

BYTE ov7720_camera_init_sharpness_5 []=
{
0xac,0xDF,
0x8f,0x08,
};

BYTE ov7720_camera_init_sharpness_6 []=
{
0xac,0xDF,
0x8f,0x0a,
};

BYTE ov7720_camera_init_sharpness_7 []=
{
0xac,0xDF,
0x8f,0x10,
};

BYTE ov7720_camera_init_sharpness_8 []=
{
0xac,0xDF,
0x8f,0x12,
};

BYTE ov7720_camera_init_sharpness_9 []=
{
0xac,0xDF,
0x8f,0x14,
};

BYTE ov7720_camera_lowlight_en [] =
{
0x0e,0xf5,
};

BYTE ov7720_camera_lowlight_dis [] =
{
0x0e,0x01,
0x2d,0x00,
0x2e,0x00,
};

BYTE ov7720_camera_50hz_internal_clock [] =
{
0x2b,0xb2,//ipcam 
//0x2b,0xb2,//videophone
};

BYTE ov7720_camera_50hz_external_clock [] =
{
//0x2b,0xa2,//ipcam 
0x2a,0x12,
0x2b,0x12,//videophone
};

BYTE ov7720_camera_60hz_internal_clock [] =
{
0x2b,0x12,//ipcam
//0x2b,0x6e,//videophone
};

BYTE ov7720_camera_60hz_external_clock [] =
{
//0x2b,0x10,//ipcam
0x2a,0x02,
0x2b,0x6e,//videophone
};

BYTE ov7720_camera_indoor [] =
{

};

BYTE ov7720_camera_outdoor [] =
{

};

#endif

