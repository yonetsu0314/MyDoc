/*----------------------------------------------------------------------*
 * File name	: fifdrv.h	/ fifo driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/fifdrv.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fifdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/14 16:30
 * 割り込みハンドラのプロトタイプ宣言追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * FIFO定義新規レビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(FIFDRV_H)
#define	FIFDRV_H

enum {
	FIFO1,				/* FIFO ch-1 number	*/
	FIFO2,				/* FIFO ch-2 number	*/
	FIFO_MAX
};

void		fif1_rxint_hdr(void);
void		fif1_txint_hdr(void);
void		fif2_rxint_hdr(void);
void		fif2_txint_hdr(void);

void		fif_int_hdr(void);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

typedef struct	fifpk {
	unsigned short	siz_txbuf;		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	unsigned short	siz_rxbuf;		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
} FIFPK;

int		fif_sys_init(int, const struct fifpk *, union aap *);

#endif	/* FIFDRV_H */

