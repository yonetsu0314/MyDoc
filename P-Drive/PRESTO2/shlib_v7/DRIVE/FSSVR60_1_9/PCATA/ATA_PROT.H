/*======================================================================
 * File name    : ata_prot.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/ata_prot.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ata_prot.h $
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
/* ファイル名：PCATA_Prot.h                                                   */
/* 概    要  ：プロタイプ宣言 & 変数領域宣言                                  */
/*           ：                                                               */
/* 作 成 日  ：2004.03.19                                                     */
/* 作 成 者  ：                                                               */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */

/******************************************************************************/

#ifndef __PCATA_PROT_H__
#define __PCATA_PROT_H__

#include <kernel.h>
/************************************/
/*	プロトタイプ宣言 				*/
/************************************/

/* PCATA_main.c */
F_INT	PCATA_main(			F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							F_BYTE					func_code,		/* I/Oファンクション番号	*/
							T_IO_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_Init(			F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_INIT_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_Mount(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_MOUNT_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_Unmount(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_UNMOUNT_PARAM *		io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_ChanelInfo(	F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_CHANEL_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_DeviceInfo(	F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_DEVICE_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_UnitInfo(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_UNIT_INFO_PARAM *		io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_ReadWrite(	F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							F_BYTE					func_code,		/* I/Oファンクション番号	*/
							T_IO_PARAM * 			io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_DetectInfo(	F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_DETECT_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_PartCreate(	F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_PART_CREATE_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

/* PCATA_io.c */
F_INT	PCATA_ReadIDTFY(	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno	);	/* デバイス番号				*/

F_INT	PCATA_ReadPTable(	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							F_BYTE *				unit_cnt,		/* ユニット数				*/
							T_MBR *					readbuffer	);	/* 読込バッファ				*/

F_INT	PCATA_ReadExPTbl(	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							F_BYTE *				unit_cnt,		/* ユニット数				*/
							T_MBR *					readbuffer,		/* 読込バッファ				*/
							F_DWORD 				extend_start,	/* 拡張領域開始セクタ番号	*/
							F_DWORD 				part_start	);
														/* パーティションテーブル.開始セクタ番号*/

F_INT	PCATA_ReadBR(		F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							F_WORD *				readbuffer,		/* 読込バッファアドレス		*/
							F_DWORD 				start_sec	);	/* パーティション開始セクタ番号	*/

F_INT	PCATA_WriteBR(		F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							F_WORD *				writebuffer,	/* 書込バッファアドレス		*/
							F_DWORD 				start_sec	);	/* パーティション開始セクタ番号	*/

F_INT	PCATA_MakeExPTbl(
							F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							T_MBR *					mbrbuffer,		/* 読込バッファアドレス		*/
							F_DWORD 				extend_start,	/* 拡張領域開始セクタ番号	*/
							F_DWORD * 				extend_size,	/* 拡張領域サイズ			*/
							F_DWORD 				part_start,		/* パーティション開始セクタ番号	*/
							T_PART_CREATE_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_DeleteExPTbl(
							F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							T_MBR *					mbrbuffer,		/* 読込バッファアドレス		*/
							F_DWORD 				extend_start,	/* 拡張領域開始セクタ番号	*/
							F_BYTE  				part_no,		/* パーティション番号		*/
							F_DWORD 				part_start,		/* パーティション開始セクタ番号	*/
							T_PART_CREATE_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	PCATA_ReadSector(	T_READ_PARAM *			io_param,		/* I/O パラメータアドレス	*/
						 	F_WORD					timeout,		/* タイムアウト値			*/
						 	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
						 	F_BYTE					unitno		);	/* ユニット番号				*/

F_INT	PCATA_WriteSector(	T_IO_PARAM *			io_param,		/* I/O パラメータアドレス	*/
						 	F_WORD					timeout,		/* タイムアウト値			*/
						 	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
						 	F_BYTE					unitno,			/* ユニット番号				*/
						 	F_BYTE					func_code	);	/* I/Oファンクション番号	*/

void	PCATA_SubLBAtoCHS(	F_DWORD					sec_num_LBA,	/* セクタ番号：LBA			*/
							F_BYTE *				sec_num_CHS,	/* セクタ番号：CHS形式		*/
							F_BYTE					deviceno	);	/* デバイス番号				*/

/* PCATA_OS_inh.c */
void	PCATA_osGetTID(		F_DWORD *				tskid		);

/* PCATA_sub.c */
void	PCATA_SubMemSet(	F_BYTE *				dest,			/* 設定先ポインタ			*/
							F_BYTE					code,			/* 設定コード				*/
							F_WORD					count		);	/* 設定バイト数             */

void	PCATA_SubMemCpy(	F_BYTE *				dest,			/* 設定先ポインタ			*/
							F_BYTE *				src,			/* 設定コード				*/
							F_WORD					count		);	/* 設定バイト数             */

/* ata_cmd.c */
ER		ide_int(			UB						set			);

ER		ide_busy(			void								);

ER		ide_check(			UH						check		);

ER		ide_cmd(			UB						chs,			/* CHS or LBA				*/
							UB						drv,			/* drive select				*/
							UH						cyl,			/* cylinder					*/
							UB						head,			/* head number				*/
							UB						f_sec,			/* start sector				*/
							UB						sec_cnt,		/* sector num				*/
							UB						cmd,			/* command					*/
							UB						fc			);

/************************************/
/*	変数領域宣言	 				*/
/************************************/

/* ドライバデータ領域        */

extern 	T_DRIVER_INFO	PCATA_drv_inf;

/* チャネルデータ領域        */

extern 	T_CHANEL_INFO	PCATA_ch_inf[MAX_CHANEL];

/* デバイスデータ領域        */

extern 	T_DEVICE_INFO	PCATA_dv_inf[MAX_DEVICE];

/* ユニットデータ領域        */

extern 	T_UNIT_INFO		PCATA_un_inf[MAX_UNIT];

#endif /* __PCATA_PROT_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

