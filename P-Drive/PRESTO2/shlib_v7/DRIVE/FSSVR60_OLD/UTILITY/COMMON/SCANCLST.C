/*======================================================================
 * File name    : scanclst.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/scanclst.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: scanclst.c $
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
/* 関 数 名：FMR_scanTbdClst                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_BYTE * FAT使用状況展開領域                                */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：未使用クラスタのBADクラスタチェックを行う                        */
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

F_INT FMR_scanTbdClst(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr                    /* FAT使用状況展開領域                */
)
{
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_DWORD l_clstcnt;                  /* クラスタ番号                       */
	F_DWORD l_fatentry;                 /* FATエントリ                        */
	F_CHAR l_readbuf;                   /* 対象Byte読込み領域                 */
	F_BYTE l_bitpos;                    /* 対象Bit位置                        */
	
	/* FAT読込み領域0を初期化しておく */
	MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 ) = DFMR_INITBYTE;
	
	for( l_clstcnt = DFMR_TOPCLSTNUM; l_clstcnt < MFMR_TOTALCLST( drvnum );
		l_clstcnt ++ )
	{
		/* 対象クラスタのBitが存在する領域、対象Bit位置を取得する */
		l_readbuf = *( ramaddr + sizeof( TFMR_SDWORKAREA ) + ( l_clstcnt >> DFMR_SD_BITONCHAR ) );
		l_bitpos = l_clstcnt & DFMR_SD_BITPOS;
		
		l_readbuf &= ( F_CHAR )( 1 << l_bitpos );
		
		if( l_readbuf == DFMR_INITBYTE )/* BitがOFFの場合は                   */
		{                               /* 未使用クラスタと見なす             */
			l_rtn = FMR_chkBadClst(
				drvnum,                 /* ドライブ番号                       */
				l_clstcnt,              /* クラスタ番号                       */
				0,                      /* R/Wバッファ番号                    */
				DFMR_FLGOFF             /* 対象クラスタ判別フラグ             */
				                        /* OFF:未使用クラスタ                 */
				);
			if( l_rtn == DFMR_SD_CHKBADCLST )
			{                           /* 対象クラスタがBADクラスタの場合    */
				MFMR_GETBAD( MFMR_FATTYPE( drvnum ),l_fatentry );
				
				/* クラスタ番号に対応するFATエントリにBADクラスタを設定する */
				if( MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 )
					!= DFMR_INITBYTE )
				{
					l_rtn = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                        /* FAT読込み領域0             */
						l_clstcnt,              /* クラスタ番号               */
						&l_fatentry,            /* 取得FATエントリ            */
						DFMR_FLGON              /* FATエントリ設定            */
						);
				}
				else                    /* FATセクタが読込まれていない場合は  */
				{                       /* エラー扱いとしてセクタを読込む     */
					l_rtn = DFMR_LNG;
				}
				if( l_rtn != DFMR_OK )
				{
					if( l_rtn == DFMR_CLSTNUMERR )
					{                   /* クラスタ番号異常の場合は異常終了   */
						return l_rtn;
					}
					if( MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 )
						!= DFMR_INITBYTE )
					{                   /* FATセクタが既に読込まれている場合は*/
					                    /* ここで書込む                       */
						l_rtn = FMR_setFatSectorRetry(
							drvnum,             /* ドライブ番号               */
						    DFMR_REFFATAREA0    /* FAT読込み領域番号          */
						);
						if( l_rtn != DFMR_OK )
						{                       /* FATセクタの書込みが不可能な*/
						                        /* 場合はエラーとする         */
							return DFMR_FATALERR;
						}
					}
					/* 正常終了ではない場合、対象クラスタは */
					/* 別のFATセクタに所属と判断            */
					l_rtn = FMR_getFatSectorRetry(
						MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                        /* FAT読込み領域0             */
						l_clstcnt,              /* クラスタ番号               */
						&l_fatentry             /* 取得FATエントリ            */
						);
					if( l_rtn != DFMR_OK )
					{
						return l_rtn;
					}
					MFMR_GETBAD( MFMR_FATTYPE( drvnum ),l_fatentry );
					l_rtn = FMR_controlFatEntry(
						MFMR_DRVINFO( drvnum ), /* 論理ドライブ構成情報       */
						MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
						                        /* FAT読込み領域0             */
						l_clstcnt,              /* クラスタ番号               */
						&l_fatentry,            /* 取得FATエントリ            */
						DFMR_FLGON              /* FATエントリ設定            */
						);
					if( l_rtn != DFMR_OK )
					{
						return DFMR_OK;         /* フェールセーフ処理         */
					}
				}
				
			}
		}
	}
	
	if( MFMR_FATSECTNUM( drvnum, DFMR_REFFATAREA0 ) != DFMR_INITBYTE )
	{                                   /* 最後に読込んだFATセクタは          */
	                                    /* この時点で書込む                   */
		l_rtn = FMR_setFatSectorRetry(
			drvnum,                     /* ドライブ番号                       */
		    DFMR_REFFATAREA0            /* FAT読込み領域番号                  */
		);
		if( l_rtn != DFMR_OK )          /* FATセクタの書込みが不可能な        */
		{                               /* 場合はエラーとする                 */
			return DFMR_FATALERR;
		}
	}
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
