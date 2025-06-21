/*----------------------------------------------------------------------*
 * File name	: sciopn.c		/ sci open								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/sciopn.c 1.1 1996/01/18 21:49:50 kabumoto Exp $
 *$Log: sciopn.c $
 * ���r�W���� 1.1  1996/01/18  21:49:50  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		SCI���I�[�v������B												*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		channel		: ���٥���ް										*/
/*		mode		: �����Ӱ��											*/
/*																		*/
/* �߂�l																*/
/*		int			: 0													*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>						/* Show xlib			*/
#include	<drive\iofsys.h>					/*	"					*/
#include	"scictl.h"							/* Show current			*/

int
sci_open(
	int		ch,
	int		mode
	)
{
	sci_mode = mode;
	sctx[ch].putp = sctx[ch].getp = sctx[ch].lcnt = 0;
	scrx[ch].putp = scrx[ch].getp = scrx[ch].lcnt = 0;
	iodrv_read[__SCI_-1] = sci_rx;
	iodrv_write[__SCI_-1] = sci_tx;
	iodrv_close[__SCI_-1] = sci_close;
	return(0);
}
