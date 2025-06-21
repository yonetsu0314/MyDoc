/*----------------------------------------------------------------------*
 * File name	: e2write.c		/ serial e2rom file write				*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2swr.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2swr.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2swr.c $
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
/*		*u_bufからEEPROMの書き込み用リングバッファにbyte転送後、		*/
/*		書き込みをイネーブルにする。									*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		file_n	: file number											*/
/*		*u_buf	: user baffer address									*/
/*		byte	: byte data length										*/
/*																		*/
/* 戻り値																*/
/*		int		:  N	･･･ write complete								*/
/*		    	  -1	･･･ error										*/
/*				  -2	･･･ 空きｴﾘｱがない								*/
/*----------------------------------------------------------------------*/

#include	"e2sctl.h"							/* Show current			*/
#include	"e2sctxt.h"							/*	"					*/

int
e2s_romf_write(
	int				file_n,						/* file number			*/
	unsigned short	u_buf[],					/* user buffer address	*/
//	unsigned char	u_buf[],				
	int				byte						/* number of byte		*/
	)
{
	int				err, len;
	unsigned short	j;

	len = byte/2;
	byte = len*2;
//	len = byte;
	j = e2srom_hd.putp;
	if(((byte+2)+((j- e2srom_hd.getp) & e2srom_hd.buf_max))
	   >= e2srom_hd.buf_max)								return(-2);

	*(e2srom_hd.buff+(j++)) = e2srom_hd.e2f_p;
	*(e2srom_hd.buff+(j++)) = (unsigned short)len;
	do {
		*(e2srom_hd.buff+(j++)) = *(u_buf++);
		j &= e2srom_hd.buf_max;
	} while(--len);
	e2srom_hd.putp = j;

	if(e2srom_hd.flag != WR_ON) {
		e2s_rom_cmd_write((WORD)EWEN);
		e2srom_hd.flag |= WR_ON;
/* 暫定仕様		*/
		if(err = e2s_rom_io_write())					return(-1);
	}
	return(byte);
}
