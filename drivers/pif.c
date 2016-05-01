/*
*************************************************************************************
*                            Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                  Unpublished & Not for Publication
*                                        All Rights Reserved                   
*   
* File        : pif.c                     
*                                                                         
* Description : 
*             
* Date        :                                                            
*************************************************************************************
*/



#include "pif.h"



/*
*************************************************************************************
*                                       PifPCMCIAModeRegSet
*
* Description: Set rPIF_PCI_CTL.
*
* Arguments  : mode         can be the following values:
*                             PIF_BOOT_MODE;
*                             PIF_CBUS_MODE;
*                             PIF_DBUS_MODE
*              mstr-bs      It specifies the burst size on the memory bus for master 
*                           transactions. can be:
*                             PCMCIA_MSTR_BS_8BYTE
*                             PCMCIA_MSTR_BS_16BYTE
*                             PCMCIA_MSTR_BS_32BYTE
*                             PCMCIA_MSTR_BS_64BYTE
*
* Return     : none.
*
* Note(s)    : 
*************************************************************************************
*/
void PifPCMCIAModeRegSet(int mode, int mstr_bs)
{
    rPIF_PCI_CTL = (mode<<26) | (mstr_bs<<21);                  
}

/*
*************************************************************************************
*                                       PifTimingRegSet
*
* Description: Set rPIF_PCI_TIM.
*
* Arguments  : setup         controls the setup time from PCMCIA CS on to the 
*                            OE/WE/IOWR/IORD strobe on. It＊s the PIF clock cycles.
*              hold          controls the hold time from the OE/WE/IOWR/IORD strobe 
*                            off to the CS.
*              interval      controls the interval of the OE/WE/IOWR/IORD on time.
*              wait_cycle    This field is useful when 'swait' bit is 1. It is the 
*                            idle cycles between each access started from asserting CS.
*              ioack_wait    can be:
*                              PIF_WAIT_INPACK - wait for 'inpack' signal during IORD
*                              PIF_NO_WAIT - don＊t care 'inpack' signal
*              pimcia_width  can be:
*                              PCMCIA_WIDTH_8BIT 每 PCMCIA data is 8 bit wide
*                              PCMCIA_WIDTH_16BIT 每 PCMCIA data is 16 bit wide
*                              PCMCIA_WIDTH_DEPEND 每 PCMCIA width is depending on 
*                                                    'is_16bit' pin input
*              wait          can be:
*                              PCMCIA_CHECK_WAIT 每 PCMCIA check the ＆wait＊ input 
*                                                  pin during OE/WE/IOWR/IORD pulse
*                              PCMCIA_NO_CHECK 每 PCMCIA doesn＊t check ＆wait＊ input pin
*              swait         can be:
*                              PCMCIA_INSERT_IDLE 每 PCMCIA insert idle cycles for 
*                                                   specified cycles before actual
*                                                   transfer cycle
*                              PCMCIA_NO_INSERT 每 PCMCIA doesn＊t insert idle cycles
*              devID         controls which PCMCIA device to access. One of the 
*                            4 CS signals are enabled according to:
*                              PIF_CS0
*                              PIF_CS1
*                              PIF_CS2
*                              PIF_CS3
*
*              xfr_type      can be:
*                              PIF_IORD - IO read use IORD
*                              PIF_IOWR - IO write use IOWR
*                              PIF_OE - memory read use OE
*                              PIF_WE - memory write use WE
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void PifTimingRegSet(int setup, int hold, int interval, int wait_cycle, int ioack_wait,\
                     int pcmcia_width, int wait, int swait, int devID, int xfr_type)
{
    rPIF_PCI_TIM = setup | (hold<<4) | (interval<<8) | (wait_cycle<<16) | (ioack_wait<23)|\
                   (pcmcia_width<<24) | (wait<<26) | (swait<<27) | (devID<<28) |\
                   (xfr_type<<30);	
}

/*
*********************************************************************************************
*                                       PifWaitTillDone
*
* Description: It is a delay function, waiting the master transaction is finished.
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void PifWaitTillDone(void)
{
    /* Wait till the busy bit is clear & the done bit is set */	
    while(rPIF_INTR & 0x60000000 ^ (0x1<<29));
    
    /* Clear the done bit */
    PIF_DONE_CLEAR();	
}


