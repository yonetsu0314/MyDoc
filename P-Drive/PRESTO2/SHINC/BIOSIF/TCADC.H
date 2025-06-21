/*----------------------------------------------------------------------*
 * File name	: tcadc.h	/ adc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/tcadc.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tcadc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/tcadc.h 1.2 1994/09/02 20:44:33 kabumoto
 *Log: tcadc.h
 * リビジョン 1.2  1994/09/02  20:44:33  kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(TCADC_H)
#define	TCADC_H

/*	<<< return parameter >>>					*/

enum {
	MASKP,				/* ﾏｽｸ用ﾋﾞｯﾄﾊﾟﾀｰﾝ		*/
	EXORP,				/* 論理反転用ﾋﾞｯﾄﾊﾟﾀｰﾝ		*/
	PTN_MAX,
};
union sgbp {
	unsigned short	us;
	unsigned char	uc[PTN_MAX];
};

typedef struct adbid {
	unsigned short	adcd;		/* adc data i/o address		*/
	unsigned short	adcs;		/* adc status i/o address	*/
	unsigned short	intl;		/* int signal i/o address	*/
	union sgbp	ptn;		/* ﾋﾞｯﾄﾊﾟﾀｰﾝ			*/
} ADBID;

ADBID *	_adc_init(int);

int	_adc_write(const void *, int, ...);
long	_adc_read(const void *);
int	_adc_stat(const void *);

#endif
