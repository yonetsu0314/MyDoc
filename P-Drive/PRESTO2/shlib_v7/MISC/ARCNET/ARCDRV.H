/*======================================================================*/
/* File name	: arcdrv.h						*/
/* Original: 		*/
/*----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/arcnet/RCS/arcdrv.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: arcdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/08/05 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * VxWorksとshlibのインターフェース
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1997-2001 ISHIDA CO., LTD.						*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*/
/*		Phone +81 775 53-4141											*/
/*----------------------------------------------------------------------*/
#define ARC_TRUE  1
#define ARC_FALSE 0

typedef struct	comh {
	unsigned char	*map_tbl;	/* 接続されているﾉｰﾄﾞ番号ﾃｰﾌﾞﾙ	*/
	unsigned int	recon_cntr;	/* ﾘｺﾝﾌｨｸﾞ発生回数ｶｳﾝﾀ		*/
} COMH;

int  cw_chk_arcnet_tbl(int);
int  cw_arc_map(void);
int  cw_map_chk(void);
int  cw_arc_stat(enum statspc);
int  cw_arc_node(int);
int  cw_arc_nextid(void);

int node_hunt(struct com_ctxt *,unsigned char);
int arc_nextid(struct com_ctxt *);
int arc_map(int);
int read_diagr(struct com_ctxt *,unsigned char,unsigned char);

int _arc_node(int no, int id);
int _arc_nextid(int no);
int _chk_arcnet_tbl(int no, int N_ID);
int _arc_map(int no);
int _map_chk(int no);
int _arc_stat(int no, enum statspc cmd);

