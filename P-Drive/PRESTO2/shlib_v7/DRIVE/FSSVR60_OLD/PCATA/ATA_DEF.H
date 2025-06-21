/*======================================================================
 * File name    : ata_def.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/ata_def.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ata_def.h $
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
/******************************************************************************/
/* ファイル名：PCATA_Def.h                                                    */
/* 概    要  ：定数定義                                                       */
/*           ：                                                               */
/* 作 成 日  ：2004.03.19                                                     */
/* 作 成 者  ：                                                               */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */

/******************************************************************************/

#ifndef __PCATA_DEF_H__
#define __PCATA_DEF_H__

/************************************/
/*	データ型宣言	 				*/
/************************************/

typedef signed   short	F_BOOL;					/* 16bitブール値			*/
typedef signed   char	F_CHAR;					/* 符号付8bit値				*/
typedef unsigned char	F_BYTE;					/* 符号無し8bit値			*/
typedef signed   short	F_SHORT;				/* 符号付16bit値			*/
typedef unsigned short	F_WORD;					/* 符号無し16bit値			*/
typedef signed   int	F_INT;					/* 符号付  16/32bit値(環境依存)	*/
typedef unsigned int	F_UINT;					/* 符号無し16/32bit値(環境依存)	*/
typedef signed   long	F_LONG;					/* 符号付32bit値			*/
typedef unsigned long	F_DWORD;				/* 符号無し32bit値			*/
typedef F_DWORD			F_PTR;					/* ポインタ					*/

/************************************/
/*	定数値　定義	 				*/
/************************************/

/******** エンディアン *************/
												/* リトルエンディアン時は   */
#define BIG_ENDIAN								/* コメント化               */

/******* I/Oファンクション番号 *****/

#define IO_INIT			0x00					/* 初期化					*/
#define IO_MOUNT		0x01					/* マウント					*/
#define IO_UNMOUNT		0x02					/* アンマウント				*/
#define IO_CHANEL_INFO	0x03					/* チャネル情報取得			*/
#define IO_DEVICE_INFO	0x04					/* デバイス情報取得			*/
#define IO_UNIT_INFO	0x05					/* ユニット情報取得			*/
#define IO_READ			0x06					/* データ読込み				*/
#define IO_WRITE		0x07					/* データ書込み				*/
#define IO_ERASE		0x08					/* データ消去				*/
#define IO_DETECT_INFO	0x09					/* メディア挿抜チェック		*/
#define IO_PART_CREATE	0x0a					/* パーティション作成		*/

/******* チャネル/ユニット番号 *****/

#define ARG_CHANEL		0xF0					/* チャネル番号				*/
#define ARG_UNIT		0x0F					/* ユニット番号				*/

/******* セクタサイズ **************/

#define SEC_SIZE		512						/* セクタサイズ：５１２バイト	*/
#define TRANS_COUNT		(SEC_SIZE/sizeof(F_WORD))	/* PCカードへの転送回数		*/

/******* 最大値 ********************/

#define MAX_CHANEL		1						/* チャネル最大数			*/
#define MAX_DEVICE		1						/* デバイス最大数			*/
#define MAX_UNIT		4     					/* ユニット最大数			*/

/****** エラーコード ***************/

#define DRV_OK			(  0 )					/* 正常終了					*/
#define ERR_PARAM		( -512 )					/* パラメタエラー			*/
#define ERR_NOTINIT		( -513 )					/* 未初期化エラー			*/
#define ERR_NOCARD		( -514 )					/* カード抜きエラー			*/
#define ERR_ILGALVS		( -515 )					/* VS1,2状態エラー			*/
#define ERR_MRSHPC		( -516 )					/* MR-SHPC-01異常			*/
#define ERR_PCCIS		( -517 )					/* PCカード CIS異常			*/
#define ERR_PCREADYTMO	( -518 )					/* PCカードReady TimeOut	*/
#define ERR_PCCARD		( -519 )					/* PCカード異常				*/
#define ERR_TSLP		( -520 )					/* PCATA_osSleep異常終了	*/
#define ERR_UNITOVER	( -521 )					/* 該当ユニット無し			*/
#define ERR_NOTMOUNT	( -522 )					/* 未マウントエラー			*/
#define ERR_DEFCARD		( -523 )					/* 別カード挿入				*/
#define ERR_ILGCARD		( -524 )					/* カード挿抜発生			*/
#define ERR_MOUNT		( -525 )					/* マウント済みエラー		*/
#define ERR_NOPRIMARY	( -526 )					/* 基本領域無し				*/
#define ERR_NOEXTEND	( -527 )					/* 拡張領域無し				*/


/******* PCカード Identify Drive 情報定義 *****/

#define ATAID_CYLINDER	0x01					/* Word Offset 1:Default number of cylinders*/
#define ATAID_HEAD		0x03					/* Word Offset 3:Default number of heads	*/
#define ATAID_SECPTR	0x06					/* Word Offset 6:Default number of sectors per track*/
#define ATAID_SECTOR1	0x3c					/* Word Offset60:Total number of sectors	*/
#define ATAID_SECTOR2	0x3d					/* Word Offset61:Total number of sectors	*/

/****** タイムアウト値定義 *********************/

#define IDE_TOUT		0x7FFF					/* Loop retry count */
#define PCREADY_TIMEOUT	5000					/* PCカードReady待ちタイムアウト時間：5秒	*/

/****** MBR(Master Boot Record) フォーマット *************/

#define MBR_BOOTSIZE	(446/sizeof(F_WORD))	/* ブートコードサイズ						*/
#define MBR_PARTSIZE	(16/sizeof(F_WORD))		/* パーティションテーブルサイズ				*/

#define MBR_PT_NONE		0x00					/* Partition Type = 未使用					*/
#define MBR_PT_FAT12	0x01					/* Partition Type = FAT12					*/
#define MBR_PT_FAT161	0x04					/* Partition Type = FAT16(32MB未満)			*/
#define MBR_PT_EXTEND1	0x05					/* Partition Type = 拡張パーティション		*/
#define MBR_PT_FAT162	0x06					/* Partition Type = FAT16(32MB以上)			*/
#define MBR_PT_FAT163	0x0e					/* Partition Type = FAT16(LBA)				*/
#define MBR_PT_FAT32	0x0c					/* Partition Type = FAT32(LBA)				*/
#define MBR_PT_EXTEND2	0x0f					/* Partition Type = 拡張パーティション(LBA)	*/

#ifdef BIG_ENDIAN                  /* Big Endian                         */
#define MBR_SIGNATURE	0x55aa					/* Signature				*/
#else /* BIG_ENDIAN */             /* Little Endian                      */
#define MBR_SIGNATURE	0xaa55					/* Signature				*/
#endif /* BIG_ENDIAN */

/****** 挿抜状態定義 */

#define	PC_MOUNT		0x00						/* 同一メディアが挿入されている	*/
#define	PC_DEFCARD		0x01						/* 別メディアが挿入されている	*/
#define	PC_NOTMOUNT		0x02						/* メディアが抜かれた			*/

#endif /* __PCATA_DEF_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

