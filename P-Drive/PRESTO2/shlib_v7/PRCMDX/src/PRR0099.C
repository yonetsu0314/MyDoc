/*======================================================================
 * File name    : prr0099.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	分散シャッターテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmdx/src/RCS/prr0099.c 1.1 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0099.c $
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
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
#define		STORAGE
#include	<prcmd\prr0099.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_sh_parm							*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	分散シャッター設定テキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_sh_parm(
	struct fmt_dfsh_parm	*buf
	)
{
	BYTE	*p;
	int	i, j, m, n;

	prr0099s.ch_id     = ascii_hex( buf->ch_id );
	prr0099s.nof_df    = ascii_hex( buf->nof_df );
	prr0099s.min_df_no = ascii_hex( buf->min_df_no );

	p = (BYTE_PTR)&buf->sh;
	for(i = prr0099s.nof_df, j = prr0099s.min_df_no-1 ; i ; i--, j++) {
		prr0099s.sh[j].on_off = ascii_hex( *p );
		p += sizeof(buf->sh[0].on_off);
		prr0099s.sh[j].fc_op_cnt = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_op_cnt);
		prr0099s.sh[j].fc_op_time = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_op_time);
		prr0099s.sh[j].fc_cl_time = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_cl_time);
		prr0099s.sh[j].mode = ascii_hex( *p );
		p += sizeof(buf->sh[0].mode);
		prr0099s.sh[j].nof_sh = htoc( (char *)p );
		p += sizeof(buf->sh[0].nof_sh);
		prr0099s.sh[j].min_sh_no = htoc( (char *)p );
		p += sizeof(buf->sh[0].min_sh_no);

		for(m = prr0099s.sh[j].nof_sh, n = prr0099s.sh[j].min_sh_no-1 ; m ; m--, n++) {
			prr0099s.sh[j].op_time[n] = htoc( (char *)p );
			p += sizeof(buf->sh[0].op_time[0]);
		}
	}

	return(0);
}
