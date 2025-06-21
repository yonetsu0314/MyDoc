/*======================================================================
 * File name    : ram_srct.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/ramdisk/RCS/ram_srct.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ram_srct.h $
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
/* ファイル名：RAM_Struct.h                                                 */
/* 概    要  ：構造体定義                                                     */
/*           ：                                                               */
/* 作 成 日  ：2004.07.13                                                     */
/* 作 成 者  ：                                                               */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */

/******************************************************************************/

#ifndef __RAM_STRUCT_H__
#define __RAM_STRUCT_H__


/*************************************/
/* I/O パラメータ定義                */
/*************************************/


	/* 初期化用 I/O パラメータ */

typedef struct tag_T_INIT_PARAM {
	F_BYTE				reserve_b[4];			/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				reserve_d[2];			/* 予備						*/
} T_INIT_PARAM;

	/* マウント用 I/O パラメータ */

typedef struct tag_T_MOUNT_PARAM {
	F_BYTE				mbr_mode;				/* MBRモード				*/
	F_BYTE				part_type;				/* パーティションタイプ		*/
	F_BYTE				sec_size;				/* セクタサイズ				*/
	F_BYTE				reserve_b;				/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				total_sec_cnt;			/* ユニット総セクタ数		*/
	F_DWORD				start_sec_num;			/* ユニット開始セクタ番号	*/
} T_MOUNT_PARAM;

		/* MBRモード 		*/

#define MOPAR_MBR_EXIST	0x00					/* MBR有り					*/
#define MOPAR_MBR_NON	0x01					/* MBR無し					*/


	/* アンマウント用 I/O パラメータ */

typedef struct tag_T_UNMOUNT_PARAM {
	F_BYTE				reserve_b[4];			/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				reserve_d[2];			/* 予備						*/
} T_UNMOUNT_PARAM;

	/* チャネル情報取得用 I/O パラメータ */

typedef struct tag_T_CHANEL_INFO_PARAM {
	F_BYTE				reserve_b[4];			/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				reserve_d[2];			/* 予備						*/
} T_CHANEL_INFO_PARAM;

	/* デバイス情報取得用 I/O パラメータ */

typedef struct tag_T_DEVICE_INFO_PARAM {
	F_WORD				sec_track;				/* トラック当りのセクタ数	*/
	F_WORD				cylinder_cnt;			/* シリンダ数				*/
	F_WORD				head_cnt;				/* ヘッド数					*/
	F_WORD				reserve_w;				/* 予備						*/
	F_DWORD				total_sec_cnt;			/* 合計セクタ数				*/
	F_DWORD				reserve_d;				/* 予備						*/
} T_DEVICE_INFO_PARAM;

	/* ユニット情報取得用 I/O パラメータ */

typedef struct tag_T_UNIT_INFO_PARAM {
	F_BYTE				part_type;				/* パーティションタイプ		*/
	F_BYTE				sec_size;				/* セクタサイズ				*/
	F_BYTE				unit_status;			/* ユニット状態				*/
	F_BYTE				reserve_b;				/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				total_sec_cnt;			/* ユニット総セクタ数		*/
	F_DWORD				start_sec_num;			/* ユニット開始セクタ番号	*/
} T_UNIT_INFO_PARAM;

	/* データ読込み用 I/O パラメータ */

typedef struct tag_T_READ_PARAM {
	F_DWORD				start_sec_num;			/* 読込み開始セクタ番号		*/
	F_WORD *			sec_cnt;				/* 読込みセクタ数			*/
	F_BYTE *			buffer_adr;				/* 読込みデータ格納領域		*/
	F_DWORD				reserve_d;				/* 予備						*/
} T_READ_PARAM;

	/* データ書込み用 I/O パラメータ */

typedef struct tag_T_WRITE_PARAM {
	F_DWORD				start_sec_num;			/* 書込み開始セクタ番号		*/
	F_WORD *			sec_cnt;				/* 書込みセクタ数			*/
	F_BYTE *			buffer_adr;				/* 書込みデータ格納領域		*/
	F_DWORD				reserve_d;				/* 予備						*/
} T_WRITE_PARAM;

	/* データ消去用 I/O パラメータ */

typedef struct tag_T_ERASE_PARAM {
	F_DWORD				start_sec_num;			/* 消去開始セクタ番号		*/
	F_WORD *			sec_cnt;				/* 消去セクタ数				*/
	F_BYTE				erase_data;				/* 消去データ				*/
	F_BYTE				reserve_b[3];			/* 予備						*/
	F_DWORD				reserve_d;				/* 予備						*/
} T_ERASE_PARAM;

	/* メディア挿抜チェック用 I/O パラメータ */

typedef struct tag_T_DETECT_INFO_PARAM {
	F_BYTE				detect_status;			/* 挿抜状態					*/
	F_BYTE				reserve_b[3];			/* 予備						*/
	F_WORD				reserve_w[2];			/* 予備						*/
	F_DWORD				reserve_d[2];			/* 予備						*/
} T_DETECT_INFO_PARAM;

	/* パーティション作成用 I/O パラメータ */

typedef struct tag_T_PART_SUB_PARAM {
	F_BYTE				part_type;				/* パーティションタイプ		*/
	F_BYTE				reserve_b[3];			/* 予備						*/
	F_DWORD				part_size;				/* パーティションサイズ		*/
} T_PART_SUB_PARAM;

typedef struct tag_T_PART_CREATE_PARAM {
	F_BYTE				part_cnt;				/* パーティション数			*/
	F_BYTE				reserve_b[3];			/* 予備						*/
	T_PART_SUB_PARAM	part_sub[4];			/* パーティションサブ情報	*/
} T_PART_CREATE_PARAM;

	/* I/O パラメータ共用体 */

typedef union tagT_IO_PARAM {
	T_INIT_PARAM		init_param;				/* 初期化					*/
	T_MOUNT_PARAM		mount_param;			/* マウント					*/
	T_UNMOUNT_PARAM		unmount_param;			/* アンマウント				*/
	T_CHANEL_INFO_PARAM	chanel_param;			/* チャネル情報取得			*/
	T_DEVICE_INFO_PARAM	device_param;			/* デバイス情報取得			*/
	T_UNIT_INFO_PARAM	unit_param;				/* ユニット情報取得			*/
	T_READ_PARAM		read_param;				/* データ読込み				*/
	T_WRITE_PARAM		write_param;			/* データ書込み				*/
	T_ERASE_PARAM		erase_param;			/* データ消去				*/
	T_DETECT_INFO_PARAM	detect_param;			/* メディア挿抜チェック		*/
	T_PART_CREATE_PARAM	part_param;				/* パーティション作成		*/
} T_IO_PARAM;


/*************************************/
/* ドライバ情報定義                  */
/*************************************/


typedef struct tagT_DRIVER_INFO {
	F_BYTE				reserve_b[4];			/* 予備						*/
} T_DRIVER_INFO;


/*************************************/
/* チャネル情報定義                  */
/*************************************/


typedef struct tagT_CHANEL_INFO {
	F_BYTE				chanel_status;			/* チャネル状態				*/
												/* Bit0,1,2：マウント状態	*/
												/* 0x00=未初期化,0x01=初期化済み,0x02=マウント済み	*/

	F_BYTE				device_cnt;				/* 接続デバイス数			*/
	F_BYTE				unit_cnt;				/* 接続ユニット数			*/
	F_BYTE				reserve_b;				/* 予備						*/
	F_BYTE *			memory_base;			/* RAMディスク開始メモリアドレス				*/
	F_DWORD				memory_size;			/* RAMディスクサイズ		*/
	F_DWORD				task_id;				/* 当チャネルを使用しているタスクのタスクID		*/
} T_CHANEL_INFO;

	/* チャネル状態定義 		*/
	/* Bit0,1,2：マウント状態	*/

#define CS_DEFAULT		0x00					/* 未初期化					*/
#define CS_INIT			0x01					/* 初期化済み				*/
#define CS_MOUNT		0x02					/* マウント済み				*/
#define CS_CLEAR		0xf8					/* マウント状態 クリア		*/


/*************************************/
/* デバイス情報定義                  */
/*************************************/


typedef struct tagT_DEVICE_INFO {
	F_BYTE				device_status;			/* デバイス状態				*/
												/* Bit0,1,2：マウント状態&カード挿抜状態		*/
												/*   0x00=未マウント,0x01=マウント済み,			*/
												/*   0x02=カード抜き発生、0x03=カード再挿入発生	*/
												/*   0x04=別カード挿入							*/
												/* Bit3：カードIREQ割込み待ち状態				*/
												/*   0=割込み待ちせず、1=割込み待ち				*/
												/* Bit4：MBR(MasterBootRecord)の有無			*/
												/*   0=MBR有り、1=MBR無し						*/

	F_BYTE				unit_cnt;				/* 接続ユニット数			*/
	F_BYTE				cnct_chanel;			/* 接続チャネル番号			*/
	F_BYTE				reserve_b1;				/* 予備						*/
	F_WORD				sec_track;				/* トラック当りのセクタ数	*/
	F_WORD				cylinder_cnt;			/* シリンダ数				*/
	F_WORD				head_cnt;				/* ヘッド数					*/
	F_WORD				reserve_w1;				/* 予備						*/
	F_DWORD				total_sec;				/* 総セクタ数				*/
	F_DWORD				volume_id;				/* ボリュームID				*/
	F_BYTE				volume_name[11];		/* ボリューム名				*/
	F_BYTE				reserve_b2;				/* 予備						*/
} T_DEVICE_INFO;

	/* デバイス状態定義 */
	/* Bit0,1,2：マウント状態&カード挿抜状態	*/

#define DS_DEFAULT		0x00					/* 未マウント				*/
#define DS_MOUNT		0x01					/* マウント済み				*/
#define DS_PULL			0x02					/* カード抜き発生			*/
#define DS_INSERT		0x03					/* カード再挿入発生			*/
#define DS_CARDERR		0x04					/* 別カード挿入				*/
#define DS_CLEAR		0xf8					/* マウント状態&カード挿抜状態 クリア	*/

	/* Bit4：MBR(MasterBootRecord)の有無			*/

#define DS_NONMBR		0x10					/* MBR無し					*/


/*************************************/
/* ユニット情報定義                  */
/*************************************/


typedef struct tagT_UNIT_INFO {
	F_BYTE				unit_status;			/* ユニット状態				*/
												/* Bit0,1,2：マウント状態	*/
												/*   0x00=未マウント,0x01=マウント済み			*/

	F_BYTE				cnct_chanel;			/* 接続チャネル番号			*/
	F_BYTE				cnct_device;			/* 接続デバイス番号			*/
	F_BYTE				part_type;				/* パーティションタイプ		*/
	F_BYTE				sec_size;				/* セクタサイズ				*/
	F_BYTE				reserve_b[3];			/* 予備						*/
	F_DWORD				total_sec_cnt;			/* ユニット総セクタ数		*/
	F_DWORD				start_sec_num;			/* ユニット開始セクタ番号	*/
} T_UNIT_INFO;

	/* ユニット状態定義 	*/
	/* Bit0,1,2：マウント	*/

#define US_DEFAULT		0x00					/* 未マウント				*/
#define US_MOUNT		0x01					/* マウント済み				*/
#define US_CLEAR		0xf8					/* マウント状態クリア		*/


/*************************************/
/* MBR & パーティションテーブル定義  */
/*************************************/


typedef struct tagT_PARTITION {
	F_BYTE				active_flg;				/* アクティブフラグ					*/
	F_BYTE				top_chs[3];				/* 開始セクタ(CHS)					*/
	F_BYTE				part_type;				/* パーティションタイプ				*/
	F_BYTE				end_chs[3];				/* 終了セクタ(CHS)					*/
	F_BYTE				top_lba[4];				/* 開始セクタ(LBA)					*/
	F_BYTE				sct_size[4];			/* セクタサイズ						*/
} T_PARTITION;

typedef struct tagT_MBR {
	F_BYTE				boot_loader[446];		/* ブートストラップローダー			*/
	T_PARTITION			part_table[4];			/* パーティションテーブル×4		*/
	F_BYTE				signature[2];			/* シグネチャ(0x55,0xAA)			*/
} T_MBR;


/*************************************/
/* バウンダリ調整用共用体            */
/*************************************/


typedef union tagT_WORD_BYTE {
	F_DWORD				dwordval;				/* ダブルワード値					*/
	F_WORD				wordval[2];				/* ワード値							*/
	F_BYTE				byteval[4];				/* バイト値							*/
} T_WORD_BYTE;


#endif /* __RAM_STRUCT_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

