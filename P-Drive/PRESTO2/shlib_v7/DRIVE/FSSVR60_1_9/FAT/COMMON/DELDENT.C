/*======================================================================
 * File name    : deldent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/deldent.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: deldent.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/16 21:00
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
/* 関 数 名：FMR_deleteDirEntry                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_DWORD   削除対象エントリ登録セクタ番号                    */
/*         ：     F_DWORD   削除対象最終LFNエントリ登録セクタ番号             */
/*         ：     F_BYTE    削除対象エントリ登録位置                          */
/*         ：     F_BYTE    削除対象最終LFNエントリ登録位置                   */
/*         ：     F_BYTE    動作フラグ:FMR_searchEntry 動作フラグと同じ       */
/*         ：     F_CHAR    対象ドライブ番号                                  */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_MEMFULL etc                                       */
/*         ：                                                                 */
/* 概    要：指定エントリ削除                                                 */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_deleteDirEntry(
	F_DWORD targetsect,                 /* 削除対象エントリ登録セクタ番号     */
	F_DWORD targetlfnsect,              /* 削除対象最終LFNエントリ登録セクタ番号 */
	F_BYTE  targetpos,                  /* 削除対象エントリ登録位置           */
	F_BYTE  targetlfnpos,               /* 削除対象最終LFNエントリ登録位置    */
    F_BYTE  prsflg,                     /* 動作フラグ                         */
    F_CHAR  drvnum                      /* 対象ドライブ番号                   */
)
{
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */

	F_INT   l_rtn;                      /* サブルーチン戻り値                 */

	if( ( prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		/**********************************************************************/
		/* 対象ディレクトリがロングファイル名の場合                           */
		/**********************************************************************/
		l_toppos.sectnum = targetlfnsect;       /* 退避しておいた各パラメータ */
		l_toppos.slot = targetlfnpos;           /* を再設定                   */
		l_toppos.bufnum = 0;                    /* 対象バッファ番号 = 0       */
		l_bottompos.sectnum = targetsect;
		l_bottompos.slot = targetpos;
		l_bottompos.bufnum = 1;                 /* 対象バッファ番号 = 1       */
		/* R/Wバッファ0に削除対象エントリが存在するセクタを読込む */
		l_rtn = FMR_readDataSector(
			drvnum,
			l_toppos.sectnum,
			l_toppos.bufnum
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		if( ( l_bottompos.sectnum != DFMR_INITBYTE )
			&& ( l_bottompos.bufnum != l_toppos.bufnum ) )
		{
			/* R/Wバッファ1にショートファイル名エントリが存在するセクタを読込 */
			l_rtn = FMR_readDataSector(
				drvnum,
				l_bottompos.sectnum,
				l_bottompos.bufnum
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		/* ロングファイル名エントリ削除処理 */
		l_rtn = FMR_deleteLFNEntry(
			drvnum,                     /* 対象論理ドライブ番号               */
			&l_toppos,                  /* 先頭位置                           */
			&l_bottompos                /* 最終位置                           */
			);
	}
	else
	{
		/**********************************************************************/
		/* 対象ディレクトリがショートファイル名の場合                         */
		/**********************************************************************/
		/* R/Wバッファ0に削除対象エントリが存在するセクタを読込む */
		l_rtn = FMR_readDataSector(
			drvnum,
			targetsect,
			0
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/* R/Wバッファ上の対象エントリを削除済みとする */
		MFMR_RWBUFF( drvnum, 0, targetpos * sizeof( TFMR_DIRENTRY ) ) = DFMR_DELENTRY;
		
		/* R/Wバッファを対象セクタに書込む */
		SetBackupFlg(drvnum);
		l_rtn = FMR_setDataSector(
			drvnum,
			targetsect,
			0
			);
		ClearBackupFlg(drvnum);
	}
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
