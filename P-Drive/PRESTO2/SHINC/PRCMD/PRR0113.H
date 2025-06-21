/*======================================================================
 * File name	: prr0113.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給機制御信号ＯＮ／ＯＦＦデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0113.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0113.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0113_H)
#define	PRR0113_H


enum prr01130e {			/* 供給機制御信号 ON/OFF	*/
	INFEED_OFF,			/* 供給機制御信号 OFF		*/
	INFEED_ON,			/* 供給機制御信号 ON		*/
};

/*------------------------------------------------------*/
/*	供給機制御信号ＯＮ／ＯＦＦパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/


typedef struct prr0113t {
	unsigned char	ch_id;		/* channel number		*/
	enum prr01130e	onoff_key;	/* ON/OFF			*/
} PRR0113T;

#if	defined(STORAGE)
struct prr0113t	prr0113s;
#else
#if	!defined(NOEXTERN)
extern struct prr0113t	prr0113s;
#endif
#endif

#endif
