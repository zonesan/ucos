/*
***************************************************************************
*                 Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                              All Rights Reserved                   
*   
* File:         bsp.c             
*                                                                         
* Description :       
*                                                
* By:           Gong Bin               
*
* Date:         Sep 29, 2005                                                           
***************************************************************************
*/



/*
***************************************************************************
*                             Mips Interrupt Sources
***************************************************************************
*/
#define IP7    0x8000    // Hardware interrupt 5
#define IP6    0x4000    // Hardware interrupt 4
#define IP5    0x2000    // Hardware interrupt 3
#define IP4    0x1000    // Hardware interrupt 2
#define IP3    0x0800    // Hardware interrupt 1
#define IP2    0x0400    // Hardware interrupt 0
#define IP1    0x0200    // Request software interrupt 1
#define IP0    0x0100    // Request software interrupt 0



/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void  Risc1Entry(void);
void  Risc1IntrEntry(void);
void  ExceptInstallIntrInRam(void);
void  ExceptEnCount_Compare (unsigned intcompare);
void  ExceptDisCount(void);
void  ExceptClrCount        (void);
unsigned int GetCount(void);
void MipsIntrReqEn(unsigned int int_source);
void MipsIntrReqDis(unsigned int int_source);
int   MipsIntrEn(void);
int MipsIntrDis(void);

