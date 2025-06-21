/*----------------------------------------------------------------------*
 * File name	: mpscdrv.h	/ mpsc driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/mpscdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mpscdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(MPSCDRV_H)
#define	MPSCDRV_H

#ifdef	P5280
enum {
	MPSC0,				/* MPSC dev#0 ch-A number	*/
	MPSC1,				/* MPSC dev#0 ch-B number	*/
	MPSC2,				/* MPSC dev#1 ch-A number	*/
	MPSC3,				/* MPSC dev#1 ch-B number	*/
	MPSC_MAX
};
#else
enum {
	MPSC0,				/* MPSC dev#0 ch-A number	*/
	MPSC1,				/* MPSC dev#0 ch-B number	*/
	MPSC_MAX
};
#endif

void		mpsc1_int_hdr(void);	/* MPSC #1 interrupt handler	*/
void		mpsc0_int_hdr(void);	/* MPSC #0 interrupt handler	*/

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

typedef struct	mpspk {
	int		char_len;		/* ｷｬﾗｸﾀ長		*/
	int		stp_len;		/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int		pari_sel;		/* ﾊﾟﾘﾃｨｰ選択		*/
	int		baud;			/* 転送ｽﾋﾟｰﾄﾞ		*/
	unsigned short	siz_txbuf;		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	unsigned short	siz_rxbuf;		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
} MPSPK;

int		mpsc_sys_init(int, const struct mpspk *, union aap *);
/*----------------------------------------------------------------------*/

#endif	/* MPSCDRV_H */

