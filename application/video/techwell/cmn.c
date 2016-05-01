#include "hdr.h"
#include "cmn.h"
#include "prtcl.h"

int	b_cmn_dtct_vs = 0;
int	b_cmn_jp_vdo = 0;
int	b_cmn_wndw_pop = 0;
int	b_cmn_wndw_mn = 0;
int	b_cmn_irq = 0;
int	b_cmn_fld_od = 0;

int	b_cmn_tmp = 0;


int b_cmn_cnt_vs = 0;
int b_cmn_cnt_fld = 0;
int b_cmn_cnt_tgl = 0;
int b_cmn_cnt_01s = 0;
int b_cmn_cnt_02s = 0;
int b_cmn_cnt_05s = 0;
int b_cmn_cnt_1s = 0;

//------------------------------------------------------------------------------------------
int	b_ds_pb_cif = 0;
int	b_ds_wth_4pb = 0;
int	b_ds_wth_16pb = 0;
int	b_ds_wth_rc = 0;
int	b_ds_in_run = 0;
int	b_ds_in_auto = 0;

int	b_ch_src = 0;
int	b_ado_lv_mr = 0;
int	b_ado_lv_mr_arw_pb = 0;

//------------------------------------------------------------------------------------------

U8	cmn_bot;
//U8	run_8ch_cnt_ana_sw;
U8	cmn_cnt_vs=0;
U8	cmn_cnt_vs_sub=0;
U8	cmn_vga_det=0;				//... __WTH_VGA__일때 idata 없애면 compile시 error 발생.

#if defined(__4CH__)
	U8	cmn_dvc = MASTER;
	U8	cmn_lst_slvr_x = MASTER;
	U8	cmn_lst_slvr_y = MASTER;
	U8	cmn_max_ch = 4;//8;
#elif	defined(__16CH__)
	U8	cmn_rc_md;
	U8	cmn_dvc = DVC_ALL;
	U8	cmn_lst_slvr_x = SLAVE3;
	U8	cmn_lst_slvr_y = SLAVE3;
	U8	cmn_max_ch = 16;
#endif

//==================================================================================
//						Common function description ( old one of 8051 )
//==================================================================================
#if defined(__16CH__)
void	WaitCmnFldOd(void)
{
	U16 _dly_=0xffff;

	b_cmn_fld_od = 0;
	while(!b_cmn_fld_od){
		if(!_dly_)	break;
		_dly_--;
	}
}
#endif
//==================================================================================
U8	GetCodeLength(unsigned char *_str)
{
	 U8 _cnt_=0;

  while((*_str++)!='\0')++_cnt_;
  return (_cnt_);
}
//==================================================================================
U8	ConvDec2Bcd(U8 _dat)	// ex : 38 -> 0x38, 99 -> 0x99 (BCD값이므로 a~f는 포함이 안 된다는 가정에서, decimal값 10마다 6씩 더해준다.)
{
    U8  _bcd_=0;
    
    _bcd_=(_dat/10)*6;//((BCDValue<<2)+(BCDValue<<1));
    _dat+=_bcd_;

    return _dat;
}
//==================================================================================
/*
U8	ConvBcd2Dec(U8 _dat)	// ex : 0x38 -> 38, 0xaf -> X (BCD값이므로 a~f는 포함이 안 된다는 가정에서, hex값 0x10마다 6씩 빼준다.)
{
    U8  _bin_=0;
        
    _bin_=(_dat/0x10)*6;//((_dat>>4)<<2)+(_dat>>4)<<1));
    _dat-=_bin_;

    return _dat;
}
//==================================================================================
//==================================================================================
U8	GetBitFlgType(U8 _var, U8 _flg)
{
	return	(_var&_flg);						//... status of destination bit
}
//==================================================================================
void	SetBitFlgType(U8 *_p_var, U8 _flg, U8 _data)
{
	*_p_var = (*_p_var & ~_flg)|_data;
}
//==================================================================================
U8	GetBitVarType(U8 _var, U8 _bit_ofst, U8 _bit_wdth)	//... 
{
	U8 _t1_,_t2_=0;
	
	for(_t1_=0;_t1_<_bit_wdth;_t1_++)	_t2_ |= (BIT0<<_t1_);
	
	return	(_var&(_t2_<<_bit_ofst))>>_bit_ofst;						//... value of destination bit
}
//==================================================================================
void	SetBitVarType(U8 *_p_var, U8 _bit_ofst, U8 _bit_wdth, U8 _data)
{
	U8 _t1_,_t2_=0;
	
	for(_t1_=0;_t1_<_bit_wdth;_t1_++)	_t2_ |= (BIT0<<_t1_);
	
	*_p_var = (*_p_var & ~(_t2_<<_bit_ofst))|(_data<<_bit_ofst);
}
//	*/
//==================================================================================
//==================================================================================
void	SetCrntDvcCh(U8 _ch, U8 *_crnt_dvc, U8 *_crnt_ch)
{
extern U8 cmn_dvc;//set 4 techwell differently ---vincent 2008.01.29

#if defined(__4CH__)
	*_crnt_dvc = MASTER;
	*_crnt_ch = _ch;
#elif	defined(__16CH__)
	*_crnt_dvc = MASTER<<((_ch&0x0c)>>1);
	*_crnt_ch = _ch&0x03;
#endif
       *_crnt_dvc = cmn_dvc;
}
//==================================================================================
U8	GetAsicFlgType(U8 _dvc, U8 _pg, U8 _addr, U8 _flg)
{
	U8 _t1_=0;

	_t1_ = ReadAsicByte(_dvc,_pg,_addr);

//	return	(_t1_&_flg) ? (ON):(OFF);			//... only 0 or 1
	return	_t1_&_flg;								//... status of destination bit
}
//==================================================================================
void	SetAsicFlgType(U8 _dvc, U8 _pg, U8 _addr, U8 _flg, U8 _data)
{
	U8 _t1_;

	_t1_ = ReadAsicByte(_dvc,_pg,_addr);
	_t1_ = (_t1_ & ~_flg)|_data;
	WriteAsicByte(_dvc,_pg,_addr,_t1_);
}
//==================================================================================
//*
U8	GetAsicVar8Type(U8 _dvc, U8 _pg, U8 _addr, U8 _flg)
{
	U8 _ofst_, _wdth_;
	U8 _t1_;

	_ofst_ = (_flg&0xf0)>>4;
	_wdth_ = _flg&0x0f;

	_flg = 0;
	for(_t1_=0;_t1_<_wdth_;_t1_++)	_flg += BIT0<<_t1_;
//	if(_wdth_ == 1)	_wdth_ = 0x01;
//	else if(_wdth_ == 2)	_wdth_ = 0x03;	
//	else if(_wdth_ == 3)	_wdth_ = 0x07;
//	else if(_wdth_ == 4)	_wdth_ = 0x0f;
//	else if(_wdth_ == 5)	_wdth_ = 0x1f;
//	else if(_wdth_ == 6)	_wdth_ = 0x3f;
//	else if(_wdth_ == 7)	_wdth_ = 0x7f;
//	else if(_wdth_ == 8)	_wdth_ = 0xff;

	_flg <<= _ofst_;
	_t1_ = ReadAsicByte(_dvc,_pg,_addr)&_flg;

	return	_t1_>>_ofst_;										//... value of destination bit
//	return	_t1_&_flg;							//... status of destination bit
//	return	(_t1_&_flg) ? (ON):(OFF);		//... only 0 or 1
}
//==================================================================================
void	SetAsicVar8Type(U8 _dvc, U8 _pg, U8 _addr, U8 _flg, U8 _data)
{
	U8 _ofst_, _wdth_;
	U8 _t1_;

	_ofst_ = (_flg&0xf0)>>4;
	_wdth_ = _flg&0x0f;

	_flg = 0;
	for(_t1_=0;_t1_<_wdth_;_t1_++)	_flg += BIT0<<_t1_;
//	if(_wdth_ == 1)	_wdth_ = 0x01;
//	else if(_wdth_ == 2)	_wdth_ = 0x03;	
//	else if(_wdth_ == 3)	_wdth_ = 0x07;
//	else if(_wdth_ == 4)	_wdth_ = 0x0f;
//	else if(_wdth_ == 5)	_wdth_ = 0x1f;
//	else if(_wdth_ == 6)	_wdth_ = 0x3f;
//	else if(_wdth_ == 7)	_wdth_ = 0x7f;
//	else if(_wdth_ == 8)	_wdth_ = 0xff;

	_flg <<= _ofst_;
	_t1_ = ReadAsicByte(_dvc,_pg,_addr);
	_t1_ = (_t1_ & ~_flg)|(_data<<_ofst_);
	WriteAsicByte(_dvc,_pg,_addr,_t1_);
}
//	*/
//==================================================================================
/*
U8	GetAsicBi_1_6Msb(U8 _dvc, U8 _pg, U8 _addr_msb, U8 _bit_msb, U8 _addr_lsb)
{
	U8 t_w=0;
	U8 _ofst_, _wdth_;
//	U8 _t1_;

	_ofst_ = (_bit_msb&0xf0)>>4;
	_wdth_ = _bit_msb&0x0f;

//	for(_t1_=_wdth_;_t1_>0;_t1_--)	_wdth_ |= (BIT0<<(_t1_-1));
	if(_wdth_ == 1)	_wdth_ = 0x01;
	else if(_wdth_ == 2)	_wdth_ = 0x03;	
	else if(_wdth_ == 3)	_wdth_ = 0x07;
	else if(_wdth_ == 4)	_wdth_ = 0x0f;
	else if(_wdth_ == 5)	_wdth_ = 0x1f;
	else if(_wdth_ == 6)	_wdth_ = 0x3f;
	else if(_wdth_ == 7)	_wdth_ = 0x7f;
	else if(_wdth_ == 8)	_wdth_ = 0xff;

	_wdth_ <<= _ofst_;
	t_w = ReadAsicByte(_dvc,_pg,_addr_msb) & _wdth_;
	t_w <<= (8-_ofst_);
	t_w |= ReadAsicByte(_dvc,_pg,_addr_lsb);

	return	t_w;
}
//==================================================================================
void	SetAsicBi_1_6Msb(U8 _dvc, U8 _pg, U8 _addr_msb, U8 _bit_msb, U8 _addr_lsb, U8 _data)
{
//	U8 t_w=0;
	U8 _ofst_, _wdth_;
	U8 t_data=0;
	U8 _t1_;

	_ofst_ = (_bit_msb&0xf0)>>4;
	_wdth_ = _bit_msb&0x0f;

//	for(_t1_=_wdth_;_t1_>0;_t1_--)	_wdth_ |= (BIT0<<(_t1_-1));
	if(_wdth_ == 1)	_wdth_ = 0x01;
	else if(_wdth_ == 2)	_wdth_ = 0x03;	
	else if(_wdth_ == 3)	_wdth_ = 0x07;
	else if(_wdth_ == 4)	_wdth_ = 0x0f;
	else if(_wdth_ == 5)	_wdth_ = 0x1f;
	else if(_wdth_ == 6)	_wdth_ = 0x3f;
	else if(_wdth_ == 7)	_wdth_ = 0x7f;
	else if(_wdth_ == 8)	_wdth_ = 0xff;

	WriteAsicByte(_dvc,_pg,_addr_lsb,(U8)_data);
	t_data = (U8)(_data>>8);
	t_data = (t_data&_wdth_)<<_ofst_;
	_wdth_ <<= _ofst_;
	_t1_ = ReadAsicByte(_dvc,_pg,_addr_msb);
	t_data = (_t1_ & ~_wdth_)|t_data;
	WriteAsicByte(_dvc,_pg,_addr_msb,t_data);
}
//	*/
//==================================================================================
U16	ChkNoVdo(void)
{
	U16 _rdat_=0;

	_rdat_ = GetAsicFlgType(MASTER,DVC_PG1,0x78,0x0f);
	_rdat_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x78,0x0f)<<4;
	_rdat_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x78,0x0f)<<8;
	_rdat_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x78,0x0f)<<12;

	return	_rdat_;
}
//==================================================================================
