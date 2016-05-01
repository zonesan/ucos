#include  "clock_controller.h"

BOOL HwCctlDisableBlock(BYTE cwBlockId)
{
	ULONG lwData;
	
	if( cwBlockId > 31 ) return FALSE;
	
	lwData = yCCTL_Interface->Block_Enable;
	lwData &= ~(1<<cwBlockId);
	yCCTL_Interface->Block_Enable = lwData;
	
	return TRUE;
}

BOOL HwCctlEnableBlock(BYTE cwBlockId)
{
	ULONG lwData;
	
	if( cwBlockId > 31 ) return FALSE;
	
	lwData = yCCTL_Interface->Block_Enable;
	lwData |= (1<<cwBlockId);
	yCCTL_Interface->Block_Enable = lwData;
	
	return TRUE;
}
VOID HwCctlEnableAllBlock(VOID)
{
	yCCTL_Interface->Block_Enable = 0xffffffff;
}

VOID HwCctlDisableAllBlock(VOID)
{
	yCCTL_Interface->Block_Enable = 0x0;
}

BOOL HwCctlResetBlock(BYTE cwBlockId)
{
	ULONG lwData;
	int i;
	
	if( cwBlockId > 31 ) return FALSE;
	
	lwData = (1<<cwBlockId);
	yCCTL_Interface->Block_Reset = lwData;
	
	for(i=0;i<1000;i++);
	
	yCCTL_Interface->Block_Reset = 0;
	
	return TRUE;
}

BOOL HwCctlSetClock(BYTE cwClockId,BYTE cwPllSource,BYTE cwDividerType,LONG lwDividerValue)
{
	ULONG lwData = 0x80000000;
	
	if( lwDividerValue > 0x10000000 ) return FALSE;
	else lwData |= lwDividerValue;
	if( cwPllSource == tPll_Risc ) lwData |= 0x40000000;
	else if( cwPllSource == tPll_Sdram ) lwData |= 0x0;
	else return FALSE;
	
	if( cwDividerType == tDivider_Accurate ) lwData |= 0x20000000;
	else if( cwDividerType == tDivider_Regular ) lwData |= 0x0;
	else return FALSE;
	
	switch(cwClockId)
	{
		case tClock_Audio:
			yCCTL_Interface->Audio_Clock = lwData;
		break;
		case tClock_Emac:
			yCCTL_Interface->Emac_Clock = lwData;
		break;
		case tClock_Vout:
			yCCTL_Interface->Vout_Clock = lwData;
		break;
		case tClock_Vin:
			yCCTL_Interface->Vin_Clock = lwData;
		break;
		case tClock_Vcodec:
			yCCTL_Interface->Vcodec_Clock = lwData;
		break;
		case tClock_Mdio:
			if( lwDividerValue > 0xffff ) return FALSE;
			lwData &= 0x8000ffff;
			yCCTL_Interface->Mdio_Clock = lwData;
		break;
		case tClock_Host:
			if( lwDividerValue > 0xffff ) return FALSE;
			lwData &= 0xc000ffff;
			yCCTL_Interface->Host_Clock = lwData;
		break;
		default:
			return FALSE;
	}
	return TRUE;	
}



VOID HwCctlEnableUart(LONG lwUartId)
{
	ULONG lwData;
	
	lwData = yCCTL_Interface->Pin_Mux_Ctl;
	
	if( !lwUartId )
	{
		lwData |= 0x1;
		HwCctlEnableBlock(tCctl_Uart0);
	}
	else 
	{
		lwData |= 0x20000000;
		HwCctlEnableBlock(tCctl_Uart1);
	}

	yCCTL_Interface->Pin_Mux_Ctl = lwData;
}

VOID HwCctlEnableEmac(VOID)
{//lwType=0:MII,lwType=1:RMII
	HwCctlEnableBlock(tCctl_Emac0);
	HwCctlEnableBlock(tCctl_Emac1);
	HwCctlSetClock(tClock_Mdio,tPll_Sdram,tDivider_Regular,0x0);
	HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0xa);
}

VOID HwCctlClockEmac(LONG lwType,LONG lwSpeed)
{
	if( lwType )//RMII
	{
		HwCctlSetClock(tClock_Mdio,tPll_Sdram,tDivider_Regular,0x5);
		//HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0x0);
	}
	else
	{
		HwCctlSetClock(tClock_Mdio,tPll_Sdram,tDivider_Regular,0x0);
		//if( lwSpeed & 0x2000 ) HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0x0);
		//else HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0xa);
	}
	if( lwSpeed & 0x2000 ) HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0x0);
	else HwCctlSetClock(tClock_Emac,tPll_Sdram,tDivider_Regular,0x4);
}

VOID HwCctlInitialization(VOID)
{
	HwCctlEnableAllBlock();
	HwCctlEnableEmac();
}
