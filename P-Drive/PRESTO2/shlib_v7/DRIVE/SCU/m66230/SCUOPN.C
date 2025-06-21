/*----------------------------------------------------------------------*
 * File name	: scuopn.c	/ scu open									*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scuopn.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scuopn.c 1.1 1970/01/01
 *Log: scuopn.c
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.				*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	SCUをオープンする。													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ											*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ												*/
/*																		*/
/* 戻り値																*/
/*	int		: 0															*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>						/* Show xlib			*/
#include	<intrpt.h>							/* Show xlib			*/
#include	<custom\custom.h>					/* Show custom			*/
#include	<drive\iofsys.h>					/*	"					*/
#include	"scuctl.h"							/* Show current			*/

int
m66230_open(
	int		ch,
	int		mode
	)
{
	_disable();
	m66230_mode = mode;
	m66230tx[ch].putp = m66230tx[ch].getp = m66230tx[ch].lcnt = (unsigned short)0;
	m66230rx[ch].putp = m66230rx[ch].getp = m66230rx[ch].lcnt = (unsigned short)0;
	iodrv_read[__MPSC_-1] = m66230_rx;
	iodrv_write[__MPSC_-1] = m66230_tx;
	iodrv_close[__MPSC_-1] = m66230_close;
	_enable();
	return(0);
}
