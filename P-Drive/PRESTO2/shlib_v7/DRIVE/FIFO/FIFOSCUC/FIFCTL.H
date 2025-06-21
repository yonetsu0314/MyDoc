/*----------------------------------------------------------------------*
 * File name	: fifctl.h	/ fifo control data define		*
 * Original	: p:/presto/shlib/drive/scu/RCS/scuctl.h 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoscuc/rcs/fifctl.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(FIFCTL_H)
#define	FIFCTL_H

int		fifo_open(int ch, int mode);
int		fifo_close(int ch);
int		fifo_rx(int ch, unsigned char *buf, int len);
int		fifo_tx(int ch, unsigned char *buf, int len);
int		fif_io_tx(int ch, struct comh *p, int byte);
int		init_fif_rx(int ch);
int		init_fif_tx(int ch);


#define	FIF_CH_MAX	(2)
#define	LF		(10)		/* New line 	   :'\n'	*/
#define	CR		(13)		/* Carriage return :'\r'	*/

#if	!defined(V_FUNC_T)
typedef	void	(*V_FUNC)();		/* voidを返す関数へのﾎﾟｲﾝﾀ	*/
#define	V_FUNC_T

typedef union	uci {
	int		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCI;
#endif

typedef struct	ucsinf {
	short		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCSINF;

#ifdef	STORAGE
struct ucsinf	fif_icu_inf[FIF_CH_MAX] = {/* FIFO INTが接続されているICU*/
			(short)-1, (unsigned char)-1, (unsigned char)-1,
			(short)-1, (unsigned char)-1, (unsigned char)-1 };

//V_FUNC const	fif_int_hdr[FIF_CH_MAX] = { fif1_rxint_hdr,
//					     fif2_rxint_hdr };
unsigned short	fpgamskflg = 0;

#else
extern union uci	fif_icu_inf[];
//extern V_FUNC const	fif_int_hdr[];
extern unsigned short	fpgamskflg;
#endif

/*------------------------------------------------------*/
/*	送受信管理用の構造体定義			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	comh {
	unsigned short	putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	lcnt;		/* ﾃｷｽﾄ･ｶｳﾝﾀｰ(受信専用)		*/
/*-------------------------------------*/
	unsigned short	flag;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧ･ﾌﾗｸﾞ		*/
#define	TX_ING	(1)			/* 送信中ﾌﾗｸﾞ			*/
#define	PENDING	(2)			/* 送信中断ﾌﾗｸﾞ			*/
#define	RXPEND	(4)			/* 受信中断ﾌﾗｸﾞ			*/
/*-------------------------------------*/
	unsigned short	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧのｻｲｽﾞ		*/
	unsigned char	*buff;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ		*/
} COMH;


#ifdef	STORAGE
int		fif_mode = -1;		/* file access mode		*/
struct comh	fifrx[FIF_CH_MAX] = {	/* SCUのRXﾍｯﾀﾞｰ部		*/
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(void *)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(void *)0,
		};
struct comh	fiftx[FIF_CH_MAX] = {	/* SCUのTXﾍｯﾀﾞｰ部		*/
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(void *)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(unsigned short)0,
					(void *)0,
		};
#else
extern int		fif_mode;
extern struct comh	fifrx[];
extern struct comh	fiftx[];
#endif


/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void 			*vfp[2];	/* void型farポインタ	*/
	char	 		*cfp[2];	/* char		"	*/
	int			*ifp[2];	/* int型	"	*/
	long			*lfp[2];	/* long型	"	*/
	unsigned int		ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]				/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp	vfpp[1]				/* [1]void型farポインタ	*/
#define	cfp	cfpp[1]				/* [1]char	"	*/
#define	ifp	ifpp[1]				/* [1]int型	"	*/
#define	lfp	lfpp[1]				/* [1]long型	"	*/
#endif

#ifdef	STORAGE
union aap		*fif_rx_tsk[FIF_CH_MAX] = {
							(void *)0,
							(void *)0
						};
long			fif_time_out = 2000L;
union aap		fif_ds = { 0L, 0L };

#else
extern union aap	*fif_rx_tsk[];
extern long		fif_time_out;
extern union aap	fif_ds;
#endif

#endif	/* FIFCTL_H */
