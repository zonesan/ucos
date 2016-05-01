#include	"hdr.h"
#include        "box.h"
#include        "osd.h"
#include        "prtcl.h"
#include        "wndw_rc_fmt.h"


//==================================================================================
//						Channel definition & variable description
//==================================================================================


//==================================================================================
//						Channel property(position & size, scale, ... ) function description
//==================================================================================
void	SetSclH(U8 _pth, U8 _dvc, U8 _ch, U16 _scl_h)
{
//	SetSclHValue(_pth,_dvc,_ch,_scl_h);
//	SetSclHFilter(_pth,_dvc,_ch);
	U8 _t1_;
	U8 _rdat_;
	
	if(BitSet(_pth,PTH_X)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x80+0x10*_ch);
		
		_scl_h >>= 8;
		if(_scl_h > 191){											//	HSCLMSB > 192(0xc0)
			_t1_ = 0;
		}
		else if(_scl_h > 111){										// 112(0x70) < HSCLMSB < 191(0xbf)
			_t1_ = 1;
		}
		else if(_scl_h > 79){										// 80(0x50) < HSCLMSB < 111(0x6f)
			_t1_ = 2;
		}
		else{																//	HSCLMSB < 79(0x4f)
			_t1_ = 3;
		}
		WriteAsicByte(_dvc,DVC_PG0,0x80+0x10*_ch,(_rdat_&0xfc)+_t1_);			//... filter data
		WriteAsicByte(_dvc,DVC_PG0,0x83+0x10*_ch,(U8)_scl_h);						//... scale data (MSB)
	}
	if(BitSet(_pth,PTH_PB)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x85+0x10*_ch);
		
		_scl_h >>= 8;
		if(_scl_h > 191){											//	HSCLMSB > 192(0xc0)
			_t1_ = 0;
		}
		else if(_scl_h > 111){										// 112(0x70) < HSCLMSB < 191(0xbf)
			_t1_ = 1;
		}
		else if(_scl_h > 79){										// 80(0x50) < HSCLMSB < 111(0x6f)
			_t1_ = 2;
		}
		else{																//	HSCLMSB < 79(0x4f)
			_t1_ = 3;
		}
		WriteAsicByte(_dvc,DVC_PG0,0x85+0x10*_ch,(_rdat_&0xfc)+_t1_);			//... filter data
		WriteAsicByte(_dvc,DVC_PG0,0x88+0x10*_ch,(U8)_scl_h);						//... scale data (MSB)
	}
	if(BitSet(_pth,PTH_Y)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x8a+0x10*_ch);
		
		_scl_h >>= 8;
		if(_scl_h > 0x7f)	_t1_ = 0;
		else	_t1_ = BIT4|1;

		WriteAsicByte(_dvc,DVC_PG0,0x8a+0x10*_ch,(_rdat_&0xec)+_t1_);			//... filter data
	}
}
//==================================================================================
void	SetSclV(U8 _pth, U8 _dvc, U8 _ch, U16 _scl_v)
{
//	SetSclVValue(_pth,_dvc,_ch,_VSCL);
//	SetSclVFilter(_pth,_dvc,_ch);
	U8 _t1_;
	U8 _rdat_;
	
	if(BitSet(_pth,PTH_X)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x80+0x10*_ch);
		
		_scl_v >>= 8;
		if(_scl_v > 191){											//	HSCLMSB > 192(0xc0)
			_t1_ = 0;
		}
		else if(_scl_v > 111){										// 112(0x70) < HSCLMSB < 191(0xbf)
			_t1_ = 0;
		}
		else if(_scl_v > 79){										// 80(0x50) < HSCLMSB < 111(0x6f)
			_t1_ = 1;
		}
		else{																//	HSCLMSB < 79(0x4f)
			_t1_ = 2;
		}
		WriteAsicByte(_dvc,DVC_PG0,0x80+0x10*_ch,(_rdat_&0xf3)+(_t1_<<2));			//... filter data
		WriteAsicByte(_dvc,DVC_PG0,0x81+0x10*_ch,(U8)_scl_v);						//... scale data (MSB)
		
		if((b_cmn_jp_vdo == VDO_PAL)&&(_scl_v > 191))
//			WriteAsicByte(_dvc,DVC_PG0,0xc5,0x0f);
			SetAsicFlgType(_dvc,DVC_PG0,0xc5,BIT0<<_ch,BIT0<<_ch);
		else
//			WriteAsicByte(_dvc,DVC_PG0,0xc5,0x00);
			SetAsicFlgType(_dvc,DVC_PG0,0xc5,BIT0<<_ch,0x00);
	}
	if(BitSet(_pth,PTH_PB)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x85+0x10*_ch);
		
		_scl_v >>= 8;
		if(_scl_v > 191){											//	HSCLMSB > 192(0xc0)
			_t1_ = 0;
		}
		else if(_scl_v > 111){										// 112(0x70) < HSCLMSB < 191(0xbf)
			_t1_ = 0;
		}
		else if(_scl_v > 79){										// 80(0x50) < HSCLMSB < 111(0x6f)
			_t1_ = 1;
		}
		else{																//	HSCLMSB < 79(0x4f)
			_t1_ = 2;
		}
		WriteAsicByte(_dvc,DVC_PG0,0x85+0x10*_ch,(_rdat_&0xf3)+(_t1_<<2));			//... filter data
		WriteAsicByte(_dvc,DVC_PG0,0x86+0x10*_ch,(U8)_scl_v);						//... scale data (MSB)
	}
	if(BitSet(_pth,PTH_Y)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG0,0x8a+0x10*_ch);

		_scl_v >>= 8;
		if(_scl_v > 0x7f)	_t1_ = 0;
		else	_t1_ = BIT5;

		WriteAsicByte(_dvc,DVC_PG0,0x8a+0x10*_ch,(_rdat_&0xd3)+_t1_);			//... filter data
		
		if((b_cmn_jp_vdo == VDO_PAL)&&(_scl_v > 191))
//			WriteAsicByte(_dvc,DVC_PG0,0xc5,0xf0);
			SetAsicFlgType(_dvc,DVC_PG0,0xc5,BIT4<<_ch,BIT4<<_ch);
		else
//			WriteAsicByte(_dvc,DVC_PG0,0xc5,0x00);
			SetAsicFlgType(_dvc,DVC_PG0,0xc5,BIT4<<_ch,0x00);
	}
}
//==================================================================================
//==================================================================================
void	SetSclRtoH(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x)
{
	U16 _scl_h_;
	U8 _rto1_, _rto2_;
	U8 _trgt_ch_;

	_rto1_ = (_rto_x&0xf0)>>4;
	_rto2_ = _rto_x&0x0f;
	if(_rto1_ > _rto2_)	_rto1_ = _rto2_;
	
	if(BitSet(_pth,PTH_X)){
//		_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x10+(_ch<<3))&0x03;		//... need to verify
		_trgt_ch_ = _ch;
		_scl_h_ = (0xffff/_rto2_)*_rto1_;
		SetSclH(PTH_X,_dvc,_trgt_ch_,_scl_h_);
	}
	if(BitSet(_pth,PTH_PB)){
//		_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x16+(_ch<<3))&0x0f;
		_trgt_ch_ = _ch;
		_scl_h_ = (0xffff/_rto2_)*_rto1_;
		SetSclH(PTH_PB,_dvc,_trgt_ch_,_scl_h_);
	}
	if(BitSet(_pth,PTH_Y)){
		if((_rto_x == 0x12)||(_rto_x == 0x11)){
//			_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x60+3*_ch)&0x03;
			_trgt_ch_ = _ch;
//			_scl_h_ = (0xffff/_rto2_)*_rto1_;
			if(_rto_x == 0x11)	_scl_h_ = 0xffff;
			else	_scl_h_ = 0x7fff;
			SetSclH(PTH_Y,_dvc,_trgt_ch_,_scl_h_);
		}
	}
}
//==================================================================================
void	SetSclRtoV(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_y)
{
	U16 _scl_v_;
	U8 _rto1_, _rto2_;
	U8 _trgt_ch_;

	_rto1_ = (_rto_y&0xf0)>>4;
	_rto2_ = _rto_y&0x0f;
	if(_rto1_ > _rto2_)	_rto1_ = _rto2_;

	if(BitSet(_pth,PTH_X)){
//		_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x10+(_ch<<3))&0x03;		//... need to verify
		_trgt_ch_ = _ch;
		_scl_v_ = (0xffff/_rto2_)*_rto1_;
		SetSclV(PTH_X,_dvc,_trgt_ch_,_scl_v_);
	}
	if(BitSet(_pth,PTH_PB)){
//		_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x16+(_ch<<3))&0x0f;
		_trgt_ch_ = _ch;
		_scl_v_ = (0xffff/_rto2_)*_rto1_;
		SetSclV(PTH_PB,_dvc,_trgt_ch_,_scl_v_);
	}
	if(BitSet(_pth,PTH_Y)){
		if((_rto_y == 0x12)||(_rto_y == 0x11)){
//			_trgt_ch_ = ReadAsicByte(_dvc,DVC_PG1,0x60+3*_ch)&0x03;
			_trgt_ch_ = _ch;
//			_scl_v_ = (0xffff/_rto2_)*_rto1_;
			if(_rto_y == 0x11)	_scl_v_ = 0xffff;
			else	_scl_v_ = 0x7fff;
			SetSclV(PTH_Y,_dvc,_trgt_ch_,_scl_v_);
		}
	}
}
//==================================================================================
void	SetSclRto(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x, U8 _rto_y)
{
	SetSclRtoH(_pth,_dvc,_ch,_rto_x);
	SetSclRtoV(_pth,_dvc,_ch,_rto_y);

//	SetScaleFilter(_pth,_dvc,_ch);
}
//==================================================================================
//==================================================================================
void	SetPicPos(U8 _pth, U8 _dvc, U8 _ch, U8 _pichl, U8 _picvt)
{
	U8 _rdat_, _picxx_;

//	if(_ch < 4){				//... main channel
//		_picxx_ = 0x30;
//	}
//	else{						//... dummy channel
//		_picxx_ = 0x40;
//		_ch &= 0x03;
//	}
//	
//	if(BitSet(_pth,PTH_X)){

	if(BitSet(_pth,PTH_X)||BitSet(_pth,PTH_PB)){
		if(BitSet(_pth,PTH_PB))		//... dummy channel
			_picxx_ = 0x40;
		else//if(_ch < 4){				//... main channel
			_picxx_ = 0x30;
			
		WriteAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2),_pichl);
		WriteAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2)+2,_picvt);

		if(BitSet(_pth,PTH_X)){
			//... SetPic..(..)함수를 사용할경우,
			//... motion box의 potition까지 함께 바뀌므로 pb에 대해 control할 경우 live에 대한 motion box내용이 바뀜.
			//... 2d box setting
			WriteAsicByte(_dvc,DVC_PG2,0x62+(_ch<<3),_pichl+1);
			WriteAsicByte(_dvc,DVC_PG2,0x64+(_ch<<3),_picvt);
		}
	}
	if(BitSet(_pth,PTH_Y)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG1,0x6d);
		if((_pichl == 90)||(_pichl == 0)){
			if(_pichl == 0)	ClearBit(_rdat_,BIT0<<(_ch<<1));
			else	SetBit(_rdat_,BIT0<<(_ch<<1));
		}
		if((_picvt == (cmn_bot>>1))||(_picvt == 0)){
			if(_picvt == 0)	ClearBit(_rdat_,BIT1<<(_ch<<1));
			else	SetBit(_rdat_,BIT1<<(_ch<<1));
		}
		WriteAsicByte(_dvc,DVC_PG1,0x6d,_rdat_);
	}
}
//==================================================================================
//void	SetPicSz(U8 _pth, U8 _dvc, U8 _ch, U8 v_PICHR, U8 v_PICVB)
//{
//	if(BitSet(_pth,PTH_X){
//		ary_picxx[_ch][1] = v_PICHR;
//		ary_picxx[_ch][3] = v_PICVB;
//		WriteAsicByte(_dvc,DVC_PG1,REG_PICHR+4*_ch,v_PICHR);
//		WriteAsicByte(_dvc,DVC_PG1,REG_PICVB+4*_ch,v_PICVB);
//	}
//	if(BitSet(_pth,PTH_Y){
//		WriteAsicByte(_dvc,DVC_PG1,REG_PICHR+7*_ch+0x30,v_PICHR);
//		WriteAsicByte(_dvc,DVC_PG1,REG_PICVB+7*_ch+0x30,v_PICVB);
//	}
//}
//	*/
//==================================================================================
void	SetPicSzRto(U8 _pth, U8 _dvc, U8 _ch, U8 _rto_x, U8 _rto_y)
{	// div_~ : ex) 0x14 -> 1/4, 0x23 -> 2/3, 0x13 -> 1/3, ...
	U8 _rto1_, _rto2_;
	U8 _pichr_, _picvb_, _picxx_;
	U8 _rdat_;
	
//	if(_ch < 4){				//... main channel
//		_picxx_ = 0x30;
//	}
//	else{						//... dummy channel
//		_picxx_ = 0x40;
//		_ch &= 0x03;
//	}
//	
//	if(BitSet(_pth,PTH_X)){

	if(BitSet(_pth,PTH_X)||BitSet(_pth,PTH_PB)){
		if(BitSet(_pth,PTH_PB))		//... dummy channel
			_picxx_ = 0x40;
		else//if(_ch < 4){				//... main channel
			_picxx_ = 0x30;
			
		_rto1_ = (_rto_x&0xf0)>>4;
		_rto2_ = _rto_x&0x0f;
		if(_rto1_ > _rto2_)	_rto1_ = _rto2_;
//		ary_picxx[_ch][1] = (180*_rto1_)/_rto2_;		// SAM87 micom (IAR C-compiler)
		_pichr_ = (180/_rto2_)*_rto1_;		// 8051 micom (Keil C-compiler)
		_rdat_ = ReadAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2));			//... PICHL
		_pichr_ += _rdat_;
		if(_pichr_ > 180)	_pichr_ = 180;
		WriteAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2)+1,_pichr_);			//... PICHR

		if(BitSet(_pth,PTH_X)){
			//... SetPic..(..)함수를 사용할경우,
			//... motion box의 potition까지 함께 바뀌므로 pb에 대해 control할 경우 live에 대한 motion box내용이 바뀜.
			//... 2d box setting
			WriteAsicByte(_dvc,DVC_PG2,0x63+(_ch<<3),((_pichr_-_rdat_-(8>>_rto2_))>>3)-1);
			//... (HW + 1(md_bnd))*(16 / 2(resol)) = HR - HL - 1(ch_bnd) - pixel_offset(full:4,quad:2,pip:1... -> not contain)
		}

		_rto1_ = (_rto_y&0xf0)>>4;
		_rto2_ = _rto_y&0x0f;
		if(_rto1_ > _rto2_)	_rto1_ = _rto2_;
//		ary_picxx[_ch][3] = (cmn_bot*_rto1_)/_rto2_;		// SAM87 micom (IAR C-compiler)
		_picvb_ = (cmn_bot/_rto2_)*_rto1_;		// 8051 micom (Keil C-compiler)
		_rdat_ = ReadAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2)+2);		//... PICVT
		_picvb_ += _rdat_;
		if(_picvb_ > cmn_bot)	_picvb_ = cmn_bot;
		WriteAsicByte(_dvc,DVC_PG1,_picxx_+(_ch<<2)+3,_picvb_);		//... PICVB

		if(BitSet(_pth,PTH_X)){
			//... SetPic..(..)함수를 사용할경우,
			//... motion box의 potition까지 함께 바뀌므로 pb에 대해 control할 경우 live에 대한 motion box내용이 바뀜.
			//... 2d box setting
	//		if(BitSet(t_2dbox_ctl,_2DBOX_EN_X)
				WriteAsicByte(_dvc,DVC_PG2,0x65+(_ch<<3),((_picvb_-_rdat_)/6)-1);
		}
	}
	if(BitSet(_pth,PTH_Y)){
		_rdat_ = ReadAsicByte(_dvc,DVC_PG1,0x6c);
		if((_rto_x == 0x12)||(_rto_x == 0x11)){
			if(_rto_x == 0x11){
				SetBit(_rdat_,BIT0<<(_ch<<1));
				_pichr_ = 176;//180-4;
			}
			else{
				ClearBit(_rdat_,BIT0<<(_ch<<1));
				_pichr_ = 88;//90-2;
			}
		}
		if((_rto_y == 0x12)||(_rto_y == 0x11)){
			if(_rto_y == 0x11){
				SetBit(_rdat_,BIT1<<(_ch<<1));
				_picvb_ = cmn_bot;
			}
			else{
				ClearBit(_rdat_,BIT1<<(_ch<<1));
				_picvb_ = cmn_bot>>1;
			}
		}
		WriteAsicByte(_dvc,DVC_PG1,0x6c,_rdat_);
	}
}
//==================================================================================
//==================================================================================
void	SetPicRto(U8 _pth, U8 _dvc, U8 _ch, U8 _pichl, U8 _picvt, U8 _rto_x, U8 _rto_y)
{
	SetPicPos(_pth,_dvc,_ch,_pichl,_picvt);
	SetPicSzRto(_pth,_dvc,_ch,_rto_x,_rto_y);
}
//==================================================================================
void	SetChFl(U8 _pth, U8 _dvc, U8 _ch)
{
		SetSclRto(_pth,_dvc,_ch,0x11,0x11);
	SetPicRto(_pth,_dvc,_ch,0,0,0x11,0x11);
}
//==================================================================================
void	SetChQdPos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	U8 _pichl, _picvt;

	if(_cnt_pos == 0){
		_pichl = 0;	_picvt = 0;
	}
	else if(_cnt_pos == 1){
		_pichl = 90;	_picvt = 0;
	}
	else if(_cnt_pos == 2){
		_pichl = 0;	_picvt = cmn_bot>>1;
	}
	else if(_cnt_pos == 3){
		_pichl = 90;	_picvt = cmn_bot>>1;
	}
	else if(_cnt_pos == 4){
		_pichl = 45;	_picvt = cmn_bot>>2;
	}
	
//		SetSclRto(_pth,_dvc,_ch,0x12,0x12);
	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x12,0x12);
}
//==================================================================================
void	SetChQd(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
//	U8 _pichl, _picvt;
//
//	if(_cnt_pos == 0){
//		_pichl = 0;	_picvt = 0;
//	}
//	else if(_cnt_pos == 1){
//		_pichl = 90;	_picvt = 0;
//	}
//	else if(_cnt_pos == 2){
//		_pichl = 0;	_picvt = cmn_bot>>1;
//	}
//	else if(_cnt_pos == 3){
//		_pichl = 90;	_picvt = cmn_bot>>1;
//	}
//	else if(_cnt_pos == 4){
//		_pichl = 45;	_picvt = cmn_bot>>2;
//	}
	
		SetSclRto(_pth,_dvc,_ch,0x12,0x12);
//	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x12,0x12);
	SetChQdPos(_pth,_dvc,_ch,_cnt_pos);
}
//==================================================================================
void	SetChNinePos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	U8 _pichl, _picvt;

//	_pichl = (_cnt_pos%3)*60;
//	_picvt = (_cnt_pos/3)*(cmn_bot/3);
	_pichl = (_cnt_pos&0x03)*60;
	if(b_cmn_jp_vdo == VDO_NTSC)	_picvt = (_cnt_pos/3)*40;
	else	_picvt = (_cnt_pos/3)*48;
//		SetSclRto(_pth,_dvc,_ch,0x13,0x13);
	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x13,0x13);
}
//==================================================================================
void	SetChNine(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
//	U8 _pichl, _picvt;
//
//	_pichl = (_cnt_pos%3)*60;
//	_picvt = (_cnt_pos/3)*(cmn_bot/3);
		SetSclRto(_pth,_dvc,_ch,0x13,0x13);
//	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x13,0x13);
	SetChNinePos(_pth,_dvc,_ch,_cnt_pos);    
}
//==================================================================================

void	SetChPIP(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	U8 _pichl, _picvt,ChipID;
       static int init16chdis =0; 
	//_pichl = (_cnt_pos&0x03)*45;
	//_picvt = (_cnt_pos>>2)*(cmn_bot>>2);
	
       ChipID = HwGetVB2100ChipID(); //position of 16 channel need to set across the chipID---by vincent 2008.01.12
       _pichl = (_cnt_pos&0x03)*45;
	_picvt = ChipID*(cmn_bot>>2);
    
       SetSclRto(_pth,_dvc,_ch,0x14,0x14);
	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x14,0x14);
#if 0
      if(init16chdis==0)//set the register to cascade 4 tw2835 in live display  mode---by vincent 2008.01.12
      {
          switch(ChipID)
          {
          case 0:
              WriteAsicByte(MASTER,1,0,0x8c);
              WriteAsicByte(MASTER,1,0x7e,0xc9);
              break;      
          case 1: 
              WriteAsicByte(SLAVE1,1,0,0x8D);
              WriteAsicByte(SLAVE1,1,0x7e,0xA9);       
              break; 
          case 2:
              WriteAsicByte(SLAVE2,1,0,0x8e);
              WriteAsicByte(SLAVE2,1,0x7e,0x89);
              break;
          case 3:
              WriteAsicByte(SLAVE3,1,0,0xbf);
              WriteAsicByte(SLAVE3,1,0x7e,0x89);
              break;
          default:
              debug("\nChipID err %\n",ChipID);
         }
         init16chdis =1;
     }
#endif
}
#if defined(__16CH__)

//==================================================================================
void	SetCh36Pos(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	U8 _pichl, _picvt;

	_pichl = (_cnt_pos%6)*30;
//	_picvt = (_cnt_pos/6)*(cmn_bot/6);
	if(b_cmn_jp_vdo == VDO_NTSC)	_picvt = (_cnt_pos/6)*20;
	else	_picvt = (_cnt_pos/6)*24;												//... _picvt = (_cnt_pos/6)*(144/6) = _cnt_pos*4;

	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x16,0x16);
}
//==================================================================================
void	SetCh36(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	
		SetSclRto(_pth,_dvc,_ch,0x16,0x16);
	SetCh36Pos(_pth,_dvc,_ch,_cnt_pos);
}
#endif
//==================================================================================
//void	SetChCIF(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
//{
////	U8 _pichl;
//
//	if(_cnt_pos == 0){
//		SetPicRto(_pth,_dvc,_ch,0,0,0x12,0x12);
//	}
//	else if(_cnt_pos == 1){
//		SetPicRto(_pth,_dvc,_ch,90,0,0x12,0x12);
//	}
//	else if(_cnt_pos == 2){
//		SetPicRto(_pth,_dvc,_ch,0,cmn_bot>>1,0x12,0x12);
//	}
//	else if(_cnt_pos == 3){
//		SetPicRto(_pth,_dvc,_ch,90,cmn_bot>>1,0x12,0x12);
//	}
//
//		SetSclRto(_pth,_dvc,_ch,0x12,0x11);
////	SetPicRto(_pth,_dvc,_ch,_pichl,0,0x12,0x11);
//}
//==================================================================================
/*
void	SetChDVR(U8 _pth, U8 _dvc, U8 _ch, U8 _pos_count)
{
	U8 _picvt;

	if(_pos_count == 0)	_picvt = 0;
	else if(_pos_count == 1)	_picvt = cmn_bot>>1;
	
		SetSclRto(_pth,_dvc,_ch,0x11,0x12);
	SetPicRto(_pth,_dvc,_ch,0,_picvt,0x11,0x12);
}
//==================================================================================
void	SetCh1213(U8 _pth, U8 _dvc, U8 _ch, U8 _cnt_pos)
{
	U8 _pichl, _picvt;

	_pichl = (_cnt_pos&0x01)*90;
	_picvt = (_cnt_pos>>1)*(cmn_bot/3);
	
//		SetSclRto(_pth,_dvc,_ch,0x12,0x13);
	SetPicRto(_pth,_dvc,_ch,_pichl,_picvt,0x12,0x13);
}
//	*/
//==================================================================================
void	WaitChStrbDoneX(U16 _flg_ch)
{
	U16 _dly_;
	U16 _t1_;
	U8 _t2_, _t3_;
	U8 _dvc_, _ch_;
	
	for(_dly_=0;_dly_<30000;_dly_++){											//... check finishing strobe for max 30000 times
		_t1_=0;
//		_t1_ |= GetAsicFlgType(MASTER,DVC_PG1,0x04,_flg_ch);
//#if	defined(__16CH__)
//		_t1_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x04,_flg_ch);
//		_t1_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x04,_flg_ch);
//		_t1_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x04,_flg_ch);
//#endif
		for(_t2_=0;_t2_<cmn_max_ch;_t2_++){
			if(_flg_ch&(BIT0<<_t2_)){
				SetCrntDvcCh(_t2_,&_dvc_,&_ch_);
				_t3_ = GetAsicFlgType(_dvc_,DVC_PG1,0x04,BIT0<<_ch_);
				_t1_ |= _t3_<<(_t2_&0x0c);
			}
		}
		if(!_t1_)	break;
	}
}
//==================================================================================
void	WaitChStrbDoneY(U16 _flg_ch)
{
	U16 _dly_;
	U16 _t1_;
	U8 _t2_, _t3_;
	U8 _dvc_, _ch_;
	
	for(_dly_=0;_dly_<30000;_dly_++){											//... check finishing strobe for max 30000 times
		_t1_=0;
//		_t1_ |= GetAsicFlgType(MASTER,DVC_PG1,0x54,_flg_ch);
//#if	defined(__16CH__)
//		_t1_ |= GetAsicFlgType(SLAVE1,DVC_PG1,0x54,_flg_ch);
//		_t1_ |= GetAsicFlgType(SLAVE2,DVC_PG1,0x54,_flg_ch);
//		_t1_ |= GetAsicFlgType(SLAVE3,DVC_PG1,0x54,_flg_ch);
//#endif
		for(_t2_=0;_t2_<cmn_max_ch;_t2_++){
			if(_flg_ch&(BIT0<<_t2_)){
				SetCrntDvcCh(_t2_,&_dvc_,&_ch_);
				_t3_ = GetAsicFlgType(_dvc_,DVC_PG1,0x54,BIT0<<_ch_);
				_t1_ |= _t3_<<(_t2_&0x0c);
			}
		}
		if(!_t1_)	break;
	}
}
//==================================================================================
//*
void InitChScaleAdapt(U8 _dvc, U8 _ch)
{
	U16 _wt1_ = 65536/cmn_bot;
	U8 _hl_, _hr_, _vt_, _vb_;
	U8 _t1_;

	_hl_ = ReadAsicByte(_dvc,DVC_PG1,0x30+(_ch<<2));
	_hr_ = ReadAsicByte(_dvc,DVC_PG1,0x31+(_ch<<2));
	_vt_ = ReadAsicByte(_dvc,DVC_PG1,0x32+(_ch<<2));
	_vb_ = ReadAsicByte(_dvc,DVC_PG1,0x33+(_ch<<2));
//#if	defined(__4CH__)
//	if(b_jp_4pb == OFF){
//#endif
//		if(GetAsicFlgType(_dvc,DVC_PG0,0x38,BIT0<<_ch)){				//... when enable pb
		if(GetAsicFlgType(_dvc,DVC_PG1,0x10+(_ch<<3),BIT2)){				//... when enable pb
//			if(wndw_ds_pb_inpt == PB_INPT_REC){
//				if(b_rc_fm_type == FM_TYPE_CIF){
				if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
					if((_hr_-_hl_) > 90){													//... max h size
						_hr_ = _hl_ + 90;
						WriteAsicByte(_dvc,DVC_PG1,0x31+(_ch<<2),_hr_);
					}
					_hr_ = (_hr_-_hl_)<<1;
					
					if((_vb_-_vt_) > (cmn_bot>>1)){									//... max v size
						_vb_ = _vt_ + (cmn_bot>>1);
						WriteAsicByte(_dvc,DVC_PG1,0x33+(_ch<<2),_vb_);
					}
					_vb_ = (_vb_-_vt_)<<1;
					
//					if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
					if(wndw_rc_fm_type == CIF_TYPE_FR){
						//... update:050707 Rev2.0
#if	defined(__4CH__)
						_t1_ = 0x00;
#elif	defined(__16CH__)
						if(_vb_ <= (cmn_bot>>1)){										//... if(realsize <= pip size)
							_vb_ <<= 1;
							_t1_ = BIT5;
						}
						else	_t1_ = 0x00;
#endif
						SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,_t1_);
					}
					else{
						SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);
					}
				}
				else{																	//... update (date:050401 Rev1.2)
					_hr_ = _hr_-_hl_;
					_vb_ = _vb_-_vt_;
					//... update:050706 Rev2.0
					if(_vb_ > (cmn_bot>>1))	SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);
					else{
						_vb_ <<= 1;
						SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);
					}
				}
//			}
//			else if(wndw_ds_pb_inpt == PB_INPT_MUX){							//... update (date:050401 Rev1.2)
//				_hr_ = _hr_-_hl_;
//				_vb_ = _vb_-_vt_;
//				//... update:050706 Rev2.0
//				if(_vb_ > (cmn_bot>>1))	SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);
//				else{
//					_vb_ <<= 1;
//					SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);
//				}
//			}
//			else{
//				if((_hr_-_hl_) > 90){													//... max h size
//					_hr_ = _hl_ + 90;
//					WriteAsicByte(_dvc,DVC_PG1,0x31+(_ch<<2),_hr_);
//				}
//				_hr_ = (_hr_-_hl_)<<1;
//				
//				if((_vb_-_vt_) > (cmn_bot>>1)){									//... max v size
//					_vb_ = _vt_ + (cmn_bot>>1);
//					WriteAsicByte(_dvc,DVC_PG1,0x33+(_ch<<2),_vb_);
//				}
//				_vb_ = (_vb_-_vt_)<<1;
//				
//				if(wndw_rc_fm_cif_type == CIF_TYPE_FR){
//						//... update:050707 Rev2.0
//#if	defined(__4CH__)
//					_t1_ = 0x00;
//#elif	defined(__16CH__)
//					if(_vb_ <= (cmn_bot>>1)){										//... if(realsize <= pip size)
//						_vb_ <<= 1;
//						_t1_ = BIT5;
//					}
//					else	_t1_ = 0x00;
//#endif
//					SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,_t1_);
//				}
//				else{
//					SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);
//				}
//			}
			SetSclH(PTH_PB,_dvc,_ch,_hr_*0x016c);	// 0x016c(65536/180=364.09)
			SetSclV(PTH_PB,_dvc,_ch,_vb_*_wt1_);
		}
		else{
			SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);
			_hr_ = _hr_-_hl_;
			_vb_ = _vb_-_vt_;

			SetSclH(PTH_X,_dvc,_ch,_hr_*0x016c);	// 0x016c(65536/180=364.09)
			SetSclV(PTH_X,_dvc,_ch,_vb_*_wt1_);
		}
////#if	defined(__4CH__)
////	}
////	else{
////		SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);
////		_hr_ = _hr_-_hl_;
////		_vb_ = _vb_-_vt_;
////	}
////#endif
//	SetSclH(PTH_X,_dvc,_ch,_hr_*0x016c);	// 0x016c(65536/180=364.09)
//	SetSclV(PTH_X,_dvc,_ch,_vb_*_wt1_);
//
////	if(GetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5))
////		SetAsicFlgType(_dvc,DVC_PG0,0x15+0x40*_ch,BIT5,BIT5);				// set 2 for V_ALIAS_X(page0) when FIELD_OP(page1) == 1.
}
//	*/
//==================================================================================
