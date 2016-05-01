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
//==================================================================================
U8 wndw_rc_fm_qu_scrl_step = 0;
U8 wndw_rc_fm_qu_size = 0;

U8 wndw_rc_fm_qu_scrl_grip_vt;

//U8 idata	wndw_rc_fm_qu_bak[8];


//==================================================================================
//						Display wndwdow function description
//==================================================================================
#if	defined(__16CH__)
U8	GetWndwRcFmQuOfst(void)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _t1_;

	if(cmn_lst_slvr_y == MASTER)	_t1_ = 0;
	else if(cmn_lst_slvr_y == SLAVE1){
		if(cmn_rc_md == RC_MD_480_FPS)	_t1_ = 4;
		else if(cmn_rc_md == RC_MD_240_FPS)	_t1_ = 0;
	}
	else if(cmn_lst_slvr_y == SLAVE2)	_t1_ = 8;
	else if(cmn_lst_slvr_y == SLAVE3){
		if(cmn_rc_md == RC_MD_480_FPS)	_t1_ = 12;
		else if(cmn_rc_md == RC_MD_240_FPS)	_t1_ = 8;
		else	_t1_ = 0;
	}

	return	_t1_;
}
#endif
//==================================================================================
//==================================================================================
U8	GetWndwRcFmQuSz(void)
{
	U8 _rdat_;
	
#if	defined(__4CH__)
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x57);
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF){
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
		_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x71)&0x1f;				//... get POPUP_SIZE
	}
	else{
		//... update:050706 Rev2.0
		_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x57)&0x7f;		//... get QUEUE_SIZE
	}
#endif

	return	_rdat_;
}
//==================================================================================
void	SetWndwRcFmQuSz(U8 _data)
{
#if	defined(__4CH__)
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x57,_data);
#elif	defined(__16CH__)
	U8 _rdat_;

//	if(b_rc_fm_type == FM_TYPE_CIF){
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
		_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x71) & ~0x1f;
		WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x71,_rdat_|_data);
	}
	else{
		WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x57,_data);
	}
#endif
}
//==================================================================================
void	SetWndwRcFmQuOSDSzNum(U8 _data)
{
	U8 _t1_;

	_t1_ = _data%100;
	_t1_ = ConvDec2Bcd(_t1_);
	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_SZ_NUM_X,WNDW_RC_FM_QU_SZ_NUM_Y,0x00,(_t1_>>4)+0x10);
	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_SZ_NUM_X+1,WNDW_RC_FM_QU_SZ_NUM_Y,0x00,(_t1_&0x0f)+0x10);
	_t1_ = _data/100;
	if(_t1_ == 0)	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_SZ_NUM_X-1,WNDW_RC_FM_QU_SZ_NUM_Y,OSD_CLR,0x00);
	else	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_SZ_NUM_X-1,WNDW_RC_FM_QU_SZ_NUM_Y,0x00,_t1_+0x10);
}
//==================================================================================
void	SetWndwRcFmQuOSDSz(void)
{
#if	defined(__4CH__)
	SetOSDGrp(PTH_Y,WNDW_RC_FM_QU_SZ_DEC_X-2,WNDW_RC_FM_QU_SZ_NUM_Y,16,1,0x00,str_wndw_grp_size);
#elif	defined(__16CH__)
	SetOSDGrp(PTH_Y,WNDW_RC_FM_QU_SZ_DEC_X-1,WNDW_RC_FM_QU_SZ_NUM_Y,9,1,0x00,str_wndw_grp_size);
#endif

	mse_flg_on_osd |= MSE_ON_OSD_SIZE;
	
	RunMseOnOSDH(PTH_Y,0,0,WNDW_RC_FM_QU_SZ_DEC_X,WNDW_RC_FM_QU_SZ_INC_X,WNDW_RC_FM_QU_SZ_NUM_Y,MSE_ON_OSD_SIZE);
	wndw_rc_fm_qu_size = GetWndwRcFmQuSz();
	SetWndwRcFmQuOSDSzNum(wndw_rc_fm_qu_size);
}
//==================================================================================
#if	defined(__16CH__)
void	SetWndwRcFmQuOSDStgNum(U8 _dvc)
{
	U8 _t1_;

	if(_dvc == SLAVE3)	_t1_ = 3;
	else if(_dvc == SLAVE2)		_t1_ = 2;
	else if(_dvc == SLAVE1)		_t1_ = 1;
	else if(_dvc == MASTER)	_t1_ = 0;

	if(cmn_rc_md == RC_MD_120_FPS)
		WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_STG_NUM_X,WNDW_RC_FM_QU_STG_NUM_Y,OSD_MIX,_t1_+0x10);
	else
		WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_STG_NUM_X,WNDW_RC_FM_QU_STG_NUM_Y,0x00,_t1_+0x10);
}
//==================================================================================
void	SetWndwRcFmQuOSDStg(void)
{
	U8 _t1_;
	
	if(cmn_rc_md == RC_MD_120_FPS)	_t1_ = OSD_MIX;
	else	_t1_ = 0x00;

	SetOSDGrp(PTH_Y,WNDW_RC_FM_QU_STG_DEC_X-1,WNDW_RC_FM_QU_STG_NUM_Y,7,1,_t1_,str_wndw_grp_stg);

	mse_flg_on_osd |= MSE_ON_OSD_STG;
	
	RunMseOnOSDH(PTH_Y|_t1_,0,0,WNDW_RC_FM_QU_STG_DEC_X,WNDW_RC_FM_QU_STG_INC_X,WNDW_RC_FM_QU_STG_NUM_Y,MSE_ON_OSD_STG);
	SetWndwRcFmQuOSDStgNum(cmn_lst_slvr_y);
}
#endif
//==================================================================================
//==================================================================================
U8	GetWndwRcFmQuScrlStep(void)
{
	U16 _step_;

	_step_ = ReadAsicByte(MASTER,DVC_PG2,0x64+(QUE_2DBOX_SCRL<<3));
	_step_ <<= 1;
	_step_ |= (ReadAsicByte(MASTER,DVC_PG2,0x61+(QUE_2DBOX_SCRL<<3))&BIT0);

	return	(U8)((_step_-MSE_SCRL_QUE_VT)>>2);
}
//==================================================================================
void	SetWndwRcFmQuScrlBar(U8 _step)
{
	U8 _t1_;

#if	defined(__4CH__)
	_t1_ = MASTER;
#elif	defined(__16CH__)
	for(_t1_=SLAVE3;_t1_>=MASTER;_t1_>>=2)		//... 2dbox must be controlled for all chip
#endif
		Set2DBoxPos(_t1_,QUE_2DBOX_SCRL,MSE_SCRL_QUE_HL,MSE_SCRL_QUE_VT+(_step<<2));
}
//==================================================================================
void	SetWndwRcFmQuOSDScrl(void)
{
	U8 _t1_;
	
	mse_flg_on_osd |= MSE_ON_OSD_SCRL;
	RunMseOnOSDV(PTH_Y,0,0,WNDW_RC_FM_QU_SCRL_X,WNDW_RC_FM_QU_SCRL_UP_Y,WNDW_RC_FM_QU_SCRL_DN_Y,MSE_ON_OSD_SCRL);

#if	defined(__4CH__)
	_t1_ = MASTER;
#elif	defined(__16CH__)
	for(_t1_=SLAVE3;_t1_>=MASTER;_t1_>>=2)
#endif
	{		//... 2dbox must be controlled for all chip
		Set2DBox(_t1_,QUE_2DBOX_SCRL,MSE_SCRL_QUE_HL,MSE_SCRL_QUE_HW,MSE_SCRL_QUE_VT,MSE_SCRL_QUE_VW);
		Set2DBoxCell(_t1_,QUE_2DBOX_SCRL,1,1);
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_SCRL,NULL,_2DBOX_TBL_COL);
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_SCRL,_2DBOX_MIXEN|_2DBOX_BNDEN,OFF);
		SetAsicFlgType(_t1_,DVC_PG2,0x60+(QUE_2DBOX_SCRL<<3),BIT5|BIT3,0x00);		//... MDBOX_MD=0,MDBOX_MIX=0
		SetAsicFlgType(_t1_,DVC_PG2,0x61+(QUE_2DBOX_SCRL<<3),0x18,0x10);				//... DETAREA_EN=1, MDBND_EN=0

		Enable2DBox(PTH_Y,_t1_,QUE_2DBOX_SCRL);
	}

	SetWndwRcFmQuScrlBar(wndw_rc_fm_qu_scrl_step);

//	Enable2DBox(PTH_Y,MASTER,QUE_2DBOX_SCRL);
}
//==================================================================================
//==================================================================================
#if	defined(__16CH__)
void	SetWndwRcFmQuPopRdMd(U8 _que_rd_mode)
{
	U8 _rdat_;
	
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x70);					//... queue read control
	_rdat_ = (_rdat_ & 0xf8)|(_que_rd_mode>>4);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x70,_rdat_);					//... queue read control
}
//==================================================================================
void	GetWndwRcFmQuPopData(U8 _addr, U8 *_a4)
{
	U8 _rdat_;

	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x75,_addr);
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x73);
	_a4[0] = _rdat_>>4;
	_a4[1] = _rdat_&0x0f;
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x74);
	_a4[2] = _rdat_>>4;
	_a4[3] = _rdat_&0x0f;
}
//==================================================================================
void	SetWndwRcFmQuPopData(U8 _addr, U8 *_a4)
{
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x73,(_a4[0]<<4)|_a4[1]);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x74,(_a4[2]<<4)|_a4[3]);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x75,BIT7|_addr);
}
//==================================================================================
void	LoadWndwRcFmQuOSDTblPop(void)
{
	U8 _t1_, _t2_, _t3_, _t4_;
	U8 _a4_[4];
	U8 _dat_;

	SetWndwRcFmQuPopRdMd(QUE_RD_MD_LD);

	for(_t1_=0;_t1_<8;_t1_++){
		for(_t2_=0;_t2_<2;_t2_++){
			_t4_ = (_t1_<<1)|_t2_;
			_t4_ += wndw_rc_fm_qu_scrl_step<<1;
			if(_t4_ <= wndw_rc_fm_qu_size){
				GetWndwRcFmQuPopData(_t4_,_a4_);

//				for(_t3_=0;_t3_<4;_t3_++)
//					DisplayValue(PTH_Y,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<3)+(_t3_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,0xff,_a4_[_t3_],1);
				for(_t3_=0;_t3_<4;_t3_++){
					_dat_ = _a4_[_t3_]&0x0f;
					if(_dat_ < 0x0a)	_dat_ += 0x10;//0x30;	
					else	_dat_ += 0x17;//0x37;
					WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<3)+(_t3_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,0x00,_dat_);
				}
			}
			else{
				for(_t3_=0;_t3_<4;_t3_++)
					WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<3)+(_t3_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,OSD_CLR,0x00);
			}
		}
	}
}
#endif
//==================================================================================
void	SetWndwRcFmQuMuxRdMd(U8 _que_rd_mode)
{
	U8 _rdat_;

	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5b);					//... queue read control
	_rdat_ = (_rdat_ & 0x8f)|_que_rd_mode;
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5b,_rdat_);					//... queue read control
}
//==================================================================================
U8	GetWndwRcFmQuMuxData(U8 _addr)
{
	U8 _rdat_;
	
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5a,_addr);
	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x59);

	return	(_rdat_&0x0f);
}
//==================================================================================
void	SetWndwRcFmQuMuxData(U8 _addr, U8 _data)
{
	U8 _rdat_;

	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x59);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x59,(_rdat_&0xf0)|_data);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5a,BIT7|_addr);
}
//==================================================================================
void	LoadWndwRcFmQuOSDTblMux(void)
{
	U8 _rdat_;
	U8 _t1_, _t2_, _t3_;

	SetWndwRcFmQuMuxRdMd(QUE_RD_MD_LD);

	for(_t1_=0;_t1_<8;_t1_++){
		for(_t2_=0;_t2_<8;_t2_++){
			_t3_ = (_t1_<<3)|_t2_; 
			_t3_ += wndw_rc_fm_qu_scrl_step<<3;
			if(_t3_ <= wndw_rc_fm_qu_size){
				_rdat_ = GetWndwRcFmQuMuxData(_t3_);

//				DisplayValue(PTH_Y,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,0xff,_rdat_,1);
				_rdat_ &= 0x0f;
				if(_rdat_ < 0x0a)	_rdat_ += 0x10;//0x30;	
				else	_rdat_ += 0x17;//0x37;
				WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,0x00,_rdat_);
			}
			else{
				WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_t2_<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_t1_,OSD_CLR,0x00);
			}
		}
	}
}
//==================================================================================
void	SetWndwRcFmQuOSDTblCol(void)
{
	U8 _t1_;

	for(_t1_=SLAVE3;_t1_>=MASTER;_t1_>>=2){		//... 2dbox must be controlled for all chip
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_COL,NULL,_2DBOX_TBL_NO_COL);
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_COL,_2DBOX_BNDEN,ON);
		SetAsicFlgType(_t1_,DVC_PG2,0x60+(QUE_2DBOX_COL<<3),BIT5|BIT3,0x00);		//... MDBOX_MD=0,MDBOX_MIX=0
		SetAsicFlgType(_t1_,DVC_PG2,0x61+(QUE_2DBOX_COL<<3),0x18,0x08);		//... DETAREA_EN=0,MDBND_EN=1

		Set2DBoxTable(PTH_Y,_t1_,QUE_2DBOX_COL,WNDW_RC_FM_QU_TBL_COL_X,WNDW_RC_FM_QU_TBL_Y,8,1,1);
	}
	for(_t1_=0;_t1_<8;_t1_++){
		WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_COL_X+(_t1_<<1),WNDW_RC_FM_QU_TBL_Y,0x00,_t1_+0x10);
	}
}
//==================================================================================
void	SetWndwRcFmQuOSDTblRow(void)
{
	U8 _t1_;
	U8 _stp_ofst_;

#if	defined(__4CH__)
	_stp_ofst_ = 3;
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		_stp_ofst_ = 1;
	else	_stp_ofst_ = 3;
#endif

	for(_t1_=SLAVE3;_t1_>=MASTER;_t1_>>=2){		//... 2dbox must be controlled for all chip
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_ROW,NULL,_2DBOX_TBL_NO_COL);
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_ROW,_2DBOX_BNDEN,ON);
		SetAsicFlgType(_t1_,DVC_PG2,0x60+(QUE_2DBOX_ROW<<3),BIT5|BIT3,0x00);		//... MDBOX_MD=0,MDBOX_MIX=0
		SetAsicFlgType(_t1_,DVC_PG2,0x61+(QUE_2DBOX_ROW<<3),0x18,0x08);		//... DETAREA_EN=0, MDBND_EN=1

		Set2DBoxTable(PTH_Y,_t1_,QUE_2DBOX_ROW,WNDW_RC_FM_QU_TBL_ROW_X,WNDW_RC_FM_QU_TBL_Y,1,9,2);
	}
	for(_t1_=0;_t1_<8;_t1_++){
//		DisplayValue(PTH_Y,WNDW_RC_FM_QU_TBL_ROW_X,WNDW_RC_FM_QU_TBL_Y+1+_t1_,0xff,(_t1_+wndw_rc_fm_qu_scrl_step)<<_stp_ofst_,2);
		DsplyOSDDatHexY(MASTER,WNDW_RC_FM_QU_TBL_ROW_X,WNDW_RC_FM_QU_TBL_Y+1+_t1_,0xff,(_t1_+wndw_rc_fm_qu_scrl_step)<<_stp_ofst_);
	}
}
//==================================================================================
void	SetWndwRcFmQuOSDTblNum(void)
{
	U8 _t1_;

	for(_t1_=SLAVE3;_t1_>=MASTER;_t1_>>=2){		//... 2dbox must be controlled for all chip
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_NUM,NULL,_2DBOX_TBL_NO_COL);
//		Set2DBoxCtrl(_t1_,QUE_2DBOX_NUM,_2DBOX_BNDEN,ON);
		SetAsicFlgType(_t1_,DVC_PG2,0x60+(QUE_2DBOX_NUM<<3),BIT5|BIT3,0x00);		//... MDBOX_MD=0,MDBOX_MIX=0
		SetAsicFlgType(_t1_,DVC_PG2,0x61+(QUE_2DBOX_NUM<<3),0x18,0x08);		//... DETAREA_EN=0, MDBND_EN=1

		Set2DBoxTable(PTH_Y,_t1_,QUE_2DBOX_NUM,WNDW_RC_FM_QU_TBL_NUM_X,WNDW_RC_FM_QU_TBL_NUM_Y,8,8,1);
	}

#if	defined(__4CH__)
	LoadWndwRcFmQuOSDTblMux();
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		LoadWndwRcFmQuOSDTblPop();
	else	LoadWndwRcFmQuOSDTblMux();
#endif
}
//==================================================================================
void	CreateWndwRcFmQu(void)
{
	U8 _dvc_, _ch_;
	U8 _t1_;
	
	b_rc_fm_que = ON;

	WriteOSDClr(PTH_Y,MASTER,WNDW_RC_FM_QU_POS_X-1,WNDW_RC_FM_QU_POS_Y-1,WNDW_RC_FM_QU_L+2,WNDW_RC_FM_QU_H+2);
	SetOSDWndw(PTH_Y,OSD_WNDW_TYPE_MAIN,WNDW_RC_FM_QU_POS_X,WNDW_RC_FM_QU_POS_Y,WNDW_RC_FM_QU_L,WNDW_RC_FM_QU_H,str_wndw_ttl_que_stp);

//	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
//		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
//		wndw_rc_fm_qu_bak[(_t1_<<1)] = ReadAsicByte(_dvc_,DVC_PG2,0x60+(_ch_<<3));
//		wndw_rc_fm_qu_bak[(_t1_<<1)+1] = ReadAsicByte(_dvc_,DVC_PG2,0x61+(_ch_<<3));
//	}

	SetWndwRcFmQuOSDSz();
#if	defined(__16CH__)
	SetWndwRcFmQuOSDStg();
#endif
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG2,0x61+(_ch_<<3),BIT5,0x00);		//... clear all of mask enable
	}
	SetWndwRcFmQuOSDScrl();
	SetWndwRcFmQuOSDTblCol();
	SetWndwRcFmQuOSDTblRow();
	SetWndwRcFmQuOSDTblNum();
}
//==================================================================================
#if	defined(__16CH__)
void	RunWndwRcFmQuStg(U8 _osd_pos_x)
{
	if(_osd_pos_x == WNDW_RC_FM_QU_SZ_DEC_X){
		if(cmn_rc_md == RC_MD_240_FPS){
			if(cmn_lst_slvr_y == SLAVE1)	cmn_lst_slvr_y = SLAVE3;
			else	cmn_lst_slvr_y = SLAVE1;
		}
		else if(cmn_rc_md == RC_MD_480_FPS){
			if(cmn_lst_slvr_y > MASTER)	cmn_lst_slvr_y >>= 2;
			else	cmn_lst_slvr_y = SLAVE3;
		}
	}
	else{
		if(cmn_rc_md == RC_MD_240_FPS){
			if(cmn_lst_slvr_y == SLAVE1)	cmn_lst_slvr_y = SLAVE3;
			else	cmn_lst_slvr_y = SLAVE1;
		}
		else if(cmn_rc_md == RC_MD_480_FPS){
			if(cmn_lst_slvr_y < SLAVE3)	cmn_lst_slvr_y <<= 2;
			else	cmn_lst_slvr_y = MASTER;
		}
	}
	SetWndwRcFmQuOSDStgNum(cmn_lst_slvr_y);
	wndw_rc_fm_qu_size = GetWndwRcFmQuSz();
	SetWndwRcFmQuOSDSzNum(wndw_rc_fm_qu_size);
	SetWndwRcFmQuOSDTblRow();
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		LoadWndwRcFmQuOSDTblPop();
	else	LoadWndwRcFmQuOSDTblMux();
}
#endif
//==================================================================================
void	RunWndwRcFmQuSz(U8 _osd_pos_x)
{
	U8 _t1_;

#if	defined(__4CH__)
	_t1_ = 127;
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		_t1_ = 31;
	else	_t1_ = 127;
#endif

	wndw_rc_fm_qu_size = GetWndwRcFmQuSz();
	if(_osd_pos_x == WNDW_RC_FM_QU_SZ_DEC_X){
		if(wndw_rc_fm_qu_size > 0)	wndw_rc_fm_qu_size--;
		else	wndw_rc_fm_qu_size = _t1_;
	}
	else{
		if(wndw_rc_fm_qu_size < _t1_)	wndw_rc_fm_qu_size++;
		else	wndw_rc_fm_qu_size = 0;
	}
	SetWndwRcFmQuSz(wndw_rc_fm_qu_size);
	SetWndwRcFmQuOSDSzNum(wndw_rc_fm_qu_size);
	
	SetWndwRcFmQuOSDTblRow();

#if	defined(__4CH__)
	LoadWndwRcFmQuOSDTblMux();
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		LoadWndwRcFmQuOSDTblPop();
	else	LoadWndwRcFmQuOSDTblMux();
#endif
}
//==================================================================================
void	RunWndwRcFmQuScrl(U8 _osd_pos_y)
{
	if(_osd_pos_y == WNDW_RC_FM_QU_SCRL_UP_Y){
		if(wndw_rc_fm_qu_scrl_step > 0)	wndw_rc_fm_qu_scrl_step--;
	}
	else{
		if(wndw_rc_fm_qu_scrl_step < 8)	wndw_rc_fm_qu_scrl_step++;
	}
	SetWndwRcFmQuScrlBar(wndw_rc_fm_qu_scrl_step);

	SetWndwRcFmQuOSDTblRow();

#if	defined(__4CH__)
	LoadWndwRcFmQuOSDTblMux();
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		LoadWndwRcFmQuOSDTblPop();
	else	LoadWndwRcFmQuOSDTblMux();
#endif
}
//==================================================================================
#if	defined(__16CH__)
void	RunWndwRcFmQuTblPop(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal)		//... S8 : signed char
{
	U8 _a4_[4];
	U8 _addr_, _addr_sub_;
	U8 _t1_;//, _t2_;
	U8 _ofst_;
	U8 _dat_;

	_osd_pos_x -= WNDW_RC_FM_QU_TBL_NUM_X;
	_osd_pos_y -= WNDW_RC_FM_QU_TBL_NUM_Y;
	_t1_ = ((cmn_max_ch>>cmn_rc_md)-1);
	_ofst_ = GetWndwRcFmQuOfst();
	if(((_osd_pos_x >= 0)&&(_osd_pos_x < 15))&&((_osd_pos_y >= 0)&&(_osd_pos_y < 8))){
		if(BitClear(_osd_pos_x,BIT0)){
			_osd_pos_x >>= 1;													//... _osd_pos_x : 0 ~ 7
			_addr_ = (_osd_pos_y<<1)|(_osd_pos_x>>2);						//... addr
			_addr_sub_ = _osd_pos_x&0x03;									//... sub addr : 0 ~ 3

			_addr_ += wndw_rc_fm_qu_scrl_step<<1;
			if(_addr_ <= wndw_rc_fm_qu_size){
				SetWndwRcFmQuPopRdMd(QUE_RD_MD_LD);
				GetWndwRcFmQuPopData(_addr_,_a4_);
				
				SetWndwRcFmQuPopRdMd(QUE_RD_MD_WRT);
				if(_cal == QUE_TBL_NUM_DEC){							//... decrement
					if(_a4_[_addr_sub_] > _ofst_)	_a4_[_addr_sub_]--;
					else	_a4_[_addr_sub_] = _ofst_+_t1_;
				}
				else{															//... increment
					if(_a4_[_addr_sub_] < (_ofst_+_t1_))	_a4_[_addr_sub_]++;
					else	_a4_[_addr_sub_] = _ofst_;
				}
				SetWndwRcFmQuPopData(_addr_,_a4_);

//				DisplayValue(PTH_Y,WNDW_RC_FM_QU_TBL_NUM_X+(_osd_pos_x<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_osd_pos_y,0xff,_a4_[_addr_sub_],1);
				_dat_ = _a4_[_addr_sub_]&0x0f;
				if(_dat_ < 0x0a)	_dat_ += 0x10;//0x30;	
				else	_dat_ += 0x17;//0x37;
				WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_osd_pos_x<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_osd_pos_y,0x00,_dat_);
			}
		}
	}
}
#endif
//==================================================================================
void	RunWndwRcFmQuTblMux(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal)		//... S8 : signed char
{
	U8 _rdat_;
	U8 _addr_;
	U8 _t1_;//, _t2_;
	U8 _ofst_;

	_osd_pos_x -= WNDW_RC_FM_QU_TBL_NUM_X;
	_osd_pos_y -= WNDW_RC_FM_QU_TBL_NUM_Y;
#if	defined(__4CH__)
	_t1_ = 3;
	_ofst_ = 0;//_t2_ = 0;
#elif	defined(__16CH__)
	_t1_ = ((cmn_max_ch>>cmn_rc_md)-1);
	_ofst_ = GetWndwRcFmQuOfst();
#endif
	if(((_osd_pos_x >= 0)&&(_osd_pos_x < 15))&&((_osd_pos_y >= 0)&&(_osd_pos_y < 8))){
		if(BitClear(_osd_pos_x,BIT0)){
			_osd_pos_x >>= 1;

			_addr_ = (_osd_pos_y<<3)|(_osd_pos_x);
			_addr_ += wndw_rc_fm_qu_scrl_step<<3;
			if(_addr_ <= wndw_rc_fm_qu_size){
				SetWndwRcFmQuMuxRdMd(QUE_RD_MD_LD);
				_rdat_ = GetWndwRcFmQuMuxData(_addr_);
				
				SetWndwRcFmQuMuxRdMd(QUE_RD_MD_WRT);
				if(_cal == QUE_TBL_NUM_DEC){							//... decrement
					if(_rdat_ > _ofst_)	_rdat_--;
					else	_rdat_ = _ofst_+_t1_;
				}
				else{															//... increment
					if(_rdat_ < (_ofst_+_t1_))	_rdat_++;
					else	_rdat_ = _ofst_;
				}
				SetWndwRcFmQuMuxData(_addr_,_rdat_);

//				DisplayValue(PTH_Y,WNDW_RC_FM_QU_TBL_NUM_X+(_osd_pos_x<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_osd_pos_y,0xff,_rdat_,1);
				_rdat_ &= 0x0f;
				if(_rdat_ < 0x0a)	_rdat_ += 0x10;//0x30;	
				else	_rdat_ += 0x17;//0x37;
				WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_QU_TBL_NUM_X+(_osd_pos_x<<1),WNDW_RC_FM_QU_TBL_NUM_Y+_osd_pos_y,0x00,_rdat_);
			}
		}
	}
}
//==================================================================================
void	RunWndwRcFmQuTbl(S8 _osd_pos_x, S8 _osd_pos_y, U8 _cal)
{
#if	defined(__4CH__)
	RunWndwRcFmQuTblMux(_osd_pos_x,_osd_pos_y,_cal);
#elif	defined(__16CH__)
//	if(b_rc_fm_type == FM_TYPE_CIF)
	if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
		RunWndwRcFmQuTblPop(_osd_pos_x,_osd_pos_y,_cal);
	else	RunWndwRcFmQuTblMux(_osd_pos_x,_osd_pos_y,_cal);
#endif
}
//==================================================================================
void	RunWndwRcFmQu(void)//U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _vt_, _vb_;
	U8 _x_,_y_;
	U8 _t1_, _t2_, _t3_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


#if	defined(__16CH__)
	if(cmn_rc_md != RC_MD_120_FPS){
		RunMseOnOSDH(PTH_Y,_osd_pos_x_,_osd_pos_y_,WNDW_RC_FM_QU_STG_DEC_X,WNDW_RC_FM_QU_STG_INC_X,WNDW_RC_FM_QU_STG_NUM_Y,MSE_ON_OSD_STG);
	}
#endif
	RunMseOnOSDH(PTH_Y,_osd_pos_x_,_osd_pos_y_,WNDW_RC_FM_QU_SZ_DEC_X,WNDW_RC_FM_QU_SZ_INC_X,WNDW_RC_FM_QU_SZ_NUM_Y,MSE_ON_OSD_SIZE);
	RunMseOnOSDV(PTH_Y,_osd_pos_x_,_osd_pos_y_,WNDW_RC_FM_QU_SCRL_X,WNDW_RC_FM_QU_SCRL_UP_Y,WNDW_RC_FM_QU_SCRL_DN_Y,MSE_ON_OSD_SCRL);

	if(b_mse_rlse_lb == 0){
		_x_ = (U8)reg_crsr_pos_x;
		_y_ = (U8)reg_crsr_pos_y;
		
		if(b_mse_init_scrl == 0){
			_vt_ = ReadAsicByte(MASTER,DVC_PG2,0x64+(QUE_2DBOX_SCRL<<3));
			_vt_ = (_vt_<<1)+(MSE_SCRL_QUE_VT&BIT0);
			_vb_ = _vt_ + MSE_SCRL_QUE_VW;//_vb_;
			if(((_x_ >= MSE_SCRL_QUE_HL)&&(_x_ <= MSE_SCRL_QUE_HR))&&//(_y_ >= _vt_)){
				((_y_ >= _vt_)&&(_y_ <= _vb_))){
				b_mse_init_scrl = 1;

				wndw_rc_fm_qu_scrl_grip_vt = _y_ - _vt_;
			}
		}
		
		if(b_mse_init_scrl == 1){
			b_mse_pshng_clk_lb = 0;		//... clear "b_mse_pshng_clk_lb" flag by force because of protecting other pshng_clk functions

			_t1_ = _y_ - (wndw_rc_fm_qu_scrl_grip_vt+MSE_SCRL_QUE_VT);
			
			if((_t1_ >= 0)&&(_t1_ <= 32)){			//... scrl top pos. of step 0 ~ 8
				if((_x_ >= (MSE_SCRL_QUE_HL-32))&&(_x_ <= (MSE_SCRL_QUE_HR+32))){

#if	defined(__4CH__)
					_t2_ = MASTER;
#elif	defined(__16CH__)
					for(_t2_=SLAVE3;_t2_>=MASTER;_t2_>>=2)
#endif
						Set2DBoxPos(_t2_,QUE_2DBOX_SCRL,MSE_SCRL_QUE_HL,MSE_SCRL_QUE_VT+_t1_);

					_t1_ >>= 2;
					if(_t1_ != wndw_rc_fm_qu_scrl_step){
						wndw_rc_fm_qu_scrl_step = _t1_;

						SetWndwRcFmQuOSDTblRow();
#if	defined(__4CH__)
						LoadWndwRcFmQuOSDTblMux();
#elif	defined(__16CH__)
//						if(b_rc_fm_type == FM_TYPE_CIF)
						if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL))
							LoadWndwRcFmQuOSDTblPop();
						else	LoadWndwRcFmQuOSDTblMux();
#endif
					}
				}
			}
		}
	}
	else{
		if(b_mse_init_scrl == 1){
			b_mse_init_scrl = 0;

			wndw_rc_fm_qu_scrl_step = GetWndwRcFmQuScrlStep();
			SetWndwRcFmQuScrlBar(wndw_rc_fm_qu_scrl_step);
		}
	}

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_FM_QU_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_QU_CLOS_Y)){
//			SetBoxBtnStrPush(QUE_BOX_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_FM_QU_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_QU_CLOS_Y)){
//			SetBoxBtnStrPull(QUE_BOX_QUIT);
			
			WriteOSDClr(PTH_Y,MASTER,WNDW_RC_FM_QU_POS_X,WNDW_RC_FM_QU_POS_Y,WNDW_RC_FM_QU_L,WNDW_RC_FM_QU_H);
			WriteOSDClrX(MASTER,WNDW_RC_FM_QU_POS_X-1,WNDW_RC_FM_QU_POS_Y-1,WNDW_RC_FM_QU_L+2,WNDW_RC_FM_QU_H+2);
//			DisableBox(PTH_Y,QUE_BOX_QUIT,QUE_BOX_FRM);	// decoration wndwdow, main wndwdow

//			if(b_cmn_jp_vdo == VDO_NTSC)
//				WriteAsicTable(cmn_dvc,DVC_PG2,0x60,tbl_ntsc_pg2_2dbox,32);
//			else
//				WriteAsicTable(cmn_dvc,DVC_PG2,0x60,tbl_pal_pg2_2dbox,32);
			for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
				SetCrntDvcCh(_t1_,&_t2_,&_t3_);
				Aply2DBoxCh(_t2_,_t3_);
//				Enable2DBox(PTH_X,_t2_,_t3_);
//				WriteAsicByte(_t2_,DVC_PG2,0x60+(_t3_<<3),wndw_rc_fm_qu_bak[(_t1_<<1)]);
//				WriteAsicByte(_t2_,DVC_PG2,0x61+(_t3_<<3),wndw_rc_fm_qu_bak[(_t1_<<1)+1]);
				WriteAsicByte(_t2_,DVC_PG2,0x60+(_t3_<<3),0x88|_t1_);
				WriteAsicByte(_t2_,DVC_PG2,0x61+(_t3_<<3),0x00);
				WriteAsicByte(_t2_,DVC_PG2,0x66+(_t3_<<3),0xfb);
			}

			b_rc_fm_que = OFF;
			CreateWndwRcFm(PTH_Y);
		}
		else if(((_osd_pos_x_ == WNDW_RC_FM_QU_SZ_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_QU_SZ_INC_X))&&
			(_osd_pos_y_ == WNDW_RC_FM_QU_SZ_NUM_Y)){
			RunWndwRcFmQuSz(_osd_pos_x_);
		}
#if	defined(__16CH__)
		else if(((_osd_pos_x_ == WNDW_RC_FM_QU_STG_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_QU_STG_INC_X))&&
			(_osd_pos_y_ == WNDW_RC_FM_QU_STG_NUM_Y)){
			if(cmn_rc_md != RC_MD_120_FPS)	RunWndwRcFmQuStg(_osd_pos_x_);
		}
#endif
		else if((_osd_pos_x_ == WNDW_RC_FM_QU_SCRL_X)&&
			((_osd_pos_y_ == WNDW_RC_FM_QU_SCRL_UP_Y)||(_osd_pos_y_ == WNDW_RC_FM_QU_SCRL_DN_Y))){
			RunWndwRcFmQuScrl(_osd_pos_y_);
		}
		else{
			RunWndwRcFmQuTbl(_osd_pos_x_,_osd_pos_y_,QUE_TBL_NUM_DEC);
		}
	}
	if(b_mse_clk_rb == 1){
		b_mse_clk_rb = 0;

		RunWndwRcFmQuTbl(_osd_pos_x_,_osd_pos_y_,QUE_TBL_NUM_INC);
	}
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;

		if(((_osd_pos_x_ == WNDW_RC_FM_QU_SZ_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_QU_SZ_INC_X))&&
			(_osd_pos_y_ == WNDW_RC_FM_QU_SZ_NUM_Y)){
			RunWndwRcFmQuSz(_osd_pos_x_);
		}
#if	defined(__16CH__)
		else if(((_osd_pos_x_ == WNDW_RC_FM_QU_STG_DEC_X)||(_osd_pos_x_ == WNDW_RC_FM_QU_STG_INC_X))&&
			(_osd_pos_y_ == WNDW_RC_FM_QU_STG_NUM_Y)){
			if(cmn_rc_md != RC_MD_120_FPS)	RunWndwRcFmQuStg(_osd_pos_x_);
		}
#endif
		else if((_osd_pos_x_ == WNDW_RC_FM_QU_SCRL_X)&&
			((_osd_pos_y_ == WNDW_RC_FM_QU_SCRL_UP_Y)||(_osd_pos_y_ == WNDW_RC_FM_QU_SCRL_DN_Y))){
			RunWndwRcFmQuScrl(_osd_pos_y_);
		}
		else{
			RunWndwRcFmQuTbl(_osd_pos_x_,_osd_pos_y_,QUE_TBL_NUM_DEC);
		}
	}
	if(b_mse_pshng_clk_rb == 1){
		b_mse_pshng_clk_rb = 0;
		
		RunWndwRcFmQuTbl(_osd_pos_x_,_osd_pos_y_,QUE_TBL_NUM_INC);
	}
}
//==================================================================================

