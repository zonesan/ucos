/*
   ITU-T G.729A Speech Coder with Annex B    ANSI-C Source Code
   Version 1.3    Last modified: August 1997
   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies,
   Rockwell International
   All rights reserved.
*/

/*-----------------------------------------------------*
 * Function Autocorr()                                 *
 *                                                     *
 *   Compute autocorrelations of signal with windowing *
 *                                                     *
 *-----------------------------------------------------*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

#include "ld8a.h"
#include "tab_ld8a.h"


/*
void Autocorr(
  Word16 x[],      
  Word16 m,        
  Word16 r_h[],    
  Word16 r_l[],    
  Word16 *exp_R0
)
{
  Word16 i, j, norm;
  Word16 y[L_WINDOW];
  Word32 sum;
  Word32 tempwg;

  extern Flag Overflow;


  for(i=0; i<L_WINDOW; i+=8)
  {
    y[i] = mult_r(x[i], hamwindow[i]);
    y[i+1] = mult_r(x[i+1], hamwindow[i+1]);
    y[i+2] = mult_r(x[i+2], hamwindow[i+2]);
    y[i+3] = mult_r(x[i+3], hamwindow[i+3]);
    y[i+4] = mult_r(x[i+4], hamwindow[i+4]);
    y[i+5] = mult_r(x[i+5], hamwindow[i+5]);
    y[i+6] = mult_r(x[i+6], hamwindow[i+6]);
    y[i+7] = mult_r(x[i+7], hamwindow[i+7]);
  }



  *exp_R0 = 1;

  do {
    Overflow = 0;
    sum = 1;                   
    for(i=0; i<L_WINDOW; i+=8)
    {
      sum = L_mac(sum, y[i], y[i]);
      sum = L_mac(sum, y[i+1], y[i+1]);
      sum = L_mac(sum, y[i+2], y[i+2]);
      sum = L_mac(sum, y[i+3], y[i+3]);
      sum = L_mac(sum, y[i+4], y[i+4]);
      sum = L_mac(sum, y[i+5], y[i+5]);
      sum = L_mac(sum, y[i+6], y[i+6]);
      sum = L_mac(sum, y[i+7], y[i+7]);
    }



    if(Overflow != 0)
    {
      for(i=0; i<L_WINDOW; i++)
      {
        //y[i] = shr(y[i], 2);
        if (y[i] < 0)
        {
        	y[i] = ~(( ~y[i]) >> 2 );
        }
        else
        {
           	y[i] = y[i] >> 2;
        }
      }
      *exp_R0 = add((*exp_R0), 4);
      Overflow = 1;
    }
  }while (Overflow != 0);


  norm = norm_l(sum);
  sum  = L_shl(sum, norm);
  //L_Extract(sum, &r_h[0], &r_l[0]);    
  r_h[0] = extract_h(sum);
  r_l[0] = extract_l(L_msu(L_shr(sum,1), r_h[0],16384));
  *exp_R0 = sub(*exp_R0, norm);


  for (i = 1; i <= m; i++)
  {
    sum = 0;
    for(j=0; j<L_WINDOW-i; j++)
      sum = L_mac(sum, y[j], y[j+i]);

    sum = L_shl(sum, norm);
    //L_Extract(sum, &r_h[i], &r_l[i]);
    r_h[i] = extract_h(sum);
    //tempwg = L_shr(sum,1);
    if (sum<0)
    {
    	tempwg = ~((~sum) >> 1);
    }    
    else
    {
        tempwg = sum >> 1;
    }
    
    
    tempwg = L_msu(tempwg, r_h[i],16384);
    r_l[i] = extract_l(tempwg);
  }
  return;
}

*/
/*-------------------------------------------------------*
 * Function Lag_window()                                 *
 *                                                       *
 * Lag_window on autocorrelations.                       *
 *                                                       *
 * r[i] *= lag_wind[i]                                   *
 *                                                       *
 *  r[i] and lag_wind[i] are in special double precision.*
 *  See "oper_32b.c" for the format                      *
 *                                                       *
 *-------------------------------------------------------*/
/*
void Lag_window(
  Word16 m,         
  Word16 r_h[],     
  Word16 r_l[]      
)
{
  Word16 i;
  Word32 x;

  for(i=1; i<=m; i++)
  {
     x  = Mpy_32(r_h[i], r_l[i], lag_h[i-1], lag_l[i-1]);
     L_Extract(x, &r_h[i], &r_l[i]);
  }
  return;
}
*/

/*___________________________________________________________________________
 |                                                                           |
 |      LEVINSON-DURBIN algorithm in double precision                        |
 |      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        |
 |---------------------------------------------------------------------------|
 |                                                                           |
 | Algorithm                                                                 |
 |                                                                           |
 |       R[i]    autocorrelations.                                           |
 |       A[i]    filter coefficients.                                        |
 |       K       reflection coefficients.                                    |
 |       Alpha   prediction gain.                                            |
 |                                                                           |
 |       Initialization:                                                     |
 |               A[0] = 1                                                    |
 |               K    = -R[1]/R[0]                                           |
 |               A[1] = K                                                    |
 |               Alpha = R[0] * (1-K**2]                                     |
 |                                                                           |
 |       Do for  i = 2 to M                                                  |
 |                                                                           |
 |            S =  SUM ( R[j]*A[i-j] ,j=1,i-1 ) +  R[i]                      |
 |                                                                           |
 |            K = -S / Alpha                                                 |
 |                                                                           |
 |            An[j] = A[j] + K*A[i-j]   for j=1 to i-1                       |
 |                                      where   An[i] = new A[i]             |
 |            An[i]=K                                                        |
 |                                                                           |
 |            Alpha=Alpha * (1-K**2)                                         |
 |                                                                           |
 |       END                                                                 |
 |                                                                           |
 | Remarks on the dynamics of the calculations.                              |
 |                                                                           |
 |       The numbers used are in double precision in the following format :  |
 |       A = AH <<16 + AL<<1.  AH and AL are 16 bit signed integers.         |
 |       Since the LSB's also contain a sign bit, this format does not       |
 |       correspond to standard 32 bit integers.  We use this format since   |
 |       it allows fast execution of multiplications and divisions.          |
 |                                                                           |
 |       "DPF" will refer to this special format in the following text.      |
 |       See oper_32b.c                                                      |
 |                                                                           |
 |       The R[i] were normalized in routine AUTO (hence, R[i] < 1.0).       |
 |       The K[i] and Alpha are theoretically < 1.0.                         |
 |       The A[i], for a sampling frequency of 8 kHz, are in practice        |
 |       always inferior to 16.0.                                            |
 |                                                                           |
 |       These characteristics allow straigthforward fixed-point             |
 |       implementation.  We choose to represent the parameters as           |
 |       follows :                                                           |
 |                                                                           |
 |               R[i]    Q31   +- .99..                                      |
 |               K[i]    Q31   +- .99..                                      |
 |               Alpha   Normalized -> mantissa in Q31 plus exponent         |
 |               A[i]    Q27   +- 15.999..                                   |
 |                                                                           |
 |       The additions are performed in 32 bit.  For the summation used      |
 |       to calculate the K[i], we multiply numbers in Q31 by numbers        |
 |       in Q27, with the result of the multiplications in Q27,              |
 |       resulting in a dynamic of +- 16.  This is sufficient to avoid       |
 |       overflow, since the final result of the summation is                |
 |       necessarily < 1.0 as both the K[i] and Alpha are                    |
 |       theoretically < 1.0.                                                |
 |___________________________________________________________________________|
*/


/* Last A(z) for case of unstable filter */
/*
static Word16 old_A[M+1]={4096,0,0,0,0,0,0,0,0,0,0};
static Word16 old_rc[2]={0,0};

void Levinson(
  Word16 Rh[],     
  Word16 Rl[],    
  Word16 A[],      
  Word16 rc[],     
  Word16 *Err     
)
{
 Word16 i, j, k;
 Word16 hi, lo;
 Word16 Kh, Kl;                
 Word16 alp_h, alp_l, alp_exp; 
 Word16 Ah[M+1], Al[M+1];      
 Word16 Anh[M+1], Anl[M+1];    
 Word32 t0, t1, t2, t3, t4;            


  t4 = (Word32) Rh[1] << 16; 
  t1=  L_mac(t4, Rl[1], 1);       
  
  if (t1 == MIN_32)
  {
     t2 = MAX_32;
  }
  else
  {
    if (t1 < 0)
    {
       t2 = -t1;
    }
    else
    {
       t2 = t1;
    }
  }
  
  t0  = Div_32(t2, Rh[0], Rl[0]);       
  if(t1 > 0) t0= L_negate(t0);          
  L_Extract(t0, &Kh, &Kl);             
  rc[0] = Kh;
  t0 = t0 >> 4;
  L_Extract(t0, &Ah[1], &Al[1]);       

  t0 = Mpy_32(Kh ,Kl, Kh, Kl);         
  t0 = L_abs(t0);                       
  t0 = L_sub( (Word32)0x7fffffffL, t0 ); 
  L_Extract(t0, &hi, &lo);              
  t0 = Mpy_32(Rh[0] ,Rl[0], hi, lo);    


  alp_exp = norm_l(t0);
  t0 = L_shl(t0, alp_exp);
  L_Extract(t0, &alp_h, &alp_l);         


  for(i= 2; i<=M; i++)
  {


    t0 = 0;
    for(j=1; j<i; j++)
      t0 = L_add(t0, Mpy_32(Rh[j], Rl[j], Ah[i-j], Al[i-j]));

    
    for(k = 4;k>0;k--)
    {
         if (t0 > (Word32) 0X3fffffffL)
           {
            Overflow = 1;
            t0 = MAX_32;
            break;
           }
         else
           {
            if (t0 < (Word32) 0xc0000000L)
              {
               Overflow = 1;
               t0 = MIN_32;
               break;
              }
           }
         t0 <<= 1;
    }
        	    
    t1 = L_Comp(Rh[i],Rl[i]);
    t0 = L_add(t0, t1);               


    t1 = L_abs(t0);
    t2 = Div_32(t1, alp_h, alp_l);   
    if(t0 > 0) t2= L_negate(t2);    
    t2 = L_shl(t2, alp_exp);         
    L_Extract(t2, &Kh, &Kl);         
    rc[i-1] = Kh;


    if (sub(abs_s(Kh), 32750) > 0)
    {
      for(j=0; j<=M; j++)
      {
        A[j] = old_A[j];
      }
      rc[0] = old_rc[0];        
      rc[1] = old_rc[1];
      return;
    }

    for(j=1; j<i; j++)
    {
      t0 = Mpy_32(Kh, Kl, Ah[i-j], Al[i-j]);
      t0 = L_add(t0, L_Comp(Ah[j], Al[j]));
      L_Extract(t0, &Anh[j], &Anl[j]);
    }
    //t2 = L_shr(t2, 4);                 
    t2 = t2 >> 4;
    L_Extract(t2, &Anh[i], &Anl[i]);    


    t0 = Mpy_32(Kh ,Kl, Kh, Kl);          
    t0 = L_abs(t0);                     
    t0 = L_sub( (Word32)0x7fffffffL, t0 ); 
    L_Extract(t0, &hi, &lo);            
    t0 = Mpy_32(alp_h , alp_l, hi, lo);   

    j = norm_l(t0);
    t0 = L_shl(t0, j);
    L_Extract(t0, &alp_h, &alp_l);        
    alp_exp = add(alp_exp, j);             


    for(j=1; j<=i; j++)
    {
      Ah[j] =Anh[j];
      Al[j] =Anl[j];
    }
  }

  *Err = shr(alp_h, alp_exp);


  A[0] = 4096;
  for(i=1; i<=M; i++)
  {
    t0   = L_Comp(Ah[i], Al[i]);
    
    //old_A[i] = A[i] = ROUND(L_shl(t0, 1));
    //t3 = L_shl(t0, 1);
    if (t0 > (Word32) 0X3fffffffL)
    {
    	Overflow = 1;
    	t3 = MAX_32;
    	//break;
    }
    else if (t0 < (Word32) 0xc0000000L)
    {
       	Overflow = 1;
       	t3 = MIN_32;
       	//break;
      
    }
    else
    {
    	t3 = t0 << 1;
    }
    
    old_A[i] = A[i] = ROUND(t3);
  }
  old_rc[0] = rc[0];
  old_rc[1] = rc[1];

  return;
}


*/

/*-------------------------------------------------------------*
 *  procedure Az_lsp:                                          *
 *            ~~~~~~                                           *
 *   Compute the LSPs from  the LPC coefficients  (order=10)   *
 *-------------------------------------------------------------*/

/* local function */
/*
static Word16 Chebps_11(Word16 x, Word16 f[], Word16 n);
static Word16 Chebps_10(Word16 x, Word16 f[], Word16 n);

void Az_lsp(
  Word16 a[],       
  Word16 lsp[],     
  Word16 old_lsp[]  
)
{
 Word16 i, j, nf, ip;
 Word16 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
 Word16 xlowtemp, xhightemp;
 Word16 x, y, sign, exp;
 Word16 *coef;
 Word16 f1[M/2+1], f2[M/2+1];
 Word32 t0, L_temp;
 Flag   ovf_coef;
 Word16 (*pChebps)(Word16 x, Word16 f[], Word16 n);
 
 ovf_coef = 0;
 pChebps = Chebps_11;

 f1[0] = 2048;        
 f2[0] = 2048;       

 for (i = 0; i< NC; i++)
 {
   Overflow = 0;
   t0 = L_mult(a[i+1], 16384);        
   t0 = L_mac(t0, a[M-i], 16384);       
   x  = extract_h(t0);

   f1[i+1] = sub(x, f1[i]);    

   t0 = L_mult(a[i+1], 16384);       
   t0 = L_msu(t0, a[M-i], 16384);     
   x  = extract_h(t0);

   f2[i+1] = add(x, f2[i]); 
   if ( Overflow ) {
     ovf_coef = 1;      }
 }

 if ( ovf_coef ) {

   pChebps = Chebps_10;

   f1[0] = 1024;        
   f2[0] = 1024;       

   for (i = 0; i< NC; i++)
   {
     t0 = L_mult(a[i+1], 8192);       
     t0 = L_mac(t0, a[M-i], 8192);     
     x  = extract_h(t0);
     f1[i+1] = sub(x, f1[i]);          

     t0 = L_mult(a[i+1], 8192);       
     t0 = L_msu(t0, a[M-i], 8192);    
     x  = extract_h(t0);
     f2[i+1] = add(x, f2[i]);         
   }
 }
 
 nf=0;      
 ip=0;        

 coef = f1;

 xlow = grid[0];
 ylow = (*pChebps)(xlow, coef, NC);

 j = 0;
 while ( (nf < M) && (j < GRID_POINTS) )
 {
   j =add(j,1);
   xhigh = xlow;
   yhigh = ylow;
   xlow  = grid[j];
   ylow  = (*pChebps)(xlow,coef,NC);

   L_temp = L_mult(ylow ,yhigh);
   if ( L_temp <= (Word32)0)
   {


     for (i = 0; i < 2; i++)
     {
     	xlowtemp = xlow >> 1;
	xhightemp = xhigh>> 1;
	xmid = add( xlowtemp , xhightemp);

       ymid = (*pChebps)(xmid,coef,NC);

       L_temp = L_mult(ylow,ymid);
       if ( L_temp <= (Word32)0)
       {
         yhigh = ymid;
         xhigh = xmid;
       }
       else
       {
         ylow = ymid;
         xlow = xmid;
       }
     }


     x   = sub(xhigh, xlow);
     y   = sub(yhigh, ylow);

     if(y == 0)
     {
       xint = xlow;
     }
     else
     {
       sign= y;
       y   = abs_s(y);
       exp = norm_s(y);
       y   = shl(y, exp);
       y   = div_s( (Word16)16383, y);
       t0  = L_mult(x, y);
       t0  = L_shr(t0, sub(20, exp) );
       y   = extract_l(t0);    

       if(sign < 0) y = negate(y);

       t0   = L_mult(ylow, y);   
       //t0   = L_shr(t0, 11);                    
       t0 = t0 >> 11;
       xint = sub(xlow, extract_l(t0));         
     }

     lsp[nf] = xint;
     xlow    = xint;
     nf =add(nf,1);

     if(ip == 0)
     {
       ip = 1;
       coef = f2;
     }
     else
     {
       ip = 0;
       coef = f1;
     }
     ylow = (*pChebps)(xlow,coef,NC);

   }
 }


 if( sub(nf, M) < 0)
 {
    for(i=0; i<M; i++)
    {
      lsp[i] = old_lsp[i];
    }

 }

 return;
}

/*--------------------------------------------------------------*
 * function  Chebps_11, Chebps_10:                              *
 *           ~~~~~~~~~~~~~~~~~~~~                               *
 *    Evaluates the Chebichev polynomial series                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  The polynomial order is                                     *
 *     n = M/2   (M is the prediction order)                    *
 *  The polynomial is given by                                  *
 *    C(x) = T_n(x) + f(1)T_n-1(x) + ... +f(n-1)T_1(x) + f(n)/2 *
 * Arguments:                                                   *
 *  x:     input value of evaluation; x = cos(frequency) in Q15 *
 *  f[]:   coefficients of the pol.                             *
 *                         in Q11(Chebps_11), in Q10(Chebps_10) *
 *  n:     order of the pol.                                    *
 *                                                              *
 * The value of C(x) is returned. (Saturated to +-1.99 in Q14)  *
 *                                                              *
 *--------------------------------------------------------------*/
 /*
static Word16 Chebps_11(Word16 x, Word16 f[], Word16 n)
{
  Word16 i, j, cheb;
  Word16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
  Word32 t0;



  b2_h = 256;                          
  b2_l = 0;

  t0 = L_mult(x, 512);                 
  t0 = L_mac(t0, f[1], 4096);           
  L_Extract(t0, &b1_h, &b1_l);         

  for (i = 2; i<n; i++)
  {
    t0 = Mpy_32_16(b1_h, b1_l, x);      
    t0 = L_shl(t0, 1);
    t0 = L_mac(t0,b2_h,(Word16)-32768L);
    t0 = L_msu(t0, b2_l, 1);
    t0 = L_mac(t0, f[i], 4096);        

    L_Extract(t0, &b0_h, &b0_l);       

    b2_l = b1_l;                      
    b2_h = b1_h;
    b1_l = b0_l;                      
    b1_h = b0_h;
  }

  t0 = Mpy_32_16(b1_h, b1_l, x);       
  t0 = L_mac(t0, b2_h,(Word16)-32768L);
  t0 = L_msu(t0, b2_l, 1);
  t0 = L_mac(t0, f[i], 2048);          

  for(j=6; j>0; j--)
  {
         if (t0 > (Word32) 0X3fffffffL)
         {
            Overflow = 1;
            t0 = MAX_32;
            break;
         }
         else
         {
            if (t0 < (Word32) 0xc0000000L)
              {
               Overflow = 1;
               t0 = MIN_32;
               break;
              }
         }
	  t0 <<= 1;
   }
  
  cheb = extract_h(t0);                
  return(cheb);
}


static Word16 Chebps_10(Word16 x, Word16 f[], Word16 n)
{
  Word16 i, j, cheb;
  Word16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
  Word32 t0;



  b2_h = 128;                      
  b2_l = 0;

  t0 = L_mult(x, 256);              
  t0 = L_mac(t0, f[1], 4096);      
  L_Extract(t0, &b1_h, &b1_l);       

  for (i = 2; i<n; i++)
  {
    t0 = Mpy_32_16(b1_h, b1_l, x);     
    //t0 = L_shl(t0, 1);
    if (t0 > (Word32) 0X3fffffffL)
    {
            Overflow = 1;
            t0 = MAX_32;
     }
     else if (t0 < (Word32) 0xc0000000L)
     {
             Overflow = 1;
             t0 = MIN_32;              
     }
     else
     {
	  t0 <<= 1;
     } 
	 
    t0 = L_mac(t0,b2_h,(Word16)-32768L);
    t0 = L_msu(t0, b2_l, 1);
    t0 = L_mac(t0, f[i], 4096);         

    L_Extract(t0, &b0_h, &b0_l);        

    b2_l = b1_l;                       
    b2_h = b1_h;
    b1_l = b0_l;                      
    b1_h = b0_h;
  }

  t0 = Mpy_32_16(b1_h, b1_l, x);       
  t0 = L_mac(t0, b2_h,(Word16)-32768L); 
  t0 = L_msu(t0, b2_l, 1);
  t0 = L_mac(t0, f[i], 2048);           
    for(j=7; j>0; j--)
    {
         if (t0 > (Word32) 0X3fffffffL)
         {
            Overflow = 1;
            t0 = MAX_32;
            break;
         }
         else
         {
            if (t0 < (Word32) 0xc0000000L)
              {
               Overflow = 1;
               t0 = MIN_32;
               break;
              }
         }
	  t0 <<= 1;
    }
  
  cheb = extract_h(t0);               

  return(cheb);
}

*/
