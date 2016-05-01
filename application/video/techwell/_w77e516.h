/*--------------------------------------------------------------------------
w77e516.H

Header file for Winbond W77C32, W77C58, W77E58, W77CE58
Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.




 and w77e516.. 
 modified by HongSoon Kim
 2006/05/06
--------------------------------------------------------------------------*/

#if 0
#ifndef __W77C32_H__
#define __W77C32_H__

/*  BYTE Registers  */
sfr P0     = 0x80;
sfr P1     = 0x90;
sfr P2     = 0xA0;
sfr P3     = 0xB0;
sfr PSW    = 0xD0;
sfr ACC    = 0xE0;
sfr B      = 0xF0;
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr PCON   = 0x87;
sfr TCON   = 0x88;
sfr TMOD   = 0x89;
sfr TL0    = 0x8A;
sfr TL1    = 0x8B;
sfr TH0    = 0x8C;
sfr TH1    = 0x8D;
sfr IE     = 0xA8;
sfr IP     = 0xB8;
sfr SCON   = 0x98;
sfr SBUF   = 0x99;
sfr SCON0  = 0x98;
sfr SBUF0  = 0x99;

/*  W77C32, W77C58 Extensions  */
sfr DPL1   = 0x84;
sfr DPH1   = 0x85;
sfr DPS    = 0x86;
sfr CKCON  = 0x8E;
sfr EXIF   = 0x91;
sfr P4     = 0xA5;
sfr SADDR  = 0xA9;
sfr SADDR1 = 0xAA;
sfr SADEN0 = 0xB9;
sfr SADEN1 = 0xBA;
sfr SCON1  = 0xC0;
sfr SBUF1  = 0xC1;
sfr ROMMAP = 0xC2;
sfr PMR    = 0xC4;
sfr STATUS = 0xC5;
sfr TA     = 0xC7;
sfr T2CON  = 0xC8;
sfr T2MOD  = 0xC9;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr WDCON  = 0xD8;
sfr EIE    = 0xE8;
sfr EIP    = 0xF8;

//////////////////////////////////////////
//	W77E516 Extensions by HongSoon Kim	//

//	SFR definition											//
sfr	P4CONA	= 0x92;
sfr	P4CONB	= 0x93;
sfr	P40AL		= 0x94;
sfr	P40AH		= 0x95;
sfr	P41AL		= 0x96;
sfr	P41AH		= 0x97;
sfr	P42AL		= 0x9A;
sfr	P42AH		= 0x9B;
sfr	P43AL		= 0x9C;
sfr	P43AH		= 0x9D;
sfr	CHPCON	= 0x9F;
sfr	P4CSIN	= 0xA2;
sfr	SFRAL		= 0xAC;
sfr	SFRAH		= 0xAD;
sfr	SFDFD		= 0xAE;
sfr	SFRCN		= 0xAF;
sfr	NVMSEL	= 0xC6;
sfr	NVMCON	= 0xCE;
sfr	NVMDAT	= 0xCF;

//	BIT address definition								//
//	P0
sbit P0_0  = P0^0;
sbit P0_1  = P0^1;
sbit P0_2  = P0^2;
sbit P0_3  = P0^3;
sbit P0_4  = P0^4;
sbit P0_5  = P0^5;
sbit P0_6  = P0^6;
sbit P0_7  = P0^7;
//	P1
sbit P1_0  = P1^0;
sbit P1_1  = P1^1;
sbit P1_2  = P1^2;
sbit P1_3  = P1^3;
sbit P1_4  = P1^4;
sbit P1_5  = P1^5;
sbit P1_6  = P1^6;
sbit P1_7  = P1^7;
//	P2
sbit P2_0  = P2^0;
sbit P2_1  = P2^1;
sbit P2_2  = P2^2;
sbit P2_3  = P2^3;
sbit P2_4  = P2^4;
sbit P2_5  = P2^5;
sbit P2_6  = P2^6;
sbit P2_7  = P2^7;
//	P3
sbit P3_0  = P3^0;
sbit P3_1  = P3^1;
sbit P3_2  = P3^2;
sbit P3_3  = P3^3;
sbit P3_4  = P3^4;
sbit P3_5  = P3^5;
sbit P3_6  = P3^6;
sbit P3_7  = P3^7;

////... hardware clear EINT2 to EINT5 flag
//sbit HC5 = T2MOD^7;		//... /EINT5
//sbit HC4 = T2MOD^6;		//... EINT4
//sbit HC3 = T2MOD^5;		//... /EINT3
//sbit HC2 = T2MOD^4;		//... EINT2

//////////////////////////////////////////
//////////////////////////////////////////

/*  BIT Registers  */
/*  PSW */
sbit CY    = PSW^7;
sbit AC    = PSW^6;
sbit F0    = PSW^5;
sbit RS1   = PSW^4;
sbit RS0   = PSW^3;
sbit OV    = PSW^2;
sbit FL    = PSW^1;
sbit P     = PSW^0;

/*  TCON  */
sbit TF1   = TCON^7;
sbit TR1   = TCON^6;
sbit TF0   = TCON^5;
sbit TR0   = TCON^4;
sbit IE1   = TCON^3;		//... external interrupt flag for EX1
sbit IT1   = TCON^2;
sbit IE0   = TCON^1;		//... external interrupt flag for EX0
sbit IT0   = TCON^0;

/*  IE  */
sbit EA    = IE^7;
sbit ES1   = IE^6;
sbit ET2   = IE^5;
sbit ES0   = IE^4;
sbit ET1   = IE^3;
sbit EX1   = IE^2;
sbit ET0   = IE^1;
sbit EX0   = IE^0;

/*  IP  */
sbit PS1   = IP^6;
sbit PT2   = IP^5;
sbit PS0   = IP^4;
sbit PT1   = IP^3;
sbit PX1   = IP^2;
sbit PT0   = IP^1;
sbit PX0   = IP^0;

/*  P1  */
sbit INT5  = P1^7;
sbit INT4  = P1^6;
sbit INT3  = P1^5;
sbit INT2  = P1^4;
sbit TXD1  = P1^3;
sbit RXD1  = P1^2;
sbit T2EX  = P1^1;
sbit T2    = P1^0;

/*  P3  */
sbit RD    = P3^7;
sbit WR    = P3^6;
sbit T1    = P3^5;
sbit T0    = P3^4;
sbit INT1  = P3^3;
sbit INT0  = P3^2;
sbit TXD0  = P3^1;
sbit RXD0  = P3^0;

/*  SCON0  */
sbit SM0   = SCON0^7; /* alternative SM0_FE_0 */
sbit SM1   = SCON0^6; /* alternative SM1_0 */
sbit SM2   = SCON0^5; /* alternative SM2_0 */
sbit REN   = SCON0^4; /* alternative REN_0 */
sbit TB8   = SCON0^3; /* alternative TB8_0 */
sbit RB8   = SCON0^2; /* alternative RB8_0 */
sbit TI    = SCON0^1; /* alternative TI_0  */
sbit RI    = SCON0^0; /* alternative RI_0  */

/*  SCON1  */
sbit SM01  = SCON1^7; /* alternative SM0_FE_1  */
sbit SM11  = SCON1^6; /* alternative SM1_1  */
sbit SM21  = SCON1^5; /* alternative SM2_1  */
sbit REN1  = SCON1^4; /* alternative REN_1  */
sbit TB81  = SCON1^3; /* alternative TB8_1  */
sbit RB81  = SCON1^2; /* alternative RB8_1  */
sbit TI1   = SCON1^1; /* alternative TI_1   */
sbit RI1   = SCON1^0; /* alternative RI_1   */

/*  T2CON  */
sbit TF2    = T2CON^7;
sbit EXF2   = T2CON^6;
sbit RCLK   = T2CON^5;
sbit TCLK   = T2CON^4;
sbit EXEN2  = T2CON^3;
sbit TR2    = T2CON^2;
sbit C_T2   = T2CON^1;
sbit CP_RL2 = T2CON^0;

/*  WDCON  */
sbit SMOD_1 = WDCON^7;
sbit POR    = WDCON^6;
sbit EPFI   = WDCON^5;
sbit PFI    = WDCON^4;
sbit WDIF   = WDCON^3;
sbit WTRF   = WDCON^2;
sbit EWT    = WDCON^1;
sbit RWT    = WDCON^0;

/*  EIE  */
sbit EWDI  = EIE^4;
sbit EX5   = EIE^3;
sbit EX4   = EIE^2;
sbit EX3   = EIE^1;
sbit EX2   = EIE^0;

/*  EIP  */
sbit PWDI  = EIP^4;
sbit PX5   = EIP^3;
sbit PX4   = EIP^2;
sbit PX3   = EIP^1;
sbit PX2   = EIP^0;

#endif

#endif
