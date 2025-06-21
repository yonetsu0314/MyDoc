/*======================================================================
 * File name    : lowsrc.c
 * Original		: r:/c/astra-s/exp/rcs/lowsrc.c 1.5
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/misc/fs/RCS/lowsrc.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: lowsrc.c $
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
/************************************************************************/
/*                                                                      */
/*      ファイル名      : LOWSRC.C                                      */
/* ==================================================================== */
/*      機能分類        : ＡＮＳＩ互換低水準Ｉ／Ｏ関数                  */
/*      作成日付        : 2000.10.11 < Wed >                            */
/*      作成者名        : 流通物流技術１課  坂本	                    */
/*                                                                      */
/*  履歴管理                                                            */
/* ------------+----------+-------------------------------------------- */
/*  日 付      | 管理番号 | 担当者名及び変更内容                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#include <stddef.h>
#include <string.h>
#include "fsys.h"
#include "dos_util.h"

#if 0
extern unsigned char* _BH_TOP;
extern unsigned char* _BH_END;

#define HEAP_SIZE (2048*1024)	/* ヒープサイズ */

#pragma section H
static union {					/* ヒープメモリの確保		*/
	long bss_heap[HEAP_SIZE/4];	/* for align 4				*/
	char buf;
} heap;
#pragma section

static char	*brk = &heap.buf;

#define HEAP_END ((size_t)(_BH_END - _BH_TOP))
#endif

ST_DIR *Cur_DirPtr;	/*	カレントディレクトリポインタ	*/
static char Find_Filename[FILELEN_MAX+1];
static char Find_Attrib;
static char Wild_Flag;

char *getcwd( char *buffer, int maxlen );

/*----------------------------------------------------------------
				ファイル名からFullPassを取得する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
char GetFullPass( char *name )
{
	char pass[FILENAME_MAX+1];

	if(	name[1] != ':' || name[2] != '\\' ) {
		memset((void *) pass, 0, sizeof(pass) );
		getcwd( pass, (int)sizeof(pass) );
		memmove( (void *)(name+strlen(pass)+1), (void *)name, (size_t)strlen(name)+1 );
		memcpy( (void *)name, (void *)pass, strlen(pass) );
		name[(int)strlen(pass)]=(char)'\\';
		return (char)0;
	}
	return (char)-1;
}

/*----------------------------------------------------------------
				カレントディレクトリを変更する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int chdir( char *dirname )
{
	ST_DIR *dir;
	char drive;
	char pass[111];
	
	if( Cur_DirPtr != NULL ) {
		drive = Cur_DirPtr->d->name;
		if( dirname[0] == '.' && 
			dirname[1] == '.' ) {
			memset((void *) pass, 0, sizeof(pass) );
			getcwd( pass, (int)sizeof(pass) );
			memmove( (void *)(dirname+3), (void *)dirname, (size_t)strlen(dirname)+1 );
			memcpy( (void *)dirname, (void *)pass, (size_t)2 );
			dirname[2]=(char)'\\';
		}
		else {
			GetFullPass( dirname );
		}
		/*	現在のディレクトリをクローズしする。*/
		closedir( Cur_DirPtr );
	}

	/*	指定のディレクトリをオープンする。*/
	dir = opendir( dirname );
	if( dir == NULL ) {
		/*	ディレクトリポインタがNULLの場合はERR。*/
		return -1;
	}
	/*	カレントディレクトリポインタを更新。*/
	Cur_DirPtr = dir;
	return 0;
}
/*----------------------------------------------------------------
				カレントディレクトリを変更する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int chdirexec( char *dirname )
{
	ST_DIR *dir;
	
	if( Cur_DirPtr != NULL ) {
		/*	現在のディレクトリをクローズしする。*/
		closedir( Cur_DirPtr );
	}

	/*	指定のディレクトリをオープンする。*/
	dir = opendir( dirname );
	if( dir == NULL ) {
		/*	ディレクトリポインタがNULLの場合はERR。*/
		return -1;
	}
	/*	カレントディレクトリポインタを更新。*/
	Cur_DirPtr = dir;
	return 0;
}
/*----------------------------------------------------------------
				カレントディレクトリ名を得る。
戻り値	 0: OK
		 1: root directry (bufferはスペース0x20でfill。)
		-1: ERR
----------------------------------------------------------------*/
static char getcurdirname( char *buffer )
{
	char dot2[]="c:\\..";
	ST_DIRENTRY *dir_entry;
	unsigned short cur_clusno;
	char ans, i;
	
	if( Cur_DirPtr == NULL ) {
		/*	カレントディレクトリポインタがNULLの場合はERR。*/
		return (char)-1;
	}
	/*	カレントディレクトリポインタからディレクトリエントリを得る。*/
	Cur_DirPtr->stat=(char)1;
	Cur_DirPtr->pos=(unsigned short)0;
	dir_entry = readdir( Cur_DirPtr );
	if( dir_entry == NULL ) {
		/*	ディレクトリエントリがNULLの場合はERR。*/
		return (char)-1;
	}
	/*	カレントディレクトリの先頭クラスタを得る。*/
	cur_clusno = dir_entry->clusno;
	if( dir_entry->attr&ATTR_VOL ) {
		/*	属性がルートディレクトリの場合は１をreturn。	*/
		memset((void *) buffer, 0x20, (size_t)DIRLEN_MAX );
		return (char)1;
	}
	/*	親ディレクトリに移動する。*/
	dot2[0] = Cur_DirPtr->d->name;
	ans = (char)chdirexec( dot2 );
	if( ans < 0 ) {
		/*	移動できない場合はERR。*/
		return (char)-1;
	}
	for(i=(char)0;i<(char)10;i++) {
		dir_entry = readdir( Cur_DirPtr );
		if( dir_entry == NULL ) {
			/*	ディレクトリエントリがNULLの場合はERR。*/
			return (char)-1;
		}
		if( !(dir_entry->attr&ATTR_DIR) ) {
			/*	属性がサブディレクトリのエントリのみチェックを行なう。*/
			continue;
		}
		if( cur_clusno == dir_entry->clusno ) {
			/*	カレントディレクトリの先頭クラスタと
				親ディレクトリの指示ディレクトリ項目の先頭クラスタが等しい
				場合、そのエントリをカレントディレクトリと判断する。	*/
			memcpy( (void *)buffer, (void *)dir_entry->name, (size_t)DIRLEN_MAX );
			return (char)0;
		}
	}
}
/*----------------------------------------------------------------
				カレントディレクトリを得る。
戻り値	buffer pointer : OK
		NULL           : ERR
----------------------------------------------------------------*/
char *getcwd( char *buffer, int maxlen )
{
	char mark[]="\\";
	char drive[]=":";
	char *buf_ptr;
	char dos_name[DIRLEN_MAX+1], dir_name[DIRLEN_MAX+1];
	int buf_len;
	char ans;
	
	buf_len = 0;
	buf_ptr = buffer;
	for(;;) {
		/*	カレントディレクトリ名を得る。*/
		ans = getcurdirname( dos_name );
		if( ans != 0 ) {
			/*	ルートディレクトリの場合。*/
			if( (buf_len + sizeof(drive)) > maxlen ) {
				/*	カレントパス長がmaxlen以上になる場合はERR。*/
				return NULL;
			}
			buf_len += (sizeof(drive));
			/*	ドライブ名を追加。	*/
			buf_ptr += (sizeof(drive));
			memmove( (void *)buf_ptr, (void *)buffer, (size_t)buf_len );
			memcpy( (void *)&buffer[1], (void *)drive, sizeof(drive)-1);
			buffer[0] = Cur_DirPtr->d->name;
			buffer[buf_len]=(char)0x00;
			/*	ディレクトリを元に戻し、処理を終了させる。*/
			ans = (char)chdirexec( buffer );
			if( ans < 0 ) {
				/*	戻せない場合はERR。*/
				return NULL;
			}
			return buffer;
		}
		dosname2name((unsigned char *)dos_name, (unsigned char *)dir_name );
		if( (buf_len + strlen(dir_name)+sizeof(mark)) > maxlen ) {
			/*	カレントパス長がmaxlen以上になる場合はERR。*/
			return NULL;
		}
		buf_ptr += (strlen(dir_name)+sizeof(mark)-1);
		buf_len += (strlen(dir_name)+sizeof(mark)-1);
		/*	ディレクトリ名追加のためにパス文字列を移動させる。*/
		memmove( (void *)buf_ptr, (void *)buffer, (size_t)buf_len );
		memcpy( (void *)&buffer[0], (void *)mark, sizeof(mark)-1);
		memcpy( (void *)(&buffer[0]+sizeof(mark)-1), (void *)dir_name, strlen(dir_name));
		buf_ptr = buffer;
	}
}
/*----------------------------------------------------------------
				指定された属性の次のファイルを見つける。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
char findnext( find_t *fileinfo )
{
	ST_DIRENTRY *dir_entry;
	unsigned short year;
	unsigned char mon;
	unsigned char day;

	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	unsigned short cdate;
	unsigned short ctime;

	for(;;) {
		dir_entry = readdir( Cur_DirPtr );
		if( dir_entry == NULL ) {
			/*	ディレクトリエントリがNULLの場合はERR。*/
			return (char)-1;
		}
		if(	Wild_Flag == 0 ) {
			/*	ファイル名がワイルドカード以外の場合のみ、
										ファイル名チェックを行なう。*/
			if( !memcmp( (void *)dir_entry->name, (void *)Find_Filename, (size_t)FILELEN_MAX ) ) {
				continue;
			}
		}
		if( !(dir_entry->attr&Find_Attrib) ) {
			continue;
		}
		fileinfo->attrib = dir_entry->attr;

		cdate = shortswap( (unsigned char *)&dir_entry->date );

		year = (unsigned short)((cdate>>9)+1980);
		mon = (unsigned char)((cdate&0x01e0)>>5);
		day = (unsigned char)(cdate&0x001f);
		fileinfo->wr_date = date2dosdate( year, mon, day );

		ctime = shortswap( (unsigned char *)&dir_entry->time );

		hour = (unsigned char)((ctime&0xf800)>>11);
		min = (unsigned char)((ctime&0x07e0)>>5);
		sec = (unsigned char)(ctime&0x1f);

		fileinfo->wr_time = time2dostime( hour, min, sec );

		dosname2name( dir_entry->name, (unsigned char*)fileinfo->name );
		fileinfo->size  = longswap( (unsigned char *)&dir_entry->size );
		return (char)0;
	}
	return (char)-1;
}
/*----------------------------------------------------------------
				指定された属性のファイルを見つける。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
char findfirst( char *filename, unsigned char attrib, find_t *fileinfo )
{
	char wild[]="*.*";
	char ans;

	if( Cur_DirPtr == NULL ) {
		/*	カレントディレクトリポインタがNULLの場合はERR。*/
		return (char)-1;
	}
	Wild_Flag = (char)0;
	if( memcmp( (void *)wild, (void *)Find_Filename, sizeof(wild) ) ) {
		/*	ファイル名がワイルドカードの場合は、フラグを立てる。*/
		Wild_Flag = (char)1;
	}
	if( !Wild_Flag ) {
		ans = (char)format_name( (unsigned char *)filename, FILELEN_MAX, (unsigned char *)Find_Filename );
		if( ans < 0 ) {
			return (char)-1;
		}
	}
/*	ans = chdir( "c:\\" );
	if( ans < 0 ) {
		return -1;
	}*/

	Find_Attrib = attrib;

	Cur_DirPtr->stat=(char)1;
	Cur_DirPtr->pos=(unsigned short)0;
	ans = findnext( fileinfo );
	return ans;
}

/*----------------------------------------------------------------
					ファイル名を変更する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_rename( char *old, char *new )
{
	GetFullPass( old );
	return (int)ht_rename( old, new );
}
/*----------------------------------------------------------------
				ファイルを削除する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_remove( char *name )
{
	GetFullPass( name );
	return (int)ht_remove( name );
}
/*----------------------------------------------------------------
					ディレクトリを作成する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_mkdir( char *name )
{
	GetFullPass( name );
	return (int)ht_mkdir( name );
}
/*----------------------------------------------------------------
					ディレクトリを削除する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_rmdir( char *name )
{
	GetFullPass( name );
	return (int)ht_rmdir( name );
}

#if 0	/* malloc/freeはリエントラントでないので使用禁止	*/
		/* i_malloc/i_freeを使用すること see sys_mpl.c		*/
/*----------------------------------------------------------------
					メモリ領域確保関数
----------------------------------------------------------------*/
char *sbrk( unsigned long size )
{
	char	*p;

	if( brk + size > &heap.buf+ HEAP_END ) {
		return (char *)(-1);
	}
	p = brk;
	brk += size;
	return( p );
}
#endif
/*----------------------------------------------------------------
		big endian ←→ little endian 変換用
		（２バイトデータを上位下位交換）
----------------------------------------------------------------*/
unsigned short shortswap( unsigned char *p )
{
	unsigned short	cnv_data = (unsigned short)0;
	int				byte;

	for (byte = 0; byte < 2; byte++) cnv_data += (unsigned short)((*p++)<<(byte * 8));

	return cnv_data;
}

/*----------------------------------------------------------------
		big endian ←→ little endian 変換用
		（４バイトデータを上位下位交換）
----------------------------------------------------------------*/
unsigned long  longswap( unsigned char *p )
{
	unsigned long	cnv_data = 0;
	int				byte;

	for (byte = 0; byte < 4; byte++) cnv_data += (*p++) << (byte * 8);

	return cnv_data;
}
