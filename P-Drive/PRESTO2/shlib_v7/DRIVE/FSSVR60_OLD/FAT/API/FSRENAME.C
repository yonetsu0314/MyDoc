/*======================================================================
 * File name    : fsrename.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/RCS/fsrename.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsrename.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
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
/* 関 数 名：FMR_Rename                                                       */
/*         ：                                                                 */
/* パラメタ：IN:  const char * 旧パス名                                       */
/*         ：     const char * 新ﾊﾟｽ名                                        */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_ENOTEMPTY                                         */
/*         ：          DFMR_NMERR                                             */
/*         ：          DFMR_ENOENT                                            */
/*         ：          DFMR_EACCES                                            */
/*         ：          DFMR_MEMFULL                                           */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ファイル/ディレクトリ名称変更                                    */
/*         ：                                                                 */
/* 作 成 日：2004.05.31                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 ファイル属性にアーカイブ属性設定                05/02/09 Y.Yoshizumi */
/* 01.05 FMR_splitPath戻り値の型変更に対応               05/02/28 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_Rename ( 
	const char * oldpath,               /* 旧パス名                           */
	const char * newpath                /* 新パス名                           */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cdirclsnum;               /* カレントディレクトリの             */
	                                    /* クラスタ番号                       */
	F_DWORD l_targetsect;               /* 対象エントリ登録セクタ番号         */
	F_DWORD l_targetlfnsect;            /* 対象最終LFNエントリ登録セクタ番号  */
    F_CHAR * l_pathlen;                 /* パス名先頭からの最終要素の位置     */

	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_WORD l_work04;                    /* 新ファイル名長,FMR_setDirEntry用   */
	F_WORD l_cnt;                       /* 汎用カウンタ                       */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ファイル名                     */

	F_BYTE l_targetpos;                 /* 対象エントリ登録位置               */
	F_BYTE l_targetlfnpos;              /* 対象最終LFNエントリ登録位置        */
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_work03;                    /* LFN用ショートファイル名サム値      */
	F_BYTE l_prsflg;                    /* 検索処理動作フラグ(旧パス用)       */
	F_BYTE l_prsflg2;                   /* 検索処理動作フラグ(新パス用)       */
	F_BYTE l_namelen;                   /* 対象ファイル名長                   */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	/* 旧パス名チェック */
	l_rtn = FMR_chkPathName(
		( F_CHAR * )oldpath,            /* 旧パス名                           */
		&l_drvnum                       /* 論理ドライブ番号                   */
		);
	if( l_rtn != DFMR_OK )              /* エラー終了の場合                   */
	{
		return l_rtn;
	}
	/* 新パス名チェック */
	l_rtn = FMR_chkPathName(
		( F_CHAR * )newpath,            /* 新パス名                           */
		( F_CHAR * )&l_work03           /* 論理ドライブ番号                   */
		);
	if( l_rtn != DFMR_OK )              /* エラー終了の場合                   */
	{
		return l_rtn;
	}
	/* 論理ドライブ番号一致チェック */
	if ( l_drvnum != l_work03 )
	{
		return DFMR_ERRPARAM;
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
	/* 旧パス名よりカレントディレクトリ検索                                   */
	/**************************************************************************/
	l_prsflg = 0;
	oldpath += 3;                       /* 先頭の｢A:\｣を飛ばす                */
	l_rtn = FMR_getCurrentDirCluster(
		l_drvnum,                       /* 論理ドライブ番号                   */
		( F_CHAR * )oldpath,            /* 旧パス名                           */
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
	/* 旧パス名の対象エントリ検索                                             */
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
			/* 対象ファイルが存在しない場合、戻り値をDFMR_ENOENTとする        */
			l_rtn = DFMR_ENOENT;
		}
		goto ERROR;
	}
	/* 旧パス名と新パス名の文字比較 */
	l_cnt = 0;
	l_work01 = 0;
	newpath += 3;                       /* ｢"ドライブ番号":\｣を飛ばす         */
	while( *( oldpath + l_cnt ) == *( newpath + l_cnt ) )
	{
		if( *( oldpath + l_cnt ) == NULL )
		{                               /* 最終要素まで一致した場合はエラー   */
			l_rtn = DFMR_ERRPARAM;
			goto ERROR;
		}
		if( *( oldpath + l_cnt ) == '\\' )  /* 区切り記号検出時は             */
		{                                   /* その次の位置を記憶             */
			l_work01 = l_cnt + 1;
		}
		l_cnt ++;
	}
	/* 新パス名最終要素を取得する */
	l_prsflg2 = 0;
	l_rtn = FMR_splitPath(
		( F_CHAR * )( newpath + l_work01 ), /* パス名                         */
		( F_CHAR * )&l_work03,          /* 区切り記号                         */
		&l_prsflg2                      /* FMR_searchEntry用動作フラグ        */
		);
	if( l_rtn <= DFMR_OK )              /* 名称が異常,または名称長が0の場合   */
	{
		l_rtn = DFMR_NMERR;
		goto ERROR;
	}
	l_work04 = l_rtn;
	if( l_work03 != 0x00 )
	{
		l_rtn = DFMR_NMERR;
		goto ERROR;
	}
	if( ( l_work01 + l_work04 ) > DFMR_MAXPASSSZ )
	{
		l_rtn = DFMR_NMERR;
		goto ERROR;
	}
	l_pathlen = newpath + l_work01;
	/**************************************************************************/
	/* 旧パス名対象エントリ登録位置をセーブ                                   */
	/**************************************************************************/
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN ) 
	{
		/* ロングファイル名の場合 */
		l_targetsect = l_bottompos.sectnum;
		l_targetpos = l_bottompos.slot;
		l_targetlfnsect = l_toppos.sectnum;
		l_targetlfnpos = l_toppos.slot;
	}
	else
	{
		/* ショートファイル名の場合 */
		l_targetsect = l_toppos.sectnum;
		l_targetpos = l_toppos.slot;
		l_targetlfnsect = 0;
		l_targetlfnpos = 0;
	}
	if( l_entry.attr != DFMR_ATTR_DIR )
	{
		/* 対象エントリがファイルの場合、ファイルハンドルのチェック */
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
		l_work03 = DFMR_ATTR_ARC;
	}
	else
	{
		l_work03 = DFMR_ATTR_DIR;
	}
	/**************************************************************************/
	/* 新パス名の対象エントリ検索                                             */
	/**************************************************************************/
	/* 検索開始位置設定 */
	if( l_cdirclsnum == 0 )             /* カレントディレクトリが             */
	{                                   /* ルートディレクトリの場合           */
		l_toppos.sectnum = MFMR_ROOTTOP( l_drvnum );
		                                /* 開始セクタはRoot先頭セクタ         */
		l_toppos.slot = DFMR_SRCHROOTTOPSLOT;  /* 開始セクタ内位置は先頭      */
	}
	else                                /* カレントディレクトリが             */
	{                                   /* サブディレクトリの場合             */
		l_work01 = l_cdirclsnum - DFMR_TOPCLSTNUM;  /* 開始セクタはカレント   */
		l_work01 *= MFMR_SECTPERCLST( l_drvnum );   /* クラスタ先頭のセクタ   */
		l_toppos.sectnum = l_work01 + MFMR_DATATOP( l_drvnum );
		l_toppos.slot = DFMR_SRCHDIRTOPSLOT;   /* 開始セクタ内位置は｢.｣｢..｣の直後 */
	}
	/* 対象エントリを検索する */
	l_prsflg2 |= DFMR_SRCHFLGCHK;
	l_rtn = FMR_searchEntry(
		l_drvnum,                       /* 対象ドライブ番号                   */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_bottompos,                   /* 検索終了最終位置                   */
		l_pathlen,                      /* 検索文字列                         */
		&l_entry,                       /* 取得エントリ                       */
		&l_getname[ 0 ],                /* 取得ロングファイル名               */
		l_work04,                       /* 検索文字列長                       */
		l_prsflg2                       /* 動作フラグ                         */
		);
	if( ( l_rtn != DFMR_ENOEMPTYENTRY ) /* 対象エントリなし又は               */
		&& ( l_rtn != DFMR_OK ) )       /* 空エントリ検出以外はエラー         */
	{
		if( l_rtn == DFMR_SAMENAME )    /* 対象エントリありの場合             */
		{
			l_rtn = DFMR_NMERR;
		}
		goto ERROR;
	}
	if( l_rtn == DFMR_ENOEMPTYENTRY )
	{
		/* 空きエントリ位置が存在しない場合、エントリ格納クラスタ拡張 */
		l_rtn = FMR_expandEntry(
			l_drvnum,                   /* 対象ドライブ番号                   */
			l_cdirclsnum,               /* カレントディレクトリのクラスタ番号 */
			&l_toppos                   /* エントリ検索結果:先頭位置          */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* 対象エントリがロングファイル名の場合、ショートファイル名を作成する     */
	/**************************************************************************/
	if( ( l_prsflg2 & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		l_rtn = FMR_getShortName(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			l_pathlen,                  /* 変換前文字列                       */
			l_work04,                   /* 変換前文字列長                     */
			l_work03,                   /* 対象エントリ種別                   */
			&l_getname[ 0 ],            /* 変換後文字列                       */
			l_cdirclsnum                /* カレントクラスタ番号               */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* 新パス名のエントリを作成する                                           */
	/**************************************************************************/
	FMR_memSet(                         /* ファイル名初期化                   */
		&l_entry.name[ 0 ],
		DFMR_SPACE,
		DFMR_VNAMESZ
		);
	/**************************************************************************/
	/* 作成したディレクトリエントリを登録する                                 */
	/**************************************************************************/
	l_work04 = ( F_WORD )l_prsflg2;
	if ( l_work03 == DFMR_ATTR_DIR )
	{
		l_work04 |= DFMR_SETFUNC_RENAMED;
	}
	else
	{
		l_entry.attr |= DFMR_ATTR_ARC;  /* アーカイブ属性設定                 */
		l_work04 |= DFMR_SETFUNC_RENAMEF;
	}
	l_rtn = FMR_setDirEntry(
		&l_toppos,                      /* 検索終了先頭位置                   */
		&l_bottompos,                   /* 検索終了最終位置                   */
		&l_entry,                       /* ディレクトリエントリ作成領域       */
		l_pathlen,                      /* 設定ファイル名                     */
		l_getname,                      /* 設定ショートファイル名             */
	    l_work04,                       /* 動作フラグ                         */
	    l_drvnum                        /* 対象ドライブ番号                   */
		);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	/**************************************************************************/
	/* 旧パス名のエントリを削除する                                           */
	/**************************************************************************/
	l_rtn = FMR_deleteDirEntry(
		l_targetsect,                   /* 削除対象エントリ登録セクタ番号     */
		l_targetlfnsect,                /* 削除対象最終LFNエントリ登録セクタ番号 */
		l_targetpos,                    /* 削除対象エントリ登録位置           */
		l_targetlfnpos,                 /* 削除対象最終LFNエントリ登録位置    */
	    l_prsflg,                       /* 動作フラグ                         */
	    l_drvnum                        /* 対象ドライブ番号                   */
		);
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
