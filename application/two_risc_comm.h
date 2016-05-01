/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        :                      
*                                                                         
* Description :    
*             
* Date        :                                                      
***************************************************************************
*/
#ifndef TWO_RISC_COMM_H
#define TWO_RISC_COMM_H


#ifdef   TWO_RISC_COMM_GLOBALS
#define  TWO_RISC_COMM_EXT
#else
#define  TWO_RISC_COMM_EXT  extern
#endif


/*
***************************************************************************
*                           Type Definitions
***************************************************************************
*/

/*
***************************************************************************
*                           Macrro Definitions
***************************************************************************
*/
#define  TWO_RISC_COMM_REC_NO_ENOUGH_BYTES    0x2
#define  TWO_RISC_COMM_REC_NO_ENOUGH_SPACES   0x2

/*
***************************************************************************
*                           GLOBAL VARIABLES
***************************************************************************
*/
#define  TASK_TWO_RISC_COMM       5000

TWO_RISC_COMM_EXT OS_STK   StkTaskTwoRiscComm[TASK_TWO_RISC_COMM];

TWO_RISC_COMM_EXT    int StayFrame;

/*
***************************************************************************
*                           RVO, PIP MAGNIFICATION VARIABLES
***************************************************************************
*/

#define RVO 1
#define PIP 2

#define NotScale    0
#define Times_2     1
#define Times_4     2
#define Times_8     3

#define Div_2    -1
#define Div_4    -2
#define Div_8    -3



/*
***************************************************************************
*                              FUCTION PROTYPES
***************************************************************************
*/
void TaskTwoRiscComm(void *data);
int TwoRiscCommInit(void);

int TwoRiscCommSendPacket(unsigned int *message, unsigned int len);
int TwoRiscCommRecPacket(void);
int TwoRiscCommRecPacket1(void);
int TwoRiscCommRecPacket2(unsigned int *rec);

int TwoRiscCommProcMsg(unsigned int *msg);
int TwoRiscCommProcVideo(unsigned int *msg, unsigned int sub_type, unsigned int len);
int TwoRiscCommProcSys(unsigned int *msg, unsigned int sub_type, unsigned int len);
int TwoRiscCommProcDVR(unsigned int *msg, unsigned int sub_type, unsigned int len);
#ifdef AUDIO_MODULE_INCLUDE
void TwoRiscCommProcAudio(unsigned int *msg, unsigned int sub_type, unsigned int len);
#endif

void TaskTwoRiscTestRiscFlag(void *data);

#endif
