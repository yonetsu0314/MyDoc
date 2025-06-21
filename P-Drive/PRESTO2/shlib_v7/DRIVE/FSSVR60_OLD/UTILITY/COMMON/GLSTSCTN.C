/*======================================================================
 * File name    : glstsctn.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/glstsctn.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: glstsctn.c $
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
/* 関 数 名：FMR_getLastSectNum                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD セクタ番号                                          */
/*         ：                                                                 */
/* 戻 り 値：最終セクタ番号                                                   */
/*         ：                                                                 */
/* 概    要：セクタが属するクラスタ内の最終セクタ番号を取得する               */
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

F_DWORD FMR_getLastSectNum(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum                     /* セクタ番号                         */
)
{
	F_DWORD l_lastsectnum;              /* 対象クラスタ内最終セクタ番号       */
	F_DWORD l_calc;                     /* 計算用                             */
	
	if( sectnum < MFMR_DATATOP( drvnum ) )      /* 対象がRoot Dir. Reg.の場合 */
	{
		l_lastsectnum = MFMR_DATATOP( drvnum ) - 1;     /* Data Reg.先頭-1    */
	}
	else
	{
		l_lastsectnum = sectnum - MFMR_DATATOP( drvnum );
		l_calc = l_lastsectnum / MFMR_SECTPERCLST( drvnum );
		l_lastsectnum = l_calc * MFMR_SECTPERCLST( drvnum );
		l_lastsectnum += MFMR_SECTPERCLST( drvnum ) - 1;
		l_lastsectnum += MFMR_DATATOP( drvnum );
	}
	return l_lastsectnum;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
