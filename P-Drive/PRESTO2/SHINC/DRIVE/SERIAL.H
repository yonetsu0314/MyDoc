/*----------------------------------------------------------------------*
 * File name	: serial.h	/ serial i/o header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/serial.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: serial.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 57KBPS,115KBPSのENUM追加
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


#if	!defined(SERIAL_H)
#define	SERIAL_H

/*------------------------------------------------------*/
/*	転送スピードの定数定義				*/
/*							*/
/*------------------------------------------------------*/

enum {
	C1200BPS,			/* 1200 bps			*/
	C2400BPS,			/* 2400 bps			*/
	C4800BPS,			/* 4800 bps			*/
	C9600BPS,			/* 9600 bps			*/
	C19KBPS,			/* 19.2K bps			*/
	C38KBPS,			/* 38.4K bps			*/
	C76KBPS,			/* 76.8K bps			*/
	C57KBPS = C76KBPS,		/* 57.6K bps			*/
	C153KBPS,			/* 153.6K bps			*/
	C115KBPS = C153KBPS,		/* 115.2K bps			*/
	C307KBPS			/* 307.2K bps			*/
};

/*------------------------------------------------------*/
/*	キャラクタ長の定数定義				*/
/*							*/
/*------------------------------------------------------*/

enum {
	C7BITLEN,			/* 7ﾋﾞｯﾄ長			*/
	C8BITLEN			/* 8ﾋﾞｯﾄ長			*/
};

/*------------------------------------------------------*/
/*	ストップ･ビットの定数定義			*/
/*							*/
/*------------------------------------------------------*/

enum {
	C1STPBIT,			/* 1ｽﾄｯﾌﾟ･ﾋﾞｯﾄ			*/
	C2STPBIT			/* 2ｽﾄｯﾌﾟ･ﾋﾞｯﾄ			*/
};

/*------------------------------------------------------*/
/*	パリティ･ビット選択時の定数定義			*/
/*							*/
/*------------------------------------------------------*/

enum {
	NO_PARI,			/* ﾊﾟﾘﾃｨ無し			*/
	ODD_PARI,			/* 奇数ﾊﾟﾘﾃｨ			*/
	EVEN_PARI			/* 偶数ﾊﾟﾘﾃｨ			*/
};

#endif	/* SERIAL_H */

