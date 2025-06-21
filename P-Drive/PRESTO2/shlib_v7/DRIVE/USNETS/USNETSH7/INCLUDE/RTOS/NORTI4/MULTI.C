/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/multi.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: multi.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/*
    MULTI.C -- multitasking support for USNET
    Copyright (C) 1993, 1995, 1997
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This version is for Hitachi HI-SH77.
*/

#include "net.h"
#include "local.h"
#include "support.h"
#include "itron.h"
#include "kernel.h"

/* Create and start task usind dynamic stack. */
/* Stack size is fixed to 1024 bytes for NetTask. */


static char nadr[]="USNETTASK";

ER RunTask(FP task, PRI prior)
{
    ER ercd;
    ID tskid;
	T_CTSK tskinf;
#ifndef V3
    tskinf.exinf = nadr;
    tskinf.tskatr = TA_HLNG;
    tskinf.task = task;
	tskinf.itskpri = prior;
    tskinf.stksz = 0x3000;
    tskinf.stk = NULL;
    tskid = acre_tsk(&tskinf);
#else
    tskinf.exinf = NULL;
    tskinf.tskatr = TA_HLNG;
    tskinf.task = task;
	tskinf.itskpri = prior;
    tskinf.stksz = 0x3000;
    tskid = vcre_tsk(&tskinf);
#endif
    if (tskid < 0 ) {
	    return tskid;
	}
    ercd = sta_tsk(tskid,0);
    return ercd;

}

void UsnetKillTask( char maxid )
{
	T_RTSK rtsk;
	ID id;
	ER err;

	for( id=1;id<=maxid;id++ ) {
		err = ref_tsk(id, &rtsk);
		if( err == E_OK && rtsk.exinf == nadr ) {
			while( rtsk.tskstat != TTS_WAI ) {
				dly_tsk(25);
				err = ref_tsk(id, &rtsk);
			}
			ter_tsk(id);
			del_tsk(id);
		}
	}
}

ER waitmorefunc( ID id, FLGPTN ptn ) {
	set_flg(id,ptn);
#if NTRACE >= 1
/*	Nprintf("set_flg(id=%x,ptn=%x)",id,ptn );*/
#endif
}
