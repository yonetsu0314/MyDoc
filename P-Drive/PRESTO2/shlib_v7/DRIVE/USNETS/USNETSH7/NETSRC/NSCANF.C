/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/nscanf.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: nscanf.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* NSCANF.C  Edition: 4  Last changed: 29-Apr-98,9:16:52  By: NICK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    NSCANF.C -- Simple String Conversions for USNET

    Copyright (C) 1993 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#include	<stdarg.h>

#define GETAP(ap) \
    ap = (char *) va_arg(args,char *);

static match(char *fmt, char cc)
{
    int exc=0;
    char *cp1;

    if (!cc)
	return 0;
    if (*fmt == '^')
    {
	exc = 1;
	fmt++;
    }
    for (cp1=fmt; *cp1 && *cp1 != ']'; )
    {
	if (cp1[1] == '-' && cp1[2] > cp1[0])
	{
	    if (cc >= cp1[0] && cc <= cp1[2])
		return exc^1;
	    cp1 += 3;
	}
	else
	{
	    if (cc == *cp1)
		return exc^1;
	    cp1++;
	}
    }
    return exc;
}

static long asclng(char **cp, int width, int base)
{
    long answer;
    char cc, sign, *cp1;

    answer = sign = 0;
    for (cp1=*cp; *cp1; cp1++)
    {
	if (*cp1 > ' ')
	    break;
    }
    if (*cp1 == '-' || *cp1 == '+')
	sign = *cp1++, width--;
    if (!*cp1 || !width)
	goto exi1;
    if (!base)
    {
	base = 10;
	if (*cp1 == '0')
	{
	    base = 8;
	    goto lab4;
	}
    }
    else if (base == 16)
    {
	if (*cp1 == '0')
	{
lab4:	    cp1++, width--;
	    if (width > 0)
	    {
		if (*cp1 == 'x' || *cp1 == 'X')
		    base = 16, cp1++, width--;
	    }
	}
    }
    for ( ; width && *cp1; cp1++,width--)
    {
	if ((cc = *cp1) < '0')
	    break;
        if (cc <= '9')
	    cc &= 0xf;
        else
        {
	    cc &= 0xdf;
	    if (cc >= 'A')
	        cc -= 'A' - 10;
        }
        if (cc >= base)
	    break;
	answer = base * answer + cc;
    }
exi1:
    *cp = cp1;
    if (sign == '-')
	answer = -answer;
    return answer;
}

int Nsscanf(char *buf, char *fmt, ...)
/*
	vsscanf:	Reentrant Scan Routine
	entry:		&input buffer
			&format string
			&argument list
	exit:		return value is number of fields scanned
*/
{
    int field;		/* field flag: 0=background, 1=%field */
    int sizdef;		/* size: 0=default, 1=short, 2=long, 3=long double */
    int width;		/* field width */
    int par1;
    long l1;
    int nfields;
    char fch;
    char *orgbuf, *prebuf;
    char *ap;
    va_list args;

    va_start(args,fmt);			/* get variable arg list address */
    if (!*buf)
	return -1;
    nfields = field = sizdef = 0;
    orgbuf = buf;
    while ((fch = *fmt++) != 0)
    {
	if (!field)
	{
	    if (fch == '%')
	    {
		if (*fmt != '%')
		{
		    field = 1;
		    continue;
		}
		fch = *fmt++;
	    }
	    if (fch <= ' ')
		for (; *buf== ' '||*buf=='\t'; buf++) ;
	    else if (fch == *buf)
		buf++;
	}
	else
	{
	    width = 0x7fff;
	    if (fch == '*')
	    {
		width = va_arg(args,int);
		goto lab6;
	    }
	    else if (fch >= '0' && fch <= '9')
	    {
		fmt--;
		width = asclng(&fmt,9,10);
lab6:		fch = *fmt++;
	    }
	    if (fch == 'h')
	    {
		sizdef = 1;
		goto lab7;
	    }
	    else if (fch == 'l')
	    {
		sizdef = 2;
lab7:       	fch = *fmt++;
	    }
	    prebuf = buf;
            switch (fch)
	    {
	    case 'd':				/* signed integer */
		par1 = 10;
		goto lab3;
	    case 'o':				/* signed integer */
		par1 = 8;
		goto lab3;
	    case 'x':				/* signed integer */
	    case 'X':				/* long signed integer */
		par1 = 16;
		goto lab3;
	    case 'u':				/* unsigned integer */
	    case 'i':				/* signed integer */
		par1 = 0;
lab3:		GETAP(ap);
		l1 = asclng(&buf, width, par1);
		if (prebuf == buf)
		    break;
		if (sizdef == 2)
		    *(long *)ap = l1;
		else if (sizdef == 1)
		    *(short *)ap = l1;
		else
		    *(int *)ap = l1;
		goto lab12;
	    case 'c':				/* character */
		GETAP(ap);
		for (; width && *buf; width--)
		{
		    *ap++ = *buf++;
		    if (width == 0x7fff)
			break;
		}
		goto lab12;
	    case '[':				/* search set */
		GETAP(ap);
		for (; width && match(fmt,*buf); width--)
		    *ap++ = *buf++;
		while (*fmt++ != ']') ;
		goto lab11;
	    case 's':				/* character array */
		GETAP(ap);
		for (; *buf==' '||*buf==0x07; buf++) ;
		for (; width && *buf && *buf>' '; width--)
		    *ap++ = *buf++;
lab11:		if (prebuf == buf)
		    break;
		*(char *)ap = 0;
		goto lab12;
	    case 'n':				/* store # chars */
		GETAP(ap);
		*(int *)ap = buf - orgbuf;
		break;
	    case 'p':				/* pointer */
		GETAP(ap);
		*(long *)ap = asclng(&buf, width, 16);
lab12:		nfields++;
		break;
	    default:				/* illegal */
		goto term;
	    }
	    field = 0;
	}
	if (!fch)
	    break;
    }
term:
    return nfields;
}

