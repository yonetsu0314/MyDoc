/*======================================================================
 * File name	: prr0134.c
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	新ＡＦＤテキスト作成
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/prcmd/src/RCS/prr0134.c 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0134.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr0133.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_adv_afd						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	新ＡＦＤの構造体データをテキストに変換する。			*/
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
pr_print_adv_afd(
	struct fmt_adv_afd	*txt,
	struct prr0133t		*pkt
	)
{
	BYTE	*buf;
	int	i,j,sect;

	txt->nof_ch = hex_ascii(pkt->nof_ch);
	buf = (BYTE_PTR)txt->ch;
	for(i = 0; i < pkt->nof_ch ; i++ ) {
		*buf = hex_ascii(pkt->ch[i].nof_sect);
		buf += sizeof(txt->ch[i].nof_sect);
		for(sect = 0 ; sect < pkt->ch[i].nof_sect ; sect++ ) {
			for(j = 0 ; j < sizeof(pkt->ch[i].sect[sect].tbl) ; j++ ) {
				ctoh((unsigned char)pkt->ch[i].sect[sect].tbl[j], (char *)buf);
				buf += sizeof( txt->ch[i].sect[sect].tbl[j] );
			}
			ctoh((unsigned char)pkt->ch[i].sect[sect].zero_d_num, (char *)buf);
			buf += sizeof( txt->ch[i].sect[sect].zero_d_num );
			ctoh((unsigned char)pkt->ch[i].sect[sect].p_os_c_num, (char *)buf);
			buf += sizeof( txt->ch[i].sect[sect].p_os_c_num );
			ctoh(pkt->ch[i].sect[sect].trb_off_head, (char *)buf);
			buf += sizeof( txt->ch[i].sect[sect].trb_off_head );
			ctoh((unsigned char)pkt->ch[i].sect[sect].cln_req_sw, (char *)buf);
			buf += sizeof( txt->ch[i].sect[sect].cln_req_sw );
			itoh(pkt->ch[i].sect[sect].good_effi, (char *)buf);
			buf += sizeof( txt->ch[i].sect[sect].good_effi );
		}
	}
	*buf = hex_ascii(pkt->nof_infeed);
	buf += sizeof( txt->nof_infeed );
	for(i = 0 ; i < pkt->nof_infeed ; i++ ) {
		itoh((unsigned short)pkt->ifd[i].df_fuzzy_gain, (char *)buf);
		buf += sizeof( txt->ifd[i].df_fuzzy_gain );
		itoh(pkt->ifd[i].df_l_detect, (char *)buf);
		buf += sizeof( txt->ifd[i].df_l_detect );
		ctoh((unsigned char)pkt->ifd[i].df_ow_ctrl, (char *)buf);
		buf += sizeof( txt->ifd[i].df_ow_ctrl );
		ctoh((unsigned char)pkt->ifd[i].dfw_on_off, (char *)buf);
		buf += sizeof( txt->ifd[i].dfw_on_off );
		ctoh((unsigned char)pkt->ifd[i].df_e_detect1, (char *)buf);
		buf += sizeof( txt->ifd[i].df_e_detect1 );
		ctoh((unsigned char)pkt->ifd[i].df_e_detect2, (char *)buf);
		buf += sizeof( txt->ifd[i].df_e_detect2 );
		itoh((unsigned short)pkt->ifd[i].df_w_max, (char *)buf);
		buf += sizeof( txt->ifd[i].df_w_max );
		itoh((unsigned short)pkt->ifd[i].df_w_min, (char *)buf);
		buf += sizeof( txt->ifd[i].df_w_min );
		ctoh(pkt->ifd[i].low_wt, (char *)buf);
		buf += sizeof( txt->ifd[i].low_wt );
	}
	return(buf);
}
