/*======================================================================
 * File name	: pri0039.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0039.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0039.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi039.h 2.1 1996/07/12 18:10:02 hiroki Exp
 *log: rzi039.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0039_H)
#define	PRI0039_H

enum pri00390e {				/* ＡＴＡイネイブル OFF/ON	*/
	ATA_ENABLE_OFF,
	ATA_ENABLE_ON,
};
enum pri00391e {				/* 現在のＡＴＡモード		*/
	NON_ATA=1,
	OPTIMIZED_ATA,
	FAILSAFE_ATA,
};
/*------------------------------------------------------*/
/*	ＡＴＡモニター状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0039t {
	unsigned char	ch_id;		/* channel number	*/
	enum pri00390e	ata_enable;	/* ATA enable		*/
	unsigned short	ata_timer;	/* ATAタイマー値	*/
	enum pri00391e	ata_mode;	/* 現在のATAﾓｰﾄﾞ	*/
	unsigned long	ata_tgt;	/* ATAターゲット	*/
	unsigned short	ata_def_time;	/* ATA DEFAULT TIME	*/
	unsigned short	ata_cwb;	/* CWB			*/
} PRI0039T;

#if	defined(STORAGE)
struct pri0039t	pri0039s;
#else
#if	!defined(NOEXTERN)
extern struct pri0039t	pri0039s;
#endif
#endif

#endif
