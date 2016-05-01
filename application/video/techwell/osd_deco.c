#include	"hdr.h"
#include        "cmn.h"
#include        "osd.h"
#include        "osd_deco.h"
#include        "prtcl.h"

#include <comm.h>

//==================================================================================
//						OSD function description
//==================================================================================

void OSDClrX_pge(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _pge, U8 _color)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_;
	

	WriteAsicByteOSD(_dvc,0x00,_color);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_color);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_color);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_color);					//... 4th pixel

//... start position
	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_vt>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;
	
        WriteAsicByteOSD(_dvc,0x05,_hl);
        WriteAsicByteOSD(_dvc,0x06,_hr);//79);
        WriteAsicByteOSD(_dvc,0x0a,0xc0|_pge);										//... acceleration|x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7){}	  

}


void OSDClrX(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _page)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_;
	
       if(_page&0x80)
           _t1_ =0x80;
       else
           _t1_ =0xff;
	WriteAsicByteOSD(_dvc,0x00,_t1_);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_t1_);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_t1_);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_t1_);					//... 4th pixel

//... start position
	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_vt>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;

	if (_t2_ != 0) {
		WriteAsicByteOSD(_dvc,0x05,_hl);
		WriteAsicByteOSD(_dvc,0x06,_hl+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,(0xc0 + (_page<<2)));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
		WriteAsicByteOSD(_dvc,0x05,_hl+(_t2_<<4));//_t1_);
		WriteAsicByteOSD(_dvc,0x06,_hl+((_t2_<<4)+_t3_));//79);
		WriteAsicByteOSD(_dvc,0x0a,(0xc0 + (_page<<2)));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}


/*	
        WriteAsicByteOSD(_dvc,0x05,_hl);
        WriteAsicByteOSD(_dvc,0x06,_hr);//79);
        WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7){}	  
*/
}

void OSDClrY(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb,  U8 _page)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_;
	
      if(_page&0x80)
           _t1_ =0x80;
       else
           _t1_ =0xff;
       
	WriteAsicByteOSD(_dvc,0x00,_t1_);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_t1_);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_t1_);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_t1_);					//... 4th pixel

//... start position
	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_vt>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;
	/*
        WriteAsicByteOSD(_dvc,0x05,_hl);
        WriteAsicByteOSD(_dvc,0x06,_hr);//79);
        WriteAsicByteOSD(_dvc,0x0a, ( 0xe0 + (_page<<2) ));										//... acceleration|x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7){}	  
*/
        if (_t2_ != 0) {
		WriteAsicByteOSD(_dvc,0x05,_hl);
		WriteAsicByteOSD(_dvc,0x06,_hl+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,(0xe0 + (_page<<2)));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
		WriteAsicByteOSD(_dvc,0x05,_hl+(_t2_<<4));//_t1_);
		WriteAsicByteOSD(_dvc,0x06,_hl+((_t2_<<4)+_t3_));//79);
		WriteAsicByteOSD(_dvc,0x0a,(0xe0 + (_page<<2)));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
}

void WriteOSDSqrX_wg(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp1, U8 _col, U8 _typ)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_;
    	U8 _ta1_[4];
       U8 _prp, page; 

    	_prp = _prp1 & 0xc0;
    	page = _prp1 & 0x07;
       printf("\n _prp = 0x%x, page = %d!\n", _prp, page); 
	
	switch(_typ){
		case 15:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 7:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;			//... main frame color
			_ta1_[1] = _prp|_col;
			_ta1_[0] = 0xff;
			break;
		case 3:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;;
			_ta1_[1] = 0xff;//_prp|_col;
			_ta1_[0] = 0xff;//_prp|_col;
			break;            
		case 1:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = 0xff;
			break;            
		case 8:
			_ta1_[3] = 0xff;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = _prp|_col;
			break;


		case 12:
			_ta1_[3] = 0xff;//_prp|_col;
			_ta1_[2] = 0xff;//_prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 14:
			_ta1_[3] = 0xff;				//... main frame color
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
			
		default:
			printf("WriteOSDLnVX API err");
				
			break;
		}


	WriteAsicByteOSD(_dvc,0x00,_ta1_[0]);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_ta1_[1]);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_ta1_[2]);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_ta1_[3]);					//... 4th pixel

	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);

	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;
	
        WriteAsicByteOSD(_dvc,0x05,_hl);
        WriteAsicByteOSD(_dvc,0x06,_hr);//79);
        WriteAsicByteOSD(_dvc,0x0a,(0xc0 | (page << 2)));										//... acceleration|x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7){}	
        

}

void WriteOSDSqrX(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp, U8 _col)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_, page;

	_t1_ = (_prp & 0xc0)|_col;
    	page = _prp & 0x07;

    	//printf("\n_t1_ = 0x%x, page = %d!\n", _t1_ , page );

	WriteAsicByteOSD(_dvc,0x00,_t1_);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_t1_);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_t1_);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_t1_);					//... 4th pixel

	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);

	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;

    	//printf("\n_t2_ = %x, _t3_ = %x !\n", _t2_ , _t3_ );

	if (_t2_ != 0) {
		WriteAsicByteOSD(_dvc,0x05,_hl);
		WriteAsicByteOSD(_dvc,0x06,_hl+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,(0xc0 | page <<2));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
		WriteAsicByteOSD(_dvc,0x05,_hl+(_t2_<<4));//_t1_);
		WriteAsicByteOSD(_dvc,0x06,_hl+((_t2_<<4)+_t3_));//79);
		WriteAsicByteOSD(_dvc,0x0a, (0xc0 | page <<2));										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	
	
/*
        WriteAsicByteOSD(_dvc,0x05,_hl);
        WriteAsicByteOSD(_dvc,0x06,_hr);//79);
        WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7){}	
*/
}
//==================================================================================
void WriteOSDSqrY(U8 _dvc, U8 _hl, U8 _hr, U16 _vt, U16 _vb, U8 _prp1, U8 _col1)	//... 90(8 pixel/_pos_x) x 240/288 domain
{
	U8 _t1_, _t2_, _t3_;
    	U8 _prp, page, _col;
      
	_prp = _prp1 & 0xc0;
	page = _prp1 & 0x03;
	_col = _col1 & 0x03;
    
	//printf("\n _prp = 0x%x, _col = 0x%x\n",  _prp,  _col);
	_t1_ = (_prp+(_prp>>4))|((_col<<4)+_col);

	WriteAsicByteOSD(_dvc,0x00,_t1_);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_t1_);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_t1_);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_t1_);					//... 4th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//	WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

//... start position
	WriteAsicByteOSD(_dvc,0x07,_vt&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_vt>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_vt>>8)<<2)+(_vb>>8));	//... ~ even field

	_t2_ = (_hr-_hl)>>4;
	_t3_ = (_hr-_hl)&0x0f;
	
	if (_t2_ != 0) {
		WriteAsicByteOSD(_dvc,0x05,_hl);
		WriteAsicByteOSD(_dvc,0x06,_hl+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
		WriteAsicByteOSD(_dvc,0x05,_hl+(_t2_<<4));//_t1_);
		WriteAsicByteOSD(_dvc,0x06,_hl+((_t2_<<4)+_t3_));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	

}
//==================================================================================
void WriteOSDLnVX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _typ, U16 _vb, U8 _prp1, U8 _col)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _ta1_[4];

	U8 page, _prp;
    
    page = _prp1 & 0x07;
    _prp = _prp1 & 0xc0;
    //printf("/npage = %d, _prp = 0x%x", page, _prp);
	    
	

	switch(_typ){
		case 15:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 1:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = 0xff;
			break;
		case 2:
			_ta1_[3] = 0xff;
			_ta1_[2] = _prp|_col;
			_ta1_[1] = 0xff;
			_ta1_[0] = 0xff;
			break;
		case 4:
			_ta1_[3] = 0xff;
			_ta1_[2] = 0xff;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = 0xff;
			break;            
		case 8:
			_ta1_[3] = 0xff;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = _prp|_col;
			break;
		case 3:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;;
			_ta1_[1] = 0xff;//_prp|_col;
			_ta1_[0] = 0xff;//_prp|_col;
			break;
		case 6:
			_ta1_[3] = 0xff;
			_ta1_[2] = _prp|_col;;
			_ta1_[1] = _prp|_col;//_prp|_col;
			_ta1_[0] = 0xff;//_prp|_col;
			break;
		case 12:
			_ta1_[3] = 0xff;//_prp|_col;
			_ta1_[2] = 0xff;//_prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 7:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;			//... main frame color
			_ta1_[1] = _prp|_col;
			_ta1_[0] = 0xff;
			break;
		case 14:
			_ta1_[3] = 0xff;				//... main frame color
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
			
		default:
			printf("WriteOSDLnVX API err");
				
			break;
		}
		


	WriteAsicByteOSD(_dvc,0x00,_ta1_[0]);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_ta1_[1]);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_ta1_[2]);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_ta1_[3]);					//... 4th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//	WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

//... start position
	WriteAsicByteOSD(_dvc,0x05,_pos_x);
	WriteAsicByteOSD(_dvc,0x07,_pos_y&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x06,_pos_x);
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_pos_y>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_pos_y>>8)<<2)+(_vb>>8));
	WriteAsicByteOSD(_dvc,0x0a,(0xc0 | (page << 2)));										//... acceleration|x path
	while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field

#if	defined(__EN_ISR__)
//	EA = 1;
#endif
}
//==================================================================================
void WriteOSDLnVY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _typ, U16 _vb, U8 _prp1, U8 _col1)	//... 90(8 pixel/_pos_x) x 240/288 domain
{
	U8 _ta1_[4];

	U8 _prp, page, _col;
    
    	_prp = _prp1 & 0xc0;
        _prp |= (_prp >>4);
       page = _prp1 & 0x07;
       _col = (_col1&0x03) | ((_col1&0x03)<<4);
       //printf("\npage = %d, _prp = 0x%x, _typ = %d, _col = 0x%x\n", page, _prp, _typ, _col);

	switch(_typ){
		case 15:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 1:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = 0xff;
			break;
		case 2:
			_ta1_[3] = 0xff;
			_ta1_[2] = _prp|_col;
			_ta1_[1] = 0xff;
			_ta1_[0] = 0xff;
			break;
		case 4:
			_ta1_[3] = 0xff;
			_ta1_[2] = 0xff;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = 0xff;
			break;            
		case 8:
			_ta1_[3] = 0xff;
			_ta1_[2] = 0xff;
			_ta1_[1] = 0xff;
			_ta1_[0] = _prp|_col;
			break;
		case 3:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;;
			_ta1_[1] = 0xff;//_prp|_col;
			_ta1_[0] = 0xff;//_prp|_col;
			break;
		case 6:
			_ta1_[3] = 0xff;
			_ta1_[2] = _prp|_col;;
			_ta1_[1] = _prp|_col;//_prp|_col;
			_ta1_[0] = 0xff;//_prp|_col;
			break;
		case 12:
			_ta1_[3] = 0xff;//_prp|_col;
			_ta1_[2] = 0xff;//_prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
		case 7:
			_ta1_[3] = _prp|_col;
			_ta1_[2] = _prp|_col;			//... main frame color
			_ta1_[1] = _prp|_col;
			_ta1_[0] = 0xff;
			break;
		case 14:
			_ta1_[3] = 0xff;				//... main frame color
			_ta1_[2] = _prp|_col;
			_ta1_[1] = _prp|_col;
			_ta1_[0] = _prp|_col;
			break;
			
		default:
			printf("WriteOSDLnVX API err");
				
			break;
		}

	WriteAsicByteOSD(_dvc,0x00,_ta1_[0]);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,_ta1_[1]);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,_ta1_[2]);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,_ta1_[3]);					//... 4th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//	WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

//... start position
	WriteAsicByteOSD(_dvc,0x05,_pos_x);
	WriteAsicByteOSD(_dvc,0x07,_pos_y&0xff);
//... end position
	WriteAsicByteOSD(_dvc,0x06,_pos_x);
	WriteAsicByteOSD(_dvc,0x08,_vb&0xff);

//	WriteAsicByteOSD(_dvc,0x09,0x02|((_pos_y>>8)<<2)+(_vb>>8));	//... ~ even field
	WriteAsicByteOSD(_dvc,0x09,((_pos_y>>8)<<2)+(_vb>>8));
	WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
	while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field

#if	defined(__EN_ISR__)
//	EA = 1;
#endif
}
//==================================================================================
//==================================================================================
void	SetOSDGrp(U8 _pth, U8 _pos_x, U8 _pos_y, U8 _lngth, U8 _hght, U8 _prp, U8 *_str)
{
	U8	_lngth_ = GetCodeLength(_str);
	U8 _hl_, _hr_, _vt_, _vb_;
	U8 _t1_;//, _t2_;

	_hl_ = _pos_x-1;
	_hr_ = _hl_+_lngth+1;
	_vt_ = _pos_y-1;
	_vb_ = _vt_+_hght+1;

	WriteOSDFnt(_pth,MASTER,_hl_,_vt_,_prp,OSD_GRP_LT);
	WriteOSDFnt(_pth,MASTER,_hl_,_vb_,_prp,OSD_GRP_LB);
	WriteOSDFnt(_pth,MASTER,_hr_,_vt_,_prp,OSD_GRP_RT);
	WriteOSDFnt(_pth,MASTER,_hr_,_vb_,_prp,OSD_GRP_RB);
	
	if(_str != str_null)
		WriteOSDStr(_pth,MASTER,_pos_x,_vt_,_prp,_str);
	else
		WriteOSDFnt(_pth,MASTER,_pos_x,_vt_,_prp,OSD_GRP_LH);

//	_t2_ = _hr_-1;
//	for(_t1_=_pos_x;_t1_<=_t2_;_t1_++){
	for(_t1_=_pos_x;_t1_<_hr_;_t1_++){
		if(_t1_>=(_pos_x+_lngth_))	WriteOSDFnt(_pth,MASTER,_t1_,_vt_,_prp,OSD_GRP_LH);
		WriteOSDFnt(_pth,MASTER,_t1_,_vb_,_prp,OSD_GRP_LH);
	}
//	_t2_ = _vb_-1;
//	for(_t1_=_pos_y;_t1_<=_t2_;_t1_++){
	for(_t1_=_pos_y;_t1_<_vb_;_t1_++){
		WriteOSDFnt(_pth,MASTER,_hl_,_t1_,_prp,OSD_GRP_LV);
		WriteOSDFnt(_pth,MASTER,_hr_,_t1_,_prp,OSD_GRP_LV);
	}

//... 수정중
//	U8 _hl_, _hr_, _vt_, _vb_;
//
//	_hl_ = ((_pos_x-1)<<2) - 2;//3;						//... hl
//	_hr_ = _hl_ + ((_lngth+1)<<2) + 3;//5;					//... hr
//	_vt_ = ((_pos_y-1)*10) - 1;							//... vt
//	_vb_ = _vt_ + ((_hght+1)*10) + 2;// + 3;//			//... vb
//
//	WriteOSDLnVY(MASTER,_hl_-1,_vt_-1,OSD_LN_V_GRP,_vb_+2,_prp,0x00);		//... HL
//	WriteOSDLnVY(MASTER,_hr_,_vt_,OSD_LN_V_GRP,_vb_+1,_prp,0x00);			//... HR
//
//	WriteOSDSqrY(MASTER,_hl_,_hr_,OSD_POS_V_EVEN|(_vt_-2),OSD_POS_V_EVEN|(_vt_-1),_prp,OSD_COL_BLK);	//... VT
//	WriteOSDSqrY(MASTER,_hl_,_hr_,_vt_-1,_vt_,_prp,OSD_COL_75WHT);	//... VT
//	WriteOSDSqrY(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vt_+0),OSD_POS_V_ODD|(_vt_+1),_prp,OSD_COL_BLK);	//... VT
//
//	WriteOSDSqrY(MASTER,_hl_,_hr_,OSD_POS_V_EVEN|(_vb_-2),OSD_POS_V_EVEN|(_vb_-1),_prp,OSD_COL_BLK);	//... VT
//	WriteOSDSqrY(MASTER,_hl_,_hr_,_vb_-1,_vb_,_prp,OSD_COL_75WHT);	//... VT
//	WriteOSDSqrY(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vb_+0),OSD_POS_V_ODD|(_vb_+1),_prp,OSD_COL_BLK);	//... VT
//
//	WriteOSDFnt(_pth,MASTER,_pos_x-1,_pos_y-1,_prp,OSD_GRP_LT);
//	WriteOSDFnt(_pth,MASTER,_pos_x-1,_vt_+_hght+1,_prp,OSD_GRP_LB);
//	WriteOSDFnt(_pth,MASTER,_hl_+_lngth+1,_pos_y-1,_prp,OSD_GRP_RT);
//	WriteOSDFnt(_pth,MASTER,_hl_+_lngth+1,_vt_+_hght+1,_prp,OSD_GRP_RB);
//
//	WriteOSDStr(_pth,MASTER,_pos_x,_pos_y-1,_prp,_str);
}
//==================================================================================
//void SetOSDWndwFrm(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght)
//{
//	_osd_pos_x = (_osd_pos_x<<2) - 2;//3;						//... hl
//	_lngth = _osd_pos_x + (_lngth<<2) + 4;//5;					//... hr
//	_osd_pos_y = (_osd_pos_y*10) - 1;						//... vt
//	_hght = _osd_pos_y + (_hght*10) + 3;//10;					//... vb
//
//	//... frame
//	WriteOSDSqrX(MASTER,_osd_pos_x,_lngth,_osd_pos_y,_hght,OSD_MIX,OSD_COL_BLU);	//... 16x20 font 기준 아님.
//}
////==================================================================================
//void SetOSDWndwTtl(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth)
//{
//	_osd_pos_x = (_osd_pos_x<<2) - 2;//3;						//... hl
//	_lngth = _osd_pos_x + (_lngth<<2) + 4;//3;//					//... hr
//	_osd_pos_y = (_osd_pos_y*10);//-1;								//... vt
//
//	WriteOSDSqrX(MASTER,_osd_pos_x,_lngth,_osd_pos_y,_osd_pos_y+10,OSD_MIX,OSD_COL_RED);	//... 16x20 font 기준 아님.
//}
////==================================================================================
//void SetOSDWndwBnd(U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght)
//{
//	_osd_pos_x = (_osd_pos_x<<2) - 2;//3;						//... hl
//	_lngth = _osd_pos_x + (_lngth<<2) + 4;//5;					//... hr
//	_osd_pos_y = (_osd_pos_y*10) - 1;						//... vt
//	_hght = _osd_pos_y + (_hght*10) + 3;//10;					//... vb
//
//	//... boundary (3D)
//	WriteOSDLnVX(MASTER,_osd_pos_x-1,_osd_pos_y,OSD_LN_V_R,_hght+2,OSD_MIX,OSD_COL_100WHT);	//HL... 16x20 font 기준 아님.
//	WriteOSDLnVX(MASTER,_lngth,_osd_pos_y+1,OSD_LN_V_3D,_hght,OSD_MIX,0x00);		//HR 2 Type... 16x20 font 기준 아님.
//	WriteOSDSqrX(MASTER,_osd_pos_x,_lngth,_osd_pos_y,_osd_pos_y+1,OSD_MIX,OSD_COL_100WHT);				//VT... 16x20 font 기준 아님.
//	WriteOSDSqrX(MASTER,_osd_pos_x,_lngth,_hght,_hght+1,OSD_MIX,OSD_COL_25WHT);			//VB TOP... 16x20 font 기준 아님.
//	WriteOSDSqrX(MASTER,_osd_pos_x,_lngth+1,_hght+1,_hght+2,OSD_MIX,OSD_COL_BLK);		//VB BOT... 16x20 font 기준 아님.
//}
//==================================================================================
//void SetOSDWndwQuit(U8 _pth, U8 _osd_pos_x, U8 _osd_pos_y)
//{
//	U16 _boxhl_, _boxvt_;
//
//	_boxhl_ = (_osd_pos_x<<3)-2;
//	_boxvt_ = (_osd_pos_y*10)+OSD_DLY_V;
//
//	SetBox(_box_num,_boxhl_,6,_boxvt_,5);
////	SetBoxMix(_box_num,_box_num,ON);
////	SetBoxBnd(_box_num,_box_num,BOX_OBND);
////	EnableBox(_pth,_box_num,_box_num);
//	SetBoxMix(_pth,_box_num,ON);
//	SetBoxBnd(_pth,_box_num,ON);//BOX_OBND);
//	EnableBox(_pth,_box_num);//,_box_num);
//
//	WriteOSDFnt(_pth,MASTER,_osd_pos_x,_osd_pos_y,0x00,OSD_QUIT);
//}
//==================================================================================

void SetOSDWndw(U8 _pth, U8 _box_type, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 *_str)
{
	U8 _hl_,_hr_;
	U16 _vt_,_vb_;

	if(_box_type == OSD_WNDW_TYPE_MAIN){
		_hl_ = (_osd_pos_x<<2) - 2;//3;						//... hl
		_hr_ = _hl_ + (_lngth<<2) + 3;//5;					//... hr
		_vt_ = (_osd_pos_y*10) - 1;						//... vt
		_vb_ = _vt_ + (_hght*10) + 3;//10;					//... vb
	
		//... main frame
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_,OSD_MIX,OSD_COL_BLU);
		//... title bar
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|(_vt_+11),OSD_MIX,OSD_COL_RED);
		//... boundary (3D)
		WriteOSDLnVX(MASTER,_hl_-1,OSD_POS_V_ODD|(_vt_-1),OSD_LN_V_R,OSD_POS_V_EVEN|(_vb_+1),OSD_MIX,OSD_COL_100WHT);	//... HL
		WriteOSDLnVX(MASTER,_hr_,OSD_POS_V_ODD|_vt_,OSD_LN_V_3D,OSD_POS_V_EVEN|_vb_,OSD_MIX,0x00);						//... HR 2 Type

		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vt_-1),OSD_POS_V_EVEN|(_vt_-1),OSD_MIX,OSD_COL_100WHT);					//... VT
		WriteOSDSqrX(MASTER,_hl_,_hr_-1,OSD_POS_V_ODD|_vb_,OSD_POS_V_EVEN|_vb_,OSD_MIX,OSD_COL_25WHT);				//... VB TOP
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vb_+1),OSD_POS_V_EVEN|(_vb_+1),OSD_MIX,OSD_COL_BLK);				//... VB BOT

		WriteOSDFnt(_pth,MASTER,_osd_pos_x+_lngth-1,_osd_pos_y,0x00,OSD_QUIT);
		//WriteAsicString(_pth,_osd_pos_x,_osd_pos_y,0x00,_str);
		WriteOSDStr(_pth,MASTER,_osd_pos_x,_osd_pos_y,0x00,_str);
	}
	else if(_box_type == OSD_WNDW_TYPE_BTN){
		_hl_ = (_osd_pos_x<<2) - 2;//3;						//... hl
		_hr_ = _hl_ + (_lngth<<2) + 3;//5;					//... hr
		_vt_ = (_osd_pos_y*10) - 1;						//... vt
		_vb_ = _vt_ + (_hght*10) + 2;// + 3;//					//... vb
	
		//... boundary (3D)
		WriteOSDLnVX(MASTER,_hl_-1,OSD_POS_V_ODD|(_vt_-1),OSD_LN_V_BTN,OSD_POS_V_EVEN|(_vb_+1),OSD_MIX,0x00);	//... HL
		WriteOSDLnVX(MASTER,_hr_,OSD_POS_V_ODD|_vt_,OSD_LN_V_3D,OSD_POS_V_EVEN|_vb_,OSD_MIX,0x00);						//... HR

		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vt_-1),OSD_POS_V_EVEN|(_vt_-1),OSD_MIX,OSD_COL_75WHT);					//... VT
		WriteOSDSqrX(MASTER,_hl_,_hr_-1,OSD_POS_V_ODD|_vb_,OSD_POS_V_EVEN|_vb_,OSD_MIX,OSD_COL_25WHT);				//... VB TOP
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|(_vb_+1),OSD_POS_V_EVEN|(_vb_+1),OSD_MIX,OSD_COL_BLK);				//... VB BOT
	}
	else if(_box_type == OSD_WNDW_TYPE_DT_BLK){
		_hl_ = (_osd_pos_x<<2) + 1;				//... hl
		_hr_ = _hl_ + 1;								//... hr
		_vt_ = (_osd_pos_y*10) + 1;				//... vt
		_vb_ = _vt_ + 4;								//... vb
	
		//... boundary (3D)
		WriteOSDLnVX(MASTER,_hl_,OSD_POS_V_ODD|_vt_,OSD_LN_V_DT_BLK_L,OSD_POS_V_EVEN|_vb_,0x00,OSD_COL_YLW);			//... HL
		WriteOSDLnVX(MASTER,_hr_,OSD_POS_V_ODD|_vt_,OSD_LN_V_DT_BLK_R,OSD_POS_V_EVEN|_vb_,0x00,OSD_COL_YLW);			//... HR

		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vt_,0x00,OSD_COL_YLW);					//... VT
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vb_,OSD_POS_V_EVEN|_vb_,0x00,OSD_COL_YLW);				//... VB TOP
	}
	else if(_box_type == OSD_WNDW_TYPE_DT_CHK){
		_hl_ = (_osd_pos_x<<2) + 1;				//... hl
		_hr_ = _hl_ + 1;								//... hr
		_vt_ = (_osd_pos_y*10) + 1;				//... vt
		_vb_ = _vt_ + 4;								//... vb
	
		//... boundary (3D)
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_,0x00,OSD_COL_YLW);
	}
	else if(_box_type == OSD_WNDW_TYPE_DT_MIX){
		_hl_ = (_osd_pos_x<<2) + 1;				//... hl
		_hr_ = _hl_ + 1;								//... hr
		_vt_ = (_osd_pos_y*10) + 1;				//... vt
		_vb_ = _vt_ + 4;								//... vb
	
		//... boundary (3D)
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_,OSD_MIX,OSD_COL_OCN);
	}
	else if(_box_type == OSD_WNDW_TYPE_DECO){
		_hl_ = (_osd_pos_x<<2) - 3;						//... hl
		_hr_ = _hl_ + (_lngth<<2) + 4;					//... hr
		_vt_ = (_osd_pos_y*10) - 5;						//... vt
		_vb_ = _vt_ + (_hght*10) + 9;//10;					//... vb
	
		//... main frame
		WriteOSDSqrX(MASTER,_hl_,_hr_,OSD_POS_V_ODD|_vt_,OSD_POS_V_EVEN|_vb_,OSD_MIX,OSD_COL_ORNG);
	}
//	else if(_box_type == OSD_CLR){
//	}
}


//==================================================================================
//U8	GetOSDAdo(U8 _flg_new)//, U8 _flg_old)
//{
//	U16 _wt1_, _wt2_;
//	
////	TR0 = 1;	//... start timer0
//	
//	_wt2_ = ((tbl_osd_irq_spkr[OSD_TBL_V_SZ_MSB]<<8)+tbl_osd_irq_spkr[OSD_TBL_V_SZ_LSB])>>1;
//	
//	if(b_cmn_jp_vdo == VDO_NTSC)	_wt1_ = OSD_SPKR01_POS_Y;
//	else	_wt1_ = OSD_SPKR01_POS_Y+(_wt2_<<1);		//... 10 line x 2
//
////	if(BitClear(_flg_new,BIT0))
////		WriteOSDBmpX(MASTER,OSD_SPKR02_POS_X,_wt1_,0x00,0x01,tbl_osd_irq_spkr);
////	else
////		WriteOSDBmpX(MASTER,OSD_SPKR02_POS_X,_wt1_,0x00,0x00,tbl_osd_irq_spkr);
////
////	if(BitClear(_flg_new,BIT1))
////		WriteOSDBmpX(MASTER,OSD_SPKR13_POS_X,_wt1_,0x00,0x02,tbl_osd_irq_spkr);
////	else
////		WriteOSDBmpX(MASTER,OSD_SPKR13_POS_X,_wt1_,0x00,0x03,tbl_osd_irq_spkr);
////
////	if(BitClear(_flg_new,BIT2))
////		WriteOSDBmpX(MASTER,OSD_SPKR02_POS_X,_wt1_+_wt2_,0x00,0x01,tbl_osd_irq_spkr);
////	else
////		WriteOSDBmpX(MASTER,OSD_SPKR02_POS_X,_wt1_+_wt2_,0x00,0x00,tbl_osd_irq_spkr);
//
//	if(BitClear(_flg_new,BIT3))
//		WriteOSDBmpX(MASTER,OSD_SPKR13_POS_X,_wt1_+_wt2_,0x00,0x02,tbl_osd_irq_spkr);
//	else
//		WriteOSDBmpX(MASTER,OSD_SPKR13_POS_X,_wt1_+_wt2_,0x00,0x03,tbl_osd_irq_spkr);
//
////	TR0 = 0;	//... stop timer0
//
//	return	_flg_new;
//}
////==================================================================================

