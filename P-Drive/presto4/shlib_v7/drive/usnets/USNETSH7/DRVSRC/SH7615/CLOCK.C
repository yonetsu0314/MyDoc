/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/clock.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: clock.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


#include "..\sh7615.h"

/* タイマ割り込み用のベクタ番号と優先順位。適当に設定すべし。 */
#define TPU2_VECTNO	77	/* Vector no. for TPU2 TGIA */
#define TPU2_PRIOR	14	/* Priority for TPU2 TGIA */

extern unsigned int clocks_per_sec;

unsigned long timercount;	/* timer count */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Nclkinit()
**
**  Clock initialization.  Called from macro Ninit() in net.c through
**  macro LOCALSETUP() defined in local.h.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* TPU2を使用して１０ミリ秒周期の割り込みを発生させる。 */
int Nclkinit()
{
    timercount = 0;
	clocks_per_sec = 100;

    return 0;
}

/* 現在のタイマ値を取得する */
unsigned long Nclock(void)
{
    return timercount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Nclkterm()
**
**  Terminate clock by stopping timer
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Nclkterm(void)
{
}

