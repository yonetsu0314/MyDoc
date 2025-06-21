/*======================================================================
 * File name    : rzr259.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張親ヘッドデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr259.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr259.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr259.h 1.2 1999/05/06 10:09:47 itami
 *Log: rzr259.h
 * リビジョン 1.2  1999/05/06  10:09:47  itami
 * WH-BH非同期供給設定追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR259_H)
#define	RZR259_H

enum rzr2590e {				/* WH->BH非同期供給する／しない	*/
	FD_WHBH_OFF,			/* しない			*/
	FD_WHBH_ON			/* する				*/
};

typedef struct rzr2590t {
	enum rzr2590e	fd_whbh;	/* WH->BH非同期供給する／しない		*/
	unsigned char	phfd_on;	/* PH-供給信号オン時間			*/
	unsigned char	ph_cv;		/* 親供給機への供給コンベア遅れ時間	*/
	unsigned char	phcv_on;	/* 親投入機への供給コンベアオン時間	*/
} RZR2590T;

typedef struct rzr2591t {
	unsigned char	nof_phead;	/* number of parent head	*/
	unsigned char	only_parent;	/* 親のみ計算する／しない	*/
	struct rzr2590t	phead[2];	/* PI2590 paramter[nof_phead]	*/
} RZR2591T;

/*------------------------------------------------------*/
/*	拡張親ヘッドデータのパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr259t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr2591t	sect[8];	/* PR2591 paramter[nof_sect]	*/
} RZR259T;

#if	defined(STORAGE)
struct rzr259t	rzr259s;
#else
#if	!defined(NOEXTERN)
extern struct rzr259t	rzr259s;
#endif
#endif

#endif
