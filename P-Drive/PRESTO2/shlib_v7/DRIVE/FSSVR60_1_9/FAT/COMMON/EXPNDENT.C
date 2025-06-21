/*======================================================================
 * File name    : expndent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/expndent.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: expndent.c $
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
/* 関 数 名：FMR_expandEntry                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR    対象ドライブ番号                                  */
/*         ：     F_DWORD   カレントディレクトリのクラスタ番号                */
/*         ：     TFMR_SEARCHINFO * エントリ検索結果:先頭位置                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_MEMFULL                                           */
/*         ：                                                                 */
/* 概    要：ディレクトリエントリ格納クラスタを拡張する                       */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_expandEntry(
    F_CHAR  drvnum,                     /* 対象ドライブ番号                   */
    F_DWORD cdirclsnum,                 /* カレントディレクトリのクラスタ番号 */
    TFMR_SEARCHINFO * toppos            /* エントリ検索結果:先頭位置          */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_eoc;                      /* EOC                                */
	F_INT   l_rtn;                      /* サブルーチン戻り値                 */
	F_BYTE  l_fatareanum;               /* 現在参照中のFAT読込み領域番号      */

	if( (cdirclsnum == 0) &&
		(MFMR_FATTYPE( drvnum ) != DFMR_FATTYPE32)  )
	{
		/* 対象がFAT12,16のRoot Dir Regionの場合 */
		return DFMR_MEMFULL;            /* エラー返却(メモリフル)             */
	}
	/* EOCコード取得 */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_eoc );
	/* カレントディレクトリの最終クラスタを求める */
	l_work01 = toppos->sectnum;
	l_work01 -= MFMR_DATATOP( drvnum );
	l_work01 /= MFMR_SECTPERCLST( drvnum );
	l_work01 += DFMR_TOPCLSTNUM;
	
	/* カレントディレクトリの最終クラスタを含むFATセクタを読込む */
	l_rtn = FMR_getFatSector(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		l_work01,                       /* クラスタ番号                       */
		&l_work02                       /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/* 空きクラスタを含むFATセクタ読込み領域を記憶 */
	l_fatareanum = DFMR_REFFATAREA0;    /* とりあえず0                        */
	/* 空きクラスタを取得する */
	l_work02 = FMR_getEmptyCluster(
		drvnum,                         /* 対象論理ドライブ番号               */
		( l_work01 + 1 ),               /* 検索開始クラスタ番号               */
		DFMR_REFFATAREA0                /* 検索FAT読込み領域番号              */
		);
	
	if( l_work02 == DFMR_LNG )
	{
		/* 空きクラスタが取得できない場合はFATセクタ管理領域より */
		/* 空きクラスタを取得する                                */
		l_rtn = FMR_searchEmptyCluster(
			drvnum,                     /* 対象論理ドライブ番号               */
			&l_work02                   /* 取得空きクラスタ番号               */
			);
		if( l_rtn != DFMR_OK )
		{
			return DFMR_MEMFULL;        /* エラー返却(メモリフル)             */
		}
		l_fatareanum = DFMR_REFFATAREA1;
	}
	
	/* 取得したクラスタのFATエントリをEOCとする */
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, l_fatareanum ),
		                                /* FAT読込み領域                      */
		l_work02,                       /* クラスタ番号                       */
		&l_eoc,                         /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	if( l_fatareanum != DFMR_REFFATAREA0 )
	{
		/* 最終クラスタを含むFATセクタと異なる場合はここで更新 */
		l_rtn = FMR_setFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, l_fatareanum ),
			                            /* FAT読込み領域                      */
			l_fatareanum                /* FAT読込み領域番号                  */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	/* 取得したクラスタを既存の最終クラスタにリンクさせる */
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		l_work01,                       /* クラスタ番号                       */
		&l_work02,                      /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	l_rtn = FMR_setFatSector(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		DFMR_REFFATAREA0                /* FAT読込み領域番号                  */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	if( l_fatareanum != DFMR_REFFATAREA0 )
	{
		/* 取得空きクラスタを含むFATセクタを読込む */
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                                /* FAT読込み領域                      */
			l_work02,                       /* クラスタ番号                       */
			&l_work01                       /* FATエントリ値                      */
			);
		l_fatareanum = DFMR_REFFATAREA0;
	}
	/* 空きエントリ位置を取得したクラスタの先頭セクタに設定する */
	toppos->sectnum = l_work02 - DFMR_TOPCLSTNUM;
	toppos->sectnum *= MFMR_SECTPERCLST( drvnum );
	toppos->sectnum += MFMR_DATATOP( drvnum );
	toppos->slot = DFMR_SRCHROOTTOPSLOT;
	toppos->bufnum = 0;                 /* 使用バッファ番号は0とする          */
	
	/* R/Wバッファ0を0で初期化しておく */
	FMR_memSet(
		MFMR_RWBUFFTOP( drvnum, 0 ),
		DFMR_INITBYTE,
		MFMR_BYTEPERSECT( drvnum )
		);
	/* 取得したクラスタを初期化しておく */
	l_work02 = toppos->sectnum;
	for( l_work01 = 0; l_work01 < MFMR_SECTPERCLST( drvnum ); l_work01 ++ )
	{
		l_rtn = FMR_setDataSector(
			drvnum,
			l_work02,
			0
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		l_work02 ++;                    /* 書込みセクタ番号更新               */
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
