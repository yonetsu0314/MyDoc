/*======================================================================
 * File name    : fsremove.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fsremove.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsremove.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * Saver60 Ver.01.09 up
 * 
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
/* 関 数 名：FMR_Remove                                                       */
/*         ：                                                                 */
/* パラメタ：IN:  const char * 削除するファイルのパス名                       */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_ENOENT                                            */
/*         ：          DFMR_ENOTEMPTY                                         */
/*         ：                                                                 */
/* 概    要：ファイルの削除                                                   */
/*         ：                                                                 */
/* 作 成 日：2004.05.27                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/* 01.08 LFN検索にてSFNエントリヒット時、SFNとして処理   06/01/16 Y.Yoshizumi */
/* 01.09 検索ハンドル確認処理削除                        06/03/01 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_Remove ( 
	const char * path                   /* 削除するファイルのパス名           */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cdirclsnum;               /* カレントディレクトリの             */
	                                    /* クラスタ番号                       */
	F_DWORD l_targetsect;               /* 対象エントリ登録セクタ番号         */
    F_CHAR * l_pathlen;                 /* パス名先頭からの最終要素の位置     */

	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */
	T_ENDIAN l_endian;                  /* エンディアン調整用ワーク           */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ファイル名                     */

	F_BYTE l_targetpos;                 /* 対象エントリ登録位置               */
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_prsflg;                    /* 検索処理動作フラグ                 */
	F_BYTE l_namelen;                   /* 対象ファイル名長                   */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	l_rtn = FMR_chkPathName(
		( F_CHAR * )path,               /* パス名                             */
		&l_drvnum                       /* 論理ドライブ番号                   */
		);
	if( l_rtn != DFMR_OK )              /* エラー終了の場合                   */
	{
		return l_rtn;
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
	/* カレントディレクトリ検索                                               */
	/**************************************************************************/
	l_prsflg = 0;
	path += 3;                          /* 先頭の｢A:\｣を飛ばす                */
	l_rtn = FMR_getCurrentDirCluster(
		l_drvnum,                       /* 論理ドライブ番号                   */
		( F_CHAR * )path,               /* パス名                             */
		&l_getname[ 0 ],                /* ロングファイル名用ワークエリア     */
		&l_prsflg,                      /* FMR_searchEntry用動作フラグ        */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_pathlen,                     /* パス名先頭からの最終要素の位置     */
		&l_cdirclsnum                   /* カレントクラスタ番号               */
		);
	if( l_rtn < DFMR_OK )               /* エラー終了の場合                   */
	{
		goto ERROR;
	}
	l_namelen = ( F_BYTE )l_rtn;
	/**************************************************************************/
	/* 対象ファイル検索                                                       */
	/**************************************************************************/
	l_rtn = FMR_searchEntry(
		l_drvnum,                       /* 対象ドライブ番号                   */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_bottompos,                   /* 検索終了最終位置                   */
		l_pathlen,                      /* 検索文字列                         */
		&l_entry,                       /* 取得エントリ                       */
		&l_getname[ 0 ],                /* 取得ロングファイル名               */
		l_namelen,                      /* 検索文字列長                       */
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
	if( l_entry.attr == DFMR_ATTR_DIR )
	{
		l_rtn = DFMR_ENOENT;
		goto ERROR;
	}
	/**************************************************************************/
	/* 対象ファイル登録位置を取得                                             */
	/**************************************************************************/
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		/* 対象ファイルがロングファイル名指定の場合 */
		if( l_bottompos.sectnum == 0 )
		{
			/* ショートファイル名エントリと一致した場合 */
			/* ショートファイル名指定とする */
			l_targetsect = l_toppos.sectnum;    /* エントリ登録セクタ番号         */
			l_targetpos = l_toppos.slot;        /* エントリ登録位置               */
			l_prsflg &= ~DFMR_SRCHFLGLFN;
		}
		else
		{
			l_targetsect = l_bottompos.sectnum; /* エントリ登録セクタ番号         */
			l_targetpos = l_bottompos.slot;     /* エントリ登録位置               */
		}
	}
	else
	{
		/* 対象ファイルがショートファイル名指定の場合 */
		l_targetsect = l_toppos.sectnum;    /* エントリ登録セクタ番号         */
		l_targetpos = l_toppos.slot;        /* エントリ登録位置               */
	}
	/**************************************************************************/
	/* ファイルハンドルの確認を行う                                           */
	/**************************************************************************/
	for( l_work01 = 0; l_work01 < DFMR_USR_MAXFILEHANDLE; l_work01 ++ )
	{
		if( ( l_targetsect ==  tfmr_filehandle[ l_work01 ].entdirsectnum )
			&& ( l_targetpos ==  tfmr_filehandle[ l_work01 ].entdirsectpos )
			&& ( l_drvnum ==  tfmr_filehandle[ l_work01 ].drivenum ) )
		{
			/* ディレクトリエントリのセクタ/セクタ内位置が                */
			/* ファイルハンドルの同パラメータと等しい場合は               */
			/* 対象ファイルがオープン中と見なす                           */
			l_rtn = DFMR_MULTIOPEN;
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* 対象ファイルのエントリを削除する                                       */
	/**************************************************************************/
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		/**********************************************************************/
		/* 対象ディレクトリがロングファイル名の場合                           */
		/**********************************************************************/
		/* ロングファイル名エントリ削除処理 */
		l_rtn = FMR_deleteLFNEntry(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			&l_toppos,                  /* 先頭位置                           */
			&l_bottompos                /* 最終位置                           */
			);
	}
	else
	{
		/**********************************************************************/
		/* 対象ディレクトリがショートファイル名の場合                         */
		/**********************************************************************/
		/* R/Wバッファ上の対象エントリを削除済みとする */
		MFMR_RWBUFF( l_drvnum, 0, l_targetpos * sizeof( TFMR_DIRENTRY ) ) = DFMR_DELENTRY;
		
		/* R/Wバッファを対象セクタに書込む */
		SetBackupFlg(l_drvnum);
		l_rtn = FMR_setDataSector(
			l_drvnum,
			l_targetsect,
			0
			);
		ClearBackupFlg(l_drvnum);
	}
	if( l_rtn != DFMR_OK )              /* エラー終了の場合                   */
	{
		goto ERROR;
	}
	/**************************************************************************/
	/* 対象ファイルのデータ部を削除する                                       */
	/**************************************************************************/
	l_endian.wordval = l_entry.fstclushi;       /* 取得したエントリより       */
	l_work01 = (F_DWORD)(l_endian.byteval[1]);  /* 開始クラスタ番号を取得     */
	l_work01 = (l_work01 << 8) | (F_DWORD)(l_endian.byteval[0]);
	l_endian.wordval = l_entry.fstcluslo;
	l_work01 = (l_work01 << 8) | (F_DWORD)(l_endian.byteval[1]);
	l_work01 = (l_work01 << 8) | (F_DWORD)(l_endian.byteval[0]);
	if( l_work01 != DFMR_INITBYTE )     /* Writeが行われたファイルの場合      */
	{
		l_rtn = FMR_deleteDataCluster(
			l_drvnum,                       /* 対象論理ドライブ番号               */
			l_work01,                       /* 先頭クラスタ番号                   */
			NULL
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
	}
	Clear_PowerFailureFlg(l_drvnum);
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
	Clear_PowerFailureFlg(l_drvnum);
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
