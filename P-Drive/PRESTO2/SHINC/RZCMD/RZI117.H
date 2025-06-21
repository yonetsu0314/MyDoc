/*======================================================================
 * File name    : rzi117.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生サブデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi117.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi117.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzi117.h 1.3 1994/11/25 08:43:59 hiroe
 *Log: rzi117.h
 * リビジョン 1.3  1994/11/25  08:43:59  hiroe
 * 金属検出用のエラー情報を追加する
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI117_H)
#define	RZI117_H


enum rzi1170e {				/* 異常内容			*/
	ERROR_AD,			/* AD				*/
	ERROR_RAM,			/* RAM				*/
	ERROR_ROM,			/* ROM				*/
	ERROR_AUTO_DF,			/* 分散重量自動変更		*/
	ERROR_METAL,			/* 金属検出			*/
};

/*------------------------------------------------------*/
/*	異常情報のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi117t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzi1170e	error;		/* 異常情報			*/
	unsigned long	error_ptn;	/* error pattern		*/
} RZI117T;

#if	defined(STORAGE)
struct rzi117t	rzi117s;
#else
#if	!defined(NOEXTERN)
extern struct rzi117t	rzi117s;
#endif
#endif

#endif
