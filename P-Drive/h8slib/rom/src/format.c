/*----------------------------------------------------------------------*
 * File name	: format.c		/ romable format						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/format.c 1.1 1996/01/19 15:20:26 kabumoto Exp $
 *$Log: format.c $
 * リビジョン 1.1  1996/01/19  15:20:26  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*				Copyright (C) 1981,1982,1983 by Manx Software Systems 	*/
/*----------------------------------------------------------------------*/

#include	<stdarg.h>								/* ANSI std			*/
#include	<ctype.h>

static char *
_fmtcvt(
	int		*ap,
	int		base,
	char	*cp,
	int		len
	)
{
	register long	val;
	static char	digits[]="0123456789abcdef";

	if (len == sizeof(long))
		val = *(long *)ap;
	else if (base > 0)
		val = *(unsigned *)ap;
	else
		val = *ap;

	len = 0;
	if (base < 0) {
		base = -base;
		if ((long)val < 0) {
			val = -val;
			len = 1;
		}
	}

	do {
		*--cp = digits[(int)(val%base)];
	} while((val /= base) != 0);
	if (len)
		*--cp = '-';
	return cp;
}

/*----------------------------------------------------------------------*/
/*		format															*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		書式を解読しｐｕｔｓｕｂへ出力する								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*putsub		: 出力する関数へのﾎﾟｲﾝﾀ								*/
/*		*fmt		: 書式へのﾎﾟｲﾝﾀ										*/
/*		ap			: 可変個引数集										*/
/*																		*/
/* 戻り値																*/
/*		int			: 書き込みを行ったﾊﾞｲﾄ数							*/
/*----------------------------------------------------------------------*/

int
format(
	int			(*putsub)(int),
	char		*fmt,
	va_list		ap
	)
{
	int		c, e, i, k, charcount, rj, fillc, maxwidth, width;
	char	*cp, s[200];

	charcount = 0;
	while ( c = *fmt++ ) {
		if ( c == '%' ) {
			s[14] = 0;
			rj = 1;
			fillc = ' ';
			maxwidth = 10000;
			if ((c = *fmt++) == '-') {
				rj = 0;
				c = *fmt++;
			}
			if (c == '0') {
				fillc = '0';
				c = *fmt++;
			}
			if (c == '*') {
				width = va_arg(ap, int);
				c = *fmt++;
			} else {
				for (width = 0 ; isdigit(c) ; c = *fmt++)
					width = width*10 + c - '0';
			}
			if ( c == '.' ) {
				if ((c = *fmt++) == '*') {
					maxwidth = va_arg(ap, int);
					c = *fmt++;
				} else {
					for (maxwidth = 0 ; isdigit(c) ; c = *fmt++)
						maxwidth = maxwidth*10 + c - '0';
				}
			}
			i = sizeof(int);
			if (c == 'l') {
				c = *fmt++;
				i = sizeof(long);
			} else if (c == 'h')
				c = *fmt++;

			switch ( c ) {
		/*--------------------------------
			case 'o':
				k = 8;
				goto do_conversion;
			case 'u':
				k = 10;
				goto do_conversion;
		---------------------------------*/
			case 'x':
				k = 16;
				goto do_conversion;

			case 'd':
				k = -10;
	do_conversion:
				cp = _fmtcvt((int *)ap, k, s+14, i);
				if(i == sizeof(long))  va_arg(ap, long);
				else				   va_arg(ap, int);
				break;

			case 's':
				i = strlen(cp = va_arg(ap, char *));
				goto havelen;
#ifdef FLOAT
		/*-------------------------------------
			case 'e':
			case 'g':
		--------------------------------------*/
			case 'f':
				ftoa(va_arg(ap, double), s,
					maxwidth==10000?6:maxwidth, c-'e');
				i = strlen(cp = s);
				maxwidth = 200;
				goto havelen;
#endif

			case 'c':
				c = va_arg(ap, int);
			default:
				*(cp = s+13) = c;
				break;
			}

			i = (s+14) - cp;
		havelen:
			if ( i > maxwidth )
				i = maxwidth;
			
			if ( rj ) {
				if ((*cp == '-' || *cp == '+') && fillc == '0') {
					--width;
					if((e= (*putsub)(*cp++)) < 0)				return(e);
				}
				for (; width-- > i ; ++charcount)
					if((e= (*putsub)(fillc)) < 0)				return(e);
			}
			for ( k = 0 ; *cp && k < maxwidth ; ++k )
				if((e= (*putsub)(*cp++)) < 0)					return(e);
			charcount += k;
			
			if ( !rj ) {
				for (; width-- > i ; ++charcount)
					if((e= (*putsub)(' ')) < 0)					return(e);
			}
		} else {
			if((e= (*putsub)(c)) < 0)							return(e);
			++charcount;
		}
	}
	return(charcount);
}
