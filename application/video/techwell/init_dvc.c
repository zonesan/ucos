#include	"hdr.h"
#include	"cmn.h"
#include	"prtcl.h"
#include	"tbl_tw2835.h"
#include	"osd.h"

//#include	"bmpMainMenu.h"


U16	reg_crsr_pos_x;
U16	reg_crsr_pos_y;

//==================================================================================
//						Device init function description
//==================================================================================

 unsigned char color_init_index[]={
29,	// used color
//	Y,	Cb,	Cr,	addr
0x7d,	0x80,	0x80,	0x80,  
//0x00,	0x80,	0x80,	0x81, //for  test 
0x10,	0x80,	0x80,	0x81, 
0x1c,	0xb8,	0x76,	0x82,
0x28,	0xef,	0x6d,	0x83,
0x50,	0x5a,	0x50,	0x84,
0x90,	0x35,	0x22,	0x85,
0x5d,	0x92,	0x47,	0x86,
0xb4,	0x80,	0x80,	0x87,
0xa9,	0xa5,	0x10,	0x88,
0xf0,	0x80,	0x80,	0x89, //white
0x71,	0x47,	0x89,	0x8a,
0xd2,	0x10,	0x92,	0x8b,
0x51,	0x5a,	0xef,	0x8c,
0x30,	0x6d,	0xb8,	0x8d,
0x3d,	0xa5,	0xaf,	0x8e,

0x00,0x80,0x80,0x8f,
0xf0,0x80,0x80,0x90,
0x1d,0xff,0x6b,0x91,
0x4c,0x54,0xff,0x92,
0x95,0x4f,0xb5,0x93,
0xb2,0xaa,0x00,0x94,
0x69,0xd4,0xea,0x95,
0x4b,0x55,0x4a,0x96,
0x61,0xb5,0x3a,0x97,
0xe1,0x00,0x94,0x98,

0xf0,0x80,0x80,0xc0,
0x4c,0x54,0xff,0xc1,
0xe1,0x00,0x94,0xc2,
0x4b,0x55,0x4a,0xc3

};

void InitPg0(void)
{
	U8 _t1_;

	if(b_cmn_jp_vdo == VDO_NTSC){
		for(_t1_=0;_t1_<4;_t1_++){
			WriteAsicTable(cmn_dvc,DVC_PG0,0x00+0x10*_t1_,tbl_ntsc_pg0_cmn,15);
			WriteAsicByte(cmn_dvc,DVC_PG0,0x0c+0x10*_t1_,(_t1_<<6)|0x06);			//... x path ANA_CH mux
			WriteAsicTable(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,tbl_ntsc_pg0_scl,16);
#if defined(__4CH__)
			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x01);			//... x path scale filter : quad
//			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x06);			//... x path scale filter : 1/3 size
#elif defined(__16CH__)
			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x0b);			//... x path scale filter : 1/4
#endif
			WriteAsicByte(cmn_dvc,DVC_PG0,0x8a+0x10*_t1_,(_t1_<<6)|0x31);			//... y path scale filter : quad
		}

		WriteAsicTable(cmn_dvc,DVC_PG0,0x40,tbl_ntsc_pg0_sfr1,21);
		WriteAsicTable(cmn_dvc,DVC_PG0,0x60,tbl_ntsc_pg0_sfr2,21);
		WriteAsicTable(cmn_dvc,DVC_PG0,0xc0,tbl_ntsc_pg0_sfr3,11);
	}
	else{										//... PAL
		for(_t1_=0;_t1_<4;_t1_++){
			WriteAsicTable(cmn_dvc,DVC_PG0,0x00+0x10*_t1_,tbl_pal_pg0_cmn,15);
			WriteAsicByte(cmn_dvc,DVC_PG0,0x0c+0x10*_t1_,(_t1_<<6)|0x06);			//... x path ANA_CH mux
			WriteAsicTable(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,tbl_pal_pg0_scl,16);
#if defined(__4CH__)
			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x01);			//... x path scale filter : quad
//			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x06);			//... x path scale filter : 1/3 size
#elif defined(__16CH__)
			WriteAsicByte(cmn_dvc,DVC_PG0,0x80+0x10*_t1_,(_t1_<<6)|0x0b);			//... x path scale filter : 1/4
#endif
			WriteAsicByte(cmn_dvc,DVC_PG0,0x8a+0x10*_t1_,(_t1_<<6)|0x31);       //... y path scale filter : quad
		}

		WriteAsicTable(cmn_dvc,DVC_PG0,0x40,tbl_pal_pg0_sfr1,21);
		WriteAsicTable(cmn_dvc,DVC_PG0,0x60,tbl_pal_pg0_sfr2,21);
		WriteAsicTable(cmn_dvc,DVC_PG0,0xc0,tbl_pal_pg0_sfr3,11);
	}
}
//==================================================================================
void InitPg1(void)
{
	U8 _t1_;//, _t2_=0;

#if defined(__4CH__)
	WriteAsicTable(cmn_dvc,DVC_PG1,0x01,tbl_pg1_x_cmn,47);
	WriteAsicTable(cmn_dvc,DVC_PG1,0x50,tbl_pg1_y_cmn,80);
	
	if(b_cmn_jp_vdo == VDO_NTSC){
		WriteAsicTable(cmn_dvc,DVC_PG1,0x30,tbl_ntsc_pg1_pic_qd,16);		//... normal quad
//		WriteAsicTable(cmn_dvc,DVC_PG1,0x30,tbl_ntsc_pg1_pic_9_lt,16);		//... non-realtime
		WriteAsicTable(cmn_dvc,DVC_PG1,0x40,tbl_ntsc_pg1_pic_9_rb,16);
		WriteAsicTable(cmn_dvc,DVC_PG1,0xa0,tbl_ntsc_pg1_enc,16);	
		WriteAsicByte(cmn_dvc,DVC_PG1,0x00,0x00);
	}
	else{
		WriteAsicTable(cmn_dvc,DVC_PG1,0x30,tbl_pal_pg1_pic_qd,16);			//... normal quad
//		WriteAsicTable(cmn_dvc,DVC_PG1,0x30,tbl_pal_pg1_pic_9_lt,16);		//... non-realtime
		WriteAsicTable(cmn_dvc,DVC_PG1,0x40,tbl_pal_pg1_pic_9_rb,16);
		WriteAsicTable(cmn_dvc,DVC_PG1,0xa0,tbl_pal_pg1_enc,16);	
		WriteAsicByte(cmn_dvc,DVC_PG1,0x00,0x80);
	}
#elif defined(__16CH__)
	U8 _dvc_, _ch_;

	WriteAsicTable(cmn_dvc,DVC_PG1,0x01,tbl_pg1_x_cmn,47);
	WriteAsicTable(cmn_dvc,DVC_PG1,0x50,tbl_pg1_y_cmn,80);
	
	if(b_cmn_jp_vdo == VDO_NTSC){
		WriteAsicTable(MASTER,DVC_PG1,0x30,tbl_ntsc_pg1_pic_16_0,16);
		WriteAsicTable(SLAVE1,DVC_PG1,0x30,tbl_ntsc_pg1_pic_16_1,16);
		WriteAsicTable(SLAVE2,DVC_PG1,0x30,tbl_ntsc_pg1_pic_16_2,16);
		WriteAsicTable(SLAVE3,DVC_PG1,0x30,tbl_ntsc_pg1_pic_16_3,16);
//		WriteAsicTable(cmn_dvc,DVC_PG1,0x40,tbl_ntsc_pg1_pic_9_rb,16);

//		WriteAsicTable(cmn_dvc,DVC_PG1,0xa0,tbl_ntsc_pg1_enc,16);	
//		WriteAsicByte(SLAVE1|SLAVE2|SLAVE3,DVC_PG1,0xa1,0x88);
		WriteAsicTable(MASTER,DVC_PG1,0xa0,tbl_ntsc_pg1_enc_mstr,16);	
		WriteAsicTable(SLAVE1|SLAVE2|SLAVE3,DVC_PG1,0xa0,tbl_ntsc_pg1_enc_slv,16);	

		if(cmn_rc_md == RC_MD_120_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x0c);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x0d);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x0e);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0x3f);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
		else if(cmn_rc_md == RC_MD_240_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x0c);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x1d);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x0e);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0x3f);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
		else if(cmn_rc_md == RC_MD_480_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x08);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x09);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x0a);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0x3b);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
	}
	else{
		WriteAsicTable(MASTER,DVC_PG1,0x30,tbl_pal_pg1_pic_16_0,16);
		WriteAsicTable(SLAVE1,DVC_PG1,0x30,tbl_pal_pg1_pic_16_1,16);
		WriteAsicTable(SLAVE2,DVC_PG1,0x30,tbl_pal_pg1_pic_16_2,16);
		WriteAsicTable(SLAVE3,DVC_PG1,0x30,tbl_pal_pg1_pic_16_3,16);
//		WriteAsicTable(cmn_dvc,DVC_PG1,0x40,tbl_pal_pg1_pic_9_rb,16);

//		WriteAsicTable(cmn_dvc,DVC_PG1,0xa0,tbl_pal_pg1_enc,16);	
//		WriteAsicByte(SLAVE1|SLAVE2|SLAVE3,DVC_PG1,0xa1,0x88);
		WriteAsicTable(MASTER,DVC_PG1,0xa0,tbl_pal_pg1_enc_mstr,16);	
		WriteAsicTable(SLAVE1|SLAVE2|SLAVE3,DVC_PG1,0xa0,tbl_pal_pg1_enc_slv,16);	

		if(cmn_rc_md == RC_MD_120_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x8c);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x8d);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x8e);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0xbf);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
		else if(cmn_rc_md == RC_MD_240_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x8c);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x9d);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x8e);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0xbf);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
		else if(cmn_rc_md == RC_MD_480_FPS){
			WriteAsicByte(MASTER,DVC_PG1,0x00,0x88);
			WriteAsicByte(MASTER,DVC_PG1,0x7e,0xc9);
			WriteAsicByte(SLAVE1,DVC_PG1,0x00,0x89);
			WriteAsicByte(SLAVE1,DVC_PG1,0x7e,0xa9);
			WriteAsicByte(SLAVE2,DVC_PG1,0x00,0x8a);
			WriteAsicByte(SLAVE2,DVC_PG1,0x7e,0x89);
			WriteAsicByte(SLAVE3,DVC_PG1,0x00,0xbb);
			WriteAsicByte(SLAVE3,DVC_PG1,0x7e,0x89);
		}
	}
#endif			

	//... queue setting
//	for(_t1_=0;_t1_<16;_t1_++){
//		//... mux queue
//		WriteAsicByte(cmn_dvc,DVC_PG1,0x59,_t1_);			//... queue data
//		WriteAsicByte(cmn_dvc,DVC_PG1,0x5a,0x80|_t1_);		//... queue addr
//		//... popup queue
////		if(_t1_<13){
////			WriteAsicByte(cmn_dvc,DVC_PG1,0x73,(_t1_<<4)|(_t1_+1));			//... queue data
////			WriteAsicByte(cmn_dvc,DVC_PG1,0x74,((_t1_+2)<<4)|(_t1_+3));	//... queue data
////			WriteAsicByte(cmn_dvc,DVC_PG1,0x75,0x80|_t1_);						//... queue addr
////		}
//	}
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x73,0x01);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x74,0x23);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x75,0x80);	//... queue addr
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x73,0x12);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x74,0x30);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x75,0x81);	//... queue addr
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x73,0x23);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x74,0x01);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x75,0x82);	//... queue addr
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x73,0x30);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x74,0x12);	//... queue data
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x75,0x83);	//... queue addr
//	
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x56,0x40);				//... start internal trigger for mux queue

#if	defined(__4CH__)
	for(_t1_=0;_t1_<4;_t1_++){
		WriteAsicByte(cmn_dvc,DVC_PG1,0x59,_t1_);							//... 
		WriteAsicByte(cmn_dvc,DVC_PG1,0x5a,BIT7|_t1_);							//... 
	}
#elif	defined(__16CH__)
//	WriteAsicByte(cmn_dvc,DVC_PG1,0x84,0xc1);									//... invert FLDENCP
//
//	WriteAsicByte(SLAVE1,DVC_PG1,0x8d,0x40);										//... when cascade
//	WriteAsicByte(SLAVE1,DVC_PG1,0x8e,0x06);										//... when cascade
	for(_t1_=0;_t1_<16;_t1_++){
		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
		SetAsicFlgType(_dvc_,DVC_PG1,0x16+(_ch_<<3),0x0f,_t1_);
	}

	if(cmn_rc_md == RC_MD_120_FPS){
		//... mux queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x57,0x0f);							//... 
		for(_t1_=0;_t1_<16;_t1_++){
			WriteAsicByte(SLAVE3,DVC_PG1,0x59,_t1_);						//... 
			WriteAsicByte(SLAVE3,DVC_PG1,0x5a,BIT7|_t1_);				//... 
		}
		//... popup queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x71,0x03);							//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0x01);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0x23);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7);				//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0x45);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0x67);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7|0x01);				//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0x89);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0xab);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7|0x02);				//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0xcd);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0xef);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7|0x03);				//... 
	}
	else if(cmn_rc_md == RC_MD_240_FPS){
		//... mux queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x57,0x07);							//... 
		for(_t1_=0;_t1_<8;_t1_++){
			WriteAsicByte(SLAVE1,DVC_PG1,0x59,_t1_);						//... 
			WriteAsicByte(SLAVE3,DVC_PG1,0x59,_t1_+8);						//... 
			WriteAsicByte(cmn_dvc,DVC_PG1,0x5a,BIT7|_t1_);				//... 
		}
		//... popup queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x71,0x01);							//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x73,0x01);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x74,0x23);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x75,BIT7);				//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x73,0x45);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x74,0x67);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x75,BIT7|0x01);				//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0x89);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0xab);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7);				//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0xcd);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0xef);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x75,BIT7|0x01);				//... 
	}
	else{//if(cmn_rc_md == RC_MD_120_FPS){
		//... mux queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x57,0x03);							//... 
		for(_t1_=0;_t1_<4;_t1_++){
			WriteAsicByte(MASTER,DVC_PG1,0x59,_t1_);						//... 
			WriteAsicByte(SLAVE1,DVC_PG1,0x59,_t1_+4);						//... 
			WriteAsicByte(SLAVE2,DVC_PG1,0x59,_t1_+8);						//... 
			WriteAsicByte(SLAVE3,DVC_PG1,0x59,_t1_+12);						//... 
			WriteAsicByte(cmn_dvc,DVC_PG1,0x5a,BIT7|_t1_);				//... 
		}
		//... popup queue
		WriteAsicByte(cmn_dvc,DVC_PG1,0x71,0x00);							//... 
		WriteAsicByte(MASTER,DVC_PG1,0x73,0x01);						//... 
		WriteAsicByte(MASTER,DVC_PG1,0x74,0x23);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x73,0x45);						//... 
		WriteAsicByte(SLAVE1,DVC_PG1,0x74,0x67);						//... 
		WriteAsicByte(SLAVE2,DVC_PG1,0x73,0x89);						//... 
		WriteAsicByte(SLAVE2,DVC_PG1,0x74,0xab);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x73,0xcd);						//... 
		WriteAsicByte(SLAVE3,DVC_PG1,0x74,0xef);						//... 
		WriteAsicByte(cmn_dvc,DVC_PG1,0x75,BIT7);				//... 
	}

//	for(_t1_=0;_t1_<16;_t1_++){
//		SetCrntDvcCh(_t1_,&_dvc_,&_ch_);
//		if(!(_t1_&0x03)){
//			SetAsicFlgType(_dvc_,DVC_PG1,0x54,0x60,0x00);				//... STRB_FLD=0
//			SetAsicFlgType(_dvc_,DVC_PG1,0x57,BIT7,0x00);				//... STRB_AUTO=0
//			SetAsicFlgType(_dvc_,DVC_PG1,0x71,BIT5,0x00);				//... POPUP_FLD=0
//		}
//		WriteAsicByte(_dvc_,DVC_PG1,0x60+3*_ch_,0x80|_ch_);			//... FUNC_MODE=0
//	}
	WaitCmnFldOd();
	WriteAsicByte(cmn_dvc,DVC_PG1,0x70,0xc0);							//... POS_CTL_EN=1, POS_TRIG_MODE=1
#endif
}
//==================================================================================
//*
void InitPg2(void)
{
	U8 _t1_;

	//... OSD color index
#if 0	
	InitOSDCol();
#else

           HwTW2835SetOSDColLUT(cmn_dvc, 1, color_init_index);
           HwTW2835SetOSDColLUT(cmn_dvc, 2, color_init_index);

#endif

	WriteAsicByte(cmn_dvc,DVC_PG2,0x0f,0x0f);		//... OSD Enable for Display/Capture	
//	WriteAsicByte(cmn_dvc,DVC_PG2,0x0f,0x0c);		//... OSD Enable for Display				---->> X
//	WriteAsicByte(cmn_dvc,DVC_PG2,0x0f,0x0a);		//... OSD Enable for Display/Capture	//... temporal

  
	    WriteOSDClrX(cmn_dvc,0,0,45,31);						//... max line num is 310 line for OSD
#if defined(__16CH__)
	WriteOSDClrX(SLAVE1,0,0,45,31);						//... max line num is 310 line for OSD
	WriteOSDClrX(SLAVE2,0,0,45,31);						//... max line num is 310 line for OSD
	WriteOSDClrX(SLAVE3,0,0,45,31);						//... max line num is 310 line for OSD
#endif

	    WriteOSDClrY(cmn_dvc,0,0,45,31);						//... max line num is 310 line for OSD
#if defined(__16CH__)
	WriteOSDClrY(SLAVE1,0,0,45,31);						//... max line num is 310 line for OSD
	WriteOSDClrY(SLAVE2,0,0,45,31);						//... max line num is 310 line for OSD
	WriteOSDClrY(SLAVE3,0,0,45,31);						//... max line num is 310 line for OSD
#endif

//	WriteAsicByte(cmn_dvc,DVC_PG2,0x10,0xb0);		//... cursor
	WriteAsicByte(cmn_dvc,DVC_PG2,0x10,0xf0);		//... cursor

	//... other ( mouse, single box, 2d box, mask ..)
	WriteAsicTable(cmn_dvc,DVC_PG2,0x10,tbl_pg2_mse_box,80);
	if(b_cmn_jp_vdo == VDO_NTSC){
		WriteAsicTable(cmn_dvc,DVC_PG2,0x60,tbl_ntsc_pg2_2dbox,32);
		for(_t1_=0;_t1_<4;_t1_++)	WriteAsicTable(cmn_dvc,DVC_PG2,0x80+0x20*_t1_,tbl_ntsc_pg2_mtn,32);
	}
	else{
		WriteAsicTable(cmn_dvc,DVC_PG2,0x60,tbl_pal_pg2_2dbox,32);
		for(_t1_=0;_t1_<4;_t1_++)	WriteAsicTable(cmn_dvc,DVC_PG2,0x80+0x20*_t1_,tbl_pal_pg2_mtn,32);
	}
}
//	*/
//==================================================================================
//void	InitReg(void)
//{
////	reg_crsr_ctrl = ReadAsicByte(MASTER,DVC_PG2,0x10);
//}
//==================================================================================
/*
void InitEncExt(void)
{
//	if(BitClear(cmn_flg,CMN_JP_VDO)){
//		WriteI2CTable(I2C_ID_7121_D,0x26,tbl_ntsc_saa7121_1,4);
//		WriteI2CTable(I2C_ID_7121_D,0x5a,tbl_ntsc_saa7121_2,38);
//	}
//	else{
//		WriteI2CTable(I2C_ID_7121_D,0x26,tbl_pal_saa7121_1,9);
//		WriteI2CTable(I2C_ID_7121_D,0x5a,tbl_pal_saa7121_2,38);
//	}
//
//	WriteI2CByte(I2C_ID_7121_D,0x3a,0x13);	// slave mode, CBGEN(BIT7 1:on, 0:off)
//	WriteI2CByte(I2C_ID_7121_D,0x6b,0x12);






//	if(b_cmn_jp_vdo == NTSC){
//		WriteI2CTable(KS0123,0x00,tbl_ntsc_ks0123,17);
//	}
//	else{
//		WriteI2CTable(KS0123,0x00,tbl_pal_ks0123,17);
//	}
}
//	*/
//==================================================================================
/*
void InitDecPB(void)
{
	if(b_jp_video == NTSC){
		WriteI2CTable(TW9901,0x02,tbl_ntsc_tw9901,53);
	}
	else{
		WriteI2CTable(TW9901,0x02,tbl_pal_tw9901,53);
	}
	WriteI2CByte(TW9901,0x06,0x80);	// soft reset
}
//==================================================================================
//	*/
