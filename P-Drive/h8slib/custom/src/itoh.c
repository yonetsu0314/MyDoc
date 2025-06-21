/*----------------------------------------------------------------------*
 * File name	: itoh.c		/ convert to ascii hex					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/custom/src/rcs/itoh.c 1.2 1996/04/04 19:20:27 minaguchi Exp $
 *$Log: itoh.c $
 * ���r�W���� 1.2  1996/04/04  19:20:27  minaguchi
 * ltoh�֐����C���e���n�̃������z�u�Ή��ł������̂����g���[���n�̃������z�u
 * �Ή��ɕύX�����B
 * 
 * ���r�W���� 1.1  1996/01/19  10:51:49  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*
#include	<custom\custom.h>
*/
static char
get_high(
	char	data
	)
{
	char	data1;
	data1 = data & 240;
	data1 = data1 >>4;
	data1 = data1 & 15;
	return(data1);
}

static char
get_low(
	char	data
	)
{
	char	data1;
	return(data & 15);
}

static char
trans_ascii(
	char	data
	)
{
	if(data <=9)	return(data+48);
	else			return(data+55);
}

/*----------------------------------------------------------------------*/
/*		itoh															*/
/*----------------------------------------------------------------------*/
/* ����                                                 		 		*/
/*		int��4byte��HEX ASCII�ɕϊ�����B								*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		i		: int value												*/
/*		*s		: pointer to string										*/
/*																		*/
/* �߂�l																*/
/*		void	: ����													*/
/*----------------------------------------------------------------------*/

itoh(i,s)
	register int	i;								/* converted value	*/
	register char	*s;
{
	s[0] = trans_ascii(get_high((char)((i >> 8) & 0xff)));
	s[1] = trans_ascii(get_low((char)((i >> 8) & 0xff)));
	s[2] = trans_ascii(get_high((char)(i & 0xff)));
	s[3] = trans_ascii(get_low((char)(i & 0xff)));
}

/*----------------------------------------------------------------------*/
/*		ctoh															*/
/*----------------------------------------------------------------------*/
/* ����                                         		         		*/
/*		char��2byte��HEX ASCII�ɕϊ�����B								*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		i		: int value												*/
/*		*s		: pointer to string										*/
/*																		*/
/* �߂�l																*/
/*		void	: ����													*/
/*----------------------------------------------------------------------*/

ctoh(
	unsigned char	i,
	register char	*s
	)
{
	s[0] = trans_ascii(get_high((char)(i & 0xff)));
	s[1] = trans_ascii(get_low((char)(i & 0xff)));
}

/*----------------------------------------------------------------------*/
/*		ltoh															*/
/*----------------------------------------------------------------------*/
/* ����                                     		             		*/
/*		long��8byte��HEX ASCII�ɕϊ�����B								*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		l		: long value											*/
/*		*s		: pointer to string										*/
/*																		*/
/* �߂�l																*/
/*		void	: ����													*/
/*----------------------------------------------------------------------*/

ltoh(
	long		l,
	char		*s
	)
{
	unsigned char	c, *p= (unsigned char *)&l, n= sizeof(l);

	do {
		c = *(p++);
		*(s++) = trans_ascii(get_high(c));
		*(s++) = trans_ascii(get_low(c));
	} while(--n);
}
