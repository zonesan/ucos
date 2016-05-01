/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/


#define	ROUND(dat32) round16(dat32)
/*	wg	*/




/*___________________________________________________________________________
 |                                                                           |
 |   Operators prototypes                                                    |
 |___________________________________________________________________________|
*/

/*Word16 sature(Word32 L_var1);           /* Limit to 16 bits,    1 */

//Word16 sature(Word32 L_var1);
Word16 add(Word16 var1, Word16 var2);     /* Short add,           1 */
Word16 sub(Word16 var1, Word16 var2);     /* Short sub,           1 */
Word16 abs_s(Word16 var1);                /* Short abs,           1 */
Word16 shl(Word16 var1, Word16 var2);     /* Short shift left,    1 */
Word16 shr(Word16 var1, Word16 var2);     /* Short shift right,   1 */
Word16 mult(Word16 var1, Word16 var2);    /* Short mult,          1 */
Word32 L_mult(Word16 var1, Word16 var2);  /* Long mult,           1 */

Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2); /* Mac,    1 */
Word32 L_msu(Word32 L_var3, Word16 var1, Word16 var2); /* Msu,    1 */

Word32 L_add(Word32 L_var1, Word32 L_var2);   /* Long add,        2 */
Word32 L_sub(Word32 L_var1, Word32 L_var2);   /* Long sub,        2 */
//Word32 L_negate(Word32 L_var1);               /* Long negate,     2 */
Word16 mult_r(Word16 var1, Word16 var2);  /* Mult with round,     2 */
Word32 L_shl(Word32 L_var1, Word16 var2); /* Long shift left,     2  L_shl_1 */
Word32 L_shl_1(Word32 L_var1, Word16 var2); /* Long shift left,     2  L_shl_1 */
Word32 L_shr(Word32 L_var1, Word16 var2); /* Long shift right,    2 */
Word16 shr_r(Word16 var1, Word16 var2);/* Shift right with round, 2 */
Word16 mac_r(Word32 L_var3, Word16 var1, Word16 var2);/* Mac with rounding, 2*/
Word16 msu_r(Word32 L_var3, Word16 var1, Word16 var2);/* Msu with rounding, 2*/

Word32 L_shr_r(Word32 L_var1, Word16 var2);/* Long shift right with round,  3*/
Word32 L_abs(Word32 L_var1);            /* Long abs,              3 */


Word16 norm_s(Word16 var1);             /* Short norm,           15 */
Word16 div_s(Word16 var1, Word16 var2); /* Short division,       18 */
Word16 norm_l(Word32 L_var1);           /* Long norm,            30 */

