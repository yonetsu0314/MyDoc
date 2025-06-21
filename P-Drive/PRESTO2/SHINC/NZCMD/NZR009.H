/*======================================================================
 * File name    : nzr009.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	安定判定時間パラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzr009.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzr009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzr009.h 1.1 1999/06/17 10:07:07 kawamura
 *Log: nzr009.h
 * リビジョン 1.1  1999/06/17  10:07:07  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZR009_H)
#define	NZR009_H


/*------------------------------------------------------*/
/*	安定判定時間パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/


typedef struct nzr0090t {
	unsigned short	stable_time;	/* stable time constant		*/
} NZR0090T;

typedef struct nzr0091t {
	unsigned char	nof_sect;	/* number of section		*/
	struct nzr0090t	sect[8];	/* section paramter[nof_sect]	*/
} NZR0091T;

typedef struct nzr009t {
	unsigned char	nof_ch;		/* number of channel		*/
	struct nzr0091t	ch[2];		/* stable paramter[nof_ch]	*/
} NZR009T;

#if	defined(STORAGE)
struct nzr009t	nzr009s;
#else
#if	!defined(NOEXTERN)
extern struct nzr009t	nzr009s;
#endif
#endif

#endif
