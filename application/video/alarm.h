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


#ifdef ALARM_GLOBALS
#define ALARM_EXTERN
#else
#define ALARM_EXTERN extern
#endif

#define STACK_SIZE_ALARM    2000
ALARM_EXTERN unsigned int global_alarm_stack[STACK_SIZE_ALARM];

//#define GPIO_ALARM0_BIT 14
//#define GPIO_ALARM1_BIT 15
//#define GPIO_ALARM2_BIT 16
//#define GPIO_ALARM3_BIT 17


/*Global variable*/
ALARM_EXTERN unsigned int alarm_gpio_isr_conut;
ALARM_EXTERN unsigned int alarm_pulse_count;

extern unsigned int alarm_gpio_status;
ALARM_EXTERN unsigned char alarm_gpio_isr_no;

ALARM_EXTERN unsigned  char enable_alarm;
ALARM_EXTERN unsigned  char alarm_gpio_no;
ALARM_EXTERN unsigned  char alarm_gpio_level;
extern unsigned int alarm_gpio_attribute;
extern unsigned int alarm_active_level[32];
extern unsigned int alarm_type[32];




void TaskAlarm();
void Alarm(char enable, char gpio_no, char gpio_level, char attribute);
void AlarmIntrInit();
void  AlarmLevelTriggeredGpioISR();
void  AlarmEdgeTriggeredGpioISR();
void  AlarmPulseCounterGpioISR();
void AlarmPulseCounterSendMess();



