//==================================================================================
//						Wndwdow box definition & variable description
//==================================================================================
//	on 8x10 font : 1 OSD's BOXHL=8, 1 OSD's BOXHW=4, 1 OSD's BOXVT=11, 1 OSD's BOXVW=5

//==================================================================================
//						Wndwdow box function description
//==================================================================================
extern void SetBoxPopHLPos(U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth);
extern void CreateBoxPopHL(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth);
extern void CreateBoxPopFrame(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght);

//extern void SetOSDWndwFrm(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght);
//extern void SetOSDWndwTtl(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth);
//extern void SetOSDWndwQuit(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y);
//extern void SetOSDWndw(U8 _pth, U8 _box_type_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 code *_str);

