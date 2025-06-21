/*----------------------------------------------------------------------*
 * File name	: fcntl.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8inc/rcs/fcntl.h 1.1 1996/01/20 11:53:11 kabumoto Exp $
 *$Log: fcntl.h $
 * リビジョン 1.1  1996/01/20  11:53:11  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*	Purpose:															*/
/*		This file defines constants for the file control options used	*/
/*		by the open() function.											*/
/*		[System V]														*/
/*----------------------------------------------------------------------*/

#define O_RDONLY	0x0000		/* open for reading only */
#define O_WRONLY	0x0001		/* open for writing only */
#define O_RDWR		0x0002		/* open for reading and writing */
#define O_APPEND	0x0008		/* writes done at eof */

#define O_CREAT 	0x0100		/* create and open file */
#define O_TRUNC 	0x0200		/* open and truncate */
#define O_EXCL		0x0400		/* open only if file doesn't already exist */

/*
// O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
// and <lf> sequences translated to <cr><lf> on write()'s
*/

#define O_TEXT		0x4000		/* file mode is text (translated) */
#define O_BINARY	0x8000		/* file mode is binary (untranslated) */

/* macro to translate the C 2.0 name used to force binary mode for files */

#define O_RAW	O_BINARY

/* Open handle inherit bit */

#define O_NOINHERIT	0x0080		/* child process doesn't inherit file */
