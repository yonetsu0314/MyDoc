/*----------------------------------------------------------------------*
 * File name	: tpuint.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/tpu/RCS/tpuint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tpuint.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		TCU による周期割り込みの設定									*/
/*----------------------------------------------------------------------*/

#define	TPUINT_C

#include	<drive\tpudrv.h>
#include	"dtpu.h"

/*----------------------------------------------------------------------*/
/* TPU 割り込みハンドラー												*/
/*																		*/
/* 引き数																*/
/* 戻り値																*/
/*----------------------------------------------------------------------*/
#pragma interrupt(tgi0a_int_hdr)		/* TPU0 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi0a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV0] != (void *)0)		interrupt_func[TPU_DEV0]();
	TPU0.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi1a_int_hdr)		/* TPU1 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi1a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV1] != (void *)0)		interrupt_func[TPU_DEV1]();
	TPU1.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi2a_int_hdr)		/* TPU2 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi2a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV2] != (void *)0)		interrupt_func[TPU_DEV2]();
	TPU2.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi3a_int_hdr)		/* TPU3 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi3a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV3] != (void *)0)		interrupt_func[TPU_DEV3]();
	TPU3.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi4a_int_hdr)		/* TPU4 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi4a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV4] != (void *)0)		interrupt_func[TPU_DEV4]();
	TPU4.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi5a_int_hdr)		/* TPU5 割り込みﾊﾝﾄﾞﾗ	*/
void	tgi5a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV5] != (void *)0)		interrupt_func[TPU_DEV5]();
	TPU5.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

