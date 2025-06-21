/*----------------------------------------------------------------------*
 * File name	: e2rom.h	/ e2rom bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/e2rom.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2rom.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(E2ROM_H)
#define	E2ROM_H

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

typedef const unsigned char	E2PHY;	/* pointer E2-ROM		*/

/*	<<< argument 2 >>>						*/

typedef long			E2WRT;	/* E2-ROM write cycle time	*/

int 	_e2rom_init(E2PHY **, E2WRT *);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

typedef struct e2ioh {			/* BIOS側に公開するﾊﾟﾗﾒｰﾀ構造	*/
	char		*e2buf;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		*/
	unsigned int	szbuf;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧのｻｲｽﾞ(byte)	*/
	unsigned int	*filep;		/* ﾌｧｲﾙ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
	unsigned int	*putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
	unsigned int	*getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
} E2IOH;

int 	_e2rom_pgwr(struct e2ioh *);

#endif	/* E2ROM_H */

