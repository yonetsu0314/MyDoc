/*======================================================================
 * File name    : sfsize.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/sfsize.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sfsize.c $
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
/* 関 数 名：FMR_setFileSize                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * ディレクトリエントリ                        */
/*         ：     F_DWORD ファイルサイズ                                      */
/*         ：                                                                 */
/* 戻 り 値：なし                                                             */
/*         ：                                                                 */
/* 概    要：対象のエントリのファイルサイズを設定する                         */
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

F_VOID FMR_setFileSize(
	TFMR_DIRENTRY * entry,              /* ディレクトリエントリ               */
	F_DWORD size                        /* ファイルサイズ                     */
)
{
#ifdef DFMR_ENDIAN_BIG
	T_ENDIAN l_buff;
#endif /* DFMR_ENDIAN_BIG */
#ifdef DFMR_ENDIAN_BIG
	l_buff.dwordval = size;
	entry->filesize = ( F_DWORD )l_buff.byteval[ 3 ];
	entry->filesize <<= 8;
	entry->filesize |= ( F_DWORD )l_buff.byteval[ 2 ];
	entry->filesize <<= 8;
	entry->filesize |= ( F_DWORD )l_buff.byteval[ 1 ];
	entry->filesize <<= 8;
	entry->filesize |= ( F_DWORD )l_buff.byteval[ 0 ];
	
#else /* DFMR_ENDIAN_BIG */
	entry->filesize = ( F_DWORD )size;

#endif /* DFMR_ENDIAN_BIG */
	return;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
