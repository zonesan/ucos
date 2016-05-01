/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : bsp.h             
*                                                                         
* Description :                      
*                                                
* By          : Gong Bin               
*
* Date        : July 11, 2005                                                           
*********************************************************************************************************
*/
#define MacMvRR udi0
#define MacMvRY udi1
#define MacMvXR udi2
#define MacMvXY udi3

#define MpyMvRR udi4
#define MpyMvRY udi5
#define MpyMvXR udi6
#define MpyMvXY udi7

//#define MvDRR udi8
#define MvDRY udi9
#define MvDXR udi10
#define MvDXY udi11
//#define MvP udi12
//#define ADD udi13
#define SAT udi14

#define RdP(src,dst) 	udi15 src, dst-512

#define MvPWR(dst,src) 	udi12 src, dst
#define MvPRD(src,dst) 	udi12 src, dst-512
#define ModeReg 0x200
#define DAGX_RD 0x600
#define DAGX_WR 0xa00
#define DAGY_RD 0xe00
#define DAGY_WR 0x1200
#define SrcRegX0 0x1600
#define SrcRegX1 0x1a00
#define SrcRegY0 0x1e00
#define SrcRegY1 0x2200
#define PreRegX 0x2600
#define PreRegY 0x2a00
#define Mac0Acc0L 0x2e00
#define Mac0Acc0H 0x3200
#define Mac0Acc1L 0x3600
#define Mac0Acc1H 0x3a00
#define Mac1Acc0L 0x3e00
#define Mac1Acc0H 0x4200
#define Mac1Acc1L 0x4600
#define Mac1Acc1H 0x4a00
#define DAGX_RD_PTR 0x4e00
#define DAGX_WR_PTR 0x5200
#define DAGY_RD_PTR 0x5600
#define DAGY_WR_PTR 0x5a00
#define X_memory 0x5f40
#define Y_memory 0x62a0 

#define ADD(src1,src2) 	udi13 src1+src2
#define Mac0Acc0 0
#define Mac1Acc0 0
#define Mac1Acc1 0x10000
#define Mac0Acc1 0x80000

#define MvDRR(dst1,dst2,src1,src2) 	udi8 src1,src2,dst1+dst2
#define MvToRFX0 0
#define MvToRFX1 0x200
#define MvToRFY0 0
#define MvToRFY1 0x100

#define InitUDIReg( dst )        \
 	MvPWR( Mac0Acc0L,dst );\
	MvPWR( Mac0Acc0H,dst );\
	MvPWR( Mac0Acc1L,dst );\
	MvPWR( Mac0Acc1H,dst );\
	MvPWR( Mac1Acc0L,dst );\
	MvPWR( Mac1Acc0H,dst );\
	MvPWR( Mac1Acc1L,dst );\
	MvPWR( Mac1Acc1H,dst );\	
	MvPWR( SrcRegX0,dst );\
	MvPWR( SrcRegX1,dst );\
	MvPWR( SrcRegY0,dst );\
	MvPWR( SrcRegY1,dst );\
	MvPWR( PreRegX,dst );\
	MvPWR( PreRegY,dst );\
	MvPWR( DAGY_RD_PTR,dst );\
	MvPWR( DAGY_WR_PTR,dst );\
	MvPWR( DAGX_RD_PTR,dst );\
	MvPWR( DAGX_WR_PTR,dst );\
	MvPWR( ModeReg,dst )
	
#define InitUDIRegAnalysis( dst )        \
 	MvPWR( Mac0Acc0L,dst );\
	MvPWR( Mac0Acc0H,dst );\
	MvPWR( Mac1Acc0L,dst );\
	MvPWR( Mac1Acc0H,dst );\	
	MvPWR( SrcRegX0,dst );\
	MvPWR( SrcRegY0,dst );\
	MvPWR( PreRegX,dst );\
	MvPWR( PreRegY,dst );\
	MvPWR( ModeReg,dst )
	
#define ResultToMem( dst )\
 	sw	s0,0(dst);\
	sw	s1,4(dst);\
	sw	s2,8(dst);\
	sw	s3,12(dst);
	
#define MacMvXRInst(dst1,dst2)\
 	MacMvXR	zero,s4,0x18;\
	MacMvXR	zero,s5,0x18;\
	MacMvXR	zero,s6,0x18;\
	MacMvXR	zero,s7,0x18;\
	MacMvXR	zero,s8,0x18;\		
	MacMvXR	zero,s1,0x18;\
	MacMvXR	zero,s2,0x18;\
	MacMvXR	zero,s3,0x18;\		
	RdP(	t2,Mac0Acc0L );\
	RdP(	v0,Mac1Acc0L );\
	addu	dst1,t2,v0;\
	subu	a1,dst1,dst2;\
	li	t0,0x8000;\
	addu	t2,a1,t0;\
	sra	t1,t2,0x10