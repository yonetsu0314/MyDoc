/*----------------------------------------------------------------------*
 * File name	: adc.h		/ adc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/adc.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: adc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(ADC_H)
#define	ADC_H

/*	<<< argument 3 >>>						*/

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
	unsigned short	ipfw;		/* ICU ipfw I/O address		*/
	unsigned short	io_adr[2];	/* id i/o address		*/
	union sgbp	ptn[2];		/* ﾋﾞｯﾄﾊﾟﾀｰﾝ			*/
} ADBID;

int 	_adc_init(int, int, struct adbid *, const void *);

int 	_adc_write(const void *, int, ...);
int 	_adc_read(const void *);
int 	_adc_stat(const void *);
int 	_adc_adrd(const void *, int, void *,
			  unsigned char *, short *);

#endif	/* ADC_H */

