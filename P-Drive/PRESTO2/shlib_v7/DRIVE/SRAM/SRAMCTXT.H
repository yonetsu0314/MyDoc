/*----------------------------------------------------------------------*
 * File name	: sramctxt.h	/ sram context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramctxt.h 1.1 1995/08/24 13:23:44 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(SRAMCTXT_H)
#define	SRAMCTXT_H

/*------------------------------------------------------*/
/*	SRAMのコンテキスト構造体			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	sram_ctxt {
	unsigned char		*iobase;	/* SRAMのﾍﾞｰｽ物理ｱﾄﾞﾚｽ	*/
	unsigned short		size;		/*	"のｻｲｽﾞ(byte数)*/
	unsigned short		offset;		/* ﾃｷｽﾄ読み込みｴﾘｱへのｵﾌｾｯﾄ	*/
} SRAM_CTXT;

#ifdef	STORAGE
struct sram_ctxt	sram_ctxt_tbl[]= {
		(unsigned char *)0, (unsigned short)0, (unsigned short)0,
		(unsigned char *)0, (unsigned short)0, (unsigned short)0
};
#else
extern struct sram_ctxt	sram_ctxt_tbl[];
#endif

#endif	/* SRAMCTXT_H */
