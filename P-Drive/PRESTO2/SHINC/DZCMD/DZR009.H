/*======================================================================
 * File name    : dzr009.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	AFV 拡張パラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzr009.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/dzcmd/RCS/dzr009.h 1.3 1970/01/01 00:00:00 itami
 *Log: dzr009.h
 * リビジョン 1.3  1970/01/01  00:00:00  itami
 * 2000/07/28 セルタイプＮＬＣ＿２Ｌを追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZR009_H)
#define	DZR009_H

enum dzr0090e {				/* AFV mode			*/
	AFV_OFF,
	AFV_AUTO,
	AFV_ON,
};

enum dzr0091e {				/* スパン分銅			*/
	SPN_20G,
	SPN_40G,
	SPN_200G,
	SPN_400G,
	SPN_500G,
	SPN_1000G,
};

enum dzr0092e {				/* cell type			*/
	NLC_012L,
	NLC_04L,
	NLC_06L,
	NLC_12L,
	NLC_24L,
	NLC_02L,
};

/*------------------------------------------------------*/
/*	AFVパラメータパケット				*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzr009t {
	enum dzr0090e	afv_mode;	/* AFV mode		*/
	unsigned char	filter;		/* filter parameter	*/
	enum dzr0091e	span_wt;	/* スパン分銅		*/
	enum dzr0092e	cell_type;	/* cell type		*/
} DZR009T;

#if	defined(STORAGE)
struct dzr009t	dzr009s;
#else
#if	!defined(NOEXTERN)
extern struct dzr009t	dzr009s;
#endif
#endif

#endif
