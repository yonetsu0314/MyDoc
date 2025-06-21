/*======================================================================
 * File name    : sbgedent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/sbgedent.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sbgedent.c $
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
/* 関 数 名：FMR_setBigEndianEntry                                            */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * 設定エントリポインタ                        */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：VOID                                                             */
/*         ：                                                                 */
/* 概    要：指定されたエントリをBigEndian用に変換する                        */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_setBigEndianEntry(
    TFMR_DIRENTRY * entry               /* 設定エントリポインタ               */
)
{

#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */

	T_ENDIAN l_endian;                  /* エンディアン調整用ワーク           */

	l_endian.wordval = entry->crttime;			/* 作成時刻                   */
	entry->crttime = (F_WORD)(l_endian.byteval[1]);
	entry->crttime = (entry->crttime << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->crtdate;			/* 作成日付                   */
	entry->crtdate = (F_WORD)(l_endian.byteval[1]);
	entry->crtdate = (entry->crtdate << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->lstaccdate;		/* アクセス日                 */
	entry->lstaccdate = (F_WORD)(l_endian.byteval[1]);
	entry->lstaccdate = (entry->lstaccdate << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->fstclushi;		/* 開始クラスタ(上位)         */
	entry->fstclushi = (F_WORD)(l_endian.byteval[1]);
	entry->fstclushi = (entry->fstclushi << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->wrttime;			/* 更新時刻                   */
	entry->wrttime = (F_WORD)(l_endian.byteval[1]);
	entry->wrttime = (entry->wrttime << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->wrtdate;			/* 更新日付                   */
	entry->wrtdate = (F_WORD)(l_endian.byteval[1]);
	entry->wrtdate = (entry->wrtdate << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->fstcluslo;		/* 開始クラスタ(下位)         */
	entry->fstcluslo = (F_WORD)(l_endian.byteval[1]);
	entry->fstcluslo = (entry->fstcluslo << 8) | (F_WORD)(l_endian.byteval[0]);

	l_endian.dwordval = entry->filesize;		/* サイズ                     */
	entry->filesize = (F_DWORD)(l_endian.byteval[3]);
	entry->filesize = (entry->filesize << 8) | (F_DWORD)(l_endian.byteval[2]);
	entry->filesize = (entry->filesize << 8) | (F_DWORD)(l_endian.byteval[1]);
	entry->filesize = (entry->filesize << 8) | (F_DWORD)(l_endian.byteval[0]);
#endif /* DFMR_ENDIAN_BIG */

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
