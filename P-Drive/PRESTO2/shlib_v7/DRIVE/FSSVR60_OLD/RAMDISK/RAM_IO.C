/*======================================================================
 * File name    : ram_io.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/ramdisk/RCS/ram_io.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ram_io.c $
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
/* ファイル名：RAM_io.c                                                     */
/* 概    要  ：RAMディスクドライバ                                          */
/*           ：I/O 制御部                                                   */
/* 作 成 日  ：2004.07.13                                                   */
/* 作 成 者  ：                                                             */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
/*  01.00 First Release									2004.07.13			*/
/****************************************************************************/

#include "ram_def.h"
#include "ram_hw.h"
#include "ram_srct.h"
#include "ram_prot.h"
#include "biosfile.h"
#include "powfail.h"

/****************************************************************************/
/* 関 数 名：RAM_ReadMBR                                                    */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanelno      チャネル番号                  */
/*         ：     F_BYTE        deviceno      デバイス番号                  */
/*         ：     F_BYTE *      unit_cnt      ユニット数                    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：MBR(MasterBootRecord)読込み                                    */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	RAM_ReadMBR(	F_BYTE		chanelno,		/* チャネル番号				*/
						F_BYTE		deviceno,		/* デバイス番号				*/
						F_BYTE *	unit_cnt )		/* ユニット数				*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	F_WORD		cnt;		/* ループカウンタ		*/
	F_SHORT		cnt1;		/* ループカウンタ		*/
	F_WORD		tmp;
	F_BYTE *	memadr;		/* レジスタアドレス		*/
	F_BYTE		unitno;		/* ユニット番号			*/

	T_PARTITION	* 	partbuf[4];	/* パーティションテーブルポインタ	*/
	F_WORD *	p_buf;		/* バッファポインタ		*/

	memadr = RAM_ch_inf[chanelno].memory_base;
	RAM_ch_inf[chanelno].unit_cnt = 0;				/* 接続ユニット数クリア		*/
	RAM_dv_inf[deviceno].unit_cnt = 0;
	unitno = 0;

	partbuf[0] = (T_PARTITION *)(memadr + MBR_BOOTSIZE);	/* パーティションテーブルアドレス設定	*/
	for( cnt = 0; cnt < 4; cnt++ ) {		/* パーティションテーブル読み込み		*/
		if ( partbuf[cnt]->part_type == MBR_PT_FAT12  ||
			 partbuf[cnt]->part_type == MBR_PT_FAT161 ||
			 partbuf[cnt]->part_type == MBR_PT_FAT162 ||
			 partbuf[cnt]->part_type == MBR_PT_FAT163 ||
			 partbuf[cnt]->part_type == MBR_PT_FAT32 ) {

			RAM_ch_inf[chanelno].unit_cnt++;	/* 接続ユニット数+1			*/
			RAM_dv_inf[deviceno].unit_cnt++;
			RAM_un_inf[unitno].cnct_chanel = chanelno;
			RAM_un_inf[unitno].cnct_device = deviceno;
			RAM_un_inf[unitno].part_type = partbuf[cnt]->part_type;
			RAM_un_inf[unitno].sec_size = (SEC_SIZE >> 9);
			RAM_un_inf[unitno].total_sec_cnt = 0;
			RAM_un_inf[unitno].start_sec_num = 0;
			for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
				RAM_un_inf[unitno].total_sec_cnt = (RAM_un_inf[unitno].total_sec_cnt << 8) \
													+ partbuf[cnt]->sct_size[cnt1];
			}
			for( cnt1 = 3; cnt1 >= 0; cnt1-- ) {
				RAM_un_inf[unitno].start_sec_num = (RAM_un_inf[unitno].start_sec_num << 8) \
													+ partbuf[cnt]->top_lba[cnt1];
			}
			unitno++;
		}
	}

	tmp = *(memadr + MBR_SIGADDR);
	if ( tmp != MBR_SIGNATURE ) {
		unitno = 0;
	}
	*unit_cnt = unitno;

	return ( ret );

}

/****************************************************************************/
/* 関 数 名：RAM_ReadSector                                                 */
/*         ：                                                               */
/* パラメタ：IN   T_READ_PARAM * io_param     I/O パラメータアドレス        */
/*         ：     F_BYTE         chanelno     チャネル番号                  */
/*         ：     F_BYTE         deviceno     デバイス番号                  */
/*         ：     F_BYTE         unitno       ユニット番号                  */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：セクタRead                                                     */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	RAM_ReadSector(T_READ_PARAM *	io_param,	/* I/O パラメータアドレス	*/
						 F_BYTE		chanelno,		/* チャネル番号				*/
						 F_BYTE		deviceno,		/* デバイス番号				*/
						 F_BYTE		unitno )		/* ユニット番号				*/
{
	F_INT		ret;		/* サブモジュール戻り値				*/
	F_BYTE		flag;		/* 奇数偶数アドレスフラグ 0=奇数、1=Word偶数、2=DoubleWord偶数	*/
	T_WORD_BYTE	tmpbuf;
	F_WORD		cnt;		/* ループカウンタ					*/

	F_DWORD		total_sec;	/* 読込み総セクタ数					*/
	F_DWORD		read_sec;	/* 読込み済みセクタ数				*/
	F_DWORD		start_adr;	/* 開始メモリアドレス				*/
	F_DWORD *	memadr;		/* 読込みメモリアドレス				*/
	F_BYTE *	p_buffer;	/* 読込みデータ格納領域				*/

/* 論理セクタ番号をメモリアドレスに変換	*/

	start_adr = io_param->start_sec_num;
	start_adr += RAM_un_inf[unitno].start_sec_num;	/* 物理セクタ番号に変換			*/
	start_adr = start_adr << 7;						/* 物理セクタ番号をダブルワードメモリアドレスに変換	*/
	memadr = (F_DWORD *)(RAM_ch_inf[chanelno].memory_base);	/* RAMディスク開始メモリアドレス設定	*/
	memadr += start_adr;							/* Read開始メモリアドレス設定	*/
	total_sec = *(io_param->sec_cnt);
	p_buffer = io_param->buffer_adr;
	read_sec = 0;

/* 読込みバッファアドレス奇数・偶数判断	*/

	if ( (F_PTR)p_buffer & (F_PTR)0x00000001 ) {	/* バッファアドレス=奇数		*/
		flag = 0;
	}
	else if  ( (F_PTR)p_buffer & (F_PTR)0x00000002 ) {	/* バッファアドレス=偶数		*/
		flag = 1;
	}
	else {
		flag = 2;
	}

	for( read_sec = 0; read_sec < total_sec; read_sec++ ) {	/* 読込みセクタ数ループ		*/

/* データ転送	*/

		if ( flag  == 1 ) {					/* バッファアドレス=偶数の場合のデータ転送	*/
			for (cnt = 0; cnt < TRANS_WCOUNT; cnt++ ) {
				*((F_WORD *)p_buffer)++ = *((F_WORD *)memadr)++;
			}
		}
		else if ( flag  == 2 ) {			/* バッファアドレス=DoubleWord偶数の場合のデータ転送	*/
			for (cnt = 0; cnt < TRANS_DCOUNT; cnt++ ) {
				*((F_DWORD *)p_buffer)++ = *memadr++;
			}
		}
		else {								/* バッファアドレス=奇数の場合のデータ転送	*/
			for (cnt = 0; cnt < TRANS_DCOUNT; cnt++ ) {
				tmpbuf.dwordval = *memadr++;
				*p_buffer++ = tmpbuf.byteval[0];
				*p_buffer++ = tmpbuf.byteval[1];
				*p_buffer++ = tmpbuf.byteval[2];
				*p_buffer++ = tmpbuf.byteval[3];
			}
		}
	}
	*(io_param->sec_cnt) = read_sec;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_WriteSector                                                */
/*         ：                                                               */
/* パラメタ：IN   T_IO_PARAM *   io_param     I/O パラメータアドレス        */
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
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
F_INT	RAM_WriteSector(T_IO_PARAM *	io_param,	/* I/O パラメータアドレス	*/
						 F_BYTE		chanelno,		/* チャネル番号				*/
						 F_BYTE		deviceno,		/* デバイス番号				*/
						 F_BYTE		unitno,			/* ユニット番号				*/
						 F_BYTE		func_code )		/* I/Oファンクション番号	*/
{
	F_INT		ret;		/* サブモジュール戻り値				*/
	F_BYTE		flag;		/* 奇数偶数アドレスフラグ 0=奇数、1=Word偶数、2=DoubleWord偶数	*/
	T_WORD_BYTE	tmpbuf;
	F_WORD		cnt;		/* ループカウンタ					*/

	F_DWORD		total_sec;	/* 書込み総セクタ数					*/
	F_DWORD		write_sec;	/* 書込み済みセクタ数				*/
	F_DWORD		start_adr;	/* 開始メモリアドレス				*/
	F_DWORD *	memadr;		/* 書込みメモリアドレス				*/
	F_BYTE *	p_buffer;	/* 書込みデータ格納領域				*/

	F_INT		l_drtn;		/* ドライバエントリ戻り値             */

/* 論理セクタ番号をメモリアドレスに変換	*/

	start_adr = io_param->write_param.start_sec_num;
	start_adr += RAM_un_inf[unitno].start_sec_num;	/* 物理セクタ番号に変換			*/
	start_adr = start_adr << 7;						/* 物理セクタ番号をダブルワードメモリアドレスに変換	*/
	memadr = (F_DWORD *)(RAM_ch_inf[chanelno].memory_base);	/* RAMディスク開始メモリアドレス設定	*/
	memadr += start_adr;							/* Write開始メモリアドレス設定	*/
	total_sec = *(io_param->write_param.sec_cnt);
	p_buffer = io_param->write_param.buffer_adr;
	write_sec = 0;

/* 書込みバッファアドレス奇数・偶数判断	*/

	if ( (F_PTR)p_buffer & (F_PTR)0x00000001 ) {	/* バッファアドレス=奇数		*/
		flag = 0;
	}
	else if  ( (F_PTR)p_buffer & (F_PTR)0x00000002 ) {	/* バッファアドレス=偶数		*/
		flag = 1;
	}
	else {											/* バッファアドレス= DoubleWord偶数	*/
		flag = 2;
	}

	if ( func_code == IO_WRITE ) {
		for( write_sec = 0; write_sec < total_sec; write_sec++ ) {	/* 書込みセクタ数ループ		*/

l_drtn = Store_BackupData((DEVIOWRITE *)&(io_param->write_param), SRAM_DRIVE); /* power */
if (l_drtn  != DRV_OK) {
	return l_drtn;
}

/* データ転送	*/

			if ( flag  == 1 ) {					/* バッファアドレス=偶数の場合のデータ転送	*/
				for (cnt = 0; cnt < TRANS_WCOUNT; cnt++ ) {
					*((F_WORD *)memadr)++ = *((F_WORD *)p_buffer)++;
				}
			}
			else if ( flag  == 2 ) {			/* バッファアドレス=DoubleWord偶数の場合のデータ転送	*/
				for (cnt = 0; cnt < TRANS_DCOUNT; cnt++ ) {
					*memadr++ = *((F_DWORD *)p_buffer)++;
				}
			}
			else {								/* バッファアドレス=奇数の場合のデータ転送	*/
				for (cnt = 0; cnt < TRANS_DCOUNT; cnt++ ) {
					tmpbuf.byteval[0] = *p_buffer++;
					tmpbuf.byteval[1] = *p_buffer++;
					tmpbuf.byteval[2] = *p_buffer++;
					tmpbuf.byteval[3] = *p_buffer++;
					*memadr++ = tmpbuf.dwordval;
				}
			}
		}
	}
	else {
		tmpbuf.byteval[0] = io_param->erase_param.erase_data;
		tmpbuf.byteval[1] = io_param->erase_param.erase_data;
		tmpbuf.byteval[2] = io_param->erase_param.erase_data;
		tmpbuf.byteval[3] = io_param->erase_param.erase_data;
		for( write_sec = 0; write_sec < total_sec; write_sec++ ) {	/* 書込みセクタ数ループ		*/

/* データ転送	*/

			for (cnt = 0; cnt < TRANS_DCOUNT; cnt++ ) {
				*memadr++ = tmpbuf.dwordval;
			}
		}
	}
	*(io_param->write_param.sec_cnt) = write_sec;
	return DRV_OK;
}

/******************************************************************************/
/*      Unpublished Copyright (C) 2003      ウェスコム株式会社                */
/******************************************************************************/
