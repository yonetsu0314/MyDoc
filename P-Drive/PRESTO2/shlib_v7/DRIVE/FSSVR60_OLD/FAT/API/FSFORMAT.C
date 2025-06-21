/*======================================================================
 * File name    : fsformat.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/RCS/fsformat.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsformat.c $
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
/* 関 数 名：FMR_Format                                                       */
/*         ：                                                                 */
/* パラメタ：IN: TFMR_FORMATINFO * フォーマット情報領域アドレス               */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_ENTRYCNTERR                                       */
/*         ：          DFMR_FATTYPEERR                                        */
/*         ：          DFMR_NODRV                                             */
/*         ：          DFMR_CHANNELERR                                        */
/*         ：                                                                 */
/* 概    要：ファイルシステムのフォーマット                                   */
/*         ：                                                                 */
/* 作 成 日：2004.06.08                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 セクタサイズに直値512を使用しているのを修正     04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

static F_INT FMR_chkVolumeName(
    F_CHAR * volumename                 /* ボリューム名                       */
);
/* ボリュームIDエントリ作成 */
static F_INT FMR_setVolumeId(
	F_BYTE * bufptr,                    /* 書込み対象バッファ                 */
	F_CHAR * volname,                   /* ボリューム名                       */
	F_INT vollen,                       /* ボリューム名長                     */
	F_DWORD volsectnum,                 /* ボリュームエントリ書込みセクタ番号 */
	F_BYTE drvinfonum,                  /* ドライバ構成情報番号               */
	F_BYTE chanel_unit                  /* チャネル番号/ユニット番号          */
	
);

static F_INT FMR_writeSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 書込みセクタ番号                   */
    F_BYTE * buffer                     /* R/Wバッファ                        */
);

static F_INT FMR_eraseSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 消去セクタ番号                     */
    F_WORD   sectcnt                    /* 消去セクタ数                       */
);

static F_INT FMR_readSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 読込みセクタ番号                   */
    F_BYTE * buffer                     /* R/Wバッファ                        */
);

int FMR_Format(
    TFMR_FORMATINFO * formatinfo        /* フォーマット情報領域               */
)
{
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_DWORD l_cnt02;
	F_DWORD l_sectperfat;               /* FAT1件当たりのセクタ数             */
	F_DWORD l_volsectnum;               /* ボリュームエントリ書込みセクタ番号 */
	F_DWORD l_rootclstnum;              /* FAT32Root Dir.のクラスタ番号       */
	F_DWORD l_rootclstmax;              /* 上値の最大許容値                   */
	
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_totalsectnum;             /* 総セクタ数                         */
	F_DWORD l_dataclstcnt;              /* データクラスタ数                   */
	F_DWORD l_topdatasectnum;           /* Data Region 先頭セクタ番号         */
	F_DWORD l_bytepersect;              /* セクタ当りのバイト数               */
	
	F_DWORD l_clstnum;                  /* クラスタ番号                       */
	F_DWORD l_fatoffset;
	F_DWORD l_thisfatsecnum;
	F_DWORD l_thisfatentoffset;
	F_WORD * l_fatentptr;
	F_DWORD * l_fatentptr2;

	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/O                        */
	TFMR_DRVIOPARAM l_drvioparam02;
	F_INT l_ortn;                       /* OS部戻り値                         */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	F_INT l_rtn;                        /* 戻り値                             */
	F_INT l_vollen;                     /* ボリューム名長                     */

	F_WORD l_BPBrsvdseccnt;             /* Reserved Reginセクタ数             */
	F_WORD l_getday;                    /* 時間(ボリュームシリアル番号)取得用 */
	F_WORD l_gettim;                    /* 時間(ボリュームシリアル番号)取得用 */
	F_WORD l_rootentcnt;                /* ルートディレクトリエントリ数       */
	F_WORD l_fatentryvalue;             /* FATエントリ値                      */

	F_BYTE l_sectimage[ DFMR_USR_MAXSECTSIZE + 1 ];
	                                    /* セクタイメージ                     */
	F_CHAR l_vollabel[ DFMR_VNAMESZ ];  /* ボリュームラベル                   */

	F_BYTE l_getmsec;                   /* 時間(ボリュームシリアル番号)取得用 */
	F_BYTE l_sectperclst;               /* クラスタ当りのセクタ数             */
	F_BYTE l_drvinfonum;                /* ドライバ構成情報番号               */
	F_BYTE l_chnum;                     /* チャネル番号                       */
	F_BYTE l_fattype;                   /* FATタイプ                          */
	F_BYTE l_rootchkflg;                /* FAT32ルートディレクト判定用フラグ  */
	                                    /*    ON :未設定                      */
	                                    /*    OFF:設定済み                    */

	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	l_rtn = DFMR_SUC;
	switch( formatinfo->FATtype )
	{
	case DFMR_FATTYPEDEP:
		break;
	case DFMR_FATTYPE12:
		if( ( formatinfo->entry_cnt 
			% ( DFMR_USR_MAXSECTSIZE / sizeof( TFMR_DIRENTRY ) ) != 0 )
			|| ( formatinfo->entry_cnt == 0 ) )
		{
			return DFMR_ENTRYCNTERR;
		}
		break;
	case DFMR_FATTYPE16:
	case DFMR_FATTYPE32:
		if( formatinfo->entry_cnt != 0 )
		{
			return DFMR_ENTRYCNTERR;
		}
		break;
	default:
		return DFMR_FATTYPEERR;
	}
	if( formatinfo->vol_name != NULL )
	{
		l_vollen = FMR_chkVolumeName(
			formatinfo->vol_name        /* ボリューム名                       */
			);
		if( l_vollen < DFMR_OK )        /* ボリューム名異常                   */
		{
			return l_vollen;
		}
		if( l_vollen == 0 )             /* ボリューム名長が0                  */
		{
			return DFMR_NMERR;
		}
		FMR_memSet(                     /* ボリュームラベル初期化             */
			&l_vollabel[ 0 ],
			DFMR_SPACE,
			DFMR_VNAMESZ
			);
		FMR_smallToCapital(             /* ボリュームラベル作成               */
			formatinfo->vol_name,       /* 変換前文字列                       */
			&l_vollabel[ 0 ],           /* 変換後文字列                       */
			l_vollen                    /* 変換文字数                         */
			);
	}
	/**************************************************************************/
	/* 対象ドライバ構成情報検索                                               */
	/**************************************************************************/
	for( l_drvinfonum = 0; l_drvinfonum < DFMR_USR_MAXDRIVER; l_drvinfonum ++ )
	{
		if( (formatinfo->driverid & ~DFMR_MBR_OFF) == tfmr_drvinfo[ l_drvinfonum ].drvid )
		{
			break;
		}
	}
	if( l_drvinfonum == DFMR_USR_MAXDRIVER )
	{
		/* 対象ドライバが存在しない場合 */
		return DFMR_NODRV;
	}
	
	l_chnum = formatinfo->chanel_unit >> 4;
	l_chnum &= 0x0F;
	if( l_chnum >= tfmr_drvinfo[ l_drvinfonum ].channelnum )
	{
		/* チャネル番号がチャネル数以上の場合 */
		return DFMR_CHANNELERR;
	}
	/**************************************************************************/
	/* セマフォ資源獲得                                                       */
	/**************************************************************************/
	l_ortn = FMR_osWaiSem(
		tfmr_drvinfo[ l_drvinfonum ].semaphoreid[ l_chnum ]
		);
	if( l_ortn != DFMR_OK )
	{
		return l_ortn;
	}
	
	/**************************************************************************/
	/* マウントを行う                                                         */
	/**************************************************************************/
	if ( (formatinfo->driverid & DFMR_MBR_OFF) == 0 )
	{
		l_drvioparam.mount.mbrmode = DFMR_FLGOFF;
	}
	else {
		l_drvioparam.mount.mbrmode = DFMR_FLGON;
	}
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		formatinfo->chanel_unit,        /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCMOUNT,              /* I/O Func.番号:マウント             */
		&l_drvioparam
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
		goto ERROR1;
	}
	l_totalsectnum = l_drvioparam.mount.totalsectcnt;   /* 総セクタ数取得     */
	l_bytepersect = l_drvioparam.mount.sectsize * 512;
	                                    /* セクタ当りのバイト数取得           */
	l_fattype = formatinfo->FATtype;

	/* パーティションタイプの判定 */
	if( (formatinfo->driverid & DFMR_MBR_OFF) != DFMR_MBR_OFF )
	{
		if ( l_fattype == DFMR_FATTYPEDEP )
		{
			if( l_drvioparam.mount.totalsectcnt <= 2048 * l_bytepersect )
			{
				l_fattype = DFMR_FATTYPE16;
			}
			else
			{
				l_fattype = DFMR_FATTYPE32;
			}
		}
		switch( l_fattype )
		{
		case DFMR_FATTYPE12:
			if( l_drvioparam.mount.parttype != DFMR_PRTTBL_TYPEF12 )
			{
				l_rtn = DFMR_PARTTYPERR;
			}
			break;
		case DFMR_FATTYPE16:
			if( l_drvioparam.mount.parttype == DFMR_PRTTBL_TYPEF16LOW )
			{
				if( ( ( DFMR_FAT16LOWMAXSIZE * 1024 ) / l_bytepersect )
					< l_drvioparam.mount.totalsectcnt )
				{
					l_rtn = DFMR_PARTTYPERR;
				}
			}
			else if( l_drvioparam.mount.parttype == DFMR_PRTTBL_TYPEF16HI )
			{
				if( ( ( DFMR_FAT16LOWMAXSIZE * 1024 ) / l_bytepersect )
					>= l_drvioparam.mount.totalsectcnt )
				{
					l_rtn = DFMR_PARTTYPERR;
				}
			}
			else if( l_drvioparam.mount.parttype == DFMR_PRTTBL_TYPEF16LBA )
			{
			}
			else
			{
				l_rtn = DFMR_PARTTYPERR;
			}
			break;
		case DFMR_FATTYPE32:
			if( (l_drvioparam.mount.parttype != DFMR_PRTTBL_TYPEF32LBA) &&
				(l_drvioparam.mount.parttype != DFMR_PRTTBL_TYPEF32CHS) )
			{
				l_rtn = DFMR_PARTTYPERR;
			}
			break;
		default:
			return DFMR_FATTYPEERR;
		}
	}
	if( l_rtn == DFMR_SUC )
	{
		/* FATタイプの判定 */
		switch( l_fattype )
		{
		case DFMR_FATTYPE12:
			if( l_drvioparam.mount.totalsectcnt > 
				(F_DWORD)( 261376 / l_drvioparam.mount.sectsize ) )
			{
				l_rtn = DFMR_FATTYPEERR;
			}
			break;
		case DFMR_FATTYPE16:
			if( ( l_drvioparam.mount.totalsectcnt <= 
				(F_DWORD)( 8400 / l_drvioparam.mount.sectsize ) )
				|| ( ( 4194304 / l_drvioparam.mount.sectsize ) < 
					l_drvioparam.mount.sectsize ) )
			{
				l_rtn = DFMR_FATTYPEERR;
			}
			break;
		default:/* case DFMR_FATTYPEDEP: */ /* FATタイプがサイズ依存          */
			if( l_drvioparam.mount.totalsectcnt <= 
				(F_DWORD)( 66600 / l_drvioparam.mount.sectsize ) )
			{
				l_rtn = DFMR_FATTYPEERR;
			}
			break;
		}
	}
	if( l_rtn != DFMR_SUC )
	{
		/**********************************************************************/
		/* FATタイプ/パーティションタイプが異常の場合                         */
		/**********************************************************************/
		goto ERROR;
	}
	/**************************************************************************/
	/* デバイス情報取得                                                       */
	/**************************************************************************/
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		formatinfo->chanel_unit,        /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCDEV,                /* I/O Func.番号:デバイス情報取得     */
		&l_drvioparam02
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
		goto ERROR;
	}
	/**************************************************************************/
	/* 1クラスタ当りのセクタ数を求める                                        */
	/**************************************************************************/
	switch( l_fattype )
	{
	case DFMR_FATTYPE12:
		if( l_totalsectnum <= 4084 )
		{
			l_sectperclst = 1;
		}
		else if( ( 4084 < l_totalsectnum )
		&& ( l_totalsectnum <= 8168 ) )
		{
			l_sectperclst = 2;
		}
		else if( ( 8168 < l_totalsectnum )
		&& ( l_totalsectnum <= 16336 ) )
		{
			l_sectperclst = 4;
		}
		else if( ( 16336 < l_totalsectnum )
		&& ( l_totalsectnum <= 32672 ) )
		{
			l_sectperclst = 8;
		}
		else if( ( 32672 < l_totalsectnum )
		&& ( l_totalsectnum <= 65344 ) )
		{
			l_sectperclst = 16;
		}
		else if( ( 65344 < l_totalsectnum )
		&& ( l_totalsectnum <= 130688 ) )
		{
			l_sectperclst = 32;
		}
		else if( ( 130688 < l_totalsectnum )
		&& ( l_totalsectnum <= 261376 ) )
		{
			l_sectperclst = 64;
		}
		else /* ( 261376 < l_totalsectnum ) */
		{
			l_sectperclst = 0xFF;       /* Fail Safe                          */
		}
		break;
	case DFMR_FATTYPE16:
		if( l_totalsectnum <= 8400 )
		{
			l_sectperclst = 0xFF;
		}
		else if( ( 8400 < l_totalsectnum )
		&& ( l_totalsectnum <= 32680 ) )
		{
			l_sectperclst = 2;
		}
		else if( ( 32680 < l_totalsectnum )
		&& ( l_totalsectnum <= 262144 ) )
		{
			l_sectperclst = 4;
		}
		else if( ( 262144 < l_totalsectnum )
		&& ( l_totalsectnum <= 524288 ) )
		{
			l_sectperclst = 8;
		}
		else if( ( 524288 < l_totalsectnum )
		&& ( l_totalsectnum <= 1048576 ) )
		{
			l_sectperclst = 16;
		}
		else if( ( 1048576 < l_totalsectnum )
		&& ( l_totalsectnum <= 2097152 ) )
		{
			l_sectperclst = 32;
		}
		else if( ( 2097152 < l_totalsectnum )
		&& ( l_totalsectnum <= 4194304 ) )
		{
			l_sectperclst = 64;
		}
		else /* ( 4194304 < l_totalsectnum ) */
		{
			l_sectperclst = 0xFF;       /* Fail Safe                          */
		}
		break;
	default:/* case DFMR_FATTYPEDEP: */ /* FATタイプがサイズ依存          */
		if( l_totalsectnum <= 66600 )
		{
			l_sectperclst = 0xFF;
		}
		else if( ( 66600 < l_totalsectnum )
		&& ( l_totalsectnum <= 532480 ) )
		{
			l_sectperclst = 1;
		}
		else if( ( 532480 < l_totalsectnum )
		&& ( l_totalsectnum <= 16777216 ) )
		{
			l_sectperclst = 8;
		}
		else if( ( 16777216 < l_totalsectnum )
		&& ( l_totalsectnum <= 33554432 ) )
		{
			l_sectperclst = 16;
		}
		else if( ( 33554432 < l_totalsectnum )
		&& ( l_totalsectnum <= 67108864 ) )
		{
			l_sectperclst = 32;
		}
		else /* ( 67108864 < l_totalsectnum ) */
		{
			l_sectperclst = 64;
		}
		break;
	}
	if( l_sectperclst == 0xFF )        /* 総クラスタ数が許容外の値の場合      */
	{
		l_rtn = DFMR_FATTYPEERR;        /* FATタイプエラー                    */
		goto ERROR;
	}
	/**************************************************************************/
	/* Reserved Region消去                                                    */
	/**************************************************************************/
	switch( l_fattype )
	{
	case DFMR_FATTYPE12:
	case DFMR_FATTYPE16:
		l_BPBrsvdseccnt = DFMR_BPBRSVDSECCNT12;
		break;
	default: /* case DFMR_FATTYPE32 */
		l_BPBrsvdseccnt = DFMR_BPBRSVDSECCNT32;
		break;
	}
	l_drtn = FMR_eraseSector(
		l_drvinfonum,
		formatinfo->chanel_unit,
		0,
		l_BPBrsvdseccnt
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
		goto ERROR;
	}
	/**************************************************************************/
	/* BPB設定                                                                */
	/**************************************************************************/
	FMR_memSet(
		&l_sectimage[ 0 ],
		DFMR_INITBYTE,
		DFMR_USR_MAXSECTSIZE
		);
	/* x86無条件分岐命令 */
	l_sectimage[ DFMR_BSOFST_JMPBOOT ] = DFMR_BSJMPBOOT00;
	l_sectimage[ DFMR_BSOFST_JMPBOOT + 1 ] = DFMR_BSJMPBOOT01;
	l_sectimage[ DFMR_BSOFST_JMPBOOT + 2 ] = DFMR_BSJMPBOOT02;
	
	/* システム名 */
	FMR_memCopy(
		&l_sectimage[ DFMR_BSOFST_OEMNAME ],
		DFMR_BSOEMNAME,
		( DFMR_BPBOFST_BYTEPERSEC - DFMR_BSOFST_OEMNAME )
		);
	
	/* 1セクタ当りのバイト数 */
	l_work01 = l_bytepersect;
	l_sectimage[ DFMR_BPBOFST_BYTEPERSEC ] = ( F_BYTE )( l_work01 & 0x000000FF );
	l_sectimage[ DFMR_BPBOFST_BYTEPERSEC + 1 ] = ( F_BYTE )( (l_work01 & 0x0000FF00) >> 8 );
	
	/* クラスタ当りのセクタ数 */
	l_sectimage[ DFMR_BPBOFST_SECPERCLUS ] = l_sectperclst;
	
	/* Reserved Regionのセクタ数 */
	l_sectimage[ DFMR_BPBOFST_RSVDSECCNT ] = ( F_BYTE )( l_BPBrsvdseccnt & 0x00FF );
	l_sectimage[ DFMR_BPBOFST_RSVDSECCNT + 1 ] = ( F_BYTE )( (l_BPBrsvdseccnt & 0xFF00) >> 8 );
	
	/* FAT数 */
	l_sectimage[ DFMR_BPBOFST_NUMFATS ] = DFMR_BPBNUMFATS;
	
	/* Root Dir.内のディレクトリエントリ数 */
	switch( l_fattype )
	{
	case DFMR_FATTYPE12:
		l_rootentcnt = formatinfo->entry_cnt;
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT ] = ( F_BYTE )(formatinfo->entry_cnt & 0x00FF);
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT + 1 ]=(F_BYTE)((formatinfo->entry_cnt & 0xFF00) >> 8);
		break;
	case DFMR_FATTYPE16:
		l_rootentcnt = DFMR_BPBROOTENTCNT;
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT ] = ( F_BYTE )( DFMR_BPBROOTENTCNT & 0x00FF );
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT + 1 ] = (F_BYTE)((DFMR_BPBROOTENTCNT & 0xFF00) >> 8);
		break;
	default: /* case DFMR_FATTYPE32 */
		l_rootentcnt = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_ROOTENTCNT + 1 ] = DFMR_INITBYTE;
		break;
	}
	
	/* 全セクタ数 */
	if( l_totalsectnum < 65536 )
	{
		l_sectimage[ DFMR_BPBOFST_TOTSEC16 ] = ( F_BYTE )( l_totalsectnum & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST_TOTSEC16 + 1 ] = (F_BYTE)((l_totalsectnum & 0x0000FF00) >> 8);
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 1 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 2 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 3 ] = DFMR_INITBYTE;
	}
	else
	{
		l_sectimage[ DFMR_BPBOFST_TOTSEC16 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_TOTSEC16 + 1 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 ] = ( F_BYTE )( l_totalsectnum & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 1 ] = ( F_BYTE )((l_totalsectnum & 0x0000FF00) >> 8);
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 2 ] = ( F_BYTE )((l_totalsectnum & 0x00FF0000) >> 16);
		l_sectimage[ DFMR_BPBOFST_TOTSEC32 + 3 ] = ( F_BYTE )((l_totalsectnum & 0xFF000000) >> 24);
	}
	
	/* メディアタイプ */
	l_sectimage[ DFMR_BPBOFST_MEDIA ] = tfmr_drvinfo[ l_drvinfonum ].mediatype;
	
	/* 1つのFATが占めるセクタ数 */
	l_work01 = formatinfo->entry_cnt * sizeof( TFMR_DIRENTRY );
	l_work01 += l_bytepersect - 1;
	l_work01 /= l_bytepersect;      
	                                    /* Root Dir. Reg.のセクタ数           */
	l_work01 = l_totalsectnum - ( l_BPBrsvdseccnt + l_work01 );
	                                    /* 全セクタ数からReserved/Root Dir.を */
	                                    /* 差引く→対象セクタ数               */
	switch( l_fattype )
	{
	case DFMR_FATTYPE12:
		l_work01 = ( l_work01 + l_sectperclst - 1 ) / l_sectperclst;
		                                /* 総クラスタ数                       */
		l_work01 = l_work01 + ( l_work01 / 2 ) + l_bytepersect - 1;
		l_sectperfat = l_work01 / l_bytepersect;
		                                /* FATサイズ                          */
		l_sectimage[ DFMR_BPBOFST_FATSZ16 ] = ( F_BYTE )( l_sectperfat & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST_FATSZ16 + 1 ] = ( F_BYTE )((l_sectperfat & 0x0000FF00) >> 8);
		break;
	case DFMR_FATTYPE16:
		l_work02 = ( l_sectperclst << 8 ) + DFMR_BPBNUMFATS;
		l_sectperfat = ( l_work01 + l_work02 - 1 ) / l_work02;
		l_sectimage[ DFMR_BPBOFST_FATSZ16 ] = ( F_BYTE )( l_sectperfat & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST_FATSZ16 + 1 ] = ( F_BYTE )((l_sectperfat & 0x0000FF00) >> 8);
		break;
	default: /* case DFMR_FATTYPE32 */
		l_work02 = ( l_sectperclst << 8 ) + DFMR_BPBNUMFATS;
		l_work02 >>= 1;
		l_sectperfat = ( l_work01 + l_work02 - 1 ) / l_work02;
		l_sectimage[ DFMR_BPBOFST_FATSZ16 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST_FATSZ16 + 1 ] = DFMR_INITBYTE;
		l_sectimage[ DFMR_BPBOFST32_FATSZ32 ] = ( F_BYTE )( l_sectperfat & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST32_FATSZ32 + 1 ] = ( F_BYTE )((l_sectperfat & 0x0000FF00) >> 8);
		l_sectimage[ DFMR_BPBOFST32_FATSZ32 + 2 ] = ( F_BYTE )((l_sectperfat & 0x00FF0000) >> 16);
		l_sectimage[ DFMR_BPBOFST32_FATSZ32 + 3 ] = ( F_BYTE )((l_sectperfat & 0xFF000000) >> 24);
		
		break;
	}
	
	/* INT13のための1トラック当りのセクタ数 */
	l_sectimage[ DFMR_BPBOFST_SECPERTRK ] = (F_BYTE)(l_drvioparam02.devinfo.sctpertrack & 0x00FF);
	l_sectimage[ DFMR_BPBOFST_SECPERTRK + 1 ] = 
					( F_BYTE )( ( l_drvioparam02.devinfo.sctpertrack & 0xFF00 ) >> 8 );
	
	/* INT13のためのヘッド数 */
	l_sectimage[ DFMR_BPBOFST_NUMHEADERS ] = (F_BYTE)(l_drvioparam02.devinfo.headnum & 0x00FF);
	l_sectimage[ DFMR_BPBOFST_NUMHEADERS + 1 ] = 
					( F_BYTE )( ( l_drvioparam02.devinfo.headnum & 0xFF00 ) >> 8 );
	
	/* ボリュームの前にあるセクタの数 */
	l_sectimage[ DFMR_BPBOFST_HIDDSEC ] = ( F_BYTE )( l_drvioparam.mount.sectnum & 0x000000FF );
	l_sectimage[ DFMR_BPBOFST_HIDDSEC + 1 ] = 
					( F_BYTE )( ( l_drvioparam.mount.sectnum & 0x0000FF00 ) >> 8 );
	l_sectimage[ DFMR_BPBOFST_HIDDSEC + 2 ] = 
					( F_BYTE )( ( l_drvioparam.mount.sectnum & 0x00FF0000 ) >> 16 );
	l_sectimage[ DFMR_BPBOFST_HIDDSEC + 3 ] = 
					( F_BYTE )( ( l_drvioparam.mount.sectnum & 0xFF000000 ) >> 24 );
	
	if( l_fattype != DFMR_FATTYPE32 )
	{
		/**********************************************************************/
		/* 以下、FAT12/16の設定                                               */
		/**********************************************************************/
		/* INT13のドライブ番号 */
		l_sectimage[ DFMR_BSOFST_DRVNUM ] = DFMR_BSDRVNUM;
		
		/* 予約領域 */
		l_sectimage[ DFMR_BSOFST_RESERVED1 ] = DFMR_BSRESERVED1;
		
		/* 拡張ブートシグネチャ */
		l_sectimage[ DFMR_BSOFST_BOOTSIG ] = DFMR_BSBOOTSIG;
		
		/* ボリュームシリアル番号 */
		FMR_osGetTime(
			&l_getday,
			&l_gettim,
			NULL
			);
		l_sectimage[ DFMR_BSOFST_VOLID ] = ( F_BYTE )( l_getday & 0x00FF );
		l_sectimage[ DFMR_BSOFST_VOLID + 1 ] = ( F_BYTE )( ( l_getday & 0xFF00 ) >> 8 );
		l_sectimage[ DFMR_BSOFST_VOLID + 2 ] = ( F_BYTE )( l_gettim & 0x00FF );
		l_sectimage[ DFMR_BSOFST_VOLID + 3 ] = ( F_BYTE )( ( l_gettim & 0xFF00 ) >> 8 );
	
		/* ボリュームラベル */
		if( formatinfo->vol_name != NULL )
		{                               /* ボリューム名指定有り               */
			FMR_memSet(                 /* 領域をスペースで初期化             */
				&l_sectimage[ DFMR_BSOFST_VOLLAB ],
				DFMR_SPACE,
				DFMR_VNAMESZ
				);
			FMR_memCopy(                /* ボリューム名をコピー               */
				&l_sectimage[ DFMR_BSOFST_VOLLAB ],
				&l_vollabel[ 0 ],
				l_vollen
				);
		}
		else                            /* ボリューム名指定無し               */
		{
			FMR_memCopy(                /* ボリューム名はデフォルト値         */
				&l_sectimage[ DFMR_BSOFST_VOLLAB ],
				DFMR_BPBVOLLAB,
				DFMR_VNAMESZ
				);
		}
		/* FATタイプ */
		if( l_fattype == DFMR_FATTYPE12 )
		{                               /* FAT12の場合                        */
			FMR_memCopy(
				&l_sectimage[ DFMR_BSOFST_FILESYSTYPE ],
				DFMR_BSFILESYSTYPE12,
				8
				);
		}
		else
		{                               /* FAT16の場合                        */
			FMR_memCopy(
				&l_sectimage[ DFMR_BSOFST_FILESYSTYPE ],
				DFMR_BSFILESYSTYPE16,
				8
				);
		}
	}
	else
	{
		/**********************************************************************/
		/* 以下、FAT32の場合                                                  */
		/**********************************************************************/
		/* 動作中のFATを表すフラグ郡 */
		l_sectimage[ DFMR_BPBOFST32_EXTFLGS ] = ( F_BYTE )( DFMR_ALLUPDATE & 0x00FF );
		l_sectimage[ DFMR_BPBOFST32_EXTFLGS + 1 ] = ( F_BYTE )( ( DFMR_ALLUPDATE & 0xFF00 ) >> 8 );
		
		/* FAT32のバージョン番号 */
		l_sectimage[ DFMR_BPBOFST32_FSVER ] = ( F_BYTE )( DFMR_BPBFSVER & 0x00FF );
		l_sectimage[ DFMR_BPBOFST32_FSVER ] = ( F_BYTE )( ( DFMR_BPBFSVER & 0xFF00 ) >> 8 );
		
		/* ルートディレクトリの先頭クラスタ番号 */
		l_sectimage[ DFMR_BPBOFST32_ROOTCLUS ] = ( F_BYTE )( DFMR_BPBROOTCLUS & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 1 ] =(F_BYTE)((DFMR_BPBROOTCLUS & 0x0000FF00) >> 8);
		l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 2 ] =(F_BYTE)((DFMR_BPBROOTCLUS & 0x00FF0000) >> 16);
		l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 3 ] =(F_BYTE)((DFMR_BPBROOTCLUS & 0xFF000000) >> 24);
		
		
		/* FSInfo構造体のセクタ番号 */
		l_sectimage[ DFMR_BPBOFST32_FSINFO ] = ( F_BYTE )( DFMR_BPBFSINFO & 0x000000FF );
		l_sectimage[ DFMR_BPBOFST32_FSINFO + 1 ] =(F_BYTE)((DFMR_BPBFSINFO & 0x0000FF00) >> 8);
		l_sectimage[ DFMR_BPBOFST32_FSINFO + 2 ] =(F_BYTE)((DFMR_BPBFSINFO & 0x00FF0000) >> 16);
		l_sectimage[ DFMR_BPBOFST32_FSINFO + 3 ] =(F_BYTE)((DFMR_BPBFSINFO & 0xFF000000) >> 24);
		
		/* ブートレコードのコピー */
		l_sectimage[ DFMR_BPBOFST32_BKBOOTSEC ] = ( F_BYTE )( DFMR_BPBBKBOOTSEC & 0x00FF );
		l_sectimage[ DFMR_BPBOFST32_BKBOOTSEC + 1 ] =(F_BYTE)((DFMR_BPBBKBOOTSEC & 0xFF00) >> 8);
		
		/* 予約領域 */
		l_sectimage[ DFMR_BSOFST32_RESERVED1 ] = DFMR_BPBRESERVED;
		
		/* 拡張ブートシグネチャ */
		l_sectimage[ DFMR_BSOFST32_BOOTSIG ] = DFMR_BSBOOTSIG;
		
		/* INT13のドライブ番号 */
		l_sectimage[ DFMR_BSOFST32_DRVNUM ] = DFMR_BSDRVNUM;
		
		/* 予約領域 */
		l_sectimage[ DFMR_BSOFST32_RESERVED1 ] = DFMR_BSOFST32_RESERVED1;
		
		/* 拡張ブートシグネチャ */
		l_sectimage[ DFMR_BSOFST32_BOOTSIG ] = DFMR_BSBOOTSIG;
		
		/* ボリュームシリアル番号 */
		FMR_osGetTime(
			&l_getday,
			&l_gettim,
			NULL
			);
		l_sectimage[ DFMR_BSOFST32_VOLID ] = ( F_BYTE )( l_getday & 0x00FF );
		l_sectimage[ DFMR_BSOFST32_VOLID + 1 ] = ( F_BYTE )( ( l_getday & 0xFF00 ) >> 8 );
		l_sectimage[ DFMR_BSOFST32_VOLID + 2 ] = ( F_BYTE )( l_gettim & 0x00FF );
		l_sectimage[ DFMR_BSOFST32_VOLID + 3 ] = ( F_BYTE )( ( l_gettim & 0xFF00 ) >> 8 );
	
		/* ボリュームラベル */
		if( formatinfo->vol_name != NULL )
		{                               /* ボリューム名指定有り               */
			FMR_memSet(                 /* 領域をスペースで初期化             */
				&l_sectimage[ DFMR_BSOFST32_VOLLAB ],
				DFMR_SPACE,
				DFMR_VNAMESZ
				);
			FMR_memCopy(
				&l_sectimage[ DFMR_BSOFST32_VOLLAB ],
				&l_vollabel[ 0 ],
				l_vollen
				);
		}
		else                            /* ボリューム名指定無し               */
		{
			FMR_memCopy(                /* ボリューム名はデフォルト値         */
				&l_sectimage[ DFMR_BSOFST32_VOLLAB ],
				DFMR_BPBVOLLAB,
				DFMR_VNAMESZ
				);
		}
		
		/* FATタイプ */
		FMR_memCopy(
			&l_sectimage[ DFMR_BSOFST32_FILESYSTYPE ],
			DFMR_BSFILESYSTYPE32,
			8
			);
	}
	/* ブートシグネチャ */
	l_sectimage[ DFMR_RSVRGN_SIGP510 ] = DFMR_RSVRGN_SIGV510;
	l_sectimage[ DFMR_RSVRGN_SIGP511 ] = DFMR_RSVRGN_SIGV511;
	
	/**************************************************************************/
	/* BPB書込み                                                              */
	/**************************************************************************/
	l_drtn = FMR_writeSector(
		l_drvinfonum,
		formatinfo->chanel_unit,
		0,
		&l_sectimage[ 0 ]
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
		goto ERROR;
	}
	/**************************************************************************/
	/* FS Info 設定/書き込み                                                  */
	/**************************************************************************/
	if( l_fattype == DFMR_FATTYPE32 )
	{
		FMR_memSet(
			&l_sectimage[ 0 ],
			DFMR_INITBYTE,
			DFMR_USR_MAXSECTSIZE
			);
		/* リードシグネチャ */
		l_sectimage[ DFMR_FSIOFST_LEADSIG ] = DFMR_FSILEADSIG0;
		l_sectimage[ DFMR_FSIOFST_LEADSIG + 1 ] = DFMR_FSILEADSIG1;
		l_sectimage[ DFMR_FSIOFST_LEADSIG + 2 ] = DFMR_FSILEADSIG2;
		l_sectimage[ DFMR_FSIOFST_LEADSIG + 3 ] = DFMR_FSILEADSIG3;
		/* 構造シグネチャ */
		l_sectimage[ DFMR_FSIOFST_STRUCSIG ] = DFMR_FSISTRUCSIG0;
		l_sectimage[ DFMR_FSIOFST_STRUCSIG + 1 ] = DFMR_FSISTRUCSIG1;
		l_sectimage[ DFMR_FSIOFST_STRUCSIG + 2 ] = DFMR_FSISTRUCSIG2;
		l_sectimage[ DFMR_FSIOFST_STRUCSIG + 3 ] = DFMR_FSISTRUCSIG3;
		/* 空きクラスタ数/番号 */
		for( l_cnt01 = 0; l_cnt01 < sizeof( F_DWORD ); l_cnt01 ++ )
		{
			l_sectimage[ DFMR_FSIOFST_FREECOUNT + l_cnt01 ] = DFMR_FSICLSTINIT;
			l_sectimage[ DFMR_FSIOFST_NXTFREE + l_cnt01 ] = DFMR_FSICLSTINIT;
		}
		/* FS Info書込み */
		l_drtn = FMR_writeSector(
			l_drvinfonum,
			formatinfo->chanel_unit,
			DFMR_BPBFSINFO,
			&l_sectimage[ 0 ]
			);
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* FAT Region初期化                                                       */
	/**************************************************************************/
	for( l_cnt01 = 0 ;l_cnt01 < DFMR_BPBNUMFATS ;l_cnt01 ++ )
	{
		/* FATセクタ消去 */
		l_drtn = FMR_eraseSector(
			l_drvinfonum,
			formatinfo->chanel_unit,
			l_BPBrsvdseccnt + ( l_cnt01 * l_sectperfat ),
			(F_WORD)l_sectperfat
			);
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
			goto ERROR;
		}
		/* FAT先頭セクタ設定 */
		FMR_memSet(
			&l_sectimage[ 0 ],
			DFMR_INITBYTE,
			DFMR_USR_MAXSECTSIZE
			);
		switch( l_fattype )
		{
		case DFMR_FATTYPE12:
			l_sectimage[ 0 ] = 0xF8;
			l_sectimage[ 1 ] = 0xFF;
			l_sectimage[ 2 ] = 0xFF;
			break;
		case DFMR_FATTYPE16:
			l_sectimage[ 0 ] = 0xF8;
			l_sectimage[ 1 ] = 0xFF;
			l_sectimage[ 2 ] = 0xFF;
			l_sectimage[ 3 ] = 0xFF;
			break;
		default: /* case DFMR_FATTYPE32 */
			l_sectimage[ 0 ] = 0xF8;
			l_sectimage[ 1 ] = 0xFF;
			l_sectimage[ 2 ] = 0xFF;
			l_sectimage[ 3 ] = 0x0F;
			l_sectimage[ 4 ] = 0xFF;
			l_sectimage[ 5 ] = 0xFF;
			l_sectimage[ 6 ] = 0xFF;
			l_sectimage[ 7 ] = 0x0F;
			l_sectimage[ 8 ] = 0xFF;
			l_sectimage[ 9 ] = 0xFF;
			l_sectimage[ 10 ] = 0xFF;
			l_sectimage[ 11 ] = 0x0F;
			break;
		}
		
		/* FATセクタ書込み */
		l_drtn = FMR_writeSector(
			l_drvinfonum,
			formatinfo->chanel_unit,
			l_BPBrsvdseccnt + ( l_cnt01 * l_sectperfat ),
			&l_sectimage[ 0 ]
			);
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* Root Dir. Region初期化                                                 */
	/**************************************************************************/
	l_volsectnum = l_BPBrsvdseccnt + ( DFMR_BPBNUMFATS * l_sectperfat );
	                                    /* FAT直後のセクタ番号を取得          */
	if( l_fattype != DFMR_FATTYPE32 )     /* FAT32以外の場合        */
	{
		/* Root Dir. Region消去 */
		l_work01 = l_rootentcnt * ( l_bytepersect / sizeof( TFMR_DIRENTRY ) );
		l_work01 += l_bytepersect - 1;
		l_work01 /= l_bytepersect;
		l_rootentcnt = (F_WORD)l_work01;
		l_drtn = FMR_eraseSector(
			l_drvinfonum,
			formatinfo->chanel_unit,
			l_volsectnum,
			(F_WORD)l_work01
			);
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
			goto ERROR;
		}
	}

	if( ( formatinfo->vol_name != NULL )/* ボリューム名指定ありの場合         */
		&& ( l_fattype != DFMR_FATTYPE32 ) )    /* FAT32以外の場合            */
	{
		/**********************************************************************/
		/* FAT 12/16 Volume ID エントリ作成、書込み                           */
		/**********************************************************************/
		l_drtn = FMR_setVolumeId(
			&l_sectimage[ 0 ],          /* 書込み対象バッファ                 */
			&l_vollabel[ 0 ],           /* ボリューム名                       */
			l_vollen,                   /* ボリューム名長                     */
			l_volsectnum,               /* ボリュームエントリ書込みセクタ番号 */
			l_drvinfonum,               /* ドライバ構成情報番号               */
			formatinfo->chanel_unit     /* チャネル番号/ユニット番号          */
			);
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* クラスタ数計算                                                         */
	/**************************************************************************/
	l_dataclstcnt = l_totalsectnum;
	l_dataclstcnt -= ( l_BPBrsvdseccnt + ( l_sectperfat * DFMR_BPBNUMFATS ) + l_rootentcnt );
	l_dataclstcnt /= l_sectperclst;
	
	l_topdatasectnum = l_BPBrsvdseccnt + ( l_sectperfat * DFMR_BPBNUMFATS ) + l_rootentcnt;
	
	l_rootchkflg = DFMR_FLGON;          /* FAT32のルートクラスタ未設定        */
	l_rootclstnum = DFMR_BPBROOTCLUS;   /* ルートDir.先頭クラスタ番号初期化   */
	l_rootclstmax = l_dataclstcnt/100;
	l_rootclstmax *= DFMR_USR_F32RDCLST;
	/**************************************************************************/
	/* Data Region/FAT初期設定                                                */
	/**************************************************************************/
	for( l_cnt01 = 0; l_cnt01 < l_dataclstcnt; l_cnt01 ++ )
	{
		/* クラスタ消去 */
		l_drtn = FMR_eraseSector(
			l_drvinfonum,
			formatinfo->chanel_unit,
			l_topdatasectnum + ( l_cnt01 * l_sectperclst ),
			(F_WORD)l_sectperclst
			);
		if( l_drtn != DFMR_OK )         /* 処理失敗又はセクタ数異常時         */
		{
			for( l_cnt02 = 0 ;l_cnt02 < DFMR_BPBNUMFATS ;l_cnt02 ++ )
			{                           /* FAT数回                            */
				/* FATエントリにBADクラスタ設定 */
				l_clstnum = l_cnt01 + DFMR_TOPCLSTNUM;  /* クラスタ番号取得   */
				switch( l_fattype )
				{
				case DFMR_FATTYPE12:
					l_fatoffset = l_clstnum + ( l_clstnum / 2 );
					l_thisfatsecnum = l_BPBrsvdseccnt + ( l_fatoffset / l_bytepersect );
					l_thisfatsecnum += l_cnt02 * l_sectperfat;
					l_thisfatentoffset = l_fatoffset % l_bytepersect;
					
					if( l_thisfatentoffset == ( l_bytepersect - 1 ) )
					{
						l_drtn = FMR_readSector(
							l_drvinfonum,
							formatinfo->chanel_unit,
							l_thisfatsecnum + 1,
							&l_sectimage[ 0 ]
							);
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
							goto ERROR;
						}
						l_sectimage[ l_bytepersect ] = l_sectimage[ 0 ];
					}
					l_drtn = FMR_readSector(
						l_drvinfonum,
						formatinfo->chanel_unit,
						l_thisfatsecnum,
						&l_sectimage[ 0 ]
						);
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
						goto ERROR;
					}
#ifdef DFMR_ENDIAN_BIG
					l_fatentryvalue = ( F_WORD )l_sectimage[ l_thisfatentoffset ];
					l_fatentryvalue <<= 8;
					l_fatentryvalue |= ( F_WORD )l_sectimage[ l_thisfatentoffset + 1 ];
#else /* DFMR_ENDIAN_BIG */
					l_fatentptr = ( F_WORD * )&l_sectimage[ l_thisfatentoffset ];
					l_fatentryvalue = *( l_fatentptr );
#endif /* DFMR_ENDIAN_BIG */
					if( ( l_clstnum & 1 ) == 1 )
					{
						l_fatentryvalue &= 0x000F;
						l_fatentryvalue |= 0xFF70;
					}
					else
					{
						l_fatentryvalue &= 0xF000;
						l_fatentryvalue |= 0x0FF7;
					}
#ifdef DFMR_ENDIAN_BIG
					l_sectimage[ l_thisfatentoffset + 1 ] = ( F_BYTE )l_fatentryvalue & 0x00FF;
					l_sectimage[ l_thisfatentoffset ] =(F_BYTE)((l_fatentryvalue & 0xFF00) >> 8);
#else /* DFMR_ENDIAN_BIG */
					*( l_fatentptr ) = l_fatentryvalue;
#endif /* DFMR_ENDIAN_BIG */
					break;
				case DFMR_FATTYPE16:
					l_fatoffset = l_clstnum * 2;
					l_thisfatsecnum = l_BPBrsvdseccnt + ( l_fatoffset / l_bytepersect );
					l_thisfatsecnum += l_cnt02 * l_sectperfat;
					l_thisfatentoffset = l_fatoffset % l_bytepersect;
					
					l_drtn = FMR_readSector(
						l_drvinfonum,
						formatinfo->chanel_unit,
						l_thisfatsecnum,
						&l_sectimage[ 0 ]
						);
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
						goto ERROR;
					}
					l_fatentptr = ( F_WORD * )&l_sectimage[ l_thisfatentoffset ];
#ifdef DFMR_ENDIAN_BIG
					*( l_fatentptr ) = 0xF7FF;
#else /* DFMR_ENDIAN_BIG */
					*( l_fatentptr ) = 0xFFF7;
#endif /* DFMR_ENDIAN_BIG */
					break;
				default: /* case DFMR_FATTYPE32 */
					l_fatoffset = l_clstnum * 4;
					l_thisfatsecnum = l_BPBrsvdseccnt + ( l_fatoffset / l_bytepersect );
					l_thisfatsecnum += l_cnt02 * l_sectperfat;
					l_thisfatentoffset = l_fatoffset % l_bytepersect;
					
					l_drtn = FMR_readSector(
						l_drvinfonum,
						formatinfo->chanel_unit,
						l_thisfatsecnum,
						&l_sectimage[ 0 ]
						);
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
						goto ERROR;
					}
					l_fatentptr2 = ( F_DWORD * )&l_sectimage[ l_thisfatentoffset ];
#ifdef DFMR_ENDIAN_BIG
					*( l_fatentptr2 ) = 0xF7FFFF0F;
#else /* DFMR_ENDIAN_BIG */
					*( l_fatentptr2 ) = 0x0FFFFFF7;
#endif /* DFMR_ENDIAN_BIG */
					break;
				}
				/* FATエントリ書込み */
				l_drtn = FMR_writeSector(
					l_drvinfonum,
					formatinfo->chanel_unit,
					l_thisfatsecnum,
					&l_sectimage[ 0 ]
					);
				if( l_drtn != DFMR_OK )
				{
					l_rtn = l_drtn;
					goto ERROR;
				}
				if( l_fattype == DFMR_FATTYPE12 )
				{
					if( l_thisfatentoffset == ( l_bytepersect - 1 ) )
					{
						l_drtn = FMR_readSector(
							l_drvinfonum,
							formatinfo->chanel_unit,
							l_thisfatsecnum + 1,
							&l_sectimage[ 0 ]
							);
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
							goto ERROR;
						}
						l_sectimage[ 0 ] = l_sectimage[ l_bytepersect ];
						
						l_drtn = FMR_writeSector(
							l_drvinfonum,
							formatinfo->chanel_unit,
							l_thisfatsecnum + 1,
							&l_sectimage[ 0 ]
							);
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
							goto ERROR;
						}
					}
				}
			}
		}
		else if( ( l_rootchkflg == DFMR_FLGON ) /* FAT32でルートディレクトリ  */
			&& ( l_fattype == DFMR_FATTYPE32 ) )/* を取得していない場合       */
		{
			if ( l_cnt01 != 0 )
			{
				l_clstnum = l_cnt01 + DFMR_TOPCLSTNUM;  /* クラスタ番号取得   */
				for( l_cnt02 = 0 ;l_cnt02 < DFMR_BPBNUMFATS ;l_cnt02 ++ )
				{                       /* FAT数回                            */
					/* FATセクタを読込む */
					l_fatoffset = l_clstnum * 4;
					l_thisfatsecnum = l_BPBrsvdseccnt + ( l_fatoffset / l_bytepersect );
					l_thisfatsecnum += l_cnt02 * l_sectperfat;
					l_thisfatentoffset = l_fatoffset % l_bytepersect;
					
					l_drtn = FMR_readSector(
						l_drvinfonum,
						formatinfo->chanel_unit,
						l_thisfatsecnum,
						&l_sectimage[ 0 ]
						);
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
						goto ERROR;
					}
					/* FATエントリをEOCに設定する */
					l_fatentptr2 = ( F_DWORD * )&l_sectimage[ l_thisfatentoffset ];
#ifdef DFMR_ENDIAN_BIG
					*( l_fatentptr2 ) = 0xFFFFFF0F;
#else /* DFMR_ENDIAN_BIG */
					*( l_fatentptr2 ) = 0x0FFFFFFF;
#endif /* DFMR_ENDIAN_BIG */
					
					/* FATセクタを書込む */
					l_drtn = FMR_writeSector(
						l_drvinfonum,
						formatinfo->chanel_unit,
						l_thisfatsecnum,
						&l_sectimage[ 0 ]
						);
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
						goto ERROR;
					}
				}
				l_rootclstnum += l_cnt01;
			}
			/* ルートディレクトリ取得済みとする */
			l_rootchkflg = DFMR_FLGOFF;
		}
		if( ( l_rootchkflg == DFMR_FLGON )      /* FAT32でルートディレクトリ  */
			&& ( l_fattype == DFMR_FATTYPE32 ) )/* を取得していない場合       */
		{
			if( l_cnt01 >= l_rootclstmax )      /* ルートディレクトリが許容値 */
			{                                   /* 外となる場合               */
				return DFMR_FORMATERR;
			}
			/* ボリュームエントリ書込みセクタ番号更新 */
			l_volsectnum += l_sectperclst;
		}
	}
	/**************************************************************************/
	/* FAT32 ルートディレクトリ設定                                           */
	/**************************************************************************/
	if( l_fattype == DFMR_FATTYPE32 )
	{
		if( l_volsectnum != ( l_BPBrsvdseccnt + ( DFMR_BPBNUMFATS * l_sectperfat ) ) )
		{
			/* ルートディレクトリのクラスタがデフォルト値以外の場合 */
			/* BPBを読込む */
			l_drtn = FMR_readSector(
				l_drvinfonum,
				formatinfo->chanel_unit,
				0,
				&l_sectimage[ 0 ]
				);
			if( l_drtn != DFMR_OK )
			{
				l_rtn = l_drtn;
				goto ERROR;
			}
			/* ルートディレクトリの先頭クラスタ番号を更新 */
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS ]=(F_BYTE)((l_rootclstnum & 0xFF000000) >> 24);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 1 ]=(F_BYTE)((l_rootclstnum & 0x00FF0000) >> 16);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 2 ]=(F_BYTE)((l_rootclstnum & 0x0000FF00) >> 8);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 3 ]=(F_BYTE)(l_rootclstnum & 0x000000FF );
#else /* DFMR_ENDIAN_BIG */             /* Little Endian                      */
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS ]=(F_BYTE)(l_rootclstnum & 0x000000FF);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 1 ]=(F_BYTE)((l_rootclstnum & 0x0000FF00) >> 8);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 2 ]=(F_BYTE)((l_rootclstnum & 0x00FF0000) >> 16);
			l_sectimage[ DFMR_BPBOFST32_ROOTCLUS + 3 ]=(F_BYTE)((l_rootclstnum & 0xFF000000) >> 24);
#endif /* DFMR_ENDIAN_BIG */
			
			/* BPBを書込む */
			l_drtn = FMR_writeSector(
				l_drvinfonum,
				formatinfo->chanel_unit,
				0,
				&l_sectimage[ 0 ]
				);
			if( l_drtn != DFMR_OK )
			{
				l_rtn = l_drtn;
				goto ERROR;
			}
		}
		if( formatinfo->vol_name != NULL )
		{
			/* ボリューム名指定有りの場合 */
			l_drtn = FMR_setVolumeId(   /* ボリュームIDエントリ作成/書込み    */
				&l_sectimage[ 0 ],      /* 書込み対象バッファ                 */
				&l_vollabel[ 0 ],       /* ボリューム名                       */
				l_vollen,               /* ボリューム名長                     */
				l_volsectnum,           /* ボリュームエントリ書込みセクタ番号 */
				l_drvinfonum,           /* ドライバ構成情報番号               */
				formatinfo->chanel_unit /* チャネル番号/ユニット番号          */
				);
			if( l_drtn != DFMR_OK )
			{
				l_rtn = l_drtn;
			}
		}
	}
	/**************************************************************************/
	/* アンマウント                                                           */
	/**************************************************************************/
ERROR:
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		formatinfo->chanel_unit,        /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCUNMOUNT,            /* I/O Func.番号:アンマウント         */
		&l_drvioparam
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
	}
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
ERROR1:
	l_ortn = FMR_osSigSem(
		tfmr_drvinfo[ l_drvinfonum ].semaphoreid[ l_chnum ]
		);
	if( l_ortn != DFMR_OK )
	{
		l_rtn = l_ortn;
	}
	
	/**************************************************************************/
	/* 正常終了(異常発生時はエラー)返却                                       */
	/**************************************************************************/
	return l_rtn;
}

/* ボリューム名称チェック */
static F_INT FMR_chkVolumeName(
    F_CHAR * volumename                 /* ボリューム名                       */
)
{
	F_WORD l_namecnt;                   /* ボリューム名カウンタ               */
	
	for( l_namecnt = 0; l_namecnt <= DFMR_VNAMESZ; l_namecnt ++ )
	{
		if( ( '0' <= *( volumename ) && *( volumename ) <= '9' )
		|| ( 'a' <= *( volumename ) && *( volumename ) <= 'z' )
		|| ( 'A' <= *( volumename ) && *( volumename ) <= 'Z' )
		|| ( *( volumename ) == '!' )   /* ボリューム名の内容が半角の有効文字 */
		|| ( *( volumename ) == '#' )
		|| ( *( volumename ) == '$' )
		|| ( *( volumename ) == '%' )
		|| ( *( volumename ) == '&' )
		|| ( *( volumename ) == '\'' )
		|| ( *( volumename ) == '(' )
		|| ( *( volumename ) == ')' )
		|| ( *( volumename ) == '-' )
		|| ( *( volumename ) == '@' )
		|| ( *( volumename ) == '[' )
		|| ( *( volumename ) == ']' )
		|| ( *( volumename ) == '^' )
		|| ( *( volumename ) == '_' )
		|| ( *( volumename ) == '`' )
		|| ( *( volumename ) == '~' )
		)
		{
			volumename ++;              /* ボリューム名を1文字進める          */
		}
		else if( *( volumename ) == NULL )  /* ボリューム名の内容が終端       */
		{
			return l_namecnt;
		}
		else
		{
			return DFMR_NMERR;          /* 有効文字でない場合はエラー         */
		}
	}
	return DFMR_NMERR;                  /* 11文字以上の場合はエラー           */
}

/* ボリュームIDエントリ作成 */

static F_INT FMR_setVolumeId(
	F_BYTE * bufptr,                    /* 書込み対象バッファ                 */
	F_CHAR * volname,                   /* ボリューム名                       */
	F_INT vollen,                       /* ボリューム名長                     */
	F_DWORD volsectnum,                 /* ボリュームエントリ書込みセクタ番号 */
	F_BYTE drvinfonum,                  /* ドライバ構成情報番号               */
	F_BYTE chanel_unit                  /* チャネル番号/ユニット番号          */
)
{
	TFMR_DIRENTRY * l_direntptr;        /* ディレクトリエントリポインタ       */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	
	FMR_memSet(                         /* バッファ初期化                     */
		bufptr,
		DFMR_INITBYTE,
		DFMR_USR_MAXSECTSIZE
		);
	l_direntptr = ( TFMR_DIRENTRY * )bufptr;
	/* 名称領域初期化 */
	FMR_memSet(                         /* 領域をスペースで初期化             */
		&l_direntptr->name[ 0 ],
		DFMR_SPACE,
		DFMR_VNAMESZ
		);
	/* 名称設定 */
	FMR_memCopy(
		&l_direntptr->name[ 0 ],
		volname,
		vollen
		);
	/* 属性設定 */
	l_direntptr->attr = DFMR_ATTR_VOLID;    /* 属性:ボリュームID              */
	/* 時刻設定 */
	FMR_osGetTime(                      /* 作成日時をOSより取得               */
		&( l_direntptr->crtdate ),
		&( l_direntptr->crttime ),
		&( l_direntptr->crttimetenth )
		);
	l_direntptr->lstaccdate = l_direntptr->crtdate;   /* アクセス日は作成日付と同じ */
	l_direntptr->wrtdate = l_direntptr->crtdate;      /* 更新日時は作成日時と同じ   */
	l_direntptr->wrttime = l_direntptr->crttime;
	/* その他 */
	l_direntptr->fstclushi = DFMR_INITBYTE; /* 開始クラスタ(上位)         */
	l_direntptr->fstcluslo = DFMR_INITBYTE; /* 開始クラスタ(下位)         */
	l_direntptr->filesize = DFMR_INITBYTE;  /* サイズ                     */
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
	FMR_setBigEndianEntry(
		l_direntptr
	);
#endif /* DFMR_ENDIAN_BIG */
	/**********************************************************************/
	/* Volume ID エントリ書込                                             */
	/**********************************************************************/
	l_drtn = FMR_writeSector(
		drvinfonum,
		chanel_unit,
		volsectnum,
		bufptr
		);
	return l_drtn;
}

/* 指定R/Wバッファから指定セクタ番号へ１セクタ書込む */

static F_INT FMR_writeSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 書込みセクタ番号                   */
    F_BYTE * buffer                     /* R/Wバッファ                        */
)
{
	TFMR_DEVIOWRITE l_deviowrite;       /* ドライバI/F(Write)I/Oパラメータ    */
	F_INT l_drtn;                       /* ドライバI/F戻り値                  */
	F_WORD l_writesectcnt;              /* 書込みセクタ数                     */

	l_deviowrite.startsctnum = sectnum; /* 書込み開始セクタ番号               */
	l_writesectcnt = 1;
	l_deviowrite.writesctnum = &l_writesectcnt; /* 書込みセクタ数             */
	l_deviowrite.writebuff = buffer;    /* 書込みデータ格納領域               */
	
	/* 書込み対象のセクタを書込む */
	l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
		chanel_unit,                    /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCWRITE,              /* I/O Func.コード：データ書込み      */
		&l_deviowrite                   /* I/Oパラメータ                      */
		);
	if( (l_drtn == DFMR_OK) && (l_writesectcnt != 1) ) 
	{
		return DFMR_FATALERR;
	}
	return l_drtn;
}

/* 指定セクタ番号から指定セクタ分消去する */

static F_INT FMR_eraseSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 消去セクタ番号                     */
    F_WORD   sectcnt                    /* 消去セクタ数                       */
)
{
	TFMR_DEVIOERASE l_devioerase;       /* ドライバI/F(Erase)I/Oパラメータ    */
	F_INT l_drtn;                       /* ドライバI/F戻り値                  */
	F_WORD l_erasesectcnt;              /* 消去セクタ数                       */

	l_devioerase.startsctnum = sectnum; /* 消去開始セクタ番号                 */
	l_erasesectcnt = sectcnt;
	l_devioerase.erasesctnum = &l_erasesectcnt; /* 消去セクタ数               */
	l_devioerase.erasedata = DFMR_INITBYTE;    /* 消去みデータ                */
	
	/* 消去対象のセクタを消去する */
	l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
		chanel_unit,                    /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCERASE,              /* I/O Func.コード：データ消去        */
		&l_devioerase                   /* I/Oパラメータ                      */
		);
	if( (l_drtn == DFMR_OK) && (l_erasesectcnt != sectcnt) ) 
	{
		return DFMR_FATALERR;
	}
	return l_drtn;
}

/* 指定R/Wバッファから指定セクタ番号へ１セクタ読込む */

static F_INT FMR_readSector(
    F_BYTE   drvnum,                    /* 対象ドライブ番号                   */
    F_BYTE   chanel_unit,               /* チャネル番号/ユニット番号          */
    F_DWORD  sectnum,                   /* 読込みセクタ番号                   */
    F_BYTE * buffer                     /* R/Wバッファ                        */
)
{
	TFMR_DEVIOREAD l_devioread;         /* ドライバI/F(Read)I/Oパラメータ     */
	F_INT l_drtn;                       /* ドライバI/F戻り値                  */
	F_WORD l_readsectcnt;               /* 読込みセクタ数                     */

	l_devioread.startsctnum = sectnum;  /* 読込み開始セクタ番号               */
	l_readsectcnt = 1;
	l_devioread.readsctnum = &l_readsectcnt;    /* 読込みセクタ数             */
	l_devioread.readbuff = buffer;      /* 読込みデータ格納領域               */
	
	/* 読込み対象のセクタを読込む */
	l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
		chanel_unit,                    /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCREAD,               /* I/O Func.コード：データ読込み      */
		&l_devioread                    /* I/Oパラメータ                      */
		);
	if( (l_drtn == DFMR_OK) && (l_readsectcnt != 1) ) 
	{
		return DFMR_FATALERR;
	}
	return l_drtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
