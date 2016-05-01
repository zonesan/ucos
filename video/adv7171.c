/*
***************************************************************************
*                   Copyright (c) 2005 vBridge Microsystem, Inc.  
*                          Unpublished & Not for Publication
*                                 All Rights Reserved                   
*   
* File        : adv7171.c             
*                                                                         
* Description : Functions for ADV7171      
*                                                
* By          : Eric               
*
* Date        : Mar 3, 2006                                                           
***************************************************************************
*/

#include "vout_interface_cfg.h"

#ifdef INCLUDE_ADV7171

#include "adv7171.h"
#include "iic.h"

#define TIME_MODE_0_REG_VAL 0x00
BYTE ADV7171_INIT_NTSC[]=
{
    MODE_0_REG,                         0x00,    //0x28,    
    MODE_1_REG,                         0x3E,    //0x06,        //0x38,         
    MODE_2_REG,                         0x00,    //0x08
    MODE_3_REG,                         0x00,
    MODE_4_REG,                         0x10,    //0x00,
    TIME_MODE_0_REG,                    0x00,    //0x08,
    TIME_MODE_1_REG,                    0x00,     //0x33,
    SUB_CARRIER_FREQ_0_REG,             0x16,
    SUB_CARRIER_FREQ_1_REG,             0x7c,
    SUB_CARRIER_FREQ_2_REG,             0xf0,
    SUB_CARRIER_FREQ_3_REG,             0x21,
    SUB_CARRIER_PHASE_REG,              0x00,
    CLOSE_CAPT_EXT_DATA_0_REG,          0x00,
    CLOSE_CAPT_EXT_DATA_1_REG,          0x00,
    CLOSE_CAPT_DATA_0_REG,              0x00,
    CLOSE_CAPT_DATA_1_REG,              0x00,
    NTSC_PEDESTAL_CONTROL_0_REG,        0x00,
    NTSC_PEDESTAL_CONTROL_1_REG,        0x00,
    NTSC_PEDESTAL_CONTROL_2_REG,        0x00,
    NTSC_PEDESTAL_CONTROL_3_REG,        0x00,
    CGMS_WSS_0,                         0x00,
    CGMS_WSS_1,                         0x00,
    CGMS_WSS_2,                         0x00,
    TELETEXT_REQ_CONTROL_REG,           0x00
};

BYTE ADV7171_INIT_PAL[]=
{
    MODE_0_REG,                         0x01,    
    MODE_1_REG,                         0x38,         
    MODE_2_REG,                         0x08,
    MODE_3_REG,                         0x00,
    MODE_4_REG,                         0x00,
    TIME_MODE_0_REG,                    0x08,
    TIME_MODE_1_REG,                    0x33,
    SUB_CARRIER_FREQ_0_REG,             0xCB,
    SUB_CARRIER_FREQ_1_REG,             0x8A,
    SUB_CARRIER_FREQ_2_REG,             0x09,
    SUB_CARRIER_FREQ_3_REG,             0x2A,
    SUB_CARRIER_PHASE_REG,              0x00,
    CLOSE_CAPT_EXT_DATA_0_REG,          0x00,
    CLOSE_CAPT_EXT_DATA_1_REG,          0x00,
    CLOSE_CAPT_DATA_0_REG,              0x00,
    CLOSE_CAPT_DATA_1_REG,              0x00,
    PAL_TTX_CONTROL_0_REG,              0x00,
    PAL_TTX_CONTROL_1_REG,              0x00,
    PAL_TTX_CONTROL_2_REG,              0x00,
    PAL_TTX_CONTROL_3_REG,              0x00,
    CGMS_WSS_0,                         0x00,
    CGMS_WSS_1,                         0x00,
    CGMS_WSS_2,                         0x00,
    TELETEXT_REQ_CONTROL_REG,           0x00
};


/*
*********************************************************************************************************                 
*   
* ADV7171 initialize         
*                                                                         
* DESCRIPTION :     ADV7171 initialize function
*    
*    Input         0 for NTSC, 1 for PAL
*
*    By:               Eric                                                        
*    
*    Date:                        03 Mar, 2006   
*                                                                       
*********************************************************************************************************
*/

int HwADV7171Initialize(BOOL video_system)
{
    unsigned int read_data, status = FAILED;
    int i = 0;

    /* software reset */
    IICWriteByte(ADV7171_IIC_ADDR, TIME_MODE_0_REG, TIME_MODE_0_REG_VAL & 0x7f, 0);
    while(i < 10000) {i++;}
    IICWriteByte(ADV7171_IIC_ADDR, TIME_MODE_0_REG, TIME_MODE_0_REG_VAL | 0x80, 0);
    while(i < 10000) {i++;}
    IICWriteByte(ADV7171_IIC_ADDR, TIME_MODE_0_REG, TIME_MODE_0_REG_VAL & 0x7f, 0);
    while(i < 10000) {i++;}
    
    if (video_system == NTSC)
    {    
        for (i = 0; i < sizeof(ADV7171_INIT_NTSC) / sizeof(ADV7171_INIT_NTSC[0]); i += 2)
        {
            if(IICWriteByte(ADV7171_IIC_ADDR, ADV7171_INIT_NTSC[i], ADV7171_INIT_NTSC[i+1], 0) == IIC_ERROR)
                return FAILED; 
        }

        /* Read to verify! */
        for (i = 0; i < sizeof(ADV7171_INIT_NTSC) / sizeof(ADV7171_INIT_NTSC[0]); i += 2)
        {
            if(IICReadByteADV(ADV7171_IIC_ADDR, ADV7171_INIT_NTSC[i], &read_data, 0) == IIC_ERROR)
            {
            }
            if(read_data != ADV7171_INIT_NTSC[i+1])
            {
            }
        }
        
    } else if (video_system == PAL) {
        for (i = 0; i < sizeof(ADV7171_INIT_PAL) / sizeof(ADV7171_INIT_PAL[0]); i += 2)
        {
            if(IICWriteByte(ADV7171_IIC_ADDR, ADV7171_INIT_PAL[i], ADV7171_INIT_PAL[i+1], 0) == IIC_ERROR)
                return FAILED;
        }
        
        /* Read to verify! */
        for (i = 0; i < sizeof(ADV7171_INIT_PAL) / sizeof(ADV7171_INIT_PAL[0]); i += 2)
        {
            if(IICReadByteADV(ADV7171_IIC_ADDR, ADV7171_INIT_PAL[i], &read_data, 0) == IIC_ERROR)
            {
            }
            if(read_data != ADV7171_INIT_PAL[i+1])
            {
            }
        }          
    } else {
        printf("\nvideo output of ADV7171 is ERROR!\n");
        IICWriteByte(ADV7171_IIC_ADDR, MODE_1_REG, 0x86, 0); //if not NTSC or PAL, Enable the color bar
        return FAILED;
    }

    return SUCCESSFUL;
}

/*
*********************************************************************************************************                 
*                                           GetADV7171ID
*           
*                                                                         
* DESCRIPTION :       Get the ID of ADV7171
*    
*    Input              
*
*    By:                                                                       
*    
*    Date:                             
*                                                                       
*********************************************************************************************************
*/

int GetADV7171ID(void)
{
    int device_id;

    device_id = 0x7171;

    return device_id;
}

//Test function, enble ADV7171 color bar output
VOID ADV7171_EnableColorbar(VOID)                        
{
//  HwI2cSingleWrite(ADV7171_I2C_ADDR, MODE_1_REG, 0x86);
    IICWriteByte(ADV7171_IIC_ADDR, MODE_1_REG, 0x86, 0);
}

void Adv7171_DisableColorBar()
{
//  HwI2cSingleWrite(ADV7171_I2C_ADDR, MODE_1_REG, 0x06);
    IICWriteByte(ADV7171_IIC_ADDR, MODE_1_REG, 0x06, 0);
}

#endif     //end of #ifdef INCLUDE_ADV7171

