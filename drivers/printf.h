/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : printf.h
*                                                                         
* Description :    
*             
* Date        : 2007.02.11                                                  
***************************************************************************
*/
#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#define PRINT_BUFFER_SIZE 11520    // for 115200 baudrate, the buffer size of 11520 can hold one second's data
#define SUB_BUFFER_SIZE 256
#define UART_ADDR UART0_BASE_ADDR
//#define UART_ADDR UART1_BASE_ADDR
#define STK_SIZE_TaskTwoRiscPrint 1000
OS_STK StkTaskTwoRiscPrint[STK_SIZE_TaskTwoRiscPrint];


void PrintTaskInit(void);
void TaskTwoRiscPrint(void *data);
int CommInqSendStringInTask(const char *pt_string);
void print_init_buffer(void);
void PrintAllInUartBuf(void);
