#include "video_cfg.h"
#if INCLUDE_MT9M112
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

#include "mt9m112.h"

void SetMt9m112FrameRate(int frame_rate)
{
	return;
}

/*
*********************************************************************************************
*                     
*
* Description: 	
*
* Arguments  : none.
*
* Return     : 
*
* Notes: the iic is 16bit-length!
*********************************************************************************************
*/	
BOOL SetMt9m112SingleRegister( int reg, int input)
{
	return 0;
}

/*
*********************************************************************************************
*                                  Camera initialize
*
* Description:     This routine read the array of MT9M112_init, then write the value in the array
                                to MT9M112 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
VOID HwMt9m112Init(VOID)
{
    int totalN = sizeof(mt9m112_camera_init) / sizeof( mt9m112_camera_init[0]);
    int i;   

    /* reset camera */

	IICWriteWord(MT9M112_CAMERA_IIC_ADDR,0xF0,0x0000,0);
	IICWriteWord(MT9M112_CAMERA_IIC_ADDR,0x0D,0x0021,0);//bit 5 SOC Reset, bit 0 Sensor Reset
 	IICWriteWord(MT9M112_CAMERA_IIC_ADDR,0x0D,0x0008,0);

    /* delay */
    for(i = 0; i<= 0xFFFF; i++);    
    
    printf("totalN=%d",totalN);


    for (i=0; i<sizeof(mt9m112_camera_init)/sizeof(int); i+=2)
    {    
        IICWriteWord(MT9M112_CAMERA_IIC_ADDR, mt9m112_camera_init[i], mt9m112_camera_init[i+1], 0); 
    }       
    SendAllCamRegValToLinux();

}


int GetMt9m112ID(void)
{
	int device_id;

	if(IICReadWord(MT9M112_CAMERA_IIC_ADDR, MT9M112_CHIP_VERSION_ADDR, &device_id, 0))
		return IIC_ERROR;

	printf("deviceid = 0x%x\n",device_id);
	return device_id;
}

#endif

