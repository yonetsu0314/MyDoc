/*======================================================================
 * File name    : inident.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/inident.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: inident.c $
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
/* 関 数 名：FMR_initDirEntry                                                 */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * エントリ作成用ポインタ                      */
/*         ：     F_DWORD         開始クラスタ番号                            */
/*         ：     F_BYTE          ドットディレクティブ数                      */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ディレクトリエントリ初期設定                                     */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_initDirEntry(
	TFMR_DIRENTRY * dirptr,             /* エントリ作成用ポインタ             */
    F_DWORD         clstnum,            /* クラスタ番号                       */
    F_BYTE          dotcnt,             /* ドットディレクティブ数             */
    F_BYTE          attr                /* エントリ属性                       */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */

	FMR_memSet(
		&( dirptr->name[ dotcnt ] ),
		DFMR_SPACE,
		( DFMR_VNAMESZ - dotcnt )
		);
	dirptr->attr = attr;                /* エントリ属性                       */
	dirptr->ntres = DFMR_INITBYTE;      /* 予約領域は0(WinNTが使うので)       */
	FMR_osGetTime(                      /* 作成日時をOSより取得               */
		&( dirptr->crtdate ),
		&( dirptr->crttime ),
		&( dirptr->crttimetenth )
		);
	dirptr->lstaccdate = dirptr->crtdate;   /* アクセス日は作成日付と同じ     */
	dirptr->wrtdate = dirptr->crtdate;      /* 更新日時は作成日時と同じ       */
	dirptr->wrttime = dirptr->crttime;
	l_work01 = clstnum & 0xFFFF0000;
	l_work01 >>= 16;
	dirptr->fstclushi = ( F_WORD )l_work01;
	l_work01 = clstnum & 0x0000FFFF;
	dirptr->fstcluslo = ( F_WORD )l_work01;
	dirptr->filesize = 0;               /* サイズは0                          */
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
