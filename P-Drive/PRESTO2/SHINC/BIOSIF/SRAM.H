/*----------------------------------------------------------------------*
 * File name	: sram.h	/ sram bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/sram.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sram.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/sram.h 1.1 1995/08/24 10:07:25 ryoji
 *Log: sram.h
 * リビジョン 1.1  1995/08/24  10:07:25  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(SRAM_H)
#define	SRAM_H

/*	<<< return parameter >>>					*/

typedef struct srampinf {
	void		*iobase;	/* sram physical address	*/
	unsigned short	mem_size;	/* size of memory		*/
} SRAMPINF;

const SRAMPINF *_sram_init( int );

#endif	/* SRAM_H */

