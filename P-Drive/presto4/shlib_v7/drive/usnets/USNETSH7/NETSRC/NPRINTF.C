/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/rcs/nprintf.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: nprintf.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * デバッグ出力無し
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* NPRINTF.C  Edition: 6  Last changed: 9-Jul-98,15:50:00  By: NICK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    NPRINTF.C -- Simplified Stream Conversion and Print for USNET

    Copyright (C) 1993 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#define MAXPFLEN 1024		/* size of print buffer */

#include <stdarg.h>
#include <string.h>
#include "mtmacro.h"
#include "fsys.h"
#include "dos_util.h"

#include "itron.h"
#include "kernel.h"

#define FILE ST_FILE

extern void Nputchr(char cc);

static void insert(char *s, char c)	/* insert c at head of string s */
{
    char *tmps;

    tmps = s;
    while (*tmps++);
    while( tmps > s ){
	tmps--;
	tmps[1] = tmps[0];
    }
    *s = c;
}


static void append(char *s, char c)	/* append c to end of string s */
{
    while(*s++);
    *s-- = 0;
    *s = c;
}

int visprintf(char *buf, char *fmt, va_list args)
{
    int count;
    int pwidth,width,pcnt,base;
    unsigned long num;
    char fch, c;		/* format char */
    char *s, *bp;
    char ljust, zsup;	/* left justify, zero suppress flags */
    char sign;		/* signed number conversion flag */
    char letter;		/* hex conversion letter increment */
    char islong;		/* long integer flag */
    char pflag;

    count = 0;
    *buf = 0;
    bp = buf;				/* point to start of buf */
    while((fch = *fmt++) != 0){
	while(*bp)			/* find end of current string */
	    bp++;			/*  where next field starts */
	if( fch == '%' ){
	    if ((fch= *fmt++) == '%')
		goto copy;
	    if ((ljust = fch) == '-')	/* left justify flag */
		fch = *fmt++;
	    zsup = fch;			/* zero suppression */
	    pwidth = sizeof(void *)*2;	/* minimum pointer print width */
	    pcnt = 0;			/* printed length of current field */
	    sign = 0;			/* default unsigned */
	    pflag = 0;			/* not %p spec */
	    base = 10;			/* default base */
	    if (fch == '*')
		width = va_arg(args, int);
	    else
	    {
	        for (width=0; fch>='0' && fch<='9'; fch=*fmt++)
		    width = width * 10 + fch - '0';
	    }
	    if ((islong = (fch & 0xdf)) == 'L')
		fch = *fmt++;
	    switch( fch ){
		case 'd':
		    sign = 1;
		    goto donumber;
		case 'o':		/* octal */
		    base = 8;
		    goto donumber;
		case 'u':
		    goto donumber;
		case 'x':		/* hex */
		    base = 16;
		    letter = 'a'-10;
		    goto donumber;
		case 'X':
		    base = 16;
		    letter = 'A'-10;
		    goto donumber;
		case 'p':		/* void * */
		    pflag = 1;
		    if( width < pwidth )
			width = pwidth;
		    base = 16;
		    letter = 'A'-10;
		    num = (long)va_arg(args, void *);
		    goto doptr;
		case 'c':
		    fch = (char) va_arg(args, int);
		    goto copy;
		case 's':
		    s = va_arg(args,char *);
		    while( *s ){		/* copy string to buf */
			append(bp,*s++);
			count++;
			pcnt++;
		    }
		    for( ; pcnt<width; pcnt++){
			count++;
			if (ljust == '-')
			    append(bp, ' ');
			else{
			    insert(bp, ' ');
			}
		    }
		    goto endarg;
	    }
donumber:
    {
	if (islong == 'L')
	    num = va_arg(args, long int);		/* long argument */
	else if (sign)
	    num = (long)va_arg(args, int);		/* integer argument */
	else
	    num = (long)va_arg(args, unsigned int);	/* unsigned integer */
	if( sign && (num & 0x80000000) ){
	    sign = 1;		/* do sign */
	    num = -num;
	}else{
	    sign = 0;
	}
doptr:
	while( num != 0l ){
	    c = num % base;
	    num /= base;
	    insert(bp, (char)(c > 9 ? c + letter : (char)c + '0'));
	    pcnt++;			/* count digits */
	    count++;
	}
	if(!*bp){
	    insert(bp, '0');		/* at least 1 zero */
	    pcnt++;
	    count++;
	}
	if( pflag ){
	    for(;pcnt < pwidth; pcnt++){
		insert(bp, '0');
		count++;
	    }
	}
	if (zsup != '0') zsup = ' ';	/* pad char */
	for (pcnt += sign ;pcnt < width; pcnt++)
	    if (ljust == 'L'){		/* left justified ? */
		append(bp, ' ');
		count++;
	    }else{
		insert(bp, zsup);
		count++;
	    }
	if (sign)
	    insert(bp, '-');
    }

	}else{
copy:	    append(bp++, fch);		/* copy char to output */
	    count++;
	}
endarg:
	continue;
    }
    return count;
}

int Nsprintf(char *buf, char *fmt, ...)
{
    va_list args;

    va_start(args,fmt);
    return visprintf(buf,fmt,args);
}

/* integer only printf, supports subset of printf for strings,
   and integer, or long printing in hex/decimal
*/

char pf_buf[MAXPFLEN];
static int count;
static ST_FILE *ffp, *p;
static long pos = 0;
int ans, ans1;
static unsigned int nprint_imsk;

static char Logdir[FILENAME_MAX]="c:\\log.txt";
static char Log_flg = 0;
static char Log_size = 3;

#if 0
/* 2001/8/27 NH */
void Logprintf(char *buf, int count )
{
	unsigned char sem;
	T_RSEM	pk_rsem;
	
	if( Log_flg != 1 ) {
		return;
	}

	/*	ファイルシステム用セマフォＩＤ取得	*/
	sem = get_id_filesem();

	/*	直前割り込みマスクレベル取得	*/
	get_ims( &nprint_imsk );
	/*	Ｅ－ＤＭＡＣ割り込み禁止	*/
	chg_ims(4);
	dis_dsp();

	ref_sem(sem,&pk_rsem);
	/*	ファイルシステム用セマフォが獲得さない場合のみ、
										ログ出力		*/
	if( pk_rsem.semcnt>0 ) {
		p = fopen( Logdir, "a" );
		if( p !=(void *)0 ) {
			ffp = p;
			if( pos > 1024*Log_size ) {
				ans = fseek( ffp, 0, SEEK_SET );
				if( ans < 0 ) {
					ans1 = 1;
				}
				ffp->stat = FS_WR;
				pos = 0;
			}
			else {
				ans = fseek( ffp, pos, SEEK_SET );
				if( ans < 0 ) {
					ans1 = 1;
				}
				ffp->stat = FS_WR;
			}
   			ans = fwrite(buf, count, 1, ffp);
			if( ans < 1 ) {
				ans1 = 1;
			}
			pos += count;
			ans = fclose(ffp);
			if( ans < 0 ) {
				ans1 = 1;
			}
		}
		else {
			ans = 1;
		}
	}
	ena_dsp();
	/*	直前の割り込みマスクレベルに戻す。	*/
	chg_ims(nprint_imsk);

    return;
}
#endif

void	rsstr232n(void);
void	rsout232(char);

void
rsstr232n(void){
	char	c;
	int	i;

	for(i=0;i<count;++i){
		c = pf_buf[i];
		if (c == 0) break;
		if (c == 0x0a){
//			rsout232((char)0x0d);
		}
//		rsout232(c);
	}
}

int Nprintf(char *fmt, ...)
{
#if 1
    char *p;
    va_list args;

    va_start(args,fmt);
    count = visprintf(pf_buf, fmt, args);

	rsstr232n();	//saka

/* 2001/8/27 NH */
/*	Logprintf(pf_buf, count ); */
	return 0;
#endif
}
/*****************************************************************
				ログファイル出力イネーブル処理
*****************************************************************/
void UsnetLogEnable( void )
{
    Log_flg = 1;
}
/*****************************************************************
				ログファイル出力ディセーブル処理
*****************************************************************/
void UsnetLogDisable( void )
{
    Log_flg = 0;
}
/*****************************************************************
				ログファイル出力許可状態取得処理
*****************************************************************/
char UsnetLogStates( void )
{
    return Log_flg;
}
/*****************************************************************
				ログファイルサイズ設定処理
*****************************************************************/
void UsnetSetLogSize( char size )
{
    Log_size = size;
}
/*****************************************************************
				ログファイル出力ディレクトリ設定処理
*****************************************************************/
void UsnetSetLogDir( char *buf )
{
	char logfile[]="log.txt";

 	if( sizeof( buf ) >= (FILENAME_MAX-10) ) {
		return;
	}
    memset( Logdir, 0, sizeof(Logdir) );
    memcpy( Logdir, buf, strlen(buf) );
	strcat( Logdir, logfile );
}

