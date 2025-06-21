/*======================================================================
 * File name    : rzi015.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	警報発生データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi015.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi015.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi015.h 2.8 1970/01/01 00:00:00 itami
 *Log: rzi015.h
 * リビジョン 2.8  1970/01/01  00:00:00  itami
 * 2002/04/18 先端PH TB MB BB REJ エラーワーニング追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI015_H)
#define	RZI015_H


enum rzi0150e {				/* 警報内容			*/
	WRNG_END,			/* 解除				*/
	WRNG_OVSCL,			/* オーバースケール		*/
	WRNG_UNDSCL,			/* アンダースケール		*/
	WRNG_ERRDMP,			/* エラー自動排出		*/
	WRNG_AZERR,			/* 自動零点			*/
	WRNG_COMERR,			/* 通信エラー			*/
	WRNG_AFD,			/* AFD				*/
	WRNG_LEFTWGT,			/* 付着重量許容値オーバー	*/
	WRNG_HP_OFF,			/* ホッパ未装備（ホッパ交換用）	*/
	WRNG_HP_FULL,			/* つまりエラー			*/
        WRNG_CNTERR,                    /* 個数換算エラー               */
        WRNG_UNDWT,                     /* 軽量エラー                   */
        WRNG_TB,                     	/* TBエラー                     */
        WRNG_MB,                     	/* MBエラー                     */
        WRNG_BB,                     	/* BBエラー                     */
        WRNG_REJ,                     	/* REJエラー                    */
        WRNG_TPH,                     	/* 先端PHエラー                 */
};

/*------------------------------------------------------*/
/*	アラーム情報のパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi015t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzi0150e	alarm;		/* 警報情報			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned char	afd;		/* AFD warning id		*/
} RZI015T;

#if	defined(STORAGE)
struct rzi015t	rzi015s;
#else
#if	!defined(NOEXTERN)
extern struct rzi015t	rzi015s;
#endif
#endif

#endif
