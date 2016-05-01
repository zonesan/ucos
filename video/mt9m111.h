//#if (defined(OV7670 ) || defined(ALL_DIGIT_CAMEAR))
#ifndef _MT9M111_H
#define _MT9M111_H

//#define _OV7670_H

#include "def_type.h"

#define CHIP_VERSION_ADDR	0x0

#define mt9m111_camera_IIC_ADDR  0x5D 
#define mt9m111_camera_REG_SIZE  13*2

int mt9m111_camera_init_30fps[]=
{
/*
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x00D1,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x0359,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x0359,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x020D,     // Context A Flicker full frame time (60Hz)
0x58, 0x0276,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x120D,     // 60Hz Flicker Search Range
0x5D, 0x1712,     // 50Hz Flicker Search Range
0x64, 0x5E1C,     // Flicker parameter
//[frame rate to be limited at 30fps]
0x37, 0x0080,	// SHUTTER_WIDTH_LIM_AE 30fps
*/
0xf0,0x0000,
0x05,0x01EA,     // Context B (full-res) Horizontal Blank
0x06,0x0005,     // Context B (full-res) Vertical Blank
0x07,0x00D1,     // Context A (preview) Horizontal Blank
0x08,0x0005,     // Context A (preview) Vertical Blank
0x20,0x0300,     // Read Mode Context B
0x21,0x000C,     // Read Mode Context A
0xf0,0x0002,
0x39,0x0359,     // AE Line size Context A
0x3A,0x06F2,     // AE Line size Context B
0x3B,0x0359,     // AE shutter delay limit Context A
0x3C,0x06F2,     // AE shutter delay limit Context B
0x57,0x020D,     // Context A Flicker full frame time (60Hz)
0x58,0x0276,     // Context A Flicker full frame time (50Hz)
0x59,0x00FD,     // Context B Flicker full frame time (60Hz)
0x5A,0x012F,     // Context B Flicker full frame time (50Hz)
0x5C,0x120D,     // 60Hz Flicker Search Range
0x5D,0x1712,     // 50Hz Flicker Search Range
0x64,0x5E1C,     // Flicker parameter
};

int mt9m111_camera_init_25fps[]=
{
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x017D,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x0405,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x0405,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x01B5,     // Context A Flicker full frame time (60Hz)
0x58, 0x020C,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x0F0B,     // 60Hz Flicker Search Range
0x5D, 0x130F,     // 50Hz Flicker Search Range
0x64, 0x5E1C,     // Flicker parameter
};

int mt9m111_camera_init_15fps[]=
{
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x042A,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x06B2,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x06B2,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x0106,     // Context A Flicker full frame time (60Hz)
0x58, 0x013B,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x120D,     // 60Hz Flicker Search Range
0x5D, 0x1712,     // 50Hz Flicker Search Range
0x64, 0x1E1C,     // Flicker parameter
};

int mt9m111_camera_init_10fps[]=
{
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x0783,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x0A0B,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x0A0B,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x00AF,     // Context A Flicker full frame time (60Hz)
0x58, 0x00D2,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x0C08,     // 60Hz Flicker Search Range
0x5D, 0x100C,     // 50Hz Flicker Search Range
0x64, 0x1E1C,     // Flicker parameter
};

int mt9m111_camera_init_7_5fps[]=
{
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x0ADD,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x0D65,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x0D65,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x0083,     // Context A Flicker full frame time (60Hz)
0x58, 0x009D,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x0905,     // 60Hz Flicker Search Range
0x5D, 0x0D09,     // 50Hz Flicker Search Range
0x64, 0x1E1C,     // Flicker parameter
};

int mt9m111_camera_init_5fps[]=
{
0xf0, 0x0000,
0x05, 0x0153,     // Context B (full-res) Horizontal Blank
0x06, 0x0005,     // Context B (full-res) Vertical Blank
0x07, 0x118F,     // Context A (preview) Horizontal Blank
0x08, 0x0005,     // Context A (preview) Vertical Blank
0x20, 0x0300,     // Read Mode Context B
0x21, 0x000C,     // Read Mode Context A
0xf0, 0x0002,
0x39, 0x1417,     // AE Line size Context A
0x3A, 0x065B,     // AE Line size Context B
0x3B, 0x1417,     // AE shutter delay limit Context A
0x3C, 0x065B,     // AE shutter delay limit Context B
0x57, 0x0057,     // Context A Flicker full frame time (60Hz)
0x58, 0x0068,     // Context A Flicker full frame time (50Hz)
0x59, 0x0114,     // Context B Flicker full frame time (60Hz)
0x5A, 0x014B,     // Context B Flicker full frame time (50Hz)
0x5C, 0x0602,     // 60Hz Flicker Search Range
0x5D, 0x0A06,     // 50Hz Flicker Search Range
0x64, 0x1E1C,     // Flicker parameter
};

int mt9m111_camera_init_vga_volatile []=
{
//init vga size
0xf0,0x0000,
0x34,0xC039,  //(1) BOOST_CONTROL_REG
0xf0,0x0001,
0x05,0x000B,  //(1) APERTURE_GAIN
0x06,0x708E,  //(3) MODE_CONTROL
0x25,0x002D, //(1) AWB_SPEED_SATURATION
0x34,0x0000,  //(1) LUMA_OFFSET
0x35,0xFF00,  //(1) CLIPPING_LIM_OUT_LUMA
0x4C,0x0001,  //(1) DEFECT_CONTEXT_A
0x4D,0x0001, //(1) DEFECT_CONTEXT_B
0x53,0x1A08, //(1) GAMMA_A_Y1_Y2
0x54,0x603D, //(1) GAMMA_A_Y3_Y4
0x55,0xAB8C,        //(1) GAMMA_A_Y5_Y6
0x56,0xDAC4,        //(1) GAMMA_A_Y7_Y8
0x57,0xFFED, //(1) GAMMA_A_Y9_Y10
0xAF,0x0018, //(1) REDUCER_ZOOM_CONTROL
0xDC,0x1A08,        //(1) GAMMA_B_Y1_Y2
0xDD,0x603D,        //(1) GAMMA_B_Y3_Y4
0xDE,0xAB8C,       //(1) GAMMA_B_Y5_Y6
0xDF,0xDAC4,       //(1) GAMMA_B_Y7_Y8
0xE0,0xFFED, //(1) GAMMA_B_Y9_Y10
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xae,0x0A09,  //use VGA Output size aspect ratios 
0xa7,0x0280, //VGA
0xaa,0x01E0, //VGA
0x3b,0x042a, //black level
0xf0,0x0002,
0x1F,0x0090,  //(1) AWB_CR_CB_LIMITS
0x22,0x9080,  //(1) AWB_RED_LIMIT
0x23,0x8878,  //(1) AWB_BLUE_LIMIT
0x24,0x5F20,  //(1) MATRIX_ADJ_LIMITS
0x28,0xEF02,  //(1) AWB_ADVANCED_CONTROL_REG
0x29,0x867A, //(1) AWB_WIDE_GATES
0x2E,0x0C44, //(1) AE_PRECISION_TARGET
0x39,0x0618,  //(1) AE_LINE_SIZE_REG_60HZ
0x3A,0x0618, //(1) AE_LINE_SIZE_REG_50HZ
0x3B,0x0618,  //(1) AE_LIMIT_SHUTTER_DELAY_60HZ
0x3C,0x0618,  //(1) AE_LIMIT_SHUTTER_DELAY_50HZ
0x57,0x0200,  //(1) AE_WIDTH_60HZ_PREVIEW
0x58,0x0267,  //(1) AE_WIDTH_50HZ_PREVIEW
0x59,0x0200,  //(1) AE_WIDTH_60HZ_FULLRES
0x5A,0x0267, //(1) AE_WIDTH_50HZ_FULLRES
0x5C,0x120D, //(1) SEARCH_FLICKER_60
0x5D,0x1611, //(1) SEARCH_FLICKER_50
0x5E,0x674C, //(1) RATIO_BASE_REG
0x5F,0x7959,  //(1) RATIO_DELTA_REG
0x60,0x0002,  //(1) SIGNS_DELTA_REG
0x64,0x7B5B, //(1) FLICKER_CONTROL_2
0xCE,0x5E9B, //(1) LED_FLASH_CONFIG
};

int  mt9m111_camera_init_sxga_volatile []=
{
//init sxga size
0xf0, 0x0001,
0x9D, 0x3CA0,	// DEFECT_CORRECTION
0xf0, 0x0002,
0x24, 0x5F20,	// MATRIX_ADJ_LIMITS
0x28, 0xEF02,	// AWB_ADVANCED_CONTROL_REG
0x5F, 0x4143,	// RATIO_DELTA_REG
0x02, 0x00EE,	// BASE_MATRIX_SIGNS
0x03, 0x291A,	// BASE_MATRIX_SCALE_K1_K5
0x04, 0x02A4,	// BASE_MATRIX_SCALE_K6_K9
0x09, 0x0068,	// BASE_MATRIX_COEF_K1
0x0A, 0x002A,	// BASE_MATRIX_COEF_K2
0x0B, 0x0004,	// BASE_MATRIX_COEF_K3
0x0C, 0x0093,	// BASE_MATRIX_COEF_K4
0x0D, 0x0082,	// BASE_MATRIX_COEF_K5
0x0E, 0x0040,	// BASE_MATRIX_COEF_K6
0x0F, 0x005F,	// BASE_MATRIX_COEF_K7
0x10, 0x004E,	// BASE_MATRIX_COEF_K8
0x11, 0x005B,	// BASE_MATRIX_COEF_K9
0x15, 0x00C9,	// DELTA_COEFS_SIGNS
0x16, 0x005E,	// DELTA_MATRIX_COEF_D1
0x17, 0x009D,	// DELTA_MATRIX_COEF_D2
0x18, 0x0006,	// DELTA_MATRIX_COEF_D3
0x19, 0x0089,	// DELTA_MATRIX_COEF_D4
0x1A, 0x0012,	// DELTA_MATRIX_COEF_D5
0x1B, 0x00A1,	// DELTA_MATRIX_COEF_D6
0x1C, 0x00E4,	// DELTA_MATRIX_COEF_D7
0x1D, 0x007A,	// DELTA_MATRIX_COEF_D8
0x1E, 0x0064,	// DELTA_MATRIX_COEF_D9
0xF6, 0x005F,	// FLASH_WB_POS
0xf0, 0x0001,
0x06, 0x600E,	// MODE_CONTROL
0x06, 0x600E,	// MODE_CONTROL
0x81, 0x0000,	// LENS_ADJ_VERT_RED_0
0x82, 0x0000,	// LENS_ADJ_VERT_RED_1_2
0x83, 0x0000,	// LENS_ADJ_VERT_RED_3_4
0x84, 0x0000,	// LENS_ADJ_VERT_GREEN_0
0x85, 0x0000,	// LENS_ADJ_VERT_GREEN_1_2
0x86, 0x0000,	// LENS_ADJ_VERT_GREEN_3_4
0x87, 0x0000,	// LENS_ADJ_VERT_BLUE_0
0x88, 0x0000,	// LENS_ADJ_VERT_BLUE_1_2
0x89, 0x0000,	// LENS_ADJ_VERT_BLUE_3_4
0x8A, 0x0000,	// LENS_ADJ_HORIZ_RED_0
0x8B, 0x0000,	// LENS_ADJ_HORIZ_RED_1_2
0x8C, 0x0000,	// LENS_ADJ_HORIZ_RED_3_4
0x8D, 0x0000,	// LENS_ADJ_HORIZ_RED_5
0x8E, 0x0000,	// LENS_ADJ_HORIZ_GREEN_0
0x8F, 0x0000,	// LENS_ADJ_HORIZ_GREEN_1_2
0x90, 0x0000,	// LENS_ADJ_HORIZ_GREEN_3_4
0x91, 0x0000,	// LENS_ADJ_HORIZ_GREEN_5
0x92, 0x0000,	// LENS_ADJ_HORIZ_BLUE_0
0x93, 0x0000,	// LENS_ADJ_HORIZ_BLUE_1_2
0x94, 0x0000,	// LENS_ADJ_HORIZ_BLUE_3_4
0x95, 0x0000,	// LENS_ADJ_HORIZ_BLUE_5
0xB6, 0x0000,	// LENS_ADJ_VERT_RED_5_6
0xB7, 0x0000,	// LENS_ADJ_VERT_RED_7_8
0xB8, 0x0000,	// LENS_ADJ_VERT_GREEN_5_6
0xB9, 0x0000,	// LENS_ADJ_VERT_GREEN_7_8
0xBA, 0x0000,	// LENS_ADJ_VERT_BLUE_5_6
0xBB, 0x0000,	// LENS_ADJ_VERT_BLUE_7_8
0xBC, 0x0000,	// LENS_ADJ_HORIZ_RED_6_7
0xBD, 0x0000,	// LENS_ADJ_HORIZ_RED_8_9
0xBE, 0x0000,	// LENS_ADJ_HORIZ_RED_10
0xBF, 0x0000,	// LENS_ADJ_HORIZ_GREEN_6_7
0xC0, 0x0000,	// LENS_ADJ_HORIZ_GREEN_8_9
0xC1, 0x0000,	// LENS_ADJ_HORIZ_GREEN_10
0xC2, 0x0000,	// LENS_ADJ_HORIZ_BLUE_6_7
0xC3, 0x0000,	// LENS_ADJ_HORIZ_BLUE_8_9
0xC4, 0x0000,	// LENS_ADJ_HORIZ_BLUE_10
0xA1, 0x0500,	// HORIZ_SIZE_RESIZE_B
0xA4, 0x0400,	// VERT_SIZE_RESIZE_B
0x08, 0x0080,  //enable correct value for ITU-R BT.656 
0x3a, 0x0800,  //enable ITU-R BT.656,output YCbCr  (context a) 
0x9b, 0x0800,  //enable ITU-R BT.656,output YCbCr  (context b)
0xf0, 0x0002,
0xD2, 0x0040,	// DEFAULT_CONFIG
0xD2, 0x0041,	// DEFAULT_CONFIG
0xD2, 0x0043,	// DEFAULT_CONFIG
0xD2, 0x0047,	// DEFAULT_CONFIG
0xD2, 0x004F,	// DEFAULT_CONFIG
0xCB, 0x0001,	// PROGRAM_ADVANCE
0x37, 0x0080,	// SHUTTER_WIDTH_LIM_AE
0xf0, 0x0000,
0x06, 0x000D,	// VERT_BLANK_B
};

int mt9m111_camera_init_cif_volatile []=
{
//init cif size
0xf0,0x0001,   //page 1
0x06,0x600e,  //mode control 
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xa7,0x0160, //cif
0xaa,0x0120, //cif
0xf0,0x0002,   //page 2
0xc8,0x0000,  //Global Context Control (context a)
0xd3,0x0000,  //User Global Context Control (context a)
};

int mt9m111_camera_init_qcif_volatile []=
{
//init qcif size
0xf0,0x0001,   //page 1
0x06,0x600e,  //mode control 
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xa7,0x00b0, //qcif
0xaa,0x0090, //qcif
0xf0,0x0002,   //page 2
0xc8,0x0000,  //Global Context Control (context a)
0xd3,0x0000,  //User Global Context Control (context a)
};

int mt9m111_camera_init_qvga_volatile []=
{
//init qvga size
0xf0,0x0001,   //page 1
0x06,0x600e,  //mode control 
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xae,0x0A09,  //use VGA Output size aspect ratios 
0xaf,0x0000,   //zoom out
0xa7,0x0140, //qvga
0xaa,0x00f0, //qvga
0xf0,0x0002,   //page 2
0xc8,0x0000,  //Global Context Control (context a)
0xd3,0x0000,  //User Global Context Control (context a)
};

int mt9m111_camera_init_qqvga_volatile []=
{
//init qqvga size
0xf0,0x0001,   //page 1
0x06,0x600e,  //mode control 
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xae,0x0A09,  //use VGA Output size aspect ratios 
0xaf,0x0000,   //zoom out
0xa7,0x00a0, //qvga
0xaa,0x0078, //qvga
0xf0,0x0002,   //page 2
0xc8,0x0000,  //Global Context Control (context a)
0xd3,0x0000,  //User Global Context Control (context a)
};

int mt9m111_camera_init_ntsc_d1_volatile []=
{
//init ntsc size
0xf0,0x0002,
0xCB,0x0001, //(4) PROGRAM_ADVANCE
0xCC,0x0004, //(4) PROGRAM_SELECT
0xD2,0x007F, //(4) DEFAULT_CONFIG
//0xC8,0X9F0B,
//0xD3,0X9F0B,
0xf0,0x0001,
0xA1,0x02c0, //(1) HORIZ_SIZE_RESIZE_B
0xA4,0x01E0, //(2) VERT_SIZE_RESIZE_B
0x08, 0x0080,  //enable correct value for ITU-R BT.656 
0x9b, 0x0800,  //enable ITU-R BT.656,output YCbCr  (context b)
/*
0xf0,0x0001,   //page 1
0x06,0x600e,  //mode control 
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xae,0x0A09,  //use VGA Output size aspect ratios 
0xaf,0x0000,   //zoom out
0xa1,0x02c0, //d1 
0xa4,0x01e0, //d1
0xf0,0x0002,   //page 2
0xc8,0x0400,  //Global Context Control (context B)
0xd3,0x0400,  //User Global Context Control (context B)
//0xD2,0x007F, // DEFAULT_CONFIG
//0xCC,0x0004, // PROGRAM_SELECT
//0xCB,0x0001, // PROGRAM_ADVANCE
*/
};

int mt9m111_camera_init_pal_d1_volatile []=
{
0xf0,0x0002,
0xCB,0x0001, //(4) PROGRAM_ADVANCE
0xCC,0x0004, //(4) PROGRAM_SELECT
0xD2,0x007F, //(4) DEFAULT_CONFIG
//0xC8,0X9F0B,
//0xD3,0X9F0B,
0xf0,0x0001,
0xA1,0x02c0, //(1) HORIZ_SIZE_RESIZE_B
0xA4,0x0240, //(2) VERT_SIZE_RESIZE_B
0x08, 0x0080,  //enable correct value for ITU-R BT.656 
0x9b, 0x0800,  //enable ITU-R BT.656,output YCbCr  (context b)
/*
//init pal size
0xf0,0x0001,
0x08,0x0080,  //enable correct value for ITU-R BT.656 
0x3a,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0x9b,0x0800,  //enable ITU-R BT.656,output YCbCr  (context a)
0xf0,0x0002,
0xCB,0x0001,	//(1) PROGRAM_ADVANCE //context switch
0xCC,0x0004,	//(1) PROGRAM_SELECT
0xD2,0x007F,	//(1) DEFAULT_CONFIG
0xf0,0x0001,
0xA1,0x02c0,	//(2) HORIZ_SIZE_RESIZE_B  //column 704
0xA4,0x0240,	//(2) VERT_SIZE_RESIZE_B   //row 576
0xA7,0x02c0,	//(2) HORIZ_SIZE_RESIZE_B  //column 704
0xAa,0x0240,	//(2) VERT_SIZE_RESIZE_B   //row 576
*/
};

int mt9m111_camera_init_brightness_minus5 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0c0d,   // AE_PRECISION_TARGET-5
};

int mt9m111_camera_init_brightness_minus4 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0c17,   // AE_PRECISION_TARGET-4
};

int mt9m111_camera_init_brightness_minus3 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C21,   // AE_PRECISION_TARGET-3
};

int mt9m111_camera_init_brightness_minus2 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C2b,   // AE_PRECISION_TARGET-2
};

int mt9m111_camera_init_brightness_minus1 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C35,   // AE_PRECISION_TARGET-1
};

int mt9m111_camera_init_brightness_zero [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C4a,   // AE_PRECISION_TARGET0
};

int mt9m111_camera_init_brightness_plus1 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C5f,   // AE_PRECISION_TARGET+1
};

int mt9m111_camera_init_brightness_plus2 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C77,   // AE_PRECISION_TARGET+2
};

int mt9m111_camera_init_brightness_plus3 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0C8c,   // AE_PRECISION_TARGET+3
};

int mt9m111_camera_init_brightness_plus4 [] =
{//Brightness:
0xf0,0x0002,
0x2E,0x0Ca1,   // AE_PRECISION_TARGET+4
};

int mt9m111_camera_init_sharpness_no [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0000,   // APERTURE_GAIN 0
};

int mt9m111_camera_init_sharpness_25percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0001,   // APERTURE_GAIN 25%
};

int mt9m111_camera_init_sharpness_50percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0002,   // APERTURE_GAIN 50%
};

int mt9m111_camera_init_sharpness_75percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0003,   // APERTURE_GAIN 75%
};

int mt9m111_camera_init_sharpness_100percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0004,  // APERTURE_GAIN 100%
};

int mt9m111_camera_init_sharpness_125percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0005,   // APERTURE_GAIN 125%
};

int mt9m111_camera_init_sharpness_150percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0006,   // APERTURE_GAIN 150%
};

int mt9m111_camera_init_sharpness_200percent [] =
{//Sharpness:
0xf0,0x0001,
0x05,0x0007,   // APERTURE_GAIN 200%
};

int mt9m111_camera_init_saturation_25percent [] =
{//Saturation:
0xf0,0x0001,
0x25,0x0025, // AWB_SPEED_SATURATION 25%
};

int mt9m111_camera_init_saturation_37_5percent [] =
{//Saturation:
0xf0,0x0001,
0x25,0x001d, // AWB_SPEED_SATURATION 37.5%
};

int mt9m111_camera_init_saturation_50percent [] =
{//Saturation:
0xf0,0x0001,
0x25,0x0015, // AWB_SPEED_SATURATION 50%
};

int mt9m111_camera_init_saturation_75percent [] =
{//Saturation:
0xf0,0x0001,
0x25,0x000d, // AWB_SPEED_SATURATION 75%
};

int mt9m111_camera_init_saturation_full [] =
{//Saturation:
0xf0,0x0001,
0x25,0x0005, // AWB_SPEED_SATURATION full
};

int mt9m111_camera_init_saturation_150percent [] =
{//Saturation:
0xf0,0x0001,
0x25,0x002D, // AWB_SPEED_SATURATION 150%
};

int mt9m111_camera_init_saturation_bw [] =
{//Saturation:
0xf0,0x0001,
0x25,0x0035, // AWB_SPEED_SATURATION black and white
};

int mt9m111_camera_init_contrast_1_0 [] =
{//[contrast 1.0]
0xf0,0x0001,
0x53,0x1104, //(4) GAMMA_A_Y1_Y2
0x54,0x5e28, //(4) GAMMA_A_Y3_Y4
0x55,0x8b7a, //(4) GAMMA_A_Y5_Y6
0x56,0xb5B1, //(4) GAMMA_A_Y7_Y8
0x57,0xE5DB, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_1_25 [] =
{//[contrast 1.25]
0xf0,0x0001,
0x53,0x1004, //(4) GAMMA_A_Y1_Y2
0x54,0x5828, //(4) GAMMA_A_Y3_Y4
0x55,0x8f79, //(4) GAMMA_A_Y5_Y6
0x56,0xbbB1, //(4) GAMMA_A_Y7_Y8
0x57,0xE8DB, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_1_5 [] =
{//[contrast 1.5]
0xf0,0x0001,
0x53,0x0f04, //(4) GAMMA_A_Y1_Y2
0x54,0x5228, //(4) GAMMA_A_Y3_Y4
0x55,0x9378, //(4) GAMMA_A_Y5_Y6
0x56,0xC1B1, //(4) GAMMA_A_Y7_Y8
0x57,0xEbDB, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_1_75 [] =
{//[contrast 1.75]
0xf0,0x0001,
0x53,0x0E04, //(4) GAMMA_A_Y1_Y2
0x54,0x4C28, //(4) GAMMA_A_Y3_Y4
0x55,0x9777, //(4) GAMMA_A_Y5_Y6
0x56,0xC7B1, //(4) GAMMA_A_Y7_Y8
0x57,0xEEDB, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_2_0 [] =
{//[contrast 2.0]
0xf0,0x0001,
0x53,0x0B04, //(14) GAMMA_A_Y1_Y2
0x54,0x4522, //(14) GAMMA_A_Y3_Y4
0x55,0x9B76, //(14) GAMMA_A_Y5_Y6
0x56,0xCEB8, //(14) GAMMA_A_Y7_Y8
0x57,0xF1E1, //(14) GAMMA_A_Y9_Y10
0x58,0xFF00, //(14) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_2_25 [] =
{//[contrast 2.25]
0xf0,0x0001,
0x53,0x0A03, //(28) GAMMA_A_Y1_Y2
0x54,0x3F1D, //(28) GAMMA_A_Y3_Y4
0x55,0xA074, //(28) GAMMA_A_Y5_Y6
0x56,0xD4BE, //(28) GAMMA_A_Y7_Y8
0x57,0xF3E5, //(28) GAMMA_A_Y9_Y10
0x58,0xFF00, //(28) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_2_50 [] =
{//[contrast 2.50]
0xf0,0x0001,
0x53,0x0803, //(46) GAMMA_A_Y1_Y2
0x54,0x3919, //(46) GAMMA_A_Y3_Y4
0x55,0xA472, //(46) GAMMA_A_Y5_Y6
0x56,0xD9C4, //(46) GAMMA_A_Y7_Y8
0x57,0xF5E9, //(46) GAMMA_A_Y9_Y10
0x58,0xFF00, //(46) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_2_75 [] =
{//[contrast 2.75]
0xf0,0x0001,
0x53,0x0702, //(56) GAMMA_A_Y1_Y2
0x54,0x3416, //(56) GAMMA_A_Y3_Y4
0x55,0xA870, //(56) GAMMA_A_Y5_Y6
0x56,0xDEC9, //(56) GAMMA_A_Y7_Y8
0x57,0xF6EC, //(56) GAMMA_A_Y9_Y10
0x58,0xFF00, //(56) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_3_0 [] =
{//[contrast 3.0]
0xf0,0x0001,
0x53,0x0601, //(4) GAMMA_A_Y1_Y2
0x54,0x2e16, //(4) GAMMA_A_Y3_Y4
0x55,0xac6f, //(4) GAMMA_A_Y5_Y6
0x56,0xe3c9, //(4) GAMMA_A_Y7_Y8
0x57,0xf7ec, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_contrast_3_25 [] =
{//[contrast 3.25]
0xf0,0x0001,
0x53,0x0500, //(4) GAMMA_A_Y1_Y2
0x54,0x2816, //(4) GAMMA_A_Y3_Y4
0x55,0xb06e, //(4) GAMMA_A_Y5_Y6
0x56,0xe8c9, //(4) GAMMA_A_Y7_Y8
0x57,0xf8ec, //(4) GAMMA_A_Y9_Y10
0x58,0xFF00, //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_0 [] =
{//[Hue0]
0xf0,0x0001,  //page 1 
0x53,0x3020,  //(88) GAMMA_A_Y1_Y2
0x54,0x6c40,  //(88) GAMMA_A_Y3_Y4
0x55,0xa380,  //(88) GAMMA_A_Y5_Y6
0x56,0xa1B4,  //(88) GAMMA_A_Y7_Y8
0x57,0xa2DD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_5 [] =
{//[Hue5]
0xf0,0x0001,  //page 1 
0x53,0x2f20,  //(88) GAMMA_A_Y1_Y2
0x54,0x6640,  //(88) GAMMA_A_Y3_Y4
0x55,0xa180,  //(88) GAMMA_A_Y5_Y6
0x56,0xCeB4,  //(88) GAMMA_A_Y7_Y8
0x57,0xa1DD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_10 [] =
{//[Hue10]
0xf0,0x0001,  //page 1 
0x53,0x2e20,  //(88) GAMMA_A_Y1_Y2
0x54,0x6040,  //(88) GAMMA_A_Y3_Y4
0x55,0x9e80,  //(88) GAMMA_A_Y5_Y6
0x56,0xCcB4,  //(88) GAMMA_A_Y7_Y8
0x57,0xa0DD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_15 [] =
{//[Hue15]
0xf0,0x0001,  //page 1 
0x53,0x2D20,  //(88) GAMMA_A_Y1_Y2
0x54,0x5A40,  //(88) GAMMA_A_Y3_Y4
0x55,0x9C80,  //(88) GAMMA_A_Y5_Y6
0x56,0xCAB4,  //(88) GAMMA_A_Y7_Y8
0x57,0xEFDD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_20 [] =
{//[Hue20]
0xf0,0x0001,  //page 1 
0x53,0x1C0A,  //(74) GAMMA_A_Y1_Y2
0x54,0x5435,  //(74) GAMMA_A_Y3_Y4
0x55,0x9A7C,  //(74) GAMMA_A_Y5_Y6
0x56,0xC8B3,  //(74) GAMMA_A_Y7_Y8
0x57,0xEEDC,  //(74) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(74) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_25 [] =
{//[Hue 25]
0xf0,0x0001,  //page 1 
0x53,0x0E04,  //(4) GAMMA_A_Y1_Y2
0x54,0x4C28,  //(4) GAMMA_A_Y3_Y4
0x55,0x9777,  //(4) GAMMA_A_Y5_Y6
0x56,0xC7B1,  //(4) GAMMA_A_Y7_Y8
0x57,0xEEDB,  //(4) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(4) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_30 [] =
{//[Hue30]
0xf0,0x0001,  //page 1 
0x53,0x0903,  //(46) GAMMA_A_Y1_Y2
0x54,0x441B,  //(46) GAMMA_A_Y3_Y4
0x55,0x9473,  //(46) GAMMA_A_Y5_Y6
0x56,0xC6AF,  //(46) GAMMA_A_Y7_Y8
0x57,0xEDDB,  //(46) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(46) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_35 [] =
{//[hue35]
0xf0,0x0001,  //page 1 
0x53,0x0602,  //(60) GAMMA_A_Y1_Y2
0x54,0x3914,  //(60) GAMMA_A_Y3_Y4
0x55,0x916E,  //(60) GAMMA_A_Y5_Y6
0x56,0xC4AD,  //(60) GAMMA_A_Y7_Y8
0x57,0xEDDA,  //(60) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(60) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_40 [] =
{//[Hue40]
0xf0,0x0001,  //page 1 
0x53,0x0402,  //(88) GAMMA_A_Y1_Y2
0x54,0x4414,  //(88) GAMMA_A_Y3_Y4
0x55,0x8e6e,  //(88) GAMMA_A_Y5_Y6
0x56,0xC6B4,  //(88) GAMMA_A_Y7_Y8
0x57,0xEdDD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

int mt9m111_camera_init_hue_45 [] =
{//[Hue45]
0xf0,0x0001,  //page 1 
0x53,0x0200,  //(88) GAMMA_A_Y1_Y2
0x54,0x4f14,  //(88) GAMMA_A_Y3_Y4
0x55,0x8b6e,  //(88) GAMMA_A_Y5_Y6
0x56,0xC8B4,  //(88) GAMMA_A_Y7_Y8
0x57,0xEcDD,  //(88) GAMMA_A_Y9_Y10
0x58,0xFF00,  //(88) GAMMA_A_Y0_Y11
};

#endif
