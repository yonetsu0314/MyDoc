/*======================================================================
 * File name    : subopen.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/subopen.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: subopen.c $
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
/* 関 数 名：FMR_subOpen                                                      */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR * 対象ファイルパス名                                 */
/*         ：     F_INT    オープンフラグ                                     */
/*         ：     F_BYTE   属性                                               */
/*         ：     F_CHAR   論理ドライブ番号                                   */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  ファイルハンドル                                       */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_EMFILE                                            */
/*         ：          DFMR_MEMFULL                                           */
/*         ：          DFMR_FATALERR                                          */
/*         ：          DFMR_EINVAL                                            */
/*         ：          DFMR_EACCES                                            */
/*         ：          DFMR_ENOENT                                            */
/*         ：          DFMR_EEXIST                                            */
/*         ：                                                                 */
/* 概    要：ファイルのオープン                                               */
/*         ：                                                                 */
/* 作 成 日：2004.05.27                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 FMR_chkOpenFlgに引数オープンモード追加          05/02/09 Y.Yoshizumi */
/*       ファイル属性にアーカイブ属性設定                05/02/09 Y.Yoshizumi */
/* 01.08 LFN検索にてSFNエントリヒット時、SFNとして処理   06/01/16 Y.Yoshizumi */
/* 01.08 ディレクトリ拡張を、新規オープンのオープンフラグチェック後に行う     */
/*                                                       06/01/16 Y.Yoshizumi */
/* 01.09 ファイルハンドル排他制御処理追加                06/03/01 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"

static F_INT FMR_chkOpenFlg(            /* オープンフラグと属性のチェック     */
	F_INT    oflg,                      /* オープンフラグ                     */
	F_BYTE   atrb,                      /* ファイル属性                       */
	F_BYTE   mode                       /* オープンモード(0=新規作成、1=既存) */
);

F_INT FMR_subOpen ( 
	F_CHAR * path,                      /* 対象ファイルパス名                 */
	F_INT    flag,                      /* オープンフラグ                     */
	F_BYTE   atrb,                      /* 属性                               */
    F_CHAR   drvnum                     /* 論理ドライブ番号                   */
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

	F_INT l_handle;                     /* 取得ファイルハンドル番号           */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_rtn1;                       /* サブルーチン戻り値                 */

	F_WORD l_work04;                    /* 対象ファイル名長,FMR_setDirEntry用 */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ファイル名                     */

	F_BYTE l_targetpos;                 /* 対象エントリ登録位置               */
	F_BYTE l_prsflg;                    /* 検索処理パラメータ:動作フラグ      */

	/**************************************************************************/
	/* カレントディレクトリ検索                                               */
	/**************************************************************************/
	l_prsflg = 0;
	path += 3;                          /* 先頭の｢A:\｣を飛ばす                */
	l_rtn = FMR_getCurrentDirCluster(
		drvnum,                         /* 論理ドライブ番号                   */
		( F_CHAR * )path,               /* パス名                             */
		&l_getname[ 0 ],                /* ロングファイル名用ワークエリア     */
		&l_prsflg,                      /* FMR_searchEntry用動作フラグ        */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_pathlen,                     /* パス名先頭からの最終要素の位置     */
		&l_cdirclsnum                   /* カレントクラスタ番号               */
		);
	if( l_rtn < DFMR_OK )               /* エラー終了の場合                   */
	{
		return l_rtn;
	}
	l_work04 = l_rtn;
	/**************************************************************************/
	/* 対象ファイル検索                                                       */
	/**************************************************************************/
	l_prsflg |= DFMR_SRCHFLGCHK;
	l_rtn = FMR_searchEntry(
		drvnum,                         /* 対象ドライブ番号                   */
		&l_toppos,                      /* 検索開始/終了先頭位置              */
		&l_bottompos,                   /* 検索終了最終位置                   */
		l_pathlen,                      /* 検索文字列                         */
		&l_entry,                       /* 取得エントリ                       */
		&l_getname[ 0 ],                /* 取得ロングファイル名               */
		l_work04,                       /* 検索文字列長                       */
		l_prsflg                        /* 動作フラグ                         */
		);
	if( ( l_rtn != DFMR_ENOEMPTYENTRY ) /* 対象エントリなし又は               */
		&& ( l_rtn != DFMR_SAMENAME )   /* 同一エントリ名有り又は             */
		&& ( l_rtn != DFMR_OK ) )       /* 空エントリ検出以外はエラー         */
	{
		return l_rtn;
	}

	if( l_rtn == DFMR_SAMENAME )
	{
		/**********************************************************************/
		/* 対象ファイルありの場合：既存ファイルオープン                       */
		/**********************************************************************/
		/**********************************************************************/
		/* 対象ファイル登録位置を取得                                         */
		/**********************************************************************/
		if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN ) 
		{
			/* ロングファイル名指定の場合 */
			if( l_bottompos.sectnum == 0 )
			{
				/* ショートファイル名エントリと一致した場合 */
				/* ショートファイル名指定とする */
				l_targetsect = l_toppos.sectnum;
				l_targetpos = l_toppos.slot;
				l_prsflg &= ~DFMR_SRCHFLGLFN;
			}
			else
			{
				l_targetsect = l_bottompos.sectnum;
				l_targetpos = l_bottompos.slot;
			}
		}
		else
		{
			/* ショートファイル名指定の場合 */
			l_targetsect = l_toppos.sectnum;
			l_targetpos = l_toppos.slot;
		}
		/**********************************************************************/
		/* ファイルハンドルのチェック                                         */
		/**********************************************************************/
		for( l_work04 = 0; l_work04 < DFMR_USR_MAXFILEHANDLE; l_work04 ++ )
		{
			if( ( l_targetsect ==  tfmr_filehandle[ l_work04 ].entdirsectnum )
				&& ( l_targetpos ==  tfmr_filehandle[ l_work04 ].entdirsectpos )
				&& ( drvnum ==  tfmr_filehandle[ l_work04 ].drivenum ) )
			{
				/* ディレクトリエントリのセクタ/セクタ内位置が                */
				/* ファイルハンドルの同パラメータと等しい場合は               */
				/* 同一ファイルと見なす(名称一致は検索処理内で行っている)     */
				return DFMR_MULTIOPEN;  /* 2重オープンエラー                  */
			}
		}
		/**********************************************************************/
		/* オープンフラグのチェック                                           */
		/**********************************************************************/
		l_rtn = FMR_chkOpenFlg(
			flag,                       /* オープンフラグ                     */
			l_entry.attr,               /* エントリの属性                     */
			1                           /* 既存ファイル                       */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
		FMR_setBigEndianEntry(
			&l_entry
		);
#endif /* DFMR_ENDIAN_BIG */
		/**************************************************************************/
		/* ファイルハンドル用セマフォ資源獲得                                     */
		/**************************************************************************/
		l_rtn = FMR_osWaiSem(
			g_semid
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/**************************************************************************/
		/* 空きハンドル検索                                                       */
		/**************************************************************************/
		for( l_handle = 0; l_handle < DFMR_USR_MAXFILEHANDLE; l_handle ++ )
		{
			if( tfmr_filehandle[ l_handle ].direntry.name[ 0 ] == DFMR_INITBYTE )
			{                  /* ハンドルの1Byte目が初期値なら空きハンドルとする */
				break;
			}
		}
		if( l_handle == DFMR_USR_MAXFILEHANDLE )/* 空きハンドルなしの場合         */
		{
			/* ファイルハンドル用セマフォ資源返却 */
			l_rtn = FMR_osSigSem(
				g_semid
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			return DFMR_EMFILE;             /* オープンファイルオーバー返却       */
		}
		/**********************************************************************/
		/* ファイルハンドルを設定する                                         */
		/**********************************************************************/
		tfmr_filehandle[ l_handle ].direntry = l_entry; /* ディレクトリエントリ */
		tfmr_filehandle[ l_handle ].entdirsectnum = l_targetsect;
		                                /* エントリが登録されているセクタ番号 */
		tfmr_filehandle[ l_handle ].fptr = 0;   /* ファイルポインタ           */
		l_work01 = ( F_DWORD )l_entry.fstclushi;
		l_work01 <<= 16;
		l_work01 |= l_entry.fstcluslo;
		l_work01 -= 2;
		l_work01 *= MFMR_SECTPERCLST( drvnum );
		l_work01 += MFMR_DATATOP( drvnum );
		tfmr_filehandle[ l_handle ].crtsectnum = l_work01; /* カレントセクタ番号 */
		tfmr_filehandle[ l_handle ].crtsectofst = 0;
		                                /* カレントセクタ内ポインタオフセット */
		tfmr_filehandle[ l_handle ].acsmode = ( F_WORD )flag;
		tfmr_filehandle[ l_handle ].acsmode &= ~DFMR_O_CREAT;
		                                /* ファイルアクセスモード             */
		tfmr_filehandle[ l_handle ].entdirsectpos = l_targetpos;
		                              /* エントリが登録されているセクタ内位置 */
		tfmr_filehandle[ l_handle ].drivenum = drvnum;      /* ドライブ番号   */
	}
	else /* l_rtn == DFMR_ENOEMPTYENTRY or DFMR_OK */
	{
		/**********************************************************************/
		/* 対象ファイルなしの場合：新規ファイルオープン                       */
		/**********************************************************************/
		/**********************************************************************/
		/* オープンフラグチェック                                             */
		/**********************************************************************/
		atrb |= DFMR_ATTR_ARC;          /* アーカイブ属性設定                 */
		l_rtn1 = FMR_chkOpenFlg(
			flag,                       /* オープンフラグ                     */
			atrb,                       /* ファイル属性                       */
			0                           /* 新規作成ファイル                   */
			);
		if( l_rtn1 != DFMR_OK )
		{
			return l_rtn1;
		}
		/**************************************************************************/
		/* ファイルハンドル用セマフォ資源獲得                                     */
		/**************************************************************************/
		l_rtn1 = FMR_osWaiSem(
			g_semid
			);
		if( l_rtn1 != DFMR_OK )
		{
			return l_rtn1;
		}
		/**************************************************************************/
		/* 空きハンドル検索                                                       */
		/**************************************************************************/
		for( l_handle = 0; l_handle < DFMR_USR_MAXFILEHANDLE; l_handle ++ )
		{
			if( tfmr_filehandle[ l_handle ].direntry.name[ 0 ] == DFMR_INITBYTE )
			{                  /* ハンドルの1Byte目が初期値なら空きハンドルとする */
				break;
			}
		}
		if( l_handle == DFMR_USR_MAXFILEHANDLE )/* 空きハンドルなしの場合         */
		{
			/* ファイルハンドル用セマフォ資源返却 */
			l_rtn = FMR_osSigSem(
				g_semid
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			return DFMR_EMFILE;             /* オープンファイルオーバー返却       */
		}
		if( l_rtn == DFMR_ENOEMPTYENTRY )
		{
			/* 空きエントリ位置が存在しない場合、エントリ格納クラスタ拡張 */
			l_rtn = FMR_expandEntry(
				drvnum,                     /* 対象ドライブ番号                   */
				l_cdirclsnum,               /* カレントディレクトリのクラスタ番号 */
				&l_toppos                   /* エントリ検索結果:先頭位置          */
				);
			if( l_rtn != DFMR_OK )
			{
				/* ファイルハンドル用セマフォ資源返却 */
				l_rtn1 = FMR_osSigSem(
					g_semid
					);
				if( l_rtn1 != DFMR_OK )
				{
					return l_rtn1;
				}
				return l_rtn;
			}
		}
		/**********************************************************************/
		/* 対象ファイルがロングファイル名の場合、ショートファイル名を作成する */
		/**********************************************************************/
		if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
		{
			l_rtn = FMR_getShortName(
				drvnum,                 /* 対象論理ドライブ番号               */
				l_pathlen,              /* 変換前文字列                       */
				l_work04,               /* 変換前文字列長                     */
				DFMR_ATTR_ARC,          /* 対象エントリ種別:ディレクトリ以外  */
				&l_getname[ 0 ],        /* 変換後文字列                       */
				l_cdirclsnum            /* カレントクラスタ番号               */
				);
			if( l_rtn != DFMR_OK )
			{
				/* ファイルハンドル用セマフォ資源返却 */
				l_rtn1 = FMR_osSigSem(
					g_semid
					);
				if( l_rtn1 != DFMR_OK )
				{
					return l_rtn1;
				}
				return l_rtn;
			}
		}
		/**********************************************************************/
		/* ディレクトリエントリを作成する(ファイル名以外)                     */
		/**********************************************************************/
		FMR_initDirEntry(
			&l_entry,
			0,
			0,
			atrb
			);
		/**********************************************************************/
		/* 作成したディレクトリエントリを登録する                             */
		/**********************************************************************/
		l_work04 = ( F_WORD )l_prsflg;
		l_work04 |= DFMR_SETFUNC_OPEN;
		SetBackupFlg(drvnum);
		l_rtn = FMR_setDirEntry(
			&l_toppos,                  /* 検索終了先頭位置                   */
			&l_bottompos,               /* 検索終了最終位置                   */
			&l_entry,                   /* ディレクトリエントリ作成領域       */
			l_pathlen,                  /* 設定ファイル名                     */
			l_getname,                  /* 設定ショートファイル名             */
		    l_work04,                   /* 動作フラグ                         */
		    drvnum                      /* 対象ドライブ番号                   */
			);
		ClearBackupFlg(drvnum);
		if( l_rtn != DFMR_OK )
		{
			/* ファイルハンドル用セマフォ資源返却 */
			l_rtn1 = FMR_osSigSem(
				g_semid
				);
			if( l_rtn1 != DFMR_OK )
			{
				return l_rtn1;
			}
			return l_rtn;
		}
		/**********************************************************************/
		/* ファイルハンドルを設定する                                         */
		/**********************************************************************/
		tfmr_filehandle[ l_handle ].direntry = l_entry;
		tfmr_filehandle[ l_handle ].entdirsectnum = l_toppos.sectnum;
		tfmr_filehandle[ l_handle ].fptr = 0;
		tfmr_filehandle[ l_handle ].crtsectnum = 0;
		tfmr_filehandle[ l_handle ].crtsectofst = 0;
		tfmr_filehandle[ l_handle ].acsmode = ( F_WORD )( flag );
		tfmr_filehandle[ l_handle ].acsmode &= ~DFMR_O_CREAT;
		tfmr_filehandle[ l_handle ].acsmode &= ~DFMR_O_EXCL;
		tfmr_filehandle[ l_handle ].entdirsectpos = l_toppos.slot;
		tfmr_filehandle[ l_handle ].drivenum = drvnum;
	}
	/**************************************************************************/
	/* ファイルハンドル用セマフォ資源返却                                     */
	/**************************************************************************/
	l_rtn = FMR_osSigSem(
		g_semid
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/**************************************************************************/
	/* ファイルハンドル番号返却                                               */
	/**************************************************************************/
	return l_handle;
}

static F_INT FMR_chkOpenFlg(            /* オープンフラグと属性のチェック     */
	F_INT  oflg,                        /* オープンフラグ                     */
	F_BYTE atrb,                        /* ファイル属性                       */
	F_BYTE mode                         /* オープンモード(0=新規作成、1=既存) */
)
{
	F_INT l_work01;                     /* ワークエリア                       */
	/**************************************************************************/
	/* フラグ組合せチェック                                                   */
	/**************************************************************************/
	/* Write Only指定とR/W指定を同時指定の場合はエラー:EINVAL */
	l_work01 = ( F_INT )( oflg & ( DFMR_O_WRONLY | DFMR_O_RDWR ) );
	if( l_work01 == ( DFMR_O_WRONLY | DFMR_O_RDWR ) )
	{
		return DFMR_EINVAL;
	}
	
	/* Exclusive指定はCreateと同時指定でなければ異常 */
	l_work01 = ( F_INT )( oflg & DFMR_O_EXCL );
	if( l_work01 == DFMR_O_EXCL )
	{
		l_work01 = ( F_INT )( oflg & DFMR_O_CREAT );
		if( l_work01 != DFMR_O_CREAT )
		{
			return DFMR_EINVAL;
		}
	}
	/**************************************************************************/
	/* 属性-フラグ整合性せチェック                                            */
	/**************************************************************************/
	/* 属性がディレクトリの場合はエラー:EACCES */
	if( ( atrb & DFMR_ATTR_DIR ) == DFMR_ATTR_DIR )
	{
		return DFMR_EACCES;
	}

	/* 新規作成時にCreate指定でなければエラー:ENOENT */
	if( mode == 0 )
	{
		l_work01 = ( F_INT )( oflg & DFMR_O_CREAT );
		if( l_work01 != DFMR_O_CREAT )
		{
			return DFMR_ENOENT;
		}
	}

	if( mode == 1 )
	{
		/* 既存ファイルにExclusive指定ならエラー:EEXIST */
		l_work01 = ( F_INT )( oflg & DFMR_O_EXCL );
		if( l_work01 == DFMR_O_EXCL )
		{
			return DFMR_EEXIST;
		}
		/* 読込み専用ファイルにCreate,Append,R/W,Write Onlyの */
		/* いずれかが指定されていればエラー:EACCES            */
		if( ( atrb & DFMR_ATTR_RONLY ) == DFMR_ATTR_RONLY )
		{
			l_work01 = ( F_INT )( oflg & DFMR_O_CREAT );
			if( l_work01 == DFMR_O_CREAT )
			{
				return DFMR_EACCES;
			}
			l_work01 = ( F_INT )( oflg & DFMR_O_APPEND );
			if( l_work01 == DFMR_O_APPEND )
			{
				return DFMR_EACCES;
			}
			l_work01 = ( F_INT )( oflg & DFMR_O_WRONLY );
			if( l_work01 == DFMR_O_WRONLY )
			{
				return DFMR_EACCES;
			}
			l_work01 = ( F_INT )( oflg & DFMR_O_RDWR );
			if( l_work01 == DFMR_O_RDWR )
			{
				return DFMR_EACCES;
			}
		}
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
