/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*****************************************************************************/
/* bit stream manipulation routines                                          */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "ld8a.h"
#include "tab_ld8a.h"
#include "vad.h"
#include "dtx.h"
#include "tab_dtx.h"
#include "octet.h"

/* prototypes for local functions */
static void  int2bin(Word16 value, Word16 no_of_bits, Word16 *bitstream);
static Word16   bin2int(Word16 no_of_bits, Word16 *bitstream);

/*----------------------------------------------------------------------------
 * prm2bits_ld8k -converts encoder parameter vector into vector of serial bits
 * bits2prm_ld8k - converts serial received bits to  encoder parameter vector
 *
 * The transmitted parameters are:
 *
 *     LPC:     1st codebook           7+1 bit
 *              2nd codebook           5+5 bit
 *
 *     1st subframe:
 *          pitch period                 8 bit
 *          parity check on 1st period   1 bit
 *          codebook index1 (positions) 13 bit
 *          codebook index2 (signs)      4 bit
 *          pitch and codebook gains   4+3 bit
 *
 *     2nd subframe:
 *          pitch period (relative)      5 bit
 *          codebook index1 (positions) 13 bit
 *          codebook index2 (signs)      4 bit
 *          pitch and codebook gains   4+3 bit
 *----------------------------------------------------------------------------
 */
void prm2bits_ld8k(
 Word16   prm[],           /* input : encoded parameters  (PRM_SIZE parameters)  */
  Word16 bits[]           /* output: serial bits (SERIAL_SIZE ) bits[0] = bfi
                                    bits[1] = 80 */
)
{
  Word16 i;
  *bits++ = SYNC_WORD;     /* bit[0], at receiver this bits indicates BFI */

  switch(prm[0]){

    /* not transmitted */
  case 0 : {
    *bits = RATE_0;
    break;
  }

  case 1 : {
    *bits++ = RATE_8000;
    for (i = 0; i < PRM_SIZE; i++) {
      int2bin(prm[i+1], bitsno[i], bits);
      bits += bitsno[i];
    }
    break;
  }

  case 2 : {

#ifndef OCTET_TX_MODE
    *bits++ = RATE_SID;
    for (i = 0; i < 4; i++) {
      int2bin(prm[i+1], bitsno2[i], bits);
      bits += bitsno2[i];
    }
#else
    *bits++ = RATE_SID_OCTET;
    for (i = 0; i < 4; i++) {
      int2bin(prm[i+1], bitsno2[i], bits);
      bits += bitsno2[i];
    }
    *bits++ = BIT_0;
#endif

    break;
  }

  default : {
    printf("Unrecognized frame type\n");
    //exit(-1);
  }

  }

  return;
}

/*----------------------------------------------------------------------------
 * int2bin convert integer to binary and write the bits bitstream array
 *----------------------------------------------------------------------------
 */
static void int2bin(
 Word16 value,             /* input : decimal value */
 Word16 no_of_bits,        /* input : number of bits to use */
 Word16 *bitstream       /* output: bitstream  */
)
{
   Word16 *pt_bitstream;
   Word16   i, bit;

   pt_bitstream = bitstream + no_of_bits;

   for (i = 0; i < no_of_bits; i++)
   {
     bit = value & (Word16)0x0001;      /* get lsb */
     if (bit == 0)
         *--pt_bitstream = BIT_0;
     else
         *--pt_bitstream = BIT_1;
     value >>= 1;
   }
}

/*----------------------------------------------------------------------------
 *  bits2prm_ld8k - converts serial received bits to  encoder parameter vector
 *----------------------------------------------------------------------------
 */
void bits2prm_ld8k(
 Word16 bits[],          /* input : serial bits (80)                       */
 Word16   prm[]          /* output: decoded parameters (11 parameters)     */
)
{
  Word16 i;
  Word16 nb_bits;

  nb_bits = *bits++;        /* Number of bits in this frame       */

  if(nb_bits == RATE_8000) {
    prm[1] = 1;
    for (i = 0; i < PRM_SIZE; i++) {
      prm[i+2] = bin2int(bitsno[i], bits);
      bits  += bitsno[i];
    }
  }
  else
#ifndef OCTET_TX_MODE
    if(nb_bits == RATE_SID) {
      prm[1] = 2;
      for (i = 0; i < 4; i++) {
        prm[i+2] = bin2int(bitsno2[i], bits);
        bits += bitsno2[i];
      }
    }
#else
  /* the last bit of the SID bit stream under octet mode will be discarded */
  if(nb_bits == RATE_SID_OCTET) {
    prm[1] = 2;
    for (i = 0; i < 4; i++) {
      prm[i+2] = bin2int(bitsno2[i], bits);
      bits += bitsno2[i];
    }
  }
#endif

  else {
    prm[1] = 0;
  }
  return;

}

/*----------------------------------------------------------------------------
 * bin2int - read specified bits from bit array  and convert to integer value
 *----------------------------------------------------------------------------
 */
static Word16 bin2int(            /* output: decimal value of bit pattern */
 Word16 no_of_bits,        /* input : number of bits to read */
 Word16 *bitstream       /* input : array containing bits */
)
{
   Word16   value, i;
   Word16 bit;

   value = 0;
   for (i = 0; i < no_of_bits; i++)
   {
     value <<= 1;
     bit = *bitstream++;
     if (bit == BIT_1)  value += 1;
   }
   return(value);
}


Word16 read_frame(unsigned char *pCodeBuf, Word16 *parm)
{
  Word16  i;
  Word16  serial[SERIAL_SIZE];          /* Serial stream               */

/*
  if(fread(serial, sizeof(short), 2, f_serial) != 2) {
    return(0);
  }
  if(fread(&serial[2], sizeof(Word16), (size_t)serial[1], f_serial)
     != (size_t)serial[1]) {
    return(0);
  }
*/

  memcpy(serial, pCodeBuf, sizeof(short)*2);
  memcpy(&serial[2], pCodeBuf+sizeof(short)*2, sizeof(Word16)*(size_t)serial[1]);

  bits2prm_ld8k(&serial[1], parm);

  /* This part was modified for version V1.3 */
  /* for speech and SID frames, the hardware detects frame erasures
     by checking if all bits are set to zero */
  /* for untransmitted frames, the hardware detects frame erasures
     by testing serial[0] */

  parm[0] = 0;           /* No frame erasure */
  if(serial[1] != 0) {
   for (i=0; i < serial[1]; i++)
     if (serial[i+2] == 0 ) parm[0] = 1;  /* frame erased     */
  }
  else if(serial[0] != SYNC_WORD) parm[0] = 1;

  if(parm[1] == 1) {
    /* check parity and put 1 in parm[5] if parity error */
    parm[5] = Check_Parity_Pitch(parm[4], parm[5]);
  }

  return(1);
}


int putbits (int n, int val, unsigned char *pByte, int *pCount, unsigned char *pBuf)
{
  int i;
  unsigned int mask;
  int bytecnt = 0;
  unsigned char *pbits = pBuf;
  unsigned char outbyte = *pByte; 
  int outcnt = *pCount;

  mask = 1 << (n-1); /* selects first (leftmost) bit */

  for (i=0; i<n; i++) {
    outbyte <<= 1;

    if (val & mask)
      outbyte |= 1;

    mask >>= 1; /* select next bit */
    outcnt--;

    if (outcnt==0) /* 8 bit buffer full */ {
      // new added
	  *pbits++ = outbyte;
      outcnt = 8;
      bytecnt++;
    }
  }

  // save values
  *pByte = outbyte;
  *pCount = outcnt;
  return bytecnt;
}


int vb_prm2bits_ld8k(
 Word16   prm[],           /* input : encoded parameters  (PRM_SIZE parameters)  */
 unsigned char *pCodeStream
)
{
	int i;   
	int outcnt = 8;
	int bytecnt = 0;
	unsigned char outbfr = 0;

	switch(prm[0]) {

    // not transmitted 
	case 0 : 
		// *bits = RATE_0;
		break;

	// G729A frame
	case 1 : 
		for (i = 0; i < PRM_SIZE; i++) {
			// int2bin(prm[i+1], bitsno[i], bits);
			// bits += bitsno[i];
			bytecnt += putbits (bitsno[i], prm[i+1], &outbfr, &outcnt, pCodeStream+bytecnt);
		}
		break;

	// SID frame
	case 2 : 
		for (i = 0; i < 4; i++) {
			bytecnt += putbits (bitsno2[i], prm[i+1], &outbfr, &outcnt, pCodeStream+bytecnt);
		}
		// add 0 for last bit
		bytecnt += putbits (1, 0, &outbfr, &outcnt, pCodeStream+bytecnt);
		break;
	}

	return bytecnt;
}


// to mask the n least significant bits of an integer 
static unsigned int msk[33] =
{
  0x00000000,0x00000001,0x00000003,0x00000007,
  0x0000000f,0x0000001f,0x0000003f,0x0000007f,
  0x000000ff,0x000001ff,0x000003ff,0x000007ff,
  0x00000fff,0x00001fff,0x00003fff,0x00007fff,
  0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,
  0x000fffff,0x001fffff,0x003fffff,0x007fffff,
  0x00ffffff,0x01ffffff,0x03ffffff,0x07ffffff,
  0x0fffffff,0x1fffffff,0x3fffffff,0x7fffffff,
  0xffffffff
};


int vb_bits2prm_ld8k(
 unsigned char *pCodeStream,	/* input: serial bits (80) */
 Word16   prm[],				/* output: decoded parameters (11 parameters) */
 int total_bits                 // total bits in the encoded frame
)
{
	int i;
//	Word16 nb_bits;
	// nb_bits = *bits++;        // Number of bits in this frame 
	int incnt = 0;
	unsigned char *pStream = pCodeStream;
	unsigned int dwValue = 0;

	if(total_bits == RATE_8000) {
		prm[1] = 1;  // default RATE_8000
		for (i = 0; i < PRM_SIZE; i++) {
			int n; // c,  

			n = bitsno[i];

			if (incnt < n) {
				dwValue = (dwValue<<16) | (*pStream<<8) | *(pStream+1);
				pStream += 2;
				incnt += 16;
			}
			prm[i+2] = (dwValue>>(incnt-n)) & msk[n];

			dwValue &= msk[incnt-n];
			incnt -= n;
		}
	}
	else if (total_bits == RATE_SID_OCTET) {
		prm[1] = 2;
		for (i = 0; i < 4; i++) {
		
			int n; // c,  

			n = bitsno2[i];

			
			if (incnt < n) {
				dwValue = (dwValue<<16) | (*pStream<<8) | *(pStream+1);
				pStream += 2;
				incnt += 16;
			}
			prm[i+2] = (dwValue>>(incnt-n)) & msk[n];

			// advance by n bits
			dwValue &= msk[incnt-n];
			incnt -= n;
		}
	}
	else {
		prm[1] = 0;
	}

	if(prm[1] == 1) {
		 
		prm[5] = Check_Parity_Pitch(prm[4], prm[5]);
	}

	return (pStream - pCodeStream);
}

