/*
*************************************************************************************
*                       Copyright (c) 2005 vBridge Microsystem, Inc.  
*                             Unpublished & Not for Publication
*                                   All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        : Aug 11, 2005                                                           
*************************************************************************************
*/



#include "risc.h"



/*
*********************************************************************************************************
*                              Miscellaneous
*********************************************************************************************************
*/
/* modes of the serial interface */

/*
0 - BOOT mode. In BOOT mode the control registers are set by boot strap.
1 - CBUS mode. In CBUS mode, data access is through CBUS. Control registers are set through CBUS. 
               BOOT mode is a special case of CBUS mode 
               where the RUN bit is automatically set by accessing the SIF_DAT register. 
2 - DBUS mode. Data access is through DBUS via the specified stream. 
               Control registers are set through CBUS.
*/
#define  SIF_BOOT_MODE    0x0      
#define  SIF_CBUS_MODE    0x1
#define  SIF_DBUS_MODE    0x2

/* Chip selection control */
#define  SIF_CS_n0        0x0
#define  SIF_CS_n1        0x1
#define  SIF_CS_n2        0x2
#define  SIF_CS_n3        0x3
#define  SIF_CS_n4        0x4
#define  SIF_CS_n5        0x5
#define  SIF_CS_n6        0x6
#define  SIF_CS_n7        0x7 

/* 
The control word is the first that shifts out on the serial interface 
0 每 no control word
1 每 control word is 8 bit wide
2 每 control word is 16 bit wide
*/
#define  SIF_CTL_SEL_NONE    0x0
#define  SIF_CTL_SEL_8BIT    0x1
#define  SIF_CTL_SEL_16BIT   0x2

/*
The address word is the second that shifts out on the serial interface
0 每 No address word
1 每 address is 1 byte wide
2 每 address is 2 byte wide
3 每 address is 3 byte wide
*/
#define  SIF_ADD_SEL_NONE    0x0
#define  SIF_ADD_SEL_1BYTE   0x1
#define  SIF_ADD_SEL_2BYTE   0x2
#define  SIF_ADD_SEL_3BYTE   0x3

/* operation mode: write or read */
#define  SIF_WRITE        0x1   
#define  SIF_READ         0x0


/* serial interface timing */
#define  SIF_SI_TIMING_RISING     1    // Sample serial input at rising edge of SPI_CLK
#define  SIF_SI_TIMING_FALLING    0    // Sample serial input at falling edge of SPI_CLK

#define  SIF_SO_TIMING_RISING    0x1   // serial output is off rising edge of SPI_CLK 
#define  SIF_SO_TIMING_FALLING   0x0   // serial output is off falling edge of SPI_CLK

#define  SIF_CLK_POL_IDLE1       0x1   // 1 - SPI_CLK IDLE level is at 1 
#define  SIF_CLK_POL_IDLE0       0x0   // 0 - SPI_CLK IDLE level is at 0

#define  SIF_CS_WIDTH_OFF_NONE   0x0   // 00 - CS does not turn off between data
#define  SIF_CS_WIDTH_OFF_1BYTE  0x1   // 01 - CS turns off between every 1 byte */
#define  SIF_CS_WIDTH_OFF_2BYTE  0x2   // 10 - CS turns off between every 2 byte */
#define  SIF_CS_WIDTH_OFF_4BYTE  0x3   // 11 - CS turns off between every 4 byte */

/* It specifies the setup and hold timing of CS vs SPI_CLK */
#define  SIF_CS_DLY_NONE         0x0   // 0 - no delay
#define  SIF_CS_DLY_1CLK         0x1   // 1 - 1 SPI_CLK
#define  SIF_CS_DLY_2CLK         0x2   // 2 - 2 SPI_CLK
#define  SIF_CS_DLY_4CLK         0x3   // 3 - 4 SPI_CLK

/* It specifies the off cycles of CS vs SPI_CLK */
#define  SIF_CS_OFF_1CLK         0x0   // 0 - 1 SPI_CLK
#define  SIF_CS_OFF_2CLK         0x1   // 1 - 2 SPI_CLK
#define  SIF_CS_OFF_4CLK         0x2   // 2 - 4 SPI_CLK
#define  SIF_CS_OFF_8CLK         0x3   // 3 - 8 SPI_CLK

/* It specifies the number of dummy bytes insterted between the address and the data on the serial line */
#define  SIF_GAP_NONE            0x0   // 0 - no GAP
#define  SIF_GAP_1BYTE           0x1   // 1 - 1 byte of gap
#define  SIF_GAP_2BYTE           0x2   // 2 - 2 byte of gap
#define  SIF_GAP_3BYTE           0x3   // 3 - 3 byte of gap
#define  SIF_GAP_4BYTE           0x4   // 4 - 4 byte of gap
#define  SIF_GAP_5BYTE           0x5   // 5 - 5 byte of gap
#define  SIF_GAP_6BYTE           0x6   // 6 - 6 byte of gap
#define  SIF_GAP_7BYTE           0x7   // 7 - 7 byte of gap


/* It specifies the extra dummy bit insterted between the GAP and the data on the serial line */                               
#define  SIF_BGAP_NONE           0x0   // 0 - no BGAP
#define  SIF_BGAP_1BIT           0x1   // 1 - 1 bit of gap


#define  SifIntrEn()      rSIF_STAT |= BIT_SIF_INTR_EN
#define  SifIntrDis()     rSIF_STAT &= (~BIT_SIF_INTR_EN)

#define  SifIntrClr()     rSIF_STAT |= BIT_SIF_INTR

#define  SifBusyRead()            rSIF_MODE & 0x80000000
#define  SifDataRead()            rSIF_DAT

#define  SifStart()               rSIF_MODE |= BIT_SIF_RUN
#define  SifAddrSet(addr)         rSIF_ADDR = addr
#define  SifCommandSet(command)   rSIF_CTL = command
#define  SifDataWrite(data)       rSIF_DAT = data

/*
*********************************************************************************************************
*                          SIF register definitions
*********************************************************************************************************
*/
#define  SIF_BASE_ADDR    BLOCK_BASE_ADDR(SIF_BLOCK)

#define  rSIF_MODE      (*(volatile unsigned *)(SIF_BASE_ADDR + 0x0))     /* Serial interface mode register */
#define  rSIF_CTL       (*(volatile unsigned *)(SIF_BASE_ADDR + 0x4))     /* Serial interface control data register */
#define  rSIF_ADDR      (*(volatile unsigned *)(SIF_BASE_ADDR + 0x8))     /* Serial interface address data register */
#define  rSIF_DAT       (*(volatile unsigned *)(SIF_BASE_ADDR + 0xC))     /* Serial interface data register */
#define  rSIF_STAT      (*(volatile unsigned *)(SIF_BASE_ADDR + 0x10))    /* Serial interface status register */
#define  rSIF_TIMING    (*(volatile unsigned *)(SIF_BASE_ADDR + 0x14))    /* Serial interface timing register */

/*
*********************************************************************************************************
*                          SIF register bit definitions
*********************************************************************************************************
*/
// rSIF_MODE bit definitions
#define  BIT_SIF_RUN          (0x1<<31)

// rSIF_STAT bit definitions
#define  BIT_SIF_INTR_EN      (0x1<<31)
#define  BIT_SIF_INTR         (0x1<<30)
#define  BIT_SIF_FULL_EMPTY   (0x1<<29)
 
/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/
void SifModeRegSet(int mode, int cs_sel, int ctl_sel, int add_sel, int op, int sID, int blen);
void SifModeSet(int op);
void SifChipSel(unsigned int cs);
void SifOpModeSet(int op);
void SifStreamIDSet(int sID);
void SifBlenSet(int blen);
void SifTimingRegSet(int si_timing, int so_timing, int clk_pol, int cs_width,\
                    int cs_dly, int cs_off, int gap, int bgap, int clk_div);



