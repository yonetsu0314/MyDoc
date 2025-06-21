/*======================================================================
 * File name    : submkdir.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/submkdir.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: submkdir.c $
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
/* 関 数 名：FMR_subMkdir                                                     */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR * パス名                                             */
/*         ：     F_CHAR   論理ドライブ番号                                   */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_MEMFULL                                           */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ディレクトリの作成                                               */
/*         ：                                                                 */
/* 作 成 日：2004.09.10                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_subMkdir ( 
    F_CHAR * path,                      /* パス名                             */
    F_CHAR   drvnum                     /* 論理ドライブ番号                   */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cdirclsnum;               /* カレントディレクトリの             */
	                                    /* クラスタ番号                       */
	F_DWORD l_dataclstnum;              /* データ部クラスタ番号               */
    F_CHAR * l_pathlen;                 /* パス名先頭からの最終要素の位置     */

	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */
	TFMR_DIRENTRY * l_dirptr;           /* エントリ作成用ポインタ             */
	TFMR_SEARCHINFO l_toppos;           /* 検索処理パラメータ:先頭位置        */
	TFMR_SEARCHINFO l_bottompos;        /* 検索処理パラメータ:最終位置        */

	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_CHAR l_getname[ DFMR_MAXFILESZ ]; /* 取得ファイル名                     */

	F_WORD l_work04;                    /* 対象ファイル名長,FMR_setDirEntry用 */

	F_BYTE l_work02;
	F_BYTE l_work03;
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
	/* 対象ディレクトリ検索                                                   */
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
		&& ( l_rtn != DFMR_OK ) )       /* 空エントリ検出以外はエラー         */
	{
		if( l_rtn == DFMR_SAMENAME )    /* 対象エントリありの場合             */
		{
			if( l_entry.attr == DFMR_ATTR_DIR )
			{
				/* 属性がディレクトリの場合 */
				l_rtn = DFMR_NMERR;
			}
			else
			{
				/* 属性がディレクトリ以外(対象エントリがファイル)の場合 */
				l_rtn = DFMR_EEXIST;
			}
		}
		return l_rtn;
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
			return l_rtn;
		}
	}
	/**************************************************************************/
	/* 新規作成ディレクトリ用空きクラスタを取得する                           */
	/**************************************************************************/
	if( ( l_cdirclsnum == 0 ) &&    /* カレントクラスタがRoot Dir Regionの場合*/
		(MFMR_FATTYPE( drvnum ) == DFMR_FATTYPE32)  )
	{
		/* 対象がFAT32のRoot Dir Regionの場合 */
		l_cdirclsnum = MFMR_ROOTTOP( drvnum );
		l_cdirclsnum -= MFMR_DATATOP( drvnum );
		l_cdirclsnum /= MFMR_SECTPERCLST( drvnum );
		l_cdirclsnum += 2; 
	}
	/* 空きクラスタを取得する */
	l_rtn = FMR_searchEmptyCluster(
		drvnum,                         /* 対象論理ドライブ番号               */
		&l_dataclstnum                  /* 取得空きクラスタ番号               */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_MEMFULL;            /* エラー返却(メモリフル)             */
	}
	l_work03 = DFMR_REFFATAREA1;

	/* 読み込んだFATセクタがFAT読込み領域0のセクタと同じ場合は */
	/* 以後の操作をFAT0で行う */
	if( (MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 ) & ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD) )
		== (MFMR_FATSECTNUM( drvnum, l_work03 ) & ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD) ) )
	{
		l_work03 = DFMR_REFFATAREA0;
	}
	/* 取得した空きクラスタのFATエントリをEOCとする */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_work01 );
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, l_work03 ),
		                                /* FAT読込み領域                      */
		l_dataclstnum,                  /* クラスタ番号                       */
		&l_work01,                      /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/* FAT読込み領域を書き戻す */
	l_rtn = FMR_setFatSector(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, l_work03 ),
									    /* FAT読込み領域                      */
		l_work03                        /* FAT読込み領域番号                  */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/**************************************************************************/
	/* R/Wバッファにディレクトリ｢.｣｢..｣を作成する                             */
	/**************************************************************************/
	/* R/Wバッファを初期化 */
	l_work03 = l_toppos.bufnum;
	MFMR_FLGSWITCH( l_work03 );
	FMR_memSet(
		MFMR_RWBUFFTOP( drvnum, l_work03 ),
		DFMR_INITBYTE,
		MFMR_BYTEPERSECT( drvnum )
		);
	/* R/Wバッファにエントリのポインタをキャスト */
	l_dirptr = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( drvnum, l_work03 ) );
	/* ｢.｣を作成 */
	l_dirptr->name[ 0 ] = '.';          /* 名称(+拡張子)は｢.｣                 */
	FMR_initDirEntry(
		l_dirptr,
		l_dataclstnum,
		1,
		DFMR_ATTR_DIR
		);
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	FMR_setBigEndianEntry(
		l_dirptr
	);
#endif /* DFMR_ENDIAN_BIG */
	/* ｢..｣を作成 */
	l_dirptr ++;                        /* ポインタを一つ進める               */
	l_dirptr->name[ 0 ] = '.';          /* 名称(+拡張子)は｢..｣                */
	l_dirptr->name[ 1 ] = '.';
	FMR_initDirEntry(
		l_dirptr,
		l_cdirclsnum,
		2,
		DFMR_ATTR_DIR
		);
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	FMR_setBigEndianEntry(
		l_dirptr
	);
#endif /* DFMR_ENDIAN_BIG */
	/**************************************************************************/
	/* ディレクトリ｢.｣｢..｣を取得した空きクラスタの先頭セクタに書込む          */
	/**************************************************************************/
	/* パラメータ設定 */
	l_work01 = FMR_getSectNum(
		l_dataclstnum,                  /* クラスタ番号                       */
		0,                              /* クラスタ内セクタ番号               */
		drvnum                          /* ドライブ番号                       */
		);
	/* 書込み */
	l_rtn = FMR_setDataSector(
		drvnum,
		l_work01,
		l_work03
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/**************************************************************************/
	/* 取得した空きクラスタの後続セクタを0で埋める                            */
	/**************************************************************************/
	l_work01 ++;                        /* 書込みセクタ番号更新               */
	/* R/Wバッファを初期化 */
	FMR_memSet(
		MFMR_RWBUFFTOP( drvnum, l_work03 ),
		DFMR_INITBYTE,
		MFMR_BYTEPERSECT( drvnum )
		);
	for( l_work02 = 1; l_work02 < MFMR_SECTPERCLST( drvnum ); l_work02 ++ )
	{
		l_rtn = FMR_setDataSector(
			drvnum,
			l_work01,
			l_work03
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		l_work01 ++;                    /* 書込みセクタ番号更新               */
	}
	/**************************************************************************/
	/* 対象ディレクトリがロングファイル名の場合、ショートファイル名を作成する */
	/**************************************************************************/
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		l_rtn = FMR_getShortName(
			drvnum,                     /* 対象論理ドライブ番号               */
			l_pathlen,                  /* 変換前文字列                       */
			l_work04,                   /* 変換前文字列長                     */
			DFMR_ATTR_DIR,              /* 対象エントリ種別                   */
			&l_getname[ 0 ],            /* 変換後文字列                       */
			l_cdirclsnum                /* カレントクラスタ番号               */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	/**************************************************************************/
	/* ディレクトリエントリを作成する(ファイル名以外)                         */
	/**************************************************************************/
	FMR_initDirEntry(
		&l_entry,
		l_dataclstnum,
		0,
		DFMR_ATTR_DIR
		);
	/**************************************************************************/
	/* 作成したディレクトリエントリを登録する                                 */
	/**************************************************************************/
	l_work04 = ( F_WORD )l_prsflg;
	l_work04 |= DFMR_SETFUNC_MKDIR;
	SetBackupFlg(drvnum);
	l_rtn = FMR_setDirEntry(
		&l_toppos,                      /* 検索終了先頭位置                   */
		&l_bottompos,                   /* 検索終了最終位置                   */
		&l_entry,                       /* ディレクトリエントリ作成領域       */
		l_pathlen,                      /* 設定ファイル名                     */
		l_getname,                      /* 設定ショートファイル名             */
	    l_work04,                       /* 動作フラグ                         */
	    drvnum                          /* 対象ドライブ番号                   */
		);
	ClearBackupFlg(drvnum);
	/**************************************************************************/
	/* 終了(FMR_setDirEntryの戻り値をそのまま返却)                            */
	/**************************************************************************/
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
