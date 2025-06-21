/*======================================================================
 * File name    : fsys.c
 * Original		: r:/c/astra-s/exp/rcs/fsys.c 1.21
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/misc/fs/RCS/fsys.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: fsys.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ファイル・システムとFLASH書込み
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
			   : 2000.08.22 : L1229,1905,1927 mkdir bug fixed
							  L1944 rmdir function bug fixed
********************************************************************/
#define FSYS_C
#include <kernel.h>
#include "fsys.h"
#include "date.h"

//#include "define.h"
//#include "clkctl.h"

	/*==========================================================
					ＩＴＲＯＮ変数定義
	==========================================================*/
#ifdef V3	/* NORTi v3 */
//	#include <norti3.h>
	static const T_CSEM file_csem  = { NULL, TA_TFIFO, 0, 1 };*/
#else		/* NORTi v4 */
//	#include </itron4/kernel.h>
	static const T_CSEM file_csem  = { TA_TFIFO, 0, 1, NULL };
#endif

	/*==========================================================
						ｄｅｆｉｎｅ宣言
	==========================================================*/
/* Disk Parameter */
#define DIR_ENTRYS_PER_SECTOR	(SECT_SZ/DIRENTRY_SZ)

	/*==========================================================
						プロトタイプ宣言
	==========================================================*/
short ramdisk( ST_DISK *d, short fncd, unsigned long sectno, void * par, unsigned short num );

	/*==========================================================
						静的変数宣言
	==========================================================*/
/* Local Variable */
static B NFILE;			/* ＭＡＸファイル数				*/
static ST_FILE *file;	/* ファイルポインタの配列の先頭 */
static ST_DISK *disk;	/* ディスクポインタ配列の先頭	*/
static UB ID_FILE_SEM;	/* ファイル排他セマフォ			*/
static ST_DIR dir;

/* for FAT12/16 */
B name_toup[FILENAME_MAX + 1];
B fname_toup[FILELEN_MAX + 1];

/*******************************************************************
	memcpy
	src から dest に count バイト数をコピーします。
	dest を返します。
********************************************************************/
static void *fs_memcpy(void *dest, const void *src, UW count)
{
	UW	n = count;
	B *de = dest;
	B *sr = src;

	while(n--)
		*de++ = *sr++;

	return dest;
}

/*******************************************************************
	memset
	指定された文字でバッファを初期化します。
	dest を返します。
********************************************************************/
static void *fs_memset(void *dest, B c, UW count)
{
	UW n = count;
	B *de = dest;

	while(n--)
		*de++ = c;

	return dest;
}

/*******************************************************************
	memmove
	src から dest に count バイト数をコピーします。
	コピー元とコピー先の領域が一部重なっている場合は、重なっている部
	分の内容をコピーしてから、上書きします。
	dest の値をそのまま返します。
********************************************************************/
static void *fs_memmove(void *dest, const void *src, UW count)
{
	B *s_top = src;
	B *d_top = dest;
	B *s_end;
	B *d_end;
	UW n = count;

	s_end = (B*)(s_top + count);
	d_end = (B*)(d_top + count);

	if((s_top < d_top) && (s_end > d_top))
	{
		while(n--)
			*d_end-- = *s_end--;
		return dest;
	}

	while(n--)
		*d_top++ = *s_top++;

	return dest;
}

/*******************************************************************
	memcmp
	２つのバッファ内の文字を比較します。
********************************************************************/
static B fs_memcmp(const void *buf1, const void *buf2, UW count)
{
	B b1, b2;
	B *b1_p = (B*)buf1;
	B *b2_p = (B*)buf2;
	UW n;

	for (n=0; n<count; n++)
	{
		b1 = *b1_p++;
		b2 = *b2_p++;
		if (b1 != b2)
			return (B)-1;
	}
	return (B)0;
}

/*******************************************************************
	strlen
	文字列の長さを取得します。
********************************************************************/
static UW fs_strlen(const B *s)
{
	B *ss = s;
	UW	r = 0;

	while(*ss++ != '\0')
		r++;

	return r;
}

/*******************************************************************
	strncpy
	s2 の最初の c バイト数を s1 にコピーしてから s1 を返します。
	c が s2 の長さ以下の場合、最後にNULL文字を付けません。
	c が s2 より長い場合、s1 には c の長さまでNULL文字が埋め込まれま
	す。
********************************************************************/
static B *fs_strncpy(B *s1, const B *s2, UW c)
{
	UW n;
	B *ss1 = s1;
	B *ss2 = s2;

	for(n = 0; n < c; n++, ss1++, ss2++)
	{
		if(*ss2 != '\0')
			*ss1 = *ss2;
		else
			*ss1 = (B)'\0';
	}
	return s1;

}

/*******************************************************************
	toupper
	小文字('a'～'z')が与えらると大文字を返します。
	それ以外はそのまま与えられた文字を返します。
********************************************************************/
static B fs_toupper(B c)
{
	if(c >= 'a' && c <= 'z')
		return (B)(c - 0x20);

	return (B)c;
}

/*******************************************************************
	fs_str_cpy_upr
	文字列中の文字を大文字に変換してコピー。
********************************************************************/
static B *fs_str_cpy_upr(B *s1, const B *s2, UW c)
{
	UW n;
	B *ss1 = s1;
	B *ss2 = s2;

	for(n = 0; n < c; n++, ss1++, ss2++)
	{
		if(*ss2 != '\0')
			*ss1 = fs_toupper(*ss2);
		else
			*ss1 = (B)'\0';
	}
	return s1;

}

/*******************************************************************
	stricmp
	文字列中の文字を小文字として比較します。
********************************************************************/
static H fs_stricmp(const B *des, const B *src)
{
	H c1, c2;

	for (;;)
	{
		c1 = *des++;
		c2 = *src++;
		if (c1 == '\0' && c2 == '\0')
			return (H)0;
		if (c1 == '\0')
			return (H)-1;
		if (c2 == '\0')
			return (H)1;
		c1 = (H)(fs_toupper((B)c1) - fs_toupper((B)c2));
		if (c1 != 0)
			return c1;
	}
}

/*******************************************************************
	Time Function
********************************************************************/
static BOOL fs_get_tm(ST_TM *ex_tm)
{
	DATE_INFO date;
	
	get_date(&date);
	get_time(&date);

    ex_tm->tm_sec  = date.second;
    ex_tm->tm_min  = date.minute;
    ex_tm->tm_hour = date.hour;
    ex_tm->tm_mday = date.day;
    ex_tm->tm_mon  = date.month - 1;
    ex_tm->tm_year = date.year + 20;
    ex_tm->tm_wday = 0;
    ex_tm->tm_yday = 0;
    ex_tm->tm_isdst = 0;

    return TRUE;
}

/*******************************************************************
	swap
********************************************************************/
static void set_le_h(void *p, UH d)
{
	UB *pp = (UB *)p;

	*pp++ = (UB)d; d >>= 8;
	*pp   = (UB)d;
}

static UH get_le_h(void *p)
{
	UB *pp = (UB *)p + 1;
	UH d;

	d = *pp--; d <<= 8;
	d |= *pp;
	return d;
}

static void set_le_w(void *p, UW d)
{
	UB *pp = (UB *)p;

	*pp++ = (UB)d; d >>= 8;
	*pp++ = (UB)d; d >>= 8;
	*pp++ = (UB)d; d >>= 8;
	*pp   = (UB)d;
}

static UW get_le_w(void *p)
{
	UB *pp = (UB *)p + 3;
	UW d;

	d  = *pp--; d <<= 8;
	d |= *pp--; d <<= 8;
	d |= *pp--; d <<= 8;
	d |= *pp;
	return d;
}

/*******************************************************************
	string check for dos
	!!now only '\', ':' check
********************************************************************/
static ER chk_str_dos(const B *name)
{
	B *p = name;

	do
	{
		if(*p == '\\' || *p == ':')
			return -1;	/* hit */
	}
	while(*++p != '\0');
	return 0;			/* no include '\' or ':' */
}

/*******************************************************************
	drv -> any info
********************************************************************/
static ER fs_drv(UB drv, ST_DISK **mydd, UW *logic, B **tbl)
{
	ST_DISK *tmpd = disk;
	do
	{
		if(drv == tmpd->name)
		{
			if(mydd  != NULL)
				*mydd  = tmpd;
			if(logic != NULL)
				*logic = (UW)tmpd->logic;
			if(tbl	 != NULL)
				*tbl   = (B*)tmpd->fatp;
			return((ER)0);
		}
		else
			tmpd = tmpd->next;
	}
	while(tmpd != NULL);
	return((ER)0);
}

/*******************************************************************
	write dir_entry.day & .time
********************************************************************/
static void fs_w_time(ST_DIRENTRY *tm_dir)
{
	ST_TM f_tm;
	H dt;

	if (fs_get_tm(&f_tm))
	{
		dt = (H)(f_tm.tm_sec / 2);
		dt |= (H)(f_tm.tm_min << 5);
		dt |= (H)(f_tm.tm_hour << 11);
		set_le_h((void *)&tm_dir->time, (UH)dt);
		dt = (H)f_tm.tm_mday;
		dt |= (H)((f_tm.tm_mon + 1) << 5);
		dt |= (H)((f_tm.tm_year - 80) << 9);
		set_le_h((void *)&tm_dir->date, (UH)dt);
	}
	else
		tm_dir->time = tm_dir->date = (unsigned short)0;
}

/*******************************************************************
	check dir num : "C:\aaa.xxx" = 0 , "C:\ddd\aaa.xxx" = 1 , ...
********************************************************************/
static ER fs_chkdnum(const B *path, UB *num)
{
	UB n = (UB)0;
	UB cnt = (UB)0;

	while(1)
	{
		if(path[n++] == '\\')
			cnt++;
		if(path[n] == '\0')
			break;
	}
	if(!cnt)
		return R_ERR;
	*num = (UB)(cnt - 1);
		return R_OK;
}

/*******************************************************************
	check dir name & file name len
********************************************************************/
static ER fs_chklen(const B *path, UB num)
{
	B *p_path = path;
	UB n = (UB)0;
	H len = (H)0;

	p_path += 2;
	while(*p_path != '\0')
	{
		if(*p_path++ == '\\')
		{
			if(n++ == num)
				break;
			if(len > DIRLEN_MAX)
				return R_ABN_FILE;
			len = (H)0;	/* 00.08.02 */
		}
		else
			len++;
	}
	len = (H)0;
	while(*p_path++ != '\0')
		len++;
	if(len > FILELEN_MAX)
		return R_ABN_FILE;

	return R_OK;
}

/*******************************************************************
	check pathname
********************************************************************/
static ER fs_chkpath(const B *path)
{
	ST_DISK *tmp_d;
	B drv;
	UB num;
	ER er;

	if(path == NULL)
		return R_ABN_FILE;

	er = fs_chkdnum(path, &num);
	if(er != R_OK)
		return R_ABN_FILE;
	if(num > DIRNUM_MAX)
		return R_OVER_DIR_NUM;

	er = fs_chklen(path, num);
	if(er != R_OK)
		return er;

	tmp_d = disk;

	drv = *path;

	while(tmp_d != NULL)
	{
		if(drv == tmp_d->name)
			return R_OK;
		tmp_d = tmp_d->next;
	}
	return R_DRIVE_ERR;
}

/*******************************************************************
	"aaaa.xyz" -> "aaaa    xyz"(8+3)
********************************************************************/
static B fs_fchg(B *to, const B *frm)
{
	UB n,nn;

	fs_memset((void *)to, (B)0x20, (UW)11);
	to[11] = (B)'\0';

	for(n = nn = (UB)0; n < (UB)12; n++,nn++)
	{
		if(0 == frm[n])
			break;
		if('.' == frm[n])
		{
			if(n > 8)
				return (B)R_ABN_FILE;
			nn = (UB)7;
		}
		else
			to[nn] = frm[n];
	}
	return (B)R_OK;
}

/*******************************************************************
	"DDDD" -> "DDDD       "(11)
********************************************************************/
static void fs_dchg(B *to, const B *frm)
{
	B *p_t = to;
	B *p_f = frm;
	UB n;

	fs_memset((void *)to, (B)0x20, (UW)11);
	to[11] = (B)'\0';

	for(n = (UB)0; n < 11; n++)
	{
		if(*p_f == '\0')
			break;
		*p_t++ = *p_f++;
	}
}

/*******************************************************************
	"dddd\eeee" 	-> "dddd       " return 次の&'\\'
	"123456789" 	-> "12345678   "
	"123456789.123" -> NULL
	"123.4567"		-> NULL
	"A.B.C" 		-> NULL
	".A"			-> NULL
********************************************************************/
static B *fs_dget(B *to, const B*frm)
{
	UB name = (UB)0;
	UB attr = (UB)0;
	B *p_t = to;
	B *p_f = frm;

	fs_memset((void *)to, (B)0x20, (UW)11);
	to[11] = (B)'\0';
#if 0
	while(*p_f != '.')
	{
		if(++name > 8)
			return NULL;
		if(*p_f == '\0' || *p_f == '\\')
			return p_f;
		*p_t++ = *p_f++;
	}
#else
	for(;;)
	{
//		if(++name > 8)
		if(++name > 9)		//2003.01.31 M.Maeda ディレクトリ名８文字に対応
			return NULL;
		if(*p_f == '\0' || *p_f == '\\')
			return p_f;
		*p_t++ = *p_f++;
	}
#endif
	if(name == 0)
		return NULL;

	p_f++;

	p_t += (8 - name);

	while(*p_f != '\0' && *p_f != '\\')
	{
		if(++attr > 3)
			return NULL;
		if(*p_f == '.')
			return NULL;
		*p_t++ = *p_f++;
	}
	return p_f;
}

/*******************************************************************
	"C:\dddd\eee\fff.xyz" -> "fff  xyz"(8+3)
********************************************************************/
static ER fs_dfchg(B *path, B *pf)
{
	B *pp = path;
	B *pos;

	while(*pp != '\0')
	{
		if(*pp++ == '\\')
			pos = pp;
	}

	return (ER)fs_fchg(pf, pos);
}

/*******************************************************************
	get dir_name and file_name	from path name
********************************************************************/
static ER fs_getfname(B *path, B *fname, B *flg)
{
	UB len;
	UB n;
	UB m;
	UB pos;

	len = (UB)fs_strlen(path);
	for(n = len, m = (UB)0; n > 0; n--, m++)
	{
		if(path[n] == '\\')
		{
			if(m > 13)				   /* filename Drive:\\Directry\\AAAAAAAA.xxx 8+4+1 */
				return	R_ABN_FILE;
			break;
		}
	}
	pos = n;
	n++;
	for(; n<len; n++)
		*fname++ = path[n];
	*fname = (B)0;
	if(pos == 0)
		*flg = (B)0;
	else
		*flg = (B)1;
	return R_OK;
}

/*******************************************************************
	Set Directry or File Data
********************************************************************/
static void  fs_set_dfdat(ST_DIRENTRY *d_ent, B *name, B attr, UH clusno, UW size)
{
	fs_memset((void *)&d_ent->name, (B)0, (UW)DIRENTRY_SZ);
	fs_memcpy((void *)&d_ent->name, (void *)name, (UW)11);
	d_ent->attr = attr;
	fs_w_time(d_ent);
	set_le_h((void *)&d_ent->clusno, clusno);
	set_le_w((void *)&d_ent->size, (UW)size);
}

/*******************************************************************
	Read Sector
********************************************************************/
static ER fs_r_sect(ST_DISK *d, UW sectno, UB *buf, UH num)
{
	d->ercd = (*d->func)(d, (short)TFN_READ_SECTOR, sectno, (void *)buf, num);
	d->p = buf;
	d->n = sectno;
	return (ER)d->ercd;
}

/*******************************************************************
	Write Sector
********************************************************************/
static ER fs_w_sect(ST_DISK *d, UW sectno, UB *buf, UH num)
{
	ER r;

	d->n = (UW)-1;
	r = (*d->func)(d, (short)TFN_WRITE_SECTOR, sectno, (void *)buf, num);
	return r;
}

/*******************************************************************
	convert filename
********************************************************************/
static B *fs_conv_fname(const B *path, B *name)
{
	H n;
	B c;
	B dot = (B)0;

	for(n = (H)0; n < 11;)
	{
		c = *path++;
		if(c == '\\')
			return NULL;
		if(c == ' ')
			continue;
		else if(c == '\0')
		{
			for(; n < 11; )
				name[n++] = (B)' ';
			break;
		}
		else if(c == '.')
		{
			if(dot)
				return NULL;
			dot = (B)1;
			for(; n < 8; )
				name[n++] = (B)' ';
		}
		else
		{
			if(n >= 8 && !dot)
				return NULL;
			name[n++] = c;
		}
	}
	return name;
}

/*******************************************************************
	filename to path
********************************************************************/
static B *fs_name_to_path(B attr, const UB *name, B *path)
{
	B *p;
	UB c;
	H n;

	p = path;
	if(attr == ATTR_DIR)
	{
		for(n = (H)0; n < 11; n++)
			*p++ = name[n];
		return	path;
	}
	for(n = (H)0; n < 11; )
	{
		if(n == 8)
			*p++ = (B)'.';
		c = name[n];
		if(c == ' ')
		{
			if(n >= 8)
				break;
			n = (H)8;
		}
		else
		{
			*p++ = (B)c;
			n++;
		}
	}
	if(*(p - 1) == '.')
		p--;
	*p = (B)'\0';
	return path;
}

/*******************************************************************
	dir entry -> tm format (time)
********************************************************************/
static B fs_dent_to_tm(ST_TM *my_tm, UH time, UH date)
{
	UH le_time = get_le_h((void *)&time);
	UH le_date = get_le_h((void *)&date);

	my_tm->tm_sec  = (le_time & 0x001F) * 2;
	my_tm->tm_min  = (le_time >> 5) & 0x003F;
	my_tm->tm_hour = le_time >> 11;

	my_tm->tm_mday = (le_date & 0x001F);
	my_tm->tm_mon  = ((le_date >> 5) & 0x000F) - 1;
	my_tm->tm_year = (le_date >> 9) + 80;

	my_tm->tm_wday = 0;
	my_tm->tm_yday = 0;
	my_tm->tm_isdst = 0;

	return (B)R_OK;
}

/*******************************************************************
	cluster to sector
********************************************************************/
static UW cluster_to_sector(ST_DISK *d, UH c_no)
{
	return (UW)(c_no - 2) * d->sc + (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1));
}

/*******************************************************************
	sector to cluster
********************************************************************/
static UH sector_to_cluster(ST_DISK *d, UW s_no)
{
	return (UH)((s_no - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) / d->sc + 2);
}

/*******************************************************************
	make subdir data
********************************************************************/
static ER fs_mk_subd(ST_DISK *d, UH c_no, UH rootno)
{
	ER	er;
	ST_DIRENTRY  d_ent;
	ST_DIRENTRY *p;
	B  wname[11];
	UW	secno = cluster_to_sector(d, c_no);

	er = fs_r_sect(d, secno, d->r_buf, (UH)1);
	if(er != R_OK)
		return er;
	fs_memset((void *)d->r_buf, (B)0x00, (UW)SECT_SZ);
	p = (ST_DIRENTRY *)d->r_buf;
	fs_memset((void *)wname, (B)0x20, (UW)11);
	wname[0] = (B)'.';
	fs_set_dfdat(&d_ent, wname, (B)ATTR_DIR, (UH)c_no, (UW)0);
	fs_memmove((void *)p, (void *)&d_ent, (UW)DIRENTRY_SZ);
	p++;
	wname[1] = (B)'.';
	fs_set_dfdat( &d_ent, wname, (B)ATTR_DIR, rootno, (UW)0 );
	fs_memmove( (void *)p, (void *)&d_ent, (UW)DIRENTRY_SZ);
	return	fs_w_sect(d, secno, d->r_buf, (UH)1);
}

/*******************************************************************
 Read FAT Sector
********************************************************************/
static ER read_fat(ST_DISK *d, int fatno, UB **buf)
{

	if(fatno >= (int)d->sf)
		return R_DISK_FULL;

	if( d->ram ) {
		*buf = (UB *)(d->fatp + (fatno * SECT_SZ));
		return R_OK;
	}
#ifdef G_FAT
	p = d->fatp + (fatno * SECT_SZ);
	fs_memcpy((void *) *buf, (void *)p, (UW)SECT_SZ);
	return R_OK;
#else
	fatno += d->rsc;
	return fs_r_sect(d, (UW)fatno, *buf, (UH)1);
#endif
}

/*******************************************************************
 Write FAT Sector
********************************************************************/
static ER write_fat(ST_DISK *d, int fatno, UB *buf)
{
	ER err;

	if (fatno >= d->sf)
		return R_DISK_FULL;

	if( d->ram ) {
		return R_OK;
	}
#ifdef G_FAT
	p = d->fatp + (fatno * SECT_SZ);
	if(p != ((B*)buf))
		fs_memcpy((void *)p, (void *)buf, (UW)SECT_SZ);
	return R_OK;
#else
	fatno += d->rsc;

//	return	fs_w_sect(d, (UW)fatno, buf, (UH)1);
	err = fs_w_sect(d, (UW)fatno, buf, (UH)1);
	if(d->fn == 2){
		err = fs_w_sect(d, (UW)fatno + d->sf, buf, (UH)1);
	}
	return err;
#endif
}

/*******************************************************************
	Read Root Directry Sector
********************************************************************/
static ER read_dir(ST_DISK *d, H dirno, UB *buf)
{
	dirno += (H)(d->rsc + d->sf * d->fn);
	if (dirno >= (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1)))
		return R_DISK_FULL;

	return fs_r_sect(d, (UW)dirno, buf, (UH)1);
}

/*******************************************************************
	Write Root Directry Sector
********************************************************************/
static ER write_dir(ST_DISK *d, H dirno, UB *buf)
{
	dirno += (H)(d->rsc + d->sf * d->fn);
	if (dirno >= (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1)))
		return R_DISK_FULL;

	return fs_w_sect(d, (UW)dirno, buf, (UH)1);
}

/*******************************************************************
	Read/Write a FAT Entry(G_FAT)
********************************************************************/
static UH gf_sub_rw_fat(ST_DISK *d, UH c_no, UH newno, B mode)
{
	int n, m;
	UH oldno;
	UB b[2];
	UB *p;

	if(d->fatbits == FAT16)
	{
		m = c_no / (SECT_SZ / 2);
		n = c_no % (SECT_SZ / 2) * 2;
	}
	else
	{
		m = (c_no * 3) / (SECT_SZ * 2);
		n = (c_no * 3) / 2 - SECT_SZ * m;
	}
	p = (UB*)(d->fatp + (SECT_SZ * m));

	if(d->fatbits == FAT16)
	{
		oldno = get_le_h((void *)(p + n));
		if (mode == 'w')
		{
			set_le_h((void *)(p + n), newno);
		}
	}
	else
	{
		b[0] = *(p + n);
		if(n == SECT_SZ - 1)
		{
			n = -1;
			p = (UB*)(d->fatp + (SECT_SZ * ++m));
		}
		b[1] = *(p + n + 1);
		oldno = get_le_h((void *)b);
		if(!(c_no & 1))
		{
			if(mode == 'w')
				set_le_h((void *)b, (UH)((oldno & 0xF000) | (newno & 0x0FFF)));
		}
		else
		{
			if(mode == 'w')
				set_le_h((void *)b, (UH)((oldno & 0x000F) | (newno << 4)));
			oldno >>= 4;
		}
		if(mode == 'w')
		{
			*(p + n + 1) = b[1];
			if(n != -1)
				*(p + n) = b[0];
			else
			{
				p = (UB*)(d->fatp + (SECT_SZ * --m));
				*(p + SECT_SZ - 1) = b[0];
			}
		}
		oldno &= 0x0FFF;
		if(oldno >= 0x0FF7)
			oldno |= 0xF000;
	}
	if(oldno >= 0xFFF8)
		oldno = (UH)0xFFFF;
	return oldno;
}

/*******************************************************************
	Read/Write a FAT Entry
********************************************************************/
static UH chg_fat_entry(ST_DISK *d, UH c_no, UH newno, B mode)
{
	int n, m;
	UH oldno;
	UB b[2];
	UB *fat = d->r_buf;
	ER er;

	if( d->ram ) {
		return gf_sub_rw_fat(d, c_no, newno, mode);
	}
#ifdef G_FAT
	return gf_sub_rw_fat(d, c_no, newno, mode);
#endif

	if(d->fatbits == FAT16)
	{
		m = c_no / (SECT_SZ / 2);
		n = c_no % (SECT_SZ / 2) * 2;
	}
	else
	{
		m = (c_no * 3) / (SECT_SZ * 2);
		n = (c_no * 3) / 2 - SECT_SZ * m;
	}
	er = read_fat(d, m, &fat );
	if(er != R_OK)
		return (UH)0xFFF8;	  /* Error */

	if(d->fatbits == FAT16)
	{
		oldno = get_le_h((void *)&fat[n]);
		if (mode == 'w') {
			set_le_h((void *)&fat[n], newno);
			write_fat(d, m, fat);
		}
	}
	else
	{
		b[0] = fat[n];
		if(n == SECT_SZ - 1)
		{
			n = -1;
			er = read_fat(d, ++m, &fat);
			if(er != R_OK)
				return (UH)0xFFF8;						/* Error */
		}
		b[1] = fat[n + 1];
		oldno = get_le_h((void *)b);
		if(!(c_no & 1))
		{
			if(mode == 'w')
				set_le_h((void *)b, (UH)((oldno & 0xF000) | (newno & 0x0FFF)));
		}
		else
		{
			if(mode == 'w')
				set_le_h((void *)b, (UH)((oldno & 0x000F) | (newno << 4)));
			oldno >>= 4;
		}
		if(mode == 'w')
		{
			fat[n + 1] = b[1];
			if (n != -1)
				fat[n] = b[0];
			else
			{
				write_fat(d, m, fat);
				read_fat(d, --m, &fat);
				fat[SECT_SZ - 1] = b[0];
			}
			write_fat(d, m, fat);
		}
		oldno &= 0x0FFF;
		if(oldno >= 0x0FF7)
			oldno |= 0xF000;
	}
	if(oldno >= 0xFFF8)
		oldno = (UH)0xFFFF;
	return oldno;
}

/*******************************************************************
	next cluster number
********************************************************************/
static UH next_cluster(ST_DISK *d, UH c_no)
{
	UH n;

	n = chg_fat_entry(d, c_no, (UH)0, (B)'r');
	if(n >= 0xFFF7)
		n = (UH)0;
	return n;
}

/*******************************************************************
	delete cluster
********************************************************************/
static UH del_cluster(ST_DISK *d, UH c_no)
{
	UH n;

	n = chg_fat_entry(d, c_no, (UH)0, (B)'w');
	if(n >= 0xFFF7)
		n = (UH)0;

/*	  nullfat = c_no;*/

	if( d->nullfat > c_no ) {
		d->nullfat = c_no;
	}

	return n;
}

/*******************************************************************
	next sector number
********************************************************************/
static UW next_sector(ST_DISK *d, UW sectno)
{
	UH n;

	if(d->sc > 1)
	{
		if((sectno + 1 - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) % d->sc != 0)
			return sectno + 1;
	}
	n = sector_to_cluster(d, sectno);
	n = next_cluster(d, n);
	if(n == 0)
		return (UW)0L;
	return cluster_to_sector(d, n);
}

/*******************************************************************
	allocate new cluster
********************************************************************/
static UH new_cluster(ST_DISK *d, UH c_no)
{
	UH n, nn, end;

	if(d->fatbits == FAT16)
		end = (UH)((SECT_SZ / 2) * d->sf);
	else
		end = (UH)((SECT_SZ * d->sf * 2) / 3);
	if(end > (UH)((d->ts - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) / d->sc))
		end = (UH)((d->ts - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) / d->sc);
#if 1
	for(n = d->nullfat; n < end; n++)
#else
	for(n = 2; n < end; n++)
#endif
	{
		if(chg_fat_entry(d, n, (UH)0, (B)'r') == (UH)0)
		{
			d->nullfat = (unsigned short)(n + 1);
			nn = chg_fat_entry(d, n, (UH)0xFFFF, (B)'w');
			if (nn != 0)
				break;
			if(c_no != 0)
			{
				nn = chg_fat_entry(d, c_no, n, (B)'w');
				if(nn == 0xFFF8)
					break;
			}
			return n;
		}
	}
	return (UH)0;
}

/*******************************************************************
	allocate new sector
********************************************************************/
static UW new_sector(ST_DISK *d, UW sectno)
{
	UH n;

	if(d->sc > 1)
	{
		if((sectno + 1 - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) % d->sc != 0)
			return sectno + 1;
	}
	n = sector_to_cluster(d, sectno);
	n = new_cluster(d, n);
	if (n == 0)
		return (UW)0L;
	return cluster_to_sector(d, n);
}

/*******************************************************************
	Search Directry Entry by Name
********************************************************************/
static ER find_dir_entry(ST_DISK *d, H *entrno, ST_DIRENTRY *d_ent, const B *name, B hnt)
{
	ST_DIRENTRY *p;
	H n;
	ER er;

	for(n = (H)0; n < d->rde; n++, p++)
	{
		if(n % DIR_ENTRYS_PER_SECTOR == 0)
		{
			er = read_dir(d, (H)(n / DIR_ENTRYS_PER_SECTOR), d->r_buf);
			if(er != R_OK)
				return er;
			p = (ST_DIRENTRY *)d->r_buf;
		}
		if(name == NULL)
		{
			if(p->name[0] == 0x00 || p->name[0] == 0xE5)
				break;
		}
		else
		{
			if(p->name[0] == 0x00)	  /* End of Entry Table */
			{
				n = (H)-1;
				break;
			}

			if (fs_memcmp((void *)p->name, (void *)name, (UW)11) == 0)
			{
				if((p->attr & ATTR_DIR) == hnt)    /* attribute */
					break;
			}
		}
	}
	/* ルートディレクトリエントリ数に達した */
	if(n == d->rde)
		*entrno = (H)-1;
	else
	{
		*entrno = n;
		fs_memcpy((void *)d_ent, (void *)p, (UW)DIRENTRY_SZ);
	}
	return R_OK;
}

/*******************************************************************
	Delete Directry Entry
********************************************************************/
static ER del_dir_entry(ST_DISK *d, H entrno)
{
	ST_DIRENTRY *p;
	UH n;
	H m;
	ER er;

	m = (H)(entrno / DIR_ENTRYS_PER_SECTOR);
	er = read_dir(d, m, d->r_buf);
	if(er != R_OK)
		return er;
	p = (ST_DIRENTRY *)d->r_buf + entrno % DIR_ENTRYS_PER_SECTOR;
	n = get_le_h((void *)&p->clusno);
	p->name[0] = (unsigned char)0xE5;
	er = write_dir(d, m, d->r_buf);
	if (er != R_OK)
		return er;
	while (n != 0)
		n = del_cluster(d, n);
	return R_OK;
}

/*******************************************************************
	Set Directry Entry
********************************************************************/
static ER set_dir_entry(ST_DISK *d, H entrno, ST_DIRENTRY *d_ent)
{
	ST_DIRENTRY *p;
	H m;
	ER er;

	m = (H)(entrno / DIR_ENTRYS_PER_SECTOR);
	er = read_dir(d, m, d->r_buf);
	if (er != R_OK)
		return er;
	fs_w_time(d_ent);
	p = (ST_DIRENTRY *)d->r_buf + entrno % DIR_ENTRYS_PER_SECTOR;
	fs_memcpy((void *)p, (void *)d_ent, (UW)DIRENTRY_SZ);
	return write_dir(d, m, d->r_buf);
}

/*******************************************************************
	Set file size of directry entry
********************************************************************/
static ER set_filesize(ST_DISK *d, B directry, H secno, H entrno, UW size)
{
	ST_DIRENTRY *d_ent;
	H m;
	ER er;

	if(directry == 0)
	{
		m = (H)(entrno / DIR_ENTRYS_PER_SECTOR);
		er = read_dir(d, m, d->r_buf);
		if(er != R_OK)
		   return er;
		d_ent = (ST_DIRENTRY *)d->r_buf + entrno % DIR_ENTRYS_PER_SECTOR;
		set_le_w((void *)&d_ent->size, size);
		return write_dir(d, m, d->r_buf);
	}
	else
	{
		er = fs_r_sect(d, (UW)secno, d->r_buf, (UH)1);
		d_ent = (ST_DIRENTRY *)d->r_buf + entrno % DIR_ENTRYS_PER_SECTOR;
		set_le_w((void *)&d_ent->size, size);
		return fs_w_sect(d, (UW)secno, d->r_buf, (UH)1);
	}
}

/*******************************************************************
	Make Directry
********************************************************************/
static ER make_entdir(ST_DISK *d, B *name)
{
	ST_DIRENTRY d_ent;
	ER er;
	UH c_no;
	H entrno;

	/* 00.08.22 same name dir check */
	er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)ATTR_DIR);
	if(er != R_OK || entrno != -1)
		return R_ERR;

	er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, (B *)NULL, (B)ATTR_DIR);
	if(er != R_OK)
		return er;
	if(entrno == -1)
		return -1;
	c_no = new_cluster(d, (UH)0);	 /* get data area */
	if(c_no == 0)
		return	-1;
	fs_set_dfdat(&d_ent, name, (B)ATTR_DIR, c_no, (UW)0);
	er = set_dir_entry(d, entrno, &d_ent);
	if(er != R_OK)
		return er;
	return fs_mk_subd(d, c_no, (UH)0);	  /* make sub directry */
}

/*******************************************************************
	Seach Sub Directry
********************************************************************/
static ER find_sub_entry(ST_DISK *d, H *entrno, ST_DIRENTRY *d_ent, const B *name, UW *rootsec, B hnt)
{
	ST_DIRENTRY *p;
	ER er;
	H  n;
	UW secno = *rootsec;

	for(;;)
	{
		er = fs_r_sect(d, secno, d->r_buf, (UH)1);
		if(er != R_OK)
			return er;
		p = (ST_DIRENTRY *)d->r_buf;
		for(n = (H)0; n < DIR_ENTRYS_PER_SECTOR; n++, p++)
		{
			if(name == NULL)
			{
				if(p->name[0] == 0x00 || p->name[0] == 0xE5 || p->name[0] == 0xF6 )/* 0xf6 windows format */
					break;
			}
			else
			{
				if(p->name[0] == 0x00)
				{
					n = (H)-1;
					break;
				}
				if(fs_memcmp((void *)p->name, (void *)name, (UW)11) == 0)
				{
					if((p->attr & ATTR_DIR) == hnt) 	/* attribute */
						break;
				}
			}
		}
		if(n == DIR_ENTRYS_PER_SECTOR)
		{
			secno = next_sector(d, secno);
			if(secno == 0)
			{
				if(name != NULL)
				{
					n = (H)-1;
					break;
				}
			}
			else
				*rootsec = secno;
		}
		else
			break;
	}
	*entrno = n;
	fs_memcpy((void *)d_ent, (void *)p, (UW)DIRENTRY_SZ);
	return	R_OK;
}

/*******************************************************************
	Make Sub Directry
********************************************************************/
static ER make_subdir(ST_DISK *d, B *name, UH rootno, B mode, H *subentno, UH *subclusno, UW *topsec)
{
	ST_DIRENTRY  d_ent;
	ST_DIRENTRY *p;
	ER er;
	H  n;
	UH clusno = rootno;
	UH wsec, secno = (UH)cluster_to_sector(d, clusno);

	for(;;)
	{
		er = fs_r_sect(d, (UW)secno, d->r_buf, (UH)1);		   /* find open area */
		if (er != R_OK)
			return er;
		p = (ST_DIRENTRY *)d->r_buf;
		for (n = (H)0; n < DIR_ENTRYS_PER_SECTOR; n++, p++)
		{
			if((p->name[0] == 0x00) ||(p->name[0] ==0xE5) || (p->name[0] == 0xF6) ) /* 0xf6 windows format */
				break;
		}
		if(n == DIR_ENTRYS_PER_SECTOR)
		{
			wsec = secno;							/* next chain	  */
			secno = (UH)next_sector(d, (UW)secno);
			if(secno == 0)
			{
				secno = (UH)new_sector(d, (UW)wsec);	  /* allocate chain */
				if(secno == 0)
					return R_ERR;
			}
			*topsec = secno;
		}
		else
			break;
	}
	clusno = new_cluster(d, (UH)0); 				  /* get sector area for data */
	if(clusno == 0)
		return	R_ERR;
	if(mode == 1)
	{
		er = fs_mk_subd(d, clusno, rootno);
		if(er != R_OK)
			return er;
	}
	er = fs_r_sect(d, (UW)secno, d->r_buf, (UH)1);			   /* make directry or file */
	if (er != R_OK)
		return er;
	p = (ST_DIRENTRY *)&d->r_buf[n*DIRENTRY_SZ];

	if(mode == 0)								  /* file	  */
		fs_set_dfdat(&d_ent, name, (B)ATTR_FILE, (UH)clusno, (UW)0);
	else											/* directry */
		fs_set_dfdat(&d_ent, name, (B)ATTR_DIR, (UH)clusno, (UW)0);
	fs_memmove((void *)p, (void *)&d_ent, (UW)DIRENTRY_SZ);
	*subentno = n;
	*subclusno = clusno;
	return fs_w_sect(d, (UW)secno, d->r_buf, (UH)1);
}

/*******************************************************************
	Delete Sub Directry
********************************************************************/
static ER del_mine( ST_DISK *d, UW secno, UH entrno )
{
	ER er;
	UH n;

	if(secno == 0)
	{
		er = read_dir(d, (H)(entrno / DIR_ENTRYS_PER_SECTOR), d->r_buf);
		if(er != R_OK)
			return er;
		n = (UH)(entrno % DIR_ENTRYS_PER_SECTOR);
		fs_memset((void *)&d->r_buf[n*DIRENTRY_SZ], (B)0xE5, (UW)1);
		return write_dir(d, (H)(entrno / DIR_ENTRYS_PER_SECTOR), d->r_buf);
	}
	else
	{
		er = fs_r_sect(d, secno, d->r_buf, (UH)1);
		if(er != R_OK)
			return er;
		fs_memset((void *)&d->r_buf[entrno*DIRENTRY_SZ], (B)0xE5, (UW)1);
		return fs_w_sect(d, secno, d->r_buf, (UH)1);
	}
}

/*******************************************************************
	Delete Sub Directry
********************************************************************/
static ER del_dirnum(ST_DISK *d, UH rootno)
{
	ST_DIRENTRY *p;
	ER	er;
	UH	c_no = rootno;
	UW	secno = cluster_to_sector(d, rootno);
	B	set = (B)(DIRENTRY_SZ * 2);			   /* cut mine ond parent */
	H n = (H)2;
	int flag = 0;

	for(;;)
	{									  /* find */
		er = fs_r_sect(d, secno, d->r_buf, (UH)1);
		if(er != R_OK)
			return er;
		p = (ST_DIRENTRY *)&d->r_buf[set];
		for ( ; n < DIR_ENTRYS_PER_SECTOR; n++, p++)
		{
//			if((p->name[0] == 0) || (p->name[0] == 0xE5) || (p->name[0]==0xF6)) /* 0xf6 windows format */
			if((p->name[0] == 0xE5) || (p->name[0]==0xF6)) /* 0xf6 windows format */
				continue;
			if(p->name[0] == 0){
				flag = 1;
				break;
			}else{
				return -1;
			}
		}
		if(flag != 0){
			break;
		}
		n = (H)0;
		set = (B)0;
		secno = next_sector(d, secno);
		if(secno == 0)
			break;
	}
	while(c_no != 0)							/* clear fat_chain and sector data */
		c_no = del_cluster(d, c_no);
	return R_OK;
}

/*******************************************************************
	search directry
********************************************************************/
static ER seek_dir(ST_DISK *d, B *dirname, UH *clus)
{
	ST_DIRENTRY d_ent;
	ER er;
	H  entrno;
	UW rootsec = 0, tmp = 0;
	UH rootno;
	B name[12];
	UB num;
	UB n;
	B *dpos;

	fs_chkdnum(dirname, &num);

	dpos = dirname+2;
	for(n=(UB)0; n<num; n++)
	{
		dpos = fs_dget(name, ++dpos);
		if(dpos == NULL)
			return R_ABN_FILE;

		if(n == 0)
			er = find_dir_entry( d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)ATTR_DIR);		   /* root			 */
		else {
			tmp = rootsec;
			er = find_sub_entry( d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)ATTR_DIR);/* not root entry */
			if(er == R_OK)
			{
				if( (entrno != -1) && (tmp != rootsec) )
					tmp = rootsec;
				rootno = sector_to_cluster( d, rootsec );
			}
		}
		if(er != R_OK)
			return	R_ERR;
		if(entrno == -1)
			return	R_ERR;
		else
		{
			rootno = get_le_h((void *)&(d_ent.clusno));
			rootsec = cluster_to_sector(d, rootno);
		}
	}
	*clus = rootno;
	return R_OK;
}

/*******************************************************************
	Make Directry List
********************************************************************/
static ER make_dir_list(ST_FILE *f, B *dirname)
{
	ST_DIRENTRY *d_ent = NULL;
	ST_DISK *d;
	struct direct *p;
	H n, i, ii;
	ER er;
	UH clusno, nn;
	UW secno;
	UB dnum;

	d = f->d;
	p = (struct direct *)f->buf;
	fs_memset((void *)p, (B)0, (UW)SECT_SZ);
	ii = (H)(SECT_SZ / sizeof (struct direct));
	n = f->entrno;

	fs_chkdnum(dirname, &dnum);

	if(dnum == 0)
	{
		for(i = (H)0; n < d->rde || i < ii; n++, d_ent++, p++, i++)
		{
			if(n % DIR_ENTRYS_PER_SECTOR == 0)
			{
				er = read_dir(d, (H)(n / DIR_ENTRYS_PER_SECTOR), d->r_buf);
				if (er != R_OK)
					return er;
				d_ent = (ST_DIRENTRY *)d->r_buf;
			}
			if (d_ent->name[0] == 0x00) /* End of Entry Table */
				break;
			fs_name_to_path(d_ent->attr, d_ent->name, p->d_name);
			if (d_ent->name[0] == 0xE5)
				p->d_name[0] = (char)'?';
			else
				p->d_ino = (short)(0x1000 * d->ch + n + 1);
		}
	}
	else
	{
		er = seek_dir(d, dirname, &clusno);
		if(er != R_OK)
			return er;
		secno = cluster_to_sector(d, clusno);
		for(;;)
		{
			er = fs_r_sect(d, secno, d->r_buf, (UH)1);
			if (er != R_OK)
				return er;
			d_ent = (ST_DIRENTRY *)d->r_buf;
			for (nn=(UH)0,i=(H)0; (nn < DIR_ENTRYS_PER_SECTOR) || (i < ii); nn++, n++, d_ent++, p++, i++ ){
				if(nn >= DIR_ENTRYS_PER_SECTOR)
					break;
				if(d_ent->name[0] == 0x00)
				{
					nn = (UH)0;
					break;
				}
				fs_name_to_path(d_ent->attr, d_ent->name, p->d_name);
				if(d_ent->name[0] == 0xE5)
					p->d_name[0] = (char)'?';
				else
					p->d_ino = (short)(0x1000 * d->ch + n + 1);
			}
			if(nn == 0)
				break;
			if(nn >= DIR_ENTRYS_PER_SECTOR)
			{
				secno = next_sector( d, secno );	/* next chain */
				if(secno == 0)
					break;
			}
		}
	}
	f->entrno = n;
	f->size += n * sizeof(struct direct);
	return R_OK;
}

/*******************************************************************
	Static Function for Filesystem Library
********************************************************************/
/*******************************************************************
	for fseek
********************************************************************/
static ER fs_fseek(ST_FILE *f, W offset, B type)
{
	ST_DISK *d;
	ER er;
	B flg = (B)0;
	W n, m, i;
	W pos;

	if	   (type == SEEK_SET)
		pos = offset;
	else if(type == SEEK_CUR)
		pos = offset + f->pos;
	else if(type == SEEK_END)
		pos = offset + f->size;

	if(pos < 0 || pos > f->size)
		return EOF;
	if (f->stat == 0)
		return EOF;

	/* ディスクパラメータブロックの取り出し */
	d = f->d;

	/* シークするセクタ数を計算 */
	if( pos ) {
		m = (W)((pos-1) / SECT_SZ - f->pos / SECT_SZ);
	}
	else {
		m = (W)(pos / SECT_SZ - f->pos / SECT_SZ);
	}
	/* 2001/2/21 sakamoto										*/
	/* セクタ内のオフセットがセクタ境界で、後ろにシークする時	*/
	/* はＦＡＴの次のセクタをシークする必要がある				*/
	if( f->byteno == SECT_SZ && pos > f->pos ) {
		m++;
	}

	/* カレントのセクタ外へのシークか現在位置へのシークか */
	if(m != 0L || (pos == f->pos)) {

		/* シークセクタが変更されていれば書き込んでおく */
		if(f->secchg) {
			er = fs_w_sect(d, (UW)f->sectno, (UB *)f->buf, (UH)1);
			if(er != R_OK)
				return EOF;
			f->secchg = (short)0;
		}
		/* オフセットが正の時はシーク時はカレント物理セクタ番号をセット */
		if(m > 0L)
			n = f->sectno;
		/* オフセットが負の時は先頭セクタから*/
		else {
			if( pos ) {
				m = (pos-1) / SECT_SZ;
			}
			else {
				m = pos / SECT_SZ;
			}
			n = cluster_to_sector(d, f->start);
			f->sectno = n;
		}
		/* セクタのサーチ */
		for(i = 0L; i < m; i++) {
			n = next_sector(d, (UW)n);
			if(n == 0) {
				f->pos = f->size;
				f->byteno = (unsigned short)SECT_SZ;
				return EOF;
			}
			flg=(B)1;
			/* 物理シークセクタ番号の設定 */
			f->sectno = n;
		}
		/* セクタキャッシュの更新 */
		er = fs_r_sect(d, (UW)n, (UB *)f->buf, (UH)1);
		if(er != R_OK)
			return EOF;
	}

	/* シークオフセットの設定 */
	f->pos = pos;

	/* 2001/2/22 sakamoto								*/
	/* シークセクタからのオフセットの設定 				*/
	/* セクタ境界へのシークは前のセクタへのシークとする	*/
	if( pos % SECT_SZ || pos == 0 ) {
		f->byteno = (UH)(pos % SECT_SZ);
	}
	else {
		f->byteno = (UH)SECT_SZ;
	}
	/* シークセクタからのオフセットが０でファイル終端の時 */
	if (f->byteno == 0 && pos == f->size && pos != 0){
		if(flg==0) {
			f->byteno = (UH)SECT_SZ;
		}
		else {
			f->byteno = (UH)0;
		}
	}
	return 0;
}

/*******************************************************************
	for ftell
********************************************************************/
static W fs_ftell(ST_FILE *f)
{
	if(f == NULL)
		return (W)EOF;

	if (f->stat == 0)
		return (W)EOF;

	return (W)f->pos;

}

/*******************************************************************
	for feof
********************************************************************/
static B fs_feof(ST_FILE *f)
{
	if(f == NULL)
		return (B)-1;

	if(f->stat == 0)
		return (B)-1;

	if(f->pos == f->size)
		return (B)1;
	else
		return (B)0;
}

static void sb_set_f(ST_FILE *f, UH entry_no, UH clust_no, UB dnum, ST_DISK *d)
{
	f->byteno = (UH)0;
	f->pos = 0L;
	f->entrno = entry_no;
	f->start = clust_no;
	f->sectno = cluster_to_sector(d, f->start);
	if(dnum == 0)
		f->dir = (char)0;
	else
		f->dir = (char)1;
}

/*******************************************************************
	for fopen
********************************************************************/
static ST_FILE *fs_fopen(const B *path, const B *mode)
{
	ST_DISK *d = disk;
	ST_DIRENTRY d_ent;
	ER er;
	H entrno;
	B name[11];
	UH	clusno, workclus;
	UW	rootsec = 0, topsec;
	UB dnum;
	UB f_chk = (UB)0;
	ST_FILE *f = file;
	H n;

	/* ファイルパスの検査 */
	er = fs_chkpath(path);
	if(er != R_OK)
		return NULL;

	/* オープン可能ファイル数の検査 */
	for(n = (H)0; n < NFILE; n++, f++) {
		/* ファイル制御ブロックに空きがある */
		if( !f->stat ) {
			f_chk = (UB)1;
			break;
		}
		else {
			/* すでにオープンされているか検査 */
			if( !fs_stricmp(f->name, path)) {
				f_chk = (UB)0;
				break;
			}
		}
	}
	/* すでにオープンされているか、最大オープン可能ファイル数を越えればエラー */
	if(!f_chk) {
		return NULL;
	}
	/* ファイル名をファイル制御ブロックにコピー */
	fs_strncpy(f->name, path, (UW)FILENAME_MAX);

	/* ドライブ名の検査 */
	while(d->name != f->name[0]) {
		d = d->next;
		if(d == NULL) {
			return NULL;
		}
	}
	/* ファイルパスの形式をディレクトリエントリのファイル形式へ変換 */
	er = fs_dfchg(f->name, name);
	if(er != R_OK) {
		return NULL;
	}
	/* ドライブ情報をコピー */
	f->d = d;

	/* ルートディレクトリのファイルか検査 */
	fs_chkdnum(f->name, &dnum);

	/* ルートディレクトリからエントリを取得 */
	if(dnum == 0) {
		if( find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)0) != R_OK) {
			return NULL;
		}
	}
	/* サブディレクトリからエントリを取得 */
	else {											/* directry and file */
		if( seek_dir(d, f->name, &clusno ) != R_OK ) {
			return	NULL;
		}
		workclus = clusno;
		rootsec = cluster_to_sector(d, clusno);
		er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)0);
		clusno = sector_to_cluster(d, rootsec);
		if(er != R_OK) {
			return NULL;
		}
		if(entrno != -1) {
			workclus = clusno;
		}
	}

	/* 書き込みオープンモードか検査 */
	if (mode[0] == 'w' || mode[0] == 'a') {
		if(entrno != -1) {
			/* 書き込みモード'W'の場合はファイル内容を破棄 */
			if(mode[0] == 'w') {
				if(dnum == 0) {
					er = del_dir_entry(d, entrno);
					if(er != R_OK) {
						return NULL;
					}
				}
				else {
					clusno = get_le_h((void *) &(d_ent.clusno) );
					while( clusno != 0 )
						clusno = del_cluster( d, clusno );
					er = del_mine( d, rootsec, (UH)entrno );
					if(er != R_OK) {
						return NULL;
					}
				}
			}
			/* 書き込みモード'A'の場合はファイル先頭にシークし終了 */
			else {
				f->motsec = rootsec;
				clusno = get_le_h((void *)&d_ent.clusno);
				er = fs_r_sect(d, cluster_to_sector(d, clusno), (UB *)f->buf, (UH)1);
				if (er != R_OK) {
					return NULL;
				}
				f->stat = (char)FS_APND;	/* 00.08.22 FS_RDWR-> */
				f->size = get_le_w((void *)&d_ent.size);
				sb_set_f(f, (UH)entrno, clusno, dnum, d);
				fs_fseek(f, (W)f->size, (B)SEEK_SET);
				return f;
			}
		}
		/* ルートディレクトリ */
		if(dnum == 0) {
			/* ディレクトリエントリの取り出し */
			if( find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, (B *)NULL, (B)0) != R_OK ) {
				return NULL;
			}
			/* エントリがなければエラー */
			if( entrno == -1 ) {
				return NULL;
			}
			/* 先頭クラスタ取得 */
			clusno = new_cluster(d, (UH)0);
			if (clusno == 0) {
				return NULL;
			}
			/* 新エントリ情報作成 */
			fs_memset((void *) &d_ent, (B)0, (UW)DIRENTRY_SZ );
			fs_memcpy((void *)d_ent.name, (void *)name, (UW)11);
			set_le_h((void *)&d_ent.clusno, clusno);
			d_ent.attr = (char)ATTR_FILE;

			/* 新エントリを書き込み */
			er = set_dir_entry(d, entrno, &d_ent);
			if (er != R_OK)
				return NULL;
		}
		/* サブディレクトリ */
		else {
			topsec = cluster_to_sector( d, workclus );
			er = make_subdir(d, name, workclus, (B)0, (H *)&entrno, (UH *)&clusno, (UW *)&topsec);
			f->motsec = topsec;
			if (er != R_OK)
				return NULL;
		}

		fs_memset((void *)f->buf, (B)0, (UW)SECT_SZ);
		f->stat = (char)FS_WR;
		if(mode[1] == '+') {
			f->stat |= (char)FS_RD;
		}
		f->size = 0L;
	}
	/* 読み出しオープンモードの場合 */
	else if (mode[0] == 'r') {
		/* ファイルが存在しなければエラー */
		if (entrno == -1) {
			return NULL;
		}
		/* 先頭セクタデータを読み込み */
		clusno = get_le_h((void *)&d_ent.clusno);
		er = fs_r_sect(d, cluster_to_sector(d, clusno), (UB *)f->buf, (UH)1);
		if (er != R_OK) {
			return NULL;
		}
		f->stat = (char)FS_RD;
		if(mode[1] == '+') {
			f->stat |= (char)FS_WR;
		}
		/* ファイルサイズを設定 */
		f->size = get_le_w((void *)&d_ent.size);
	}
	else {
		return NULL;
	}
	sb_set_f(f, (UH)entrno, clusno, dnum, d);

	/* 書き込みモード'A'の場合はファイル先頭にシークし終了 */
	if(mode[0] == 'a') {
		fs_fseek(f, (W)f->size, (B)SEEK_SET);
	}
	return f;
}

/*******************************************************************
	for mkdir
********************************************************************/
static ER fs_mkdir(const B *dirname)
{
	ER er;
	ST_DIRENTRY d_ent;
	ST_DISK  *d = disk;
	H  entrno;
	UW rootsec = 0, tmp0, dmy;
	UH rootno, tmp1 = (UH)0;
	B name[12];
	UB num;
	UB n;
	B *dpos = dirname + 2;

	er = fs_chkpath(dirname);
	if(er != R_OK)
		return er;

	while (d->name != dirname[0])
	{
		d = d->next;
		if (d == NULL)
			return R_DRIVE_ERR;
	}

	fs_chkdnum(dirname, &num);

	if(num == 0)
	{
		dpos = fs_dget(name, ++dpos);
		if(dpos == NULL)
			return R_ABN_FILE;
		return make_entdir(d, name);
	}

	for(n = (UB)0; n < (num + 1); n++)
	{
		dpos = fs_dget(name, ++dpos);
		if(dpos == NULL)
			return R_ABN_FILE;

		if(n == 0)
		{
			er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)ATTR_DIR);		  /* root */
			if(entrno == -1)
				return R_ERR;  /* 00.08.22 */
		}
		else
		{
			if(entrno == -1)
				return R_ERR;  /* 00.08.22 */
			tmp0 = rootsec;
			tmp1 = rootno;
			er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)ATTR_DIR);
			if(er == R_OK)
			{
				if((entrno != -1) && (tmp0 != rootsec))
					tmp0 = rootsec;
				rootno = sector_to_cluster(d, rootsec);
			}
		}
		if(er != R_OK)
			return R_ERR;
		if(entrno != -1)
		{
			rootno = get_le_h((void *)&(d_ent.clusno));
			rootsec = cluster_to_sector(d, rootno);
		}
	}
	if(entrno != -1)	/* 00.08.22 */
		return R_ERR;

	rootno = tmp1;

	return make_subdir(d, name, rootno, (B)1, (H *)&entrno, (UH *)&tmp1, (UW *)&dmy);

}

/*******************************************************************
	for rmdir
********************************************************************/
static ER  fs_rmdir(B *dirname)
{
	ST_DIRENTRY d_ent;
	ER er;
	H  entrno;
	UW rootsec = 0, tmp = 0;  /* tmp = 0 00.08.22 */
	UH rootno;
	B name[12];
	UB set = (UB)0;
	UB num;
	UB n;
	B *dpos;
	ST_DISK *d = disk;
	B  r;

	r = (B)fs_chkpath(dirname);
	if(r != R_OK)
		return (ER)r;

	while (d->name != dirname[0])
	{
		d = d->next;
		if(d == NULL)
			return (ER)R_DRIVE_ERR;
	}

	fs_chkdnum(dirname, &num);

	dpos = dirname + 2;
	for(n = (UB)0; n < (num + 1); n++)
	{
		dpos = fs_dget(name, ++dpos);
		if(dpos == NULL)
			return (ER)R_ABN_FILE;
		if(n == 0)
			er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)ATTR_DIR);
		else
		{
			tmp = rootsec;
			er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)ATTR_DIR);
			if(er == R_OK)
			{
				if((entrno != -1) && (tmp != rootsec))
					tmp = rootsec;
				rootno = sector_to_cluster(d, rootsec);
			}
		}
		if(er != R_OK)
			return	(ER)R_ERR;
		if(entrno == -1)
			return	(ER)R_ERR;
		else
		{
			set=(UB)1;
			rootno = get_le_h((void *)&(d_ent.clusno));
			rootsec = cluster_to_sector(d, rootno);
		}
	}

	er = del_dirnum(d, rootno);
	if(er != R_OK)
	   return (ER)er;
	return (ER)del_mine(d, tmp, (UH)entrno);

}

/*******************************************************************
	Read Fat (ALL Area) Device->RAM
********************************************************************/
static UB ht_R_FAT(ST_DISK *d)
{
	UW s = d->rsc;
	UW end = d->rsc + d->sf;
	B *p = d->fatp;

	while(s != end)
	{
		fs_r_sect(d, s, (UB *)p, (UH)1);
		s++;
		p += SECT_SZ;
	}
	return (UB)R_OK;
}

/*******************************************************************
	Write Fat (ALL Area) RAM->Device
********************************************************************/
static UB ht_W_FAT(ST_DISK *d)
{
	UW s = d->rsc;
	UW end = d->rsc + d->sf;
	B *p = d->fatp;

	while(s != end)
	{
		fs_w_sect(d, s, (UB*)p, (UH)1);
		if(d->fn == 2)
			fs_w_sect(d, s + d->sf, (UB*)p, (UH)1);
		s++;
		p += SECT_SZ;
	}
	return (UB)R_OK;
}

/*******************************************************************
	for fclose
********************************************************************/
static ER fs_fclose(ST_FILE *f)
{
	ST_DISK *d;
	ER er;

	if(f == NULL)
		return EOF;

	if(f->stat == 0)
		return EOF;

	d = f->d;

	if(f->secchg)
	{
		er = fs_w_sect(d, (UW)f->sectno, (UB *)f->buf, (UH)1);
		if(er != R_OK)
			return EOF;
	}
	if(f->stat == FS_WR || f->stat == FS_RDWR || f->stat == FS_APND)
	{
		er = set_filesize(d, f->dir, (H)f->motsec, (H)f->entrno, f->size);
		if(er != R_OK)
		{
			f->stat = (char)0;
			return EOF;
		}
	}
	f->stat = (char)0;
#ifdef G_FAT
	if( !d->ram ) {
		ht_W_FAT(d);
	}
#endif
	return 0;
}

/*******************************************************************
	for fclose
********************************************************************/
static ER fs_fflush(ST_FILE *f)
{
	ST_DISK *d;
	ER er;

	if(f == NULL)
		return EOF;

	if(f->stat == 0)
		return EOF;

	d = f->d;

	if(f->secchg)
	{
		er = fs_w_sect(d, (UW)f->sectno, (UB *)f->buf, (UH)1);
		f->secchg = (short)0;
		if(er != R_OK)
			return EOF;
	}
	if(f->stat == FS_WR || f->stat == FS_RDWR || f->stat == FS_APND)
	{
		er = set_filesize(d, f->dir, (H)f->motsec, (H)f->entrno, f->size);
		if(er != R_OK)
		{
			return EOF;
		}
	}
#ifdef G_FAT
	if( !d->ram ) {
		ht_W_FAT(d);
	}
#endif
	return 0;
}

/*******************************************************************
	for fgetc
********************************************************************/
static int fs_fgetc(ST_FILE *f)
{
	ST_DISK *d;
	W n;
	ER er;
	int r_i;

	if(f->stat == 0)
		return EOF;
	d = f->d;

	if(f->byteno >= SECT_SZ)
	{
		if(f->start == 0)
		{
			er = make_dir_list(f, (B *)NULL);
			if(er != R_OK)
				return EOF;
		}
		else
		{
			if(f->secchg)
			{
				er = fs_w_sect(d, (UW)f->sectno, (UB *)f->buf, (UH)1);
				if(er != R_OK)
					return EOF;
				f->secchg = (short)0;
			}
			n = next_sector(d, f->sectno);
			if(n == 0)
				return EOF;
			er = fs_r_sect(d, (UW)n, (UB *)f->buf, (UH)1);
			if(er != R_OK)
				return EOF;
			f->sectno = n;
		}
		f->byteno = (UH)0;
	}
	if(f->pos >= f->size)
		return EOF;
	f->pos++;
	r_i = (int)(*((UB*)f->buf + f->byteno++));
	return r_i;
}

/*******************************************************************
	for fgets
********************************************************************/
static B *fs_fgets(B *s, H n, ST_FILE *fp)
{
	H i;
	int r_get;

	if(n < 1)
		return NULL;

	for(i = (H)0; i < (n - 1); )
	{
		r_get = fs_fgetc(fp);
		if(r_get == EOF)
			break;
		if(r_get == '\r')  /* CR */
			continue;
		if(r_get == '\n')
			break;
		s[i++] = (B)r_get;
	}

	s[i] = (B)'\0';
	if(r_get != '\n')
		return NULL;
	else
		return s;
}

/*******************************************************************
	for fputc
********************************************************************/
static int fs_fputc(ST_FILE *f, UB c)
{
	ST_DISK *d;
	ER er;
	W n;

	if(f->stat == 0 || f->stat == FS_RD)
		return EOF;
	d = f->d;

//	if(f->stat == FS_APND)	  /* 00.08.22 add */
//		fs_fseek(f, (W)f->size, (B)SEEK_SET);

	if(f->byteno >= SECT_SZ)
	{
		if(f->secchg)
		{
			er = fs_w_sect(d, (UW)f->sectno, (UB *)f->buf, (UH)1);
			if(er != R_OK)
				return EOF;
			f->secchg = (short)0;
		}
		n = next_sector(d, f->sectno);
		if(n == 0)
		{
			n = new_sector(d, f->sectno);
			if(n == 0)
				return EOF;
			fs_memset((void *)f->buf, (B)0, (UW)SECT_SZ);
		}
		else
		{
			er = fs_r_sect(d, (UW)n, (UB *)f->buf, (UH)1);
			if(er != R_OK)
				return EOF;
		}
		f->sectno = n;
		f->byteno = (UH)0;
	}
	*((UB *)f->buf + f->byteno++) = c;
	if (++f->pos >= f->size)
		f->size = f->pos;
	f->secchg = (short)1;
	return (int)c;
}

/*******************************************************************
	for fputs
********************************************************************/
static B fs_fputs(const B *s, ST_FILE *fp)
{
	int c;
	ER ercd;
	B *buf = s;

	while ((c = *buf++) != '\0')
	{
		ercd = fs_fputc(fp, (UB)c);
		if(ercd == EOF)
			return (B)EOF;
	}
	return (B)R_OK;
}

/*******************************************************************
	for remove
********************************************************************/
static ER fs_remove(const B *path)
{
	ST_DISK *d = disk;
	ST_DIRENTRY d_ent;
	ER er;
	H entrno;
	B name[11];
	B  fname[14], flg;
	UW rootsec = 0;
	UH clusno;

	er = fs_chkpath(path);
	if(er != R_OK)
		return er;

	while(d->name != path[0])
	{
		d = d->next;
		if(d == NULL)
			return R_DRIVE_ERR;
	}

	er = fs_getfname(path + 2, fname, &flg);
	if(er != R_OK)
		return er;
	if(fs_conv_fname(fname, name) == NULL)
		return R_NO_FILE;
	if(flg == 0)
	{
		er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)0);
		if(er != R_OK)
			return er;
		if(entrno == -1)
			return R_NO_FILE;
	}
	else
	{
		er = seek_dir(d, path, &clusno);
		if(er != R_OK)
			return er;
		rootsec = cluster_to_sector(d, clusno);
		er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)0);
		clusno = sector_to_cluster(d, rootsec);
		if(er != R_OK)
			return	er;
		clusno = get_le_h((void *)&(d_ent.clusno));
	}
	if(flg == 0)
		return del_dir_entry(d, entrno);
	else
	{
		while(clusno != 0)
			clusno = del_cluster(d, clusno);
		return del_mine(d, rootsec, (UH)entrno);
	}
}

/*******************************************************************
	for rename
********************************************************************/
static ER fs_rename(const B *old, const B *new)
{
	ST_DIRENTRY d_ent;
	ER er, er1, er2;
	H entrno;
	B name[11];
	B oldfname[14];
	UW rootsec = 0;
	UH clusno, clsno;
	UB dnum;
	ST_DISK *d = disk;

	er = fs_chkpath(old);
	if(er != R_OK)
		return er;

	if(fs_strlen(new) > FILELEN_MAX)
		return R_ABN_FILE;

	while(d->name != old[0])
	{
		d = d->next;
		if(d == NULL)
			return R_DRIVE_ERR;
	}

	er = fs_dfchg(old + 2, oldfname);
	if(er != R_OK)
		return er;

	er = fs_fchg(name, new);
	if(er != R_OK)
		return er;

	fs_chkdnum(old + 2, &dnum);

	if(dnum == 0)
	{
		er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)0);
		if(er != R_OK)
			return er;
		if(entrno != -1)
			return -1;
	}
	else
	{
		er = seek_dir(d, old, &clusno);
		if(er != R_OK)
			return er;
		clsno = clusno;
		rootsec = cluster_to_sector(d, clusno);
		er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)0);
		clusno = sector_to_cluster(d, rootsec);
		if(er != R_OK)
			return er;
		if( entrno != -1 )
			return	-1;
	}

	if(dnum == 0)
	{
		er1 = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, oldfname, (B)0);
		er2 = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, oldfname, (B)ATTR_DIR);

		if (er1 != R_OK && er2 != R_OK )
			return er;
		if(entrno == -1)
			return	-1;
	}
	else
	{
		rootsec = cluster_to_sector(d, clsno);

		er1 = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, oldfname, (UW *)&rootsec, (B)0);
		er2 = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, oldfname, (UW *)&rootsec, (B)ATTR_DIR);

		if (er1 != R_OK && er2 != R_OK )
			return er;
		if(entrno == -1)
			return -1;
	}

	er = fs_fchg(name, new);
	if(er != R_OK)
		return er;

	if(dnum == 0)
	{
		fs_memcpy((void *)d_ent.name, (void *)name, (UW)11);
		er = set_dir_entry(d, entrno, &d_ent);
	}
	else
	{
		er = fs_r_sect(d, rootsec, (UB *)d->r_buf, (UH)1);
		if(er != R_OK)
			return er;
		fs_memcpy((void *)&d->r_buf[entrno*DIRENTRY_SZ], (void *)name, (UW)11);
		er = fs_w_sect(d, rootsec, d->r_buf, (UH)1);
	}
	return er;
}

/****************************************************************************
	for finfo
*****************************************************************************/
static B fs_finfo(const B *path, ST_FDATA *fdt)
{
	ST_DISK *d = disk;
	ST_DIRENTRY d_ent;
	ER er;
	H entrno;
	B name[11];
	UH clusno, workclus;
	UW rootsec;
	UB dnum;

	er = fs_chkpath(path);
	if(er != R_OK)
		return (B)R_ERR;

	while(d->name != path[0])
	{
		d = d->next;
		if(d == NULL)
			return (B)R_ERR;
	}

	er = fs_dfchg(path, name);
	if(er != R_OK)
		return (B)R_ERR;

	fs_chkdnum(path, &dnum);
	if(dnum == 0)
	{
		er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)0);
		if (er != R_OK)
			return (B)R_ERR;
	}
	else
	{
		er = seek_dir(d, path, &clusno);
		if(er != R_OK)
			return (B)R_ERR;
		workclus = clusno;
		rootsec = cluster_to_sector(d, clusno);
		er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)0);
		clusno = sector_to_cluster(d, rootsec);
		if(er != R_OK)
			return (B)R_ERR;
		if(entrno != -1)
			workclus = clusno;
	}

	if(entrno == -1)
		return (B)R_ERR;

	fs_memcpy((void *)&fdt->name, (void *)&d_ent.name, (UW)11);
	fdt->attr = d_ent.attr;
	er = fs_dent_to_tm(&fdt->time, d_ent.time, d_ent.date);
	fdt->clust = get_le_h((void *)&d_ent.clusno);
	fdt->size = get_le_w((void *)&d_ent.size);

	return (B)R_OK;
}

/*******************************************************************
	for getfree
********************************************************************/
static UW fs_getfree(B drv)
{
	ST_DISK *d = disk;
	UH	r_fat;
	UH	n, end;
	UW size;
	UW num = 0;

	if(d == NULL)
		return (UW)0;

	while(d->name != drv)
	{
		d = d->next;
		if(d == NULL)
			return (UW)0;
	}

	if(d->fatbits == 16)
		end = (UH)((SECT_SZ / 2) * d->sf);
	else
		end = (UH)((SECT_SZ * d->sf * 2) / 3);

	if(end > (UH)((d->ts - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) / d->sc))
		end = (UH)(((d->ts - (d->rsc + d->sf * d->fn + ((d->rde * DIRENTRY_SZ - 1) / SECT_SZ + 1))) / d->sc) + 2);

	for(n = (UH)2; n < end; n++)
	{
		r_fat = chg_fat_entry(d, n, (UH)0, (B)'r');
		if(r_fat == 0xFFF8)
			return (UW)0;
		if(r_fat == 0)
			num++;
	}
	size = d->sc * SECT_SZ * num;
	return (UW)size;

}

/*******************************************************************
	for fread
********************************************************************/
static UW fs_fread(void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW i, len;
	UB *b;
	int r_get;

	len = (UW)size * n;
	if (len == 0L)
		return (UW)0;

	b = (UB*)ptr;
	for (i = 0; i < len; i++)
	{
		r_get = fs_fgetc(fp);
		if (r_get == EOF)
			break;
		*b++ = (UB)r_get;
	}
	return (UW)(i / size);
}

/*******************************************************************
	for fwrite
********************************************************************/
static UW fs_fwrite(const void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW i, len;
	UB *b;
	int r_put;

	len = (UW)size * n;
	if (len == 0L)
		return (UW)0;

	b = (UB *)ptr;
	for (i = 0; i < len; i++)
	{
		r_put = fs_fputc(fp, *b++);
		if(r_put == EOF)
			break;
	}
	return (UW)(i / size);
}

/*******************************************************************
	for fread(G_FAT)
********************************************************************/
static UW fsh_fread(void *ptr, UW size, UW n, ST_FILE *fp)
{
	ST_DISK *d = fp->d;
	UH clst_sz = (UH)(d->sc * SECT_SZ);
	UW len = size * n;
	UB *b = (UB*)ptr;
	UW m;
	B er;
	UW s_top, s_mdl, s_lst;
	UH sec_no;
	UW r_size;
	UW lp;
	UW chain;
	UH start_sec;
	int r_get;
	UW tmp;

	tmp = fp->size - fp->pos;
	if(tmp < len)
		len = tmp;

	if(len == 0L)
		return (UW)0;

	if(((fp->pos % clst_sz) + len) < (clst_sz * 2))
	{
		for(m = 0; m < len; m++)
		{
			r_get = fs_fgetc(fp);
			if(r_get == EOF)
				break;
			*b++ = (UB)r_get;
		}
#if 0
		return (UW)(m / size);
#else
		return (UW)(m);
#endif
	}

	s_top = clst_sz - (fp->pos % clst_sz);
	s_lst = (fp->pos + len) % clst_sz;
	s_mdl = len - s_top - s_lst;

	for(m = 0; m < s_top; m++)
	{
		r_get = fs_fgetc(fp);
		if(r_get == EOF)
#if 0
			return (UW)(m / size);
#else
			return (UW)(m);
#endif
		*b++ = (UB)r_get;
	}

	r_size = (UW)(s_top / size);

	chain = 0;
	lp = s_mdl / clst_sz;
	for(m = 0; m < lp; m++)
	{
		sec_no = (UH)next_sector(d, fp->sectno);
		if(sec_no == 0)
#if 0
			return r_size;
#else
			return (UW)0;
#endif
		if(chain == 0)
		{
			chain++;
			start_sec = sec_no;
			fp->sectno = sec_no + d->sc - 1;
			continue;
		}
		if((fp->sectno + 1) == sec_no)
		{
			if(chain < (256 / d->sc))
			{
				chain++;
				fp->sectno = sec_no + d->sc - 1;
				continue;
			}
		}
		er = (B)fs_r_sect(d, (UW)start_sec, b, (UH)(d->sc * chain));
		start_sec = sec_no;
		if(er != R_OK)
#if 0
			return r_size;
#else
			return (UW)0;
#endif
		b += (clst_sz * chain);
		chain = 1;
		fp->sectno = sec_no + d->sc - 1;
	}

	if(chain >= 1)
	{
		er = (B)fs_r_sect(d, (UW)start_sec, b, (UH)(d->sc * chain));
		if(er != R_OK)
#if 0
			return r_size;
#else
			return (UW)0;
#endif
		b += (clst_sz * chain);
		fp->sectno = start_sec + (d->sc * chain) - 1;
	}
	r_size += (clst_sz * lp / size);
	fp->pos += (clst_sz * lp);

	for(m = 0; m < s_lst; m++)
	{
		r_get = fs_fgetc(fp);
		if(r_get == EOF)
#if 0
			return (UW)(m / size + r_size);
#else
			return (UW)(m);
#endif
		*b++ = (UB)r_get;
	}
#if 0
	return (tmp / size);
#else
	return len;
#endif
}

/*******************************************************************
	for fwrite(G_FAT)
********************************************************************/
static UW fsh_fwrite(const void *ptr, UW size, UW n, ST_FILE *fp)
{
	ST_DISK *d = fp->d;
	UH clst_sz = (UH)(d->sc * SECT_SZ);
	UW len = size * n;
	UB *b = (UB*)ptr;
	UW m;
	B er;
	UW s_top, s_mdl, s_lst;
	UH sec_no;
	UW r_size;
	UW lp;
	UW chain;
	UH start_sec;
	int r_put;

	if(len == 0L)
		return (UW)0;

	if((fp->pos % clst_sz + len) < (clst_sz * 2))
	{
		for(m = 0; m < len; m++)
		{
			r_put = fs_fputc(fp, *b++);
			if(r_put == EOF)
				break;
		}
		return (UW)(m / size);
	}

	if(fp->stat == FS_APND)    /* 00.08.22 add */
		fs_fseek(fp, (W)fp->size, (B)SEEK_SET);

	s_top = clst_sz - (fp->pos % clst_sz);
	s_lst = (fp->pos + len) % clst_sz;
	s_mdl = len - s_top - s_lst;

	for(m = 0; m < s_top; m++)
	{
		r_put = fs_fputc(fp, *b++);
		if(r_put == EOF)
			return (UW)(m / size);
	}
	r_size = (UW)(s_top / size);

	if(fp->secchg)
	{
		er = (B)fs_w_sect(d, (UW)fp->sectno, (UB *)fp->buf, (UH)1);
		if(er != R_OK)
			return r_size;
		fp->secchg = (short)0;
	}
	chain = 0;
	lp = s_mdl / clst_sz;
	for(m = 0; m < lp; m++)
	{
		sec_no = (UH)next_sector(d, fp->sectno);
		if(sec_no == 0)
		{
			sec_no = (UH)new_sector(d, fp->sectno);
			if(sec_no == 0)
				return r_size;
		}
		if(chain == 0)
		{
			chain++;
			start_sec = sec_no;
			fp->sectno = sec_no + d->sc - 1;
			continue;
		}
		if((fp->sectno + 1) == sec_no)
		{
			if(chain < (256 / d->sc))
			{
				chain++;
				fp->sectno = sec_no + d->sc - 1;
				continue;
			}
		}
		er = (B)fs_w_sect(d, (UW)start_sec, b, (UH)(d->sc * chain));
		start_sec = sec_no;
		if(er != R_OK)
			return r_size;
		b += (clst_sz * chain);
		chain = 1;
		fp->sectno = sec_no + d->sc - 1;
	}

	if(chain >= 1)
	{
		er = (B)fs_w_sect(d, (UW)start_sec, b, (UH)(d->sc * chain));
		if(er != R_OK)
			return r_size;
		b += (clst_sz * chain);
		fp->sectno = start_sec + (d->sc * chain) - 1;
	}
	r_size += (clst_sz * lp / size);
	fp->pos += (clst_sz * lp);
	if(fp->pos > fp->size)
		fp->size = fp->pos;

	for(m = 0; m < s_lst; m++)
	{
		r_put = fs_fputc(fp, *b++);
		if(r_put == EOF)
			return (UW)(m / size + r_size);
	}
	return n;
}

/*******************************************************************
	for opendir
********************************************************************/
static ST_DIR *fs_opendir(const B *dname)
{
	ST_DIR *my_dir = &dir;
	UB dnum;
	ST_DISK *d = disk;
	ST_DIRENTRY d_ent;
	ER er;
	H entrno;
	UH rootno;
	UB n;
	B *dpos;
	UW rootsec = 0;
	UW tmp_w = 0;
	B name[12];

	if(my_dir->stat != 0)
		return NULL;

	while(d->name != dname[0])
	{
		d = d->next;
		if(d == NULL)
			return NULL;
	}
	/* 00.08.20 "A:" -> "A:\\" */
	/* if((dname[1] == ':') && (fs_strlen(dname) == 2)) */
	if((fs_memcmp((void *)&dname[1], (void *)":\\", (UW)2) == 0) && (fs_strlen(dname) == 3))
	{
		my_dir->dnum = (unsigned char)0;
		my_dir->stat = (char)1;
		my_dir->entr_no = (unsigned short)0;
		my_dir->sect = (unsigned short)(d->rsc + d->sf * d->fn);
		my_dir->pos = (unsigned short)0;
		my_dir->d = d;
		return my_dir;
	}

	fs_chkdnum(dname, &dnum);

	my_dir->dnum = (unsigned char)(dnum + 1);

	dpos = dname + 2;
	for(n = (UB)0; n < my_dir->dnum; n++)
	{
		dpos = fs_dget(name, ++dpos);
		if(dpos == NULL)
			return NULL;

#if 1
		if( name[0]=='.' && name[1]=='.' ) {
			rootsec = my_dir->sect;
			er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)ATTR_DIR);
			if(er == R_OK)
			{
				if((entrno != -1) && (tmp_w != rootsec))
					tmp_w = rootsec;
				rootno = sector_to_cluster(d, rootsec);
			}
		}
		else if(n == 0) {
#else
		if(n == 0){
#endif
			er = find_dir_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (B)ATTR_DIR);
		}
		else {
			tmp_w = rootsec;
			er = find_sub_entry(d, (H *)&entrno, (ST_DIRENTRY *)&d_ent, name, (UW *)&rootsec, (B)ATTR_DIR);
			if(er == R_OK)
			{
				if((entrno != -1) && (tmp_w != rootsec))
					tmp_w = rootsec;
				rootno = sector_to_cluster(d, rootsec);
			}
		}
		if(er != R_OK)
			return NULL;
		if(entrno == -1)
			return NULL;
		else
		{
			rootno = get_le_h((void *)&(d_ent.clusno));
#if 0
			rootsec = cluster_to_sector(d, rootno);
#else
			if( rootno == 0 ) {
				rootsec = d->rsc + d->sf * d->fn;
			}
			else {
				rootsec = cluster_to_sector(d, rootno);
			}
#endif
		}
	}

	my_dir->stat = (char)1;
	my_dir->entr_no = rootno;
	my_dir->sect = (unsigned short)rootsec;
	my_dir->pos = (unsigned short)0;
	my_dir->d = d;
	return my_dir;
}

/*******************************************************************
	for closedir
********************************************************************/
static B fs_closedir(ST_DIR *dp)
{
	if(dp->stat == 0)
		return (B)R_ERR;

	dp->stat = (char)0;
	return (B)R_OK;
}

/*******************************************************************
	for readdir(sub)
********************************************************************/
static ST_DIRENTRY *fs_readdir_sub(ST_DIR *dp)
{
	ST_DIRENTRY *d_p;
	ER er;
	UH secno = dp->sect;

	if(dp->stat != 1)
		return NULL;

	if(dp->pos == 0)
	{
		er = fs_r_sect(dp->d, (UW)secno, (UB *)dp->buf, (UH)1);
		if(er != R_OK)
			return NULL;
	}

	d_p = (ST_DIRENTRY*)(dp->buf + (DIRENTRY_SZ * dp->pos));
	if(*(B*)d_p == '\0')
	{
		dp->stat = (char)-1;
		return NULL;
	}

	if((++dp->pos % DIR_ENTRYS_PER_SECTOR) == 0)
	{
		dp->pos = (unsigned short)0;
		secno = (UH)next_sector(dp->d, (UW)secno);
		if(secno == 0)
			dp->stat = (char)-1;
		else
			dp->sect = secno;
	}
	return d_p;
}

/*******************************************************************
	readdir
********************************************************************/
static ST_DIRENTRY *fs_readdir(ST_DIR *dp)
{
	ST_DIRENTRY *r_p;

	for(;;)
	{
		r_p = fs_readdir_sub(dp);
		if(r_p == NULL)
			break;
#if 0
		if(r_p->name[0] == '.' || r_p->name[0] == 0xE5)
#else
		if( r_p->name[0] == 0xE5)
#endif
			continue;
		break;
	}
	return r_p;
}

/*******************************************************************
	Filesystem Library Function
********************************************************************/
/*******************************************************************
	fopen
********************************************************************/
ST_FILE *ht_fopen(const B *fname, const B *mode)
{
	ST_FILE *er_fp;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, fname, (UW)(FILENAME_MAX + 1));
	er_fp = fs_fopen(name_toup, mode);
	sig_sem((ID)ID_FILE_SEM);

	return er_fp;
}

/*******************************************************************
	fclose
********************************************************************/
B ht_fclose(ST_FILE *fp)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	er = (B)fs_fclose(fp);
	sig_sem((ID)ID_FILE_SEM);

	return er;
}

/*******************************************************************
	fgetc
********************************************************************/
int ht_fgetc(ST_FILE *fp)
{
	int r_get;

	wai_sem((ID)ID_FILE_SEM);
	r_get = fs_fgetc(fp);
	sig_sem((ID)ID_FILE_SEM);

	return r_get;
}

/*******************************************************************
	fgets
********************************************************************/
B *ht_fgets(B *s, H n, ST_FILE *fp)
{
	B *r_get;

	wai_sem((ID)ID_FILE_SEM);
	r_get = fs_fgets(s, n, fp);
	sig_sem((ID)ID_FILE_SEM);

	return r_get;
}

/*******************************************************************
	fputc
********************************************************************/
int ht_fputc(int c, ST_FILE *fp)
{
	int r_put;

	wai_sem((ID)ID_FILE_SEM);
	r_put = fs_fputc(fp, (UB)c);
	sig_sem((ID)ID_FILE_SEM);

	return r_put;
}

/*******************************************************************
	fputs
********************************************************************/
B ht_fputs(const B *s, ST_FILE *fp)
{
	B r;

	wai_sem((ID)ID_FILE_SEM);
	r = fs_fputs(s, fp);
	sig_sem((ID)ID_FILE_SEM);

	return r;
}

/*******************************************************************
	fread
********************************************************************/
UW ht_fread(void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW r_size;

	wai_sem((ID)ID_FILE_SEM);
#ifdef G_FAT
	r_size = fsh_fread(ptr, size, n, fp);
#else
	if( fp->d->ram ) {
		r_size = fsh_fread(ptr, size, n, fp);
	}
	else {
		r_size = fs_fread(ptr, size, n, fp);
	}
#endif
	sig_sem((ID)ID_FILE_SEM);
	return r_size;
}
/*******************************************************************
	fread
********************************************************************/
UW hh_fread(void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW r_size;

	wai_sem((ID)ID_FILE_SEM);
#ifdef G_FAT
	r_size = fsh_fread(ptr, size, n, fp);
#else
	if( fp->d->ram ) {
		r_size = fsh_fread(ptr, size, n, fp);
	}
	else {
		r_size = fs_fread(ptr, size, n, fp);
	}
#endif
	sig_sem((ID)ID_FILE_SEM);
	return r_size;
}

/*******************************************************************
	fwrite
********************************************************************/
UW ht_fwrite(const void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW w_size;

	wai_sem((ID)ID_FILE_SEM);
#ifdef G_FAT
	w_size = fsh_fwrite(ptr, size, n, fp);
#else
	if( fp->d->ram ) {
		w_size = fsh_fwrite(ptr, size, n, fp);
	}
	else {
		w_size = fs_fwrite(ptr, size, n, fp);
	}
#endif
	sig_sem((ID)ID_FILE_SEM);

	return w_size;
}

/*******************************************************************
	fwrite
********************************************************************/
UW hh_fwrite(const void *ptr, UW size, UW n, ST_FILE *fp)
{
	UW w_size;

	wai_sem((ID)ID_FILE_SEM);
#ifdef G_FAT
	w_size = fsh_fwrite(ptr, size, n, fp);
#else
	if( fp->d->ram ) {
		w_size = fsh_fwrite(ptr, size, n, fp);
	}
	else {
		w_size = fs_fwrite(ptr, size, n, fp);
	}
#endif
	sig_sem((ID)ID_FILE_SEM);

	return w_size;
}

/*******************************************************************
	fseek
********************************************************************/
B ht_fseek(ST_FILE * fp, W offset, B type)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	er = (B)fs_fseek(fp, offset, type);
	sig_sem((ID)ID_FILE_SEM);

	return er;
}

/*******************************************************************
	ftell
********************************************************************/
W ht_ftell(ST_FILE *fp)
{
	W er_tell;

	wai_sem((ID)ID_FILE_SEM);
	er_tell = fs_ftell(fp);
	sig_sem((ID)ID_FILE_SEM);

	return er_tell;
}

/*******************************************************************
	feof
********************************************************************/
B ht_feof(ST_FILE *fp)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	er = fs_feof(fp);
	sig_sem((ID)ID_FILE_SEM);

	return er;
}

/*******************************************************************
	rename
********************************************************************/
B ht_rename(const B *old, const B *new)
{
	B er;

	if(chk_str_dos(new) == -1)
		return (B)-1;
	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, old, (UW)(FILENAME_MAX + 1));
	fs_str_cpy_upr(fname_toup, new, (UW)(FILELEN_MAX + 1));
	er = (B)fs_rename(name_toup, fname_toup);
	sig_sem((ID)ID_FILE_SEM);

	if(er == R_OK)
		return (B)0;
	return (B)-1;
}

/*******************************************************************
	remove
********************************************************************/
B ht_remove(const B *pathname)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, pathname, (UW)(FILENAME_MAX + 1));
	er = (B)fs_remove(name_toup);
	sig_sem((ID)ID_FILE_SEM);

	if(er == R_OK)
		return (B)0;
	return (B)-1;
}

/****************************************************************************
	finfo
*****************************************************************************/
B ht_finfo(const B *pathname, ST_FDATA *fdt)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, pathname, (UW)(FILENAME_MAX + 1));
	er = (B)fs_finfo(name_toup, fdt);
	sig_sem((ID)ID_FILE_SEM);

	if(er == R_OK)
		return (B)0;
	return (B)-1;
}

/*******************************************************************
	mkdir
********************************************************************/
B ht_mkdir(const B *pathname)
{
	B er;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, pathname, (UW)(FILENAME_MAX + 1));
	er = (B)fs_mkdir(name_toup);
	sig_sem((ID)ID_FILE_SEM);

	if(er == R_OK)
		return (B)0;
	return (B)-1;
}

/*******************************************************************
	rmdir
********************************************************************/
B ht_rmdir(const B *pathname)
{
	B  r;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, pathname, (UW)(FILENAME_MAX + 1));
	r = (B)fs_rmdir(name_toup);
	sig_sem((ID)ID_FILE_SEM);

	if( r == R_OK )
		return (B)0;
	return (B)-1;
}

/*******************************************************************
	opendir
********************************************************************/
ST_DIR *ht_opendir(const B *dname)
{
	ST_DIR *r_dir;

	wai_sem((ID)ID_FILE_SEM);
	fs_str_cpy_upr(name_toup, dname, (UW)(FILENAME_MAX + 1));
	r_dir = fs_opendir(name_toup);
	sig_sem((ID)ID_FILE_SEM);

	return r_dir;
}

/*******************************************************************
	closedir
********************************************************************/
B ht_closedir(ST_DIR *dp)
{
	B r;

	wai_sem((ID)ID_FILE_SEM);
	r = fs_closedir(dp);
	sig_sem((ID)ID_FILE_SEM);

	return r;
}

/*******************************************************************
	readdir
********************************************************************/
ST_DIRENTRY *ht_readdir(ST_DIR *dp)
{
	ST_DIRENTRY *r_p;

	wai_sem((ID)ID_FILE_SEM);
	r_p = fs_readdir(dp);
	sig_sem((ID)ID_FILE_SEM);

	return r_p;
}

/*******************************************************************
	getfree
********************************************************************/
UW ht_getfree(B drv)
{
	UW size;

	wai_sem((ID)ID_FILE_SEM);
	size = fs_getfree(fs_toupper(drv));
	sig_sem((ID)ID_FILE_SEM);

	return size;
}

/*******************************************************************
	ini_fsys
********************************************************************/
B ini_fsys(ST_FILE *f, H n, H semid)
{

	fs_memset((void *)f, (B)0, (UW)sizeof(ST_FILE) * n);
	fs_memset((void *)&dir, (B)0, (UW)sizeof(ST_DIR));
	file = f;
	NFILE = (B)n;
	if( !ID_FILE_SEM ) {
		ID_FILE_SEM = (UB)acre_sem( (T_CSEM *)&file_csem );
		if (ID_FILE_SEM < 0 ) {
			return (B)-1;
		}
	}
	return (B)sig_sem((ID)ID_FILE_SEM);
}

/*******************************************************************
	ini_disk
********************************************************************/
B ini_disk(ST_DISK * d, B drv, DISK_FP func, UW addr, UW par,
		   DISK_CALLBACK callback, B removable, B *fatp)
{
	ST_DISK *p;
	B r;

	d->name = (B)fs_toupper(drv);
	d->func = func;
	d->callback = callback;
	d->dsk_flg = (char)0;
	if( removable )
		d->dsk_flg |= DISK_REMOVABLE;
	d->addr = addr;
	d->par = par;
	d->ch = (char)0;
	d->nullfat = (unsigned short)2;
	d->n = (UW)-1;
	if( d->func != ramdisk ) {
		d->ram = (char)0;
#ifdef G_FAT
		d->fatp = fatp;
#endif
	}
	else {
		d->ram = (char)1;
	}
	if(disk == NULL)
		disk = d;
	else
	{	d->ch++;
		p = disk;
		while (p->next != NULL)
			p = p->next;
		p->next = d;
	}
	d->next = NULL;

	r = (B)(*d->func)(d, (short)TFN_HARDWARE_INIT, (UW)0, NULL, (UH)1);
	if(r != R_OK)
		return r;
	if(!removable) {
		r = (B)(*d->func)(d, (short)TFN_MOUNT, (UW)0, NULL, (UH)1);		/* identify */
#ifdef G_FAT
		if( !d->ram ) {
			ht_R_FAT(d);
		}
#endif
	}
	return r;
}

/*******************************************************************
	mount_disk
********************************************************************/
B mount_disk(ST_DISK * d)
{
	B r;
	d->n = 0;
	d->p = NULL;
	d->ercd = (short)R_OK;
	r = (B)(*d->func)(d, (short)TFN_MOUNT ,(UW) 0, NULL, (UH)1);
#ifdef G_FAT
	if( !d->ram ) {
		ht_R_FAT(d);
	}
#endif
	return r;
}

/*******************************************************************
	unmount_disk
********************************************************************/
B unmount_disk(ST_DISK * d)
{
	B r;

	if ( !(d->dsk_flg & DISK_REMOVABLE) )
		return (B)R_NOT_SUPORT;
#ifdef G_FAT
	if( !d->ram ) {
		ht_W_FAT(d);
	}
#endif
	r = (B)(*d->func)(d, (short)TFN_UNMOUNT , (UW)0, NULL, (UH)1);
	return r;
}

/*******************************************************************
	read FAT
********************************************************************/
B fat_to_ram(B drv)
{
	ST_DISK* my_d;

	fs_drv((UB)drv, (ST_DISK **)&my_d, (UW *)NULL, (B **)NULL);

	ht_R_FAT(my_d);
	return((B)0);
}

/*******************************************************************
	write FAT
********************************************************************/
B ram_to_fat(B drv)
{
	ST_DISK* my_d;

	fs_drv((UB)drv, (ST_DISK **)&my_d, (UW *)NULL, (B **)NULL);
	ht_W_FAT(my_d);
	return((B)0);
}

/* ISHIDA 2000/10/26 s.sakamoto */
/*******************************************************************
	for chsize（現在のシーク位置でファイルサイズを変更）
********************************************************************/
static B fs_chsize( ST_FILE *fp )
{
	UH clusno, last;

	/* シークセクタが変更されていれば書き込んでおく */
	if( fp->secchg ) {
		if( fs_w_sect( fp->d, (UW)fp->sectno, (UB *)fp->buf, (UH)1 ) ) {
			return (B)EOF;
		}
		fp->secchg = (short)0;
	}

	/* シークしているセクタのクラスタを計算 */
	clusno = sector_to_cluster( fp->d, fp->sectno );

	/* 消去するクラスタのサーチ */
	last = clusno;
	clusno = next_cluster( fp->d, clusno );

	/* 後ろに続くクラスタがあれば */
	if( clusno ) {
		/* ＦＡＴのチェインを外す */
		while( clusno ) {
			clusno = del_cluster( fp->d, clusno );
		}
		chg_fat_entry( fp->d, last, (UH)0xFFFF, (B)'w');
	}
	/* ファイルサイズの変更 */
	fp->size = fp->pos;


	return (B)0;
}

/*******************************************************************
	f_chsize
********************************************************************/
B f_chsize( ST_FILE *fp )
{
	B r_err;

	wai_sem((ID) ID_FILE_SEM );
	r_err = fs_chsize( fp );
	sig_sem((ID) ID_FILE_SEM );

	return r_err;
}

/*******************************************************************
	f_chsize
********************************************************************/
B f_fflush( ST_FILE *fp )
{
	B r_err;

	wai_sem((ID) ID_FILE_SEM );
	r_err = (B)fs_fflush(fp);
	sig_sem((ID) ID_FILE_SEM );

	return r_err;
}
/*******************************************************************
	get_id_filesem
********************************************************************/
UB get_id_filesem( void )
{
	return ID_FILE_SEM;
}
