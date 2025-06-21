/*---------------------------------------------------------------------
 *$Header: p:/presto2/shinc/RCS/fsys.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fsys.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 実態のない関数dformatのﾌﾟﾛﾄﾀｲﾌﾟを削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/*******************************************************************
    FileSystem (FAT FORMAT FILESYSTEM)
                                                          H.Takeuchi
    History    : 2000.06.01 : Ver 1.00 : Release
               : 2000.08.21 : Ver 2.00 : Update
                 2000.08.25 : Ver 2.01 : Update
********************************************************************/

#ifndef FSYS
#define FSYS

/* G_FATを#defineするとファイルマネージャ処理で遅くなる */
//#define G_FAT

#define ON_NORTI
/*#define USE_INT*/

#undef SEEK_SET
#define SEEK_SET    0
#undef SEEK_CUR
#define SEEK_CUR    1
#undef SEEK_END
#define SEEK_END    2

#undef EOF
#define EOF     (-1)

#ifdef FSYS_C

#ifndef ON_NORTI
typedef char            B;
typedef unsigned char  UB;
typedef short           H;
typedef unsigned short UH;
typedef long            W;
typedef unsigned long  UW;
typedef H              FN;
typedef H              ER;
typedef void          *VP;
typedef short        BOOL;
#endif  /* ON_NORTI */

#undef TRUE
#define TRUE        1

#undef FALSE
#define FALSE       0

#endif /* FSYS_C */

#define ATTR_RO			0x01			/* read-only 	*/
#define ATTR_HIDE		0x02			/* hidden 		*/
#define ATTR_SYS		0x04  			/* system 		*/
#define ATTR_VOL		0x08			/* volume label */
#define ATTR_DIR		0x10			/* directory 	*/
#define ATTR_ARCH		0x20			/* archived 	*/
#define ATTR_FILE	    0x20


#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FAT12
#define FAT12      12
#define FAT16      16
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SH7000)     /* Green Hills */
int stricmp(const char *d, const char *s);
#endif

/* File System Parameter */
#undef  FILENAME_MAX
#define FILENAME_MAX    111    /* A:\ddddddddddd~\ffffffff.fff = 2 + 12*8 + 13 = 111 */
#undef  DIRLEN_MAX
#define DIRLEN_MAX       11    /* ddddddddddd */
#undef  FILELEN_MAX
#define FILELEN_MAX      12    /* aaaaaaaa.xxx */
#undef  DIRNUM_MAX
#define DIRNUM_MAX        8    /* dir num max */
#define SECT_SZ         512

/* Disk Parameter Block */
typedef struct st_disk {
    struct st_disk *next;    /* pointer to next disk parameter block */
    unsigned short sc;      /* sectors per cluster */
    unsigned short rsc;     /* reserved sector count */
    unsigned short fn;      /* number of FAT */
    unsigned short rde;     /* root directry entrys */
    unsigned short sf;      /* sectors per FAT */
    unsigned long ts;       /* total sectors */
    char  fatbits;          /* fat bits */
    char  ch;               /* drive number */
    char  name;             /* drive name ('A','B'...) */
    char  ram; 	            /* ram disk */
    short (*func)(struct st_disk *, short, unsigned long, void*, unsigned short); /* pointer to drive function */
    short (*callback)(struct st_disk *, short);     /* pointer ro callback function */
    char  drive;            /* drive number */
    char  dsk_flg;          /* disk status & attribute */
                            /*   D7...1=mounted,0=not mounted/unmounted     */
                            /*   D6...1=Inserted,0=not inserted             */
                            /*   D0...1=removable,0=Fixed media             */
    short  id_cyc;          /* insert check handler's ID */
    unsigned long addr;     /* device base address */
    unsigned long par;      /* device dependent parameter */
    unsigned long n;        /* last sector number */
    unsigned char *p;       /* last sector buffer address */
    char *fatp;             /* pointer to FATable */
    short ercd;             /* last error code */
    unsigned long logic;
/*****追加*******/
	unsigned short nullfat;
	unsigned char  r_buf[SECT_SZ];
/*****追加*******/
} ST_DISK;

#define DISK_MOUNT      0x80
#define DISK_INS        0x40
#define DISK_REMOVABLE  0x01

typedef short (*DISK_FP)(ST_DISK *, short, unsigned long, void*, unsigned short);
typedef short (*DISK_CALLBACK)(ST_DISK *, short);

/* Directry Entry */
typedef struct st_direntry {
    unsigned char name[8+3];
    char attr;
    char rsv[10];
    unsigned short time;
    unsigned short date;
    unsigned short clusno;
    unsigned long size;
} ST_DIRENTRY;

#define DIRENTRY_SZ    32    /* 64 at VFAT/FAT32 */

/* File Control Block */
typedef struct st_file {
    short ercd;
    unsigned short entrno;		/* ディレクトリエントリ番号		*/
    unsigned short byteno;		/* シークセクタからのオフセット	*/
    unsigned short start;		/* 先頭クラスタ番号				*/
    unsigned long sectno;		/* シーク物理セクタ番号			*/
    unsigned long motsec;		/* ディレクトリエントリのセクタ	*/
    unsigned long pos;			/* シークオフセット				*/
    unsigned long size;			/* ファイルサイズ				*/
    char stat;
    char name[FILENAME_MAX+1];	/* ファイル名					*/
    ST_DISK *d;					/* ＤＰＢへのポインタ			*/
    char dir;					/* サブディレクトリの時１		*/
    char dum;
    short secchg;				/* キャッシュ更新フラグ			*/
    char buf[SECT_SZ];			/* １セクタ分のキャッシュ		*/
} ST_FILE;

#define FS_RD    0x01
#define FS_WR    0x10
#define FS_RDWR  (FS_RD | FS_WR)  /* 0x11 */
#define FS_APND  0x13

typedef struct st_tm {
    int tm_sec;     /* 秒 */
    int tm_min;     /* 分 */
    int tm_hour;    /* 時（0～23）*/
    int tm_mday;    /* 日（1～31）*/
    int tm_mon;     /* 月（0～11, 1月を0とする）*/
    int tm_year;    /* 年（1900年を0とする）*/
    int tm_wday;    /* 曜（0～6, 日曜日を0とする）*/
    int tm_yday;    /* 1月1日からの日数（0～365） */
    int tm_isdst;   /* 夏時間（-1） */
}ST_TM;

/* Directry */
#define DIRSIZ          14

struct direct {
    short d_ino;
    char d_name[DIRSIZ];
};

typedef struct st_dir{
    char  stat;
    unsigned char  dnum;
    unsigned short entr_no;
    unsigned short sect;
    unsigned short pos;
    char buf[SECT_SZ];
    ST_DISK *d;
}ST_DIR;

/* for get file info */
typedef struct st_fdata {
    char name[11];       /* file name */
    char attr;           /* attribute */
    ST_TM time;          /* day time */
    short clust;         /* top clustor No. */
    unsigned long size;  /* length */
}ST_FDATA;

/* Function Codes */
#define TFN_READ_SECTOR     1
#define TFN_WRITE_SECTOR    2
#define TFN_IDENTIFY        3
#define TFN_FORMAT          4
#define TFN_HARDWARE_INIT   5
#define TFN_HARDWARE_RESET  6
#define TFN_MOUNT           7
#define TFN_UNMOUNT         8
#define TFN_MEDIACHK        9
#define TFN_SECTOR_ADR     10

/* Function Code to Callback Function */
#define TFN_DISK_READING    1
#define TFN_DISK_WRITING    2
#define TFN_DISK_STOPPED    3
#define TFN_DISK_DETECTED   4
#define TFN_DISK_REMOVED    5
#define TFN_DISK_MOUNTED    6
#define TFN_DISK_UNMOUNTED  7
#define TFN_DISK_ERROR      8

/* Error Codes */
#define R_OK                 0 /* successful         */
#define R_ABN_FILE          -1 /* abnormal file name */
#define R_SAME_FILE         -2 /* same name file     */
#define R_FAT_FULL          -3 /* fatable full       */
#define R_READ_DISK         -4 /* disk read error    */
#define R_WRITE_DISK        -5 /* disk write error   */
#define R_OVER_DIR_NUM      -6 /* over dir num       */
#define R_OVER_FILE_NUM     -7 /* over file num      */
#define R_NO_FILE           -8 /* file is not        */
#define R_ABN_MODE          -9 /* abnormal mode      */
#define R_FILE_NOT_OPEN    -10 /* file not open      */
#define R_ABN_SIZE         -11 /* abnormal size      */
#define R_PARAM_ERR        -12 /* parameter error    */
#define R_NOT_SUPORT       -13 /* not supported      */
#define R_NOT_OPEN         -14 /* file is not opened */
#define R_DISK_FULL        -15 /* disk full          */
#define R_DRIVE_ERR        -16 /* drive name error   */
#define R_ERR              -99 /* error              */

/* Functions */
//int dformat(const char *, unsigned long);
int h_dirlist(const char * , unsigned char * , short);
int detail_directory(char *, char *, unsigned short);
int next_detail_directory(short, char *, unsigned short);

ST_FILE   *ht_fopen(const char * fname, const char * mode);
char       ht_fclose(ST_FILE * fp);
int        ht_fgetc(ST_FILE * fp);
char      *ht_fgets(char * s, short n, ST_FILE * fp);
int        ht_fputc(int c, ST_FILE * fp);
char       ht_fputs(const char * s, ST_FILE * fp);
unsigned long      ht_fread(void * ptr, unsigned long size, unsigned long n, ST_FILE * fp);
unsigned long      ht_fwrite(const void * ptr, unsigned long size, unsigned long n, ST_FILE * fp);
char       ht_fseek(ST_FILE * fp, long offset, char type);
long       ht_ftell(ST_FILE * fp);
char       ht_feof(ST_FILE * fp);
char       ht_rename(const char * old, const char * new);
char       ht_remove(const char * pathname);
char       ht_finfo(const char * pathname, ST_FDATA * fdt);
char       ht_mkdir(const char * pathname);
char       ht_rmdir(const char * pathname);
ST_DIR    *ht_opendir(const char * dname);
char       ht_closedir(ST_DIR * dp);
ST_DIRENTRY *ht_readdir(ST_DIR * dp);
unsigned long      ht_getfree(char drv);

char       ini_fsys(ST_FILE * f, short n, short semid);
char       ini_disk(ST_DISK * d, char drv, DISK_FP func, unsigned long addr, unsigned long par, DISK_CALLBACK callback, char removable, char *fatp);
char       mount_disk(ST_DISK * d);
char       unmount_disk(ST_DISK * d);
char fat_to_ram(char drv);
char ram_to_fat(char drv);
unsigned char get_id_filesem( void );

/* ISHIDA 2000/10/26 s.sakamoto */
char f_chsize( ST_FILE *fp );

#define fopen(f,m)       ht_fopen(f,m)
#define fclose(fp)       ht_fclose(fp)
#define fgetc(fp)        ht_fgetc(fp)
#define fgets(s,n,fp)    ht_fgets(s,n,fp)
#define fputc(s,fp)      ht_fputc(s,fp)
#define fputs(s,fp)      ht_fputs(s,fp)
#define fseek(fp,o,t)    ht_fseek(fp,o,t)
#define ftell(fp)        ht_ftell(fp)
//#define feof(fp)         ht_feof(fp)	 /* stdio.hの((fp)->_ioflag1&_IOEOF)と競合 */
#define rename(o,n)      ff_rename(o,n)	/*020821*/
#define remove(p)        ff_remove(p)	/*020821*/
#define finfo(p,fd)      ht_finfo(p,fd)
#define mkdir(p)         ff_mkdir(p)	/*020821*/
#define rmdir(p)         ff_rmdir(p)	/*020821*/
#define opendir(d)       ht_opendir(d)
#define closedir(dp)     ht_closedir(dp)
#define readdir(dp)      ht_readdir(dp)
#define getfree(dv)      ht_getfree(dv)

#define fread(p,s,n,fp)  ht_fread(p,s,n,fp)
#define fwrite(b,s,n,f)  ht_fwrite(b,s,n,f)

#ifdef __cplusplus
};
#endif
#endif /* FSYS */
