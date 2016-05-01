#include "video_cfg.h"
#ifdef DISPLAY_INCLUDE

#define VSC_API_GLOBALES

#include "VSC_API.h"
#include "VIN_API.h"
#include "VOUT_API.h"
#include "../port/os_cpu.h"
#include "../uCOS Configure/os_cfg.h"
#include "../uCOS2.52/uCOS_II.h"

#include "../mpeg/mpeg4_decoder.h"
#include "../mpeg/mpeg4_encoder.h"
#include "two_risc_display.h"

#include "../application/video/startup_picture.h"

#include "video_cfg.h"
#include "Vout.h"
#include "comm.h"
#include "VIN_SET.h"
#include "two_risc_video.h"
#include "../application/video/vb2100osd.h"
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
 extern unsigned int gVscOddEvenFrameSelect;  

VOID Video_VDE_RegInit(VOID)
{
    unsigned int width, height;
    unsigned int pixel_start, pixel_stop, line_start, line_stop;
    unsigned int v0_s_ratio = 0, v1_s_ratio = 0;
    unsigned int pip_display_pixel, pip_display_line;
    unsigned int rvo_display_pixel, rvo_display_line;
    
    if(gtRvoCurrent.width < MIN_WIDTH || gtRvoCurrent.width > MAX_MPEG_DECODE_WIDTH ||
       gtRvoCurrent.height < MIN_HEIGHT || gtRvoCurrent.height > MAX_MPEG_DECODE_HEIGHT ||
       (gtRvoCurrent.width & 7) != 0 ) {
        logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: invalid gtRvoCurrent.width or gtRvoCurrent.height! Rvo is disabled!\n");
        gRvoEnable = 0;
    }

    if(gtDisplayFormat.vout_format == NTSC) {
        width = VOUT_DISPLAY_NTSC_WIDTH;
        height = VOUT_DISPLAY_NTSC_HEIGHT >> (gtDisplayFormat.vout_interlaced == TRUE);        
    } else if(gtDisplayFormat.vout_format == PAL) {
        width = VOUT_DISPLAY_PAL_WIDTH;
        height = VOUT_DISPLAY_PAL_HEIGHT >> (gtDisplayFormat.vout_interlaced == TRUE);        
    } else if(gtDisplayFormat.vout_format == LCD) {
        width = gtDisplayFormat.width;
        height = gtDisplayFormat.height >> (gtDisplayFormat.vout_interlaced == TRUE);
    } else {
        logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: display format error!\n");
        gRvoEnable = 0;
        gPipEnable = 0;
        gOsdEnable = 0;
    }
       
    VDE_FM_SIZE = (width<<20) | (height<<8);    

    rvo_display_line = gtRvoCurrent.height;

    /* Rvo */
    switch(gtRvoCurrent.pixel_ratio) {
        case horizontal_no_scale:
            v0_s_ratio = 0;
            if((rvo_display_pixel = gtRvoCurrent.width) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }
             
            break;

        case horizontal_times_2:
            v0_s_ratio = 1;
            if((rvo_display_pixel = gtRvoCurrent.width * 2) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }
          
            break;

        case horizontal_times_4:
            v0_s_ratio = 2;
            if((rvo_display_pixel = gtRvoCurrent.width * 4) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }
             
            break;

        case horizontal_div_2:
            v0_s_ratio = 4;
            if((rvo_display_pixel = gtRvoCurrent.width / 2) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }
  
            break;

        case horizontal_div_4:
            v0_s_ratio = 5;
            if((rvo_display_pixel = gtRvoCurrent.width / 4) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }
 
            break;

        case horizontal_div_8:
            v0_s_ratio = 6;
            if((rvo_display_pixel = gtRvoCurrent.width / 8) & 0x7) {
                logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");   
                gRvoEnable = 0;
            }

            break;          

        default:
            v0_s_ratio = 0;
            logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo pixel ratio error! Rvo is disabled!\n");
            gRvoEnable = 0;
            break;                                  
    }
    printf("\nVideo_VDE_RegInit: rvo_display_pixel = %d!\n", rvo_display_pixel); 
                         
    switch(gtRvoCurrent.line_ratio) {
        case vertical_no_scale:
            rvo_display_line = gtRvoCurrent.height;       
            break;

        case vertical_times_2:
            rvo_display_line = gtRvoCurrent.height * 2;
            break;

        case vertical_times_4:
            rvo_display_line = gtRvoCurrent.height * 4;   
            break;

        case vertical_times_8:
            rvo_display_line = gtRvoCurrent.height * 8;   
            break;

        case vertical_div_2:
            rvo_display_line = gtRvoCurrent.height / 2;   
            break;

        case vertical_div_4:
            rvo_display_line = gtRvoCurrent.height / 4;   
            break;

        case vertical_div_8:
            rvo_display_line = gtRvoCurrent.height / 8;   
            break;          

        default:
            logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: rvo line ratio error! Rvo is disabled!\n");
            gRvoEnable = 0;
            break;                                  
    }
    printf("\nVideo_VDE_RegInit: rvo_display_line = %d!\n", rvo_display_line);   

    if(rvo_display_pixel < MIN_WIDTH || rvo_display_pixel > MAX_MPEG_DECODE_WIDTH ||
       rvo_display_line < MIN_HEIGHT || rvo_display_line > MAX_MPEG_DECODE_HEIGHT ||
       (gtRvoCurrent.width & 7) != 0 ) {
        logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: invalid rvo width or height! Rvo is disabled!\n");
        gRvoEnable = 0;
    }

    /* decide rvo start & stop, (for interlaced) */
    if(gtDisplayFormat.vout_format == LCD) {
        pixel_start = 0;
        line_start = 0;
    } else {
        pixel_start = (width - rvo_display_pixel) / 2 & ~0xf;   // should be multiple of 16  
        line_start = (height - (rvo_display_line >> (gtDisplayFormat.vout_interlaced == TRUE))) /2;     
    }
    
    pixel_stop  =  pixel_start + rvo_display_pixel;    
    line_stop  = line_start + (rvo_display_line >> (gtDisplayFormat.vout_interlaced == TRUE));
   
    VDE_RVO_START = (pixel_start << 20) | (line_start << 8);
    VDE_RVO_STOP  = (pixel_stop  << 20) | (line_stop  << 8);    

    VDE_OSD_START    = BIT_VDE_OSD_PIXEL_START | BIT_VDE_OSD_LINE_START >> (gtDisplayFormat.vout_interlaced == TRUE);
    VDE_OSD_STOP = BIT_VDE_OSD_PIXEL_STOP | BIT_VDE_OSD_LINE_STOP >> (gtDisplayFormat.vout_interlaced == TRUE);             

#ifdef PIP_ENABLE
    /* Pip */
    if(gPipEnable) {
        switch(gtPipCurrent.pixel_ratio) {
            case horizontal_no_scale:
                v1_s_ratio = 0;
                if((pip_display_pixel = gImageWidth) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;
                }
             
                break;

            case horizontal_times_2:
                v1_s_ratio = 1;
                if((pip_display_pixel = gImageWidth * 2) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;
                }
                      
                break;

            case horizontal_times_4:
                v1_s_ratio = 2;
                if((pip_display_pixel = gImageWidth * 4) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;
                }
                       
                break;

            case horizontal_div_2:
                v1_s_ratio = 4;
                if((pip_display_pixel = gImageWidth / 2) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;
                }
                         
                break;

            case horizontal_div_4:
                v1_s_ratio = 5;
                if((pip_display_pixel = gImageWidth / 4) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;
                }
                   
                break;

            case horizontal_div_8:
                v1_s_ratio = 6;
                if((pip_display_pixel = gImageWidth / 8) & 0x7) {
                    printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled! pip_display_pixel = %d!\n", pip_display_pixel);   
                    gPipEnable = 0;                
                }
                             
                break;          

            default:
                v1_s_ratio = 0;
                printf("\nVideo_VDE_RegInit: pip pixel ratio error! Pip is disabled!\n"); 
                gPipEnable = 0;   
                break;                                  
        }
        printf("\nVideo_VDE_RegInit: pip_display_pixel = %d\n", pip_display_pixel);  

        switch(gtPipCurrent.line_ratio) {
            case vertical_no_scale:
                pip_display_line = gImageHeight;           
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_times_2:
                pip_display_line = gImageHeight * 2;
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_times_4:
                pip_display_line = gImageHeight * 4;            
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_times_8:
                pip_display_line = gImageHeight * 8;            
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_div_2:
                pip_display_line = gImageHeight / 2;  
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_div_4:
                pip_display_line = gImageHeight / 4;   
                printf("pip_display_line=%d,", pip_display_line);
                break;

            case vertical_div_8:
                pip_display_line = gImageHeight / 8;    
                printf("pip_display_line=%d,", pip_display_line);                
                break;          

            default:
                printf("\nVideo_VDE_RegInit: pip line ratio error! Pip is disabled!\n");                 
                gPipEnable = 0;  
                break;                                  
        }
        printf("\nVideo_VDE_RegInit: pip_display_line = %d\n", pip_display_line);  

        if((gtPipSetting.start_pixel & ~0x7) + pip_display_pixel <= width && (gtPipSetting.start_line + pip_display_line >> (gtDisplayFormat.vout_interlaced == TRUE)) <= height) {
            VDE_PIP_START = ((gtPipSetting.start_pixel & ~0x7)<<20) | ((gtPipSetting.start_line >> (gtDisplayFormat.vout_interlaced == TRUE))<<8);            
            VDE_PIP_STOP = ((gtPipSetting.start_pixel & ~0x7) + pip_display_pixel<<20) | ((gtPipSetting.start_line + pip_display_line >> (gtDisplayFormat.vout_interlaced == TRUE))<<8);            
            printf("\n\nVideo_VDE_RegInit: PIP start/end pixel/line OK!\n\n");        
        } else {
            gPipEnable = 0;       
            logprintf(FATAL_ERROR, VSC, "\nVideo_VDE_RegInit: PIP go beyond the display!PIP is disabled\n");
        }      
    }
#endif
    
    //VDE_OSD_START    = BIT_VDE_OSD_PIXEL_START | BIT_VDE_OSD_LINE_START;
    //VDE_OSD_STOP = BIT_VDE_OSD_PIXEL_STOP | BIT_VDE_OSD_LINE_STOP/2;        

    VDE_S_RATIO    = BIT_VDE_S_RATIO_SC_ID | ((v0_s_ratio & 0x7)<<28) | ((v1_s_ratio & 0x7)<<25);    
    
    VDE_S_RATIO_COEF = BIT_VDE_S_RATIO_COEF0 | BIT_VDE_S_RATIO_COEF1 | BIT_VDE_S_RATIO_COEF2 | BIT_VDE_S_RATIO_COEF3;
    VDE_S_RATIO_SIGN = BIT_VDE_S_RATIO_SIGN_C0 | BIT_VDE_S_RATIO_SIGN_C1 | BIT_VDE_S_RATIO_SIGN_C2 | BIT_VDE_S_RATIO_SIGN_C3;

    //VDE_FILL_COLOR = BIT_VDE_FILL_COLOR_Y | BIT_VDE_FILL_COLOR_U | BIT_VDE_FILL_COLOR_V;    
    VDE_FILL_COLOR = 0x80808000;    // gray
   // VDE_FILL_COLOR = 0x55555500;    // green
    
    //VDE_OSD_T_MASK = BIT_VDE_OSD_TRANS_MASK;
    VDE_CT_CTRL    = BIT_VDE_CT_ENA_V0 | BIT_VDE_CT_ENA_V1 | BIT_VDE_CT_ENA_16 | BIT_VDE_CT_CTRL;        
    VDE_BT_CTRL    = BIT_VDE_BT_ENA_V0 | BIT_VDE_BT_ENA_V1 | BIT_VDE_BT_ADD_SUB | BIT_VDE_BT_CTRL;        
    VDE_SH_CTRL    = BIT_VDE_SH_ENA_V0 | BIT_VDE_SH_ENA_V1 | BIT_VDE_SH_CTRL | BIT_VDE_SH_THRES;        
    VDE_HUE_CTRL = BIT_VDE_HUE_ENA_V0 | BIT_VDE_HUE_ENA_V1 | BIT_VDE_HUE_CB_ADD_SUB | BIT_VDE_CB_HUE | BIT_VDE_HUE_CR_ADD_SUB | BIT_VDE_CR_HUE;        
    VDE_SA_CTRL    = BIT_VDE_SA_ENA_V0 | BIT_VDE_SA_ENA_V1 | BIT_VDE_SA_CTRL;        
    VDE_LBUF_BASE    = BIT_VDE_LBUF_U_BASE | BIT_VDE_LBUF_V_BASE;
    #if 0
    VDE_OSD_PT_0x0 = BIT_VDE_OSD_PT_CR_0x0 | BIT_VDE_OSD_PT_CB_0x0 | BIT_VDE_OSD_PT_Y_0x0;      
    VDE_OSD_PT_0x1 = BIT_VDE_OSD_PT_CR_0x1 | BIT_VDE_OSD_PT_CB_0x1 | BIT_VDE_OSD_PT_Y_0x1;
    VDE_OSD_PT_0x2 = BIT_VDE_OSD_PT_CR_0x2 | BIT_VDE_OSD_PT_CB_0x2 | BIT_VDE_OSD_PT_Y_0x2;
    VDE_OSD_PT_0x3 = BIT_VDE_OSD_PT_CR_0x3 | BIT_VDE_OSD_PT_CB_0x3 | BIT_VDE_OSD_PT_Y_0x3;
    VDE_OSD_PT_0x4 = BIT_VDE_OSD_PT_CR_0x4 | BIT_VDE_OSD_PT_CB_0x4 | BIT_VDE_OSD_PT_Y_0x4;
    VDE_OSD_PT_0x5 = BIT_VDE_OSD_PT_CR_0x5 | BIT_VDE_OSD_PT_CB_0x5 | BIT_VDE_OSD_PT_Y_0x5;
    VDE_OSD_PT_0x6 = BIT_VDE_OSD_PT_CR_0x6 | BIT_VDE_OSD_PT_CB_0x6 | BIT_VDE_OSD_PT_Y_0x6;  
    VDE_OSD_PT_0x7 = BIT_VDE_OSD_PT_CR_0x7 | BIT_VDE_OSD_PT_CB_0x7 | BIT_VDE_OSD_PT_Y_0x7; 
    VDE_OSD_PT_0x8 = BIT_VDE_OSD_PT_CR_0x8 | BIT_VDE_OSD_PT_CB_0x8 | BIT_VDE_OSD_PT_Y_0x8;
    VDE_OSD_PT_0x9 = BIT_VDE_OSD_PT_CR_0x9 | BIT_VDE_OSD_PT_CB_0x9 | BIT_VDE_OSD_PT_Y_0x9;
    VDE_OSD_PT_0xA = BIT_VDE_OSD_PT_CR_0xA | BIT_VDE_OSD_PT_CB_0xA | BIT_VDE_OSD_PT_Y_0xA;
    VDE_OSD_PT_0xB = BIT_VDE_OSD_PT_CR_0xB | BIT_VDE_OSD_PT_CB_0xB | BIT_VDE_OSD_PT_Y_0xB;
    VDE_OSD_PT_0xC = BIT_VDE_OSD_PT_CR_0xC | BIT_VDE_OSD_PT_CB_0xC | BIT_VDE_OSD_PT_Y_0xC;
    VDE_OSD_PT_0xD = BIT_VDE_OSD_PT_CR_0xD | BIT_VDE_OSD_PT_CB_0xD | BIT_VDE_OSD_PT_Y_0xD;
    VDE_OSD_PT_0xE = BIT_VDE_OSD_PT_CR_0xE | BIT_VDE_OSD_PT_CB_0xE | BIT_VDE_OSD_PT_Y_0xE;
    VDE_OSD_PT_0xF = BIT_VDE_OSD_PT_CR_0xF | BIT_VDE_OSD_PT_CB_0xF | BIT_VDE_OSD_PT_Y_0xF;
    #endif    
    VDE_VSC_WRITE_LJMP    |= 0x0;
    VDE_VSC_READ_V0_LJMP |= 0x0;

}

VOID AppVscInitialize(WORD VscSizeWidth, WORD VscSizeHeight, LONG Vsc_Y_skip, LONG Vsc_UV_skip,LONG Vsc_Y_scaling,LONG Vsc_UV_scaling)                                        
{
    VOUT_WIDTH = VscSizeWidth;
    VOUT_HEIGHT = VscSizeHeight;
    VSC_WIDTH = VscSizeWidth;
    VSC_HEIGHT = VscSizeHeight;    
    ScInit(VSC_WR_Y_S, sBufUse_Internal, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_WR_Y_S_ADDR);
    ScInit(VSC_WR_U_S, sBufUse_Internal, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_WR_U_S_ADDR);
    ScInit(VSC_WR_V_S, sBufUse_Internal, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_WR_V_S_ADDR);
    //Video_VDE_RegInit();                                                                                                                
    //Video_VOUT_RegInit();
    VOUT_VSC_SECRET_BIT = 0x0168E596;    // set 0xa8040000

    // VSC write extended video context need not be int here!(Because VSC send data to write streams, not MC!)
    McVideoExtendedInit(5,VSC_WIDTH,VSC_HEIGHT,Vsc_Y_skip,Vsc_UV_skip,Vsc_Y_scaling,Vsc_UV_scaling);    
}    

VOID AppRvoInitialize(WORD RVOSizeWidth, WORD RVOSizeHeight, WORD RVOInputFmt, WORD interlaced, WORD RVO_horizontal_ratio, WORD RVO_vertical_ratio)
{

    WORD y_addr, u_addr, v_addr;
    
    WORD RVO_Y_skip;
    WORD RVO_Y_scaling;
    WORD RVO_UV_skip;
    WORD RVO_UV_scaling;

    unsigned int i = 0;
    unsigned int *pstatic;    

    if(interlaced == TRUE) {
        printf("\nAppRvoInitialize: RVO interlaced! gtRvoCurrent.interlaced = %d!\n", gtRvoCurrent.interlaced);
    } else {
        printf("\nAppRvoInitialize: RVO progressive! gtRvoCurrent.interlaced = %d!\n", gtRvoCurrent.interlaced);
    }

    printf("\nAppRvoInitialize paras: width=%d, height=%d, format=%d, horizontal_ratio=%d, vertical_ratio=%d!\n", \
                RVOSizeWidth, RVOSizeHeight, RVOInputFmt, RVO_horizontal_ratio, RVO_vertical_ratio);

    switch(RVO_horizontal_ratio) {
        case horizontal_no_scale:
            break;

        case horizontal_div_2:
            switch(RVO_vertical_ratio) {
                case vertical_no_scale:
                    RVO_vertical_ratio = vertical_times_2;
                    break;

                case vertical_div_2:
                    RVO_vertical_ratio = vertical_no_scale;                    
                    break;

                case vertical_div_4:
                    RVO_vertical_ratio = vertical_div_2;                    
                    break;

                case vertical_div_8:
                    RVO_vertical_ratio = vertical_div_4;                    
                    break;

                case vertical_times_2:
                    RVO_vertical_ratio = vertical_times_4;                    
                    break;
                
                case vertical_times_4:
                    RVO_vertical_ratio = vertical_times_8;                    
                    break;         
                    
                case vertical_times_8:
                default:
                    printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
                    gRvoEnable = 0;
                    return;                                          
            }
            break;

        case horizontal_div_4:
            switch(RVO_vertical_ratio) {
                case vertical_no_scale:
                    RVO_vertical_ratio = vertical_times_4;
                    break;

                case vertical_div_2:
                    RVO_vertical_ratio = vertical_times_2;                    
                    break;

                case vertical_div_4:
                    RVO_vertical_ratio = vertical_no_scale;                    
                    break;

                case vertical_div_8:
                    RVO_vertical_ratio = vertical_div_2;                    
                    break;

                case vertical_times_2:     
                    RVO_vertical_ratio = vertical_times_8;
                    break;
                    
                case vertical_times_4:
                case vertical_times_8:                    
                default:
                    printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
                    gRvoEnable = 0;
                    return;                                          
            }
            break;            

        case horizontal_div_8:
            switch(RVO_vertical_ratio) {
                case vertical_no_scale:
                    RVO_vertical_ratio = vertical_times_8;
                    break;

                case vertical_div_2:
                    RVO_vertical_ratio = vertical_times_4;                    
                    break;

                case vertical_div_4:
                    RVO_vertical_ratio = vertical_times_2;                    
                    break;

                case vertical_div_8:
                    RVO_vertical_ratio = vertical_no_scale;                    
                    break;

                case vertical_times_2:                
                case vertical_times_4:
                case vertical_times_8:                    
                default:
                    printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
                    gRvoEnable = 0;
                    return;                                          
            }
            break;

        case horizontal_times_2:
            switch(RVO_vertical_ratio) {
                case vertical_no_scale:
                    RVO_vertical_ratio = vertical_div_2;
                    break;

                case vertical_div_2:
                    RVO_vertical_ratio = vertical_div_4;                    
                    break;

                case vertical_div_4:
                    RVO_vertical_ratio = vertical_div_8;                    
                    break;
                    
                case vertical_times_2:
                    RVO_vertical_ratio = vertical_no_scale;                    
                    break;
                
                case vertical_times_4:
                    RVO_vertical_ratio = vertical_times_2;                    
                    break;         
                    
                case vertical_times_8:
                    RVO_vertical_ratio = vertical_times_4;                    
                    break;         
                    
                case vertical_div_8:
                default:
                    printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
                    gRvoEnable = 0;
                    return;                                          
            }
            break;

        case horizontal_times_4:
            switch(RVO_vertical_ratio) {
                case vertical_no_scale:
                    RVO_vertical_ratio = vertical_div_4;
                    break;

                case vertical_div_2:
                    RVO_vertical_ratio = vertical_div_8;                    
                    break;
                    
                case vertical_times_2:
                    RVO_vertical_ratio = vertical_div_2;                    
                    break;
                
                case vertical_times_4:
                    RVO_vertical_ratio = vertical_no_scale;                    
                    break;         
                    
                case vertical_times_8:
                    RVO_vertical_ratio = vertical_times_2;                    
                    break;         

                case vertical_div_4:                    
                case vertical_div_8:
                default:
                    printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
                    gRvoEnable = 0;
                    return;                                          
            }
            break;

        default:
            printf("\nAppRvoInitialize: horizontal ratio error(%d)! RVO is disabled!\n", RVO_horizontal_ratio);
            gRvoEnable = 0;
            return;               
    }

    if (RVOInputFmt == YUV422) {    // 422    
        switch(RVO_vertical_ratio) {
            case vertical_no_scale:       
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;              
                    }                             
                }                
                break;
        
            case vertical_times_2:        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;              
                    }                             
                }                  
                break;
        
            case vertical_times_4:    
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 1;              
                    }                             
                }                
                break;
        
            case vertical_times_8:
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;                        
                    } else {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 1;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 1;              
                    }                             
                }                   
                break;
        
            case  vertical_div_2:        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;                        
                    } else {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;              
                    }                             
                }                
                break;
        
            case vertical_div_4:     
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;                        
                    } else {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;              
                    }                             
                }                  
                break;

            case vertical_div_8:     
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;                        
                    } else {
                        printf("\nAppRvoInitialize: YUV422, Vout interlaced,  Vsc progressive, vertical ratio error(%d)! RVO is disabled!\n", RVO_vertical_ratio);
                        gRvoEnable = 0;
                        return;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                          
                    } else {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 0;              
                    }                             
                }                  
                break;
                
            default:
                printf("\nAppRvoInitialize: YUV422, Vout progressive/interlaced,  Vsc progressive/interlaced, vertical ratio error(%d)! RVO is disabled!\n", RVO_vertical_ratio);
                gRvoEnable = 0;
                return;                       
        }                                                
    } else if (RVOInputFmt == YUV420) {    // input is 420, but output is 422   
        switch(RVO_vertical_ratio) {  
            case vertical_no_scale:     // no vertical scale        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                            
                    } else {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;              
                    }                             
                }
                break;
                        
            case vertical_times_2:
                if(gtDisplayFormat.vout_interlaced) {  
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
#if 0                    
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                 
#endif                        
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;                 
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                         
                    } else {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 1;                
                    }                
                }
                break;
                
            case vertical_times_4:
                if(gtDisplayFormat.vout_interlaced) {     
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
#if 0                    
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                 
#endif
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 1;                 
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return;                         
                    } else {
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 1;                
                    }                 
                }
                break;
                        
            case vertical_times_8:
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 0;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 1;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {
#if 0                    
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                 
#endif
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 1;
                        RVO_UV_skip = 3;
                        RVO_UV_scaling = 1;                 
                    }               
                } else {    // Vout progressive
                    printf("\nAppRvoInitialize: YUV420, Vout progressive,  Vsc progressive/interlaced, vertical ratio error(%d)! RVO is disabled!\n", RVO_vertical_ratio);
                    gRvoEnable = 0;
                    return;                          
                }
                break;
                             
            case vertical_div_2:    
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;
                    } else {                
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;                
                    }                
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return; 
                    } else {                
                        RVO_Y_skip = 1;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 0;
                        RVO_UV_scaling = 0;                
                    }
                }
                break;
                       
            case vertical_div_4:
                if(gtDisplayFormat.vout_interlaced) {
                    if(interlaced == TRUE) {             
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;                        
                    } else {
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;                
                    }
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return; 
                    } else {                
                        RVO_Y_skip = 2;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 1;
                        RVO_UV_scaling = 0;                
                    }                
                }            
                break;
                
            case vertical_div_8:
                if(gtDisplayFormat.vout_interlaced) {    
                    if(interlaced == TRUE) {           
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;      
                        RVOSizeHeight = RVOSizeHeight/2;  
                    } else {    
                        printf("\nAppRvoInitialize: YUV420, Vout progressive,  Vsc progressive/interlaced, vertical ratio error(%d)! RVO is disabled!\n", RVO_vertical_ratio);
                        gRvoEnable = 0;
                        return;                
                    }
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppRvoInitialize: Vsc interlaced does not work when Vout progressive!RVO is disabled!\n");
                        gRvoEnable = 0;
                        return; 
                    } else {                
                        RVO_Y_skip = 3;
                        RVO_Y_scaling = 0;
                        RVO_UV_skip = 2;
                        RVO_UV_scaling = 0;                
                    }                
                }     
                break;
                
            default:
                printf("\nAppRvoInitialize: YUV420, Vout progressive/interlaced,  Vsc progressive/interlaced, vertical ratio error(%d)! RVO is disabled!\n", RVO_vertical_ratio);
                gRvoEnable = 0;
                return;
        }
    } else {
        printf("\nAppRvoInitialize: format error: not YUV420 or YUV422!\n");
        gRvoEnable = 0;
        return;
    }
    
    //Video_VDE_RegInit();
    ScInit(VSC_RD_Y0_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_Y0_S_ADDR);
    ScInit(VSC_RD_U0_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_U0_S_ADDR);
    ScInit(VSC_RD_V0_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_V0_S_ADDR);
    McVideoExtendedInit(2, RVOSizeWidth, RVOSizeHeight, RVO_Y_skip, RVO_UV_skip, RVO_Y_scaling, RVO_UV_scaling);
    //McVideoExtendedInit(2,RVOSizeWidth,RVOSizeHeight,1 ,1 ,0,0);
    McScMap(VSC_RD_Y0_M, VSC_RD_Y0_S);
    McScMap(VSC_RD_U0_M, VSC_RD_U0_S);
    McScMap(VSC_RD_V0_M, VSC_RD_V0_S);

/*
    pstatic = (unsigned int *)((unsigned int)gMpeg4DecodeOptIaddr2 |0xa0000000);
    for (i = 0; i < gtStartupPic.width * gtStartupPic.height /4; i++)
        *pstatic++ = startup_picture_yuv[i];

    pstatic = (unsigned int *)((unsigned int)gMpeg4DecodeOptIaddr2 + MAX_WIDTH * MAX_HEIGHT  | 0xa0000000);
    for (i = 0; i < gtStartupPic.width * gtStartupPic.height /16; i++)
        *pstatic++ = startup_picture_yuv[gtStartupPic.width * gtStartupPic.height /4 + i];

    pstatic = (unsigned int *)((unsigned int)gMpeg4DecodeOptIaddr2 + MAX_WIDTH * MAX_HEIGHT + MAX_WIDTH * MAX_HEIGHT /4 | 0xa0000000);
    for (i = 0; i < gtStartupPic.width * gtStartupPic.height /16; i++)
        *pstatic++ = startup_picture_yuv[gtStartupPic.width * gtStartupPic.height /4 + gtStartupPic.width * gtStartupPic.height /16 + i];*/

#ifdef STARTUP_PICTURE               

    gtRvoCurrent.y_addr = startupPic_addr & 0x1fffffff;
    gtRvoCurrent.u_addr = gtRvoCurrent.y_addr + gtStartupPic.width * gtStartupPic.height;
    if(gtStartupPic.yuv_input_format == YUV420)
        gtRvoCurrent.v_addr = gtRvoCurrent.u_addr + gtStartupPic.width * gtStartupPic.height /4;
    else    // 422
        gtRvoCurrent.v_addr = gtRvoCurrent.u_addr + gtStartupPic.width * gtStartupPic.height /2;  
    
#else
    memset((unsigned int)gMpeg4DecodeOptIaddr2 | 0xa0000000, 0x80ff80ff, MPEG_DECODE_OUTPUT_BUFSIZE);
    gtRvoCurrent.y_addr = ((unsigned int)gMpeg4DecodeOptIaddr2) & 0x1fffffff;
    gtRvoCurrent.u_addr = gtRvoCurrent.y_addr + ((mp4DecodeParams.opt_cb_offset)<<8);
    gtRvoCurrent.v_addr = gtRvoCurrent.y_addr + ((mp4DecodeParams.opt_cr_offset)<<8);
#endif

    if(OSQPost(gptMpeg4DecodeOutputReady, &gtRvoCurrent) != OS_NO_ERR) {
        printf("\nAppRvoInitialize: OSQPost error!\n");
    }

    if(gVscOddEvenFrameSelect == EVEN_FRAME_FIRST)
        gtRvoCurrent.y_addr += gtRvoCurrent.width;
    McVideoInit(VSC_RD_Y0_M, 0, 0, gtRvoCurrent.y_addr);
    McVideoInit(VSC_RD_U0_M, 0, 0, gtRvoCurrent.u_addr);
    McVideoInit(VSC_RD_V0_M, 0, 0, gtRvoCurrent.v_addr);

}

VOID AppPipInitialize(WORD PIPSizeWidth, WORD PIPSizeHeight, WORD PIPInputFmt, WORD interlaced, WORD PIP_horizontal_ratio, WORD PIP_vertical_ratio)
{
    WORD PIP_Y_skip;
    WORD PIP_Y_scaling;
    WORD PIP_UV_skip;
    WORD PIP_UV_scaling;

    printf("\nAppPipInitialize paras: width=%d, height=%d, format=%d, horizontal_ratio=%d, vertical_ratio=%d!\n", \
                PIPSizeWidth, PIPSizeHeight, PIPInputFmt, PIP_horizontal_ratio, PIP_vertical_ratio);


    if (PIPInputFmt == YUV422) {    // 422    
        switch(PIP_vertical_ratio) {
            case vertical_no_scale:       
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;              
                    }                             
                }                
                break;
        
            case vertical_times_2:        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;              
                    }                             
                }                  
                break;
        
            case vertical_times_4:    
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 1;              
                    }                             
                }                
                break;
        
            case vertical_times_8:
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;                        
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 1;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 1;              
                    }                             
                }                   
                break;
        
            case  vertical_div_2:        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;                        
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;              
                    }                             
                }                
                break;
        
            case vertical_div_4:     
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;                        
                    } else {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;              
                    }                             
                }                  
                break;

            case vertical_div_8:     
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;                        
                    } else {
                        printf("\nAppPipInitialize: YUV422, Vout interlaced,  Vsc progressive, vertical ratio error(%d)! PIP is disabled!\n", PIP_vertical_ratio);
                        gPipEnable = 0;
                        return;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                          
                    } else {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 0;              
                    }                             
                }                  
                break;
                
            default:
                printf("\nAppPipInitialize: YUV422, Vout progressive/interlaced,  Vsc progressive/interlaced, vertical ratio error(%d)! PIP is disabled!\n", PIP_vertical_ratio);
                gPipEnable = 0;
                return;                       
        }                                                
    } else if (PIPInputFmt == YUV420) {    // input is 420, but output is 422   
        switch(PIP_vertical_ratio) {  
            case vertical_no_scale:     // no vertical scale        
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                            
                    } else {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;              
                    }                             
                }
                break;
                        
            case vertical_times_2:
                if(gtDisplayFormat.vout_interlaced) {  
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;                 
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                         
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 1;                
                    }                
                }
                break;
                
            case vertical_times_4:
                if(gtDisplayFormat.vout_interlaced) {     
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 1;                 
                    }
                } else {    // Vout progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return;                         
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 1;                
                    }                 
                }
                break;
                        
            case vertical_times_8:
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 0;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 1;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 1;
                        PIP_UV_skip = 3;
                        PIP_UV_scaling = 1;                 
                    }               
                } else {    // Vout progressive
                    printf("\nAppPipInitialize: YUV420, Vout progressive,  Vsc progressive/interlaced, vertical ratio error(%d)! PIP is disabled!\n", PIP_vertical_ratio);
                    gPipEnable = 0;
                    return;                          
                }
                break;
                             
            case vertical_div_2:    
                if(gtDisplayFormat.vout_interlaced) {       
                    if(interlaced == TRUE) {
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;
                    } else {                
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;                
                    }                
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return; 
                    } else {                
                        PIP_Y_skip = 1;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 0;
                        PIP_UV_scaling = 0;                
                    }
                }
                break;
                       
            case vertical_div_4:
                if(gtDisplayFormat.vout_interlaced) {
                    if(interlaced == TRUE) {             
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;                        
                    } else {
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;                
                    }
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return; 
                    } else {                
                        PIP_Y_skip = 2;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 1;
                        PIP_UV_scaling = 0;                
                    }                
                }            
                break;
                
            case vertical_div_8:
                if(gtDisplayFormat.vout_interlaced) {    
                    if(interlaced == TRUE) {           
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;      
                        PIPSizeHeight = PIPSizeHeight/2;  
                    } else {    
                        printf("\nAppPipInitialize: YUV420, Vout progressive,  Vsc progressive/interlaced, vertical ratio error(%d)! PIP is disabled!\n", PIP_vertical_ratio);
                        gPipEnable = 0;
                        return;                
                    }
                } else {    // progressive
                    if(interlaced == TRUE) {
                        printf("\nAppPipInitialize: Vsc interlaced does not work when Vout progressive!PIP is disabled!\n");
                        gPipEnable = 0;
                        return; 
                    } else {                
                        PIP_Y_skip = 3;
                        PIP_Y_scaling = 0;
                        PIP_UV_skip = 2;
                        PIP_UV_scaling = 0;                
                    }                
                }     
                break;
                
            default:
                printf("\nAppPipInitialize: YUV420, Vout progressive/interlaced,  Vsc progressive/interlaced, vertical ratio error(%d)! PIP is disabled!\n", PIP_vertical_ratio);
                gPipEnable = 0;
                return;
        }
    } else {
        printf("\nAppPipInitialize: format error: not YUV420 or YUV422!\n");
        gPipEnable = 0;
        return;
    }

#if 0
    if(PIPInputFmt == YUV422) {   // 422
        switch(PIP_vertical_ratio) {    
            case vertical_no_scale:        
                PIP_Y_skip =1;
                PIP_Y_scaling =0;
                PIP_UV_skip =1;
                PIP_UV_scaling=0;
                break;
                
            case vertical_times_2:        
                PIP_Y_skip=0;
                PIP_Y_scaling=0;
                PIP_UV_skip=0;
                PIP_UV_scaling=0;
                break;
        
            case vertical_times_4:        
                PIP_Y_skip=1;
                PIP_Y_scaling=1;
                PIP_UV_skip=1;
                PIP_UV_scaling=1;
                break;
        
            case vertical_times_8:        
                PIP_Y_skip=2;
                PIP_Y_scaling=1;
                PIP_UV_skip=2;
                PIP_UV_scaling=1;
                break;                
        
            case  vertical_div_2:        
                PIP_Y_skip=2;
                PIP_Y_scaling=0;
                PIP_UV_skip=2;
                PIP_UV_scaling=0;
                break;
        
            case vertical_div_4:        
                PIP_Y_skip=3;
                PIP_Y_scaling=0;
                PIP_UV_skip=3;
                PIP_UV_scaling=0;
                break;
                
            default:
                logprintf(FATAL_ERROR, VSC, "\npip vertical ratio error! Pip is disabled!\n");
                gPipEnable = 0;
                return;                
        }                
    } else if(PIPInputFmt == YUV420) {    // input is 420, but output is 422  
        switch(PIP_vertical_ratio) {
            case vertical_no_scale:     // no vertical scale        
                if(gtDisplayFormat.vout_interlaced) {       
                    PIP_Y_skip = 1;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 0;
                    PIP_UV_scaling = 0;                
                } else {
                    PIP_Y_skip = 0;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 1;
                    PIP_UV_scaling = 1;                
                }
                break;
                        
            case vertical_times_2:
                if(gtDisplayFormat.vout_interlaced) {      
                    /*PIP_Y_skip = 1;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 0;
                    PIP_UV_scaling = 0;                */
                                
                    PIP_Y_skip = 0;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 1;
                    PIP_UV_scaling = 1;                                
                } else {
                    PIP_Y_skip = 1;
                    PIP_Y_scaling = 1;
                    PIP_UV_skip = 2;
                    PIP_UV_scaling = 1;                
                }
                break;
                
            case vertical_times_4:
                if(gtDisplayFormat.vout_interlaced) {       
                    PIP_Y_skip = 1;
                    PIP_Y_scaling = 1;
                    PIP_UV_skip = 2;
                    PIP_UV_scaling = 1;                
                } else {
                    PIP_Y_skip = 2;
                    PIP_Y_scaling = 1;
                    PIP_UV_skip = 3;
                    PIP_UV_scaling = 1;                
                }  
                break;
                      
            case vertical_times_8:
                if(gtDisplayFormat.vout_interlaced) {       
                    PIP_Y_skip = 2;
                    PIP_Y_scaling = 1;
                    PIP_UV_skip = 3;
                    PIP_UV_scaling = 1;                
                } else {
                    logprintf(FATAL_ERROR, VSC, "\npip vertical ratio error! Pip is disabled!\n");
                    gPipEnable = 0;
                    return;                          
                }
                break;
                             
            case vertical_div_2:
                if(gtDisplayFormat.vout_interlaced) {       
                    PIP_Y_skip = 2;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 1;
                    PIP_UV_scaling = 0;                
                } else {
                    PIP_Y_skip = 1;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 0;
                    PIP_UV_scaling = 0;                                
                }
                break;
                       
            case vertical_div_4:
                if(gtDisplayFormat.vout_interlaced) {       
                    PIP_Y_skip = 3;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 2;
                    PIP_UV_scaling = 0;                
                } else {
                    PIP_Y_skip = 2;
                    PIP_Y_scaling = 0;
                    PIP_UV_skip = 1;
                    PIP_UV_scaling = 0;                 
                }
                break;
                            
            case vertical_div_8:
                if(gtDisplayFormat.vout_interlaced) {        
                    logprintf(FATAL_ERROR, VSC, "\npip vertical ratio error! Pip is disabled!\n");
                    gPipEnable = 0;
                    return;                
                }

                PIP_Y_skip = 3;            
                PIP_Y_scaling = 0;
                PIP_UV_skip = 2;
                PIP_UV_scaling = 0;
                break;
                                
            default:
               logprintf(FATAL_ERROR, VSC, "\npip vertical ratio error! Pip is disabled!\n");
                gPipEnable = 0;
                return;
        }
    } else {
        logprintf(FATAL_ERROR, VSC, "\npip vertical ratio error! Pip is disabled!\n");
        gPipEnable = 0;
        return;
    }
#endif    

    ScInit(VSC_RD_Y1_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_Y1_S_ADDR);
    ScInit(VSC_RD_U1_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_U1_S_ADDR);
    ScInit(VSC_RD_V1_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_512Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_V1_S_ADDR);
    McVideoExtendedInit(3, PIPSizeWidth, PIPSizeHeight, PIP_Y_skip, PIP_UV_skip, PIP_Y_scaling, PIP_UV_scaling); 
    McScMap(VSC_RD_Y1_M, VSC_RD_Y1_S);
    McScMap(VSC_RD_U1_M, VSC_RD_U1_S);
    McScMap(VSC_RD_V1_M, VSC_RD_V1_S);

    memset(VIN_YUV_START_ADDR|0xa0000000, 0x80ff80ff, MAX_VIN_WIDTH * MAX_VIN_HEIGHT * 3 / 2);
    gtPipCurrent.y_addr = VIN_YUV_START_ADDR & 0x1fffffff;
    gtPipCurrent.u_addr = gtPipCurrent.y_addr + MAX_VIN_Y_SIZE;
    gtPipCurrent.v_addr = gtPipCurrent.u_addr + MAX_VIN_UV_SIZE;

#ifdef PIP_ENABLE
    if(OSQPost(gptMqueueVideoInFrame, &gtPipCurrent) != OS_NO_ERR) {
        printf("\nAppRvoInitialize: OSQPost error!\n");
    }
#endif    
    McVideoInit(VSC_RD_Y1_M, 0, 0, gtPipCurrent.y_addr);
    McVideoInit(VSC_RD_U1_M, 0, 0, gtPipCurrent.u_addr);
    McVideoInit(VSC_RD_V1_M, 0, 0, gtPipCurrent.v_addr);  
} 

#ifdef OSD_ENABLE
/*
*********************************************************************************************
*                                       OSD initialize
*
* Description: This routine inits the video registers.
*
* Arguments  : OSDSizeWidth is the OSD display width(0~720).OSDSizeHeight is the OSD display heigth(0~480),
*                   OSD_x_start, OSD_y_start OSD display location,backgroundColor is OSD background color(0~F),
*                   OSD_T_Mask the color to transparent
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
VOID AppOSDInitialize(WORD OSDSizeWidth, WORD OSDSizeHeight,  WORD OSD_x_start, WORD OSD_y_start, unsigned char backgroundColor,unsigned char  OSD_T_Mask,unsigned int addr)
{  
    unsigned int i,j,k=0,temp;
    OSD_PIXEL_START = OSD_x_start;
    OSD_LINE_START =OSD_y_start; 
    OSD_PIXEL_STOP = OSD_x_start + OSDSizeWidth; 
    OSD_LINE_STOP = OSD_y_start + OSDSizeHeight; 


    if(OSD_PIXEL_STOP>gtOsdCurrent.width  || OSD_LINE_STOP>gtOsdCurrent.height){
        printf("\r\n OSD error!");
        gOsdEnable=0;  
        return ;
    }

    if(OSDSizeWidth==0 || OSDSizeHeight==0){
        printf("\r\n OSD error!");
        gOsdEnable=0;  
        return ;
    }

    if(OSD_PIXEL_START%8 !=0 || OSDSizeWidth%8 !=0){
        gOsdEnable=0;
        printf("\r\n OSD error!");
        return ;
     }
    
        
    ScInit(VSC_RD_OSD_S, sBufUse_Sdram, sDBSize_4Byte, sBufSize_256Byte, sSBSize_4Byte, sMBSize_32Byte, sSwap_no, sLast_Disable, VSC_RD_OSD_S_ADDR);
    McScMap(VSC_RD_OSD_M, VSC_RD_OSD_S);


    McVideoExtendedInit(4, OSDSizeWidth,OSDSizeHeight, gtDisplayFormat.vout_interlaced == TRUE, gtDisplayFormat.vout_interlaced == TRUE, 0, 0); 

    McVideoInit(VSC_RD_OSD_M, 0, 0, addr);
    EN_TRANS_MASK( OSD_T_Mask);  
    EN_VDE_OSD_TRANS();// add by zjb Nov 30th
    EN_VDE_OSD();
}
#endif

VOID AppVscStart(VOID)
{    
    ScOpen(VSC_WR_Y_S);
    ScOpen(VSC_WR_U_S);
    ScOpen(VSC_WR_V_S);                                                                                                                             
    EN_VDE();    
    EN_VOUT_VIDEO(); 
} 

VOID AppVscStop(VOID)
{
    DIS_VOUT_VIDEO(); 
    DIS_VDE();
    DIS_VDE_RVO();                                                                                                                                    
    DIS_VDE_PIP();                                                                                                                                
    DIS_VDE_OSD(); 
    ScClose(VSC_RD_Y0_S);
    ScClose(VSC_RD_U0_S);
    ScClose(VSC_RD_V0_S);
    ScClose(VSC_RD_Y1_S);
    ScClose(VSC_RD_U1_S);
    ScClose(VSC_RD_V1_S);
    ScClose(VSC_RD_OSD_S);
    ScClose(VSC_WR_Y_S);
    ScClose(VSC_WR_U_S);
    ScClose(VSC_WR_V_S);
} 

/*
*********************************************************************************************
*                               DisplayColorBlock
*
* Description:         This fuction diaplay a color block in the OSD plate
*
* Arguments  :         x         is the block start x location, has to be even number
                    y        is the block start y location
*                    size_x     is the number of pixels of the block, has to be even number
                    size_y     is the number of lines of the block,
                    color     is the color of the block, there are 16 different color 
                            BLACK,DARK_RED,DARK_GREEN,BROWN,DARK_BLUE,PURPULE,GREEN,GREY,
                            LIGHT_GREY,RED,LIGHT_GREEN,YELLOW,BLUE,LIGHT_PURPLE,LIGHT_BLUE,WHITE                    
                                
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/                                
#ifdef OSD_ENABLE                                
/*VOID AppDisplayColorBlock(SHORT x, SHORT y, SHORT size_x, SHORT size_y, BYTE color)
{          
    int i, j;
    
    for(i=0;i<size_y;i++)
    {

        for(j=0;j<size_x/2;j++)
        {    
                (*(char *)(0xa0000000 + OSD_memory + (0x140*(y+i)) + (x/2) +j)) = color;
        }
    }
}*/

/* clear the memory of OSD from 0xa00000 for 4k*/

void InitOsdMemory(WORD OSDStartPixel, WORD OSDStartLine,WORD OSDSizeWidth, WORD OSDSizeHeight,unsigned char Fill_color,unsigned int pic_addr,unsigned char  OSD_T_Mask)
{
    gtClearScreenSetting tClearScreenParameters;
 
    unsigned int   i,j;

    OS_CPU_SR    cpu_sr;

    printf("\nstart InitOsdMemory!\n"); 

    gtOsdCurrent.color_pallate= (unsigned int *)color_index;
    Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);

    tClearScreenParameters.pixel_start = OSDStartPixel;
    tClearScreenParameters.line_start = OSDStartLine;
    tClearScreenParameters.width = OSDSizeWidth;
    tClearScreenParameters.height = OSDSizeHeight;
    tClearScreenParameters.attribute = 0;    
    Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters);

    tClearScreenParameters.attribute = 0x1;
    Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters);
    
    printf("\nstop InitOsdMemory!\n"); 
    
}




VOID Vb2100FillOsdPT(unsigned char dev, unsigned char path, unsigned int *index_addr)
{

    unsigned int i, j, index_mun;
    unsigned int *osd_pt;
    unsigned char *index_data;

    index_data = (unsigned char *)index_addr;

    index_mun = index_data[0];

    osd_pt = (volatile unsigned *)(VDE_BASE_ADDR + 0x80 + 0*0x04);		/* This is the address to access OSD palette table through CBUS 1st address entry */
     
    for(i=1, j=1;i<=index_mun;i++,j+=4){
        *osd_pt = index_data[j] |index_data[j+1]<<8 |index_data[j+2]<<16;
        osd_pt++;      
    }

}



VOID Vb2100FillBmp(unsigned char dev, unsigned char path, unsigned int bmp_start_pixel,unsigned int  bmp_start_line,unsigned int *index_addr, unsigned int *bmp_addr, unsigned int attribute)
{
    unsigned int i,j,k;
    unsigned char page;
    
    unsigned char *bmp_data;
    unsigned char color1, color2, color;
    unsigned int  addr;    
    unsigned int x,y;
    unsigned char FillColor;
    unsigned int bmp_stop_pixel, bmp_stop_line;

    OS_CPU_SR    cpu_sr;

    page = attribute&0xf;
 
    bmp_data = (unsigned char *)bmp_addr;

    gtBmpParameters.height = bmp_data[0]<<8 | bmp_data[1];
    gtBmpParameters.width = bmp_data[2]<<8 | bmp_data[3];
    printf("W=%d,H=%d\n", gtBmpParameters.width, gtBmpParameters.height);

    bmp_stop_pixel= bmp_start_pixel+gtBmpParameters.width;
    bmp_stop_line = bmp_start_line+gtBmpParameters.height;


    if(bmp_stop_pixel > gtOsdCurrent.width|| bmp_stop_line > gtOsdCurrent.height){
        printf("\r\nVb2100FillBmp:stop position coordinate is overrun!");
        gOsdEnable=0;  
        return ;          
    }

    if(gtBmpParameters.width==0 || gtBmpParameters.height==0){
        printf("\r\nVb2100FillBmp:width or height is error!\n");
        gOsdEnable=0;
        return ;
    }
    
    if(bmp_start_pixel%8 !=0 || gtBmpParameters.width%8 !=0){
        printf("\r\nVb2100FillBmp is error!");
        gOsdEnable=0;
        return ;
     }

    gtOsdCurrent.color_pallate = index_addr;
    Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH,  gtOsdCurrent.color_pallate);
   
    switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

    }
    

    bmp_data +=5;
 
    //printf("\r\n start to fill bmp!"); 
    
    for(j=0; j<gtBmpParameters.height; j++){
        y = bmp_start_line+j;
  
        for(i=0; i<gtBmpParameters.width; i+=2){
            x= bmp_start_pixel+i;
            color1 = *bmp_data++;
            color2 = *bmp_data++;

            if(color1==0xff) color1 = 0;
            if(color2==0xff) color2 = 0;

            color = color2<<4 | color1;

            addr = (x+y*gtOsdCurrent.width)/2;
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)(gtOsdCurrent.fill_addr+addr)=color;
            OS_EXIT_CRITICAL();
        }
    }
    
    //printf("\r\n fill bmp succed!"); 
   
}



VOID Vb2100ClearScreen(unsigned char dev, unsigned char path, gtClearScreenSetting tClearScreenParameters)
{
    unsigned int pixel_start, line_start, pixel_end, line_end;
    unsigned int width, height;
    unsigned char page;

    unsigned int i, j, x_pos, y_pos;
    unsigned char FillColor, Fill_color, DisplayColor;
    unsigned int  addr; 

    OS_CPU_SR    cpu_sr;

    pixel_start = tClearScreenParameters.pixel_start;
    line_start = tClearScreenParameters.line_start;
    width = tClearScreenParameters.width;
    height = tClearScreenParameters.height;
    page = tClearScreenParameters.attribute&0xf;

    pixel_end = pixel_start+width;
    line_end = line_start+height;


    if(pixel_end>gtOsdCurrent.width || line_end>gtOsdCurrent.height ){
        printf("\r\nVb2100FillColor:end position coordinate is overrun!");
        gOsdEnable=0; 
        return ;
    }

    if(width==0 || height==0){
        printf("\r\nVb2100ClearScreen:width or height is error!");
        gOsdEnable=0; 
        return ;                     
    }


    switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

    }
    
    Fill_color = gtOsdCurrent.OSD_T_Mask;

    for(j=0;j<height;j++){
            x_pos = pixel_start;
            y_pos = line_start+j;
            if(pixel_start%2 != 0){
                x_pos --;

                addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

                DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
                FillColor = (Fill_color<<4) |(DisplayColor&0xf);
                
                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)(gtOsdCurrent.fill_addr +addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                x_pos += 2;
            }
            
            FillColor = Fill_color<<4 | Fill_color;
            for(i=0;i<width/2;i++){
                addr = (x_pos+y_pos*gtOsdCurrent.width)/2;
                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                x_pos += 2;
            }
            
            if(width%2 !=0){

                addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

                DisplayColor = *(volatile unsigned char*)(gtOsdCurrent.fill_addr+addr);
                FillColor = (DisplayColor&0xf0) | Fill_color;
                
                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)(gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
            }
        }

}




VOID Vb2100FillColor(unsigned char dev, unsigned char path, gtFillColorSetting tFillColorParameters)
{
    unsigned int pixel_start, line_start, pixel_end,line_end;
    unsigned int width, height;
    unsigned char page, color;

    unsigned int i, j, x_pos, y_pos;
    unsigned char FillColor;
    unsigned char DisplayColor;
    unsigned int  addr; 

    OS_CPU_SR    cpu_sr;

    pixel_start = tFillColorParameters.pixel_start;
    line_start = tFillColorParameters.line_start;
    width = tFillColorParameters.width;
    height = tFillColorParameters.height;
    page = tFillColorParameters.attribute&0xf;
    color = (tFillColorParameters.attribute>>0x4)&0xf;

    pixel_end = pixel_start+width;
    line_end = line_start+height;
    


    if(pixel_end>gtOsdCurrent.width || line_end>gtOsdCurrent.height){
        printf("\r\nVb2100FillColor:end position coordinate is overrun!");
        gOsdEnable=0;   
        return ;           
    }

    if(width==0 || height==0){
        printf("\r\nVb2100FillColor:width or height is error!");
        gOsdEnable=0;   
        return ;           
    }

    if(gtOsdCurrent.color_pallate!= (unsigned int *)color_index){
        gtOsdCurrent.color_pallate = (unsigned int *)color_index;
        Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);
    }
    
     switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

    }

    for(j=0;j<height;j++){
        x_pos = pixel_start;
        y_pos = line_start+j;
        if(pixel_start%2 != 0){
            x_pos --;

            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

            DisplayColor = *(volatile unsigned char*)(gtOsdCurrent.fill_addr+addr);
            FillColor = (color<<4) |(DisplayColor&0xf);
            
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
            x_pos += 2;
        }
        
        FillColor = color<<4 | color;
        for(i=0;i<width/2;i++){
            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
            x_pos += 2;
        }
        
        if(width%2 !=0){

            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

            DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
            FillColor = (DisplayColor&0xf0) | color;
            
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
        }
    }

}





VOID Vb2100DrawLine(unsigned char dev, unsigned char path, gtDrawLineSetting tDrawLineParameters)
{
    unsigned int start_x_pos, start_y_pos, end_x_pos, end_y_pos;
    unsigned char page, color;  

    unsigned int i, j, x_pos, y_pos;
    unsigned char FillColor, DisplayColor;
    unsigned int  addr;

    WORD line_length;

    OS_CPU_SR    cpu_sr;
    
    start_x_pos = tDrawLineParameters.start_x_pos;
    start_y_pos = tDrawLineParameters.start_y_pos;
    end_x_pos = tDrawLineParameters.end_x_pos;
    end_y_pos = tDrawLineParameters.end_y_pos;
    page = (tDrawLineParameters.attribute) & 0xf;
    color = ((tDrawLineParameters.attribute)>>4) & 0xf;

  
    if(start_x_pos>gtOsdCurrent.width|| start_y_pos>gtOsdCurrent.height){
        printf("\r\nVb2100DrawLine:start position coordinate is overrun!");
        gOsdEnable=0;  
        return ;            
    }
    if(end_x_pos>gtOsdCurrent.width || end_y_pos>gtOsdCurrent.height ){
        printf("\r\nVb2100DrawLine:end position coordinate is overrun!");
        gOsdEnable=0;  
        return ;            
    }

    if(gtOsdCurrent.color_pallate!= (unsigned int *)color_index){
        gtOsdCurrent.color_pallate = (unsigned int *)color_index;
        Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);
    }


    switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff|0xa0000000)-OSD_OFFSET;
            break;

    }

    if( end_x_pos == start_x_pos ){        
        printf("V!");        
        line_length = end_y_pos-start_y_pos;        

        if(start_x_pos%2 ==0){
            x_pos = start_x_pos;
            for(j=0;j<line_length;j++){
                y_pos = start_y_pos+j;
                addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

                DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
                FillColor = (DisplayColor&0xf0) | color;
                
                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
            }
        } else{
            x_pos = start_x_pos-1;
            for(j=0;j<line_length;j++){
                y_pos = start_y_pos+j;
                addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

                DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
                FillColor = (color<<4) |(DisplayColor&0xf);
                
                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
            }
        }

    } else {    
        printf("H!");
        
        y_pos = start_y_pos;
        x_pos = start_x_pos;
        line_length = end_x_pos-start_x_pos;
        
        if(start_x_pos%2 !=0){
            x_pos --;

            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

            DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
            FillColor = (color<<4) |(DisplayColor&0xf);
            
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
            x_pos += 2;
        }
        
        FillColor = color<<4 | color;
        for(i=0;i<line_length/2;i++){
            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
            x_pos += 2;
        }

        if(line_length%2 !=0){

            addr = (x_pos+y_pos*gtOsdCurrent.width)/2;

            DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
            FillColor = (DisplayColor&0xf0) | color;
            
            OS_ENTER_CRITICAL();
            *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
            OS_EXIT_CRITICAL(); 
        }
        
    }


}




VOID Vb2100ChangeOSDPage(unsigned char dev, unsigned char path, unsigned short page)
{
    unsigned int addr;

    unsigned char err;
    
    switch(page){
        case 0:
            addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
            break;

        case 1:
            addr = (((unsigned int)OSD_memory_1)&0x1fffffff);
            break;

        default:
            addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
            break;         
    }

    if(gtOsdCurrent.display_addr != addr){
        if((err = OSQPost(gptMqueueOSDFrame, addr)) != OS_NO_ERR) {
            debug("\nOSD input queue is full, err=%d!\n", err);
        }
    }
}

#if 0
VOID Vb2100WriteString(unsigned char dev, unsigned char path, gtWriteStringSetting tWriteStringParameters)
{
    unsigned int start_x, start_y, stop_x, stop_y;
    unsigned int *string_addr;
    unsigned int string_len;
    unsigned char string_color, page;

    unsigned int i, j, k;
    unsigned int x, y;
    unsigned int addr;

    unsigned char *str_addr;
    unsigned int string_ascii;
    unsigned short *string_data;
    unsigned short line_data;
    unsigned char FillColor, color1, color2, DisplayColor;

    WORD word_width, word_len;
    
    OS_CPU_SR    cpu_sr;

    tWriteStringParameters.string_lib_addr = string_tab;
    
    start_x = tWriteStringParameters.start_x;
    start_y = tWriteStringParameters.start_y;
    string_addr = tWriteStringParameters.string_addr;
    string_len = tWriteStringParameters.string_len;
    page = tWriteStringParameters.attribute&0xf;
    string_color = (tWriteStringParameters.attribute>>0x4)&0xf;

    word_width = 12;
    word_len = 16;
    stop_x = start_x+word_width*string_len;
    stop_y = start_y+word_len;
    
    str_addr = (unsigned char *) string_addr;


    if(stop_x> gtOsdCurrent.width|| stop_y> gtOsdCurrent.height){
        printf("\r\nVb2100WriteString:stop position coordinate is overrun!");
        gOsdEnable=0;   
        return ;         
    }

    if(string_len==0){
        printf("\r\nVb2100WriteString: string_len is error!");
        gOsdEnable=0;   
        return ;         
    }

    if(gtOsdCurrent.color_pallate!= (unsigned int *)color_index){
        gtOsdCurrent.color_pallate = (unsigned int *)color_index;
        Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);
    }

    switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff);
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff);
            break;
    }


    for(k=0; k<string_len; k++){
        string_ascii = *str_addr++; 
        string_data = (unsigned short *)(tWriteStringParameters.string_lib_addr+8*string_ascii);
        for(j=0; j<word_len; j++){
            //printf("*string_data=0x%x\n", *string_data);
            y = start_y+j;
            line_data = (*string_data>>4)&0xfff;
            x = start_x+word_width-2; 
            if(start_x%2 ==0){
                for(i=0; i<word_width/2; i++){
                    if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                    else color2 = string_color;
                    line_data >>= 1;   
                    if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask;
                    else color1 = string_color;
                    line_data >>= 1; 
                    FillColor = color2<<4 | color1;
                    addr = (x+y*gtOsdCurrent.width)/2;
                    OS_ENTER_CRITICAL();
                    *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr)=FillColor;
                    OS_EXIT_CRITICAL();
                    x -= 2;
                }
            } else{          
                x ++;
                addr = (x+y*gtOsdCurrent.width)/2;
                DisplayColor = *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr);
          
                if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask;
                else color1 = string_color;
                line_data >>= 1;                 
                FillColor = (DisplayColor&0xf0) | color1;

                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                x -= 2;

                for(i=0; i<word_width/2-1; i++){ 
                    if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                    else color2 = string_color;
                    line_data >>= 1;   
                    if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask;
                    else color1 = string_color;
                    line_data >>= 1; 
                    FillColor = color2<<4 | color1;
                    addr = (x+y*gtOsdCurrent.width)/2;
                    OS_ENTER_CRITICAL();
                    *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr)=FillColor;
                    OS_EXIT_CRITICAL();
                    x -=2;
                }
                
                addr = (x+y*gtOsdCurrent.width)/2;
                DisplayColor = *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr);
  
                if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                else color2 = string_color;
                FillColor = color2<<4 | (DisplayColor&0xf);

                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)(( gtOsdCurrent.fill_addr |0xa0000000)+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                               
            }
            string_data += 1;
        }
        start_x += word_width;        
    }
}
#else

VOID Vb2100WriteString(unsigned char dev, unsigned char path, gtWriteStringSetting tWriteStringParameters)
{
    unsigned int start_x, start_y, stop_x, stop_y;
    unsigned int *string_addr;
    unsigned int string_len;
    unsigned char string_color, page;

    unsigned int i, j, k;
    unsigned int x, y;
    unsigned int addr;

    unsigned char *str_addr;
    unsigned int string_ascii;
    unsigned int *string_data;
    unsigned int line_data;
    unsigned char FillColor, color1, color2, DisplayColor;

    WORD word_width, word_len;
    
    OS_CPU_SR    cpu_sr;

    tWriteStringParameters.string_lib_addr = font;
    
    start_x = tWriteStringParameters.start_x;
    start_y = tWriteStringParameters.start_y;
    string_addr = tWriteStringParameters.string_addr;
    string_len = tWriteStringParameters.string_len;
    page = tWriteStringParameters.attribute&0xf;
    string_color = (tWriteStringParameters.attribute>>0x4)&0xf;

    word_width = FONT_WIDTH;
    word_len = FONT_HEIGHT;
    stop_x = start_x+FONT_WIDTH*string_len;
    stop_y = start_y+FONT_HEIGHT;
    
    str_addr = (unsigned char *) string_addr;


    if(stop_x> gtOsdCurrent.width|| stop_y> gtOsdCurrent.height){
        printf("\r\nVb2100WriteString:stop position coordinate is overrun!");
        gOsdEnable=0;   
        return ;         
    }
  
    if(string_len==0){
        printf("\r\nVb2100WriteString: string_len is error!");
        gOsdEnable=0;   
        return ;         
    }

    if(gtOsdCurrent.color_pallate!= (unsigned int *)color_index){
        gtOsdCurrent.color_pallate = (unsigned int *)color_index;
        Vb2100FillOsdPT(VB2100_MASTER, VB2100_PATH, gtOsdCurrent.color_pallate);
    }

    switch(page){
        case 0:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff |0xa0000000)-OSD_OFFSET;
            break;
            
        case 1:
             gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_1)&0x1fffffff |0xa0000000)-OSD_OFFSET;
            break;

        default:
            gtOsdCurrent.fill_addr = (((unsigned int)OSD_memory_0)&0x1fffffff |0xa0000000)-OSD_OFFSET;
            break;
    }


    for(k=0; k<string_len; k++){
        string_ascii = *str_addr++; 
        if(string_ascii==0) string_data = tWriteStringParameters.string_lib_addr;
        else string_data = tWriteStringParameters.string_lib_addr+32*(string_ascii-32);
        for(j=0; j<word_len; j++){
            //printf("*string_data=0x%x\n", *string_data);
            line_data = (*string_data>>4)&0x00ffffff;
            y = start_y+j;
            x = start_x+word_width-2; 
            if(start_x%2 ==0){
                for(i=0; i<word_width/2; i++){
                    if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                    else color2 = string_color;
                    line_data >>= 1;   
                    if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask; 
                    else color1 = string_color;
                    line_data >>= 1; 
                    FillColor = color2<<4 | color1;
                    addr = (x+y*gtOsdCurrent.width)/2;
                    OS_ENTER_CRITICAL();
                    *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor;
                    OS_EXIT_CRITICAL();
                    x -= 2;
                }
            } else{          
                x ++;
                addr = (x+y*gtOsdCurrent.width)/2;
                DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
          
                if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask;
                else color1 = string_color;
                line_data >>= 1;                 
                FillColor = (DisplayColor&0xf0) | color1;

                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                x -= 2;

                for(i=0; i<word_width/2-1; i++){ 
                    if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                    else color2 = string_color;
                    line_data >>= 1;   
                    if((line_data&0x1) ==0) color1 = gtOsdCurrent.OSD_T_Mask;
                    else color1 = string_color;
                    line_data >>= 1; 
                    FillColor = color2<<4 | color1;
                    addr = (x+y*gtOsdCurrent.width)/2;
                    OS_ENTER_CRITICAL();
                    *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor;
                    OS_EXIT_CRITICAL();
                    x -=2;
                }
                
                addr = (x+y*gtOsdCurrent.width)/2;
                DisplayColor = *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr);
  
                if((line_data&0x1) ==0) color2 = gtOsdCurrent.OSD_T_Mask;
                else color2 = string_color;
                FillColor = color2<<4 | (DisplayColor&0xf);

                OS_ENTER_CRITICAL();
                *(volatile unsigned char*)( gtOsdCurrent.fill_addr+addr)=FillColor ;
                OS_EXIT_CRITICAL(); 
                               
            }
            string_data += 1;
        }
        start_x += word_width;        
    }
}

#endif
    

#endif

#endif
