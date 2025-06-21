/*----------------------------------------------------------------------*
 * File name	: e2wrtsk.c	/ EE-PROM write task			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2wrtsk.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2wrtsk.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141				       		*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	15msecのタイマ割り込みの中で実行され、リングバッファから	*/
/*	EEPROMへページ単位で書き込みを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI std		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<biosif\e2rom.h>		/* Show bios		*/
#include	"e2ctl.h"			/* SHow current		*/
#include	"e2ctxt.h"			/*	"		*/

static struct e2ioh	drv_info = {
		(void *)0,		/* pointer to ring buffer	*/
		0,			/* size of ring buffer		*/
		&e2rom_hd.e2f_p,	/* DGROUP参照			*/
		&e2rom_hd.putp,		/*	"			*/
		&e2rom_hd.getp		/*	"			*/
};

static void
e2rom_wr_tsk(
	int	init_code
)
{
	unsigned int		sem_cnt;	/* 返却する資源数	*/
	int			err;

	drv_info.e2buf = (char *)e2rom_hd.buff;
	drv_info.szbuf = e2rom_hd.buf_max;
	while(1) {
		err = SLP_TSK();
		assert(! err);
		while(e2rom_hd.getp != e2rom_hd.putp) {
			sem_cnt = _e2rom_pgwr(&drv_info);
			err = HLT_TSK(&interval);
			assert(! err);
			if(e2rom_hd.getp == e2rom_hd.putp)
				e2rom_hd.flag &= ~WR_ON;
			err = SIG_SEM(e2buf_sem.acadr, sem_cnt);
			assert(! err);
		}
	}
}

/*----------------------------------------------------------------------*/
/*	init_e2rom_tsk							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: error							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	e2rom_wr_tsk()関係の初期化を行う。				*/
/*----------------------------------------------------------------------*/

#ifdef	NDEBUG
#define	DEF_STK		0x100
#else
#define	DEF_STK		0x200
#endif

int
init_e2rom_tsk(void)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/

	tsk_pkt.tsk_adr = e2rom_wr_tsk;
	tsk_pkt.priority = (unsigned int)E2R_TSK_PRI;
	tsk_pkt.stk_size = DEF_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = e2r_ds.acadr;
	if(CRE_TSK( &e2wr_tsk.acadr, (unsigned int)E2R_TSK_ID, &tsk_pkt))
								return(-1);
	if(STA_TSK( e2wr_tsk.acadr, (unsigned int)0))		return(-1);
	return(0);
}
