/*----------------------------------------------------------------------*
 * File name	: xio.h		/ low-level file handling & I/O func.		*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/rom/RCS/xio.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: xio.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 * このファイルは p:/h8inc/rom/rcs/xio.h 1.1 と同じ
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* Purpose:																*/
/*	This file contains the function declarations for the low-level		*/
/*	file handling and I/O functions.									*/
/*----------------------------------------------------------------------*/


/* function prototypes */

int		close(int);
int		open(char *, int);
int		read(int, void *, unsigned int);
int		seek(int, int, int);
int		write(int, void *, unsigned int);
