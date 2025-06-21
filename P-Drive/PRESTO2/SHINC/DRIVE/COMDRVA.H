/*----------------------------------------------------------------------*
 * File name	: comdrva.h	/ com20020b driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/comdrva.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: comdrva.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 割込みハンドラのプロトタイプ宣言追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/rcs/comdrva.h 1.2 1995/11/07 16:57:24 ryoji
 *Log: comdrva.h
 * リビジョン 1.2  1995/11/07  16:57:24  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(COMDRVA_H)
#define	COMDRVA_H

enum {
	ARCNET0,			/* ARCNET #0 device number	*/
	ARCNET1,			/* ARCNET #1 device number	*/
	ARCNET2,			/* ARCNET #2 device number	*/
	ARCNET3,			/* ARCNET #3 device number	*/
	ARCNET_MAX
};

void	arcnet0_int_hdr(void);		/* COM20020 #0 interrupt handler*/
void	arcnet1_int_hdr(void);		/* COM20020 #1 interrupt handler*/
void	arcnet2_int_hdr(void);		/* COM20020 #2 interrupt handler*/
void	arcnet3_int_hdr(void);		/* COM20020 #3 interrupt handler*/

void	arcnet0_int_hdr_(void);
void	arcnet1_int_hdr_(void);
void	arcnet2_int_hdr_(void);
void	arcnet3_int_hdr_(void);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

enum comspeed {
	C156KBPS,			/* 156.25K BPS			*/
	C312KBPS,			/* 312.5K BPS			*/
	C625KBPS,			/* 625K BPS			*/
	C1250KBPS,			/* 1.25M BPS			*/
	C2500KBPS,			/* 2.5M BPS			*/
};

enum statspc {
	GET_RECON,			/* get reconfiguration counter	*/
	TXPND_STAT,			/* ger tx pendding status	*/
};

enum cmdspc {
	CLR_TX,				/* clear tx pendding		*/
	CLR_RX,				/* clear rx pendding		*/
};

typedef struct	compk {
	unsigned char	node_no;	/* node ID			*/
	enum comspeed	baud;		/* 転送ｽﾋﾟｰﾄﾞ			*/
	unsigned short	siz_buf;	/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
} COMPK;

int	arcnet_sys_init(int, const struct compk *, union aap *);
char	*(get_arc_map)(int devno);
int	arc_contact(int devno, int node);
int	arc_stat(int devno, enum statspc cmd);
int	arc_ctrl(int devno, enum cmdspc cmd);


/*------------------------------------------------------*/
/*	送受信ﾃｷｽﾄ･ﾌｫｰﾏｯﾄ				*/
/*							*/
/* 注)	COM20020に対してのread/writeは、ﾃｷｽﾄの先頭	*/
/*	にﾉｰﾄﾞ番号のﾍｯﾀﾞｰ部が含まれる。			*/
/*------------------------------------------------------*/

#if	!defined(COM_BUF)
#define	COM_BUF		1		/* APL側の仮のﾃｷｽﾄ･ｻｲｽﾞを指定	*/
#endif

typedef struct	comfmt {
	unsigned char	src_node;	/* ｿｰｽ･ﾉｰﾄﾞ番号			*/
	unsigned char	dst_node;	/* ﾃﾞｨｽﾃｨﾈｰｼｮﾝ･ﾉｰﾄﾞ番号		*/
	unsigned char	text[COM_BUF];	/* ﾃｷｽﾄ部			*/
} COMFMT;

#endif	/* COMDRVA_H */

