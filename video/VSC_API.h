#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

//#include "includes.h"
#include 	"risc.h"
#include	"Mc.h"
#include	"Sc.h"
#include 	"comm.h"
#include	"2risc_memory_map.h"
#include	"def_type.h"

/*
*********************************************************************************************************
*              Definition for VSC memory address, To be change by USer
*********************************************************************************************************
*/ 
#ifdef VSC_API_GLOBALES
#define VSC_API_EXT
#else
#define VSC_API_EXT extern
#endif

#define  OSD_WIDTH  704
#define  OSD_HEIGHT 576
#define VIN2VSC

extern unsigned int vsc_y_0[640*480 * 3 / 4];    
   
VSC_API_EXT unsigned int OSD_memory_0[OSD_WIDTH*OSD_HEIGHT/8]; //add by zjb DEC 4th
VSC_API_EXT unsigned int OSD_memory_1[OSD_WIDTH*OSD_HEIGHT/8]; //add by zjb DEC 4th

#ifdef VIN2VSC
#define RVO_Y_memory_0	   	(((unsigned int)vsc_y_0) & 0x1fffffff)    // use pfysical address
#define RVO_U_memory_0    	(RVO_Y_memory_0 + 640 * 480 )
#define RVO_V_memory_0    	(RVO_U_memory_0 + 640 * 480 )
#define RVO_Y_memory_1		(RVO_V_memory_0 + 640 * 480 )			
#define RVO_U_memory_1	(RVO_Y_memory_1+ 640 * 480 )      
#define RVO_V_memory_1		(RVO_U_memory_1 + 640 * 480 )	      
#define PIP_Y_memory_0		VIN_Y_memory_0     	  	
#define PIP_U_memory_0		VIN_U_memory_0     	  
#define PIP_V_memory_0		VIN_V_memory_0     	  
#define PIP_Y_memory_1		VIN_Y_memory_1    	   
#define PIP_U_memory_1		VIN_U_memory_1     	  
#define PIP_V_memory_1		VIN_V_memory_1 

#else
#define RVO_Y_memory_0		0x498000			
#define RVO_U_memory_0	0x598000      
#define RVO_V_memory_0		0x698000      
#define RVO_Y_memory_1		0x4e4000      
#define RVO_U_memory_1  	0x5e4000      
#define RVO_V_memory_1		0x6e4000   
#define PIP_Y_memory_0		0x400000     	  	
#define PIP_U_memory_0		0x500000     	  
#define PIP_V_memory_0		0x600000     	  
#define PIP_Y_memory_1		0x44c000     	   
#define PIP_U_memory_1		0x54c000     	  
#define PIP_V_memory_1		0x64c000 

#endif

#define VDE_BASE_ADDR	BLOCK_BASE_ADDR(VSC_BLOCK) // not sure which block

#define YUV422			0
#define YUV420			1

#define horizontal_no_scale 	0
#define horizontal_times_2 	1
#define horizontal_times_4 	2
#define horizontal_div_2 		4
#define horizontal_div_4		5
#define horizontal_div_8		6

#define vertical_no_scale		0
#define vertical_times_2		1
#define vertical_times_4		2	
#define vertical_times_8		3	
#define vertical_div_2		4		
#define vertical_div_4		5
#define vertical_div_8		6

#define FONT_WIDTH   24
#define FONT_HEIGHT  32
#define OSD_OFFSET 16               //32pixels----16byte

/*
*********************************************************************************************************
*              	Definition for VSC Video height and width, to be change by User
*********************************************************************************************************
*/  

WORD VSC_WIDTH;
WORD VSC_HEIGHT;
WORD VOUT_WIDTH;
WORD VOUT_HEIGHT;
WORD RVO_ratio;
WORD PIP_ratio;
WORD RVO_PIXEL_START;
WORD RVO_LINE_START;
WORD RVO_PIXEL_STOP;
WORD RVO_LINE_STOP;
WORD PIP_PIXEL_START;
WORD PIP_LINE_START;
WORD PIP_PIXEL_STOP;
WORD PIP_LINE_STOP;
WORD OSD_PIXEL_START;
WORD OSD_LINE_START;
WORD OSD_PIXEL_STOP;
WORD OSD_LINE_STOP;

//VSC register
#define 	rVDE_FM_SIZE_WIDTH  	  	VSC_WIDTH			/* Video Width (must be multiple of 8) 0x280 */
#define	rVDE_FM_SIZE_HEIGHT  	  	VSC_HEIGHT			/* Video Height 0x1E0 */
#define  	rVDE_RVO_PIXEL_START 	  	RVO_PIXEL_START	/* RVO window starting pixel location on screen (must be multiple of 8) 	0x0 */
#define 	rVDE_RVO_LINE_START 	  	RVO_LINE_START 	/* RVO window starting line location on screen 0x0 */
#define 	rVDE_RVO_PIXEL_STOP 	  	RVO_PIXEL_STOP 	/* RVO window stopping pixel location on screen (must be multiple of 8) 	0x280	*/                                        
#define 	rVDE_RVO_LINE_STOP   	  	RVO_LINE_STOP 		/* RVO window stopping line location on screen 	0x1E0	*/                                           
#define 	rVDE_PIP_PIXEL_START	  	PIP_PIXEL_START	/* PIP window starting pixel location on screen (must be multiple of 8 0x0	*/
#define 	rVDE_PIP_LINE_START  	  	PIP_LINE_START		/* PIP window starting line location on screen 0x0 */
#define	rVDE_PIP_PIXEL_STOP	  	PIP_PIXEL_STOP 		/* PIP window stopping pixel location on screen (must be multiple of 8). 	0x140 */
#define	rVDE_PIP_LINE_STOP   	  	PIP_LINE_STOP 		/* PIP window stopping line location on screen, 	0xF0 */
#define 	rVDE_OSD_PIXEL_START 	  	OSD_PIXEL_START		/* OSD window starting pixel location on screen (must be multiple of 8) */
#define	rVDE_OSD_LINE_START 	  	OSD_LINE_START		/* OSD window starting line location on screen 	*/                         
#define	rVDE_OSD_PIXEL_STOP	  	OSD_PIXEL_STOP       /* OSD window stopping pixel location on screen (must be multiple of 8) */	
#define 	rVDE_OSD_LINE_STOP 	  	OSD_LINE_STOP		/* OSD window stopping line location on screen */                         
#define 	rVDE_S_RATIO_SC_ID	  	0			/* 0:V0=RVO,V1=PIP 1:V0=PIP, V1=RVO */
#define 	rVDE_V0_S_RATIO  	 	  	RVO_ratio	/* RVO scaling ratio 000:0, 001:2, 010:4, 100:1/2, 101:1/4, 110:1/8 */
#define 	rVDE_V1_S_RATIO    	 	 	PIP_ratio	/* PIP scaling ratio 000:0, 001:2, 010:4, 100:1/2, 101:1/4, 110:1/8 */
#define 	rVDE_S_RATIO_COEF0	  	2			/* Coefficient 0 0xA */
#define 	rVDE_S_RATIO_COEF1 	  	125			/* Coefficient 0 0x8A */
#define 	rVDE_S_RATIO_COEF2 	  	126			/* Coefficient 0 0x8A */
#define	rVDE_S_RATIO_COEF3  	  	2			/* Coefficient 0 0xA */
#define  	rVDE_S_RATIO_SIGN_C0	  	0			/* Coefficient 0, 0 positive, 1 negative */
#define 	rVDE_S_RATIO_SIGN_C1	  	0			/* Coefficient 1, 0 positive, 1 negative */
#define 	rVDE_S_RATIO_SIGN_C2	  	0			/* Coefficient 2, 0 positive, 1 negative */
#define 	rVDE_S_RATIO_SIGN_C3	 	0			/* Coefficient 3, 0 positive, 1 negative */
#define 	rVDE_FILL_COLOR_Y	  	 	0			/* Blank Pixel Fill-in Color Y_FILL */
#define 	rVDE_FILL_COLOR_U  	  	0			/* Blank Pixel Fill-in Color U_FILL */
#define	rVDE_FILL_COLOR_V   	  	0			/* Blank Pixel Fill-in Color V_FILL */
#define	rVDE_OSD_TRANS_MASK	  	6			/* OSD transparent mask value*/
#define 	rVDE_CT_ENA_V0    	  		0			/* stream 0 enhancement contrast enable */
#define	rVDE_CT_ENA_V1      	  		0			/* stream 1 enhancement contrast enable */
#define	rVDE_CT_ENA_16    			0			/* Enable offset 16 before/after contrast */
#define 	rVDE_CT_CTRL      			0			/* This value controls the contrast gain Gain =N/8 (N=0..15) */
#define 	rVDE_BT_ENA_V0 			0			/* stream 0 Brightness control, 0:disable, 1:enable */
#define 	rVDE_BT_ENA_V1   			0			/* stream 1 Brightness control, 0:disable, 1:enable */
#define 	rVDE_BT_ADD_SUB    			0			/* Brightness 0:add 1:subtract */
#define 	rVDE_BT_CTRL            		0x1F		/* Brightness offset value bit [28:24] */
#define 	rVDE_SH_ENA_V0          		0			/* V0 enhancement sharpness enalbe */
#define 	rVDE_SH_ENA_V1          		0			/* V1 enhancement sharpness enalbe */
#define	rVDE_SH_CTRL            		0			/* sharpness gain control */
#define  	rVDE_SH_THRES           		0			/* sharpness adjustment threshold */
#define 	rVDE_HUE_ENA_V0         		0			/* V0 video enhancement hue enable */
#define 	rVDE_HUE_ENA_V1         		0			/* V1 video enhancement hue enable */	
#define 	rVDE_HUE_CB_ADD_SUB  		0			/* CB 0:add, 1:subtract */
#define 	rVDE_CB_HUE             		0			/* CB hue gain */
#define 	rVDE_HUE_CR_ADD_SUB  		0			/* CR 0:add, 1:subtract */
#define	rVDE_CR_HUE             		0			/* CR hue gain */
#define	rVDE_SA_ENA_V0          		0			/* V0 video ehancement saturation enable */
#define 	rVDE_SA_ENA_V1          		0			/* V1 video ehancement saturation enable */
#define	rVDE_SA_CTRL            		0			/* saturation gain Gain = N/8 */
#define	rVDE_LBUF_U_BASE        		512			/* U component base address */
#define 	rVDE_LBUF_V_BASE        		1024		/* V component base address */	
#define 	rVDE_OSD_PT_CR_0x0 		128			/* OSD palette table 1st address entry */
#define 	rVDE_OSD_PT_CB_0x0      	128			/* OSD palette table 1st address entry */
#define 	rVDE_OSD_PT_Y_0x0     		0			/* OSD palette table 1st address entry */
#define 	rVDE_OSD_PT_CR_0x1 		192			/* OSD palette table 2st address entry */
#define 	rVDE_OSD_PT_CB_0x1    		106			/* OSD palette table 2st address entry */
#define 	rVDE_OSD_PT_Y_0x1     		38			/* OSD palette table 2st address entry */
#define 	rVDE_OSD_PT_CR_0x2 		74			/* OSD palette table 3rd address entry */
#define 	rVDE_OSD_PT_CB_0x2      	86            	/* OSD palette table 3rd address entry */
#define 	rVDE_OSD_PT_Y_0x2     		75            	/* OSD palette table 3rd address entry */
#define 	rVDE_OSD_PT_CR_0x3 		138			/* OSD palette table 4th address entry */
#define 	rVDE_OSD_PT_CB_0x3      	64           	/* OSD palette table 4th address entry */
#define 	rVDE_OSD_PT_Y_0x3     		113           	/* OSD palette table 4th address entry */
#define 	rVDE_OSD_PT_CR_0x4 		118			/* OSD palette table 5th address entry */
#define 	rVDE_OSD_PT_CB_0x4      	192           	/* OSD palette table 5th address entry */
#define 	rVDE_OSD_PT_Y_0x4     		15            	/* OSD palette table 5th address entry */
#define 	rVDE_OSD_PT_CR_0x5 		182			/* OSD palette table 6th address entry */
#define 	rVDE_OSD_PT_CB_0x5      	170          	/* OSD palette table 6th address entry */
#define 	rVDE_OSD_PT_Y_0x5     		53           	/* OSD palette table 6th address entry */
#define 	rVDE_OSD_PT_CR_0x6		64			/* OSD palette table 7th address entry */
#define 	rVDE_OSD_PT_CB_0x6      	150           	/* OSD palette table 7th address entry */
#define 	rVDE_OSD_PT_Y_0x6     		90            	/* OSD palette table 7th address entry */
#define 	rVDE_OSD_PT_CR_0x7 		128			/* OSD palette table 8th address entry */
#define 	rVDE_OSD_PT_CB_0x7      	128           	/* OSD palette table 8th address entry */
#define 	rVDE_OSD_PT_Y_0x7     		128           	/* OSD palette table 8th address entry */
#define 	rVDE_OSD_PT_CR_0x8 		128			/* OSD palette table 9th address entry */
#define 	rVDE_OSD_PT_CB_0x8      	128           	/* OSD palette table 9th address entry */
#define 	rVDE_OSD_PT_Y_0x8     		192           	/* OSD palette table 9th address entry */
#define 	rVDE_OSD_PT_CR_0x9 		255			/* OSD palette table 10th address entry */
#define 	rVDE_OSD_PT_CB_0x9      	85            	/* OSD palette table 10th address entry */
#define 	rVDE_OSD_PT_Y_0x9     		76            	/* OSD palette table 10th address entry */
#define 	rVDE_OSD_PT_CR_0xA 		21          		/* OSD palette table 11th address entry */	
#define 	rVDE_OSD_PT_CB_0xA      	43            	/* OSD palette table 11th address entry */
#define 	rVDE_OSD_PT_Y_0xA     		150           	/* OSD palette table 11th address entry */
#define 	rVDE_OSD_PT_CR_0xB 		149			/* OSD palette table 12th address entry */
#define 	rVDE_OSD_PT_CB_0xB      	0             	/* OSD palette table 12th address entry */
#define 	rVDE_OSD_PT_Y_0xB     		226           	/* OSD palette table 12th address entry */
#define 	rVDE_OSD_PT_CR_0xC 		107       		/* OSD palette table 13th address entry */
#define 	rVDE_OSD_PT_CB_0xC      	255           	/* OSD palette table 13th address entry */
#define 	rVDE_OSD_PT_Y_0xC     		29            	/* OSD palette table 13th address entry */
#define 	rVDE_OSD_PT_CR_0xD 		235			/* OSD palette table 14th address entry */
#define 	rVDE_OSD_PT_CB_0xD      	212           	/* OSD palette table 14th address entry */
#define 	rVDE_OSD_PT_Y_0xD     		105           	/* OSD palette table 14th address entry */
#define 	rVDE_OSD_PT_CR_0xE 		0			/* OSD palette table 15th address entry */
#define 	rVDE_OSD_PT_CB_0xE      	171          	/* OSD palette table 15th address entry */
#define 	rVDE_OSD_PT_Y_0xE     		179           	/* OSD palette table 15th address entry */
#define 	rVDE_OSD_PT_CR_0xF 		128			/* OSD palette table 16th address entry */
#define 	rVDE_OSD_PT_CB_0xF      	128           	/* OSD palette table 16th address entry */
#define 	rVDE_OSD_PT_Y_0xF     		255           	/* OSD palette table 16th address entry */

/*
*********************************************************************************************************
*                          Video register definition
*********************************************************************************************************
*/


#define VDE_CTL				(*(volatile unsigned *)(VDE_BASE_ADDR + 0x0))		/* VDE control register */
#define VDE_FM_SIZE			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x4))		/* controls the video full screen resolution */
#define VDE_RVO_START		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x8))             /* RVO starting point on screen */    		
#define VDE_RVO_STOP		(*(volatile unsigned *)(VDE_BASE_ADDR + 0xC))		/* RVO stopping point on screen */ 
#define VDE_PIP_START		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x10))		/* PIP starting point on screen */	
#define VDE_PIP_STOP		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x14))           /* PIP stopping point on screen */
#define VDE_OSD_START		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x18))		/* OSD starting point on screen */
#define VDE_OSD_STOP		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x1C))           /* OSD stopping point on screen */
#define VDE_S_RATIO			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x20))		/* RVO/PIP scaling ratio */
#define VDE_S_RATIO_COEF	(*(volatile unsigned *)(VDE_BASE_ADDR + 0x24))		/* V0 scaling ratio */
#define VDE_S_RATIO_SIGN	(*(volatile unsigned *)(VDE_BASE_ADDR + 0x28))		/* S_RATION_SIGN */
#define VDE_FILL_COLOR		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x2C))		/* fill-in color for blank pixel */
#define VDE_OSD_T_MASK		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x30))		/* OSD transparent mask pixel */
#define  EN_TRANS_MASK(x)		VDE_OSD_T_MASK=(VDE_OSD_T_MASK&0)|x<<28            //add by zjb Dec 4nd
  #define VDE_CT_CTRL			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x34))		/* contrast adjustment range */
#define VDE_BT_CTRL			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x38))		/* brightness adjustment range */
#define VDE_SH_CTRL			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x3C))		/* sharpness adjustment range */
#define VDE_HUE_CTRL		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x40))		/* hue adjustment range */
#define VDE_SA_CTRL			(*(volatile unsigned *)(VDE_BASE_ADDR + 0x44))		/* saturation adjustment range */
#define VDE_LBUF_BASE		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x48))		/* scaling line buffer U/V base address. Y base always 0 */
#define VDE_INTR_STS		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x4C))		/* Interrupt status, write 1 to the bit will clear it */
#define VDE_OSD_PT_0x0		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 0*0x04))		/* This is the address to access OSD palette table through CBUS 1st address entry */
#define VDE_OSD_PT_0x1		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 1*0x04))		/* This is the address to access OSD palette table through CBUS 2nd address entry */
#define VDE_OSD_PT_0x2		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 2*0x04))		/* This is the address to access OSD palette table through CBUS 3rd address entry */
#define VDE_OSD_PT_0x3		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 3*0x04))		/* This is the address to access OSD palette table through CBUS 4th address entry */
#define VDE_OSD_PT_0x4		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 4*0x04))		/* This is the address to access OSD palette table through CBUS 5th address entry */
#define VDE_OSD_PT_0x5		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 5*0x04))		/* This is the address to access OSD palette table through CBUS 6th address entry */
#define VDE_OSD_PT_0x6		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 6*0x04))		/* This is the address to access OSD palette table through CBUS 7th address entry */
#define VDE_OSD_PT_0x7		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 7*0x04))		/* This is the address to access OSD palette table through CBUS 8th address entry */
#define VDE_OSD_PT_0x8		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 8*0x04))		/* This is the address to access OSD palette table through CBUS 9th address entry */
#define VDE_OSD_PT_0x9		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 9*0x04))		/* This is the address to access OSD palette table through CBUS 10th address entry */
#define VDE_OSD_PT_0xA		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 10*0x04))      	/* This is the address to access OSD palette table through CBUS 11th address entry */    			
#define VDE_OSD_PT_0xB		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 11*0x04))		/* This is the address to access OSD palette table through CBUS 12th address entry */
#define VDE_OSD_PT_0xC		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 12*0x04))		/* This is the address to access OSD palette table through CBUS 13th address entry */
#define VDE_OSD_PT_0xD		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 13*0x04))      	/* This is the address to access OSD palette table through CBUS 14th address entry */		
#define VDE_OSD_PT_0xE		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 14*0x04))		/* This is the address to access OSD palette table through CBUS 15th address entry */
#define VDE_OSD_PT_0xF		(*(volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 15*0x04))		/* This is the address to access OSD palette table through CBUS 16th address entry */

#define VDE_VIN_LJMP			(*(volatile unsigned *)(0xa8023020))      
#define VDE_VOUT_LJMP			(*(volatile unsigned *)(0xa8023024))        
#define VDE_VSC_READ_V0_LJMP	(*(volatile unsigned *)(0xa8023028))        
#define VDE_VSC_READ_V1_LJMP	(*(volatile unsigned *)(0xa802302c))        
#define VDE_OSD_LJMP			(*(volatile unsigned *)(0xa8023030))        
#define VDE_VSC_WRITE_LJMP		(*(volatile unsigned *)(0xa8023034))        


// Video Display Engive VDE control register definition
#define BIT_VDE_ENA						(0x1<<31)
#define BIT_VDE_RVO_ENA				(0x1<<30)
#define BIT_VDE_PIP_ENA					(0x1<<29)
#define BIT_VDE_OSD_ENA				(0x1<<28)
#define BIT_VDE_OSD_TRANS_ENA			(0x1<<27)
#define BIT_VDE_FM_SIZE_WIDTH			(rVDE_FM_SIZE_WIDTH<<20)
#define BIT_VDE_FM_SIZE_HEIGHT		(rVDE_FM_SIZE_HEIGHT<<8)
#define BIT_VDE_RVO_PIXEL_START		(rVDE_RVO_PIXEL_START<<20)
#define BIT_VDE_RVO_LINE_START			(rVDE_RVO_LINE_START<<8)
#define BIT_VDE_RVO_PIXEL_STOP			(rVDE_RVO_PIXEL_STOP<<20)
#define BIT_VDE_RVO_LINE_STOP			(rVDE_RVO_LINE_STOP<<8)
#define BIT_VDE_PIP_PIXEL_START		(rVDE_PIP_PIXEL_START<<20)
#define BIT_VDE_PIP_LINE_START			(rVDE_PIP_LINE_START<<8)
#define BIT_VDE_PIP_PIXEL_STOP			(rVDE_PIP_PIXEL_STOP<<20)
#define BIT_VDE_PIP_LINE_STOP			(rVDE_PIP_LINE_STOP<<8)
#define BIT_VDE_OSD_PIXEL_START		(rVDE_OSD_PIXEL_START<<20)
#define BIT_VDE_OSD_LINE_START		(rVDE_OSD_LINE_START<<8)
#define BIT_VDE_OSD_PIXEL_STOP			(rVDE_OSD_PIXEL_STOP<<20)
#define BIT_VDE_OSD_LINE_STOP			(rVDE_OSD_LINE_STOP<<8)
#define BIT_VDE_S_RATIO_SC_ID			(rVDE_S_RATIO_SC_ID<<31)
#define BIT_VDE_V0_S_RATIO				(rVDE_V0_S_RATIO<<28)
#define BIT_VDE_V1_S_RATIO				(rVDE_V1_S_RATIO<<25)
#define BIT_VDE_S_RATIO_COEF0			(rVDE_S_RATIO_COEF0<<24)
#define BIT_VDE_S_RATIO_COEF1			(rVDE_S_RATIO_COEF1<<16)
#define BIT_VDE_S_RATIO_COEF2			(rVDE_S_RATIO_COEF2<<8)
#define BIT_VDE_S_RATIO_COEF3			(rVDE_S_RATIO_COEF3<<0)
#define BIT_VDE_S_RATIO_SIGN_C0		(rVDE_S_RATIO_SIGN_C0<<31)
#define BIT_VDE_S_RATIO_SIGN_C1		(rVDE_S_RATIO_SIGN_C1<<30)
#define BIT_VDE_S_RATIO_SIGN_C2		(rVDE_S_RATIO_SIGN_C2<<29)
#define BIT_VDE_S_RATIO_SIGN_C3		(rVDE_S_RATIO_SIGN_C3<<28)
#define BIT_VDE_FILL_COLOR_Y			(rVDE_FILL_COLOR_Y<<24)
#define BIT_VDE_FILL_COLOR_U			(rVDE_FILL_COLOR_U<<16)
#define BIT_VDE_FILL_COLOR_V			(rVDE_FILL_COLOR_V<<8)
#define BIT_VDE_OSD_TRANS_MASK		(rVDE_OSD_TRANS_MASK<<28)
#define BIT_VDE_CT_ENA_V0				(rVDE_CT_ENA_V0<<31)
#define BIT_VDE_CT_ENA_V1				(rVDE_CT_ENA_V1<<30)
#define BIT_VDE_CT_ENA_16				(rVDE_CT_ENA_16<<29)
#define BIT_VDE_CT_CTRL					(rVDE_CT_CTRL<<25)
#define BIT_VDE_BT_ENA_V0				(rVDE_BT_ENA_V0<<31)
#define BIT_VDE_BT_ENA_V1				(rVDE_BT_ENA_V1<<30)
#define BIT_VDE_BT_ADD_SUB				(rVDE_BT_ADD_SUB<<29)
#define BIT_VDE_BT_CTRL					(rVDE_BT_CTRL<<24)
#define BIT_VDE_SH_ENA_V0				(rVDE_SH_ENA_V0<<31)
#define BIT_VDE_SH_ENA_V1				(rVDE_SH_ENA_V1<<30)
#define BIT_VDE_SH_CTRL					(rVDE_SH_CTRL<<27)
#define BIT_VDE_SH_THRES				(rVDE_SH_THRES<<19)
#define BIT_VDE_HUE_ENA_V0				(rVDE_HUE_ENA_V0<<31)
#define BIT_VDE_HUE_ENA_V1				(rVDE_HUE_ENA_V1<<30)
#define BIT_VDE_HUE_CB_ADD_SUB		(rVDE_HUE_CB_ADD_SUB<<29)
#define BIT_VDE_CB_HUE					(rVDE_CB_HUE<<25)
#define BIT_VDE_HUE_CR_ADD_SUB		(rVDE_HUE_CR_ADD_SUB<<24)
#define BIT_VDE_CR_HUE					(rVDE_CR_HUE<<20)
#define BIT_VDE_SA_ENA_V0				(rVDE_SA_ENA_V0<<31)
#define BIT_VDE_SA_ENA_V1				(rVDE_SA_ENA_V1<<30)
#define BIT_VDE_SA_CTRL					(rVDE_SA_CTRL<<26)
#define BIT_VDE_LBUF_U_BASE			(rVDE_LBUF_U_BASE<<20)
#define BIT_VDE_LBUF_V_BASE			(rVDE_LBUF_V_BASE<<8)
#define BIT_VDE_FRM_DONE				(0x1<<31)
#define BIT_VDE_OSD_PT_CR_0x0			(rVDE_OSD_PT_CR_0x0<<16)
#define BIT_VDE_OSD_PT_CB_0x0			(rVDE_OSD_PT_CB_0x0<<8)
#define BIT_VDE_OSD_PT_Y_0x0			(rVDE_OSD_PT_Y_0x0<<0)
#define BIT_VDE_OSD_PT_CR_0x1			(rVDE_OSD_PT_CR_0x1<<16)
#define BIT_VDE_OSD_PT_CB_0x1			(rVDE_OSD_PT_CB_0x1<<8)
#define BIT_VDE_OSD_PT_Y_0x1			(rVDE_OSD_PT_Y_0x1<<0)
#define BIT_VDE_OSD_PT_CR_0x2			(rVDE_OSD_PT_CR_0x2<<16)
#define BIT_VDE_OSD_PT_CB_0x2			(rVDE_OSD_PT_CB_0x2<<8)
#define BIT_VDE_OSD_PT_Y_0x2			(rVDE_OSD_PT_Y_0x2<<0)
#define BIT_VDE_OSD_PT_CR_0x3			(rVDE_OSD_PT_CR_0x3<<16)
#define BIT_VDE_OSD_PT_CB_0x3			(rVDE_OSD_PT_CB_0x3<<8)
#define BIT_VDE_OSD_PT_Y_0x3			(rVDE_OSD_PT_Y_0x3<<0)
#define BIT_VDE_OSD_PT_CR_0x4			(rVDE_OSD_PT_CR_0x4<<16)
#define BIT_VDE_OSD_PT_CB_0x4			(rVDE_OSD_PT_CB_0x4<<8)
#define BIT_VDE_OSD_PT_Y_0x4			(rVDE_OSD_PT_Y_0x4<<0)
#define BIT_VDE_OSD_PT_CR_0x5			(rVDE_OSD_PT_CR_0x5<<16)
#define BIT_VDE_OSD_PT_CB_0x5			(rVDE_OSD_PT_CB_0x5<<8)
#define BIT_VDE_OSD_PT_Y_0x5			(rVDE_OSD_PT_Y_0x5<<0)
#define BIT_VDE_OSD_PT_CR_0x6			(rVDE_OSD_PT_CR_0x6<<16)
#define BIT_VDE_OSD_PT_CB_0x6			(rVDE_OSD_PT_CB_0x6<<8)
#define BIT_VDE_OSD_PT_Y_0x6			(rVDE_OSD_PT_Y_0x6<<0)
#define BIT_VDE_OSD_PT_CR_0x7			(rVDE_OSD_PT_CR_0x7<<16)
#define BIT_VDE_OSD_PT_CB_0x7			(rVDE_OSD_PT_CB_0x7<<8)
#define BIT_VDE_OSD_PT_Y_0x7			(rVDE_OSD_PT_Y_0x7<<0)
#define BIT_VDE_OSD_PT_CR_0x8			(rVDE_OSD_PT_CR_0x8<<16)
#define BIT_VDE_OSD_PT_CB_0x8			(rVDE_OSD_PT_CB_0x8<<8)
#define BIT_VDE_OSD_PT_Y_0x8			(rVDE_OSD_PT_Y_0x8<<0)	
#define BIT_VDE_OSD_PT_CR_0x9			(rVDE_OSD_PT_CR_0x9<<16)
#define BIT_VDE_OSD_PT_CB_0x9			(rVDE_OSD_PT_CB_0x9<<8)
#define BIT_VDE_OSD_PT_Y_0x9			(rVDE_OSD_PT_Y_0x9<<0)
#define BIT_VDE_OSD_PT_CR_0xA			(rVDE_OSD_PT_CR_0xA<<16)
#define BIT_VDE_OSD_PT_CB_0xA			(rVDE_OSD_PT_CB_0xA<<8)
#define BIT_VDE_OSD_PT_Y_0xA			(rVDE_OSD_PT_Y_0xA<<0)
#define BIT_VDE_OSD_PT_CR_0xB			(rVDE_OSD_PT_CR_0xB<<16)
#define BIT_VDE_OSD_PT_CB_0xB			(rVDE_OSD_PT_CB_0xB<<8)
#define BIT_VDE_OSD_PT_Y_0xB			(rVDE_OSD_PT_Y_0xB<<0)
#define BIT_VDE_OSD_PT_CR_0xC			(rVDE_OSD_PT_CR_0xC<<16)
#define BIT_VDE_OSD_PT_CB_0xC			(rVDE_OSD_PT_CB_0xC<<8)
#define BIT_VDE_OSD_PT_Y_0xC			(rVDE_OSD_PT_Y_0xC<<0)
#define BIT_VDE_OSD_PT_CR_0xD			(rVDE_OSD_PT_CR_0xD<<16)
#define BIT_VDE_OSD_PT_CB_0xD			(rVDE_OSD_PT_CB_0xD<<8)
#define BIT_VDE_OSD_PT_Y_0xD			(rVDE_OSD_PT_Y_0xD<<0)
#define BIT_VDE_OSD_PT_CR_0xE			(rVDE_OSD_PT_CR_0xE<<16)
#define BIT_VDE_OSD_PT_CB_0xE			(rVDE_OSD_PT_CB_0xE<<8)
#define BIT_VDE_OSD_PT_Y_0xE			(rVDE_OSD_PT_Y_0xE<<0)
#define BIT_VDE_OSD_PT_CR_0xF			(rVDE_OSD_PT_CR_0xF<<16)
#define BIT_VDE_OSD_PT_CB_0xF			(rVDE_OSD_PT_CB_0xF<<8)
#define BIT_VDE_OSD_PT_Y_0xF			(rVDE_OSD_PT_Y_0xF<<0)

/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/

#define EN_VDE()					VDE_CTL |= BIT_VDE_ENA					// Enable VSC
#define EN_VDE_RVO()			VDE_CTL |= BIT_VDE_RVO_ENA				// Enable RVO	
#define EN_VDE_PIP()				VDE_CTL |= BIT_VDE_PIP_ENA				// Enable PIP
#define EN_VDE_OSD()			VDE_CTL |= BIT_VDE_OSD_ENA				// Enable OSD
#define EN_VDE_OSD_TRANS()		VDE_CTL |= BIT_VDE_OSD_TRANS_ENA		// Enable OSD transparency

#define  EN_VDE_TRANS_MASK(x)		VDE_OSD_T_MASK=(VDE_OSD_T_MASK&0x0<<28)|x<<28  //add by zjb Nov30th

#define DIS_VDE()				VDE_CTL &= ~BIT_VDE_ENA				// disable VSC             
#define DIS_VDE_RVO()			VDE_CTL &= ~BIT_VDE_RVO_ENA                  	// disable RVO	            
#define DIS_VDE_PIP()			VDE_CTL &= ~BIT_VDE_PIP_ENA                   	// disable PIP             
#define DIS_VDE_OSD()			VDE_CTL &= ~BIT_VDE_OSD_ENA                   	// disable OSD             
#define DIS_VDE_OSD_TRANS()	VDE_CTL &= ~BIT_VDE_OSD_TRANS_ENA	       // disable OSD transparency
#define INTR_VSC_DIS()			VDE_INTR_STS |= BIT_VDE_FRM_DONE		// clear VDE frame done flag

#define VB2100_MASTER  0
#define VB2100_PATH 0

typedef  struct {
    unsigned short width;
    unsigned short height;
    unsigned short start_pixel;  
    unsigned short start_line;   
    unsigned int index_addr;
    unsigned int bmp_addr;
    unsigned int attribute;
} gtBMPSetting;


typedef struct{
    unsigned short pixel_start;
    unsigned short line_start;
    unsigned short width;
    unsigned short height;
    unsigned int attribute;    
}gtClearScreenSetting;



typedef struct{
    unsigned short start_x_pos;
    unsigned short start_y_pos;
    unsigned short end_x_pos;
    unsigned short end_y_pos;
    unsigned int attribute;    
}gtDrawLineSetting;


typedef struct{
    unsigned short pixel_start;
    unsigned short line_start;
    unsigned short width;
    unsigned short height;
    unsigned int attribute;    
}gtFillColorSetting;


typedef  struct {
    unsigned int *string_lib_addr;   
    unsigned int string_addr;   
    unsigned int string_len;
    unsigned int start_x;  
    unsigned int start_y;
    unsigned int attribute;      
} gtWriteStringSetting;




VSC_API_EXT  gtBMPSetting gtBmpParameters;

/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/

VOID AppVscInitialize(WORD VscSizeWidth, WORD VscSizeHeight, LONG Vsc_Y_skip, LONG Vsc_UV_skip,LONG Vsc_Y_scaling,LONG Vsc_UV_scaling);	
VOID AppRvoInitialize(WORD RVOSizeWidth, WORD RVOSizeHeight, WORD RVOInputFmt, WORD interlaced, WORD RVO_horizontal_ratio, WORD RVO_vertical_ratio);
VOID AppPipInitialize(WORD PIPSizeWidth, WORD PIPSizeHeight, WORD PIPInputFmt, WORD interlaced, WORD PIP_horizontal_ratio, WORD PIP_vertical_ratio);
VOID AppVscStop(VOID);
VOID AppVscSizeUpdate(WORD VscSizeWidth, WORD VscSizeHeight, LONG Vsc_skip, LONG Vsc_scaling);
VOID AppRvoSizeUpdate(WORD RVOSizeWidth, WORD RVOSizeHeight, LONG RVO_skip, LONG RVO_scaling, WORD RVO_x_start, WORD RVO_y_start, WORD RVO_x_stop, WORD RVO_y_stop);
VOID AppPipSizeUpdate(WORD PIPSizeWidth, WORD PIPSizeHeight, LONG PIP_skip, LONG PIP_scaling, WORD PIP_x_start, WORD PIP_y_start, WORD PIP_x_stop, WORD PIP_y_stop);
VOID AppDisplayColorBlock(SHORT x, SHORT y, SHORT size_x, SHORT size_y, BYTE color);   
VOID AppOSDInitialize(WORD OSDSizeWidth, WORD OSDSizeHeight,  WORD OSD_x_start, WORD OSD_y_stop,unsigned char backgroundColor,unsigned char  OSD_T_Mask,unsigned int addr);
VOID  InitOsdMemory(WORD OSDStartPixel, WORD OSDStartLine,WORD OSDSizeWidth, WORD OSDSizeHeight,unsigned char Fill_color,unsigned int pic_addr,unsigned char  OSD_T_Mask);//fill the OSD with given color
VOID Vb2100FillOsdPT(unsigned char dev, unsigned char path, unsigned int *index_addr);
VOID Vb2100FillBmp(unsigned char dev, unsigned char path, unsigned int bmp_start_pixel,unsigned int  bmp_start_line,unsigned int *index_addr, unsigned int *bmp_addr, unsigned int attribute);
VOID Vb2100ClearScreen(unsigned char dev, unsigned char path, gtClearScreenSetting tClearScreenParameters);
VOID Vb2100FillColor(unsigned char dev, unsigned char path, gtFillColorSetting tFillColorParameters);
VOID Vb2100DrawLine(unsigned char dev, unsigned char path, gtDrawLineSetting tDrawLineParameters);
VOID Vb2100ChangeOSDPage(unsigned char dev, unsigned char path, unsigned short  page);
VOID Vb2100WriteString(unsigned char dev, unsigned char path, gtWriteStringSetting tWriteStringParameters);
VOID  PixelShow(WORD x,WORD y,WORD Pixel_start,WORD pixel_stop,unsigned char  OSD_T_Mask,unsigned int  pic_addr,unsigned char  color);
VOID ShowPic(unsigned int pic_addr );
VOID AppVscStart(VOID);

#endif
