/*======================================================================
 * File name    : rzr319.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	セクション毎のAFDデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr319.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr319.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr319.h 1.1 1997/10/30 18:02:01 hiroe
 *Log: rzr319.h
 * リビジョン 1.1  1997/10/30  18:02:01  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR319_H)
#define	RZR319_H


/*------------------------------------------------------*/
/*	セクション毎のAFD設定パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr319t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するセchのクションの数	*/
	int		afd_mode[8];	/* AFD モード			*/
} RZR319T;

#if	defined(STORAGE)
struct rzr319t	rzr319s;
#else
#if	!defined(NOEXTERN)
extern struct rzr319t	rzr319s;
#endif
#endif

#endif
