/*======================================================================
 * File name    : fatinfo.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/fatinfo.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fatinfo.c $
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
/* 関 数 名：FMR_restoreFATInfo                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_DWORD 開始セクタオフセット                                */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：FATセクタ管理情報を構築する                                      */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_restoreFATInfo(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topsectofst                 /* 開始セクタオフセット               */
)
{
	F_WORD l_maxfatsectnum;             /* 最大FAT読込み数                    */
	F_DWORD l_minclstnum;               /* FATセクタ内最小クラスタ番号        */
	F_DWORD l_maxclstnum;               /* FATセクタ内最大クラスタ番号        */
	F_DWORD l_fatentry;                 /* 取得FATエントリ                    */
	F_DWORD l_emptyclstcnt;             /* 空きクラスタ数                     */
	F_DWORD l_leastemptynum;            /* 最小空きクラスタ番号               */
	
	F_INT l_drtn;                       /* サブルーチン戻り値                 */
	
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_DWORD l_cnt02;
	F_DWORD l_work01;                   /* ワークエリア                       */
	
	/**************************************************************************/
	/* 初期パラメータ設定                                                     */
	/**************************************************************************/
	/* 最大FAT読込み数を求める */
	if( ( MFMR_SECTPERFAT( drvnum ) - topsectofst ) >= DFMR_FATMNGSIZE )
	{                                   /* オフセット位置から後ろに管理領域   */
	                                    /* 最大数以上のセクタが存在する場合   */
		l_maxfatsectnum = DFMR_FATMNGSIZE;
		                                /* 最大FAT読込み数は管理領域最大数    */
	}
	else
	{
		l_maxfatsectnum = ( F_WORD )( MFMR_SECTPERFAT( drvnum ) - topsectofst );
		                                /* 最大読込み数はオフセット以降全て   */
	}
	MFMR_FATSECTTOP( drvnum ) = MFMR_FATTOP( drvnum ) + topsectofst;
	                                    /* 開始FATセクタ番号設定              */
	/**************************************************************************/
	/* 最小空きクラスタ/空きクラスタ数取得                                    */
	/**************************************************************************/
	/* 対象のFATの数分ループ */
	for( l_cnt01 = 0; l_cnt01 < l_maxfatsectnum; l_cnt01 ++ )
	{
		l_emptyclstcnt = 0;                     /* 空きクラスタ数初期化       */
		l_leastemptynum = DFMR_NOEMPTYCLST;     /* 最小空きクラスタ番号初期化 */

		/* 対象FATセクタ内の最小/最大クラスタ番号を求める */
		l_work01 = MFMR_BYTEPERSECT( drvnum ) / sizeof( F_DWORD );
		l_minclstnum = ( topsectofst + l_cnt01 ) * l_work01;
		                            /* FATセクタ内最小クラスタ番号        */
		l_maxclstnum = l_minclstnum + l_work01 - 1;
		                            /* FATセクタ内最大クラスタ番号        */
		if( l_maxclstnum > MFMR_TOTALCLST( drvnum ) )
		{                               /* FATセクタ内最大クラスタ番号が      */
		                                /* クラスタ数を超えた場合             */
			l_maxclstnum = MFMR_TOTALCLST( drvnum );
		}                               /* 最大クラスタ番号←クラスタ数       */
		
		/* 対象FATセクタを読込み領域1に読込む */
		l_drtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
			                            /* FAT読込み領域アドレス              */
			l_minclstnum,               /* クラスタ番号                       */
			&l_fatentry                 /* FATエントリ値                      */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		if( l_fatentry == 0 )           /* FATエントリが｢未使用｣の場合        */
		{
			l_emptyclstcnt ++;                  /* 空きクラスタ数更新         */
			l_leastemptynum = l_minclstnum;     /* 最小空きクラスタ番号に設定 */
		}
		/* FATセクタ内2番目～最終までのクラスタ番号のFATエントリ取得、判定 */
		for( l_cnt02 = 1; l_cnt02 <= ( l_maxclstnum - l_minclstnum ); l_cnt02 ++ )
		{
			/* FATエントリ取得 */
			l_work01 = l_minclstnum + l_cnt02;
			l_drtn = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報アドレス       */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
				                        /* FAT読込み領域アドレス              */
				l_work01,               /* クラスタ番号                       */
				&l_fatentry,            /* FATエントリ値                      */
				DFMR_FLGOFF             /* R/W種別：Read                      */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
			if( l_fatentry == 0 )       /* FATエントリが｢未使用｣の場合        */
			{
				l_emptyclstcnt ++;      /* 空きクラスタ数更新                 */
				if( l_leastemptynum == DFMR_NOEMPTYCLST )
				{                       /* 最小空きクラスタをまだ取得して     */
				                        /* いない場合                         */
					l_leastemptynum = l_work01; /* 最小空きクラスタ番号に設定 */
				}
			}
		}
		/* FAT管理テーブル更新 */
		MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) = l_emptyclstcnt;
		MFMR_FATEMPTYMIN( drvnum, l_cnt01 ) = l_leastemptynum;
	}
	
	/* 残りの部分を初期化 */
	for( ; l_cnt01 < DFMR_FATMNGSIZE; l_cnt01 ++ )
	{
		MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) = 0;/* 空きクラスタ数初期化       */
		MFMR_FATEMPTYMIN( drvnum, l_cnt01 ) = DFMR_NOEMPTYCLST;
		                                        /* 最小空きクラスタ番号初期化 */
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

