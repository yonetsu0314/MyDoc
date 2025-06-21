/*======================================================================
 * File name    : rzi041.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ応答結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi041.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi041.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi041.h 1.2 1996/12/19 09:47:12 hiroki
 *Log: rzi041.h
 * リビジョン 1.2  1996/12/19  09:47:12  hiroki
 *----------------------------------------------------------------------
 *		(c) Copy	right 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI041_H)
#define	RZI041_H


typedef struct rzi0410t {
	unsigned char	sta_flag;	/* state flag			*/
	long		weight;		/* 重量				*/
	long		count;		/* 個数 			*/
	unsigned char	ser_no;		/* シリアル番号			*/
} RZI0410T;

/*------------------------------------------------------*/
/*	シフトデータ応答結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi041t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	all_num;	/* all data num 		*/
	unsigned char	data_page;	/* data page          		*/
	unsigned char	data_num;	/* ページ内のデータ数		*/
	struct rzi0410t	data[16];	/* 各データ内容			*/
} RZI041T;

#if	defined(STORAGE)
struct rzi041t	rzi041s;
#else
#if	!defined(NOEXTERN)
extern struct rzi041t	rzi041s;
#endif
#endif

#endif
