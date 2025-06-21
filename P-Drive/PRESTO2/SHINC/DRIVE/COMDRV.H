/*----------------------------------------------------------------------*
 * File name	: comdrv.h	/ com20020b driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/comdrv.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: comdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/13 19:30
 * DMUアプリで使用するarcnetに関連したプロトタイプ
 * 宣言を追加する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/rcs/comdrv.h 1.4 1995/11/15 10:03:31 ryoji
 *Log: comdrv.h
 * リビジョン 1.4  1995/11/15  10:03:31  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(COMDRV_H)
#define	COMDRV_H

enum {
	ARCNET0,			/* ARCNET #0 device number	*/
	ARCNET1,			/* ARCNET #1 device number	*/
	ARCNET_MAX
};

void		arcnet0_int_hdr(void);	/* COM20020 #0 interrupt handler*/
void		arcnet1_int_hdr(void);	/* COM20020 #1 interrupt handler*/

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
char *	(get_arc_map)(int devno);
int	arc_contact(int devno, int node);
int	arc_stat(int devno, enum statspc cmd);
int	arc_ctrl(int devno, enum cmdspc cmd);
int _arc_node(int no, int id);
int _arc_nextid(int no);
int _chk_arcnet_tbl(int no, int N_ID);
int _arc_map(int no);
int _map_chk(int no);
int _arc_stat(int no, enum statspc cmd);

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

#endif	/* COMDRV_H */

