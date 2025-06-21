/*----------------------------------------------------------------------*
 * File name	: admtxt.h	/ adc moniotr text header		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/admtxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: admtxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/admtxt.h 1.1 1970/01/01
 *Log: admtxt.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* Function	:							*/
/*	monitor用のコマンド及び通信テキストを定義。			*/
/*----------------------------------------------------------------------*/


#define	RXBUF_MAX	2048		/* 受信ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
#define	TXBUF_MAX	2048		/* 送信ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/

#define	PUNCTUATOR	','		/* 句切り文字			*/

/*------------------------------------------------------*/
/*	コマンドの定義					*/
/*							*/
/*------------------------------------------------------*/

enum mcmd {
/*-------------------------------------*/
	RESET,				/* software reset		*/
	START,				/* start sampling		*/
	STOP,				/* stop sampling		*/
	IBOOT,				/* intial parameter boot	*/
	CBOOT,				/* change parameter boot	*/
	ZERO,				/* zero tracking		*/
	READ,				/* memory read			*/
	WRITE,				/* memory write			*/
	DSPIBOOT,			/* DSP intial parameter boot	*/
	DSPCBOOT,			/* DSP change parameter boot	*/
	DSPZERO,			/* DSP zero tracking		*/
	SPAN,				/* span adjustment		*/
	AFV,				/* AFV adjustment		*/
	DSPREAD,			/* DSP memory read		*/
	DSPWRITE,			/* DSP memory write		*/
	CHADC,				/* ADC board change		*/
	SHOWMAP,			/* ARCNET show mapping		*/
	WEIREAD,			/* weight read			*/
/*-------------------------------------*/
	MON_CMD_MAX
};

int		mon_text_interpret(unsigned char *, int);
int		mon_dad_reset(int, char **);
int		mon_dad_sta_sampl(int, char **);
int		mon_dad_stp_sampl(int, char **);
int		mon_dad_iboot(int, char **);
int		mon_dad_cboot(int, char **);
int		mon_dad_mzero(int, char **);
int		mon_dad_memrd(int, char **);
int		mon_dad_memwr(int, char **);
int		mon_dsp_iboot(int, char **);
int		mon_dsp_cboot(int, char **);
int		mon_dsp_zero(int, char **);
int		mon_dsp_span(int, char **);
int		mon_dsp_sta_afv(int, char **);
int		mon_dsp_stp_afv(int, char **);
int		mon_dsp_memrd(int, char **);
int		mon_dsp_memwr(int, char **);
int		mon_chg_adc_board(int, char **);
int		mon_arc_showmap(int, char **);
int		mon_dad_weird(int, char **);

/*------------------------------------------------------*/
/*	メッセージの定義				*/
/*							*/
/*------------------------------------------------------*/

enum mmsg {
/*-------------------------------------*/
	IDX_DATA	= 0,		/* index data			*/
	DATA_SAMPL,			/* sampling data		*/
	DATA_AFV,			/* afv coefficient data		*/
/*-------------------------------------*/
	IDX_SUCES,			/* index success		*/
	RESET_SUCES	= IDX_SUCES,	/* software reset 成功		*/
	START_SUCES,			/* start sampling  "		*/
	STOP_SUCES,			/* stop sampling   "		*/
	IBOOT_SUCES,			/* intial parameter boot  "	*/
	CBOOT_SUCES,			/* change parameter boot  "	*/
	ZERO_SUCES,			/* zero tracking   "		*/
	READ_SUCES,			/* memory read	   "		*/
	WRITE_SUCES,			/* memory write	   "		*/
	DSP_IBOOT_SUCES,		/* DSP intial parameter boot  "	*/
	DSP_CBOOT_SUCES,		/* DSP change parameter boot  "	*/
	DSP_ZERO_SUCES,			/* DSP zero tracking	"	*/
	SPAN_SUCES,			/* DSP span adjustment	"	*/
	AFV_STA_SUCES,			/* DSP afv adjustment	"	*/
	AFV_STP_SUCES,			/* DSP afv adjustment	"	*/
	DSP_READ_SUCES,			/* DSP memory read	   "	*/
	DSP_WRITE_SUCES,		/* DSP memory write	   "	*/
	CHG_ADC_SUCES,			/* ADC board change   "		*/
	ARC_SHOWMAP_SUCES,		/* ARCNET show mapping   "	*/
	WEI_READ_SUCES,			/* weight read		"	*/
/*-------------------------------------*/
	IDX_FAIL,			/* index fail			*/
	RESET_FAIL	= IDX_FAIL,	/* software reset 失敗		*/
	START_FAIL,			/* start sampling  "		*/
	STOP_FAIL,			/* stop sampling   "		*/
	IBOOT_FAIL,			/* intial parameter boot  "	*/
	CBOOT_FAIL,			/* change parameter boot  "	*/
	ZERO_FAIL,			/* zero tracking   "		*/
	READ_FAIL,			/* memory read	   "		*/
	WRITE_FAIL,			/* memory write	   "		*/
	DSP_IBOOT_FAIL,			/* DSP intial parameter boot  "	*/
	DSP_CBOOT_FAIL,			/* DSP change parameter boot  "	*/
	DSP_ZERO_FAIL,			/* DSP zero tracking	"	*/
	SPAN_FAIL,			/* DSP span adjustment	"	*/
	AFV_STA_FAIL,			/* DSP afv adjustment	"	*/
	AFV_STP_FAIL,			/* DSP afv adjustment	"	*/
	DSP_READ_FAIL,			/* DSP memory read	   "	*/
	DSP_WRITE_FAIL,			/* DSP memory write	   "	*/
	CHG_ADC_FAIL,			/* ADC board change   "		*/
	ARC_SHOWMAP_FAIL,		/* ARCNET show mapping   "	*/
	WEI_READ_FAIL,			/* weight read		   "	*/
/*-------------------------------------*/
	IDX_UNEXE,			/* index unexecute		*/
	RESET_UNEXE	= IDX_UNEXE,	/* software reset 未実行	*/
	START_UNEXE,			/* start sampling  "		*/
	STOP_UNEXE,			/* stop sampling   "		*/
	IBOOT_UNEXE,			/* intial parameter boot  "	*/
	CBOOT_UNEXE,			/* change parameter boot  "	*/
	ZERO_UNEXE,			/* zero tracking   "		*/
	READ_UNEXE,			/* memory read	   "		*/
	WRITE_UNEXE,			/* memory write	   "		*/
	DSP_IBOOT_UNEXE,		/* DSP intial parameter boot  "	*/
	DSP_CBOOT_UNEXE,		/* DSP change parameter boot  "	*/
	DSP_ZERO_UNEXE,			/* DSP zero tracking	"	*/
	SPAN_UNEXE,			/* DSP span adjustment	"	*/
	AFV_STA_UNEXE,			/* DSP afv adjustment	"	*/
	AFV_STP_UNEXE,			/* DSP afv adjustment	"	*/
	DSP_READ_UNEXE,			/* DSP memory read	   "	*/
	DSP_WRITE_UNEXE,		/* DSP memory write	   "	*/
	CHG_ADC_UNEXE,			/* ADC board change   "		*/
/*-------------------------------------*/
	IDX_ERROR,			/* index error			*/
	MON_COM_ERROR	= IDX_ERROR,	/* ｺﾐｭﾆｹｰｼｮﾝ･ｴﾗｰ		*/
	ILLEGAL_TXT_FMT,		/* ﾃｷｽﾄ･ｺﾏﾝﾄﾞのﾌｫｰﾏｯﾄが不正	*/
	MON_UNDEF_CMD,			/* 未定義のｺﾏﾝﾄﾞ		*/
	ILLEGAL_NO_PRM,			/* ﾄｰｸﾝの数が合わない		*/
	ILLEGAL_PRM1,			/* ﾊﾟﾗﾒｰﾀ1の内容が不正		*/
	ILLEGAL_PRM2,			/* ﾊﾟﾗﾒｰﾀ2の内容が不正		*/
	ILLEGAL_PRM3,			/* ﾊﾟﾗﾒｰﾀ3の内容が不正		*/
	ILLEGAL_PRM4,			/* ﾊﾟﾗﾒｰﾀ4の内容が不正		*/
	ILLEGAL_PRM5,			/* ﾊﾟﾗﾒｰﾀ5の内容が不正		*/
	ILLEGAL_PRM6,			/* ﾊﾟﾗﾒｰﾀ6の内容が不正		*/
	ILLEGAL_PRM7,			/* ﾊﾟﾗﾒｰﾀ7の内容が不正		*/
	ILLEGAL_NO_PRM1,		/* ﾊﾟﾗﾒｰﾀ1の数が合わない	*/
	ILLEGAL_NO_PRM2,		/* ﾊﾟﾗﾒｰﾀ2の数が合わない	*/
	ILLEGAL_NO_PRM3,		/* ﾊﾟﾗﾒｰﾀ3の数が合わない	*/
	ILLEGAL_NO_PRM4,		/* ﾊﾟﾗﾒｰﾀ4の数が合わない	*/
	ILLEGAL_NO_PRM5,		/* ﾊﾟﾗﾒｰﾀ5の数が合わない	*/
	ILLEGAL_NO_PRM6,		/* ﾊﾟﾗﾒｰﾀ6の数が合わない	*/
	ILLEGAL_NO_PRM7,		/* ﾊﾟﾗﾒｰﾀ7の数が合わない	*/
	NOT_ASII_HEX,			/* ASCII HEXでないｺｰﾄﾞ		*/
	ERR_RD_SAMPL,			/* ｻﾝﾌﾟﾘﾝｸﾞ･ﾊﾞｯﾌｧのﾘｰﾄﾞｴﾗｰ	*/
	TERM_DATA_SAMPL,		/* ｻﾝﾌﾟﾘﾝｸﾞの終了		*/
	ERR_RD_FIL3RD,			/* ADCの3段目ﾃﾞｰﾀのﾘｰﾄﾞｴﾗｰ	*/
	ERR_DSP_FUNC,			/* DSPの関数ｴﾗｰ			*/
	INVALID_FIL3RD,			/* ADCの3段目ﾃﾞｰﾀが無効		*/
	TERM_AFV_ADJ,			/* AFV調整の終了		*/
	TERM_STB_CHK,			/* 安定ﾁｪｯｸの終了		*/
/*-------------------------------------*/
	MSG_MAX
};

int		mon_text_snd(enum mmsg, void **);
char *		mon_sampl_text(char *, void **);
char *		mon_afv_text(char *, void **);
char *		mon_zero_text(char *, void **);
char *		mon_memrd_text(char *, void **);
char *		mon_ter_sampl_text(char *, void **);
char *		mon_cboot_text(char *, void **);
char *		mon_dsp_zero_text(char *, void **);
char *		mon_span_text(char *, void **);
char *		mon_showmap_text(char *, void **);
char *		mon_wei_text(char *, void **);

int		mon_save_sampl_stat(unsigned short);

enum cnv {				/*	<<< argument 1 >>>	*/
	CNV_3RD		= (0x1 << 5),	/* 3RD段目ﾃﾞｰﾀの変換		*/
	CNV_4TH		= (0x1 << 6),	/* 4TH	"			*/
};
int		sta_snd_tsk(enum cnv);

enum sts {				/*	<<< argument 1 >>>	*/
	AFV_HALT,			/* AFV調整停止			*/
	AFV_PROGRESS,			/* AFV調整中			*/
};
int		sta_afv_tsk(unsigned long);
int		stp_afv_tsk(void);
