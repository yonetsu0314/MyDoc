/*======================================================================
 * File name    : frchaoff.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/frchaoff.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: frchaoff.c $
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
/* 関 数 名：FMR_framChainClstBitOff                                          */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：     F_DWORD EOCコード                                           */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：                                                                 */
/* 戻 り 値：DFMR_OK 正常終了                                                 */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：チェインされたクラスタの使用状況をOFFにする                      */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_INT FMR_framChainClstBitOff(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD eoc,                        /* EOCコード                          */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_nowclst;                  /* 現在のクラスタ番号                 */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_BYTE l_badflg;                    /* BADクラスタ検出判定用フラグ        */
	                                    /* ON :BADクラスタ検出中とする        */
	                                    /* OFF:現在読込んでいるFATセクタ内で  */
	                                    /*     BADクラスタは未検出            */
	
	/* 先頭クラスタ番号を現在のクラスタとする */
	l_nowclst = topclstnum;
	
	/* BADクラスタ未検出とする */
	l_badflg = DFMR_FLGOFF;
	
	/* FAT読込み領域0に先頭クラスタ番号を含むFATセクタを読込み */
	/* 先頭クラスタ番号のFATエントリ値を取得する               */
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		l_nowclst,                      /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/* 先頭クラスタに対するクラスタ使用状況をOFFとする */
	FMR_framBitOff(
		l_nowclst,                      /* クラスタ番号                       */
		ramaddr                         /* クラスタ使用状況展開領域           */
		);
	
	/* 現在のクラスタをFATエントリとする */
	l_nowclst = l_fatentry;
	while( 1 )
	{
		/* クラスタ番号がEOCなら処理終了 */
		if( l_nowclst >= eoc )
		{
			return DFMR_OK;
		}
		/* クラスタ番号に対するクラスタ使用状況をOFFとする */
		FMR_framBitOff(
			l_nowclst,                  /* クラスタ番号                       */
			ramaddr                     /* クラスタ使用状況展開領域           */
			);
		/* クラスタ番号に対するFATエントリを取得する */
		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_nowclst,                  /* クラスタ番号                       */
			&l_fatentry,                /* FATエントリ値                      */
			DFMR_FLGOFF                 /* Read/Write種別  OFF:取得           */
			);
		if( l_rtn != DFMR_OK )          /* エラーの場合は対象クラスタが       */
		{                               /* 他のFATセクタにあると判断          */
			if( l_rtn == DFMR_CLSTNUMERR )
			{                           /* クラスタ番号異常の場合は異常終了   */
				return l_rtn;
			}
			if( l_badflg != DFMR_FLGOFF )   /* BADクラスタを検出済みの場合    */
			{
				/* 現在読込まれているFATセクタを書込む */
				l_rtn = FMR_setFatSectorRetry(  /* FATセクタを書込む          */
					drvnum,             /* ドライブ番号                       */
				    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				
				/* BADクラスタを未検出とする */
				l_badflg = DFMR_FLGOFF;
			}
			/* FATセクタを読込む */
			l_rtn = FMR_getFatSectorRetry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_nowclst,              /* クラスタ番号                       */
				&l_fatentry             /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		
		/* FATエントリをクラスタ番号とする */
		l_nowclst = l_fatentry;
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
