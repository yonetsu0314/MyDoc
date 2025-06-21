/*======================================================================
 * File name    : makentry.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/makentry.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: makentry.c $
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
/* 関 数 名：FMR_makeEntry                                                    */
/*         ：                                                                 */
/* パラメタ：IN:  TFMR_DIRENTRY * エントリイメージ作成領域                    */
/*         ：     F_CHAR * ファイル名称                                       */
/*         ：     F_BYTE 属性                                                 */
/*         ：     F_DWORD 先頭クラスタ番号                                    */
/*         ：     F_DWORD ファイルサイズ                                      */
/*         ：OUT：TFMR_DIRENTRY * エントリイメージ作成領域                    */
/*         ：                                                                 */
/* 戻 り 値：なし                                                             */
/*         ：                                                                 */
/* 概    要：ディレクトリエントリのイメージを作成する                         */
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

F_VOID FMR_makeEntry(
	TFMR_DIRENTRY * entry,              /* エントリイメージ作成領域           */
	F_CHAR * name,                      /* ファイル名称                       */
	F_BYTE attr,                        /* 属性                               */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD size                        /* ファイルサイズ                     */
)
{
	F_WORD l_day;                       /* 日付取得用                         */
	F_WORD l_time;                      /* 時刻取得用                         */
	F_BYTE l_mtime;                     /* ミリ秒取得用                       */
	F_BYTE l_cnt;                       /* ループカウンタ                     */
#ifdef DFMR_ENDIAN_BIG
	T_ENDIAN l_endian;                  /* Big Endian変換テーブル             */
#endif /* DFMR_ENDIAN_BIG */
	
	/* ファイル名/拡張子の設定 */
	for( l_cnt = 0; l_cnt < DFMR_FNAMESZ; l_cnt ++ )
	{                                   /* 名称領域初期化                     */
		entry->name[ l_cnt ] = DFMR_SPACE;
	}
	for( l_cnt = 0; l_cnt < DFMR_EXTSZ; l_cnt ++ )
	{                                   /* 拡張子領域初期化                   */
		entry->extension[ l_cnt ] = DFMR_SPACE;
	}
	
	if( ( *( name ) == '.' )
		&& ( *( name + 1 ) == NULL ) )
	{
		entry->name[ 0 ] = '.';
	}
	else if( ( *( name ) == '.' )
		&& ( *( name + 1 ) == '.' )
		&& ( *( name + 2 ) == NULL ) )
	{
		entry->name[ 0 ] = '.';
		entry->name[ 1 ] = '.';
	}
	else
	{
		for( l_cnt = 0; l_cnt < DFMR_FNAMESZ; l_cnt ++ )    /* ファイル名設定     */
		{                                   /* 終了条件①：NULL終端               */
		                                    /*         ②：8文字                  */
		                                    /*         ③：｢.｣検出                */
			if( ( *( name ) == NULL )
				|| ( *( name ) == '.' ) )
			{
				break;
			}
			
			entry->name[ l_cnt ] = *( name );
			name ++;
		}
		
		if( *( name ) == '.' )
		{                                   /* 拡張子ありの場合                   */
			name ++;                        /* ｢.｣を読み飛ばす                    */
			for( l_cnt = 0; l_cnt < DFMR_EXTSZ; l_cnt ++ )  /* 拡張子設定         */
			{                               /* 終了条件①：NULL終端               */
			                                /*         ②：3文字                  */
				if( *( name ) == NULL )
				{
					break;
				}
				
				entry->extension[ l_cnt ] = *( name );
				name ++;
			}
		}
	}
	
	/* 属性の設定 */
	entry->attr = attr;
	
	/* 予約領域(WinNT使用箇所)の設定 */
	entry->ntres = DFMR_INITBYTE;
	
	/* 先頭クラスタ番号の設定 */
#ifdef DFMR_ENDIAN_BIG                          /* Big Endianの場合           */
	l_endian.dwordval = topclstnum;
	entry->fstclushi = ( ( F_WORD )( l_endian.byteval[ 1 ] ) ) << 8;
	entry->fstclushi |= ( F_WORD )( l_endian.byteval[ 0 ] );
	
	entry->fstcluslo = ( ( F_WORD )( l_endian.byteval[ 3 ] ) ) << 8 ;
	entry->fstcluslo |= ( F_WORD )( l_endian.byteval[ 2 ] );
	
#else /* DFMR_ENDIAN_BIG */                     /* Little Endianの場合        */
	entry->fstclushi = ( F_WORD )( ( topclstnum >> 16 ) && 0x0000FFFF );
	entry->fstcluslo = ( F_WORD )( ( topclstnum  ) && 0x0000FFFF );
#endif /* DFMR_ENDIAN_BIG */

	/* 日付/時刻の設定 */
	FMR_osGetTime(
		&l_day,                         /* 取得年月日設定領域                 */
		&l_time,                        /* 取得時分秒設定領域                 */
		&l_mtime                        /* 取得ミリ秒設定領域                 */
		);
#ifdef DFMR_ENDIAN_BIG                          /* Big Endianの場合           */
	l_endian.wordval = l_day;
	l_day = ( ( F_WORD )( l_endian.byteval[ 1 ] ) )  << 8;
	l_day |= ( ( F_WORD )( l_endian.byteval[ 0 ] ) );
	l_endian.wordval = l_time;
	l_time = ( ( F_WORD )( l_endian.byteval[ 1 ] ) )  << 8;
	l_time |= ( ( F_WORD )( l_endian.byteval[ 0 ] ) );
#endif /* DFMR_ENDIAN_BIG */
	entry->crttimetenth = l_mtime;      /* 作成時刻(msec)                     */
	entry->crttime = l_time;            /* 作成時刻                           */
	entry->crtdate = l_day;             /* 作成日付                           */
	entry->lstaccdate = l_day;          /* アクセス日                         */
	entry->wrttime = l_time;            /* 更新時刻                           */
	entry->wrtdate = l_day;             /* 更新日付                           */
	
	/* ファイルサイズの設定 */
#ifdef DFMR_ENDIAN_BIG                          /* Big Endianの場合           */
	l_endian.dwordval = size;
	entry->filesize = ( ( F_DWORD )( l_endian.byteval[ 3 ] ) ) << 24;
	entry->filesize |= ( ( F_DWORD )( l_endian.byteval[ 2 ] ) ) << 16;
	entry->filesize |= ( ( F_DWORD )( l_endian.byteval[ 1 ] ) ) << 8;
	entry->filesize |= ( F_DWORD )( l_endian.byteval[ 0 ] );
	
#else /* DFMR_ENDIAN_BIG */                     /* Little Endianの場合        */
	entry->filesize = size;
#endif /* DFMR_ENDIAN_BIG */
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
