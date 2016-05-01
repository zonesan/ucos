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

	.section .ydata,"aw",@nobits
	.align	4
	.origin	0

a140:	.half	7807, 7807, -3733, -3733	// pre highpass filter (Q12) a140[3] = {4096, 7807, -3733}
b140:	.half	1899, 1899, -3798, -3798	// pre highpass filter (Q11) a140[3] = {1899, -3798, 1899}

