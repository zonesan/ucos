//==================================================================================
//						OSD definition & variable description
//==================================================================================
#define	OSD_WNDW_TYPE_MAIN			0//0x10
#define	OSD_WNDW_TYPE_BTN				1//0x20
#define	OSD_WNDW_TYPE_DECO			2//0x40
#define	OSD_WNDW_TYPE_DT_BLK			3
#define	OSD_WNDW_TYPE_DT_CHK			4
#define	OSD_WNDW_TYPE_DT_MIX			5

#define	OSD_LN_V_L				1
#define	OSD_LN_V_R				2
#define	OSD_LN_V_3D				3
#define	OSD_LN_V_BTN			4
#define	OSD_LN_V_DT_BLK_L		5
#define	OSD_LN_V_DT_BLK_R		6
#define	OSD_LN_V_GRP			5

//==================================================================================
//						OSD function prototype description
//==================================================================================
extern void WriteOSDSqrX(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp, U8 _col);
extern void WriteOSDSqrX_wg(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp, U8 _col, U8 _typ);	//... 180(4 pixel/_pos_x) x 240/288 domain

extern void WriteOSDSqrY(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp, U8 _col);
extern void WriteOSDLnVX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _typ, U16 _hght, U8 _prp, U8 _col);
extern void WriteOSDLnVY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _typ, U16 _vb, U8 _prp, U8 _col);

extern void	SetOSDGrp(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght, U8 _prp, U8 *_str);
//extern void SetOSDWndwFrm(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght);
//extern void SetOSDWndwTtl(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth);
//extern void SetOSDWndwBnd(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght);
//extern void SetOSDWndwQuit(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y);
extern void SetOSDWndw(U8 _pth, U8 _box_type, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 *_str);
//extern U8	GetOSDAdo(U8 _flg_new);//, U8 _flg_old)

