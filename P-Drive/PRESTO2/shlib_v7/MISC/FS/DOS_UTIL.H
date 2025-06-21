/*======================================================================
 * File name    : dos_util.h
 * Original		: p:/ccw_test/system/rcs/dos_util.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/misc/fs/RCS/dos_util.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: dos_util.h $
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

#ifndef __DOS_UTIL__
#define __DOS_UTIL__


	/*==========================================================
						デファイン定数宣言
	==========================================================*/
/* ファイル属性 */
#define ATTR_RO			0x01			/* read-only 	*/
#define ATTR_HIDE		0x02			/* hidden 		*/
#define ATTR_SYS		0x04  			/* system 		*/
#define ATTR_VOL		0x08			/* volume label */
#define ATTR_DIR		0x10			/* directory 	*/
#define ATTR_ARCH		0x20			/* archived 	*/

#define ATTR_NONE    0 					/* no attribute bits */
#define ATTR_UNUSED  (ATTR_VOL | ATTR_ARCH | ATTR_SYS | ATTR_HIDE)
								/* attribute bits that are copied "as is" */
#define ATTR_EXT     (ATTR_RO | ATTR_HIDE | ATTR_SYS | ATTR_VOL)
								/* bits that are used by the Windows 95/Windows NT extended FAT */

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
char getcurdirname( char *buffer );
char *getcwd( char *buffer, int maxlen );
char findnext( find_t *fileinfo );
char findfirst( char *filename, unsigned char attrib, find_t *fileinfo );

unsigned short date2dosdate( unsigned short year, unsigned char mon, unsigned char day );
void dosdate2date( unsigned short dosdate, unsigned short *year, unsigned char *mon, unsigned char *day );
unsigned short time2dostime( unsigned char hour, unsigned char min, unsigned char sec );
void dostime2time( unsigned short dostime, unsigned char *hour, unsigned char *min, unsigned char *sec );
void dosname2name( const unsigned char *dosname, unsigned char *name );
int format_name( const unsigned char *name, int len, unsigned char *result );

unsigned short shortswap( unsigned char *p );
unsigned long  longswap( unsigned char *p );

unsigned short swapb( unsigned short s );
unsigned long  swapl( unsigned long l );

#endif
