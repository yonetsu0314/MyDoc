/*======================================================================
 * File name    : shellsrc.c
 * Original		: r:/c/astra-s/exp/rcs/lowsrc.c 1.5
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/fssvr60/utility2/RCS/shellsrc.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 * $Log: shellsrc.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/17 12:30
 * ファイルシステム変換関数(cnvfsys.c)の適用
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "shellsrc.h"
#include "fsbase.h"
#define __S60FAT_H__	/* 二重定義エラーを避けるため */
#include "cnvfsys.h"

static char Find_Filename[DFMR_MAXPASSSZ+1];
static char Cur_Dir[DFMR_MAXPASSSZ+1];
static char Find_Attrib;
static char Wild_Flag;
static long find_handle;
static TFMR_FILE_INFO  findinfo;           /* ファイル状態格納領域               */


/*----------------------------------------------------------------
				'/'を'\\'に置き換える
戻り値	 
		
----------------------------------------------------------------*/
static void ReplacePathSymbol(char *str)
{
	int len, i;

	len = strlen(str);
	for (i=0; i<len; i++) {
		if (*(str+i) == '/') {
			*(str+i) = '\\';
		}
	}
}

/*----------------------------------------------------------------
				パスを大文字に変換する
戻り値	 
		
----------------------------------------------------------------*/
static void ReplaceToupper(char *str)
{
	int len, i;
	char ch;
	
	len = strlen(str);
	for (i=0; i<len; i++) {
		ch = (char)toupper(*(str+i));
		*(str+i) = ch;
	}
}

/*----------------------------------------------------------------
				ファイル名からFullPassを取得する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
char GetFullPass( char *name )
{	
	ReplacePathSymbol(name);
	ReplaceToupper(name);	

	if(	name[1] == ':') {
		return (char)-1;
	}

	if( Cur_Dir[0] == 0 ) {
		return (char)-1;
	}
	
	memmove( (void *)(name+strlen(Cur_Dir)+1), (void *)name, (size_t)strlen(name)+1 );
	memcpy( (void *)name, (void *)Cur_Dir, strlen(Cur_Dir) );
	name[(int)strlen(Cur_Dir)]=(char)'\\';
	return (char)0;
}

/*----------------------------------------------------------------
				カレントディレクトリを変更する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int chdir( char *dirname )
{
	int len, i;
	
	if( Cur_Dir[0] != 0 ) {
		if( dirname[0] == '.' && dirname[1] == '.' ) {	/* 親ディレクトリへ移動 */
			len = strlen(Cur_Dir);
			
			if ((Cur_Dir[1] == ':') && (len == 2)) {	/* カレントディレクトリがルートの場合 */
				return -1;
			}
			
			for (i=len-1; i>=0; i--) {	/* c:\xxx\yyyy →  c:\xxx */
				if (Cur_Dir[i] == '\\') {
					Cur_Dir[i] = '\0';
					return 0;
				}
				Cur_Dir[i] = '\0';
			}
			return -1;
		}
		else if (dirname[0] == '.') {	/* 自身のディレクトリの場合 */
			return 0;
		}
		else {
			GetFullPass( dirname );
		}
	}

	/*	指定のディレクトリをセットする。*/
	if( chdirexec( dirname ) ) {
		return -1;
	}
	return 0;
}
/*----------------------------------------------------------------
				指定のディレクトリをカレントディレクトリとする
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int chdirexec( char *dirname )
{
	int ret;
	char l_drvnum;
	
	l_drvnum = dirname[0];       /* 論理ドライブ番号取得        */
	l_drvnum &= 0xDF;

	/* 論理ドライブ番号が無効な値の場合   */
  	if( ( l_drvnum < DFMR_TOPDRVNUM ) || ( DFMR_ENDDRVNUM < l_drvnum ) ) {    
		return -1;           /* パラメータエラーを返却             */
	}
	if( MFMR_DRVINFO( l_drvnum ) == ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		return -1;    /* 論理ドライブ未接続エラー           */
	}

	ReplaceToupper(dirname);	

	if ((dirname[1] == ':') && (strlen(dirname) == 2)) {
	}	
	else {	
		/*	指定のディレクトリが有効かチェックする。*/
		ret = IFS_stat( dirname, &findinfo );

		if( ret != IFS_SUC ) {
			/*	ディレクトリポインタがNULLの場合はERR。*/
			return -1;
		}
		/* 属性のチェック */
		if( ( findinfo.attr & ~( IFS_ATTR_RONLY | IFS_ATTR_HIDE | IFS_ATTR_DIR ) ) != 0x00 )
		{
			return -1;
		}
	}
	
	memset( (void *)Cur_Dir, 0, sizeof(Cur_Dir) );
	/*	カレントディレクトリを更新 */
	memcpy( (void *)Cur_Dir, (void *)dirname, strlen(dirname) );
	return 0;
}

/*----------------------------------------------------------------
				カレントディレクトリを得る。
戻り値	buffer pointer : OK
		NULL           : ERR
----------------------------------------------------------------*/
char *getcwd( char *buffer, int maxlen )
{
	if( strlen(Cur_Dir) > maxlen ) {
		/*	カレントパス長がmaxlen以上になる場合はERR。*/
		return NULL;
	}
	return  strcpy(buffer, Cur_Dir);
}

/*----------------------------------------------------------------
				指定された属性の次のファイルを見つける。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
char findnext( find_t *fileinfo )
{
	int ret;

	for(;;) {
		ret = IFS_FindNext(find_handle, &findinfo);
		if( ret < 0 ) {
			IFS_FindClose(find_handle);
			return (char)-1;
		}

		if( !(findinfo.attr&Find_Attrib) ) {
			continue;
		}

		fileinfo->attrib = findinfo.attr;
		fileinfo->wr_date = findinfo.wrtdate;
		fileinfo->wr_time = findinfo.wrttime;
		dosname2name( findinfo.sfn, (unsigned char*)fileinfo->name );
		fileinfo->size = findinfo.filesize;
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
	char wild[]="*";
	char ans;

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

	getcwd(Find_Filename, (int)sizeof(Find_Filename)-(int)strlen(filename)-2);
	strcat(Find_Filename, "\\");
	strcat(Find_Filename, filename);
	
	find_handle = IFS_FindFirst(Find_Filename, &findinfo);
	if( find_handle < 0 ) {
		IFS_FindClose(find_handle);
		return (char)-1;
	}

	Find_Attrib = attrib;
	if( !(findinfo.attr&Find_Attrib) ) {
		return (char)-1;
	}

	fileinfo->attrib = findinfo.attr;
	fileinfo->wr_date = findinfo.wrtdate;
	fileinfo->wr_time = findinfo.wrttime;
	dosname2name( findinfo.sfn, (unsigned char*)fileinfo->name );
	fileinfo->size = findinfo.filesize;
	return (char)0;
}

/*----------------------------------------------------------------
					ファイルをopenする。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_fopen( const char * path, const char *mode )
{
	GetFullPass( path );
	return IFS_fopen( path, mode );
}
/*----------------------------------------------------------------
					ファイル名を変更する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_rename( char *old, char *new )
{
	GetFullPass( old );
	GetFullPass( new );
	return IFS_rename( old, new );
}
/*----------------------------------------------------------------
				ファイルを削除する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_remove( char *name )
{
	GetFullPass( name );
	return IFS_remove( name );
}
/*----------------------------------------------------------------
					ディレクトリを作成する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_mkdir( char *name )
{
	GetFullPass( name );
	return IFS_mkdir( name );
}
/*----------------------------------------------------------------
					ディレクトリを削除する。
戻り値	 0: OK
		-1: ERR
----------------------------------------------------------------*/
int	ff_rmdir( char *name )
{
	GetFullPass( name );
	return IFS_rmdir( name );
}

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
