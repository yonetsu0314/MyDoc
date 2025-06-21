/*----------------------------------------------------------------------*
 * File name	: seek.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/seek.c 1.1 1996/01/20 12:06:40 kabumoto Exp $
 *$Log: seek.c $
 * ���r�W���� 1.1  1996/01/20  12:06:40  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����																	*/
/*		�K�i�|UNIX														*/
/*		�w�肵��fd�ɑΉ�����t�@�C���̃t�@�C����|�C���^���Aorigin		*/
/*		�Ɏw�肵���ʒu����offset�o�C�g���ꂽ�ʒu�Ɉړ�����B			*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		fd	: �Ώ�̧�ق������											*/
/*		offset	: origin������޲Đ�									*/
/*		origin	: �ړ��̊�̈ʒu										*/
/*																		*/
/* �߂�l																*/
/*		int		: N		��� �ړ���̈ʒu���擪���牽�o�C�g�����		*/
/*				  			���邩										*/
/*				  -N	��� error										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>								/* ANSI std			*/
#include	<fcntl.h>
#include	<rom\handle.h>
#include	<drive\iofsys.h>

int
seek(
	int		fd,
	int		offset,
	int		origin
	)
{
	int		ch, i;

	ch = fd & 0xff;
	if(((i= ((fd & 0xff00) >> 8)-1) < 0) || (i >= __FILE_KEY_MAX_) ||
	  (iodrv_seek[i] == NULL))							return(-1);
	return(iodrv_seek[i](ch, offset, origin));
}
