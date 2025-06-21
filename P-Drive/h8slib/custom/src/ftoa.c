/*----------------------------------------------------------------------*
 * File name	: ftoa.c		/ 										*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/custom/src/rcs/ftoa.c 1.1 1996/01/19 10:51:34 kabumoto Exp $
 *$Log: ftoa.c $
 * リビジョン 1.1  1996/01/19  10:51:34  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/

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


ftoa(number, buffer, maxwidth, flag)
	double number; register char *buffer;
{
	register int 	i;
	int 			exp, digit, decpos, ndig;

	ndig = maxwidth+1;
	exp = 0;
	if (number < 0.0) {
		number = -number;
		*buffer++ = '-';
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

	if (flag == 2) {						/* 'g' format */
		ndig = maxwidth;
		if (exp < -4 || exp >= maxwidth)
			flag = 0;						/* switch to 'e' format */
	} else if (flag == 1)					/* 'f' format */
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
			*buffer++ = '0';
			*buffer++ = '.';
			i = -exp - 1;
			if (ndig <= 0)
				i = maxwidth;
			while (i--)
				*buffer++ = '0';
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
				*buffer++ = digit+'0';
				number = (number - digit) * 10.0;
			} else
				*buffer++ = '0';
			if (--ndig == 0)
				break;
			if (decpos && --decpos == 0)
				*buffer++ = '.';
		}
	}

	if (!flag) {
		*buffer++ = 'e';
		if (exp < 0) {
			exp = -exp;
			*buffer++ = '-';
		} else
			*buffer++ = '+';
		if (exp >= 100) {
			*buffer++ = exp/100 + '0';
			exp %= 100;
		}
		*buffer++ = exp/10 + '0';
		*buffer++ = exp%10 + '0';
	}
	*buffer = 0;
}
