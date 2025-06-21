/*======================================================================
 * File name    : scandisk.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/api/RCS/scandisk.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scandisk.c $
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
/* 関 数 名：FMR_ScanDisk                                                     */
/*         ：                                                                 */
/* パラメタ：IN:  char ドライブ番号                                           */
/*         ：     unsigned char * クラスタ使用状況展開領域アドレス            */
/*         ：     unsigned long クラスタ使用状況展開領域サイズ                */
/*         ：     unsigned char * 破損ファイル復旧ログ出力領域アドレス        */
/*         ：     unsigned long 破損ファイル復旧ログ出力領域サイズ            */
/*         ：     unsigned char 破損ファイル復旧ログ動作指定フラグ            */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：システム内の全データクラスタの検索を行い、                       */
/*         ：破損ファイルの復旧及びBADクラスタのチェックを行う                */
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

int FMR_ScanDisk(
	char drvnum,                        /* ドライブ番号                       */
	unsigned char * ramaddr ,           /* クラスタ使用状況展開領域アドレス   */
	unsigned long ramsize,              /* クラスタ使用状況展開領域サイズ     */
	unsigned char * logarea,          /* 破損ファイル復旧ログ出力領域アドレス */
	unsigned long logsize,              /* 破損ファイル復旧ログ出力領域サイズ */
	unsigned char logflg                /* 破損ファイル復旧ログ動作指定フラグ */
)
{
	TFMR_SDWORKAREA * l_workarea;       /* スキャンディスク用ワークエリア     */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_ortn;                       /* エラー時セマフォ返却戻り値         */
	F_INT l_handle;                     /* ファイルハンドル番号               */
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_WORD l_entrypos;                  /* エントリ名称設定位置               */
	F_CHAR l_entrypath[ DFMR_MAXFILESZ ];   /* エントリ名格納領域             */
	F_BYTE l_bit;                       /* クラスタ使用状況Bit取得用          */
	
	TFMR_SDLOGTBLSHT * l_logtbls;       /* ログポインタ(フルパス非指定)       */
	TFMR_SDLOGTBLLNG * l_logtbll;       /* ログポインタ(フルパス指定)         */
	
	TFMR_SEARCHINFO l_toppos;           /* 断片ファイル格納ディレクトリ位置   */
	TFMR_DIRENTRY l_entry;              /* 同上エントリ                       */
	
	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	drvnum &=  0xDF;
	if( ( drvnum < DFMR_TOPDRVNUM ) || ( DFMR_ENDDRVNUM < drvnum ) )
	{                                   /* ドライブ番号無効値                 */
		return DFMR_ERRPARAM;           /* パラメータエラー                   */
	}
	if( MFMR_DRVINFO( drvnum ) == ( F_VOID * )DFMR_RAMINIT )
	{
		return DFMR_DRVNOCONNECTERR;    /* 論理ドライブ未接続エラー           */
	}
	l_work01 = ( MFMR_TOTALCLST( drvnum ) + DFMR_BITSIZE - 1 ) / DFMR_BITSIZE;
	l_work01 += sizeof( TFMR_SDWORKAREA );  /* スキャンディスク用ワークエリア */
	                                        /* のサイズを加算                 */

	if( ramsize < l_work01 )            /* クラスタ使用状況展開領域サイズが   */
	{                                   /* 足りない場合                       */
		return DFMR_ERRPARAM;           /* パラメータエラー                   */
	}
	
	/**************************************************************************/
	/* ロギングパラメータ確認/設定                                            */
	/**************************************************************************/
	l_workarea = ( TFMR_SDWORKAREA * )ramaddr;      /* RAM領域先頭をワーク    */
	                                                /* エリアに割り当てる     */
	l_workarea->mode = logflg;          /* 動作フラグを設定する               */
	if( logarea == NULL )               /* ログ出力領域がNULLの場合           */
	{
		l_workarea->mode &= ~DFMR_SD_LOGMODEON;
		                                /* ログ動作は行わない                 */
	}

	if( ( l_workarea->mode & DFMR_SD_LOGMODEON ) == DFMR_SD_LOGMODEON )
	{	                                /* ログ出力ありの場合                 */
		if( ( l_workarea->mode & DFMR_SD_LOGFULLPATH ) == DFMR_SD_LOGFULLPATH )
		{                               /* フルパス指定の場合                 */
			if( logsize < sizeof( TFMR_SDLOGTBLLNG ) )
			{                           /* ログ出力領域サイズがログ１件分未満 */
				                        /* の場合                             */
				return DFMR_ERRPARAM;   /* パラメータエラー                   */
			}
			l_workarea->logmax = logsize / sizeof( TFMR_SDLOGTBLLNG );
		}
		else                            /* ファイル名のみの場合               */
		{
			if( logsize < sizeof( TFMR_SDLOGTBLSHT ) )
			{
				return DFMR_ERRPARAM;   /* パラメータエラー                   */
			}
			l_workarea->logmax = logsize / sizeof( TFMR_SDLOGTBLSHT );
		}
		l_workarea->logcnt = DFMR_SD_LOGCNTINIT;/* ログカウンタ初期化         */
	}
	
	/**************************************************************************/
	/* 初期設定                                                               */
	/**************************************************************************/
	/* クラスタ使用状況展開領域をALL ONで初期化する */
	FMR_memSet(
		( ramaddr + sizeof( TFMR_SDWORKAREA ) ),
		                                /* クラスタ使用状況展開領域アドレス   */
		DFMR_SDBITALLON,                /* 初期値0xFF                         */
		( ramsize - sizeof( TFMR_SDWORKAREA ) )
		                                /* クラスタ使用状況展開領域サイズ     */
		);
	
	/* クラスタ0,1のBitをOFFとする */
	*( ramaddr + sizeof( TFMR_SDWORKAREA ) ) &= DFMR_SD_TOPCLSTBITOFF;
	/* エントリ名格納領域をALL 0で初期化する */
	FMR_memSet(
		&l_entrypath[ 0 ],              /* エントリ名格納領域アドレス         */
		DFMR_INITBYTE,                  /* 初期値0                            */
		DFMR_MAXFILESZ      	        /* エントリ名格納領域サイズ           */
		);
	/* ログ出力領域をALL0で初期化する */
	if( ( l_workarea->mode & DFMR_SD_LOGMODEON ) == DFMR_SD_LOGMODEON )
	{
		FMR_memSet(
			logarea,                  /* 破損ファイル復旧ログ出力領域アドレス */
			DFMR_INITBYTE,              /* 初期値0xFF                         */
			logsize                     /* 破損ファイル復旧ログ出力領域サイズ */
			);
	}
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_rtn =  FMR_osWaiSem(
		MFMR_SEMID( drvnum )
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/**************************************************************************/
	/* 未使用クラスタの検出を行う                                             */
	/**************************************************************************/
	l_rtn = FMR_pickTbdClst(
		drvnum,                         /* ドライブ番号                       */
		ramaddr                         /* クラスタ使用状況展開領域           */
		);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	
	/**************************************************************************/
	/* 未使用クラスタのBADクラスタチェックを行う                              */
	/**************************************************************************/
	l_rtn = FMR_scanTbdClst(
		drvnum,                         /* ドライブ番号                       */
		ramaddr                         /* クラスタ使用状況展開領域           */
		);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	
	/**************************************************************************/
	/* ルートディレクトリよりチェインされているクラスタの                     */
	/* BADクラスタチェックを行い、使用状況BitをOFFとする                      */
	/**************************************************************************/
	if( MFMR_FATTYPE( drvnum ) != DFMR_FATTYPE32 )
	{                                   /* FAT32以外の場合                    */
		/* ルートディレクトリよりチェインを辿る */
		l_rtn = FMR_scanEntryUnderRoot(
			drvnum,                      /* ドライブ番号                      */
			&l_entrypath[ 0 ],           /* エントリ名格納領域                */
			ramaddr,                     /* クラスタ使用状況展開領域          */
			logarea                      /* ログ出力領域                      */
			);
	}
	else                                 /* FAT32の場合                       */
	{
		/* ルートディレクトリよりチェインを辿る */
		l_work01 = FMR_getClstNum(       /* ワークエリア=先頭クラスタ番号     */
			drvnum,                      /* ドライブ番号                      */
			MFMR_ROOTTOP( drvnum )       /* セクタ番号                        */
			);
		l_entrypos = 0;                  /* エントリ名称設定位置を先頭とする  */
		l_rtn = FMR_scanEntry(
			&l_entrypath[ 0 ],           /* エントリ名格納領域                */
			ramaddr,                     /* クラスタ使用状況展開領域          */
			logarea,                     /* ログ出力領域                      */
			&l_work01,                   /* 先頭クラスタ番号                  */
			&l_entrypos,                 /* エントリ名設定先頭位置            */
			drvnum                       /* ドライブ番号                      */
			);
	}
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	/**************************************************************************/
	/* 破損ファイルの存在を確認する                                           */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < MFMR_TOTALCLST( drvnum ); l_cnt01 ++ )
	{
		l_bit = FMR_framBitChk(
			l_cnt01,                    /* クラスタ番号                       */
			ramaddr                     /* クラスタ使用状況展開領域           */
			);
		if( l_bit != DFMR_FLGOFF )
		{
			break;
		}
	}
	if( l_cnt01 != MFMR_TOTALCLST( drvnum ) )
	{                                   /* 破損ファイルが存在した場合         */
		/**********************************************************************/
		/* 断片ファイル格納ディレクトリを作成する                             */
		/**********************************************************************/
		/* ディレクトリの名称の拡張子(数値)部分以外を作成する */
		l_entrypath[ 0 ] = drvnum;
		l_entrypath[ 1 ] = ':';
		l_entrypath[ 2 ] = '\\';
		FMR_memCopy(
			&l_entrypath[ DFMR_SDDRVLEN ],
			DFMR_SDDIRNAME,
			DFMR_SDDIRNAMECHARLEN
			);
		
		l_entrypath[ DFMR_SDDRVLEN + DFMR_SDDIRNAMELEN ] = NULL;
		                                /* 最後はNULL終端                     */
		
		for( l_cnt01 = 0; l_cnt01 < DFMR_SDDIRMAX; l_cnt01 ++ )
		{
			/* 拡張子(数値)部分の作成 */
			/* 1の位 */
			l_work01 = l_cnt01 % 10;    /* カウンタの1の位取得                */
			l_entrypath[ DFMR_SDDRVLEN + DFMR_SDDIRNAMECHARLEN + 2 ]
				= ( F_BYTE )( l_work01 + '0' );
			
			/* 10の位 */
			l_work01 = l_cnt01 % 100;
			l_work01 = l_work01 / 10;   /* カウンタの10の位取得               */
			l_entrypath[ DFMR_SDDRVLEN + DFMR_SDDIRNAMECHARLEN + 1 ]
				= ( F_BYTE )( l_work01 + '0' );
			
			/* 100の位 */
			l_work01 = l_cnt01 % 1000;
			l_work01 = l_work01 / 100;  /* カウンタの100の位取得              */
			l_entrypath[ DFMR_SDDRVLEN + DFMR_SDDIRNAMECHARLEN ]
				= ( F_BYTE )( l_work01 + '0' );
			
			/* ディレクトリを作成する */
			l_rtn = FMR_subMkdir(
				&l_entrypath[ 0 ],
				drvnum
				);
			if( l_rtn == DFMR_OK )      /* 正常終了の場合は断片ファイル作成を */
			{                           /* 行う                               */
				break;
			}
			else if( ( l_rtn == DFMR_NMERR ) || ( l_rtn == DFMR_EEXIST ) )
			{                           /* 同名ファイル/ディレクトリ有りの    */
			                            /* 場合は処理続行                     */
				continue;
			}
			else                        /* 上記以外の場合はエラーを返却       */
			{
				if( l_rtn == DFMR_MEMFULL )
				{                       /* メモリフルの場合はエラーコードを   */
				                        /* 断片ファイル作成不可に変更         */
					l_rtn = DFMR_SDROOTENTFULL;
				}
				goto ERROR;
			}
		}
		if( l_cnt01 >= DFMR_SDDIRMAX )  /* カウンタが最大件数以上となった     */
		{                               /* 場合は断片ファイル作成不可とする   */
			l_rtn = DFMR_SDROOTENTFULL;
			goto ERROR;
		}
		
		/* 断片ファイル格納ディレクトリのエントリを取得する */
		l_toppos.sectnum = MFMR_ROOTTOP( drvnum );      /* 検索開始位置を     */
		l_toppos.slot = DFMR_SRCHROOTTOPSLOT;           /* 設定する           */
		l_toppos.bufnum = DFMR_FLGOFF;

		l_rtn =  FMR_readSectRetry(     /* R/Wバッファ0にルート先頭セクタを   */
		                                /* 読込む                             */
			drvnum,                     /* ドライブ番号                       */
			l_toppos.sectnum,           /* セクタ番号                         */
			DFMR_FLGOFF                 /* R/Wバッファ番号 = 0                */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
		
		l_rtn = FMR_searchEntry(
			drvnum,                         /* 対象ドライブ番号               */
			&l_toppos,                      /* 検索開始/終了先頭位置          */
			NULL,                           /* 検索終了最終位置               */
			&l_entrypath[ DFMR_SDDRVLEN ],  /* 検索文字列                     */
			&l_entry,                       /* 取得エントリ                   */
			NULL,                           /* 取得ロングファイル名           */
			DFMR_SDDIRNAMELEN,              /* 検索文字列長                   */
			0x01                            /* 動作フラグ:検索エントリ数=1    */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
		
		/* 取得したエントリより先頭クラスタ番号を取得する */
		l_work01 = FMR_getTopClstNum(   /* ワークエリア=先頭クラスタ番号      */
			&l_entry                    /* ディレクトリエントリ               */
			);
		
		/**********************************************************************/
		/* 断片ファイルを作成する                                             */
		/**********************************************************************/
		l_rtn = FMR_makeScrapFile(
			drvnum,                     /* ドライブ番号                       */
			l_work01,                   /* 先頭クラスタ番号                   */
			ramaddr,                    /* クラスタ使用状況展開領域           */
			logarea                     /* ログ出力領域                       */
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
	}
	if( ( ( l_workarea->mode )
		& ( DFMR_SD_LOGMODEON | DFMR_SD_LOGWRTFILE ) )
		== ( DFMR_SD_LOGMODEON | DFMR_SD_LOGWRTFILE ) )
	                                    /* ログファイル出力指定の場合         */
	{
		l_logtbll = ( TFMR_SDLOGTBLLNG *  )logarea;
		                                /* 先頭ログのエラーコード取得のための */
		                                /* キャスト                           */
		                                /* (位置はフルパスON/OFF共同じ)       */
		if( l_logtbll->errcode != DFMR_INITBYTE )
		{                               /* ロギングが行われていた場合         */
			/******************************************************************/
			/* ログファイルの作成を行う                                       */
			/******************************************************************/
			/* ファイルサイズを求める */
			if( ( ( l_workarea->mode ) & DFMR_SD_LOGFULLPATH )
				== DFMR_SD_LOGFULLPATH )
			{                           /* フルパス指定時                     */
				l_logtbll = ( TFMR_SDLOGTBLLNG *  )logarea;
				for( l_cnt01 = 0; l_cnt01 < l_workarea->logmax; l_cnt01 ++ )
				{
					if( l_logtbll->errcode == DFMR_INITBYTE )
					{
						break;
					}
					l_logtbll ++;
				}
				l_cnt01 *= sizeof( TFMR_SDLOGTBLLNG );
			}
			else
			{                           /* ファイル名のみの場合               */
				l_logtbls = ( TFMR_SDLOGTBLSHT *  )logarea;
				for( l_cnt01 = 0; l_cnt01 < l_workarea->logmax; l_cnt01 ++ )
				{
					if( l_logtbls->errcode == DFMR_INITBYTE )
					{
						break;
					}
					l_logtbls ++;
				}
				l_cnt01 *= sizeof( TFMR_SDLOGTBLSHT );
			}
			
			if( l_cnt01 == 0 )          /* ログが0件の場合はログ出力は        */
			{                           /* 行わない                           */
				l_rtn = DFMR_SUC;       /* 戻り値に正常終了を設定する         */
				goto ERROR;
			}
			l_entrypath[ 0 ] = drvnum;
			l_entrypath[ 1 ] = ':';
			l_entrypath[ 2 ] = '\\';
			FMR_memCopy(
				&l_entrypath[ DFMR_SDDRVLEN ],
				DFMR_SDLOGNAME,
				DFMR_SDLOGNAMELEN
				);
			l_entrypath[ DFMR_SDLOGNAMELEN + DFMR_SDDRVLEN ] = NULL;
			l_handle = FMR_subOpen ( 
				&l_entrypath[ 0 ],              /* 対象ファイルパス名         */
				( DFMR_O_CREAT | DFMR_O_RDWR),  /* オープンフラグ             */
				DFMR_ATTR_ARC,                  /* 属性                       */
				drvnum
				);
			if( l_handle < 0 )
			{
				l_rtn = DFMR_SDLOGOUTERR;
				goto ERROR;
			}
			
			l_rtn = FMR_subWrite(
				l_handle,               /* ファイルハンドル                   */
				logarea,                /* 書込みデータ格納領域               */
				l_cnt01                 /* 書込みサイズ                       */
				);
			FMR_initFileHandle( 
			    l_handle                /* ファイルハンドル番号               */
			);
			if( l_rtn < 0 )
			{
				l_rtn = DFMR_SDLOGOUTERR;
				goto ERROR;
			}
		}
	}
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_rtn =  FMR_osSigSem(
		MFMR_SEMID( drvnum )
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	return DFMR_SUC;                    /* 正常終了                           */
	
ERROR:
	l_ortn =  FMR_osSigSem(
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
