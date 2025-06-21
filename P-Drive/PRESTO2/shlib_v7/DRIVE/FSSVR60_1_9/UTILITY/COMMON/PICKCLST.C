/*======================================================================
 * File name    : pickclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/pickclst.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: pickclst.c $
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
/* 関 数 名：FMR_pickTbdClst                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：未使用クラスタを摘出る                                           */
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

F_INT FMR_pickTbdClst(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_clstcnt;                  /* クラスタ番号                       */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_DWORD l_bad;                      /* BADクラスタコード                  */
	
	/* BADクラスタコードを取得する */
	MFMR_GETBAD( MFMR_FATTYPE( drvnum ), l_bad )
	/* FAT読込み領域0に先頭FATセクタを読込む */
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),  /* FAT読込み領域0         */
		DFMR_TOPCLSTNUM,                /* クラスタ番号                       */
		&l_fatentry                     /* 取得FATエントリ                    */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	if( ( l_fatentry == DFMR_NEWCLUS )  /* 未使用又はBADクラスタの場合        */
		|| ( l_fatentry == l_bad ) )
	{
		FMR_framBitOff(                 /* BitをOFFとする                     */
			DFMR_TOPCLSTNUM,            /* 先頭クラスタ番号                   */
			ramaddr                     /* クラスタ使用状況展開領域           */
			);
	}
	
	for( l_clstcnt = DFMR_TOPCLSTNUM + 1; l_clstcnt < MFMR_TOTALCLST( drvnum ); l_clstcnt ++ )
	{
		/* クラスタ番号に対応するFATエントリを取得する */
		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),  /* FAT読込み領域0     */
			l_clstcnt,                  /* クラスタ番号                       */
			&l_fatentry,                /* 取得FATエントリ                    */
			DFMR_FLGOFF                 /* FATエントリ取得                    */
			);
		if( l_rtn != DFMR_OK )
		{
			if( l_rtn == DFMR_CLSTNUMERR )
			{                           /* クラスタ番号異常の場合は異常終了   */
				return l_rtn;
			}
			/* 正常終了ではない場合、対象クラスタは次のFATセクタに所属と判断 */
			l_rtn = FMR_getFatSectorRetry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),  /* FAT読込み領域0 */
				l_clstcnt,              /* クラスタ番号                       */
				&l_fatentry             /* 取得FATエントリ                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		if( ( l_fatentry == DFMR_NEWCLUS )
			|| ( l_fatentry == l_bad ) )/* 未使用又はBADクラスタの場合        */
		{
			FMR_framBitOff(             /* BitをOFFとする                     */
				l_clstcnt,              /* 先頭クラスタ番号                   */
				ramaddr                 /* クラスタ使用状況展開領域           */
				);
		}
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
