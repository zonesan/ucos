#ifndef SLIC_TEST
#define	SLIC_TEST

#include "audio_cfg.h"
#ifdef INCLUDE_WINBOND_SLIC
// Structs
#include "def_type.h"

typedef struct {

	unsigned short  SlicOSC1_FREQ;
	unsigned short  SlicOSC1_AMP;
	unsigned short  SlicOSC1_OnTime;
	unsigned short  SlicOSC1_OffTime;

	unsigned short  SlicOSC2_FREQ;
	unsigned short  SlicOSC2_AMP;
	unsigned short  SlicOSC2_OnTime;
	unsigned short  SlicOSC2_OffTime;

}structCountryTones;

typedef struct {

	unsigned short  SlicOSC2_FREQ;
	unsigned short  SlicOSC2_AMP;
	unsigned short  SlicOSC2_OnTime;
	unsigned short  SlicOSC2_OffTime;

}structRingTones;

//International Ring Parameters
///
#define	INIT_OS2AT_USA       0X0640	//	Initialization Ringing Osc. Active Timer Low Byte (2 s,125 탎/LSB)
#define	INIT_OS2IT_USA   	 0X0C80	//	Initialization Ringing Osc. Inactive Timer High Byte (4 s, 125 탎/LSB)


#define	INIT_OS2AT_JAPAN     0X0320	//	Initialization Ringing Osc. Active Timer High Byte (1 s,125 탎/LSB)
#define	INIT_OS2IT_JAPAN     0X0640	//	Initialization Ringing Osc. Inactive Timer High Byte (2 s, 125 탎/LSB)

#define	INIT_OS2AT_KOREA     0X0320	//	Initialization Ringing Osc. Active Timer High Byte (1 s,125 탎/LSB)
#define	INIT_OS2IT_KOREA     0X0640	//	Initialization Ringing Osc. Inactive Timer High Byte (2 s, 125 탎/LSB)

#define	INIT_OS2AT_CHINA     0X0320	//	Initialization Ringing Osc. Active Timer High Byte (1 s,125 탎/LSB)SB)
#define	INIT_OS2IT_CHINA     0X0C80	//	Initialization Ringing Osc. Inactive Timer High Byte (4 s, 125 탎/LSB)


#define	INIT_OS2AT_FRANCE    0X04B0	//	Initialization Ringing Osc. Active Timer High Byte (1.5 s,125 탎/LSB)
#define	INIT_OS2IT_FRANCE    0X0AF0	//	Initialization Ringing Osc. Inactive Timer High Byte (3.5 s, 125 탎/LSB)


#define	INIT_OS2AT_ITALY     0X0320
#define	INIT_OS2IT_ITALY     0X0640

#define	INIT_OS2AT_GERMANY   0X0320
#define	INIT_OS2IT_GERMANY   0X0640


#define	INIT_OS2AT_UK    0X0320	//	Initialization Ringing Osc. Active Timer Low Byte (1 s,125 탎/LSB)
#define	INIT_OS2IT_UK    0X0C80	//	Initialization Ringing Osc. Inactive Timer Low Byte (4 s, 125 탎/LSB)


#define	INIT_OS2C_USA		0x7E6C	// Ringing Oscillator Frequency (20 Hz)	
#define	INIT_OS2C_JAPAN	    0x7EFD  // Ringing Oscillator Frequency (16 Hz)
#define	INIT_OS2C_KOREA	    0x7E6C	// Ringing Oscillator Frequency (20 Hz)	
#define	INIT_OS2C_CHINA		0x7D8A  // Ringing Oscillator Frequency (25 Hz)
#define	INIT_OS2C_FRANCE	0x7641  // Ringing Oscillator Frequency (50 Hz)
#define	INIT_OS2C_ITALY	    0x7D8A  // Ringing Oscillator Frequency (25 Hz)
#define	INIT_OS2C_GERMANY	0x7D8A
#define	INIT_OS2C_UK	    0x7D8A
#define	INIT_OS2C_UK_CCA    0x7D8A

#define	INIT_OS2IC_USA		0x0E99	//	70 Vpk
#define	INIT_OS2IC_JAPAN	0x0C88	//	75 Vpk
#define	INIT_OS2IC_KOREA	0x0FA4	//	75 Vpk	
#define	INIT_OS2IC_CHINA	0x1381	//	75 Vpk	
#define	INIT_OS2IC_FRANCE   0x28D7	//	80 Vpk
#define	INIT_OS2IC_ITALY    0x0FA2	//	60 Vpk
#define	INIT_OS2IC_GERMANY	0x0FA2	//	60 Vpk
#define	INIT_OS2IC_UK	    0x1235	//	70 Vpk
#define	INIT_OS2IC_UK_CCA   0x1381	//	75 Vpk

struct tPcmMode
{
    // audio code set
    char CodecMode;         // codec mode selection: A-low, u-low, linear
    char BitMode;           // bit mode: 16bit, 8bit
    // pcm if set
    char InternalFsync;     // Frame sync source
    char FsType;            // Frame sync type
    char GciFormat;         // GCI Clock Format: 1 bclk pre data bit, 2 bclk pre data bit
    char TriMode;           // Tri-state PCMT LSB on negative/positive edge of BCLK
    int Fbclk;              // bclk frequency
    // time slot set
    int TxTimeSlot;         // Transmit Timeslot Start
    int RxTimeSlot;         //
};

// codec mode selection
#define PCM_CODEC_ALOW      0
#define PCM_CODEC_ULOW      1
#define PCM_CODEC_LINEAR    2
// bit mode: 16bit, 8bit
#define PCM_CODEC_8BIT      1
#define PCM_CODEC_16BIT     2
// Frame sync source
#define PCM_EXFS            0
#define PCM_INFS            1
// Frame sync type
#define PCM_SHORT_FS        0
#define PCM_LONG_FS         1 
// GCI Clock Format
#define PCM_GCI_1BIT        0
#define PCM_GCI_2BIT        1
// Tri-state PCMT LSB on negative/positive edge of BCLK
#define PCM_TRI_NEGATIVE    1
#define PCM_TRI_POSITIVE    0


#define SLIC_NOERROR    0
#define SLIC_ERROR      1

void ReadWriteTest(void);
void DumpReg(unsigned int channel);
void Slic_Task(void *data);
void slic_test_write(void);
void slic_test_init(unsigned long channelid);
void Calculate(void);
unsigned long interruptBits (unsigned long channelid);
unsigned char interruptChannelData(unsigned long channelid);
VOID SetState(LONG newState,LONG channelid);
void HwSlicStateMachine(unsigned long channelid);
void defered_isr(unsigned long channelid);
void disableOscillators(unsigned long channelid);
void makeConnection( unsigned char c1, unsigned char c2,unsigned long channelid);
void breakConnection( unsigned char c1, unsigned char c2,unsigned long channelid);
unsigned char getLoopStatus(unsigned long channelid);
BYTE GetKey(unsigned long channelid);
WORD GenerateTones (structCountryTones *Tones,unsigned long channelid);
WORD SetupRing (structRingTones  *RingTones,unsigned long channelid);
WORD GenerateMeterPulse(WORD Frequency, WORD Amplitude, WORD OnTime, WORD OffTime,unsigned long channelid);
void goActive(unsigned long channelid);
void goOpen(unsigned long channelid);
void activateRinging(unsigned long channelid);
void stopRinging(unsigned long channelid);
void pulseMeter(unsigned long channelid);
int SlicInit(int channel);
int SlicSetPcmMode(struct tPcmMode *PcmMode, int channel);
void ProcessCallid(char *pointer,int len,int channelid);



#endif
#endif

