/*======================================================================
 * File name	: prr0137.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	拡張パラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0137.h 1.3 1970/01/01 00:00:00 itami Exp $
 *$Log: prr0137.h $
 * リビジョン 1.3  1970/01/01  00:00:00  itami
 * 2007/02/28 タイミングデータサブコマンドを追加する。
 * 
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 * 2007/02/21 サブコマンド追加。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2007, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0137_H)
#define	PRR0137_H

enum prr01370e {			/* サブコマンド			*/
	SCMD_PW_DATA,			/* 単重データ			*/
	SCMD_PH_ON,			/* ＰＨ開時間			*/
	SCMD_CLEAN_START,		/* 掃除モード開始		*/
	SCMD_CLEAN_STOP,		/* 掃除モード停止		*/
	SCMD_DATA_CLR,			/* 消費量データクリア		*/
	SCMD_PLC_DATA,			/* ＰＬＣデータ			*/
	SCMD_TM_DATA,			/* タイミングデータ		*/
};

/*------------------------------------------------------*/
/*	拡張パラメータのパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0137t {
	unsigned char	ch_id;			/* channel number		*/
	enum prr01370e	sub_cmd;		/* sub command			*/
	unsigned char	nof_parm;		/* number of parm		*/
	unsigned long 	parm[40];		/* データ			*/
} PRR0137T;

#if	defined(STORAGE)
struct prr0137t	prr0137s;
#else
#if	!defined(NOEXTERN)
extern struct prr0137t	prr0137s;
#endif
#endif

#endif
