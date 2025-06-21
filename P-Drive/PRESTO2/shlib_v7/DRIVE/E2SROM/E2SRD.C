/*----------------------------------------------------------------------*
 * File name	: e2srd.c	/ serial e2rom file read					*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2srd.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2srd.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2srd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		EEPROMからの要求byte数*u_bufへ読みだしを行う。					*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		file_n	: file number											*/
/*		*u_buf	: user baffer address									*/
/*		byte	: byte data length										*/
/*																		*/
/* 戻り値																*/
/*		int		: 要求byte数の残り										*/
/*----------------------------------------------------------------------*/

#include	"e2sctl.h"							/* Show current			*/
#include	"e2sctxt.h"							/*	"					*/

int
e2s_romf_read(
	int				file_n,						/* file number			*/
	unsigned short	u_buf[],					/* user buffer address	*/
//	unsigned char	u_buf[],					
	int				byte						/* number of byte		*/
	)
{
	unsigned short	i, len;

	len = (WORD)(byte/2);
	byte = len*2;
//	len = (WORD)byte;
	i = e2srom_hd.e2f_p;
	do {
		if(e2s_rom_cmd_write((WORD)READ, i++, u_buf++))		return(-1);
	} while(--len);
	e2srom_hd.e2f_p = i;
	return(byte);
}
