/******************************************************************************
* NORTi4 Kernel                                                               *
*                                                                             *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000   Ver. 1.00 NORTi Version 4                                     *
* 16/Jun/2000   Ver. 1.10 Evaluation release                                  *
* 25/Aug/2000   Ver. 1.20 Release (only H8 seriese)                           *
*                reinforced hook routines                                     *
*                corrected excb initializing                                  *
*                corrected cre_cyc (cyclic handler number)                    *
* 03/Oct/2000   Ver. 4.02.xx Release                                          *
*                corrected tloc_mtx (return point)                            *
*                corrected chg_ims, unl_cpu, ena_dsp (dispatch disable state) *
*                corrected fsnd_dtq                                           *
* 28/Oct/2000   Ver. 4.02.00 repaired                                         *
*                corrected hook routines                                      *
* 29/Oct/2000   Ver. 4.03.00 Release                                          *
*                タスクハンドラでコンテキスト処理方法を変更                OK *
*                起動要求キューイング中の ext_tsk 処理変更                 OK *
*                def_ovr で定義->解除->再定義の場合の不具合修正            OK *
* 06/Nov/2000   Ver. 4.03.00 Release                                          *
*                一部コンパイラのオプティマイズに対応                      TA *
*                corrected ref_alm error code                              TA *
* 13/Sep/2001   Ver. 4.04.00 Release                                          *
*                corrected get_mpl, get_pri hook                           TA *
*                corrected cre_mbf (ユーザー指定バッファ使用時)            TA *
*                corrected chg_pri, get_pri (パラメータチェック)           TA *
*                corrected acre_isr, acre_dtq (E_ID -> E_NOID)             TA *
*                corrected del_flg, del_por (システムメモリの返却サイズ)   TA *
*                corrected sus_tsk (自タスク指定時)                        TA *
*                corrected del_dtq (送信待ちタスクが存在する場合)          TA *
*                corrected ref_mbf (最大長が 1の場合)                      TA *
*                corrected loc_mtx (優先度継承プロトコルの場合)            TA *
*                corrected tcal_por (待ちタスクが存在する場合)             TA *
*                corrected def_cyc (TCY_OFF属性を指定した場合)             TA *
* 13/Dec/2001   Ver. 4.05.00 Release                                          *
*                M16C/60シリーズ用カーネルと統合                           TA *
*                corrected exd_tsk (ユーザー定義スタックの使用時)          TA *
* 15/Jan/2002    corrected function name (M16C以外の場合)                  TA *
* 21/May/2002   Ver. 4.06.00 Release                                          *
*                動的メモリ管理の不具合を修正                              TA *
*                タスクIDの型を統一                                        TA *
*                corrected sig_ovr（アイドル中に処理した場合）             TA *
*                corrected twai_flg（引数の型）                            TA *
*                corrected cre_cyc, sta_cyc, stp_alm error code            TA *
*                corrected cre_cyc（周期ハンドラ起動フェーズが0の場合）    TA *
* 03/Oct/2002    EWARM 対応                                                TA *
* 22/May/2003   Ver. 4.07.00 Release                                          *
*                動的メモリの管理方法を一新（メモリ効率の向上）            TA *
*                rel_mpl（チェックを強化）                                 TA *
*                corrected ref_mbx（メッセージ有りの待ちタスクID）         TA *
*                corrected frsm_tsk（サスペンドでないタスクへのfrsm_tsk）  TA *
*                corrected del_tsk（ユーザー定義スタックの使用時）         TA *
*                corrected ref_tsk（タイムアウトまでの時間が不定値）       TA *
*                corrected ref_tsk, ref_tst（const -> static const）       TA *
*                corrected chg_pri（自タスクへの発行の場合）               TA *
*                corrected ter_tsk（ミューテックスロック中の場合）         TA *
* 25/Jun/2003    corrected def_ovr（正常終了以外のエラーコード）           TA *
*                RVCT 対応                                                 TA *
******************************************************************************/
#define NOKNL4_C

#include "kernel.h"
#include "nosys4.h"

#ifdef ALL

#define OS_CORE
#define V4_CRE_TSK
#define V3_CRE_TSK
#define V4_CAN_TSK
#define V4_DEL_TSK
#define V4_STA_TSK
#define V4_ACT_TSK
#define V4_EXT_TSK
#define V4_EXD_TSK
#define V4_TER_TSK
#define V4_DIS_DSP
#define V3_DIS_DSP
#define V4_ENA_DSP
#define V3_ENA_DSP
#define V4_CHG_PRI
#define V4_GET_PRI
#define V4_ROT_RDQ
#define V4_REL_WAI
#define V4_GET_TID
#define V3_VGET_TID
#define V4_REF_TSK
#define V3_REF_TSK
#define V4_REF_TST
#define V4_SUS_TSK
#define V4_RSM_TSK
#define V4_TSLP_TSK
#define V4_WUP_TSK
#define V4_CAN_WUP
#define V3_CAN_WUP
#define V3_VCAN_WUP
#define V4_CRE_SEM
#define V3_CRE_SEM
#define V4_DEL_SEM
#define V4_SIG_SEM
#define V4_TWAI_SEM
#define V4_REF_SEM
#define V3_REF_SEM
#define V3_CRE_FLG
#define V4_CRE_FLG
#define V4_DEL_FLG
#define V4_SET_FLG
#define V4_CLR_FLG
#define V4_TWAI_FLG
#define V4_REF_FLG
#define V3_REF_FLG
#define V4_CRE_MBX
#define V3_CRE_MBX
#define V4_DEL_MBX
#define V4_SND_MBX
#define V4_TRCV_MBX
#define V4_REF_MBX
#define V3_REF_MBX
#define V4_CRE_MBF
#define V3_CRE_MBF
#define V4_DEL_MBF
#define V4_TSND_MBF
#define V4_TRCV_MBF
#define V3_TRCV_MBF
#define V4_REF_MBF
#define V3_REF_MBF
#define V4_CRE_POR
#define V3_CRE_POR
#define V4_DEL_POR
#define V4_TCAL_POR
#define V3_TCAL_POR
#define V4_TACP_POR
#define V3_TACP_POR
#define V4_FWD_POR
#define V4_RPL_RDV
#define V4_REF_POR
#define V3_REF_POR
#define V4_REF_RDV
#define V4_LOC_CPU
#define V4_UNL_CPU
#define V3_LOC_CPU
#define V3_UNL_CPU
#define V4_SNS_CTX
#define V4_SNS_LOC
#define V4_SNS_DSP
#define V4_SNS_DPN
#define V3_CHG_IMS
#define V3_REF_IMS
#define V4_CRE_MPL
#define V3_CRE_MPL
#define V4_DEL_MPL
#define V4_TGET_MPL
#define V4_REL_MPL
#define V4_REF_MPL
#define V3_REF_MPL
#define V4_CRE_MPF
#define V4_DEL_MPF
#define V4_TGET_MPF
#define V4_REL_MPF
#define V4_REF_MPF
#define V3_CRE_MPF
#define V3_REF_MPF
#define V4_SET_TIM
#define V4_GET_TIM
#define V3_SET_TIM
#define V3_GET_TIM
#define V4_DLY_TSK
#define V3_DEF_CYC
#define V3_ACT_CYC
#define V3_REF_CYC
#define V4_CRE_CYC
#define V4_DEL_CYC
#define V4_STA_CYC
#define V4_STP_CYC
#define V4_REF_CYC
#define V4_CRE_ALM
#define V4_DEL_ALM
#define V4_STA_ALM
#define V4_STP_ALM
#define V4_REF_ALM
#define V3_DEF_ALM
#define V3_REF_ALM
#define V4_DEF_OVR
#define V4_STA_OVR
#define V4_STP_OVR
#define V4_REF_OVR
#define V4_CRE_ISR
#define V4_DEL_ISR
#define V4_REF_ISR
#define V3_RET_TMR
#define V3_GET_VER
#define V4_REF_VER
#define V4_REF_SYS
#define V4_REF_CFG
#define V3_REF_CFG
#define V4_DEF_TEX
#define V4_RAS_TEX
#define V4_DIS_TEX
#define V4_ENA_TEX
#define V4_SNS_TEX
#define V4_REF_TEX
#define V4_CRE_DTQ
#define V4_DEL_DTQ
#define V4_SND_DTQ
#define V4_FSND_DTQ
#define V4_TRCV_DTQ
#define V4_REF_DTQ
#define V4_CRE_MTX
#define V4_TLOC_MTX
#define V4_UNL_MTX
#define V4_DEL_MTX
#define V4_REF_MTX
#define V4_MUTEX
#define SIG_OVR
#define V4_DEF_SVC
#define V4_CAL_SVC
#define OS_inSYSMEM
#endif

#ifndef ERC
#define ERC         1           /* error check (0:No, 1:Yes) */
#endif

#ifndef CLKBITS
#define CLKBITS     48
#endif

/* kernel C function prototype */

BOOL FNEAR msg_2_mbf(T_MBF SNEAR *, VP, unsigned);
unsigned FNEAR mbf_2_msg(T_MBF SNEAR *, VP);
unsigned FNEAR mbf_smsgcnt(T_MBF SNEAR *);
void FNEAR chk_tmq(void);
void FNEAR chk_cyc(void);
void FNEAR chk_alm(void);
void FNEAR sig_ovr(void);
void FNEAR tim_add(const SYSTIME *, SYSTIME SNEAR *, T_CYC SNEAR *);
void FNEAR tim_dif(SYSTIME SNEAR *, SYSTIME SNEAR *, SYSTIME DFAR *);
void FNEAR del_thq(T_CYC SNEAR * SNEAR *, T_CYC SNEAR *, ID);
void FNEAR add_thq(T_CYC SNEAR * SNEAR *, T_CYC SNEAR *, ID);
void FNEAR del_tmq(T_TCB SNEAR *, ID);
void FNEAR add_tmq(TMO, T_TCB SNEAR *, ID);
void FNEAR del_que(T_TCB SNEAR *, ID);
void FNEAR add_que(TSK_ID SNEAR *, T_TCB SNEAR *, ID);
void FNEAR chg_que(TSK_ID SNEAR *, T_TCB SNEAR *, ID);
void FNEAR chg_rdq(T_TCB SNEAR *, ID);
#ifndef del_que_ret_ims_
#define del_que_ret_ims_(a,b,c)     del_que_ret_ims(b,c)
#endif
ER FNEAR del_que_ret_ims_(long, T_TCB SNEAR *, ID);
#ifndef add_rdq_dispatch_
#define add_rdq_dispatch_(a,b,c)    add_rdq_dispatch(b,c)
#endif
ER FNEAR add_rdq_dispatch_(long, T_TCB SNEAR *, ID);
#ifndef chg_rdq_dispatch_
#define chg_rdq_dispatch_(a,b,c)    chg_rdq_dispatch(b,c)
#endif
ER FNEAR chg_rdq_dispatch_(long, T_TCB SNEAR *, ID);
#ifndef del_que_dispatch1_
#define del_que_dispatch1_(a,b,c)   del_que_dispatch1(b,c)
#endif
ER FNEAR del_que_dispatch1_(long, T_TCB SNEAR *, ID);
#ifndef chg_que_dispatch1_
#define chg_que_dispatch1_(a,b,c,d) chg_que_dispatch1(b,c,d)
#endif
ER FNEAR chg_que_dispatch1_(long, TSK_ID SNEAR *, T_TCB SNEAR *, ID);

#if defined(CPU_86)
 ER FNEAR osmem_init(T_MEM PFAR *, SIZE);
 void PFAR * FNEAR blk_alloc(T_MEM PFAR * SNEAR *, SIZE);
 ER FNEAR blk_free(T_MEM PFAR * SNEAR *, void PFAR *, SIZE);
 ER FNEAR osnmem_init(T_NMEM SNEAR *, SIZE);
 void SNEAR * FNEAR blk_nalloc(T_NMEM SNEAR * SNEAR *, SIZE);
 ER FNEAR blk_nfree(T_NMEM SNEAR * SNEAR *, void SNEAR *, SIZE);
 #define ossmem_init(m,s)    osmem_init(m,s)
 #define ospmem_init(m,s)    osmem_init(m,s)
 #define osymem_init(m,s)    osnmem_init(m,s)
 #define blk_salloc(m,s)     blk_alloc(m,s)
 #define blk_palloc(m,s)     blk_alloc(m,s)
 #define blk_yalloc(m,s)     blk_nalloc(m,s)
 #define blk_sfree(p,m,s)    blk_free(p,m,s)
 #define blk_pfree(p,m,s)    blk_free(p,m,s)
 #define blk_yfree(p,m,s)    blk_nfree(p,m,s)
#elif defined(CPU_M16C)
 ER FNEAR osmem_init(T_MEM PFAR *, SIZE);
 void PFAR * FNEAR blk_alloc(T_MEM PFAR * SNEAR *, SIZE);
 ER FNEAR blk_free(T_MEM PFAR * SNEAR *, void PFAR *, SIZE);
 ER FNEAR osnmem_init(T_NMEM SNEAR *, SIZE);
 void SNEAR * FNEAR blk_nalloc(T_NMEM SNEAR * SNEAR *, SIZE);
 ER FNEAR blk_nfree(T_NMEM SNEAR * SNEAR *, void SNEAR *, SIZE);
 #define ossmem_init(m,s)    osnmem_init(m,s)
 #define ospmem_init(m,s)    osmem_init(m,s)
 #define osymem_init(m,s)    osnmem_init(m,s)
 #define blk_salloc(m,s)     blk_nalloc(m,s)
 #define blk_palloc(m,s)     blk_alloc(m,s)
 #define blk_yalloc(m,s)     blk_nalloc(m,s)
 #define blk_sfree(p,m,s)    blk_nfree(p,m,s)
 #define blk_pfree(p,m,s)    blk_free(p,m,s)
 #define blk_yfree(p,m,s)    blk_nfree(p,m,s)
#else
 ER FNEAR osmem_init(T_MEM PFAR *, SIZE);
 void PFAR * FNEAR blk_alloc(T_MEM PFAR * SNEAR *, SIZE);
 ER FNEAR blk_free(T_MEM PFAR * SNEAR *, void PFAR *, SIZE);
 #define ossmem_init(m,s)    osmem_init(m,s)
 #define ospmem_init(m,s)    osmem_init(m,s)
 #define osymem_init(m,s)    osmem_init(m,s)
 #define blk_salloc(m,s)     blk_alloc(m,s)
 #define blk_palloc(m,s)     blk_alloc(m,s)
 #define blk_yalloc(m,s)     blk_alloc(m,s)
 #define blk_sfree(p,m,s)    blk_free(p,m,s)
 #define blk_pfree(p,m,s)    blk_free(p,m,s)
 #define blk_yfree(p,m,s)    blk_free(p,m,s)
#endif

SIZE FNEAR mpl_size(T_MEM PFAR *, SIZE PFAR *);

void FNEAR osmem_cpy(DFAR B *, DFAR B *, unsigned);
void SFAR * FNEAR stk_alloc(SIZE);
ER FNEAR stk_free(void SFAR *, SIZE);
void PFAR * FNEAR mpl_alloc(SIZE);
ER FNEAR mpl_free(void PFAR *, SIZE);
void SNEAR * FNEAR sys_alloc(SIZE);
ER FNEAR sys_free(void SNEAR *, SIZE);

/* kernel ASM function prototype */

#ifndef return_dispatch
#define return_dispatch(a)  return dispatch()
#endif
#ifndef return_dispatch1
#define return_dispatch1(a) return dispatch1()
#endif
#ifndef return_dispatch2
#define return_dispatch2(a) return dispatch2()
#endif
#ifndef dispatch
ER FNEAR dispatch(void);     /* dispatch with INEST/IMASK check */
ER FNEAR dispatch1(void);    /* dispatch without INEST/IMASK check */
ER FNEAR dispatch2(void);    /* dispatch for syssta, chg_ims */
void FNEAR dispatch3(void);  /* dispatch for ext_tsk, exd_tsk */
#endif
#ifndef dis_ims
void FNEAR dis_ims(void);    /* set mask at entry of system call */
#endif
#ifndef return_ret_ims
#define return_ret_ims(a)   return ret_ims()
#ifndef ret_ims
ER FNEAR ret_ims(void);      /* recover mask at exit of system call */
#endif
#endif
#ifndef ret_ims2_
#define ret_ims2_(a)        ret_ims2()
#endif
#ifndef return_ret_ims2
#define return_ret_ims2(a,b)    ret_ims2();return(b);
#endif
#ifndef ret_ims2
void FNEAR ret_ims2(void);
#endif
#ifndef ope_ims
void FNEAR ope_ims(void);
#endif
#ifndef return_ext_tsk
#define return_ext_tsk(a,b,c,d)  a_ext_tsk(a,b,c,d)
void FNEAR a_ext_tsk(T_CTX SNEAR *ctx, VP_INT stacd, T_TCB SNEAR *tcb, ID tskid);
#endif

/* kernel ASM interface variable */

#if defined(CPU_86)
extern int SNEAR cdecl IDS;                  /* initial DS           */
extern int SNEAR cdecl IFLAG;                /* initial FLAG         */
#endif
extern RDVNO SNEAR _kernel_rdv_seqno;
extern V4_SYSTIM SNEAR cdecl _kernel_clk;
extern FP _kernel_dcall;

extern T_ISR_ST SNEAR _kernel_ovrisr;
extern T_OVR SNEAR _kernel_ovr;

extern ID SNEAR _kernel_mbxfr;
extern const UB _KERNEL_pisrposi;
extern const UB _KERNEL_hdrlen;
extern void _kernel_interr(void);
ER _kernel_check_cisr(const V4_T_CISR *pk_cisr);
void _kernel_set_inthdr(FP hdr, const V4_T_CISR *pk_cisr);
FP _kernel_get_inthdr(const V4_T_CISR *pk_cisr);


/* kernel internal function for v4.0 */
void reset_occ_cpri(ID);
void unlock_allmtx(ID);
BOOL wup_tsk_mtx(T_MTX SNEAR *,ID);
BOOL push_msg2mbf(ID);
BOOL rel_mplchk(ID);
void v4_ent_exp(BOOL, T_TCB SNEAR *, T_EXCB SNEAR *);

#define get_wid()   tcb->sp->wid
#define set_wid(i)  WID = (UINT)(i)

#ifdef OS_CORE
/************************************************/
/* System Initialize                            */
/************************************************/

ER FNEAR c_sysini(B SFAR *stktop, B SFAR *sp)
{
    ER ercd;
    int i;

    dis_ims();
    c_sysini_sub();

    STKTOP = (T_MEM SFAR *)stktop;
    ercd = ossmem_init((T_NMEM SNEAR *)STKTOP, (SIZE)(sp - stktop));
    if (ercd != E_OK)
        return ercd;
    if ((MPLTOP = (T_MEM PFAR *)pMPLMEM) != NULL)
    {   ercd = ospmem_init(MPLTOP, CFG.mplmsz);
        if (ercd != E_OK)
            return ercd;
    }
    if ((SYSTOP = (T_NMEM SNEAR *)pSYSMEM) != NULL)
    {   ercd = osymem_init(SYSTOP, CFG.sysmsz);
        if (ercd != E_OK)
            return ercd;
    }
  #if (CLKBITS>32)
    _kernel_clk.utime = SYSCK.utime = 0;
  #endif
    _kernel_clk.ltime = SYSCK.ltime = 0;
    IDLCNT = 0;
    NOWPRI = 0;
    SYSER = E_OK;
    DELAY = FALSE;
    INEST = 0;
    SDISP = 0;
    TMREQ = 0;
    PREVTSK = 0;
    _kernel_rdv_seqno = 0;
    _kernel_dcall = NULL;
    _kernel_ovrisr.prev = NULL;
    for (i = 1; i <= CFG.tskid_max+1; i++) pTCB[i] = NULL;
    for (i = 1; i <= CFG.semid_max; i++) pSEM[i] = NULL;
    for (i = 1; i <= CFG.flgid_max; i++) pFLG[i] = NULL;
    for (i = 1; i <= CFG.mbxid_max; i++) pMBX[i] = NULL;
    for (i = 1; i <= CFG.mbfid_max; i++) pMBF[i] = NULL;
    for (i = 1; i <= CFG.porid_max; i++) pPOR[i] = NULL;
    for (i = 1; i <= CFG.mplid_max; i++) pMPL[i] = NULL;
    for (i = 1; i <= CFG.mpfid_max; i++) pMPF[i] = NULL;
    for (i = 1; i <= CFG.dtqid_max; i++) pDTQ[i] = NULL;
    for (i = 1; i <= CFG.cycno_max; i++) pCYC[i] = NULL;
    for (i = 1; i <= CFG.almno_max; i++) pALM[i] = NULL;
    for (i = 1; i <= CFG.mtxid_max; i++) pMTX[i] = NULL;
    for (i = 1; i <= CFG.isrid_max; i++) pISR[i] = NULL;
    for (i = 1; i <= CFG.svcfn_max; i++) pSVC[i] = NULL;
    for (i = 1; i <= CFG.tpri_max+1; i++) RDQ[i] = 0;
    for (i = 0; i < CFG.tmrqsz;  i++) TMQ[i] = 0;
    for (i = 0; i < CFG.cycqsz;  i++) CHQ[i] = 0;
    for (i = 0; i < CFG.almqsz;  i++) AHQ[i] = 0;
    DDISP = 1;
    TMQMS = (UB)(CFG.tmrqsz - 1);
    CHQMS = (UB)(CFG.cycqsz - 1);
    AHQMS = (UB)(CFG.almqsz - 1);
    i = CFG.tskid_max+1;
    _kernel_mbxfr = CFG.mbxid_max;
    pTCB[i] = (T_TCB SNEAR *)&SYSTCB;
    RDQ[0] = (TSK_ID)i;
    RDQ[CFG.tpri_max+1] = (TSK_ID)i;

    return E_OK;
}

#endif

#ifdef OS_CORE
/************************************************/
/* System Start                                 */
/************************************************/

ER syssta(void)
{
    int cnt;

    if (SYSER)
        return SYSER;
    DDISP = 0;
    syssta_sub();

    for (;;)
    {
        SYSTCB.pri = (B)(CFG.tpri_max + 1);
        if (DDISP)
            SDISP = PREVTSK;
        else
            SDISP = 0;
        dispatch2();

        if (_kernel_dcall != NULL)
            (*_kernel_dcall)();

        /* idle task process */

        for (cnt = 0xffff;;)
        {   if (TMREQ)
                break;
            if (cnt != 0)
                cnt--;
        }
        IDLCNT = (UH)(0xffff - cnt);

        /* interval timer process */

        dis_ims();
        do
        {
          #if (CLKBITS>32)
            if (++SYSCK.ltime == 0)
                ++SYSCK.utime;
          #else
            ++SYSCK.ltime;
          #endif
            chk_tmq();
            chk_cyc();
            chk_alm();
        } while (--TMREQ != 0);
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Timer Queue Management                       */
/************************************************/

void FNEAR chk_tmq(void)
{
    T_TCB SNEAR *tcb;
    T_MTX SNEAR *mtx;
    TSK_ID SNEAR *tmq;
    TSK_ID tskid, end;
    INT sts;
    TSK_PRI pri;
    DLYTIME ltime = (DLYTIME)SYSCK.ltime;

    tmq = &TMQ[(unsigned)ltime & TMQMS];

    for (;;)
    {   tskid = *tmq;
        if (tskid == 0)
            return;
        tcb = pTCB[tskid];
        end = tcb->ptm;
        for (;;)
        {   if (tcb->tmo == ltime)
            {   del_tmq(tcb, tskid);
                sts = tcb->sts;
                tcb->sts = S_RDY;
                if ((sts & STS_MSK0) == S_DLY)
                    tcb->sp->ercd = E_OK;
                else
                {   tcb->sp->ercd = E_TMOUT;
                    if (sts >= S_FLG)
                    {   del_que(tcb, tskid);
                        if ((sts & STS_MSK1) == S_MTX)
                        {   mtx = pMTX[get_wid()];
                            if ((mtx->atr == TA_INHERIT) &&
                                (mtx->ceil >= tcb->pri))
                                reset_occ_cpri(mtx->occ);
                        }
                        else if ((sts & STS_MSK1) == S_SMB)
                            push_msg2mbf(get_wid());
                        else if ((sts & STS_MSK0) == S_MPL)
                            rel_mplchk(get_wid());
                    }
                }
                if (tcb->sus == 0)
                {   pri = tcb->pri;
                    if (pri < NOWPRI)
                        DELAY = 1;
                    add_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
                }
                ope_ims();
                break;
            }
            if (tskid == end)
                return;
            tskid = tcb->ntm;
            tcb = pTCB[tskid];
        }
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Cyclic Handler Management                    */
/************************************************/

void FNEAR chk_cyc(void)
{
    T_CYC SNEAR *cyc;
    UB SNEAR *chq;
    HNO cycno, end;
    void (FFAR *cychdr)(VP_INT) ;
    DLYTIME ltime = (DLYTIME)SYSCK.ltime;
  #if (CLKBITS>32)
    H utime = SYSCK.utime;
  #endif

    chq = &CHQ[(unsigned)ltime & CHQMS];

    for (;;)
    {   cycno = *chq;
        if (cycno == 0)
            return;
        cyc = pCYC[cycno];
        end = cyc->ptm;
        for (;;)
        {   if ((ltime == (DLYTIME)cyc->ltime)
          #if (CLKBITS>32)
             && (utime == cyc->utime)
          #endif
               )
            {   del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
                cyc->ltime = cyc->cyctim.ltime + ltime;
              #if (CLKBITS>32)
                cyc->utime = cyc->cyctim.utime + utime;
                if (cyc->ltime < cyc->cyctim.ltime)
                    cyc->utime++;
              #endif
                add_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
                ret_ims2();
                if (cyc->cycact & TCY_ON)
                {   cychdr = (void (FFAR *)(VP_INT))cyc->cychdr;
                    cychdr(cyc->exinf);
                }
                dis_ims();
                break;
            }
            if (cycno == end)
                return;
            cycno = cyc->ntm;
            cyc = pCYC[cycno];
        }
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Alarm Handler Management                     */
/************************************************/

void FNEAR chk_alm(void)
{
    T_ALM SNEAR *alm;
    UB SNEAR *ahq;
    HNO almno, end;
    void (FFAR *almhdr)(VP_INT) ;
    DLYTIME ltime = (DLYTIME)SYSCK.ltime;
  #if (CLKBITS>32)
    H utime = SYSCK.utime;
  #endif

    ahq = &AHQ[(unsigned)ltime & AHQMS];

    for (;(almno = *ahq) != 0;)
    {   alm = pALM[almno];
        end = alm->ptm;
        for (;;)
        {   if ((ltime == (DLYTIME)alm->ltime)
          #if (CLKBITS>32)
             && (utime == alm->utime)
          #endif
               )
            {   del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almno);
                almhdr = (void (FFAR *)(VP_INT))alm->almhdr;
                alm->ntm = 0;
                alm->ptm = 0;
                ret_ims2();
                almhdr(alm->exinf);
                dis_ims();
                break;
            }
            if (almno == end)
                return;
            almno = alm->ntm;
            alm = pALM[almno];
        }
    }
}

#endif
#ifdef V4_CRE_TSK
/************************************************/
/* Create Task                                  */
/************************************************/

ER v4_cre_tsk(ID tskid, const V4_T_CTSK *pk_ctsk)
{
    T_TCB SNEAR *tcb;
    B SFAR *sp;
    unsigned stksz;
    int atid = (tskid == ID_AUTO);
    UINT size, coff;
    ER ercd;
  #if ERC
    int ptn;
    int *p;

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else if (pk_ctsk->itskpri > CFG.tpri_max)
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_CRE_TSK, (long)tskid, (long)pk_ctsk);
  #endif
    if (atid)
    {   for (tskid = CFG.tskid_max;;)
        {   if (pTCB[tskid] == NULL)
            {   ercd = tskid;
                break;
            }
            if (--tskid == 0)
            {   return_ret_ims2(THF_TSK, E_NOID);
            }
        }
    }
    else
    {   if (pTCB[tskid] != NULL)
        {   return_ret_ims2(THF_TSK, E_OBJ);
        }
        ercd = E_OK;
    }

    if (!_kernel_inROM((const void *)pk_ctsk))
    {
        coff = sizeof (T_TCB);
        size = coff + sizeof(V4_T_CTSK);
    }
    else
    {
        coff = 0;
        size = sizeof(T_TCB);
    }
    tcb = sys_alloc((SIZE)size);
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, SYSER = E_SYS);
    }
    stksz = pk_ctsk->stksz;
    if (pk_ctsk->stk == NULL)
    {   sp = stk_alloc((SIZE)stksz);
        if (sp == NULL)
        {   sys_free(tcb, (SIZE)size);
            return_ret_ims2(THF_TSK, SYSER = E_NOMEM);
        }
    }
    else
        sp = pk_ctsk->stk; /* Warning change far pointer to near pointer */
    pTCB[tskid] = tcb;
    tcb->sts = S_DMT;
    tcb->act = 0;
    tcb->lock = 0;
    if (coff != 0)
    {   tcb->ctsk = (const V4_T_CTSK *)((ADDR)tcb + coff);
        osmem_cpy((DFAR B*)tcb->ctsk,(DFAR B*)pk_ctsk,sizeof(*pk_ctsk));
    }
    else
        tcb->ctsk = pk_ctsk;
    tcb->bpri = tcb->pri = (TSK_PRI)pk_ctsk->itskpri;
    tcb->stk = (T_CTX SFAR *)sp;
    tcb->excb = NULL;
  #if ERC
    cre_tsk_sub();
  #ifndef LARGE_KERNEL
  #if (SIZEOF_INT <= 2)
    ptn = (tskid << 8) | tskid;
    stksz /= 2;
  #else
    ptn = (tskid << 24) | (tskid << 16) | (tskid << 8) | tskid;
    stksz /= 4;
  #endif
  #else
   #if (SIZEOF_INT <= 2)
    ptn = tskid;
    stksz /= 2;
   #else
    ptn = (tskid << 16) | tskid;
    stksz /= 4;
   #endif
  #endif
    p = (int *)sp;
    do { *p++ = ptn; } while (--stksz != 0);
  #endif
    ret_ims2_(THF_TSK);
    if (pk_ctsk->tskatr & TA_ACT)
        v4_act_tsk(tskid);
    return ercd;
  #if ERC
    }
    vsnd_deb2e(THF_TSK, TFN_CRE_TSK, (long)tskid, (long)pk_ctsk, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif      /* cre_tsk */
#ifdef V3_CRE_TSK
/************************************************/
/* Create Task                                  */
/************************************************/

ER v3_cre_tsk(ID tskid, const V3_T_CTSK *pk_ctsk)
{
    V4_T_CTSK ctsk;

    ctsk.tskatr = pk_ctsk->tskatr;
    ctsk.exinf = pk_ctsk->exinf;
    ctsk.task = pk_ctsk->task;
    ctsk.itskpri = pk_ctsk->itskpri;
    ctsk.stksz = pk_ctsk->stksz;
    ctsk.stk = NULL;
    ctsk.name = &_KERNEL_NNM;

    return v4_cre_tsk(tskid, (const V4_T_CTSK *)&ctsk);
}

#endif      /* cre_tsk */
#ifdef V4_DEL_TSK
/************************************************/
/* Delete Task                                  */
/************************************************/

ER v4_del_tsk(ID tskid)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    unsigned stksz;
    B SFAR *stktop;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (tskid == TSK_SELF)
        ercd = E_OBJ;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSK, TFN_DEL_TSK, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    if (tcb->sts != S_DMT)
    {   return_ret_ims2(THF_TSK, E_OBJ);
    }
    if (DDISP && (RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {   if (PREVTSK == (TSK_ID)tskid)
            DDISP = TSS_TSK;
    }
    if (tcb->ctsk->stk == NULL)
    {   stksz = tcb->ctsk->stksz;
        stktop = (B SFAR *)tcb->stk;
        stk_free(stktop, (SIZE)stksz);
    }
    if ((excb = tcb->excb) != NULL)
    {   if (!_kernel_inROM((const void *)tcb->excb->dtex))
            sys_free(excb, (SIZE)(sizeof(T_EXCB) + sizeof(V4_T_DTEX)));
        else
            sys_free(excb, (SIZE)sizeof(T_EXCB));
    }
    if (!_kernel_inROM((const void *)tcb->ctsk))
        sys_free(tcb, (SIZE)(sizeof (T_TCB) + sizeof(V4_T_CTSK)));
    else
        sys_free(tcb, (SIZE)sizeof (T_TCB));
    pTCB[tskid] = NULL;
    return_ret_ims(THF_TSK);
  #if ERC
    }
    vsnd_deb1e(THF_TSK, TFN_DEL_TSK, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_ACT_TSK
/************************************************/
/* Activate Task                                */
/************************************************/
ER v4_act_tsk(ID tskid)
{
    T_TCB SNEAR *tcb;
    T_CTX SFAR *ctx;
    VP_INT stacd;
    FP task;
  #if defined(CPU_86)
    B *stktop;
  #endif
  #if ERC
    ER ercd ;
  #endif

    if (tskid == TSK_SELF)
        tskid = RDQ[0];

  #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSK, TFN_ACT_TSK, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    else if (tcb->sts == S_DMT)
    {   ctx = (T_CTX SFAR *)((B SFAR *)tcb->stk + tcb->ctsk->stksz) - 1;
        stacd = tcb->ctsk->exinf;
        task = tcb->ctsk->task;
        sta_tsk_sub();
        ctx->pc = task;
        tcb->sp = ctx;
        tcb->wup = 0;
        tcb->sus = 0;
        tcb->rcnt = 0;
        tcb->sts = S_RDY;
        return add_rdq_dispatch_(THF_TSK, tcb, tskid);
    }
    if (++tcb->act != (B)(ACTCNT_MAX+1))
    {   return_ret_ims(THF_TSK);
    }
    tcb->act = (B)ACTCNT_MAX;
    return_ret_ims2(THF_TSK, E_QOVR);
  #if ERC
    }
    vsnd_deb1e(THF_TSK, TFN_ACT_TSK, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STA_TSK
/************************************************/
/* Start Task                                   */
/************************************************/
ER v4_sta_tsk(ID tskid, VP_INT stacd)
{
    T_TCB SNEAR *tcb;
    T_CTX SFAR *ctx;
    FP task;
  #if defined(CPU_86)
    B SFAR *stktop;
  #endif
  #if ERC
    ER ercd ;

    if (tskid == TSK_SELF)
        ercd = E_OBJ;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_STA_TSK, (long)tskid, (long)stacd);
  #endif
    if ((tcb = pTCB[tskid]) == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    else if (tcb->sts == S_DMT)
    {   ctx = (T_CTX SFAR *)((B SFAR *)tcb->stk + tcb->ctsk->stksz) - 1;
        task = tcb->ctsk->task;
        sta_tsk_sub();
        ctx->pc = task;
        tcb->sp = ctx;
        tcb->wup = 0;
        tcb->sus = 0;
        tcb->rcnt = 0;
        tcb->sts = S_RDY;
        return add_rdq_dispatch_(THF_TSK, tcb, tskid);
    }
    return_ret_ims2(THF_TSK, E_OBJ);
  #if ERC
    }
    vsnd_deb2e(THF_TSK, TFN_STA_TSK, (long)tskid, (long)stacd, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_CAN_TSK
/************************************************/
/* Cancel Task                                  */
/************************************************/

ER v4_can_act(ID tskid)
{
    T_TCB SNEAR *tcb;
    INT act;
  #if ERC
    ER ercd;

    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
    vsnd_deb1(THF_TSK, TFN_CAN_ACT, (long)tskid);
  #endif
    if (tskid == TSK_SELF)
        tskid = RDQ[0];
    dis_ims();
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    act = tcb->act;
    tcb->act = 0;
    return_ret_ims2(THF_TSK, (UB)act);
  #if ERC
    }
    vsnd_deb1e(THF_TSK, TFN_CAN_ACT, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_EXT_TSK
/************************************************/
/* Exit Task                                    */
/************************************************/

void c_ext_tsk(T_CTX SFAR *ctx, VP_INT stacd, T_TCB SNEAR *tcb, ID tskid)
{
    FP task;

    task = tcb->ctsk->task;
    sta_tsk_sub();
    ctx->pc = task;
    tcb->sp = ctx;
    tcb->wup = 0;
    tcb->sus = 0;
    tcb->rcnt = 0;
    tcb->sts = S_RDY;
    add_que((TSK_ID SNEAR *)RDQ + tcb->bpri, tcb, tskid);
    dispatch3();
}

void v4_ext_tsk(void)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    ID tskid;
    T_CTX SFAR *ctx;
    VP_INT stacd;

  #if ERC
    if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {   SYSER = E_CTX;
        DDISP = TSS_TSK;
        vsnd_deb0(THF_TSK, TFN_EXT_TSK);
        return;
    }
  #endif
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_EXT_TSK);
  #endif
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    tcb->bpri = tcb->pri = (TSK_PRI)tcb->ctsk->itskpri;
    if ((excb = tcb->excb) != NULL)
    {   excb->sp = NULL;
        excb->stat = TTEX_DIS;
        excb->pndptn = 0;
    }
    del_que(tcb, tskid);
    if (tcb->lock)
    {   unlock_allmtx(tcb->lock);
        tcb->lock = 0;
    }
    if (tcb->act > 0)
    {   tcb->act--;
        ctx = (T_CTX SFAR *)((B SFAR *)tcb->stk + tcb->ctsk->stksz) - 1;
        stacd = (VP_INT) tcb->ctsk->exinf;
        return_ext_tsk(ctx, stacd, tcb, tskid);
    }
    else
        tcb->sts = S_DMT;
    dispatch3();
}

#endif
#ifdef V4_EXD_TSK
/************************************************/
/* Exit and Delete Task                         */
/************************************************/

void v4_exd_tsk(void)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    unsigned stksz;
    B SFAR *stktop;
    ID tskid;

  #if ERC
    if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {
        DDISP = TSS_TSK;
        vsnd_deb0e(THF_TSK, TFN_EXD_TSK,SYSER = E_CTX);
        return;
    }
  #endif
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_EXD_TSK);
  #endif
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    del_que(tcb, tskid);
    if (tcb->lock)
        unlock_allmtx(tcb->lock);
    if (tcb->ctsk->stk == NULL)
    {   stksz = tcb->ctsk->stksz;
        stktop = (B SFAR *)tcb->stk;
        stk_free(stktop, (SIZE)stksz);
    }
    if ((excb = tcb->excb) != NULL)
    {   if (!_kernel_inROM((const void *)tcb->excb->dtex))
            sys_free(excb, (SIZE)(sizeof(T_EXCB) + sizeof(V4_T_DTEX)));
        else
            sys_free(excb, (SIZE)sizeof(T_EXCB));
    }
    if (!_kernel_inROM((const void *)tcb->ctsk))
        sys_free(tcb, (SIZE)(sizeof (T_TCB) + sizeof(V4_T_CTSK)));
    else
        sys_free(tcb, (SIZE)sizeof (T_TCB));
    pTCB[tskid] = NULL;
    dispatch3();
}

#endif
#ifdef V4_TER_TSK
/************************************************/
/* Terminate Task                               */
/************************************************/

ER v4_ter_tsk(ID tskid)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    INT sts;
    T_CTX SFAR *ctx;
    VP_INT stacd;
    FP task;
  #if ERC
    ER ercd;

    if ((tskid == TSK_SELF) || (!INEST && RDQ[0] == (TSK_ID)tskid))
        ercd = E_ILUSE;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSK, TFN_TER_TSK, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    sts = tcb->sts;
    if (sts != S_DMT)
    {   if (INEST && RDQ[0] == (TSK_ID)tskid)
            DELAY = 1;
        if (DDISP && (RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        {   if (PREVTSK == (TSK_ID)tskid)
                DDISP = TSS_TSK;
        }
        tcb->bpri = tcb->pri = (TSK_PRI)tcb->ctsk->itskpri;
        if (sts & S_TMO)
            del_tmq(tcb, tskid);
        if ((sts == S_RDY && tcb->sus == 0)
         || (sts >= S_FLG))
        {   del_que(tcb, tskid);
            if ((sts & STS_MSK1) == S_MTX)
                reset_occ_cpri(pMTX[get_wid()]->occ);
            else if ((sts & STS_MSK1) == S_SMB)
                push_msg2mbf(get_wid());
            else if ((sts & STS_MSK0) == S_MPL)
                rel_mplchk(get_wid());
        }
        if (tcb->lock)
        {   unlock_allmtx(tcb->lock);
            tcb->lock = 0;
        }
        if ((excb = tcb->excb) != NULL)
        {   excb->sp = NULL;
            excb->stat = TTEX_DIS;
            excb->pndptn = 0;
        }
        if ((UB)tcb->act > 0)
        {   tcb->act--;
            ctx = (T_CTX SFAR *)((B SFAR *)tcb->stk + tcb->ctsk->stksz) - 1;
            stacd = (VP_INT) tcb->ctsk->exinf;
            task = tcb->ctsk->task;
            sta_tsk_sub();
            ctx->pc = task;
            tcb->sp = ctx;
            tcb->wup = 0;
            tcb->sus = 0;
            tcb->rcnt = 0;
            tcb->sts = S_RDY;
            return add_rdq_dispatch_(THF_TSK, tcb, tskid);
        }
        else
            tcb->sts = S_DMT;
        return_ret_ims(THF_TSK);
    }
    return_ret_ims2(THF_TSK, E_OBJ);
  #if ERC
    }
    vsnd_deb1e(THF_TSK, TFN_TER_TSK, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_DIS_DSP
/************************************************/
/* Disable Dispatch                             */
/************************************************/

ER v4_dis_dsp(void)
{
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_DIS_DSP);
  #endif
    if (DDISP & TSS_DDSP)
    {
        if (ref_sys_sub(IMASK))
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
    }
    else
    {
      #if ERC
        if (sns_ctx_sub())
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
      #endif
        DDISP |= (TSS_V4_DDSP | TSS_DDSP);
    }
    return_ret_ims(THF_TSK);
}

#endif
#ifdef V3_DIS_DSP
/************************************************/
/* Disable Dispatch                             */
/************************************************/

ER v3_dis_dsp(void)
{
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_DIS_DSP);
  #endif
    if (DDISP & TSS_DDSP)
    {
        if (ref_sys_sub(IMASK))
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
    }
    else
    {
      #if ERC
        if (sns_ctx_sub())
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
      #endif
        DDISP |= TSS_DDSP;
    }
    return_ret_ims(THF_TSK);
}

#endif
#ifdef V4_ENA_DSP
/************************************************/
/* Enable Dispatch                              */
/************************************************/

ER v4_ena_dsp(void)
{
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_ENA_DSP);
  #endif
    if (DDISP & TSS_DDSP)
    {
      #if ERC
        if (sns_ctx_sub())
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
      #endif
        if (DDISP & TSS_V4_LOC)
            DDISP &= ~TSS_V4_DDSP;
        else
        {   DDISP &= ~(TSS_V4_DDSP | TSS_DDSP);
            if (!DDISP)
            {   SDISP = 0;
                if (DELAY)
                    return_dispatch2(THF_TSK);
            }
        }
    }
    return_ret_ims(THF_TSK);
}

#endif
#ifdef V3_ENA_DSP
/************************************************/
/* Enable Dispatch                              */
/************************************************/

ER v3_ena_dsp(void)
{
    dis_ims();
  #if ERC
    visnd_deb0(THF_TSK, TFN_ENA_DSP);
  #endif
    if (DDISP & TSS_DDSP)
    {
      #if ERC
        if (sns_ctx_sub()||ref_sys_sub(IMASK))
        {   return_ret_ims2(THF_TSK, SYSER = E_CTX);
        }
      #endif
        DDISP &= ~TSS_DDSP;
        if (!DDISP)
        {   SDISP = 0;
            if (DELAY)
                return_dispatch2(THF_TSK);
        }
    }
    return_ret_ims(THF_TSK);
}

#endif
#ifdef V4_CHG_PRI
/************************************************/
/* Change Task Priority                         */
/************************************************/
ER v4_chg_pri(ID tskid, PRI tskpri)
{
    T_TCB SNEAR *tcb;
    T_MTX SNEAR *mtx;
    TSK_ID SNEAR *que;
    ID mtxid;
    PRI pri, maxpri;
    BOOL no_mtx, self = (tskid == TSK_SELF);
  #if ERC
    ER ercd;
  #endif

    if (self)
        tskid = RDQ[0];
  #if ERC
    if ((UINT)tskpri > (UINT)CFG.tpri_max)
       ercd = E_PAR;
    else if ((UINT) tskid > (UINT) CFG.tskid_max)
       ercd = E_ID;
    else if (INEST && self)
       ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_CHG_PRI, (long)tskid, (long)tskpri);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    if (tcb->sts == S_DMT)
    {   return_ret_ims2(THF_TSK, E_OBJ);
    }
    if (tskpri == TPRI_INI)
        tskpri = tcb->ctsk->itskpri;
    no_mtx = (tcb->lock == 0) && ((tcb->sts & STS_MSK1) != S_MTX);

    if (!no_mtx)
    {   if (tcb->pri > tskpri)
        {   maxpri = CFG.tpri_max;
            for(mtxid = tcb->lock;
                mtxid != 0;
                mtxid = mtx->nid)
            {   mtx = pMTX[mtxid];
                pri = mtx->ceil;
                if ((mtx->atr == TA_CEILING) && (tskpri < pri))
                {   return_ret_ims2(THF_TSK, E_ILUSE);
                }
                if ((pri != 0) && (maxpri > pri))
                    maxpri = pri;
            }
            if (maxpri < tskpri)
                goto L2;
        }
        else
L2:     {   tcb->bpri = (TSK_PRI)tskpri;
            return_ret_ims(THF_TSK);
        }
    }

    tcb->bpri = (TSK_PRI)tskpri;
    pri = tcb->pri;
    tcb->pri = (TSK_PRI)tskpri;
    if (tcb->sts == S_RDY && tcb->sus == 0)
    {   chg_que((TSK_ID SNEAR *)RDQ + tskpri, tcb, tskid);
        if (( (tskid == RDQ[0]) && (tskpri >= NOWPRI)) ||
            (!(tskid == RDQ[0]) && (tskpri <= NOWPRI)))
        {  return_dispatch(THF_TSK);
        }
        if (tskid == RDQ[0])
            NOWPRI = (TSK_PRI)tskpri;
        return_ret_ims(THF_TSK);
    }

    if (tcb->sts >= S_FLG)
    {   que = tcb->que;
        if (*(que+1) != TSK_QUETAIL)
        {   que -= pri - (TSK_PRI)tskpri;
            chg_que(que, tcb, tskid);
        }
        if ((tcb->sts & STS_MSK1) == S_MTX)
        {   mtxid = get_wid();
            mtx = pMTX[mtxid];
            if (mtx->atr == TA_INHERIT)
            {   que = mtx->que;
                for(maxpri = 1; *que == 0; maxpri++, que++);
                if (maxpri != mtx->ceil)
                    reset_occ_cpri(mtx->occ);
            }
        }
        else if ((tcb->sts &STS_MSK1) == S_SMB)
        {   if (push_msg2mbf(get_wid()) && !DDISP)
            {   return_dispatch1(THF_TSK);
            }
        }
        else if ((tcb->sts & STS_MSK0) == S_MPL)
        {   if (rel_mplchk(get_wid()))
            {   return_dispatch1(THF_TSK);
            }
        }
    }
    return_ret_ims(THF_TSK);
  #if ERC
    }
    vsnd_deb2e(THF_TSK, TFN_CHG_PRI, (long)tskid, (long)tskpri, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_ROT_RDQ
/************************************************/
/* Rotate Ready Queue                           */
/************************************************/

ER v4_rot_rdq(PRI tskpri)
{
    T_TCB SNEAR *tcb;
    ID tskid;
    TSK_ID SNEAR *rdq;

    if (tskpri == TPRI_SELF)
    {
      if ((RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        {    vsnd_deb1(THF_TSK, TFN_ROT_RDQ, (long)tskpri);
            return SYSER = E_PAR;
        }
      dis_ims();
      #if ERC
        visnd_deb1(THF_TSK, TFN_ROT_RDQ, (long)tskpri);
      #endif
        tskid = RDQ[0];
        tcb = pTCB[tskid];
        if (NOWPRI != tcb->bpri)
        {   rdq = RDQ + tcb->bpri;
            if ((tskid = *rdq) == 0)
                return_ret_ims(THF_TSK);
            tcb = pTCB[tskid];
        }
        else
            rdq = tcb->que;
        if (tcb->nid != tskid )
        {   *rdq = tcb->nid;
            return_dispatch(THF_TSK);
        }
        return_ret_ims(THF_TSK);
    }
  #if ERC
    if ((UINT)tskpri > (UINT)CFG.tpri_max)
    {   vsnd_deb1e(THF_TSK, TFN_ROT_RDQ, (long)tskpri, E_PAR);
        return SYSER = E_PAR;
    }
  #endif
    rdq = RDQ + tskpri;
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSK, TFN_ROT_RDQ, (long)tskpri);
  #endif
    tskid = *rdq;
    if ((tskid != 0) && ((tcb = pTCB[tskid])->nid) != tskid)
    {   *rdq = tcb->nid;
        if (tskpri <= (PRI)NOWPRI)
        {   return_dispatch(THF_TSK);
        }
    }
    return_ret_ims(THF_TSK);
}

#endif
#ifdef V4_REL_WAI
/************************************************/
/* Release Wait                                 */
/************************************************/

ER v4_rel_wai(ID tskid)
{
    T_TCB SNEAR *tcb;
    T_MTX SNEAR *mtx;
    INT sts;
  #if ERC
    ER ercd;

    if (tskid == TSK_SELF)
        ercd = E_OBJ;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSK, TFN_REL_WAI, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    sts = tcb->sts;
    if (sts & S_TMO)
    {   sts &= ~S_TMO;
        del_tmq(tcb, tskid);
    }
    if (sts >= S_FLG)
    {   tcb->sts = S_RDY;
        tcb->sp->ercd = E_RLWAI;
        if (sts == S_MTX)
        {   mtx = pMTX[get_wid()];
            del_que(tcb, tskid);
            if ((mtx->atr == TA_INHERIT) && (mtx->ceil >= tcb->pri))
                reset_occ_cpri(mtx->occ);
            if (tcb->sus == 0)
                return add_rdq_dispatch_(THF_TSK, tcb, tskid);
            else
                return_ret_ims(THF_TSK);
        }
        else if (sts == S_SMB)
        {   del_que(tcb, tskid);
            if (tcb->sus == 0)
                chg_rdq(tcb, tskid);
            if (push_msg2mbf(get_wid()) || (tcb->pri < NOWPRI))
            {   return_dispatch(THF_TSK);
            }
            else
            {   return_ret_ims(THF_TSK);
            }
        }
        else if (sts == S_MPL)
        {   del_que(tcb, tskid);
            if (tcb->sus == 0)
                chg_rdq(tcb, tskid);
            if (rel_mplchk(get_wid()) || (tcb->pri < NOWPRI))
            {   return_dispatch(THF_TSK);
            }
            else
            {   return_ret_ims(THF_TSK);
            }
        }
        else if (tcb->sus == 0)
            return chg_rdq_dispatch_(THF_TSK, tcb, tskid);
        else
            return del_que_ret_ims_(THF_TSK, tcb, tskid);
    }
    if (sts >= S_SLP)
    {   tcb->sts = S_RDY;
        tcb->sp->ercd = E_RLWAI;
        if (tcb->sus == 0)
            return add_rdq_dispatch_(THF_TSK, tcb, tskid);
        else
            return_ret_ims(THF_TSK);
    }
    return_ret_ims2(THF_TSK, E_OBJ);
  #if ERC
    }
    vsnd_deb1e(THF_TSK, TFN_REL_WAI, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_GET_PRI
/************************************************/
/* Get Task Priority                            */
/************************************************/

ER v4_get_pri(ID tskid, PRI *p_tskpri)
{
    T_TCB SNEAR *tcb;
    INT sts;
    BOOL self = (tskid == TSK_SELF);
  #if ERC
    ER ercd;
  #endif

    if (self)
        tskid = RDQ[0];
  #if ERC
    if ((UINT) tskid > (UINT) CFG.tskid_max)
        ercd = E_ID;
    else if (INEST && self)
        ercd = E_ID;
    else
    {
  #endif

    if (tskid == RDQ[0])
    {   tcb = pTCB[RDQ[0]];
        *p_tskpri = tcb->pri;
        vsnd_deb2e(THF_TSK, TFN_GET_PRI, (long)tskid, (long)p_tskpri, E_OK);
        return E_OK;
    }

    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_GET_PRI, (long)tskid, (long)p_tskpri);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSK, E_NOEXS);
    }
    sts = tcb->sts;
    if (sts == S_DMT)
    {   return_ret_ims2(THF_TSK, E_OBJ);
    }
    *p_tskpri = tcb->pri;
    return_ret_ims(THF_TSK);
  #if ERC
    }
    vsnd_deb2e(THF_TSK, TFN_GET_PRI, (long)tskid, (long)p_tskpri, (long)ercd);
    return SYSER = ercd;
  #endif

}

#endif
#ifdef V4_GET_TID
/************************************************/
/* Get Task Identifier                          */
/************************************************/

ER v4_get_tid(ID *p_tskid)
{
  #if ERC
    vsnd_deb1e(THF_TSK, TFN_GET_TID, (long)p_tskid, E_OK);
  #endif
    if (RDQ[0] == (TSK_ID)(CFG.tskid_max+1))
        *p_tskid = TSK_NONE;
    else
        *p_tskid = RDQ[0];
    return E_OK;
}

#endif
#ifdef V3_VGET_TID
/************************************************/
/* Get Task Identifier Itself                   */
/************************************************/

ID v3_vget_tid(void)
{
    ID id;

    if (RDQ[0] == (TSK_ID)(CFG.tskid_max+1))
        id = TSK_NONE;
    else
        id = RDQ[0];
  #if ERC
    vsnd_deb0e(THF_TSK, TFN_GET_TID, (long)id);
  #endif
    return id;
}

#endif
#ifdef V4_REF_TSK
/************************************************/
/* Refer Task Status                            */
/************************************************/

ER v4_ref_tsk(ID tskid, V4_T_RTSK *pk_rtsk)
{
    T_TCB SNEAR *tcb;
    INT sts, sus;
    static const UH ttw[] =
    {   TTW_SLP,TTW_DLY,TTW_RDV,TTW_FLG,TTW_SMBF,TTW_CAL,
        TTW_ACP,TTW_SEM,TTW_MBX,TTW_MBF,TTW_MPL ,TTW_MPF
    };
    static const UH ttw2[] =
    {   TTW_SDTQ,0,0,TTW_MTX,0,TTW_RDTQ
    };

    if ((UINT)tskid > (UINT)CFG.tskid_max)
    {
      #if ERC
        vsnd_deb2e(THF_TSK, TFN_REF_TSK, (long)tskid, (long)pk_rtsk, E_ID);
      #endif
        return E_ID;
    }
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_REF_TSK, (long)tskid, (long)pk_rtsk);
  #endif
    if (tskid == TSK_SELF)
    {   tskid = RDQ[0];
        tcb = pTCB[tskid];
    }
    else
    {   tcb = pTCB[tskid];
        if (tcb == NULL)
        {   return_ret_ims2(THF_TSK, E_NOEXS);
        }
    }
    pk_rtsk->exinf = tcb->ctsk->exinf;
    pk_rtsk->tskatr = tcb->ctsk->tskatr;
    pk_rtsk->task = tcb->ctsk->task;
    pk_rtsk->itskpri = tcb->ctsk->itskpri;
    pk_rtsk->stksz = tcb->ctsk->stksz;
    pk_rtsk->tskpri = tcb->pri;
    pk_rtsk->tskbpri = tcb->bpri;
    pk_rtsk->actcnt = (UB)tcb->act;
    sts = (INT)tcb->sts;
    if (sts == S_DMT)
    {   ret_ims2_(THF_TSK);
        pk_rtsk->tskstat = TTS_DMT;
        pk_rtsk->wupcnt = 0;
        pk_rtsk->suscnt = 0;
        pk_rtsk->lefttmo = 0;
        pk_rtsk->tskwait = 0;
        pk_rtsk->wid = 0;
        return E_OK;
    }
    pk_rtsk->wupcnt = (UB)tcb->wup;
    if (sts == S_RDY)
    {   if (tskid == (ID)RDQ[0])
        {   ret_ims2_(THF_TSK);
            pk_rtsk->suscnt = 0;
            pk_rtsk->tskstat = TTS_RUN;
        }
        else
        {   sus = tcb->sus;
            ret_ims2_(THF_TSK);
            pk_rtsk->suscnt = sus;
            if (sus == 0)
                pk_rtsk->tskstat = TTS_RDY;
            else
                pk_rtsk->tskstat = TTS_SUS;
        }
        pk_rtsk->lefttmo = 0;
        pk_rtsk->tskwait = 0;
        pk_rtsk->wid = 0;
        return E_OK;
    }
    if (sts & S_TMO)
        pk_rtsk->lefttmo = tcb->tmo - SYSCK.ltime;
    else
        pk_rtsk->lefttmo = TMO_FEVR;
    if ((sts &= STS_MSK1) <= S_DLY)
        pk_rtsk->wid = 0;
    else
        pk_rtsk->wid = get_wid();
    sus = tcb->sus;
    ret_ims2_(THF_TSK);
    pk_rtsk->suscnt = sus;
    if (sus == 0)
        pk_rtsk->tskstat = TTS_WAI;
    else
        pk_rtsk->tskstat = TTS_WAS;
    if ((sts >= S_SDT) && (sts & S_CLR))
        pk_rtsk->tskwait = ttw2[(sts - S_SDT) >> 4];
    else
        pk_rtsk->tskwait = ttw[(sts - S_SLP) >> 4];
    return E_OK;
}

#endif
#ifdef V3_REF_TSK
/************************************************/
/* Refer Task Status                            */
/************************************************/

ER v3_ref_tsk(V3_T_RTSK *pk_rtsk, ID tskid)
{
    T_TCB SNEAR *tcb;
    INT sts, sus;
    static const UH ttw[] =
    {   TTW_SLP,TTW_DLY,TTW_RDV,TTW_FLG,TTW_SMBF,TTW_CAL,
        TTW_ACP,TTW_SEM,TTW_MBX,TTW_MBF,TTW_MPL ,TTW_MPF
    };

    if (tskid > CFG.tskid_max)
    {
      #if ERC
        vsnd_deb2e(THF_TSK, TFN_REF_TSK, (long)tskid, (long)pk_rtsk, E_ID);
      #endif
        return E_ID;
    }
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_REF_TSK, (long)tskid, (long)pk_rtsk);
  #endif
    if (tskid == TSK_SELF)
    {   tskid = RDQ[0];
        tcb = pTCB[tskid];
    }
    else
    {   tcb = pTCB[tskid];
        if (tcb == NULL)
        {   return_ret_ims2(THF_TSK, E_NOEXS);
        }
    }
    pk_rtsk->exinf = (tcb->ctsk)->exinf;
    pk_rtsk->tskatr = (tcb->ctsk)->tskatr;
    pk_rtsk->task = tcb->ctsk->task;
    pk_rtsk->itskpri = tcb->ctsk->itskpri;
    pk_rtsk->stksz = tcb->ctsk->stksz;
    pk_rtsk->tskpri = tcb->bpri;
    sts = (INT)(tcb->sts & 0xf0);
    if (sts == S_DMT)
    {   ret_ims2_(THF_TSK);
        pk_rtsk->tskstat = TTS_DMT;
        pk_rtsk->wupcnt = 0;
        pk_rtsk->suscnt = 0;
        pk_rtsk->tskwait = 0;
        pk_rtsk->wid = 0;
        return E_OK;
    }
    pk_rtsk->wupcnt = (UB)tcb->wup;
    if (sts == S_RDY)
    {   if (tskid == (ID)RDQ[0])
        {   ret_ims2_(THF_TSK);
            pk_rtsk->suscnt = 0;
            pk_rtsk->tskstat = TTS_RUN;
        }
        else
        {   sus = tcb->sus;
            ret_ims2_(THF_TSK);
            pk_rtsk->suscnt = sus;
            if (sus == 0)
                pk_rtsk->tskstat = TTS_RDY;
            else
                pk_rtsk->tskstat = TTS_SUS;
        }
        pk_rtsk->tskwait = 0;
        pk_rtsk->wid = 0;
        return E_OK;
    }
    if (sts <= S_DLY)
        pk_rtsk->wid = 0;
    else
        pk_rtsk->wid = get_wid();
    sus = tcb->sus;
    ret_ims2_(THF_TSK);
    pk_rtsk->suscnt = sus;
    if (sus == 0)
        pk_rtsk->tskstat = TTS_WAI;
    else
        pk_rtsk->tskstat = TTS_WAS;
    pk_rtsk->tskwait = ttw[(sts - S_SLP) >> 4];
    return E_OK;
}

#endif
#ifdef V4_REF_TST
/************************************************/
/* Quick Refer Task Status                      */
/************************************************/

ER v4_ref_tst(ID tskid, V4_T_RTST *pk_rtst)
{
    T_TCB SNEAR *tcb;
    INT sts, sus;
    static const UH ttw[] =
    {   TTW_SLP,TTW_DLY,TTW_RDV,TTW_FLG,TTW_SMBF,TTW_CAL,
        TTW_ACP,TTW_SEM,TTW_MBX,TTW_MBF,TTW_MPL ,TTW_MPF
    };
    static const UH ttw2[] =
    {   TTW_SDTQ,0,0,TTW_MTX,0,TTW_RDTQ
    };

    if ((UINT)tskid > (UINT)CFG.tskid_max)
    {
      #if ERC
        vsnd_deb2e(THF_TSK, TFN_REF_TST, (long)tskid, (long)pk_rtst, E_ID);
      #endif
        return E_ID;
    }
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSK, TFN_REF_TST, (long)tskid, (long)pk_rtst);
  #endif
    if (tskid == TSK_SELF)
    {   tskid = RDQ[0];
        tcb = pTCB[tskid];
    }
    else
    {   tcb = pTCB[tskid];
        if (tcb == NULL)
        {   return_ret_ims2(THF_TSK, E_NOEXS);
        }
    }
    sts = (INT)(tcb->sts & STS_MSK0);
    if (sts == S_DMT)
    {   ret_ims2_(THF_TSK);
        pk_rtst->tskstat = TTS_DMT;
        pk_rtst->tskwait = 0;
        return E_OK;
    }
    if (sts == S_RDY)
    {   if (tskid == (ID)RDQ[0])
        {   ret_ims2_(THF_TSK);
            pk_rtst->tskstat = TTS_RUN;
        }
        else
        {   sus = tcb->sus;
            ret_ims2_(THF_TSK);
            if (sus == 0)
                pk_rtst->tskstat = TTS_RDY;
            else
                pk_rtst->tskstat = TTS_SUS;
        }
        pk_rtst->tskwait = 0;
        return E_OK;
    }
    sts = (INT)(tcb->sts & STS_MSK1);
    sus = tcb->sus;
    ret_ims2_(THF_TSK);
    if (sus == 0)
        pk_rtst->tskstat = TTS_WAI;
    else
        pk_rtst->tskstat = TTS_WAS;
    if ((sts >= S_SDT) && (sts & S_CLR))
        pk_rtst->tskwait = ttw2[(sts - S_SDT) >> 4];
    else
        pk_rtst->tskwait = ttw[(sts - S_SLP) >> 4];
    return E_OK;
}

#endif
#ifdef V4_SUS_TSK
/************************************************/
/* Suspend Task                                 */
/************************************************/

ER v4_sus_tsk(ID tskid)
{
    T_TCB SNEAR *tcb;
    B sus;
    INT sts;
  #if ERC
    ER ercd;

    if (DDISP && tskid == TSK_SELF)
        ercd = E_CTX;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    if (tskid == TSK_SELF)
        tskid = RDQ[0];
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSKS, TFN_SUS_TSK, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSKS, E_NOEXS);
    }
    sts = tcb->sts;
    if (sts != S_DMT)
    {   sus = ++tcb->sus;
        if (sus != (B)(SUSCNT_MAX+1))
        {   if (sus == 1 && (sts == S_RDY))
            {   if (RDQ[0] == (TSK_ID)tskid)
                {   if (INEST)
                        DELAY = 1;
                    else
                        return del_que_dispatch1_(THF_TSKS, tcb, tskid);
                }
                return del_que_ret_ims_(THF_TSKS, tcb, tskid);
            }
            return_ret_ims(THF_TSKS);
        }
        tcb->sus = (B)SUSCNT_MAX;
        return_ret_ims2(THF_TSKS, E_QOVR);
    }
    return_ret_ims2(THF_TSKS, E_OBJ);
  #if ERC
    }
    vsnd_deb1e(THF_TSKS, TFN_SUS_TSK, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_RSM_TSK
/************************************************/
/* Resume Task                                  */
/************************************************/
/* actcd = 0: resume, 1: force resume           */
ER v4_rsm_tsk(ID tskid, INT actcd)
{
    T_TCB SNEAR *tcb;
    B sus;
    INT sts;
  #if ERC
    ER ercd;

    if ((tskid == TSK_SELF) || tskid == RDQ[0])
        ercd = E_OBJ;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TSKS, TFN_RSM_TSK, (long)tskid, (long)actcd);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSKS, E_NOEXS);
    }
    sts = tcb->sts;
    if ((sts != S_DMT) && ((sus = tcb->sus) != 0))
    {   if (actcd)
            sus = 0;
        else
            sus--;
        tcb->sus = sus;
        if ((sus == 0) && (sts == S_RDY))
            return add_rdq_dispatch_(THF_TSKS, tcb, tskid);
        else
            return_ret_ims(THF_TSKS);
    }
    return_ret_ims2(THF_TSKS, E_OBJ);
  #if ERC
    }
    vsnd_deb2e(THF_TSKS, TFN_RSM_TSK, (long)tskid, (long)actcd, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TSLP_TSK
/************************************************/
/* Sleep Task with Timeout                      */
/************************************************/

ER v4_tslp_tsk(TMO tmout)
{
    T_TCB SNEAR *tcb;
    ID tskid;
    INT wup;

  #if ERC
    if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {
        vsnd_deb1e(THF_TSKS, ((tmout==TMO_FEVR)?TFN_SLP_TSK:TFN_TSLP_TSK), (long)tmout, E_CTX);
        return SYSER = E_CTX;
    }
  #endif
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSKS, ((tmout==TMO_FEVR)?TFN_SLP_TSK:TFN_TSLP_TSK), (long)tmout);
  #endif
    wup = tcb->wup;
    if (wup == 0)
    {   if (tmout != TMO_POL)
        {   if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_SLP|S_TMO);
            }
            else
                tcb->sts = S_SLP;
            return del_que_dispatch1_(THF_TSKS, tcb, tskid);
        }
        return_ret_ims2(THF_TSKS, E_TMOUT);
    }
    tcb->wup = (B)(wup - 1);
    return_ret_ims(THF_TSKS);
}

#endif
#ifdef V4_WUP_TSK
/************************************************/
/* Wakeup Task                                  */
/************************************************/

ER v4_wup_tsk(ID tskid)
{
    T_TCB SNEAR *tcb;
    INT sts;
  #if ERC
    ER ercd;
  #endif

    if (tskid == TSK_SELF)
        tskid = RDQ[0];
  #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSKS, TFN_WUP_TSK, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TSKS, E_NOEXS);
    }
    sts = tcb->sts;
    if (sts == (S_SLP|S_TMO))
    {   tcb->sts = S_RDY;
        del_tmq(tcb, tskid);
        if (tcb->sus == 0)
            return add_rdq_dispatch_(THF_TSKS, tcb, tskid);
        else
            return_ret_ims(THF_TSKS);
    }
    if (sts == S_SLP)
    {   tcb->sts = S_RDY;
        if (tcb->sus == 0)
            return add_rdq_dispatch_(THF_TSKS, tcb, tskid);
        else
            return_ret_ims(THF_TSKS);
    }
    if (sts != S_DMT)
    {   if (++tcb->wup != (B)(WUPCNT_MAX+1))
            return_ret_ims(THF_TSKS);
        tcb->wup = (B)WUPCNT_MAX;
        return_ret_ims2(THF_TSKS, E_QOVR);
    }
    return_ret_ims2(THF_TSKS, E_OBJ);
  #if ERC
    }
    vsnd_deb1e(THF_TSKS, TFN_WUP_TSK, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_CAN_WUP
/************************************************/
/* Cancel Wakeup Task                           */
/************************************************/

ER_UINT v4_can_wup(ID tskid)
{
    T_TCB SNEAR *tcb;
    INT wupcnt;

    if (tskid == TSK_SELF)
       tskid = RDQ[0];
  #if ERC
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
    {   vsnd_deb1e(THF_TSKS, TFN_CAN_WUP, (long)tskid, E_ID);
        return SYSER = E_ID;
    }
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TSKS, TFN_CAN_WUP, (long)tskid);
  #endif
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {
        wupcnt =  E_NOEXS;
    }
    else if (tcb->sts == S_DMT)
    {
        wupcnt =  E_OBJ;
    }
    else
    {   wupcnt = (UB)tcb->wup;
        tcb->wup = 0;
    }
    return_ret_ims2(THF_TSKS, wupcnt);
}
#endif

#ifdef V3_CAN_WUP
ER v3_can_wup(INT *p_wupcnt, ID tskid)
{

    #if ERC
    visnd_deb2(THF_TSKS, TFN_CAN_WUP, (long)p_wupcnt, (long)tskid);
    if (p_wupcnt == NULL)
        return E_PAR;
    #endif

    *p_wupcnt = (INT)v4_can_wup(tskid);
    if (*p_wupcnt > 0)
        return E_OK;
    else
        return (ER) *p_wupcnt;
}
#endif
#ifdef V3_VCAN_WUP
/************************************************/
/* Cancel Wakeup Task Itself                    */
/************************************************/

void v3_vcan_wup(void)
{
    T_TCB SNEAR *tcb;
    ID tskid;

  #if ERC
    vsnd_deb2(THF_TSKS, TFN_CAN_WUP, (long)NULL, TSK_SELF);
  #endif
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    tcb->wup = 0;
}

#endif

#ifdef V4_DEF_TEX
/************************************************/
/* Define Task Exception Routine                */
/************************************************/

ER v4_def_tex(ID tskid, const V4_T_DTEX *pk_dtex)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    UINT size, off;
    TEXPTN pndptn;
    STAT stat;
    T_CTX SFAR *sp;
  #if ERC
    ER ercd;

    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else if ((pk_dtex != NULL) && (pk_dtex->texrtn == NULL))
        ercd = E_PAR;
    else
    {
    #endif
    if (tskid == TSK_SELF)
        tskid = RDQ[0];
    dis_ims();
  #if ERC
    visnd_deb2(THF_TEX, TFN_DEF_TEX, (long)tskid, (long)pk_dtex);
  #endif
    if ((tcb = pTCB[tskid]) == NULL)
    {   return_ret_ims2(THF_TEX, E_NOEXS);
    }
    if ((tcb->excb != NULL) || (pk_dtex != NULL))
    {
        if (tcb->excb == NULL)
        {
            pndptn = 0;
            stat = TTEX_DIS;
            sp = NULL;
        L1: size = sizeof(T_EXCB);
            if (!_kernel_inROM((const void *)pk_dtex))
            {   off = size;
                size += sizeof(*pk_dtex);
            }
            else
                off = 0;
            excb = sys_alloc((SIZE)size);
            tcb->excb = excb;
            excb->pndptn = pndptn;
            excb->stat = stat;
            excb->sp = sp;
            if (off != 0)
            {   tcb->excb->dtex = (const V4_T_DTEX *)((ADDR)excb + off);
                osmem_cpy((DFAR B*)tcb->excb->dtex,(DFAR B*)pk_dtex, sizeof(*pk_dtex));
            }
            else
                tcb->excb->dtex = pk_dtex;
        }
        else
        {   size = sizeof(T_EXCB);
            if (!_kernel_inROM((const void *)tcb->excb->dtex))
                size += sizeof (V4_T_DTEX);
            pndptn = tcb->excb->pndptn;
            stat = tcb->excb->stat;
            sp = tcb->excb->sp;
            sys_free(tcb->excb, (SIZE)size);
            if (pk_dtex != NULL)
                goto L1;
            tcb->excb = NULL;
        }
    }
    return_ret_ims(THF_TEX);
    #if ERC
    }
    vsnd_deb2e(THF_TEX, TFN_DEF_TEX, (long)tskid, (long)pk_dtex, (long)ercd);
    return SYSER = ercd;
    #endif
}

#endif
#ifdef V4_RAS_TEX
/************************************************/
/* Define Task Exception Routine                */
/************************************************/

void c_ext_exp(ER ercd)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    TEXPTN stacd;
    VP_INT exinf;
    void (FFAR *texrtn)(TEXPTN,VP_INT);

    tcb = pTCB[RDQ[0]];
    excb = tcb->excb;
    dis_ims();
    stacd = excb->pndptn;
    exinf = tcb->ctsk->exinf;
    if (excb->ctxsv == TRUE)
        excb->sp->ercd = ercd;
  #if ERC
    visnd_deb0(THF_TEX, TFN_EXT_EXP);
  #endif
    excb->pndptn = 0;
    do {
        texrtn = (void (FFAR *)(TEXPTN,VP_INT))excb->dtex->texrtn;
        ret_ims2();
        texrtn(stacd, exinf);
        dis_ims();
        stacd = excb->pndptn;
        excb->pndptn = 0;
    } while(stacd != 0);
    tcb->sp = excb->sp;
    excb->sp = NULL;

    dispatch3();
}

#ifndef ASM_EXT_EXP
#define a_ext_exp c_ext_exp
#else
extern void a_ext_exp(ER ercd);
#endif

void v4_ent_exp(BOOL runtskid, T_TCB SNEAR *tcb, T_EXCB SNEAR *excb)
{
    VP_INT exinf;
    TEXPTN stacd;
    void (FFAR *texrtn)(TEXPTN,VP_INT);

    excb->sp = tcb->sp;
    if (tcb->sts == S_RDY && runtskid && !INEST)
    {
        exinf = tcb->ctsk->exinf;
        while(excb->pndptn)
        {   stacd = excb->pndptn;
            excb->pndptn = 0;
            texrtn = (void (FFAR *)(TEXPTN,VP_INT))excb->dtex->texrtn;
            ret_ims2();
            texrtn(stacd, exinf);
            dis_ims();
        }
        excb->sp = NULL;
    }
    else
    {   T_CTX SFAR *ctx;
        FP rtn = (FP)a_ext_exp;
        ctx = (T_CTX *)tcb->sp - 1;
        ras_tex_sub();
        if ((excb->ctxsv = (tcb->sts != S_RDY)?TRUE:FALSE) == TRUE)
        {
            ctx->ptn = tcb->sp->ptn;
            ctx->ptr = tcb->sp->ptr;
            ctx->ercd = tcb->sp->ercd;
            ctx->wid = tcb->sp->wid;
        }
        ctx->pc = rtn;
        tcb->sp = ctx;
    }
}

ER v4_ras_tex(ID tskid, TEXPTN rasptn)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    #if ERC
    ER ercd;

    #endif
    if (tskid == TSK_SELF)
        tskid = RDQ[0];
    #if ERC
    if (tskid == CFG.tskid_max + 1)
        ercd = E_ID;
    else if (rasptn == 0)
        ercd = E_PAR;
    else if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
    #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TEX, TFN_RAS_TEX, (long)tskid, (long)rasptn);
  #endif
    if ((tcb = pTCB[tskid]) == NULL)
    {   return_ret_ims2(THF_TEX, E_NOEXS);
    }
    if (((excb = tcb->excb) == NULL) || (tcb->sts == S_DMT))
    {   return_ret_ims2(THF_TEX, E_OBJ);
    }
    excb->pndptn |= rasptn;
    if ((excb->stat != TTEX_DIS) && (excb->sp == NULL))
        v4_ent_exp(tskid == RDQ[0], tcb, excb);

    return_ret_ims(THF_TEX);
    #if ERC
    }
    vsnd_deb2e(THF_TEX, TFN_RAS_TEX, (long)tskid, (long)rasptn, (long)ercd);
    return SYSER = ercd;
    #endif
}

#endif
#ifdef V4_DIS_TEX
ER v4_dis_tex()
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    ID tskid = RDQ[0];
    #if ERC
    ER ercd;

    if (tskid == CFG.tskid_max + 1)
        ercd = E_CTX;
    else
    {
    #endif
    dis_ims();
    tcb = pTCB[tskid];
  #if ERC
    visnd_deb0(THF_TEX, TFN_DIS_TEX);
  #endif
    if ((excb = tcb->excb) == NULL)
    {   return_ret_ims2(THF_TEX, E_OBJ);
    }
    excb->stat = TTEX_DIS;
    return_ret_ims(THF_TEX);
    #if ERC
    }
    vsnd_deb0e(THF_TEX, TFN_DIS_TEX, (long)ercd);
    return SYSER = ercd;
    #endif
}

#endif
#ifdef V4_ENA_TEX
ER v4_ena_tex()
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    ID tskid = RDQ[0];
    #if ERC
    ER ercd;

    if (tskid == CFG.tskid_max + 1)
        ercd = E_CTX;
    else
    {
    #endif
    dis_ims();
    tcb = pTCB[tskid];
  #if ERC
    visnd_deb0(THF_TEX, TFN_ENA_TEX);
  #endif
    if ((excb = tcb->excb) == NULL)
    {   return_ret_ims2(THF_TEX, E_OBJ);
    }

    excb->stat = TTEX_ENA;
    if ((excb->pndptn) && excb->sp == NULL)
    {
        v4_ent_exp(TRUE, tcb, excb);
    }
    return_ret_ims(THF_TEX);
    #if ERC
    }
    vsnd_deb0e(THF_TEX, TFN_ENA_TEX, (long)ercd);
    return SYSER = ercd;
    #endif
}

#endif
#ifdef V4_SNS_TEX
BOOL v4_sns_tex()
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;

    if (RDQ[0] == CFG.tskid_max + 1)
    {   vsnd_deb0e(THF_TEX, TFN_SNS_TEX, (long)TRUE);
        return TRUE;
    }

    tcb = pTCB[RDQ[0]];
    dis_ims();
  #if ERC
    visnd_deb0(THF_TEX, TFN_SNS_TEX);
  #endif
    if ((excb = tcb->excb) == NULL)
    {   return_ret_ims2(THF_TEX, TRUE);
    }
    return_ret_ims2(THF_TEX, (excb->stat == TTEX_DIS));
}

#endif
#ifdef V4_REF_TEX
BOOL v4_ref_tex(ID tskid, V4_T_RTEX *p_rtex)
{
    T_TCB SNEAR *tcb;
    T_EXCB SNEAR *excb;
    #if ERC
    ER ercd;
    #endif
    if (tskid == TSK_SELF)
        tskid = RDQ[0];
    #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
    #endif

    dis_ims();
    #if ERC
    visnd_deb2(THF_TEX, TFN_REF_TEX, (long)tskid, (long)p_rtex);
    #endif
    tcb = pTCB[tskid];
    if ((excb = tcb->excb) == NULL || (tcb->sts == S_DMT))
    {   return_ret_ims2(THF_TEX, E_OBJ);
    }
    p_rtex->texstat = (excb->sp) ? TTEX_DIS:excb->stat;
    p_rtex->pndptn = excb->pndptn;
    return_ret_ims(THF_TEX);
    #if ERC
    }
    vsnd_deb2e(THF_TEX, TFN_REF_TEX, (long)tskid, (long)p_rtex, (long)ercd);
    return SYSER = ercd;
    #endif
}

#endif
#ifdef V4_CRE_SEM
/************************************************/
/* Create Semaphore                             */
/************************************************/
ER v4_cre_sem(ID semid, const V4_T_CSEM *pk_csem)
{
    T_SEM SNEAR *sem;
    int i, n, atid = (semid == ID_AUTO);
    UINT size, coff;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)semid > (UINT)CFG.semid_max)
        ercd = E_ID;
    else if ((UINT)pk_csem->maxsem  > (UINT)TMAX_MAXSEM
          || (UINT)pk_csem->isemcnt > (UINT)pk_csem->maxsem)
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_SEM, TFN_CRE_SEM, (long)semid, (long)pk_csem);
  #endif
    if (atid)
    {   for (semid = CFG.semid_max;;)
        {   if (pSEM[semid] == NULL)
                break;
            if (--semid == 0)
            {   return_ret_ims2(THF_SEM, E_NOID);
            }
        }
        ercd = semid;
    }
    else
    {   if (pSEM[semid] != NULL)
        {   return_ret_ims2(THF_SEM, E_OBJ);
        }
        ercd = E_OK;
    }
    if (pk_csem->sematr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_SEM) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_csem))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CSEM);
    }
    else
        coff = 0;
    sem = sys_alloc((SIZE)size);
    if (sem == NULL)
    {   return_ret_ims2(THF_SEM, SYSER = E_SYS);
    }
    pSEM[semid] = sem;
    if (coff != 0)
    {   sem->csem = (const V4_T_CSEM *)((ADDR)sem + coff);
        osmem_cpy((DFAR B*)sem->csem,(DFAR B*)pk_csem,sizeof(*pk_csem));
    }
    else
        sem->csem = pk_csem;
    sem->cnt = (H)pk_csem->isemcnt;
    sem->max = (H)pk_csem->maxsem;
    for (i = 0; i <= n; i++)
        sem->que[i] = 0;
    sem->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_SEM, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_SEM, TFN_CRE_SEM, (long)semid, (long)pk_csem, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_SEM
/************************************************/
/* Create Semaphore                             */
/************************************************/
ER v3_cre_sem(ID semid, const V3_T_CSEM *pk_csem)
{
    V4_T_CSEM   csem;

    csem.sematr = pk_csem->sematr;
    csem.isemcnt = pk_csem->isemcnt;
    csem.maxsem = pk_csem->maxsem;
    csem.name = &_KERNEL_NNM;

    return v4_cre_sem(semid, (const V4_T_CSEM *)&csem);
}

#endif
#ifdef V4_DEL_SEM
/************************************************/
/* Delete Semaphore                             */
/************************************************/

ER v4_del_sem(ID semid)
{
    T_SEM SNEAR *sem;
    T_TCB SNEAR *tcb;
    ID tskid;
    TSK_ID SNEAR *que;
    int disp = FALSE;
    UINT size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (semid == 0)
        ercd = E_ID;
    else if ((UINT)semid > (UINT)CFG.semid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_SEM, TFN_DEL_SEM, (long)semid);
  #endif
    sem = pSEM[semid];
    if (sem == NULL)
    {   return_ret_ims2(THF_SEM, E_NOEXS);
    }
    if (sem->cnt == 0)
    {   que = sem->que;
        tskid = *que;
        for (;;)
        {   for (;;)
            {   if (tskid == 0)
                    break;
                tcb = pTCB[tskid];
                tcb->sp->ercd = E_DLT;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   chg_rdq(tcb, tskid);
                    disp |= tcb->pri < NOWPRI;
                }
                else
                    del_que(tcb, tskid);
                tskid = *que;
            }
            tskid = *++que;
            if (tskid >= TSK_QUETAIL)
                break;
        }
    }
    size = sizeof (T_SEM);
    if (sem->que[1] < TSK_QUETAIL)
        size = size + (CFG.tpri_max - 1) * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)sem->csem))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CSEM);
    }
    sys_free(sem, (SIZE)size);
    pSEM[semid] = NULL;
    if (disp)
    {   return_dispatch(THF_SEM);
    }
    else
    {   return_ret_ims(THF_SEM);
    }
  #if ERC
    }
    vsnd_deb1e(THF_SEM, TFN_DEL_SEM, (long)semid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_SIG_SEM
/************************************************/
/* Signal Semaphore                             */
/************************************************/

ER v4_sig_sem(ID semid)
{
    T_SEM SNEAR *sem;
    T_TCB SNEAR *tcb;
    ID tskid;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if ((semid == 0) || ((UINT)semid > (UINT)CFG.semid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_SEM, TFN_SIG_SEM, (long)semid);
  #endif
    sem = pSEM[semid];
    if (sem == NULL)
    {   return_ret_ims2(THF_SEM, E_NOEXS);
    }
    if (sem->cnt == 0)
    {   que = sem->que;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                tcb = pTCB[tskid];
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                    return chg_rdq_dispatch_(THF_SEM, tcb, tskid);
                else
                    return del_que_ret_ims_(THF_SEM, tcb, tskid);
            }
        }
    }
    if (sem->cnt++ == sem->max)
    {   sem->cnt = sem->max;
        return_ret_ims2(THF_SEM, E_QOVR);
    }
    return_ret_ims(THF_SEM);
  #if ERC
    }
    vsnd_deb1e(THF_SEM, TFN_SIG_SEM, (long)semid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TWAI_SEM
/************************************************/
/* Wait on Semaphore with Timeout               */
/************************************************/

ER v4_twai_sem(ID semid, TMO tmout)
{
    T_SEM SNEAR *sem;
    T_TCB SNEAR *tcb;
    ID tskid;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if ((semid == 0) || ((UINT)semid > (UINT)CFG.semid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_SEM, ((tmout==TMO_FEVR)?TFN_WAI_SEM:((tmout==TMO_POL)?TFN_POL_SEM:TFN_TWAI_SEM)), (long)semid, (long)tmout);
  #endif
    sem = pSEM[semid];
    if (sem == NULL)
    {   return_ret_ims2(THF_SEM, E_NOEXS);
    }
    if (sem->cnt == 0)
    {
        if (tmout != TMO_POL)
        {
          #if ERC
            if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_SEM, SYSER = E_CTX);
            }
          #endif
            tskid = RDQ[0];
            tcb = pTCB[tskid];
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_SEM|S_TMO);
            }
            else
                tcb->sts = S_SEM;
            que = sem->que;
            set_wid(semid);
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            return chg_que_dispatch1_(THF_SEM, que, tcb, tskid);
        }
        return_ret_ims2(THF_SEM, E_TMOUT);
    }
    sem->cnt--;
    return_ret_ims(THF_SEM);
  #if ERC
    }
    vsnd_deb2e(THF_SEM, ((tmout==TMO_FEVR)?TFN_WAI_SEM:((tmout==TMO_POL)?TFN_POL_SEM:TFN_TWAI_SEM)), (long)semid, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_SEM
/************************************************/
/* Refer Semaphore Status                       */
/************************************************/

ER v4_ref_sem(ID semid, V4_T_RSEM *pk_rsem)
{
    T_SEM SNEAR *sem;
    ID tskid;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if ((semid == 0) || (semid > CFG.semid_max))
        SYSER = ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_SEM, TFN_REF_SEM, (long)semid, (long)pk_rsem);
      #endif
        sem = pSEM[semid];
        if (sem == NULL)
        {   return_ret_ims2(THF_SEM, E_NOEXS);
        }
        pk_rsem->semcnt = (UINT)sem->cnt;
        if (sem->cnt == 0)
        {   que = sem->que;
            for (;;)
            {   tskid = *que++;
                if (tskid != 0)
                {   if (tskid >= TSK_QUETAIL)
                        break;
                    pk_rsem->wtskid = tskid;
                    return_ret_ims(THF_SEM);
                }
            }
        }
        pk_rsem->wtskid = TSK_NONE;
        return_ret_ims(THF_SEM);
  #if ERC
    }
    vsnd_deb2e(THF_SEM, TFN_REF_SEM, (long)semid, (long)pk_rsem, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_SEM
/************************************************/
/* Refer Semaphore Status                       */
/************************************************/

ER v3_ref_sem(V3_T_RSEM *pk_rsem, ID semid)
{
    T_SEM SNEAR *sem;
    ID tskid;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if ((semid == 0) || (semid > CFG.semid_max))
        SYSER = ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_SEM, TFN_REF_SEM, (long)semid, (long)pk_rsem);
      #endif
        sem = pSEM[semid];
        if (sem == NULL)
        {   return_ret_ims2(THF_SEM, E_NOEXS);
        }
        pk_rsem->exinf = NULL;
        pk_rsem->semcnt = (INT)((UH)sem->cnt);
        if (sem->cnt == 0)
        {   que = sem->que;
            for (;;)
            {   tskid = *que++;
                if (tskid != 0)
                {   if (tskid >= TSK_QUETAIL)
                        break;
                    pk_rsem->wtsk = tskid;
                    return_ret_ims(THF_SEM);
                }
            }
        }
        pk_rsem->wtsk = FALSE;
        return_ret_ims(THF_SEM);
  #if ERC
    }
    vsnd_deb2e(THF_SEM, TFN_REF_SEM, (long)semid, (long)pk_rsem, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V4_CRE_FLG
/************************************************/
/* Create EventFlag                             */
/************************************************/

ER v4_cre_flg(ID flgid, const V4_T_CFLG *pk_cflg)
{
    T_FLG SNEAR *flg;
    int i, n, atid = (flgid == ID_AUTO);
    UINT size, coff;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)flgid > (UINT)CFG.flgid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_FLG, TFN_CRE_FLG, (long)flgid, (long)pk_cflg);
  #endif
    if (atid)
    {
        for (flgid = CFG.flgid_max;;)
        {   if (pFLG[flgid] == NULL)
                break;
            if (--flgid == 0)
            {   return_ret_ims2(THF_FLG, E_NOID);
            }
        }
        ercd = flgid;
    }
    else
    {   if (pFLG[flgid] != NULL)
        {   return_ret_ims2(THF_FLG, E_OBJ);
        }
        ercd = E_OK;
    }
    if (pk_cflg->flgatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_FLG) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_cflg))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CFLG);
    }
    else
        coff = 0;
    flg = sys_alloc((SIZE)size);
    if (flg == NULL)
    {   return_ret_ims2(THF_FLG, SYSER = E_SYS);
    }
    pFLG[flgid] = flg;
    if (coff != 0)
    {   flg->cflg = (const V4_T_CFLG *)((ADDR)flg + coff);
        osmem_cpy((DFAR B*)flg->cflg,(DFAR B*)pk_cflg,sizeof(*pk_cflg));
    }
    else
        flg->cflg = pk_cflg;
    flg->ptn = pk_cflg->iflgptn;
    flg->wmul = (B)pk_cflg->flgatr;
    for(i = 0; i <= n; i++)
        flg->que[i] = 0;
    flg->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_FLG, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_FLG, TFN_CRE_FLG, (long)flgid, (long)pk_cflg, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_FLG
/************************************************/
/* Create EventFlag                             */
/************************************************/

ER v3_cre_flg(ID flgid, const V3_T_CFLG *pk_cflg)
{
    V4_T_CFLG cflg;

    cflg.flgatr = pk_cflg->flgatr;
    cflg.iflgptn = (FLGPTN)pk_cflg->iflgptn;
    cflg.name = &_KERNEL_NNM;

    return v4_cre_flg(flgid, (const V4_T_CFLG *)&cflg);

}

#endif
#ifdef V4_DEL_FLG
/************************************************/
/* Delete EventFlag                             */
/************************************************/

ER v4_del_flg(ID flgid)
{
    T_FLG SNEAR *flg;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    UINT size;
    int disp = FALSE;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_FLG, TFN_DEL_FLG, (long)flgid);
  #endif
    flg = pFLG[flgid];
    if (flg == NULL)
    {   return_ret_ims2(THF_FLG, E_NOEXS);
    }
    que = flg->que;
    for(;*que < TSK_QUETAIL;++que)
    {
        for (;(tskid = *que) != 0;)
        {   tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {   chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
        }
    }
    size = sizeof(T_FLG) + ((*que > TSK_QUETAIL)?((CFG.tpri_max - 1)*sizeof(TSK_ID)):0);
    if (!_kernel_inROM((const void *)flg->cflg))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CFLG);
    }
    sys_free(flg, (SIZE)size);
    pFLG[flgid] = NULL;
    if (disp)
    {   return_dispatch(THF_FLG);
    }
    else
    {   return_ret_ims(THF_FLG);
    }
  #if ERC
    }
    vsnd_deb1e(THF_FLG, TFN_DEL_FLG, (long)flgid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_SET_FLG
/************************************************/
/* Set EventFlag                                */
/************************************************/

static ER FNEAR set_flg_sub(T_FLG SNEAR *flg, UINT setptn, TSK_ID SNEAR *que)
{
    T_TCB SNEAR *tcb;
    TSK_ID nxt, end, tskid;
    INT sts, disp = FALSE;
    TSK_PRI pri;
    UINT waiptn;

    setptn |= flg->ptn;
    do {
        tskid = *que;
        tcb = pTCB[tskid];
        end = tcb->pid;
        for(;;) {
            nxt = tcb->nid;
            sts = tcb->sts;
            if (!(sts & S_ORW))
            {   waiptn = (UINT)tcb->sp->ptn;
                if ((UINT)(waiptn & setptn) == waiptn)
                {
                L2: *(UINT PFAR *)(tcb->sp->ptr) = setptn;
                    if (sts & S_TMO)
                        del_tmq(tcb, tskid);
                    tcb->sts = S_RDY;
                    if (tcb->sus == 0)
                    {   pri = tcb->pri;
                        if (pri < NOWPRI)
                            disp |= tcb->pri < NOWPRI;
                        chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
                    }
                    else
                        del_que(tcb, tskid);
                    if (sts & S_CLR)
                    {   flg->ptn = 0;
                        if (disp)
                        {   return_dispatch(THF_FLG);
                        }
                        return_ret_ims(THF_FLG);
                    }
                }
            }
            else
            {   if (tcb->sp->ptn & setptn)
                    goto L2;
            }
            if (end == tskid)
                break;
            else
            {   tskid = nxt;
                tcb = pTCB[tskid];
            }
        }
        while(*(++que) == 0);
    } while (*que < TSK_QUETAIL);
    flg->ptn = setptn;
    if (disp)
    {   return_dispatch(THF_FLG);
    }
    return_ret_ims(THF_FLG);
}

ER v4_set_flg(ID flgid, FLGPTN setptn)
{
    T_FLG SNEAR *flg;
    T_TCB SNEAR *tcb;
    ID tskid;
    int sts;
    UINT waiptn;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if (setptn == 0)
        ercd = E_OK;
    else if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_FLG, TFN_SET_FLG, (long)flgid, (long)setptn);
  #endif
    flg = pFLG[flgid];
    if (flg == NULL)
    {   return_ret_ims2(THF_FLG, E_NOEXS);
    }
    que = flg->que;
    for(;;que++)
    {   tskid = *que;
        if (tskid != 0)
            break;
    }
    if (tskid < TSK_QUETAIL)
    {   tcb = pTCB[tskid];
        if ((tcb->pid == (TSK_ID)tskid) && !(flg->wmul & TA_WMUL))
        {   sts = tcb->sts;
            if (!(sts & S_ORW))
            {   waiptn = (UINT)tcb->sp->ptn;
                setptn |= flg->ptn;
                if ((UINT)(waiptn & setptn) == waiptn)
                {
                L1: *(UINT PFAR *)(tcb->sp->ptr) = setptn;
                    if (sts & S_CLR)
                        setptn = 0;
                    flg->ptn = setptn;
                    if (sts & S_TMO)
                        del_tmq(tcb, tskid);
                    tcb->sts = S_RDY;
                    if (tcb->sus == 0)
                        return chg_rdq_dispatch_(THF_FLG, tcb, tskid);
                    else
                        return del_que_ret_ims_(THF_FLG, tcb, tskid);
                }
            }
            else
            {   setptn |= flg->ptn;
                if (tcb->sp->ptn & setptn)
                    goto L1;
            }
            flg->ptn = setptn;
            return_ret_ims(THF_FLG);
        }
        return set_flg_sub(flg, setptn, que);
    }
    flg->ptn |= setptn;
    return_ret_ims(THF_FLG);
  #if ERC
    }
    vsnd_deb2e(THF_FLG, TFN_SET_FLG, (long)flgid, (long)setptn, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_CLR_FLG
/************************************************/
/* Clear EventFlag                              */
/************************************************/

ER v4_clr_flg(ID flgid, FLGPTN clrptn)
{
    T_FLG SNEAR *flg;
  #if ERC
    ER ercd;

    if (clrptn == (FLGPTN) -1)
        ercd = E_OK;
    else if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_FLG, TFN_CLR_FLG, (long)flgid, (long)clrptn);
  #endif
    flg = pFLG[flgid];
    if (flg == NULL)
    {   return_ret_ims2(THF_FLG, E_NOEXS);
    }
    flg->ptn &= clrptn;
    return_ret_ims(THF_FLG);
  #if ERC
    }
    vsnd_deb2e(THF_FLG, TFN_CLR_FLG, (long)flgid, (long)clrptn, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TWAI_FLG
/************************************************/
/* Wait EventFlag with Timeout                  */
/************************************************/

ER v4_twai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, UINT *p_flgptn, TMO tmout)
{
    T_FLG SNEAR *flg;
    T_TCB SNEAR *tcb;
    ID tskid;
    FLGPTN ptn;
    TSK_ID SNEAR *que;
  #if ERC
    ER ercd;

    if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else if ((waiptn == 0)
          || (wfmode & ~(S_ORW|S_CLR)) || (tmout < (-2)))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb5(THF_FLG, ((tmout==TMO_FEVR)?TFN_WAI_FLG:((tmout==TMO_POL)?TFN_POL_FLG:TFN_TWAI_FLG)), (long)flgid, (long)waiptn, (long)wfmode, (long)p_flgptn, (long)tmout);
  #endif
    flg = pFLG[flgid];
    if (flg == NULL)
    {   return_ret_ims2(THF_FLG, E_NOEXS);
    }
    if (!(flg->wmul & TA_WMUL) && *flg->que)
    {   return_ret_ims2(THF_FLG, E_ILUSE);
    }
    ptn = flg->ptn;
    if (!(wfmode & TWF_ORW))
    {   if ((UINT)(ptn & waiptn) != waiptn)
        {
    L1:
            if (tmout != TMO_POL)
            {
              #if ERC
                if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
                {   return_ret_ims2(THF_FLG, SYSER = E_CTX);
                }
              #endif
                tskid = RDQ[0];
                tcb = pTCB[tskid];
                if (tmout != TMO_FEVR)
                {   add_tmq(tmout, tcb, tskid);
                    tcb->sts = (UB)(wfmode | (S_FLG|S_TMO) | (flg->wmul & TA_CLR));
                }
                else
                    tcb->sts = (UB)(wfmode | S_FLG | (flg->wmul & TA_CLR));
                CTXPTN = waiptn;
                CTXPTR = p_flgptn;
                set_wid(flgid);
                que = flg->que;
                if (*(que + 1) < TSK_QUETAIL)
                    que += NOWPRI - 1;
                return chg_que_dispatch1_(THF_FLG, que, tcb, tskid);
            }
            return_ret_ims2(THF_FLG, E_TMOUT);
        }
    }
    else
    {   if ((ptn & waiptn) == 0)
            goto L1;
    }
    *p_flgptn = ptn;
    if ((wfmode & S_CLR) || (flg->wmul & TA_CLR))
        flg->ptn = 0;
    return_ret_ims(THF_FLG);
  #if ERC
    }
    vsnd_deb5e(THF_FLG, ((tmout==TMO_FEVR)?TFN_WAI_FLG:((tmout==TMO_POL)?TFN_POL_FLG:TFN_TWAI_FLG)), (long)flgid, (long)waiptn, (long)wfmode, (long)p_flgptn, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_FLG
/************************************************/
/* Refer EventFlag Status                       */
/************************************************/

ER v4_ref_flg(ID flgid, V4_T_RFLG *pk_rflg)
{
    T_FLG SNEAR *flg;
    TSK_ID SNEAR *que;
    TSK_ID tskid;
  #if ERC
    ER ercd;

    if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_FLG, TFN_REF_FLG, (long)flgid, (long)pk_rflg);
      #endif
        flg = pFLG[flgid];
        if (flg == NULL)
        {   return_ret_ims2(THF_FLG, E_NOEXS);
        }
        pk_rflg->flgptn = flg->ptn;
        que = flg->que;
        for(;;que++)
        {   tskid = *que;
            if(tskid != 0)
                break;
        }
        pk_rflg->wtskid = (tskid >= TSK_QUETAIL)?TSK_NONE:tskid;
        return_ret_ims(THF_FLG);
  #if ERC
    }
    vsnd_deb2e(THF_FLG, TFN_REF_FLG, (long)flgid, (long)pk_rflg, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_FLG
/************************************************/
/* Refer EventFlag Status                       */
/************************************************/

ER v3_ref_flg(V3_T_RFLG *pk_rflg, ID flgid)
{
    T_FLG SNEAR *flg;
  #if ERC
    ER ercd;

    if ((flgid == 0) || ((UINT)flgid > (UINT)CFG.flgid_max))
        ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_FLG, TFN_REF_FLG, (long)flgid, (long)pk_rflg);
      #endif
        flg = pFLG[flgid];
        if (flg == NULL)
        {   return_ret_ims2(THF_FLG, E_NOEXS);
        }
        pk_rflg->exinf = NULL;
        pk_rflg->flgptn = (FLGPTN)flg->ptn;
        pk_rflg->wtsk = flg->que[0];
        return_ret_ims(THF_FLG);
  #if ERC
    }
    vsnd_deb2e(THF_FLG, TFN_REF_FLG, (long)flgid, (long)pk_rflg, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V4_CRE_DTQ
/************************************************/
/* Create Data Queue                            */
/************************************************/

ER v4_cre_dtq(ID dtqid, const V4_T_CDTQ* pk_cdtq)
{
    T_DTQ SNEAR *dtq;
    int n, i, qsize, atid = (dtqid == ID_AUTO);
    UINT size, toff = 0;
    ADDR top, coff;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)dtqid > (UINT)CFG.dtqid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_DTQ, TFN_CRE_DTQ, (long)dtqid, (long)pk_cdtq);
  #endif
    if (atid)
    {   for (dtqid = CFG.dtqid_max;;)
        {   if (pDTQ[dtqid] == NULL)
                break;
            if (--dtqid == 0)
            {   return_ret_ims2(THF_DTQ, E_NOID);
            }
        }
        ercd = dtqid;
    }
    else
    {   if (pDTQ[dtqid] != NULL)
        {   return_ret_ims2(THF_DTQ, E_OBJ);
        }
        ercd = E_OK;
    }
    if (pk_cdtq->dtqatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_DTQ) + n * sizeof(TSK_ID);

    if ((qsize = pk_cdtq->dtqcnt) != 0)
    {   if ((top = (ADDR)pk_cdtq->dtq) == (ADDR)NULL)
        {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
            toff = size;
            size = toff + pk_cdtq->dtqcnt * sizeof(VP);
        }
    }
    else
    {   qsize = 0;
        top = 0;
    }

    if (!_kernel_inROM((const void *)pk_cdtq))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CDTQ);
    }
    else
        coff = 0;
    dtq = sys_alloc((SIZE)size);
    if (dtq == NULL)
    {   return_ret_ims2(THF_DTQ, SYSER = E_SYS);
    }
    pDTQ[dtqid] = dtq;
    if (coff != 0)
    {   dtq->cdtq = (const V4_T_CDTQ *)((ADDR)dtq + coff);
        osmem_cpy((DFAR B*)dtq->cdtq,(DFAR B*)pk_cdtq,sizeof(*pk_cdtq));
    }
    else
        dtq->cdtq = pk_cdtq;
    if (toff != 0)
        top =  ((ADDR)dtq + toff);
    dtq->top = (VP_INT)top;
    dtq->put = (VP_INT)top;
    dtq->get = (VP_INT)top;
    dtq->btm = (VP_INT)((VP_INT*)top + qsize);
    dtq->size = qsize;
    dtq->cnt = 0;
    dtq->rque = TSK_NONE;
    dtq->dmmy = TSK_QUETAIL;
    for (i = 0; i <= n; i++)
        dtq->sque[i] = TSK_NONE;
    dtq->sque[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_DTQ, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_DTQ, TFN_CRE_DTQ, (long)dtqid, (long)pk_cdtq, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_DEL_DTQ
/************************************************/
/* Delete Data Queue                            */
/************************************************/

ER v4_del_dtq(ID dtqid)
{
    T_DTQ SNEAR *dtq;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int disp = FALSE;
    int size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((dtqid == 0) || ((UINT)dtqid > (UINT)CFG.dtqid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_DTQ, TFN_DEL_DTQ, (long)dtqid);
  #endif
    dtq = pDTQ[dtqid];
    if (dtq == NULL)
    {   return_ret_ims2(THF_DTQ, E_NOEXS);
    }
    if (dtq->cnt == dtq->size)
    {   que = dtq->sque;
        tskid = (ID)*que;
        for (;;)
        {   for (;;)
            {   if (tskid == 0)
                    break;
                tcb = pTCB[tskid];
                tcb->sp->ercd = E_DLT;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   chg_rdq(tcb, tskid);
                    disp |= tcb->pri < NOWPRI;
                }
                else
                    del_que(tcb, tskid);
                tskid = *que;
            }
            tskid = *++que;
            if (tskid >= TSK_QUETAIL)
                break;
        }
    }
    if (dtq->cnt == 0)
    {  que = &dtq->rque;
        for(;(tskid = *que) != 0;)
        {   tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {   chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
        }
    }
    size = sizeof (T_DTQ);
    if (dtq->sque[1] < TSK_QUETAIL)
        size += (CFG.tpri_max - 1)*sizeof(TSK_ID);

    if ((dtq->cdtq->dtq) == NULL)
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + dtq->cdtq->dtqcnt * sizeof(VP);
    }

    if (!_kernel_inROM((const void *)dtq->cdtq))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CDTQ);
    }
    sys_free(dtq, (SIZE)size);
    pDTQ[dtqid] = NULL;
    if (disp == TRUE)
    {   return_dispatch(THF_DTQ);
    }
    else
    {   return_ret_ims(THF_DTQ);
    }
  #if ERC
    }
    vsnd_deb1e(THF_DTQ, TFN_DEL_DTQ, (long)dtqid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_SND_DTQ
/************************************************/
/* Send Message to Data Queue                   */
/************************************************/

ER v4_tsnd_dtq(ID dtqid, VP_INT data, TMO tmout)
{
    T_DTQ SNEAR *dtq;
    T_TCB SNEAR *tcb;
    ID tskid;
    TSK_ID SNEAR *que;
    VP_INT PFAR *p;
  #if ERC
    ER ercd;

    if ((dtqid == 0) || ((UINT)dtqid > (UINT)CFG.dtqid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_DTQ, TFN_SND_DTQ, (long)dtqid, (long)data);
  #endif
    dtq = pDTQ[dtqid];
    if (dtq == NULL)
    {   return_ret_ims2(THF_DTQ, E_NOEXS);
    }
    if (dtq->cnt == 0)
    {   tskid = dtq->rque;
        if (tskid != TSK_NONE)
        {   tcb = pTCB[tskid];
            *(VP_INT PFAR *)tcb->sp->ptr = data;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
                return chg_rdq_dispatch_(THF_DTQ, tcb, tskid);
            else
                return del_que_ret_ims_(THF_DTQ, tcb, tskid);
        }
    }
    if (dtq->cnt == dtq->size)
    {   /* add task to sque */
        if (tmout != TMO_POL)
        {
            #if ERC
            if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_DTQ, SYSER = E_CTX);
            }
            #endif
            tskid = RDQ[0];
            tcb = pTCB[tskid];
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB) (S_SDT|S_TMO);
            }
            else
                tcb->sts = S_SDT;
            CTXPTR = data;
            set_wid(dtqid);
            que = dtq->sque;
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            return chg_que_dispatch1_(THF_DTQ, que, tcb, tskid);
        }
        return_ret_ims2(THF_DTQ, E_TMOUT);
    }
    else
    {   p = dtq->put;
        *p = data;
        dtq->cnt++;
        if ((ADDR)++p >= (ADDR)dtq->btm)
            dtq->put = dtq->top;
        else
            dtq->put = p;
    }
    return_ret_ims(THF_DTQ);
  #if ERC
    }
    vsnd_deb2e(THF_DTQ, TFN_SND_DTQ, (long)dtqid, (long)data, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_FSND_DTQ
/************************************************/
/* Send Message to Data Queue                   */
/************************************************/

ER v4_fsnd_dtq(ID dtqid, VP_INT data)
{
    T_DTQ SNEAR *dtq;
    T_TCB SNEAR *tcb;
    ID tskid;
    VP_INT PFAR *p;
  #if ERC
    ER ercd;

    if ((dtqid == 0) || ((UINT)dtqid > (UINT)CFG.dtqid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_DTQ, TFN_FSND_DTQ, (long)dtqid, (long)data);
  #endif
    dtq = pDTQ[dtqid];
    if (dtq == NULL)
    {   return_ret_ims2(THF_DTQ, E_NOEXS);
    }
    else if(dtq->size == 0)
    {   return_ret_ims2(THF_DTQ, E_ILUSE);
    }
    if ((tskid = dtq->rque) != TSK_NONE)
    {   tcb = pTCB[tskid];
        *(VP_INT PFAR *)tcb->sp->ptr = data;
        if (tcb->sts & S_TMO)
            del_tmq(tcb, tskid);
        tcb->sts = S_RDY;
        if (tcb->sus == 0)
            return chg_rdq_dispatch_(THF_DTQ, tcb, tskid);
        else
            return del_que_ret_ims_(THF_DTQ, tcb, tskid);
    }
    if (dtq->cnt == dtq->size)
    {   p = dtq->get;
        if ((ADDR)++p >= (ADDR)dtq->btm)
            dtq->get = dtq->top;
        else
            dtq->get = p;
    }
    else
        dtq->cnt++;
    p = dtq->put;
    *p = data;
    if ((ADDR)++p >= (ADDR)dtq->btm)
        dtq->put = dtq->top;
    else
        dtq->put = p;
    return_ret_ims(THF_DTQ);
  #if ERC
    }
    vsnd_deb2e(THF_DTQ, TFN_FSND_DTQ, (long)dtqid, (long)data, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TRCV_DTQ
/************************************************/
/* Receive Message from Data Queue with Timeout */
/************************************************/

ER v4_trcv_dtq(ID dtqid, VP_INT *pdata, TMO tmout)
{
    T_DTQ SNEAR *dtq;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    VP_INT PFAR *p;
    ID tskid;
  #if ERC
    ER ercd;

    if ((dtqid == 0) || ((UINT)dtqid > (UINT)CFG.dtqid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb3(THF_DTQ, ((tmout==TMO_FEVR)?TFN_RCV_DTQ:((tmout==TMO_POL)?TFN_PRCV_DTQ:TFN_TRCV_DTQ)), (long)dtqid, (long)pdata, (long)tmout);
  #endif
    dtq = pDTQ[dtqid];
    if (dtq == NULL)
    {   return_ret_ims2(THF_DTQ, E_NOEXS);
    }
    if (dtq->cnt == 0)
    {
        que = dtq->sque;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
                break;
        }
        if (tskid >= TSK_QUETAIL)
        {
            if (tmout != TMO_POL)
            {
              #if ERC
                if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
                {   return_ret_ims2(THF_DTQ, SYSER = E_CTX);
                }
              #endif
                tskid = RDQ[0];
                tcb = pTCB[tskid];
                if (tmout != TMO_FEVR)
                {   add_tmq(tmout, tcb, tskid);
                    tcb->sts = (UB)(S_RDT|S_TMO);
                }
                else
                    tcb->sts = S_RDT;
                CTXPTR = pdata;
                set_wid(dtqid);
                que = &dtq->rque;
                return chg_que_dispatch1_(THF_DTQ, que, tcb, tskid);
            }
            return_ret_ims2(THF_DTQ, E_TMOUT);
        }
        else
        {
            tcb = pTCB[tskid];
            *pdata = tcb->sp->ptr;
            goto L1;
        }
    }
    p = dtq->get;
    *pdata = *p;
    dtq->cnt--;
    if ((ADDR)++p >= (ADDR)dtq->btm)
        dtq->get = dtq->top;
    else
        dtq->get = p;
    que = dtq->sque;
    for(;;)
    {   tskid = *que++;
        if (tskid != TSK_NONE)
        {   if (tskid >= TSK_QUETAIL)
                break;
            tcb = pTCB[tskid];
            p = dtq->put;
            *p = tcb->sp->ptr;
            dtq->cnt++;
            if ((ADDR)++p >= (ADDR)dtq->btm)
                dtq->put = dtq->top;
            else
                dtq->put = p;
L1:         if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
               return chg_rdq_dispatch_(THF_DTQ, tcb, tskid);
            else
               return del_que_ret_ims_(THF_DTQ, tcb, tskid);
         }
    }
    return_ret_ims(THF_DTQ);
  #if ERC
    }
    vsnd_deb3e(THF_DTQ, ((tmout==TMO_FEVR)?TFN_RCV_DTQ:((tmout==TMO_POL)?TFN_PRCV_DTQ:TFN_TRCV_DTQ)), (long)dtqid, (long)pdata, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_DTQ
/************************************************/
/* Refer Data Queue Status                      */
/************************************************/

ER v4_ref_dtq(ID dtqid, V4_T_RDTQ *pk_rdtq)
{
    T_DTQ SNEAR *dtq;
    TSK_ID SNEAR *que;
    ID tskid;
    ER ercd;

  #if ERC
    if (dtqid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if ((UINT)dtqid > (UINT)CFG.dtqid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_DTQ, TFN_REF_DTQ, (long)dtqid, (long)pk_rdtq);
      #endif
        dtq = pDTQ[dtqid];
        if (dtq == NULL)
        {   return_ret_ims2(THF_DTQ, E_NOEXS);
        }
        pk_rdtq->sdtqcnt = dtq->cnt;
        pk_rdtq->rtskid = dtq->rque;
        que = dtq->sque;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    pk_rdtq->stskid = TSK_NONE;
                else
                    pk_rdtq->stskid = tskid;
                return_ret_ims(THF_DTQ);
            }
        }
    }
  #if ERC
    vsnd_deb2e(THF_DTQ, TFN_REF_DTQ, (long)dtqid, (long)pk_rdtq, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_CRE_MTX
/************************************************/
/* Create MUtex                                 */
/************************************************/
ER v4_cre_mtx(ID mtxid, const V4_T_CMTX *pk_cmtx)
{
    T_MTX SNEAR *mtx;
    int ceilpri, atr;
    int i, n, atid = (mtxid == ID_AUTO);
    UINT size, coff;
    ER ercd;

    atr = pk_cmtx->mtxatr;
    ceilpri = (atr == TA_CEILING)? pk_cmtx->ceilpri : CFG.tpri_max;
  #if ERC
    if (INEST)
        ercd = E_CTX;
    else if ((UINT)mtxid > (UINT)CFG.mtxid_max)
        ercd = E_ID;
    else if (atr > TA_CEILING)
        ercd = E_RSATR;
    else if ((atr == TA_CEILING) &&
            (ceilpri == 0 || (UINT)ceilpri > (UINT)CFG.tpri_max))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MTX, TFN_CRE_MTX, (long)mtxid, (long)pk_cmtx);
  #endif
    if (atid)
    {   for (mtxid = CFG.mtxid_max; pMTX[mtxid] != NULL;)
        {   if (--mtxid == 0)
            {   return_ret_ims2(THF_MTX, E_NOID);
            }
        }
        ercd = mtxid;
    }
    else
    {   if (pMTX[mtxid] != NULL)
        {   return_ret_ims2(THF_MTX, E_OBJ);
        }
        ercd = E_OK;
    }
    if (atr != TA_TFIFO)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_MTX) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_cmtx))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CMTX);
    }
    else
        coff = 0;
    mtx = sys_alloc((SIZE)size);
    if (mtx == NULL)
    {   return_ret_ims2(THF_MTX, SYSER = E_SYS);
    }
    pMTX[mtxid] = mtx;
    if (coff != 0)
    {   mtx->cmtx = (const V4_T_CMTX *)((ADDR)mtx + coff);
        osmem_cpy((DFAR B*)mtx->cmtx,(DFAR B*)pk_cmtx,sizeof(*pk_cmtx));
    }
    else
        mtx->cmtx = pk_cmtx;
    mtx->ceil = (atr < TA_INHERIT)? 0:(TSK_PRI)ceilpri;
    mtx->occ = (TSK_ID)0;
    mtx->nid = (OBJ_ID)0;
    mtx->atr = atr;
    for (i = 0; i <= n; i++)
        mtx->que[i] = 0;
    mtx->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_MTX, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_MTX, TFN_CRE_MTX, (long)mtxid, (long)pk_cmtx, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_DEL_MTX
/************************************************/
/* Delete Mutex                                 */
/************************************************/

ER v4_del_mtx(ID mtxid)
{
    T_MTX SNEAR *mtx;
    T_TCB SNEAR *tcb;
    ID tskid, nid;
    TSK_ID SNEAR *que;
    int disp = FALSE;
    UINT size;

  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (mtxid == 0)
        ercd = E_ID;
    else if ((UINT)mtxid > (UINT)CFG.mtxid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MTX, TFN_DEL_MTX, (long)mtxid);
  #endif
    mtx = pMTX[mtxid];
    if (mtx == NULL)
    {   return_ret_ims2(THF_MTX, E_NOEXS);
    }
    que = mtx->que;
    tskid = *que;
    for (;;)
    {   for (;;)
        {   if (tskid == 0)
                break;
            tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {   chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
            tskid = *que;
        }
        tskid = *++que;
        if (tskid >= TSK_QUETAIL)
            break;
    }
    if ((tskid = mtx->occ) != 0)
    {   tcb = pTCB[tskid];

        if (tcb->lock == mtxid)
        {
            tcb->lock = mtx->nid;
        }
        else
        {
            for(nid = tcb->lock;
                mtxid != pMTX[nid]->nid;
                nid = pMTX[nid]->nid);
            pMTX[nid]->nid = mtx->nid;
        }
    }
    size = sizeof (T_MTX);
    if (mtx->que[1] < TSK_QUETAIL)
        size = size + (CFG.tpri_max - 1)*sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)mtx->cmtx))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CMTX);
    }
    sys_free(mtx, (SIZE)size);
    pMTX[mtxid] = NULL;
    if (disp == TRUE)
    {   return_dispatch(THF_MTX);
    }
    else
    {   return_ret_ims(THF_MTX);
    }
  #if ERC
    }
    vsnd_deb1e(THF_MTX, TFN_DEL_MTX, (long)mtxid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_UNL_MTX
/************************************************/
/* Unlock Mutex                                 */
/************************************************/

ER v4_unl_mtx(ID mtxid)
{
    T_MTX SNEAR *mtx, *mtx2;
    T_TCB SNEAR *tcb;
    ID tskid;
    OBJ_ID SNEAR *queo;
    PRI ceil, newpri;
    BOOL disp;
  #if ERC
    ER ercd;

    if ((mtxid == 0) || ((UINT)mtxid > (UINT)CFG.mtxid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MTX, TFN_UNL_MTX, (long)mtxid);
  #endif
    mtx = pMTX[mtxid];
    if (mtx == NULL)
    {   return_ret_ims2(THF_MTX, E_NOEXS);
    }
    else if (mtx->occ != (tskid = RDQ[0]))
    {   return_ret_ims2(THF_MTX, E_ILUSE);
    }
    ceil = mtx->ceil;
    tcb = pTCB[tskid];
    newpri = tcb->bpri;
    for(queo = &tcb->lock;
        *queo != 0; )
    {   if (*queo == mtxid)
        {   *queo = mtx->nid;
            if (ceil)
                continue;
            else
                break;
        }
        mtx2 = pMTX[*queo];
        if ((mtx2->ceil != 0) && (newpri > mtx2->ceil))
            newpri = mtx2->ceil;
        queo = &mtx2->nid;
    }
    if (newpri != tcb->pri)
    {   chg_que((TSK_ID SNEAR *)RDQ + newpri, tcb, tskid);
        disp = newpri > NOWPRI;
        tcb->pri = newpri;
    }
    else
        disp = FALSE;
    if (disp | wup_tsk_mtx(mtx, mtxid))
    {   return_dispatch1(THF_MTX);
    }
    return_ret_ims(THF_MTX);
  #if ERC
    }
    vsnd_deb1e(THF_MTX, TFN_UNL_MTX, (long)mtxid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TLOC_MTX
/************************************************/
/* Lock Mutex with Timeout                      */
/************************************************/

ER v4_tloc_mtx(ID mtxid, TMO tmout)
{
    T_MTX SNEAR *mtx;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    ID nid;
    ATR atr;
    PRI ceil;
  #if ERC
    ER ercd;

    if ((mtxid == 0) || ((UINT)mtxid > (UINT)CFG.mtxid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MTX, ((tmout==TMO_FEVR)?TFN_LOC_MTX:((tmout==TMO_POL)?TFN_PLOC_MTX:TFN_TLOC_MTX)), (long)mtxid, (long)tmout);
  #endif
    mtx = pMTX[mtxid];
    if (mtx == NULL)
    {   return_ret_ims2(THF_MTX, E_NOEXS);
    }
    if (mtx->occ == (tskid = RDQ[0]))
    {   return_ret_ims2(THF_MTX, E_ILUSE);
    }
    tcb = pTCB[tskid];
    atr = mtx->atr;
    ceil = mtx->ceil;
    if ((atr == TA_CEILING) && (ceil > tcb->bpri))
    {   return_ret_ims2(THF_MTX, E_ILUSE);
    }
    if (mtx->occ == 0)
    {   mtx->occ = tskid;
        if ((nid = tcb->lock) == 0)
            ;
        tcb->lock = mtxid;
        mtx->nid = nid;
        if (atr == TA_INHERIT)
            mtx->ceil = tcb->pri;
        else if ((atr == TA_CEILING) && (tcb->pri > ceil))
        {   tcb->pri = ceil;
            return chg_que_dispatch1_(THF_MTX, (TSK_ID SNEAR *)RDQ + ceil, tcb, tskid);
        }
    }
    else
    {
        if (tmout != TMO_POL)
        {
          #if ERC
            if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_MTX, SYSER = E_CTX);
            }
          #endif
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_MTX|S_TMO);
            }
            else
                tcb->sts = S_MTX;
            set_wid(mtxid);
            que = mtx->que;
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            chg_que(que, tcb, tskid);
            if ((atr == TA_INHERIT) && (ceil > NOWPRI))
            {   mtx->ceil = NOWPRI;
                reset_occ_cpri(mtx->occ);
            }
            return_dispatch1(THF_MTX);
        }
        return_ret_ims2(THF_MTX, E_TMOUT);
    }
    return_ret_ims(THF_MTX);
  #if ERC
    }
    vsnd_deb2e(THF_MTX, ((tmout==TMO_FEVR)?TFN_LOC_MTX:((tmout==TMO_POL)?TFN_PLOC_MTX:TFN_TLOC_MTX)), (long)mtxid, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_MTX
/************************************************/
/* Refer Mutex Status                           */
/************************************************/

ER v4_ref_mtx(ID mtxid, V4_T_RMTX *pk_rmtx)
{
    T_MTX SNEAR *mtx;
    TSK_ID SNEAR *que;
    ID tskid;
  #if ERC
    ER ercd;

    if ((mtxid == 0) || ((UINT)mtxid > (UINT)CFG.mtxid_max))
        SYSER = ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_MTX, TFN_REF_MTX, (long)mtxid, (long)pk_rmtx);
      #endif
        mtx = pMTX[mtxid];
        if (mtx == NULL)
        {   return_ret_ims2(THF_MTX, E_NOEXS);
        }
        pk_rmtx->htskid = mtx->occ;
        que = mtx->que;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                pk_rmtx->wtskid = tskid;
                return_ret_ims(THF_MTX);
            }
        }
        pk_rmtx->wtskid = TSK_NONE;
        return_ret_ims(THF_MTX);
   #if ERC
   }
    vsnd_deb2e(THF_MTX, TFN_REF_MTX, (long)mtxid, (long)pk_rmtx, (long)ercd);
    return ercd;
  #endif
}

#endif

#ifdef V4_CRE_MBX
/************************************************/
/* Create Mailbox                               */
/************************************************/
ER v4_cre_mbx(ID mbxid, const V4_T_CMBX* pk_cmbx)
{
    T_MBX SNEAR *mbx;
    V4_T_MSGHDR PFAR *mque;
    int n, ns, i, moff, coff, mpri;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)mbxid > (UINT)CFG.mbxid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MBX, TFN_CRE_MBX, (long)mbxid, (long)pk_cmbx);
  #endif
    if (mbxid == ID_AUTO)
    {   if ((mbxid = _kernel_mbxfr) == 0)
        {   return_ret_ims2(THF_MBX, E_NOID);
        }
        else
        {   for(;pMBX[mbxid] != NULL;)
            {   if (--mbxid == 0)
                {   return_ret_ims2(THF_MBX, E_NOID);
                }
            }
            ercd = mbxid;
            _kernel_mbxfr = mbxid - 1;
        }
    }
    else
    {   if (pMBX[mbxid] != NULL)
        {   return_ret_ims2(THF_MBX, E_OBJ);
        }
        ercd = E_OK;
    }

    if (pk_cmbx->mbxatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    ns = sizeof (T_MBX) + n * sizeof(TSK_ID);

    if (pk_cmbx->mbxatr & TA_MPRI)
        mpri = pk_cmbx->maxmpri;
    else
        mpri = 1;

    if (pk_cmbx->mprihd == NULL)
    {   ns = (ns + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        moff = ns;
        ns = moff + TSZ_MPRIHD(mpri);
    }
    else
        moff = 0;
    if (!_kernel_inROM((const void *)pk_cmbx))
    {   ns = (ns + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = ns;
        ns = ns + sizeof(V4_T_CMBX);
    }
    else
        coff = 0;
    mbx = sys_alloc((SIZE)ns);
    if (mbx == NULL)
    {   return_ret_ims2(THF_MBX, SYSER = E_SYS);
    }
    pMBX[mbxid] = mbx;
    mbx->mpri = (UB)(mpri);
    mbx->size = (UB)ns;
    mbx->moff = (UB)moff;
    mque = (moff != 0)?(V4_T_MSGHDR PFAR *)((ADDR)mbx + moff):(V4_T_MSGHDR PFAR *)pk_cmbx->mprihd;
    if (coff != 0)
    {   mbx->cmbx = (const V4_T_CMBX *)((ADDR)mbx + coff);
        osmem_cpy((DFAR B *)mbx->cmbx, (DFAR B *)pk_cmbx, sizeof(*pk_cmbx));
    }
    else
        mbx->cmbx = pk_cmbx;
    for (i = 0; i < mpri; i++,mque++)
    {   mque->top = NULL;
        mque->end = NULL;
    }
    for (i = 0; i <= n; i++)
        mbx->que[i] = 0;
    mbx->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_MBX, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_MBX, TFN_CRE_MBX, (long)mbxid, (long)pk_cmbx, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_MBX
/************************************************/
/* Create Mailbox                               */
/************************************************/

ER v3_cre_mbx(ID mbxid, const V3_T_CMBX* pk_cmbx)
{
    V4_T_CMBX v4_cmbx;

    v4_cmbx.mbxatr = pk_cmbx->mbxatr;
    if (pk_cmbx->mbxatr & TA_MPRI)
        v4_cmbx.maxmpri = CFG.tpri_max;
    else
        v4_cmbx.maxmpri = 1;
    v4_cmbx.mprihd = NULL;
    v4_cmbx.name = &_KERNEL_NNM;
    return v4_cre_mbx(mbxid, (const V4_T_CMBX *)&v4_cmbx);
}
#endif
#ifdef V4_DEL_MBX
/************************************************/
/* Delete Mailbox                               */
/************************************************/

ER v4_del_mbx(ID mbxid)
{
    T_MBX SNEAR *mbx;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int disp = 0;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((mbxid == 0) || ((UINT)mbxid > (UINT)CFG.mbxid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MBX, TFN_DEL_MBX, (long)mbxid);
  #endif
    mbx = pMBX[mbxid];
    if (mbx == NULL)
    {   return_ret_ims2(THF_MBX, E_NOEXS);
    }
    que = mbx->que;
    tskid = *que;
    for (;;)
    {   for (;;)
        {   if (tskid == 0)
                break;
            tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {   chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
            tskid = *que;
        }
        tskid = *++que;
        if (tskid >= TSK_QUETAIL)
            break;
    }

    sys_free(mbx, (SIZE)mbx->size);
    pMBX[mbxid] = NULL;
    if (mbxid > _kernel_mbxfr)
        _kernel_mbxfr = mbxid;
    if (disp)
    {   return_dispatch(THF_MBX);
    }
    return_ret_ims(THF_MBX);
  #if ERC
    }
    vsnd_deb1e(THF_MBX, TFN_DEL_MBX, (long)mbxid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_SND_MBX
/************************************************/
/* Send Message to Mailbox                      */
/************************************************/

ER v4_snd_mbx(ID mbxid, VP pk_msg)
{
    T_MBX SNEAR *mbx;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    V4_T_MSGHDR PFAR *mque;
    ID tskid;
  #if ERC
    ER ercd;

    if ((mbxid == 0) || ((UINT)mbxid > (UINT)CFG.mbxid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MBX, TFN_SND_MBX, (long)mbxid, (long)pk_msg);
  #endif
    mbx = pMBX[mbxid];
    if (mbx == NULL)
    {   return_ret_ims2(THF_MBX, E_NOEXS);
    }

    que = mbx->que;
    for (;;)
    {   tskid = *que++;
        if (tskid != 0)
        {   if (tskid >= TSK_QUETAIL)
                break;
            tcb = pTCB[tskid];
            *(V3_T_MSG PFAR * PFAR *)tcb->sp->ptr = (V3_T_MSG PFAR *)pk_msg;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
                return chg_rdq_dispatch_(THF_MBX, tcb, tskid);
            else
                return del_que_ret_ims_(THF_MBX, tcb, tskid);
        }
    }
    if (mbx->moff == 0)
        mque = (V4_T_MSGHDR PFAR *)(mbx->cmbx->mprihd);
    else
        mque = (V4_T_MSGHDR PFAR *)((ADDR)mbx + mbx->moff);
    if (mbx->mpri > 1)
    {
        #if ERC
        if ((((V3_T_MSG PFAR *)pk_msg)->msgpri > mbx->mpri) || (((V3_T_MSG PFAR *)pk_msg)->msgpri < TMIN_MPRI))
        {   return_ret_ims2(THF_MBX, E_PAR);
        }
        #endif
        mque = mque + ((V3_T_MSG PFAR *)pk_msg)->msgpri - 1;
    }
    if (mque->top != NULL)
        mque->end->next = (V3_T_MSG PFAR *)pk_msg;
    else
        mque->top = (V3_T_MSG PFAR *)pk_msg;
    mque->end = (V3_T_MSG PFAR *)pk_msg;
    ((V3_T_MSG PFAR *)pk_msg)->next = NULL;
    return_ret_ims(THF_MBX);
  #if ERC
    }
    vsnd_deb2e(THF_MBX, TFN_SND_MBX, (long)mbxid, (long)pk_msg, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TRCV_MBX
/************************************************/
/* Receive Message from Mailbox with Timeout    */
/************************************************/

ER v4_trcv_mbx(ID mbxid, VP ppk_msg, TMO tmout)
{
    T_MBX SNEAR *mbx;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    V3_T_MSG PFAR *top;
    V4_T_MSGHDR PFAR *mque;
    ID tskid;
    int i;
  #if ERC
    ER ercd;

    if ((mbxid == 0) || (mbxid > CFG.mbxid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb3(THF_MBX, ((tmout==TMO_FEVR)?TFN_RCV_MBX:((tmout==TMO_POL)?TFN_PRCV_MBX:TFN_TRCV_MBX)), (long)mbxid, (long)ppk_msg, (long)tmout);
  #endif
    mbx = pMBX[mbxid];
    if (mbx == NULL)
    {   return_ret_ims2(THF_MBX, E_NOEXS);
    }

    if (mbx->moff == 0)
        mque = (V4_T_MSGHDR PFAR *)mbx->cmbx->mprihd;
    else
        mque = (V4_T_MSGHDR PFAR *)((ADDR)mbx + mbx->moff);

    for(i = 0; i < mbx->mpri; i++, mque++)
    {   if (mque->top != NULL)
        {   top = (V3_T_MSG PFAR *)mque->top;
            mque->top = top->next;
            *(V3_T_MSG PFAR * PFAR *)ppk_msg = top;
            return_ret_ims(THF_MBX);
        }
    }
    if (tmout != TMO_POL)
    {
      #if ERC
        if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        {   return_ret_ims2(THF_MBX, SYSER = E_CTX);
        }
      #endif
        tskid = RDQ[0];
        tcb = pTCB[tskid];
        if (tmout != TMO_FEVR)
        {   add_tmq(tmout, tcb, tskid);
            tcb->sts = (UB)(S_MBX|S_TMO);
        }
        else
            tcb->sts = S_MBX;
        CTXPTR = ppk_msg;
        set_wid(mbxid);
        que = mbx->que;
        if (*(que + 1) < TSK_QUETAIL)
            que += NOWPRI - 1;
        return chg_que_dispatch1_(THF_MBX, que, tcb, tskid);
    }
    return_ret_ims2(THF_MBX, E_TMOUT);

  #if ERC
    }
    vsnd_deb3e(THF_MBX, ((tmout==TMO_FEVR)?TFN_RCV_MBX:((tmout==TMO_POL)?TFN_PRCV_MBX:TFN_TRCV_MBX)), (long)mbxid, (long)ppk_msg, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_MBX
/************************************************/
/* Refer Mailbox Status                         */
/************************************************/

ER v4_ref_mbx(ID mbxid, V4_T_RMBX *pk_rmbx)
{
    T_MBX SNEAR *mbx;
    TSK_ID SNEAR *que;
    V4_T_MSGHDR PFAR *mque;
    ID tskid;
    int i;
    ER ercd;

  #if ERC
    if (mbxid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if (mbxid > CFG.mbxid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MBX, TFN_REF_MBX, (long)mbxid, (long)pk_rmbx);
      #endif
        mbx = pMBX[mbxid];
        if (mbx == NULL)
        {   return_ret_ims2(THF_MBX, E_NOEXS);
        }

        if (mbx->moff == 0)
            mque = (V4_T_MSGHDR PFAR *)mbx->cmbx->mprihd;
        else
            mque = (V4_T_MSGHDR PFAR *)((ADDR)mbx + mbx->moff);

        for(i = 0; i < mbx->mpri; i++, mque++)
        {   if (mque->top != NULL)
            {   pk_rmbx->pk_msg = (T_MSG PFAR *)mque->top;
                pk_rmbx->wtskid = TSK_NONE;
                return_ret_ims(THF_MBX);
            }
        }
        pk_rmbx->pk_msg = NULL;
        que = mbx->que;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                pk_rmbx->wtskid = tskid;
                return_ret_ims(THF_MBX);
            }
        }
        pk_rmbx->wtskid = TSK_NONE;
        return_ret_ims(THF_MBX);
    }
  #if ERC
    vsnd_deb2e(THF_MBX, TFN_REF_MBX, (long)mbxid, (long)pk_rmbx, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V3_REF_MBX
/************************************************/
/* Refer Mailbox Status                         */
/************************************************/

ER v3_ref_mbx(V3_T_RMBX *pk_rmbx, ID mbxid)
{
    T_MBX SNEAR *mbx;
    TSK_ID SNEAR *que;
    V4_T_MSGHDR PFAR *mque;
    ID tskid;
    int i;
    ER ercd;

  #if ERC
    if (mbxid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if (mbxid > CFG.mbxid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MBX, TFN_REF_MBX, (long)mbxid, (long)pk_rmbx);
      #endif
        mbx = pMBX[mbxid];
        if (mbx == NULL)
        {   return_ret_ims2(THF_MBX, E_NOEXS);
        }
        pk_rmbx->exinf = NULL;
        if (mbx->moff == 0)
            mque = (V4_T_MSGHDR PFAR *)mbx->cmbx->mprihd;
        else
            mque = (V4_T_MSGHDR PFAR *)((ADDR)mbx + mbx->moff);

        for(i = 0; i < mbx->mpri; i++, mque++)
        {   if (mque->top != NULL)
            {   pk_rmbx->pk_msg = (T_MSG PFAR *)mque->top;
                return_ret_ims(THF_MBX);
            }
        }
        pk_rmbx->pk_msg = NULL;
       que = mbx->que;
       for (;;)
       {   tskid = *que++;
           if (tskid != 0)
           {   if (tskid >= TSK_QUETAIL)
                   break;
               pk_rmbx->wtsk = tskid;
               return_ret_ims(THF_MBX);
           }
       }
        pk_rmbx->wtsk = TSK_NONE;
        return_ret_ims(THF_MBX);
    }
  #if ERC
    vsnd_deb2e(THF_MBX, TFN_REF_MBX, (long)mbxid, (long)pk_rmbx, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_CRE_MBF
/************************************************/
/* Create MessageBuffer                         */
/************************************************/

ER v4_cre_mbf(ID mbfid, const V4_T_CMBF *pk_cmbf)
{
    T_MBF SNEAR *mbf;
    B PFAR *buf;
    unsigned bufsz;
    int m, n, i, atid = (mbfid == ID_AUTO);
    UINT size, coff, roff;
    TSK_ID *que;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if (pk_cmbf->maxmsz == 0)
        ercd = E_PAR;
    else if ((UINT)mbfid > (UINT)CFG.mbfid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MBF, TFN_CRE_MBF, (long)mbfid, (long)pk_cmbf);
  #endif
    if (atid)
    {   for (mbfid = CFG.mbfid_max;;)
        {   if (pMBF[mbfid] == NULL)
                break;
            if (--mbfid == 0)
            {   return_ret_ims2(THF_MBF, E_NOID);
            }
        }
        ercd = mbfid;
    }
    else
    {   if (pMBF[mbfid] != NULL)
        {   return_ret_ims2(THF_MBF, E_OBJ);
        }
        ercd = E_OK;
    }
    if (pk_cmbf->mbfatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_MBF) + n * sizeof(TSK_ID);
    if (pk_cmbf->mbfatr & TA_TPRIR)
        m = CFG.tpri_max;
    else
        m = 1;
    roff = size;
    size = size + (m + 1) * sizeof(TSK_ID);

    if (!_kernel_inROM((const void *)pk_cmbf))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CMBF);
    }
    else
        coff = 0;

    bufsz = pk_cmbf->mbfsz;
    if ((pk_cmbf->mbf == NULL) && (bufsz != 0))
    {   buf = mpl_alloc((SIZE)bufsz);
        if (buf == NULL)
        {   return_ret_ims2(THF_MBF, SYSER = E_NOMEM);
        }
    }
    else
        buf = (B PFAR *)pk_cmbf->mbf;

    mbf = sys_alloc((SIZE)size);
    if (mbf == NULL)
    {   return_ret_ims2(THF_MBF, SYSER = E_SYS);
    }
    pMBF[mbfid] = mbf;
    if (coff != 0)
    {   mbf->cmbf = (const V4_T_CMBF *)((ADDR)mbf + coff);
        osmem_cpy((DFAR B*)mbf->cmbf,(DFAR B*)pk_cmbf,sizeof(*pk_cmbf));
    }
    else
        mbf->cmbf = pk_cmbf;
    mbf->buf = (bufsz == 0)?NULL:buf;
    mbf->putp = 0;
    mbf->getp = 0;
    mbf->allsz = (UH)bufsz;
    mbf->frsz =  (UH)bufsz;
    mbf->maxsz = pk_cmbf->maxmsz;
    for(i = 0; i <= n; i++)
    {   mbf->stsk[i] = 0;
    }
    mbf->stsk[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    mbf->wtsk = roff;
    que = (TSK_ID SNEAR *)((ADDR)mbf + roff);
    for(i = 0; i < m; i++)
    {   *que++ = 0;
    }
    *que = (m==1)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_MBF, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_MBF, TFN_CRE_MBF, (long)mbfid, (long)pk_cmbf, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_MBF
/************************************************/
/* Create MessageBuffer                         */
/************************************************/

ER v3_cre_mbf(ID mbfid, const V3_T_CMBF *pk_cmbf)
{
    V4_T_CMBF v4_cmbf;

    v4_cmbf.mbfatr = (pk_cmbf->mbfatr & ~TA_TPRI);
    if (pk_cmbf->mbfatr & TA_TPRI)
        v4_cmbf.mbfatr |= TA_TPRIR;
    v4_cmbf.maxmsz = pk_cmbf->maxmsz;
    v4_cmbf.mbfsz = pk_cmbf->bufsz;
    v4_cmbf.mbf = NULL;
    v4_cmbf.name = &_KERNEL_NNM;

    return v4_cre_mbf(mbfid, (const V4_T_CMBF *)&v4_cmbf);
}

#endif
#ifdef V4_DEL_MBF
/************************************************/
/* Delete MessageBuffer                         */
/************************************************/

ER v4_del_mbf(ID mbfid)
{
    T_MBF SNEAR *mbf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int i,disp = FALSE;
    UINT size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (mbfid == 0)
        ercd = E_ID;
    else if ((UINT)mbfid > (UINT)CFG.mbfid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MBF, TFN_DEL_MBF, (long)mbfid);
  #endif
    mbf = pMBF[mbfid];
    if (mbf == NULL)
    {   return_ret_ims2(THF_MBF, E_NOEXS);
    }
    for(que = mbf->stsk, i = 0; i < 2;
        que = (TSK_ID *)((ADDR)mbf + mbf->wtsk),i++)
    {
        tskid = *que;
        for (;;)
        {   for (;tskid != 0;)
            {   if (tskid == 0)
                    break;
                tcb = pTCB[tskid];
                tcb->sp->ercd = E_DLT;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   chg_rdq(tcb, tskid);
                    disp |= tcb->pri < NOWPRI;
                }
                else
                    del_que(tcb, tskid);
                tskid = *que;
            }
            tskid = *++que;
            if (tskid >= TSK_QUETAIL)
                break;
        }
    }
    if (mbf->buf != mbf->cmbf->mbf)
        mpl_free(mbf->buf, (SIZE)mbf->allsz);
    size = sizeof(T_MBF);
    if (mbf->stsk[1] < TSK_QUETAIL)
        size += (CFG.tpri_max - 1) * sizeof(TSK_ID);
    if (mbf->cmbf->mbfatr & TA_TPRIR)
        size += (CFG.tpri_max + 1) * sizeof(TSK_ID);
    else
        size += 2 * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)mbf->cmbf))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CMBF);
    }
    sys_free(mbf, (SIZE)size);
    pMBF[mbfid] = NULL;
    if (disp)
    {   return_dispatch(THF_MBF);
    }
    else
    {   return_ret_ims(THF_MBF);
    }
  #if ERC
    }
    vsnd_deb1e(THF_MBF, TFN_DEL_MBF, (long)mbfid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TSND_MBF
/************************************************/
/* Send Message to MessageBuffer with Timeout   */
/************************************************/

ER v4_tsnd_mbf(ID mbfid, VP msg, UINT msgsz, TMO tmout)
{
    T_MBF SNEAR *mbf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
  #if ERC
    ER ercd;

    if (mbfid == 0)
        ercd = E_ID;
    else if ((UINT)mbfid > (UINT)CFG.mbfid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb4(THF_MBF, ((tmout==TMO_FEVR)?TFN_SND_MBF:((tmout==TMO_POL)?TFN_PSND_MBF:TFN_TSND_MBF)), (long)mbfid, (long)msg, (long)msgsz, (long)tmout);
  #endif
    mbf = pMBF[mbfid];
    if (mbf == NULL)
    {   return_ret_ims2(THF_MBF, E_NOEXS);
    }
    if (msgsz > mbf->maxsz)
    {   return_ret_ims2(THF_MBF, E_PAR);
    }

    que = (TSK_ID *)((ADDR)mbf + mbf->wtsk);
    for(;;)
    {   tskid = *que++;
        if (tskid != 0)
        {   if (tskid >= TSK_QUETAIL)
                break;

            tcb = pTCB[tskid];
            if (msgsz != 0)
                osmem_cpy((DFAR B *)tcb->sp->ptr, (DFAR B *)msg, msgsz);
            tcb->sp->ercd = msgsz;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
                return chg_rdq_dispatch_(THF_MBF, tcb, tskid);
            else
                return del_que_ret_ims_(THF_MBF, tcb, tskid);

        }
    }

    if (msgsz != 0)
    {   for(que = mbf->stsk; *que == 0; que++);
        if (*que >= TSK_QUETAIL)
        {   if (msg_2_mbf(mbf, msg, msgsz))
                return_ret_ims(THF_MBF);
        }
    }

    if (tmout != TMO_POL)
    {
      #if ERC
        if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        {   return_ret_ims2(THF_MBF, SYSER = E_CTX);
        }
      #endif
        tskid = RDQ[0];
        tcb = pTCB[tskid];
        if (tmout != TMO_FEVR)
        {   add_tmq(tmout, tcb, tskid);
            tcb->sts = (UB)(S_SMB|S_TMO);
        }
        else
            tcb->sts = S_SMB;
        CTXPTR = msg;
        CTXPTN = (UINT)msgsz;
        set_wid(mbfid);
        que = mbf->stsk;
        if (*(que + 1) < TSK_QUETAIL)
            que += NOWPRI - 1;
        return chg_que_dispatch1_(THF_MBF, que, tcb, tskid);
    }
    return_ret_ims2(THF_MBF, E_TMOUT);
  #if ERC
    }
    vsnd_deb4e(THF_MBF, ((tmout==TMO_FEVR)?TFN_SND_MBF:((tmout==TMO_POL)?TFN_PSND_MBF:TFN_TSND_MBF)), (long)mbfid, (long)msg, (long)msgsz, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef OS_CORE
BOOL push_msg2mbf(ID mbfid)
{
    BOOL disp = FALSE;
    int sndsz;
    T_MBF SNEAR *mbf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    PRI pri;

    mbf = pMBF[mbfid];
    que = mbf->stsk;
    for(;;que++)
    {   if ((tskid = *que) != 0)
        {   if (tskid >= TSK_QUETAIL)
                return disp;
            do {
                tcb = pTCB[tskid];
                if ((sndsz = tcb->sp->ptn) != 0)
                {   if (!msg_2_mbf(mbf, tcb->sp->ptr, sndsz))
                        return disp;
                }
                else
                    return disp;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   pri = tcb->pri;
                    if (pri < NOWPRI)
                        disp |= tcb->pri < NOWPRI;
                    chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
                }
                else
                    del_que(tcb, tskid);
            } while((tskid = *que) != 0);
        }
    }
}
#endif
#ifdef V4_TRCV_MBF
/****************************************************/
/* Receive Message from MessageBuffer with Timeout  */
/****************************************************/

ER_UINT v4_trcv_mbf(ID mbfid, VP msg, TMO tmout)
{
    T_MBF SNEAR *mbf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    TSK_ID tskid;
    UINT sndsz;
    TSK_PRI pri;
  #if ERC
    ER ercd;

    if (mbfid == 0)
        ercd = E_ID;
    else if ((UINT)mbfid > (UINT)CFG.mbfid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb3(THF_MBF, ((tmout==TMO_FEVR)?TFN_RCV_MBF:((tmout==TMO_POL)?TFN_PRCV_MBF:TFN_TRCV_MBF)), (long)mbfid, (long)msg, (long)tmout);
  #endif
    mbf = pMBF[mbfid];
    if (mbf == NULL)
    {   return_ret_ims2(THF_MBF, (ER_UINT)E_NOEXS);
    }
    if (mbf->allsz == mbf->frsz)
    {
        que = mbf->stsk;
        for(;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                    tcb = pTCB[tskid];
                    if ((sndsz = tcb->sp->ptn) != 0)
                        osmem_cpy((DFAR B *)msg, (DFAR B *)tcb->sp->ptr, sndsz);
                    if (tcb->sts & S_TMO)
                        del_tmq(tcb, tskid);
                    tcb->sts = S_RDY;
                    if (tcb->sus == 0)
                    {   pri = tcb->pri;
                        chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
                        if (pri < NOWPRI)
                        {   dispatch();
                            return sndsz;
                        }
                    }
                    else
                        del_que(tcb, tskid);
                return_ret_ims2(THF_MBF, sndsz);
            }
        }
        if (tmout != TMO_POL)
        {
          #if ERC
            if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_MBF, SYSER = E_CTX);
            }
          #endif
            tskid = RDQ[0];
            tcb = pTCB[tskid];
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_MBF|S_TMO);
            }
            else
                tcb->sts = S_MBF;
            CTXPTR = msg;
            set_wid(mbfid);
            que = (TSK_ID *)((ADDR)mbf + mbf->wtsk);
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            return chg_que_dispatch1_(THF_MBF, que, tcb, tskid);
        }
        return_ret_ims2(THF_MBF, (ER_UINT)E_TMOUT);
    }
    sndsz = (UINT) mbf_2_msg(mbf, msg);
    if (push_msg2mbf(mbfid))
    {   dispatch();
        return sndsz;
    }
    return_ret_ims2(THF_MBF, sndsz);
  #if ERC
    }
    vsnd_deb3e(THF_MBF, ((tmout==TMO_FEVR)?TFN_RCV_MBF:((tmout==TMO_POL)?TFN_PRCV_MBF:TFN_TRCV_MBF)), (long)mbfid, (long)msg, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}
#endif
#ifdef V3_TRCV_MBF
ER v3_trcv_mbf(VP msg, int *p_msgsz, ID mbfid, TMO tmout)
{
    ER_UINT sz = v4_trcv_mbf(mbfid, msg, tmout);
    if ((INT) sz > 0)
    {   *p_msgsz = sz;
        return E_OK;
    }
    else
        return sz;
}
#endif

#ifdef V4_REF_MBF
/************************************************/
/* Refer MessageBuffer Status                   */
/************************************************/

ER v4_ref_mbf(ID mbfid, V4_T_RMBF *pk_rmbf)
{
    T_MBF SNEAR *mbf;
    TSK_ID SNEAR *que;
    ID tskid;
    ER ercd;

  #if ERC
    if (mbfid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if ((UINT)mbfid > (UINT)CFG.mbfid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MBF, TFN_REF_MBF, (long)mbfid, (long)pk_rmbf);
      #endif
        mbf = pMBF[mbfid];
        if (mbf == NULL)
        {   return_ret_ims2(THF_MBF, E_NOEXS);
        }
        for (que = mbf->stsk;(tskid = *que) == 0;que++);
        pk_rmbf->stskid = (tskid >= TSK_QUETAIL) ?TSK_NONE:tskid;
        for (que = (TSK_ID *)((ADDR)mbf + mbf->wtsk);(tskid = *que) == 0;que++);
        pk_rmbf->rtskid = (tskid >= TSK_QUETAIL) ?TSK_NONE:tskid;

        if (mbf->allsz != mbf->frsz)
            pk_rmbf->smsgcnt = mbf_smsgcnt(mbf);
        else
            pk_rmbf->smsgcnt = 0;
        pk_rmbf->fmbfsz = mbf->frsz;

        return_ret_ims(THF_MBF);
    }
  #if ERC
    vsnd_deb2e(THF_MBF, TFN_REF_MBF, (long)mbfid, (long)pk_rmbf, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V3_REF_MBF
/************************************************/
/* Refer MessageBuffer Status                   */
/************************************************/

ER v3_ref_mbf(V3_T_RMBF *pk_rmbf, ID mbfid)
{
    T_MBF SNEAR *mbf;
    TSK_ID SNEAR *que;
    ID tskid;
    unsigned i, getp;
    ER ercd;
    UH msgsz;

  #if ERC
    if (mbfid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if (mbfid > CFG.mbfid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MBF, TFN_REF_MBF, (long)mbfid, (long)pk_rmbf);
      #endif
        mbf = pMBF[mbfid];
        if (mbf == NULL)
        {   return_ret_ims2(THF_MBF, E_NOEXS);
        }
        pk_rmbf->exinf = NULL;

        for (que = (TSK_ID *)((ADDR)mbf + mbf->wtsk);(tskid = *que) == 0;que++);
        pk_rmbf->wtsk = (tskid >= TSK_QUETAIL) ?FALSE:tskid;

        for (que = mbf->stsk;(tskid = *que) == 0;que++);
        pk_rmbf->stsk = (tskid >= TSK_QUETAIL) ?FALSE:tskid;

        if (mbf->allsz != mbf->frsz)
        {   if (mbf->maxsz > 1)
            {   getp = mbf->getp;
                i = 0;
                do
                {   *((B *)&msgsz + i) = *((B PFAR *)mbf->buf + getp);
                    if (++getp == mbf->allsz)
                        getp = 0;
                } while (++i != sizeof(msgsz));
                pk_rmbf->msgsz = (int)msgsz;
            }
            else
                pk_rmbf->msgsz = 1;
        }
        else if (tskid < TSK_QUETAIL)
            pk_rmbf->msgsz = pTCB[tskid]->sp->ptn;
        else
            pk_rmbf->msgsz = 0;
        pk_rmbf->frbufsz = mbf->frsz;

        return_ret_ims(THF_MBF);
    }
  #if ERC
    vsnd_deb2e(THF_MBF, TFN_REF_MBF, (long)mbfid, (long)pk_rmbf, (long)ercd);
  #endif
    return ercd;
}

#endif

#ifdef OS_CORE
/************************************************/
/* Copy Message to MessageBuffer                */
/************************************************/

BOOL FNEAR msg_2_mbf(T_MBF SNEAR *mbf, VP msg, unsigned size)
{
    unsigned i;
    UH putp;
    B PFAR *frm, *to;
    union {
        UH sz_uh;
        UB sz_ub[2];
    } sz;

    if (mbf->maxsz == 1)
    {   if (mbf->frsz == 0)
            return FALSE;
        --mbf->frsz;
        mbf->buf[putp = mbf->putp] = *((B PFAR *)msg);
        if (++putp == mbf->allsz)
            putp = 0;
        mbf->putp = putp;
    }
    else
    {   if (mbf->frsz < sizeof(UH) + size)
            return FALSE;
        frm = (B PFAR *)msg;
        putp = mbf->putp;
        mbf->frsz -= sizeof(UH) + (sz.sz_uh = (UH)size);
        mbf->buf[putp++] = sz.sz_ub[0];
        if (putp == mbf->allsz)
            putp = 0;
        mbf->buf[putp++] = sz.sz_ub[1];
        if (putp == mbf->allsz)
            putp = 0;
        if (putp + sz.sz_uh >= mbf->allsz)
        {   to = mbf->buf + putp;
            for(sz.sz_uh -= (i = mbf->allsz - putp); i != 0; i--)
                *to++ = *frm++;
            putp = 0;
        }
        to = mbf->buf + putp;
        mbf->putp = putp + sz.sz_uh;
        for(i = sz.sz_uh; i != 0; i--)
            *to++ = *frm++;
    }
    return TRUE;
}

#endif
#ifdef V4_TRCV_MBF
/************************************************/
/* Copy MessageBuffer to Message                */
/************************************************/

unsigned FNEAR mbf_2_msg(T_MBF SNEAR *mbf, VP msg)
{
    unsigned i, size;
    UH getp;
    B PFAR *frm, *to;
    union {
        UH sz_uh;
        UB sz_ub[2];
    } sz;

    if (mbf->maxsz == 1)
    {   *((B PFAR *)msg) = mbf->buf[getp = mbf->getp];
        ++mbf->frsz;
        if (++getp == mbf->allsz)
            getp = 0;
        mbf->getp = getp;
        return 1;
    }
    else
    {   to = (B PFAR *)msg;
        getp = mbf->getp;
        sz.sz_ub[0] = mbf->buf[getp++];
        if (getp == mbf->allsz)
            getp = 0;
        sz.sz_ub[1] = mbf->buf[getp++];
        if (getp == mbf->allsz)
            getp = 0;
        mbf->frsz += sizeof(UH) + sz.sz_uh;
        size = (unsigned)sz.sz_uh;
        if (getp + sz.sz_uh >= mbf->allsz)
        {   frm = mbf->buf + getp;
            for(sz.sz_uh -= (i = mbf->allsz - getp); i != 0; i--)
                *to++ = *frm++;
            getp = 0;
        }
        frm = mbf->buf + getp;
        mbf->getp = getp + sz.sz_uh;
        for(i = sz.sz_uh; i != 0; i--)
            *to++ = *frm++;
        return size;
    }
}
#endif
#ifdef V4_REF_MBF
/************************************************/
/* Copy MessageBuffer to Message                */
/************************************************/

unsigned FNEAR mbf_smsgcnt(T_MBF SNEAR *mbf)
{
    unsigned cnt = 0;
    UH getp, frsz = mbf->frsz;
    union {
        UH sz_uh;
        UB sz_ub[2];
    } sz;

    if (mbf->maxsz == 1)
        return (mbf->allsz - mbf->frsz);
    else
    {   getp = mbf->getp;
        while(1)
        {   sz.sz_ub[0] = mbf->buf[getp++];
            if (getp == mbf->allsz)
                getp = 0;
            sz.sz_ub[1] = mbf->buf[getp++];
            if (getp == mbf->allsz)
                getp = 0;
            frsz += sizeof(UH) + sz.sz_uh;
            cnt++;
            if (frsz >= mbf->allsz)
                return cnt;
            getp = getp + sz.sz_uh;
            if (getp >= mbf->allsz)
                getp -= mbf->allsz;
        }
    }
}
#endif
#ifdef V4_CRE_POR
/************************************************/
/* Create Port for Rendezvous                   */
/************************************************/

ER v4_cre_por(ID porid, const V4_T_CPOR *pk_cpor)
{
    T_POR SNEAR *por;
    int n, i, atid = (porid == ID_AUTO);
    UINT size, coff;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)porid > (UINT)CFG.porid_max)
        ercd = E_ID;
    else if (pk_cpor->poratr > TA_TPRI)
        ercd = E_RSATR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_POR, TFN_CRE_POR, (long)porid, (long)pk_cpor);
  #endif
    if (atid)
    {   for (porid = CFG.porid_max;;)
        {   if (pPOR[porid] == NULL)
                break;
            if (--porid == 0)
            {   return_ret_ims2(THF_POR, E_NOID);
            }
        }
        ercd = porid;
    }
    else
    {   if (pPOR[porid] != NULL)
        {   return_ret_ims2(THF_POR, E_OBJ);
        }
        ercd = E_OK;
    }
    if (pk_cpor->poratr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof (T_POR) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_cpor))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CPOR);
    }
    else
        coff = 0;
    por = sys_alloc((SIZE)size);
    if (por == NULL)
    {   return_ret_ims2(THF_POR, SYSER = E_SYS);
    }
    pPOR[porid] = por;
    if (coff != 0)
    {   por->cpor = (const V4_T_CPOR *)((ADDR)por + coff);
        osmem_cpy((DFAR B*)por->cpor, (DFAR B*)pk_cpor, sizeof(*pk_cpor));
    }
    else
        por->cpor = pk_cpor;
    por->maxcmsz = (UH)pk_cpor->maxcmsz;
    por->maxrmsz = (UH)pk_cpor->maxrmsz;
    por->atsk = TSK_NONE;
    por->dmmy = TSK_QUETAIL;
    for (i = 0; i <= n; i++)
        por->wtsk[i] = 0;
    por->wtsk[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_POR, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_POR, TFN_CRE_POR, (long)porid, (long)pk_cpor, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_POR
/************************************************/
/* Create Port for Rendezvous                   */
/************************************************/

ER v3_cre_por(ID porid, const V3_T_CPOR *pk_cpor)
{
    V4_T_CPOR cpor;

    cpor.poratr = pk_cpor->poratr;
    cpor.maxcmsz = pk_cpor->maxcmsz;
    cpor.maxrmsz = pk_cpor->maxrmsz;
    cpor.name = &_KERNEL_NNM;
    return v4_cre_por(porid, (const V4_T_CPOR *)&cpor);

}

#endif
#ifdef V4_DEL_POR
/************************************************/
/* Delete Port for Rendezvous                   */
/************************************************/

ER v4_del_por(ID porid)
{
    T_POR SNEAR *por;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int disp = FALSE;
    UINT size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((porid == 0) || (porid > CFG.porid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_POR, TFN_DEL_POR, (long)porid);
  #endif
    por = pPOR[porid];
    if (por == NULL)
    {   return_ret_ims2(THF_POR, E_NOEXS);
    }

    que = por->wtsk;
    for(;*que < TSK_QUETAIL;++que)
    {
        for (;(tskid = *que) != 0;)
        {   tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {   chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
        }
    }
    size = sizeof (T_POR) + ((*que > TSK_QUETAIL)?((CFG.tpri_max - 1)*sizeof(TSK_ID)):0);
    que = &por->atsk;
    for(;(tskid = *que) != 0;)
    {   tcb = pTCB[tskid];
        tcb->sp->ercd = E_DLT;
        if (tcb->sts & S_TMO)
            del_tmq(tcb, tskid);
        tcb->sts = S_RDY;
        if (tcb->sus == 0)
        {   chg_rdq(tcb, tskid);
            disp |= tcb->pri < NOWPRI;
        }
        else
            del_que(tcb, tskid);
    }
    if (!_kernel_inROM((const void *)por->cpor))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CPOR);
    }
    sys_free(por, (SIZE)size);
    pPOR[porid] = NULL;
    if (disp == TRUE)
    {   return_dispatch(THF_POR);
    }
    else
    {   return_ret_ims(THF_POR);
    }
  #if ERC
    }
    vsnd_deb1e(THF_POR, TFN_DEL_POR, (long)porid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif


#ifdef V4_TCAL_POR
/************************************************/
/* Call Port for Rendezvous with Timeout        */
/************************************************/

ER_UINT v4_tcal_por(ID porid, RDVPTN calptn, VP msg, UINT cmsgsz, TMO tmout)
{
    T_POR SNEAR *por;
    T_TCB SNEAR *tcb;
    T_TCB SNEAR *selftcb;
    TSK_ID SNEAR *que;
    ID tskid,nxt,end,selfid;
    T_CPORCTX rdvctx;
    UINT acpptn;
    RDVNO rdvno;
  #if ERC
    ER ercd;

    if ((porid == 0) || ((UINT)porid > (UINT)CFG.porid_max))
        ercd = E_ID;
    else if ((calptn == 0) || (tmout == TMO_POL))
        ercd = E_PAR;
    else if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        ercd = E_CTX;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb5(THF_POR, ((tmout==TMO_FEVR)?TFN_CAL_POR:TFN_TCAL_POR), (long)porid, (long)calptn, (long)msg, (long)cmsgsz, (long)tmout);
  #endif
    por = pPOR[porid];
    if (por == NULL)
    {   return_ret_ims2(THF_POR, (ER_UINT)E_NOEXS);
    }
    if (cmsgsz > por->maxcmsz)
    {   return_ret_ims2(THF_POR, (ER_UINT)E_PAR);
    }
    if ((nxt = por->atsk) != 0)
    {   tcb = pTCB[nxt];
        end = tcb->pid;
        for (;;)
        {   tskid = nxt;
            tcb = pTCB[tskid];
            nxt = tcb->nid;
            acpptn = tcb->sp->ptn;
            if (acpptn & calptn)
            {   selfid = RDQ[0];
                selftcb = pTCB[selfid];
                if (cmsgsz > 0)
                    osmem_cpy((DFAR B *)((T_APORCTX PFAR *)tcb->sp->ptr)->buf, (DFAR B *)msg, cmsgsz);
                tcb->sp->ercd = cmsgsz;
                rdvno = (++_kernel_rdv_seqno << 8*sizeof(TSK_ID)) + selfid;
                *((T_APORCTX PFAR *)tcb->sp->ptr)->p_rdvno = rdvno;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                    chg_rdq(tcb, tskid);
                else
                    del_que(tcb, tskid);
                rdvctx.buf = msg;
                rdvctx.size = cmsgsz;
                rdvctx.maxsz = por->maxrmsz;
                rdvctx.rdvno = rdvno;
                CTXPTR = &rdvctx;
                CTXPTN = calptn;
                if (tmout != TMO_FEVR)
                {   add_tmq(tmout, selftcb, selfid);
                    selftcb->sts = (UB)(S_RDV|S_TMO);
                }
                else
                    selftcb->sts = S_RDV;
                return del_que_dispatch1_(THF_POR, selftcb, selfid);
            }
            if (tskid == end)
            break;
        }
    }
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    if (tmout != TMO_FEVR)
    {   add_tmq(tmout, tcb, tskid);
        tcb->sts = (UB)(S_CAL|S_TMO);
    }
    else
        tcb->sts = S_CAL;

    rdvctx.buf = msg;
    rdvctx.size = cmsgsz;
    rdvctx.maxsz = por->maxrmsz;
    rdvctx.rdvno = 0;
    CTXPTR = &rdvctx;
    CTXPTN = calptn;
    set_wid(porid);
    que = por->wtsk;
    if (*(que + 1) < TSK_QUETAIL)
        que += NOWPRI - 1;
    return chg_que_dispatch1_(THF_POR, que, tcb, tskid);
  #if ERC
    }
    vsnd_deb5e(THF_POR, ((tmout==TMO_FEVR)?TFN_CAL_POR:TFN_TCAL_POR), (long)porid, (long)calptn, (long)msg, (long)cmsgsz, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_TCAL_POR
/************************************************/
/* Call Port for Rendezvous with Timeout        */
/************************************************/

ER v3_tcal_por(VP msg, int *p_rmsgsz, ID porid, UINT calptn, int cmsgsz, TMO tmout)
{
    ER ercd;

    if ((ercd = v4_tcal_por(porid, calptn, msg, cmsgsz, tmout)) > 0)
    {   *p_rmsgsz = ercd;
        return E_OK;
    }
    else
        return ercd;
}

#endif
#ifdef V4_TACP_POR
/************************************************/
/* Accept Port for Rendezvous with Timeout      */
/************************************************/

ER_UINT v4_tacp_por(ID porid, RDVPTN acpptn, RDVNO *p_rdvno, VP msg, TMO tmout)
{
    T_POR SNEAR *por;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    ID nxt,end;
    T_APORCTX rdvctx;
    UINT calptn, msize;
    RDVNO rdvno;
  #if ERC
    ER ercd;

    if ((porid == 0) || ((UINT)porid > (UINT)CFG.porid_max))
        ercd = E_ID;
    else if (acpptn == 0)
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb5(THF_POR, ((tmout==TMO_FEVR)?TFN_ACP_POR:((tmout==TMO_POL)?TFN_PACP_POR:TFN_TACP_POR)), (long)porid, (long)acpptn, (long)p_rdvno, (long)msg, (long)tmout);
  #endif
    por = pPOR[porid];
    if (por == NULL)
    {   return_ret_ims2(THF_POR, (ER_UINT)E_NOEXS);
    }
    que = por->wtsk;
    for(;;)
    {   while((tskid = *que++) == 0);
        if (tskid >= TSK_QUETAIL)
            break;
        tcb = pTCB[tskid];
        end = tcb->pid;
        for(;;)
        {   nxt = tcb->nid;
            calptn = tcb->sp->ptn;
            if (calptn & acpptn)
            {   if ((msize = ((T_CPORCTX PFAR *)tcb->sp->ptr)->size) > 0)
                    osmem_cpy((DFAR B *)msg, (DFAR B *)((T_CPORCTX PFAR *)tcb->sp->ptr)->buf, msize);
                rdvno = (++_kernel_rdv_seqno << 8*sizeof(TSK_ID)) + tskid;
                ((T_CPORCTX PFAR *)tcb->sp->ptr)->rdvno = rdvno;
                *p_rdvno = rdvno;
                if (tcb->sts & S_TMO)
                    tcb->sts = (UB) (S_RDV | S_TMO);
                else
                    tcb->sts = S_RDV;
                del_que(tcb, tskid);
                return_ret_ims2(THF_POR, msize);
            }
            if (tskid == end)
                break;
            else
            {   tskid = nxt;
                tcb = pTCB[tskid];
            }
        }
    }
    if (tmout != TMO_POL)
    {
      #if ERC
        if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
        {   return_ret_ims2(THF_POR, SYSER = E_CTX);
        }
      #endif
        tskid = RDQ[0];
        tcb = pTCB[tskid];
        if (tmout != TMO_FEVR)
        {   add_tmq(tmout, tcb, tskid);
            tcb->sts = (UB)(S_ACP|S_TMO);
        }
        else
            tcb->sts = S_ACP;
        rdvctx.buf = msg;
        rdvctx.p_rdvno = p_rdvno;
        CTXPTR = &rdvctx;
        CTXPTN = acpptn;
        set_wid(porid);
        que = &por->atsk;
        return chg_que_dispatch1_(THF_POR, que, tcb, tskid);
    }
    return_ret_ims2(THF_POR, (ER_UINT)E_TMOUT);
  #if ERC
    }
    vsnd_deb5e(THF_POR, ((tmout==TMO_FEVR)?TFN_ACP_POR:((tmout==TMO_POL)?TFN_PACP_POR:TFN_TACP_POR)), (long)porid, (long)acpptn, (long)p_rdvno, (long)msg, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_TACP_POR
/************************************************/
/* Accept Port for Rendezvous with Timeout      */
/************************************************/

ER v3_tacp_por(RNO *p_rdvno, VP msg, int *p_cmsgsz, ID porid, UINT acpptn, TMO tmout)
{
    ER ercd;

    if ((ercd = v4_tacp_por(porid, acpptn, p_rdvno, msg, tmout)) > 0)
    {   *p_cmsgsz = ercd;
        return E_OK;
    }
    else
        return ercd;
}

#endif
#ifdef V4_FWD_POR
/************************************************/
/* Forward Rendezvous to Other Port             */
/************************************************/

ER v4_fwd_por(ID porid, RDVPTN calptn, RDVNO rdvno, VP msg, UINT cmsgsz)
{
    T_POR SNEAR *por;
    T_TCB SNEAR *tcb, *atcb;
    TSK_ID SNEAR *que;
    ID tskid,nxt,end,ctskid = (rdvno & (TSK_ID) -1);
    UINT acpptn;
    UH maxrmsz;
  #if ERC
    ER ercd;

    if ((porid == 0) || ((UINT)porid > (UINT)CFG.porid_max))
        ercd = E_ID;
    else if (calptn == 0)
        ercd = E_PAR;
    else if ((ctskid == 0) || ((UINT)ctskid > (UINT)CFG.tskid_max))
        ercd = E_OBJ;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb5(THF_POR, TFN_FWD_POR, (long)porid, (long)calptn, (long)rdvno, (long)msg, (long)cmsgsz);
  #endif
    por = pPOR[porid];
    if (por == NULL)
    {   return_ret_ims2(THF_POR, E_NOEXS);
    }
    tcb = pTCB[ctskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    if ((tcb->sts & STS_MSK0) != S_RDV)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    if (((T_CPORCTX PFAR *)tcb->sp->ptr)->rdvno != rdvno)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    maxrmsz = ((T_CPORCTX PFAR *)tcb->sp->ptr)->maxsz;
    if (cmsgsz > maxrmsz)
    {   return_ret_ims2(THF_POR, E_PAR);
    }
    if (cmsgsz > por->maxcmsz)
    {   return_ret_ims2(THF_POR, E_PAR);
    }
    if (por->maxrmsz > maxrmsz)
    {   return_ret_ims2(THF_POR, E_ILUSE);
    }
    nxt = por->atsk;
    if (nxt != 0)
    {   atcb = pTCB[nxt];
        end = atcb->pid;
        for (;;)
        {   tskid = nxt;
            atcb = pTCB[tskid];
            nxt = atcb->nid;
            acpptn = atcb->sp->ptn;
            if (acpptn & calptn)
            {   if (cmsgsz > 0)
                    osmem_cpy((DFAR B *)((T_APORCTX PFAR *)atcb->sp->ptr)->buf, (DFAR B *)msg, cmsgsz);
                atcb->sp->ercd = cmsgsz;
                ((T_CPORCTX PFAR *)tcb->sp->ptr)->rdvno =
                    *((T_APORCTX PFAR *)atcb->sp->ptr)->p_rdvno =
                    (++_kernel_rdv_seqno << 8*sizeof(TSK_ID)) + ctskid;
                if (atcb->sts & S_TMO)
                    del_tmq(atcb, tskid);
                atcb->sts = S_RDY;
                if (atcb->sus == 0)
                    chg_rdq(atcb, tskid);
                else
                    del_que(atcb, tskid);
                tcb->sts = (tcb->sts & S_TMO)|(UB)S_RDV;
                ((T_CPORCTX PFAR *)tcb->sp->ptr)->size = cmsgsz;
                tcb->sp->ptn = calptn;
                if (atcb->sus == 0)
                    return chg_rdq_dispatch_(THF_POR, atcb, tskid);
                else
                    return del_que_ret_ims_(THF_POR, atcb, tskid);
            }
            if (tskid == end)
            break;
        }
    }
    tcb->sts = (tcb->sts & S_TMO)|(UB)S_CAL;
    if (cmsgsz > 0)
        osmem_cpy((DFAR B *)(((T_CPORCTX PFAR *)tcb->sp->ptr)->buf), (DFAR B *)msg, cmsgsz);
    ((T_CPORCTX PFAR *)tcb->sp->ptr)->size = cmsgsz;
    tcb->sp->ptn = calptn;
    que = por->wtsk;
    if (*(que + 1) < TSK_QUETAIL)
        que += NOWPRI - 1;
    add_que(que, tcb, ctskid);
    return_ret_ims(THF_POR);
  #if ERC
    }
    vsnd_deb5e(THF_POR, TFN_FWD_POR, (long)porid, (long)calptn, (long)rdvno, (long)msg, (long)cmsgsz, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_RPL_RDV
/************************************************/
/* Reply Rendezvous                             */
/************************************************/

ER v4_rpl_rdv(RDVNO rdvno, VP msg, UINT rmsgsz)
{
    T_TCB SNEAR *tcb;
    UH maxrmsz;
    ID ctskid = (rdvno & (TSK_ID) -1);
  #if ERC
    ER ercd;

    if ((ctskid == 0) || ((UINT)ctskid > (UINT)CFG.tskid_max))
        ercd = E_OBJ;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb3(THF_POR, TFN_RPL_RDV, (long)rdvno, (long)msg, (long)rmsgsz);
  #endif
    tcb = pTCB[ctskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    if ((tcb->sts & STS_MSK0) != S_RDV)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    if (((T_CPORCTX PFAR *)tcb->sp->ptr)->rdvno != rdvno)
    {   return_ret_ims2(THF_POR, E_OBJ);
    }
    maxrmsz = ((T_CPORCTX PFAR *)tcb->sp->ptr)->maxsz;
    if (rmsgsz > maxrmsz)
    {   return_ret_ims2(THF_POR, E_PAR);
    }
    if (rmsgsz > 0)
        osmem_cpy((DFAR B *)(((T_CPORCTX PFAR *)tcb->sp->ptr)->buf), (DFAR B *)msg, rmsgsz);
    if (tcb->sts & S_TMO)
        del_tmq(tcb, ctskid);
    tcb->sp->ercd = rmsgsz;
    tcb->sts = S_RDY;
    if (tcb->sus == 0)
        return add_rdq_dispatch_(THF_POR, tcb, ctskid);
    return_ret_ims(THF_POR);
  #if ERC
    }
    vsnd_deb3e(THF_POR, TFN_RPL_RDV, (long)rdvno, (long)msg, (long)rmsgsz, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_POR
/************************************************/
/* Refer Port Status                            */
/************************************************/

ER v4_ref_por(ID porid, V4_T_RPOR *pk_rpor)
{
    T_POR SNEAR *por;
    ID tskid;
    TSK_ID *que;
  #if ERC
    ER ercd;

    if ((porid == 0) || ((UINT)porid > (UINT)CFG.porid_max))
        SYSER = ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_POR, TFN_REF_POR, (long)porid, (long)pk_rpor);
      #endif
        por = pPOR[porid];
        if (por == NULL)
        {   return_ret_ims2(THF_POR, E_NOEXS);
        }
        pk_rpor->atskid = (por->atsk == 0)?TSK_NONE:por->atsk;
        que = por->wtsk;
        for (; (tskid = *que) < TSK_QUETAIL; que++)
        {   if (tskid != 0)
            {   pk_rpor->ctskid = tskid;
                return_ret_ims(THF_POR);
            }
        }
        pk_rpor->ctskid = TSK_NONE;
        return_ret_ims(THF_POR);
  #if ERC
    }
    vsnd_deb2e(THF_POR, TFN_REF_POR, (long)porid, (long)pk_rpor, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_POR
/************************************************/
/* Refer Port Status                            */
/************************************************/

ER v3_ref_por(V3_T_RPOR *pk_rpor, ID porid)
{
    T_POR SNEAR *por;
    TSK_ID tskid;
    ER ercd;
    TSK_ID *que;

  #if ERC
    if (porid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if (porid > CFG.porid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_POR, TFN_REF_POR, (long)porid, (long)pk_rpor);
      #endif
        por = pPOR[porid];
        if (por == NULL)
        {   return_ret_ims2(THF_POR, E_NOEXS);
        }
        pk_rpor->exinf = 0;
        pk_rpor->atsk = (por->atsk == 0)?FALSE:por->atsk;
        que = por->wtsk;
        for (; (tskid = *que) < TSK_QUETAIL; que++)
        {   if (tskid != 0)
            {   pk_rpor->wtsk = tskid;
                return_ret_ims(THF_POR);
            }
        }
        pk_rpor->wtsk = FALSE;
        return_ret_ims(THF_POR);

    }
  #if ERC
    vsnd_deb2e(THF_POR, TFN_REF_POR, (long)porid, (long)pk_rpor, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_REF_RDV
/************************************************/
/* Refer Rendezvous Status                      */
/************************************************/

ER v4_ref_rdv(RDVNO rdvno, V4_T_RRDV *pk_rrdv)
{
    T_TCB SNEAR *tcb;
    ID ctskid = (rdvno & (TSK_ID) -1);

  #if ERC
    pk_rrdv->wtskid = TSK_NONE;
    if ((ctskid != 0) && ((UINT)ctskid <= (UINT)CFG.tskid_max))
    {
  #endif
    dis_ims();
    visnd_deb2(THF_POR, TFN_REF_RDV, (long)rdvno, (long)pk_rrdv);
    tcb = pTCB[ctskid];
    if (tcb != NULL)
    {   if (tcb->sts == S_RDV)
        {   if (((T_CPORCTX PFAR *)tcb->sp->ptr)->rdvno == rdvno)
            {   pk_rrdv->wtskid = ctskid;
            }
        }
    }
    return_ret_ims(THF_POR);
  #if ERC
    }
    vsnd_deb2e(THF_POR, TFN_REF_RDV, (long)rdvno, (long)pk_rrdv, (long)E_OK);
    return E_OK;
  #endif
}

#endif
#ifdef V4_LOC_CPU
/************************************************/
/* Lock CPU                                     */
/************************************************/

ER v4_loc_cpu(void)
{
    dis_ims();
  #if ERC
    visnd_deb0e(THF_INT, TFN_LOC_CPU, E_OK);
  #endif
    if (DDISP & TSS_V4_DDSP)
        DDISP = TSS_LOC_DDSP | TSS_LOC;
    else
        DDISP = TSS_V4_LOC | TSS_LOC;
    return E_OK;
}

#endif
#ifdef V3_LOC_CPU
/************************************************/
/* Lock CPU                                     */
/************************************************/

ER v3_loc_cpu(void)
{
  #if ERC
    if ((INEST)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {   vsnd_deb0e(THF_INT, TFN_LOC_CPU, E_CTX);
        return SYSER = E_CTX;
    }
  #endif
    dis_ims();
  #if ERC
    visnd_deb0e(THF_INT, TFN_LOC_CPU, E_OK);
  #endif
    DDISP |= TSS_LOC;
    return E_OK;
}

#endif
#ifdef V4_UNL_CPU
/************************************************/
/* Unlock CPU                                   */
/************************************************/

ER v4_unl_cpu(void)
{
    dis_ims();
  #if ERC
    visnd_deb0(THF_INT, TFN_UNL_CPU);
  #endif
    unl_cpu_sub();
    if (DDISP & TSS_V4_DDSP)
        DDISP = TSS_V4_DDSP | TSS_DDSP;
    else
    {   DDISP = TSS_TSK;
        if (!INEST && (RDQ[0] != (TSK_ID)(CFG.tskid_max+1)))
        {   SDISP = 0;
            if (DELAY)
                return_dispatch2(THF_INT);
        }
    }
    return_ret_ims(THF_INT);
}

#endif
#ifdef V3_UNL_CPU
/************************************************/
/* Unlock CPU                                   */
/************************************************/

ER v3_unl_cpu(void)
{
  #if ERC
    if ((INEST)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {   vsnd_deb0e(THF_INT, TFN_UNL_CPU, E_CTX);
        return SYSER = E_CTX;
    }
  #endif
    dis_ims();
  #if ERC
    visnd_deb0(THF_INT, TFN_UNL_CPU);
  #endif
    unl_cpu_sub();
    DDISP = TSS_TSK;
    SDISP = 0;
    if (DELAY)
    {   return_dispatch2(THF_INT);
    }
    return_ret_ims(THF_INT);
}

#endif
#ifdef V3_CHG_IMS
/************************************************/
/* Change Interrupt Mask                        */
/************************************************/

ER v3_chg_ims(UINT imask)
{
    dis_ims();
  #if ERC
    visnd_deb1(THF_INT, TFN_CHG_IMS, (long)imask);
  #endif
    chg_ims_sub(imask);
    if (!INEST)
    {   if (imask)
            DDISP |= TSS_IMS;
        else
        {   DDISP &= ~TSS_IMS;
            if ((DDISP == 0) && (RDQ[0] != (TSK_ID)(CFG.tskid_max+1)))
            {   SDISP = 0;
                if (DELAY)
                {   return_dispatch2(THF_INT);
                }
            }
        }
    }
    return_ret_ims(THF_INT);
}

#endif
#ifdef V3_REF_IMS
/************************************************/
/* Refer Interrupt Mask Status                  */
/************************************************/

ER v3_ref_ims(UINT *p_imask)
{
    UINT ims;

    dis_ims();
  #if ERC
    visnd_deb1(THF_INT, TFN_GET_IMS, (long)p_imask);
  #endif
    ims = IMASK;
    ret_ims2_(THF_INT);
    *p_imask = ref_ims_sub(ims);
    return E_OK;
}

#endif
#ifdef V4_CRE_MPL
/************************************************/
/* Create Variable-size MemoryPool              */
/************************************************/

ER v4_cre_mpl(ID mplid, const V4_T_CMPL *pk_cmpl)
{
    T_MPL SNEAR *mpl;
    T_MEM PFAR *top;
    int n, i, atid = (mplid == ID_AUTO);
    UINT size, coff;
    SIZE allsz;
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if (!atid & ((mplid == 0) || ((UINT)mplid > (UINT)CFG.mplid_max)))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MPL, TFN_CRE_MPL, (long)mplid, (long)pk_cmpl);
  #endif
    if (atid)
    {   for (mplid = CFG.mplid_max; pMPL[mplid] != NULL;)
        {   if (--mplid == 0)
            {   return_ret_ims2(THF_MPL, E_NOID);
            }
        }
        ercd = mplid;
    }
    else
    {   if (pMPL[mplid] != NULL)
        {   return_ret_ims2(THF_MPL, E_OBJ);
        }
        ercd = E_OK;
    }
    allsz = pk_cmpl->mplsz;
    if ((top = pk_cmpl->mpl) == NULL)
    {   top = mpl_alloc(allsz);
        if (top == NULL)
        {   return_ret_ims2(THF_MPL, SYSER = E_NOMEM);
        }
    }
    if (pk_cmpl->mplatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof(T_MPL) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_cmpl))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CMPL);
    }
    else
        coff = 0;
    mpl = sys_alloc((SIZE)size);
    if (mpl == NULL)
    {   return_ret_ims2(THF_MPL, SYSER = E_SYS);
    }

    ospmem_init(top, allsz);
    pMPL[mplid] = mpl;
    if (coff != 0)
    {   mpl->cmpl = (const V4_T_CMPL *)((ADDR)mpl + coff);
        osmem_cpy((DFAR B *)mpl->cmpl, (DFAR B *)pk_cmpl, sizeof(*pk_cmpl));
    }
    else
        mpl->cmpl = pk_cmpl;
    mpl->top = top;
    mpl->allad = (B PFAR *)top;
    mpl->allsz = allsz;
    for (i = 0; i <= n; i++)
        mpl->que[i] = 0;
    mpl->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_MPL, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_MPL, TFN_CRE_MPL, (long)mplid, (long)pk_cmpl, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_MPL
/************************************************/
/* Create Variable-size MemoryPool              */
/************************************************/

ER v3_cre_mpl(ID mplid, const V3_T_CMPL *pk_cmpl)
{
    V4_T_CMPL cmpl;

    cmpl.mplatr = pk_cmpl->mplatr;
    cmpl.mplsz = pk_cmpl->mplsz;
    cmpl.mpl = NULL;
    cmpl.name = &_KERNEL_NNM;
    return v4_cre_mpl(mplid, (const V4_T_CMPL *)&cmpl);
}

#endif
#ifdef V4_DEL_MPL
/************************************************/
/* Delete Variable-size MemoryPool              */
/************************************************/

ER v4_del_mpl(ID mplid)
{
    T_MPL SNEAR *mpl;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int disp = FALSE;
    UINT size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((mplid == 0) || (mplid > CFG.mplid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MPL, TFN_DEL_MPL, (long)mplid);
  #endif
    mpl = pMPL[mplid];
    if (mpl == NULL)
    {   return_ret_ims2(THF_MPL, E_NOEXS);
    }
    que = mpl->que;
    for(;*que < TSK_QUETAIL;++que)
    {
        for (;(tskid = *que) != 0;)
        {   tcb = pTCB[tskid];
            tcb->sp->ercd = E_DLT;
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            tcb->sts = S_RDY;
            if (tcb->sus == 0)
            {    chg_rdq(tcb, tskid);
                disp |= tcb->pri < NOWPRI;
            }
            else
                del_que(tcb, tskid);
        }
    }
    if (mpl->cmpl->mpl == NULL)
        mpl_free(mpl->allad, mpl->allsz);
    size = sizeof(T_MPL);
    if (mpl->que[1] < TSK_QUETAIL)
        size = size + (CFG.tpri_max - 1) * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)mpl->cmpl))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CMPL);
    }
    sys_free(mpl, (SIZE)size);
    pMPL[mplid] = NULL;
    if (disp == TRUE)
    {   return_dispatch(THF_MPL);
    }
    else
    {   return_ret_ims(THF_MPL);
    }
  #if ERC
    }
    vsnd_deb1e(THF_MPL, TFN_DEL_MPL, (long)mplid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TGET_MPL
/************************************************/
/* Get Variable-size Memory Block with Timeout  */
/************************************************/

ER v4_tget_mpl(ID mplid, UINT blksz, VP p_blk, TMO tmout)
{
    T_MPL SNEAR *mpl;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    int PFAR *blk;
    ID tskid;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((mplid == 0) || (mplid > CFG.mplid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb4(THF_MPL, ((tmout==TMO_FEVR)?TFN_GET_MPL:((tmout==TMO_POL)?TFN_PGET_MPL:TFN_TGET_MPL)), (long)mplid, (long)blksz, (long)p_blk, (long)tmout);
  #endif
    mpl = pMPL[mplid];
    if (mpl == NULL)
    {   return_ret_ims2(THF_MPL, E_NOEXS);
    }
    blk = blk_palloc(&mpl->top, (SIZE)blksz + SIZEOF_ALIGN);
    if (blk == NULL)
    {   if (tmout != TMO_POL)
        {
          #if ERC
            if ((DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_MPL, SYSER = E_CTX);
            }
          #endif
            tskid = RDQ[0];
            tcb = pTCB[tskid];
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_MPL|S_TMO);
            }
            else
                tcb->sts = S_MPL;
            CTXPTN = (UINT)blksz;
            CTXPTR = p_blk;
            set_wid(mplid);
            que = mpl->que;
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            return chg_que_dispatch1_(THF_MPL, que, tcb, tskid);
        }
        return_ret_ims2(THF_MPL, E_TMOUT);
    }
    *blk = (SIZE)blksz + SIZEOF_ALIGN;
    blk = (int PFAR *)((ADDR)blk + SIZEOF_ALIGN);
    *(int PFAR * PFAR *)p_blk = blk;
    return_ret_ims(THF_MPL);
  #if ERC
    }
    vsnd_deb4e(THF_MPL, ((tmout==TMO_FEVR)?TFN_GET_MPL:((tmout==TMO_POL)?TFN_PGET_MPL:TFN_TGET_MPL)), (long)mplid, (long)blksz, (long)p_blk, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}
#endif
#ifdef OS_CORE
BOOL rel_mplchk(ID mplid)
{
    BOOL disp = FALSE;
    T_MPL SNEAR *mpl;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    PRI pri;
    int PFAR *p;
    SIZE blksz;

    mpl = pMPL[mplid];
    que = mpl->que;
    for(;(tskid = *que) < TSK_QUETAIL;que++)
    {   if ((tskid) != 0)
        {   do {
                tcb = pTCB[tskid];
                blksz = (SIZE)tcb->sp->ptn + SIZEOF_ALIGN;
                p = blk_palloc((T_MEM PFAR * SNEAR *)&mpl->top, (SIZE)blksz);
                if (p == NULL)
                    return disp;
                *p = (SIZE)blksz;
                p = (int PFAR *)((ADDR)p + SIZEOF_ALIGN);
                *(int PFAR * PFAR *)tcb->sp->ptr = p;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   pri = tcb->pri;
                    if (pri < NOWPRI)
                        disp |= tcb->pri < NOWPRI;
                    chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
                }
                else
                    del_que(tcb, tskid);
            } while((tskid = *que) != 0);
        }
    }
    return disp;
}

#endif
#ifdef V4_REL_MPL
/************************************************/
/* Release Variable-size Memory Block           */
/************************************************/

ER v4_rel_mpl(ID mplid, VP blk)
{
    T_MPL SNEAR *mpl;
    SIZE size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((mplid == 0) || (mplid > CFG.mplid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MPL, TFN_REL_MPL, (long)mplid, (long)blk);
  #endif
    mpl = pMPL[mplid];
    if (mpl == NULL)
    {   return_ret_ims2(THF_MPL, E_NOEXS);
    }
    if ((ADDR)blk & (ADDR)(SIZEOF_INT - 1))
    {   return_ret_ims2(THF_MPL, E_PAR);
    }
    blk = (int PFAR *)((ADDR)blk - SIZEOF_ALIGN);
    size = *(SIZE PFAR *)blk;
    if ( (size < SIZEOF_ALIGN)
       ||(size > mpl->allsz)
       ||((ADDR)blk < (ADDR)mpl->allad)
       ||(((ADDR)blk+size) > (ADDR)(mpl->allad+mpl->allsz)))
    {   return_ret_ims2(THF_MPL, E_PAR);
    }
    if (E_OK != blk_pfree(&mpl->top, blk, size))
    {   return_ret_ims2(THF_MPL, E_PAR);
    }
    if (rel_mplchk(mplid))
    {   return_dispatch(THF_MPL);
    }
    return_ret_ims(THF_MPL);
  #if ERC
    }
    vsnd_deb2e(THF_MPL, TFN_REL_MPL, (long)mplid, (long)blk, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_MPL
/************************************************/
/* Refer Variable-size MemoryPool Status        */
/************************************************/

ER v4_ref_mpl(ID mplid, V4_T_RMPL *pk_rmpl)
{
    T_MPL SNEAR *mpl;
    TSK_ID SNEAR *que;
    ID tskid;
    ER ercd;
    SIZE fblksz;

  #if ERC
    if (mplid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if ((UINT)mplid > (UINT)CFG.mplid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MPL, TFN_REF_MPL, (long)mplid, (long)pk_rmpl);
      #endif
        mpl = pMPL[mplid];
        if (mpl == NULL)
        {   return_ret_ims2(THF_MPL, E_NOEXS);
        }
        pk_rmpl->fmplsz = mpl_size(mpl->top, &fblksz);
        pk_rmpl->fblksz = (fblksz > (SIZE)((UINT)(-1))) ? (UINT)(-1):(UINT)fblksz;
        que = mpl->que;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                pk_rmpl->wtskid = tskid;
                return_ret_ims(THF_MPL);
            }
        }
        pk_rmpl->wtskid = TSK_NONE;
        return_ret_ims(THF_MPL);
    }
  #if ERC
    vsnd_deb2e(THF_MPL, TFN_REF_MPL, (long)mplid, (long)pk_rmpl, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V3_REF_MPL
/************************************************/
/* Refer Variable-size MemoryPool Status        */
/************************************************/

ER v3_ref_mpl(V3_T_RMPL PFAR *pk_rmpl, ID mplid)
{
    T_MPL SNEAR *mpl;
    TSK_ID SNEAR *que;
    ID tskid;
    ER ercd;

  #if ERC
    if (mplid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if ((UINT)mplid > (UINT)CFG.mplid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MPL, TFN_REF_MPL, (long)mplid, (long)pk_rmpl);
      #endif
        mpl = pMPL[mplid];
        if (mpl == NULL)
        {   return_ret_ims2(THF_MPL, E_NOEXS);
        }
        pk_rmpl->exinf = 0;
        pk_rmpl->maxsz = mpl_size(mpl->top, &pk_rmpl->frsz);
        que = mpl->que;
        for (;;)
        {   tskid = *que++;
            if (tskid != 0)
            {   if (tskid >= TSK_QUETAIL)
                    break;
                pk_rmpl->wtsk = tskid;
                return_ret_ims(THF_MPL);
            }
        }
        pk_rmpl->wtsk = FALSE;
        return_ret_ims(THF_MPL);
    }
  #if ERC
    vsnd_deb2e(THF_MPL, TFN_REF_MPL, (long)mplid, (long)pk_rmpl, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_CRE_MPF
/************************************************/
/* Create Fixed-size MemoryPool                 */
/************************************************/

ER v4_cre_mpf(ID mpfid, const V4_T_CMPF *pk_cmpf)
{
    T_MPF SNEAR *mpf;
    B PFAR *blf;
    int n, i, atid = (mpfid == ID_AUTO);
    UINT size, coff;
    unsigned blfsz;
    ER ercd;
  #if (ERC && SIZEOF_PTR <= 2)
    UW allsz;
  #else
    SIZE allsz;
  #endif

  #if ERC
    if (INEST)
        ercd = E_CTX;
    else if ((UINT)mpfid > (UINT)CFG.mpfid_max)
        ercd = E_ID;
    else if ((pk_cmpf->blfsz == 0) || (pk_cmpf->blkcnt == 0))
        ercd = E_PAR;
    else
    {
  #endif
    blfsz = ((SIZE)pk_cmpf->blfsz + SIZEOF_INT - 1) & (SIZE)(0 - SIZEOF_INT);
    if (blfsz < SIZEOF_PTR)
        blfsz = SIZEOF_PTR;
  #if (ERC && SIZEOF_INT <= 2)
    allsz = (UW)blfsz * pk_cmpf->blkcnt;
    if (allsz > 0xffffUL)
        ercd = E_PAR;
    else
    {
  #else
    allsz = (SIZE)blfsz * pk_cmpf->blkcnt;
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MPF, TFN_CRE_MPF, (long)mpfid, (long)pk_cmpf);
  #endif
    if (atid)
    {   for (mpfid = CFG.mpfid_max;;)
        {   if (pMPF[mpfid] == NULL)
                break;
            if (--mpfid == 0)
            {   return_ret_ims2(THF_MPF, E_NOID);
            }
        }
        ercd = mpfid;
    }
    else
    {   if (pMPF[mpfid] != NULL)
        {   return_ret_ims2(THF_MPF, E_OBJ);
        }
        ercd = E_OK;
    }
    if ((blf = (B PFAR *)pk_cmpf->mpf) == NULL)
    {   blf = mpl_alloc((SIZE)allsz);
        if (blf == NULL)
        {   return_ret_ims2(THF_MPF, SYSER = E_NOMEM);
        }
    }
    if (pk_cmpf->mpfatr & TA_TPRI)
        n = CFG.tpri_max - 1;
    else
        n = 0;
    size = sizeof(T_MPF) + n * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)pk_cmpf))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        coff = size;
        size = size + sizeof(V4_T_CMPF);
    }
    else
        coff = 0;
    mpf = sys_alloc((SIZE)size);
    if (mpf == NULL)
    {   return_ret_ims2(THF_MPF, SYSER = E_SYS);
    }
    pMPF[mpfid] = mpf;
    if (coff != 0)
    {   mpf->cmpf = (const V4_T_CMPF *)((ADDR)mpf + coff);
        osmem_cpy((DFAR B *)mpf->cmpf, (DFAR B *)pk_cmpf, sizeof(*pk_cmpf));
    }
    else
        mpf->cmpf = pk_cmpf;
    mpf->top = blf;
    mpf->allad = blf;
    mpf->allsz = (unsigned)allsz;
    mpf->cnt = (UH)pk_cmpf->blkcnt;
    for (i = pk_cmpf->blkcnt - 1; i != 0; i--)
        blf = *(B PFAR * PFAR *)blf = blf + blfsz;
    *(B PFAR * PFAR *)blf = NULL;
    for (i = 0; i <= n; i++)
        mpf->que[i] = 0;
    mpf->que[i] = (n==0)?TSK_QUETAIL:TSK_PQUETAIL;
    return_ret_ims2(THF_MPF, ercd);
  #if ERC
  #if (SIZEOF_INT <= 2)
    }
  #endif
    }
    vsnd_deb2e(THF_MPF, TFN_CRE_MPF, (long)mpfid, (long)pk_cmpf, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_CRE_MPF
/************************************************/
/* Create Fixed-size MemoryPool                 */
/************************************************/

ER v3_cre_mpf(ID mpfid, const V3_T_CMPF *pk_cmpf)
{
    V4_T_CMPF cmpf;

    cmpf.mpfatr = pk_cmpf->mpfatr;
    cmpf.blkcnt = pk_cmpf->mpfcnt;
    cmpf.blfsz = pk_cmpf->blfsz;
    cmpf.mpf = NULL;
    cmpf.name = &_KERNEL_NNM;
    return v4_cre_mpf(mpfid, (const V4_T_CMPF *)&cmpf);
}

#endif
#ifdef V4_DEL_MPF
/************************************************/
/* Delete Fixed-size MemoryPool                 */
/************************************************/

ER v4_del_mpf(ID mpfid)
{
    T_MPF SNEAR *mpf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
    int disp = FALSE;
    UINT size;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((mpfid == 0) || ((UINT)mpfid > (UINT)CFG.mpfid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_MPF, TFN_DEL_MPF, (long)mpfid);
  #endif
    mpf = pMPF[mpfid];
    if (mpf == NULL)
    {   return_ret_ims2(THF_MPF, E_NOEXS);
    }
    if (mpf->top == NULL)
    {   que = mpf->que;
        for(;*que < TSK_QUETAIL;++que)
        {
            for (;(tskid = *que) != 0;)
            {   tcb = pTCB[tskid];
                tcb->sp->ercd = E_DLT;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                {   chg_rdq(tcb, tskid);
                    disp |= tcb->pri < NOWPRI;
                }
                else
                    del_que(tcb, tskid);
            }
        }
    }
    if (mpf->cmpf->mpf == NULL)
        mpl_free(mpf->allad, (SIZE)mpf->allsz);
    size = sizeof(T_MPF);
    if (mpf->que[1] < TSK_QUETAIL)
        size = size + (CFG.tpri_max - 1) * sizeof(TSK_ID);
    if (!_kernel_inROM((const void *)mpf->cmpf))
    {   size = (size + SIZEOF_PTR - 1) & (SIZE)(0 - SIZEOF_PTR);
        size = size + sizeof(V4_T_CMPF);
    }
    sys_free(mpf, (SIZE)size);
    pMPF[mpfid] = NULL;
    if (disp)
    {   return_dispatch(THF_MPF);
    }
    else
    {   return_ret_ims(THF_MPF);
    }
  #if ERC
    }
    vsnd_deb1e(THF_MPF, TFN_DEL_MPF, (long)mpfid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_TGET_MPF
/************************************************/
/* Get Fixed-size Memory Block with Timeout     */
/************************************************/

ER v4_tget_mpf(ID mpfid, VP p_blf, TMO tmout)
{
    T_MPF SNEAR *mpf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    B PFAR *blf;
    ID tskid;
  #if ERC
    ER ercd;

    if ((mpfid == 0) || ((UINT)mpfid > (UINT)CFG.mpfid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb3(THF_MPF, ((tmout==TMO_FEVR)?TFN_GET_MPF:((tmout==TMO_POL)?TFN_PGET_MPF:TFN_TGET_MPF)), (long)mpfid, (long)p_blf, (long)tmout);
  #endif
    mpf = pMPF[mpfid];
    if (mpf == NULL)
    {   return_ret_ims2(THF_MPF, E_NOEXS);
    }
    if (mpf->top == NULL)
    {   if (tmout != TMO_POL)
        {
          #if ERC
            if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
            {   return_ret_ims2(THF_MPF, SYSER = E_CTX);
            }
          #endif
            tskid = RDQ[0];
            tcb = pTCB[tskid];
            if (tmout != TMO_FEVR)
            {   add_tmq(tmout, tcb, tskid);
                tcb->sts = (UB)(S_MPF|S_TMO);
            }
            else
                tcb->sts = S_MPF;
            CTXPTR = p_blf;
            set_wid(mpfid);
            que = mpf->que;
            if (*(que + 1) < TSK_QUETAIL)
                que += NOWPRI - 1;
            return chg_que_dispatch1_(THF_MPF, que, tcb, tskid);
        }
        return_ret_ims2(THF_MPF, E_TMOUT);
    }
    blf = mpf->top;
    *(B PFAR * PFAR *)p_blf = blf;
    mpf->top = *(B PFAR * PFAR *)blf;
    mpf->cnt--;
    return_ret_ims(THF_MPF);
  #if ERC
    }
    vsnd_deb3e(THF_MPF, ((tmout==TMO_FEVR)?TFN_GET_MPF:((tmout==TMO_POL)?TFN_PGET_MPF:TFN_TGET_MPF)), (long)mpfid, (long)p_blf, (long)tmout, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REL_MPF
/************************************************/
/* Release Fixed-size Memory Block              */
/************************************************/

ER v4_rel_mpf(ID mpfid, VP blf)
{
    T_MPF SNEAR *mpf;
    T_TCB SNEAR *tcb;
    TSK_ID SNEAR *que;
    ID tskid;
  #if ERC
    ER ercd;

    if ((mpfid == 0) || ((UINT)mpfid > (UINT)CFG.mpfid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_MPF, TFN_REL_MPF, (long)mpfid, (long)blf);
  #endif
    mpf = pMPF[mpfid];
    if (mpf == NULL)
    {   return_ret_ims2(THF_MPF, E_NOEXS);
    }
    if (((ADDR)blf &  (ADDR)(SIZEOF_INT - 1))
      ||((ADDR)blf <  (ADDR)mpf->allad)
      ||((ADDR)blf >= (ADDR)(mpf->allad + mpf->allsz)))
    {   return_ret_ims2(THF_MPF, E_PAR);
    }
    if (mpf->top == NULL)
    {   que = mpf->que;
        for (;(tskid = *que) < TSK_QUETAIL;que++)
        {
            if (tskid != 0)
            {   tcb = pTCB[tskid];
                *(B PFAR * PFAR *)tcb->sp->ptr = (B PFAR *)blf;
                if (tcb->sts & S_TMO)
                    del_tmq(tcb, tskid);
                tcb->sts = S_RDY;
                if (tcb->sus == 0)
                    return chg_rdq_dispatch_(THF_MPF, tcb, tskid);
                else
                    return del_que_ret_ims_(THF_MPF, tcb, tskid);
            }
        }
    }
    *(B PFAR * PFAR *)blf = mpf->top;
    mpf->top = blf;
    mpf->cnt++;
    return_ret_ims(THF_MPF);
  #if ERC
    }
    vsnd_deb2e(THF_MPF, TFN_REL_MPF, (long)mpfid, (long)blf, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_MPF
/************************************************/
/* Refer Fixed-size MemoryPool Status           */
/************************************************/

ER v4_ref_mpf(ID mpfid, V4_T_RMPF *pk_rmpf)
{
    T_MPF SNEAR *mpf;
    TSK_ID SNEAR *que;
    ID tskid;
  #if ERC
    ER ercd;

    if ((mpfid == 0) || ((UINT)mpfid > (UINT)CFG.mpfid_max))
        SYSER = ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_MPF, TFN_REF_MPF, (long)mpfid, (long)pk_rmpf);
      #endif
        mpf = pMPF[mpfid];
        if (mpf == NULL)
        {   return_ret_ims2(THF_MPF, E_NOEXS);
        }
        pk_rmpf->fblkcnt = mpf->cnt;
        if (mpf->top == NULL)
        {   que = mpf->que;
            for (;(tskid = *que) == 0;que++);
            if (tskid < TSK_QUETAIL)
            {   pk_rmpf->wtskid = tskid;
                return_ret_ims(THF_MPF);
            }
        }
        pk_rmpf->wtskid = TSK_NONE;
        return_ret_ims(THF_MPF);
  #if ERC
    }
    vsnd_deb2e(THF_MPF, TFN_REF_MPF, (long)mpfid, (long)pk_rmpf, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_MPF
/************************************************/
/* Refer Fixed-size MemoryPool Status           */
/************************************************/

ER v3_ref_mpf(V3_T_RMPF *pk_rmpf, ID mpfid)
{
    T_MPF SNEAR *mpf;
    TSK_ID SNEAR *que;
    ID tskid;
    ER ercd;

  #if ERC
    if (mpfid == 0)
        SYSER = ercd = E_ID;
    else
  #endif
    if ((UINT)mpfid > (UINT)CFG.mpfid_max)
        ercd = E_ID;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_MPF, TFN_REF_MPF, (long)mpfid, (long)pk_rmpf);
      #endif
        mpf = pMPF[mpfid];
        if (mpf == NULL)
        {   return_ret_ims2(THF_MPF, E_NOEXS);
        }
        pk_rmpf->exinf = 0;
        pk_rmpf->frbcnt = mpf->cnt;
        if (mpf->top == NULL)
        {   que = mpf->que;
            for (;;)
            {   tskid = *que++;
                if (tskid != 0)
                {   if (tskid >= TSK_QUETAIL)
                        break;
                    pk_rmpf->wtsk = tskid;
                    return_ret_ims(THF_MPF);
                }
            }
        }
        pk_rmpf->wtsk = FALSE;
        return_ret_ims(THF_MPF);
    }
  #if ERC
    vsnd_deb2e(THF_MPF, TFN_REF_MPF, (long)mpfid, (long)pk_rmpf, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_SET_TIM
/************************************************/
/* Set Absolute Time                            */
/************************************************/
ER v4_set_tim(V4_SYSTIM *pk_tim)
{
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_SET_TIM, (long)pk_tim);
  #endif
    _kernel_clk.ltime = pk_tim->ltime - SYSCK.ltime;
  #if (CLKBITS>32)
    _kernel_clk.utime = pk_tim->utime - SYSCK.utime;
    if (pk_tim->ltime < SYSCK.ltime)
        _kernel_clk.utime--;
  #endif
    return_ret_ims(THF_TIM);
}

#endif
#ifdef V3_SET_TIM
/************************************************/
/* Set System Time                              */
/************************************************/

ER v3_set_tim(const SYSTIME *pk_tim)
{
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_SET_TIM, (long)pk_tim);
  #endif
  #if (CLKBITS>32)
    SYSCK.utime = pk_tim->utime;
  #endif
    SYSCK.ltime = pk_tim->ltime;
    return_ret_ims(THF_TIM);
}

#endif
#ifdef V4_GET_TIM
/************************************************/
/* Get Absolute Time                            */
/************************************************/

ER v4_get_tim(V4_SYSTIM *pk_tim)
{
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_GET_TIM, (long)pk_tim);
  #endif
    pk_tim->ltime = SYSCK.ltime + _kernel_clk.ltime;
  #if (CLKBITS>32)
    pk_tim->utime = SYSCK.utime + _kernel_clk.utime;
    if (pk_tim->ltime < SYSCK.ltime)
        pk_tim->utime++;
  #endif
    return_ret_ims(THF_TIM);
}

#endif
#ifdef V3_GET_TIM
/************************************************/
/* Get Time                                     */
/************************************************/

ER v3_get_tim(SYSTIME *pk_tim)
{
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_GET_TIM, (long)pk_tim);
  #endif
  #if (CLKBITS>32)
    pk_tim->utime = SYSCK.utime;
  #endif
    pk_tim->ltime = SYSCK.ltime;
    return_ret_ims(THF_TIM);
}

#endif
#ifdef V4_DLY_TSK
/************************************************/
/* Delay Task                                   */
/************************************************/

ER v4_dly_tsk(RELTIM dlytim)
{
    T_TCB SNEAR *tcb;
    ID tskid;

  #if ERC
    if ((INEST)||(DDISP)||(RDQ[0] == (TSK_ID)(CFG.tskid_max+1)))
    {   vsnd_deb1e(THF_TIM, TFN_DLY_TSK, (long)dlytim, E_CTX);
        return SYSER = E_CTX;
    }
  #endif
    tskid = RDQ[0];
    tcb = pTCB[tskid];
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_DLY_TSK, (long)dlytim);
  #endif
    if (dlytim != 0)
    {   add_tmq(dlytim, tcb, tskid);
        tcb->sts = (UB)(S_DLY|S_TMO);
        return del_que_dispatch1_(THF_TIM, tcb, tskid);
    }
    return_ret_ims2(THF_TIM, E_OK);
}

#endif
#ifdef V4_CRE_CYC
/************************************************/
/* Define Cyclic Handler                        */
/************************************************/

ER v4_cre_cyc(ID cycno, const V4_T_CCYC *pk_ccyc)
{
    T_CYC SNEAR *cyc;
    ATR atr;
    CYCTIME cycphs;
    int atid = (cycno == ID_AUTO);
    ER ercd;
  #if ERC

    if (INEST)
        ercd = E_CTX;
    else if (!atid && ((cycno == 0) || ((UINT)cycno > (UINT)CFG.cycno_max)))
        ercd = E_ID;
    else if (pk_ccyc == NULL)
        ercd = E_PAR;
    else
    {
  #endif
    if ((atr = pk_ccyc->cycatr) & TA_STA)
        atr |= TCY_ON;
    else
        atr &= (ATR)-2;
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_CRE_CYC, (long)cycno, (long)pk_ccyc);
  #endif
    if (atid)
    {   for (cycno = CFG.cycno_max; pCYC[cycno] != NULL;)
        {   if (--cycno == 0)
            {   return_ret_ims2(THF_TIM, E_NOID);
            }
        }
        ercd = cycno;
    }
    else
    {   if (pCYC[cycno] != NULL)
        {   return_ret_ims2(THF_TIM, E_OBJ);
        }
        ercd = E_OK;
    }
    cyc = sys_alloc((SIZE)sizeof (T_CYC));
    if (cyc == NULL)
    {   return_ret_ims2(THF_TIM, SYSER = E_SYS);
    }
    pCYC[cycno] = cyc;
    cyc->ntm = 0;
    cyc->ptm = 0;
    #if (CLKBITS>32)
    cyc->cyctim.utime = cycphs.utime = 0;
    #endif
    cyc->cyctim.ltime = pk_ccyc->cyctim;
    cycphs.ltime = (pk_ccyc->cycphs==0)?pk_ccyc->cyctim:pk_ccyc->cycphs;
    cyc->cychdr = pk_ccyc->cychdr;
    cyc->exinf = pk_ccyc->exinf;
    cyc->cycact = atr;
    if (atr & (TA_STA|TA_PHS))
    {   tim_add((const SYSTIME *)&cycphs, &SYSCK, cyc);
        add_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
    }
    return_ret_ims2(THF_TIM, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_CRE_CYC, (long)cycno, (long)pk_ccyc, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_DEF_CYC
/************************************************/
/* Define Cyclic Handler                        */
/************************************************/

ER v3_def_cyc(HNO cycno, const V3_T_DCYC *pk_dcyc)
{
    T_CYC SNEAR *cyc;
    int atid = (cycno == ID_AUTO);
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (!atid && ((cycno == 0) || ((UINT)cycno > (UINT)CFG.cycno_max)))
        ercd = E_PAR;
    else if (pk_dcyc != (const V3_T_DCYC *)NADR && pk_dcyc->cycact & ~(TCY_ON|TCY_OFF))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_DEF_CYC, (long)cycno, (long)pk_dcyc);
  #endif
    if (atid)
    {   for (cycno = CFG.cycno_max; pCYC[cycno] != NULL;)
        {   if (--cycno == 0)
            {   return_ret_ims2(THF_TIM, E_NOID);
            }
        }
        cyc = NULL;
    }
    else  cyc = pCYC[cycno];
    if (pk_dcyc == (const V3_T_DCYC *)NADR)
    {   if (cyc != NULL)
        {   del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
            sys_free(cyc, (SIZE)sizeof (T_CYC));
            pCYC[cycno] = NULL;
        }
        return_ret_ims2(THF_TIM, E_OK);
    }
    if (cyc == NULL)
    {   cyc = sys_alloc((SIZE)sizeof (T_CYC));
        if (cyc == NULL)
        {   return_ret_ims2(THF_TIM, SYSER = E_SYS);
        }
    }
    else
        del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
    pCYC[cycno] = cyc;
    cyc->ntm = 0;
    cyc->ptm = 0;
    #if (CLKBITS>32)
    cyc->cyctim.utime = pk_dcyc->cyctim.utime;
    #endif
    cyc->cyctim.ltime = pk_dcyc->cyctim.ltime;
    cyc->cychdr = pk_dcyc->cychdr;
    cyc->exinf = pk_dcyc->exinf;
    cyc->cycact = (UINT)pk_dcyc->cycact;
    tim_add((const SYSTIME *)&cyc->cyctim, &SYSCK, cyc);
    add_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
    if (atid)
    {   return_ret_ims2(THF_TIM, cycno);
    }
    else
    {   return_ret_ims(THF_TIM);
    }
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_DEF_CYC, (long)cycno, (long)pk_dcyc, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_DEL_CYC
/************************************************/
/* Delete Cyclic Handler                        */
/************************************************/

ER v4_del_cyc(ID cycid)
{
    T_CYC SNEAR *cyc;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((cycid == 0) || ((UINT)cycid > (UINT)CFG.cycno_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_DEL_CYC, (long)cycid);
  #endif
    cyc = pCYC[cycid];
    if (cyc == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (cyc->cycact & (TCY_ON | TA_PHS))
        del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycid);
    sys_free(cyc, (SIZE)sizeof (T_CYC));
    pCYC[cycid] = NULL;
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_DEL_CYC, (long)cycid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STA_CYC
/************************************************/
/* Start Cyclic Handler                         */
/************************************************/

ER v4_sta_cyc(ID cycid)
{
    T_CYC SNEAR *cyc;
  #if ERC
    ER ercd;

    if ((cycid == 0) || ((UINT)cycid > (UINT)CFG.cycno_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_STA_CYC, (long)cycid);
  #endif
    cyc = pCYC[cycid];
    if (cyc == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (cyc->cycact & TA_PHS)
        cyc->cycact |= TCY_ON;
    else
    {   if (cyc->cycact & TCY_ON)
        {    del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycid);
            cyc->ntm = 0;
            cyc->ptm = 0;
        }
        tim_add((const SYSTIME *)&cyc->cyctim, &SYSCK, cyc);
        add_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycid);
        cyc->cycact |= TCY_ON;
    }
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_STA_CYC, (long)cycid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STP_CYC
/************************************************/
/* Stop Cyclic Handler                          */
/************************************************/

ER v4_stp_cyc(ID cycid)
{
    T_CYC SNEAR *cyc;
  #if ERC
    ER ercd;

    if ((cycid == 0) || ((UINT)cycid > (UINT)CFG.cycno_max))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_STP_CYC, (long)cycid);
  #endif
    cyc = pCYC[cycid];
    if (cyc == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (cyc->cycact & TCY_ON)
    {
        if (!(cyc->cycact & TA_PHS))
        {   del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycid);
            cyc->ntm = 0;
            cyc->ptm = 0;
        }
        cyc->cycact &= ~TCY_ON;
    }
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_STP_CYC, (long)cycid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_ACT_CYC
/************************************************/
/* Activate Cyclic Handler                      */
/************************************************/

ER v3_act_cyc(HNO cycno, UINT cycact)
{
    T_CYC SNEAR *cyc;
  #if ERC
    ER ercd;

    if ((cycno == 0) || ((UINT)cycno > (UINT)CFG.cycno_max))
        ercd = E_PAR;
    else if (cycact & ~(TCY_ON|TCY_OFF|TCY_INI))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_ACT_CYC, (long)cycno, (long)cycact);
  #endif
    cyc = pCYC[cycno];
    if (cyc == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    cyc->cycact = cycact;
    if (cycact & TCY_INI)
    {   del_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
        cyc->ntm = 0;
        cyc->ptm = 0;
        tim_add((const SYSTIME *)&cyc->cyctim, &SYSCK, cyc);
        add_thq((T_CYC SNEAR * SNEAR *)pCYC, cyc, cycno);
    }
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_ACT_CYC, (long)cycno, (long)cycact, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_CYC
/************************************************/
/* Refer Cyclic Handler Status                  */
/************************************************/

ER v4_ref_cyc(ID cycno, V4_T_RCYC *pk_rcyc)
{
    T_CYC SNEAR *cyc;
    SYSTIME now, next;
  #if ERC
    ER ercd;

    if ((cycno == 0) || ((UINT)cycno > (UINT)CFG.cycno_max))
        ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_TIM, TFN_REF_CYC, (long)cycno, (long)pk_rcyc);
      #endif
        cyc = pCYC[cycno];
        if (cyc == NULL)
        {   return_ret_ims2(THF_TIM, E_NOEXS);
        }
      #if (CLKBITS>32)
        now.utime = SYSCK.utime;
      #endif
        now.ltime = SYSCK.ltime;
        pk_rcyc->cycstat = (cyc->cycact&TCY_ON)?TCYC_STA:TCYC_STP;
        tim_dif((SYSTIME SNEAR *)&now, (SYSTIME SNEAR *)&cyc->utime, (SYSTIME DFAR *)&next);
        pk_rcyc->lefttim = (RELTIM) next.ltime;
        return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_REF_CYC, (long)cycno, (long)pk_rcyc, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_CYC
/************************************************/
/* Refer Cyclic Handler Status                  */
/************************************************/

ER v3_ref_cyc(V3_T_RCYC *pk_rcyc, HNO cycno)
{
    T_CYC SNEAR *cyc;
    SYSTIME now;
    ER ercd;

  #if ERC
    if (cycno == 0)
        SYSER = ercd = E_PAR;
    else
  #endif
    if (cycno > CFG.cycno_max)
        ercd = E_PAR;
    else
    {
        dis_ims();
      #if ERC
        visnd_deb2(THF_TIM, TFN_REF_CYC, (long)cycno, (long)pk_rcyc);
      #endif
        cyc = pCYC[cycno];
        if (cyc == NULL)
        {   return_ret_ims2(THF_TIM, E_NOEXS);
        }
      #if (CLKBITS>32)
        now.utime = SYSCK.utime;
      #endif
        now.ltime = SYSCK.ltime;
        pk_rcyc->exinf = cyc->exinf;
        tim_dif((SYSTIME SNEAR *)&now, (SYSTIME SNEAR *)&cyc->utime, (SYSTIME DFAR *)&pk_rcyc->lfttim);
        pk_rcyc->cycact = (UINT)(cyc->cycact & (TCY_ON | TCY_OFF));
        return_ret_ims(THF_TIM);
    }
  #if ERC
    vsnd_deb2e(THF_TIM, TFN_REF_CYC, (long)cycno, (long)pk_rcyc, (long)ercd);
  #endif
    return ercd;
}

#endif
#ifdef V4_CRE_ALM
/************************************************/
/* Define Alarm Handler                         */
/************************************************/

ER v4_cre_alm(ID almno, const V4_T_CALM *pk_calm)
{
    T_ALM SNEAR *alm;
    int atid = (almno == ID_AUTO);
    ER ercd;

  #if ERC
    if (INEST)
        ercd = E_CTX;
    else if (!atid && ((almno == 0) || ((UINT)almno > (UINT)CFG.almno_max)))
        ercd = E_ID;
    else if (pk_calm == NULL)
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_CRE_ALM, (long)almno, (long)pk_calm);
  #endif
    if (atid)
    {   for (almno = CFG.almno_max; pALM[almno] != NULL;)
        {   if (--almno == 0)
            {   return_ret_ims2(THF_TIM, E_NOID);
            }
        }
        ercd = almno;
    }
    else
    {   if (pALM[almno] != NULL)
        {   return_ret_ims2(THF_TIM, E_OBJ);
        }
        ercd = E_OK;
    }
    alm = sys_alloc((SIZE)sizeof (T_ALM));
    if (alm == NULL)
    {   return_ret_ims2(THF_TIM, SYSER = E_SYS);
    }
    pALM[almno] = alm;
    alm->exinf = pk_calm->exinf;
    alm->almhdr = pk_calm->almhdr;
    alm->ntm = 0;
    alm->ptm = 0;
    return_ret_ims2(THF_TIM, ercd);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_CRE_ALM, (long)almno, (long)pk_calm, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_DEF_ALM
/************************************************/
/* Define Alarm Handler                         */
/************************************************/

ER v3_def_alm(HNO almno, const V3_T_DALM *pk_dalm)
{
    T_ALM SNEAR *alm;
    int atid = (almno == ID_AUTO);
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if (!atid && ((almno == 0) || ((UINT)almno > (UINT)CFG.almno_max)))
        ercd = E_PAR;
    else if (pk_dalm != (const V3_T_DALM *)NADR && pk_dalm->tmmode & ~(TTM_ABS|TTM_REL))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_DEF_ALM, (long)almno, (long)pk_dalm);
  #endif
    if (atid)
    {   for (almno = CFG.almno_max;;)
        {   if (pALM[almno] == NULL)
                break;
            if (--almno == 0)
            {   return_ret_ims2(THF_TIM, E_PAR);
            }
        }
        alm = NULL;
    }
    else
        alm = pALM[almno];
    if (pk_dalm == (V3_T_DALM *)NADR)
    {   if (alm != NULL)
        {   if (alm->ntm != 0)
                del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almno);
            sys_free(alm, (SIZE)sizeof (T_ALM));
            pALM[almno] = NULL;
        }
        return_ret_ims(THF_TIM);
    }
    if (alm == NULL)
    {   alm = sys_alloc((SIZE)sizeof (T_ALM));
        if (alm == NULL)
        {   return_ret_ims2(THF_TIM, SYSER = E_SYS);
        }
    }
    else if (alm->ntm != 0)
        del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almno);
    pALM[almno] = alm;
    alm->exinf = pk_dalm->exinf;
    alm->almhdr = pk_dalm->almhdr;
    alm->ntm = 0;
    alm->ptm = 0;
    if (pk_dalm->tmmode == TTM_REL)
        tim_add((const SYSTIME *)&pk_dalm->almtim, &SYSCK, (T_CYC SNEAR *)alm);
    else
    {
      #if (CLKBITS>32)
        alm->utime = pk_dalm->almtim.utime;
      #endif
        alm->ltime = pk_dalm->almtim.ltime;
    }
    add_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almno);
    if (atid)
    {   return_ret_ims2(THF_TIM, almno);
    }
    else
    {   return_ret_ims(THF_TIM);
    }
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_DEF_ALM, (long)almno, (long)pk_dalm, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_DEL_ALM
/************************************************/
/* Delete Alarm Handler                         */
/************************************************/

ER v4_del_alm(ID almid)
{
    T_ALM SNEAR *alm;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((almid == 0) || ((UINT)almid > (UINT)CFG.almno_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_DEL_ALM, (long)almid);
  #endif
    alm = pALM[almid];
    if (alm == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (alm->ntm != 0)
        del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almid);
    sys_free(alm, (SIZE)sizeof (T_ALM));
    pALM[almid] = NULL;
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_DEL_ALM, (long)almid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STA_ALM
/************************************************/
/* Start Alarm Handler                          */
/************************************************/

ER v4_sta_alm(ID almid, RELTIM almtim)
{
    T_ALM SNEAR *alm;
    SYSTIME almtime;
  #if ERC
    ER ercd;

    if ((almid == 0) || ((UINT)almid > (UINT)CFG.almno_max))
        ercd = E_ID;
    else
    {
  #endif
    #if (CLKBITS>32)
    almtime.utime = 0;
    #endif
    almtime.ltime = almtim;
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_STA_ALM, (long)almid, (long)almtim);
  #endif
    alm = pALM[almid];
    if (alm == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (alm->ntm != 0)
    {   del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almid);
        alm->ntm = 0;
        alm->ptm = 0;
    }
    tim_add((const SYSTIME *)&almtime, &SYSCK, (T_CYC SNEAR *)alm);
    add_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almid);
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_STA_ALM, (long)almid, (long)almtim, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STP_ALM
/************************************************/
/* Stop Alarm Handler                           */
/************************************************/

ER v4_stp_alm(ID almid)
{
    T_ALM SNEAR *alm;
  #if ERC
    ER ercd;

    if ((almid == 0) || ((UINT)almid > (UINT)CFG.almno_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_STP_ALM, (long)almid);
  #endif
    alm = pALM[almid];
    if (alm == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    if (alm->ntm != 0)
    {   del_thq((T_CYC SNEAR * SNEAR *)pALM, (T_CYC SNEAR *)alm, almid);
        alm->ntm = 0;
        alm->ptm = 0;
    }
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_STP_ALM, (long)almid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_ALM
/************************************************/
/* Refer Alarm Handler Status                   */
/************************************************/

ER v4_ref_alm(ID almno, V4_T_RALM *pk_ralm)
{
    T_ALM SNEAR *alm;
    SYSTIME left;
  #if ERC
    ER ercd;

    if ((almno == 0) || ((UINT)almno > (UINT)CFG.almno_max))
        ercd = E_ID;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_TIM, TFN_REF_ALM, (long)almno, (long)pk_ralm);
      #endif
        alm = pALM[almno];
        if (alm == NULL)
        {   return_ret_ims2(THF_TIM, E_NOEXS);
        }
        pk_ralm->almstat = (alm->ntm==0)?TALM_STP:TALM_STA;
        tim_dif((SYSTIME SNEAR *)&SYSCK, (SYSTIME SNEAR *)alm, (SYSTIME DFAR *)&left);
        pk_ralm->lefttim = left.ltime;
        return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_REF_ALM, (long)almno, (long)pk_ralm, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V3_REF_ALM
/************************************************/
/* Refer Alarm Handler Status                   */
/************************************************/

ER v3_ref_alm(V3_T_RALM *pk_ralm, HNO almno)
{
    T_ALM SNEAR *alm;
    SYSTIME now;
  #if ERC
    ER ercd;

    if ((almno == 0) || ((UINT)almno > (UINT)CFG.almno_max))
        ercd = E_PAR;
    else
    {
  #endif
        dis_ims();
      #if ERC
        visnd_deb2(THF_TIM, TFN_REF_ALM, (long)almno, (long)pk_ralm);
      #endif
        alm = pALM[almno];
        if (alm == NULL)
        {   return_ret_ims2(THF_TIM, E_NOEXS);
        }
        pk_ralm->exinf = alm->exinf;
      #if (CLKBITS>32)
        now.utime = SYSCK.utime;
      #endif
        now.ltime = SYSCK.ltime;
        tim_dif((SYSTIME SNEAR *)&now, (SYSTIME SNEAR *)&alm->utime, (SYSTIME DFAR *)&pk_ralm->lfttim);
        return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_REF_ALM, (long)almno, (long)pk_ralm, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V4_DEF_OVR

ER cre_isr_st(T_ISR_ST SNEAR *isr, const T_CISR *pk_cisr)
{
    T_ISR_ST SNEAR *isr2;
    B SNEAR *hdr;
    VP *p;

    if ((ADDR)_kernel_get_inthdr(pk_cisr) == (ADDR)_kernel_interr)
    {   hdr = (B SNEAR *)sys_alloc((SIZE)_KERNEL_hdrlen);
        if (hdr == NULL)
        {   isr->prev = NULL;
            return SYSER = E_SYS;
        }
        osmem_cpy((DFAR B *)hdr, (DFAR B *)_kernel_interr, _KERNEL_hdrlen);
        *(T_ISR_ST SNEAR * SNEAR *)(hdr + _KERNEL_pisrposi) = isr;
        isr->prev = (T_ISR_ST SNEAR * SNEAR *)(hdr + _KERNEL_pisrposi);
        _kernel_set_inthdr((FP)hdr, pk_cisr);
    }
    else
    {
        isr2 = ((T_ISR_ST SNEAR *)*(VP *)(p = (VP *)((ADDR)_kernel_get_inthdr(pk_cisr) + _KERNEL_pisrposi)));
                                          /* Warning change far pointer to near pointer */
        if (isr2 == NULL)
        {   *p = isr;
            isr->prev = (T_ISR_ST SNEAR * SNEAR *)p;
        }
        else
        {
            while(isr2->next != NULL)
            {
                isr2 = isr2->next;
            }
            isr2->next = isr;
            isr->prev = &(isr2->next);
        }
    }
    isr->next = NULL;
    return E_OK;
}

void del_isr_st(T_ISR_ST SNEAR *isr)
{
    if (isr->prev != NULL)
    {   *isr->prev = isr->next;
        if (isr->next != NULL)
            (isr->next)->prev = isr->prev;
        isr->prev = NULL;
    }
}

/************************************************/
/* Define Overrun Handler                       */
/************************************************/

ER v4_def_ovr(const V4_T_DOVR *pk_dovr)
{
    ER ercd;
    T_CISR cisr;

  #if ERC
    if (INEST)
        ercd = E_CTX;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_DEF_OVR, (long)pk_dovr);
  #endif
    if (pk_dovr == NULL)
    {   _kernel_ovr.otskid = TSK_NONE;
        _kernel_ovr.ovrhdr = _kernel_ovr.ovrclr = NULL;
        del_isr_st(&_kernel_ovrisr);
        return_ret_ims(THF_TIM);
    }
    if (_kernel_ovrisr.prev != NULL)
    {
        del_isr_st(&_kernel_ovrisr);
    }
    _kernel_ovrisr.isr = (FP)sig_ovr;
    cisr.intno = pk_dovr->intno;
    cisr.imask = pk_dovr->imask;
    if ((ercd = cre_isr_st(&_kernel_ovrisr, (const T_CISR *)&cisr)) == E_OK)
    {   _kernel_ovr.ovrhdr = pk_dovr->ovrhdr;
        _kernel_ovr.ovrclr = pk_dovr->ovrclr;
        _kernel_ovr.otskid = TSK_NONE;
        return_ret_ims(THF_TIM);
    }
    else
    {   return_ret_ims2(THF_TIM, ercd);
    }
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_DEF_OVR, (long)pk_dovr, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STA_OVR
/************************************************/
/* Start Overrun Handler                        */
/************************************************/

ER v4_sta_ovr(ID tskid, OVRTIM ovrtim)
{
    T_TCB SNEAR *tcb;
  #if ERC
    ER ercd;
  #endif

    if (tskid == TSK_SELF)
        tskid = RDQ[0];
  #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_STA_OVR, (long)tskid, (long)ovrtim);
  #endif
    if (_kernel_ovr.ovrhdr == NULL)
    {   return_ret_ims2(THF_TIM, E_OBJ);
    }
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    tcb->rcnt = ovrtim;
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_STA_OVR, (long)tskid, (long)ovrtim, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_STP_OVR
/************************************************/
/* Stop Overrun Handler                         */
/************************************************/

ER v4_stp_ovr(ID tskid)
{
    T_TCB SNEAR *tcb;
  #if ERC
    ER ercd;
  #endif

    if (tskid == TSK_SELF)
        tskid = RDQ[0];
  #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_TIM, TFN_STP_OVR, (long)tskid);
  #endif
    if (_kernel_ovr.ovrhdr == NULL)
    {   return_ret_ims2(THF_TIM, E_OBJ);
    }
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    tcb->rcnt = 0;
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb1e(THF_TIM, TFN_STP_OVR, (long)tskid, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_REF_OVR
/************************************************/
/* Refer Overrun Handler                        */
/************************************************/

ER v4_ref_ovr(ID tskid, V4_T_ROVR *pk_rovr)
{
    T_TCB SNEAR *tcb;
  #if ERC
    ER ercd;
  #endif

    if (tskid == TSK_SELF)
        tskid = RDQ[0];
  #if ERC
    if ((UINT)tskid > (UINT)CFG.tskid_max)
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_TIM, TFN_REF_OVR, (long)tskid, (long)pk_rovr);
  #endif
    if (_kernel_ovr.ovrhdr == NULL)
    {   return_ret_ims2(THF_TIM, E_OBJ);
    }
    tcb = pTCB[tskid];
    if (tcb == NULL)
    {   return_ret_ims2(THF_TIM, E_NOEXS);
    }
    pk_rovr->ovrstat = (pk_rovr->leftotm = tcb->rcnt)==0?TOVR_STP:TOVR_STA;
    return_ret_ims(THF_TIM);
  #if ERC
    }
    vsnd_deb2e(THF_TIM, TFN_REF_OVR, (long)tskid, (long)pk_rovr, (long)ercd);
    return ercd;
  #endif
}

#endif
#ifdef V4_CRE_ISR
/************************************************/
/* Create Interrupt Service Routine             */
/************************************************/

ER v4_cre_isr(ID isrid, const V4_T_CISR *pk_cisr)
{
    T_ISR SNEAR *isr, *isr2;
    int atid = (isrid == ID_AUTO);
    VP *p;
    B SNEAR *hdr;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((UINT)isrid > (UINT)CFG.isrid_max)
        ercd = E_ID;
    else if ((ercd = (_kernel_check_cisr(pk_cisr))) != E_OK)
        ;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_INT, TFN_CRE_ISR, (long)isrid, (long)pk_cisr);
  #endif
    if (atid)
    {
        for (isrid = CFG.isrid_max;;)
        {   if (pISR[isrid] == NULL)
            {   atid = isrid;
                break;
            }
            if (--isrid == 0)
            {   return_ret_ims2(THF_INT, E_NOID);
            }
        }
    }
    else if (pISR[isrid] != NULL)
    {   return_ret_ims2(THF_INT, E_OBJ);
    }
    if ((isr = (T_ISR SNEAR *)sys_alloc((SIZE)sizeof (T_ISR))) == NULL)
    {   return_ret_ims2(THF_INT, SYSER = E_SYS);
    }
    if ((ADDR)_kernel_get_inthdr(pk_cisr) == (ADDR)_kernel_interr)
    {   hdr = (B SNEAR *)sys_alloc((SIZE)_KERNEL_hdrlen);
        if (hdr == NULL)
        {   sys_free(isr, (SIZE)sizeof(T_ISR));
            return_ret_ims2(THF_INT, SYSER = E_SYS);
        }
        osmem_cpy((DFAR B *)hdr, (DFAR B *)_kernel_interr, _KERNEL_hdrlen);
        *(T_ISR SNEAR * SNEAR *)(hdr + _KERNEL_pisrposi) = isr;
        isr->prev = (T_ISR SNEAR * SNEAR *)(hdr + _KERNEL_pisrposi);
        _kernel_set_inthdr((FP)hdr, pk_cisr);
    }
    else
    {
        isr2 = ((T_ISR SNEAR *)*(VP *)(p = (VP *)((ADDR)_kernel_get_inthdr(pk_cisr) + _KERNEL_pisrposi)));
                                       /* Warning change far pointer to near pointer */
        if (isr2 == NULL)
        {   *p = isr;
            isr->prev = (T_ISR SNEAR * SNEAR *)p;
        }
        else
        {
            while(isr2->next != NULL)
            {
            #if ERC
                if (isr2->next->prev != &isr2->next)
                {   sys_free(isr, (SIZE)sizeof(T_ISR));
                    return_ret_ims2(THF_INT, SYSER = E_ILUSE);
                }
            #endif
                isr2 = isr2->next;
            }
            isr2->next = isr;
            isr->prev = &(isr2->next);
        }
    }
    isr->isr = pk_cisr->isr;
    isr->exinf = pk_cisr->exinf;
    isr->next = NULL;
    isr->intno = pk_cisr->intno;
    pISR[isrid] = isr;
    return_ret_ims2(THF_INT, atid);
  #if ERC
    }
    vsnd_deb2e(THF_INT, TFN_CRE_ISR, (long)isrid, (long)pk_cisr, (long)ercd);
    return SYSER = ercd;
  #endif
}
#endif

#ifdef V4_DEL_ISR
/************************************************/
/* Delete Interrupt Service Routine             */
/************************************************/

ER v4_del_isr(ID isrid)
{
    T_ISR SNEAR *isr;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((isrid == 0) || ((UINT)isrid > (UINT)CFG.isrid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb1(THF_INT, TFN_DEL_ISR, (long)isrid);
  #endif
    if ((isr = pISR[isrid]) == NULL)
    {   return_ret_ims2(THF_INT, E_NOEXS);
    }
    if (isr->prev != NULL)
    {   *isr->prev = isr->next;
        if (isr->next != NULL)
            (isr->next)->prev = isr->prev;
    }
    sys_free(isr, (SIZE)sizeof(T_ISR));
    pISR[isrid] = NULL;
    return_ret_ims(THF_INT);
  #if ERC
    }
    vsnd_deb1e(THF_INT, TFN_DEL_ISR, (long)isrid, (long)ercd);
    return SYSER = ercd;
  #endif
}
#endif

#ifdef V4_REF_ISR
/************************************************/
/* Refer Interrupt Service Routine              */
/************************************************/

ER v4_ref_isr(ID isrid, V4_T_RISR *pk_risr)
{
    T_ISR SNEAR *isr;
  #if ERC
    ER ercd;

    if ((isrid == 0) || ((UINT)isrid > (UINT)CFG.isrid_max))
        ercd = E_ID;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_INT, TFN_REF_ISR, (long)isrid, (long)pk_risr);
  #endif
    if ((isr = pISR[isrid]) == NULL)
    {   return_ret_ims2(THF_INT, E_NOEXS);
    }
    pk_risr->intno = isr->intno;

    return_ret_ims(THF_INT);
  #if ERC
    }
    vsnd_deb2e(THF_INT, TFN_DEL_ISR, (long)isrid, (long)pk_risr, (long)ercd);
    return SYSER = ercd;
  #endif
}
#endif
#ifdef V4_DEF_SVC
/************************************************/
/* Define Extended Service Call                 */
/************************************************/

ER v4_def_svc(FN fncd, const V4_T_DSVC *pk_dsvc)
{
    T_SVC SNEAR *svc;
  #if ERC
    ER ercd;

    if (INEST)
        ercd = E_CTX;
    else if ((fncd == 0) || ((UINT)fncd > (UINT)CFG.svcfn_max))
        ercd = E_PAR;
    else
    {
  #endif
    dis_ims();
  #if ERC
    visnd_deb2(THF_SYS, TFN_DEF_SVC, (long)fncd, (long)pk_dsvc);
  #endif
    if (pk_dsvc != NULL)
    {
       #if ERC
        if ((UINT)pk_dsvc->parn > (UINT)_KERNEL_SVCMAXPAR)
        {
            return_ret_ims2(THF_SYS, E_PAR);
        }
       #endif
        svc = sys_alloc((SIZE)sizeof (T_SVC));
        if (svc == NULL)
        {   return_ret_ims2(THF_SYS, SYSER = E_SYS);
        }
        svc->svcrtn = pk_dsvc->svcrtn;
        svc->parn = (B)pk_dsvc->parn;
        pSVC[fncd]=svc;
        return_ret_ims(THF_SYS);
    }
    else
    {
        svc = pSVC[fncd];
        if (svc == NULL)
        {   return_ret_ims2(THF_SYS, E_NOEXS);
        }
        sys_free(svc, (SIZE)sizeof (T_SVC));
        pSVC[fncd] = NULL;
        return_ret_ims(THF_SYS);
    }
  #if ERC
    }
    vsnd_deb2e(THF_SYS, TFN_DEF_SVC, (long)fncd, (long)pk_dsvc, (long)ercd);
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V4_CAL_SVC
/************************************************/
/* Call SVC                                     */
/************************************************/
typedef ER_UINT (FFAR *SVCRTN)();

ER v4_cal_svc(FN fncd, ...)
{
    va_list ap;
    union{
        SVCRTN rtn;
        ER (FFAR *p0)(void) ;
        ER (FFAR *p1)(VP_INT) ;
        ER (FFAR *p2)(VP_INT,VP_INT) ;
        ER (FFAR *p3)(VP_INT,VP_INT,VP_INT) ;
        ER (FFAR *p4)(VP_INT,VP_INT,VP_INT,VP_INT) ;
        ER (FFAR *p5)(VP_INT,VP_INT,VP_INT,VP_INT,VP_INT) ;
        ER (FFAR *p6)(VP_INT,VP_INT,VP_INT,VP_INT,VP_INT,VP_INT) ;
    } svcrtn;
    T_SVC SNEAR *svc;
    INT parn;
    ER ercd;
    int i;
    VP_INT par[_KERNEL_SVCMAXPAR];

  #if ERC
    if ((fncd == 0) || ((UINT)fncd > (UINT)CFG.svcfn_max))
        ercd = E_PAR;
    else
    {
  #endif
    va_start(ap,fncd);
    dis_ims();
    svc = pSVC[fncd];
  #if 0
    visnd_deb2(THF_SYS, (long)0, (long)fncd, (long)svc);
  #endif
    ret_ims2();
    if (svc == NULL)
        return E_RSFN;
    else
    {
        svcrtn.rtn = (SVCRTN)svc->svcrtn;
        parn = svc->parn;
        for(i=0; i < parn; i++)
            par[i] = va_arg(ap,VP_INT);
        va_end(ap);
        switch (parn)
        {   case 0:
                ercd = (*svcrtn.p0)();
                break;
            case 1:
                ercd = (*svcrtn.p1)(par[0]);
                break;
            case 2:
                ercd = (*svcrtn.p2)(par[0], par[1]);
                break;
            case 3:
                ercd = (*svcrtn.p3)(par[0], par[1], par[2]);
                break;
            case 4:
                ercd = (*svcrtn.p4)(par[0], par[1], par[2], par[3]);
                break;
            case 5:
                ercd = (*svcrtn.p5)(par[0], par[1], par[2], par[3], par[4]);
                break;
            default:
                ercd = (*svcrtn.p6)(par[0], par[1], par[2], par[3], par[4], par[5]);
        }
        return ercd;
    }
  #if ERC
    }
   #if 0
    vsnd_deb1e(THF_SYS, (long)0, (long)fncde, (long)ercd);
   #endif
    return SYSER = ercd;
  #endif
}

#endif
#ifdef V3_RET_TMR
/************************************************/
/* Return Timer Handler                         */
/************************************************/

void v3_ret_tmr(void)
{
  #if ERC
    vsnd_deb0(THF_TIM, TFN_RET_TMR);
  #endif
}

#endif
#ifdef V4_REF_VER
/************************************************/
/* Get Version Information                      */
/************************************************/

ER v4_ref_ver(V4_T_RVER PFAR *pk_ver)
{
    int i;

  #if ERC
    vsnd_deb1e(THF_SYS, TFN_REF_VER, (long)pk_ver, E_OK);
  #endif
    pk_ver->maker = norti_ver->maker;
    pk_ver->prid = norti_ver->prid;
    pk_ver->spver = norti_ver->spver;
    pk_ver->prver = norti_ver->prver;
    for (i = 0; i < 4; i++)
        pk_ver->prno[i]=norti_ver->prno[i];
    return E_OK;
}

#endif
#ifdef V3_GET_VER
/************************************************/
/* Get Version Information                      */
/************************************************/

ER v3_get_ver(V3_T_VER PFAR *pk_ver)
{
    int i;

  #if ERC
    vsnd_deb1e(THF_SYS, TFN_GET_VER, (long)pk_ver, E_OK);
  #endif
    pk_ver->maker = norti_ver->maker;
    pk_ver->id = ((const V3_T_VER *)norti_ver)->id;
    pk_ver->spver = norti_ver->spver;
    pk_ver->prver = norti_ver->prver;
    for (i = 0; i < 4; i++)
        pk_ver->prno[i] = norti_ver->prno[i];
    pk_ver->cpu = ((const V3_T_VER *)norti_ver)->cpu;
    pk_ver->var = ((const V3_T_VER *)norti_ver)->var;
    return E_OK;
}

#endif

#ifdef V4_SNS_CTX
/************************************************/
/* Refer Context                                */
/************************************************/

BOOL v4_sns_ctx(void)
{
    BOOL sns;

    dis_ims();
  #if ERC
    visnd_deb0(THF_SYS, TFN_SNS_CTX);
  #endif
    sns = sns_ctx_sub();
    return_ret_ims2(THF_SYS, sns);
}

#endif
#ifdef V4_SNS_LOC
/************************************************/
/* Refer CPU lock                               */
/************************************************/

BOOL v4_sns_loc(void)
{
    UINT imask;

    dis_ims();
  #if ERC
    visnd_deb0(THF_SYS, TFN_SNS_LOC);
  #endif
    imask = IMASK;
    return_ret_ims2(THF_SYS, ref_sys_sub(imask));
}

#endif
#ifdef V4_SNS_DSP
/************************************************/
/* Refer CPU lock                               */
/************************************************/

BOOL v4_sns_dsp(void)
{
    B sns;

    dis_ims();
  #if ERC
    visnd_deb0(THF_SYS, TFN_SNS_DSP);
  #endif
    sns = DDISP;
    return_ret_ims2(THF_SYS, (sns != TSS_TSK));
}

#endif
#ifdef V4_SNS_DPN
/************************************************/
/* Refer CPU lock                               */
/************************************************/

BOOL v4_sns_dpn(void)
{
    B sns;

    dis_ims();
  #if ERC
    visnd_deb0(THF_SYS, TFN_SNS_DPN);
  #endif
    sns = DDISP | ref_sys_sub(IMASK);
    return_ret_ims2(THF_SYS, sns);
}

#endif
#ifdef V4_REF_SYS
/************************************************/
/* Refer System Status                          */
/************************************************/

ER v4_ref_sys(V4_T_RSYS *pk_rsys)
{
    UINT imask;

    if ((INEST < 0) || (RDQ[0] > CFG.tskid_max))
        pk_rsys->sysstat = TSS_INDP;
    else
    {   if (DDISP != TSS_TSK)
        {   dis_ims();
          #if ERC
            visnd_deb1(THF_SYS, TFN_REF_SYS, (long)pk_rsys);
          #endif
            imask = IMASK;
            ret_ims2_(THF_SYS);

            if (ref_sys_sub(imask))
                pk_rsys->sysstat = TSS_LOC;
            else
                pk_rsys->sysstat = TSS_DDSP;
            return E_OK;
        }
        else
            pk_rsys->sysstat = TSS_TSK;
    }
  #if ERC
    vsnd_deb1e(THF_SYS, TFN_REF_SYS, (long)pk_rsys, E_OK);
  #endif
    return E_OK;
}

#endif
#ifdef V4_REF_CFG
/************************************************/
/* Refer Configuration Information              */
/************************************************/

ER v4_ref_cfg(V4_T_RCFG *pk_rcfg)
{
  #if ERC
    vsnd_deb1e(THF_SYS, TFN_REF_CFG, (long)pk_rcfg, E_OK);
  #endif
    pk_rcfg->tskid_max = CFG.tskid_max;
    pk_rcfg->semid_max = CFG.semid_max;
    pk_rcfg->flgid_max = CFG.flgid_max;
    pk_rcfg->mbxid_max = CFG.mbxid_max;
    pk_rcfg->mbfid_max = CFG.mbfid_max;
    pk_rcfg->porid_max = CFG.porid_max;
    pk_rcfg->mplid_max = CFG.mplid_max;
    pk_rcfg->mpfid_max = CFG.mpfid_max;
    pk_rcfg->cycno_max = CFG.cycno_max;
    pk_rcfg->almno_max = CFG.almno_max;
    pk_rcfg->tpri_max  = CFG.tpri_max;
    pk_rcfg->tmrqsz    = CFG.tmrqsz;
    pk_rcfg->cycqsz    = CFG.cycqsz;
    pk_rcfg->almqsz    = CFG.almqsz;
    pk_rcfg->istksz    = CFG.istksz;
    pk_rcfg->tstksz    = CFG.tstksz;
    pk_rcfg->sysmsz    = CFG.sysmsz;
    pk_rcfg->mplmsz    = CFG.mplmsz;
    pk_rcfg->stkmsz    = CFG.stkmsz;
    pk_rcfg->dtqid_max = CFG.dtqid_max;
    pk_rcfg->mtxid_max = CFG.mtxid_max;
    pk_rcfg->isrid_max = CFG.isrid_max;
    pk_rcfg->svcfn_max = CFG.svcfn_max;
    return E_OK;
}

#endif
#ifdef V3_REF_CFG
/************************************************/
/* Refer Configuration Information              */
/************************************************/

ER v3_ref_cfg(V3_T_RCFG *pk_rcfg)
{
  #if ERC
    vsnd_deb1e(THF_SYS, TFN_REF_CFG, (long)pk_rcfg, E_OK);
  #endif
    pk_rcfg->tskid_max = CFG.tskid_max;
    pk_rcfg->semid_max = CFG.semid_max;
    pk_rcfg->flgid_max = CFG.flgid_max;
    pk_rcfg->mbxid_max = CFG.mbxid_max;
    pk_rcfg->mbfid_max = CFG.mbfid_max;
    pk_rcfg->porid_max = CFG.porid_max;
    pk_rcfg->mplid_max = CFG.mplid_max;
    pk_rcfg->mpfid_max = CFG.mpfid_max;
    pk_rcfg->cycno_max = CFG.cycno_max;
    pk_rcfg->almno_max = CFG.almno_max;
    pk_rcfg->tpri_max  = CFG.tpri_max;
    pk_rcfg->tmrqsz    = CFG.tmrqsz;
    pk_rcfg->cycqsz    = CFG.cycqsz;
    pk_rcfg->almqsz    = CFG.almqsz;
    pk_rcfg->istksz    = CFG.istksz;
    pk_rcfg->tstksz    = CFG.tstksz;
    pk_rcfg->sysmsz    = CFG.sysmsz;
    pk_rcfg->mplmsz    = CFG.mplmsz;
    pk_rcfg->stkmsz    = CFG.stkmsz;
    return E_OK;
}

#endif
#ifdef OS_CORE
/************************************************/
/* System Clock                                 */
/************************************************/

void v3_sig_tim(void)
{

    SYSTCB.pri = 0;
    SDISP = (TSK_ID)CFG.tskid_max+1;
    TMREQ++;
}
#endif
#ifdef SIG_OVR
/************************************************/
/* Overrun handler                              */
/************************************************/

void ovr_sta(void)
{
    void (FFAR *ovrhdr)(ID,VP_INT);

    ovrhdr = (void (FFAR *)(ID,VP_INT))_kernel_ovr.ovrhdr;
    ovrhdr(_kernel_ovr.otskid, pTCB[_kernel_ovr.otskid]->ctsk->exinf);
    _kernel_ovr.otskid = TSK_NONE;
    _kernel_dcall = NULL;
}

void sig_ovr(void)
{
    ID tskid;
    int cnt;

    if (_kernel_ovr.ovrclr != NULL)
        (*_kernel_ovr.ovrclr)();
    if ((tskid = RDQ[0]) != (TSK_ID)(CFG.tskid_max+1))
    {   if ((cnt = pTCB[tskid]->rcnt) != 0)
        {
           #if ERC
             visnd_deb2(THF_TIM, TFN_SIG_OVR, (long)tskid, (long)cnt);
           #endif
           if (--cnt==0)
            {
               if (_kernel_dcall == NULL)
               {   _kernel_ovr.otskid = tskid;
                   _kernel_dcall = (FP)ovr_sta;
               }
               else
                   return;
            }
            pTCB[tskid]->rcnt = cnt;
        }
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Add Time                                     */
/************************************************/

void FNEAR tim_add(const SYSTIME *t1, SYSTIME SNEAR *t2, T_CYC SNEAR *add)
{
    add->ltime = t1->ltime + t2->ltime;
  #if (CLKBITS>32)
    add->utime = t1->utime + t2->utime;
    if (add->ltime < t1->ltime)
        add->utime++;
  #endif
}

#endif
#ifdef OS_CORE
/************************************************/
/* Calc Time Difference                         */
/************************************************/

void FNEAR tim_dif(SYSTIME SNEAR *from, SYSTIME SNEAR *to, SYSTIME DFAR *diff)
{
  #if (CLKBITS>32)
    if (to->utime > from->utime)
    {   diff->utime = to->utime - from->utime;
        if (to->ltime < from->ltime)
            diff->utime--;
        diff->ltime = to->ltime - from->ltime;
    }
    else if (to->utime == from->utime)
    {   diff->utime = 0;
        if (to->ltime >= from->ltime)
            diff->ltime = to->ltime - from->ltime;
        else
            diff->ltime = 0;
    }
    else
    {
        diff->utime = 0;
        diff->ltime = 0;
    }
  #else
    if (to->ltime >= from->ltime)
        diff->ltime = to->ltime - from->ltime;
    else
        diff->ltime = 0;
  #endif
}

#endif
#ifdef OS_CORE
/************************************************/
/* Delete TCB from Timer Queue                  */
/************************************************/

void FNEAR del_tmq(T_TCB SNEAR *tcb, ID tskid)
{
    T_TCB SNEAR *tcb1;
    T_TCB SNEAR *tcb2;
    ID tid1, tid2;
    TSK_ID SNEAR *tmq;

    tmq = TMQ + ((unsigned)tcb->tmo & TMQMS);
    tid1 = tcb->ptm;
    if (tid1 != tskid)
    {   tid2 = tcb->ntm;
        if (*tmq == (TSK_ID)tskid)
            *tmq = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->ptm = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tcb1->ntm = (TSK_ID)tid2;
    }
    else
        *tmq = 0;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Add TCB to Timer Queue                       */
/************************************************/

void FNEAR add_tmq(TMO tmout, T_TCB SNEAR *tcb, ID tskid)
{
    T_TCB SNEAR *tcb1;
    T_TCB SNEAR *tcb2;
    ID tid1, tid2;
    TSK_ID SNEAR *tmq;

    tmout += SYSCK.ltime;
    tcb->tmo = tmout;
    tmq = TMQ + ((unsigned)tmout & TMQMS);

    tid1 = *tmq;
    if (tid1 != 0)
    {   tcb->ntm = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tid2 = tcb1->ptm;
        tcb1->ptm = (TSK_ID)tskid;
        tcb->ptm = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->ntm = (TSK_ID)tskid;
    }
    else
    {   *tmq = (TSK_ID)tskid;
        tcb->ntm = (TSK_ID)tskid;
        tcb->ptm = (TSK_ID)tskid;
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Delete Handler from Timer Handler Queue      */
/************************************************/

void FNEAR del_thq(T_CYC SNEAR * SNEAR *p, T_CYC SNEAR *hcb, ID tmhid)
{
    T_CYC SNEAR *hcb1;
    T_CYC SNEAR *hcb2;
    ID hid1, hid2;
    UB SNEAR *thq;

    if ((T_CYC SNEAR * SNEAR *)p == pCYC)
        thq = CHQ + ((unsigned)hcb->ltime & CHQMS);
    else
        thq = AHQ + ((unsigned)hcb->ltime & AHQMS);

    hid1 = hcb->ptm;
    if (hid1 != tmhid)
    {   hid2 = hcb->ntm;
        if (*thq == (UB)tmhid)
            *thq = (UB)hid2;
        hcb2 = p[hid2];
        hcb2->ptm = (UB)hid1;
        hcb1 = p[hid1];
        hcb1->ntm = (UB)hid2;
    }
    else
        *thq = 0;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Add Handler to Timer Handler Queue           */
/************************************************/

void FNEAR add_thq(T_CYC SNEAR * SNEAR *p, T_CYC SNEAR *hcb, ID tmhid)
{
    T_CYC SNEAR *hcb1;
    T_CYC SNEAR *hcb2;
    ID hid1, hid2;
    UB SNEAR *thq;

    if ((T_CYC SNEAR * SNEAR *)p == pCYC)
        thq = CHQ + (unsigned)(hcb->ltime & CHQMS);
    else
        thq = AHQ + (unsigned)(hcb->ltime & AHQMS);

    hid1 = *thq;
    if (hid1 != 0)
    {   hcb->ntm = (UB)hid1;
        hcb1 = p[hid1];
        hid2 = hcb1->ptm;
        hcb1->ptm = (UB)tmhid;
        hcb->ptm = (UB)hid2;
        hcb2 = p[hid2];
        hcb2->ntm = (UB)tmhid;
    }
    else
    {   *thq = (UB)tmhid;
        hcb->ntm = (UB)tmhid;
        hcb->ptm = (UB)tmhid;
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Delete TCB from Queue                        */
/************************************************/

void FNEAR del_que(T_TCB SNEAR *tcb, ID tskid)
{
    T_TCB SNEAR *tcb1;
    T_TCB SNEAR *tcb2;
    ID tid1, tid2;
    TSK_ID SNEAR *que;

    que = tcb->que;
    tid1 = tcb->pid;
    if (tid1 != tskid)
    {   tid2 = tcb->nid;
        if (*que == (TSK_ID)tskid)
            *que = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->pid = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tcb1->nid = (TSK_ID)tid2;
    }
    else
        *que = 0;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Add TCB to Queue Tail                        */
/************************************************/

void FNEAR add_que(TSK_ID SNEAR *que, T_TCB SNEAR *tcb, ID tskid)
{
    T_TCB SNEAR *tcb1;
    T_TCB SNEAR *tcb2;
    ID tid1, tid2;

    tcb->que = que;
    tid1 = *que;
    if (tid1 != 0)
    {   tcb->nid = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tid2 = tcb1->pid;
        tcb1->pid = (TSK_ID)tskid;
        tcb->pid = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->nid = (TSK_ID)tskid;
    }
    else
    {   *que = (TSK_ID)tskid;
        tcb->nid = (TSK_ID)tskid;
        tcb->pid = (TSK_ID)tskid;
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Change TCB to Another Queue                  */
/************************************************/

void FNEAR chg_que(TSK_ID SNEAR *que, T_TCB SNEAR *tcb, ID tskid)
{
    T_TCB SNEAR *tcb1;
    T_TCB SNEAR *tcb2;
    ID tid1, tid2;
    TSK_ID SNEAR *que2;

    que2 = tcb->que;
    tid1 = tcb->pid;
    if (tid1 != tskid)
    {   tid2 = tcb->nid;
        if (*que2 == tskid)
            *que2 = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->pid = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tcb1->nid = (TSK_ID)tid2;
    }
    else
        *que2 = 0;

    tcb->que = que;
    tid1 = *que;
    if (tid1 != 0)
    {   tcb->nid = (TSK_ID)tid1;
        tcb1 = pTCB[tid1];
        tid2 = tcb1->pid;
        tcb1->pid = (TSK_ID)tskid;
        tcb->pid = (TSK_ID)tid2;
        tcb2 = pTCB[tid2];
        tcb2->nid = (TSK_ID)tskid;
    }
    else
    {   *que = (TSK_ID)tskid;
        tcb->nid = (TSK_ID)tskid;
        tcb->pid = (TSK_ID)tskid;
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* Change TCB to Ready Queue                    */
/************************************************/

void FNEAR chg_rdq(T_TCB SNEAR *tcb, ID tskid)
{
    TSK_PRI pri;

    pri = tcb->pri;
    chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
}

#endif
#ifdef OS_CORE
/************************************************/
/* del_que and ret_ims                          */
/************************************************/

ER FNEAR del_que_ret_ims_(long flag, T_TCB SNEAR *tcb, ID tskid)
{
    del_que(tcb, tskid);
    return_ret_ims(flag);
}

#endif
#ifdef OS_CORE
/************************************************/
/* add_que to RDQ and dispatch                  */
/************************************************/

ER FNEAR add_rdq_dispatch_(long flag, T_TCB SNEAR *tcb, ID tskid)
{
    TSK_PRI pri;

    pri = tcb->pri;
    add_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
    if (pri < NOWPRI)
    {   return_dispatch(flag);
    }
    else
    {   return_ret_ims(flag);
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* chg_que to RDQ and dispatch                  */
/************************************************/

ER FNEAR chg_rdq_dispatch_(long flag, T_TCB SNEAR *tcb, ID tskid)
{
    TSK_PRI pri;

    pri = tcb->pri;
    chg_que((TSK_ID SNEAR *)RDQ + pri, tcb, tskid);
    if (pri < NOWPRI)
    {   return_dispatch(flag);
    }
    else
    {   return_ret_ims(flag);
    }
}

#endif
#ifdef OS_CORE
/************************************************/
/* del_que and dispatch                         */
/************************************************/

ER FNEAR del_que_dispatch1_(long flag, T_TCB SNEAR *tcb, ID tskid)
{
    del_que(tcb, tskid);
    return_dispatch1(flag);
}

#endif
#ifdef OS_CORE
/************************************************/
/* del_que and dispatch                         */
/************************************************/

ER FNEAR chg_que_dispatch1_(long flag, TSK_ID SNEAR *rdq, T_TCB SNEAR *tcb, ID tskid)
{
    chg_que(rdq, tcb, tskid);
    return_dispatch1(flag);
}

#endif
#ifdef OS_CORE
/************************************************/
/* Intialize OS MemoryPool                      */
/************************************************/

ER FNEAR osmem_init(T_MEM PFAR *memtop, SIZE size)
{
    size &= (SIZE)(0 - SIZEOF_ALIGN);
    memtop->next = 0;
    memtop->size = size;
    return E_OK;
}

#endif
#ifdef OS_CORE
/************************************************/
/* OS MemoryCopy                                */
/************************************************/

void FNEAR osmem_cpy(DFAR B *dst, DFAR B *src, unsigned size)
{
    while (size-- != 0)
        *dst++ = *src++;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Allocate Memory Block                        */
/************************************************/

void PFAR * FNEAR blk_alloc(T_MEM PFAR * SNEAR *memtop, SIZE size)
{
    T_MEM PFAR *p;
    T_MEM PFAR *p1;
    T_MEM PFAR *p2;
    T_MEM PFAR *pnext;
    SIZE psize;
  #if MINIMUM_BLK
    ADDR p1flag;
  #endif

    if ((size = (size + SIZEOF_ALIGN - 1) & (SIZE)(0 - SIZEOF_ALIGN)) == 0)
        return NULL;
    p1 = (T_MEM PFAR *)memtop;
    p = p1->next;
  #if MINIMUM_BLK
    p1flag = 0;
  #endif
    for (;;)
    {   if (p == 0)
            return NULL;
      #if MINIMUM_BLK
        if ((ADDR)(pnext = p->next) & 1)
            psize = SIZEOF_ALIGN;
        else
            psize = p->size;
      #else
        pnext = p->next;
        psize = p->size;
      #endif
        if (psize >= size)
        {   psize -= size;
            break;
        }
        p1 = p;
      #if MINIMUM_BLK
        p1flag = (ADDR)pnext & 0x1;
        p = (T_MEM PFAR *)((ADDR)pnext & ~p1flag);
      #else
        p = pnext;
      #endif
    }
  #if MINIMUM_BLK
    pnext = (T_MEM PFAR *)((ADDR)pnext & (ADDR)(0-2));
  #else
    pnext = pnext;
  #endif
    if (psize == 0)
      #if MINIMUM_BLK
        p1->next = (T_MEM PFAR *)((ADDR)pnext | p1flag);
      #else
        p1->next = pnext;
      #endif
    else
    {   p2 = (T_MEM PFAR *)((ADDR)p + size);
      #if MINIMUM_BLK
        p1->next = (T_MEM PFAR *)((ADDR)p2 | p1flag);
        if (psize == SIZEOF_ALIGN)
            p2->next = (T_MEM PFAR *)((ADDR)pnext | 0x1);
        else
      #else
        p1->next = p2;
      #endif
        {   p2->next = pnext;
            p2->size = psize;
        }
    }
    return (void PFAR *)p;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Free Memory Block                            */
/************************************************/

ER FNEAR blk_free(T_MEM PFAR * SNEAR *memtop, void PFAR *blk, SIZE size)
{
    T_MEM PFAR *p;
    T_MEM PFAR *p1;
    T_MEM PFAR *p2;
    T_MEM PFAR *p1next;
    T_MEM PFAR *p2next;
    SIZE p1size;

    size = (size + SIZEOF_ALIGN - 1) & (SIZE)(0 - SIZEOF_ALIGN);
    p = (T_MEM PFAR *)blk;
    p1 = (T_MEM PFAR *)memtop;
    p1next = p1->next;
    p1size = 0;
    for(;;)
    {
      #if MINIMUM_BLK
        p2 = (T_MEM PFAR *)((ADDR)p1next & (ADDR)(0-2));
      #else
        p2 = p1next;
      #endif
        if (((ADDR)p2 == 0) || ((ADDR)p2 > (ADDR)p))
        {
            if (((ADDR)p2 != 0)
              &&((ADDR)p2 < ((ADDR)p + (ADDR)size)))
            {   return E_PAR;
            }
            if ((ADDR)p == ((ADDR)p1 + p1size))
            {   p1size += size;
            }
            else
            {
              #if MINIMUM_BLK
                p1->next = ((p1size == SIZEOF_ALIGN)?(T_MEM PFAR *)((ADDR)p | 0x1):p);
              #else
                p1->next = p;
              #endif
                p1 = p;
                p1size = size;
            }
            if (p2 == 0)
            {
              #if MINIMUM_BLK
                if (p1size == SIZEOF_ALIGN)
                {   p1->next = (T_MEM PFAR *)1;
                }
                else
              #endif
                {   p1->next = 0;
                    p1->size = p1size;
                }
            }
            else if ((ADDR)p2 == ((ADDR)p1 + p1size))
            {   p2next = p2->next;
              #if MINIMUM_BLK
                p1->next = (T_MEM PFAR *)((ADDR)p2next & (ADDR)(0-2));
                p1->size = p1size + (((ADDR)p2next & 0x1)?SIZEOF_ALIGN:p2->size);
              #else
                p1->next = p2next;
                p1->size = p1size + p2->size;
              #endif
            }
            else
            {
              #if MINIMUM_BLK
                if (p1size == SIZEOF_ALIGN)
                {   p1->next = (T_MEM PFAR *)((ADDR)p2 | 1);
                }
                else
              #endif
                {   p1->next = p2;
                    p1->size = p1size;
                }
            }
            break;
        }
        p1 = p2;
        p1next = p1->next;
      #if MINIMUM_BLK
        p1size = ((ADDR)p1next & 0x1)?SIZEOF_ALIGN:p2->size;
      #else
        p1size = p2->size;
      #endif
        if ((ADDR)p < ((ADDR)p1 + (ADDR)p1size))
        {   return E_PAR;
        }
    }
    return E_OK;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Get Memory Pool Size                         */
/************************************************/

SIZE FNEAR mpl_size(T_MEM PFAR *memtop, SIZE PFAR *p_frsz)
{
    SIZE maxsz, frsz, size;
    T_MEM PFAR *p;

    maxsz = 0;
    frsz = 0;
    for (p = memtop; p != 0;)
    {   size = ((ADDR)p->next & 1)?SIZEOF_ALIGN:p->size;
        frsz += size;
        if (size > maxsz)
            maxsz = size;
        p = (T_MEM PFAR *)((ADDR)p->next & (ADDR)(0-2));
    }
    *p_frsz = frsz;
    return maxsz;
}

#endif
#if (defined(OS_CORE)&&(defined(CPU_86)||defined(CPU_M16C)))
/************************************************/
/* Intialize OS MemoryPool                      */
/************************************************/

ER FNEAR osnmem_init(T_NMEM SNEAR *memtop, SIZE size)
{
    size &= (SIZE)(0 - SIZEOF_NALIGN);
    memtop->next = 0;
    memtop->size = (UINT)size;
    return E_OK;
}

#endif
#if (defined(OS_CORE)&&(defined(CPU_86)||defined(CPU_M16C)))
/************************************************/
/* Allocate Memory Block                        */
/************************************************/

void SNEAR * FNEAR blk_nalloc(T_NMEM SNEAR * SNEAR *memtop, SIZE size)
{
    T_NMEM SNEAR *p;
    T_NMEM SNEAR *p1;
    T_NMEM SNEAR *p2;
    T_NMEM SNEAR *pnext;
    UINT psize, p1flag, rsize;

    if ((rsize = ((UINT)size + SIZEOF_NALIGN - 1) & (UINT)(0 - SIZEOF_NALIGN)) == 0)
        return NULL;
    p1 = (T_NMEM SNEAR *)memtop;
    p = p1->next;
    p1flag = 0;
    for (;;)
    {   if (p == 0)
            return NULL;
        if ((UINT)(pnext = p->next) & 1)
            psize = SIZEOF_NALIGN;
        else
            psize = p->size;
        if (psize >= rsize)
        {   psize -= rsize;
            break;
        }
        p1 = p;
        p1flag = (UINT)pnext & 0x1;
        p = (T_NMEM SNEAR *)((UINT)pnext & ~p1flag);
    }
    pnext = (T_NMEM SNEAR *)((UINT)pnext & (UINT)(0-2));
    if (psize == 0)
        p1->next = (T_NMEM SNEAR *)((UINT)pnext | p1flag);
    else
    {   p2 = (T_NMEM SNEAR *)((UINT)p + rsize);
        p1->next = (T_NMEM SNEAR *)((UINT)p2 | p1flag);
        if (psize == SIZEOF_NALIGN)
            p2->next = (T_NMEM SNEAR *)((UINT)pnext | 0x1);
        else
        {   p2->next = pnext;
            p2->size = psize;
        }
    }
    return (void SNEAR *)p;
}

#endif
#if (defined(OS_CORE)&&(defined(CPU_86)||defined(CPU_M16C)))
/************************************************/
/* Free Memory Block                            */
/************************************************/

ER FNEAR blk_nfree(T_NMEM SNEAR * SNEAR *memtop, void SNEAR *blk, SIZE size)
{
    T_NMEM SNEAR *p;
    T_NMEM SNEAR *p1;
    T_NMEM SNEAR *p2;
    T_NMEM SNEAR *p1next;
    T_NMEM SNEAR *p2next;
    UINT p1size, rsize;

    rsize = ((UINT)size + SIZEOF_NALIGN - 1) & (UINT)(0 - SIZEOF_NALIGN);
    p = (T_NMEM SNEAR *)blk;
    p1 = (T_NMEM SNEAR *)memtop;
    p1next = p1->next;
    p1size = 0;
    for(;;)
    {   p2 = (T_NMEM SNEAR *)((UINT)p1next & (UINT)(0-2));
        if (((UINT)p2 == 0) || ((UINT)p2 > (UINT)p))
        {
            if (((UINT)p2 != 0)
              &&((UINT)p2 < ((UINT)p + (UINT)rsize)))
            {   return E_PAR;
            }
            if ((B SNEAR *)p == ((B SNEAR *)p1 + p1size))
            {   p1size += rsize;
            }
            else
            {   p1->next = ((p1size == SIZEOF_NALIGN)?(T_NMEM SNEAR *)((UINT)p|0x1):p);
                p1 = p;
                p1size = rsize;
            }
            if (p2 == 0)
            {   if (p1size == SIZEOF_NALIGN)
                {   p1->next = (T_NMEM SNEAR *)1;
                }
                else
                {   p1->next = 0;
                    p1->size = p1size;
                }
            }
            else if ((B SNEAR *)p2 == ((B SNEAR *)p1 + p1size))
            {   p2next = p2->next;
                p1->next = (T_NMEM SNEAR *)((UINT)p2next & (UINT)(0-2));
                p1->size = p1size + (((UINT)p2next & 0x1)?SIZEOF_NALIGN:p2->size);
            }
            else
            {   if (p1size == SIZEOF_ALIGN)
                {   p1->next = (T_NMEM SNEAR *)((UINT)p2 | 1);
                }
                else
                {   p1->next = p2;
                    p1->size = p1size;
                }
            }
        }
        p1 = p2;
        p1next = p1->next;
        p1size = ((UINT)p1next & 0x1)?SIZEOF_NALIGN:p2->size;
        if ((UINT)p < ((UINT)p1 + (UINT)p1size))
        {   return E_PAR;
        }
    }
    return E_OK;
}

#endif
#ifdef OS_CORE
/************************************************/
/* Allocate Stack                               */
/************************************************/

void SFAR * FNEAR stk_alloc(SIZE size)
{
    return (void SFAR *)blk_salloc((T_NMEM SFAR * SNEAR *)&STKTOP, size);
}

#endif
#ifdef OS_CORE
/************************************************/
/* Free Stack                                   */
/************************************************/

ER FNEAR stk_free(void SFAR *blk, SIZE size)
{
    return blk_sfree((T_NMEM SFAR * SNEAR *)&STKTOP, blk, size);
}

#endif
#ifdef OS_CORE
/************************************************/
/* Allocate Memory Pool                         */
/************************************************/

void PFAR * FNEAR mpl_alloc(SIZE size)
{
    if (pMPLMEM != NULL)
        return (void PFAR *)blk_palloc((T_MEM PFAR * SNEAR *)&MPLTOP, size);
    else
        return (void PFAR *)blk_salloc((T_NMEM SFAR * SNEAR *)&STKTOP, size);
}

#endif
#ifdef OS_CORE
/************************************************/
/* Free Memory Pool                             */
/************************************************/

ER FNEAR mpl_free(void PFAR *blk, SIZE size)
{
    if (pMPLMEM != NULL)
        return blk_pfree((T_MEM PFAR * SNEAR *)&MPLTOP, blk, size);
    else
        return blk_sfree((T_NMEM SFAR * SNEAR *)&STKTOP, blk, size); /* Warning change far pointer to near pointer */
}

#endif
#ifdef OS_CORE
/************************************************/
/* Allocate Control Block                       */
/************************************************/

void SNEAR * FNEAR sys_alloc(SIZE size)
{
    if (pSYSMEM != NULL)
        return (void SNEAR *)blk_yalloc(&SYSTOP, size);
    else
      #if (defined(CPU_86)&&(SIZEOF_PTR==4))
        return (void SNEAR *)(int)(W)blk_salloc((T_NMEM SFAR * SNEAR *)&STKTOP, size);
      #else
        return (void SNEAR *)blk_salloc((T_NMEM SFAR * SNEAR *)&STKTOP, size);
      #endif
}
#endif
#ifdef OS_CORE
/************************************************/
/* Free Control Block                           */
/************************************************/

ER FNEAR sys_free(void SNEAR *blk, SIZE size)
{

    if (pSYSMEM != NULL)
       return blk_yfree((T_NMEM SNEAR * SNEAR *)&SYSTOP, blk, size);
    else
       return blk_sfree((T_NMEM SFAR * SNEAR *)&STKTOP, blk, size);
}

#endif
#ifdef V4_MUTEX
BOOL wup_tsk_mtx(T_MTX SNEAR *mtx, ID mtxid)
{
    TSK_ID SNEAR *que;
    T_TCB SNEAR *tcb;
    ATR atr;
    PRI ceil;
    ID tskid, nid;
    BOOL disp = FALSE;

    mtx->occ = 0;
    atr = mtx->atr;
    ceil = mtx->ceil;
    que = mtx->que;
    for (;;que++)
    {   tskid = *que;
        if (tskid >= TSK_QUETAIL)
            break;
L1:     if (tskid != 0)
        {   tcb = pTCB[tskid];
            if (tcb->sts & S_TMO)
                del_tmq(tcb, tskid);
            del_que(tcb, tskid);
            tcb->sts = S_RDY;
            if ((atr == TA_CEILING) && (tcb->bpri < ceil))
            {   tcb->sp->ercd = E_ILUSE;
                if (tcb->sus == 0)
                    add_que((TSK_ID SNEAR *)RDQ + tcb->pri, tcb, tskid);
                tskid = *que;
                disp |= tcb->pri < NOWPRI;
                goto L1;
            }
            else
            {   if ((atr == TA_CEILING) && (tcb->pri >= ceil))
                    tcb->pri = ceil;
                else
                {   if (ceil)
                        mtx->ceil = tcb->pri;
                }
                mtx->occ = tskid;
                nid = tcb->lock;
                tcb->lock = mtxid;
                mtx->nid = nid;
                if (tcb->sus == 0)
                    add_que((TSK_ID SNEAR *)RDQ + tcb->pri, tcb, tskid);
                return disp || tcb->pri < NOWPRI;
            }
        }
    }
    return disp;
}

void unlock_allmtx(ID mtxid)
{
    T_MTX SNEAR *mtx;

    do {
        mtx = pMTX[mtxid];
        (void) wup_tsk_mtx(mtx, mtxid);
        mtxid = mtx->nid;
        mtx->nid = 0;
    } while (mtxid);
}
#endif
#ifdef OS_CORE
void reset_occ_cpri(ID tskid)
{
    T_TCB SNEAR *tcb;
    T_MTX SNEAR *mtx;
    TSK_ID SNEAR *que;
    ID mtxid;
    PRI tskpri, maxpri, pri;

    tcb = pTCB[tskid];
L1: mtxid = tcb->lock;
    tskpri = tcb->bpri;
    do {
        mtx = pMTX[mtxid];
        if (mtx->atr == TA_INHERIT)
        {   que = mtx->que;
            for(maxpri = 1; *que == 0; maxpri++, que++);
            mtx->ceil = maxpri;
            if (maxpri < tskpri)
                tskpri = maxpri;
        }
        else if ((mtx->atr == TA_CEILING) && (tskpri > mtx->ceil))
            tskpri = mtx->ceil;
        mtxid = mtx->nid;
    } while (mtxid);
    if (tcb->pri != tskpri)
    {   pri = tcb->pri;
        tcb->pri = (TSK_PRI)tskpri;
        if (tcb->sts == S_RDY && tcb->sus == 0)
           chg_que((TSK_ID SNEAR *)RDQ + tskpri, tcb, tskid);
        if (tcb->sts >= S_FLG)
        {   que = tcb->que;

            if (*(que+1) != TSK_QUETAIL)
            {   que -= pri - (TSK_PRI)tskpri;
                chg_que(que, tcb, tskid);
            }

            if ((tcb->sts & STS_MSK1) == S_MTX)
            {   mtx = pMTX[get_wid()];
                if (mtx->atr == TA_INHERIT)
                {   que = mtx->que;
                    for(maxpri = 1; *que == 0; maxpri++, que++);
                    if (maxpri != mtx->ceil)
                    {   tskid = mtx->occ;
                        mtx->ceil = maxpri;
                        tcb = pTCB[tskid];
                        if (maxpri >= tcb->bpri)
                            tskpri = tcb->bpri;
                        else
                            tskpri = maxpri;
                        goto L1;
                    }
                }
            }
            else if ((tcb->sts & STS_MSK1) == S_SMB)
                push_msg2mbf(get_wid());
            else if ((tcb->sts & STS_MSK0) == S_MPL)
                rel_mplchk(get_wid());
        }

    }
}

#endif

/* end */
