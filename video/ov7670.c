#include "video_cfg.h"
#if INCLUDE_OV7670
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

#include "ov7670.h"
#include "Camera.h"

extern unsigned int gFreqFromCrystal;

/*
*********************************************************************************************
*                                  HwOV7670Init
*
* Description:     This routine read the array of OV7670_init, then write the value in the array
                                to OV7670 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

//0x11,0x92,0x93
const BYTE gMode_OV7670_FrameRate[4][6][3]=
{//5fps is wrong
//        			5fps				10fps			15fps				20fps			25fps			30fps
/*QVGA*/0x05, 0xa5, 0x00,    0x03, 0xff, 0x00,     0x03, 0x00, 0x00,     0x02, 0x00,0x00,    0x01, 0x60,0x00,    0x01, 0x00,0x00,
/*VGA*/  0x05, 0xa5, 0x00,    0x03, 0xff, 0x00,     0x03, 0x00, 0x00,     0x02, 0x00,0x00,    0x01, 0x60,0x00,    0x01, 0x00,0x00,
/*QCIF*/ 0x05, 0xa5, 0x00,    0x03, 0xff, 0x00,     0x03, 0x00, 0x00,     0x02, 0x00,0x00,    0x01, 0x60,0x00,    0x01, 0x00,0x00,
/*CIF*/   0x05, 0xa5, 0x00,    0x03, 0xff, 0x00,     0x03, 0x00, 0x00,     0x02, 0x00,0x00,    0x01, 0x60,0x00,    0x01, 0x00,0x00
};

int Default_Frame_Rate = 30;
int Frame_Rate_Param = 512;
void SetOV7670FrameRate(int frame_rate)
{
    int Reg_Value = 0;
    int i;
    int Reg92 = 0;
    int Reg93 = 0;    
    printf("\nSetOV7670FrameRate: gFrameRate = %d!\n", gFrameRate);

    if(gFrameRate >= 1 && gFrameRate <= 11)
        frame_rate = 7;
    else if(gFrameRate >= 12 && gFrameRate <= 22)
        frame_rate = 15;
    else if(gFrameRate >= 23)
        frame_rate = 30;
    
    printf("\nFramerate is = %d!\n", gFrameRate);

    printf("***************************\r\n");
    printf("in SetFrameRate\r\n");
    printf("***************************\r\n");
        

        Reg_Value = Frame_Rate_Param *( Default_Frame_Rate - frame_rate ) / frame_rate;
        //printf("frame:%d, Reg_Value:%d, that is:0x%x\r\n", i, Reg_Value, Reg_Value );

        Reg92 = Reg_Value & 0xff;
        Reg93 = ( Reg_Value & 0xff00) >> 8;
        OV7670_camera_init_frame_rate[1] = (BYTE) Reg92;
        OV7670_camera_init_frame_rate [3] = (BYTE) Reg93;

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
* Notes: the iic is byte-length!
*
*********************************************************************************************
*/	
BOOL SetOV7670SingleRegister( BYTE reg, BYTE input)
{

    unsigned char status = FAILED;
    BOOL ret;
    
    static Risc_Packet_t pkt = {0};
    pkt.start = RISC_PKT_START;
    pkt.end = RISC_PKT_END;
    pkt.main_type = tVideo;
	   
    BYTE *p=NULL;
    int n=0;
    int iwCaseN = 0;//case calculate
    int totolN =0;
    int val =-1;
   // GetCamera_Sensor_Init(&p, &totolN);
    p = OV7670_camera_init;
    totolN = sizeof(OV7670_camera_init) / sizeof( OV7670_camera_init[0]);

    int iwRegN = -1;

    while(p < &OV7670_camera_init[totolN])
    {
        if((*p) == reg)
            iwRegN = n;
        p+=2;
        n+=2;
    }

    if(iwRegN == -1)
    {
        debug("\nSetCamSingleRegister: don't have this REG %x in the array!\n" , reg);
    }

    ret = IICWriteByte(OV7670_CAMERA_IIC_ADDR,  reg, input, 0);
	
    if(ret == IIC_OK) {
        if(iwRegN != -1)
        {	
            OV7670_camera_init[iwRegN + 1] = input;	///////////
        }

        IICReadByte(OV7670_CAMERA_IIC_ADDR, reg, &val, 0);

        pkt.sub_type = tGetCamReg;
        pkt.length = 8;
        *(int *)(pkt.message) = reg;
        *(int *)(((int *)pkt.message) + 1) = val;
	  
      	 if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1     
        } else {
            printf("\nSetCamSingleRegister: TwoRiscCommSendPacket error!\n");
        }

        debug("\nSetCamSingleRegister: setReg %x == %x  succeed! And read %d!\n", reg, input, val);

        return ret;
		
    } else {    // IIC_ERROR
        debug("\nSetCamSingleRegister: setReg %x == %x  failed!", reg, input);    
        return ret;
    }	          
}

/*
*********************************************************************************************
*                                            SetOV7670Brightness    
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

void SetOV7670Brightness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Brightness: gtCameraSetting.brightness = %d,\n ", gtCameraSetting.brightness);

    switch(gtCameraSetting.brightness) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_0) / sizeof ( ov7670_camera_init_brightness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_0[i], ov7670_camera_init_brightness_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_1) / sizeof ( ov7670_camera_init_brightness_1[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_1[i], ov7670_camera_init_brightness_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_2) / sizeof ( ov7670_camera_init_brightness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_2[i], ov7670_camera_init_brightness_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_3) / sizeof ( ov7670_camera_init_brightness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_3[i], ov7670_camera_init_brightness_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_4) / sizeof (ov7670_camera_init_brightness_4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_4[i], ov7670_camera_init_brightness_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_5) / sizeof (ov7670_camera_init_brightness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_5[i], ov7670_camera_init_brightness_5[i+1], 0);
            break;
    
        case 6:     // 6
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_6) / sizeof (ov7670_camera_init_brightness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_6[i], ov7670_camera_init_brightness_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_7) / sizeof (ov7670_camera_init_brightness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_7[i], ov7670_camera_init_brightness_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_8) / sizeof (ov7670_camera_init_brightness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_8[i], ov7670_camera_init_brightness_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_9) / sizeof (ov7670_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_9[i], ov7670_camera_init_brightness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7670_camera_init_brightness_9) / sizeof (ov7670_camera_init_brightness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_brightness_9[i], ov7670_camera_init_brightness_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7670Contrast    
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

void SetOV7670Contrast(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Contrast: gtCameraSetting.contrast = %d,\n ", gtCameraSetting.contrast);

    switch(gtCameraSetting.contrast) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_0) / sizeof ( ov7670_camera_init_contrast_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_0[i], ov7670_camera_init_contrast_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_1) / sizeof ( ov7670_camera_init_contrast_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_1[i], ov7670_camera_init_contrast_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_2) / sizeof ( ov7670_camera_init_contrast_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_2[i], ov7670_camera_init_contrast_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_3) / sizeof ( ov7670_camera_init_contrast_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_3[i], ov7670_camera_init_contrast_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_4) / sizeof ( ov7670_camera_init_contrast_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_4[i], ov7670_camera_init_contrast_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_5) / sizeof ( ov7670_camera_init_contrast_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_5[i], ov7670_camera_init_contrast_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_6) / sizeof ( ov7670_camera_init_contrast_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_6[i], ov7670_camera_init_contrast_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_7) / sizeof (ov7670_camera_init_contrast_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_7[i], ov7670_camera_init_contrast_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_8) / sizeof (ov7670_camera_init_contrast_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_8[i], ov7670_camera_init_contrast_8[i+1], 0);
            break;

        case 9:     // 9
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_9) / sizeof (ov7670_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_9[i], ov7670_camera_init_contrast_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7670_camera_init_contrast_9) / sizeof (ov7670_camera_init_contrast_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_contrast_9[i], ov7670_camera_init_contrast_9[i+1], 0);
            break;

    }
}

/*
*********************************************************************************************
*                                            SetOV7670Saturation    
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

void SetOV7670Saturation(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Saturation: gtCameraSetting.saturation = %d,\n", gtCameraSetting.saturation);

    switch(gtCameraSetting.saturation) {
        case 0:     // 0
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_0) / sizeof ( ov7670_camera_init_saturation_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_0[i], ov7670_camera_init_saturation_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_1) / sizeof ( ov7670_camera_init_saturation_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_1[i], ov7670_camera_init_saturation_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_2) / sizeof ( ov7670_camera_init_saturation_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_2[i], ov7670_camera_init_saturation_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_3) / sizeof ( ov7670_camera_init_saturation_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_3[i], ov7670_camera_init_saturation_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_4) / sizeof ( ov7670_camera_init_saturation_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_4[i], ov7670_camera_init_saturation_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_5) / sizeof ( ov7670_camera_init_saturation_5[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_5[i], ov7670_camera_init_saturation_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_6) / sizeof ( ov7670_camera_init_saturation_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_6[i], ov7670_camera_init_saturation_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_7) / sizeof (ov7670_camera_init_saturation_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_7[i], ov7670_camera_init_saturation_7[i+1], 0);
            break;

       case 8:     // 8
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_8) / sizeof (ov7670_camera_init_saturation_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_8[i], ov7670_camera_init_saturation_8[i+1], 0);
            break;

       case 9:     // 9
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_9) / sizeof (ov7670_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_9[i], ov7670_camera_init_saturation_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7670_camera_init_saturation_9) / sizeof (ov7670_camera_init_saturation_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_saturation_9[i], ov7670_camera_init_saturation_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                            SetOV7670Hue    
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

void SetOV7670Hue(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Hue: gtCameraSetting.hue = %d,\n ", gtCameraSetting.hue);

    switch(gtCameraSetting.hue) {
        case 0:     //0
            reg_volatile_num = sizeof(ov7670_camera_init_hue_0) / sizeof ( ov7670_camera_init_hue_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_0[i], ov7670_camera_init_hue_0[i+1], 0);
            break;

        case 1:     // 1
            reg_volatile_num = sizeof(ov7670_camera_init_hue_1) / sizeof ( ov7670_camera_init_hue_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_1[i], ov7670_camera_init_hue_1[i+1], 0);
            break;

        case 2:     // 2
            reg_volatile_num = sizeof(ov7670_camera_init_hue_2) / sizeof ( ov7670_camera_init_hue_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_2[i], ov7670_camera_init_hue_2[i+1], 0);
            break;

        case 3:     // 3
            reg_volatile_num = sizeof(ov7670_camera_init_hue_3) / sizeof ( ov7670_camera_init_hue_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_3[i], ov7670_camera_init_hue_3[i+1], 0);
            break;

        case 4:     // 4
            reg_volatile_num = sizeof(ov7670_camera_init_hue_4) / sizeof ( ov7670_camera_init_hue_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_4[i], ov7670_camera_init_hue_4[i+1], 0);
            break;

        case 5:     // 5
            reg_volatile_num = sizeof(ov7670_camera_init_hue_5) / sizeof ( ov7670_camera_init_hue_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_5[i], ov7670_camera_init_hue_5[i+1], 0);
            break;

        case 6:     // 6
            reg_volatile_num = sizeof(ov7670_camera_init_hue_6) / sizeof ( ov7670_camera_init_hue_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_6[i], ov7670_camera_init_hue_6[i+1], 0);
            break;

        case 7:     // 7
            reg_volatile_num = sizeof(ov7670_camera_init_hue_7) / sizeof (ov7670_camera_init_hue_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_7[i], ov7670_camera_init_hue_7[i+1], 0);
            break;

        case 8:     // 8
            reg_volatile_num = sizeof(ov7670_camera_init_hue_8) / sizeof (ov7670_camera_init_hue_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_8[i], ov7670_camera_init_hue_8[i+1], 0);
            break;

        case 9:     //9
            reg_volatile_num = sizeof(ov7670_camera_init_hue_9) / sizeof (ov7670_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_9[i], ov7670_camera_init_hue_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7670_camera_init_hue_9) / sizeof (ov7670_camera_init_hue_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_hue_9[i], ov7670_camera_init_hue_9[i+1], 0);
            break;
    }
}

/*
*********************************************************************************************
*                                            SetOV7670Sharpness    
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

void SetOV7670Sharpness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Sharpness: gtCameraSetting.sharpness = %d,\n ", gtCameraSetting.sharpness);

    switch(gtCameraSetting.sharpness) {
        case 0:     // -5
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_0) / sizeof ( ov7670_camera_init_sharpness_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_0[i], ov7670_camera_init_sharpness_0[i+1], 0);
            break;

        case 1:     // -4
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_1) / sizeof ( ov7670_camera_init_sharpness_1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_1[i], ov7670_camera_init_sharpness_1[i+1], 0);
            break;

        case 2:     // -3
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_2) / sizeof ( ov7670_camera_init_sharpness_2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_2[i], ov7670_camera_init_sharpness_2[i+1], 0);
            break;

        case 3:     // -2
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_3) / sizeof ( ov7670_camera_init_sharpness_3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_3[i], ov7670_camera_init_sharpness_3[i+1], 0);
            break;

        case 4:     // -1
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_4) / sizeof ( ov7670_camera_init_sharpness_4[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_4[i], ov7670_camera_init_sharpness_4[i+1], 0);
            break;

        case 5:     // 0
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_5) / sizeof ( ov7670_camera_init_sharpness_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_5[i], ov7670_camera_init_sharpness_5[i+1], 0);
            break;

        case 6:     // +1
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_6) / sizeof ( ov7670_camera_init_sharpness_6 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_6[i], ov7670_camera_init_sharpness_6[i+1], 0);
            break;

        case 7:     // +2
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_7) / sizeof (ov7670_camera_init_sharpness_7 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_7[i], ov7670_camera_init_sharpness_7[i+1], 0);
            break;

        case 8:     // +3
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_8) / sizeof (ov7670_camera_init_sharpness_8 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_8[i], ov7670_camera_init_sharpness_8[i+1], 0);
            break;

        case 9:     // +4
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_9) / sizeof (ov7670_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_9[i], ov7670_camera_init_sharpness_9[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(ov7670_camera_init_sharpness_9) / sizeof (ov7670_camera_init_sharpness_9 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_sharpness_9[i], ov7670_camera_init_sharpness_9[i+1], 0);
            break;
    }
}



#if 0
void SetOV7670Effect(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Effect: gtCameraSetting.effect = %d,\n ", gtCameraSetting.effect);

    switch(gtCameraSetting.effect) {
        case 0:     // purple
            reg_volatile_num = sizeof(ov7670_camera_init_effect_purple) / sizeof ( ov7670_camera_init_effect_purple [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_purple[i], ov7670_camera_init_effect_purple[i+1], 0);
            break;

        case 1:     // bluish
            reg_volatile_num = sizeof(ov7670_camera_init_effect_bluish) / sizeof ( ov7670_camera_init_effect_bluish[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_bluish[i], ov7670_camera_init_effect_bluish[i+1], 0);
            break;

        case 2:     // bw_negative
            reg_volatile_num = sizeof(ov7670_camera_init_effect_bw_negative) / sizeof ( ov7670_camera_init_effect_bw_negative [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_bw_negative[i], ov7670_camera_init_effect_bw_negative[i+1], 0);
            break;

        case 3:     // black and white
            reg_volatile_num = sizeof(ov7670_camera_init_effect_bw) / sizeof ( ov7670_camera_init_effect_bw [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_bw[i], ov7670_camera_init_effect_bw[i+1], 0);
            break;

        case 4:     // greenish
            reg_volatile_num = sizeof(ov7670_camera_init_effect_greenish) / sizeof (ov7670_camera_init_effect_greenish [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_greenish[i], ov7670_camera_init_effect_greenish[i+1], 0);
            break;

        case 5:     // negative
            reg_volatile_num = sizeof(ov7670_camera_init_effect_negative) / sizeof (ov7670_camera_init_effect_negative [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_negative[i], ov7670_camera_init_effect_negative[i+1], 0);
            break;

        case 6:     // normal
            reg_volatile_num = sizeof(ov7670_camera_init_effect_normal) / sizeof (ov7670_camera_init_effect_normal [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_normal[i], ov7670_camera_init_effect_normal[i+1], 0);
            break;

        case 7:     // redish
            reg_volatile_num = sizeof(ov7670_camera_init_effect_redish) / sizeof (ov7670_camera_init_effect_redish [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_redish[i], ov7670_camera_init_effect_redish[i+1], 0);
            break;
            
        default:
            reg_volatile_num = sizeof(ov7670_camera_init_effect_normal) / sizeof (ov7670_camera_init_effect_normal [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, ov7670_camera_init_effect_normal[i], ov7670_camera_init_effect_normal[i+1], 0);

            break;
    }
}

#endif
/*
*********************************************************************************************
*                                            SetOV7670Lightmode    
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

void SetOV7670Lightmode(void)
{
    int i;
    int reg_volatile_num;

    printf("SetOV7670Lightmode: gtCameraSetting.lightmode = %d,\n ", gtCameraSetting.lightmode);

    switch(gtCameraSetting.lightmode) 
    {
            case 0:         //normal light mode
                  reg_volatile_num = sizeof (ov7670_camera_lowlight_dis) /sizeof (ov7670_camera_lowlight_dis[0]);
                  for (i = 0; i<reg_volatile_num; i +=2)
                  IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_lowlight_dis[i],ov7670_camera_lowlight_dis[i+1],0);
                break;      
                
            case 1:         //the camera is set to be low mode
                  IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_lowlight_en[0],ov7670_camera_lowlight_en[1],0);
                break;
                
            default:
                printf("Error Lightmode.\n");
                break;
    }
}

/*
*********************************************************************************************
*                                            SetOV7670Frequency    
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

void SetOV7670Frequency(void)
{
    int i;
    int reg_volatile_num;
    ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
    

  //  gtCameraSetting.frequency = 0;
    printf("SetOV7670Frequency: gtCameraSetting.frequency = %d,\n ", gtCameraSetting.frequency);

    switch(gtCameraSetting.frequency) 
    {
          case 0:           //the max frame rate is 25fps  50Hz
                switch (mode)
                {
                    case VGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_vga_external) /sizeof (ov7670_camera_50hz_vga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_vga_external[i],ov7670_camera_50hz_vga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_vga_internal) /sizeof (ov7670_camera_50hz_vga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_vga_internal[i],ov7670_camera_50hz_vga_internal[i+1],0);
                         }
                         break;

                    case QVGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qvga_external) /sizeof (ov7670_camera_50hz_qvga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qvga_external[i],ov7670_camera_50hz_qvga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qvga_internal) /sizeof (ov7670_camera_50hz_qvga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qvga_internal[i],ov7670_camera_50hz_qvga_internal[i+1],0);
                         }
                         break;

                    case QQVGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qcif_external) /sizeof (ov7670_camera_50hz_qcif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qcif_external[i],ov7670_camera_50hz_qcif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qcif_internal) /sizeof (ov7670_camera_50hz_qcif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qcif_internal[i],ov7670_camera_50hz_qcif_internal[i+1],0);
                         }
                         break;
                     
                    case CIF:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_cif_external) /sizeof (ov7670_camera_50hz_cif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_cif_external[i],ov7670_camera_50hz_cif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_cif_internal) /sizeof (ov7670_camera_50hz_cif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_cif_internal[i],ov7670_camera_50hz_cif_internal[i+1],0);
                         }
                         break;
                     
                    case QCIF:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qcif_external) /sizeof (ov7670_camera_50hz_qcif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qcif_external[i],ov7670_camera_50hz_qcif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_qcif_internal) /sizeof (ov7670_camera_50hz_qcif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_qcif_internal[i],ov7670_camera_50hz_qcif_internal[i+1],0);
                         }
                         break;
                         
                     default:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_vga_external) /sizeof (ov7670_camera_50hz_vga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_vga_external[i],ov7670_camera_50hz_vga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_50hz_vga_internal) /sizeof (ov7670_camera_50hz_vga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_50hz_vga_internal[i],ov7670_camera_50hz_vga_internal[i+1],0);
                         }
                         break;
                }

                break;
                
          case 1:           //the max frame rate is 30fps   60Hz
                switch (mode)
                {
                    case VGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_vga_external) /sizeof (ov7670_camera_60hz_vga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_vga_external[i],ov7670_camera_60hz_vga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_vga_internal) /sizeof (ov7670_camera_60hz_vga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_vga_internal[i],ov7670_camera_60hz_vga_internal[i+1],0);
                         }
                         break;

                    case QVGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qvga_external) /sizeof (ov7670_camera_60hz_qvga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qvga_external[i],ov7670_camera_60hz_qvga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qvga_internal) /sizeof (ov7670_camera_60hz_qvga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qvga_internal[i],ov7670_camera_60hz_qvga_internal[i+1],0);
                         }
                         break;

                    case QQVGA:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qcif_external) /sizeof (ov7670_camera_60hz_qcif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qcif_external[i],ov7670_camera_60hz_qcif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qcif_internal) /sizeof (ov7670_camera_60hz_qcif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qcif_internal[i],ov7670_camera_60hz_qcif_internal[i+1],0);
                         }
                         break;
                     
                    case CIF:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_cif_external) /sizeof (ov7670_camera_60hz_cif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_cif_external[i],ov7670_camera_60hz_cif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_cif_internal) /sizeof (ov7670_camera_60hz_cif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_cif_internal[i],ov7670_camera_60hz_cif_internal[i+1],0);
                         }
                         break;
                     
                    case QCIF:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qcif_external) /sizeof (ov7670_camera_60hz_qcif_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qcif_external[i],ov7670_camera_60hz_qcif_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_qcif_internal) /sizeof (ov7670_camera_60hz_qcif_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_qcif_internal[i],ov7670_camera_60hz_qcif_internal[i+1],0);
                         }
                         break;
                         
                     default:
                         if(gFreqFromCrystal ==TRUE) {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_vga_external) /sizeof (ov7670_camera_60hz_vga_external[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_vga_external[i],ov7670_camera_60hz_vga_external[i+1],0);
                         } else {
                             reg_volatile_num = sizeof (ov7670_camera_60hz_vga_internal) /sizeof (ov7670_camera_60hz_vga_internal[0]);
                             for (i = 0; i<reg_volatile_num; i +=2)
                                 IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_60hz_vga_internal[i],ov7670_camera_60hz_vga_internal[i+1],0);
                         }
                         break;
                }
                break;
                
          default:
                  printf("Error Frequency.\n");
                break;
    }
    
}

/*
*********************************************************************************************
*                                            SetOV7670Homemode    
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

void SetOV7670Homemode(void)
{
    int i;
    int reg_volatile_num;

//    gtCameraSetting.homemode = 1;
    printf("SetOV7670Homemode: gtCameraSetting.homemode = %d,\n ", gtCameraSetting.homemode);

    switch(gtCameraSetting.homemode) 
    {
          case 0:
                  reg_volatile_num = sizeof (ov7670_camera_indoor) /sizeof (ov7670_camera_indoor[0]);
                  for (i = 0; i<reg_volatile_num; i +=2)
                  IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_indoor[i],ov7670_camera_indoor[i+1],0);
                break;

          case 1:
                  reg_volatile_num = sizeof (ov7670_camera_outdoor) /sizeof (ov7670_camera_outdoor[0]);
                  for (i = 0; i<reg_volatile_num; i +=2)
                  IICWriteByte(OV7670_CAMERA_IIC_ADDR,ov7670_camera_outdoor[i],ov7670_camera_outdoor[i+1],0);
                break;
                
          default:
                break;
    }
    
}

/*
*********************************************************************************************
*                                            SetOV7670ImageMirror    
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

void SetOV7670ImageMirror(void)
{
    int read_data;
    
    printf("SetOV7670ImageMirror: gtCameraSetting.image_mirror = %d,\n ", gtCameraSetting.image_mirror);

    IICReadByte(OV7670_CAMERA_IIC_ADDR, 0x1e, &read_data, 0);

    if (1 == gtCameraSetting.image_mirror) {
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, 0x1e, read_data | (1 << 5), 0);
    } else if (0 == gtCameraSetting.image_mirror) {
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, 0x1e, read_data & (~(1 << 5)), 0);
    }
}

/*
*********************************************************************************************
*                                            SetOV7670ImageFlip    
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

void SetOV7670ImageFlip(void)
{
    int read_data;
    
    printf("SetOV7670ImageFlip: gtCameraSetting.image_flip = %d,\n ", gtCameraSetting.image_flip);

    IICReadByte(OV7670_CAMERA_IIC_ADDR, 0x1e, &read_data, 0);

    if (1 == gtCameraSetting.image_flip) {
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, 0x1e, read_data | (1 << 4), 0);
    } else if (0 == gtCameraSetting.image_flip) {
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, 0x1e, read_data & (~(1 << 4)), 0);
    }
}

/*
*********************************************************************************************
*                                            HwOV7670Init
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

VOID HwOV7670Init(VOID)
{   
    int totalN = sizeof(OV7670_camera_init) / sizeof( OV7670_camera_init[0]);
    int i;
    int reg_template_num = sizeof ( OV7670_camera_init) /sizeof ( OV7670_camera_init[0] );
    int reg_volatile_num = sizeof ( OV7670_camera_init_vga_volatile ) / sizeof ( OV7670_camera_init_vga_volatile [ 0 ] );
    ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
    printf("\r\nmode is%d\r\n", mode );    

    /* reset camera */
    IICWriteByte(OV7670_CAMERA_IIC_ADDR, 0x12, 0x80, 0);

    /* delay */
    for(i = 0; i<= 0xFFFF; i++);    

    SetOV7670FrameRate(gFrameRate);
    
    printf("totalN=%d",totalN);
    for (i=0; i<reg_template_num; i+=2)
    {
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init[i], OV7670_camera_init[i+1], 0);      
    }

    switch ( mode )
    {
        case VGA:
            printf("\r\nVGA\r\n");
            reg_volatile_num = sizeof( OV7670_camera_init_vga_volatile ) /sizeof ( OV7670_camera_init_vga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_vga_volatile[i],OV7670_camera_init_vga_volatile[i+1], 0);                                  
            break;

        case QVGA:
            printf("\r\nQVGA\r\n");
            reg_volatile_num = sizeof( OV7670_camera_init_qvga_volatile ) /sizeof(OV7670_camera_init_qvga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qvga_volatile [i], OV7670_camera_init_qvga_volatile [i+1], 0);
            break;

        case QQVGA:
            printf("\r\nQQVGA\r\n");
            reg_volatile_num = sizeof( OV7670_camera_init_qcif_volatile ) /sizeof(OV7670_camera_init_qcif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qcif_volatile [i], OV7670_camera_init_qcif_volatile [i+1], 0);
            break;

        case CIF:
            printf("\r\nCIF\r\n");
            reg_volatile_num = sizeof ( OV7670_camera_init_cif_volatile )/ sizeof ( OV7670_camera_init_cif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_cif_volatile[i], OV7670_camera_init_cif_volatile[i+1], 0);
            break;

        case QCIF:          
            printf("\r\nQCIF\r\n");
#if 0  
            reg_volatile_num = sizeof ( OV7670_camera_init_qcif_volatile ) / sizeof ( OV7670_camera_init_qcif_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qcif_volatile [i], OV7670_camera_init_qcif_volatile [i+1], 0);                
#endif            

            reg_volatile_num = sizeof ( OV7670_camera_init_qcif_volatile )/ sizeof ( OV7670_camera_init_qcif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qcif_volatile[i], OV7670_camera_init_qcif_volatile[i+1], 0);
            break;

        case NTSC_CIF:
            printf("\r\nNTSC_CIF\r\n");
            reg_volatile_num = sizeof ( OV7670_camera_init_cif_volatile )/ sizeof ( OV7670_camera_init_cif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_cif_volatile[i], OV7670_camera_init_cif_volatile[i+1], 0);
            break;

        case NTSC_QCIF:
            printf("\r\nNTSC_QCIF\r\n");
            reg_volatile_num = sizeof ( OV7670_camera_init_qcif_volatile )/ sizeof ( OV7670_camera_init_qcif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qcif_volatile[i], OV7670_camera_init_qcif_volatile[i+1], 0);
            break;

        case SUB_QCIF:                                    //why not use qcif:when use qcif the image will become green and red.
            printf("\r\nSUB_QCIF\r\n");
            reg_volatile_num = sizeof ( OV7670_camera_init_qvga_volatile )/ sizeof ( OV7670_camera_init_qvga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_qvga_volatile[i], OV7670_camera_init_qvga_volatile[i+1], 0);
            break;

        default:
           printf("\r\nVGA\r\n");
            reg_volatile_num = sizeof( OV7670_camera_init_vga_volatile ) /sizeof ( OV7670_camera_init_vga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_vga_volatile[i],OV7670_camera_init_vga_volatile[i+1], 0);                                  
            break;
    }
    
    reg_volatile_num = sizeof ( OV7670_camera_init_frame_rate ) / sizeof ( OV7670_camera_init_frame_rate [0] );
    for ( i = 0; i < reg_volatile_num; i += 2 )
        IICWriteByte(OV7670_CAMERA_IIC_ADDR, OV7670_camera_init_frame_rate [i], OV7670_camera_init_frame_rate [i+1], 0);


    SetOV7670Brightness();   
    SetOV7670Contrast();
    SetOV7670Saturation();
    SetOV7670Hue();
    SetOV7670Sharpness();
    
    SetOV7670Lightmode();
    SetOV7670Frequency();
    SetOV7670Homemode();
  //  SetOV7670Effect();
    SetOV7670ImageFlip();
    SetOV7670ImageMirror();
    
    SendAllCamRegValToLinux();

}

/*
*********************************************************************************************
*                                            GetOV7670ID
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

int GetOV7670ID(void)
{
	int device_id,data,status;

	if(IICReadByte(OV7670_CAMERA_IIC_ADDR, REG_PID, &data, 0) < 0)
		return IIC_ERROR;
	device_id = data << 8;

	if(IICReadByte(OV7670_CAMERA_IIC_ADDR, REG_VER, &data, 0) < 0)
		return IIC_ERROR;
	device_id |= (data & 0xff);

	printf("deviceid = 0x%x\n",device_id);
	return device_id;
}

#endif

