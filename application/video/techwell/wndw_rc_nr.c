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
#include        "wndw_mtn_md.h"
#include        "wndw_ado_gain.h"
#include        "wndw_ado_live.h"
#include        "wndw_ado_rec.h"
#include        "wndw_rg.h"
#include        "wndw_rc_indx.h"
#include        "wndw_rc_nr.h"
#include        "wndw_rc_fmt_qu.h"


//==================================================================================
//						Display wndwdow function description
//==================================================================================
void	SetWndwRcNrOSDWnRdo(U8 _pth)
{
	U8 _rdat_;
	U8 _t1_, _t2_;
	
	_rdat_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG1,0x52,0x42);
	for(_t1_=0;_t1_<4;_t1_++){
		if(_t1_ == _rdat_)	_t2_ = OSD_RDO_CHK;
		else	_t2_ = OSD_RDO_BLK;

		WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_RDO_X,WNDW_RC_NR_RDO_Y+_t1_,0x00,_t2_);
	}
}
//==================================================================================
void	SetWndwRcNrOSDTyp(void)
{
	U8 _rdat_;
	U8 _t1_, _t2_, _t3_, _t4_;
//	U8 _ofst_h_=3,_ofst_v_=11;
	
	_rdat_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG1,0x52,0x42);
	if(_rdat_ == 0){					//... 3 x 3
		for(_t1_=0;_t1_<3;_t1_++){
			for(_t2_=0;_t2_<3;_t2_++){
				WriteOSDBmpY(MASTER,WNDW_RC_NR_BMP_X+(3*_t2_),WNDW_RC_NR_BMP_Y+(11*_t1_),0x00,OSD_SQR_CHK,tbl_osd_nr_typ);
			}
		}
	}
	else if(_rdat_ == 1){			//... cross
		for(_t1_=0;_t1_<3;_t1_++){
			for(_t2_=0;_t2_<3;_t2_++){
				_t3_ = (_t1_+(_t1_<<1))+_t2_;			//... = 3*_t1_ + _t2_
				if((_t3_==0)||(_t3_==2)||(_t3_==6)||(_t3_==8))	_t4_ = OSD_SQR_BLK;
				else	_t4_ = OSD_SQR_CHK;
				WriteOSDBmpY(MASTER,WNDW_RC_NR_BMP_X+(3*_t2_),WNDW_RC_NR_BMP_Y+(11*_t1_),0x00,_t4_,tbl_osd_nr_typ);
			}
		}
	}
	else if(_rdat_ == 2){			//... multiplier
		for(_t1_=0;_t1_<3;_t1_++){
			for(_t2_=0;_t2_<3;_t2_++){
				_t3_ = (_t1_+(_t1_<<1))+_t2_;			//... = 3*_t1_ + _t2_
				if((_t3_==1)||(_t3_==3)||(_t3_==5)||(_t3_==7))	_t4_ = OSD_SQR_BLK;
				else	_t4_ = OSD_SQR_CHK;
				WriteOSDBmpY(MASTER,WNDW_RC_NR_BMP_X+(3*_t2_),WNDW_RC_NR_BMP_Y+(11*_t1_),0x00,_t4_,tbl_osd_nr_typ);
			}
		}
	}
	else if(_rdat_ == 3){			//... vertical bar
		for(_t1_=0;_t1_<3;_t1_++){
			for(_t2_=0;_t2_<3;_t2_++){
				_t3_ = (_t1_+(_t1_<<1))+_t2_;			//... = 3*_t1_ + _t2_
				if((_t3_==1)||(_t3_==4)||(_t3_==7))	_t4_ = OSD_SQR_CHK;
				else	_t4_ = OSD_SQR_BLK;
				WriteOSDBmpY(MASTER,WNDW_RC_NR_BMP_X+(3*_t2_),WNDW_RC_NR_BMP_Y+(11*_t1_),0x00,_t4_,tbl_osd_nr_typ);
			}
		}
	}
}
//==================================================================================
void	SetWndwRcNrOSDWn(U8 _pth)
{
	SetOSDGrp(_pth,WNDW_RC_NR_RDO_X-1,WNDW_RC_NR_RDO_Y,17,4,0x00,str_wndw_grp_wn_typ);
	WriteOSDStr(_pth,MASTER,WNDW_RC_NR_RDO_X+2,WNDW_RC_NR_RDO_Y,0x00,str_wndw_rc_nr_typ1);
	WriteOSDStr(_pth,MASTER,WNDW_RC_NR_RDO_X+2,WNDW_RC_NR_RDO_Y+1,0x00,str_wndw_rc_nr_typ2);
	WriteOSDStr(_pth,MASTER,WNDW_RC_NR_RDO_X+2,WNDW_RC_NR_RDO_Y+2,0x00,str_wndw_rc_nr_typ3);
	WriteOSDStr(_pth,MASTER,WNDW_RC_NR_RDO_X+2,WNDW_RC_NR_RDO_Y+3,0x00,str_wndw_rc_nr_typ4);

	SetWndwRcNrOSDWnRdo(_pth);
	SetWndwRcNrOSDTyp();
}
//==================================================================================
void	SetWndwRcNrOSDAdptChk(U8 _pth)
{
//	U8 _rdat_;
	
//	_rdat_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x50,BIT7);
//	if(_rdat_ == 0)//	_t2_ = OSD_CHK_BOX_BLK;
	if(GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x50,BIT7))				//... TM_MODE=1 (normal mode)
		WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_ADPT_X,WNDW_RC_NR_CHK_ADPT_Y,0x00,OSD_CHK_BOX_BLK);
	else																				//... TM_MODE=0 (adaptive mode)
		WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_ADPT_X,WNDW_RC_NR_CHK_ADPT_Y,0x00,OSD_CHK_BOX_CHK);
}
//==================================================================================
void	SetWndwRcNrOSDAdpt(U8 _pth)
{
	WriteOSDStr(_pth,MASTER,WNDW_RC_NR_CHK_ADPT_X+2,WNDW_RC_NR_CHK_ADPT_Y,0x00,str_wndw_rc_nr_adpt);
	SetWndwRcNrOSDAdptChk(_pth);
}
//==================================================================================
void	SetWndwRcNrOSDChChk(U8 _pth, U8 _ch)
{
	U8 _dvc_, _ch_;
	U8 _rdat_;
	U8 _t1_=0, _t2_;
	
	SetCrntDvcCh(_ch,&_dvc_,&_ch_);
	_rdat_ = GetAsicFlgType(_dvc_,DVC_PG1,0x60+3*_ch_,BIT2);

	if(_rdat_==0)	_t2_ = OSD_CHK_BOX_BLK;
	else	_t2_ = OSD_CHK_BOX_CHK;
#if	defined(__4CH__)
	if(_ch > 3){
		_t1_ = OSD_MIX;
		_t2_ = OSD_CHK_BOX_BLK;
	}
#endif
	if(_ch < 8)	WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_ch<<1),WNDW_RC_NR_CHK_CH07_Y,_t1_,_t2_);
	else	WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+((_ch-8)<<1),WNDW_RC_NR_CHK_CH8F_Y,_t1_,_t2_);
}
//==================================================================================
void	SetWndwRcNrOSDCh(U8 _pth)
{
	U8 _t1_=0, _t2_, _t3_;//, _t4_;

	SetOSDGrp(_pth,WNDW_RC_NR_CHK_CH_X-1,WNDW_RC_NR_CHK_CH07_Y-1,17,4,0x00,str_wndw_grp_ch_en);

//	_rdat_l_ = (U8)(_data&0x00ff);//ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5d);
//	_rdat_h_ = (U8)((_data&0xff00)>>8);//ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5c);
	for(_t2_=0;_t2_<16;_t2_++){
#if	defined(__4CH__)
		if(_t2_ > 3)	_t1_ = OSD_MIX;
#endif
		if(_t2_ < 8){
			WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_t2_<<1),WNDW_RC_NR_CHK_CH07_Y-1,_t1_,0x10+_t2_);
//			if(BitSet(_rdat_l_,BIT0<<_t2_))	_t2_ = OSD_CHK_BOX_CHK;
//			else	_t2_ = OSD_CHK_BOX_BLK;
//			WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_t2_<<1),WNDW_RC_NR_CHK_CHK07_Y,_t1_,_t2_);
		}
		else{
			_t3_ = _t2_-8;
			if(_t3_ < 2)	WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_t3_<<1),WNDW_RC_NR_CHK_CH8F_Y-1,_t1_,0x18+_t3_);
			else	WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_t3_<<1),WNDW_RC_NR_CHK_CH8F_Y-1,_t1_,0x1f+_t3_);
//			if(BitSet(_rdat_h_,BIT0<<_t3_))	_t2_ = OSD_CHK_BOX_CHK;
//			else	_t2_ = OSD_CHK_BOX_BLK;
//			WriteOSDFnt(_pth,MASTER,WNDW_RC_NR_CHK_CH_X+(_t3_<<1),WNDW_RC_NR_CHK_CHK8F_Y,_t1_,_t2_);
		}
		SetWndwRcNrOSDChChk(_pth,_t2_);
	}
}
//==================================================================================
//==================================================================================
void	CreateWndwRcNr(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

	WriteOSDClr(_pth,MASTER,WNDW_RC_NR_POS_X,WNDW_RC_NR_POS_Y,WNDW_RC_NR_L,WNDW_RC_NR_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_RC_NR_POS_X,WNDW_RC_NR_POS_Y,WNDW_RC_NR_L,WNDW_RC_NR_H,str_wndw_ttl_ns_rd);

	mse_flg_on_osd = 0xff;
	SetWndwRcNrOSDWn(_pth);
	SetWndwRcNrOSDAdpt(_pth);
	SetWndwRcNrOSDCh(_pth);
}
//==================================================================================
void	RunWndwRcNr(U8 _pth)
{
	U8 _dvc_, _ch_;
	U8 _t1_, _t2_;//, _t3_;
	U8 _osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

//	RunWndwRcNrOSDChArw(_pth,_osd_pos_x_,_osd_pos_y_);
//	RunWndwRcNrOSDAdptArw(_pth,_osd_pos_x_,_osd_pos_y_);

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_NR_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_NR_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1){
		b_mse_btn_dwn_rb = 0;
		
	}
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_NR_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_NR_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
//			ClearOSD(_pth,WNDW_RC_NR_POS_X,WNDW_RC_NR_POS_Y,WNDW_RC_NR_L,WNDW_RC_NR_H);
//			DisableBox(_pth,BOX_WNDW_QUIT,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_RC_NR_POS_X,WNDW_RC_NR_POS_Y,WNDW_RC_NR_L,WNDW_RC_NR_H);
			WriteOSDClrX(MASTER,WNDW_RC_NR_POS_X-1,WNDW_RC_NR_POS_Y-1,WNDW_RC_NR_L+2,WNDW_RC_NR_H+2);

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else if((_osd_pos_x_ == WNDW_RC_NR_RDO_X)&&
				((_osd_pos_y_ >= WNDW_RC_NR_RDO_Y)&&(_osd_pos_y_ < (WNDW_RC_NR_RDO_Y+4)))){
			_t1_ = _osd_pos_y_ - WNDW_RC_NR_RDO_Y;
			_t2_ = GetAsicVar8Type(wndw_crnt_dvc,DVC_PG1,0x52,0x42);
			if(_t1_ != _t2_){
				for(_t2_=0;_t2_<cmn_max_ch;_t2_++){
					if(!(_t2_&0x03)){
						SetCrntDvcCh(_t2_,&_dvc_,&_ch_);
//						_t2_ = GetAsicVar8Type(_dvc_,DVC_PG1,0x52,0x42);
						SetAsicVar8Type(_dvc_,DVC_PG1,0x52,0x42,_t1_);
					}
				}
				SetWndwRcNrOSDWnRdo(_pth);
				SetWndwRcNrOSDTyp();
			}
		}
		else if((_osd_pos_x_ == WNDW_RC_NR_CHK_ADPT_X)&&(_osd_pos_y_ == WNDW_RC_NR_CHK_ADPT_Y)){
			_t2_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x50,BIT7)^BIT7;
			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				if(!(_t1_&0x03)){
					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					SetAsicFlgType(_dvc_,DVC_PG1,0x50,BIT7,_t2_);
				}
			}
			SetWndwRcNrOSDAdptChk(_pth);
		}
//		else if((_osd_pos_x_ == WNDW_RC_NR_CH_DEC_X)||(_osd_pos_x_ == WNDW_RC_NR_CH_INC_X)){
//			if(_osd_pos_y_ == WNDW_RC_NR_CH_NUM_Y)	RunWndwRcNrCh(_pth,_osd_pos_x_);
//		}
//		else if((_osd_pos_x_ == WNDW_RC_NR_ADPT_DEC_X)||(_osd_pos_x_ == WNDW_RC_NR_ADPT_INC_X)){
//			if((_osd_pos_y_ == WNDW_RC_NR_ADPT_THR_Y)||(_osd_pos_y_ == WNDW_RC_NR_ADPT_SLP_Y)){
//				if(_osd_pos_x_ == WNDW_RC_NR_ADPT_DEC_X)	RunWndwRcNrDec(_pth,_osd_pos_y_);
//				else	RunWndwRcNrInc(_pth,_osd_pos_y_);
//			}
//		}
//		else if((_osd_pos_x_ == WNDW_RC_NR_CHK_FLT_X)&&(_osd_pos_y_ == WNDW_RC_NR_CHK_FLT_Y)){
//			_t1_ = GetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+3*wndw_crnt_ch,BIT2)^BIT2;
//			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+3*wndw_crnt_ch,BIT2,_t1_);
//			SetWndwRcNrOSDChChk(_pth);
//		}
//		else if(b_rc_fm_ls_sk == LOSS_SKIP_MANL){
#if	defined(__4CH__)
		else if(_osd_pos_y_ == WNDW_RC_NR_CHK_CH07_Y){
			if((_osd_pos_x_ >= WNDW_RC_NR_CHK_CH_X)&&(_osd_pos_x_ <= (WNDW_RC_NR_CHK_CH_X+6))){
				_t1_ = _osd_pos_x_ - WNDW_RC_NR_CHK_CH_X;
				if(BitClear(_t1_,BIT0)){
					_t1_ >>= 1;
#elif	defined(__16CH__)
		else if((_osd_pos_y_ == WNDW_RC_NR_CHK_CH07_Y)||(_osd_pos_y_ == WNDW_RC_NR_CHK_CH8F_Y)){
			if((_osd_pos_x_ >= WNDW_RC_NR_CHK_CH_X)&&(_osd_pos_x_ <= (WNDW_RC_NR_CHK_CH_X+14))){
				_t1_ = _osd_pos_x_ - WNDW_RC_NR_CHK_CH_X;
				if(BitClear(_t1_,BIT0)){
					_t1_ >>= 1;
					if(_osd_pos_y_ == WNDW_RC_NR_CHK_CH8F_Y)	_t1_ += 8;
#endif
//					_rdat_ = GetWndwRcFmLsSkMn();
//					_rdat_ ^= 0x0001<<_t1_;
//					SetWndwRcFmLsOSDSkMn(_rdat_);
//					SetWndwRcFmLsSkMn(_rdat_);
					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					_t2_ = ReadAsicByte(_dvc_,DVC_PG1,0x60+3*_ch_)^BIT2;
					WriteAsicByte(_dvc_,DVC_PG1,0x60+3*_ch_,_t2_);
					SetWndwRcNrOSDChChk(_pth,_t1_);
				}
			}
		}
//		}
	}
	if(b_mse_clk_rb == 1){
		b_mse_clk_rb = 0;
		
	}
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

//		if((_osd_pos_x_ == WNDW_RC_NR_CH_DEC_X)||(_osd_pos_x_ == WNDW_RC_NR_CH_INC_X)){
//			if(_osd_pos_y_ == WNDW_RC_NR_CH_NUM_Y)	RunWndwRcNrCh(_pth,_osd_pos_x_);
//		}
//		else if((_osd_pos_x_ == WNDW_RC_NR_ADPT_DEC_X)||(_osd_pos_x_ == WNDW_RC_NR_ADPT_INC_X)){
//			if((_osd_pos_y_ == WNDW_RC_NR_ADPT_THR_Y)||(_osd_pos_y_ == WNDW_RC_NR_ADPT_SLP_Y)){
//				if(_osd_pos_x_ == WNDW_RC_NR_ADPT_DEC_X)	RunWndwRcNrDec(_pth,_osd_pos_y_);
//				else	RunWndwRcNrInc(_pth,_osd_pos_y_);
//			}
//		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

