/*======================================================================
 * File name    : rzr100.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	分散シャッター制御テキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr100.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr100.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr100.c 1.1 1998/07/16 14:39:13 itami
 *Log: rzr100.c
 * リビジョン 1.1  1998/07/16  14:39:13  itami
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
#include	<rzcmd\rzr099.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_sh_parm						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	分散シャッター制御構造体のデータをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)	    		*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ		    			*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_dshf_parm)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
rz_print_sh_parm(
	struct fmt_dfsh_parm	*txt,
	struct rzr099t		*pkt
	)
{
	BYTE	*buf;
	int	i, j, m, n;

	txt->ch_id     = hex_ascii( pkt->ch_id );
	txt->nof_df    = hex_ascii( pkt->nof_df );
	txt->min_df_no = hex_ascii( pkt->min_df_no );

	buf = (BYTE_PTR)txt->sh;
	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		*buf = hex_ascii( pkt->sh[j].on_off );
		buf += sizeof(txt->sh[0].on_off);
		ctoh(pkt->sh[j].fc_op_cnt, (char *)buf);
		buf += sizeof(txt->sh[0].fc_op_cnt);
		ctoh(pkt->sh[j].fc_op_time, (char *)buf);
		buf += sizeof(txt->sh[0].fc_op_time);
		ctoh(pkt->sh[j].fc_cl_time, (char *)buf);
		buf += sizeof(txt->sh[0].fc_cl_time);
		*buf = hex_ascii( pkt->sh[j].mode );
		buf += sizeof(txt->sh[0].mode);
		ctoh(pkt->sh[j].nof_sh, (char *)buf);
		buf += sizeof(txt->sh[0].nof_sh);
		ctoh(pkt->sh[j].min_sh_no, (char *)buf);
		buf += sizeof(txt->sh[0].min_sh_no);
		for(m = pkt->sh[j].nof_sh, n = pkt->sh[j].min_sh_no-1 ; m ; m--, n++) {
			ctoh(pkt->sh[j].op_time[n], (char *)buf);
			buf += sizeof(txt->sh[0].op_time[0]);
		}
	}

	return(buf);
}
