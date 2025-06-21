/*======================================================================
 * File name    : setfinfo.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/setfinfo.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: setfinfo.c $
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
/* 関 数 名：FMR_setFileInfo                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY *  取得エントリ                               */
/*         ：     F_CHAR *         取得ロングファイル名                       */
/*         ：     TFMR_FILE_INFO * ファイル状態格納領域                       */
/*         ：                                                                 */
/* 戻 り 値：VOID                                                             */
/*         ：                                                                 */
/* 概    要：ファイル状態を設定する                                           */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 ShortFileName時、LongFileNameをコピーしない     04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_setFileInfo(
	TFMR_DIRENTRY *  entry,             /* 取得エントリ                       */
	F_CHAR *         getname,           /* 取得ロングファイル名               */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
)
{
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	T_ENDIAN l_endian;                  /* エンディアン調整用ワーク           */
#endif /* DFMR_ENDIAN_BIG */

	/**************************************************************************/
	/* ファイル状態の設定                                                     */
	/**************************************************************************/
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	l_endian.wordval = entry->crttime;		/* 作成時刻                       */
	fileinfo->crttime = (F_WORD)(l_endian.byteval[1]);
	fileinfo->crttime = (fileinfo->crttime << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->crtdate;		/* 作成日付                       */
	fileinfo->crtdate = (F_WORD)(l_endian.byteval[1]);
	fileinfo->crtdate = (fileinfo->crtdate << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->lstaccdate;	/* アクセス日                     */
	fileinfo->lstaccdate = (F_WORD)(l_endian.byteval[1]);
	fileinfo->lstaccdate = (fileinfo->lstaccdate << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->wrttime;		/* 更新時刻                       */
	fileinfo->wrttime = (F_WORD)(l_endian.byteval[1]);
	fileinfo->wrttime = (fileinfo->wrttime << 8) | (F_WORD)(l_endian.byteval[0]);
	l_endian.wordval = entry->wrtdate;		/* 更新日付                       */
	fileinfo->wrtdate = (F_WORD)(l_endian.byteval[1]);
	fileinfo->wrtdate = (fileinfo->wrtdate << 8) | (F_WORD)(l_endian.byteval[0]);

	l_endian.dwordval = entry->filesize;	/* サイズ                         */
	fileinfo->filesize = (F_DWORD)(l_endian.byteval[3]);
	fileinfo->filesize = (fileinfo->filesize << 8) | (F_DWORD)(l_endian.byteval[2]);
	fileinfo->filesize = (fileinfo->filesize << 8) | (F_DWORD)(l_endian.byteval[1]);
	fileinfo->filesize = (fileinfo->filesize << 8) | (F_DWORD)(l_endian.byteval[0]);
#else /* DFMR_ENDIAN_BIG */             /* Little Endian                      */
	fileinfo->crtdate = entry->crtdate;     /* 作成日付                       */
	fileinfo->crttime = entry->crttime;     /* 作成時刻                       */
	fileinfo->lstaccdate = entry->lstaccdate;   /* 最終アクセス日付           */
	fileinfo->wrtdate = entry->wrtdate;     /* 更新日付                       */
	fileinfo->wrttime = entry->wrttime;     /* 更新時刻                       */
	fileinfo->filesize = entry->filesize;   /* ファイルサイズ                 */
#endif /* DFMR_ENDIAN_BIG */
	fileinfo->crtmsec = entry->crttimetenth;    /* 作成時刻(msec)             */
	fileinfo->attr = entry->attr;           /* ファイル属性                   */
	FMR_memCopy(                            /* ショートファイル名             */
		&( fileinfo->sfn[ 0 ] ),            /* コピー先                       */
		&( entry->name[ 0 ] ),              /* コピー元                       */
		( DFMR_FNAMESZ + DFMR_EXTSZ )       /* コピーサイズ                   */
		);
	if ( *getname != NULL ) {
		FMR_memCopy(                        /* ロングファイル名               */
			&( fileinfo->lfn[ 0 ] ),        /* コピー先                       */
			( F_BYTE * )getname,            /* コピー元                       */
			DFMR_MAXFILESZ                  /* コピーサイズ                   */
			);
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
