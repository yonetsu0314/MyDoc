/*----------------------------------------------------------------------*
 * File name	: getchar.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/getchar.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: getchar.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
extern int getch(void);
extern int putch(int chr);	/* output character	*/
/*------------------------------*/
/*	c = getchar();		*/
/*		char c;		*/
/*------------------------------*/
char getchar(void)
{
	char	c;
	c = (char)getch();
	putch((int)c);
	return(c);
}
