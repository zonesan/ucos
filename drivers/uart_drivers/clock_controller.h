#ifndef _clock_controller_h
#define _clock_controller_h

#include "vb1000.h"


typedef struct
{
	unsigned int Boot_Load_Ctl;
	unsigned int Pll_Timer;
	unsigned int Risc_Pll;
	unsigned int Sdram_Pll;
	unsigned int Pin_Mux_Ctl;
	unsigned int Audio_Clock;
	unsigned int Emac_Clock;
	unsigned int Mdio_Clock;
	unsigned int Host_Clock;
	unsigned int Risc_Sdram_Clock;
	unsigned int Vout_Clock;
	unsigned int Vin_Clock;
	unsigned int Vcodec_Clock;
	unsigned int Block_Enable;
	unsigned int Block_Reset;
	unsigned int Cb_Debug_Out;
}tCCTL_Regs;

#define yCCTL_Interface ((volatile tCCTL_Regs *) BLOCK_BASE_ADDR(CCTL_BLOCK))

typedef enum tCctl_Clock
{
	tClock_Audio,
	tClock_Emac,
	tClock_Mdio,
	tClock_Host,
	tClock_Vout,
	tClock_Vin,
	tClock_Vcodec,
}tCctl_Clock;

typedef enum tCctl_Pll
{
	tPll_Sdram,
	tPll_Risc,
	tPll_Crystal,
	tPll_Crystal2x,
}tCctl_Pll;

typedef enum tCctl_Divider
{
	tDivider_Regular,
	tDivider_Accurate,
}tCctl_Divider;

typedef enum tCctl_Block
{
	tCctl_Risc0 = 0x0,
	tCctl_Risc1,
	tCctl_Mc,
	tCctl_Sif,
	tCctl_Sc,
	tCctl_Vin,
	tCctl_Vout,
	tCctl_Audio,
	tCctl_Dmac,
	tCctl_Ppe,
	tCctl_Pif,
	tCctl_Vsc,
	tCctl_Emac0,
	tCctl_Emac1,
	tCctl_Sec,
	tCctl_Sd0,
	tCctl_Sd1,
	tCctl_Uart0,
	tCctl_Uart1,
	tCctl_Venc,
	tCctl_Vdec,
	tCctl_Ide,
	tCctl_Rtc,
	tCctl_Wd,
	
}tCctl_Block;

/*
*********************************************************************************************************
*                              FUCTION PROTYPES
*********************************************************************************************************
*/

//VOID HwCctlInitialization(VOID);
//VOID HwCctlClockEmac(LONG lwType,LONG lwSpeed);
//VOID HwCctlEnableEmac(VOID);
void HwCctlEnableUart(LONG lwUartId);

#endif
