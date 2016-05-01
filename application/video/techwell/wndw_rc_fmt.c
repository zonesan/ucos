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
int	b_rc_fm_que;
int	b_rc_fm_loss;
int	b_rc_fm_ls_sk;

U8 wndw_rc_fm_type = CIF_TYPE_FR;
/*	//...	update:050705 Rev2.0
#if	defined(__16CH__)
int	b_rc_fm_que_cnt_rst = wndw_flg_rc_fm^4;
#endif
//	*/


//==================================================================================
//						Display wndwdow function description
//==================================================================================
//void	SetWndwRcFmOSDType(U8 _pth)
//{
//	U8 _t1_, _t2_;
//
//	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X0+2,WNDW_RC_FM_RDO_CIF_Y,0x00,str_wndw_cmn_cif);
//	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X0+2,WNDW_RC_FM_RDO_MUX_Y,0x00,str_wndw_cmn_mux);
//
//	if(b_rc_fm_type == FM_TYPE_CIF){
//		_t1_ = OSD_RDO_CHK;
//		_t2_ = OSD_RDO_BLK;
//	}
//	else{
//		_t1_ = OSD_RDO_BLK;
//		_t2_ = OSD_RDO_CHK;
//	}
//	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X0,WNDW_RC_FM_RDO_CIF_Y,0x00,_t1_);
//	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X0,WNDW_RC_FM_RDO_MUX_Y,0x00,_t2_);
//}
//==================================================================================
void	SetWndwRcFmOSDCifRdo(U8 _pth)
{
	U8 _t1_, _t2_;//, _t3_;

	if(wndw_rc_fm_type == CIF_TYPE_FR){
		_t1_ = OSD_RDO_CHK;
		_t2_ = OSD_RDO_BLK;
	}
	else if(wndw_rc_fm_type == CIF_TYPE_FL){
		_t1_ = OSD_RDO_BLK;
		_t2_ = OSD_RDO_CHK;
	}
	else{
		_t1_ = OSD_RDO_BLK;
		_t2_ = OSD_RDO_BLK;
	}
	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_CIF_FR_Y,0x00,_t1_);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_CIF_FL_Y,0x00,_t2_);
}
//==================================================================================
void	SetWndwRcFmOSDCif(U8 _pth)
{
//	U8 _t1_, _t2_;//, _t3_;

//	if((b_rc_fm_type == FM_TYPE_MUX)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t2_ = OSD_MIX;
//	else	_t2_ = 0x00;
	
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X+2,WNDW_RC_FM_RDO_CIF_FR_Y,0x00,str_wndw_rc_frm_cif);
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X+2,WNDW_RC_FM_RDO_CIF_FL_Y,0x00,str_wndw_rc_fld_cif);

//	for(_t3_=0;_t3_<2;_t3_++){
//		if(wndw_rc_fm_cif_type == _t3_)	_t1_ = OSD_RDO_CHK;
//		else	_t1_ = OSD_RDO_BLK;
//		WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X1,WNDW_RC_FM_RDO_CIF_FR_Y+_t3_,_t2_,_t1_);
//	}


//	if(wndw_rc_fm_type == CIF_TYPE_FR){
//		_t1_ = OSD_RDO_CHK;
//		_t2_ = OSD_RDO_BLK;
//	}
//	else if(wndw_rc_fm_type == CIF_TYPE_FL){
//		_t1_ = OSD_RDO_BLK;
//		_t2_ = OSD_RDO_CHK;
//	}
//	else{
//		_t1_ = OSD_RDO_BLK;
//		_t2_ = OSD_RDO_BLK;
//	}
//	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_CIF_FR_Y,0x00,_t1_);
//	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_CIF_FL_Y,0x00,_t2_);
	SetWndwRcFmOSDCifRdo(_pth);
}
//==================================================================================
void	SetWndwRcFmOSDMuxRdo(U8 _pth)
{
	U8 _t1_, _t2_, _t3_;

	for(_t3_=0;_t3_<3;_t3_++){
		if(wndw_rc_fm_type == (_t3_+2))	_t1_ = OSD_RDO_CHK;
		else	_t1_ = OSD_RDO_BLK;
#if	defined(__4CH__)
		if((wndw_ds_sc_fmt == SC_FMT_8CH)&&(_t3_ != 1))
			_t2_ = OSD_MIX;
		else	_t2_ = 0x00;
		WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_MUX_FR_Y+_t3_,_t2_,_t1_);
#elif	defined(__16CH__)
		_t2_ = 0;
		WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_MUX_FR_Y+_t3_,0x00,_t1_);
#endif
	}
}
//==================================================================================
void	SetWndwRcFmOSDMux(U8 _pth)
{
	U8 _t1_;//, _t2_, _t3_;

//	if((b_rc_fm_type == FM_TYPE_CIF)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
	if(wndw_ds_sc_fmt == SC_FMT_8CH)	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;
	
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X+2,WNDW_RC_FM_RDO_MUX_FR_Y,_t1_,str_wndw_rc_fl_d1);
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X+2,WNDW_RC_FM_RDO_MUX_FL_Y,0x00,str_wndw_rc_hf_d1);
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_RDO_X+2,WNDW_RC_FM_RDO_MUX_LV_Y,_t1_,str_wndw_rc_lv_sw);

//	for(_t3_=0;_t3_<3;_t3_++){
//		if(wndw_rc_fm_type == (_t3_+2))	_t1_ = OSD_RDO_CHK;
//		else	_t1_ = OSD_RDO_BLK;
//		if(wndw_rc_fm_type == MUX_TYPE_FL)	_t2_ = 0x00;
//		else	_t2_ = OSD_MIX;
//		WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_RDO_X,WNDW_RC_FM_RDO_MUX_FR_Y+_t3_,_t2_,_t1_);
//	}
	SetWndwRcFmOSDMuxRdo(_pth);
}
//==================================================================================
void	SetWndwRcFmOSDBtn(U8 _pth)
{
	U8 _t1_;

//#if	defined(__4CH__)
//	if((b_rc_fm_type == FM_TYPE_CIF)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
//	else	_t1_ = 0x00;
//#elif	defined(__16CH__)
//	_t1_ = 0x00;
//#endif
	
	SetOSDWndw(NULL,OSD_WNDW_TYPE_BTN,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_QUE_Y,9,1,str_null);
	SetOSDWndw(NULL,OSD_WNDW_TYPE_BTN,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_LOSS_Y,9,1,str_null);
#if	defined(__4CH__)
//	if((b_rc_fm_type == FM_TYPE_CIF)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
	if((wndw_rc_fm_type <= CIF_TYPE_FL)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;

	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_QUE_Y,_t1_,str_wndw_rc_que_stp);
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_LOSS_Y,_t1_,str_wndw_rc_no_vid);
#elif	defined(__16CH__)
//... inactive button
//	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_QUE_Y,OSD_MIX,str_wndw_rc_que_stp);
//	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_LOSS_Y,OSD_MIX,str_wndw_rc_no_vid);
	_t1_ = 0x00;
//... active button
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_QUE_Y,_t1_,str_wndw_rc_que_stp);
	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_BTN_X,WNDW_RC_FM_BTN_LOSS_Y,_t1_,str_wndw_rc_no_vid);
#endif
}
//==================================================================================
void	RunWndwRcFmOSDArw(U8 _pth, U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	U8 _t1_;

#if	defined(__4CH__)
//	if((b_rc_fm_type == FM_TYPE_CIF)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
	if((wndw_rc_fm_type <= CIF_TYPE_FL)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;
#elif	defined(__16CH__)
	_t1_ = 0x00;
#endif
	
	RunMseOnOSDH(_pth|_t1_,_osd_pos_x_,_osd_pos_y_,WNDW_RC_FM_DEC_X,WNDW_RC_FM_INC_X,WNDW_RC_FM_PRD_Y,MSE_ON_OSD_PRD);
}
//==================================================================================
void	SetWndwRcFmOSDNum(U8 _pth, U16 _data)
{
	U8 _t1_, _t2_;

#if	defined(__4CH__)
//	if((b_rc_fm_type == FM_TYPE_CIF)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t2_ = OSD_MIX;
	if((wndw_rc_fm_type <= CIF_TYPE_FL)||(wndw_ds_sc_fmt == SC_FMT_8CH))	_t2_ = OSD_MIX;
	else	_t2_ = 0x00;
#elif	defined(__16CH__)
	_t2_ = 0x00;
#endif
	
	_t1_ = (U8)(_data%100);
	_t1_ = ConvDec2Bcd(_t1_);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X+2,WNDW_RC_FM_PRD_Y,_t2_,(_t1_>>4)+0x10);
	WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X+3,WNDW_RC_FM_PRD_Y,_t2_,(_t1_&0x0f)+0x10);
	_t1_ = (U8)(_data/100);
	if(_t1_ == 0)	WriteOSDClr(_pth,MASTER,WNDW_RC_FM_NUM_X,WNDW_RC_FM_PRD_Y,2,1);
	else{
		if(_t1_ < 10){
			WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X,WNDW_RC_FM_PRD_Y,OSD_CLR,0x00);
			WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X+1,WNDW_RC_FM_PRD_Y,_t2_,(_t1_&0x0f)+0x10);
		}
		else{
			WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X,WNDW_RC_FM_PRD_Y,_t2_,0x11);
			WriteOSDFnt(_pth,MASTER,WNDW_RC_FM_NUM_X+1,WNDW_RC_FM_PRD_Y,_t2_,0x10);
		}
	}
}
//==================================================================================
//==================================================================================
U16	GetWndwRcFmPrd(void)
{
	U16 _rdat_=0;
	U8 _t1_, _t2_;
	
#if	defined(__4CH__)
	_t1_ = 0x59;
	_t2_ = 0x58;
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_MUX){
	if(wndw_rc_fm_type > CIF_TYPE_FL){
		_t1_ = 0x59;
		_t2_ = 0x58;
	}
	else{
		_t1_ = 0x71;
		_t2_ = 0x72;
	}
#endif

	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,_t1_);
	_rdat_ = (_rdat_&0xc0)<<2;
	_rdat_ |= ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,_t2_);

/*	//... update:050705 Rev2.0
	if(b_rc_fm_type == FM_TYPE_CIF){
		if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
			_rdat_ >>= 1;
		}
	}
	//	*/

	return	_rdat_;
}
//==================================================================================
void	SetWndwRcFmPrd(U16 _data)
{
	U8 _rdat_;
	U8 _t1_, _t2_, _t3_;
	
#if	defined(__4CH__)
	_t1_ = 0x59;
	_t2_ = 0x58;
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_MUX){
	if(wndw_rc_fm_type > CIF_TYPE_FL){
		_t1_ = 0x59;
		_t2_ = 0x58;
	}
	else{
		_t1_ = 0x71;
		_t2_ = 0x72;
	}
#endif

/*	//... update:050705 Rev2.0
	if(b_rc_fm_type == FM_TYPE_CIF){
		if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
			_data = (_data<<1)|0x01;
		}
	}
	//	*/

	_t3_ = (U8)(_data&0x00ff);
	WriteAsicByte(cmn_dvc,DVC_PG1,_t2_,_t3_);
	_t3_ = (U8)((_data&0xff00)>>2);
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,_t1_)&0x3f;
	WriteAsicByte(cmn_dvc,DVC_PG1,_t1_,_t3_|_rdat_);
}
//==================================================================================
void	SetWndwRcFmOSDPrd(U8 _pth)
{
	U16 _rdat_=0;
	U8 _t1_;

#if	defined(__4CH__)
//	if((b_rc_fm_type == FM_TYPE_MUX)&&(wndw_ds_sc_fmt != SC_FMT_8CH))	_t1_ = 0x00;
	if((wndw_rc_fm_type > CIF_TYPE_FL)&&(wndw_ds_sc_fmt != SC_FMT_8CH))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;
#elif	defined(__16CH__)
	_t1_ = 0x00;
#endif

	WriteOSDStr(_pth,MASTER,WNDW_RC_FM_PRD_X,WNDW_RC_FM_PRD_Y,_t1_,str_wndw_rc_prd);

	mse_flg_on_osd = MSE_ON_OSD_PRD;
	
	RunWndwRcFmOSDArw(_pth,0,0);
	
	_rdat_ = GetWndwRcFmPrd();
	SetWndwRcFmOSDNum(_pth,_rdat_);
}
//==================================================================================
//==================================================================================
void	SetWndwRcFmTypeCif(void)
{
	U8 _t1_;//, _t2_=0;

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);

//		if(wndw_crnt_dvc != _t2_){
//			_t2_ = wndw_crnt_dvc;
		if(!(_t1_&0x03)){
//			SetFrameOp(PTH_Y,wndw_crnt_dvc,wndw_rc_fm_cif_type);
			if(wndw_rc_fm_type == CIF_TYPE_FR)
				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x51,BIT6,0x00);		//... FRAME_OP=0
			else
				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x51,BIT6,BIT6);		//... FRAME_OP=1

			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x56,BIT6,0x00);								//... TRIG_MODE=0
////... update:050802 Rev2.1_ pseudo 8ch field by field mux switching test
//			SetAsicFlgType(MASTER,DVC_PG1,0x50,0xff,0x00);						//... set input mode on MPPDEC_X[3:0]

//#if	defined(__16CH__)
//			WaitCmnFldOd();
//			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x70,0xc0,0xc0);					//... POS_CTL_EN=1,POPUP_TRIG_MODE=1
//
////			//... update:050705 Rev2.0
//////			if(wndw_rc_fm_cif_type == CIF_TYPE_FR){									//... 360x120 / 360x144 field mode
////			if(wndw_rc_fm_type == CIF_TYPE_FR){
//////				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x54,0x60,0x00);				//... STRB_FLD=0
////SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x54,0x60,0x60);								//... STRB_FLD=3
////				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x57,BIT7,BIT7);				//... STRB_AUTO=1
////				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x71,BIT5,BIT5);				//... enable field base for POPUP_FLD
////			}
////			else{																					//... 360x240 / 360x288 field mode
////SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x54,0x60,0x00);								//... STRB_FLD=0
////				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x57,BIT7,0x00);				//... STRB_AUTO=0
////				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x71,BIT5,0x00);				//... disable field base for POPUP_FLD
////			}
//#endif
		}
		
//		if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
		if(wndw_rc_fm_type == CIF_TYPE_FR){
			SetChQd(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,wndw_crnt_ch);
		}
		else{
//			if(wndw_crnt_ch == 0)	SetPictureRatio(PTH_Y,wndw_crnt_dvc,0,0,0,0x12,0x12);
//			else if(wndw_crnt_ch == 1)	SetPictureRatio(PTH_Y,wndw_crnt_dvc,1,90,0,0x12,0x12);
//			else if(wndw_crnt_ch == 2)	SetPictureRatio(PTH_Y,wndw_crnt_dvc,2,0,global_bot>>1,0x12,0x12);
//			else if(wndw_crnt_ch == 3)	SetPictureRatio(PTH_Y,wndw_crnt_dvc,3,90,global_bot>>1,0x12,0x12);
			SetChQdPos(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,wndw_crnt_ch);

			SetSclRto(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,0x12,0x11);
//		//... update:050705 Rev2.0
//#if	defined(__16CH__)
////			SetChFuncMode(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_NRML);
//			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+wndw_crnt_ch*3,0x30,0x00);				//... FUNC_MODE=0
//#endif
		}
		//... update:050705 Rev2.0
#if	defined(__4CH__)
		if(wndw_ds_sc_fmt != SC_FMT_8CH)
//			SetChFuncMode(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_NRML);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+wndw_crnt_ch*3,0x30,0x00);				//... FUNC_MODE=0
#endif
	}

#if	defined(__16CH__)
	WaitCmnFldOd();
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);
		SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+wndw_crnt_ch*3,0x30,0x00);				//... FUNC_MODE=0
	}
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		if(!(_t1_&0x03)){
			SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x70,0xc0,0xc0);					//... POS_CTL_EN=1,POPUP_TRIG_MODE=1
		}
	}
#endif
}
//==================================================================================
void	SetWndwRcFmTypeMux(void)
{
	U8 _t1_;//, _t2_=0;//, _t3_;

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);

//		if(wndw_crnt_dvc != _t2_){
//			_t2_ = wndw_crnt_dvc;
		if(!(_t1_&0x03)){
//			SetFrameOp(PTH_Y,wndw_crnt_dvc,OFF);
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x51,BIT6,0x00);		//... FRAME_OP=0

//			_t3_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG1,0x56);
			if(wndw_ds_sc_fmt == SC_FMT_8CH){
//				ClearBit(_t3_,BIT6);
				SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x56,BIT6,0x00);		//... TRIG_MODE=0
			}
			else{
//				if(wndw_rc_fm_mux_type == MUX_TYPE_LV){
				if(wndw_rc_fm_type == MUX_TYPE_LV){
	//				SetBit(_t3_,BIT7);
	//				SetBit(_t3_,0x30);
					SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x56,BIT7|BIT6|0x30,BIT7|BIT6|0x30);		//... MUX_LIVE=1, TRIG_MODE=1, MUX_FLD=3
				}
				else if(wndw_rc_fm_type == MUX_TYPE_FR){
	//				ClearBit(_t3_,BIT7);
	//				SetBit(_t3_,0x30);
					SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x56,BIT7|BIT6|0x30,BIT6|0x30);		//... MUX_LIVE=0, TRIG_MODE=1, MUX_FLD=3
				}
				else{
	//				ClearBit(_t3_,BIT7|0x30);
					SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x56,BIT7|BIT6|0x30,BIT6);		//... MUX_LIVE=0, TRIG_MODE=1, MUX_FLD=0
				}
			}
//			WriteAsicByte(wndw_crnt_dvc,DVC_PG1,0x56,_t3_);
#if	defined(__16CH__)
			SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x70,0xc0,0x00);			//... POS_CTL_EN=0,POPUP_TRIG_MODE=0
#endif
		}
		SetChFl(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch);
//		SetChFuncMode(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,POP_UP,ON);
//		SetChFuncMode(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_MUX);
		SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+wndw_crnt_ch*3,0x70,0x60);		//... POP_UP_EN=1,FUNC_MODE=2
	}
}
//==================================================================================
void	SetWndwRcFmType(void)
{
	U8 _t1_;//, _t2_=0;
//	U16 _dly_;

	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);
//		SetChFuncMode(PTH_Y,wndw_crnt_dvc,wndw_crnt_ch,NULL,CH_FUNC_STRB);
		SetAsicFlgType(wndw_crnt_dvc,DVC_PG1,0x60+wndw_crnt_ch*3,0x30,0x10);		//... FUNC_MODE=1
	}
////	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
////		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);
////		if(wndw_crnt_dvc != _t2_){
////			_t2_ = wndw_crnt_dvc;
////		}
////	}
//
//	for(_dly_=0;_dly_<30000;_dly_++){											//... check finishing strobe for max 30000 times
//		_t1_=0;
//		_t1_ |= GetAsicFlgType(MASTER,DVC_PG1,0x54,0x0f);
//#if	defined(__16CH__)
//		_t1_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x54,0x0f);
//		_t1_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x54,0x0f);
//		_t1_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x54,0x0f);
//#endif
//		if(_t1_==0)	break;
//	}

//	WaitChStrbDoneY(0x0f);				//... check for four channels of each devices
	WaitChStrbDoneY(0xffff);				//... check for four channels of each devices

//	if(b_rc_fm_type == FM_TYPE_CIF)	SetWndwRcFmTypeCif();
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		SetWndwRcFmTypeCif();
	else	SetWndwRcFmTypeMux();
}
//==================================================================================
//==================================================================================
void	CreateWndwRcFm(U8 _pth)
{
//	b_status_wndw = ON;
	b_cmn_wndw_mn = ON;

	WriteOSDClr(_pth,MASTER,WNDW_RC_FM_POS_X,WNDW_RC_FM_POS_Y,WNDW_RC_FM_L,WNDW_RC_FM_H);
	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_RC_FM_POS_X,WNDW_RC_FM_POS_Y,WNDW_RC_FM_L,WNDW_RC_FM_H,str_wndw_ttl_rc_fm);

//	SetWndwRcFmOSDType(_pth);

	SetWndwRcFmOSDCif(_pth);

	SetWndwRcFmOSDMux(_pth);
	SetWndwRcFmOSDPrd(_pth);
	SetWndwRcFmOSDBtn(_pth);
}
//==================================================================================
void	RunWndwRcFmType(U8 _pth)//U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	U16 _rdat_;
	U8 _dvc_, _ch_;
	U8 _t1_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


#if	defined(__4CH__)
//	if(b_rc_fm_type == FM_TYPE_MUX)
	if((wndw_rc_fm_type > CIF_TYPE_FL)&&(wndw_ds_sc_fmt != SC_FMT_8CH))
#endif
		RunWndwRcFmOSDArw(_pth,_osd_pos_x_,_osd_pos_y_);

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_FM_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_CLOS_Y)){
//			SetBoxBtnStrPush(BOX_WNDW_QUIT);
		}

#if	defined(__4CH__)
//		else if((b_rc_fm_type == FM_TYPE_MUX)&&(wndw_ds_sc_fmt != SC_FMT_8CH)&&
		else if((wndw_rc_fm_type > CIF_TYPE_FL)&&(wndw_ds_sc_fmt != SC_FMT_8CH)&&
			((_osd_pos_x_ >= WNDW_RC_FM_BTN_X)&&(_osd_pos_x_ < (WNDW_RC_FM_BTN_X+9))))
#elif	defined(__16CH__)
		else if((_osd_pos_x_ >= WNDW_RC_FM_BTN_X)&&(_osd_pos_x_ < (WNDW_RC_FM_BTN_X+9)))
#endif
		{
//#if	defined(__4CH__)
			if(_osd_pos_y_ == WNDW_RC_FM_BTN_LOSS_Y){
//				SetBoxBtnStrPush(FM_BOX_LOSS);
			}
			else if(_osd_pos_y_ == WNDW_RC_FM_BTN_QUE_Y){
//				SetBoxBtnStrPush(FM_BOX_QUE);
			}
//#endif
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_FM_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WNDW_QUIT);
			
			WriteOSDClr(_pth,MASTER,WNDW_RC_FM_POS_X,WNDW_RC_FM_POS_Y,WNDW_RC_FM_L,WNDW_RC_FM_H);
			WriteOSDClrX(MASTER,WNDW_RC_FM_POS_X-1,WNDW_RC_FM_POS_Y-1,WNDW_RC_FM_L+2,WNDW_RC_FM_H+2);
//			WriteOSDClr(_pth,MASTER,16,4,15,15);
//			DisableBox(PTH_Y,FM_BOX_LOSS,BOX_WNDW_FRM);	// decoration wndwdow, main wndwdow

			SetOSDNoMn();

//			b_status_wndw = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
//		else if((_osd_pos_x_ == WNDW_RC_FM_RDO_X)&&((_osd_pos_y_ >= WNDW_RC_FM_RDO_CIF_FR_Y)&&(_osd_pos_y_ <= WNDW_RC_FM_RDO_MUX_LV_Y))){
//			_t1_ = _osd_pos_y_ - WNDW_RC_FM_RDO_CIF_FR_Y;
//			if(_t1_ != wndw_rc_fm_type){
//				wndw_rc_fm_type = _t1_;
//
//					SetWndwRcFmType();
//
//					SetWndwRcFmOSDCifRdo(_pth);
//					SetWndwRcFmOSDMuxRdo(_pth);
//
//					if(wndw_ds_sc_fmt != SC_FMT_8CH){
////						SetWndwRcFmOSDCif(_pth);
////						SetWndwRcFmOSDMux(_pth);
//
//						SetWndwRcFmOSDPrd(_pth);
//						SetWndwRcFmOSDBtn(_pth);
//
//						if(wndw_ds_sc_fmt == SC_FMT_1CH){
//							SetWndwDsScFmtFl();
//						}
//						else if(wndw_ds_sc_fmt == SC_FMT_4CH){
//							SetWndwDsScFmtLv();
//						}
//#if	defined(__16CH__)
//						else if(wndw_ds_sc_fmt == SC_FMT_16CH){
//							SetWndwDsScFmtLv();
//						}
//#endif
//					}
//#if	defined(__4CH__)
//					else{
//
//						SetWndwDsScFmtPsd8Ch();
//					}
//#endif
//			}
//		}
#if	defined(__4CH__)
		else if(wndw_ds_sc_fmt == SC_FMT_8CH){
			if((_osd_pos_x_ == WNDW_RC_FM_RDO_X)&&((_osd_pos_y_<=WNDW_RC_FM_RDO_CIF_FL_Y)||(_osd_pos_y_==WNDW_RC_FM_RDO_MUX_FL_Y))){
				_t1_ = _osd_pos_y_ - WNDW_RC_FM_RDO_CIF_FR_Y;
				if(_t1_ != wndw_rc_fm_type){
					wndw_rc_fm_type = _t1_;
	
					SetWndwRcFmType();
	
					SetWndwRcFmOSDCifRdo(_pth);
					SetWndwRcFmOSDMuxRdo(_pth);
	
//#if	defined(__4CH__)
					SetWndwDsScFmtPsd8Ch();
//#endif
				}
			}
		}
#endif
		else if((_osd_pos_x_ == WNDW_RC_FM_RDO_X)&&((_osd_pos_y_ >= WNDW_RC_FM_RDO_CIF_FR_Y)&&(_osd_pos_y_ <= WNDW_RC_FM_RDO_MUX_LV_Y))){
			_t1_ = _osd_pos_y_ - WNDW_RC_FM_RDO_CIF_FR_Y;
			if(_t1_ != wndw_rc_fm_type){
				wndw_rc_fm_type = _t1_;

				SetWndwRcFmType();

				SetWndwRcFmOSDCifRdo(_pth);
				SetWndwRcFmOSDMuxRdo(_pth);

//				if(wndw_ds_sc_fmt != SC_FMT_8CH){
////				SetWndwRcFmOSDCif(_pth);
////				SetWndwRcFmOSDMux(_pth);

					SetWndwRcFmOSDPrd(_pth);
					SetWndwRcFmOSDBtn(_pth);

//					if(wndw_ds_sc_fmt == SC_FMT_1CH){
//						SetWndwDsScFmtFl();
//					}
//					else if(wndw_ds_sc_fmt == SC_FMT_4CH){
//						SetWndwDsScFmtLv();
//					}
//#if	defined(__16CH__)
//					else if(wndw_ds_sc_fmt == SC_FMT_16CH){
//						SetWndwDsScFmtLv();
//					}
//#endif
////				}
////#if	defined(__4CH__)
////				else{
////
////					SetWndwDsScFmtPsd8Ch();
////				}
////#endif





#if defined(__4CH__)
					if(wndw_ds_sc_fmt == SC_FMT_4CH)
#elif defined(__16CH__)
					if(wndw_ds_sc_fmt == SC_FMT_16CH)
#endif
						SetWndwDsScFmtLvPb();

					for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
						SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
						InitChScaleAdapt(_dvc_,_ch_);
					}







			}
		}
		else if((_osd_pos_y_ == WNDW_RC_FM_PRD_Y)||(_osd_pos_y_ == WNDW_RC_FM_BTN_QUE_Y)||(_osd_pos_y_ == WNDW_RC_FM_BTN_LOSS_Y)){
#if	defined(__4CH__)
//			if((b_rc_fm_type == FM_TYPE_MUX)&&(wndw_ds_sc_fmt != SC_FMT_8CH)){
			if((wndw_rc_fm_type > CIF_TYPE_FL)&&(wndw_ds_sc_fmt != SC_FMT_8CH)){
#endif
				if(_osd_pos_y_ == WNDW_RC_FM_PRD_Y){
					if((_osd_pos_x_ == WNDW_RC_FM_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_INC_X)){
						_rdat_ = GetWndwRcFmPrd();
						if(_osd_pos_x_ == WNDW_RC_FM_DEC_X){
							if(_rdat_ > 0)	_rdat_--;
							else	_rdat_ = 0x03ff;
						}
						else{// if(_osd_pos_x_ == WNDW_RC_FM_INC_X){
							if(_rdat_ < 0x03ff)	_rdat_++;
							else	_rdat_ = 0;
						}
						SetWndwRcFmOSDNum(_pth,_rdat_);

						SetWndwRcFmPrd(_rdat_);
					}
				}
//#if	defined(__4CH__)
				else if((_osd_pos_x_ >= WNDW_RC_FM_BTN_X)&&(_osd_pos_x_ < (WNDW_RC_FM_BTN_X+9))){
					if(_osd_pos_y_ == WNDW_RC_FM_BTN_LOSS_Y){
//						SetBoxBtnStrPull(FM_BOX_LOSS);

						CreateWndwRcFmLs();
					}
					else if(_osd_pos_y_ == WNDW_RC_FM_BTN_QUE_Y){
//						SetBoxBtnStrPull(FM_BOX_QUE);

						CreateWndwRcFmQu();
					}
				}
//#endif
#if	defined(__4CH__)
			}
#endif
		}
	}
	if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

#if	defined(__4CH__)
//		if((b_rc_fm_type == FM_TYPE_MUX)&&(_osd_pos_y_ == WNDW_RC_FM_PRD_Y))
		if((wndw_rc_fm_type > CIF_TYPE_FL)&&(_osd_pos_y_ == WNDW_RC_FM_PRD_Y))
#elif	defined(__16CH__)
		if(_osd_pos_y_ == WNDW_RC_FM_PRD_Y)
#endif
		{
			if((_osd_pos_x_ == WNDW_RC_FM_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_INC_X)){
				_rdat_ = GetWndwRcFmPrd();
				if(_osd_pos_x_ == WNDW_RC_FM_DEC_X){
					if(_rdat_ > 0)	_rdat_--;
//					else	_rdat_ = 0x01fe;
					else	_rdat_ = 0x03ff;
				}
				else{// if(_osd_pos_x_ == WNDW_RC_FM_INC_X){
					if(_rdat_ < 0x03ff)	_rdat_++;
//					if(_rdat_ < 0x01fe)	_rdat_++;
					else	_rdat_ = 0;
				}
				SetWndwRcFmOSDNum(_pth,_rdat_);

				SetWndwRcFmPrd(_rdat_);
			}
		}
	}
	if(b_mse_pshng_clk_rb == 1)	b_mse_pshng_clk_rb = 0;
}
//==================================================================================
void	RunWndwRcFm(U8 _pth)//U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	if(b_rc_fm_que == ON){
		RunWndwRcFmQu();//_osd_pos_x_,_osd_pos_y_);
	}
	else if(b_rc_fm_loss == ON){
		RunWndwRcFmLs();//_osd_pos_x_,_osd_pos_y_);
	}
	else
		RunWndwRcFmType(_pth);//_osd_pos_x_,_osd_pos_y_);
}
//==================================================================================

