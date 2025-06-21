/*----------------------------------------------------------------------*
 * File name	: dadinv.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadinv.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadinv.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  m-yamana
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<intrpt.h>			/* show intrpt		*/
#include	<custom\custom.h>		/* show custom		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\dadc.h>			/* Show biosif		*/
#include	<drive\ndadcdrv.h>		/*	"		*/
#define		__DADINV_C__
#include	"dadctl.h"			/* Show current		*/
#include	"dadctxt.h"			/*	"		*/
#undef		__DADINV_C__

#define	RETRY	3				/* ベリファイ回数	*/

#ifdef	MAS_P5561
int
verify_5561puti(
	unsigned short	dat,
	volatile unsigned short	*p
	)
{
	unsigned char	c= (unsigned char)RETRY;

	_disable();
	do {
		*p = dat;
		if(*p == dat) {
			_enable();				return(0);
		}
	} while(--c);
	_enable();
	return(-1);
}

long
verify_5561geti(
	volatile unsigned short	*p
	)
{
	unsigned short	i;
	unsigned char	c= (unsigned char)RETRY;

	_disable();
	do {
		i = *p;
		if(i == *p) {
			_enable();
			INVERSION(i);
			return (long)i;
		}
	} while(--c);
	_enable();
	return(-1L);
}

void
inverse_dadc(char *buf, short len)
{
int i;
	for (i=0;i<len;++i) {
		*buf = (char)(~(*buf));
		++buf;
	}
	return;
}
#endif	/* MAS_P5561 */
