/*======================================================================
 * File name    : findnext.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/RCS/findnext.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: findnext.c $
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
/* 関 数 名：FMR_FindNext                                                     */
/*         ：                                                                 */
/* パラメタ：IN:  long 検索ハンドル                                           */
/*         ：                                                                 */
/*         ：OUT: TFMR_FILE_INFO * ファイル状態格納領域                       */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_EBADF                                             */
/*         ：          DFMR_ERRPARAM                                          */
/*         ：                                                                 */
/* 概    要：ディレクトリ/ファイルの検索                                      */
/*         ：                                                                 */
/* 作 成 日：2004.05.31                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 取得ロングファイル名領域初期化追加              04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_FindNext ( 
	long srchhandle,                    /* 検索ハンドル                       */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;

	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ロングファイル名               */
	
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_prsflg;                    /* 検索処理動作フラグ                 */
	
	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	/* 検索ハンドルの確認 */
	if( ( srchhandle < 0 ) || ( DFMR_USR_MAXSEARCHHANDLE <= srchhandle ) )
	{
		/* 検索ハンドルが範囲外の場合 */
		return DFMR_EBADF;
	}
	if( tfmr_searchhandle[ srchhandle ].sectornum == 0xFFFFFFFF )
	{
		/* 検索ハンドルに検索情報が関連付けされていない場合 */
		return DFMR_EBADF;
	}
	/* ファイル状態格納領域のチェック */
	if( fileinfo == NULL )              /* ファイル状態格納領域がNULLの場合   */
	{
		return DFMR_ERRPARAM;           /* パラメータエラーを返却             */
	}
	/**************************************************************************/
	/* 初期設定                                                               */
	/**************************************************************************/
	/* 論理ドライブ番号取得 */
	l_drvnum = tfmr_searchhandle[ srchhandle ].drivenum;

	if( MFMR_DRVINFO( l_drvnum ) == ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		return DFMR_DRVNOCONNECTERR;    /* 論理ドライブ未接続エラー           */
	}
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn = FMR_osWaiSem(
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	/**************************************************************************/
	/* 検索ハンドルを1エントリ分進める                                        */
	/**************************************************************************/
	if( tfmr_searchhandle[ srchhandle ].sectornum >= MFMR_DATATOP( l_drvnum ) )
	{
		/* 検索開始位置がData Regionの場合 */
		/* FAT読込み領域0に検索開始位置セクタを含むFATセクタを読込む */
		l_work01 = tfmr_searchhandle[ srchhandle ].sectornum;
		l_work01 -= MFMR_DATATOP( l_drvnum );
		l_work01 /= MFMR_SECTPERCLST( l_drvnum );
		l_work01 += DFMR_TOPCLSTNUM;
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_work01,                   /* クラスタ番号                       */
			&l_work02                   /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
	}
	l_toppos.sectnum = tfmr_searchhandle[ srchhandle ].sectornum;
	l_toppos.slot = tfmr_searchhandle[ srchhandle ].position;
	l_rtn = FMR_skipSearchPosition(
		l_drvnum,                       /* 論理ドライブ番号                   */
		DFMR_REFFATAREA0,               /* FAT読込み領域番号                  */
		&l_toppos                        /* 検索セクタ位置                     */
		);
	if( l_rtn != DFMR_OK )
	{
		if( l_rtn == DFMR_NOENTRY )     /* 前回の検索位置が検索ディレクトリの */
		{                               /* 終端だった場合                     */
			l_rtn = DFMR_ENOENT;
		}
		goto ERROR;
	}
	/**************************************************************************/
	/* 対象エントリ検索                                                       */
	/**************************************************************************/
	/* 検索フラグ設定 */
	l_prsflg = DFMR_SRCHFLGWC;          /* ワイルドカード指定ON               */
	l_prsflg |= DFMR_SRCHFLGLFN;        /* ロングファイル名指定ON             */
	l_getname[ 0 ] = NULL;
	
	/* 対象エントリを検索する */
	for( l_work01 = 0; l_work01 < DFMR_MAXFILESZ ; l_work01 ++ )
	{
		if( tfmr_searchhandle[ srchhandle ].keyword[ l_work01 ] == NULL )
		{
			break;
		}
	}
	l_rtn = FMR_searchEntry(
		l_drvnum,                       /* 対象ドライブ番号                   */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_bottompos,                   /* 検索終了最終位置                   */
		&tfmr_searchhandle[ srchhandle ].keyword[ 0 ],
		                                /* 検索文字列                         */
		&l_entry,                       /* 取得エントリ                       */
		&l_getname[ 0 ],                /* 取得ロングファイル名               */
		( F_BYTE)l_work01,              /* 検索文字列長                       */
		l_prsflg                        /* 動作フラグ                         */
		);
	if( l_rtn != DFMR_OK )
	{
		if( l_rtn == DFMR_NOENTRY )
		{
			/* 対象ディレクトリが存在しない場合、戻り値をDFMR_ENOENTとする    */
			l_rtn = DFMR_ENOENT;
		}
		goto ERROR;
	}
	/**************************************************************************/
	/* ファイル状態の設定                                                     */
	/**************************************************************************/
	FMR_setFileInfo(
		&l_entry,
		&l_getname[ 0 ],
		fileinfo
		);
	/**************************************************************************/
	/* 検索ハンドルの設定を行う                                               */
	/**************************************************************************/
	if( l_bottompos.sectnum == 0 )
	{
		/**********************************************************************/
		/* 対象エントリのショートファイル名が検索文字列と一致した場合         */
		/**********************************************************************/
		tfmr_searchhandle[ srchhandle ].sectornum = l_toppos.sectnum;
		tfmr_searchhandle[ srchhandle ].position = l_toppos.slot;
	}
	else
	{
		/**********************************************************************/
		/* 対象エントリのロングファイル名が検索文字列と一致した場合           */
		/**********************************************************************/
		tfmr_searchhandle[ srchhandle ].sectornum = l_bottompos.sectnum;
		tfmr_searchhandle[ srchhandle ].position = l_bottompos.slot;
		
	}
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	/**************************************************************************/
	/* 正常終了返却                                                           */
	/**************************************************************************/
	return DFMR_SUC;

ERROR:
	/* 異常終了 */
	l_ortn = FMR_osSigSem(              /* セマフォ資源返却                   */
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	return l_rtn;

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
