/*======================================================================
 * File name    : fsapicnv.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/rcs/fsapicnv.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsapicnv.h $
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

#ifndef _FSAPICNV_H_
#define _FSAPICNV_H_

#define fopen(p,f,a)		FMR_Open(p,f,a)
#define fclose(fh)			FMR_Close(fh)
#define fseek(fh,of,or)		FMR_Seek(fh,of,or)
#define fread(fh,r,s)		FMR_Read(fh,r,s)
#define fwrite(fh,w,s)		FMR_Write(fh,w,s)
#define ftell(fh)			FMR_Tell(fh)
#define setatr(p,a)			FMR_SetAtr(p,a)
#define rename(o,n)			FMR_Rename(o,n)
#define remove(p)			FMR_Remove(p)
#define mkdir(p)			FMR_Mkdir(p)
#define rmdir(p)			FMR_Rmdir(p)
#define finfo(p,d)			FMR_Stat(p,d)
#define fgetc(fh)			fmr_fgetc(fh)
#define fgets(s,n,fh)		fmr_fgets(s,n,fh)
#define fputc(s,fh)			fmr_fputc(s,fp)
#define fputs(s,fh)			fmr_fputs(s,fp)

#endif /* _FSAPICNV_H_ */
