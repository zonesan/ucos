/*
*************************************************************************************
*                       Copyright (c) 2005 vBridge Microsystem, Inc.  
*                            Unpublished & Not for Publication
*                                   All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Dec 6, 2005                                                           
*************************************************************************************
*/



#include "risc.h"

   

#define  PIF_PCI_LEN_MAX    4096    // by bytes



/*
*************************************************************************************
*                          Miscellaneous
*************************************************************************************
*/
/* PCMCIA interface generic operations */
/* mode */
#define  PIF_BOOT_MODE    0x0      // 0 - boot mode
#define  PIF_CBUS_MODE    0x1      // 1 - CBUS mode
#define  PIF_DBUS_MODE    0x2      // 2 - DBUS mode

/* It specifies the burst size on the memory bus for master transactions. */
#define  PCMCIA_MSTR_BS_8BYTE    0x0     // 0 每 8 byte burst
#define  PCMCIA_MSTR_BS_16BYTE   0x1     // 1 每 16 byte burst
#define  PCMCIA_MSTR_BS_32BYTE   0x2     // 2 每 32 byte burs
#define  PCMCIA_MSTR_BS_64BYTE   0x3     // 3 每 64 byte burst                                                                                                                                                                                                  
                                 
/* PCMCIA interface timing */
#define  PIF_WAIT_INPACK         0x1       // wait for 'inpack' signal during IORD
#define  PIF_NO_WAIT             0x0       // don＊t care 'inpack' signal

#define  PCMCIA_WIDTH_8BIT       0x2       // PCMCIA is 8 bit 
#define  PCMCIA_WIDTH_16BIT      0x1       // PCMCIA is 16 bit 
#define  PCMCIA_WIDTH_DEPEND     0x0       // PCMCIA width is depending on 
                                           // 'is_16bit' pin input
                                                                  
#define  PCMCIA_CHECK_WAIT       0x1       // 1 每 PCMCIA check the ＆wait＊ input pin during OE/WE/IOWR/IORD pulse
#define  PCMCIA_NO_CHECK         0x0       // 0 每 PCMCIA doesn＊t check ＆wait＊ input pin
                            
#define  PCMCIA_INSERT_IDLE      0x1       // 1 每 PCMCIA insert idle cycle for specified cycles 
                                           // before actual transfer cycle
#define  PCMCIA_NO_INSERT        0x0       // 0 每 PCMCIA doesn＊t insert idle cycles                                                                 

#define  PIF_CS0    0x0        
#define  PIF_CS1    0x1        
#define  PIF_CS2    0x2        
#define  PIF_CS3    0x3        

#define  PIF_IORD    0x0      // IO read use IORD
#define  PIF_IOWR    0x1      // IO write use IOWR
#define  PIF_OE      0x2      // memory read use OE
#define  PIF_WE      0x3      // memory write use WE 
  
  
  
                           
/*
*********************************************************************************************************
*                          PIF register definitions
*********************************************************************************************************
*/
#define  PIF_BASE_ADDR    BLOCK_BASE_ADDR(PIF_BLOCK)

#define  rPIF_PCI_CTL      (*(volatile unsigned *)(PIF_BASE_ADDR + 0x0))     // This register controls the generic operations of the PCI/PCMCIA interface.
#define  rPIF_PCI_DEV_A    (*(volatile unsigned *)(PIF_BASE_ADDR + 0x4))     // This register specifies the device byte address for the PCI/PCMCIA master transfer.
#define  rPIF_PCI_MEM_A    (*(volatile unsigned *)(PIF_BASE_ADDR + 0x8))     // This register specifies the internal memory byte address for PCI/PCMCIA master transfer.
#define  rPIF_PCI_LEN      (*(volatile unsigned *)(PIF_BASE_ADDR + 0xC))     // This register specifies the transfer length for PCI/PCMCIA master transfer.     
#define  rPIF_PCI_TIM      (*(volatile unsigned *)(PIF_BASE_ADDR + 0x10))    // This register specifies the PCMCIA interface timing.
#define  rPIF_PCMCIA_DATA  (*(volatile unsigned *)(PIF_BASE_ADDR + 0x14))    // Data register for master data transaction
#define  rPIF_INTR         (*(volatile unsigned *)(PIF_BASE_ADDR + 0x18))    // This register controls and shows the interrupt status of PIF.



/*
*********************************************************************************************************
*                          PIF register bit definitions
*********************************************************************************************************
*/
// The PCMCIA interface generic operation control register bit definitions
#define  BIT_START            (0x1<<31)     

// The PCMCIA interface timing register bit definitions
#define  BIT_XFR_TYPE         (0x3<<30)

// The PIF interrupt status register bit definitions
#define  BIT_INTR_ENA         (0x1<<31)
#define  BIT_DONE             (0x1<<29)



/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/
#define PIF_INTR_EN()     rPIF_INTR |= BIT_INTR_ENA        
#define PIF_INTR_DIS()    rPIF_INTR &= ~BIT_INTR_ENA       

#define PIF_DONE_CLEAR()  rPIF_INTR |= BIT_DONE

#define PIF_START()       rPIF_PCI_CTL |= BIT_START

#define PIF_READ_SET()    rPIF_PCI_TIM = rPIF_PCI_TIM & (~BIT_XFR_TYPE) | (PIF_OE<<30)
#define PIF_WRITE_SET()   rPIF_PCI_TIM = rPIF_PCI_TIM & (~BIT_XFR_TYPE) | (PIF_WE<<30)



/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/
void PifPCMCIAModeRegSet(int mode, int mstr_bs);
void PifTimingRegSet(int setup, int hold, int interval, int wait_cycle, int ioack_wait,\
                     int pcmcia_width, int wait, int swait, int devID, int xfr_type);
void PifWaitTillDone(void);
