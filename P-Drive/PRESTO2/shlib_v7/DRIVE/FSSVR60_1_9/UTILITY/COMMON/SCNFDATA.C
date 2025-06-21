/*======================================================================
 * File name    : scnfdata.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/scnfdata.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scnfdata.c $
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
/* 関 数 名：FMR_scanFileData                                                 */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD ファイルサイズ                                      */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：     F_CHAR * エントリ名格納領域                                 */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：     F_BYTE * ログ出力領域                                       */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：ファイルデータのリンク切れ/BADクラスタチェックを行う             */
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

/* BADクラスタ/多重リンク発見時エラー処理 */
static F_INT FMR_scanFileDataErr(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD lastclst,                   /* 前クラスタ番号                     */
	F_DWORD eoc,                        /* EOCコード                          */
	F_DWORD topclst,                    /* 先頭クラスタ番号                   */
	F_DWORD clstcnt,                    /* クラスタ数                         */
	F_BYTE logerrcode,                  /* ログのエラーコード                 */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	TFMR_SDWORKAREA * sdworkarea        /* スキャンディスク用ワークエリア     */
);

F_INT FMR_scanFileData(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD size,                       /* ファイルサイズ                     */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
)
{
	TFMR_SDWORKAREA * l_sdworkarea;     /* スキャンディスク用ワークエリア     */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_clstcnt;                  /* クラスタカウンタ                   */
	F_DWORD l_clstnum;                  /* クラスタ数                         */
	F_DWORD l_nowclst;                  /* 現在のクラスタ番号                 */
	F_DWORD l_nowtopclst;               /* 現在の先頭クラスタ番号             */
	F_DWORD l_lastclst;                 /* 前クラスタ番号                     */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_DWORD l_eocmin;                   /* 比較用EOCコード                    */
	F_DWORD l_eoc;                      /* 設定用EOCコード                    */
	F_DWORD l_bad;                      /* BADクラスタコード                  */
	F_BYTE l_fatflg;                    /* クラスタ使用状況                   */
	F_BYTE l_badflg;                    /* BADクラスタ検出判定用フラグ        */
	                                    /* ON :BADクラスタ検出中とする        */
	                                    /* OFF:現在読込んでいるFATセクタ内で  */
	                                    /*     BADクラスタは未検出            */
	F_BYTE l_brokenflg;                 /* 破損ファイル検出判定用フラグ       */
	                                    /* ON :破損箇所検出済み               */
	                                    /* OFF:破損箇所未検出                 */
	
	/* 先頭クラスタ番号チェック */
	if( topclstnum == DFMR_INITBYTE )   /* 先頭クラスタ番号が0(エントリのみ)  */
	{                                   /* の場合                             */
		if( size != DFMR_INITBYTE )     /* サイズが0以外の場合                */
		{
			FMR_recErrLog(
				drvnum,                 /* ドライブ番号                       */
				DFMR_INITBYTE,          /* 断片ファイルの先頭クラスタ番号     */
				DFMR_INITBYTE,          /* 断片ファイルのクラスタ数           */
				DFMR_SD_LOGERRMSIZE,    /* サイズエラー                       */
				entrypath,              /* 元ファイル名                       */
				logarea,                /* ログ出力領域                       */
				l_sdworkarea            /* ワークエリア                       */
				);
			return DFMR_SDSBROKENFILE;
		}
		return DFMR_OK;
	}
	/**************************************************************************/
	/* 初期設定                                                               */
	/**************************************************************************/
	/* EOC/BADクラスタコード取得 */
	MFMR_GETEOCMIN( MFMR_FATTYPE( drvnum ), l_eocmin );
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_eoc );
	MFMR_GETBAD( MFMR_FATTYPE( drvnum ), l_bad );
	l_sdworkarea = ( TFMR_SDWORKAREA * )ramaddr;
	
	/* 現在のクラスタ番号/先頭クラスタ番号を先頭クラスタ番号とする */
	l_nowclst = topclstnum;
	l_nowtopclst = l_nowclst;
	
	/* 前クラスタ番号を0とする */
	l_lastclst = DFMR_INITBYTE;
	
	/* 対象ファイルの破損部分未検出 */
	l_brokenflg = DFMR_FLGOFF;
	
	/* ファイルサイズからクラスタ数を求める */
	size = size
		+ ( MFMR_SECTPERCLST( drvnum ) * MFMR_BYTEPERSECT( drvnum ) ) - 1;
	l_clstnum = size
		/ ( MFMR_SECTPERCLST( drvnum ) * MFMR_BYTEPERSECT( drvnum ) );
	
	/* FAT読込み領域0を初期化 */
	MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 ) = DFMR_INITBYTE;
	
	/* FAT読込み領域0に先頭クラスタ番号を含むFATセクタを読込み */
	/* 先頭クラスタ番号のFATエントリ値を取得する               */
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		l_nowclst,                      /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/**************************************************************************/
	/* 先頭クラスタのBADクラスタチェック                                      */
	/**************************************************************************/
	/* 先頭クラスタ番号クラスタ使用状況を確認する */
	l_fatflg = FMR_framBitChk(          /* クラスタ使用状況のBitの確認を行う  */
		l_nowclst,                      /* クラスタ番号                       */
		ramaddr                         /* クラスタ使用状況展開領域           */
	);
	if( l_fatflg != DFMR_FLGON )        /* クラスタが未使用/確認済みの場合    */
	{
		/* ロギング処理を行う */
		FMR_recErrLog(
			drvnum,                     /* ドライブ番号                       */
			DFMR_INITBYTE,              /* 断片ファイルの先頭クラスタ番号     */
			DFMR_INITBYTE,              /* 断片ファイルのクラスタ数           */
			DFMR_SD_LOGERRNOTOP,        /* エラー種別                         */
			entrypath,                  /* 元ファイル名                       */
			logarea,                    /* ログ出力領域                       */
			l_sdworkarea                /* ワークエリア                       */
			);
		return DFMR_SDSBROKENFILE;
	}
	
	/* クラスタカウンタ初期化 */
	l_clstcnt = 1;
	
	/* BADクラスタ未検出 */
	l_badflg = DFMR_FLGOFF;
	
	/* 先頭クラスタのBADクラスタチェックを行う */
	l_rtn = FMR_chkBadClst(
		drvnum,                         /* ドライブ番号                       */
		l_nowclst,                      /* クラスタ番号                       */
		0,                              /* R/Wバッファ番号 = 0                */
		DFMR_FLGON                      /* 対象クラスタ判別フラグ             */
		                                /*  = ON :使用中のクラスタ            */
		);
	
	if( l_rtn != DFMR_OK )              /* BADクラスタの場合                  */
	{
		/* BADクラスタ検出済みとする */
		l_badflg = DFMR_FLGON;
		
		/* 破損箇所検出済みとする */
		l_brokenflg = DFMR_FLGON;
		
		/* 対象のクラスタ使用状況をOFFとする */
		FMR_framBitOff(
			l_nowclst,                  /* クラスタ番号                       */
			ramaddr                     /* クラスタ使用状況展開領域           */
			);
		
		/* 対象クラスタのFATエントリをBADクラスタに設定 */
		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_nowclst,                  /* クラスタ番号                       */
			&l_bad,                     /* FATエントリ値(BADクラスタ)         */
			DFMR_FLGON                  /* Read/Write種別  ON:設定            */
			);
		
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
		/* ロギング処理を行う */
		FMR_recErrLog(
			drvnum,                     /* ドライブ番号                       */
			DFMR_INITBYTE,              /* 断片ファイルの先頭クラスタ番号     */
			DFMR_INITBYTE,              /* 断片ファイルのクラスタ数           */
			DFMR_SD_LOGERRDIV,          /* BADクラスタ                        */
			entrypath,                  /* 元ファイル名                       */
			logarea,                    /* ログ出力領域                       */
			l_sdworkarea                /* ワークエリア                       */
			);
		if( l_fatentry >= l_eocmin )    /* FATエントリがEOCの場合             */
		{
			/* FAT読込み領域0を書込む */
			l_rtn = FMR_setFatSectorRetry(
				drvnum,                 /* ドライブ番号                       */
			    DFMR_REFFATAREA0        /* FAT読込み領域番号                  */
			);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			return DFMR_SDSBROKENFILE;
		}
		l_nowtopclst = l_fatentry;
		/* クラスタは未検出とする */
		l_clstcnt = 0;
	}
	else
	{
		/* 前クラスタ番号に対象クラスタ番号を設定 */
		l_lastclst = l_nowclst;
		
		if( l_fatentry >= l_eocmin )       /* クラスタ番号がEOCの場合            */
		{
			/* EOCを検出したクラスタ数とファイルサイズから */
			/* 割り出したクラスタ数が異なる場合            */
			if( l_clstcnt != l_clstnum )
			{
				FMR_recErrLog(
					drvnum,             /* ドライブ番号                       */
					DFMR_INITBYTE,      /* 断片ファイルの先頭クラスタ番号     */
					DFMR_INITBYTE,      /* 断片ファイルのクラスタ数           */
					DFMR_SD_LOGERRMSIZE,/* サイズエラー                       */
					entrypath,          /* 元ファイル名                       */
					logarea,            /* ログ出力領域                       */
					l_sdworkarea        /* ワークエリア                       */
					);
				return DFMR_SDSBROKENFILE;
			}
			else                        /* サイズが一致した場合               */
			{
				/* クラスタ使用状況をOFFとする */
				l_rtn = FMR_framChainClstBitOff(
					drvnum,             /* ドライブ番号                       */
					topclstnum,         /* 先頭クラスタ番号                   */
					l_eocmin,              /* EOCコード                          */
					ramaddr             /* クラスタ使用状況展開領域           */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				return DFMR_OK;
			}
		}
	}
	/* クラスタ番号更新 */
	l_nowclst = l_fatentry;
	
	/* クラスタ番号に対するFATエントリを取得 */
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		l_nowclst,                      /* クラスタ番号                       */
		&l_fatentry,                    /* FATエントリ値                      */
		DFMR_FLGOFF                     /* Read/Write種別  OFF:取得           */
		);
	if( l_rtn != DFMR_OK )              /* エラーの場合                       */
	{
		if( l_rtn == DFMR_CLSTNUMERR )  /* クラスタ番号異常の場合は異常終了   */
		{
			return l_rtn;
		}
		
		if( l_badflg != DFMR_FLGOFF )   /* BADクラスタを検出済みの場合        */
		{
			/* 現在読込まれているFATセクタを書込む */
			l_rtn = FMR_setFatSectorRetry(      /* FATセクタを書込む          */
				drvnum,                 /* ドライブ番号                       */
			    DFMR_REFFATAREA0        /* FAT読込み領域番号                  */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* BADクラスタを未検出とする */
			l_badflg = DFMR_FLGOFF;
		}
		/* FATセクタを読込む */
		l_rtn = FMR_getFatSectorRetry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_nowclst,                  /* クラスタ番号                       */
			&l_fatentry                 /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	
	
	/**************************************************************************/
	/* 2番目以降のクラスタのBADクラスタチェック                               */
	/**************************************************************************/
	/* FATのチェインを辿り、データ部のBADクラスタチェックを行う */
	while( 1 )
	{
		/* 対象クラスタの使用状況を確認する */
		l_fatflg = FMR_framBitChk(      /* クラスタ使用状況のBitの確認を行う  */
			l_nowclst,                  /* クラスタ番号                       */
			ramaddr                     /* クラスタ使用状況展開領域           */
		);
		if( l_fatflg != DFMR_FLGON )    /* クラスタが未使用/確認済みの場合    */
		{
			/* エラー処理を行う */
			l_rtn = FMR_scanFileDataErr(
				drvnum,                 /* ドライブ番号                       */
				l_lastclst,             /* 前クラスタ番号                     */
				l_eoc,                  /* EOCコード                          */
				l_nowtopclst,           /* 先頭クラスタ番号                   */
				l_clstcnt,              /* クラスタ数                         */
				DFMR_SD_LOGERRMULTI,    /* 多重リンクエラー                   */
				entrypath,              /* エントリ名格納領域                 */
				logarea,                /* ログ出力領域                       */
				l_sdworkarea            /* スキャンディスク用ワークエリア     */
			);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* FAT読込み領域0を書込む */
			l_rtn = FMR_setFatSectorRetry(
				drvnum,                 /* ドライブ番号                   */
				DFMR_REFFATAREA0        /* FAT読込み領域番号              */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			return DFMR_SDSBROKENFILE;
		}
		
		/* BADクラスタチェックを行う */
		l_rtn = FMR_chkBadClst(
			drvnum,                     /* ドライブ番号                       */
			l_nowclst,                  /* クラスタ番号                       */
			0,                          /* R/Wバッファ番号 = 0                */
			DFMR_FLGON                  /* 対象クラスタ判別フラグ             */
			                            /*  = ON :使用中のクラスタ            */
			);
		if( l_rtn != DFMR_OK )          /* BADクラスタの場合                  */
		{
			/* BADクラスタ検出済みとする */
			l_badflg = DFMR_FLGON;
			
			/* 破損箇所検出済みとする */
			l_brokenflg = DFMR_FLGON;
			
			/* 対象クラスタのFATエントリをBADクラスタに設定 */
			l_rtn = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_nowclst,              /* クラスタ番号                       */
				&l_bad,                 /* FATエントリ値(BADクラスタ)         */
				DFMR_FLGON              /* Read/Write種別  ON:設定            */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* エラー処理を行う */
			l_rtn = FMR_scanFileDataErr(
				drvnum,                 /* ドライブ番号                   */
				l_lastclst,             /* 前クラスタ番号                 */
				l_eoc,                  /* EOCコード                      */
				l_nowtopclst,           /* 先頭クラスタ番号               */
				l_clstcnt,              /* クラスタ数                     */
				DFMR_SD_LOGERRDIV,      /* BADクラスタ                    */
				entrypath,              /* エントリ名格納領域             */
				logarea,                /* ログ出力領域                   */
				l_sdworkarea            /* スキャンディスク用ワークエリア */
			);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* 前クラスタ番号を0とする */
			l_lastclst = DFMR_INITBYTE;
			
			/* 現在の先頭クラスタ番号にFATエントリの値を設定する */
			l_nowtopclst = l_fatentry;
			
			/* 対象のクラスタ使用状況をOFFとする */
			FMR_framBitOff(
				l_nowclst,          /* クラスタ番号                       */
				ramaddr             /* クラスタ使用状況展開領域           */
				);
			/* クラスタ数を初期値に戻す */
			l_clstcnt = 0;
			
		}
		else
		{
			/* 前クラスタ番号に対象クラスタ番号を設定 */
			l_lastclst = l_nowclst;
			/* クラスタ数加算 */
			l_clstcnt ++;
			
			if( l_clstcnt > ( l_clstnum * DFMR_SD_RECURRENTMAX ) )
			{                           /* クラスタ数がサイズから割出した数   */
			                            /* を大きく上回った場合はエラーとする */
				/* ロギング処理を行う */
				FMR_recErrLog(
					drvnum,                 /* ドライブ番号                   */
					DFMR_INITBYTE,          /* 断片ファイルの先頭クラスタ番号 */
					0,                      /* 断片ファイルのクラスタ数       */
					DFMR_SD_LOGERRRELINK,   /* 再帰リンク                     */
					entrypath,              /* 元ファイル名                   */
					logarea,                /* ログ出力領域                   */
					l_sdworkarea            /* ワークエリア                   */
					);
				
				return DFMR_SDSBROKENFILE;      /* 処理続行                   */
			}
		}
		
		/* クラスタ番号更新 */
		l_nowclst = l_fatentry;
		
		if( l_nowclst >= l_eocmin )     /* クラスタ番号がEOCの場合            */
		{
			break;                      /* BADクラスタチェック終了            */
		}
		
		/* クラスタ番号のFATエントリを取得 */
		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_nowclst,                  /* クラスタ番号                       */
			&l_fatentry,                /* FATエントリ値                      */
			DFMR_FLGOFF                 /* Read/Write種別  OFF:取得           */
			);
		if( l_rtn != DFMR_OK )          /* エラーの場合                       */
		{
			if( l_rtn == DFMR_CLSTNUMERR )
			{                           /* クラスタ番号異常の場合は異常終了   */
				return l_rtn;
			}
			if( l_badflg != DFMR_FLGOFF )   /* BADクラスタを検出済みの場合    */
			{
				/* 現在読込まれているFATセクタを書込む */
				l_rtn = FMR_setFatSectorRetry(
					drvnum,             /* ドライブ番号                       */
				    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				
				/* BADクラスタを未検出とする */
				l_badflg = DFMR_FLGOFF;
			}
			/* FATセクタを読込む */
			l_rtn = FMR_getFatSectorRetry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_nowclst,              /* クラスタ番号                       */
				&l_fatentry             /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
	}
	
	/* BADクラスタを途中で検出した場合 */
	if( l_badflg == DFMR_FLGON )
	{
		/* FAT読込み領域0を書込む */
		l_rtn = FMR_setFatSectorRetry(  /* FATセクタを書込む                  */
			drvnum,                     /* ドライブ番号                       */
		    DFMR_REFFATAREA0            /* FAT読込み領域番号                  */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	
	/* 破損箇所検出済みの場合 */
	if( l_brokenflg == DFMR_FLGON )
	{
		return DFMR_SDSBROKENFILE;
	}

	
	/* EOCを検出したクラスタ数とファイルサイズから */
	/* 割り出したクラスタ数が異なる場合            */
	if( l_clstcnt != l_clstnum )
	{
		/* ロギング処理を行う */
		FMR_recErrLog(
			drvnum,                     /* ドライブ番号                       */
			l_nowtopclst,               /* 断片ファイルの先頭クラスタ番号     */
			l_clstcnt,                  /* 断片ファイルのクラスタ数           */
			DFMR_SD_LOGERRMSIZE,        /* サイズエラー                       */
			entrypath,                  /* 元ファイル名                       */
			logarea,                    /* ログ出力領域                       */
			l_sdworkarea                /* ワークエリア                       */
			);
		
		return DFMR_SDSBROKENFILE;      /* 処理続行                           */
	}
	
	/* クラスタ使用状況をOFFとする */
	l_rtn = FMR_framChainClstBitOff(
		drvnum,                         /* ドライブ番号                       */
		topclstnum,                     /* 先頭クラスタ番号                   */
		l_eocmin,                       /* EOCコード                          */
		ramaddr                         /* クラスタ使用状況展開領域           */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/* 正常終了返却 */
	return DFMR_OK;
}

/* BADクラスタ/多重リンク発見時エラー処理 */
static F_INT FMR_scanFileDataErr(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD lastclst,                   /* 前クラスタ番号                     */
	F_DWORD eoc,                        /* EOCコード                          */
	F_DWORD topclst,                    /* 先頭クラスタ番号                   */
	F_DWORD clstcnt,                    /* クラスタ数                         */
	F_BYTE logerrcode,                  /* ログのエラーコード                 */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	TFMR_SDWORKAREA * sdworkarea        /* スキャンディスク用ワークエリア     */
)
{
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	
	if( lastclst != DFMR_INITBYTE )
	{                                   /* 前クラスタが特定されている場合     */
		/* 前クラスタのFATエントリをEOCに設定 */
		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			lastclst,                   /* クラスタ番号                       */
			&eoc,                       /* FATエントリ値(BADクラスタ)         */
			DFMR_FLGON                  /* Read/Write種別  ON:設定            */
			);
		
		if( l_rtn != DFMR_OK )          /* エラーの場合                       */
		{
			if( l_rtn == DFMR_CLSTNUMERR )
			{                           /* クラスタ番号異常の場合は異常終了   */
				return l_rtn;
			}
			/* FAT読込み領域1に前クラスタを含むFATセクタを読込む */
			l_rtn = FMR_getFatSectorRetry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
				                        /* FAT読込み領域                      */
				lastclst,               /* クラスタ番号                       */
				&l_fatentry             /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* 前クラスタのFATエントリをEOCに設定 */
			l_rtn = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
				                        /* FAT読込み領域                      */
				lastclst,               /* クラスタ番号                       */
				&eoc,                   /* FATエントリ値(BADクラスタ)         */
				DFMR_FLGON              /* Read/Write種別  ON:設定            */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* FAT読込み領域1を書込む */
			l_rtn = FMR_setFatSectorRetry(  /* FATセクタを書込む              */
				drvnum,                     /* ドライブ番号                   */
			    DFMR_REFFATAREA1            /* FAT読込み領域番号              */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
	}
	/* ロギング処理を行う */
	FMR_recErrLog(
		drvnum,                         /* ドライブ番号                       */
		topclst,                        /* 断片ファイルの先頭クラスタ番号     */
		clstcnt,                        /* 断片ファイルのクラスタ数           */
		logerrcode,                     /* エラー種別                         */
		entrypath,                      /* 元ファイル名                       */
		logarea,                        /* ログ出力領域                       */
		sdworkarea                      /* ワークエリア                       */
		);
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
