/*----------------------------------------------------------------------*
 * File name	: e2srdo.c	/ read E2SROM DO							*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/e2srom/RCS/e2srdo.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2srdo.c $
 * rW 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/12 xN^[ΤπPIO©ηE2SROMΙΟX
 * 
 * rW 1.1  1970/01/01  00:00:00  sakaguti
 * VAqnlh^e
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* ΰΎ                                                  				*/
/*	dQrqnlΜcnπόΝ·ιB										*/
/*----------------------------------------------------------------------*/
/* ψ«																*/
/*	int	: ΓήΚή²½₯ΕέΚή°													*/
/*	int	: Ξί°Δ₯Α¬έΘΩ													*/
/*																		*/
/* ίθl																*/
/*	unsigned char	: port data											*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif						*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE	*/

unsigned char
_eesrom_handle_do(
	int	devno,					/* device number	*/
	int	port_ch					/* port channel		*/
	)
{
	return ((unsigned char)trapa_svc(VECTOR_E2SROM, 0x3, devno, port_ch));
}
