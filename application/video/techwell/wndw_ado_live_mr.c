#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ado_live.h"
#include "wndw_ado_live_mr.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"


#if defined(__WTH_ADO__)
//==================================================================================
//						Audio Live wndwdow definition & variable description
//==================================================================================
//U8 idata	wndw_ado_lv_mr_sel;
//U8 idata	wndw_ado_lv_mr_mute;


//==================================================================================
//						Audio Live wndwdow function description
//==================================================================================
void	RunWndwAdoLvMrOSDPbArw(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y)//, U8 _pos_x_l, U8 _pos_x_r, U8 _pos_y, U8 _pos_flg)
{
	U8 _pth_, _prp_;
	U8 _t1_;

	_pth_ = _pth_prp&0x0f;//0xf0;
	_prp_ = _pth_prp&0xf0;//0x0f;
	if(((_osd_pos_x == WNDW_ADO_LV_MR_DEC_X0)||(_osd_pos_x == WNDW_ADO_LV_MR_INC_X0))&&(_osd_pos_y == WNDW_ADO_LV_MR_PB_Y)){
		if(b_ado_lv_mr_arw_pb == 0){
			b_ado_lv_mr_arw_pb = 1;
			
			if(_osd_pos_x == WNDW_ADO_LV_MR_DEC_X0)	_t1_ = OSD_ARW_L_BIG;//OSD_MINUS_SML;
			else	_t1_ = OSD_ARW_R_BIG;//OSD_PLUS_SML;
			WriteOSDFnt(_pth_,MASTER,_osd_pos_x,_osd_pos_y,OSD_MIX|1,_t1_);
		}
	}
	else{
		if(b_ado_lv_mr_arw_pb == 1){
			b_ado_lv_mr_arw_pb = 0;

			WriteOSDFnt(_pth_,MASTER,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_PB_Y,_prp_,OSD_ARW_L_SML);
			WriteOSDFnt(_pth_,MASTER,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_PB_Y,_prp_,OSD_ARW_R_SML);
		}
	}
}
//==================================================================================
void	SetWndwAdoLvMrOSDArw(U8 _pth, U8 _ch)//U8 _osd_pos_y)
{
	U8 _dvc_, _ch_;
	U8 _t1_, _t2_;
	
	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
	if(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))				//... MIX_MUTE data
		_t1_ = OSD_MIX;
#if defined(__4CH__)
	else{
		if(_ch<4)	_t1_ = 0x00;
		else	_t1_ = OSD_MIX;
	}
#elif defined(__16CH__)
	else
		_t1_ = 0x00;
#endif

	if(_ch&BIT0)									//... odd channel
		_t2_ = WNDW_ADO_LV_MR_DEC_X1;
	else												//... even channel
		_t2_ = WNDW_ADO_LV_MR_DEC_X0;
//	_t2_ = _osd_pos_y-WNDW_ADO_LV_MR_CHK_Y;
//	if((wndw_ado_lv_mr_sel != ADO_SEL_MIX)||BitSet(wndw_ado_lv_mr_mute,(BIT0<<_t2_)))	_t1_ = OSD_MIX;
//	else	_t1_ = 0x00;

	RunMseOnOSDH(_pth|_t1_,0,0,_t2_,_t2_+3,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),BIT0<<_ch);
}
//==================================================================================
//==================================================================================
void	SetWndwAdoLvMrOSDPbChk(U8 _pth)
{
//	U8 _dvc_, _ch_;
//	U8 _rdat_;
	U8 _t1_, _t2_;//, _t3_;
	
//	_rdat_ = GetAsicFlgType(MASTER,DVC_PG0,0x6d,BIT4);		//... MIX_MUTE data
//	if(_rdat_){
	if(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4)){				//... MIX_MUTE data
		_t1_ = OSD_MIX;
		_t2_ = OSD_CHK_BOX_BLK;
	}
	else{
		_t1_ = 0x00;
		_t2_ = OSD_CHK_BOX_CHK;
	}
	
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_CHK_X0,WNDW_ADO_LV_MR_PB_Y,0x00,_t2_);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_LV_MR_CHK_X0+2,WNDW_ADO_LV_MR_PB_Y,0x00,str_wndw_ado_lv_pb);

	RunWndwAdoLvMrOSDPbArw(_pth|_t1_,0,0);
}
//==================================================================================
void	SetWndwAdoLvMrOSDPbNum(U8 _pth)
{
	U8 _t1_, _t2_;
	U8 _rdat_;
	
//	_rdat_ = GetAsicFlgType(MASTER,DVC_PG0,0x6d,BIT4);		//... MIX_MUTE data
//	if(_rdat_)	_t2_ = OSD_MIX;
//	else	_t2_ = 0x00;
//	_rdat_ = GetAsicFlgType(MASTER,DVC_PG0,0x6d,BIT4);		//... MIX_MUTE data
	if(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4))	_t2_ = OSD_MIX;
	else	_t2_ = 0x00;

	_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f);		//... MIX_RATIOP data
	_rdat_ = ConvDec2Bcd(_rdat_);
	_t1_ = (_rdat_&0xf0)>>4;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_NUM_X0-1,WNDW_ADO_LV_MR_PB_Y,OSD_CLR,0x00);
	else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_NUM_X0-1,WNDW_ADO_LV_MR_PB_Y,_t2_,_t1_+0x10);
	_t1_ = _rdat_&0x0f;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_NUM_X0,WNDW_ADO_LV_MR_PB_Y,_t2_,_t1_+0x10);
}
//==================================================================================
void	SetWndwAdoLvMrOSDChk(U8 _pth, U8 _ch)//U8 _data)
{
	U8 _dvc_, _ch_;
//	U8 _rdat_;
	U8 _t1_, _t2_, _t3_;
	
	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
#if defined(__4CH__)
	if(_ch<4){
		_t1_ = 0x00;
		if(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))				//... MIX_MUTE data
			_t2_ = OSD_CHK_BOX_BLK;
		else	_t2_ = OSD_CHK_BOX_CHK;
	}
	else{
		_t1_ = OSD_MIX;
		_t2_ = OSD_CHK_BOX_BLK;
	}
#elif defined(__16CH__)
	_t1_ = 0x00;
	if(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))				//... MIX_MUTE data
		_t2_ = OSD_CHK_BOX_BLK;
	else
		_t2_ = OSD_CHK_BOX_CHK;
#endif

	if(_ch&BIT0){									//... odd channel
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_CHK_X1,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t1_,_t2_);
		_t3_ = WNDW_ADO_LV_MR_CHK_X1;
	}
	else{												//... even channel
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_MR_CHK_X0,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t1_,_t2_);
		_t3_ = WNDW_ADO_LV_MR_CHK_X0;
	}
	
	if(_ch < 10)
		WriteOSDFnt(_pth,MASTER,_t3_+2,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t1_,_ch+0x10);
	else
		WriteOSDFnt(_pth,MASTER,_t3_+2,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t1_,_ch+0x17);
}
//==================================================================================
void	SetWndwAdoLvMrOSDNum(U8 _pth, U8 _ch)//U8 _osd_pos_y, U8 _data)
{
	U8 _t1_, _t2_, _t3_;
	U8 _dvc_, _ch_;
	U8 _rdat_;

	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
	_rdat_ = GetWndwAdoLvMr(_dvc_,_ch_);
	if(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))	_t2_ = OSD_MIX;			//... MIX_MUTE data
#if defined(__4CH__)
	else{
		if(_ch<4)	_t2_ = 0x00;
		else{
			_t2_ = OSD_MIX;
			_rdat_ = 0x00;
		}
	}
#elif defined(__16CH__)
	else	_t2_ = 0x00;
#endif

	if(_ch&BIT0)									//... odd channel
		_t3_ = WNDW_ADO_LV_MR_NUM_X1;
	else												//... even channel
		_t3_ = WNDW_ADO_LV_MR_NUM_X0;

	_rdat_ = ConvDec2Bcd(_rdat_);
	_t1_ = (_rdat_&0xf0)>>4;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,_t3_-1,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),OSD_CLR,0x00);
	else	WriteOSDFnt(_pth,MASTER,_t3_-1,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t2_,_t1_+0x10);
	_t1_ = _rdat_&0x0f;
	WriteOSDFnt(_pth,MASTER,_t3_,WNDW_ADO_LV_MR_CHK_Y+(_ch>>1),_t2_,_t1_+0x10);
}
//==================================================================================
void	SetWndwAdoLvMrOSD(U8 _pth)//U8 _data)
{
	U8 _t1_;//, _t2_;

	//... ch0 ~ ch15
	for(_t1_=0;_t1_<16;_t1_++){
		SetWndwAdoLvMrOSDChk(_pth,_t1_);
		SetWndwAdoLvMrOSDArw(_pth,_t1_);
		SetWndwAdoLvMrOSDNum(_pth,_t1_);
	}
	//... pb
	SetWndwAdoLvMrOSDPbChk(_pth);
//	RunWndwAdoLvMrOSDPbArw(_pth|_t1_,0,0);
	SetWndwAdoLvMrOSDPbNum(_pth);
}
//==================================================================================
U8	GetWndwAdoLvMr(U8 _dvc, U8 _ch)//_osd_pos_y)
{
	U8 _t1_;

//	if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+1)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+2)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+3)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+4)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04);
	if(_ch == 0)	_t1_ = GetAsicVar8Type(_dvc,DVC_PG0,0x6e,0x04);		//... MIX_RATIO0
	else if(_ch == 1)	_t1_ = GetAsicVar8Type(_dvc,DVC_PG0,0x6e,0x44);		//... MIX_RATIO1
	else if(_ch == 2)	_t1_ = GetAsicVar8Type(_dvc,DVC_PG0,0x6f,0x04);		//... MIX_RATIO2
	else if(_ch == 3)	_t1_ = GetAsicVar8Type(_dvc,DVC_PG0,0x6f,0x44);		//... MIX_RATIO3

	return	_t1_;
}
//==================================================================================
void	SetWndwAdoLvMr(U8 _dvc, U8 _ch, U8 _dat)//U8 _osd_pos_y, U8 _data)
{
//	if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04,_data);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+1)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44,_data);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+2)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04,_data);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+3)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44,_data);
//	else if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y+4)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04,_data);
	if(_ch == 0)	SetAsicVar8Type(_dvc,DVC_PG0,0x6e,0x04,_dat);		//... MIX_RATIO0
	else if(_ch == 1)	SetAsicVar8Type(_dvc,DVC_PG0,0x6e,0x44,_dat);		//... MIX_RATIO1
	else if(_ch == 2)	SetAsicVar8Type(_dvc,DVC_PG0,0x6f,0x04,_dat);		//... MIX_RATIO2
	else if(_ch == 3)	SetAsicVar8Type(_dvc,DVC_PG0,0x6f,0x44,_dat);		//... MIX_RATIO3
}
//==================================================================================
//==================================================================================
void	CreateWndwAdoLvMr(U8 _pth)
{
////	b_status_wndw = ON;
//	b_cmn_wndw_mn = ON;
	b_ado_lv_mr = ON;

//	ClearOSD(X_PATH,WNDW_ADO_LV_MR_POS_X,WNDW_ADO_LV_MR_POS_Y,WNDW_ADO_LV_MR_L,WNDW_ADO_LV_MR_H);
//	SetOSDWndw(X_PATH,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_ADO_LV_MR_POS_X,WNDW_ADO_LV_MR_POS_Y,
//		WNDW_ADO_LV_MR_L,WNDW_ADO_LV_MR_H,str_wndw_ttl_ado_live);
	WriteOSDClr(_pth,MASTER,WNDW_ADO_LV_MR_POS_X-1,WNDW_ADO_LV_MR_POS_Y-1,WNDW_ADO_LV_MR_L+2,WNDW_ADO_LV_MR_H+2);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_ADO_LV_MR_POS_X,WNDW_ADO_LV_MR_POS_Y,WNDW_ADO_LV_MR_L,WNDW_ADO_LV_MR_H,str_wndw_ttl_ado_lv_mr);


//	wndw_ado_lv_mr_sel = GetAsicVar8Type(MASTER,DVC_PG0,0x71,0x05);			//... MIX_OUTSEL data
//	wndw_ado_lv_mr_mute = GetAsicFlgType(MASTER,DVC_PG0,0x6d,0x1f);		//... MIX_MUTE data

	mse_flg_on_osd = 0xffff;
	b_ado_lv_mr_arw_pb = 1;
//	SetWndwAdoLvMrOSDSel(_pth);
	SetWndwAdoLvMrOSD(_pth);
}
//==================================================================================
//==================================================================================
//void	RunWndwAdoLvMrDec(U8 _pth, U8 _ch)//_osd_pos_y)
//{
//	U8 _dvc_, _ch_;
//	U8 _rdat_;
//
//	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
//	if(!(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))){							//... not MUTE
//		if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT0)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04);
//				if(_rdat_ > 0)	_rdat_--;
//				else	_rdat_ = 15;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+1)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT1)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44);
//				if(_rdat_ > 0)	_rdat_--;
////				else	_rdat_ = 15;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+1,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+2)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT2)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04);
//				if(_rdat_ > 0)	_rdat_--;
////				else	_rdat_ = 15;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+2,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+3)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT3)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44);
//				if(_rdat_ > 0)	_rdat_--;
////				else	_rdat_ = 15;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+3,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+4)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT4)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04);
//				if(_rdat_ > 0)	_rdat_--;
////				else	_rdat_ = 15;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+4,_rdat_);
//			}
//		}
//	}
//}
////==================================================================================
//void	RunWndwAdoLvMrInc(U8 _pth, U8 _ch)//_osd_pos_y)
//{
//	U8 _rdat_;
//
////	if(wndw_ado_lv_mr_sel == ADO_SEL_MIX){
//		if(_osd_pos_y == WNDW_ADO_LV_MR_CHK_Y){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT0)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04);
//				if(_rdat_ < 15)	_rdat_++;
////				else	_rdat_ = 0;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+1)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT1)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44);
//				if(_rdat_ < 15)	_rdat_++;
////				else	_rdat_ = 0;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6e,0x44,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+1,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+2)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT2)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04);
//				if(_rdat_ < 15)	_rdat_++;
////				else	_rdat_ = 0;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+2,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+3)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT3)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44);
//				if(_rdat_ < 15)	_rdat_++;
////				else	_rdat_ = 0;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x6f,0x44,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+3,_rdat_);
//			}
//		}
//		else if(_osd_pos_y == (WNDW_ADO_LV_MR_CHK_Y+4)){
//			if(BitClear(wndw_ado_lv_mr_mute,BIT4)){
//				_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04);
//				if(_rdat_ < 15)	_rdat_++;
////				else	_rdat_ = 0;
//				SetAsicVar8Type(MASTER,DVC_PG0,0x70,0x04,_rdat_);
//				SetWndwAdoLvMrOSDNum(_pth,WNDW_ADO_LV_MR_CHK_Y+4,_rdat_);
//			}
//		}
////	}
//}
//==================================================================================
void	RunWndwAdoLvMr(U8 _pth)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;//, _t2_;
	U8 _dvc_, _ch_;
	U8 _rdat_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


//		//*
//	if(wndw_ado_lv_mr_sel == ADO_SEL_MIX){
//		if(BitClear(wndw_ado_lv_mr_mute,BIT0))
//			RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_CHK_Y,MSE_ON_OSD_CH1);
//		if(BitClear(wndw_ado_lv_mr_mute,BIT1))
//			RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_CHK_Y+1,MSE_ON_OSD_CH2);
//		if(BitClear(wndw_ado_lv_mr_mute,BIT2))
//			RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_CHK_Y+2,MSE_ON_OSD_CH3);
//		if(BitClear(wndw_ado_lv_mr_mute,BIT3))
//			RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_CHK_Y+3,MSE_ON_OSD_CH4);
//		if(BitClear(wndw_ado_lv_mr_mute,BIT4))
//			RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_LV_MR_DEC_X0,WNDW_ADO_LV_MR_INC_X0,WNDW_ADO_LV_MR_CHK_Y+4,MSE_ON_OSD_PB);
//	}
//			//	*/
	
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_LV_MR_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_LV_MR_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_LV_MR_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_LV_MR_CLOS_Y)){
			WriteOSDClr(_pth,MASTER,WNDW_ADO_LV_MR_POS_X,WNDW_ADO_LV_MR_POS_Y,WNDW_ADO_LV_MR_L,WNDW_ADO_LV_MR_H);
			WriteOSDClrX(MASTER,WNDW_ADO_LV_MR_POS_X-1,WNDW_ADO_LV_MR_POS_Y-1,WNDW_ADO_LV_MR_L+2,WNDW_ADO_LV_MR_H+2);

			b_ado_lv_mr = OFF;
			
			CreateWndwAdoLv(_pth);
		}
		else{
#if defined(__4CH__)
			if((_osd_pos_y_ == WNDW_ADO_LV_MR_CHK_Y)||(_osd_pos_y_ == (WNDW_ADO_LV_MR_CHK_Y+1))){
#elif defined(__16CH__)
			if((_osd_pos_y_ >= WNDW_ADO_LV_MR_CHK_Y)&&(_osd_pos_y_ < WNDW_ADO_LV_MR_PB_Y)){
#endif
				_t1_ = (_osd_pos_y_ - WNDW_ADO_LV_MR_CHK_Y)<<1;
//				if((_osd_pos_x_==WNDW_ADO_LV_MR_CHK_X1)||(_osd_pos_x_==WNDW_ADO_LV_MR_DEC_X1)||(_osd_pos_x_==WNDW_ADO_LV_MR_INC_X1))
//					_t1_++;

				if((_osd_pos_x_ == WNDW_ADO_LV_MR_CHK_X0)||(_osd_pos_x_ == WNDW_ADO_LV_MR_CHK_X1)){
					if(_osd_pos_x_ == WNDW_ADO_LV_MR_CHK_X1)	_t1_++;

					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					_rdat_ = GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_)^(BIT0<<_ch_);				//... MIX_MUTE data
					SetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_,_rdat_);
					
					SetWndwAdoLvMrOSDChk(_pth,_t1_);
					mse_flg_on_osd = 0xffff;
					SetWndwAdoLvMrOSDArw(_pth,_t1_);
					SetWndwAdoLvMrOSDNum(_pth,_t1_);
				}
				else if((_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X0)||(_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X1)){
					if(_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X1)	_t1_++;
//					RunWndwAdoLvMrDec(_pth,_t1_);

					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					if(!(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))){							//... not MUTE
						_rdat_ = GetWndwAdoLvMr(_dvc_,_ch_);
						if(_rdat_ > 0)	_rdat_--;
//						else	_rdat_ = 15;
						SetWndwAdoLvMr(_dvc_,_ch_,_rdat_);
						SetWndwAdoLvMrOSDNum(_pth,_t1_);
					}
				}
				else if((_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X0)||(_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X1)){
					if(_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X1)	_t1_++;
//					RunWndwAdoLvMrInc(_pth,_t1_);

					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					if(!(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))){							//... not MUTE
						_rdat_ = GetWndwAdoLvMr(_dvc_,_ch_);
						if(_rdat_ < 15)	_rdat_++;
//						else	_rdat_ = 0;
						SetWndwAdoLvMr(_dvc_,_ch_,_rdat_);
						SetWndwAdoLvMrOSDNum(_pth,_t1_);
					}
				}
			}
			else if(_osd_pos_y_ == WNDW_ADO_LV_MR_PB_Y){
				if(_osd_pos_x_ == WNDW_ADO_LV_MR_CHK_X0){
					_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4)^BIT4;				//... MIX_MUTE data
					SetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4,_rdat_);
					
					b_ado_lv_mr_arw_pb = 1;
					SetWndwAdoLvMrOSDPbChk(_pth);
//					RunWndwAdoLvMrOSDPbArw(_pth|_t1_,0,0);
					SetWndwAdoLvMrOSDPbNum(_pth);
				}
				else if(_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X0){
//					RunWndwAdoLvMrDec(_pth,_osd_pos_y_);
					if(!(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4))){			//... not MUTE
						_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f);
						if(_rdat_ > 0)	_rdat_--;
//						else	_rdat_ = 15;
						SetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f,_rdat_);
						SetWndwAdoLvMrOSDPbNum(_pth);
					}
				}
				else if(_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X0){
//					RunWndwAdoLvMrInc(_pth,_osd_pos_y_);
					if(!(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4))){			//... not MUTE
						_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f);
						if(_rdat_ < 15)	_rdat_++;
//						else	_rdat_ = 0;
						SetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f,_rdat_);
						SetWndwAdoLvMrOSDPbNum(_pth);
					}
				}
			}
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

#if defined(__4CH__)
		if((_osd_pos_y_ == WNDW_ADO_LV_MR_CHK_Y)||(_osd_pos_y_ == (WNDW_ADO_LV_MR_CHK_Y+1))){
#elif defined(__16CH__)
		if((_osd_pos_y_ >= WNDW_ADO_LV_MR_CHK_Y)&&(_osd_pos_y_ < WNDW_ADO_LV_MR_PB_Y)){
#endif
			_t1_ = (_osd_pos_y_ - WNDW_ADO_LV_MR_CHK_Y)<<1;
			if((_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X0)||(_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X1)){
				if(_osd_pos_x_==WNDW_ADO_LV_MR_DEC_X1)	_t1_++;
//					RunWndwAdoLvMrDec(_pth,_t1_);

				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))){							//... not MUTE
					_rdat_ = GetWndwAdoLvMr(_dvc_,_ch_);
					if(_rdat_ > 0)	_rdat_--;
					SetWndwAdoLvMr(_dvc_,_ch_,_rdat_);
					SetWndwAdoLvMrOSDNum(_pth,_t1_);
				}
			}
			else if((_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X0)||(_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X1)){
				if(_osd_pos_x_==WNDW_ADO_LV_MR_INC_X1)	_t1_++;
//					RunWndwAdoLvMrInc(_pth,_t1_);

				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(GetAsicFlgType(_dvc_,DVC_PG0,0x6d,BIT0<<_ch_))){							//... not MUTE
					_rdat_ = GetWndwAdoLvMr(_dvc_,_ch_);
					if(_rdat_ < 15)	_rdat_++;
					SetWndwAdoLvMr(_dvc_,_ch_,_rdat_);
					SetWndwAdoLvMrOSDNum(_pth,_t1_);
				}
			}
		}
		else if(_osd_pos_y_ == WNDW_ADO_LV_MR_PB_Y){
			if(_osd_pos_x_ == WNDW_ADO_LV_MR_DEC_X0){
//					RunWndwAdoLvMrDec(_pth,_osd_pos_y_);
				if(!(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4))){			//... not MUTE
					_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f);
					if(_rdat_ > 0)	_rdat_--;
	//				else	_rdat_ = 15;
					SetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f,_rdat_);
					SetWndwAdoLvMrOSDPbNum(_pth);
				}
			}
			else if(_osd_pos_x_ == WNDW_ADO_LV_MR_INC_X0){
//					RunWndwAdoLvMrInc(_pth,_osd_pos_y_);
				if(!(GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x6d,BIT4))){			//... not MUTE
					_rdat_ = GetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f);
					if(_rdat_ < 15)	_rdat_++;
	//				else	_rdat_ = 0;
					SetAsicFlgType(cmn_lst_slvr_x,DVC_PG0,0x70,0x0f,_rdat_);
					SetWndwAdoLvMrOSDPbNum(_pth);
				}
			}
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

#endif
