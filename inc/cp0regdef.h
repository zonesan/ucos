/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : cp0regdef.h             
*                                                                         
* Description : 
*                                                             
* Date        : Sep 29, 2005                                                           
*********************************************************************************************************
*/

#define  C0_Index         $0
#define  C0_Random        $1
#define  C0_EntryLo0      $2
#define  C0_EntryLo1      $3
#define  C0_Context       $4
#define  C0_PageMask      $5
#define  C0_Wired0        $6
#define  C0_BadVaddr      $8
#define  C0_Count         $9
#define  C0_EntryHi       $10
#define  C0_Compare       $11
#define  C0_Status        $12
#define  C0_Cause         $13
#define  C0_EPC           $14
#define  C0_PRId          $15
#define  C0_Config        $16
#define  C0_LLAddr        $17
#define  C0_WatchLo       $18
#define  C0_WatchHi       $19
#define  C0_XContext      $20
#define  C0_DEPC          $24
#define  C0_PerfCnt       $25
#define  C0_ECC           $26
#define  C0_CacheErr      $27
#define  C0_TagLo         $28
#define  C0_TagHi         $29
#define  C0_ErrorEPC      $30

/* Aliases for System Control Coprocessor (CP0) registers */
#define  C0_INDEX           C0_Index
#define  C0_RANDOM          C0_Random
#define  C0_ENTRYLO0        C0_EntryLo0
#define  C0_ENTRYLO1        C0_EntryLo1
#define  C0_CONTEXT         C0_Context
#define  C0_PAGEMASK        C0_PageMask
#define  C0_WIRED0          C0_Wired0
#define  C0_BADVADDR        C0_BadVAddr
#define  C0_COUNT           C0_Count
#define  C0_ENTRYHI         C0_EntryHi
#define  C0_COMPARE         C0_Compare
#define  C0_STATUS          C0_Status
#define  C0_CAUSE           C0_Cause

#define  C0_CONFIG          C0_Config
#define  C0_LLADDR          C0_LLAddr
#define  C0_WATCHLO	    C0_WatchLo
#define  C0_WATCHHI         C0_WatchHi
#define  C0_DEBUG           C0_Debug
#define  C0_PERFCNT         C0_PerfCnt
#define  C0_ERRCTL          C0_ErrCtl
#define  C0_CACHEERR        C0_CacheErr
#define  C0_TAGLO           C0_TagLo
#define  C0_DATALO          C0_DataLo
#define  C0_TAGHI           C0_TagHi
#define  C0_DATAHI          C0_DataHi
#define  C0_ERROREPC        C0_ErrorEPC

