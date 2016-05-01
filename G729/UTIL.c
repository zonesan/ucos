/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*-------------------------------------------------------------------*
 * Function  Set zero()                                              *
 *           ~~~~~~~~~~                                              *
 * Set vector x[] to zero                                            *
 *-------------------------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"

void Set_zero(
  Word16 x[],      
  Word16 L         
)
{
   Word16 i;

   for (i = 0; i < L; i++)
     x[i] = 0;

   return;
}

/*-------------------------------------------------------------------*
 * Function  Copy:                                                   *
 *           ~~~~~                                                   *
 * Copy vector x[] to y[]                                            *
 *-------------------------------------------------------------------*/
/*
void Copy(
  Word16 x[],      
  Word16 y[],      
  Word16 L         
)
{
   Word16 i;

   for (i = 0; i < L; i++)
     y[i] = x[i];

   return;
}
*/
/* Random generator  

Word16 Random(Word16 *seed)
{

  *seed = extract_l(L_add(L_shr(L_mult(*seed, 31821), 1), 13849L));

  return(*seed);
}
*/


