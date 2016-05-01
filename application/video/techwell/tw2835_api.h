
#include "cmn.h"
extern unsigned char  gVideoMotionEn;
extern unsigned char  gVideoLossEn;
extern unsigned char  gVideoBlindEn;
extern unsigned char  gVideoNightEn;


//#define    Hw2835_16Channel   /*16Channel to 4Channel*/

//#define    Hw2835_mouse   /*mouse interrupt set*/
//#define    Hw2835_reset   /*reset signal set*/


typedef struct{
    U16 start_pos_x;
    U16 start_pos_y;
    U16 width;
    U16 higth;
    U8 page;    
}TW2835Clr;

typedef struct{
    U16 start_pos_x;
    U16 start_pos_y;
    U16 end_pos_x;
    U16 end_pos_y;
    U32 attribute;    
}TW2835_Line;

typedef struct{
    U16 start_x;
    U16 start_y;
    U16 heigth;
    U16 width;
    U32 attribute;    
}TW2835_FillColor;

typedef struct{
    U16 start_x;
    U16 start_y;
    U8 * str_addr;
    U8 str_len;
    U32 attribute;    
}TW2835_WriteString;

typedef struct{
    U16 TDBox_HL;
    U16 TDBox_VT;
    U16 TDBox_VW;
    U16 TDBox_HW;
    U8 TDBox_NUM;
    U8 TDBox_PlaneCol;
    U16 TDBox_Attribute;  
}TW2835_2DBox;

typedef struct{
    U16 MD_start_x;
    U16 MD_start_y;
    U16 MD_width;
    U16 MD_heigth;
    U8 md_lvsens;
    U8 md_speedsens;
    U8 md_tmpsens;
    U8 md_spsens;
    U8 * md_AreaVectorAddr;
    U16 TDBox_Attribute;  
}TW2835_MotionDetector;

typedef struct{
    U16 subInstruction;
    U32 Instruction_attribute_1;
    U32 Instruction_attribute_2;
}TW2835_AudioSet;


#define DVR_AUDIO_GAIN	  0x00
#define DVR_AUDIO_LIVE	  0x01
#define DVR_AUDIO_RECORD	  0x03


extern U8 OUTMODEFLAG;
extern unsigned char md_set_ch;

void	HwTW2835FillColor(U8 _dvc, U8 _pth, TW2835_FillColor * tFillColorParameter);

void HwTW2835Init(U8 _pth, U8 VinModeControl, U8 VoutModeControl, U8 VinCH);
void HwTW2835Init_CZH(U8 _pth, U8 VinModeControl, U8 VoutModeControl, U8 VinCH);


void	HwTW2835WriteOSDBMP(U8 _dvc, U8 _pth, U16 _osd_pos_x, U16 _osd_pos_y, U16 width, U16 higth, U32 *BMPBuffer);
void	HwTW2835FillOSDBMP(U8 _dvc, U8 _pth, U16 _osd_pos_x, U16 _osd_pos_y, U8 *BMPBuffer, U32 *BMPIndex,  U8 attribute);


void HwTW2835WriteOSDStr(U8 _dvc, U8 _pth, TW2835_WriteString * t2835WriteString );

void	HwTW2835CreateLn(U8 _dvc, U8 _pth, TW2835_Line * tDrawLineParameter);

void	HwTw2835ChangeOutputDisplay(U8 Ch_ModeCon, U8 Ch_lv_Num);

void	HwTw2835SetOutputMode_Y(U8 Ch_ModeCon, U8 Ch_lv_Num);//by vincent 2007.10.20

void	HwTw2835SetOutputMode_PB(U8 Ch_ModeCon, U8 Ch_lv_Num);//by vincent 2007.10.20

void	HwTw2835SetOutputMode_X(U8 Ch_ModeCon, U8 Ch_lv_Num);//by vincent 2007.10.20

void	HwTW2835Clr(U8 _dvc, U8 _pth, TW2835Clr * ClrScreePara);

void	HwTW2835Playback(U8 _dvc, U8 _pth, U8 PB_Mode, U8 PB_OutChannel, U8 PB_InChannel);

void	HwTW2835Motiondetector(U8 _dvc, U8 _pth, U8 _ch, TW2835_MotionDetector * MotionDetectorParameter );


void	HwTW2835BitmapPageChange(U8 _dvc, U8 _pth, U8 page_attributr);

void	HwTW2835AdjustHue(U8 _dvc, U8 _pth, U8 _ch, U8 HueRange);

void	HwTW2835AdjustSaturation(U8 _dvc, U8 _pth, U8 _ch, U8 SaturationRange);

void	HwTW2835AdjustContrast(U8 _dvc, U8 _pth, U8 _ch, U8 ContrastRange);

void	HwTW2835AdjustBrightness(U8 _dvc, U8 _pth, U8 _ch, U8 BrightnessRange);

void Hw2835PrintRegister();
void Hw2835PrintRegisterMD();

void	HwTW2835_2DBoxDisplay(U8 _dvc, U8 _pth, U8 _ch, TW2835_2DBox * TwoDBoxDisplay );

void AppTW2835AudioInit();
void AppTW2835AudioSetVolume( U8 _dvc, U8 _ch, U8 direction, U8 volume);

void AppTW2835Audio_Gain(U8 _dvc, U8 _pth, U8 _ch, U16 attribute  );

void AppTW2835Audio_Live(U8 _dvc, U8 _pth, U8 _ch, unsigned int  attribute  );

void AppTW2835_Record(U8 _dvc, U8 _pth, U8 _ch,unsigned int attribute, unsigned int * pRecordSEQAddr );

int HwGetVB2100ChipID(void);

void HwEnableVideoLossDetect( unsigned short channel,unsigned short enable);

void HwEnableVideoBlindDetect(U8 _dvc, U8 channel,U8 enable,unsigned short BD_sense);

void HwEnableVideoNightDetect(U8 _dvc, U8 channel,U8 enable,unsigned short ND_sense);

void HwEnableDisplayChannel(int _ch_);
    
void HwDisableDisplayChannel(int _ch_);

void HwEnableRecordChannel(int _ch_);
    
void HwDisableRecordChannel(int _ch_);

void HwSetBGContrast(U8 Value);