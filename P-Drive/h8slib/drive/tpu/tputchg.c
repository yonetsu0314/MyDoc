/*----------------------------------------------------------------------*
 * File name	: tputchg.c		/ system initialize						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/tpu/RCS/tputchg.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tputchg.c $
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
#include	"dtpu.h"

/*----------------------------------------------------------------------*/
/* タイマーの再設定														*/
/*																		*/
/* 引き数																*/
/*		TPU 番号 0-5													*/
/*		割り込み周期													*/
/* 戻り値																*/
/*		 0	･･･ 正常終了												*/
/*		 -1	･･･ 失敗													*/
/*----------------------------------------------------------------------*/
int tpu_tmr_ival_chg(
	int tpu_no,							/* 0 to 5 */
	unsigned short	ival_t				/* 1usec ～ 65535usec */
	)
{
	int		i;
	long	x = 1L;
	volatile struct st_tpu0 *adrs0_3 	 = tpu_adrs[tpu_no];
	volatile struct st_tpu1 *adrs1_2_4_5 = tpu_adrs[tpu_no];

	if((tpu_no < 0) || (tpu_no >= TPU_DEV_MAX))		return -1;
	if(interrupt_func[tpu_no] == (void *)0)			return -1;

	TPU.TSTR.BYTE &= ~(1<<tpu_no);			/* TCNT カウント停止 */

	if((tpu_no == TPU_DEV0) || (tpu_no == TPU_DEV3)) {
		adrs0_3->TIER.BIT.TGIEA = 0;		/* 割り込み停止	*/
		adrs0_3->TCNT			= 0;		/* カウンタークリアー */

		if(ival_t > 10000)		x = 10L;
		for(i=0; i<4; i++) {
			if(((BASE_CLOCK/x) * (long)ival_t / (psc_tbl[i]/x) ) < 65000L) {
				adrs0_3->TCR.BIT.TPSC = (unsigned char)(i);
				adrs0_3->TGRA = (unsigned int)(((BASE_CLOCK/x) * (long)ival_t) / (psc_tbl[i]/x));
				adrs0_3->TIER.BIT.TGIEA =1;				/* 割り込み許可		*/
				TPU.TSTR.BYTE |= (1<<tpu_no);			/* TCNT カウント開始 */
				return 0;
			}
		}
		return -1;
	}
	else {
		adrs1_2_4_5->TIER.BIT.TGIEA = 0;		/* 割り込み停止	*/
		adrs1_2_4_5->TCNT			= 0;		/* カウンタークリアー */

		if(ival_t > 10000)		x = 10L;
		for(i=0; i<4; i++) {
			if(((BASE_CLOCK/x) * (long)ival_t / (psc_tbl[i]/x) ) < 65000L) {
				adrs1_2_4_5->TCR.BIT.TPSC = (unsigned char)(i);
				adrs1_2_4_5->TGRA = (unsigned int)(((BASE_CLOCK/x) * (long)ival_t) / (psc_tbl[i]/x));
				adrs1_2_4_5->TIER.BIT.TGIEA =1;			/* 割り込み許可		*/
				TPU.TSTR.BYTE |= (1<<tpu_no);			/* TCNT カウント開始 */
				return 0;
			}
		}
		return -1;
	}
}
