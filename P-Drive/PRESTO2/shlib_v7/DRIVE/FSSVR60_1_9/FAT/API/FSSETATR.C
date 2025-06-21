/*======================================================================
 * File name    : fssetatr.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fssetatr.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fssetatr.c $
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
/* 関 数 名：FMR_SetAtr                                                       */
/*         ：                                                                 */
/* パラメタ：IN:  const char * パス名                                         */
/*         ：     unsigned char 属性                                          */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_EBADF                                             */
/*         ：          DFMR_ENOTEMPTY                                         */
/*         ：                                                                 */
/* 概    要：ファイル属性を設定する                                           */
/*         ：                                                                 */
/* 作 成 日：2004.05.31                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/* 01.08 LFN検索にてSFNエントリヒット時、SFNとして処理   06/01/16 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_SetAtr ( 
	const char * path,                  /* パス名                             */
	unsigned char attr                  /* 属性                               */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cdirclsnum;               /* カレントディレクトリの             */
	                                    /* クラスタ番号                       */
	F_DWORD l_targetsect;               /* 対象エントリ登録セクタ番号         */
    F_CHAR * l_pathlen;                 /* パス名先頭からの最終要素の位置     */

	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */
	TFMR_DIRENTRY * l_entptr;           /* エントリ格納用ポインタ             */
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_WORD l_cnt;                       /* 汎用カウンタ                       */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ファイル名                     */

	F_BYTE l_targetpos;                 /* 対象エントリ登録位置               */
	F_BYTE l_targetbuff;                /* 対象セクタ読込みバッファ番号       */
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
	if( ( attr & ~( DFMR_ATTR_RONLY | DFMR_ATTR_HIDE | DFMR_ATTR_SYS | DFMR_ATTR_ARC ) ) != 0x00 )
	{
		return DFMR_ERRPARAM;           /* パラメータエラー返却               */
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
	/* カレントディレクトリ検索処理 */
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
	/* 対象エントリ検索                                                       */
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
			/* 対象エントリが存在しない場合、戻り値をDFMR_ENOENTとする    */
			l_rtn = DFMR_ENOENT;
		}
		goto ERROR;
	}
	if( l_entry.attr == DFMR_ATTR_DIR )
	{
		/**********************************************************************/
		/* 取得した対象エントリがディレクトリの場合                           */
		/**********************************************************************/
		attr |= DFMR_ATTR_DIR;          /* 属性にディレクトリを含める         */
	}
	/**************************************************************************/
	/* 対象エントリ登録位置を取得                         */
	/**************************************************************************/
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		/* 対象エントリがロングファイル名指定の場合 */
		if( l_bottompos.sectnum == 0 )
		{
			/* ショートファイル名エントリと一致した場合 */
			/* ショートファイル名指定とする */
			l_targetsect = l_toppos.sectnum;    /* エントリ登録セクタ番号         */
			l_targetpos = l_toppos.slot;        /* エントリ登録位置               */
			l_targetbuff = l_toppos.bufnum;
			l_prsflg &= ~DFMR_SRCHFLGLFN;
		}
		else
		{
			l_targetsect = l_bottompos.sectnum; /* エントリ登録セクタ番号         */
			l_targetpos = l_bottompos.slot;     /* エントリ登録位置               */
			l_targetbuff = l_bottompos.bufnum;
		}
	}
	else
	{
		/* 対象エントリがショートファイル名指定の場合 */
		l_targetsect = l_toppos.sectnum;    /* エントリ登録セクタ番号         */
		l_targetpos = l_toppos.slot;        /* エントリ登録位置               */
		l_targetbuff = l_toppos.bufnum;
	}
	/**************************************************************************/
	/* ファイルハンドルの確認を行う                                           */
	/**************************************************************************/
	for( l_cnt = 0; l_cnt < DFMR_USR_MAXFILEHANDLE; l_cnt ++ )
	{
		if( ( l_targetsect ==  tfmr_filehandle[ l_cnt ].entdirsectnum )
			&& ( l_targetpos ==  tfmr_filehandle[ l_cnt ].entdirsectpos )
			&& ( l_drvnum ==  tfmr_filehandle[ l_cnt ].drivenum ) )
		{
			/* ディレクトリエントリのセクタ/セクタ内位置が                */
			/* ファイルハンドルの同パラメータと等しい場合は               */
			/* 対象ファイルがオープン中と見なす                           */
			l_rtn = DFMR_MULTIOPEN;
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* 属性の設定                                                             */
	/**************************************************************************/
	/* 取得したエントリの属性を変更 */
	l_entry.attr = attr;
	
	/* R/Wバッファを更新 */
	l_entptr = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( l_drvnum, l_targetbuff ) );
	*( l_entptr + l_targetpos ) = l_entry ;

	/* R/Wバッファを対象セクタに書込む */
	SetBackupFlg(l_drvnum);
	l_rtn = FMR_setDataSector(
		l_drvnum,
		l_targetsect,
		l_targetbuff
		);
	ClearBackupFlg(l_drvnum);
	Clear_PowerFailureFlg(l_drvnum);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
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
