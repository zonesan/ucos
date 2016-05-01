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
#include        "wndw_rc_fmt_qu.h"


//==================================================================================
//						Register window definition & variable description
//==================================================================================
U8 wndw_rg_osd_pos_x=50;
U8 wndw_rg_osd_pos_y=WNDW_RG_CTRL_Y;// 31;// 

#if	defined(__WNDW_RG_EV__)
U8 wndw_rg_wrt_en = 0x3f;//0xff;
U8 wndw_rg_rd_en = 0x3f;//0xff;
U8 wndw_rg_av_dvc[WNDW_RG_NUM] = {MASTER,MASTER,MASTER,MASTER,MASTER,MASTER};//,MASTER,MASTER};
U8 wndw_rg_av_pg[WNDW_RG_NUM] = {0,0,0,0,0,0};//,0,0};
U8 wndw_rg_av_addr[WNDW_RG_NUM] = {0,0,0,0,0,0};//,0,0};
U8 wndw_rg_av_dat[WNDW_RG_NUM] = {0,0,0,0,0,0};//,0,0};
#elif	defined(__WNDW_RG_PRO__)
U8 wndw_rg_rd_en = 0x0f;
U8 wndw_rg_av_dvc[WNDW_RG_NUM] = {MASTER,MASTER,MASTER,MASTER};
U8 wndw_rg_av_pg[WNDW_RG_NUM] = {0,0,0,0};
U8 wndw_rg_av_addr[WNDW_RG_NUM] = {0,0,0,0};
U8 wndw_rg_av_dat[WNDW_RG_NUM] = {0,0,0,0};
#endif

U8 wndw_rg_bit_ptr_l = 7;
U8 wndw_rg_bit_ptr_r = 0;


//==================================================================================
//						Register window function description
//==================================================================================
void	SetWndwRgOSDDvc(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dvc)
{
	U8 _t1_=0;

	_pos_x = 0;					//... temporal
#if	defined(__4CH__)
	_t1_ = OSD_MIX;
#elif	defined(__16CH__)
//	if((_pos_x == WNDW_RG_DVC_X)||(_pos_x == WNDW_RG_DVC_X+1))	_t1_ = OSD_MIX;//1;//
//	else	_t1_ = 0;//|OSD_MIX;
#endif

	if(BitSet(_pth,PTH_X)){
		if(_av_dvc[_pos_y] == MASTER)		WriteOSDStrX(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_ms);
		else if(_av_dvc[_pos_y] == SLAVE1)	WriteOSDStrX(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s1);
		else if(_av_dvc[_pos_y] == SLAVE2)	WriteOSDStrX(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s2);
		else if(_av_dvc[_pos_y] == SLAVE3)	WriteOSDStrX(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s3);
	}
	if(BitSet(_pth,PTH_Y)){
		if(_av_dvc[_pos_y] == MASTER)		WriteOSDStrY(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_ms);
		else if(_av_dvc[_pos_y] == SLAVE1)	WriteOSDStrY(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s1);
		else if(_av_dvc[_pos_y] == SLAVE2)	WriteOSDStrY(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s2);
		else if(_av_dvc[_pos_y] == SLAVE3)	WriteOSDStrY(MASTER,WNDW_RG_DVC_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,str_wndw_rg_s3);
	}
}
//==================================================================================
void	SetWndwRgOSDPg(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_pg)
{
	U8 _t1_=0;

	_pos_x = 0;			//... temporal
//	if(WNDW_RG_PAGE_X != _pos_x)	_t1_ = 0;//|OSD_MIX;
//	else	_t1_ = OSD_MIX;//1;//

	if(BitSet(_pth,PTH_X)){
		WriteOSDFntX(MASTER,WNDW_RG_PAGE_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,_av_pg[_pos_y]+0x10);//0x30);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDFntY(MASTER,WNDW_RG_PAGE_X,WNDW_RG_CTRL_Y+_pos_y,_t1_,_av_pg[_pos_y]+0x10);//0x30);
	}
}
//==================================================================================
void	SetWndwRgOSDAddr(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_addr)
{
	U8 _t1_=0, _t2_, _t3_;
	
	for(_t2_=0;_t2_<2;_t2_++){
		if(_t2_ == 0)	_t3_ = (_av_addr[_pos_y]&0xf0)>>4;
		else	_t3_ = _av_addr[_pos_y]&0x0f;

		if(_t3_ > 9)	_t3_ += 0x07;

		_pos_x = 0;			//... temporal
//		if((WNDW_RG_ADDR_X+_t2_) != _pos_x)	_t1_ = 0;//|OSD_MIX;
//		else	_t1_ = OSD_MIX;//1;//

		if(BitSet(_pth,PTH_X)){
			WriteOSDFntX(MASTER,WNDW_RG_ADDR_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
		if(BitSet(_pth,PTH_Y)){
			WriteOSDFntY(MASTER,WNDW_RG_ADDR_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
	}
}
//==================================================================================
void	SetWndwRgOSDDatBin(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat)
{
	U8 _t1_=0, _t2_, _t3_;
	
	for(_t2_=0;_t2_<8;_t2_++){
	
		_pos_x = 0;			//... temporal
//		if((WNDW_RG_DATA_B_X+_t2_) != _pos_x)	_t1_ = 0;//|OSD_MIX;
//		else	_t1_ = OSD_MIX;//1;//

		if(BitSet(_av_dat[_pos_y],BIT7>>_t2_))	_t3_ = 1;
		else	_t3_ = 0;

		if(BitSet(_pth,PTH_X)){
			WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
		if(BitSet(_pth,PTH_Y)){
			WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
	}
}
//==================================================================================
void	SetWndwRgOSDDatHex(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat)
{
	U8 _t1_=0, _t2_, _t3_;
	
	for(_t2_=0;_t2_<2;_t2_++){

		_pos_x = 0;			//... temporal
//		if((WNDW_RG_DATA_H_X+_t2_) != _pos_x)	_t1_ = 0;//|OSD_MIX;
//		else	_t1_ = OSD_MIX;//1;//

		if(_t2_ == 0)	_t3_ = _av_dat[_pos_y]>>4;
		else	_t3_ = _av_dat[_pos_y]&0x0f;

		if(_t3_ > 9)	_t3_ += 0x07;

		if(BitSet(_pth,PTH_X)){
			WriteOSDFntX(MASTER,WNDW_RG_DATA_H_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
		if(BitSet(_pth,PTH_Y)){
			WriteOSDFntY(MASTER,WNDW_RG_DATA_H_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
		}
	}
}
//==================================================================================
void	SetWndwRgOSDDat(U8 _pth, U8 _pos_x, U8 _pos_y, U8 *_av_dat)
{
//	for(_t2_=0;_t2_<8;_t2_++){
//		if((WNDW_RG_DATA_B_X+_t2_) != _pos_x)	_t1_ = 0;//|OSD_MIX;
//		else	_t1_ = OSD_MIX;//1;//
////		IfBitSet(_av_dat[_pos_y],BIT7>>_t2_)	_t3_ = 1;
//		if(BitSet(_av_dat[_pos_y],BIT7>>_t2_))	_t3_ = 1;
//		else	_t3_ = 0;
////		WriteAsic1Font(_pth,WNDW_RG_DATA_B_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
//		if(BitSet(_pth,PTH_X)){
//			WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
//		}
//		if(BitSet(_pth,PTH_Y)){
//			WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
//		}
//	}
//	for(_t2_=0;_t2_<2;_t2_++){
//		if((WNDW_RG_DATA_H_X+_t2_) != _pos_x)	_t1_ = 0;//|OSD_MIX;
//		else	_t1_ = OSD_MIX;//1;//
//		if(_t2_ == 0)	_t3_ = _av_dat[_pos_y]>>4;
//		else	_t3_ = _av_dat[_pos_y]&0x0f;
//
//		if(_t3_ > 9)	_t3_ += 0x07;
////		WriteAsic1Font(_pth,WNDW_RG_DATA_H_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_);
//		if(BitSet(_pth,PTH_X)){
//			WriteOSDFntX(MASTER,WNDW_RG_DATA_H_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
//		}
//		if(BitSet(_pth,PTH_Y)){
//			WriteOSDFntY(MASTER,WNDW_RG_DATA_H_X+_t2_,WNDW_RG_CTRL_Y+_pos_y,_t1_,_t3_+0x10);//0x30);
//		}
//	}
	SetWndwRgOSDDatBin(_pth,_pos_x,_pos_y,_av_dat);
	SetWndwRgOSDDatHex(_pth,_pos_x,_pos_y,_av_dat);
}
//==================================================================================
#if	defined(__WNDW_RG_EV__)
void	SetWndwRgOSDWrt(U8 _pth, U8 _pos_y)
{
	U8 _t1_;
	
	if(BitSet(wndw_rg_wrt_en,BIT0<<_pos_y))	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;

	if(BitSet(_pth,PTH_X)){
		WriteOSDFntX(MASTER,WNDW_RG_WRT_X,WNDW_RG_CTRL_Y+_pos_y,0x00,_t1_);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDFntY(MASTER,WNDW_RG_WRT_X,WNDW_RG_CTRL_Y+_pos_y,0x00,_t1_);
	}
}
#endif
//==================================================================================
void	SetWndwRgOSDRd(U8 _pth, U8 _pos_y)
{
	U8 _t1_;
	
	if(BitSet(wndw_rg_rd_en,BIT0<<_pos_y))	_t1_ = OSD_CHK_BOX_CHK;
	else	_t1_ = OSD_CHK_BOX_BLK;

	if(BitSet(_pth,PTH_X)){
		WriteOSDFntX(MASTER,WNDW_RG_READ_X,WNDW_RG_CTRL_Y+_pos_y,0x00,_t1_);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDFntY(MASTER,WNDW_RG_READ_X,WNDW_RG_CTRL_Y+_pos_y,0x00,_t1_);
	}
}
//==================================================================================
void	CreateWndwRg(U8 _pth)
{
	U8 _t1_;//, _t2_;

	b_cmn_wndw_mn = ON;
//	SetBit(cmn_flg,CMN_WNDW_MN);

//	ClearOSD(_pth,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);
	if(BitSet(_pth,PTH_X))
		WriteOSDClrX(MASTER,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);
	if(BitSet(_pth,PTH_Y))
		WriteOSDClrY(MASTER,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);

	SetOSDWndw(_pth,OSD_WNDW_TYPE_MAIN,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H,str_wndw_ttl_reg_ctrl);

#if	defined(__WNDW_RG_EV__)
	if(BitSet(_pth,PTH_X)){
		WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_l,WNDW_RG_RNG_Y,OSD_MIX,OSD_ARW_D_SML);
		WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_r,WNDW_RG_RNG_Y,OSD_MIX,OSD_ARW_D_SML);
		WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_RNG_Y,OSD_MIX,'A'-0x20);
		WriteOSDFntX(MASTER,WNDW_RG_WRT_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_W_UPPER);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_l,WNDW_RG_RNG_Y,OSD_MIX,OSD_ARW_D_SML);
		WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_r,WNDW_RG_RNG_Y,OSD_MIX,OSD_ARW_D_SML);
		WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_RNG_Y,OSD_MIX,'A'-0x20);
		WriteOSDFntY(MASTER,WNDW_RG_WRT_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_W_UPPER);
	}
#elif	defined(__WNDW_RG_PRO__)
	if(BitSet(_pth,PTH_X)){
		WriteOSDStrX(MASTER,WNDW_RG_DATA_B_X+3,WNDW_RG_RNG_Y,OSD_MIX,str_wndw_rg_radx);
		WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_W_UPPER);
	}
	if(BitSet(_pth,PTH_Y)){
		WriteOSDStrY(MASTER,WNDW_RG_DATA_B_X+3,WNDW_RG_RNG_Y,OSD_MIX,str_wndw_rg_radx);
		WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_W_UPPER);
	}
#endif
	if(BitSet(_pth,PTH_X))
		WriteOSDFntX(MASTER,WNDW_RG_READ_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_R_UPPER);
	if(BitSet(_pth,PTH_Y))
		WriteOSDFntY(MASTER,WNDW_RG_READ_X,WNDW_RG_RNG_Y,OSD_MIX,OSD_R_UPPER);

	for(_t1_=0;_t1_<WNDW_RG_NUM;_t1_++){
		SetWndwRgOSDDvc(_pth,50,_t1_,wndw_rg_av_dvc);
		SetWndwRgOSDPg(_pth,50,_t1_,wndw_rg_av_pg);
		SetWndwRgOSDAddr(_pth,50,_t1_,wndw_rg_av_addr);

		if(BitSet(_pth,PTH_X))
			WriteOSDFntX(MASTER,WNDW_RG_PAGE_X+1,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_X_SMALL);
		if(BitSet(_pth,PTH_Y))
			WriteOSDFntY(MASTER,WNDW_RG_PAGE_X+1,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_X_SMALL);
		wndw_rg_av_dat[_t1_] = ReadAsicByte(wndw_rg_av_dvc[_t1_],wndw_rg_av_pg[_t1_],wndw_rg_av_addr[_t1_]);

		SetWndwRgOSDDat(_pth,50,_t1_,wndw_rg_av_dat);
		if(BitSet(_pth,PTH_X)){
			WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+8,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_DD);
			WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
		}
		if(BitSet(_pth,PTH_Y)){
			WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+8,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_DD);
			WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
		}
#if	defined(__WNDW_RG_EV__)
		if(BitSet(_pth,PTH_X)){
			WriteOSDFntX(MASTER,WNDW_RG_APLY_I_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_PLUS_SML);
			WriteOSDFntX(MASTER,WNDW_RG_APLY_D_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_MINUS_SML);
		}
		if(BitSet(_pth,PTH_Y)){
			WriteOSDFntY(MASTER,WNDW_RG_APLY_I_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_PLUS_SML);
			WriteOSDFntY(MASTER,WNDW_RG_APLY_D_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_MINUS_SML);
		}
		SetWndwRgOSDWrt(_pth,_t1_);
#endif

		SetWndwRgOSDRd(_pth,_t1_);
	}

//	mouse_flag_on_osd = 0x00;
	mse_flg_on_osd = 0x00;
}
//==================================================================================
void	RunWndwRgInc(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	_osd_pos_y -= WNDW_RG_CTRL_Y;

#if	defined(__4CH__)
	if((_osd_pos_x == WNDW_RG_PAGE_X)||(_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){
		if(_osd_pos_x == WNDW_RG_PAGE_X){
#elif	defined(__16CH__)
	if((_osd_pos_x == WNDW_RG_DVC_X)||(_osd_pos_x == WNDW_RG_DVC_X+1)||
		(_osd_pos_x == WNDW_RG_PAGE_X)||(_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){

		if((_osd_pos_x == WNDW_RG_DVC_X)||(_osd_pos_x == WNDW_RG_DVC_X+1)){
			if(wndw_rg_av_dvc[_osd_pos_y] > MASTER)	wndw_rg_av_dvc[_osd_pos_y] >>= 2;
			else	wndw_rg_av_dvc[_osd_pos_y] = SLAVE3;
			SetWndwRgOSDDvc(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dvc);
		}
		else if(_osd_pos_x == WNDW_RG_PAGE_X){
#endif

			if(wndw_rg_av_pg[_osd_pos_y] < 2)	wndw_rg_av_pg[_osd_pos_y]++;
			else	wndw_rg_av_pg[_osd_pos_y] = 0;
			SetWndwRgOSDPg(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_pg);
		}
		else if((_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){
			if(_osd_pos_x == WNDW_RG_ADDR_X){
				if((wndw_rg_av_addr[_osd_pos_y]&0xf0) < 0xf0)	wndw_rg_av_addr[_osd_pos_y] += 0x10;
				else	wndw_rg_av_addr[_osd_pos_y] &= 0x0f;
			}
			else{
				if((wndw_rg_av_addr[_osd_pos_y]&0x0f) < 0x0f)	wndw_rg_av_addr[_osd_pos_y]++;
				else	wndw_rg_av_addr[_osd_pos_y] &= 0xf0;
			}
			SetWndwRgOSDAddr(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_addr);
		}
		wndw_rg_av_dat[_osd_pos_y] = ReadAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y]);
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
	else if((_osd_pos_x >= WNDW_RG_DATA_B_X)&&(_osd_pos_x < (WNDW_RG_DATA_B_X+8))){
		wndw_rg_av_dat[_osd_pos_y] ^= BIT7>>( _osd_pos_x-WNDW_RG_DATA_B_X);
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
	else if((_osd_pos_x >= WNDW_RG_DATA_H_X)&&(_osd_pos_x < (WNDW_RG_DATA_H_X+2))){
		if(_osd_pos_x == WNDW_RG_DATA_H_X){
			if((wndw_rg_av_dat[_osd_pos_y]&0xf0) < 0xf0)	wndw_rg_av_dat[_osd_pos_y] += 0x10;
			else	wndw_rg_av_dat[_osd_pos_y] &= 0x0f;
		}
		else{
			if((wndw_rg_av_dat[_osd_pos_y]&0x0f) < 0x0f)	wndw_rg_av_dat[_osd_pos_y]++;
			else	wndw_rg_av_dat[_osd_pos_y] &= 0xf0;
		}
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
}
//==================================================================================
void	RunWndwRgDec(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	_osd_pos_y -= WNDW_RG_CTRL_Y;

#if	defined(__4CH__)
	if((_osd_pos_x == WNDW_RG_PAGE_X)||(_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){
		if(_osd_pos_x == WNDW_RG_PAGE_X){
#elif	defined(__16CH__)
	if((_osd_pos_x == WNDW_RG_DVC_X)||(_osd_pos_x == WNDW_RG_DVC_X+1)||
		(_osd_pos_x == WNDW_RG_PAGE_X)||(_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){

		if((_osd_pos_x == WNDW_RG_DVC_X)||(_osd_pos_x == WNDW_RG_DVC_X+1)){
			if(wndw_rg_av_dvc[_osd_pos_y] < SLAVE3)	wndw_rg_av_dvc[_osd_pos_y] <<= 2;
			else	wndw_rg_av_dvc[_osd_pos_y] = MASTER;
			SetWndwRgOSDDvc(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dvc);
		}
		else if(_osd_pos_x == WNDW_RG_PAGE_X){
#endif

			if(wndw_rg_av_pg[_osd_pos_y] > 0)	wndw_rg_av_pg[_osd_pos_y]--;
			else	wndw_rg_av_pg[_osd_pos_y] = 2;
			SetWndwRgOSDPg(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_pg);
		}
		else if((_osd_pos_x == WNDW_RG_ADDR_X)||(_osd_pos_x == (WNDW_RG_ADDR_X+1))){
			if(_osd_pos_x == WNDW_RG_ADDR_X){
				if((wndw_rg_av_addr[_osd_pos_y]&0xf0) > 0)	wndw_rg_av_addr[_osd_pos_y] -= 0x10;
				else	wndw_rg_av_addr[_osd_pos_y] |= 0xf0;
			}
			else{
				if((wndw_rg_av_addr[_osd_pos_y]&0x0f) > 0)	wndw_rg_av_addr[_osd_pos_y]--;
				else	wndw_rg_av_addr[_osd_pos_y] |= 0x0f;
			}
			SetWndwRgOSDAddr(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_addr);
		}
		wndw_rg_av_dat[_osd_pos_y] = ReadAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y]);
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
	else if((_osd_pos_x >= WNDW_RG_DATA_B_X)&&(_osd_pos_x < (WNDW_RG_DATA_B_X+8))){
		wndw_rg_av_dat[_osd_pos_y] ^= BIT7>>(_osd_pos_x-WNDW_RG_DATA_B_X);;
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
	else if((_osd_pos_x >= WNDW_RG_DATA_H_X)&&(_osd_pos_x < (WNDW_RG_DATA_H_X+2))){
		if(_osd_pos_x == WNDW_RG_DATA_H_X){
			if((wndw_rg_av_dat[_osd_pos_y]&0xf0) > 0)	wndw_rg_av_dat[_osd_pos_y] -= 0x10;
			else	wndw_rg_av_dat[_osd_pos_y] |= 0xf0;
		}
		else{
			if((wndw_rg_av_dat[_osd_pos_y]&0x0f) > 0)	wndw_rg_av_dat[_osd_pos_y]--;
			else	wndw_rg_av_dat[_osd_pos_y] |= 0x0f;
		}
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
}
//==================================================================================
#if	defined(__WNDW_RG_EV__)
void	SetWndwRgRngData(U8 _osd_pos_x, U8 _osd_pos_y)
{
	U8 _wdth_, _ofst_;
	U8 _t1_;

	if(wndw_rg_bit_ptr_l > wndw_rg_bit_ptr_r){
		_wdth_ = (0xff>>(7-wndw_rg_bit_ptr_l))&(0xff<<wndw_rg_bit_ptr_r);
		_ofst_ = wndw_rg_bit_ptr_r;
	}
	else{
		_wdth_ = (0xff>>(7-wndw_rg_bit_ptr_r))&(0xff<<wndw_rg_bit_ptr_l);
		_ofst_ = wndw_rg_bit_ptr_l;
	}
	_t1_ = (wndw_rg_av_dat[_osd_pos_y] & _wdth_)>>_ofst_;

	if(_osd_pos_x == WNDW_RG_APLY_I_X){
		if(_t1_ < (_wdth_>>_ofst_))		_t1_++;
		else	_t1_ = 0;
	}
	else if(_osd_pos_x == WNDW_RG_APLY_D_X){
		if(_t1_ > 0)	_t1_--;
		else	_t1_ = (_wdth_>>_ofst_);
	}
	wndw_rg_av_dat[_osd_pos_y] = (wndw_rg_av_dat[_osd_pos_y] & ~_wdth_)|(_t1_<<_ofst_);
}
#endif
//==================================================================================
void	RunWndwRgAply(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
#if	defined(__WNDW_RG_EV__)
	if((_osd_pos_x == WNDW_RG_APLY_I_X)||(_osd_pos_x == WNDW_RG_APLY_W_X)||(_osd_pos_x == WNDW_RG_APLY_D_X)){
		_osd_pos_y -= WNDW_RG_CTRL_Y;

		if(_osd_pos_x == WNDW_RG_APLY_W_X){
//			IfBitSet(wndw_rg_wrt_en,BIT0<<_osd_pos_y){
			if(BitSet(wndw_rg_wrt_en,BIT0<<_osd_pos_y)){
				WriteAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y],wndw_rg_av_dat[_osd_pos_y]);
			}
//			IfBitSet(wndw_rg_rd_en,BIT0<<_osd_pos_y){
			if(BitSet(wndw_rg_rd_en,BIT0<<_osd_pos_y)){
				wndw_rg_av_dat[_osd_pos_y] = ReadAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y]);
			}
			SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
		}
		else{
			SetWndwRgRngData(_osd_pos_x,_osd_pos_y);
			WriteAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y],wndw_rg_av_dat[_osd_pos_y]);
			SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
		}
	}
#elif	defined(__WNDW_RG_PRO__)
	if(_osd_pos_x == WNDW_RG_APLY_W_X){
		_osd_pos_y -= WNDW_RG_CTRL_Y;
		
		WriteAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y],wndw_rg_av_dat[_osd_pos_y]);

//		IfBitSet(wndw_rg_rd_en,BIT0<<_osd_pos_y){
		if(BitSet(wndw_rg_rd_en,BIT0<<_osd_pos_y)){
			wndw_rg_av_dat[_osd_pos_y] = ReadAsicByte(wndw_rg_av_dvc[_osd_pos_y],wndw_rg_av_pg[_osd_pos_y],wndw_rg_av_addr[_osd_pos_y]);
		}
		SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);
	}
#endif
}
//==================================================================================
/*
void	SetWndwRgOSD(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
{
	SetWndwRgOSDDvc(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dvc);//,_osd_pos_y_-WNDW_RG_CTRL_Y);//,wndw_rg_osd_pos_y);//
	SetWndwRgOSDPg(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_pg);//,_osd_pos_y_-WNDW_RG_CTRL_Y);//,wndw_rg_osd_pos_y);//
	SetWndwRgOSDAddr(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_addr);//,_osd_pos_y_-WNDW_RG_CTRL_Y);//,wndw_rg_osd_pos_y);//
	SetWndwRgOSDDat(_pth,_osd_pos_x,_osd_pos_y,wndw_rg_av_dat);//,_osd_pos_y_-WNDW_RG_CTRL_Y);//,wndw_rg_osd_pos_y);//
}
//	*/
//==================================================================================
void	RunWndwRg(U8 _pth)
{
	U8	_osd_pos_x_, _osd_pos_y_;
//#if	defined(__APLY_ALL__)
	U8 _t1_;
//#endif
#if	defined(__WNDW_RG_EV__)
//	U8 _t2_;
#endif

	_osd_pos_x_ = GetMseOSDX(reg_crsr_pos_x);
	_osd_pos_y_ = GetMseOSDY(reg_crsr_pos_y);

//	if((_osd_pos_y_ >= WNDW_RG_CTRL_Y)&&(_osd_pos_y_ < (WNDW_RG_CTRL_Y+WNDW_RG_NUM))){
//		if(_osd_pos_y_ != wndw_rg_osd_pos_y){
//			SetWndwRgOSD(_pth,50,wndw_rg_osd_pos_y-WNDW_RG_CTRL_Y);
//			wndw_rg_osd_pos_y = _osd_pos_y_;
//			SetWndwRgOSD(_pth,_osd_pos_x_,_osd_pos_y_-WNDW_RG_CTRL_Y);
//		}
//		if((_osd_pos_x_ >= WNDW_RG_DVC_X)&&(_osd_pos_x_ < (WNDW_RG_DATA_H_X+2))){
//			if(_osd_pos_x_ != wndw_rg_osd_pos_x){
//				wndw_rg_osd_pos_x = _osd_pos_x_;
//
//				SetWndwRgOSD(_pth,_osd_pos_x_,_osd_pos_y_-WNDW_RG_CTRL_Y);
//			}
//		}
//	}

#if	defined(__WNDW_RG_EV__)
	for(_t1_=0;_t1_<8;_t1_++){
		if(BitClear(wndw_rg_wrt_en,BIT0<<_t1_)&&BitSet(wndw_rg_rd_en,BIT0<<_t1_)){
			wndw_rg_av_dat[_t1_] = ReadAsicByte(wndw_rg_av_dvc[_t1_],wndw_rg_av_pg[_t1_],wndw_rg_av_addr[_t1_]);
//			SetWndwRgOSDDat(_pth,_osd_pos_x_,_t1_,wndw_rg_av_dat);
			SetWndwRgOSDDatHex(_pth,_osd_pos_x_,_t1_,wndw_rg_av_dat);
		}
	}


//	for(_t1_=0;_t1_<WNDW_RG_NUM;_t1_++){
//		if(((_osd_pos_x_ == WNDW_RG_APLY_I_X)||(_osd_pos_x_ == WNDW_RG_APLY_W_X)||(_osd_pos_x_ == WNDW_RG_APLY_D_X))&&
//			(_osd_pos_y_ == (WNDW_RG_CTRL_Y+_t1_))){
////			IfBitClear(mouse_flag_on_osd,BIT0<<_t1_){
//			if(BitClear(mse_flg_on_osd,BIT0<<_t1_)){
//				SetBit(mse_flg_on_osd,BIT0<<_t1_);
//				
//				if(_osd_pos_x_ == WNDW_RG_APLY_I_X)	_t2_ = OSD_PLUS_BIG;
//				else if(_osd_pos_x_ == WNDW_RG_APLY_W_X)	_t2_ = OSD_W_UPPER;
//				else	_t2_ = OSD_MINUS_BIG;
////				WriteAsic1Font(_pth,_osd_pos_x_,WNDW_RG_CTRL_Y+_t1_,OSD_MIX|1,_t2_);
//				if(BitSet(_pth,PTH_X))
//					WriteOSDFntX(MASTER,_osd_pos_x_,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,_t2_);
//				if(BitSet(_pth,PTH_Y))
//					WriteOSDFntY(MASTER,_osd_pos_x_,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,_t2_);
//			}
//		}
//		else{
////			IfBitSet(mouse_flag_on_osd,BIT0<<_t1_){
//			if(BitSet(mse_flg_on_osd,BIT0<<_t1_)){
//				ClearBit(mse_flg_on_osd,BIT0<<_t1_);
//
//				if(BitSet(_pth,PTH_X)){
//					WriteOSDFntX(MASTER,WNDW_RG_APLY_I_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_PLUS_SML);
//					WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
//					WriteOSDFntX(MASTER,WNDW_RG_APLY_D_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_MINUS_SML);
//				}
//				if(BitSet(_pth,PTH_Y)){
//					WriteOSDFntY(MASTER,WNDW_RG_APLY_I_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_PLUS_SML);
//					WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
//					WriteOSDFntY(MASTER,WNDW_RG_APLY_D_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_MINUS_SML);
//				}
//			}
//		}
//	}
#elif	defined(__WNDW_RG_PRO__)
//	for(_t1_=0;_t1_<WNDW_RG_NUM;_t1_++){
//		if((_osd_pos_x_ == WNDW_RG_APLY_W_X)&&(_osd_pos_y_ == (WNDW_RG_CTRL_Y+_t1_))){
////			IfBitClear(mouse_flag_on_osd,BIT0<<_t1_){
//			if(BitClear(mse_flg_on_osd,BIT0<<_t1_)){
//				SetBit(mse_flg_on_osd,BIT0<<_t1_);
//				
////				WriteAsic1Font(_pth,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,OSD_MIX|1,OSD_W_UPPER);
//				if(BitSet(_pth,PTH_X))
//					WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_W_UPPER);
//				if(BitSet(_pth,PTH_Y))
//					WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,OSD_MIX,OSD_W_UPPER);
//			}
//		}
//		else{
////			IfBitSet(mouse_flag_on_osd,BIT0<<_t1_){
//			if(BitSet(mse_flg_on_osd,BIT0<<_t1_)){
//				ClearBit(mse_flg_on_osd,BIT0<<_t1_));
//
////				WriteAsic1Font(_pth,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
//				if(BitSet(_pth,PTH_X))
//					WriteOSDFntX(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
//				if(BitSet(_pth,PTH_Y))
//					WriteOSDFntY(MASTER,WNDW_RG_APLY_W_X,WNDW_RG_CTRL_Y+_t1_,0x00,OSD_W_SML);
//			}
//		}
//	}
#endif

//...		left button function
	if(b_mse_btn_dwn_lb == 1){
		b_mse_btn_dwn_lb = 0;
//	if(BitSet(mse_flg,MSE_BTN_DWN_LB)){
//		ClearBit(mse_flg,MSE_BTN_DWN_LB);

//		if((_osd_pos_x_ == WNDW_RG_CLOS_X)&&(_osd_pos_y_ == WNDW_RG_CLOS_Y))
//			SetBoxBtnStrPush(BOX_WIN_QUIT);
	}
	if(b_mse_clk_lb == 1){
		b_mse_clk_lb = 0;
//	if(BitSet(mse_flg,MSE_CLK_LB)){
//		ClearBit(mse_flg,MSE_CLK_LB);

		if((_osd_pos_x_ == WNDW_RG_CLOS_X)&&(_osd_pos_y_ == WNDW_RG_CLOS_Y)){
//			SetBoxBtnStrPull(BOX_WIN_QUIT);

//			ClearOSD(_pth,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);
			if(BitSet(_pth,PTH_X))
				WriteOSDClrX(MASTER,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);
			if(BitSet(_pth,PTH_Y))
				WriteOSDClrY(MASTER,WNDW_RG_POS_X,WNDW_RG_POS_Y,WNDW_RG_L,WNDW_RG_H);

				WriteOSDClrX(MASTER,WNDW_RG_POS_X-1,WNDW_RG_POS_Y-1,WNDW_RG_L+2,WNDW_RG_H+2);
//			if(BitSet(_pth,PTH_X))
//				WriteOSDClrX(MASTER,16,4,15,15);
//			if(BitSet(_pth,PTH_Y))
//				WriteOSDClrY(MASTER,16,4,15,15);

//			DisableBox(_pth,BOX_WIN_QUIT,BOX_WIN_FRM);	// decoration window, main window

			SetOSDNoMn();

			b_cmn_wndw_mn = OFF;
//			ClearBit(cmn_flg,CMN_WNDW_MN);
			wndw_mn_pos_crnt = 30;
			wndw_rg_osd_pos_x = 50;
			wndw_rg_osd_pos_y = WNDW_RG_CTRL_Y;// 31;// 
		}
#if	defined(__WNDW_RG_EV__)
		else if(_osd_pos_y_ == WNDW_RG_RNG_Y){
			if((_osd_pos_x_ >= WNDW_RG_DATA_B_X)&&(_osd_pos_x_ < (WNDW_RG_DATA_B_X+8))){
//				WriteAsic1Font(_pth,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_CLR,0x00);
//				wndw_rg_bit_ptr_l = WNDW_RG_DATA_B_X+7 - _osd_pos_x_;
//				WriteAsic1Font(_pth,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
//				WriteAsic1Font(_pth,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				if(BitSet(_pth,PTH_X)){
					WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_CLR,0x00);
					WriteOSDFntX(MASTER,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
					WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				}
				if(BitSet(_pth,PTH_Y)){
					WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_CLR,0x00);
					WriteOSDFntY(MASTER,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
					WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				}
				wndw_rg_bit_ptr_l = WNDW_RG_DATA_B_X+7 - _osd_pos_x_;
			}
	#if	defined(__APLY_ALL__)
			else if(_osd_pos_x_ == WNDW_RG_APLY_W_X){
				for(_t1_=0;_t1_<WNDW_RG_NUM;_t1_++){
//					IfBitSet(wndw_rg_wrt_en,BIT0<<_t1_){
					if(BitSet(wndw_rg_wrt_en,BIT0<<_t1_)){
						WriteAsicByte(wndw_rg_av_dvc[_t1_],wndw_rg_av_pg[_t1_],wndw_rg_av_addr[_t1_],wndw_rg_av_dat[_t1_]);
					}
//					IfBitSet(wndw_rg_rd_en,BIT0<<_t1_){
					if(BitSet(wndw_rg_rd_en,BIT0<<_t1_)){
						wndw_rg_av_dat[_t1_] = ReadAsicByte(wndw_rg_av_dvc[_t1_],wndw_rg_av_pg[_t1_],wndw_rg_av_addr[_t1_]);
					}
					SetWndwRgOSDDat(_pth,50,_t1_,wndw_rg_av_dat);
				}
			}
	#endif
		}
#endif
		else if((_osd_pos_y_ >= WNDW_RG_CTRL_Y)&&(_osd_pos_y_ < (WNDW_RG_CTRL_Y+WNDW_RG_NUM))){
			if(_osd_pos_x_ == WNDW_RG_READ_X){
				_osd_pos_y_ -= WNDW_RG_CTRL_Y;
				
				wndw_rg_rd_en ^= BIT0<<_osd_pos_y_;
				SetWndwRgOSDRd(_pth,_osd_pos_y_);
			}
#if	defined(__WNDW_RG_EV__)
			else if(_osd_pos_x_ == WNDW_RG_WRT_X){
				_osd_pos_y_ -= WNDW_RG_CTRL_Y;
				
				wndw_rg_wrt_en ^= BIT0<<_osd_pos_y_;
				SetWndwRgOSDWrt(_pth,_osd_pos_y_);
			}
#endif
			else{
				RunWndwRgDec(_pth,_osd_pos_x_,_osd_pos_y_);
				RunWndwRgAply(_pth,_osd_pos_x_,_osd_pos_y_);
			}
		}
	}
	if(b_mse_pshng_clk_lb == 1){
		b_mse_pshng_clk_lb = 0;
//	if(BitSet(mse_flg,MSE_PSHNG_CLK_LB)){
//		ClearBit(mse_flg,MSE_PSHNG_CLK_LB);

		if((_osd_pos_y_ >= WNDW_RG_CTRL_Y)&&(_osd_pos_y_ < (WNDW_RG_CTRL_Y+WNDW_RG_NUM))){
			RunWndwRgDec(_pth,_osd_pos_x_,_osd_pos_y_);
			RunWndwRgAply(_pth,_osd_pos_x_,_osd_pos_y_);
		}
	}

//...		right button function
	if(b_mse_btn_dwn_rb == 1){
		b_mse_btn_dwn_rb = 0;
//	if(BitSet(mse_flg,MSE_BTN_DWN_RB)){
//		ClearBit(mse_flg,MSE_BTN_DWN_RB);
	}
	if(b_mse_clk_rb == 1){
		b_mse_clk_rb = 0;
//	if(BitSet(mse_flg,MSE_CLK_RB)){
//		ClearBit(mse_flg,MSE_CLK_RB);

//if((_osd_pos_x_ == OSD_PTH_POS_X)&&(_osd_pos_y_ == OSD_PTH_POS_Y)){
//	P4_3 ^= 1;
//}


		if((_osd_pos_y_ >= WNDW_RG_CTRL_Y)&&(_osd_pos_y_ < (WNDW_RG_CTRL_Y+WNDW_RG_NUM)))
			RunWndwRgInc(_pth,_osd_pos_x_,_osd_pos_y_);
#if	defined(__WNDW_RG_EV__)
		else if(_osd_pos_y_ == WNDW_RG_RNG_Y){
			if((_osd_pos_x_ >= WNDW_RG_DATA_B_X)&&(_osd_pos_x_ < (WNDW_RG_DATA_B_X+8))){
//				WriteAsic1Font(_pth,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_CLR,0x00);
//				wndw_rg_bit_ptr_r = WNDW_RG_DATA_B_X+7 - _osd_pos_x_;
//				WriteAsic1Font(_pth,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
//				WriteAsic1Font(_pth,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				if(BitSet(_pth,PTH_X)){
					WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_CLR,0x00);
					WriteOSDFntX(MASTER,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
					WriteOSDFntX(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				}
				if(BitSet(_pth,PTH_Y)){
					WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7 - wndw_rg_bit_ptr_r,_osd_pos_y_,OSD_CLR,0x00);
					WriteOSDFntY(MASTER,_osd_pos_x_,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
					WriteOSDFntY(MASTER,WNDW_RG_DATA_B_X+7-wndw_rg_bit_ptr_l,_osd_pos_y_,OSD_MIX,OSD_ARW_D_SML);
				}
				wndw_rg_bit_ptr_r = WNDW_RG_DATA_B_X+7 - _osd_pos_x_;
			}
		}
#endif
	}
	if(b_mse_pshng_clk_rb == 1){
		b_mse_pshng_clk_rb = 0;
//	if(BitSet(mse_flg,MSE_PSHNG_CLK_RB)){
//		ClearBit(mse_flg,MSE_PSHNG_CLK_RB);

		if((_osd_pos_y_ >= WNDW_RG_CTRL_Y)&&(_osd_pos_y_ < (WNDW_RG_CTRL_Y+WNDW_RG_NUM)))
			RunWndwRgInc(_pth,_osd_pos_x_,_osd_pos_y_);
	}
}
//==================================================================================


//				if(BitSet(mse_flg,MSE_BTN_DWN_LB)){
//					ClearBit(mse_flg,MSE_BTN_DWN_LB);
//				}
//				if(BitSet(mse_flg,MSE_CLK_LB)){
//					ClearBit(mse_flg,MSE_CLK_LB);
//				}
//				if(BitSet(mse_flg,MSE_PSHNG_CLK_LB)){
//					ClearBit(mse_flg,MSE_PSHNG_CLK_LB);
//				}
//				
//				
//				if(BitSet(mse_flg,MSE_BTN_DWN_RB)){
//					ClearBit(mse_flg,MSE_BTN_DWN_RB);
//				}
//				if(BitSet(mse_flg,MSE_CLK_RB)){
//					ClearBit(mse_flg,MSE_CLK_RB);
//				}
//				if(BitSet(mse_flg,MSE_PSHNG_CLK_RB)){
//					ClearBit(mse_flg,MSE_PSHNG_CLK_RB);
//				}
