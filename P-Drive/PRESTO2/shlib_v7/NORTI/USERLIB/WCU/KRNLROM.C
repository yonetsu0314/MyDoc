/*======================================================================
 * File name    : krnlrom.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/wcu/RCS/krnlrom.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: krnlrom.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include <machine.h>
#include <kernel.h>

extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;

BOOL _kernel_inROM(const VP p)
{
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));
}
