//==================================================================================
//						Key definition & variable description
//==================================================================================
#define	KEY_CNT_CHTR		16				//... default
//#define	KEY_CNT_CHTR		32					//... temporal



extern int	b_key0_past;
extern int	b_key0_rlse;
extern int	b_key0_pshd;
extern int	b_key0_pshng;
extern int	b_key0_dwn;
extern int	b_key0_clk;
extern int	b_key0_pshng_clk;
extern int	b_key0_tmp;

extern U8	key0_cnt;

//extern U8 	key1_flg;
//extern int	b_key1_past;
//extern int	b_key1_rlse;
//extern int	b_key1_pshd;
//extern int	b_key1_pshng;
//extern int	b_key1_dwn;
//extern int	b_key1_clk;
//extern int	b_key1_pshng_clk;
//extern int	b_key1_tmp;
//
//extern U8	key1_cnt;


//==================================================================================
//						Key function prototype description
//==================================================================================
extern void	SetKey0(void);
//extern void SetKey1(void);
