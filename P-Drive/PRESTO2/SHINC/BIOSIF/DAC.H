/*----------------------------------------------------------------------*
 * File name	: dac.h		/ dac bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/dac.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dac.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/dac.h 1.1 1994/08/11 20:07:13 ohtani
 *Log: dac.h
 * リビジョン 1.1  1994/08/11  20:07:13  ohtani
 *----------------------------------------------------------------------*/
/*		(C) Copyright 1994, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*/
/*		PHONE (0775) 53-4141					*/
/*----------------------------------------------------------------------*/

#if	!defined(DAC_H)
#define	DAC_H

enum dacch {
	DAC_A,
	DAC_B,
	DAC_C,
	DAC_D,
};

int	_dac_wr(int, unsigned char);

#endif	/* DAC_H */

