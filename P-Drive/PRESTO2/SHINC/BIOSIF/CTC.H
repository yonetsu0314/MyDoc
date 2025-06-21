/*----------------------------------------------------------------------*
 * File name	: ctc.h		/ ctc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/ctc.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: ctc.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * コメントのネスト対応
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/ctc.h 1.2 1994/08/18 14:53:51 sato
 *Log: ctc.h
 * リビジョン 1.2  1994/08/18  14:53:51  sato
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(CTC_H)
#define	CTC_H

/*	<<< argument 1 >>>	       					*/

enum ctcdn {
	CTCCH0,				/* CTC -ch0			*/
	CTCCH1,				/* CTC -ch1			*/
	CTCCH2,				/* CTC -ch2			*/
	CTCCH_MAX,			/* maxmum channel number	*/
	CTCMOD	= CTCCH_MAX,		/* CTC -mode			*/
/*-------------------------------------*/
	CTCDV0	= 0*256,		/* CTC0 device number		*/
	CTCDVN	= 1*256,		/* CTCn device number		*/
};
#define	CTCDEV_MAX (CTCDVN/256)		/* maximum device number	*/

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>	       					*/

enum ctcm {
	CTC1MS	= 10,			/* 1ms interrupt intervel	*/
	CTC2MS,				/* 2ms interrupt intervel	*/
	CTC5MS,				/* 5ms interrupt intervel	*/
	CTC10MS,			/* 10ms interrupt intervel	*/
/*-------------------------------------*/
	CTC_M0	= 0x100,		/* ｶｳﾝﾄ･ｱｯﾌﾟ時割り込み発生	*/
};

int *	_ctc_init(enum ctcdn, enum ctcm);

int	_ctc_write(enum ctcdn, int);
int	_ctc_read(enum ctcdn);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>		 TCU Command word	 	*/

#define	CTC_SC_BIT	0xc0		/* select mode 0,1 bit		*/
#define	CTC_MLT_LUTCH	CTC_SC_BIT	/* multiple lutch command	*/
/*-------------------------------------*/
#define	CTC_RWM_BIT	0x30		/* RD/WR mode 0,1 bit		*/
#define	CTC_CNT_LUTCH	0		/* count lutch command		*/
#define	CTC_RW2B	CTC_RWM_BIT	/* 2 byte R/W mode(L->H)	*/
/*-------------------------------------*/
#define	CTC_CM_BIT	0xe0		/* counter mode 0,1,2 bit	*/

int	_ctc_mode(enum ctcdn, int);


/*--------------------------------------------------------------*/
/* Function:							*/
/*	ｶｳﾝﾀ･ﾗｯﾁを行い、16ﾋﾞｯﾄｶｳﾝﾀの値をﾘｰﾄﾞし、結果を*ret	*/
/*	にｽﾄｱｰする。						*/
/*--------------------------------------------------------------*/

void	_ctc_cnt_read(enum ctcdn, int *);

#define	_ctc_cnt_read(			/* ret:  16bit count		*/\
		chno,			/* arg1: channel number		*/\
		ret			/* arg2: pointer to cnt. value	*/\
		)	                				\
	_ctc_mode(chno&0xff00, (chno<<6)&CTC_SC_BIT+CTC_CNT_LUTCH);	\
 	*ret = _ctc_read(chno);

#endif
