/*----------------------------------------------------------------------*
 * File name	: pioctxt.h	/ pio context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ppi/RCS/pioctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pioctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

enum {
	MASKP,				/* ﾏｽｸ用ﾋﾞｯﾄﾊﾟﾀｰﾝ		*/
	EXORP,				/* 論理反転用ﾋﾞｯﾄﾊﾟﾀｰﾝ		*/
	PTN_MAX,
};
union sgbp {
	unsigned short	us;
	unsigned char	uc[PTN_MAX];
};

/*------------------------------------------------------*/
/*	PIOのコンテキスト構造体				*/
/*							*/
/*------------------------------------------------------*/

typedef struct	pio_ctxt {
	unsigned int	ipfw;		/* ICU ipfw I/O address		*/
	unsigned int	level;		/* ﾚﾍﾞﾙ入力用ﾎﾟｰﾄ･ｱﾄﾞﾚｽ		*/
	union sgbp	ptn;		/* ﾋﾞｯﾄﾊﾟﾀｰﾝ			*/
} DPM_CTXT;

#ifdef	STORAGE
struct pio_ctxt	pio_ctxt_tbl[EXSIG_MAX]= {
			(unsigned int)0, (unsigned int)0, (unsigned short)0
};
#else
extern struct pio_ctxt	pio_ctxt_tbl[];
#endif
