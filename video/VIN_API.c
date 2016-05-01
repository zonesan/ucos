/*
*********************************************************************************************************
*                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                      Unpublished & Not for Publication
*                            All Rights Reserved                   
*   
* File        : Video_API.c                   
*                                                                         
* Description : This file contains the application interface of video input
*               output, VSC, I2C interface to camera sensor and ADV7171 
*
* By          : Eric
*             
* Date        : Apr 11, 2006      h                                                     
*********************************************************************************************************
*/



#define VIN_API_GLOBLS

#include"../inc/video_cfg.h"
#include    "risc.h"
#include    "Mc.h"
#include    "Sc.h"
//#include    "ftuart.h"
#include    "comm.h"
#include "../video/VIN_API.h"
#include    "2risc_memory_map.h"
#include    "cctl.h"
#include "sun_type_defs.h"
#include "VIN_SET.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"
#include "two_risc_video.h"
#include "iic.h"

#include "video_intr.h"

#include "analog_camera.h"
#include "chip_cfg.h"
//#include "ov7670.h"

/*
*********************************************************************************************************                 
*   
* File:             I2C.C             
*                                                                         
* DESCRIPTION :     I2C function  
*
*    By:               Eric                                                        
*    
*    Date:                        15 Jan, 2006   
*                                                                       
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                        Register setting for OV7670
*********************************************************************************************************
*/



#ifdef                   GC0306
BYTE camera_sensor_init[]=
{
0x00,0x11,
0x01,0x48,
0x02,0x22,
0x03,0x03,
0x04,0xae,
0x05,0x00,
0x06,0x00,
0x07,0x00,
0x08,0x00,
0x09,0x01,
0x0a,0xe8,
0x0b,0x02,
0x0c,0x88,
0x0d,0x00,
0x0e,0xa0,
0x0f,0x00,
0x10,0x25,
0x11,0x1c,
0x12,0x04,
0x13,0x00,
0x40,0x7c,
0x41,0x9f,
0x42,0x30,
0x43,0x40,
0x44,0xe0,
0x45,0x24,
0x46,0x20,
0x47,0x00,
0x48,0x00,
0x49,0x00,
0x4a,0x00,
0x4b,0x00,
0x4c,0x00,
0x4d,0x00,
0x4e,0x23,
0x4f,0x23,
0x50,0x00,
0x51,0x20,
0x52,0x40,
0x53,0x60,
0x54,0x80,
0x55,0xa0,
0x56,0xc0,
0x57,0xe0,
0x59,0x00,
0x5a,0x00,
0x5b,0x00,
0x5c,0x00,
0x61,0x60,
0x63,0x6a,
0x65,0x78,
0x67,0x60,
0x69,0x40,
0x6a,0xed,
0x6b,0xf4,
0x6c,0x0 ,
0x6d,0x58,
0x6e,0xe7,
0x70,0x00,
0x71,0x10,
0x72,0x00,
0x73,0x14,
0x74,0x00,
0x75,0x10,
0x76,0x28,
0x77,0x3c,
0x78,0x50,
0x80,0x30,
0x81,0x30,
0x82,0x24,
0x83,0x14,
0x84,0x26,
0x85,0x0a,
0x86,0x00,
0x87,0x92,
0x88,0x0a,
0x89,0x04,
0x8a,0x18,
0x8b,0x3f,
0x8c,0x22,
0x8d,0x80,
0x8e,0x00,
0x8f,0x00,
0x90,0x20,
0x91,0x3c,
0x92,0x52,
0x93,0x74,
0x94,0x8e,
0x95,0xa2,
0x96,0xb2,
0x97,0xbf,
0x98,0xd2,
0x99,0xe2,
0x9a,0xee,
0x9b,0xf6,
0x9c,0xfe,
0xa0,0x48,
0xa1,0x50,
0xa2,0x30,
0xa3,0x30,
0xa4,0xc0,
0xa5,0x02,
0xa6,0x60,
0xa7,0x05,
0xa8,0xf4,
0xa9,0x0c,
0xaa,0x00,
0xab,0xf0,
0xac,0xdc,
0xad,0x32,
0xae,0x80,
0xaf,0x88,
0xc0,0x20,
0xc1,0xf0,
0xc2,0x20,
0xc3,0x00,
0xc4,0x01,
0xc5,0x21,
0xc6,0x80,
0xc7,0x00,
0xc8,0x00,
0xc9,0x00,
0xca,0x40,
0xcb,0x40,
0xcc,0x40,
0xcd,0x40,
0xce,0x40,
0xcf,0x40,  
0xd0,0x03,
0xd1,0x80,
0xd2,0x85,
0xd3,0x00,
0xd4,0x70,
0xd5,0xf0,
0xd6,0x80,
0xd7,0x08,
0xd8,0x0a,
0xd9,0x10,
0xda,0x00,
0xdb,0x00,
0xdc,0x00,
0xdd,0x07,
0xe0,0x05,
0xe1,0x05,
0xe2,0x22,
0xe3,0x18,
0xe4,0x14,
0xe5,0x11,
0xe6,0x1a,
0xe7,0x12
};

#endif

#if 0
/*
*********************************************************************************************
*                                  I2C fuction sub fuctions
*
* Description:     
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 

*********************************************************************************************
*/    

VOID I2C_delay(VOID)
{
    SHORT i;
    for(i = 0; i<= I2C_DELAY; i++)
    {
    }
}

VOID SDA_high(VOID)
{
    GPIO26_output();                    
    GPIO26_high();                        
    I2C_delay();
}

VOID SDA_low(VOID)
{
    GPIO26_output();
    GPIO26_low();
    I2C_delay();
}

VOID SCL_high(VOID)
{
    GPIO25_output();
    GPIO25_high();
    I2C_delay();
}

VOID SCL_low(VOID)
{
    GPIO25_output();
    GPIO25_low();
    I2C_delay();
}

VOID clock(VOID)
{
    SCL_high();
    SCL_low();
}

VOID Start_bit(VOID)
{
    SDA_high();
    SCL_high();
    SDA_low();
    SCL_low();
}

VOID Stop_bit(VOID)
{
    SDA_low();
    SCL_high();
    SDA_high();
}

BOOL Check_ack(VOID)
{            
    int i;
#ifdef FPGA
    int ACK_wait_time = 6000;
#else
    int ACK_wait_time = 6000*10;
#endif

    BOOL ack_bit;    
    
    GPIO26_output();    //set SDA as output
    GPIO26_high();      //set SDA high
    GPIO26_input();     //set SDA as input

    for(i = 0; i < ACK_wait_time; i++) {        
        ack_bit = !((rRISC_GPIO & GPIO26_bitmask)>>GPIO26_bit);   //monitor SDA 
        
        if (ack_bit == 1){
            i = ACK_wait_time;
            clock();
            return 1;   //if slave acknowledge, SDA is low, return 1    
        }
    }
        
    if(ack_bit == 0) {
        clock();    //output clock to tell slave received ack, and continue to read data
        return 0;   //slave not ack, return 0
    }                                                                                                                                        
}

VOID Not_ACK(VOID)
{
    SDA_high();
    clock();
}

/*
*********************************************************************************************
*                                  I2C single write
*
* Description:  This routine read the slave address, input register and the value wants to  
*                    write to the register, then out data in I2C format.
*
* Arguments  : slave_addr   is the chip address;
*              reg    is the register address in the chip;       
*              input    is the value to be be writen into the register.
*
* Return     : 1 means success, 0 means fail.
*
* Note(s)    : This function can only perform single write
*               I2C Register Write sequence:         
*               |Start|Slave address write|ACK|Sub address|ACK|Wrtie Data|ACK|Stop|
*********************************************************************************************
*/    
BOOL HwI2cSingleWrite(BYTE slave_add, BYTE reg, BYTE input)
{        
    BYTE temp_mask = 0x80;
    BYTE i;
    
    Start_bit();
    
    for(i=0; i<8; i++) {
        if (slave_add & temp_mask)
            SDA_high();
        else
            SDA_low();
        clock();
        temp_mask = temp_mask >> 1;
    }        
                
    if(Check_ack()) {    
        temp_mask = 0x80;
        for(i =0;i<8;i++) {        
            if (reg & temp_mask)
                SDA_high();
            else
                SDA_low();
            clock();
            temp_mask = temp_mask >> 1;
        }
                    
        if (Check_ack()) {
            temp_mask = 0x80;
            for (i =0;i<8;i++) {
                if(input & temp_mask)
                    SDA_high();
                else
                    SDA_low();
                clock();
                temp_mask = temp_mask >> 1;
            }
        } else {
            Stop_bit();
            return FAILED;
        }

        if (Check_ack()) {
            Stop_bit();
            return SUCCESSFUL;
        } else {
            Stop_bit();
            return FAILED;
        }
    } else {
        Stop_bit();
        return FAILED;
    }
}

/*
*************************************************************************************************
*                                  I2C single read
*
* Description:     This routine read the slave address, input register then return the data of 
                                the register.
*
* Arguments  : slave_addr   is the chip address;
*              read_reg    is the register address in the chip;       
*              status    will save the status of the read operation, FAIL or SUCCESSFUL.
*
* Note(s)    : This function can only perform single read
*               I2C Register Read sequence:     
*               |Start|Slave address write|ACK|Sub address|ACK|STOP|Start|Slave address read|ACK|read Data|ACK|Stop|
*************************************************************************************************
*/
BYTE HwI2cSingleRead(BYTE slave_add, BYTE read_reg, BYTE *status)
{
    BYTE temp_mask = 0x80;
    BYTE read_data = 0x00;
    BYTE i, j;
    BYTE slave_add_wr = slave_add;
    BYTE slave_add_rd = slave_add + 0x01;;
        
    Start_bit();
    for(i=0; i<8; i++) {    //write slave addres bit7=0
        if (slave_add_wr & temp_mask)
            SDA_high();
        else
            SDA_low();
        clock();
        temp_mask = temp_mask >> 1;
    }
                
    if(1/*Check_ack()*/) {    //wait for ACK        
        temp_mask = 0x80;
        for(i =0;i<8;i++) {   //write sub_address           
            if (read_reg & temp_mask)
                SDA_high();
            else
                SDA_low();
            clock();
            temp_mask = temp_mask >> 1;
        }
                    
        if(1/*Check_ack()*/) {     //wait for ACK
            temp_mask = 0x80;
            //Stop_bit();       //Stop
            Start_bit();      //Repeat Start
            for(i=0; i<8; i++) {    //write slave address bit7=1
                if(slave_add_rd & temp_mask)
                    SDA_high();
                else
                    SDA_low();
                clock();
                temp_mask = temp_mask >> 1;
            }
                    
            if(1/*Check_ack()*/) {
                temp_mask = 0x80;
                GPIO26_output();    //set SDA as output
                GPIO26_high();      //set SDA high
                GPIO26_input();     //set SDA as input
                        
                for(i=0; i<8; i++) {    //Start to read data                         
                    I2C_delay();    
                                    
                    if((rRISC_GPIO & GPIO26_bitmask)>>GPIO26_bit) {    //read GPIO status
                        read_data = read_data | temp_mask;    //if SDA is high, record this bit to read_data
                        clock();
                    } else    //if SDA is low, leave this bit remain low, go to next bit
                        clock();
                            
                    temp_mask = temp_mask >> 1;
                }
                Not_ACK();        
                Stop_bit();
                *status = SUCCESSFUL;
                return read_data;
            } else {
                clock();
                Stop_bit();
            }        
        } else {
            clock();
            Stop_bit();
        }    
    } else {
        clock();
        Stop_bit();
    }
    
    *status = FAILED;
    return 0;                        
}            
#endif

#if 0
/*
*********************************************************************************************
*                                  Camera initialize
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
VOID HwCameraInitialize(VOID)
{
    //BYTE i;    
    int totalN = sizeof(camera_sensor_init) / sizeof( camera_sensor_init[0]);
    int i;
#ifdef OV7670    
    int reg_template_num = sizeof ( camera_sensor_init) /sizeof ( camera_sensor_init[0] );
    int reg_volatile_num = sizeof ( camera_sensor_init_vga_volatile ) / sizeof ( camera_sensor_init_vga_volatile [ 0 ] );
    ImageMode mode = GetImageMode(gImageWidth, gImageHeight);
   printf("\ngImageWidth = %d, gImageHeight = %d", gImageWidth, gImageHeight);
    printf("\r\nmode is%d\r\n", mode );
#endif    

    /* reset camera */
    IICWriteByte(CAMERA_SENSOR_IIC_ADDR, 0x12, 0x80, 0);
//  HwI2cSingleWrite(CAMERA_SENSOR_ADDRESS, 0x12, 0x80);
//  HwI2cSingleWrite(CAMERA_SENSOR_ADDRESS, camera_sensor_init[0], camera_sensor_init[1]);

    /* delay */
    for(i = 0; i<= 0xFFFF; i++);    
    
    printf("totalN=%d",totalN);
#ifdef OV7670    
    for (i=0; i<reg_template_num; i+=2)
#endif        

#ifdef OV7650    
    for (i=0; i<CAMERA_SENSOR_REG_SIZE; i+=2)
#endif        


    {
        IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init[i], camera_sensor_init[i+1], 0);      
//      HwI2cSingleWrite(CAMERA_SENSOR_ADDRESS, camera_sensor_init[i], camera_sensor_init[i+1]);      
    }

#ifdef OV7670   
    switch ( mode )
        {
            case VGA:
                printf("\r\nVGA\r\n");
                reg_volatile_num = sizeof( camera_sensor_init_vga_volatile ) /sizeof ( camera_sensor_init_vga_volatile[0]);

                for ( i = 0; i < reg_volatile_num; i += 2)
                    //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_vga_volatile[i],camera_sensor_init_vga_volatile[i+1] );
                    IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_vga_volatile[i],camera_sensor_init_vga_volatile[i+1], 0);                  
                
                break;
            case QVGA:
                printf("\r\nQVGA\r\n");
                reg_volatile_num = sizeof( camera_sensor_init_qvga_volatile ) /sizeof(camera_sensor_init_qvga_volatile[0]);

                for ( i = 0; i < reg_volatile_num; i += 2)
                    //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_qvga_volatile [i], camera_sensor_init_qvga_volatile [i+1]);
                    IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_qvga_volatile [i], camera_sensor_init_qvga_volatile [i+1], 0);
                break;

            case CIF:
                printf("\r\nCIF\r\n");
                reg_volatile_num = sizeof ( camera_sensor_init_cif_volatile )/ sizeof ( camera_sensor_init_cif_volatile[0]);

                for ( i = 0; i < reg_volatile_num; i += 2 )
                    //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_cif_volatile[i], camera_sensor_init_cif_volatile[i+1]);
                    IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_cif_volatile[i], camera_sensor_init_cif_volatile[i+1], 0);

                break;
            case QCIF:
                printf("\r\nQCIF\r\n");
                reg_volatile_num = sizeof ( camera_sensor_init_qcif_volatile ) / sizeof ( camera_sensor_init_qcif_volatile [0] );

                for ( i = 0; i < reg_volatile_num; i += 2 )
                    //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_qcif_volatile [i], camera_sensor_init_qcif_volatile [i+1] );
                    IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_qcif_volatile [i], camera_sensor_init_qcif_volatile [i+1], 0);
                
                break;
            default:
                break;
        }

        reg_volatile_num = sizeof ( camera_sensor_init_frame_rate ) / sizeof ( camera_sensor_init_frame_rate [0] );
        for ( i = 0; i < reg_volatile_num; i += 2 )
            //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_frame_rate [i], camera_sensor_init_frame_rate [i+1] );
            IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_frame_rate [i], camera_sensor_init_frame_rate [i+1], 0);
#endif        
/*
    for ( i = 0; i < reg_volatile_num; i += 2)
        {
            //HwI2cSingleWrite ( CAMERA_SENSOR_ADDRESS, camera_sensor_init_vga_volatile[i], camera_sensor_init_vga_volatile [i+1]);
            IICWriteByte(CAMERA_SENSOR_IIC_ADDR, camera_sensor_init_vga_volatile[i], camera_sensor_init_vga_volatile [i+1], 0);
        }    
    //SendAllCamRegValToLinux();
    printf("SendAllCamRegValToLinux\n");
*/
    SendAllCamRegValToLinux();

}
#endif

/*
*********************************************************************************************
*                                       VideoRegInit
*
* Description: This routine inits the video registers.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
extern int gCameraSelectDigital;
extern unsigned int gVinOddEvenFrameSelect;
extern pCamera pcurrent_camera;
VOID Video_VIN_RegInit(VOID)
{
    unsigned int s_ratio;

    if(gCameraSelectDigital == TRUE)
        s_ratio = 0;
    else {
        s_ratio = AnalogCameraRatio;
        
        if(s_ratio > 2) {   // should not happen
            printf("\nVideo_VIN_RegInit: s_ratio exceeds 2, forced to zero!\n");
            s_ratio = 0;
        }        
        
        if(s_ratio == 0) // no scale
            gVinOddEvenFrameSelect = ODD_FRAME_FIRST;
        else            
            gVinOddEvenFrameSelect = EVEN_FRAME_FIRST;            
    }                    

    if (pcurrent_camera != NULL) {
        if (1 == pcurrent_camera->clk_polority) {//sample when negedge
            if(gtVinFormat.interlaced)
                if(gImage_Mode == EVENODD_IMAGE)
                    VIN_CTL = BIT_VIN_CLK_POLORITY | BIT_VIN_FIRST_FIELD_ID | (1<<27) | (1<<26) \
                                | (s_ratio << 23) | BIT_VIN_CLK_DIR; 
                else
                    VIN_CTL = BIT_VIN_CLK_POLORITY | BIT_VIN_FIRST_FIELD_ID | (0<<27) | (1<<26) \
                                | (s_ratio << 23) | BIT_VIN_CLK_DIR;
            else
                VIN_CTL = BIT_VIN_CLK_POLORITY | BIT_VIN_FRAME_PIC | BIT_VIN_FIRST_FIELD_ID | BIT_VIN_FMT | BIT_VIN_F0_DROP_LINE \
                                | BIT_VIN_F1_DROP_LINE | (s_ratio << 23) | BIT_VIN_CLK_DIR;      
        } else if (0 == pcurrent_camera->clk_polority) {//sample when posedge
            if(gtVinFormat.interlaced)
                if(gImage_Mode == EVENODD_IMAGE)
                    VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FIRST_FIELD_ID | (1<<27) | (1<<26) \
                                | (s_ratio << 23) | BIT_VIN_CLK_DIR; 
                else
                    VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FIRST_FIELD_ID | (0<<27) | (1<<26) \
                                | (s_ratio << 23) | BIT_VIN_CLK_DIR;
            else
                VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FRAME_PIC | BIT_VIN_FIRST_FIELD_ID | BIT_VIN_FMT | BIT_VIN_F0_DROP_LINE \
                                | BIT_VIN_F1_DROP_LINE | (s_ratio << 23) | BIT_VIN_CLK_DIR;      
        }
    } else {
        if(gtVinFormat.interlaced)
            if(gImage_Mode == EVENODD_IMAGE)
                VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FIRST_FIELD_ID | (1<<27) | (1<<26) \
                            | (s_ratio << 23) | BIT_VIN_CLK_DIR; 
#ifndef DVR        
            else
                VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FIRST_FIELD_ID | (0<<27) | (1<<26) \
                            | (s_ratio << 23) | BIT_VIN_CLK_DIR;
#else        
            else
            {
        //========by vincent 2008.01.22=========
            //for mpp input need to seperate two channel by clk polority
               int chipID,ClkPolority;
               chipID = (*(int *)(0xa8010010 ) >>18) & 0x3;
                if((chipID%2)==0)ClkPolority=0;
                else ClkPolority=1;
                
                VIN_CTL = (ClkPolority<<30)/*BIT_VIN_CLK_POLORITY */| BIT_VIN_FIRST_FIELD_ID | (0<<27) | (1<<26) \
                            | (s_ratio << 23) | BIT_VIN_CLK_DIR;
            }
#endif      

        else
            VIN_CTL = BIT_VIN_CLK_POLORITY_POS | BIT_VIN_FRAME_PIC | BIT_VIN_FIRST_FIELD_ID | BIT_VIN_FMT | BIT_VIN_F0_DROP_LINE \
                            | BIT_VIN_F1_DROP_LINE | (s_ratio << 23) | BIT_VIN_CLK_DIR;              
    }

    switch(s_ratio) {
        case 0:     // no scale
            VIN_FM_SIZE = (gImageWidth/2 - 1<<20) | ((gImageHeight >> (gtVinFormat.interlaced == TRUE)) - 1<<8); 
            break;

        case 1:     // scale down 2
            VIN_FM_SIZE = (gImageWidth - 1<<20) | ((gImageHeight*2 >> (gtVinFormat.interlaced == TRUE)) - 1<<8); 
            break;

        case 2:     // scale down 4
            VIN_FM_SIZE = (gImageWidth*2 - 1<<20) | ((gImageHeight*4 >> (gtVinFormat.interlaced == TRUE)) - 1<<8);   
            break;

        default:    // should not happen            
            VIN_FM_SIZE = (gImageWidth/2 - 1<<20) | ((gImageHeight >> (gtVinFormat.interlaced == TRUE)) - 1<<8); 
            break;            
    }    
    
    CctlVinClkSet();
}

/*
*******************************************************************************************************************                 
*   
*                                                                    Initialize VIN, VOUT, VSC, RVO, PIP, OSD       
*                                                                         
* DESCRIPTION :     This function ouput one frame through VOUT from XXX_memory_1 defined in Video_API.h. After 
*                                    enable VOUT, video output is pass to interrupt rountine.
*
*    By:               Eric                                                        
*    
*    Date:                        11 Apr, 2006   
*                                                                       
*******************************************************************************************************************
*/

extern BOOL VinBufferNumber;

VOID AppVinInitialize(VOID)         
{
    int i;

    for(i=0; i<MAX_VIN_BUF_NUM; i++) {
        gtVideoInFrame[i].y_addr = VIN_YUV_START_ADDR + i*VIN_YUV_SIZE;
        gtVideoInFrame[i].u_addr = gtVideoInFrame[i].y_addr + MAX_VIN_Y_SIZE;
        gtVideoInFrame[i].v_addr = gtVideoInFrame[i].u_addr + MAX_VIN_UV_SIZE;
    }
    VinBufferNumber = 0;
    
    if (IsVinEnabled())
        DIS_VIN_VIDEO();

    /* enable the VIN block */
    CctlVinBlockEn();

    // Added by hjm
    // cctl reset vin, and release reset vin
    CctlVinReset();
    
    INTR_VIN_DIS(); 
    Video_VIN_RegInit();

    ScInit(VIN_Y_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_16Byte, sSwap_no, sLast_Disable, VIN_Y_S_ADDR);
    ScInit(VIN_U_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_256Byte, sSBSize_4Byte, sMBSize_8Byte, sSwap_no, sLast_Disable, VIN_U_S_ADDR);
    ScInit(VIN_V_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_256Byte, sSBSize_4Byte, sMBSize_8Byte, sSwap_no, sLast_Disable, VIN_V_S_ADDR);

/*
    ScInit(VIN_Y_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VIN_Y_S_ADDR);
    ScInit(VIN_U_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VIN_U_S_ADDR);
    ScInit(VIN_V_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VIN_V_S_ADDR);
*/

    if(gtVinFormat.interlaced == TRUE) {
#if 0
#ifdef EVEN_ODD
        McVideoExtendedInit(0, gImageWidth, gImageHeight >> 1, 0, 0, 0, 0);
#else
        McVideoExtendedInit(0, gImageWidth, gImageHeight >> 1, 1, 0, 0, 0);
#endif
#endif
        if(gImage_Mode == EVENODD_IMAGE)
            McVideoExtendedInit(0, gImageWidth, gImageHeight >> 1, 0, 0, 0, 0);
        else
            McVideoExtendedInit(0, gImageWidth, gImageHeight >> 1, 1, 0, 0, 0);
    } else {
        McVideoExtendedInit(0, gImageWidth, gImageHeight, 0, 0, 0, 0);
    }

    printf("\nAppVinInitialize: gImageWidth = %d,  gImageHeight = %d!\n", gImageWidth, gImageHeight);
    McScMap(VIN_Y_M, VIN_Y_S);
    McScMap(VIN_U_M, VIN_U_S);
    McScMap(VIN_V_M, VIN_V_S);

    /* according to the interlace/odd,even frame order to decide */
    if(!gtVinFormat.interlaced) {    //vin yuv format progressive
        McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);
        McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);
        McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);
    } else {        //vin yuv format interlaced
        if(gVinOddEvenFrameSelect == ODD_FRAME_FIRST) {
            McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);
            McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);
            McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);
        } else {
            if(gImage_Mode == EVENODD_IMAGE) {
                McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth * gImageHeight / 2);    // Re-initialize VIN memory to buffer 0
                McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0
                McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr + gImageWidth * gImageHeight / 8);    // Re-initialize VIN memory to buffer 0
            } else {
                McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr + gImageWidth);    // Re-initialize VIN memory to buffer 0
                McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);    // Re-initialize VIN memory to buffer 0        
                McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr);    // Re-initialize VIN memory to buffer 0
            }
        }
    }

/*    
    McVideoInit(VIN_Y_M, 0, 0, gtVideoInFrame[VinBufferNumber].y_addr);
    McVideoInit(VIN_U_M, 0, 0, gtVideoInFrame[VinBufferNumber].u_addr);
    McVideoInit(VIN_V_M, 0, 0, gtVideoInFrame[VinBufferNumber].v_addr); 
*/    

    VinIntrEn();
}

/*
*********************************************************************************************
*                           Vin, Vout, VSC start
*
* Description:         This fuctions Start VIN, VOUT, VSC
*
* Arguments  :                 
                                
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/ 

VOID AppVinStart(VOID)
{
    ScOpen(VIN_Y_S);
    ScOpen(VIN_U_S);
    ScOpen(VIN_V_S);            
  EN_VIN_VIDEO();
}

/*
*********************************************************************************************
*                           Vin, Vout, VSC stop
*
* Description:         This fuction Stop VIN, VOUT, VSC
*
* Arguments  :                 
                                
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/ 

VOID AppVinStop(VOID)
{
    DIS_VIN_VIDEO();
    ScClose(VIN_Y_S);
    ScClose(VIN_U_S);
    ScClose(VIN_V_S);            
}

/*
*********************************************************************************************
*                       Vin, Vout, VSC, RVO, PIP Size update
*
* Description:         This fuction updates the size of VIN, VOUT, VSC, RVO, PIP
*
* Arguments  :                 
                                
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/   
VOID AppVinSizeUpdate(WORD VinSizeWidth, WORD VinSizeHeight, LONG skip, LONG scaling) 
{        
    VIN_CTL |= BIT_VIN_S_RATIO;
    gImageWidth = VinSizeWidth;
    gImageHeight = VinSizeHeight;
    VIN_FM_SIZE = BIT_VIN_WIDTH | BIT_VIN_HEIGHT; 
    McVideoExtendedInit(0, gImageWidth, gImageHeight, skip, skip, scaling, scaling);
}


void  GetCamera_Sensor_Init(BYTE **cameraInit, int *N)
{
return;
#if 0 //stone debug 
#if (defined(OV7670) || defined(OV7650))
    *cameraInit = camera_sensor_init;
    *N = sizeof(camera_sensor_init) / sizeof( camera_sensor_init[0]);
#endif

#if defined(MT9M111)
    if(gFrameRate == 15) {
        *cameraInit = camera_sensor_init_15fps;
        *N = sizeof(camera_sensor_init_15fps) / sizeof( camera_sensor_init_15fps[0]);
    } else {    // 30 framerate
        *cameraInit = camera_sensor_init_30fps;
        *N = sizeof(camera_sensor_init_30fps) / sizeof( camera_sensor_init_30fps[0]);
    }        
#endif
#endif
}


int IsVinEnabled()
{
    return (VIN_CTL & BIT_VIN_ENA) ? 1 : 0;
}


