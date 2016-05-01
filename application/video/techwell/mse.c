#include "hdr.h"
#include "cmn.h"
#include "prtcl.h"
#include "mse.h"
#include "init_dvc.h"
#include "osd.h"
#include "ch.h"
#include "gpio.h"

//==================================================================================
//						Mouse definition & variable description
//==================================================================================

int b_mse_btn_l = 0;
int b_mse_btn_r = 0;
int b_mse_rlse_lb = 0;
int b_mse_rlse_rb = 0;
int b_mse_pshd_lb = 0;
int b_mse_pshd_rb = 0;
int b_mse_pshng_lb = 0;
int b_mse_pshng_rb = 0;

int b_mse_btn_dwn_lb = 0;
int b_mse_btn_dwn_rb = 0;
int b_mse_clk_lb = 0;
int b_mse_clk_rb = 0;
int b_mse_pshng_clk_lb = 0;
int b_mse_pshng_clk_rb = 0;
int b_mse_no_btn = 0;
int b_mse_no_mvng = 0;

int b_mse_rd_done = 0;
int b_mse_scl_bnd_only = 0;
int b_mse_init_scrl = 0;
int b_mse_init_zm = 0;
int b_mse_init_scl = 0;
int b_mse_init_drg = 0;

int b_mse_stts_pth = 0;
int b_mse_stts_pth_pop = 0;
int b_mse_stts_pth_mn = 0;
int b_mse_stts_free = 0;
int b_mse_stts_crsr = 0;
int b_mse_tmp_l = 0;
int b_mse_tmp_r = 0;

U16	mse_flg_on_osd = 0x00;

U8	mse_rdata[4];// = 0;
//... mse_rdata[3]->[31:24],mse_rdata[2]->[23:16],mse_rdata[1]->[15:8],mse_rdata[0]->[7:0]
U8	mse_cnt_clk;

U8	mse_ch;

U8	mse_osd_x=45;//50;//
U8	mse_osd_y=56;//28;//30;//
U8	mse_osd_x_sub=45;//50;//
U8	mse_osd_y_sub=56;//30;//

U8	mse_btn_pshng_cnt = 0;


//==================================================================================
//						Mouse function description
//==================================================================================
void WriteMse(U8 _wdat)
{	//... User have to change data line during clock is low.
	U16 _dly_;

	clr_gpio_int_ena(GPIO_MSE_CLK_BIT);

	//... Mouse start condition
	set_mse_clk_out();
	set_mse_dat_out();
	DLY_FOR(100);							//... w77e516 at 40MHz
	clr_mse_clk();
	DLY_FOR(1000);							//... w77e516 at 40MHz
	clr_mse_dat();
	DLY_FOR(500);							//... w77e516 at 40MHz
	set_mse_clk();
	set_mse_clk_in();
//---------------------------------

	mse_cnt_clk = 0;
	mse_rdata[0] = _wdat;
	mse_rdata[1] = 0x00;
	mse_rdata[2] = 0x00;
	mse_rdata[3] = 0x00;
	
	set_gpio_int_ena(GPIO_MSE_CLK_BIT);
}
//==================================================================================
/*
U8 ReadMse(U8 packet_num)
{	//... Data is changed during clock is high, so data must be latched on clock falling edge.
	U8	i, parity=0, rdata=0;

	for(i=0;i<11;i++)
	{
		while(MOUSE_CLK == 0);
		if((packet_num != 3)||(i != 10))
			while(MOUSE_CLK == 1);
			
		if((i>0)&&(i<9))
		{		
			if(MOUSE_DATA == 1)
			{
				rdata |= BIT7;		// LSB first
				parity++;
			}
			if(i < 8)	
				rdata >>= 1;
		}
	}

	return rdata;
}
//	*/
//==================================================================================
/*
void ReadMseMove(void)
{
	U8 i;
	
	for(i=0;i<4;i++)
	{
		mse_a4_rdata[i] = ReadMse(i);
		if(mse_a4_rdata[0] != 0xfa)
			break;
	}
}
//	*/
//==================================================================================
void InitMse(void)
{
//	U8 buff;
	
	WriteMse(0xff);			//... Mouse reset
	/*
	WriteMse(0xe8);			//... set resolution	
	buff = ReadMse(0);
	if(buff == 0xfa)
		WriteMse(0x00);		//... resolution -> 0
		//	*/
}
//==================================================================================
void SetMseCrsr(U8 _tgl_crsr)
{
	U8 _t1_=ReadAsicByte(MASTER,DVC_PG2,0x10);

	if(_tgl_crsr == CRSR_BIG)	SetBit(_t1_,BIT5);
	else	ClearBit(_t1_,BIT5);
	WriteAsicByte(MASTER,DVC_PG2,0x10,_t1_);
}
//==================================================================================
void SetMseLyr(U8 _tgl_lyr)
{
	U8 _t1_=ReadAsicByte(MASTER,DVC_PG2,0x10);

	if(_tgl_lyr == ON)	SetBit(_t1_,BIT4);
	else	ClearBit(_t1_,BIT4);
	WriteAsicByte(MASTER,DVC_PG2,0x10,_t1_);
}
//==================================================================================
void SetMsePtrX(U16 _pos_x)
{
	U8 _t1_=ReadAsicByte(MASTER,DVC_PG2,0x10);

	IfBitSet(_pos_x, BIT0)	SetBit(_t1_,BIT1);
	else	ClearBit(_t1_,BIT1);
	WriteAsicByte(MASTER,DVC_PG2,0x10,_t1_);
	WriteAsicByte(MASTER,DVC_PG2,0x11,(U8)(_pos_x>>1));
}
//==================================================================================
void SetMsePtrY(U16 _pos_y)
{
	U8 _t1_=ReadAsicByte(MASTER,DVC_PG2,0x10);

	IfBitSet(_pos_y, BIT0)	SetBit(_t1_,BIT0);
	else	ClearBit(_t1_,BIT0);
	WriteAsicByte(MASTER,DVC_PG2,0x10,_t1_);
	WriteAsicByte(MASTER,DVC_PG2,0x12,(U8)(_pos_y>>1));
}
//==================================================================================
void SetMsePtr(U16 _pos_x, U16 _pos_y)			//... set mouse pointer
{
	SetMsePtrX(_pos_x);
	SetMsePtrY(_pos_y);
}
//==================================================================================
/*
void DsplyMsePtr(U8 _pth)							//... display mouse pointer
{
	U8 _t1_=ReadAsicByte(MASTER,DVC_PG2,0x10);

	_t1_ &= 0x3f;
	IfBitSet(_pth,PTH_X)	_t1_ |= BIT7;
	IfBitSet(_pth,PTH_Y)	_t1_ |= BIT6;
	WriteAsicByte(MASTER,DVC_PG2,0x10,_t1_);
}
//	*/
//==================================================================================
//*
U8	GetMseCh(U8 _pos_x, U8 _pos_y)
{
#if defined(__4CH__)
	U8	_pop_en_ch_flg_=0, _pnt_on_ch_flg_=0, _ch_fltr_;
#elif defined(__16CH__)
	U16	_pop_en_ch_flg_=0, _pnt_on_ch_flg_=0, _ch_fltr_;
#endif
	

	U8 _pic_sz_[4];
	U8 _chnl_, _bnd_=0;
	U8 _dvc_, _ch_;
	U8 _t1_;//, _t2_;

	for(_chnl_=0;_chnl_<cmn_max_ch;_chnl_++){		// point가 위에 있는 ch과 popup이 설정된 ch을 scanning.
//		if(_chnl_ < 4){
//
//			_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x10+(_chnl_<<3));
//			if(BitSet(_t1_,BIT6))	SetBit(_pop_en_ch_flg_,BIT0<<_chnl_);
//	
////			for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,REG_PICHL+(_ch_<<2)+_t1_);
//			for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x30+(_chnl_<<2)+_t1_);
//		}
//		else{
//			_t2_ = _chnl_&0x03;
//			_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x13+(_t2_<<3));
//			if(BitSet(_t1_,BIT6))	SetBit(_pop_en_ch_flg_,BIT0<<_chnl_);
//	
////			for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,REG_PICHL+(_ch_<<2)+_t1_);
//			for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x40+(_t2_<<2)+_t1_);
//		}
		SetCrntDvcCh(_chnl_,&_dvc_,&_ch_);
		_t1_ = ReadAsicByte(_dvc_,DVC_PG1,0x10+(_ch_<<3));
		if(BitSet(_t1_,BIT6))	SetBit(_pop_en_ch_flg_,BIT0<<_chnl_);
		for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);
		
		if((_pos_x>=_pic_sz_[0])&&(_pos_x<=_pic_sz_[1])&&(_pos_y>=_pic_sz_[2])&&(_pos_y<=_pic_sz_[3]))
			SetBit(_pnt_on_ch_flg_,BIT0<<_chnl_);
	}
	
	if(_pnt_on_ch_flg_ == 0)	_chnl_ = 0xff;//return 0x0f;
	else{
		_ch_fltr_ = _pnt_on_ch_flg_ & _pop_en_ch_flg_;
		if(_ch_fltr_ == 0){	// point 가 위에 있는 _chnl_ 중 popup이 설정된 ch이 없는 경우.
			for(_chnl_=0;_chnl_<cmn_max_ch;_chnl_++){
				if(BitSet(_pnt_on_ch_flg_,BIT0<<_chnl_))	break;//return _chnl_;
			}
		}
		else{
			for(_chnl_=0;_chnl_<cmn_max_ch;_chnl_++){	// _ch_fltr_ != 0 인 경우, priority가 높은 ch을 뽑아냄.
				if(BitSet(_ch_fltr_,BIT0<<_chnl_))	break;//return _chnl_;
			}
		}
	}

//	if(b_mode_ch == _4CH){			//... for drag & scale
		if(b_mse_rlse_lb == 1){
//		if(BitSet(mse_flg,MSE_RLSE_LB)){//b_mse_release_lb == 1){
			if(_chnl_ != (mse_ch&0x0f))	ClearBit(_bnd_,0xf0);
			else	_bnd_ = mse_ch&0xf0;
			if(_chnl_ != 0xff){
				SetCrntDvcCh(_chnl_,&_dvc_,&_ch_);
				for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);
//				if(_chnl_ < 4){
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x30+(_chnl_<<2)+_t1_);
//				}
//				else{
//					_t2_ = _chnl_&0x03;
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x40+(_t2_<<2)+_t1_);
//				}

				if(((_pos_x >= _pic_sz_[0])&&(_pos_x <= (_pic_sz_[0]+1)))||
					((_pos_x >= (_pic_sz_[1]-1))&&(_pos_x <= _pic_sz_[1]))||
					((_pos_y >= _pic_sz_[2])&&(_pos_y <= (_pic_sz_[2]+1)))||
					((_pos_y >= (_pic_sz_[3]-1))&&(_pos_y <= _pic_sz_[3]))){
						if(b_mse_stts_crsr == CRSR_BIG){
							b_mse_stts_crsr = CRSR_SML;
							SetMseCrsr(CRSR_SML);		// small mouse pointer
						}
//						if(BitSet(mse_flg,MSE_STTS_CRSR)){//b_mse_status_cur == BIG){
//							ClearBit(mse_flg,MSE_STTS_CRSR);
//							SetMseCrsr(CRSR_SML);		// small mouse pointer
//						}

					if((_pos_x == _pic_sz_[0])||(_pos_x == (_pic_sz_[0]+1)))
						SetBit(_bnd_,BIT7);
					else	ClearBit(_bnd_,BIT7);		//... Left		x
					if((_pos_x >= (_pic_sz_[1]-1))&&(_pos_x <= (_pic_sz_[1]+1)))
						SetBit(_bnd_,BIT6);
					else	ClearBit(_bnd_,BIT6);		//... Right		o
					if((_pos_y == _pic_sz_[2]) || (_pos_y == (_pic_sz_[2]+1)))
						SetBit(_bnd_,BIT5);
					else	ClearBit(_bnd_,BIT5);		//... Top		x
					if((_pos_y >= (_pic_sz_[3]-1))&&(_pos_y <= (_pic_sz_[3]+1)))
						SetBit(_bnd_,BIT4);
					else	ClearBit(_bnd_,BIT4);		//... Bottom	o

					if((_bnd_==0x50)||(_bnd_==0x60)||(_bnd_==0x90)||	(_bnd_==0xa0))
						SetMseLyr(OFF);
					else
						SetMseLyr(ON);
				}
				else{
					if(b_mse_stts_crsr == CRSR_SML){
						b_mse_stts_crsr = CRSR_BIG;
//					if(BitClear(mse_flg,MSE_STTS_CRSR)){//b_mse_status_cur == BIG){
//						SetBit(mse_flg,MSE_STTS_CRSR);

						SetMseCrsr(CRSR_BIG);		// small mouse pointer
						SetMseLyr(ON);
						ClearBit(_bnd_,0xf0);
					}
				}
			}
			else{
				if(b_mse_stts_crsr == CRSR_SML){
					b_mse_stts_crsr = CRSR_BIG;
//				if(BitClear(mse_flg,MSE_STTS_CRSR)){//b_mse_status_cur == BIG){
//					SetBit(mse_flg,MSE_STTS_CRSR);

					SetMseCrsr(CRSR_BIG);		// big mouse pointer
					SetMseLyr(ON);
					ClearBit(_bnd_,0xf0);
				}
			}
		}
		else{
			_chnl_ = (mse_ch & 0x0f);
			_bnd_ = (mse_ch & 0xf0);
		}
//	}

	return	_bnd_|_chnl_;
}
//	*/
//==================================================================================
//*
U8	GetMseMaskX(U8 _dvc, U8 _ch, U16 _crsr_pos_x)
{
	U16 _pic_;
	U8 _hw_, _x_;

	_pic_ = ReadAsicByte(_dvc,DVC_PG1,0x30+(_ch<<2));
	_pic_ = (_pic_<<1)+2;		//... +2 -> ???
	_hw_ = ReadAsicByte(_dvc,DVC_PG2,0x63+(_ch<<3));

	_x_ = (U8)((_crsr_pos_x - _pic_)/(_hw_+1));		//... +1 -> 2D ary box boundary
	if(_crsr_pos_x > 1){
		if(_x_ > 0x0f)	_x_ = 0x0f;
	}
	else	_x_ = 0x00;
	
	return	_x_;
}
//==================================================================================
U8	GetMseMaskY(U8 _dvc, U8 _ch, U16 _crsr_pos_y)
{
	U16 _pic_;
	U8 _vw_, _y_;

	_pic_ = ReadAsicByte(_dvc,DVC_PG1,0x32+(_ch<<2));
	_pic_ = (_pic_<<1)+1;		//... +1 -> ???
	_vw_ = ReadAsicByte(_dvc,DVC_PG2,0x65+(_ch<<3));
	
	_y_ = (U8)((_crsr_pos_y - _pic_)/(_vw_+1));		//... +1 -> 2D ary box boundary
	if(_crsr_pos_y > 0){
		if(_y_ > 0x0b)	_y_ = 0x0b;
	}
	else	_y_ = 0x00;

	return	_y_;
}
//	*/
//==================================================================================
U8	GetMseOSDX(U16 _pos_x)
{	// var_XPOINT / 8 = INDEX_H : (8 x 10 font)
	return (U8)(_pos_x/8);				// O
}
//==================================================================================
U8	GetMseOSDY(U16 _pos_y)
{
	// var_YPOINT / 11 = INDEX_Y : (8 x 10 font) & VSPACE==1 & VDELAY == 4
	if(reg_crsr_pos_y < OSD_DLY_V)	return	0;
	else	return (U8)((_pos_y-OSD_DLY_V)/10);
}
//==================================================================================
void	SetMseMvmnt(void)								//... set mouse movement
{
//	signed int  _mv_x_, _mv_y_, _mse_nxt_ptr_;
	S16	_mv_x_, _mv_y_, _mse_nxt_ptr_;
//	U8	t_mse_rd0

//	if((mse_rdata & 0xff) == 0xfa)
	if(mse_rdata[0] == 0xfa)
	{
		//... mse_rdata[31:24],[23:16],[15:8],[7:0] is each read data packet
//		if(BitSet((mse_rdata>>8)&0xff, BIT4))	_mv_x_ = 0xffffff00|((mse_rdata>>16)&0xff);
//		else	_mv_x_ = 0x00000000|((mse_rdata>>16)&0xff);
//		if(BitSet((mse_rdata>>8)&0xff, BIT5))	_mv_y_ = 0xffffff00|((mse_rdata>>24)&0xff);
//		else	_mv_y_ = 0x00000000|((mse_rdata>>24)&0xff);
		if(BitSet(mse_rdata[1], BIT4))	_mv_x_ = 0xff00|(U16)mse_rdata[2];
		else	_mv_x_ = 0x0000|(U16)mse_rdata[2];
		if(BitSet(mse_rdata[1], BIT5))	_mv_y_ = 0xff00|(U16)mse_rdata[3];
		else	_mv_y_ = 0x0000|(U16)mse_rdata[3];

		if((_mv_x_ != 0)||(_mv_y_ != 0))
		{
			b_mse_no_mvng = OFF;
//			ClearBit(mse_flg,MSE_NO_MVNG);

			_mse_nxt_ptr_ = reg_crsr_pos_x + _mv_x_;
			if(_mse_nxt_ptr_ < 0)	reg_crsr_pos_x = 0;
			else if(_mse_nxt_ptr_ > 360)	reg_crsr_pos_x = 360;
			else	reg_crsr_pos_x = _mse_nxt_ptr_;
			
			_mse_nxt_ptr_ = reg_crsr_pos_y - _mv_y_;
			if(_mse_nxt_ptr_ < 0)	reg_crsr_pos_y = 0;
			else if(_mse_nxt_ptr_ > (cmn_bot<<1))	reg_crsr_pos_y = cmn_bot<<1;
			else	reg_crsr_pos_y = _mse_nxt_ptr_;
		}
		else	b_mse_no_mvng = ON;
//		else	SetBit(mse_flg,MSE_NO_MVNG);
	}	
}
//==================================================================================
void	SetMseBtn(void)								//... set mouse button
{
	U8 _mse_btn_;
	U8 mse_flg_var;
	
	mse_flg_var = mse_flg_bits;

	_mse_btn_ = mse_rdata[1]&(RIGHT_BTN|LEFT_BTN);
	if(_mse_btn_ != (mse_flg_var&(RIGHT_BTN|LEFT_BTN))){
		b_mse_no_btn = OFF;
//		ClearBit(mse_flg,MSE_NO_BTN);
		
		mse_flg_var = (mse_flg_bits & ~(RIGHT_BTN|LEFT_BTN))|_mse_btn_;
		b_mse_btn_l = (mse_flg_var>>0)&1; 
		b_mse_btn_r = (mse_flg_var>>1)&1;                
		b_mse_rlse_lb = (mse_flg_var>>2)&1;
		b_mse_rlse_rb = (mse_flg_var>>3)&1;
		b_mse_pshd_lb = (mse_flg_var>>4)&1;
		b_mse_pshd_rb = (mse_flg_var>>5)&1;
		b_mse_pshng_lb = (mse_flg_var>>6)&1;
		b_mse_pshng_rb = (mse_flg_var>>7)&1;
		
	
//		if(BitClear(mse_flg,MSE_BTN_L)){				//... detecting low to high(button up) transition.
//			SetBit(mse_flg,MSE_RLSE_LB);
//			if(BitSet(mse_flg,MSE_PSHD_LB))	SetBit(mse_flg,MSE_CLK_LB);
//			ClearBit(mse_flg,MSE_PSHD_LB);
//			ClearBit(mse_flg,MSE_PSHNG_LB);
//		}
//		else{													//... detecting high to low(button down) transition.
//			if(BitClear(mse_flg,MSE_PSHD_LB)&&BitClear(mse_flg,MSE_PSHNG_LB)){
//				SetBit(mse_flg,MSE_PSHD_LB);
//				ClearBit(mse_flg,MSE_RLSE_LB);
//				SetBit(mse_flg,MSE_BTN_DWN_LB);
//				
//				if(BitClear(mse_flg,MSE_PSHD_RB)||BitSet(mse_flg,MSE_PSHNG_RB)){
//					mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR;				//... default
//				}
//			}
//		}
		if(b_mse_btn_l == 0){				//... detecting low to high(button up) transition.
			b_mse_rlse_lb = 1;
			if(b_mse_pshd_lb == 1)	b_mse_clk_lb = 1;
			b_mse_pshd_lb = 0;
			b_mse_pshng_lb = 0;
		}
		else{													//... detecting high to low(button down) transition.
			if((b_mse_pshd_lb == 0)&&(b_mse_pshng_lb == 0)){
				b_mse_pshd_lb = 1;
				b_mse_rlse_lb = 0;
				b_mse_btn_dwn_lb = 1;
				
				if((b_mse_pshd_rb == 0)||(b_mse_pshng_rb == 1)){
					mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR;				//... default
				}
			}
		}
		
//		if(BitClear(mse_flg,MSE_BTN_R)){				//... detecting low to high(button up) transition.
//			SetBit(mse_flg,MSE_RLSE_RB);
//			if(BitSet(mse_flg,MSE_PSHD_RB))	SetBit(mse_flg,MSE_CLK_RB);
//			ClearBit(mse_flg,MSE_PSHD_RB);
//			ClearBit(mse_flg,MSE_PSHNG_RB);
//		}
//		else{													//... detecting high to low(button down) transition.
//			if(BitClear(mse_flg,MSE_PSHD_RB)&&BitClear(mse_flg,MSE_PSHNG_RB)){
//				SetBit(mse_flg,MSE_PSHD_RB);
//				ClearBit(mse_flg,MSE_RLSE_RB);
//				SetBit(mse_flg,MSE_BTN_DWN_RB);
//				
//				if(BitClear(mse_flg,MSE_PSHD_LB)||BitSet(mse_flg,MSE_PSHNG_LB)){
//					mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR;				//... default
//				}
//			}
//		}
		if(b_mse_btn_r == 0){				//... detecting low to high(button up) transition.
			b_mse_rlse_rb = 1;
			if(b_mse_pshd_rb == 1)	b_mse_clk_rb = 1;
			b_mse_pshd_rb = 0;
			b_mse_pshng_rb = 0;
		}
		else{													//... detecting high to low(button down) transition.
			if((b_mse_pshd_rb == 0)&&(b_mse_pshng_rb == 0)){
				b_mse_pshd_rb = 1;
				b_mse_rlse_rb = 0;
				b_mse_btn_dwn_rb = 1;
				
				if((b_mse_pshd_lb == 0)||(b_mse_pshng_lb == 1)){
					mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR;				//... default
				}
			}
		}
	}
	else{
		if((b_mse_rlse_rb == 1)&&(b_mse_rlse_lb == 1))	b_mse_no_btn = ON;
//		if(BitSet(mse_flg,MSE_RLSE_RB)&&BitSet(mse_flg,MSE_RLSE_LB))	SetBit(mse_flg,MSE_NO_BTN);
		
//		if(BitSet(mse_flg,MSE_PSHD_LB)){
//			if(mse_btn_pshng_cnt > 0)	mse_btn_pshng_cnt--;
//			else{
//				if(BitClear(mse_flg,MSE_PSHNG_LB)){
//					SetBit(mse_flg,MSE_PSHNG_LB);								// pushing button checking
//				}
//				else{
//					SetBit(mse_flg,MSE_PSHNG_CLK_LB);		// continuous button checking
//				}
//				
////				mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR>>3;				//... default
//				mse_btn_pshng_cnt = MSE_BTN_DLY_CNTNS;				//... default
//			}
//		}
//		
//		if(BitSet(mse_flg,MSE_PSHD_RB)){
//			if(mse_btn_pshng_cnt > 0)	mse_btn_pshng_cnt--;
//			else{
//				if(BitClear(mse_flg,MSE_PSHNG_RB)){
//					SetBit(mse_flg,MSE_PSHNG_RB);								// pushing button checking
//				}
//				else{
//					SetBit(mse_flg,MSE_PSHNG_CLK_RB);		// continuous button checking
//				}
//				
////				mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR>>3;				//... default
//				mse_btn_pshng_cnt = MSE_BTN_DLY_CNTNS;				//... default
//			}
//		}
		if(b_mse_pshd_lb == 1){
			if(mse_btn_pshng_cnt > 0)	mse_btn_pshng_cnt--;
			else{
				if(b_mse_pshng_lb == 0){
					b_mse_pshng_lb = 1;								// pushing button checking
				}
				else{
					b_mse_pshng_clk_lb = 1;		// continuous button checking
				}
				
//				mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR>>3;				//... default
				mse_btn_pshng_cnt = MSE_BTN_DLY_CNTNS;				//... default
			}
		}
		
		if(b_mse_pshd_rb == 1){
			if(mse_btn_pshng_cnt > 0)	mse_btn_pshng_cnt--;
			else{
				if(b_mse_pshng_rb == 0){
					b_mse_pshng_rb = 1;								// pushing button checking
				}
				else{
					b_mse_pshng_clk_rb = 1;		// continuous button checking
				}
				
//				mse_btn_pshng_cnt = MSE_BTN_CNT_CHTR>>3;				//... default
				mse_btn_pshng_cnt = MSE_BTN_DLY_CNTNS;				//... default
			}
		}
	}
}
//==================================================================================
void	RunMseOnOSDH(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y, U8 _pos_x_l, U8 _pos_x_r, U8 _pos_y, U8 _pos_flg)
{
	U8 _pth_, _prp_;
	U8 _t1_;

	_pth_ = _pth_prp&0x0f;//0xf0;
	_prp_ = _pth_prp&0xf0;//0x0f;
	if(((_osd_pos_x == _pos_x_l)||(_osd_pos_x == _pos_x_r))&&(_osd_pos_y == _pos_y)){
		if(BitClear(mse_flg_on_osd,_pos_flg)){
			SetBit(mse_flg_on_osd,_pos_flg);
			
			if(_osd_pos_x == _pos_x_l)	_t1_ = OSD_ARW_L_BIG;//OSD_MINUS_SML;
			else	_t1_ = OSD_ARW_R_BIG;//OSD_PLUS_SML;
			WriteOSDFnt(_pth_,MASTER,_osd_pos_x,_osd_pos_y,OSD_MIX|1,_t1_);
		}
	}
	else{
		if(BitSet(mse_flg_on_osd,_pos_flg)){
			ClearBit(mse_flg_on_osd,_pos_flg);

			WriteOSDFnt(_pth_,MASTER,_pos_x_l,_pos_y,_prp_,OSD_ARW_L_SML);
			WriteOSDFnt(_pth_,MASTER,_pos_x_r,_pos_y,_prp_,OSD_ARW_R_SML);
		}
	}
}
//==================================================================================
void	RunMseOnOSDV(U8 _pth_prp, U8 _osd_pos_x, U8 _osd_pos_y, U8 _pos_x, U8 _pos_y_t, U8 _pos_y_b, U8 _pos_flg)
{
	U8 _pth_, _prp_;
	U8 _t1_;

	_pth_ = _pth_prp&0x0f;//0xf0;
	_prp_ = _pth_prp&0xf0;//0x0f;
	if((_osd_pos_x == _pos_x)&&((_osd_pos_y == _pos_y_t)||(_osd_pos_y == _pos_y_b))){
		if(BitClear(mse_flg_on_osd,_pos_flg)){
			SetBit(mse_flg_on_osd,_pos_flg);
			
			if(_osd_pos_y == _pos_y_t)	_t1_ = OSD_ARW_U_BIG;//OSD_MINUS_SML;
			else	_t1_ = OSD_ARW_D_BIG;//OSD_PLUS_SML;
			WriteOSDFnt(_pth_,MASTER,_osd_pos_x,_osd_pos_y,OSD_MIX|1,_t1_);
		}
	}
	else{
		if(BitSet(mse_flg_on_osd,_pos_flg)){
			ClearBit(mse_flg_on_osd,_pos_flg);

			WriteOSDFnt(_pth_,MASTER,_pos_x,_pos_y_t,_prp_,OSD_ARW_U_SML);
			WriteOSDFnt(_pth_,MASTER,_pos_x,_pos_y_b,_prp_,OSD_ARW_D_SML);
		}
	}
}
//==================================================================================

