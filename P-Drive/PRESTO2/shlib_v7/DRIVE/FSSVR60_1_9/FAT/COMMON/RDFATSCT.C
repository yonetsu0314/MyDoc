/*======================================================================
 * File name    : rdfatsct.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/rdfatsct.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rdfatsct.c $
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
/* 関 数 名：FMR_readFatSector                                                */
/*         ：                                                                 */
/* パラメタ：IN: TFMR_LOGDRV_INFO * 論理ドライブ構成情報                      */
/*         ：    TFMR_FATBUFF *     FAT読込領域                               */
/*         ：    F_DWORD            読込FATセクタ番号                         */
/*         ：    F_WORD             FAT読込領域オフセット                     */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  エラーコード                                           */
/*         ：                                                                 */
/* 概    要：FATセクタ読込み&ミラーリングFAT読込み                            */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_readFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF     * fatbuff,         /* FAT読込領域                        */
    F_DWORD            sectnum,         /* 読込FATセクタ番号                  */
    F_WORD             fatbuffoffset    /* FAT読込領域オフセット              */
)
{
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/O                        */
	F_INT           l_drtn;             /* ドライバエントリ戻り値             */
	F_WORD          l_sectcnt;          /* ドライバI/O用                      */
	F_BYTE          l_fatcnt;           /* FAT数                              */
	F_BYTE          l_cnt01;            /* ループカウンタ                     */

	/**************************************************************************/
	/* FATセクタ読込み                                                        */
	/**************************************************************************/
	l_fatcnt = logdrv_info->fatcnt;
	l_drvioparam.read.startsctnum = sectnum;
	l_sectcnt = 1;
	l_drvioparam.read.readsctnum = &l_sectcnt;
	l_drvioparam.read.readbuff = &( fatbuff->fat[ fatbuffoffset ] );
	l_drtn = ( logdrv_info->drventry )(
		logdrv_info->channelunitnum,
		DFMR_DRVFUNCREAD,
		&l_drvioparam
		);
	if( (l_drtn != DFMR_OK) ||
		(l_sectcnt != 1) )
	{
		/**********************************************************************/
		/* 処理失敗又はセクタ数異常時                                         */
		/**********************************************************************/
		if( l_fatcnt < 2 )
		{
			/******************************************************************/
			/* ミラーリングFATセクタがない場合                                */
			/******************************************************************/
			fatbuff->fatsectnum = 0;
			return DFMR_FATERR;         /* FATエラー                          */
		}
		/* ミラーリングFATセクタを読込む */
		for( l_cnt01 = 1; l_cnt01 < l_fatcnt; l_cnt01 ++ )
		{
			l_drvioparam.read.startsctnum = sectnum + logdrv_info->sectperfat * l_cnt01;
			l_sectcnt = 1;
			l_drvioparam.read.readsctnum = &l_sectcnt;
			l_drtn = ( logdrv_info->drventry )(
				logdrv_info->channelunitnum,
				DFMR_DRVFUNCREAD,
				&l_drvioparam
				);
			if( (l_drtn != DFMR_OK) ||
				(l_sectcnt != 1) )
			{
				/* 処理失敗又はセクタ数異常時 */
				continue;
			}
			break;
		}
		if ( l_cnt01 >= l_fatcnt )
		{
			fatbuff->fatsectnum = 0;
			return DFMR_FATERR;         /* FATエラー                          */
		}
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
