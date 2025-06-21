/*----------------------------------------------------------------------*
 * File name	: e2siowr.c	/ serial EE-PROM write task					*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2siowr.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2siowr.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2siowr.c $
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
/*		10msecのタイマ割り込みの中で実行され、リングバッファから		*/
/*		EEPROMへ16bit単位で書き込みを行う。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		void	: 無し													*/
/*																		*/
/* 戻り値																*/
/*		int		: 0														*/
/*----------------------------------------------------------------------*/

#include	<assert.h>								/* ANSI std			*/
#include	"e2sctl.h"								/* SHow current		*/
#include	"e2sctxt.h"								/*	"				*/

int
e2s_rom_io_write(void)
{
	unsigned short	adr;
//	unsigned char	*datp;
	unsigned short	*datp;
	int				err, i, j;

	j = e2srom_hd.getp;
	while(j != e2srom_hd.putp) {
		adr = *(e2srom_hd.buff+(j++));  j &= e2srom_hd.buf_max;
		for(i= *(e2srom_hd.buff+(j++)), j &= e2srom_hd.buf_max; i--; ) {
//			datp = (unsigned char *)(e2srom_hd.buff+(j++));  j &= e2srom_hd.buf_max;
//			++datp;
			datp = (unsigned short *)(e2srom_hd.buff+(j++));  j &= e2srom_hd.buf_max;
			if(err = e2s_rom_cmd_write((WORD)WRITE, adr++, datp)) {
				j = (j+i) & e2srom_hd.buf_max;	goto wrend;
			}
			e2s_rom_chk_ready();					/* Ready 待ち		*/
		}
	}
wrend:
	e2srom_hd.getp = (unsigned short)j;
	e2srom_hd.flag &= ~WR_ON;
	if(e2s_rom_cmd_write((WORD)EWDS))							return(-1);
	return(err);
}
