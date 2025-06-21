/*======================================================================
 * File name    : dellnent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/dellnent.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: dellnent.c $
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
/* 関 数 名：FMR_deleteLFNEntry                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     TFMR_SAERCHINFO * 先頭位置                                  */
/*         ：     TFMR_SAERCHINFO * 最終位置                                  */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_FATALERR                                     */
/*         ：                                                                 */
/* 概    要：ロングファイル名のエントリを削除する                             */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_deleteLFNEntry(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    TFMR_SEARCHINFO * topslot,          /* 先頭位置                           */
    TFMR_SEARCHINFO * btmslot           /* 最終位置                           */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */
	F_DWORD l_sectnum;                  /* 対象セクタ番号                     */

	TFMR_LFNENTRY * l_lfnptr;           /* LFN構造体ポインタ                  */

	F_INT l_drtn;                       /* ドライバエントリ戻り値             */

	F_BYTE l_totentrycnt;               /* 対象エントリ数                     */
	F_BYTE l_sectentrycnt;              /* セクタ内エントリ数                 */
	F_BYTE l_topslot;                   /* セクタ内先頭エントリ位置           */
	F_BYTE l_targetbuffnum;             /* 対象R/Wバッファ番号                */
	
	/**************************************************************************/
	/* パラメータ設定を行う                                                   */
	/**************************************************************************/
	l_modspc = MFMR_SECTPERCLST( drvnum ) - 1;
	l_sectnum = topslot -> sectnum;     /* 対象セクタ番号取得                 */
	l_topslot = topslot -> slot;        /* セクタ内先頭エントリ位置取得       */
	l_targetbuffnum = topslot -> bufnum;/* 対象バッファ番号取得               */
	l_lfnptr = ( TFMR_LFNENTRY * )&( MFMR_RWBUFF( drvnum, l_targetbuffnum,
		l_topslot * sizeof( TFMR_LFNENTRY ) ) );
	                                    /* バッファにポインタをキャスト       */
	l_totentrycnt = l_lfnptr->ord;
	l_totentrycnt &= ~DFMR_LFN_LAST;    /* LFNエントリ数を取得                */
	l_totentrycnt ++;                   /* Dirエントリ分を加算                */
	
	/* セクタ内に存在する対象LFNエントリの数を求める */
	l_sectentrycnt = ( F_BYTE )( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) );
	l_sectentrycnt -= l_topslot;
	if( l_sectentrycnt >= l_totentrycnt )
	{
		/* 残りのエントリ数が1セクタ内の許容数以下の場合 */
		l_sectentrycnt = l_totentrycnt; /* セクタ内のエントリ数               */
		                                /* ←残りのエントリ数                 */
	}
	
	/**************************************************************************/
	/* 削除処理を行う                                                         */
	/**************************************************************************/
	while( 1 )                          /* 対象セクタ数が0となるまで          */
	{
		/* 対象のエントリを削除済みにする */
		for( l_cnt01 = 0; l_cnt01 < l_sectentrycnt; l_cnt01 ++ )
		{
			l_lfnptr->ord = DFMR_DELENTRY;  /* 対象エントリを削除済みとする   */
			l_lfnptr ++;
		}
		/* 対象のエントリが読込まれているセクタを書込む */
		SetBackupFlg(drvnum);
		l_drtn = FMR_setDataSector(
			drvnum,
			l_sectnum,
			l_targetbuffnum
			);
		ClearBackupFlg(drvnum);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		
		/* パラメータの更新を行う */
		l_totentrycnt -= l_sectentrycnt;/* 対象エントリ数から                 */
		                                /* 削除したエントリ分差引く           */
		if( l_totentrycnt == 0 )        /* 全て削除完了の場合                 */
		{
			return DFMR_OK;             /* 正常終了                           */
		}
		l_topslot = 0;                  /* 先頭エントリ位置はセクタ先頭       */
		
		if( l_totentrycnt <= MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) )
		{
			/* 対象エントリ数がセクタ内の許容数以下の場合 */
			/* セクタ内エントリ数←残りのエントリ数 */
			l_sectentrycnt = l_totentrycnt;
			
			/* 対象セクタ番号を最終位置より取得する */
			l_sectnum = btmslot -> sectnum;
			
			/* 対象R/Wバッファ番号を最終位置より取得する */
			l_targetbuffnum = btmslot -> bufnum;
			
		}
		else
		{
			/* セクタ内エントリ数←セクタサイズ分 */
			l_sectentrycnt = MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY );
			if( l_sectnum < MFMR_DATATOP( drvnum ) )
			{                           /* 対象エントリがRoot Dir. Region     */
			                            /* に属する場合                       */
				l_sectnum ++;
			}
			else                        /* 対象エントリがData Regionに属する  */
			{                           /* 場合                               */
				l_work01 = l_sectnum - MFMR_DATATOP( drvnum );
				l_work02 = l_work01;
				l_work01 &= l_modspc;
				if( l_work01 == ( F_DWORD )( MFMR_SECTPERCLST( drvnum ) - 1 ) )
				{
					l_work02 /= MFMR_SECTPERCLST( drvnum );
					l_work02 += 2;      /* 対象セクタが含まれるクラスタ番号   */
					
					/* 対象セクタがクラスタ内最終セクタの場合 */
					/* 次にリンクされているクラスタを取得する */
					l_drtn = FMR_getFatSector(
						MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
						                        /* FAT読込み領域              */
						l_work02,               /* クラスタ番号               */
						&l_work01               /* FATエントリ値              */
						);
					if( l_drtn != DFMR_OK )
					{
						return l_drtn;
					}
					/* 対象セクタ番号に取得したクラスタの先頭セクタ番後を設定する */
					l_work01 -= 2;
					l_work01 *= MFMR_SECTPERCLST( drvnum );
					l_sectnum = l_work01 + MFMR_DATATOP( drvnum );
				}
				else
				{
					/* 対象セクタがクラスタ内最終セクタ以外の場合 */
					/* 対象セクタ番号インクリメント */
					l_sectnum ++;
				
				}
			}
			/* 対象セクタを読込む */
			l_drtn = FMR_readDataSector(
				drvnum,
				l_sectnum,
				l_targetbuffnum
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		/* LFN構造体ポインタを対象R/Wバッファ番号が示す */
		/* バッファの先頭にキャストする                 */
		l_lfnptr = ( TFMR_LFNENTRY * )( MFMR_RWBUFFTOP( drvnum, l_targetbuffnum ) );
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
