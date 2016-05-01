/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File:         AEC.h             
*                                                                         
* Description :       
*                                                
* By:                          
*
* Date:         Aug 17, 2006                                                           
**********************************************************************************************************
*/



#include  "def_type.h"
//#include  "TYPEDEF.H"
#define  FrameLength_10ms		80
#define  FrameLength_20ms		160
#define  FrameLength_30ms		240


VOID AppAudioAECInit(LONG lwChannel,LONG lwTalk, SHORT swFrameLenth);
//VOID AppAudioAECInit(LONG lwChannel,LONG lwTalk);

//VOID AppAudioAECRun(PSHORT vsi, PSHORT vri, PSHORT vso);
VOID AppAudioAECRun(LONG lwChannel, LONG lwTalk, SHORT *vsi, SHORT *vri, SHORT *vso);

