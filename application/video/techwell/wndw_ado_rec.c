#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ado_rec.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"

#if defined(__WTH_ADO__)
//==================================================================================
//						Audio Rcord wndwdow function description
//==================================================================================
void	SetWndwAdoRcOSDSrChk(U8 _pth, U8 _tgl)
{
	if(_tgl != 0){
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_SR_RDO_8_X,WNDW_ADO_RC_SR_RDO_Y,0x00,OSD_RDO_BLK);//OSD_CHK_BOX_BLK);
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_SR_RDO_16_X,WNDW_ADO_RC_SR_RDO_Y,0x00,OSD_RDO_CHK);//OSD_CHK_BOX_CHK);
	}
	else{
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_SR_RDO_8_X,WNDW_ADO_RC_SR_RDO_Y,0x00,OSD_RDO_CHK);//OSD_CHK_BOX_CHK);
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_SR_RDO_16_X,WNDW_ADO_RC_SR_RDO_Y,0x00,OSD_RDO_BLK);//OSD_CHK_BOX_BLK);
	}
}
//==================================================================================
void	SetWndwAdoRcOSDSr(U8 _pth)
{
	U8 _t1_;
	
	SetOSDGrp(_pth,WNDW_ADO_RC_SR_RDO_8_X-1,WNDW_ADO_RC_SR_RDO_Y,16,1,0x00,str_wndw_grp_smpl_rto);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_SR_RDO_8_X+2,WNDW_ADO_RC_SR_RDO_Y,0x00,str_wndw_ado_rc_8k);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_SR_RDO_16_X+2,WNDW_ADO_RC_SR_RDO_Y,0x00,str_wndw_ado_rc_16k);

	_t1_ = GetAsicFlgType(MASTER,DVC_PG0,0x62,BIT2);
	SetWndwAdoRcOSDSrChk(_pth,_t1_);
}
//==================================================================================
//==================================================================================
void	SetWndwAdoRcOSDBwChk(U8 _pth, U8 _tgl)
{
	if(_tgl != 0){
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_BW_RDO_8_X,WNDW_ADO_RC_BW_RDO_Y,0x00,OSD_RDO_CHK);//OSD_CHK_BOX_CHK);
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_BW_RDO_16_X,WNDW_ADO_RC_BW_RDO_Y,0x00,OSD_RDO_BLK);//OSD_CHK_BOX_BLK);
	}
	else{
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_BW_RDO_8_X,WNDW_ADO_RC_BW_RDO_Y,0x00,OSD_RDO_BLK);//OSD_CHK_BOX_BLK);
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_BW_RDO_16_X,WNDW_ADO_RC_BW_RDO_Y,0x00,OSD_RDO_CHK);//OSD_CHK_BOX_CHK);
	}
}
//==================================================================================
void	SetWndwAdoRcOSDBw(U8 _pth)
{
	U8 _t1_;
	
	SetOSDGrp(_pth,WNDW_ADO_RC_BW_RDO_8_X-1,WNDW_ADO_RC_BW_RDO_Y,16,1,0x00,str_wndw_grp_bit_wdth);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_BW_RDO_8_X+2,WNDW_ADO_RC_BW_RDO_Y,0x00,str_wndw_ado_rc_8bit);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_BW_RDO_16_X+2,WNDW_ADO_RC_BW_RDO_Y,0x00,str_wndw_ado_rc_16bit);

	_t1_ = GetAsicFlgType(MASTER,DVC_PG0,0x62,BIT1);
	SetWndwAdoRcOSDBwChk(_pth,_t1_);
}
//==================================================================================
//==================================================================================
void	SetWndwAdoRcOSDMcChk(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

	for(_t1_=0;_t1_<4;_t1_++){
		if(_t1_ == _data)		_t2_ = OSD_RDO_CHK;
		else	_t2_ = OSD_RDO_BLK;
		WriteOSDFnt(_pth,MASTER,WNDW_ADO_RC_MC_RDO_X,WNDW_ADO_RC_MC_RDO_Y+_t1_,0x00,_t2_);
	}
}
//==================================================================================
void	SetWndwAdoRcOSDMc(U8 _pth)
{
	U8 _t1_;
	
	SetOSDGrp(_pth,WNDW_ADO_RC_MC_RDO_X-1,WNDW_ADO_RC_MC_RDO_Y,16,4,0x00,str_wndw_grp_mlt_ch);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_MC_RDO_X+2,WNDW_ADO_RC_MC_RDO_Y,0x00,str_wndw_ado_rc_2ch);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_MC_RDO_X+2,WNDW_ADO_RC_MC_RDO_Y+1,0x00,str_wndw_ado_rc_4ch);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_MC_RDO_X+2,WNDW_ADO_RC_MC_RDO_Y+2,0x00,str_wndw_ado_rc_8ch);
	WriteOSDStr(_pth,MASTER,WNDW_ADO_RC_MC_RDO_X+2,WNDW_ADO_RC_MC_RDO_Y+3,0x00,str_wndw_ado_rc_16ch);

	_t1_ = GetAsicVar8Type(MASTER,DVC_PG0,0x63,0x02);
	SetWndwAdoRcOSDMcChk(_pth,_t1_);
}
//==================================================================================
//==================================================================================
void	CreateWndwAdoRc(U8 _pth)
{
//	U8 _t1_;
	
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

//	ClearOSD(X_PATH,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,WNDW_ADO_RC_L,WNDW_ADO_RC_H);
//	SetOSDWndw(X_PATH,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,
//		WNDW_ADO_RC_L,WNDW_ADO_RC_H,str_wndw_ttl_ado_rec);
	WriteOSDClr(_pth,MASTER,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,WNDW_ADO_RC_L,WNDW_ADO_RC_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,WNDW_ADO_RC_L,WNDW_ADO_RC_H,str_wndw_ttl_ado_rc);

	SetWndwAdoRcOSDSr(_pth);
	SetWndwAdoRcOSDBw(_pth);
	SetWndwAdoRcOSDMc(_pth);
}
//==================================================================================
//==================================================================================
void	RunWndwAdoRc(U8 _pth)//U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	U8 _t1_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_RC_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_RC_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_ADO_RC_CLOS_X)&&(_osd_pos_y_ == WNDW_ADO_RC_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
//			ClearOSD(X_PATH,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,WNDW_ADO_RC_L,WNDW_ADO_RC_H);
//			DisableBox(X_PATH,BOX_WNDW_FRM-2,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_ADO_RC_POS_X,WNDW_ADO_RC_POS_Y,WNDW_ADO_RC_L,WNDW_ADO_RC_H);
			WriteOSDClrX(MASTER,WNDW_ADO_RC_POS_X-1,WNDW_ADO_RC_POS_Y-1,WNDW_ADO_RC_L+2,WNDW_ADO_RC_H+2);

			SetOSDNoMn();
//
//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else{
			if(_osd_pos_y_ == WNDW_ADO_RC_SR_RDO_Y){
				if((_osd_pos_x_ == WNDW_ADO_RC_SR_RDO_8_X)||(_osd_pos_x_ == WNDW_ADO_RC_SR_RDO_16_X)){
					if(_osd_pos_x_ == WNDW_ADO_RC_SR_RDO_8_X)	_t1_ = 0x00;
					else	_t1_ = BIT2;
					SetAsicFlgType(MASTER,DVC_PG0,0x62,BIT2,_t1_);
					SetAsicFlgType(MASTER,DVC_PG0,0x6c,BIT2,_t1_);
					SetWndwAdoRcOSDSrChk(_pth,_t1_);
				}
			}
			else if(_osd_pos_y_ == WNDW_ADO_RC_BW_RDO_Y){
				if((_osd_pos_x_ == WNDW_ADO_RC_BW_RDO_8_X)||(_osd_pos_x_ == WNDW_ADO_RC_BW_RDO_16_X)){
					if(_osd_pos_x_ == WNDW_ADO_RC_BW_RDO_8_X)	_t1_ = BIT1;
					else	_t1_ = 0x00;
					SetAsicFlgType(MASTER,DVC_PG0,0x62,BIT1,_t1_);
					SetAsicFlgType(MASTER,DVC_PG0,0x6c,BIT1,_t1_);
					SetWndwAdoRcOSDBwChk(_pth,_t1_);
				}
			}
			else if(_osd_pos_x_ == WNDW_ADO_RC_MC_RDO_X){
				if((_osd_pos_y_ >= WNDW_ADO_RC_MC_RDO_Y)&&(_osd_pos_y_ < (WNDW_ADO_RC_MC_RDO_Y+4))){
//					if(_osd_pos_y_ == WNDW_ADO_RC_MC_RDO_Y)		_t1_ = 0;
//					else	_t1_ = 1;
					_t1_ = _osd_pos_y_ - WNDW_ADO_RC_MC_RDO_Y;
					SetAsicVar8Type(MASTER,DVC_PG0,0x63,0x01,_t1_);
					SetWndwAdoRcOSDMcChk(_pth,_t1_);
				}
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

#endif
