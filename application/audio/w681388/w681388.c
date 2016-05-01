#include "audio_cfg.h"
#ifdef INCLUDE_WINBOND_SLIC
#include "w681388.h"
#include "w681388Init.h"
#include "w681388InitRev.h"
#include <sif.h>
#include <comm.h>

#define SLIC_MAX_CHANNEL    8
unsigned int SlicCs[SLIC_MAX_CHANNEL] = {
    0x01,   // channel 0
    0x06,   // channel 1
    0x00,   // channel 2
    0x00,   // channel 3
    0x00,   // channel 4
    0x00,   // channle 5
    0x00,   // channle 6
    0x00,   // channle 7
};

void HwSifSeting(void)
{
	*(volatile unsigned int*)0xa8010034 |= (0x1 << 3); 		//enable sif
	*(volatile unsigned int*)0xa8010038 |= (0x1 << 3); 		//reset sif
	 *(volatile unsigned int*)0xa8010038 = 0x0;
	*(volatile unsigned int *)0xa8010010 |= (0x1 << 3);
    
	rRISC_GPIO_DIR &= ~(1 << 10);      //gpio10 is output.	0xfffffbff
	rRISC_GPIO &= ~(1 << 10);          //write 0 to gpio10	
	delay10us(10000);

	rRISC_GPIO_DIR &= ~(1 << 10);    //gpio10 is output.
	rRISC_GPIO |= (1 << 10);         //write 1 to gpio10
	delay10us(10000);

}

int w681388Reset(int channel)
{
	unsigned char temp;
	int cnt;
	
	temp = 0;
	
	while(!(temp & 0x1))
	{
		delay10us(1000);
        	temp = w681388Read(W681388REG_PLLS, channel);
		//printf("PLLS = 0x%x",w681388Read(W681388REG_PLLS,channel));
       	 if(cnt++ > 5)
            	return 0;
    	}
    	return 1;
}
//#define SIMULATION
int w681388SetCodeMode(int CodeMode, int channel)
{
    unsigned char temp;
    temp = w681388Read(W681388REG_PCMC, channel);
    temp |= (CodeMode & (0x3 << 6));
    w681388Write(temp, W681388REG_PCMC, channel);
    return 0;
}

int SlicSetTimeSlot(int TsTx, int TsRx, int channel)
{

    int temp;
    if((TsTx > 0x3ff) || (TsRx > 0x3ff))
    {
        //return err
    }
    
    w681388Write(TsTx & 0xff, W681388REG_TTL, channel);
    w681388Write(TsRx & 0xff, W681388REG_RTL, channel);
    
    temp = ((TsTx & 0x300) >> 8) | ((TsRx & 0x300) >> 6);
    w681388Write(temp, W681388REG_TCH, channel);
	//printf("tx = 0x%x,rx = 0x%x,channel = %d\n",TsTx,TsRx,channel);
	//DumpReg(channel);
    return 0;
}

int w681388GetFbclk(int channel)
{
    unsigned char bcfs;
    int frequency;
    bcfs = w681388Read(W681388REG_PLLS, channel);
    if(bcfs < 0x0c)
    {
        frequency = 256 * (bcfs + 1);
        return frequency;
    }
    return 0;
}

int w681388SetFbclk(int Fbclk, int channel)
{
    unsigned char temp;
    temp = w681388Read(W681388REG_PCMFS, channel);
    switch(Fbclk)
    {
        case 256:
            temp |= PCMFS_BCLK256k;
            break;
        case 512:
            temp |= PCMFS_BCLK512k;
            break;
        case 768:
            temp |= PCMFS_BCLK768k;
            break;
        case 1000:
            temp |= PCMFS_BCLK1000k;
            break;
        case 1024:
            temp |= PCMFS_BCLK1024k;
            break;
        case 1536:
            temp |= PCMFS_BCLK1536k;
            break;
        case 2000:
            temp |= PCMFS_BCLK2000k;
            break;
        case 2048:
            temp |= PCMFS_BCLK2048k;
            break;
        case 4000:
            temp |= PCMFS_BCLK4000k;
            break;
        case 4096:
            temp |= PCMFS_BCLK4096k;
            break;
        case 8000:
            temp |= PCMFS_BCLK8000k;
            break;
        case 8192:
            temp |= PCMFS_BCLK8192k;
            break;
        default:
            // to do: return err;
            break;
    }
    w681388Write(temp, W681388REG_PCMFS, channel);
    return 0;
}


char w681388GetChipVersion(int channel)
{
    //return readDirectReg(W681388REG_DVID, channel);
    return w681388Read(W681388REG_DVID, channel);
}


void w681388PcmIfReset(int channel)
{
    unsigned char temp;
    temp = w681388Read(W681388REG_PCMFS, channel);
    temp |= 0x1;
    w681388Write(temp, W681388REG_PCMFS, channel);
}

void w681388PcmIfCtl(int en, int channel)
{
    unsigned char temp;
    temp = w681388Read(W681388REG_PCMC, channel);
    if(en)
        temp |= 0x1;
    else
        temp &= ~0x1;
    w681388Write(temp, W681388REG_PCMC, channel);
}


void w681388HpFilterCtl(char HprCtl, char HptCtl, int channel)
{
    unsigned char temp;
    temp = w681388Read(W681388REG_PHF, channel);
    if(HprCtl)
        temp |= PHF_HPR;
    else
        temp &= ~PHF_HPR;
    if(HptCtl)
        temp |= PHF_HPT;
    else
        temp &= ~PHF_HPT;
    w681388Write(temp, W681388REG_PHF, channel);
}

unsigned int w681388GetInterruptId(int channel)
{
    unsigned char vector;
    unsigned int IsrId;
    unsigned temp;
    vector = GET_INTV(channel);
    
    IsrId = 0;
    
    if(vector | INTV_IR1)
    {
        temp = GET_INT1(channel);
        IsrId |= (temp & 0xff);
        CLR_INT1(temp, channel);
    }
    
    if(vector | INTV_IR2)
    {
        temp = GET_INT2(channel);
        IsrId |= (temp & 0xff) << 8;
        CLR_INT2(temp, channel);
    }
    
    if(vector | INTV_IR3)
    {
        temp = GET_INT3(channel);
        IsrId |= (temp & 0xff) << 16;
        CLR_INT3(temp, channel);
    }
    return IsrId;
}
    
void w681388InterruptCtl(int intr, int en, int channel)
{
    unsigned char id;
    unsigned char temp;
    
    if(intr & 0xff)
    {
        id = intr & 0xff;
        temp = w681388Read(W681388REG_IE1, channel);
        if(en)
            temp |= id;
        else
            temp &= ~id;
        w681388Write(temp, W681388REG_IE1, channel);
    }
    
    if((intr & (0xff << 16)))
    {
        id = (intr & 0xff) >> 16;
        temp = w681388Read(W681388REG_IE2, channel);
        if(en)
            temp |= id;
        else
            temp &= ~id;
        w681388Write(temp, W681388REG_IE2, channel);
    }
    if(intr & (0xff << 8))
    {
        id = (intr & 0xff) >> 8;
        temp = w681388Read(W681388REG_IE3, channel);
        if(en)
            temp |= id;
        else
            temp &= ~id;
        w681388Write(temp, W681388REG_IE3, channel);
    }
}

int w681388AdjLineState(char LineState, int channel)
{
    char CurrentLineState;
    //printf("Adjust line state!......\n");
    SET_LS_STATUS(LineState, channel);          //set line state open
    delay10us(200);

    //now check
    CurrentLineState = GET_LS_REALTIME_STATUS(channel);     //read current line state
    
    //need to add error breakout
    if(CurrentLineState != LineState)           //if not open try one more time, need set to open again
    {   
        SET_LS_STATUS(LineState, channel);      //set line state open
        delay10us(200);
        CurrentLineState = GET_LS_REALTIME_STATUS(channel);
        if(CurrentLineState != LineState)
        {
            printf("Failed!\n");
            return 0;
        }
    }
    return 1;
}



void w681388GenerateTones(struct tToneParam *param, int channel)
{
    unsigned char OscCntl;
    
    OscCntl = w681388Read(W681388REG_OSNC, channel);             //Get Oscillator Control
    
    if(param->osc == OSCILLATOR_1)
    {
        OscCntl |= 0x01;                            //Setup Oscillator Control
        
        SET_OS1_AMPLITUDE(param->A, channel);       //Set Amplitude
        SET_OS1_FREQUENCY(param->F, channel);       //Set Frequency
        SET_OS1_ON_TIME(param->Ton, channel);       //Set Active timer (On Time)
        SET_OS1_OFF_TIME(param->Toff, channel);     //Set Inactive timer  (Off Time)
    }
    
    if(param->osc == OSCILLATOR_2)
    {
        OscCntl |= 0x02;                            //Setup Oscillator Control
        
        SET_OS1_AMPLITUDE(param->A, channel);       //Set Amplitude
        SET_OS1_FREQUENCY(param->F, channel);       //Set Frequency
        SET_OS1_ON_TIME(param->Ton, channel);       //Set Active timer (On Time)
        SET_OS1_OFF_TIME(param->Toff, channel);     //Set Inactive timer  (Off Time)
    }

    w681388Write(OscCntl, W681388REG_OSNC, channel);                            //Update Oscillator Control
}

int abs(int oData)
{
    if(oData >= 0)
        return oData;
    else
        return -oData;
}

void DelayLoop()
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

void delay10us(int cnt)      //delay cnt * 10us
{
    int i;
    for(i=0; i<200*cnt; i++)
        DelayLoop();
}

void w681388Calibrate(int channel)
{
    unsigned char UCAcc1, UCAcc2, vbg, vbg1, temp, min, minval;
    unsigned int looper;
   // printf("start Calibrate!\n");
    w681388Write(0x77, W681388REG_CAL1, channel);        //setup defaults
    w681388Write(0x99, W681388REG_CAL2, channel);        //setup defaults
    w681388Write(0x09, W681388REG_CAL3, channel);        //setup defaults
    w681388Write(0x00, W681388REG_TROFFS1, channel);     //setup defaults
    w681388Write(0x00, W681388REG_TROFFS2, channel);     //setup defaults
    w681388Write(0x00, W681388REG_TROFFS3, channel);     //setup defaults
    //w681388Write(0x13, W681388REG_ETC, channel);       //setup defaults
    w681388Write(0x30, W681388REG_IM2, channel);         //setup defaults

    if(!w681388AdjLineState(LS_OPEN, channel))
        return;

	min = 0;
    minval = 0xff;
    w681388Write(0x08, W681388REG_CC, channel);                        //enable bandgap cal
    for(looper = 0; looper < 8; looper++)
    {
        w681388Write((unsigned char)(0x08 | looper), W681388REG_CC, channel);
        
        delay10us(3000);           //some delay
        
        temp = w681388Read(W681388REG_TEMP, channel);        //read temp sense
        vbg = w681388Read(W681388REG_VBGAP, channel);        //read VBG
        
        if(abs(temp - vbg) < minval)
        {
            minval = abs(temp - vbg);
            min = looper;
        }
        if (temp == vbg)
            break;
    }
    //printf("temperature = %d; Bandgap Voltage Trim = %d\n", (int)(temp - 80), vbg);
    w681388Write(min & ~0x08, W681388REG_CC, channel);                //course trim is set
    
    //now set to reverse on-hook trans for CAL1
    //printf("reverse on-hook trans for CAL1\n");
    UCAcc2 = w681388Read(W681388REG_OHV, channel);   //save polarity
    if(UCAcc2 & OVH_SIGN_MASK)
    {
       // printf("positive calibration!\n");
        w681388Write(0x0, W681388REG_ETC, channel);                    //bypass some filters to speed it up
        
        if (!w681388AdjLineState(LS_RV_OH_TX, channel))
            return;
        
        delay10us(20000);
        
        w681388Write(ETC_CBY, W681388REG_ETC, channel);
    
        vbg = w681388Read(W681388REG_CAL1, channel);
        vbg1 = vbg &0x0f;
        
        UCAcc1 = w681388Read(W681388REG_QT3I, channel);
        UCAcc2 = w681388Read(W681388REG_QT1I, channel);
        //printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
        //check to see if Q3 already greater than Q1
        if(UCAcc1 > UCAcc2)
        {
            w681388Write((vbg & 0xf0) | 0x04, W681388REG_CAL1, channel);        //set CAL1 ch1 to 0
            w681388Write((vbg & 0x0f) | (6 << 4), W681388REG_CAL2, channel);        //set CAL4 ch1 to 2
            UCAcc1  = w681388Read(W681388REG_CAL3, channel);
            w681388Write((UCAcc1 & 0xf0) | 6, W681388REG_CAL3, channel);        //set CAL5 ch1 to 2
            vbg1 = 0;
            
            delay10us(3000);
            
            UCAcc1 = w681388Read(W681388REG_QT3I, channel);
            UCAcc2 = w681388Read(W681388REG_QT1I, channel);
           // printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
        }
    
        while ((UCAcc2>=UCAcc1)&(vbg1 != 0x0d))         //go until QR3 > QR1
        {
            vbg1 += 1;
            w681388Write((vbg & 0xf0) | vbg1, W681388REG_CAL1, channel);
            w681388Write((vbg & 0x0f) | ((vbg1 + 2) <<4), W681388REG_CAL2, channel);
            
            delay10us(20000);
            
            UCAcc1 = w681388Read(W681388REG_QT3I, channel);
            UCAcc2 = w681388Read(W681388REG_QT1I, channel);
            //printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
            vbg = w681388Read(W681388REG_CAL1, channel);
            vbg1 = vbg &0x0f;
        }
        //now set rve trim
        vbg = w681388Read(W681388REG_CAL3, channel);
        vbg1 = vbg &0x0f;
        UCAcc1 = w681388Read(W681388REG_QT3I, channel);
        UCAcc2 = w681388Read(W681388REG_QT1I, channel);
       // printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
        //check to see if Q3 already greater than Q1
        if (UCAcc1 > UCAcc2)
        {
            w681388Write((vbg & 0xf0) | 0x06, W681388REG_CAL3, channel);        //set CAL1 ch2 to 0
            //set CAL5 ch2 to 2
            vbg1 = 6;
            
            delay10us(3000);
            
            UCAcc1 = w681388Read(W681388REG_QT3I, channel);
            UCAcc2 = w681388Read(W681388REG_QT1I, channel);
           // printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
        }
        
        while ((UCAcc2>=UCAcc1)&(vbg1 != 0x0f))                     //go until QR3 > QR1
        {
            vbg1 += 1;
            w681388Write((vbg & 0xf0) | vbg1, W681388REG_CAL3, channel);
            
            delay10us(20000);
            
            UCAcc1 = w681388Read(W681388REG_QR3I, channel);
            UCAcc2 = w681388Read(W681388REG_QR1I, channel);
           // printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
            vbg = w681388Read(W681388REG_CAL3, channel);
            vbg1 = vbg &0x0f;
        }
    }
    else
    {
        //printf("negative calibration!\n");
        //now set to forward on-hook trans for CAL1    
        if(!w681388AdjLineState(LS_FW_OH_TX, channel))
            return;
        
        delay10us(20000);   //wait ~200mS           
        
        w681388Write(ETC_CBY, W681388REG_ETC, channel);  //bypass some filters to speed it up
        
        //now in forward on-hook transmision state
        //first set CAL1 & CAL5 VBAT trim
        vbg = w681388Read(W681388REG_CAL1, channel);
        vbg1 = vbg & 0x0f;
        
        delay10us(20000);
        
        UCAcc1 = w681388Read(W681388REG_QR3I, channel);
        UCAcc2 = w681388Read(W681388REG_QR1I, channel);
        //printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
        
        //check to see if Q3 already greater than Q1
        if(UCAcc1 > UCAcc2)
        {
            w681388Write((vbg & 0xf0) | 0x04, W681388REG_CAL1, channel);        //set VBAT1 Trim to 4
            w681388Write((vbg & 0xf0) | 0x06, W681388REG_CAL3, channel);        //set RVE1 Trim to 6
            
            UCAcc1  = w681388Read(W681388REG_CAL2, channel);
            w681388Write((UCAcc1 & 0x0f) | (6 << 4), W681388REG_CAL2, channel);        //set TVTE1 Trim to 6
            vbg1 = 0;
            
            delay10us(3000);
            
            UCAcc1 = w681388Read(W681388REG_QR3I, channel);
            UCAcc2 = w681388Read(W681388REG_QR1I, channel);
           // printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
        }
    
        while((UCAcc2 >= UCAcc1) && (vbg1 != 0x0d))     //go until QR3 > QR1
        {
            vbg1 += 1;
            w681388Write((vbg & 0xf0) | vbg1, W681388REG_CAL1, channel);
            w681388Write((vbg & 0xf0) | (vbg1 + 2), W681388REG_CAL3, channel);
            
            delay10us(20000);
            
            UCAcc1 = w681388Read(W681388REG_QR3I, channel);
            UCAcc2 = w681388Read(W681388REG_QR1I, channel);
          //  printf("QR3I=%d ?> QR1I=%d\n", UCAcc1, UCAcc2);
            vbg = w681388Read(W681388REG_CAL1, channel);
            vbg1 = vbg & 0x0f;
        }
        
        vbg = w681388Read(W681388REG_CAL2, channel);
        vbg1 = vbg>>4;
        UCAcc1 = w681388Read(W681388REG_QT3I, channel);
        UCAcc2 = w681388Read(W681388REG_QT1I, channel);
       // printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
        //check to see if Q3 already greater than Q1
        if(UCAcc1 > UCAcc2)
        {
            w681388Write((vbg & 0x0f) | (6 << 4), W681388REG_CAL2, channel);        //set CAL1 ch2 to 0
            UCAcc1 = w681388Read(W681388REG_CAL2, channel);
            vbg1 = 6;
            
            delay10us(3000);
            
            UCAcc1 = w681388Read(W681388REG_QT3I, channel);
            UCAcc2 = w681388Read(W681388REG_QT1I, channel);
          //  printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
        }
        
        while((UCAcc2 >= UCAcc1) && (vbg1 != 0x0f))          //go until QR3 > QR1
        {
            vbg1 += 1;
            w681388Write((vbg & 0x0f) | (vbg1 << 4), W681388REG_CAL1, channel);
            
            delay10us(20000);
            
            UCAcc1 = w681388Read(W681388REG_QT3I, channel);
            UCAcc2 = w681388Read(W681388REG_QT1I, channel);
           // printf("QT3I=%d ?> QT1I=%d\n", UCAcc1, UCAcc2);
            vbg = w681388Read(W681388REG_CAL1, channel);
            vbg1 = vbg>>4;
        }
        //we be finished
    }

    //set back to open
    if (!w681388AdjLineState(LS_OPEN, channel))
        return;
    w681388Write(0x00, W681388REG_ETC, channel);

}

int w681388Initialize(int channel)
{
    int i;
   // printf("chip initialize......\n");
    for(i = 0; i < sizeof(w681388RegInit); i++)
        w681388Write(w681388RegInit[i], i, channel);
   // printf("Done!\n");
    return 0;
}

int w681388BustWrite(unsigned char *data, int cnt, unsigned char addr, int channel)
{
#ifdef SIMULATION
    int i;
    for(i = 0; i < cnt; i++)
    {
        printf("w A=%x; D=%x; C=%d\n", addr + i, (int)*(data + i), channel);
        register_men[addr + i] = *(data + i);
    }
#else
    unsigned int cs;
    int i;
    
    cs = SlicCs[channel];
    
    RiscSemPend(5);             //waite bus free
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    //set spi bus timing
    rSIF_TIMING = 0xa1000700;
    //write address and command to chip
    rSIF_DAT = addr & 0xff;                 //write command: 0x02
    rSIF_CTL = 0x02;
    rSIF_MODE = 0xa0200001 | ((cs & 0x7) << 26) | (0x1 << 24);      //start to tx!  (cmd + addr = 16bit)

    //wtite data to chip
    for(i = 0; i < cnt; i++)
    {
        while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
        rSIF_DAT = *(data + i) & 0xff;
        rSIF_MODE = 0xa0200001 | ((cs & 0x7) << 26);      //data is 8bit
    }
    
    RiscSemUnLock(5);
#endif
    return 0;
}

int W681388BustRead(unsigned char *data, int cnt, int addr, int channel)
{
    unsigned int cs;
    
    cs = SlicCs[channel];
    
    return 0;
}

int w681388Write(unsigned char data, unsigned char addr, int channel)
{
#ifdef SIMULATION
    printf("w A=%x; D=%x; C=%d\n", addr, data, channel);
    register_men[addr] = data;
#else
    unsigned int cs;
    
    cs = SlicCs[channel];
    
    RiscSemPend(5);             //waite bus free
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    
    //set spi bus timing
    rSIF_TIMING = 0xa1000700;
    
    //write address and command to chip
    rSIF_DAT = addr & 0xff;                 //write command : 0x00
    rSIF_CTL = 0x0;
    
    rSIF_MODE = 0xa0200001 | ((cs & 0x7) << 26) | (0x1 << 24);      //start to tx!  (cmd + addr = 16bit)

    //wtite data to chip
    while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.
    rSIF_DAT = data & 0xff;
    rSIF_MODE = 0xa0200001 | ((cs & 0x7) << 26);      //data is 8bit
    
    RiscSemUnLock(5);
#endif
    return 0;
}

unsigned char w681388Read(unsigned char addr, int channel)
{
#ifdef SIMULATION
    int temp;
    unsigned char data;
    printf("r A=%x; C=%d\n", addr, channel);
    printf("D=");
    scanf("%d", &temp);
    if(temp == 256)
        data = register_men[addr];
    else
        data = (char)(temp & 0xff);
    printf("rd = %d\n", data);
#else
    unsigned int cs;
    unsigned char data;
    unsigned temp;
    
    cs = SlicCs[channel];
    RiscSemPend(5);
    while(SifBusyRead());
    
    //set spi bus timing
    rSIF_TIMING = 0xa1000700;
    //write address and command to chip
    rSIF_DAT = (addr & 0xff);
    rSIF_CTL = 0x80;                                    //read command : 0x80
    temp = 0xa0200001 | ((cs & 0x7) << 26) | (0x1 << 24);    //start to tx!   cmd addr
    rSIF_MODE = temp;
    //printf("rSIF_MODE = 0x%8.8x\n", temp);
    while(SifBusyRead());
    rSIF_MODE = 0xa0000001 | ((cs & 0x7) << 26);                //start to read!
    while(SifBusyRead());
    data  = rSIF_DAT;                                   //read data
    
    RiscSemUnLock(5);
#endif
    return data;
}

void TwoRiscSetReg(int msg,int channelid)
{	
	typedef union
	{
		int message;
		struct 
		{
			char flag;
			char index;
			short value;
		}info;
	}tMsg;

	tMsg mm;

	mm.message = msg;

	
	printf("Channel %d :Set Reg[%d] to 0x%x\n",channelid,mm.info.index,mm.info.value);
	//if(mm.info.flag)
	//	writeIndirectReg(mm.info.index, mm.info.value, channelid);
	//else
	//	writeDirectReg(mm.info.index, (char)mm.info.value, channelid);
	w681388Write((char)mm.info.value, mm.info.index, channelid);
	printf("The Valud you Set is 0x%x\n",w681388Read(mm.info.index,channelid));
	
}

void compare_init(void)
{
    int size1;
    int size2;
    int min;
    int cnt;
    size1 = sizeof(w681388RegInit);
    size2 = sizeof(w681388InitRev);
    min = (size1 < size2) ? size1 : size2;
    printf("min of %d, %d is %d\n", size1, size2, min);
    for(cnt = 0; cnt <= min; cnt++)
    {
        if(w681388RegInit[cnt] != w681388InitRev[cnt])
            printf("0x%2.2x : 0x%2.2x; but rev = 0x%2.2x!\n", cnt, w681388RegInit[cnt], w681388InitRev[cnt]);
    }
}

#ifdef SIMULATION
#include <math.h>
int CalculateToneFReg(int frequence)
{
    //OmC [15 : 0] = cos(2¦Ðfn / 8000Hz) x 32768
    float temp;
    temp = 6.28318 * frequence;
    temp = temp / 8000;
    temp = cos(temp);
    temp = temp * 32768;
    printf("fn = %d : OmC = 0x%4.4x (%f)\n", frequence, (int)temp, temp);
    return (int)temp;
}

int CalculateToneAReg(int frequence, float Apeak)
{
    //OmIC [15 : 0] = A ¡Á sin(2¦Ðfn / 8000Hz) x 32768
    //A = APEAK / 1.5779
    //APEAK cannot exceed 1.2 VPK
    float temp;
    float A;
    A = Apeak/1.5779;
    temp = 6.28318 * frequence;
    temp = temp/8000;
    temp = A * sin(temp);
    temp = temp * 32768;
    printf("fn = %d, Apeak = %f : OmIC = 0x%4.4x (%f)\n", frequence, Apeak, (int)temp, temp);
    return (int)temp;    
}
#else
int CalculateToneRegs(int frequence)
{
    return -1;
}
int CalculateToneAReg(int frequence, float Apeak)
{
    return -1;
}
#endif

#endif

