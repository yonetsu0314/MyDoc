/*======================================================================
 * File name    : fmemset.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/fmemset.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fmemset.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * Saver60 Ver.01.09 up
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
/* 関 数 名：FMR_memSet                                                       */
/*         ：                                                                 */
/* パラメタ：IN:  F_BYTE 初期化データ                                         */
/*         ：     F_DWORD バッファサイズ                                      */
/*         ：                                                                 */
/*         ：OUT: F_VOID * バッファアドレス                                   */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：void                                                             */
/*         ：                                                                 */
/* 概    要：バッファを初期化する                                             */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.07 初期化データ(pattern)をF_DWORDへキャスト後、    05/12/12 Y.Yoshizumi */
/*       ビットシフト実行するように変更                                       */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_memSet(
    F_VOID * buff,                      /* バッファアドレス                   */
    F_BYTE pattern,                     /* 初期化データ                       */
    F_DWORD size                        /* バッファサイズ                     */
)
{
	F_DWORD l_restsize;                 /* 残りサイズ                         */
	F_DWORD l_work;                     /* ワークエリア                       */
	F_DWORD l_work2;
	F_DWORD l_cnt;                      /* ループカウンタ                     */
	F_DWORD * l_dbuff;                  /* DWORD型ポインタ                    */
	F_BYTE * l_bbuff;                   /* BYTE型ポインタ                     */
	
	
	/* 残りサイズ初期化 */
	l_restsize = size;
	
	l_bbuff = ( F_BYTE * )buff;
	/* 残りサイズがsizeof(F_DWORD)Byte未満の場合 */
	if( l_restsize < sizeof(F_DWORD) )
	{
		/* Byte単位で初期化を行う */
		while( l_restsize > 0 )
		{
			*( l_bbuff ) = pattern;
			l_bbuff ++;
			l_restsize --;
		}
		return ;
	}
	l_work = ( F_DWORD )buff;
	/* バッファアドレスの最初のsizeof(F_DWORD)Byte境界位置まで初期化を行う */
	if( ( l_work & (sizeof(F_DWORD) - 1) ) != 0 )
	{
		l_work &= (sizeof(F_DWORD) - 1);
		l_work = sizeof(F_DWORD) - l_work;
		for( l_cnt = 0; l_cnt < l_work; l_cnt ++ )
		{
			*( l_bbuff ) = pattern;
			l_bbuff ++;
		}
		l_restsize -= l_work;
	}
	/* 残りサイズの内、４で割り切れるサイズ分について */
	/* DWORD単位でコピーを行う */
	l_dbuff = ( F_DWORD * )l_bbuff;
	l_work = l_restsize / sizeof(F_DWORD);
	l_work2 = ( F_DWORD )pattern 
		| ( ( ( F_DWORD )pattern ) << 8 )   /*                              */
		| ( ( ( F_DWORD )pattern ) << 16 )  /*                              */
		| ( ( ( F_DWORD )pattern ) << 24 ); /*                              */

	for( l_cnt = 0; l_cnt < l_work; l_cnt ++ )
	{
		*( l_dbuff ) =  l_work2;
		l_dbuff ++;
	}
	l_restsize -= l_work * sizeof(F_DWORD);
	l_bbuff = ( F_BYTE * )l_dbuff;
	
	/* 残りサイズがある場合 */
	if( l_restsize != 0 )
	{
		/* 残りはバイト単位でコピー */
		for( l_cnt = 0; l_cnt < l_restsize; l_cnt ++ )
		{
			*( l_bbuff ) = pattern;
			l_bbuff ++;
		}
	}

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
