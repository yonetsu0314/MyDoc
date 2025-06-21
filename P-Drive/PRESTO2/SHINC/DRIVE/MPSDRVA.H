/*----------------------------------------------------------------------*
 * File name	: mpscdrva.h	/ mpsc driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/mpsdrva.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mpsdrva.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/RCS/mpsdrva.h 1.4 1970/01/01 00:00:00 itami
 *Log: mpsdrva.h
 * リビジョン 1.4  1970/01/01  00:00:00  itami
 * 2000/07/19 追加受信ﾊﾞｲﾄ数設定関数追加
 *----------------------------------------------------------------------*/
/*	(c) Copyright 1988-1998, ISHIDA SCALES MFG. CO., LTD.		*/
/*	959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN				*/
/*	SHIGA JAPAN							*/
/*	(077) 553-4141							*/
/*----------------------------------------------------------------------*/

#if	!defined(MPSDRVA_H)
#define	MPSDRVA_H

#ifdef	P5280
enum {
	MPSC0,				/* MPSC dev#0 ch-A number	*/
	MPSC1,				/* MPSC dev#0 ch-B number	*/
	MPSC2,				/* MPSC dev#1 ch-A number	*/
	MPSC3,				/* MPSC dev#1 ch-B number	*/
	MPSC_MAX
};
#else
#ifdef	P5431
enum {
	MPSC0,				/* MPSC dev#0 ch-A number	*/
	MPSC1,				/* MPSC dev#0 ch-B number	*/
	MPSC2,				/* MPSC dev#1 ch-A number	*/
	MPSC3,				/* MPSC dev#1 ch-B number	*/
	MPSC4,				/* MPSC dev#2 ch-A number	*/
	MPSC5,				/* MPSC dev#3 ch-B number	*/
	MPSC_MAX
};
#else
enum {
	MPSC0,				/* MPSC dev#0 ch-A number	*/
	MPSC1,				/* MPSC dev#0 ch-B number	*/
	MPSC_MAX
};
#endif
#endif

void		mpsc2_int_hdr(void);	/* MPSC #2 interrupt handler	*/
void		mpsc1_int_hdr(void);	/* MPSC #1 interrupt handler	*/
void		mpsc0_int_hdr(void);	/* MPSC #0 interrupt handler	*/

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

enum {
	HALF_DUPLEX, FULL_DUPLEX
};
enum {
	MODEM_CTRL_OFF, MODEM_CTRL_ON	/* full duplex の時有効 */
};

typedef struct	mpspk {
	int		char_len;		/* ｷｬﾗｸﾀ長		*/
	int		stp_len;		/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int		pari_sel;		/* ﾊﾟﾘﾃｨｰ選択		*/
	int		baud;			/* 転送ｽﾋﾟｰﾄﾞ		*/
	unsigned short	siz_txbuf;		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	unsigned short	siz_rxbuf;		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	int		duplex;
	int		modem_ctrl;
} MPSPK;

int		mpsc_sys_init(int, const struct mpspk *, union aap *);

/*----------------------------------------------------------------------*/
/*	以下は、dmps3.lib をリンクした時のみ有効			*/
/*----------------------------------------------------------------------*/
int mpsc_dcd_rd(int channel);
int mpsc_cts_rd(int channel);
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*	以下は、dmps4.lib をリンクした時のみ有効			*/
/*----------------------------------------------------------------------*/
int set_wup_char(int ch,unsigned char *p);
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*	以下は、dmps5.lib をリンクした時のみ有効			*/
/*----------------------------------------------------------------------*/
int set_addc(int channel, int cnt);
/*----------------------------------------------------------------------*/

#endif	/* MPSDRVA_H */

