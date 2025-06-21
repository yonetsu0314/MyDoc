/*======================================================================
 * File name	: prr0121.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	リレー駆動要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0121.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0121.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0121_H)
#define	PRR0121_H


/*------------------------------------------------------*/
/*	リレー駆動要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0121t {
	unsigned char	brd_no;		/* リレー基板番号		*/
	unsigned char	drive_ptn;	/* 駆動リレーパターン		*/
	unsigned char	drive_delay;	/* 駆動遅延時間[10msec]		*/
	unsigned char	drive_time;	/* 駆動時間[10msec,0=OFF,255=ON]*/
} PRR0121T;

#if	defined(STORAGE)
struct prr0121t	prr0121s;
#else
#if	!defined(NOEXTERN)
extern struct prr0121t	prr0121s;
#endif
#endif

#endif
