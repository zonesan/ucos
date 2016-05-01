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
* Date        : May 22 , 2007                                                           
***************************************************************************
*/

#include  "includes.h"
#include  "typedef.h"
#include  "cpu.h"
#include  "gpio.h"
#include  "techwell_app.h"

#include	"prtcl.h"

Flag fail;

/* User's own code */

#define CASE_CNT 19

#if 0
#define TW2835_CS0  (0x1 << 4)
#define TW2835_CS1  (0x1 << 5)
#define TW2835_CS2  (0x1 << 6)
#define TW2835_CS3  (0x1 << 7)
#define TW2835_RST  (0x1 << 2)
#define TW2835_HSPB (0x1 << 0)

#define TW2835_IIC_ADDR 0x7e


void TechwellAppMain(void)
{

    int case_i;
    Word16 call_cnt;
    Word16 seed16 = 0;




    tw2835_io_init();
    tw2835_reset();
    TW_IICDelay(200000);
    select_chip(0);

    //HwTW2835Init(PTH_X, TV_MODE_PAL, 4, 0);

    tw2835();




}



void tw2835_io_init(void)
{
    int temp;
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x84);
    temp &= ~(TW2835_CS0 | TW2835_CS1 | TW2835_CS2 | TW2835_CS3| TW2835_RST | TW2835_HSPB);
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x84) = temp;
    
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp |= TW2835_HSPB;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

void tw2835_reset()
{
    int temp;

    set_gpio_out(GPIO_I2C_SCL_BIT);
    
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp &= ~TW2835_RST;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
    TW_IICDelay(200000);
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp |= TW2835_RST;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

int select_chip(unsigned int cs)
{
    unsigned int temp;
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    if(cs > 3)
        return -1;
    temp |= (0xf << 4);
    temp &= ~(0x1 << (4 + cs));
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

#endif


