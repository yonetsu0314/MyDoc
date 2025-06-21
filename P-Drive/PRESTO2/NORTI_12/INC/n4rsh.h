/******************************************************************************
* NORTi Version 4 (SH) Definitions                                            *
*                                                                             *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 22/Apr/2000                                                                 *
* 30/Oct/2000  supported EW/ICC                                               *
* 20/Nov/2000  data -> bss section (Green Hills)                              *
* 25/Dec/2000  supported SH2E series                                          *
* 28/Feb/2000  supported XASSV series                                         *
* 06/Sep/2001  modified Hitachi Version macro                                 *
* 08/Nov/2001  modified chg_ims_sub                                           *
* 22/Jan/2002  supported SH3 (DSP & CL Bit)                                   *
* 17/Oct/2002  corrected XASSV sta_tsk_sub                                 TA *
* 03/Jun/2003  Renesas C 6.00R1, 6.0A ‚Å‚Í HV60A ‚ð’è‹`‚·‚é‚æ‚¤‚ÉC³      KA *
******************************************************************************/

#ifndef N4RSH_H
#define N4RSH_H

/* context structure */

#if defined(__GNUC__)       /* exeGCC */

typedef struct t_ctx
{
  #if defined(SH4)
  #if defined(GRX)
    int xf0;
    int xf1;
    int xf2;
    int xf3;
    int xf4;
    int xf5;
    int xf6;
    int xf7;
    int xf8;
    int xf9;
    int xf10;
    int xf11;
    int xf12;
    int xf13;
    int xf14;
    int xf15;
  #endif
    int fr12;
    int fr13;
    int fr14;
    int fr15;
  #endif
    int r8;
    int r9;
    int r10;
    int r11;
    int r12;
    int r13;
    int gbr;
    int mach;
    int macl;
  #if (defined(SH3)||defined(SH4))
    FP pc;
    int sr;
  #endif
  #if defined(SH4)
    int fpscr;
    int fpul;
    int fr0;
    int fr1;
    int fr2;
    int fr3;
    int fr4;
    int fr5;
    int fr6;
    int fr7;
    int fr8;
    int fr9;
    int fr10;
    int fr11;
  #endif
    ID  wid;            /* r4 */
    UINT ptn;           /* r5 */
    VP ptr;             /* r6 */
    int r7;
    int ercd;           /* r0 */
    int r1;
    int r2;
    int r3;
    int pr;
    int r14;
  #if (!(defined(SH3)||defined(SH4)))
    FP pc;
    int sr;
  #endif

} T_CTX;

#elif defined(__ghs)        /* Green Hills */

typedef struct t_ctx
{
  #if defined(SH4)
  #if defined(GRX)
    int xf0;
    int xf1;
    int xf2;
    int xf3;
    int xf4;
    int xf5;
    int xf6;
    int xf7;
    int xf8;
    int xf9;
    int xf10;
    int xf11;
    int xf12;
    int xf13;
    int xf14;
    int xf15;
  #endif
    int fr12;
    int fr13;
    int fr14;
    int fr15;
  #endif
    int r8;
    int r9;
    int r10;
    int r11;
    int r12;
    int r13;
    int r14;
    int gbr;
    int mach;
    int macl;
  #if (defined(SH3)||defined(SH4))
    FP pc;
    int sr;
  #endif
  #if defined(SH4)
    int fpscr;
    int fpul;
    int fr0;
    int fr1;
    int fr2;
    int fr3;
    int fr4;
    int fr5;
    int fr6;
    int fr7;
    int fr8;
    int fr9;
    int fr10;
    int fr11;
  #endif
    int pr;
    int ercd;           /* r0 */
    int r1;
    int r2;
    int r3;
    ID  wid;            /* r4 */
    UINT ptn;           /* r5 */
    VP ptr;             /* r6 */
    int r7;
  #if (!(defined(SH3)||defined(SH4)))
    FP pc;
    int sr;
  #endif

} T_CTX;

#elif defined(__TID__)     /* EW/ICC */

typedef struct t_ctx
{
    int r14;
    int r13;
    int r12;
    int r11;
    int r10;
    int r9;
    int r8;
    int gbr;
    int mach;
    int macl;
  #if defined(SH3)
    FP pc;
    int sr;
  #endif
    int r7;
    VP ptr;             /* r6 */
    UINT ptn;           /* r5 */
    int ercd;           /* r4 */
    ID  wid;            /* r3 */
    int r2;
    int r1;
    int pr;
    int r0;
  #if (!defined(SH3))
    FP pc;
    int sr;
  #endif

} T_CTX;

#elif defined(GAIO)         /* XASSV */

typedef struct t_ctx
{
  #if (defined(GRX)&&defined(SH4))
    int xf15;
    int xf14;
    int xf13;
    int xf12;
    int xf11;
    int xf10;
    int xf9;
    int xf8;
    int xf7;
    int xf6;
    int xf5;
    int xf4;
    int xf3;
    int xf2;
    int xf1;
    int xf0;
  #endif
    int gbr;
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr15;
    int fr14;
    int fr13;
    int fr12;
    int fr11;
    int fr10;
    int fr9;
    int fr8;
  #endif
    int r1;
    int r2;
    int r8;
    int r9;
    int r14;
  #if (defined(SH3)||defined(SH3E)||defined(SH4))
    int sr;
    FP pc;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr7;
    int fr6;
    int fr5;
    int fr4;
    int fr3;
    int fr2;
    int fr1;
    int fr0;
  #endif
    ER  ercd;           /* r0 */
    VP  ptr;            /* r7 */
    UINT ptn;           /* r6 */
    ID  wid;
    int stacd;          /* r4 */
    int r3;
    int r13;
    int r12;
    int r11;
    int r10;
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fpscr;
    int fpul;
  #endif
    int mach;
    int macl;
    int pr;
  #if (!(defined(SH3)||defined(SH3E)||defined(SH4)))
    FP pc;
    int sr;
  #endif

} T_CTX;

#elif (defined(__HITACHI__) && (__HITACHI_VERSION__ >= 0x0700))
                            /* Renesas C Ver.7.0 */
typedef struct t_ctx
{
  #if (defined(GRX)&&defined(SH4))
    int xf0;
    int xf1;
    int xf2;
    int xf3;
    int xf4;
    int xf5;
    int xf6;
    int xf7;
    int xf8;
    int xf9;
    int xf10;
    int xf11;
    int xf12;
    int xf13;
    int xf14;
    int xf15;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr12;
    int fr13;
    int fr14;
    int fr15;
  #endif
    int r8;
    int r9;
    int r10;
    int r11;
    int r12;
    int r13;
  #if ((!defined(SH4))||defined(_FPD)||defined(_FPS))
    int r14;
  #endif
    int gbr;
    int mach;
    int macl;
  #if (defined(SH3)||defined(SH3E)||defined(SH4))
    FP pc;
    int sr;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fpscr;
    int fpul;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr11;
    int fr10;
    int fr9;
    int fr8;
    int fr7;
    int fr6;
    int fr5;
    int fr4;
    int fr3;
    int fr2;
    int fr1;
    int fr0;
  #endif
    int pr;
  #if (defined(SH4)&&(!defined(_FPD))&&(!defined(_FPS)))
    int r14;
  #endif
    int r7;
    VP ptr;             /* r6 */
    UINT ptn;           /* r5 */
    ID  wid;            /* r4 */
    int r3;
    int r2;
    int r1;
    int ercd;           /* r0 */
  #if (!(defined(SH3)||defined(SH3E)||defined(SH4)))
    FP pc;
    int sr;
  #endif

} T_CTX;

#else                       /* Renesas C */

typedef struct t_ctx
{
  #if (defined(GRX)&&defined(SH4))
    int xf0;
    int xf1;
    int xf2;
    int xf3;
    int xf4;
    int xf5;
    int xf6;
    int xf7;
    int xf8;
    int xf9;
    int xf10;
    int xf11;
    int xf12;
    int xf13;
    int xf14;
    int xf15;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr12;
    int fr13;
    int fr14;
    int fr15;
  #endif
    int r8;
    int r9;
    int r10;
    int r11;
    int r12;
    int r13;
    int r14;
    int gbr;
    int mach;
    int macl;
 #if (__HITACHI_VERSION__ >= 0x0600)
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fpscr;
    int fpul;
  #endif
  #if (defined(SH3)||defined(SH3E)||defined(SH4))
    int sr;
    FP pc;
  #endif
 #else
  #if (defined(SH3)||defined(SH3E)||defined(SH4))
    FP pc;
    int sr;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fpscr;
    int fpul;
  #endif
 #endif
  #if (__HITACHI_VERSION__ < 0x0600) || !((defined(SH4)||defined(SH2E)) && !defined(HV60A))
    int pr;
  #endif
  #if (defined(SH2E)||defined(SH3E)||defined(SH4))
    int fr0;
    int fr1;
    int fr2;
    int fr3;
    int fr4;
    int fr5;
    int fr6;
    int fr7;
    int fr8;
    int fr9;
    int fr10;
    int fr11;
  #endif
  #if (__HITACHI_VERSION__ >= 0x0600) && ((defined(SH4)||defined(SH2E)) && !defined(HV60A))
    int pr;
  #endif
    int ercd;           /* r0 */
    int r1;
    int r2;
    int r3;
    ID  wid;            /* r4 */
    UINT ptn;           /* r5 */
    VP ptr;             /* r6 */
    int r7;
  #if (!(defined(SH3)||defined(SH3E)||defined(SH4)))
    FP pc;
    int sr;
  #endif

} T_CTX;

#endif

/* memory pool & stack area definitions */

#ifdef MPLMSZ
#if ((MPLMSZ)/SIZEOF_INT!=0)

#if defined(__GNUC__)           /* exeGCC */
int cdecl MPLMEM[(MPLMSZ)/SIZEOF_INT] __attribute__ ((section("MPLMEM"))) = {0};
#elif defined(__ghs)            /* Green Hills */
#pragma ghs section bss=".MPLMEM"
int cdecl MPLMEM[(MPLMSZ)/SIZEOF_INT];
#pragma ghs section bss=default
#elif defined(__TID__)          /* EW/ICC */
#pragma dataseg=MPLMEM
__no_init int MPLMEM[(MPLMSZ)/SIZEOF_INT];
#pragma dataseg=default
#elif defined(GAIO)             /* XASSV */
int cdecl MPLMEM[(MPLMSZ)/SIZEOF_INT];
#else                           /* Renesas C */
#pragma section MPLMEM
int cdecl MPLMEM[(MPLMSZ)/SIZEOF_INT];
#pragma section
#endif

#endif
#endif

#ifdef STKMSZ
#if ((STKMSZ)/SIZEOF_INT!=0)

#if defined(__GNUC__)           /* exeGCC */
int cdecl STKMEM[(STKMSZ)/SIZEOF_INT] __attribute__ ((section("STKMEM"))) = {0};
#elif defined(__ghs)            /* Green Hills */
#pragma ghs section bss=".STKMEM"
int cdecl STKMEM[(STKMSZ)/SIZEOF_INT];
#pragma ghs section bss=default
#elif defined(__TID__)          /* EW/ICC */
#pragma dataseg=STKMEM
__no_init int STKMEM[(STKMSZ)/SIZEOF_INT];
#pragma dataseg=default
#elif defined(GAIO)             /* XASSV */
int cdecl STKMEM[(STKMSZ)/SIZEOF_INT];
#else                           /* Renesas C */
#pragma section STKMEM
int cdecl STKMEM[(STKMSZ)/SIZEOF_INT];
#pragma section
#endif

#endif
#endif

/* CPU-dependent */

#if (defined(SH3)||defined(SH3E)||defined(SH4))
extern const UW KNL_SR;
#else
extern const UH KNL_SR;
#endif

#define syssta_sub()        IMASK &= ~0x00f0
#define unl_cpu_sub()       IMASK &= ~0x00f0

#if (defined(SH3E)||defined(SH4))
#define chg_ims_sub(imask)  IMASK = 0x40000000 | (UW)((imask << 4) & 0x00f0)
#elif defined(SH3)
#define chg_ims_sub(imask) {IMASK &= ~0x000000f0;\
                            IMASK |= ((imask << 4) & 0x00f0);}
#else
#define chg_ims_sub(imask)  IMASK = (UH)((imask << 4) & 0x00f0)
#endif

#define ref_sys_sub(imask)  ((imask & 0x00f0) >= (KNL_SR & 0x00f0))
#define ref_ims_sub(ims)    ((ims & 0x00f0) >> 4)

#define cre_tsk_sub()       stksz -= sizeof(T_CTX)
#define c_sysini_sub()      \
    if (pSTKMEM != NULL)                        \
    {   stktop = (B *)pSTKMEM;                  \
        sp = stktop + CFG.stkmsz;               \
    }                                           \
    else                                        \
    {   sp -= CFG.tstksz;                       \
    }                                           \
    ISP = sp;                                   \
    i = CFG.istksz;                             \
    while (i-- != 0)                            \
        *--sp = 0x00;

#if defined(__GNUC__)       /* exeGCC */
void *get_gbr(void);
int get_cr(void);

#elif defined(__ghs)        /* Green Hills */
void *get_gbr(void);
int get_cr(void);

#elif defined(__TID__)      /* EW/ICC */
void *get_gbr(void);

#elif defined(GAIO)         /* XASSV */
#include <ht1_inline.h>

#else                       /* Renesas C */
#include <machine.h>
#if !(defined(SH3)||defined(SH3E)||defined(SH4))           /* Renesas C, SH-1/2 */
#define dis_ims()   do{int sr;sr=get_cr();set_cr(KNL_SR);IMASK=(UH)sr;}while(0)
#define ret_ims2()  set_cr(IMASK)
#define ope_ims()   do{int im,sr;im=IMASK;sr=get_cr();set_cr(im);if(im!=sr){set_cr(sr);IMASK=(UH)im;}}while(0)
#ifndef return_ret_ims
#define return_ret_ims(a)   do{set_cr(IMASK);return E_OK;}while(0)
#endif
#endif
#endif

#if (defined(SH3E)||defined(SH4))
#if defined(__GNUC__)       /* exeGCC */
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x000C0001;    \
                            ctx->sr = 0x40000000; }
#elif defined(__ghs)        /* Green Hills */
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000; }
#elif defined(GAIO)         /* XASSV */
#define sta_tsk_sub()     { ctx->stacd = (ID)stacd;     \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000; }
#else                       /* Renesas C */
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0x40000000; }
#endif

#elif defined(SH3)
#if defined(__TID__)        /* EW/ICC */
#define sta_tsk_sub()     { ctx->ercd = (ID)stacd;      \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000 | ((int)get_cr() & 0x8fffff00); \
                            ctx->pr = (int)v4_ext_tsk; }
#elif defined(GAIO)         /* XASSV */
#define sta_tsk_sub()     { ctx->stacd = (ID)stacd;     \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000 | ((int)get_cr() & 0x8fffff00); \
                            ctx->pr = (int)v4_ext_tsk; }
#else
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000 | ((int)get_cr() & 0x8fffff00); \
                            ctx->pr = (int)v4_ext_tsk; }
#endif

#elif defined(SH2E)
#if defined(__HITACHI__)    /* Renesas C */
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk; }
#elif defined(GAIO)         /* XASSV */
#define sta_tsk_sub()     { ctx->stacd = (ID)stacd;     \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk; }
#endif

#else
#if defined(__TID__)        /* EW/ICC */
#define sta_tsk_sub()     { ctx->ercd = (ID)stacd;      \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk; }
#elif defined(GAIO)
#define sta_tsk_sub()     { ctx->stacd = (ID)stacd;     \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk; }
#else
#define sta_tsk_sub()     { ctx->wid = (ID)stacd;       \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk; }
#endif
#endif

#if (defined(SH3E)||defined(SH4))
#if defined(__GNUC__)       /* exeGCC */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x000C0001;    \
                            ctx->sr = 0x40000000; }
#elif defined(__ghs)        /* Green Hills */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000; }
#elif defined(GAIO)         /* XASSV */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000; }
#else                       /* Renesas C */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0x40000000; }
#endif

#elif defined(SH3)
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000 | ((int)get_cr() & 0x8fffff00); }

#elif defined(SH2E)
#if defined(__HITACHI__)    /* Renesas C */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0; }
#elif defined(GAIO)         /* XASSV */
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0; }
#endif

#else
#define ras_tex_sub()     { ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0; }

#endif

#endif /* N4RSH_H */
