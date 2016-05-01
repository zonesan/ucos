//==================================================================================
//						page0 initialize table description
//==================================================================================

extern  unsigned char	tbl_ntsc_pg0_cmn[];
extern  unsigned char	tbl_ntsc_pg0_sfr1[];
extern  unsigned char	tbl_ntsc_pg0_sfr2[];
extern  unsigned char	tbl_ntsc_pg0_scl[];
extern  unsigned char	tbl_ntsc_pg0_sfr3[];
extern  unsigned char	tbl_pal_pg0_cmn[];
extern  unsigned char	tbl_pal_pg0_sfr1[];
extern  unsigned char	tbl_pal_pg0_sfr2[];
extern  unsigned char	tbl_pal_pg0_scl[];
extern  unsigned char	tbl_pal_pg0_sfr3[];

//==================================================================================
//						Page1 initialize table description
//==================================================================================
extern  unsigned char	tbl_pg1_x_cmn[];
extern  unsigned char	tbl_ntsc_pg1_pic_fl[];
extern  unsigned char	tbl_ntsc_pg1_pic_qd[];
extern  unsigned char	tbl_pal_pg1_pic_fl[];
extern  unsigned char	tbl_pal_pg1_pic_qd[];
#if defined(__4CH__)
	extern  unsigned char	tbl_ntsc_pg1_pic_9_lt[];
	extern  unsigned char	tbl_ntsc_pg1_pic_9_rb[];
	extern  unsigned char	tbl_ntsc_pg1_enc[];
	extern  unsigned char	tbl_pal_pg1_pic_9_lt[];
	extern  unsigned char	tbl_pal_pg1_pic_9_rb[];
	extern  unsigned char	tbl_pal_pg1_enc[];
#elif defined(__16CH__)
	extern  unsigned char	tbl_ntsc_pg1_pic_16_0[];
	extern  unsigned char	tbl_ntsc_pg1_pic_16_1[];
	extern  unsigned char	tbl_ntsc_pg1_pic_16_2[];
	extern  unsigned char	tbl_ntsc_pg1_pic_16_3[];
	extern  unsigned char	tbl_ntsc_pg1_enc_mstr[];
	extern  unsigned char	tbl_ntsc_pg1_enc_slv[];
	extern  unsigned char	tbl_pal_pg1_pic_16_0[];
	extern  unsigned char	tbl_pal_pg1_pic_16_1[];
	extern  unsigned char	tbl_pal_pg1_pic_16_2[];
	extern  unsigned char	tbl_pal_pg1_pic_16_3[];
	extern  unsigned char	tbl_pal_pg1_enc_mstr[];
	extern  unsigned char	tbl_pal_pg1_enc_slv[];
#endif
extern  unsigned char	tbl_pg1_y_cmn[];

//==================================================================================
//						Page2 initialize table description
//==================================================================================

extern  unsigned char	tbl_pg2_mse_box[];
extern  unsigned char	tbl_ntsc_pg2_2dbox[];
extern  unsigned char	tbl_ntsc_pg2_mtn[];

extern  unsigned char	tbl_pal_pg2_2dbox[];
extern  unsigned char	tbl_pal_pg2_mtn[];
