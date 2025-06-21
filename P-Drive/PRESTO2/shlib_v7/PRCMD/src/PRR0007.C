/*======================================================================
 * File name	: prr0007.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	包装機仕様テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0007.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0007.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shlib/rzcmd/src/rcs/rzr007.c 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr007.c
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
#include	<prcmd\prr0007.h>		/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	pr_scan_bmif_spec						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	包装機連動仕様テキストを構造体にセーブする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
pr_scan_bmif_spec(
	struct fmt_bmif_spec	*buf
	)
{
	BYTE	*p;
	int	i;

	prr0007s.nof_head = htoc((char *) buf->nof_head );
	prr0007s.nof_bm = ascii_hex( buf->nof_bm );

	p = (BYTE_PTR)buf->ch;
	for(i = 0 ; i < (int)prr0007s.nof_bm ; i++) {
		prr0007s.ch[i].bm_assign_ptn = htol( (char *)p );
		p += sizeof( buf->ch[i].bm_assign_ptn );
		prr0007s.ch[i].interface = ascii_hex( *p );
		p += sizeof( buf->ch[i].interface );
		prr0007s.ch[i].mul_dump_ini = ascii_hex( *p );
		p += sizeof( buf->ch[i].mul_dump_ini );
		prr0007s.ch[i].mul_dump_conf = ascii_hex( *p );
		p += sizeof( buf->ch[i].mul_dump_conf );
		prr0007s.ch[i].manu_dump_ini = ascii_hex( *p );
		p += sizeof( buf->ch[i].manu_dump_ini );
		prr0007s.ch[i].manu_dump_conf = ascii_hex( *p );
		p += sizeof( buf->ch[i].manu_dump_conf );
	}
	return(0);
}
