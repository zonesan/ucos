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
int	b_rc_in_bnd;
int	b_rc_in_dgtl;
int	b_rc_in_anlg;
int	b_rc_in_dtct;
int	b_rc_in_usr;
int	b_rc_in_run;



//==================================================================================
//						Display wndwdow function description
//==================================================================================
void	SetWndwRcInOSDType(U8 _pth)
{
	U8 _t1_;

	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X+2,WNDW_RC_IN_CHK_TYPE_Y,0x00,str_wndw_in_bnd_indx);
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X+2,WNDW_RC_IN_CHK_TYPE_Y+1,0x00,str_wndw_in_dgtl_indx);
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X+2,WNDW_RC_IN_CHK_TYPE_Y+2,0x00,str_wndw_in_anlg_indx);

	if(b_rc_in_bnd == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X,WNDW_RC_IN_CHK_TYPE_Y,0x00,_t1_);

	if(b_rc_in_dgtl == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X,WNDW_RC_IN_CHK_TYPE_Y+1,0x00,_t1_);

	if(b_rc_in_anlg == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_TYPE_X,WNDW_RC_IN_CHK_TYPE_Y+2,0x00,_t1_);
}
//==================================================================================
void	SetWndwRcInOSDAnlgChk(U8 _pth)
{
	U8 _t1_, _t2_;
	
	if(b_rc_in_anlg == ON)	_t2_ = 0x00;
	else	_t2_ = OSD_MIX;
	
	if(b_rc_in_dtct == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_CHK_DT_Y,_t2_,_t1_);

	if(b_rc_in_usr == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_CHK_UR_Y,_t2_,_t1_);

	if(b_rc_in_run == ON)	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_CHK_RN_Y,_t2_,_t1_);
}
//==================================================================================
void	SetWndwRcInOSDAnlg(U8 _pth)
{
	U8 _t1_;

	if(b_rc_in_anlg == ON)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0+2,WNDW_RC_IN_CHK_RN_Y,_t1_,str_wndw_in_run_in_clk);
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0+2,WNDW_RC_IN_CHK_DT_Y,_t1_,str_wndw_in_dtct_id);
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0+2,WNDW_RC_IN_CHK_UR_Y,_t1_,str_wndw_in_usr_id);

	SetWndwRcInOSDAnlgChk(_pth);
}
//==================================================================================
void	SetWndwRcInOSDSpec(U8 _pth)
{
	U8 _t1_;//, _t2_;

	if(b_rc_in_anlg == ON)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_HS_Y,_t1_,str_wndw_in_h_strt);
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_PX_Y,_t1_,str_wndw_in_pxl_bit);

	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_VS_Y,_t1_,str_wndw_in_v_strt);
//	WriteOSDStr(_pth,MASTER,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_LN_Y,_t1_,str_wndw_in_line);
}
//==================================================================================
void	RunWndwRcInOSDArwH(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;

	if(b_rc_in_anlg == ON)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_RC_IN_DEC_X,WNDW_RC_IN_INC_X,WNDW_RC_IN_HS_Y,MSE_ON_OSD_HS);
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_RC_IN_DEC_X,WNDW_RC_IN_INC_X,WNDW_RC_IN_PX_Y,MSE_ON_OSD_PX);
}
//==================================================================================
void	RunWndwRcInOSDArwV(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;

	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_RC_IN_DEC_X,WNDW_RC_IN_INC_X,WNDW_RC_IN_VS_Y,MSE_ON_OSD_VS);
//	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_RC_IN_DEC_X,WNDW_RC_IN_INC_X,WNDW_RC_IN_LN_Y,MSE_ON_OSD_LN);
}
//==================================================================================
void	SetWndwRcInOSDNumHs(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

	if(b_rc_in_anlg == ON)	_t2_ = 0x00;
	else	_t2_ = OSD_MIX;
	
	_t1_ = _data%100;
	_t1_ = ConvDec2Bcd(_t1_);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X,WNDW_RC_IN_HS_Y,_t2_,(_t1_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X+1,WNDW_RC_IN_HS_Y,_t2_,(_t1_&0x0f)+0x10);
	_t1_ = _data/100;
	if(_t1_ == 0)	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X-1,WNDW_RC_IN_HS_Y,OSD_CLR,_t1_+0x10);
	else	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X-1,WNDW_RC_IN_HS_Y,_t2_,_t1_+0x10);
}
//==================================================================================
void	SetWndwRcInOSDNumPx(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

	if(b_rc_in_anlg == ON)	_t2_ = 0x00;
	else	_t2_ = OSD_MIX;
	
	_t1_ = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X,WNDW_RC_IN_PX_Y,_t2_,(_t1_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X+1,WNDW_RC_IN_PX_Y,_t2_,(_t1_&0x0f)+0x10);
}
//==================================================================================
void	SetWndwRcInOSDNumVs(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON))	_t2_ = 0x00;
	else	_t2_ = OSD_MIX;
	
	_t1_ = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X,WNDW_RC_IN_VS_Y,_t2_,(_t1_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X+1,WNDW_RC_IN_VS_Y,_t2_,(_t1_&0x0f)+0x10);
}
//==================================================================================
//void	SetWndwRcInOSDNumLn(U8 _pth, U8 _data)
//{
//	U8 _t1_;
//
//	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON))	_t1_ = 0x00;
//	else	_t1_ = OSD_MIX;
//	
//	WriteOSDFnt(_pth,MASTER,WNDW_RC_IN_NUM_X,WNDW_RC_IN_LN_Y,_t1_,_data+0x10);
//}
//==================================================================================
void	SetWndwRcInOSDNum(U8 _pth)
{
	U8 _rdat_;

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x81);
//	SetWndwRcInOSDNumHs(_pth,_rdat_);
//
//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x82);
//	SetWndwRcInOSDNumPx(_pth,_rdat_&0x1f);
//
//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x83);
//	SetWndwRcInOSDNumVs(_pth,_rdat_&0x1f);
////	SetWndwRcInOSDNumLn(_pth,(_rdat_&0xe0)>>5);

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t1_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x81,0xff);
	SetWndwRcInOSDNumHs(_pth,_rdat_);

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t2_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x82,0x1f);
	SetWndwRcInOSDNumPx(_pth,_rdat_);

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t3_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x83,0x1f);
	SetWndwRcInOSDNumVs(_pth,_rdat_);
}
//==================================================================================
//==================================================================================
void	InitWndwRcInFlg(void)
{
	U8 _t1_;
	
	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x55);
	if(BitSet(_t1_,BIT4))	b_rc_in_bnd = ON;
	else	b_rc_in_bnd = OFF;

	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
	if(BitSet(_t1_,BIT6))	b_rc_in_anlg = ON;		//... VIS_AUTO_EN
	else	b_rc_in_anlg = OFF;
	if(BitSet(_t1_,BIT4))	b_rc_in_dtct = ON;
	else	b_rc_in_dtct = OFF;
	if(BitSet(_t1_,BIT3))	b_rc_in_usr = ON;
	else	b_rc_in_usr = OFF;
	if(BitSet(_t1_,BIT2))	b_rc_in_dgtl = ON;
	else	b_rc_in_dgtl = OFF;

//	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
	if(BitSet(_t1_,BIT1))	b_rc_in_run = ON;
	else	b_rc_in_run = OFF;
}
//==================================================================================
//==================================================================================
void	CreateWndwRcIn(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

	WriteOSDClr(_pth,MASTER,WNDW_RC_IN_POS_X,WNDW_RC_IN_POS_Y,WNDW_RC_IN_L,WNDW_RC_IN_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_RC_IN_POS_X,WNDW_RC_IN_POS_Y,WNDW_RC_IN_L,WNDW_RC_IN_H,str_wndw_ttl_rc_in);

	InitWndwRcInFlg();

	SetWndwRcInOSDType(_pth);
	SetWndwRcInOSDAnlg(_pth);

	SetOSDWndw(_pth,OSD_WNDW_TYPE_DECO,WNDW_RC_IN_CHK_X0,WNDW_RC_IN_HS_Y,17,3,str_null);
	SetWndwRcInOSDSpec(_pth);
	
	mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;
	
	RunWndwRcInOSDArwH(_pth,0,0);
	RunWndwRcInOSDArwV(_pth,0,0);
	SetWndwRcInOSDNum(_pth);
}
//==================================================================================
void	RunWndwRcInDec(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;
	U8 _dvc_, _ch_;
	U8 _t1_;
	
	if(b_rc_in_anlg == ON){
		if(_osd_pos_y == WNDW_RC_IN_HS_Y){
			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x81);
//			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x81,0xff);
			if(_rdat_ > 0)	_rdat_--;
			else	_rdat_ = 0xff;
			SetWndwRcInOSDNumHs(_pth,_rdat_);

			WriteAsicByte(cmn_dvc,DVC_PG1,0x81,_rdat_);
		}
		else if(_osd_pos_y == WNDW_RC_IN_PX_Y){
//			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x82);
			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x82,0x1f);
			if(_rdat_ > 0)	_rdat_--;
			else	_rdat_ = 0x1f;
			SetWndwRcInOSDNumPx(_pth,_rdat_);

			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(_t1_&0x03))
					SetAsicFlgType(_dvc_,DVC_PG1,0x82,0x1f,_rdat_);
			}
		}
	}
	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON)){
//		if((_osd_pos_y == WNDW_RC_IN_VS_Y)||(_osd_pos_y == WNDW_RC_IN_LN_Y)){
		if(_osd_pos_y == WNDW_RC_IN_VS_Y){
//			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x83);
			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x83,0x1f);
//			if(_osd_pos_y == WNDW_RC_IN_VS_Y){
				_t1_ = _rdat_&0x1f;
				if(_t1_ > 0)	_t1_--;
				else	_t1_ = 0x1f;
				SetWndwRcInOSDNumVs(_pth,_t1_);
				_rdat_ = (_rdat_&0xe0)|_t1_;
//			}
//			else{// if(_osd_pos_y == WNDW_RC_IN_LN_Y){
//				_t1_ = (_rdat_&0xe0)>>5;
//				if(_t1_ > 0)	_t1_--;
//				else	_t1_ = 7;
//				SetWndwRcInOSDNumLn(_pth,_t1_);
//				_rdat_ = (_rdat_&0x1f)|(_t1_<<5);
//			}

			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(_t1_&0x03))
					SetAsicFlgType(_dvc_,DVC_PG1,0x83,0x1f,_rdat_);
			}
		}
	}
}
//==================================================================================
void	RunWndwRcInInc(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;
	U8 _dvc_, _ch_;
	U8 _t1_;
	
	if(b_rc_in_anlg == ON){
		if(_osd_pos_y == WNDW_RC_IN_HS_Y){
			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x81);
//			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x81,0xff);
			if(_rdat_ < 0xff)	_rdat_++;
			else	_rdat_ = 0;
			SetWndwRcInOSDNumHs(_pth,_rdat_);

			WriteAsicByte(cmn_dvc,DVC_PG1,0x81,_rdat_);
		}
		else if(_osd_pos_y == WNDW_RC_IN_PX_Y){
//			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x82);
			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x82,0x1f);
			if(_rdat_ < 0x1f)	_rdat_++;
			else	_rdat_ = 0;
			SetWndwRcInOSDNumPx(_pth,_rdat_);

			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(_t1_&0x03))
					SetAsicFlgType(_dvc_,DVC_PG1,0x82,0x1f,_rdat_);
			}
		}
	}
	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON)){
//		if((_osd_pos_y == WNDW_RC_IN_VS_Y)||(_osd_pos_y == WNDW_RC_IN_LN_Y)){
		if(_osd_pos_y == WNDW_RC_IN_VS_Y){
//			_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x83);
			_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x83,0x1f);
//			if(_osd_pos_y == WNDW_RC_IN_VS_Y){
				_t1_ = _rdat_&0x1f;
				if(_t1_ < 0x1f)	_t1_++;
				else	_t1_ = 0;
				SetWndwRcInOSDNumVs(_pth,_t1_);
				_rdat_ = (_rdat_&0xe0)|_t1_;
//			}
//			else{// if(_osd_pos_y == WNDW_RC_IN_LN_Y){
//				_t1_ = (_rdat_&0xe0)>>5;
//				if(_t1_ < 7)	_t1_++;
//				else	_t1_ = 0;
//				SetWndwRcInOSDNumLn(_pth,_t1_);
//				_rdat_ = (_rdat_&0x1f)|(_t1_<<5);
//			}

			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
				if(!(_t1_&0x03))
					SetAsicFlgType(_dvc_,DVC_PG1,0x83,0x1f,_rdat_);
			}
		}
	}
}
//==================================================================================
void	RunWndwRcIn(U8 _pth)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	
	if(b_rc_in_anlg == ON)
		RunWndwRcInOSDArwH(_pth,_osd_pos_x_,_osd_pos_y_);
	if((b_rc_in_dgtl == ON)||(b_rc_in_anlg == ON))
		RunWndwRcInOSDArwV(_pth,_osd_pos_x_,_osd_pos_y_);

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_IN_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_IN_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_IN_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_IN_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
			WriteOSDClr(_pth,MASTER,WNDW_RC_IN_POS_X,WNDW_RC_IN_POS_Y,WNDW_RC_IN_L,WNDW_RC_IN_H);
			WriteOSDClrX(MASTER,WNDW_RC_IN_POS_X-1,WNDW_RC_IN_POS_Y-1,WNDW_RC_IN_L+2,WNDW_RC_IN_H+2);
//			DisableBox(_pth,IN_BOX_DECO,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else if(_osd_pos_x_ == WNDW_RC_IN_CHK_TYPE_X){
			if((_osd_pos_y_ >= WNDW_RC_IN_CHK_TYPE_Y)&&(_osd_pos_y_ <= (WNDW_RC_IN_CHK_TYPE_Y+2))){
				if(_osd_pos_y_ == WNDW_RC_IN_CHK_TYPE_Y){
					b_rc_in_bnd ^= 1;

					_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x55);
					if(b_rc_in_bnd == ON)	SetBit(_t1_,BIT4);
					else	ClearBit(_t1_,BIT4);
					WriteAsicByte(cmn_dvc,DVC_PG1,0x55,_t1_);
				}
				else{
					_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
					if(_osd_pos_y_ == (WNDW_RC_IN_CHK_TYPE_Y+1)){
						b_rc_in_dgtl ^= 1;

						if(b_rc_in_dgtl == ON)	SetBit(_t1_,BIT2);
						else	ClearBit(_t1_,BIT2);
					}
					else{
						b_rc_in_anlg ^= 1;

						if(b_rc_in_anlg == ON){
							SetBit(_t1_,BIT6|BIT4|BIT3|BIT1);
							b_rc_in_dtct = ON;
							b_rc_in_usr = ON;
							b_rc_in_run = ON;
						}
						else{
							ClearBit(_t1_,BIT6|BIT4|BIT3|BIT1);
							b_rc_in_dtct = OFF;
							b_rc_in_usr = OFF;
							b_rc_in_run = OFF;
						}

						SetWndwRcInOSDAnlg(_pth);
					}
					SetWndwRcInOSDSpec(_pth);
					
					mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;
					
					RunWndwRcInOSDArwH(_pth,0,0);
					RunWndwRcInOSDArwV(_pth,0,0);
					SetWndwRcInOSDNum(_pth);

					WriteAsicByte(cmn_dvc,DVC_PG1,0x80,_t1_);
				}
				
				SetWndwRcInOSDType(_pth);
			}
		}
		else if(_osd_pos_x_ == WNDW_RC_IN_CHK_X0){
			if(b_rc_in_anlg == ON){
//				if((_osd_pos_y_ == WNDW_RC_IN_CHK_DT_Y)||(_osd_pos_y_ == WNDW_RC_IN_CHK_RN_Y)){
				if((_osd_pos_y_ == WNDW_RC_IN_CHK_DT_Y)||(_osd_pos_y_ == WNDW_RC_IN_CHK_UR_Y)||(_osd_pos_y_ == WNDW_RC_IN_CHK_RN_Y)){
						_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
					if(_osd_pos_y_ == WNDW_RC_IN_CHK_DT_Y){
						b_rc_in_dtct ^= 1;

//						_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
						if(b_rc_in_dtct == ON)	SetBit(_t1_,BIT4);
						else	ClearBit(_t1_,BIT4);
//						WriteAsicByte(cmn_dvc,DVC_PG1,0x80,_t1_);
					}
					else if(_osd_pos_y_ == WNDW_RC_IN_CHK_UR_Y){
						b_rc_in_usr ^= 1;

//						_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
						if(b_rc_in_usr == ON)	SetBit(_t1_,BIT3);
						else	ClearBit(_t1_,BIT3);
//						WriteAsicByte(cmn_dvc,DVC_PG1,0x80,_t1_);
					}
					else{
						b_rc_in_run ^= 1;

//						_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x80);
						if(b_rc_in_run == ON)	SetBit(_t1_,BIT1);
						else	ClearBit(_t1_,BIT1);
//						WriteAsicByte(cmn_dvc,DVC_PG1,0x80,_t1_);
					}
						WriteAsicByte(cmn_dvc,DVC_PG1,0x80,_t1_);
					
					SetWndwRcInOSDAnlgChk(_pth);
				}
			}
		}
		else if((_osd_pos_y_ >= WNDW_RC_IN_HS_Y)&&(_osd_pos_y_ <= WNDW_RC_IN_VS_Y)){
			if(_osd_pos_x_ == WNDW_RC_IN_DEC_X)	RunWndwRcInDec(_pth,_osd_pos_y_);
			else if(_osd_pos_x_ == WNDW_RC_IN_INC_X)	RunWndwRcInInc(_pth,_osd_pos_y_);
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

			if((_osd_pos_y_ >= WNDW_RC_IN_HS_Y)&&(_osd_pos_y_ <= WNDW_RC_IN_VS_Y)){
				if(_osd_pos_x_ == WNDW_RC_IN_DEC_X)	RunWndwRcInDec(_pth,_osd_pos_y_);
				else if(_osd_pos_x_ == WNDW_RC_IN_INC_X)	RunWndwRcInInc(_pth,_osd_pos_y_);
			}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

