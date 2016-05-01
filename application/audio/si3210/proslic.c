/* ProSLIC.c  */
/* This file contains all the SPI port and Indirect Register
data and code */
/*  8/22/02  RDL  */

#include "audio_cfg.h"

#ifdef INCLUDE_SI3210
//#include "data.h"
//#include "Conio.h"
//#include "stdio.h"
#include "proslic.h"
#include "risc.h"
#include "def_type.h"
#include "two_risc_slic.h"
#include "m25p64.h"
#include "sif.h"
#include "comm.h"
//#include "stdlib.h"
//#include "time.h"
//#include "stdlib.h"
//#include "gr909.h"
//#include "chip_cfg.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

//added for os operation
extern chipStruct chipData[CHANNEL_NUM];
int sem = 0;


CHAR * exceptionStrings[] = 
{ " ProSLIC not communicating", "Time out durring Power Up", "Time out durring Power Down","Power is Leaking; might be a short",

" Tip or Ring Ground Short",

"Too Many Q1 Power Alarms" ,"Too Many Q2 Power Alarms" ,"Too Many Q3 Power Alarms" 

"Too Many Q4 Power Alarms" ,"Too Many Q5 Power Alarms" ,"Too Many Q6 Power Alarms" };


VOID exception (enum exceptions e)
/* This is where an embedded system would call its exception handler */
/* This code runs a print out routine */
{
	printf( "\n                 WARNING: %s\n",exceptionStrings[e] );
	//printf( " Terminating the program\n");
}
/*
VOID sleep( unsigned long wait ) //wait (unit:ms)
{
 ULONG cpu_sr, time2, time1, t;

	wait = 2*wait/3;
 OS_ENTER_CRITICAL();

  time1 = GetCount();

 while(1) {

  time2 = GetCount();

if(time2 > time1)
     t = time2 - time1;
else
     t = 0xffffffff  - time1 + time2;
     
     //transfer to ms
     t = t / (CPU_MAIN_CLOCK/(2*1000));
  
     if(t>wait)
         break;

 }
}
*/

VOID sleep( unsigned int wait )	//wait (unit:ms)
{
#if 0
	ULONG target = wait*12500*8;

	ExceptEnCount_Compare(target);
	ExceptClrCount();
	while(GetCount() < target);
#endif

    /* gb modify on 2007.04.20 */
    unsigned int second, minisecond;

    second = wait/1000;
    minisecond = wait%1000;

    if(minisecond < 1000/OS_TICKS_PER_SEC)         
        minisecond = 1000/OS_TICKS_PER_SEC;    

    if(second > 59)
        second = 59;
                
    OSTimeDlyHMSM(0, 0, second, minisecond);    
    	
}
/*
unsigned long delay(unsigned long wait)
{
	ULONG target = wait*12500;

	ExceptEnCount_Compare(target);
	ExceptClrCount();
	while(GetCount() < target);

	return(GetCount() - target);
}
*/
void SifInit()
{   
    while(SifBusyRead());      // If a transaction doesn't end, 
                                            // the initialization cannot be started.

    SifTimingRegSet(SIF_SI_TIMING_RISING, SIF_SO_TIMING_FALLING, SIF_CLK_POL_IDLE1, \
                    SIF_CS_WIDTH_OFF_NONE, SIF_CS_DLY_1CLK, SIF_CS_OFF_1CLK, \
                    SIF_GAP_NONE, SIF_BGAP_NONE, 0x7 /* clk_div */);     
    SifModeRegSet(SIF_CBUS_MODE, 0, SIF_CTL_SEL_NONE, SIF_ADD_SEL_NONE, \
                  SIF_READ, 0 /* sID */, 1 /* blen */);

    SifIntrDis();                             // Disable the SIF interrupt.
}

void semlock()
{
	int i;
	do{
		i++;
		i--;
		//printf("s");
	}while(sem);
	sem =1;
}

void semunlock()
{
	sem = 0;
}

void SlicSemPend(int SemID)
{
    int lock;	
 	int timeout = 10000;
	
    do{
	lock = *(volatile unsigned *)(RISC_BASE_ADDR + 0x20 + SemID * 4);
		if(0 == timeout--)
		{
			printf("sem timeout %d times\n",sem++);
			RiscSemUnLock(SemID);
			break;
		}
    }while(lock);
}
//write a byte to Spi
void byteToSpi (unsigned char byte,unsigned int cs)
{	
	
	unsigned int tmp = 0xa0200001;

	//semlock();
	//SlicSemPend(5);
	RiscSemPend(5);
	//while(RiscRdCNT(SIF_LINUX_TO_UCOS_CNT)){}
	//RiscIncCNT(SIF_UCOS_TO_LINUX_CNT, 1);
	
      	while(SifBusyRead());       // If a transaction doesn't end, new one cannot be started.

	SifTimingRegSet(SIF_SI_TIMING_RISING, SIF_SO_TIMING_FALLING, SIF_CLK_POL_IDLE1, \
                    SIF_CS_WIDTH_OFF_NONE, SIF_CS_DLY_1CLK, SIF_CS_OFF_1CLK, \
                    SIF_GAP_NONE, SIF_BGAP_NONE, 0x7 /* clk_div */);     
	
	rSIF_DAT = byte;

	tmp |= (cs<<26);
	//printf("mode=0x%x\n",tmp);
	rSIF_MODE = tmp;

	//RiscDecCNT(SIF_UCOS_TO_LINUX_CNT, 1);
	//semunlock();
	RiscSemUnLock(5);
	//while(SifBusyRead());  
    
}// byteToSpi


unsigned char  spiToByte(unsigned int cs)
{	
	//SlicSemPend(5);
	RiscSemPend(5);
	//semlock();
	//while(RiscRdCNT(SIF_LINUX_TO_UCOS_CNT)){}
	//RiscIncCNT(SIF_UCOS_TO_LINUX_CNT, 1);
	
	unsigned int tmp = 0xa0000001;
	while(SifBusyRead());  

	SifTimingRegSet(SIF_SI_TIMING_RISING, SIF_SO_TIMING_FALLING, SIF_CLK_POL_IDLE1, \
                    SIF_CS_WIDTH_OFF_NONE, SIF_CS_DLY_1CLK, SIF_CS_OFF_1CLK, \
                    SIF_GAP_NONE, SIF_BGAP_NONE, 0x7 /* clk_div */);     
	tmp = tmp | (cs<<26);

	rSIF_MODE = tmp;
	//printf("mode=0x%x\n",tmp);

	while(SifBusyRead());  

	tmp  = rSIF_DAT;

	//RiscDecCNT(SIF_UCOS_TO_LINUX_CNT, 1);
	//semunlock();
	RiscSemUnLock(5);
	
        return tmp;
	
} 


VOID HwSlicReset()
{
	//rRISC_GPIO_DIR &= 0xf7ffffff;	//gpio27 is output.
	//rRISC_GPIO &= 0xf7ffffff;	//write 0 to gpio27	

	rRISC_GPIO_DIR &= 0xfffffbff;	//gpio10 is output.
	rRISC_GPIO &= 0xfffffBff;	//write 0 to gpio10	

}

VOID HwSlicGpioReset(int channelid)
{
	//rRISC_GPIO_DIR &= 0xf7ffffff;	//gpio27 is output.
	//rRISC_GPIO &= 0xf7ffffff;	//write 0 to gpio27	

	rRISC_GPIO_DIR &= ~(1<<chipData[channelid].reset);	//gpio10 is output.	0xfffffbff
	rRISC_GPIO &= ~(1<<chipData[channelid].reset);	//write 0 to gpio10	
}

/* This sets Parllel port  pin1 (strobe to high ********************/

VOID HwSlicGpioReleaseReset(int channelid)
{	
	//rRISC_GPIO_DIR &= 0xf7ffffff;	//gpio27 is output.
	//rRISC_GPIO |= 0x8000000;	//write 1 to gpio27	

	rRISC_GPIO_DIR &= ~(1<<chipData[channelid].reset);	//gpio10 is output.
	rRISC_GPIO |= (1<<chipData[channelid].reset);	//write 1 to gpio10	
//chip select 
	//rRISC_GPIO_DIR &= 0xff7ffbff;	//gpio10 is output.
	//rRISC_GPIO ^= 0x800000;	//write 1 to gpio10	
}
/*********************Parallel Port Bit Bang Sequences *********/

/* This small sequence is for the parallel port to 
bit bang out the value of the interrupt line.
This just enables the output driver so the interrupt 
line of the Proslic can be read by the parallel port
*/

/* This more complex routine  bit-bangs out the address as above and the clocks the 
   data back from the ProSLIC */

BYTE possibleAddressCorrect(BYTE i,ULONG channelid)
{

	BYTE j;
	if (!Si3215orSi3216(channelid)) 
		return (i);
		
	if (i < 28)
			j = i+13;
		else
	if ((i > 63) && (i < 69))
				j = i - 23;
		else
	if ((i > 68) && (i <75 ))
				j = i + 30;
		else
	if ((i>74)&&(i<81))
				j = i + 13;
		else
		j = i; 
		
			return j;
	
}


/*
Si3216 checks to see if this is a Si3215 or a Si3216
if it is the indirect registers need to be adjusted.
*/

LONG Si3215orSi3216(ULONG channelid)

{
	return (readDirectReg(1,channelid) & 0x80 == 0x80);
}


#ifndef DAISYDOUBLE

BYTE readDirectReg(BYTE cwaddress,unsigned int channelid)
{
	byteToSpi((BYTE)(cwaddress|0x80),chipData[channelid].cs);
	 return spiToByte(chipData[channelid].cs);
}


VOID writeDirectReg(BYTE cwaddress, BYTE cwdata,unsigned int channelid)
{
	byteToSpi((BYTE)(0x7f & cwaddress),chipData[channelid].cs);
	byteToSpi(cwdata,chipData[channelid].cs);

}
#endif

VOID waitForIndirectReg(unsigned int channelid)
{
	while (readDirectReg(I_STATUS,channelid));
}

WORD readIndirectReg(BYTE cwaddress,unsigned int channelid)
{ 
	cwaddress = possibleAddressCorrect(cwaddress,channelid);  // corrects the address if Si3216/Si3215
	waitForIndirectReg(channelid);
	writeDirectReg(IAA,cwaddress,channelid); 
	waitForIndirectReg(channelid);
	return ( readDirectReg(IDA_LO,channelid) | (readDirectReg (IDA_HI,channelid))<<8);
}

VOID writeIndirectReg(BYTE cwaddress, WORD swdata, unsigned int channelid)
{
	cwaddress = possibleAddressCorrect(cwaddress,channelid);
	waitForIndirectReg(channelid);
	writeDirectReg(IDA_LO,(BYTE)(swdata & 0xFF),channelid);
	writeDirectReg(IDA_HI,(BYTE)((swdata & 0xFF00)>>8),channelid);
	writeDirectReg(IAA,cwaddress,channelid);
}

/******************************************************************************************************************************/
/************************************************DAISY CHAIN*******************************************************************/
/******************************************************************************************************************************/
/*
BYTE readProSlicDirectRegDaisychain(BYTE cwchipNumber, BYTE cwaddress)
{
	byteToSpi(chipSelectByte[cwchipNumber]);
	byteToSpi((BYTE)(cwaddress|0x80));
	 return spiToByte();
}

VOID writeProSlicDirectRegDaisychain(BYTE cwchipNumber, BYTE cwaddress, BYTE cwdata)
{
	byteToSpi(chipSelectByte[cwchipNumber]);
	byteToSpi((BYTE)(0x7f & cwaddress));
	byteToSpi(cwdata);

}
*/
#ifdef DAISYDOUBLE

BYTE readDirectReg( BYTE cwaddress)
{
	byteToSpi(chipSelectByte[currentChip+1]);
	byteToSpi((BYTE)(cwaddress|0x80));
	 return spiToByte();
}

VOID writeDirectReg( BYTE cwaddress, BYTE cwdata)
{
	byteToSpi(chipSelectByte[currentChip+1]);
	byteToSpi((BYTE)(0x7f & cwaddress));
	byteToSpi(cwdata);

}

VOID setDaisyChainMode(VOID)
{
	byteToSpi(0);
	byteToSpi(0x80);

	for (currentChip=0; currentChip<8; currentChip++)
	{	
		writeDirectReg(2,0xaa);
		if (readDirectReg(2)==0xaa) 
			numberOfChips=currentChip+1;
		
		else 
		{
			printf("\n There are %d chips present\n", numberOfChips);
			return;	
		}
	}

}
#endif

/*
The following Array contains:
*/
indirectRegister  indirectRegisters[] =
{
/* Reg#			Label		Initial Value  */

{	0,	"DTMF_ROW_0_PEAK",	0x55C2	},
{	1,	"DTMF_ROW_1_PEAK",	0x51E6	},
{	2,	"DTMF_ROW2_PEAK",	0x4B85	},
{	3,	"DTMF_ROW3_PEAK",	0x4937	},
{	4,	"DTMF_COL1_PEAK",	0x3333	},
{	5,	"DTMF_FWD_TWIST",	0x0202	},
{	6,	"DTMF_RVS_TWIST",	0x0202	},
{	7,	"DTMF_ROW_RATIO",	0x0198	},
{	8,	"DTMF_COL_RATIO",	0x0198	},
{	9,	"DTMF_ROW_2ND_ARM",	0x0611	},
{	10,	"DTMF_COL_2ND_ARM",	0x0202	},
{	11,	"DTMF_PWR_MIN_",	0x00E5	},
{	12,	"DTMF_OT_LIM_TRES",	0x0A1C	},
{	13,	"OSC1_COEF",		0x7b30	},
{	14,	"OSC1X",			0x0063	},
{	15,	"OSC1Y",			0x0000	},
{	16,	"OSC2_COEF",		0x7870	},
{	17,	"OSC2X",			0x007d	},
{	18,	"OSC2Y",			0x0000	},
{	19,	"RING_V_OFF",		0x0000	},
{	20,	"RING_OSC",			0x7EF0	},
{	21,	"RING_X",			0x0160	},
{	22,	"RING_Y",			0x0000	},
{	23,	"PULSE_ENVEL",		0x2000	},
{	24,	"PULSE_X",			0x2000	},
{	25,	"PULSE_Y",			0x0000	},
{	26,	"RECV_DIGITAL_GAIN",0x4000	},
{	27,	"XMIT_DIGITAL_GAIN",0x4000	},
{	28,	"LOOP_CLOSE_TRES",	0x1000	},
{	29,	"RING_TRIP_TRES",	0x3600	},
{	30,	"COMMON_MIN_TRES",	0x1000	},
{	31,	"COMMON_MAX_TRES",	0x0200	},
{	32,	"PWR_ALARM_Q1Q2",	0x7c0   },
{	33,	"PWR_ALARM_Q3Q4",	0x2600	},
{	34,	"PWR_ALARM_Q5Q6",	0x1B80	},
{	35,	"LOOP_CLSRE_FlTER",	0x8000	},
{	36,	"RING_TRIP_FILTER",	0x0320	},
{	37,	"TERM_LP_POLE_Q1Q2",0x08c	},
{	38,	"TERM_LP_POLE_Q3Q4",0x0100	},
{	39,	"TERM_LP_POLE_Q5Q6",0x0010	},
{	40,	"CM_BIAS_RINGING",	0x0C00	},
{	41,	"DCDC_MIN_V",		0x0C00	},
{	43,	"LOOP_CLOSE_TRES Low",	0x1000	},
{	99,	"FSK 0 FREQ PARAM",	0x00DA	},
{	100,"FSK 0 AMPL PARAM",	0x6B60	},
{	101,"FSK 1 FREQ PARAM",	0x0074	},
{	102,"FSK 1 AMPl PARAM",	0x79C0	},
{	103,"FSK 0to1 SCALER",	0X1120	},
{	104,"FSK 1to0 SCALER",	0x3BE0	},
{	97,	"RCV_FLTR",			0},
{0,"",0},
};
/*
VOID waitForProSlicIndirectRegAccessDaisychain(BYTE cwchipNumber)
{
	while(readProSlicDirectRegDaisychain(cwchipNumber,I_STATUS));

}

WORD readProSlicInDirectRegDaisychain(BYTE cwchipNumber, BYTE cwaddress)
{ 
	WORD swboth=0, swhi=0 ;
    	cwaddress = possibleAddressCorrect(cwaddress);
	waitForProSlicIndirectRegAccessDaisychain(cwchipNumber);
	writeProSlicDirectRegDaisychain(cwchipNumber,IAA,cwaddress);
	waitForProSlicIndirectRegAccessDaisychain(cwchipNumber);
	return ( readProSlicDirectRegDaisychain(cwchipNumber,IDA_LO) | (readProSlicDirectRegDaisychain (cwchipNumber,IDA_HI))<<8);
}


VOID writeProSlicInDirectRegDaisychain(BYTE cwchipNumber, BYTE cwaddress, WORD cwdata)
{
	cwaddress = possibleAddressCorrect(cwaddress);
	waitForProSlicIndirectRegAccessDaisychain(cwchipNumber);
	writeProSlicDirectRegDaisychain(cwchipNumber,IDA_LO,(BYTE)(cwdata & 0xFF));
	writeProSlicDirectRegDaisychain(cwchipNumber,IDA_HI,(BYTE)((cwdata & 0xFF00)>>8));
	writeProSlicDirectRegDaisychain(cwchipNumber,IAA,cwaddress);
}


VOID HwSlicInitializeIndirectRegistersDaisychain(BYTE cwchipNumber)
{
	BYTE i;
	for (i=0; i<42; i++){
		writeProSlicInDirectRegDaisychain(cwchipNumber,i,indirectRegisters[i].initial);
	}
}


VOID printIndirectRegistersDaisychain(BYTE cwchipNumber)
{ 
	BYTE i;
	for (i=0; i<42; i++){
		printf ("\n Chip%2i", cwchipNumber);
		printf(" %s = 0x%4.2X  should be 0x%4.2X ",indirectRegisters[i].name,readProSlicInDirectRegDaisychain(cwchipNumber,i),indirectRegisters[i].initial );
	}
}


VOID HwSlicVerifyIndirectRegistersDaisychain(BYTE cwchipNumber)
{ 
	LONG lwpassed = 1;
	WORD i,j, swinitial;
	for (i=0; i<42; i++) {
		j=readProSlicInDirectRegDaisychain(cwchipNumber, (BYTE) i);
		swinitial= indirectRegisters[i].initial;
		if ( j != swinitial )
		{
			
			printf("\n %s  iREG %X = %X  should be %X  for chip %i",indirectRegisters[i].name,i,j,swinitial,cwchipNumber );
			lwpassed = 0;
		}	
	}

	if (lwpassed) {
		printf("Initialization of Indirect Registers completed successfully.\n");
	} else {
		printf("\n");
			printf("Initialization of Indirect Registers UNSUCCESSFULL.\n");
		
	}
}


VOID genTone(tone_struct tone) { 
	// Uber function to extract values for oscillators from tone_struct
	// place them in the registers, and enable the oscillators.
	BYTE osc1_ontimer_enable=0, osc1_offtimer_enable=0,osc2_ontimer_enable=0,osc2_offtimer_enable=0;
	LONG enable_osc2=0;

	//loopBackOff();
	disableOscillators(); // Make sure the oscillators are not already on.

	if (tone.osc1.coeff == 0 || tone.osc1.x == 0) {
		// Error!
		printf("You passed me a invalid struct!\n");
		return;
	}
	// Setup osc 1
	writeIndirectReg( OSC1_COEF, tone.osc1.coeff);
	writeIndirectReg( OSC1X, tone.osc1.x);
	writeIndirectReg( OSC1Y, tone.osc1.y);
	//printf("OUt-> 0x%04x\n",tone.osc1.coeff);
	// Active Timer


	if (tone.osc1.on_hi_byte != 0) {
		writeDirectReg( OSC1_ON__LO, tone.osc1.on_low_byte);
		writeDirectReg( OSC1_ON_HI, tone.osc1.on_hi_byte);
		osc1_ontimer_enable = 0x10;
	}
	// Inactive Timer
	if (tone.osc1.off_hi_byte != 0) {
		writeDirectReg( OSC1_OFF_LO, tone.osc1.off_low_byte);
		writeDirectReg( OSC1_OFF_HI, tone.osc1.off_hi_byte);
		osc1_offtimer_enable = 0x08;
	}
	
	if (tone.osc2.coeff != 0) {
		// Setup OSC 2
		writeIndirectReg( OSC2_COEF, tone.osc2.coeff);
		writeIndirectReg( OSC2X, tone.osc2.x);
		writeIndirectReg( OSC2Y, tone.osc2.y);
		
		// Active Timer
		if (tone.osc1.on_hi_byte != 0) {
			writeDirectReg( OSC2_ON__LO, tone.osc2.on_low_byte);
			writeDirectReg( OSC2_ON_HI, tone.osc2.on_hi_byte);
			osc2_ontimer_enable = 0x10;
		}
		// Inactive Timer
		if (tone.osc1.off_hi_byte != 0) {
			writeDirectReg( OSC2_OFF_LO, tone.osc2.off_low_byte);
			writeDirectReg( OSC2_OFF_HI, tone.osc2.off_hi_byte);
			osc2_offtimer_enable = 0x08;
		}
		enable_osc2 = 1;
	}

	writeDirectReg( OSC1, (BYTE)(0x06 | osc1_ontimer_enable | osc1_offtimer_enable));
	if (enable_osc2)
		writeDirectReg( OSC2, (BYTE)(0x06 | osc2_ontimer_enable | osc2_offtimer_enable));
	return;
}

*/
VOID dialTone(ULONG channelid)
{
  writeIndirectReg(13,DIALTONE_IR13,channelid);
  writeIndirectReg(14,DIALTONE_IR14,channelid);
  writeIndirectReg(16,DIALTONE_IR16,channelid);
  writeIndirectReg(17,DIALTONE_IR17,channelid);
  writeDirectReg(36,  DIALTONE_DR36,channelid);
  writeDirectReg(37,  DIALTONE_DR37,channelid);
  writeDirectReg(38,  DIALTONE_DR38,channelid);
  writeDirectReg(39,  DIALTONE_DR39,channelid);
  writeDirectReg(40,  DIALTONE_DR40,channelid);
  writeDirectReg(41,  DIALTONE_DR41,channelid);
  writeDirectReg(42,  DIALTONE_DR42,channelid);
  writeDirectReg(43,  DIALTONE_DR43,channelid);

  writeDirectReg(32,  DIALTONE_DR32,channelid);
  writeDirectReg(33,  DIALTONE_DR33,channelid);
}


VOID busyTone(ULONG channelid)
{
 
  writeIndirectReg(13,BUSYTONE_IR13,channelid);
  writeIndirectReg(14,BUSYTONE_IR14,channelid);
  writeIndirectReg(16,BUSYTONE_IR16,channelid);
  writeIndirectReg(17,BUSYTONE_IR17,channelid);
  writeDirectReg(36,  BUSYTONE_DR36,channelid);
  writeDirectReg(37,  BUSYTONE_DR37,channelid);
  writeDirectReg(38,  BUSYTONE_DR38,channelid);
  writeDirectReg(39,  BUSYTONE_DR39,channelid);
  writeDirectReg(40,  BUSYTONE_DR40,channelid);
  writeDirectReg(41,  BUSYTONE_DR41,channelid);
  writeDirectReg(42,  BUSYTONE_DR42,channelid);
  writeDirectReg(43,  BUSYTONE_DR43,channelid);
  
  writeDirectReg(32,  BUSYTONE_DR32,channelid);
  writeDirectReg(33,  BUSYTONE_DR33,channelid);
}

VOID reorderTone(ULONG channelid)
{
  writeIndirectReg(13,REORDERTONE_IR13,channelid);
  writeIndirectReg(14,REORDERTONE_IR14,channelid);
  writeIndirectReg(16,REORDERTONE_IR16,channelid);
  writeIndirectReg(17,REORDERTONE_IR17,channelid);
  writeDirectReg(36,  REORDERTONE_DR36,channelid);
  writeDirectReg(37,  REORDERTONE_DR37,channelid);
  writeDirectReg(38,  REORDERTONE_DR38,channelid);
  writeDirectReg(39,  REORDERTONE_DR39,channelid);
  writeDirectReg(40,  REORDERTONE_DR40,channelid);
  writeDirectReg(41,  REORDERTONE_DR41,channelid);
  writeDirectReg(42,  REORDERTONE_DR42,channelid);
  writeDirectReg(43,  REORDERTONE_DR43,channelid);
  
  writeDirectReg(32,  REORDERTONE_DR32,channelid);
  writeDirectReg(33,  REORDERTONE_DR33,channelid);
 
}


VOID congestionTone(ULONG channelid)
{
  writeIndirectReg(13,CONGESTIONTONE_IR13,channelid);
  writeIndirectReg(14,CONGESTIONTONE_IR14,channelid);
  writeIndirectReg(16,CONGESTIONTONE_IR16,channelid);
  writeIndirectReg(17,CONGESTIONTONE_IR17,channelid);
  writeDirectReg(36,  CONGESTIONTONE_DR36,channelid);
  writeDirectReg(37,  CONGESTIONTONE_DR37,channelid);
  writeDirectReg(38,  CONGESTIONTONE_DR38,channelid);
  writeDirectReg(39,  CONGESTIONTONE_DR39,channelid);
  writeDirectReg(40,  CONGESTIONTONE_DR40,channelid);
  writeDirectReg(41,  CONGESTIONTONE_DR41,channelid);
  writeDirectReg(42,  CONGESTIONTONE_DR42,channelid);
  writeDirectReg(43,  CONGESTIONTONE_DR43,channelid);

  writeDirectReg(32,  CONGESTIONTONE_DR32,channelid);
  writeDirectReg(33,  CONGESTIONTONE_DR33,channelid);

}

VOID ringBackTone(ULONG channelid)
{
  writeIndirectReg(13,RINGBACKTONE_IR13,channelid);
  writeIndirectReg(14,RINGBACKTONE_IR14,channelid);
  writeIndirectReg(16,RINGBACKTONE_IR16,channelid);
  writeIndirectReg(17,RINGBACKTONE_IR17,channelid);
  writeDirectReg(36,  RINGBACKTONE_DR36,channelid);
  writeDirectReg(37,  RINGBACKTONE_DR37,channelid);
  writeDirectReg(38,  RINGBACKTONE_DR38,channelid);
  writeDirectReg(39,  RINGBACKTONE_DR39,channelid);
  writeDirectReg(40,  RINGBACKTONE_DR40,channelid);
  writeDirectReg(41,  RINGBACKTONE_DR41,channelid);
  writeDirectReg(42,  RINGBACKTONE_DR42,channelid);
  writeDirectReg(43,  RINGBACKTONE_DR43,channelid);
  
  writeDirectReg(32,  RINGBACKTONE_DR32,channelid);
  writeDirectReg(33,  RINGBACKTONE_DR33,channelid);
 
}

 VOID standardRinging(ULONG channelid)
 { 	
	// Enables ringing mode on ProSlic for standard North American ring
	//	RING_ON__LO	48
	//	RING_ON_HI	49
	//	RING_OFF_LO	50
	//	RING_OFF_HI	51
	// Active Timer


	writeDirectReg( RING_ON__LO, 0x40,channelid); // low reg 48
	writeDirectReg( RING_ON_HI, 0x1f,channelid); // hi reg 49
	// Inactive Timer
	writeDirectReg( RING_OFF_LO, 0x80,channelid); // low reg 50
	writeDirectReg( RING_OFF_HI, 0x3e,channelid); // hi reg 51
	// Enable timers for ringing oscillator
	writeDirectReg( 34, 0x18,channelid);

}

VOID HwSlicSetTimeSlot(ULONG channelid,ULONG rx_slot,ULONG tx_slot)
{
	writeDirectReg(0x2, rx_slot*8+1,channelid);          //trasaction start count;
    	writeDirectReg(0x4, tx_slot*8+1,channelid);          //receive start count   
}

VOID HwSlicSetPara(ULONG channelid,BYTE para)
{
	BYTE tmp;

	tmp = (PCM_ENALBE<<5)  | (para<<3);
	
	writeDirectReg(0x1, tmp,channelid); 
}

void SlicSetTimeSlot(int TsTx, int TsRx, int channel)
{
	writeDirectReg(0x2, TsTx, channel);          //trasaction start count;
	writeDirectReg(0x4, TsRx, channel);          //receive start count   
}

 BYTE HwSlicPowerUp(ULONG channelid)
{ 
	BYTE cwvBat ; 
	LONG i=0, lwinitialTime, lwpowerTime=0;

	if (HwSlicChipType(channelid) == 3)  // M HwSlicChipVersion correction
	{
		writeDirectReg(92,INIT_SI3210M_DR92,channelid);// M HwSlicChipVersion
		writeDirectReg(93,INIT_SI3210M_DR93,channelid);// M HwSlicChipVersion
	}
	else	
	{
		writeDirectReg(93, 0x19,channelid); 
		writeDirectReg(92, 0xff,channelid); /* set the period of the DC-DC converter to 1/64 kHz  START OUT SLOW*/
	}

	//initialTime= clock();
	
	//iteDirectReg(74,0x20); 		//stone debug
	
	writeDirectReg(14, 0,channelid); /* Engage the DC-DC converter */
/*	printf("reg[14] = 0x%x",readDirectReg(14));
	for(i=0;i<20;i++)
	{
		printf("Direct Reg[%d] = 0x%x",i,readDirectReg(i));
		printf("Indirect Reg[%d] = 0x%x",i,readIndirectReg(i));
	}
	i = 0;
	*/  
	while ((cwvBat=readDirectReg(82,channelid)) < 0xc0)	//0xc0
	{ 
		sleep(50);
		++i;
		if (i > 200) exception(TIMEoUTpOWERuP);
	
	}
	printf("\ncurrent votage is :%dV\n",cwvBat*376/1000);

	//powerTime= clock() - initialTime;
	//if (i>500) 	printf("\nPower Up took %i milliseconds.\n",powerTime); 
   	
	
	if (HwSlicChipType(channelid) == 3)  // M HwSlicChipVersion correction
	{
		writeDirectReg(92,0X80 +INIT_SI3210M_DR92,channelid);// M HwSlicChipVersion
	}
	else
		writeDirectReg(93, 0x99,channelid);  /* DC-DC Calibration  */
//printf("start calibrate,type=0x%x\n",HwSlicChipType(channelid));
	while(0x80&readDirectReg(93,channelid));
		//printf("0x%x\n",readDirectReg(93,channelid));  // Wait for DC-DC Calibration to complete
//printf("end calibrate\n");
	return cwvBat;
}
 
BYTE HwSlicPowerLeakTest(ULONG channelid)
{ 
	BYTE cwvBat ;
	LONG i=0 ;
	
	writeDirectReg(64,0,channelid);

	writeDirectReg(14, 0x10,channelid); 
	sleep(500);  // one second
	if (cwvBat=readDirectReg(82,channelid) < 0x4)  // 6 volts
		 exception(POWERlEAK);
	
	return cwvBat;
}


VOID xcalibrate(ULONG channelid)
{ 
	BYTE i ;

	//HwSlicSetState(CALIBRATE);
	
	writeDirectReg(97, 0x1e,channelid); /* Do all of the Calibarations */
	writeDirectReg(96, 0x47,channelid); /* Start the calibration No Speed Up */
	
	manualCalibrate(channelid);

	writeDirectReg(23,4,channelid);  // enable interrupt for the balance Cal
	writeDirectReg(97,0x1,channelid);
	writeDirectReg(96,0x40,channelid);

	while (readDirectReg(96,channelid) != 0 );
	printf("\nCalibration Vector Registers 98 - 107: ");
	
	for (i=98; i < 107; i++)  printf( "%X.", readDirectReg(i,channelid));
	printf("%X \n\n",readDirectReg(107,channelid));

}

VOID HwSlicGoActive(ULONG channelid)

{

	writeDirectReg(64,1,channelid);	/* LOOP STATE REGISTER SET TO ACTIVE */
							/* Active works for on-hook and off-hook see spec. */
							/* The phone hook-switch sets the off-hook and on-hook substate*/
	sleep(100);
	//if ((readDirectReg(80,channelid) < 2) || readDirectReg(81,channelid) < 2) exception(TIPoRrINGgROUNDsHORT); /* Check for grounded Tip or Ring Leads*/

}

BYTE HwSlicChipVersion(ULONG channelid)
{
	return 0xf & readDirectReg(0,channelid); 
}

BYTE HwSlicChipType (ULONG channelid)
{
	return (0x30 & readDirectReg(0,channelid)) >> 4; 
}

VOID HwSlicSelfTest(ULONG channelid)
{

	/*  Begin Sanity check  Optional */
	if (readDirectReg(8,channelid) !=2) exception(PROSLICiNSANE);
	if (readDirectReg(64,channelid) !=0) exception(PROSLICiNSANE);
	if (readDirectReg(11,channelid) !=0x33) exception(PROSLICiNSANE);
	printf("reg[8] = 0x%x   reg[64] = 0x%x  reg[11] = 0x%x ",readDirectReg(8,channelid),readDirectReg(64,channelid),readDirectReg(11,channelid));

	/* End Sanity check */
}

VOID HwSlicInitialize(ULONG channelid)
{

	BYTE cwtype,cwversion;
	if(channelid == 0)
	{
		HwSlicGpioReset(channelid); /* sets the reset of the proslic */
		sleep( 250); /* hold it for a quarter of a second */
	}
	// sleep ( is in milliseconds )
	/* This is discharging a capacitor through a CMOS buffer */
	//if(channelid == chipData[0].cs)
	HwSlicGpioReleaseReset(channelid); 
	/* Release the Reset */
	
	sleep( 250);
	/*  Another Quarter of a Second */
	//printf"use channel %d\n",channelid);
	HwSlicSelfTest(channelid);
	cwversion = HwSlicChipVersion(channelid);
	cwtype = HwSlicChipType(channelid);
	HwSlicInitializeIndirectRegisters(channelid);
	
	if ( cwtype ==0) // Si3210 not the Si3211 or Si3212	
	{
		writeDirectReg(67,0x17,channelid); // Make VBat switch not automatic 
	// The above is a saftey measure to prevent Q7 from accidentaly turning on and burning out.
	//  It works in combination with the statement below.  Pin 34 DCDRV which is used for the battery switch on the
	//  Si3211 & Si3212 
		writeDirectReg(66,1,channelid);  //    Q7 should be set to OFF for si3210
	}
	if (cwversion <=2 )  //  REVISION B   
		writeDirectReg(73,4,channelid);  // set common mode voltage to 6 volts
	
	/* Do Flush durring HwSlicPowerUp and calibrate */

	if (cwtype == 0 || cwtype==3) //  Si3210
	{
		HwSlicPowerUp(channelid);  // Turn on the DC-DC converter and verify voltage.
		HwSlicPowerLeakTest(channelid); // Check for power leaks
		HwSlicPowerUp(channelid); // Turn on the DC-DC converter again
	}
	//printf("InitializeDirectRegisters ok\n");
	HwSlicInitializeDirectRegisters(channelid);
	HwSlicCalibrate(channelid);
	//printf("ICalibrate ok\n");
	sleep(1);
	HwSlicClearInterrupts(channelid);
	
	HwSlicGoActive(channelid);


}


VOID stopRinging(ULONG channelid)
{
	
	 if ((0xf & readDirectReg(0,channelid))<=2 )  // if REVISION B  
	 	writeDirectReg(69,10,channelid);   // Loop Debounce Register  = initial value
    
	HwSlicGoActive(channelid);
	
}

WORD manualCalibrate(ULONG channelid)
{ 
	BYTE x,y,i,cwprogress=0; 
	// progress contains individual bits for the Tip and Ring Calibrations
	//Initialized DR 98 and 99 to get consistant results.
	// 98 and 99 are the results registers and the search should have same intial conditions.

	writeDirectReg(98,0x10,channelid); // This is necessary if the calibration occurs other than at HwSlicGpioReset time
	writeDirectReg(99,0x10,channelid);

	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(98,i,channelid);
		sleep(40);
		if((readDirectReg(88,channelid)) == 0)
		{	
			cwprogress|=1;
			x=i;
			break;
		}
	} // for

	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(99,i,channelid);
		sleep(40);
		if((readDirectReg(89,channelid)) == 0){
			cwprogress|=2;
			y=i;
		break;
		}
	
	}//for

	return cwprogress;

}
 
LONG groundShort(ULONG channelid)
{ 
	LONG rc;

	rc= ( (readDirectReg(80,channelid) < 2) || readDirectReg(81,channelid) < 2);
		
	if (rc) {
		;
		//exception(TIPoRrINGgROUNDsHORT);
		//dumpregs();
	}
	return rc;
}

VOID clearAlarmBits(ULONG channelid)
{
	writeDirectReg(19,0xFC,channelid); //Clear Alarm bits
}

unsigned long interruptBits (ULONG channelid) {
	// Determines which interrupt bit is set
	LONG  lwcount = 1;
	BYTE j = 0x01 ;
	union {
		BYTE reg_data[3];
		long interrupt_bits;
	} u ;
	u.interrupt_bits=0;
	
// ONLY CLEAR the ACTIVE INTERRUPT or YOU WILL CREATE CRITICAL SECTION ERROR of LEAVING
// THE TIME OPEN BETWEEN THE Nth BIT and the N+1thbit within the same BYTE.
// eg. if the inactive oscillators are firing at nearly the same time
// you would only see one.

	u.reg_data[0] = readDirectReg( 18,channelid);
	writeDirectReg( 18,u.reg_data[0],channelid);
	u.reg_data[1] = readDirectReg( 19,channelid);
	writeDirectReg( 19,u.reg_data[1],channelid );
	u.reg_data[2] = readDirectReg( 20,channelid);
	writeDirectReg( 20,u.reg_data[2],channelid);

	return u.interrupt_bits ;

}

VOID callerid(ULONG channelid)
{
	BYTE i;
	
	writeDirectReg(INTRPT_MASK3, 0,channelid);
  	writeDirectReg(INTRPT_STATUS3, 0xff,channelid);
	//writeDirectReg(INTRPT_MASK2, 0,channelid);
	writeDirectReg(INTRPT_MASK2, 0x2,channelid);	//for loop status detect
	writeDirectReg(INTRPT_STATUS2, 0xff,channelid);
	writeDirectReg(INTRPT_MASK1, 0,channelid);
 	writeDirectReg(INTRPT_STATUS1,0xff,channelid);

	//sleep (250);  // 250 millisecond spacing between the ringing and the caller id

	sendProSLICID(channelid);

	chipData[channelid].eventEnable = 1;

	//disableOscillators(channelid);
		
	for ( i=32; i<=45; i++) 
		if (i !=34)  // Don't write to the ringing oscillator control
		writeDirectReg(i,0,channelid);


	writeDirectReg(INTRPT_MASK3, 0xff,channelid);
	writeDirectReg(INTRPT_STATUS3, 0xff,channelid);
	writeDirectReg(INTRPT_MASK2, 0xff,channelid);
	writeDirectReg(INTRPT_STATUS2, 0xff,channelid);	
	writeDirectReg(INTRPT_MASK1, 0xff,channelid);
	writeDirectReg(INTRPT_STATUS1,0xff,channelid);
}

VOID activateRinging(ULONG channelid)
{	
	
	writeDirectReg( LINE_STATE, RING_LINE,channelid); // REG 64,4
	
}

VOID converterOn(ULONG channelid)
{
	writeDirectReg(14,	0,channelid);
}

VOID disableOscillators(ULONG channelid) 
{ 
	// Turns of OSC1 and OSC2
	BYTE i;
	//printf("Disabling Oscillators!!!\n");
	for ( i=32; i<=45; i++) 
		if (i !=34)  // Don't write to the ringing oscillator control
		writeDirectReg(i,0,channelid);
	writeDirectReg(LINE_STATE, 0x1,channelid);	//go active 	comment 080102
	
}

VOID initializeLoopDebounceReg(ULONG channelid)
{
	writeDirectReg(69,10,channelid);   // Loop Debounce Register  = initial value
}

VOID setupNeon(ULONG channelid)
{
	while (readDirectReg(82,channelid) < (60/1.5 - 3));
	writeDirectReg(72,0x3F,channelid);//  High Neon
}

VOID printLoopState(ULONG channelid)
{
	printf(readDirectReg(68,channelid)&4?"On hook":"Off hook");
}

BYTE GetStatus(ULONG channelid)
{
	return (readDirectReg(68,channelid)&4 ? 'n':'f');
}

BYTE loopStatus(ULONG channelid)
{
 	return (readDirectReg(68,channelid) & 0x3);

}

BYTE digit(ULONG channelid)
{
	return readDirectReg(24,channelid) & 0x0f;
}

LONG digit_valid(ULONG channelid)
{
	BYTE dtmf;

	//OSTimeDlyHMSM(0,0,0,45);

	dtmf = readDirectReg(24,channelid);

	if(dtmf & DIGIT_VALID)
	{
		return 1;
	}
	else	
		return 0;
}

LONG HwSlicCalibrate(ULONG channelid)
{ 
	BYTE x,y,progress=0; // progress contains individual bits for the Tip and Ring Calibrations

	BYTE   DRvalue;
	LONG timeOut = 0,nCalComplete,i=0;

/* Do Flush durring HwSlicPowerUp and calibrate */
	printf("channel %d calibrating...",channelid);

	writeDirectReg(21,DISABLE_ALL_DR21,channelid);//(0)  Disable all interupts in DR21
	writeDirectReg(22,DISABLE_ALL_DR22,channelid);//(0)	Disable all interupts in DR21
	writeDirectReg(23,DISABLE_ALL_DR23,channelid);//(0)	Disabel all interupts in DR21
	writeDirectReg(64,OPEN_DR64,channelid);//(0)
//printf("a1\n");
	writeDirectReg(97,STANDARD_CAL_DR97,channelid); //(0x18)Calibrations without the ADC and DAC offset and without common mode calibration.
	writeDirectReg(96,STANDARD_CAL_DR96,channelid); //(0x47)	Calibrate common mode and differential DAC mode DAC + ILIM
	sleep(10);
       do 
	{
        	DRvalue = readDirectReg(96,channelid);
            	nCalComplete = DRvalue==CAL_COMPLETE_DR96;// (0)  When Calibration completes DR 96 will be zero
            	if(i++>MAX_CAL_PERIOD)// (800) MS
            	{
			timeOut= 1;
			break;
            	}
		sleep(1);
	}while (nCalComplete&&!timeOut);
//printf("a2\n");
	if (timeOut)
	{
		printf("calibrate timeout 1! DR[96] = 0x%x \n",DRvalue);
		//return -1;
	}
//Initialized DR 98 and 99 to get consistant results.
// 98 and 99 are the results registers and the search should have same intial conditions.



/*******************************The following is the manual gain mismatch calibration****************************/
/*******************************This is also available as a function *******************************************/
//printf("a3\n");
	sleep(20);
//printf("a4\n");
	writeIndirectReg(88,0,channelid);
	writeIndirectReg(89,0,channelid);
	writeIndirectReg(90,0,channelid);
	writeIndirectReg(91,0,channelid);
	writeIndirectReg(92,0,channelid);
	writeIndirectReg(93,0,channelid);

	writeDirectReg(98,0x10,channelid); // This is necessary if the calibration occurs other than at HwSlicGpioReset time
	writeDirectReg(99,0x10,channelid);
//printf("b1\n");
	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(98,i,channelid);
		sleep(40);
		if((readDirectReg(88,channelid)) == 0)
		{	progress|=1;
			x=i;
			break;
		}
	} // for
//	printf("c1\n");
	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(99,i,channelid);
		
		sleep(40);
		if((readDirectReg(89,channelid)) == 0){
			progress|=2;
			y=i;
			break;
		}
	}//for

/*******************************The preceding is the manual gain mismatch calibration****************************/



/**********************************The following is the longitudinal Balance Cal***********************************/
	HwSlicGoActive(channelid);
 	if  (loopStatus(channelid) & 4)
	  	return ERRORCODE_LONGBALCAL ;
//	printf("c2\n");
	writeDirectReg(64,OPEN_DR64,channelid);
	
	writeDirectReg(23,ENB2_DR23,channelid);  // enable interrupt for the balance Cal
	writeDirectReg(97,BIT_CALCM_DR97,channelid); // this is a singular calibration bit for longitudinal calibration
	writeDirectReg(96,0x40,channelid);

	i = 0;
       while(readDirectReg(96,channelid) != 0 )
	 {
		if(i++>MAX_CAL_PERIOD)// (800) MS
            	{
			timeOut= 1;
			break;
            	}
		sleep(1);
	 }

	if (timeOut)
	{
		printf("calibrate timeout 2! DR[96] = 0x%x \n",DRvalue);
		//return -1;
	}
	
   	writeDirectReg(21,INIT_DR21,channelid);
    	writeDirectReg(22,INIT_DR22,channelid);
    	writeDirectReg(23,INIT_DR23,channelid);


/**********************************The preceding is the longitudinal Balance Cal***********************************/

	printf("Ok!\n");
	return(0);

}// End of calibration


VOID HwSlicInitializeIndirectRegisters(ULONG channelid)										
{										
	writeIndirectReg(	0	,	INIT_IR0		,channelid);	//	0x55C2	DTMF_ROW_0_PEAK
	writeIndirectReg(	1	,	INIT_IR1		,channelid);	//	0x51E6	DTMF_ROW_1_PEAK
	writeIndirectReg(	2	,	INIT_IR2		,channelid);	//	0x4B85	DTMF_ROW2_PEAK
	writeIndirectReg(	3	,	INIT_IR3		,channelid);	//	0x4937	DTMF_ROW3_PEAK
	writeIndirectReg(	4	,	INIT_IR4		,channelid);	//	0x3333	DTMF_COL1_PEAK
	writeIndirectReg(	5	,	INIT_IR5		,channelid);	//	0x0202	DTMF_FWD_TWIST
	writeIndirectReg(	6	,	INIT_IR6		,channelid);	//	0x0202	DTMF_RVS_TWIST
	writeIndirectReg(	7	,	INIT_IR7		,channelid);	//	0x0198	DTMF_ROW_RATIO
	writeIndirectReg(	8	,	INIT_IR8		,channelid);	//	0x0198	DTMF_COL_RATIO
	writeIndirectReg(	9	,	INIT_IR9		,channelid);	//	0x0611	DTMF_ROW_2ND_ARM
	writeIndirectReg(	10	,	INIT_IR10		,channelid);	//	0x0202	DTMF_COL_2ND_ARM
	writeIndirectReg(	11	,	INIT_IR11		,channelid);	//	0x00E5	DTMF_PWR_MIN_
	writeIndirectReg(	12	,	INIT_IR12		,channelid);	//	0x0A1C	DTMF_OT_LIM_TRES
	writeIndirectReg(	13	,	INIT_IR13		,channelid);	//	0x7b30	OSC1_COEF
	writeIndirectReg(	14	,	INIT_IR14		,channelid);	//	0x0063	OSC1X
	writeIndirectReg(	15	,	INIT_IR15		,channelid);	//	0x0000	OSC1Y
	writeIndirectReg(	16	,	INIT_IR16		,channelid);	//	0x7870	OSC2_COEF
	writeIndirectReg(	17	,	INIT_IR17		,channelid);	//	0x007d	OSC2X
	writeIndirectReg(	18	,	INIT_IR18		,channelid);	//	0x0000	OSC2Y
	writeIndirectReg(	19	,	INIT_IR19		,channelid);	//	0x0000	RING_V_OFF
	writeIndirectReg(	20	,	INIT_IR20		,channelid);	//	0x7EF0	RING_OSC
	writeIndirectReg(	21	,	INIT_IR21		,channelid);	//	0x0160	RING_X
	writeIndirectReg(	22	,	INIT_IR22		,channelid);	//	0x0000	RING_Y
	writeIndirectReg(	23	,	INIT_IR23		,channelid);	//	0x2000	PULSE_ENVEL
	writeIndirectReg(	24	,	INIT_IR24		,channelid);	//	0x2000	PULSE_X
	writeIndirectReg(	25	,	INIT_IR25		,channelid);	//	0x0000	PULSE_Y
	writeIndirectReg(	26	,	INIT_IR26		,channelid);	//	0x4000	RECV_DIGITAL_GAIN
	writeIndirectReg(	27	,	INIT_IR27		,channelid);	//	0x4000	XMIT_DIGITAL_GAIN
	writeIndirectReg(	28	,	INIT_IR28		,channelid);	//	0x1000	LOOP_CLOSE_TRES
	writeIndirectReg(	29	,	INIT_IR29		,channelid);	//	0x3600	RING_TRIP_TRES
	writeIndirectReg(	30	,	INIT_IR30		,channelid);	//	0x1000	COMMON_MIN_TRES
	writeIndirectReg(	31	,	INIT_IR31		,channelid);	//	0x0200	COMMON_MAX_TRES
	writeIndirectReg(	32	,	INIT_IR32		,channelid);	//	0x7c0  	PWR_ALARM_Q1Q2
	writeIndirectReg(	33	,	INIT_IR33		,channelid);	//	0x2600	PWR_ALARM_Q3Q4
	writeIndirectReg(	34	,	INIT_IR34		,channelid);	//	0x1B80	PWR_ALARM_Q5Q6
	writeIndirectReg(	35	,	INIT_IR35		,channelid);	//	0x8000	LOOP_CLSRE_FlTER
	writeIndirectReg(	36	,	INIT_IR36		,channelid);	//	0x0320	RING_TRIP_FILTER
	writeIndirectReg(	37	,	INIT_IR37		,channelid);	//	0x08c	TERM_LP_POLE_Q1Q2
	writeIndirectReg(	38	,	INIT_IR38		,channelid);	//	0x0100	TERM_LP_POLE_Q3Q4
	writeIndirectReg(	39	,	INIT_IR39		,channelid);	//	0x0010	TERM_LP_POLE_Q5Q6
	writeIndirectReg(	40	,	INIT_IR40		,channelid);	//	0x0C00	CM_BIAS_RINGING
	writeIndirectReg(	41	,	INIT_IR41		,channelid);	//	0x0C00	DCDC_MIN_V
	writeIndirectReg(	43	,	INIT_IR43		,channelid);	//	0x1000	LOOP_CLOSE_TRES Low
	writeIndirectReg(	99	,	INIT_IR99		,channelid);	//	0x00DA	FSK 0 FREQ PARAM
	writeIndirectReg(	100	,	INIT_IR100		,channelid);	//	0x6B60	FSK 0 AMPL PARAM
	writeIndirectReg(	101	,	INIT_IR101		,channelid);	//	0x0074	FSK 1 FREQ PARAM
	writeIndirectReg(	102	,	INIT_IR102		,channelid);	//	0x79C0	FSK 1 AMPl PARAM
	writeIndirectReg(	103	,	INIT_IR103		,channelid);	//	0x1120	FSK 0to1 SCALER
	writeIndirectReg(	104	,	INIT_IR104		,channelid);	//	0x3BE0	FSK 1to0 SCALER
	writeIndirectReg(	97	,	INIT_IR97		,channelid);	//	0x0000	TRASMIT_FILTER
}										

VOID HwSlicInitializeDirectRegisters(ULONG channelid)
{
	writeDirectReg(0,	INIT_DR0	,channelid);//0X00	Serial Interface
	writeDirectReg(1,	INIT_DR1	,channelid);//0X28	PCM Mode
	writeDirectReg(2,	INIT_DR2	,channelid);//0X00	PCM TX Clock Slot Low Byte (1 PCLK cycle/LSB)
	writeDirectReg(3,	INIT_DR3	,channelid);//0x00	PCM TX Clock Slot High Byte
	writeDirectReg(4,	INIT_DR4	,channelid);//0x00	PCM RX Clock Slot Low Byte (1 PCLK cycle/LSB)
	writeDirectReg(5,	INIT_DR5	,channelid);//0x00	PCM RX Clock Slot High Byte
	writeDirectReg(8,	INIT_DR8	,channelid);//0X00	Loopbacks (digital loopback default)
	writeDirectReg(9,	INIT_DR9	,channelid);//0x00	Transmit and receive path gain and control
	writeDirectReg(10,	INIT_DR10	,channelid);//0X28	Initialization Two-wire impedance (600  and enabled)
	writeDirectReg(11,	INIT_DR11	,channelid);//0x33	Transhybrid Balance/Four-wire Return Loss
	writeDirectReg(18,	INIT_DR18	,channelid);//0xff	Normal Oper. Interrupt Register 1 (clear with 0xFF)
	writeDirectReg(19,	INIT_DR19	,channelid);//0xff	Normal Oper. Interrupt Register 2 (clear with 0xFF)
	writeDirectReg(20,	INIT_DR20	,channelid);//0xff	Normal Oper. Interrupt Register 3 (clear with 0xFF)
	writeDirectReg(21,	INIT_DR21	,channelid);//0xff	Interrupt Mask 1
	writeDirectReg(22,	INIT_DR22	,channelid);//0xff	Initialization Interrupt Mask 2
	writeDirectReg(23,	INIT_DR23	,channelid);//0xff	 Initialization Interrupt Mask 3
	writeDirectReg(32,	INIT_DR32	,channelid);//0x00	Oper. Oscillator 1 Controltone generation
	writeDirectReg(33,	INIT_DR33	,channelid);//0x00	Oper. Oscillator 2 Controltone generation
	writeDirectReg(34,	INIT_DR34	,channelid);//0X18	34 0x22 0x00 Initialization Ringing Oscillator Control
	writeDirectReg(35,	INIT_DR35	,channelid);//0x00	Oper. Pulse Metering Oscillator Control
	writeDirectReg(36,	INIT_DR36	,channelid);//0x00	36 0x24 0x00 Initialization OSC1 Active Low Byte (125 탎/LSB)
	writeDirectReg(37,	INIT_DR37	,channelid);//0x00	37 0x25 0x00 Initialization OSC1 Active High Byte (125 탎/LSB)
	writeDirectReg(38,	INIT_DR38	,channelid);//0x00	38 0x26 0x00 Initialization OSC1 Inactive Low Byte (125 탎/LSB)
	writeDirectReg(39,	INIT_DR39	,channelid);//0x00	39 0x27 0x00 Initialization OSC1 Inactive High Byte (125 탎/LSB)
	writeDirectReg(40,	INIT_DR40	,channelid);//0x00	40 0x28 0x00 Initialization OSC2 Active Low Byte (125 탎/LSB)
	writeDirectReg(41,	INIT_DR41	,channelid);//0x00	41 0x29 0x00 Initialization OSC2 Active High Byte (125 탎/LSB)
	writeDirectReg(42,	INIT_DR42	,channelid);//0x00	42 0x2A 0x00 Initialization OSC2 Inactive Low Byte (125 탎/LSB)
	writeDirectReg(43,	INIT_DR43	,channelid);//0x00	43 0x2B 0x00 Initialization OSC2 Inactive High Byte (125 탎/LSB)
	writeDirectReg(44,	INIT_DR44	,channelid);//0x00	44 0x2C 0x00 Initialization Pulse Metering Active Low Byte (125 탎/LSB)
	writeDirectReg(45,	INIT_DR45	,channelid);//0x00	45 0x2D 0x00 Initialization Pulse Metering Active High Byte (125 탎/LSB)
	writeDirectReg(46,	INIT_DR46	,channelid);//0x00	46 0x2E 0x00 Initialization Pulse Metering Inactive Low Byte (125 탎/LSB)
	writeDirectReg(47,	INIT_DR47	,channelid);//0x00	47 0x2F 0x00 Initialization Pulse Metering Inactive High Byte (125 탎/LSB)
	writeDirectReg(48,	INIT_DR48	,channelid);//0X80	48 0x30 0x00 0x80 Initialization Ringing Osc. Active Timer Low Byte (2 s,125 탎/LSB)
	writeDirectReg(49,	INIT_DR49	,channelid);//0X3E	49 0x31 0x00 0x3E Initialization Ringing Osc. Active Timer High Byte (2 s,125 탎/LSB)
	writeDirectReg(50,	INIT_DR50	,channelid);//0X00	50 0x32 0x00 0x00 Initialization Ringing Osc. Inactive Timer Low Byte (4 s, 125 탎/LSB)
	writeDirectReg(51,	INIT_DR51	,channelid);//0X7D	51 0x33 0x00 0x7D Initialization Ringing Osc. Inactive Timer High Byte (4 s, 125 탎/LSB)
	writeDirectReg(52,	INIT_DR52	,channelid);//0X00	52 0x34 0x00 Normal Oper. FSK Data Bit
	writeDirectReg(63,	INIT_DR63	,channelid);//0X54	63 0x3F 0x54 Initialization Ringing Mode Loop Closure Debounce Interval
	writeDirectReg(64,	INIT_DR64	,channelid);//0x00	64 0x40 0x00 Normal Oper. Mode Byte뾭rimary control
	writeDirectReg(65,	INIT_DR65	,channelid);//0X61	65 0x41 0x61 Initialization External Bipolar Transistor Settings
	writeDirectReg(66,	INIT_DR66	,channelid);//0X03	66 0x42 0x03 Initialization Battery Control
	writeDirectReg(67,	INIT_DR67	,channelid);//0X1F	67 0x43 0x1F Initialization Automatic/Manual Control
	writeDirectReg(69,	INIT_DR69	,channelid);//0X0C	69 0x45 0x0A 0x0C Initialization Loop Closure Debounce Interval (1.25 ms/LSB)
	writeDirectReg(70,	INIT_DR70	,channelid);//0X0A	70 0x46 0x0A Initialization Ring Trip Debounce Interval (1.25 ms/LSB)
	writeDirectReg(71,	INIT_DR71	,channelid);//0X01	71 0x47 0x00 0x01 Initialization Off-Hook Loop Current Limit (20 mA + 3 mA/LSB)
	writeDirectReg(72,	INIT_DR72	,channelid);//0X20	72 0x48 0x20 Initialization On-Hook Voltage (open circuit voltage) = 48 V(1.5 V/LSB)
	writeDirectReg(73,	INIT_DR73	,channelid);//0X02	73 0x49 0x02 Initialization Common Mode VoltageVCM = 3 V(1.5 V/LSB)
	writeDirectReg(74,	INIT_DR74	,channelid);//0X32	74 0x4A 0x32 Initialization VBATH (ringing) = 75 V (1.5 V/LSB)
	writeDirectReg(75,	INIT_DR75	,channelid);//0X10	75 0x4B 0x10 Initialization VBATL (off-hook) = 24 V (TRACK = 0)(1.5 V/LSB)
	if (HwSlicChipType(channelid) != 3)
		writeDirectReg(92,	INIT_DR92	,channelid);//0x7f	92 0x5C 0xFF 7F Initialization DCDC Converter PWM Period (61.035 ns/LSB)
	else
	writeDirectReg(92,	INIT_SI3210M_DR92	,channelid);//0x7f	92 0x5C 0xFF 7F Initialization DCDC Converter PWM Period (61.035 ns/LSB)

	writeDirectReg(93,	INIT_DR93	,channelid);//0x14	93 0x5D 0x14 0x19 Initialization DCDC Converter Min. Off Time (61.035 ns/LSB)
	writeDirectReg(94,	INIT_DR94	,channelid);	//stone added for debug
	writeDirectReg(96,	INIT_DR96	,channelid);//0x00	96 0x60 0x1F Initialization Calibration Control Register 1(written second and starts calibration)
	writeDirectReg(97,	INIT_DR97	,channelid);//0X1F	97 0x61 0x1F Initialization Calibration Control Register 2(written before Register 96)
	writeDirectReg(98,	INIT_DR98	,channelid);//0X10	98 0x62 0x10 Informative Calibration result (see data sheet)
	writeDirectReg(99,	INIT_DR99	,channelid);//0X10	99 0x63 0x10 Informative Calibration result (see data sheet)
	writeDirectReg(100,	INIT_DR100	,channelid	);//0X11	100 0x64 0x11 Informative Calibration result (see data sheet)
	writeDirectReg(101,	INIT_DR101	,channelid);//0X11	101 0x65 0x11 Informative Calibration result (see data sheet)
	writeDirectReg(102,	INIT_DR102	,channelid);//0x08	102 0x66 0x08 Informative Calibration result (see data sheet)
	writeDirectReg(103,	INIT_DR103	,channelid);//0x88	103 0x67 0x88 Informative Calibration result (see data sheet)
	writeDirectReg(104,	INIT_DR104	,channelid);//0x00	104 0x68 0x00 Informative Calibration result (see data sheet)
	writeDirectReg(105,	INIT_DR105	,channelid);//0x00	105 0x69 0x00 Informative Calibration result (see data sheet)
	writeDirectReg(106,	INIT_DR106	,channelid);//0x20	106 0x6A 0x20 Informative Calibration result (see data sheet)
	writeDirectReg(107,	INIT_DR107	,channelid);//0x08	107 0x6B 0x08 Informative Calibration result (see data sheet)
	writeDirectReg(108,	INIT_DR108	,channelid);//0xEB	108 0x63 0x00 0xEB Initialization Feature enhancement register
}
   


VOID HwSlicClearInterrupts(ULONG channelid)
{
	writeDirectReg(	18	,	INIT_DR18	,channelid);//0xff	Normal Oper. Interrupt Register 1 (clear with 0xFF)
	writeDirectReg(	19	,	INIT_DR19	,channelid);//0xff	Normal Oper. Interrupt Register 2 (clear with 0xFF)
	writeDirectReg(	20	,	INIT_DR20	,channelid);//0xff	Normal Oper. Interrupt Register 3 (clear with 0xFF)
}

VOID ResetAndInitialize909(ULONG channelid)
{
	

	LONG i=0, j=0  ; 
	LONG startTime = 0;
	
	HwSlicInitialize(channelid);  /* initialize the ProSLIC */
	
	writeDirectReg(72,0,channelid); // set Voc to 0
	writeDirectReg(64,FORWARD_ACTIVE_DR64,channelid); // le
	sleep(30);
	writeDirectReg(64,OPEN_DR64,channelid);
	writeDirectReg(72,INIT_DR72,channelid);

}

LONG verifyIndirectReg(BYTE address, WORD should_be_value,ULONG channelid)
{ 
	LONG error_flag ;
	WORD value;
		value = readIndirectReg(address,channelid);
		error_flag = (should_be_value != value);
		
		if ( error_flag )
		{
			printf("\n   iREG %d = %X  should be %X ",address,value,should_be_value );			
		}	
		return error_flag;
}


LONG HwSlicVerifyIndirectRegisters(ULONG channelid)										
{		
	LONG error=0;

	error |= verifyIndirectReg(	0	,	INIT_IR0		,channelid);	//	0x55C2	DTMF_ROW_0_PEAK
	error |= verifyIndirectReg(	1	,	INIT_IR1		,channelid);	//	0x51E6	DTMF_ROW_1_PEAK
	error |= verifyIndirectReg(	2	,	INIT_IR2		,channelid);	//	0x4B85	DTMF_ROW2_PEAK
	error |= verifyIndirectReg(	3	,	INIT_IR3		,channelid);	//	0x4937	DTMF_ROW3_PEAK
	error |= verifyIndirectReg(	4	,	INIT_IR4		,channelid);	//	0x3333	DTMF_COL1_PEAK
	error |= verifyIndirectReg(	5	,	INIT_IR5		,channelid);	//	0x0202	DTMF_FWD_TWIST
	error |= verifyIndirectReg(	6	,	INIT_IR6		,channelid);	//	0x0202	DTMF_RVS_TWIST
	error |= verifyIndirectReg(	7	,	INIT_IR7		,channelid);	//	0x0198	DTMF_ROW_RATIO
	error |= verifyIndirectReg(	8	,	INIT_IR8		,channelid);	//	0x0198	DTMF_COL_RATIO
	error |= verifyIndirectReg(	9	,	INIT_IR9		,channelid);	//	0x0611	DTMF_ROW_2ND_ARM
	error |= verifyIndirectReg(	10	,	INIT_IR10		,channelid);	//	0x0202	DTMF_COL_2ND_ARM
	error |= verifyIndirectReg(	11	,	INIT_IR11		,channelid);	//	0x00E5	DTMF_PWR_MIN_
	error |= verifyIndirectReg(	12	,	INIT_IR12		,channelid);	//	0x0A1C	DTMF_OT_LIM_TRES
	error |= verifyIndirectReg(	13	,	INIT_IR13		,channelid);	//	0x7b30	OSC1_COEF
	error |= verifyIndirectReg(	14	,	INIT_IR14		,channelid);	//	0x0063	OSC1X
	error |= verifyIndirectReg(	15	,	INIT_IR15		,channelid);	//	0x0000	OSC1Y
	error |= verifyIndirectReg(	16	,	INIT_IR16		,channelid);	//	0x7870	OSC2_COEF
	error |= verifyIndirectReg(	17	,	INIT_IR17		,channelid);	//	0x007d	OSC2X
	error |= verifyIndirectReg(	18	,	INIT_IR18		,channelid);	//	0x0000	OSC2Y
	error |= verifyIndirectReg(	19	,	INIT_IR19		,channelid);	//	0x0000	RING_V_OFF
	error |= verifyIndirectReg(	20	,	INIT_IR20		,channelid);	//	0x7EF0	RING_OSC
	error |= verifyIndirectReg(	21	,	INIT_IR21		,channelid);	//	0x0160	RING_X
	error |= verifyIndirectReg(	22	,	INIT_IR22		,channelid);	//	0x0000	RING_Y
	error |= verifyIndirectReg(	23	,	INIT_IR23		,channelid);	//	0x2000	PULSE_ENVEL
	error |= verifyIndirectReg(	24	,	INIT_IR24		,channelid);	//	0x2000	PULSE_X
	error |= verifyIndirectReg(	25	,	INIT_IR25		,channelid);	//	0x0000	PULSE_Y
	error |= verifyIndirectReg(	26	,	INIT_IR26		,channelid);	//	0x4000	RECV_DIGITAL_GAIN
	error |= verifyIndirectReg(	27	,	INIT_IR27		,channelid);	//	0x4000	XMIT_DIGITAL_GAIN
	error |= verifyIndirectReg(	28	,	INIT_IR28		,channelid);	//	0x1000	LOOP_CLOSE_TRES
	error |= verifyIndirectReg(	29	,	INIT_IR29		,channelid);	//	0x3600	RING_TRIP_TRES
	error |= verifyIndirectReg(	30	,	INIT_IR30		,channelid);	//	0x1000	COMMON_MIN_TRES
	error |= verifyIndirectReg(	31	,	INIT_IR31		,channelid);	//	0x0200	COMMON_MAX_TRES
	error |= verifyIndirectReg(	32	,	INIT_IR32		,channelid);	//	0x7c0  	PWR_ALARM_Q1Q2
	error |= verifyIndirectReg(	33	,	INIT_IR33		,channelid);	//	0x2600	PWR_ALARM_Q3Q4
	error |= verifyIndirectReg(	34	,	INIT_IR34		,channelid);	//	0x1B80	PWR_ALARM_Q5Q6
	error |= verifyIndirectReg(	35	,	INIT_IR35		,channelid);	//	0x8000	LOOP_CLSRE_FlTER
	error |= verifyIndirectReg(	36	,	INIT_IR36		,channelid);	//	0x0320	RING_TRIP_FILTER
	error |= verifyIndirectReg(	37	,	INIT_IR37		,channelid);	//	0x08c	TERM_LP_POLE_Q1Q2
	error |= verifyIndirectReg(	38	,	INIT_IR38		,channelid);	//	0x0100	TERM_LP_POLE_Q3Q4
	error |= verifyIndirectReg(	39	,	INIT_IR39		,channelid);	//	0x0010	TERM_LP_POLE_Q5Q6
	error |= verifyIndirectReg(	40	,	INIT_IR40		,channelid);	//	0x0C00	CM_BIAS_RINGING
	error |= verifyIndirectReg(	41	,	INIT_IR41		,channelid);	//	0x0C00	DCDC_MIN_V
	error |= verifyIndirectReg(	43	,	INIT_IR43		,channelid);	//	0x1000	LOOP_CLOSE_TRES Low
	error |= verifyIndirectReg(	99	,	INIT_IR99		,channelid);	//	0x00DA	FSK 0 FREQ PARAM
	error |= verifyIndirectReg(	100	,	INIT_IR100		,channelid);	//	0x6B60	FSK 0 AMPL PARAM
	error |= verifyIndirectReg(	101	,	INIT_IR101		,channelid);	//	0x0074	FSK 1 FREQ PARAM
	error |= verifyIndirectReg(	102	,	INIT_IR102		,channelid);	//	0x79C0	FSK 1 AMPl PARAM
	error |= verifyIndirectReg(	103	,	INIT_IR103		,channelid);	//	0x1120	FSK 0to1 SCALER
	error |= verifyIndirectReg(	104	,	INIT_IR104		,channelid);	//	0x3BE0	FSK 1to0 SCALER
	
	return error;
}										

#endif

