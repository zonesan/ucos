#include	"hdr.h"
#include	"box.h"
#include	"prtcl.h"

//==================================================================================
//						Box function description
//==================================================================================
//void SetBox3DEffect(U8 tgl_dim, U8 tgl_emp)
//{
//	U8	r_box_mode;
//	
//	r_box_mode = ReadAsicByte(MASTER,DVC_PG2,REG_BOX_MODE);
//	if(tgl_dim == ON)	SetBit(r_box_mode,BOX_TYPE);
//	else	ClearBit(r_box_mode,BOX_TYPE);
//	if(tgl_emp == ON)	SetBit(r_box_mode,BOX_EMP);
//	else	ClearBit(r_box_mode,BOX_EMP);
//	WriteAsicByte(MASTER,DVC_PG2,REG_BOX_MODE,r_box_mode);
//}
////==================================================================================
//void SetBoxPlaneCol(U8 _box_strt, U8 _box_end, U8 _col)
//{
//	U8	_rdat_;
//	U8	_t1_, _t2_;
//
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCOL10+(_t1_>>1));
//		_t2_ = (_t1_&0x01)<<2;														//... _t2_ = 0 or _t2_ = 4
//		_rdat_ = (_rdat_ & (0xf0>>_t2_))|_col<<_t2_;
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCOL10+(_t1_>>1),_rdat_);
//	}
//}
////==================================================================================
//void SetBoxPlane(U8 _box_strt, U8 _box_end, U8 tgl_pln)
//{
//	U8	_rdat_;
//	U8 _t1_;
//	
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_);
//		if(tgl_pln == ON)	SetBit(_rdat_,BOX_PLN_EN);
//		else	ClearBit(_rdat_,BOX_PLN_EN);
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,_rdat_);
//	}
//}
////==================================================================================
//void EnableBox(U8 _pth, U8 _box_num)//U8 _box_strt, U8 _box_end)
//{
////	U8	_rdat_;
////	U8 _t1_=0;
////
////	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
////		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_);
////		ClearBit(_rdat_,BOX_EN_X|BOX_EN_Y);
////		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,_pth|_rdat_);
////	}
//
//	if(BitSet(_pth,PTH_X))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT0,BIT0);
//	if(BitSet(_pth,PTH_Y))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT3,BIT3);
//}
////==================================================================================
void DisableBox(U8 _pth, U8 _box_num)//, U8 _box_strt, U8 _box_end)
{
//	U8	_rdat_;
//	U8 _t1_;
//
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_);
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,(~ _pth) & _rdat_);
//	}

	if(BitSet(_pth,PTH_X))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT1|BIT0,0x00);
	if(BitSet(_pth,PTH_Y))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT4|BIT3,0x00);
}
//==================================================================================
void SetBoxPln(U8 _pth, U8 _box_num, U8 _tgl)//U8 _box_strt, U8 _box_end
{
//	U8	_rdat_
//	U8 _t1_=0
//
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++)
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_)
//		ClearBit(_rdat_,BOX_EN_X|BOX_EN_Y)
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,_pth|_rdat_)
//	

	if(BitSet(_pth,PTH_X))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT0,_tgl);
	if(BitSet(_pth,PTH_Y))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT3,_tgl<<3);
}
//==================================================================================
void SetBoxBnd(U8 _pth, U8 _box_num, U8 _tgl)//U8 _box_strt, U8 _box_end, U8 boundary)
{	//... single box boundary is generated same as channel boundary structure.
	//... that is, boxhl/boxvt is inside of box area, boxhw/boxvw is outside of box area.
//	U8	_rdat_;
//	U8	_t1_;
//
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_);
//		ClearBit(_rdat_,(BOX_OBND|BOX_IBND));
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,_rdat_|boundary);
//	}

	if(BitSet(_pth,PTH_X))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT1,_tgl<<1);
	if(BitSet(_pth,PTH_Y))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT4,_tgl<<4);
}
//==================================================================================
void SetBoxMix(U8 _pth, U8 _box_num, U8 _tgl)//U8 _box_strt, U8 _box_end, U8 tgl_ht)
{
//	U8	_rdat_;
//	U8	_t1_;
//
//	for(_t1_ = _box_strt ; _t1_ <= _box_end ; _t1_++){
//		_rdat_ = ReadAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_);
//		if(tgl_ht == ON)	SetBit(_rdat_,BOX_PLN_MIX);
//		else	ClearBit(_rdat_,BOX_PLN_MIX);
//		WriteAsicByte(MASTER,DVC_PG2,REG_BOXCTL+5*_t1_,_rdat_);
//	}
	if(BitSet(_pth,PTH_X))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT2,_tgl<<2);
	if(BitSet(_pth,PTH_Y))	SetAsicFlgType(MASTER,DVC_PG2,0x20+_box_num*6,BIT5,_tgl<<5);
}
//==================================================================================
void SetBoxPos(U8 _box_num, U16 _boxhl, U16 _boxvt)
{
	U8	_rdat_;

	_rdat_ = ReadAsicByte(MASTER,DVC_PG2,0x21+_box_num*6);

	if(BitSet(_boxhl, BIT0))	SetBit(_rdat_, BIT3);
	else	ClearBit(_rdat_, BIT3);
	if(BitSet(_boxvt, BIT0))	SetBit(_rdat_, BIT1);
	else	ClearBit(_rdat_, BIT1);

	WriteAsicByte(MASTER,DVC_PG2,0x21+_box_num*6,_rdat_);
	WriteAsicByte(MASTER,DVC_PG2,0x22+_box_num*6,(U8)(_boxhl>>1));
	WriteAsicByte(MASTER,DVC_PG2,0x24+_box_num*6,(U8)(_boxvt>>1));
}
//==================================================================================
void SetBoxSize(U8 _box_num, U16 _boxhw, U16 _boxvw)
{
//	WriteAsicByte(MASTER,DVC_PG2,REG_BOXHW+5*_box_num,_boxhw);
//	WriteAsicByte(MASTER,DVC_PG2,REG_BOXVW+5*_box_num,_boxvw);

	U8	_rdat_;

	_rdat_ = ReadAsicByte(MASTER,DVC_PG2,0x21+_box_num*6);

	if(BitSet(_boxhw, BIT0))	SetBit(_rdat_, BIT2);
	else	ClearBit(_rdat_, BIT2);
	if(BitSet(_boxvw, BIT0))	SetBit(_rdat_, BIT0);
	else	ClearBit(_rdat_, BIT0);

	WriteAsicByte(MASTER,DVC_PG2,0x21+_box_num*6,_rdat_);
	WriteAsicByte(MASTER,DVC_PG2,0x23+_box_num*6,(U8)(_boxhw>>1));
	WriteAsicByte(MASTER,DVC_PG2,0x25+_box_num*6,(U8)(_boxvw>>1));
}
//==================================================================================
void SetBox(U8 _box_num, U16 _boxhl, U16 _boxhw, U16 _boxvt, U16 _boxvw)
{
	SetBoxPos(_box_num,_boxhl,_boxvt);
	SetBoxSize(_box_num,_boxhw,_boxvw);
}
//==================================================================================
//void DisableBox(U8 _pth, U8 _box_num)//U8 _box_strt, U8 _box_end)
//{
//	DisableBox(_pth,_box_num);//_box_strt,_box_end);
//}
//==================================================================================

