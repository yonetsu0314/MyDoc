/*----------------------------------------------------------------------*
 * File name	: from.h	/ flash rom bios prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/rcs/from.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: from.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * NORTi用typedef H,W,UWを使わない表現に変更
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｺﾏﾝﾄﾞ５種追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(FROM_H)
#define	FROM_H

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

typedef const unsigned char	FRPHY;	/* pointer FLASH-ROM		*/

/*	<<< argument 2 >>>						*/

typedef long			FRWRT;	/* FLASH-ROM write cycle time	*/

int 	_flashrom_init(FRPHY **, FRWRT *);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

typedef struct frioh {			/* BIOS側に公開するﾊﾟﾗﾒｰﾀ構造	*/
	char		*frbuf;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		*/
	unsigned int	szbuf;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧのｻｲｽﾞ(byte)	*/
	unsigned int	*filep;		/* ﾌｧｲﾙ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
	unsigned int	*putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
	unsigned int	*getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀへのﾎﾟｲﾝﾀ		*/
} FRIOH;

int		_flashrom_init(FRPHY  **fromphy, FRWRT  *intrvl);
int 	_flashrom_pgwr(struct frioh *);
long	_flash_write(short *adr,short data);
long	_sect_erase(unsigned long sect);
void	_set_mode(unsigned short cmd);
void	_set_fladr(unsigned long flashadr);
unsigned short		_get_polling(void);

#endif	/* FROM_H */

