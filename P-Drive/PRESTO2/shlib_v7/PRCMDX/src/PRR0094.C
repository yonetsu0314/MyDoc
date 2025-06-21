/*======================================================================
 * File name    : prr0094.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	マルチ連動パラメータ２テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/rcs/prr0094.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0094.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr094.c 1.3 1997/12/09 11:45:02
 *Log: rzr094.c
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0093.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_mlt_parm2						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	マルチ連動パラメータ２をテキストに変換する。	    		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dca_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *pr_print_mlt_parm2(
	struct fmt_mlt_parm2	*txt,
	struct prr0093t		*pkt
	)
{
	txt->ch_id = hex_ascii( pkt->ch_id );
        ctoh( pkt->multi_onoff, (char *)&txt->multi_onoff );
        ctoh( pkt->multi_enter, (char *)&txt->multi_enter );
        ctoh( pkt->combi_delay, (char *)&txt->combi_delay );
        ctoh( pkt->consec_mode, (char *)&txt->consec_mode );
        ctoh( pkt->mlt_dump_mode, (char *)&txt->mlt_dump_mode );
        ctoh( pkt->interface, (char *)&txt->interface );
        ctoh( pkt->speed, (char *)&txt->speed );
        ctoh( pkt->conv_delay, (char *)&txt->conv_delay );
        ctoh( pkt->conv_on_time, (char *)&txt->conv_on_time );
        ctoh( pkt->ds_delay, (char *)&txt->ds_delay );
        ctoh( pkt->ds_on_time, (char *)&txt->ds_on_time );
        ctoh( pkt->ds_sht_cnt, (char *)&txt->ds_sht_cnt );
        ctoh( pkt->mlt_dump_conf, (char *)&txt->mlt_dump_conf );
        ctoh( pkt->p_ds_delay, (char *)&txt->p_ds_delay );
        ctoh( pkt->p_ds_on_time, (char *)&txt->p_ds_on_time );
        ctoh( pkt->p_ds_sht_cnt, (char *)&txt->p_ds_sht_cnt );
	return(++txt);
}
