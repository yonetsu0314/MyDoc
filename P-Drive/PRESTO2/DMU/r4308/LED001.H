/*======================================================================
 * File name    : led001.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto/ips/src/rcs/led001.h 1.1 1970/01/01 00:00:00 hiroe Exp $
 *$Log: led001.h $
 * リビジョン 1.1  1970/01/01  00:00:00  hiroe
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef		LED001_H
#define		LED001_H

int		setup_led_ctrl(void);

#ifdef	STORAGE
union aap	ledctrl_tsk;
#else
extern union aap	ledctrl_tsk;
#endif


#endif
