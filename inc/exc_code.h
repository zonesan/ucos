/*
***************************************************************************
*                 Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File:         exc_code.h             
*                                                                         
* Description : the value of ExcCode(Cause bit6~bit2)       
*                                                        
* Date:         Feb 3, 2008                                                           
***************************************************************************
*/


/* Exception Code */
#define EXC_INT     0       /* External interrupt */
#define EXC_MOD     1       /* TLB modification */
#define EXC_TLBL    2       /* TLB miss (Load or Ifetch) */
#define EXC_TLBS    3       /* TLB miss (Save) */
#define EXC_ADEL    4       /* Addr error (Load or Ifetch) */
#define EXC_ADES    5       /* Address error (Save) */
#define EXC_IBE     6       /* Bus error (Ifetch) */
#define EXC_DBE     7       /* Bus error (data load/store) */
#define EXC_SYS     8       /* System call */
#define EXC_BP      9       /* Break point */
#define EXC_RI      10      /* Reserved instruction */
#define EXC_CPU     11      /* Coprocessor unusable */
#define EXC_OVF     12      /* Arithmetic overflow */
#define EXC_TR      13      /* Trap instruction */