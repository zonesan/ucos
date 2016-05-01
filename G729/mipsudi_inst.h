/*
*******************************************************************************
* Copyright (c) 2006 vBridge Microsystem, Inc.
* All Rights Reserved
*
* File        : mipsudi_inst.h
* Description :
* By          : Greg Ren
* Date        : Feb, 2006
*******************************************************************************
*/

/*
 * This file defines all UDI instruction macros.
 * Macro design:
 *	1. All instructions are lower case only.
 *	2. Use .word assembly directive to convert directly to instruction coding.
 *	3. Include error checking macro to ensure valid UDI instruction value.
 * Catches:
 *	1. The register names ca not be used directly due the leading '$" sign.
 *	   User has to use re-defined (in this file) GPR names.
 */

/*
 * Common definition
 */
#define	udi_base	0x70000010
#define gpr_max		31

/* GPR re-define */
#define gprv0	2
#define gprv1	3

#define	gpra0	4
#define	gpra1	5
#define	gpra2	6
#define	gpra3	7
#define	gprt0	8
#define	gprt1	9
#define	gprt2	10
#define	gprt3	11
#define	gprt4	12
#define	gprt5	13
#define	gprt6	14
#define	gprt7	15

#define gprs0	16
#define gprs1	17
#define gprs2	18
#define gprs3	19
#define gprs4	20
#define gprs5	21
#define gprs6	22
#define gprs7	23
#define gprs8	30
#define gprt8	24
#define gprt9	25



/* MAC and GPR to X/Y data move */
#define x0		(0<<15)
#define x1		(1<<15)
#define y0		(0<<14)
#define y1		(1<<14)
#define acc0		(0<<11)
#define acc1		(1<<11)
#define ux0		(0<<10)
#define ux1		(8<<10)
#define sx0		(1<<10)
#define sx1		(9<<10)
#define uy0		(0<<9)
#define uy1		(8<<9)
#define sy0		(1<<9)
#define sy1		(9<<9)

#define shnone		(0<<6)
#define shl		(1<<6)
#define shr		(2<<6)
#define shsat		(3<<6)

/* Parallel data move code */
#define	mvxy_flag	0x80000000
#define	rd32		(0x00|mvxy_flag)	// Read 32-bit
#define	rd32i		(0x01|mvxy_flag)	// Read 32-bit (w/address inc)
#define	rd32dpf		(0x02|mvxy_flag)	// Read 32-bit DPF
#define	rd32dpfi	(0x03|mvxy_flag)	// Read 32-bit DPF(w/address inc)
#define	rd16		(0x04|mvxy_flag)	// Read 16-bit
#define	rd16i		(0x05|mvxy_flag)	// Read 16-bit (w/address inc)
#define	wrmac		(0x0a|mvxy_flag)	// Write 32-bit (from MAC)
#define	wrmaci		(0x0b|mvxy_flag)	// Write 32-bit (from MAC) (w/address inc)
#define	wrmachi		(0x0c|mvxy_flag)	// Write 16-bit (from MAC_HI)
#define	wrmachii	(0x0d|mvxy_flag)	// Write 16-bit (from MAC_HI) (w/address inc)
#define	wrmaclo		(0x0e|mvxy_flag)	// Write 16-bit (from MAC_LO)
#define	wrmacloi	(0x0f|mvxy_flag)	// Write 16-bit (from MAC_LO) (w/address inc)
#define	mvnop		(0x10|mvxy_flag)	// No data move
#define	mvignore	0

/* MvP - udi12: dst = src (Local register definitions) */
#define	local_reg_flag	0x40000000
#define udi_mode	(local_reg_flag|0x00)
#define udi_dagx_rd	(local_reg_flag|0x01)
#define udi_dagx_wr	(local_reg_flag|0x02)
#define udi_dagy_rd	(local_reg_flag|0x03)
#define udi_dagy_wr	(local_reg_flag|0x04)
#define udi_x0		(local_reg_flag|0x05)
#define udi_x1		(local_reg_flag|0x06)
#define udi_y0		(local_reg_flag|0x07)
#define udi_y1		(local_reg_flag|0x08)
#define udi_prex	(local_reg_flag|0x09)
#define udi_prey	(local_reg_flag|0x0a)
#define udi_mac0acc0lo	(local_reg_flag|0x0b)
#define udi_mac0acc0hi	(local_reg_flag|0x0c)
#define udi_mac0acc1lo	(local_reg_flag|0x0d)
#define udi_mac0acc1hi	(local_reg_flag|0x0e)
#define udi_mac1acc0lo	(local_reg_flag|0x0f)
#define udi_mac1acc0hi	(local_reg_flag|0x10)
#define udi_mac1acc1lo	(local_reg_flag|0x11)
#define udi_mac1acc1hi	(local_reg_flag|0x12)
#define udi_dagx_rdptr	(local_reg_flag|0x13)
#define udi_dagx_wrptr	(local_reg_flag|0x14)
#define udi_dagy_rdptr	(local_reg_flag|0x15)
#define udi_dagy_wrptr	(local_reg_flag|0x16)
#define udi_reg_max	(local_reg_flag|0x1f)
#define	udi_nop		mvp gprt0,udi_mac0acc0lo

/* SAT - udi14: macn_accn = SAT(macn_accn >> shift) */
#define mac0 (0)
#define mac1 (1)
#define	round_enable (1<<19) // Which bit???


/*
 * Error checking macros
 */
.macro	mac_err_check	accn, xmul, ymul, shift
	.if (\accn <> acc0) && (\accn <> acc1)
	.error "Invalid accumulator specified"
	.endif
	.if (\xmul <> sx0) && (\xmul <> sx1) && (\xmul <> ux0) && (\xmul <> ux1)
	.error "Invalid source X for multiplication"
	.endif
	.if (\ymul <> sy0) && (\ymul <> sy1) && (\ymul <> uy0) && (\ymul <> uy1)
	.error "Invalid source Y for multiplication"
	.endif
	.if (\shift < shnone) || (\shift > shsat)
	.error "Invalid shift value for multiplication"
	.endif
.endm

.macro	movrx_err_check	gprx, xmov
	.if !((\gprx >= 0) && (\gprx <= gpr_max))
	.error "Invalid source gpr for X data move"
	.endif
	.if (\xmov <> x0) && (\xmov <> x1)
	.error "Invalid destination for X data move"
	.endif
.endm

.macro	movry_err_check	gpry, ymov
	.if !((\gpry >= 0) && (\gpry <= gpr_max))
	.error "Invalid source gpr for Y data move"
	.endif
	.if (\ymov <> y0) && (\ymov <> y1)
	.error "Invalid destination for Y data move"
	.endif
.endm

.macro	movx_err_check	dst, src
	.if (\dst & mvxy_flag)
	.if (\src <> x0) && (\src <> x1) && (\src <> mvignore)
	.error "Invalid source for X data move"
	.endif
	.if !((\dst >= wrmac) && (\dst <= mvnop))
	.error "Invalid destination for X data move"
	.endif
	.elseif (\src & mvxy_flag)
	.if (\dst <> x0) && (\dst <> x1)
	.error "Invalid destination for X data move"
	.endif
	.if !((\src >= rd32) && (\src <= rd16i))
	.error "Invalid source for X data move"
	.endif
	.else
	.error "Invalid X data move"
	.endif
.endm

.macro	movy_err_check	dst, src
	.if (\dst & mvxy_flag)
	.if (\src <> y0) && (\src <> y1) && (\src <> mvignore)
	.error "Invalid source for Y data move"
	.endif
	.if !((\dst >= wrmac) && (\dst <= mvnop))
	.error "Invalid destination for Y data move"
	.endif
	.elseif (\src & mvxy_flag)
	.if (\dst <> y0) && (\dst <> y1)
	.error "Invalid destination for Y data move"
	.endif
	.if !((\src >= rd32) && (\src <= rd16i))
	.error "Invalid source for Y data move"
	.endif
	.else
	.error "Invalid Y data move"
	.endif
.endm

/*
 * UDI instruction macros
 */
/* MacMvRR - udi0: accn = acc +/- ((xmul * ymul) << shift), xmov = gprx, ymov = gpry */
.macro	macmvrr	accn, xmul, ymul, shift, xmov, gprx, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(\gprx<<21)|(\gpry<<16)|\xmov|\ymov|\accn|\xmul|\ymul|(0<<8)|\shift|0
.endm

.macro	msumvrr	accn, xmul, ymul, shift, xmov, gprx, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(\gprx<<21)|(\gpry<<16)|\xmov|\ymov|\accn|\xmul|\ymul|(1<<8)|\shift|0
.endm

/* MacMvRY - udi1: accn = acc +/- ((xmul * ymul) << shift), xmov = gprx, ydst = ysrc */
.macro	macmvry	accn, xmul, ymul, shift, xmov, gprx, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(\gprx<<21)|(((\ydst|\ysrc)&31)<<16)|\xmov|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(0<<8)|\shift|1
.endm

.macro	msumvry	accn, xmul, ymul, shift, xmov, gprx, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(\gprx<<21)|(((\ydst|\ysrc)&31)<<16)|\xmov|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(1<<8)|\shift|1
.endm

/* MacMvXR - udi2: accn = acc +/- ((xmul * ymul) << shift), xdst = xsrc, ymov = gpry */
.macro	macmvxr	accn, xmul, ymul, shift, xdst, xsrc, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(\gpry<<16)|((\xdst|\xsrc)&x1)|\ymov|\accn|\xmul|\ymul|(0<<8)|\shift|2
.endm

.macro	msumvxr	accn, xmul, ymul, shift, xdst, xsrc, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(\gpry<<16)|((\xdst|\xsrc)&x1)|\ymov|\accn|\xmul|\ymul|(1<<8)|\shift|2
.endm

/* MacMvXY - udi3: accn = acc +/- ((xmul * ymul) << shift), xdst = xsrc, ydst = ysrc */
.macro	macmvxy	accn, xmul, ymul, shift, xdst, xsrc, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(((\ydst|\ysrc)&31)<<16)|((\xdst|\xsrc)&x1)|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(0<<8)|\shift|3
.endm

.macro	msumvxy	accn, xmul, ymul, shift, xdst, xsrc, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(((\ydst|\ysrc)&31)<<16)|((\xdst|\xsrc)&x1)|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(1<<8)|\shift|3
.endm

/* MpyMvRR - udi4: accn = (xmul * ymul) << shift, xmov = gprx, ymov = gpry */
.macro	mpymvrr	accn, xmul, ymul, shift, xmov, gprx, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(\gprx<<21)|(\gpry<<16)|\xmov|\ymov|\accn|\xmul|\ymul|(0<<8)|\shift|4
.endm

/* MpyMvRY - udi5: accn = (xmul * ymul) << shift, xmov = gprx, ydst = ysrc */
.macro	mpymvry	accn, xmul, ymul, shift, xmov, gprx, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movrx_err_check	\gprx, \xmov
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(\gprx<<21)|(((\ydst|\ysrc)&31)<<16)|\xmov|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(0<<8)|\shift|5
.endm

/* MpyMvXR - udi6: accn = (xmul * ymul) << shift, xdst = xsrc, ymov = gpry */
.macro	mpymvxr	accn, xmul, ymul, shift, xdst, xsrc, ymov, gpry
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(\gpry<<16)|((\xdst|\xsrc)&x1)|\ymov|\accn|\xmul|\ymul|(0<<8)|\shift|6
.endm

/* MpyMvXY - udi7: accn = (xmul * ymul) << shift, xdst = xsrc, ydst = ysrc */
.macro	mpymvxy	accn, xmul, ymul, shift, xdst, xsrc, ydst, ysrc
	mac_err_check	\accn, \xmul, \ymul, \shift
	movx_err_check	\xdst, \xsrc
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(((\ydst|\ysrc)&31)<<16)|((\xdst|\xsrc)&x1)|((\ydst|\ysrc)&y1)|\accn|\xmul|\ymul|(0<<8)|\shift|7
.endm

/* MvDRR - udi8: xmov = gprx, ymov = gpry */
.macro	mvdrr	xmov, gprx, ymov, gpry
	movrx_err_check	\gprx, \xmov
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(\gprx<<21)|(\gpry<<16)|\xmov|\ymov|8
.endm

/* MvDRY - udi9:  xmov = gprx, ydst = ysrc */
.macro	mvdry	xmov, gprx, ydst, ysrc
	movrx_err_check	\gprx, \xmov
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(\gprx<<21)|(((\ydst|\ysrc)&31)<<16)|\xmov|((\ydst|\ysrc)&y1)|9
.endm

/* MvDXR - udi10: xdst = xsrc, ymov = gpry */
.macro	mvdxr	xdst, xsrc, ymov, gpry
	movx_err_check	\xdst, \xsrc
	movry_err_check	\gpry, \ymov
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(\gpry<<16)|((\xdst|\xsrc)&x1)|\ymov|10
.endm

/* MvDXY - udi11: xdst = xsrc, ymov = gpry */
.macro	mvdxy	xdst, xsrc, ydst, ysrc
	movx_err_check	\xdst, \xsrc
	movy_err_check	\ydst, \ysrc
	.word	0x70000010|(((\xdst|\xsrc)&31)<<21)|(((\ydst|\ysrc)&31)<<16)|((\xdst|\xsrc)&x1)|((\ydst|\ysrc)&y1)|11
.endm

/* MvP - udi12: dst = src (Local register definitions) */
#define	local_reg_flag	0x40000000
#define udi_mode	(local_reg_flag|0x00)
#define udi_dagx_rd	(local_reg_flag|0x01)
#define udi_dagx_wr	(local_reg_flag|0x02)
#define udi_dagy_rd	(local_reg_flag|0x03)
#define udi_dagy_wr	(local_reg_flag|0x04)
#define udi_x0		(local_reg_flag|0x05)
#define udi_x1		(local_reg_flag|0x06)
#define udi_y0		(local_reg_flag|0x07)
#define udi_y1		(local_reg_flag|0x08)
#define udi_prex	(local_reg_flag|0x09)
#define udi_prey	(local_reg_flag|0x0a)
#define udi_mac0acc0lo	(local_reg_flag|0x0b)
#define udi_mac0acc0hi	(local_reg_flag|0x0c)
#define udi_mac0acc1lo	(local_reg_flag|0x0d)
#define udi_mac0acc1hi	(local_reg_flag|0x0e)
#define udi_mac1acc0lo	(local_reg_flag|0x0f)
#define udi_mac1acc0hi	(local_reg_flag|0x10)
#define udi_mac1acc1lo	(local_reg_flag|0x11)
#define udi_mac1acc1hi	(local_reg_flag|0x12)
#define udi_dagx_rdptr	(local_reg_flag|0x13)
#define udi_dagx_wrptr	(local_reg_flag|0x14)
#define udi_dagy_rdptr	(local_reg_flag|0x15)
#define udi_dagy_wrptr	(local_reg_flag|0x16)
#define udi_reg_max	(local_reg_flag|0x1f)

.macro	mvp	dst, src
	.if (\dst & local_reg_flag)
	.if !((\src >= 0) && (\src <= gpr_max))
	.error "Invalid source gpr for register move"
	.endif
	.if !((\dst >= udi_mode) && (\dst <= udi_reg_max))
	.error "Invalid local destination register"
	.endif
	.word	0x70000010|((\src&31)<<21)|((\dst&31)<<16)|(1<<15)|12
	.elseif (\src & local_reg_flag)
	.if !((\dst >= 0) && (\dst <= gpr_max))
	.error "Invalid destination gpr for register move"
	.endif
	.if !((\src >= udi_mode) && (\src <= udi_reg_max))
	.error "Invalid local source register"
	.endif
	.word	0x70000010|((\dst&31)<<21)|((\src&31)<<16)|12
	.else
	.error "Invalid register move"
	.endif
.endm

/* ADD - udi13: mac0accn = mac0accn + mac1accn */
.macro	addacc	mac0accn, mac1accn
	.if (\mac0accn <> acc0) && (\mac0accn <> acc1)
	.error "Invalid destination and source ACC source"
	.endif
	.if (\mac1accn <> acc0) && (\mac1accn <> acc1)
	.error "Invalid source ACC"
	.endif
	.word	0x70000010|(\mac0accn<<(25-11))|(\mac1accn<<(22-11))|13
.endm

/* SAT - udi14: macn_accn = SAT(macn_accn >> shift) */
#define mac0 (0)
#define mac1 (1)
#define	round_enable (1<<19)  
#define	round_disable (0<<19) 
#define	sat_enable (1<<18)  
#define	sat_disable (0<<18)
#define	satp16 (1<<17)  
#define	satp32 (0<<17)

.macro	satacc	macn, accn, round, saturation, satpn, shift
	.if (\macn <> mac0) && (\macn <> mac1)
	.error "Invalid MAC for saturation"
	.endif
	.if (\accn <> acc0) && (\accn <> acc1)
	.error "Invalid ACC for saturation"
	.endif
	.if (\saturation <> sat_enable ) && (\saturation <> sat_disable)
	.error "Invalid saturation "
	.endif
	.if (\satpn <> satp16 ) && (\satpn <> satp32)
	.error "Invalid satpos "
	.endif
	.if (\round <> round_enable ) && (\round <> round_disable)
	.error "Invalid round "
	.endif
	.if (\shift >= 0) && (\shift <= 12)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(\shift<<20)|(\round)|(\saturation)|(\satpn)|14
	.elseif (\shift == 16)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(13<<20)|(\round)|(\saturation)|(\satpn)|14
	.elseif (\shift == 24)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(14<<20)|(\round)|(\saturation)|(\satpn)|14
	.elseif (\shift == -16)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(15<<20)|(\round)|(\saturation)|(\satpn)|14
	.else
	.error "Invalid shift for ACC saturation"
	.endif
.endm

/* SAT - udi14: macn_accn = SAT(ROUND(macn_accn >> shift)) */
.macro	satracc	macn, accn, shift
	.if (\macn <> mac0) && (\macn <> mac1)
	.error "Invalid MAC for saturation"
	.endif
	.if (\accn <> acc0) && (\accn <> acc1)
	.error "Invalid ACC for saturation"
	.endif
	.if (\shift >= 0) && (\shift <= 12)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(\shift<<20)|round_enable|14
	.elseif (\shift == 16)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(13<<20)|round_enable|14
	.elseif (\shift == 24)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(14<<20)|round_enable|14
	.elseif (\shift == -16)
	.word	0x70000010|(\macn<<25)|(\accn<<(24-11))|(15<<20)|round_enable|14
	.else
	.error "Invalid shift for ACC saturation"
	.endif
.endm

/* RdP - udi15: dst = src (UDI register definitions). Does NOT flow through PIPELINE, 1 cycle finish */

.macro	 rdp	dst, src
	.if (\dst & local_reg_flag)
	.error "Invalid local destination register"
	.elseif (\src & local_reg_flag)
	.if !((\dst >= 0) && (\dst <= gpr_max))
	.error "Invalid destination gpr for register move"
	.endif
	.if !((\src >= udi_mode) && (\src <= udi_reg_max))
	.error "Invalid local source register"
	.endif
	.word	0x70000010|((\dst&31)<<21)|((\src&31)<<16)|15
	.else
	.error "Invalid register move"
	.endif
.endm


