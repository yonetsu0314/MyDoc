/*----------------------------------------------------------------------*
 * File name	: dadctxt.h	/ dadc context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadctxt.h 1.2 1970/01/01
 *Log: dadctxt.h
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/30  15:50
 * ・基板(P5561/P5554)に対応させた。
 *   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
 *   定義しないことでP5554(村田氏作成)に対応させた。
 *   
 * ・エンディアン(ビッグ/リトル)に対応させた。
 *   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
 *   定義しないことでリトルエンディアンに対応させた。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dadctxt.h 1.1 1996/07/22 13:37:37 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DADCTXT_H)
#define	DADCTXT_H

#if	!defined(DAD_WEI_MAX)
#define	DAD_WEI_MAX	24		/* 計量器の最大要素数		*/
#define	DAD_AFV_MAX	4		/* AFVｾﾙの	"		*/
#define	DAD_DF_MAX	8		/* 分散ﾌｨｰﾀﾞの	"		*/
#define DAD_SECT_MAX	2		/* ｾｸｼｮﾝ数			*/
#endif

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
	ILLEGAL		= 0,		/* illegal command		*/
	DEFAULT		= 1,		/* EX. WCU_ICU command		*/
	NODE1,				/*  "  RCU			*/
	NODE2,				/*  "  DUC			*/
	NODE3,				/*  "  EXC			*/
	NODE4,				/*  "  HIC			*/
	NODE_MAX,
/*-------------------------------------*/
	BUSY_M16	= 0x55aa,	/* 16bit typeの初期ﾋﾞｼﾞｰ状態	*/
};
typedef enum ecmd	ECMD;


/*------------------------------------------------------*/
/*	DADC(DPMEM)のコンテキスト構造体			*/
/*							*/
/*------------------------------------------------------*/

typedef struct	dadctxt {
	unsigned char	*iobase;/* DP-SRAMのﾍﾞｰｽ物理ｱﾄﾞﾚｽ	*/
	int		mem_type;	/* word/byteのｱｸｾｽ方法(MTYP)	[(BYTE_M / WORD_M) p:/presto/shlib/drive/dpmem/RCS/dpmctxt.h] */
	unsigned int	wrcmd;		/* ｺﾏﾝﾄﾞ書き込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	rdcmd;		/* ｺﾏﾝﾄﾞ読み込みｴﾘｱのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt;		/* ﾃｷｽﾄ書き込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	wrtxt_siz;	/* 	"	  のｻｲｽﾞ(byte数)*/
	unsigned int	rdtxt;		/* ﾃｷｽﾄ読み込みｴﾘｱへのｵﾌｾｯﾄ	*/
	unsigned int	rdtxt_siz;	/*	"	  のｻｲｽﾞ(byte数)*/
} DADCTXT;

#ifdef	STORAGE
struct dadctxt	dadc_ctxt_tbl[]= {
	(void *)0, 0, 0, 0, 0, 0x800, 0x800, 0x800,
	(void *)0, 0, 0, 0, 0, 0x800, 0x800, 0x800,
	(void *)0, 0, 0, 0, 0, 0x800, 0x800, 0x800,
	(void *)0, 0, 0, 0, 0, 0x800, 0x800, 0x800
};
#else
extern struct dadctxt		dadc_ctxt_tbl[];
#endif

/*------------------------------------------------------*/
/*	DADCのボードID構造体				*/
/*							*/
/*------------------------------------------------------*/

typedef struct	dadc_id {		/* dadc_id_io private struct	*/
	unsigned int	ipfw;		/* ICU ipfw I/O address		*/
	unsigned int	io_adr[DAD_MAX];	/* ID I/O address		*/
	union sgbp	ptn[DAD_MAX];		/* ﾋﾞｯﾄﾊﾟﾀｰﾝ			*/
} DADC_ID;

#ifdef	STORAGE
struct dadc_id	dadc_id_io;
#else
extern struct dadc_id	dadc_id_io;
#endif

#endif	/* DADCTXT_H */
