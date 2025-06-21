/******************************************************************************
* NORTi Network File Systemt Header                                           *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 07/Mar/1999                                                                 *
* 06/May/1999 'remove' is added.                                              *
* 23/May/1999 'feof' and 'rename' is added.                                   *
* 22/Sep/1999 resource ID auto assign added.                                  *
* 29/Nov/2000 18個以上のファイルを置けない不具合修正                          *
* 12/Api/2001 GAIO対応用に'#ifndef stdin'と'#ifndef FILE'を追加               *
* 12/Api/2001 GAIOの予約語と衝突のため direct→_direct                        *
* 12/Sep/2001 エラーコード:EV_FATLINKの追加                        M.Kawahara *
* 17/Jan/2002 B*をchar*に修正、ワーニングの消去                    M.Kawahara *
* 08/Feb/2002 マクロ定義衝突回避 for CC32R                                 OK *
* 18/Jun/2002 dformatマクロ定義バグ修正                                    KA *
* 25/Feb/2003 "Green Hills" コンパイラの判断を __ghs でするように修正      KA *
******************************************************************************/

#ifdef GAIO
  #ifndef _MAXFILE
  #include <stdio.h>
  #endif
#else
  #ifndef SEEK_SET
  #include <stdio.h>
  #endif
#endif

#ifndef SEEK_SET /* If FILE is not defined in stdio.h */
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#ifndef FILE
#define FILE        void
#endif
#endif

#ifndef NONFILE_H
#define NONFILE_H
#ifdef __cplusplus
extern "C" {
#endif

/* string.h */

#if defined(__ghs)              /* Green Hills */
  int stricmp(const char *d, const char *s);
  #define   unpacked    /* */
#elif defined(_MCC68K)          /* MCC68K       */
  int stricmp(const char *d, const char *s);
#else                           /* Hitachi SH-C */
  #define   unpacked    /* */
#endif

/* File System Parameter */

#undef  FILENAME_MAX
#define FILENAME_MAX        (2+8+1+3)   /* D:nnnnnnnn.eee */
#undef  NFILE
#define SECTOR_SIZE         512

/* Disk Parameter Block */

typedef unpacked struct t_disk {
    unpacked struct t_disk *next;   /* Pointer to Next Disk Parameter Block */
    UH sc;                          /* Sectors per Cluster              */
    UH rsc;                         /* Reserved Sector Count            */
    UH fn;                          /* Number of FATs                   */
    UH rde;                         /* Root Directry Entrys             */
    UH sf;                          /* Sectors per FAT                  */
    UH saveclus;                    /* allocate cluster no              */
    UW ts;                          /* Total Sectors                    */
    B  ch;                          /* Drive Number                     */
    B  name;                        /* Drive Name ('A'-)                */
    ER (*func)(unpacked struct t_disk *, FN, UW, VP); /* Pointer to Read/Write Function */
    UW addr;                        /* Device Base Address              */
    UW par;                         /* Device Dependent Parameter       */
    ER ercd;                        /* Last Error Code                  */
} T_DISK;

typedef ER (*DISK_FP)(T_DISK *, FN, UW, VP);

/* Directry Entry */

typedef unpacked struct t_dir_entry {
    UB name[8+3];
    B attr;
    B rsv[10];
    UH time;
    UH date;
    UH clusno;
    UW size;
} T_DIR_ENTRY;

#define DIR_ENTRY_SZ        (sizeof (T_DIR_ENTRY))  /* 32 */

/* File Control Block */

typedef unpacked struct t_file {
    ER ercd;
    UH entrno;
    UH byteno;  /* buf内のoffset */
    UH start;   /* 開始クラスタ番号 */
    UW sectno;
    UW pos;
    UW size;
    B stat;
    B name[FILENAME_MAX+1];
    T_DISK *d;
    B buf[SECTOR_SIZE];
} T_FILE;

#define TFS_OPENED      0x01
#define TFS_CHANGED     0x02
#define TFS_RD_MODE     0x04
#define TFS_WR_MODE     0x08

/* Directry */

#define DIRECT_FSIZE
#define DIRSIZ          14

unpacked struct _direct {
    short d_ino;
    char d_name[DIRSIZ];
    char fsize[8+1];
    char filler[1];
    char dmmy[6];
};
#ifndef GAIO
#define direct _direct
#endif

/* Function Codes */

#define TFN_READ_SECTOR     1
#define TFN_WRITE_SECTOR    2
#define TFN_IDENTIFY        3
#define TFN_FORMAT          4
#define TFN_HARDWARE_INIT   5
#define TFN_HARDWARE_RESET  6

/* Error Codes */

#define EV_NOFILE       (-1)        /* file is not        */
#define EV_DISKFULL     (-2)        /* disk full          */
#define EV_DRIVE        (-3)        /* drive name error   */
#define EV_FSYS_NAME    (-4)        /* abnormal file name */
#define EV_ALREADY      (-5)        /* Already Opend at Write mode      */
#define EV_NOFCB        (-6)        /* No room for File Control Block   */
#define EV_NOOPEN       (-7)        /* file is not opened */
#define EV_NOEMPTY      (-8)        /* ディレクトリが空でない ★追加    */
#define EV_EXIST        (-9)        /* ファイルが存在する     ★追加    */
#define EV_FSYS_PAR     (-10)       /* parameter error    */
#define EV_FATLINK      (-11)       /* FATが破壊されている */

/* Functions */

ER     file_ini( T_FILE *f, INT nfile, ID tskid, ID semid);
ER     disk_ini( T_DISK *, char*, DISK_FP, UW, UW );
FILE*  my_fopen( const char *path, const char *mode );
int    my_fclose( FILE *fp );
int    my_fseek( FILE *fp, long offset, int origin );
int    my_fgetc( FILE *fp );
int    my_fputc( int c, FILE *fp );
char*  my_fgets( char *string, int n, FILE *fp );
int    my_fputs( const char *string, FILE *fp );
size_t my_fread( void *buf, size_t size, size_t count, FILE *fp );
size_t my_fwrite( const void *buf, size_t size, size_t count, FILE *fp );
long   my_ftell( FILE *fp );
int    my_feof( FILE *fp );
int    my_remove( const char *path );
int    my_rename( const char *oldname, const char *newname );
int    my_dformat( const char* drivename );

/*
 * 標準LIB内での定義を回避し置き換えるため ..by OK,2002.2.8
 * for CC32R
 */

#undef fopen
#undef fclose
#undef fseek
#undef fgetc
#undef fputc
#undef fgets
#undef fputs
#undef fread
#undef fwrite
#undef ftell
#undef feof
#undef remove
#undef rename

#define fopen(n,m)      my_fopen(n,m)
#define fclose(f)       my_fclose(f)
#define fseek(f,p,m)    my_fseek(f,p,m)
#define fgetc(f)        my_fgetc(f)
#define fputc(c,f)      my_fputc(c,f)
#define fgets(b,l,f)    my_fgets(b,l,f)
#define fputs(b,f)      my_fputs(b,f)
#define fread(b,s,n,f)  my_fread(b,s,n,f)
#define fwrite(b,s,n,f) my_fwrite(b,s,n,f)
#define ftell(f)        my_ftell(f)
#define feof(f)         my_feof(f)
#define remove(n)       my_remove(n)
#define rename(n1,n2)   my_rename(n1,n2)
#define dformat(n)      my_dformat(n)

ER ramdisk(T_DISK *, FN, UW, VP);

#ifdef __cplusplus
}
#endif
#endif /* NONFILE_H */

