#ifndef W681388_H
#define W681388_H

#include "audio_cfg.h"
#ifdef INCLUDE_WINBOND_SLIC
#include "w681388reg.h"

struct tToneParam
{
    int F;      //Frequency
    int A;      //Amplitude
    int Ton;    //On time
    int Toff;   //Off time
    int osc;    //Oscillator number
};

#define OSCILLATOR_1    1
#define OSCILLATOR_2    2

int w681388Reset(int channel);
int w681388SetCodeMode(int CodeMode, int channel);
int SlicSetTimeSlot(int TsTx, int TsRx, int channel);
int w681388SetFbclk(int Fbclk, int channel);
char w681388GetChipVersion(int channel);
int w681388GetFbclk(int channel);
void w681388PcmIfReset(int channel);
void w681388PcmIfCtl(int en, int channel);
void w681388HpFilterCtl(char HprCtl, char HptCtl, int channel);
unsigned int w681388GetInterruptId(int channel);
void w681388InterruptCtl(int intr, int en, int channel);
int w681388AdjLineState(char LineState, int channel);
int abs1(int oData);
void DelayLoop();
void delay10us(int cnt);
void w681388Calibrate(int channel);
int w681388Initialize(int channel);
int w681388BustWrite(unsigned char *data, int cnt, unsigned char addr, int channel);
int w681388BustRead(unsigned char *data, int cnt, int addr, int channel);
int w681388Write(unsigned char data, unsigned char addr, int channel);
unsigned char w681388Read(unsigned char addr, int channel);
int CalculateToneFReg(int frequence);
int CalculateToneAReg(int frequence, float Apeak);
void compare_init(void);
void TwoRiscSetReg(int msg,int channelid);


char readDirectReg(unsigned char cwaddress,unsigned int channel);
unsigned char  spiToByte(unsigned int cs);
void byteToSpi (unsigned char byte,unsigned int cs);

#endif
#endif

