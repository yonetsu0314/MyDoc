/*======================================================================
 * File name    : ctrlfat.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/ctrlfat.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ctrlfat.c $
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
/* 関 数 名：FMR_controlFatEntry                                              */
/*         ：                                                                 */
/* パラメタ：IN: TFMR_LOGDRV_INFO * 論理ドライブ構成情報                      */
/*         ：    TFMR_FATBUFF * FAT読込み領域                                 */
/*         ：    F_DWORD クラスタ番号                                         */
/*         ：    F_BYTE Read/Write種別 OFF:取得,ON:設定                       */
/*         ：                                                                 */
/*         ：IN/OUT: F_DWORD * FATエントリ値                                  */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_CLSTNUMERR                                        */
/*         ：          DFMR_NOFATENTRY                                        */
/*         ：                                                                 */
/* 概    要：FATエントリの値取得/設定                                         */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_controlFatEntry(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD * fatentry,                 /* FATエントリ値                      */
    F_BYTE ctrlflg                      /* Read/Write種別 OFF:取得,ON:設定    */
)
{
	F_DWORD l_fatofst;                  /* FATオフセット                      */
	F_DWORD l_fatsectnum;               /* FATセクタ番号                      */
	F_DWORD l_fatsectofst;              /* FATセクタオフセット                */
	F_DWORD l_work01;
	
	if( clstnum > logdrv_info->totalclstnum + 1 )
	{
		/**********************************************************************/
		/* クラスタ番号が許容範囲外                                           */
		/**********************************************************************/
		return DFMR_CLSTNUMERR;           /* 異常終了                           */
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
			l_fatsectnum ++;
			l_fatsectofst = 3;
			
			l_work01 = fatbuff->fatsectnum;
			l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
			if( l_fatsectnum != l_work01 )
			{
				return DFMR_NOFATENTRY;
			}
			
			if( ( fatbuff->fatsectnum & DFMR_FWDSECTREAD) != DFMR_FWDSECTREAD )
			{							/* 前セクタ読込みフラグOFFの場合      */
				return DFMR_NOFATENTRY;
			}
			
			if( ctrlflg == DFMR_FLGOFF )
			{
				/**************************************************************/
				/* 取得指定                                                   */
				/**************************************************************/
				*( fatentry ) = FMR_getFatValue(
					&( fatbuff->fat[ l_fatsectofst ] ),
					logdrv_info->fattype,
					clstnum
					);
				return DFMR_OK;
			}
			else
			{
				/**************************************************************/
				/* 設定指定                                                   */
				/**************************************************************/
				FMR_setFatValue(
					&( fatbuff->fat[ l_fatsectofst ] ),
					logdrv_info->fattype,
					clstnum,
					*( fatentry )
					);
				fatbuff->fatsectnum |= DFMR_FWDSECTUPDATE;	/* 前セクタ更新フラグON */
				return DFMR_OK;
			}
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
		
	}
	
	l_work01 = fatbuff->fatsectnum;
	l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
											/* 前セクタ読込み・更新フラグOFF */
	if( l_fatsectnum != l_work01 )
	{
		return DFMR_NOFATENTRY;
	}
	
	if( ctrlflg == DFMR_FLGOFF )
	{
		/**********************************************************************/
		/* 取得指定                                                           */
		/**********************************************************************/
		*( fatentry ) = FMR_getFatValue(
			&( fatbuff->fat[ l_fatsectofst ] ),
			logdrv_info->fattype,
			clstnum
			);
	}
	else
	{
		/**********************************************************************/
		/* 設定指定                                                           */
		/**********************************************************************/
		FMR_setFatValue(
			&( fatbuff->fat[ l_fatsectofst ] ),
			logdrv_info->fattype,
			clstnum,
			*( fatentry )
			);
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
