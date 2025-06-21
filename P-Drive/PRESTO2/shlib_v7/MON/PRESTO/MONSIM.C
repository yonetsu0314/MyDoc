/*----------------------------------------------------------------------*
 * File name	: monsim.c	/ moniter simulate main			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/monsim.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: monsim.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/monsim.c 1.1 1970/01/01
 *Log: monsim.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<conio.h>			/* MS-C	"		*/
#include	<dos.h>				/*	"		*/
#include	"admtxt.h"			/* current		*/
#include	"functbl.h"			/*	"		*/
#include	"usrkint.h"			/*			*/

static void (_interrupt	*func)();
static volatile int	ctrl = END_PROC;

/*----------------------------------------------------------------------*/
/*	mon_main							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	DOS環境化でのファイル入力を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc	: 引き数の数						*/
/*	argv[]	: ﾊﾟﾗﾒｰﾀ･ﾎﾟｲﾝﾀの配列へのﾎﾟｲﾝﾀ				*/
/*			argv[1]	･･･ ｺﾏﾝﾄﾞ･ﾌｧｲﾙ名			*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

void
main(
	int	argc,				/* no of argument	*/
	char	*argv[]				/* pointer to arg[]	*/
	)
{
	FILE	*fp =stdin;			/* file pointer		*/
	char	*p, buf[RXBUF_MAX];		/* 受信ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ	*/
	int	err, len, c, kn;
	void	*str;

/*	func = _dos_getvect(USER_KEY);		*/
	func_tbl[INITIAL_FUNC]();
	while(1) {
		printf("\nType key '0-9,A-Z,a-z' then return\n");
		kn = 0;
		do {
			switch(c = getche()) {
			case '\x1b':		/* ESC			*/
			case '\x11':		/* CTRL-Q		*/
				goto restart;

			case '\x12':		/* CTRL-R		*/
				dos_vector_reset();
				break;

			case '\b':		/* BS			*/
			case '\x7f':		/* DEL			*/
				if(kn)  --kn;
				break;

			case '\r':		/* CR			*/
				break;

			default:
				if(kn < sizeof(buf))  buf[kn++] = c;
			}
		} while(c != '\r');

		if((buf[0] >= '0')&&(buf[0] <= '9'))
			c = buf[0]-'0';
		else if((buf[0] >= 'A')&&(buf[0] <= 'Z'))
			c = (buf[0]-'A')+10;
		else if((buf[0] >= 'a')&&(buf[0] <= 'z'))
			c = (buf[0]-'a')+10;
		if(func_tbl[c] == NULL)  printf("Undefine function");
		else			 func_tbl[c]();
	}
restart:
/*	_dos_setvect(USER_KEY, func);		*/

	if(argc > 2) {
		printf("\nToo many token !\n");
		return;
	}
	if(argc == 2) {				/* ﾃｷｽﾄ･ﾌｧｲﾙからの起動	*/
		if((fp = fopen(argv[1],"rt")) == NULL) {
			printf("\nFile not found !\n");
			goto endm;
		}
	}
	else if (argc == 1) {			/* ｷｰ入力による起動	*/
		fflush(stdin);
		printf("\nPlease input command\n");
	}
	_dos_sem = 1;
	if((str = fgets( buf, sizeof(buf), fp)) == NULL) {
		printf("\nFile read error: 'stdin' ");
		goto endm;
	}
	_dos_sem = 0;
	if((p = strchr( buf, '\n')) != NULL)
		*p = '\0';
	if(err = mon_text_interpret( buf, sizeof(buf))) {
		printf("\nError return from: 'text_interpret' ");
	}
endm:
	if((fp != stdin) && (fp != NULL))  fclose(fp);
	return;
}
