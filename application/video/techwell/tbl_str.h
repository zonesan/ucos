//==================================================================================
//						Const string description
//==================================================================================

unsigned char	str_null[] = " ";

unsigned char	str_cmn_path_ds[] = "X";//"DISPLAY";
unsigned char	str_cmn_path_rec[] = "Y";//"RECORD ";

unsigned char	str_wndw_pop_ds_sb[] = "Display ;";
unsigned char	str_wndw_pop_ds[] = "Display";
//unsigned char	str_wndw_pop_ch[] = "Source";//"Channel";
unsigned char	str_wndw_pop_zm[] = "Zoom";
unsigned char	str_wndw_pop_mtn[] = "Motion  ;";
unsigned char	str_wndw_pop_rc_sb[] = "Record  ;";
unsigned char	str_wndw_pop_rc[] = "Record";
unsigned char	str_wndw_pop_ado[] = "Audio   ;";
unsigned char	str_wndw_pop_rg[] = "Register";
unsigned char	str_wndw_pop_abt[] = "About";

unsigned char	str_wndw_pop_sb_scrn[] = "Screen";
unsigned char	str_wndw_pop_sb_src[] = "Source";
//unsigned char	str_wndw_pop_sb_pb[] = "Playback";
unsigned char	str_wndw_pop_sb_mtn[] = "Motion";
unsigned char	str_wndw_pop_sb_blnd[] = "Blind";
unsigned char	str_wndw_pop_sb_nght[] = "Night";
unsigned char	str_wndw_pop_sb_fmt[] = "Format";
unsigned char	str_wndw_pop_sb_indx[] = "Index";
unsigned char	str_wndw_pop_sb_gn[] = "Gain  ";
unsigned char	str_wndw_pop_sb_lv[] = "Live  ";
unsigned char	str_wndw_pop_sb_rc[] = "Record";
unsigned char	str_wndw_pop_sb_dt[] = "Detect";

//unsigned char	str_wndw_cmn_cif[] = "CIF";
//unsigned char	str_wndw_cmn_frm[] = "Frame";
//unsigned char	str_wndw_cmn_fld[] = "Field";
//unsigned char	str_wndw_cmn_fl_d1[] = "Full D1";
//unsigned char	str_wndw_cmn_hf_d1[] = "Half D1";
unsigned char	str_wndw_cmn_same_as_rec[] = "Same as record";
unsigned char	str_wndw_cmn_ch[] = "Channel";
unsigned char	str_wndw_cmn_mux[] = "MUX";

unsigned char	str_wndw_cmn_clos[] = "Close";

unsigned char	str_wndw_ttl_ds_scrn[] = "Display screen";
unsigned char	str_wndw_ttl_pb_inpt[] = "Playback input";
unsigned char	str_wndw_ttl_pb_indx[] = "Playback index";
unsigned char	str_wndw_ttl_mtn_md[] = "Motion detect";
unsigned char	str_wndw_ttl_mtn_bd[] = "Blind detect";
unsigned char	str_wndw_ttl_mtn_nd[] = "Night detect";
unsigned char	str_wndw_ttl_ch_stp[] = "Source setup";//"Channel setup";
unsigned char	str_wndw_ttl_rc_fm[] = "Record format";
unsigned char	str_wndw_ttl_que_stp[] = "Queue setup";
unsigned char	str_wndw_ttl_no_vid[] = "No video";
unsigned char	str_wndw_ttl_rc_in[] = "Record index";
unsigned char	str_wndw_ttl_reg_ctrl[] = "Register control";
unsigned char	str_wndw_ttl_ado_gn[] = "Audio gain";
unsigned char	str_wndw_ttl_ado_lv[] = "Audio live";
unsigned char	str_wndw_ttl_ado_lv_mr[] = "Mix ratio";
unsigned char	str_wndw_ttl_ado_rc[] = "Audio record";
//unsigned char	str_wndw_ttl_ado_dt[] = "Audio detect";
unsigned char	str_wndw_ttl_abt_info[] = "About";


//unsigned char	str_wndw_grp_mtn[] = "]Motion[";
//unsigned char	str_wndw_grp_blnd[] = "]Blind[";
//unsigned char	str_wndw_grp_nght[] = "]Night[";
unsigned char	str_wndw_grp_dtctn[] = "]Detection[";
unsigned char	str_wndw_grp_loss[] = "]No video[";
unsigned char	str_wndw_grp_sel[] = "]Select[";
//unsigned char	str_wndw_grp_mix[] = "]Mix ratio[";
unsigned char	str_wndw_grp_in_gn[] = "]Input[";//"]Input gain[";
unsigned char	str_wndw_grp_out_gn[] = "]Output[";//"]Output gain[";
unsigned char	str_wndw_grp_smpl_rto[] = "]Sample rate[";
unsigned char	str_wndw_grp_bit_wdth[] = "]Bit width[";
unsigned char	str_wndw_grp_mlt_ch[] = "]Multi channel[";
unsigned char	str_wndw_grp_md[] = "]Mode[";
unsigned char	str_wndw_grp_fltr[] = "]Filter[";
unsigned char	str_wndw_grp_thrs[] = "]Threshold[";
unsigned char	str_wndw_grp_src[] = "]Source[";
unsigned char	str_wndw_grp_size[] = "]SIZE[";
#if	defined(__16CH__)
unsigned char	str_wndw_grp_stg[] = "]STAGE[";
#endif
unsigned char	str_wndw_grp_skip_ch[] = "]Skip ch[";


unsigned char	str_wndw_ds_full[] = "1 ch";//"Full";
unsigned char	str_wndw_ds_quad[] = "4 ch";//"Quad";
unsigned char	str_wndw_ds_wth_4pb[] = "With 4ch PB";//"8 ch";//"Triplex";
unsigned char	str_wndw_ds_16ch[] = "16 ch";//"16 Split";
unsigned char	str_wndw_ds_wth_16pb[] = "With 16ch PB";//"32 ch";
unsigned char	str_wndw_ds_psdo_8ch[] = "Pseudo 8ch";
unsigned char	str_wndw_ds_wth_rc[] = "With record";
unsigned char	str_wndw_ds_fld_sec[] = "FLD/SEC";
unsigned char	str_wndw_ds_byps[] = "Bypass";
unsigned char	str_wndw_ds_frz[] = "Freeze";
unsigned char	str_wndw_ds_blak[] = "Blank";
unsigned char	str_wndw_ds_blik[] = "Blink";

unsigned char	str_wndw_in_auto_det[] = "Auto detect";
unsigned char	str_wndw_in_mnl_det[] = "Manual detect";
unsigned char	str_wndw_in_bnd_indx[] = "Boundary index";
unsigned char	str_wndw_in_anlg_indx[] = "Analog index";
unsigned char	str_wndw_in_dtct_id[] = "Detection info";
unsigned char	str_wndw_in_usr_id[] = "User channel ID";
unsigned char	str_wndw_in_run_in_clk[] = "Run in clock";
unsigned char	str_wndw_in_h_strt[] = "H start";
unsigned char	str_wndw_in_pxl_bit[] = "Pixel/bit";
unsigned char	str_wndw_in_v_strt[] = "V start";
//unsigned char	str_wndw_in_line[] = "# of line";
unsigned char	str_wndw_in_dgtl_indx[] = "Digital index";

unsigned char	str_wndw_ch_wn[] = "Window";
unsigned char	str_wndw_ch_live_vid[] = "Live video";
unsigned char	str_wndw_ch_pb[] = "Playback";
unsigned char	str_wndw_ch_en_enh[] = "Enable enhance";

unsigned char	str_wndw_mtn_cll[] = "# of Cell";
unsigned char	str_wndw_mtn_lvl[] = "Level";
unsigned char	str_wndw_mtn_sptl[] = "Spatial";
unsigned char	str_wndw_mtn_tmpl[] = "Temporal";
unsigned char	str_wndw_mtn_vel[] = "Velocity";
unsigned char	str_wndw_mtn_dspl_mtn[] = "Display motion";
unsigned char	str_wndw_mtn_dspl_msk[] = "Display mask";

unsigned char	str_wndw_rc_frm_cif[] = "Frame CIF";
unsigned char	str_wndw_rc_fld_cif[] = "Field CIF";
unsigned char	str_wndw_rc_fl_d1[] = "Full D1";
unsigned char	str_wndw_rc_hf_d1[] = "Half D1";
unsigned char	str_wndw_rc_lv_sw[] = "Live switch";
unsigned char	str_wndw_rc_prd[] = "Period";
unsigned char	str_wndw_rc_que_stp[] = "Que setup";
unsigned char	str_wndw_rc_no_vid[] = "No video ";

unsigned char	str_wndw_pop_sb_nr[] = "Noise";
unsigned char	str_wndw_grp_ch_en[] = "]Channel enable[";
unsigned char	str_wndw_grp_wn_typ[] = "]Window type[";
//unsigned char	str_wndw_rc_nr_en_fltr[] = "Enable filter";
//unsigned char	str_wndw_rc_nr_3x3[] = "3x3";
//unsigned char	str_wndw_rc_nr_crs[] = "Cross";
//unsigned char	str_wndw_rc_nr_mlt[] = "Multiplier";
//unsigned char	str_wndw_rc_nr_v_br[] = "Vertical bar";
unsigned char	str_wndw_rc_nr_typ1[] = "Type 1";
unsigned char	str_wndw_rc_nr_typ2[] = "Type 2";
unsigned char	str_wndw_rc_nr_typ3[] = "Type 3";
unsigned char	str_wndw_rc_nr_typ4[] = "Type 4";
unsigned char	str_wndw_rc_nr_adpt[] = "Adaptive mode";
//unsigned char	str_wndw_rc_nr_nrml[] = "Normal mode";
//unsigned char	str_wndw_rc_nr_thr[] = "Threshold";
//unsigned char	str_wndw_rc_nr_slp[] = "Slope";

unsigned char	str_wndw_no_vid_byps[] = "Bypass";
unsigned char	str_wndw_no_vid_hold[] = "Holding";
unsigned char	str_wndw_no_vid_blue_back[] = "Blue back";
unsigned char	str_wndw_no_vid_skip[] = "Skip";
unsigned char	str_wndw_no_vid_auto[] = "Auto";
unsigned char	str_wndw_no_vid_manl[] = "Manual";

unsigned char	str_wndw_pop_vga[] = "VGA out ;";
unsigned char	str_wndw_pop_sb_out_rs[] = "Resolution";
unsigned char	str_wndw_pop_sb_de_int[] = "DeInterlace";//"De-interlacing";
unsigned char	str_wndw_pop_sb_img_en[] = "Enhance";//"Image Enhance";
unsigned char	str_wndw_pop_sb_ns_rd[] = "Noise";//"Noise Reduction";
unsigned char	str_wndw_ttl_out_rs[] = "Resolution";
unsigned char	str_wndw_ttl_de_int[] = "3D De-interlace";//"De-interlacing";
unsigned char	str_wndw_ttl_img_en[] = "Image enhance";
unsigned char	str_wndw_ttl_ns_rd[] = "Noise reduction";
unsigned char	str_wndw_grp_3d_nr[] = "]3D NR[";
unsigned char	str_wndw_grp_3d_de_int[] = "]Mode[";//"]3D De-interlace[";
unsigned char	str_wndw_vga_or_vga[] = "VGA (640x480)";
unsigned char	str_wndw_vga_or_svga[] = "SVGA(800x600)";
unsigned char	str_wndw_vga_di_adpt[] = "Adaptive";//"Adaptive mode";
unsigned char	str_wndw_vga_di_bob[] = "Intra (Bob)";//"Intra only mode(Bob)";
unsigned char	str_wndw_vga_di_wv[] = "Inter (Weave)";//"Inter only mode(Weave)";
unsigned char	str_wndw_vga_di_edg[] = "Edge preserving";
unsigned char	str_wndw_vga_di_fst[] = "Fast motion";//"Fast motion detection";
unsigned char	str_wndw_vga_di_mtn_1[] = "Motion boundary";
//unsigned char	str_wndw_vga_di_mtn_2[] = "preserving";
unsigned char	str_wndw_vga_ie_shrp[] = "Sharpness";//"Sharpness enhancer";
unsigned char	str_wndw_vga_ie_lti[] = "LTI";
unsigned char	str_wndw_vga_ie_cti[] = "CTI";
unsigned char	str_wndw_vga_ie_bw[] = "B/W extension";//"B/W level extension";
unsigned char	str_wndw_vga_nr_off[] = "OFF";
unsigned char	str_wndw_vga_nr_tpcl[] = "Typical";
unsigned char	str_wndw_vga_nr_max[] = "Max";
unsigned char	str_wndw_vga_nr_ccs[] = "CCS";

unsigned char	str_wndw_ado_cmn_ch1[] = "Ch 1";
unsigned char	str_wndw_ado_cmn_ch2[] = "Ch 2";
unsigned char	str_wndw_ado_cmn_ch3[] = "Ch 3";
unsigned char	str_wndw_ado_cmn_ch4[] = "Ch 4";
unsigned char	str_wndw_ado_gn[] = "Gain";
unsigned char	str_wndw_ado_lv_pb[] = "PB";
unsigned char	str_wndw_ado_lv_mix[] = "Mix";
unsigned char	str_wndw_ado_lv_mix_rto[] = "Mix ratio";
unsigned char	str_wndw_ado_rc_8k[] = "8K";
unsigned char	str_wndw_ado_rc_16k[] = "16K";
unsigned char	str_wndw_ado_rc_8bit[] = "8Bit";
unsigned char	str_wndw_ado_rc_16bit[] = "16Bit";
unsigned char	str_wndw_ado_rc_2ch[] = "2 channel";
unsigned char	str_wndw_ado_rc_4ch[] = "4 channel";
unsigned char	str_wndw_ado_rc_8ch[] = "8 channel";
unsigned char	str_wndw_ado_rc_16ch[] = "16 channel";
//unsigned char	str_wndw_ado_dt_absl[] = "Absolute";
//unsigned char	str_wndw_ado_dt_rltv[] = "Relative";

unsigned char	str_wndw_rg_radx[] = "B    : H";
unsigned char	str_wndw_rg_ms[] = "MA";//"_A";//
unsigned char	str_wndw_rg_s1[] = "S1";//"_B";//
unsigned char	str_wndw_rg_s2[] = "S2";//"_C";//
unsigned char	str_wndw_rg_s3[] = "S3";//"_D";//


