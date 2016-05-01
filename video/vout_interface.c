/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                       Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : vout_interface.c                     
*                                                                         
* Description :   
*             
* Date        : Fab.1st.2008                                                           
***************************************************************************
*/

#include "def_type.h"
#include "vout_interface.h"
#include "vout_interface_cfg.h"
#include "iic.h"

#if INCLUDE_ADV7171
extern int GetADV7171ID(void);
extern int HwADV7171Initialize(BOOL video_system);
#endif

#if INCLUDE_MTV335
extern int GetMTV335ID(void);
extern int HwMTV335Initialize(unsigned int video_system);
#endif

VoutInterface VoutInterface_list[] =
{
#if INCLUDE_ADV7171
{
    ADV7171_ID,
    "ADV7171",
    0,    //if ADV7171 is found, the value will set to be 1

    GetADV7171ID,
    HwADV7171Initialize
},
#endif

#if INCLUDE_MTV335
{
    MTV335_ID,
    "MTV335",
    0,    //if MTV335 is found, the value will set to be 1

    GetMTV335ID,
    HwMTV335Initialize
},
#endif

    {0,0,0,0,0}
};

/*
*********************************************************************************************
*                                  VoutInterfaceInit
*
* Description:finds and initializes the video output    
*
* Arguments  : output format:NTSC,PAL,LCD.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
void GetVoutInterface(void)
{
    VoutInterface *pVout;
    int device_id;
    pVout = VoutInterface_list;

    while (pVout->deviceID) {
        debug("\nDetecting %s\n", pVout->desc);
        device_id = pVout->GetDeviceID();
        //debug("device_id = 0x%x\n", device_id);
        
        if (pVout->deviceID == device_id) {
            pVout->dev_flag = 1;
            debug("Get %s\n", pVout->desc);
        } else {
            pVout->dev_flag = 0;   
            debug("Can't get %s\n", pVout->desc);
        }
        pVout++;
    }
}

/*
*********************************************************************************************
*                                  VoutInterfaceInit
*
* Description:finds and initializes the video output    
*
* Arguments  : output format:NTSC,PAL,LCD.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

void VoutInterfaceIntialize(unsigned int video_system)
{
    VoutInterface *pVout;
    pVout = VoutInterface_list;

    while (pVout->deviceID) {
        if (pVout->dev_flag) {
            debug("Initializing %s!\n", pVout->desc);

            if (pVout->VoutInterfaceInit) {
                if (pVout->VoutInterfaceInit(video_system) == SUCCESSFUL) {
                    debug("Initialize %s successful!\n", pVout->desc);
                } else 
                    debug("Initialize %s failed!\n", pVout->desc);
            }
        }
        pVout++;
    }
}

