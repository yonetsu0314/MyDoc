/*----------------------------------------------------------------------*
 * File name	: scan.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/scan.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scan.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/05/29 12:30
 * scanfmtのプロトタイプのミスマッチを修正する。
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
/* Copyright (C) 1982, 1984 by Manx Software Systems */
#include <ctype.h>
#include <string.h>

#define EOF	-1

static int maxwidth;
static int (*gsub)();
/*char *strchr();*/

int skipblank(void)
{
	while (isspace((*gsub)(0)))
		;
	if ((*gsub)(1) == EOF)
		return EOF;
	return 0;
}

int getnum(char *list, char *values, int base, long *valp)
{
	char *cp;
	int c, cnt;
	long val;
	int sign;

	if (maxwidth <= 0)
		return 0;
	val = cnt = sign = 0;
	if ((c = (*gsub)(0)) == '-') {
		sign = 1;
		++cnt;
	} else if (c == '+')
		++cnt;
	else
		(*gsub)(1);

	for ( ; cnt < maxwidth ; ++cnt) {
		if ((cp = strchr(list, c = (*gsub)(0))) == 0) {
			if (base == 16 && val == 0 && (c=='x' || c=='X'))
				continue;
			(*gsub)(1);
			break;
		}
		val *= base;
		val += values[cp-list];
	}
	if (sign)
		*valp = -val;
	else
		*valp = val;
	return cnt;
}

int scanfmt(
	int	(*getsub)(),
	char	*fmt,
	int	**args
	)
{
#ifdef FLOAT
	double atof();
#endif
	long lv;
	int c, count, base, cc;
	char suppress, lflag, widflg;
	char *cp;
	auto char tlist[130];
	static char list[] = "ABCDEFabcdef9876543210";
	static char vals[] = {
			(char)10, (char)11, (char)12, (char)13,
			(char)14, (char)15, (char)10, (char)11,
			(char)12, (char)13, (char)14, (char)15,
			(char)9,  (char)8,  (char)7,  (char)6,
			(char)5,  (char)4,  (char)3,  (char)2,
			(char)1,  (char)0
	};

	count = 0;
	gsub = getsub;
	while (c = *fmt++) {
		if (c == '%') {
			widflg = lflag = suppress = (char)0;
			maxwidth = 127;
			if (*fmt == '*') {
				++fmt;
				suppress = (char)1;
			}
			if (isdigit(*fmt)) {
				maxwidth = 0;
				do {
					maxwidth = maxwidth*10 + *fmt - '0';
				} while (isdigit(*++fmt));
				widflg = (char)1;
			}
			if (*fmt == 'l') {
				lflag = (char)1;
				++fmt;
			}
	
			switch (cc = *fmt++) {
			case '%':
				c = '%';
				goto matchit;
			case 'h':			/* specify short (for compatibility) */
				lflag = (char)0;
				goto decimal;
			case 'd':
	decimal:
				c = 12;
				base = 10;
				goto getval;

			case 'x':
				c = 0;
				base = 16;
	getval:
				if (skipblank())
					goto stopscan;
				if (getnum(&list[c], &vals[c], base, &lv) == 0)
					goto stopscan;
				if (!suppress) {
					if (lflag)
						*(long *)(*args++) = lv;
					else
						**args++ = lv;
					++count;
				}
				break;

#ifdef FLOAT
			case 'f':
				if (skipblank())
					goto stopscan;
				if (getflt(tlist))
					goto stopscan;
				if (!suppress) {
					if (lflag)
						*(double *)(*args++) = atof(tlist);
					else
						*(float *)(*args++) = atof(tlist);
					++count;
				}
				break;
#endif
/*----
			case '[':
				lflag = 0;
				if (*fmt == '^' || *fmt == '~') {
					++fmt;
					lflag = 1;
				}
				for (cp = tlist ; (c = *fmt++) != ']' ; )
					*cp++ = c;
				*cp = 0;
				goto string;
----*/
			case 's':
				lflag    = (char)1;
				tlist[0] = (char)' ';
				tlist[1] = (char)'\t';
				tlist[2] = (char)'\n';
				tlist[3] = (char)0;
	string:
				if (skipblank())
					goto stopscan;
	charstring:
				if (!suppress)
					cp = (char *)*args++;
				widflg = (char)0;
				while (maxwidth--) {
					if ((c = (*gsub)(0)) == EOF)
						break;
					if (lflag ? (strchr(tlist,c)!=0) : (strchr(tlist,c)==0)) {
						(*gsub)(1);	/* unget last character */
						break;
					}
					if (!suppress)
						*cp++ = (char)c;
					widflg = (char)1;
				}
				if (!widflg)
					goto stopscan;
				if (!suppress) {
					if (cc != 'c')
						*cp = (char)0;
					++count;
				}
				break;

			case 'c':
				if (!widflg)
					maxwidth = 1;
				tlist[0] = (char)0;
				lflag = (char)1;
				goto charstring;
			}
		} else if (isspace(c)) {
			if (skipblank())
				goto stopscan;
		} else {
matchit:
			if ((*gsub)(0) != c) {
				(*gsub)(1);
				goto stopscan;
			}
		}
	}

stopscan:
	if (count == 0) {
		if ((*gsub)(0) == EOF)
			return EOF;
		(*gsub)(1);
	}
	return count;
}

#ifdef FLOAT
getflt(buffer)
char *buffer;
{
	char *cp;
	int c;
	char decpt, sign, exp;

	sign = exp = decpt = 0;

	for (cp = buffer ; maxwidth-- ; *cp++ = c) {
		c = (*gsub)(0);
		if (!isdigit(c)) {
			if (!decpt && c == '.')
				decpt = 1;
			else if (!exp && (c == 'e' || c == 'E') && cp != buffer) {
				sign = 0;
				exp = decpt = 1;
				continue;
			} else if (sign || (c != '-' && c != '+')) {
				(*gsub)(1);
				break;
			}
		}
		sign = 1;
	}
	*cp = 0;
	return cp==buffer;
}
#endif

