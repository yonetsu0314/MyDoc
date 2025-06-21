/*----------------------------------------------------------------------*
 * File name	: jformat.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/jformat.c 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: jformat.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/05/07  17:21
 * 未定義の関数jstrlenを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* Copyright (C) 1981,1982,1983 by Manx Software Systems */
#include	<ctype.h>
#include	<string.h>

#define _Z      (unsigned char)0x00    /* ascii */
#define _K      (unsigned char)0x01    /* Kana moji */
#define _KP     (unsigned char)0x02    /* Kana punct */
#define _J1     (unsigned char)0x04    /* Kanji 1st byte */
#define _J2     (unsigned char)0x08    /* Kanji 2nd byte */
#define _J2K    (unsigned char)(_J2|_K)
#define _J21    (unsigned char)(_J2|_J1)
#define _J2KP   (unsigned char)(_J2|_KP)

unsigned char    _jctype[] = {
        /*          0       1       2       3       4       5       6       7 */
        /*-----------+-------+-------+-------+-------+-------+-------+--------*/
        /*00*/     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
                   _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
        /*10*/     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
                   _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
        /*20*/     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
                   _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
        /*30*/     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
                   _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,     _Z,
        /*40*/    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
                  _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
        /*50*/    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
                  _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
        /*60*/    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
                  _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
        /*70*/    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,
                  _J2,    _J2,    _J2,    _J2,    _J2,    _J2,    _J2,     _Z,
        /*80*/    _J2,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
                 _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
        /*90*/   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
                 _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
        /*A0*/    _J2,  _J2KP,  _J2KP,  _J2KP,  _J2KP,  _J2KP,   _J2K,   _J2K,
                 _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
        /*B0*/   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
                 _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
        /*C0*/   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
                 _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
        /*D0*/   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
                 _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,   _J2K,
        /*E0*/   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
                 _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
        /*F0*/   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,   _J21,
                 _J21,   _J21,   _J21,   _J21,   _J21,     _Z,     _Z,     _Z,
};

int
jstrlen(
	const unsigned char *s
	)
{
        int	n;

        for (n = 0; *s; n++, s++) {
                if (_jctype[*s] & _J1) {
                        if (*++s == '\0')
                                break;
                }
        }
        return(n);
}


static char *
_fmtcvt(
	int	*ap,
	int	base,
	char	*cp,
	int	len
	)
{
	long val;
	static char digits[]="0123456789abcdef";

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
	} while ((val /= base) != 0);
	if (len)
		*--cp = (char)'-';
	return cp;
}

int
jformat(
	int	(*putsub)(),
	char	*fmt,
	char	*argp
	)
{
	int c;
	union {
		int *ip;
		char *cp;
		char **cpp;
#ifdef FLOAT
		double *dp;
#endif
	} args; 
	int charcount;
	int rj, fillc;
	int maxwidth, width;
	int i, k;
	char *cp;
	auto char s[200];

	charcount = 0;
	args.cp = argp;
	while ( c = *fmt++ ) {
		if ( c == '%' ) {
			s[14] = (char)0;
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
				width = *args.ip++;
				c = *fmt++;
			} else {
				for (width = 0 ; isdigit(c) ; c = *fmt++)
					width = width*10 + c - '0';
			}
			if ( c == '.' ) {
				if ((c = *fmt++) == '*') {
					maxwidth = *args.ip++;
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
				cp = _fmtcvt(args.ip, k, s+14, i);
				args.cp += i;
				break;

			case 's':
				cp = *args.cpp++;
				i = jstrlen((unsigned char *)cp);
				goto havelen;
#ifdef FLOAT
		/*-------------------------------------
			case 'e':
			case 'g':
		--------------------------------------*/
			case 'f':
				ftoa(*args.dp++, s, maxwidth==10000?6:maxwidth, c-'e');
				i = jstrlen(cp = s);
				maxwidth = 200;
				goto havelen;
#endif

			case 'c':
				c = *args.ip++;
			default:
				*(cp = s+13) = (char)c;
				break;
			}

			i = (s+14) - cp;
		havelen:
			if ( i > maxwidth )
				i = maxwidth;
			
			if ( rj ) {
				if ((*cp == '-' || *cp == '+') && fillc == '0') {
					--width;
					if ((*putsub)(*cp++) == -1)
						return -1;
				}
				for (; width-- > i ; ++charcount)
					if ((*putsub)(fillc) == -1)
						return -1;
			}
			for ( k = 0 ; *cp && k < maxwidth ; ++k )
				if ((*putsub)(*cp++) == -1)
					return -1;
			charcount += k;
			
			if ( !rj ) {
				for (; width-- > i ; ++charcount)
					if ((*putsub)(' ') == -1)
						return -1;
			}
		} else {
			if ((*putsub)(c) == -1)
				return -1;
			++charcount;
		}
	}
	return charcount;
}

