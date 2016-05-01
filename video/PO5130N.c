#include "video_cfg.h"
#if INCLUDE_PO5130N
#include"../inc/video_cfg.h"
//#include    "ftuart.h"
#include    "comm.h"
#include "../video/VIN_API.h"
#include "sun_type_defs.h"
#include "VIN_SET.h"

#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "iic.h"

#include "video_intr.h"

#include "PO5130N.h"
#include "Camera.h"


/*
*********************************************************************************************
*                                            HwPO5130NInit    
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/

VOID HwPO5130NInit(VOID)
{
    int i;
    int value;
 //   int reg_volatile_num;

    int reg_template_num = sizeof ( po5130n_camera_init) /sizeof ( po5130n_camera_init[0] );

    for (i=0; i<reg_template_num; i+=2)
    {
        IICWriteByte(PO5130N_CAMERA_IIC_ADDR, po5130n_camera_init[i],po5130n_camera_init[i+1], 0);
    }


    for(i=0;i<0xffff;i++);


    reg_template_num = sizeof ( po5130n_camera_init) /sizeof ( po5130n_camera_init[0] );

    for (i=0; i<reg_template_num; i+=2)
    {
        IICReadByte(PO5130N_CAMERA_IIC_ADDR, po5130n_camera_init[i], &value, 0);
    }
    
    
}

int GetPO5130NID(void)
{
	int device_id,data,status;

	if(IICReadByte(PO5130N_CAMERA_IIC_ADDR, DEVICEID_H, &data, 0) < 0)
		return IIC_ERROR;
	device_id = data << 8;

	if(IICReadByte(PO5130N_CAMERA_IIC_ADDR, DEVICEID_L, &data, 0) < 0)
		return IIC_ERROR;
	device_id |= (data & 0xff);

	printf("deviceid = 0x%x\n",device_id);
	return device_id;
}

#endif
