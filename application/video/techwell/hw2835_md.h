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
* Date        : June 9, 2007                                                           
***************************************************************************
*/






#define GPIO_MOYION_DETECTOR_BIT 11

#define STACK_SIZE_MOTIONDETECTOR    2000

unsigned int global_MotionDetector_stack[STACK_SIZE_MOTIONDETECTOR];




void HW2835MotionDetectorInit();
void  HW2835MotionDetectorGpioISR(void);
int HW2835MotionDetectorGpioIntrInit(void);
int ReadGpioState(void);

//void TaskMotionDetector();

void Hw2835ReadMDRegister(void);





