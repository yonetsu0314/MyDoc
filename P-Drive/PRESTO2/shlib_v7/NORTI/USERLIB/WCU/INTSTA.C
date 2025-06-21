/*======================================================================
 * File name    : intsta.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/norti/userlib/wcu/RCS/intsta.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: intsta.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 割り込みレベルを７から１４に変更する。ブートローダーにあわせる。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/06 20:00
 * タイマー割り込みの間隔を1msecに変更する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/04 CLKを9.8MHzに変更
 * 
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

#ifndef IP
#define IP      14
#endif

#ifdef MSEC
#undef	MSEC
#define MSEC      1
#else
#define MSEC      1
#endif

#ifndef CLK
#define CLK      9830400*2
#endif

#define TMU_TCR     TMU_TCR0
#define TMU_TCNT    TMU_TCNT0
#define TMU_TCOR    TMU_TCOR0
#define INT_TUNI    INT_TUNI0

ER intsta(void)
{
	UW tc;

	sfr_outw(INTC_IPRA, (unsigned short)(sfr_inw(INTC_IPRA) | (IP << 12)));

	tc = ((CLK) * MSEC) / 16000;
	sfr_outl(TMU_TCOR, tc);			/* setup timer constant */
	sfr_outl(TMU_TCNT, tc);			/* setup timert counter */
	sfr_outw(TMU_TCR, (unsigned short)0x21);/* enable underflow interrupt and */
						/* select 1/16 prescaler */
	sfr_set(TMU_TSTR, 0x01);		/* start to count */

	return E_OK;
}
