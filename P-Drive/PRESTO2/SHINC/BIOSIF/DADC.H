/*----------------------------------------------------------------------*
 * File name	: dadc.h	/ dadc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/dadc.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(DADC_H)
#define	DADC_H

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

typedef struct dadpinf {
	unsigned int	io_adr;		/* id i/o address		*/
	union sgbp	ptn;		/* ﾋﾞｯﾄﾊﾟﾀｰﾝ			*/
} DADPINF;

const DADPINF *		_dadc_init(int);

#endif	/* DADC_H */

