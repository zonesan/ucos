/*
***************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File:         comm.c             
*                                                                         
* Description : Uart driver file.      
*                                                
* By:           Gong Bin               
*
* Date:         Sep 20, 2005                                                           
***************************************************************************
*/


/* debug level */
#define  FATAL_ERROR_EN      
#define  MEDIUM_ERROR_EN   
#define  MINOR_ERROR_EN    
#define  WARNING_EN                             
//#define  DEBUG_MESSAGE_EN  

/* module category */
#define  UDI_EN            
#define  AUDIO_IN_EN       
#define  AUDIO_OUT_EN      
#define  VIN_EN            
#define  VOUT_EN           
#define  VSC_EN            
#define  MP4_ENCODE_EN                        
#define  MP4_DECODE_EN     


#define    COMM_GLOBALS

#include "../drivers/uart_drivers/ftuart.h"
#include "includes.h"
#include "2risc_comm_protocol.h"

#include "chip_cfg.h"

#ifdef PRINT_IN_TASK
#include "printf.h"
#endif

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "../application/two_risc_system.h"


int CommInit(int ch, int baudrate)
{   
    if(HwUartSetBaudRate(ch, baudrate) == FALSE)
       return FALSE;
    HwUartInitialization(ch, tConfig_8Bit|tConfig_1StopBit|tConfig_NoParity);	
    
    return TRUE;
}

/*
***************************************************************************
*                                 debug
*
* Description: 
*
* Arguments  : 
*
* Returns    : 
*
* Note(s)    : 
***************************************************************************
*/
int debug (char *pcFmt,...)
{
#ifdef    DEBUG
	
    va_list ap;
    char pbString[256];

    va_start(ap,pcFmt);
    vsprintf(pbString, pcFmt, ap);   
#ifdef PRINT_IN_TASK
    return CommInqSendStringInTask(pbString);
#else
    return (CommInqSendStringAlways(UART_CHANNEL, pbString));
#endif
    va_end(ap);

#else
    
    return 0;    
            
#endif            
}

/*
*********************************************************************************************
*                                       printf
*
* Description: Writes output to the consol stream, under control of the string pointed to by
*              format that specifies how subsequent arguments are converted for output. If 
*              there are insufficient arguments for the format, the behavior is undefined.
*              If the format is exhausted while arguments remain, the excess arguments are
*              ignored.
*
* Arguments  : ch       is the COMM port channel and can be:
*                                0    : UART0
*                                1    : UART1
*                                2    : UART2
*              pcFmt - Format string. It can contain all the format specifies.
*              ...   - Are the passed parameters (pointers to the objects to receive the
*                      converted input).
*
* Returns    : COMM_NO_ERR       shows the call is successful.
*              COMM_INVALID_CH   shows the "ch" is an inalid uart port channel number.
*
* Note(s)    : CommInqPrintf applies the inquire mode to transmit, which is different 
*              from CommPrintf.
*********************************************************************************************
*/
int printf (char *pcFmt,...)
{
#ifndef GNU_SIMULATOR
	
    va_list ap;
    char pbString[256];

    va_start(ap,pcFmt);
    vsprintf(pbString, pcFmt, ap);   
#ifdef PRINT_IN_TASK
    return CommInqSendStringInTask(pbString);
#else
    return (CommInqSendStringAlways(UART_CHANNEL, pbString));
#endif
    va_end(ap);

#else

    return 0;    

#endif            
}

int printc (char *pcFmt,...)
{
#ifndef GNU_SIMULATOR
	
    va_list ap;
    char pbString[256];

    if(gtPrintMsgControl.en_print_periodical_msg == 0)
        return;

    va_start(ap,pcFmt);
    vsprintf(pbString, pcFmt, ap);   
#ifdef PRINT_IN_TASK
    return CommInqSendStringInTask(pbString);
#else
    return (CommInqSendStringAlways(UART_CHANNEL, pbString));
#endif
    va_end(ap);

#else

    return 0;

#endif            
}

#ifdef PRINT_IN_TASK
int _PRINTF(char *pcFmt,...)
{
#ifndef    GNU_SIMULATOR
	
        va_list ap;
        char pbString[256];

        va_start(ap,pcFmt);
        vsprintf(pbString, pcFmt, ap);   
        return (CommInqSendStringAlways(UART_CHANNEL, pbString));
        va_end(ap);

#else

    return 0;
        
#endif            
}

int _DEBUG(char *pcFmt,...)
{
#ifdef    DEBUG
	
    va_list ap;
    char pbString[256];

    va_start(ap,pcFmt);
    vsprintf(pbString, pcFmt, ap);   
    return (CommInqSendStringAlways(UART_CHANNEL, pbString));
    va_end(ap);

#else
    
    return 0;    
            
#endif            
}
#endif      // end of #ifdef PRINT_IN_TASK

/*
*********************************************************************************************
*                                       CommInqSendStringAlways
*
* Description: This routine waits till the string is sent.
*
* Arguments  : ch                is the COMM port channel and can be:
*                                0    : UART0
*                                1    : UART1
*                                2    : UART2
*              pbString          the string to be sent.
*
* Returns    : COMM_NO_ERR       shows the call is successful.
*              COMM_INVALID_CH   shows the "ch" is an inalid uart port channel number.
*
* Note(s)    : CommInqSendStringAlways cannot be shileded by "#ifndef    GNU_SIMULATOR", 
*              which is different from CommInqSendString.
*********************************************************************************************
*/
static int CommInqSendStringAlways(int ch, char *pbString)
{
    //OS_ENTER_CRITICAL();	
    //RiscSemAccept(SEM_UART0);
    LONG lwUartBaseAddr;
     	
    while (*pbString) {
        switch (ch) {
            case 0:
                lwUartBaseAddr = UART0_BASE_ADDR;
                while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
                wrb(*pbString++,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));              
                //while( (yUART_Interface->LT.LSR & 0x60) == 0 ) ; // Wait until transmit data register is empty.
                //yUART_Interface->RW.THR = *pbString++;           //send data;
                break;

            case 1:
                lwUartBaseAddr = UART1_BASE_ADDR;
                while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
                wrb(*pbString++,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));              
                //while( (yUART_Interface->LT.LSR & 0x60) == 0 ) ; // Wait until transmit data register is empty.
                //yUART_Interface->RW.THR = *pbString++;           //send data;
                break;                
                
            case 2:
                
                
            default:
                return (COMM_INVALID_CH);
        }
    }
    
    //RiscSemPost(SEM_UART0);
    
    return (COMM_NO_ERR);   
}

/*
*********************************************************************************************
*                                       CommInqSendString
*
* Description: This routine waits till the string is sent.
*
* Arguments  : ch                is the COMM port channel and can be:
*                                0    : UART0
*                                1    : UART1
*                                2    : UART2
*              pbString          the string to be sent.
*
* Returns    : COMM_NO_ERR       shows the call is successful.
*              COMM_INVALID_CH   shows the "ch" is an inalid uart port channel number.
*
* Note(s)    : CommInqSendString applies the inquire mode to transmit, which is different 
*              from CommSendString.
*********************************************************************************************
*/
static int CommInqSendString(int ch, char *pbString)
{
#ifndef    GNU_SIMULATOR

    LONG lwUartBaseAddr;

    //RiscSemAccept(SEM_UART0);
     	
    while (*pbString) {
        switch (ch) {
            case 0:
                lwUartBaseAddr = UART0_BASE_ADDR;
                while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
                wrb(*pbString++,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));              
                //while( (yUART_Interface->LT.LSR & 0x60) == 0 ) ; // Wait until transmit data register is empty.
                //yUART_Interface->RW.THR = *pbString++;           //send data;
                break;

            case 1:
                lwUartBaseAddr = UART1_BASE_ADDR;
                while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
                wrb(*pbString++,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));              
                //while( (yUART_Interface->LT.LSR & 0x60) == 0 ) ; // Wait until transmit data register is empty.
                //yUART_Interface->RW.THR = *pbString++;           //send data;
                break;                 
                
            case 2:
                
                
            default:
                return (COMM_INVALID_CH);
        }
    }
    
    //RiscSemPost(SEM_UART0);
    
    return (COMM_NO_ERR);   

#else

    return 0;
  
#endif            
}

/*
*********************************************************************************************
*                                       CommInqSendChar
*
* Description: This routine send a char to Uart.
*
* Arguments  : ch                is the COMM port channel and can be:
*                                0    : UART0
*                                1    : UART1
*                                2    : UART2
*              pbChar            the char to be sent.
*
* Returns    : COMM_NO_ERR       shows the call is successful.
*              COMM_INVALID_CH   shows the "ch" is an inalid uart port channel number.
*
* Note(s)    : 
*********************************************************************************************
*/
static int CommInqSendChar(int ch, char pbChar)
{
    long lwUartBaseAddr;	
	
    switch (ch) {
        case 0:
            lwUartBaseAddr = UART0_BASE_ADDR;
            while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
            wrb(pbChar,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));                                                              
            break;

        case 1:
            lwUartBaseAddr = UART1_BASE_ADDR;
            while( (rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR)) & 0x60) != 0x60 ) ;//busy
            wrb(pbChar,UART_REGS_ADDR(lwUartBaseAddr,tUartTHR));                                                              
            break;               
                
        case 2:
                
                
        default:
            return (COMM_INVALID_CH);
    }
    return (COMM_NO_ERR);   
}

/*
*********************************************************************************************
*                                       CommPutStringToBuf
*
* Description: This routine put a string into the memory with specified address.
*
* Arguments  : start_addr        is the start address.
*              pbString          the string to be put.
*
* Returns    : COMM_NO_ERR       shows the call is successful.
*              COMM_INVALID_CH   shows the "ch" is an inalid uart port channel number.
*
* Note(s)    : 
*********************************************************************************************
*/
char *CommPutStringToBuf(char *start_addr, char *pbString)
{
    while (*pbString) { 
        *start_addr++ = *pbString++;
    }
    return start_addr;
}

/*
*********************************************************************************************
*                                       WaitPressKey
*
* Description: This routine waits till the serial port receive at least one byte.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 
*********************************************************************************************
*/
char WaitPressKey(void)
{
    char receive;	
	
    while(1) {	
        if(CheckPressKey(&receive)) {
            return receive;            
        }
    }        	
}

/*
*********************************************************************************************
*                                       CheckPressKey
*
* Description: This routine check if the serial port receive any bytes.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 
*********************************************************************************************
*/
int CheckPressKey(char *receive)
{
    int lwUartId = UART_CHANNEL;
    BYTE cwStatus;
    BOOL result = FALSE;
    LONG lwUartBaseAddr;	
	
    if( !lwUartId ) lwUartBaseAddr = UART0_BASE_ADDR;
    else if( lwUartId == 1 ) lwUartBaseAddr = UART1_BASE_ADDR;
    else return FALSE;	

    cwStatus = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartLSR));
    if( cwStatus & 0x1 ) 
    {
        *receive = rdb(UART_REGS_ADDR(lwUartBaseAddr,tUartRBR));		
    }
    else return FALSE;
	
    if( cwStatus & 0x9e ) return FALSE;
    return TRUE;	    
}

/*
*********************************************************************************************
*                                       CommWrite
*
* Description: This routine send a specified number bytes to the serial port.
*
* Arguments  : bPtr        is the data pointer.
*              ctr         is the number to be sent.
*
* Returns    : none
*
* Note(s)    : 
*********************************************************************************************
*/
void CommWrite(char *bPtr, int ctr)
{/*	           
    while(ctr > 0){
        // while( (yUART_Interface->LT.LSR & 0x20) == 0 ); // Wait until transmit data register is empty.
        while( (yUART_Interface->LT.LSR & 0x60) == 0 ); // Wait until transmit data register is empty.
        yUART_Interface->RW.THR = *bPtr++;              //send data;        	
        ctr--;        
    }*/    	    
}

/*
*********************************************************************************************
*                                       CommReceiveFile
*
* Description: This routine send a specified number bytes to the serial port.
*
* Arguments  : bPtr        is the data pointer.
*              ctr         is the number to be sent.
*
* Returns    : none
*
* Note(s)    : Copied from Eric
*********************************************************************************************
*/

void CommReceiveFile(int ctr, int target_add1, int target_add2)
{ 
//#ifdef COMM_MODULE_INCLUDE
/*   	
    char receive;		
	  char *tmp_add1;
	  char *tmp_add2;
	  
	  tmp_add1 = (char *)(0xa0000000 + target_add1);
	  tmp_add2 = (char *)(0xa0000000 + target_add2);
	  
    while(ctr > 0)
    {
        if(yUART_ReadChar(&receive)) 
        {		
            (*(char *)(tmp_add1++)) = receive;
            (*(char *)(tmp_add2++)) = receive;
            ctr--;            
        }
    }    	    */
//#endif    
}


char *MemPrintf(char *start_addr, char *pcFmt,...)
{
/*	
        va_list ap;
        char pbString[256];

        va_start(ap,pcFmt);
        vsprintf(pbString, pcFmt, ap);   
        return(CommPutStringToBuf(start_addr, pbString));
        va_end(ap);
*/        
}

/*
*********************************************************************************************
*                                       logprintf
*
* Description: This function prints message according to debug_level & category so that 
*              developer can easily turn on/off any particular message.
*
* Arguments  : debug_level      has the followings:
*                                       FATAL_ERROR
*                                       MEDIUM_ERROR
*                                       MINOR_ERROR
*                                       WARNING
*                                       DEBUG_MESSAGE
*              Category         has the followings:
*                                       UDI
*                                       AUDIO_IN
*                                       AUDIO_OUT
*                                       VIN
*                                       VOUT
*                                       VSC
*                                       MP4_EN
*                                       MP4_DE
*                               For example, if the message is about VIN, you can select VIN
*                               as the second parameter.
*
* Returns    : SUCCESSFUL       if any message is printed.
*              FAILED           if no message is printed.
*
* Note(s)    : 
*********************************************************************************************
*/
int logprintf(int debug_level, int category, char *pcFmt,...)
{
#ifndef    GNU_SIMULATOR
	
    va_list ap;
    char pbString[256];

    /* debug level */
    switch(debug_level) {
#ifdef FATAL_ERROR_EN        
        case FATAL_ERROR:
            break;
#endif            
          
#ifdef MEDIUM_ERROR_EN        
        case MEDIUM_ERROR:
            break;
#endif                    
        
#ifdef MINOR_ERROR_EN           
        case MINOR_ERROR:
            break;
#endif                    
        
#ifdef WARNING_EN          
        case WARNING:
            break;
#endif                    
        
#ifdef DEBUG_MESSAGE_EN                                    
        case DEBUG_MESSAGE:
            break;
#endif
                    
        default:
            return FAILED;         
    }
    
    /* module category */
    switch(category) {
#ifdef UDI_EN        
        case UDI:        
            break;
#endif            
        
#ifdef AUDIO_IN_EN        
        case AUDIO_IN:
            break;
#endif

#ifdef AUDIO_OUT_EN            
        case AUDIO_OUT:            
            break;
#endif  

#ifdef VIN_EN          
        case VIN:  
            break;
#endif 

#ifdef VOUT_EN           
        case VOUT:  
            break;
#endif

#ifdef VSC_EN            
        case VSC:  
            break;
#endif

#ifdef MP4_ENCODE_EN            
        case MP4_ENCODE:                            
            break;
#endif

#ifdef MP4_DECODE_EN            
        case MP4_DECODE:            
            break;
#endif            
        default:
            return FAILED;         
    }    

    va_start(ap,pcFmt);
    vsprintf(pbString, pcFmt, ap);   

#ifdef PRINT_IN_TASK
    return CommInqSendStringInTask(pbString);
#else
    return (CommInqSendStringAlways(UART_CHANNEL, pbString));
#endif
    
    va_end(ap);

#else

    return 0;
        
#endif            
}

