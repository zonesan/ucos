
Word32 udi_Mpy_32(Word32 Ai, Word32 Bi);
void udi_Mpy_32_RR(Word32 Ai, Word32 Bi);

Word32 udi_Mpy_32_16(Word32 Ai, Word16 Bl);
void udi_Mpy_32_16_RR(Word32 Ai, Word16 Bl);

Word32 udi_Mac_32_16(Word32 Ai, Word16 Bl);
void udi_Mac_32_16_RR(Word32 Ai, Word16 Bl);

Word32 udi_Div_32(Word32 L_num, Word16 denom_hi, Word16 denom_lo);



Word16 udi_Chebps_11(Word16 x, Word16 f[], Word16 n);
Word16 udi_Chebps_10(Word16 x, Word16 f[], Word16 n);

//-------------------------------
// Start the #define 
//------------------------------

//void udi_L_Extract(Word32 L_32, Word16 *hi, Word16 *lo);
#define udi_L_Extract(L_32, hi, lo) \
        *hi  = (Word16) (L_32 >> 16); \
        *lo  = (L_32 & 0x0000ffff) >> 1

//Word32 udi_L_Comp(Word16 hi, Word16 lo);
#define udi_L_Comp(L_32, hi, lo) \
        L_32 = (Word32) (hi << 16); \
        L_32 = L_32 + (lo<<1)


/* reset mode register for lo*lo to acc0x, and enable 32bit acc saturation */
#define L32_SET_MODE_RESET \
        UDI_SET_REG((1<<23), UDI_REG_MODE_CTL)

#define L32_SAT_ACC \
        SAT(UDI_POST_SH0, UDI_NORND, UDI_SAT32, UDI_SAT_ACC00)

/* set mode register for hi*hi to acc0x */
#define L32_SET_MODE_HH \
        UDI_SET_REG((2<<24)|(2<<26), UDI_REG_MODE_CTL)

/* set mode register for lo*lo to acc0x */
#define L32_SET_MODE_LL \
        UDI_SET_REG((0<<24)|(0<<26), UDI_REG_MODE_CTL)

/* set mode register for lo*hi to acc0x */
#define L32_SET_MODE_HL \
        UDI_SET_REG((2<<26), UDI_REG_MODE_CTL)

/* set mode register for hi*lo to acc0x */
#define L32_SET_MODE_LH \
        UDI_SET_REG((2<<24), UDI_REG_MODE_CTL)

/* l_unsign*l_unsign to acc0x */
#define L32_mult_UU(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MPYMVRR, SRC0, SRC1, ((ACC<<11)|(0<<9)|(0<<6))>>6)

/* l_unsign*h_sign to acc1x */
#define L32_mult_US(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MPYMVRR, SRC0, SRC1, ((ACC<<11)|(1<<9)|(0<<6))>>6)

/* h_sign*l_unsign to acc0x */
#define L32_mult_SU(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MPYMVRR, SRC0, SRC1, ((ACC<<11)|(2<<9)|(0<<6))>>6)
         
/* h_sign*h_sign to acc0x */
#define L32_mult_SS(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MPYMVRR, SRC0, SRC1, ((ACC<<11)|(3<<9)|(3<<6))>>6)

/* h_sign*h_sign, added to acc0x*/
#define L32_Mac_SS(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MACMVRR, SRC0, SRC1, ((ACC<<11)|(3<<9)|(3<<6))>>6)

/* l_unsign*l_unsign, added to acc0x */
#define L32_Mac_UU(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MACMVRR, SRC0, SRC1, ((ACC<<11)|(0<<9)|(3<<6))>>6)

/* l_unsign*l_unsign, added to acc0x, NO left shift 1 bit */
#define L32_Mac_UU_NOSHF(SRC0, SRC1, ACC) \
        mips_udi_rri_nv(UDI_MACMVRR, SRC0, SRC1, ((ACC<<11)|(0<<9)|(0<<6))>>6)

/* L_shl(t0, 1) */
#define L_shl1_RR(L_32) \
        L32_mult_SS(L_32, 0x10000, UDI_ACC0); \
        L32_SET_MODE_HH; \
        SHIFT(UDI_POST_SHL16, UDI_NORND, UDI_SAT_ACC00); \
        L32_Mac_UU(L_32, 0x1, UDI_ACC0) ; \
        L32_SET_MODE_LL; \
	SAT(UDI_POST_SH0, UDI_NORND, UDI_SAT32, UDI_SAT_ACC00); \
	L32_SET_MODE_RESET

/* L_shl(t0, n) */
#define L_shl_RR(SRC0, N) \
        L32_mult_SU(SRC0, 1<<N, UDI_ACC0); \
        L32_SET_MODE_HL; \
        SHIFT(UDI_POST_SHL16, UDI_NORND, UDI_SAT_ACC00); \
        L32_Mac_UU_NOSHF(SRC0, 1<<N, UDI_ACC0); \
        L32_SET_MODE_LL; \
	SAT(UDI_POST_SH0, UDI_NORND, UDI_SAT32, UDI_SAT_ACC00); \
	L32_SET_MODE_RESET

/* L_shl(t0, n) */
#define L_shl_RRR(SRC0, N, DEST) \
        L32_mult_SU(SRC0, 1<<N, UDI_ACC0); \
        L32_SET_MODE_HL; \
        SHIFT(UDI_POST_SHL16, UDI_NORND, UDI_SAT_ACC00); \
        L32_Mac_UU_NOSHF(SRC0, 1<<N, UDI_ACC0); \
        L32_SET_MODE_LL; \
	SAT(UDI_POST_SH0, UDI_NORND, UDI_SAT32, UDI_SAT_ACC00); \
	L32_SET_MODE_RESET; \
        UDI_NOP(); \
        UDI_NOP(); \
        UDI_GET_REG_NP(DEST, UDI_REG_MAC0_ACC0_L)


#define L_shl_VAR(SRC0, VAR, DEST) \
        while (VAR > 15) {         \
        L_shl_RRR(SRC0, 15, SRC0); \
        VAR = VAR - 15;            \
        }                          \
        L_shl_RRR(SRC0, VAR, DEST)
