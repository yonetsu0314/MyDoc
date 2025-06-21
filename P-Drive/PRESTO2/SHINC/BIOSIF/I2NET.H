/*----------------------------------------------------------------------*
 * File name	: i2net.h	/ i2netc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/i2net.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: i2net.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(I2NET_H)
#define	I2NET_H

/*	<<< argument 1 >>>						*/

typedef struct	incpinf {
	void	*stsrp;			/* stsr構造体へのﾎﾟｲﾝﾀｰ		*/
	void	*cmdrp;			/* cmdr構造体へのﾎﾟｲﾝﾀｰ		*/
	void	*prmrp;			/* prmr構造体へのﾎﾟｲﾝﾀｰ		*/
	void	*piorp;			/* pior構造体へのﾎﾟｲﾝﾀｰ		*/
	void	*rxbrp;			/* 受信bufr構造体へのﾎﾟｲﾝﾀｰ	*/
	void	*txbrp;			/* 送信bufr構造体へのﾎﾟｲﾝﾀｰ	*/
} INCPINF;

int 	_inc_init(int, INCPINF *);
/*----------------------------------------------------------------------*/

enum {
	_RES,				/* bit resetの定数		*/
	_SET				/* bit set    "			*/
};

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCの初期化完了をﾁｪｯｸする			*/
/*------------------------------------------------------*/

#define	_is_init_cmplete(		/* ret:  INI_CMPL/0 or -1	*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitrd(INI_CMPL, & cmdr_p ->macflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCへｿﾌﾄｳｪｱｰ･ﾘｾｯﾄ･ｺﾏﾝﾄﾞを発行する		*/
/*------------------------------------------------------*/

#define	_soft_reset(			/* ret:  0 or -1		*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitwr(RESET, _SET, & cmdr_p ->macflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCへﾊﾟﾗﾒｰﾀ書き込み要求ｺﾏﾝﾄﾞを発行する	*/
/*------------------------------------------------------*/

#define	_prm_write_req(			/* ret:  0 or -1		*/\
		ON_OFF,			/* arg1: _SET or _RES		*/\
		cmdr_p			/* arg2: pointer to CMDR	*/\
		)			\
	_inc_bitwr(PRM_WR, ON_OFF, & cmdr_p ->macflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCのﾊﾟﾗﾒｰﾀ書き込み要求に対する応答		*/
/*	をﾁｪｯｸする					*/
/*------------------------------------------------------*/

#define	_is_prmwr_ack(			/* ret:  PRM_ACK/0 or -1	*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitrd(PRM_ACK, & cmdr_p ->marflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCへ設定したﾊﾟﾗﾒｰﾀに対応するﾌﾗｸﾞをｾｯﾄする	*/
/*------------------------------------------------------*/

#define	_set_prmset_flag(		/* ret:  0 or -1		*/\
		MODE,			/* arg1: I2NET:0/ASYNC:1	*/\
		FLAG,			/* arg2: FLAG (bit pattern)	*/\
		cmdr_p			/* arg3: pointer to CMDR	*/\
		)			\
	(MODE) ?\
	(_inc_bitwr(FLAG, _SET, & cmdr_p ->pmasflg)):\
	(_inc_bitwr(FLAG, _SET, & cmdr_p ->pmsflg))

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCのASYNCﾓｰﾄﾞを設定する			*/
/*------------------------------------------------------*/

#define	_set_async_mode(		/* ret:  0 or -1		*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitwr(ASYNC_MD, _SET, & cmdr_p ->sacflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCの補助動作制御書き込みｺﾏﾝﾄﾞを発行する	*/
/*------------------------------------------------------*/

#define	_set_subact_ctl(		/* ret:  0 or -1		*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitwr(SAC_WR, _SET, & cmdr_p ->macflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCがASYNCﾓｰﾄﾞになっている事を確認する	*/
/*------------------------------------------------------*/

#define	_is_async_grant(		/* ret:  ASYNC_GRANT/0 or -1	*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitrd(ASYNC_GRANT, & cmdr_p ->sarflg)

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCのﾊﾟﾗﾒｰﾀ･設定ｴﾗｰ･ﾌﾗｸﾞを読み込む		*/
/*------------------------------------------------------*/

#define	_is_prm_error(			/* ret:  ﾊﾟﾗﾒｰﾀ･ｴﾗｰ or -1	*/\
		MODE,			/* arg1: I2NET:0/ASYNC:1	*/\
		cmdr_p			/* arg3: pointer to CMDR	*/\
		)			\
	(MODE) ?\
	(_inc_getc(& cmdr_p ->pmaeflg)):\
	(_inc_getc(& cmdr_p ->pmeflg))

/*------------------------------------------------------*/
/* Function:						*/
/*	I2NETCへ送信ﾘｸｴｽﾄ･ｺﾏﾝﾄﾞを発行する		*/
/*------------------------------------------------------*/

#define	_set_tx_req(			/* ret:  0 or -1		*/\
		cmdr_p			/* arg1: pointer to CMDR	*/\
		)			\
	_inc_bitwr(TX_REQ, _SET, & cmdr_p ->macflg)


int	_inc_getc(void *);
int	_inc_putc(int, void *);
void *	_inc_fmemcpy(void *, void *, int);
int	_inc_bitrd(unsigned char, void *);
int	_inc_bitwr(unsigned char, int, void *);

#endif	/* I2NET_H */

