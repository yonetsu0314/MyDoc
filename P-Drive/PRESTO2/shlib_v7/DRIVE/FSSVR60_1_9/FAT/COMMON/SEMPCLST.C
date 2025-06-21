/*======================================================================
 * File name    : sempclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/sempclst.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sempclst.c $
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
/* 関 数 名：FMR_searchEmptyCluster                                           */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：                                                                 */
/*         ：OUT: F_DWORD * 取得空きクラスタ番号                              */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_MEMFULL                                      */
/*         ：                                                                 */
/* 概    要：FATセクタ管理領域より空きクラスタを取得する                      */
/*         ：                                                                 */
/*         ：※FAT32の場合、読込んでいるFATセクタ管理領域内に空きクラスタが   */
/*         ：  存在しない場合、続きのFATセクタ管理領域を再構築して空き        */
/*         ：  クラスタを検索する                                             */
/*         ：                                                                 */
/* 作 成 日：2004.05.20                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

static F_INT FMR_searchFATSectArea (
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
	F_DWORD   areacnt,                  /* FATセクタ管理領域テーブル数        */
	F_DWORD * emptyclustnum             /* 取得空きクラスタ番号               */
);

F_INT FMR_searchEmptyCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD * emptyclustnum             /* 取得空きクラスタ番号               */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;                   /* ワークエリア                       */
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	
	/**************************************************************************/
	/* 空きクラスタを含むFATセクタをFATセクタ管理領域より検索する             */
	/**************************************************************************/
	if( ( MFMR_SECTPERFAT( drvnum ) - ( MFMR_FATSECTTOP( drvnum )
		- MFMR_FATTOP( drvnum ) ) ) >= DFMR_FATMNGSIZE )
	{
		l_work01 = DFMR_FATMNGSIZE;
	}
	else
	{
		l_work01 = MFMR_SECTPERFAT( drvnum );
		l_work01 -= ( MFMR_FATSECTTOP( drvnum ) - MFMR_FATTOP( drvnum ) ); 
	}
	l_rtn = FMR_searchFATSectArea (
		drvnum,                         /* 論理ドライブ番号                   */
		l_work01,                       /* FATセクタ管理領域テーブル数        */
		emptyclustnum                   /* 取得空きクラスタ番号               */
		);
	if( l_rtn == DFMR_OK )
	{
		return l_rtn;
	}
	/**************************************************************************/
	/* 空きクラスタを含むFATセクタが存在しない場合                            */
	/**************************************************************************/
	if( MFMR_FATTYPE( drvnum ) == DFMR_FATTYPE32 )
	{
		/**********************************************************************/
		/* FAT32の場合FATセクタ管理領域を更新して検索を続ける                 */
		/**********************************************************************/
		l_work01 = MFMR_FATSECTTOP( drvnum );
		l_work01 -= MFMR_FATTOP( drvnum );
		l_work02 = l_work01;					/* 元のFAT読込みセクタを保存  */
		l_work01 += DFMR_FATMNGSIZE;
		/**********************************************************************/
		/* 最終FATセクタの検索が終わるまで                                    */
		/**********************************************************************/
		for( l_cnt01 = l_work01; l_cnt01 < MFMR_SECTPERFAT( drvnum ); l_cnt01 += DFMR_FATMNGSIZE )
		{
			/******************************************************************/
			/* FATセクタ管理領域を再構築する                                  */
			/******************************************************************/
			l_rtn = FMR_restoreFATInfo(
				drvnum,                 /* 対象論理ドライブ番号               */
				l_cnt01                 /* 開始セクタオフセット               */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/******************************************************************/
			/* FATセクタ管理領域に展開したFATセクタ情報数を取得               */
			/******************************************************************/
			if( ( MFMR_SECTPERFAT( drvnum ) - ( MFMR_FATSECTTOP( drvnum ) 
				- MFMR_FATTOP( drvnum ) ) ) >= DFMR_FATMNGSIZE )          
			{
				/**************************************************************/
				/* 開始位置～FATセクタ終端がFATセクタ管理領域で管理できる     */
				/* セクタ数以上の場合                                         */
				/**************************************************************/
				/* 展開したFATセクタ管理情報数はFATセクタ管理領域最大数分     */
				l_work01 = DFMR_FATMNGSIZE;
			}
			else
			{
				/**************************************************************/
				/* 開始位置～FATセクタ終端がFATセクタ管理領域で管理できる     */
				/* セクタ数未満の場合                                         */
				/**************************************************************/
				/* 展開したFATセクタ管理情報数は｢開始位置～FATセクタ終端｣分   */
				l_work01 = MFMR_SECTPERFAT( drvnum );
				l_work01 -= ( MFMR_FATSECTTOP( drvnum ) - MFMR_FATTOP( drvnum ) ); 
			}
			
			/******************************************************************/
			/* FATセクタ管理領域内のセクタを全検索                            */
			/******************************************************************/
			l_rtn = FMR_searchFATSectArea ( 
				drvnum,                 /* 論理ドライブ番号                   */
				l_work01,               /* FATセクタ管理領域テーブル数        */
				emptyclustnum           /* 取得空きクラスタ番号               */
				);
			if( l_rtn == DFMR_OK )
			{
				return l_rtn;
			}
		}
		/**********************************************************************/
		/* 先頭FATセクタから元のFAT読込みセクタまで検索                       */
		/**********************************************************************/
		if ( l_work02 != 0 )
		{
			/* 元のFAT読込みセクタが先頭FATセクタでない場合 */
			for( l_cnt01 = 0; l_cnt01 < l_work02; l_cnt01 += DFMR_FATMNGSIZE )
			{
				/**************************************************************/
				/* FATセクタ管理領域を再構築する                              */
				/**************************************************************/
				l_rtn = FMR_restoreFATInfo(
					drvnum,                 /* 対象論理ドライブ番号           */
					l_cnt01                 /* 開始セクタオフセット           */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				l_work01 = DFMR_FATMNGSIZE;
				/**************************************************************/
				/* FATセクタ管理領域内のセクタを全検索                        */
				/**************************************************************/
				l_rtn = FMR_searchFATSectArea ( 
					drvnum,             /* 論理ドライブ番号                   */
					l_work01,           /* FATセクタ管理領域テーブル数        */
					emptyclustnum       /* 取得空きクラスタ番号               */
					);
				if( l_rtn == DFMR_OK )
				{
					return l_rtn;
				}
			}
		}
		/**********************************************************************/
		/* FAT32で先頭FATセクタから最終FATセクタまで検索した場合              */
		/**********************************************************************/
		return DFMR_MEMFULL;            /* メモリフル返却                     */
	}
	else
	{
		/**********************************************************************/
		/* FAT12/16の場合はメモリフル                                         */
		/**********************************************************************/
		return DFMR_MEMFULL;
	}
}

static F_INT FMR_searchFATSectArea ( 
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
	F_DWORD   areacnt,                  /* FATセクタ管理領域テーブル数        */
	F_DWORD * emptyclustnum             /* 取得空きクラスタ番号               */
)
{
	F_DWORD   l_work01;                 /* ワークエリア                       */
	F_DWORD   l_getfatentry;            /* FATエントリ                        */
	F_INT     l_rtn;                    /* サブルーチン戻り値                 */
	F_WORD    l_cnt01;

	for( l_cnt01 = 0; l_cnt01 < areacnt; l_cnt01 ++ )
	{
		if( MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) != 0 )
		{
			/******************************************************************/
			/* 空きクラスタを含むFATセクタが存在した場合                      */
			/******************************************************************/
			/* その最小空きクラスタ番号を取得する */
			*( emptyclustnum ) = MFMR_FATEMPTYMIN( drvnum, l_cnt01 );
			/* 最小空きクラスタ番号を取得したFATセクタを */
			/* FAT読込み領域1に読込む                    */
			l_rtn = FMR_getFatSector(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
										/* FAT読込み領域                      */
				*( emptyclustnum ),     /* クラスタ番号                       */
				&l_work01               /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			l_work01 = *( emptyclustnum ) + 1;
			if( MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) == 1 )
			{
				/* 空きクラスタが1の場合 */
				MFMR_FATEMPTYMIN( drvnum, l_cnt01 ) = DFMR_NOEMPTYCLST;
				                        /* 最小空きクラスタ番号に             */
				                        /* 空きクラスタなしを設定             */
				MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) =0;
			}
			else                        /* 空きクラスタが複数存在する場合     */
			{
				while( 1 )
				{
					/* 空きクラスタを検索する */
					l_rtn = FMR_controlFatEntry(    /* FATエントリを取得する  */
						MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報   */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
										/* FAT読込み領域                      */
						l_work01,       /* クラスタ番号                       */
						&l_getfatentry, /* FATエントリ値                      */
						DFMR_FLGOFF     /* Read/Write種別 OFF:取得            */
						);
					if( l_rtn != DFMR_OK )  /* エラーの場合は                 */
					{                       /* ｢空き領域なし｣とする           */
						MFMR_FATEMPTYMIN( drvnum, l_cnt01 ) = DFMR_NOEMPTYCLST;
						MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) =0;
						break;
					}
					if( l_getfatentry == DFMR_NEWCLUS )
					{
						/* 空きクラスタ発見時 */
						/* それを最小空きクラスタ番号とする */
						MFMR_FATEMPTYMIN( drvnum, l_cnt01 ) = l_work01;
						/* FATセクタ内の空きクラスタ数をデクリメント */
						MFMR_FATEMPTYNUM( drvnum, l_cnt01 ) --;
						break;
					}
					l_work01 ++;
				}
			}
			return DFMR_OK;
		}
	}
	return DFMR_MEMFULL;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
