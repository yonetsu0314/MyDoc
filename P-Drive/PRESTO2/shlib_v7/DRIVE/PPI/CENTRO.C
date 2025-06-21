/*----------------------------------------------------------------------*
 * File name	: centro.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ppi/RCS/centro.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: centro.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#define		P5280
#include	<biosif\ppi.h>

#define		RETRY	50000			/* retry count		*/
#define		RDY	0x08			/* READYの入力BIT位置	*/
#define		CR	0X0a			/* CR code		*/
#define		LF	0x0d			/* LF code		*/
#define		ON	0			/* strobe :active low	*/
#define		OFF	1			/*    "	  :inactive	*/


/*----------------------------------------------------------------------*/
/*	loop_wait							*/
/*----------------------------------------------------------------------*/
/* 説明							 		*/
/*  	約２００μｓ（ミニマム）ウエイトする。		 		*/
/*							 		*/
/*----------------------------------------------------------------------*/

#define		Tw	200			/* ウエイトする時間μｓ	*/
#define		CLK	136			/* 1clock = 136ns	*/
#define		FET	5			/* inst. fetch = 5 state*/
#define		B1	6			/* ループ外の命令バイト */
#define		E1	42			/*     "   実行ステート */
#define		B2	3			/* ループ内の命令バイト */
#define		E2	18			/*     "   実行ステート */

#define		TMC	(Tw-((B1*FET+E1)*CLK/1000)) \
			/((B2*FET+E2)*CLK/1000)	/* ループ時間定数	*/

static void
loop_wait(void)
{
	register short	lc;			/* loop counter		*/

	lc = (short)TMC;
	do {
	}while(--lc);
}


/*----------------------------------------------------------------------*/
/*	strobed_output							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	data		: 出力データ（バイトデータ）			*/
/*									*/
/* 説明									*/
/*  	データをストローブ付きでＰＰＩ-ＰＡへ出力する。			*/
/*									*/
/*----------------------------------------------------------------------*/

static void
strobed_output(
	unsigned char	data			/* output data		*/
	)
{
	_pio_wr(PIO0, PORT_A, data);
	loop_wait();
	_ctl_strobe(ON);
	loop_wait();
	_ctl_strobe(OFF);
	loop_wait();
}


/*----------------------------------------------------------------------*/
/*	centro								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	len		: テキスト長[BYTE]				*/
/*	pnt		: 出力データの格納アドレス			*/
/*									*/
/* 戻り値								*/
/*	int		: エラーの場合１				*/
/*									*/
/* 説明									*/
/*	制御はデバイス側のREADYをチェックし,DETA[8bit]+STROBEを出力	*/
/*	する。なおバッファー内のテキストにリーディングフラグ[= CR],	*/
/*	トレーリングフラグ[= LF]コードを付加する。			*/
/*									*/
/*----------------------------------------------------------------------*/

int
centro(
	int		len,			/* text length		*/
	unsigned char	*pnt			/* output data pointer	*/
	)
{
	int	i;				/* retry counter	*/

	_ctl_strobe(OFF);
	for(i= RETRY; _interlock(RDY); i--) {
		if(! i)  return(1);
	}
	strobed_output((unsigned char)CR);
	for( ; len; len--,pnt++) {
		for(i= RETRY; _interlock(RDY); i--) {
			if(! i)  return(1);
		}
		strobed_output(*pnt);
	}
	for(i= RETRY; _interlock(RDY); i--) {
		if(! i)  return(1);
	}
	strobed_output((unsigned char)LF);
	return(0);
}
