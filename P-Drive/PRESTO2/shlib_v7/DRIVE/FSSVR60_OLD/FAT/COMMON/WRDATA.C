/*======================================================================
 * File name    : wrdata.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/wrdata.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: wrdata.c $
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
/* 関 数 名：FMR_writeDataSectors                                             */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_DWORD 先頭セクタ番号                                      */
/*         ：     F_BYTE * 書込みデータ格納領域                               */
/*         ：     F_DWORD 書込みセクタ数                                      */
/*         ：     F_DWORD 検索開始クラスタ番号                                */
/*         ：     F_BYTE  FAT読込み領域番号                                   */
/*         ：                                                                 */
/*         ：OUT: F_DWORD * 新規取得先頭クラスタ番号                          */
/*         ：     F_DWORD * 新規取得最終クラスタ番号                          */
/*         ：     F_BYTE * 最終FAT読込み領域番号                              */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_FATALERR                                     */
/*         ：                                                                 */
/* 概    要：新規セクタ書込み処理                                             */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
/*** MEMO *********************************************************************/
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_writeDataSectors(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topsectnum,                 /* 先頭セクタ番号                     */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD targetsectnum,              /* 書込みセクタ数                     */
    F_DWORD startclstnum,               /* 検索開始クラスタ番号               */
    F_BYTE  fatarea,                    /* FAT読込み領域番号                  */
    F_DWORD * newtopclstnum,            /* 新規取得先頭クラスタ番号           */
    F_DWORD * newbottomclstnum,         /* 新規取得最終クラスタ番号           */
    F_BYTE * lastfatarea                /* 最終FAT読込み領域番号              */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */
	F_DWORD l_startsectnum;             /* 書込み開始セクタ番号               */
	F_DWORD l_restsectnum;              /* 残りセクタ数                       */
	F_DWORD l_seqwritenum;              /* 連続書込みセクタ数                 */
	F_DWORD l_nowclstnum;               /* 現在取得中のクラスタ番号           */
	F_DWORD l_nextclstnum;              /* 次に書込みを開始するクラスタ番号   */
	
	F_BYTE * l_startptr;                /* 書込み開始位置ポインタ             */
	
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/F                        */
	
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	
	F_WORD l_targetsctnum;              /* 対象セクタ数                       */
	
	F_BYTE l_fatareanum;                /* 現在参照中のFAT読込み領域番号      */
	
	/**************************************************************************/
	/* パラメータの初期設定を行う                                             */
	/**************************************************************************/
	l_modspc = MFMR_SECTPERCLST( drvnum ) - 1;
	l_fatareanum = fatarea;             /* 現在参照中のFAT読込み領域設定      */
	l_restsectnum = targetsectnum;      /* 残りセクタ数設定                   */
	l_startptr = databuff;              /* 書込み開始位置取得                 */
	*( newtopclstnum ) = 0;             /* 新規取得最終クラスタ番号初期化     */
	
	if( topsectnum == 0 )
	{
		/**********************************************************************/
		/* 書込み開始セクタが未定の場合                                       */
		/**********************************************************************/
		/* FAT読込み領域より空きクラスタを取得する */
		l_nowclstnum = FMR_getEmptyCluster(
			drvnum,                     /* 対象論理ドライブ番号               */
			startclstnum,               /* 検索開始クラスタ番号               */
			l_fatareanum                /* 検索FAT読込み領域                  */
			);
		if( l_nowclstnum == DFMR_LNG )
		{
			/******************************************************************/
			/* FAT読込み領域に空きクラスタが存在しない場合                   */
			/******************************************************************/
			/* アクセス対象をFAT読込み領域1に変更 */
			l_fatareanum = DFMR_REFFATAREA1;
			
			/* FATセクタ管理領域から空きクラスタを取得する */
			l_rtn = FMR_searchEmptyCluster(
				drvnum,                 /* 対象論理ドライブ番号               */
				&l_nowclstnum           /* 取得空きクラスタ番号               */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		*( newtopclstnum ) = l_nowclstnum;  /* 新規取得最終クラスタ番号に取得した */
		                                	/* 空きクラスタ番号を設定             */
		l_nextclstnum = l_nowclstnum;		/* 取得したクラスタ←先頭セクタを書込み開始セクタ */
		l_startsectnum = l_nowclstnum - 2;              
		l_startsectnum *= MFMR_SECTPERCLST( drvnum );
		l_startsectnum += MFMR_DATATOP( drvnum );   /* 開始セクタ設定         */
		
		if( l_restsectnum >= MFMR_SECTPERCLST( drvnum ) )
		{                               /* 残りセクタがクラスタ当りの         */
		                                /* セクタ数以上の場合                 */
			l_seqwritenum = MFMR_SECTPERCLST( drvnum );
			                            /* 連続書込みセクタ数設定             */
		}
		else                            /* 残りセクタ数がクラスタ当りの       */
		{                               /* セクタ数未満の場合                 */
			l_seqwritenum = l_restsectnum;      /* 連続書込みセクタ数は       */
		}                                       /* クラスタ当りのセクタ数     */
	}
	else
	{
		/**********************************************************************/
		/* 書込み開始セクタが呼出元で取得済みの場合                             */
		/**********************************************************************/
		l_startsectnum = topsectnum;
		l_work01 = l_startsectnum - MFMR_DATATOP( drvnum ); 
		l_nowclstnum = l_work01 / MFMR_SECTPERCLST( drvnum );
		l_nowclstnum += 2;              /* 現在取得中のクラスタ番号を取得     */
		l_nextclstnum = l_nowclstnum;
		l_work01 &= l_modspc;                   /* 書込み開始セクタの         */
		                                        /* クラスタ内オフセット       */
		*( newtopclstnum ) = 0;         /* 新規取得最終クラスタ番号初期化     */
		if( l_restsectnum >= ( MFMR_SECTPERCLST( drvnum ) - l_work01 ) )
		{                               /* 残りセクタが開始セクタを含む       */
		                                /* クラスタ内の残りセクタ以上の場合   */
			l_seqwritenum = ( MFMR_SECTPERCLST( drvnum ) - l_work01 );
			                            /* 連続書込みセクタ数設定             */
		}
		else                            /* 残りセクタ数がクラスタ当りの       */
		{                               /* セクタ数未満の場合                 */
			l_seqwritenum = l_restsectnum;      /* 連続書込みセクタ数は       */
		}                                       /* クラスタ当りのセクタ数     */
	}

	/**************************************************************************/
	/* パラメータの初期設定を行う                                             */
	/**************************************************************************/
	while( 1 )                          /* 書込み完了 or エラー発生で処理終了 */
	{
		/**********************************************************************/
		/* 連続書込みを行うセクタを取得する                                   */
		/**********************************************************************/
		while( l_seqwritenum < l_restsectnum )
		{ 
			/* 現在取得中のクラスタ番号を検索開始クラスタ番号として */
			/* 空きクラスタを現在参照中のFAT読込み領域より取得する  */
			l_nextclstnum = FMR_getEmptyCluster(
				drvnum,                 /* 対象論理ドライブ番号               */
				( l_nowclstnum + 1 ),   /* 検索開始クラスタ番号               */
				l_fatareanum            /* 検索FAT読込み領域                  */
				);
			if( l_nextclstnum == DFMR_LNG )
			{
				/**************************************************************/
				/* FAT読込み領域内で空きクラスタが存在しなかった場合          */
				/**************************************************************/
				if( l_fatareanum != DFMR_REFFATAREA0 )
				{
					/**********************************************************/
					/* アクセス対象のFAT読込み領域が0以外の場合はFATを書込む  */
					/**********************************************************/
					l_drtn = FMR_setFatSector(
						MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, l_fatareanum ),
						                /* FAT読込み領域アドレス              */
						l_fatareanum    /* FAT読込み領域番号                  */
						);
					if( l_drtn != DFMR_OK )
					{
						return l_drtn;
					}
				}
				/* FATセクタ管理領域から空きクラスタを取得する */
				l_rtn = FMR_searchEmptyCluster(
					drvnum,             /* 対象論理ドライブ番号               */
					&l_nextclstnum      /* 取得空きクラスタ番号               */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}

				/**************************************************************/
				/* 現在取得中のクラスタ番号(l_nowclstnum)のFATエントリに      */
				/* 上のFMR_searchEmptyClusterで取得したクラスタ番号           */
				/* (l_nextclstnum)を設定する                                  */
				/**************************************************************/
				l_drtn = FMR_linkNewCluster(
					l_nowclstnum,       /* リンク対象クラスタ番号             */
					&l_nextclstnum,     /* FATエントリ設定値                  */
					&l_fatareanum,      /* 現在参照中のFAT読込み領域番号      */
					drvnum              /* 対象ドライブ番号                   */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				if( *( newtopclstnum ) == 0 )
				{
					/**********************************************************/
					/* 新規取得先頭クラスタ番号がまだ未設定の場合             */
					/**********************************************************/
					/* 取得した空きクラスタ番号を新規取得クラスタ番号に設定 */
					*( newtopclstnum ) = l_nextclstnum;
				}
				break;
			}
			else if( l_nextclstnum != ( l_nowclstnum + 1 ) )
			{
				/**************************************************************/
				/* 空きクラスタが現在取得中のクラスタと連続でなかった場合     */
				/**************************************************************/
				if( *( newtopclstnum ) == 0 )
				{
					/**********************************************************/
					/* 新規取得先頭クラスタ番号がまだ未設定の場合             */
					/**********************************************************/
					/* 取得した空きクラスタ番号を新規取得クラスタ番号に設定 */
					*( newtopclstnum ) = l_nextclstnum;
				}
				/**************************************************************/
				/* 現在取得中のクラスタ番号(l_nowclstnum)のFATエントリに      */
				/* 上のFMR_getEmptyClusterで取得したクラスタ番号              */
				/* (l_nextclstnum)を設定する                                  */
				/**************************************************************/
				l_drtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報アドレス   */
					MFMR_FATTABLE( drvnum, l_fatareanum ),
					                        /* FAT読込み領域アドレス          */
					l_nowclstnum,           /* クラスタ番号                   */
					&l_nextclstnum,         /* FATエントリ値                  */
					DFMR_FLGON              /* Read/Write種別:Write           */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				break;
			}
			else
			{
				/**************************************************************/
				/* 空きクラスタが現在取得中のクラスタと連続だった場合         */
				/**************************************************************/
				if( *( newtopclstnum ) == 0 )
				{
					/**********************************************************/
					/* 新規取得先頭クラスタ番号がまだ未設定の場合             */
					/**********************************************************/
					/* 取得した空きクラスタ番号を新規取得クラスタ番号に設定   */
					*( newtopclstnum ) = l_nextclstnum;
				}
				/**************************************************************/
				/* 現在取得中のクラスタ番号(l_nowclstnum)のFATエントリに      */
				/* 上のFMR_getEmptyClusterで取得したクラスタ番号              */
				/* (l_nextclstnum)を設定する                                  */
				/**************************************************************/
				l_drtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報アドレス   */
					MFMR_FATTABLE( drvnum, l_fatareanum ),
					                        /* FAT読込み領域アドレス          */
					l_nowclstnum,           /* クラスタ番号                   */
					&l_nextclstnum,         /* FATエントリ値                  */
					DFMR_FLGON              /* Read/Write種別:Write           */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				if( l_restsectnum > ( MFMR_SECTPERCLST( drvnum ) + l_seqwritenum ) )
				{
					/**********************************************************/
					/* 必要なセクタ数がクラスタあたりのセクタ数以上の場合     */
					/**********************************************************/
					/* 連続書込みセクタ数にクラスタ当りのセクタ数を加算 */
					l_seqwritenum += MFMR_SECTPERCLST( drvnum );
					/* たった今取得したクラスタ番号を */
					/* 現在取得中のクラスタ番号とする */
					l_nowclstnum = l_nextclstnum;
				}
				else
				{
					/**********************************************************/
					/* 必要なセクタ数がクラスタ当りのセクタ数未満の場合       */
					/**********************************************************/
					/* 残りセクタ数→連続書込みセクタ数 */
					l_seqwritenum = l_restsectnum;
					break;
				}
			}
		}
		/**********************************************************************/
		/* 取得したセクタ群にデータを書込む                                   */
		/**********************************************************************/
		/* 書込み処理パラメータ設定 */
		l_drvioparam.write.startsctnum = l_startsectnum;
		                                /* 書込み開始セクタ番号               */
		l_targetsctnum = ( F_WORD )l_seqwritenum; /* 書込みセクタ数                     */
		l_drvioparam.write.writesctnum = &l_targetsctnum;
		l_drvioparam.write.writebuff = l_startptr;
		if( ( l_drvioparam.write.startsctnum == 0 )
			|| ( l_drvioparam.write.startsctnum < MFMR_DATATOP( drvnum ) ) )
		{
			return DFMR_FATALERR;
		}
		
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル番号/ユニット番号          */
			DFMR_DRVFUNCWRITE,          /* I/O Func.番号 = Write              */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		if( l_targetsctnum != ( F_WORD )l_seqwritenum ) /* 書込みセクタ数が取得した   */
		{                                       /* セクタと異なる場合         */
			return DFMR_FATALERR;
		}
		l_restsectnum -= l_seqwritenum; /* 残りセクタ数から連続書き込み       */
		                                /* セクタ数分差引く                   */
		
		if( l_restsectnum == 0 )
		{
			/******************************************************************/
			/* 書込み完了の場合                                               */
			/******************************************************************/
			*( lastfatarea ) = l_fatareanum;
			                            /* 最終FAT読込み領域を設定            */
			*( newbottomclstnum ) = l_nextclstnum;
			                            /* 最後に取得したクラスタが新規取得   */
			                            /* 最終クラスタ番号                   */
			return DFMR_OK;             /* 処理終了                           */
		}
		/**********************************************************************/
		/* パラメータ更新                                                     */
		/**********************************************************************/
		l_startptr += l_seqwritenum * MFMR_BYTEPERSECT( drvnum );
		                                /* 書込み開始位置更新                 */
		l_work01 = MFMR_FATSECTNUM( drvnum, l_fatareanum );
		l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF */
		l_work01 -= MFMR_FATSECTTOP( drvnum );
		
		/* 次に書込みを開始するクラスタ番号を */
		/* 現在取得中のクラスタ番号とする     */
		l_nowclstnum = l_nextclstnum;	/* 次に書込みを開始するクラスタ番号の */
								/* 先頭セクタを書き込み開始セクタ番号とする   */
		l_startsectnum = l_nowclstnum - 2;              
		l_startsectnum *= MFMR_SECTPERCLST( drvnum );
		l_startsectnum += MFMR_DATATOP( drvnum );   /* 開始セクタ設定         */
		if( l_restsectnum > MFMR_SECTPERCLST( drvnum ) )
		{
			/* while break時、空クラスタを既に１個取得済みの為 */
			l_seqwritenum = MFMR_SECTPERCLST( drvnum );
		}
		else {
			l_seqwritenum = l_restsectnum;
		}
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/


