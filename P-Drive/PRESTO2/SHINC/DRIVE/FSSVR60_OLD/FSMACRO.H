/*======================================================================
 * File name    : fsmacro.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/fsmacro.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsmacro.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/16 18:00
 * Saver60 Ver.01.04 up
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
/******************************************************************************/
/* 関 数 名：FMR_Macro.h                                                      */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：マクロ定義                                                       */
/*         ：                                                                 */
/* 作 成 日：2004.04.15                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロ削除                        04/12/10 Y.Yoshizumi */
/*    MFMR_RWBUFFTOP,MFMR_RWSECTNUM,MFMR_RWBUFFマクロ追加04/12/10 Y.Yoshizumi */
/******************************************************************************/

#ifndef __FMR_MACRO_H__
#define __FMR_MACRO_H__

/* フラグON/OFFの切り替え */
#define MFMR_FLGSWITCH( flg ) if( flg == 0 ) flg = 1; else flg = 0;

/* EOCのコード(設定用)を決定 */
#define MFMR_GETEOC( pt,eoc )         \
{                                     \
	switch( pt )                      \
	{                                 \
	case DFMR_FATTYPE12:              \
		eoc = DFMR_EOC12;             \
		break;                        \
	case DFMR_FATTYPE16:              \
		eoc = DFMR_EOC16;             \
		break;                        \
	case DFMR_FATTYPE32:              \
	default:                          \
		eoc = DFMR_EOC32;             \
		break;                        \
	}                                 \
}

/* EOCのコード(判定用)を決定 */
#define MFMR_GETEOCMIN( pt,eoc )      \
{                                     \
	switch( pt )                      \
	{                                 \
	case DFMR_FATTYPE12:              \
		eoc = DFMR_EOC12MIN;          \
		break;                        \
	case DFMR_FATTYPE16:              \
		eoc = DFMR_EOC16MIN;          \
		break;                        \
	case DFMR_FATTYPE32:              \
	default:                          \
		eoc = DFMR_EOC32MIN;          \
		break;                        \
	}                                 \
}

/* BADクラスタのコードを決定 */
#define MFMR_GETBAD( pt,bad )         \
{                                     \
	switch( pt )                      \
	{                                 \
	case DFMR_FATTYPE12:              \
		bad = DFMR_BAD12;             \
		break;                        \
	case DFMR_FATTYPE16:              \
		bad = DFMR_BAD16;             \
		break;                        \
	case DFMR_FATTYPE32:              \
	default:                          \
		bad = DFMR_BAD32;             \
		break;                        \
	}                                 \
}
/******************************************/
/* RAM領域アクセス                        */
/******************************************/
/* 論理ドライブ構成情報                   */
/*     パラメータ:                        */
/*         a :論理ドライブ番号            */
/******************************************/
/* 論理ドライブ構成情報               */
#define MFMR_DRVINFO(a) \
( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo )
/* 論理ドライブ番号                   */
#define MFMR_DRVNUM(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> drvnum )
/* ドライバID                         */
#define MFMR_DRVID(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> drvid )
/* チャネル/ユニット番号              */
#define MFMR_CUNUM(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> channelunitnum )
/* クラスタ当りのセクタ数             */
#define MFMR_SECTPERCLST(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> sectperclst )
/* FATタイプ                          */
#define MFMR_FATTYPE(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> fattype )
/* FAT数                              */
#define MFMR_FATCNT(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> fatcnt )
/* セクタ当りのバイト数               */
#define MFMR_BYTEPERSECT(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> bytepersect )
/* ディレクトリエントリ数             */
#define MFMR_DIRENT(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> direntcnt )
/* セマフォID                         */
#define MFMR_SEMID(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> semid )
/* アクティブFATフラグ                */
#define MFMR_ACTFATFLG(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> actfatflg )
/* FSINFOセクタ番号                   */
#define MFMR_FSINFO(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> fsinfosect )
/* ドライバエントリ                   */
#define MFMR_DRVENTRY(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> drventry )
/* パーティション全体のセクタ数       */
#define MFMR_PARTSECTNUM(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> partsectnum )
/* FAT1件当りのセクタ数               */
#define MFMR_SECTPERFAT(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> sectperfat )
/* FAT Regionの先頭セクタ番号         */
#define MFMR_FATTOP(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> fattopsect )
/* ルートディレクトリの先頭セクタ番号 */
#define MFMR_ROOTTOP(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> roottopsect )
/* Data Regionの先頭セクタ番号        */
#define MFMR_DATATOP(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> datatopsect )
/* 総クラスタ数                       */
#define MFMR_TOTALCLST(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> totalclstnum )
/* ボリュームID                       */
#define MFMR_VOLID(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> volumeid )
/* ボリューム名                       */
#define MFMR_VOLNAME(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].logdrvinfo ) -> volumename )

/******************************************/
/* FAT読込み領域                          */
/*     パラメータ:                        */
/*         a :論理ドライブ番号            */
/*         b :FAT読込み領域番号           */
/*         c :読込み領域のオフセット      */
/******************************************/
/* FAT読込み領域                      */
#define MFMR_FATTABLE(a,b) \
( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatbuff + b )
/* 読込まれているFATセクタ番号        */
#define MFMR_FATSECTNUM(a,b) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatbuff + b ) -> fatsectnum )
/* FAT読込み領域                      */
#define MFMR_FATSECTAREA(a,b,c) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatbuff + b ) -> fat[ c ] )

/******************************************/
/* FATセクタ管理情報                      */
/*     パラメータ:                        */
/*         a :論理ドライブ番号            */
/*         b :セクタ管理テーブル番号      */
/******************************************/
/* FATセクタ管理情報                  */
#define MFMR_FATSECTINFO(a) \
( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatsectarea )
/* 開始FATセクタ番号                  */
#define MFMR_FATSECTTOP(a) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatsectarea ) -> startfatsect )
/* 空きクラスタ数                     */
#define MFMR_FATEMPTYNUM(a,b) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatsectarea ) -> sectadmintbl[ b ].freeclstnum )
/* 最小空きクラスタ番号               */
#define MFMR_FATEMPTYMIN(a,b) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].fatsectarea ) -> sectadmintbl[ b ].leastfreeclst )

/******************************************/
/* R/Wバッファ                            */
/*     パラメータ:                        */
/*         a :論理ドライブ番号            */
/*         b :R/Wバッファ番号             */
/*         c :R/Wバッファのオフセット     */
/******************************************/
/* R/Wバッファ先頭アドレス            */
#define MFMR_RWBUFFTOP(a,b) \
( &( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].rwbuffarea + b ) -> rwbuff[ 0 ] ) )
/* R/Wセクタ番号                      */
#define MFMR_RWSECTNUM(a,b) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].rwbuffarea + b ) -> rwsectnum )
/* R/Wバッファ内容                    */
#define MFMR_RWBUFF(a,b,c) \
( ( tfmr_part_ramaddr[ a - DFMR_TOPDRVNUM ].rwbuffarea + b ) -> rwbuff[ c ] )

#endif /* __FMR_MACRO_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
