/*======================================================================
 * File name    : powfail.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/rcs/powfail.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: powfail.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
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
#ifndef	__POWFAIL_H__
#define	__POWFAIL_H__

/*==========================================================
					定数宣言
==========================================================*/
#define START_BACKUP_AREA	(0x22001000)		/* バックアップ領域先頭アドレス */
#define TOTAL_BACKUP_SECT	(56)				/* バックアップ可能セクタ数 */

#define BACKUP_SECTSIZE		(512)				/* セクタサイズ */
#define BACKUP_MEMFULL		(0)					/* バックアップ領域メモリフルエラー */

enum {
	SRAM_BKUP_AREA_NUM,	/* 0: SRAMドライブ使用バックアップ領域番号	*/
	CF_BKUP_AREA_NUM,	/* 1: CFドライブ使用バックアップ領域番号	*/
	USE_BKUP_AREA_NUM	/* 2: 使用バックアップ領域数 */
};


/*==========================================================
						構造体宣言
==========================================================*/
/* データ書込みI/O */
typedef struct tagDEVIOWRITE{
    unsigned long startsctnum;                /* 書込み開始セクタ番号               */
    unsigned short * writesctnum;             /* 書込みセクタ数                     */
    unsigned char * writebuff;                /* 書込みデータ格納領域               */
    unsigned long reserved;                   /* 予約領域                           */
}DEVIOWRITE;

/* データ読込みI/O */
typedef struct tagDEVIOREAD{
    unsigned long startsctnum;                /* 読込み開始セクタ番号               */
    unsigned short * readsctnum;              /* 読込みセクタ数                     */
    unsigned char * readbuff;                 /* 読込みデータ格納領域               */
    unsigned long reserved;                   /* 予約領域                           */
}DEVIOREAD;

/* セクタ毎のバックアップデータ */
typedef struct tagBACKUPSECT{
    unsigned long startsctnum;						/* 書込み開始セクタ番号 */
    unsigned char writebuff[BACKUP_SECTSIZE];		/* backupセクタ格納領域 */
}BACKUPSECT;

/* バックアップデータ */
typedef struct tagBACKUPDATA{
    unsigned long volumeid;							/* ボリュームシリアル番号 */
    int power_fail;									/* 電断判別フラグ */
    BACKUPSECT backupsect[TOTAL_BACKUP_SECT];		/* セクタ毎のバックアップデータ */
}BACKUPDATA;


/*==========================================================
					関数プロトタイプ宣言
==========================================================*/
void SetScanDiskFlg(int drvnum);
int GetScanDiskFlg(int drvnum);
void ClearScanDiskFlg(int drvnum);
void SetBackupFlg(int drvnum);
int GetBackupFlg(int drvnum);
void ClearBackupFlg(int drvnum);
void Set_PowerFailureFlg(int drvnum);
int Get_PowerFailureFlg(int drvnum);
void Clear_PowerFailureFlg(int drvnum);
void Init_BackupParameter(void);
int Restore_BackupData(int drvnum);
int Store_BackupData(DEVIOWRITE *wr_param, int drvnum);
unsigned short CheckSum_BackupArea(void);

#endif		/* __POWFAIL_H__ */

/*======================================================================*/
/*====                   ***** END OF FILE *****                    ====*/
/*======================================================================*/
