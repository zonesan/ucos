#ifndef __VIN_API_H__
#define  __VIN_API_H__

#include "chip_cfg.h"
#include "video_cfg.h"
#include "def_type.h"
#define small_board

#include "../video_data/saved_vin_data.h"

#ifdef VIN_API_GLOBLS
#define VIN_API_EXT
#else 
#define VIN_API_EXT extern
#endif
#include"../inc/video_cfg.h"

                 
/*
*********************************************************************************************************
*              Definition for VIN, VOUT, RVO, PIP, OSD memory address, To be change by USer
*********************************************************************************************************
*/
VIN_API_EXT unsigned int VinBufferNumber;

VIN_API_EXT unsigned int UCOS_TO_LINUX_YUVBufferNumber;

// comment it to save data space
#ifdef NO_VIDEO_DATA
VIN_API_EXT unsigned int _vin_y_0[VIN_BUF_SIZE / sizeof(int) + 4];
#define vin_y_0    (((unsigned int)_vin_y_0 + 0x10) & 0xfffffff0)
//VIN_API_EXT unsigned int vin_y_0[1];
#else
//VIN_API_EXT unsigned int vin_y_0[1];            
#endif        

#define VIN_YUV_START_ADDR (((unsigned int)vin_y_0) & 0x1fffffff)    // use pfysical address
               
/*
*********************************************************************************************************
*                        Register setting for ADV7171
*********************************************************************************************************
*/
#ifdef			GC0306
#define CAMERA_SENSOR_IIC_ADDR  0x10
#endif


/*
*********************************************************************************************************
*              	Definition for VIN, and VOUT, Video height and width, to be change by User
*********************************************************************************************************
*/  
//Video clock control register
#define 	rVIN_CLOCK_ENABLE			0								/* 1: Enable VIN clock generator, 0:Disable VIN clock generator	*/
#define 	rVIN_PLL_SOURCE				0								/* 1: use RISC PLL, 0: use SDRAM PLL*/
#define 	rVIN_DIVIDER_TYPE			0								/* 1: Use accurate divider, 0: Use regular divider	*/
#define 	rVIN_DIVIDER_VALUE		0								/* Bit[28:0] The divider value adds to the divider counter. Clock inputs from bit[29] of the counter */

//VIN register
#define  	rVIN_CLK_POLORITY			1         			/* VIN_CTL bit[30], 0: posedge 1: negedge */
#define  	rVIN_CLK_POLORITY_POS		0         			/* VIN_CTL bit[30], 0: posedge 1: negedge */
#define  	rVIN_FRAME_PIC				1         			/* VIN_CTL bit[29], 0: interlaced pictue 1: progressive picture */
#define  	rVIN_FIRST_FIELD_ID  	0         			/* VIN_CTL bit[28], 0: First field in the frame when interlaced picture/ Interlace bit27~=bit28  */
#define  	rVIN_FMT       				1     			  	/* VIN_CTL bit[27], Video format 0: 4:2:2, 1: 4:2:0 */
#define  	rVIN_F0_DROP_LINE    	0         			/* VIN_CTL bit[26], 0: drop 2nd line UV@F=0 1: drop 1st line UV@F=0 */
#define  	rVIN_F1_DROP_LINE    	0         			/* VIN_CTL bit[25], 0: drop 2nd line UV@F=1 1: drop 1st line UV@F=1 */
#define 	rVIN_S_RATIO       		0       /* VIN_CTL bit[24:23], 00: no scale, 01: scaling down 2, 10:scaling down 4, 11: Reserved*/
#define 	rVIN_CLK_DIR					1								/* VIN_CTL bit[22], 0: vin_clk is output, 1: vin_clk is input*/
#define		rVIN_WIDTH  					gImageWidth/2 - 1		/* FM_SIZE bit[31:20], Active video Width	[31:20]	(640/2)-1 */
#define		rVIN_HEIGHT						(gImageHeight >> (gtVinFormat.interlaced == TRUE)) - 1		/* FM_SIZE bit[19:8], Active video Height [19:8]	(480-1) */

/*
*********************************************************************************************************
*                          Definition for I2C SDA and SCL
*********************************************************************************************************
*/
#ifdef FPGA
#define GPIO25_bit 25
#define GPIO26_bit 26
#endif

#ifdef CHIP
#define GPIO25_bit 13
#define GPIO26_bit 12
#endif

#define GPIO25_bitmask (1 << GPIO25_bit)
#define GPIO26_bitmask (1 << GPIO26_bit)
#define GPIO25_output()		rRISC_GPIO_DIR &= ~GPIO25_bitmask 			//Set GPIO25 as output
#define GPIO25_input()		rRISC_GPIO_DIR |= GPIO25_bitmask 			//Set GPIO25 as input
#define GPIO25_high()		rRISC_GPIO |= GPIO25_bitmask						//Set GPIO25 high
#define GPIO25_low()		rRISC_GPIO &= ~GPIO25_bitmask					//Set GPIO25 low
#define GPIO26_output()		rRISC_GPIO_DIR &= ~GPIO26_bitmask 			//Set GPIO26 as output
#define GPIO26_input()		rRISC_GPIO_DIR |= GPIO26_bitmask 			//Set GPIO26 as input
#define GPIO26_high()		rRISC_GPIO |= GPIO26_bitmask						//Set GPIO26 high
#define GPIO26_low()		rRISC_GPIO &= ~GPIO26_bitmask					//Set GPIO26 low


#ifdef FPGA
#define I2C_DELAY 	10
#endif

#ifdef CHIP
#define I2C_DELAY 	10*5	// 10*8
#endif

#define VID_IN_BASE_ADDR    					BLOCK_BASE_ADDR(VIN_BLOCK) 
#define VID_CLOCK_CTL_BASE_ADDR   		BLOCK_BASE_ADDR(CCTL_BLOCK) 
#define VID_SC_CTL_BASE_ADDR					BLOCK_BASE_ADDR(SC_BLOCK)
#define VIN_CLOCK_CTL			(*(volatile unsigned *)(VID_CLOCK_CTL_BASE_ADDR + 0x2C))			/* Video input clock control */  

#define VIN_CTL						(*(volatile unsigned *)(VID_IN_BASE_ADDR + 0x0))    					/* Input Video control register */
#define VIN_FM_SIZE    		(*(volatile unsigned *)(VID_IN_BASE_ADDR + 0x4))    					/* Video input frame size */
#define VIN_INTR_STS   		(*(volatile unsigned *)(VID_IN_BASE_ADDR + 0x8))    					/* Video input interrupt status */

/*
*********************************************************************************************************
*                          Video register bit definitions
*********************************************************************************************************
*/

// Video input control register VIN_CTL bit definition
#define BIT_VIN_ENA          			(0x1<<31)
#define BIT_VIN_CLK_POLORITY			(rVIN_CLK_POLORITY<<30)
#define BIT_VIN_CLK_POLORITY_POS			(rVIN_CLK_POLORITY_POS<<30)
#define BIT_VIN_FRAME_PIC       	(rVIN_FRAME_PIC<<29)
#define BIT_VIN_FIRST_FIELD_ID		(rVIN_FIRST_FIELD_ID<<28)
#define BIT_VIN_FMT       				(rVIN_FMT<<27)
#define BIT_VIN_F0_DROP_LINE      (rVIN_F0_DROP_LINE<<26)
#define BIT_VIN_F1_DROP_LINE      (rVIN_F1_DROP_LINE<<25)
#define BIT_VIN_S_RATIO						(rVIN_S_RATIO<<23)
#define BIT_VIN_CLK_DIR        		(rVIN_CLK_DIR<<22)		
#define BIT_VIN_WIDTH							(rVIN_WIDTH<<20)
#define BIT_VIN_HEIGHT						(rVIN_HEIGHT<<8)
#define BIT_VIN_BUF_OVERFLOW			(0x1<<31)

// Video clock control bit definition
#define BIT_VIN_CLOCK_ENABLE			(rVIN_CLOCK_ENABLE<<31)	
#define BIT_VIN_PLL_SOURCE       	(rVIN_PLL_SOURCE<<30)		
#define BIT_VIN_DIVIDER_TYPE			(rVIN_DIVIDER_TYPE<<29)	
#define BIT_VIN_DIVIDER_VALUE			(rVIN_DIVIDER_VALUE<<0)	

/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/

#define EN_VIN_VIDEO()       	VIN_CTL  |=  BIT_VIN_ENA          						// Enable Video input interface.
#define DIS_VIN_VIDEO()      	VIN_CTL  &= ~BIT_VIN_ENA          						// Disable Video input interface.
#define INTR_VIN_DIS()       	VIN_INTR_STS |=  BIT_VIN_BUF_OVERFLOW     		// Disable the Video input interrupts


/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/

void HwCameraInitialize(VOID);
int HwADV7171Initialize(BOOL video_system);
VOID AppVinInitialize(VOID); 
VOID AppVinStart(VOID);
VOID AppVinStop(VOID);
VOID AppVinSizeUpdate(WORD VinSizeWidth, WORD VinSizeHeight, LONG skip, LONG scaling);
void  GetCamera_Sensor_Init(BYTE **cameraInit, int *N);
int IsVinEnabled();
extern BYTE camera_sensor_init_frame_rate[];
#endif

