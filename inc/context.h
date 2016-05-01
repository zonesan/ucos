/*
*********************************************************************************************************
*                                Copyright (c) 2005 vBridge Microsystem, Inc.  
*                                      Unpublished & Not for Publication
*                                            All Rights Reserved                   
*   
* File        : includes.h             
*                                                                         
* Description :                                  
*
* Date        : Sep 20, 2005                                                           
**********************************************************************************************************
*/

/* store the interrupted task's context:
     push context: at, v0-v1, a0-a3, t0-t9, s0-s7, gp, s8/fp, ra, & pc
   the following registers need not be saved:
     zero   : always zero
     k0, k1 : just used for interrupt/trap 
     sp     : is stored in OSTCB
*/     
#define STORE_REG_RET(Retaddr) \
      .set    noat;            \
      .set    noreorder;       \
                               \
      subu    sp, 128;         \
      sw      ra, 0(sp);       \
      sw      fp, 4(sp);       \
      sw      gp, 8(sp);       \
      sw      t9, 12(sp);      \
      sw      t8, 16(sp);      \
      sw      s7, 20(sp);      \
      sw      s6, 24(sp);      \
      sw      s5, 28(sp);      \
      sw      s4, 32(sp);      \
      sw      s3, 36(sp);      \
      sw      s2, 40(sp);      \
      sw      s1, 44(sp);      \
      sw      s0, 48(sp);      \
      sw      t7, 52(sp);      \
      sw      t6, 56(sp);      \
      sw      t5, 60(sp);      \
      sw      t4, 64(sp);      \
      sw      t3, 68(sp);      \
      sw      t2, 72(sp);      \
      sw      t1, 76(sp);      \
      sw      t0, 80(sp);      \
      sw      a3, 84(sp);      \
      sw      a2, 88(sp);      \
      sw      a1, 92(sp);      \
      sw      a0, 96(sp);      \
      sw      v1, 100(sp);     \
      sw      v0, 104(sp);     \
      sw      AT, 108(sp);     \
      mfhi    t0;              \
      mflo    t1;              \
      sw      t0, 112(sp);     \
      sw      t1, 116(sp);     \
      MFC0(   t0, C0_STATUS ); \
      sw      t0, 120(sp);     \
      sw      Retaddr, 124(sp);\
                               \
      .set    at

// pop context (normal task switch): at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define  RESTORE_REG_RET()    \
      .set    noat;           \
      .set    noreorder;      \
                              \
      lw      ra, 0(sp);      \
      lw      fp, 4(sp);      \
      lw      gp, 8(sp);      \
      lw      t9, 12(sp);     \
      lw      t8, 16(sp);     \
      lw      s7, 20(sp);     \
      lw      s6, 24(sp);     \
      lw      s5, 28(sp);     \
      lw      s4, 32(sp);     \
      lw      s3, 36(sp);     \
      lw      s2, 40(sp);     \
      lw      s1, 44(sp);     \
      lw      s0, 48(sp);     \
      lw      t7, 52(sp);     \
      lw      t6, 56(sp);     \
      lw      t5, 60(sp);     \
      lw      t4, 64(sp);     \
      lw      t3, 68(sp);     \
      lw      t2, 72(sp);     \
      lw      t1, 76(sp);     \
      lw      t0, 80(sp);     \
      lw      a3, 84(sp);     \
      lw      a2, 88(sp);     \
      lw      a1, 92(sp);     \
      lw      a0, 96(sp);     \
      lw      v1, 100(sp);    \
      lw      v0, 104(sp);    \
      lw      AT, 108(sp);    \
      lw      k0, 112(sp);    \
      lw      k1, 116(sp);    \
      mthi    k0;             \
      mtlo    k1;             \
      lw      k0, 120(sp);    \
      MTC0(   k0, C0_STATUS); \
      lw      k0, 124(sp);    \
      addu    sp, 128;        \
      jr      k0;             \
      nop;                    \
                              \
      .set    at

// pop context(task switch in interrupt): at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_ERET() \
      .set    noat; \
      .set    noreorder; \
      \
      lw      ra, 0(sp); \
      lw      fp, 4(sp); \
      lw      gp, 8(sp); \
      lw      t9, 12(sp); \
      lw      t8, 16(sp); \
      lw      s7, 20(sp); \
      lw      s6, 24(sp); \
      lw      s5, 28(sp); \
      lw      s4, 32(sp); \
      lw      s3, 36(sp); \
      lw      s2, 40(sp); \
      lw      s1, 44(sp); \
      lw      s0, 48(sp); \
      lw      t7, 52(sp); \
      lw      t6, 56(sp); \
      lw      t5, 60(sp); \
      lw      t4, 64(sp); \
      lw      t3, 68(sp); \
      lw      t2, 72(sp); \
      lw      t1, 76(sp); \
      lw      t0, 80(sp); \
      lw      a3, 84(sp); \
      lw      a2, 88(sp); \
      lw      a1, 92(sp); \
      lw      a0, 96(sp); \
      lw      v1, 100(sp); \
      lw      v0, 104(sp); \
      lw      AT, 108(sp); \
      lw      k0, 112(sp);    \
      lw      k1, 116(sp);    \
      mthi    k0;             \
      mtlo    k1;             \
      lw      k0, 120(sp); \
      ori     k0, k0, 0x2;\
      MTC0(   k0, C0_STATUS ); \
      lw      k0, 124(sp); \
      MTC0(   k0, C0_EPC); \
      addu    sp, 128; \
      eret; \
      nop; \
      \
      .set    at
