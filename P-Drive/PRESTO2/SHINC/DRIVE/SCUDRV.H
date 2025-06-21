/*----------------------------------------------------------------------*
 * File name	: scudrv.h	/ scu driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/scudrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scudrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(SCUDRV_H)
#define	SCUDRV_H

void		scu1_errint_hdr(void);
void		scu1_rxint_hdr(void);
void		scu1_txint_hdr(void);
void		scu2_errint_hdr(void);
void		scu2_rxint_hdr(void);
void		scu2_txint_hdr(void);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

typedef struct	scupk {
	int		char_len;		/* ｷｬﾗｸﾀ長		*/
	int		stp_len;		/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int		pari_sel;		/* ﾊﾟﾘﾃｨｰ選択		*/
	unsigned short	siz_txbuf;		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	unsigned short	siz_rxbuf;		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
} SCUPK;

int		scu_sys_init(int, const struct scupk *, union aap *);

#endif	/* SCUDRV_H */

