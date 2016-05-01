/*
*********************************************************************************************************
*                                Copyright (c) 2006 VBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File:         encode.c             
*                                                                         
* Description :       
*                                                
* By:           Modified 2005 by Daniel Ding of Vbridge Microsystem, Inc.               
*
* Date:         Mar 9, 2006                                                           
**********************************************************************************************************
*/


#include "G726.h"
#include "g72x.h"

#define CHANNELNUM 2
#define TONENUM 3

int L_abs_g726(int L_var1)
  {
   int L_var_out;

   if (L_var1 == 0x80000000)
     {
      L_var_out = 0x7fffffff;
     }
   else
     {
      if (L_var1 < 0)
        {
         L_var_out = -L_var1;
        }
      else
        {
         L_var_out = L_var1;
        }
     }

   return(L_var_out);
  }

/*
 * g711.c
 *
 * u-law, A-law and linear PCM conversions.
 */
#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
#define	BIAS		(0x84)		/* Bias for linear code. */
static short seg_end[8] = {0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

/* copy from CCITT G.711 specifications */
unsigned char _u2a[128] = {			/* u- to A-law conversions */
	1,	1,	2,	2,	3,	3,	4,	4,
	5,	5,	6,	6,	7,	7,	8,	8,
	9,	10,	11,	12,	13,	14,	15,	16,
	17,	18,	19,	20,	21,	22,	23,	24,
	25,	27,	29,	31,	33,	34,	35,	36,
	37,	38,	39,	40,	41,	42,	43,	44,
	46,	48,	49,	50,	51,	52,	53,	54,
	55,	56,	57,	58,	59,	60,	61,	62,
	64,	65,	66,	67,	68,	69,	70,	71,
	72,	73,	74,	75,	76,	77,	78,	79,
	81,	82,	83,	84,	85,	86,	87,	88,
	89,	90,	91,	92,	93,	94,	95,	96,
	97,	98,	99,	100,	101,	102,	103,	104,
	105,	106,	107,	108,	109,	110,	111,	112,
	113,	114,	115,	116,	117,	118,	119,	120,
	121,	122,	123,	124,	125,	126,	127,	128};

unsigned char _a2u[128] = {			/* A- to u-law conversions */
	1,	3,	5,	7,	9,	11,	13,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31,
	32,	32,	33,	33,	34,	34,	35,	35,
	36,	37,	38,	39,	40,	41,	42,	43,
	44,	45,	46,	47,	48,	48,	49,	49,
	50,	51,	52,	53,	54,	55,	56,	57,
	58,	59,	60,	61,	62,	63,	64,	64,
	65,	66,	67,	68,	69,	70,	71,	72,
	73,	74,	75,	76,	77,	78,	79,	79,
	80,	81,	82,	83,	84,	85,	86,	87,
	88,	89,	90,	91,	92,	93,	94,	95,
	96,	97,	98,	99,	100,	101,	102,	103,
	104,	105,	106,	107,	108,	109,	110,	111,
	112,	113,	114,	115,	116,	117,	118,	119,
	120,	121,	122,	123,	124,	125,	126,	127};

static int search( int	val, short *table, int	size)
{
	int		i;

	for (i = 0; i < size; i++) {
		if (val <= *table++)
			return (i);
	}
	return (size);
}

/*
 * linear2alaw() - Convert a 16-bit linear PCM value to 8-bit A-law
 *
 * linear2alaw() accepts an 16-bit integer and encodes it as A-law data.

 */
unsigned char linear2alaw( int	pcm_val)	/* 2's complement (16-bit range) */
{
	int		mask;
	int		seg;
	unsigned char	aval;

	if (pcm_val >= 0) {
		mask = 0xD5;		/* sign (7th) bit = 1 */
	} else {
		mask = 0x55;		/* sign bit = 0 */
		pcm_val = -pcm_val - 8;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, 8);

	/* Combine the sign, segment, and quantization bits. */

	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		aval = seg << SEG_SHIFT;
		if (seg < 2)
			aval |= (pcm_val >> 4) & QUANT_MASK;
		else
			aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
		return (aval ^ mask);
	}
}

/*
 * alaw2linear() - Convert an A-law value to 16-bit linear PCM
 *
 */
int alaw2linear( unsigned char	a_val)
{
	int		t;
	int		seg;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
	switch (seg) {
	case 0:
		t += 8;
		break;
	case 1:
		t += 0x108;
		break;
	default:
		t += 0x108;
		t <<= seg - 1;
	}

	t = (a_val & SIGN_BIT) ? t : -t;
	return (t);
	
	/* return ((a_val & SIGN_BIT) ? t : -t); */
}


unsigned char linear2ulaw( int pcm_val)	/* 2's complement (16-bit range) */
{
	int		mask;
	int		seg;
	unsigned char	uval;

	/* Get the sign and the magnitude of the value. */
	if (pcm_val < 0) {
		pcm_val = BIAS - pcm_val;
		mask = 0x7F;
	} else {
		pcm_val += BIAS;
		mask = 0xFF;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, 8);

	/*
	 * Combine the sign, segment, quantization bits;
	 * and complement the code word.
	 */
	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
		return (uval ^ mask);
	}

}

/*
 * ulaw2linear() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
int ulaw2linear( unsigned char	u_val)
{
	int		t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

/* A-law to u-law conversion */
unsigned char alaw2ulaw( unsigned char	aval)
{
	aval &= 0xff;
	return ((aval & 0x80) ? (0xFF ^ _a2u[aval ^ 0xD5]) :
	    (0x7F ^ _a2u[aval ^ 0x55]));
}

/* u-law to A-law conversion */
unsigned char ulaw2alaw( unsigned char	uval)
{
	uval &= 0xff;
	return ((uval & 0x80) ? (0xD5 ^ (_u2a[0xFF ^ uval] - 1)) :
	    (0x55 ^ (_u2a[0x7F ^ uval] - 1)));
}

/*
 * g721.c
 *
 * Description:
 *
 * g721_encoder(), g721_decoder()
 *
 * These routines comprise an implementation of the CCITT G.721 ADPCM
 * coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of work station attributes, such as hardware 2's
 * complement arithmetic and large memory.  Specifically, certain time
 * consuming operations such as multiplications are replaced
 * with lookup tables and software 2's complement operations are
 * replaced with hardware 2's complement.
 *
 * The deviation from the bit level specification (lookup tables)
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.721 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */

static short qtab_721[7] = {-124, 80, 178, 246, 300, 349, 400};
/*
 * Maps G.721 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short _dqlntab_721[16] = {-2048, 4, 135, 213, 273, 323, 373, 425,
				425, 373, 323, 273, 213, 135, 4, -2048};

/* Maps G.721 code word to log of scale factor multiplier. */
static short _witab_721[16] = {-12, 18, 41, 64, 112, 198, 355, 1122,
				1122, 355, 198, 112, 64, 41, 18, -12};
/*
 * Maps G.721 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short _fitab_721[16] = {0, 0, 0, 0x200, 0x200, 0x200, 0x600, 0xE00,
				0xE00, 0x600, 0x200, 0x200, 0x200, 0, 0, 0};

/*
 * g721_encoder()
 *
 * Encodes the input vale of linear PCM, A-law or u-law data sl and returns
 * the resulting code. -1 is returned for unknown input coding value.
 */
int g721_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, se, sez;		/* ACCUM */
	short		d;			/* SUBTA */
	short		sr;			/* ADDB */
	short		y;			/* MIX */
	short		dqsez;			/* ADDC */
	short		dq, i, sepi;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;			/* 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sepi = predictor_pole(state_ptr);
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	se = (sezi + sepi) >> 1;	/* estimated signal */

	d = sl - se;				/* estimation difference */

	/* quantize the prediction difference */
	y = step_size(state_ptr);		/* quantizer step size */
	i = quantize(d, y, qtab_721, 7);	/* i = ADPCM code */

	dq = reconstruct(i & 8, _dqlntab_721[i], y);	/* quantized est diff */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq;	/* reconst. signal */

	dqsez = sr + sez - se;			/* pole prediction diff. */

	update(4, y, _witab_721[i] << 5, _fitab_721[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g721_decoder()
 *
 * Description:
 *
 * Decodes a 4-bit code of G.721 encoded data of i and
 * returns the resulting linear PCM, A-law or u-law value.
 * return -1 for unknown out_coding value.
 */
int g721_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

	i &= 0x0f;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* dynamic quantizer step size */

	dq = reconstruct(i & 0x08, _dqlntab_721[i], y); /* quantized diff. */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : se + dq;	/* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(4, y, _witab_721[i] << 5, _fitab_721[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was 14-bit dynamic range */
	default:
		return (-1);
	}
}


/*
 * g723_24.c
 *
 * Description:
 *
 * g723_24_encoder(), g723_24_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 24 Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which take advantage
 * of workstation attributes, such as hardware 2's complement arithmetic.
 *
 */

/*
 * Maps G.723_24 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab_723_24[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};

/* Maps G.723_24 code word to log of scale factor multiplier. */
static short	_witab_723_24[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};

/*
 * Maps G.723_24 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short	_fitab_723_24[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static short qtab_723_24[3] = {8, 218, 331};

/*
 * g723_24_encoder()
 *
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 * Returns -1 if invalid input coding value.
 */
int g723_24_encoder(int sl, int in_coding, struct g72x_state *state_ptr)
{
	short		sei, sezi, se, sez;	/* ACCUM */
	short		d;			/* SUBTA */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dqsez;			/* ADDC */
	short		dq, i;

#if 1
	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation diff. */

	/* quantize prediction difference d */
	y = step_size(state_ptr);	/* quantizer step size */
	i = quantize(d, y, qtab_723_24, 3);	/* i = ADPCM code */
	dq = reconstruct(i & 4, _dqlntab_723_24[i], y); /* quantized diff. */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* pole prediction diff. */

	update(3, y, _witab_723_24[i], _fitab_723_24[i], dq, sr, dqsez, state_ptr);

	return (i);
#endif

}

/*
 * g723_24_decoder()
 *
 * Decodes a 3-bit CCITT G.723_24 ADPCM code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_24_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

#if 1
	i &= 0x07;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x04, _dqlntab_723_24[i], y); /* unquantize pred diff */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(3, y, _witab_723_24[i], _fitab_723_24[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
#endif

}

/*
 * g723_40.c
 *
 * Description:
 *
 * g723_40_encoder(), g723_40_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 40Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of workstation attributes, such as hardware 2's
 * complement arithmetic.
 *
 * The deviation from the bit level specification (lookup tables),
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.723 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */


/*
 * Maps G.723_40 code word to ructeconstructed scale factor normalized log
 * magnitude values.
 */
static short _dqlntab_723_40[32] = {-2048, -66, 28, 104, 169, 224, 274, 318,
				358, 395, 429, 459, 488, 514, 539, 566,
				566, 539, 514, 488, 459, 429, 395, 358,
				318, 274, 224, 169, 104, 28, -66, -2048};

/* Maps G.723_40 code word to log of scale factor multiplier. */
static short _witab_723_40[32] = {448, 448, 768, 1248, 1280, 1312, 1856, 3200,
			4512, 5728, 7008, 8960, 11456, 14080, 16928, 22272,
			22272, 16928, 14080, 11456, 8960, 7008, 5728, 4512,
			3200, 1856, 1312, 1280, 1248, 768, 448, 448};

/*
 * Maps G.723_40 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short _fitab_723_40[32] = {0, 0, 0, 0, 0, 0x200, 0x200, 0x200,
			0x200, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0xC00,
			0xC00, 0xC00, 0xA00, 0x800, 0x600, 0x400, 0x200, 0x200,
			0x200, 0x200, 0x200, 0, 0, 0, 0, 0};

static short qtab_723_40[15] = {-122, -16, 68, 139, 198, 250, 298, 339,
				378, 413, 445, 475, 502, 528, 553};

/*
 * g723_40_encoder()
 *
 * Encodes a 16-bit linear PCM, A-law or u-law input sample and retuens
 * the resulting 5-bit CCITT G.723 40Kbps code.
 * Returns -1 if the input coding value is invalid.
 */
int g723_40_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	short		sei, sezi, se, sez;	/* ACCUM */
	short		d;			/* SUBTA */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dqsez;			/* ADDC */
	short		dq, i;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear((unsigned char)sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation difference */

	/* quantize prediction difference */
	y = step_size(state_ptr);	/* adaptive quantizer step size */
	i = quantize(d, y, qtab_723_40, 15);	/* i = ADPCM code */

	dq = reconstruct(i & 0x10, _dqlntab_723_40[i], y);	/* quantized diff */

	sr = (dq < 0) ? se - (dq & 0x7FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* dqsez = pole prediction diff. */

	update(5, y, _witab_723_40[i], _fitab_723_24[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g723_40_decoder()
 *
 * Decodes a 5-bit CCITT G.723 40Kbps code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int g723_40_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y;  /* dif */			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

	i &= 0x1f;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x10, _dqlntab_723_40[i], y);	/* estimation diff. */

	sr = (dq < 0) ? (se - (dq & 0x7FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;		/* pole prediction diff. */

	update(5, y, _witab_723_40[i], _fitab_723_40[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 0x10, qtab_723_40));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 0x10, qtab_723_40));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
}



/*
 * g726_16.c
 *
 * Description:
 *
 * g723_16_encoder(), g723_16_decoder()
 *
 * These routines comprise an implementation of the CCITT G.726 16 Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which take advantage
 * of workstation attributes, such as hardware 2's complement arithmetic.
 *
 * The ITU-T G.726 coder is an adaptive differential pulse code modulation
 * (ADPCM) waveform coding algorithm, suitable for coding of digitized
 * telephone bandwidth (0.3-3.4 kHz) speech or audio signals sampled at 8 kHz.
 * This coder operates on a sample-by-sample basis. Input samples may be 
 * represented in linear PCM or companded 8-bit G.711 (m-law/A-law) formats
 * (i.e., 64 kbps). For 32 kbps operation, each sample is converted into a
 * 4-bit quantized difference signal resulting in a compression ratio of 
 * 2:1 over the G.711 format. For 24 kbps 40 kbps operation, the quantized
 * difference signal is 3 bits and 5 bits, respectively.
 *
 */

static short	_dqlntab[4] = { 116, 365, 365, 116}; 
static short	_witab[4] = {-704, 14048, 14048, -704};
static short	_fitab[4] = {0, 0xE00, 0xE00, 0};
static int qtab_723_16[1] = {261};

/*
 * g723_16_encoder()
 *
 * Encodes a linear PCM, A-law or u-law input sample and returns its 2-bit code.
 * Returns -1 if invalid input coding value.
 */
int g726_16_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	int		sezi;
	int		sez;			/* ACCUM */
	int		sei;
	int		se;
	int		d;				/* SUBTA */
	int		y;				/* MIX */
	int		i;
	int		dq;
	int		sr;				/* ADDB */
	int		dqsez;			/* ADDC */

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation diff. */

	/* quantize prediction difference d */
	y = step_size(state_ptr);	/* quantizer step size */
	i = quantize(d, y, qtab_723_16, 1);  /* i = ADPCM code */

	      /* Since quantize() only produces a three level output
	       * (1, 2, or 3), we must create the fourth one on our own
	       */
	if (i == 3)                          /* i code for the zero region */
	  if ((d & 0x8000) == 0)             /* If d > 0, i=3 isn't right... */
	    i = 0;
	    
	dq = reconstruct(i & 2, _dqlntab[i], y); /* quantized diff. */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* pole prediction diff. */

	update(2, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g723_16_decoder()
 *
 * Decodes a 2-bit CCITT G.723_16 ADPCM code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int g726_16_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	int		sezi;
	int		sez;			/* ACCUM */
	int		sei;
	int		se;
	int		y;				/* MIX */
	int		dq;
	int		sr;				/* ADDB */
	int		dqsez;

	i &= 0x03;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x02, _dqlntab[i], y); /* unquantize pred diff */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(2, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 2, qtab_723_16));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 2, qtab_723_16));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
}



static short power2[15] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
		0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};

// Daniel 11/15/2005, global can only be accessed in this file
static struct	g72x_state state[CHANNELNUM][TONENUM];
static int		in_coding[CHANNELNUM][TONENUM];
static int		(*enc_routine[CHANNELNUM][TONENUM])();
static int		enc_bits[CHANNELNUM][TONENUM];
static int	out_bits_40k[CHANNELNUM][TONENUM];

// prototypes
static int quan(int	val, short *table,int size);

/*
 * Pack output codes into bytes and write them to stdout.
 * Returns 1 if there is residual output, else returns 0.
 */
// int pack_output(unsigned code, int bits)
int pack_output(LONG lwChannelNum, LONG lwTalkNum, unsigned char code_byte, int bits, unsigned char *pOutBuf, int *outOneByte)
{
	static unsigned int	out_buffer = 0;
	static int	out_bits = 0;
	static int	out_bits_24k = 16;
	//static int	out_bits_40k = 16;
	unsigned char		out_byte;
	unsigned int code = (unsigned int)code_byte; 
	// wg added
	
		out_bits_40k[lwChannelNum][lwTalkNum] -= bits;
		out_buffer |= (code << out_bits_40k[lwChannelNum][lwTalkNum]);
		
		if (out_bits_40k[lwChannelNum][lwTalkNum] <= 8) {
			out_byte = ( out_buffer >> 8 ) & 0xff;
			out_bits_40k[lwChannelNum][lwTalkNum] += 8;
			out_buffer <<= 8;
			*pOutBuf = out_byte;
			*outOneByte = 1;
		}
		else
			*outOneByte = 0;
			
	return (out_bits > 0);
}


static int fmult(int an, int srn)
{
	short		anmag, anexp, anmant;
	short		wanexp, wanmant;  // wanmag, 
	short		retval;

	anmag = (an > 0) ? an : ((-an) & 0x1FFF);
	anexp = quan(anmag, power2, 15) - 6;
	anmant = (anmag == 0) ? 32 :
	    (anexp >= 0) ? anmag >> anexp : anmag << -anexp;
	wanexp = anexp + ((srn >> 6) & 0xF) - 13;
    	wanmant = (anmant * (srn & 077) + 0x30) >> 4;
	retval = (wanexp >= 0) ? ((wanmant << wanexp) & 0x7FFF) :
	    (wanmant >> -wanexp);
    
	return(((an^srn) < 0) ? -retval : retval);

}

static int quan(int	val, short *table,int size)
{
	int		i;

	for (i = 0; i < size; i++)
		if (val < *table++)
			break;
	return (i);
}


unsigned char swapG726(unsigned char g726OutputData)
{
	unsigned char i, j, g726ReturnData;
	i = g726OutputData;
	i <<= 4;
	i = i & 0x0f0;
	j = g726OutputData;
	j >>= 4;
	j = j & 0x0f;
	g726ReturnData = i | j;
	return g726ReturnData;
}


void g72x_init_state(struct g72x_state *state_ptr)
{
	int		cnta;

	state_ptr->yl = 34816;
	state_ptr->yu = 544;
	state_ptr->dms = 0;
	state_ptr->dml = 0;
	state_ptr->ap = 0;
	for (cnta = 0; cnta < 2; cnta++) {
		state_ptr->a[cnta] = 0;
		state_ptr->pk[cnta] = 0;
		state_ptr->sr[cnta] = 32;
	}
	for (cnta = 0; cnta < 6; cnta++) {
		state_ptr->b[cnta] = 0;
		state_ptr->dq[cnta] = 32;
	}
	state_ptr->td = 0;
}

/*
 * predictor_zero()
 *
 * computes the estimated signal from 6-zero predictor.
 *
 */
int predictor_zero(struct g72x_state *state_ptr)
{
	int		i;
	int		sezi;

	sezi = fmult(state_ptr->b[0] >> 2, state_ptr->dq[0]);
	for (i = 1; i < 6; i++)			/* ACCUM */
		sezi += fmult(state_ptr->b[i] >> 2, state_ptr->dq[i]);
	return (sezi);
}
/*
 * predictor_pole()
 *
 * computes the estimated signal from 2-pole predictor.
 *
 */
int predictor_pole(struct g72x_state *state_ptr)
{
	return (fmult(state_ptr->a[1] >> 2, state_ptr->sr[1]) +
	    fmult(state_ptr->a[0] >> 2, state_ptr->sr[0]));
}
/*
 * step_size()
 *
 * computes the quantization step size of the adaptive quantizer.
 *
 */
int step_size(struct g72x_state *state_ptr)
{
	int		y;
	int		dif;
	int		al;

	if (state_ptr->ap >= 256)
		return (state_ptr->yu);
	else {
		y = state_ptr->yl >> 6;
		dif = state_ptr->yu - y;
		al = state_ptr->ap >> 2;
		if (dif > 0)
			y += (dif * al) >> 6;
		else if (dif < 0)
			y += (dif * al + 0x3F) >> 6;
		return (y);
	}
}

/*
 * quantize()
 *
 * Given a raw sample, 'd', of the difference signal and a
 * quantization step size scale factor, 'y', this routine returns the
 * ADPCM codeword to which that sample gets quantized.  The step
 * size scale factor division operation is done in the log base 2 domain
 * as a subtraction.
 */
int quantize(
	int		d,	/* Raw difference signal sample */
	int		y,	/* Step size multiplier */
	short		*table,	/* quantization table */
	int		size)	/* table size of short integers */
{
	short		dqm;	/* Magnitude of 'd' */
	short		exp;	/* Integer part of base 2 log of 'd' */
	short		mant;	/* Fractional part of base 2 log */
	short		dl;	/* Log of magnitude of 'd' */
	short		dln;	/* Step size scale factor normalized log */
	int		i;

	/*
	 * LOG
	 *
	 * Compute base 2 log of 'd', and store in 'dl'.
	 */
	dqm = L_abs_g726(d);
	//dqm = abs(d);
	exp = quan(dqm >> 1, power2, 15);
	mant = ((dqm << 7) >> exp) & 0x7F;	/* Fractional portion. */
	dl = (exp << 7) + mant;

	/*
	 * SUBTB
	 *
	 * "Divide" by step size multiplier.
	 */
	dln = dl - (y >> 2);

	/*
	 * QUAN
	 *
	 * Obtain codword i for 'd'.
	 */
	i = quan(dln, table, size);
	if (d < 0)			/* take 1's complement of i */
		return ((size << 1) + 1 - i);
	else if (i == 0)		/* take 1's complement of 0 */
		return ((size << 1) + 1); /* new in 1988 */
	else
		return (i);
}
/*
 * reconstruct()
 *
 * Returns reconstructed difference signal 'dq' obtained from
 * codeword 'i' and quantization step size scale factor 'y'.
 * Multiplication is performed in log base 2 domain as addition.
 */
int reconstruct(
	int		sign,	/* 0 for non-negative value */
	int		dqln,	/* G.72x codeword */
	int		y)	/* Step size multiplier */
{
	short		dql;	/* Log of 'dq' magnitude */
	short		dex;	/* Integer part of log */
	short		dqt;
	short		dq;	/* Reconstructed difference signal sample */

	dql = dqln + (y >> 2);	/* ADDA */

	if (dql < 0) {
		return ((sign) ? -0x8000 : 0);
	} else {		/* ANTILOG */
		dex = (dql >> 7) & 15;
		dqt = 128 + (dql & 127);
		dq = (dqt << 7) >> (14 - dex);
		return ((sign) ? (dq - 0x8000) : dq);
	}
}


/*
 * update()
 *
 * updates the state variables for each output code
 */
void update(
	int		code_size,	/* distinguish 723_40 with others */
	int		y,		/* quantizer step size */
	int		wi,		/* scale factor multiplier */
	int		fi,		/* for long/short term energies */
	int		dq,		/* quantized prediction difference */
	int		sr,		/* reconstructed signal */
	int		dqsez,		/* difference from 2-pole predictor */
	struct g72x_state *state_ptr)	/* coder state pointer */
{
	int		cnt;
	short		mag, exp;	 // , mant  // Adaptive predictor, FLOAT A   
	short		a2p;		/* LIMC */
	short		a1ul;		/* UPA1 */
	short		pks1;	/* UPA2 */
	short		fa1;
	// short	ua2, uga2a, uga2b;
	char		tr;		/* tone/transition detector */
	short		ylint, thr2, dqthr;
	short  		ylfrac, thr1;
	short		pk0;

	pk0 = (dqsez < 0) ? 1 : 0;	/* needed in updating predictor poles */

	mag = dq & 0x7FFF;		/* prediction difference magnitude */
	/* TRANS: Transition detector */
	ylint = (short)(state_ptr->yl >> 15);	/* exponent part of yl */
	ylfrac = (state_ptr->yl >> 10) & 0x1F;	/* fractional part of yl */
	thr1 = (32 + ylfrac) << ylint;		/* threshold */
	thr2 = (ylint > 9) ? 31 << 10 : thr1;	/* limit thr2 to 31 << 10 */
	dqthr = (thr2 + (thr2 >> 1)) >> 1;	/* dqthr = 0.75 * thr2 */
	if (state_ptr->td == 0)		/* signal supposed voice */
		tr = 0;
	else if (mag <= dqthr)		/* supposed data, but small mag */
		tr = 0;			/* treated as voice */
	else				/* signal is data (modem) */
		tr = 1;

	/*
	 * Quantizer scale factor adaptation.
	 */

	/* FUNCTW & FILTD & DELAY */
	/* update non-steady state step size multiplier */
	state_ptr->yu = y + ((wi - y) >> 5);

	/* LIMB */
	if (state_ptr->yu < 544)	/* 544 <= yu <= 5120 */
		state_ptr->yu = 544;
	else if (state_ptr->yu > 5120)
		state_ptr->yu = 5120;

	/* FILTE & DELAY */
	/* update steady state step size multiplier */
	state_ptr->yl += state_ptr->yu + ((-state_ptr->yl) >> 6);

	/*
	 * Adaptive predictor coefficients.
	 */
	if (tr == 1) {			/* reset a's and b's for modem signal */
		state_ptr->a[0] = 0;
		state_ptr->a[1] = 0;
		state_ptr->b[0] = 0;
		state_ptr->b[1] = 0;
		state_ptr->b[2] = 0;
		state_ptr->b[3] = 0;
		state_ptr->b[4] = 0;
		state_ptr->b[5] = 0;
	} else {			/* update a's and b's */
		pks1 = pk0 ^ state_ptr->pk[0];		/* UPA2 */

		/* update predictor pole a[1] */
		a2p = state_ptr->a[1] - (state_ptr->a[1] >> 7);
		if (dqsez != 0) {
			fa1 = (pks1) ? state_ptr->a[0] : -state_ptr->a[0];
			if (fa1 < -8191)	/* a2p = function of fa1 */
				a2p -= 0x100;
			else if (fa1 > 8191)
				a2p += 0xFF;
			else
				a2p += fa1 >> 5;

			if (pk0 ^ state_ptr->pk[1])
				/* LIMC */
				if (a2p <= -12160)
					a2p = -12288;
				else if (a2p >= 12416)
					a2p = 12288;
				else
					a2p -= 0x80;
			else if (a2p <= -12416)
				a2p = -12288;
			else if (a2p >= 12160)
				a2p = 12288;
			else
				a2p += 0x80;
		}

		/* TRIGB & DELAY */
		state_ptr->a[1] = a2p;

		/* UPA1 */
		/* update predictor pole a[0] */
		state_ptr->a[0] -= state_ptr->a[0] >> 8;
		if (dqsez != 0)
			if (pks1 == 0)
				state_ptr->a[0] += 192;
			else
				state_ptr->a[0] -= 192;

		/* LIMD */
		a1ul = 15360 - a2p;
		if (state_ptr->a[0] < -a1ul)
			state_ptr->a[0] = -a1ul;
		else if (state_ptr->a[0] > a1ul)
			state_ptr->a[0] = a1ul;

		/* UPB : update predictor zeros b[6] */
		for (cnt = 0; cnt < 6; cnt++) {
			if (code_size == 5)		/* for 40Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 9;
			else			/* for G.721 and 24Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 8;
			if (dq & 0x7FFF) {			/* XOR */
				if ((dq ^ state_ptr->dq[cnt]) >= 0)
					state_ptr->b[cnt] += 128;
				else
					state_ptr->b[cnt] -= 128;
			}
		}
	}

	for (cnt = 5; cnt > 0; cnt--)
		state_ptr->dq[cnt] = state_ptr->dq[cnt-1];
	/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
	if (mag == 0) {
		state_ptr->dq[0] = (dq >= 0) ? 0x20 : 0xFC20;
	} else {
		exp = quan(mag, power2, 15);
		state_ptr->dq[0] = (dq >= 0) ?
		    (exp << 6) + ((mag << 6) >> exp) :
		    (exp << 6) + ((mag << 6) >> exp) - 0x400;
	}

	state_ptr->sr[1] = state_ptr->sr[0];
	/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
	if (sr == 0) {
		state_ptr->sr[0] = 0x20;
	} else if (sr > 0) {
		exp = quan(sr, power2, 15);
		state_ptr->sr[0] = (exp << 6) + ((sr << 6) >> exp);
	} else if (sr > -32768) {
		mag = -sr;
		exp = quan(mag, power2, 15);
		state_ptr->sr[0] =  (exp << 6) + ((mag << 6) >> exp) - 0x400;
	} else
		state_ptr->sr[0] = (short)0xFC20;

	/* DELAY A */
	state_ptr->pk[1] = state_ptr->pk[0];
	state_ptr->pk[0] = pk0;

	/* TONE */
	if (tr == 1)		/* this sample has been treated as data */
		state_ptr->td = 0;	/* next one will be treated as voice */
	else if (a2p < -11776)	/* small sample-to-sample correlation */
		state_ptr->td = 1;	/* signal may be data */
	else				/* signal is voice */
		state_ptr->td = 0;

	/*
	 * Adaptation speed control.
	 */
	state_ptr->dms += ( fi - state_ptr->dms ) >> 5;		/* FILTA */
	state_ptr->dml += ((( fi << 2 ) - state_ptr->dml ) >> 7 );	/* FILTB */

	if (tr == 1)
		state_ptr->ap = 256;
	else if (y < 1536)					/* SUBTC */
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if (state_ptr->td == 1)
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if ( L_abs_g726( (state_ptr->dms << 2) - state_ptr->dml ) >= (state_ptr->dml >> 3) )
	//else if ( abs( (state_ptr->dms << 2) - state_ptr->dml ) >= (state_ptr->dml >> 3) )
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else
		state_ptr->ap += (-state_ptr->ap) >> 4;
}

/*
 * tandem_adjust(sr, se, y, i, sign)
 *
 * At the end of ADPCM decoding, it simulates an encoder which may be receiving
 * the output of this decoder as a tandem process. If the output of the
 * simulated encoder differs from the input to this decoder, the decoder output
 * is adjusted by one level of A-law or u-law codes.
 *
 * Input:
 *	sr	decoder output linear PCM sample,
 *	se	predictor estimate sample,
 *	y	quantizer step size,
 *	i	decoder input code,
 *	sign	sign bit of code i
 *
 * Return:
 *	adjusted A-law or u-law compressed sample.
 */
int tandem_adjust_alaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* A-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted A-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = -1;
	sp = linear2alaw((sr >> 1) << 3);	/* short to A-law compression */
	dx = (alaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);

	if (id == i) {			/* no adjustment on sp */
		return (sp);
	} else {			/* sp adjustment needed */
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;

		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80) {
				sd = (sp == 0xD5) ? 0x55 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
			} else {
				sd = (sp == 0x2A) ? 0x2A :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			}
		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0xAA) ? 0xAA :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			else
				sd = (sp == 0x55) ? 0xD5 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
		}
		return (sd);
	}
}

int tandem_adjust_ulaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* u-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted u-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = 0;
	sp = linear2ulaw(sr << 2);	/* short to u-law compression */
	dx = (ulaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);
	if (id == i) {
		return (sp);
	} else {
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;
		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80)
				sd = (sp == 0xFF) ? 0x7E : sp + 1;
			else
				sd = (sp == 0) ? 0 : sp - 1;

		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0x80) ? 0x80 : sp - 1;
			else
				sd = (sp == 0x7F) ? 0xFE : sp + 1;
		}
		return (sd);
	}
}

// Daniel Added 11/15/2005


VOID AppAudioG726EncodeInit(LONG lwChannelNum, LONG lwTalkNum, LONG lwAudioG726EnType, LONG lwAudioG729InputType)
{
	switch (lwAudioG726EnType) {
	case e_G726_16KT:	//1
		enc_routine[lwChannelNum][lwTalkNum] = g726_16_encoder;
		enc_bits[lwChannelNum][lwTalkNum] = 2;
		break;	
		
	case e_G726_24K:	//1
		enc_routine[lwChannelNum][lwTalkNum] = g723_24_encoder;
		enc_bits[lwChannelNum][lwTalkNum] = 3;
		break;
	case e_G726_32K:	//2
		enc_routine[lwChannelNum][lwTalkNum] = g721_encoder;
		enc_bits[lwChannelNum][lwTalkNum] = 4;
		break;
	case e_G726_40K:	//3
		enc_routine[lwChannelNum][lwTalkNum] = g723_40_encoder;
		enc_bits[lwChannelNum][lwTalkNum] = 5;
		break;
	default:
		enc_routine[lwChannelNum][lwTalkNum] = g721_encoder;
		enc_bits[lwChannelNum][lwTalkNum] = 4;
		break;
	}

	in_coding[lwChannelNum][lwTalkNum] = lwAudioG729InputType;
	
	out_bits_40k[lwChannelNum][lwTalkNum] = 16;

	g72x_init_state(&state[lwChannelNum][lwTalkNum]);
	
}


LONG AppAudioG726EncodeFunction(
	LONG	lwChannelNum,
	LONG 	lwTalkNum, 
	PBYTE	pcwG726EncodeInputData,
	BYTE	cwG726InputDataType,
	PBYTE 	pcwG726EncodeOutData,
	LONG 	lwAudioG726AudioFrameLen
)
{
	LONG  sl;
	BYTE  code;
	LONG  kk;
	PBYTE pInput = pcwG726EncodeInputData;
	SHORT	pInput1[400];
	PBYTE pOutput = pcwG726EncodeOutData;
	LONG totalCodeBytes = 0, outOneByte;
	
	
	unsigned char OutputData;

	for (kk=0; kk<lwAudioG726AudioFrameLen; kk++) 
	{
		/* 16bit linear sample  */
		if (in_coding[lwChannelNum][lwTalkNum] == AUDIO_ENCODING_LINEAR) {
			sl = *(PSHORT)pInput;
			pInput += 2;

		}
		/*  8bit alaw/ulaw sample  */
		else {
			sl = *pInput++;
		}

		code = (BYTE)(*enc_routine[lwChannelNum][lwTalkNum])(sl, in_coding[lwChannelNum][lwTalkNum], &state[lwChannelNum][lwTalkNum]);

		// data cotrol code
		pack_output(lwChannelNum, lwTalkNum, code, enc_bits[lwChannelNum][lwTalkNum], pOutput, &outOneByte);
		if (outOneByte) {
			
			pOutput++;
			totalCodeBytes++;
		}
	}
	
	return totalCodeBytes;
}



LONG AppAudioG726GetInputFormat( LONG lwChannelNum, LONG lwTalkNum)
{
	return in_coding[lwChannelNum][lwTalkNum];
}

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/
static struct g72x_state dec_state[CHANNELNUM][TONENUM];
static LONG out_coding[CHANNELNUM][TONENUM];
static LONG out_size[CHANNELNUM][TONENUM];
static LONG (*dec_routine[CHANNELNUM][TONENUM])();
static LONG  dec_bits[CHANNELNUM][TONENUM];
/*wg add*/
static ULONG  	in_buffer[CHANNELNUM][TONENUM];
static LONG	in_bits[CHANNELNUM][TONENUM];

/*___________________________________________________________________________
 |                                                                           |
 |   Functions                                                               |
 |___________________________________________________________________________|
*/
VOID AppAudioG726DecodeInit(LONG lwChannelNum, LONG lwTalkNum, LONG lwAudioG726Type, LONG lwAudioG726OutputType)
{
	SHORT	LenTest;
	
	switch (lwAudioG726Type) {
	case e_G726_16KT:
		dec_routine[lwChannelNum][lwTalkNum] = g726_16_decoder;
		dec_bits[lwChannelNum][lwTalkNum] = 2;
		break;
	case e_G726_24K:
		dec_routine[lwChannelNum][lwTalkNum] = g723_24_decoder;
		dec_bits[lwChannelNum][lwTalkNum] = 3;
		break;
	case e_G726_32K:
		dec_routine[lwChannelNum][lwTalkNum] = g721_decoder;
		dec_bits[lwChannelNum][lwTalkNum] = 4;
		break;
	case e_G726_40K:
		dec_routine[lwChannelNum][lwTalkNum] = g723_40_decoder;
		dec_bits[lwChannelNum][lwTalkNum] = 5;
		break;
	default:
		dec_routine[lwChannelNum][lwTalkNum] = g721_decoder;
		dec_bits[lwChannelNum][lwTalkNum] = 4;
		break;
	}

	switch (lwAudioG726OutputType) {
	case AUDIO_ENCODING_ULAW:
		out_coding[lwChannelNum][lwTalkNum] = AUDIO_ENCODING_ULAW;
		out_size[lwChannelNum][lwTalkNum] = sizeof (CHAR);
		break;
	case AUDIO_ENCODING_ALAW:
		out_coding[lwChannelNum][lwTalkNum] = AUDIO_ENCODING_ALAW;
		out_size[lwChannelNum][lwTalkNum] = sizeof (CHAR);
		LenTest = sizeof (int);
		LenTest = sizeof (short);
		LenTest = sizeof (long);
		LenTest = sizeof (long long);
		break;
	case AUDIO_ENCODING_LINEAR:
		out_coding[lwChannelNum][lwTalkNum] = AUDIO_ENCODING_LINEAR;
		out_size[lwChannelNum][lwTalkNum] = sizeof (SHORT);
		break;
	default:
		out_coding[lwChannelNum][lwTalkNum] = AUDIO_ENCODING_ULAW;
		out_size[lwChannelNum][lwTalkNum] = sizeof (CHAR);
		break;
	}
	
	in_buffer[lwChannelNum][lwTalkNum] = 0;
	in_bits[lwChannelNum][lwTalkNum] = 0;

	g72x_init_state(&dec_state[lwChannelNum][lwTalkNum]);
}


LONG AppAudioG726DecodeFunction(
	LONG	 lwChannelNum,
	LONG	 lwTalkNum,
	PBYTE	pcwG726DecodeInputData,
	BYTE 	cwG726InputDataType,
	PBYTE	pcwG726DecodeOutData,
	LONG 	lwAudioG726AudioFrameLen
)
{
	SHORT 	sample, sample1;
	BYTE 	code;
	//static ULONG  	in_buffer = 0;
	//static LONG	in_bits = 0;
	BYTE	in_byte;
	
	BYTE  	*pInput = pcwG726DecodeInputData;
	BYTE  	*pOutput = pcwG726DecodeOutData;
	LONG  	totalCodeBytes = 0;

	while (pInput < (pcwG726DecodeInputData + lwAudioG726AudioFrameLen)) {
		
			if (in_bits[lwChannelNum][lwTalkNum] < dec_bits[lwChannelNum][lwTalkNum]) {
				
				in_byte = *pInput++;				
				in_buffer[lwChannelNum][lwTalkNum] |= (ULONG)in_byte << (8 - in_bits[lwChannelNum][lwTalkNum]);
				in_bits[lwChannelNum][lwTalkNum] += 8;
			}
			code = (in_buffer[lwChannelNum][lwTalkNum] >> (16-dec_bits[lwChannelNum][lwTalkNum])) & ((1 << dec_bits[lwChannelNum][lwTalkNum]) - 1);
			in_buffer[lwChannelNum][lwTalkNum] <<= dec_bits[lwChannelNum][lwTalkNum];
			in_bits[lwChannelNum][lwTalkNum] -= dec_bits[lwChannelNum][lwTalkNum];

		sample = (*dec_routine[lwChannelNum][lwTalkNum])(code, out_coding[lwChannelNum][lwTalkNum], &dec_state[lwChannelNum][lwTalkNum]);
		if (out_size[lwChannelNum][lwTalkNum] == 2) {

			//*(SHORT *)pOutput = sample;
			*pOutput++ = (BYTE)sample;
			sample1 = sample >> 8;
			*pOutput++ = (BYTE)sample1;
			//pOutput += 2;
			totalCodeBytes += 2;
		} else {
			code = (BYTE)sample;

			*pOutput++ = code;
			totalCodeBytes++;
		}
	}

	return totalCodeBytes;
}

LONG G726GetOutputFormat(LONG lwChannelNum, LONG lwTalkNum)
{
	return out_coding[lwChannelNum][lwTalkNum];
}

LONG AppAudioG726Encode(
	LONG lwChannelNum,
	LONG lwTalkNum,
	PBYTE	pcwG726EncodeInputData,
	BYTE	cwG726InputDataType,
	PBYTE	pcwG726EncodeOutData,
	LONG	lwAudioG726AudioFrameLen			
)
{
	LONG 	lwG726EncodedFrameLen;
		
	lwG726EncodedFrameLen = AppAudioG726EncodeFunction(lwChannelNum, lwTalkNum, pcwG726EncodeInputData, cwG726InputDataType, pcwG726EncodeOutData, lwAudioG726AudioFrameLen);		
		
	return lwG726EncodedFrameLen;
	
}

LONG AppAudioG726Decode(
	LONG	 lwChannelNum,
	LONG	 lwTalkNum,
	PBYTE 	pcwG726DecodeInputData,
	BYTE	cwG726InputDataType,
	PBYTE 	pcwG726DecodeOutData,
	LONG 	lwAudioG726AudioFrameLen
)
{	
	LONG	lwG726DecodedFrameLen;
	
	lwG726DecodedFrameLen = AppAudioG726DecodeFunction(lwChannelNum, lwTalkNum, pcwG726DecodeInputData, cwG726InputDataType, pcwG726DecodeOutData, lwAudioG726AudioFrameLen);
	
	return lwG726DecodedFrameLen;
}


