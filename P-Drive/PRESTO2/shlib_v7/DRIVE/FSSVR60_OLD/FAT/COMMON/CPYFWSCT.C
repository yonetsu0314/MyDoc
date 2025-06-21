/*======================================================================
 * File name    : cpyfwsct.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/cpyfwsct.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cpyfwsct.c $
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
/* 関 数 名：FMR_copyForwardSector                                            */
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
/* 概    要：クラスタ内の指定セクタより前のセクタをコピーする                 */
/*         ：                                                                 */
/* 作 成 日：2004.04.**                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_copyForwardSector(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD srcsectnum,                 /* コピー元セクタ番号                 */
    F_DWORD targetsectnum               /* コピー先セクタ番号                 */
)
{
	F_DWORD l_srcsectcnt;               /* コピー元セクタカウンタ             */
	F_DWORD l_targetsectcnt;            /* コピー先セクタカウンタ             */

	F_INT l_drtn;                       /* ドライバエントリ戻り値             */

	/* パラメータ設定 */
	/* 任意のセクタAが属するクラスタの先頭セクタ番号の取得 */
	/* 1.セクタ番号AからData Region先頭セクタ番号を差引く */
	/* 2.1.で求めたセクタ番号をクラスタ当りのセクタ数で割る */
	/* 3.2.で求めた値にをクラスタ当りのセクタ数を掛ける */
	
	l_srcsectcnt = srcsectnum - MFMR_DATATOP( drvnum );
	l_srcsectcnt /= MFMR_SECTPERCLST( drvnum );
	l_srcsectcnt *= MFMR_SECTPERCLST( drvnum );
	l_srcsectcnt += MFMR_DATATOP( drvnum );
	
	l_targetsectcnt = targetsectnum - MFMR_DATATOP( drvnum );
	l_targetsectcnt /= MFMR_SECTPERCLST( drvnum );
	l_targetsectcnt *= MFMR_SECTPERCLST( drvnum );
	l_targetsectcnt += MFMR_DATATOP( drvnum );
	
	while( l_srcsectcnt < srcsectnum )
	{
		/* コピー元セクタ読込み設定 */
		l_drtn = FMR_readDataSector(
			drvnum,
			l_srcsectcnt,
			0
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		/* コピー先セクタ書込み設定 */
		l_drtn = FMR_setDataSector(
			drvnum,
			l_targetsectcnt,
			0
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		
		/* パラメータ更新 */
		l_srcsectcnt ++;
		l_targetsectcnt ++;
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
