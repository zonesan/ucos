#include	"hdr.h"
#include	"cmn.h"
#include	"prtcl.h"
#include	"wndw_rc_fmt.h"
#include	"osd.h"
#include	"wndw_ds_scrn.h"

#if defined(__4CH__)
//==================================================================================
//
//==================================================================================
U8	run_8ch_cnt_ana_sw;
U8	run_8ch_cnt_mux;
U8	run_8ch_flg_edg[4];


//==================================================================================
//
//==================================================================================
void	Run8ChFast(void)
{
	U8 _t1_, _t2_, _t3_;
//	U16 _dly_;
	
	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x04);
	for(_t3_=0;_t3_<4;_t3_++){
		_t2_ = ReadAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_);
		if(BitClear(_t2_,BIT3)){																	//... input A
			if(BitClear(_t1_,BIT0<<_t3_)){														//... check strobe done
//				DLY_FOR(DLY_SW_STRB);
				_t2_ |= BIT3;																			//... change to input B
				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_);				//... analog switch
				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_|BIT2);		//... soft reset
//				WriteAsicByte(MASTER,DVC_PG1,0x04,_t1_|(BIT4<<_t3_));			//... strobe on opposite ch
				WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT4<<_t3_));			//... strobe on opposite ch
//				WriteAsicByte(MASTER,DVC_PG1,0x54,(BIT0<<_t3_));					//... strobe on opposite ch
				
				if(_t3_ == 0)	run_8ch_cnt_ana_sw++;
			}
		}
		else{																								//... input B
			if(BitClear(_t1_,BIT4<<_t3_)){														//... check strobe done
//				DLY_FOR(DLY_SW_STRB);
				_t2_ &= ~BIT3;																			//... change to input A
				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_);				//... analog switch
				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_|BIT2);		//... soft reset
//				WriteAsicByte(MASTER,DVC_PG1,0x04,_t1_|(BIT0<<_t3_));			//... strobe on opposite ch
				WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT0<<_t3_));			//... strobe on opposite ch
//				WriteAsicByte(MASTER,DVC_PG1,0x54,(BIT0<<_t3_));					//... strobe on opposite ch
				
				if(_t3_ == 0)	run_8ch_cnt_ana_sw++;
			}
		}
	}
}
//==================================================================================
//void	Run8ChCif(void)
//{
//	U8 _t1_, _t2_, _t3_;
//	U16 _dly_;
//	
//	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x54);
//	for(_t3_=0;_t3_<4;_t3_++){
//		if(BitClear(_t1_,BIT0<<_t3_)){														//... check strobe done
//			DLY_FOR(DLY_SW_STRB);
//			_t2_ = ReadAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_)^BIT3;
//			WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_);				//... analog switch
//			WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_|BIT2);		//... soft reset
//			if(b_ch_src == CH_SRC_LV){
//				if(BitClear(_t2_,BIT3))																	//... input A
//					WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT0<<_t3_));			//... strobe on opposite ch
//				else																								//... input B
//					WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT4<<_t3_));			//... strobe on opposite ch
//			}
//			WriteAsicByte(MASTER,DVC_PG1,0x54,(_t1_&0xf0)|(BIT0<<_t3_));					//... strobe on opposite ch
//			
//			if(_t3_ == 0)	run_8ch_cnt_ana_sw++;
//		}
//	}
//}
//==================================================================================
void	Run8ChCif(void)
{
	U8 _t1_, _t2_, _t3_;
	U16 _dly_;
	
	_t1_ = ReadAsicByte(MASTER,DVC_PG1,0x54);
	if(BitClear(_t1_,0x0f)){														//... check strobe done

		if(wndw_rc_fm_type == CIF_TYPE_FL)
			DLY_FOR(DLY_SW_STRB);												//... about 1 field when non-realtime and CIF record

		for(_t3_=0;_t3_<4;_t3_++){
			_t2_ = ReadAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_)^BIT3;
			WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_);				//... analog switch
			WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t3_,_t2_|BIT2);		//... soft reset
			if(b_ch_src == CH_SRC_LV){
				if(BitClear(_t2_,BIT3))																	//... input A
					WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT0<<_t3_));			//... strobe on opposite ch
				else																								//... input B
					WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT4<<_t3_));			//... strobe on opposite ch
			}
			WriteAsicByte(MASTER,DVC_PG1,0x54,(_t1_&0xf0)|(BIT0<<_t3_));
			
			if(_t3_ == 0)	run_8ch_cnt_ana_sw++;
		}
//		WriteAsicByte(MASTER,DVC_PG1,0x54,(_t1_&0xf0)|0x0f);
	}
}
//==================================================================================
void	Run8ChMux(void)
//... frame by frame
//... Auto strobe pseudo 8 ch & Mux
//... use mux interrupt, not use que & internal trigger mode(period:01h).
//... FLDENC signal interrupt
{
	U8 _t1_, _t2_;//, _t3_;
	
	if(run_8ch_cnt_mux < 3)	run_8ch_cnt_mux++;
	else	run_8ch_cnt_mux = 0;
	
	if(run_8ch_cnt_mux == 0)	run_8ch_cnt_ana_sw++;

	if(run_8ch_cnt_mux == 0)		_t1_ = 2;
	else if(run_8ch_cnt_mux == 1)	_t1_ = 3;
	else if(run_8ch_cnt_mux == 2)	_t1_ = 0;
	else	_t1_ = 1;

	_t2_ = ReadAsicByte(MASTER,DVC_PG1,0x59);
	WriteAsicByte(MASTER,DVC_PG1,0x59,(_t2_&0xf0)|BIT4|run_8ch_cnt_mux);	//... mux interrupt
	_t2_ = ReadAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_)^BIT3;
	WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_,_t2_);								//... ANA_SW
	WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_,_t2_|BIT2);							//... SW_RESET

	if(b_ch_src == CH_SRC_LV){
//		_t3_ = ReadAsicByte(MASTER,DVC_PG1,REG_FUNC_MODE_X+(_t1_<<3));// ^ DMCH_PATH;
//		IfBitSet(_t2_,BIT2)	SetBit(_t3_,DMCH_PATH);//BIT2);						//... dummy channel
//		else	ClearBit(_t3_,DMCH_PATH);//BIT2);													//... main channel
//		WriteAsicByte(MASTER,DVC_PG1,REG_FUNC_MODE_X+(_t1_<<3),_t3_);
//
//		_t1_ = ReadAsicByte(MASTER,DVC_PG1,REG_STRB_CTL_X);
//		WriteAsicByte(MASTER,DVC_PG1,REG_STRB_CTL_X,(_t2_&0xf0)|(BIT0<<run_8ch_cnt_mux));						//... SW_RESET|ANA_SW
		if(BitClear(_t2_,BIT3))																	//... input A
			WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT0<<run_8ch_cnt_mux));			//... strobe on opposite ch
		else																								//... input B
			WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT4<<run_8ch_cnt_mux));			//... strobe on opposite ch
	}
}
////... field by field with MPPDEC's strobe signal
//{
//	U8 _t1_, _t2_, _t3_;//, _t4_;
//	
//	for(_t1_=0;_t1_<4;_t1_++){
//		_t2_ = (P2&(BIT0<<_t1_))>>_t1_;
//		if(_t2_ != (run_8ch_flg_edg[_t1_]&BIT0)){
//			run_8ch_flg_edg[_t1_] = ((run_8ch_flg_edg[_t1_]<<1)|_t2_)&0x07;
//			if(run_8ch_flg_edg[_t1_] == 0x02){										//... 0x02 of 0x07 means low -> high -> low : falling edge detected.
//
//				if(_t1_ == 0)	run_8ch_cnt_ana_sw++;
//
//				_t3_ = ReadAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_)^BIT3;
//				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_,_t3_);						//... ANA_SW
//				WriteAsicByte(MASTER,DVC_PG0,0x0d+0x10*_t1_,_t3_|BIT2);					//... SW_RESET
//
//				if(b_ch_src == CH_SRC_LV){
//					if(BitClear(_t3_,BIT3))																	//... input A
//						WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT0<<_t1_));			//... strobe on opposite ch
//					else																								//... input B
//						WriteAsicByte(MASTER,DVC_PG1,0x04,(BIT4<<_t1_));			//... strobe on opposite ch
//				}
//			}
//		}
//	}
//}
//==================================================================================
void	Run8ChOSDRtoNum(void)
{
	DsplyOSDDatHexY(MASTER,WNDW_DS_SC_CNT_X,WNDW_DS_SC_CNT_Y,0xff,ConvDec2Bcd(run_8ch_cnt_ana_sw));				//... display total switching count every 1 VS
	run_8ch_cnt_ana_sw = 0;
}
//==================================================================================
void	Run8ChOSDRto(void)
{
	WriteOSDStrY(MASTER,WNDW_DS_SC_UNIT_X,WNDW_DS_SC_CNT_Y,0x00,str_wndw_ds_fld_sec);
	Run8ChOSDRtoNum();
}
//==================================================================================

#endif

