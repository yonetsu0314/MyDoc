/*======================================================================
 * File name    : subwrite.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/subwrite.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: subwrite.c $
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
/* 関 数 名：FMR_subWrite                                                     */
/*         ：                                                                 */
/* パラメタ：IN:  F_INT    ファイルハンドル                                   */
/*         ：     F_VOID * 書込みデータ格納領域                               */
/*         ：     F_UINT   書き込みサイズ                                     */
/*         ：                                                                 */
/*         ：OUT:                                                             */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  書込みサイズ                                           */
/*         ：異常終了  DFMR_EBADF                                             */
/*         ：          DFMR_ERRPARAM                                          */
/*         ：          DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：ファイルへのデータ書込み                                         */
/*         ：                                                                 */
/* 作 成 日：2004.09.11                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       ファイル属性にアーカイブ属性設定                05/02/09 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

static F_INT FMR_SubControlFatEntry ( 
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
	F_DWORD   clstnum,                  /* クラスタ番号                       */
	F_DWORD * entryval                  /* FATエントリ値                      */
);

F_INT FMR_subWrite (
	F_INT    filehandle,                /* ファイルハンドル                   */
	F_VOID * writebuff,                 /* 書込みデータ格納領域               */
	F_UINT   size                       /* 書込みサイズ                       */
)
{
	F_DWORD l_restsize;                 /* 書込み済みサイズ                   */
	F_DWORD l_newtopclstnum;            /* 新規取得クラスタ番号               */
	F_DWORD l_startsectnum;             /* 書込み開始セクタ番号               */
	F_DWORD l_updclst_snum;             /* 更新開始クラスタ番号               */
	F_DWORD l_updclst_enum;             /* 更新終了クラスタ番号               */

	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_work04;
	
	F_DWORD l_evacfptr;                 /* ファイルポインタ退避用             */
	F_DWORD l_evaccrtsectnum;           /* カレントセクタ番号退避用           */
	F_DWORD l_savefptr;                 /* ファイルポインタ退避用             */
	F_DWORD l_savecrtsectnum;           /* カレントセクタ番号退避用           */

	F_DWORD l_modbps;                   /* 剰余算計算用(Byte/Sector)          */
	F_LONG  l_seekofst;                 /* seek用オフセット                   */
	
	TFMR_DIRENTRY * l_entryptr;         /* エントリアクセス用ポインタ         */
	
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_ortn;                       /* OS部戻り値                         */

	F_WORD l_evaccrtsectofst;           /* ポインタオフセット退避用           */
	F_WORD l_savecrtsectofst;           /* ポインタオフセット退避用           */
	
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_mode;                      /* 書込みモード 0=新規,1=追加,2=更新  */

	/**************************************************************************/
	/* 初期設定                                                               */
	/**************************************************************************/
	l_restsize = 0;                     /* 書込み済みサイズをする             */
	l_drvnum = tfmr_filehandle[ filehandle ].drivenum;  /* ドライブ番号取得   */
	l_modbps = MFMR_BYTEPERSECT( l_drvnum ) - 1;
	/**************************************************************************/
	/* アクセスモード確認                                                     */
	/**************************************************************************/
	if( ( tfmr_filehandle[ filehandle ].acsmode & DFMR_O_APPEND ) == DFMR_O_APPEND )
	{
		/**********************************************************************/
		/* アクセスモードがAppend指定の場合                                   */
		/**********************************************************************/
		/* ファイルポインタを終端に移動 */
		l_rtn = FMR_setPointer(
			filehandle,
			0,                          /* ｢ファイル終端から0Byte移動｣で      */
			DFMR_SEEK_END               /* ファイルポインタを終端に移動させる */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	/**************************************************************************/
	/* 書込みパターン(新規/追加/更新)の振分処理                               */
	/**************************************************************************/
	if( ( tfmr_filehandle[ filehandle ].direntry.fstclushi == 0 )
		&& ( tfmr_filehandle[ filehandle ].direntry.fstcluslo == 0 ) )
	{
		/**********************************************************************/
		/* 対象ファイルの先頭クラスタが0の場合(新規書込み)                    */
		/**********************************************************************/
		/* FATセクタ管理領域より空きクラスタを取得する */
		l_rtn = FMR_searchEmptyCluster(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			&l_newtopclstnum            /* 取得空きクラスタ番号               */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
		/* 取得した空きクラスタを含むFATセクタをFAT読込み領域0に読込む */
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_newtopclstnum,            /* クラスタ番号                       */
			&l_work01                   /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}

		l_work01 = FMR_getSectNum(      /* 取得した空きクラスタの先頭セクタ番号を求める */
			l_newtopclstnum,            /* クラスタ番号                       */
			0,                          /* クラスタ内セクタ番号               */
			l_drvnum                    /* ドライブ番号                       */
			);
		/* 追加書込み処理を行う */
		l_rtn = FMR_appendData(
			filehandle,                 /* ファイルハンドル番号               */
			l_work01,                   /* 書込み開始セクタ番号               */
			( F_BYTE * )writebuff,      /* 書込みデータ格納領域               */
			( F_DWORD )size,            /* 書込みサイズ                       */
			( l_newtopclstnum + 1 ),    /* 検索開始クラスタ番号               */
			&l_work01                   /* 新規取得先頭クラスタ番号           */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
		/* 書込み済みサイズ←書込みサイズを加算 */
		l_restsize += size;
		/* ファイルハンドルの更新を行う */
		l_work01 = l_newtopclstnum & 0xFFFF0000;/* 開始クラスタ番号上位を取得 */
		l_work01 >>= 16;
		tfmr_filehandle[ filehandle ].direntry.fstclushi = ( F_WORD )l_work01;
		l_work01 = l_newtopclstnum & 0x0000FFFF;/* 同下位を取得               */
		tfmr_filehandle[ filehandle ].direntry.fstcluslo = ( F_WORD )l_work01;

		tfmr_filehandle[ filehandle ].direntry.filesize = l_restsize;
		                                /* サイズ←書込んだサイズ             */
		l_mode = 0;
	}
	else if( ( tfmr_filehandle[ filehandle ].direntry.filesize 
			== tfmr_filehandle[ filehandle ].fptr )
		&& ( ( tfmr_filehandle[ filehandle ].direntry.filesize
			& l_modbps ) == 0 ) )
	{
		/**********************************************************************/
		/* 更新すべきセクタがない場合(追加書込み)                             */
		/**********************************************************************/
		/* カレントセクタが属するクラスタを含む */
		/* FATセクタをFAT読込み領域0に読込む    */
		l_work02 = tfmr_filehandle[ filehandle ].crtsectnum;
		l_work02 -= MFMR_DATATOP( l_drvnum );
		l_work02 /= MFMR_SECTPERCLST( l_drvnum );   /* カレントセクタから     */
		l_work02 += 2;                              /* カレントクラスタを取得 */
		
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			l_work02,                   /* クラスタ番号                       */
			&l_work01                   /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
		l_work01 = tfmr_filehandle[ filehandle ].fptr;
		l_work03 = (MFMR_BYTEPERSECT( l_drvnum ) * MFMR_SECTPERCLST( l_drvnum )) - 1;
		if ( (l_work01 & l_work03) == 0 )
		{
			/******************************************************************/
			/* ファイルポインタがクラスタ終端に位置する場合                   */
			/******************************************************************/
			l_work02 = FMR_getEmptyCluster(    /* セクタ読込み領域0から空きクラスタを取得する */
				l_drvnum,               /* 対象論理ドライブ番号               */
				l_work02,               /* 検索開始クラスタ番号               */
				DFMR_REFFATAREA0        /* 検索FAT読込み領域番号              */
				);
			if( l_work02 == DFMR_LNG )
			{
				/**********************************************************/
				/* FAT読込み領域内で空きクラスタが存在しなかった場合      */
				/**********************************************************/
				/* FATセクタ管理領域より空きクラスタを取得する */
				l_rtn = FMR_searchEmptyCluster(
					l_drvnum,               /* 対象論理ドライブ番号               */
					&l_work02               /* 取得空きクラスタ番号               */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				/* 取得した空きクラスタを含むFATセクタをFAT読込み領域0に読込む */
				l_rtn = FMR_getFatSector(
					MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
					MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域                      */
					l_work02,                   /* クラスタ番号                       */
					&l_work04                   /* FATエントリ値                      */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
			}
			l_startsectnum = FMR_getSectNum(/* 取得した空きクラスタの先頭セクタ番号を求める */
				l_work02,                   /* クラスタ番号                    */
				0,                          /* クラスタ内セクタ番号            */
				l_drvnum                    /* ドライブ番号                    */
				);
		}
		else
		{
			/******************************************************************/
			/* ファイルポインタがクラスタ終端以外に位置する場合               */
			/******************************************************************/
			l_startsectnum = tfmr_filehandle[ filehandle ].crtsectnum;
			                            /* 書込み開始セクタ番号は             */
			                            /* カレントセクタ                     */
		}
		
		l_work01 = tfmr_filehandle[ filehandle ].crtsectnum;
		l_work01 -= MFMR_DATATOP( l_drvnum );
		l_work01 /= MFMR_SECTPERCLST( l_drvnum );
		l_work01 += 2;                  /* カレントセクタが属するクラスタ番号 */
		/* 追加書込み処理を行う                                               */
		l_rtn = FMR_appendData(
			filehandle,                 /* ファイルハンドル番号               */
			l_startsectnum,             /* 書込み開始セクタ番号               */
			( F_BYTE * )writebuff,      /* 書込みデータ格納領域               */
			( F_DWORD )size,            /* 書込みサイズ                       */
			(l_work02 + 1),             /* 検索開始クラスタ番号               */
			                            /*  = カレントクラスタ番号            */
			&l_newtopclstnum            /* 新規取得先頭クラスタ番号           */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		if ( (tfmr_filehandle[ filehandle ].fptr & l_work03) == 0 )
		{
			/******************************************************/
			/* 新規クラスタを追加書込み処理の前に取得している場合 */
			/******************************************************/
			l_rtn = FMR_SubControlFatEntry(
				l_drvnum,               /* 論理ドライブ構成情報               */
				l_work01,               /* クラスタ番号                       */
				&l_work02               /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
		}
		/* 書込み済みサイズ←書込みサイズを加算 */
		l_restsize += size;
		tfmr_filehandle[ filehandle ].direntry.filesize += l_restsize;
		                                /* サイズ←書込んだサイズを加算       */
		l_mode = 1;
	}
	else
	{
		/**********************************************************************/
		/* 更新すべきセクタがある場合(更新書込み)                             */
		/**********************************************************************/
		/* 更新終了クラスタ番号を求める */
		l_work01 = tfmr_filehandle[ filehandle ].fptr + size;
		if( l_work01 >= tfmr_filehandle[ filehandle ].direntry.filesize )
		{
			/**********************************************/
			/* 書込み後サイズが書込み前サイズを超える場合 */
			/**********************************************/
			/* 更新終了クラスタ番号はEOC          */
			MFMR_GETEOC( MFMR_FATTYPE( l_drvnum ), l_updclst_enum );
		}
		else
		{
			/************************************************/
			/* 書込み後サイズが書込み前サイズを超えない場合 */
			/************************************************/
			/* 現在のファイルポインタを保存 */
			l_evacfptr = tfmr_filehandle[ filehandle ].fptr;
			l_evaccrtsectnum = tfmr_filehandle[ filehandle ].crtsectnum;
			l_evaccrtsectofst = tfmr_filehandle[ filehandle ].crtsectofst;
			
			l_work02 = (MFMR_BYTEPERSECT( l_drvnum ) * MFMR_SECTPERCLST( l_drvnum )) - 1;
			if ( (l_work01 & l_work02) == 0 )
			{
				/* クラスタ境界に書込み終了位置が位置する場合、１つ前にする。 */
				/* (FMR_setPointerでは、クラスタ境界の場合、次のセクタに位置させる為  */
				l_work01 = size;
				l_work01--;
			}
			else
			{
				l_work01 = size;
			}

			/* ファイルポインタを書込み終了位置へ移動 */
			l_rtn = FMR_setPointer(
				filehandle,                 /* ファイルハンドル番号               */
				l_work01,                   /* オフセット                         */
				DFMR_SEEK_CUR               /* 初期位置                           */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* 書込み終了位置のクラスタ番号を取得 */
			l_work01 = tfmr_filehandle[ filehandle ].crtsectnum;
			l_work01 -= MFMR_DATATOP( l_drvnum );
			l_work01 /= MFMR_SECTPERCLST( l_drvnum );
			l_work01 += 2; 
			l_rtn = FMR_controlFatEntry(
				MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報アドレス       */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),
				                            /* FAT読込み領域アドレス              */
				                            /* (ポインタ移動後、必ず領域1に移動後 */
				                            /* カレントクラスタ番号を含むFATセクタ*/
				                            /* が読込まれている)                  */
				l_work01,                   /* クラスタ番号                       */
				&l_updclst_enum,            /* FATエントリ値                      */
				DFMR_FLGOFF                 /* Read/Write種別:Read                */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* 保存したポインタを元に戻す */
			tfmr_filehandle[ filehandle ].fptr = l_evacfptr;
			tfmr_filehandle[ filehandle ].crtsectnum = l_evaccrtsectnum;
			tfmr_filehandle[ filehandle ].crtsectofst = l_evaccrtsectofst;
		}
		l_work01 = tfmr_filehandle[ filehandle ].crtsectnum;
		l_work01 -= MFMR_DATATOP( l_drvnum );
		l_work01 /= MFMR_SECTPERCLST( l_drvnum );
		l_work01 += 2;
		l_work02 = ( F_DWORD )tfmr_filehandle[ filehandle ].direntry.fstclushi;
		l_work02 <<= 16;
		l_work02 |= ( F_DWORD )tfmr_filehandle[ filehandle ].direntry.fstcluslo;
		
		if( l_work01 == l_work02 )
		{
			/******************************************************************/
			/* カレントセクタがファイル先頭のクラスタに属している場合         */
			/******************************************************************/
			/* FATセクタ管理領域より空きクラスタを取得する */
			l_rtn = FMR_searchEmptyCluster(
				l_drvnum,               /* 対象論理ドライブ番号               */
				&l_work02               /* 取得空きクラスタ番号               */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* 取得した空きクラスタを含むFATセクタをFAT読込み領域0に読込む */
			l_rtn = FMR_getFatSector(
				MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報           */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
				                            /* FAT読込み領域                  */
				l_work02,                   /* クラスタ番号                   */
				&l_work01                   /* FATエントリ値                  */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* 更新書込み処理パラメータ設定 */
			l_newtopclstnum = l_work02; /* 新規取得先頭クラスタ番号に取得空クラスタ番号設定 */
			
			/* 更新書込み処理を行う */
			l_rtn = FMR_updateData(
				filehandle,             /* ファイルハンドル番号               */
				( F_BYTE * )writebuff,  /* 書込みデータ格納領域               */
				( F_DWORD )size,        /* 書込みサイズ                       */
				( l_work02 + 1 ),       /* 検索開始クラスタ番号               */
				( l_work02 + 1 ),       /* 未更新最終クラスタ番号は           */
				                        /* 検索開始クラスタと同値を渡す       */
				&l_newtopclstnum        /* 新規取得先頭クラスタ番号           */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* ファイルハンドル.エントリイメージの先頭クラスタ番号を保存      */
			l_updclst_snum = ( F_DWORD )tfmr_filehandle[ filehandle ].direntry.fstclushi;
			l_updclst_snum <<= 16;
			l_updclst_snum |= ( F_DWORD )tfmr_filehandle[ filehandle ].direntry.fstcluslo;

			/* ファイルハンドル.エントリイメージの先頭クラスタ番号を更新 */
			l_work01 = l_work02 & 0xFFFF0000;       /* 開始クラスタ番号上位を */
			l_work01 >>= 16;                        /* 取得                   */
			tfmr_filehandle[ filehandle ].direntry.fstclushi
				 = ( F_WORD )l_work01;
			l_work01 = l_work02 & 0x0000FFFF;       /* 同下位を取得           */
			tfmr_filehandle[ filehandle ].direntry.fstcluslo
				 = ( F_WORD )l_work01;
			l_mode = 2;
			
		}
		else
		{
			/******************************************************************/
			/* カレントセクタがファイル先頭のクラスタに属していない場合       */
			/******************************************************************/
			/* 未更新最終クラスタを求める */
			l_work01 = tfmr_filehandle[ filehandle ].fptr;
			l_work01 /= MFMR_BYTEPERSECT( l_drvnum );   /* カレントクラスタの */
			l_work01 /= MFMR_SECTPERCLST( l_drvnum );   /* 1つ前のクラスタの  */
			l_work01 -= 1;                              /* 先頭位置を取得     */
			l_work01 *= MFMR_SECTPERCLST( l_drvnum );
			l_work01 *= MFMR_BYTEPERSECT( l_drvnum );
			/* 現在のポインタを退避させる */
			l_evacfptr = tfmr_filehandle[ filehandle ].fptr;
			l_evaccrtsectnum = tfmr_filehandle[ filehandle ].crtsectnum;
			l_evaccrtsectofst = tfmr_filehandle[ filehandle ].crtsectofst;
			/* ファイルポインタを1つ前のクラスタの先頭へ移動させる */
			l_seekofst = l_work01 - l_evacfptr;
			l_rtn = FMR_setPointer(
				filehandle,
				l_seekofst,
				DFMR_SEEK_CUR
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			/* カレントセクタより未更新最終クラスタ番号を取得 */
			l_work01 = tfmr_filehandle[ filehandle ].crtsectnum;
			l_work01 -= MFMR_DATATOP( l_drvnum );
			l_work01 /= MFMR_SECTPERCLST( l_drvnum );
			l_work01 += 2;
			/* 退避していたポインタを元に戻す */
			l_savefptr = tfmr_filehandle[ filehandle ].fptr;
			l_savecrtsectnum = tfmr_filehandle[ filehandle ].crtsectnum;
			l_savecrtsectofst = tfmr_filehandle[ filehandle ].crtsectofst;
			tfmr_filehandle[ filehandle ].fptr = l_evacfptr;
			tfmr_filehandle[ filehandle ].crtsectnum = l_evaccrtsectnum;
			tfmr_filehandle[ filehandle ].crtsectofst = l_evaccrtsectofst;
			
			/* 未更新最終クラスタを含むFATセクタをFAT読込み領域0に読込む */
			l_rtn = FMR_getFatSector(
				MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報           */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
				                            /* FAT読込み領域                  */
				l_work01,                   /* クラスタ番号                   */
				&l_work02                   /* FATエントリ値                  */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			

			/* 未更新最終クラスタのFATエントリ値を保存      */
			MFMR_GETEOC( MFMR_FATTYPE( l_drvnum ), l_updclst_snum );
			if ( l_work02 == l_updclst_snum )
			{
				/* 未更新最終クラスタがファイル最終セクタの場合 */
				/* (ファイルが1クラスタのみの場合) */
				l_updclst_snum = l_work01;
			}
			else
			{
				/* 未更新最終クラスタがファイル最終セクタでない場合 */
				l_updclst_snum = l_work02;
			}
			l_newtopclstnum = 0; /* 新規取得先頭クラスタ番号に未取得設定 */

			/* 更新書込み処理を行う */
			l_rtn = FMR_updateData(
				filehandle,             /* ファイルハンドル番号               */
				( F_BYTE * )writebuff,  /* 書込みデータ格納領域               */
				( F_DWORD )size,        /* 書込みサイズ                       */
				l_updclst_snum,         /* 検索開始クラスタ番号               */
				l_work01,               /* 未更新最終クラスタ番号             */
				&l_newtopclstnum        /* 新規取得先頭クラスタ番号           */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			
			/* 未更新最終クラスタのFATエントリに取得した */
			/* 先頭セクタが属するクラスタ番号を設定する  */
			l_rtn = FMR_SubControlFatEntry(
				l_drvnum,               /* 論理ドライブ構成情報               */
				l_work01,               /* クラスタ番号                       */
				&l_newtopclstnum        /* FATエントリ値                      */
				);
			if( l_rtn != DFMR_OK )
			{
				return l_rtn;
			}
			l_mode = 3;
		}
		l_work01 = tfmr_filehandle[ filehandle ].fptr + size;
		l_work02 = tfmr_filehandle[ filehandle ].direntry.filesize;
		if( l_work01 > l_work02 )
		{
			/******************************************************************/
			/* ファイルサイズが更新された場合                                 */
			/******************************************************************/
			tfmr_filehandle[ filehandle ].direntry.filesize = l_work01;
		}
		
		/* 書込み済みサイズ更新 */
		l_restsize += size;
	}

	/******************************************************************/
	/* FAT読込み領域0書込み＆ディレクトリエントリ更新                 */
	/******************************************************************/
	/* FAT読込み領域0をデバイスに書込む */
	l_rtn = FMR_setFatSector(
		MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
		                            /* FAT読込み領域                      */
		DFMR_REFFATAREA0            /* FAT読込み領域番号                  */
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	FMR_osGetTime(                          /* 更新日時を取得             */
		&tfmr_filehandle[ filehandle ].direntry.wrtdate,   /* 更新日付   */
		&tfmr_filehandle[ filehandle ].direntry.wrttime,   /* 更新時刻   */
		NULL                        /* ミリ秒は不要                       */
		);
	tfmr_filehandle[ filehandle ].direntry.lstaccdate
		= tfmr_filehandle[ filehandle ].direntry.wrtdate;
		                            /* アクセス日付は更新日付と同値       */
	if ( l_mode == 0 )
	{
		l_rtn = FMR_setPointer(         /* ファイルポインタを終端に移動させる */
			filehandle,
			0,
			DFMR_SEEK_END
			);
	}
	else if ( l_mode == 1 )
	{
		l_rtn = FMR_setPointer(         /* ファイルポインタを終端に移動させる */
			filehandle,
			l_restsize,                 /* 書込み終了位置まで                 */
			DFMR_SEEK_CUR               /* 現在位置から                       */
			);
	}
	else if ( l_mode == 2 )
	{
		l_rtn = FMR_setPointer(         /* ファイルポインタを移動させる       */
			filehandle,
			l_work01,                   /* 書込み終了位置まで                 */
			DFMR_SEEK_SET               /* 先頭から                           */
			);
	}
	else
	{
		l_work01 -= l_savefptr;
		tfmr_filehandle[ filehandle ].fptr = l_savefptr;
		tfmr_filehandle[ filehandle ].crtsectnum = l_savecrtsectnum;
		tfmr_filehandle[ filehandle ].crtsectofst = l_savecrtsectofst;
		l_rtn = FMR_setPointer(         /* ファイルポインタを移動させる       */
			filehandle,
			l_work01,                   /* 書込み終了位置まで                 */
			DFMR_SEEK_CUR               /* 先頭から                           */
			);
	}
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	/* エントリが書込まれているセクタをR/Wバッファに読込む */
	l_rtn = FMR_readDataSector(
		l_drvnum,
		tfmr_filehandle[ filehandle ].entdirsectnum,
		0
		);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	
	/* R/Wバッファ上の対象エントリを更新する */
	l_entryptr = ( TFMR_DIRENTRY * )&( MFMR_RWBUFF( l_drvnum, 0,
					 tfmr_filehandle[ filehandle ].entdirsectpos * sizeof( TFMR_DIRENTRY ) ) );
	tfmr_filehandle[ filehandle ].direntry.attr |= DFMR_ATTR_ARC;   /* アーカイブ属性設定 */
	*( l_entryptr ) = tfmr_filehandle[ filehandle ].direntry;

#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	FMR_setBigEndianEntry(
		l_entryptr
	);
#endif /* DFMR_ENDIAN_BIG */

	/* エントリが書込まれているセクタにR/Wバッファを書込む */
	SetBackupFlg(l_drvnum);
	l_rtn = FMR_setDataSector(
		l_drvnum,
		tfmr_filehandle[ filehandle ].entdirsectnum,
		0
		);
	ClearBackupFlg(l_drvnum);
	if( l_rtn != DFMR_OK )
	{
		return l_rtn;
	}
	if ( (l_mode == 2) || (l_mode == 3) )
	{
		/**************************************************************************/
		/* 更新元クラスタを全て｢未使用｣とする                                     */
		/**************************************************************************/
		l_rtn = FMR_deleteDataCluster(
			l_drvnum,                       /* 対象論理ドライブ番号               */
			l_updclst_snum,                 /* 先頭クラスタ番号                   */
			l_updclst_enum                  /* 終了クラスタ番号                   */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	/**************************************************************************/
	/* 書込み済みサイズを返却                                                 */
	/**************************************************************************/
	return l_restsize;
}

static F_INT FMR_SubControlFatEntry ( 
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
	F_DWORD   clstnum,                  /* クラスタ番号                       */
	F_DWORD * entryval                  /* FATエントリ値                      */
)
{
	F_DWORD   l_work01;                 /* ワークエリア                       */
	F_INT     l_rtn;                    /* サブルーチン戻り値                 */

	l_rtn = FMR_controlFatEntry(
		MFMR_DRVINFO( drvnum ),         /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
		                                /* FAT読込み領域                      */
		clstnum,                        /* クラスタ番号                       */
		entryval,                       /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別 ON:設定             */
		);
	if( l_rtn == DFMR_NOFATENTRY )
	{
		/**********************************************************************/
		/* 現在アクセス中のクラスタがFAT読込み領域0が                         */
		/* 読込まれているFATセクタに属さない場合                              */
		/**********************************************************************/
		/* FAT読込み領域0を上書きする */
		l_rtn = FMR_setFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			DFMR_REFFATAREA0            /* FAT読込み領域番号                  */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/* FAT読込み領域0に該当クラスタを読込む */
		l_rtn = FMR_getFatSector(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域アドレス              */
			clstnum,                    /* クラスタ番号                       */
			&l_work01                   /* FATエントリ値                      */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}

		l_rtn = FMR_controlFatEntry(
			MFMR_DRVINFO( drvnum ),     /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域                      */
			clstnum,                    /* クラスタ番号                       */
			entryval,                   /* FATエントリ値                      */
			DFMR_FLGON                  /* Read/Write種別 ON:設定             */
			);
	}
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
