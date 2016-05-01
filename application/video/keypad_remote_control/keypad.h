/*
***************************************************************************
*                   Copyright (c) 2007 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : 
*                                                                         
* Description : *                                                                          
*
* Date        : June 25, 2007                                                           
***************************************************************************
*/


#ifdef KEYPAD_GLOBALS
#define KEYPAD_EXTERN
#else
#define KEYPAD_EXTERN extern
#endif

#define STACK_SIZE_KEYPAD    2000
KEYPAD_EXTERN unsigned int global_keypad_stack[STACK_SIZE_KEYPAD];

#define KEYPAD_PRIO 25

#define GPIO_KB0_BIT 0
#define GPIO_KB1_BIT 1
#define GPIO_KB2_BIT 2
#define GPIO_KB3_BIT 3
#define GPIO_KB4_BIT 4
#define GPIO_SCK595_BIT 5
#define GPIO_SER595_BIT 6
#define GPIO_RCK595_BIT 7

#define GPIO_KB_NUM 5
#define GPIO_MM74HC595_NUM 3

#define KEYPAD_NO_KEY_BIT 30

#define KEY_MAX_ROWS 5
#define KEY_MAX_COLS  6

#define KEY_SCAN_DLY 200




void TaskKeypad();

void set_n_gpio(int n, int start_pos);
void clr_n_gpio(int n, int start_pos);
void set_n_gpio_out(int n, int start_pos);
void set_n_gpio_in(int n, int start_pos);
void clr_n_gpio_intr_ena(int n, int start_pos);
void set_n_gpio_intr_ena(int n, int start_pos);
void set_n_gpio_intr_pol(int n, int start_pos, int pol);


void MM74HC595Init();
int MM74HC595(int serial_data);
void StatusLed(int status_led);
void Buzzer(int buzzer);
void Dvr_AlarmOutWriteTo595(int alarm_no,int value);

void KeySelCol(INT8U col);
INT8U KeyGetRow();
static BOOLEAN IsKeyPressed(void);
static INT8U KeyDecode(void);

void KeypadInit();
void  KeypadGpioISR(void);
int KeypadGpioIntrInit(void);




