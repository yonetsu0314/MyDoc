/*----------------------------------------------------------------------*
 * File name	: mst012.c	/ set monitor text			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mst012.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mst012.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mst012.c 1.1 1996/07/22 19:25:51
 *Log: mst012.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<custom\custom.h>		/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

/*----------------------------------------------------------------------*/
/*	mon_span_text							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｽﾊﾟﾝ調整完了のﾃｷｽﾄを作成する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf	: pointer to string buffer				*/
/*	*argl[]	: pointer to argument[]					*/
/*		      argl[0]	･･･ unsigned long *			*/
/*				    実行結果ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ[2]へのﾎﾟｲﾝﾀ	*/
/*		      argl[1]	･･･ unsigned short *			*/
/*				    ｽﾊﾟﾝ係数[計量器+AFV数]へのﾎﾟｲﾝﾀ	*/
/*									*/
/* 戻り値								*/
/*	char *	: ﾃｷｽﾄの最後のｱﾄﾞﾚｽ					*/
/*----------------------------------------------------------------------*/

union zp {
	unsigned long	lp;
	unsigned short	us[sizeof(DAD_ARY_PTN)/sizeof(short)];
};

char *
mon_span_text(
	char		*buf,			/* pointer to str buffer*/
	void		*argl[]			/* pointer to argument[]*/
	)
{
	union zp	*bpp;
	char		*cp, *p;
	unsigned short	k;
	int		len, i, j, m;

	bpp = argl[0];
	len = strlen(buf);
	cp = (char *)(buf+len);
	*(cp++) = (char)'\n';
	*(cp++) = (char)'\r';
	m = 2;
	do {
		for(j= sizeof(union zp)/sizeof(bpp->us[0])-1 ; j >= 0 ; j--) {
			*(cp++) = (char)'\t';
			for(i= 0, k= (unsigned short)0x8000; i < 16 ; i++, k >>= 1) {
				if(bpp->us[j] & k)  *(cp++) = (char)'1';
				else		    *(cp++) = (char)'0';
			}
		}
		*(cp++) = (char)'\n';
		*(cp++) = (char)'\r';
		bpp++;
	} while(--m);

	*(cp++) = (char)'\n';
	*(cp++) = (char)'\r';
	p = argl[1];
	i = 8;
	for(m =0 ; m < DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX ; m++) {
		if(i == 8)  *(cp++) = (char)'\t';
		itoh(*(unsigned short *)p, cp);
		p += 2;
		cp += 4;
		if(--i) *(cp++) = (char)' ';
		else {
			*(cp++) = (char)'\n';
			*(cp++) = (char)'\r';
			i = 8;
		}
	}
	*cp = (char)'\0';
	return(cp);
}
