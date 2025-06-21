/******************************************************************************
* NORTi4 system definitions                                                   *
*                                                                             *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
* 06/Aug/2000  Ver. 1.2x                                                      *
* 23/Aug/2000  Crorrected task ID type                                        *
* 28/Sep/2000  Supported PPC Series                                 OK(MiSPO) *
* 26/Mar/2001  Supported FR30 Series                                      TA  *
* 23/Jul/2001  Supported XASS-V ARM series                                TA  *
* 29/Jul/2001  Supported MIPS Series(R4000/R3000)                         OK  *
* 10/Oct/2001  Supported M32R Series by CC32R                             OK  *
* 13/Dec/2001  Supported M16C/60 Series                                   TA  *
* 04/Feb/2003  SDISP fiels size B->TSK_ID                                 KT  *
* 23/Apr/2003  T_NMEM.size SIZE->UINT                                     TA  *
******************************************************************************/

#ifndef NOSYS4_H
#define NOSYS4_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#ifdef HOOK
#include "n4hook.h"
#endif

/************************************/
/* Kernel configurations            */
/************************************/


typedef H OBJ_ID;
#ifdef LARGE_KERNEL
typedef UH TSK_ID;
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
#include "n4rh8.h"
#elif defined(CPU_H8S)
#include "n4rh8.h"
#elif defined(CPU_SH)
#include "n4rsh.h"
#elif defined(CPU_FR)
#include "n4rfr.h"
#elif defined(CPU_V850)
#include "n4rv850.h"
#elif defined(CPU_PPC)
#include "n4rppc.h"
#elif (defined(CPU_ARM)||defined(CPU_THUMB))
#include "n4rarm.h"
#elif defined(CPU_R3000) || defined(CPU_R4000)
#include "n4rmips.h"
#elif defined(CPU_M16C)
#include "n4rm16c.h"
#elif defined(CPU_M32R)
#include "n4rm32r.h"
#endif

#define sns_ctx_sub()       ((INEST)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))

/************************************/
/* Hook-routine definitions         */
/************************************/

#define THF_TSK     0x0001
#define THF_TSKS    0x0002
#define THF_SEM     0x0004
#define THF_FLG     0x0008
#define THF_MBX     0x0010
#define THF_MBF     0x0020
#define THF_POR     0x0040
#define THF_INT     0x0080
#define THF_MPL     0x0100
#define THF_MPF     0x0200
#define THF_TIM     0x0400
#define THF_SYS     0x0800
#define THF_TEX     0x1000
#define THF_MTX     0x2000
#define THF_DTQ     0x4000

#define THG_SYS     0x0001
#define THG_RET     0x0002
#define THG_DSP     0x0004

#ifndef HOOK

#define vsnd_deber(a,b)
#define visnd_deber(a,b)

#define vsnd_deb0(a,b)
#define vsnd_deb1(a,b,c)
#define vsnd_deb2(a,b,c,d)
#define vsnd_deb3(a,b,c,d,e)
#define vsnd_deb4(a,b,c,d,e,f)
#define vsnd_deb5(a,b,c,d,e,f,g)
#define vsnd_deb6(a,b,c,d,e,f,g,h)

#define vsnd_deb0e(a,b,c)
#define vsnd_deb1e(a,b,c,d)
#define vsnd_deb2e(a,b,c,d,e)
#define vsnd_deb3e(a,b,c,d,e,f)
#define vsnd_deb4e(a,b,c,d,e,f,g)
#define vsnd_deb5e(a,b,c,d,e,f,g,h)
#define vsnd_deb6e(a,b,c,d,e,f,g,h,i)

#define visnd_deb0e(a,b,c)
#define visnd_deb0(a,b)
#define visnd_deb1(a,b,c)
#define visnd_deb2(a,b,c,d)
#define visnd_deb3(a,b,c,d,e)
#define visnd_deb4(a,b,c,d,e,f)
#define visnd_deb5(a,b,c,d,e,f,g)
#define visnd_deb6(a,b,c,d,e,f,g,h)
#endif

/************************************/
/* control block definitions        */
/************************************/

/* OS Memory Header */

typedef struct t_mem
{   struct t_mem PFAR *next;
    SIZE size;
} T_MEM;

#if (defined(CPU_86)||defined(CPU_M16C))
typedef struct t_nmem
{   struct t_nmem SNEAR *next;
    UINT size;
} T_NMEM;
#else
#define T_NMEM      T_MEM
#endif

/* Task Exception Control Block */

typedef struct t_excb
{
    T_CTX SFAR *sp;
    TEXPTN pndptn;
    H stat;
    B ctxsv;
    const V4_T_DTEX *dtex;
} T_EXCB;


/* Task Control Block */

typedef struct t_tcb
{   T_CTX SFAR *sp;
    TSK_PRI pri;
    UB sts;
    TSK_ID nid;
    TSK_ID pid;
    B wup;
    B sus;
    TSK_ID ntm;
    TSK_ID ptm;
    DLYTIME tmo;
    TSK_ID SNEAR *que;
    T_CTX SFAR *stk;
    const V4_T_CTSK *ctsk;
    T_EXCB SNEAR *excb;
    RELTIM rcnt;
    OBJ_ID lock;
    B act;
    TSK_PRI bpri;
} T_TCB;

/* System Task Control Block */

typedef struct t_systcb
{   T_CTX SFAR *sp;
    TSK_PRI pri;
    UB sts;
    TSK_ID nid;
    TSK_ID pid;
    B wup;
    B sus;
    TSK_ID ntm;
    TSK_ID ptm;
    DLYTIME tmo;
    TSK_ID SNEAR *que;
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

/* Overrun Handler Control Block */

typedef struct t_ovr
{   FP ovrhdr;
    FP ovrclr;
    ID otskid;
} T_OVR;

/* Semaphore Control Block */

typedef struct t_sem
{
    const V4_T_CSEM *csem;
    H cnt;
    H max;
    TSK_ID que[2];
} T_SEM;

/* EventFlag Control Block */

typedef struct t_flg
{   const V4_T_CFLG *cflg;
    UINT ptn;
    B wmul;
    TSK_ID que[2];
} T_FLG;

/* Data Queue Control Block */

typedef struct t_dtq
{   const V4_T_CDTQ *cdtq;
    VP_INT top;
    VP_INT put;
    VP_INT get;
    VP_INT btm;
    UH size;
    UH cnt;
    TSK_ID rque;
    TSK_ID dmmy;
    TSK_ID sque[2];
} T_DTQ;

/* Mailbox Control Block */

typedef struct t_mbx
{   const V4_T_CMBX *cmbx;
    UB mpri;
    UB size;
    UB moff;
    TSK_ID que[2];
} T_MBX;

/* Mutex Control Block */

typedef struct t_mtx
{   const V4_T_CMTX *cmtx;
    ATR atr;
    OBJ_ID nid;
    TSK_ID occ;
    TSK_PRI ceil;
    TSK_ID que[2];
} T_MTX;

/* MessageBuffer Control Block */

typedef struct t_mbf
{   const V4_T_CMBF *cmbf;
    B PFAR *buf;
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
{   const V4_T_CPOR *cpor;
    UH maxcmsz;
    UH maxrmsz;
    TSK_ID atsk;
    TSK_ID dmmy;
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
    RDVNO PFAR *p_rdvno;
} T_APORCTX;

/* Variable-size MemoryPool Control Block */

typedef struct t_mpl
{   const V4_T_CMPL *cmpl;
    T_MEM PFAR *top;
    B PFAR *allad;
    SIZE allsz;
    TSK_ID que[2];
} T_MPL;

/* Fixed-size MemoryPool Control Block */

typedef struct t_mpf
{   const V4_T_CMPF *cmpf;
    B PFAR *top;
    B PFAR *allad;
    SIZE allsz;
    UH cnt;
    TSK_ID que[2];
} T_MPF;

/* ISR Control Block */

typedef struct t_isr_st
{   FP isr;
    VP_INT exinf;
    struct t_isr_st SNEAR *next;
    struct t_isr_st SNEAR **prev;
} T_ISR_ST;


typedef struct t_isr
{   FP isr;
    VP_INT exinf;
    struct t_isr SNEAR *next;
    struct t_isr SNEAR **prev;
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

#define S_CLR       0x04    /* 0x01 EventFlag clear specification */
#define S_ORW       0x01    /* 0x02 EventFlag OR wait */
#define S_TMO       0x02    /* 0x04 WAIT with timeout */

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

extern T_TCB SNEAR * SNEAR cdecl _pTCB[];
extern T_SEM SNEAR * SNEAR cdecl _pSEM[];
extern T_FLG SNEAR * SNEAR cdecl _pFLG[];
extern T_MBX SNEAR * SNEAR cdecl _pMBX[];
extern T_MBF SNEAR * SNEAR cdecl _pMBF[];
extern T_POR SNEAR * SNEAR cdecl _pPOR[];
extern T_MPL SNEAR * SNEAR cdecl _pMPL[];
extern T_MPF SNEAR * SNEAR cdecl _pMPF[];
extern T_CYC SNEAR * SNEAR cdecl _pCYC[];
extern T_ALM SNEAR * SNEAR cdecl _pALM[];
extern T_DTQ SNEAR * SNEAR cdecl _pDTQ[];
extern T_MTX SNEAR * SNEAR cdecl _pMTX[];
extern T_ISR SNEAR * SNEAR cdecl _pISR[];
extern T_SVC SNEAR * SNEAR cdecl _pSVC[];

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

extern int SNEAR * const cdecl pSYSMEM;
extern int PFAR * const cdecl pMPLMEM;
extern int SFAR * const cdecl pSTKMEM;

/* kernel data */

extern SYSTIME SNEAR cdecl SYSCK;
#if (SIZEOF_INT == 1)
extern UB SNEAR IMASK;
#elif (defined(SH3)||defined(SH3E)||defined(SH4)||defined(CPU_FR)||defined(CPU_PPC)||defined(CPU_ARM)||defined(CPU_THUMB)||defined(CPU_M32R)||defined(CPU_R3000)||defined(CPU_R4000))
extern UW SNEAR cdecl IMASK;
#else
extern UH SNEAR cdecl IMASK;
#endif
extern UH SNEAR cdecl IDLCNT;
extern T_SYSTCB SNEAR cdecl SYSTCB;
extern ER SNEAR cdecl SYSER;
extern volatile TSK_PRI SNEAR cdecl NOWPRI;
extern volatile B SNEAR cdecl DELAY;
extern volatile B SNEAR cdecl INEST;
extern volatile B SNEAR cdecl DDISP;
extern volatile TSK_ID SNEAR cdecl SDISP;
extern volatile B SNEAR cdecl TMREQ;
extern volatile TSK_ID SNEAR cdecl PREVTSK;
extern UB SNEAR cdecl TMQMS;
extern UB SNEAR cdecl CHQMS;
extern UB SNEAR cdecl AHQMS;
extern UINT SNEAR cdecl CTXPTN;
extern VP SNEAR cdecl CTXPTR;
extern UINT SNEAR cdecl WID;
extern B SFAR * SNEAR cdecl ISP;
extern T_NMEM SNEAR * SNEAR cdecl SYSTOP;
extern T_MEM PFAR * SNEAR cdecl MPLTOP;
extern T_MEM SFAR * SNEAR cdecl STKTOP;
#ifndef NOCFG3_H
extern TSK_ID SNEAR cdecl RDQ[];
extern TSK_ID SNEAR cdecl TMQ[];
extern UB SNEAR cdecl CHQ[];
extern UB SNEAR cdecl AHQ[];
#endif
extern const B _KERNEL_NNM;
extern BOOL _kernel_inROM(const void *);

#ifdef __cplusplus
}
#endif
#endif /* NOSYS4_H */
