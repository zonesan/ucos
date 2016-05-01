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
//						Zm Popup definition & variable description
//==================================================================================
int	b_zm_pop_en;
int	b_zm_pop_focs;


//==================================================================================
//						Zm Popup function description
//==================================================================================
void RemoveZmPop(void)
{
//	DisableBox(PTH_X,BOX_POPUP_MAIN_HL,BOX_POPUP_MAIN);
	DisableBox(PTH_X,BOX_POP_MN_HL);
	DisableBox(PTH_X,BOX_POP_MN);
	WriteOSDClr(PTH_Y,MASTER,mse_osd_x,mse_osd_y,ZM_POP_H,ZM_POP_V);
}
//==================================================================================
void CreateZmPop(void)
{
	U16 _indx_y_lmt_ = cmn_bot<<1;

	_indx_y_lmt_ = (_indx_y_lmt_-OSD_DLY_V)/10;

	RemoveZmPop();

	mse_osd_x = GetMseOSDX(reg_crsr_pos_x);
	mse_osd_y = GetMseOSDY(reg_crsr_pos_y);

	if(mse_osd_x > (45 - ZM_POP_H))	mse_osd_x = 45 - ZM_POP_H;
	if(mse_osd_y > (_indx_y_lmt_-ZM_POP_V))	mse_osd_y = _indx_y_lmt_-ZM_POP_V;

	CreateBoxPopFrame(PTH_X,BOX_POP_MN,mse_osd_x,mse_osd_y,ZM_POP_H,ZM_POP_V);
	WriteOSDStr(PTH_Y,MASTER,mse_osd_x,mse_osd_y,0x00,str_wndw_cmn_clos);
	CreateBoxPopHL(PTH_X,BOX_POP_MN_HL,mse_osd_x,mse_osd_y,ZM_POP_H);

	b_zm_pop_en = ON;
	b_zm_pop_focs = ON;
}
//==================================================================================
void RunZmPop(void)
{
//	U8	_osd_pos_x_, _osd_pos_y_;
//
//	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
//	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);
	U8	_osd_pos_x_, _osd_pos_y_;

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);


	if(((_osd_pos_x_ >= mse_osd_x)&&(_osd_pos_x_ < (mse_osd_x+ZM_POP_H)))&&(_osd_pos_y_ == mse_osd_y)){
		if(b_zm_pop_focs == OFF){
			b_zm_pop_focs = ON;
			
//			EnableBox(PTH_X,BOX_POPUP_MAIN_HL,BOX_POPUP_MAIN_HL);
			SetAsicFlgType(MASTER,DVC_PG2,0x20+BOX_POP_MN_HL*6,0x07,0x05);	//... MIX=1, BND=0, AREA=1
		}

		if(b_mse_btn_dwn_lb == 1){
			b_mse_btn_dwn_lb = 0;
		}
		if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;

			SetAsicFlgType(cmn_lst_slvr_x,DVC_PG1,0x0c,BIT7|BIT3|BIT2,0x00);
			SetAsicFlgType(MASTER,DVC_PG1,0x06,BIT2,BIT2);
#if	defined(__16CH__)
			SetAsicFlgType(SLAVE1,DVC_PG1,0x06,BIT2,BIT2);
			SetAsicFlgType(SLAVE2,DVC_PG1,0x06,BIT2,BIT2);
			SetAsicFlgType(SLAVE3,DVC_PG1,0x06,BIT2,BIT2);
#endif
			RemoveZmPop();

			b_zm_pop_en = OFF;
//			b_status_win = OFF;
//			global_menu_crnt_pos = 30;
			b_cmn_wndw_mn = OFF;
			wndw_mn_pos_crnt = 30;
		}
		if(b_mse_clk_rb == 1)	b_mse_clk_rb = 0;
		if(b_mse_pshng_clk_lb == 1)	b_mse_pshng_clk_lb = 0;
	}
	else{
		if(b_zm_pop_focs == ON){
			b_zm_pop_focs = OFF;
//			DisableBox(PTH_X,BOX_POPUP_MAIN_HL,BOX_POPUP_MAIN_HL);
			DisableBox(PTH_X,BOX_POP_MN_HL);
		}
		if(b_mse_btn_dwn_lb == 1)	b_mse_btn_dwn_lb = 0;
		if(b_mse_btn_dwn_rb == 1)	b_mse_btn_dwn_rb = 0;
		if(b_mse_clk_lb == 1){
			b_mse_clk_lb = 0;
			
			RemoveZmPop();

			SetOSDNoMn();

			b_zm_pop_en = OFF;
		}
		if(b_mse_clk_rb == 1){					// create popup main
			b_mse_clk_rb = 0;
			
			CreateZmPop();
		}
		if(b_mse_pshng_clk_lb == 1)	b_mse_pshng_clk_lb = 0;
	}
}

//==================================================================================

