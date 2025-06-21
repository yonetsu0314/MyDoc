/*======================================================================
 * File name    : isritm.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/wcu/RCS/isritm.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: isritm.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include <kernel.h>
#include "sh7709a.h"

void isritm(void)
{
	sfr_outw(TMU_TCR0, (unsigned short)0x21);	/* clear underflow flag */
	sig_tim();					/* kernel timer process */
}
