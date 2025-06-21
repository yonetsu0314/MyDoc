/*----------------------------------------------------------------------*
 * File name	: rcvtsk.c	/ moniter recieve task			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/rcvtsk.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: rcvtsk.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/04 ciosim.hのincludeをやめる。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/rcvtsk.c 1.1 1996/07/22 19:25:48
 *Log: rcvtsk.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI atd		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	"rxdef.h"			/* Show APL directory	*/
#ifdef	_SIM
#define	STORAGE
#endif
#include	"cmfhdl.h"			/*	"		*/
#ifdef	STORAGE
#undef	STORAGE
#endif
#include	"admtxt.h"			/* current		*/


void	abort(void);
/*----------------------------------------------------------------------*/
/*	mon_rcv_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	HOSTから受信したテキストの解析を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	init_code	: ﾀｽｸに与える初期値				*/
/*									*/
/* 戻り値								*/
/*	void		: 無し						*/
/*----------------------------------------------------------------------*/

static void
mon_rcv_tsk(
	unsigned int	init_code		/* ﾀｽｸに与える初期値	*/
	)
{
	int		len;			/* ﾃｷｽﾄ長		*/
	unsigned char	buf[RXBUF_MAX];		/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ		*/
	char		*argl[1];		/* 引き数ﾘｽﾄ		*/
	int		err;			/* error code		*/

	while(1) {
		err = SLP_TSK();
		assert(! err);
		len = (int)con_gets((char *)&buf[0], (int)sizeof(buf));
		if(len  && (len != COM_FAIL)) {
			if(len == RCV_ERROR)
				mon_text_snd( MON_COM_ERROR, (void **)argl);
			else
				mon_text_interpret( &buf[0], len);
		}
	}
}

/*----------------------------------------------------------------------*/
/*	setup_mon_rcv							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	モニター用I/Oからの受信タスクの生成及び起動を行う。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/

#define	WORK	1024

int
setup_mon_rcv(void)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/

	tsk_pkt.tsk_adr = (void *)mon_rcv_tsk;
	tsk_pkt.priority = MONRCV_TSK_PRI;
	tsk_pkt.stk_size = RXBUF_MAX +TXBUF_MAX +WORK +0x200;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	if(CRE_TSK( &rxtsk[CON_DN].acadr, (unsigned int)MONRCV_TSK_ID,
			&tsk_pkt))				return(-1);
	if(STA_TSK( rxtsk[CON_DN].acadr, (unsigned int)0))			return(-1);
	return(0);
}
