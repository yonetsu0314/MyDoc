/*----------------------------------------------------------------------*
 * File name	: ftoa.c	/ 					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/ftoa.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: ftoa.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*
	ftoa.c
*/
static double round[] = {
	1.0e+0,
	0.5e+0,
	0.5e-1,
	0.5e-2,
	0.5e-3,
	0.5e-4,
	0.5e-5,
	0.5e-6,
	0.5e-7,
	0.5e-8,
	0.5e-9,
	0.5e-10,
	0.5e-11,
	0.5e-12,
	0.5e-13,
	0.5e-14,
	0.5e-15,
	0.5e-16,
	0.5e-17,
	0.5e-18,
};

void
ftoa(
	double	number,
	char	*buffer,
	int	maxwidth,
	int	flag
	)
{
	int i, exp, digit, decpos, ndig;

	ndig = maxwidth+1;
	exp = 0;
	if (number < 0.0) {
		number = -number;
		*buffer++ = (char)'-';
	}
	if (number > 0.0) {
		while (number < 1.0) {
			number *= 10.0;
			--exp;
		}
		while (number >= 10.0) {
			number /= 10.0;
			++exp;
		}
	}

	if (flag == 2) {		/* 'g' format */
		ndig = maxwidth;
		if (exp < -4 || exp >= maxwidth)
			flag = 0;		/* switch to 'e' format */
	} else if (flag == 1)	/* 'f' format */
		ndig += exp;

	if (ndig >= 0) {
		if ((number += round[ndig>16?16:ndig]) >= 10.0) {
			number = 1.0;
			++exp;
			if (flag)
				++ndig;
		}
	}

	if (flag) {
		if (exp < 0) {
			*buffer++ = (char)'0';
			*buffer++ = (char)'.';
			i = -exp - 1;
			if (ndig <= 0)
				i = maxwidth;
			while (i--)
				*buffer++ = (char)'0';
			decpos = 0;
		} else {
			decpos = exp+1;
		}
	} else {
		decpos = 1;
	}

	if (ndig > 0) {
		for (i = 0 ; ; ++i) {
			if (i < 16) {
				digit = (int)number;
				*buffer++ = (char)(digit+'0');
				number = (number - digit) * 10.0;
			} else
				*buffer++ = (char)'0';
			if (--ndig == 0)
				break;
			if (decpos && --decpos == 0)
				*buffer++ = (char)'.';
		}
	}

	if (!flag) {
		*buffer++ = (char)'e';
		if (exp < 0) {
			exp = -exp;
			*buffer++ = (char)'-';
		} else
			*buffer++ = (char)'+';
		if (exp >= 100) {
			*buffer++ = (char)(exp/100 + '0');
			exp %= 100;
		}
		*buffer++ = (char)(exp/10 + '0');
		*buffer++ = (char)(exp%10 + '0');
	}
	*buffer = (char)0;
}
