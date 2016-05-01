#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_mtn_md.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "wndw_rc_fmt.h"

#include "hw2835_md.h"
#include "video_cfg.h"
#include "regs.h"
#include "tw2835_api.h"

#include "gpio.h"
#include "hw2835_md.h"
//==================================================================================
U8 wndw_mtn_md_blck_strt;

unsigned char md_set_ch = 0;
//U8 bdata	wndw_flag_mtn = 0x00;
//sbit	b_mtn_dspl = wndw_flag_mtn^7;


//==================================================================================
//						Motion and 2Dbox window function description
//==================================================================================
void Enable2DBox(U8 _pth, U8 _dvc, U8 _ch)
{
//	U8	_rdat_;
//
//	_rdat_ = ReadAsicByte(_dvc,DVC_PG2,0x60+(_ch<<3));
//	ClearBit(_rdat_,_2DBOX_EN_X|_2DBOX_EN_Y);
//	WriteAsicByte(_dvc,DVC_PG2,0x60+(_ch<<3),_path|_rdat_);
	U8 _t1_=0;
	
	if(BitSet(_pth,PTH_X))	_t1_ |= BIT7;
	if(BitSet(_pth,PTH_Y))	_t1_ |= BIT6;
	SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),0xc0,_t1_);
}
//==================================================================================
void Disable2DBoxDsply(U8 _dvc, U8 _ch)
{
	SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5|BIT4,0x00);					//... SBOX_EN=0
	SetAsicFlgType(_dvc,DVC_PG2,0x61+(_ch<<3),BIT5,0x00);
}
//==================================================================================
/*
void	Set2DBoxCtrl(U8 _dvc, U8 _ch, U8 _property, U8 _tgl)
{
	U8 t_rdata;

	t_rdata = ReadAsicByte(_dvc,DVC_PG2,0x60+(_ch<<3));
	if(BitSet(_tgl,0x08)){
		t_rdata = (t_rdata & 0xcf)|(_tgl & 0x30);
	}
	else{
		if(_tgl == ON)	SetBit(t_rdata,_property);
		else	ClearBit(t_rdata,_property);
	}
	WriteAsicByte(_dvc,DVC_PG2,0x60+(_ch<<3),t_rdata);
}
//	*/
//==================================================================================
void Aply2DBoxCh(U8 _dvc, U8 _ch)
{
	U8 _hl_, _hr_, _vt_, _vb_;
	U8 _t1_, _t2_;

	_hl_ = ReadAsicByte(_dvc,DVC_PG1,0x30+(_ch<<2));
	_hr_ = ReadAsicByte(_dvc,DVC_PG1,0x31+(_ch<<2));
	_vt_ = ReadAsicByte(_dvc,DVC_PG1,0x32+(_ch<<2));
	_vb_ = ReadAsicByte(_dvc,DVC_PG1,0x33+(_ch<<2));

	//... 2d box setting
	WriteAsicByte(_dvc,DVC_PG2,0x62+(_ch<<3),_hl_+1);
	WriteAsicByte(_dvc,DVC_PG2,0x64+(_ch<<3),_vt_);
	_t1_ = (_hr_-_hl_-1)>>3;
	if(_t1_ > 1)	_t2_ = _t1_ - 1;
	else	_t2_ = 0;
	WriteAsicByte(_dvc,DVC_PG2,0x63+(_ch<<3),_t2_);
	//... (HW + 1(md_bnd))*(16 / 2(resol)) = HR - HL - 1(ch_bnd) - pixel_offset(full:4,quad:2,pip:1... -> not contain)
	_t1_ = (_vb_-_vt_)/6;
	if(_t1_ > 1)	_t2_ = _t1_ - 1;
	else	_t2_ = 0;
	WriteAsicByte(_dvc,DVC_PG2,0x65+(_ch<<3),_t2_);
}
//==================================================================================
void Set2DBoxPos(U8 _dvc, U8 _ch, U16 _boxhl, U16 _boxvt)
{
	U8	_rdat_;

	_rdat_ = ReadAsicByte(_dvc,DVC_PG2,0x61+(_ch<<3));
	if(BitSet(_boxhl, BIT0))	SetBit(_rdat_, BIT1);
	else	ClearBit(_rdat_, BIT1);
	if(BitSet(_boxvt, BIT0))	SetBit(_rdat_, BIT0);
	else	ClearBit(_rdat_, BIT0);

	WriteAsicByte(_dvc,DVC_PG2,0x61+(_ch<<3),_rdat_);
	WriteAsicByte(_dvc,DVC_PG2,0x62+(_ch<<3),(U8)(_boxhl>>1));
	WriteAsicByte(_dvc,DVC_PG2,0x64+(_ch<<3),(U8)(_boxvt>>1));
}
//==================================================================================
void Set2DBoxSize(U8 _dvc, U8 _ch, U8 _boxhw, U8 _boxvw)
{
	WriteAsicByte(_dvc,DVC_PG2,0x63+(_ch<<3),_boxhw);
	WriteAsicByte(_dvc,DVC_PG2,0x65+(_ch<<3),_boxvw);
}
//==================================================================================
void Set2DBox(U8 _dvc, U8 _ch, U16 _boxhl, U8 _boxhw, U16 _boxvt, U8 _boxvw)
{
	Set2DBoxPos(_dvc,_ch,_boxhl,_boxvt);
	Set2DBoxSize(_dvc,_ch,_boxhw,_boxvw);
}
//==================================================================================
void Set2DBoxCell(U8 _dvc, U8 _ch, U8 _lngth, U8 _hght)
{
	WriteAsicByte(_dvc,DVC_PG2,0x66+(_ch<<3),(--_lngth)<<4|(--_hght));
}
//==================================================================================
//void Set2DBoxCur(U8 _dvc, U8 _ch, U8 _pos_x, U8 _pos_y)
//{
//	WriteAsicByte(_dvc,DVC_PG2,0x67+(_ch<<3),(_pos_x<<4)|_pos_y);
//}
//==================================================================================
void	Set2DBoxTable(U8 _pth, U8 _dvc, U8 _ch, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 _osd_per_cell)
{
	Set2DBox(_dvc,_ch,(_osd_pos_x<<3)-4,7+(_osd_per_cell<<3),(_osd_pos_y*10)+OSD_DLY_V-1,9);
	Set2DBoxCell(_dvc,_ch,_lngth,_hght);
	
	Enable2DBox(_pth,_dvc,_ch);
}
//==================================================================================
void	SetMskBlck(U8 _dvc, U8 _ch, U8 _blk_hl, U8 _blk_vt, U8 _blk_wdth, U8 _blk_hght)
{
	U16 _msk_;
//	U8	_rdat_;
	U8 _x_, _y_, _pos_end_y_, _pos_end_x_;
	
	_pos_end_y_ = _blk_vt + _blk_hght;
	_pos_end_x_ = _blk_hl + _blk_wdth;

//	_rdat_ = ReadAsicByte(_dvc,DVC_PG2,0x82+0x20*_ch);
//	WriteAsicByte(_dvc,DVC_PG2,0x83+0x20*_ch,_rdat_|BIT7);
	SetAsicFlgType(_dvc,DVC_PG2,0x83+0x20*_ch,0xc0,0x80);			//... MD_MASK_RD_MD=2

	for(_y_ = _blk_vt ; _y_ < _pos_end_y_ ; _y_++){
		_msk_ = 0;
		_msk_ |= ReadAsicByte(_dvc,DVC_PG2,0x86+0x20*_ch+(_y_<<1))<<8;
		_msk_ |= ReadAsicByte(_dvc,DVC_PG2,0x86+0x20*_ch+(_y_<<1)+1);

			for(_x_ = _blk_hl ; _x_ < _pos_end_x_ ; _x_++)	_msk_ ^= BIT0<<_x_;

		WriteAsicByte(_dvc,DVC_PG2,0x86+0x20*_ch+(_y_<<1),(U8)((_msk_&0xff00)>>8));
		WriteAsicByte(_dvc,DVC_PG2,0x86+0x20*_ch+(_y_<<1)+1,(U8)(_msk_&0x00ff));
	}
}
//==================================================================================
//==================================================================================
void	RunWndwMtnMdOSDChArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_CH_DEC_X,WNDW_MTN_MD_CH_INC_X,WNDW_MTN_MD_CH_NUM_Y,MSE_ON_OSD_MD_CH);
}
//==================================================================================
void	RunWndwMtnMdOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_SENS_DEC_X,WNDW_MTN_MD_SENS_INC_X,WNDW_MTN_MD_SENS_CL_Y,MSE_ON_OSD_MD_CL);
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_SENS_DEC_X,WNDW_MTN_MD_SENS_INC_X,WNDW_MTN_MD_SENS_LV_Y,MSE_ON_OSD_MD_LV);
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_SENS_DEC_X,WNDW_MTN_MD_SENS_INC_X,WNDW_MTN_MD_SENS_SP_Y,MSE_ON_OSD_MD_SP);
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_SENS_DEC_X,WNDW_MTN_MD_SENS_INC_X,WNDW_MTN_MD_SENS_TM_Y,MSE_ON_OSD_MD_TM);
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_MD_SENS_DEC_X,WNDW_MTN_MD_SENS_INC_X,WNDW_MTN_MD_SENS_VL_Y,MSE_ON_OSD_MD_VL);
}
//==================================================================================
//==================================================================================
void	SetWndwMtnMdOSDChNum(U8 _pth)
{
	U8 _t1_;

#if	defined(__16CH__)
	if(wndw_pop_crnt_ch < 10)		_t1_ = OSD_CLR;
	else	_t1_ = 0x00;
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_CH_NUM_X-1,WNDW_MTN_MD_CH_NUM_Y,_t1_,0x11);
#endif
	if(wndw_pop_crnt_ch > 9)		_t1_ = wndw_pop_crnt_ch+6;			//... +6 -> +0x10 -10;
	else	_t1_ = wndw_pop_crnt_ch+0x10;
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_CH_NUM_X,WNDW_MTN_MD_CH_NUM_Y,0x00,_t1_);
}
//==================================================================================
void	SetWndwMtnMdOSDCh(U8 _pth)
{
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_POS_X+3,WNDW_MTN_MD_CH_NUM_Y,0x00,str_wndw_cmn_ch);
	RunWndwMtnMdOSDChArw(_pth,0,0);

	SetWndwMtnMdOSDChNum(_pth);
}
//==================================================================================
//==================================================================================
U8	GetWndwMtnMdNum(U8 _osd_pos_y)
{
	U8 _t1_;
	
//	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y)		_t1_ = GetMtnMdSens(wndw_crnt_dvc,wndw_crnt_ch,MD_SENS_CL);
//	else if(_osd_pos_y == WNDW_MTN_MD_SENS_LV_Y)	_t1_ = GetMtnMdSens(wndw_crnt_dvc,wndw_crnt_ch,MD_SENS_LV);
//	else if(_osd_pos_y == WNDW_MTN_MD_SENS_VL_Y)	_t1_ = GetMtnMdSens(wndw_crnt_dvc,wndw_crnt_ch,MD_SENS_VL);
//	else if(_osd_pos_y == WNDW_MTN_MD_SENS_SP_Y)	_t1_ = GetMtnMdIrq(wndw_crnt_dvc,wndw_crnt_ch,MD_IRQ_SP);
//	else if(_osd_pos_y == WNDW_MTN_MD_SENS_TM_Y)	_t1_ = GetMtnMdIrq(wndw_crnt_dvc,wndw_crnt_ch,MD_IRQ_TM);
	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y)		_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x83+0x20*wndw_crnt_ch,0x62);//MD_SENS_CL);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_LV_Y)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x83+0x20*wndw_crnt_ch,0x05);//MD_SENS_LV);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_SP_Y)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x85+0x20*wndw_crnt_ch,0x04);//MD_IRQ_SP);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_TM_Y)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x85+0x20*wndw_crnt_ch,0x44);//MD_IRQ_TM);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_VL_Y)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x84+0x20*wndw_crnt_ch,0x06);//MD_SENS_VL);

	return	_t1_;
}
//==================================================================================
void	SetWndwMtnMdNum(U8 _osd_pos_y, U8 _data)
{
	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y)		SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x83+0x20*wndw_crnt_ch,0x62,_data);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_LV_Y)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x83+0x20*wndw_crnt_ch,0x05,_data);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_SP_Y)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x85+0x20*wndw_crnt_ch,0x04,_data);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_TM_Y)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x85+0x20*wndw_crnt_ch,0x44,_data);
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_VL_Y)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x84+0x20*wndw_crnt_ch,0x06,_data);
}
//==================================================================================
void	SetWndwMtnMdOSDNum(U8 _pth, U8 _osd_pos_y, U8 _data)
{
	_data = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_SENS_NUM_X,_osd_pos_y,0x00,(_data&0x0f)+0x10);
	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y){}
	else{
		_data >>= 4;
		if(_data == 0)	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_SENS_NUM_X-1,_osd_pos_y,OSD_CLR,0x00);
		else	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_SENS_NUM_X-1,_osd_pos_y,0x00,_data+0x10);
	}
}
//==================================================================================
void	SetWndwMtnMdOSDChk(U8 _pth, U8 _osd_pos_y)
{
//	U8 _dvc_, _ch_;
	U8 _t1_;//, _t2_;
	
	if(_osd_pos_y == WNDW_MTN_MD_CHK_MTN_Y)
		_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5);
	else if(_osd_pos_y == WNDW_MTN_MD_CHK_MSK_Y)
		_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5);

	if(_t1_ == BIT5)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_MD_CHK_X,_osd_pos_y,0x00,_t1_);
}
//==================================================================================
//void	SetWndwMtnMdOSDDspl(U8 _pth)
//{
//	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_CHK_X+2,WNDW_MTN_MD_CHK_MTN_Y,0x00,str_wndw_mtn_dspl_mtn);
//	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_CHK_X+2,WNDW_MTN_MD_CHK_MSK_Y,0x00,str_wndw_mtn_dspl_msk);
//
//	SetWndwMtnMdOSDChk(_pth);
//}
//==================================================================================
void	SetWndwMtnMdOSDNumAll(U8 _pth)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnMdNum(WNDW_MTN_MD_SENS_CL_Y);
	SetWndwMtnMdOSDNum(_pth,WNDW_MTN_MD_SENS_CL_Y,_t1_);
	_t1_ = GetWndwMtnMdNum(WNDW_MTN_MD_SENS_LV_Y);
	SetWndwMtnMdOSDNum(_pth,WNDW_MTN_MD_SENS_LV_Y,_t1_);
	_t1_ = GetWndwMtnMdNum(WNDW_MTN_MD_SENS_SP_Y);
	SetWndwMtnMdOSDNum(_pth,WNDW_MTN_MD_SENS_SP_Y,_t1_);
	_t1_ = GetWndwMtnMdNum(WNDW_MTN_MD_SENS_TM_Y);
	SetWndwMtnMdOSDNum(_pth,WNDW_MTN_MD_SENS_TM_Y,_t1_);
	_t1_ = GetWndwMtnMdNum(WNDW_MTN_MD_SENS_VL_Y);
	SetWndwMtnMdOSDNum(_pth,WNDW_MTN_MD_SENS_VL_Y,_t1_);
}
//==================================================================================
void	SetWndwMtnMdOSD(U8 _pth)
{
////	SetOSDGrp(_pth,WNDW_MTN_MD_SENS_TTL_X-1,WNDW_MTN_MD_SENS_CL_Y,18,7,0x00,str_wndw_grp_mtn);
//	SetOSDGrp(_pth,WNDW_MTN_MD_SENS_TTL_X-1,WNDW_MTN_MD_SENS_CL_Y,18,5,0x00,str_null);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_DECO,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_CL_Y,16,5,str_null);

	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_CL_Y,0x00,str_wndw_mtn_cll);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_LV_Y,0x00,str_wndw_mtn_lvl);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_SP_Y,0x00,str_wndw_mtn_sptl);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_TM_Y,0x00,str_wndw_mtn_tmpl);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_SENS_TTL_X,WNDW_MTN_MD_SENS_VL_Y,0x00,str_wndw_mtn_vel);

	RunWndwMtnMdOSDArw(_pth,0,0);

	SetWndwMtnMdOSDNumAll(_pth);

//	SetWndwMtnMdOSDDspl(_pth);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_CHK_X+2,WNDW_MTN_MD_CHK_MTN_Y,0x00,str_wndw_mtn_dspl_mtn);
	WriteOSDStr(_pth,MASTER,WNDW_MTN_MD_CHK_X+2,WNDW_MTN_MD_CHK_MSK_Y,0x00,str_wndw_mtn_dspl_msk);
	SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MTN_Y);
	SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MSK_Y);
}
//==================================================================================
//==================================================================================
void	CreateWndwMtnMd(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

//	ClearOSD(_pth,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,WNDW_MTN_MD_L,WNDW_MTN_MD_H);
//	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,
//		WNDW_MTN_MD_L,WNDW_MTN_MD_H,str_wndw_ttl_mtn_stp);
	WriteOSDClr(_pth,MASTER,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,WNDW_MTN_MD_L,WNDW_MTN_MD_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,WNDW_MTN_MD_L,WNDW_MTN_MD_H,str_wndw_ttl_mtn_md);

	mse_flg_on_osd = 0xff;
	SetWndwMtnMdOSDCh(_pth);
	SetWndwMtnMdOSD(_pth);
//	SetWndwMtnOSDB(_pth);

	if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5))		//... if(MDAREA_EN=1)
		SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,BIT4);	//... SBOX_EN=1 for new channel
}
//==================================================================================
void	RunWndwMtnMdCh(U8 _pth, U8 _osd_pos_x)
{
	U8 _t1_;//, _t2_;
	
//	SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);
	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,0x00);		//... SBOX_EN=0 for old channel

	if(_osd_pos_x == WNDW_MTN_MD_CH_DEC_X){
		if(wndw_pop_crnt_ch > 0)		wndw_pop_crnt_ch--;
		else	wndw_pop_crnt_ch = cmn_max_ch-1;
	}
	else{
		if(wndw_pop_crnt_ch < (cmn_max_ch-1))	wndw_pop_crnt_ch++;
		else	wndw_pop_crnt_ch = 0;
	}

	SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);
	_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5);
//	_t2_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5);
//	if((_t1_ == BIT5)&&(_t2_ == BIT5))
	if(_t1_ == BIT5)
		SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,BIT4);	//... SBOX_EN=1 for new channel

	SetWndwMtnMdOSDChNum(_pth);
	SetWndwMtnMdOSDNumAll(_pth);
//	SetWndwMtnMdOSDDspl(_pth);
	SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MTN_Y);
	SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MSK_Y);
//	SetWndwMtnOSDBNumAll(_pth);
}
//==================================================================================
void	RunWndwMtnMdDec(U8 _pth, U8 _osd_pos_y)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnMdNum(_osd_pos_y);
	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 3;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_LV_Y){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 31;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_SP_Y){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 15;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_TM_Y){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 15;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_VL_Y){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 62;
	}
	SetWndwMtnMdNum(_osd_pos_y,_t1_);
	SetWndwMtnMdOSDNum(_pth,_osd_pos_y,_t1_);
}
//==================================================================================
void	RunWndwMtnMdInc(U8 _pth, U8 _osd_pos_y)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnMdNum(_osd_pos_y);
	if(_osd_pos_y == WNDW_MTN_MD_SENS_CL_Y){
		if(_t1_ < 3)	_t1_++;
		else	_t1_ = 0;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_LV_Y){
		if(_t1_ < 31)	_t1_++;
		else	_t1_ = 0;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_SP_Y){
		if(_t1_ < 15)	_t1_++;
		else	_t1_ = 0;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_TM_Y){
		if(_t1_ < 15)	_t1_++;
		else	_t1_ = 0;
	}
	else if(_osd_pos_y == WNDW_MTN_MD_SENS_VL_Y){
		if(_t1_ < 62)	_t1_++;
		else	_t1_ = 0;
	}
	SetWndwMtnMdNum(_osd_pos_y,_t1_);
	SetWndwMtnMdOSDNum(_pth,_osd_pos_y,_t1_);
}

//==================================================================================
void	HwTW2835Motiondetector(U8 _dvc, U8 _pth, U8 _ch, TW2835_MotionDetector * MotionDetectorParameter )
{
    U8 md_lvsens, md_speedsens, md_tmpsens, md_spsens;	
    U16 start_x, start_y;
    U16 blk_width, blk_height;
    U8 _t1_, _t2_;
    U16 temp1, temp2;
    U16 test_tmp = 0x0ffff;
    U8 reg_temp[12], i;
    U8 * MotionDetectionSet, cAreaHigh, cAreaLow;
    unsigned int temp;

    MotionDetectionSet = MotionDetectorParameter->md_AreaVectorAddr;
    

    md_set_ch = _ch;
    if((MotionDetectorParameter->md_lvsens!=0) || (MotionDetectorParameter->md_speedsens!=0) || (MotionDetectorParameter->md_tmpsens!=0) || (MotionDetectorParameter->md_spsens!=0)){

        gVideoMotionEn |= (1<<_ch);//set the flag enable motion detect--vincent 20008.01.31
        debug("\ngVideoMotionEn:%x\n",gVideoMotionEn);
//-----when init TW2835 ,enable MD&BD,but not set the flag 'gVideoMotionEn' --vincent 20008.02.02
	/*Enable motion detector*/
        //WriteAsicByte(MASTER, DVC_PG2, 0x80+0x20*_ch, 0x17);    //enbale MD & set blind detection    
        //WriteAsicByte(_dvc,DVC_PG1,0x80 + 20*_ch, 0x17);
        /*interrupt polarity*/
        //WriteAsicByte(_dvc,DVC_PG1,0x76, 0);
        
        /* Level Sensivitity */
        WriteAsicByte(_dvc,DVC_PG2,0x83+0x20*_ch,(MotionDetectorParameter->md_lvsens - 1));
        /* Spatial Sensivitity */
        WriteAsicByte(_dvc,DVC_PG2,0x84+0x20*_ch,(MotionDetectorParameter->md_speedsens - 1));
        /* Temporal Sensivitity */    
        WriteAsicByte(_dvc,DVC_PG2,0x85+0x20*_ch,(((MotionDetectorParameter->md_tmpsens - 1)<<4)&0xf0) |((MotionDetectorParameter->md_spsens - 1)&0x0f));
        
#if 0
        if(GetAsicFlgType(_dvc,DVC_PG2,0x61+(_ch<<3),BIT5))		//... if(MDAREA_EN=1)
            SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT4,BIT4);	//... SBOX_EN=1 for new channel
        _t1_ = GetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5)^BIT5;
        if(_t1_ == BIT5){
            _t1_ = GetAsicFlgType(_dvc,DVC_PG2,0x61+(_ch<<3),BIT5);
            if(_t1_ == BIT5)	SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5|BIT4,BIT5|BIT4);
            else	SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5|BIT4,BIT5);
        }
        else	SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5|BIT4,0x00);		//... MDBOX_MD=0,SBOX_EN=0


    if(OUTMODEFLAG == 0){
        WriteAsicByte(_dvc,DVC_PG2,0x60,0xa8);
        WriteAsicByte(_dvc,DVC_PG2,0x61,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x62,0x01);
        WriteAsicByte(_dvc,DVC_PG2,0x63,0x15);
        WriteAsicByte(_dvc,DVC_PG2,0x64,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x65,0x17);
        WriteAsicByte(_dvc,DVC_PG2,0x66,0xfb);

        WriteAsicByte(_dvc,DVC_PG2,0x68,0xa9);
        WriteAsicByte(_dvc,DVC_PG2,0x69,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x6a,0x1);
        WriteAsicByte(_dvc,DVC_PG2,0x6b,0x15);
        WriteAsicByte(_dvc,DVC_PG2,0x6c,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x6d,0x17);
        WriteAsicByte(_dvc,DVC_PG2,0x6e,0xfb);

        WriteAsicByte(_dvc,DVC_PG2,0x70,0xaa);
        WriteAsicByte(_dvc,DVC_PG2,0x71,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x72,0x1);
        WriteAsicByte(_dvc,DVC_PG2,0x73,0x15);
        WriteAsicByte(_dvc,DVC_PG2,0x74,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x75,0x17);
        WriteAsicByte(_dvc,DVC_PG2,0x76,0xfb);
        
        WriteAsicByte(_dvc,DVC_PG2,0x78,0xab);
        WriteAsicByte(_dvc,DVC_PG2,0x79,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x7a,0x1);
        WriteAsicByte(_dvc,DVC_PG2,0x7b,0x15);
        WriteAsicByte(_dvc,DVC_PG2,0x7c,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x7d,0x17);
        WriteAsicByte(_dvc,DVC_PG2,0x7e,0xfb);
        }
    else{
        //CH0
        WriteAsicByte(_dvc,DVC_PG2,0x60,0xa8);//Enable the 2Dbox;2dbox MD mode;enable blending
        WriteAsicByte(_dvc,DVC_PG2,0x61,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x62,0x01);
        WriteAsicByte(_dvc,DVC_PG2,0x63,0x0a);
        WriteAsicByte(_dvc,DVC_PG2,0x64,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x65,0x0b);
        WriteAsicByte(_dvc,DVC_PG2,0x66,0xfb);//HNUM 16;VNUM 12
        //CH1
        WriteAsicByte(_dvc,DVC_PG2,0x68,0xa9);
        WriteAsicByte(_dvc,DVC_PG2,0x69,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x6a,0x5b);
        WriteAsicByte(_dvc,DVC_PG2,0x6b,0x0a);
        WriteAsicByte(_dvc,DVC_PG2,0x6c,0x00);
        WriteAsicByte(_dvc,DVC_PG2,0x6d,0x0b);
        WriteAsicByte(_dvc,DVC_PG2,0x6e,0xfb);
        //CH2
        WriteAsicByte(_dvc,DVC_PG2,0x70,0xaa);
        WriteAsicByte(_dvc,DVC_PG2,0x71,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x72,0x1);
        WriteAsicByte(_dvc,DVC_PG2,0x73,0x0a);
        WriteAsicByte(_dvc,DVC_PG2,0x74,0x48);
        WriteAsicByte(_dvc,DVC_PG2,0x75,0x0b);
        WriteAsicByte(_dvc,DVC_PG2,0x76,0xfb);
        //CH3
        WriteAsicByte(_dvc,DVC_PG2,0x78,0xab);
        WriteAsicByte(_dvc,DVC_PG2,0x79,0x0);
        WriteAsicByte(_dvc,DVC_PG2,0x7a,0x5b);
        WriteAsicByte(_dvc,DVC_PG2,0x7b,0x0a);
        WriteAsicByte(_dvc,DVC_PG2,0x7c,0x48);
        WriteAsicByte(_dvc,DVC_PG2,0x7d,0x0b);
        WriteAsicByte(_dvc,DVC_PG2,0x7e,0xfb);

        }
    #endif

	//set_gpio_in(GPIO_MOYION_DETECTOR_BIT);//inited in  tw2835 init part

	//WriteAsicByte(MASTER,DVC_PG1,0x63,0x80);//enble record path ch1????
       //Adjust the horizontal starting position for motion detection: 8pixel
	WriteAsicByte(MASTER,DVC_PG2,0x82+0x20*_ch,0x08); //only detect odd field;
	                                                    


	for(i = 0; i < 24; i+=2){//MD_MASK
		cAreaHigh = ((MotionDetectionSet[i+1]>>7)&0x01)|((MotionDetectionSet[i+1]>>5)&0x02)|((MotionDetectionSet[i+1]>>3)&0x04)|((MotionDetectionSet[i+1]>>1)&0x08)|((MotionDetectionSet[i+1]<<1)&0x10)|((MotionDetectionSet[i+1]<<3)&0x20)|((MotionDetectionSet[i+1]<<5)&0x40)|((MotionDetectionSet[i+1]<<7)&0x80);
		cAreaLow = ((MotionDetectionSet[i]>>7)&0x01)|((MotionDetectionSet[i]>>5)&0x02)|((MotionDetectionSet[i]>>3)&0x04)|((MotionDetectionSet[i]>>1)&0x08)|((MotionDetectionSet[i]<<1)&0x10)|((MotionDetectionSet[i]<<3)&0x20)|((MotionDetectionSet[i]<<5)&0x40)|((MotionDetectionSet[i]<<7)&0x80);  
		WriteAsicByte(MASTER,DVC_PG2,0x86+i+0x20*_ch,cAreaHigh);
		WriteAsicByte(MASTER,DVC_PG2,0x86+i+1+0x20*_ch,cAreaLow);
	}
#ifdef DVR
          // HW2835MotionDetectorInit();   //inited in  tw2835 init part
#endif        
        }
    else{
        gVideoMotionEn &= ~(1<<_ch);//clear the flag enable motion detect--vincent 20008.01.31
        debug("\ngVideoMotionEn:%x\n",gVideoMotionEn);
        SetAsicFlgType(_dvc,DVC_PG2,0x60+(_ch<<3),BIT5|BIT4,0x00);//2DBox table mode
        //WriteAsicByte(MASTER, DVC_PG2, 0x80+0x20*_ch, 0x80); //disable MD

        //clr_gpio_int_ena(GPIO_MOYION_DETECTOR_BIT); //inited in  tw2835 init part
        for(i = 0; i < 24; i+=2){//mask all cells to disable MD
		WriteAsicByte(MASTER,DVC_PG2,0x86+i+0x20*_ch,0xff);
		WriteAsicByte(MASTER,DVC_PG2,0x86+i+1+0x20*_ch,0xff);
	}
        } 
   
}
//==================================================================================
void	RunWndwMtnMd(U8 _pth)
{
	U8 _osd_pos_x_, _osd_pos_y_;
	U8 _msk_x_, _msk_y_;
	U8 _blk_hl_, _blk_wdth_, _blk_vt_, _blk_hght_;
	U8 _t1_, _t2_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	RunWndwMtnMdOSDChArw(_pth,_osd_pos_x_,_osd_pos_y_);
	RunWndwMtnMdOSDArw(_pth,_osd_pos_x_,_osd_pos_y_);
//	RunWndwMtnOSDBArw(_pth,_osd_pos_x_,_osd_pos_y_);

	_msk_x_ = GetMseMaskX(wndw_crnt_dvc,wndw_crnt_ch,reg_crsr_pos_x);		//... mask_x current
	_msk_y_ = GetMseMaskY(wndw_crnt_dvc,wndw_crnt_ch,reg_crsr_pos_y);		//... mask_y current

/*
	if((GetWndwMtnMdDspl(BIT5)) == OFF){
	}
	else{
	//	*/
//	if((GetWndwMtnMdDspl(BIT5)) == BIT5){

	_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5);
//	_t2_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5);
//	if((_t1_ == BIT5)&&(_t2_ == BIT5)){
	if(_t1_ == BIT5){
		if(wndw_pop_crnt_ch == (mse_ch&0x0f))
//			Set2DBoxCur(wndw_crnt_dvc,wndw_crnt_ch,_msk_x_,_msk_y_);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x67+(wndw_crnt_ch<<3),0xff,(_msk_x_<<4)|_msk_y_);
	}
	
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_MD_CLOS_X)&&(_osd_pos_y_ == WNDW_MTN_MD_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1){
		b_mse_btn_dwn_rb = 0;
		
		if((mse_ch&0x0f) == wndw_pop_crnt_ch){
			wndw_mtn_md_blck_strt = (_msk_x_<<4)|_msk_y_;
		}
		else	wndw_mtn_md_blck_strt |= 0x0f;
	}
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_MD_CLOS_X)&&(_osd_pos_y_ == WNDW_MTN_MD_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
//			ClearOSD(_pth,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,WNDW_MTN_MD_L,WNDW_MTN_MD_H);
//			DisableBox(_pth,BOX_WNDW_QUIT,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_MTN_MD_POS_X,WNDW_MTN_MD_POS_Y,WNDW_MTN_MD_L,WNDW_MTN_MD_H);
			WriteOSDClrX(MASTER,WNDW_MTN_MD_POS_X-1,WNDW_MTN_MD_POS_Y-1,WNDW_MTN_MD_L+2,WNDW_MTN_MD_H+2);

//			Set2DBoxCtrl(wndw_crnt_dvc,wndw_crnt_ch,_2DBOX_CUREN,OFF);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,0x00);		//... SBOX_EN

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else if(_osd_pos_x_ == WNDW_MTN_MD_CHK_X){
			if(_osd_pos_y_ == WNDW_MTN_MD_CHK_MTN_Y){
				_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5)^BIT5;
				if(_t1_ == BIT5){
					_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5);
					if(_t1_ == BIT5)	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5|BIT4,BIT5|BIT4);
					else	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5|BIT4,BIT5);
				}
				else	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT5|BIT4,0x00);		//... MDBOX_MD=0,SBOX_EN=0
				SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MTN_Y);
			}
			else if(_osd_pos_y_ == WNDW_MTN_MD_CHK_MSK_Y){
				_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5)^BIT5;
				SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5,_t1_);
				if(_t1_ == BIT5)	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,BIT4);	//... SBOX_EN=1
				else	SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,0x00);					//... SBOX_EN=0
				SetWndwMtnMdOSDChk(_pth,WNDW_MTN_MD_CHK_MSK_Y);
			}
		}
		else if((_osd_pos_x_ == WNDW_MTN_MD_CH_DEC_X)||(_osd_pos_x_ == WNDW_MTN_MD_CH_INC_X)){
			if(_osd_pos_y_ == WNDW_MTN_MD_CH_NUM_Y)	RunWndwMtnMdCh(_pth,_osd_pos_x_);
		}
		else if((_osd_pos_x_ == WNDW_MTN_MD_SENS_DEC_X)||(_osd_pos_x_ == WNDW_MTN_MD_SENS_INC_X)){
			if((_osd_pos_y_ >= WNDW_MTN_MD_SENS_CL_Y)&&(_osd_pos_y_ <= WNDW_MTN_MD_SENS_VL_Y)){
				if(_osd_pos_x_ == WNDW_MTN_MD_SENS_DEC_X)	RunWndwMtnMdDec(_pth,_osd_pos_y_);
				else	RunWndwMtnMdInc(_pth,_osd_pos_y_);
			}
		}
	}
	if(b_mse_clk_rb == 1){
		b_mse_clk_rb = 0;
		
		if((mse_ch&0x0f) == wndw_pop_crnt_ch){
			if((wndw_mtn_md_blck_strt&0x0f) != 0x0f){		//... if it's finished setting mask area start to end in current channel area.
//				if((GetWndwMtnMdDspl(BIT5)) == BIT5){
				_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x61+(wndw_crnt_ch<<3),BIT5);
				if(_t1_ == BIT5){
					_t1_ = (wndw_mtn_md_blck_strt&0xf0)>>4;					//... mask_x old
					_t2_ = wndw_mtn_md_blck_strt&0x0f;						//... mask_y old

					if(_msk_x_ > _t1_){
						_blk_wdth_ = _msk_x_ - _t1_ + 1;
						_blk_hl_ = _t1_;
					}
					else{
						_blk_wdth_ = _t1_ - _msk_x_ + 1;
						_blk_hl_ = _msk_x_;
					}

					if(_msk_y_ > _t2_){
						_blk_hght_ = _msk_y_ - _t2_ + 1;
						_blk_vt_ = _t2_;
					}
					else{
						_blk_hght_ = _t2_ - _msk_y_ + 1;
						_blk_vt_ = _msk_y_;
					}
					SetMskBlck(wndw_crnt_dvc,wndw_crnt_ch,_blk_hl_,_blk_vt_,_blk_wdth_,_blk_hght_);
				}
			}
		}
		else	wndw_mtn_md_blck_strt |= 0x0f;
	}
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_MD_CH_DEC_X)||(_osd_pos_x_ == WNDW_MTN_MD_CH_INC_X)){
			if(_osd_pos_y_ == WNDW_MTN_MD_CH_NUM_Y)	RunWndwMtnMdCh(_pth,_osd_pos_x_);
		}
		else if((_osd_pos_x_ == WNDW_MTN_MD_SENS_DEC_X)||(_osd_pos_x_ == WNDW_MTN_MD_SENS_INC_X)){
			if((_osd_pos_y_ >= WNDW_MTN_MD_SENS_CL_Y)&&(_osd_pos_y_ <= WNDW_MTN_MD_SENS_VL_Y)){
				if(_osd_pos_x_ == WNDW_MTN_MD_SENS_DEC_X)	RunWndwMtnMdDec(_pth,_osd_pos_y_);
				else	RunWndwMtnMdInc(_pth,_osd_pos_y_);
			}
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

