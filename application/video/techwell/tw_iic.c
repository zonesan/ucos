#include  "includes.h"
#include  "typedef.h"
#include "tw_iic.h"
#include "tw_iic.h"
#include "regs.h"
#include "cpu.h"

//#define DEBUG_I2C

void TW_IICDelayLoop()
{
    asm volatile (
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop\n"
    "nop");
}
TW_IIC_Read_Delay(int cnt)
    {
 
#if 1
    int i;
    for(i=0; i<cnt*3; i++)
        TW_IICDelayLoop();
#endif
}
void TW_IICDelay(int cnt)
{
#if 0
    int i;
    for(i=0; i<2*cnt; i++)
        TW_IICDelayLoop();
#endif
}

void TW_IICinit(void)
{
    unsigned int cpu_sr;
    
    TW_IIC_SDA_OUT();
    TW_IIC_SDA_SET();
    TW_IIC_SCL_OUT();
    TW_IIC_SCL_SET();
}

void TW_IICStart(void)          /* statr the iic bus */
{
    unsigned int cpu_sr;

    //TW_IIC_SDA_OUT();    /*does this option necessary ?*/
    TW_IIC_SDA_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
}
void TW_IICStop(void)           /* stop the iic bus */
{
    unsigned int cpu_sr;

    TW_IIC_SDA_OUT();
    TW_IIC_SDA_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SDA_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
}
int  TW_IICRead(void)           /* read the iic bus */
{
    unsigned int cpu_sr;

    int i;
    int buf = 0;
    TW_IIC_SDA_OUT();
    TW_IIC_SDA_SET();
    TW_IIC_SDA_IN();
    for(i = 0 ; i < 8 ; i++)
    {
        buf = buf<<1;
        TW_IIC_SCL_SET();
        TW_IIC_Read_Delay(TW_IIC_PUSH_DELAY);
        if(TW_IIC_SDA_GET())
            buf |= 0x1;
        else
            buf &= ~0x1;
        TW_IIC_SCL_CLR();
        TW_IIC_Read_Delay(TW_IIC_PUSH_DELAY);
    }
    return buf;
}

void TW_IICWrite(int buf)       /* write the iic bus */
{
    unsigned int cpu_sr;

    int i;
    int gpio_dat;

    TW_IIC_SDA_OUT();
    buf &= 0xff;
    gpio_dat = rIIC_SDA_BIT_REG;
    for(i = 0; i <= 7; i++)
    {
        if((buf & 0x80))
	    gpio_dat |= (1<<rIIC_SDA_BIT_SHIFT);
        else
	    gpio_dat &= ~(1<<rIIC_SDA_BIT_SHIFT);

	clr_ie;
        rIIC_SDA_BIT_REG = gpio_dat;
        TW_IICDelay(TW_IIC_PUSH_DELAY);
	//TW_IIC_SCL_SET();
	rIIC_SCL_BIT_REG = gpio_dat | (1<<rIIC_SCL_BIT_SHIFT);
        TW_IICDelay(TW_IIC_PUSH_DELAY);
	//TW_IIC_SCL_CLR();
	rIIC_SCL_BIT_REG = gpio_dat & ~(1<<rIIC_SCL_BIT_SHIFT);
        //TW_IICDelay(TW_IIC_PUSH_DELAY);
	set_ie;

        buf = buf << 1;
    }
}
int TW_IICChkAck(void)         /* chack the acknowledge signel */
{
    unsigned int cpu_sr;

    int WaitTime;
    TW_IIC_SDA_IN();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    WaitTime = TW_IIC_WAIT_TIME;
    while(TW_IIC_SDA_GET())
    {
        WaitTime--;
        if(WaitTime <= 0)
            break;
    }
    /* to do : read ack signal */
    TW_IIC_SCL_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    /* to do : add err deal */
    if(WaitTime != 0)
        return TW_IIC_ACK_SUCCESS;
    printf("iic time out!\n");
    return TW_IIC_ACK_ERROR;
}

void TW_IICChkNAck(void)
{
    unsigned int cpu_sr;

    TW_IIC_SDA_IN();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
}
void TW_IICSendAck(void)        /* send the acknowledge signel */
{
    unsigned int cpu_sr;

    TW_IIC_SDA_OUT();
    TW_IIC_SDA_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
}

void TW_IICSendNAck(void)
{
    unsigned int cpu_sr;

    TW_IIC_SDA_OUT();
    TW_IIC_SDA_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_SET();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
    TW_IIC_SCL_CLR();
    TW_IICDelay(TW_IIC_PUSH_DELAY);
}

int TW_IICWriteByte(int SlaveAddr, int ByteAddr, int data, int mode)
{
    unsigned int cpu_sr;
    int addr;
    addr = (SlaveAddr & 0x7f) << 1;
    //printf("%d  ", addr);
    TW_IICStart();
    TW_IICWrite(addr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICWrite(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICWrite(data);
    //printf("%d\n", data);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICStop();
    return TW_IIC_OK;
}
int TW_IICWriteByteS(int SlaveAddr, int ByteAddr, int data, int cnt, int mode)
{
    //
}
int TW_IICReadByte(int SlaveAddr, int ByteAddr, int *data, int mode)
{
    unsigned int cpu_sr;
    int addr;
    //return 1;       //!!!!!!!!!! this function has some error
    addr = (SlaveAddr & 0x7f) << 1;
    TW_IICStart();
    TW_IICWrite(addr);
    printf("%d  ", addr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICWrite(ByteAddr);
    printf("%d  ", ByteAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICStop();
    TW_IICStart();
    addr = addr | 0x1;
    TW_IICWrite(addr);
    printf("%d  ", addr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    *data = TW_IICRead();
    printf("%d  \n", *data);
    TW_IICChkNAck();
    TW_IICStop();
    return TW_IIC_OK;
}
int TW_IICReadByteS(int SlaveAddr, int ByteAddr, int *data, int cnt, int mode)
{
    //
}

int TW_IICWriteByteB(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len)
{
    unsigned int cpu_sr;
       int i,ret;
       static int nTimeoutCnt=0;
       
	for (i=0; i<len; i=i+256) 
      {
		ret =TW_IICWriteByteB256(SlaveAddr, PageAddr, ByteAddr, data, len-i);
             if(ret ==TW_IIC_ERROR)
             {
                 nTimeoutCnt++;
                 if(nTimeoutCnt>=10)
                 {
                     printf("\nTW2835 iic time out 10 times !Need to initial tw2835 again!\n");
                     OSTimeDly(1); 
                     return -1;
                 }
                 else
                 {
                     i=i-256;
                     printf("\n iic rewrite %d %dx%x\n",nTimeoutCnt,PageAddr,ByteAddr);
                 }
             }
             else
                nTimeoutCnt =0;             
	}
}

int TW_IICWriteByteB256(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len)
{
    unsigned int cpu_sr;
    int addr;
    int i;
    addr = (SlaveAddr & 0x7f) << 1;
#ifdef DEBUG_I2C
    printf("BWrite %x  ", addr);
#endif
    TW_IICStart();
    //write slave address
    TW_IICWrite(addr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    //write page address for TW
#ifdef DEBUG_I2C
    printf("%x  ", PageAddr);
#endif
    TW_IICWrite(PageAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    //write index address for TW
#ifdef DEBUG_I2C
    printf("%x  ", ByteAddr);
#endif
    TW_IICWrite(ByteAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    for (i=0; i<len; i++) { 
#ifdef DEBUG_I2C
    	printf("data=%x \n", *(data+i));
#endif
    	TW_IICWrite(*(data+i));
    	if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    	{
        	TW_IICStop();
        	return TW_IIC_ERROR;
    	}
    }
    TW_IICStop();
    return TW_IIC_OK;
}

int TW_IICReadByteB(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len)
{
    unsigned int cpu_sr;
   int i,ret;
       static int nTimeoutCntR=0;
       
	for (i=0; i<len; i=i+256) 
        {
		ret =TW_IICReadByteB256(SlaveAddr, PageAddr, ByteAddr, data, len-i);
             if(ret ==TW_IIC_ERROR)
             {
                 nTimeoutCntR++;
                 if(nTimeoutCntR>=10)
                 {
                     printf("\nTW2835 iic time out 10 times !Need to initial tw2835 again!\n");
                     OSTimeDly(1); 
                     return -1;
                 }
                 else
                 {
                     i=i-256;
                     printf("\n iic re-READ %d %dx%x\n",nTimeoutCntR,PageAddr,ByteAddr);
                 }
             }
             else
                nTimeoutCntR =0;
	}

}

int TW_IICReadByteB256(int SlaveAddr, int PageAddr, int ByteAddr, U8 *data, int len)
{
    unsigned int cpu_sr;
    int addr;
    int i;
    addr = (SlaveAddr & 0x7f) << 1;
#ifdef DEBUG_I2C
    printf("Bread %x  ", addr);
#endif
    TW_IICStart();
    TW_IICWrite(addr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
#ifdef DEBUG_I2C
    printf("%x  ", PageAddr);
#endif
    TW_IICWrite(PageAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
#ifdef DEBUG_I2C
    printf("%x  ", ByteAddr);
#endif
    TW_IICWrite(ByteAddr);
    if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
    {
        TW_IICStop();
        return TW_IIC_ERROR;
    }
    TW_IICStop();
    TW_IICStart();
    addr = addr | 0x1;
#ifdef DEBUG_I2C
    printf("%x  ", addr);
#endif
    TW_IICWrite(addr);
    for (i=0; i<len; i++) { 
       if(TW_IICChkAck() != TW_IIC_ACK_SUCCESS)
       {
           TW_IICStop();
           return TW_IIC_ERROR;
       }
#ifdef DEBUG_I2C
    	printf("data=%x  \n", *(data+i));
#endif
    	*(data+i) = TW_IICRead();
    }
    TW_IICChkNAck();
    TW_IICStop();
    return TW_IIC_OK;
}

