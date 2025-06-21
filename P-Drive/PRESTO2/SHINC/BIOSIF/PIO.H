/*----------------------------------------------------------------------*
 * File name	: pio.h	/ pio bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/pio.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pio.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/pio.h 1.2 1995/08/23 11:36:43 ryoji
 *Log: pio.h
 * リビジョン 1.2  1995/08/23  11:36:43  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PIO_H)
#define	PIO_H

enum inport {
	PIN_A,
	PIN_B,
	PIN_C,
	PIN_D,
	PIN_CH_MAX,			/* 	"最大ﾎﾟｰﾄ数		*/
};

enum outport {
	POUT_A,
	POUT_B,
	POUT_C,
	POUT_D,
	POUT_CH_MAX,			/* 	"最大ﾎﾟｰﾄ数		*/
};

int 		_port_in(enum inport);
int 		_port_out(enum outport, int);

#endif
