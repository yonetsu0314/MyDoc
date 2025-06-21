/*======================================================================
 * File name    : fsread.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/rcs/fsread.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsread.c $
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
/* 関 数 名：FMR_Read                                                         */
/*         ：                                                                 */
/* パラメタ：IN:  int ファイルハンドル番号                                    */
/*         ：     unsigned int 読込みサイズ                                   */
/*         ：                                                                 */
/*         ：OUT: void * 読込みデータ格納領域                                 */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  読込みが完了したバイト数                               */
/*         ：異常終了  DFMR_ERRPARAM                                          */
/*         ：          DFMR_EBADF                                             */
/*         ：          DFMR_EACCES                                            */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ファイルからのデータ読込み                                       */
/*         ：                                                                 */
/* 作 成 日：2004.05.27                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

int FMR_Read ( 
	int filehandle,                     /* ファイルハンドル番号               */
	void * readbuff,                    /* 読込みデータ格納領域               */
	unsigned int size                   /* 読込みサイズ                       */
)
{
	F_DWORD l_restsize;                 /* 残りサイズ                         */
	F_DWORD l_readsize;                 /* 読込み済サイズ                     */
	F_DWORD l_startsectnum;             /* 読込み開始セクタ番号               */
	F_DWORD l_readsectcnt;              /* 読込みセクタ数                     */
	F_DWORD l_restsectcnt;              /* 残りセクタ数                       */
	
	F_DWORD l_nextclstnum;              /* 続きクラスタ番号                   */
	F_DWORD l_nowclstnum;               /* 現在のクラスタ番号                 */
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_modtbl;                   /* 剰余算計算用                       */
	F_DWORD l_eoc;                      /* EOCコード                          */
	
	F_BYTE * l_buffptr;                 /* 読込みバッファポインタ             */
	TFMR_DRVIOPARAM l_drvioparam;       /* デバイスドライバI/F                */

	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	
	F_WORD l_offset;                    /* 読込み開始セクタ内オフセット       */
	F_WORD l_targetsectcnt;             /* 対象セクタ数                       */

	F_BYTE l_startsectoffset;           /* クラスタ内開始セクタ番号           */
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	
	
	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	/* ファイルハンドル確認 */
	if( ( filehandle < 0 ) || ( DFMR_USR_MAXFILEHANDLE <= filehandle ) )
	{
		/* ファイルハンドル番号が許容範囲外の場合 */
		return DFMR_ERRPARAM;
	}
	
	if( tfmr_filehandle[ filehandle ].direntry.name[ 0 ] == DFMR_INITBYTE )
	{
		/* ファイルハンドルにファイルが関連付けされていない場合 */
		return DFMR_EBADF;
	}
	/* 読込みサイズ確認 */
	if( size <= 0 )
	{
		/* 読込みサイズが0以下の場合 */
		return DFMR_ERRPARAM;
	}
	/* 読込みデータ格納領域確認 */
	if( readbuff == NULL )
	{
		/* 読込みデータ格納領域がNULLの場合 */
		return DFMR_ERRPARAM;
	}
	/* ファイルポインタの確認 */
	if( 0 == ( tfmr_filehandle[ filehandle ].direntry.filesize
		 - tfmr_filehandle[ filehandle ].fptr ) )
	{
		/* ファイルポインタがファイル終端の場合 */
		return DFMR_EACCES;
	}
	/* アクセスモードの確認 */
	if( ( tfmr_filehandle[ filehandle ].acsmode & DFMR_O_WRONLY ) == DFMR_O_WRONLY )
	{
		/* アクセスモードがR/W,Write onlyの場合 */
		return DFMR_EACCES;
	}
	
	/**************************************************************************/
	/* 初期設定                                                               */
	/**************************************************************************/
	/* 読込みサイズ取得 */
	l_work01 = tfmr_filehandle[ filehandle ].direntry.filesize;
	l_work01 -= tfmr_filehandle[ filehandle ].fptr;
	
	if( l_work01 <= size )
	{
		/* 読込みサイズが既存のファイルサイズ以上の場合 */
		l_restsize = l_work01;          /* 既存サイズを残りサイズとする       */
	}
	else
	{
		/* 読込みサイズが既存のファイルサイズ未満の場合 */
		l_restsize = size;              /* 読込みサイズを残りサイズとする     */
	}
	
	l_readsize = 0;                     /* 書込み済みサイズ←0                */
	l_startsectnum = tfmr_filehandle[ filehandle ].crtsectnum;
	                                    /* 読込み開始セクタ番号               */
	                                    /* ←カレントセクタ番号               */
	l_offset = tfmr_filehandle[ filehandle ].crtsectofst;
	                                    /* 読込み開始セクタ内オフセット←     */
	                                    /* カレントセクタ内ポインタオフセット */
	l_buffptr = ( F_BYTE * )readbuff;   /* 読込みデータ格納領域にポインタを   */
	                                    /* キャスト                           */
	l_drvnum = tfmr_filehandle[ filehandle ].drivenum;
	                                    /* 論理ドライブ番号取得               */
	if( MFMR_DRVINFO( l_drvnum ) == ( F_VOID * )DFMR_RAMINIT )
	{
		/* 論理ドライブが未接続の場合 */
		return DFMR_DRVNOCONNECTERR;    /* 論理ドライブ未接続エラー           */
	}
	l_modtbl = ( MFMR_SECTPERCLST( l_drvnum ) - 1 );
	l_nowclstnum = l_startsectnum - MFMR_DATATOP( l_drvnum );
	l_nowclstnum /= MFMR_SECTPERCLST( l_drvnum );
	l_nowclstnum += 2;                  /* 現在のクラスタ番号を               */
	                                    /* カレントセクタ番号より求める       */
	l_startsectoffset = ( F_BYTE )( l_startsectnum - MFMR_DATATOP( l_drvnum ) );
	l_startsectoffset &= l_modtbl;
	                                    /* クラスタ内開始セクタ番号を求める   */
	MFMR_GETEOCMIN( MFMR_FATTYPE( l_drvnum ), l_eoc );  /* EOC設定            */
	
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn = FMR_osWaiSem(
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	
	/**************************************************************************/
	/* 初期設定続き                                                           */
	/* (デバイスにアクセスするのでセマフォ資源獲得後でなければならない)       */
	/**************************************************************************/
	/* FATセクタ読込み領域に現在のクラスタ番号が存在するセクタを読込み */
	/* 次のクラスタ番号(FATエントリ)を取得する                         */
	l_rtn = FMR_getFatSector(
		MFMR_DRVINFO( l_drvnum ),       /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),     /* FAT読込み領域     */
		l_nowclstnum,                   /* クラスタ番号                       */
		&l_nextclstnum                  /* FATエントリ値                      */
		);

	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	
	/**************************************************************************/
	/* 先頭部端数セクタの読込み                                               */
	/**************************************************************************/
	if( l_offset != 0 )
	{
		/**********************************************************************/
		/* 先頭部端数が存在した場合                                           */
		/**********************************************************************/
		/* 読込み開始セクタを読込む */
		l_rtn = FMR_readDataSector(
			l_drvnum,
			l_startsectnum,
			0
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
		/* 読込んだセクタより読込みデータをバッファにコピーする */
		if( ( l_offset + l_restsize ) <= MFMR_BYTEPERSECT( l_drvnum ) )
		{
			/************************************************/
			/* 読込みサイズが読込んだセクタ内で完結する場合 */
			/************************************************/
			/* バッファにデータをコピーする */
			FMR_memCopy(
				l_buffptr,              /* 読込みバッファ                     */
				&( MFMR_RWBUFF( l_drvnum, 0, l_offset ) ),
				                        /* R/Wバッファの読込み先頭位置        */
				l_restsize              /* 残りサイズ                         */
				);
			/* パラメータ更新 */
			l_readsize += l_restsize;   /* 読込み済みサイズ更新               */
			l_buffptr += l_restsize;    /* バッファポインタをコピーした       */
			                            /* サイズ分加算                       */
			l_restsize -= l_restsize;   /* 残りサイズから読込んだ             */
			                            /* サイズ分差引く                     */
			
		}
		else
		{
			/**************************************************/
			/* 読込みサイズが読込んだセクタ以降も存在する場合 */
			/**************************************************/
			/* バッファにデータをコピーする */
			l_work01 = MFMR_BYTEPERSECT( l_drvnum ) - l_offset;
			                            /* セクタ内での読込みサイズを取得     */
			FMR_memCopy(
				l_buffptr,              /* 読込みバッファ                     */
				&( MFMR_RWBUFF( l_drvnum, 0, l_offset ) ),
				                        /* R/Wバッファの読込み先頭位置        */
				l_work01                /* 読込みサイズ                       */
				);
			/* パラメータ更新 */
			l_readsize += l_work01;     /* 読込み済みサイズ更新               */
			l_buffptr += l_work01;      /* バッファポインタをコピーした       */
			                            /* サイズ分加算                       */
			l_restsize -= l_work01;     /* 残りサイズから読込んだ             */
			                            /* サイズ分差引く                     */
			/* 次に読込むセクタを求める */
			l_work01 = l_startsectnum - MFMR_DATATOP( l_drvnum );
			l_work01 &= l_modtbl;
			if( ( F_WORD )l_work01 != ( F_WORD )( MFMR_SECTPERCLST( l_drvnum ) - 1 ) )
			{
				/**************************************************************/
				/* 読込み開始セクタがクラスタ終端以外のセクタの場合           */
				/**************************************************************/
				l_startsectnum ++;      /* 読込み開始セクタを1セクタ分進める  */
				l_startsectoffset ++;   /* 読込みクラスタ内セクタ番号も       */
				                        /* 1セクタ分進める                    */
			}
			else
			{
				/**************************************************************/
				/* 読込み開始セクタがクラスタ終端のセクタの場合               */
				/**************************************************************/
				/* 続きのクラスタの先頭セクタを読込み開始クラスタとする */
				l_startsectnum = FMR_getSectNum(
					l_nextclstnum,      /* クラスタ番号                       */
					0,                  /* クラスタ内番号は先頭               */
					l_drvnum            /* 論理ドライブ番号                   */
					);
				
				/* 続きのクラスタを現在のクラスタとする */
				l_nowclstnum = l_nextclstnum;
				
				/* FATセクタ読込み領域に現在のクラスタ番号が存在するセクタを  */
				/* 読込み次のクラスタ番号(FATエントリ)を取得する              */
				l_rtn = FMR_getFatSector(
					MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報       */
					MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域              */
					l_nowclstnum,               /* クラスタ番号               */
					&l_nextclstnum              /* FATエントリ値              */
					);
				if( l_rtn != DFMR_OK )
				{
					goto ERROR;
				}
				
				/* 読込み開始セクタ内オフセットをセクタ先頭とする */
				l_offset = 0;
				
				/* 読込みクラスタ内セクタ番号もセクタ先頭とする */
				l_startsectoffset = 0;
			}
		}
	}
	/**************************************************************************/
	/* セクタ単位の連続読込み                                                 */
	/**************************************************************************/
	if( l_restsize > MFMR_BYTEPERSECT( l_drvnum ) )
	{
		/**********************************************************************/
		/* 残りサイズが1セクタ分以上の場合は連続読込みを行う                  */
		/**********************************************************************/
		/* パラメータ設定 */
		l_restsectcnt = l_restsize / MFMR_BYTEPERSECT( l_drvnum );
		                                /* 残りセクタ数を設定                 */
		l_readsectcnt = 0;              /* 読込みセクタ数初期化               */
		/**********************************************************************/
		/* 連続読込み処理を行う                                               */
		/**********************************************************************/
		while( l_restsectcnt > 0 )
		{
			/******************************************************************/
			/* 残りセクタ数が0となるまで連続読込みを行う                      */
			/******************************************************************/
			/* 現在のクラスタに対する判定 */
			l_work01 = MFMR_SECTPERCLST( l_drvnum ) - l_startsectoffset;
			if( l_restsectcnt <= l_work01 )
			{
				/* 残りセクタ数が対象クラスタのセクタ数以下の場合 */
				l_readsectcnt += l_restsectcnt;
				                        /* 読込みセクタ数←残りセクタ数を加算 */
				l_restsectcnt = 0;      /* 残りセクタ数←0                    */
			}
			else
			{
				l_readsectcnt += l_work01;  /* 読込みセクタ数←対象クラスタ   */
				                            /* 内のセクタ数を加算             */
				l_restsectcnt -= l_work01;  /* 残りセクタ数←対象クラスタ     */
				                            /* 内のセクタ数を減算             */
				l_startsectoffset = 0;
			}
			
			/* 続きクラスタに対する判定 */
			if( ( l_nextclstnum != ( l_nowclstnum + 1 ) )
				|| ( l_restsectcnt == 0 ) )
			{
				/**************************************************************/
				/* 続きのクラスタが現在のクラスタの連番ではない場合           */
				/**************************************************************/
				/* 読込み処理を行う */
				l_drvioparam.read.startsctnum = l_startsectnum;
				l_targetsectcnt = (F_WORD)l_readsectcnt;
				l_drvioparam.read.readsctnum = &l_targetsectcnt;
				l_drvioparam.read.readbuff = l_buffptr;
				
				l_rtn = MFMR_DRVENTRY( l_drvnum )(
					MFMR_CUNUM( l_drvnum ),     /* チャネル番号/ユニット番号  */
					DFMR_DRVFUNCREAD,           /* I/O Func.番号:データ読込み */
					&l_drvioparam
					);
				if( (l_rtn != DFMR_OK) ||
					(l_targetsectcnt != (F_WORD)l_readsectcnt) )
				{                               /* 処理失敗又はセクタ数異常時 */
					if( l_rtn == DFMR_OK )
					{
						l_rtn = DFMR_FATALERR;
					}
					goto ERROR;
				}
				
				/* パラメータの更新を行う */
				l_restsize -= l_readsectcnt * MFMR_BYTEPERSECT( l_drvnum );
				                        /* 残りサイズから読込んだサイズ分を   */
				                        /* 減算                               */
				l_readsize += l_readsectcnt * MFMR_BYTEPERSECT( l_drvnum );
				                        /* 読込み済サイズに読込んだサイズ分を */
				                        /* 加算                               */
				l_buffptr += l_readsectcnt * MFMR_BYTEPERSECT( l_drvnum );
				                        /* 読込みバッファポインタを読込んだ   */
				                        /* サイズ分進める                     */
				/* 読込み開始セクタ番号を取得 */
				l_startsectnum += l_readsectcnt;
				                        /* 次の読込み開始セクタ番号は読込んだ */
				                        /* セクタ数を加算した先               */
				l_work01 = l_startsectnum - MFMR_DATATOP( l_drvnum );
				l_work01 &= l_modtbl;
				if(  l_work01 == 0 )
				{
					/* クラスタ先頭のセクタの場合は連番とは限らないので */
					/* 続きのクラスタ番号の先頭セクタを取得する         */
					l_startsectnum = l_nextclstnum - DFMR_TOPCLSTNUM;
					l_startsectnum *= MFMR_SECTPERCLST( l_drvnum );
					l_startsectnum += MFMR_DATATOP( l_drvnum );
				}
				l_readsectcnt = 0;      /* 読込みセクタ数←0                  */
			}
			if ( l_nextclstnum >= l_eoc ) {
				break;
			}
			l_nowclstnum = l_nextclstnum;   /* 現在のクラスタ番号             */
			                                /* ←続きのクラスタ番号           */
			/* 続きのクラスタ番号を取得する */
			l_rtn = FMR_controlFatEntry(
				MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報           */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
				                            /* FAT読込み領域                  */
				l_nowclstnum,               /* クラスタ番号                   */
				&l_nextclstnum,             /* FATエントリ値                  */
				DFMR_FLGOFF                 /* Read/Write種別 OFF:取得        */
				);
			if( l_rtn != DFMR_OK )
			{
				if( l_rtn == DFMR_CLSTNUMERR )  /* クラスタ番号異常の場合は異常終了   */
				{
					goto ERROR;
				}
				/**************************************************************/
				/* 失敗時は現在のクラスタが他のFATセクタに属していると判断    */
				/**************************************************************/
				/* FAT読込み領域0に現在のクラスタが属するFATセクタを読込み */
				/* 続きのクラスタ番号(FATエントリ)を取得する               */
				l_rtn = FMR_getFatSector(
					MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報       */
					MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域              */
					l_nowclstnum,               /* クラスタ番号               */
					&l_nextclstnum              /* FATエントリ値              */
					);
				if( l_rtn != DFMR_OK )
				{
					goto ERROR;
				}
			}
		}
	}
	/**************************************************************************/
	/* 終端部端数セクタ読込み                                                 */
	/**************************************************************************/
	if( l_restsize > 0 )
	{
		/**********************************************************************/
		/* 残りサイズが存在する場合は終端部セクタ読込みを行う                 */
		/**********************************************************************/
		/* 読込み開始セクタを読込む */
		l_rtn = FMR_readDataSector(
			l_drvnum,
			l_startsectnum,
			0
			);
		if( l_rtn != DFMR_OK )
		{
			goto ERROR;
		}
		/* 読込んだセクタの先頭から残りサイズ分を読込みバッファポインタが示す */
		/* 領域にコピーを行う                                                 */
		FMR_memCopy(
			l_buffptr,                  /* 読込みバッファポインタ             */
			MFMR_RWBUFFTOP( l_drvnum, 0 ),  /* 読込んだセクタの先頭           */
			l_restsize
			);
		/* パラメータ更新 */
		l_readsize += l_restsize;       /* 読込み済みサイズに残りサイズを加算 */
		
	}
	/**************************************************************************/
	/* ファイルポインタ移動                                                   */
	/**************************************************************************/
	/* ファイルポインタを移動させる */
	l_rtn = FMR_setPointer(
		filehandle,                     /* ファイルハンドル番号               */
		l_readsize,                     /* オフセット=読込み済みサイズ        */
		DFMR_SEEK_CUR                   /* 初期位置=現在値                    */
		);
	if( l_rtn != DFMR_OK )
	{
		goto ERROR;
	}
	
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	/**************************************************************************/
	/* 書込み済みサイズを返却                                                 */
	/**************************************************************************/
	return ( F_INT )l_readsize;

ERROR:
	/* 異常終了 */
	l_ortn = FMR_osSigSem(              /* セマフォ資源返却                   */
		MFMR_SEMID( l_drvnum )
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	return l_rtn;

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
