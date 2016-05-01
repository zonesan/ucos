/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : printf.c
*                                                                         
* Description :    
*             
* Date        : 2007.02.11                                                  
***************************************************************************
*/
#include "ftuart.h"
#include "vb1000.h"
#include "printf.h"

#include "comm.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "../application/two_risc_system.h"
#include "ucos_task_priority.h"
char print_buffer[PRINT_BUFFER_SIZE];
static unsigned int wr_index;
static unsigned int rd_index;
static volatile int buf_full;
static volatile int buf_empty;


/*
***************************************************************************
*                          print_wr_buffer
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void print_init_buffer(void)
{
    for(wr_index = 0; wr_index < PRINT_BUFFER_SIZE; wr_index++)
        print_buffer[wr_index] = 0;
    wr_index = 0;
    rd_index = 0;
    buf_full = 0;
    buf_empty = 1;
}

/*
***************************************************************************
*                  CommInqSendStringInTask
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int CommInqSendStringInTask(const char *pt_string)
{
    OS_CPU_SR  cpu_sr;
    char string[] = "\r\nC@mmFull\r\n";
    unsigned int tmp, i;

    
    OS_ENTER_CRITICAL();
    if(buf_full == 1)
    {
        OS_EXIT_CRITICAL(); 
        return 0;
    }
    while(*pt_string)
    {
        if(wr_index >= PRINT_BUFFER_SIZE)
            wr_index = 0;
        print_buffer[wr_index] = *pt_string;
        wr_index++;
        pt_string++;
        if(wr_index == rd_index)
        {
            buf_full = 1;

            /* tell users comm buffer has been full! */
#if 0            
            _PRINTF("%s", string);
#else
            tmp = wr_index;
            for(i = 0; i < sizeof(string); i++) {
                if(tmp == 0)
                    tmp = PRINT_BUFFER_SIZE - 1;
                print_buffer[--tmp] = string[sizeof(string) - i - 1];
            }
#endif
            break;
        }
    }
    //_PRINTF("\n %d == %d / %d %d \n",wr_index, rd_index, buf_full, buf_empty);
    buf_empty = 0;
    OS_EXIT_CRITICAL(); 
    return 0;     
}

/*
***************************************************************************
*                          print_rd_buffer
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int print_rd_buffer(int length, char *pt_string)
{
    int read_cnt;
    OS_CPU_SR  cpu_sr;

    //OS_ENTER_CRITICAL();    
    if(buf_empty == 1)
    {
        //OS_EXIT_CRITICAL();     
        return 0;
    }
        
    OS_ENTER_CRITICAL();
    
    read_cnt = 0;
    while(length > 0)
    {
        if(rd_index >= PRINT_BUFFER_SIZE)
            rd_index = 0;
        *pt_string = print_buffer[rd_index];
        rd_index++;
        pt_string++;
        read_cnt++;
        length--;
        if(rd_index == wr_index)
        {
            buf_empty = 1;
            break;
         }
    }
    buf_full = 0;
    OS_EXIT_CRITICAL(); 
    return read_cnt;
}

/*
***************************************************************************
*                          CommInqSendString
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void PrintSendString( int cnt, const char *pbString)
{
    while(cnt > 0)
    {
        while( (rdb(UART_REGS_ADDR(UART_ADDR,tUartLSR)) & 0x60) != 0x60 ) ;//busy
        wrb(*pbString,UART_REGS_ADDR(UART_ADDR,tUartTHR));
        pbString++;
        cnt--;
    }
}

/*
***************************************************************************
*                          PrintAllInUartBuf
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    :  
***************************************************************************
*/
void PrintAllInUartBuf(void)
{
    char buf[SUB_BUFFER_SIZE];
    int cnt;

    OS_TCB pdata;
    unsigned char err;

    _PRINTF("ucos print msg in exception/interrupt START:\n");
    
 #ifdef PRINT_IN_TASK   
    if((err = OSTaskQuery(PRIOR_TASK_PRINT, &pdata)) == 0) {
        cnt = print_rd_buffer(SUB_BUFFER_SIZE, buf);
        while(cnt)
        {
            if(gtPrintMsgControl.en_print_all_msg)
                PrintSendString(cnt, buf);
            cnt = print_rd_buffer(SUB_BUFFER_SIZE, buf);
        } 
    }
#endif    
    _PRINTF("OSIntNesting=%d, OSPrioCur=%d, OSPrioHighRdy=%d.\n", OSIntNesting, OSPrioCur, OSPrioHighRdy);

    /* EPC print has moved to BspPrintfCp0Reg! */
    //_PRINTF("C0_ErrorEPC = 0x%x(code=0x%x), C0_EPC = 0x%x(code=0x%x, 0x%x).\n", ErrorEPC_addr, ErrorEPC_instr, EPC_addr, EPC_instr, EPC_instr_slot);
    
    _PRINTF("ucos print msg in exception/interrupt END!\n");
    
}

/*
***************************************************************************
*                          TaskTwoRiscPrint
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void TaskTwoRiscPrint(void *data)
{
    int cnt;
    char sub_buffer[SUB_BUFFER_SIZE];
    _PRINTF("xxxxxxxxxx : Print task initialized successfully!\r\n");
    cnt = 0;
    //print_init_buffer();
    while(1)
    {
        cnt = print_rd_buffer(SUB_BUFFER_SIZE, sub_buffer);
        if(gtPrintMsgControl.en_print_all_msg)
            PrintSendString(cnt, sub_buffer);
        if(cnt == 0) {
            OSTimeDlyHMSM(0, 0, 0, 100);
        }
    }
}

