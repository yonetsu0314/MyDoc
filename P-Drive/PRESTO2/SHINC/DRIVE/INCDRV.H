/*----------------------------------------------------------------------*
 * File name	: incdrv.h	/ i2netc driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/incdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: incdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(INCDRV_H)
#define	INCDRV_H

#define	INCMD_T

enum {
	I2NET0,				/* I2NETC #0 device number	*/
	I2NET1,				/* I2NETC #1 device number	*/
	I2NET2,				/* I2NETC #2 device number	*/
	I2NET_MAX
};

void		inc0_int_hdr(void);	/* I2NETC #0 interrupt handler	*/
void		inc1_int_hdr(void);	/* I2NETC #1 interrupt handler	*/
void		inc2_int_hdr(void);	/* I2NETC #2 interrupt handler	*/

/*------------------------------------------------------*/
/*	I2NETﾓｰﾄﾞ時のﾊﾟﾗﾒｰﾀ構造体			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	incpk0 {
	char		node_no[4];	/* ｿｰｽ･ﾉｰﾄﾞ･ﾅﾝﾊﾞｰ		*/
	int		seq_no;		/* ｼｰｹﾝｽ･ﾅﾝﾊﾞｰ			*/
	int		baud;		/* 転送ｽﾋﾟｰﾄﾞ			*/
	int		brod_ct;	/* ﾌﾞﾛｰﾄﾞｷｬｽﾄ送信回数		*/
	unsigned short	siz_txbuf;	/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
	unsigned short	siz_rxbuf;	/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
} INCPK0;

/*------------------------------------------------------*/
/*	ASYNCﾓｰﾄﾞ時のﾊﾟﾗﾒｰﾀ構造体			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	incpk1 {
	int		char_len;	/* ｷｬﾗｸﾀ長			*/
	int		stp_len;	/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数			*/
	int		pari_sel;	/* ﾊﾟﾘﾃｨｰ選択			*/
	int		baud;		/* 転送ｽﾋﾟｰﾄﾞ			*/
	unsigned short	siz_txbuf;	/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
	unsigned short	siz_rxbuf;	/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
} INCPK1;


/*------------------------------------------------------*/
/*	I2NET/ASYNCﾓｰﾄﾞのﾊﾟﾗﾒｰﾀ共用体			*/
/*							*/
/*------------------------------------------------------*/

typedef union	incpk {			/* argument 2			*/
	struct incpk0	pk0;		/* I2NETﾓｰﾄﾞ時のﾊﾟﾗﾒｰﾀ		*/
	struct incpk1	pk1;		/* ASYNCﾓｰﾄﾞ時のﾊﾟﾗﾒｰﾀ		*/
} INCPK;

enum incmd {
	I2NET,				/* I2NETﾓｰﾄﾞ			*/
	INC_ASYNC			/* 調歩同期ﾓｰﾄﾞ			*/
};
typedef	enum incmd	INCMD;		/* operation mode		*/

enum incrv {
	INCPRM_ERR0	= -3,		/* I2NET parameter error	*/
	INCPRM_ERR1,			/* ASYNC parameter error	*/
	INCSYS_ERR,			/* system error			*/
	INCCMPL				/* success			*/
};
typedef enum incrv	INCRV;		/* return value			*/


INCRV		i2net_sys_init(int, INCMD, const void *, union aap *);

/*------------------------------------------------------*/
/*	I2NETﾓｰﾄﾞ時の送受信ﾃｷｽﾄ内ﾍｯﾀﾞｰ部		*/
/*							*/
/* 注)	I2NETﾃﾞﾌｫﾙﾄﾓｰﾄﾞ時のread/writeにはﾃｷｽﾄの先頭に	*/
/*	(第2ﾊﾟﾗﾒｰﾀ)以下のﾍｯﾀﾞｰ部が含まれる。write時で	*/
/*	node_noに0を設定した場合、node_noの変更は行わ	*/
/*	ない。						*/
/*------------------------------------------------------*/

#if	!defined(INC_BUF)
#define	INC_BUF		1		/* APL側の仮のﾃｷｽﾄ･ｻｲｽﾞを指定	*/
#endif

typedef struct	incfmt {
	unsigned char	ascii_node[4];	/* ﾉｰﾄﾞ･ﾅﾝﾊﾞｰ(dst:送信/src:受信)*/
	unsigned char	ascii_seq[4];	/* ｼｰｹﾝｽ･ﾅﾝﾊﾞｰ(受信時)		*/
	unsigned char	text[INC_BUF];	/* ﾃｷｽﾄ部			*/
} INCFMT;


/*------------------------------------------------------*/
/* Function:						*/
/*	DIP switchのﾃﾞｰﾀを読み込む			*/
/*------------------------------------------------------*/
#ifdef	P5348
#define	_dip_sw()			/* ret:  intｽｲｯﾁ･ﾃﾞｰﾀ/err:-1	*/\
	i2net_pio_read(I2NET0, INC_INP0)
#endif

enum incpid {				/* i2netc parallel port ID no.	*/
	INC_INP0,			/* input port 0			*/
	INC_INP1,			/*	"     1			*/
	INC_OTP0			/* output port 0		*/
};
typedef enum incpid	INCPID;

int		i2net_pio_read(int, INCPID);
/*----------------------------------------------------------------------*/

int		i2net_pio_write(int, int);

#endif	/* INCDRV_H */

