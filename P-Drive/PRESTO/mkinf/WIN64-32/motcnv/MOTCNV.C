/*
 *==========================================================================
 *	Filename	: MOTCNV.C
 *--------------------------------------------------------------------------
 *	Function	: MOTOROLA EXORCISER TO INTELLEC HEX OBJECT CONVERTER
 *--------------------------------------------------------------------------
 *$Header: p:/tool/motcnv/RCS/MOTCNV.C 1.3 2005-01-20 18:48:30+09 kawamura Exp $
 *$Log: MOTCNV.C $
 *Revision 1.3  2005-01-20 18:48:30+09  kawamura
 *2005/01/20  09:48
 *一部の表記を英語にする。
 *
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2005/01/19  18:06
 * メイン関数の戻り値をvoidからintに変更する。
 * 
 * リビジョン 1.1  1997/02/14  18:52:56  minaguchi
 * 初期リビジョン
 * 
 *--------------------------------------------------------------------------
 *		(c) Copyright 1996-1997, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(0775) 53-4141
 *==========================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"utype.h"
#include	"cnv.h"

/*==========================================================================
 *		シンボル定義
 *--------------------------------------------------------------------------*/
#define		STRSIZE		(256)
#define		ADDR_HMASK	(DWORD)(0xFFFF0000)
#define		ADDR_LMASK	(DWORD)(0x0000FFFF)

/*==========================================================================
 *		関数プロトタイプ
 *--------------------------------------------------------------------------*/
static void	strsumcat( char *str );

/*==========================================================================
 *		変数確保
 *--------------------------------------------------------------------------*/
BYTE	motpath[_MAX_PATH];		/* MOTOROLA EXORCISERフルパス名	*/
BYTE	hexpath[_MAX_PATH];		/* INTELLEC HEX フルパス名		*/
BYTE	drive[_MAX_DRIVE];		/* ドライブ指定の文字			*/
BYTE	dir[_MAX_DIR];			/* ディレクトリのパス名			*/
BYTE	fname[_MAX_FNAME];		/* ファイル名					*/
BYTE	ext[_MAX_EXT];			/* ファイルの拡張子				*/
BYTE	motstr[STRSIZE];		/* MOTOROLA EXORCISER 文字列	*/
BYTE	hexstr[STRSIZE];		/* INTELLEC HEX 文字列			*/

/*==========================================================================
 *		メイン関数
 *--------------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{
	FILE	*fpmot, *fphex;
	WORD	bcnt = 0;
	DWORD	addr = 0;
	BYTE	datstr[STRSIZE];
	WORD	off_addr = 0;
	WORD	seg_addr = 0;
	WORD	last_seg_addr = 0;

	fprintf( stderr, "MOTOROLA EXORCISER FORMAT TO INTELLEC HEX FORMAT OBJECT CONVERTER\n" );
	fprintf( stderr, "Copyright (c) ISHIDA CO.,LTD 1996-1997. All rights reserved.  $Revision: 1.3 $\n\n" );

	if( argc==1 || argc > 3 ) {
		fprintf( stderr, "Usage: MOTCNV [Drive:][Path]filename(.MOT) [[Drive:][Path:]filename(.HEX)]\n" );
		return -1;
	}
	else if( argc==2 ) {
		sprintf( &motpath[0], "%s", argv[1] );
		_strupr( &motpath[0] );
		_splitpath( &motpath[0], &drive[0], &dir[0], &fname[0], &ext[0] );
		if( strncmp( &ext[0], ".HEX", 4 )==0 ) {
			fprintf( stderr, "入力ファイルに拡張子が.HEXであるファイルを指定することはできません.\n" );
			return -1;
		}
		_makepath( &hexpath[0], &drive[0], &dir[0], &fname[0], ".HEX" );
	}
	else if( argc==3 ) {
		sprintf( &motpath[0], "%s", argv[1] );
		_strupr( &motpath[0] );
		_splitpath( &motpath[0], &drive[0], &dir[0], &fname[0], &ext[0] );
		if( strncmp( &ext[0], ".HEX", 4 )==0 ) {
			fprintf( stderr, "入力ファイルに拡張子が.HEXであるファイルを指定することはできません.\n" );
			return -1;
		}
		sprintf( &hexpath[0], "%s", argv[2] );
		_strupr( &hexpath[0] );
		if( strcmp( &motpath[0], &hexpath[0] )==0 ) {
			fprintf( stderr, "入力ファイル名と出力ファイル名が同じです.\n" );
			return -1;
		}
	}
	fpmot = fopen( &motpath[0], "r" );
	if( fpmot==NULL ) {
		fprintf( stderr, "入力ファイル(%s)をオープンできません.\n", &motpath[0] );
		return -1;
	}
	fphex = fopen( &hexpath[0], "w" );
	if( fphex==NULL ) {
		fprintf( stderr, "出力ファイル(%s)をオープンできません.\n\n", &hexpath[0] );
		return -1;
	}
	fprintf( stderr, "INPUT FILE: %s\n", &motpath[0] );
	fprintf( stderr, "OUTPUT FILE: %s\n", &hexpath[0] );
	fprintf( stderr, "\n" );

	while( fgets( &motstr[0], STRSIZE, fpmot )!=NULL ) {
		if( motstr[0]=='S' ) {
			if( motstr[1]=='0' ) {		/* ヘッダレコード */
			}
			else if( motstr[1]=='1' ) {	/* データレコード（ショート） */
				sscanf( &motstr[2], "%02X%04lX%s", &bcnt, &addr, &datstr[0] );
				bcnt -= 3;
			}
			else if( motstr[1]=='2' ) {	/* データレコード（スタンダード） */
				sscanf( &motstr[2], "%02X%06lX%s", &bcnt, &addr, &datstr[0] );
				bcnt -= 4;
			}
			else if( motstr[1]=='3' ) {	/* データレコード（ロング） */
				sscanf( &motstr[2], "%02X%08lX%s", &bcnt, &addr, &datstr[0] );
				bcnt -= 5;
			}
			else if( motstr[1]=='7' )	break;	/* エンドレコード */
			else if( motstr[1]=='8' )	break;	/* エンドレコード */
			else if( motstr[1]=='9' )	break;	/* エンドレコード */

			if( motstr[1]=='1' || motstr[1]=='2' || motstr[1]=='3' ) {
				seg_addr = (WORD)((addr>>4) & 0xF000);
				if( seg_addr!=last_seg_addr ) {
					sprintf( &hexstr[0], "02000002%04X", seg_addr );
					strsumcat( &hexstr[0] );
					fprintf( fphex, ":%s\n", &hexstr[0] );
				}
				last_seg_addr = seg_addr;

				off_addr = (WORD)(addr & ADDR_LMASK);
				sprintf( &hexstr[0], "%02X%04X00", bcnt, off_addr );
				strncat( &hexstr[0], &datstr[0], (2*bcnt) );
				strsumcat( &hexstr[0] );
				fprintf( fphex, ":%s\n", &hexstr[0] );
			}
		}
		else {
			_fcloseall();
			remove( &hexpath[0] );
			fprintf( stderr, "入力ファイル(%s)のデータフォーマットが不正です.\n", &motpath[0] );
			fprintf( stderr, "出力ファイル(%s)を削除しました.\n", &hexpath[0] );
			return -1;
		}
	}
	fprintf( fphex, ":00000001FF\n" );	/* エンドレコード出力 */
	_fcloseall();
	return 0;
}

/*==========================================================================
 *		チェックサムの付加
 *--------------------------------------------------------------------------
 *	引き数: チェックサムを付加する文字列
 *	戻り値:	なし
 *--------------------------------------------------------------------------*/
static void strsumcat( char *str )
{
	int		len, i;
	int		sum = 0;
	char	sumstr[3];

	len = ((WORD)strlen(&str[0]) >> 1);
	for( i=0; i<len; i++ ) {
		sum += htoc(&str[2*i]);
	}
	sum = ((~sum)+1) & 0xFF;
	sprintf( &sumstr[0], "%02X", sum );
	strcat( &str[0], &sumstr[0] );
}
/*==========================================================================*/
