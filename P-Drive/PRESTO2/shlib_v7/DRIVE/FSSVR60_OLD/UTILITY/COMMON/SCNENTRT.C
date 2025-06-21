/*======================================================================
 * File name    : scnentrt.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/scnentrt.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scnentrt.c $
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
/* 関 数 名：FMR_scanEntryUnderRoot                                           */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_CHAR * エントリ名格納領域                                 */
/*         ：     F_BYTE * クラスタ使用状況展開領域                           */
/*         ：     F_BYTE * ログ出力領域                                       */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：エントリの検索を行う(FAT12/16ルートディレクトリ下)               */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

#define MODE_SRCHENTRY 0                /* 動作モード：エントリ検索中         */
#define MODE_GETLFN    2                /* 動作モード：LFN取得中              */
#define MODE_FINISHLFN 3                /* 動作モード：LFN取得完了            */
#define TOPLFNORDER 1                   /* 先頭LFNエントリの順序              */

F_INT FMR_scanEntryUnderRoot(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
)
{
	TFMR_SEARCHINFO l_toplfnpos;        /* 最終LFNエントリ位置                */
	TFMR_SEARCHINFO l_btmlfnpos;        /* 最終取得LFNエントリ位置            */
	TFMR_SEARCHINFO l_nowpos;           /* 現在のエントリ位置                 */
	TFMR_SDWORKAREA * l_sdworkarea;     /* スキャンディスク用ワークエリア     */
	TFMR_ENTRY * l_entry;               /* エントリ取得用ポインタ             */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_rtn2;                       /* サブルーチン戻り値                 */
	F_DWORD l_topclstnum;               /* 先頭クラスタ番号                   */
	F_WORD l_pathlen;                   /* 名称長                             */
	F_BYTE l_badflg;                    /* アクセス不能セクタ検出判定用フラグ */
	                                    /* ON :読込み不能のセクタが存在       */
	                                    /* OFF:読込み不能のセクタは未検出     */
	F_BYTE l_updateflg;                 /* セクタ更新判定用フラグ             */
	                                    /* ON :セクタ更新済み                 */
	                                    /* OFF:セクタ未更新                   */
	F_BYTE l_mode;                      /* 動作モード                         */
	F_BYTE l_ord;                       /* LFN順序                            */
	F_BYTE l_chksum;                    /* LFNチェックサム                    */
	
	/* 初期設定を行う */
	l_badflg = DFMR_FLGOFF;             /* 読込み不能のセクタ未検出           */
	l_nowpos.bufnum = DFMR_FLGOFF;      /* バッファ番号0                      */
	l_nowpos.sectnum = MFMR_ROOTTOP( drvnum ); 
	                                    /* ルートディレクトリ先頭セクタ       */
	l_sdworkarea = ( TFMR_SDWORKAREA * )ramaddr;
	                                    /* スキャンディスク用ワークエリア取得 */
	
	l_updateflg = DFMR_FLGOFF;          /* セクタは未更新                     */
	l_mode = MODE_SRCHENTRY;            /* 動作モード：エントリ検索中         */
	
	l_nowpos.slot = DFMR_SRCHROOTTOPSLOT;
	
	/* エントリの検索処理を行う */
	while( l_nowpos.sectnum < MFMR_DATATOP( drvnum ) )
	{                                   /* セクタ単位のループ                 */
		/* 対象のセクタを読込む */
		while( l_nowpos.sectnum < MFMR_DATATOP( drvnum ) )
		{
			l_rtn = FMR_readSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_nowpos.sectnum,       /* セクタ番号                         */
				l_nowpos.bufnum         /* R/Wバッファ番号                    */
				);
			if( l_rtn == DFMR_OK )
			{
				break;
			}
			else
			{
				l_badflg = DFMR_FLGON;  /* 読込み不能のセクタ検出             */
				l_nowpos.sectnum ++;    /* 次のセクタを対象とする             */
				if( l_nowpos.sectnum >= MFMR_DATATOP( drvnum ) )
				{                       /* ルート最終セクタまで読込んだ場合   */
					return DFMR_SDILLEGALROOT;
				}
			}
		}
		/* エントリをR/Wバッファより取得する */
		l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
				l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
		while( l_nowpos.slot
			 < ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) )
		)                               /* エントリ単位のループ               */
		{
			/* マトリクス処理 */
			if( l_entry->lfn.ord == DFMR_TBDENTRY )
			{                           /* 対象エントリが未使用               */
				if( ( l_mode == MODE_GETLFN )
					|| ( l_mode == MODE_FINISHLFN ) )
				{                       /* 動作モードがLFN取得中/取得完了     */
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
					{                   /* セクタ内先頭以外の場合             */
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
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
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						0                   /* エントリ名設定先頭位置         */
						);
				}
				
				if( l_updateflg == DFMR_FLGON )
				{                       /* セクタが更新されていた場合         */
					/* 対象セクタを書込む */
					l_rtn = FMR_writeSectRetry(
						drvnum,             /* ドライブ番号                   */
						l_nowpos.sectnum,   /* セクタ番号                     */
						l_nowpos.bufnum     /* R/Wバッファ番号                */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					
				}
				if( l_badflg == DFMR_FLGON )
				{                       /* 読込み不能のセクタが存在した場合   */
					return DFMR_SDILLEGALROOT;  /* ディレクトリ異常有り返却   */
				}
				return DFMR_OK;         /* 正常終了返却                       */
			}
			else if( l_entry->lfn.ord == DFMR_DELENTRY )
			{                           /* 対象エントリが削除済み             */
				if( ( l_mode == MODE_GETLFN )
					|| ( l_mode == MODE_FINISHLFN ) )
				{                       /* 動作モードがLFN取得中/取得完了     */
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
					{                   /* セクタ内先頭以外の場合             */
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
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
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
					/* 現在位置にポインタをキャストする */
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
			{                           /* 対象エントリが最終LFNエントリ      */
				if( ( l_mode == MODE_GETLFN )
					|| ( l_mode == MODE_FINISHLFN ) )
				{                       /* 動作モードがLFN取得中/取得完了     */
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
					{                   /* セクタ内先頭以外の場合             */
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
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
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
					/* 現在位置にポインタをキャストする */
					l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
				}
				/* LFNエントリ位置を設定する */
				l_toplfnpos = l_nowpos;     /* 最終LFNエントリ位置            */
				l_btmlfnpos = l_nowpos;     /* 最終取得LFNエントリ位置        */
				
				/* 順序、チェックサムを取得する */
				l_ord = l_entry->lfn.ord & ~DFMR_LFN_LAST;  /* LFN順序        */
				l_chksum = l_entry->lfn.chksum;     /* LFNチェックサム        */
				
				if( l_ord == TOPLFNORDER )  /* 順序が1の場合                  */
				{
					l_mode = MODE_FINISHLFN;/* 動作モード：LFN取得完了        */
				}
				else
				{
					l_mode = MODE_GETLFN;   /* 動作モード：LFN取得中          */
				}
			}
			else if(
				( ( l_entry->lfn.ord & DFMR_LFN_LAST ) != DFMR_LFN_LAST )
				&& l_entry->lfn.attr == DFMR_LFN_ATTR
			)
			{                           /* 対象エントリが最終以外LFNエントリ  */
				if( l_mode == MODE_SRCHENTRY )
				{                       /* 動作モードがエントリ検索中         */
					/* 取得したエントリを削除済みとする */
					l_entry->lfn.ord = DFMR_DELENTRY;
					
					/* セクタ更新済みとする */
					l_updateflg = DFMR_FLGON;
					
					/* 不正LFNエントリ削除ログを作成する */
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
				}
				else if( l_mode == MODE_GETLFN )
				{                       /* 動作モードがLFN取得中              */
					/* 取得したエントリのパラメータチェックを行う */
					if( ( l_entry->lfn.ord != ( l_ord - 1 ) )
						|| ( l_entry->lfn.chksum != l_chksum ) )
					{                   /* 順序、チェックサムの何れかが異常   */
						/* 取得中のLFNエントリを削除する */
						l_rtn = FMR_deleteErrLFNEntry(
							drvnum,         /* ドライブ番号                   */
							&l_toplfnpos,   /* 削除対象先頭エントリ位置       */
							&l_btmlfnpos,   /* 削除対象最終エントリ位置       */
							&l_nowpos       /* 現在アクセス中のエントリ位置   */
							);
						if( l_rtn != DFMR_OK )
						{
							return l_rtn;
						}
						/* 取得したエントリを削除済みとする */
						l_entry->lfn.ord = DFMR_DELENTRY;
						/* 不正LFNエントリ削除ログを作成する */
						l_pathlen = 0;
						FMR_recLFNErrLog(
							&( l_entry->lfn ),  /* 最終LFNエントリ            */
							entrypath,          /* エントリ名格納領域         */
							l_sdworkarea,       /* ScanDisk用ワークエリア     */
							logarea,            /* ログ出力領域               */
							&l_pathlen          /* エントリ名設定先頭位置     */
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
						
						if( l_ord == TOPLFNORDER )  /* 順序が1の場合          */
						{
							l_mode = MODE_FINISHLFN;
						}
						/* 最終取得LFNエントリ位置を更新する */
						l_btmlfnpos = l_nowpos;
					}
				}
				else                    /* 動作モード：LFN取得完了            */
				{
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					/* 取得したエントリを削除済みとする */
					l_entry->lfn.ord = DFMR_DELENTRY;
					
					/* 不正LFNエントリ削除ログを作成する */
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
					/* セクタ更新済みとする */
					l_updateflg = DFMR_FLGON;
					
					/* エントリ検索中とする */
					l_mode = MODE_SRCHENTRY;
				}
			}
			else if( l_entry->lfn.attr == DFMR_ATTR_VOLID )
			{                           /* 対象エントリがボリュームID         */
				if( ( l_mode == MODE_GETLFN )
					|| ( l_mode == MODE_FINISHLFN ) )
				{                       /* 動作モードがLFN取得中/取得完了     */
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
					{                   /* セクタ内先頭以外の場合             */
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
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
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
					/* 現在位置にポインタをキャストする */
					l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
				}
				
				/* エントリ検索中とする */
				l_mode = MODE_SRCHENTRY;
			}
			else                        /* 対象エントリがディレクトリエントリ */
			{
				if( ( l_mode == MODE_GETLFN )       /* LFN取得中の場合        */
					|| ( ( l_mode == MODE_FINISHLFN )   /* LFN取得完了で      */
					&& ( l_chksum != FMR_getChkSum( /* チェックサムが異常     */
						( F_CHAR * )&l_entry->dir.name ) ) )    /* の場合     */
				)
				{
					/* 取得中のLFNエントリを削除する */
					l_rtn = FMR_deleteErrLFNEntry(
						drvnum,         /* ドライブ番号                       */
						&l_toplfnpos,   /* 削除対象先頭エントリ位置           */
						&l_btmlfnpos,   /* 削除対象最終エントリ位置           */
						&l_nowpos       /* 現在アクセス中のエントリ位置       */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
					{                   /* セクタ内先頭の場合                 */
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
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
					l_pathlen = 0;
					FMR_recLFNErrLog(
						&( l_entry->lfn ),  /* 最終LFNエントリ                */
						entrypath,          /* エントリ名格納領域             */
						l_sdworkarea,       /* ScanDisk用ワークエリア         */
						logarea,            /* ログ出力領域                   */
						&l_pathlen          /* エントリ名設定先頭位置         */
						);
					/* 現在位置にポインタをキャストする */
					l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_nowpos.bufnum,
							l_nowpos.slot * sizeof( TFMR_DIRENTRY ) ) );
				}
				/* エントリの名称をパス名に設定 */
				if( ( l_sdworkarea->mode & DFMR_SD_LOGMODEON )
					== DFMR_SD_LOGMODEON )
				{                       /* ロギングありの場合                 */
					/* パス名にエントリ名を設定 */
					l_pathlen = FMR_getDirName(
						( F_CHAR * )&( l_entry->dir.name ),
						                            /* エントリの名称部       */
						entrypath                   /* 名称を設定する領域     */
						);
					if( ( l_sdworkarea->mode & DFMR_SD_LOGFULLPATH )
						!= DFMR_SD_LOGFULLPATH )
					{                           /* ファイル名のみ指定の場合   */
						l_pathlen = 0;          /* エントリ名設定先頭位置は   */
						                        /* バッファの先頭             */
					}
				}
				
				if( l_updateflg == DFMR_FLGON )
				{                       /* セクタが更新されている場合         */
					/* 対象セクタを書込む */
					l_rtn = FMR_writeSectRetry(
						drvnum,             /* ドライブ番号                   */
						l_nowpos.sectnum,   /* セクタ番号                     */
						l_nowpos.bufnum     /* R/Wバッファ番号                */
						);
					l_updateflg = DFMR_FLGOFF;  /* セクタ未更新とする         */
				}
				/* 先頭クラスタ番号を取得する */
				l_topclstnum = FMR_getTopClstNum(
					&l_entry->dir
					);
				/* 属性の確認を行う */
				if( ( l_entry->dir.attr & DFMR_ATTR_DIR ) == DFMR_ATTR_DIR )
				{                       /* 属性がディレクトリのエントリの場合 */
					/* エントリ検索処理を行う */
					l_rtn = FMR_scanEntry(
						entrypath,      /* エントリ名格納領域                 */
						ramaddr,        /* クラスタ使用状況展開領域           */
						logarea,        /* ログ出力領域                       */
						&l_topclstnum,  /* 先頭クラスタ番号                   */
						&l_pathlen,     /* エントリ名設定先頭位置             */
						drvnum          /* ドライブ番号                       */
						);
					if( ( l_rtn != DFMR_OK )/* 異常終了の場合             */
						&& ( l_rtn != DFMR_SDILLEGALDIR )
						&& ( l_rtn != DFMR_SDALLERRDIR ) )
					{
						return l_rtn;
					}
					
					/* 対象のセクタを読込む */
					l_rtn2 = FMR_readSectRetry(
						drvnum,             /* ドライブ番号                       */
						l_nowpos.sectnum,   /* セクタ番号                         */
						l_nowpos.bufnum     /* R/Wバッファ番号                    */
						);
					if( l_rtn2 != DFMR_OK )
					{
						return l_rtn2;
					}
					
					if( ( l_rtn == DFMR_SDILLEGALDIR )
						|| ( l_rtn == DFMR_SDALLERRDIR ) )
					{                       /* ディレクトリが不正の場合   */
						l_entry->lfn.ord = DFMR_DELENTRY;
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
						/* ロギングを行う */
						if( l_rtn == DFMR_SDILLEGALDIR )
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
						&l_entry->dir   /* 先頭クラスタ番号が変更             */
						) )             /* されている場合                     */
					{
						/* 先頭クラスタ番号を設定する */
						FMR_setTopClstNum(
							&l_entry->dir,  /* ディレクトリエントリ           */
							l_topclstnum    /* 先頭クラスタ番号               */
							);
						l_updateflg = DFMR_FLGON;   /* セクタ更新済み         */
					}
				}
				else                    /* 属性がファイルのエントリの場合     */
				{
					/* データ部のBADクラスタチェックを行う */
					l_rtn = FMR_scanFileData(
						drvnum,         /* ドライブ番号                       */
						FMR_getFileSize( &l_entry->dir ),
						                /* ファイルサイズ                     */
						l_topclstnum,   /* 先頭クラスタ番号                   */
						entrypath,      /* エントリ名格納領域                 */
						ramaddr,        /* クラスタ使用状況展開領域           */
						logarea         /* ログ出力領域                       */
						);
					if( l_rtn != DFMR_OK )
					{
						if( l_rtn == DFMR_SDSBROKENFILE )
						{               /* 対象が破損ファイルの場合           */
							if( ( l_mode == MODE_GETLFN )
								|| ( l_mode == MODE_FINISHLFN )
							)           /* 対象がLFNファイルの場合            */
							{                   
								/* 先頭位置のセクタを読込む */
								l_rtn = FMR_readSectRetry(
									drvnum,             /* ドライブ番号   */
									l_toplfnpos.sectnum,   /* セクタ番号     */
									l_toplfnpos.bufnum     /* R/Wバッファ番号*/
									);
								if( l_rtn != DFMR_OK )
								{
									return l_rtn;
								}
								if( l_toplfnpos.sectnum != l_nowpos.sectnum )
								{
									/* 現在位置のセクタを読込む */
									l_rtn = FMR_readSectRetry(
										drvnum,             /* ドライブ番号   */
										l_nowpos.sectnum,   /* セクタ番号     */
										l_nowpos.bufnum     /* R/Wバッファ番号*/
										);
									if( l_rtn != DFMR_OK )
									{
										return l_rtn;
									}
								}
								/* 取得中のLFNエントリを削除する */
								l_rtn = FMR_deleteErrLFNEntry(
									drvnum,
									    /* ドライブ番号                       */
									&l_toplfnpos,
									    /* 削除対象先頭エントリ位置           */
									&l_btmlfnpos,
									    /* 削除対象最終エントリ位置           */
									&l_nowpos
									    /* 現在アクセス中のエントリ位置       */
									);
								if( l_rtn != DFMR_OK )
								{
									return l_rtn;
								}
								/* 対象ファイルを削除済みとする */
								l_entry->dir.name[ 0 ] = DFMR_DELENTRY;
								l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
							}
							else
							{
								/* 対象のセクタを読込む */
								l_rtn = FMR_readSectRetry(
									drvnum,             /* ドライブ番号   */
									l_nowpos.sectnum,   /* セクタ番号     */
									l_nowpos.bufnum     /* R/Wバッファ番号*/
									);
								if( l_rtn != DFMR_OK )
								{
									return l_rtn;
								}
								
								/* 対象ファイルを削除済みとする */
								l_entry->dir.name[ 0 ] = DFMR_DELENTRY;
								l_updateflg = DFMR_FLGON;   /* セクタ更新済み     */
							}
						}
						else
						{
							return l_rtn;
						}
					}
					else
					{
						/* 対象のセクタを読込む */
						l_rtn = FMR_readSectRetry(
							drvnum,             /* ドライブ番号               */
							l_nowpos.sectnum,   /* セクタ番号                 */
							l_nowpos.bufnum     /* R/Wバッファ番号            */
							);
						if( l_rtn != DFMR_OK )
						{
							return l_rtn;
						}
					}
				}
				
				/* パス名からエントリ名称を削除 */
				if( ( l_sdworkarea->mode & DFMR_SD_LOGMODEON )
						== DFMR_SD_LOGMODEON )
				{                       /* ロギングありの場合                 */
					/* パス名のエントリ名部分を初期化 */
					FMR_memSet(
						entrypath,      /* 先頭位置           */
						DFMR_INITBYTE,  /* 初期化データ=0                     */
						( DFMR_FNAMESZ + DFMR_EXTSZ + 1 )
						                /* ファイル名+拡張子+｢.｣のサイズ      */
						);
				}
				l_mode = MODE_SRCHENTRY;/* 動作モード：エントリ検索中         */
				
			}
			l_nowpos.slot ++;           /* セクタ内位置更新                   */
			l_entry ++;                 /* エントリを1件分進める              */
		}
		
		if( l_updateflg == DFMR_FLGON ) /* セクタが更新されていた場合         */
		{
			/* 対象セクタを書込む */
			l_rtn = FMR_writeSectRetry(
				drvnum,                 /* ドライブ番号                       */
				l_nowpos.sectnum,       /* セクタ番号                         */
				l_nowpos.bufnum         /* R/Wバッファ番号                    */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			l_updateflg = DFMR_FLGOFF;  /* セクタ未更新とする                 */
		}
		
		/* 現在のエントリ位置を更新 */
		l_nowpos.sectnum ++;
		l_nowpos.slot = 0;
		if( ( ( l_mode == MODE_GETLFN ) || ( l_mode == MODE_FINISHLFN ) )
			&& ( l_toplfnpos.bufnum == l_nowpos.bufnum ) )
		if( l_mode != MODE_SRCHENTRY )  /* LFN取得中/取得完了時は             */
		{                               /* R/Wバッファを切替える              */
			MFMR_FLGSWITCH( l_nowpos.bufnum );
		}
		
	}
	/* ルートディレクトリの最終セクタ検索完了で処理終了とする */
	if( ( l_mode == MODE_GETLFN )
		|| ( l_mode == MODE_FINISHLFN ) )
	{                       /* 動作モードがLFN取得中/取得完了     */
		/* 取得中のLFNエントリを削除する */
		l_nowpos = l_btmlfnpos;         /* 最終位置を現在位置とする           */
		l_rtn = FMR_deleteErrLFNEntry(
			drvnum,                     /* ドライブ番号                       */
			&l_toplfnpos,               /* 削除対象先頭エントリ位置           */
			&l_btmlfnpos,               /* 削除対象最終エントリ位置           */
			&l_nowpos                   /* 現在アクセス中のエントリ位置       */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		if( l_nowpos.slot != DFMR_SRCHROOTTOPSLOT )
		{                               /* セクタ内先頭以外の場合             */
			l_updateflg = DFMR_FLGON;   /* セクタ更新済み                     */
		}
		/* 最終LFNエントリの位置にポインタをキャストする */
		l_entry = ( TFMR_ENTRY * )&( MFMR_RWBUFF( drvnum, l_btmlfnpos.bufnum,
				l_btmlfnpos.slot * sizeof( TFMR_DIRENTRY ) ) );
		/* 不正LFNエントリ削除ログを作成する */
		l_pathlen = 0;
		FMR_recLFNErrLog(
			&( l_entry->lfn ),          /* 最終LFNエントリ                    */
			entrypath,                  /* エントリ名格納領域                 */
			l_sdworkarea,               /* ScanDisk用ワークエリア             */
			logarea,                    /* ログ出力領域                       */
			&l_pathlen                  /* エントリ名設定先頭位置             */
			);
		l_updateflg = DFMR_FLGON;
	}
	
	if( l_updateflg == DFMR_FLGON )     /* セクタが更新されていた場合         */
	{
		/* 対象セクタを書込む */
		l_rtn = FMR_writeSectRetry(
			drvnum,                     /* ドライブ番号                       */
			l_nowpos.sectnum,           /* セクタ番号                         */
			l_nowpos.bufnum             /* R/Wバッファ番号                    */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
	}
	
	
	if( l_badflg == DFMR_FLGON )
	{
		return DFMR_SDILLEGALROOT;
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
