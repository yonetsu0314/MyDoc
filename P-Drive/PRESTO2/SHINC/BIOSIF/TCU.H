/*----------------------------------------------------------------------*
 * File name	: tcu.h		/ tcu bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/tcu.h 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: tcu.h $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/07/31 16:15 誤記修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * コメントのネスト対応
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/tcu.h 1.4 1995/04/05 14:18:48 sato
 *Log: tcu.h
 * リビジョン 1.4  1995/04/05  14:18:48  sato
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(TCU_H)
#define	TCU_H

/*--------------------------------------------------------------*/
/* Function:							*/
/*	SCUのﾎﾞｰﾚｰﾄを設定する					*/
/*--------------------------------------------------------------*/

#define	_baud_scu(			/* ret:  0			*/\
		baud_code		/* arg1: sireal.hを参照		*/\
		)			\
	_scu_baud(0, baud_code)

#define	_baud_scu1(			/* ret:  0			*/\
		baud_code		/* arg1: sireal.hを参照		*/\
		)			\
	_scu_baud(1, baud_code)

int	_scu_baud(int, int);

#ifdef	P5280				/* HardwareがP5280の場合	*/
/*--------------------------------------------------------------*/
/* Function:							*/
/*	MPSCのﾎﾞｰﾚｰﾄを設定する					*/
/*--------------------------------------------------------------*/

#define	_baud_mpsc(			/* ret:  0			*/\
		baud_code		/* arg1: sireal.hを参照		*/\
		)			\
	_tcu_init(2, baud_code)


#else					/* それ以外のHaredware		*/
/*--------------------------------------------------------------*/
/* Function:							*/
/*	Timer割り込みの周期を設定する				*/
/*--------------------------------------------------------------*/

/*	<<< tm_codeは以下の定数を設定 >>>				*/
enum {
	C1MS = 10,			/* 1ms interrupt intervel	*/
	C2MS,				/* 2ms interrupt intervel	*/
	C5MS,				/* 5ms interrupt intervel	*/
	C10MS,				/* 10ms interrupt intervel	*/
/*-------------------------------------*/
	TCU_M0	= 0x100,		/* ｶｳﾝﾄ･ｱｯﾌﾟ時割り込み発生	*/
};
#define	tmint_base(			/* ret:  0			*/\
		tm_code			/* arg1: enumを参照		*/\
		)			\
	_tcu_init(2, tm_code)

#endif


int  	_tcu_init(int, int);
int  	_tcu_write(int, int);
int  	_tcu_read(int);
int  	_tcu_bread(int);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>		 TCU Command word	 	*/

#define	TCU_SC_BIT	0xc0		/* select mode 0,1 bit		*/
#define	TCU_MLT_LUTCH	TCU_SC_BIT	/* multiple lutch command	*/
/*-------------------------------------*/
#define	TCU_RWM_BIT	0x30		/* RD/WR mode 0,1 bit		*/
#define	TCU_CNT_LUTCH	0		/* count lutch command		*/
#define	TCU_RW2B	TCU_RWM_BIT	/* 2 byte R/W mode(L->H)	*/
/*-------------------------------------*/
#define	TCU_CM_BIT	0xe0		/* counter mode 0,1,2 bit	*/

int  	_tcu_mode(int);


/*--------------------------------------------------------------*/
/* Function:							*/
/*	ﾏﾙﾁﾌﾟﾙ･ﾗｯﾁを行い、16ﾋﾞｯﾄｽﾃｰﾀｽ値と16ﾋﾞｯﾄｶｳﾝﾀ値をﾘｰﾄﾞし、	*/
/*	結果を*stat、*cntにｽﾄｱｰする。				*/
/*--------------------------------------------------------------*/

void	_tcu_cnt_read(int, int *, int *);

#define	_tcu_cnt_read(			/* ret:  16bit count		*/\
		chno,			/* arg1: TCU channel number	*/\
		stat,			/* arg2: pointer to stat. value	*/\
		cnt			/* arg3: pointer to cnt. value	*/\
		)	                				\
	_tcu_mode(TCU_MLT_LUTCH|(0x01<<(chno+1)));			\
	*stat = _tcu_bread(chno);					\
 	*cnt = _tcu_read(chno);

#endif
