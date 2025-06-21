/*======================================================================
 * File name    : dlerrlfn.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/dlerrlfn.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: dlerrlfn.c $
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
/* 関 数 名：FMR_deleteErrLFNEntry                                            */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     TFMR_SEARCHINFO * 削除対象先頭エントリ位置                  */
/*         ：     TFMR_SEARCHINFO * 削除対象最終エントリ位置                  */
/*         ：     TFMR_SEARCHINFO * 現在アクセス中のエントリ位置              */
/*         ：                                                                 */
/* 戻 り 値：DFMR_OK 正常終了                                                 */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：対象の不整合なLFNエントリを削除する                              */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_INT FMR_deleteErrLFNEntry(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	TFMR_SEARCHINFO * toplfnpos,        /* 削除対象先頭エントリ位置           */
	TFMR_SEARCHINFO * btmlfnpos,        /* 削除対象最終エントリ位置           */
	TFMR_SEARCHINFO * nowlfnpos         /* 現在アクセス中のエントリ位置       */
)
{
	TFMR_LFNENTRY * l_lfnptr;           /* LFN構造体ポインタ                  */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_sectnum;                  /* 対象セクタ番号                     */
	F_DWORD l_lastsectnum;              /* 対象クラスタ内最終セクタ番号       */
	F_DWORD l_clstnum;                  /* 対象クラスタ番号                   */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_BYTE l_slot;                      /* 対象セクタ内位置                   */
	F_BYTE l_bufnum;                    /* 対象バッファ番号                   */
	
	/* 先頭位置が読込まれているバッファ番号を取得する */
	l_bufnum = toplfnpos->bufnum;
	
	/* 先頭位置を取得する */
	l_sectnum = toplfnpos->sectnum;
	l_slot = toplfnpos->slot;
	l_lfnptr = ( TFMR_LFNENTRY * )( MFMR_RWBUFFTOP( drvnum, l_bufnum ) );
	
	/* クラスタ内の最終セクタ番号を求めておく */
	l_lastsectnum = FMR_getLastSectNum(
		drvnum,                         /* ドライブ番号                       */
		l_sectnum                       /* セクタ番号                         */
		);
	
	while( 1 )
	{
		/* 対象エントリを削除済みとする */
		( l_lfnptr + l_slot )->ord = DFMR_DELENTRY;
		
		if( ( l_sectnum == btmlfnpos->sectnum )     /* 対象エントリが         */
			&& ( l_slot == btmlfnpos->slot ) )      /* 削除対象の最終         */
		{                                           /* エントリの場合         */
			if( nowlfnpos->sectnum != l_sectnum )
			{
				/* 現在位置と最終位置が異なるセクタの場合 */
				/* (最終位置がセクタ終端の場合に起こる)   */
				/* 対象セクタを書込む */
				l_rtn = FMR_writeSectRetry(
					drvnum,             /* ドライブ番号                       */
					l_sectnum,          /* セクタ番号                         */
					l_bufnum            /* R/Wバッファ番号                    */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
			}
			return DFMR_OK;
		}
		
		/* セクタ内位置の更新 */
		l_slot ++;
		
		if( l_slot >= MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) )
		{
			/* 対象セクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				l_bufnum                /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			if( l_sectnum == l_lastsectnum )    /* 現在のセクタ番号がクラスタ */
			{                                   /* 内の最終セクタの場合       */
				/* 対象クラスタ番号を求める */
				l_clstnum = l_sectnum - MFMR_DATATOP( drvnum );
				l_clstnum = l_clstnum / MFMR_SECTPERCLST( drvnum );
				l_clstnum += DFMR_TOPCLSTNUM;
				
				/* 対象クラスタのFATエントリを取得する */
				l_rtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報       */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域                      */
					l_clstnum,                  /* クラスタ番号                       */
					&l_fatentry,                /* FATエントリ値                      */
					DFMR_FLGOFF                 /* Read/Write種別  OFF:取得           */
					);
				if( l_rtn != DFMR_OK )          /* エラーの場合は対象クラスタが       */
				{                               /* 他のFATセクタにあると判断          */
					if( l_rtn == DFMR_CLSTNUMERR )
					{                           /* クラスタ番号異常の場合は異常終了   */
						return l_rtn;
					}
					/* FATセクタを読込む */
					l_rtn = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
						                            /* FAT読込み領域                      */
						l_clstnum,                  /* クラスタ番号                       */
						&l_fatentry                 /* FATエントリ値                      */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
				}
				
				/* 対象クラスタの先頭セクタ番号を対象セクタ番号とする */
				l_sectnum = FMR_getSectNum(
					l_fatentry,         /* クラスタ番号                       */
					0,                  /* クラスタ内セクタ番号               */
					drvnum              /* ドライブ番号                       */
					);
				
				/* 対象クラスタの最終セクタ番号を求める */
				l_lastsectnum = FMR_getLastSectNum(
					drvnum,             /* ドライブ番号                       */
					l_sectnum           /* セクタ番号                         */
					);
			}
			else
			{
				/* 対象セクタ番号を更新する */
				l_sectnum ++;
			}
			
			if( l_sectnum != nowlfnpos->sectnum )
			{                           /* 求めたセクタ番号が最終位置を含む   */
			                            /* セクタと異なる場合                 */
				/* バッファに対象セクタを読込む */
				l_rtn = FMR_readSectRetry(
					drvnum,             /* ドライブ番号                       */
					l_sectnum,          /* セクタ番号                         */
					l_bufnum            /* R/Wバッファ番号                    */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
			}
			else                        /* 求めたセクタ番号が最終位置を含む   */
			{                           /* セクタと同じ場合                   */
				/* バッファを切替える */
				l_bufnum = btmlfnpos->bufnum;
				
				/* LFNのポインタを再設定する */
				l_lfnptr = ( TFMR_LFNENTRY * )( MFMR_RWBUFFTOP( drvnum, l_bufnum ) );
			}
			/* セクタ内位置を更新する */
			l_slot = DFMR_SRCHROOTTOPSLOT;      /* セクタ内位置は0            */
		}
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
