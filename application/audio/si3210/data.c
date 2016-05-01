
//#include "Conio.h"
//#include "stdio.h"

#include "audio_cfg.h"

#ifdef INCLUDE_SI3210

#include "proslic.h"

#if 0
extern unsigned char currentChip=0, numberOfChips=3;

extern unsigned char chipSelectByte[] = 
{
	0, 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xff
};
#endif

/**************************** Tone Definitions *********************************************/

/*

These structures are use to hold the values of the group of registers which control the two oscillators
on the ProSLIC.  These oscillators are implemented with an amplitude parameter called the frequency 
parameter called the x parameter and an amplitude parameter called the y parameter.  Each frequency and
amplitude map to an x and y parameter.  This mapping is not linear.  Also included in this structure
is the value of cadence timer periods.  Each field in this structure contains the value of one ProSLIC
register.

typedef struct {
	unsigned short coeff;
	unsigned short x;
	unsigned short y;
	unsigned char on_hi_byte;
	unsigned char on_low_byte;
	unsigned char off_hi_byte;
	unsigned char off_low_byte;
} Oscillator;




typedef struct {
	Oscillator osc1;
	Oscillator osc2;
} tone_struct;  

typedef struct {
unsigned short frequency;
unsigned short coeff; 
unsigned short x; 
unsigned short y; 
} coeffData;

*/



/****************** COMMON TELEPHONEY TONES*********************************/
tone_struct DialTone = {  /* OSC1= 350 Hz OSC2= 440 Hz .0975 Volts -18 dBm */
	{0x7b30,0x0063,0,0,0,0,0},{0x7870,0x007d,0,0,0,0,0}
};
tone_struct ReorderTone = {	/* OSC1= 480 Hz OSC2 = 620 .0975 Volts -18 dBm */
	{0x7700,0x0089,0,0x09,0x60,0x06,0x40},{0x7120,0x00b2,0,0x09,0x60,0x06,0x40}
};
tone_struct CongestionTone = { /* OSC1= 480 Hz OSC2 = 620 .0975 Volts -18 dBM */
	{0x7700,0x0089,0,0x06,0x40,0x09,0x60},{0x7120,0x00b2,0,0x06,0x40,0x09,0x60}
};
tone_struct RingbackPBX = {	/* OSC1 = 440 Hz OSC2= 480 .0975 Volts -18 dBM */
	{0x7870,0x007d,0,0x1f,0x40,0x5d,0xc0},{0x7700,0x0089,0,0x1f,0x40,0x5d,0xc0}
};
tone_struct RingbackNormal = { /* OSC1 = 440 Hz OSC2 = 480 .0975 Volts -18 dBm */
	{0x7870,0x007d,0,0x3e,0x80,0x7d,0x00},{0x7700,0x0089,0,0x3e,0x80,0x7d,0x00}
};
tone_struct BusySignal = { /* OSC1= 480  OSC2 = 620 .0975 Voltz -18 dBm 8*/
	{0x7700,0x0089,0,0x0f,0xa0,0x0f,0xa0},{0x7120,0x00b2,0,0x0f,0xa0,0x0f,0xa0}
};

tone_struct RingbackJapan = { /* OSC1 = 400 Hz OSC2 = 435 .0975 Volts -18 dBm */
	{0x79c0,0x00e9,0,0x1f,0x40,0x3e,0x80},{0x7940,0x00f2,0,0x1f,0x40,0x3e,0x80}

};


tone_struct BusyJapan = { /* OSC1 = 400 Hz OSC2 = 435 .0975 Volts -18 dBm */
	{0x79c0,0x00e9,0,0x0f,0xa0,0x0f,0xa0},{0,0,0,0,0,0,0}

};

tone_struct JapanDialTone = { /* OSC1 = 400 Hz OSC2 = 435 .0975 Volts -18 dBm */
	{0x79c0,0x00e9,0,0,0,0,0},{0,0,0,0,0,0,0}

};
/*


Microsoft(R) Windows 98
   (C)Copyright Microsoft Corp 1981-1999.

C:\>tone 400 .0975

INFO: Freq = 400.000000    for n=1 or n=2
 OSCn = 0x79c0, OSCnX = 0x0071, OSCnY = 0x0000

C:\>tone 420 .0975

INFO: Freq = 420.000000    for n=1 or n=2
 OSCn = 0x7920, OSCnX = 0x0077, OSCnY = 0x0000

C:\>tone 415 .0975

INFO: Freq = 415.000000    for n=1 or n=2
 OSCn = 0x7940, OSCnX = 0x0076, OSCnY = 0x0000

C:\>tone 415 .2

INFO: Freq = 415.000000    for n=1 or n=2
 OSCn = 0x7940, OSCnX = 0x00f2, OSCnY = 0x0000

C:\>tone 400 .2

INFO: Freq = 400.000000    for n=1 or n=2
 OSCn = 0x79c0, OSCnX = 0x00e9, OSCnY = 0x0000

C:\>


  */


int Japan=0; // Used as a flag for to give japan tones out.

struct  {

	int next;
	ringStruct ringRegs;
}  ringCadenceTables [] [5] =



//{	20,	"RING_OSC",			0x7EF0	},
//{	21,	"RING_X",			0x0160	},
//{	22,	"RING_Y",			0x0000	},


{


{
	
	{0, {0x7EF0,0x160,0, 1000*8,2000*8}},//1F40=8000 on,3E80=16000 off
	
	
},

{
	{1, {0x7EF0,0x160,0,   200*8,400*8}},//1F40=8000 on,3E80=16000 off
	{0, {0x7EF0,0x160,0,   400*8,1000*8}},//1F40=8000 on,3E80=16000 off
	
},
{
	{1, {0x7EF0,0x160,0,  400*8,600*8}},
	{0, {0x7EF0,0x160,0,  400*8,1600*8}},
	
	
		
},
{
	{1, {0x7EF0,0x160,0,  600*8,600*8}},
	{0, {0x7EF0,0x160,0,  1200*8,600*8}},
	
		
},

{
	
	{1, {0x7EF0,0x160,0,  800*8,400*8}},
	{0, {0x7EF0,0x160,0,  200*8,1600*8}},
		
},
{
	
	{0, {0x7EF0,0x160,0,  200*8,300*8}},
	
		
}

};
/*


                 1209 Hz 1336 Hz 1477 Hz 1633 Hz

                          ABC     DEF
   697 Hz          1       2       3       A

                  GHI     JKL     MNO
   770 Hz          4       5       6       B

                  PRS     TUV     WXY
   852 Hz          7       8       9       C

                          oper
   941 Hz          *       0       #       D

0 dbm0 tones

INFO: Freq = 1633.000000    for n=1 or n=2 
 OSCn = 0x2460, OSCnX = 0x10ab, OSCnY = 0x0000

INFO: Freq = 697.000000    for n=1 or n=2 
 OSCn = 0x6d50, OSCnX = 0x0643, OSCnY = 0x0000

INFO: Freq = 770.000000    for n=1 or n=2 
 OSCn = 0x6950, OSCnX = 0x06f6, OSCnY = 0x0000

INFO: Freq = 852.000000    for n=1 or n=2 
 OSCn = 0x6460, OSCnX = 0x07c3, OSCnY = 0x0000

INFO: Freq = 941.000000    for n=1 or n=2 
 OSCn = 0x5ea0, OSCnX = 0x08a5, OSCnY = 0x0000

INFO: Freq = 1209.000000    for n=1 or n=2 
 OSCn = 0x4a80, OSCnX = 0x0b79, OSCnY = 0x0000

INFO: Freq = 1336.000000    for n=1 or n=2 
 OSCn = 0x3fc0, OSCnX = 0x0cec, OSCnY = 0x0000

INFO: Freq = 1477.000000    for n=1 or n=2 
 OSCn = 0x3320, OSCnX = 0x0ea0, OSCnY = 0x0000

INFO: Freq = 1633.000000    for n=1 or n=2 
 OSCn = 0x2460, OSCnX = 0x10ab, OSCnY = 0x0000


*/

/*************************0.0 Dbm0 tones*****************************************/
tone_struct dual_tones[]= { // Define(s) Touch-Tones & Call progress tones
	// OSC 1  x     y on_h on_l			OSC 2
	{{0x6d50,0x0643,0,0x03,0x20,0,0}, {0x4a80,0x0b79,0,0x03,0x20,0,0}}, // DTMF 1
	{{0x6d50,0x0643,0,0x03,0x20,0,0}, {0x3fc0,0x0cec,0,0x03,0x20,0,0}}, // DTMF 2
	{{0x6d50,0x0643,0,0x03,0x20,0,0}, {0x3320,0x0ea0,0,0x03,0x20,0,0}}, // DTMF 3
	{{0x6d50,0x0643,0,0x03,0x20,0,0}, {0x2460,0x10ab,0,0x03,0x20,0,0}}, // DTMF A
	{{0x6950,0x06f6,0,0x03,0x20,0,0}, {0x4a80,0x0b79,0,0x03,0x20,0,0}}, // DTMF 4
	{{0x6950,0x06f6,0,0x03,0x20,0,0}, {0x3fc0,0x0cec,0,0x03,0x20,0,0}}, // DTMF 5
	{{0x6950,0x06f6,0,0x03,0x20,0,0}, {0x3320,0x0ea0,0,0x03,0x20,0,0}}, // DTMF 6
	{{0x6950,0x06f6,0,0x03,0x20,0,0}, {0x2460,0x10ab,0,0x03,0x20,0,0}}, // DTMF B
	{{0x6460,0x07c3,0,0x03,0x20,0,0}, {0x4a80,0x0b79,0,0x03,0x20,0,0}}, // DTMF 7
	{{0x6460,0x07c3,0,0x03,0x20,0,0}, {0x3fc0,0x0cec,0,0x03,0x20,0,0}}, // DTMF 8
	{{0x6460,0x07c3,0,0x03,0x20,0,0}, {0x3320,0x0ea0,0,0x03,0x20,0,0}}, // DTMF 9
	{{0x6460,0x07c3,0,0x03,0x20,0,0}, {0x2460,0x10ab,0,0x03,0x20,0,0}}, // DTMF C
	{{0x5ea0,0x08a5,0,0x03,0x20,0,0}, {0x4a80,0x0b79,0,0x03,0x20,0,0}}, // DTMF *
	{{0x5ea0,0x08a5,0,0x03,0x20,0,0}, {0x3fc0,0x0cec,0,0x03,0x20,0,0}}, // DTMF 0
	{{0x5ea0,0x08a5,0,0x03,0x20,0,0}, {0x3320,0x0ea0,0,0x03,0x20,0,0}}, // DTMF #
	{{0x5ea0,0x08a5,0,0x03,0x20,0,0}, {0x2460,0x10ab,0,0x03,0x20,0,0}}  // DTMF D
};


/* This is an array of frequencies and their oscillator register values
for a single tones.  Lower in the code is a getOscCoeff (short int freq)*/



unsigned short oscCoeff[] = /*  These are the Primary Tones for Signaling Tone construction */
/* -18 dbM */
{ 
350    /* HERTZ */, 0x7b30, 0x0063,
440    /* HERTZ */ ,0x7870, 0x007d,
480    /* HERTZ */, 0x7700, 0x0089,
620    /* HERTZ */, 0x7120, 0x00b2,
200    /* HERTZ */, 0x7e70, 0x0038,
300    /* HERTZ */, 0x7c70, 0x0055,
400    /* HERTZ */, 0x79c0, 0x0071
};


#define VSCALE .375
unsigned char upperlimit= (unsigned char)(31/VSCALE), lowerlimit=(unsigned char)(10/VSCALE)+1;
unsigned short renOffset=30, renScaler=110;
short renError=0;




#endif



