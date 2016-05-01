/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : comm.h             
*                                                                         
* Description :                      
*                                                
* By          : Gong Bin               
*
* Date        : Sep 20, 2005                                                           
**********************************************************************************************************
*/



#include "stdarg.h"
#include "chip_cfg.h"


#ifdef   COMM_GLOBALS
#define  COMM_EXT
#else
#define  COMM_EXT  extern
#endif



/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/
#define  COMM_RX_BUF_SIZE        256                 /* Number of characters in Rx ring buffer          */
#define  COMM_TX_BUF_SIZE        1024                /* Number of characters in Tx ring buffer          */

#define  COMM_NO_ERR             0
#define  COMM_INVALID_CH         1
#define  COMM_RX_TIMEOUT         2
#define  COMM_TX_TIMEOUT         4
#define  COMM_RX_FLAG_TIMEOUT    8

#define  COMM_INT_MODE           1
#define  COMM_INQ_MODE           0

#define  COMM_CH_UART0           0
#define  COMM_CH_UART1           1
#define  COMM_CH_UART2           2


#define  FATAL_ERROR    1  
#define  MEDIUM_ERROR   2
#define  MINOR_ERROR    3
#define  WARNING        4                     
#define  DEBUG_MESSAGE  5

#define  UDI            1
#define  AUDIO_IN       2
#define  AUDIO_OUT      3
#define  VIN            4
#define  VOUT           5
#define  VSC            6
#define  MP4_ENCODE     7                   
#define  MP4_DECODE     8


/*
*********************************************************************************************************
*                                               FUCTION PROTYPES
*********************************************************************************************************
*/
int CommInit(int ch, int baudrate);

int debug(char *pcFmt,...);
int printf(char *pcFmt,...);
int printc(char *pcFmt,...);

#ifdef PRINT_IN_TASK
int _DEBUG(char *pcFmt,...);
int _PRINTF(char *pcFmt,...);
#else
#define _DEBUG debug
#define _PRINTF printf
#endif

int vsprintf(char *buf, char *fmt, va_list ap);

static int CommInqSendStringAlways(int ch, char *pbString);
static int  CommInqSendString (int ch, char *pbString);

char *CommPutStringToBuf(char *start_addr, char *pbString);
static int CommInqSendChar(int ch, char pbChar);

char WaitPressKey(void);
int CheckPressKey(char *receive);
void CommWrite(char *bPtr, int ctr);
char *MemPrintf(char *start_addr, char *pcFmt,...);
void CommReceiveFile(int ctr, int target_add1, int target_add2);
int logprintf(int debug_level, int category, char *pcFmt,...);
