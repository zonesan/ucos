#include    "hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ds_scrn.h"
#include "wndw_mtn_md.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "wndw_rc_fmt.h"


#include    "isr.h"
#include    "hst.h"
#include    "key.h"
#include    "wndw_rc_fmt_ls.h"
#include    "wndw_mtn_bd_nd.h"
#include    "wndw_ds_src.h"
#include    "wndw_ds_indx.h"
#include    "wndw_zm.h"
#include    "wndw_ado_gain.h"
#include    "wndw_ado_live.h"
#include    "wndw_ado_rec.h"
#include    "wndw_rg.h"
#include    "wndw_rc_indx.h"
#include    "wndw_rc_nr.h"
#include    "mse_free.h"
#include    "run.h"
#include    "gpio.h"
#include        "tbl_tw2835.h"

#include        "tw2835_api.h"
#include        "hw2835_md.h"

#include "regs.h"
#include <comm.h>

//#include "tbl_osd_4.h"
//#include "../application/video/techwell/bmpMainMenu.h"

U8  OUTMODEFLAG = 0;
int gChipId;//for judge
unsigned char  gVideoMotionEn=0;
unsigned char  gVideoLossEn=0;
unsigned char  gVideoBlindEn=0;
unsigned char  gVideoNightEn=0;

#ifdef Hw2835_16Channel
//extern    void SetMainTst(void);
#define TW2835_CS0  (0x1 << 4)
#define TW2835_CS1  (0x1 << 5)
#define TW2835_CS2  (0x1 << 6)
#define TW2835_CS3  (0x1 << 7)
#endif


#define TW2835_RST  (0x1 << 2)
#define TW2835_HSPB (0x1 << 0)

#define TW2835_IIC_ADDR 0x7e
#ifdef Hw2835_16Channel   
void tw2835_io_init(void)
{
    int temp;
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x84);
 
    temp &= ~(TW2835_CS0 | TW2835_CS1 | TW2835_CS2 | TW2835_CS3| TW2835_RST | TW2835_HSPB);


    *(volatile unsigned *)(RISC_BASE_ADDR + 0x84) = temp;
    
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp |= TW2835_HSPB;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

void tw2835_reset()
{
    int temp;

    set_gpio_out(GPIO_I2C_SCL_BIT);
    
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp &= ~TW2835_RST;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
    TW_IICDelay(200000);
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    temp |= TW2835_RST;
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

int select_chip(unsigned int cs)
{
    unsigned int temp;
    temp = *(volatile unsigned *)(RISC_BASE_ADDR + 0x80);
    if(cs > 3)
        return -1;
    temp |= (0xf << 4);
    temp &= ~(0x1 << (4 + cs));
    *(volatile unsigned *)(RISC_BASE_ADDR + 0x80) = temp;
}

#endif
//==================================================================================
//==================================================================================

void HwTW2835Init_CZH(U8 _pth, U8 VinModeControl, U8 VoutModeControl, U8 VinCH)
{
    U8 _t1_=0;
    U8 _t2_=0x1;
    U8 _t3_=0;
    U8 _dvc_, _ch_;
    U16 _dly_;


    
#ifdef Hw2835_16Channel
    tw2835_io_init();
//#endif
    tw2835_reset();
    TW_IICDelay(200000);

//#ifdef Hw2835_16Channel    
    select_chip(0);
#endif    

#ifdef Hw2835_mouse
   InitIsr();
#endif  


    InitVdo(VinModeControl);
#ifdef Hw2835_reset
    InitRstN();
#endif
    //  test_I2C(0);

    WriteAsicTable(MASTER,DVC_PG1,0xa0,tbl_ntsc_pg1_enc,16);

    InitPg0();
    InitPg1();

    InitPg2();

    //  InitReg();

    SetOSDNoMn();

    //... initial table
    for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
#if defined(__4CH__)
        SetChQd(PTH_X,_dvc_,_ch_,_ch_);
#elif defined(__16CH__)
        SetChPIP(PTH_X,_dvc_,_ch_,_t1_);
#endif  	
    }


#if defined(__WTH_VGA__)
    MDIN180Init();
#endif

#ifdef Hw2835_mouse
    InitMse();
    DLY_FOR(30000);
    DLY_FOR(30000);

    WriteMse(0xeb);
#endif
    if(BitSet(_pth,PTH_X))  WriteAsicByte(MASTER,DVC_PG1,0xa0,0x56);

    if(VoutModeControl == 0){
        OUTMODEFLAG = 0;
        if(BitSet(_pth,PTH_X))  SetChFl(PTH_X,MASTER,VinCH);
        if(BitSet(_pth,PTH_Y))  SetChFl(PTH_Y,MASTER,VinCH);
    }
    else{
    OUTMODEFLAG = 1;
    }

    //SetChFl(PTH_Y,MASTER,VinCH);
    

    WriteAsicByte(MASTER,DVC_PG1,0xa3,0x11);
    WriteAsicByte(MASTER,DVC_PG1,0xad,0x80);
    WriteAsicByte(MASTER,DVC_PG1,0xac,0x80);


    WriteAsicByte(MASTER,DVC_PG1,0xb0,0x11);
    WriteAsicByte(MASTER,DVC_PG1,0xb3,0x34);
    WriteAsicByte(MASTER,DVC_PG1,0xb5,0x56);
    WriteAsicByte(MASTER,DVC_PG1,0xae,0x8a);
    WriteAsicByte(MASTER,DVC_PG1,0xaf,0x8a);
    WriteAsicByte(MASTER,DVC_PG0,0x41,0xf7);
    
 
}



int D1_ModeFlag = 0;
void HwTW2835SysSetting(U8 _pth, U8 VinModeControl, U8 VoutModeControl, U8 VinCH)
{
    int ChipID;
       
    SetAsicFlgType(cmn_dvc,DVC_PG2,0x80,0x80,0);//enable MD ,blind detect,night detect 
    SetAsicFlgType(cmn_dvc,DVC_PG2,0xa0,0x80,0);
    SetAsicFlgType(cmn_dvc,DVC_PG2,0xc0,0x80,0);
    SetAsicFlgType(cmn_dvc,DVC_PG2,0xe0,0x80,0);   

    WriteAsicByte(cmn_dvc,DVC_PG1,0xa3,0x01);
    WriteAsicByte(cmn_dvc,DVC_PG1,0xad,0x80);
    WriteAsicByte(cmn_dvc,DVC_PG1,0xa0,0x53);
    WriteAsicByte(cmn_dvc, DVC_PG2, 0x38, 0);
    
//======SET BG COLOR=====by VINCENT 2007.11.07=========================
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x0f,BIT0|BIT1,BIT0);

    SetAsicFlgType(cmn_dvc,DVC_PG1,0x5f,BIT0|BIT1,0);
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x5f,BIT2|BIT3,0);
// =======set S-Video=====BY VINCENT 2007.11.13=========================
    WriteAsicByte(cmn_dvc, DVC_PG1, 0xA1, 0x32);
//=====set manual corp============================================
    SetAsicFlgType(cmn_dvc, DVC_PG0, 0xC0, 0x08,0x08);
//=======MPP(D1) output setting   2007.11.21================================
  if(VoutModeControl ==2)
    {
#if 1
        WriteAsicByte(cmn_dvc,DVC_PG1,0xb0,0x2a);
        WriteAsicByte(cmn_dvc,DVC_PG1,0xb3,0x34);
        WriteAsicByte(cmn_dvc,DVC_PG1,0xb5,0x56);
        WriteAsicByte(cmn_dvc,DVC_PG1,0xae,0x8a);
        WriteAsicByte(cmn_dvc,DVC_PG1,0xaf,0x8a);
        WriteAsicByte(cmn_dvc,DVC_PG0,0x41,0x77);
        
        SetAsicFlgType(cmn_dvc,DVC_PG1,0xAC,0x0F,0x07);//Control the ITU656 clock delay
        D1_ModeFlag =1;
        debug("\n Enable MPP (D1) mode\n");
#endif
    }
  else
    D1_ModeFlag =0;
//======SET alpha blending to 75%=====by VINCENT 2007.12.20==================    
    SetAsicFlgType(cmn_dvc,DVC_PG2,0x1f,0x3f,0xaa);
    debug("\n 1xac = 0x%x\n",ReadAsicByte(cmn_dvc,DVC_PG1,0xac));
    debug("\n 0x00 = 0x%x\n",ReadAsicByte(cmn_dvc,DVC_PG0,0x00));
    debug("\n 2x1f = 0x%x\n",ReadAsicByte(cmn_dvc,DVC_PG2,0x1f));
//======set the register for 16 channel live mode----by vincent 2008.01.23==============
    ChipID = HwGetVB2100ChipID();
    debug("\n VB2100ChipID = %x\n",ChipID);
    if(VoutModeControl==3)//16 channel mode--4chip
    {
        switch(ChipID)
        {
              case 0:
                  WriteAsicByte(cmn_dvc,1,0,0x88);
                  WriteAsicByte(cmn_dvc,1,0x7e,0xc9);
                  break;      
              case 1: 
                  WriteAsicByte(cmn_dvc,1,0,0x89);
                  WriteAsicByte(cmn_dvc,1,0x7e,0xA9);       
                  break; 
              case 2:
                  WriteAsicByte(cmn_dvc,1,0,0x8a);
                  WriteAsicByte(cmn_dvc,1,0x7e,0x89);
                  break;
              case 3:
                  WriteAsicByte(cmn_dvc,1,0,0xbb);
                  WriteAsicByte(cmn_dvc,1,0x7e,0x89);
                  break;
              default:
                  debug("\nChipID err %\n",ChipID);
        }
    }
 
//----------------init gpio interrupt----vincent 2008.01.31--------------------------------------
     set_gpio_in(GPIO_MOYION_DETECTOR_BIT);
     HW2835MotionDetectorInit(); 
     WriteAsicByte(cmn_dvc,DVC_PG1,0x76, 0);//set gpio IRQ  polarity & mode.
     
//----------------------disable boundary---vincent 2008.03.04----------------------------
     SetAsicFlgType(MASTER,DVC_PG1,0x11,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x19,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x21,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x29,BIT1,0);
        //diable record path boundary
     SetAsicFlgType(MASTER,DVC_PG1,0x61,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x64,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x67,BIT1,0);
     SetAsicFlgType(MASTER,DVC_PG1,0x6a,BIT1,0);
/*-----------------------------don't display mouse------------------------------------------------*/
     SetAsicFlgType(MASTER,DVC_PG2,0x10,BIT7|BIT6,0);
}

void HwTW2835Init(U8 _pth, U8 VinModeControl, U8 VoutModeControl, U8 VinCH)
{
    U8 _t1_=0;
    U8 _t2_=0x1;
    U8 _t3_=0;
    U8 _dvc_, _ch_;
    U16 _dly_;


    
#ifdef Hw2835_16Channel
    tw2835_io_init();
//#endif
    tw2835_reset();
    TW_IICDelay(200000);

//#ifdef Hw2835_16Channel    
    select_chip(0);
#endif    

#ifdef Hw2835_mouse
   InitIsr();
#endif  


    InitVdo(VinModeControl);
    if(VinModeControl)
        debug("\nTW2835 is PAL\n");
    else
        debug("\nTW2835 is NTSC\n");
#ifdef Hw2835_reset
    InitRstN();
#endif
    gChipId = HwGetVB2100ChipID();
    switch(gChipId)
    {
        case 0: cmn_dvc = 1;break;  //master
        case 1: cmn_dvc = 3;break;  //master
        case 2: cmn_dvc = 7;break;  //master
        case 3: cmn_dvc = 0xf;break;  //master
        default: cmn_dvc = 1;break;  //master
    }
    
    printf("\ngChipId:%d\n SlaveAddr:%d \n ",gChipId,cmn_dvc);

    WriteAsicTable(cmn_dvc,DVC_PG1,0xa0,tbl_ntsc_pg1_enc,16);
      
    InitPg0();
    InitPg1();

    InitPg2();

    SetOSDNoMn();

#if defined(__WTH_VGA__)
    MDIN180Init();
#endif

#ifdef Hw2835_mouse
    InitMse();
    DLY_FOR(30000);
    DLY_FOR(30000);

    WriteMse(0xeb);
#endif
 //   if(BitSet(_pth,PTH_X))  WriteAsicByte(cmn_dvc,DVC_PG1,0xa0,0x56);//set one time is enough--vincent 08.04.08

 //Init output mode!
    if(BitSet(_pth,PTH_X)) HwTw2835SetOutputMode_X(VoutModeControl,VinCH);
    if(BitSet(_pth,PTH_Y)) HwTw2835SetOutputMode_Y(VoutModeControl,VinCH);
 //===========BY VINCENT 2007.10.20============================== 
 
      HwTW2835SysSetting( _pth,  VinModeControl,  VoutModeControl,  VinCH);
}


#define wg_max_ch 4

void    HwTw2835ChangeOutputDisplay(U8 Ch_ModeCon, U8 Ch_lv_Num)
{
    U8 _dvc_, _ch_;
    U8 _t1_;//, _t2_=0;
//  U16 _dly_;

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x10);         //... FUNC_MODE=1
    }


    WaitChStrbDoneX(0xffff);            //... check for four channels of each devices

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,0x00);         //... POP_UP_EN=0
    }

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        if(Ch_ModeCon == SC_FMT_1CH){
                  OUTMODEFLAG = 0;
            if(_t1_ == Ch_lv_Num)   SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);         //... POP_UP_EN=1
            SetChFl(PTH_X,_dvc_,_ch_);
    	}
        else if(Ch_ModeCon == SC_FMT_4CH){
                  OUTMODEFLAG = 1;
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6); //... POP_UP_EN=1
            //if(b_ds_wth_4pb)  SetChNine(PTH_X,_dvc_,_ch_,_ch_);
            //else	
            SetChQd(PTH_X,_dvc_,_ch_,_ch_);
    	}
            else if(Ch_ModeCon == SC_FMT_8CH){
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6); //... POP_UP_EN=1
            //if(b_ds_wth_4pb)  SetChNine(PTH_X,_dvc_,_ch_,_ch_);
            //else	
            SetChNine(PTH_X,_dvc_,_ch_,_ch_);
    	}
//#if   defined(__4CH__)
//      else if(wndw_ds_sc_fmt == SC_FMT_4CH_PB){
//          SetChNine(PTH_X,_dvc_,_ch_,_ch_);
//      }
//#elif defined(__16CH__)

        else if(Ch_ModeCon == SC_FMT_16CH){
            //if(b_ds_wth_16pb){
            //  if(_t1_ < 14)   SetCh36(PTH_X,_dvc_,_ch_,_t1_);
            //  else        SetCh36(PTH_X,_dvc_,_ch_,_t1_+2);
        	//}
            //else	
            SetChPIP(PTH_X,_dvc_,_ch_,_t1_);
    	}

    }

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x00);         //... FUNC_MODE=0
    }
}

int FindChannelDiff(U8 *pos)
{
    int i,temp =0;

    for(i=0;i<wg_max_ch;i++)
    {
        if((temp&(1<<pos[i]))==0)
            temp |= (1<<pos[i]);
        else
            break;
    }
    if(i<wg_max_ch)
        return 1;//have same channel
    else
        return 0;//all 4 different channel
}

//===================ADD BY VINCENT 2007.10.19================================
void    HwTw2835SetOutputMode_X(U8 Ch_ModeCon, U8 Ch_lv_Num)
{
    U8 _dvc_, _ch_;
    U8 _t1_,pos[4],chan[4];

     for(_t1_=0;_t1_<wg_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x10);			//... FUNC_MODE=1
	}


	WaitChStrbDoneX(0xffff);			//... check for four channels of each devices
	
    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,0x00);         //... POP_UP_EN=0
    }

    if(Ch_ModeCon == SC_FMT_4CH)
    {
        pos[0] = Ch_lv_Num&0x03;
        pos[1] = (Ch_lv_Num&0x0c)>>2;
        pos[2] = (Ch_lv_Num&0x30)>>4;
        pos[3] = (Ch_lv_Num&0xc0)>>6;
        if((Ch_lv_Num==0)||( FindChannelDiff(pos)))
        {
            chan[0] = 0; chan[1] = 1; 
            chan[2] = 2; chan[3] = 3;
        }
        else
        {
            int i,j;
            for(i=0;i<4;i++)
            {
                for(j=0;j<4;j++)
                     if(i ==pos[j])
                        break;
                chan[i] = j;
            }
        }
       
    }
    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        if(Ch_ModeCon == SC_FMT_1CH){
                  OUTMODEFLAG = 0;
            if(_t1_ == Ch_lv_Num)   SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);         //... POP_UP_EN=1
            SetChFl(PTH_X,_dvc_,_ch_);
    	}
        else if(Ch_ModeCon == SC_FMT_4CH){
            
            OUTMODEFLAG = 1;
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6); //... POP_UP_EN=1
            SetChQd(PTH_X,_dvc_,_ch_,chan[_ch_]);

    	}
            else if(Ch_ModeCon == SC_FMT_8CH){
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6); //... POP_UP_EN=1
            SetChNine(PTH_X,_dvc_,_ch_,_ch_);
    	}


        else if(Ch_ModeCon == SC_FMT_16CH){
            SetChPIP(PTH_X,_dvc_,_ch_,_t1_);
    	}
    }

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30|BIT2,0x00|0);         //... FUNC_MODE=0
    }
     SetAsicFlgType(_dvc_,DVC_PG1,0x10+(0<<3),0x3,0x00);
     SetAsicFlgType(_dvc_,DVC_PG1,0x10+(1<<3),0x3,0x01);
     SetAsicFlgType(_dvc_,DVC_PG1,0x10+(2<<3),0x3,0x02);
     SetAsicFlgType(_dvc_,DVC_PG1,0x10+(3<<3),0x3,0x03);
}


void    HwTw2835SetOutputMode_Y(U8 Ch_ModeCon, U8 Ch_lv_Num)
{
    U8 _dvc_, _ch_;
    U8 _t1_;//, _t2_=0;

     for(_t1_=0;_t1_<wg_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x10);			//... FUNC_MODE=1
	}


	WaitChStrbDoneX(0xffff);			//... check for four channels of each devices
	
    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(_ch_*3),BIT6,0x00);          //... POP_UP_EN=0
    }

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        if(Ch_ModeCon == SC_FMT_1CH){
                  OUTMODEFLAG = 0;
            if(_t1_ == Ch_lv_Num)   SetAsicFlgType(_dvc_,DVC_PG1,0x60+(_ch_*3),BIT6,BIT6);          //... POP_UP_EN=1
            SetChFl(PTH_Y,_dvc_,_ch_);
    	}
        else if(Ch_ModeCon == SC_FMT_4CH){
                  OUTMODEFLAG = 1;
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x60+(_ch_*3),BIT6,BIT6);  //... POP_UP_EN=1
            SetChQd(PTH_Y,_dvc_,_ch_,_ch_);            
    	}
            else if(Ch_ModeCon == SC_FMT_8CH){
            if((_t1_&0x0c) == (Ch_lv_Num&0x0c)) SetAsicFlgType(_dvc_,DVC_PG1,0x60+(_ch_*3),BIT6,BIT6);  //... POP_UP_EN=1
            SetChNine(PTH_Y,_dvc_,_ch_,_ch_);
    	}
        else if(Ch_ModeCon == SC_FMT_16CH){
            SetChPIP(PTH_Y,_dvc_,_ch_,_t1_);
    	}

    }

    for(_t1_=0;_t1_<wg_max_ch;_t1_++){
        SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(_ch_*3),0x30,0x00);          //... FUNC_MODE=0
    }
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(0*3),0x3,0x00);
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(1*3),0x3,0x01);
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(2*3),0x3,0x02);
        SetAsicFlgType(_dvc_,DVC_PG1,0x60+(3*3),0x3,0x03);
       
}

void    HwTw2835SetOutputMode_PB(U8 Ch_ModeCon, U8 Ch_lv_Num)
{
    U8 _dvc_, _ch_,_t1_;
    U8 vertical_offset,reg_addr[3];
    extern int b_cmn_jp_vdo;
    
    for(_t1_=0;_t1_<wg_max_ch;_t1_++)
    {
	SetCrntDvcCh(_t1_,&_dvc_,&_ch_);	
    }
    
    if(Ch_ModeCon == SC_FMT_4CH)
    {
        for(_t1_=0;_t1_<wg_max_ch;_t1_++)
        {
            
            
           if(b_cmn_jp_vdo == VDO_PAL)
                vertical_offset = 72;
            else
                vertical_offset = 60;
            reg_addr[0] = 0x8b + (_t1_<<4);
            reg_addr[1] = 0x8d + (_t1_<<4);
            reg_addr[2] = 0x8f + (_t1_<<4);
            
             
            WriteAsicByte(MASTER, DVC_PG0, reg_addr[0] , 0);  
            WriteAsicByte(MASTER, DVC_PG0, reg_addr[1] , 0);
            SetAsicFlgType(MASTER, DVC_PG0, reg_addr[2] , 0x1,0x0);   
             
            WriteAsicByte(MASTER,DVC_PG0,0x86+_t1_*0x10,0x7f);   
            WriteAsicByte(MASTER,DVC_PG0,0x88+_t1_*0x10,0x7f);
        }
        SetAsicFlgType(_dvc_,DVC_PG1,0x30,BIT3,BIT3);
        SetAsicFlgType(_dvc_,DVC_PG1,0x38,BIT3,BIT3);
        WriteAsicByte(_dvc_,DVC_PG1,0x33,0x46);
        WriteAsicByte(_dvc_,DVC_PG1,0x37,0x46);
         WriteAsicByte(_dvc_,DVC_PG1,0x3A,0x46);
        WriteAsicByte(_dvc_,DVC_PG1,0x3E,0x46);
        
        WriteAsicByte(MASTER, DVC_PG0, 0x9b , 0x10); 
        WriteAsicByte(MASTER, DVC_PG0, 0xBb , 0x10); 
        WriteAsicByte(MASTER, DVC_PG0, 0xAD , 0xC); 
        WriteAsicByte(MASTER, DVC_PG0, 0xBD , 0xC); 

        
        SetAsicFlgType(_dvc_,DVC_PG1,0x11,BIT1,BIT1);
        SetAsicFlgType(_dvc_,DVC_PG1,0x19,BIT1,BIT1);
        SetAsicFlgType(_dvc_,DVC_PG1,0x21,BIT1,BIT1);
        SetAsicFlgType(_dvc_,DVC_PG1,0x29,BIT1,BIT1);
    }
    else  if(Ch_ModeCon == SC_FMT_1CH)
    {
        for(_t1_=0;_t1_<wg_max_ch;_t1_++)
        {
         
                WriteAsicByte(MASTER,DVC_PG0,0x86+_t1_*0x10,0xff);   
                WriteAsicByte(MASTER,DVC_PG0,0x88+_t1_*0x10,0xff);   
           
            if(b_cmn_jp_vdo == VDO_PAL)
                vertical_offset = 144;
            else
                vertical_offset = 120;
            reg_addr[0] = 0x8b + (_t1_<<4);
            reg_addr[1] = 0x8d + (_t1_<<4);
            reg_addr[2] = 0x8f + (_t1_<<4);
            
            switch(_t1_){ //Channel0 --->channel0;Channel1 --->channel1 ...
               case 0:               
                   break;              
               case 1:
                   WriteAsicByte(MASTER, DVC_PG0, reg_addr[0] , 0x68);   
                   SetAsicFlgType(MASTER, DVC_PG0, reg_addr[2] , 0x1,0x01);                    
                   break;                    
               case 2:                   
                   WriteAsicByte(MASTER, DVC_PG0, reg_addr[1] , vertical_offset); 
                   break;     
               case 3:
                   WriteAsicByte(MASTER, DVC_PG0, reg_addr[0] , 0x68);                   
                   WriteAsicByte(MASTER, DVC_PG0, reg_addr[1] , vertical_offset); 
                   SetAsicFlgType(MASTER, DVC_PG0, reg_addr[2] , 0x1,0x01);                    
                   break;    
               default:    
                   printf("\nTechWell: playback InputChannel error!\n");
                   break;
          }               
        }
        SetAsicFlgType(_dvc_,DVC_PG1,0x30,BIT3,0);
        SetAsicFlgType(_dvc_,DVC_PG1,0x38,BIT3,0);
        WriteAsicByte(_dvc_,DVC_PG1,0x33,0x48);
        WriteAsicByte(_dvc_,DVC_PG1,0x37,0x48);
         WriteAsicByte(_dvc_,DVC_PG1,0x3A,0x48);
        WriteAsicByte(_dvc_,DVC_PG1,0x3E,0x48);
        
        SetAsicFlgType(_dvc_,DVC_PG1,0x11,BIT1,0);
        SetAsicFlgType(_dvc_,DVC_PG1,0x19,BIT1,0);
        SetAsicFlgType(_dvc_,DVC_PG1,0x21,BIT1,0);
        SetAsicFlgType(_dvc_,DVC_PG1,0x29,BIT1,0);
    }
}
//===================ADD BY VINCENT 2007.10.19================================


void SetOSDWndwX(U8 _dvc, U8 _pth, U8 _box_type, U16 _osd_pos_x, U16 _osd_pos_y, U16 _width, U16 _hght, U8 _color, U8 _attribute)
{
    U8 _hl_,_hr_;
    U16 _vt_,_vb_;

    _hl_ = _osd_pos_x>>2;
    _hr_ = _hl_ + (_width>>2);
    _vt_ = _osd_pos_y>>1;       				
    _vb_ = _vt_ +(_hght>>1);

    WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_, _attribute, _color);


}

void SetOSDWndwY(U8 _dvc, U8 _pth, U8 _box_type, U16 _osd_pos_x, U16 _osd_pos_y, U16 _lngth, U16 _hght, U8 _color, U8 _attribute)
{
    U8 _hl_,_hr_;
    U16 _vt_,_vb_;

    _hl_ = _osd_pos_x>>3;//3;               		//... hl
    _hr_ = _hl_ + (_lngth>>3);//5;              	//... hr
    _vt_ = _osd_pos_y>>1;               		//... vt
    _vb_ = _vt_ +(_hght>>1);//10;               	//... vb

    //printf("\n_hl_ = %d, _hr_ = %d, _vt_ = %d, _vb_ = %d, _attribute = 0x%x !\n", _hl_, _hr_, _vt_, _vb_, _attribute);
    //... main frame
    WriteOSDSqrY(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_,_attribute,_color);

}

void SetOSDClrWndwX(U8 _dvc, U8 _pth, U16 _osd_pos_x, U16 _osd_pos_y, U16 _lngth, U16 _hght, U8 _page)
{
    U8 _hl_,_hr_;
    U16 _vt_,_vb_;
    U8 i,k,t[4];
    
    _hl_ = _osd_pos_x;
    _hr_ = _hl_ + _lngth;
    _vt_ = _osd_pos_y;      				
    _vb_ = _vt_ +_hght;

    //printf("\n_hl = %d, _hr = %d, _vt = %d, _vb = %d, _page = %d !\n", _hl_, _hr_, _vt_, _vb_, _page );
    while(1)
    {
	    OSDClrX(_dvc, _hl_, _hr_, OSD_POS_V_ODD|_vt_, OSD_POS_V_EVEN|_vb_, _page);

            SetAsicFlgType(_dvc,DVC_PG2,0x4,BIT6,BIT6);
	     if(_page&0x80)
	           k =0x80;
	       else
	           k =0xff;
	    for(i=0;i<4;i++)
	        t[i] =ReadAsicByte(_dvc,DVC_PG2,i);
	    if((t[0]==k)&(t[1]==k)&(t[2]==k)&(t[3]==k))
	        break;
	    printf("\n RE-WRITE BMP \n");
}
}

void SetOSDClrWndwY(U8 _dvc, U8 _pth,  U16 _osd_pos_x, U16 _osd_pos_y, U16 _lngth, U16 _hght, U8 _page)
{
    U8 _hl_,_hr_;
    U16 _vt_,_vb_;

    _hl_ = _osd_pos_x;
    _hr_ = _hl_ + _lngth;
    _vt_ = _osd_pos_y;      				
    _vb_ = _vt_ +_hght;
    
    OSDClrY(_dvc, _hl_, _hr_, OSD_POS_V_ODD|_vt_, OSD_POS_V_EVEN|_vb_, _page);


}

void    HwTW2835Clr(U8 _dvc, U8 _pth, TW2835Clr * ClrScreePara)
{


    U16 _osd_pos_x, _osd_pos_y;
    U16 width, higth;
    U8 _page;
    
    _osd_pos_x = ClrScreePara->start_pos_x >> 2;
    width = ClrScreePara->width >> 2;
    _osd_pos_y = ClrScreePara->start_pos_y >> 1;
    higth = ClrScreePara->higth >> 1;
    _page = ClrScreePara->page;
    
    if(BitSet(_pth,PTH_Y)){
        _osd_pos_x = _osd_pos_x >> 1;
        width = width >> 1;
        }
    
    if(_osd_pos_x > 180){
        _osd_pos_x = 179;        
        }

    if((_osd_pos_x + width) > 180){
        width = 179 - _osd_pos_x;
        if(_osd_pos_x == 179) width = 0;        
        }

    if(_osd_pos_y > 288){
        _osd_pos_y = 287;        
        }    

    if((_osd_pos_y + higth) > 288){
        higth = 288 - _osd_pos_y;
        if(_osd_pos_y == 287) higth = 0; 
        }
    //printf("\n_osd_pos_x = %d, _osd_pos_y = %d, width = %d, higth = %d !\n", _osd_pos_x, _osd_pos_y, width, higth );
    if(BitSet(_pth,PTH_X))  SetOSDClrWndwX(_dvc, _pth,_osd_pos_x,_osd_pos_y,width,higth, _page);
    if(BitSet(_pth,PTH_Y)){
        if((_page != 0)&(_page != 1)) printf("/n OSD Record page is only 0 or 1 !/n");
        else        SetOSDClrWndwY(_dvc, _pth,_osd_pos_x,_osd_pos_y,width,higth, _page);
        }
    

}


void    HwTW2835FillColor(U8 _dvc, U8 _pth, TW2835_FillColor * tFillColorParameter)
{

    U8 _attribute; 
    U16 _osd_pos_x, _osd_pos_y, width, higth;
    U8 _color, page;

    _osd_pos_x = tFillColorParameter->start_x;
    _osd_pos_y = tFillColorParameter->start_y;
    width = tFillColorParameter->width;
    higth = tFillColorParameter->heigth;
    page = (tFillColorParameter->attribute) & 0x07;
    _color = ((tFillColorParameter->attribute)>>4) & 0xff;

    _attribute = ((tFillColorParameter->attribute)>>10) & 0xc0;
    _attribute = _attribute | page;

    //printf("\n_osd_pos_x = %d, _osd_pos_y = %d, width = %d, higth = %d, _attribute = 0x%x !\n", _osd_pos_x, _osd_pos_y, width, higth, _attribute);

    if(_osd_pos_x  > 704){
        printf("X Start Position Coordinate is very large! ");      
        }    
    if((_osd_pos_x+width) > 704){
        printf("width is very large! ");        
        }

    if(_osd_pos_y > 576){
        printf("Y Start Position Coordinate is very large! ");        
        }    

    if((_osd_pos_y + higth) > 576){
        printf("higth is very large! "); 
        }
    if(BitSet(_pth,PTH_X))  SetOSDWndwX(_dvc, _pth,OSD_WNDW_TYPE_MAIN,_osd_pos_x,_osd_pos_y,width,higth,_color, _attribute);
    if(BitSet(_pth,PTH_Y))  SetOSDWndwY(_dvc, _pth,OSD_WNDW_TYPE_MAIN,_osd_pos_x,_osd_pos_y,width,higth,_color, _attribute);
    

}



void    HwTW2835CreateLn(U8 _dvc, U8 _pth, TW2835_Line * tDrawLineParameter)
{
    U16 start_pos_x, start_pos_y, end_pos_x, end_pos_y;
    U8 _color, pos_control;
    U8 x_pos, y_pos;
    U8 line_attribute;
    U8 page;
    U8 line_control_1, line_control_2;
    U16 start_x, start_y, end_x, end_y, width;
    unsigned short temp = 0x00ff;;
    U8 start_x_remainder, start_y_remainder, end_x_remainder, end_y_remainder;


    start_pos_x = tDrawLineParameter->start_pos_x;
    start_pos_y = tDrawLineParameter->start_pos_y;
    end_pos_x = tDrawLineParameter->end_pos_x;
    end_pos_y = tDrawLineParameter->end_pos_y;
    _color = ((tDrawLineParameter->attribute)>>4) & 0xff;
    page = (tDrawLineParameter->attribute) & 0x07;
    line_attribute = ((tDrawLineParameter->attribute)>>10) & 0xc0;
    line_attribute = line_attribute | page;

    if(start_pos_x > 705){
        printf("X Start Position Coordinate is very large! ");        
        }
    if(end_pos_x > 705){
        printf("X End Position Coordinate is very large! ");        
        }
    
    if(start_pos_y > 577){
        printf("Y Start Position Coordinate is very large! ");        
        }
    if(end_pos_y > 577){
        printf("Y End Position Coordinate is very large! ");        
        }   
    //printf("\nstart_pos_x = %d, start_pos_y = %d, end_pos_x = %d, end_pos_y = %d !\n", start_pos_x, start_pos_y, end_pos_x, end_pos_y);
    start_x = start_pos_x >> 2;
    end_x = end_pos_x >> 2;
    start_y = start_pos_y >> 1;
    end_y = end_pos_y >> 1;
    
    start_x_remainder = (U8)(start_pos_x & 0x03);
    end_x_remainder = (U8)(end_pos_x & 0x03);
    if(end_pos_x < start_pos_x){
        end_x = start_pos_x >> 2;
        start_x = end_pos_x >> 2; 
        }
    //printf("\nstart_x = %d, start_y = %d, end_x = %d, end_y = %d !\n", start_x, start_y, end_x, end_y);
    
    //if(BitSet(_pth,PTH_X)) line_attribute = 0;
    if(BitSet(_pth,PTH_Y)) {
        //line_attribute = 0;
        start_x = start_pos_x >> 3;
        end_x = end_pos_x >> 3;
        }
        
    
    if((start_pos_y > (end_pos_y + 4)) || (end_pos_y > (start_pos_y + 4))){
        if(start_x == end_x) {
            line_control_1 = ((temp >> start_x_remainder)>>(8-end_x_remainder))<<(4-end_x_remainder);
            line_control_2 = 0;
            }
        else{
            line_control_1 = (((temp >> start_x_remainder)>>(4-end_x_remainder))<<(4-end_x_remainder))>>4;
            line_control_2 = (((temp >> start_x_remainder)>>(4-end_x_remainder))<<(4-end_x_remainder))&0x0f;
            }
        printf("\nstart_x = %d, end_x = %d, start_x_remainder = %d, end_x_remainder = %d, line_control_1 = %d, line_control_2 = %d !\n", start_x, end_x, start_x_remainder, end_x_remainder, line_control_1, line_control_2);       
        if(BitSet(_pth,PTH_X)) {
            WriteOSDLnVX(_dvc,start_x,OSD_POS_V_ODD|(start_y),line_control_1,OSD_POS_V_EVEN|(end_y),line_attribute,_color);
            if(line_control_2 != 0) WriteOSDLnVX(_dvc,start_x+1,OSD_POS_V_ODD|(start_y),line_control_2,OSD_POS_V_EVEN|(end_y),line_attribute,_color);
            }
        if(BitSet(_pth,PTH_Y)){
            WriteOSDLnVY(_dvc,start_x,OSD_POS_V_ODD|(start_y),line_control_1,OSD_POS_V_EVEN|(end_y),line_attribute,_color);
            if(line_control_2 != 0) WriteOSDLnVY(_dvc,start_x+1,OSD_POS_V_ODD|(start_y),line_control_2,OSD_POS_V_EVEN|(end_y),line_attribute,_color);
            
            //WriteOSDLnVY(_dvc,start_pos_x,OSD_POS_V_ODD|(start_pos_y),pos_control,OSD_POS_V_EVEN|(end_pos_y),line_attribute,_color);
            }
        }
    
    temp = 0x0f;    

    //WriteAsicByte(_dvc, DVC_PG2, 0x38, 0);
    
    if(start_pos_y == end_pos_y){
        if(BitSet(_pth,PTH_X)) {
            if(start_x_remainder != 0){
                line_control_1 = (temp >> start_x_remainder);
                //printf("\nstart_x = %d, line_control_1 = %d, start_x_remainder = %d !\n", start_x, line_control_1, start_x_remainder);
                WriteOSDSqrX_wg(_dvc,start_x,start_x+1,OSD_POS_V_ODD|(start_y),0x200|(start_y),line_attribute,_color, line_control_1 );
                start_x++;                
                }

            printf("\nstart_x = %d, end_x = %d, line_attribute = 0x%x, line_control_1 = 0x%x !\n", start_x, end_x, line_attribute, line_control_1);
    	
            WriteOSDSqrX_wg(_dvc,start_x,end_x,OSD_POS_V_ODD|(start_y),0x200|(start_y),line_attribute,_color, 15 );
            
            if(end_x_remainder != 0){
                line_control_1 = (temp << (4-end_x_remainder))&0x0f;
                //printf("\nend_x = %d, line_control_1 = %d, end_x_remainder = %d !\n", end_x, line_control_1, end_x_remainder);
                WriteOSDSqrX_wg(_dvc,end_x,end_x,OSD_POS_V_ODD|(start_y),0x200|(start_y),line_attribute,_color, line_control_1 );  
                }
            
            }
        if(BitSet(_pth,PTH_Y)) WriteOSDSqrY(_dvc,start_x,end_x,OSD_POS_V_ODD|(start_y),OSD_POS_V_EVEN|(start_y),line_attribute,_color);
        }
    
}


void    HwTW2835WriteOSDBMP(U8 _dvc, U8 _pth, U16 _osd_pos_x, U16 _osd_pos_y, U16 width, U16 higth, U32 *BMPBuffer)
{

    U16 i, j;
    U32 temp;
    U8 _color_1, _color_2, _color_3, _color_4, _attribute;

    _osd_pos_x = _osd_pos_x >> 2;
    width = width >> 2;
    _osd_pos_y = _osd_pos_y >> 1;
    higth = higth >> 1;
    
    if(_osd_pos_x > 180){
        _osd_pos_x = 179;        
        }

    if((_osd_pos_x + width) > 180){
        width = 179 - _osd_pos_x;
        if(_osd_pos_x == 179) width = 0;        
        }

    if(_osd_pos_y > 288){
        _osd_pos_y = 287;        
        }    

    if((_osd_pos_y + higth) > 288){
        higth = 288 - _osd_pos_y;
        if(_osd_pos_y == 287) higth = 0; 
        }
    
 #if 1           
    if(BitSet(_pth,PTH_X))  
        for(i=0; i<higth; i++){
            for(j=0; j < width; j++){
                temp = *BMPBuffer++;
                _attribute = (U8)(temp & 0xc0);
                _color_1 = (U8)(temp & 0xff);
                _color_2 = (U8)((temp >> 8) & 0xff);
                _color_3 = (U8)((temp >> 16) & 0xff);
                _color_4 = (U8)((temp >> 24) & 0xff);              
                WriteOSDBmp_pointX(_dvc,_osd_pos_x+j, _osd_pos_y+i, _color_1, _color_2, _color_3, _color_4, _attribute);
                }
            }
    if(BitSet(_pth,PTH_Y))  
        for(i=0; i<higth; i++){
            for(j=0; j < width; j++){
                temp = *BMPBuffer++;
                _attribute = (U8)(temp & 0xc0);
                _color_1 = (U8)(temp & 0xff);
                _color_2 = (U8)((temp >> 8) & 0xff);
                _color_3 = (U8)((temp >> 16) & 0xff);
                _color_4 = (U8)((temp >> 24) & 0xff);
                WriteOSDBmp_pointY(_dvc,_osd_pos_x,_osd_pos_y, _color_1, _color_2, _attribute);
                }
            }
            
            
    
 #else
    if(BitSet(_pth,PTH_X))  SetOSDWndwX(_pth,OSD_WNDW_TYPE_MAIN,_osd_pos_x,_osd_pos_y,0,0,_color_1, _attribute);
    if(BitSet(_pth,PTH_Y))  SetOSDWndwY(_pth,OSD_WNDW_TYPE_MAIN,_osd_pos_x,_osd_pos_y,0,0,_color_1, _attribute);
 #endif
    
}


void    HwTW2835AdjustHue(U8 _dvc, U8 _pth, U8 _ch, U8 HueRange)
{

    WriteAsicByte(_dvc,DVC_PG0,(0x07+_ch*16),HueRange);
    
}

void    HwTW2835AdjustSaturation(U8 _dvc, U8 _pth, U8 _ch, U8 SaturationRange)
{

    WriteAsicByte(_dvc,DVC_PG0,(0x08+_ch*16),SaturationRange);
    
}

void    HwTW2835AdjustContrast(U8 _dvc, U8 _pth, U8 _ch, U8 ContrastRange)
{

    WriteAsicByte(_dvc,DVC_PG0,(0x09+_ch*16),ContrastRange);
    
}

void    HwTW2835AdjustBrightness(U8 _dvc, U8 _pth, U8 _ch, U8 BrightnessRange)
{

    WriteAsicByte(_dvc,DVC_PG0,(0x0a+_ch*16),BrightnessRange);
    
}


void    HwTW2835BitmapPageChange(U8 _dvc, U8 _pth, U8 page_attributr)
{

    U8 t1, _attribute, _t3_,value;
   /* 
    if(BitSet(_pth,PTH_X)){
            if( page_attributr > 5 ) printf("\nDVR@ The Bitmap page scope of display path is 0~5!\n" );
        WriteAsicByte(MASTER,DVC_PG1,0xa0,0x53);
            _t3_ = ReadAsicByte(MASTER, DVC_PG1, 0xa0);
        //printf("\n1xa0 = %x\n", _t3_);
            }
    if(BitSet(_pth,PTH_Y)){
            if( page_attributr != 0 ) printf("\nDVR@ The Bitmap page of record path is only 0!\n" ); 
            WriteAsicByte(MASTER,DVC_PG1,0xa0,0x77);
            _t3_ = ReadAsicByte(MASTER, DVC_PG1, 0xa0);
        //printf("\n1xa0 = %x\n", _t3_);
            }
*/ //set one time is enough--vincent 08.04.08
   /* t1 = ReadAsicByte(_dvc,DVC_PG2,0x0f);
    
    _attribute = ((page_attributr << 4) & 0x70)|( t1 & 0x0f);

    WriteAsicByte(_dvc,DVC_PG2,0x0f,_attribute);*/
  
    if(BitSet(_pth,PTH_X))
    {
        if( page_attributr > 5 ) 
        {
            printf("\nDVR@ The Bitmap page scope of display path is 0~5!\n" );
            return ;
        }
    }
    if(BitSet(_pth,PTH_Y))
    {
        printf("\nDVR@ The Bitmap page of record path is only 0!\n" ); 
        return ;
    }
 
    //t1 = ReadAsicByte(_dvc,DVC_PG2,0x0f);
    value = ((page_attributr << 4) & 0x70)|( 0x0f);
    t1=TW_IICWriteByteB256(_dvc, DVC_PG2, 0x0f, &value, 1);
   
}

void    HwTW2835FillOSDBMP(U8 _dvc, U8 _pth, U16 _osd_pos_x, U16 _osd_pos_y, U8 *BMPBuffer, U32 *BMPIndex,  U8 attribute)
{

    U16 i, j;
    U32 temp;
    U8 _prp_pg;
    U8 _color_1, _color_2, _color_3, _color_4, _attribute;

    U8 _dly_, _t1_;

    _osd_pos_x = _osd_pos_x >> 2;
    _osd_pos_y = _osd_pos_y >> 1;

    _attribute = (attribute & 0xc0);	
    _prp_pg = attribute & 0x07;

    
    if(_osd_pos_x > 180){
        _osd_pos_x = 179;        
        }


    if(_osd_pos_y > 288){
        _osd_pos_y = 287;        
        }    
    //SetAsicFlgType(cmn_dvc,DVC_PG2,0x0f,BIT0|BIT1,BIT0|BIT1);
    if(BitSet(_pth,PTH_X)){ 
    	
        //WriteAsicByte(_dvc,DVC_PG1,0xa0,0x53);//set one time is enough--vincent 08.04.08
        FillOSDBmpX(_dvc, _osd_pos_x, _osd_pos_y, _prp_pg, 0x00, (U8 *)BMPBuffer, _attribute);
    }
    if(BitSet(_pth,PTH_Y)){
/*
    for(_dly_=0;_dly_<14;_dly_++){
        for(_t1_=0;_t1_<8;_t1_++){			
WriteOSDBmpY(MASTER,50+(_t1_*(gt_tst_fnt_16x20_tst_fnt_16x20[3]>>3)),10+(_dly_*(gt_tst_fnt_16x20_tst_fnt_16x20[1]>>1)),0x00,(_dly_*8)+_t1_,
    gt_tst_fnt_16x20_tst_fnt_16x20);
    	}
    }
*/
        
   // WriteAsicByte(MASTER,DVC_PG1,0xa0,0x53);//set one time is enough--vincent 08.04.08
    WriteOSDBmpY(MASTER,_osd_pos_x>>1,_osd_pos_y,0x00,0,BMPBuffer);

    }
    
}

void HwTW2835WriteOSDStr(U8 _dvc, U8 _pth, TW2835_WriteString * t2835WriteString )
{
    U16  _pos_y;
        U8 _prp, _pos_x;
       U8 *_str_ptr, _str_len;
       U8 test_tmp,_stringcolor,i,*str;

       _pos_x = (t2835WriteString->start_x)>>4;
       _pos_y = t2835WriteString->start_y>>1;
       _str_ptr = t2835WriteString->str_addr;
       _str_len = t2835WriteString->str_len;
       _prp = ((t2835WriteString->attribute)&0x07) | (((t2835WriteString->attribute)&0x030) << 2);
       _stringcolor = ((t2835WriteString->attribute) & 0xff00) >> 8;  //byVINCENT 2007.10.18
       if(_stringcolor>63) printf("\n_stringcolor out of ramge\n");
       
       WriteAsicByte(_dvc, DVC_PG2, 0x38, 0);
       test_tmp = ReadAsicByte(_dvc, DVC_PG2, 0x38);
       //TW2835's character lib does not contain the value from 0~31,
       //so set this value to ' ';avoid error character. 2008.01.02---VINCENT
       str = _str_ptr;
       for(i=0;i<_str_len;i++)
       {   
            if((*str)<0x20)           
                *str = 0x20; 
            str++;
       }
    if(BitSet(_pth,PTH_X))  WriteOSDStrX_wg(_dvc,_pos_x,_pos_y,_prp,_str_ptr, _str_len,_stringcolor);
    if(BitSet(_pth,PTH_Y))  WriteOSDStrY_wg(_dvc,_pos_x,_pos_y,_prp,_str_ptr, _str_len,_stringcolor);
}

//==================================================================================
void    HwTW2835_SingleBoxDisplay(U8 _dvc, U8 _pth, U8 _ch, TW2835_2DBox * TwoDBoxDisplay )
{
    int color,attribute;
    int pos_x,pos_y,width,height;
    int boundary_col,temp;

    pos_x = TwoDBoxDisplay->TDBox_HL;
    pos_y = TwoDBoxDisplay->TDBox_VT;
    height = TwoDBoxDisplay->TDBox_VW;
    width = TwoDBoxDisplay->TDBox_HW;
    color = TwoDBoxDisplay->TDBox_PlaneCol &0x0f;
    attribute = TwoDBoxDisplay->TDBox_Attribute;
    
    temp =ReadAsicByte(_dvc,DVC_PG2,0x20+_ch*6);
    if(_pth ==1) //display path
        attribute =(temp&0x38) |(attribute &0xc7) ;
    else             //record path   
        attribute =(temp&0x07) |(attribute &0xf8) ;
    
    if(pos_x > 720)   
        printf("Single Box Display X start is too large !\n");  
    if(pos_y > 576)    
        printf("SingleBoxDisplay Y start is too large !\n");
    if(height > 576)
    {
        printf("SingleBoxDisplay higth is large !\n");
        height = 576;
    }
    if(width > 720)
    {
        printf("SingleBoxDisplay width is large !\n");
        width = 720;
    }
    
    pos_x = pos_x >> 2;
    width = width >> 2;
    pos_y = pos_y >> 2;
    height = height >> 2;
    
     WriteAsicByte(_dvc,DVC_PG2,0x20+_ch*6, attribute);
     WriteAsicByte(_dvc,DVC_PG2,0x21+_ch*6, color<<4);

     WriteAsicByte(_dvc,DVC_PG2,0x22+_ch*6, pos_x);
     WriteAsicByte(_dvc,DVC_PG2,0x23+_ch*6, width);

     WriteAsicByte(_dvc,DVC_PG2,0x24+_ch*6, pos_y);
     WriteAsicByte(_dvc,DVC_PG2,0x25+_ch*6, height);
}

void    HwTW2835_2DBoxDisplay(U8 _dvc, U8 _pth, U8 _ch, TW2835_2DBox * TwoDBoxDisplay )
{
    U16 pos_x, pos_y;
    U16 higth, width;
    U8 TDBox_Num, TDBox_PlaneCol;
    U16 attribute;
    U8 TDBox_BndCol, TDBox_BndEN, TDBox_MIX, TDBox_EN;
    U8 test_tmp[10];
    U8 i;

    pos_x = TwoDBoxDisplay->TDBox_HL;
    if(pos_x > 720){
        printf("2DBoxDisplay X start is large !");
        }
    pos_x = pos_x >> 1;
            
    pos_y = TwoDBoxDisplay->TDBox_VT;
        if(pos_y > 576){
        printf("2DBoxDisplay Y start is large !");
        }
    pos_y = pos_y >> 1;
    
    higth = TwoDBoxDisplay->TDBox_VW;
    if(higth > 510){
        printf("2DBoxDisplay higth is large !");
        higth = 510;
        }
    if((higth +TwoDBoxDisplay->TDBox_VT) > 576){
        printf("2DBoxDisplay Y scope is large !");
        }
    higth = higth >> 1;
    
    width = TwoDBoxDisplay->TDBox_HW;
    if(width > 510){
        printf("2DBoxDisplay width is large !");
        width = 510;
        }
    if((width +TwoDBoxDisplay->TDBox_HL) > 720){
        printf("2DBoxDisplay X scope is large !");
        }
    width = width >> 1;

    TDBox_Num = TwoDBoxDisplay->TDBox_NUM;
    TDBox_PlaneCol = TwoDBoxDisplay->TDBox_PlaneCol;
    attribute = TwoDBoxDisplay->TDBox_Attribute;
    TDBox_BndCol = (unsigned char)(attribute & 0x03);
    TDBox_BndEN = (unsigned char)((attribute & 0x04)>>2);
    TDBox_MIX = (unsigned char)((attribute & 0x08));
    TDBox_EN = (unsigned char)((attribute & 0x10)<<3);
    //TDBox_EN = 0x7f & TDBox_EN;
    //TDBox_Mode = (unsigned char)(attribute & 0x20);
//==set the mask plane &detect plane as same color when only set detect plane color
//-------by vincent 2008.01.24    
    if((TDBox_PlaneCol &0xf0)==0)
       TDBox_PlaneCol |= (TDBox_PlaneCol <<4);
    if( TDBox_EN != 0 ){
        SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x3f,(TDBox_MIX));
        if(_pth == PTH_X)
            SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x80,0x80);
        else if(_pth == PTH_Y)
            SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x40,0x40);//enable record path
//=======use both mask plane & detect plane---by vincent 2008.01.24=============            
        WriteAsicByte(_dvc,DVC_PG2,(0x61+_ch*8),(0x30|(TDBox_BndEN<<3)|((pos_x&0x01)<<1)|(pos_y&0x01)));
        WriteAsicByte(_dvc,DVC_PG2,0x62+_ch*8, (unsigned char)(pos_x>>1));
        WriteAsicByte(_dvc,DVC_PG2,0x63+_ch*8, width);        

        WriteAsicByte(_dvc,DVC_PG2,0x64+_ch*8, (unsigned char)(pos_y>>1));
        WriteAsicByte(_dvc,DVC_PG2, (0x65+_ch*8), higth);
        WriteAsicByte(_dvc,DVC_PG2,(0x66+_ch*8), TDBox_Num);
        WriteAsicByte(_dvc,DVC_PG2,(0x5b+_ch),TDBox_PlaneCol);
        WriteAsicByte(_dvc,DVC_PG2,0x5f,(TDBox_BndCol<<(_ch*2)));

        test_tmp[0] = ReadAsicByte(_dvc, DVC_PG2, 0x60);
        test_tmp[1] = ReadAsicByte(_dvc, DVC_PG2, 0x61);
        test_tmp[2] = ReadAsicByte(_dvc, DVC_PG2, 0x62);
        test_tmp[3] = ReadAsicByte(_dvc, DVC_PG2, 0x63);
        test_tmp[4] = ReadAsicByte(_dvc, DVC_PG2, 0x64);
        test_tmp[5] = ReadAsicByte(_dvc, DVC_PG2, 0x65);
        test_tmp[6] = ReadAsicByte(_dvc, DVC_PG2, 0x66);
        test_tmp[7] = ReadAsicByte(_dvc, DVC_PG2, 0x5b);        
        test_tmp[8] = ReadAsicByte(_dvc, DVC_PG2, 0x5f);
        printf("2x60 = %2x! 2x61 = %2x! 2x62 = %2x! 2x63 = %2x! 2x64 = %2x! 2x65 = %2x! 2x66 = %2x! 2x5b = %2x! 2x5f = %2x! \n", test_tmp[0], test_tmp[1], test_tmp[2], test_tmp[3], test_tmp[4], test_tmp[5], test_tmp[6], test_tmp[7], test_tmp[8]);
        }
    else{
        SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x3f,(TDBox_MIX));
        if(_pth == PTH_X)
            SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x80,0x00);
        else if(_pth == PTH_Y)
            SetAsicFlgType(_dvc,DVC_PG2,(0x60+_ch*8),0x40,0x00);//disable record path
        
        }

}

void Hw2835PrintRegister()
{
    U8 i, _t1_;
    U8 reg_temp[10];

        
    for(i = 0; i <25 ; i++){
        reg_temp[0] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8);
        reg_temp[1] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+1);
        reg_temp[2] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+2);
        reg_temp[3] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+3);
        reg_temp[4] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+4);
        reg_temp[5] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+5);
        reg_temp[6] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+6);
        reg_temp[7] = ReadAsicByte(MASTER, DVC_PG0, 0x00+i*8+7);
        printf("0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! 0x%2x = %2x! \n", i*8, reg_temp[0], i*8+1, reg_temp[1], i*8+2, reg_temp[2], i*8+3, reg_temp[3], i*8+4, reg_temp[4], i*8+5, reg_temp[5], i*8+6, reg_temp[6], i*8+7, reg_temp[7]);
        } 
    printf("\n");

    for(i = 0; i <24 ; i++){
        reg_temp[0] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8);
        reg_temp[1] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+1);
        reg_temp[2] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+2);
        reg_temp[3] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+3);
        reg_temp[4] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+4);
        reg_temp[5] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+5);
        reg_temp[6] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+6);
        reg_temp[7] = ReadAsicByte(MASTER, DVC_PG1, 0x00+i*8+7);
        printf("1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! 1x%2x = %2x! \n", i*8, reg_temp[0], i*8+1, reg_temp[1], i*8+2, reg_temp[2], i*8+3, reg_temp[3], i*8+4, reg_temp[4], i*8+5, reg_temp[5], i*8+6, reg_temp[6], i*8+7, reg_temp[7]);
        }
    printf("\n ");

    for(i = 0; i <32 ; i++){
        reg_temp[0] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8);
        reg_temp[1] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+1);
        reg_temp[2] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+2);
        reg_temp[3] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+3);
        reg_temp[4] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+4);
        reg_temp[5] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+5);
        reg_temp[6] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+6);
        reg_temp[7] = ReadAsicByte(MASTER, DVC_PG2, 0x00+i*8+7);
        printf("2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! 2x%2x = %2x! \n", i*8, reg_temp[0], i*8+1, reg_temp[1], i*8+2, reg_temp[2], i*8+3, reg_temp[3], i*8+4, reg_temp[4], i*8+5, reg_temp[5], i*8+6, reg_temp[6], i*8+7, reg_temp[7]);
        }
    

}

void Hw2835PrintRegisterMD()
{
    U8 i, _t1_;
    U8 reg_temp[10];
        //WriteAsicByte(MASTER,DVC_PG1,0xa0,0x52);//set one time is enough--vincent 08.04.08
        reg_temp[0] = ReadAsicByte(MASTER, DVC_PG1, 0xa0);
        reg_temp[1] = ReadAsicByte(MASTER, DVC_PG1, 0xa3);
        reg_temp[2] = ReadAsicByte(MASTER, DVC_PG1, 0xad);
        reg_temp[3] = ReadAsicByte(MASTER, DVC_PG1, 0xac);
        printf("\n1xa0 = %2x! 1xa3 = %2x! 1xad = %2x! 1xac = %2x!  \n", reg_temp[0],  reg_temp[1],  reg_temp[2],  reg_temp[3]);

    

}



extern unsigned int AudioVoice ;

void AppTW2835Audio(U8 _dvc, U8 _pth, U8 _ch, TW2835_AudioSet * tAudioSet  )
{
    unsigned char i,j;
       unsigned char cAudioGainShift, cAudioGainDirection, cAudioGainAdjust;
       unsigned char cAudioGainLive, cAudioGainMixmute;
       unsigned char cAudioGainMixratio0, cAudioGainMixratio1, cAudioGainMixratio2, cAudioGainMixratio3, cAudioGainMixratioP;
       unsigned char cAudioGainMixmode, cAudioGainOutsel;
       unsigned char cAudioGainTemp;

       unsigned char cAudioRecordMultch, cAudioRecordADATM, cAudioRM_bitrate, cAudioRM_samrate, cAudioRM_bitwid, cAudioRM_sync;
       unsigned char cAudioPB_bitrate, cAudioPB_samrate, cAudioPB_bitwid, cAudioPB_sync;
       unsigned int * pRecordSEQAddr;
    
       switch( tAudioSet->subInstruction ){
           case DVR_AUDIO_GAIN:
                      cAudioGainShift = ( tAudioSet->Instruction_attribute_1 ) & 0x0f;
                    cAudioGainDirection = (( tAudioSet->Instruction_attribute_1 ) & 0x0f0) >> 4;  
                    cAudioGainAdjust = (( tAudioSet->Instruction_attribute_1 ) & 0x0ff00) >> 8;
                      
                    if(cAudioGainShift == 0){
                        AppTW2835AudioSetVolume(_dvc, _ch, cAudioGainDirection, cAudioGainAdjust);
                        }
                    if(cAudioGainShift == 1){
                        AppTW2835AudioSetVolume(_dvc, _ch, cAudioGainDirection, cAudioGainAdjust);
                        } 
                	
                break;
           case DVR_AUDIO_LIVE:
                    cAudioGainLive = ( tAudioSet->Instruction_attribute_1 ) & 0x0f;
                     cAudioGainMixmute = (( tAudioSet->Instruction_attribute_1 ) & 0x1f0)>>4;
                     cAudioGainMixratio0 = (( tAudioSet->Instruction_attribute_1 ) & 0x0f000)>>12;
                     cAudioGainMixratio1 = (( tAudioSet->Instruction_attribute_1 ) & 0x0f0000)>>16;
                     cAudioGainMixratio2 = (( tAudioSet->Instruction_attribute_1 ) & 0x0f00000)>>20;
                     cAudioGainMixratio3 = (( tAudioSet->Instruction_attribute_1 ) & 0x0f000000)>>24;
                     cAudioGainMixratioP = (( tAudioSet->Instruction_attribute_1 ) & 0x0f0000000)>>28;
                     cAudioGainOutsel = (( tAudioSet->Instruction_attribute_1 ) & 0x1f00000000)>>32;
                     cAudioGainMixmode = (( tAudioSet->Instruction_attribute_1 ) & 0x2000000000)>>37;

                     if(cAudioGainLive == 0){
                        WriteAsicByte(_dvc,DVC_PG0,0x71, (cAudioGainMixmode << 5) | cAudioGainOutsel);
                        }
                     else{
                        WriteAsicByte(_dvc,DVC_PG0,0x6d, 0 | cAudioGainMixmute);
                        WriteAsicByte(_dvc,DVC_PG0,0x6e, (cAudioGainMixratio1<<4) | cAudioGainMixratio0);
                        WriteAsicByte(_dvc,DVC_PG0,0x6f, (cAudioGainMixratio3<<4) | cAudioGainMixratio2);
                        cAudioGainTemp = ReadAsicByte(_dvc, DVC_PG0, 0x70);
                        WriteAsicByte(_dvc,DVC_PG0,0x70, (cAudioGainTemp & 0xf0) | cAudioGainMixratioP);
                        }
                break;
           case DVR_AUDIO_RECORD:
                    cAudioRecordMultch = ( tAudioSet->Instruction_attribute_1 ) & 0x03;
                     cAudioRecordADATM = (( tAudioSet->Instruction_attribute_1 ) & 0x04)>>2;
                     cAudioRM_bitrate = (( tAudioSet->Instruction_attribute_1 ) & 0x08)>>3;
                     cAudioRM_samrate = (( tAudioSet->Instruction_attribute_1 ) & 0x010)>>4;
                     cAudioRM_bitwid = (( tAudioSet->Instruction_attribute_1 ) & 0x020)>>5;
                     cAudioRM_sync = (( tAudioSet->Instruction_attribute_1 ) & 0x040)>>6;
                     
                     cAudioPB_bitrate = (( tAudioSet->Instruction_attribute_1 ) & 0x80)>>7;
                     cAudioPB_samrate = (( tAudioSet->Instruction_attribute_1 ) & 0x0100)>>8;
                     cAudioPB_bitwid = (( tAudioSet->Instruction_attribute_1 ) & 0x0200)>>9;
                     cAudioPB_sync = (( tAudioSet->Instruction_attribute_1 ) & 0x0400)>>10;
                     pRecordSEQAddr =  tAudioSet->Instruction_attribute_2 ;
                     
                     SetAsicFlgType( _dvc, DVC_PG0, 0x62, BIT4|BIT2|BIT1|BIT0, (cAudioRM_bitrate<<4)|(cAudioRM_samrate<<2)|(cAudioRM_bitwid<<1)|cAudioRM_sync );
                     WriteAsicByte(_dvc,DVC_PG0,0x63, (cAudioRecordADATM<<2) | cAudioRecordMultch);
                     SetAsicFlgType( _dvc, DVC_PG0, 0x6c, BIT4|BIT2|BIT1|BIT0, (cAudioRM_bitrate<<4)|(cAudioRM_samrate<<2)|(cAudioRM_bitwid<<1)|cAudioRM_sync );
                     for(i = 0; i < 8 ; i++) WriteAsicByte(_dvc,DVC_PG0,0x64, *((unsigned char *) pRecordSEQAddr + i));
                     
                break;
        default:
                    printf("\nDVR@ error: Audio subInstruction error!\n");
        }    
    
}


void AppTW2835AudioSetVolume( U8 _dvc, U8 _ch, U8 direction, U8 volume)
{
    unsigned char volume_60, volume_61;

        if(direction == 0){
            volume_60 = ReadAsicByte(_dvc, DVC_PG0, 0x60);
           volume_61 = ReadAsicByte(_dvc, DVC_PG0, 0x61);
           switch( _ch ){
               case 0:
                        WriteAsicByte(_dvc,DVC_PG0,0x60, (volume_60 & 0xf0) | (volume & 0x0f));
                    break;
               case 1:
                        WriteAsicByte(_dvc,DVC_PG0,0x60, (volume_60 & 0x0f) | ((volume & 0x0f)<<4));
                    break;
               case 2:
                        WriteAsicByte(_dvc,DVC_PG0,0x61, (volume_61 & 0xf0) | (volume & 0x0f));
                    break;
               case 3:
                        WriteAsicByte(_dvc,DVC_PG0,0x61, (volume_61 & 0x0f) | ((volume & 0x0f)<<4));
                    break;
            default:
                        printf("\nDVR@ warning: AudioSetVolume channnel number must 0~3!\n");
            } 
       }
        else if(direction == 1){
            volume_61 = ReadAsicByte(_dvc, DVC_PG0, 0x70);
            WriteAsicByte(_dvc,DVC_PG0,0x70, (volume_61 & 0x0f) | ((volume & 0x0f)<<4));
            }
        else printf("DVR@ warning: AudioSetVolume change inc=0; dec=1 ! ");
        
            
}

//================by VINCENT 2007.10.22=================================
void AppTW2835Audio_Gain(U8 _dvc, U8 _pth, U8 _ch, U16 attribute  )
{
    unsigned char  cAudioGainDirection;
    unsigned char cAudioGainAdjust;
    unsigned char cAudioGainUpDown;
    unsigned char volume_60, volume_61;
    signed char volume_temp,sign;   
    
 
    cAudioGainUpDown = attribute & 0x1;
    cAudioGainDirection = (attribute & 0x2) >> 1;
    cAudioGainAdjust = (attribute & 0x7ffc) >> 2;
    //printf("\n volume before add : %d  ch_0 : %x\n ",cAudioGainAdjust,ReadAsicByte(_dvc, DVC_PG0, 0x61)); 
   if(cAudioGainUpDown) 
        sign = -1;
   else
        sign = 1;
    if(cAudioGainDirection == 0)//input 
    {
           volume_60 = ReadAsicByte(_dvc, DVC_PG0, 0x60);
           volume_61 = ReadAsicByte(_dvc, DVC_PG0, 0x61);
           switch( _ch )
           {
               case 0:
                        volume_temp = (signed char) (volume_60 & 0xf) + (sign * (signed char)cAudioGainAdjust);
                        printf(" \nvolume_temp = %x \n",volume_temp);            
                        if(volume_temp < 0) volume_temp = 0;
                        if(volume_temp > 15) volume_temp = 15;
                        WriteAsicByte(_dvc,DVC_PG0,0x60, (volume_60 & 0xf0) | (volume_temp & 0x0f));
                    break;
               case 1:
                        volume_temp = (signed char)((volume_60 & 0xf0) >>4)+  (sign * (signed char)cAudioGainAdjust);
                        if(volume_temp < 0) volume_temp = 0;
                        if(volume_temp > 15) volume_temp = 15;
                        WriteAsicByte(_dvc,DVC_PG0,0x60, (volume_60 & 0x0f) | ((volume_temp & 0x0f)<<4));
                    break;
               case 2:
                        volume_temp = (signed char) (volume_61 & 0xf) +  (sign * (signed char)cAudioGainAdjust);
                        if(volume_temp < 0) volume_temp = 0;
                        if(volume_temp > 15) volume_temp = 15;
                        WriteAsicByte(_dvc,DVC_PG0,0x61, (volume_61 & 0xf0) | (volume_temp & 0x0f));
                    break;
               case 3:
                        volume_temp = (signed char)((volume_61 & 0xf0) >> 4) +  (sign * (signed char)cAudioGainAdjust);
                        if(volume_temp < 0) volume_temp = 0;
                        if(volume_temp > 15) volume_temp = 15;
                        WriteAsicByte(_dvc,DVC_PG0,0x61, (volume_61 & 0x0f) | ((volume_temp & 0x0f)<<4));
                    break;
            default:
                        printf("\nDVR@ warning: AudioSetVolume channnel number must 0~3!\n");
           } 
    }
        else if(cAudioGainDirection == 1){ //output
            volume_61 = ReadAsicByte(_dvc, DVC_PG0, 0x70);
            volume_temp = (signed char) ((volume_61 & 0xf0) >> 4) +  (sign * (signed char)cAudioGainAdjust);
            if(volume_temp < 0) volume_temp = 0;
            if(volume_temp > 15) volume_temp = 15;
            WriteAsicByte(_dvc,DVC_PG0,0x70, (volume_61 & 0x0f) | ((volume_temp & 0x0f)<<4));
            }
        else printf("DVR@ warning: AudioSetVolume change inc=0; dec=1 ! ");                     
        printf("\n volume after add :  ch_1_0 : %x\n ",ReadAsicByte(_dvc, DVC_PG0, 0x60)); 
        printf("\n volume after add :  ch_3_2 : %x\n ",ReadAsicByte(_dvc, DVC_PG0, 0x61)); 
        printf("\n volume after add :  out : %x\n ",ReadAsicByte(_dvc, DVC_PG0, 0x70));  
}
void AppTW2835Audio_Live(U8 _dvc, U8 _pth, U8 _ch, unsigned int  attribute  )
{
      unsigned char i,j;
      unsigned char cAudioGainMixmute;
      unsigned char cAudioGainMixratio0;
      unsigned char cAudioGainMixratio1;
      unsigned char cAudioGainMixratio2;
      unsigned char cAudioGainMixratio3;
      unsigned char cAudioGainMixratioP;
      unsigned char cAudioMixMode;
      unsigned char cAudioMixOutSel;
      
      cAudioGainMixratio0 = (attribute & 0xF) ;
      cAudioGainMixratio0 = (attribute & 0xF0) >> 4;
      cAudioGainMixratio0 = (attribute & 0xF00) >> 8;
      cAudioGainMixratio0 = (attribute & 0xF000) >> 12;
      cAudioGainMixratioP = (attribute & 0xF0000) >> 16;
      cAudioMixOutSel      = (attribute & 0x1F00000) >> 20;      
      cAudioGainMixmute  = (attribute & 0x3E000000) >>25;
      cAudioMixMode        = (attribute & 0x40000000) >> 30;
      
     // if(cAudioMixOutSel == 0xf)
      //  cAudioMixOutSel = 0x14;
      WriteAsicByte(_dvc,DVC_PG0,0x6d, 0 | cAudioGainMixmute);
      WriteAsicByte(_dvc,DVC_PG0,0x6e, (cAudioGainMixratio1<<4) | cAudioGainMixratio0);
      WriteAsicByte(_dvc,DVC_PG0,0x6f, (cAudioGainMixratio3<<4) | cAudioGainMixratio2);      
      SetAsicFlgType(_dvc, DVC_PG0, 0x70, 0x0f, cAudioGainMixratioP);
      WriteAsicByte(_dvc,DVC_PG0,0x71,0x40| (cAudioMixMode<<5) | cAudioMixOutSel);
      
}


void AppTW2835_Record(U8 _dvc, U8 _pth, U8 _ch,unsigned int attribute, unsigned int * pRecordSEQAddr )
{
         unsigned char cAudioRecordMultch, cAudioRecordADATM;
         unsigned char cAudioRM_bitrate, cAudioRM_samrate;
         unsigned char  cAudioRM_bitwid, cAudioRM_sync;
         unsigned char cAudioPB_bitrate, cAudioPB_samrate;
         unsigned char cAudioPB_bitwid, cAudioPB_sync;
         unsigned char i;
         
         cAudioRecordMultch = ( attribute) & 0x03;
         cAudioRecordADATM = (attribute & 0x04)>>2;
         cAudioRM_bitrate = (attribute & 0x08)>>3;
         cAudioRM_samrate = (attribute & 0x010)>>4;
         cAudioRM_bitwid = (attribute & 0x020)>>5;
         cAudioRM_sync = (attribute & 0x040)>>6;
         
         cAudioPB_bitrate = (attribute & 0x80)>>7;
         cAudioPB_samrate = (attribute & 0x0100)>>8;
         cAudioPB_bitwid = (attribute & 0x0200)>>9;
         cAudioPB_sync = (attribute & 0x0400)>>10;
      
         
         SetAsicFlgType( _dvc, DVC_PG0, 0x62, BIT4|BIT2|BIT1|BIT0, (cAudioRM_bitrate<<4)|(cAudioRM_samrate<<2)|(cAudioRM_bitwid<<1)|cAudioRM_sync );
        //SetAsicFlgType( _dvc, DVC_PG0, 0x62, BIT3, BIT3 );
         WriteAsicByte(_dvc,DVC_PG0,0x63, (cAudioRecordADATM<<2) | cAudioRecordMultch);
         SetAsicFlgType( _dvc, DVC_PG0, 0x6c, BIT5|BIT4|BIT2|BIT1|BIT0, 0|(cAudioPB_bitrate<<4)|(cAudioPB_samrate<<2)|(cAudioPB_bitwid<<1)|cAudioPB_sync );
         for(i = 0; i < 8 ; i++) 
         {
            WriteAsicByte(_dvc,DVC_PG0,0x64+i, *((unsigned char *) pRecordSEQAddr + i));
            printf("\n0x%x  0x%x\n",0x64+i,ReadAsicByte(_dvc,DVC_PG0,0x64+i));
         }
         
    
}

void AppTW2835AudioInit()
{
         unsigned char cAudioRecordMultch, cAudioRecordADATM;
         unsigned char cAudioRM_bitrate, cAudioRM_samrate;
         unsigned char  cAudioRM_bitwid, cAudioRM_sync;
         unsigned char cAudioPB_bitrate, cAudioPB_samrate;
         unsigned char cAudioPB_bitwid, cAudioPB_sync;
         unsigned char i,pRecordSEQAddr[8] = {0x10,0x32,0x54,0x76,0x98,0xba,0xdc,0xfe};
         
         cAudioRecordMultch = 3;      // 1 4 audios
         cAudioRecordADATM = 0;     // Digital serial data of record audio
         cAudioRM_bitrate = 0;         //256 bit per sample period (256fs) 
         cAudioRM_samrate = 0;       //0 :8KHz                  1 :16KHz
         cAudioRM_bitwid = 0;          //0:16 bit                  1:8 bit 
         cAudioRM_sync = 1;            //0:I2S                    1:DSP format
         
         cAudioPB_bitrate =0;           //256 bit per sample period (256fs) 
         cAudioPB_samrate = 0;       //0 :8KHz                  1 :16KHz
         cAudioPB_bitwid = 0;          //0:16 bit                  1:8 bit 
         cAudioPB_sync = 1;            //0:I2S                    1:DSP format
      
         
         SetAsicFlgType( MASTER, DVC_PG0, 0x62, BIT4|BIT2|BIT1|BIT0, (cAudioRM_bitrate<<4)|(cAudioRM_samrate<<2)|(cAudioRM_bitwid<<1)|cAudioRM_sync );
         WriteAsicByte(MASTER,DVC_PG0,0x63, (cAudioRecordADATM<<2) | cAudioRecordMultch);
         //SetAsicFlgType( MASTER, DVC_PG0, 0x6c, BIT4|BIT2|BIT1|BIT0, (cAudioPB_bitrate<<4)|(cAudioPB_samrate<<2)|(cAudioPB_bitwid<<1)|cAudioPB_sync );
         for(i = 0; i < 8 ; i++) 
         {
            WriteAsicByte(MASTER,DVC_PG0,0x64+i,  pRecordSEQAddr[ i]);
            printf("\nRecordSEQ %d: %x\n",i,pRecordSEQAddr[ i]);
         }    
}
//================by VINCENT 2007.10.22=================================

int HwGetVB2100ChipID(void)
{
     return ((*(int *)(0xa8010010 ) >>18) & 0x3);
 
}

void HwEnableVideoLossDetect(unsigned short channel,unsigned short enable)
{
    
    if(enable)//1:enable  0:disable
        gVideoLossEn |= (1<<channel);
    else
        gVideoLossEn &= ~(1<<channel); 
    printf("\n gVideoLossEn %x\n",gVideoLossEn);   
}

void HwEnableVideoBlindDetect(U8 _dvc, U8 channel,U8 enable,unsigned short BD_sense)
{
     unsigned char cellsen,levelsen;
     
     if(enable)//1:enable  0:disable
         gVideoBlindEn |= (1<<channel);
     else
         gVideoBlindEn &= ~(1<<channel); 
     if(enable)
     {
         cellsen = BD_sense &0xff;
         levelsen = BD_sense>>8;
         SetAsicFlgType( _dvc, DVC_PG2, 0x80+0x20*channel,BIT5|BIT4,(cellsen<<4));
         SetAsicFlgType( _dvc, DVC_PG2, 0x80+0x20*channel,0x0f,levelsen);
     }
     printf("\n gVideoBlindEn %x\n",gVideoBlindEn);
}

void HwEnableVideoNightDetect(U8 _dvc, U8 channel,U8 enable,unsigned short ND_sense)
{
     unsigned char tempsen,levelsen;
     
     if(enable &0x01)//1:enable  0:disable
         gVideoNightEn |= (1<<channel);
     else
         gVideoNightEn &= ~(1<<channel); 
     if(enable)
     {
         tempsen = ND_sense>>8 ;
         levelsen = ND_sense&0xff;
         SetAsicFlgType( _dvc, DVC_PG2, 0x81+0x20*channel,0xf0,(levelsen<<4));
         SetAsicFlgType( _dvc, DVC_PG2, 0x81+0x20*channel,0x0f,tempsen);
     }
     printf("\n gVideoNightEn %x\n",gVideoNightEn);
}


void HwEnableDisplayChannel(int _ch_)
{    
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x10+(_ch_<<3),BIT7,BIT7);

}
void HwDisableDisplayChannel(int _ch_)
{    
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x10+(_ch_<<3),BIT7,0);

}

void HwEnableRecordChannel(int _ch_)
{    
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x60+(_ch_*3),BIT7,BIT7);

}
void HwDisableRecordChannel(int _ch_)
{    
    SetAsicFlgType(cmn_dvc,DVC_PG1,0x60+(_ch_*3),BIT7,0);

}

void HwSetBGContrast(U8 Value)
{
    switch(Value)
    {
        case 1: Value =  0x00;break;
        case 2: Value =  0x2a;break;
        case 3: Value =  0x3f;break;
        default: Value =  0x00;break;
    }
    SetAsicFlgType(cmn_dvc,DVC_PG2,0x1f,0x3f,Value);
}
