/*----------------------------------------------------------------------*
 * File name	: scirx.c		/ sci rx								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/scirx.c 1.2 1996/01/20 10:52:32 kabumoto Exp $
 *$Log: scirx.c $
 * ���r�W���� 1.2  1996/01/20  10:52:32  kabumoto
 * ���荞�ݐ����CCR��I����ASCI��SCR��RIE�ɕύX
 * 
 * ���r�W���� 1.1  1996/01/18  21:50:10  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                                	  				*/
/*		��M�����O�o�b�t�@�̓��e���A�w��o�C�g���܂��́A�e�L�X�g�P��	*/
/*		�Ń��[�U��o�b�t�@�ɃR�s�[����Bscirx�֐���read�֐��̃R�[���[	*/
/*		���ƂȂ��Ă���B												*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		channel		: ���٥���ް										*/
/*		*u_buf		: string buffer										*/
/*		len 		: length											*/
/*																		*/
/* �߂�l																*/
/*		int			: 0   ��� ���ݸޖ���								*/
/*					  n   ��� character length							*/
/*					 -1   ��� error										*/
/*----------------------------------------------------------------------*/

#include	<custom\custom.h>					/* show custom			*/
#include	"scictl.h"							/* Show current			*/
#include	"scictxt.h"							/*	"					*/


int
sci_rx(
	int				channel,					/* channel number		*/
	unsigned char	*u_buf,						/* string buffer pointer*/
	int				len							/* number of byte		*/
	)
{
	if(len)  return( sci_io_read(channel, u_buf, len));
	else	 return( sci_io_gets(channel, u_buf));
}

/*----------------------------------------------------------------------*/
/*		sci_io_read														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		ch			: channel no.										*/
/*		*u_buf  	: user buffer pointer								*/
/*		len 		: length											*/
/*																		*/
/* �߂�l																*/
/*		int			: no of read byte									*/
/*																		*/
/* ����																	*/
/*		��M�����O�o�b�t�@�̓��e���A�w��o�C�g��(len)���[�U�o�b�t�@		*/
/*		�ɃR�s�[���R�s�[�����o�C�g����Ԃ��B�w��o�C�g���̃R�s�[�I		*/
/*		���O�ɁA�o�b�t�@���e���R�s�[���I�����ꍇ(getp == putp)�A��		*/
/*		�̎��_�Œ��f����B												*/
/*----------------------------------------------------------------------*/

int
sci_io_read(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf,					/* string buffer pointer	*/
	int				len						/* length					*/
	)
{
	unsigned char		*rp;				/* ring buffer���߲���		*/
	struct comh			*p;					/* ��M�Ǘ�ͯ�ނւ��߲��	*/
	unsigned short		i;
	int					err, n;

	p = &scrx[ch];
	if(p->getp == p->putp)								return(0);
	n = len;
	rp = p->buff;
	i = p->getp;
	do {
		if((*(u_buf++) = *(rp + i)) == CR)  p->lcnt--;
		i = ++i & p->buf_max;
		if(i == p->putp)  break;
	} while(--n);
	p->getp = i;
	return(len - n);
}

/*----------------------------------------------------------------------*/
/*		sci_io_gets														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		ch			: channel no.										*/
/*		*u_buf		: string buffer										*/
/*																		*/
/* �߂�l																*/
/*		int			: 0   ��� ���ݸޖ���								*/
/*					  n   ��� character length							*/
/*					 -1   ��� error										*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		��M�����O�o�b�t�@�̓��e���A�^�[�~�l�[�^(CR)�P�ʂŃ��[�U�o		*/
/*		�b�t�@�ɃR�s�[��NULL��t������B���̊֐����ŃX�^�[�g��t���O		*/
/*		�y�уG���h��t���O��������邽�߁A��M�����t���[���̃o�C�g��		*/
/*		���-2���ꂽ�l���Ԃ����B										*/
/*----------------------------------------------------------------------*/

int
sci_io_gets(
	int				ch,						/* channel no.				*/
	unsigned char	*u_buf					/* string buffer pointer	*/
	)
{
	unsigned char		*rp;				/* ring buffer���߲���		*/
	struct comh			*p;					/* ��M�Ǘ�ͯ�ނւ��߲��	*/
	unsigned short		i;
	unsigned char		c, f;
	int					err, n =0, len;
	volatile struct st_sci*	cp;

	p = &scrx[ch];
	if(p->lcnt == 0)									return(0);
	rp = p->buff;
	i = p->getp;
	do {
		c = *(rp + i);
		i = ++i & p->buf_max;
		if(c == LF) {
			++n;
			continue;
		}
		else if(c == CR) {
			*u_buf = '\0';
			len = ((i- p->getp)& p->buf_max) -(n+1);
			p->getp = i;
			p->lcnt --;									return(len);
		}
		*(u_buf ++) = c;
	} while(i != p->putp);

	(cp = sci_ctxt_tbl[ch])->SCR.BIT.RIE = 0;
	p->getp = i;
	p->lcnt = 0;
	cp->SCR.BIT.RIE = 1;
	return(-1);
}
