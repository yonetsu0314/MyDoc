/*======================================================================
 * File name    : fmemcopy.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/fmemcopy.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fmemcopy.c $
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
/* 関 数 名：FMR_memCopy                                                      */
/*         ：                                                                 */
/* パラメタ：IN:  F_VOID * コピー元バッファアドレス                           */
/*         ：     F_DWORD コピーサイズ                                        */
/*         ：                                                                 */
/*         ：OUT: F_VOID * コピー先バッファアドレス                           */
/*         ：                                                                 */
/* 戻 り 値：void                                                             */
/*         ：                                                                 */
/* 概    要：バッファをコピーする                                             */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_memCopy(
    F_VOID * deststr,                   /* コピー先バッファアドレス           */
    F_VOID * srcstr,                    /* コピー元バッファアドレス           */
    F_DWORD size                        /* コピーサイズ                       */
)
{
	F_DWORD l_restsize;                 /* 残りサイズ                         */
	F_DWORD l_work;                     /* ワークエリア                       */
	F_DWORD l_cnt;                      /* ループカウンタ                     */
	F_DWORD * l_dsrc;                   /* DWORD型ポインタ                    */
	F_DWORD * l_ddest;
	F_WORD * l_wsrc;                    /* WORD型ポインタ                     */
	F_WORD * l_wdest;
	F_BYTE * l_bsrc;                    /* BYTE型ポインタ                     */
	F_BYTE * l_bdest;
	F_DWORD l_srccal;                   /* 条件判定用                         */
	F_DWORD l_destcal;
	
	
	/* 残りサイズ初期化 */
	l_restsize = size;
	
	/* 残りサイズがsizeof(F_DWORD)Byte未満の場合 */
	if( l_restsize < sizeof(F_DWORD) )
	{
		l_bsrc = ( F_BYTE * )srcstr;
		l_bdest = ( F_BYTE * )deststr;
		while( l_restsize > 0 )
		{
			*( l_bdest ) = *( l_bsrc );
			l_bdest ++;
			l_bsrc ++;
			l_restsize --;
		}
		return ;
	}
	
	/* コピー先/元バッファのsizeof(F_DWORD)Byte単位のアドレスが等しい */
	l_srccal = ( F_DWORD )srcstr;
	l_destcal = ( F_DWORD )deststr;
	l_bsrc = ( F_BYTE * )srcstr;
	l_bdest = ( F_BYTE * )deststr;
	if( ( l_destcal & (sizeof(F_DWORD) - 1) ) == ( l_srccal & (sizeof(F_DWORD) - 1) ) )
	{
		/* コピー先バッファアドレスのsizeof(F_DWORD)Byte単位のオフセットが0以外 */
		if( ( l_destcal & (sizeof(F_DWORD) - 1) ) != 0 )
		{
			l_work = sizeof(F_DWORD) - l_destcal & (sizeof(F_DWORD) - 1);
			for( l_cnt = 0; l_cnt < l_work; l_cnt ++ )
			{
				*( l_bdest ) = *( l_bsrc );
				l_bdest ++;
				l_bsrc ++;
			}
			l_restsize -= l_work;
		}
		/* 残りサイズの内、４で割り切れるサイズ分について */
		/* DWORD単位でコピーを行う */
		l_dsrc = ( F_DWORD * )l_bsrc;
		l_ddest = ( F_DWORD * )l_bdest;
		l_work = l_restsize / sizeof(F_DWORD);
		for( l_cnt = 0; l_cnt < l_work; l_cnt ++ )
		{
			*( l_ddest ) =  *( l_dsrc );
			l_dsrc ++;
			l_ddest ++;
		}
		l_restsize -= l_work * sizeof(F_DWORD);
		l_bsrc = ( F_BYTE * )l_dsrc;
		l_bdest = ( F_BYTE * )l_ddest;
	}
	/* コピー先/元バッファのsizeof(F_DWORD)Byte単位のアドレスがsizeof(F_WORD)違い */
	else if( ( l_destcal & (sizeof(F_WORD) - 1) ) == ( l_srccal & (sizeof(F_WORD) - 1) ) )
	{
		/* コピー先バッファアドレスのsizeof(F_WORD)Byte単位のオフセットが0以外 */
		if( ( l_destcal & (sizeof(F_WORD) - 1) ) != 0 )
		{
			*( l_bdest ) = *( l_bsrc );
			l_bdest ++;
			l_bsrc ++;
			l_restsize --;
		}
		/* 残りサイズの内、２で割り切れるサイズ分について */
		/* WORD単位でコピーを行う */
		l_wsrc = ( F_WORD * )l_bsrc;
		l_wdest = ( F_WORD * )l_bdest;
		l_work = l_restsize / sizeof(F_WORD);
		for( l_cnt = 0; l_cnt < l_work; l_cnt ++ )
		{
			*( l_wdest ) =  *( l_wsrc );
			l_wsrc ++;
			l_wdest ++;
		}
		l_restsize -= l_work * sizeof(F_WORD);
		l_bsrc = ( F_BYTE * )l_wsrc;
		l_bdest = ( F_BYTE * )l_wdest;
	}
	else
	{
	}
	
	/* 残りサイズがある場合 */
	if( l_restsize != 0 )
	{
		/* 残りはバイト単位でコピー */
		for( l_cnt = 0; l_cnt < l_restsize; l_cnt ++ )
		{
			*( l_bdest ) = *( l_bsrc );
			l_bdest ++;
			l_bsrc ++;
		}
	}

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
