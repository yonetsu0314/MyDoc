/*======================================================================
 * File name    : gfatrtry.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/gfatrtry.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gfatrtry.c $
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
/* 関 数 名：FMR_getFatSectorRetry                                            */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_LOGDRV_INFO * 論理ドライブ構成情報                     */
/*         ：     TFMR_FATBUFF * FAT読込み領域                                */
/*         ：     F_DWORD クラスタ番号                                        */
/*         ：     F_DWORD * FATエントリ値                                     */
/*         ：                                                                 */
/* 戻 り 値：DFMR_OK 正常終了                                                 */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：対象クラスタ番号が属するFATセクタを読込み、FATエントリを取得する */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_INT FMR_getFatSectorRetry(
	TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
	TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_DWORD * fatentry                  /* FATエントリ値                      */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_retrycnt;                   /* リトライカウンタ                   */
	
	for( l_retrycnt = 0; l_retrycnt < DFMR_USR_SDRETRY; l_retrycnt ++ )
	{
		/* FATセクタを読込む */
		l_rtn = FMR_getFatSector(
			logdrv_info,                /* 論理ドライブ構成情報               */
			fatbuff,                    /* FAT読込み領域                      */
			clstnum,                    /* クラスタ番号                       */
			fatentry                    /* FATエントリ値                      */
			);
		if( l_rtn == DFMR_OK )          /* 正常終了の場合は処理終了           */
		{
			break;
		}
		FMR_osRetryWait( );             /* ウェイト                           */
	}
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
