/*======================================================================
 * File name    : copysect.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/copysect.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: copysect.c $
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
/* 関 数 名：FMR_copySector                                                   */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR   対象論理ドライブ番号                               */
/*         ：     F_BYTE   コピーモード                                       */
/*         ：              0=クラスタ先頭セクタよりコピー元セクタまでコピー   */
/*         ：              1=コピー元セクタ+1からクラスタ終端セクタまでコピー */
/*         ：              2=クラスタ先頭から書込み終了セクタまでコピー       */
/*         ：              3=クラスタ全てコピー                               */
/*         ：     F_WORD   書込みサイズ(コピーモード=1の時は、0指定の事)      */
/*         ：     F_INT    ファイルハンドル(コピーモード=1の時は、0指定の事)  */
/*         ：     F_DWORD  コピー元セクタ番号                                 */
/*         ：     F_DWORD  コピー先セクタ番号                                 */
/*         ：     F_BYTE * 書込み開始位置(コピーモード=1の時は、0指定の事)    */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：コピーモードに従ってクラスタ内のセクタをコピーする               */
/*         ：                                                                 */
/* 作 成 日：2005.02.09                                                       */
/* 作 成 者：Y.Yoshizumi                                                      */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.05 R/Wセクタ数をユーザー定義値とセクタ数/クラスタ  05/02/28 Y.Yoshizumi */
/*       を比較して決定する                                                   */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_copySector(
    F_CHAR   drvnum,                    /* 対象論理ドライブ番号               */
    F_BYTE   copymode,                  /* コピーモード                       */
    F_WORD   writesize,                 /* 書込みサイズ                       */
    F_INT    handle,                    /* ファイルハンドル                   */
    F_DWORD  srcsectnum,                /* コピー元セクタ番号                 */
    F_DWORD  dstsectnum,                /* コピー先セクタ番号                 */
    F_BYTE * writeptr                   /* 書込み開始位置                     */
)
{
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/Oパラメータ              */
	F_DWORD l_srcsectnum;               /* コピー元セクタ番号                 */
	F_DWORD l_dstsectnum;               /* コピー先セクタ番号                 */

	F_WORD  l_copysectcnt;              /* コピーセクタ数                     */
	F_WORD  l_drvsectcnt;               /* ドライバ用読込み／書込みセクタ数   */
	F_WORD  l_bufsectcnt;               /* R/Wバッファセクタ数                */
	F_WORD  l_work01;                   /*                                    */
	F_WORD  l_work02;                   /*                                    */
	F_INT   l_drtn;                     /* ドライバエントリ戻り値             */

	if( DFMR_RWBUFFSIZE > MFMR_SECTPERCLST( drvnum ) )
	{
		/* R/Wバッファサイズがクラスタ当りのセクタ数を超える場合 */
		l_bufsectcnt = MFMR_SECTPERCLST( drvnum );
	}
	else
	{
		/* R/Wバッファサイズがクラスタ当りのセクタ数を超えない場合 */
		l_bufsectcnt = DFMR_RWBUFFSIZE;
	}
	/* 任意のセクタAが属するクラスタの先頭セクタ番号の取得 */
	/* 1.セクタ番号AからData Region先頭セクタ番号を差引く */
	/* 2.1.で求めたセクタ番号をクラスタ当りのセクタ数で割る */
	/* 3.2.で求めた値にをクラスタ当りのセクタ数を掛ける */
	
	switch( copymode )
	{
	case DFMR_COPYSEC_FWDCPY:
	case DFMR_COPYSEC_CPYEND:
	case DFMR_COPYSEC_CPYALL:
		l_srcsectnum = srcsectnum - MFMR_DATATOP( drvnum );
		l_srcsectnum /= MFMR_SECTPERCLST( drvnum );
		l_srcsectnum *= MFMR_SECTPERCLST( drvnum );
		l_srcsectnum += MFMR_DATATOP( drvnum );

		l_dstsectnum = dstsectnum - MFMR_DATATOP( drvnum );
		l_dstsectnum /= MFMR_SECTPERCLST( drvnum );
		l_dstsectnum *= MFMR_SECTPERCLST( drvnum );
		l_dstsectnum += MFMR_DATATOP( drvnum );

		l_copysectcnt = srcsectnum - l_srcsectnum;
		if( writesize != 0 )
		{
			l_copysectcnt ++;
		}
		break;
	case DFMR_COPYSEC_BCKCPY:
		l_srcsectnum = srcsectnum + 1;
		l_dstsectnum = dstsectnum + 1;

		l_copysectcnt = srcsectnum - MFMR_DATATOP( drvnum );
		l_copysectcnt /= MFMR_SECTPERCLST( drvnum );
		l_copysectcnt *= MFMR_SECTPERCLST( drvnum );
		l_copysectcnt += MFMR_DATATOP( drvnum );
		l_copysectcnt += MFMR_SECTPERCLST( drvnum ) - 1;
		l_copysectcnt -= srcsectnum;
		break;
	default:
		return DFMR_FATALERR;
	}

	MFMR_RWSECTNUM( drvnum, 0 ) = 0xffffffff;
	l_drtn = DFMR_OK;
	l_work02 = MFMR_SECTPERCLST( drvnum );

	while( l_copysectcnt > l_bufsectcnt )
	{
		/*****************************************************************************/
		/* コピーモード＝０，２，３の場合                                            */
		/*   クラスタ先頭セクタからコピー元セクタ-1までR/Wバッファサイズ単位でコピー */
		/* コピーモード＝１の場合                                                    */
		/*   コピー元セクタ+1からクラスタ終端セクタまでR/Wバッファサイズ単位でコピー */
		/*****************************************************************************/
		/* コピー元を読込む */
		l_drvioparam.read.startsctnum = l_srcsectnum;   /* 読込み開始セクタ番号   */
		l_drvsectcnt = l_bufsectcnt;                 /* 読込みセクタ数         */
		l_drvioparam.read.readsctnum = &l_drvsectcnt;
		l_drvioparam.read.readbuff = MFMR_RWBUFFTOP( drvnum, 0 );   /* 読込みデータ格納領域   */
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
			DFMR_DRVFUNCREAD,           /* I/O Func.コード：データ読込み      */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		if( l_drtn != DFMR_OK )
		{
			MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
			return l_drtn;
		}
		if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_bufsectcnt) ) 
		{
			MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
			return DFMR_FATALERR;
		}

		/* コピー先へ書込む */
		l_drvioparam.write.startsctnum = l_dstsectnum;  /* 書込み開始セクタ番号   */
		l_drvsectcnt = l_bufsectcnt;                 /* 書込みセクタ数         */
		l_drvioparam.write.writesctnum = &l_drvsectcnt;
		l_drvioparam.write.writebuff = MFMR_RWBUFFTOP( drvnum, 0 ); /* 書込みデータ格納領域   */
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
			DFMR_DRVFUNCWRITE,          /* I/O Func.コード：データ書込み      */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_bufsectcnt) ) 
		{
			return DFMR_FATALERR;
		}
		l_srcsectnum += l_bufsectcnt;    /* コピー元セクタ更新             */
		l_dstsectnum += l_bufsectcnt;    /* コピー先セクタ更新             */
		l_work02 -= l_bufsectcnt;        /* クラスタ内残りセクタ更新       */
		l_copysectcnt -= l_bufsectcnt;   /* コピーセクタ数更新             */
	}

	if( l_copysectcnt != 0 )
	{
		/* コピー元セクタを読込む */
		if( copymode == DFMR_COPYSEC_CPYALL )
		{
			l_work01 = l_bufsectcnt;
		}
		else
		{
			l_work01 = l_copysectcnt;
		}
		l_drvioparam.read.startsctnum = l_srcsectnum;   /* 読込み開始セクタ番号   */
		l_drvsectcnt = l_work01;                   /* 読込みセクタ数         */
		l_drvioparam.read.readsctnum = &l_drvsectcnt;
		l_drvioparam.read.readbuff = MFMR_RWBUFFTOP( drvnum, 0 );   /* 読込みデータ格納領域   */
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
			DFMR_DRVFUNCREAD,           /* I/O Func.コード：データ読込み      */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		if( l_drtn != DFMR_OK )
		{
			MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
			return l_drtn;
		}
		if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_work01) ) 
		{
			MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
			return DFMR_FATALERR;
		}
		if( writesize != 0 )
		{
			/* R/Wバッファ上のデータに先頭部端数のデータを上書きする */
			l_work01 = MFMR_BYTEPERSECT( drvnum ) * (l_copysectcnt - 1);
			l_work01 += tfmr_filehandle[ handle ].crtsectofst;  /* クラスタ内オフセットを計算 */
			FMR_memCopy(
				&( MFMR_RWBUFF( drvnum, 0, l_work01 ) ),    /* R/Wバッファ    */
				writeptr,               /* 書込みデータ格納領域               */
				writesize               /* 先頭部端数のサイズ                 */
				);
		}
		/* コピー先セクタへ書込む */
		if( copymode == DFMR_COPYSEC_CPYEND )
		{
			l_work01 = MFMR_BYTEPERSECT( drvnum );
			l_work01 -= tfmr_filehandle[ handle ].crtsectofst;
			if( writesize > l_work01 )
			{
				l_work02 = writesize - l_work01;
				l_work02 --;
				l_work02 /= MFMR_BYTEPERSECT( drvnum );
				l_drvsectcnt += (l_work02 + 1);
			}
		}
		l_drvioparam.write.startsctnum = l_dstsectnum;  /* 書込み開始セクタ番号   */
		l_work01 = l_drvsectcnt;                        /* 書込みセクタ数         */
		l_drvioparam.write.writesctnum = &l_drvsectcnt;
		l_drvioparam.write.writebuff = MFMR_RWBUFFTOP( drvnum, 0 ); /* 書込みデータ格納領域   */
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
			DFMR_DRVFUNCWRITE,          /* I/O Func.コード：データ書込み      */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_work01) ) 
		{
			return DFMR_FATALERR;
		}
		if( copymode == DFMR_COPYSEC_CPYALL )
		{
			l_srcsectnum += l_bufsectcnt;
			l_dstsectnum += l_bufsectcnt;
			l_work02 -= l_bufsectcnt;
			while( l_work02 > 0 )
			{
				/********************************/
				/* クラスタ内残りセクタをコピー */
				/********************************/
				/* 残りコピー元を読込む */
				l_drvioparam.read.startsctnum = l_srcsectnum;   /* 読込み開始セクタ番号   */
				l_drvsectcnt = l_bufsectcnt;                 /* 読込みセクタ数         */
				l_drvioparam.read.readsctnum = &l_drvsectcnt;
				l_drvioparam.read.readbuff = MFMR_RWBUFFTOP( drvnum, 0 );   /* 読込みデータ格納領域   */
				l_drtn = MFMR_DRVENTRY( drvnum )(
					MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
					DFMR_DRVFUNCREAD,           /* I/O Func.コード：データ読込み      */
					&l_drvioparam               /* I/Oパラメータ                      */
					);
				if( l_drtn != DFMR_OK )
				{
					MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
					return l_drtn;
				}
				if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_bufsectcnt) ) 
				{
					MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
					return DFMR_FATALERR;
				}

				/* 残りコピー先へ書込む */
				l_drvioparam.write.startsctnum = l_dstsectnum;  /* 書込み開始セクタ番号   */
				l_drvsectcnt = l_bufsectcnt;                 /* 書込みセクタ数         */
				l_drvioparam.write.writesctnum = &l_drvsectcnt;
				l_drvioparam.write.writebuff = MFMR_RWBUFFTOP( drvnum, 0 ); /* 書込みデータ格納領域   */
				l_drtn = MFMR_DRVENTRY( drvnum )(
					MFMR_CUNUM( drvnum ),       /* チャネル/ユニット番号              */
					DFMR_DRVFUNCWRITE,          /* I/O Func.コード：データ書込み      */
					&l_drvioparam               /* I/Oパラメータ                      */
					);
				MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
				if( (l_drtn == DFMR_OK) && (l_drvsectcnt != l_bufsectcnt) ) 
				{
					return DFMR_FATALERR;
				}
				l_srcsectnum += l_bufsectcnt;
				l_dstsectnum += l_bufsectcnt;
				l_work02 -= l_bufsectcnt;
			}
		}
	}
	return l_drtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
