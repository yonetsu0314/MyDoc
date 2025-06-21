/*======================================================================
 * File name    : stpclstn.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/stpclstn.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: stpclstn.c $
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
/* 関 数 名：FMR_setTopClstNum                                                */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * ディレクトリエントリ                        */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：                                                                 */
/* 戻 り 値：なし                                                             */
/*         ：                                                                 */
/* 概    要：対象のエントリの先頭クラスタ番号を設定する                       */
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

F_VOID FMR_setTopClstNum(
	TFMR_DIRENTRY * l_entry,            /* ディレクトリエントリ               */
	F_DWORD topclstnum                  /* 先頭クラスタ番号                   */
)
{
#ifdef DFMR_ENDIAN_BIG
	T_ENDIAN l_buff;
#endif /* DFMR_ENDIAN_BIG */
#ifdef DFMR_ENDIAN_BIG
	l_buff.dwordval = topclstnum;
	l_entry->fstclushi = ( F_WORD )l_buff.byteval[ 1 ];
	l_entry->fstclushi <<= 8;
	l_entry->fstclushi |= ( F_WORD )l_buff.byteval[ 0 ];
	
	l_entry->fstcluslo = ( F_WORD )l_buff.byteval[ 3 ];
	l_entry->fstcluslo <<= 8;
	l_entry->fstcluslo |= ( F_WORD )l_buff.byteval[ 2 ];
	
#else /* DFMR_ENDIAN_BIG */
	l_entry->fstclushi = ( F_WORD )( ( topclstnum & 0xFFFF0000 ) >> 16 );
	l_entry->fstcluslo = ( F_WORD )( topclstnum & 0x0000FFFF );

#endif /* DFMR_ENDIAN_BIG */
	return;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
