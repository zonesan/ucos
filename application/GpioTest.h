/*
***************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        :     GpioTest.h                 
*                                                                         
* Description :     
*             
* Date        :                                                           
***************************************************************************
*/

//================BY VINCENT 2007.08.30===========================

#ifdef   GPIO_TEST_GLOBALS
#define  GPIO_TEST_EXT
#else
#define  GPIO_TEST_EXT  extern
#endif

#define  TASK_GPIO_TEST     1000
GPIO_TEST_EXT  OS_STK    StkTaskGpioTest[TASK_GPIO_TEST];

GPIO_TEST_EXT OS_EVENT *gptQMsgGpioTest;
GPIO_TEST_EXT void *ArrayQMsgGpioTest[2];
 void TaskGpioTest(void *data);

//================BY VINCENT 2007.08.30===========================

