/*======================================================================
 * File name    : lnerrlog.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/lnerrlog.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: lnerrlog.c $
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
/* 関 数 名：FMR_recLFNErrLog                                                 */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_LFNENTRY * 最終LFNエントリ                             */
/*         ：     F_CHAR * エントリ名格納領域                                 */
/*         ：     F_WORD * エントリ名設定先頭位置                             */
/*         ：OUT: F_BYTE * ログ出力領域                                       */
/*         ：     TFMR_SDWORKAREA * スキャンディスク用ワークエリア            */
/*         ：                                                                 */
/* 戻 り 値：なし                                                             */
/*         ：                                                                 */
/* 概    要：ロギングを行う                                                   */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_VOID FMR_recLFNErrLog(
	TFMR_LFNENTRY * l_entry,            /* 最終LFNエントリ                    */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	TFMR_SDWORKAREA * sdworkarea,       /* クラスタ使用状況展開領域           */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	F_WORD * entrypathlen               /* エントリ名設定先頭位置             */
)

{
	TFMR_SDLOGTBLSHT * l_sdlogtblsht;   /* ログ構造体(フルパス非指定)         */
	TFMR_SDLOGTBLLNG * l_sdlogtbllng;   /* ログ構造体(フルパス指定)           */
	F_WORD l_cnt;                       /* ループカウンタ                     */
	F_WORD l_namelen;                   /* 名称長                             */
	/* ロギング動作OFFの場合 */
	if( ( ( sdworkarea->mode ) & DFMR_SD_LOGMODEON ) != DFMR_SD_LOGMODEON )
	{
		return;
	}
	
	/* 元ファイル名表示方法による分岐 */
	if( ( ( sdworkarea->mode ) & DFMR_SD_LOGFULLPATH )
		!= DFMR_SD_LOGFULLPATH )        /* ファイル名のみ表示の場合           */
	{
		/* ログ構造体キャスト */
		l_sdlogtblsht = ( TFMR_SDLOGTBLSHT * )logarea;
		l_sdlogtblsht += sdworkarea->logcnt;
		
		/* ログ設定 */
		l_sdlogtblsht->errcode = DFMR_SD_LOGERRLFNILL;
		                                /* エラーコード = 不正なLFNエントリ   */
		l_sdlogtblsht->orgtopclstnum = DFMR_INITBYTE;
		                                /* 元ファイルの先頭クラスタ番号 = 0   */
		l_sdlogtblsht->orgclstnum = DFMR_INITBYTE;
		                                /* 破損ファイルのクラスタ数 = 0       */
		FMR_memSet(                     /* 元ファイル名格納領域初期化         */
			l_sdlogtblsht->orgname,
			DFMR_INITBYTE,
			( DFMR_FNAMESZ + DFMR_EXTSZ + 1 )
			);
		/* 元ファイル名設定 */
		l_namelen = 0;                  /* 名称長初期化                       */
		for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME1SZ; l_cnt ++ )
		{                               /* ファイル名1を取得                  */
			if( l_entry -> name1[ l_cnt * sizeof( F_WORD ) ] == NULL )
			{
				break;
			}
			l_sdlogtblsht->orgname[ l_namelen ]
				= l_entry -> name1[ l_cnt * sizeof( F_WORD ) ];
			l_namelen ++;
		}
		if( ( l_cnt == DFMR_LFN_FNAME1SZ ) && ( l_entry -> name2[ 0 ] != NULL ) )
		{                           /* ファイル名2が存在する場合          */
			for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME2SZ; l_cnt ++ )
			{                       /* ファイル名2を取得                  */
				if( l_entry -> name2[ l_cnt * sizeof( F_WORD ) ] == NULL )
				{
					break;
				}
				l_sdlogtblsht->orgname[ l_namelen ]
					= l_entry -> name2[ l_cnt * sizeof( F_WORD ) ];
				l_namelen ++;
			}
			if( ( l_cnt == DFMR_LFN_FNAME2SZ ) && ( l_entry -> name2[ 0 ] != NULL ) )
			{                           /* ファイル名3が存在する場合          */
				for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME3SZ - 1; l_cnt ++ )
				{                       /* ファイル名3を取得                  */
					if( l_entry -> name3[ l_cnt * sizeof( F_WORD ) ] == NULL )
					{
						break;
					}
					l_sdlogtblsht->orgname[ l_namelen ]
						= l_entry -> name3[ l_cnt * sizeof( F_WORD ) ];
					l_namelen ++;
				}
			}
		}
	}
	else                                /* フルパス指定の場合                 */
	{
		/* ログ構造体キャスト */
		l_sdlogtbllng = ( TFMR_SDLOGTBLLNG * )logarea;
		l_sdlogtbllng += sdworkarea->logcnt;
		
		/* ログ設定 */
		l_sdlogtbllng->errcode = DFMR_SD_LOGERRLFNILL;
		                                /* エラーコード = 不正なLFNエントリ   */
		l_sdlogtbllng->orgtopclstnum = DFMR_INITBYTE;
		                                /* 元ファイルの先頭クラスタ番号 = 0   */
		l_sdlogtbllng->orgclstnum = DFMR_INITBYTE;
		                                /* 破損ファイルのクラスタ数 = 0       */
		                                
		FMR_memSet(                     /* 元ファイル名格納領域初期化         */
			l_sdlogtbllng->orgname,
			DFMR_INITBYTE,
			DFMR_MAXFILESZ
			);
		FMR_memCopy(                    /* 元ファイル名設定                   */
			l_sdlogtbllng->orgname,
			entrypath,
			*( entrypathlen )
			);
		l_sdlogtbllng->orgname[ *( entrypathlen ) ] = '\\';
		                                /* パス区切り記号設定                 */
		l_namelen = 1;                  /* 名称長初期化(パス区切り記号)       */
	
	
		for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME1SZ; l_cnt ++ )
		{                               /* ファイル名1を取得                  */
			if( l_entry -> name1[ l_cnt * sizeof( F_WORD ) ] == NULL )
			{
				break;
			}
			l_sdlogtbllng->orgname[ *( entrypathlen ) + l_namelen  ]
				= l_entry -> name1[ l_cnt * sizeof( F_WORD ) ];
			l_namelen ++;
		}
		if( ( l_cnt == DFMR_LFN_FNAME1SZ ) && ( l_entry -> name2[ 0 ] != NULL ) )
		{                           /* ファイル名2が存在する場合          */
			for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME2SZ; l_cnt ++ )
			{                       /* ファイル名2を取得                  */
				if( l_entry -> name2[ l_cnt * sizeof( F_WORD ) ] == NULL )
				{
					break;
				}
				l_sdlogtbllng->orgname[ *( entrypathlen ) + l_namelen  ]
					= l_entry -> name2[ l_cnt * sizeof( F_WORD ) ];
				l_namelen ++;
			}
			if( ( l_cnt == DFMR_LFN_FNAME2SZ ) && ( l_entry -> name2[ 0 ] != NULL ) )
			{                           /* ファイル名3が存在する場合          */
				for( l_cnt = 0; l_cnt < DFMR_LFN_FNAME3SZ - 1; l_cnt ++ )
				{                       /* ファイル名3を取得                  */
					if( l_entry -> name3[ l_cnt * sizeof( F_WORD ) ] == NULL )
					{
						break;
					}
					l_sdlogtbllng->orgname[ *( entrypathlen ) + l_namelen  ]
						= l_entry -> name3[ l_cnt * sizeof( F_WORD ) ];
					l_namelen ++;
				}
			}
		}
	}
	
	/* ログカウンタ更新 */
	sdworkarea->logcnt ++;
	
	if( sdworkarea->logcnt >= sdworkarea->logmax )
	{
		sdworkarea->logcnt = DFMR_SD_LOGCNTINIT;
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
