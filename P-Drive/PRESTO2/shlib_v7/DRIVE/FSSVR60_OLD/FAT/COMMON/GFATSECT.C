/*======================================================================
 * File name    : gfatsect.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/gfatsect.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gfatsect.c $
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
/* 関 数 名：FMR_getFatSector                                                 */
/*         ：                                                                 */
/* パラメタ：IN: TFMR_LOGDRV_INFO * 論理ドライブ構成情報                      */
/*         ：    TFMR_FATBUFF * FAT読込み領域                                 */
/*         ：    F_DWORD クラスタ番号                                         */
/*         ：                                                                 */
/*         ：OUT: F_DWORD * FATエントリ値                                     */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  エラーコード                                           */
/*         ：                                                                 */
/* 概    要：FATセクタ読込み                                                  */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_getFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD * fatentry                  /* FATエントリ値                      */
)
{
	F_DWORD l_fatofst;                  /* FATオフセット                      */
	F_DWORD l_fatsectnum;               /* FATセクタ番号                      */
	F_DWORD l_fatsectofst;              /* FATセクタオフセット                */

	F_INT l_drtn;                       /* ドライバエントリ戻り値             */

	if( clstnum > logdrv_info->totalclstnum + 1 )
	{
		/**********************************************************************/
		/* クラスタ番号が許容範囲外                                           */
		/**********************************************************************/
		return DFMR_CLSTNUMERR;         /* 異常終了                           */
	}
	if( logdrv_info->fattype == DFMR_FATTYPE12 )
	{
		/**********************************************************************/
		/* FAT12の場合                                                        */
		/**********************************************************************/
		l_fatofst = clstnum;
		l_fatofst += ( clstnum / 2 );
		
		l_fatsectnum = logdrv_info->fattopsect;
		l_fatsectnum += l_fatofst / logdrv_info->bytepersect;
		
		l_fatsectofst = l_fatofst % logdrv_info->bytepersect;
		l_fatsectofst += 4;
		
		if( l_fatsectofst == ( F_DWORD )( logdrv_info->bytepersect + 3 ) )
		{
			if( l_fatsectnum !=
				( fatbuff->fatsectnum & ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD ) ) )
			{
				l_drtn = FMR_readFatSector(
					logdrv_info,            /* 論理ドライブ構成情報               */
					fatbuff,                /* FAT読込領域                        */
					l_fatsectnum,           /* 読込FATセクタ番号                  */
					4                       /* FAT読込領域オフセット              */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				fatbuff->fatsectnum = l_fatsectnum;
			}
			fatbuff->fat[ 3 ] = fatbuff->fat[ l_fatsectofst ];
			l_fatsectnum ++;
			l_fatsectofst = 3;
		}
		if( l_fatsectnum !=
			( fatbuff->fatsectnum & ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD ) ) )
		{
			l_drtn = FMR_readFatSector(
				logdrv_info,            /* 論理ドライブ構成情報               */
				fatbuff,                /* FAT読込領域                        */
				l_fatsectnum,           /* 読込FATセクタ番号                  */
				4                       /* FAT読込領域オフセット              */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		*( fatentry ) = FMR_getFatValue(
			&( fatbuff->fat[ l_fatsectofst ] ),
			logdrv_info->fattype,
			clstnum
			);
		fatbuff->fatsectnum = l_fatsectnum;
		if( l_fatsectofst == 3 )
		{
			fatbuff->fatsectnum |= DFMR_FWDSECTREAD;	/* 前セクタ読込みフラグON */
		}
		else
		{
			fatbuff->fatsectnum &= ~DFMR_FWDSECTREAD;	/* 前セクタ読込みフラグOFF */
		}
	}
	else
	{
		/*********************************************************************/
		/* FAT16/32の場合                                                    */
		/*********************************************************************/
		if( logdrv_info->fattype == DFMR_FATTYPE16 )
		{
			/*****************************************************************/
			/* FAT16の場合                                                   */
			/*****************************************************************/
			l_fatofst = clstnum * sizeof( F_WORD );
		}
		else
		{
			/*****************************************************************/
			/* FAT32の場合                                                   */
			/*****************************************************************/
			l_fatofst = clstnum * sizeof( F_DWORD );
		}
		l_fatsectnum = logdrv_info->fattopsect;
		l_fatsectnum += l_fatofst / logdrv_info->bytepersect;
		
		l_fatsectofst = l_fatofst % logdrv_info->bytepersect;
		
		if( l_fatsectnum != fatbuff->fatsectnum )
		{
			l_drtn = FMR_readFatSector(
				logdrv_info,            /* 論理ドライブ構成情報               */
				fatbuff,                /* FAT読込領域                        */
				l_fatsectnum,           /* 読込FATセクタ番号                  */
				0                       /* FAT読込領域オフセット              */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		*( fatentry ) = FMR_getFatValue(
			&( fatbuff->fat[ l_fatsectofst ] ),
			logdrv_info->fattype,
			clstnum
			);
		fatbuff->fatsectnum = l_fatsectnum;
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
