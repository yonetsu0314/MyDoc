/*======================================================================
 * File name    : gempclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/gempclst.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gempclst.c $
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
/* 関 数 名：FMR_getEmptyCluster                                              */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_DWORD 検索開始クラスタ番号                                */
/*         ：     F_BYTE FAT読込み領域番号                                    */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：F_DWORD 正常終了 クラスタ番号                                    */
/*         ：        異常終了 DFMR_LNG                                        */
/*         ：                                                                 */
/* 概    要：空きクラスタ番号を取得する。                                     */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
#include "fsbase.h"

F_DWORD FMR_getEmptyCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD clstnum,                    /* 検索開始クラスタ番号               */
    F_BYTE fatarea                      /* 検索FAT読込み領域番号              */
)
{
	F_DWORD l_fatcnt;                   /* FATカウンタ                        */
	F_DWORD l_getclstnum;               /* 取得した空きクラスタ番号           */
	F_DWORD l_getfatentry;              /* FATエントリ                        */
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_work04;

	F_INT l_drtn;                       /* 戻り値                             */
	F_INT l_rtn;                        /* 戻り値                             */
	
	
	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_fatcnt = clstnum;                 /* FATカウンタ←検索開始クラスタ番号  */
	
	/* 検索FAT読込み領域に読込まれているFATセクタの番号取得 */
	l_work01 = MFMR_FATSECTNUM( drvnum, fatarea );
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
		l_rtn = FMR_restoreFATInfo(
			drvnum,             /* 対象論理ドライブ番号               */
			l_work03            /* 開始セクタオフセット               */
			);
		if( l_rtn != DFMR_OK )
		{
			return DFMR_LNG;
		}
		l_work02 = MFMR_FATSECTTOP( drvnum );
	}
	/* FATセクタ内の最小空きクラスタ番号を取得 */
	l_work03 = l_work01 - l_work02;
	l_work04 = MFMR_FATEMPTYMIN( drvnum, l_work03 );
	l_work01 = MFMR_FATEMPTYNUM( drvnum, l_work03 );
	l_fatcnt = l_work04;                 /* FATカウンタ←最小空きクラスタ番号  */
	/**************************************************************************/
	/* 検索前に可能な判定を行う                                               */
	/**************************************************************************/
	if( l_work01 == 0 )
	{
		/* 対象セクタの空きクラスタ番号が0の場合 */
		return DFMR_LNG;
	}
	
	/**************************************************************************/
	/* 空きクラスタ検索を行う                                                 */
	/**************************************************************************/
	if( MFMR_DRVINFO( drvnum )->fattype == DFMR_FATTYPE12 )
	{
		/**********************************************************************/
		/* FAT12の場合                                                        */
		/**********************************************************************/
		/* FATエントリを取得する */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, fatarea ),   /* FAT読込み領域              */
			l_fatcnt,                   /* クラスタ番号                       */
			&l_getfatentry,             /* FATエントリ値                      */
			DFMR_FLGOFF                 /* Read/Write種別 OFF:取得            */
			);
		if( l_drtn == DFMR_OK )
		{
		}
		else if( l_drtn == DFMR_NOFATENTRY )
		{
			l_drtn = FMR_getFatSector(
				MFMR_DRVINFO( drvnum ),   /* 論理ドライブ構成情報           */
				MFMR_FATTABLE( drvnum, fatarea ), /* FAT読込み領域          */
				l_fatcnt,                 /* クラスタ番号                   */
				&l_getfatentry            /* FATエントリ値                  */
				);
			if( l_drtn != DFMR_OK )
			{
				return DFMR_LNG;
			}
		}
		else
		{
			return DFMR_LNG;
		}
	}

	if ( l_work04 >= l_fatcnt )
	{
		/* 検索開始クラスタ番号が最小空クラスタ番号以下の場合 */
		l_getclstnum = l_work04;
		if ( l_work01 == 1 )
		{
			/* 空きクラスタ数が1の場合 */
			MFMR_FATEMPTYMIN( drvnum, l_work03 ) = DFMR_NOEMPTYCLST;
			MFMR_FATEMPTYNUM( drvnum, l_work03 ) =0;
			return l_getclstnum;
		}
		else
		{
			if ( l_work04 == l_fatcnt )
			{
				l_fatcnt ++;
			}
			while( 1 )
			{
				/* FATエントリを取得する */
				l_drtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, fatarea ),   /* FAT読込み領域              */
					l_fatcnt,                   /* クラスタ番号                       */
					&l_getfatentry,             /* FATエントリ値                      */
					DFMR_FLGOFF                 /* Read/Write種別 OFF:取得            */
					);
				
				if( l_drtn != DFMR_OK )         /* エラーの場合は｢空き領域なし｣とする */
				{
					return DFMR_LNG;
				}
				
				if( l_getfatentry == DFMR_NEWCLUS )
				{
					/* 取得したFATエントリが空の場合 */
					MFMR_FATEMPTYMIN( drvnum, l_work03 ) = l_fatcnt;
					MFMR_FATEMPTYNUM( drvnum, l_work03 ) --;
					return l_getclstnum;
				}
				l_fatcnt ++;
				if ( l_work04 == l_fatcnt )
				{
					l_fatcnt ++;
				}
			}
			/* 次の空クラスタが見つからない場合 */
			MFMR_FATEMPTYMIN( drvnum, l_work03 ) = DFMR_NOEMPTYCLST;
			MFMR_FATEMPTYNUM( drvnum, l_work03 ) =0;
			return l_getclstnum;
		}
	}
	else
	{
		/* 検索開始クラスタ番号が最小空クラスタ番号以上の場合 */
		if ( l_work01 == 1 )
		{
			/* 空きクラスタ数が1の場合 */
			return DFMR_LNG;
		}
		else
		{
			while( 1 )
			{
				/* FATエントリを取得する */
				l_drtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, fatarea ),   /* FAT読込み領域              */
					l_fatcnt,                   /* クラスタ番号                       */
					&l_getfatentry,             /* FATエントリ値                      */
					DFMR_FLGOFF                 /* Read/Write種別 OFF:取得            */
					);
				
				if( l_drtn != DFMR_OK )         /* エラーの場合は｢空き領域なし｣とする */
				{
					return DFMR_LNG;
				}
				
				if( l_getfatentry == DFMR_NEWCLUS )
				{
					/* 取得したFATエントリが空の場合 */
					l_getclstnum = l_fatcnt;
					MFMR_FATEMPTYNUM( drvnum, l_work03 ) --;
					return l_getclstnum;
				}
				l_fatcnt ++;
			}
			/* 次の空クラスタが見つからない場合 */
			return DFMR_LNG;
		}
	}
}

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
