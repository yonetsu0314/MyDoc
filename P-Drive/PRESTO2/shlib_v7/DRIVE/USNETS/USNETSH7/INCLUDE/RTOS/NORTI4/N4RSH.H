/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/n4rsh.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: n4rsh.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/******************************************************************************
* NORTi3/SH Definitions                                                       *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 22/Apr/2000                                                                 *
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

#elif defined(__SH7000)     /* Green Hills */

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

#else                       /* Hitachi C */

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
  #if (defined(SH3E)||defined(SH4))
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
  #if (defined(SH3)||defined(SH3E)||defined(SH4))
    FP pc;
    int sr;
  #endif
  #if (defined(SH3E)||defined(SH4))
    int fpscr;
    int fpul;
  #endif
    int pr;
  #if (defined(SH3E)||defined(SH4))
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
#elif defined(__SH7000)         /* Green Hills */
#pragma ghs section data=".MPLMEM"
int cdecl MPLMEM[(MPLMSZ)/SIZEOF_INT] = {0};
#pragma ghs section data=default
#else                           /* Hitachi C */
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
#elif defined(__SH7000)         /* Green Hills */
#pragma ghs section data=".STKMEM"
int cdecl STKMEM[(STKMSZ)/SIZEOF_INT] = {0};
#pragma ghs section data=default
#else                           /* Hitachi C */
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

#if (defined(SH3)||defined(SH3E)||defined(SH4))
#define chg_ims_sub(imask)  IMASK = 0x40000000 | (UW)((imask << 4) & 0x00f0)
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

#elif defined(__SH7000)     /* Green Hills */
void *get_gbr(void);

#else                       /* Hitachi C */
#include <machine.h>
#if !(defined(SH3)||defined(SH3E)||defined(SH4))           /* Hitachi C, SH-1/2 */
#define dis_ims()   do{int sr;sr=get_cr();set_cr(KNL_SR);IMASK=(UH)sr;}while(0)
#define ret_ims2()  set_cr(IMASK)
#define ope_ims()   do{int im,sr;im=IMASK;sr=get_cr();set_cr(im);if(im!=sr){set_cr(sr);IMASK=(UH)im;}}while(0)
#define return_ret_ims()    do{set_cr(IMASK);return E_OK;}while(0)
#endif
#endif

#if (defined(SH3E)||defined(SH4))
#if defined(__GNUC__)       /* exeGCC */
#define sta_tsk_sub()       ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x000C0001;    \
                            ctx->sr = 0x40000000
#elif defined(__SH7000)     /* Green Hills */
#define sta_tsk_sub()       ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000
#else                       /* Hitachi C */
#define sta_tsk_sub()       ctx->wid = (ID)stacd;       \
                            ctx->pr = (int)v4_ext_tsk;  \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0x40000000
#endif

#elif defined(SH3)
#define sta_tsk_sub()       ctx->wid = (ID)stacd;       \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000;       \
                            ctx->pr = (int)v4_ext_tsk
#else

#define sta_tsk_sub()       ctx->wid = (ID)stacd;       \
                            ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0;                \
                            ctx->pr = (int)v4_ext_tsk
#endif

#if (defined(SH3E)||defined(SH4))
#if defined(__GNUC__)       /* exeGCC */
#define ras_tex_sub()       ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x000C0001;    \
                            ctx->sr = 0x40000000;
#elif defined(__SH7000)     /* Green Hills */
#define ras_tex_sub()       ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = 0x00040001;    \
                            ctx->sr = 0x40000000;
#else                       /* Hitachi C */
#define ras_tex_sub()       ctx->gbr = (int)get_gbr();  \
                            ctx->fpscr = (int)get_fpscr();\
                            ctx->sr = 0x40000000;
#endif

#elif defined(SH3)
#define ras_tex_sub()       ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0x40000000;

#else
#define ras_tex_sub()       ctx->gbr = (int)get_gbr();  \
                            ctx->sr = 0;

#endif

#endif /* N4RSH_H */
