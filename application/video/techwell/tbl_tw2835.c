#include	"hdr.h"

//==================================================================================
//						page0 initialize table description
//==================================================================================

/*	//... non-realtime table
//--------------------------		NTSC		------------------------------//
//=================================================================================
unsigned char	tbl_ntsc_pg0_cmn[] = {        
//												CH1			CH2			CH3			CH4
	0x00,0xc8,0x20,0xd0,		//...		0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
	0x06,0xf0,0x08,0x80,		//...		0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x80,0x80,0x80,0x02,		//...		0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x06,0x00,0x11				//...		0x0c~0x0e	0x1c~0x1e	0x2c~0x2e	0x3c~0x3e
};

unsigned char	tbl_ntsc_pg0_sfr1[] = {		//... other decoder part
	0x00,0x00,0x7f,0x4f,		//...		0x40~0x43
	0xa0,0x10,0x2f,0x64,		//...		0x44~0x47
	0x80,0x80,0x82,0x82,		//...		0x48~0x4b
	0x00,0x00,0x05,0x00,		//...		0x4c~0x4f
	0x00,0x88,0x06,0x00,		//...		0x50~0x53
	0x00							//...		0x54
};

unsigned char	tbl_ntsc_pg0_sfr2[] = {		//... audio part
	0x88,0x88,0x00,0x00,		//...		0x60~0x63
	0x10,0x32,0x54,0x76,		//...		0x64~0x67
	0x98,0xba,0xdc,0xfe,		//...		0x68~0x6b
	0x00,0x00,0x88,0x88,		//...		0x6c~0x6f
	0x88,0x11,0x40,0x88,		//...		0x70~0x73
	0x88							//...		0x74
};

unsigned char	tbl_ntsc_pg0_scl[] = {
//												CH1			CH2			CH3			CH4
	0x06,0x55,0x55,0x55,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
	0x55,0x00,0xff,0xff,		//...		0x84~0x87	0x94~0x97	0xa4~0xa7	0xb4~0xb7
	0xff,0xff,0x31,0x00,		//...		0x88~0x8b	0x98~0x9b	0xa8~0xab	0xb8~0xbb
	0xd0,0x00,0xf0,0x08		//...		0x8c~0x8e	0x9c~0x9e	0xac~0xae	0xbc~0xbe
};

unsigned char	tbl_ntsc_pg0_sfr3[] = {		//... 
	0x00,0x00,0x00,0x07,		//...		0xc0~0xc3
	0x00,0x00,0xf0,0xff,		//...		0xc4~0xc7
	0x00,0x3c,0x0f				//...		0xc8~0xca
};



//--------------------------		PAL		------------------------------//
//=================================================================================
unsigned char	tbl_pal_pg0_cmn[] = {        
//												CH1			CH2			CH3			CH4
	0x00,0x88,0x20,0xd0,		//...		0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
	0x05,0x20,0x28,0x80,		//...		0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x80,0x80,0x80,0x82,		//...		0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x06,0x00,0x11				//...		0x0c~0x0e	0x1c~0x1e	0x2c~0x2e	0x3c~0x3e
};

unsigned char	tbl_pal_pg0_sfr1[] = {		//... other decoder part
	0x00,0x00,0x7f,0x4f,		//...		0x40~0x43
	0xa0,0x10,0x2f,0x64,		//...		0x44~0x47
	0x80,0x80,0x82,0x82,		//...		0x48~0x4b
	0x00,0x00,0x05,0x00,		//...		0x4c~0x4f
	0x00,0x88,0x06,0x00,		//...		0x50~0x53
	0x00							//...		0x54
};

unsigned char	tbl_pal_pg0_sfr2[] = {		//... audio part
	0x88,0x88,0x00,0x00,		//...		0x60~0x63
	0x10,0x32,0x54,0x76,		//...		0x64~0x67
	0x98,0xba,0xdc,0xfe,		//...		0x68~0x6b
	0x00,0x00,0x88,0x88,		//...		0x6c~0x6f
	0x88,0x11,0x40,0x88,		//...		0x70~0x73
	0x88							//...		0x74
};

unsigned char	tbl_pal_pg0_scl[] = {
//												CH1			CH2			CH3			CH4
	0x06,0x55,0x55,0x55,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
	0x55,0x00,0xff,0xff,		//...		0x84~0x87	0x94~0x97	0xa4~0xa7	0xb4~0xb7
	0xff,0xff,0x31,0x00,		//...		0x88~0x8b	0x98~0x9b	0xa8~0xab	0xb8~0xbb
	0xd0,0x00,0x20,0x28		//...		0x8c~0x8e	0x9c~0x9e	0xac~0xae	0xbc~0xbe
};

unsigned char	tbl_pal_pg0_sfr3[] = {		//... 
	0x00,0x00,0x00,0x07,		//...		0xc0~0xc3
	0x00,0xff,0xf0,0xff,		//...		0xc4~0xc7
	0x00,0x3c,0x0f				//...		0xc8~0xca
};
//	*/
//==================================================================================

//*	//... normal quad table
//--------------------------		NTSC		------------------------------//
//=================================================================================
unsigned char	tbl_ntsc_pg0_cmn[] = {        
//												CH1			CH2			CH3			CH4
	0x00,0xc8,0x20,0xd0,		//...		0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
	0x06,0xf0,0x08,0x80,		//...		0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x80,0x80,0x80,0x02,		//...		0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x06,0x00,0x11				//...		0x0c~0x0e	0x1c~0x1e	0x2c~0x2e	0x3c~0x3e
};

unsigned char	tbl_ntsc_pg0_sfr1[] = {		//... other decoder part
//	0x00,0x00,0x7f,0x45,		//...		0x40~0x43	//... before 060315
//	0x00,0x7f,0x7f,0x45,		//...		0x40~0x43	//... update:060315
	0x00,0x77,0x77,0x45,		//...		0x40~0x43	//... update:060626
	0xa0,0xd0,0x2f,0x64,		//...		0x44~0x47
	0x80,0x80,0x82,0x82,		//...		0x48~0x4b
	0x00,0x0f,0x05,0x00,		//...		0x4c~0x4f
	0x00,0x80,0x06,0x00,		//...		0x50~0x53
	0x00							//...		0x54
};

unsigned char	tbl_ntsc_pg0_sfr2[] = {		//... audio part
	0x88,0x88,0x00,0x00,		//...		0x60~0x63
	0x10,0x32,0x54,0x76,		//...		0x64~0x67
	0x98,0xba,0xdc,0xfe,		//...		0x68~0x6b
	0x00,0x00,0x88,0x88,		//...		0x6c~0x6f
	0x88,0x54,0x00,0x00,		//...		0x70~0x73
	0x00							//...		0x74
};

unsigned char	tbl_ntsc_pg0_scl[] = {
//												CH1			CH2			CH3			CH4
#if defined(__4CH__)
	0x01,0x7f,0xff,0x7f,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
#elif defined(__16CH__)
	0x0b,0x3f,0xff,0x3f,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
#endif
	0xff,0x00,0xff,0xff,		//...		0x84~0x87	0x94~0x97	0xa4~0xa7	0xb4~0xb7
	0xff,0xff,0x31,0x00,		//...		0x88~0x8b	0x98~0x9b	0xa8~0xab	0xb8~0xbb
	0xd0,0x00,0xf0,0x08		//...		0x8c~0x8e	0x9c~0x9e	0xac~0xae	0xbc~0xbe
};

unsigned char	tbl_ntsc_pg0_sfr3[] = {		//... 
//	0x00,0x00,0x00,0x07,		//...		0xc0~0xc3	//... AUTOBGNDxx on
	0x00,0x00,0x00,0x00,		//...		0xc0~0xc3	//... AUTOBGNDxx off
	0x00,0x00,0xf0,0xff,		//...		0xc4~0xc7
	0x00,0x3c,0x4f				//...		0xc8~0xca	//... update:060315
};



//--------------------------		PAL		------------------------------//
//=================================================================================
unsigned char	tbl_pal_pg0_cmn[] = {        
//												CH1			CH2			CH3			CH4
	0x00,0x88,0x1b,0xd0,		//...		0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
	0x05,0x20,0x28,0x80,		//...		0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x80,0x80,0x80,0x82,		//...		0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x06,0x00,0x11				//...		0x0c~0x0e	0x1c~0x1e	0x2c~0x2e	0x3c~0x3e
};

unsigned char	tbl_pal_pg0_sfr1[] = {		//... other decoder part
//	0x00,0x00,0x7f,0x45,		//...		0x40~0x43	//... before 060315
//	0x00,0x7f,0x7f,0x45,		//...		0x40~0x43	//... update:060315
	0x00,0x77,0x77,0x45,		//...		0x40~0x43	//... update:060626
	0xa0,0xd0,0x2f,0x64,		//...		0x44~0x47
	0x80,0x80,0x82,0x82,		//...		0x48~0x4b
	0x00,0x0f,0x05,0x00,		//...		0x4c~0x4f
	0x00,0x80,0x06,0x00,		//...		0x50~0x53
	0x00							//...		0x54
};

unsigned char	tbl_pal_pg0_sfr2[] = {		//... audio part
	0x88,0x88,0x00,0x00,		//...		0x60~0x63
	0x10,0x32,0x54,0x76,		//...		0x64~0x67
	0x98,0xba,0xdc,0xfe,		//...		0x68~0x6b
	0x00,0x00,0x88,0x88,		//...		0x6c~0x6f
	0x88,0x54,0x00,0x00,		//...		0x70~0x73
	0x00							//...		0x74
};

unsigned char	tbl_pal_pg0_scl[] = {
//												CH1			CH2			CH3			CH4
#if defined(__4CH__)
	0x01,0x7f,0xff,0x7f,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
#elif defined(__16CH__)
	0x0b,0x3f,0xff,0x3f,		//...		0x80~0x83	0x90~0x93	0xa0~0xa3	0xb0~0xb3
#endif
	0xff,0x00,0xff,0xff,		//...		0x84~0x87	0x94~0x97	0xa4~0xa7	0xb4~0xb7
	0xff,0xff,0x31,0x00,		//...		0x88~0x8b	0x98~0x9b	0xa8~0xab	0xb8~0xbb
	0xd0,0x00,0x20,0x28		//...		0x8c~0x8e	0x9c~0x9e	0xac~0xae	0xbc~0xbe
};

unsigned char	tbl_pal_pg0_sfr3[] = {		//... 
//	0x00,0x00,0x00,0x07,		//...		0xc0~0xc3	//... AUTOBGNDxx on
	0x00,0x00,0x00,0x00,		//...		0xc0~0xc3	//... AUTOBGNDxx off
//	0x00,0xff,0xf0,0xff,		//...		0xc4~0xc7	//... recommend:full table
	0x00,0x00,0xf0,0xff,		//...		0xc4~0xc7	//... temporal:quad table
	0x00,0x3c,0x4f				//...		0xc8~0xca	//... update:060315
};
//	*/
//==================================================================================





//*
//==================================================================================
//						Page1 initialize table description
//==================================================================================
unsigned char	tbl_pg1_x_cmn[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	     0x02,0x00,0x00,0x00,0x84,0x00,0x00,	0x00,0x00,0x00,0xd7,0x23,0x2d,0x1e,0xa7,	//...  0x01
	0x80,0x02,0x00,0x00,0x02,0x00,0x10,0x00,	0x81,0x02,0x00,0x01,0x02,0x00,0x11,0x00,	//...  0x10	//... normal quad
	0x82,0x02,0x00,0x02,0x02,0x00,0x12,0x00,	0x83,0x02,0x00,0x03,0x02,0x00,0x13,0x00,	//...  0x20
//	0x90,0x02,0x00,0x90,0x02,0x00,0x10,0x00,	0x91,0x02,0x00,0x91,0x02,0x00,0x11,0x00,	//...  0x10	//... non-realtime
//	0x92,0x02,0x00,0x92,0x02,0x00,0x12,0x00,	0x93,0x02,0x00,0x93,0x02,0x00,0x13,0x00,	//...  0x20
};

//--------------------------		NTSC		------------------------------//
//... H: 45(2d),90(5a),135(87),60(3c),120(78),180(b4)
//... V: 30(1e),40(28),60(3c),80(50),90(5a),120(78) for NTSC
//... V: 36(24),48(30),72(48),96(60),108(6c),144(90) for PAL
unsigned char	tbl_ntsc_pg1_pic_fl[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0xb4,0x00,0x78,0x00,0xb4,0x00,0x78,	0x00,0xb4,0x00,0x78,0x00,0xb4,0x00,0x78	//...  
};

unsigned char	tbl_ntsc_pg1_pic_qd[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x5a,0x00,0x3c,0x5a,0xb4,0x00,0x3c,	0x00,0x5a,0x3c,0x78,0x5a,0xb4,0x3c,0x78	//...  
};

#if defined(__4CH__)
unsigned char	tbl_ntsc_pg1_pic_9_lt[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x3c,0x00,0x28,0x3c,0x78,0x00,0x28,	0x78,0xb4,0x00,0x28,0x00,0x3c,0x28,0x50	//...   
};

unsigned char	tbl_ntsc_pg1_pic_9_rb[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x78,0xb4,0x28,0x50,0x00,0x3c,0x50,0x78,	0x3c,0x78,0x50,0x78,0x78,0xb4,0x50,0x78	//...   
};

#elif defined(__16CH__)
unsigned char	tbl_ntsc_pg1_pic_16_0[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x00,0x1e,0x2d,0x5a,0x00,0x1e,	0x5a,0x87,0x00,0x1e,0x87,0xb4,0x00,0x1e	//...   
};

unsigned char	tbl_ntsc_pg1_pic_16_1[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x1e,0x3c,0x2d,0x5a,0x1e,0x3c,	0x5a,0x87,0x1e,0x3c,0x87,0xb4,0x1e,0x3c	//...   
};

unsigned char	tbl_ntsc_pg1_pic_16_2[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x3c,0x5a,0x2d,0x5a,0x3c,0x5a,	0x5a,0x87,0x3c,0x5a,0x87,0xb4,0x3c,0x5a	//...   
};

unsigned char	tbl_ntsc_pg1_pic_16_3[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x5a,0x78,0x2d,0x5a,0x5a,0x78,	0x5a,0x87,0x5a,0x78,0x87,0xb4,0x5a,0x78	//...   
};
#endif

//--------------------------		PAL		------------------------------//
//... H: 45(2d),90(5a),135(87),60(3c),120(78),180(b4)
//... V: 30(1e),40(28),60(3c),80(50),90(5a),120(78) for NTSC
//... V: 36(24),48(30),72(48),96(60),108(6c),144(90) for PAL
unsigned char	tbl_pal_pg1_pic_fl[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0xb4,0x00,0x90,0x00,0xb4,0x00,0x90,	0x00,0xb4,0x00,0x90,0x00,0xb4,0x00,0x90	//...  
};

unsigned char	tbl_pal_pg1_pic_qd[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x5a,0x00,0x48,0x5a,0xb4,0x00,0x48,	0x00,0x5a,0x48,0x90,0x5a,0xb4,0x48,0x90	//...  
};

#if defined(__4CH__)
unsigned char	tbl_pal_pg1_pic_9_lt[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x3c,0x00,0x30,0x3c,0x78,0x00,0x30,	0x78,0xb4,0x00,0x30,0x00,0x3c,0x30,0x60	//...   
};

unsigned char	tbl_pal_pg1_pic_9_rb[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x78,0xb4,0x30,0x60,0x00,0x3c,0x60,0x90,	0x3c,0x78,0x60,0x90,0x78,0xb4,0x60,0x90	//...   
};

#elif defined(__16CH__)
unsigned char	tbl_pal_pg1_pic_16_0[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x00,0x24,0x2d,0x5a,0x00,0x24,	0x5a,0x87,0x00,0x24,0x87,0xb4,0x00,0x24	//...   
};

unsigned char	tbl_pal_pg1_pic_16_1[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x24,0x48,0x2d,0x5a,0x24,0x48,	0x5a,0x87,0x24,0x48,0x87,0xb4,0x24,0x48	//...   
};

unsigned char	tbl_pal_pg1_pic_16_2[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x48,0x6c,0x2d,0x5a,0x48,0x6c,	0x5a,0x87,0x48,0x6c,0x87,0xb4,0x48,0x6c	//...   
};

unsigned char	tbl_pal_pg1_pic_16_3[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x00,0x2d,0x6c,0x90,0x2d,0x5a,0x6c,0x90,	0x5a,0x87,0x6c,0x90,0x87,0xb4,0x6c,0x90	//...   
};
#endif	
//=================================================================================

unsigned char	tbl_pg1_y_cmn[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
#if defined(__4CH__)
	0x28,0x00,0x10,0xa0,0x00,0x00,0x00,0x03,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xa7,	//... 0x50 
	0x80,0x02,0x00,0x81,0x02,0x00,0x82,0x02,	0x00,0x83,0x02,0x00,0x00,0xe4,0x00,0x00,	//... 0x60	//... normal quad
//	0x90,0x02,0x00,0x91,0x02,0x00,0x92,0x02,	0x00,0x93,0x02,0x00,0x00,0xe4,0x00,0x00,	//... 0x60	//... non-realtime
	0x00,0x03,0x00,0x00,0x00,0x00,0x02,0x00,	0xff,0xff,0xff,0xff,0xf0,0x00,0x89,0x84,	//... 0x70 
#elif defined(__16CH__)
	0x28,0x00,0x10,0xa0,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xa7,	//... 0x50
	0x80,0x02,0x00,0x81,0x02,0x00,0x82,0x02,	0x00,0x83,0x02,0x00,0x00,0xe4,0x00,0x00,	//... 0x60
	0x00,0x03,0x00,0x00,0x00,0x00,0x02,0x00,	0xff,0xff,0xff,0xff,0xf0,0x00,0x89,0x84,	//... 0x70
#endif
//	0xff,0x00,0x51,0x07,0xeb,0x10,0xa8,0x00,	0x51,0xe7,0x80,0x00,0x00,0x00,0x00,0x00,	//... 0x80
	0xdf,0x00,0x91,0x07,0xeb,0x10,0xa8,0x00,	0x91,0xe7,0x80,0x00,0x00,0x00,0x00,0x00,	//... 0x80	//... update:060715
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	//... 0x90
};


#if	defined(__4CH__)
//--------------------------		NTSC		------------------------------//
unsigned char	tbl_ntsc_pg1_enc[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x77,0x23,0xd7,0x01,0x80,0x20,0x00,0x0c,	0x20,0x09,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
//--------------------------		PAL		------------------------------//
unsigned char	tbl_pal_pg1_enc[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x77,0x23,0xd7,0x01,0x80,0x20,0x00,0x0c,	0x20,0x4c,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
#elif	defined(__16CH__)
//--------------------------		NTSC		------------------------------//
unsigned char	tbl_ntsc_pg1_enc_mstr[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x66,0x23,0x97,0x01,0x80,0x20,0x00,0x0c,	0x20,0x09,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
unsigned char	tbl_ntsc_pg1_enc_slv[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x66,0xff,0x97,0x01,0x80,0x20,0x00,0x0c,	0x20,0x09,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
//--------------------------		PAL		------------------------------//
unsigned char	tbl_pal_pg1_enc_mstr[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x66,0x23,0x97,0x01,0x80,0x20,0x00,0x0c,	0x20,0x4c,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
unsigned char	tbl_pal_pg1_enc_slv[] = {
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	0x66,0xff,0x97,0x01,0x80,0x20,0x00,0x0c,	0x20,0x4c,0xaa,0x00,0x40,0x40,0x00,0x00	//... 0xa0
};
#endif
//*
//==================================================================================
//						Page2 initialize table description
//==================================================================================

//--------------------------		PAGE 2		------------------------------//
unsigned char	tbl_pg2_mse_box[] = {		// 80 EA
//  00   01   02   03   04   05   06   07		 08   09   0A   0B   0C   0D   0E   0F
	//... mouse & LUT
	0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0x10
	//... single box
	0x00,0xb0,0x00,0x00,0x00,0x00,0x40,0x00,	0x00,0x00,0x00,0x00,0x00,0xb0,0x00,0x00,	//...  0x20
	0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0x30
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0x40
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x40,0x40,0x40,0x42,0xff	//...  0x50
};


unsigned char	tbl_ntsc_pg2_2dbox[] = {		// 32 EA
	//... 2d array box
#if defined(__4CH__)
	//... quad position
	0x88,0x00,0x01,0x0a,0x00,0x09,0xfb,0x00,	0x89,0x00,0x5b,0x0a,0x00,0x09,0xfb,0x00,	//...  0x60
	0x8a,0x00,0x01,0x0a,0x3c,0x09,0xfb,0x00,	0x8b,0x00,0x5b,0x0a,0x3c,0x09,0xfb,0x00	//...  0x70
#elif defined(__16CH__)
	//... 0 position
	0x88,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	0x89,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	//...  0x60
	0x8a,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	0x8b,0x00,0x00,0x00,0x00,0x00,0xfb,0x00	//...  0x70
#endif
};

unsigned char	tbl_ntsc_pg2_mtn[] = {		// 32 EA
	//... sensitivity & mask info
	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0x80
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	//...  0x90
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xa0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xb0
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xc0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xd0
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xe0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	//...  0xf0
};


unsigned char	tbl_pal_pg2_2dbox[] = {		// 32 EA
	//... 2d array box
#if defined(__4CH__)
	//... quad position
	0x88,0x00,0x01,0x0a,0x00,0x0b,0xfb,0x00,	0x89,0x00,0x5b,0x0a,0x00,0x0b,0xfb,0x00,	//...  0x60
	0x8a,0x00,0x01,0x0a,0x48,0x0b,0xfb,0x00,	0x8b,0x00,0x5b,0x0a,0x48,0x0b,0xfb,0x00	//...  0x70
#elif defined(__16CH__)
	//... 0 position
	0x88,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	0x89,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	//...  0x60
	0x8a,0x00,0x00,0x00,0x00,0x00,0xfb,0x00,	0x8b,0x00,0x00,0x00,0x00,0x00,0xfb,0x00	//...  0x70
#endif
};

unsigned char	tbl_pal_pg2_mtn[] = {		// 32 EA
	//... sensitivity & mask info
	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0x80
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	//...  0x90
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xa0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xb0
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xc0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xd0
//	0x17,0x73,0x88,0x4a,0x07,0x24,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//...  0xe0
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	//...  0xf0
};

//code unsigned char	tbl_ntsc_page2_ary_box[] = {	// 32 EA
//	0x84,0x4c,	1	,0x0a,0x00,0x09,0xfb,0x00,		// 0x60 ~
//	0x84,0x4c,0x5b	,0x0a,0x00,0x09,0xfb,0x00,		// 0x68 ~
//	0x84,0x4c,	1	,0x0a,0x3c,0x09,0xfb,0x00,		// 0x70 ~
//	0x84,0x4c,0x5b	,0x0a,0x3c,0x09,0xfb,0x00		// 0x78 ~
//};
//
//code unsigned char	tbl_pal_page2_ary_box[] = {		// 32 EA
//	0x84,0x4c,	1	,0x0a,0x00,0x0b,0xfb,0x00,		// 0x60 ~
//	0x84,0x4c,0x5b	,0x0a,0x00,0x0b,0xfb,0x00,		// 0x68 ~
//	0x84,0x4c,	1	,0x0a,0x48,0x0b,0xfb,0x00,		// 0x70 ~
//	0x84,0x4c,0x5b	,0x0a,0x48,0x0b,0xfb,0x00		// 0x78 ~
//};
//
//code unsigned char	tbl_page2_motn_sens[] = {		// 6 EA
//	0x17,0x08,0x6a,0x07,0x00,0x24					//... 0x80 ~ 0x85
//};
//
//code unsigned char	tbl_page2_motn_mask[] = {		// 24 EA
//											0x00,0x00,	//... 0x86 ~ 0x87
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//... 0x88 ~ 0x8f
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//... 0x90 ~ 0x97
//	0x00,0x00,0x00,0x00,0x00,0x00					//... 0x98 ~ 0x9d
//};

//	*/
