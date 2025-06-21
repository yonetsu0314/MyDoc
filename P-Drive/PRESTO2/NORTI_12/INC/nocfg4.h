/******************************************************************************
* NORTi4 Configuration Header                                                 *
*                                                                             *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
* 07/Aug/2000  Supported H8 Series                                            *
* 23/Aug/2000  Crorrected task ID type                                        *
* 28/Sep/2000  Supported PPC Series                                       OK  *
* 23/Jan/2001  Supported PPC Interrupt Normal                             TA  *
* 26/Mar/2001  Supported FR30 Series                                      TA  *
* 23/Jul/2001  Supported XASS-V ARM series                                TA  *
* 29/Jul/2001  Supported MIPS Series(R4000/R3000)                         OK  *
* 04/Oct/2001  Supported M16C/60 series                                   TA  *
* 10/Oct/2001  Supported M32R Series by CC32R                             OK  *
* 29/Nov/2002  SDISP field size B->TSK_ID                                 KT  *
* 03/Apr/2003  For ARM (in the flexible executive mode.)                  TA  *
******************************************************************************/

#ifndef NOCFG4_H
#define NOCFG4_H

#include "nosys4.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

/* Constants for Configuration */

#ifndef TSKID_MAX
#define TSKID_MAX   8               /* 1 ~ 250 */
#endif
#ifndef SEMID_MAX
#define SEMID_MAX   8               /* 1 ~ 999 */
#endif
#ifndef FLGID_MAX
#define FLGID_MAX   8               /* 1 ~ 999 */
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
#define CYCNO_MAX   1               /* 1 ~ 250 */
#endif
#ifndef ALMNO_MAX
#define ALMNO_MAX   1               /* 1 ~ 250 */
#endif
#ifndef TPRI_MAX
#define TPRI_MAX    8               /* 1 ~ 31 */
#endif
#ifndef TMRQSZ
#define TMRQSZ      1               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef CYCQSZ
#define CYCQSZ      1               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef ALMQSZ
#define ALMQSZ      1               /* 1,2,4,8,16,32,64,128,256 */
#endif
#ifndef ISTKSZ
#define ISTKSZ      sizeof(T_CTX)*4
#endif
#ifndef TSTKSZ
#define TSTKSZ      sizeof(T_CTX)*4
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



#ifndef KNL_LEVEL
#if (defined(CPU_68K)||defined(CPU_H8S)||defined(CPU_H85)||defined(CPU_M16C))
#define KNL_LEVEL   5
#elif defined(CPU_SH)
#define KNL_LEVEL   14
#elif defined(CPU_FR)
#define KNL_LEVEL   18
#elif defined(CPU_H83)
#define KNL_LEVEL   2
#elif defined(CPU_R4000) || defined(CPU_R3000)
#define KNL_LEVEL   8
#elif defined(CPU_PPC)
#define KNL_LEVEL   -1
#elif defined(CPU_M32R)
#define KNL_LEVEL   1
#endif
#endif

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

const T_RVER DFAR * const cdecl norti_ver = &_NORTI_VER;

/* Status Register Value of Kernel */

#if defined(CPU_68K)
const UH KNL_SR = 0x2000|((KNL_LEVEL)<<8);
#elif defined(CPU_FR)
const UW KNL_SR = 0x00000010|((KNL_LEVEL)<<16);
#elif defined(CPU_H8S)
  #if INTM==0
  const UH KNL_CCR = 0x8000;
  #elif INTM==1
    #if KNL_LEVEL==1
    const UH KNL_CCR = 0x8000;
    #else
    const UH KNL_CCR = 0xc000;
    #endif
  #else
    const UH KNL_EXR = (KNL_LEVEL)<<8;
  #endif
#elif defined(CPU_H85)
const UH KNL_SR = (KNL_LEVEL)<<8;
#elif defined(CPU_SH)
#if (defined(SH3)||defined(SH3E)||defined(SH4))
const UW KNL_SR = 0x40000000|((KNL_LEVEL)<<4); /* MD=1,RB=0,BL=0 */
#else
const UH KNL_SR = (KNL_LEVEL)<<4;
#endif
#elif (defined(CPU_ARM)||defined(CPU_THUMB))
  #if defined(IRQ_Disable)
  const UW KNL_SR = 0x00000080;
  #else
  const UW KNL_SR = 0x00000000;
  #endif
  UW INT_PSR;
  UW USER_PSR;
#elif defined(CPU_H83)
  #if INTM==0
  const UH KNL_CCR = 0x8000;
  #else
    #if KNL_LEVEL==1
    const UH KNL_CCR = 0x8000;
    #else
    const UH KNL_CCR = 0xc000;
    #endif
  #endif
#elif defined(CPU_R4000) || defined(CPU_R3000)
const UW KNL_SR = KNL_LEVEL;
#elif defined(CPU_PPC)
  #if defined(INT_NOR)
  const UW KNL_SR = 0x00000010;
  #else
  const UW KNL_SR = (16-KNL_LEVEL);
  #endif
#elif defined(CPU_M16C)
  const UH KNL_FLG =  0x00C0|(KNL_LEVEL)<<12;
#elif defined(CPU_M32R)
  #if defined(INT_NOMX)
  const UW KNL_SR = 0x0000;
  #else
  const UW KNL_SR = KNL_LEVEL;
  #endif
#endif

/* Kernel Internal Data */

T_TCB SNEAR * SNEAR cdecl _pTCB[TSKID_MAX+1];
T_SEM SNEAR * SNEAR cdecl _pSEM[SEMID_MAX];
T_FLG SNEAR * SNEAR cdecl _pFLG[FLGID_MAX];
T_MBX SNEAR * SNEAR cdecl _pMBX[MBXID_MAX];
T_MBF SNEAR * SNEAR cdecl _pMBF[MBFID_MAX];
T_POR SNEAR * SNEAR cdecl _pPOR[PORID_MAX];
T_MPL SNEAR * SNEAR cdecl _pMPL[MPLID_MAX];
T_MPF SNEAR * SNEAR cdecl _pMPF[MPFID_MAX];
T_CYC SNEAR * SNEAR cdecl _pCYC[CYCNO_MAX];
T_ALM SNEAR * SNEAR cdecl _pALM[ALMNO_MAX];
T_DTQ SNEAR * SNEAR cdecl _pDTQ[DTQID_MAX];
T_MTX SNEAR * SNEAR cdecl _pMTX[MTXID_MAX];
T_ISR SNEAR * SNEAR cdecl _pISR[ISRID_MAX];
T_SVC SNEAR * SNEAR cdecl _pSVC[SVCFN_MAX];
FP SNEAR _kernel_dcall;
ID SNEAR _kernel_mbxfr;
RDVNO SNEAR _kernel_rdv_seqno;
T_ISR_ST SNEAR _kernel_ovrisr;
T_OVR SNEAR _kernel_ovr;
SYSTIME SNEAR cdecl _kernel_clk;
SYSTIME SNEAR cdecl SYSCK;
#if (SIZEOF_INT == 1)
UB SNEAR IMASK;
#elif (defined(SH3)||defined(SH3E)||defined(SH4)||defined(CPU_FR)||defined(CPU_PPC)||defined(CPU_ARM)||defined(CPU_THUMB)||defined(CPU_M32R)||defined(CPU_R3000)||defined(CPU_R4000))
UW SNEAR cdecl IMASK;
#else
UH SNEAR cdecl IMASK;
#endif
UH SNEAR cdecl IDLCNT;
T_SYSTCB SNEAR cdecl SYSTCB;
ER SNEAR cdecl SYSER;
volatile TSK_PRI SNEAR cdecl NOWPRI;
volatile B SNEAR cdecl DELAY;
volatile B SNEAR cdecl INEST;
volatile B SNEAR cdecl DDISP;
volatile TSK_ID SNEAR cdecl SDISP;
volatile B SNEAR cdecl TMREQ;
volatile TSK_ID SNEAR cdecl PREVTSK;
UB SNEAR cdecl TMQMS;
UB SNEAR cdecl CHQMS;
UB SNEAR cdecl AHQMS;
UINT SNEAR cdecl CTXPTN;
VP SNEAR cdecl CTXPTR;
UINT SNEAR cdecl WID;
B SFAR * SNEAR cdecl ISP;
T_NMEM SNEAR * SNEAR cdecl SYSTOP;
T_MEM PFAR * SNEAR cdecl MPLTOP;
T_MEM SFAR * SNEAR cdecl STKTOP;
TSK_ID SNEAR cdecl RDQ[TPRI_MAX+2];
TSK_ID SNEAR cdecl TMQ[TMRQSZ];
UB SNEAR cdecl CHQ[CYCQSZ];
UB SNEAR cdecl AHQ[ALMQSZ];
const B _KERNEL_NNM = 0;

#if ((SYSMSZ)/SIZEOF_INT!=0)
int SNEAR cdecl SYSMEM[(SYSMSZ)/SIZEOF_INT];
int SNEAR * const cdecl pSYSMEM = (int SNEAR * cdecl)SYSMEM;
#else
int SNEAR * const cdecl pSYSMEM = (int SNEAR * cdecl)NULL;
#endif

#if ((MPLMSZ)/SIZEOF_INT!=0)
int PFAR * const cdecl pMPLMEM = MPLMEM;
#else
int PFAR * const cdecl pMPLMEM = (int PFAR * cdecl)NULL;
#endif

#if ((STKMSZ)/SIZEOF_INT!=0)
int SFAR * const cdecl pSTKMEM = STKMEM;
#else
int SFAR * const cdecl pSTKMEM = (int SFAR * cdecl)NULL;
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
UH SNEAR cdecl STKHQQ = 0;
#else
UH SNEAR pascal STKHQQ = 0;
#endif
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif  /* NOCFG4_H */

