/*----------------------------------------------------------------------*
 * File name	: adcdrv.h	/ adc driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/adcdrv.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: adcdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/01/15  19:25
 * P5561が定義されていれば、pdadcdrv.hをインクルードする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/rcs/adcdrv.h 1.4 1996/04/03 14:02:14 chimura
 *Log: adcdrv.h
 * リビジョン 1.4  1996/04/03  14:02:14  chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(ADCDRV_H)
#define	ADCDRV_H

#define ADC_ERR		-1		/* 通信エラー			*/


void		adc0_int_hdr(void);	/* adc#0 intterupt handler	*/
void		adc1_int_hdr(void);	/* adc#1 intterupt handler	*/

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum adcid {
	ADC1,				/* ADC borad-1 ID		*/
	ADC2,				/* 	"    2  "		*/
	ADC_MAX
};
typedef enum adcid	ADCID;

int		ad_sys_init(ADCID);
/*----------------------------------------------------------------------*/

int		ad_check(ADCID, int *, int *);
int		ad_init0(ADCID, unsigned char, unsigned char,
			unsigned char, int, int);
int		ad_init1(ADCID, int, int, int, int, int);
int		ad_start(ADCID);
int		ad_stop(ADCID);
int		ad_memrd(ADCID, unsigned short, int *);
int		ad_memwr(ADCID, unsigned short, unsigned char);
int		ad_mzero(ADCID, unsigned long, unsigned long *,
			void *, void *, void *);
int		ad_azero(ADCID, int, void *, void *);
int		ad_span(ADCID, int, void *);
int		ad_spnck(ADCID, int);
int		ad_spnref(ADCID, int, void *);
int		ad_set_span(ADCID, int, unsigned short, unsigned short);
int		ad_get_span(ADCID, int, void *, void *);
int		ad_set_zero(ADCID, int, unsigned short, unsigned short);

/*----------------------------------------------------------------------*/
/*	<<< return parameter >>>       					*/
					/* 戻り値の定義			*/
#define	BOTTOM		(0x1 << 0)	/* ボトム			*/
#define	TOP		(0x1 << 1)	/* フルスケール			*/
#define	UNSTBL		(0x1 << 2)	/* 不安定			*/

int		ad_scl_read(ADCID, int, void *);
/*----------------------------------------------------------------------*/
/*	<<< argument 4 >>>     						*/
					/* ﾎﾟｲﾝﾀの示す内容はad_scl_read	*/
					/* の戻り値を参照		*/

int		ad_msc_read(ADCID, int, void *,
			    unsigned char *, short *);
/*----------------------------------------------------------------------*/
#if	defined(P5561)
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#else
#if	defined(P5422)
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#else
#if	defined(P5385)
#include	<drive\dadcdrv.h>		/* Show drive		*/
#endif
#endif
#endif

#endif	/* ADCDRV_H */
