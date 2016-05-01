#include "hdr.h"
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


U8 wndw_pop_sub_num_h;
U8 wndw_pop_sub_num_v;

U8 wndw_pop_crnt_ch;
U8 wndw_crnt_dvc;
U8 wndw_crnt_ch;
U8 wndw_mn_pos_crnt = 30;
//==================================================================================
//						Popup function description
//==================================================================================
void SetPopMainOSD(U8 _pth, U8 _pos_x, U8 _pos_y)
{
	if(BitSet(_pth,PTH_X)){
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_DS,0x00,str_wndw_pop_ds_sb);
//#if defined(__4CH__)
//		if((wndw_ds_sc_fmt == SC_FMT_4CH)&&(b_ds_wth_4pb))
//#elif defined(__16CH__)
//		if((wndw_ds_sc_fmt == SC_FMT_16CH)&&(b_ds_wth_16pb))
//#endif
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_CH,OSD_MIX,str_wndw_pop_ch);
//		else
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_CH,0x00,str_wndw_pop_ch);
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ZM,0x00,str_wndw_pop_zm);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_RC,0x00,str_wndw_pop_rc);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_MTN,0x00,str_wndw_pop_mtn);
#if defined(__WTH_ADO__)
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ADO,0x00,str_wndw_pop_ado);
#elif defined(__WTH_VGA__)
		if(cmn_vga_det)
			WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_VGA,0x00,str_wndw_pop_vga);
#endif
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_RG,0x00,str_wndw_pop_rg);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+(POP_POS_RG+cmn_vga_det),0x00,str_wndw_pop_rg);
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ABT,0x00,str_wndw_pop_abt);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_DS,0x00,str_wndw_pop_ds);
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_CH,OSD_MIX,str_wndw_pop_ch);  
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ZM,OSD_MIX,str_wndw_pop_zm);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_RC,0x00,str_wndw_pop_rc_sb);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_MTN,OSD_MIX,str_wndw_pop_mtn);
#if defined(__WTH_ADO__)
		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ADO,0x00,str_wndw_pop_ado);
#elif defined(__WTH_VGA__)
		if(cmn_vga_det)
			WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_VGA,0x00,str_wndw_pop_vga);
#endif
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_RG,0x00,str_wndw_pop_rg);
		WriteOSDStrY(MASTER,_pos_x,_pos_y+(POP_POS_RG+cmn_vga_det),0x00,str_wndw_pop_rg);
//		WriteOSDStrY(MASTER,_pos_x,_pos_y+POP_POS_ABT,0x00,str_wndw_pop_abt);
	}
}
//==================================================================================
void SetPopMain(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght)
{
//	_lngth = 0;			//... temporal
//	_hght = 0;			//... temporal
	CreateBoxPopFrame(_pth,BOX_POP_MN,_pos_x,_pos_y,_lngth,_hght);
//	IfBitSet(_pth,PTH_X)	SetPopMainOSD(PTH_X,_pos_x,_pos_y);
//	IfBitSet(_pth,PTH_Y)	SetPopMainOSD(PTH_Y,_pos_x,_pos_y);
	if(BitSet(_pth,PTH_X))	SetPopMainOSD(PTH_X,_pos_x,_pos_y);
	if(BitSet(_pth,PTH_Y))	SetPopMainOSD(PTH_Y,_pos_x,_pos_y);
}
//==================================================================================
void SetPopSubOSD(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _main_mn_pos_crnt)
{
//	U8 _t1_;

//	IfBitSet(_pth,PTH_X){
//	if(BitSet(_pth,PTH_X)){
//		if(_main_mn_pos_crnt == POP_POS_SCRN_X){
////			WriteOSDStrX(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_scrn_x_fl);
////			WriteOSDStrX(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_scrn_x_qd);
////			WriteOSDStrX(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_scrn_x_nrt);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_scrn_x_fl);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_scrn_x_qd);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_scrn_x_nrt);
//		}
//	}
////	IfBitSet(_pth,PTH_Y){
//	if(BitSet(_pth,PTH_Y)){
//		if(_main_mn_pos_crnt == POP_POS_SCRN_Y){
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_scrn_y_qd);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_scrn_y_cif);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_scrn_y_mux);
//		}
//	}
	if(BitSet(_pth,PTH_X)){
		if(_main_mn_pos_crnt == POP_POS_DS){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_scrn);
#if defined(__4CH__)
			if((wndw_ds_sc_fmt == SC_FMT_4CH)&&(b_ds_wth_4pb))
#elif defined(__16CH__)
			if((wndw_ds_sc_fmt == SC_FMT_16CH)&&(b_ds_wth_16pb))
#endif
				WriteOSDStrY(MASTER,_pos_x,_pos_y+1,OSD_MIX,str_wndw_pop_sb_src);
			else
				WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_src);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_zm);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+3,0x00,str_wndw_pop_sb_indx);
		}
		else if(_main_mn_pos_crnt == POP_POS_MTN){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_mtn);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_blnd);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_nght);
		}
#if defined(__WTH_ADO__)
		else if(_main_mn_pos_crnt == POP_POS_ADO){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_gn);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_lv);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_rc);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+3,0x00,str_wndw_pop_sb_dt);
		}
#elif defined(__WTH_VGA__)
//		else if(_main_mn_pos_crnt == POP_POS_VGA){
		else if((_main_mn_pos_crnt == POP_POS_VGA)&&(cmn_vga_det)){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_out_rs);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_de_int);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_img_en);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+3,0x00,str_wndw_pop_sb_ns_rd);
		}
#endif
	}
//	IfBitSet(_pth,PTH_Y){
	if(BitSet(_pth,PTH_Y)){
		if(_main_mn_pos_crnt == POP_POS_RC){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_fmt);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_indx);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_ns_rd);
		}
#if defined(__WTH_ADO__)
		else if(_main_mn_pos_crnt == POP_POS_ADO){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_gn);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_lv);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_rc);
//			WriteOSDStrY(MASTER,_pos_x,_pos_y+3,0x00,str_wndw_pop_sb_dt);
		}
#elif defined(__WTH_VGA__)
//		else if(_main_mn_pos_crnt == POP_POS_VGA){
		else if((_main_mn_pos_crnt == POP_POS_VGA)&&(cmn_vga_det)){
			WriteOSDStrY(MASTER,_pos_x,_pos_y+0,0x00,str_wndw_pop_sb_out_rs);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+1,0x00,str_wndw_pop_sb_de_int);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+2,0x00,str_wndw_pop_sb_img_en);
			WriteOSDStrY(MASTER,_pos_x,_pos_y+3,0x00,str_wndw_pop_sb_ns_rd);
		}
#endif
	}
}
//==================================================================================
void SetPopSub(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght, U8 _main_mn_pos_crnt)
{
//	_lngth = 0;			//... temporal
//	_hght = 0;			//... temporal
	CreateBoxPopFrame(_pth,BOX_POP_SB,_pos_x,_pos_y,_lngth,_hght);
//	IfBitSet(_pth,PTH_X)	SetPopSubOSD(PTH_X,_pos_x,_pos_y, _main_mn_pos_crnt);
//	IfBitSet(_pth,PTH_Y)	SetPopSubOSD(PTH_Y,_pos_x,_pos_y, _main_mn_pos_crnt);
	if(BitSet(_pth,PTH_X))	SetPopSubOSD(PTH_X,_pos_x,_pos_y, _main_mn_pos_crnt);
	if(BitSet(_pth,PTH_Y))	SetPopSubOSD(PTH_Y,_pos_x,_pos_y, _main_mn_pos_crnt);
}
//==================================================================================
//==================================================================================
void RemovePopMain(void)
{
//	U8 _t1_;
//	U8 _pth_;
//	
////	if(b_mouse_status_path_popup == MOUSE_PATH_X)	_pth_ = PTH_X;
//	if(BitClear(mse_flg,MSE_STTS_PTH_POP))	_pth_ = PTH_X;
//	else	_pth_ = PTH_Y;

//	for(_t1_=0;_t1_<4;_t1_++)
//		DisableBox(PTH_Y,_t1_);//BOX_POP_MN_HL,BOX_POP_MN);

//	if(BitSet(_pth_,PTH_X))	WriteOSDClrX(MASTER,mse_osd_x,mse_osd_y,POP_MN_NUM_H,POP_MN_NUM_V);
//	if(BitSet(_pth_,PTH_Y))	WriteOSDClrY(MASTER,mse_osd_x,mse_osd_y,POP_MN_NUM_H,POP_MN_NUM_V);
	
	if(b_mse_stts_pth_pop == MSE_PTH_X){
//	if(BitClear(mse_flg,MSE_STTS_PTH_POP))
//		WriteOSDClrX(MASTER,mse_osd_x,mse_osd_y,POP_MN_NUM_H,POP_MN_NUM_V);
		DisableBox(PTH_X,BOX_POP_MN_HL);
		DisableBox(PTH_X,BOX_POP_MN);
		WriteOSDClrY(MASTER,mse_osd_x,mse_osd_y,POP_MN_NUM_H,POP_MN_NUM_V+cmn_vga_det);	//... 060307:temporal
	}
	else{
		DisableBox(PTH_Y,BOX_POP_MN_HL);
		DisableBox(PTH_Y,BOX_POP_MN);
		WriteOSDClrY(MASTER,mse_osd_x,mse_osd_y,POP_MN_NUM_H,POP_MN_NUM_V+cmn_vga_det);
	}

	b_mse_stts_pth_pop = b_mse_stts_pth;
//	if(BitClear(mse_flg,MSE_STTS_PTH))	ClearBit(mse_flg,MSE_STTS_PTH_POP);
//	else	SetBit(mse_flg,MSE_STTS_PTH_POP);
}
//==================================================================================
void CreatePopMain(void)
{
	U16 _osd_lmt_v_ = cmn_bot<<1;							//... v limit for OSD index

	U8 _pop_num_h_, _pop_num_v_;
	U8 _pth_;

	_osd_lmt_v_ = (_osd_lmt_v_-OSD_DLY_V)/10;
	RemovePopMain();
	mse_osd_x = GetMseOSDX(reg_crsr_pos_x);
	mse_osd_y = GetMseOSDY(reg_crsr_pos_y);

	if(b_mse_stts_pth_pop == MSE_PTH_X){
//	if(BitClear(mse_flg,MSE_STTS_PTH_POP)){
		_pth_ = PTH_X;
//		_pop_num_h_ = POP_X_MN_NUM_H;
//		_pop_num_v_ = POP_X_MN_NUM_V;
	}
	else{
		_pth_ = PTH_Y;
//		_pop_num_h_ = POP_Y_MN_NUM_H;
//		_pop_num_v_ = POP_Y_MN_NUM_V;
	}
	
//	_pth_ = PTH_Y;																		//... 060307:temporal
	
	_pop_num_h_ = POP_MN_NUM_H;
	_pop_num_v_ = POP_MN_NUM_V + cmn_vga_det;
	if(mse_osd_x > (45 - _pop_num_h_))	mse_osd_x = 45 - _pop_num_h_;
	if(mse_osd_y > (_osd_lmt_v_-_pop_num_v_))	mse_osd_y = _osd_lmt_v_-_pop_num_v_;
	SetPopMain(_pth_,mse_osd_x,mse_osd_y,_pop_num_h_,_pop_num_v_);

	CreateBoxPopHL(_pth_,BOX_POP_MN_HL,mse_osd_x,mse_osd_y,_pop_num_h_);
	//... update:050706 Rev2.0
	if(mse_ch == 0xff)	wndw_pop_crnt_ch = 0;
	else	wndw_pop_crnt_ch = mse_ch&0x0f;
	
	SetCrntDvcCh(wndw_pop_crnt_ch,&wndw_crnt_dvc,&wndw_crnt_ch);

	b_cmn_wndw_pop = ON;
//	SetBit(cmn_flg,CMN_WNDW_POP);
}
//==================================================================================
void RemovePopSub(void)
{
//	U8 _t1_;
//	U8 _pth_;
	
//	if(b_mouse_status_path_popup == MOUSE_PATH_X)	_pth_ = PTH_X;
//	else	_pth_ = PTH_Y;

	if(b_mse_stts_pth_pop == MSE_PTH_X){
//	if(BitClear(mse_flg,MSE_STTS_PTH_POP)){
//		_pth_ = PTH_X;
//		WriteOSDClrX(MASTER,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h,wndw_pop_sub_num_v);
		DisableBox(PTH_X,BOX_POP_SB_HL);
		DisableBox(PTH_X,BOX_POP_SB);
		WriteOSDClrY(MASTER,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h,wndw_pop_sub_num_v);	//... 060307:temporal
	}
	else{
//		_pth_ = PTH_Y;
		DisableBox(PTH_Y,BOX_POP_SB_HL);
		DisableBox(PTH_Y,BOX_POP_SB);
		WriteOSDClrY(MASTER,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h,wndw_pop_sub_num_v);
	}

//	for(_t1_=0;_t1_<4;_t1_++)
//		DisableBox(PTH_Y,_t1_);//BOX_POP_SB_HL,BOX_POP_SB);
//	WriteOSDClr(PTH_Y,MASTER,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h,wndw_pop_sub_num_v);

	mse_osd_x_sub=45;//50;//
	mse_osd_y_sub=56;//30;//
}
//==================================================================================
void CreatePopSub(U8 _main_mn_pos_crnt)
{
	U16 _osd_lmt_v_ = cmn_bot<<1;

	U8 _osd_lmt_x_;
	U8 _pop_main_num_h_;
	U8 _pth_;

	_osd_lmt_v_ = (_osd_lmt_v_-OSD_DLY_V)/10;

	if(b_mse_stts_pth_pop == MSE_PTH_X){
//	if(BitClear(mse_flg,MSE_STTS_PTH_POP)){
		_pth_ = PTH_X;
//		_pop_main_num_h_ = POP_X_MN_NUM_H;
	}
	else{
		_pth_ = PTH_Y;
//		_pop_main_num_h_ = POP_Y_MN_NUM_H;
	}
	
//	_pth_ = PTH_Y;																		//... 060307:temporal
	
	_pop_main_num_h_ = POP_MN_NUM_H;
	_osd_lmt_x_ = 45 - (_pop_main_num_h_+wndw_pop_sub_num_h+1);

	mse_osd_y_sub = GetMseOSDY(reg_crsr_pos_y);

	if(mse_osd_x > _osd_lmt_x_)	mse_osd_x_sub = mse_osd_x-wndw_pop_sub_num_h-1;
	else	mse_osd_x_sub = mse_osd_x+_pop_main_num_h_+1;
	if(mse_osd_y_sub > (_osd_lmt_v_-wndw_pop_sub_num_v))	mse_osd_y_sub = _osd_lmt_v_-wndw_pop_sub_num_v;
	SetPopSub(_pth_,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h,wndw_pop_sub_num_v,_main_mn_pos_crnt);
	CreateBoxPopHL(_pth_,BOX_POP_SB_HL,mse_osd_x_sub,mse_osd_y_sub,wndw_pop_sub_num_h);
}
//==================================================================================
//==================================================================================

void RunPopProc(void)
{
	U8 _osd_pos_x_, _osd_pos_y_;
	U8 _pop_main_num_h_, _pop_main_num_v_;
	U8 _mn_crnt_pos_main_, _mn_crnt_pos_sub_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

//	if(b_mouse_status_path_popup == MOUSE_PATH_X){
//		_pop_main_num_h_ = POP_X_MN_NUM_H;
//		_pop_main_num_v_ = POP_X_MN_NUM_V;
//	}
//	else{
//		_pop_main_num_h_ = POP_Y_MN_NUM_H;
//		_pop_main_num_v_ = POP_Y_MN_NUM_V;
//	}
	_pop_main_num_h_ = POP_MN_NUM_H;
	_pop_main_num_v_ = POP_MN_NUM_V + cmn_vga_det;
	
	if(((_osd_pos_x_ >= mse_osd_x)&&(_osd_pos_x_ <= (mse_osd_x+_pop_main_num_h_))&&
		(_osd_pos_y_ >= mse_osd_y)&&(_osd_pos_y_ < (mse_osd_y+_pop_main_num_v_)))//){
		&&	(b_mse_stts_pth_pop == b_mse_stts_pth)){
//		&&	(((mse_flg&MSE_STTS_PTH_POP)>>1) == (mse_flg&MSE_STTS_PTH))){
		
		SetBoxPopHLPos(BOX_POP_MN_HL,mse_osd_x,_osd_pos_y_,_pop_main_num_h_);
		_mn_crnt_pos_main_ = _osd_pos_y_ - mse_osd_y;

		if(wndw_mn_pos_crnt != _mn_crnt_pos_main_){
			wndw_mn_pos_crnt = _mn_crnt_pos_main_;

			RemovePopSub();

			if(b_mse_stts_pth_pop == MSE_PTH_X){
//			if(BitClear(mse_flg,MSE_STTS_PTH_POP)){
				if(_mn_crnt_pos_main_ == POP_POS_DS){
					wndw_pop_sub_num_h = 6;
					wndw_pop_sub_num_v = 4;
					CreatePopSub(_mn_crnt_pos_main_);
				}
				else if(_mn_crnt_pos_main_ == POP_POS_MTN){
					wndw_pop_sub_num_h = 6;
					wndw_pop_sub_num_v = 3;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#if defined(__WTH_ADO__)
				else if(_mn_crnt_pos_main_ == POP_POS_ADO){
					wndw_pop_sub_num_h = 6;
					wndw_pop_sub_num_v = 3;//4;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#elif defined(__WTH_VGA__)
//				else if(_mn_crnt_pos_main_ == POP_POS_VGA){
				else if((_mn_crnt_pos_main_ == POP_POS_VGA)&&(cmn_vga_det)){
					wndw_pop_sub_num_h = 11;//15;
					wndw_pop_sub_num_v = 4;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#endif
				else{
					wndw_pop_sub_num_h = 0;
					wndw_pop_sub_num_v = 0;
				}
			}
			else{
				if(_mn_crnt_pos_main_ == POP_POS_RC){
					wndw_pop_sub_num_h = 6;
					wndw_pop_sub_num_v = 3;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#if defined(__WTH_ADO__)
				else if(_mn_crnt_pos_main_ == POP_POS_ADO){
					wndw_pop_sub_num_h = 6;
					wndw_pop_sub_num_v = 3;//4;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#elif defined(__WTH_VGA__)
//				else if(_mn_crnt_pos_main_ == POP_POS_VGA){
				else if((_mn_crnt_pos_main_ == POP_POS_VGA)&&(cmn_vga_det)){
					wndw_pop_sub_num_h = 11;//15;
					wndw_pop_sub_num_v = 4;
					CreatePopSub(_mn_crnt_pos_main_);
				}
#endif
				else{
					wndw_pop_sub_num_h = 0;
					wndw_pop_sub_num_v = 0;
				}
			}
		}		

//...		left button function
		if(b_mse_btn_dwn_lb == 1){
			b_mse_btn_dwn_lb = 0;
//		if(BitSet(mse_flg,MSE_BTN_DWN_LB)){
//			ClearBit(mse_flg,MSE_BTN_DWN_LB);
		}
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;

//			if((_mn_crnt_pos_main_ == POP_POS_MTN)||(_mn_crnt_pos_main_ == POP_POS_VGA)||(_mn_crnt_pos_main_ == POP_POS_ADO)){
			if(_mn_crnt_pos_main_ == POP_POS_MTN){
			}
#if defined(__WTH_ADO__)
			else if(_mn_crnt_pos_main_ == POP_POS_ADO){
				//... no operation
			}
#endif
//#if defined(__4CH__)
//			else if((_mn_crnt_pos_main_ == POP_POS_CH)&&(wndw_ds_sc_fmt == SC_FMT_4CH)&&(b_ds_wth_4pb)){
//				//... no operation
//			}
//#elif defined(__16CH__)
//			else if((_mn_crnt_pos_main_ == POP_POS_CH)&&(wndw_ds_sc_fmt == SC_FMT_16CH)&&(b_ds_wth_16pb)){
//				//... no operation
//			}
//#endif
			else{
//				if((b_mse_stts_pth_pop == MSE_PTH_Y)&&(_mn_crnt_pos_main_ == POP_POS_MTN)){
//					//... no operation
//				}
				if((_mn_crnt_pos_main_ == POP_POS_DS)&&(b_mse_stts_pth_pop == MSE_PTH_X)){
					//... no operation
				}
				else if((_mn_crnt_pos_main_ == POP_POS_RC)&&(b_mse_stts_pth_pop == MSE_PTH_Y)){
					//... no operation
				}
#if defined(__WTH_VGA__)
				else if((_mn_crnt_pos_main_ == POP_POS_VGA)&&(cmn_vga_det)){
					//... no operation
				}
#endif
				else{
					RemovePopMain();
					b_cmn_wndw_pop = OFF;
	//				ClearBit(cmn_flg,CMN_WNDW_POP);
		
					b_mse_stts_pth_mn = b_mse_stts_pth_pop;
	//				if(BitClear(mse_flg,MSE_STTS_PTH_POP))	ClearBit(mse_flg,MSE_STTS_PTH_MN);
	//				else	SetBit(mse_flg,MSE_STTS_PTH_MN);
					if(b_mse_stts_pth_mn == MSE_PTH_X){
	//				if(BitClear(mse_flg,MSE_STTS_PTH_MN)){

//						if(_mn_crnt_pos_main_ == POP_POS_CH)			CreateWndwCh(PTH_Y);
//						if(_mn_crnt_pos_main_ == POP_POS_ZM)		CreateWndwZm();
//						else if(_mn_crnt_pos_main_ == POP_POS_MTN)	CreateWndwMtn(PTH_Y);
						if(_mn_crnt_pos_main_ == POP_POS_RC){
							b_mse_stts_pth = MSE_PTH_Y;						//... change to record path
	
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa0,0xbb);
#if defined(__4CH__)
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa1,0x76);   //... s-video (y path)	//... 060705:temporal   
#elif defined(__16CH__)
//							WriteAsicByte(cmn_dvc,DVC_PG1,0xa1,0x67);   //... s-video (y path)
							WriteAsicByte(MASTER,DVC_PG1,0xa1,0x67);			//... 
							WriteAsicByte(SLAVE1,DVC_PG1,0xa1,0xff);			//... 
							WriteAsicByte(SLAVE2,DVC_PG1,0xa1,0xff);			//... 
							WriteAsicByte(SLAVE3,DVC_PG1,0xa1,0xff);			//... 
#endif
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa2,0xd7);	//... cvbs (y path)
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa3,0x10);	//... 656 output

							SetOSDNoMn();

							wndw_mn_pos_crnt = 30;
						}
//						else if(_mn_crnt_pos_main_ == POP_POS_RG)		CreateWndwRg(PTH_Y);
						else if(_mn_crnt_pos_main_ == (POP_POS_RG+cmn_vga_det))		CreateWndwRg(PTH_Y);
//						else if(_mn_crnt_pos_main_ == POP_POS_ABT)	;//CreateWndwAbt(PTH_Y);
		//				if(_mn_crnt_pos_main_ == POP_POS_RG)	CreateWndwRg(PTH_X);
					}
					else{
						if(_mn_crnt_pos_main_ == POP_POS_DS){
							b_mse_stts_pth = MSE_PTH_X;						//... change to display path

							WriteAsicByte(cmn_dvc,DVC_PG1,0xa0,0x66);
#if defined(__4CH__)
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa1,0x32);	//... s-video (x path)	//... 060705:temporal
#elif defined(__16CH__)
//							WriteAsicByte(cmn_dvc,DVC_PG1,0xa1,0x23);	//... s-video (x path)
							WriteAsicByte(MASTER,DVC_PG1,0xa1,0x23);			//... 
							WriteAsicByte(SLAVE1,DVC_PG1,0xa1,0xff);			//... 
							WriteAsicByte(SLAVE2,DVC_PG1,0xa1,0xff);			//... 
							WriteAsicByte(SLAVE3,DVC_PG1,0xa1,0xff);			//... 
#endif
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa2,0x97);	//... cvbs (x path)
							WriteAsicByte(cmn_dvc,DVC_PG1,0xa3,0x01);	//... 656 output

							SetOSDNoMn();

							wndw_mn_pos_crnt = 30;
						}
//						else if(_mn_crnt_pos_main_ == POP_POS_RG)			CreateWndwRg(PTH_Y);
						else if(_mn_crnt_pos_main_ == (POP_POS_RG+cmn_vga_det))			CreateWndwRg(PTH_Y);
//						else if(_mn_crnt_pos_main_ == POP_POS_ABT)	;//CreateWndwAbt(PTH_Y);
					}
				}
			}
		}
		if(b_mse_pshng_clk_lb == 1){
			b_mse_pshng_clk_lb = 0;
//		if(BitSet(mse_flg,MSE_PSHNG_CLK_LB)){
//			ClearBit(mse_flg,MSE_PSHNG_CLK_LB);
		}
		
//...		right button function
		if(b_mse_btn_dwn_rb == 1){
			b_mse_btn_dwn_rb = 0;
		}
		if(b_mse_clk_rb == 1){
			b_mse_clk_rb = 0;
		}
		if(b_mse_pshng_clk_rb == 1){
			b_mse_pshng_clk_rb = 0;
		}
	}
	else if(((_osd_pos_x_ >= mse_osd_x_sub)&&(_osd_pos_x_ <= (mse_osd_x_sub+wndw_pop_sub_num_h))&&
		(_osd_pos_y_ >= mse_osd_y_sub)&&(_osd_pos_y_ < (mse_osd_y_sub+wndw_pop_sub_num_v)))//){
		&&	(b_mse_stts_pth_pop == b_mse_stts_pth)){
//		&&	(((mse_flg&MSE_STTS_PTH_POP)>>1) == (mse_flg&MSE_STTS_PTH))){
		
		SetBoxPopHLPos(BOX_POP_SB_HL,mse_osd_x_sub,_osd_pos_y_,wndw_pop_sub_num_h);
		_mn_crnt_pos_sub_ = _osd_pos_y_ - mse_osd_y_sub;

//...		left button function
		if(b_mse_btn_dwn_lb == 1){
			b_mse_btn_dwn_lb = 0;
//		if(BitSet(mse_flg,MSE_BTN_DWN_LB)){
//			ClearBit(mse_flg,MSE_BTN_DWN_LB);
		}
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;
//		if(BitSet(mse_flg,MSE_CLK_LB)){
//			ClearBit(mse_flg,MSE_CLK_LB);

			_mn_crnt_pos_main_ = wndw_mn_pos_crnt&0x0f;
			wndw_mn_pos_crnt |= _mn_crnt_pos_sub_<<4;

			RemovePopSub();
			RemovePopMain();
			b_cmn_wndw_pop = OFF;
//			ClearBit(cmn_flg,CMN_WNDW_POP);

			b_mse_stts_pth_mn = b_mse_stts_pth_pop;
//			if(BitClear(mse_flg,MSE_STTS_PTH_POP))	ClearBit(mse_flg,MSE_STTS_PTH_MN);
//			else	SetBit(mse_flg,MSE_STTS_PTH_MN);
			if(b_mse_stts_pth_mn == MSE_PTH_X){
//			if(BitClear(mse_flg,MSE_STTS_PTH_MN)){

//				if(_mn_crnt_pos_main_ == POP_POS_SCRN_X)	CreateWndwScrnX();
				if(wndw_mn_pos_crnt == POP_POS_DS_SC)		CreateWndwDsSc(PTH_Y);
//				else if(wndw_mn_pos_crnt == POP_POS_DS_PB)	CreateWndwDsPb(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_DS_SR){
#if defined(__4CH__)
					if((wndw_ds_sc_fmt == SC_FMT_4CH)&&(b_ds_wth_4pb)){
						//... no operation
					}
#elif defined(__16CH__)
					if((wndw_ds_sc_fmt == SC_FMT_16CH)&&(b_ds_wth_16pb)){
						//... no operation
					}
#endif
					else	CreateWndwDsSr(PTH_Y);
				}
				else if(wndw_mn_pos_crnt == POP_POS_DS_IN)	CreateWndwDsIn(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_ZM)		CreateWndwZm();

				else if(wndw_mn_pos_crnt == POP_POS_MTN_MD)	CreateWndwMtnMd(PTH_Y);
//				else if(wndw_mn_pos_crnt == POP_POS_MTN_BD)	CreateWndwMtnBd(PTH_Y);
//				else if(wndw_mn_pos_crnt == POP_POS_MTN_ND)	CreateWndwMtnNd(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_MTN_BD)	CreateWndwMtnBdNd(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_MTN_ND)	CreateWndwMtnBdNd(PTH_Y);

#if defined(__WTH_ADO__)
				else if(wndw_mn_pos_crnt == POP_POS_ADO_GN)	CreateWndwAdoGn(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_ADO_LV)	CreateWndwAdoLv(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_ADO_RC)	CreateWndwAdoRc(PTH_Y);
//				else if(wndw_mn_pos_crnt == POP_POS_ADO_DT)	CreateWndwAdoDt(PTH_Y);
#elif defined(__WTH_VGA__)
				if((_mn_crnt_pos_main_ == POP_POS_VGA)&&(cmn_vga_det)){
					if(wndw_mn_pos_crnt == POP_POS_VGA_OR)	CreateWndwVGAOr(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_DI)	CreateWndwVGADi(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_IE)	CreateWndwVGAIe(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_NR)	CreateWndwVGANr(PTH_Y);
				}
#endif
			}
			else{
//				if(_mn_crnt_pos_main_ == POP_POS_SCRN_Y)	CreateWndwScrnY();
				if(wndw_mn_pos_crnt == POP_POS_RC_FM)			CreateWndwRcFm(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_RC_IN)	CreateWndwRcIn(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_RC_NR)	CreateWndwRcNr(PTH_Y);

#if defined(__WTH_ADO__)
				else if(wndw_mn_pos_crnt == POP_POS_ADO_GN)	CreateWndwAdoGn(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_ADO_LV)	CreateWndwAdoLv(PTH_Y);
				else if(wndw_mn_pos_crnt == POP_POS_ADO_RC)	CreateWndwAdoRc(PTH_Y);
//				else if(wndw_mn_pos_crnt == POP_POS_ADO_DT)	CreateWndwAdoDt(PTH_Y);
#elif defined(__WTH_VGA__)
				if((_mn_crnt_pos_main_ == POP_POS_VGA)&&(cmn_vga_det)){
					if(wndw_mn_pos_crnt == POP_POS_VGA_OR)	CreateWndwVGAOr(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_DI)	CreateWndwVGADi(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_IE)	CreateWndwVGAIe(PTH_Y);
					else if(wndw_mn_pos_crnt == POP_POS_VGA_NR)	CreateWndwVGANr(PTH_Y);
				}
#endif
			}
		}
		if(b_mse_pshng_clk_lb == 1){
			b_mse_pshng_clk_lb = 0;
//		if(BitSet(mse_flg,MSE_PSHNG_CLK_LB)){
//			ClearBit(mse_flg,MSE_PSHNG_CLK_LB);
		}
		
//...		right button function
		if(b_mse_btn_dwn_rb == 1){
			b_mse_btn_dwn_rb = 0;
		}
		if(b_mse_clk_rb == 1){
			b_mse_clk_rb = 0;
		}
		if(b_mse_pshng_clk_rb == 1){
			b_mse_pshng_clk_rb = 0;
		}
	}
	else{
//...		left button function
		if(b_mse_btn_dwn_lb == 1){
			b_mse_btn_dwn_lb = 0;
//		if(BitSet(mse_flg,MSE_BTN_DWN_LB)){
//			ClearBit(mse_flg,MSE_BTN_DWN_LB);
		}
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;
//		if(BitSet(mse_flg,MSE_CLK_LB)){
//			ClearBit(mse_flg,MSE_CLK_LB);

			RemovePopSub();
			RemovePopMain();

			SetOSDNoMn();

			wndw_mn_pos_crnt = 30;
			b_cmn_wndw_pop = OFF;
//			ClearBit(cmn_flg,CMN_WNDW_POP);
		}
		if(b_mse_pshng_clk_lb == 1){
			b_mse_pshng_clk_lb = 0;
//		if(BitSet(mse_flg,MSE_PSHNG_CLK_LB)){
//			ClearBit(mse_flg,MSE_PSHNG_CLK_LB);
		}

//...		right button function
		if(b_mse_btn_dwn_rb == 1){
			b_mse_btn_dwn_rb = 0;
//		if(BitSet(mse_flg,MSE_BTN_DWN_RB)){
//			ClearBit(mse_flg,MSE_BTN_DWN_RB);
		}
		if(b_mse_clk_rb == 1){
			b_mse_clk_rb = 0;
//		if(BitSet(mse_flg,MSE_CLK_RB)){
//			ClearBit(mse_flg,MSE_CLK_RB);

			RemovePopSub();
			CreatePopMain();
			wndw_mn_pos_crnt = 30;
		}
		if(b_mse_pshng_clk_rb == 1){
			b_mse_pshng_clk_rb = 0;
//		if(BitSet(mse_flg,MSE_PSHNG_CLK_RB)){
//			ClearBit(mse_flg,MSE_PSHNG_CLK_RB);
		}
	}
}

//==================================================================================

//				if(b_mse_btn_dwn_lb == 1){
//					b_mse_btn_dwn_lb = 0;
//				}
//				if(b_mse_clk_lb == 1){
//					b_mse_clk_lb = 0;
//				}
//				if(b_mse_pshng_clk_lb == 1){
//					b_mse_pshng_clk_lb = 0;
//				}
//				
//				
//				if(b_mse_btn_dwn_rb == 1){
//					b_mse_btn_dwn_rb = 0;
//				}
//				if(b_mse_clk_rb == 1){
//					b_mse_clk_rb = 0;
//				}
//				if(b_mse_pshng_clk_rb == 1){
//					b_mse_pshng_clk_rb = 0;
//				}
