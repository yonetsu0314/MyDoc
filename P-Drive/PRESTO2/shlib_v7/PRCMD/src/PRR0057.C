/*======================================================================
 * File name	: prr0057.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションテキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0057.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0057.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
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
#include	<prcmd\prr0057.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	pr_scan_afd_sect						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＦＤセクションテキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_afd_sect(
	struct fmt_afd_sect	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0057s.ch_id = ascii_hex(buf->ch_id);
	prr0057s.nof_sect = ascii_hex(buf->nof_sect);
	p = (BYTE_PTR)buf->sect;
	for(i = 0 ; i < prr0057s.nof_sect ; i++ ) {
		prr0057s.sect[i].ctrl_mode = htoc( (char *)p );
		p += sizeof(buf->sect[i].ctrl_mode);
		prr0057s.sect[i].ctrl_sw = htoc( (char *)p );
		p += sizeof(buf->sect[i].ctrl_sw);
		prr0057s.sect[i].amp_max = htoc( (char *)p );
		p += sizeof(buf->sect[i].amp_max);
		prr0057s.sect[i].amp_min = htoc( (char *)p );
		p += sizeof(buf->sect[i].amp_min);
		prr0057s.sect[i].time_max = htoc( (char *)p );
		p += sizeof(buf->sect[i].time_max);
		prr0057s.sect[i].time_min = htoc( (char *)p );
		p += sizeof(buf->sect[i].time_min);
	}
	prr0057s.nof_df = ascii_hex( *p );
	p += sizeof(buf->nof_df);
	for(i = 0 ; i < prr0057s.nof_df ; i++ ) {
		prr0057s.df[i].amp_max = htoc( (char *)p );
		p += sizeof(buf->df[i].amp_max);
		prr0057s.df[i].amp_min = htoc( (char *)p );
		p += sizeof(buf->df[i].amp_min);
		prr0057s.df[i].time_max = htoc( (char *)p );
		p += sizeof(buf->df[i].time_max);
		prr0057s.df[i].time_min = htoc( (char *)p );
		p += sizeof(buf->df[i].time_min);
	}
	prr0057s.nof_af = htoc( (char *)p );
	p += sizeof(buf->nof_af);
	for(i = 0 ; i < prr0057s.nof_af ; i++ ) {
		prr0057s.af[i].amp_max = htoc( (char *)p );
		p += sizeof(buf->af[i].amp_max);
		prr0057s.af[i].amp_min = htoc( (char *)p );
		p += sizeof(buf->af[i].amp_min);
		prr0057s.af[i].time_max = htoc( (char *)p );
		p += sizeof(buf->af[i].time_max);
		prr0057s.af[i].time_min = htoc( (char *)p );
		p += sizeof(buf->af[i].time_min);
	}
	return(0);
}
