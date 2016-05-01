//#include "includes.h"
#include	"risc.h"
#include	"Mc.h"
#include 	"Sc.h"
#include 	"comm.h"
#include	"2risc_memory_map.h"
#include	"def_type.h"

#include  "video_cfg.h"  

//#define VIN2VOUT

#ifdef VIN2VOUT
#define VOUT_Y_memory_0	0x700000
#define VOUT_U_memory_0 	0x800000
#define VOUT_V_memory_0	0x900000
#define VOUT_Y_memory_1	0x780000
#define VOUT_U_memory_1 	0x880000
#define VOUT_V_memory_1	0x980000
#endif

WORD VOUT_WIDTH;
WORD VOUT_HEIGHT;

//VOUT clock control register
#define 	rVOUT_CLOCK_ENABLE	1		/* 1: Enable VOUT clock generator, 0:Disable VOUT clock generator	*/

#ifdef FreqFromCrystal
#define 	rVOUT_PLL_SOURCE		0		/* 0: use RISC PLL, 1: use SDRAM PLL*/
#else
#define 	rVOUT_PLL_SOURCE		1
#endif

//#define 	rVOUT_DIVIDER_TYPE	0		/* 1: Use accurate divider, 0: Use regular divider	*/
#define 	rVOUT_DIVIDER_TYPE	1		/* 1: Use accurate divider, 0: Use regular divider	*/

#ifdef FreqFromCrystal
#define 	rVOUT_DIVIDER_VALUE	0x6		/* Bit[28:0] The divider value adds to the divider counter. Clock ouptputs from bit[29] of the counter, 0x06 1.75MHz */
#else
//#define 	rVOUT_DIVIDER_VALUE	0x3		/* Bit[28:0] The divider value adds to the divider counter. Clock ouptputs from bit[29] of the counter, 0x06 1.75MHz */
#define 	rVOUT_DIVIDER_VALUE	0x67AE147		/* Bit[28:0] The divider value adds to the divider counter. Clock ouptputs from bit[29] of the counter, 0x06 1.75MHz */
#endif

#define 	rVOUT_CLK_POLORITY	0		/* VOUT_CTL bit[30], 0: posedge 1: negedge */
#define	rVOUT_FRAME_PIC		0		/* VOUT_CTL bit[29], 0: interlaced pictue 1: progressive picture */
#define  	rVOUT_FIRST_FIELD_ID  	0     		/* VOUT_CTL bit[28], 0: First field in the frame when interlaced picture  */
#define 	rVOUT_CLIP_ENABLE		1		/* VOUT_CTL bit[27], 0: NO CLIP, 1: CLIP Y:16-235; C: 16-240 */


#ifdef FreqFromCrystal
#define 	rVOUT_CLK_DIR			1		/* VOUT_CTL bit[26], 0: vout_clk is output, 1: vout_clk is input	*/
#else
#define 	rVOUT_CLK_DIR			0		/* VOUT_CTL bit[26], 0: vout_clk is output, 1: vout_clk is input	*/
#endif

#define 	rVOUT_L_BLK			0x6A				/* VOUT_LFMT bit[31:20], Number of blanking pixels at beiginning of every line	divide by (N/2-1) has to be *8 0x1F*/
#define 	rVOUT_L_ACT			(VOUT_WIDTH/2-1)	/* VOUT_LFMT bit[19:8], Active pixel per line	divide by (640/2-1) 0x13F*/
#define 	rVOUT_VFMT0_TOP_BLK	(10 - 1)				/* VOUT_VFMT0 bit[31:22], Number of blanking line before active line (N-1)	0x16*/
#define	rVOUT_VFMT0_ACT		(VOUT_HEIGHT/2-1)	/* VOUT_VFMT0 bit[21:12], Number of active line per frame (N-1)	*/
#define	rVOUT_VFMT0_BOT_BLK	(12 - 1)				/* VOUT_VFMT0 bit[9:0],Number of blanking line after active line (N-1)	0x17*/
#define 	rVOUT_VFMT1_TOP_BLK	(10 - 1)				/* VOUT_VFMT1 bit[31:22], Number of blanking line before active line (N-1)	0x16*/
#define	rVOUT_VFMT1_ACT		(VOUT_HEIGHT/2-1)	/* VOUT_VFMT1 bit[21:12], Number of active line per frame (N-1)	0x1DF	*/
#define	rVOUT_VFMT1_BOT_BLK	(13 - 1)				/* VOUT_VFMT1 bit[9:0],Number of blanking line after active line (N-1)	0x17*/
#define 	rVOUT_EOF_ERR			0					/* INTR_STS bit[30], End of field status mismatch with DBUS	*/

#define VID_OUT_BASE_ADDR    		BLOCK_BASE_ADDR(VOUT_BLOCK) 
#define VID_CLOCK_CTL_BASE_ADDR   	BLOCK_BASE_ADDR(CCTL_BLOCK)
#define VID_SC_CTL_BASE_ADDR		BLOCK_BASE_ADDR(SC_BLOCK)

/*
*********************************************************************************************************
*                          Video register definition
*********************************************************************************************************
*/
#define VOUT_CLOCK_CTL  		(*(volatile unsigned *)(VID_CLOCK_CTL_BASE_ADDR + 0x28))	/* Video output clock control */ 
#define VOUT_CTL      				(*(volatile unsigned *)(VID_OUT_BASE_ADDR + 0x0))    			/* Output Video control register */
#define VOUT_LFMT    				(*(volatile unsigned *)(VID_OUT_BASE_ADDR + 0x4))    			/* Video output line format */
#define VOUT_VFMT0				(*(volatile unsigned *)(VID_OUT_BASE_ADDR + 0x8))    			/* Video output field 0 format */
#define VOUT_VFMT1				(*(volatile unsigned *)(VID_OUT_BASE_ADDR + 0xC))    			/* Video output field 1 format */
#define VOUT_INTR_STS			(*(volatile unsigned *)(VID_OUT_BASE_ADDR + 0x10))   			/* Video output interrupt status */
#define VOUT_VSC_SECRET_BIT	(*(volatile unsigned *)(VID_SC_CTL_BASE_ADDR + 0x0)) 		//0xa8040000

// Video output control register VOUT_CTL bit definition
#define BIT_VOUT_ENA				(0x1<<31)
#define BIT_VOUT_CLK_POLORITY		(rVOUT_CLK_POLORITY<<30)
#define BIT_VOUT_FRAME_PIC       		(rVOUT_FRAME_PIC<<29)
#define BIT_VOUT_FIRST_FIELD_ID	(rVOUT_FIRST_FIELD_ID<<28)
#define BIT_VOUT_CLIP_ENABLE		(rVOUT_CLIP_ENABLE<<27)
#define BIT_VOUT_CLK_DIR        		(rVOUT_CLK_DIR<<26)
#define BIT_VOUT_L_BLK				(rVOUT_L_BLK<<20)
#define BIT_VOUT_L_ACT				(rVOUT_L_ACT<<8)
#define BIT_VOUT_VFMT0_TOP_BLK	(rVOUT_VFMT0_TOP_BLK<<22)
#define BIT_VOUT_VFMT0_ACT			(rVOUT_VFMT0_ACT<<10)
#define BIT_VOUT_VFMT0_BOT_BLK	(rVOUT_VFMT0_BOT_BLK<<0)
#define BIT_VOUT_VFMT1_TOP_BLK	(rVOUT_VFMT1_TOP_BLK<<22)
#define BIT_VOUT_VFMT1_ACT			(rVOUT_VFMT1_ACT<<10)
#define BIT_VOUT_VFMT1_BOT_BLK	(rVOUT_VFMT1_BOT_BLK<<0)
#define BIT_VOUT_BUF_UNDERRUN		(0x1<<31)
#define BIT_VOUT_EOF_BUF			(rVOUT_EOF_ERR<<30)
#define BIT_VOUT_CLOCK_ENABLE		(rVOUT_CLOCK_ENABLE<<31)				
#define BIT_VOUT_PLL_SOURCE       	(rVOUT_PLL_SOURCE<<30)					
#define BIT_VOUT_DIVIDER_TYPE		(rVOUT_DIVIDER_TYPE<<29)				
#define BIT_VOUT_DIVIDER_VALUE		(rVOUT_DIVIDER_VALUE<<0)	

/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/
#define EN_VOUT_VIDEO()      	VOUT_CTL  |=  BIT_VOUT_ENA          					// Enable Video output interface.`
#define DIS_VOUT_VIDEO()     	VOUT_CTL  &= ~BIT_VOUT_ENA          				// Disable Video output interface.
#define INTR_VOUT_DIS()       VOUT_INTR_STS |=  BIT_VOUT_BUF_UNDERRUN    		//  Disable the Video output interrupts

/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/
VOID AppVoutInitialize(WORD VoutSizeWidth, WORD VoutSizeHeight, LONG skip, LONG scaling);
VOID AppVoutStart(VOID);
VOID AppVoutStop (VOID);
VOID AppVoutSizeUpdate(WORD VoutSizeWidth, WORD VoutSizeHeight, LONG skip, LONG scaling);
















































