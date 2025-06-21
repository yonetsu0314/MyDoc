/*----------------------------------------------------------------------*
 * File name	: frwrtsk.c	/ FLASH ROM write task			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frwrtsk.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frwrtsk.c $
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
/*	FLASH ROMへページ単位で書き込みを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI std		*/
/*#include	<dos.h>*/			/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<biosif\from.h>		/* Show bios		*/
#include	"frctl.h"			/* SHow current		*/
#include	"frctxt.h"			/*	"		*/

static struct frioh	drv_info = {
		(void *)0,		/* pointer to ring buffer	*/
		0,			/* size of ring buffer		*/
		&flashrom_hd.frf_p,	/* DGROUP参照			*/
		&flashrom_hd.putp,	/*	"			*/
		&flashrom_hd.getp	/*	"			*/
};

static void
flashrom_wr_tsk(
	int	init_code
	)
{
	unsigned int		sem_cnt;	/* 返却する資源数	*/
	int			err;

	drv_info.frbuf = (char *)flashrom_hd.buff;
	drv_info.szbuf = flashrom_hd.buf_max;
	while(1) {
		err = SLP_TSK();
		assert(! err);
		while(flashrom_hd.getp != flashrom_hd.putp) {
			sem_cnt = _flashrom_pgwr(&drv_info);
			err = HLT_TSK(&interval);
			assert(! err);
			if(flashrom_hd.getp == flashrom_hd.putp)
				flashrom_hd.flag &= ~WR_ON;
			err = SIG_SEM(frbuf_sem.acadr, sem_cnt);
			assert(! err);
		}
	}
}

/*----------------------------------------------------------------------*/
/*	init_flashrom_tsk						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: error							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	flashrom_wr_tsk()関係の初期化を行う。				*/
/*----------------------------------------------------------------------*/

#ifdef	NDEBUG
#define	DEF_STK		0x100
#else
#define	DEF_STK		0x200
#endif

int
init_flashrom_tsk(void)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/

	tsk_pkt.tsk_adr = flashrom_wr_tsk;
	tsk_pkt.priority = (unsigned int)FR_TSK_PRI;
	tsk_pkt.stk_size = DEF_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = fr_ds.acadr;
	if(CRE_TSK( &frwr_tsk.acadr, (unsigned int)FR_TSK_ID, &tsk_pkt))
								return(-1);
	if(STA_TSK( frwr_tsk.acadr, (unsigned int)0))		return(-1);
	return(0);
}
