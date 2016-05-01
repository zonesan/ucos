/*
***************************************************************************
*                  Copyright (c) 2005 vBridge Microsystem, Inc.  
*                        Unpublished & Not for Publication
*                               All Rights Reserved                   
*   
* File        : two_risc_display.c                     
*                                                                         
* Description : VSC, MPEG decode integration   
*             
* Date        : Oct 25, 2006
***************************************************************************
*/


#define   TWO_RISC_ROLLSTR_GLOBALS

#include "includes.h"

#include "video_cfg.h"

#ifdef OSD_ENABLE
#include "two_risc_rollstr.h"
#include "two_risc_display.h"
#include "VSC_API.h"



/*
***************************************************************************
*                         TaskRollString
*
* Description: 
*
* Arguments  : 
*
* Return     : 
*
* Note(s)    : 
***************************************************************************
*/
/*unsigned char ascii[65] = {33,34,35,36,37,38,39,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,
                                        82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,
                                        106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122};*/

void TaskRollString(void *data)
{
    gtWriteStringSetting tWriteStringParameters;
    gtClearScreenSetting tClearScreenParameters;
    unsigned char page, color;

    unsigned int RollStrCnt = 0;

    unsigned int i;
    unsigned int RollStrDisplayCnt, RollStrDisCnt;
    unsigned int xindex;
    unsigned int str_len, str_num, str_start_num;

    unsigned char *string_addr;

    BspStartTicker(OS_TICKS_PER_SEC);

    
    gtRollStr.char_width = FONT_WIDTH;
    gtRollStr.char_height = FONT_HEIGHT;  
    gtRollStr.start_pixel = 0;
    gtRollStr.start_line = gtOsdCurrent.height-gtRollStr.char_height-4;

    page = (gtRollStr.attribute>>0x1)&0x7;
    color = (gtRollStr.attribute>>0x4)&0xf;

    //gtRollStr.str_addr = (unsigned int*)ascii;
    //gtRollStr.str_len = sizeof(ascii);
    //color = 0;
    //page = 0;

    gtRollStr.str_width = gtRollStr.char_width*gtRollStr.str_len;
    gtRollStr.str_height = gtRollStr.char_height;

    RollStrDisplayCnt = gtOsdCurrent.width-gtRollStr.char_width-gtOsdCurrent.width%gtRollStr.char_width;
    RollStrDisCnt = RollStrDisplayCnt+gtRollStr.str_width-gtRollStr.char_width;  

    str_start_num = 0;
    string_addr = (unsigned char*)gtRollStr.str_addr;
    str_len = gtRollStr.str_len;


    while(1){

        xindex = RollStrCnt%RollStrDisCnt;
        
        if(xindex<gtRollStr.str_width) str_num = xindex/gtRollStr.char_width+1;

        if((str_start_num<str_len) && (xindex>=RollStrDisplayCnt)){
            if(xindex%gtRollStr.char_width==0) str_start_num++;
        } else{
            str_start_num = 0;
        }


        tWriteStringParameters.start_x = gtRollStr.start_pixel+RollStrDisplayCnt-xindex+gtRollStr.char_width*str_start_num;
        tWriteStringParameters.start_y = gtRollStr.start_line;
        tWriteStringParameters.string_addr = string_addr+str_start_num;
        tWriteStringParameters.string_len = str_num-str_start_num;
        tWriteStringParameters.attribute = color<<4 | page;
        
        if(str_num==1 || (str_start_num>0 && tWriteStringParameters.start_x==gtRollStr.char_width)){
            tClearScreenParameters.pixel_start = gtRollStr.start_pixel;
            tClearScreenParameters.line_start = gtRollStr.start_line;
            tClearScreenParameters.width = gtRollStr.char_width;
            tClearScreenParameters.height = gtRollStr.char_height;
            tClearScreenParameters.attribute = (unsigned int) page;
            Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters);
        }

          
        //printf("n=%d, s=%d,xindex=%d,start_x=%d\n",str_num,str_start_num,xindex,tWriteStringParameters.start_x);   
        
        Vb2100WriteString(VB2100_MASTER, VB2100_PATH, tWriteStringParameters);

        if(str_start_num>0 && tWriteStringParameters.string_len<gtOsdCurrent.width/gtRollStr.char_width-1){
            tClearScreenParameters.pixel_start = tWriteStringParameters.start_x+gtRollStr.char_width*tWriteStringParameters.string_len;
            tClearScreenParameters.line_start = gtRollStr.start_line;
            tClearScreenParameters.width = gtRollStr.char_width;
            tClearScreenParameters.height = gtRollStr.char_height;
            tClearScreenParameters.attribute = (unsigned int) page;
            Vb2100ClearScreen(VB2100_MASTER, VB2100_PATH, tClearScreenParameters);
        }
                
        RollStrCnt++;
        
        OSTimeDlyHMSM (0, 0, 0, 100);
    }

}

#endif
