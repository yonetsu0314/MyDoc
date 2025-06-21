/*======================================================================
 * File name    : compare.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/compare.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: compare.c $
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
/* 関 数 名：FMR_compare                                                      */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR * 比較元文字列:ディレクトリエントリ.エントリ名       */
/*         ：     F_CHAR * 比較先文字列:APIよりのディレクトリ・ファイル名     */
/*         ：     F_WORD 比較文字列数                                         */
/*         ：     F_BYTE ワイルドカードフラグ bit6:ON ワイルドカード指定      */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*                 異常終了 DFMR_ERR                                          */
/*         ：                                                                 */
/* 概    要：文字列の比較を行う。                                             */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

#define MAX( a, b ) ( ( a ) > ( b ) ? a : b )   /* ２値の大きいほうを選別     */

static F_INT FMR_bmSearch(
    F_CHAR * src,                       /* 比較元文字列                       */
    F_BYTE srclen,                      /* 比較元文字列長                     */
    F_CHAR * dest,                      /* 比較先文字列                       */
    F_BYTE destlen                      /* 比較先文字列長                     */
);

F_INT FMR_compare(
    F_CHAR * srcstr,                    /* 比較元文字列                       */
    F_CHAR * dststr,                    /* 比較先文字列                       */
    F_BYTE srclen,                      /* 比較元文字列数                     */
    F_BYTE dstlen,                      /* 比較先文字列数                     */
    F_BYTE wcflg                        /* ワイルドカードフラグ               */
)
{
	F_CHAR * l_dstpart;                 /* 比較先文字列の部品の先頭アドレス   */
	F_CHAR * l_pwork;					/* ワークエリア(文字列ポインタ用)	  */
	F_INT l_srcofst;                    /* 比較先元字列の部品照合時オフセット */
	F_BYTE l_dstpartlen;                /* 比較先文字列の部品長               */
	F_BYTE l_srcrestlen;                /* 比較元文字残り                     */
	F_BYTE l_wccharcnt;                 /* ワイルドカード'?'の個数カウンタ    */
	F_BYTE l_work;                      /* ワークエリア(文字数カウント用)     */
	F_CHAR l_srcchar;                   /* 比較元文字                         */
	F_CHAR l_destchar;                  /* 比較先文字                         */

	l_srcrestlen = srclen;
	l_pwork = dststr;					/* 比較先文字列→ワークエリア         */
	l_work = 0;							/* ワークエリアを初期化               */

	for( l_work = 0; l_work < srclen; l_work ++ )
	{
		/* 比較先文字列にワイルドカード発見 */
		if( *( dststr ) == '*' )
		{
			if( wcflg == 0x00 )         /* ワイルドカード無効の場合は異常終了 */
			{
				return DFMR_ERR;
			}
			else
			{
				break;
			}
		}
		l_destchar = *( dststr );
		l_srcchar = *( srcstr );
		if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
		{
			l_destchar &= 0xDF;         /* 小文字を大文字へ変換               */
		}
		if( ( 'a' <= l_srcchar ) && ( l_srcchar <= 'z' ) )
		{
			l_srcchar &= 0xDF;          /* 小文字を大文字へ変換               */
		}
		if( l_destchar != l_srcchar )   /* 異なる文字発見                     */
		{
			if( l_destchar == '?' )     /* 1文字ワイルドカードは無条件で一致  */
			{
				if( wcflg == 0x00 )     /* ワイルドカード無効の場合は異常終了 */
				{
					return DFMR_ERR;
				}
			}
			else
			{
				return DFMR_ERR;
			}
		}
		dststr ++;                      /* 次の文字                           */
		srcstr ++;                      /*                                    */
		l_srcrestlen --;
	}
	
	if( l_work == srclen )              /* 線形走査のみで一致した場合         */
	{
		if( ( *(dststr) == 0x00 ) || ( *(dststr) == '\\' ) )
		{                               /* 検索用のポインタがNULLor'\'なら    */
			return DFMR_OK;             /* 正常終了                           */
		}
		else if( *(dststr) != '*')      /* 検索用のポインタが'*'なら          */
		{								/* 処理続行                           */
		return DFMR_ERR;
		}
	}
	/**************************************************************************/
	/* これより以降はワイルドカード'*'ありの場合の処理                        */
	/**************************************************************************/
	while( 1 )
	{
		l_wccharcnt = 0;
		/* 比較先文字列のワイルドカード"*"を読み飛ばす */
		while( *( dststr ) == '*' )
		{
			dststr ++;
		}
		/* '*'の直後がNULLの場合は無条件正常終了 */
		if( *( dststr ) == 0x00 )
		{
			return DFMR_OK;
		}
		/* '*'の直後に'?'の場合 */
		else if( *( dststr ) == '?' )
		{
			l_wccharcnt = 1;
			dststr ++;
			while( ( *( dststr ) == '*' )
				|| ( *( dststr ) == '?' ) )
			{
				if( *( dststr ) == '?' )
				{
					l_wccharcnt ++;
				}
				dststr ++;
			}
			if( l_wccharcnt > l_srcrestlen)
			{                        /* '?'が残り文字数より多い場合、異常終了 */
				return DFMR_ERR;
			}
			srcstr += l_wccharcnt;      /* '?'の数分検索対象文字列を進める    */
			l_srcrestlen -= l_wccharcnt;/* '?'の数分検索対象文字列の残り数を減算 */
			/* 直後がNULLの場合 */
			if( *( dststr ) == 0x00 )
			{
			return DFMR_OK;
			}
		}
		/* 部品の先頭アドレスを取得 */
		l_dstpart = dststr;
		l_dstpartlen = 0;
		/* 比較先文字列の部品を取得 */
		while( ( *( dststr ) != '*' )
			&& ( *( dststr ) != '?' )
			&& ( *( dststr ) != 0x00 ) )
		{
			l_dstpartlen ++;
			dststr ++;
		}
		
		/* 比較元文字列より比較先文字列が多い場合はエラー */
		if( l_srcrestlen < l_dstpartlen  )
		{
			return DFMR_ERR;
		}
		/* 指定文字パターン取得処理 */
		l_srcofst = FMR_bmSearch(
			srcstr,
			(F_BYTE)l_srcrestlen,
			l_dstpart,
			(F_BYTE)l_dstpartlen
			);
		if( l_srcofst == DFMR_ERR )     /* 対象文字列が存在しない場合はエラー */
		{
			return DFMR_ERR;
		}
		/* 比較元文字列を一致個所まで更新 */
		srcstr += l_srcofst;
		l_pwork = srcstr;
						/* オフセット後の検索対象文字列をワークエリアに避難   */
		srcstr += l_dstpartlen;
		l_srcrestlen -= ( F_WORD )l_srcofst;
		l_work  = l_srcrestlen;
						/* オフセット後の検索対象文字列残数をワークエリアに避難 */
		l_srcrestlen -= l_dstpartlen;

		/* 現在位置以降の'?'を含む文字列が一致しているかを確認 */
		l_destchar = *( dststr );
		l_srcchar = *( srcstr );
		if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
		{
			l_destchar &= 0xDF;         /* 小文字を大文字へ変換               */
		}
		if( ( 'a' <= l_srcchar ) && ( l_srcchar <= 'z' ) )
		{
			l_srcchar &= 0xDF;          /* 小文字を大文字へ変換               */
		}
		while( l_destchar == l_srcchar || l_destchar == '?' )
		{
			/* 検索用文字列・検索対象文字列の現在位置がNULLの場合、正常終了 */
			if(l_destchar == 0x00 && l_srcchar == 0x00 )
			{
				return DFMR_OK;
			}
			/* 検索対象文字列だけがNULLの場合は、異常終了 */
			else if( l_srcchar == 0x00 )
			{
				return DFMR_ERR;
			}
			dststr++;
			srcstr++;
			l_srcrestlen--;
			l_destchar = *( dststr );
			l_srcchar = *( srcstr );
			if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
			{
				l_destchar &= 0xDF;         /* 小文字を大文字へ変換           */
			}
			if( ( 'a' <= l_srcchar ) && ( l_srcchar <= 'z' ) )
			{
				l_srcchar &= 0xDF;          /* 小文字を大文字へ変換           */
			}
		}
		/* 現在の検索用の文字位置が'*'の場合、このままの位置から検索*/
		/*それ以外は検索用文字位置を戻し、検索対象文字列をオフセット後の位置から一つ進める*/
		if(l_destchar != '*')
		{
			dststr = l_dstpart;
			l_srcrestlen = --l_work;
			srcstr = ++l_pwork;
		}
	}
}

static F_INT FMR_bmSearch(
    F_CHAR * src,                       /* 比較元文字列                       */
    F_BYTE srclen,                      /* 比較元文字列長                     */
    F_CHAR * dest,                      /* 比較先文字列                       */
    F_BYTE destlen                      /* 比較先文字列長                     */
)
{
	
	F_BYTE l_skip[ 256 ];    /* 文字列不一致時、ずらす値を表す    */
	
	F_WORD l_srccnt;                    /* 比較元文字列の比較位置ポインタ     */
	F_WORD l_destcnt;                   /* 比較先文字列の比較位置ポインタ     */
	F_CHAR l_srcchar;                   /* 比較元文字                         */
	F_CHAR l_destchar;                  /* 比較先文字                         */

	/* 表l_skipを作成 */
	for( l_srccnt = 0; l_srccnt < 256; l_srccnt ++ )
	{
		l_skip[ l_srccnt ] = destlen;
	}
	
	for( l_srccnt = 0; l_srccnt < destlen - 1; l_srccnt ++ )
	{
		l_destchar = dest[ l_srccnt ];
		if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
		{
			l_destchar &= 0xDF;         /* 小文字を大文字へ変換               */
		}
		l_skip[ l_destchar ] = destlen - l_srccnt - 1;
	}
	
	l_srccnt = destlen - 1;             /* ポインタ初期化                     */
	                                    /* 比較先文字列の後ろから前に向かって */
	                                    /* 比較を行う                         */
	
	/* テキストの最後尾に行き当たるまで繰り返す。 */
	while( l_srccnt < srclen )          /* 比較元文字列の最後尾に行き当たる   */
	{                                   /* まで繰り返す                       */
		l_destcnt = destlen - 1;        /* 比較先文字列ポインタを文字列の最後 */
		                                /* の文字を差すようにする             */
		l_destchar = dest[ l_destcnt ];
		l_srcchar = src[ l_srccnt ];
		if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
		{
			l_destchar &= 0xDF;         /* 小文字を大文字へ変換               */
		}
		if( ( 'a' <= l_srcchar ) && ( l_srcchar <= 'z' ) )
		{
			l_srcchar &= 0xDF;          /* 小文字を大文字へ変換               */
		}
		while( l_srcchar == l_destchar )
		{                               /* 比較元/先文字列が一致する間繰り返す*/
			if( l_destcnt == 0 )        /* 最初の文字まで一致したら検索は成功 */
			{
				return l_srccnt;
			}
			
			l_srccnt --;                /* ポインタl_srccntを1文字分戻す      */
			l_destcnt --;               /* ポインタl_destcntを1文字分戻す     */
			l_destchar = dest[ l_destcnt ];
			l_srcchar = src[ l_srccnt ];
			if( ( 'a' <= l_destchar ) && ( l_destchar <= 'z' ) )
			{
				l_destchar &= 0xDF;     /* 小文字を大文字へ変換               */
			}
			if( ( 'a' <= l_srcchar ) && ( l_srcchar <= 'z' ) )
			{
				l_srcchar &= 0xDF;      /* 小文字を大文字へ変換               */
			}
		}
		
		l_srccnt = l_srccnt             /* 一致しなかったのでパターンをずらす */
			+ MAX( l_skip[ l_srcchar ], destlen - l_destcnt );
	}
	return DFMR_ERR;                    /* 結局見つからなかった               */
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
