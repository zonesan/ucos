#include	"hdr.h"
#include	"cmn.h"
#include	"mse.h"
#include	"prtcl.h"
#include	"init_dvc.h"
#include	"wndw_rc_fmt.h"
#include	"ch.h"

//==================================================================================
//==================================================================================
//						Mse & channel co-operation definition & variable description
//==================================================================================
//==================================================================================
U8	mse_ch_drg;
U8	mse_ch_scl;
U8	mse_ch_ptr;

U8	mse_ch_l;
U8	mse_ch_r;
U8	mse_ch_t;
U8	mse_ch_b;


//==================================================================================
//==================================================================================
//						Mse & channel co-operation function description
//==================================================================================
//==================================================================================
/*		//... display channel number
void	SetMseFreeOSDCh(U8 _index_x, U8 _index_y, U8 _ch)
{
	U8 _t1_;
	
	RamClearOSD(PTH_X);
	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0xa0);
//	IfBitClear(_t1_,BIT7|BIT6)
		DisplayValue(PTH_X,_index_x,_index_y,0xff,_ch,2);
}
//	*/
//==================================================================================
void SetMseFreeDrg(void)
{
	U8 _pos_x_, _pos_y_;
	U8 _pic_sz_[4];
	U8 _dvc_, _ch_;
	U8 _t1_;//, _t2_;

	_pos_x_ = (U8)(reg_crsr_pos_x>>1);
	_pos_y_ = (U8)(reg_crsr_pos_y>>1);


	if((b_mse_rlse_lb == 0)&&(b_mse_init_scl == 0)){
		if(b_mse_init_drg == 0){
			b_mse_init_drg = 1;
			if(b_mse_stts_pth == MSE_PTH_X)	b_mse_stts_free = ON;
//	if(BitClear(mse_flg,MSE_RLSE_LB)&&BitClear(mse_flg,MSE_INIT_SCL)){
//		if(BitClear(mse_flg,MSE_INIT_DRG)){
//			SetBit(mse_flg,MSE_INIT_DRG);
//			if(BitClear(mse_flg,MSE_STTS_PTH))	SetBit(mse_flg,MSE_STTS_FREE);
			
			if(mse_ch != 0xff){
				mse_ch_drg = mse_ch&0x0f;
				
				SetCrntDvcCh(mse_ch_drg,&_dvc_,&_ch_);
				for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);

//				if(mse_ch_drg < 4){
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x30+(mse_ch_drg<<2)+_t1_);
//				}
//				else{
//					_t2_ = mse_ch_drg&0x03;
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x40+(_t2_<<2)+_t1_);
//				}
				
				SetMseLyr(OFF);
				mse_ch_l = _pos_x_ - _pic_sz_[0];
				mse_ch_r = _pic_sz_[1] - _pos_x_;
				mse_ch_t = _pos_y_ - _pic_sz_[2];
				mse_ch_b = _pic_sz_[3] - _pos_y_;
				
/*		//... display channel number
				_t1_ = (_pic_sz_[0]+2)>>2;
				_t2_ = (_pic_sz_[2]+3)/5;
				SetMseFreeOSDCh(_t1_,_t2_,mse_ch_drg);
				//	*/
			}
			else
				mse_ch_drg = 0xff;
		}

		if(b_mse_stts_free == ON){
//		if(BitSet(mse_flg,MSE_STTS_FREE)){
			if(mse_ch_drg != 0xff){
				if(mse_btn_pshng_cnt == MSE_BTN_DLY_CLK){
					for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
						SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
						if(_t1_ == mse_ch_drg){
//							if(_t1_ < 4){
//	//							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,ON);
//	//							SetChCtrl(PTH_X,_dvc_,_ch_,FREEZE|ENHANCE|BLINK,OFF);
								SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);				//... enable popup
								SetAsicFlgType(_dvc_,DVC_PG1,0x11+(_ch_<<3),BIT6|BIT3|BIT0,0x00);	//... disable freeze,enhance,blink
//							}
//							else{
//								_t2_ = _t1_&0x03;
//	//							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,ON);
//	//							SetChCtrl(PTH_X,_dvc_,_ch_,FREEZE|ENHANCE|BLINK,OFF);
//								SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t2_<<3),BIT6,BIT6);				//... enable popup
//								SetAsicFlgType(MASTER,DVC_PG1,0x14+(_t2_<<3),BIT6|BIT3|BIT0,0x00);	//... disable freeze,enhance,blink
//							}
						}
						else{
//							if(_t1_ < 4){
//	//							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,OFF);
								SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,0x00);				//... disable popup
//							}
//							else{
//								_t2_ = _t1_&0x03;
//	//							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,OFF);
//								SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t2_<<3),BIT6,0x00);				//... disable popup
//							}
						}
					}
				}

				SetCrntDvcCh(mse_ch_drg,&_dvc_,&_ch_);
				for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);

//				if(mse_ch_drg < 4){
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x30+(mse_ch_drg<<2)+_t1_);
//				}
//				else{
//					_t2_ = mse_ch_drg&0x03;
//					for(_t1_=0;_t1_<4;_t1_++)
//						_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x40+(_t2_<<2)+_t1_);
//				}


//				if((_pos_x_-mse_ch_l) > 0)//-1)
				if(_pos_x_ > mse_ch_l)
				{
					if((_pos_x_+mse_ch_r) <= 180)	
						_pic_sz_[0] = _pos_x_ - mse_ch_l;
//						_pic_sz_[0] = _pic_sz_[1]-(mse_ch_l+mse_ch_r);
				}
				else
				{
					_pic_sz_[0] = 0;
					_pic_sz_[1] = mse_ch_l + mse_ch_r;
				}


				if((_pos_x_+mse_ch_r) <= 180){
//					if((_pos_x_-mse_ch_l) > 0)//-1)
					if(_pos_x_ > mse_ch_l)
						_pic_sz_[1] = _pos_x_ + mse_ch_r;
				}
				else{
					_pic_sz_[0] = 180 - (mse_ch_l + mse_ch_r);
					_pic_sz_[1] = 180;
				}


//				if((_pos_y_-mse_ch_t)  > 0)//-1)
				if(_pos_y_ > mse_ch_t)
				{
					if((_pos_y_+mse_ch_b) <= cmn_bot)
						_pic_sz_[2] = _pos_y_ - mse_ch_t;
				}
				else{
					_pic_sz_[2] = 0;
					_pic_sz_[3] = mse_ch_t + mse_ch_b;
				}


				if((_pos_y_+mse_ch_b) <= cmn_bot){
//					if((_pos_y_-mse_ch_t) > 0)//-1)
					if(_pos_y_ > mse_ch_t)
						_pic_sz_[3] = _pos_y_ + mse_ch_b;
				}
				else{
					_pic_sz_[2] = cmn_bot - (mse_ch_t + mse_ch_b);
					_pic_sz_[3] = cmn_bot;
				}


				for(_t1_=0;_t1_<4;_t1_++)	WriteAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_,_pic_sz_[_t1_]);
				//... 2d box setting
				WriteAsicByte(_dvc_,DVC_PG2,0x62+(_ch_<<3),_pic_sz_[0]+1);
				WriteAsicByte(_dvc_,DVC_PG2,0x64+(_ch_<<3),_pic_sz_[2]);

//				if(mse_ch_drg < 4){
//					for(_t1_=0;_t1_<4;_t1_++)
//						WriteAsicByte(MASTER,DVC_PG1,0x30+(mse_ch_drg<<2)+_t1_,_pic_sz_[_t1_]);
//						
//					//... 2d box setting
//					WriteAsicByte(MASTER,DVC_PG2,0x62+(mse_ch_drg<<3),_pic_sz_[0]+1);
//					WriteAsicByte(MASTER,DVC_PG2,0x64+(mse_ch_drg<<3),_pic_sz_[2]);
//				}
//				else{
//					_t2_ = mse_ch_drg&0x03;
//					for(_t1_=0;_t1_<4;_t1_++)
//						WriteAsicByte(MASTER,DVC_PG1,0x40+(_t2_<<2)+_t1_,_pic_sz_[_t1_]);
//						
//					//... 2d box setting
//					WriteAsicByte(MASTER,DVC_PG2,0x62+(_t2_<<3),_pic_sz_[0]+1);
//					WriteAsicByte(MASTER,DVC_PG2,0x64+(_t2_<<3),_pic_sz_[2]);
//				}


/*		//... display channel boundary value
				if(b_jp_video == NTSC)	_t2_ = 20;
				else	_t2_ = 24;
				for(_t1_=0;_t1_<4;_t1_++)	DisplayValue(PTH_X,1+3*_t1_,_t2_,0xff,_pic_sz_[_t1_],2);
				// */

/*	//DWDW... display channel number
				if(b_mse_no_moving == OFF){
					_t1_ = (_pic_sz_[0]+2)>>2;
					_t2_ = (_pic_sz_[2]+3)/5;
					SetMseFreeOSDCh(_t1_,_t2_,mse_ch_drg);
				}
				// */
			}
		}
	}
	else{
		if(b_mse_init_drg == 1){
			b_mse_init_drg = 0;
			SetMseLyr(ON);
			b_mse_stts_free = OFF;
//		if(BitSet(mse_flg,MSE_INIT_DRG)){
//			ClearBit(mse_flg,MSE_INIT_DRG);
//			SetMseLyr(ON);
//			ClearBit(mse_flg,MSE_STTS_FREE);

/*		//... display channel boundary value
			if(b_jp_video == NTSC)	_t2_ = 20;
			else	_t2_ = 24;
			ClearOSD(PTH_X,0,_t2_,45,1);
			//	*/
		}
	}
}
//==================================================================================
//==================================================================================
//*
U16	GetMseFreeSclVSub(U16 _scl, U8 _dvc, U8 _ch, U8 _picvt, U8 _picvb)
{
	if((_picvb - _picvt) <= (cmn_bot>>1)){
		_scl <<= 1;
		SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);		//... FIELD_OP=1
		SetAsicFlgType(_dvc,DVC_PG1,0x06,BIT2,BIT2);					//... CH_VAR_CTL=1
	}
	else{
		SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);		//... FIELD_OP=0
		SetAsicFlgType(_dvc,DVC_PG1,0x06,BIT2,0x00);					//... CH_VAR_CTL=0
	}

	return	_scl;
}
//==================================================================================
U16	GetMseFreeSclV(U16 _scl, U8 _dvc, U8 _ch, U8 _picvt, U8 _picvb)
{
////#if	defined(__4CH__)
////	if(b_jp_4pb == OFF){
////#endif
//		if(GetAsicFlag(_dvc,DVC_PG0,0x38,BIT0<<_ch)){				//... when enable pb
		if(GetAsicFlgType(_dvc,DVC_PG1,0x10+(_ch<<3),BIT2)){				//... PB_PATH_EN=1
////			if(glbl_dspl_pb_inpt == PB_INPT_REC){
//				if(((b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FL))||
//					((b_rec_fmt_type == FMT_TYPE_MUX)&&(glbl_rec_fmt_mux_type != MUX_TYPE_LV))){				//... max v scale of pb input is quad for each channel
				if(wndw_rc_fm_type > CIF_TYPE_FR){
					_scl = GetMseFreeSclVSub(_scl,_dvc,_ch,_picvt,_picvb);
				}
//				else if((b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FR)){
				else if(wndw_rc_fm_type == CIF_TYPE_FR){
					//... update:050707 Rev2.0
					_scl <<= 1;
#if	defined(__16CH__)
					if((_picvb - _picvt) <= (cmn_bot>>2)){			//... if(real size <= pip)
						_scl <<= 1;
						SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,BIT5);		//... FIELD_OP=1
						SetAsicFlgType(_dvc,DVC_PG1,0x06,BIT2,BIT2);					//... CH_VAR_CTL=1
					}
					else{
						SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);		//... FIELD_OP=0
						SetAsicFlgType(_dvc,DVC_PG1,0x06,BIT2,0x00);					//... CH_VAR_CTL=0
					}
#endif
				}
//			}
//			else if(glbl_dspl_pb_inpt == PB_INPT_MUX){
//				if(glbl_rec_fmt_mux_type != MUX_TYPE_LV){
//					_scl = GetMseFreeSclVSub(_scl,_dvc,_ch,_picvt,_picvb);
//				}
//			}
//			else{
//				if(glbl_rec_fmt_cif_type == CIF_TYPE_FL){
//					_scl = GetMseFreeSclVSub(_scl,_dvc,_ch,_picvt,_picvb);
//				}
//				else{
//					//... update:050707 Rev2.0
//					_scl <<= 1;
////#if	defined(__16CH__)
////					if((_picvb - _picvt) <= (cmn_bot>>2)){			//... if(real size <= pip)
////						_scl <<= 1;
////						SetAsicFlag(_dvc,DVC_PG1,REG_FIELD_OP_X,BIT4<<_ch,BIT4<<_ch);
////						SetAsicFlag(_dvc,DVC_PG1,REG_MUX_CTL_X,BIT2,BIT2);
////					}
////					else{
////							SetAsicFlag(_dvc,DVC_PG1,REG_FIELD_OP_X,BIT4<<_ch,0x00);
////							SetAsicFlag(_dvc,DVC_PG1,REG_MUX_CTL_X,BIT2,0x00);
////					}
////#endif
//				}
//			}
		}
		else{
			SetAsicFlgType(_dvc,DVC_PG1,0x12+(_ch<<3),BIT5,0x00);		//... FIELD_OP=0
			SetAsicFlgType(_dvc,DVC_PG1,0x06,BIT2,0x00);					//... CH_VAR_CTL=0
		}
//#if	defined(__4CH__)
//	}
//#endif

	return	_scl;
}
//	*/
//==================================================================================
void	SetMseFreeSclV(U8 _dvc, U8 _ch, U8 _picxx, U8 _picvt, U8 _picvb)
{
	U16 _scl_v_unt_ = 65536/cmn_bot;//

	if(b_mse_scl_bnd_only == 0){
//	if(BitClear(mse_flg,MSE_SCL_BND_ONLY)){
		_scl_v_unt_ *= (_picvb-_picvt);
		_scl_v_unt_ = GetMseFreeSclV(_scl_v_unt_,_dvc,_ch,_picvt,_picvb);

		if(GetAsicFlgType(_dvc,DVC_PG1,0x10+(_ch<<3),BIT2))				//... PB_PATH_EN=1
//			SetSclV(PTH_PB,_dvc,mse_ch_scl,_scl_v_unt_);	
			SetSclV(PTH_PB,_dvc,_ch,_scl_v_unt_);	
		else
//			SetSclV(PTH_X,_dvc,mse_ch_scl,_scl_v_unt_);	
			SetSclV(PTH_X,_dvc,_ch,_scl_v_unt_);	

//		if(GetAsicFlag(_dvc,DVC_PG1,REG_FIELD_OP_X,BIT4<<_ch))
//			SetAsicFlag(_dvc,DVC_PG0,0x15+0x40*_ch,BIT5,BIT5);				// set 2 for V_ALIAS_X(page0) when FIELD_OP(page1) == 1.
	}
//	if(mse_ch_ptr < 4){
		if(_picxx == 0x32)	WriteAsicByte(_dvc,DVC_PG1,0x32+(_ch<<2),_picvt);
		else	WriteAsicByte(_dvc,DVC_PG1,0x33+(_ch<<2),_picvb);
//	}
//	else{
//		if(_picxx == 0x42)	WriteAsicByte(_dvc,DVC_PG1,0x42+(_ch<<2),_picvt);
//		else	WriteAsicByte(_dvc,DVC_PG1,0x43+(_ch<<2),_picvb);
//	}
}
//==================================================================================
void	SetMseFreeSclH(U8 _dvc, U8 _ch, U8 _picxx, U8 _pichl, U8 _pichr, U8 _shft)
{
	if(b_mse_scl_bnd_only == 0){
//	if(BitClear(mse_flg,MSE_SCL_BND_ONLY)){
		if(GetAsicFlgType(_dvc,DVC_PG1,0x10+(_ch<<3),BIT2))				//... PB_PATH_EN=1
//			SetSclH(PTH_PB,_dvc,mse_ch_scl,((_pichr-_pichl)*0x016c)<<_shft);	// 0x016c(65536/180=364.09)
			SetSclH(PTH_PB,_dvc,_ch,((_pichr-_pichl)*0x016c)<<_shft);	// 0x016c(65536/180=364.09)
		else
//			SetSclH(PTH_X,_dvc,mse_ch_scl,((_pichr-_pichl)*0x016c)<<_shft);	// 0x016c(65536/180=364.09)
			SetSclH(PTH_X,_dvc,_ch,((_pichr-_pichl)*0x016c)<<_shft);	// 0x016c(65536/180=364.09)
	}
//	if(mse_ch_ptr < 4){
		if(_picxx == 0x30)	WriteAsicByte(_dvc,DVC_PG1,0x30+(_ch<<2),_pichl);
		else	WriteAsicByte(_dvc,DVC_PG1,0x31+(_ch<<2),_pichr);
//	}
//	else{
//		if(_picxx == 0x40)	WriteAsicByte(_dvc,DVC_PG1,0x40+(_ch<<2),_pichl);
//		else	WriteAsicByte(_dvc,DVC_PG1,0x41+(_ch<<2),_pichr);
//	}
}
//==================================================================================
void	TglMseChMrr(U8 _dvc, U8 _ch, U8 _mrr)
{
	U8 _ch_ctl_;
	
//	if(mse_ch_ptr < 4){
		_ch_ctl_ = ReadAsicByte(_dvc,DVC_PG1,0x11+(_ch<<3));
		_ch_ctl_ ^= _mrr;
		WriteAsicByte(_dvc,DVC_PG1,0x11+(_ch<<3),_ch_ctl_);
//	}
//	else{
//		_ch_ctl_ = ReadAsicByte(_dvc,DVC_PG1,0x14+(_ch<<3));
//		_ch_ctl_ ^= _mrr;
//		WriteAsicByte(_dvc,DVC_PG1,0x14+(_ch<<3),_ch_ctl_);
//	}
}
//==================================================================================
void SetMseFreeScl(void)
{
	U8 _pos_x_, _pos_y_;
	U8 _pic_sz_[4];
	U8 _dvc_, _ch_;
	U8 _t1_, _t2_;

	_pos_x_ = (U8)(reg_crsr_pos_x>>1);
	_pos_y_ = (U8)(reg_crsr_pos_y>>1);

	if((b_mse_rlse_lb == 0)&&(b_mse_init_drg == 0)){
		if(b_mse_init_scl == 0){
			b_mse_init_scl = 1;
//	if(BitClear(mse_flg,MSE_RLSE_LB)&&BitClear(mse_flg,MSE_INIT_DRG)){
//		if(BitClear(mse_flg,MSE_INIT_SCL)){
//			SetBit(mse_flg,MSE_INIT_SCL);

			if(b_mse_stts_pth == MSE_PTH_X)	b_mse_stts_free = ON;
			if(b_mse_rlse_rb == 0)	b_mse_scl_bnd_only = 1;
//			if(BitClear(mse_flg,MSE_STTS_PTH))	SetBit(mse_flg,MSE_STTS_FREE);
//			if(BitClear(mse_flg,MSE_RLSE_RB))	SetBit(mse_flg,MSE_SCL_BND_ONLY);

			mse_ch_ptr = mse_ch & 0x0f;

//				for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
////					SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
////					if(_t1_ == mse_ch_ptr){
////						mse_ch_scl = ReadAsicByte(_dvc_,DVC_PG1,REG_FUNC_MODE_X+(_ch_<<3)) & DEC_PTH;
//					if(_t1_ == mse_ch_ptr){
//						mse_ch_scl = ReadAsicByte(MASTER,DVC_PG1,0x10+(_t1_<<3)) & 0x03;	//... DEC_PTH
//
///*		//... display channel number
//						_t1_ = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<3));
//						_t1_ = (_t1_+2)>>2;
//						_t2_ = ReadAsicByte(_dvc_,DVC_PG1,0x32+(_ch_<<3));
//						_t2_ = (_t2_+3)/5;
//						SetMseFreeOSDCh(_t1_,_t2_,mse_ch_ptr);
//						//	*/
//					}
//				}

			mse_ch_scl = mse_ch_ptr;
		}

		if(b_mse_stts_free == ON){
//		if(BitSet(mse_flg,MSE_STTS_FREE)){
			if(mse_btn_pshng_cnt == MSE_BTN_DLY_CLK){
				for(_t1_=0;_t1_<cmn_max_ch;_t1_++){
						SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
					if(_t1_ == mse_ch_ptr){
//						if(_t1_ < 4){
////							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,ON);
////							SetChCtrl(PTH_X,_dvc_,_ch_,FREEZE|ENHANCE|BLINK,OFF);
							SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,BIT6);				//... enable popup
							SetAsicFlgType(_dvc_,DVC_PG1,0x11+(_ch_<<3),BIT6|BIT3|BIT0,0x00);	//... disable freeze,enhance,blink
//						}
//						else{
//							_t2_ = _t1_&0x03;
////							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,ON);
////							SetChCtrl(PTH_X,_dvc_,_ch_,FREEZE|ENHANCE|BLINK,OFF);
//							SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t2_<<3),BIT6,BIT6);				//... enable popup
//							SetAsicFlgType(MASTER,DVC_PG1,0x14+(_t2_<<3),BIT6|BIT3|BIT0,0x00);	//... disable freeze,enhance,blink
//						}
					}
					else{
//						if(_t1_ < 4){
////							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,OFF);
							SetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT6,0x00);				//... disable popup
//						}
//						else{
//							_t2_ = _t1_&0x03;
////							SetChFuncMode(PTH_X,_dvc_,_ch_,POP_UP,OFF);
//							SetAsicFlgType(MASTER,DVC_PG1,0x13+(_t2_<<3),BIT6,0x00);				//... disable popup
//						}
					}
				}
			}

			SetCrntDvcCh(mse_ch_ptr,&_dvc_,&_ch_);
			for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);
//			for(_t1_=0;_t1_<4;_t1_++)
//				_pic_sz_[_t1_] = ReadAsicByte(MASTER,DVC_PG1,0x30+(mse_ch_ptr<<2)+_t1_);


//			IfBitSet(mse_ch,BIT7){
			if(BitSet(mse_ch,BIT7)){
				if(_pos_x_ <= _pic_sz_[1]){
//					if(GetAsicFlag(_dvc_,DVC_PG0,0x38,BIT0<<_ch_)){				//... when enable pb
					if(GetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT2)){				//... PB_PATH_EN=1
//#if	defined(__4CH__)
//						if((b_jp_4pb == OFF)&&(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF))||(glbl_dspl_pb_inpt > PB_INPT_MUX)))//{
//#elif	defined(__16CH__)
//						if(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF))||(glbl_dspl_pb_inpt > PB_INPT_MUX))//{
//#endif
						if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
							if(_pos_x_ >= (_pic_sz_[1]-90)){
								_pic_sz_[0] = _pos_x_;
							}
							else{
								_pic_sz_[0] = _pic_sz_[1]-90;
							}
							SetMseFreeSclH(_dvc_,_ch_,0x30,_pic_sz_[0],_pic_sz_[1],1);
							WriteAsicByte(_dvc_,DVC_PG2,0x62+(_ch_<<3),_pic_sz_[0]+1);
						}
						else{
							_pic_sz_[0] = _pos_x_;

							SetMseFreeSclH(_dvc_,_ch_,0x30,_pic_sz_[0],_pic_sz_[1],0);
							WriteAsicByte(_dvc_,DVC_PG2,0x62+(_ch_<<3),_pic_sz_[0]+1);
						}
					}
					else{
						_pic_sz_[0] = _pos_x_;

						SetMseFreeSclH(_dvc_,_ch_,0x30,_pic_sz_[0],_pic_sz_[1],0);
						//... 2d box setting
						WriteAsicByte(_dvc_,DVC_PG2,0x62+(_ch_<<3),_pic_sz_[0]+1);

					}
				}
				else{
					TglMseChMrr(_dvc_,_ch_,BIT5);
					_pic_sz_[0] = _pic_sz_[1];
					mse_ch ^= BIT7|BIT6;
//					WriteAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2),_pic_sz_[0]);
					WriteAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2),_pic_sz_[0]);
					//... 2d box setting
					WriteAsicByte(_dvc_,DVC_PG2,0x62+(_ch_<<3),_pic_sz_[0]+1);
				}
			}
//			else IfBitSet(mse_ch,BIT6){
			else if(BitSet(mse_ch,BIT6)){
				if(_pos_x_ >= _pic_sz_[0]){
//					if(GetAsicFlag(_dvc_,DVC_PG0,0x38,BIT0<<_ch_)){				//... when enable pb
					if(GetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT2)){				//... when enable pb
//#if	defined(__4CH__)
//						if((b_jp_4pb == OFF)&&(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF))||(glbl_dspl_pb_inpt > PB_INPT_MUX)))//{
//#elif	defined(__16CH__)
//						if(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF))||(glbl_dspl_pb_inpt > PB_INPT_MUX))//{
//#endif
						if((wndw_rc_fm_type == CIF_TYPE_FR)||(wndw_rc_fm_type == CIF_TYPE_FL)){
							if(_pos_x_ <= (_pic_sz_[0]+90)){
								_pic_sz_[1] = _pos_x_;
							}
							else{
								_pic_sz_[1] = _pic_sz_[0]+90;
							}
							SetMseFreeSclH(_dvc_,_ch_,0x31,_pic_sz_[0],_pic_sz_[1],1);
						}
						else{
							_pic_sz_[1] = _pos_x_;

							SetMseFreeSclH(_dvc_,_ch_,0x31,_pic_sz_[0],_pic_sz_[1],0);
						}
					}
					else{
						_pic_sz_[1] = _pos_x_;

						SetMseFreeSclH(_dvc_,_ch_,0x31,_pic_sz_[0],_pic_sz_[1],0);
					}
				}
				else{
					TglMseChMrr(_dvc_,_ch_,BIT5);
					_pic_sz_[1] = _pic_sz_[0];
					mse_ch ^= BIT7|BIT6;
					WriteAsicByte(_dvc_,DVC_PG1,0x31+(_ch_<<2),_pic_sz_[1]);
				}
			}

//			IfBitSet(mse_ch,BIT5){
			if(BitSet(mse_ch,BIT5)){
				if(_pos_y_ <= _pic_sz_[3]){
//					if(GetAsicFlag(_dvc_,DVC_PG0,0x38,BIT0<<_ch_)){				//... when enable pb
					if(GetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT2)){				//... when enable pb
//#if	defined(__4CH__)
//						if((((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FR))||
//							(glbl_dspl_pb_inpt == PB_CIF_FRM))&&(b_jp_4pb == OFF))//{
//#elif	defined(__16CH__)
//						if(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FR))||
//							(glbl_dspl_pb_inpt == PB_CIF_FRM))//{
//#endif
						if(wndw_rc_fm_type == CIF_TYPE_FR){
							//... when record out is quad size( 360x120 or 360x144 )
							if(_pos_y_ >= (_pic_sz_[3]-(cmn_bot>>1))){
								_pic_sz_[2] = _pos_y_;
							}
							else{
								_pic_sz_[2] = _pic_sz_[3]-(cmn_bot>>1);
							}
							SetMseFreeSclV(_dvc_,_ch_,0x32,_pic_sz_[2],_pic_sz_[3]);
							WriteAsicByte(_dvc_,DVC_PG2,0x64+(_ch_<<3),_pic_sz_[2]);
						}
						else{
							_pic_sz_[2] = _pos_y_;

							SetMseFreeSclV(_dvc_,_ch_,0x32,_pic_sz_[2],_pic_sz_[3]);
							WriteAsicByte(_dvc_,DVC_PG2,0x64+(_ch_<<3),_pic_sz_[2]);

						}
					}
					else{
						_pic_sz_[2] = _pos_y_;

						SetMseFreeSclV(_dvc_,_ch_,0x32,_pic_sz_[2],_pic_sz_[3]);
						//... 2d box setting
						WriteAsicByte(_dvc_,DVC_PG2,0x64+(_ch_<<3),_pic_sz_[2]);

					}
				}
				else{
					TglMseChMrr(_dvc_,_ch_,BIT4);
					_pic_sz_[2] = _pic_sz_[3];
					mse_ch ^= BIT5|BIT4;
					WriteAsicByte(_dvc_,DVC_PG1,0x32+(_ch_<<2),_pic_sz_[2]);
					//... 2d box setting
					WriteAsicByte(_dvc_,DVC_PG2,0x64+(_ch_<<3),_pic_sz_[2]);
				}
			}
//			else IfBitSet(mse_ch,BIT4){
			else if(BitSet(mse_ch,BIT4)){
				if(_pos_y_ >= _pic_sz_[2]){
//					if(GetAsicFlag(_dvc_,DVC_PG0,0x38,BIT0<<_ch_)){				//... when enable pb
					if(GetAsicFlgType(_dvc_,DVC_PG1,0x10+(_ch_<<3),BIT2)){				//... when enable pb
//#if	defined(__4CH__)
//						if((((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FR))||
//							(glbl_dspl_pb_inpt == PB_CIF_FRM))&&(b_jp_4pb == OFF))//{
//#elif	defined(__16CH__)
//						if(((glbl_dspl_pb_inpt == PB_INPT_REC)&&(b_rec_fmt_type == FMT_TYPE_CIF)&&(glbl_rec_fmt_cif_type == CIF_TYPE_FR))||
//							(glbl_dspl_pb_inpt == PB_CIF_FRM))//{
//#endif
						if(wndw_rc_fm_type == CIF_TYPE_FR){
							//... when record out is quad size( 360x120 or 360x144 )
							if(_pos_y_ <= (_pic_sz_[2]+(cmn_bot>>1))){
								_pic_sz_[3] = _pos_y_;
							}
							else{
								_pic_sz_[3] = _pic_sz_[2]+(cmn_bot>>1);
							}
							SetMseFreeSclV(_dvc_,_ch_,0x33,_pic_sz_[2],_pic_sz_[3]);
						}
						else{
							_pic_sz_[3] = _pos_y_;

							SetMseFreeSclV(_dvc_,_ch_,0x33,_pic_sz_[2],_pic_sz_[3]);
						}
					}
					else{
						_pic_sz_[3] = _pos_y_;

						SetMseFreeSclV(_dvc_,_ch_,0x33,_pic_sz_[2],_pic_sz_[3]);
					}
				}
				else{
					TglMseChMrr(_dvc_,_ch_,BIT4);
					_pic_sz_[3] = _pic_sz_[2];
					mse_ch ^= BIT5|BIT4;
					WriteAsicByte(_dvc_,DVC_PG1,0x33+(_ch_<<2),_pic_sz_[3]);
				}
			}


/*		//... display channel boundary value
			if(b_jp_video == NTSC)	_t2_ = 20;
			else	_t2_ = 24;
			for(_t1_=0;_t1_<4;_t1_++)	DisplayValue(PTH_X,1+3*_t1_,_t2_,0xff,_pic_sz_[_t1_],2);
			//	*/

/*		//... display channel number
			if(b_mse_no_moving == OFF){
				_t1_ = (_pic_sz_[0]+2)>>2;
				_t2_ = (_pic_sz_[2]+3)/5;
				SetMseFreeOSDCh(_t1_,_t2_,mse_ch_ptr);
			}
			//	*/

//*		//... adaptive motion detection cell
			_t1_ = (_pic_sz_[1]-_pic_sz_[0]-1)>>3;
			if(_t1_ > 1)	_t2_ = _t1_ - 1;
			else	_t2_ = 0;
			WriteAsicByte(_dvc_,DVC_PG2,0x63+((_ch_&0x3)<<3),_t2_);
			//... (HW + 1(md_bnd))*(16 / 2(resol)) = HR - HL - 1(ch_bnd) - pixel_offset(full:4,quad:2,pip:1... -> not contain)
			_t1_ = (_pic_sz_[3]-_pic_sz_[2])/6;
			if(_t1_ > 1)	_t2_ = _t1_ - 1;
			else	_t2_ = 0;
			WriteAsicByte(_dvc_,DVC_PG2,0x65+((_ch_&0x3)<<3),_t2_);
		//	*/
		}
	}
	else{
		if(b_mse_init_scl == 1){
			b_mse_init_scl = 0;
			b_mse_scl_bnd_only = 0;
			b_mse_stts_free = OFF;
//		if(BitSet(mse_flg,MSE_INIT_SCL)){
//			ClearBit(mse_flg,MSE_INIT_SCL);
//			ClearBit(mse_flg,MSE_SCL_BND_ONLY);
//			ClearBit(mse_flg,MSE_STTS_FREE);

/*		//... display channel boundary value
			if(b_jp_video == NTSC)	_t2_ = 20;
			else	_t2_ = 24;
			ClearOSD(PTH_X,0,_t2_,45,1);
			//	*/

/*			//... magnetic effect
						SetCrntDvcCh(mse_ch_ptr,&_dvc_,&_ch_);
			_t1_ = ReadAsicByte(_dvc_,DVC_PG2,REG_2DBOX_CTL+(_ch_<<3));
			IfBitSet(_t1_,_2DBOX_EN_X){		//... if motion detection on, fitting at the button-rlse moment after free scaling
				for(_t1_=0;_t1_<4;_t1_++)	_pic_sz_[_t1_] = ReadAsicByte(_dvc_,DVC_PG1,0x30+(_ch_<<2)+_t1_);

				t_2dbox_w = ReadAsicByte(_dvc_,DVC_PG2,REG_2DBOX_HW+(_ch_<<3));
				_pic_sz_[1] = _pic_sz_[0]+((t_2dbox_w+1)<<3)+1;
				WriteAsicByte(_dvc_,DVC_PG1,0x31+(_ch_<<3),_pic_sz_[1]);
//				if(mse_ch_scl == 4)	SetHScalePB(ary_picxx[mse_ch_ptr][1]-ary_picxx[mse_ch_ptr][0]);
//				else	SetHScale(PTH_X,mse_ch_scl,(ary_picxx[mse_ch_ptr][1]-ary_picxx[mse_ch_ptr][0])*0x016c);	// 0x016c(65536/180=364.09)
				if(b_mse_rlse_rb == 1)	SetHScale(PTH_X,_dvc_,mse_ch_scl,(_pic_sz_[1]-_pic_sz_[0])*0x016c);	// 0x016c(65536/180=364.09)

				t_2dbox_w = ReadAsicByte(_dvc_,DVC_PG2,REG_2DBOX_VW+(_ch_<<3));
				_pic_sz_[3] = _pic_sz_[2]+((t_2dbox_w+1)*6);
				WriteAsicByte(_dvc_,DVC_PG1,0x33+(_ch_<<3),_pic_sz_[3]);
//				if(mse_ch_scl == 4)	SetVScalePB(ary_picxx[mse_ch_ptr][3]-ary_picxx[mse_ch_ptr][2]);
//				else	SetVScale(PTH_X,mse_ch_scl,(ary_picxx[mse_ch_ptr][3]-ary_picxx[mse_ch_ptr][2])*_scl_v_unt_);
				if(b_mse_rlse_rb == 1)	SetVScale(PTH_X,_dvc_,mse_ch_scl,(_pic_sz_[3]-_pic_sz_[2])*_scl_v_unt_);
			}
			//	*/
		}
	}
}
//==================================================================================
