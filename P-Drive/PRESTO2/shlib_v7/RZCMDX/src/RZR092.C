/*======================================================================
 * File name    : rzr092.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動パラメータ1テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr092.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr092.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr092.c 1.3 1997/12/08 21:39:56 itami
 *Log: rzr092.c
 * リビジョン 1.3  1997/12/08  21:39:56  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr091.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_mlt_parm1						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動パラメータ1をテキストに変換する。	    		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *rz_print_mlt_parm1(
	struct fmt_mlt_parm1	*txt,
	struct rzr091t		*pkt
	)
{
	unsigned short	c,d;

	txt->ch_id = hex_ascii( pkt->ch_id );
	ctoh( pkt->neighbor_node, (char *)&txt->neighbor_node );
	ctoh( pkt->shift_cnt, (char *)&txt->shift_cnt );
	ctoh( pkt->retry_time, (char *)&txt->retry_time );
	ctoh( pkt->retry_span, (char *)&txt->retry_span );
	ctoh( pkt->retry_cnt, (char *)&txt->retry_cnt );
	ctoh( pkt->master_slave, (char *)&txt->master_slave );
	ctoh( pkt->ch_status, (char *)&txt->ch_status );
/*	itoh( *((unsigned short *)&pkt->dsw), (char *)&txt->dsw );	*/
/*	処理系によるビットフィールドの違いを無理矢理あわせる		*/
	d = pkt->dsw.my_node;
	c = pkt->dsw.chg_ch;
	c <<= 5;
	d |= c;
	c = pkt->dsw.ilk_inf;
	c <<= 6;
	d |= c;
	c = pkt->dsw.Dummy;
	c <<= 8;
	d |= c;
	itoh( d, (char *)&txt->dsw );
	return(++txt);
}
