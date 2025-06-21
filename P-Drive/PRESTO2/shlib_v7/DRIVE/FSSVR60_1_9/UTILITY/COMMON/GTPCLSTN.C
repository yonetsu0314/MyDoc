/*======================================================================
 * File name    : gtpclstn.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/gtpclstn.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gtpclstn.c $
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
/* 関 数 名：FMR_getTopClstNum                                                */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * ディレクトリエントリ                        */
/*         ：                                                                 */
/* 戻 り 値：先頭クラスタ番号                                                 */
/*         ：                                                                 */
/* 概    要：対象のエントリより先頭クラスタ番号を取得する                     */
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

F_DWORD FMR_getTopClstNum(
	TFMR_DIRENTRY * l_entry             /* ディレクトリエントリ               */
)
{
	F_DWORD l_topclstnum;
#ifdef DFMR_ENDIAN_BIG
	T_ENDIAN l_buff;
#endif /* DFMR_ENDIAN_BIG */
	l_buff.wordval = l_entry->fstclushi;
	l_topclstnum = ( F_DWORD )l_buff.byteval[ 1 ];
	l_topclstnum <<= 8;
	l_topclstnum |= ( F_DWORD )l_buff.byteval[ 0 ];
	l_topclstnum <<= 8;
	l_buff.wordval = l_entry->fstcluslo;
	l_topclstnum = ( F_DWORD )l_buff.byteval[ 1 ];
	l_topclstnum <<= 8;
	l_topclstnum |= ( F_DWORD )l_buff.byteval[ 0 ];
#ifdef DFMR_ENDIAN_BIG
	
#else /* DFMR_ENDIAN_BIG */
	l_topclstnum = ( F_DWORD )l_entry->fstclushi;
	l_topclstnum <<= 16;
	l_topclstnum |= ( F_DWORD )l_entry->fstcluslo;

#endif /* DFMR_ENDIAN_BIG */
	return l_topclstnum;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
