/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : vout_interface.h                     
*                                                                         
* Description :   
*             
* Date        : Fab.1st.2008                                                           
***************************************************************************
*/

#ifndef _VOUT_INTERFACE_H
#define _VOUT_INTERFACE_H

#include "def_type.h"

#define ADV7171_ID      0x7171
#define MTV335_ID       0x335

typedef struct {
    int deviceID;   //the ID of the device
    char *desc;    //the name of the device 
    unsigned char dev_flag;  //indicate the video output device is connected or not
                             //0:not connect,1:connect.default 0,
    int (*GetDeviceID)(void);    //function pointer for get the device ID
    int (*VoutInterfaceInit)(BOOL video_system);   //function call for init camera
}VoutInterface,*pVoutInterface;


/*function prototype declaration*/

void GetVoutInterface(void);
void VoutInterfaceIntialize(unsigned int video_system);

#endif        //endif of _VOUT_INTERFACE_H

