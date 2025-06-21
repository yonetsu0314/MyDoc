/*======================================================================
 * File name    : setdent.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/setdent.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: setdent.c $
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
/* 関 数 名：FMR_setDirEntry                                                  */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_SEARCHINFO * 検索終了先頭位置                          */
/*         ：     TFMR_SAERCHINFO * 検索終了最終位置                          */
/*         ：     TFMR_DIRENTRY *   ディレクトリエントリ作成領域              */
/*         ：     F_CHAR *          設定ファイル名                            */
/*         ：     F_CHAR *          設定ショートファイル名                    */
/*         ：     F_WORD            動作フラグ                                */
/*         ：                      bit11:ON=Open,bit10:ON=Rename(File),       */
/*         ：                      bit9:ON=Rename(Directory),bit8:ON=Mkdir    */
/*         ：                      bit7-0:FMR_searchEntry 動作フラグと同じ    */
/*         ：     F_CHAR            対象ドライブ番号                          */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ディレクトリエントリ.ファイル名設定&登録                         */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/* 01.07 R/Wバッファ上のエントリのみエンディアン変換     05/12/12 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_setDirEntry(
    TFMR_SEARCHINFO * toppos,           /* 検索終了先頭位置                   */
    TFMR_SEARCHINFO * bottompos,        /* 検索終了最終位置                   */
	TFMR_DIRENTRY *   entry,            /* ディレクトリエントリ作成領域       */
	F_CHAR *          filename,         /* 設定ファイル名                     */
	F_CHAR *          shortname,        /* 設定ショートファイル名             */
    F_WORD            flag,             /* 動作フラグ                         */
    F_CHAR            drvnum            /* 対象ドライブ番号                   */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;

	TFMR_DIRENTRY * l_entptr;           /* エントリ格納用ポインタ             */

	F_INT   l_rtn;                      /* サブルーチン戻り値                 */

	F_BYTE l_prsflg;                    /* 検索処理パラメータ:動作フラグ      */
	F_BYTE l_work03;                    /* カウンタ、サム値                   */

	l_prsflg = ( F_BYTE )( flag & DFMR_SETFUNC_SRCH );
	l_rtn = DFMR_OK;
	if( ( l_prsflg & DFMR_SRCHFLGLFN ) == DFMR_SRCHFLGLFN )
	{
		/**********************************************************************/
		/* 対象エントリがロングファイル名の場合                               */
		/**********************************************************************/
		/* エントリを登録する位置がRoot Dir.の場合は    */
		/* 十分な空きエントリ位置が存在するかを確認する */
		if( toppos->sectnum < MFMR_DATATOP( drvnum ) )
		{                               /* 登録位置がRoot Dir. Regionの場合   */
			l_work01 = ( F_DWORD )( l_prsflg & DFMR_SRCHLEN );  /* エントリ数取得 */
			l_work02 = ( F_DWORD )( MFMR_BYTEPERSECT( drvnum ) / sizeof( TFMR_LFNENTRY ) );
			if( l_work01 > ( l_work02 - ( F_DWORD )toppos->slot ) )
			{                           /* 登録位置から1セクタ以内に          */
			                            /* 収まらない場合                     */
				l_work01 -= l_work02 - toppos->slot;
				l_work01 += l_work02 - 1;
				l_work01 /= l_work02;   /* 書込み開始位置が属するセクタから   */
				                        /* 何セクタ分跨るかを求める           */
				if( ( toppos->sectnum + l_work01 ) >= MFMR_DATATOP( drvnum ) )
				{                       /* 必要なセクタがData Regionに        */
				                        /* 達する場合                         */
					return DFMR_MEMFULL;    /* エラー返却(メモリフル)         */
				}
			}
		}
		/**********************************************************************/
		/* 対象ディレクトリ/ファイル検索結果時のR/Wバッファに復旧             */
		/**********************************************************************/
		/* R/Wバッファ0復旧 */
		l_rtn = FMR_readDataSector(
			drvnum,
			toppos->sectnum,
			toppos->bufnum
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/* R/Wバッファ1復旧 */
		MFMR_FLGSWITCH( toppos->bufnum );
		if( ( bottompos->sectnum != DFMR_INITBYTE )
			&& ( bottompos->bufnum == toppos->bufnum ) )
		{
			l_rtn = FMR_readDataSector(
				drvnum,
				bottompos->sectnum,
				bottompos->bufnum
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		else
		{
			/* R/Wバッファを0で初期化しておく */
			FMR_memSet(
				MFMR_RWBUFFTOP( drvnum, toppos->bufnum ),
				DFMR_INITBYTE,
				MFMR_BYTEPERSECT( drvnum )
				);
		}
		MFMR_FLGSWITCH( toppos->bufnum );
		/**********************************************************************/
		/* ショートファイル名を設定する                                       */
		/**********************************************************************/
		for( l_work03 = 0; l_work03 < DFMR_FNAMESZ; l_work03 ++, shortname ++ )
		{                               /* ファイル名設定                     */
			if ( ( flag & ( DFMR_SETFUNC_MKDIR + DFMR_SETFUNC_RENAMED ) ) != 0 )
			{
				/* Mkdir,Rename(Directorty)の場合 */
				if( *( shortname ) == DFMR_INITBYTE )
				{
					break;
				}
			}
			else
			{
				/* Open,Rename(File)の場合 */
				if( ( *( shortname ) == '.' )
					|| ( *( shortname ) == DFMR_INITBYTE ) )
				{
					break;
				}
			}
			entry->name[ l_work03 ] = *( shortname );
		}
		if ( ( flag & ( DFMR_SETFUNC_RENAMEF + DFMR_SETFUNC_OPEN ) ) != 0 )
		{
			/* Rename(File),Openの場合 */
			if( *( shortname ) == '.' )
			{                           /* 拡張子がある場合                   */
				shortname ++;
				for( l_work03 = 0; l_work03 < DFMR_EXTSZ; l_work03 ++, shortname ++ )
				{                       /* 拡張子設定                         */
					if( *( shortname ) == DFMR_INITBYTE )
					{
						break;
					}
					entry->extension[ l_work03 ] = *( shortname );
				}
			}
		}
		/**********************************************************************/
		/* ショートファイル名のサム値を求める                                 */
		/**********************************************************************/
		l_work03 = FMR_getChkSum(
			( F_CHAR * )&entry->name    /* ショートファイル名                 */
			);
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
		if ( ( flag & ( DFMR_SETFUNC_RENAMED + DFMR_SETFUNC_RENAMEF ) ) != 0 )
		{
			/* Rename(Directory,File)の場合 */
			FMR_setBigEndianEntry( /* FMR_setLFNEntryの都合上、一度BigEndianに変換 */
				entry
				);
		}
#endif /* DFMR_ENDIAN_BIG */
		/* エントリを登録する */
		l_rtn = FMR_setLFNEntry(
			drvnum,                     /* 対象論理ドライブ番号               */
			entry,                      /* エントリイメージ                   */
			filename,                   /* ロングファイル名                   */
			l_work03,                   /* サム値                             */
			toppos                      /* 先頭位置                           */
			);
	}
	else
	{
		/**********************************************************************/
		/* 対象エントリがショートファイル名の場合                             */
		/**********************************************************************/
		/* ファイル名を設定 */
		for( l_work03 = 0; l_work03 < DFMR_FNAMESZ; l_work03 ++, filename ++ )
		{                               /* ファイル名をセット                 */
			if( ( *( filename )  == '.' )
				|| ( *( filename ) == DFMR_INITBYTE ) )
			{
				break;
			}
			FMR_smallToCapital(
				filename,
				(F_CHAR *)&entry->name[ l_work03 ],
				1
				);
		}
		if( *( filename ) == '.' )
		{                               /* 拡張子がある場合                   */
			filename ++;
			for( l_work03 = 0; l_work03 < DFMR_EXTSZ; l_work03 ++, filename ++ )
			{                           /* 拡張子をセット                     */
				if( *( filename ) == DFMR_INITBYTE )
				{
					break;
				}
				FMR_smallToCapital(
					filename,
					(F_CHAR *)&entry->extension[ l_work03 ],
					1
					);
			}
		}
		/**********************************************************************/
		/* 空きエントリ位置に対象エントリを登録する                           */
		/**********************************************************************/
		/* R/Wバッファの対象位置にエントリイメージを書込む */
		l_entptr = ( TFMR_DIRENTRY * )( MFMR_RWBUFFTOP( drvnum, toppos->bufnum ) );
		l_entptr += toppos->slot;
		*( l_entptr ) = *( entry );
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
		if ( ( flag & ( DFMR_SETFUNC_MKDIR + DFMR_SETFUNC_OPEN ) ) != 0 )
		{
			/* Mkdir,Openの場合 */
			FMR_setBigEndianEntry(
				l_entptr
				);
		}
#endif /* DFMR_ENDIAN_BIG */

		/* 対象のR/Wバッファを書込む */
		SetBackupFlg(drvnum);
		l_rtn = FMR_setDataSector(
			drvnum,
			toppos->sectnum,
			toppos->bufnum
			);
		ClearBackupFlg(drvnum);
	}
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
