/*----------------------------------------------------------------------*
 * File name	: puts.c		/ romable puts							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/puts.c 1.1 1996/01/19 09:47:20 kabumoto Exp $
 *$Log: puts.c $
 * ���r�W���� 1.1  1996/01/19  09:47:20  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*	puts(char *s)														*/
/*		puts writes the null-terminated	strings to the stdio stream		*/
/*		and then an end-of-line sequence.								*/
/*		It returns a non-negative value, if no errors occur.			*/
/*----------------------------------------------------------------------*/

puts(s)
	register char	*s;
{
	if(cputs(s) != -1)
		{ 	putch('\n');
			putch('\r');
			return(0);
		}
	return(-1);
}
