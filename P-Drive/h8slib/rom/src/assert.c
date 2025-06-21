/*----------------------------------------------------------------------*
 * File name	: assert.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/assert.c 1.1 1996/01/19 15:54:11 kabumoto Exp $
 *$Log: assert.c $
 * リビジョン 1.1  1996/01/19  15:54:11  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		規格－ANSI														*/
/*		Assertion failed: [expretion], [file name], [line number]の		*/
/*		ﾒｯｾｰｼﾞを出力し、本関数を起動したﾌﾟﾛｾｽをｱﾎﾞｰﾄする。				*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		*expretion	: ｴﾗｰとなった記述									*/
/*		*file_name	: ｿｰｽ･ﾌﾟﾛｸﾞﾗﾑのﾌｧｲﾙ･ﾈｰﾑ								*/
/*		line_no		: ｿｰｽ･ﾌﾟﾛｸﾞﾗﾑのﾗｲﾝ番号								*/
/*																		*/
/* 戻り値																*/
/*		void		: 無し												*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>								/* ANSI std			*/

void
_assert(
	char			*expretion,
	char			*file_name,
	unsigned int	line_no
	)
{
	int			err;
	long		tim = 0x80000;

	do {
		err = printf("\n\rAssertion failed: (%s), %s, %-4d",
					expretion, file_name, line_no);
		while(tim--);
	} while(err == -2);
	while(1);
/*	exit(-1);	*/
}
