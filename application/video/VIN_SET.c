
#include "video_cfg.h"
#include "VIN_SET.h"
#include "sun_type_defs.h"
#include "VIN_API.h"
#include "mpeg4_encoder.h"
#include "comm.h"
#include "IP_CAM_API.h"
#include "2risc_comm_protocol.h"
#include "video_api.h"
#include "iic.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#include "../two_risc_comm.h"
#include "Camera.h"
#include "two_risc_video.h"
extern pCamera pcurrent_camera;
extern unsigned char camera_sensor_init[];
extern const BYTE gMode_FrameRate[4][6][3];
  
unsigned int gImageWidth = 320;//QVGAWIDTH;//IMAGE_WIDTH;
unsigned int gImageHeight = 240;//QVGAWIDTH;//IMAGE_HEIGHT;
int gFrameRate = FRAME_RATE;
int gIntraFrameRate = INTRA_FRAME_RATE;
int gVideoQuality = LOW_QUANT;


/*******************************
 *fucntion SetQVGA
 *return val 
 *      -1 error,can not find register
 *       0  success
 *******************************/
int SetQVGA()
{

    return 0;
}

/*******************************
 *fucntion SetVGA
 *return val 
 *      -1 error,can not find register
 *       0  success
 *******************************/
int SetVGA()
{

    return 0;
}


/*******************************
 *fucntion SetCIF
 *return val 
 *      -1 error,can not find register
 *       0  success
 *******************************/
int SetCIF()
{

    return 0;
}



/*******************************
 *fucntion SetQCIF
 *return val 
 *      -1 error,can not find register
 *       0  success
 *******************************/
int SetQCIF()
{
    return 0;
}

int SetQQVGA()
{
#if 0
#endif
    return 0;
}
  
int SetCameraImgMode(ImageMode mode)
{
  
    switch (mode) {
        case SXGA:  
        case PAL_D1:
        case NTSC_D1:            
        case VGA:
        //camera_sensor_init[VGAN + 1] = CAM_VGA;
        //camera_sensor_init[CIFN + 1] = 0x00;
            if(SetVGA()<0)
            return -1;
            break;
        case QVGA:
            if(SetQVGA()<0)
                return -1;
            break;
        case QQVGA:
            if(SetQQVGA()<0)
                return -1;
            break;
        case CIF:
            if(SetCIF()<0)
                return -1;
            break;
        case QCIF:
            if(SetQCIF()<0)
                return -1;
            //camera_sensor_init[CIFN + 1] = CAM_QCIF;
            break;
        default:
            //  printf("error, SetCameraImgMode() param mode %d is not supported.\n", (int)mode);
      return -1;
    }
  

    return 0;
}

int SetCameraImgSize(uint16_t width, uint16_t height)
{


    ImageMode mode=GetImageMode(width,height);
    if(mode==-1)
        return -1;
    return 0;
}

ImageMode GetImageMode(uint16_t width, uint16_t height)
{
    ImageMode mode;
    mode = -1;  

    /* judge the width & height exceeds the MAXIUM? */
    if(width > MAX_VIN_WIDTH) {
        width = MAX_VIN_WIDTH;
        printf("\nGetImageMode: width exceeds MAX_VIN_WIDTH! gImageWidth = MAX_VIN_WIDTH!\n");
    }

    if(height > MAX_VIN_HEIGHT) {
        height = MAX_VIN_HEIGHT;
        printf("\nGetImageMode: height exceeds MAX_VIN_HEIGHT! gImageHeight = MAX_VIN_HEIGHT!\n");
    }
        
    if(gEncodeMode == MPEG_ENCODE) {    //mpeg encode mode
        if (width == 640 && height == 480)
            mode = VGA;
        else if (width == 320 && height == 240)
            mode = QVGA;
        else if (width == 160 && height == 120)
            mode = QQVGA;
        else if (width == 176 && height == 144)
            mode = QCIF;
        else if (width == 352 && height == 288)
            mode = CIF;
            //else if (width == 704 && height == gtVinFormat.height)
        else if (width == 704 && height == 480)
            mode = NTSC_D1;
        else if (width == 704 && height == 576)
            mode = PAL_D1;
        else if (width == 1280 && height == 1024)            
            mode = SXGA;             
        else if (width == 352 && height == 240)            
            mode = NTSC_CIF;             
        else if (width == 176 && height == 120)            
            mode = NTSC_QCIF;       
        else if (width == 128 && height == 96)
            mode = SUB_QCIF;
        } else if(gEncodeMode == H263_ENCODE) {   //H263 encode mode  just support QCIF,CIF,PAL_D1
            if (width == 176 && height == 144)
                mode = QCIF;
            else if (width == 128 && height == 96)
                mode = SUB_QCIF;
            else if (width == 704 && height == 576)
                mode = PAL_D1;
            else {
                mode = CIF;
                width = 352;
                height = 288;
            }    
        } else
            printf("VIN_SET.c:GetImageMode:encode mode error\n");

    if(mode != -1) {
        gImageWidth = width;
        gImageHeight = height;
    }

    return mode;

}

void SendAllCamRegValToLinux()
{
    int i=0;
    int val=-1;
    unsigned char status = FAILED;
    
    Risc_Packet_t pkt = {0};
        pkt.start = RISC_PKT_START;
        pkt.end = RISC_PKT_END;
    pkt.main_type = tVideo;

    for(i=0; i<=0xc9; i++) {
        //val =(int)HwI2cSingleRead(CAMERA_SENSOR_ADDRESS, (BYTE)i, &status);
    if(pcurrent_camera->reg_width == 1) //stone debug
        IICReadByte(pcurrent_camera->iic_addr, i, &val, 0);
    else if(pcurrent_camera->reg_width == 2)
        if(IICReadWord(pcurrent_camera->iic_addr, i, &val, 0) != IIC_OK)
            continue;
    else
        val = 0x91491455;

        pkt.sub_type = tGetCamReg;
        pkt.length = 8;
        *(int *)(pkt.message) = i;
        *(int *)(((int *)pkt.message) + 1) = val;
    if(TwoRiscCommSendPacket((size_t *)&pkt, RPACKET_LEN)==SUCCESSFUL){
        RiscFlagSet();    // Set risc1 flag to interrupt risc1     
    }else{
        printf("TwoRiscCommSendPacket error \n");
    }
        

        //printf("%x=%x\n",i,val);

    }
    
}
