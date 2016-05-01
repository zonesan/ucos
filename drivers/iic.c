#include <iic.h>
#include <comm.h>

void IICDelayLoop()
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
void IICDelay(int cnt)
{
    int i;
    for(i=0; i<10*cnt; i++)
        IICDelayLoop();
}

void IICinit(void)
{
    unsigned int cpu_sr;
    
    IIC_SDA_OUT();
    IIC_SDA_SET();
    IIC_SCL_OUT();
    IIC_SCL_SET();
}

void IICStart(void)          /* statr the iic bus */
{
    unsigned int cpu_sr;

    //IIC_SDA_OUT();    /*does this option necessary ?*/
    IIC_SDA_CLR();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY);
}
void IICStop(void)           /* stop the iic bus */
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_CLR();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY);
}
void IICStop2(void)           /* stop the iic bus */
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    //IICDelay(IIC_PUSH_DELAY);
    //IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY);
}
int  IICRead(void)           /* read the iic bus */
{
    unsigned int cpu_sr;

    int i;
    int buf = 0;
    IIC_SDA_OUT();
    IIC_SDA_SET();
    IIC_SDA_IN();
    for(i = 0 ; i < 8 ; i++)
    {
        buf = buf<<1;
        IIC_SCL_SET();
        IICDelay(IIC_PUSH_DELAY);
        if(IIC_SDA_GET())
            buf |= 0x1;
        else
            buf &= ~0x1;
        IIC_SCL_CLR();
        IICDelay(IIC_PUSH_DELAY);
    }
    return buf;
}

void IICWrite(int buf)       /* write the iic bus */
{
    unsigned int cpu_sr;

    int i;
    IIC_SDA_OUT();
    buf &= 0xff;
    for(i = 0; i <= 7; i++)
    {
        if((buf & 0x80))
            IIC_SDA_SET();
        else
            IIC_SDA_CLR();
        IICDelay(IIC_PUSH_DELAY);
        IIC_SCL_SET();
        IICDelay(IIC_PUSH_DELAY);
        IIC_SCL_CLR();
        IICDelay(IIC_PUSH_DELAY);
        buf = buf << 1;
    }
}
int IICChkAck(void)         /* chack the acknowledge signel */
{
    unsigned int cpu_sr;

    int WaitTime;
    IIC_SDA_IN();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY);
    WaitTime = IIC_WAIT_TIME;
    while(IIC_SDA_GET())
    {
        WaitTime--;
        if(WaitTime <= 0)
            break;
    }
    /* to do : read ack signal */
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY);
    /* to do : add err deal */
    if(WaitTime != 0)
        return IIC_ACK_SUCCESS;
    printf("IICChkAck: iic time out!\n");
    //printf("0xa8000080=0x%x, 0xa8000084=0x%x\n", *(volatile int *)0xa8000080, *(volatile int *)0xa8000084);
    return IIC_ACK_ERROR;
}

void IICChkNAck(void)
{
    unsigned int cpu_sr;

    IIC_SDA_IN();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY);
}
void IICSendAck(void)        /* send the acknowledge signel */
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_CLR();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY);
}

void IICSendNAck(void)
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY);
}

int IICWriteByteContinuous(int SlaveAddr, int *Datap, int Byte_number)
{ 
    unsigned int cpu_sr;

    int addr;
    int i;
    addr = (SlaveAddr & 0x7f) << 1;
    IICStart();
    IICWrite(addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }

    IICWrite(*Datap);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }

    for (i=1; i<=Byte_number; i++)
    {
        IICWrite(*(Datap+i));
        if(IICChkAck() != IIC_ACK_SUCCESS)
        {
            IICStop();
            return IIC_ERROR;
        }
    }
    IICStop();
    return IIC_OK;  
}
int IICWriteByte(int SlaveAddr, int ByteAddr, int data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    addr = (SlaveAddr & 0x7f) << 1;
    //printf("%d  ", addr);
    IICStart();
    IICWrite(addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(data);
    //printf("%d\n", data);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICStop();
    printf("0x%x[0x%x] = 0x%x!\n", SlaveAddr, ByteAddr, data);
    return IIC_OK;
}
int IICWriteWord(int SlaveAddr, int ByteAddr, int data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    int data_l;
    int data_m;
    data_l=data;
    data_m=(data&0xff00)>>8;
    addr = (SlaveAddr & 0x7f) << 1;
    //printf("%d  ", addr);
    IICStart();
    IICWrite(addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(data_m);
    //printf("%d\n", data);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(data_l);
    //printf("%d\n", data);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICStop();
    printf("0x%x[0x%x] = 0x%x!\n", SlaveAddr, ByteAddr, data);
    return IIC_OK;
}
int IICWriteByteS(int SlaveAddr, int ByteAddr, int data, int cnt, int mode)
{
    //
}
int IICReadByte(int SlaveAddr, int ByteAddr, int *data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    //return 1;       //!!!!!!!!!! this function has some error
    addr = (SlaveAddr & 0x7f) << 1;
    IICStart();
    IICWrite(addr);
    //printf("%d  ", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICStop();
    IICStart();
    addr = addr | 0x1;
    IICWrite(addr);
    //printf("%d  ", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    *data = IICRead();
    //printf("%d\n", *data);
    IICChkNAck();
    IICStop();
    printf("0x%x[0x%x] == 0x%x!\n", SlaveAddr, ByteAddr, *data);
    return IIC_OK;
}

int IICReadByteADV(int SlaveAddr, int ByteAddr, int *data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    //return 1;       //!!!!!!!!!! this function has some error
    addr = (SlaveAddr & 0x7f) << 1;
    IICStart();
    IICWrite(addr);
    //printf("%d\n", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICWrite(ByteAddr);
    //printf("%d\n", ByteAddr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    IICStop2();
    IICStart();
    addr = addr | 0x1;
    IICWrite(addr);
    //printf("%d\n", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        return IIC_ERROR;
    }
    *data = IICRead();
    //printf("%d\n", *data);
    IICChkNAck();
    IICStop();
    printf("0x%x[0x%x] == 0x%x!\n", SlaveAddr, ByteAddr, *data);
    return IIC_OK;
}
int IICReadWord(int SlaveAddr, int ByteAddr, int *data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    int data_m;
    int data_l;
    //return 1;       //!!!!!!!!!! this function has some error
    addr = (SlaveAddr & 0x7f) << 1;
    IICStart();
    IICWrite(addr);
    //printf("%d  ", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        printf("$");
        return IIC_ERROR;
    }
    IICWrite(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        printf("%%");
        return IIC_ERROR;
    }
    IICStop();
    IICStart();
    addr = addr | 0x1;
    IICWrite(addr);
    //printf("%d  ", addr);
    if(IICChkAck() != IIC_ACK_SUCCESS)
    {
        IICStop();
        printf("^");
        return IIC_ERROR;
    }
    data_m = IICRead();
    //printf("data_m=0x%x\n",data_m);
    IICSendAck();
    data_l = IICRead();
    //printf("data_l=0x%x\n",data_l);
    *data=data_m<<8|data_l&0x00ff;
    //printf("data=0x%x \n", *data);
    IICChkNAck();
    IICStop();
    printf("0x%x[0x%x] == 0x%x!\n", SlaveAddr, ByteAddr, *data);
    return IIC_OK;
}
int IICReadByteS(int SlaveAddr, int ByteAddr, int *data, int cnt, int mode)
{
    //
}


void IICStartTVP(void)          /* statr the iic bus */
{
    unsigned int cpu_sr;

    //IIC_SDA_OUT();    /*does this option necessary ?*/
    IIC_SDA_CLR();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY_TVP);
}

void IICStopTVP(void)           /* stop the iic bus */
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_CLR();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY_TVP);
}

int  IICReadTVP(void)           /* read the iic bus */
{
    unsigned int cpu_sr;

    int i;
    int buf = 0;
    IIC_SDA_OUT();
    IIC_SDA_SET();
    IIC_SDA_IN();

    for(i = 0 ; i < 8 ; i++)
    {
        buf = buf<<1;
        IICDelay(IIC_PUSH_DELAY_TVP);
        IIC_SCL_SET();
        if(IIC_SDA_GET())
            buf |= 0x1;
        else
            buf &= ~0x1;
        IICDelay(IIC_PUSH_DELAY_TVP );
        IIC_SCL_CLR();
    }
    return buf;
}

void IICWriteTVP(int buf)       /* write the iic bus */
{
    unsigned int cpu_sr;

    int i;
    IIC_SDA_OUT();
    buf &= 0xff;
    for(i = 0; i <= 7; i++)
    {
        if((buf & 0x80))
            IIC_SDA_SET();
        else
            IIC_SDA_CLR();
        IICDelay(IIC_PUSH_DELAY_TVP);
        IIC_SCL_SET();
        IICDelay(IIC_PUSH_DELAY_TVP);
        IIC_SCL_CLR();
        IICDelay(IIC_PUSH_DELAY_TVP);
        buf = buf << 1;
    }
}
int IICChkAckTVP(void)         /* chack the acknowledge signel */
{
    unsigned int cpu_sr;

    int WaitTime;
    IIC_SDA_IN();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY_TVP);
    WaitTime = IIC_WAIT_TIME;
    while(IIC_SDA_GET())
    {
        WaitTime--;
        if(WaitTime <= 0)
            break;
    }
    /* to do : read ack signal */
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY_TVP);
    /* to do : add err deal */
    if(WaitTime != 0)
        return IIC_ACK_SUCCESS;
    printf("IICChkAckTVP: iic time out!\n");
    //printf("0xa8000080=0x%x, 0xa8000084=0x%x\n", *(volatile int *)0xa8000080, *(volatile int *)0xa8000084);
    return IIC_ACK_ERROR;
}

void IICSendNAckTVP(void)
{
    unsigned int cpu_sr;

    IIC_SDA_OUT();
    IIC_SDA_SET();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SCL_SET();
    IICDelay(IIC_PUSH_DELAY_TVP);
    IIC_SCL_CLR();
    IICDelay(IIC_PUSH_DELAY_TVP);
}

int IICWriteByteTVP(int SlaveAddr, int ByteAddr, int data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    addr = (SlaveAddr & 0x7f) << 1;
    //printf("%d  ", addr);
    IICStartTVP();
    IICWriteTVP(addr);
    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }
    IICWriteTVP(ByteAddr);
    //printf("%d  ", ByteAddr);
    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }
    IICWriteTVP(data);
    //printf("%d\n", data);
    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }
    IICDelay(40);
    IICStopTVP();
    printf("0x%x[0x%x] = 0x%x!\n", SlaveAddr, ByteAddr, data);
    return IIC_OK;
}

int IICReadByteTVP(int SlaveAddr, int ByteAddr, int *data, int mode)
{
    unsigned int cpu_sr;

    int addr;
    addr = (SlaveAddr & 0x7f) << 1;
    IICStartTVP();
    IICWriteTVP(addr);
    
#if defined(IIC_DEBUG)
    printf("0x%x  ", addr);
#endif

    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }

    IICWriteTVP(ByteAddr);
    
#if defined(IIC_DEBUG)
    printf("0x%x  ", ByteAddr);
#endif

    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }
    IICStopTVP();
    IICDelay(2);
    IICStartTVP();
    addr = addr | 0x1;
    IICWriteTVP(addr);
    
#if defined(IIC_DEBUG)
    printf("0x%x  ", addr);
#endif

    if(IICChkAckTVP() != IIC_ACK_SUCCESS)
    {
        IICStopTVP();
        return IIC_ERROR;
    }

    *data = IICReadTVP();
    
#if defined(IIC_DEBUG)
    printf("0x%x\n", *data);
#endif

    IICSendNAckTVP();
    IICStopTVP();
    printf("0x%x[0x%x] == 0x%x!\n", SlaveAddr, ByteAddr, *data);
    return IIC_OK;
}


