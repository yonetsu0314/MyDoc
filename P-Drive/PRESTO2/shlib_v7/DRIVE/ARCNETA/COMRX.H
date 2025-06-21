/*----------------------------------------------------------------------*
 * File name	: comrx.h	/ arc-net rx task header		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comrx.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: comrx.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comrx.h 1.1 1995/11/07 17:12:46 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(COMRX_H)
#define	COMRX_H

#if	!defined(acadr)
typedef union aap {
	void		*vfp[2];	/* void型farポインタ	*/
	char		*cfp[2];	/* char		"	*/
	int		*ifp[2];	/* int型	"	*/
	long		*lfp[2];	/* long型	"	*/
	unsigned int	ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]			/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp	vfpp[1]			/* [1]void型farポインタ	*/
#define	cfp	cfpp[1]			/* [1]char	"	*/
#define	ifp	ifpp[1]			/* [1]int型	"	*/
#define	lfp	lfpp[1]			/* [1]long型	"	*/
#endif

typedef struct	rxh {
	int		putp;		/* ﾌﾟｯﾄ･ﾎﾟｲﾝﾀｰ			*/
	int		getp;		/* ｹﾞｯﾄ･ﾎﾟｲﾝﾀｰ			*/
	unsigned int	lcnt;		/* ﾃｷｽﾄ受信回数(受信専用)	*/
	unsigned int	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰのｻｲｽﾞ		*/
} RXH;

#ifdef	STORAGE
struct rxh 	com_rxhd[COM_DEV_MAX] = {
			0,
			0,
			0,
			0
};

union aap	*com_rx_tsk[COM_DEV_MAX] = {
			(void *)0		/* RXﾀｽｸのｱｸｾｽ･ｱﾄﾞﾚｽの[]*/
};
#else
extern struct rxh 	com_rxhd[];
extern union aap	*com_rx_tsk[];
#endif

#endif	/* COMRX_H */

