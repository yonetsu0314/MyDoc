/*======================================================================
 * File name    : rzr117.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイミング拡張パラメータ（１）テキスト解読
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr117.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr117.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/rcs/rzr117.c 1.2 1995/05/15 17:42:39 sakagami
 *Log: rzr117.c
 * リビジョン 1.2  1995/05/15  17:42:39  sakagami
 * サーボ上限停止時間の追加
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
#include	<rzcmd\rzr117.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_rz600_tmg						*/
/*----------------------------------------------------------------------*/
/* 説明					  				*/
/*	ＲＺ－６００用タイミングテキストを構造体にセーブする。		*/
/*----------------------------------------------------------------------*/
/* 引き数	      							*/
/*	*buf	: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		    		*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
rz_scan_rz600_tmg(
	struct fmt_rz600_tmg	*buf
	)
{
	rzr117s.cnv_delay    = htoc( (char *)buf->cnv_delay );/*搬送ｺﾝﾍﾞﾔ遅れ時間*/
	rzr117s.cnv_hold_tm  = htoc( (char *)buf->cnv_hold_tm );/*搬送ｺﾝﾍﾞﾔオン時間*/
	rzr117s.srv_delay    = htoc( (char *)buf->srv_delay );/*排出用ｻｰﾎﾞ遅れ時間*/
	rzr117s.srv_hold_tm  = htoc( (char *)buf->srv_hold_tm );/*ｻｰﾎﾞ上限停止時間*/
	rzr117s.sv_delay     = htoc( (char *)buf->sv_delay );/*排出用ｼﾘﾝﾀﾞ遅れ時間*/
	rzr117s.sv_hold_tm   = htoc( (char *)buf->sv_hold_tm );/*排出用ｼﾘﾝﾀﾞオン時間*/
	rzr117s.men1_delay   = htoc( (char *)buf->men1_delay );  /*麺押し１遅れ時間*/
	rzr117s.men1_hold_tm = htoc( (char *)buf->men1_hold_tm );/*麺押し１オン時間*/
	rzr117s.men2_delay   = htoc( (char *)buf->men2_delay );  /*麺押し２遅れ時間*/
	rzr117s.men2_hold_tm = htoc( (char *)buf->men2_hold_tm );/*麺押し２オン時間*/
	rzr117s.men3_delay   = htoc( (char *)buf->men3_delay );  /*麺押し３遅れ時間*/
	rzr117s.men3_hold_tm = htoc( (char *)buf->men3_hold_tm );/*麺押し３オン時間*/
	rzr117s.men4_delay   = htoc( (char *)buf->men4_delay );  /*麺押し４遅れ時間*/
	rzr117s.men4_hold_tm = htoc( (char *)buf->men4_hold_tm );/*麺押し４オン時間*/
	return(0);
}
