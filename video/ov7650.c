#include "video_cfg.h"
#if INCLUDE_OV7650
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

#include "mpeg4_encoder.h"

#include "ov7650.h"

extern tMPEG4_ENCODER_PARMS mp4EncodeParams;
/*
*********************************************************************************************
*                                  HwOV7650Init
*
* Description:     This routine read the array of OV7650_init, then write the value in the array
                                to OV7650 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/

//0x11,0x92,0x93

const BYTE gMode_OV7650_FrameRate[4][6][3]=
{//5fps is wrong
//        			5fps				10fps			15fps				20fps			25fps			30fps
/*QVGA*/0x89, 0xb1, 0x40,  0x82, 0xa0, 0x90,      0x82, 0x80, 0x00,     0x81, 0xa0,0x90,    0x81, 0x80,0xa0,    0x81, 0x80,0x00,
/*VGA*/  0x85, 0x80, 0x00,    0x81, 0xa0, 0x90,     0x81, 0x80, 0x00,     0x80, 0xa0,0x90,    0x80, 0x80,0xa0,    0x80, 0x80,0x00,
/*QCIF*/ 0x00, 0x00, 0x00,   0x00, 0x00, 0x00,     0x00, 0x00, 0x00,     0x00, 0x00, 0x00,   0x00, 0x00,0x00,    0x00, 0x00, 0x00,//can't match now
/*CIF*/   0x89, 0x91, 0xd0,   0x85, 0x91, 0x00,     0x83, 0x91, 0x00,     0x82, 0x91,0x00,    0x81, 0x91,0xd0,    0x80, 0x91,0x00
};

void SetOV7650FrameRate(int frame_rate)
{

	ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
	BYTE *p=NULL;
	int n=0;
	int iwCaseN = 0;//case calculate
	int totolN =0;
	p = OV7650_camera_init;
	totolN = sizeof(OV7650_camera_init) / sizeof( OV7650_camera_init[0]);

	int iwReg0x11N = 0;
	int iwReg0x2aN = 0;
	int iwReg0x2bN = 0;
	int iwRegN=3;//number of register 
	gFrameRate = frame_rate;
	
	while(p < &OV7650_camera_init[totolN])
	{
		switch(*p)
		{
		case 0x11:
			iwReg0x11N =n;
			iwCaseN += 1;
			break;
		case 0x2a:
			iwReg0x2aN  = n;
			iwCaseN += 1;
			break;
		case 0x2b:
			iwReg0x2bN  = n;
			iwCaseN += 1;
			break;
		default:
			break;
		}
		p+=2;
		n+=2;
	}

		// Did not find register.
	if(iwCaseN != iwRegN){
		/*
		printf("register error , did not find , in SetCameraImgMode()\n");
		printf("totolN=%d\n",totolN);	
		printf("n=%d\n",n);	
		printf("iwCaseN=%d\n",iwCaseN);
		printf("iwReg0x11N=%d\n",iwReg0x11N);
		printf("iwReg0x2aN=%d\n",iwReg0x14N);
		printf("iwReg0x2bN=%d\n",iwReg0x26N);
		*/
		//return -1;
	}
	debug("before set framRate\n==================================\n");
	debug("OV7650_camera_init[iwReg0x11N + 1]=%d\n",OV7650_camera_init[iwReg0x11N + 1]);
	debug("OV7650_camera_init[iwReg0x2aN + 1]=%d\n",OV7650_camera_init[iwReg0x2aN + 1]);
	debug("OV7650_camera_init[iwReg0x2bN + 1]=%d\n",OV7650_camera_init[iwReg0x2bN + 1]);

	// Finded register.	
	//set register
	switch(mode)
	{
	case QVGA:
		debug("QVGA\n");
		OV7650_camera_init[iwReg0x11N + 1]=gMode_OV7650_FrameRate[0][frame_rate/5-1][0];
		OV7650_camera_init[iwReg0x2aN + 1]=gMode_OV7650_FrameRate[0][frame_rate/5-1][1];
		OV7650_camera_init[iwReg0x2bN + 1]=gMode_OV7650_FrameRate[0][frame_rate/5-1][2];
		break;
	case VGA:
		debug("VGA\n");
		OV7650_camera_init[iwReg0x11N + 1]=gMode_OV7650_FrameRate[1][frame_rate/5-1][0];
		OV7650_camera_init[iwReg0x2aN + 1]=gMode_OV7650_FrameRate[1][frame_rate/5-1][1];
		OV7650_camera_init[iwReg0x2bN + 1]=gMode_OV7650_FrameRate[1][frame_rate/5-1][2];
		break;
	case QCIF:
		debug("QCIF\n");
		OV7650_camera_init[iwReg0x11N + 1]=gMode_OV7650_FrameRate[2][frame_rate/5-1][0];
		OV7650_camera_init[iwReg0x2aN + 1]=gMode_OV7650_FrameRate[2][frame_rate/5-1][1];
		OV7650_camera_init[iwReg0x2bN + 1]=gMode_OV7650_FrameRate[2][frame_rate/5-1][2];
		break;
	case CIF:
		debug("CIF\n");
		OV7650_camera_init[iwReg0x11N + 1]=gMode_OV7650_FrameRate[3][frame_rate/5-1][0];
		OV7650_camera_init[iwReg0x2aN + 1]=gMode_OV7650_FrameRate[3][frame_rate/5-1][1];
		OV7650_camera_init[iwReg0x2bN + 1]=gMode_OV7650_FrameRate[3][frame_rate/5-1][2];
		break;
	default:
		//debug("SetFrameRate error\n");
		break;
	}

	//set mpeg4
	mp4EncodeParams.frame_rate = frame_rate;
	debug("after set framRate\n==================================\n");
	debug("OV7650_camera_init[iwReg0x11N + 1]=%d\n",OV7650_camera_init[iwReg0x11N + 1]);
	debug("OV7650_camera_init[iwReg0x2aN + 1]=%d\n",OV7650_camera_init[iwReg0x2aN + 1]);
	debug("OV7650_camera_init[iwReg0x2bN + 1]=%d\n",OV7650_camera_init[iwReg0x2bN + 1]);
	//mpeg4EncoderSetCtrl(&mp4EncodeParams);

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
BOOL SetOV7650SingleRegister( BYTE reg, BYTE input)
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
   // GetOV7650_camera_Init(&p, &totolN);
    p = OV7650_camera_init;
    totolN = sizeof(OV7650_camera_init) / sizeof( OV7650_camera_init[0]);

    int iwRegN = -1;

    while(p < &OV7650_camera_init[totolN])
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

    ret = IICWriteByte(OV7650_CAMERA_IIC_ADDR,  reg, input, 0);
	
    if(ret == IIC_OK) {
        if(iwRegN != -1)
        {	
            OV7650_camera_init[iwRegN + 1] = input;	///////////
        }

        IICReadByte(OV7650_CAMERA_IIC_ADDR, reg, &val, 0);

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
*                                  HwOV7650Init
*
* Description:     This routine read the array of OV7650_init, then write the value in the array
                                to OV7650 sensor through I2C.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : none.
*********************************************************************************************
*/
VOID HwOV7650Init(VOID)
{ 
    int totalN = sizeof(OV7650_camera_init) / sizeof( OV7650_camera_init[0]);
    int i;

    /* reset camera */
    IICWriteByte(OV7650_CAMERA_IIC_ADDR, 0x12, 0x80, 0);

    /* delay */
    for(i = 0; i<= 0xFFFF; i++);    
    
    printf("totalN=%d",totalN);
 
    for (i=0; i<OV7650_CAMERA_REG_SIZE; i+=2)
    {
        IICWriteByte(OV7650_CAMERA_IIC_ADDR, OV7650_camera_init[i], OV7650_camera_init[i+1], 0);      
    }

    SendAllCamRegValToLinux();

}


int GetOV7650ID(void)
{
	int device_id,data,status;

	if(IICReadByte(OV7650_CAMERA_IIC_ADDR, REG_PID, &data, 0) < 0)
		return IIC_ERROR;
	device_id = data << 8;

	if(IICReadByte(OV7650_CAMERA_IIC_ADDR, REG_VER, &data, 0) < 0)
		return IIC_ERROR;
	device_id |= (data & 0xff);

	printf("deviceid = 0x%x\n",device_id);
	return device_id;
}

#endif

