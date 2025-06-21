/*----------------------------------------------------------------------*
 * File name	: scitx.c		/ sci tx								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/scitx.c 1.1 1996/01/19 14:45:18 kabumoto Exp $
 *$Log: scitx.c $
 * ���r�W���� 1.1  1996/01/19  14:45:18  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                                 		 		*/
/*		SCI�ɑ΂��đ��M���s���B�{���W���[����write�֐��̃R�[���[		*/
/*		���̊֐��ƂȂ��Ă���B											*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		channel		: ���٥���ް										*/
/*		*u_buf		: string buffer										*/
/*		len			: number of byte									*/
/*																		*/
/* �߂�l																*/
/*		int			: 0	��� ����I��									*/
/*----------------------------------------------------------------------*/

#include	<string.h>								/* ANSI std			*/
#include	"scictl.h"								/* Show current		*/


int
sci_tx(
	int				channel,					/* channel number		*/
	unsigned char	*u_buf,						/* string buffer pointer*/
	int				len							/* number of byte		*/
	)
{
	if(len)  return( sci_io_write(channel, u_buf, len));
	else	 return( sci_io_puts(channel, u_buf));
}

/*----------------------------------------------------------------------*/
/*		sci_io_write													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*		len			: number of byte									*/
/*																		*/
/* �߂�l																*/
/*		int			: n	��� ���������޲Đ�							*/
/*					 -1	��� system error								*/
/*					 -2	��� �󂫴ر���Ȃ�								*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		�X�g�����O�X�������O�o�b�t�@�ɏ������݁A���M���C�l�[�u��		*/
/*		�ɂ���B�Ȃ����M���łȂ��ꍇ��1ST�L�����N�^�̏������݂�			*/
/*		�s���B															*/
/*----------------------------------------------------------------------*/

int
sci_io_write(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* number of byte			*/
	)
{
	unsigned char		c, *rp;				/* ring buffer���߲���		*/
	struct comh			*p;					/* ���M�Ǘ�ͯ�ނւ��߲��	*/
	unsigned short		i, j;
	int					err, n;

	p = &sctx[ch];
	if(len == 0 || (n= len)> p->buf_max)				return(-1);
	i= j= p->putp;
	if((n +((i- p->getp)& p->buf_max)) >= p->buf_max)	return(-2);
	rp = p->buff;
	do {
		*(rp + i) = *(u_buf++);
		i = ++i & p->buf_max;
	} while(--n);
	p->putp = i;
	sci_io_tx(ch, p);
	return(len);
}

/*----------------------------------------------------------------------*/
/*		sci_io_puts														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*																		*/
/* �߂�l																*/
/*		int			: n	��� ���������޲Đ�							*/
/*					 -1	��� error										*/
/*					 -2	��� �󂫴ر���Ȃ�								*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		�X�g�����O�X�ɃX�^�[�g��t���O/�G���h��t���O�t�����A�����O		*/
/*		�o�b�t�@�ɏ������݂��s������A���M���C�l�[�u���ɂ���B			*/
/*		�Ȃ����M���łȂ��ꍇ��1ST�L�����N�^�̏������݂��s���B			*/
/*----------------------------------------------------------------------*/

int
sci_io_puts(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char		c, *rp;				/* ring buffer���߲���		*/
	struct comh			*p;					/* ��M�Ǘ�ͯ�ނւ��߲��	*/
	int					len;				/* number of byte			*/
	unsigned short		i, j;
	int					err, n;

	p = &sctx[ch];
	if((len = strlen((char *)u_buf))== 0 ||
	   (n= len) > p->buf_max-2)								return(-1);
	i= j= p->putp;
	if(((n+2)+((i- p->getp)& p->buf_max)) >= p->buf_max)	return(-2);
	rp = p->buff;
	*(rp + i) = LF,	i = ++i & p->buf_max;
	do {
		*(rp + i) = *(u_buf++);
		i = ++i & p->buf_max;
	} while(--n);
	*(rp + i) = CR,	 i = ++i & p->buf_max;
	p->putp = i;
	sci_io_tx(ch, p);
	return(len);
}
