#include        "hdr.h"
#include        "box.h"
#include        "osd.h"
#include        "prtcl.h"



//==================================================================================
//						Wndwdow box function description
//==================================================================================
void SetBoxPopHLPos(U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth)
{	//	on 16x20 font : 1 OSD's BOXHL=8, 1 OSD's BOXHW=8, 1 OSD's BOXVT=10, 1 OSD's BOXVW=10
	U16 _boxhl_, _boxvt_;
	U16 _boxhw_, _boxvw_;
	
	if(_osd_pos_x == 0){
		_boxhl_ = 0;
		_boxhw_ = (_lngth<<3)+2;
	}
	else{
		_boxhl_ = (_osd_pos_x<<3)-2;
		_boxhw_ = (_lngth<<3)+4;
	}
	
	if(_osd_pos_y == 0){
		_boxvt_ = 0;
		_boxvw_ = 9;//10;
	}
	else{
		_boxvt_ = (_osd_pos_y*10)+OSD_DLY_V;
		_boxvw_ = 9;//10;
	}

	SetBox(_box_num,_boxhl_,_boxhw_,_boxvt_,_boxvw_);
}
//==================================================================================
void CreateBoxPopHL(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth)
{
	SetBoxPopHLPos(_box_num,_osd_pos_x,_osd_pos_y,_lngth);
	SetBoxMix(_pth,_box_num,ON);		//... flat box effect
	SetBoxBnd(_pth,_box_num,OFF);		//... flat box effect
//	EnableBox(_pth,_box_num);//,_box_num);
	SetBoxPln(_pth,_box_num,ON);		//... flat box effect
}
//==================================================================================
void CreateBoxPopFrame(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght)
{
	U16 _boxhl_, _boxvt_;
	U8 _boxhw_, _boxvw_;
	
	if(_osd_pos_x == 0){
		_boxhl_ = 0;
		_boxhw_ = (_lngth<<3)+4;	// 10*4
	}
	else{
		_boxhl_ = (_osd_pos_x<<3)-4;
		_boxhw_ = (_lngth<<3)+8;
	}
	
	if(_osd_pos_y == 0){
		_boxvt_ = 0;
		_boxvw_ = (_hght*10);							// _hght(OSD) * BOXVW(==5) + (== _hght * VSPACE(==1) / 2) + margin
	}
	else{
		_boxvt_ = (_osd_pos_y*10)+OSD_DLY_V-2;
		_boxvw_ = (_hght*10)+3;							// _hght(OSD) * BOXVW(==5) + (== _hght * VSPACE(==1) / 2) + margin
	}

	SetBox(_box_num,_boxhl_,_boxhw_,_boxvt_,_boxvw_);
//	SetBoxMix(_box_num,_box_num,ON);
//	SetBoxBnd(_box_num,_box_num,BOX_OBND);
//	EnableBox(_pth,_box_num,_box_num);
	SetBoxMix(_pth,_box_num,ON);
	SetBoxBnd(_pth,_box_num,ON);//BOX_OBND);
//	EnableBox(_pth,_box_num);//,_box_num);
	SetBoxPln(_pth,_box_num,ON);		//... flat box effect
}
//==================================================================================

