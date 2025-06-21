/*======================================================================
 * File name    : gfsize.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/gfsize.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gfsize.c $
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
/* 関 数 名：FMR_getFileSize                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * ディレクトリエントリ                        */
/*         ：                                                                 */
/* 戻 り 値：ファイルサイズ                                                   */
/*         ：                                                                 */
/* 概    要：対象のエントリよりファイルサイズを取得する                       */
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

F_DWORD FMR_getFileSize(
	TFMR_DIRENTRY * l_entry             /* ディレクトリエントリ               */
)
{
	F_DWORD l_size;
#ifdef DFMR_ENDIAN_BIG
	T_ENDIAN l_buff;
#endif /* DFMR_ENDIAN_BIG */
	l_buff.dwordval = l_entry->filesize;
	l_size = ( F_DWORD )l_buff.byteval[ 3 ];
	l_size <<= 8;
	l_size |= ( F_DWORD )l_buff.byteval[ 2 ];
	l_size <<= 8;
	l_size |= ( F_DWORD )l_buff.byteval[ 1 ];
	l_size <<= 8;
	l_size |= ( F_DWORD )l_buff.byteval[ 0 ];
	
#ifdef DFMR_ENDIAN_BIG
	
#else /* DFMR_ENDIAN_BIG */
	l_size = l_entry->filesize;
#endif /* DFMR_ENDIAN_BIG */
	return l_size;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
