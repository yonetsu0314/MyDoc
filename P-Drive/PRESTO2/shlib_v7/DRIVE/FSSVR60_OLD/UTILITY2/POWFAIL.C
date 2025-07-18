/*======================================================================
 * File name    : powfail.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility2/RCS/powfail.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: powfail.c $
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
#define	__POWFAIL_C__

#include	<string.h>
#include	"powfail.h"
#include	"biosfile.h"
#include	"fsbase.h"

static int act_scandisk[DFMR_USR_MAXDRVNUM];		/* スキャンディスク実行中フラグ */
static int backupflg[DFMR_USR_MAXDRVNUM];			/* 書込み前セクタ内容バックアップフラグ */
static int use_area_num[DFMR_USR_MAXDRVNUM];		/* 各ドライブ使用バックアップ領域ID登録テーブル */
static BACKUPDATA *backupdata[USE_BKUP_AREA_NUM];	/* 電断対策用バックアップデータ */

/*****************************************************************************
*
*	<電断対策処理>
*		機能:	FAT、ディレクトリエントリのセクタデータをバックアップする
*			SRAM:バックアップ領域0
*			CF:バックアップ領域1
*
******************************************************************************/

/*----------------------------------------------------------------------*/
/*	スキャンディスク実行中フラグ(実行中はバックアップを行わない)		*/
/*----------------------------------------------------------------------*/
void SetScanDiskFlg(int drvnum)
{
	act_scandisk[drvnum-DFMR_TOPDRVNUM] = 1;
}

int GetScanDiskFlg(int drvnum)
{
	return act_scandisk[drvnum-DFMR_TOPDRVNUM];
}

void ClearScanDiskFlg(int drvnum)
{
	act_scandisk[drvnum-DFMR_TOPDRVNUM] = 0;
}

/*----------------------------------------------------------------------*/
/*	書込み前のセクタ内容バックアップフラグ				*/
/*----------------------------------------------------------------------*/
void SetBackupFlg(int drvnum)
{
	if ( !GetScanDiskFlg(drvnum) ) {
		backupflg[drvnum-DFMR_TOPDRVNUM] = 1;
	}
}

int GetBackupFlg(int drvnum)
{
	return backupflg[drvnum-DFMR_TOPDRVNUM];
}

void ClearBackupFlg(int drvnum)
{
	if ( !GetScanDiskFlg(drvnum) ) {
		backupflg[drvnum-DFMR_TOPDRVNUM] = 0;
	}
}

/*----------------------------------------------------------------------*/
/*	電断判別フラグ(値はバックアップしたセクタ数)				*/
/*----------------------------------------------------------------------*/
void Set_PowerFailureFlg(int drvnum)
{
	int areanum;
	
	areanum = use_area_num[drvnum - DFMR_TOPDRVNUM];
	backupdata[areanum]->power_fail += 1; 
}

int Get_PowerFailureFlg(int drvnum)
{
	int areanum;
	
	areanum = use_area_num[drvnum - DFMR_TOPDRVNUM];
	return backupdata[areanum]->power_fail;
}

void Clear_PowerFailureFlg(int drvnum)
{
	int areanum;
	
	areanum = use_area_num[drvnum - DFMR_TOPDRVNUM];
	backupdata[areanum]->power_fail = 0;
}

/*----------------------------------------------------------------------*/
/*	バックアップパラメータの初期化				*/
/*		SRAM:バックアップ領域0				*/
/*		CF:バックアップ領域1				*/
/*----------------------------------------------------------------------*/
void Init_BackupParameter(void)
{	
	int areanum;

	/* 使用変数の初期化 */
	memset( (void *)act_scandisk, 0, sizeof(act_scandisk) );
	memset( (void *)backupflg, 0, sizeof(backupflg) );
	memset( (void *)use_area_num, 0, sizeof(use_area_num) );

	/* バックアップ領域のメモリ割り当て */
	for (areanum = 0; areanum < USE_BKUP_AREA_NUM; areanum++) {
		backupdata[areanum] = (BACKUPDATA *)(START_BACKUP_AREA + sizeof(BACKUPDATA) * areanum);
	}
	
	/* 各ドライブの使用バックアップ領域の登録 */
	use_area_num[SRAM_DRIVE - DFMR_TOPDRVNUM] = SRAM_BKUP_AREA_NUM;
	use_area_num[CF_DRIVE - DFMR_TOPDRVNUM] = CF_BKUP_AREA_NUM;
}

/*----------------------------------------------------------------------*/
/*	FAT、ディレクトリエントリのセクタデータの復元				*/
/*----------------------------------------------------------------------*/
int Restore_BackupData(int drvnum)
{	
	DEVIOWRITE wr_param;
	unsigned short targetctnum;     /* 書込みデータ数              */
	int areanum, secnum, cnt, drtn, ret;

	ret= DFMR_OK;
	areanum = use_area_num[drvnum - DFMR_TOPDRVNUM];

	/* ボリュームシリアル番号のチェック */
	if ( backupdata[areanum]->volumeid != MFMR_VOLID(drvnum) ) {
		/* 一度もStore_BackupDataが実行されなかった場合、異なるCFがセットされた場合 */
		/* バックアップ領域のクリア */
		memset( (void *)backupdata[areanum], 0, sizeof(BACKUPDATA) );
		return ret; 
	}

	/* 電断が起こっていた場合はバックアップ内容を復元する */
	secnum = Get_PowerFailureFlg(drvnum);
	if ( (secnum > 0) &&  (secnum <= TOTAL_BACKUP_SECT) ) {
		for (cnt = 0;  cnt < secnum; cnt++) {
			wr_param.startsctnum = backupdata[areanum]->backupsect[cnt].startsctnum;
			targetctnum = 1;
			wr_param.writesctnum = &targetctnum;
			wr_param.writebuff = backupdata[areanum]->backupsect[cnt].writebuff;			
			drtn = MFMR_DRVENTRY(drvnum)(MFMR_CUNUM(drvnum), DFMR_DRVFUNCWRITE, &wr_param);
			if ((drtn != DFMR_OK) || (targetctnum != 1)) {
				if (drtn != DFMR_OK) {
					ret = drtn;
				}
				else {
					ret = DFMR_FATALERR;
				}
			}
		}
	}

	/* バックアップ領域のクリア */
	memset( (void *)backupdata[areanum], 0, sizeof(BACKUPDATA) );
	return ret;
}

/*----------------------------------------------------------------------*/
/*	FAT、ディレクトリエントリのセクタデータをバックアップする			*/
/*----------------------------------------------------------------------*/
int Store_BackupData(DEVIOWRITE *wr_param, int drvnum)
{
	DEVIOREAD rd_param;
	unsigned char *memadr;
	unsigned short targetctnum;     /* 書込みデータ数              */
	int areanum, secnum, drtn, cnt;

	if ( !(GetBackupFlg(drvnum)) ) {
	/* データ領域へのデータ書込みの場合、またはスキャンディスク実行中 */
		return DFMR_OK;
	}
	
	if ( (secnum = Get_PowerFailureFlg(drvnum)) >= TOTAL_BACKUP_SECT ) {
		return BACKUP_MEMFULL;	/* バックアップ領域メモリフル */
	}
	
	areanum = use_area_num[drvnum - DFMR_TOPDRVNUM];	
	memadr = backupdata[areanum]->backupsect[secnum].writebuff;

	/* ボリュームシリアル番号のバックアップ */
	backupdata[areanum]->volumeid = MFMR_VOLID(drvnum);
	/* 書込み開始セクタ番号のバックアップ */
	backupdata[areanum]->backupsect[secnum].startsctnum = wr_param->startsctnum;

	/* 同じセクタ番号が既にバックアップされているかチェックする */
	for (cnt = 0;  cnt < secnum; cnt++) {
		if (wr_param->startsctnum == backupdata[areanum]->backupsect[cnt].startsctnum) {
			return DFMR_OK; /* 同じセクタ番号が既にバックアップされている場合 */
		}
	}
	
/* 書込み前のセクタ内容をバックアップする */
	rd_param.startsctnum = wr_param->startsctnum;
	rd_param.reserved = wr_param->reserved;
	targetctnum = *(wr_param->writesctnum);
	rd_param.readsctnum = &targetctnum;
	rd_param.readbuff = memadr;
	
	drtn = MFMR_DRVENTRY(drvnum)(MFMR_CUNUM(drvnum), DFMR_DRVFUNCREAD, &rd_param);
	if ((drtn != DFMR_OK) || (targetctnum != 1)) {
		if (drtn != DFMR_OK) {
			return drtn;
		}
		else {
			return DFMR_FATALERR;
		}
	}
		
	Set_PowerFailureFlg(drvnum); /* 電断判別フラグのセット */
	return DFMR_OK;
}

/*======================================================================*/
/*====                   ***** END OF FILE *****                    ====*/
/*======================================================================*/
