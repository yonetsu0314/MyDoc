/*----------------------------------------------------------------------*
 * File name	: tmrtsk.c	/ software timer decrement task		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/timer/RCS/tmrtsk.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: tmrtsk.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/17 13:45
 * CAN_WUP()でshort型の引数wup_cntのポインタをintにキャストしていたために、
 * 奇数アドレスにintを書き込みアドレスエラーとなっていた不具合修正。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	本ﾀｽｸは5msecで周期起床され、ﾀｲﾏｰ･ﾚｼﾞｽﾀをﾃﾞｨｸﾘﾒﾝﾄする。なお、	*/
/*	このﾀｽｸはﾌﾟﾗｲｵﾘﾃｨの関係でOSからｻｰﾋﾞｽを受けられない場合を想	*/
/*	定し、ﾀｲﾏｰの精度の保証とﾀｽｸのｵｰﾊﾞｰﾍｯﾄﾞを軽減するため、ﾀｽｸに	*/
/*	かけられている複数回の起床要求を一回の処理で実行する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	"tmrctl.h"

static void
timer_tsk(
	int			init_code	/* ﾀｽｸに与える初期値	*/
	)
{
	unsigned int		wup_cnt;	/* 起床要求回数		*/
	unsigned int		no_itm5,	/* 5msﾀｲﾏｰのｱｲﾃﾑ数	*/
				no_itm10;	/* 10msﾀｲﾏｰのｱｲﾃﾑ数	*/
	static unsigned int	alternate = 0;	/* ｵﾙﾀﾈｰﾄ･ﾌﾗｸﾞ		*/
	int			err;
	register int		n;

	no_itm5 = item_n[0];
	no_itm10 = item_n[1];
	while(1) {
		err = SLP_TSK();
		err |= CAN_WUP(&wup_cnt, tmr_tsk.acadr);
		assert(! err);
		wup_cnt ++;
		if(no_itm5) {
			n = 0;
			do {
				timer_dec( &tm5[n], (unsigned short)wup_cnt);
			} while(++ n < no_itm5);
		}
		if(no_itm10) {
			alternate += wup_cnt;
			if(wup_cnt = (unsigned short)(alternate >> 1)) {
				n = 0;
				do {
					timer_dec( &tm10[n], (unsigned short)wup_cnt);
				} while(++ n < no_itm10);
			}
			alternate &= 0x1;
		}
	}
	init_code = init_code;	/* For warning. */
}

/*----------------------------------------------------------------------*/
/*	init_tmr_tsk							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: error							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	timer_tsk()関係の初期化を行う。					*/
/*----------------------------------------------------------------------*/

#ifdef	NDEBUG
#define	DEF_STK		0x100
#else
#define	DEF_STK		0x200
#endif

int init_tmr_tsk(void)
{
/*	int		err; */
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/
	struct pkcwup	cwup_pkt;		/* cyc_wup用ﾊﾟｹｯﾄ	*/

	tsk_pkt.tsk_adr = timer_tsk;
	tsk_pkt.priority = TMR_TSK_PRI;
	tsk_pkt.stk_size = DEF_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = tmr_ds.acadr;
	if(CRE_TSK( &tmr_tsk.acadr, (unsigned int)TMR_TSK_ID, (struct pktsk *)&tsk_pkt))	return(-1);
	cwup_pkt.ltime = 10;
	cwup_pkt.utime = 0;
	cwup_pkt.interval = 5;
	cwup_pkt.count = 0;
	if(CYC_WUP( (unsigned int)REL_TIME, tmr_tsk.acadr, (struct pkcwup *)&cwup_pkt))	return(-1);
	if(STA_TSK(tmr_tsk.acadr, (unsigned int)0))				return(-1);
	return(0);
}
