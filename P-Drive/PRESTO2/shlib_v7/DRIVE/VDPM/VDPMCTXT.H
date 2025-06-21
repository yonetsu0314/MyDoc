/*----------------------------------------------------------------------*
 * File name	: vdpmctxt.h	/ virtual dpmem context header		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpmctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vdpmctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/vdpm/rcs/vdpmctxt.h 1.1 1995/09/16 14:29:18 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(VDPMCTXT_H)
#define	VDPMCTXT_H

#if	!defined(S_PTR_T)
#define	S_PTR	(unsigned short)	/* near pointerのｽｶﾗ型の宣言	*/
#define	S_PTR_T
#endif

#if	!defined(WD_PTR_T)
#define	WD_PTR	(unsigned short *)	/* word型へのﾎﾟｲﾝﾀのｷｬｽﾄ宣言	*/
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

typedef struct	vdpm_ctxt {
	unsigned char	*iobase;	/* DP-SRAMのﾍﾞｰｽ物理ｱﾄﾞﾚｽ	*/
	int		mem_type;	/* word/byteのｱｸｾｽ方法(MTYP)	*/
	unsigned int	wrcmd;		/* ｺﾏﾝﾄﾞ書き込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	rdcmd;		/* ｺﾏﾝﾄﾞ読み込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt;		/* ﾃｷｽﾄ書き込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt_siz;	/* 	"	  のｻｲｽﾞ(byte数)*/
	unsigned int	rdtxt;		/* ﾃｷｽﾄ読み込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	rdtxt_siz;	/*	"	  のｻｲｽﾞ(byte数)*/
} VDPM_CTXT;

#ifdef	STORAGE
struct vdpm_ctxt	vdpm_ctxt_tbl[]= {
		(unsigned char *)0, 0, 0, 0, 0, 0, 0, 0,
		(unsigned char *)0, 0, 0, 0, 0, 0, 0, 0
};
#else
extern struct vdpm_ctxt	vdpm_ctxt_tbl[];
#endif

#endif	/* VDPMCTXT_H */
