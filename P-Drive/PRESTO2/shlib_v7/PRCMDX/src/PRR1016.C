/*======================================================================
 * File name	: prr1016.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性拡張テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr1016.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: prr1016.c $
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
#include	<prcmd\prr1015.h>		/* Show Current		*/

/*----------------------------------------------------------------------*/
/*	pr_print_fd_spec_edge						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	フィーダ駆動特性構造体拡張データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_fd_spec_edge)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void  *pr_print_fd_spec_edge( 
		struct fmt_fd_spec_edge *txt, 
		struct prr1015t *pkt
		)
{
	int i;
	
	for(i=0;i<4;i++) {
		itoh((unsigned short)pkt->param[i].start.Ap, (char *)txt->mess[i].start.Ap);
		itoh((unsigned short)pkt->param[i].start.Aon, (char *)txt->mess[i].start.Aon);
		itoh((unsigned short)pkt->param[i].start.Bp, (char *)txt->mess[i].start.Bp);
		itoh((unsigned short)pkt->param[i].start.Bon, (char *)txt->mess[i].start.Bon);
		
		itoh((unsigned short)pkt->param[i].stop.Ap, (char *)txt->mess[i].stop.Ap);
		itoh((unsigned short)pkt->param[i].stop.Aon, (char *)txt->mess[i].stop.Aon);
		itoh((unsigned short)pkt->param[i].stop.Bp, (char *)txt->mess[i].stop.Bp);
		itoh((unsigned short)pkt->param[i].stop.Bon, (char *)txt->mess[i].stop.Bon);
	}
	return(++txt);
}
