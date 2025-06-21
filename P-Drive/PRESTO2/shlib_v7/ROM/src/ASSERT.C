/*----------------------------------------------------------------------*
 * File name	: assert.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/assert.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: assert.c $
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
/*	規格－ANSI							*/
/*	Assertion failed: [expretion], [file name], [line number]の	*/
/*	ﾒｯｾｰｼﾞを出力し、本関数を起動したﾌﾟﾛｾｽをｱﾎﾞｰﾄする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*expretion	: ｴﾗｰとなった記述				*/
/*	*file_name	: ｿｰｽ･ﾌﾟﾛｸﾞﾗﾑのﾌｧｲﾙ･ﾈｰﾑ				*/
/*	line_no		: ｿｰｽ･ﾌﾟﾛｸﾞﾗﾑのﾗｲﾝ番号				*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
extern void	abort(void);

void
_assert(
	char		*expretion,
	char		*file_name,
	unsigned int	line_no
	)
{
	printf("\n\rAssertion failed: (%s), %s, %-4d",
		expretion, file_name, line_no);
	while(1) { ; }
	abort();
}
