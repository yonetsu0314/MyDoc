/*======================================================================
 * File name    : fsinfo.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fsinfo.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsinfo.c $
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
/* 関 数 名：FMR_Info                                                         */
/*         ：                                                                 */
/* パラメタ：IN:    F_CHAR drvnum    ドライブ番号                             */
/*         ：                                                                 */
/*         ：OUT:   TFMR_DRIVE_INFO *ドライブ状態格納領域                     */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ファイルシステム情報取得                                         */
/*         ：                                                                 */
/* 作 成 日：2004.05.27                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_Info ( 
	F_CHAR drvnum,                      /* ドライブ番号                       */
	TFMR_DRIVE_INFO * drvinfo           /* ドライブ状態格納領域               */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cnt01;                    /* カウンタ                           */
	F_DWORD l_cnt02;
	F_DWORD l_emptysize;                /* 空き領域サイズ                     */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	/* ドライブ番号のチェック */
	if( ( drvnum < DFMR_TOPDRVNUM ) || ( DFMR_ENDDRVNUM < drvnum ) )
	{
		/* 論理ドライブ番号が許容範囲外の場合 */
		return DFMR_DRVNUMERR;           /* 論理ドライブ番号エラー             */
	}

	if( tfmr_part_ramaddr[ drvnum - DFMR_TOPDRVNUM ].logdrvinfo == ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		return DFMR_DRVNOCONNECTERR;    /* 論理ドライブ未接続エラー           */
	}
	
	/* ドライブ状態格納領域のチェック */
	if( drvinfo == NULL )               /* ドライブ状態格納領域がNULLの場合   */
	{
		return DFMR_ERRPARAM;           /* パラメータエラーを返却             */
	}
	
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn = FMR_osWaiSem(
		MFMR_SEMID( drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	/**************************************************************************/
	/* パラメータ取得                                                         */
	/**************************************************************************/
	/* セクタ番号0を読込む */
	l_rtn = FMR_readDataSector(
		drvnum,
		0,
		0
		);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	/* BPBよりパラメータを取得する */
	/* ボリュームラベル取得 */
	FMR_memCopy(
		&( drvinfo -> volume[ 0 ] ),
		&( MFMR_RWBUFF( drvnum, 0, DFMR_BSOFST_VOLLAB ) ),
		DFMR_VNAMESZ
		);
	/* メディアタイプ取得 */
	drvinfo -> mediatype = MFMR_RWBUFF( drvnum, 0, DFMR_BPBOFST_MEDIA );
	
	/* RAM領域よりパラメータを取得する */
	/* FATタイプ取得 */
	drvinfo -> fattype = MFMR_FATTYPE( drvnum );
	/* 容量取得 */
	drvinfo -> totalsize = MFMR_TOTALCLST( drvnum ) * MFMR_SECTPERCLST( drvnum )
							 * MFMR_BYTEPERSECT( drvnum );    /* 総バイト数   */
	
	/**************************************************************************/
	/* 空き容量取得                                                           */
	/**************************************************************************/
	l_emptysize = 0;                    /* 空き領域サイズ初期化               */
	if( MFMR_FATTYPE( drvnum ) == DFMR_FATTYPE32 )
	{
		for( l_cnt01 = 0; l_cnt01 < MFMR_SECTPERFAT( drvnum ); l_cnt01 += DFMR_FATMNGSIZE )
		{
			/* FATセクタ管理情報を構築する */
			l_rtn = FMR_restoreFATInfo(
				drvnum,                     /* 対象論理ドライブ番号               */
				l_cnt01                     /* 開始セクタオフセット               */
				);
			if( l_rtn != DFMR_OK )          /* エラー終了の場合                   */
			{
				goto ERROR;
			}
			if( ( MFMR_SECTPERFAT( drvnum ) - l_cnt01 ) >= DFMR_FATMNGSIZE )
			{                               /* オフセット位置から後ろに管理領域   */
			                                /* 最大数以上のセクタが存在する場合   */
				l_work01 = DFMR_FATMNGSIZE; /* 最大FAT読込み数は管理領域最大数    */
			}
			else
			{
				l_work01 = MFMR_SECTPERFAT( drvnum ) - l_cnt01;
				                            /* 最大読込み数はオフセット以降全て   */
			}
			for( l_cnt02 = 0; l_cnt02 < l_work01; l_cnt02 ++ )
			{
				/* 各セクタの空きクラスタ数を順次加算 */
				l_emptysize += MFMR_FATEMPTYNUM( drvnum, l_cnt02 );
			}
		}
	}
	else
	{
		for( l_cnt02 = 0; l_cnt02 < MFMR_SECTPERFAT( drvnum ); l_cnt02 ++ )
		{
			/* 各セクタの空きクラスタ数を順次加算 */
			l_emptysize += MFMR_FATEMPTYNUM( drvnum, l_cnt02 );
		}
	}
	/* 取得した空きクラスタ数をドライブ状態格納領域に設定 */
	drvinfo -> emptyzise = l_emptysize * MFMR_SECTPERCLST( drvnum ) * MFMR_BYTEPERSECT( drvnum );
	/* 使用領域は総要領 - 空きクラスタ */
	drvinfo -> usedsize = drvinfo -> totalsize - drvinfo -> emptyzise;
	
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		MFMR_SEMID( drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	
	/**************************************************************************/
	/* 正常終了返却                                                           */
	/**************************************************************************/
	return DFMR_SUC;

ERROR:
	/* 異常終了 */
	l_ortn = FMR_osSigSem(              /* セマフォ資源返却                   */
		MFMR_SEMID( drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	return l_rtn;

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
