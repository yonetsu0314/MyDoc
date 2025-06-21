/*----------------------------------------------------------------------*
 * File name	: scan.c		/ romable scan							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/scan.c 1.1 1996/01/19 09:49:13 kabumoto Exp $
 *$Log: scan.c $
 * ���r�W���� 1.1  1996/01/19  09:49:13  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*				Copyright (C) 1982, 1984 by Manx Software Systems		*/
/*----------------------------------------------------------------------*/
#include <ctype.h>

#define EOF	-1

static int maxwidth;
static int (*gsub)();
char *strchr();

scanfmt(getsub, fmt, args)
int (*getsub)(); register char *fmt; register int **args;
{
#ifdef FLOAT
	double atof();
#endif
	long lv;
	register int c, count, base, cc;
	char suppress, lflag, widflg;
	char *cp;
	auto char tlist[130];
	static char list[] = "ABCDEFabcdef9876543210";
	static char vals[] = {
			10,11,12,13,14,15,10,11,12,13,14,15,9,8,7,6,5,4,3,2,1,0
	};

	count = 0;
	gsub = getsub;
	while (c = *fmt++) {
		if (c == '%') {
			widflg = lflag = suppress = 0;
			maxwidth = 127;
			if (*fmt == '*') {
				++fmt;
				suppress = 1;
			}
			if (isdigit(*fmt)) {
				maxwidth = 0;
				do {
					maxwidth = maxwidth*10 + *fmt - '0';
				} while (isdigit(*++fmt));
				widflg = 1;
			}
			if (*fmt == 'l') {
				lflag = 1;
				++fmt;
			}
	
			switch (cc = *fmt++) {
			case '%':
				c = '%';
				goto matchit;
			case 'h':			/* specify short (for compatibility) */
				lflag = 0;
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
				lflag = 1;
				tlist[0] = ' ';
				tlist[1] = '\t';
				tlist[2] = '\n';
				tlist[3] = 0;
	string:
				if (skipblank())
					goto stopscan;
	charstring:
				if (!suppress)
					cp = (char *)*args++;
				widflg = 0;
				while (maxwidth--) {
					if ((c = (*gsub)(0)) == EOF)
						break;
					if (lflag ? (strchr(tlist,c)!=0) : (strchr(tlist,c)==0)) {
						(*gsub)(1);	/* unget last character */
						break;
					}
					if (!suppress)
						*cp++ = c;
					widflg = 1;
				}
				if (!widflg)
					goto stopscan;
				if (!suppress) {
					if (cc != 'c')
						*cp = 0;
					++count;
				}
				break;

			case 'c':
				if (!widflg)
					maxwidth = 1;
				tlist[0] = 0;
				lflag = 1;
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

skipblank()
{
	while (isspace((*gsub)(0)))
		;
	if ((*gsub)(1) == EOF)
		return EOF;
	return 0;
}

#ifdef FLOAT
getflt(buffer)
char *buffer;
{
	register char *cp;
	register int c;
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

getnum(list, values, base, valp)
char *list; char *values; long *valp;
{
	register char *cp;
	register int c, cnt;
	long val;
	int sign;

	if (maxwidth <= 0)
		return 0L;
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

