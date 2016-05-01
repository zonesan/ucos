#include	"hdr.h"
#include "cmn.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "box_wndw.h"
#include "box.h"
#include        "wndw_rc_fmt.h"
#include        "wndw_rc_fmt_ls.h"
#include        "wndw_pop.h"
#include        "wndw_mtn_bd_nd.h"
#include        "wndw_ds_scrn.h"
#include        "wndw_ds_src.h"
#include        "wndw_ds_indx.h"
#include        "wndw_zm.h"
#include        "wndw_zm_pop.h"
#include        "wndw_mtn_md.h"
#include        "wndw_ado_gain.h"
#include        "wndw_ado_live.h"
#include        "wndw_ado_rec.h"
#include        "wndw_rg.h"
#include        "wndw_rc_indx.h"
#include        "wndw_rc_nr.h"
#include        "wndw_rc_fmt_qu.h"


//==================================================================================
//						zm window definition & variable description
//==================================================================================
U8 wndw_zm_ch_l;
U8 wndw_zm_ch_r;
U8 wndw_zm_ch_t;
U8 wndw_zm_ch_b;


//==================================================================================
//						zm window function description
//==================================================================================
void	CreateWndwZm(void)
{
	U8 _rdat_;
	
//	b_status_win = ON;
	b_cmn_wndw_mn = ON;

	_rdat_ = ReadAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0c);
	SetBit(_rdat_,(BIT3|BIT2));
	WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0c,_rdat_);
}
//==================================================================================
void	RunWndwZm(void)
{
	U8 _crsr_pos_x_, _crsr_pos_y_;
	U8	_zm_hl_, _zm_hr_, _zm_vt_, _zm_vb_;
//	U8 _mse_btn_flg_=0;
	U8 _rdat_;
	U8 _dvc_, _ch_;
	U8 _t1_;
	U8	_osd_pos_x_, _osd_pos_y_;


	_zm_hl_ = ReadAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0d);
	_zm_hr_ = _zm_hl_ + 90;
	_zm_vt_ = ReadAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0e);

	if(b_cmn_jp_vdo == VDO_NTSC)	_zm_vb_ = _zm_vt_ + 60;
	else	_zm_vb_ = _zm_vt_ + 72;


	_crsr_pos_x_ = (U8)(reg_crsr_pos_x>>1);
	_crsr_pos_y_ = (U8)(reg_crsr_pos_y>>1);

//	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
//	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);
	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


	if(b_zm_pop_en == ON)	RunZmPop();
	else{
		if(b_mse_btn_dwn_lb == 1){
			b_mse_btn_dwn_lb = 0;

		}
		if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;

			_rdat_ = ReadAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0c);
			if(mse_btn_pshng_cnt > MSE_BTN_DLY_CLK){
				_rdat_ ^= BIT7;
//				if(BitSet(_rdat_,BIT7))	SetAsicFlgType(MASTER,DVC_PG1,0x06,BIT2,0x00);		//... CH_VAR_EN_D=0
//				else	SetAsicFlgType(MASTER,DVC_PG1,0x06,BIT2,BIT2);								//... CH_VAR_EN_D=1
//#if	defined(__16CH__)
//				if(BitSet(_rdat_,BIT7)){
//					SetAsicFlgType(SLAVE1,DVC_PG1,0x06,BIT2,0x00);
//					SetAsicFlgType(SLAVE2,DVC_PG1,0x06,BIT2,0x00);
//					SetAsicFlgType(SLAVE3,DVC_PG1,0x06,BIT2,0x00);
//				}
//				else{
//					SetAsicFlgType(SLAVE1,DVC_PG1,0x06,BIT2,BIT2);
//					SetAsicFlgType(SLAVE2,DVC_PG1,0x06,BIT2,BIT2);
//					SetAsicFlgType(SLAVE3,DVC_PG1,0x06,BIT2,BIT2);
//				}
//#endif
				for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					if(BitSet(_rdat_,BIT7)){
						SetAsicFlgType(_dvc_,DVC_PG1,0x06,BIT2,0x00);							//... CH_VAR_EN_D=0
						SetAsicFlgType(_dvc_,DVC_PG2,0x60+(_ch_<<3),BIT5,0x00);				//... MDBOX_MD=0
						SetAsicFlgType(_dvc_,DVC_PG2,0x61+(_ch_<<3),BIT5,0x00);				//... MDMASK_EN=0
					}
					else{
						SetAsicFlgType(_dvc_,DVC_PG1,0x06,BIT2,BIT2);							//... CH_VAR_EN_D=1
					}
				}
				WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0c,_rdat_);
			}
		}
		if(b_mse_clk_rb == 1){
			b_mse_clk_rb = 0;

			CreateZmPop();
		}
	}

	
	if(b_mse_rlse_lb == 0){
		if(b_mse_init_zm == 0){
			b_mse_init_zm = 1;
			if((_crsr_pos_x_ >= _zm_hl_)&&(_crsr_pos_x_ <= _zm_hr_)&&(_crsr_pos_y_ >= _zm_vt_)&&(_crsr_pos_y_ <= _zm_vb_)){
//				SetMseLayer(OFF);
				SetAsicFlgType(MASTER,DVC_PG2,0x10,BIT4,0x00);

				wndw_zm_ch_l = _crsr_pos_x_ - _zm_hl_;
				wndw_zm_ch_r = _zm_hr_ - _crsr_pos_x_;
				wndw_zm_ch_t = _crsr_pos_y_ - _zm_vt_;
				wndw_zm_ch_b = _zm_vb_ - _crsr_pos_y_;
			}
		}
		if((_crsr_pos_x_ >= _zm_hl_)&&(_crsr_pos_x_ <= _zm_hr_)&&(_crsr_pos_y_ >= _zm_vt_)&&(_crsr_pos_y_ <= _zm_vb_)){
			if((_crsr_pos_x_-wndw_zm_ch_l) > -1){
				if((_crsr_pos_x_+wndw_zm_ch_r) < 181)	_zm_hl_ = _crsr_pos_x_ - wndw_zm_ch_l;
			}
			else{
				_zm_hl_ = 0;
				_zm_hr_ = wndw_zm_ch_l + wndw_zm_ch_r;
			}
			WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0d,_zm_hl_);

			if((_crsr_pos_x_+wndw_zm_ch_r) < 181){
				if((_crsr_pos_x_-wndw_zm_ch_l) > -1)	_zm_hr_ = _crsr_pos_x_ + wndw_zm_ch_r;
			}
			else{
				_zm_hl_ = 180 - (wndw_zm_ch_l + wndw_zm_ch_r);
				_zm_hr_ = 180;
			}
			WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0d,_zm_hl_);

			if((_crsr_pos_y_-wndw_zm_ch_t) > -1){
				if((_crsr_pos_y_+wndw_zm_ch_b) <= cmn_bot)	_zm_vt_ = _crsr_pos_y_ - wndw_zm_ch_t;
			}
			else{
				_zm_vt_ = 0;
				_zm_vb_ = wndw_zm_ch_t + wndw_zm_ch_b;
			}
			WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0e,_zm_vt_);

			if((_crsr_pos_y_+wndw_zm_ch_b) <= cmn_bot){
				if((_crsr_pos_y_-wndw_zm_ch_t) > -1)	_zm_vb_ = _crsr_pos_y_ + wndw_zm_ch_b;
			}
			else{
				_zm_vt_ = cmn_bot - (wndw_zm_ch_t + wndw_zm_ch_b);
				_zm_vb_ = cmn_bot;
			}
			WriteAsicByte(cmn_lst_slvr_x,DVC_PG1,0x0e,_zm_vt_);
		}
	}
	else{
		if(b_mse_init_zm == 1){
			b_mse_init_zm = 0;
//			SetMseLayer(ON);
			SetAsicFlgType(MASTER,DVC_PG2,0x10,BIT4,BIT4);
		}
	}
}
//==================================================================================
