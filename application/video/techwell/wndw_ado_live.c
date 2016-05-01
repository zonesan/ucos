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
U8 wndw_ado_lv_sel;
//U8 wndw_ado_lv_mute;


//==================================================================================
//						Audio Live wndwdow function description
//==================================================================================
void	SetWndwAdoLvOSDSelRdo(U8 _pth)//U8 _mix_sel)
{
	U8 _t1_, _t2_, _t3_, _t4_;
	
	for(_t2_=0;_t2_<4;_t2_++){
		for(_t1_=0;_t1_<4;_t1_++){
			_t3_ = (_t2_<<2)+_t1_;
			if(_t3_ == wndw_ado_lv_sel)	_t4_ = OSD_RDO_CHK;
			else	_t4_ = OSD_RDO_BLK;
#if defined(__4CH__)
			if(_t3_<4)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,0x00,_t4_);
			else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,OSD_MIX,_t4_);
#elif defined(__16CH__)
			WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,0x00,_t4_);
#endif
		}
	}
	if(wndw_ado_lv_sel == ADO_SEL_PB)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_PB_X,WNDW_ADO_LV_SEL_RDO_PB_Y,0x00,_t1_);
	if(wndw_ado_lv_sel == ADO_SEL_MIX)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_MIX_X,WNDW_ADO_LV_SEL_RDO_PB_Y,0x00,_t1_);
}
//==================================================================================
void	SetWndwAdoLvOSDSel(U8 _pth)//U8 _data)
{
	U8 _t1_, _t2_, _t3_;
	
	SetOSDGrp(_pth,WNDW_ADO_LV_SEL_RDO_0_X-1,WNDW_ADO_LV_SEL_RDO_03_Y,17,5,0x00,str_wndw_grp_sel);

	for(_t2_=0;_t2_<4;_t2_++){
		for(_t1_=0;_t1_<4;_t1_++){
			_t3_ = (_t2_<<2)+_t1_;
#if defined(__4CH__)
			if(_t3_<10){
				if(_t3_<4)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+2+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,0x00,_t3_+0x10);
				else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+2+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,OSD_MIX,_t3_+0x10);
			}
			else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+2+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,OSD_MIX,_t3_+0x17);
#elif defined(__16CH__)
			if(_t3_<10)	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+2+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,0x00,_t3_+0x10);
			else	WriteOSDFnt(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_0_X+2+(_t1_<<2),WNDW_ADO_LV_SEL_RDO_03_Y+_t2_,0x00,_t3_+0x17);
#endif
		}
	}

	WriteOSDStr(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_PB_X+2,WNDW_ADO_LV_SEL_RDO_PB_Y,0x00,str_wndw_ado_lv_pb);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_LV_SEL_RDO_MIX_X+2,WNDW_ADO_LV_SEL_RDO_PB_Y,0x00,str_wndw_ado_lv_mix);

	SetWndwAdoLvOSDSelRdo(_pth);//_data);
}
//==================================================================================
void	SetWndwAdoLvOSDMix(U8 _pth)//U8 _data)
{
	U8 _t1_;//, _t2_;

	SetOSDWndw(NULL,OSD_WNDW_TYPE_BTN,WNDW_ADO_LV_BTN_X,WNDW_ADO_LV_BTN_Y,9,1,str_null);

	if(wndw_ado_lv_sel == ADO_SEL_MIX)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_ADO_LV_BTN_X,WNDW_ADO_LV_BTN_Y,_t1_,str_wndw_ado_lv_mix_rto);
}
//==================================================================================
void	CreateWndwAdoLv(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

//	ClearOSD(X_PATH,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,WNDW_ADO_LV_L,WNDW_ADO_LV_H);
//	SetOSDWndw(X_PATH,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,
//		WNDW_ADO_LV_L,WNDW_ADO_LV_H,str_wndw_ttl_ado_live);
	WriteOSDClr(_pth,MASTER,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,WNDW_ADO_LV_L,WNDW_ADO_LV_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,WNDW_ADO_LV_L,WNDW_ADO_LV_H,str_wndw_ttl_ado_lv);


	wndw_ado_lv_sel = GetAsicFlgType(MASTER,DVC_PG0,0x71,0x1f);			//... MIX_OUTSEL data
//	wndw_ado_lv_mute = GetAsicFlgType(MASTER,DVC_PG0,0x6d,0x1f);			//... MIX_MUTE data

	mse_flg_on_osd = 0x003f;
	SetWndwAdoLvOSDSel(_pth);
	SetWndwAdoLvOSDMix(_pth);
}
//==================================================================================
void	RunWndwAdoLvSel(U8 _pth)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;//, _t2_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_LV_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_LV_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_LV_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_LV_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);

//			ClearOSD(X_PATH,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,WNDW_ADO_LV_L,WNDW_ADO_LV_H);
//			DisableBox(X_PATH,BOX_WNDW_FRM-2,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_ADO_LV_POS_X,WNDW_ADO_LV_POS_Y,WNDW_ADO_LV_L,WNDW_ADO_LV_H);
			WriteOSDClrX(MASTER,WNDW_ADO_LV_POS_X-1,WNDW_ADO_LV_POS_Y-1,WNDW_ADO_LV_L+2,WNDW_ADO_LV_H+2);

			SetOSDNoMn();
//
//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else{
#if defined(__4CH__)
			if((_osd_pos_y_ == WNDW_ADO_LV_SEL_RDO_03_Y)||(_osd_pos_y_ == WNDW_ADO_LV_SEL_RDO_PB_Y)){
#elif defined(__16CH__)
			if((_osd_pos_y_ >= WNDW_ADO_LV_SEL_RDO_03_Y)&&(_osd_pos_y_ <= WNDW_ADO_LV_SEL_RDO_PB_Y)){
#endif
				if(_osd_pos_y_ == WNDW_ADO_LV_SEL_RDO_PB_Y){
					if((_osd_pos_x_ == WNDW_ADO_LV_SEL_RDO_PB_X)||(_osd_pos_x_ == WNDW_ADO_LV_SEL_RDO_MIX_X)){

						if(_osd_pos_x_ == WNDW_ADO_LV_SEL_RDO_PB_X)	_t1_ = ADO_SEL_PB;
						else if(_osd_pos_x_ == WNDW_ADO_LV_SEL_RDO_MIX_X)	_t1_ = ADO_SEL_MIX;

						if(_t1_ != wndw_ado_lv_sel){
							wndw_ado_lv_sel = _t1_;
							SetAsicFlgType(MASTER,DVC_PG0,0x71,0x1f,wndw_ado_lv_sel);
							SetWndwAdoLvOSDSelRdo(_pth);
							SetWndwAdoLvOSDMix(_pth);
						}
					}
				}
				else{
					if((_osd_pos_x_==WNDW_ADO_LV_SEL_RDO_0_X)||(_osd_pos_x_==WNDW_ADO_LV_SEL_RDO_1_X)||
						(_osd_pos_x_==WNDW_ADO_LV_SEL_RDO_2_X)||(_osd_pos_x_==WNDW_ADO_LV_SEL_RDO_3_X)){

						_t1_ = ((_osd_pos_y_-WNDW_ADO_LV_SEL_RDO_03_Y)<<2)+((_osd_pos_x_-WNDW_ADO_LV_SEL_RDO_0_X)>>2);
					
						if(_t1_ != wndw_ado_lv_sel){
							wndw_ado_lv_sel = _t1_;
							SetAsicFlgType(MASTER,DVC_PG0,0x71,0x1f,wndw_ado_lv_sel);
							SetWndwAdoLvOSDSelRdo(_pth);
							SetWndwAdoLvOSDMix(_pth);
						}
					}
				}

			}
			else if(wndw_ado_lv_sel == ADO_SEL_MIX){
				if(((_osd_pos_x_ >= WNDW_ADO_LV_BTN_X)&&(_osd_pos_x_ < (WNDW_ADO_LV_BTN_X+9)))&&(_osd_pos_y_ == WNDW_ADO_LV_BTN_Y))
					CreateWndwAdoLvMr(_pth);
			}
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================
void	RunWndwAdoLv(U8 _pth)//U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	if(b_ado_lv_mr == ON){
		RunWndwAdoLvMr(_pth);//_osd_pos_x_,_osd_pos_y_);
	}
	else
		RunWndwAdoLvSel(_pth);//_osd_pos_x_,_osd_pos_y_);
}
//==================================================================================

#endif
