/*======================================================================
 * File name    : unmount.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/RCS/unmount.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: unmount.c $
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
/* 関 数 名：FMR_Unmount                                                      */
/*         ：                                                                 */
/* パラメタ：IN: char 論理ドライブ番号                                        */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_DRVNUMERR                                         */
/*         ：          DFMR_DRVNOCONNECTERR                                   */
/*         ：                                                                 */
/* 概    要：論理ドライブのアンマウント                                       */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       RAMテーブルアドレス.R/Wバッファ名変更           04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_Unmount(
	char drvnum                         /* 論理ドライブ番号                   */
)
{
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/O                        */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* 戻り値                             */

	F_WORD l_semid;                     /* セマフォID                         */
	F_WORD l_cnt;                       /* ループカウンタ                     */

	F_CHAR l_drvindex;                  /* ドライブ番号インデックス           */
	                                    /* ('A'=0とした数値)                  */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	if( ( drvnum < DFMR_TOPDRVNUM ) || ( DFMR_ENDDRVNUM < drvnum ) )
	{
		/* 論理ドライブ番号が許容範囲外の場合 */
		return DFMR_DRVNUMERR;          /* 論理ドライブ番号エラー             */
	}
	l_drvindex = drvnum;
	l_drvindex -= DFMR_TOPDRVNUM;
	
	if( tfmr_part_ramaddr[ l_drvindex ].logdrvinfo == ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		return DFMR_DRVNOCONNECTERR;    /* 論理ドライブ未接続エラー           */
	}
	l_semid = MFMR_SEMID( drvnum );
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn = FMR_osWaiSem(
		l_semid
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	/**************************************************************************/
	/* ファイルハンドル初期化                                                 */
	/**************************************************************************/
	for( l_cnt = 0; l_cnt < DFMR_USR_MAXFILEHANDLE; l_cnt ++ )
	{
		if( tfmr_filehandle[ l_cnt ].direntry.name[ 0 ] != DFMR_INITBYTE )
		{                               /* オープン中のファイルは全てクローズ */
			FMR_initFileHandle(
				( F_INT )l_cnt
				);
		}
	}
	/**************************************************************************/
	/* 検索ハンドル初期化                                                     */
	/**************************************************************************/
	for( l_cnt = 0; l_cnt < DFMR_USR_MAXSEARCHHANDLE; l_cnt ++ )
	{
		if( tfmr_searchhandle[ l_cnt ].sectornum != 0xFFFFFFFF )
		{                               /* オープン中のファイルは全てクローズ */
			FMR_initSearchHandle(
				( F_INT )l_cnt
				);
		}
	}
	while( 1 )
	{
		/**********************************************************************/
		/* FSInfo更新                                                         */
		/**********************************************************************/
		if( MFMR_FATTYPE( drvnum ) == DFMR_FATTYPE32 )
		{
			/******************************************************************/
			/* FSInfo読込み                                                   */
			/******************************************************************/
			l_rtn = FMR_readDataSector(
				drvnum,
				MFMR_FSINFO( drvnum ),
				0
				);
			if( l_rtn != DFMR_OK )
			{
				break;
			}
			/******************************************************************/
			/* FSInfo更新                                                     */
			/******************************************************************/
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_FREECOUNT ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_FREECOUNT + 1 ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_FREECOUNT + 2 ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_FREECOUNT + 3 ) = 0xFF;
			
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_NXTFREE ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_NXTFREE + 1 ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_NXTFREE + 2 ) = 0xFF;
			MFMR_RWBUFF( drvnum, 0, DFMR_FSIOFST_NXTFREE + 3 ) = 0xFF;
			
			/******************************************************************/
			/* FSInfo書込み                                                   */
			/******************************************************************/
			l_rtn = FMR_setDataSector(
				drvnum,
				MFMR_FSINFO( drvnum ),
				0
				);
		}
		break;
	}
	/**************************************************************************/
	/* ドライバアンマウント                                                   */
	/**************************************************************************/
	l_rtn = MFMR_DRVENTRY( drvnum )(
		MFMR_CUNUM( drvnum ),           /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCUNMOUNT,            /* I/O Func.番号:アンマウント         */
		&l_drvioparam
		);
	/**************************************************************************/
	/* 論理ドライブRAMアドレステーブル初期化                                  */
	/**************************************************************************/
	tfmr_part_ramaddr[ l_drvindex ].logdrvinfo = ( F_VOID * )DFMR_RAMINIT;
			                            /* 論理ドライブ構成情報               */
	tfmr_part_ramaddr[ l_drvindex ].fatbuff = ( F_VOID * )DFMR_RAMINIT;
			                            /* FAT読込み領域                      */
	tfmr_part_ramaddr[ l_drvindex ].workarea = ( F_VOID * )DFMR_RAMINIT;
			                            /* ワークエリア                       */
	tfmr_part_ramaddr[ l_drvindex ].fatsectarea = ( F_VOID * )DFMR_RAMINIT;
			                            /* FATセクタ管理領域                  */
	tfmr_part_ramaddr[ l_drvindex ].rwbuffarea = ( F_VOID * )DFMR_RAMINIT;
			                            /* R/Wバッファ                        */
	
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		l_semid
		);
	if( l_ortn != DFMR_OK )
	{
		l_rtn = l_ortn;
	}
	
	/**************************************************************************/
	/* 正常終了(異常発生時はエラー)返却                                       */
	/**************************************************************************/
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
