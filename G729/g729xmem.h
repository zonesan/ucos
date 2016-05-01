/*
*********************************************************************************************************
*                                Copyright (c) 2006 vBridge Microsystem, Inc.
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved
*
* File        : bsp.h
*
* Description :
*
* By          : Greg Ren
*
* Date        : Jan, 2006
*********************************************************************************************************
*/

		.set noat
		.set noreorder

		.section .xdata,"aw",@nobits
		.align	1
		.origin	0

preflt_xy:	.space	4*4		// 4 words state for pre-filter {Yn-1, Yn-2, rsvd, Xn-1, rsvd Xn-2}

signal:		.space	2*80		// voice frame buffer

