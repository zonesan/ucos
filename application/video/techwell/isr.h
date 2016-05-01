
//#define	__EN_ISR__

//==================================================================================
//				ISR(Interrupt service routine) function prototype description
//==================================================================================
extern void ExeIsrEx0(void);
extern void ExeIsrEx1(void);
extern void ExeIsrEx2(void);
extern void ExeIsrEx3(void);
extern void ExeIsrTmr0(void);
extern void ExeIsrTmr1(void);
extern void ExeIsrTmr2(void);

extern void InitIsr(void);
extern void IsrGpio(void);
//extern void	StrtIsr(void);
//extern void	EnIsr(U32 _isr);
//extern void	DisIsr(void);


