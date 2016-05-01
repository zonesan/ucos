/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Feb 9, 2006                                                           
***************************************************************************
*/



#include "cctl.h"


#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"
#include "chip_cfg.h"

#ifdef VIDEO_MODULE_INCLUDE
#include "video_cfg.h"
#include "../video/VIN_API.h"
#include "../../video/VOUT_API.h"
#endif

/*
***************************************************************************
*                           GLOBAL FUNCTIONS
***************************************************************************
*/

#ifdef VIDEO_MODULE_INCLUDE
/*
***************************************************************************
*                              CctlVoutClkSet
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
extern  unsigned int gFreqFromCrystal;
unsigned int gVoutClkSet = FALSE;
int CctlVoutClkSet(void)
{
    //static int vout_clock_set = 0;
    unsigned int cpu_sr;

    OS_ENTER_CRITICAL();

    /* Comment it for CctlVoutClkSet will be called if gFreqFromCrystal changed!
    if(vout_clock_set) {
        OS_EXIT_CRITICAL();
        return FAILED;
    }  

    vout_clock_set = 1;
    */
    if(gVoutClkSet == TRUE) {
        OS_EXIT_CRITICAL();
        return FAILED;
    }
    if(gFreqFromCrystal == FALSE) {
        VOUT_CTL = VOUT_CTL & 0xfbffffff;   // clear bit26
        //VOUT_CTL = BIT_VOUT_CLK_POLORITY | BIT_VOUT_FRAME_PIC | BIT_VOUT_FIRST_FIELD_ID | BIT_VOUT_CLIP_ENABLE | (0<<26);   

    //       VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | (1<<30) | (0<<29)/*BIT_VOUT_DIVIDER_TYPE */| (0x3<<0);      //just for tvp5150a test
        VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | (1<<30) | BIT_VOUT_DIVIDER_TYPE | (0x67AE147<<0);      //the internal clock for old UT board
    //    VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | (1<<30) | BIT_VOUT_DIVIDER_TYPE | (0x645a1ca<<0);            //the internal clock for VB2100 with OV7720 camera
    } else {
        VOUT_CTL = VOUT_CTL | (1<<26);               
        //VOUT_CTL = BIT_VOUT_CLK_POLORITY | BIT_VOUT_FRAME_PIC | BIT_VOUT_FIRST_FIELD_ID | BIT_VOUT_CLIP_ENABLE | (1<<26);   
        VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | (0<<30) | BIT_VOUT_DIVIDER_TYPE | (0x6<<0);      
    }
    gVoutClkSet = TRUE;
#if 0
    VOUT_CTL = BIT_VOUT_CLK_POLORITY | BIT_VOUT_FRAME_PIC | BIT_VOUT_FIRST_FIELD_ID | BIT_VOUT_CLIP_ENABLE | BIT_VOUT_CLK_DIR;   
    VOUT_CLOCK_CTL = BIT_VOUT_CLOCK_ENABLE | BIT_VOUT_PLL_SOURCE | BIT_VOUT_DIVIDER_TYPE | BIT_VOUT_DIVIDER_VALUE;      
#endif

    OS_EXIT_CRITICAL();

    return SUCCESSFUL;
}

/*
***************************************************************************
*                              CctlVinClkSet
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : This function should be called after VIN_CTL is set. Do not
*              need worry about this because CctlVinClkSet is called in 
*              Video_VIN_RegInit(Video_VIN_RegInit will set VIN_CTL at first).
***************************************************************************
*/
int CctlVinClkSet(void)
{
    if(VIN_CTL & 0x00400000)    // Vin clock is input?
        return SUCCESSFUL;      // rCCTL_VIN_CLOCK_DIVIDE need not be set
        
    /* enable clock, select risc pll, use regular devider */    
    rCCTL_VIN_CLOCK_DIVIDE = CCTL_VIN_CLOCK_ENABLE | CCTL_VIN_PLL_SOURCE_RISC |
                             CCTL_VIN_DIVIDER_TYPE_REGULAR | CCTL_VIN_DIVIDER_VALUE;  
                          
    return SUCCESSFUL;   
}

// Used when changing vin resolution
// Added by hjm
void CctlVinReset()
{
    int i;

    rCCTL_BLOCK_RESET |= CCTL_VIN_RESET_BIT;
    //for(i = 0; i < 1000000; i++);
	
    rCCTL_BLOCK_RESET &= ~(CCTL_VIN_RESET_BIT);
    //for(i = 0; i < 1000000; i++);	
}

void CctlVencReset()
{
    int i;

    rCCTL_BLOCK_RESET |= CCTL_VENC_BLOCK;	
    rCCTL_BLOCK_RESET &= ~(CCTL_VENC_BLOCK);    /* no need delay */
}

void CctlVdecReset()
{
    rCCTL_BLOCK_RESET |= CCTL_VDEC_BLOCK;	
    rCCTL_BLOCK_RESET &= ~(CCTL_VDEC_BLOCK);    /* no need delay */	
}

/*
***************************************************************************
*                              CctlVcodecClkSet
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : This function is for mpeg encode & decode.
***************************************************************************
*/
int CctlVcodecClkSet(void)
{
    static int mpeg_clock_set = 0;
    unsigned int cpu_sr;

    OS_ENTER_CRITICAL();

    if(mpeg_clock_set) {
        OS_EXIT_CRITICAL();
        return FAILED;
    }    
    
    mpeg_clock_set = 1;   
    
    /* enable clock, select risc pll, use regular devider */    
    rCCTL_VCODEC_CLOCK_DIVIDE = CCTL_VCODEC_CLOCK_ENABLE | CCTL_VCODEC_PLL_SOURCE_RISC |
                            CCTL_VCODEC_DIVIDER_TYPE_REGULAR | CCTL_VCODEC_DIVIDER_VALUE;  
  //  rCCTL_VCODEC_CLOCK_DIVIDE = CCTL_VCODEC_CLOCK_ENABLE | CCTL_VCODEC_PLL_SOURCE_SDRAM |
     //                      CCTL_VCODEC_DIVIDER_TYPE_REGULAR | CCTL_VCODEC_DIVIDER_VALUE;  

    OS_EXIT_CRITICAL();
                          
    return SUCCESSFUL;   
}
/*
***************************************************************************
*                              CctlVinBlockEn
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void CctlVinBlockEn(void)
{
    rCCTL_BLOCK_ENABLE |= CCTL_VIN_BLOCK;	
}

/*
***************************************************************************
*                              CctlVencBlockEn
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void CctlVencBlockEn(void)
{
    rCCTL_BLOCK_ENABLE |= CCTL_VENC_BLOCK;	
}

/*
***************************************************************************
*                              CctlVdecBlockEn
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void CctlVdecBlockEn(void)
{
    rCCTL_BLOCK_ENABLE |= CCTL_VDEC_BLOCK;	
}

/*
***************************************************************************
*                              CctlVdecBlockDis
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void CctlVdecBlockDis(void)
{
    rCCTL_BLOCK_ENABLE &= (~CCTL_VDEC_BLOCK);	
}
#endif

//Audio Module////////////////////
//#ifdef AUDIO_MODULE_INCLUDE
#include "audio.h"
/*
***************************************************************************
*                           GLOBAL FUNCTIONS
***************************************************************************
*/

/*
***************************************************************************
*                              CctlAudioClkSet
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : This function should be called after AppAudioTimingSet is called
*              (AppAudioTimingSet will set rAUD_CTL).
***************************************************************************
*/
int CctlAudioClkSet(void)
{
    if(!(rAUD_CTL & 0x08000000))    // ACLK is input?
        return SUCCESSFUL;          // rCCTL_AUDIO_CLOCK_DIVIDE need not be set
        
    /* enable clock, select risc pll, use regular devider */    
   // rCCTL_AUDIO_CLOCK_DIVIDE = CCTL_AUDIO_CLOCK_ENABLE | CCTL_AUDIO_PLL_SOURCE_RISC | \
                          CCTL_AUDIO_DIVIDER_TYPE_REGULAR | CCTL_AUDIO_DIVIDER_VALUE;  

    rCCTL_AUDIO_CLOCK_DIVIDE = CCTL_AUDIO_CLOCK_ENABLE | CCTL_AUDIO_PLL_SOURCE_RISC | \
                               CCTL_AUDIO_DIVIDER_TYPE_ACCURATE | CCTL_AUDIO_DIVIDER_VALUE;

    return SUCCESSFUL;                          
}

/*
***************************************************************************
*                              CctlAudioBlockEn
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void CctlAudioBlockEn(void)
{
    rCCTL_BLOCK_ENABLE |= CCTL_AUDIO_BLOCK;	
}
//#endif

