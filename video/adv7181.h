/*******************************************************************************
  Panview 2.1 Board									       
  Copyright(C) 2002 WIPRO TECHNOLOGIES, Embedded & Internet Access Solutions  
  All rights reserved.                                                        
                                                                      
  FILENAME                  : adv7181.h			    		               
                                                                              
  AUTHOR                    :                                            
                                                                              
  DESCRIPTION               : This file contains the values to program the ADV7181 Decoder
 				
                                                                              
  REVISION DATE             : 11-18-04                                        
  REVISION DESCRIPTION      : created                                         
 *******************************************************************************/
#ifndef		__ADV7181_H__
#define		__ADV7181_H__

#include "includes.h"
#include "def_type.h"
#include "VIN_API.h"

/*
 * I2C Slave addr of ADV7181(with ALSB = 0)
*/
//#define ADV7181_READ_ADDR     0x41
//#define ADV7181_WRITE_ADDR    0x40
#define ADV7181_IIC_ADDR        0x20
//--------------------------------------------------------

//#define ADV_REGSIZE		ADV_REGISTERS*2
//#define ADV_REGISTERS 	28	//27.05.2005 removes black bar on LCD monitor		//was 28, eric change it to 29
//#define ADV_REGISTERS 	33	//27.05.2005 removes black bar on LCD monitor		//was 28, paul change it to 29
#define ADV_REGISTERS_READ 	227
//--------------------------------------------------------
// ADV 7181 Registers
//--------------------------------------------------------

#define IN_CTRL_REG				0x00
#define VIDEO_SEL_REG			0x01
#define VIDEO_ENH_CTRL_REG		0x02
#define OUT_CTRL_REG			0x03	
#define EXTND_OUT_CTRL_REG		0x04


#define AUTODETECT_EN__REG		0x07
#define CONTRAST_ADJ__REG		0x08
#define SAT_ADJ_REG				0x09   //(Reserved)
#define BRIGHT_ADJ_REG			0x0A
#define HUE_ADJ_REG				0x0B
#define DEFLT_Y_VAL_REG			0x0C
#define DEFLT_C_VAL_REG			0x0D
#define ADI_CTRL_REG            0x0E
#define POWER_MNG_REG			0x0F
#define STATUS1_REG				0x10   //(Read only)
#define INFO_REG				0x11   //(Read only)
#define STATUS2_REG				0x12   //(Read only)
#define STATUS3_REG				0x13   //(Read only)
#define ANLG_CLMP_CTRL_REG		0x14
#define DGTL_CLMP_CTRL1_REG		0x15

#define SHAPING_FLTR_CTRL1_REG	0x17
#define SHAPING_FLTR_CTRL2_REG	0x18
#define COMB_FLTR_CTRL_REG		0x19



#define PIXEL_DLY_CTRL_REG		0x27


#define MISC_GAIN_CTRL_REG		0x2B
#define AGC_MODE_CTRL_REG		0x2C
#define CHROMA_GAIN_CTRL1_REG	0x2D
#define CHROMA_GAIN_CTRL2_REG	0x2E
#define LUMA_GAIN_CTRL1_REG		0x2F
#define LUMA_GAIN_CTRL2_REG		0x30
#define VSYNC_FIELD_CTL1_REG    0x31
#define VSYNC_FIELD_CTL2_REG    0x32
#define VSYNC_FIELD_CTL3_REG    0x33
#define HSYNC_POSTN_CTRL1_REG	0x34
#define HSYNC_POSTN_CTRL2_REG	0x35
#define HSYNC_POSTN_CTRL3_REG	0x36
#define POLARITY_REG 			0x37
#define NTSC_COMB_CTL_REG 		0x38
#define PAL_COMB_CTL_REG 		0x39
#define ADC_CTL_REG				0x3A


#define MAN_WIN_REG				0x3D


#define CTI_DNR_CTL1_REG		0x4D
#define CTI_DNR_CTL2_REG		0x4E

#define CTI_DNR_CTL4_REG		0x50
#define LOCK_CNT_REG			0x51

#define RES1_REG				0x89  //(Reserved)
#define RES2_REG				0x8D  //(Reserved)

#define FREERUN_LEN1_REG		0x8F

#define RES3_REG				0xB5  //(Reserved)

#define ADC_SWITCH1_REG			0xC3
#define ADC_SWITCH2_REG			0xC4

#define RES4_REG				0xD2  //(Reserved) 
#define RES5_REG				0xD3  //(Reserved)
#define RES6_REG				0xD4  //(Reserved)
#define RES7_REG				0xD6  //(Reserved)
#define RES8_REG				0xD8  //(Reserved)

#define SD_OFFSET_CB_REG		0xE1
#define SD_OFFSET_CR_REG		0xE2
#define SD_SAT_CB_REG    		0xE3
#define SD_SAT_CR_REG    		0xE4
#define NTSC_VBIT_BEG_REG		0xE5
#define NTSC_VBIT_END_REG		0xE6
#define NTSC_FBIT_TOG_REG		0xE7
#define PAL_VBIT_BEG_REG		0xE8
#define PAL_VBIT_END_REG		0xE9
#define PAL_FBIT_TOG_REG		0xEA


//--------------------------------------------------------

int HwADV7181Initialize(BOOL video_system);
void HwAdv7181Init(void);
void SetAdv7181Brightness(void);
void SetAdv7181Contrast(void);
void SetAdv7181Hue(void);
void SetAdv7181Saturation(void);
void SetAdv7181Sharpness(void);
int GetAdv7181ID(void);
unsigned char ADV7181AutodetectVMode(unsigned int *vmode);


#endif //	__ADV7181_H__

//--------------------------------------------------------

