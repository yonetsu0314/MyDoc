/*======================================================================
 * File name    : rzi039.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＴＡモニター状態データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi039.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi039.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi039.h 2.1 1996/07/12 18:10:02 hiroki
 *Log: rzi039.h
 * リビジョン 2.1  1996/07/12  18:10:02  hiroki
 * ATA96仕様
 *----------------------------------------------------------------------
 *		(c) Copyright 1994, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI039_H)
#define	RZI039_H

enum rzi0390e {				/* ＡＴＡイネイブル OFF/ON	*/
	ATA_ENABLE_OFF,
	ATA_ENABLE_ON,
};
enum rzi0391e {				/* 現在のＡＴＡモード		*/
	NON_ATA=1,
	OPTIMIZED_ATA,
	FAILSAFE_ATA,
};
/*------------------------------------------------------*/
/*	ＡＴＡモニター状態パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi039t {
	unsigned char		ch_id;		/* channel number	*/
	enum rzi0390e		ata_enable;	/* ATA enable        	*/
	unsigned int 		ata_timer;	/* ATAタイマー値 	*/
	enum rzi0391e		ata_mode;	/* 現在のATAﾓｰﾄﾞ     	*/
	unsigned long int 	ata_tgt;	/* ATAターゲット　	*/
	unsigned int 		ata_def_time;	/* ATA DEFAULT TIME	*/
	unsigned int 		ata_cwb;	/* CWB 			*/
} RZI039T;

#if	defined(STORAGE)
struct rzi039t	rzi039s;
#else
#if	!defined(NOEXTERN)
extern struct rzi039t	rzi039s;
#endif
#endif

#endif
