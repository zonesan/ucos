#ifndef ADV7171_H
#define ADV7171_H

#include "includes.h"
#include "def_type.h"
#include "VIN_API.h"

#define ADV7171_IIC_ADDR            0x2a

/* ADV7171 SUBADDRESS REGISTER */     
#define MODE_0_REG                			0x00
#define MODE_1_REG                			0x01
#define MODE_2_REG                			0x02
#define MODE_3_REG                			0x03
#define MODE_4_REG                			0x04
#define TIME_MODE_0_REG           			0x07
#define TIME_MODE_1_REG           			0x08
#define SUB_CARRIER_FREQ_0_REG    		0x09
#define SUB_CARRIER_FREQ_1_REG    		0x0a
#define SUB_CARRIER_FREQ_2_REG    		0x0b
#define SUB_CARRIER_FREQ_3_REG    		0x0c
#define SUB_CARRIER_PHASE_REG     		0x0d
#define CLOSE_CAPT_EXT_DATA_0_REG 	0x0e
#define CLOSE_CAPT_EXT_DATA_1_REG 	0x0f
#define CLOSE_CAPT_DATA_0_REG     		0x10
#define CLOSE_CAPT_DATA_1_REG     		0x11
// Pedestal is used in NTSC mode
#define NTSC_PEDESTAL_CONTROL_0_REG    0x12
#define NTSC_PEDESTAL_CONTROL_1_REG    0x13
#define NTSC_PEDESTAL_CONTROL_2_REG    0x14
#define NTSC_PEDESTAL_CONTROL_3_REG    0x15
// Teletext is used in PAL mode
#define PAL_TTX_CONTROL_0_REG    		0x12
#define PAL_TTX_CONTROL_1_REG    		0x13
#define PAL_TTX_CONTROL_2_REG    		0x14
#define PAL_TTX_CONTROL_3_REG    		0x15
#define	CGMS_WSS_0					0x16
#define	CGMS_WSS_1					0x17
#define	CGMS_WSS_2					0x18
#define TELETEXT_REQ_CONTROL_REG		0x19



// Mode Registers
#define MR0_CHROMA_FILTER         0x1    	// 000 - 1.3MHZ low pass filter
                                         					// 001 - 0.65MHZ low pass filter
                                         					// 010 - 1.0MHZ low pass filter
                                         					// 011 - 2.0MHZ low pass filter
                                         					// 101 - CIF 
                                         					// 110 - Q CIF
#define MR0_LUMA_FILTER           0x2    		// 000 - low pass filter(NTSC)
                                         					// 001 - low pass filter(PAL)
                                         					// 010 - NOTCH filter(NTSC)
                                         					// 011 - NOTCH filter(PAL)
                                         					// 100 - Extended mode
                                         					// 101 - CIF
                                         					// 110 - QCIF
#define MR0_OUTPUT_VIDEO_STANDARD_SELECT 	0x0  // 00 - NTSC;  01 - PAL(BDGHI);  11 - PAL(M)
#define MR1_COLORBAR_CONTROL      				0x0    // 0 - disabled;  1 - enabled
#define MR1_DAC_A_control         					0x0    // 0 - normal; 1 - power down
#define MR1_DAC_B_control         					0x1    // 0 - normal; 1 - power down
#define MR1_DAC_D_control         					0x1    // 0 - normal; 1 - power down
#define MR1_DAC_C_control         					0x1    // 0 - normal; 1 - power down
#define MR1_CLOSED_CAPT_FILED_SELECT 			0x00  // 00 - No data out;  01 - ODD filed only
                                           							// 10 - EVEN field only; 11 - Both fileds
#define MR1_INTERLACE_CONTROL     				0x0    // 0 - interlaced;  1 - non interlaced
#define MR2_RESERVED                       				0x0    // 0 - reserved
#define MR2_LOW_POWER_MODE        				0x0    // 0 - disabled;  1 - enabled
#define MR2_BURST_CONTROL         				0x0    // 0 - enabled;  1 - disabled
#define MR2_CHROMINANCE_CONTROL   			0x0    // 0 - enable color;  1 - disable color
#define MR2_ACTIVE_LINE_DURATION  				0x1    // 0 - 720 pixels;  1 - 710 pixels / 702 pixels
#define MR2_GENLOCK_CONTROL       				0x0    // X0 - disable genlock; 01 - enable subcarrier reset pin
                                         								// 11 - enable RTC pin
#define MR2_SQUARE_PIXEL_CONTROL  			0x0    // 0 - disable; 1 - enable
#define MR3_INPUT_DEFAULT_COLOR   				0x0    // 0 - disabled; 1 - enabled
#define MR3_TTXRQ_BIT_MODE_CONTROL 			0x0   // 0 - normal;  1 - bit request
#define MR3_CHROMA_OUTPUT_SELECT  			0x0    // 0 - disabled; 1 - enabled;
#define MR3_DAC_OUTPUT            					0x0    // 0 - DAC A: composite, DAC B: Blue/Comp/U, 
                                         								//     DAC C: Red/Chroma/V, DAC D: Green/Luma/Y;
                                         								// 1 - DAC A: Green/Luma/Y, DAC B: Blue/Comp/U, 
                                         								//     DAC C: Red/Chroma/V, DAC D: composite
#define MR3_VBI_OPEN              					0x0    // 0 - disabled; 1 - enabled;
#define MR3_RESERVED              					0x0    // 00 - reserved
#define MR4_47                    						0x0    // 0 must be written to this bit
#define MR4_SLEEP_MODE_CONTROL    				0x0    // 0 - enabled; 1 - disabled;
#define MR4_PEDESTAL_CONTROL      				0x0    // 0 - pedestal off; 1 - pedestal on
#define MR4_VSYNC_3H              					0x0    // 0 - disabled;  1 - enabled
#define MR4_RGB_SYC               					0x0    // 0 - disabled;  1 - enabled
#define MR4_RGB_YUV_CONTROL       				0x0    // 0 - RGB output;  1 - YUV output
#define MR4_OUTPUT_SELECT         				0x0    // 0 - YC output; 1 - RGB/YUV output

// Timing Registers
#define TR0_PIXEL_PORT_CONTROL    				0x0    // 0 - 8 bit;  1 - 16 bit
#define TR0_LUMA_DELAY            					0x0    // 00 - 0ns delay; 01 - 74ns delay;
                                         					 			// 10 - 148ns delay; 11 - 222ns delay
#define TR0_BLANK_INPUT_CONTROL   				0x1    // 0 - enabled; 1 - disabled
#define TR0_TIMING_MODE_SELECTION  			0x0   // 00 - mode 0;  01 - mode 1;
                                         								// 10 - mode 2;  11 - mode 3;
#define TR0_MASTER_SLAVE_CONTROL  			0x0    // 0 - slave timing;  1 - master timing
#define TR1_HSYNC_TO_PIXEL_DATA_ADJUST 		0x0  // 00 - 0*Tpclk;  01 - 1*Tpclk; 
                                            							// 10 - 2*Tpclk;  11 - 3*Tpclk
#define TR1_HSYNC_TO_FILED_RISING_EDEG_DELAY 0x3 // (Mode 1 Only)  X0 - Tb
                                                 						// X1 - Tb + 32us
#define TR1_VSYNC_WIDTH           					0x3    // (Mode 2 Only)  00 - 1*Tpclk;  01 - 4*Tpclk;
                                        									//10 - 16*Tpclk; 11 - 128*Tpclk;
#define TR1_HSYNC_TO_FIELD_OR_VSYNC_DELAY 	0x0  // 00 - 0*Tpclk; 01 - 4*Tpclk; 
                                           							// 10 - 8*Tpclk; 11 - 16*Tpclk
#define TR1_HSYNC_WIDTH           					0x3    // 00 - 1*Tpclk;  01 - 4*Tpclk;
                                         								// 10 - 16*Tpclk;  11 - 128*Tpclk
//TELETEXT_CONTROL          
#define TC0_TTXREQ_RISING_EDGE_CONTROL    		0x0 // 0000 - 0 PCLK; ...; 1111 - 15 PCLK
#define TC0_TTXREQ_FALLING_EDGE_CONTROL   	0x0 // 0000 - 0 PCKL; ...; 1111 - 15 PCLK

#define CGMS_WSS0_WIDE_SCREEN_SIGNAL_CONTROL 	0x0 // 0 - disable; 1 - enable
#define CGMS_WSS0_EVEN_FIELD_CONTROL 			0x0 // 0 - disable; 1 - enable
#define CGMS_WSS0_ODD_FIELD_CONTROL  			0x0 // 0 - disable; 1 - enable
#define CGMS_WSS0_CRC_CHECK_CONTROL  			0x0 // 0 - disable; 1 - enable


// Function Discriptions -----------------------------------------

/** @param : video_system              0   for NTSC,     1 for PAL
**/
int HwADV7171Initialize(BOOL video_system);
int GetADV7171ID(void);
VOID AppDisplayColorBlock(SHORT x, SHORT y, SHORT size_x, SHORT size_y, BYTE color);
VOID ADV7171_EnableColorBar(VOID);
void Adv7171_DisableColorBar(VOID);


#endif


