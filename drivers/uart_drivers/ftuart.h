#ifndef __UART_H
#define __UART_H


#include "vb1000.h"
#include "cctl.h"

#define DEFAULT_UART 0x0

#define UART_UCLK    (CPU_MAIN_CLOCK / 2)
#define UART0_BASE_ADDR (BLOCK_BASE_ADDR(UART_BLOCK))
#define UART1_BASE_ADDR (UART0_BASE_ADDR+0x180)
#define UART_REGS_ADDR(UART_BASE_ADDR,REGS) (UART_BASE_ADDR+REGS)

/*
 Description:UART register address offset
*/
typedef enum tUartCoreRegs
{
	tUartRBR	= 0x00,	//Receive Buffer Register
	tUartTHR	= 0x00, //Transmitter Holding Register
	tUartIER	= 0x04,	//Interrupt Enable Register
	tUartIIR	= 0x08,	//Interrupt Identification Register
	tUartFCR	= 0x08,	//FIFO Control Register
	tUartLCR	= 0x0C,	//Line Control Register
	tUartMCR	= 0x10,	//Modem Control Register
	tUartLSR	= 0x14,	//Line Status Register
	tUartTST	= 0x14,	//Testing Register
	tUartMSR	= 0x18,	//Modem Status Register
	tUartSPR	= 0x1C,	//Scrath Pad Register

/*
 Registers accessible when DLAB=1
*/

	tUartDLL	= 0x00,	//Baud Rate Divisor Latch Least Siginificant Byte
	tUartDLM	= 0x04,	//Baud Rate Divisor Latch Most Siginificant Byte
	tUartPSR	= 0x08,	//Present Register

/*
 Infrared FIR Mode
*/

 	tUartMDR	= 0x20,	//Mode Definition Register
	tUartACR	= 0x24,	//Auxiliary Control Register
	tUartTXLENL	= 0x28,	//Transmitter Frame Length Low
	tUartTXLENH	= 0x2C,	//Transmitter Frame Length High
	tUartMRXLENL	= 0x30,	//Maximum Receicer Frame Length Low
	tUartMRXLENH	= 0x34,	//Maximum Receiver Frame Length High
	tUartPLR	= 0x38,	//FIR Preamble Length Register
	tUartFMIIR_PIO	= 0x3C,	//FIR Mode Interrupt Identification Register in PIO Mode
	tUartFMIIR_DMA	= 0x3C,	//FIR Mode Interrupt Identification Register in DMA Mode
	tUartFMIIER_PIO	= 0x40,	//FIR Mode Interrupt Enable Register for PIO Mode
	tUartFMIIER_DMA	= 0x40,	//FIR Mode Interrupt Enable Register for DMA Mode
	tUartSTFF_STS	= 0x44,	//Status FIFO Line Status Register
	tUartSTFF_RXLENL= 0x48,	//Status FIFO Received Frame Length Register Low
	tUartSTFF_RXLENH= 0x4C,	//Status FIFO Received Frame Length Register High
	tUartFMLSR	= 0x50,	//FIR Mode Link Status Register
	tUartFMSIER	= 0x54,	//FIR Mode Link Status Interrupt Enable Register
	tUartRSR	= 0x58,	//Resume Register
	tUartRXFF_CNTR	= 0x5C,	//Rx FIFO Count Register
	tUartLSTFMLENL	= 0x60,	//Last Frame Length Register Low
	tUartLSTFMLENH	= 0x64,	//Last Frame Length Register High
	tUartFEATURE	= 0x68,	//Feature Register
	tUartREVD1	= 0x6C,	//Revision Register Digit 1
	tUartREVD2	= 0x70,	//Revision Register Digit 2
	tUartREVD3	= 0x74	//Revision Register Digit 3
	
}tUartCoreRegs;

typedef enum tUartRegs
{
	tUart0CTL	= 0x80,	//this register is for generic control of UART0
	tUart1CTL	= 0x84 //this register is for generic control of UART1
}tUartRegs;

typedef enum tConfigValues
{
	tConfig_8Bit		=0x3,	// 8 bits per character.
	tConfig_7Bit		=0x2,	// 7 bits per character.
	tConfig_6Bit		=0x1,	// 6 bits per character.
	tConfig_5Bit		=0x0,	// 5 bits per character.
	tConfig_1StopBit	=0x0,	// one stop bit is added to each character.
	tConfig_2StopBit	=0x4,	// two stop bits are added to each character.
	tConfig_Parity		=0x8,	// a parity bit is added to each character.
	tConfig_NoParity	=0x0,	// no parity bit is added.
	tConfig_EvenParity	=0x10,	// even parity is specified.
	tConfig_OddParity	=0x0	// odd parity is specified.
}tConfigValues;


typedef enum tBaudRate
{
	tBaudRate_115200Bps		=115200,// data rate of 115200 bits per second.
	tBaudRate_76800Bps		=78600,	// data rate of 76800 bits per second.
	tBaudRate_56700Bps		=56700,	// data rate of 56700 bits per second.
	tBaudRate_38400Bps		=38400,	// data rate of 38400 bits per second.
	tBaudRate_19200Bps		=19200,	// data rate of 19200 bits per second.
	tBaudRate_14400Bps		=14400,	// data rate of 14400 bits per second.
	tBaudRate_9600Bps		=9600,	// data rate of 9600 bits per second.
	tBaudRate_2400Bps		=2400,	// data rate of 2400 bits per second.
	tBaudRate_1200Bps		=1200,	// data rate of 1200 bits per second.
	tBaudRate_110Bps		=110	// data rate of 110 bits per second.
	
}tBaudRate;

typedef enum tInterruptSource
{
	tInterruptSource_Modem	= 0x8, //MODEM Status
	tInterruptSource_RLS	= 0x4, //Receiver Line Status
	tInterruptSource_Tx	= 0x2, //THR Empty
	tInterruptSource_Rx	= 0x1  //Receiver Data Available
	
}tInterruptSource;


BOOL HwUartSetBaudRate(LONG lwUartId,ULONG lwBaudRate);
VOID HwUartInitialization(LONG lwUartId,BYTE cwConfig);
VOID HwUartSendData(LONG lwUartId,PBYTE pcwData,LONG lwLen);
BOOL HwUartReceiveChar(LONG lwUartId,PBYTE pcwData);
BOOL HwUartReceiveData(LONG lwUartId,PBYTE pcwData,PLONG plwLen);
VOID HwUartEnableInterrupt(LONG lwUartId,LONG lwInterruptSource);
VOID HwUartDisableInterrupt(LONG lwUartId,LONG lwInterruptSource);

#endif
