/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/nocfg4.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: nocfg4.h $
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
* NORTi4 Configuration Header                                                 *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
******************************************************************************/
#ifndef NOCFG4_H
#define NOCFG4_H

#include </itron4/nosys4.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

/* Constants for Configuration */

#ifndef TSKID_MAX
/*#define TSKID_MAX   8*/               /* 1 ~ 250 */
#define TSKID_MAX 16
#endif
#ifndef SEMID_MAX
/*#define SEMID_MAX   8*/               /* 1 ~ 999 */
#define SEMID_MAX 23
#endif
#ifndef FLGID_MAX
/*#define FLGID_MAX   8 */              /* 1 ~ 999 */
#define FLGID_MAX 23
#endif
#ifndef MBXID_MAX
#define MBXID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef MBFID_MAX
#define MBFID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef PORID_MAX
#define PORID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef MPLID_MAX
#define MPLID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef MPFID_MAX
#define MPFID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef DTQID_MAX
#define DTQID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef MTXID_MAX
#define MTXID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef ISRID_MAX
#define ISRID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef SVCFN_MAX
#define SVCFN_MAX   8               /* 1 ~ 50 */
#endif
#ifndef CYCNO_MAX
#define CYCNO_MAX   8               /* 1 ~ 250 */
#endif
#ifndef ALMNO_MAX
#define ALMNO_MAX   8               /* 1 ~ 250 */
#endif
#ifndef TPRI_MAX
/*#define TPRI_MAX    8*/               /* 1 ~ 31 */
#define TPRI_MAX  16
#endif
#ifndef TMRQSZ
#define TMRQSZ      256               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef CYCQSZ
#define CYCQSZ      256               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef ALMQSZ
#define ALMQSZ      256               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef ISTKSZ
/*#define ISTKSZ      sizeof(T_CTX)*4*/
#define ISTKSZ      0x2000
#endif
#ifndef TSTKSZ
/*#define TSTKSZ      sizeof(T_CTX)*4*/
#define TSTKSZ      0x2000
#endif
#ifndef SYSMSZ
#define SYSMSZ      0               /* 0: use stack area */
#endif
#ifndef MPLMSZ
#define MPLMSZ      0               /* 0: use stack area */
#endif
#ifndef STKMSZ
#define STKMSZ      0               /* 0: use default stack area */
#endif


#define KNL_LEVEL   5
/*
#ifndef KNL_LEVEL
#if (defined(CPU_68K)||defined(CPU_H8S)||defined(CPU_H85))
#define KNL_LEVEL   5
#elif defined(CPU_SH)
#define KNL_LEVEL   14
#elif (defined(CPU_ARM)||defined(CPU_THUMB))
#define KNL_LEVEL   2
#elif defined(CPU_H83)
#define KNL_LEVEL   2
#endif
#endif
*/

/* Configuration Information */

const V4_T_RCFG cdecl CFG =
{   TSKID_MAX,
    SEMID_MAX,
    FLGID_MAX,
    MBXID_MAX,
    MBFID_MAX,
    PORID_MAX,
    MPLID_MAX,
    MPFID_MAX,
    CYCNO_MAX,
    ALMNO_MAX,
    TPRI_MAX,
    TMRQSZ,
    CYCQSZ,
    ALMQSZ,
    ISTKSZ,
    TSTKSZ,
    SYSMSZ,
    MPLMSZ,
    STKMSZ,
    DTQID_MAX,
    MTXID_MAX,
    ISRID_MAX,
    SVCFN_MAX
};

const T_RVER FAR * const cdecl norti_ver = &_NORTI_VER;

/* Status Register Value of Kernel */

#if defined(CPU_68K)
const UH KNL_SR = 0x2000|((KNL_LEVEL)<<8);
#elif defined(CPU_H8S)
const UH KNL_EXR = (KNL_LEVEL)<<8;
#elif defined(CPU_H85)
const UH KNL_SR = (KNL_LEVEL)<<8;
#elif defined(CPU_SH)
#if (defined(SH3)||defined(SH3E)||defined(SH4))
const UW KNL_SR = 0x40000000|((KNL_LEVEL)<<4); /* MD=1,RB=0,BL=0 */
#else
const UH KNL_SR = (KNL_LEVEL)<<4;
#endif
#elif (defined(CPU_ARM)||defined(CPU_THUMB))
const UH KNL_SR = (KNL_LEVEL)<<6;
#elif defined(CPU_H83)
#if KNL_LEVEL==1
const UH KNL_CCR = 0x8000;
#else
const UH KNL_CCR = 0xc000;
#endif
#endif

/* Kernel Internal Data */

T_TCB NEAR * NEAR cdecl _pTCB[TSKID_MAX+1];
T_SEM NEAR * NEAR cdecl _pSEM[SEMID_MAX];
T_FLG NEAR * NEAR cdecl _pFLG[FLGID_MAX];
T_MBX NEAR * NEAR cdecl _pMBX[MBXID_MAX];
T_MBF NEAR * NEAR cdecl _pMBF[MBFID_MAX];
T_POR NEAR * NEAR cdecl _pPOR[PORID_MAX];
T_MPL NEAR * NEAR cdecl _pMPL[MPLID_MAX];
T_MPF NEAR * NEAR cdecl _pMPF[MPFID_MAX];
T_CYC NEAR * NEAR cdecl _pCYC[CYCNO_MAX];
T_ALM NEAR * NEAR cdecl _pALM[ALMNO_MAX];
T_DTQ NEAR * NEAR cdecl _pDTQ[DTQID_MAX];
T_MTX NEAR * NEAR cdecl _pMTX[MTXID_MAX];
T_ISR NEAR * NEAR cdecl _pISR[ISRID_MAX];
T_SVC NEAR * NEAR cdecl _pSVC[SVCFN_MAX];
FP _kernel_dcall;
ID _kernel_mbxfr;
RDVNO _kernel_rdv_seqno;
T_ISR_ST _kernel_ovrisr;
SYSTIME NEAR cdecl _kernel_clk;
SYSTIME NEAR cdecl SYSCK;
#if (SIZEOF_INT == 1)
UB NEAR IMASK;
#elif (defined(SH3)||defined(SH3E)||defined(SH4))
UW NEAR cdecl IMASK;
#else
UH NEAR cdecl IMASK;
#endif
UH NEAR cdecl IDLCNT;
T_SYSTCB NEAR cdecl SYSTCB;
ER NEAR cdecl SYSER;
volatile TSK_PRI NEAR cdecl NOWPRI;
volatile B NEAR cdecl DELAY;
volatile B NEAR cdecl INEST;
volatile B NEAR cdecl DDISP;
volatile B NEAR cdecl SDISP;
volatile B NEAR cdecl TMREQ;
volatile B NEAR cdecl PREVTSK;
UB NEAR cdecl TMQMS;
UB NEAR cdecl CHQMS;
UB NEAR cdecl AHQMS;
UINT NEAR cdecl CTXPTN;
VP NEAR cdecl CTXPTR;
UINT NEAR cdecl WID;
B FAR * NEAR cdecl ISP;
T_NMEM NEAR * NEAR cdecl SYSTOP;
T_MEM * NEAR cdecl MPLTOP;
T_MEM * NEAR cdecl STKTOP;
TSK_ID NEAR cdecl RDQ[TPRI_MAX+2];
TSK_ID NEAR cdecl TMQ[TMRQSZ];
UB NEAR cdecl CHQ[CYCQSZ];
UB NEAR cdecl AHQ[ALMQSZ];
B NEAR _KERNEL_NNM = 0;

#if ((SYSMSZ)/SIZEOF_INT!=0)
int NEAR cdecl SYSMEM[(SYSMSZ)/SIZEOF_INT];
int NEAR * const cdecl pSYSMEM = SYSMEM;
#else
int NEAR * const cdecl pSYSMEM = NULL;
#endif

#if ((MPLMSZ)/SIZEOF_INT!=0)
int * const cdecl pMPLMEM = MPLMEM;
#else
int * const cdecl pMPLMEM = NULL;
#endif

#if ((STKMSZ)/SIZEOF_INT!=0)
int * const cdecl pSTKMEM = STKMEM;
#else
int * const cdecl pSTKMEM = NULL;
#endif

/* ROMable NORTi4/86 only */

#if (defined(CPU_86)&&defined(ROM))
#if defined(__TURBOC__)
#if (defined(__SMALL__)||defined(__MEDIUM__))
#define sysini  sysini_rom
#endif
#elif defined(M_I86) /* MS-C */
#define sysini  sysini_rom
#if (_MSC_VER>=700)
UH NEAR cdecl STKHQQ = 0;
#else
UH NEAR pascal STKHQQ = 0;
#endif
#endif
#endif

#ifdef __cplusplus
};
#endif

#endif  /* NOCFG4_H */

