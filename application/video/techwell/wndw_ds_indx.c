#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ds_indx.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"

//==================================================================================
U8 wndw_ds_in_type = IN_TYPE_REC;

//==================================================================================
//						Display wndwdow function description
//==================================================================================
void	SetWndwDsInOSDTypeRdo(U8 _pth)
{
	U8 _t1_, _t2_;
 	for(_t1_=0;_t1_<3;_t1_++){
		if(_t1_ == wndw_ds_in_type)		_t2_ = OSD_RDO_CHK;
		else	_t2_ = OSD_RDO_BLK;
		
		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_TYPE_X,WNDW_DS_IN_RDO_TYPE_Y+_t1_,0x00,_t2_);
	}
}
//==================================================================================
void	SetWndwDsInOSDType(U8 _pth)
{
//	U8 _t1_, _t2_;

	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_RDO_TYPE_X+2,WNDW_DS_IN_RDO_TYPE_Y+IN_TYPE_REC,0x00,str_wndw_cmn_same_as_rec);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_RDO_TYPE_X+2,WNDW_DS_IN_RDO_TYPE_Y+IN_TYPE_DGTL,0x00,str_wndw_in_dgtl_indx);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_RDO_TYPE_X+2,WNDW_DS_IN_RDO_TYPE_Y+IN_TYPE_ANLG,0x00,str_wndw_in_anlg_indx);

// 	for(_t1_=0;_t1_<3;_t1_++){
//		if(_t1_ == wndw_ds_in_type)		_t2_ = OSD_RDO_CHK;
//		else	_t2_ = OSD_RDO_BLK;
//		
//		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_TYPE_X,WNDW_DS_IN_RDO_TYPE_Y+_t1_,0x00,_t2_);
//	}
	SetWndwDsInOSDTypeRdo(_pth);
}
//==================================================================================
void	SetWndwDsInOSDAnlgRdo(U8 _pth)
{
	U8 _t1_;//, _t2_;
	
	if(wndw_ds_in_type < IN_TYPE_ANLG)	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;
	
	if(b_ds_in_auto == ON){
		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_DT_X,WNDW_DS_IN_RDO_DT_Y,_t1_,OSD_RDO_CHK);
		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_DT_X,WNDW_DS_IN_RDO_DT_Y+1,_t1_,OSD_RDO_BLK);
	}
	else{
		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_DT_X,WNDW_DS_IN_RDO_DT_Y,_t1_,OSD_RDO_BLK);
		WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_RDO_DT_X,WNDW_DS_IN_RDO_DT_Y+1,_t1_,OSD_RDO_CHK);
	}
}
//==================================================================================
void	SetWndwDsInOSDAnlgChk(U8 _pth)
{
	U8 _t1_;//, _t2_;
	
//	if(b_ds_in_auto == ON)	_t1_ = OSD_MIX;
//	else	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_CHK_RN_X+2,WNDW_DS_IN_CHK_RN_Y,_t1_,str_wndw_in_run_in_clk);

	if(b_ds_in_run == ON)	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_CHK_RN_X,WNDW_DS_IN_CHK_RN_Y,_t1_,OSD_CHK_BOX_CHK);
	else	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_CHK_RN_X,WNDW_DS_IN_CHK_RN_Y,_t1_,OSD_CHK_BOX_BLK);
	

}
//==================================================================================
void	SetWndwDsInOSDAnlg(U8 _pth)
{
	U8 _t1_;//, _t2_;

	if(wndw_ds_in_type < IN_TYPE_ANLG)	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;
	
//	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_CHK_X0+2,WNDW_DS_IN_CHK_DT_Y,_t1_,str_wndw_in_eror_cor);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_RDO_DT_X+2,WNDW_DS_IN_RDO_DT_Y,_t1_,str_wndw_in_auto_det);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_RDO_DT_X+2,WNDW_DS_IN_RDO_DT_Y+1,_t1_,str_wndw_in_mnl_det);

//	if(b_ds_in_eror == ON)	_t2_ = OSD_CHK_BOX_CHK;
//	else	_t2_ = OSD_CHK_BOX_BLK;
//	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_CHK_X0,WNDW_DS_IN_CHK_DT_Y,_t1_,_t2_);

	SetWndwDsInOSDAnlgRdo(_pth);
	SetWndwDsInOSDAnlgChk(_pth);

//	if(b_ds_in_run == ON)	_t2_ = OSD_CHK_BOX_CHK;
//	else	_t2_ = OSD_CHK_BOX_BLK;
//	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_CHK_X0,WNDW_DS_IN_CHK_RN_Y,_t1_,_t2_);
//
//	if(b_ds_in_run == OFF)	_t1_ = OSD_MIX;
//	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_CHK_RN_X+2,WNDW_DS_IN_CHK_AT_Y,_t1_,str_wndw_in_auto_det);
//	if(b_ds_in_auto == ON)	_t2_ = OSD_CHK_BOX_CHK;
//	else	_t2_ = OSD_CHK_BOX_BLK;
//	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_CHK_RN_X,WNDW_DS_IN_CHK_AT_Y,_t1_,_t2_);
}
//==================================================================================
void	SetWndwDsInOSDSpec(U8 _pth)
{
	U8 _t1_;

//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_TTL_X,WNDW_DS_IN_HS_Y,_t1_,str_wndw_in_h_strt);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_TTL_X,WNDW_DS_IN_PX_Y,_t1_,str_wndw_in_pxl_bit);
	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_TTL_X,WNDW_DS_IN_VS_Y,_t1_,str_wndw_in_v_strt);
//	WriteOSDStr(_pth,MASTER,WNDW_DS_IN_CHK_X0,WNDW_DS_IN_LN_Y,_t1_,str_wndw_in_line);
}
//==================================================================================
void	RunWndwDsInOSDArw(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;

//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_IN_DEC_X,WNDW_DS_IN_INC_X,WNDW_DS_IN_HS_Y,MSE_ON_OSD_HS);
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_IN_DEC_X,WNDW_DS_IN_INC_X,WNDW_DS_IN_PX_Y,MSE_ON_OSD_PX);
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_IN_DEC_X,WNDW_DS_IN_INC_X,WNDW_DS_IN_VS_Y,MSE_ON_OSD_VS);
//	RunMseOnOSDH(_pth|_t1_,_osd_pos_x,_osd_pos_y,WNDW_DS_IN_DEC_X,WNDW_DS_IN_INC_X,WNDW_DS_IN_LN_Y,MSE_ON_OSD_LN);
}
//==================================================================================
void	SetWndwDsInOSDNumHs(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	_t2_ = _data%100;
	_t2_ = ConvDec2Bcd(_t2_);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X,WNDW_DS_IN_HS_Y,_t1_,(_t2_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X+1,WNDW_DS_IN_HS_Y,_t1_,(_t2_&0x0f)+0x10);
	_t2_ = _data/100;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X-1,WNDW_DS_IN_HS_Y,_t1_,_t2_+0x10);
}
//==================================================================================
void	SetWndwDsInOSDNumPx(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	_t2_ = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X,WNDW_DS_IN_PX_Y,_t1_,(_t2_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X+1,WNDW_DS_IN_PX_Y,_t1_,(_t2_&0x0f)+0x10);
}
//==================================================================================
void	SetWndwDsInOSDNumVs(U8 _pth, U8 _data)
{
	U8 _t1_, _t2_;

//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	
	_t2_ = ConvDec2Bcd(_data);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X,WNDW_DS_IN_VS_Y,_t1_,(_t2_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X+1,WNDW_DS_IN_VS_Y,_t1_,(_t2_&0x0f)+0x10);
}
//==================================================================================
//void	SetWndwDsInOSDNumLn(U8 _pth, U8 _data)
//{
//	U8 _t1_;
//
//	if((wndw_ds_in_type == IN_TYPE_ANLG)&&((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)))	_t1_ = 0x00;
//	else	_t1_ = OSD_MIX;
//	
////	WriteOSDFnt(_pth,MASTER,WNDW_DS_IN_NUM_X+1,WNDW_DS_IN_LN_Y,_t1_,_data+0x10);
//}
//==================================================================================
void	SetWndwDsInOSDNum(U8 _pth)
{
	U8 _rdat_;
	U8 _t1_, _t2_, _t3_;
	
	if(wndw_ds_in_type == IN_TYPE_REC){
		_t1_ = 0x81;
		_t2_ = 0x82;
		_t3_ = 0x83;
	}
	else{
		_t1_ = 0x87;
		_t2_ = 0x88;
		_t3_ = 0x89;
	}
//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t1_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,_t1_,0xff);
	SetWndwDsInOSDNumHs(_pth,_rdat_);

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t2_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,_t2_,0x1f);
	SetWndwDsInOSDNumPx(_pth,_rdat_);

//	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,_t3_);
	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,_t3_,0x1f);
	SetWndwDsInOSDNumVs(_pth,_rdat_);
//	SetWndwDsInOSDNumLn(_pth,(_rdat_&0xe0)>>5);
}
//==================================================================================
//==================================================================================
void	InitWndwDsInFlg(void)
{
	U8 _t1_;
	
	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x86);
	if(BitSet(_t1_,BIT7))	b_ds_in_auto = ON;
	else	b_ds_in_auto = OFF;
//	if(BitSet(_t1_,BIT6))	b_ds_in_eror = ON;
//	else	b_ds_in_eror = OFF;
	if(wndw_ds_in_type != IN_TYPE_REC){
		if(BitSet(_t1_,BIT4))	wndw_ds_in_type = IN_TYPE_DGTL;
		else	wndw_ds_in_type = IN_TYPE_ANLG;
//		else if(BitSet(_t1_,BIT5))	wndw_ds_in_type = IN_TYPE_ANLG;
	}
	if(BitSet(_t1_,BIT3))	b_ds_in_run = ON;
	else	b_ds_in_run = OFF;
}
//==================================================================================
//==================================================================================
void	CreateWndwDsIn(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

//	ClearOSD(_pth,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,WNDW_DS_IN_L,WNDW_DS_IN_H);
//	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN|BOX_WNDW_FRM,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,
//		WNDW_DS_IN_L,WNDW_DS_IN_H,str_wndw_ttl_pb_in);
	WriteOSDClr(_pth,MASTER,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,WNDW_DS_IN_L,WNDW_DS_IN_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,WNDW_DS_IN_L,WNDW_DS_IN_H,str_wndw_ttl_pb_indx);

	InitWndwDsInFlg();

	SetWndwDsInOSDType(_pth);
	SetWndwDsInOSDAnlg(_pth);

	SetOSDWndw(_pth,OSD_WNDW_TYPE_DECO,WNDW_DS_IN_TTL_X,WNDW_DS_IN_HS_Y,16,3,str_null);
	SetWndwDsInOSDSpec(_pth);
	
	mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;

	RunWndwDsInOSDArw(_pth,0,0);
	SetWndwDsInOSDNum(_pth);
}
//==================================================================================
void	RunWndwDsInDec(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;
	U8 _t1_;
	
	if(_osd_pos_y == WNDW_DS_IN_HS_Y){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x87);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x87,0xff);
		if(_rdat_ > 0)	_rdat_--;
		else	_rdat_ = 0xff;
		SetWndwDsInOSDNumHs(_pth,_rdat_);

		WriteAsicByte(cmn_dvc,DVC_PG1,0x87,_rdat_);
	}
	else if(_osd_pos_y == WNDW_DS_IN_PX_Y){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x88);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x88,0x1f);
		_t1_ = _rdat_&0x1f;
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = 0x1f;
		SetWndwDsInOSDNumPx(_pth,_t1_);

		_rdat_ = (_rdat_&0xc0)|_t1_;
		WriteAsicByte(cmn_dvc,DVC_PG1,0x88,_rdat_);
	}
	else{
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x89);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x89,0x1f);
		if(_osd_pos_y == WNDW_DS_IN_VS_Y){
			_t1_ = _rdat_&0x1f;
			if(_t1_ > 0)	_t1_--;
			else	_t1_ = 0x1f;
			SetWndwDsInOSDNumVs(_pth,_t1_);
			_rdat_ = (_rdat_&0xe0)|_t1_;
		}
//		else{// if(_osd_pos_y == WNDW_DS_IN_LN_Y){
//			_t1_ = (_rdat_&0xe0)>>5;
//			if(_t1_ > 0)	_t1_--;
//			else	_t1_ = 7;
//			SetWndwDsInOSDNumLn(_pth,_t1_);
//			_rdat_ = (_rdat_&0x1f)|(_t1_<<5);
//		}

		WriteAsicByte(cmn_dvc,DVC_PG1,0x89,_rdat_);
	}
}
//==================================================================================
void	RunWndwDsInInc(U8 _pth, U8 _osd_pos_y)
{
	U8 _rdat_;
	U8 _t1_;
	
	if(_osd_pos_y == WNDW_DS_IN_HS_Y){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x87);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x87,0xff);
		if(_rdat_ < 0xff)	_rdat_++;
		else	_rdat_ = 0;
		SetWndwDsInOSDNumHs(_pth,_rdat_);

		WriteAsicByte(cmn_dvc,DVC_PG1,0x87,_rdat_);
	}
	else if(_osd_pos_y == WNDW_DS_IN_PX_Y){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x88);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x88,0x1f);
		_t1_ = _rdat_&0x1f;
		if(_t1_ < 0x1f)	_t1_++;
		else	_t1_ = 0;
		SetWndwDsInOSDNumPx(_pth,_t1_);

		_rdat_ = (_rdat_&0xc0)|_t1_;
		WriteAsicByte(cmn_dvc,DVC_PG1,0x88,_rdat_);
	}
	else{
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x89);
		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x89,0x1f);
		if(_osd_pos_y == WNDW_DS_IN_VS_Y){
			_t1_ = _rdat_&0x1f;
			if(_t1_ < 0x1f)	_t1_++;
			else	_t1_ = 0;
			SetWndwDsInOSDNumVs(_pth,_t1_);
			_rdat_ = (_rdat_&0xe0)|_t1_;
		}
//		else{// if(_osd_pos_y == WNDW_DS_IN_LN_Y){
//			_t1_ = (_rdat_&0xe0)>>5;
//			if(_t1_ < 7)	_t1_++;
//			else	_t1_ = 0;
//			SetWndwDsInOSDNumLn(_pth,_t1_);
//			_rdat_ = (_rdat_&0x1f)|(_t1_<<5);
//		}

		WriteAsicByte(cmn_dvc,DVC_PG1,0x89,_rdat_);
	}
}
//==================================================================================
void	RunWndwDsIn(U8 _pth)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	
//	if(wndw_ds_in_type == IN_TYPE_ANLG){
//		if((b_ds_in_run == OFF)||(b_ds_in_auto == OFF))	RunWndwDsInOSDArw(_pth,_osd_pos_x_,_osd_pos_y_);
//	}
	if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF))	RunWndwDsInOSDArw(_pth,_osd_pos_x_,_osd_pos_y_);


	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_DS_IN_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_IN_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_DS_IN_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_IN_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
//			ClearOSD(_pth,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,WNDW_DS_IN_L,WNDW_DS_IN_H);
//			DisableBox(_pth,IN_BOX_DECO,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow
			WriteOSDClr(_pth,MASTER,WNDW_DS_IN_POS_X,WNDW_DS_IN_POS_Y,WNDW_DS_IN_L,WNDW_DS_IN_H);
			WriteOSDClrX(MASTER,WNDW_DS_IN_POS_X-1,WNDW_DS_IN_POS_Y-1,WNDW_DS_IN_L+2,WNDW_DS_IN_H+2);
//
			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else if((_osd_pos_x_ == WNDW_DS_IN_RDO_TYPE_X)&&
			((_osd_pos_y_ >= WNDW_DS_IN_RDO_TYPE_Y)&&(_osd_pos_y_ <= (WNDW_DS_IN_RDO_TYPE_Y+IN_TYPE_ANLG)))){

			_t1_ = _osd_pos_y_ - WNDW_DS_IN_RDO_TYPE_Y;
			if(_t1_ != wndw_ds_in_type){
				if((_t1_ < IN_TYPE_ANLG)&&(wndw_ds_in_type < IN_TYPE_ANLG)){
					//... if select one of rec or dgtl, pass OSD process... (-> redrawndwg protection)
					wndw_ds_in_type = _t1_;				//... don't move out of if() sentence
					SetWndwDsInOSDNum(_pth);
				}
				else{
					wndw_ds_in_type = _t1_;
					SetWndwDsInOSDAnlg(_pth);
//					SetWndwDsInOSDAnlgRdo(_pth);
//					SetWndwDsInOSDAnlgChk(_pth);
					SetWndwDsInOSDSpec(_pth);
					
					mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;

					RunWndwDsInOSDArw(_pth,0,0);
					SetWndwDsInOSDNum(_pth);
				}

//				SetWndwDsInOSDType(_pth);
				SetWndwDsInOSDTypeRdo(_pth);

//				_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x86);
//				if(wndw_ds_in_type == IN_TYPE_DGTL)	SetBit(_t1_,BIT5);
//				else	ClearBit(_t1_,BIT5);
//				WriteAsicByte(cmn_dvc,DVC_PG1,0x86,_t1_);
				if(wndw_ds_in_type == IN_TYPE_DGTL)
					SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT5|BIT4,BIT4);				//... VBI_ENA=0,VBI_CODE_EN=1
				else
					SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT5|BIT4,BIT5);				//... VBI_ENA=1,VBI_CODE_EN=0
			}
		}
		else if(wndw_ds_in_type == IN_TYPE_ANLG){
			if((_osd_pos_x_ == WNDW_DS_IN_RDO_DT_X)&&((_osd_pos_y_ == WNDW_DS_IN_RDO_DT_Y)||(_osd_pos_y_ == (WNDW_DS_IN_RDO_DT_Y+1)))){
				if(_osd_pos_y_ == WNDW_DS_IN_RDO_DT_Y){
					b_ds_in_auto = ON;

					b_ds_in_run = ON;
					SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT7|BIT3,BIT7|BIT3);			//... AUTO_VBI_DET=1,VBI_RIC_ON=1
				}
				else{
					b_ds_in_auto = OFF;
					SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT7,0x00);						//... AUTO_VBI_DET=0
				}
				SetWndwDsInOSDAnlgRdo(_pth);
				SetWndwDsInOSDAnlgChk(_pth);
				SetWndwDsInOSDSpec(_pth);
				
				mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;

				RunWndwDsInOSDArw(_pth,0,0);
				SetWndwDsInOSDNum(_pth);
			}
			else if(b_ds_in_auto == OFF){
				if((_osd_pos_x_ == WNDW_DS_IN_CHK_RN_X)&&(_osd_pos_y_ == WNDW_DS_IN_CHK_RN_Y)){
					b_ds_in_run ^= 1;

					if(b_ds_in_run == ON)
						SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT3,BIT3);				//... VBI_RIC_ON=1
					else
						SetAsicFlgType(MASTER,DVC_PG1,0x86,BIT3,0x00);				//... VBI_RIC_ON=0
	
					SetWndwDsInOSDAnlgChk(_pth);
				}
				else if((_osd_pos_y_ >= WNDW_DS_IN_HS_Y)&&(_osd_pos_y_ <= WNDW_DS_IN_VS_Y)){
//					if((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)){
					if(b_ds_in_auto == OFF){
						if(_osd_pos_x_ == WNDW_DS_IN_DEC_X)	RunWndwDsInDec(_pth,_osd_pos_y_);
						else if(_osd_pos_x_ == WNDW_DS_IN_INC_X)	RunWndwDsInInc(_pth,_osd_pos_y_);
					}
				}
			}
//			else if(b_ds_in_run == ON){
//				if((_osd_pos_x_ == WNDW_DS_IN_CHK_RN_X)&&(_osd_pos_y_ == WNDW_DS_IN_CHK_AT_Y)){
//					_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x86);
//					b_ds_in_auto ^= 1;
//					if(b_ds_in_auto == ON)	SetBit(_t1_,BIT7);
//					else	ClearBit(_t1_,BIT7);
//					
////					SetWndwDsInOSDAnlg(_pth);
//					SetWndwDsInOSDAnlgRdo(_pth);
//					SetWndwDsInOSDAnlgChk(_pth);
//					SetWndwDsInOSDSpec(_pth);
//
//					mse_flg_on_osd = MSE_ON_OSD_HS|MSE_ON_OSD_PX|MSE_ON_OSD_VS|MSE_ON_OSD_LN;
//
//					RunWndwDsInOSDArw(_pth,0,0);
//					SetWndwDsInOSDNum(_pth);
//
//					WriteAsicByte(cmn_dvc,DVC_PG1,0x86,_t1_);
//				}
//			}
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

//		if(wndw_ds_in_type == IN_TYPE_ANLG){
		if((wndw_ds_in_type == IN_TYPE_ANLG)&&(b_ds_in_auto == OFF)){
			if((_osd_pos_y_ >= WNDW_DS_IN_HS_Y)&&(_osd_pos_y_ <= WNDW_DS_IN_VS_Y)){
//				if((b_ds_in_run == OFF)||(b_ds_in_auto == OFF)){
					if(_osd_pos_x_ == WNDW_DS_IN_DEC_X)	RunWndwDsInDec(_pth,_osd_pos_y_);
					else if(_osd_pos_x_ == WNDW_DS_IN_INC_X)	RunWndwDsInInc(_pth,_osd_pos_y_);
//				}
			}
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

