/*======================================================================
 * File name    : rzr099.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	分散シャッターテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr099.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr099.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr099.c 1.1 1998/07/16 14:39:07 itami
 *Log: rzr099.c
 * リビジョン 1.1  1998/07/16  14:39:07  itami
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
#define		STORAGE
#include	<rzcmd\rzr099.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_sh_parm							*/
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
rz_scan_sh_parm(
	struct fmt_dfsh_parm	*buf
	)
{
	BYTE	*p;
	int	i, j, m, n;

	rzr099s.ch_id     = ascii_hex( buf->ch_id );
	rzr099s.nof_df    = ascii_hex( buf->nof_df );
	rzr099s.min_df_no = ascii_hex( buf->min_df_no );

	p = (BYTE_PTR)&buf->sh;
	for(i = rzr099s.nof_df, j = rzr099s.min_df_no-1 ; i ; i--, j++) {
		rzr099s.sh[j].on_off = ascii_hex( *p );
		p += sizeof(buf->sh[0].on_off);
		rzr099s.sh[j].fc_op_cnt = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_op_cnt);
		rzr099s.sh[j].fc_op_time = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_op_time);
		rzr099s.sh[j].fc_cl_time = htoc( (char *)p );
		p += sizeof(buf->sh[0].fc_cl_time);
		rzr099s.sh[j].mode = ascii_hex( *p );
		p += sizeof(buf->sh[0].mode);
		rzr099s.sh[j].nof_sh = htoc( (char *)p );
		p += sizeof(buf->sh[0].nof_sh);
		rzr099s.sh[j].min_sh_no = htoc( (char *)p );
		p += sizeof(buf->sh[0].min_sh_no);

		for(m = rzr099s.sh[j].nof_sh, n = rzr099s.sh[j].min_sh_no-1 ; m ; m--, n++) {
			rzr099s.sh[j].op_time[n] = htoc( (char *)p );
			p += sizeof(buf->sh[0].op_time[0]);
		}
	}

	return(0);
}
