/*----------------------------------------------------------------------*
 * File name	: putch.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/rom/src/RCS/putch.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: putch.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/11/12 11:30
 * SH2ではdebout()を呼ぶ
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * CPUSH2ならsci_io_writeとした
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/05/01 20:30
 * ビッグエンディアンに対応する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	STDOUTへ1文字直接書き込みを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	chr	: 書き込みを行うｷｬﾗｸﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: 書き込みを行った文字	･･･ complete			*/
/*		  EOF			･･･ fail			*/
/*----------------------------------------------------------------------*/
/* Modification history							*/
/*	Version :    Date	:  Name					*/
/*	v1.00	: 28-JUN-1993   : 株本 隆司				*/
/*				  / Initial coding			*/
/*----------------------------------------------------------------------*/

#include	<drive\iofsys.h>

int	scu_io_write(int ch, void *buf, int len);
void debout(char c);

extern int	stdio_filno;
extern int	stdio_devno;

int
putch(
	int		chr			/* output character	*/
	)
{
	unsigned char	c;

	c = (unsigned char)chr;
	if( !stdio_filno )
#ifdef CPUSH2
//		return(sci_io_write( 0, (void *)&c, (int)sizeof(char)));
	{
		debout((char)c);
		return(1);
	}
#else
		return(scu_io_write( 0, (void *)&c, (int)sizeof(char)));
#endif
	else
		return( iodrv_write[stdio_filno](stdio_devno, (unsigned char *)&c, (int)sizeof(char)) );
}
