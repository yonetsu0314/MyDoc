/*======================================================================
 * File name	: cnvfsys.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		機種のファイルシステムに関する関数群のヘッダ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/rcs/cnvfsys.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cnvfsys.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#ifndef		__CNVFSYS_H_INC__
#define		__CNVFSYS_H_INC__

#include <drive\fssvr60\s60fat.h>

/*---- defines ---------------------------------------------------------*/
/*------------------------------------*/
/*--- アクセスモード（OpenFlag） -----*/
/*------------------------------------*/
#define	IFS_O_RDONLY	(DFMR_O_RDONLY)
#define	IFS_O_WRONLY	(DFMR_O_WRONLY)
#define	IFS_O_RDWR		(DFMR_O_RDWR)
#define	IFS_O_CREAT		(DFMR_O_CREAT|DFMR_O_RDWR)
#define	IFS_O_APPEND	(DFMR_O_APPEND|DFMR_O_RDWR)
#define	IFS_O_EXCL		(DFMR_O_CREAT|DFMR_O_RDWR|DFMR_O_EXCL)

/*------------------------------------*/
/*--- ファイル属性 -------------------*/
/*------------------------------------*/
#define	IFS_ATTR_RONLY	(DFMR_ATTR_RONLY)
#define	IFS_ATTR_HIDE	(DFMR_ATTR_HIDE)
#define	IFS_ATTR_SYS	(DFMR_ATTR_SYS)
#define	IFS_ATTR_VOLID	(DFMR_ATTR_VOLID)
#define	IFS_ATTR_DIR	(DFMR_ATTR_DIR)
#define	IFS_ATTR_ARC	(DFMR_ATTR_ARC)

/*------------------------------------*/
/*--- SEEK 初期位置 ------------------*/
/*------------------------------------*/
#define	IFS_SEEK_SET	(DFMR_SEEK_SET)
#define	IFS_SEEK_CUR	(DFMR_SEEK_CUR)
#define	IFS_SEEK_END	(DFMR_SEEK_END)

/*------------------------------------*/
/*--- エラーコード -------------------*/
/*------------------------------------*/
#define	IFS_SUC			(DFMR_SUC)		/* API正常終了		0          */
#define	IFS_ENOENT		(DFMR_ENOENT)	/* 該当ファイル無し	-2         */
#define	IFS_EEXIST		(DFMR_EEXIST)	/* 既存ファイル有り	-17        */

/*---- valiables -------------------------------------------------------*/

/*---- prototypes ------------------------------------------------------*/
int		IFS_open(const char *path, int flag, unsigned char atrb);
int		IFS_fopen(const char *filename, const char *mode);
int		IFS_close(int filehandle);
int		IFS_fclose(int filehandle);
int 	IFS_read(int filehandle, void *readbuff, unsigned int size);
int		IFS_write(int filehandle, void *writebuff, unsigned int size);
long	IFS_seek(int filehandle, long offset, int origin);
long	IFS_tell(int filehandle);
int		IFS_remove(const char *path);
int		IFS_setatr(const char *path, unsigned char atrb);
int		IFS_rename(const char *oldpath, const char *newpath);
int		IFS_mkdir(const char *path);
int		IFS_rmdir(const char *path);
int		IFS_stat(const char *path, TFMR_FILE_INFO *fileinfo);
long	IFS_FindFirst(const char *path, TFMR_FILE_INFO *fileinfo);
int		IFS_FindNext(long srchhandle, TFMR_FILE_INFO *fileinfo);
int		IFS_FindClose(long srchhandle);
int		IFS_info(char drvnum, TFMR_DRIVE_INFO *drvinfo);
int		IFS_fgetc(int filehandle);
char	*IFS_fgets(char *line, int maxline, int filehandle);
int		IFS_fputc(int ch, int filehandle);
int		IFS_fputs(const char *line, int filehandle);

#endif		/* __CNVFSYS_H_INC__ */

/*--- End of File ---*/

