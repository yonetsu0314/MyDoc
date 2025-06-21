/*----------------------------------------------------------------------*
 * File name	: scictl.h		/ sci control data define				*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scictl.h 1.2
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/sci/RCS/scictl.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scictl.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/24 コメント整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

int		scif_open(int ch, int mode);
int		scif_close(int ch);
int		scif_rx(int devno, unsigned char *buf, int len);
int		scif_tx(int devno, unsigned char *buf, int len);
int		scif1_io_tx(struct comh *p, int byte);
int		scif2_io_tx(struct comh *p, int byte);

#define		LF		(10)				/* New line 	   :'\n'		*/
#define		CR		(13)				/* Carriage return :'\r'		*/
#define		STX		(2)					/* Start of Text				*/
#define		ETX		(3)					/* End of Text					*/
#define		BS		(8)					/* Backspace					*/
#define		HT		(9)					/* Horizontal Tabulation		*/
#define		ENQ		(5)					/* Enquiry						*/
#define		ACK		(6)					/* Acknowledge					*/
#define		NAK		(0x15)				/* Neagtive Acknowledge			*/
#define		XON		(0x13)				/* Device Control 3				*/
#define		XOFF	(0x11)				/* Device Control 1				*/
#define		CTLC	(0x03)				/* Control C					*/
#define		CTLS	(0x13)				/* Control S					*/
#define		CTLX	(0x18)				/* Control X					*/
#define		ESCP	(0x1b)				/* Escape						*/
#define		DEL		(0x7f)				/* Delete						*/

#define		SCIF_CH1	0				/* SCIF Channel 1				*/
#define		SCIF_CH2	1				/* SCIF Channel 2				*/
#define		SCIF_CH_MAX	2				/* SCIFの最大ﾁｬﾈﾙ数				*/

#if	!defined(V_FUNC_T)
typedef	void	( *V_FUNC)();			/* voidを返す関数へのﾎﾟｲﾝﾀ		*/
#define	V_FUNC_T

typedef union	uci {					//saka: High,Low反転に注意
	short			i;					/* wordでｱｸｾｽする場合			*/
	unsigned char	uc[2];				/* [1]:low-byte [0]:high-byte	*/
} UCI;
#endif

#ifdef	STORAGE
union uci	scif_icu_inf[SCIF_CH_MAX] = {
			-1,-1						/* SCIF INTが接続されているICU	*/
		};
V_FUNC const	scif_int_hdr[SCIF_CH_MAX] = {
				sci1_rxint_hdr, sci2_rxint_hdr
};
#else
extern union uci	scif_icu_inf[];
extern V_FUNC const	scif_int_hdr[];
#endif

/*------------------------------------------------------*/
/*		送受信管理用の構造体定義						*/
/*														*/
/*------------------------------------------------------*/
typedef struct	comh {
	unsigned short	putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	lcnt;		/* ﾃｷｽﾄ･ｶｳﾝﾀｰ(受信専用)	*/
/*-------------------------------------*/
	unsigned short	flag;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧ･ﾌﾗｸﾞ		*/
#define	TX_ING	(1)				/* 送信中ﾌﾗｸﾞ			*/
#define	PENDING	(2)				/* 送信中断ﾌﾗｸﾞ			*/
/*-------------------------------------*/
	unsigned short	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧのｻｲｽﾞ		*/
	unsigned char	*buff;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ	*/
} COMH;



#ifdef	STORAGE
struct comh	scrx[] = {					/* SCIのRXﾍｯﾀﾞｰ部				*/
	{(unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (void *)0},
	{(unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (void *)0}
};
struct comh	sctx[] = {					/* SCIのTXﾍｯﾀﾞｰ部				*/
	{(unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (void *)0},
	{(unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (unsigned short)0, (void *)0}
};
int			sci_mode = -1;				/* file access mode				*/
#else

extern struct comh	scrx[];
extern struct comh	sctx[];
extern int		sci_mode;
#endif

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義							*/
/*														*/
/*------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void			*vfp[2];	/* void型farポインタ	*/
	char			*cfp[2];	/* char		"			*/
	int				*ifp[2];	/* int型	"			*/
	long			*lfp[2];	/* long型	"			*/
	unsigned int	ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]			/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp		vfp[1]			/* [1]void型farポインタ	*/
#define	cfp		cfp[1]			/* [1]char	"			*/
#define	ifp		ifp[1]			/* [1]int型	"			*/
#define	lfp		lfp[1]			/* [1]long型	"		*/
#endif

#ifdef	STORAGE
union aap 		*scif_rx_tsk[SCIF_CH_MAX] = {
				(void  *)0, (void  *)0
};
long			scu_time_out = 2000;	/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)		*/
union aap		scif_ds = { 0L };
#else
extern union aap 	*scif_rx_tsk[];
extern long			scu_time_out;
extern union aap	scif_ds;
#endif
