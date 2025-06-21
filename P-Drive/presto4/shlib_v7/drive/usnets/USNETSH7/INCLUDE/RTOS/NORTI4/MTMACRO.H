/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/mtmacro.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: mtmacro.h $
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
    MTMACRO.H -- multitasking macros for USNET
    Copyright (C) 1993, 1995
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This version is for Hitachi HI-SH7.
*/
#include <umachine.h>
#include "itron.h"
#include "kernel.h"
#include "driver.h"

#ifndef RX_DEF_DRIVER
#include "drive\rxdefd.h"
#define RX_DEF_DRIVER
#endif


#define MT 2		/* 0 = no multitasking */
			/* 1 = co-operative multitasking */
			/* 2 = pre-emptive multitasking */

/* Priority for a server, a client and the network task.  The last must not
   be higher than either of the first two.  For ITRON, high number = low
   priority.
*/
#define SERV_PRIOR	ETHER_SERVTSK_PRI
#define CLIENT_PRIOR	ETHER_SERVTSK_PRI
#define NET_PRIOR	ETHER_NETTSK_PRI

#define YIELD()

#define TASKFUNCTION TASK

/* Available task id numbers for usnet. Only vaild when RunTask() is */
/* configured to use static stack. See multi.c. */
/*#define MIN_TSK_ID   1*/
/*#define MAX_TSK_ID   5*/

/* Prototype for RunTask() */
ER RunTask(FP task, PRI prior);          /* create and run a task */

/* create and run a task */
/*#define RUNTASK(task, prior) RunTask((TASKP)task, prior)*/
#define RUNTASK(task, prior) RunTask((FP)task, prior)
#define RUNTASK0(task, prior) RunTask0((FP)task, prior)
#define RUNTASK1(task, prior) RunTask1((FP)task, prior)
#define RUNTASK2(task, prior) RunTask2((FP)task, prior)
#define RUNTASK3(task, prior) RunTask3((FP)task, prior)
#define RUNTASK4(task, prior) RunTask4((FP)task, prior)

#define KILLTASK() exd_tsk()             /* kill this task */

/* Base of signal number. Not a HI77xx's event flag id. Must be zero. */
#define EVBASE 0

/* Base of event flag id for usnet. Usnet uses event flag id from FLGBASE */
/* to (2*(NCONNS+NNETS)+2)/32+FLGBASE. These event flags must be created. */
/* #define FLGBASE ETHER_EVF_ID	*/
#define FLGBASE(no) (((no)/32)?ETHER_EVF_EX_ID:ETHER_EVF_ID)
/* #define FLGBASE(no) (no/32+ETHER_EVF_EX0_ID) */

ER waitmorefunc( ID id, FLGPTN ptn );
/* #define WAITNOMORE(signo)    set_flg(signo/32+FLGBASE,0x0001<<(signo%32)) */
#define WAITNOMORE(signo)    set_flg(FLGBASE(signo),0x0001<<(signo%32))
/* #define WAITNOMORE_IR(signo) set_flg(signo/32+FLGBASE,0x0001<<(signo%32)) */
#define WAITNOMORE_IR(signo) set_flg(FLGBASE(signo),0x0001<<(signo%32))

extern unsigned int clocks_per_sec;
/*unsigned long   TimeMS(void);*/


#if 0
#define WAITFOR(condition, signo, msec, flag) \
{ unsigned long wwul1; \
    for (flag=0,wwul1=TimeMS(); !(condition); ) { \
	if (TimeMS()-wwul1 >= msec) { \
	    flag = ETIMEDOUT; \
	    break; \
	} \
	YIELD(); \
}   }
#else
#undef WAITFOR

ER waitforfunc( ID id, FLGPTN ptn, int mode, UINT *ptr, TMO t, int flg );
void flgerr( ID id, FLGPTN ptn, int mode, UINT *ptr, TMO t,ER ercd );
#define WAITFOR(condition, signo, msec, flag) \
{ \
  unsigned long t0,t; \
  UINT fw; \
  ER ercd; \
  for (flag=0,t0=TimeMS(); !(condition); ) { \
    if ((t = TimeMS()-t0) >= msec) { \
      flag = ETIMEDOUT; \
      break; \
    } \
    t = (msec*clocks_per_sec)/1000; \
    /* ercd = twai_flg(signo/32+FLGBASE,0x0001<<(signo%32),TWF_ANDW,&fw,t); */ \
    ercd = twai_flg(FLGBASE(signo),0x0001<<(signo%32),TWF_ANDW,&fw,t); \
    if( ercd == E_OK ) {\
		/* clr_flg(signo/32+FLGBASE,~(0x0001<<(signo%32))); */ \
		clr_flg(FLGBASE(signo),~(0x0001<<(signo%32))); \
  	}\
  } \
}
#endif
/*  flgerr( signo/32+FLGBASE,0x0001<<(signo%32),TWF_ANDW,&fw,t,ercd );\*/


/* Prevent and restore preempting.  Null if no preempting is used. */
void            Ndisable(void);
void            Nenable(void);
#if 1
#define BLOCKPREE()  DISABLE();\
					dis_dsp();
					
#define RESUMEPREE() ena_dsp();\
					ENABLE();
#else
#define BLOCKPREE()  dis_dsp();
					
#define RESUMEPREE() ena_dsp();
#endif
