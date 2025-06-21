/*----------------------------------------------------------------------*
 * File name	: dpmctxt.h	/ dpmem context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmctxt.h 1.1 1993/10/20 20:13:00 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DPMCTXT_H)
#define	DPMCTXT_H

#if	!defined(S_PTR_T)
#define	S_PTR	(unsigned short)	/* near pointerのｽｶﾗ型の宣言	*/
#define	S_PTR_T
#endif

#if	!defined(WD_PTR_T)
#define	WD_PTR	(unsigned short *)	/* word型へのfarﾎﾟｲﾝﾀのｷｬｽﾄ宣言	*/
#define	WD_PTR_T
#endif

/*------------------------------------------------------*/
/*	DPMEMのコマンド定義				*/
/*							*/
/* 注)	定義したNODEを、どのﾃﾞﾊﾞｲｽのﾃｷｽﾄに割り当てる	*/
/*	かは、APLのｲﾝﾌﾟﾘﾒﾝﾄ依存である。			*/
/*	割り込みﾊﾝﾄﾞﾗからrx_tsk[ECMD-1]のﾀｽｸを起床す	*/
/*	る変数として使用する。				*/
/*------------------------------------------------------*/

enum ecmd {
	CMD_ACK		= 0,		/* command acknowledge		*/
/*-------------------------------------*/
	DEFAULT		= 1,		/* EX. WCU_ICU command		*/
	NODE1,				/*  "  RCU			*/
	NODE2,				/*  "  DUC			*/
	NODE3,				/*  "  EXC			*/
	NODE4,				/*  "  HIC			*/
	NODE5,
	NODE6,
	NODE7,
	NODE8,
	NODE_MAX,
/*-------------------------------------*/
	BUSY_M8		= 0xaa,		/* 8 bit typeの初期ﾋﾞｼﾞｰ状態	*/
	BUSY_M16	= 0x55aa,	/* 16		"		*/
};
typedef enum ecmd	ECMD;

/*------------------------------------------------------*/
/*	DPMEMのコンテキスト構造体			*/
/*							*/
/*------------------------------------------------------*/

enum mtyp {
	BYTE_M,				/* ﾊﾞｲﾄ単位のｺﾏﾝﾄﾞRD/WR		*/
	WORD_M,				/* ﾜｰﾄﾞ		"		*/
};
typedef enum mtyp	MTYP;

typedef struct	dpm_ctxt {
	unsigned int	ipfw;		/* ICU ipfw I/O address		*/
	unsigned char	*iobase;	/* DP-SRAMのﾍﾞｰｽ物理ｱﾄﾞﾚｽ	*/
	int		mem_type;	/* word/byteのｱｸｾｽ方法(MTYP)	*/
	unsigned int	wrcmd;		/* ｺﾏﾝﾄﾞ書き込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	rdcmd;		/* ｺﾏﾝﾄﾞ読み込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt;		/* ﾃｷｽﾄ書き込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt_siz;	/* 	"	  のｻｲｽﾞ(byte数)*/
	unsigned int	rdtxt;		/* ﾃｷｽﾄ読み込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	rdtxt_siz;	/*	"	  のｻｲｽﾞ(byte数)*/
} DPM_CTXT;

#ifdef	STORAGE
struct dpm_ctxt	dpm_ctxt_tbl[]= {
		0, (void *)0, 0, 0, 0, 0, 0, 0, 0,
		0, (void *)0, 0, 0, 0, 0, 0, 0, 0
};
#else
extern struct dpm_ctxt	dpm_ctxt_tbl[];
#endif

#endif	/* DPMCTXT_H */

