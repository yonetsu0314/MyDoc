/*======================================================================
 * File name    : skpsrpos.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/skpsrpos.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: skpsrpos.c $
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
/* 関 数 名：FMR_skipSearchPosition                                          */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 論理ドライブ番号                                     */
/*         ：     F_BYTE FAT読込み領域番号                                    */
/*         ：     TFMR_SEARCHINFO * 検索セクタ位置                            */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_NOENTRY                                      */
/*         ：                                                                 */
/* 概    要：エントリ検索位置を1件進める。                                    */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_skipSearchPosition(
    F_CHAR drvnum,                      /* 論理ドライブ番号                   */
    F_BYTE fatbuffnum,                  /* FAT読込み領域番号                  */
    TFMR_SEARCHINFO * searchinfo        /* 検索セクタ位置                     */
)
{
	F_DWORD l_sectnum;                  /* 対象セクタ番号                     */
	F_BYTE l_slot;                      /* 対象セクタ内位置                   */
	
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_eoc;                      /* EOCコード                          */
	F_INT l_drtn;                       /* サブルーチン戻り値                 */
	
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */
	
	
	MFMR_GETEOCMIN( MFMR_FATTYPE( drvnum ), l_eoc );    /* EOC取得            */
	
	/* パラメータ設定 */
	l_modspc = MFMR_SECTPERCLST( drvnum ) - 1;
	l_sectnum = searchinfo -> sectnum;  /* セクタ番号を取得                   */
	l_slot = searchinfo -> slot;        /* セクタ内位置を取得                 */
	
	if( l_slot != ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY ) ) )
	{                                   /* セクタ内位置がセクタ終端ではない   */
		l_slot ++;                      /* セクタ内位置を一つ進める           */
	}
	else
	{
		if( l_sectnum < MFMR_DATATOP( drvnum ) )
		{                               /* セクタ番号がRoot Dir. Region       */
			l_slot = 0;                 /* 一つ先のセクタに位置を勧める       */
			l_sectnum ++;
			if( l_sectnum == MFMR_DATATOP( drvnum ) )
			{                           /* 一つ先がData Region                */
				return DFMR_NOENTRY;    /* 対象エントリなし                   */
			}
		}
		else                            /* セクタ番号がData Region            */
		{
			if( ( ( l_sectnum - MFMR_DATATOP( drvnum ) ) & l_modspc ) == l_modspc )
			{                           /* 対象セクタがクラスタ終端のセクタ   */
				/* 対象セクタが属するクラスタ番号を求める */
				l_work01 = l_sectnum - MFMR_DATATOP( drvnum );
				l_work01 /= MFMR_SECTPERCLST( drvnum );
				l_work01 += 2;
				
				/* 求めたクラスタ番号のFATエントリを求める */
				l_drtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報       */
					MFMR_FATTABLE( drvnum, fatbuffnum ),
					                            /* FAT読込み領域              */
					l_work01,                   /* クラスタ番号               */
					&l_work02,                  /* FATエントリ値              */
					DFMR_FLGOFF                 /* Read/Write種別 OFF:取得    */
					);
				if( l_drtn != DFMR_OK )
				{
					if( l_drtn == DFMR_NOFATENTRY )
					{                           /* FATエントリなしの場合      */
						/* 取得したクラスタ番号を含むFATセクタを読込む */
						l_drtn = FMR_getFatSector(
							MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報   */
							MFMR_FATTABLE( drvnum, fatbuffnum ),  
													/* FAT読込み領域          */
							l_work01,               /* クラスタ番号           */
							&l_work02               /* FATエントリ値          */
							);
						if( l_drtn != DFMR_OK )
						{
							return l_drtn;
						}
					}
					else
					{
						return l_drtn;
					}
				}
				
				/* FATエントリの示すクラスタの先頭セクタを求める */
				if( l_work02 >= l_eoc ) /* 求めたFATエントリがEOC             */
				{
					return DFMR_NOENTRY;/* 対象エントリなし                   */
				}
				l_slot = 0;             /* 一つ先のセクタに位置を勧める       */
				l_sectnum = l_work02 - DFMR_TOPCLSTNUM;
				l_sectnum *= MFMR_SECTPERCLST( drvnum );
				l_sectnum += MFMR_DATATOP( drvnum );
			}
			else                        /* 対象セクタがクラスタ終端以外       */
			{
				l_slot = 0;             /* 一つ先のセクタに位置を勧める       */
				l_sectnum ++;
			}
		}
	}
	/* パラメータ設定 */
	searchinfo -> sectnum = l_sectnum;  /* セクタ番号を取得                   */
	searchinfo -> slot = l_slot;        /* セクタ内位置を取得                 */
	
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
