#include    "hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ds_scrn.h"
#include "wndw_ds_src.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "wndw_rc_fmt.h"
#include "wndw_mtn_md.h"
#include <comm.h>

//==================================================================================
//                      Display wndwdow function description
//==================================================================================
#if defined(__16CH__)
U8  GetWndwDsSrPbOfst(void)
{
    U8 _t1_;

    if(cmn_rc_md == RC_MD_480_FPS){
        if(wndw_crnt_dvc == MASTER) _t1_ = 0;
        else if(wndw_crnt_dvc == SLAVE1)    _t1_ = 4;
        else if(wndw_crnt_dvc == SLAVE2)    _t1_ = 8;
        else if(wndw_crnt_dvc == SLAVE3)    _t1_ = 12;
    }
    else if(cmn_rc_md == RC_MD_240_FPS){
        if(wndw_crnt_dvc < SLAVE2)  _t1_ = 0;
        else    _t1_ = 8;
    }
    else if(cmn_rc_md == RC_MD_120_FPS) _t1_ = 0;

    return  _t1_;
}
#endif
//==================================================================================
//==================================================================================
void    RunWndwDsSrOSDArwWn(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
    U8 _t1_;

    if(wndw_ds_sc_fmt == SC_FMT_8CH)    _t1_ = OSD_MIX;
    else    _t1_ = 0x00;

    RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_SR_DEC_WN_X,WNDW_DS_SR_INC_WN_X,WNDW_DS_SR_NUM_WN_Y,MSE_ON_OSD_DS_SR_WN);
}
//==================================================================================
void    RunWndwDsSrOSDArwLv(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
    U8 _t1_;
    
//  if((b_ch_src == CH_SRC_PB)||(wndw_ds_sc_fmt == SC_FMT_8CH)) _t1_ = OSD_MIX;
    if((GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2))||(wndw_ds_sc_fmt == SC_FMT_8CH)) //... (PB_PATH_EN=1)||(8ch)
        _t1_ = OSD_MIX;
    else    _t1_ = 0x00;
    
    RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_SR_DEC_LV_X,WNDW_DS_SR_INC_LV_X,WNDW_DS_SR_NUM_LV_Y,MSE_ON_OSD_DS_SR_LV);
}
//==================================================================================
void    RunWndwDsSrOSDArwPb(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
    U8 _t1_;
/*
#if defined(__4CH__)
    if((b_ch_src == CH_SRC_LV)||(wndw_ds_sc_fmt == SC_FMT_8CH)||(GLBL_PB_MD == PB_MD_FLTNG))    _t1_ = OSD_MIX;
#elif   defined(__16CH__)
    if((b_ch_src == CH_SRC_LV)||(wndw_ds_sc_fmt == SC_FMT_8CH)) _t1_ = OSD_MIX;
#endif
//  */
//  if((b_ch_src == CH_SRC_LV)||(wndw_ds_sc_fmt == SC_FMT_8CH)) _t1_ = OSD_MIX;
    if((!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)))||(wndw_ds_sc_fmt == SC_FMT_8CH))  //... (PB_PATH_EN=0)||(8ch)
        _t1_ = OSD_MIX;
    else    _t1_ = 0x00;
    
    RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_SR_DEC_PB_X,WNDW_DS_SR_INC_PB_X,WNDW_DS_SR_NUM_PB_Y,MSE_ON_OSD_DS_SR_PB);
}
//==================================================================================
//==================================================================================
void    SetWndwDsSrOSDWn(U8 _pth)
{
    U8 _t1_, _t2_;

    if(wndw_ds_sc_fmt == SC_FMT_8CH)    _t2_ = OSD_MIX;
    else    _t2_ = 0x00;

    WriteOSDStr(_pth,MASTER,WNDW_DS_SR_RDO_X,WNDW_DS_SR_NUM_WN_Y,_t2_,str_wndw_ch_wn);

#if defined(__16CH__)
    if(wndw_pop_crnt_ch < 10)   _t1_ = OSD_CLR;
    else    _t1_ = _t2_;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_WN_X-1,WNDW_DS_SR_NUM_WN_Y,_t1_,0x11);
#endif
    if(wndw_pop_crnt_ch > 9)        _t1_ = wndw_pop_crnt_ch+6;          //... +6 -> +0x10 -10;
    else    _t1_ = wndw_pop_crnt_ch+0x10;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_WN_X,WNDW_DS_SR_NUM_WN_Y,_t2_,_t1_);
}
//==================================================================================
//==================================================================================
//U8    GetWndwDsSrLv(void)
//{
//  U8 _t1_;
//
//  _t1_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch);
//
//  return  (_t1_&0x03);
//}
////==================================================================================
//void  SetWndwDsSrLv(U8 _data)
//{
//  
////    SetDsSrFuncMode(_pth,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_STRB);
////    SetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x22+0x40*wndw_crnt_ch,0x03,_data);
//  SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x10);     //... FUNC_MODE=1
//  SetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0xc0,_data<<6);
//  
////    SetDsSrFuncMode(_pth,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_NRML);
//  SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x00);     //... FUNC_MODE=0
//}
//==================================================================================
void    SetWndwDsSrOSDLvNum(U8 _pth, U8 _data)
{
    U8 _t1_;
#if defined(__16CH__)
    U8 _t2_;
#endif
    
//  if((b_ch_src == CH_SRC_PB)||(wndw_ds_sc_fmt == SC_FMT_8CH)) _t1_ = OSD_MIX;
//  else    _t1_ = 0x00;
    if((GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2))||(wndw_ds_sc_fmt == SC_FMT_8CH)) //... (PB_PATH_EN=1)||(8ch)
        _t1_ = OSD_MIX;
    else    _t1_ = 0x00;
    
#if defined(__4CH__)
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_LV_X,WNDW_DS_SR_NUM_LV_Y,_t1_,_data+0x10);
#elif   defined(__16CH__)
    _data += (wndw_pop_crnt_ch&0x0c);
    if(_data > 9)   _t2_ = _data+6;         //... +6 -> +0x10 -10;
    else    _t2_ = _data+0x10;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_LV_X,WNDW_DS_SR_NUM_LV_Y,_t1_,_t2_);
    if(_data < 10)  _t1_ = OSD_CLR;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_LV_X-1,WNDW_DS_SR_NUM_LV_Y,_t1_,0x11);
#endif
}
//==================================================================================
void    SetWndwDsSrOSDLv(U8 _pth)
{
    U8 _t1_;//, _t2_;
    
    WriteOSDStr(_pth,MASTER,WNDW_DS_SR_RDO_X+2,WNDW_DS_SR_RDO_LV_Y,0x00,str_wndw_ch_live_vid);

    if(wndw_ds_sc_fmt == SC_FMT_8CH){
        if(b_ch_src == CH_SRC_LV)   _t1_ = OSD_RDO_CHK;
        else    _t1_ = OSD_RDO_BLK;
    }
    else{
    //  if(b_ch_src == CH_SRC_LV){
        if(!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)))            //... PB_PATH_EN=0
            _t1_ = OSD_RDO_CHK;
        else    _t1_ = OSD_RDO_BLK;
    }
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_RDO_X,WNDW_DS_SR_RDO_LV_Y,0x00,_t1_);

//  _t1_ = GetWndwDsSrLv();
    _t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0x62);
    SetWndwDsSrOSDLvNum(_pth,_t1_);
}
//==================================================================================
//==================================================================================
//U8    GetWndwDsSrPb(void)
//{
//  U8 _t1_;
//
////    if(b_jp_4pb == ON){
////        _t1_ = ReadAsicByte(MASTER,DVC_PG0,0x39);
////        _t1_ &= (0x03<<(wndw_crnt_ch<<1));
////        _t1_ >>= (wndw_crnt_ch<<1);
////    }
////    else{
//      _t1_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3));
//      _t1_ &= 0x0f;
////    }
//
//  return  _t1_;
//}
////==================================================================================
//void  SetWndwDsSrPb(U8 _data)
//{
//  U8 _rdat_;
//
////    if(b_jp_4pb == ON){
////        _rdat_ = ReadAsicByte(MASTER,DVC_PG0,0x39);
////        _rdat_ &= ~(0x03<<(wndw_crnt_ch<<1));
////        _rdat_ |= (_data<<(wndw_crnt_ch<<1));
////        WriteAsicByte(MASTER,DVC_PG0,0x39,_rdat_);
////    }
////    else{
//      _rdat_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3));
//      WriteAsicByte(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),(_rdat_ & ~0x0f)|_data);
////    }
//}
//==================================================================================
void    SetWndwDsSrOSDPbNum(U8 _pth, U8 _data)
{
    U8 _t1_, _t2_;
    
//  if((b_ch_src == CH_SRC_LV)||(wndw_ds_sc_fmt == SC_FMT_8CH)) _t1_ = OSD_MIX;
//  else    _t1_ = 0x00;
    if((!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)))||(wndw_ds_sc_fmt == SC_FMT_8CH))  //... (PB_PATH_EN=0)||(8ch)
        _t1_ = OSD_MIX;
    else    _t1_ = 0x00;
    
#if defined(__16CH__)
    if(_data < 10)  _t2_ = OSD_CLR;
    else    _t2_ = _t1_;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_PB_X-1,WNDW_DS_SR_NUM_PB_Y,_t2_,0x11);
#endif
    if(_data > 9)       _t2_ = _data+6;         //... +6 -> +0x10 -10;
    else    _t2_ = _data+0x10;
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_NUM_PB_X,WNDW_DS_SR_NUM_PB_Y,_t1_,_t2_);
}
//==================================================================================
void    SetWndwDsSrOSDPb(U8 _pth)
{
    U8 _t1_;//, _t2_;
///*
//#if   defined(__4CH__)
//  if(GLBL_PB_MD == PB_MD_FLTNG)   _t2_ = OSD_MIX;
//  else    _t2_ = 0x00;
//#elif defined(__16CH__)
//  _t2_ = 0x00;
//#endif
////    */
//  _t2_ = 0x00;

    WriteOSDStr(_pth,MASTER,WNDW_DS_SR_RDO_X+2,WNDW_DS_SR_RDO_PB_Y,0x00,str_wndw_ch_pb);

    if(wndw_ds_sc_fmt == SC_FMT_8CH){
        if(b_ch_src == CH_SRC_PB)   _t1_ = OSD_RDO_CHK;
        else    _t1_ = OSD_RDO_BLK;
    }
    else{
        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2))           //... PB_PATH_EN=1
            _t1_ = OSD_RDO_CHK;
        else    _t1_ = OSD_RDO_BLK;
    }
    
    WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_RDO_X,WNDW_DS_SR_RDO_PB_Y,0x00,_t1_);

//  _t1_ = GetWndwDsSrPb();
    _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),0x0f);
    SetWndwDsSrOSDPbNum(_pth,_t1_);
}
//==================================================================================
//==================================================================================
//U8    GetWndwDsSrEnh(void)
//{
//  return  GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3);
//}
////==================================================================================
//void  SetWndwDsSrEnh(U8 _tgl)
//{
////    SetDsSrCtrl(_pth,wndw_crnt_dvc,wndw_crnt_ch,BIT3,_tgl);
//  SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3,_tgl<<3);
//}
//==================================================================================
void    SetWndwDsSrOSDEnhChk(U8 _pth)
{
    U8 _t1_;
    
    _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3);
    if(_t1_ == OFF)     _t1_ = OSD_CHK_BOX_BLK;
    else    _t1_ = OSD_CHK_BOX_CHK;
    
    if(wndw_ds_sc_fmt == SC_FMT_8CH)
        WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_CHK_X,WNDW_DS_SR_CHK_Y,OSD_MIX,_t1_);
    else
        WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_CHK_X,WNDW_DS_SR_CHK_Y,0x00,_t1_);
}
//==================================================================================
void    SetWndwDsSrOSDEnh(U8 _pth)
{
    U8 _t1_;//, _t2_;
    
    if(wndw_ds_sc_fmt == SC_FMT_8CH)    _t1_ = OSD_MIX;
    else    _t1_ = 0x00;
    
    WriteOSDStr(_pth,MASTER,WNDW_DS_SR_CHK_X+2,WNDW_DS_SR_CHK_Y,_t1_,str_wndw_ch_en_enh);

////    _t1_ = GetWndwDsSrEnh();
//  _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3)^BIT3;
//  if(_t1_ == OFF)     _t1_ = OSD_CHK_BOX_BLK;
//  else    _t1_ = OSD_CHK_BOX_CHK;
//  WriteOSDFnt(_pth,MASTER,WNDW_DS_SR_CHK_X,WNDW_DS_SR_CHK_Y,_t2_,_t1_);
    
    SetWndwDsSrOSDEnhChk(_pth);
}
//==================================================================================
//==================================================================================
//void  SelWndwDsSrSrc(void)
//{
//  U8 _t1_;
//
//  if(wndw_ds_sc_fmt == SC_FMT_8CH){
//      if(b_ch_src == CH_SRC_PB){
//          SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);                  				//... STRB_FLD=2
////            SetAsicFlgType(MASTER,DVC_PG1,0x16,BIT7,BIT7);                  				//... PB_AUTO_EN=1
//      }
//      else{
//          SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x00);                  				//... STRB_FLD=0
////            SetAsicFlgType(MASTER,DVC_PG1,0x16,BIT7,0x00);                  				//... PB_AUTO_EN=0
//      }
//  }
//  else{
//  //  SetDsSrFuncMode(_pth,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_STRB);
//      SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x10);     //... FUNC_MODE=1
//      WaitChStrbDoneX(BIT0<<wndw_crnt_ch);
//  
//      if(b_ch_src == CH_SRC_LV)   _t1_ = 0x00;
//      else    _t1_ = BIT2;//BIT0<<wndw_crnt_ch;
//  //  SetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x38,BIT0<<wndw_crnt_ch,_t1_);
//      SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2,_t1_);     //... PB_PATH_EN=_t1_
//  	
//  //  if((b_ch_src == CH_SRC_LV)||(b_jp_4pb == ON))
//  //      SetDsSrFuncMode(_pth,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_NRML);	
//      if(b_ch_src == CH_SRC_LV)
//          SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x00);     //... FUNC_MODE=0
//  }
//}
//==================================================================================
//==================================================================================
void    CreateWndwDsSr(U8 _pth)
{
//  b_status_wndw = ON;
    b_cmn_wndw_mn = ON;

    WriteOSDClr(_pth,MASTER,WNDW_DS_SR_POS_X,WNDW_DS_SR_POS_Y,WNDW_DS_SR_L,WNDW_DS_SR_H);
    SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_DS_SR_POS_X,WNDW_DS_SR_POS_Y,WNDW_DS_SR_L,WNDW_DS_SR_H,str_wndw_ttl_ch_stp);

//  if(wndw_ds_sc_fmt != SC_FMT_8CH){
////        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x38,BIT0<<wndw_crnt_ch) == OFF)    b_ch_src = CH_SRC_LV;
//      if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2) == OFF)    b_ch_src = CH_SRC_LV;
//      else    b_ch_src = CH_SRC_PB;
//  }

    mse_flg_on_osd = MSE_ON_OSD_DS_SR_WN|MSE_ON_OSD_DS_SR_LV|MSE_ON_OSD_DS_SR_PB;
    
    RunWndwDsSrOSDArwWn(_pth,0,0);
    SetWndwDsSrOSDWn(_pth);
    SetOSDGrp(_pth,WNDW_DS_SR_RDO_X-1,WNDW_DS_SR_RDO_LV_Y,16,4,0x00,str_wndw_grp_src);
    RunWndwDsSrOSDArwLv(_pth,0,0);
    SetWndwDsSrOSDLv(_pth);
    RunWndwDsSrOSDArwPb(_pth,0,0);
    SetWndwDsSrOSDPb(_pth);
    SetWndwDsSrOSDEnh(_pth);
}
//==================================================================================
void    RunWndwDsSrDec(U8 _pth, U8 _osd_pos_y)
{
    U8 _wt1_, _ofst_;
    U8 _t1_;
    
    if(_osd_pos_y == WNDW_DS_SR_NUM_WN_Y){
        if(wndw_pop_crnt_ch > 0)        wndw_pop_crnt_ch--;
        else    wndw_pop_crnt_ch = cmn_max_ch-1;
        SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

////        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x38,BIT0<<wndw_crnt_ch) == OFF)    b_ch_src = CH_SRC_LV;
//      if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2) == OFF)    b_ch_src = CH_SRC_LV;
//      else    b_ch_src = CH_SRC_PB;

        SetWndwDsSrOSDWn(_pth);
        SetWndwDsSrOSDLv(_pth);
        SetWndwDsSrOSDPb(_pth);
        SetWndwDsSrOSDEnhChk(_pth);
    }
    else if(_osd_pos_y == WNDW_DS_SR_NUM_LV_Y){
//      if(b_ch_src == CH_SRC_LV){
        if(!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2))){           //... PB_PATH_EN=0
            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x10);     //... FUNC_MODE=1
            WaitChStrbDoneX(BIT0<<wndw_pop_crnt_ch);

//          _t1_ = GetWndwDsSrLv();
            _t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0x62);
            if(_t1_ > 0)	_t1_--;
            else    _t1_ = 3;
//          SetWndwDsSrLv(_t1_);
            SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0x62,_t1_);

            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x00);     //... FUNC_MODE=0

            SetWndwDsSrOSDLv(_pth);
    	}
    }
    else if(_osd_pos_y == WNDW_DS_SR_NUM_PB_Y){
        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)){          //... PB_PATH_EN=1
#if defined(__4CH__)
            _wt1_ = 3;
            _ofst_ = 0;
#elif   defined(__16CH__)
            _wt1_ = ((cmn_max_ch>>cmn_rc_md)-1);
            _ofst_ = GetWndwDsSrPbOfst();
#endif
//          _t1_ = GetWndwDsSrPb();
            _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),0x0f);
            if(_t1_ > _ofst_)	_t1_--;
            else    _t1_ = (_ofst_+_wt1_);
//          SetWndwDsSrPb(_t1_);
            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),0x0f,_t1_);
            SetWndwDsSrOSDPb(_pth);
    	}
    }
//      if(b_ch_src == CH_SRC_LV){
//          if(_osd_pos_y == WNDW_DS_SR_NUM_LV_Y){
//              _t1_ = GetWndwDsSrLv();
//              if(_t1_ > 0)	_t1_--;
//              else	_t1_ = 3;
//              SetWndwDsSrLv(_t1_);
//              SetWndwDsSrOSDLv(_pth);
//      	}
//      }
//      else{
//          if(_osd_pos_y == WNDW_DS_SR_NUM_PB_Y){
//#if   defined(__4CH__)
//              _wt1_ = 3;
//              _ofst_ = 0;
//#elif defined(__16CH__)
//              _wt1_ = ((cmn_max_ch>>cmn_rc_md)-1);
//              _ofst_ = GetWndwDsSrPbOfst();
//#endif
//              _t1_ = GetWndwDsSrPb();
//              if(_t1_ > _ofst_)	_t1_--;
//              else    _t1_ = (_ofst_+_wt1_);
//              SetWndwDsSrPb(_t1_);
//              SetWndwDsSrOSDPb(_pth);
//      	}
//      }
//  }
}
//==================================================================================
void    RunWndwDsSrInc(U8 _pth, U8 _osd_pos_y)
{
    U8 _t1_;//, _t2_;
    U8 _wt1_, _ofst_;

    if(_osd_pos_y == WNDW_DS_SR_NUM_WN_Y){
        if(wndw_pop_crnt_ch < (cmn_max_ch-1))   wndw_pop_crnt_ch++;
        else    wndw_pop_crnt_ch = 0;
        SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

////        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG0,0x38,BIT0<<wndw_crnt_ch) == OFF)    b_ch_src = CH_SRC_LV;
//      if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2) == OFF)    b_ch_src = CH_SRC_LV;
//      else    b_ch_src = CH_SRC_PB;

        SetWndwDsSrOSDWn(_pth);
        SetWndwDsSrOSDLv(_pth);
        SetWndwDsSrOSDPb(_pth);
        SetWndwDsSrOSDEnhChk(_pth);
    }
//  else{
//      if(b_ch_src == CH_SRC_LV){
//          if(_osd_pos_y == WNDW_DS_SR_NUM_LV_Y){
//              _t1_ = GetWndwDsSrLv();
//              if(_t1_ < 3)	_t1_++;
//              else	_t1_ = 0;
//              SetWndwDsSrLv(_t1_);
//              SetWndwDsSrOSDLv(_pth);
//      	}
//      }
//      else{
//          if(_osd_pos_y == WNDW_DS_SR_NUM_PB_Y){
//#if   defined(__4CH__)
//              _wt1_ = 3;
//              _ofst_ = 0;
//#elif defined(__16CH__)
//              _wt1_ = ((cmn_max_ch>>cmn_rc_md)-1);
//              _ofst_ = GetWndwDsSrPbOfst();
//#endif
//              _t1_ = GetWndwDsSrPb();
//              if(_t1_ < (_ofst_+_wt1_))	_t1_++;
//              else    _t1_ = _ofst_;
//              SetWndwDsSrPb(_t1_);
//              SetWndwDsSrOSDPb(_pth);
//      	}
//      }
//  }
    else if(_osd_pos_y == WNDW_DS_SR_NUM_LV_Y){
//      if(b_ch_src == CH_SRC_LV){
        if(!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2))){           //... PB_PATH_EN=0
            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x10);     //... FUNC_MODE=1
            WaitChStrbDoneX(BIT0<<wndw_pop_crnt_ch);

//          _t1_ = GetWndwDsSrLv();
            _t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0x62);
            if(_t1_ < 3)	_t1_++;
            else    _t1_ = 0;
//          SetWndwDsSrLv(_t1_);
            SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x80+0x10*wndw_crnt_ch,0x62,_t1_);

            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30,0x00);     //... FUNC_MODE=0

            SetWndwDsSrOSDLv(_pth);
    	}
    }
    else if(_osd_pos_y == WNDW_DS_SR_NUM_PB_Y){
        if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)){          //... PB_PATH_EN=1
#if defined(__4CH__)
            _wt1_ = 3;
            _ofst_ = 0;
#elif   defined(__16CH__)
            _wt1_ = ((cmn_max_ch>>cmn_rc_md)-1);
            _ofst_ = GetWndwDsSrPbOfst();
#endif
//          _t1_ = GetWndwDsSrPb();
            _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),0x0f);
            if(_t1_ < (_ofst_+_wt1_))	_t1_++;
            else    _t1_ = _ofst_;
//          SetWndwDsSrPb(_t1_);
            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16+(wndw_crnt_ch<<3),0x0f,_t1_);
            SetWndwDsSrOSDPb(_pth);
    	}
    }
}
//==================================================================================
void    RunWndwDsSr(U8 _pth)
{
    U8 _t1_;
    U8 _osd_pos_x_, _osd_pos_y_;

    _osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
    _osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

    if(wndw_ds_sc_fmt != SC_FMT_8CH){
        RunWndwDsSrOSDArwWn(_pth,_osd_pos_x_,_osd_pos_y_);
//      if(b_ch_src == CH_SRC_LV)
        if(!(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)))            //... PB_PATH_EN=0
            RunWndwDsSrOSDArwLv(_pth,_osd_pos_x_,_osd_pos_y_);
        else    RunWndwDsSrOSDArwPb(_pth,_osd_pos_x_,_osd_pos_y_);
    }

    if(b_mse_btn_dwn_lb == 1){
        b_mse_btn_dwn_lb = 0;

        if((_osd_pos_x_ == WNDW_DS_SR_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_SR_CLOS_Y)){
//          SetBoxBtnStrPush(BOX_WNDW_QUIT);
    	}
    }
    if(b_mse_btn_dwn_rb == 1)   b_mse_btn_dwn_rb = 0;
    if(b_mse_clk_lb == 1){
        b_mse_clk_lb = 0;

        if((_osd_pos_x_ == WNDW_DS_SR_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_SR_CLOS_Y)){
//          SetBoxBtnStrPull(BOX_WNDW_QUIT);
    		
            WriteOSDClr(_pth,MASTER,WNDW_DS_SR_POS_X,WNDW_DS_SR_POS_Y,WNDW_DS_SR_L,WNDW_DS_SR_H);
            WriteOSDClrX(MASTER,WNDW_DS_SR_POS_X-1,WNDW_DS_SR_POS_Y-1,WNDW_DS_SR_L+2,WNDW_DS_SR_H+2);
//          DisableBox(_pth,BOX_WNDW_QUIT,BOX_WNDW_FRM);    // decoration wndwdow, main wndwdow

            SetOSDNoMn();

//          b_status_wndw = OFF;
            b_cmn_wndw_mn = OFF;
            wndw_mn_pos_crnt = 30;
    	}
        else if(_osd_pos_x_ == WNDW_DS_SR_RDO_X){
            if((_osd_pos_y_ == WNDW_DS_SR_RDO_LV_Y)||(_osd_pos_y_ == WNDW_DS_SR_RDO_PB_Y)){
//#if   defined(__4CH__)
//              if(GLBL_PB_MD != PB_MD_FLTNG){
//#endif
//                  if(_osd_pos_y_ == WNDW_DS_SR_RDO_LV_Y)  b_ch_src = CH_SRC_LV;
//          		else{
//                      if(wndw_ds_in_type != IN_TYPE_DGTL) b_ch_src = CH_SRC_PB;
//      			}
                if(wndw_ds_sc_fmt == SC_FMT_8CH){
                    if(_osd_pos_y_ == WNDW_DS_SR_RDO_LV_Y)  _t1_ = CH_SRC_LV;
                    else    _t1_ = CH_SRC_PB;
                    if(_t1_ != b_ch_src){
                        b_ch_src = _t1_;

//                      SetWndwDsSrOSDLv(_pth);
//                      SetWndwDsSrOSDPb(_pth);
//
//                      if(b_ch_src == CH_SRC_PB){
//                          SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);      							//... any field
////                            SetAsicFlgType(MASTER,DVC_PG0,0x38,0x0f,0x0f);      							//... enable pb
//                          SetAsicFlgType(MASTER,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2,BIT2);			//... enable pb
//          			}
//              		else{
//                          SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x00);      							//... odd field
////                            SetAsicFlgType(MASTER,DVC_PG0,0x38,0x0f,0x00);      							//... disable pb
//                          SetAsicFlgType(MASTER,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2,0x00);			//... disable pb
//          			}
//
//                      for(_t1_=0;_t1_<4;_t1_++){
//                          if((b_ch_src == CH_SRC_PB)&&(b_rc_fm_type == FM_TYPE_CIF))
//                              SetSclRto(_pth,MASTER,_t1_,0x23,0x23);
//                          else    SetSclRto(_pth,MASTER,_t1_,0x13,0x23);
//          				
////                            SetAsicFlgType(MASTER,DVC_PG1,REG_FIELD_OP_X,BIT4<<_t1_,BIT4<<_t1_);
//                          SetAsicFlgType(MASTER,DVC_PG1,0x12+(_t1_<<3),BIT5,BIT5);
////                            SetAsicFlgType(MASTER,DVC_PG0,0x15+0x40*_t1_,BIT5,BIT5);                // set 2 for V_ALIAS_X(page0) when FIELD_OP(page1) == 1.
//          			}

//                      SelWndwDsSrSrc();//_pth);
                        if(b_ch_src == CH_SRC_PB){
                            SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);      							//... STRB_FLD=2
                            SetAsicFlgType(MASTER,DVC_PG1,0x16,BIT7,BIT7);      							//... PB_AUTO_EN=1
        				}
            			else{
                            SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x00);      							//... STRB_FLD=0
//                          SetAsicFlgType(MASTER,DVC_PG1,0x16,BIT7,0x00);          						//... PB_AUTO_EN=0
        				}
#if defined(__4CH__)
                        SetWndwDsScFmtPsd8Ch();
#endif
                        SetWndwDsSrOSDLv(_pth);
                        SetWndwDsSrOSDPb(_pth);
        			}
        		}
            	else{
                    if(_osd_pos_y_ == WNDW_DS_SR_RDO_LV_Y)	_t1_ = 0x00;
                    else	_t1_ = BIT2;
                    if(_t1_ != GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),BIT2)){
                        if(_t1_){                   													//... PB_PATH_EN=1
//                          SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x01,0x03,0x02);       							//... STRB_FLD=2
                            if(wndw_rc_fm_type == CIF_TYPE_FR)
                                SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x01,0x03,0x03);           //... STRB_FLD=3		(because AUTO_ENHANCE=1)
            				else
                                SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2
                            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x16,BIT7,BIT7);       							//... PB_AUTO_EN=1
                            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30|BIT2,0x10|BIT2);   //... FUNC_MODE=1,PB_PATH_EN=1

                            Disable2DBoxDsply(wndw_crnt_dvc,wndw_crnt_ch);
        				}
            			else{
//                          WaitChStrbDoneX(BIT0<<wndw_crnt_ch);
                            SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x10+(wndw_crnt_ch<<3),0x30|BIT2,0x00);    //... FUNC_MODE=0,PB_PATH_EN=0
        				}


//                      SelWndwDsSrSrc();//_pth);

                        InitChScaleAdapt(wndw_crnt_dvc,wndw_crnt_ch);

                        SetWndwDsSrOSDLv(_pth);
                        SetWndwDsSrOSDPb(_pth);
                        mse_flg_on_osd = MSE_ON_OSD_DS_SR_LV|MSE_ON_OSD_DS_SR_PB;
                        RunWndwDsSrOSDArwLv(_pth,_osd_pos_x_,_osd_pos_y_);
                        RunWndwDsSrOSDArwPb(_pth,_osd_pos_x_,_osd_pos_y_);
        			}
        		}
        			
//#if   defined(__4CH__)
//      		}
//#endif
    		}
    	}
        else if(wndw_ds_sc_fmt != SC_FMT_8CH){
            if(_osd_pos_x_ == WNDW_DS_SR_CHK_X){
                if(_osd_pos_y_ == WNDW_DS_SR_CHK_Y){
//                  _t1_ = GetWndwDsSrEnh();
                    _t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3)^BIT3;
//                  if(_t1_ == OFF)	_t1_ = ON;
//                  else	_t1_ = OFF;
//                  SetWndwDsSrEnh(_t1_);
                    SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x11+(wndw_crnt_ch<<3),BIT3,_t1_);
                    SetWndwDsSrOSDEnhChk(_pth);
        		}
    		}
            else if((_osd_pos_x_ == WNDW_DS_SR_DEC_WN_X)||(_osd_pos_x_ == WNDW_DS_SR_INC_WN_X)){
                if(_osd_pos_y_ == WNDW_DS_SR_NUM_WN_Y){
                    if(_osd_pos_x_ == WNDW_DS_SR_DEC_WN_X)  RunWndwDsSrDec(_pth,_osd_pos_y_);
                    else    RunWndwDsSrInc(_pth,_osd_pos_y_);

                    mse_flg_on_osd = MSE_ON_OSD_DS_SR_LV|MSE_ON_OSD_DS_SR_PB;
                    RunWndwDsSrOSDArwLv(_pth,_osd_pos_x_,_osd_pos_y_);
                    RunWndwDsSrOSDArwPb(_pth,_osd_pos_x_,_osd_pos_y_);
        		}
    		}
            else if((_osd_pos_x_ == WNDW_DS_SR_DEC_LV_X)||(_osd_pos_x_ == WNDW_DS_SR_INC_LV_X)){
                if((_osd_pos_y_ == WNDW_DS_SR_NUM_LV_Y)||(_osd_pos_y_ == WNDW_DS_SR_NUM_PB_Y)){
                    if(_osd_pos_x_ == WNDW_DS_SR_DEC_LV_X)  RunWndwDsSrDec(_pth,_osd_pos_y_);
                    else    RunWndwDsSrInc(_pth,_osd_pos_y_);
        		}
    		}
    	}
    }
    if(b_mse_clk_rb == 1)   b_mse_clk_rb = 0;
    if(b_mse_pshng_clk_lb == 1){
        b_mse_pshng_clk_lb = 0;

        if(wndw_ds_sc_fmt != SC_FMT_8CH){
            if((_osd_pos_x_ == WNDW_DS_SR_DEC_WN_X)||(_osd_pos_x_ == WNDW_DS_SR_INC_WN_X)){
                if(_osd_pos_y_ == WNDW_DS_SR_NUM_WN_Y){
                    if(_osd_pos_x_ == WNDW_DS_SR_DEC_WN_X)  RunWndwDsSrDec(_pth,_osd_pos_y_);
                    else    RunWndwDsSrInc(_pth,_osd_pos_y_);

                    mse_flg_on_osd = MSE_ON_OSD_DS_SR_LV|MSE_ON_OSD_DS_SR_PB;
                    RunWndwDsSrOSDArwLv(_pth,_osd_pos_x_,_osd_pos_y_);
                    RunWndwDsSrOSDArwPb(_pth,_osd_pos_x_,_osd_pos_y_);
        		}
    		}
            else if((_osd_pos_x_ == WNDW_DS_SR_DEC_LV_X)||(_osd_pos_x_ == WNDW_DS_SR_INC_LV_X)){
                if((_osd_pos_y_ == WNDW_DS_SR_NUM_LV_Y)||(_osd_pos_y_ == WNDW_DS_SR_NUM_PB_Y)){
                    if(_osd_pos_x_ == WNDW_DS_SR_DEC_LV_X)  RunWndwDsSrDec(_pth,_osd_pos_y_);
                    else    RunWndwDsSrInc(_pth,_osd_pos_y_);
        		}
    		}
    	}
    }
    if(b_mse_pshng_clk_rb == 1) b_mse_pshng_clk_rb = 0;
}
//==================================================================================
void    HwTW2835Playback(U8 _dvc, U8 _pth, U8 PB_Mode, U8 PB_OutChannel, U8 PB_InChannel)
{
        U8  vertical_offset;
        U16 i,j;
        U8 reg_addr[3];        
        extern int D1_ModeFlag;
    
            
       if(_pth &0x80) 
       {   
            SetAsicFlgType(MASTER,DVC_PG1,0x10+(PB_OutChannel<<3),BIT2|0x3,0|PB_OutChannel); 
            debug("\ndisable playback channel %d\n",PB_OutChannel);
       }
      else
       {
            if(b_cmn_jp_vdo == VDO_PAL)
                vertical_offset = 144;
            else
                vertical_offset = 120;
            reg_addr[0] = 0x8b + (PB_OutChannel<<4);
            reg_addr[1] = 0x8d + (PB_OutChannel<<4);
            reg_addr[2] = 0x8f + (PB_OutChannel<<4);
            
            switch(PB_InChannel){
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
         
          //SetAsicFlgType(MASTER, DVC_PG1, 0x10+(PB_OutChannel<<3), 0x40,0);
          SetAsicFlgType(MASTER, DVC_PG1, 0x10+(PB_OutChannel<<3), 0xfc,0x84);
          //SetAsicFlgType(MASTER, DVC_PG1, 0x16+(PB_OutChannel<<3), 0x3,PB_InChannel);

          SetAsicFlgType(MASTER,DVC_PG1,0x10+(0<<3),0x3,0x00);
          SetAsicFlgType(MASTER,DVC_PG1,0x10+(1<<3),0x3,0x01);
          SetAsicFlgType(MASTER,DVC_PG1,0x10+(2<<3),0x3,0x02);
          SetAsicFlgType(MASTER,DVC_PG1,0x10+(3<<3),0x3,0x03);
      }
   /*  if(D1_ModeFlag) //if  D1 mode disable the boundary
     {
         j= 0;  
         debug("\n D1 mode disable the boundary\n");
     }
     else j = BIT1;                //else enable the boundary
     SetAsicFlgType(MASTER,DVC_PG1,0x11,BIT1,j);
     SetAsicFlgType(MASTER,DVC_PG1,0x19,BIT1,j);
     SetAsicFlgType(MASTER,DVC_PG1,0x21,BIT1,j);
     SetAsicFlgType(MASTER,DVC_PG1,0x29,BIT1,j);
    */
       for(i=0;i<4;i++)
            debug("\n ch%d  %x   \n",i,ReadAsicByte(MASTER, DVC_PG1, 0x10+(i<<3)));

}

