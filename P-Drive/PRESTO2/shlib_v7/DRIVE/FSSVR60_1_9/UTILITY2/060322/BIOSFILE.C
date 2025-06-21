/*======================================================================
 * File name    : biosfile.c
 * Original		: r:/c/astra-s/exp/rcs/biosfile.c 1.16
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/fssvr60/utility2/rcs/biosfile.c 1.5 1970/01/01 00:00:00 kagatume Exp $
 * $Log: biosfile.c $
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * 電断復旧、不具合修正
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2005/6/27 14:10
 * 16Mを超える容量のCFに対してもスキャンディスク
 * が実行可能なようにメモリ割り当てを変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/03/10 12:00
 * InitAtaDisk()で、cf_baseの初期化を行う
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/24 19:00
 * InitSramDisk_Arg()内において、sram_mount()からの戻り値retが-1の時
 * return -1となるようにしていたのをretが負の時にreturn retとなるように
 * 変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <string.h>
#include "fsbase.h"
#include "biosfile.h"
#include "utility.h"
#include "drive\pcmtpl.h"
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#define	STORAGE
#include <biosif\cf.h>
#undef	STORAGE

/*==========================================================
							変数定義
==========================================================*/
/* 注：４バイトバウンダリ上に配置 */
static long	userram_sram[ FS_RAM_SIZE/4 ];		/* SRAM用RAM領域 */
static long	userram_cf[ FS_RAM_SIZE/4 ];		/* CF用RAM領域 */
static long	sramsdskram[ SDSRAM_RAM_SIZE/4 ];	/* SRAM用クラスタ使用状況展開領域 */
static long	sramsdsklog[ SD_LOG_SIZE/4 ];		/* CF用クラスタ使用状況展開領域 */
static long	cfsdskram[ SDCF_RAM_SIZE/4 ];		/* CF用クラスタ使用状況展開領域 */
static long	cfsdsklog[ SD_LOG_SIZE/4 ];			/* CF用破損ファイル復旧ログ出力領域 */

/*****************************************************************
	ファイルシステムのライブラリidの取得
*****************************************************************/
static void get_fatlib_id( void )
{
	volatile char *lib_id = NULL;
	
	extern char	*fsfatid;
	extern char	*fsutl2id;

	lib_id = fsfatid;
	if (lib_id == NULL) return; /* 最適化対策 */
	
	lib_id = fsutl2id;
	if (lib_id == NULL) return; /* 最適化対策 */
}

/*****************************************************************
	ファイルシステムの初期化
*****************************************************************/
int InitFileSystem( void )
{
	int ret;
	
	get_fatlib_id();

	ret = FMR_Init();	/* ファイルシステムの初期化 */
	if (ret != DFMR_SUC) return ret;

	Init_BackupParameter();	/* 電断対策用パラメータの初期化 */
	return DFMR_SUC;
}


/*****************************************************************
	ＳＲＡＭディスクの使用開始
*****************************************************************/
int InitSramDisk( int modeflg )
{
	int ret;
	
	if (modeflg) {
		ret = SramFormat();
		if (ret != DFMR_SUC) {
			return ret;
		}
		
	/* リードオンリーのテストファイルを作成することにより、構成管理ファイルが
	   2006年以前である電断対応ファイルシステムを搭載しているブートローダ
	   における、未使用クラスタの管理テーブルと電断復旧後の物理FAT状態の
	   不整合の問題を防ぐ。
	*/
		ret = ReadOnlyTestFileCreate();
		if (ret != DFMR_SUC) {
			return ret;
		}
		return DFMR_SUC;
	}
	else {
		ret = sram_mount();
		if (ret != DFMR_SUC) {
			return ret;
		}
		
		ret = Restore_BackupData(SRAM_DRIVE);	/* 電断対策用バックアップ復元処理 */
		if (ret != DFMR_SUC) {
			return ret;
		}

	/* 電断復旧後に再度マウントを行う。
	   マウント時に電断復旧後の物理FAT状態を元に、未使用クラスタの管理テーブルを
	   作成するようにするため。
	*/
		ret = sram_unmount();
		if (ret != DFMR_SUC) {
			return ret;
		}

		ret = sram_mount();
		if (ret != DFMR_SUC) {
			return ret;
		}

	/* ファイルシステムが壊れていても（例：SRAMが全て0で初期化されている時）、
	   sram_mount()を実行するとマウントができてしまう場合がある。そのため、
	   TestFileCreate()により、ファイルシステムが動作するかをチェックする。
	*/
		//ret = TestFileCreate("C:\\SRAMTEST.TXT");
		
	/* 本来はTestFileCreate()を使用してファイルシステムの動作をチェックするが、
	   リードオンリーのテストファイルが存在するかどうかでチェックを行う。
	*/
		ret = ReadOnlyTestFileCreate();
		if (ret != DFMR_SUC) {
			return ret;
		}
		
		return DFMR_SUC;
	}
}

/*****************************************************************
	テストファイル作成
*****************************************************************/
int TestFileCreate( const char *name )
{
	int		fhdl, ret, len;
	char	text[] = "01234567890123456789";

	/* テスト用ファイル生成 */
	fhdl = FMR_Open(name, (DFMR_O_CREAT|DFMR_O_RDWR), DFMR_ATTR_ARC);
	if( fhdl < 0 ) {
		return -1;
	}
	len = strlen(text);
	ret = FMR_Write(fhdl, text, (unsigned int)len);
	if(ret != len) {
		FMR_Close(fhdl);
		return -1;
	}
	ret = FMR_Close(fhdl);
	if(ret < 0) {
		return -1;
	}
	ret = FMR_Remove(name);
	if(ret < 0) {
		return -1;
	}
	
	return DFMR_SUC;
}

/*****************************************************************
	読込み専用テストファイル作成
*****************************************************************/
int ReadOnlyTestFileCreate( void )
{
	int		fhdl, ret;
	TFMR_FILE_INFO  findinfo;	/* ファイル状態格納領域 */
	char dirname[] = "C:\\SRAMTEST.TXT";
	
	/* 読込み専用テストファイルが既にあるかチェック */
	ret = FMR_Stat( dirname, &findinfo );
	if (ret == DFMR_ENOENT) {
		
		/* 読込み専用テストファイル生成 */
		fhdl = FMR_Open(dirname, (DFMR_O_CREAT|DFMR_O_RDWR), DFMR_ATTR_RONLY);
		if( fhdl < 0 ) {
			return fhdl;
		}
		ret = FMR_Close(fhdl);
		if(ret < 0) {
			return ret;
		}
		
		return DFMR_SUC;
	}
	else if (ret == DFMR_SUC) {
		/* テストファイル有り：チェックOK */
		return DFMR_SUC;
	}
	else {
		/* ファイルシステム異常 */
		return ret;
	}
}

/*****************************************************************
	ＳＲＡＭディスクのマウント
*****************************************************************/
int sram_mount( void )
{
	int ret;
	TFMR_MOUNTINFO l_mountinfo;         /* マウント情報 */

	l_mountinfo.driverid = (DFMR_DRVID_RAM + DFMR_MBR_OFF);
	l_mountinfo.chanel_unit = 0;

	ret = FMR_Mount(
		SRAM_DRIVE,                         /* 論理ドライブ番号                   */
		(char *)&userram_sram,              /* RAM情報アドレス                    */
		FS_RAM_SIZE,                        /* RAM領域サイズ                      */
		&l_mountinfo                        /* マウント情報領域アドレス           */
		);	
	return ret;
}

/*****************************************************************
	ＳＲＡＭディスクのアンマウント
*****************************************************************/
int sram_unmount( void )
{
	return FMR_Unmount(SRAM_DRIVE);
}

/*****************************************************************
	ＳＲＡＭディスクのフォーマット
*****************************************************************/
int sram_format( void )
{
	int		ret;
	TFMR_FORMATINFO l_formatinfo;       /* フォーマット情報 */
	char volname[] = "SRAMDISK";

	l_formatinfo.driverid =  (DFMR_DRVID_RAM + DFMR_MBR_OFF);
	l_formatinfo.chanel_unit = 0;
	l_formatinfo.entry_cnt = 224;
	l_formatinfo.FATtype = DFMR_FATTYPE12;
	l_formatinfo.vol_name = volname;

	ret = FMR_Format(&l_formatinfo);
	if (ret != DFMR_SUC) return ret;

	return DFMR_SUC;
}

int SramFormat( void )
{
	int ret;

	ret = sram_unmount();
	if ( !(ret == DFMR_SUC || ret == DFMR_DRVNOCONNECTERR) ) return ret;
	/* DFMR_DRVNOCONNECTERR：マウントされていないメディアをアンマウントした場合  */
	
	ret = sram_format();
	if( ret != DFMR_SUC ) return ret;
	
	ret = sram_mount();
	if( ret != DFMR_SUC ) return ret;
	
	return DFMR_SUC;
}

/*****************************************************************
	ＳＲＡＭのスキャンディスク
*****************************************************************/
int SramScanDisk( void )
{	
	int		ret;

	SetScanDiskFlg(SRAM_DRIVE); /* スキャンディスク中はバックアップを行わない */
	ret = FMR_ScanDisk(
		SRAM_DRIVE,						/* ドライブ番号                       */
		(unsigned char *)sramsdskram,	/* クラスタ使用状況展開領域アドレス   */
		SDSRAM_RAM_SIZE,				/* クラスタ使用状況展開領域サイズ     */
		(unsigned char *)sramsdsklog,	/* 破損ファイル復旧ログ出力領域アドレス */
		SD_LOG_SIZE,					/* 破損ファイル復旧ログ出力領域サイズ */
		DFMR_SD_LOGMODEON | DFMR_SD_LOGWRTFILE	/* 破損ファイル復旧ログ動作指定フラグ */
	);
	ClearScanDiskFlg(SRAM_DRIVE);
	return ret;
}


/*****************************************************************
	ＡＴＡディスクの使用開始
*****************************************************************/
int InitAtaDisk( void )
{
	int ret;
	const struct cfpinf	*phy_inf;	/* & to physical info	*/

	phy_inf = _cf_init((int)0);
	cf_base.comm = phy_inf->comm;
	cf_base.attr = phy_inf->attr;
	cf_base.io   = phy_inf->io;

	ret = SetUserCardHandler( CFCardHandler );
	if (ret != DFMR_SUC) return ret;

	PcmciaEvent();
	return DFMR_SUC;
}

/*****************************************************************
	ＣＦディスクの使用開始
*****************************************************************/
int InitCfDisk( void )
{
	int ret;
	
	ret = cf_mount();
	if (ret != DFMR_SUC) {
		return ret;
	}
		
	ret = Restore_BackupData(CF_DRIVE);	/* 電断対策用バックアップ復元処理 */
	if (ret != DFMR_SUC) {
		return ret;
	}

	/* 電断復旧後に再度マウントを行う。
	   マウント時に電断復旧後の物理FAT状態を元に、未使用クラスタの管理テーブルを
	   作成するようにするため。
	*/
	ret = cf_unmount();
	if (ret != DFMR_SUC) {
		return ret;
	}

	ret = cf_mount();
	if (ret != DFMR_SUC) {
		return ret;
	}
	
	return DFMR_SUC;
}

/*****************************************************************
	ＣＦカードのイベント処理
*****************************************************************/
void CFCardHandler( int sock, int type, int event )
{
	if( type == CISTPL_FUNCID_FIXED ) {
		if( event == SS_INSERTION ) {
			InitCfDisk();
		}
		else if( event == SS_EJECTION ) {
			cf_unmount();
		}
	}
}

/*****************************************************************
	ＣＦのマウント
*****************************************************************/
int cf_mount( void )
{
	int		ret;
	TFMR_MOUNTINFO l_mountinfo;         /* マウント情報 */

	l_mountinfo.driverid = DFMR_DRVID_PCATA;
	l_mountinfo.chanel_unit = 0;

	ret = FMR_Mount(
		CF_DRIVE,                           /* 論理ドライブ番号                   */
		(char *)&userram_cf,                /* RAM情報アドレス                    */
		FS_RAM_SIZE,                        /* RAM領域サイズ                      */
		&l_mountinfo                        /* マウント情報領域アドレス           */
		);
	return ret;
}

/*****************************************************************
	ＣＦのアンマウント
*****************************************************************/
int cf_unmount( void )
{
	return FMR_Unmount(CF_DRIVE);
}

/*****************************************************************
	ＣＦのフォーマット
*****************************************************************/
int cf_format( void )
{
	int		ret;
	TFMR_FORMATINFO l_formatinfo;       /* フォーマット情報 */
	char volname[] = "CFDISK";

	l_formatinfo.driverid =  DFMR_DRVID_PCATA;
	l_formatinfo.chanel_unit = 0;
	l_formatinfo.entry_cnt = 512;
	l_formatinfo.FATtype = DFMR_FATTYPE12;
	l_formatinfo.vol_name = volname;

	ret = FMR_Format(&l_formatinfo);
	if (ret != DFMR_SUC) return ret;

	return DFMR_SUC;
}

int CfFormat( void )
{
	int ret;

	ret = cf_unmount();
	if (ret != DFMR_SUC) return ret;
	
	ret = cf_format();
	if (ret != DFMR_SUC) return ret;
	
	ret = cf_mount();
	if (ret != DFMR_SUC) return ret;
	
	return DFMR_SUC;
}

/*****************************************************************
	ＣＦのスキャンディスク
*****************************************************************/
int CfScanDisk( void )
{
	int		ret;

	SetScanDiskFlg(CF_DRIVE); /* スキャンディスク中はバックアップを行わない */
	ret = FMR_ScanDisk(
		CF_DRIVE,					/* ドライブ番号                       */
		(unsigned char *)cfsdskram,	/* クラスタ使用状況展開領域アドレス   */
		SDCF_RAM_SIZE,				/* クラスタ使用状況展開領域サイズ     */
		(unsigned char *)cfsdsklog,	/* 破損ファイル復旧ログ出力領域アドレス */
		SD_LOG_SIZE,				/* 破損ファイル復旧ログ出力領域サイズ */
		DFMR_SD_LOGMODEON | DFMR_SD_LOGWRTFILE	/* 破損ファイル復旧ログ動作指定フラグ */
	);
	ClearScanDiskFlg(CF_DRIVE);
	return ret;
}

/*======================================================================*/
/*====                   ***** END OF FILE *****                    ====*/
/*======================================================================*/
