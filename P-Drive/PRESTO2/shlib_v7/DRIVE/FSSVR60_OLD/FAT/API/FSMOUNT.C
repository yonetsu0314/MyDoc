/*======================================================================
 * File name    : fsmount.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/api/RCS/fsmount.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsmount.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
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
/* 関 数 名：FMR_Mount                                                        */
/*         ：                                                                 */
/* パラメタ：IN: char 論理ドライブ番号                                        */
/*         ：    char * RAM情報アドレス                                       */
/*         ：    unsigned long RAM領域サイズ                                  */
/*         ：    TFMR_MOUNTINFO * マウント情報領域アドレス                    */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_RAMSIZEERR                                        */
/*         ：          DFMR_DRVNUMERR                                         */
/*         ：          DFMR_DRVCONNECTERR                                     */
/*         ：          DFMR_NODRV                                             */
/*         ：          DFMR_CHANNELERR                                        */
/*         ：                                                                 */
/* 概    要：論理ドライブのマウント                                           */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 RAMテーブルアドレス.R/Wバッファ名変更           04/12/10 Y.Yoshizumi */
/*       R/Wバッファ読込セクタ番号初期化を追加           04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

static F_VOID FMR_setFatSectTable(
    F_BYTE *            fatbuff,        /* FATバッファアドレス                */
    F_BYTE              fattype,        /* FATタイプ                          */
    F_DWORD             clstnum,        /* クラスタ番号                       */
    TFMR_FATSECT_AREA * fatsectarea,    /* FATセクタ管理領域                  */
    F_BYTE              fatsectindex    /* FATセクタ管理テーブルインデックス  */
);

static F_INT FMR_readMirFatSector(
    F_BYTE          drvnum,             /* 対象ドライブ番号                   */
    F_BYTE          chanel_unit,        /* チャネル番号/ユニット番号          */
    F_BYTE *        fatbuff,            /* FATバッファアドレス                */
    F_DWORD         sectnum,            /* セクタ番号                         */
    F_DWORD         sectperfat,         /* FAT1件当りのセクタ数               */
    F_BYTE          fatcnt              /* FAT数                              */
);

static F_VOID FMR_setDWORDdata(
    F_BYTE *        sectimage,          /* ソースバッファアドレス             */
    F_DWORD *       dataarea,           /* 設定先アドレス                     */
    F_WORD          offset              /* ソースバッファオフセット           */
);

int FMR_Mount(
	char drvnum,                        /* 論理ドライブ番号                   */
	char * raminfo,                     /* RAM情報アドレス                    */
	unsigned long ramlen,               /* RAM領域サイズ                      */
	TFMR_MOUNTINFO * mountinfo          /* マウント情報領域アドレス           */
)
{
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	F_DWORD l_cnt02;
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work03;
	
	F_DWORD l_fatsectnum;               /* FATセクタ番号                      */
	F_DWORD l_partsectnum;              /* ユニット全体のセクタ数             */
	F_DWORD l_sectperfat;               /* FAT1件当りのセクタ数               */
	F_DWORD l_fattopsect;               /* FAT Regionの先頭セクタ番号         */
	F_DWORD l_roottopsect;              /* ルートディレクトリの先頭セクタ番号 */
	F_DWORD l_datatopsect;              /* Data Regionの先頭セクタ番号        */
	F_DWORD l_totalclstnum;             /* 総クラスタ数                       */
	F_DWORD l_volumeid;                 /* ボリュームID                       */
	
	F_BYTE * l_rwbuff;                  /* R/Wバッファ                        */
	F_CHAR * l_raminfoptr;              /* RAM領域ポインタ                    */

	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/O                        */
	TFMR_LOGDRV_INFO * l_logdrvinfo;    /* 論理ドライブ構成情報               */
	TFMR_FATBUFF * l_fatbuff;           /* FAT読込み領域                      */
	TFMR_WORK_AREA * l_workarea;        /* ワークエリア                       */
	TFMR_FATSECT_AREA * l_fatsectarea;  /* FATセクタ管理領域                  */
	
	F_INT l_rtn;                        /* 戻り値                             */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	F_INT l_ortn;                       /* OS部戻り値                         */
	
	F_WORD l_fatoffset;                 /* FATオフセット                      */
	F_WORD l_bytepersect;               /* セクタ当りのバイト数               */
	F_WORD l_direntcnt;                 /* ディレクトリエントリ数             */
	F_WORD l_actfatflg;                 /* アクティブFATフラグ                */
	F_WORD l_fsinfosect;                /* FSInfoセクタ番号                   */
	F_WORD l_sectnum;                   /* ドライバI/O用                      */
	
	F_BYTE l_sectimage[ DFMR_USR_MAXSECTSIZE + 1 ];
	                                    /* セクタイメージ                     */
	F_BYTE l_volumename[ DFMR_VNAMESZ ];/* ボリューム名                       */

	F_BYTE l_chnum;                     /* チャネル番号                       */
	F_BYTE l_sectperclst;               /* クラスタ当りのセクタ数             */
	F_BYTE l_fattype;                   /* FATタイプ                          */
	F_BYTE l_fatcnt;                    /* FAT数                              */
	F_BYTE l_fatsectindex;              /* FATセクタ管理テーブルインデックス  */
	F_BYTE l_drvinfonum;                /* ドライバ構成情報番号               */
	F_CHAR l_drvindex;                  /* ドライブインデックス               */
	
	/**************************************************************************/
	/* パラメータチェック                                                     */
	/**************************************************************************/
	l_rtn = DFMR_SUC;
	
	if( ramlen == 0 )
	{
		/* RAM領域サイズが0の場合 */
		return DFMR_RAMSIZEERR;         /* RAM領域サイズエラー                */
	}
	
	if( ( drvnum < DFMR_TOPDRVNUM ) || ( DFMR_ENDDRVNUM < drvnum ) )
	{
		/* 論理ドライブ番号が許容範囲外の場合 */
		return DFMR_DRVNUMERR;          /* 論理ドライブ番号エラー             */
	}
	
	l_drvindex = drvnum;
	l_drvindex -= DFMR_TOPDRVNUM;
	if( tfmr_part_ramaddr[ l_drvindex ].logdrvinfo != ( TFMR_LOGDRV_INFO * )0xFFFFFFFF )
	{
		/* 論理ドライブが接続済みの場合 */
		return DFMR_DRVCONNECTERR;      /* 論理ドライブ接続済みエラー         */
	}
	
	/**************************************************************************/
	/* ドライバ構成情報取得                                                   */
	/**************************************************************************/
	for( l_drvinfonum = 0; l_drvinfonum < DFMR_USR_MAXDRIVER; l_drvinfonum ++ )
	{
		/* ドライバ最大数までドライバ構成情報を検索 */
		if( tfmr_drvinfo[ l_drvinfonum ].drvid  == ( mountinfo->driverid & ~DFMR_MBR_OFF ) )
		{
			break;
		}
	}
	
	if( l_drvinfonum == DFMR_USR_MAXDRIVER )
	{
		/* 指定ドライバIDが存在しない */
		return DFMR_NODRV;              /* 対象ドライバなし                   */
	}
	l_chnum = mountinfo->chanel_unit;
	l_chnum &= 0xF0;
	l_chnum >>= 4;
	if( l_chnum >= tfmr_drvinfo[ l_drvinfonum ].channelnum )
	{
		/* 指定チャネル番号が許容チャネル番号超過 */
		return DFMR_CHANNELERR;         /* チャネル番号エラー                 */
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
	/* ドライバ、マウント                                                     */
	/**************************************************************************/
	if ( (mountinfo->driverid & DFMR_MBR_OFF) == 0 )
	{
		l_drvioparam.mount.mbrmode = DFMR_FLGOFF;
	}
	else {
		l_drvioparam.mount.mbrmode = DFMR_FLGON;
	}
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		mountinfo->chanel_unit,         /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCMOUNT,              /* I/O Func.番号:マウント             */
		&l_drvioparam
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
		goto ERROR1;
	}
	/* パーティションタイプによる判定 */
	if( l_drvioparam.mount.mbrmode == DFMR_FLGOFF )
	{                                   /* MBRありの場合のみ                  */
		switch( l_drvioparam.mount.parttype )
		{
		case DFMR_PRTTBL_TYPEF12:       /* FAT12                              */
		case DFMR_PRTTBL_TYPEF16LOW:    /* FAT16 32MByte未満                  */
		case DFMR_PRTTBL_TYPEF16HI:     /* FAT16 32MByte以上                  */
		case DFMR_PRTTBL_TYPEF16LBA:    /* FAT16(LBA)                         */
		case DFMR_PRTTBL_TYPEF32CHS:    /* FAT32(CHS)                         */
		case DFMR_PRTTBL_TYPEF32LBA:    /* FAT32(LBA)                         */
			break;
		default:
			/******************************************************************/
			/* パーティションタイプが無効の場合                               */
			/******************************************************************/
			l_rtn = DFMR_PARTTYPERR;    /* パーティションタイプエラー         */
			goto ERROR;
		}
	}
	/**************************************************************************/
	/* BPB読込み                                                              */
	/**************************************************************************/
	l_drvioparam.read.startsctnum = 0;
	l_sectnum = 1;
	l_drvioparam.read.readsctnum = &l_sectnum;
	l_drvioparam.read.readbuff = &l_sectimage[ 0 ];
	
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		mountinfo->chanel_unit,
		DFMR_DRVFUNCREAD,
		&l_drvioparam
		);
	if( (l_drtn != DFMR_OK) ||
		(l_sectnum != 1) )
	{                               /* 処理失敗又はセクタ数異常時 */
		if( l_drtn != DFMR_OK )
		{
			l_rtn = l_drtn;
		}
		else
		{
			l_rtn = DFMR_FATALERR;
		}
		goto ERROR;
	}
	/* BPBよりパラメータを取得 */
	l_sectperclst = l_sectimage[ DFMR_BPBOFST_SECPERCLUS ];
	l_fatcnt = l_sectimage[ DFMR_BPBOFST_NUMFATS ];
	l_bytepersect = ( F_WORD )l_sectimage[ DFMR_BPBOFST_BYTEPERSEC + 1 ];
	l_bytepersect <<= 8;
	l_bytepersect |= ( F_WORD )l_sectimage[ DFMR_BPBOFST_BYTEPERSEC ];
	l_direntcnt = ( F_WORD )l_sectimage[ DFMR_BPBOFST_ROOTENTCNT + 1 ];
	l_direntcnt <<= 8;
	l_direntcnt |= ( F_WORD )l_sectimage[ DFMR_BPBOFST_ROOTENTCNT ];
	l_fattopsect = ( F_DWORD )l_sectimage[ DFMR_BPBOFST_RSVDSECCNT + 1 ];
	l_fattopsect <<= 8;
	l_fattopsect |= ( F_DWORD )l_sectimage[ DFMR_BPBOFST_RSVDSECCNT ];
	l_fattopsect &= 0x0000FFFF;
	
	/**************************************************************************/
	/* FATタイプを決定                                                        */
	/**************************************************************************/
	l_work01 = l_direntcnt + ( l_bytepersect / sizeof( TFMR_DIRENTRY ) ) - 1;
	l_work01 /= l_bytepersect / sizeof( TFMR_DIRENTRY );
	
	if( ( l_sectimage[ DFMR_BPBOFST_FATSZ16 ] != 0 )
		|| ( l_sectimage[ DFMR_BPBOFST_FATSZ16 + 1 ] != 0 ) )
	{
		l_sectperfat = ( F_WORD )l_sectimage[ DFMR_BPBOFST_FATSZ16 + 1 ];
		l_sectperfat <<= 8;
		l_sectperfat |= ( F_WORD )l_sectimage[ DFMR_BPBOFST_FATSZ16 ];
	}
	else
	{
		FMR_setDWORDdata(
			&l_sectimage[0],            /* ソースバッファアドレス             */
			&l_sectperfat,              /* 設定先アドレス                     */
			DFMR_BPBOFST32_FATSZ32      /* ソースバッファオフセット           */
			);
	}
	
	if( ( l_sectimage[ DFMR_BPBOFST_TOTSEC16 ] != 0 )
		|| ( l_sectimage[ DFMR_BPBOFST_TOTSEC16 + 1 ] != 0 ) )
	{
		l_partsectnum = ( F_DWORD )l_sectimage[ DFMR_BPBOFST_TOTSEC16 + 1 ];
		l_partsectnum <<= 8;
		l_partsectnum |= ( F_DWORD )l_sectimage[ DFMR_BPBOFST_TOTSEC16 ];
	}
	else
	{
		FMR_setDWORDdata(
			&l_sectimage[0],            /* ソースバッファアドレス             */
			&l_partsectnum,             /* 設定先アドレス                     */
			DFMR_BPBOFST_TOTSEC32       /* ソースバッファオフセット           */
			);
	}
	
	l_totalclstnum = l_partsectnum - ( l_fattopsect + ( l_fatcnt * l_sectperfat ) + l_work01 );
	l_totalclstnum /= l_sectperclst;
	
	if( l_totalclstnum < 4085 )
	{
		l_fattype = DFMR_FATTYPE12;
	}
	else if( l_totalclstnum < 65525)
	{
		l_fattype = DFMR_FATTYPE16;
	}
	else
	{
		l_fattype = DFMR_FATTYPE32;
	}
	
	/* FAT(パーティション)タイプに依存するパラメータを取得 */
	if( l_fattype == DFMR_FATTYPE32 )
	{
		l_actfatflg = ( F_WORD )l_sectimage[ DFMR_BPBOFST32_EXTFLGS + 1 ];
		l_actfatflg <<= 8;
		l_actfatflg |= ( F_WORD )l_sectimage[ DFMR_BPBOFST32_EXTFLGS ];
		
		l_datatopsect = l_fattopsect;
		l_datatopsect += l_fatcnt * l_sectperfat;

		if( ( l_actfatflg & 0x0080 ) == 0x0080 )
		{
			l_fattopsect += l_sectperfat * l_actfatflg & 0x000F;
			l_fatcnt = 1;
		}
		else
		{
			l_actfatflg = 0;
		}
		l_fsinfosect = ( F_WORD )l_sectimage[ DFMR_BPBOFST32_FSINFO + 1 ];
		l_fsinfosect <<= 8;
		l_fsinfosect |= ( F_WORD )l_sectimage[ DFMR_BPBOFST32_FSINFO ];

		FMR_setDWORDdata(
			&l_sectimage[0],            /* ソースバッファアドレス             */
			&l_roottopsect,             /* 設定先アドレス                     */
			DFMR_BPBOFST32_ROOTCLUS     /* ソースバッファオフセット           */
			);
		l_roottopsect -= 2;
		l_roottopsect *= l_sectperclst;
		l_roottopsect += l_datatopsect;
		
		FMR_setDWORDdata(
			&l_sectimage[0],            /* ソースバッファアドレス             */
			&l_volumeid,                /* 設定先アドレス                     */
			DFMR_BSOFST32_VOLID         /* ソースバッファオフセット           */
			);
		FMR_memCopy(
			&l_volumename[ 0 ],
			&l_sectimage[ DFMR_BSOFST32_VOLLAB ],
			DFMR_VNAMESZ
			);
	}
	else
	{
		l_actfatflg = 0;
		l_fsinfosect = 0;
		
		l_roottopsect = l_fattopsect;
		l_roottopsect += l_fatcnt * l_sectperfat;
		
		l_datatopsect = l_roottopsect;
		l_datatopsect +=(((l_direntcnt * sizeof(TFMR_DIRENTRY))+l_bytepersect - 1) / l_bytepersect);
		
		FMR_setDWORDdata(
			&l_sectimage[0],            /* ソースバッファアドレス             */
			&l_volumeid,                /* 設定先アドレス                     */
			DFMR_BSOFST_VOLID           /* ソースバッファオフセット           */
			);
		FMR_memCopy(
			&l_volumename[ 0 ],
			&l_sectimage[ DFMR_BSOFST_VOLLAB ],
			DFMR_VNAMESZ
			);
	}
	/**************************************************************************/
	/* RAM領域サイズチェック                                                  */
	/**************************************************************************/
	l_work01 = sizeof( TFMR_LOGDRV_INFO );  /* ｢論理ドライブ構成情報｣サイズ   */
	l_work01 += sizeof( TFMR_FATBUFF ) * DFMR_RWFATNUM;
	                                        /* ｢FAT読込み領域｣サイズ          */
	l_work01 += sizeof( TFMR_WORK_AREA );   /* ｢ワークエリア｣サイズ           */
	if( l_fattype == DFMR_FATTYPE32 )       /* ｢FATセクタ管理領域｣サイズ      */
	{
		l_work01 += sizeof( TFMR_FATSECT_AREA );
	}
	else
	{
		l_work01 += sizeof( F_DWORD );
		l_work01 += sizeof( SECTADMINTBL ) * l_sectperfat;
	}
	l_work01 += l_bytepersect * DFMR_RWBUFFSIZE;/* ｢R/Wバッファ｣サイズ        */
	
	if( l_work01 > ramlen )             /* RAM領域サイズが足りない場合        */
	{
		/**********************************************************************/
		/* RAM領域サイズが必要サイズを満たしていない場合                      */
		/**********************************************************************/
		l_rtn = DFMR_RAMSIZEERR;        /* RAM領域サイズエラー                */
		goto ERROR;
	}
	
	/**************************************************************************/
	/* 論理ドライブRAMアドレス割当て(ローカル)                                */
	/**************************************************************************/
	l_raminfoptr = raminfo;
	l_logdrvinfo = ( TFMR_LOGDRV_INFO * )l_raminfoptr;
	l_raminfoptr += sizeof( TFMR_LOGDRV_INFO );
	l_fatbuff = ( TFMR_FATBUFF * )l_raminfoptr;
	l_raminfoptr += sizeof( TFMR_FATBUFF ) * DFMR_RWFATNUM;
	l_workarea = ( TFMR_WORK_AREA * )l_raminfoptr;
	l_raminfoptr += sizeof( TFMR_WORK_AREA );
	l_fatsectarea = ( TFMR_FATSECT_AREA * )l_raminfoptr;
	if( l_fattype == DFMR_FATTYPE32 )
	{
		l_raminfoptr += sizeof( TFMR_FATSECT_AREA );
	}
	else
	{
		l_raminfoptr += sizeof( F_DWORD );
		l_raminfoptr += sizeof( SECTADMINTBL ) * l_sectperfat;
	}
	/**************************************************************************/
	/* 論理ドライブ構成情報設定                                               */
	/**************************************************************************/
	l_logdrvinfo->drvnum = drvnum;              /* 論理ドライブ番号           */
	l_logdrvinfo->drvid = mountinfo->driverid;  /* ドライバID                 */
	l_logdrvinfo->channelunitnum = mountinfo->chanel_unit;
	                                            /* チャネル番号/ユニット番号  */
	l_logdrvinfo->sectperclst = l_sectperclst;  /* クラスタ当りのセクタ数     */
	l_logdrvinfo->fattype = l_fattype;          /* FATタイプ                  */
	l_logdrvinfo->fatcnt = l_fatcnt;            /* FAT数                      */
	l_logdrvinfo->bytepersect = l_bytepersect;  /* セクタ当りのバイト数       */
	l_logdrvinfo->direntcnt = l_direntcnt;      /* ディレクトリエントリ数     */
	l_logdrvinfo->semid = tfmr_drvinfo[ l_drvinfonum ].semaphoreid[ l_chnum ];
	                                            /* セマフォID                 */
	l_logdrvinfo->actfatflg = l_actfatflg;      /* アクティブFATフラグ        */
	l_logdrvinfo->fsinfosect = l_fsinfosect;    /* FSInfoセクタ番号           */
	l_logdrvinfo->drventry = tfmr_drvinfo[ l_drvinfonum ].drventry;
                                                /* ドライバエントリアドレス   */
	l_logdrvinfo->partsectnum = l_partsectnum;  /* ユニット全体のセクタ数     */
	l_logdrvinfo->sectperfat = l_sectperfat;    /* FAT1件当りのセクタ数       */
	l_logdrvinfo->fattopsect = l_fattopsect;    /* FAT Regionの先頭セクタ番号 */
	l_logdrvinfo->roottopsect = l_roottopsect;  /* ルートディレクトリの       */
	                                            /* 先頭セクタ番号             */
	l_logdrvinfo->datatopsect = l_datatopsect;  /* Data Regionの先頭セクタ番号*/
	l_logdrvinfo->totalclstnum = l_totalclstnum;/* 総クラスタ数               */
	l_logdrvinfo->volumeid = l_volumeid;        /* ボリュームID               */
	FMR_memCopy(                                /* ボリューム名               */
		&l_logdrvinfo->volumename[ 0 ],
		&l_volumename[ 0 ],
		DFMR_VNAMESZ
		);
	/**************************************************************************/
	/* FATセクタ読込み                                                        */
	/**************************************************************************/
	if( l_fattype == DFMR_FATTYPE12 )
	{
		l_rwbuff = &l_fatbuff->fat[ 4 ];
	}
	else
	{
		l_rwbuff = &l_fatbuff->fat[ 0 ];
	}
	l_drtn = FMR_readMirFatSector(
		l_drvinfonum,                   /* 対象ドライブ番号                   */
		mountinfo->chanel_unit,         /* チャネル番号/ユニット番号          */
		l_rwbuff,                       /* FATバッファアドレス                */
		l_fattopsect,                   /* セクタ番号                         */
		l_sectperfat,                   /* FAT1件当りのセクタ数               */
		l_fatcnt                        /* FAT数                              */
		);
	if( l_drtn != DFMR_OK )
	{
		goto ERROR;
	}
	/**************************************************************************/
	/* FATセクタ管理テーブル[0]初期化                                         */
	/**************************************************************************/
	l_fatsectnum = l_fattopsect;        /* FATセクタ番号←FAT先頭セクタ       */
	l_fatsectarea->startfatsect = l_fattopsect;
	                                    /* 開始FATセクタ番号=FAT先頭セクタ    */
	l_fatsectindex = 0;                 /* インデックス = 0                   */
	l_fatsectarea->sectadmintbl[ l_fatsectindex ].freeclstnum = 0;
	                                    /* 空きクラスタ数 = 0                 */
	l_fatsectarea->sectadmintbl[ l_fatsectindex ].leastfreeclst = DFMR_RAMINIT;
	                                    /* 空きクラスタ番号 = 0xFFFFFFFF      */
	
	/**************************************************************************/
	/* FATオフセット設定                                                      */
	/**************************************************************************/
	switch( l_fattype )
	{
	case DFMR_FATTYPE12:
		l_fatoffset = 5;				/* クラスタ№1のバッファ内位置(1+4)   */
		break;
	case DFMR_FATTYPE16:
		l_fatoffset = 2;
		break;
	default: /* case DFMR_FATTYPE32 */
		l_fatoffset = 4;
		l_work01 = l_bytepersect >> 2;	/* byte/secter÷4			*/
		l_work01 *=  DFMR_FATMNGSIZE;	/* FAT管理領域数×(byte/secter÷4)	*/
		l_work01 -=  2;
		if ( l_totalclstnum > l_work01 )
		{
			l_totalclstnum = l_work01;
		}
		break;
	}
	for( l_cnt01 = 2; l_cnt01 < l_totalclstnum + 2; l_cnt01 ++ )
	{
		/* FATタイプによる分岐 */
		if( l_fattype == DFMR_FATTYPE12 )
		{                               /* FAT12の場合                        */
			/* FATオフセット更新 */
			if( ( l_cnt01 & 1 ) == 1 )
			{
				l_fatoffset ++;
			}
			else
			{
				l_fatoffset += 2;
			}
			
			if( l_fatoffset == ( F_WORD )( l_bytepersect + 3 ) )
			{                           /* FATオフセットがセクタ当りの        */
			                            /* バイト数+3の場合                   */
				/* FAT読込みバッファ[3]←FAT読込みバッファ[FATオフセット] */
				l_fatbuff->fat[ 3 ] = l_fatbuff->fat[ l_fatoffset ];
				
				/* FATオフセット←3 */
				l_fatoffset = 3;
			}
			else
			{
				if( l_fatoffset == ( F_WORD )( l_bytepersect + 4 ) )
				{                       /* FATオフセットがセクタ当りの        */
				                        /* バイト数+4の場合                   */
					l_fatoffset = 4;
				}
				else
				{
					/* FATセクタ管理テーブルを設定する */
					FMR_setFatSectTable(
						&l_fatbuff->fat[ l_fatoffset ], /* FATバッファアドレス*/
						l_fattype,                      /* FATタイプ          */
						l_cnt01,                        /* クラスタ番号       */
						l_fatsectarea,                  /* FATセクタ管理領域  */
						l_fatsectindex  /* FATセクタ管理テーブルインデックス  */
						);
					continue;
				}
			}
		}
		else
		{                               /* FAT16,FAT32の場合                  */
			if( l_fattype == DFMR_FATTYPE16 )
			{                           /* FAT16の場合                        */
				l_fatoffset += 2;
			}
			else {                      /* FAT32の場合                        */
				l_fatoffset += 4;
			}
			if( l_fatoffset == l_bytepersect )  /* FATオフセットがセクタ      */
			{                                   /* 当りのバイト数の場合       */
				l_fatoffset = 0;
			}
			else
			{
				/* FATセクタ管理テーブルを設定する */
				FMR_setFatSectTable(
					&l_fatbuff->fat[ l_fatoffset ], /* FATバッファアドレス    */
					l_fattype,                      /* FATタイプ              */
					l_cnt01,                        /* クラスタ番号           */
					l_fatsectarea,                  /* FATセクタ管理領域      */
					l_fatsectindex  /* FATセクタ管理テーブルインデックス      */
					);
				continue;
			}
		}
		/**********************************************************************/
		/* FATセクタ番号 +1                                                   */
		/**********************************************************************/
		l_fatsectnum ++;
		
		/**********************************************************************/
		/* FATセクタ読込み                                                    */
		/**********************************************************************/
		l_drtn = FMR_readMirFatSector(
			l_drvinfonum,                   /* 対象ドライブ番号                   */
			mountinfo->chanel_unit,         /* チャネル番号/ユニット番号          */
			l_rwbuff,                       /* FATバッファアドレス                */
			l_fatsectnum,                   /* セクタ番号                         */
			l_sectperfat,                   /* FAT1件当りのセクタ数               */
			l_fatcnt                        /* FAT数                              */
			);
		if( l_drtn != DFMR_OK )
		{
			goto ERROR;
		}
		/**********************************************************************/
		/* 次のFATセクタ管理テーブル初期化                                    */
		/**********************************************************************/
		l_fatsectindex = ( F_BYTE )( l_fatsectnum - l_fattopsect );
		                                /* インデックス = FATセクタ番号       */
		                                /* - FAT Region先頭セクタ番号         */
		l_fatsectarea->sectadmintbl[ l_fatsectindex ].freeclstnum = 0;
		                                /* 空きクラスタ数 = 0                 */
		l_fatsectarea->sectadmintbl[ l_fatsectindex ].leastfreeclst = DFMR_RAMINIT;
		/* FATセクタ管理テーブルを設定する */
		FMR_setFatSectTable(
			&l_fatbuff->fat[ l_fatoffset ], /* FATバッファアドレス    */
			l_fattype,                      /* FATタイプ              */
			l_cnt01,                        /* クラスタ番号           */
			l_fatsectarea,                  /* FATセクタ管理領域      */
			l_fatsectindex  /* FATセクタ管理テーブルインデックス      */
			);
	}
	/**************************************************************************/
	/* ミラーリングFATセクタのアクセス確認                                    */
	/**************************************************************************/
	if( l_fatcnt >= 2 )
	{                                   /* ミラーリングFATが存在する場合      */
		l_work03 = l_sectperfat;
		if( l_work03 > DFMR_FATMNGSIZE )
		{                               /* FATセクタが256以上(FAT32の場合)    */
			l_work03 = DFMR_FATMNGSIZE;
		}
		for( l_cnt01 = 0; l_cnt01 < l_work03; l_cnt01 ++ )
		{
			for( l_cnt02 = 1; l_cnt02 < l_fatcnt; l_cnt02 ++ )
			{
				/******************************************************************/
				/* 対象FATセクタ読込み                                            */
				/******************************************************************/
				l_drvioparam.read.startsctnum = l_fattopsect + l_sectperfat * l_cnt02 + l_cnt01;
				l_sectnum = 1;
				l_drvioparam.read.readsctnum = &l_sectnum;
				l_drvioparam.read.readbuff = &l_fatbuff->fat[ 0 ];
				l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
					mountinfo->chanel_unit,
					DFMR_DRVFUNCREAD,
					&l_drvioparam
					);
				if( (l_drtn != DFMR_OK) ||
					(l_sectnum != 1) )
				{                               /* 処理失敗又はセクタ数異常時 */
					l_drvioparam.read.startsctnum = l_fattopsect + l_cnt01;
					l_sectnum = 1;
					l_drvioparam.read.readsctnum = &l_sectnum;
					l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
						mountinfo->chanel_unit,
						DFMR_DRVFUNCREAD,
						&l_drvioparam
						);
					if( (l_drtn != DFMR_OK) ||
						(l_sectnum != 1) )
					{                               /* 処理失敗又はセクタ数異常時 */
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
						}
						else
						{
							l_rtn = DFMR_FATALERR;
						}
						goto ERROR;
					}
					/* 読込んだFATセクタを元のセクタに書込む */
					l_drvioparam.write.startsctnum = l_fattopsect + l_sectperfat * l_cnt02 + l_cnt01;
					l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
						mountinfo->chanel_unit,
						DFMR_DRVFUNCWRITE,
						&l_drvioparam
						);
					/* 書込み結果に関わらず、次対象FATセクタ読込みを行う */
				}
			}
		}
	}
	/**************************************************************************/
	/* 論理ドライブRAMテーブルアドレス設定                                    */
	/**************************************************************************/
	tfmr_part_ramaddr[ l_drvindex ].logdrvinfo = l_logdrvinfo;
	                                    /* 論理ドライブ構成情報               */
	tfmr_part_ramaddr[ l_drvindex ].fatbuff = l_fatbuff;
	                                    /* FAT読込み領域                      */
	tfmr_part_ramaddr[ l_drvindex ].workarea = l_workarea;
	                                    /* ワークエリア                       */
	tfmr_part_ramaddr[ l_drvindex ].fatsectarea = l_fatsectarea;
	                                    /* FATセクタ管理領域                  */
	tfmr_part_ramaddr[ l_drvindex ].rwbuffarea = ( TFMR_RWBUFF * )l_raminfoptr;
	                                    /* R/Wバッファ                        */
	/**************************************************************************/
	/* FAT読込み領域.FATセクタ番号設定                                        */
	/**************************************************************************/
	tfmr_part_ramaddr[ l_drvindex ].fatbuff -> fatsectnum = 0;
	( tfmr_part_ramaddr[ l_drvindex ].fatbuff + 1 ) -> fatsectnum = 0;
	
	/**************************************************************************/
	/* R/Wバッファ読込セクタ番号初期化                                        */
	/**************************************************************************/
	tfmr_part_ramaddr[ l_drvindex ].rwbuffarea -> rwsectnum = 0xffffffff;
	( tfmr_part_ramaddr[ l_drvindex ].rwbuffarea + 1 ) -> rwsectnum = 0xffffffff;
	/**************************************************************************/
	/* セマフォ資源返却                                                       */
	/**************************************************************************/
	l_ortn = FMR_osSigSem(
		tfmr_drvinfo[ l_drvinfonum ].semaphoreid[ l_chnum ]
		);
	if( l_ortn != DFMR_OK )
	{
		l_rtn = l_ortn;
	}
	/**************************************************************************/
	/* 正常終了返却                                                           */
	/**************************************************************************/
	return l_rtn;

ERROR:
	/**************************************************************************/
	/* 異常終了処理                                                           */
	/**************************************************************************/
	/* アンマウント */
	l_drtn = ( tfmr_drvinfo[ l_drvinfonum ].drventry )(
		mountinfo->chanel_unit,         /* チャネル番号/ユニット番号          */
		DFMR_DRVFUNCUNMOUNT,            /* I/O Func.番号:アンマウント         */
		&l_drvioparam
		);
	if( l_drtn != DFMR_OK )
	{
		l_rtn = l_drtn;
	}
ERROR1:
	/* セマフォ資源返却 */
	l_ortn = FMR_osSigSem(
		tfmr_drvinfo[ l_drvinfonum ].semaphoreid[ l_chnum ]
		);
	if( l_ortn != DFMR_OK )
	{
		l_rtn = l_ortn;
	}
	return l_rtn;

}

/* FATセクタ管理テーブルを設定する */

static F_VOID FMR_setFatSectTable(
    F_BYTE *            fatbuff,        /* FATバッファアドレス                */
    F_BYTE              fattype,        /* FATタイプ                          */
    F_DWORD             clstnum,        /* クラスタ番号                       */
    TFMR_FATSECT_AREA * fatsectarea,    /* FATセクタ管理領域                  */
    F_BYTE              fatsectindex    /* FATセクタ管理テーブルインデックス  */
)
{
	F_DWORD l_work01;                   /* ワークエリア                       */

	/* FATエントリ値取出 */
	l_work01 = FMR_getFatValue(
		fatbuff,
		fattype,
		clstnum
		);
	if( l_work01 == DFMR_NEWCLUS )
	{
		/* FATエントリが｢未使用｣の場合 */
		fatsectarea->sectadmintbl[ fatsectindex ].freeclstnum ++;
		                                /* 空きクラスタ数更新                 */
		if( fatsectarea->sectadmintbl[ fatsectindex ].leastfreeclst == DFMR_RAMINIT )
		{
			/* 空きクラスタ番号が未取得状態の場合 */
			fatsectarea->sectadmintbl[ fatsectindex ].leastfreeclst = clstnum;
			                            /* 対象のクラスタ番号を               */
			                            /* 空きクラスタ番号に設定             */
		}
	}
}

/* FATセクタ読込＆ミラーリングによる復旧 */

static F_INT FMR_readMirFatSector(
    F_BYTE          drvnum,             /* 対象ドライブ番号                   */
    F_BYTE          chanel_unit,        /* チャネル番号/ユニット番号          */
    F_BYTE *        fatbuff,            /* FATバッファアドレス                */
    F_DWORD         sectnum,            /* セクタ番号                         */
    F_DWORD         sectperfat,         /* FAT1件当りのセクタ数               */
    F_BYTE          fatcnt              /* FAT数                              */
)
{
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバI/O                        */
	F_INT           l_rtn;              /* 戻り値                             */
	F_INT           l_drtn;             /* ドライバエントリ戻り値             */
	F_WORD          l_sectcnt;          /* ドライバI/O用                      */
	F_BYTE          l_cnt01;            /* ループカウンタ                     */

	/**************************************************************************/
	/* FATセクタ読込み                                                        */
	/**************************************************************************/
	l_drvioparam.read.readbuff = fatbuff;
	l_drvioparam.read.startsctnum = sectnum;
	l_sectcnt = 1;
	l_drvioparam.read.readsctnum = &l_sectcnt;
	l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
		chanel_unit,
		DFMR_DRVFUNCREAD,
		&l_drvioparam
		);
	
	if( (l_drtn != DFMR_OK) ||
		(l_sectcnt != 1) )
	{
		/**********************************************************************/
		/* 処理失敗又はセクタ数異常時                                         */
		/**********************************************************************/
		if( fatcnt < 2 )
		{
			/******************************************************************/
			/* ミラーリングFATセクタがない場合                                */
			/******************************************************************/
			return DFMR_FATERR;         /* FATエラー                          */
		}
		/* ミラーリングFATセクタを読込む */
		for( l_cnt01 = 1; l_cnt01 < fatcnt; l_cnt01 ++ )
		{
			l_drvioparam.read.startsctnum = sectnum + sectperfat * l_cnt01;
			l_sectcnt = 1;
			l_drvioparam.read.readsctnum = &l_sectcnt;
			l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
				chanel_unit,
				DFMR_DRVFUNCREAD,
				&l_drvioparam
				);
			if( (l_drtn != DFMR_OK) ||
				(l_sectcnt != 1) )
			{
				/* 処理失敗又はセクタ数異常時 */
				continue;
			}
			/* 読込んだFATセクタを元のセクタに書込む */
			l_drvioparam.write.startsctnum = sectnum;
			l_drtn = ( tfmr_drvinfo[ drvnum ].drventry )(
				chanel_unit,
				DFMR_DRVFUNCWRITE,
				&l_drvioparam
				);
			if( (l_drtn != DFMR_OK) ||
				(l_sectcnt != 1) )
			{
				/* 処理失敗又はセクタ数異常時 */
				continue;
			}
			break;
		}
		if ( l_cnt01 >= fatcnt )
		{
			return DFMR_FATERR;         /* FATエラー                          */
		}
	}
	return DFMR_OK;
}

/* ソースバッファより設定先へDWORD型データ設定 */

static F_VOID FMR_setDWORDdata(
    F_BYTE *        sectimage,          /* ソースバッファアドレス             */
    F_DWORD *       dataarea,           /* 設定先アドレス                     */
    F_WORD          offset              /* ソースバッファオフセット           */
)
{
	*( dataarea ) = ( F_DWORD )(*( sectimage + offset + 3 ));
	*( dataarea ) <<= 8;
	*( dataarea ) |= ( F_DWORD )(*( sectimage + offset + 2 ));
	*( dataarea ) <<= 8;
	*( dataarea ) |= ( F_DWORD )(*( sectimage + offset + 1 ));
	*( dataarea ) <<= 8;
	*( dataarea ) |= ( F_DWORD )(*( sectimage + offset ));
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
