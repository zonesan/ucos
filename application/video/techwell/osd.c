#include	"hdr.h"
#include	"cmn.h"
#include	"osd.h"
#include	"prtcl.h"
#include	"wndw_ds_scrn.h"
#include	"run.h"

#include	"tbl_osd.h"
#include	"tbl_str.h"

#include "tw_iic.h"
//==================================================================================
//						OSD function description
//==================================================================================

//void WriteOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 *_bmp_ptr)
//{
////	U8 _wdat_;
////	U8 _t1_;
//	U8 _wdth_, _num_h_, _num_v_;
//	U16 _hght_, _ofst_indx_, _ofst_v_;
//	
//#if	defined(__EN_ISR__)
////	EA = 0;
//#endif
//
////	_hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB])>>_bmp_ptr[OSD_TBL_RSLTN];
////... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
//	_hght_ = (_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
////	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ];
//	
//	_ofst_indx_ = (_bmp_ptr[OSD_TBL_H_SZ]*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
//	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
//	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_bmp_ptr[OSD_TBL_H_SZ]);	//... add array_index to offset for 1 index column
//
////	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... horizontal size / 4(pixels)
//
//	WriteAsicByteOSD(_dvc,0x06,0x00);
//	WriteAsicByteOSD(_dvc,0x08,0x00);
//
//	for(_num_v_=0;_num_v_<_hght_;_num_v_+=(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
//		
//		for(_num_h_=0;_num_h_<_bmp_ptr[OSD_TBL_H_SZ];_num_h_+=4){
//			_ofst_v_ = _num_v_*(_bmp_ptr[OSD_TBL_H_SZ]*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 
//
////			_wdat_ = _bmp_ptr[_ofst_indx_+_ofst_v_+_num_h_];
//
//			WriteAsicByteOSD(_dvc,0x00,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_]);						//... 1st pixel
//			WriteAsicByteOSD(_dvc,0x01,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+1]);					//... 2nd pixel
//			WriteAsicByteOSD(_dvc,0x02,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+2]);					//... 3rd pixel
//			WriteAsicByteOSD(_dvc,0x03,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+3]);					//... 4th pixel
//			
//	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
//			
//			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
////			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
//			
//			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
////			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
////			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
//			WriteAsicByteOSD(_dvc,0x09,((_pos_y+_num_v_)>>8)<<2);
//
////			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
//			WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
//			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
//
//			if(_bmp_ptr[OSD_TBL_RSLTN]){
//				_ofst_v_ = (_num_v_+1)*(_bmp_ptr[OSD_TBL_H_SZ]*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 
//				
//				WriteAsicByteOSD(_dvc,0x00,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_]);	//... 1st pixel
//				WriteAsicByteOSD(_dvc,0x01,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+1]);	//... 2nd pixel
//				WriteAsicByteOSD(_dvc,0x02,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+2]);	//... 3rd pixel
//				WriteAsicByteOSD(_dvc,0x03,_prp|_bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+3]);	//... 4th pixel
//			
//		//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//				WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
//				
//				WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//	//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
//				
//				WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//	//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//	//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
//				WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+_num_v_)>>8)<<2));
//	
//	//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
//				WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
//				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
//			}
//		}
//	}
//
//#if	defined(__EN_ISR__)
////	EA = 1;
//#endif
//}
//==================================================================================
void WriteOSDBmp_pointX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 color_1, U8 color_2, U8  color_3, U8 color_4, U8 attribute)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
        U8 _wdat_;
        U8 _t1_;
        U16  _num_h_, _num_v_;
        U16 _hght_, _ofst_indx_, _ofst_v_;
	
        _num_h_ = _pos_x;
        _num_v_ = _pos_y;
        

        //WriteAsicByteOSD(_dvc,0x06,0x00);
        //WriteAsicByteOSD(_dvc,0x08,0x00);

        attribute = attribute & 0xc0;
        _t1_ = (color_1&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x00,attribute|_t1_);					//... 1st pixel
        _t1_ = (color_2&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x01,attribute|_t1_);					//... 2nd pixel
        _t1_ = (color_3&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x02,attribute|_t1_);					//... 3rd pixel
        _t1_ = (color_4&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x03,attribute|_t1_);					//... 4th pixel


        WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

        WriteAsicByteOSD(_dvc,0x05,_num_h_);
        WriteAsicByteOSD(_dvc,0x06,_num_h_);

        WriteAsicByteOSD(_dvc,0x07,(U8)((_num_v_)&0xff));
        WriteAsicByteOSD(_dvc,0x08,(U8)((_num_v_)&0xff));
        _t1_ = 0;
        if(_pos_y > 255) _t1_ = 0x5;

        WriteAsicByteOSD(_dvc,0x09, _t1_);

        WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


        _t1_ = (color_1&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x00,attribute|_t1_);					//... 1st pixel
        _t1_ = (color_2&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x01,attribute|_t1_);					//... 2nd pixel
        _t1_ = (color_3&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x02,attribute|_t1_);					//... 3rd pixel
        _t1_ = (color_4&0x3f);
        if(!_t1_)	_t1_ = 0xff;
        WriteAsicByteOSD(_dvc,0x03,attribute|_t1_);					//... 4th pixel

        WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

        WriteAsicByteOSD(_dvc,0x05,_num_h_);
        //WriteAsicByteOSD(_dvc,0x06,_num_h_);

        WriteAsicByteOSD(_dvc,0x07,(U8)((_num_v_)&0xff));
        //WriteAsicByteOSD(_dvc,0x08,(U8)((_num_v_)&0xff));

        _t1_ = 0;
        if(_pos_y > 255) _t1_ = 0x5;

        WriteAsicByteOSD(_dvc,0x09,0x0a|_t1_);

        WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
        while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field



}

#if 1
#if 1
void FillOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp_pg, U8 _indx, U8 *_bmp_ptr, U8 attribute)
{

    U8 _t1_;
    U8 _pg_;//, _bf_sz_;
    U8 _bf_dt_[12];
    U16 _num_h_, _num_v_;
    U16 _wdth_, _hght_;
    U16 _wt1_, _wt2_;
    U16 _ofst_indx_, _ofst_v_od_, _ofst_v_evn_;


    //_pg_ = _prp_pg&0x07;
    //_prp_pg &= 0xc0;

	_pg_ = _prp_pg;
	//_prp_pg &= 0xc0;	

    _hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB])<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
    _wdth_ = ((_bmp_ptr[OSD_TBL_H_SZ])<<8)+_bmp_ptr[OSD_TBL_RSLTN];

    _ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
    _ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
    _ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column


    //WriteAsicByteOSD(_dvc,0x06,0x00);
    //WriteAsicByteOSD(_dvc,0x08,0x00);

    _wt1_ = _wdth_*_bmp_ptr[OSD_TBL_CLMN];

    for(_num_v_=0;_num_v_<_hght_;_num_v_+=2){//(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
        _ofst_v_od_ = _num_v_*_wt1_;
        _ofst_v_evn_ = _ofst_v_od_+_wt1_;//(_num_v_+1)*_wt1_;

        //WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);

        for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
            _t1_ = (_num_h_>>2)&0x0f;								//... define max buffer size

            _wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_od_+_num_h_;		//
            _bf_dt_[0] = attribute|_bmp_ptr[_wt2_+0];
            _bf_dt_[1] = attribute|_bmp_ptr[_wt2_+1];
            _bf_dt_[2] = attribute|_bmp_ptr[_wt2_+2];
            _bf_dt_[3] = attribute|_bmp_ptr[_wt2_+3];
            _bf_dt_[4] = 0x80+_t1_;
            if((_t1_==0x0f)||((_num_h_+4)>=_wdth_))
            {
    	         _bf_dt_[5] = _pos_x+(_num_h_>>2)-_t1_;
    	         _bf_dt_[6] = 0x00;
    	         _bf_dt_[7] = (_pos_y+(_num_v_>>1))&0xff;
    	         _bf_dt_[8] = 0;
    	         _bf_dt_[9] = (_t1_<<4)|(((_pos_y+(_num_v_>>1))>>8)<<2);	 
                TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 10);


           // if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
                //WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
                //WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|(((_pos_y+(_num_v_>>1))>>8)<<2));		
                WriteAsicByteOSD(_dvc,0x0a,0x80+(_pg_<<2));					//... x path
                while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
                continue;
            }            
            int data[1]={0x80};
            while(data[0]&0x80)
                TW_IICReadByteB(_dvc, 0x2, 0x04, data, 1);
            TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);
        }


        for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
            _t1_ = (_num_h_>>2)&0x0f;

            _wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_evn_+_num_h_;
            _bf_dt_[0] = attribute|_bmp_ptr[_wt2_+0];
            _bf_dt_[1] = attribute|_bmp_ptr[_wt2_+1];
            _bf_dt_[2] = attribute|_bmp_ptr[_wt2_+2];
            _bf_dt_[3] = attribute|_bmp_ptr[_wt2_+3];
            _bf_dt_[4] = 0x80+_t1_;
            if((_t1_==0x0f)||((_num_h_+4)>=_wdth_))
            {
	         _bf_dt_[5] = _pos_x+(_num_h_>>2)-_t1_;
	         _bf_dt_[6] = 0x00;
	         _bf_dt_[7] = (_pos_y+(_num_v_>>1))&0xff;
	         _bf_dt_[8] = 0;
	         _bf_dt_[9] = (_t1_<<4)|0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2);	 
                TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 10);



          //  if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
                //WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
                //WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));		
                WriteAsicByteOSD(_dvc,0x0a,0x80+(_pg_<<2));					//... x path
                while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field        
                 continue;
            }            
            int data[1]={0x80};
            while(data[0]&0x80)
                TW_IICReadByteB(_dvc, 0x2, 0x04, data, 1);
            TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);
            
        }
    }
}
#else
void FillOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp_pg, U8 _indx, U8 *_bmp_ptr,  U8 attribute)
{

    U8 _t1_;
    U8 _pg_;//, _bf_sz_;
    U8 _bf_dt_[5];
    U16 _num_h_, _num_v_;
    U16 _wdth_, _hght_;
    U16 _wt1_, _wt2_;
    U16 _ofst_indx_, _ofst_v_od_, _ofst_v_evn_;


    _pg_ = _prp_pg&0x07;
    //_prp_pg &= 0xc0;

    _hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB])<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
    _wdth_ = ((_bmp_ptr[OSD_TBL_H_SZ])<<8)+_bmp_ptr[OSD_TBL_RSLTN];

    _ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
    _ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
    _ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column


    WriteAsicByteOSD(_dvc,0x06,0x00);
    WriteAsicByteOSD(_dvc,0x08,0x00);

    _wt1_ = _wdth_*_bmp_ptr[OSD_TBL_CLMN];

    for(_num_v_=0;_num_v_<_hght_;_num_v_+=2){//(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
        _ofst_v_od_ = _num_v_*_wt1_;
        _ofst_v_evn_ = _ofst_v_od_+_wt1_;//(_num_v_+1)*_wt1_;

        WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);

        for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
            _t1_ = (_num_h_>>2)&0x0f;								//... define max buffer size

            _wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_od_+_num_h_;		//
            _bf_dt_[0] = attribute|_bmp_ptr[_wt2_+0];
            _bf_dt_[1] = attribute|_bmp_ptr[_wt2_+1];
            _bf_dt_[2] = attribute|_bmp_ptr[_wt2_+2];
            _bf_dt_[3] = attribute|_bmp_ptr[_wt2_+3];
            _bf_dt_[4] = 0x80+_t1_;
            //WriteAsicTableOSD(_dvc,0x00,_bf_dt_);
            WriteAsicByteOSD(_dvc,0x00, _bf_dt_[0]);
            WriteAsicByteOSD(_dvc,0x01, _bf_dt_[1]);
            WriteAsicByteOSD(_dvc,0x02, _bf_dt_[2]);
            WriteAsicByteOSD(_dvc,0x03, _bf_dt_[3]);
            WriteAsicByteOSD(_dvc,0x04, _bf_dt_[4]);

            if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
                WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
                WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|(((_pos_y+(_num_v_>>1))>>8)<<2));		

                while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
                WriteAsicByteOSD(_dvc,0x0a,0x80+(_pg_<<2));					//... x path
            }
        }


        for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
            _t1_ = (_num_h_>>2)&0x0f;

            _wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_evn_+_num_h_;
            _bf_dt_[0] = attribute|_bmp_ptr[_wt2_+0];
            _bf_dt_[1] = attribute|_bmp_ptr[_wt2_+1];
            _bf_dt_[2] = attribute|_bmp_ptr[_wt2_+2];
            _bf_dt_[3] = attribute|_bmp_ptr[_wt2_+3];
            _bf_dt_[4] = 0x80+_t1_;
            //WriteAsicTableOSD(_dvc,0x00,_bf_dt_);

            WriteAsicByteOSD(_dvc,0x00, _bf_dt_[0]);
            WriteAsicByteOSD(_dvc,0x01, _bf_dt_[1]);
            WriteAsicByteOSD(_dvc,0x02, _bf_dt_[2]);
            WriteAsicByteOSD(_dvc,0x03, _bf_dt_[3]);
            WriteAsicByteOSD(_dvc,0x04, _bf_dt_[4]);
			
            if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
                WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
                WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));		

                while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
                WriteAsicByteOSD(_dvc,0x0a,0x80+(_pg_<<2));					//... x path
            }
        }
        //printf("\ninterrupt tFillBMP64\n");
    }
}
#endif

#if 1
void WriteOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8  *_bmp_ptr)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _wdat_;
	U8 _t1_;
	U8 _wdth_, _num_h_, _num_v_;
	U16 _hght_, _ofst_indx_, _ofst_v_;
	

	_hght_ = (_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... for old table which 1 byte has 4 pixel info.
	
	_ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<_hght_;_num_v_+=(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
		
		for(_num_h_=0;_num_h_<_wdth_;_num_h_++){
			_ofst_v_ = _num_v_*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

			_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
			
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);

			WriteAsicByteOSD(_dvc,0x09,((_pos_y+(_num_v_>>1))>>8)<<2);

			WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


			if(_bmp_ptr[OSD_TBL_RSLTN]){
				_ofst_v_ = (_num_v_+1)*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

				_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];
	//			_wdat_ = tbl_osd_tmp_even[_ofst_indx_+_ofst_v_+_num_h_];
				
				_t1_ = (_wdat_&0xc0)>>6;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
				_t1_ = (_wdat_&0x30)>>4;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
				_t1_ = (_wdat_&0xc)>>2;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
				_t1_ = _wdat_&0x3;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
				
		//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
				WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
				
				WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
	//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
				
				WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);
	//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
	//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
				WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));
	
	//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
				WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
			}
		}
	}

}
#else
void WriteOSDBmpX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8  *_bmp_ptr)	//... 180(4 pixel/_pos_x) x 240/288 domain
{
	U8 _wdat_;
	U8 _t1_;
	U8 _wdth_, _num_h_, _num_v_;
	U16 _hght_, _ofst_indx_, _ofst_v_;
	

	_hght_ = (_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... for old table which 1 byte has 4 pixel info.
	
	_ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column

//	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... horizontal size / 4(pixels)

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<_hght_;_num_v_+=(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
		
		for(_num_h_=0;_num_h_<_wdth_;_num_h_++){
			_ofst_v_ = _num_v_*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

			_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+(_num_v_>>1))>>8)<<2);

//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
			WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


			if(_bmp_ptr[OSD_TBL_RSLTN]){
				_ofst_v_ = (_num_v_+1)*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

				_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];
	//			_wdat_ = tbl_osd_tmp_even[_ofst_indx_+_ofst_v_+_num_h_];
				
				_t1_ = (_wdat_&0xc0)>>6;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
				_t1_ = (_wdat_&0x30)>>4;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
				_t1_ = (_wdat_&0xc)>>2;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
				_t1_ = _wdat_&0x3;
				if(!_t1_)	_t1_ = 0xff;
				WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
				
		//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
				WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
				
				WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
	//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
				
				WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);
	//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
	//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
				WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));
	
	//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
				WriteAsicByteOSD(_dvc,0x0a,0x80);					//... x path
				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
			}
		}
	}

}

#endif

#endif
//==================================================================================
void WriteOSDBmp_pointY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 color_1, U8 color_2, U8 attribute)	//... 90(8 pixel/_pos_x) x 240/288 domain

{
        U8 _wdat_;
        U8 _t1_;
        U8 _wdth_, _num_h_, _num_v_;
        U16 _hght_, _ofst_indx_, _ofst_v_;

        _num_h_ = _pos_x;
        _num_v_ = (U8)_pos_y;

        WriteAsicByteOSD(_dvc,0x06,0x00);
        WriteAsicByteOSD(_dvc,0x08,0x00);

        _t1_ = ((color_1&0xc0)>>2)+((color_1&0x30)>>4);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x00,attribute|_t1_);					//... 1st, 2nd pixel
        _t1_ = ((color_1&0x0c)<<2)+(color_1&0x03);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x01,attribute|_t1_);					//... 3rd, 4th pixel


        _t1_ = ((color_2&0xc0)>>2)+((color_2&0x30)>>4);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x02,attribute|_t1_);					//... 5th, 6th pixel
        _t1_ = ((color_2&0x0c)<<2)+(color_2&0x03);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x03,attribute|_t1_);					//... 7th, 8th pixel
        

        WriteAsicByteOSD(_dvc, 0x04, 0x80);					//... write to buffer
        WriteAsicByteOSD(_dvc, 0x05, _num_h_);
        WriteAsicByteOSD(_dvc, 0x07, _num_v_);
        WriteAsicByteOSD(_dvc,0x09,((_num_v_>>1)>>8)<<2);
        WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
        while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


        _t1_ = ((color_1&0xc0)>>2)+((color_1&0x30)>>4);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x00,attribute|_t1_);					//... 1st, 2nd pixel
        _t1_ = ((color_1&0x0c)<<2)+(color_1&0x03);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x01,attribute|_t1_);					//... 3rd, 4th pixel


        _t1_ = ((color_2&0xc0)>>2)+((color_2&0x30)>>4);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x02,attribute|_t1_);					//... 5th, 6th pixel
        _t1_ = ((color_2&0x0c)<<2)+(color_2&0x03);
        if(!(_t1_&0x30))	_t1_ |= 0xf0;
        if(!(_t1_&0x03))	_t1_ |= 0x0f;
        WriteAsicByteOSD(_dvc,0x03,attribute|_t1_);					//... 7th, 8th pixel


        WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
        WriteAsicByteOSD(_dvc, 0x05, _num_h_);
        WriteAsicByteOSD(_dvc, 0x07, _num_v_);
        WriteAsicByteOSD(_dvc,0x09,0x0a|((_num_v_>>1)>>8)<<2);
        WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
        while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field

}

#if 1

#if 0
void WriteOSDBmpY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8  *_bmp_ptr)
//... _pos_x: ~ 180(4 pixel/_pos_x), _pos_y: ~ 240/288 domain
{
//	U8 _wdat_;
	U8 _t1_;
//	U8 _pg_;//, _bf_sz_;
	U8 _bf_dt_[5];
	U16 _num_h_, _num_v_;
	U16 _wdth_, _hght_;
	U16 _wt1_, _wt2_;
	U16 _ofst_indx_, _ofst_v_od_, _ofst_v_evn_;
	

	_prp |= _prp>>4;

	_hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB])<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
	_wdth_ = ((_bmp_ptr[OSD_TBL_H_SZ])<<8)+_bmp_ptr[OSD_TBL_RSLTN];
	_wdth_ >>= 1;
	
	_ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	_wt1_ = _wdth_*_bmp_ptr[OSD_TBL_CLMN];

	for(_num_v_=0;_num_v_<_hght_;_num_v_+=2){
		_ofst_v_od_ = _num_v_*_wt1_;
		_ofst_v_evn_ = _ofst_v_od_+_wt1_;//(_num_v_+1)*_wt1_;

		WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);

		for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
			_t1_ = (_num_h_>>2)&0x0f;								//... define max buffer size

			_wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_od_+_num_h_;		//
			_bf_dt_[0] = _prp|_bmp_ptr[_wt2_+0];
			_bf_dt_[1] = _prp|_bmp_ptr[_wt2_+1];
			_bf_dt_[2] = _prp|_bmp_ptr[_wt2_+2];
			_bf_dt_[3] = _prp|_bmp_ptr[_wt2_+3];
			_bf_dt_[4] = 0x80+_t1_;
			TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);

			if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
				WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
				WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|(((_pos_y+(_num_v_>>1))>>8)<<2));		

				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
				WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			}
		}

		for(_num_h_=0;_num_h_<_wdth_;_num_h_+=4){
			_t1_ = (_num_h_>>2)&0x0f;

			_wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_evn_+_num_h_;
			_bf_dt_[0] = _prp|_bmp_ptr[_wt2_+0];
			_bf_dt_[1] = _prp|_bmp_ptr[_wt2_+1];
			_bf_dt_[2] = _prp|_bmp_ptr[_wt2_+2];
			_bf_dt_[3] = _prp|_bmp_ptr[_wt2_+3];
			_bf_dt_[4] = 0x80+_t1_;
			//WriteAsicTableOSD(_dvc,0x00,_bf_dt_);
			TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);

			if((_t1_==0x0f)||((_num_h_+4)>=_wdth_)){
				WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
				WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));		

				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
				WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			}
		}

	}
}
#else
void WriteOSDBmpY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8  *_bmp_ptr)
//... _pos_x: ~ 180(4 pixel/_pos_x), _pos_y: ~ 240/288 domain
{
//	U8 _wdat_;
	U8 _t1_;
//	U8 _pg_;//, _bf_sz_;
	U8 _bf_dt_[5];
	U16 _num_h_, _num_v_;
	U16 _wdth_, _hght_;
	U16 _wt1_, _wt2_;
	U16 _ofst_indx_, _ofst_v_od_, _ofst_v_evn_;
	


	_prp |= _prp>>4;

	_hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB])<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
	_wdth_ = ((_bmp_ptr[OSD_TBL_H_SZ])<<8)+_bmp_ptr[OSD_TBL_RSLTN];
   
	_wdth_ >>= 1;
	
	_ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column
	 printf("\n_hght_ = %d, _edth_ = %d, _ofst_indx_ = %d\n",_hght_, _wdth_ ,_ofst_indx_);
	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	_wt1_ = _wdth_*_bmp_ptr[OSD_TBL_CLMN];

	for(_num_v_=0;_num_v_<_hght_;_num_v_+=2){//(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
		_ofst_v_od_ = _num_v_*_wt1_;
		_ofst_v_evn_ = _ofst_v_od_+_wt1_;//(_num_v_+1)*_wt1_;

		WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);

		for(_num_h_=0;_num_h_<(_wdth_);_num_h_+=4){
			_t1_ = (_num_h_>>2)&0x0f;								//... define max buffer size

			_wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_od_+(_num_h_);		//
#if 1			
			_bf_dt_[0] = _prp|_bmp_ptr[_wt2_+0];
			_bf_dt_[1] = _prp|_bmp_ptr[_wt2_+1];
			_bf_dt_[2] = _prp|_bmp_ptr[_wt2_+2];
			_bf_dt_[3] = _prp|_bmp_ptr[_wt2_+3];
#else
			_bf_dt_[0] = (_bmp_ptr[_wt2_+1] & 0x0f) | ((_bmp_ptr[_wt2_+0]<<4) & 0xf0);
			_bf_dt_[1] = (_bmp_ptr[_wt2_+3] & 0x0f) | ((_bmp_ptr[_wt2_+2]<<4) & 0xf0);
			_bf_dt_[2] = (_bmp_ptr[_wt2_+5] & 0x0f) | ((_bmp_ptr[_wt2_+4]<<4) & 0xf0);
			_bf_dt_[3] = (_bmp_ptr[_wt2_+7] & 0x0f) | ((_bmp_ptr[_wt2_+6]<<4) & 0xf0);
#endif
			_bf_dt_[4] = 0x80+_t1_;
			//WriteAsicTableOSD(_dvc,0x00,_bf_dt_);
			TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);

			if((_t1_==0x0f)||((_num_h_+4)>=(_wdth_))){
				WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
				WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|(((_pos_y+(_num_v_>>1))>>8)<<2));		

				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
				WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			}
		}

//		if(_bmp_ptr[OSD_TBL_RSLTN]){
			for(_num_h_=0;_num_h_<(_wdth_);_num_h_+=4){
				_t1_ = (_num_h_>>2)&0x0f;

				_wt2_ = OSD_TBL_DAT+_ofst_indx_+_ofst_v_evn_+(_num_h_);
#if 1			
			_bf_dt_[0] = _prp|_bmp_ptr[_wt2_+0];
			_bf_dt_[1] = _prp|_bmp_ptr[_wt2_+1];
			_bf_dt_[2] = _prp|_bmp_ptr[_wt2_+2];
			_bf_dt_[3] = _prp|_bmp_ptr[_wt2_+3];
#else
			_bf_dt_[0] = (_bmp_ptr[_wt2_+1] & 0x0f) | ((_bmp_ptr[_wt2_+0]<<4) & 0xf0);
			_bf_dt_[1] = (_bmp_ptr[_wt2_+3] & 0x0f) | ((_bmp_ptr[_wt2_+2]<<4) & 0xf0);
			_bf_dt_[2] = (_bmp_ptr[_wt2_+5] & 0x0f) | ((_bmp_ptr[_wt2_+4]<<4) & 0xf0);
			_bf_dt_[3] = (_bmp_ptr[_wt2_+7] & 0x0f) | ((_bmp_ptr[_wt2_+6]<<4) & 0xf0);
#endif
				_bf_dt_[4] = 0x80+_t1_;
				//WriteAsicTableOSD(_dvc,0x00,_bf_dt_);
				TW_IICWriteByteB(_dvc, 0x2, 0x00, _bf_dt_, 5);

				if((_t1_==0x0f)||((_num_h_+4)>=(_wdth_))){
					WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>2)-_t1_);
					WriteAsicByteOSD(_dvc,0x09,(_t1_<<4)|0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));		

					while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
					WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
				}
			}
//		}
	}

}
#endif
#else

void WriteOSDBmpY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx, U8 *_bmp_ptr)	//... 90(8 pixel/_pos_x) x 240/288 domain
{
	U8 _wdat_;
	U8 _t1_;
	U8 _wdth_, _num_h_, _num_v_;
	U16 _hght_, _ofst_indx_, _ofst_v_;
	
#if	defined(__EN_ISR__)
//	EA = 0;
#endif

	_prp |= _prp>>4;

//	_hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB])>>_bmp_ptr[OSD_TBL_RSLTN];
//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
//	_hght_ = ((_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB])<<_bmp_ptr[OSD_TBL_RSLTN];
	_hght_ = (_bmp_ptr[OSD_TBL_V_SZ_MSB]<<8)+_bmp_ptr[OSD_TBL_V_SZ_LSB];
	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... for old table which 1 byte has 4 pixel info.
	
	_ofst_indx_ = (_wdth_*_hght_)*_bmp_ptr[OSD_TBL_CLMN];
	_ofst_indx_ = _ofst_indx_*(_indx/_bmp_ptr[OSD_TBL_CLMN]);		//... data size of one index row (= (one bitmap icon size)*column)
	_ofst_indx_ = _ofst_indx_+((_indx%_bmp_ptr[OSD_TBL_CLMN])*_wdth_);	//... add array_index to offset for 1 index column

//	_wdth_ = _bmp_ptr[OSD_TBL_H_SZ]>>2;							//... horizontal size / 4(pixels)

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<_hght_;_num_v_+=(1<<_bmp_ptr[OSD_TBL_RSLTN])){	//... if(_bmp_ptr[OSD_TBL_RSLTN]==0)	field resolution, else	frame resolution
		
		for(_num_h_=0;_num_h_<_wdth_;_num_h_+=2){
			_ofst_v_ = _num_v_*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

			_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];

//			_t1_ = (_wdat_&0xc0)>>6;
//			if(!_t1_)	_t1_ = 0xff;
//			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
//			_t1_ = (_wdat_&0x30)>>4;
//			if(!_t1_)	_t1_ = 0xff;
//			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
//			_t1_ = (_wdat_&0xc)>>2;
//			if(!_t1_)	_t1_ = 0xff;
//			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
//			_t1_ = _wdat_&0x3;
//			if(!_t1_)	_t1_ = 0xff;
//			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
//
//			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel

//			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_+1];
			_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel
			
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+(_num_v_>>1))>>8)<<2);

//			WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
			WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


			if(_bmp_ptr[OSD_TBL_RSLTN]){
				_ofst_v_ = (_num_v_+1)*(_wdth_*_bmp_ptr[OSD_TBL_CLMN]);	// _num_v_ * 32;	// 

				_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_];
	//			_wdat_ = tbl_osd_tmp_even[_ofst_indx_+_ofst_v_+_num_h_];
				
//				_t1_ = (_wdat_&0xc0)>>6;
//				if(!_t1_)	_t1_ = 0xff;
//				WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
//				_t1_ = (_wdat_&0x30)>>4;
//				if(!_t1_)	_t1_ = 0xff;
//				WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
//				_t1_ = (_wdat_&0xc)>>2;
//				if(!_t1_)	_t1_ = 0xff;
//				WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
//				_t1_ = _wdat_&0x3;
//				if(!_t1_)	_t1_ = 0xff;
//				WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel

//			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_+1];
			_wdat_ = _bmp_ptr[5+_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel
			
		//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
				WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
				
				WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
	//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
				
				WriteAsicByteOSD(_dvc,0x07,(_pos_y+(_num_v_>>1))&0xff);
	//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
	//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
				WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+(_num_v_>>1))>>8)<<2));
	
//				WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
				WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
				while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
			}
		}
	}

}
#endif
//==================================================================================
//==================================================================================

#if 0
void WriteOSDBmp(U8 _pth, U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx)
{
	if(BitSet(_pth,PTH_X))	WriteOSDBmpX(_dvc,_pos_x,_pos_y,_prp,_indx);
	if(BitSet(_pth,PTH_Y))	WriteOSDBmpY(_dvc,_pos_x,_pos_y,_prp,_indx);
}
#endif
//==================================================================================
void WriteOSDFntX_hys(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp1, U8 _indx,U8 _stringcolor)	//... only one _pth at once
{
	U8 _wdat_;
//	U8 _ta1_[4];
	U8 _t1_;//, _t2_;
	U8 _num_h_, _num_v_;//, num_ofst;
	U16 _ofst_indx_, _ofst_v_;
    	U8 _prp, page;

	_pos_x <<= 2;					//... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char	
	_prp = _prp1&0xc0;
    	page = _prp1 & 0x07;

	_ofst_indx_ = (320 * (_indx>>3)) + ((_indx&0x07)<<2);

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<10;_num_v_++){
		_ofst_v_ = _num_v_<<5;	// _num_v_ * 32;	// 
		
		for(_num_h_=0;_num_h_<4;_num_h_++){

			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_];
//			_wdat_ = tbl_osd_tmp_odd[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
                        
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel

			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+_num_v_)>>8)<<2);			//... start: OSD_POS_V_ODD, end: OSD_POS_V_ODD

//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
			WriteAsicByteOSD(_dvc,0x0a,(0x80 | (page<<2)));					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_];
////			_wdat_ = tbl_osd_tmp_even[_ofst_indx_+_ofst_v_+_num_h_];
			
			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
                    else 
                    { 
                        if(_t1_ == 1) _t1_= _stringcolor+2;
                        else  _t1_= _stringcolor;
                    }
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
			WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+_num_v_)>>8)<<2));	//... start: OSD_POS_V_EVEN, end: OSD_POS_V_EVEN

//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
			WriteAsicByteOSD(_dvc,0x0a,(0x80 | (page<<2)));					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
		}
	}

}

void WriteOSDFntX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp1, U8 _indx)	//... only one _pth at once
{
	U8 _wdat_;
//	U8 _ta1_[4];
	U8 _t1_;//, _t2_;
	U8 _num_h_, _num_v_;//, num_ofst;
	U16 _ofst_indx_, _ofst_v_;
    	U8 _prp, page;

	_pos_x <<= 2;					//... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char	
	_prp = _prp1&0xc0;
    	page = _prp1 & 0x07;

	_ofst_indx_ = (320 * (_indx>>3)) + ((_indx&0x07)<<2);

	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<10;_num_v_++){
		_ofst_v_ = _num_v_<<5;	// _num_v_ * 32;	// 
		
		for(_num_h_=0;_num_h_<4;_num_h_++){

			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_];
//			_wdat_ = tbl_osd_tmp_odd[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel

			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+_num_v_)>>8)<<2);			//... start: OSD_POS_V_ODD, end: OSD_POS_V_ODD

//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
			WriteAsicByteOSD(_dvc,0x0a,(0x80 | (page<<2)));					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field


			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_];
////			_wdat_ = tbl_osd_tmp_even[_ofst_indx_+_ofst_v_+_num_h_];
			
			_t1_ = (_wdat_&0xc0)>>6;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st pixel
			_t1_ = (_wdat_&0x30)>>4;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 2nd pixel
			_t1_ = (_wdat_&0xc)>>2;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 3rd pixel
			_t1_ = _wdat_&0x3;
			if(!_t1_)	_t1_ = 0xff;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 4th pixel
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+_num_h_);
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
			WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+_num_v_)>>8)<<2));	//... start: OSD_POS_V_EVEN, end: OSD_POS_V_EVEN

//			WriteAsicByteOSD(_dvc,0x0a,0xc0);					//... x path
			WriteAsicByteOSD(_dvc,0x0a,(0x80 | (page<<2)));					//... x path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
		}
	}


}

//==================================================================================
void WriteOSDFntY_hys(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp1, U8 _indx, U8 _stringcolor)	//... only one _pth at once
{
	U8 _wdat_;
	U8 _t1_,t2;//, _t2_;
	U8 _num_h_, _num_v_;//, num_ofst;
	U16 _ofst_indx_, _ofst_v_;
//	U8 _pos_x_, _pos_y_;
	U8 _prp, page;
	


	_pos_x <<= 1;														//... (_pos_x)*2 -> 8 pixel * 2 -> 16 pixel char
	_prp = ((_prp1&0xc0)>>4)|(_prp1&0xc0);
    	page = _prp1 & 0x07;

	_ofst_indx_ = (320 * (_indx>>3)) + ((_indx&0x07)<<2);
	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<10;_num_v_++){
		_ofst_v_ = _num_v_<<5;	// _num_v_ * 32;	// 
		
		for(_num_h_=0;_num_h_<4;_num_h_+=2){

			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                    else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
                    WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel
			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel
                    _t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+_num_v_)>>8)<<2);			//... start: OSD_POS_V_ODD, end: OSD_POS_V_ODD

//			WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
			WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field

			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
                    _t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel

			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                     else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel

                    _t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
                    else 
                    {
                        if((_t1_ >>4) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = (t2 <<4) |(_t1_&0x03); //(color+index)%4
                     }
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
                    else  
                    {
                        if((_t1_&0x03) == 1) t2 =( _stringcolor+2) %4;
                        else t2 = _stringcolor;
                        _t1_ = t2 |(_t1_ &0xf0) ;
                    }
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel

	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
			WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+_num_v_)>>8)<<2));	//... start: OSD_POS_V_EVEN, end: OSD_POS_V_EVEN
			//WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2));

//			WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
			WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
		}
	}
}

void WriteOSDFntY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp1, U8 _indx)	//... only one _pth at once
{
	U8 _wdat_;
	U8 _t1_;//, _t2_;
	U8 _num_h_, _num_v_;//, num_ofst;
	U16 _ofst_indx_, _ofst_v_;
//	U8 _pos_x_, _pos_y_;
	U8 _prp, page;
	


	_pos_x <<= 1;														//... (_pos_x)*2 -> 8 pixel * 2 -> 16 pixel char
	_prp = ((_prp1&0xc0)>>4)|(_prp1&0xc0);
    	page = _prp1 & 0x07;

	_ofst_indx_ = (320 * (_indx>>3)) + ((_indx&0x07)<<2);
	WriteAsicByteOSD(_dvc,0x06,0x00);
	WriteAsicByteOSD(_dvc,0x08,0x00);

	for(_num_v_=0;_num_v_<10;_num_v_++){
		_ofst_v_ = _num_v_<<5;	// _num_v_ * 32;	// 
		
		for(_num_h_=0;_num_h_<4;_num_h_+=2){

			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel

			_wdat_ = tbl_osd_16x20_odd[_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8));
			WriteAsicByteOSD(_dvc,0x09,((_pos_y+_num_v_)>>8)<<2);			//... start: OSD_POS_V_ODD, end: OSD_POS_V_ODD

//			WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
			WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field

			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x00,_prp|_t1_);					//... 1st, 2nd pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x01,_prp|_t1_);					//... 3rd, 4th pixel

			_wdat_ = tbl_osd_16x20_even[_ofst_indx_+_ofst_v_+_num_h_+1];

			_t1_ = ((_wdat_&0xc0)>>2)+((_wdat_&0x30)>>4);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x02,_prp|_t1_);					//... 5th, 6th pixel
			_t1_ = ((_wdat_&0x0c)<<2)+(_wdat_&0x03);
			if(!(_t1_&0x30))	_t1_ |= 0xf0;
			if(!(_t1_&0x03))	_t1_ |= 0x0f;
			WriteAsicByteOSD(_dvc,0x03,_prp|_t1_);					//... 7th, 8th pixel
			
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
			WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer
			
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_num_h_>>1));
//			WriteAsicByteOSD(_dvc,0x06,_pos_x+_num_h_);
			
			WriteAsicByteOSD(_dvc,0x07,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x08,(_pos_y+_num_v_)&0xff);
//			WriteAsicByteOSD(_dvc,0x09,0x0a|((((_pos_y+_num_v_)>>8)<<2)+((_pos_y+_num_v_)>>8)));
			WriteAsicByteOSD(_dvc,0x09,0x0a|(((_pos_y+_num_v_)>>8)<<2));	//... start: OSD_POS_V_EVEN, end: OSD_POS_V_EVEN
			//WriteAsicByteOSD(_dvc,0x09,(((_pos_y+_num_v_)>>8)<<2));

//			WriteAsicByteOSD(_dvc,0x0a,0xe0);					//... y path
			WriteAsicByteOSD(_dvc,0x0a,0xa0);					//... y path
			while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
		}
	}

#if	defined(__EN_ISR__)
//	EA = 1;
#endif





//	rINTMSK &= ~(BIT_EINT4_7|BIT_EINT8_23);								//... disable mask bit EINT8_23
}
//==================================================================================
void WriteOSDFnt(U8 _pth, U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _prp, U8 _indx)
{
	if(BitSet(_pth,PTH_X))	WriteOSDFntX(_dvc,_pos_x,_pos_y,_prp,_indx);
	if(BitSet(_pth,PTH_Y))	WriteOSDFntY(_dvc,_pos_x,_pos_y,_prp,_indx);
}

/*  wg add  */
//==================================================================================
void WriteOSDStrX_wg(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr,  U8 _str_len,  U8 _str_color)
{
	U8 _lngth_ = GetCodeLength(_str_ptr);
	U8 _indx_;
	U8 _t1_;
    	
       //printf("\n_pos_x = %d, _pos_y = %d, _prp=0x%x\n", _pos_x, _pos_y, _prp);
	
	for(_t1_=0;_t1_<_str_len;_t1_++){
		_indx_ = _str_ptr[_t1_]-0x20;				//... expand U8 to U8, but not sign bit(MSB of 8bit)
		if((_pos_x+_t1_) > 45) printf("/nDVR@ The string Length is encroach! /n");
		//WriteOSDFntX(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_);
		WriteOSDFntX_hys(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_,_str_color);
	}
}
//==================================================================================
void WriteOSDStrY_wg(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr,  U8 _str_len,  U8 _str_color)
{
	U8 _lngth_ = GetCodeLength(_str_ptr);
	U8 _indx_;
	U8 _t1_;
	//printf("\n_pos_x = %d, _pos_y = %d, _prp = 0x%x, _str_len = %d ! \n", _pos_x, _pos_y, _prp, _str_len );
	for(_t1_=0;_t1_<_str_len;_t1_++){
		_indx_ = _str_ptr[_t1_]-0x20;				//... expand U8 to U8, but not sign bit(MSB of 8bit)
		if((_pos_x+_t1_) > 45) printf("/nDVR@ The string Length is encroach! /n");
		//WriteOSDFntY(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_);
		WriteOSDFntY_hys(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_,_str_color);
	}
}




//==================================================================================
void WriteOSDStrX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr)
{
	U8 _lngth_ = GetCodeLength(_str_ptr);
	U8 _indx_;
	U8 _t1_;
	
	for(_t1_=0;_t1_<_lngth_;_t1_++){
		_indx_ = _str_ptr[_t1_]-0x20;				//... expand U8 to U8, but not sign bit(MSB of 8bit)
		WriteOSDFntX(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_);
	}
}
//==================================================================================
void WriteOSDStrY(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr)
{
	U8 _lngth_ = GetCodeLength(_str_ptr);
	U8 _indx_;
	U8 _t1_;
	
	for(_t1_=0;_t1_<_lngth_;_t1_++){
		_indx_ = _str_ptr[_t1_]-0x20;				//... expand U8 to U8, but not sign bit(MSB of 8bit)
		WriteOSDFntY(_dvc,_pos_x+_t1_,_pos_y,_prp,_indx_);
	}
}
//==================================================================================
void WriteOSDStr(U8 _pth, U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _prp, U8 *_str_ptr)
{
	if(BitSet(_pth,PTH_X))	WriteOSDStrX(_dvc,_pos_x,_pos_y,_prp,_str_ptr);
	if(BitSet(_pth,PTH_Y))	WriteOSDStrY(_dvc,_pos_x,_pos_y,_prp,_str_ptr);
}
//==================================================================================
void WriteOSDClrX(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght)	//... only one _pth at once
{
//	U8 _t1_;
	U8 _t2_, _t3_;
	
#if	defined(__EN_ISR__)
//	EA = 0;
#endif
//	U8 _t1_;

	WriteAsicByteOSD(_dvc,0x00,0xff);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,0xff);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,0xff);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,0xff);					//... 4th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//	WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

//... start position
//	_pos_x <<= 2;														//... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char
	_pos_y = (_pos_y<<1)+(_pos_y<<3);							//... (_pos_y)*(2+8)

//	WriteAsicByteOSD(_dvc,0x05,_pos_x);
	WriteAsicByteOSD(_dvc,0x07,_pos_y&0xff);

//... end position
//	_pos_x += _wdth<<2;
	_hght = _pos_y+((_hght<<1)+(_hght<<3));

//	WriteAsicByteOSD(_dvc,0x06,--_pos_x);
	WriteAsicByteOSD(_dvc,0x08,(--_hght)&0xff);

//	WriteAsicByteOSD(_dvc,0x09,((_pos_y>>8)<<2)+(_hght>>8));		//... odd field
//	WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
//
//	while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
////Delay(1);

	WriteAsicByteOSD(_dvc,0x09,0x02|((_pos_y>>8)<<2)+(_hght>>8));	//... ~ even field
//	WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
//
//	while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
////Delay(1);
	_pos_x <<= 2;
	_wdth <<= 2;
	
	_t2_ = _wdth>>4;
	_t3_ = _wdth&0x0f;
	
//	for(_t1_=0;_t1_<=_t2_;_t1_++){
//		if(_t1_<_t2_){
//			WriteAsicByteOSD(0,0x05,_pos_x+(_t1_<<4));//_t1_);
//			WriteAsicByteOSD(0,0x06,_pos_x+((_t1_+1)<<4));//79);
//		}
//		else{
//			WriteAsicByteOSD(0,0x05,_pos_x+(_t2_<<4));//_t1_);
//			WriteAsicByteOSD(0,0x06,_pos_x+((_t2_<<4)+(--_t3_)));//79);
//		}
//		WriteAsicByteOSD(0,0x0a,0xc0);										//... acceleration|x path
//		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
//	}
	if (_t2_ != 0) {
			WriteAsicByteOSD(_dvc,0x05,_pos_x);
			WriteAsicByteOSD(_dvc,0x06,_pos_x+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_t2_<<4));//_t1_);
			WriteAsicByteOSD(_dvc,0x06,_pos_x+((_t2_<<4)+_t3_-1));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xc0);										//... acceleration|x path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	

#if	defined(__EN_ISR__)
//	EA = 1;
#endif
}
//==================================================================================
void WriteOSDClrY(U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght)	//... only one _pth at once
{
//	U8 _t1_;
	U8 _t2_, _t3_;
	
#if	defined(__EN_ISR__)
//	EA = 0;
#endif
//	U8 _t1_;

	WriteAsicByteOSD(_dvc,0x00,0xff);					//... 1st pixel
	WriteAsicByteOSD(_dvc,0x01,0xff);					//... 2nd pixel
	WriteAsicByteOSD(_dvc,0x02,0xff);					//... 3rd pixel
	WriteAsicByteOSD(_dvc,0x03,0xff);					//... 4th pixel
	//... acceleration 을 사용할 경우 buffer write 과정 생략가능.
//	WriteAsicByteOSD(_dvc,0x04,0x80);					//... write to buffer

//... start position
//	_pos_x <<= 1;														//... (_pos_x)*2 -> 8 pixel * 2 -> 16 pixel char
	_pos_y = (_pos_y<<1)+(_pos_y<<3);							//... (_pos_y)*(2+8)

//	WriteAsicByteOSD(_dvc,0x05,_pos_x);
	WriteAsicByteOSD(_dvc,0x07,_pos_y&0xff);

//... end position
//	_pos_x += _wdth<<1;
	_hght = _pos_y+((_hght<<1)+(_hght<<3));

//	WriteAsicByteOSD(_dvc,0x06,--_pos_x);
	WriteAsicByteOSD(_dvc,0x08,(--_hght)&0xff);

//	WriteAsicByteOSD(_dvc,0x09,((_pos_y>>8)<<2)+(_hght>>8));		//... odd field
//	WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
//
//	while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for odd field
////Delay(1);

	WriteAsicByteOSD(_dvc,0x09,0x02|((_pos_y>>8)<<2)+(_hght>>8));	//... ~ even field
//	WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
//
//	while(ReadAsicOSDMemWr(_dvc)&BIT7);								//... wait finishing write for even field
////Delay(1);
	_pos_x <<= 1;														//... (_pos_x)*2 -> 8 pixel * 2 -> 16 pixel char
	_wdth <<= 1;
	
	_t2_ = _wdth>>4;
	_t3_ = _wdth&0x0f;
	
//	for(_t1_=0;_t1_<=_t2_;_t1_++){
//		if(_t1_<_t2_){
//			WriteAsicByteOSD(0,0x05,_pos_x+(_t1_<<4));//_t1_);
//			WriteAsicByteOSD(0,0x06,_pos_x+((_t1_+1)<<4));//79);
//		}
//		else{
//			WriteAsicByteOSD(0,0x05,_pos_x+(_t2_<<4));//_t1_);
//			WriteAsicByteOSD(0,0x06,_pos_x+((_t2_<<4)+(--_t3_)));//79);
//		}
//		WriteAsicByteOSD(0,0x0a,0xe0);										//... acceleration|y path
//		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
//	}
	if (_t2_ != 0) {
			WriteAsicByteOSD(_dvc,0x05,_pos_x);
			WriteAsicByteOSD(_dvc,0x06,_pos_x+(_t2_<<4));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}
	if (_t3_ != 0){
			WriteAsicByteOSD(_dvc,0x05,_pos_x+(_t2_<<4));//_t1_);
			WriteAsicByteOSD(_dvc,0x06,_pos_x+((_t2_<<4)+_t3_-1));//79);
		WriteAsicByteOSD(_dvc,0x0a,0xe0);										//... acceleration|y path
		while(ReadAsicOSDMemWr(_dvc)&BIT7){}								//... wait finishing write for even field
	}

#if	defined(__EN_ISR__)
//	EA = 1;
#endif
}
//==================================================================================
void WriteOSDClr(U8 _pth, U8 _dvc, U8 _pos_x, U16 _pos_y, U8 _wdth, U16 _hght)	//... 16x20 font 기준.
{
	if(BitSet(_pth,PTH_X))	WriteOSDClrX(_dvc,_pos_x,_pos_y,_wdth,_hght);
	if(BitSet(_pth,PTH_Y))	WriteOSDClrY(_dvc,_pos_x,_pos_y,_wdth,_hght);
}
//==================================================================================
/*
U8 DsplyOSDDatHexX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr)
{
	U8 _rdat_;
//	U8 _pg=0, _addr=0;
	U8 _t3_;
	
//	_pg = _addr>>16;
//	_addr = _addr&0xff;
	if(_pg == 0xff)	_rdat_ = _addr;
//	else if((_dvc == TW9901)||(_dvc == SAA7121))	_rdat_ = ReadI2CByte(_dvc,_addr);
//	else if(_dvc == TW9901)	_rdat_ = ReadI2CByte(_dvc,_addr);
	else	_rdat_ = ReadAsicByte(_dvc,_pg,_addr);

	for(_t3_=0;_t3_<2;_t3_++){
		_pg = _rdat_&(0x0f<<(_t3_<<2));
		_pg >>= _t3_<<2;
		if(_pg < 0x0a)	_addr = 0x10;//0x30;	
		else	_addr = 0x17;//0x37;

//		if(_size < 2)	WriteOSDFntX(MASTER,_pos_x,_pos_y,0x00,_pg+_addr);	// _size(1: bit[3-0]) -> __pos_x_
//		else	WriteAsic1Font(MASTER,_pos_x+1-_t3_,_pos_y,0x00,_pg+_addr);	// _size(2: bit[7-0]) -> [7-4]:__pos_x_,[3-0]:__pos_x_+1
		WriteOSDFntX(_dvc,_pos_x+1-_t3_,_pos_y,0x00,_pg+_addr);	// _size(2: bit[7-0]) -> [7-4]:__pos_x_,[3-0]:__pos_x_+1
	}
	
	return	_rdat_;
}
//	*/
//==================================================================================
U8 DsplyOSDDatHexY(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr)
{
	U8 _rdat_;
//	U8 _pg=0, _addr=0;
	U8 _t3_;
	
//	_pg = _addr>>16;
//	_addr = _addr&0xff;
	if(_pg == 0xff)	_rdat_ = _addr;
//	else if((_dvc == TW9901)||(_dvc == SAA7121))	_rdat_ = ReadI2CByte(_dvc,_addr);
//	else if(_dvc == TW9901)	_rdat_ = ReadI2CByte(_dvc,_addr);
	else	_rdat_ = ReadAsicByte(_dvc,_pg,_addr);

	for(_t3_=0;_t3_<2;_t3_++){
		_pg = _rdat_&(0x0f<<(_t3_<<2));
		_pg >>= _t3_<<2;
		if(_pg < 0x0a)	_addr = 0x10;//0x30;	
		else	_addr = 0x17;//0x37;

//		if(_size < 2)	WriteOSDFntX(MASTER,_pos_x,_pos_y,0x00,_pg+_addr);	// _size(1: bit[3-0]) -> __pos_x_
//		else	WriteAsic1Font(MASTER,_pos_x+1-_t3_,_pos_y,0x00,_pg+_addr);	// _size(2: bit[7-0]) -> [7-4]:__pos_x_,[3-0]:__pos_x_+1
		WriteOSDFntY(_dvc,_pos_x+1-_t3_,_pos_y,0x00,_pg+_addr);	// _size(2: bit[7-0]) -> [7-4]:__pos_x_,[3-0]:__pos_x_+1
	}
	
	return	_rdat_;
}
//==================================================================================
/*
U8 DsplyOSDDatBinX(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr)
{
	U8 _rdat_;
//	U8 _pg,_addr=0;
	
//	_pg = _addr>>16;
//	_addr = _addr&0xff;
	if(_pg == 0xff)	_rdat_ = _addr;
//	else if((_dvc == TW9901)||(_dvc == SAA7121))	_rdat_ = ReadI2CByte(_dvc,_addr);
//	else if(_dvc == TW9901)	_rdat_ = ReadI2CByte(_dvc,_addr);
	else	_rdat_ = ReadAsicByte(_dvc,_pg,_addr);

	for(_pg=0;_pg<8;_pg++){
		if(BitSet(_rdat_,BIT7>>_pg))	_addr = 0x11;
		else	_addr = 0x10;
		WriteOSDFntX(_dvc,_pos_x+_pg,_pos_y,0x00,_addr);
	}
	
	return	_rdat_;
}
//==================================================================================
U8 DsplyOSDDatBinY(U8 _dvc, U8 _pos_x, U8 _pos_y, U8 _pg, U8 _addr)
{
	U8 _rdat_;
//	U8 _pg,_addr=0;
	
//	_pg = _addr>>16;
//	_addr = _addr&0xff;
	if(_pg == 0xff)	_rdat_ = _addr;
//	else if((_dvc == TW9901)||(_dvc == SAA7121))	_rdat_ = ReadI2CByte(_dvc,_addr);
//	else if(_dvc == TW9901)	_rdat_ = ReadI2CByte(_dvc,_addr);
	else	_rdat_ = ReadAsicByte(_dvc,_pg,_addr);

	for(_pg=0;_pg<8;_pg++){
		if(BitSet(_rdat_,BIT7>>_pg))	_addr = 0x11;
		else	_addr = 0x10;
		WriteOSDFntY(_dvc,_pos_x+_pg,_pos_y,0x00,_addr);
	}
	
	return	_rdat_;
}
//	*/
//==================================================================================
void	SetOSDCol(U8 _pth, U8 _col_num, U8 _y, U8 _cb, U8 _cr)
{
	WriteAsicByte(cmn_dvc,DVC_PG2,0x0b,_y);						//... Y
	WriteAsicByte(cmn_dvc,DVC_PG2,0x0c,_cb);						//... Cb
	WriteAsicByte(cmn_dvc,DVC_PG2,0x0d,_cr);						//... Cr
	if(BitSet(_pth,PTH_X))
		WriteAsicByte(cmn_dvc,DVC_PG2,0x0e,BIT7|_col_num);		//... address
	else
		WriteAsicByte(cmn_dvc,DVC_PG2,0x0e,BIT7|BIT6|_col_num);		//... address
}

void	HwTW2835SetOSDColLUT(U8 _dvc, U8 _pth, U8  *_tbl_osd_clr)//U8 _col_num, U8 _y, U8 _cb, U8 _cr)
{
	U8 _t1_, _t2_, _t3_;
	
	_t1_ = _tbl_osd_clr[0];
    	//printf("\ncolor_num = %d\n", _t1_);


	if(BitSet(_pth,PTH_X)){	
		for(_t2_=0;_t2_<_t1_;_t2_++){
			WriteAsicByte(_dvc,DVC_PG2,0x0b,_tbl_osd_clr[1+(_t2_<<2)]);
			WriteAsicByte(_dvc,DVC_PG2,0x0c,_tbl_osd_clr[1+(_t2_<<2)+1]);
			WriteAsicByte(_dvc,DVC_PG2,0x0d,_tbl_osd_clr[1+(_t2_<<2)+2]);
			WriteAsicByte(_dvc,DVC_PG2,0x0e,_tbl_osd_clr[1+(_t2_<<2)+3]);
			_t3_ = _tbl_osd_clr[1+(_t2_<<2)+3];
			}
		_t3_++;
	    
		for(_t2_=_t1_;_t2_<64;_t2_++){
	        //printf("O");
			WriteAsicByte(_dvc,DVC_PG2,0x0b,0);
			WriteAsicByte(_dvc,DVC_PG2,0x0c,0);
			WriteAsicByte(_dvc,DVC_PG2,0x0d,0);
			WriteAsicByte(_dvc,DVC_PG2,0x0e,_t3_++); 
			}
		}

	if(BitSet(_pth,PTH_Y)){
		//printf("O");
		for(_t2_=0;_t2_<_t1_;_t2_++){
			WriteAsicByte(_dvc,DVC_PG2,0x0b,_tbl_osd_clr[1+(_t2_<<2)]);
			WriteAsicByte(_dvc,DVC_PG2,0x0c,_tbl_osd_clr[1+(_t2_<<2)+1]);
			WriteAsicByte(_dvc,DVC_PG2,0x0d,_tbl_osd_clr[1+(_t2_<<2)+2]);
			WriteAsicByte(_dvc,DVC_PG2,0x0e,_tbl_osd_clr[1+(_t2_<<2)+3]);
			}    
		}
}
//==================================================================================
void	InitOSDCol(void)
{
	SetOSDCol(PTH_Y,OSD_COL_BLK,0x00,0x80,0x80);
	SetOSDCol(PTH_Y,OSD_COL_50WHT,0x7f,0x80,0x80);
	SetOSDCol(PTH_Y,OSD_COL_75WHT,0xc0,0x80,0x80);

	SetOSDCol(PTH_X,OSD_COL_BLK,0x00,0x80,0x80);
	SetOSDCol(PTH_X,OSD_COL_50WHT,0x7f,0x80,0x80);
	SetOSDCol(PTH_X,OSD_COL_75WHT,0xc0,0x80,0x80);
	SetOSDCol(PTH_X,OSD_COL_25WHT,0x3f,0x80,0x80);
	SetOSDCol(PTH_X,OSD_COL_100WHT,0xf0,0x80,0x80);
	SetOSDCol(PTH_X,OSD_COL_BLU,0x1d,0xff,0x6b);
	SetOSDCol(PTH_X,OSD_COL_RED,0x4c,0x54,0xff);
	SetOSDCol(PTH_X,OSD_COL_ORNG,0x95,0x4f,0xb5);
	SetOSDCol(PTH_X,OSD_COL_CYN,0xb2,0xaa,0x00);
	SetOSDCol(PTH_X,OSD_COL_MGN,0x69,0xd4,0xea);
	SetOSDCol(PTH_X,OSD_COL_GRN,0x4b,0x55,0x4a);
	SetOSDCol(PTH_X,OSD_COL_OCN,0x61,0xb5,0x3a);
	SetOSDCol(PTH_X,OSD_COL_YLW,0xe1,0x00,0x94);
}
//==================================================================================
void	SetOSDMn(void)
{
////	WriteAsicByte(MASTER,DVC_PG2,0x38,0x0f);	//... OSD overlay : inverse OSD path with OSD overlay function
//	WriteAsicByte(MASTER,DVC_PG2,0x38,0x0c);	//... overlay Y path OSD on X path		//... 060307:temporal

//	if(b_mse_stts_pth == MSE_PTH_X)
//		WriteAsicByte(MASTER,DVC_PG2,0x38,0x09);	//... overlay Y path OSD on X path with high priority than X path
//	else
//		WriteAsicByte(MASTER,DVC_PG2,0x38,0x06);	//... overlay X path OSD on Y path with low priority than Y path
	WriteOSDClrX(MASTER,0,0,45,31);						//... max line num is 310 line for OSD
#if	defined(__4CH__)
	if(wndw_ds_sc_fmt == SC_FMT_8CH)
		WriteOSDClrY(MASTER,WNDW_DS_SC_CNT_X,WNDW_DS_SC_CNT_Y,11,1);
#endif

	WriteAsicByte(MASTER,DVC_PG2,0x38,0x09);	//... overlay Y path OSD on X path with high priority than X path
}
//==================================================================================
void	SetOSDNoMn(void)
{
       extern U8 cmn_dvc;
	if(wndw_ds_sc_fmt == SC_FMT_4CH){
//		WriteAsicByte(MASTER,DVC_PG2,0x38,0x00);	//... OSD overlay : default
		WriteAsicByte(cmn_dvc,DVC_PG2,0x38,0x08);	//... overlay Y path OSD on X path with high priority than X path
		GetAsicFlgType(cmn_dvc,DVC_PG1,0x7c,0x0f);	//... user must read 0x7c to release IRQ line.	***********************
	}
#if	defined(__4CH__)
	else if(wndw_ds_sc_fmt == SC_FMT_8CH){
		Run8ChOSDRto();
	}
#endif
}
//==================================================================================

