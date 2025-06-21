/*----------------------------------------------------------------------*
 * File name	: icu.h		/ icu bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/icu.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: icu.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/02/06 19:15
 * ベクターの関数のポインタ配列数を128にする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(ICU_H)
#define	ICU_H

enum {
	ICU_CHP,			/* [0]:low-byte(channel ptn)	*/
	ICU_DVN,			/* [1]:high-byte(device number)	*/
	ICU_ARY_MAX
};

/*------------------------------------------------------*/
/*	割り込みハンドラのベクター･テーブル		*/
/*							*/
/*------------------------------------------------------*/

#if	!defined(V_FUNC_T)
#define		V_FUNC_T
typedef void	(*V_FUNC)();

typedef union	uci {
	unsigned char	uc[ICU_ARY_MAX];
	short		i;		/* return parameter		*/
} UCI;
#endif

typedef struct	vects {			/* APL vector table		*/
	int	no;			/* 割り込みﾊﾝﾄﾞﾗの数		*/
	V_FUNC	fun[128];		/* 関数へのﾎﾟｲﾝﾀの配列		*/
} VECTS;

#if	!defined(STORAGE)
extern const struct vects	vector_strc;
#endif

int		_icu_dev_hunt(V_FUNC);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>	       					*/

enum icudn {
	MAS_ICU,			/* Master ICU device number	*/
	SLV_ICU0,			/* Slave ICU device number	*/
	SLV_ICU1,			/* Slave ICU device number	*/
	SLV_ICU2,			/* Slave ICU device number	*/
	SLV_ICU3,			/* Slave ICU device number	*/
	SLV_ICU4,			/* Slave ICU device number	*/
	SLV_ICU5,			/* Slave ICU device number	*/
	SLV_ICU6,			/* Slave ICU device number	*/
	SLV_ICU7			/* Slave ICU device number	*/
};

/*	<<< argument 2 >>>	       					*/

#define	INTP0		(0x1 << 0)	/* ICU INTP0			*/
#define	INTP1		(0x1 << 1)	/* ICU INTP1			*/
#define	INTP2		(0x1 << 2)	/* ICU INTP2			*/
#define	INTP3		(0x1 << 3)	/* ICU INTP3			*/
#define	INTP4		(0x1 << 4)	/* ICU INTP4			*/
#define	INTP5		(0x1 << 5)	/* ICU INTP5			*/
#define	INTP6		(0x1 << 6)	/* ICU INTP6			*/
#define	INTP7		(0x1 << 7)	/* ICU INTP7			*/

int 	_dis_icu(enum icudn, int);
int 	_ena_icu(enum icudn, int);

int 	_get_ipfw(enum icudn);

#endif	/* ICU_H */

