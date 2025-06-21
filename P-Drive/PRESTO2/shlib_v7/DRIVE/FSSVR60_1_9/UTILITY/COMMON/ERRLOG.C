/*======================================================================
 * File name    : errlog.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/errlog.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: errlog.c $
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
/* 関 数 名：FMR_recErrLog                                                    */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD 断片ファイルの先頭クラスタ番号                      */
/*         ：     F_WORD 断片ファイルのクラスタ数                             */
/*         ：     F_CHAR エラー種別                                           */
/*         ：     F_CHAR * 元ファイル名                                       */
/*         ：OUT: F_BYTE * ログ出力領域                                       */
/*         ：     TFMR_SDWORKAREA * スキャンディスク用ワークエリア            */
/*         ：                                                                 */
/* 戻 り 値：なし                                                             */
/*         ：                                                                 */
/* 概    要：ロギングを行う                                                   */
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

F_VOID FMR_recErrLog(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 断片ファイルの先頭クラスタ番号     */
	F_WORD clstcnt,                     /* 断片ファイルのクラスタ数           */
	F_CHAR errcode,                     /* エラー種別                         */
	F_CHAR * orgpath,                   /* 元ファイル名                       */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	TFMR_SDWORKAREA * sdworkarea        /* スキャンディスク用ワークエリア     */
)
{
	TFMR_SDLOGTBLSHT * l_sdlogtblsht;   /* ログ構造体(フルパス非指定)         */
	TFMR_SDLOGTBLLNG * l_sdlogtbllng;   /* ログ構造体(フルパス指定)           */
	F_WORD l_cnt;                       /* ループカウンタ                     */
	
	/* ロギング動作OFFの場合 */
	if( ( ( sdworkarea->mode ) & DFMR_SD_LOGMODEON ) != DFMR_SD_LOGMODEON )
	{
		return;
	}
	
	/* 元ファイル名表示方法による分岐 */
	if( ( ( sdworkarea->mode ) & DFMR_SD_LOGFULLPATH )
		!= DFMR_SD_LOGFULLPATH )        /* ファイル名のみ表示の場合           */
	{
		/* ログ構造体キャスト */
		l_sdlogtblsht = ( TFMR_SDLOGTBLSHT * )logarea;
		l_sdlogtblsht += sdworkarea->logcnt;
		
		/* ログ設定 */
		l_sdlogtblsht->errcode = errcode;   /* エラーコード                   */
		l_sdlogtblsht->orgtopclstnum = topclstnum;
		                                    /* 元ファイルの先頭クラスタ番号   */
		l_sdlogtblsht->orgclstnum = clstcnt;/* 破損ファイルのクラスタ数       */
		FMR_memSet(                         /* 元ファイル名格納領域初期化     */
			l_sdlogtblsht->orgname,
			DFMR_INITBYTE,
			( DFMR_FNAMESZ + DFMR_EXTSZ + 1 )
			);
		orgpath ++;                     /* 先頭にデリミタがあるので跳ばす     */
		for( l_cnt = 0; l_cnt < ( DFMR_FNAMESZ + DFMR_EXTSZ + 1 ); l_cnt ++ )
		{                                   /* 元ファイル名設定               */
			if( *( orgpath + l_cnt ) == NULL )
			{
				break;
			}
			l_sdlogtblsht->orgname[ l_cnt ] = *( orgpath + l_cnt );
		}
	}
	else                                /* フルパス指定の場合                 */
	{
		/* ログ構造体キャスト */
		l_sdlogtbllng = ( TFMR_SDLOGTBLLNG * )logarea;
		l_sdlogtbllng += sdworkarea->logcnt;
		
		/* ログ設定 */
		l_sdlogtbllng->errcode = errcode;   /* エラーコード                   */
		l_sdlogtbllng->orgtopclstnum = topclstnum;
		                                    /* 元ファイルの先頭クラスタ番号   */
		l_sdlogtbllng->orgclstnum = clstcnt;/* 破損ファイルのクラスタ数       */
		FMR_memSet(                         /* 元ファイル名格納領域初期化     */
			l_sdlogtbllng->orgname,
			DFMR_INITBYTE,
			DFMR_MAXFILESZ
			);
		for( l_cnt = 0; l_cnt < DFMR_MAXFILESZ; l_cnt ++ )
		{                                   /* 元ファイル名設定               */
			if( *( orgpath + l_cnt ) == NULL )
			{
				break;
			}
			l_sdlogtbllng->orgname[ l_cnt ] = *( orgpath + l_cnt );
		}
	}
	
	/* ログカウンタ更新 */
	sdworkarea->logcnt ++;
	
	if( sdworkarea->logcnt >= sdworkarea->logmax )
	{
		sdworkarea->logcnt = DFMR_SD_LOGCNTINIT;
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
