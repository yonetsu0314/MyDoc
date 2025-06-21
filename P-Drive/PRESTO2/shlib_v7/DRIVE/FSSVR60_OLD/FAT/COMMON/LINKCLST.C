/*======================================================================
 * File name    : linkclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/linkclst.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: linkclst.c $
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
/* 関 数 名：FMR_linkNewCluster                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_DWORD   リンク対象クラスタ番号                            */
/*         ：     F_DWORD * FATエントリ設定値                                 */
/*         ：     F_BYTE  * 現在参照中のFAT読込み領域番号                     */
/*         ：     F_CHAR    対象ドライブ番号                                  */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_MEMFULL                                           */
/*         ：                                                                 */
/* 概    要：リンク対象クラスタに指定されたFATエントリ値を設定する            */
/*         ：設定するFATエントリ値が属するFATセクタがFAT読込み領域１に        */
/*         ：読込まれている事が前提                                           */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_linkNewCluster(
	F_DWORD   linkclstnum,              /* リンク対象クラスタ番号             */
	F_DWORD * fatentrynum,              /* FATエントリ設定値                  */
	F_BYTE  * fatareanum,               /* 現在参照中のFAT読込み領域番号      */
    F_CHAR    drvnum                    /* 対象ドライブ番号                   */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_INT   l_drtn;                     /* サブルーチン戻り値                 */

	if( *( fatareanum ) != DFMR_REFFATAREA0 )
	/* 現在参照中FAT読込み領域が0以外の場合 */
	{
		/**********************************************************************/
		/* 現在参照中FAT読込み領域が1の場合、リンク対象クラスタ番号           */
		/* (linkclstnum)の属するFATセクタをFAT読込み領域1に読込む             */
		/**********************************************************************/
		l_drtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( drvnum, *( fatareanum ) ),
			                            /* FAT読込み領域アドレス              */
			linkclstnum,                /* 読込クラスタ番号                   */
			&l_work01                   /* 取得FATエントリ値                  */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	/**************************************************************************/
	/* リンク対象クラスタ番号(linkclstnum)のFATエントリにFATエントリ設定値    */
	/* (fatentrynum)を設定する                                                */
	/**************************************************************************/
	l_drtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報アドレス       */
		MFMR_FATTABLE( drvnum, *( fatareanum ) ),
		                                /* FAT読込み領域アドレス              */
		linkclstnum,                    /* リンク対象クラスタ番号             */
		fatentrynum,                    /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別：Write指定          */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	l_work01 = MFMR_FATSECTNUM(drvnum, DFMR_REFFATAREA0);
	l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
	l_work02 = MFMR_FATSECTNUM(drvnum, DFMR_REFFATAREA1);
	l_work02 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
	/**************************************************************************/
	/* リンク対象クラスタ番号(linkclstnum)の属するFATセクタを書き戻す         */
	/**************************************************************************/
	l_drtn = FMR_setFatSector(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報アドレス       */
		MFMR_FATTABLE( drvnum, *( fatareanum ) ),
		                                /* FAT読込み領域アドレス              */
		*( fatareanum )                 /* FAT読込み領域番号                  */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	if( *( fatareanum ) != DFMR_REFFATAREA0 )
	{
		if( l_work01 == l_work02 )
		{
			/******************************************************************/
			/* FAT読込み領域0,1共、同一FATセクタを読込んでいる場合、          */
			/* 上で書き戻したFATセクタをFAT読込み領域0に読込む                */
			/******************************************************************/
			l_drtn = FMR_getFatSector(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報アドレス       */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域アドレス              */
				linkclstnum,            /* 読込クラスタ番号                   */
				&l_work01               /* 取得FATエントリ値                  */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		/**********************************************************************/
		/* 設定したFATエントリ値(fatentrynum)が属するFATセクタを              */
		/* FAT読込み領域1に読込む(元の読込状態に戻す)                         */
		/**********************************************************************/
		l_drtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( drvnum, *( fatareanum ) ),
			                            /* FAT読込み領域アドレス              */
			*( fatentrynum),            /* 読込クラスタ番号                   */
			&l_work01                   /* 取得FATエントリ値                  */
			);
	}
	else
	{
		if( l_work01 != l_work02 )
		{
			/******************************************************************/
			/* FAT読込み領域0,1で、別のFATセクタを読込んでいる場合            */
			/* 現在参照中のFAT読込み領域を1とする                             */
			/******************************************************************/
			*( fatareanum ) = DFMR_REFFATAREA1;
		}
	}
	return l_drtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
