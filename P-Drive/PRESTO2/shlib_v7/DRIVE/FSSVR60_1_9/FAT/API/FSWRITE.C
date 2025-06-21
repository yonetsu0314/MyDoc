/*======================================================================
 * File name    : fswrite.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fswrite.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fswrite.c $
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
/* 関 数 名：FMR_Write                                                        */
/*         ：                                                                 */
/* パラメタ：IN:  int          ファイルハンドル                               */
/*         ：     void *       書込みデータ格納領域                           */
/*         ：     unsigned int 書き込みサイズ                                 */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  書込みサイズ                                           */
/*         ：異常終了  DFMR_EBADF                                             */
/*         ：          DFMR_ERRPARAM                                          */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ファイルへのデータ書込み                                         */
/*         ：                                                                 */
/* 作 成 日：2004.05.28                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

int FMR_Write ( 
	int          filehandle,            /* ファイルハンドル                   */
	void *       writebuff,             /* 書込みデータ格納領域               */
	unsigned int size                   /* 書込みサイズ                       */
)
{
	F_DWORD l_restsize;                 /* 書込み済みサイズ                   */
	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_error;                      /* エラー戻り値                       */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	/* ファイルハンドルの確認 */
	if( ( filehandle < 0 ) || ( DFMR_USR_MAXFILEHANDLE <= filehandle ) )
	{
		/* ファイルハンドルが範囲外の場合 */
		l_error =  DFMR_EBADF;
		goto ERROR;
	}
	if( tfmr_filehandle[ filehandle ].direntry.name[ 0 ] == DFMR_INITBYTE )
	{
		/* ファイルハンドルにファイルが関連付けされていない場合 */
		l_error = DFMR_EBADF;
		goto ERROR;
	}
	if( writebuff == NULL )
	{
		/* 書込みデータ格納領域がNULLの場合 */
		l_error = DFMR_ERRPARAM;
		goto ERROR;
	}
	if( MFMR_DRVINFO( tfmr_filehandle[ filehandle ].drivenum )
		== ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		l_error = DFMR_DRVNOCONNECTERR;
		goto ERROR;
	}
	/* 対象ファイルのアクセスモ-ドの確認 */
	if( ( ( tfmr_filehandle[ filehandle ].acsmode & DFMR_O_WRONLY ) != DFMR_O_WRONLY )
		&& ( tfmr_filehandle[ filehandle ].acsmode & DFMR_O_RDWR ) != DFMR_O_RDWR )
	{
		/* Read Onlyの場合 */
		l_error = DFMR_EBADF;
		goto ERROR;
	}
	if( size == 0 )
	{
		/* 書込みサイズが0の場合 */
		l_error = DFMR_ERRPARAM;
		goto ERROR;
	}
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn =  FMR_osWaiSem(
		MFMR_SEMID( tfmr_filehandle[ filehandle ].drivenum )
		);
	if( l_ortn != DFMR_OK )
	{
		l_error = l_ortn;
		goto ERROR;
	}
	/**************************************************************************/
	/* 書込み処理                                                             */
	/**************************************************************************/
	l_restsize = FMR_subWrite(
		filehandle,                     /* ファイルハンドル                   */
		writebuff,                      /* 書込みデータ格納領域               */
		size                            /* 書込みサイズ                       */
		);
	Clear_PowerFailureFlg(tfmr_filehandle[ filehandle ].drivenum);
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		MFMR_SEMID( tfmr_filehandle[ filehandle ].drivenum )
		);
	if( l_ortn != DFMR_OK )
	{
		l_error = l_ortn;
		goto ERROR;
	}
	
	/**************************************************************************/
	/* 書込み済みサイズを返却                                                 */
	/**************************************************************************/
	return l_restsize;

ERROR:
	/* 異常終了 */
	Clear_PowerFailureFlg(tfmr_filehandle[ filehandle ].drivenum);
	return l_error;

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
