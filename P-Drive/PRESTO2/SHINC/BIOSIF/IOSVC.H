/*----------------------------------------------------------------------*
 * File name	: iosvc.h	/ bios svc header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/rcs/iosvc.h 1.7 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: iosvc.h $
 * リビジョン 1.7  1970/01/01  00:00:00  sakaguti
 * 2004/12/11 CFのベクター番号追加
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * 2004/12/7 FPGAFIFOのベクター番号追加
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2004/10/26 E2SROMのベクター番号追加
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/04/30 15:00
 * biosにおけるセクション初期化、レジスタ初期化用ベクター番号を
 * 追加する。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ＳＨ２の場合softintは0x20から始まる
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/02/06 19:20
 * ベクター番号を変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/iosvc.h 1.5 1995/08/25 09:26:19 ryoji
 *Log: iosvc.h
 * リビジョン 1.5  1995/08/25  09:26:19  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(IOSVC_H)
#define	IOSVC_H

enum {
#ifdef CPUSH2
	VECTOR_SCU	= 0x20,		/* SCU int vector number	*/
#else
	VECTOR_SCU	= 0x80,		/* SCU int vector number	*/
#endif
	VECTOR_TCU,			/* TCU int vector number	*/
	VECTOR_MPSC,			/* MPSC int vector number	*/
	VECTOR_PIO,			/* PIO int vector number	*/
	VECTOR_ADC,			/* ADC int vector number	*/
	VECTOR_DADC	= VECTOR_ADC,	/* DADC int vector number	*/
	VECTOR_ICU,			/* ICU int vector number	*/
	VECTOR_E2ROM,			/* E2ROM int vector number	*/
	VECTOR_DPRAM,			/* Dual Port RAM int vector no.	*/
	VECTOR_CLK,			/* CLOCK int vector number	*/
	VECTOR_XAM,			/* Extended Addr int vector no.	*/
	VECTOR_SRAM,			/* SRAM int vector no.		*/
	VECTOR_FROM,			/* FLASH ROM int vector number	*/
	VECTOR_ARCNET,			/* ARCNET int vector number	*/
	VECTOR_I2NET,			/* I2NET int vector number	*/
	VECTOR_DSP,			/* DSP int vector number	*/
	VECTOR_GDC,			/* GDC int vector number	*/
	VECTOR_DAC,			/* D/A con. int vector number	*/
	VECTOR_CTC,			/* Cnt&Tmr int vector number	*/
	VEC_NOUSE1,			/* 一般使用不可			*/
	VECTOR_VFD,			/* VFD int vector no.		*/
	VECTOR_INIT,			/* INIT bios int vector no.	*/
	VECTOR_E2SROM,			/* E2SROM int vector number	*/
	VECTOR_FPGAFIFO,			/* FPGAFIFO int vector number	*/
	VECTOR_CF,			/* CF int vector number	*/
};

#endif	/* IOSVC_H */

