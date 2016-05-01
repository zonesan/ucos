#include	"hdr.h"
#include "cmn.h"
#include "wndw_pop.h"
#include "wndw_ds_scrn.h"
#include "wndw_mtn_md.h"
#include "osd.h"
#include "osd_deco.h"
#include "mse.h"
#include "prtcl.h"
#include "init_dvc.h"
#include "ch.h"
#include "wndw_rc_fmt.h"


//==================================================================================
#if defined(__4CH__)
U8 wndw_ds_sc_fmt = SC_FMT_4CH;
#elif	defined(__16CH__)
U8 wndw_ds_sc_fmt = SC_FMT_16CH;
#endif

//U8 idata	wndw_ds_sc_loss = SC_LOSS_BLAK;


//==================================================================================
//						Display wndwdow function description
//==================================================================================
void	SetWndwDsScOSDFmtChk(U8 _pth)
{
	U8 _t1_, _t2_;//, _t3_;

#if defined(__4CH__)
	if(wndw_ds_sc_fmt == SC_FMT_4CH)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH_PB,_t1_,str_wndw_ds_wth_4pb);

	if(b_ds_wth_4pb)	_t2_ = OSD_CHK_BOX_CHK;
	else	_t2_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH_PB,_t1_,_t2_);

	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH_PB,OSD_MIX,OSD_CHK_BOX_BLK);

	if(wndw_ds_sc_fmt == SC_FMT_8CH)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH_RC,_t1_,str_wndw_ds_wth_rc);

	if(b_ds_wth_rc)	_t2_ = OSD_CHK_BOX_CHK;
	else	_t2_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH_RC,_t1_,_t2_);
#elif	defined(__16CH__)
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH_PB,OSD_MIX,OSD_CHK_BOX_BLK);

	if(wndw_ds_sc_fmt == SC_FMT_16CH)	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH_PB,_t1_,str_wndw_ds_wth_16pb);

	if(b_ds_wth_16pb)	_t2_ = OSD_CHK_BOX_CHK;
	else	_t2_ = OSD_CHK_BOX_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH_PB,_t1_,_t2_);

	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_CHK_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH_RC,OSD_MIX,OSD_CHK_BOX_BLK);
#endif
}
//==================================================================================
void	SetWndwDsScOSDFmtRdo(U8 _pth)
{
	U8 _t1_;

	if(wndw_ds_sc_fmt == SC_FMT_1CH)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_1CH,0x00,_t1_);

	if(wndw_ds_sc_fmt == SC_FMT_4CH)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH,0x00,_t1_);

#if defined(__4CH__)
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH,OSD_MIX,OSD_RDO_BLK);

	if(wndw_ds_sc_fmt == SC_FMT_8CH)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH,0x00,_t1_);
#elif	defined(__16CH__)
	if(wndw_ds_sc_fmt == SC_FMT_16CH)	_t1_ = OSD_RDO_CHK;
	else	_t1_ = OSD_RDO_BLK;
	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH,0x00,_t1_);

	WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_FM_X,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH,OSD_MIX,OSD_RDO_BLK);
#endif

	SetWndwDsScOSDFmtChk(_pth);
}
//==================================================================================
void	SetWndwDsScOSDFmt(U8 _pth)
{
//	U8 _t1_;

	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_1CH,0x00,str_wndw_ds_full);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH,0x00,str_wndw_ds_quad);

#if defined(__4CH__)
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH,OSD_MIX,str_wndw_ds_16ch);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH_PB,OSD_MIX,str_wndw_ds_wth_16pb);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH,0x00,str_wndw_ds_psdo_8ch);
#elif	defined(__16CH__)
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH_PB,OSD_MIX,str_wndw_ds_wth_4pb);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH,0x00,str_wndw_ds_16ch);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_FM_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH,OSD_MIX,str_wndw_ds_psdo_8ch);
	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_CHK_X+2,WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH_RC,OSD_MIX,str_wndw_ds_wth_rc);
#endif

	SetWndwDsScOSDFmtRdo(_pth);
}
//==================================================================================
//void	SetWndwDsScOSDRdoLs(U8 _pth)
//{
//	U8 _t1_, _t2_, _t3_;
//
//	if(wndw_ds_sc_fmt == SC_FMT_8CH)	_t1_ = OSD_MIX;
//	else	_t1_ = 0x00;
//
//	for(_t3_=0;_t3_<4;_t3_++){
//		if(_t3_ == wndw_ds_sc_loss)	_t2_ = OSD_RDO_CHK;
//		else	_t2_ = OSD_RDO_BLK;
//
//		WriteOSDFnt(_pth,MASTER,WNDW_DS_SC_RDO_LS_X,WNDW_DS_SC_RDO_LS_Y+_t3_,_t1_,_t2_);
//	}
//}
////==================================================================================
//void	SetWndwDsScOSDLoss(U8 _pth)
//{
//	U8 _t1_;
//
//	if(wndw_ds_sc_fmt == SC_FMT_8CH)	_t1_ = OSD_MIX;
//	else	_t1_ = 0x00;
//	
//	SetOSDGrp(_pth,WNDW_DS_SC_RDO_LS_X-1,WNDW_DS_SC_RDO_LS_Y,14,4,_t1_,str_wndw_grp_loss);
//
//	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_LS_X+2,WNDW_DS_SC_RDO_LS_Y,_t1_,str_wndw_ds_byps);
//	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_LS_X+2,WNDW_DS_SC_RDO_LS_Y+1,_t1_,str_wndw_ds_frz);
//	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_LS_X+2,WNDW_DS_SC_RDO_LS_Y+2,_t1_,str_wndw_ds_blak);
//	WriteOSDStr(_pth,MASTER,WNDW_DS_SC_RDO_LS_X+2,WNDW_DS_SC_RDO_LS_Y+3,_t1_,str_wndw_ds_blik);
//
//	SetWndwDsScOSDRdoLs(_pth);
//}
//==================================================================================
//==================================================================================
//void	InitWndwDsScVarLs(void)
//{
//	U8 _rdat_;
//	
////	_rdat_ = ReadAsicByte(MASTER,DVC_PG1,0x05);
////	_rdat_ = (_rdat_ & 0xc0)>>6;
//	_rdat_ = GetAsicVar8Type(MASTER,DVC_PG1,0x05,0x62);
//	wndw_ds_sc_loss = _rdat_;
//}
//==================================================================================
void	ChngWndwDsScFmtLv(void)
{
	U8 _dvc_, _ch_;
	U8 _t1_;//, _t2_=0;
//	U16 _dly_;

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x10);			//... FUNC_MODE=1
	}

//	for(_dly_=0;_dly_<30000;_dly_++){											//... check finishing strobe for max 30000 times
//		_t1_=0;
//		_t1_ |= GetAsicFlgType(MASTER,DVC_PG1,0x04,0x0f);
//#if	defined(__16CH__)
//		_t1_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x04,0x0f);
//		_t1_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x04,0x0f);
//		_t1_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x04,0x0f);
//#endif
//		if(_t1_==0)	break;
//	}

//	WaitChStrbDoneX(0x0f);				//... check for four channels of each devices
	WaitChStrbDoneX(0xffff);			//... check for four channels of each devices

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,0x00);			//... POP_UP_EN=0
	}

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		if(wndw_ds_sc_fmt == SC_FMT_1CH){
			if(_t1_ == wndw_pop_crnt_ch)	SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);			//... POP_UP_EN=1
			SetChFl(PTH_X,_dvc_,_ch_);
		}
		else if(wndw_ds_sc_fmt == SC_FMT_4CH){
			if((_t1_&0x0c) == (wndw_pop_crnt_ch&0x0c))	SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);	//... POP_UP_EN=1
			if(b_ds_wth_4pb)	SetChNine(PTH_X,_dvc_,_ch_,_ch_);
			else	SetChQd(PTH_X,_dvc_,_ch_,_ch_);
		}
//#if	defined(__4CH__)
//		else if(wndw_ds_sc_fmt == SC_FMT_4CH_PB){
//			SetChNine(PTH_X,_dvc_,_ch_,_ch_);
//		}
//#elif	defined(__16CH__)
#if	defined(__16CH__)
		else if(wndw_ds_sc_fmt == SC_FMT_16CH){
			if(b_ds_wth_16pb){
				if(_t1_ < 14)	SetCh36(PTH_X,_dvc_,_ch_,_t1_);
				else		SetCh36(PTH_X,_dvc_,_ch_,_t1_+2);
			}
			else	SetChPIP(PTH_X,_dvc_,_ch_,_t1_);
		}
#endif
	}

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),0x30,0x00);			//... FUNC_MODE=0
	}
}
//==================================================================================
#if defined(__4CH__)

void	InitWndwDsScFmtPsd8Ch(U8 _tgl)
{
	U8 _t1_;

	if(_tgl){
		WriteAsicByte(MASTER,DVC_PG0,0x43,0x4f);			//... GNTIME=3, OSTIME=3
		WriteAsicByte(MASTER,DVC_PG0,0x45,0x10);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x4d,0x00);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x51,0x88);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x53,0x40);			//... 
//		SetAsicFlgType(MASTER,DVC_PG0,0xf8,BIT5,BIT5);							//... enable AUTO_BGND

		if(b_ch_src == CH_SRC_LV)
			SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x00);			//... STRB_FLD=0
		else
			SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2

//		//... y path ...................................
//		if(b_rc_fm_type == FM_TYPE_CIF){
//			for(_t1_=0;_t1_<4;_t1_++)
//				SetAsicFlgType(MASTER,DVC_PG1,0x60+_t1_*3,0x30,0x10);			//... FUNC_MODE=1
//
////			if(wndw_rc_fm_cif_type != CIF_TYPE_FL){
////				wndw_rc_fm_cif_type = CIF_TYPE_FL;
////
////				SetAsicFlgType(MASTER,DVC_PG1,0x51,BIT6,BIT6);
//////				for(_t1_=0;_t1_<4;_t1_++){
//////					SetChQdPos(PTH_Y,MASTER,_t1_,_t1_);
//////					SetSclRto(PTH_Y,MASTER,_t1_,0x12,0x11);
//////				}
////			}
//		}
//		else{
//			if(wndw_rc_fm_mux_type != MUX_TYPE_FL){
//				wndw_rc_fm_mux_type = MUX_TYPE_FL;
//				SetAsicFlgType(MASTER,DVC_PG1,0x56,0xf0,0x00);		//... MUX_MODE=0,TRIG_MODE=0,MUX_FLD=0
//			}
//		}
		wndw_rc_fm_type = CIF_TYPE_FL;						//... just limitation for promotion
		SetWndwRcFmType();										//... just limitation for promotion
	}
	else{
		WriteAsicByte(MASTER,DVC_PG0,0x43,0x45);			//... GNTIME=1, OSTIME=1
		WriteAsicByte(MASTER,DVC_PG0,0x45,0xd0);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x4d,0x0f);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x51,0x80);			//... 
		WriteAsicByte(MASTER,DVC_PG0,0x53,0x00);			//... 
//		SetAsicFlgType(MASTER,DVC_PG0,0xf8,BIT5,0x00);							//... disable AUTO_BGND
		for(_t1_=0;_t1_<4;_t1_++)
			SetAsicFlgType(MASTER,DVC_PG0,0x0d+0x10*_t1_,BIT3,0x00);		//... ANA_SW=0

		SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2

		//... y path ...................................
//		if(b_rc_fm_type == FM_TYPE_CIF){
		if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
			for(_t1_=0;_t1_<4;_t1_++){
				SetAsicFlgType(MASTER,DVC_PG1,0x60+_t1_*3,0x30,0x00);			//... FUNC_MODE=0
//				SetAsicFlgType(MASTER,DVC_PG1,0x61+_t1_*3,BIT2,0x00);			//... BLANK_EN=0
			}
		}
		else{
			SetAsicFlgType(MASTER,DVC_PG1,0x56,BIT6,BIT6);				//... TRIG_MODE=1
		}
	}
}
//==================================================================================
void	SetWndwDsScFmtPsd8Ch(void)
{
	U8 _t1_;

	for(_t1_=0;_t1_<4;_t1_++){
		SetChNinePos(PTH_X,MASTER,_t1_,_t1_);
		SetChNinePos(PTH_PB,MASTER,_t1_,_t1_+5);

		if(b_ch_src == CH_SRC_PB){
//			if(b_rc_fm_type == FM_TYPE_CIF){
			if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
				SetSclRto(PTH_PB,MASTER,_t1_,0x23,0x23);
//				if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
				if(wndw_rc_fm_type == CIF_TYPE_FR){
					SetAsicFlgType(MASTER,DVC_PG1,0x12+(_t1_<<3),BIT5,0x00);					//... FIELD_OP=1
					SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,0x00);					//... FIELD_OP=1
				}
				else{
					SetAsicFlgType(MASTER,DVC_PG1,0x12+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
					SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
				}
			}
			else{
				SetSclRto(PTH_PB,MASTER,_t1_,0x13,0x23);
				SetAsicFlgType(MASTER,DVC_PG1,0x12+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
				SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
			}

			//... DM_PATH_SEL이 실제로 바뀌어 있음( 0: B, 1: A)
			//... live path
			SetAsicFlgType(MASTER,DVC_PG1,0x10+(_t1_<<3),0x30|BIT3|BIT2,0x10|BIT3|BIT2);	//... FUNC_MODE=1,DM_PATH_SEL=1,PB_PATH_EN=1
//			SetAsicFlgType(MASTER,DVC_PG2,0x60+(_t1_<<3),0x80,0x00);	//... MDBOX_EN=0
			//... pb path
			SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t1_<<3),0xb0|BIT3|BIT2,0x90|BIT2);			//... CHENA=1,FUNC_MODE=1,DM_PATH_SEL=0,PB_PATH_EN=1
		}
		else{
			SetSclRto(PTH_X,MASTER,_t1_,0x13,0x23);

			//... live path
			SetAsicFlgType(MASTER,DVC_PG1,0x10+(_t1_<<3),0x30|BIT3|BIT2,0x10);			//... FUNC_MODE=1,DM_PATH_SEL=0,PB_PATH_EN=0
			SetAsicFlgType(MASTER,DVC_PG1,0x12+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
//			SetAsicFlgType(MASTER,DVC_PG2,0x60+(_t1_<<3),0x80,0x00);	//... MDBOX_EN=0
			//... pb path
			SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t1_<<3),0xb0|BIT3|BIT2,0x90);			//... CHENA=1,FUNC_MODE=1,DM_PATH_SEL=0,PB_PATH_EN=0
			SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,BIT5);					//... FIELD_OP=1
		}
	}
}
//==================================================================================
void	SetWndwDsScFmtPsd8ChRc(void)
{
	U8 _t1_;
	
	if(b_ds_wth_rc){
		//... y path ...................................
//		SetAsicFlgType(MASTER,DVC_PG1,0x61+_t1_*3,BIT2,0x00);			//... BLANK_EN=0

//		if(b_rc_fm_type == FM_TYPE_CIF){
		if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
			for(_t1_=0;_t1_<4;_t1_++)
				SetAsicFlgType(MASTER,DVC_PG1,0x60+_t1_*3,0x30,0x10);			//... FUNC_MODE=1

//			if(wndw_rc_fm_cif_type != CIF_TYPE_FL){
//				wndw_rc_fm_cif_type = CIF_TYPE_FL;
//
//				SetAsicFlgType(MASTER,DVC_PG1,0x51,BIT6,BIT6);
////				for(_t1_=0;_t1_<4;_t1_++){
////					SetChQdPos(PTH_Y,MASTER,_t1_,_t1_);
////					SetSclRto(PTH_Y,MASTER,_t1_,0x12,0x11);
////				}
//			}
		}
		else{
//			SetAsicFlgType(MASTER,DVC_PG1,0x56,BIT6,BIT6);						//... internal trigger mode

//			if(wndw_rc_fm_mux_type != MUX_TYPE_FL){
//				wndw_rc_fm_mux_type = MUX_TYPE_FL;
			if(wndw_rc_fm_type != MUX_TYPE_FL){
				wndw_rc_fm_type = MUX_TYPE_FL;
			}
			SetAsicFlgType(MASTER,DVC_PG1,0x56,0xf0,0x00);		//... MUX_MODE=0,TRIG_MODE=0,MUX_FLD=0
		}
	}
	else{
		//... y path ...................................
//		SetAsicFlgType(MASTER,DVC_PG1,0x61+_t1_*3,BIT2,BIT2);			//... BLANK_EN=1

//		if(b_rc_fm_type == FM_TYPE_CIF){
		if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
		}
		else{
			SetAsicFlgType(MASTER,DVC_PG1,0x56,BIT6,BIT6);						//... internal trigger mode
		}
	}
}

#endif
//==================================================================================
void	SetWndwDsScFmtLvPb(void)
{
	U8 _t1_;
	U8 _dvc_, _ch_;
	
	//... pb path
#if defined(__4CH__)
	if(b_ds_wth_4pb){				//... enable pb 4ch: triplex(lv 4ch + pb 4ch)
		if(wndw_rc_fm_type == CIF_TYPE_FR)
			SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x03);			//... STRB_FLD=3		(because AUTO_ENHANCE=1)
		else
			SetAsicFlgType(MASTER,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2
		
		for(_t1_=0;_t1_<4;_t1_++){
			SetChNinePos(PTH_PB,MASTER,_t1_,_t1_+5);
//			if(b_rc_fm_type == FM_TYPE_CIF){
			if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
				SetSclRto(PTH_PB,MASTER,_t1_,0x23,0x23);

//				if(wndw_rc_fm_cif_type == CIF_TYPE_FR)
				if(wndw_rc_fm_type == CIF_TYPE_FR)
					SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,0x00);			//... FIELD_OP=0
				else	SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,BIT5);		//... FIELD_OP=1
			}
			else{															//... PB (MUX)
				SetSclRto(PTH_PB,MASTER,_t1_,0x13,0x23);
				SetAsicFlgType(MASTER,DVC_PG1,0x15+(_t1_<<3),BIT5,BIT5);				//... FIELD_OP=1
			}
			WriteAsicByte(MASTER,DVC_PG1,0x14+(_t1_<<3),0x02);						//... ENHANCE=0
			WriteAsicByte(MASTER,DVC_PG1,0x16+(_t1_<<3),0x90|_t1_);
			WriteAsicByte(MASTER,DVC_PG1,0x13+(_t1_<<3),0x90|BIT2|_t1_);		//... PB_PATH_EN=1
		}
	}
#elif	defined(__16CH__)
	if(b_ds_wth_16pb){				//... enable pb 4ch: triplex(lv 4ch + pb 4ch)
//		if(wndw_rc_fm_type == CIF_TYPE_FR)
//			SetAsicFlgType(_dvc_,DVC_PG1,0x01,0x03,0x03);			//... STRB_FLD=3		(because AUTO_ENHANCE=1)
//		else
			SetAsicFlgType(_dvc_,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2
		
		for(_t1_=0;_t1_<16;_t1_++){
			SetCrntDvcCh(_t1_,&_dvc_,&_ch_);

			if(_t1_ < 2)	SetCh36Pos(PTH_PB,_dvc_,_ch_,_t1_+18);
			else		SetCh36Pos(PTH_PB,_dvc_,_ch_,_t1_+20);
//			if(b_rc_fm_type == FM_TYPE_CIF){
			if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
//				SetSclRto(PTH_PB,_dvc_,_ch_,0x13,0x13);
//
////				if(wndw_rc_fm_cif_type == CIF_TYPE_FR)
//				if(wndw_rc_fm_type == CIF_TYPE_FR)
//					SetAsicFlgType(_dvc_,DVC_PG1,0x15+(_ch_<<3),BIT5,0x00);			//... FIELD_OP=0
//				else	SetAsicFlgType(_dvc_,DVC_PG1,0x15+(_ch_<<3),BIT5,BIT5);		//... FIELD_OP=1

				if(wndw_rc_fm_type == CIF_TYPE_FR)	SetSclRto(PTH_PB,_dvc_,_ch_,0x13,0x23);
				else	SetSclRto(PTH_PB,_dvc_,_ch_,0x13,0x13);
				SetAsicFlgType(_dvc_,DVC_PG1,0x15+(_ch_<<3),BIT5,BIT5);		//... FIELD_OP=1
			}
			else{															//... PB (MUX)
				SetSclRto(PTH_PB,_dvc_,_ch_,0x16,0x13);
				SetAsicFlgType(_dvc_,DVC_PG1,0x15+(_ch_<<3),BIT5,BIT5);				//... FIELD_OP=1
			}
			WriteAsicByte(_dvc_,DVC_PG1,0x14+(_ch_<<3),0x02);						//... ENHANCE=0
			WriteAsicByte(_dvc_,DVC_PG1,0x16+(_ch_<<3),0x90|_t1_);
			WriteAsicByte(_dvc_,DVC_PG1,0x13+(_ch_<<3),0x90|BIT2|_ch_);		//... PB_PATH_EN=1
		}
	}
#endif
	else{								//... disable pb 4ch: only live 4ch
		for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
			SetCrntDvcCh(_t1_,&_dvc_,&_ch_);

			if(!(_t1_&0x03))
				SetAsicFlgType(_dvc_,DVC_PG1,0x01,0x03,0x02);			//... STRB_FLD=2
	
			//... strobe mode인 dummy ch을 disable 과 enable을 반복할때,
			//... 1. live mode로 setting후 disable, 2. strobe 완료후 disable, 3. 이도저도 싫으면 enable전에 MODE_RST=1로 setting.
			SetAsicFlgType(_dvc_,DVC_PG1,0x13+(_ch_<<3),BIT7|0x30,0x00);			//... CHENA=0,FUNC_MODE=0

////			SetAsicFlgType(_dvc_,DVC_PG2,REG_2DBOX_CTL+(_t1_<<3),_2DBOX_EN_X,_2DBOX_EN_X);
//			SetAsicFlgType(_dvc_,DVC_PG2,0x60+(_t1_<<3),0x80,0x80);	//... MDBOX_EN=1
		}
	}
}
//==================================================================================
void	SetWndwDsScFmtLv(void)
{
	U8 _dvc_, _ch_;
	U8 _t1_;
	
	//... live path
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT3|BIT2,0x00);			//... DM_PATH_SEL=0,PB_PATH_EN=0
		SetAsicFlgType(_dvc_,DVC_PG1,0x11+(_ch_<<3),BIT6|BIT3,0x00);			//... FRZ_CH=0,ENHANCE=0
		SetAsicFlgType(_dvc_,DVC_PG1,0x12+(_ch_<<3),BIT5,0x00);					//... FIELD_OP=0
	}

	ChngWndwDsScFmtLv();
	SetWndwDsScFmtLvPb();
}
//==================================================================================
void	SetWndwDsScFmtFl(void)
{
	U8 _dvc_, _ch_;
	U8 _t1_;
	
	//... if needed, do zoom control for playback

	//... live path
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT3|BIT2,0x00);			//... DM_PATH_SEL=0,PB_PATH_EN=0
		SetAsicFlgType(_dvc_,DVC_PG1,0x11+(_ch_<<3),BIT6|BIT3,0x00);			//... FRZ_CH=0,ENHANCE=0
		SetAsicFlgType(_dvc_,DVC_PG1,0x12+(_ch_<<3),BIT5,0x00);					//... FIELD_OP=0
		//... strobe mode인 dummy ch을 disable 과 enable을 반복할때,
		//... 1. live mode로 setting후 disable, 2. strobe 완료후 disable, 3. 이도저도 싫으면 enable전에 MODE_RST=1로 setting.
		SetAsicFlgType(_dvc_,DVC_PG1,0x13+(_ch_<<3),BIT7|0x30,0x00);			//... CHENA=0,FUNC_MODE=0
	}

	ChngWndwDsScFmtLv();
}
//==================================================================================
//==================================================================================
void	CreateWndwDsSc(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

	WriteOSDClr(_pth,MASTER,WNDW_DS_SC_POS_X,WNDW_DS_SC_POS_Y,WNDW_DS_SC_L,WNDW_DS_SC_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_DS_SC_POS_X,WNDW_DS_SC_POS_Y,WNDW_DS_SC_L,WNDW_DS_SC_H,str_wndw_ttl_ds_scrn);

//	InitWndwDsScVarLs();

	//SetWndwDsScOSDFmt(_pth);
//	SetWndwDsScOSDLoss(_pth);
}
//==================================================================================
void	RunWndwDsSc(U8 _pth)//U8 _index_x, U8 _index_y)
{
	U8 _t1_;
	U8 _dvc_, _ch_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

	
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_DS_SC_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_SC_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_DS_SC_CLOS_X)&&(_osd_pos_y_ == WNDW_DS_SC_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
			WriteOSDClr(_pth,MASTER,WNDW_DS_SC_POS_X,WNDW_DS_SC_POS_Y,WNDW_DS_SC_L,WNDW_DS_SC_H);
			WriteOSDClrX(MASTER,WNDW_DS_SC_POS_X-1,WNDW_DS_SC_POS_Y-1,WNDW_DS_SC_L+2,WNDW_DS_SC_H+2);
////			WriteOSDClr(PTH_Y,MASTER,WNDW_DS_SC_CNT_X,WNDW_DS_SC_CNT_Y,10,1);
//			DisableBox(_pth,BOX_WNDW_FRM-2,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		else{
#if defined(__4CH__)
			if((_osd_pos_x_ == WNDW_DS_SC_RDO_FM_X)&&
				((_osd_pos_y_ == WNDW_DS_SC_RDO_FM_Y)||(_osd_pos_y_ == (WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH))||
				(_osd_pos_y_ == (WNDW_DS_SC_RDO_FM_Y+SC_FMT_8CH)))){

				_t1_ = _osd_pos_y_ - WNDW_DS_SC_RDO_FM_Y;
//				if(_t1_ != wndw_ds_sc_fmt){
					if((_t1_ < SC_FMT_8CH)&&(wndw_ds_sc_fmt < SC_FMT_8CH)){
						//... if select one of full or quad, pass OSD process... (-> redrawndwg protection)
						wndw_ds_sc_fmt = _t1_;				//... don't move out of if() sentence
					}
					else{
						wndw_ds_sc_fmt = _t1_;
					}
#elif	defined(__16CH__)
			if((_osd_pos_x_ == WNDW_DS_SC_RDO_FM_X)&&
				((_osd_pos_y_ == WNDW_DS_SC_RDO_FM_Y)||(_osd_pos_y_ == (WNDW_DS_SC_RDO_FM_Y+SC_FMT_4CH))||
				(_osd_pos_y_ == (WNDW_DS_SC_RDO_FM_Y+SC_FMT_16CH)))){

				_t1_ = _osd_pos_y_ - WNDW_DS_SC_RDO_FM_Y;
//				if(_t1_ != wndw_ds_sc_fmt){
					wndw_ds_sc_fmt = _t1_;
#endif

#if defined(__4CH__)
					if(wndw_ds_sc_fmt == SC_FMT_8CH){
						InitWndwDsScFmtPsd8Ch(ON);
						SetWndwDsScFmtPsd8Ch();
						SetWndwDsScFmtPsd8ChRc();
					}
					else{
						InitWndwDsScFmtPsd8Ch(OFF);
#elif	defined(__16CH__)
					if(wndw_ds_sc_fmt == SC_FMT_16CH){
						SetWndwDsScFmtLv();
					}
					else{
#endif
						if(wndw_ds_sc_fmt == SC_FMT_4CH){
							SetWndwDsScFmtLv();
						}
						else if(wndw_ds_sc_fmt == SC_FMT_1CH){
							SetWndwDsScFmtFl();
						}
					}
					
					SetWndwDsScOSDFmtRdo(_pth);

//				}
			}
			else{
#if defined(__4CH__)
				if(wndw_ds_sc_fmt == SC_FMT_4CH){
					if((_osd_pos_x_ == WNDW_DS_SC_CHK_X)&&(_osd_pos_y_ == WNDW_DS_SC_CHK_4PB_Y)){
						b_ds_wth_4pb ^= 1;
						
						SetWndwDsScFmtLv();
						SetWndwDsScOSDFmtChk(_pth);
						
						if(b_ds_wth_4pb){
							for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
								SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
								Disable2DBoxDsply(_dvc_,_ch_);
							}
						}
					}
				}
				else if(wndw_ds_sc_fmt == SC_FMT_8CH){
					if((_osd_pos_x_ == WNDW_DS_SC_CHK_X)&&(_osd_pos_y_ == WNDW_DS_SC_CHK_RC_Y)){
						b_ds_wth_rc ^= 1;

//						//... add (date:050401 Rev1.2)
//						if(b_rc_fm_type == FM_TYPE_MUX){
////... update:050802 Rev2.1_ pseudo 8ch field by field mux switching test
//							if(b_ds_wth_rc == 1){
//////								SetAsicFlgType(MASTER,DVC_PG1,REG_MUX_CTL_Y,TRIG_MODE,0x00);						//... external trigger mode
////								SetAsicFlgType(MASTER,DVC_PG1,REG_MUX_CTL_Y,TRIG_MODE,TRIG_MODE);			//... internal trigger mode
////								SetAsicFlgType(MASTER,DVC_PG1,0x50,0xff,0x80);						//... set Strobe acknowledge of display path on MPPDEC_X[3:0]
//								SetAsicFlgType(MASTER,DVC_PG1,0x56,BIT6,BIT6);						//... internal trigger mode
//							}
//							else{
////								SetAsicFlgType(MASTER,DVC_PG1,REG_MUX_CTL_Y,TRIG_MODE,0x00);					//... external trigger mode
////								SetAsicFlgType(MASTER,DVC_PG1,0x50,0xff,0x00);						//... set input mode on MPPDEC_X[3:0]
//								SetAsicFlgType(MASTER,DVC_PG1,0x56,BIT6,0x00);						//... internal trigger mode
//							}
//						}
						SetWndwDsScFmtPsd8ChRc();
						SetWndwDsScOSDFmtChk(_pth);

						if(b_ds_wth_rc){
							for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
								SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
								Disable2DBoxDsply(_dvc_,_ch_);
							}
						}
					}
				}
#elif	defined(__16CH__)
				if(wndw_ds_sc_fmt == SC_FMT_16CH){
					if((_osd_pos_x_ == WNDW_DS_SC_CHK_X)&&(_osd_pos_y_ == WNDW_DS_SC_CHK_16PB_Y)){
						b_ds_wth_16pb ^= 1;
						
						SetWndwDsScFmtLv();
						SetWndwDsScOSDFmtChk(_pth);

						if(b_ds_wth_16pb){
							for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
								SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
								Disable2DBoxDsply(_dvc_,_ch_);
							}
						}
					}
				}
#endif
//				else{//if(wndw_ds_sc_fmt != SC_FMT_8CH){
//					if((_osd_pos_x_ == WNDW_DS_SC_RDO_LS_X)&&
//						((_osd_pos_y_ >= WNDW_DS_SC_RDO_LS_Y)&&(_osd_pos_y_ <= (WNDW_DS_SC_RDO_LS_Y+3)))){
//						
//						_t1_ = _osd_pos_y_ - WNDW_DS_SC_RDO_LS_Y;
//						if(_t1_ != wndw_ds_sc_loss){
//							wndw_ds_sc_loss = _t1_;//_osd_pos_y_ - WNDW_DS_SC_RDO_LS_Y;
//
//							SetWndwDsScOSDLoss(_pth);
//
////							_t1_ = ReadAsicByte(MASTER,DVC_PG1,REG_FRZ_FLD_X);
////							_t1_ = (_t1_ & 0x3f)|(wndw_ds_sc_loss<<6);
////							WriteAsicByte(global_dvc,DVC_PG1,REG_FRZ_FLD_X,_t1_);
//							SetAsicFlgType(cmn_dvc,DVC_PG1,0x05,0xc0,wndw_ds_sc_loss<<6);
//						}
//					}
//				}
			}
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1)	b_mse_pshng_clk_lb = 0;
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================

