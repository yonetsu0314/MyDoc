/*======================================================================
 * File name    : biosfile.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/rcs/biosfile.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: biosfile.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * 電断復旧、不具合修正
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/6/27 14:10
 * 16Mを超える容量のCFに対してもスキャンディスク
 * が実行可能なようにメモリ割り当てを変更する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/16 20:00
 * Saver60 Ver.01.04 up に対応するためFS_RAM_SIZEの定義を変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 2002, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CITY
 *			SHIGA JAPAN
 *			(077) 553-4141
 *======================================================================
 */
#ifndef	__BIOSFILE_H__
#define	__BIOSFILE_H__

#include <drive\fssvr60_1_9\fsuser.h>

	/*==========================================================
							マクロ定義
	==========================================================*/
#define	SRAM_DRIVE		'C'
#define	CF_DRIVE		'E'
#define	FS_RAM_SIZE		(3180+DFMR_USR_MAXSECTSIZE*DFMR_RWBUFFSIZE)
	/* ↑ ファイルシステム割り当てRAM領域サイズ
		DFMR_RWBUFFSIZE：1クラスタ当たりのセクタ数
					（使用するメディアにおける最大数をセットする。この数が多すぎるとメモリの
						無駄な使用となり、少ない場合は速度パフォーマンスの改善が得られない）
		DFMR_USR_MAXSECTSIZE：1セクタサイズ 
	*/
#define	SDCF_RAM_SIZE	(8704)
#define	SDSRAM_RAM_SIZE	(512)
	/* ↑ scandisk用クラスタ使用状況展開領域サイズ
		必要メモリ量＝（論理ドライブの総クラスタ数÷８）＋３２バイト
		128MB以上 CF(FAT16) 総クラスタ数=約65536
		1MB SRAM(FAT12) 総クラスタ数=約1920
	*/
#define	SD_LOG_SIZE		(128)	/* scandisk用破損ファイル復旧ログ出力領域サイズ */
	/* ↑ scandisk用破損ファイル復旧ログ出力領域サイズ
		・ファイル名のみのログの場合：必要メモリ量＝３２バイト×ログ件数
		・フルパスでのログの場合：必要メモリ量＝２７６バイト×ログ件数
		※ログ件数を超えた場合は、古いログから上書きする
	*/

	/*==========================================================
						プロトタイプ宣言
	==========================================================*/
int InitFileSystem( void );
int InitAtaDisk( void );
int InitCfDisk( void );
int InitSramDisk( int modeflag );

void CFCardHandler( int sock, int type, int event );
int TestFileCreate( const char *name);
int ReadOnlyTestFileCreate( void );
int sram_mount( void );
int sram_unmount( void );
int sram_format( void );
int SramFormat( void );
int SramScanDisk( void );
int cf_mount( void );
int cf_unmount( void );
int cf_format( void );
int CfFormat( void );
int CfScanDisk( void );

#endif		/* __BIOSFILE_H__ */

/*======================================================================*/
/*====                   ***** END OF FILE *****                    ====*/
/*======================================================================*/
