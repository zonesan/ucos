#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_mtn_bd_nd.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "wndw_rc_fmt.h"


//==================================================================================
//					Blind and Night Detect window function description
//==================================================================================
void	RunWndwMtnBdNdOSDChArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_BDND_CH_DEC_X,WNDW_MTN_BDND_CH_INC_X,WNDW_MTN_BDND_CH_NUM_Y,MSE_ON_OSD_BDND_CH);
}
//==================================================================================
void	RunWndwMtnBdNdOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_BDND_SENS_DEC_X,WNDW_MTN_BDND_SENS_INC_X,WNDW_MTN_BDND_SENS_CL_Y,MSE_ON_OSD_BDND_CL);
	RunMseOnOSDH(_pth,_osd_pos_x,_osd_pos_y,WNDW_MTN_BDND_SENS_DEC_X,WNDW_MTN_BDND_SENS_INC_X,WNDW_MTN_BDND_SENS_LV_Y,MSE_ON_OSD_BDND_LV);
}
//==================================================================================
//==================================================================================
void	SetWndwMtnBdNdOSDChNum(U8 _pth)
{
	U8 _t1_;

#if	defined(__16CH__)
	if(wndw_pop_crnt_ch < 10)		_t1_ = OSD_CLR;
	else	_t1_ = 0x00;
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_CH_NUM_X-1,WNDW_MTN_BDND_CH_NUM_Y,_t1_,0x11);
#endif
	if(wndw_pop_crnt_ch > 9)		_t1_ = wndw_pop_crnt_ch+6;			//... +6 -> +0x10 -10;
	else	_t1_ = wndw_pop_crnt_ch+0x10;
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_CH_NUM_X,WNDW_MTN_BDND_CH_NUM_Y,0x00,_t1_);
}
//==================================================================================
void	SetWndwMtnBdNdOSDCh(U8 _pth)
{
	WriteOSDStr(_pth,MASTER,WNDW_MTN_BDND_POS_X+3,WNDW_MTN_BDND_CH_NUM_Y,0x00,str_wndw_cmn_ch);
	RunWndwMtnBdNdOSDChArw(_pth,0,0);

	SetWndwMtnBdNdOSDChNum(_pth);
}
//==================================================================================
//==================================================================================
U8	GetWndwMtnBdNdNum(U8 _osd_pos_y)
{
	U8 _t1_;
	
	if(wndw_mn_pos_crnt == POP_POS_MTN_BD){
		if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y)
			_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x80+0x20*wndw_crnt_ch,0x42);//BD_SENS_CL);
		else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y)
			_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x80+0x20*wndw_crnt_ch,0x04);//BD_SENS_LV);
	}
	else if(wndw_mn_pos_crnt == POP_POS_MTN_ND){
		if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y)
			_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x81+0x20*wndw_crnt_ch,0x04);//ND_SENS_TM);
		else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y)
			_t1_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x81+0x20*wndw_crnt_ch,0x44);//ND_SENS_LV);
	}

	return	_t1_;
}
//==================================================================================
void	SetWndwMtnBdNdNum(U8 _osd_pos_y, U8 _data)
{
	if(wndw_mn_pos_crnt == POP_POS_MTN_BD){
		if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y)
			SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x80+0x20*wndw_crnt_ch,0x42,_data);//BD_SENS_CL);
		else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y)
			SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x80+0x20*wndw_crnt_ch,0x04,_data);//BD_SENS_LV);
	}
	else if(wndw_mn_pos_crnt == POP_POS_MTN_ND){
		if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y)
			SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x81+0x20*wndw_crnt_ch,0x04,_data);//ND_SENS_TM);
		else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y)
			SetAsicVar8Type(wndw_crnt_dvc,DVC_PG2,0x81+0x20*wndw_crnt_ch,0x44,_data);//ND_SENS_LV);
	}
}
//==================================================================================
void	SetWndwMtnBdNdOSDNum(U8 _pth, U8 _osd_pos_y, U8 _data)
{
	_data = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_SENS_NUM_X,_osd_pos_y,0x00,(_data&0x0f)+0x10);
//	if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y){}
//	else{
		_data >>= 4;
		if(_data == 0)	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_SENS_NUM_X-1,_osd_pos_y,OSD_CLR,0x00);
		else	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_SENS_NUM_X-1,_osd_pos_y,0x00,_data+0x10);
//	}
}
//==================================================================================
void	SetWndwMtnBdNdOSDNumAll(U8 _pth)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnBdNdNum(WNDW_MTN_BDND_SENS_CL_Y);
	SetWndwMtnBdNdOSDNum(_pth,WNDW_MTN_BDND_SENS_CL_Y,_t1_);
	_t1_ = GetWndwMtnBdNdNum(WNDW_MTN_BDND_SENS_LV_Y);
	SetWndwMtnBdNdOSDNum(_pth,WNDW_MTN_BDND_SENS_LV_Y,_t1_);
}
//==================================================================================
void	SetWndwMtnBdNdOSD(U8 _pth)
{
	SetOSDWndw(_pth,OSD_WNDW_TYPE_DECO,WNDW_MTN_BDND_SENS_TTL_X,WNDW_MTN_BDND_SENS_CL_Y,17,2,str_null);

	if(wndw_mn_pos_crnt == POP_POS_MTN_BD){
		WriteOSDStr(_pth,MASTER,WNDW_MTN_BDND_SENS_TTL_X,WNDW_MTN_BDND_SENS_CL_Y,0x00,str_wndw_mtn_cll);
		WriteOSDStr(_pth,MASTER,WNDW_MTN_BDND_SENS_TTL_X,WNDW_MTN_BDND_SENS_LV_Y,0x00,str_wndw_mtn_lvl);
	}
	else if(wndw_mn_pos_crnt == POP_POS_MTN_ND){
		WriteOSDStr(_pth,MASTER,WNDW_MTN_BDND_SENS_TTL_X,WNDW_MTN_BDND_SENS_CL_Y,0x00,str_wndw_mtn_tmpl);
		WriteOSDStr(_pth,MASTER,WNDW_MTN_BDND_SENS_TTL_X,WNDW_MTN_BDND_SENS_LV_Y,0x00,str_wndw_mtn_lvl);
	}

	RunWndwMtnBdNdOSDArw(_pth,0,0);

	SetWndwMtnBdNdOSDNumAll(_pth);
}
//==================================================================================
void	SetWndwMtnBdNdOSDDtBx(U8 _pth, U8 _ch)
{
	U8 _dvc_, _ch_;
	U8 _rdat_;
	U8 _t1_;//=0, _t2_;
	
	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
	if(wndw_mn_pos_crnt == POP_POS_MTN_BD)	_t1_ = 0x7a;
	else if(wndw_mn_pos_crnt == POP_POS_MTN_ND)	_t1_ = 0x7b;
	_rdat_ = GetAsicFlgType(_dvc_,DVC_PG1,_t1_,BIT0<<_ch_);

	if(_rdat_)	_t1_ = OSD_WNDW_TYPE_DT_CHK;
	else	_t1_ = OSD_WNDW_TYPE_DT_BLK;
#if	defined(__4CH__)
	if(_ch < 4){
		SetOSDWndw(_pth,_t1_,WNDW_MTN_BDND_DT_X+(_ch<<1),WNDW_MTN_BDND_DT_Y+1,1,1,str_null);
	}
	else{
		if(_ch < 8)	SetOSDWndw(_pth,OSD_WNDW_TYPE_DT_MIX,WNDW_MTN_BDND_DT_X+(_ch<<1),WNDW_MTN_BDND_DT_Y+1,1,1,str_null);
		else	SetOSDWndw(_pth,OSD_WNDW_TYPE_DT_MIX,WNDW_MTN_BDND_DT_X+((_ch-8)<<1),WNDW_MTN_BDND_DT_Y+3,1,1,str_null);
	}
#elif	defined(__16CH__)
	if(_ch < 8)	SetOSDWndw(_pth,_t1_,WNDW_MTN_BDND_DT_X+(_ch<<1),WNDW_MTN_BDND_DT_Y+1,1,1,str_null);
	else	SetOSDWndw(_pth,_t1_,WNDW_MTN_BDND_DT_X+((_ch-8)<<1),WNDW_MTN_BDND_DT_Y+3,1,1,str_null);
#endif
}
//==================================================================================
void	SetWndwMtnBdNdOSDDt(U8 _pth)
{
	U8 _t1_=0, _t2_, _t3_;//, _t4_;

	SetOSDGrp(_pth,WNDW_MTN_BDND_DT_X-1,WNDW_MTN_BDND_DT_Y,17,4,0x00,str_wndw_grp_dtctn);

	for(_t2_=0;_t2_<16;_t2_++){
#if	defined(__4CH__)
		if(_t2_ > 3)	_t1_ = OSD_MIX;
#endif
		if(_t2_ < 8){
			WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_DT_X+(_t2_<<1),WNDW_MTN_BDND_DT_Y,_t1_,0x10+_t2_);
		}
		else{
			_t3_ = _t2_-8;
			if(_t3_ < 2)	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_DT_X+(_t3_<<1),WNDW_MTN_BDND_DT_Y+2,_t1_,0x18+_t3_);
			else	WriteOSDFnt(_pth,MASTER,WNDW_MTN_BDND_DT_X+(_t3_<<1),WNDW_MTN_BDND_DT_Y+2,_t1_,0x1f+_t3_);
		}
		SetWndwMtnBdNdOSDDtBx(_pth,_t2_);
	}
}
//==================================================================================
//==================================================================================
void	CreateWndwMtnBdNd(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

//	ClearOSD(_pth,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H);
//	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,
//		WNDW_MTN_BDND_L,WNDW_MTN_BDND_H,str_wndw_ttl_mtn_stp);
	WriteOSDClr(_pth,MASTER,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H);
	if(wndw_mn_pos_crnt == POP_POS_MTN_BD)
		SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H,str_wndw_ttl_mtn_bd);
	else if(wndw_mn_pos_crnt == POP_POS_MTN_ND)
		SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H,str_wndw_ttl_mtn_nd);

	mse_flg_on_osd = 0xff;
	SetWndwMtnBdNdOSDCh(_pth);
	SetWndwMtnBdNdOSD(_pth);
	SetWndwMtnBdNdOSDDt(_pth);
}
//==================================================================================
void	RunWndwMtnBdNdCh(U8 _pth, U8 _osd_pos_x)
{
	if(_osd_pos_x == WNDW_MTN_BDND_CH_DEC_X){
		if(wndw_pop_crnt_ch > 0)		wndw_pop_crnt_ch--;
		else	wndw_pop_crnt_ch = cmn_max_ch-1;
	}
	else{
		if(wndw_pop_crnt_ch < (cmn_max_ch-1))	wndw_pop_crnt_ch++;
		else	wndw_pop_crnt_ch = 0;
	}
	SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

	SetWndwMtnBdNdOSDChNum(_pth);
	SetWndwMtnBdNdOSDNumAll(_pth);
}
//==================================================================================
void	RunWndwMtnBdNdDec(U8 _pth, U8 _osd_pos_y)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnBdNdNum(_osd_pos_y);
//	if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y){
//		if(wndw_mn_pos_crnt == POP_POS_MTN_BD){
//			if(_t1_ > 0)	_t1_--;
////			else	_t1_ = 3;
//		}
//		else if(wndw_mn_pos_crnt == POP_POS_MTN_ND){
//			if(_t1_ > 0)	_t1_--;
////			else	_t1_ = 15;
//		}
//	}
//	else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y){
//		if(_t1_ > 0)	_t1_--;
////		else	_t1_ = 15;
//	}
	if(_t1_ > 0)	_t1_--;

	SetWndwMtnBdNdNum(_osd_pos_y,_t1_);
	SetWndwMtnBdNdOSDNum(_pth,_osd_pos_y,_t1_);
}
//==================================================================================
void	RunWndwMtnBdNdInc(U8 _pth, U8 _osd_pos_y)
{
	U8 _t1_;
	
	_t1_ = GetWndwMtnBdNdNum(_osd_pos_y);
	if(_osd_pos_y == WNDW_MTN_BDND_SENS_CL_Y){
		if(wndw_mn_pos_crnt == POP_POS_MTN_BD){
			if(_t1_ < 3)	_t1_++;
//			else	_t1_ = 0;
		}
		else if(wndw_mn_pos_crnt == POP_POS_MTN_ND){
			if(_t1_ < 15)	_t1_++;
//			else	_t1_ = 0;
		}
	}
	else if(_osd_pos_y == WNDW_MTN_BDND_SENS_LV_Y){
		if(_t1_ < 15)	_t1_++;
//		else	_t1_ = 0;
	}

	SetWndwMtnBdNdNum(_osd_pos_y,_t1_);
	SetWndwMtnBdNdOSDNum(_pth,_osd_pos_y,_t1_);
}
//==================================================================================
void	RunWndwMtnBdNd(U8 _pth)
{
	U8 _osd_pos_x_, _osd_pos_y_;
//	U8 _t1_, _t2_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	RunWndwMtnBdNdOSDChArw(_pth,_osd_pos_x_,_osd_pos_y_);
	RunWndwMtnBdNdOSDArw(_pth,_osd_pos_x_,_osd_pos_y_);

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_BDND_CLOS_X)&&(_osd_pos_y_ == WNDW_MTN_BDND_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1){
		b_mse_btn_dwn_rb = 0;
		
	}
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_BDND_CLOS_X)&&(_osd_pos_y_ == WNDW_MTN_BDND_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
//			ClearOSD(_pth,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H);
//			DisableBox(_pth,BOX_WNDW_QUIT,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_MTN_BDND_POS_X,WNDW_MTN_BDND_POS_Y,WNDW_MTN_BDND_L,WNDW_MTN_BDND_H);
			WriteOSDClrX(MASTER,WNDW_MTN_BDND_POS_X-1,WNDW_MTN_BDND_POS_Y-1,WNDW_MTN_BDND_L+2,WNDW_MTN_BDND_H+2);

//			Set2DBoxCtrl(wndw_crnt_dvc,wndw_crnt_ch,_2DBOX_CUREN,OFF);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG2,0x60+(wndw_crnt_ch<<3),BIT4,0x00);		//... SBOX_EN

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else if((_osd_pos_x_ == WNDW_MTN_BDND_CH_DEC_X)||(_osd_pos_x_ == WNDW_MTN_BDND_CH_INC_X)){
			if(_osd_pos_y_ == WNDW_MTN_BDND_CH_NUM_Y)	RunWndwMtnBdNdCh(_pth,_osd_pos_x_);
		}
		else if((_osd_pos_x_ == WNDW_MTN_BDND_SENS_DEC_X)||(_osd_pos_x_ == WNDW_MTN_BDND_SENS_INC_X)){
			if((_osd_pos_y_ == WNDW_MTN_BDND_SENS_CL_Y)||(_osd_pos_y_ == WNDW_MTN_BDND_SENS_LV_Y)){
				if(_osd_pos_x_ == WNDW_MTN_BDND_SENS_DEC_X)	RunWndwMtnBdNdDec(_pth,_osd_pos_y_);
				else	RunWndwMtnBdNdInc(_pth,_osd_pos_y_);
			}
		}
	}
	if(b_mse_clk_rb == 1){
		b_mse_clk_rb = 0;
		
	}
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_MTN_BDND_CH_DEC_X)||(_osd_pos_x_ == WNDW_MTN_BDND_CH_INC_X)){
			if(_osd_pos_y_ == WNDW_MTN_BDND_CH_NUM_Y)	RunWndwMtnBdNdCh(_pth,_osd_pos_x_);
		}
		else if((_osd_pos_x_ == WNDW_MTN_BDND_SENS_DEC_X)||(_osd_pos_x_ == WNDW_MTN_BDND_SENS_INC_X)){
			if((_osd_pos_y_ == WNDW_MTN_BDND_SENS_CL_Y)||(_osd_pos_y_ == WNDW_MTN_BDND_SENS_LV_Y)){
				if(_osd_pos_x_ == WNDW_MTN_BDND_SENS_DEC_X)	RunWndwMtnBdNdDec(_pth,_osd_pos_y_);
				else	RunWndwMtnBdNdInc(_pth,_osd_pos_y_);
			}
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

