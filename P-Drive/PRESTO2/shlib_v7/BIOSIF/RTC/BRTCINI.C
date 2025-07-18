/*----------------------------------------------------------------------
 * File name	: rtcini.c	/ RTC initialize
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/biosif/rtc/RCS/brtcini.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: brtcini.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/11/12 TRAP中の割り込み禁止処理をやめる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * trapa中は、外部割込み禁止にした
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *----------------------------------------------------------------------
 * 説明
 *	RTCを初期化する
 *----------------------------------------------------------------------
 * 引き数
 *	無し
 *
 * 戻り値
 *	int	: 0
 *----------------------------------------------------------------------
 */

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<machine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */

int
_rtc_initialize(void)
{
//	int	imask_bak;
	int	err;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (int)trapa_svc(VECTOR_CLK, 0x0);
//	set_imask( imask_bak );
	return(err);
}
