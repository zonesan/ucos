#include	"hdr.h"
#include	"prtcl.h"
#include	"tw_iic.h"
//#include	"iic.h"

//==================================================================================
//==================================================================================
//		function description
//==================================================================================
//==================================================================================
//==================================================================================
void WriteAsicByteOSD(U8 _dvc, U8 _addr, U8 _wdat)
{
U8 *dptr;
	dptr = &_wdat;
	TW_IICWriteByteB(_dvc, 0x2, _addr, dptr, 1); //DWDW, OSD page# is 2
}

//==================================================================================
void WriteAsicByte(U8 _dvc, U8 _pg, U8 _addr, U8 _wdat)
{
U8 *dptr;
	dptr = &_wdat;
	TW_IICWriteByteB(_dvc, _pg, _addr, dptr, 1); //DWDW 
}

//==================================================================================
void WriteAsicTable(U8 _dvc, U8 _pg, U8 _addr, U8 *_tbl_ptr, U8 _tbl_cnt)
{
	TW_IICWriteByteB(_dvc, _pg, _addr, _tbl_ptr, _tbl_cnt); //DWDW
}

//==================================================================================
U8 ReadAsicByte(U8 _dvc, U8 _pg, U8 _addr)
{
U8 _rdat_;

	TW_IICReadByteB(_dvc, _pg, _addr, &_rdat_, 1);
	return _rdat_;
}

//==================================================================================
U8	ReadAsicOSDMemWr(U8 _dvc)//void)
{
U8 _rdat_;

	TW_IICReadByteB(_dvc, 0x2, 0x0a, &_rdat_, 1); //OSDMemWr is page 2, addr 0x0a
	return _rdat_;
}

