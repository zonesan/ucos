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


//==================================================================================
U8 wndw_rc_fm_ls_md = LOSS_MD_BYPS;
U16 wndw_rc_fm_ls_sk_mn = 0;


//==================================================================================
//						Display wndwdow function description
//==================================================================================
void	SetWndwRcFmLsOSDMd(void)
{
	U8 _t1_, _t2_, _t3_;

	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X+2,WNDW_RC_FM_LS_RDO_MD_Y+LOSS_MD_BYPS,0x00,str_wndw_no_vid_byps);
	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X+2,WNDW_RC_FM_LS_RDO_MD_Y+LOSS_MD_HOLD,0x00,str_wndw_no_vid_hold);
	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X+2,WNDW_RC_FM_LS_RDO_MD_Y+LOSS_MD_BLUE,0x00,str_wndw_no_vid_blue_back);
//	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&(wndw_rc_fm_ls_md == LOSS_MD_SKIP))	_t3_ = 0x00;
	if(wndw_rc_fm_type >= MUX_TYPE_FR)	_t3_ = 0x00;
	else	_t3_ = OSD_MIX;
	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X+2,WNDW_RC_FM_LS_RDO_MD_Y+LOSS_MD_SKIP,_t3_,str_wndw_no_vid_skip);

	for(_t1_=0;_t1_<4;_t1_++){
		if(_t1_ == wndw_rc_fm_ls_md)	_t2_ = OSD_RDO_CHK;
		else	_t2_ = OSD_RDO_BLK;
		if(_t1_ == LOSS_MD_SKIP)
			WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X,WNDW_RC_FM_LS_RDO_MD_Y+_t1_,_t3_,_t2_);
		else
			WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_MD_X,WNDW_RC_FM_LS_RDO_MD_Y+_t1_,0x00,_t2_);
	}
}
//==================================================================================
void	SetWndwRcFmLsOSDSk(void)
{
	U8 _t1_;
#if defined(__4CH__)
	U8 _t2_, _t3_;

//	if(wndw_rc_fm_ls_md == LOSS_MD_SKIP)	_t1_ = 0x00;
	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&(wndw_rc_fm_ls_md == LOSS_MD_SKIP))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;

	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X+2,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_AUTO,_t1_,str_wndw_no_vid_auto);
	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X+2,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_MANL,_t1_,str_wndw_no_vid_manl);

	for(_t3_=0;_t3_<2;_t3_++){
		if(_t3_ == b_rc_fm_ls_sk)	_t2_ = OSD_RDO_CHK;
		else	_t2_ = OSD_RDO_BLK;
		WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X,WNDW_RC_FM_LS_RDO_SK_Y+_t3_,_t1_,_t2_);
	}
#elif defined(__16CH__)
//	if(wndw_rc_fm_ls_md == LOSS_MD_SKIP)	_t1_ = 0x00;
	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&(wndw_rc_fm_ls_md == LOSS_MD_SKIP))	_t1_ = 0x00;
	else	_t1_ = OSD_MIX;

	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X+2,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_AUTO,OSD_MIX,str_wndw_no_vid_auto);
	WriteOSDStr(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X+2,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_MANL,_t1_,str_wndw_no_vid_manl);

	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_AUTO,OSD_MIX,OSD_RDO_BLK);
	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_RDO_SK_X,WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_MANL,_t1_,OSD_RDO_CHK);
#endif
}
//==================================================================================
void	SetWndwRcFmLsOSDSkMn(U16 _data)
{
	U8 _rdat_l_, _rdat_h_;
	U8 _t1_, _t2_, _t3_, _t4_;

//	if((wndw_rc_fm_ls_md == LOSS_MD_SKIP)&&(b_rc_fm_ls_sk == LOSS_SKIP_MANL))	_t1_ = 0x00;
	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&((wndw_rc_fm_ls_md == LOSS_MD_SKIP)&&(b_rc_fm_ls_sk == LOSS_SKIP_MANL)))
		_t1_ = 0x00;
	else	_t1_ = OSD_MIX;

	SetOSDGrp(PTH_Y,WNDW_RC_FM_LS_SK_CH_X-1,WNDW_RC_FM_LS_SK_CH07_Y,17,4,_t1_,str_wndw_grp_skip_ch);

	_rdat_l_ = (U8)(_data&0x00ff);//ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5d);
	_rdat_h_ = (U8)((_data&0xff00)>>8);//ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5c);
	for(_t3_=0;_t3_<16;_t3_++){
#if	defined(__4CH__)
		if(_t3_ > 3)	_t1_ = OSD_MIX;
#endif
		if(_t3_ < 8){
			WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_SK_CH_X+(_t3_<<1),WNDW_RC_FM_LS_SK_CH07_Y,_t1_,0x10+_t3_);
			if(BitSet(_rdat_l_,BIT0<<_t3_))	_t2_ = OSD_CHK_BOX_CHK;
			else	_t2_ = OSD_CHK_BOX_BLK;
			WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_SK_CH_X+(_t3_<<1),WNDW_RC_FM_LS_SK_CHK07_Y,_t1_,_t2_);
		}
		else{
			_t4_ = _t3_-8;
			if(_t4_ < 2)	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_SK_CH_X+(_t4_<<1),WNDW_RC_FM_LS_SK_CH8F_Y,_t1_,0x18+_t4_);
			else	WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_SK_CH_X+(_t4_<<1),WNDW_RC_FM_LS_SK_CH8F_Y,_t1_,0x1f+_t4_);
			if(BitSet(_rdat_h_,BIT0<<_t4_))	_t2_ = OSD_CHK_BOX_CHK;
			else	_t2_ = OSD_CHK_BOX_BLK;
			WriteOSDFnt(PTH_Y,MASTER,WNDW_RC_FM_LS_SK_CH_X+(_t4_<<1),WNDW_RC_FM_LS_SK_CHK8F_Y,_t1_,_t2_);
		}
	}
}
//==================================================================================
//==================================================================================
//U16	GetWndwRcFmLsSkMn(void)
//{
//	U16 _rdat_=0;
//
//	_rdat_ = ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5c);
//	_rdat_ <<= 8;
//	_rdat_ |= ReadAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5d);
//	
//	return	_rdat_;
//}
//==================================================================================
void	SetWndwRcFmLsSkMn(U16 _data)
{
	U8 _t1_;

/*	//... update:050708 Rev2.0
	_t1_ = (U8)(_data&0x00ff);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5d,_t1_);
	_t1_ = (U8)((_data&0xff00)>>8);
	WriteAsicByte(cmn_lst_slvr_y,DVC_PG1,0x5c,_t1_);
	//	*/
	_t1_ = (U8)(_data&0x00ff);
	WriteAsicByte(cmn_dvc,DVC_PG1,0x5d,_t1_);
	_t1_ = (U8)((_data&0xff00)>>8);
	WriteAsicByte(cmn_dvc,DVC_PG1,0x5c,_t1_);
}
//==================================================================================
void	SetWndwRcFmLsMd(void)
{
	U8 _rdat_;
	U8 _t1_, _t2_=0;

#if	defined(__4CH__)
	//... auto is for only 4 ch
	for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
		SetCrntDvcCh(_t1_,&wndw_crnt_dvc,&wndw_crnt_ch);

//		if(wndw_crnt_dvc != _t2_){
//			_t2_ = wndw_crnt_dvc;
		if(!(_t1_&0x03)){
			_rdat_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG1,0x5b);
			if(wndw_rc_fm_ls_md == LOSS_MD_HOLD)	ClearBit(_rdat_,BIT3);
			else if(wndw_rc_fm_ls_md == LOSS_MD_SKIP)	SetBit(_rdat_,BIT3);
			WriteAsicByte(wndw_crnt_dvc,DVC_PG1,0x5b,_rdat_);
		}
	}
#endif
	
	if(wndw_rc_fm_ls_md == LOSS_MD_BYPS)	_t1_ = LOSS_MODE_BYPS;
	else if(wndw_rc_fm_ls_md == LOSS_MD_HOLD)	_t1_ = LOSS_MODE_FRZ;
	else if(wndw_rc_fm_ls_md == LOSS_MD_BLUE)	_t1_ = LOSS_MODE_BLAK;
	//... update:050708 Rev2.0
#if	defined(__4CH__)
	else if(wndw_rc_fm_ls_md == LOSS_MD_SKIP)	_t1_ = LOSS_MODE_FRZ;
#elif	defined(__16CH__)
	else if(wndw_rc_fm_ls_md == LOSS_MD_SKIP){
		if(wndw_rc_fm_type < MUX_TYPE_FR)	_t1_ = LOSS_MODE_FRZ;
		else	_t1_ = LOSS_MODE_BLK_FRZ;			//... LOSS_MODE_BLK_FRZ is same value as LOSS_MD_SKIP
	}
	_rdat_ = 0;
#endif

//	if((wndw_rc_fm_type < MUX_TYPE_FR)&&(wndw_rc_fm_ls_md == LOSS_MD_SKIP)){
////		wndw_rc_fm_ls_md = LOSS_MD_BYPS;
//		SetAsicFlgType(cmn_dvc,DVC_PG1,0x55,0xc0,0x00);
//	}
//	else
		SetAsicFlgType(cmn_dvc,DVC_PG1,0x55,0xc0,_t1_<<6);
}
//==================================================================================
//==================================================================================
void	InitWndwRcFmVarLsMd(void)
{
#if defined(__4CH__)
	U8 _rdat_;
	
	_rdat_ = GetAsicVar8Type(MASTER,DVC_PG1,0x55,0x62);
	if(_rdat_ == LOSS_MODE_FRZ){
		_rdat_ = ReadAsicByte(wndw_crnt_dvc,DVC_PG1,0x5b);
		if(BitSet(_rdat_,BIT3))	wndw_rc_fm_ls_md = LOSS_MD_SKIP;
		else	wndw_rc_fm_ls_md = LOSS_MD_HOLD;
	}
	else	wndw_rc_fm_ls_md = _rdat_;
#elif defined(__16CH__)
	if((wndw_rc_fm_type < MUX_TYPE_FR)&&(wndw_rc_fm_ls_md == LOSS_MD_SKIP))	wndw_rc_fm_ls_md = LOSS_MODE_FRZ;

	if(wndw_rc_fm_ls_md != LOSS_MD_SKIP){
//		SetWndwRcFmLsSkMn(0x0000);
		wndw_rc_fm_ls_sk_mn = 0;
		SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn);
	}
#endif
}
//==================================================================================
//==================================================================================
void	CreateWndwRcFmLs(void)
{
#if defined(__16CH__)
	U16 _rdat_=0;

	b_rc_fm_ls_sk = LOSS_SKIP_MANL;
#endif

	b_rc_fm_loss = ON;

	WriteOSDClr(PTH_Y,MASTER,WNDW_RC_FM_LS_POS_X-1,WNDW_RC_FM_LS_POS_Y-1,WNDW_RC_FM_LS_L+2,WNDW_RC_FM_LS_H+2);
	SetOSDWndw(PTH_Y,OSD_WNDW_TYPE_MAIN,WNDW_RC_FM_LS_POS_X,WNDW_RC_FM_LS_POS_Y,WNDW_RC_FM_LS_L,WNDW_RC_FM_LS_H,str_wndw_ttl_no_vid);

	InitWndwRcFmVarLsMd();
	
	SetWndwRcFmLsOSDMd();
	SetWndwRcFmLsOSDSk();
#if defined(__4CH__)
////	_rdat_ = GetWndwRcFmLsSkMn();
////	SetWndwRcFmLsOSDSkMn(_rdat_);
//	SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
	SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
#elif defined(__16CH__)
	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&((wndw_rc_fm_ls_md == LOSS_MD_SKIP)&&(b_rc_fm_ls_sk == LOSS_SKIP_MANL))){
//		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x78,0x0f);
//		_rdat_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x78,0x0f)<<4;
//		_rdat_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x78,0x0f)<<8;
//		_rdat_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x78,0x0f)<<12;
		_rdat_ = ChkNoVdo();
	}
//	else{
//		_rdat_ = 0;
//	}

////	_rdat_ = GetWndwRcFmLsSkMn();
////	SetWndwRcFmLsOSDSkMn(_rdat_);
//	SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
	SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
#endif
}
//==================================================================================
void	RunWndwRcFmLs(void)//U8 _osd_pos_x_, U8 _osd_pos_y_)
{
	U16 _rdat_=0;
	U8 _t1_;
	U8 _ofst_;
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


//	if((wndw_rc_fm_type >= MUX_TYPE_FR)&&((wndw_rc_fm_ls_md == LOSS_MD_SKIP)&&(b_rc_fm_ls_sk == LOSS_SKIP_MANL))){
//		_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x78,0x0f);
//		_rdat_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x78,0x0f)<<4;
//		_rdat_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x78,0x0f)<<8;
//		_rdat_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x78,0x0f)<<12;
//	}

	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;

		if((_osd_pos_x_ == WNDW_RC_FM_LS_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_LS_CLOS_Y)){
//			SetBoxBtnStrPush(LOSS_BOX_QUIT);
		}
	}
	if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;

		if(wndw_rc_fm_type >= MUX_TYPE_FR)	_ofst_ = LOSS_MD_SKIP;
		else	_ofst_ = LOSS_MD_BLUE;

		if((_osd_pos_x_ == WNDW_RC_FM_LS_CLOS_X)&&(_osd_pos_y_ == WNDW_RC_FM_LS_CLOS_Y)){
//			SetBoxBtnStrPull(LOSS_BOX_QUIT);
			
			WriteOSDClr(PTH_Y,MASTER,WNDW_RC_FM_LS_POS_X,WNDW_RC_FM_LS_POS_Y,WNDW_RC_FM_LS_L,WNDW_RC_FM_LS_H);
			WriteOSDClrX(MASTER,WNDW_RC_FM_LS_POS_X-1,WNDW_RC_FM_LS_POS_Y-1,WNDW_RC_FM_LS_L+2,WNDW_RC_FM_LS_H+2);
//			DisableBox(PTH_Y,LOSS_BOX_QUIT,LOSS_BOX_FRM);	// decoration wndwdow, main wndwdow

			b_rc_fm_loss = OFF;

			CreateWndwRcFm(PTH_Y);
		}
		else if((_osd_pos_y_ >= WNDW_RC_FM_LS_RDO_MD_Y)&&(_osd_pos_y_ <= (WNDW_RC_FM_LS_RDO_MD_Y+_ofst_))){
			if(_osd_pos_x_ == WNDW_RC_FM_LS_RDO_MD_X){
				_t1_ = _osd_pos_y_ - WNDW_RC_FM_LS_RDO_MD_Y;
				if(_t1_ != wndw_rc_fm_ls_md){
					if((_t1_ < LOSS_MD_SKIP)&&(wndw_rc_fm_ls_md < LOSS_MD_SKIP)){
						wndw_rc_fm_ls_md = _t1_;
					}
					else{
						wndw_rc_fm_ls_md = _t1_;

						SetWndwRcFmLsOSDSk();
#if defined(__4CH__)
//						SetWndwRcFmLsSkMn(0x0000);
//						SetWndwRcFmLsOSDSkMn(0x0000);
						wndw_rc_fm_ls_sk_mn = 0;
						SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn);
						SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
#elif defined(__16CH__)
						if(wndw_rc_fm_ls_md != LOSS_MD_SKIP){
//							SetWndwRcFmLsSkMn(0x0000);
//							SetWndwRcFmLsOSDSkMn(0x0000);
							wndw_rc_fm_ls_sk_mn = 0;
							SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn);
							SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
						}
						else{
							if(wndw_rc_fm_type >= MUX_TYPE_FR){
//								_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x78,0x0f);
//								_rdat_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x78,0x0f)<<4;
//								_rdat_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x78,0x0f)<<8;
//								_rdat_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x78,0x0f)<<12;
								_rdat_ = ChkNoVdo();
							}
//						//	else{
//						//		_rdat_ = 0;
//						//	}

//							SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
							SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
						}
#endif
						
					}
					SetWndwRcFmLsOSDMd();

					SetWndwRcFmLsMd();
				}
			}
		}
		else if(wndw_rc_fm_ls_md == LOSS_MD_SKIP){
			if((_osd_pos_y_ == WNDW_RC_FM_LS_RDO_SK_Y)||(_osd_pos_y_ == (WNDW_RC_FM_LS_RDO_SK_Y+LOSS_SKIP_MANL))){
				if(_osd_pos_x_ == WNDW_RC_FM_LS_RDO_SK_X){
#if defined(__4CH__)
					if(_osd_pos_y_ == WNDW_RC_FM_LS_RDO_SK_Y)	_t1_ = LOSS_SKIP_AUTO;
					else	_t1_ = LOSS_SKIP_MANL;
#elif defined(__16CH__)
					_t1_ = LOSS_SKIP_MANL;
#endif
					
					if(_t1_ != b_rc_fm_ls_sk){
						b_rc_fm_ls_sk = _t1_;

						SetWndwRcFmLsOSDSk();
						if(b_rc_fm_ls_sk == LOSS_SKIP_AUTO){
//							SetWndwRcFmLsSkMn(0x0000);
//							SetWndwRcFmLsOSDSkMn(0x0000);
							wndw_rc_fm_ls_sk_mn = 0;
							SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn);
							SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn);
						}
						else{
//							_rdat_ = GetWndwRcFmLsSkMn();
//							SetWndwRcFmLsOSDSkMn(_rdat_);
							_rdat_ = ChkNoVdo();
							SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
						}
					}
				}
			}
			else if(b_rc_fm_ls_sk == LOSS_SKIP_MANL){
#if	defined(__4CH__)
				if(_osd_pos_y_ == WNDW_RC_FM_LS_SK_CHK07_Y){
					if((_osd_pos_x_ >= WNDW_RC_FM_LS_SK_CH_X)&&(_osd_pos_x_ <= (WNDW_RC_FM_LS_SK_CH_X+6))){
						_t1_ = _osd_pos_x_ - WNDW_RC_FM_LS_SK_CH_X;
						if(BitClear(_t1_,BIT0)){
							_t1_ >>= 1;
#elif	defined(__16CH__)
				if((_osd_pos_y_ == WNDW_RC_FM_LS_SK_CHK07_Y)||(_osd_pos_y_ == WNDW_RC_FM_LS_SK_CHK8F_Y)){
					if((_osd_pos_x_ >= WNDW_RC_FM_LS_SK_CH_X)&&(_osd_pos_x_ <= (WNDW_RC_FM_LS_SK_CH_X+14))){
						_t1_ = _osd_pos_x_ - WNDW_RC_FM_LS_SK_CH_X;
						if(BitClear(_t1_,BIT0)){
							_t1_ >>= 1;
							if(_osd_pos_y_ == WNDW_RC_FM_LS_SK_CHK8F_Y)	_t1_ += 8;
#endif
//							_rdat_ = GetWndwRcFmLsSkMn();
//							_rdat_ ^= 0x0001<<_t1_;
//							SetWndwRcFmLsOSDSkMn(_rdat_);
//							SetWndwRcFmLsSkMn(_rdat_);
							_rdat_ = ChkNoVdo();

							wndw_rc_fm_ls_sk_mn ^= 0x0001<<_t1_;
							SetWndwRcFmLsSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
							SetWndwRcFmLsOSDSkMn(wndw_rc_fm_ls_sk_mn|_rdat_);
						}
					}
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

