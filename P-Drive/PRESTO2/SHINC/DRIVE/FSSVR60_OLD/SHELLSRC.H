/*======================================================================
 * File name    : shellsrc.h
 * Original		: p:/ccw_test/system/rcs/dos_util.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/shellsrc.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: shellsrc.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/17 15:00
 * ff_fopenのプロトタイプを変更する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/

#ifndef __SHELLSRC__
#define __SHELLSRC__


	/*==========================================================
						デファイン定数宣言
	==========================================================*/
#undef  FILELEN_MAX
#define FILELEN_MAX      12    /* aaaaaaaa.xxx */

	/*==========================================================
							構造体宣言
	==========================================================*/
typedef struct find_t {
	char reserved[21];
	char attrib;
	unsigned short wr_time;
	unsigned short wr_date;
	long size;
	char name[13];
} find_t;

	/*==========================================================
						関数プロトタイプ宣言
	==========================================================*/
int chdir( char *dirname );
int chdirexec( char *dirname );
char *getcwd( char *buffer, int maxlen );
char findnext( find_t *fileinfo );
char findfirst( char *filename, unsigned char attrib, find_t *fileinfo );

unsigned short date2dosdate( unsigned short year, unsigned char mon, unsigned char day );
void dosdate2date( unsigned short dosdate, unsigned short *year, unsigned char *mon, unsigned char *day );
unsigned short time2dostime( unsigned char hour, unsigned char min, unsigned char sec );
void dostime2time( unsigned short dostime, unsigned char *hour, unsigned char *min, unsigned char *sec );
void dosname2name( const unsigned char *dosname, unsigned char *name );
int format_name( const unsigned char *name, int len, unsigned char *result );

int	ff_fopen( const char * path, const char *mode );
int	ff_rename( char *old, char *new );
int	ff_remove( char *name );
int	ff_mkdir( char *name );
int	ff_rmdir( char *name );
unsigned short shortswap( unsigned char *p );
unsigned long  longswap( unsigned char *p );

#endif
