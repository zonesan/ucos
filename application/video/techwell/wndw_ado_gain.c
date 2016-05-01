#include "hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ado_gain.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"

#if defined(__WTH_ADO__)
//==================================================================================
//						Audio Gain window function description
//==================================================================================
//void	RunWndwAdoGnOSDIgChArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
//{
//	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_CH_DEC_X,WNDW_ADO_GN_IG_CH_INC_X,WNDW_ADO_GN_IG_CH_NUM_Y,MSE_ON_OSD_IG_CH);
//}
//==================================================================================
//void	RunWndwAdoGnOSDIgArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
//{
////	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y,MSE_ON_OSD_IG_CH1);
////	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y+1,MSE_ON_OSD_IG_CH2);
////	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y+2,MSE_ON_OSD_IG_CH3);
////	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y+3,MSE_ON_OSD_IG_CH4);
//	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y,MSE_ON_OSD_IG);
//}
//==================================================================================
//==================================================================================
void	SetWndwAdoGnOSDIgChNum(U8 _pth)//, U8 _index_y, U8 _dat)
{
	U8 _t1_, _t2_;
	
	_t2_ = ConvDec2Bcd(wndw_pop_crnt_ch);
	
	_t1_ = (_t2_&0xf0)>>4;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_CH_NUM_X-1,WNDW_ADO_GN_IG_CH_NUM_Y,OSD_CLR,0x00);
	else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_CH_NUM_X-1,WNDW_ADO_GN_IG_CH_NUM_Y,0x00,_t1_+0x10);
	_t1_ = _t2_&0x0f;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_CH_NUM_X,WNDW_ADO_GN_IG_CH_NUM_Y,0x00,_t1_+0x10);
}
//==================================================================================
void	SetWndwAdoGnOSDIgNum(U8 _pth, U8 _dat)
{
	U8 _t1_;
	
	_dat = ConvDec2Bcd(_dat);
	
	_t1_ = (_dat&0xf0)>>4;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_NUM_X-1,WNDW_ADO_GN_IG_NUM_Y,OSD_CLR,0x00);
	else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_NUM_X-1,WNDW_ADO_GN_IG_NUM_Y,0x00,_t1_+0x10);
	_t1_ = _dat&0x0f;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_IG_NUM_X,WNDW_ADO_GN_IG_NUM_Y,0x00,_t1_+0x10);
}
//==================================================================================
void	SetWndwAdoGnOSDIg(U8 _pth)
{
	U8 _t1_;//, _t2_;

	SetOSDGrp(_pth,WNDW_ADO_GN_TTL_X-1,WNDW_ADO_GN_IG_CH_Y,16,2,0x00,str_wndw_grp_in_gn);

	WriteOSDStr(_pth,MASTER,WNDW_ADO_GN_TTL_X,WNDW_ADO_GN_IG_CH_Y,0x00,str_wndw_cmn_ch);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_GN_TTL_X,WNDW_ADO_GN_IG_NUM_Y,0x00,str_wndw_ado_gn);

//	RunWndwAdoGnOSDIgChArw(_pth,0,0);
	RunMseOnOSDH(_pth,0,0,WNDW_ADO_GN_IG_CH_DEC_X,WNDW_ADO_GN_IG_CH_INC_X,WNDW_ADO_GN_IG_CH_NUM_Y,MSE_ON_OSD_IG_CH);
//	RunWndwAdoGnOSDIgArw(_pth,0,0);
	RunMseOnOSDH(_pth,0,0,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y,MSE_ON_OSD_IG);

//	for(_t2_=0;_t2_<cmn_max_ch;_t2_++){
		SetWndwAdoGnOSDIgChNum(_pth);
		_t1_ = GetWndwAdoGnIg();//WNDW_ADO_GN_IG_NUM_Y+_t2_);
		SetWndwAdoGnOSDIgNum(_pth,_t1_);
//	}
}
//==================================================================================
void	SetWndwAdoGnOSDOgNum(U8 _pth, U8 _dat)
{
	U8 _t1_;
	
	_dat = ConvDec2Bcd(_dat);
	_t1_ = (_dat&0xf0)>>4;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_OG_NUM_X-1,WNDW_ADO_GN_OG_NUM_Y,OSD_CLR,0x00);
	else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_OG_NUM_X-1,WNDW_ADO_GN_OG_NUM_Y,0x00,0x10+_t1_);
	_t1_ = _dat&0x0f;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_GN_OG_NUM_X,WNDW_ADO_GN_OG_NUM_Y,0x00,0x10+_t1_);
}
//==================================================================================
void	SetWndwAdoGnOSDOg(U8 _pth)
{
	U8 _t1_;

	SetOSDGrp(_pth,WNDW_ADO_GN_POS_X+1,WNDW_ADO_GN_OG_NUM_Y,16,1,0x00,str_wndw_grp_out_gn);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_GN_TTL_X,WNDW_ADO_GN_OG_NUM_Y,0x00,str_wndw_ado_gn);

	RunMseOnOSDH(_pth,0,0,WNDW_ADO_GN_OG_DEC_X,WNDW_ADO_GN_OG_INC_X,WNDW_ADO_GN_OG_NUM_Y,MSE_ON_OSD_OG);

	_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x44);
	SetWndwAdoGnOSDOgNum(_pth,_t1_);
}
//==================================================================================
U8	GetWndwAdoGnIg(void)//U8 _index_y)
{
	U8 _t1_;

//	if(_index_y == WNDW_ADO_GN_IG_NUM_Y)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x61,0x44);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+1)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x60,0x04);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+2)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x60,0x44);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+3)
//		_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x61,0x04);
	if(wndw_crnt_ch == 0)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x60,0x04);			//... AIGAIN0
	else if(wndw_crnt_ch == 1)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x60,0x44);		//... AIGAIN1
	else if(wndw_crnt_ch == 2)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x61,0x04);		//... AIGAIN2
	else if(wndw_crnt_ch == 3)	_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x61,0x44);		//... AIGAIN3

	return	_t1_;
}
//==================================================================================
void	SetWndwAdoGnIg(U8 _dat)
{
//	if(_index_y == WNDW_ADO_GN_IG_NUM_Y)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x61,0x44,_dat);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+1)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x60,0x04,_dat);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+2)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x60,0x44,_dat);
//	else if(_index_y == WNDW_ADO_GN_IG_NUM_Y+3)
//		SetAsicVar8Type(MASTER,DVC_PG0,0x61,0x04,_dat);
	if(wndw_crnt_ch == 0)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x60,0x04,_dat);			//... AIGAIN0
	else if(wndw_crnt_ch == 1)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x60,0x44,_dat);		//... AIGAIN1
	else if(wndw_crnt_ch == 2)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x61,0x04,_dat);		//... AIGAIN2
	else if(wndw_crnt_ch == 3)	SetAsicVar8Type(wndw_crnt_dvc,DVC_PG0,0x61,0x44,_dat);		//... AIGAIN3
}
//==================================================================================
//==================================================================================
void	CreateWndwAdoGn(U8 _pth)
{
//	b_status_win = ON;
	b_cmn_wndw_mn = ON;

////	ClearOSD(X_PATH,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
////	SetOSDWndw(X_PATH,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,
////		WNDW_ADO_GN_L,WNDW_ADO_GN_H,str_win_ttl_ado_gain);
	WriteOSDClr(_pth,MASTER,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H,str_wndw_ttl_ado_gn);

//	if(BitSet(_pth,PTH_X)){
//		WriteOSDClrX(MASTER,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
////		SetOSDWndw(X_PATH,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,
////			WNDW_ADO_GN_L,WNDW_ADO_GN_H,str_win_ttl_ado_gain);
//	}
//	else{
//		WriteOSDClrY(MASTER,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
//	}

	mse_flg_on_osd = 0x001f;

	SetWndwAdoGnOSDIg(_pth);
	SetWndwAdoGnOSDOg(_pth);
}
//==================================================================================
//==================================================================================
void	RunWndwAdoGnDec(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;

	if(_osd_pos_y == WNDW_ADO_GN_IG_CH_NUM_Y){
		if(wndw_pop_crnt_ch > 0)	wndw_pop_crnt_ch--;
		else	wndw_pop_crnt_ch = (cmn_max_ch-1);
		SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

		SetWndwAdoGnOSDIgChNum(_pth);
		_rdat_ = GetWndwAdoGnIg();//WNDW_ADO_GN_IG_NUM_Y+_t2_);
		SetWndwAdoGnOSDIgNum(_pth,_rdat_);
	}
	else if(_osd_pos_y == WNDW_ADO_GN_OG_NUM_Y){
		_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x44);
		if(_rdat_ > 0)	_rdat_--;
//		else	_rdat_ = 15;
		SetAsicVar8Type(MASTER,DVC_PG0,0x70,0x44,_rdat_);
		SetWndwAdoGnOSDOgNum(_pth,_rdat_);
	}
	else{
		_rdat_ = GetWndwAdoGnIg();//_osd_pos_y);
		if(_rdat_ > 0)	_rdat_--;
//				else	_rdat_ = 15;
		SetWndwAdoGnIg(_rdat_);
		SetWndwAdoGnOSDIgNum(_pth,_rdat_);
	}
}
//==================================================================================
void	RunWndwAdoGnInc(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;

	if(_osd_pos_y == WNDW_ADO_GN_IG_CH_NUM_Y){
		if(wndw_pop_crnt_ch < (cmn_max_ch-1))	wndw_pop_crnt_ch++;
		else	wndw_pop_crnt_ch = 0;
		SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

		SetWndwAdoGnOSDIgChNum(_pth);
		_rdat_ = GetWndwAdoGnIg();//WNDW_ADO_GN_IG_NUM_Y+_t2_);
		SetWndwAdoGnOSDIgNum(_pth,_rdat_);
	}
	else if(_osd_pos_y == WNDW_ADO_GN_OG_NUM_Y){
		_rdat_ = GetAsicVar8Type(MASTER,DVC_PG0,0x70,0x44);
		if(_rdat_ < 15)	_rdat_++;
//		else	_rdat_ = 0;
		SetAsicVar8Type(MASTER,DVC_PG0,0x70,0x44,_rdat_);
		SetWndwAdoGnOSDOgNum(_pth,_rdat_);
	}
	else{
		_rdat_ = GetWndwAdoGnIg();//_osd_pos_y);
		if(_rdat_ < 15)	_rdat_++;
//				else	_rdat_ = 0;
		SetWndwAdoGnIg(_rdat_);
		SetWndwAdoGnOSDIgNum(_pth,_rdat_);
	}
}
//==================================================================================
void	RunWndwAdoGn(U8 _pth)
{
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

//	RunWndwAdoGnOSDIgArw(_pth,_osd_pos_x_,_osd_pos_y_);
	RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_GN_IG_CH_DEC_X,WNDW_ADO_GN_IG_CH_INC_X,WNDW_ADO_GN_IG_CH_NUM_Y,MSE_ON_OSD_IG_CH);
	RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_GN_IG_DEC_X,WNDW_ADO_GN_IG_INC_X,WNDW_ADO_GN_IG_NUM_Y,MSE_ON_OSD_IG);
	RunMseOnOSDH(_pth,_osd_pos_x_,_osd_pos_y_,WNDW_ADO_GN_OG_DEC_X,WNDW_ADO_GN_OG_INC_X,WNDW_ADO_GN_OG_NUM_Y,MSE_ON_OSD_OG);
	
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_GN_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_GN_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_GN_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_GN_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);

//			ClearOSD(X_PATH,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
			WriteOSDClr(_pth,MASTER,WNDW_ADO_GN_POS_X,WNDW_ADO_GN_POS_Y,WNDW_ADO_GN_L,WNDW_ADO_GN_H);
			WriteOSDClrX(MASTER,WNDW_ADO_GN_POS_X-1,WNDW_ADO_GN_POS_Y-1,WNDW_ADO_GN_L+2,WNDW_ADO_GN_H+2);
//			DisableBox(X_PATH,BOX_WNDW_FRM-2,BOX_WNDW_FRM);	// decoration window, main window

//... should be active later
			SetOSDNoMn();

//			b_status_win = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else{
			if(_osd_pos_y_ == WNDW_ADO_GN_IG_CH_NUM_Y){
				if(_osd_pos_x_ == WNDW_ADO_GN_IG_CH_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
				else if(_osd_pos_x_ == WNDW_ADO_GN_IG_CH_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
			}
			else if(_osd_pos_y_ == WNDW_ADO_GN_IG_NUM_Y){
				if(_osd_pos_x_ == WNDW_ADO_GN_IG_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
				else if(_osd_pos_x_ == WNDW_ADO_GN_IG_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
			}
			else if(_osd_pos_y_ == WNDW_ADO_GN_OG_NUM_Y){
				if(_osd_pos_x_ == WNDW_ADO_GN_OG_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
				else if(_osd_pos_x_ == WNDW_ADO_GN_OG_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
			}
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

		if(_osd_pos_y_ == WNDW_ADO_GN_IG_CH_NUM_Y){
			if(_osd_pos_x_ == WNDW_ADO_GN_IG_CH_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
			else if(_osd_pos_x_ == WNDW_ADO_GN_IG_CH_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
		}
		else if(_osd_pos_y_ == WNDW_ADO_GN_IG_NUM_Y){
			if(_osd_pos_x_ == WNDW_ADO_GN_IG_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
			else if(_osd_pos_x_ == WNDW_ADO_GN_IG_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
		}
		else if(_osd_pos_y_ == WNDW_ADO_GN_OG_NUM_Y){
			if(_osd_pos_x_ == WNDW_ADO_GN_OG_DEC_X)	RunWndwAdoGnDec(_pth,_osd_pos_y_);
			else if(_osd_pos_x_ == WNDW_ADO_GN_OG_INC_X)	RunWndwAdoGnInc(_pth,_osd_pos_y_);
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

#endif
