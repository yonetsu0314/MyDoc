/*======================================================================
 * File name    : biosfile.c
 * Original		: r:/c/astra-s/exp/rcs/biosfile.c 1.16
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/misc/fs/rcs/biosfile.c 1.6 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: biosfile.c $
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * InitAtaDisk()で、cf_baseの初期化を行う
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * ＦＳのSRAMTOPを0x02010000にし,容量を１ＭＢにする。
 * 初期化時SRAM領域を０クリアする
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * InitSramDisk InitDramDiskの引数でフォーマットの有無を選択
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
/*      ファイル名      : BIOSFILE.C                                    */
/* ==================================================================== */
/*      機能分類        : システムメイン関数                            */
/*      作成日付        : 2000.10.04 < Wed >                            */
/*      作成者名        : 流通物流技術部１課 坂本	                    */
/*                                                                      */
/*  履歴管理                                                            */
/* ------------+----------+-------------------------------------------- */
/*  日 付      | 管理番号 | 担当者名及び変更内容                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#include <kernel.h>/*020729*/
#include <string.h>/*020729*/

#include "fsys.h"/*020729*/
#include "drive\pcmtpl.h"/*020729*/
#include "drive\pcmsock.h"/*020729*/
#include "drive\fs_ata.h"/*020729*/
#include "drive\pcmcard.h"

#define	STORAGE
#include <biosif\cf.h>
#undef	STORAGE
	/*==========================================================
							マクロ定義
	==========================================================*/
#define MAX_FILE        (30)		/*最大ファイル数*//*020729*/
#define MAX_DISK_NUM	(3)			/* 同時オープンディスク数	*/

#define __OPEN__

	/*==========================================================
							外部変数定義
	==========================================================*/
ST_FILE ex_f[MAX_FILE];
ST_DISK ex_d[MAX_DISK_NUM];

unsigned char tblFATC[SECT_SZ*256];

unsigned char FileErrReg;			/* ファイルエラーレジスタ 	*/
extern int format_flag;

unsigned long SRAM_TOP = 0x02010000;	/*020726 SRAM後半分256kB*/


	/*==========================================================
						プロトタイプ宣言
	==========================================================*/
ER flash_ATA(ST_DISK *d, FN fncd, UW sectno, VP par, UH snum);
void ramdisk( void );
void CFCardHandler( int sock, int type, int event );/*020726*/
void InitDramDisk( int flag );
void InitAtaDisk( void );
void InitSramDisk( int flag );


/*****************************************************************
				ＳＲＡＭ実装容量チェック
*****************************************************************/
unsigned long sram_size( unsigned long *top, unsigned long max )
{
	unsigned long *p = (unsigned long*)((unsigned long)top & 0xFFFF0000);
	unsigned long *pp;

	unsigned long last;
	unsigned long size;

	/* 非キャッシュエリア(0x2xxxxxxx)でアクセスする */
	last = (unsigned long)p;
	last |= (unsigned long)0x20000000;/*030131*/
	p = (unsigned long *)last;
	pp = p;

	/* 512Kビット毎のチェック *//*020729*/
	for( size=0; size<max; p=pp+size/sizeof(long) ) {

		/* 元々あったデータを格納 */
		last=*p;

		/* テストデータを書き込み */
		*p = 0x12345678;

		/* テストデータのベリファイ */
		if( *p != 0x12345678 ) {
			break;
		}
		/* 元々あったデータを戻す */
		*p = last;

		/* サイズ更新 */
		size+=0x10000;/*030131*/
	}	
	size -= ((unsigned long)top & 0x0000FFFFL );
	return size;
}

/*****************************************************************
	ファイルシステムの初期化
*****************************************************************/
void InitFileSystem( void )
{
	/* ファイルシステムの初期化 */
//	ini_fsys(  ex_f, MAX_FILE, FILE_SEM_ID );
	ini_fsys(  ex_f, MAX_FILE, 0 );		/* saka:自動でSEMIDは採番される */
}

/*****************************************************************
	ＤＲＡＭディスクの使用開始
*****************************************************************/
void InitDramDisk( int flag )
{
	/* ＤＲＡＭドライブの作成（毎回フォーマット） */
	format_flag = flag;
	ini_disk( &ex_d[1], 'D', (DISK_FP)ramdisk, (unsigned long)0x06800000, 0x100000, NULL, 0, NULL );
	format_flag = 0;
}

/*****************************************************************
	ＡＴＡディスクの使用開始
*****************************************************************/
void InitAtaDisk( void )
{
	const struct cfpinf	*phy_inf;	/* & to physical info	*/

	phy_inf = _cf_init((int)0);
	cf_base.comm = phy_inf->comm;
	cf_base.attr = phy_inf->attr;
	cf_base.io   = phy_inf->io;

#if 1/*020726*/
	/* ＣＦカード挿入時のコールバック関数を設定する */
	ini_disk( &ex_d[2], 'E', (DISK_FP)flash_ATA, (unsigned long)0, 0, NULL, (B)1, (B *)tblFATC );
	SetUserCardHandler( CFCardHandler );

	/* ＣＦカードが挿入されていれば実行 */
	PcmciaEvent();
#else
	ini_disk( &ex_d[2], 'E', (DISK_FP)flash_ATA, 0L, 0L, NULL, 1, (char *)tblFATC );
	ini_pcmcia();	/* PCMCIA初期化ﾒｲﾝ       */
#endif
}


/*****************************************************************
	ＳＲＡＭディスクの初期化
*****************************************************************/
static void ramdisk_format( void )
{
	unsigned long size;
	unsigned long *p, *end;

	size = sram_size( (unsigned long*)SRAM_TOP,(unsigned long) 0x000f0000 );/*saka*/
	/* フォーマット時ＳＲＡＭを０でクリア */
	if( format_flag ) {
		p = (unsigned long *)SRAM_TOP;
		end = (unsigned long *)(SRAM_TOP + size);
		while (p < end) *p++ = 0L;
	}
	/* ＳＲＡＭドライブの作成 */
	ini_disk( &ex_d[0], 'C', (DISK_FP)ramdisk, SRAM_TOP, size, NULL, 0, NULL );
}

/*****************************************************************
	ＳＲＡＭディスクの使用開始
*****************************************************************/
void InitSramDisk( int flag )
{
	/* 実験用のため毎回フォーマットし、バックアップしない。 */
	/* (format_flag = 1)                                    */
	/* バックアップするときは毎回フォーマットしないので     */
	/* 外部からフォーマットできる手段を実装する必要がある。 */
	format_flag = flag;
	ramdisk_format();
	format_flag = 0;
}

/*****************************************************************
	テストファイル作成
*****************************************************************/
int TestFileCreate( const char *name)
{
	ST_FILE	*fp;
	int		st;
	char	text[30] = {""};

	/* テスト用のファイル生成 */
	fp = fopen( (char*)name, "w" );
	if( fp == NULL ) {
		return -1;
	}
	st = fseek( fp, 0, SEEK_SET );
	st = fwrite("01234567890123456789", 20, 1, fp);
	fclose(fp);

	/* 再確認用の読込み */
	fp = fopen( (char*)name, "r" );
	fgets(text, 10, fp);
	fclose(fp);

	return st;
}

/*****************************************************************
	ＣＦカードのイベント処理
*****************************************************************/
void CFCardHandler( int sock, int type, int event )
{
	if( type == CISTPL_FUNCID_FIXED ) {
		if( event == SS_INSERTION ) {
			mount_disk(&ex_d[2]);
		}
		else if( event == SS_EJECTION ) {
			unmount_disk(&ex_d[2]);
		}
	}
}
