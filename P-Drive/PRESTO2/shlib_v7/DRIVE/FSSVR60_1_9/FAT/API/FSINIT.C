/*======================================================================
 * File name    : fsinit.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fsinit.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsinit.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * Saver60 Ver.01.09 up
 * 
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
/* 関 数 名：FMR_Init                                                         */
/*         ：                                                                 */
/* パラメタ：IN:                                                              */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_DRVINITERR                                        */
/*                     DFMR_CRESENERR                                         */
/*         ：                                                                 */
/* 概    要：ファイルシステムの初期化                                         */
/*         ：                                                                 */
/* 作 成 日：2004.06.07                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 RAMテーブルアドレス.R/Wバッファ名変更           04/12/10 Y.Yoshizumi */
/* 01.07 セマフォ作成数をドライバ構成情報.チャネル数に変更 05/12/12 Y.Yoshizumi */
/*       OS層初期化処理追加                              05/12/12 Y.Yoshizumi */
/* 01.09 ファイル&検索ハンドル用セマフォ作成処理追加     06/03/01 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

int FMR_Init( 
)
{
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_DWORD l_cnt02;                    /* ループカウンタ                     */
	F_WORD l_semid;                     /* 取得セマフォID                     */
	TFMR_DRVIOPARAM drvioparam;         /* ドライバI/Oパラメータ              */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	
	/**************************************************************************/
	/* RAMテーブルアドレス初期化                                              */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < DFMR_USR_MAXDRVNUM; l_cnt01 ++ )
	{
		tfmr_part_ramaddr[ l_cnt01 ].logdrvinfo = ( F_VOID * )DFMR_RAMINIT;
		                                /* 論理ドライブ構成情報               */
		tfmr_part_ramaddr[ l_cnt01 ].fatbuff = ( F_VOID * )DFMR_RAMINIT;
		                                /* FAT読込み領域                      */
		tfmr_part_ramaddr[ l_cnt01 ].workarea = ( F_VOID * )DFMR_RAMINIT;
		                                /* ワークエリア                       */
		tfmr_part_ramaddr[ l_cnt01 ].fatsectarea = ( F_VOID * )DFMR_RAMINIT;
		                                /* FATセクタ管理領域                  */
		tfmr_part_ramaddr[ l_cnt01 ].rwbuffarea = ( F_VOID * )DFMR_RAMINIT;
		                                /* R/Wバッファ領域                    */
	}
	/**************************************************************************/
	/* ファイルハンドル初期化                                                 */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < DFMR_USR_MAXFILEHANDLE; l_cnt01 ++ )
	{
		FMR_initFileHandle(             /* ファイルハンドル初期化             */
			( F_INT )l_cnt01            /* カウンタをファイルハンドルとする   */
			);
	}
	/**************************************************************************/
	/* 検索ハンドル初期化                                                     */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < DFMR_USR_MAXSEARCHHANDLE; l_cnt01 ++ )
	{
		FMR_initSearchHandle(           /* 検索ハンドル初期化                 */
			( F_INT )l_cnt01            /* カウンタをファイルハンドルとする   */
			);
	}
#if 0
	/**************************************************************************/
	/* OS層初期化                                                             */
	/**************************************************************************/
	l_drtn = FMR_osInit();
	if( l_drtn != DFMR_OK )
	{
		return DFMR_DRVINITERR;
	}
#endif
	/**************************************************************************/
	/* ドライバ初期化                                                         */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < DFMR_USR_MAXDRIVER; l_cnt01 ++ )
	{
		l_drtn = ( tfmr_drvinfo[ l_cnt01 ].drventry )(
			0,                          /* チャネル/ユニット番号(ダミー)      */
			DFMR_DRVFUNCINIT,           /* I/O Func.番号:初期化               */
			&drvioparam                 /* I/Oパラメータ(Reserved)            */
			);
		if( l_drtn != DFMR_OK )
		{
			return DFMR_DRVINITERR;
		}
		for( l_cnt02 = 0; l_cnt02 < tfmr_drvinfo[ l_cnt01 ].channelnum; l_cnt02 ++ )
		{
			/* セマフォを作成する                                             */
#ifdef DFMR_SEMID_AUTO
			l_drtn = FMR_osAcreSem();
			l_semid = l_drtn;
			tfmr_drvinfo[ l_cnt01 ].semaphoreid[ l_cnt02 ] = l_semid;
#else /* DFMR_SEMID_AUTO */
			l_semid = tfmr_drvinfo[ l_cnt01 ].semaphoreid[ l_cnt02 ];
			l_drtn = FMR_osAcreSem(
				l_semid
				);
#endif /* DFMR_SEMID_AUTO */
			if( l_drtn == DFMR_NG )
			{
				return DFMR_CRESENERR;
			}
		}
	}
	/**************************************************************************/
	/* ファイル&検索ハンドル用セマフォを作成                                  */
	/**************************************************************************/
#ifdef DFMR_SEMID_AUTO
	l_drtn = FMR_osAcreSem();
	l_semid = l_drtn;
#else /* DFMR_SEMID_AUTO */
	l_semid = DFMR_USR_SEMID_HANDLE;
	l_drtn = FMR_osAcreSem(
		l_semid
		);
#endif /* DFMR_SEMID_AUTO */
	if( l_drtn == DFMR_NG )
	{
		return DFMR_CRESENERR;
	}
	g_semid = l_semid;
	return DFMR_SUC;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
