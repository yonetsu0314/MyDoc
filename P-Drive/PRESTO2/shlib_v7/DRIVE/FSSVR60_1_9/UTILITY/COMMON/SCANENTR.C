/*======================================================================
 * File name    : scanentr.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/scanentr.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scanentr.c $
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
/* 関 数 名：FMR_scanEntry                                                    */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_CHAR * エントリ名格納領域                                 */
/*         ：     F_WORD エントリ名設定先頭位置                               */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：     F_BYTE * ログ出力領域                                       */
/*         ：     F_DWORD * 先頭クラスタ番号                                  */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：エントリの検索を行う                                             */
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

#define MODE_SRCHENTRY 0                /* 動作モード：エントリ検索中         */
#define MODE_GETLFN    2                /* 動作モード：LFN取得中              */
#define MODE_FINISHLFN 3                /* 動作モード：LFN取得完了            */
#define TOPLFNORDER 1                   /* 先頭LFNエントリの順序              */

#define MASK_LFNORD 0x001F
#define MASK_NAMELEN 0x01E0
#define SHIFT_NAMELEN 5
#define MASK_MODE 0x0600
#define SHIFT_MODE 9
#define MASK_BADFLG 0x0800
#define SHIFT_BADFLG 11
#define MASK_UPDATEFLG 0x1000
#define SHIFT_UPDATEFLG 12


F_INT FMR_scanEntry(
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	F_DWORD * topclstnum,               /* 先頭クラスタ番号                   */
	F_WORD * entrypathlen,              /* エントリ名設定先頭位置             */
	F_CHAR drvnum                       /* ドライブ番号                       */
)
{
	TFMR_SEARCHINFO l_toplfnpos;        /* 最終LFNエントリ位置                */
	TFMR_SEARCHINFO l_btmlfnpos;        /* 最終取得LFNエントリ位置            */
	TFMR_SEARCHINFO l_nowpos;           /* 現在のエントリ位置                 */
	TFMR_ENTRY * l_entry;               /* エントリ取得用ポインタ             */
	TFMR_SDWORKAREA * l_sdworkarea;     /* スキャンディスク用ワークエリア     */
	F_DWORD l_clstnum;                  /* 対象クラスタ番号                   */
	F_DWORD l_fclstnum;                 /* 前クラスタ番号                     */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_DWORD l_topclstnum;               /* 先頭クラスタ番号                   */
	F_WORD l_pathlen;                   /* パス長                             */
	F_BYTE l_badflg;                    /* BADクラスタ検出判定用フラグ        */
	                                    /* ON :BADクラスタ検出中とする        */
	                                    /* OFF:現在読込んでいるFATセクタ内で  */
	                                    /*     BADクラスタは未検出            */
	F_BYTE l_updateflg;                 /* セクタ更新判定用フラグ             */
	                                    /* ON :セクタ更新済み                 */
	                                    /* OFF:セクタ未更新                   */
	                                    /* ※先頭クラスタ確認時は以下となる   */
	                                    /* ON :BADクラスタ検出                */
	                                    /* OFF:BADクラスタ未検出              */
	F_BYTE l_mode;                      /* 動作モード                         */
	F_BYTE l_ord;                       /* LFN順序                            */
	F_BYTE l_chksum;                    /* LFNチェックサム                    */
	
	/* 初期設定を行う */
	l_clstnum = *( topclstnum );        /* 対象クラスタを先頭クラスタとする   */
	l_badflg = DFMR_FLGOFF;             /* BADクラスタ未検出                  */
	l_updateflg = DFMR_FLGOFF;          /* セクタ未更新                       */
	l_sdworkarea = ( TFMR_SDWORKAREA * )ramaddr;
	                                    /* スキャンディスク用ワークエリア取得 */
	
	/* 対象クラスタ番号を含むFATセクタを領域0に読込む */
	l_sdworkarea->dwork01.iwork = FMR_getFatSectorRetry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),      /* FAT読込み領域      */
		l_clstnum,                      /* クラスタ番号                       */
		&l_fatentry                     /* FATエントリ値                      */
		);
	if( l_sdworkarea->dwork01.iwork != DFMR_OK )
	{                                   /* 正常終了の場合は処理続行           */
		return l_sdworkarea->dwork01.iwork;
	}
	
	/* 先頭クラスタのBADクラスタチェックを行う */
	while( 1 )
	{
		/* クラスタ使用状況を取得する */
		if( DFMR_FLGON != FMR_framBitChk(   /* クラスタ使用状況を取得         */
			l_clstnum,                      /* クラスタ番号                   */
			ramaddr                         /* クラスタ使用状況展開領域       */
			)
		)                               /* クラスタが未使用/確認済みの場合    */
		{
			*( topclstnum ) = DFMR_INITBYTE;    /* 先頭クラスタ番号を0とする  */
			return DFMR_SDALLERRDIR;
		}
		/* BADクラスタチェックを行う */
		l_sdworkarea->dwork01.iwork = FMR_chkBadClst(
			drvnum,                     /* ドライブ番号                       */
			l_clstnum,                  /* クラスタ番号                       */
			0,                          /* R/Wバッファ番号                    */
			DFMR_FLGON                  /* 対象クラスタ：使用中               */
			);
		if( l_sdworkarea->dwork01.iwork == DFMR_OK )
		{                               /* 正常終了の場合は処理続行           */
			/* クラスタ使用状況をOFFとする */
			FMR_framBitOff(
				l_clstnum,              /* クラスタ番号                       */
				ramaddr                 /* クラスタ使用状況展開領域           */
			);
			break;
		}
		else if( l_sdworkarea->dwork01.iwork = DFMR_SD_CHKBADCLST )
		{                               /* BADクラスタの場合                  */
			l_badflg = DFMR_FLGON;      /* 読込んでいるFATセクタ内で,         */
			                            /* BADクラスタ検出済みとする          */
			l_updateflg = DFMR_FLGON;   /* 先頭部でBADクラスタ検出済みとする  */
			/* 次のクラスタ(FATエントリの値)を先頭クラスタとする */
			*( topclstnum ) = l_fatentry;
			
			/* 対象クラスタのFATエントリをBADクラスタとする */
			MFMR_GETBAD(                /* BADクラスタコード取得              */
				MFMR_FATTYPE( drvnum ),
				l_sdworkarea->dwork02.dwork
				);
			l_sdworkarea->dwork01.iwork = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_clstnum,              /* クラスタ番号                       */
				&( l_sdworkarea->dwork02.dwork ),
				                        /* FATエントリ値(BADクラスタ)         */
				DFMR_FLGON              /* Read/Write種別  ON:設定            */
				);
			if( l_sdworkarea->dwork01.iwork != DFMR_OK )
			{
				return l_sdworkarea->dwork01.iwork;
			}
			/* 対象クラスタのFAT使用状況をOFFにする */
			FMR_framBitOff(
				l_clstnum,              /* クラスタ番号                       */
				ramaddr                 /* クラスタ使用状況展開領域           */
			);
			/* FATエントリを対象クラスタとする */
			l_clstnum = l_fatentry;
			
			/* EOCチェックを行う */
			MFMR_GETEOCMIN(             /* EOCコードを取得                    */
				MFMR_FATTYPE( drvnum ),
				l_sdworkarea->dwork01.dwork
				);
			if( l_clstnum >= l_sdworkarea->dwork01.dwork )
			{                           /* 先頭クラスタが全てBADの場合        */
				/* 現在読込まれているFATセクタを書込む */
				l_sdworkarea->dwork01.iwork = FMR_setFatSectorRetry(
					drvnum,             /* ドライブ番号                       */
				    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_sdworkarea->dwork01.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork01.iwork;
				}
				
				*( topclstnum ) = DFMR_INITBYTE;    /* 先頭クラスタ番号を0とする  */
				return DFMR_SDALLERRDIR;/* BADクラスタチェック終了            */
			}
			/* 対象クラスタのFATエントリを取得する */
			l_sdworkarea->dwork01.iwork = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_clstnum,              /* クラスタ番号                       */
				&l_fatentry,            /* FATエントリ値                      */
				DFMR_FLGOFF             /* Read/Write種別  OFF:取得           */
				);
			if( l_sdworkarea->dwork01.iwork != DFMR_OK )
			{                           /* エラーの場合は対象クラスタが       */
			                            /* 他のFATセクタにあると判断          */
				if( l_sdworkarea->dwork01.iwork == DFMR_CLSTNUMERR )
				{                       /* クラスタ番号異常の場合は異常終了   */
					return l_sdworkarea->dwork02.iwork;
				}
				/* 現在読込まれているFATセクタを書込む */
				l_sdworkarea->dwork01.iwork = FMR_setFatSectorRetry(
					drvnum,             /* ドライブ番号                       */
				    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_sdworkarea->dwork01.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork01.iwork;
				}
				
				/* BADクラスタを未検出とする */
				l_badflg = DFMR_FLGOFF;
				/* FATセクタを読込む */
				l_sdworkarea->dwork01.iwork = FMR_getFatSectorRetry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_clstnum,          /* クラスタ番号                       */
					&l_fatentry         /* FATエントリ値                      */
					);
				if( l_sdworkarea->dwork01.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork01.iwork;
				}
			}
		}
		else                            /* その他エラーコードの場合は         */
		{                               /* エラー終了                         */
			return l_sdworkarea->dwork01.iwork;
		}
	}
	
	/* エントリ検索処理初期設定を行う */
	l_nowpos.bufnum = DFMR_FLGOFF;      /* バッファ番号0                      */
	l_nowpos.sectnum = FMR_getSectNum(
		l_clstnum,                      /* クラスタ番号                       */
		0,                              /* クラスタ内セクタ番号               */
		drvnum                          /* ドライブ番号                       */
		);
	l_mode = MODE_SRCHENTRY;            /* 動作モード：エントリ検索中         */
	
	if( l_badflg != DFMR_FLGOFF )
	{                                   /* 現在読込まれているFATが更新済みの  */
	                                    /* 場合(BADクラスタ)                  */
		/* 現在読込まれているFATセクタを書込む */
		l_sdworkarea->dwork01.iwork = FMR_setFatSectorRetry(
			drvnum,                     /* ドライブ番号                       */
		    DFMR_REFFATAREA0            /* FAT読込み領域番号                  */
			);
		if( l_sdworkarea->dwork01.iwork != DFMR_OK )
		{
			return l_sdworkarea->dwork01.iwork;
		}
		
		/* BADクラスタを未検出とする */
		l_badflg = DFMR_FLGOFF;
	}
	if( l_updateflg == DFMR_FLGON )     /* 先頭クラスタがBADクラスタの場合    */
	{
		/* ロギングを行う */
		FMR_recErrLog(
			drvnum,                     /* ドライブ番号                       */
			DFMR_INITBYTE,              /* 断片ファイルの先頭クラスタ番号     */
			DFMR_INITBYTE,              /* 断片ファイルのクラスタ数           */
			DFMR_SD_LOGERRTOPBAD,       /* 先頭クラスタがBADクラスタ          */
			entrypath,                  /* 元ファイル名                       */
			logarea,                    /* ログ出力領域                       */
			l_sdworkarea                /* ワークエリア                       */
			);
		/* セクタ内位置を0に設定 */
		l_nowpos.slot = DFMR_SRCHROOTTOPSLOT;
	}
	else                                /* 先頭クラスタが正常な場合           */
	{
		if( ( MFMR_FATTYPE( drvnum ) == DFMR_FATTYPE32 )
			&& ( *( topclstnum ) == FMR_getClstNum(
				drvnum,                      /* ドライブ番号                      */
				MFMR_ROOTTOP( drvnum )       /* セクタ番号                        */
				)
			)
		)
		{                               /* FAT32ルートディレクトリ先頭        */
			/* セクタ内位置を0に設定 */
			l_nowpos.slot = DFMR_SRCHROOTTOPSLOT;
		}
		else
		{
			l_entry = ( TFMR_ENTRY * )( MFMR_RWBUFFTOP( drvnum, l_nowpos.bufnum ) );
			if( !( ( l_entry->dir.name[ 0 ] == '.' )
				&& ( l_entry->dir.name[ 1 ] == DFMR_SPACE ) ) )
			{                           /* ｢.｣が存在しない場合                */
				return DFMR_SDILLEGALDIR;       /* エラー返却                 */
			}
			
			l_entry ++;
			
			if( !( ( l_entry->dir.name[ 0 ] == '.' )
				&& ( l_entry->dir.name[ 1 ] == '.' )
				&& ( l_entry->dir.name[ 2 ] == DFMR_SPACE ) ) )
			{                           /* ｢..｣が存在しない場合               */
				return DFMR_SDILLEGALDIR;       /* エラー返却                 */
			}
			
			/* セクタ内位置を2に設定 */
			l_nowpos.slot = DFMR_SRCHDIRTOPSLOT;
		}
	}
	
	/* セクタ更新判定用フラグ初期化 */
	l_updateflg = DFMR_FLGOFF;
	
	/* エントリの検索処理を行う */
	while( 1 )                          /* クラスタ単位のループ               */
	{
		while( 1 )                      /* セクタ単位のループ                 */
		{
			/* エントリをR/Wバッファより取得する */
			l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
					l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
			while( l_nowpos.slot
				< ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) )
			)                           /* エントリ単位のループ               */
			{
				/* マトリクス処理 */
				if( l_entry->lfn.ord == DFMR_TBDENTRY )
				{                       /* 対象エントリが未使用               */
					if( ( l_mode == MODE_GETLFN )
						|| ( l_mode == MODE_FINISHLFN ) )
					{                   /* 動作モードがLFN取得中/取得完了     */
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
						{               /* セクタ内先頭以外の場合             */
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
						else
						{
							/* LFNエントリ削除処理内で最終位置を読込んでいる */
							/* バッファが切り替わっているのでここで番号を    */
							/* 切替える                                      */
							if( l_toplfnpos.bufnum != l_btmlfnpos.bufnum )
							{
								/* 先頭位置と最終位置が異なる場合のみ */
								MFMR_FLGSWITCH( l_btmlfnpos.bufnum );
							}
						}
						/* 最終LFNエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
								l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
					}
					
					if( l_updateflg == DFMR_FLGON )
					{                   /* セクタが更新されていた場合         */
						/* 対象セクタを書込む */
						l_sdworkarea->dwork01.iwork = FMR_writeSectRetry(
							drvnum,             /* ドライブ番号               */
							l_nowpos.sectnum,   /* セクタ番号                 */
							l_nowpos.bufnum     /* R/Wバッファ番号            */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
					}
					
					MFMR_GETEOCMIN(     /* EOCコードを取得                    */
						MFMR_FATTYPE( drvnum ),
						l_sdworkarea->dwork01.dwork
						);
					if( l_fatentry < l_sdworkarea->dwork01.dwork )
					{                   /* FATエントリがEOCでない場合         */
						MFMR_GETEOC(    /* EOCコードを取得                    */
							MFMR_FATTYPE( drvnum ),
							l_sdworkarea->dwork01.dwork
							);
						/* FATエントリをEOCとする */
						l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
							MFMR_DRVINFO( drvnum ), 
							            /* 論理ドライブ構成情報               */
							MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
							            /* FAT読込み領域                      */
							l_clstnum,  /* クラスタ番号                       */
							&( l_sdworkarea->dwork01.dwork ),
							            /* FATエントリ値(EOC)                 */
							DFMR_FLGON  /* Read/Write種別  ON:設定            */
							);
						
						if( l_sdworkarea->dwork02.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork02.iwork;
						}
						/* 現在読込まれているFATセクタを書込む */
						l_sdworkarea->dwork01.iwork = FMR_setFatSectorRetry(
							drvnum,             /* ドライブ番号               */
						    DFMR_REFFATAREA0    /* FAT読込み領域番号          */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
					}
					return DFMR_OK;     /* 正常終了返却                       */
				}
				else if( l_entry->lfn.ord == DFMR_DELENTRY )
				{                       /* 対象エントリが削除済み             */
					if( ( l_mode == MODE_GETLFN )
						|| ( l_mode == MODE_FINISHLFN ) )
					{                   /* 動作モードがLFN取得中/取得完了     */
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
						{               /* セクタ内先頭以外の場合             */
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
						else
						{
							/* LFNエントリ削除処理内で最終位置を読込んでいる */
							/* バッファが切り替わっているのでここで番号を    */
							/* 切替える                                      */
							if( l_toplfnpos.bufnum != l_btmlfnpos.bufnum )
							{
								/* 先頭位置と最終位置が異なる場合のみ */
								MFMR_FLGSWITCH( l_btmlfnpos.bufnum );
							}
						}
						/* 最終LFNエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
								l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* 現在のエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
					}
					/* エントリ検索中とする */
					l_mode = MODE_SRCHENTRY;
				}
				else if(
					( ( l_entry->lfn.ord & DFMR_LFN_LAST ) == DFMR_LFN_LAST )
					&& l_entry->lfn.attr == DFMR_LFN_ATTR
				)
				{                       /* 対象エントリが最終LFNエントリ      */
					if( ( l_mode == MODE_GETLFN )
						|| ( l_mode == MODE_FINISHLFN ) )
					{                   /* 動作モードがLFN取得中/取得完了     */
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
						{               /* セクタ内先頭以外の場合             */
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
						else
						{
							/* LFNエントリ削除処理内で最終位置を読込んでいる */
							/* バッファが切り替わっているのでここで番号を    */
							/* 切替える                                      */
							if( l_toplfnpos.bufnum != l_btmlfnpos.bufnum )
							{
								/* 先頭位置と最終位置が異なる場合のみ */
								MFMR_FLGSWITCH( l_btmlfnpos.bufnum );
							}
						}
						/* 最終LFNエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
								l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* 現在のエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
					}
					/* LFNエントリ位置を設定する */
					l_toplfnpos = l_nowpos;     /* 最終LFNエントリ位置        */
					l_btmlfnpos = l_nowpos;     /* 最終取得LFNエントリ位置    */
					
					/* 順序、チェックサムを取得する */
					l_ord = l_entry->lfn.ord & ~DFMR_LFN_LAST;  /* LFN順序    */
					l_chksum = l_entry->lfn.chksum;     /* LFNチェックサム    */
					
					if( l_ord == TOPLFNORDER )  /* 順序が1の場合              */
					{
						l_mode = MODE_FINISHLFN;/* 動作モード：LFN取得完了    */
					}
					else
					{
						l_mode = MODE_GETLFN;   /* 動作モード：LFN取得中      */
					}
				}
				else if(
					( ( l_entry->lfn.ord & DFMR_LFN_LAST ) != DFMR_LFN_LAST )
					&& l_entry->lfn.attr == DFMR_LFN_ATTR
				)
				{                       /* 対象エントリが最終以外LFNエントリ  */
					if( l_mode == MODE_SRCHENTRY )
					{                   /* 動作モードがエントリ検索中         */
						/* 取得したエントリを削除済みとする */
						l_entry->lfn.ord = DFMR_DELENTRY;
						
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* セクタ更新済みとする */
						l_updateflg = DFMR_FLGON;
					}
					else if( l_mode == MODE_GETLFN )
					{                   /* 動作モードがLFN取得中              */
						/* 取得したエントリのパラメータチェックを行う */
						if( ( l_entry->lfn.ord != ( l_ord - 1 ) )
							|| ( l_entry->lfn.chksum != l_chksum ) )
						{               /* 順序、チェックサムの何れかが異常   */
							/* 取得中のLFNエントリを削除する */
							l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
								drvnum,         /* ドライブ番号               */
								&l_toplfnpos,   /* 削除対象先頭エントリ位置   */
								&l_btmlfnpos,   /* 削除対象最終エントリ位置   */
								&l_nowpos   /* 現在アクセス中のエントリ位置   */
								);
							if( l_sdworkarea->dwork01.iwork != DFMR_OK )
							{
								return l_sdworkarea->dwork01.iwork;
							}
							/* 取得したエントリを削除済みとする */
							l_entry->lfn.ord = DFMR_DELENTRY;
							
							/* 不正LFNエントリ削除ログを作成する */
							FMR_recLFNErrLog(
								&( l_entry->lfn ),  /* 最終LFNエントリ        */
								entrypath,          /* エントリ名格納領域     */
								l_sdworkarea,       /* ScanDisk用ワークエリア */
								logarea,            /* ログ出力領域           */
								entrypathlen        /* エントリ名設定先頭位置 */
								);
							/* セクタ更新済みとする */
							l_updateflg = DFMR_FLGON;
							
							/* エントリ検索中とする */
							l_mode = MODE_SRCHENTRY;
						}
						else
						{
							/* 順序を更新する */
							l_ord --;
							
							if( l_ord == TOPLFNORDER )  /* 順序が1の場合      */
							{
								l_mode = MODE_FINISHLFN;
							}
							/* 最終取得LFNエントリ位置を更新する */
							l_btmlfnpos = l_nowpos;
						}
					}
					else                /* 動作モード：LFN取得完了            */
					{
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						/* 取得したエントリを削除済みとする */
						l_entry->lfn.ord = DFMR_DELENTRY;
						
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* セクタ更新済みとする */
						l_updateflg = DFMR_FLGON;
						l_mode = MODE_GETLFN;   /* 動作モード：LFN取得中      */
					}
				}
				else if( l_entry->lfn.attr == DFMR_ATTR_VOLID )
				{                       /* 対象エントリがボリュームID         */
					if( ( l_mode == MODE_GETLFN )
						|| ( l_mode == MODE_FINISHLFN ) )
					{                   /* 動作モードがLFN取得中/取得完了     */
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
						{               /* セクタ内先頭以外の場合             */
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
						else
						{
							/* LFNエントリ削除処理内で最終位置を読込んでいる */
							/* バッファが切り替わっているのでここで番号を    */
							/* 切替える                                      */
							if( l_toplfnpos.bufnum != l_btmlfnpos.bufnum )
							{
								/* 先頭位置と最終位置が異なる場合のみ */
								MFMR_FLGSWITCH( l_btmlfnpos.bufnum );
							}
						}
						/* 最終LFNエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
								l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* 現在のエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
					}
					
					/* エントリ検索中とする */
					l_mode = MODE_SRCHENTRY;
				}
				else                    /* 対象エントリがディレクトリエントリ */
				{
					if( ( l_mode == MODE_GETLFN )       /* LFN取得中の場合    */
						|| ( ( l_mode == MODE_FINISHLFN )   /* LFN取得完了で  */
						&& ( l_chksum != FMR_getChkSum( /* チェックサムが異常 */
							( F_CHAR * )l_entry->dir.name ) ) )    /* の場合 */
					)
					{                   
						/* 取得中のLFNエントリを削除する */
						l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos    /* 現在アクセス中のエントリ位置   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
						{               /* セクタ内先頭以外の場合             */
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
						else
						{
							/* LFNエントリ削除処理内で最終位置を読込んでいる */
							/* バッファが切り替わっているのでここで番号を    */
							/* 切替える                                      */
							if( l_toplfnpos.bufnum != l_btmlfnpos.bufnum )
							{
								/* 先頭位置と最終位置が異なる場合のみ */
								MFMR_FLGSWITCH( l_btmlfnpos.bufnum );
							}
						}
						/* 最終LFNエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
								l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
						/* 不正LFNエントリ削除ログを作成する */
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							entrypathlen        /* エントリ名設定先頭位置     */
							);
						/* 現在のエントリの位置にポインタをキャストする */
						l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
					}
					/* エントリの名称をパス名に設定 */
					if( ( l_sdworkarea->mode & DFMR_SD_LOGMODEON )
						== DFMR_SD_LOGMODEON )
					{                   /* ロギングありの場合                 */
						if( ( l_sdworkarea->mode & DFMR_SD_LOGFULLPATH )
							!= DFMR_SD_LOGFULLPATH )
						{               /* ロギングがファイル名のみの場合     */
							/* エントリ名格納位置を初期化 */
							FMR_memSet(
								entrypath,      /* エントリ名格納領域アドレス */
								DFMR_INITBYTE,  /* 初期値0                    */
								( DFMR_FNAMESZ + DFMR_EXTSZ + 1 )
								        /* ファイル名+拡張子+｢.｣のサイズ      */
								);
						}
						/* パス名にエントリ名を設定 */
						l_pathlen = FMR_getDirName(
							( F_CHAR * )l_entry->dir.name,
							                            /* エントリの名称部   */
							( entrypath + *( entrypathlen ) )
							                            /* 名称を設定する領域 */
							);
						if( ( l_sdworkarea->mode & DFMR_SD_LOGFULLPATH )
							== DFMR_SD_LOGFULLPATH )
						{               /* ロギングがフルパス指定の場合       */
							/* エントリ名称設定先頭位置を更新 */
							*( entrypathlen ) += l_pathlen;
						}
					}
					
					if( l_updateflg == DFMR_FLGON )
					{                   /* セクタが更新されている場合         */
						/* 対象セクタを書込む */
						l_sdworkarea->dwork01.iwork = FMR_writeSectRetry(
							drvnum,             /* ドライブ番号               */
							l_nowpos.sectnum,   /* セクタ番号                 */
							l_nowpos.bufnum     /* R/Wバッファ番号            */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						l_updateflg = DFMR_FLGOFF;  /* セクタ未更新とする     */
					}
					if( l_badflg == DFMR_FLGON )
					{                   /* FATセクタが更新されている場合      */
						/* FATセクタを書込む */
						l_sdworkarea->dwork01.iwork = FMR_setFatSectorRetry(
							drvnum,         /* ドライブ番号                   */
						    DFMR_REFFATAREA0/* FAT読込み領域番号              */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork01.iwork;
						}
						
						/* BADクラスタを未検出とする */
						l_badflg = DFMR_FLGOFF;
					}
					/* 先頭クラスタ番号を取得する */
					l_topclstnum = FMR_getTopClstNum(
						&l_entry->dir
						);
					/* 属性の確認を行う */
					if( ( l_entry->dir.attr & DFMR_ATTR_DIR ) == DFMR_ATTR_DIR )
					{                   /* 属性がディレクトリのエントリの場合 */
						/* エントリ検索処理を行う */
						l_sdworkarea->dwork01.iwork = FMR_scanEntry(
							entrypath,          /* エントリ名格納領域         */
							ramaddr,            /* クラスタ使用状況展開領域   */
							logarea,            /* ログ出力領域               */
							&l_topclstnum,      /* 先頭クラスタ番号           */
							entrypathlen,       /* エントリ名設定先頭位置     */
							drvnum              /* ドライブ番号               */
							);
						if( ( l_sdworkarea->dwork01.iwork != DFMR_OK )
							&& ( l_sdworkarea->dwork01.iwork
								 != DFMR_SDILLEGALDIR )
							&& ( l_sdworkarea->dwork01.iwork
								 != DFMR_SDALLERRDIR )
						)
						{                       /* 異常終了の場合             */
							return l_sdworkarea->dwork01.iwork;
						}
						/* 対象のセクタを読込む */
						l_sdworkarea->dwork02.iwork = FMR_readSectRetry(
							drvnum,             /* ドライブ番号               */
							l_nowpos.sectnum,   /* セクタ番号                 */
							l_nowpos.bufnum     /* R/Wバッファ番号            */
							);
						if( l_sdworkarea->dwork02.iwork != DFMR_OK )
						{
							return l_sdworkarea->dwork02.iwork;
						}
						if( ( l_sdworkarea->dwork01.iwork == DFMR_SDILLEGALDIR )
							|| ( l_sdworkarea->dwork01.iwork
								 == DFMR_SDALLERRDIR )
						)
						{                       /* ディレクトリが不正の場合   */
							l_entry->lfn.ord = DFMR_DELENTRY;
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
							/* ロギングを行う */
							if( l_sdworkarea->dwork01.iwork == DFMR_SDILLEGALDIR )
							{
								l_sdworkarea->bwork = DFMR_SD_LOGERRTOPILL;
							}
							else
							{
								l_sdworkarea->bwork = DFMR_SD_LOGERRNOTOP;
							}
							FMR_recErrLog(
								drvnum,             /* ドライブ番号                   */
								DFMR_INITBYTE,      /* 断片ファイルの先頭クラスタ番号 */
								DFMR_INITBYTE,      /* 断片ファイルのクラスタ数       */
								l_sdworkarea->bwork,/* エラーコード                   */
								entrypath,          /* 元ファイル名                   */
								logarea,            /* ログ出力領域                   */
								l_sdworkarea        /* ワークエリア                   */
								);
						}
						
						if( l_topclstnum != FMR_getTopClstNum(
							&( l_entry->dir )   /* 先頭クラスタ番号が変更     */
							) )                 /* されている場合             */
						{
							/* 先頭クラスタ番号を設定する */
							FMR_setTopClstNum(
								&( l_entry->dir ),  /* ディレクトリエントリ   */
								l_topclstnum    /* 先頭クラスタ番号           */
								);
							l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						}
					}
					else                /* 属性がファイルのエントリの場合     */
					{
						/* データ部のBADクラスタチェックを行う */
						l_sdworkarea->dwork01.iwork = FMR_scanFileData(
							drvnum,         /* ドライブ番号                   */
							FMR_getFileSize( &( l_entry->dir ) ),
							                /* ファイルサイズ                 */
							l_topclstnum,   /* 先頭クラスタ番号               */
							entrypath,      /* エントリ名格納領域             */
							ramaddr,        /* クラスタ使用状況展開領域       */
							logarea         /* ログ出力領域                   */
							);
						if( l_sdworkarea->dwork01.iwork != DFMR_OK )
						{
							if( l_sdworkarea->dwork01.iwork
								== DFMR_SDSBROKENFILE )
							{               /* 対象が破損ファイルの場合       */
								if( ( l_mode == MODE_GETLFN )
									|| ( l_mode == MODE_FINISHLFN )
								)           /* 対象がLFNファイルの場合        */
								{                   
									/* 先頭位置のセクタを読込む */
									l_sdworkarea->dwork01.iwork
										= FMR_readSectRetry(
										drvnum,             /* ドライブ番号   */
										l_toplfnpos.sectnum,   /* セクタ番号     */
										l_toplfnpos.bufnum     /* R/Wバッファ番号*/
										);
									if( l_sdworkarea->dwork01.iwork != DFMR_OK )
									{
										return l_sdworkarea->dwork01.iwork;
									}
									if( l_toplfnpos.sectnum != l_nowpos.sectnum )
									{
										/* 現在位置のセクタを読込む */
										l_sdworkarea->dwork01.iwork
											= FMR_readSectRetry(
											drvnum,             /* ドライブ番号   */
											l_nowpos.sectnum,   /* セクタ番号     */
											l_nowpos.bufnum     /* R/Wバッファ番号*/
											);
										if( l_sdworkarea->dwork01.iwork != DFMR_OK )
										{
											return l_sdworkarea->dwork01.iwork;
										}
									}
									/* 取得中のLFNエントリを削除する */
									l_sdworkarea->dwork01.iwork
										= FMR_deleteErrLFNEntry(
										drvnum,
										    /* ドライブ番号                   */
										&l_toplfnpos,
										    /* 削除対象先頭エントリ位置       */
										&l_btmlfnpos,
										    /* 削除対象最終エントリ位置       */
										&l_nowpos
										    /* 現在アクセス中のエントリ位置   */
										);
									if( l_sdworkarea->dwork01.iwork != DFMR_OK )
									{
										return l_sdworkarea->dwork01.iwork;
									}
									/* 対象ファイルを削除済みとする */
									l_entry->dir.name[ 0 ] = DFMR_DELENTRY;
									l_updateflg = DFMR_FLGON;
									                    /* セクタ更新済み     */
								}
								else
								{
									/* 対象のセクタを読込む */
									l_sdworkarea->dwork01.iwork
										= FMR_readSectRetry(
										drvnum,             /* ドライブ番号   */
										l_nowpos.sectnum,   /* セクタ番号     */
										l_nowpos.bufnum     /* R/Wバッファ番号*/
										);
									if( l_sdworkarea->dwork01.iwork != DFMR_OK )
									{
										return l_sdworkarea->dwork01.iwork;
									}
									
									/* 対象ファイルを削除済みとする */
									l_entry->dir.name[ 0 ] = DFMR_DELENTRY;
									l_updateflg = DFMR_FLGON;
									                    /* セクタ更新済み     */
								}
							}
							else
							{
								return l_sdworkarea->dwork01.iwork;
							}
						}
						else
						{
							/* 対象のセクタを読込む */
							l_sdworkarea->dwork01.iwork = FMR_readSectRetry(
								drvnum,             /* ドライブ番号           */
								l_nowpos.sectnum,   /* セクタ番号             */
								l_nowpos.bufnum     /* R/Wバッファ番号        */
								);
							if( l_sdworkarea->dwork01.iwork != DFMR_OK )
							{
								return l_sdworkarea->dwork01.iwork;
							}
						}
					}
					
					/* パス名からエントリ名称を削除 */
					if( ( l_sdworkarea->mode & DFMR_SD_LOGMODEON )
						== DFMR_SD_LOGMODEON )
					{                   /* ロギングありの場合                 */
						if( ( l_sdworkarea->mode & DFMR_SD_LOGFULLPATH )
							== DFMR_SD_LOGFULLPATH )
						{               /* ロギングがフルパス指定の場合       */
							/* エントリ名称設定先頭位置を更新 */
							*( entrypathlen ) -= l_pathlen;
						}
						/* パス名のエントリ名部分を初期化 */
						FMR_memSet(
							( entrypath + *( entrypathlen ) ),  /* 先頭位置   */
							DFMR_INITBYTE,      /* 初期化データ=0             */
							l_pathlen           /* 名称長                     */
							);
					}
					
					/* 対象のFATセクタを読込む */
					l_sdworkarea->dwork01.iwork = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                /* FAT読込み領域                      */
						l_clstnum,      /* クラスタ番号                       */
						&l_fatentry     /* FATエントリ値                      */
						);
					if( l_sdworkarea->dwork01.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork01.iwork;
					}
					
					l_mode = MODE_SRCHENTRY;/* 動作モード：エントリ検索中         */
				}
				l_nowpos.slot ++;       /* セクタ内位置更新                   */
				l_entry ++;             /* エントリを1件分進める              */
			}
			
			if( l_updateflg == DFMR_FLGON )     /* セクタが更新されていた場合 */
			{
				/* 対象セクタを書込む */
				l_sdworkarea->dwork01.iwork = FMR_writeSectRetry(
					drvnum,             /* ドライブ番号                       */
					l_nowpos.sectnum,   /* セクタ番号                         */
					l_nowpos.bufnum     /* R/Wバッファ番号                    */
					);
				if( l_sdworkarea->dwork01.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork01.iwork;
				}
				
				l_updateflg = DFMR_FLGOFF;      /* セクタ未更新とする         */
			}
			
			if( l_nowpos.sectnum        /* 対象セクタがクラスタ内最終セクタ   */
				== FMR_getLastSectNum(  /* だった場合                         */
					drvnum,
					l_nowpos.sectnum
					)
			)
			{
				break;
			}
			
			/* 現在のエントリ位置を更新 */
			l_nowpos.sectnum ++;
			l_nowpos.slot = 0;
			if( ( ( l_mode == MODE_GETLFN ) || ( l_mode == MODE_FINISHLFN ) )
				&& ( l_toplfnpos.bufnum == l_nowpos.bufnum ) )
			{                           /* LFN取得中/取得完了時にR/Wバッファ  */
			                            /* の内容が先頭エントリを含むセクタの */
			                            /* 場合                               */
				/* R/Wバッファを切替える */
				MFMR_FLGSWITCH( l_nowpos.bufnum );
			}
			/* 対象セクタを読込む */
			l_sdworkarea->dwork01.iwork = FMR_readSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_nowpos.sectnum,       /* セクタ番号                         */
				l_nowpos.bufnum         /* R/Wバッファ番号                    */
				);
			if( l_sdworkarea->dwork01.iwork != DFMR_OK )
			{
				return l_sdworkarea->dwork01.iwork;
			}
		}
		/* 現在のクラスタが最終クラスタの場合 */
		MFMR_GETEOCMIN(                 /* EOCコードを取得                    */
			MFMR_FATTYPE( drvnum ),
			l_sdworkarea->dwork01.dwork
			);
		if( l_fatentry >= l_sdworkarea->dwork01.dwork )
		{
			break;
		}
		
		/* 前クラスタ番号を現在のクラスタ番号とする */
		l_fclstnum = l_clstnum;

		/* 対象クラスタのBADクラスタチェックを行う */
		if( ( ( l_mode == MODE_GETLFN ) || ( l_mode == MODE_FINISHLFN ) )
			&& ( l_toplfnpos.bufnum == l_nowpos.bufnum ) )
		{                               /* LFN取得中/取得完了時にR/Wバッファ  */
		                                /* の内容が先頭エントリを含むセクタの */
		                                /* 場合                               */
			/* R/Wバッファを切替える */
			MFMR_FLGSWITCH( l_nowpos.bufnum );
		}
		while( 1 )
		{
			/* 現在のクラスタをFATエントリの値とする */
			l_clstnum = l_fatentry;
			
			/* EOCチェックを行う */
			MFMR_GETEOCMIN(             /* EOCコードを取得                    */
				MFMR_FATTYPE( drvnum ),
				l_sdworkarea->dwork01.dwork
				);
			if( l_clstnum >= l_sdworkarea->dwork01.dwork )
			{                           /* 対象のクラスタ番号がEOCの場合      */
				break;                  /* BADクラスタチェック終了            */
			}
			/* 対象クラスタのFATエントリを取得する */
			l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
				MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				                        /* FAT読込み領域                      */
				l_clstnum,              /* クラスタ番号                       */
				&l_fatentry,            /* FATエントリ値                      */
				DFMR_FLGOFF             /* Read/Write種別  OFF:取得           */
				);
			if( l_sdworkarea->dwork02.iwork != DFMR_OK )
			{                           /* エラーの場合は対象クラスタが       */
			                            /* 他のFATセクタにあると判断          */
				if( l_sdworkarea->dwork02.iwork == DFMR_CLSTNUMERR )
				{                       /* クラスタ番号異常の場合は異常終了   */
					return l_sdworkarea->dwork02.iwork;
				}
				if( l_badflg != DFMR_FLGOFF )
				{                       /* 現在読込まれているFATが更新済みの  */
				                        /* 場合(BADクラスタ)                  */
					/* 現在読込まれているFATセクタを書込む */
					l_sdworkarea->dwork02.iwork = FMR_setFatSectorRetry(
						drvnum,         /* ドライブ番号                       */
					    DFMR_REFFATAREA0/* FAT読込み領域番号                  */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
					
				}
				/* FATセクタを読込む */
				l_sdworkarea->dwork02.iwork = FMR_getFatSectorRetry(
					MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                        /* FAT読込み領域                      */
					l_clstnum,              /* クラスタ番号                       */
					&l_fatentry             /* FATエントリ値                      */
					);
				if( l_sdworkarea->dwork02.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork02.iwork;
				}
			}
			
			/* クラスタ使用状況を取得する */
			if( DFMR_FLGON != FMR_framBitChk(   /* クラスタ使用状況を取得する */
				l_clstnum,                      /* クラスタ番号               */
				ramaddr                         /* クラスタ使用状況展開領域   */
				)
			)                           /* クラスタが未使用/確認済みの場合    */
			{
				/* 前クラスタ番号のFATエントリを｢EOC｣とする */
				MFMR_GETEOC(            /* EOCコードを取得                    */
					MFMR_FATTYPE( drvnum ),
					l_sdworkarea->dwork01.dwork
					);
				
				l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ), 
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_fclstnum,         /* クラスタ番号                       */
					&( l_sdworkarea->dwork01.dwork ),
					                    /* FATエントリ値(EOC)                 */
					DFMR_FLGON          /* Read/Write種別  ON:設定            */
					);
				
				if( l_sdworkarea->dwork02.iwork != DFMR_OK )
				{                       /* エラーの場合は別のFATセクタに      */
				                        /* 前クラスタがあると判断             */
					if( l_sdworkarea->dwork02.iwork == DFMR_CLSTNUMERR )
					{                   /* クラスタ番号異常の場合は異常終了   */
						return l_sdworkarea->dwork02.iwork;
					}
					/* 対象FATセクタを読込む */
					l_sdworkarea->dwork02.iwork = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                /* FAT読込み領域                      */
						l_fclstnum,     /* クラスタ番号                       */
						&l_fatentry     /* FATエントリ値                      */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
					/* 前クラスタ番号のFATエントリを｢EOC｣とする */
					l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ), 
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                /* FAT読込み領域                      */
						l_fclstnum,     /* クラスタ番号                       */
						&( l_sdworkarea->dwork01.dwork ),
					                    /* FATエントリ値(EOC)                 */
						DFMR_FLGON      /* Read/Write種別  ON:設定            */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
				}
				
				/* 現在読込まれているFATセクタを書込む */
				l_sdworkarea->dwork02.iwork = FMR_setFatSectorRetry(
					drvnum,             /* ドライブ番号                       */
				    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
					);
				if( l_sdworkarea->dwork02.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork02.iwork;
				}

				if( ( l_mode == MODE_GETLFN )
					|| ( l_mode == MODE_FINISHLFN ) )
				{                       /* 動作モードがLFN取得中/取得完了     */
					/* 取得中のLFNエントリを削除する */
					l_nowpos = l_btmlfnpos;     /* 現在位置を最終位置とする   */
					l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_sdworkarea->dwork01.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork01.iwork;
					}
					/* 最終位置を含むセクタを書込む */
					l_sdworkarea->dwork01.iwork = FMR_writeSectRetry(
						drvnum,                 /* ドライブ番号               */
						l_nowpos.sectnum,       /* セクタ番号                 */
						l_nowpos.bufnum         /* R/Wバッファ番号            */
						);
					if( l_sdworkarea->dwork01.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork01.iwork;
					}
				}
				/* ロギングを行う */
				FMR_recErrLog(
					drvnum,                 /* ドライブ番号                   */
					DFMR_INITBYTE,          /* 断片ファイルの先頭クラスタ番号 */
					DFMR_INITBYTE,          /* 断片ファイルのクラスタ数       */
					DFMR_SD_LOGERRTAILOFF,  /* 先頭クラスタがBADクラスタ      */
					entrypath,              /* 元ファイル名                   */
					logarea,                /* ログ出力領域                   */
					l_sdworkarea            /* ワークエリア                   */
					);
				return DFMR_OK;         /* これ以上の検索は不可のためここで   */
				                        /* 正常終了とする                     */
			}
			/* BADクラスタチェックを行う */
			l_sdworkarea->dwork02.iwork = FMR_chkBadClst(
				drvnum,                 /* ドライブ番号                       */
				l_clstnum,              /* クラスタ番号                       */
				l_nowpos.bufnum,        /* R/Wバッファ番号                    */
				DFMR_FLGON              /* 対象クラスタ：使用中               */
				);
			if( l_sdworkarea->dwork02.iwork == DFMR_OK )
			{                           /* 正常終了の場合は処理続行           */
				/* クラスタ使用状況をOFFとする */
				FMR_framBitOff(
					l_clstnum,          /* クラスタ番号                       */
					ramaddr             /* クラスタ使用状況展開領域           */
				);
				/* 現在のエントリ位置を更新 */
				l_nowpos.sectnum = FMR_getSectNum(
					l_clstnum,
					0,
					drvnum
					);
				l_nowpos.slot = 0;
				break;
			}
			else if( l_sdworkarea->dwork02.iwork = DFMR_SD_CHKBADCLST )
			{                           /* BADクラスタの場合                  */
				l_badflg = DFMR_FLGON;  /* 読込んでいるFATセクタ内で,         */
				                        /* BADクラスタ検出済みとする          */
				
				/* 対象クラスタのFATエントリをBADクラスタとする */
				MFMR_GETBAD(            /* BADクラスタコード取得              */
					MFMR_FATTYPE( drvnum ),
					l_sdworkarea->dwork01.dwork
					);
				l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_clstnum,          /* クラスタ番号                       */
					&( l_sdworkarea->dwork01.dwork ),
					                    /* FATエントリ値(BADクラスタ)         */
					DFMR_FLGON          /* Read/Write種別  ON:設定            */
					);
				if( l_sdworkarea->dwork02.iwork != DFMR_OK )
				{
					return l_sdworkarea->dwork02.iwork;
				}
				
				/* 前クラスタのFATエントリを取得したFATエントリとする */
				l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
					MFMR_DRVINFO( drvnum ),
					                    /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                    /* FAT読込み領域                      */
					l_fclstnum,         /* クラスタ番号                       */
					&l_fatentry,        /* FATエントリ値                      */
					DFMR_FLGON          /* Read/Write種別  ON:設定            */
					);
				if( l_sdworkarea->dwork02.iwork != DFMR_OK )
				{                       /* エラーの場合は別のFATセクタに      */
				                        /* 前クラスタがあると判断             */
					if( l_sdworkarea->dwork02.iwork == DFMR_CLSTNUMERR )
					{                   /* クラスタ番号異常の場合は異常終了   */
						return l_sdworkarea->dwork02.iwork;
					}
					/* 対象FATセクタを読込む */
					l_sdworkarea->dwork02.iwork = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ),
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
						                /* FAT読込み領域                      */
						l_fclstnum,     /* クラスタ番号                       */
						&l_clstnum      /* FATエントリ値                      */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
					/* 前クラスタ番号のFATエントリを取得したFATエントリとする */
					l_sdworkarea->dwork02.iwork = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ), 
						                /* 論理ドライブ構成情報               */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA1 ),
						                /* FAT読込み領域                      */
						l_fclstnum,     /* クラスタ番号                       */
						&l_fatentry,    /* FATエントリ値(BADクラスタ)         */
						DFMR_FLGON      /* Read/Write種別  ON:設定            */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
					/* 現在読込まれているFATセクタを書込む */
					l_sdworkarea->dwork02.iwork = FMR_setFatSectorRetry(
						drvnum,             /* ドライブ番号                   */
					    DFMR_REFFATAREA1    /* FAT読込み領域番号              */
						);
					if( l_sdworkarea->dwork02.iwork != DFMR_OK )
					{
						return l_sdworkarea->dwork02.iwork;
					}
				}
				/* 対象クラスタのFAT使用状況をOFFにする */
				FMR_framBitOff(
					l_clstnum,          /* クラスタ番号                       */
					ramaddr             /* クラスタ使用状況展開領域           */
				);
			}
			else                        /* その他エラーコードの場合は         */
			{                           /* エラー終了                         */
				return l_sdworkarea->dwork02.iwork;
			}
		}
		if( l_badflg != DFMR_FLGOFF )
		{                           /* 現在読込まれているFATが更新済みの  */
		                            /* 場合(BADクラスタ)                  */
			/* 現在読込まれているFATセクタを書込む */
			l_sdworkarea->dwork02.iwork = FMR_setFatSectorRetry(
				drvnum,             /* ドライブ番号                       */
			    DFMR_REFFATAREA0    /* FAT読込み領域番号                  */
				);
			if( l_sdworkarea->dwork02.iwork != DFMR_OK )
			{
				return l_sdworkarea->dwork02.iwork;
			}
			
			/* BADクラスタを未検出とする */
			l_badflg = DFMR_FLGOFF;
			/* ロギングを行う */
			FMR_recErrLog(
				drvnum,                 /* ドライブ番号                   */
				DFMR_INITBYTE,          /* 断片ファイルの先頭クラスタ番号 */
				DFMR_INITBYTE,          /* 断片ファイルのクラスタ数       */
				DFMR_SD_LOGERRDIV,      /* 先頭クラスタがBADクラスタ      */
				entrypath,              /* 元ファイル名                   */
				logarea,                /* ログ出力領域                   */
				l_sdworkarea            /* ワークエリア                   */
				);
		}
		/* BADクラスタチェック中にEOCが検出された場合は処理終了とする */
		MFMR_GETEOCMIN(
			MFMR_FATTYPE( drvnum ),
			l_sdworkarea->dwork01.dwork
			);
		if( l_clstnum >= l_sdworkarea->dwork01.dwork )
		{
			break;
		}
	}
	/* EOCクラスタの検索完了で処理終了とする */
	if( ( l_mode == MODE_GETLFN )
		|| ( l_mode == MODE_FINISHLFN ) )
	{                                   /* 動作モードがLFN取得中/取得完了     */
		/* 取得中のLFNエントリを削除する */
		l_nowpos = l_btmlfnpos;         /* 現在位置を最終位置とする           */
		l_sdworkarea->dwork01.iwork = FMR_deleteErrLFNEntry(
			drvnum,                     /* ドライブ番号                       */
			&l_toplfnpos,               /* 削除対象先頭エントリ位置           */
			&l_btmlfnpos,               /* 削除対象最終エントリ位置           */
			&l_nowpos                   /* 現在アクセス中のエントリ位置       */
			);
		if( l_sdworkarea->dwork01.iwork != DFMR_OK )
		{
			return l_sdworkarea->dwork01.iwork;
		}
		/* 最終位置を含むセクタを更新するために */
		/* セクタ更新判定用フラグをONとする     */
		l_updateflg = DFMR_FLGON;
		
		/* 最終位置のエントリを取得する */
		l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
				l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
		/* 不正LFNエントリ削除ログを作成する */
		FMR_recLFNErrLog(
			&( l_entry->lfn ),          /* 最終LFNエントリ                    */
			entrypath,                  /* エントリ名格納領域                 */
			l_sdworkarea,               /* ScanDisk用ワークエリア             */
			logarea,                    /* ログ出力領域                       */
			entrypathlen                /* エントリ名設定先頭位置             */
			);
	}
	
	if( l_updateflg == DFMR_FLGON )     /* セクタが更新されていた場合         */
	{
		/* 対象セクタを書込む */
		l_sdworkarea->dwork01.iwork = FMR_writeSectRetry(
			drvnum,                     /* ドライブ番号                       */
			l_nowpos.sectnum,           /* セクタ番号                         */
			l_nowpos.bufnum             /* R/Wバッファ番号                    */
			);
		if( l_sdworkarea->dwork01.iwork != DFMR_OK )
		{
			return l_sdworkarea->dwork01.iwork;
		}
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/


