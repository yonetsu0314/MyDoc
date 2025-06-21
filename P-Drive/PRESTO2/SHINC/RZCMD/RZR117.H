/*======================================================================
 * File name    : rzr117.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングデータ拡張パラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr117.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr117.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr117.h 1.2 1995/05/15 17:50:09 sakagami
 *Log: rzr117.h
 * リビジョン 1.2  1995/05/15  17:50:09  sakagami
 * サーボ上限停止時間の追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR117_H)
#define	RZR117_H

/*------------------------------------------------------*/
/*	タイミングパラメータパケット（拡張分）		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr117t {
	unsigned char	cnv_delay;	/* 搬送ｺﾝﾍﾞﾔ  遅れ時間　*/
	unsigned char	cnv_hold_tm;	/* 搬送ｺﾝﾍﾞﾔ　オン時間　*/
	unsigned char	srv_delay;	/* 排出用ｻｰﾎﾞ 遅れ時間  */
	unsigned char	srv_hold_tm;	/* ｻｰﾎﾞ　 上限停止時間  */
	unsigned char	sv_delay;	/* 排出用ｼﾘﾝﾀﾞ遅れ時間	*/
	unsigned char	sv_hold_tm;	/* 排出用ｼﾘﾝﾀﾞオン時間　*/
	unsigned char	men1_delay;	/* 麺押し１遅れ時間　　 */
	unsigned char	men1_hold_tm;	/* 麺押し１オン時間     */
	unsigned char	men2_delay;	/* 麺押し２遅れ時間　　 */
	unsigned char	men2_hold_tm;	/* 麺押し２オン時間     */
	unsigned char	men3_delay;	/* 麺押し３遅れ時間（予備）*/
	unsigned char	men3_hold_tm;	/* 麺押し３オン時間（予備）*/
	unsigned char	men4_delay;	/* 麺押し４遅れ時間（予備）*/
	unsigned char	men4_hold_tm;	/* 麺押し４オン時間（予備）*/
} RZR117T;

#if	defined(STORAGE)
struct rzr117t	rzr117s;
#else
#if	!defined(NOEXTERN)
extern struct rzr117t	rzr117s;
#endif
#endif

#endif
