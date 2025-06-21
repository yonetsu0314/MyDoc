/*======================================================================
 * File name    : scrpfile.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/scrpfile.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scrpfile.c $
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
/* 関 数 名：FMR_makeScrapFile                                                */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：     F_BYTE * ログ出力領域                                       */
/*         ：     F_WORD 最大ログ件数                                         */
/*         ：                                                                 */
/* 戻 り 値：DFMR_OK 正常終了                                                 */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：断片ファイルを作成する                                           */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

/* 断片ファイルの名称の数値部を作成する */
static F_VOID FMR_setScrapFileNameNum(
	F_CHAR * name,                      /* ファイル名格納領域                 */
	F_WORD cnt                          /* 名称数値部の数                     */
);

/* ファイル名をエントリのファイル/拡張子形式に変換する */
static F_VOID FMR_setFileName(
	F_CHAR * orgname,                   /* ファイル(+拡張子)名                */
	F_CHAR * entryfrm                   /* エントリ形式                       */
);

/* クラスタ使用状況展開領域よりBIT ONのクラスタ番号を取得する */
static F_DWORD FMR_getNoCheckClst(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_DWORD l_startclstnum              /* 検索開始クラスタ番号               */
);

/* ディレクトリの拡張を行う */
static F_INT FMR_expandDirClst(
	F_INT drvnum,                       /* ドライブ番号                       */
	F_DWORD lastclstnum,                /* 最終クラスタ番号                   */
	F_DWORD * expclstnum                /* 拡張したクラスタ番号               */
);

/* 指定最終クラスタ番号を先頭クラスタとするエントリを検索する */
/* エントリが存在した場合は先頭クラスタ番号、サイズを更新する */
static F_INT FMR_reconfigScrapFileEntry(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD ssectnum,                   /* 検索開始セクタ番号                 */
	F_BYTE sslot,                       /* 検索開始セクタ内位置               */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD btmclstnum,                 /* 最終クラスタ番号                   */
	F_WORD clstcnt                      /* クラスタ数                         */
);

F_INT FMR_makeScrapFile(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
)
{
	TFMR_DIRENTRY * l_entry;            /* エントリ書込み領域ポインタ         */
	TFMR_SDLOGTBLSHT * l_logtbls;       /* ログポインタ(フルパス非指定)       */
	TFMR_SDLOGTBLLNG * l_logtbll;       /* ログポインタ(フルパス指定)         */
	TFMR_SDWORKAREA * l_sdworkarea;     /* スキャンディスク用ワークエリア     */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_clstnum;                  /* 書込み対象クラスタ番号             */
	F_DWORD l_emptyclstnum;             /* 取得空きクラスタ番号               */
	F_DWORD l_sectnum;                  /* 書込み対象セクタ番号               */
	F_DWORD l_logsectnum;               /* ログ断片ファイル書込みセクタ番号   */
	F_DWORD l_topclstnum;               /* 断片ファイルの先頭クラスタ番号     */
	F_DWORD l_dataclstnum;              /* 断片ファイルのデータ部クラスタ番号 */
	F_DWORD l_fdataclstnum;             /* 断片ファイルの前クラスタ番号       */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_DWORD l_size;                     /* ファイルサイズ                     */
	F_DWORD l_eoc;                      /* EOCコード                          */
	F_DWORD l_eocmin;                   /* EOCコード(比較用                   */
	F_DWORD l_bad;                      /* BADクラスタコード                  */
	F_DWORD l_fatarea;                  /* 対象FAT読込み領域                  */
	F_DWORD l_div;                      /* 計算用                             */
	F_WORD l_cnt;                       /* 名称数値部カウンタ                 */
	F_WORD l_clstcnt;                   /* 断片ファイルのクラスタ数カウンタ   */
	F_WORD l_logcnt;                    /* ログ断片ファイル数カウンタ         */
	F_CHAR l_name[ DFMR_FNAMESZ + DFMR_EXTSZ + 1 ];
	                                    /* 名称バッファ(名称+拡張子+｢.｣)      */
	F_BYTE l_slot;                      /* エントリ格納位置                   */
	F_BYTE l_logslot;                   /* ログ断片ファイルエントリ格納位置   */
	F_BYTE l_fatflg;                    /* クラスタ使用状況                   */
	
	/* EOC/BADクラスタコードを取得する */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_eoc );
	MFMR_GETEOCMIN( MFMR_FATTYPE( drvnum ), l_eocmin );
	MFMR_GETBAD( MFMR_FATTYPE( drvnum ), l_bad );
	
	/* ワークエリアを取得する */
	l_sdworkarea = ( TFMR_SDWORKAREA * )ramaddr;
	
	if( ( ( l_sdworkarea->mode ) & DFMR_SD_LOGMODEON ) == DFMR_SD_LOGMODEON )
	{                                   /* ロギング動作ONの場合               */
		/* ログ出力領域にポインタをキャストする */
		if( ( ( l_sdworkarea->mode ) & DFMR_SD_LOGFULLPATH )
			== DFMR_SD_LOGFULLPATH )    /* フルパス指定の場合                 */
		{
			l_logtbll = ( TFMR_SDLOGTBLLNG * )logarea;
		}
		else                            /* ファイル名のみの場合               */
		{
			l_logtbls = ( TFMR_SDLOGTBLSHT *  )logarea;
		}
	}
	
	/* クラスタ番号を取得する */
	l_clstnum = topclstnum;
	
	/* ファイル名を初期化する */
	FMR_memCopy(
		&l_name[ 0 ],                   /* 名称バッファ                       */
		DFMR_SDFILENAME,                /* 断片ファイル名称初期値             */
		DFMR_FNAMESZ + DFMR_EXTSZ + 1   /* バッファサイズ                     */
		);
	
	/* 先頭クラスタの先頭セクタを読込んでおく */
	l_sectnum = FMR_getSectNum(         /* クラスタの先頭セクタを取得する     */
		l_clstnum,                      /* クラスタ番号                       */
		0,                              /* クラスタ内セクタ番号               */
		drvnum                          /* ドライブ番号                       */
		);
		
	l_rtn = FMR_readSectRetry(          /* クラスタの先頭セクタを読込む       */
		drvnum,                         /* ドライブ番号                       */
		l_sectnum,                      /* セクタ番号                         */
		DFMR_FLGOFF                     /* R/Wバッファ番号=0                  */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
	/* 読込んだセクタの先頭位置をエントリ格納位置とする */
	l_slot = DFMR_SRCHDIRTOPSLOT;       /* エントリ格納位置設定               */
	l_entry = ( TFMR_DIRENTRY * )&( MFMR_RWBUFF( drvnum, 0,
		 l_slot * sizeof( TFMR_DIRENTRY ) ) );  /* ポインタをキャスト         */
	
	/* ログとして出力する分の断片ファイルを作成する */
	for( l_cnt = 0; l_cnt < l_sdworkarea->logmax; l_cnt ++ )
	{
		if( ( ( l_sdworkarea->mode ) & DFMR_SD_LOGMODEON ) 
			!= DFMR_SD_LOGMODEON )
		{                               /* ロギング動作OFFの場合              */
			break;
		}
		if( ( ( l_sdworkarea->mode ) & DFMR_SD_LOGFULLPATH )
			== DFMR_SD_LOGFULLPATH )    /* フルパス指定の場合                 */
		{
			if( l_logtbll->errcode == DFMR_INITBYTE )
			{                           /* ログ出力分のエントリの書込みが     */
				                        /* 終了した場合                       */
				break;
			}
			if( ( l_logtbll->errcode == DFMR_SD_LOGERRTOPILL )
				|| ( l_logtbll->errcode == DFMR_SD_LOGERRLFNILL )
				|| ( l_logtbll->errcode == DFMR_SD_LOGERRNOTOP )
				|| ( l_logtbll->errcode == DFMR_SD_LOGERRRELINK )
				|| ( l_logtbll->errcode == DFMR_SD_LOGERRTAILOFF )
				|| ( l_logtbll->orgtopclstnum == DFMR_INITBYTE )
			)
			{                           /* エラーコードが不正なディレクトリの */
			                            /* 場合                               */
				l_logtbll ++;
				continue;
			}
		}
		else                            /* フルパス非指定の場合               */
		{
			if( l_logtbls->errcode == DFMR_INITBYTE )
			{                           /* ログ出力分のエントリの書込みが     */
				                        /* 終了した場合                       */
				break;
			}
			if( ( l_logtbls->errcode == DFMR_SD_LOGERRTOPILL )
				|| ( l_logtbls->errcode == DFMR_SD_LOGERRLFNILL )
				|| ( l_logtbls->errcode == DFMR_SD_LOGERRNOTOP )
				|| ( l_logtbls->errcode == DFMR_SD_LOGERRRELINK )
				|| ( l_logtbls->errcode == DFMR_SD_LOGERRTAILOFF )
				|| ( l_logtbls->orgtopclstnum == DFMR_INITBYTE )
			)
			{                           /* エラーコードが不正なディレクトリの */
			                            /* 場合                               */
				l_logtbls ++;
				continue;
			}
		}
		/* 通番のファイル名を作成する */
		FMR_setScrapFileNameNum(
			&l_name[ 0 ],               /* ファイル名格納領域                 */
			l_cnt                       /* 名称数値部の数                     */
			);
		
		/* エントリを作成する */
		if( ( ( l_sdworkarea->mode ) & DFMR_SD_LOGFULLPATH )
			== DFMR_SD_LOGFULLPATH )    /* フルパス指定の場合                 */
		{
			l_topclstnum = l_logtbll->orgtopclstnum;
			                            /* 先頭クラスタ番号取得               */
			l_size = l_logtbll->orgclstnum      /* ファイルサイズ取得         */
				* MFMR_SECTPERCLST( drvnum )
				* MFMR_BYTEPERSECT( drvnum );
			
			/* ログの断片ファイル名称を設定する */
			FMR_setFileName(
				&l_name[ 0 ],           /* ファイル(+拡張子)名                */
				l_logtbll->pcename      /* エントリ形式                       */
				);
			
			/* ログポインタを更新 */
			l_logtbll ++;
		}
		else                            /* フルパス非指定の場合               */
		{
			l_topclstnum = l_logtbls->orgtopclstnum;
			                            /* 先頭クラスタ番号取得               */
			l_size = l_logtbls->orgclstnum      /* ファイルサイズ取得         */
				* MFMR_SECTPERCLST( drvnum )
				* MFMR_BYTEPERSECT( drvnum );
			
			/* ログの断片ファイル名称を設定する */
			FMR_setFileName(
				&l_name[ 0 ],           /* ファイル(+拡張子)名                */
				l_logtbls->pcename      /* エントリ形式                       */
				);
			
			/* ログポインタを更新 */
			l_logtbls ++;
		}
		/* エントリを作成する */
		FMR_makeEntry(
			l_entry,                    /* エントリイメージ作成領域           */
			&l_name[ 0 ],               /* ファイル名称                       */
			DFMR_ATTR_ARC,              /* 属性:アーカイブ                    */
			l_topclstnum,               /* 先頭クラスタ番号                   */
			l_size                      /* ファイルサイズ                     */
			);
		
		/* 作成した断片ファイルのクラスタの使用状況をOFFにする */
		l_rtn = FMR_framChainClstBitOff(
			drvnum,                     /* ドライブ番号                       */
			l_topclstnum,               /* 先頭クラスタ番号                   */
			l_eocmin,                   /* EOCコード                          */
			ramaddr                     /* クラスタ使用状況展開領域           */
			);
		
		/* エントリ書込み領域更新 */
		l_entry ++;                     /* ポインタ更新                       */
		l_slot ++;                      /* エントリ格納位置更新               */
		
		l_div = MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY );
		if( l_slot >= ( F_BYTE )l_div ) /* エントリ格納位置がセクタサイズを   */
		{                               /* 超えた場合                         */
			/* 対象セクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGOFF             /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			
			/* エントリ書込み領域更新 */
			l_entry = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( drvnum, 0 ) );
			                            /* ポインタ更新                       */
			l_slot = DFMR_SRCHROOTTOPSLOT;
			                            /* エントリ格納位置更新               */
			/* セクタ番号を更新 */
			if( l_sectnum >= FMR_getLastSectNum(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum               /* セクタ番号                         */
				)
			)
			{
				/* ディレクトリの拡張を行う */
				l_rtn = FMR_expandDirClst(
					drvnum,             /* ドライブ番号                       */
					l_clstnum,          /* アクセス中のクラスタ番号           */
					&l_emptyclstnum     /* 取得するクラスタ番号               */
					);
				if( l_rtn != DFMR_OK )
				{
					if( l_rtn == DFMR_SDROOTENTFULL )
					{                   /* メモリフルの場合                   */
						l_rtn = DFMR_OK;/* 退避を行うのは可能分まで           */
					}
					return l_rtn;
				}
				l_clstnum = l_emptyclstnum;
				                        /* 取得したクラスタ番号を書込み       */
				                        /* 対象クラスタ番号とする             */
			}
		}
	}
	/* ロギングされた断片ファイルのエントリ格納位置を保存しておく */
	l_logsectnum = l_sectnum;           /* 書込み対象セクタ番号               */
	l_logslot = l_slot;                 /* エントリ格納位置                   */
	l_logcnt = l_cnt;                   /* ログ件数                           */


	/* クラスタ使用状況の残りの破損ファイルを断片化する */
	while( 1 )                          /* クラスタ使用状況が全てOFFとなるまで*/
	{
		/* クラスタ使用状況展開領域よりBIT ONのクラスタ番号を取得する */
		l_topclstnum = FMR_getNoCheckClst(
			drvnum,                     /* ドライブ番号                       */
			ramaddr,                    /* クラスタ使用状況展開領域           */
			DFMR_TOPCLSTNUM             /* 検索開始クラスタ番号               */
			);
		if( l_topclstnum == DFMR_INITBYTE )
		{                               /* クラスタ使用状況がONのクラスタが   */
		                                /* 存在しない場合                     */
			/* 断片ファイルのエントリ登録セクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGOFF             /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			return DFMR_OK;             /* 正常終了とする                     */
		}
		/* 取得したクラスタ番号よりチェインを辿る */
		l_dataclstnum = l_topclstnum;
		l_clstcnt = 0;
		l_fdataclstnum = DFMR_INITBYTE;
		while( 1 )
		{
			/* 対象クラスタの使用状況を確認する */
			l_fatflg = FMR_framBitChk(  /* クラスタ使用状況のBitの確認を行う  */
				l_dataclstnum,          /* クラスタ番号                       */
				ramaddr                 /* クラスタ使用状況展開領域           */
				);
			
			if( l_fatflg == DFMR_FLGOFF )   /* 対象クラスタのBitがOFFの場合   */
			{
				break;
			}
			
			/* 対象クラスタのBADクラスタチェックを行う */
			l_rtn = FMR_chkBadClst(
				drvnum,                 /* ドライブ番号                       */
				l_dataclstnum,          /* クラスタ番号                       */
				DFMR_FLGON,             /* R/Wバッファ番号 = 1                */
				DFMR_FLGON              /* 対象クラスタ判別フラグ             */
				                        /*  = ON :使用中のクラスタ            */
				);
			
			if( l_rtn != DFMR_OK )      /* 対象クラスタがBADクラスタ          */
			{
				if( l_fdataclstnum != DFMR_INITBYTE )
				{                       /* 前クラスタが存在する場合           */
					/* 前クラスタを含むFATセクタを読込む */
					l_rtn = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                /* FAT読込み領域                      */
						l_fdataclstnum, /* クラスタ番号                       */
						&l_fatentry     /* FATエントリ値                      */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_SDSCRAPERR;
						                /* 断片ファイル作成時エラー発生       */
					}
				}
				else
				{                       /* 前クラスタが存在しない場合         */
					MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 ) = DFMR_INITBYTE;
				}
				
				/* 対象クラスタを含むFATセクタを読込む */
				l_fatarea = DFMR_REFFATAREA1;
				l_rtn = FMR_getFatSectorRetry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, l_fatarea ),
					                    /* FAT読込み領域                      */
					l_dataclstnum,      /* クラスタ番号                       */
					&l_fatentry         /* FATエントリ値                      */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生   */
				}
				if( MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 )
					== MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA1 ) )
				{                       /* 前/対象クラスタが同一FATセクタに   */
				                        /* 属する場合                         */
					l_fatarea = DFMR_REFFATAREA0;
				}
				/* 対象クラスタのFATエントリをBADクラスタとする */
				l_rtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, l_fatarea ),
					                    /* FAT読込み領域                      */
					l_dataclstnum,      /* クラスタ番号                       */
					&l_bad,             /* FATエントリ値                      */
					DFMR_FLGON          /* Read/Write種別 ON:設定             */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生   */
				}
				if( l_fatarea != DFMR_REFFATAREA0 )
				{
					/* 対象クラスタを含むFATセクタを書込む */
					l_rtn = FMR_setFatSectorRetry(
						drvnum,         /* ドライブ番号                       */
						l_fatarea       /* FAT読込み領域番号                  */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_SDSCRAPERR;
						                /* 断片ファイル作成時エラー発生       */
					}
				}
				
				/* 対象クラスタのクラスタ使用状況をOFFとする */
				FMR_framBitOff(
					l_dataclstnum,      /* クラスタ番号                       */
					ramaddr             /* クラスタ使用状況展開領域           */
					);
				
				if( l_fdataclstnum != DFMR_INITBYTE )
				{                       /* 前クラスタが存在する場合           */
					/* 前クラスタのFATエントリをEOCとする */
					l_rtn = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                /* FAT読込み領域                      */
						l_fdataclstnum, /* クラスタ番号                       */
						&l_eoc,         /* FATエントリ値                      */
						DFMR_FLGON      /* Read/Write種別 ON:設定             */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_SDSCRAPERR;
						                /* 断片ファイル作成時エラー発生       */
					}
					/* 前クラスタを含むFATセクタを書込む */
					l_rtn = FMR_setFatSectorRetry(
						drvnum,         /* ドライブ番号                       */
						DFMR_REFFATAREA0/* FAT読込み領域番号                  */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_SDSCRAPERR;
						                /* 断片ファイル作成時エラー発生       */
					}
				}
				break;
			}
			
			/* 対象クラスタのクラスタ使用状況をOFFとする */
			FMR_framBitOff(
				l_dataclstnum,          /* クラスタ番号                       */
				ramaddr                 /* クラスタ使用状況展開領域           */
				);
			/* 対象クラスタのFATエントリを取得する */
			l_rtn = FMR_getFatSectorRetry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_dataclstnum,          /* クラスタ番号                       */
				&l_fatentry             /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR;
				                        /* 断片ファイル作成時エラー発生       */
			}
			l_fdataclstnum = l_dataclstnum;
			l_dataclstnum = l_fatentry;
			l_clstcnt ++;
			if( l_dataclstnum >= l_eocmin )
			{
				break;
			}
		}
		if( l_fatflg == DFMR_FLGOFF )   /* 取得したクラスタのチェインの最終   */
		{                               /* FATエントリ番号がEOC以外の場合     */
			/* R/Wバッファ0を元のセクタに書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGOFF             /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			
			/* 取得した最終クラスタを先頭クラスタ番号とするエントリを検索する */
			l_rtn = FMR_reconfigScrapFileEntry(
				drvnum,                 /* ドライブ番号                       */
				l_logsectnum,           /* 検索開始セクタ番号                 */
				l_logslot,              /* 検索開始セクタ内位置               */
				l_topclstnum,           /* 先頭クラスタ番号                   */
				l_dataclstnum,          /* 最終クラスタ番号                   */
				l_clstcnt               /* クラスタ数                         */
				);
			if( l_rtn == DFMR_OK )      /* 対象のエントリが存在した場合       */
			{
				/* R/Wバッファ0に元のセクタを読込む */
				l_rtn = FMR_readSectRetry(
					drvnum,                 /* ドライブ番号                       */
					l_sectnum,              /* セクタ番号                         */
					DFMR_FLGOFF             /* R/Wバッファ番号                    */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
				}
				continue;               /* 処理続行                           */
			}
			else if( l_rtn == DFMR_NOENTRY )
			{                           /* 対象のエントリが存在しない場合     */
				/* 最終クラスタのFATエントリをEOCとする */
				/* 最終クラスタを含むFATセクタを読込む */
				l_rtn = FMR_getFatSectorRetry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_fdataclstnum,     /* クラスタ番号                       */
					&l_fatentry         /* FATエントリ値                      */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生   */
				}
				
				/* 最終クラスタのFATエントリをEOCとする */
				l_rtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_fdataclstnum,     /* クラスタ番号                       */
					&l_eoc,             /* FATエントリ値                      */
					DFMR_FLGON          /* Read/Write種別 ON:設定             */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生   */
				}
				
				/* 最終クラスタを含むFATセクタを書込む */
				l_rtn = FMR_setFatSectorRetry(
					drvnum,             /* ドライブ番号                       */
					DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_rtn != DFMR_OK )
				{
					return DFMR_SDSCRAPERR;
					                    /* 断片ファイル作成時エラー発生       */
				}
			}
		}
		
		/* データ部サイズが0の場合(取得した先頭クラスタがBADクラスタの */
		/* 場合に起こる)はエントリを作成せずに処理を続行               */
		if( l_clstcnt == 0 )
		{
			continue;
		}
		
		/* 断片ファイルのエントリを作成する */
		/* 通番のファイル名を作成する */
		FMR_setScrapFileNameNum(
			&l_name[ 0 ],               /* ファイル名格納領域                 */
			l_cnt                       /* 名称数値部の数                     */
			);
		/* 対象の位置に断片ファイルのエントリ書込む */
		l_size = l_clstcnt                  /* クラスタ数よりファイル         */
			* MFMR_SECTPERCLST( drvnum )    /* サイズを求める                 */
			* MFMR_BYTEPERSECT( drvnum );
		
		FMR_makeEntry(                  /* エントリを作成する                 */
			l_entry,                    /* エントリイメージ作成領域           */
			&l_name[ 0 ],               /* ファイル名称                       */
			DFMR_ATTR_ARC,              /* 属性:アーカイブ                    */
			l_topclstnum,               /* 先頭クラスタ番号                   */
			l_size                      /* ファイルサイズ                     */
			);

		/* 名称数値部カウンタを更新する */
		l_cnt ++;
		
		if( l_cnt >= DFMR_SDFILEMAX )
		{                               /* 破損ファイルが10000件以上と        */
		                                /* なった場合                         */
			/* 最後にエントリを書込んだセクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGOFF             /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			return DFMR_OK;             /* 正常終了返却                       */
		}
		
		/* エントリ書込み領域更新 */
		l_entry ++;                     /* ポインタ更新                       */
		l_slot ++;                      /* エントリ格納位置更新               */
		
		l_div = MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY );
		if( l_slot >= ( F_BYTE )l_div ) /* エントリ格納位置がセクタサイズを   */
		{                               /* 超えた場合                         */
			/* 対象セクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGOFF             /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			
			/* エントリ書込み領域更新 */
			l_entry = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( drvnum, 0 ) );
			                            /* ポインタ更新                       */
			l_slot = DFMR_SRCHROOTTOPSLOT;
			                            /* エントリ格納位置更新               */
			/* セクタ番号を更新 */
			l_div = FMR_getLastSectNum(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum               /* セクタ番号                         */
				);
			if( l_sectnum >= l_div )
			{
				/* ディレクトリの拡張を行う */
				l_rtn = FMR_expandDirClst(
					drvnum,             /* ドライブ番号                       */
					l_clstnum,          /* アクセス中のクラスタ番号           */
					&l_emptyclstnum     /* 取得するクラスタ番号               */
					);
				if( l_rtn != DFMR_OK )
				{
					if( l_rtn == DFMR_SDROOTENTFULL )
					{                   /* メモリフルの場合                   */
						l_rtn = DFMR_OK;/* 退避を行うのは可能分まで           */
					}
					return l_rtn;
				}
				l_clstnum = l_emptyclstnum;
				                        /* 取得したクラスタ番号を書込み       */
				                        /* 対象クラスタ番号とする             */
				/* 取得したクラスタの先頭セクタ番号を取得する */
				l_sectnum =  FMR_getSectNum(
					l_clstnum,          /* クラスタ番号                       */
					0,                  /* クラスタ内セクタ番号               */
					drvnum              /* ドライブ番号                       */
					);
			}
			else
			{
				l_sectnum ++;
			}
			/* R/Wバッファを0で初期化する */
			FMR_memSet(
				MFMR_RWBUFFTOP( drvnum, 0 ),
				DFMR_INITBYTE,
				MFMR_BYTEPERSECT( drvnum )
				);
		}
	}
}

/* 断片ファイルの名称の数値部を作成する */
static F_VOID FMR_setScrapFileNameNum(
	F_CHAR * name,                      /* ファイル名格納領域                 */
	F_WORD cnt                          /* 名称数値部の数                     */
)
{
	F_DWORD l_fig;                      /* 桁の数値取得用領域                 */
	
	/* 1の位 */
	l_fig = cnt % 10;                   /* カウンタの1の位取得                */
	*( name + DFMR_FNAMESZ - 1 ) = ( F_BYTE )( l_fig + '0' );
	
	/* 10の位 */
	l_fig = cnt % 100;
	l_fig = l_fig / 10;                 /* カウンタの10の位取得               */
	*( name + DFMR_FNAMESZ - 2 ) = ( F_BYTE )( l_fig + '0' );
	
	/* 100の位 */
	l_fig = cnt % 1000;
	l_fig = l_fig / 100;                /* カウンタの100の位取得              */
	*( name + DFMR_FNAMESZ - 3 ) = ( F_BYTE )( l_fig + '0' );
	
	/* 1000の位 */
	l_fig = cnt % 10000;
	l_fig = l_fig / 1000;               /* カウンタの1000の位取得             */
	*( name + DFMR_FNAMESZ - 4 ) = ( F_BYTE )( l_fig + '0' );
}

/* ファイル名をエントリのファイル/拡張子形式に変換する */
static F_VOID FMR_setFileName(
	F_CHAR * orgname,                   /* ファイル(+拡張子)名                */
	F_CHAR * entryfrm                   /* エントリ形式                       */
)
{
	F_BYTE l_cnt;                       /* ループカウンタ                     */
	
	/* ファイル名を設定する */
	for( l_cnt = 0; l_cnt < DFMR_FNAMESZ; l_cnt ++ )    /* ファイル名設定     */
	{                                   /* 終了条件①：NULL終端               */
	                                    /*         ②：8文字                  */
	                                    /*         ③：｢.｣検出                */
		if( ( *( orgname ) == NULL )
			|| ( *( orgname ) == '.' ) )
		{
			FMR_memSet(                 /* 残りをスペースで埋める             */
				( entryfrm + l_cnt ),
				DFMR_SPACE,
				( DFMR_FNAMESZ - l_cnt )
				);
			break;
		}
		
		*( entryfrm + l_cnt ) = *( orgname );
		orgname ++;
	}
	
	if( *( orgname ) == '.' )
	{                                   /* 拡張子ありの場合                   */
		orgname ++;                     /* ｢.｣を読み飛ばす                    */
		entryfrm += DFMR_FNAMESZ;       /* 格納先を拡張子の領域とする         */
		for( l_cnt = 0; l_cnt < DFMR_EXTSZ; l_cnt ++ )  /* 拡張子設定         */
		{                               /* 終了条件①：NULL終端               */
		                                /*         ②：3文字                  */
			if( *( orgname ) == NULL )
			{
				FMR_memSet(             /* 残りをスペースで埋める             */
					( entryfrm + l_cnt ),
					DFMR_SPACE,
					( DFMR_EXTSZ - l_cnt )
					);
				break;
			}
			
			*( entryfrm + l_cnt ) = *( orgname );
			orgname ++;
		}
	}
	return;
}

/* クラスタ使用状況展開領域よりBIT ONのクラスタ番号を取得する */
static F_DWORD FMR_getNoCheckClst(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_DWORD l_startclstnum              /* 検索開始クラスタ番号               */
)
{
	F_DWORD l_cnt;                      /* ループカウンタ                     */
	
	for( l_cnt = l_startclstnum; l_cnt < MFMR_TOTALCLST( drvnum ); l_cnt ++ )
	{
		/* ループカウンタが示すクラスタの使用状況を取得する */
		if( DFMR_FLGON == FMR_framBitChk( l_cnt, ramaddr ) )
		{
			return l_cnt;
		}
	}
	return DFMR_INITBYTE;               /* 未確認のクラスタが存在しない場合は */
	                                    /* 0を返却                            */
}
/* ディレクトリの拡張を行う */
static F_INT FMR_expandDirClst(
	F_INT drvnum,                       /* ドライブ番号                       */
	F_DWORD lastclstnum,                /* 最終クラスタ番号                   */
	F_DWORD * expclstnum                /* 拡張したクラスタ番号               */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_BYTE l_emptysectbuf;              /* 空きクラスタを含むFATセクタを      */
	                                    /* 読込んでいるFAT読込み領域番号      */
	F_BYTE l_cnt;                       /* ループカウンタ                     */
	
	/* 空きクラスタを取得する */
	l_rtn = FMR_searchEmptyCluster(
		drvnum,                         /* 対象論理ドライブ番号               */
		expclstnum                      /* 取得空きクラスタ番号               */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDROOTENTFULL;
	}
	
	/* 空きクラスタをALL 0でクリアする */
	FMR_memSet(                         /* R/Wバッファ1をイメージ(ALL0)とする */
		MFMR_RWBUFFTOP( drvnum, 1 ),
		DFMR_INITBYTE,
		MFMR_BYTEPERSECT( drvnum )
		);
	for( l_cnt = 0; l_cnt < MFMR_SECTPERCLST( drvnum ); l_cnt ++ )
	{
		l_rtn = FMR_writeSectRetry(
			drvnum,                     /* ドライブ番号                       */
			FMR_getSectNum(             /* セクタ番号                         */
				*( expclstnum ),        /* クラスタ番号                       */
				l_cnt,                  /* クラスタ内セクタ番号               */
				drvnum                  /* ドライブ番号                       */
				),
			DFMR_FLGON                  /* R/Wバッファ番号                    */
			);
		if( l_rtn != DFMR_OK )
		{
			return DFMR_SDSCRAPERR;     /* 断片ファイル作成時エラー発生       */
		}
	}
	
	/* 最終クラスタを含むFATセクタを読込み領域0に読込む */
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),
		                                /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		lastclstnum,                    /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
	/* 取得した空きクラスタを含むFATセクタを読込み領域1に読込む */
	l_emptysectbuf = DFMR_REFFATAREA1;
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),
		                                /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, l_emptysectbuf ),
		                                /* FAT読込み領域                      */
		*( expclstnum ),                /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
	if( MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 )
		== MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA1 ) )
	{                                   /* 空きクラスタと最終クラスタが同じ   */
	                                    /* FATセクタに所属している場合        */
		l_emptysectbuf = DFMR_REFFATAREA0;
	}
	/* 取得した空きクラスタのFATエントリをEOCとする */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_fatentry );
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, l_emptysectbuf ),
		                                /* FAT読込み領域                      */
		*( expclstnum ),                /* クラスタ番号                       */
		&l_fatentry,                    /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
	if( l_emptysectbuf != DFMR_REFFATAREA0 )
	{                                   /* 空きクラスタがが最終クラスタと     */
	                                    /* 異なるFATセクタに属する場合        */
		/* FAT読込み領域1を書込む */
		l_rtn = FMR_setFatSectorRetry(
			drvnum,                     /* 論理ドライブ番号                   */
			l_emptysectbuf              /* FAT読込み領域番号                  */
			);
		if( l_rtn != DFMR_OK )
		{
			return DFMR_SDSCRAPERR;     /* 断片ファイル作成時エラー発生       */
		}
	}
	
	/* 最終クラスタのFATエントリを取得した空きクラスタの */
	/* クラスタ番号とする                                */
	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		lastclstnum,                    /* クラスタ番号                       */
		expclstnum,                     /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	/* FAT読込み領域0を書込む */
	l_rtn = FMR_setFatSectorRetry(
		drvnum,                         /* 論理ドライブ番号                   */
		DFMR_REFFATAREA0                /* FAT読込み領域番号                  */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
}

/* 指定最終クラスタ番号を先頭クラスタとするエントリを検索する */
/* エントリが存在した場合は先頭クラスタ番号、サイズを更新する */
static F_INT FMR_reconfigScrapFileEntry(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD ssectnum,                   /* 検索開始セクタ番号                 */
	F_BYTE sslot,                       /* 検索開始セクタ内位置               */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD btmclstnum,                 /* 最終クラスタ番号                   */
	F_WORD clstcnt                      /* クラスタ数                         */
)
{
	TFMR_DIRENTRY * l_entry;            /* ディレクトリエントリ               */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_sectnum;                  /* 対象セクタ番号                     */
	F_BYTE l_slot;                      /* 対象セクタ内位置                   */
	F_DWORD l_clstnum;                  /* 対象クラスタ番号                   */
	F_DWORD l_topclstnum;               /* 取得エントリの先頭クラスタ番号     */
	F_DWORD l_fatentry;                 /* 対象クラスタのFATエントリ          */
	F_DWORD l_size;                     /* ファイルサイズ                     */
	
	/* 初期設定 */
	l_sectnum = ssectnum;
	l_slot = sslot;
	l_clstnum = FMR_getClstNum(
		drvnum,                         /* ドライブ番号                       */
		l_sectnum                       /* セクタ番号                         */
		);
	
	/* 検索開始セクタを領域1読込む */
	l_rtn = FMR_readSectRetry(
		drvnum,                         /* ドライブ番号                       */
		l_sectnum,                      /* セクタ番号                         */
		DFMR_FLGON                      /* R/Wバッファ番号                    */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	/* ｢検索開始セクタを含むクラスタ｣を含むFATセクタを領域1に読込む */
	l_rtn = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
		                                /* FAT読込み領域                      */
		l_clstnum,                      /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_rtn != DFMR_OK )
	{
		return DFMR_SDSCRAPERR;         /* 断片ファイル作成時エラー発生       */
	}
	
	/* エントリを取得する */
	l_entry = ( TFMR_DIRENTRY * )&( MFMR_RWBUFF( drvnum, 1,
		l_slot * sizeof ( TFMR_DIRENTRY ) ) );
	/* 対象エントリを検索する */
	while( 1 )
	{
		/* 対象エントリが未使用の場合 */
		if( l_entry->name[ 0 ] == DFMR_INITBYTE )
		{
			return DFMR_NOENTRY;        /* 対象エントリ無しエラー             */
		}
		/* 対象のエントリより先頭クラスタ番号を取得する */
		l_topclstnum = FMR_getTopClstNum(
			l_entry                     /* ディレクトリエントリ               */
			);
		if( l_topclstnum == btmclstnum )/* 取得した先頭クラスタ番号と最終     */
		{                               /* クラスタ番号が等しい場合           */
			/* 対象エントリのサイズを取得する */
			l_size = FMR_getFileSize(
				l_entry                 /* ディレクトリエントリ               */
				);
			/* サイズに新たなクラスタ分を加算 */
			l_size += clstcnt
				* MFMR_SECTPERCLST( drvnum )
				* MFMR_BYTEPERSECT( drvnum );
			/* 対象エントリのサイズを更新する */
			FMR_setFileSize(
				l_entry,                /* ディレクトリエントリ               */
				l_size                  /* ファイルサイズ                     */
				);
			/* 対象エントリの先頭クラスタ番号を更新する */
			FMR_setTopClstNum(
				l_entry,                /* ディレクトリエントリ               */
				topclstnum              /* 先頭クラスタ番号                   */
				);
			/* 対象エントリを読込んでいるセクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGON              /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
			return DFMR_OK;
		}
		/* 位置を更新 */
		l_entry ++;                     /* エントリ更新                       */
		l_slot ++;
		if( l_slot >= ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY ) ) )
		{                               /* セクタ最終位置まで検索した場合     */
			l_slot = 0;                 /* 位置をセクタ先頭とする             */
			l_entry = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( drvnum, 1 ) );
			if( l_sectnum == FMR_getLastSectNum(
				drvnum,
				l_sectnum
				)                       /* 現在のセクタがクラスタ内の         */
			)                           /* 最終セクタの場合                   */
			{
				/* 対象クラスタのFATエントリを取得する */
				l_rtn = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
					                    /* FAT読込み領域                      */
					l_clstnum,          /* クラスタ番号                       */
					&l_fatentry,        /* FATエントリ値                      */
					DFMR_FLGOFF         /* Read/Write種別  OFF:取得           */
					);
				if( l_rtn != DFMR_OK )  /* エラーの場合は対象クラスタが       */
				{                       /* 他のFATセクタにあると判断          */
					if( l_rtn == DFMR_CLSTNUMERR )
					{                   /* クラスタ番号異常の場合は異常終了   */
						return l_rtn;
					}
					/* FATセクタを読込む */
					l_rtn = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
						                /* FAT読込み領域                      */
						l_clstnum,      /* クラスタ番号                       */
						&l_fatentry     /* FATエントリ値                      */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_SDSCRAPERR; 
						                /* 断片ファイル作成時エラー発生       */
					}
				}
				l_clstnum = l_fatentry; /* クラスタ番号更新                   */
				
				MFMR_GETEOCMIN( MFMR_FATTYPE( drvnum ), l_fatentry )
				if( l_clstnum >= l_fatentry )
				{                       /* 対象クラスタ番号がEOCの場合        */
					return DFMR_NOENTRY;/* 対象エントリ無しエラー             */
				}
				l_sectnum = FMR_getSectNum(
					l_clstnum,          /* クラスタ番号                       */
					0,                  /* クラスタ内セクタ番号    */
					drvnum              /* ドライブ番号                       */
					);
			}
			else
			{
				l_sectnum ++;
			}
			/* 検索対象セクタを領域1読込む */
			l_rtn = FMR_readSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_sectnum,              /* セクタ番号                         */
				DFMR_FLGON              /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return DFMR_SDSCRAPERR; /* 断片ファイル作成時エラー発生       */
			}
		}
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
