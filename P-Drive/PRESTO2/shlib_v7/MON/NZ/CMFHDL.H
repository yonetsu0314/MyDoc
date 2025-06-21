/*======================================================================
 * File name    : cmfhdl.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	通信デバイスファイルアクセスモジュール
 *	変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/dz_wcu2/rcs/cmfhdl.h 1.2.6308.1 1970/01/01 00:00:00 syuji Exp $
 *$Log: cmfhdl.h $
 * リビジョン 1.2.6308.1  1970/01/01  00:00:00  syuji
 * 2002/12/23 12:00
 * ＳＨＣコンパイラー使用によるバグフィックス。
 * 	ＥＮＵＭ宣言のタグ名の誤り修正。
 * 	関数プロトタイプの不一致エラーを修正。
 * 	マクロの２重定義の修正。
 * 	外部参照変数の定義。
 * 	変数名等の’＄’文字使用の禁止。’＿’に変更。
 * 	旧記述様式を改める。
 * 
 * リビジョン 1.2  1996/06/19  16:33:33  chimura
 * 変数の重複を回避する。
 * 
 * リビジョン 1.1  1996/06/18  18:41:44  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/dz_wcu/rcs/cmfhdl.h 3.1 1995/07/01 16:48:34 syuji
 *Log: cmfhdl.h
 * リビジョン 3.1  1995/07/01  16:48:34  syuji
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		CMFHDL_H
#define		CMFHDL_H
#ifndef	HPTN_H
#include	"hptn.h"			/* Show Current		*/
#endif
/*--------------------------------------------------------------*/
/*	デバイスを選択する場合の列挙型定数			*/
/*								*/
/* 注:	入出力を行うデバイスの変更はenumのデバイス名の順序	*/
/*	を変更する。						*/
/*--------------------------------------------------------------*/

enum {
	CON_DN,				/* 0: STD I/O  = "con"		*/
	DUC0_DN,			/* 1: DUC0     = "tty0"		*/
	DUC1_DN,			/* 2: DUC1     = "tty1"		*/
	ICU_DN,				/* 3: ICU      = "mbx0"		*/
	DUCN_DN,			/* 4: DUCN     = "lan0"		*/
	DSP_DN,				/* 5: DSP      = "mbx1"		*/
	COM_DN_MAX,
};

enum msg1nn{
	MSG1_EXC,			/* 例外処理用			*/
	DSP_NN,				/* node 1:default		*/
	MSG1_MAX
};

enum msg0nn{
	MSG0_EXC,			/* 例外処理用			*/
	INTER_NN,			/* node 1:default		*/
	RCU_NN,				/* node 2:RCU			*/
	HIC_NN,				/* node 3:HIC			*/
	MSG0_MAX,
};

#ifdef	STORAGE
union aap	rxtsk[COM_DN_MAX];	/* 受信ﾀｽｸのｱｸｾｽ･ｱﾄﾞﾚｽ		*/
union aap	msg1tsk[MSG1_MAX];
union aap	vdpm0tsk[MSG0_MAX];
#else
extern union aap	rxtsk[];
extern union aap	msg1tsk[];
extern union aap	vdpm0tsk[];
#endif

#define		rcurcv_tsk	vdpm0tsk[RCU_NN]


/*--------------------------------------------------------------*/
/*	ファイル･ハンドルを選択する場合の列挙型定数		*/
/*								*/
/*--------------------------------------------------------------*/

enum {
	DUC_FH0,			/* DUC channel 0		*/
	DUC_FH1,			/* DUC channel 1		*/
	ICU_FH,				/* ICU channel			*/
	DSP_FH,				/* DSP channel			*/
	CON_FH,				/* CON channel			*/
	LAN_FH,				/* DUC arcnet channel		*/
	COM_FH_MAX,
};

/*----------------------------------------------------------------------*/
/*	<<< return value >>>						*/
/*----------------------------------------------------------------------*/
enum comrv{
	COM_RETRY 	= -4,		/* リトライ			*/
	COM_ENODE,			/* ノード無し			*/
	RCV_ERROR,			/* 受信ｴﾗｰ			*/
	COM_DOWN	= RCV_ERROR,	/* 回線断			*/
	COM_FAIL,			/* 失敗				*/
	COM_COMPL			/* 成功				*/
};

typedef enum comrv	COMRV;


COMRV port_tbl_init(void);
COMRV con_gets(unsigned char *buf, int max);
COMRV con_puts(unsigned char *buf, int len);
COMRV duc_gets(enum lino ch, unsigned char *buf, int max);
COMRV duc_puts(enum lino ch, unsigned char *buf, int len);
COMRV rcu_gets(unsigned char *buf, int max);
COMRV rcu_puts(unsigned char *buf, int len);
COMRV ilk_read(unsigned char *buf, int max);
COMRV ilk_write(unsigned char *buf, int len);
COMRV con_write(unsigned char *buf, int len);
COMRV lan_read(unsigned char *buf, int max);
COMRV lan_write(unsigned char *buf, int len);

#ifdef	DUN
#if	!defined(STORAGE)
#if	!defined(duc_gets)
#define	duc_gets(ch, buf, max)	lan_read(buf, max)
#endif
#if	!defined(duc_puts)
#define	duc_puts(ch, buf, len)	lan_write(buf-2, len)
#endif
#endif
#endif

#endif
