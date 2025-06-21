/*======================================================================
 * File name    : spltpath.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/spltpath.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: spltpath.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
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
/* 関 数 名：FMR_splitPath                                                    */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR * パス名                                             */
/*         ：     F_BYTE * FMR_searchEntry用動作フラグ                        */
/*         ：               bit5:ON=ワイルドカードチェック指定                */
/*         ：                                                                 */
/*         ：OUT: F_CHAR * 区切り記号                                         */
/*         ：     F_BYTE * FMR_searchEntry用動作フラグ                        */
/*         ：                                                                 */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：F_SHORT 正常終了 文字列長                                        */
/*         ：        異常終了 DFMR_NG(文字列長超過)                           */
/*         ：                                                                 */
/* 概    要：パス名よりファイル/ディレクトリ名を取得する                      */
/*         ：                                                                 */
/* 作 成 日：2004.04.**                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.05 戻り値の型をF_WORDからF_SHORTに変更             05/02/28 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_SHORT FMR_splitPath(
    F_CHAR * pathname,                  /* パス名                             */
    F_CHAR * delimiter,                 /* 区切り記号                         */
    F_BYTE * prsflg                     /* FMR_searchEntry用動作フラグ        */
)
{
	F_WORD l_cnt;                       /* 文字列長                           */
	F_BYTE l_lfnflg;                    /* LFNフラグ                          */
	                                    /* Bit7 ON:ロングファイル名           */
	                                    /* Bit7 OFF:ショートファイル名        */
	                                    /* Bit6 ON:ワイルドカードあり         */
	                                    /* Bit1 ON:ファイル名に英小文字あり   */
	                                    /* Bit0 ON:ファイル名に英大文字あり   */
	F_BYTE l_namecnt;                   /* ファイル名カウンタ                 */
	F_BYTE l_extcnt;                    /* 拡張子カウンタ                     */
	
	/* 初期設定を行う */
	l_namecnt = 0;                      /* ファイル名カウンタ初期値           */
	l_extcnt = 0;                       /* 拡張子カウンタ初期値               */
	l_lfnflg = 0;                       /* LFNフラグ = ショートファイル名     */
	
	for( l_cnt = 0; l_cnt < DFMR_MAXFILESZ; l_cnt ++ )
	{
		/* 区切り記号発見 */
		if( *( pathname ) == '\\' || *( pathname ) == NULL )
		{
			/* パス名より区切り記号を取得 */
			*( delimiter ) = *( pathname );
			if( ( *( prsflg ) & DFMR_SRCHFLGWC ) == DFMR_SRCHFLGWC )
			{							/* ワイルドカードチェック指定の場合 */
				if( ( ( l_lfnflg & 0x40 ) == 0 ) &&
					( *( pathname ) == NULL ) )
				{
				/* ワイルドカードなしの場合 */
					*( prsflg ) &= ~DFMR_SRCHFLGWC;
				}
			}
			if( ( l_lfnflg & 0x80 ) == 0 )
			{
			/* ショートファイル名の場合 */
				*( prsflg ) |= DFMR_SRCHFLGSFN;
			}
			else
			{
			/* ロングファイル名の場合 */
				l_lfnflg = (F_BYTE)( (l_cnt + DFMR_LFN_FNAMESZ - 1) / DFMR_LFN_FNAMESZ );
												/* エントリ数取得                     */
				l_lfnflg ++;                    /* ショートファイル名の分を加算       */
				l_lfnflg &= DFMR_SRCHLEN;
				l_lfnflg |= DFMR_SRCHFLGLFN;
				*( prsflg ) |= l_lfnflg;
			}
			/* 正常終了 */
			return l_cnt;
		}
		/* 無効文字チェック */
		if( ( '0' <= *( pathname ) && *( pathname ) <= '9' )
		|| ( 'a' <= *( pathname ) && *( pathname ) <= 'z' )
		|| ( 'A' <= *( pathname ) && *( pathname ) <= 'Z' )
		|| ( *( pathname ) == '!' )     /* パス名の内容が半角の有効文字       */
		|| ( *( pathname ) == '#' )     /* '.'は別でカウントするのでここでは  */
		|| ( *( pathname ) == '$' )     /* 考慮しない                         */
		|| ( *( pathname ) == '%' )
		|| ( *( pathname ) == '&' )
		|| ( *( pathname ) == '\'' )
		|| ( *( pathname ) == '(' )
		|| ( *( pathname ) == ')' )
		|| ( *( pathname ) == '+' )
		|| ( *( pathname ) == ',' )
		|| ( *( pathname ) == '-' )
		|| ( *( pathname ) == ';' )
		|| ( *( pathname ) == '=' )
		|| ( *( pathname ) == '@' )
		|| ( *( pathname ) == '[' )
		|| ( *( pathname ) == ']' )
		|| ( *( pathname ) == '^' )
		|| ( *( pathname ) == '_' )
		|| ( *( pathname ) == '`' )
		|| ( *( pathname ) == '{' )
		|| ( *( pathname ) == '}' )
		|| ( *( pathname ) == '~' )
		)
		{
			if( ( l_lfnflg & 0x80 ) == 0 )  /* ショートファイル名の場合       */
			{
				if( ( *( pathname ) == '+' )/* パス名の内容が                 */
				|| ( *( pathname ) == ',' ) /* LFNでのみ有効な値              */
				|| ( *( pathname ) == ';' )
				|| ( *( pathname ) == '=' )
				|| ( *( pathname ) == '[' )
				|| ( *( pathname ) == ']' )
				)
				{
					l_lfnflg |= 0x80;   /* LFNとする                          */
				}
				else if( ( 'a' <= *( pathname ) && *( pathname ) <= 'z' ) )
				{                       /* 英小文字の場合                     */
					if( ( l_lfnflg & 0x01 ) == 0x01 )
					{                   /* 英大文字が以前に存在した場合       */
						l_lfnflg |= 0x80;   /* LFNとする                      */
					}
					else
					{
						l_lfnflg |= 0x02;   /* 英小文字ありとする             */
					}
				}
				else if( ( 'A' <= *( pathname ) && *( pathname ) <= 'Z' ) )
				{                       /* 英大文字の場合                     */
					if( ( l_lfnflg & 0x02 ) == 0x02 )
					{                   /* 英小文字が以前に存在した場合       */
						l_lfnflg |= 0x80;   /* LFNとする                      */
					}
					else
					{
						l_lfnflg |= 0x01;   /* 英大文字ありとする             */
					}
				}
				
				if( ( l_lfnflg & 0x80 ) == 0 )  /* まだショートファイル名の   */
				{                               /* 場合                       */
					if( l_extcnt == 0 ) /* 拡張子カウント中でない             */
					{
						l_namecnt ++;   /* ファイル名カウンタ更新             */
					}
					else                /* 拡張子カウント中                   */
					{
						l_extcnt ++;    /* 拡張子カウンタ更新                 */
					}
				}
			}
		}
		else if( *( pathname ) == '.' ) /* パス名の内容が'.'                  */
		{
			if( ( l_lfnflg & 0x80 ) == 0 )  /* ショートファイル名カウント中   */
			{
				if( l_extcnt == 0 )     /* 拡張子カウンタが0                  */
				{
					l_extcnt ++;        /* 拡張子カウンタ更新                 */
				}
				else
				{
					l_lfnflg |= 0x80;   /* 2個目の'.'なのでLFNと判断          */
				}
			}
		}
		else if( ( *( pathname ) == '*' )
			  || ( *( pathname ) == '?' ) ) /* パス名の内容が'*'又は'?'       */
		{
			if( ( *( prsflg ) & DFMR_SRCHFLGWC ) != DFMR_SRCHFLGWC )
			{							/* ワイルドカードチェック不要の場合 */
				return DFMR_NMERR;      /* 有効文字でない場合はエラー         */
			}
			l_lfnflg |= 0x40;           /* ワイルドカードあり                 */
		}
		else
		{
			return DFMR_NMERR;          /* 有効文字でない場合はエラー         */
		}
		if( ( l_lfnflg & 0x80 ) == 0 )  /* ショートファイル名確認時           */
		{
			if( l_namecnt > DFMR_FNAMESZ )  /* ファイル名カウンタがショート   */
			{                               /* ファイル名最大サイズ超過       */
				l_lfnflg |= 0x80;           /* LFNとする                      */
			}
			else
			{
				if( l_extcnt > ( DFMR_EXTSZ + 1 ) ) /* 拡張子カウンタが       */
					                                /* 拡張子数最大           */
				{                                   /* サイズを超過           */
					l_lfnflg |= 0x80;       /* LFNとする                      */
				}
			}
		}
		/* パス名を1文字進める */
		pathname ++;
	}
	return DFMR_NMERR;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
