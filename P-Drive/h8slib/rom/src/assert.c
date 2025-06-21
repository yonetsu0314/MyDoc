/*----------------------------------------------------------------------*
 * File name	: assert.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/assert.c 1.1 1996/01/19 15:54:11 kabumoto Exp $
 *$Log: assert.c $
 * ���r�W���� 1.1  1996/01/19  15:54:11  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����																	*/
/*		�K�i�|ANSI														*/
/*		Assertion failed: [expretion], [file name], [line number]��		*/
/*		ү���ނ��o�͂��A�{�֐����N��������۾����ްĂ���B				*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*expretion	: �װ�ƂȂ����L�q									*/
/*		*file_name	: ������۸��т�̧�٥Ȱ�								*/
/*		line_no		: ������۸��т�ײݔԍ�								*/
/*																		*/
/* �߂�l																*/
/*		void		: ����												*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>								/* ANSI std			*/

void
_assert(
	char			*expretion,
	char			*file_name,
	unsigned int	line_no
	)
{
	int			err;
	long		tim = 0x80000;

	do {
		err = printf("\n\rAssertion failed: (%s), %s, %-4d",
					expretion, file_name, line_no);
		while(tim--);
	} while(err == -2);
	while(1);
/*	exit(-1);	*/
}
