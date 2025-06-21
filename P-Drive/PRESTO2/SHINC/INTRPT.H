/*----------------------------------------------------------------------*
 * File name	: intrpt.h						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/RCS/intrpt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: intrpt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/*
m-yamana
このファイル名 INTRPT.H は暫定的です。
とりあえずこの名前で先にコンパイルを通してしまいます。2002/12/18
*/

#if	!defined(INTRPT_H)
#define	INTRPT_H

#include <machine.h>

/* function prototypes */

#ifndef		_disable
#define		_disable( x )	local_disable( x )
void		local_disable( void );
#endif

#ifndef		_enable
#define		_enable( x )	local_enable( x )
void		local_enable( void );
#endif


#ifndef		disable_interrupt
#define		disable_interrupt()	set_imask(15)
#endif

#ifndef		enable_interrupt
#define		enable_interrupt()	set_imask(0)
#endif

#endif	/* INTRPT_H */

