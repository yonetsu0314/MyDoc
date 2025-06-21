/*======================================================================
 * File name    : dos_util.c
 * Original		: p:/ccw_test/system/rcs/dos_util.c 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/misc/fs/RCS/dos_util.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: dos_util.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ファイル・システムとFLASH書込み
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/

#include <string.h>

#include "fsys.h"


	/*==========================================================
							マクロ定義
	==========================================================*/

	/*==========================================================
							静的変数定義
	==========================================================*/


	/*==========================================================
							初期設定データ
	==========================================================*/

	/*==========================================================
						テーブルデータ定義
	==========================================================*/
/* ｍｓ－ｄｏｓ予約語 */
const static unsigned char* const reserved_names[] = {
    "CON     ","PRN     ","NUL     ","AUX     ",
    "LPT1    ","LPT2    ","LPT3    ","LPT4    ",
    "COM1    ","COM2    ","COM3    ","COM4    ",
    NULL };

/* 使用禁止キャラクタ */
const static unsigned char bad_chars[] = "+=,;:\\ *?<>|\"";
const static unsigned char del_simbol = (unsigned char)0xe5;

/*===================================================================
						日付をＤＯＳ日付に変換
 ===================================================================*/
unsigned short date2dosdate( unsigned short year, unsigned char mon, unsigned char day )
{
	unsigned short dosdate;

	dosdate = (unsigned short)(year-(unsigned short)1980);
	dosdate <<=4;
	dosdate |= mon;
	dosdate <<=5;
	dosdate |= day;
	return dosdate;
}
/*===================================================================
						ＤＯＳ日付を日付に変換
 ===================================================================*/
void dosdate2date( unsigned short dosdate, unsigned short *year, unsigned char *mon, unsigned char *day )
{
	*day  = (unsigned char)(dosdate & 0x1f);
	*mon  = (unsigned char)(( dosdate>>=5 ) & 0x0f);
	*year = (unsigned char)((( dosdate>>=4 ) & 0x7f ) + 1980);
}
/*===================================================================
						時刻をＤＯＳ時刻に変換
 ===================================================================*/
unsigned short time2dostime( unsigned char hour, unsigned char min, unsigned char sec )
{
	unsigned short dostime;

	dostime = (unsigned short)hour;
	dostime <<= 6;
	dostime |= (unsigned short)min;
	dostime <<= 5;
	dostime |= (unsigned short)(sec/2);
	return dostime;
}

/*===================================================================
						ＤＯＳ時刻を時刻に変換
 ===================================================================*/
void dostime2time( unsigned short dostime, unsigned char *hour, unsigned char *min, unsigned char *sec )
{
	*sec  = (unsigned char)(( dostime & 0x1f ) * 2);
	*min  = (unsigned char)(( dostime>>=5 ) & 0x3f);
	*hour = (unsigned char)(( dostime>>=6 ) & 0x1f);
}
/*===================================================================
				ＤＯＳファイル名を一般ファイル名に変換
 ===================================================================*/
void dosname2name( const unsigned char *dosname, unsigned char *name )
{
	int i, j;

	for( i=j=0; i<8; i++ ) {
		if( dosname[i] == ' ' )
			break;
		name[j++] = dosname[i];		
	}
	if( dosname[8] != ' ' ) {
		name[j++] = (unsigned char)'.';
		for( i=8; i<FILELEN_MAX; i++ ) {
			if( dosname[i] == ' ' )
				break;
			name[j++] = dosname[i];		
		}
	}
	name[j] = (unsigned char)'\0';
}

/*===================================================================
			ＤＯＳファイル名の作成（ファイル名の検査も行う）
			-----------------------------------------------
		*name: 検査したいファイル名
		 len:  検査したいファイル名の文字列長
		*res:  編集結果名
 ====================================================================*/
int format_name( const unsigned char *name, int len, unsigned char *result )
{
	unsigned char *work;
	const unsigned char **reserved;
	unsigned char c;

	/* current directory */
	if( len == 1 && name[0] == '.' ) {
		memset( (void *)result, (int)' ', (size_t)FILELEN_MAX );
		*result = (unsigned char)'.';
		return 0;
	}

	/* parent directory */
	if( len == 2 && memcmp( (void *)name, (void *)"..", (size_t)2 )== 0 ) {
		memset( (void *)result,  (int)' ', (size_t)FILELEN_MAX );
		memcpy( (void *)result, (void *)"..", (size_t)2 );
		return 0;
	}

	
	/* 拡張子よりも前の文字の検査 */
	for( c=(unsigned char)0, work=result; len && work-result < 8; work++ ) {
	  	c = *name++;
		len--;
		if( strchr((char*)bad_chars, (int)c ) || ( c<(unsigned char)' ' )) return -1;
		if((result==work) && (c==del_simbol)) c=(unsigned char)0x05;
		if (c == (unsigned char)'.') break;
		*work = (unsigned char)((c >= (unsigned char)'a' && c <= (unsigned char)'z')? c-32 : c);
	}

	/* 拡張子前が８文字以上ある時 */
	if ( len && c != '.') {
		c = *name++;
		len--;
		if (c != '.') return -1;	/* ９文字目が拡張子でなければエラー*/
	}

	/* 拡張子の頭出し */
	while (c != '.' && len--) c = *name++;

	/* 拡張子があった時 */
	if (c == '.') {
		/* 拡張子前を８文字分になるようにスペースを埋める */
		while (work-result < 8) *work++ = (unsigned char)' ';
		/* 拡張子以降の検査 */
		while (len > 0 && work-result < FILELEN_MAX) {
			c = *name++;
			len--;
			if (strchr((char*)bad_chars,(int)c) || (c < (unsigned char)' ' )) 	return -1;
//saka?			c == ' ';
			*work++ = (unsigned char)((c >= (unsigned char)'a' && c <= (unsigned char)'z')? c-32 : c);
		}
		if (len) return -1;
	}

	/* 残りはスペースで埋める */
	while( work-result < FILELEN_MAX ) *work++ = (unsigned char)' ';
	/*	最後はＮＵＬＬ文字	*/
	*work = (unsigned char)0x00;

	/* ＤＯＳ予約語の検査 */
	for( reserved = reserved_names; *reserved; reserved++ ) {
		if (!strncmp((char*)result ,(char*)*reserved, (size_t)8) ) return -1;
	}
	return 0;
}
