#include "video_cfg.h"
#if INCLUDE_MT9M111
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

#include "mt9m111.h"

#include "Camera.h"

void SetMt9m111FrameRate(int frame_rate)
{
    int i;

    printf("\nSetMt9m111FrameRate: gFrameRate = %d!\n", gFrameRate);
    
    if(frame_rate == 5)
        for (i=0; i<sizeof(mt9m111_camera_init_5fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_5fps[i], mt9m111_camera_init_5fps[i+1], 0);
    else if(frame_rate == 10)
        for (i=0; i<sizeof(mt9m111_camera_init_10fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_10fps[i], mt9m111_camera_init_10fps[i+1], 0);
    else if(frame_rate == 15)
        for (i=0; i<sizeof(mt9m111_camera_init_15fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_15fps[i], mt9m111_camera_init_15fps[i+1], 0);
    else if(frame_rate == 25)
        for (i=0; i<sizeof(mt9m111_camera_init_25fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_25fps[i], mt9m111_camera_init_25fps[i+1], 0);
    else if(frame_rate == 30)
        for (i=0; i<sizeof(mt9m111_camera_init_30fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_30fps[i], mt9m111_camera_init_30fps[i+1], 0);
    else if(frame_rate == 7)
        for (i=0; i<sizeof(mt9m111_camera_init_7_5fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_7_5fps[i], mt9m111_camera_init_7_5fps[i+1], 0);
    else    // default 30 framerate
        for (i=0; i<sizeof(mt9m111_camera_init_30fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_30fps[i], mt9m111_camera_init_30fps[i+1], 0);        

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
BOOL SetMt9m111SingleRegister( int reg, int input)
{
#if 1
    unsigned char status = FAILED;
    BOOL ret;
    
    static Risc_Packet_t pkt = {0};
    pkt.start = RISC_PKT_START;
    pkt.end = RISC_PKT_END;
    pkt.main_type = tVideo;
	   
    int *p=NULL;
    int n=0;
    int iwCaseN = 0;//case calculate
    int totolN =0;
    int val =-1;
	
    if(gFrameRate == 5) {
        p = mt9m111_camera_init_5fps;
        totolN = sizeof(mt9m111_camera_init_5fps) / sizeof( mt9m111_camera_init_5fps[0]);
    } else if(gFrameRate == 10) {    // 10 framerate
        p = mt9m111_camera_init_10fps;
        totolN = sizeof(mt9m111_camera_init_10fps) / sizeof( mt9m111_camera_init_10fps[0]);
    } else if(gFrameRate == 15) {    // 15 framerate
        p = mt9m111_camera_init_15fps;
        totolN = sizeof(mt9m111_camera_init_15fps) / sizeof( mt9m111_camera_init_15fps[0]);
    } else if(gFrameRate == 25) {    // 25 framerate
        p = mt9m111_camera_init_25fps;
        totolN = sizeof(mt9m111_camera_init_25fps) / sizeof( mt9m111_camera_init_25fps[0]);
    } else if(gFrameRate == 30) {    // 30 framerate
        p = mt9m111_camera_init_30fps;
        totolN = sizeof(mt9m111_camera_init_30fps) / sizeof( mt9m111_camera_init_30fps[0]);
    } else  {    // 7.5 framerate
        p = mt9m111_camera_init_7_5fps;
        totolN = sizeof(mt9m111_camera_init_7_5fps) / sizeof( mt9m111_camera_init_7_5fps[0]);
    }                    
	
    int iwRegN = -1;

    if(gFrameRate == 5) {
        while(p < &mt9m111_camera_init_5fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else if(gFrameRate == 10) {
        while(p < &mt9m111_camera_init_10fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else if(gFrameRate == 15) {
        while(p < &mt9m111_camera_init_15fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else if(gFrameRate == 25) {
        while(p < &mt9m111_camera_init_25fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else if(gFrameRate == 30) {
        while(p < &mt9m111_camera_init_30fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else if(gFrameRate == 75) {
        while(p < &mt9m111_camera_init_7_5fps[totolN])
        {
            if((*p) == reg)
                iwRegN = n;
            p+=2;
            n+=2;
        }
    } else {
        printf("\nSetCamSingleRegisterWord: gFrameRate error = %d\n", gFrameRate);
        return;
    }        

    if(iwRegN == -1)
    {
        debug("\nSetCamSingleRegister: don't have this REG %x in the array!\n" , reg);
    }

    ret = IICWriteWord(mt9m111_camera_IIC_ADDR,  reg, input, 0);
	
    if(ret == IIC_OK) {
        if(iwRegN != -1)
        {
            if(gFrameRate == 5) 
                mt9m111_camera_init_5fps[iwRegN + 1] = input;	///////////

            if(gFrameRate == 10) 
                mt9m111_camera_init_10fps[iwRegN + 1] = input;	///////////

            if(gFrameRate == 15) 
                mt9m111_camera_init_15fps[iwRegN + 1] = input;	///////////
   
            if(gFrameRate == 25) 
                mt9m111_camera_init_25fps[iwRegN + 1] = input;	///////////

            if(gFrameRate == 30) 
                mt9m111_camera_init_30fps[iwRegN + 1] = input;	///////////

            if(gFrameRate == 75) 
                mt9m111_camera_init_7_5fps[iwRegN + 1] = input;	///////////
        }

        if(IICReadWord(mt9m111_camera_IIC_ADDR, reg, &val, 0) == IIC_ERROR)
            val = 0x91491455;

        pkt.sub_type = tGetCamReg;
        pkt.length = 8;
        *(int *)(pkt.message) = reg;
        *(int *)(((int *)pkt.message) + 1) = val;
	  
      	 if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN) == SUCCESSFUL) {
            RiscFlagSet();    // Set risc1 flag to interrupt risc1     
        } else {
            printf("\nSetCamSingleRegister: TwoRiscCommSendPacket error!\n");
        }

        debug("\nSetCamSingleRegister: setReg 0x%x = 0x%x  succeed! And read 0x%x!\n", reg, input, val);

        return ret;
		
    } else {    // IIC_ERROR
        debug("\nSetCamSingleRegister: setReg 0x%x = 0x%x  failed!", reg, input);    
        return ret;
    }
#endif    
}

void SetMt9m111Brightness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetMt9m111Brightness: gtCameraSetting.brightness = %d, ", gtCameraSetting.brightness);

    switch(gtCameraSetting.brightness) {

        case 0:     // -5
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_minus5) / sizeof ( mt9m111_camera_init_brightness_minus5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_minus5[i], mt9m111_camera_init_brightness_minus5[i+1], 0);
            break;

        case 1:     // -4
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_minus4) / sizeof ( mt9m111_camera_init_brightness_minus4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_minus4[i], mt9m111_camera_init_brightness_minus4[i+1], 0);
            break;

        case 2:     // -3
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_minus3) / sizeof ( mt9m111_camera_init_brightness_minus3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_minus3[i], mt9m111_camera_init_brightness_minus3[i+1], 0);
            break;
 
        case 3:     // -2
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_minus2) / sizeof ( mt9m111_camera_init_brightness_minus2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_minus2[i], mt9m111_camera_init_brightness_minus2[i+1], 0);
            break;

        case 4:     // -1
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_minus1) / sizeof ( mt9m111_camera_init_brightness_minus1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_minus1[i], mt9m111_camera_init_brightness_minus1[i+1], 0);
            break;

        case 5:     // 0
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_zero) / sizeof ( mt9m111_camera_init_brightness_zero [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_zero[i], mt9m111_camera_init_brightness_zero[i+1], 0);
            break;

        case 6:     // +1
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_plus1) / sizeof ( mt9m111_camera_init_brightness_plus1 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_plus1[i], mt9m111_camera_init_brightness_plus1[i+1], 0);
            break;

        case 7:     // +2
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_plus2) / sizeof ( mt9m111_camera_init_brightness_plus2 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_plus2[i], mt9m111_camera_init_brightness_plus2[i+1], 0);
            break;

        case 8:     // +3
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_plus3) / sizeof ( mt9m111_camera_init_brightness_plus3 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_plus3[i], mt9m111_camera_init_brightness_plus3[i+1], 0);
            break;

        case 9:     // +4
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_plus4) / sizeof ( mt9m111_camera_init_brightness_plus4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_plus4[i], mt9m111_camera_init_brightness_plus4[i+1], 0);
            break;
       
        default:
            reg_volatile_num = sizeof(mt9m111_camera_init_brightness_plus4) / sizeof ( mt9m111_camera_init_brightness_plus4 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_brightness_plus4[i], mt9m111_camera_init_brightness_plus4[i+1], 0);
            break;
    }
}

void SetMt9m111Sharpness(void)
{
    int i;
    int reg_volatile_num;

    printf("SetMt9m111Sharpness: gtCameraSetting.sharpness = %d, ", gtCameraSetting.sharpness);

    switch(gtCameraSetting.sharpness) {
        case 0:     // sharpness no
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_no) / sizeof ( mt9m111_camera_init_sharpness_no[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_no[i], mt9m111_camera_init_sharpness_no[i+1], 0);
            break;

        case 1:     // sharpness25%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_25percent) / sizeof ( mt9m111_camera_init_sharpness_25percent[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_25percent[i], mt9m111_camera_init_sharpness_25percent[i+1], 0);
            break;

        case 2:     // sharpness50%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_50percent) / sizeof ( mt9m111_camera_init_sharpness_50percent[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_50percent[i], mt9m111_camera_init_sharpness_50percent[i+1], 0);
            break;

        case 3:     // sharpness75%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_75percent) / sizeof ( mt9m111_camera_init_sharpness_75percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_75percent[i], mt9m111_camera_init_sharpness_75percent[i+1], 0);
            break;

        case 4:     // sharpness100%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_100percent) / sizeof ( mt9m111_camera_init_sharpness_100percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_100percent[i], mt9m111_camera_init_sharpness_100percent[i+1], 0);
            break;

        case 5:     // sharpness125%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_125percent) / sizeof ( mt9m111_camera_init_sharpness_125percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_125percent[i], mt9m111_camera_init_sharpness_125percent[i+1], 0);
            break;

        case 6:     // sharpness150%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_150percent) / sizeof ( mt9m111_camera_init_sharpness_150percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_150percent[i], mt9m111_camera_init_sharpness_150percent[i+1], 0);
            break;

        case 7:     // sharpness200%
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_200percent) / sizeof ( mt9m111_camera_init_sharpness_200percent[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_200percent[i], mt9m111_camera_init_sharpness_200percent[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(mt9m111_camera_init_sharpness_200percent) / sizeof ( mt9m111_camera_init_sharpness_200percent[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sharpness_200percent[i], mt9m111_camera_init_sharpness_200percent[i+1], 0);
            break;
    }
}

void SetMt9m111Saturation(void)
{
    int i;
    int reg_volatile_num;

    printf("SetMt9m111Saturation: gtCameraSetting.saturation = %d, ", gtCameraSetting.saturation);

    switch(gtCameraSetting.saturation) {
        case 0:     // saturation25%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_25percent) / sizeof ( mt9m111_camera_init_saturation_25percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_25percent[i], mt9m111_camera_init_saturation_25percent[i+1], 0);
            break;

        case 1:     // saturation37.5%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_37_5percent) / sizeof ( mt9m111_camera_init_saturation_37_5percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_37_5percent[i], mt9m111_camera_init_saturation_37_5percent[i+1], 0);
            break;

        case 2:     // saturation50%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_50percent) / sizeof ( mt9m111_camera_init_saturation_50percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_50percent[i], mt9m111_camera_init_saturation_50percent[i+1], 0);
            break;

        case 3:     // saturation75%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_75percent) / sizeof ( mt9m111_camera_init_saturation_75percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_75percent[i], mt9m111_camera_init_saturation_75percent[i+1], 0);
            break;

        case 4:     // saturation150%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_full) / sizeof ( mt9m111_camera_init_saturation_full [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_full[i], mt9m111_camera_init_saturation_full[i+1], 0);
            break;

        case 5:     // saturation150%
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_150percent) / sizeof ( mt9m111_camera_init_saturation_150percent [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_150percent[i], mt9m111_camera_init_saturation_150percent[i+1], 0);
            break;

        case 6:     // saturation black and white
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_bw) / sizeof ( mt9m111_camera_init_saturation_bw [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_bw[i], mt9m111_camera_init_saturation_bw[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(mt9m111_camera_init_saturation_bw) / sizeof ( mt9m111_camera_init_saturation_bw [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_saturation_bw[i], mt9m111_camera_init_saturation_bw[i+1], 0);
            break;
    }
}

void SetMt9m111Contrast(void)
{
    int i;
    int reg_volatile_num;

    printf("SetMt9m111Contrast: gtCameraSetting.contrast = %d, ", gtCameraSetting.contrast);

    switch(gtCameraSetting.contrast) {
        case 0:     // contrast1.0
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_1_0) / sizeof ( mt9m111_camera_init_contrast_1_0[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_1_0[i], mt9m111_camera_init_contrast_1_0[i+1], 0);
            break;

        case 1:     // contrast1.25
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_1_25) / sizeof ( mt9m111_camera_init_contrast_1_25 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_1_25[i], mt9m111_camera_init_contrast_1_25[i+1], 0);
            break;

        case 2:     // contrast1.5
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_1_5) / sizeof ( mt9m111_camera_init_contrast_1_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_1_5[i], mt9m111_camera_init_contrast_1_5[i+1], 0);
            break;

        case 3:     // contrast1.75
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_1_75) / sizeof ( mt9m111_camera_init_contrast_1_75[0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_1_75[i], mt9m111_camera_init_contrast_1_75[i+1], 0);
            break;

        case 4:     // contrast2.0
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_2_0) / sizeof ( mt9m111_camera_init_contrast_2_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_2_0[i], mt9m111_camera_init_contrast_2_0[i+1], 0);
            break;

        case 5:     // contrast2.25
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_2_25) / sizeof ( mt9m111_camera_init_contrast_2_25 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_2_25[i], mt9m111_camera_init_contrast_2_25[i+1], 0);
            break;

        case 6:     // contrast2.50
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_2_50) / sizeof ( mt9m111_camera_init_contrast_2_50 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_2_50[i], mt9m111_camera_init_contrast_2_50[i+1], 0);
            break;

        case 7:     // contrast2.75
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_2_75) / sizeof ( mt9m111_camera_init_contrast_2_75 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_2_75[i], mt9m111_camera_init_contrast_2_75[i+1], 0);
            break;

        case 8:     // contrast3.0
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_3_0) / sizeof ( mt9m111_camera_init_contrast_3_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_3_0[i], mt9m111_camera_init_contrast_3_0[i+1], 0);
            break;

        case 9:     // contrast3.25
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_3_25) / sizeof ( mt9m111_camera_init_contrast_3_25 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_3_25[i], mt9m111_camera_init_contrast_3_25[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(mt9m111_camera_init_contrast_3_25) / sizeof ( mt9m111_camera_init_contrast_3_25 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_contrast_3_25[i], mt9m111_camera_init_contrast_3_25[i+1], 0);        
            break;
    }
}

void SetMt9m111Hue(void)
{
    int i;
    int reg_volatile_num;

    printf("SetMt9m111Hue: gtCameraSetting.hue = %d, ", gtCameraSetting.hue);

    switch(gtCameraSetting.hue) {
        case 0:     // hue0
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_0) / sizeof ( mt9m111_camera_init_hue_0 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_0[i], mt9m111_camera_init_hue_0[i+1], 0);
            break;

        case 1:     // hue5
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_5) / sizeof ( mt9m111_camera_init_hue_5 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_5[i], mt9m111_camera_init_hue_5[i+1], 0);
            break;

        case 2:     // hue10
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_10) / sizeof ( mt9m111_camera_init_hue_10 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_10[i], mt9m111_camera_init_hue_10[i+1], 0);
            break;

        case 3:     // hue15
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_15) / sizeof ( mt9m111_camera_init_hue_15 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_15[i], mt9m111_camera_init_hue_15[i+1], 0);
            break;

        case 4:     // hue20
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_20) / sizeof ( mt9m111_camera_init_hue_20 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_20[i], mt9m111_camera_init_hue_20[i+1], 0);
            break;

        case 5:     // hue25
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_25) / sizeof ( mt9m111_camera_init_hue_25 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_25[i], mt9m111_camera_init_hue_25[i+1], 0);
            break;

        case 6:     // hue30
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_30) / sizeof ( mt9m111_camera_init_hue_30 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_30[i], mt9m111_camera_init_hue_30[i+1], 0);
            break;

        case 7:     // hue35
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_35) / sizeof ( mt9m111_camera_init_hue_35 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_35[i], mt9m111_camera_init_hue_35[i+1], 0);
            break;

        case 8:     // hue40
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_40) / sizeof ( mt9m111_camera_init_hue_40 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_40[i], mt9m111_camera_init_hue_40[i+1], 0);
            break;

        case 9:     // hue45
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_45) / sizeof ( mt9m111_camera_init_hue_45 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_45[i], mt9m111_camera_init_hue_45[i+1], 0);
            break;

        default:
            reg_volatile_num = sizeof(mt9m111_camera_init_hue_45) / sizeof ( mt9m111_camera_init_hue_45 [0] );
            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_hue_45[i], mt9m111_camera_init_hue_45[i+1], 0);           
            break;
    }
}

/*
*********************************************************************************************
*                                  HwMT9M111Init
*
* Description:     This routine read the array of MT9M111_init, then write the value in the array
                                to MT9M111 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
VOID HwMt9m111Init(VOID)
{    
    //int totalN = sizeof(mt9m111_camera_init) / sizeof( mt9m111_camera_init[0]);
    int i;
    //int reg_template_num = sizeof ( mt9m111_camera_init) /sizeof ( mt9m111_camera_init[0] );
    int reg_volatile_num = sizeof ( mt9m111_camera_init_vga_volatile ) / sizeof ( mt9m111_camera_init_vga_volatile [ 0 ] );
    ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
    printf("\r\nmode is%d\r\n", mode );  

    /* reset camera */
    IICWriteWord(mt9m111_camera_IIC_ADDR,0xF0,0x0000,0);
    IICWriteWord(mt9m111_camera_IIC_ADDR,0x0D,0x0021,0);//bit 5 SOC Reset, bit 0 Sensor Reset
    IICWriteWord(mt9m111_camera_IIC_ADDR,0x0D,0x0008,0);

    /* delay */
    for(i = 0; i<= 0xFFFF; i++);    
    
    SetMt9m111FrameRate(gFrameRate);

    /*
    if(gFrameRate == 5)
        for (i=0; i<sizeof(mt9m111_camera_init_5fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_5fps[i], mt9m111_camera_init_5fps[i+1], 0);
    else if(gFrameRate == 10)
        for (i=0; i<sizeof(mt9m111_camera_init_10fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_10fps[i], mt9m111_camera_init_10fps[i+1], 0);
    else if(gFrameRate == 15)
        for (i=0; i<sizeof(mt9m111_camera_init_15fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_15fps[i], mt9m111_camera_init_15fps[i+1], 0);
    else if(gFrameRate == 25)
        for (i=0; i<sizeof(mt9m111_camera_init_25fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_25fps[i], mt9m111_camera_init_25fps[i+1], 0);
    else if(gFrameRate == 30)
        for (i=0; i<sizeof(mt9m111_camera_init_30fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_30fps[i], mt9m111_camera_init_30fps[i+1], 0);
    else
        for (i=0; i<sizeof(mt9m111_camera_init_7_5fps)/sizeof(int); i+=2)
            IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_7_5fps[i], mt9m111_camera_init_7_5fps[i+1], 0);
    */            

    switch ( mode )
    {
        case VGA:
            printf("\r\nHwMt9m111Init: VGA\r\n");
            reg_volatile_num = sizeof( mt9m111_camera_init_vga_volatile ) /sizeof ( mt9m111_camera_init_vga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_vga_volatile[i],mt9m111_camera_init_vga_volatile[i+1], 0);                                  
            break;

        case SXGA:
            printf("\r\nHwMt9m111Init: SXGA\r\n");
            reg_volatile_num = sizeof( mt9m111_camera_init_sxga_volatile ) /sizeof ( mt9m111_camera_init_sxga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_sxga_volatile[i],mt9m111_camera_init_sxga_volatile[i+1], 0);                                  
            break;
        
        
        case QVGA:
            printf("\r\nHwMt9m111Init: QVGA\r\n");
            reg_volatile_num = sizeof( mt9m111_camera_init_qvga_volatile ) /sizeof(mt9m111_camera_init_qvga_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2)
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_qvga_volatile [i], mt9m111_camera_init_qvga_volatile [i+1], 0);
            break;

        case CIF:
            printf("\r\nHwMt9m111Init: CIF\r\n");
            reg_volatile_num = sizeof ( mt9m111_camera_init_cif_volatile )/ sizeof ( mt9m111_camera_init_cif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_cif_volatile[i], mt9m111_camera_init_cif_volatile[i+1], 0);
            break;

        case QCIF:
            printf("\r\nHwMt9m111Init: QCIF\r\n");
            reg_volatile_num = sizeof ( mt9m111_camera_init_qcif_volatile )/ sizeof ( mt9m111_camera_init_qcif_volatile[0]);

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_qcif_volatile[i], mt9m111_camera_init_qcif_volatile[i+1], 0);
            break;
    
        case QQVGA:
            printf("\nHwMt9m111Init: QQVGA\n");
            reg_volatile_num = sizeof ( mt9m111_camera_init_qqvga_volatile ) / sizeof ( mt9m111_camera_init_qqvga_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_qqvga_volatile [i], mt9m111_camera_init_qqvga_volatile [i+1], 0);                
            break; 

        case NTSC_D1:
            printf("r\nHwMt9m111Init: NTSC_D1\r\n");
            reg_volatile_num = sizeof ( mt9m111_camera_init_ntsc_d1_volatile ) / sizeof ( mt9m111_camera_init_ntsc_d1_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_ntsc_d1_volatile [i], mt9m111_camera_init_ntsc_d1_volatile [i+1], 0);                
            break; 
     
        case PAL_D1:
            printf("\nHwMt9m111Init: @PAL_D1\r\n");
            reg_volatile_num = sizeof ( mt9m111_camera_init_pal_d1_volatile ) / sizeof ( mt9m111_camera_init_pal_d1_volatile [0] );

            for ( i = 0; i < reg_volatile_num; i += 2 )
                IICWriteWord(mt9m111_camera_IIC_ADDR, mt9m111_camera_init_pal_d1_volatile [i], mt9m111_camera_init_pal_d1_volatile [i+1], 0);                
            break; 
                       
        default:
            printf("\r\nHwMt9m111Init: default: VGA\r\n");
            reg_volatile_num = sizeof( mt9m111_camera_init_vga_volatile ) /sizeof ( mt9m111_camera_init_vga_volatile[0]);            
            break;
    }

    SetMt9m111Brightness();
    SetMt9m111Sharpness();
    SetMt9m111Saturation();
    SetMt9m111Contrast();
    SetMt9m111Hue();

    SendAllCamRegValToLinux();
}

int GetMt9m111ID(void)
{
    int device_id;
	//as the read function of Mt9m111 is not ok.so return it's real id.
	//return 0x143A;
	 /* reset camera */
        IICWriteWord(mt9m111_camera_IIC_ADDR,0xF0,0x0000,0);
        IICWriteWord(mt9m111_camera_IIC_ADDR,0x0D,0x0021,0);//bit 5 SOC Reset, bit 0 Sensor Reset
        IICWriteWord(mt9m111_camera_IIC_ADDR,0x0D,0x0008,0);
    if(IICReadWord(mt9m111_camera_IIC_ADDR, CHIP_VERSION_ADDR, &device_id, 0)==IIC_ERROR)
        return IIC_ERROR;

    printf("deviceid = 0x%x\n",device_id);
    return device_id;
}

#endif

