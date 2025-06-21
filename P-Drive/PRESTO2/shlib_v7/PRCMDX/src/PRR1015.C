/*======================================================================
 * File name	: prr1015.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性拡張テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr1015.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: prr1015.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#define		STORAGE
#include	<prcmd\prr1015.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_fd_spec_edge							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動特性拡張テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_fd_spec_edge(
	struct fmt_fd_spec_edge	*buf
	)
{
	int i;
	PRR10151 *p;
	
	for(i=0;i<4;i++) {
		p = &buf->mess[i];
		prr1015s.param[i].start.Ap = htoi((char *)p->start.Ap);
		prr1015s.param[i].start.Aon = htoi((char *)p->start.Aon);
		prr1015s.param[i].start.Bp = htoi((char *)p->start.Bp);
		prr1015s.param[i].start.Bon = htoi((char *)p->start.Bon);
		prr1015s.param[i].stop.Ap = htoi((char *)p->stop.Ap);
		prr1015s.param[i].stop.Aon = htoi((char *)p->stop.Aon);
		prr1015s.param[i].stop.Bp = htoi((char *)p->stop.Bp);
		prr1015s.param[i].stop.Bon = htoi((char *)p->stop.Bon);
	}

	return(0);
}
