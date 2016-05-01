/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Aug 11, 2005                                                           
**********************************************************************************************************
*/



#include "sif.h"



/*
*********************************************************************************************
*                                       SifModeRegSet
*
* Description: Set rSIF_MODE.
*
* Arguments  : mode         can be the following values:
*                             SIF_BOOT_MODE;
*                             SIF_CBUS_MODE;
*                             SIF_DBUS_MODE
*              cs_sel       can be:
*                             SIF_CS_n0
*                             SIF_CS_n1
*                             SIF_CS_n2
*                             SIF_CS_n3
*                             SIF_CS_n4
*                             SIF_CS_n5
*                             SIF_CS_n6
*                             SIF_CS_n7
*              ctl_sel      can be:
*                             SIF_CTL_SEL_NONE 每 no control word
*                             SIF_CTL_SEL_8BIT 每 control word is 8 bit wide
*                             SIF_CTL_SEL_16BIT 每 control word is 16 bit wide
*              add_sel      can be:
*                             SIF_ADD_SEL_NONE 每 No address word
*                             SIF_ADD_SEL_1BYTE 每 address is 1 byte wide
*                             SIF_ADD_SEL_2BYTE 每 address is 2 byte wide
*                             SIF_ADD_SEL_3BYTE 每 address is 3 byte wide
*              op           is the operation, can be
*                             SIF_WRITE
*                             SIF_READ
*              sID          is the stream ID for DBUS mode
*              blen         is the byte length of the transaction for DBUS mode. 
*                           Counted by bytes.
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifModeRegSet(int mode, int cs_sel, int ctl_sel, int add_sel, int op, int sID, int blen)
{
    rSIF_MODE = (mode<<29) | (cs_sel<<26) | (ctl_sel<<24) | (add_sel<<22) | (op<<21) | \
                (sID<<12) | blen; 	
}

/*
*********************************************************************************************
*                                      SifModeSet
*
* Description: Change the mode.
*
* Arguments  : op   is the value to be writen into the rSIF_MODE register. They are:
*                           SIF_BOOT_MODE
*                           SIF_CBUS_MODE
*                           SIF_DBUS_MODE  
*                           
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifModeSet(int op)
{
    rSIF_MODE = rSIF_MODE & (~(0x3<<29)) | (op<<29);
}

/*
*********************************************************************************************
*                                       SifChipSel
*
* Description: Chip selection control.
*
* Arguments  : cs     is the value to be writen into the rSIF_MODE register.   
*                         If cs = SIF_CS_n0, the serial flash will hook to CS_n[0];
*                         If cs = SIF_CS_n1, the serial flash will hook to CS_n[1];
*                         If cs = SIF_CS_n2, the serial flash will hook to CS_n[2];
*                         If cs = SIF_CS_n3, the serial flash will hook to CS_n[3];
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifChipSel(unsigned int cs)
{
    rSIF_MODE = rSIF_MODE & (~(0x3<<26)) | (cs<<26);
}

/*
*********************************************************************************************
*                                      SifOpModeSet
*
* Description: Decide the serial write operation or read operation.
*
* Arguments  : op   is the value to be writen into the rSIF_MODE register.
*                           If op = SIF_WRITE, it is a serial write operation.   
*                           If op = SIF_READ, it is a serial read operation;
*                           
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifOpModeSet(int op)
{
    rSIF_MODE = rSIF_MODE & (~(0x1<<21)) | (op<<21);
}

/*
*********************************************************************************************
*                                      SifStreamIDSet
*
* Description: Specify the stream ID for DBUS mode.
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifStreamIDSet(int sID)
{
    rSIF_MODE = rSIF_MODE & (~(0x3f<<12)) | (sID<<12);
}

/*
*********************************************************************************************
*                                      SifBlenSet
*
* Description: Specify the byte length of the transaction.
*
* Arguments  : blen   is the value to be writen into the rSIF_MODE register.   
*                           If blen = 0, there is no data to be transmitted.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifBlenSet(int blen)
{
    rSIF_MODE = rSIF_MODE & (~0xfff) | blen;
}

/*
*********************************************************************************************
*                                       SifTimingRegSet
*
* Description: Set rSIF_TIMING.
*
* Arguments  : si_timing     can be:
*                              SIF_SI_TIMING_RISING - Sample serial input at rising 
*                                                     edge of SPI_CLK
*                              SIF_SI_TIMING_FALLING - Sample serial input at falling 
*                                                      edge of SPI_CLK
*              so_timing     can be:
*                              SIF_SO_TIMING_RISING - serial output is off rising 
*                                                     edge of SPI_CLK
*                              SIF_SO_TIMING_FALLING - serial output is off falling 
*                                                     edge of SPI_CLK 
*              clk_pol       can be:
*                              SIF_CLK_POL_IDLE1 - SPI_CLK IDLE level is at 1 
*                              SIF_CLK_POL_IDLE0 - SPI_CLK IDLE level is at 0 
*              cs_width      can be:
*                              SIF_CS_WIDTH_OFF_NONE - CS does not turn off between data
*                              SIF_CS_WIDTH_OFF_1BYTE - CS turns off between every 1 byte
*                              SIF_CS_WIDTH_OFF_2BYTE - CS turns off between every 2 byte 
*                              SIF_CS_WIDTH_OFF_4BYTE - CS turns off between every 4 byte 
*              cs_dly        can be:
*                              SIF_CS_DLY_NONE - no delay
*                              SIF_CS_DLY_1CLK - 1 SPI_CLK
*                              SIF_CS_DLY_2CLK - 2 SPI_CLK
*                              SIF_CS_DLY_4CLK - 4 SPI_CLK
*              cs_off        can be:
*                              SIF_CS_OFF_1CLK - 1 SPI_CLK
*                              SIF_CS_OFF_2CLK - 2 SPI_CLK
*                              SIF_CS_OFF_4CLK - 4 SPI_CLK
*                              SIF_CS_OFF_8CLK - 8 SPI_CLK 
*              gap           can be:
*                              SIF_GAP_NONE - no GAP
*                              SIF_GAP_1BYTE - 1 byte of gap
*                              SIF_GAP_2BYTE - 2 byte of gap
*                              SIF_GAP_3BYTE - 3 byte of gap
*                              SIF_GAP_4BYTE - 4 byte of gap
*                              SIF_GAP_5BYTE - 5 byte of gap
*                              SIF_GAP_6BYTE - 6 byte of gap
*                              SIF_GAP_7BYTE - 7 byte of gap
*                              SIF_GAP_8BYTE - 8 byte of gap 
*              bgap          can be:
*                              SIF_BGAP_NONE - no BGAP
*                              SIF_BGAP_1BIT - 1 bit of gap
*              clk_div       is the clock divider of SPI clock generator. 
*                            If the register value is n and the input clock frequency is F,
*                            then the SPI clock frequency is F/(2*(n+1)). 
*                            If the input clock frequency is 100Mhz, 
*                            the maximum SPI speed is 50Mhz.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void SifTimingRegSet(int si_timing, int so_timing, int clk_pol, int cs_width,\
                    int cs_dly, int cs_off, int gap, int bgap, int clk_div)
{
    rSIF_TIMING = (si_timing<<31) | (so_timing<<30) | (clk_pol<<29) | (cs_width<<26) |\
                  (cs_dly<<24) | (cs_off<<22) | (gap<<18) | (bgap<<17) | (clk_div<<8);	
}

