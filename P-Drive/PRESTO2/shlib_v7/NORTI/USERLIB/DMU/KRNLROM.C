/*======================================================================
 * File name    : krnlrom.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/dmu/RCS/krnlrom.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: krnlrom.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2003/11/12 compiler ver7 vup
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ﾀｲﾏをcre_isr()関数を用いて生成
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * SH2用NORTiユーザ初期化関数
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include <machine.h>
#include <kernel.h>

#define INTVEC      0x6000000L
#define INTVSZ      1024

extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;

BOOL _kernel_inROM(const VP p)
{
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));
}

ER _kernel_check_cisr(const T_CISR *pk_cisr)
{
    if (pk_cisr->intno >= (INTVSZ) / 4)
        return E_PAR;
    return E_OK;
}

void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    *p = (UW)hdr;
}

FP _kernel_get_inthdr(const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    return (FP)*p;
}

