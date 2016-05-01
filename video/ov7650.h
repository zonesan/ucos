//#if (defined(OV7650 ) || defined(ALL_DIGIT_CAMEAR))
#ifndef _OV7650_H
#define _OV7650_H

//#define _OV7650_H

#include "def_type.h"
#include "chip_cfg.h"

#define REG_PID 	0xA
#define REG_VER 0xB


#define OV7650_CAMERA_IIC_ADDR  0x21
#define	OV7650_CAMERA_REG_SIZE	32*2


BYTE OV7650_camera_init[]=
{
//0x12, 0x80,        //reset all registers to default values
0x01, 0x00,        //BLUE
0x02, 0x80,        //RED
0x03, 0x24,        //Saturation value Bit[7:4]
0x04, 0x34,        //HUE
0x05, 0x3E,        //CWF
0x06, 0x5A,        //BRT

#ifdef FPGA
0x11, 0x0D,        //00: 30fps: 01: 12.25MHz, 02: 7.5fps, 0B: 2.5MHz, 0D:1.75MHz
#endif 

#ifdef CHIP
//0x11, 0x00,        //00: 30fps: 01: 12.25MHz, 02: 7.5fps, 0B: 2.5MHz, 0D:1.75MHz
0x11, 0x80,
#endif

0x12, 0x15,        //output format UYVY
0x13, 0xb3,        //b0: ITU656 format, a0: YUV, bit[0]-AEC, bit[1]-AGC
//0x10, 0x0C,        //AEC exposure time MSB
0x10, 0x5C,        //AEC exposure time MSB
0x00, 0x00,        //GAIN 
0x14, 0x04,        //04-for vga// 0x24 -for QVGA resolution
//0x15, 0x45,        //output format UYVY, Y[7:0] output when PCLK falling edge (04-falling edge; 44-rising edge)
0x15, 0x44,        //output format UYVY, Y[7:0] output when PCLK falling edge (04-falling edge; 44-rising edge)

0x16, 0x00,        //add by hjm, 
0x17, 0x1a,        //add by hjm, Out format-Horizontal frame (HREF column) start 
0x18, 0xba,        //add by hjm, Out format-Horizontal frame (HREF column) stop
0x19, 0x03,        //add by hjm, Out format-Vertical frame (Row) start    
0x1a, 0xf3,        //add by hjm, Out format-Verticalframe (Row) stop
0x1e, 0x00,        //add by hjm, 

0x1F, 0x40,        //FACT
0x20, 0xd0,        //COME
0x24, 0x74,        //AEW
0x25, 0x84,        //AEB
0x26, 0x32,        //COMF    
0x27, 0xF2,        //COMG
0x28, 0x20,        //20 -progressive ;00-interlaced
0x2a, 0x80,        //Frame rate Adjust
0x2b, 0x00,        //Frame rate adjust setting LSB
0x2d, 0x05,        //COMJ
0x60, 0x06,        //Signal Process Control B
0x6c, 0x11,        //Color matrix - R channel
0x6d, 0x33,        //Color matrix - G channel
0x6e, 0x22,        //Color matrix - B channel
0x74, 0x00,        //COMM
//0x75, 0x02,        //COMN
0x76, 0x02,        //AEC exposure time LSB bit0, 1
0x84, 0x00        //0x00: VGA, 0xC0:CIF, 0xA0:QCIF
};

#endif
