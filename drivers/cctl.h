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


#ifndef __CCTL_H__
#define __CCTL_H__

#include "risc.h" 
#include "chip_cfg.h"


/*
***************************************************************************
*                        CCTL register bit definitions
***************************************************************************
*/
#define  CCTL_BASE_ADDR    BLOCK_BASE_ADDR(CCTL_BLOCK)

#define  rCCTL_BOOT_LOAD_CTL    *(volatile unsigned *)(CCTL_BASE_ADDR + 0x0)   /* This register controls the boot load address and boot load code length. It is used when CPU
                                                                                reboots itself for the first time after reset. */
#define  rCCTL_PLL_TIMER               *(volatile unsigned *)(CCTL_BASE_ADDR + 0x4)
#define  rCCTL_RISC_PLL                *(volatile unsigned *)(CCTL_BASE_ADDR + 0x8)
#define  rCCTL_SDRAM_PLL               *(volatile unsigned *)(CCTL_BASE_ADDR + 0xC)
#define  rCCTL_PIN_MUX_CTL             *(volatile unsigned *)(CCTL_BASE_ADDR + 0x10)
#define  rCCTL_AUDIO_CLOCK_DIVIDE      *(volatile unsigned *)(CCTL_BASE_ADDR + 0x14)
#define  rCCTL_EMAC_CLOCK_DIVIDE       *(volatile unsigned *)(CCTL_BASE_ADDR + 0x18)    
#define  rCCTL_MDIO_CLOCK_DIVIDE       *(volatile unsigned *)(CCTL_BASE_ADDR + 0x1C)
#define  rCCTL_HOST_CLOCK_DIVIDE       *(volatile unsigned *)(CCTL_BASE_ADDR + 0x20)
#define  rCCTL_RISC_SDRAM_CLOCK_DIVIDE *(volatile unsigned *)(CCTL_BASE_ADDR + 0x24)
#define  rCCTL_VOUT_CLOCK_DIVIDE       *(volatile unsigned *)(CCTL_BASE_ADDR + 0x28)
#define  rCCTL_VIN_CLOCK_DIVIDE        *(volatile unsigned *)(CCTL_BASE_ADDR + 0x2C)
#define  rCCTL_VCODEC_CLOCK_DIVIDE     *(volatile unsigned *)(CCTL_BASE_ADDR + 0x30)
#define  rCCTL_BLOCK_ENABLE            *(volatile unsigned *)(CCTL_BASE_ADDR + 0x34)
#define  rCCTL_BLOCK_RESET             *(volatile unsigned *)(CCTL_BASE_ADDR + 0x38)
#define  rCCTL_CB_DEBUG_OUT            *(volatile unsigned *)(CCTL_BASE_ADDR + 0x3C)
#define  rCCTL_PIN_CTL                 *(volatile unsigned *)(CCTL_BASE_ADDR + 0x40)

/*
***************************************************************************
*                        risc & modules clock values
***************************************************************************
*/
#define EXT_CLOCK      12500000    // used for watchdog 


/* Audio clock value */
#define  CCTL_AUDIO_CLOCK_ENABLE        (0x1<<31)

#define  CCTL_AUDIO_PLL_SOURCE_RISC     (0x0<<30)    // use RISC PLL    
#define  CCTL_AUDIO_PLL_SOURCE_SDRAM    (0x1<<30)    // use SDRAM PLL

#define  CCTL_AUDIO_DIVIDER_TYPE_ACCURATE    (0x1<<29)
#define  CCTL_AUDIO_DIVIDER_TYPE_REGULAR     (0x0<<29)

#if CPU_MAIN_CLOCK == 200000000
#define  CCTL_AUDIO_DIVIDER_VALUE    0xa7c5ac //for CCTL_AUDIO_DIVIDER_TYPE_ACCURATE clock
//#define  CCTL_AUDIO_DIVIDER_VALUE    0x7
#elif CPU_MAIN_CLOCK  == 25000000
#define  CCTL_AUDIO_DIVIDER_VALUE    0x0
#endif


/* VIN clock divide value */
#define  CCTL_VIN_CLOCK_ENABLE             (0x1<<31)

#define  CCTL_VIN_PLL_SOURCE_RISC          (0x0<<30)
#define  CCTL_VIN_PLL_SOURCE_SDRAM         (0x1<<30)

#define  CCTL_VIN_DIVIDER_TYPE_ACCURATE    (0x1<<29)
#define  CCTL_VIN_DIVIDER_TYPE_REGULAR     (0x0<<29)

#if CPU_MAIN_CLOCK == 200000000
#define  CCTL_VIN_DIVIDER_VALUE    0x3    // Vin clk is 25MHz
#elif CPU_MAIN_CLOCK  == 25000000
#define  CCTL_VIN_DIVIDER_VALUE    0x6    // Vin clk is 1.79MHz
#endif


/* Vcodec clock divide value */
#define  CCTL_VCODEC_CLOCK_ENABLE             (0x1<<31)

#define  CCTL_VCODEC_PLL_SOURCE_RISC          (0x0<<30)
#define  CCTL_VCODEC_PLL_SOURCE_SDRAM         (0x1<<30)

#define  CCTL_VCODEC_DIVIDER_TYPE_ACCURATE    (0x1<<29)
#define  CCTL_VCODEC_DIVIDER_TYPE_REGULAR     (0x0<<29)

#if CPU_MAIN_CLOCK == 200000000
#define  CCTL_VCODEC_DIVIDER_VALUE    0x0    // this value should be re-considered    
#elif CPU_MAIN_CLOCK  == 25000000
#define  CCTL_VCODEC_DIVIDER_VALUE    0x0   
#endif


/*
***************************************************************************
*                        CCTL Blocks, used for 0x34 & 0x38
***************************************************************************
*/
#define  CCTL_RISC0_BLOCK    (0x1<<0)
#define  CCTL_RISC1_BLOCK    (0x1<<1)
#define  CCTL_MC_BLOCK       (0x1<<2)
#define  CCTL_SIF_BLOCK      (0x1<<3)
#define  CCTL_SC_BLOCK       (0x1<<4)
#define  CCTL_VIN_BLOCK      (0x1<<5)
#define  CCTL_VOUT_BLOCK     (0x1<<6)
#define  CCTL_AUDIO_BLOCK    (0x1<<7)
#define  CCTL_DMAC_BLOCK     (0x1<<8)
#define  CCTL_PPE_BLOCK      (0x1<<9)
#define  CCTL_PIF_BLOCK      (0x1<<10)
#define  CCTL_VSC_BLOCK      (0x1<<11)
#define  CCTL_EMAC0_BLOCK    (0x1<<12)
#define  CCTL_EMAC1_BLOCK    (0x1<<13)
#define  CCTL_SEC_BLOCK      (0x1<<14)
#define  CCTL_SD0_BLOCK      (0x1<<15)
#define  CCTL_SD1_BLOCK      (0x1<<16)
#define  CCTL_UART0_BLOCK    (0x1<<17)
#define  CCTL_UART1_BLOCK    (0x1<<18)
#define  CCTL_VENC_BLOCK     (0x1<<19)
#define  CCTL_VDEC_BLOCK     (0x1<<20)
#define  CCTL_IDE_BLOCK      (0x1<<21)
#define  CCTL_RTC_BLOCK      (0x1<<22)
#define  CCTL_WD_BLOCK       (0x1<<23)
// Added by hjm
#define CCTL_VIN_RESET_BIT (0x1 << 5)

/*
***************************************************************************
*                              Miscellaneous
***************************************************************************
*/
#define  LED1_ON()    /*rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/\
                      rRISC_GPIO &= ~0x4000000      /* Pull GPIO26 low */
#define  LED1_OFF()   /*rRISC_GPIO_DIR &= ~0x4000000; /* Set GPIO26 output*/\
                      rRISC_GPIO |= 0x4000000       /* Pull GPIO26 high */                    
                    
#define  LED0_ON()    /*rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/\
                      rRISC_GPIO &= ~0x2000000      /* Pull GPIO25 low */
#define  LED0_OFF()   /*rRISC_GPIO_DIR &= ~0x2000000; /* Set GPIO25 output*/\
                      rRISC_GPIO |= 0x2000000       /* Pull GPIO25 high */  
                      
/*
***************************************************************************
*                           FUCTION PROTYPES
***************************************************************************
*/
/* risc block clock set functions */
int CctlAudioClkSet(void);
int CctlVoutClkSet(void);
int CctlVinClkSet(void);
int CctlVcodecClkSet(void);

/* risc block enable functions */
void CctlAudioBlockEn(void);
void CctlVinBlockEn(void);
void CctlVencBlockEn(void);
void CctlVdecBlockEn(void);
void CctlVinReset();
void CctlVencReset();
void CctlVdecReset();
void CctlVdecBlockDis(void);
#endif


