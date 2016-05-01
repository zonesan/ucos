/*--------------------------------------------------------*
 *         Static memory allocation.                      *
 *--------------------------------------------------------*/
Word16 MeanLSF[M];
Word16 Min_buffer[16];
Word16 Prev_Min, Next_Min, Min;
Word16 MeanE, MeanSE, MeanSLE, MeanSZC;
Word16 prev_energy;
Word16 count_sil, count_update, count_ext;
Word16 wg_flag, v_flag, less_count;



        /* Speech vector */

 Word16 old_speech[L_TOTAL];
 Word16 *speech, *p_window;
 Word16 *new_speech;                    /* Global variable */

        /* Weighted speech vector */

 Word16 old_wsp[L_FRAME+PIT_MAX];
 Word16 *wsp;

        /* Excitation vector */

 Word16 en_old_exc[L_FRAME+PIT_MAX+L_INTERPOL];
 Word16 *udi_exc;

        /* Lsp (Line spectral pairs) */

 Word16 en_lsp_old[M]={30000, 26000, 21000, 15000, 8000, 0, -8000,-15000,-21000,-26000};
 Word16 en_lsp_old_q[M];

        /* Filter's memory */
Word16 mem_w0[M];
 Word16   mem_w[M], mem_zero[M];
 Word16  en_sharp;

        /* For G.729B */
        /* DTX variables */
 Word16 en_pastVad;
 Word16 ppastVad;
 Word16 en_seed;

 Word16 past_qua_en_wg[4] = { -14336, -14336, -14336, -14336 };


 

