/*======================================================================
 * File name    : gshtname.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/gshtname.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: gshtname.c $
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
/* 関 数 名：FMR_getShortName                                                 */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR 対象論理ドライブ番号                                 */
/*         ：     F_CHAR * 変換前文字列                                       */
/*         ：     F_WORD 変換前文字列長                                       */
/*         ：     F_BYTE 対象エントリ種別                                     */
/*         ：     F_DWORD カレントクラスタ番号                                */
/*         ：                                                                 */
/*         ：OUT: F_CHAR * 変換後文字列                                       */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：ロングファイル名よりショートファイル名を求める                   */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"
static F_DWORD FMR_power10Base(         /* 10の階乗計算を行う                 */
	F_BYTE power                        /* 指数                               */
);

F_INT FMR_getShortName(
	F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
	F_CHAR * targetname,                /* 変換前文字列                       */
	F_WORD targetlen,                   /* 変換前文字列長                     */
	F_BYTE targetentry,                 /* 対象エントリ種別                   */
	F_CHAR * outstring,                 /* 変換後文字列                       */
	F_DWORD cclstnum                    /* カレントクラスタ番号               */
)
{
	F_DWORD l_samecnt;                  /* 同名エントリカウンタ               */
	F_DWORD l_samelimit;                /* 同名エントリカウンタ最大値         */
	F_DWORD l_work2;

	F_INT l_rtn;                        /* エントリ検索処理戻り値             */

	TFMR_SEARCHINFO srchpos;            /* 検索開始位置                       */
	TFMR_DIRENTRY l_getentry;           /* 取得エントリ格納領域               */

	F_WORD l_extpos;                    /* 拡張子先頭位置                     */
	F_WORD l_extlen;                    /* 拡張子文字数                       */
	F_WORD l_lnamepos;                  /* 長ファイル名検索位置               */
	F_WORD l_namelen;                   /* ファイル名文字数                   */
	F_WORD l_work3;

	F_BYTE l_snamelen;                  /* 短ファイル名長                     */
	F_BYTE l_figcnt;                    /* 同名エントリ桁カウンタ             */
	F_BYTE l_maxtail;                   /* 最大末尾部数                       */
	F_BYTE l_work;                      /* ワークエリア                       */

	/*********************************/
	/* 拡張子の位置、長さ設定        */
	/*********************************/
	/* 拡張子先頭位置←変換前文字列長 */
	l_extpos = targetlen;
	l_namelen = targetlen;              /* ファイル名文字数←対象文字列長     */

	/* バッファを0で埋める */
	FMR_memSet(                         /*                                    */
		outstring,                      /*                                    */
		DFMR_INITBYTE,                  /*                                    */
		( DFMR_FNAMESZ + DFMR_EXTSZ + 2 )   /* '.' + NULL終端                 */
		);                              /*                                    */
	/* 対象の属性判定 */
	if( targetentry == DFMR_ATTR_DIR )  /* 対象がディレクトリ                 */
	{
		l_extlen = 0;                   /* 拡張子文字数←0                    */
	}
	else                                /* 対象がファイル                     */
	{
		while( l_extpos > 0 )           /* 拡張子先頭位置が0となるまで        */
		{
			/* 検索位置の文字判定 */
			if( *( targetname + l_extpos ) == '.' )
			{                           /* ピリオドは拡張子と判断             */
				/* 拡張子文字数設定 */
				l_extlen = ( F_WORD )( targetlen - l_extpos - 1 );
				if( l_extlen > DFMR_EXTSZ )
				{                       /* 3文字以上は3文字に丸め込む         */
					l_extlen = DFMR_EXTSZ;
				}
				l_namelen = l_extpos;
				l_work3 = l_namelen;
				if( l_work3 > DFMR_FNAMESZ )
				{
					l_work3 = DFMR_FNAMESZ;
				}
				*( outstring + l_work3 ) = '.';
				l_extpos ++;            /* 1文字戻す                          */
				break;
			}
			else
			{
				l_extpos --;            /* 拡張子文字数更新                   */
			}
		}
		/* 拡張子が存在しなかった場合 */
		if( l_extpos == 0 )
		{
			l_extpos = targetlen;       /* 拡張子位置は名称終端               */
			l_extlen = 0;               /* 拡張子長は0                        */
		}
	}
	/****************/
	/* 拡張子の設定 */
	/****************/
	l_work = 0;                         /*                                    */
	l_work2 = l_extlen;                 /*                                    */
	for( l_work2 = l_extlen; l_work2 > 0; l_work2 -- ) /*                     */
	{                                   /* 拡張子設定完了まで                 */
		switch( *( targetname + l_extpos + l_work ) )  /*                     */
		{                               /*                                    */
		case DFMR_SPACE:                /* 半角スペース                       */
			break;                      /* 処理なし(読み飛ばす)               */
		case '+':                       /* 長ファイル名でのみ許容されている   */
		case ',':                       /* 文字はLOW LINEに置き換える         */
		case ';':                       /*                                    */
		case '=':                       /*                                    */
		case '[':                       /*                                    */
		case ']':                       /*                                    */
			*( outstring + l_work3 + l_work + 1 ) = '_';   /*                 */
			l_work ++;                  /*                                    */
			break;                      /*                                    */
		default:                        /* その他                             */
			FMR_smallToCapital(         /* 大文字に変換                       */
				( targetname + l_extpos + l_work ),    /* 変換前文字列        */
				( outstring + l_work3 + l_work + 1),   /* 変換後文字列        */
				1                       /* 1文字                              */
				);                      /*                                    */
			l_work ++;                  /*                                    */
			break;                      /*                                    */
		}                               /*                                    */
	}                                   /*                                    */
	
	/*********************************/
	/* 短ファイル名の本体部作成      */
	/*********************************/
	l_lnamepos = 0;                     /* 長ファイル名検索位置←0            */
	l_snamelen = 0;                     /* 短ファイル名長←0                  */
	
	while( l_snamelen < l_namelen )
	{                                   /* 短ファイル名本体部は6文字まで      */
		/* 対象文字による分岐 */
		switch( *( targetname + l_lnamepos ) )
		{
		case DFMR_SPACE:                /* 半角スペース                       */
			break;                      /* 処理なし(読み飛ばす)               */
		case '+':                       /* 長ファイル名でのみ                 */
		case ',':                       /* 許容されている文字                 */
		case ';':                       /* LOW LINEに置き換える               */
		case '=':
		case '[':
		case ']':
			*( outstring + l_snamelen ) = '_';
			l_snamelen ++;
			break;
		default:                        /* その他                             */
			FMR_smallToCapital(         /* 大文字に変換                       */
				( targetname + l_lnamepos ),   /* 変換前文字列                */
				( outstring + l_snamelen ),    /* 変換後文字列                */
				1                              /* 1文字                       */
				);

			l_snamelen ++;
			break;
		}
		
		/* 長ファイル名更新 */
		l_lnamepos ++;                  /* 長ファイル名インクリメント         */
		
		/* 短ファイル名は8文字まで */
		if( l_lnamepos >= DFMR_FNAMESZ )
		{
			break;
		}
	}
	/* ファイル名文字数が8文字以下の場合は処理終了 */
	if( l_namelen <= DFMR_FNAMESZ )
	{
		return DFMR_OK;
	}
	/* 短ファイル名文字数が6文字以上の場合は本体部を6文字に丸め込む */
	if( l_snamelen > ( DFMR_FNAMESZ - 2 ) )
	{
		l_snamelen = DFMR_FNAMESZ - 2;
	}
	/*********************************/
	/* 本体部が同じエントリを検索    */
	/*********************************/
	/* パラメータ設定 */
	l_samecnt = 0;                      /* 同名エントリカウンタ←0            */
	l_figcnt = 1;                       /* 同名エントリ桁カウンタ←1          */
	l_maxtail = 0;                      /* 最大末尾部数←0                    */
	*( outstring + l_snamelen ) = '~';  /* 変換後文字列の                     */
	                                    /* ｢短ファイル名長｣Byte目←'~'        */
	*( outstring + l_snamelen + 1 )     /* 変換後文字列の                     */
	 = '?';                             /* ｢短ファイル名長+1｣Byte目←'?'      */
	
	if( cclstnum == 0 )                 /* 検索開始位置がRoot Dir. Region     */
	{
		srchpos.sectnum = MFMR_ROOTTOP( drvnum );   /* 検索開始位置           */
		srchpos.slot = DFMR_SRCHROOTTOPSLOT;   /* ←カレントディレクトリの先頭 */
	}
	else                                /* 検索開始位置がData Region          */
	{
		srchpos.sectnum = cclstnum - 2; /* 検索開始位置←カレントディレクトリ */
		srchpos.sectnum *= MFMR_SECTPERCLST( drvnum );  /* の先頭             */
		srchpos.sectnum += MFMR_DATATOP( drvnum );
		srchpos.slot = DFMR_SRCHDIRTOPSLOT;
	}
	srchpos.bufnum = 0;
	
	while( 1 )                          /* 本体部分が同じエントリの検索が     */
	{                                   /* 完了するまで                       */
		/* 数値部の最大値作成 */
		l_samelimit = FMR_power10Base( l_figcnt );
		
		/* 数値部の桁単位の検索 */
		while( l_samecnt < l_samelimit )
		{
			/* エントリを検索する */
			for( l_work3 = 0; l_work3 < DFMR_MAXFILESZ ; l_work3 ++ )
			{
				if( ( *( outstring + l_work3 ) ) == NULL )
				{
					break;
				}
			}
			l_rtn = FMR_searchEntry(
				drvnum,                 /* 対象論理ドライブ番号               */
				&srchpos,               /* 検索開始位置                       */
				NULL,                   /* 検索終了最終位置(NULL指定)         */
				outstring,              /* 検索文字列                         */
				&l_getentry,            /* 取得エントリ格納領域               */
				NULL,                   /* 取得ロングファイル名(NULL指定)     */
				( F_BYTE)l_work3,       /* 検索文字列長                       */
				( 0x01 | DFMR_SRCHFLGWC )   /* 動作フラグ                     */
				);
			
			/* 検索結果による判定 */
			if( l_rtn == DFMR_NOENTRY ) /* 対象エントリが存在しない場合       */
			{
				/* 短ファイル名長設定 */
				for( l_work = 0; l_work < l_figcnt; l_work ++ )
				{
					*( outstring + l_snamelen + l_work + 1 )
					 = ( F_BYTE )( ( l_maxtail / FMR_power10Base( ( F_BYTE )( l_figcnt - l_work - 1 ) ) )
					 + '0' );
					l_maxtail = ( F_BYTE )( l_maxtail % FMR_power10Base( ( F_BYTE )( l_figcnt - l_work - 1 ) ) );
				}
				return DFMR_OK;         /* 正常終了                           */
			}
			else if( l_rtn == DFMR_OK ) /* 正常終了の場合                     */
			{
				/* 検索したエントリの末尾の数値部を取得 */
				l_work2 = 0;
				for( l_work = 0; l_work < l_figcnt; l_work ++ )
				{
					if( ( '0' <= l_getentry.name[ l_snamelen + l_work + 1 ] )
					 && ( l_getentry.name[ l_snamelen + l_work + 1 ] <= '9' ) )
					{
						l_work2 
						 += ( l_getentry.name[ l_snamelen + l_work + 1 ] - '0' )
						 * ( FMR_power10Base( ( F_BYTE )( l_figcnt - l_work ) ) / 10 );
					}
					else                /* 数値部に数値以外の文字が使用されて */
					{                   /* いた場合は人造物として処理         */
						continue;       /* 検索続行                           */
					}
				}
				/* 検索したエントリの末尾の数値部判定 */
				if( l_maxtail <= l_work2 )
				{                       /* 末尾の文字が最大末尾部数以上の場合 */
					l_maxtail = ( F_BYTE )( l_work2 + 1 );
					if( l_work2 == ( l_samelimit - 1 ) )
					{                   /* 末尾の文字が現在の桁数の最大値     */
						break;
					}
				}
				/* 変数設定 */
				l_rtn = FMR_skipSearchPosition( /* 検索開始位置設定           */
					drvnum,             /* 対象論理ドライブ番号               */
					DFMR_REFFATAREA0,   /* FAT読込み領域                      */
					&srchpos            /* 検索開始位置                       */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				l_samecnt ++;           /* 同名エントリカウンタ更新           */
			}
			else                        /* 内部でエラーが発生した場合         */
			{
				return l_rtn;
			}
		}
		/* 短ファイル名長が0の場合はこれ以上短く出来ないのでエラー返却 */
		if( l_snamelen == 0 )
		{
			return DFMR_EEXIST;         /* 既存ファイルあり                   */
		}
		l_figcnt ++;                    /* 同名エントリ桁カウンタ更新         */
		if( cclstnum == 0 )             /* 検索開始位置がRoot Dir. Region     */
		{
			srchpos.sectnum = MFMR_ROOTTOP( drvnum );   /* 検索開始位置       */
			srchpos.slot = DFMR_SRCHROOTTOPSLOT;   /* ←カレントディレクトリの先頭 */
		}
		else                            /* 検索開始位置がData Region          */
		{
			srchpos.sectnum = cclstnum - 2; /* 検索開始位置←カレント         */
			srchpos.sectnum *= MFMR_SECTPERCLST( drvnum );  /* ディレクトリの */
			srchpos.sectnum += MFMR_DATATOP( drvnum );      /* 先頭           */
			srchpos.slot = DFMR_SRCHDIRTOPSLOT;
		}
		srchpos.slot = DFMR_SRCHROOTTOPSLOT;    /* ←カレントディレクトリの先頭 */
		srchpos.bufnum = 0;
		
		/* 短ファイル名が8文字まで使用している場合は本体部を1文字削る */
		if( ( l_snamelen + l_figcnt ) >= DFMR_FNAMESZ )
		{
			l_snamelen --;
		}
		*( outstring + l_snamelen ) = '~';      /* 変更後文字列を更新         */
		for( l_work = 0; l_work < l_figcnt; l_work ++ )
		{
			*( outstring + l_snamelen + l_work + 1 ) = '?';
		}
	}
}

/* 10の階乗計算を行う */
static F_DWORD FMR_power10Base(
	F_BYTE power                        /* 指数                               */
)
{
	F_BYTE l_cnt;                       /* ループカウンタ                     */
	F_DWORD l_ret;                      /* 戻り値                             */

	l_ret = 1;
	for( l_cnt = 0; l_cnt < power; l_cnt ++ )
	{
		l_ret *= 10;
	}
	return l_ret;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
