/*======================================================================
 * File name    : slfnent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/slfnent.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: slfnent.c $
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
/* 関 数 名：FMR_setLFNEntry                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     TFMR_DIRENTRY エントリイメージ                              */
/*         ：     F_CHAR * ロングファイル名                                   */
/*         ：     F_BYTE サム値                                               */
/*         ：     TFMR_SAERCHINFO * 先頭位置                                  */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_FATALERR                                     */
/*         ：                                                                 */
/* 概    要：ロングファイル名のエントリを書込む                               */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_setLFNEntry(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    TFMR_DIRENTRY * entry,              /* エントリイメージ                   */
    F_CHAR * lfname,                    /* ロングファイル名                   */
    F_BYTE sumvalue,                    /* サム値                             */
    TFMR_SEARCHINFO * topslot           /* 先頭位置                           */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_sectnum;                  /* 対象セクタ番号                     */
	F_DWORD l_eoc;                      /* EOCコード                          */
	F_DWORD l_fwdclstnum;               /* 前クラスタ番号                     */
	
	TFMR_LFNENTRY l_lfnentry;           /* LFNエントリイメージ                */
	TFMR_DIRENTRY * l_entryptr;         /* ディレクトリエントリ登録位置       */
	TFMR_LFNENTRY * l_lfnentryptr;      /* LFNエントリ登録位置                */
	
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	
	F_WORD l_utfbuff[ DFMR_LFN_FNAMESZ ];   /* Unicodeファイル名取得用バッファ*/
	F_WORD l_lastlfncnt;                /* 最終LFN文字数                      */
	F_WORD	l_length;						/* ファイル名文字数 */
	
	F_BYTE l_slot;                      /* セクタ内エントリ登録位置           */
	F_BYTE l_totalcnt;                  /* LFNエントリ数カウンタ              */
	F_BYTE l_buffnum;                   /* 使用バッファ番号                   */
	F_BYTE l_lfnentrycnt;               /* LFNエントリ数                      */
	F_BYTE l_newclstflg;                /* 新規クラスタ取得フラグ             */
	
	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_totalcnt = 0;                     /* エントリ数カウンタ = 0             */
	l_buffnum = topslot->bufnum;        /* バッファ番号 = 先頭位置より取得    */
	l_sectnum = topslot->sectnum;       /* セクタ番号 = 同上                  */
	l_slot = topslot->slot;
	l_lfnentrycnt = 1;                  /* LFNエントリ数                      */
	l_lastlfncnt = 0;
	for( l_work01 = 0; l_work01 < DFMR_MAXFILESZ; l_work01 ++ )
	{
		if( *( lfname + l_work01 ) == NULL )
		{
			break;
		}
		l_lastlfncnt ++;
		if( ( ( l_work01 % DFMR_LFN_FNAMESZ ) == 0 )
			&& ( l_work01 != 0 ) )
		{
			l_lfnentrycnt ++;
			l_lastlfncnt = 1;
		}
	}
	l_length = (F_WORD)l_work01++;
	MFMR_GETEOC( MFMR_FATTYPE( drvnum ), l_eoc );       /* EOC設定            */
	l_fwdclstnum = 0;                   /* 前クラスタ番号設定                 */
	l_newclstflg = 0;
	/**************************************************************************/
	/* ディレクトリエントリ登録                                               */
	/**************************************************************************/
	while( 1 )
	{
		if( l_totalcnt == l_lfnentrycnt )     
		{
			/******************************************************************/
			/* LFNエントリ登録完了                                            */
			/******************************************************************/
			/* ディレクトリエントリを作成する */
			l_work01 = l_slot * sizeof( TFMR_DIRENTRY );
			l_entryptr = ( TFMR_DIRENTRY * )&( MFMR_RWBUFF( drvnum, l_buffnum, l_work01 ) );
			*( l_entryptr ) = *( entry );

#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
			FMR_setBigEndianEntry(
				l_entryptr
			);
#endif /* DFMR_ENDIAN_BIG */

			if( l_newclstflg == 0 )
			{
				/*************************************************************/
				/* 書込み先が既存のクラスタ                                  */
				/*************************************************************/
				/* 対象セクタ番号にR/Wバッファの内容を書込む */
				SetBackupFlg(drvnum);
				l_drtn = FMR_setDataSector(
					drvnum,
					l_sectnum,
					l_buffnum
					);
				ClearBackupFlg(drvnum);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				if( l_sectnum >= MFMR_DATATOP( drvnum ) )
				{
					/* FATを更新(上書き)する */
					l_drtn = FMR_setFatSector(
						MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報   */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                            /* FAT読込み領域アドレス  */
						DFMR_REFFATAREA0            /* FAT読込み領域番号      */
						);
					if( l_drtn != DFMR_OK )
					{
						return l_drtn;
					}
				}
				topslot->sectnum = l_sectnum;
				topslot->slot = l_slot;
				return DFMR_OK;
			}
			else
			{
				/**************************************************************/
				/* 書込み先が新規に取得したクラスタ                           */
				/**************************************************************/
				/* R/Wバッファのディレクトリエントリ書込み */
				/* 位置より後を全て0で埋める               */
				l_work01 = ( l_slot + 1 ) * sizeof( TFMR_DIRENTRY ); 
				l_work02 = MFMR_BYTEPERSECT( drvnum ) - l_work01;
				FMR_memSet(
					&( MFMR_RWBUFF( drvnum, l_buffnum, l_work01 ) ),
					                    /* バッファアドレス                   */
					DFMR_INITBYTE,      /* 初期化データ                       */
					l_work02            /* バッファサイズ                     */
					);
					
				/* 対象セクタ番号にR/Wバッファの内容を書込む */
				l_drtn = FMR_setDataSector(
					drvnum,
					l_sectnum,
					l_buffnum
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				
				l_work01 = l_sectnum - MFMR_DATATOP( drvnum );
				l_work01 /= MFMR_SECTPERCLST( drvnum );
				l_work01 *= MFMR_SECTPERCLST( drvnum );
				l_work01 += ( MFMR_SECTPERCLST( drvnum ) - 1 );
				l_work01 += MFMR_DATATOP( drvnum );
				if( l_sectnum != l_work01 )
				{
					/**********************************************************/
					/* 書込んだ先がクラスタ終端のセクタではない               */
					/**********************************************************/
					l_sectnum ++;       /* 対象セクタ番号インクリメント       */
					
					/* R/Wバッファを全て0で埋める */
					FMR_memSet(
						MFMR_RWBUFFTOP( drvnum, l_buffnum ),    /* バッファアドレス */
						DFMR_INITBYTE,                          /* 初期化データ     */
						MFMR_BYTEPERSECT( drvnum )              /* バッファサイズ   */
						);
					
					while( l_sectnum <= l_work01 )
					{
						/* 対象セクタ番号にR/Wバッファの内容を書込む */
						l_drtn = FMR_setDataSector(
							drvnum,
							l_sectnum,
							l_buffnum
							);
						if( l_drtn != DFMR_OK )
						{
							return l_drtn;
						}
						
						l_sectnum ++;   /* 対象セクタ番号インクリメント      */
					}
				}
				l_drtn = FMR_setFatSector(
					MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報      */
					MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域アドレス     */
					DFMR_REFFATAREA0            /* FAT読込み領域番号         */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				topslot->sectnum = l_sectnum;
				topslot->slot = l_slot;
				return DFMR_OK;
			}
		}
		
		/* LFNエントリを作成する */
		if( l_totalcnt == 0 )         
		{                               /* 最終LFNエントリの場合              */
			l_work01 = l_lastlfncnt;    /* 最終エントリ名数取得               */
		}
		else                            /* 最終でない場合                     */
		{
			l_work01 = DFMR_LFN_FNAMESZ;    /* 変換文字列長は最大値           */
		}
		/* Unicodeファイル名取得用バッファ初期化 */
		FMR_memSet(
			&l_utfbuff[ 0 ],            /* バッファアドレス                   */
			0xFF,                       /* 初期化データ                       */
			( DFMR_LFN_FNAMESZ * sizeof( F_WORD ) )
			);                          /* バッファサイズ                     */
		if (l_work01 != DFMR_LFN_FNAMESZ) {	/* １エントリ登録のファイル名が13文字でない場合*/
			if (l_length != DFMR_MAXFILESZ) {/* NULL含む*/
				l_utfbuff[l_work01] = 0;	/* NULLを設定 */
			}
		}
		l_work02 = l_lfnentrycnt - l_totalcnt - 1;
		l_work02 *= DFMR_LFN_FNAMESZ;
		FMR_exchgSJISToUnicode(
			( lfname + l_work02 ),      /* 変換前文字列                       */
			&l_utfbuff[ 0 ],            /* 変換後文字列                       */
			( F_WORD )l_work01          /* 変換文字数                         */
			);
		
		l_lfnentry.ord = ( F_BYTE )( l_lfnentrycnt - l_totalcnt );
		                                /* 順序設定                           */
		if( l_totalcnt == 0 )           /* 最終LFNエントリの場合              */
		{
			l_lfnentry.ord |= DFMR_LFN_LAST;
		}                               /* 最終を表すBIT ON                   */
		l_lfnentry.attr = DFMR_LFN_ATTR;/* 属性:LFNエントリ                   */
		l_lfnentry.type = DFMR_INITBYTE;/* 予約領域:0                         */
		l_lfnentry.chksum = sumvalue;   /* サム値                             */
		l_lfnentry.fstcluslo = DFMR_INITBYTE;
		                                /* 開始クラスタ番号は0                */
		l_work01 = 0;
		l_work02 = DFMR_LFN_FNAME1SZ * sizeof( F_WORD );
		FMR_memCopy(
			&l_lfnentry.name1[ 0 ],     /* コピー先バッファアドレス           */
			&l_utfbuff[ l_work01 ],     /* コピー元バッファアドレス           */
			l_work02                    /* コピーサイズ                       */
			);
		l_work01 += DFMR_LFN_FNAME1SZ;
		l_work02 = DFMR_LFN_FNAME2SZ * sizeof( F_WORD );
		
		FMR_memCopy(
			&l_lfnentry.name2[ 0 ],     /* コピー先バッファアドレス           */
			&l_utfbuff[ l_work01 ],     /* コピー元バッファアドレス           */
			l_work02                    /* コピーサイズ                       */
			);
		l_work01 += DFMR_LFN_FNAME2SZ;
		l_work02 = DFMR_LFN_FNAME3SZ * sizeof( F_WORD );
		
		FMR_memCopy(
			&l_lfnentry.name3[ 0 ],     /* コピー先バッファアドレス           */
			&l_utfbuff[ l_work01 ],     /* コピー元バッファアドレス           */
			l_work02                    /* コピーサイズ                       */
			);
		/* バッファにセクタイメージを書込む */
		l_work01 = l_slot * sizeof( TFMR_DIRENTRY );   
		l_lfnentryptr = ( TFMR_LFNENTRY * )&( MFMR_RWBUFF( drvnum, l_buffnum, l_work01 ) );
		*( l_lfnentryptr ) = l_lfnentry;            
		/* カウンタ更新 */
		l_totalcnt ++;                  /* LFNエントリ数カウンタ更新          */
		l_slot ++;                      /* セクタ内エントリ登録位置更新       */
		
		l_work01 = MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_DIRENTRY );
		if( l_slot == l_work01 )
		{
			/******************************************************************/
			/* エントリ登録位置がセクタ最終                                   */
			/******************************************************************/
			/* 対象セクタ番号にR/Wバッファの内容を書込む */
			SetBackupFlg(drvnum);
			l_drtn = FMR_setDataSector(
				drvnum,
				l_sectnum,
				l_buffnum
				);
			ClearBackupFlg(drvnum);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
			
			l_slot = 0;                 /* セクタ内エントリ登録位置更新       */
			if( l_sectnum < MFMR_DATATOP( drvnum ) )
			{
				/**************************************************************/
				/* FAT12/16のルートディレクトリの場合                         */
				/**************************************************************/
				l_sectnum ++;
				if( l_sectnum == MFMR_DATATOP( drvnum ) )
				{                       /* フェ－ルセーフ処理            */
					return DFMR_MEMFULL;
				}
				if( (F_DWORD)( l_lfnentrycnt - l_totalcnt )
						< ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) ) )
				{
					/**********************************************************/
					/* 残りエントリ数が1セクタ当りのエントリ数未満            */
					/**********************************************************/
					MFMR_FLGSWITCH( l_buffnum ) /* 読込みバッファ使用番号更新 */
				}
			}
			else
			{
				/**************************************************************/
				/* FAT12/16のルートディレクトリ以外の場合                     */
				/**************************************************************/
				l_work01 = l_sectnum - MFMR_DATATOP( drvnum );
				l_work01 /= MFMR_SECTPERCLST( drvnum );
				l_fwdclstnum = l_work01 + DFMR_TOPCLSTNUM;  /* クラスタ番号保存*/
				l_work01 *= MFMR_SECTPERCLST( drvnum );
				l_work01 += ( MFMR_SECTPERCLST( drvnum ) - 1 );
				l_work01 += MFMR_DATATOP( drvnum );
				if( l_sectnum == l_work01 )
				{
					/**************************************************************/
					/* 対象セクタ番号がクラスタ終端のセクタ                       */
					/**************************************************************/
					/* 対象セクタが属するFATエントリを取得する */
					l_drtn = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),/* FAT読込み領域*/
						l_fwdclstnum,               /* クラスタ番号               */
						&l_work01,                  /* FATエントリ値              */
						DFMR_FLGOFF                 /* R/W種別:Read               */
						);
					if( l_drtn != DFMR_OK )
					{
						if( l_drtn == DFMR_NOFATENTRY )
						{                           /* FATエントリなしの場合      */
							/* 取得したクラスタ番号を含むFATセクタを読込む */
							l_drtn = FMR_getFatSector(
								MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報   */
								MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),  
														/* FAT読込み領域          */
								l_fwdclstnum,           /* クラスタ番号           */
								&l_work01               /* FATエントリ値          */
								);
							if( l_drtn != DFMR_OK )
							{
								return l_drtn;
							}
						}
						else
						{
							return l_drtn;
						}
					}
					if( l_work01 != l_eoc )
					{
						/**********************************************************/
						/* 取得したエントリがEOCではない                          */
						/**********************************************************/
						/* 取得したFATエントリが示すクラスタの */
						/* 先頭セクタ番号を対象セクタ番号とする */
						l_sectnum = FMR_getSectNum(
							l_work01,       /* クラスタ番号                       */
							0,              /* クラスタ内セクタ番号               */
							drvnum          /* ドライブ番号                       */
							);
					}
					else
					{
						/**********************************************************/
						/* 取得したエントリがEOC                                  */
						/**********************************************************/
						/* 空きクラスタを取得する */
						l_work01 = FMR_getEmptyCluster(
							drvnum,         /* 対象論理ドライブ番号               */
							l_fwdclstnum,   /* 検索開始クラスタ番号               */
							DFMR_REFFATAREA0    /* 検索FAT読込み領域              */
							);
						if( l_work01 == DFMR_LNG )
						{
							/* 空きクラスタが存在しない場合 */
							/* FAT読込み領域内で空きクラスタが存在しなかった場合  */
							/* FATセクタ管理領域より空きクラスタを取得する */
							l_rtn = FMR_searchEmptyCluster(
								drvnum,     /* 対象論理ドライブ番号               */
								&l_work01   /* 取得空きクラスタ番号               */
								);
							if( l_rtn != DFMR_OK )
							{
								return DFMR_MEMFULL;    /* エラー返却(メモリフル) */
							}
							/* 対象セクタ番号を含むクラスタのFATエントリ */
							/* ←取得したクラスタ番号                    */
							l_drtn = FMR_controlFatEntry(
								MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報   */
								MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
														/* FAT読込み領域          */
								l_fwdclstnum,           /* クラスタ番号           */
								&l_work01,              /* FATエントリ値          */
								DFMR_FLGON              /* R/W種別:Write          */
								);
							if( l_drtn != DFMR_OK )
							{
								return l_drtn;
							}
							/* 対象のFATを更新 */
							l_drtn = FMR_setFatSector(
								MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報   */
								MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
														/* FAT読込み領域          */
								DFMR_REFFATAREA0        /* FAT読込み領域番号      */
								);
							if( l_drtn != DFMR_OK )
							{
								return l_drtn;
							}
							/* 取得したクラスタ番号を含むFATセクタを読込む */
							l_drtn = FMR_getFatSector(
								MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報   */
								MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),  
														/* FAT読込み領域          */
								l_work01,               /* クラスタ番号           */
								&l_work03               /* FATエントリ値          */
								);
							if( l_drtn != DFMR_OK )
							{
								return l_drtn;
							}
						}
						else
						{
							/* 対象セクタ番号を含むクラスタのFATエントリ */
							/* ←取得したクラスタ番号                    */
							l_drtn = FMR_controlFatEntry(
								MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報       */
								MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),/* FAT読込み領域*/
								l_fwdclstnum,               /* クラスタ番号               */
								&l_work01,                  /* FATエントリ値              */
								DFMR_FLGON                  /* R/W種別:Write              */
								);
							if( l_drtn != DFMR_OK )
							{
								return l_drtn;
							}
						}
						/* 取得したクラスタのFATエントリの値をEOCとする */
						l_drtn = FMR_controlFatEntry(
							MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報       */
							MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),/* FAT読込み領域*/
							l_work01,                   /* クラスタ番号               */
							&l_eoc,                     /* FATエントリ値              */
							DFMR_FLGON                  /* R/W種別:Write              */
							);
						if( l_drtn != DFMR_OK )
						{
							return l_drtn;
						}
						/* 取得したクラスタの先頭セクタ番号を求める */
						l_work03 = FMR_getSectNum(
							l_work01,       /* クラスタ番号                       */
							0,              /* クラスタ内セクタ番号               */
							drvnum          /* ドライブ番号                       */
							);
						/* 対象セクタ番号更新 */
						l_sectnum = l_work03;
						
						l_newclstflg = 1;
					}
					
				}
				else
				{
					/**************************************************************/
					/* 対象セクタ番号がクラスタ終端のセクタ以外                   */
					/**************************************************************/
					l_sectnum ++;           /* セクタ番号インクリメント           */
				}
				if( (F_DWORD)( l_lfnentrycnt - l_totalcnt )
					< ( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) ) )
				{
					/**********************************************************/
					/* 残りエントリ数が1セクタ当りのエントリ数未満            */
					/**********************************************************/
					MFMR_FLGSWITCH( l_buffnum ) /* 読込みバッファ使用番号更新 */
				}
			}
		}
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
