#include "ftuart.h"
#include "clock_controller.h"

VOID HwUartEnableInterrupt(LONG lwUartId,LONG lwInterruptSource)
{
	LONG lwUartBaseAddr;
	BYTE cwData;
	
	if( !lwUartId ) lwUartBaseAddr = UART0_BASE_ADDR;
	else if( lwUartId == 1 ) lwUartBaseAddr = UART1_BASE_ADDR;
	else return;
	
	cwData = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartIER));
	cwData |= lwInterruptSource;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartIER));
	
	
}

VOID HwUartDisableInterrupt(LONG lwUartId,LONG lwInterruptSource)
{
	LONG lwUartBaseAddr;
	BYTE cwData;
	
	if( !lwUartId ) lwUartBaseAddr = UART0_BASE_ADDR;
	else if( lwUartId == 1 ) lwUartBaseAddr = UART1_BASE_ADDR;
	else return;

	cwData = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartIER));
	cwData &= ~lwInterruptSource;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartIER));
}

inline LONG HwComputeBaudRate(ULONG io_irda_uclk,ULONG baudrate)
{//psr:bit[24:16],dlm:bit[15:8],dll:bit[7:0]
	LONG psr = 1,flag;
	ULONG temp,temp_baudrate,min_baudrate,max_baudrate,temp_y;
	
	io_irda_uclk = io_irda_uclk / 16;
	
//	temp_baudrate = baudrate*5/100;
//	min_baudrate = baudrate - temp_baudrate;
//	max_baudrate = baudrate + temp_baudrate;
	
	do{
		flag = 0;
		io_irda_uclk = io_irda_uclk / psr;
		temp = io_irda_uclk / baudrate;
		temp_y = 2*(io_irda_uclk % baudrate);
		if( temp_y > baudrate ) temp++;
		if( temp >= 0x10000 ) {
			psr = (temp / 0x10000) + 1;	
			flag = 1;
		}

	}while( flag );

//	temp_baudrate = io_irda_uclk / (temp*psr);
//	if( temp_baudrate < min_baudrate || temp_baudrate > max_baudrate ) return 0;	
			
	return ( ((psr<<16)|temp) & 0xffffff );	
}


BOOL HwUartSetBaudRate(LONG lwUartId,ULONG lwBaudRate)
{
	LONG lwRegValues = HwComputeBaudRate(UART_UCLK,lwBaudRate);
	BYTE cwData,cwLCR;
	LONG lwUartBaseAddr;
	
	if( lwRegValues <= 0xffff ) return FALSE;

	if( !lwUartId ) lwUartBaseAddr = UART0_BASE_ADDR;
	else if( lwUartId == 1 ) lwUartBaseAddr = UART1_BASE_ADDR;
	else return FALSE;
	
	cwLCR = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLCR)) & 0x7f;
	wrb(0x80,UART_REGS_ADDR(lwUartBaseAddr,tUartLCR));
	cwData = lwRegValues >> 16;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartPSR));
	cwData = (lwRegValues >> 8) & 0xff;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartDLM));
	cwData = lwRegValues & 0xff;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartDLL));
	wrb(cwLCR,UART_REGS_ADDR(lwUartBaseAddr,tUartLCR));
	
	return TRUE;
	
}

VOID HwUartInitialization(LONG lwUartId,BYTE cwConfig)
{
	BYTE cwData;
	LONG lwData,lwUartBaseAddr = UART0_BASE_ADDR;
	
	
	HwCctlEnableUart(lwUartId);
	
	if( !lwUartId ) 
	{
		
		lwData = rdl(UART_REGS_ADDR(lwUartBaseAddr,tUart0CTL));
		lwData |= 0x6;
		wrl(lwData,UART_REGS_ADDR(lwUartBaseAddr,tUart0CTL));
		
		lwUartBaseAddr = UART0_BASE_ADDR;
	}
	else if( lwUartId == 1 ) 
	{
		lwData = rdl(UART_REGS_ADDR(lwUartBaseAddr,tUart1CTL));
		lwData |= 0x6;
		wrl(lwData,UART_REGS_ADDR(lwUartBaseAddr,tUart1CTL));
		
		lwUartBaseAddr = UART1_BASE_ADDR;
	}
	else return;

	cwData = cwConfig & 0x7f;
	wrb(cwData,UART_REGS_ADDR(lwUartBaseAddr,tUartLCR));
	wrb(0x6,UART_REGS_ADDR(lwUartBaseAddr,tUartFCR));
	wrb(0x59,UART_REGS_ADDR(lwUartBaseAddr,tUartFCR));
	wrb(0x0,UART_REGS_ADDR(lwUartBaseAddr,tUartMCR));
	wrb(0x10,UART_REGS_ADDR(lwUartBaseAddr,tUartMDR));
	wrb(0x3,UART_REGS_ADDR(lwUartBaseAddr,tUartACR));
	
	cwData = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR));
	
}

