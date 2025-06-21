/*----------------------------------------------------------------------*
 * File name	: comctl.h	/ arc-net control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comctl.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: comctl.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2004/07/21 15:30:00
 * 自動認識機能用にarc_exist_flag[]を追加。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comctl.h 1.1 1995/11/07 17:13:13 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#if	!defined(COMCTL_H)
#define	COMCTL_H

#define	inp(a)			*(unsigned char *)(a)
#define	outp(a,b)		*(unsigned char *)(a) = (unsigned char)b
/**/

int		arcnet_open(int devno, int mode);
int		arcnet_close(int devno);
int		arcnet_rx(int devno, unsigned char *buf, int len);
int		arcnet_tx(int devno, unsigned char *buf, int len);
int		arc_ena_rx(struct com_ctxt *pct, struct rxh *phd);
int		setup_rxint_tsk(int devno);
int		read_diagr(struct com_ctxt *pct, unsigned char mask,
			unsigned char res);
int		arc_dpm_read(int devno, unsigned char **rtp);
int		check_tx_pend(int devno);
int		arc_nextid(struct com_ctxt *pc);

#define	LF	(unsigned char)'\n'	/* New line 	   :0x0a	*/
#define	CR	(unsigned char)'\r'	/* Carriage return :0x0d	*/

#define	COM_DEV_MAX	4		/* ARCNETｺﾝﾄﾛｰﾗの最大ﾃﾞﾊﾞｲｽ数	*/

#if	!defined(V_FUNC_T)
typedef	void	(*V_FUNC)();	/* voidを返す関数へのﾎﾟｲﾝﾀ	*/
#define	V_FUNC_T
#endif

typedef struct	ucsinf {
	short		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCSINF;

#ifdef	STORAGE
struct ucsinf	com_icu_inf[COM_DEV_MAX] = {	/* INTが接続されているICU	*/
			(short)-1,(unsigned char)-1,(unsigned char)-1,
			(short)-1,(unsigned char)-1,(unsigned char)-1,
			(short)-1,(unsigned char)-1,(unsigned char)-1,
			(short)-1,(unsigned char)-1,(unsigned char)-1
		};
V_FUNC const	com_int_hdr[] = {
			arcnet0_int_hdr, arcnet1_int_hdr,
			arcnet2_int_hdr, arcnet3_int_hdr
		};
#else
extern struct ucsinf	com_icu_inf[];
extern V_FUNC const	com_int_hdr[];
#endif

/*------------------------------------------------------*/
/*	ネットワーク管理用の構造体定義			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	comh {
	unsigned char	*map_tbl;	/* 接続されているﾉｰﾄﾞ番号ﾃｰﾌﾞﾙ	*/
	unsigned int	recon_cntr;	/* ﾘｺﾝﾌｨｸﾞ発生回数ｶｳﾝﾀ		*/
} COMH;

#ifdef	STORAGE
int		com_mode[COM_DEV_MAX] = {
			-1, -1,		/* file access mode		*/
			-1, -1		/* file access mode		*/
		};
struct comh 	com_hd_tbl[COM_DEV_MAX] = {
			0, 0,		/* ARCNETのﾍｯﾀﾞｰ部の配列	*/
			0, 0,		/* ARCNETのﾍｯﾀﾞｰ部の配列	*/
		};
#else
extern int		com_mode[];
extern struct comh	com_hd_tbl[];
#endif

/*------------------------------------------------------*/
/*	テキスト管理用の構造体定義			*/
/*							*/
/*------------------------------------------------------*/
#define	PKTH_T

typedef struct	pkth {
	unsigned char	all;		/* 総ﾊﾟｹｯﾄ数-1			*/
	unsigned char	rest;		/* 残りﾊﾟｹｯﾄ数-1		*/
} PKTH;

#if	defined(PKTH_T)
#define	SIZE_PKTH	sizeof(PKTH)
#else
#define	SIZE_PKTH	0
#endif

#define	SHORT_PACKET

#if	defined(LONG_PACKET)
#define	RXPAGE_MIN	2		/* 受信開始ﾍﾟｰｼﾞ=2		*/
#else
#define	RXPAGE_MIN	1		/* 受信開始ﾍﾟｰｼﾞ=1		*/
#endif
#define	RXPAGE_MAX	7		/* 受信最終ﾍﾟｰｼﾞ		*/

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void		*vfp[2];	/* void型farポインタ	*/
	char		*cfp[2];	/* char		"	*/
	int		*ifp[2];	/* int型	"	*/
	long		*lfp[2];	/* long型	"	*/
	unsigned int	ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]			/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp	vfpp[1]			/* [1]void型farポインタ	*/
#define	cfp	cfpp[1]			/* [1]char	"	*/
#define	ifp	ifpp[1]			/* [1]int型	"	*/
#define	lfp	lfpp[1]			/* [1]long型	"	*/
#endif

#ifdef	STORAGE
union aap	com_sem[COM_DEV_MAX] = {
			0L, 0L,		/* COM_chipｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
			0L, 0L		/* COM_chipｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
unsigned int	com_sem_id[COM_DEV_MAX] = {
			COM0_SEM_ID, COM1_SEM_ID,
			COM2_SEM_ID, COM3_SEM_ID
};
unsigned char	com_nest[COM_DEV_MAX] = {
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0 /* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};
long		com_time_out = 2000L;	/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)		*/
union aap	com_ds = { 0L,0L };	/* ARCNETﾄﾞﾗｲﾊﾞｰのsegment情報	*/

union aap	com_tx_sem[COM_DEV_MAX] = {
			0L, 0L,		/* TXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
			0L, 0L,		/* TXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
			0L, 0L,		/* TXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
			0L, 0L		/* TXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
unsigned int	com_tx_sem_id[COM_DEV_MAX] = {
			COM0TX_SEM_ID, COM1TX_SEM_ID,
			COM2TX_SEM_ID, COM3TX_SEM_ID
};
unsigned char	com_tx_nest[COM_DEV_MAX] = {
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0,/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
			(unsigned char)0 /* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};
int		tx_init_flag[COM_DEV_MAX] = {
			1, 1,
			1, 1
};
int		arc_exist_flag[COM_DEV_MAX] = {		/* ARCNET IC 有:1 無:0 */
			0, 0,
			0, 0
};
#else
extern union aap	com_sem[];
extern unsigned int	com_sem_id[];
extern unsigned char	com_nest[];
extern long		com_time_out;
extern union aap	com_ds;
extern union aap	com_tx_sem[];
extern unsigned int	com_tx_sem_id[];
extern unsigned char	com_tx_nest[];
extern int		tx_init_flag[];
extern int		arc_exist_flag[];
#endif

#endif	/* COMCTL_H */

