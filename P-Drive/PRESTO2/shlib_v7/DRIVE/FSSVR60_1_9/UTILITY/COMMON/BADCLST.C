/*======================================================================
 * File name    : badclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/badclst.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: badclst.c $
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
/* 関 数 名：FMR_chkBadClst                                                   */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD クラスタ番号                                        */
/*         ：     F_BYTE R/Wバッファ番号                                      */
/*         ：     F_BYTE 対象クラスタ判別フラグ                               */
/*         ：                OFF:未使用クラスタ                               */
/*         ：                ON :使用中のクラスタ                             */
/*         ：                                                                 */
/* 戻 り 値：DFMR_OK 正常クラスタ                                             */
/*         ：DFMR_SD_CHKBADCLST BADクラスタ                                   */
/*         ：DFMR_SD_CHKTBDCLST 未使用クラスタ                                */
/*         ：                                                                 */
/* 概    要：クラスタ内のセクタについて読込み確認を行い、正常/BADクラスタの   */
/*         ：判別を行う                                                       */
/*         ：対象クラスタが未使用クラスタの場合、読込み確認失敗時に           */
/*         ：書込み確認を行い復旧を試みる                                     */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_INT FMR_chkBadClst(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_BYTE buffnum,                     /* R/Wバッファ番号                    */
	F_BYTE dataflg                      /* 対象クラスタ判別フラグ             */
	                                    /* OFF:未使用クラスタ                 */
	                                    /* ON :使用中のクラスタ               */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_sectcnt;                  /* セクタ番号                         */
	F_BYTE l_cnt;                       /* ループカウンタ                     */
	
	/* クラスタの最終セクタ番号を求める */
	l_sectcnt = FMR_getSectNum(
		clstnum,                        /* クラスタ番号                       */
		( MFMR_SECTPERCLST( drvnum ) - 1 ),     /* クラスタ内セクタ番号    */
		drvnum                          /* ドライブ番号                       */
		);
	
	/* 対象クラスタ内のセクタを終端から順次読込む */
	for( l_cnt = 0; l_cnt < MFMR_SECTPERCLST( drvnum ); l_cnt ++ )
	{
		/* 対象セクタを読込む */
		l_rtn = FMR_readSectRetry(
			drvnum,                     /* ドライブ番号                       */
			( l_sectcnt - l_cnt ),      /* セクタ番号                         */
			buffnum                     /* R/Wバッファ番号                    */
			);
		if( l_rtn != DFMR_OK )          /* 読込めないセクタが存在した場合     */
		{
			if( dataflg != DFMR_FLGOFF )/* 対象クラスタが使用中の             */
			{                           /* クラスタの場合                     */
				return DFMR_SD_CHKBADCLST;      /* バッドクラスタ             */
			}
			break;
		}
	}
	if( l_cnt == MFMR_SECTPERCLST( drvnum ) )
	{                                   /* 全て正常に読込めた場合は正常終了   */
		return DFMR_OK;
	}
	
	/* 対象のR/Wバッファを0で埋める */
	FMR_memSet(
		MFMR_RWBUFFTOP( drvnum, buffnum ),
		DFMR_INITBYTE,
		MFMR_BYTEPERSECT( drvnum )
		);
	
	/* 対象クラスタ内セクタを終端から順次書込む */
	for( l_cnt = 0; l_cnt < MFMR_SECTPERCLST( drvnum ); l_cnt ++ )
	{
		/* 対象セクタを読込む */
		l_rtn = FMR_writeSectRetry(
			drvnum,                     /* ドライブ番号                       */
			( l_sectcnt - l_cnt ),      /* セクタ番号                         */
			buffnum                     /* R/Wバッファ番号                    */
			);
		if( l_rtn != DFMR_OK )          /* 書込めないセクタが存在した場合     */
		{
			return DFMR_SD_CHKBADCLST;  /* バッドクラスタ                     */
		}
	}
	/* 全て正常に書込めた場合 */
	return DFMR_SD_CHKTBDCLST;          /* 未使用                             */
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
