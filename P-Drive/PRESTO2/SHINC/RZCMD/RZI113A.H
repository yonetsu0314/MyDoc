/*======================================================================
 * File name    : rzi113a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	状態データ拡張パラメータ構造（拡張１）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi113a.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi113a.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi113a.h 1.1 1996/07/26 11:23:49 hiroki
 *Log: rzi113a.h
 * リビジョン 1.1  1996/07/26  11:23:49  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI113_H)
#define	RZI113_H
/*------------------------------------------------------*/
/*	ＡＴＡ　ＴＡＲＧＥＴ　　　　　　　　　　	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi113t {
	unsigned char	nof_ch;		/* number of channel		*/
	unsigned long	ata_target[2];	/* ata target			*/
} RZI113T;

#if	defined(STORAGE)
struct rzi113t	rzi113s;
#else
#if	!defined(NOEXTERN)
extern struct rzi113t	rzi113s;
#endif
#endif

#endif	/* RZI113A_H */
