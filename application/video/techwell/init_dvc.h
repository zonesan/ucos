//==================================================================================
//						Device init definition & variable description
//==================================================================================

//==================================================================================
//						Device init constant description
//==================================================================================
////extern code unsigned char	tbl_ntsc_pg0_ch0[];      
//extern code unsigned char	tbl_ntsc_pg0_cmn[];      
//extern code unsigned char	tbl_ntsc_pg0_sfr1[];		//... other decoder part
//extern code unsigned char	tbl_ntsc_pg0_sfr2[];		//... audio part
//extern code unsigned char	tbl_ntsc_pg0_scl[];
//extern code unsigned char	tbl_ntsc_pg0_sfr3[];		//... 
//extern code unsigned char	tbl_pal_pg0_cmn[];       
//extern code unsigned char	tbl_pal_pg0_sfr1[];		//... other decoder part
//extern code unsigned char	tbl_pal_pg0_sfr2[];		//... audio part
//extern code unsigned char	tbl_pal_pg0_scl[];
//extern code unsigned char	tbl_pal_pg0_sfr3[];		//... 
//
////extern code U8	tbl_page0_4pb_en[];
////extern code U8	tbl_page0_4pb_dis[];
////
////extern code U8	tbl_page1_4pb_en[];
////extern code U8	tbl_page1_4pb_dis[];

//extern	U8 reg_crsr_ctrl;
extern U16	reg_crsr_pos_x;
extern U16	reg_crsr_pos_y;
////==================================================================================
////						Device init function prototype description
////==================================================================================
extern void InitPg0(void);
extern void InitPg1(void);
extern void InitPg2(void);
//extern void	InitReg(void);
//extern void InitEncExt(void);
