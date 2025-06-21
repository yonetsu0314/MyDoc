/*----------------------------------------------------------------------*
 * File name	: ppia.h	/ pio bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/ppia.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: ppia.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/ppia.h 1.1 1994/08/12 14:04:52 kabumoto
 *Log: ppia.h
 * リビジョン 1.1  1994/08/12  14:04:52  kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PPI_H)
#define	PPI_H

enum pioport {
	PORT_A,				/* i8255 port-A			*/
	PORT_B,				/*	"     B			*/
	PORT_C,				/*	"     C			*/
	PIO_CH_MAX,			/* 	"最大ﾎﾟｰﾄ数		*/
};

enum pio_devch {
	PIO0,
	PIO1,
	PIO2,
	PIO3,
	PIO_DEV_MAX,			/* BIOSで管理する最大ﾃﾞﾊﾞｲｽ数	*/
};


int  	_pio_init(enum pio_devch);
int  	_pio_wr(enum pio_devch, enum pioport, unsigned char);
int 		_pio_rd(enum pio_devch, enum pioport);
int  	_pio_bctl(enum pio_devch, int, int);

/*----------------------------------------------------------------------*/
/*	<<< return parameter >>>					*/

typedef struct pioinf {
	unsigned short	level_port;	/* 割り込みのﾚﾍﾞﾙ入力ﾎﾟｰﾄ	*/
	unsigned short	xor_mask_bp;	/* 論理反転&ﾏｽｸ･ﾋﾞｯﾄﾊﾟﾀｰﾝ	*/
} PIOINF;

const PIOINF  * _pio_icu_port(enum pio_devch, int);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum piosr {
	PIO_RES,
	PIO_SET,
};

int 		_pio175_wr(enum piosr, int);

#endif
