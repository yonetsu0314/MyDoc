/*======================================================================
 * File name    : ata_io.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/ata_io.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ata_io.c $
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
/****************************************************************************/
/* ファイル名：PCATA_io.c                                                   */
/* 概    要  ：PCカードATAドライバ                                          */
/*           ：I/O 制御部                                                   */
/* 作 成 日  ：2004.03.19                                                   */
/* 作 成 者  ：                                                             */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
/*  01.00 First Release									2004.03.19			*/
/****************************************************************************/

#include "ata_def.h"
#include "ata_srct.h"
#include "ata_prot.h"
#include "drive\atadrv.h"
#include "drive\fs_ata.h"
#include "biosfile.h"
#include "powfail.h"

/****************************************************************************/
/* 関 数 名：PCATA_ReadIDTFY                                                */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：PCカードよりIdentifyDrive情報読込み                            */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_ReadIDTFY(F_BYTE		chanelno,		/* チャネル番号				*/
						F_BYTE		deviceno )		/* デバイス番号				*/
{
	F_WORD		cnt;		/* ループカウンタ	*/
	F_WORD		tmp,tmp1;
	F_DWORD		tmpd;

	F_BYTE		cyl_lo;	/* ATA コマンド用 シリンダ番号下位 */
	F_BYTE		cyl_hi;	/* ATA コマンド用 シリンダ番号上位 */
	F_WORD		cyl;	/* ATA コマンド用 シリンダ番号 */
	F_BYTE		secno;	/* ATA コマンド用 セクタ番号 */
	F_BYTE		head;	/* ATA コマンド用 ヘッド番号 */

	if(DRV_OK != ide_busy()) return ERR_PCREADYTMO;

/* ATAコマンド:Identify Drive 発行					*/

	secno  = 0;
	cyl_lo = 0;
	cyl_hi = 0;
	cyl    = 0;
	head   = 0;

#ifdef USE_INT
	ide_int(0); 	 /* Card Ready interrupt ON */
	vcan_wup();
#endif

	if(DRV_OK != ide_cmd(1, 0, cyl, head, secno, 0, IDE_IDTFY_DRIVE, 0)) {
		return ERR_PCREADYTMO;
	}

#ifndef USE_INT
	if( DRV_OK != ide_busy() ) {
		return ERR_PCREADYTMO;
	}
#endif

#ifndef USE_INT
	if(DRV_OK != ide_check(CHK_IDE_SECT)) {
		return ERR_PCREADYTMO;
	}
#else
	fsysid = vget_tid();
	slp_tsk();
	data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif

/* Identify Drive情報の読込み */

	for( cnt = 0; cnt < TRANS_COUNT; cnt++ ) {		/* 256WORD読込み迄ループ		*/
#ifdef BIG_ENDIAN                  /* Big Endian                         */
		*(((F_BYTE*)&tmp)+1) = PCCard_io_inpb(ATA_DATA_REG);
		*(F_BYTE*)&tmp = PCCard_io_inpb(ATA_DATA_REG);
#else
		*(F_BYTE*)&tmp = PCCard_io_inpb(ATA_DATA_REG);
		*(((F_BYTE*)&tmp)+1) = PCCard_io_inpb(ATA_DATA_REG);
#endif
		switch (cnt) {
			case ATAID_CYLINDER:					/* Default number of cylinders	*/
				PCATA_dv_inf[deviceno].cylinder_cnt = tmp;
				break;
			case ATAID_HEAD:						/* Default number of heads		*/
				PCATA_dv_inf[deviceno].head_cnt = tmp;
				break;
			case ATAID_SECPTR:						/* Default number of sectors per track	*/
				PCATA_dv_inf[deviceno].sec_track = tmp;
				break;
			case ATAID_SECTOR1:						/* number of sectors(LOW)		*/
				tmp1 = tmp;
				break;
			case ATAID_SECTOR2:						/* number of sectors(HIGH)		*/
				tmpd = (F_DWORD)tmp;
				tmpd <<= 16;
				tmpd |= (F_DWORD)tmp1;
				PCATA_dv_inf[deviceno].total_sec = tmpd;
				break;
		}
	}
	
	if(DRV_OK != ide_busy()) {
		return ERR_PCREADYTMO;
	}
	
#ifdef USE_INT
	ide_int(1); 	 /* Card Ready interrupt OFF */
#endif
	
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_ReadPTable                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     F_BYTE *      unit_cnt      ユニット数                    */
/*         ：     T_MBR *       readbuffer    読込バッファ                  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：MBR.PartitionTable読込み                                       */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_ReadPTable(
			F_BYTE		chanelno,		/* チャネル番号				*/
			F_BYTE		deviceno,		/* デバイス番号				*/
			F_BYTE *	unit_cnt,		/* ユニット数				*/
			T_MBR *		readbuffer		/* 読込バッファ				*/
)
{
	F_INT		ret;			/* サブモジュール戻り値				*/
	F_DWORD		extend_start;	/* 拡張領域開始セクタ番号			*/
	F_BYTE		cnt;			/* ループカウンタ					*/
	F_CHAR		cnt1;			/* ループカウンタ					*/
	F_BYTE		unitno;			/* ユニット番号						*/

	PCATA_ch_inf[chanelno].unit_cnt = 0;				/* 接続ユニット数クリア		*/
	PCATA_dv_inf[deviceno].unit_cnt = 0;
	unitno = 0;

/* MBR(MasterBootRecord) 読込					*/

	ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)readbuffer, 0);
	if ( ret != DRV_OK ) {
		return ( ret );
	}
	if ( *(F_WORD *)(readbuffer->signature) != MBR_SIGNATURE ) {
		return ( ERR_PCCARD );
	}

/* パーティションテーブル読込 */

	for( cnt = 0; cnt < 4; cnt++ ) {
		if ( ( readbuffer->part_table[ cnt ].part_type != MBR_PT_NONE )	&&
			 ( readbuffer->part_table[ cnt ].part_type != MBR_PT_EXTEND1 )	&&
			 ( readbuffer->part_table[ cnt ].part_type != MBR_PT_EXTEND2 ) ) {
			PCATA_un_inf[unitno].cnct_chanel = chanelno;
			PCATA_un_inf[unitno].cnct_device = deviceno;
			PCATA_un_inf[unitno].part_type = readbuffer->part_table[ cnt ].part_type;
			PCATA_un_inf[unitno].sec_size = (SEC_SIZE >> 9);
			PCATA_un_inf[unitno].total_sec_cnt = 0;
			PCATA_un_inf[unitno].start_sec_num = 0;
			for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
				PCATA_un_inf[unitno].total_sec_cnt =
					(PCATA_un_inf[unitno].total_sec_cnt << 8) +
					 readbuffer->part_table[ cnt ].sct_size[cnt1];
			}
			for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
				PCATA_un_inf[unitno].start_sec_num =
					(PCATA_un_inf[unitno].start_sec_num << 8) +
					 readbuffer->part_table[ cnt ].top_lba[cnt1];
			}
			unitno++;
		}
	}
	if ( unitno == 0 ) {
		return ERR_PCCARD;			/* 基本領域が無ければエラー	*/
	}
	*unit_cnt = unitno;

	for( cnt = 0; cnt < 4; cnt++ ) {
		if ( ( readbuffer->part_table[ cnt ].part_type == MBR_PT_EXTEND1 ) ||
			 ( readbuffer->part_table[ cnt ].part_type == MBR_PT_EXTEND2 ) ) {
			/* 拡張パーティションの場合		*/
			extend_start = 0;
			for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
				extend_start = (extend_start << 8) + readbuffer->part_table[ cnt ].top_lba[cnt1];
			}
			ret = PCATA_ReadExPTbl(chanelno, deviceno, unit_cnt, readbuffer, extend_start, 0);
			if ( ret != DRV_OK ) {
				ret = DRV_OK;	/* 拡張パーティションの読込エラーでも動作させる	*/
			}
		}
	}
	return ( ret );
}

/****************************************************************************/
/* 関 数 名：PCATA_ReadExPTbl                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     F_BYTE *      unit_cnt      ユニット数                    */
/*         ：     T_MBR *       readbuffer    読込バッファ                  */
/*         ：     F_DWORD       extend_start  拡張領域開始セクタ番号        */
/*         ：     F_DWORD       part_start    パーティションテーブル.開始セクタ番号*/
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：パーティションテーブル読込み                                   */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_ReadExPTbl(
			F_BYTE		chanelno,		/* チャネル番号				*/
			F_BYTE		deviceno,		/* デバイス番号				*/
			F_BYTE *	unit_cnt,		/* ユニット数				*/
			T_MBR *		readbuffer,		/* 読込バッファ				*/
			F_DWORD 	extend_start,	/* 拡張領域開始セクタ番号	*/
			F_DWORD 	part_start		/* パーティションテーブル.開始セクタ番号*/
)
{
	F_INT		ret;			/* サブモジュール戻り値				*/
	F_DWORD		start_sec;	/* パーティション開始セクタ		*/
	F_BYTE		cnt;			/* ループカウンタ					*/
	F_CHAR		cnt1;			/* ループカウンタ					*/
	F_BYTE		unitno;			/* ユニット番号						*/

	unitno = *unit_cnt;
	start_sec = extend_start + part_start;

/* EBR(ExtendBootRecord 読込					*/

	ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)readbuffer, start_sec);
	if ( ret != DRV_OK ) {
		return ( ret );
	}
	if ( *(F_WORD *)(readbuffer->signature) != MBR_SIGNATURE ) {
		return ( ERR_PCCARD );
	}

/* パーティションテーブル読込 */

	if ( ( readbuffer->part_table[0].part_type != MBR_PT_EXTEND1 )	&&
		 ( readbuffer->part_table[0].part_type != MBR_PT_EXTEND2 ) ) {
		PCATA_un_inf[unitno].cnct_chanel = chanelno;
		PCATA_un_inf[unitno].cnct_device = deviceno;
		PCATA_un_inf[unitno].part_type = readbuffer->part_table[0].part_type;
		PCATA_un_inf[unitno].sec_size = (SEC_SIZE >> 9);
		PCATA_un_inf[unitno].total_sec_cnt = 0;
		PCATA_un_inf[unitno].start_sec_num = 0;
		for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
			PCATA_un_inf[unitno].total_sec_cnt =
				(PCATA_un_inf[unitno].total_sec_cnt << 8) +
				readbuffer->part_table[0].sct_size[cnt1];
		}
		for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
			PCATA_un_inf[unitno].start_sec_num =
				(PCATA_un_inf[unitno].start_sec_num << 8) +
				readbuffer->part_table[0].top_lba[cnt1];
		}
		PCATA_un_inf[unitno].start_sec_num += start_sec;
		unitno++;
		*unit_cnt = unitno;
	}
	if ( ( readbuffer->part_table[1].part_type == MBR_PT_EXTEND1 ) ||
		 ( readbuffer->part_table[1].part_type == MBR_PT_EXTEND2 ) ) {
		/* 拡張パーティションの場合		*/
		start_sec = 0;
		for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
			start_sec = (start_sec << 8) + readbuffer->part_table[1].top_lba[cnt1];
		}
		ret = PCATA_ReadExPTbl(chanelno, deviceno, unit_cnt, readbuffer, extend_start, start_sec);
	}
	return ( ret );
}

/****************************************************************************/
/* 関 数 名：PCATA_ReadBR                                                   */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     F_WORD *      readbuffer    読込バッファアドレス          */
/*         ：     F_DWORD       start_sec     パーティション開始セクタ番号  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：MBR(MasterBootRecord)又はEBR(ExtendBootRecord)読込み           */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_ReadBR(
	F_BYTE			chanelno,			/* チャネル番号							*/
	F_BYTE			deviceno,			/* デバイス番号							*/
	F_WORD *		readbuffer,			/* 読込バッファアドレス					*/
	F_DWORD 		start_sec			/* パーティション開始セクタ番号			*/
)
{
	F_INT			ret;				/* サブモジュール戻り値					*/
	F_DWORD			regadr;				/* レジスタアドレス						*/
	T_TFR			regbuf;				/* ATAコマンドバッファ					*/
	F_WORD			cnt;				/* ループカウンタ						*/

	F_BYTE			cyl_lo;				/* ATA コマンド用 シリンダ番号下位 		*/
	F_BYTE			cyl_hi;				/* ATA コマンド用 シリンダ番号上位		*/
	F_WORD			cyl;				/* ATA コマンド用 シリンダ番号 			*/
	F_BYTE			secno;				/* ATA コマンド用 セクタ番号 			*/
	F_BYTE			head;				/* ATA コマンド用 ヘッド番号			*/

/* ATAコマンド:READ 発行					*/

	secno  = (F_BYTE)(start_sec	   & 0xff);
	cyl_lo = (F_BYTE)((start_sec>>8)  & 0xff);
	cyl_hi = (F_BYTE)((start_sec>>16) & 0xff);
	cyl    = ( cyl_hi << 8 ) | cyl_lo;
	head   = (F_BYTE)((start_sec>>24) & 0x0f);

#ifdef USE_INT
	ide_int(0); 	 /* Card Ready interrupt ON */
	vcan_wup();
#endif

	if (DRV_OK != ide_cmd(1, 0, cyl, head, secno, 1, IDE_READ_SECTOR, 0)) {
		return ERR_PCREADYTMO;
	}

#ifndef USE_INT
	if (DRV_OK != ide_check(CHK_IDE_SECT)) {
		return ERR_PCREADYTMO;
	}
#else
	fsysid = vget_tid();
	slp_tsk();
	data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif

/* データ読込み */

	for( cnt = 0; cnt < TRANS_COUNT; cnt++ ) {
		*( ((F_BYTE*)readbuffer)++ ) = PCCard_io_inpb(ATA_DATA_REG);
		*( ((F_BYTE*)readbuffer)++ ) = PCCard_io_inpb(ATA_DATA_REG);
	}
	if (DRV_OK != ide_busy()) {
		return ERR_PCREADYTMO;
	}
	
#ifdef USE_INT
	ide_int(1); 	 /* Card Ready interrupt OFF */
#endif

	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_WriteBR                                                  */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     F_WORD *      writebuffer   書込バッファアドレス          */
/*         ：     F_DWORD       start_sec     パーティション開始セクタ番号  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：MBR(MasterBootRecord)又はEBR(ExtendBootRecord)書込み           */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_WriteBR(
	F_BYTE			chanelno,			/* チャネル番号							*/
	F_BYTE			deviceno,			/* デバイス番号							*/
	F_WORD *		writebuffer,		/* 書込バッファアドレス					*/
	F_DWORD 		start_sec			/* パーティション開始セクタ番号			*/
)
{
	F_INT			ret;				/* サブモジュール戻り値					*/
	F_DWORD			regadr;				/* レジスタアドレス						*/
	T_TFR			regbuf;				/* ATAコマンドバッファ					*/
	F_WORD			cnt;				/* ループカウンタ						*/

	F_BYTE			cyl_lo;				/* ATA コマンド用 シリンダ番号下位 		*/
	F_BYTE			cyl_hi;				/* ATA コマンド用 シリンダ番号上位		*/
	F_WORD			cyl;				/* ATA コマンド用 シリンダ番号 			*/
	F_BYTE			secno;				/* ATA コマンド用 セクタ番号 			*/
	F_BYTE			head;				/* ATA コマンド用 ヘッド番号			*/

/* ATAコマンド:Write Sector 発行					*/

	secno  = (F_BYTE)(start_sec	   & 0xff);
	cyl_lo = (F_BYTE)((start_sec>>8)  & 0xff);
	cyl_hi = (F_BYTE)((start_sec>>16) & 0xff);
	cyl    = ( cyl_hi << 8 ) | cyl_lo;
	head   = (F_BYTE)((start_sec>>24) & 0x0f);

	if (DRV_OK != ide_cmd(1, 0, cyl, head, secno, 1, IDE_WRITE_SECTOR, 0)) {
		return ERR_PCREADYTMO;
	}

#ifndef USE_INT
	if (DRV_OK != ide_check(CHK_IDE_SECT)) {
		return ERR_PCREADYTMO;
	}
#else
	fsysid = vget_tid();
	slp_tsk();
	data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif

/* データ転送	*/

	PCATA_dv_inf[deviceno].device_status |= DS_WAIT_INTR;
	for (cnt = 0; cnt < TRANS_COUNT; cnt++ ) {
		PCCard_io_outb(ATA_DATA_REG, *( ((F_BYTE*)writebuffer)++) );
		PCCard_io_outb(ATA_DATA_REG, *( ((F_BYTE*)writebuffer)++) );
	}
	
	if (DRV_OK != ide_busy()) {
		return ERR_PCREADYTMO;
	}
	
#ifdef USE_INT
	ide_int(1); 	 /* Card Ready interrupt OFF */
#endif

	PCATA_dv_inf[deviceno].device_status &= ~DS_WAIT_INTR;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_MakeExPTbl                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     T_MBR *       mbrbuffer     読込バッファアドレス          */
/*         ：     F_DWORD       extend_start  拡張領域開始セクタ番号        */
/*         ：     F_DWORD *     extend_size   拡張領域サイズ                */
/*         ：     F_DWORD       part_start    パーティション開始セクタ番号  */
/*         ：     PART_CREATE_PARAM * io_param I/O パラメータアドレス       */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：拡張領域.パーティションテーブル作成                            */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_MakeExPTbl(
	F_BYTE			chanelno,			/* チャネル番号							*/
	F_BYTE			deviceno,			/* デバイス番号							*/
	T_MBR *			mbrbuffer,			/* 読込バッファアドレス					*/
	F_DWORD 		extend_start,		/* 拡張領域開始セクタ番号				*/
	F_DWORD * 		extend_size,		/* 拡張領域サイズ						*/
	F_DWORD 		part_start,			/* パーティション開始セクタ番号			*/
	T_PART_CREATE_PARAM * io_param		/* I/O パラメータアドレス				*/
)
{
	F_INT			ret;				/* サブモジュール戻り値					*/
	F_DWORD			work;
	F_DWORD			work1;
	F_WORD *		bufferptr;			/* バッファアドレス						*/
	F_SHORT			cnt;				/* ループカウンタ						*/

	if ( mbrbuffer->part_table[ 0 ].part_type == MBR_PT_NONE ) {
		/* 削除済みパーティションの場合	*/
		if ( ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND1 )	&&
			 ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND2 ) ) {
			return ( ERR_NOEXTEND );	/* 拡張領域無し							*/
		}
		work = 0;
		for( cnt = 3; cnt >= 0; cnt-- ) {
			work = (work << 8) + mbrbuffer->part_table[ 1 ].top_lba[cnt];
		}
		if ( io_param->part_size <= (work - part_start -
									PCATA_dv_inf[deviceno].sec_track) ) {
			mbrbuffer->part_table[ 0 ].active_flg = 0x00;
			work = extend_start + part_start + PCATA_dv_inf[deviceno].sec_track;
			PCATA_SubLBAtoCHS(work, mbrbuffer->part_table[ 0 ].top_chs, deviceno);
			mbrbuffer->part_table[ 0 ].part_type = io_param->part_type;
			work1 = io_param->part_size;
			work1 += work;
			work1--;
			PCATA_SubLBAtoCHS(work1, mbrbuffer->part_table[ 0 ].end_chs, deviceno);
			mbrbuffer->part_table[ 0 ].top_lba[ 0 ] =
								(F_BYTE)PCATA_dv_inf[deviceno].sec_track;
			mbrbuffer->part_table[ 0 ].top_lba[ 1 ] =
								(F_BYTE)(PCATA_dv_inf[deviceno].sec_track >> 8);
			mbrbuffer->part_table[ 0 ].top_lba[ 2 ] = 0;
			mbrbuffer->part_table[ 0 ].top_lba[ 3 ] = 0;
			mbrbuffer->part_table[ 0 ].sct_size[ 0 ] =
								(F_BYTE)io_param->part_size;
			mbrbuffer->part_table[ 0 ].sct_size[ 1 ] = 
								(F_BYTE)(io_param->part_size >> 8);
			mbrbuffer->part_table[ 0 ].sct_size[ 2 ] = 
								(F_BYTE)(io_param->part_size >> 16);
			mbrbuffer->part_table[ 0 ].sct_size[ 3 ] = 
								(F_BYTE)(io_param->part_size >> 24);
			bufferptr = (F_WORD *)mbrbuffer->signature;
			*bufferptr = MBR_SIGNATURE;
			work -= PCATA_dv_inf[deviceno].sec_track;
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
			/* BPB Clear */
			bufferptr = (F_WORD *)mbrbuffer;
			for( cnt = 0; cnt < (SEC_SIZE/sizeof(F_WORD)); cnt++ ) {
				*bufferptr = 0xf6f6;
				bufferptr++;
			}
			work += PCATA_dv_inf[deviceno].sec_track;
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
			return ( ret );
		}
	}
	if ( mbrbuffer->part_table[ 1 ].part_type == MBR_PT_NONE ) {
		mbrbuffer->part_table[ 1 ].active_flg = 0x00;
		work = 0;
		work1 = 0;
		for( cnt = 3; cnt >= 0; cnt-- ) {
			work = (work << 8) + mbrbuffer->part_table[ 0 ].sct_size[cnt];
		}
		for( cnt = 3; cnt >= 0; cnt-- ) {
			work1 = (work1 << 8) + mbrbuffer->part_table[ 0 ].top_lba[cnt];
		}
		work += work1;
		if ( io_param->part_size > (*extend_size - work -
									PCATA_dv_inf[deviceno].sec_track) ) {
			return ( ERR_PARAM );
		}
		work += part_start;
		mbrbuffer->part_table[ 1 ].top_lba[ 0 ] = (F_BYTE)work;
		mbrbuffer->part_table[ 1 ].top_lba[ 1 ] = (F_BYTE)(work >> 8);
		mbrbuffer->part_table[ 1 ].top_lba[ 2 ] = (F_BYTE)(work >> 16);
		mbrbuffer->part_table[ 1 ].top_lba[ 3 ] = (F_BYTE)(work >> 24);
		mbrbuffer->part_table[ 1 ].part_type = MBR_PT_EXTEND2;
		work += extend_start;
		PCATA_SubLBAtoCHS(work, mbrbuffer->part_table[ 1 ].top_chs, deviceno);
		work1 = io_param->part_size + PCATA_dv_inf[deviceno].sec_track;
		mbrbuffer->part_table[ 1 ].sct_size[ 0 ] = (F_BYTE)work1;
		mbrbuffer->part_table[ 1 ].sct_size[ 1 ] = (F_BYTE)(work1 >> 8);
		mbrbuffer->part_table[ 1 ].sct_size[ 2 ] = (F_BYTE)(work1 >> 16);
		mbrbuffer->part_table[ 1 ].sct_size[ 3 ] = (F_BYTE)(work1 >> 24);
		work1 += work;
		work1--;
		PCATA_SubLBAtoCHS(work1, mbrbuffer->part_table[ 1 ].end_chs, deviceno);
		work1 = extend_start + part_start;
		ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work1);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
		/* 新規EBR作成	*/
		bufferptr = (F_WORD *)mbrbuffer;
		for (cnt = 0; cnt < TRANS_COUNT; cnt++ ) {
			*bufferptr = 0;
			bufferptr++;
		}
		mbrbuffer->part_table[ 0 ].active_flg = 0x00;
		work += PCATA_dv_inf[deviceno].sec_track;
		PCATA_SubLBAtoCHS(work, mbrbuffer->part_table[ 0 ].top_chs, deviceno);
		mbrbuffer->part_table[ 0 ].part_type = io_param->part_type;
		work1 = io_param->part_size;
		work1 += work;
		work1--;
		PCATA_SubLBAtoCHS(work1, mbrbuffer->part_table[ 0 ].end_chs, deviceno);
		mbrbuffer->part_table[ 0 ].top_lba[ 0 ] =
							(F_BYTE)PCATA_dv_inf[deviceno].sec_track;
		mbrbuffer->part_table[ 0 ].top_lba[ 1 ] =
							(F_BYTE)(PCATA_dv_inf[deviceno].sec_track >> 8);
		mbrbuffer->part_table[ 0 ].top_lba[ 2 ] = 0;
		mbrbuffer->part_table[ 0 ].top_lba[ 3 ] = 0;
		mbrbuffer->part_table[ 0 ].sct_size[ 0 ] =
							(F_BYTE)io_param->part_size;
		mbrbuffer->part_table[ 0 ].sct_size[ 1 ] = 
							(F_BYTE)(io_param->part_size >> 8);
		mbrbuffer->part_table[ 0 ].sct_size[ 2 ] = 
							(F_BYTE)(io_param->part_size >> 16);
		mbrbuffer->part_table[ 0 ].sct_size[ 3 ] = 
							(F_BYTE)(io_param->part_size >> 24);
		bufferptr = (F_WORD *)mbrbuffer->signature;
		*bufferptr = MBR_SIGNATURE;
		work -= PCATA_dv_inf[deviceno].sec_track;
		ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
		/* BPB Clear */
		bufferptr = (F_WORD *)mbrbuffer;
		for( cnt = 0; cnt < (SEC_SIZE/sizeof(F_WORD)); cnt++ ) {
			*bufferptr = 0xf6f6;
			bufferptr++;
		}
		work += PCATA_dv_inf[deviceno].sec_track;
		ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
		return ( ret );
	}
	if ( ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND1 )	&&
		 ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND2 ) ) {
		return ( ERR_NOEXTEND );	/* 拡張領域無し							*/
	}
	work = 0;
	work1 = 0;
	for( cnt = 3; cnt >= 0; cnt-- ) {
		work = (work << 8) + mbrbuffer->part_table[ 0 ].sct_size[cnt];
	}
	for( cnt = 3; cnt >= 0; cnt-- ) {
		work1 = (work1 << 8) + mbrbuffer->part_table[ 0 ].top_lba[cnt];
	}
	work += work1;
	*extend_size -= work;

	work = 0;
	for( cnt = 3; cnt >= 0; cnt-- ) {
		work = (work << 8) + mbrbuffer->part_table[ 1 ].top_lba[cnt];
	}
	work += extend_start;
	ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
	if ( ret != DRV_OK ) {
		return ( ret );
	}
	work -= extend_start;
	ret = PCATA_MakeExPTbl(chanelno, deviceno, mbrbuffer,
						   extend_start, extend_size, work, io_param);
	return ( ret );
}

/****************************************************************************/
/* 関 数 名：PCATA_DeleteExPTbl                                             */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     T_MBR *       mbrbuffer     読込バッファアドレス          */
/*         ：     F_DWORD       extend_start  拡張領域開始セクタ番号        */
/*         ：     F_BYTE        part_no       パーティション番号            */
/*         ：     F_DWORD       part_start    パーティション開始セクタ番号  */
/*         ：     PART_CREATE_PARAM * io_param I/O パラメータアドレス       */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：拡張領域.パーティションテーブル作成                            */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_DeleteExPTbl(
	F_BYTE			chanelno,			/* チャネル番号							*/
	F_BYTE			deviceno,			/* デバイス番号							*/
	T_MBR *			mbrbuffer,			/* 読込バッファアドレス					*/
	F_DWORD 		extend_start,		/* 拡張領域開始セクタ番号				*/
	F_BYTE  		part_no,			/* パーティション番号					*/
	F_DWORD 		part_start,			/* パーティション開始セクタ番号			*/
	T_PART_CREATE_PARAM * io_param		/* I/O パラメータアドレス				*/
)
{
	F_INT			ret;				/* サブモジュール戻り値					*/
	F_DWORD			work;
	F_DWORD			work1;
	F_WORD *		bufptr;				/* バッファアドレス						*/
	F_SHORT			cnt;				/* ループカウンタ						*/

	if ( ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND1 )	&&
		 ( mbrbuffer->part_table[ 1 ].part_type != MBR_PT_EXTEND2 ) ) {
		return ( ERR_PARAM );		/* パーティション無し			*/
	}
	work = part_start;			/* 読込セクタ番号保存					*/
	work1 = 0;
	if ( part_no != 1 ) {
		for( cnt = 3; cnt >= 0; cnt-- ) {
			work1 = (work1 << 8) + mbrbuffer->part_table[ 1 ].top_lba[cnt];
		}
	}
	ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)mbrbuffer, (extend_start + work1));
	if ( ret != DRV_OK ) {
		return ( ret );
	}
	if ( *(F_WORD *)(mbrbuffer->signature) != MBR_SIGNATURE ) {
		return ( ERR_PCCARD );
	}
	if ( part_no < io_param->part_no  ) {
		ret = PCATA_DeleteExPTbl(chanelno, deviceno, mbrbuffer, extend_start,
								 (part_no + 1), work1, io_param);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
		if ( (mbrbuffer->part_table[ 0 ].part_type == MBR_PT_NONE) &&
			 (mbrbuffer->part_table[ 1 ].part_type == MBR_PT_NONE) ) {
			/* パーティション無し			*/
			if ( part_no != 1 ) {
				work += extend_start;
			}
			ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
			bufptr = (F_WORD *)&(mbrbuffer->part_table[ 1 ]);
			for( cnt = 0; cnt < ( MBR_PARTSIZE * 3 ); cnt++ ) {
				*bufptr = 0;
				bufptr++;
			}
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
		}
		return ( ret );
	}
	else if ( part_no == io_param->part_no  ) {
		if ( mbrbuffer->part_table[ 0 ].part_type == MBR_PT_NONE ) {
			return ( ERR_PARAM );		/* パーティション無し			*/
		}
		if ( mbrbuffer->part_table[ 1 ].part_type == MBR_PT_NONE ) {
			/* 後続拡張領域無しの場合	*/
			if ( part_no != 1 ) {
				work += extend_start;
			}
			ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
			bufptr = (F_WORD *)&(mbrbuffer->part_table[ 1 ]);
			for( cnt = 0; cnt < ( MBR_PARTSIZE * 3 ); cnt++ ) {
				*bufptr = 0;
				bufptr++;
			}
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, work);
			return ( ret );
		}
		else if ( ( mbrbuffer->part_table[ 1 ].part_type == MBR_PT_EXTEND1 )	||
			 	  ( mbrbuffer->part_table[ 1 ].part_type == MBR_PT_EXTEND2 ) ) {
			/* 後続拡張領域有りの場合	*/
			bufptr = (F_WORD *)&(mbrbuffer->part_table[ 0 ]);
			for( cnt = 0; cnt < MBR_PARTSIZE; cnt++ ) {
				*bufptr = 0;
				bufptr++;
			}
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)mbrbuffer, (extend_start + work1));
			return ( ret );
		}
		return ( ERR_NOEXTEND );	/* 拡張領域無し						*/
	}
	return ( ERR_PARAM );		/* パーティション無し			*/
}

/****************************************************************************/
/* 関 数 名：PCATA_ReadSector                                               */
/*         ：                                                               */
/* パラメタ：IN   T_READ_PARAM * io_param     I/O パラメータアドレス        */
/*         ：     F_WORD         timeout      タイムアウト値                */
/*         ：     F_BYTE         chanelno     チャネル番号                  */
/*         ：     F_BYTE         deviceno     デバイス番号                  */
/*         ：     F_BYTE         unitno       ユニット番号                  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：セクタRead                                                     */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_ReadSector(T_READ_PARAM *	io_param,	/* I/O パラメータアドレス	*/
						 F_WORD		timeout,		/* タイムアウト値			*/
						 F_BYTE		chanelno,		/* チャネル番号				*/
						 F_BYTE		deviceno,		/* デバイス番号				*/
						 F_BYTE		unitno )		/* ユニット番号				*/
{
	F_WORD		cnt;	/* ループカウンタ					*/
	F_DWORD		cnt_sec;	/* セクタカウンタ					*/
	F_DWORD		total_sec;	/* 読込み総セクタ数					*/
	F_DWORD		read_sec;	/* 読込み済みセクタ数				*/
	F_DWORD		start_sec;	/* 読込み開始物理セクタ番号			*/
	F_BYTE *	bufptr;	/* 読込みデータ格納領域				*/

	F_BYTE		cyl_lo;	/* ATA コマンド用 シリンダ番号下位 */
	F_BYTE		cyl_hi;	/* ATA コマンド用 シリンダ番号上位 */
	F_WORD		cyl;	/* ATA コマンド用 シリンダ番号 */
	F_BYTE		secno;	/* ATA コマンド用 セクタ番号 */
	F_BYTE		head;	/* ATA コマンド用 ヘッド番号 */

/* 論理セクタ番号を物理セクタ番号に変換	*/

	start_sec = io_param->start_sec_num;
	start_sec += PCATA_un_inf[unitno].start_sec_num;
	total_sec = *(io_param->sec_cnt);
	bufptr = io_param->buffer_adr;
	read_sec = 0;

	while( total_sec ) {							/* 読込み総セクタ数分ループ		*/

/* ATAコマンド:Read Sector 発行					*/

		secno  = (F_BYTE)(start_sec	   & 0xff);
		cyl_lo = (F_BYTE)((start_sec>>8)  & 0xff);
		cyl_hi = (F_BYTE)((start_sec>>16) & 0xff);
		cyl    = ( cyl_hi << 8 ) | cyl_lo;
		head   = (F_BYTE)((start_sec>>24) & 0x0f);
		if (total_sec >= 256 ) {					/* ATAコマンド用読込みセクタ数 設定	*/
			cnt_sec = 256;
		}
		else {
			cnt_sec = total_sec;
		}
					
#ifdef USE_INT
		ide_int(0); 	 /* Card Ready interrupt ON */
		vcan_wup();
#endif

		if (DRV_OK != ide_cmd(1, 0, cyl, head, secno, cnt_sec, IDE_READ_SECTOR, 0)) {
			*(io_param->sec_cnt) = read_sec;
			return ERR_PCREADYTMO;
		}

#ifndef USE_INT
		if ( DRV_OK != ide_busy() ) {
			*(io_param->sec_cnt) = read_sec;
			return ERR_PCREADYTMO;
		}
#endif

		for( cnt = 0; cnt < cnt_sec; cnt++ ) {		/* 読込みセクタ数ループ		*/

#ifndef USE_INT
			if (DRV_OK != ide_check(CHK_IDE_SECT)) {
				*(io_param->sec_cnt) = read_sec;
				return ERR_PCREADYTMO;
			}
#else
			fsysid = vget_tid();
			slp_tsk();
			data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif
		/* データ転送	*/
			ata_read_sect((F_WORD *)bufptr);
			if (DRV_OK != ide_busy()) {
				*(io_param->sec_cnt) = read_sec;
				return ERR_PCREADYTMO;
			}
				
			bufptr += SEC_SIZE;
			read_sec++;
		}

#ifdef USE_INT
		ide_int(1); 	 /* Card Ready interrupt OFF */
#endif
		total_sec -= cnt_sec;
		start_sec += cnt_sec;
	}
	
	*(io_param->sec_cnt) = read_sec;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_WriteSector                                              */
/*         ：                                                               */
/* パラメタ：IN   T_IO_PARAM *   io_param     I/O パラメータアドレス        */
/*         ：     F_WORD         timeout      タイムアウト値                */
/*         ：     F_BYTE         chanelno     チャネル番号                  */
/*         ：     F_BYTE         unitno       ユニット番号                  */
/*         ：     F_BYTE         deviceno     デバイス番号                  */
/*         ：     F_BYTE         func_code    I/Oファンクション番号         */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：セクタWrite                                                    */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	PCATA_WriteSector(T_IO_PARAM *	io_param,	/* I/O パラメータアドレス	*/
						 F_WORD		timeout,		/* タイムアウト値			*/
						 F_BYTE		chanelno,		/* チャネル番号				*/
						 F_BYTE		deviceno,		/* デバイス番号				*/
						 F_BYTE		unitno,			/* ユニット番号				*/
						 F_BYTE		func_code )		/* I/Oファンクション番号	*/
{
	F_WORD		cnt,cnt1;	/* ループカウンタ					*/
	F_DWORD		cnt_sec;	/* セクタカウンタ					*/
	F_DWORD		total_sec;	/* 書込み総セクタ数					*/
	F_DWORD		write_sec;	/* 書込み済みセクタ数				*/
	F_DWORD		start_sec;	/* 書込み開始物理セクタ番号			*/
	F_BYTE *	bufptr;	/* 書込みデータ格納領域				*/

	F_BYTE		cyl_lo;	/* ATA コマンド用 シリンダ番号下位 */
	F_BYTE		cyl_hi;	/* ATA コマンド用 シリンダ番号上位 */
	F_WORD		cyl;	/* ATA コマンド用 シリンダ番号 */
	F_BYTE		secno;	/* ATA コマンド用 セクタ番号 */
	F_BYTE		head;	/* ATA コマンド用 ヘッド番号 */

	F_INT		l_drtn;	/* ドライバエントリ戻り値             */

/* 論理セクタ番号を物理セクタ番号に変換	*/

	start_sec = io_param->write_param.start_sec_num;
	start_sec += PCATA_un_inf[unitno].start_sec_num;
	total_sec = *(io_param->write_param.sec_cnt);
	if ( func_code == IO_WRITE ) {
		bufptr = io_param->write_param.buffer_adr;
	}
	write_sec = 0;
	
l_drtn = Store_BackupData((DEVIOWRITE *)&(io_param->write_param), CF_DRIVE); /* power */
if (l_drtn  != DRV_OK) {
	return l_drtn;
}

	while( total_sec ) {							/* 書込み総セクタ数分ループ		*/

/* ATAコマンド:Write Sector 発行					*/

		secno  = (F_BYTE)(start_sec	   & 0xff);
		cyl_lo = (F_BYTE)((start_sec>>8)  & 0xff);
		cyl_hi = (F_BYTE)((start_sec>>16) & 0xff);
		cyl    = ( cyl_hi << 8 ) | cyl_lo;
		head   = (F_BYTE)((start_sec>>24) & 0x0f);
		if (total_sec >= 256 ) {					/* ATAコマンド用読込みセクタ数 設定	*/
			cnt_sec = 256;
		}
		else {
			cnt_sec = total_sec;
		}

#ifdef USE_INT
		ide_int(0); 	 /* Card Ready interrupt ON */
		vcan_wup();
#endif

		if (DRV_OK != ide_cmd(1, 0, cyl, head, secno, cnt_sec, IDE_WRITE_SECTOR, 0)) {
			*(io_param->write_param.sec_cnt) = write_sec;
			return ERR_PCREADYTMO;
		}

		if ( DRV_OK != ide_busy() ) {
			*(io_param->write_param.sec_cnt) = write_sec;
			return ERR_PCREADYTMO;
		}

		for( cnt = 0; cnt < cnt_sec; cnt++ ) {		/* 書込みセクタ数ループ		*/

#ifndef USE_INT
			if (DRV_OK != ide_check(CHK_IDE_SECT)) {
				*(io_param->write_param.sec_cnt) = write_sec;
				return ERR_PCREADYTMO;
			}
#else
			fsysid = vget_tid();
			slp_tsk();
			data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif
		/* データ転送	*/
			if ( func_code == IO_WRITE ) {
				ata_write_sect((F_WORD *)bufptr);
				bufptr += SEC_SIZE;
			}
			else {
				ata_erase_sect(io_param->erase_param.erase_data);
			}

			if (DRV_OK != ide_busy()) {
				*(io_param->write_param.sec_cnt) = write_sec;
				return ERR_PCREADYTMO;
			}
			
			write_sec++;
		}

#ifdef USE_INT
		ide_int(1); 	 /* Card Ready interrupt OFF */
#endif
		total_sec -= cnt_sec;
		start_sec += cnt_sec;
	}
	
	*(io_param->write_param.sec_cnt) = write_sec;
	return DRV_OK;
}

/******************************************************************************/
/*      Unpublished Copyright (C) 2003      ウェスコム株式会社                */
/******************************************************************************/
