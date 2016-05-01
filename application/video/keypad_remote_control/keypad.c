/*
***************************************************************************
*                   Copyright (c) 2007 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        :     keypad.c 
*                                                                         
* Description : Receive the remote control pulses and decode                                          *                                                                          
*
* Date        : June 25, 2007                                                           
***************************************************************************
*/



#define KEYPAD_GLOBALS

#include "video_cfg.h"

#ifdef DVR

#include "includes.h"
#include "keypad.h"
#include "gpio.h"
#include "../drivers/risc.h"

#include "../2risc platform/2risc_memory_map_IPC.h"
#include "../2risc platform/2risc_comm_protocol.h"
#include "../2risc platform/video_api.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"


OS_EVENT *QEeventKeypad;

/*Global Variables*/
static char key_code;
static int key_flag;
static int serial_value;


/*
***************************************************************************
*                          TaskKeypad
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

void TaskKeypad()
{
    unsigned char err;
    Risc_Packet_t risc_packet;

    BspStartTicker(OS_TICKS_PER_SEC);

    MM74HC595Init();
    KeypadInit(); 
    KeySelCol(KEY_MAX_COLS);

    QEeventKeypad = OSSemCreate(0);

    while(1){
              
        OSSemPend(QEeventKeypad,  0, &err);

        if(err != OS_NO_ERR)			
            printf("\r\nKeypad: OSSemPend error!\r\n");

        //printf("#");
        
        OSTimeDlyHMSM(0, 0, 0, KEY_SCAN_DLY);   

        set_n_gpio_intr_ena(GPIO_KB_NUM, GPIO_KB0_BIT);
        
        KeySelCol(KEY_MAX_COLS);
  

    }
}





/*
***************************************************************************
*                                KeypadInit
*
* Description: Enable Keypad gpio interrupt
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void KeypadInit()
{

    key_flag = 0;
    key_code = KEYPAD_NO_KEY_BIT;

    RiscGPIOIntrInit();
    
    KeypadGpioIntrInit(); 

}





/*
***************************************************************************
*                            KeypadGpioISR
*
* Description: Keypad gpio interrupt service routine
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
void  KeypadGpioISR(void)
{   
    unsigned char err;
    Risc_Packet_t risc_packet;
    
    key_flag++; 
    if(key_flag==1){ 
        err = OSSemPost(QEeventKeypad);
        if(err != OS_NO_ERR)			
            printf("\r\nKeypad: OSSemPost error!\r\n");
         
        clr_n_gpio_intr_ena(GPIO_KB_NUM, GPIO_KB0_BIT);
        
    }

    if(key_flag==2){
                
        if(IsKeyPressed()){
            
            key_code = KeyDecode();
            
            if(key_code<KEYPAD_NO_KEY_BIT){
                
                printf("key_code=%d\n", key_code);

                risc_packet.start = RISC_PKT_START;
                risc_packet.main_type = tDVR;
                risc_packet.sub_type = tKeypad;
                risc_packet.length = 0x4;
                risc_packet.status = 0;

                *((int *)(&risc_packet.message[0])) = key_code;

                risc_packet.end = RISC_PKT_END;

                if(TwoRiscCommSendPacket((unsigned int *)&risc_packet, RPACKET_LEN) == SUCCESSFUL) {
                    RiscFlagSet();    // Set risc1 flag to interrupt risc1 
                    //debug("\r\n KeypadGpioISR: send packet succed!\r\n");
                } else{ 
                    debug("\r\n KeypadGpioISR: send packet error!\r\n");
                }
                key_code = KEYPAD_NO_KEY_BIT;
            }
        }
        
        key_flag = 0;

    } 

    
}

/*
***************************************************************************
*                                KeypadGpioIntrInit
*
* Description: Enable Keypad gpio interrupt
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
int KeypadGpioIntrInit(void)
{
    
    /* enable gpio intr */

    set_n_gpio_in(GPIO_KB_NUM, GPIO_KB0_BIT);
    set_n_gpio(GPIO_KB_NUM, GPIO_KB0_BIT);

    set_n_gpio_intr_pol(GPIO_KB_NUM, GPIO_KB0_BIT, 0);    //set active low
    set_n_gpio_intr_ena(GPIO_KB_NUM, GPIO_KB0_BIT);

     
    return SUCCESSFUL;
}




void set_n_gpio(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        set_gpio(start_pos);
        start_pos++;
    }
}

void clr_n_gpio(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        clr_gpio(start_pos);
        start_pos++;
    }
}

void set_n_gpio_out(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        set_gpio_out(start_pos);
        start_pos++;
    }
}

void set_n_gpio_in(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        set_gpio_in(start_pos);
        start_pos++;
    }
}
void clr_n_gpio_intr_ena(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        clr_gpio_int_ena(start_pos);
        start_pos++;
    }
}

void set_n_gpio_intr_ena(int n, int start_pos)
{
    int i;

    for(i=0;i<n;i++){ 
        set_gpio_int_ena(start_pos);
        start_pos++;
    }
}

void set_n_gpio_intr_pol(int n, int start_pos, int pol)
{
    int i;

    for(i=0;i<n;i++){ 
        set_gpio_int_pol(start_pos, pol);
        start_pos++;
    }
}


void MM74HC595Init()
{
    int i;
    int serial_data_bit; 
    
    set_n_gpio_out(GPIO_MM74HC595_NUM, GPIO_SCK595_BIT);

    clr_n_gpio(GPIO_MM74HC595_NUM, GPIO_SCK595_BIT);
    
    serial_value = 0x10000000;//init buzzer as high(no voice)---vincent

    for(i=0;i<32;i++)
    {
        serial_data_bit = (serial_value>>(32-1-i))&0x1;
        if(serial_data_bit) set_gpio(GPIO_SER595_BIT);
        else clr_gpio(GPIO_SER595_BIT);
        
        set_gpio(GPIO_SCK595_BIT);

        delay(1, 10);
        //rRISC0_TIMER = 0x80000064;

        clr_gpio(GPIO_SCK595_BIT);

        delay(1, 10);
    }
    set_gpio(GPIO_RCK595_BIT);

    delay(1, 10);

    clr_gpio(GPIO_RCK595_BIT);

}



int MM74HC595(int serial_data)
{
    int i;
    int serial_data_bit;   

    set_n_gpio_out(GPIO_MM74HC595_NUM, GPIO_SCK595_BIT);

    clr_n_gpio(GPIO_MM74HC595_NUM, GPIO_SCK595_BIT);

    for(i=0;i<32;i++)
    {

        serial_data_bit = (serial_data>>(32-1-i))&0x1;
        if(serial_data_bit) set_gpio(GPIO_SER595_BIT);
        else clr_gpio(GPIO_SER595_BIT);
        
        set_gpio(GPIO_SCK595_BIT);

        delay(1, 10);//less delay time ,avoid vout isr

        clr_gpio(GPIO_SCK595_BIT);

        delay(1, 10);//less delay time ,avoid vout isr

    }
    set_gpio(GPIO_RCK595_BIT);   
   
    delay(1, 10);   
   
    clr_gpio(GPIO_RCK595_BIT);

}

void Dvr_AlarmOutWriteTo595(int alarm_no,int value)
{
    if(value)
        serial_value |= (1<<(alarm_no+24));
    else
        serial_value &= ~(1<<(alarm_no+24));
    printf("\n serial_value 0x%x\n",serial_value);                    
    MM74HC595(serial_value);
}


void StatusLed(int status_led)
{
    int serial_data;
#ifndef DVR_BOARD
    serial_data = (status_led<<16)|(serial_value&0xffff);               //status led bit :16~31bits
#else
    serial_data = (status_led<<8)|(serial_value&0xff0000ff);         //status led bit :8~23bits
#endif
    serial_value = serial_data;

    MM74HC595(serial_data);
}

void Buzzer(int buzzer)
{
    int serial_data;
#ifndef DVR_BOARD
    //if(buzzer) serial_data = (0x1<<12)|(serial_value&0xffffefff);       //buzzer bit:12bit , high 
    //else serial_data = serial_value&0xffffefff;
    serial_data = ((buzzer&0x1)<<12)|(serial_value&0xffffefff);               //buzzer bit:12bit , high 
#else
    //if(buzzer) serial_data = ((~buzzer&0x1)<<28)|(serial_value&0xfeffffff);                         //buzzer bit:28bit , low
    if(buzzer==0)
        serial_data = serial_value | (1<<28);
    else
        serial_data = serial_value &(~ (1<<28));
#endif

    serial_value = serial_data;

    MM74HC595(serial_data);


}



void KeySelCol(INT8U col)
{
    INT32U serial_data;

    if(col==KEY_MAX_COLS)serial_data = serial_value&0xffffff00;      
    else serial_data = serial_value|((~(0x1<<col))&0x3f);
    
    MM74HC595(serial_data);

}


INT8U KeyGetRow()
{
    INT8U row;
      
    set_n_gpio_in(GPIO_KB_NUM, GPIO_KB0_BIT);
    row = (rRISC_GPIO>>GPIO_KB0_BIT)&0x1f;
    row = ~(row|0xe0);

    return(row);

}


static BOOLEAN IsKeyPressed(void)
{
    if(KeyGetRow())
    {
        return(TRUE);
    }else{
        return(FALSE);
    }    

}

static INT8U KeyDecode(void)
{
    INT8U row;
    INT8U col;
    BOOLEAN done;
    INT8U row_id;
    INT8U msk;


    done = FALSE;
    col = 0;
    while(col<KEY_MAX_COLS && !done)
    {
        KeySelCol(col);
        if(IsKeyPressed()) done = TRUE;
        else col++;

    }
    
    row = KeyGetRow();

    msk = 0x1;
    row_id = 0;
    done = FALSE;
    while(row_id<KEY_MAX_ROWS && !done)
    {
        if(row & msk) {
            done = TRUE;
        } else{ 
            row_id++;
            msk<<=1;       
        }

    }
      
    
    return(row_id*KEY_MAX_COLS+col);

}



#endif


