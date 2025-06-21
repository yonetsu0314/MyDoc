/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/nosys4.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: nosys4.h $
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
* NORTi4 system definitions                                                   *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
******************************************************************************/

#ifndef NOSYS4_H
#define NOSYS4_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>
/************************************/
/* Kernel configurations            */
/************************************/


typedef H OBJ_ID;
#ifdef LARGE_KERNEL
typedef OBJ_ID TSK_ID;
#else
typedef UB TSK_ID;
#endif

typedef B TSK_PRI;

#define TSK_PQUETAIL ((TSK_ID)-1)
#define TSK_QUETAIL ((TSK_ID)-2)

/************************************/
/* CPU-dependent definitions        */
/************************************/

#if defined(CPU_86)
#include "n4r86.h"
#elif defined(CPU_68K)
#include "n4r68k.h"
#elif defined(CPU_H83)
#include "n4rh83.h"
#elif defined(CPU_H85)
#include "n4rh85.h"
#elif defined(CPU_H8S)
#include "n4rh8s.h"
#elif defined(CPU_SH)
#include </itron4/n4rsh.h>
#elif defined(CPU_V850)
#include "n4rv850.h"
#elif defined(CPU_PPC)
#include "n4rppc.h"
#elif (defined(CPU_ARM)||defined(CPU_THUMB))
#include "n4rarm.h"
#endif

#define sns_ctx_sub()       ((INEST)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))

/************************************/
/* Hook-routine definitions         */
/************************************/

void vsnd_deb0(long);
void vsnd_deb1(long, long);
void vsnd_deb2(long, long, long);
void vsnd_deb3(long, long, long, long);
void vsnd_deb4(long, long, long, long, long);
void vsnd_deb5(long, long, long, long, long, long);
void vsnd_deb6(long, long, long, long, long, long, long);
void vsnd_debv(long, va_list);

void visnd_deb0(long);
void visnd_deb1(long, long);
void visnd_deb2(long, long, long);
void visnd_deb3(long, long, long, long);
void visnd_deb4(long, long, long, long, long);
void visnd_deb5(long, long, long, long, long, long);
void visnd_deb6(long, long, long, long, long, long, long);
void visnd_debv(long, va_list);

#ifndef HOOK

#define vsnd_deb0(a)
#define vsnd_deb1(a,b)
#define vsnd_deb2(a,b,c)
#define vsnd_deb3(a,b,c,d)
#define vsnd_deb4(a,b,c,d,e)
#define vsnd_deb5(a,b,c,d,e,f)
#define vsnd_deb6(a,b,c,d,e,f,g)
#define vsnd_debv(a,b)

#define visnd_deb0(a)
#define visnd_deb1(a,b)
#define visnd_deb2(a,b,c)
#define visnd_deb3(a,b,c,d)
#define visnd_deb4(a,b,c,d,e)
#define visnd_deb5(a,b,c,d,e,f)
#define visnd_deb6(a,b,c,d,e,f,g)
#define visnd_debv(a,b)
#endif

/************************************/
/* control block definitions        */
/************************************/

/* OS Memory Header */

typedef struct t_mem
{   struct t_mem *next;
    unsigned size;
} T_MEM;

#if defined(CPU_86)
typedef struct t_nmem
{   struct t_nmem NEAR *next;
    unsigned size;
} T_NMEM;
#else
#define T_NMEM      T_MEM
#endif

/* Task Exception Control Block */

typedef struct t_excb
{
    T_CTX *sp;
    TEXPTN pndptn;
    H stat;
    B ctxsv;
    const V4_T_DTEX *dtex;
} T_EXCB;


/* Task Control Block */

typedef struct t_tcb
{   T_CTX FAR *sp;
    TSK_PRI pri;
    UB sts;
    TSK_ID nid;
    TSK_ID pid;
    B wup;
    B sus;
    TSK_ID ntm;
    TSK_ID ptm;
    DLYTIME tmo;
    TSK_ID NEAR *que;
    T_CTX *stk;
    const V4_T_CTSK *ctsk;
    T_EXCB NEAR *excb;
    RELTIM rcnt;
    OBJ_ID lock;
    B act;
    TSK_PRI bpri;
} T_TCB;

/* System Task Control Block */

typedef struct t_systcb
{   T_CTX FAR *sp;
    TSK_PRI pri;
    UB sts;
    TSK_ID nid;
    TSK_ID pid;
    B wup;
    B sus;
    TSK_ID ntm;
    TSK_ID ptm;
    DLYTIME tmo;
    TSK_ID NEAR *que;
} T_SYSTCB;

/* Cyclic Handler Control Block */

#if (SIZEOF_INT == 1)

typedef struct t_cyc
{   H utime;
    UH ltime;
    TSK_ID ntm;
    TSK_ID ptm;
    UINT cycact;
    CYCTIME cyctim;
    VP_INT exinf;
    FP cychdr;
} T_CYC;

#else

typedef struct t_cyc
{   H utime;
    UW ltime;
    TSK_ID ntm;
    TSK_ID ptm;
    UINT cycact;
    CYCTIME cyctim;
    VP_INT exinf;
    FP cychdr;
} T_CYC;

#endif

/* Alarm Handler Control Block */

#if (SIZEOF_INT == 1)

typedef struct t_alm
{   H utime;
    UH ltime;
    TSK_ID ntm;
    TSK_ID ptm;
    VP_INT exinf;
    FP almhdr;
} T_ALM;

#else

typedef struct t_alm
{   H utime;
    UW ltime;
    TSK_ID ntm;
    TSK_ID ptm;
    VP_INT exinf;
    FP almhdr;
} T_ALM;

#endif

/* Semaphore Control Block */

typedef struct t_sem
{
    V4_T_CSEM *exi;
    H cnt;
    H max;
    TSK_ID que[2];
} T_SEM;

/* EventFlag Control Block */

typedef struct t_flg
{   V4_T_CFLG *exi;
    UINT ptn;
    B wmul;
    TSK_ID que[2];
} T_FLG;

/* Data Queue Control Block */

typedef struct t_dtq
{   V4_T_CDTQ *exi;
    VP_INT top;
    VP_INT put;
    VP_INT get;
    VP_INT btm;
    UINT size;
    UINT cnt;
    TSK_ID rque;
    TSK_ID sque[2];
} T_DTQ;

/* Mailbox Control Block */

typedef struct t_mbx
{   V4_T_CMBX *exi;
    UB mpri;
    UB size;
    UB moff;
    TSK_ID que[2];
} T_MBX;

/* Mutex Control Block */

typedef struct t_mtx
{   V4_T_CMTX *exi;
    ATR atr;
    OBJ_ID nid;
    TSK_ID occ;
    TSK_PRI ceil;
    TSK_ID que[2];
} T_MTX;

/* MessageBuffer Control Block */

typedef struct t_mbf
{   V4_T_CMBF *exi;
    B *buf;
    UH putp;
    UH getp;
    UH allsz;
    UH frsz;
    UH maxsz;
    TSK_ID wtsk;
    TSK_ID stsk[2];
} T_MBF;

/* Port for Rendezvous Control Block */

typedef struct t_por
{   V4_T_CPOR *exi;
    UH maxcmsz;
    UH maxrmsz;
    TSK_ID atsk;
    TSK_ID wtsk[2];
} T_POR;

typedef struct t_cporctx
{   VP buf;
    UH size;
    UH maxsz;
    RDVNO rdvno;
} T_CPORCTX;

typedef struct t_aporctx
{   VP buf;
    RDVNO *p_rdvno;
} T_APORCTX;

/* Variable-size MemoryPool Control Block */

typedef struct t_mpl
{   V4_T_CMPL *exi;
    T_MEM *top;
    B *allad;
    unsigned allsz;
    TSK_ID que[2];
} T_MPL;

/* Fixed-size MemoryPool Control Block */

typedef struct t_mpf
{   V4_T_CMPF *exi;
    B *top;
    B *allad;
    unsigned allsz;
    UH cnt;
    TSK_ID que[2];
} T_MPF;

/* ISR Control Block */

typedef struct t_isr_st
{   FP isr;
    VP_INT exinf;
    struct t_isr_st *next;
    VP prev;
} T_ISR_ST;


typedef struct t_isr
{   FP isr;
    VP_INT exinf;
    struct t_isr *next;
    VP prev;
    INTNO intno;
} T_ISR;

/* SVC Control Block */

typedef struct t_svc
{   FP svcrtn;
    B parn;
} T_SVC;

/************************************/
/* common constants                 */
/************************************/

/* T_TCB sts */

#define S_CLR       0x04    /* 0x01 /* EventFlag clear specification */
#define S_ORW       0x01    /* 0x02 /* EventFlag OR wait */
#define S_TMO       0x02    /* 0x04 /* WAIT with timeout */

#define S_DMT       0x00    /* DORMANT */
#define S_RDY       0x10    /* RUN,READY */

#define S_SLP       0x20    /* wait due to slp_tsk or tslp_tsk */
#define S_DLY       0x30    /* wait due to dly_tsk */
#define S_RDV       0x40    /* wait for rendezvous completion */
#define S_FLG       0x50    /* wait due to wai_flg or twai_flg */
#define S_SMB       0x60    /* wait due to snd_mbf or tsnd_mbf */
#define S_CAL       0x70    /* wait for rendezvous call */
#define S_ACP       0x80    /* wait for rendezvous accept */
#define S_SEM       0x90    /* wait due to wai_sem or twai_sem */
#define S_MBX       0xa0    /* wait due to rcv_msg or trcv_msg */
#define S_MBF       0xb0    /* wait due to rcv_mbf or trcv_mbf */
#define S_MPL       0xc0    /* wait due to get_blk or tget_blk */
#define S_MPF       0xd0    /* wait due to get_blf or tget_blf */
#define S_SDT       0x64    /* wait due to snd_dtq or rcv_dtq */
#define S_RDT       0xb4    /* wait due to rcv_dtq or snd_dtq */
#define S_MTX       0x94    /* wait due to loc_mtx */

#define STS_MSK0    0xf0
#define STS_MSK1    0xfc

/* pointer array to control block */

extern T_TCB NEAR * NEAR cdecl _pTCB[];
extern T_SEM NEAR * NEAR cdecl _pSEM[];
extern T_FLG NEAR * NEAR cdecl _pFLG[];
extern T_MBX NEAR * NEAR cdecl _pMBX[];
extern T_MBF NEAR * NEAR cdecl _pMBF[];
extern T_POR NEAR * NEAR cdecl _pPOR[];
extern T_MPL NEAR * NEAR cdecl _pMPL[];
extern T_MPF NEAR * NEAR cdecl _pMPF[];
extern T_CYC NEAR * NEAR cdecl _pCYC[];
extern T_ALM NEAR * NEAR cdecl _pALM[];
extern T_DTQ NEAR * NEAR cdecl _pDTQ[];
extern T_MTX NEAR * NEAR cdecl _pMTX[];
extern T_ISR NEAR * NEAR cdecl _pISR[];
extern T_SVC NEAR * NEAR cdecl _pSVC[];

/* pointer array macro */

#define pTCB    (_pTCB-1)
#define pSEM    (_pSEM-1)
#define pFLG    (_pFLG-1)
#define pMBX    (_pMBX-1)
#define pMBF    (_pMBF-1)
#define pPOR    (_pPOR-1)
#define pMPL    (_pMPL-1)
#define pMPF    (_pMPF-1)
#define pCYC    (_pCYC-1)
#define pALM    (_pALM-1)
#define pDTQ    (_pDTQ-1)
#define pMTX    (_pMTX-1)
#define pISR    (_pISR-1)
#define pSVC    (_pSVC-1)

/* limit */

#define _KERNEL_SVCMAXPAR 6

/* kernel constant data */

extern int NEAR * const cdecl pSYSMEM;
extern int * const cdecl pMPLMEM;
extern int * const cdecl pSTKMEM;

/* kernel data */

extern SYSTIME NEAR cdecl SYSCK;
#if (SIZEOF_INT == 1)
extern UB NEAR IMASK;
#elif (defined(SH3)||defined(SH3E)||defined(SH4))
extern UW NEAR cdecl IMASK;
#else
extern UH NEAR cdecl IMASK;
#endif
extern UH NEAR cdecl IDLCNT;
extern T_SYSTCB NEAR cdecl SYSTCB;
extern ER NEAR cdecl SYSER;
extern volatile TSK_PRI NEAR cdecl NOWPRI;
extern volatile B NEAR cdecl DELAY;
extern volatile B NEAR cdecl INEST;
extern volatile B NEAR cdecl DDISP;
extern volatile B NEAR cdecl SDISP;
extern volatile B NEAR cdecl TMREQ;
extern volatile B NEAR cdecl PREVTSK;
extern UB NEAR cdecl TMQMS;
extern UB NEAR cdecl CHQMS;
extern UB NEAR cdecl AHQMS;
extern UINT NEAR cdecl CTXPTN;
extern VP NEAR cdecl CTXPTR;
extern UINT NEAR cdecl WID;
extern B FAR * NEAR cdecl ISP;
extern T_NMEM NEAR * NEAR cdecl SYSTOP;
extern T_MEM * NEAR cdecl MPLTOP;
extern T_MEM * NEAR cdecl STKTOP;
#ifndef NOCFG3_H
extern UB NEAR cdecl RDQ[];
extern UB NEAR cdecl TMQ[];
extern UB NEAR cdecl CHQ[];
extern UB NEAR cdecl AHQ[];
#endif
extern B NEAR _KERNEL_NNM;
extern BOOL _kernel_inROM(VP);
extern BOOL _kernel_inSYSMEM(VP);

#ifdef __cplusplus
};
#endif
#endif /* NOSYS4_H */
