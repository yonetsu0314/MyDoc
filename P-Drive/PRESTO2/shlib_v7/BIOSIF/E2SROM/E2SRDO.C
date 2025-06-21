/*----------------------------------------------------------------------*
 * File name	: e2srdo.c	/ read E2SROM DO							*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/e2srom/RCS/e2srdo.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2srdo.c $
 * ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/12 ƒxƒNƒ^[”Ô†‚ğPIO‚©‚çE2SROM‚É•ÏX
 * 
 * ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
 * ƒVƒŠƒAƒ‹‚q‚n‚l‚h^‚e
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* à–¾                                                  				*/
/*	‚d‚Q‚r‚q‚n‚l‚Ì‚c‚n‚ğ“ü—Í‚·‚éB										*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”																*/
/*	int	: ÃŞÊŞ²½¥ÅİÊŞ°													*/
/*	int	: Îß°Ä¥Á¬İÈÙ													*/
/*																		*/
/* –ß‚è’l																*/
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
