/*======================================================================
 * File name	: prr0058.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＤセクションテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0058.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0058.c $
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
#include	<prcmd\prr0057.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_afd_sect						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ＡＦＤセクションの構造体データをテキストに変換する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*txt	: 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ(char *)			*/
/*	*pkt	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: txt+sizeof(fmt_afd_sect)	･･･ 正常終了		*/
/*		  NULL				･･･ エラー		*/
/*----------------------------------------------------------------------*/

void *
pr_print_afd_sect(
	struct fmt_afd_sect	*txt,
	struct prr0057t		*pkt
	)
{
	BYTE	*buf;
	int	i;

	txt->ch_id = hex_ascii(pkt->ch_id);
	txt->nof_sect = hex_ascii(pkt->nof_sect);
	buf = (BYTE_PTR)txt->sect;
	for(i = 0 ; i < pkt->nof_sect ; i++ ) {
		ctoh((unsigned char)pkt->sect[i].ctrl_mode, (char *)buf);
		buf += sizeof( txt->sect[i].ctrl_mode );
		ctoh((unsigned char)pkt->sect[i].ctrl_sw, (char *)buf);
		buf += sizeof( txt->sect[i].ctrl_sw );
		ctoh(pkt->sect[i].amp_max, (char *)buf);
		buf += sizeof( txt->sect[i].amp_max );
		ctoh(pkt->sect[i].amp_min, (char *)buf);
		buf += sizeof( txt->sect[i].amp_min );
		ctoh(pkt->sect[i].time_max, (char *)buf);
		buf += sizeof( txt->sect[i].time_max );
		ctoh(pkt->sect[i].time_min, (char *)buf);
		buf += sizeof( txt->sect[i].time_min );
	}
	*buf = hex_ascii(pkt->nof_df);
	buf += sizeof( txt->nof_df );
	for(i = 0 ; i < pkt->nof_df ; i++ ) {
		ctoh(pkt->df[i].amp_max, (char *)buf);
		buf += sizeof( txt->df[i].amp_max );
		ctoh(pkt->df[i].amp_min, (char *)buf);
		buf += sizeof( txt->df[i].amp_min );
		ctoh(pkt->df[i].time_max, (char *)buf);
		buf += sizeof( txt->df[i].time_max );
		ctoh(pkt->df[i].time_min, (char *)buf);
		buf += sizeof( txt->df[i].time_min );
	}
	ctoh(pkt->nof_af, (char *)buf);
	buf += sizeof( txt->nof_af );
	for(i = 0 ; i < pkt->nof_af ; i++ ) {
		ctoh(pkt->af[i].amp_max, (char *)buf);
		buf += sizeof( txt->af[i].amp_max );
		ctoh(pkt->af[i].amp_min, (char *)buf);
		buf += sizeof( txt->af[i].amp_min );
		ctoh(pkt->af[i].time_max, (char *)buf);
		buf += sizeof( txt->af[i].time_max );
		ctoh(pkt->af[i].time_min, (char *)buf);
		buf += sizeof( txt->af[i].time_min );
	}
	return(buf);
}
