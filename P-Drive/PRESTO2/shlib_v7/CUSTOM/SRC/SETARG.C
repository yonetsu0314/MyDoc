/*----------------------------------------------------------------------*
 * File name	: setarg.c	/ 					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/setarg.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: setarg.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	bufで示されるバッファ内容のストリングスを、引き数のセパレー	*/
/*	タのコード" "で分け複数のストリングスに変換する、その結果と	*/
/*	してストリングスへのポインタ(*argv)を設定し、分割した数(arg	*/
/*	c)を返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf		: ｽﾄﾘﾝｸﾞｽ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ			*/
/*	max		: argcの最大値					*/
/*	*argv[]		: ﾎﾟｲﾝﾀの配列へのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: argc	･･･ 引き数の数(>=1)			*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>
#include	<string.h>

static const char	*s = " \t";		/* terminate string	*/

int
setarg(
	char		*buf,			/* pointer to sirng buf	*/
	int		max,			/* maximun argc		*/
	char		*argv[]			/* pointer to argv[]	*/
	)
{
	int		argc =0;		/* no of argument	*/
	char		*cp, *cp1;		/* current string	*/
	char		*pp;			/* privious string	*/
	int		len;

	if((len = strlen( buf)) <= 0)  return(-1);
	if(max == 0)  max = (len >> 1);
	pp = cp = buf;
	while((cp = strpbrk( cp, s))!= NULL) {
		*(cp++) = (char)'\0';
		if((cp - pp) <= 1)  {
			cp1 = cp;
			do {
				cp = cp1;
				if((cp1 = strpbrk( cp1, s))== NULL) {
					argv[++argc] = cp;
					goto end;
				}
				*(cp1++) = (char)'\0';
			} while((cp1 - cp) <= 1);
			pp = cp;
			cp = cp1;
		}
		argv[++argc] = pp;
		if(argc >= max)    goto end;
		pp = cp;
	}
	argv[++argc] = pp;
end:
	if(*argv[argc]== '\0' || strchr(s, (int)*argv[argc])!= NULL)  --argc;
	if(argc == 0)  return(-1);
	return(argc);
}
