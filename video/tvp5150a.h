/*
***************************************************************************
*                   Copyright (c) 2005 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : Tvp5150a.h             
*                                                                         
* Description : Functions for Tvp5150a      
*                                                
* By          :              
*
* Date        : Dec.13, 2007                                                           
***************************************************************************
*/

#ifndef __TVP5150A_H__
#define __TVP5150A_H__

#include "def_type.h"

#define REG_ID_MSB 0x80
#define REG_ID_LSB  0x81
#define STATUS_REG_1  0x88
#define STATUS_REG_5  0x8C
#define TVP5150A_IIC_ADDR 0x5D     //I2CSEL is 1
//#define TVP5150A_IIC_ADDR 0x5C     //I2CSEL is 0

/*prototype functions declaration*/
void HwTvp5150aInit(void);
int HwTvp5150aInitialize(BOOL video_system);
void SetTvp5150aBrightness(void);
void SetTvp5150aContrast(void);
void SetTvp5150aHue(void);
void SetTvp5150aSaturation(void);
void SetTvp5150aSharpness(void);
int GetTvp5150aID(void);
unsigned char GetTvp5150aVStd(unsigned int *vstd);

#endif        //__TVP5150A_H__

