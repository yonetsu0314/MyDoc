/*----------------------------------------------------------------------*
 * File name	: tpusini.c		/ system initialize						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/tpu/RCS/tpusini.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tpusini.c $
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
/*		TPU による周期割り込みの設定									*/
/*----------------------------------------------------------------------*/

#include	<drive\tpudrv.h>

#define	STORAGE
#include	"dtpu.h"
#undef 	STRAGE

/*----------------------------------------------------------------------*/
/* タイマーの初期化														*/
/*																		*/
/* 引き数																*/
/*		TPU 番号 0-5													*/
/*		割り込み周期													*/
/*		割り込み処理関数												*/
/* 戻り値																*/
/*		 0	･･･ 正常終了												*/
/*		 -1	･･･ 初期化失敗												*/
/*----------------------------------------------------------------------*/
int tpu_tmr_sys_init(
	int tpu_no,							/* 0 to 5 */
	unsigned short	ival_t,				/* 1usec ～ 65535usec */
	void (*i_func)(void)
	)
{
	int		i;
	long	x = 1L;
	volatile struct st_tpu0 *adrs0_3 	 = tpu_adrs[tpu_no];
	volatile struct st_tpu1 *adrs1_2_4_5 = tpu_adrs[tpu_no];

	if(i_func == (void *)0)							return -1;
	if((tpu_no < 0) || (tpu_no >= TPU_DEV_MAX))		return -1;

	TPU.TSTR.BYTE &= ~(1<<tpu_no);		/* TCNT カウント停止 */
	TPU.TSYR.BYTE &= ~(1<<tpu_no);		/* TCNT 独立動作	  */

	if((tpu_no == TPU_DEV0) || (tpu_no == TPU_DEV3)) {

		adrs0_3->TCR.BYTE	= 0;
		adrs0_3->TMDR.BYTE	= 0;
		adrs0_3->TIOR.WORD	= 0;
		adrs0_3->TIER.BYTE	= 0;
		adrs0_3->TSR.BYTE	= 0;
		adrs0_3->TCNT		= 0;
		adrs0_3->TGRA		= 0;
		adrs0_3->TGRB		= 0;
		adrs0_3->TGRC		= 0;
		adrs0_3->TGRD		= 0;

		if(ival_t > 10000)		x = 10L;
		for(i=0; i<4; i++) {
			if(((BASE_CLOCK/x) * (long)ival_t / (psc_tbl[i]/x) ) < 65000L) {
				adrs0_3->TCR.BIT.TPSC = (unsigned char)(i);
				adrs0_3->TGRA = (unsigned int)(((BASE_CLOCK/x) * (long)ival_t) / (psc_tbl[i]/x));
				interrupt_func[tpu_no] = i_func;	/* 割り込み関数の設定 */
				adrs0_3->TCR.BIT.CCLR = 1;				/* TGRA コンペアマッチで TCNT クリアー	*/
				adrs0_3->TIER.BIT.TGIEA =1;				/* 割り込み許可	*/
				TPU.TSTR.BYTE |= (1<<tpu_no);				/* TCNT0 カウント開始 */
				return 0;
			}
		}
		return -1;
	}
	else {

		adrs1_2_4_5->TCR.BYTE	= 0;
		adrs1_2_4_5->TMDR.BYTE	= 0;
		adrs1_2_4_5->TIOR.BYTE	= 0;
		adrs1_2_4_5->TIER.BYTE	= 0;
		adrs1_2_4_5->TSR.BYTE	= 0;
		adrs1_2_4_5->TCNT		= 0;
		adrs1_2_4_5->TGRA		= 0;
		adrs1_2_4_5->TGRB		= 0;

		if(ival_t > 10000)		x = 10L;
		for(i=0; i<4; i++) {
			if(((BASE_CLOCK/x) * (long)ival_t / (psc_tbl[i]/x) ) < 65000L) {
				adrs1_2_4_5->TCR.BIT.TPSC = (unsigned char)(i);
				adrs1_2_4_5->TGRA = (unsigned int)(((BASE_CLOCK/x) * (long)ival_t) / (psc_tbl[i]/x));
				interrupt_func[tpu_no] = i_func;		/* 割り込み関数の設定 */
				adrs1_2_4_5->TCR.BIT.CCLR = 1;			/* TGRA コンペアマッチで TCNT クリアー	*/
				adrs1_2_4_5->TIER.BIT.TGIEA =1;			/* 割り込み許可	*/
				TPU.TSTR.BYTE |= (1<<tpu_no);			/* TCNT カウント開始 */
				return 0;
			}
		}
		return -1;
	}
}
