/*======================================================================
 * Function 	:t0026
 *	プログラム番号定義
 *----------------------------------------------------------------------
 *$Log: t0026.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */ 
const char	rom_id[]= "$Header: p:/presto/dmu_v7/bootload/rcs/t0051.c 1.1 1970/01/01 00:00:00 kagatume Exp $";

char *get_lib_id(void)
{
char	*lib_id;
extern char	*customid;
extern char	*romid;
extern char	*cnvid;

extern char	*dcomid;
extern char	*dfifid;
extern char	*de2rid;
extern char	*de2srid;
extern char	*dfrid;
extern char	*dsciid;
extern char	*dsramid;
extern char	*dtmrid;
extern char	*dethid;
extern char	*dfsid;
extern char	*dcfid;
extern char	*drtcid;
extern char	*mfsid;
extern char	*mdmuid;

extern char	*bcfid;
extern char	*bfifid;
extern char	*biniid;
extern char	*bcomid;
extern char	*be2rid;
extern char	*bicuid;
extern char	*bfrid;
extern char	*bscuid;
extern char	*btcuid;
extern char	*brtcid;
extern char	*bsramid;
extern char	*n4userid;

	lib_id = customid;
	lib_id = romid;
	lib_id = cnvid;

	lib_id = dcomid;
	lib_id = dfifid;
	lib_id = de2rid;
	lib_id = de2srid;
//	lib_id = dfrid;
	lib_id = dsciid;
	lib_id = dsramid;
//	lib_id = dtmrid;
	lib_id = dethid;
	lib_id = dfsid;
	lib_id = dcfid;
	lib_id = drtcid;
	lib_id = mdmuid;
	lib_id = mfsid;

	lib_id = bcfid;
	lib_id = bfifid;
	lib_id = biniid;
	lib_id = bcomid;
	lib_id = be2rid;
	lib_id = bicuid;
//	lib_id = bfrid;
//	lib_id = bscuid;
//	lib_id = btcuid;
	lib_id = brtcid;
	lib_id = bsramid;
	lib_id = n4userid;

	return(rom_id);
}
