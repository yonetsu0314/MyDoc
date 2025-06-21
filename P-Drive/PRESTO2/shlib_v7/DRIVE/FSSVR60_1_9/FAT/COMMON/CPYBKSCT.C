/*======================================================================
 * File name    : cpybksct.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/cpybksct.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cpybksct.c $
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
/* 関 数 名：FMR_copyBackSector                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_DWORD コピー元セクタ番号                                  */
/*         ：     F_DWORD コピー先セクタ番号                                  */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：クラスタ内の指定セクタより後のセクタをコピーする                 */
/*         ：                                                                 */
/* 作 成 日：2004.06.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_copyBackSector(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD srcsectnum,                 /* コピー元セクタ番号                 */
    F_DWORD targetsectnum               /* コピー先セクタ番号                 */
)
{
	F_DWORD l_srclastsect;              /* コピー元最終セクタ番号             */
	
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */

	/* パラメータ設定 */
	l_srclastsect = srcsectnum - MFMR_DATATOP( drvnum );
	l_srclastsect /= MFMR_SECTPERCLST( drvnum );
	l_srclastsect *= MFMR_SECTPERCLST( drvnum );
	l_srclastsect += MFMR_DATATOP( drvnum );
	l_srclastsect += MFMR_SECTPERCLST( drvnum ) - 1;
	
	srcsectnum ++;
	targetsectnum ++;
	
	while( srcsectnum <= l_srclastsect )
	{
		/* コピー元セクタ読込み設定 */
		l_drtn = FMR_readDataSector(
			drvnum,
			srcsectnum,
			0
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		
		/* コピー先セクタ書込み設定 */
		l_drtn = FMR_setDataSector(
			drvnum,
			targetsectnum,
			0
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		
		/* パラメータ更新 */
		srcsectnum ++;
		targetsectnum ++;
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
