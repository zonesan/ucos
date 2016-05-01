//==================================================================================
//						OSD definition & variable description
//==================================================================================
#define	OSD_RDO_BLK				0x04//(0x68)
#define	OSD_RDO_CHK				0x05//(0x69)
#define	OSD_CHK_BOX_BLK		0x06//(0x6a)
#define	OSD_CHK_BOX_CHK		0x07//(0x6b)

#define	OSD_R_UPPER				('R'-0x20)
#define	OSD_W_UPPER				('W'-0x20)
#define	OSD_W_SML				(0x57)//(0x63)//
#define	OSD_B_LOWER				('b'-0x20)
#define	OSD_H_LOWER				('h'-0x20)
#define	OSD_PLUS_SML			('+'-0x20)
#define	OSD_PLUS_BIG			(0x61)
#define	OSD_MINUS_SML			('-'-0x20)
#define	OSD_MINUS_BIG			(0x62)

#define	OSD_ARW_L_BIG			(0x64)
#define	OSD_ARW_R_BIG			(0x65)
#define	OSD_ARW_L_SML			(0x66)
#define	OSD_ARW_R_SML			(0x67)

#define	OSD_ARW_U_BIG			(0x6c)
#define	OSD_ARW_D_BIG			(0x6d)
#define	OSD_ARW_U_SML			(0x6e)
#define	OSD_ARW_D_SML			(0x6f)

#define	OSD_X_SMALL				('x'-0x20)		//... (= '\') -> 'x'

#define	OSD_QUIT					(0x60)
#define	OSD_DD					(':'-0x20)

#define	OSD_GRP_LE				(0x3b)
#define	OSD_GRP_LH				(0x3c)
#define	OSD_GRP_RE				(0x3d)
#define	OSD_GRP_LT				(0x5b)
#define	OSD_GRP_RT				(0x5c)
#define	OSD_GRP_LB				(0x5d)
#define	OSD_GRP_RB				(0x5e)
#define	OSD_GRP_LV				(0x5f)

//... tbl_osd_nr_typ
#define	OSD_SQR_CHK				(0x00)
#define	OSD_SQR_BLK				(0x01)




#define	OSD_COL_BLK			1
#define	OSD_COL_50WHT		2
#define	OSD_COL_75WHT		3
#define	OSD_COL_25WHT		4
#define	OSD_COL_100WHT		5
#define	OSD_COL_BLU			6
#define	OSD_COL_RED			7
#define	OSD_COL_ORNG		8
#define	OSD_COL_CYN			9
#define	OSD_COL_MGN			10
#define	OSD_COL_GRN			11
#define	OSD_COL_OCN			12
#define	OSD_COL_YLW			13

#define	OSD_DLY_V			0

#define	OSD_CLR				(0xff)
#define	OSD_BLK				BIT6
#define	OSD_MIX				BIT7

#define	OSD_POS_V_ODD		0
#define	OSD_POS_V_EVEN		BIT9

#define	OSD_PTH_POS_X		2
#define	OSD_PTH_POS_Y		1

#define	OSD_TBL_V_SZ_MSB		0
#define	OSD_TBL_V_SZ_LSB		1
#define	OSD_TBL_H_SZ			2
#define	OSD_TBL_RSLTN			3
#define	OSD_TBL_CLMN			4
#define	OSD_TBL_DAT				5

//#define	OSD_SPKR02_POS_X		(84-1)// (24>>2)*14 - 1 for boundary
//#define	OSD_SPKR13_POS_X		(90+1)// (24>>2)*15 + 1 for boundary
//#define	OSD_SPKR01_POS_Y		110// (20>>1)*11 - 2 for boundary
//#define	OSD_SPKR23_POS_Y		122// (20>>1)*12 - 2 for boundary


//==================================================================================
//						OSD constant description
//==================================================================================
//... OSD bitmap table
extern U8 tbl_osd_16x20_odd[];
extern U8 tbl_osd_16x20_even[];
extern U8 tbl_osd_irq_spkr[];
extern U8	tbl_osd_nr_typ[];

//... string array
extern U8	str_null[];
extern U8	str_cmn_path_ds[];
extern U8	str_cmn_path_rec[];

extern U8	str_wndw_pop_ds_sb[];
extern U8	str_wndw_pop_ds[];
//extern U8	str_wndw_pop_ch[];
extern U8	str_wndw_pop_zm[];
extern U8	str_wndw_pop_mtn[];
extern U8	str_wndw_pop_rc_sb[];
extern U8	str_wndw_pop_rc[];
extern U8	str_wndw_pop_ado[];
extern U8	str_wndw_pop_rg[];
extern U8	str_wndw_pop_abt[];
extern U8	str_wndw_pop_sb_scrn[];
extern U8	str_wndw_pop_sb_src[];
//extern U8	str_wndw_pop_sb_pb[];
extern U8	str_wndw_pop_sb_mtn[];
extern U8	str_wndw_pop_sb_blnd[];
extern U8	str_wndw_pop_sb_nght[];
extern U8	str_wndw_pop_sb_fmt[];
extern U8	str_wndw_pop_sb_indx[];
extern U8	str_wndw_pop_sb_gn[];
extern U8	str_wndw_pop_sb_lv[];
extern U8	str_wndw_pop_sb_rc[];
extern U8	str_wndw_pop_sb_dt[];

//extern U8	str_wndw_cmn_cif[];
//extern U8	str_wndw_cmn_frm[];
//extern U8	str_wndw_cmn_fld[];
//extern U8	str_wndw_cmn_fl_d1[];
//extern U8	str_wndw_cmn_hf_d1[];
extern U8	str_wndw_cmn_same_as_rec[];
extern U8	str_wndw_cmn_ch[];
extern U8	str_wndw_cmn_mux[];

extern U8	str_wndw_cmn_clos[];

extern U8	str_wndw_ttl_ds_scrn[];
extern U8	str_wndw_ttl_pb_inpt[];
extern U8	str_wndw_ttl_pb_indx[];
extern U8	str_wndw_ttl_mtn_md[];
extern U8	str_wndw_ttl_mtn_bd[];
extern U8	str_wndw_ttl_mtn_nd[];
extern U8	str_wndw_ttl_ch_stp[];
extern U8	str_wndw_ttl_rc_fm[];
extern U8	str_wndw_ttl_que_stp[];
extern U8	str_wndw_ttl_no_vid[];
extern U8	str_wndw_ttl_rc_in[];
extern U8	str_wndw_ttl_reg_ctrl[];
extern U8	str_wndw_ttl_ado_gn[];
extern U8	str_wndw_ttl_ado_lv[];
extern U8	str_wndw_ttl_ado_lv_mr[];
extern U8	str_wndw_ttl_ado_rc[];
//extern U8	str_wndw_ttl_ado_dt[];
extern U8	str_wndw_ttl_abt_info[];

//extern U8	str_wndw_grp_mtn[];
//extern U8	str_wndw_grp_blnd[];
//extern U8	str_wndw_grp_nght[];
extern U8	str_wndw_grp_dtctn[];
extern U8	str_wndw_grp_loss[];
extern U8	str_wndw_grp_sel[];
//extern U8	str_wndw_grp_mix[];
extern U8	str_wndw_grp_in_gn[];
extern U8	str_wndw_grp_out_gn[];
extern U8	str_wndw_grp_smpl_rto[];
extern U8	str_wndw_grp_bit_wdth[];
extern U8	str_wndw_grp_mlt_ch[];
extern U8	str_wndw_grp_md[];
extern U8	str_wndw_grp_fltr[];
extern U8	str_wndw_grp_thrs[];
extern U8	str_wndw_grp_src[];
extern U8	str_wndw_grp_size[];
#if	defined(__16CH__)
extern U8	str_wndw_grp_stg[];
#endif
extern U8	str_wndw_grp_skip_ch[];

extern U8	str_wndw_ds_full[];
extern U8	str_wndw_ds_quad[];
extern U8	str_wndw_ds_wth_4pb[];
extern U8	str_wndw_ds_16ch[];
extern U8	str_wndw_ds_wth_16pb[];
extern U8	str_wndw_ds_psdo_8ch[];
extern U8	str_wndw_ds_wth_rc[];
extern U8	str_wndw_ds_fld_sec[];
extern U8	str_wndw_ds_byps[];
extern U8	str_wndw_ds_frz[];
extern U8	str_wndw_ds_blak[];
extern U8	str_wndw_ds_blik[];

extern U8	str_wndw_in_auto_det[];
extern U8	str_wndw_in_mnl_det[];
extern U8	str_wndw_in_bnd_indx[];
extern U8	str_wndw_in_anlg_indx[];
extern U8	str_wndw_in_dtct_id[];
extern U8	str_wndw_in_usr_id[];
extern U8	str_wndw_in_run_in_clk[];
extern U8	str_wndw_in_h_strt[];
extern U8	str_wndw_in_pxl_bit[];
extern U8	str_wndw_in_v_strt[];
//extern U8	str_wndw_in_line[];
extern U8	str_wndw_in_dgtl_indx[];

extern U8	str_wndw_ch_wn[];
extern U8	str_wndw_ch_live_vid[];
extern U8	str_wndw_ch_pb[];
extern U8	str_wndw_ch_en_enh[];

extern U8	str_wndw_mtn_cll[];
extern U8	str_wndw_mtn_lvl[];
extern U8	str_wndw_mtn_sptl[];
extern U8	str_wndw_mtn_tmpl[];
extern U8	str_wndw_mtn_vel[];
extern U8	str_wndw_mtn_dspl_mtn[];
extern U8	str_wndw_mtn_dspl_msk[];

extern U8	str_wndw_rc_frm_cif[];
extern U8	str_wndw_rc_fld_cif[];
extern U8	str_wndw_rc_fl_d1[];
extern U8	str_wndw_rc_hf_d1[];
extern U8	str_wndw_rc_lv_sw[];
extern U8	str_wndw_rc_prd[];
extern U8	str_wndw_rc_que_stp[];
extern U8	str_wndw_rc_no_vid[];

extern U8	str_wndw_pop_sb_nr[];
extern U8	str_wndw_grp_ch_en[];
extern U8	str_wndw_grp_wn_typ[];
//extern U8	str_wndw_rc_nr_en_fltr[];
//extern U8	str_wndw_rc_nr_3x3[];
//extern U8	str_wndw_rc_nr_crs[];
//extern U8	str_wndw_rc_nr_mlt[];
//extern U8	str_wndw_rc_nr_v_br[];
extern U8	str_wndw_rc_nr_typ1[];
extern U8	str_wndw_rc_nr_typ2[];
extern U8	str_wndw_rc_nr_typ3[];
extern U8	str_wndw_rc_nr_typ4[];
extern U8	str_wndw_rc_nr_adpt[];
//extern U8	str_wndw_rc_nr_nrml[];
//extern U8	str_wndw_rc_nr_thr[];
//extern U8	str_wndw_rc_nr_slp[];

extern U8	str_wndw_no_vid_byps[];
extern U8	str_wndw_no_vid_hold[];
extern U8	str_wndw_no_vid_blue_back[];
extern U8	str_wndw_no_vid_skip[];
extern U8	str_wndw_no_vid_auto[];
extern U8	str_wndw_no_vid_manl[];

extern U8	str_wndw_pop_vga[];
extern U8	str_wndw_pop_sb_out_rs[];
extern U8	str_wndw_pop_sb_de_int[];
extern U8	str_wndw_pop_sb_img_en[];
extern U8	str_wndw_pop_sb_ns_rd[];
extern U8	str_wndw_ttl_out_rs[];
extern U8	str_wndw_ttl_de_int[];
extern U8	str_wndw_ttl_img_en[];
extern U8	str_wndw_ttl_ns_rd[];
extern U8	str_wndw_grp_3d_nr[];
extern U8	str_wndw_grp_3d_de_int[];
extern U8	str_wndw_vga_or_vga[];
extern U8	str_wndw_vga_or_svga[];
extern U8	str_wndw_vga_di_adpt[];
extern U8	str_wndw_vga_di_bob[];
extern U8	str_wndw_vga_di_wv[];
extern U8	str_wndw_vga_di_edg[];
extern U8	str_wndw_vga_di_fst[];
extern U8	str_wndw_vga_di_mtn_1[];
//extern U8	str_wndw_vga_di_mtn_2[];
extern U8	str_wndw_vga_ie_shrp[];
extern U8	str_wndw_vga_ie_lti[];
extern U8	str_wndw_vga_ie_cti[];
extern U8	str_wndw_vga_ie_bw[];
extern U8	str_wndw_vga_nr_off[];
extern U8	str_wndw_vga_nr_tpcl[];
extern U8	str_wndw_vga_nr_max[];
extern U8	str_wndw_vga_nr_ccs[];

extern U8	str_wndw_ado_cmn_ch1[];
extern U8	str_wndw_ado_cmn_ch2[];
extern U8	str_wndw_ado_cmn_ch3[];
extern U8	str_wndw_ado_cmn_ch4[];
extern U8	str_wndw_ado_gn[];
extern U8	str_wndw_ado_lv_pb[];
extern U8	str_wndw_ado_lv_mix[];
extern U8	str_wndw_ado_lv_mix_rto[];
extern U8	str_wndw_ado_rc_8k[]; 
extern U8	str_wndw_ado_rc_16k[];
extern U8	str_wndw_ado_rc_8bit[];
extern U8	str_wndw_ado_rc_16bit[];
extern U8	str_wndw_ado_rc_2ch[];
extern U8	str_wndw_ado_rc_4ch[];
extern U8	str_wndw_ado_rc_8ch[];
extern U8	str_wndw_ado_rc_16ch[];
//extern U8	str_wndw_ado_dt_absl[];
//extern U8	str_wndw_ado_dt_rltv[];

extern U8	str_wndw_rg_radx[];
extern U8	str_wndw_rg_ms[];
extern U8	str_wndw_rg_s1[];
extern U8	str_wndw_rg_s2[];
extern U8	str_wndw_rg_s3[];

//==================================================================================
//						OSD function prototype description
//==================================================================================
extern void WriteOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 *_bmp_ptr);	//... only one _pth at once
extern void FillOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp_pg, U8 _indx, U8 *_bmp_ptr, U8 attribute );

extern void WriteOSDBmpY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 *_bmp_ptr);	//... only one _pth at once

extern void WriteOSDFntX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx);	//... only one _path at once
extern void WriteOSDFntY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx);	//... only one _path at once
extern void WriteOSDFnt(U8 _pth, U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx);
extern void WriteOSDStrX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr);
extern void WriteOSDStrY(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr);
extern void WriteOSDStrX_wg(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr,  U8 _str_len,  U8 _str_color);
extern void WriteOSDStrY_wg(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr,  U8 _str_len,  U8 _str_color);
extern void WriteOSDFntX_hys(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 _stringcolor);
extern void WriteOSDFntY_hys(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 _stringcolor);	


extern void WriteOSDStr(U8 _pth, U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr);
extern void WriteOSDClrX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght);	//... only one _path at once
extern void WriteOSDClrY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght);	//... only one _path at once
extern void WriteOSDClr(U8 _pth, U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght);
//extern U8 DsplyOSDDatHexX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr);
extern U8 DsplyOSDDatHexY(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr);
//extern U8 DsplyOSDDatBinX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr);
extern void  OSDClrX_pge(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _pge, U8 _color)	;
extern void OSDClrX(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _page);
extern void OSDClrY(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb,  U8 _page);

extern void	SetOSDCol(U8 _pth, U8 _col_num, U8 _y, U8 _cb, U8 _cr);
extern void	HwTW2835SetOSDColLUT(U8 _dvc, U8 _pth, U8  *_tbl_osd_clr);//U8 _col_num, U8 _y, U8 _cb, U8 _cr);

extern void	InitOSDCol(void);
extern void	SetOSDMn(void);
extern void	SetOSDNoMn(void);


extern void WriteOSDBmp_pointX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 color_1, U8 color_2, U8  color_3, U8 color_4, U8 attribute);	//... 180(4 pixel/_pos_x) x 240/288 domain
extern void WriteOSDBmp_pointY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 color_1, U8 color_2, U8 attribute);





