/*======================================================================
 * File name    : srchent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/srchent.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: srchent.c $
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
/* 関 数 名：FMR_searchEntry                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR            対象ドライブ番号                          */
/*         ：     TFMR_SAERCHINFO * 検索開始先頭位置                          */
/*         ：     F_CHAR *          検索文字列                                */
/*         ：     TFMR_DIRENTRY *   取得エントリ                              */
/*         ：           空エントリ検索の場合NULL指定                          */
/*         ：           エントリ名検索の場合、一致エントリ格納領域指定        */
/*         ：           動作フラグbit7:ONの場合、一致エントリ格納領域指定     */
/*         ：     F_BYTE            検索文字列長                              */
/*         ：     F_BYTE            動作フラグ                                */
/*         ：           bit0-4:LFN必要エントリ数(SFNの場合0x01)               */
/*         ：           bit5:ON=ワイルドカード検索指定                        */
/*         ：           bit6:ON=LFN検索指定                                   */
/*         ：           bit7:ON=空エントリ検索時、エントリ名検索を行う        */
/*         ：                                                                 */
/*         ：OUT: TFMR_SAERCHINFO * 検索終了先頭位置                          */
/*         ：     TFMR_SAERCHINFO * 検索終了最終位置                          */
/*         ：     TFMR_DIRENTRY * 取得エントリ                                */
/*         ：     F_CHAR * 取得ロングファイル名                               */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：エントリ名検索の場合                                             */
/*         ：       一致エントリ検出時　：DFMR_OK                             */
/*         ：       一致エントリ未検出時：DFMR_NOENTRY                        */
/*         ：空エントリ検索の場合                                             */
/*         ：       空エントリ検出時　　：DFMR_OK                             */
/*         ：       空エントリ未検出時　：DFMR_ENOEMPTYENTRY                  */
/*         ：空エントリ検索&エントリ名検索の場合                              */
/*         ：       空エントリ検出時　　：DFMR_OK                             */
/*         ：       一致エントリ検出時　：DFMR_SAMENAME                       */
/*         ：       空エントリ未検出時　：DFMR_ENOEMPTYENTRY                  */
/*         ：                                                                 */
/* 概    要：エントリを検索する                                               */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

#define MODE_SRCHENTRY 0                /* 動作モード：エントリ検索中         */
#define MODE_SRCHFREEAREA 1             /* 動作モード：連続空き領域検索中     */
#define MODE_GETLFN 2                   /* 動作モード：LFN取得中              */
#define MODE_FINISHLFN 3                /* 動作モード：LFN取得完了            */

F_BYTE FMR_subCompare(
	TFMR_LFNENTRY * lfnentry,           /* LFNエントリ用ポインタ              */
    F_CHAR * targetname,                /* 検索文字列                         */
    F_CHAR * getlfn,                    /* 取得ロングファイル名               */
	F_BYTE targetlen,                   /* 検索文字列長                       */
	F_BYTE getentry,                    /* 取得エントリ数                     */
	F_BYTE wcflg                        /* ワイルドカード指定フラグ           */
);

F_INT FMR_searchEntry(
    F_CHAR drvnum,                      /* 対象ドライブ番号                   */
    TFMR_SEARCHINFO * toppos,           /* 検索開始/終了先頭位置              */
    TFMR_SEARCHINFO * bottompos,        /* 検索終了最終位置                   */
    F_CHAR * targetname,                /* 検索文字列                         */
    TFMR_DIRENTRY * entry,              /* 取得エントリ                       */
    F_CHAR * getlfn,                    /* 取得ロングファイル名               */
    F_BYTE targetlen,                   /* 検索文字列長                       */
    F_BYTE prsflg                       /* 動作フラグ                         */
)
{
	F_DWORD l_nowsect;                  /* 検索セクタ                         */
	F_DWORD l_nowclstnum;               /* 現在のクラスタ番号                 */
	F_DWORD l_nextclstnum;              /* 次クラスタ番号                     */
	F_DWORD l_topentrysect;             /* LFN/空きエントリ先頭セクタ         */
	F_DWORD l_bottomsect;               /* クラスタ内最終セクタ番号           */
	F_DWORD l_eoc;                      /* EOCコード                          */

	F_CHAR * l_bufptr;                  /* ショートファイル名取得用ポインタ   */

	TFMR_LFNENTRY * l_lfnentry;         /* LFN&SFNエントリ用ポインタ          */

	F_INT l_rtn;                        /* サブルーチン戻り値                 */

	F_CHAR l_namebuf[ DFMR_LFN_FNAMESZ ]; /* エントリ単位のSFNバッファ        */

	F_BYTE l_work;                      /* ワークエリア                       */
	F_BYTE l_cnt;                       /* カウンタ                           */
	F_BYTE l_nowslot;                   /* 検索位置                           */
	F_BYTE l_getentry;                  /* 取得エントリ数                     */
	F_BYTE l_topentryslot;              /* LFN/空きエントリ先頭セクタ内位置   */
	F_BYTE l_topentbuf;                 /* LFN/空きエントリ先頭読込み         */
	                                    /* バッファ番号                       */
	F_BYTE l_mode;                      /* 動作モード                         */
	F_BYTE l_buffsw;                    /* R/Wバッファスイッチ                */
	F_BYTE l_lfnchksum;                 /* ロングファイル名のチェックサム     */
	F_BYTE l_emptyflg;                  /* 空エリア検出フラグ ≠0：空エリア検出 */
	
	/**********************************/
	/* ローカル変数の初期設定を行う   */
	/**********************************/
	l_mode = MODE_SRCHENTRY;            /* 動作モード←エントリ検索中         */
	l_buffsw = 0;                       /* R/Wバッファスイッチ←0             */
	l_nowsect = toppos->sectnum;        /* 検索セクタ←検索開始セクタ番号     */
	l_nowslot = toppos->slot;           /* 検索位置←検索開始セクタ内位置     */
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_eoc );
	                                    /* FATタイプよりEOCを求める           */
	l_emptyflg = 0;
	l_bufptr = targetname;
	l_topentbuf = 0;
	/**********************************/
	/* 検索セクタの位置を確認         */
	/**********************************/
	if( l_nowsect >= MFMR_DATATOP( drvnum ) )
	{                                   /* 検索セクタがData Regionに所属      */
		l_nowclstnum = l_nowsect - MFMR_DATATOP( drvnum );
		l_nowclstnum /= MFMR_SECTPERCLST( drvnum );
		l_bottomsect = l_nowclstnum;
		l_nowclstnum += 2;              /* 現在のクラスタ                     */
		
		l_bottomsect *= MFMR_SECTPERCLST( drvnum );
		l_bottomsect += MFMR_SECTPERCLST( drvnum ) - 1;
		l_bottomsect += MFMR_DATATOP( drvnum );
		                                /* 現在のクラスタの最終セクタ番号     */
		/* FAT読込み領域0に現在のクラスタのFAT番号が所属するFATセクタを読込む */
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
			l_nowclstnum,               /* クラスタ番号                       */
			&l_nextclstnum              /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	else                                /* 検索セクタがRoot Dir. Regionに所属 */
	{
		l_bottomsect = MFMR_DATATOP( drvnum ) - 1;
		                                /* ルートディレクトリの最終セクタ番号 */
		                                /* = Data Regionの先頭セクタ - 1      */
		l_nextclstnum = l_eoc;          /* 最終クラスタ(次クラスタなし)       */
	}
	
	/*********************************/
	/* 検索セクタを読込む            */
	/*********************************/
	/* I/Oパラメータ設定 */
	l_rtn = FMR_readDataSector(
		drvnum,
		l_nowsect,
		l_buffsw
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/*********************************/
	/* クラスタ単位のループ          */
	/*********************************/
	while( 1 )
	{
		/*********************************/
		/* セクタ単位のループ            */
		/*********************************/
		while( 1 )
		{                               /* 終了条件:クラスタ内最終セクタ超過  */
			/*********************************/
			/* エントリ単位のループ          */
			/*********************************/
			l_lfnentry = ( TFMR_LFNENTRY * )( MFMR_RWBUFFTOP( drvnum, l_buffsw ) );
			l_lfnentry += l_nowslot;
			while( l_nowslot 
				< MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY ) )
			{                    /* 終了条件:セクタ内最終エントリまで検索終了 */
				/*********************************/
				/* マトリクス処理                */
				/*********************************/
				/* 先頭文字による判定 */
				if( l_lfnentry -> ord == DFMR_TBDENTRY )
				{
					/**********************************************************/
					/* 未使用エントリの場合                                   */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:
					                    /* 連続空き領域検索中                 */
						/* マトリクス処理1-1 */
						if ( l_emptyflg == 0 )
						{
							toppos->sectnum = l_topentrysect;/* 先頭位置の設定 */
							toppos->slot = l_topentryslot;
							toppos->bufnum = l_topentbuf;
							bottompos->sectnum = 0;     /* 終了位置の設定     */
							bottompos->slot = 0;
							bottompos->bufnum = 0;
						}
						return DFMR_OK;
					case MODE_GETLFN:   /* LFN取得中                          */
					case MODE_FINISHLFN:/* LFN取得完了                        */
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* マトリクス処理1-0 */
						if ( ( entry == NULL ) ||
							 ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK ) )
						{               /* 空エントリ検索の場合               */
							if ( l_emptyflg == 0 )
							{
								toppos->sectnum = l_nowsect;/* 先頭位置の設定 */
								toppos->slot = l_nowslot;
								toppos->bufnum = l_buffsw;
								bottompos->sectnum = 0;     /* 終了位置の設定 */
								bottompos->slot = 0;
								bottompos->bufnum = 0;
							}
							return DFMR_OK;
						}
						else            /* 検索対象がディレクトリエントリ     */
						{
							return DFMR_NOENTRY;        /* 検索エントリなし   */
						}
					}
				}
				else if( l_lfnentry -> ord == DFMR_DELENTRY )
				{
					/**********************************************************/
					/* 削除済みエントリの場合                                 */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:
					                    /* 連続空き領域検索中                 */
						/* マトリクス処理2-1 */
						l_getentry ++;                  /* 取得エントリ数更新 */
						if( ( prsflg & DFMR_SRCHLEN ) == l_getentry )
						{               /* 取得完了の場合                     */
							toppos->sectnum = l_topentrysect;   /* 先頭位置の */
							toppos->slot = l_topentryslot;      /* 設定       */
							toppos->bufnum = l_topentbuf;
							bottompos->sectnum = l_nowsect; /* 終了位置の設定 */
							bottompos->slot = l_nowslot;
							bottompos->bufnum = l_buffsw;
							if ( ( prsflg & DFMR_SRCHFLGCHK ) != DFMR_SRCHFLGCHK )
							{
								/* 空エントリ検索時、エントリ名検索不用の場合 */
								return DFMR_OK;
							}
							l_emptyflg = 1;
							l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						}
						break;
					case MODE_GETLFN:   /* LFN取得中                          */
					case MODE_FINISHLFN:/* LFN取得完了                        */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* マトリクス処理2-0 */
						if ( ( entry == NULL ) ||
							 ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK ) )
						{
							if ( l_emptyflg == 0 )
							{
								/* 空エントリ検索の場合                       */
								if( ( prsflg & DFMR_SRCHLEN ) == 1 )
								{   /* 必要エントリが1件の場合は今回で検索完了*/
									toppos->sectnum = l_nowsect;/* 先頭位置の設定 */
									toppos->slot = l_nowslot;
									toppos->bufnum = l_buffsw;
									bottompos->sectnum = l_nowsect; /* 終了位置の */
									bottompos->slot = l_nowslot;    /* 設定       */
									bottompos->bufnum = l_buffsw;
									if ( ( prsflg & DFMR_SRCHFLGCHK ) != DFMR_SRCHFLGCHK )
									{
										/* エントリ名検索不用の場合               */
										return DFMR_OK;
									}
									l_emptyflg = 1;
								}
								else        /* 必要エントリ数が2件以上の場合      */
								{
									l_topentrysect = l_nowsect; /* 先頭位置設定   */
									l_topentryslot = l_nowslot; 
									l_topentbuf = l_buffsw;
									l_mode = MODE_SRCHFREEAREA; /* モード遷移：   */
									                        /* 連続空き領域検索中 */
									l_getentry = 1;         /* 取得エントリ数1件  */
								}
							}
						}
						break;
					}
				}
				else if( ( ( l_lfnentry -> attr ) == DFMR_LFN_ATTR ) &&
					( ( l_lfnentry -> ord & DFMR_LFN_LAST ) == DFMR_LFN_LAST ) )
				{
					/**********************************************************/
					/* 最終LFNエントリの場合                                  */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:
					                    /* 連続空き領域検索中                 */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						if ( ( prsflg & DFMR_SRCHFLGCHK ) != DFMR_SRCHFLGCHK )
						{
							/* 空エントリ検索時、エントリ名検索不用の場合 */
							break;
						}
					case MODE_GETLFN:   /* LFN取得中                          */
					case MODE_FINISHLFN:/* LFN取得完了                        */
						/* マトリクス処理3-2 */
						FMR_memSet(    /* 取得LFNバッファ初期化      */
							getlfn,
							DFMR_INITBYTE,
							DFMR_MAXFILESZ
						);
						/* ローカル変数設定 */
						l_getentry = l_lfnentry -> ord;
						l_getentry &= DFMR_LFN_ORDMASK;
						l_getentry -= 1;/* 順序の下位6bit-1                   */
						l_topentrysect = l_nowsect; /* 先頭位置の設定         */
						l_topentryslot = l_nowslot;
						l_topentbuf = l_buffsw;
						l_lfnchksum = ( l_lfnentry -> chksum );
						                /* サム値を取得                       */
						l_mode = FMR_subCompare( /* 取得ロングファイル名と検索文字列の比較 */
							l_lfnentry,
							targetname,
							getlfn,
							targetlen,
							l_getentry,
							( prsflg & DFMR_SRCHFLGWC )
							);
						break;
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* マトリクス処理3-0 */
						if( ( ( prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN ) )
						{
							/* ロングファイル名指定の場合                     */
							if ( ( entry != NULL ) ||
								 ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK ) )
							{
								/* エントリ名検索又は空きエントリ検索&エントリ名検索の場合 */
								FMR_memSet(    /* 取得LFNバッファ初期化      */
									getlfn,
									DFMR_INITBYTE,
									DFMR_MAXFILESZ
								);
								l_getentry = ( l_lfnentry -> ord );
								l_getentry &= DFMR_LFN_ORDMASK;
								l_getentry -= 1;            /* 順序の下位6bit - 1 */
								l_topentrysect = l_nowsect; /* 先頭位置設定       */
								l_topentryslot = l_nowslot;
								l_topentbuf = l_buffsw;
								l_lfnchksum = ( l_lfnentry -> chksum );
								                            /* エントリのサム値   */
								l_mode = FMR_subCompare( /* 取得ロングファイル名と検索文字列の比較 */
									l_lfnentry,
									targetname,
									getlfn,
									targetlen,
									l_getentry,
									( prsflg & DFMR_SRCHFLGWC )
									);
							}
						}
						break;
					}
				}
				else if( ( ( l_lfnentry -> attr ) == DFMR_LFN_ATTR ) &&
					( ( l_lfnentry -> ord & DFMR_LFN_LAST ) != DFMR_LFN_LAST ) ) 
				{
					/**********************************************************/
					/* 最終以外のLFNエントリの場合                            */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:
					case MODE_FINISHLFN:/* LFN取得完了                        */
					                    /* 連続空き領域検索中                 */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						break;
					case MODE_GETLFN:   /* LFN取得中                          */
						/* マトリクス処理4-2 */
						if( ( l_lfnchksum == ( l_lfnentry -> chksum ) )
						                /* サム値が等しい                     */
							&& ( l_getentry == ( l_lfnentry -> ord ) ) )
							            /* 順序が序列                         */
						{
							l_getentry --;                 /* 取得エントリ数  */
							                               /* 更新            */
							l_mode = FMR_subCompare( /* 取得ロングファイル名と検索文字列の比較 */
								l_lfnentry,
								targetname,
								getlfn,
								targetlen,
								l_getentry,
								( prsflg & DFMR_SRCHFLGWC )
								);
						}
						else                            /* 前回の続きではない */
						{
							l_mode = MODE_SRCHENTRY;    /* モード遷移：       */
							                            /* LFN取得完了        */
						}
						break;
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* NOP */
						break;
					}
				}
				else if( ( l_lfnentry -> attr ) == DFMR_ATTR_VOLID )
				{
					/**********************************************************/
					/* ボリュームIDエントリの場合                             */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:     /* 連続空き領域検索中         */
					case MODE_GETLFN:   /* LFN取得中                          */
					case MODE_FINISHLFN:/* LFN取得完了                        */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						break;
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* NOP */
						break;
					}
				}
				else
				{
					/**********************************************************/
					/* ディレクトリエントリの場合                             */
					/**********************************************************/
					switch( l_mode )    /* 動作モードによる振分               */
					{
					case MODE_SRCHFREEAREA:     /* 連続空き領域検索中         */
					case MODE_GETLFN:   /* LFN取得中                          */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						break;
					case MODE_FINISHLFN:/* LFN取得完了                        */
						/* マトリクス処理0-3 */
						if( l_lfnchksum == FMR_getChkSum(   /* サム値が一致   */
							( F_CHAR * )&( ( ( TFMR_DIRENTRY * )l_lfnentry ) -> name[ 0 ] )
							) )
						{
							*( entry ) = *( ( TFMR_DIRENTRY * )l_lfnentry );
							                            /* 取得したエントリ   */
							toppos->sectnum = l_topentrysect;   /* 先頭位置の */
							toppos->slot = l_topentryslot;      /* 設定       */
							toppos->bufnum = l_topentbuf;
							bottompos->sectnum = l_nowsect; /* 終了位置の設定 */
							bottompos->slot = l_nowslot;
							bottompos->bufnum = l_buffsw;
							if ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK )
							{
								/* 空エントリ検索時、エントリ名検索を行う場合 */
								return DFMR_SAMENAME;  /* 同一エントリ名有り  */
							}
							return DFMR_OK;
						}
						/* サム値が一致しない場合は処理続行 */
						l_mode = MODE_SRCHENTRY;
						                /* モード遷移：エントリ検索中         */
						break;
					case MODE_SRCHENTRY:/* エントリ検索中                     */
					default:            /* ｢その他｣は｢エントリ検索中｣とする   */
						/* マトリクス処理0-0 */
						if ( ( entry != NULL ) ||
							 ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK ) )
						{
							/* エントリ名検索又は空きエントリ検索&エントリ名検索の場合 */
							/* ショートファイル名(拡張子込)を取得する */
							l_bufptr = ( F_CHAR * )&l_namebuf[ 0 ];
							for( l_work = 0; l_work < DFMR_FNAMESZ; l_work ++ )
							{       /* ファイル名取得                         */
								if( ( ( TFMR_DIRENTRY * )l_lfnentry ) -> name[ l_work ] ==
										DFMR_SPACE )
								{
									break;
								}
								*( l_bufptr + l_work ) =
									( ( TFMR_DIRENTRY * )l_lfnentry ) -> name[ l_work ];
							}
							if( ( ( TFMR_DIRENTRY * )l_lfnentry ) ->extension[ 0 ] !=
									DFMR_SPACE )
							{       /* 拡張子がある場合                       */
								*( l_bufptr + l_work ) = '.';
								l_work ++;
								for( l_cnt = 0; l_cnt < DFMR_EXTSZ; l_cnt ++ )
								{
									if( ( ( TFMR_DIRENTRY * )l_lfnentry ) ->extension[ l_cnt ] ==
											DFMR_SPACE )
									{
										break;
									}
									*( l_bufptr + l_work ) =
										( ( TFMR_DIRENTRY * )l_lfnentry ) ->extension[ l_cnt ];
									l_work ++;
								}
							}
							*( l_bufptr + l_work ) = NULL;    /* NULL終端とする  */
							if( ( ( ( prsflg & DFMR_SRCHFLGWC ) == DFMR_SRCHFLGWC)
									&& (targetlen < l_work) )
							 || ( targetlen == l_work ) )
							{
								/* 文字列の比較を行う */
								if(
									FMR_compare(
										l_bufptr,   /* 比較元文字列           */
										targetname, /* 比較先文字列           */
										l_work,     /* 比較元文字列数         */
										targetlen,  /* 比較先文字列数         */
										( prsflg & DFMR_SRCHFLGWC ) /* Wild Card指定フラグ */
									) == DFMR_OK )  /* 一致した場合           */
								{
									*( entry ) = *( ( TFMR_DIRENTRY * )l_lfnentry );
								                    /* 取得したエントリ   */
									toppos->sectnum = l_nowsect;/* 先頭位置の設定 */
									toppos->slot = l_nowslot;
									toppos->bufnum = l_buffsw;
									if ( ( getlfn != NULL ) )
									{
										*getlfn = NULL;     /* NULL終端とする  */
									}
									if( ( prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
									{          /* LFNの検索指定されていた場合 */
										bottompos->sectnum = 0; /* 終了位置の設定 */
										bottompos->slot = 0; 
										bottompos->bufnum = 0; 
									}
									if ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK )
									{
										/* 空エントリ検索時、エントリ名検索を行う場合 */
										return DFMR_SAMENAME;  /* 同一エントリ名有り  */
									}
									return DFMR_OK;
								}
							}
						}
						break;
					}
				}
				l_nowslot ++;           /* 位置インクリメント                 */
				l_lfnentry ++;
			}
			if( l_bottomsect <= l_nowsect )
			{
				break;
			}
			l_nowsect ++;               /* セクタ番号インクリメント           */
			l_nowslot = 0;              /* セクタ内の先頭位置                 */
			/*********************************/
			/* R/Wバッファ切替               */
			/*********************************/
			if( ( l_mode != MODE_SRCHENTRY ) || ( l_emptyflg == 1 ) )
			{
				if( l_topentbuf == l_buffsw )
				{
					MFMR_FLGSWITCH( l_buffsw );
				}
			}
			/*********************************/
			/* 検索セクタを読込む            */
			/*********************************/
			/* I/Oパラメータ設定 */
			l_rtn = FMR_readDataSector(
				drvnum,
				l_nowsect,
				l_buffsw
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		if( l_nextclstnum >= l_eoc )
		{                               /* クラスタ単位のループ終了判定       */
			if ( l_emptyflg == 1 )
			{
				return DFMR_OK;
			}
			toppos->sectnum = l_nowsect;/* 先頭位置に検索セクタを設定         */
			if ( ( entry == NULL ) ||
				 ( ( prsflg & DFMR_SRCHFLGCHK ) == DFMR_SRCHFLGCHK ) )
			{                                   /* 空エントリ検索の場合       */
				return DFMR_ENOEMPTYENTRY;      /* 未使用/無効領域なし        */
			}
			else
			{
				return DFMR_NOENTRY;            /* 対象エントリなし           */
			}
		}
		else
		{
			l_nowsect = ( l_nextclstnum - 2 ) * MFMR_SECTPERCLST( drvnum );
			l_nowsect += MFMR_DATATOP( drvnum );    /* 次クラスタの先頭セクタ */
			l_bottomsect = l_nowsect + MFMR_SECTPERCLST( drvnum ) - 1;
			                            /* 次クラスタの最終セクタ             */
			l_nowslot = 0;              /* セクタ内の先頭位置                 */
			l_nowclstnum = l_nextclstnum;
			/* FAT読込み領域0に現在のクラスタのFAT番号が */
			/* 所属するFATセクタを読込む */
			l_rtn = FMR_getFatSector(
				MFMR_DRVINFO( drvnum ),
				MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
				l_nowclstnum,
				&l_nextclstnum
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/*********************************/
			/* R/Wバッファ切替               */
			/*********************************/
			if( ( l_mode != MODE_SRCHENTRY ) || ( l_emptyflg == 1 ) )
			{
				if( l_topentbuf == l_buffsw )
				{
					MFMR_FLGSWITCH( l_buffsw );
				}
			}
			/************************************/
			/* 対象クラスタの先頭セクタを読込む */
			/************************************/
			/* I/Oパラメータ設定 */
			l_rtn = FMR_readDataSector(
				drvnum,
				l_nowsect,
				l_buffsw
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
	}
}

F_BYTE FMR_subCompare(
	TFMR_LFNENTRY * lfnentry,           /* LFNエントリ用ポインタ              */
    F_CHAR * targetname,                /* 検索文字列                         */
    F_CHAR * getlfn,                    /* 取得ロングファイル名               */
	F_BYTE targetlen,                   /* 検索文字列長                       */
	F_BYTE getentry,                    /* 取得エントリ数                     */
	F_BYTE wcflg                        /* ワイルドカード指定フラグ           */
)
{
	F_CHAR l_namebuf[ DFMR_LFN_FNAME1SZ * sizeof( F_WORD ) ]; /* バッファバウンダリ調整 */
	F_BYTE   l_work;                    /* 取得エントリ名長                   */

	getlfn += ( getentry * DFMR_LFN_FNAMESZ );
	/* ロングファイル名(Uicode)取得 */
	FMR_memCopy(						/* バッファバウンダリ調整             */
		(F_WORD *)l_namebuf,
		( lfnentry -> name1 ),
		( DFMR_LFN_FNAME1SZ * sizeof( F_WORD ) ) 
		);
	FMR_exchgUnicodeToSJIS(
		(F_WORD *)l_namebuf,                    /* ファイル名(UTF)            */
		( getlfn ),
		DFMR_LFN_FNAME1SZ
		);
	FMR_exchgUnicodeToSJIS(
		( F_WORD * )( lfnentry -> name2 ),      /* ファイル名(UTF)            */
		( getlfn + DFMR_LFN_FNAME1SZ ),
		DFMR_LFN_FNAME2SZ
		);
	if( getentry != ( DFMR_LFN_ENTMAX - 1) ){
		FMR_exchgUnicodeToSJIS(
			( F_WORD * )( lfnentry -> name3 ),  /* ファイル名(UTF)            */
			( getlfn + DFMR_LFN_FNAME1SZ + DFMR_LFN_FNAME2SZ ),
			DFMR_LFN_FNAME3SZ
			);
	}
	if( getentry >= 1 ) 
	{
		/* 取得未完了 */
		return MODE_GETLFN;            /* モード遷移：LFN取得中  */
	}
	/* 取得完了 */
	l_work = 0;
	while( *( getlfn + l_work ) != NULL )
	{
		l_work ++;
	}
	if( ( (wcflg != 0) && (targetlen < l_work) )
	 || ( targetlen == l_work ) )
	{
		/* 文字列の比較を行う */
		if(
			FMR_compare(
				getlfn,                 /* 比較元文字列       */
				targetname,             /* 比較先文字列       */
				l_work,                 /* 比較元文字列数     */
				targetlen,              /* 比較先文字列数     */
				wcflg                   /* Wild Card指定フラグ*/
			) == DFMR_OK )
		{
			/* 一致した場合 */
			return MODE_FINISHLFN; /* モード遷移：LFN取得完了 */
		}
		else
		{
			/* 一致しなかった場合 */
			return MODE_GETLFN; /* モード遷移：エントリ検索中   */
		}
	}
	else
	{
		/* 比較文字列長が不一致 */
		return MODE_GETLFN;     /* モード遷移：エントリ検索中   */
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
