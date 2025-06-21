/*======================================================================
 * File name    : gdirclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/gdirclst.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gdirclst.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
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
/* 関 数 名：FMR_getCurrentDirCluster                                         */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR    論理ドライブ番号                                  */
/*         ：     F_CHAR *  パス名                                            */
/*         ：     F_CHAR *  ロングファイル名用ワークエリア                    */
/*         ：     F_BYTE *  FMR_searchEntry用動作フラグ                       */
/*         ：               bit5:ON=ワイルドカードチェック指定                */
/*         ：                                                                 */
/*         ：OUT: F_BYTE *  FMR_searchEntry用動作フラグ                       */
/*         ：     TFMR_SAERCHINFO * 検索開始先頭位置                          */
/*         ：     F_CHAR **  パス名先頭からの最終要素の位置                   */
/*         ：     F_DWORD * カレントクラスタ番号                              */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 取得文字列長                                      */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：パス名よりファイル/ディレクトリ名を取得する                      */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.05 FMR_splitPath戻り値の型変更に対応               05/02/28 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_getCurrentDirCluster(
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
    F_CHAR *  pathname,                 /* パス名                             */
    F_CHAR *  getlongname,              /* ロングファイル名用ワークエリア     */
    F_BYTE *  prsflg,                   /* FMR_searchEntry用動作フラグ        */
    TFMR_SEARCHINFO * toppos,           /* 検索開始/終了先頭位置              */
    F_CHAR **  pathlen,                 /* パス名先頭からの最終要素の位置     */
    F_DWORD * clstnum                   /* カレントクラスタ番号               */
)
{
	/* エントリ検索処理パラメータ */
	TFMR_SEARCHINFO l_toppos;           /* 検索開始/終了先頭位置              */
	TFMR_SEARCHINFO l_bottompos;        /* 検索終了最終位置                   */
	TFMR_DIRENTRY l_entry;              /* 取得エントリ                       */

	F_DWORD l_getclstnum;               /* 取得クラスタ番号                   */
	F_DWORD l_work01;                   /* ワークエリア                       */
	T_ENDIAN l_endian;                  /* エンディアン調整用ワーク           */

	F_INT  l_rtn;                       /* サブルーチン戻り値                 */
	F_WORD l_len;                       /* パス要素長                         */
	F_WORD l_pathlen;                   /* パス名長                           */
	F_CHAR l_delimiter;                 /* 区切り記号                         */

	l_getclstnum = 0;                   /* 取得クラスタ番号初期値 = 0         */
	l_pathlen = 3;                      /* パス名長初期値 = 3(A:\)            */
	l_toppos.sectnum = MFMR_ROOTTOP( drvnum );
	l_toppos.slot = DFMR_SRCHROOTTOPSLOT;
	l_toppos.bufnum = 0;
	
	while( 1 )
	{
		/* パスの先頭要素を取得 */
		l_rtn = FMR_splitPath(
			pathname,                   /* 対象パス名                         */
			&l_delimiter,               /* 区切り記号                         */
			prsflg                      /* FMR_searchEntry用動作フラグ        */
			);
			
		if( l_rtn <= DFMR_OK )          /* 名称が異常,または名称長が0の場合   */
		{
			return DFMR_NMERR;          /* 名称異常                           */
		}
		l_len = l_rtn;
		l_pathlen += l_len;             /* パス名長 + 文字列長                */
		if( l_delimiter != NULL )
		{
			l_pathlen ++;               /* パス名長 + 区切り記号長            */
		}
		if( l_pathlen > DFMR_MAXPASSSZ )
		{
			return DFMR_NMERR;          /* パス名オーバー                     */
		}
		if( l_delimiter == NULL )       /* 最終要素検出                       */
		{
			*( clstnum ) = l_getclstnum;/* カレントクラスタ番号を設定         */
			*( pathlen ) = pathname;    /* パス名先頭からの最終要素の位置を設定   */
			/* 検索開始位置設定 */
			if( l_getclstnum == 0 )             /* カレントディレクトリが     */
			{                                   /* ルートディレクトリの場合   */
				toppos->sectnum = MFMR_ROOTTOP( drvnum );
				                                /* 開始セクタはRoot先頭セクタ */
				toppos->slot = DFMR_SRCHROOTTOPSLOT;  /* 開始セクタ内位置は先頭 */
			}
			else                                /* カレントディレクトリが     */
			{                                   /* サブディレクトリの場合     */
				l_work01 = l_getclstnum - DFMR_TOPCLSTNUM;  /* 開始セクタはカレント */
				l_work01 *= MFMR_SECTPERCLST( drvnum );     /* クラスタ先頭のセクタ */
				toppos->sectnum = l_work01 + MFMR_DATATOP( drvnum );
				toppos->slot = DFMR_SRCHDIRTOPSLOT;   /* 開始セクタ内位置は｢.｣｢..｣の直後 */
			}
			return l_len;               /* 正常終了                           */
		}
		/* エントリ検索 */
		l_rtn = FMR_searchEntry( 
			drvnum,                     /* 対象論理ドライブ番号               */
			&l_toppos,                  /* 検索開始/終了先頭位置              */
			&l_bottompos,               /* 検索終了最終位置                   */
			pathname,                   /* 検索対象文字列                     */
			&l_entry,                   /* 取得エントリ                       */
			getlongname,                /* 取得ロングファイル名               */
			l_len,                      /* 検索文字列長                       */
			*( prsflg )                 /* 動作フラグ                         */
			);
			
		if( l_rtn != DFMR_OK )          /* 検索失敗時はエラー返却             */
		{
			return l_rtn;
		}
		if( l_entry.attr != DFMR_ATTR_DIR )     /* 属性がディレクトリ以外     */
		{
			return DFMR_EACCES;         /* エラー返却                         */
		}
		/* 先頭クラスタ番号取得 */
		l_endian.wordval = l_entry.fstclushi;
		l_getclstnum = (F_DWORD)(l_endian.byteval[1]);
		l_getclstnum = (l_getclstnum << 8) | (F_DWORD)(l_endian.byteval[0]);
		l_endian.wordval = l_entry.fstcluslo;
		l_getclstnum = (l_getclstnum << 8) | (F_DWORD)(l_endian.byteval[1]);
		l_getclstnum = (l_getclstnum << 8) | (F_DWORD)(l_endian.byteval[0]);
		l_toppos.sectnum = FMR_getSectNum(
			l_getclstnum,
			0,
			drvnum
			);
		l_toppos.slot = DFMR_SRCHDIRTOPSLOT;
		l_toppos.bufnum = 0;
		/* 先頭要素更新 */
		pathname += l_len + 1;          /* 文字列長+ 区切り記号               */
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
