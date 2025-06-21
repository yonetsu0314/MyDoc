/*----------------------------------------------------------------------*
 * File name	: scuctl.h	/ scu control data define					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scuctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scuctl.h 1.1 1970/01/01
 *Log: scuctl.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/

#if	!defined(SCUCTL_H)
#define	SCUCTL_H

int		m66230_open(int ch, int mode);
int		m66230_close(int ch);
int		m66230_rx(int ch, unsigned char *buf, int len);
int		m66230_tx(int ch, unsigned char *buf, int len);
int		m66230_io_tx(int ch, struct comh *p, int byte);
int		init_m66230_rx(int ch);
int		init_m66230_tx(int ch);


#define	M66230_CH_MAX	(2)
#define	LF				(10)		/* New line 	   :'\n'			*/
#define	CR				(13)		/* Carriage return :'\r'			*/

#if	!defined(V_FUNC_T)
typedef	void	(*V_FUNC)();		/* voidを返す関数へのﾎﾟｲﾝﾀ			*/
#define	V_FUNC_T

typedef union	uci {
	int				i;				/* wordでｱｸｾｽする場合				*/
	unsigned char	uc[2];			/* [0]:low-byte [1]:high-byte		*/
} UCI;
#endif

typedef struct	ucsinf {
	short			i;				/* wordでｱｸｾｽする場合				*/
	unsigned char	uc[2];			/* [0]:low-byte [1]:high-byte		*/
} UCSINF;

/*------------------------------------------------------*/
/*	送受信管理用の構造体定義							*/
/*														*/
/*------------------------------------------------------*/

typedef struct	comh {
	unsigned short	putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀ			*/
	unsigned short	lcnt;		/* ﾃｷｽﾄ･ｶｳﾝﾀｰ(受信専用)	*/
/*-------------------------------------*/
	unsigned short	flag;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧ･ﾌﾗｸﾞ		*/
#define	TX_ING		(1)			/* 送信中ﾌﾗｸﾞ			*/
#define	PENDING		(2)			/* 送信中断ﾌﾗｸﾞ			*/
/*-------------------------------------*/
	unsigned short	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧのｻｲｽﾞ		*/
	unsigned char	*buff;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ	*/
} COMH;


#ifdef	STORAGE
int			m66230_mode = -1;			/* file access mode	*/
struct comh	m66230rx[M66230_CH_MAX] = {	/* SCUのRXﾍｯﾀﾞｰ部	*/
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
struct comh	m66230tx[M66230_CH_MAX] = {	/* SCUのTXﾍｯﾀﾞｰ部	*/
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
extern int			m66230_mode;
extern struct comh	m66230rx[];
extern struct comh	m66230tx[];
#endif


/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義							*/
/*														*/
/*------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void 			*vfp[2];	/* void型farポインタ	*/
	char	 		*cfp[2];	/* char		"			*/
	int				*ifp[2];	/* int型	"			*/
	long			*lfp[2];	/* long型	"			*/
	unsigned int	ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr		ary[1]		/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp			vfpp[1]		/* [1]void型farポインタ	*/
#define	cfp			cfpp[1]		/* [1]char	"			*/
#define	ifp			ifpp[1]		/* [1]int型	"			*/
#define	lfp			lfpp[1]		/* [1]long型	"		*/
#endif

#ifdef	STORAGE
union aap		*m66230_rx_tsk[M66230_CH_MAX] = {
							(void *)0,
							(void *)0
						};
long			m66230_time_out = 2000L;
union aap		m66230_ds = { 0L, 0L };

#else
extern union aap	*m66230_rx_tsk[];
extern long			m66230_time_out;
extern union aap	m66230_ds;
#endif

#endif	/* SCUCTL_H */
