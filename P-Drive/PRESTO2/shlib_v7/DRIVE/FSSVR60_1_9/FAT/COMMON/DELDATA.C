/*======================================================================
 * File name    : deldata.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/deldata.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: deldata.c $
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
/* 関 数 名：FMR_deleteDataCluster                                            */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：     F_DWORD 終了クラスタ番号                                    */
/*         ：         (終了クラスタ番号=0の場合、EOCクラスタまで未使用にする) */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：データ部クラスタを未使用にする                                   */
/*         ：(FAT操作は全てFAT読込み領域0で行う                               */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
#include "fsbase.h"

F_INT FMR_deleteDataCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
    F_DWORD endclstnum                  /* 終了クラスタ番号                   */
)
{
	F_DWORD l_targetclstnum;            /* 対象クラスタ番号                   */
	F_DWORD l_eoc;                      /* EOCコード                          */
	F_DWORD l_tbd;                      /* FATエントリ:未使用                 */
	F_DWORD l_nextclstnum;              /* 次のクラスタ番号(対象FATエントリ)  */
	F_DWORD l_work01;
	F_DWORD l_work02;
	F_DWORD l_work03;

	F_INT l_drtn;                       /* サブルーチン戻り値                 */

	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_targetclstnum = topclstnum;       /* 対象クラスタ番号←先頭クラスタ番号 */
	l_tbd = DFMR_NEWCLUS;               /* FATエントリ｢未使用｣コード設定      */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ),l_eoc );    /* EOC設定                */
	MFMR_FATSECTNUM(drvnum, DFMR_REFFATAREA1) = 0;
	
	/**************************************************************************/
	/* FAT読込み領域0に対象クラスタ番号を含むFATセクタを読込む                */
	/**************************************************************************/
	l_drtn = FMR_getFatSector(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ), /* FAT読込み領域          */
		l_targetclstnum,                /* クラスタ番号                       */
		&l_nextclstnum                  /* FATエントリ値                      */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	
	/**************************************************************************/
	/* 対象クラスタのFATエントリを｢未使用｣とする                              */
	/**************************************************************************/
	while( 1 )
	{
		/* 対象クラスタのFATエントリを｢未使用｣に設定 */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ), /* FAT読込み領域      */
			l_targetclstnum,            /* クラスタ番号                       */
			&l_tbd,                     /* FATエントリの値:未使用             */
			DFMR_FLGON                  /* Read/Write種別 :設定               */
			);
			
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		/******************************************************************/
		/* FAT読込み領域.FATセクタ番号よりFATセクタ管理情報の位置を取得し */
		/* 空クラスタ数、空クラスタ番号を更新                             */
		/******************************************************************/
		/* FAT読込み領域に読込まれているFATセクタの番号取得 */
		l_work01 = MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 );
		l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
												/* 前セクタ読込み・更新フラグOFF */
		/* FATセクタ管理情報の先頭セクタ番号を取得 */
		l_work02 = MFMR_FATSECTTOP( drvnum );
		
		if( ( l_work01 < l_work02 )
		|| ( ( l_work02 + DFMR_FATMNGSIZE ) < l_work01 ) )
		{
			/* FATセクタ管理領域に読込まれているFATセクタ情報が */
			/* 検索対象FAT読込み領域のFATを含んでいない場合 */
			l_work03 = l_work01 - MFMR_FATTOP( drvnum );
			l_work03 /= DFMR_FATMNGSIZE;    /* FATセクタ管理情報の        */
			l_work03 *= DFMR_FATMNGSIZE;    /* 先頭セクタを求める         */
			
			/* FATセクタ管理情報を再構築 */
			l_drtn = FMR_restoreFATInfo(
				drvnum,           /* 対象論理ドライブ番号               */
				l_work03            /* 開始セクタオフセット               */
				);
			if( l_drtn != DFMR_OK )
			{
				return DFMR_LNG;
			}
			l_work02 = MFMR_FATSECTTOP( drvnum );
		}
		l_work03 = l_work01 - l_work02;

		/* 最小空きクラスタ番号設定 */
		if (( MFMR_FATEMPTYMIN( drvnum, l_work03 ) == DFMR_NOEMPTYCLST )
			|| (MFMR_FATEMPTYMIN( drvnum, l_work03 ) > l_targetclstnum ))
		{
			MFMR_FATEMPTYMIN( drvnum, l_work03 ) = l_targetclstnum;
		}
		/* FATセクタ内の空きクラスタ数をインクリメント */
		MFMR_FATEMPTYNUM( drvnum, l_work03 ) ++;
		
		if( (l_nextclstnum >= l_eoc) ||
			((endclstnum != NULL) &&
			 (l_nextclstnum == endclstnum)))
		{
			/* 対象クラスタのFATエントリが｢EOC｣または、終了クラスタ以上の場合は終了 */
			/* 対象FATセクタを書込む */
			l_drtn = FMR_setFatSector(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				DFMR_REFFATAREA0        /* FAT読込み領域番号                  */
				);
			
			return l_drtn;              /* 戻り値をそのまま返却               */
		}
		l_targetclstnum = l_nextclstnum;/* 取得して置いたFATエントリを        */
		                                /* 対象エントリとする                 */
		
		/* 対象エントリのFATエントリを取得する */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ), /* FAT読込み領域      */
			l_targetclstnum,            /* クラスタ番号                       */
			&l_nextclstnum,             /* FATエントリの値:EOC                */
			DFMR_FLGOFF                 /* Read/Write種別 :取得               */
			);
		if( l_drtn != DFMR_OK )         /* 取得失敗時はエラー内容により分岐   */
		{
			if( l_drtn == DFMR_NOFATENTRY )
			{                           /* FATエントリなしの場合              */
				/* 対象FATセクタを書込む */
				l_drtn = FMR_setFatSector(
					MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報           */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                        /* FAT読込み領域                  */
					DFMR_REFFATAREA0        /* FAT読込み領域番号              */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				
				/* 対象クラスタ番号を含むFATセクタをFAT読込み領域0に読込む */
				l_drtn = FMR_getFatSector(
					MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報        */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                        /* FAT読込み領域               */
					l_targetclstnum,        /* クラスタ番号                */
					&l_nextclstnum          /* FATエントリ値               */
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
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

