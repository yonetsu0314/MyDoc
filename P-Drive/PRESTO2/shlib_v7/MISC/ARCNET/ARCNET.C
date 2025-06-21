/*======================================================================*/
/* File name	: ARCNET.c	<BIOS version>							*/
/* Original: p:/vxworks/rcu/lib/lrom/rcs/arcnet_b.c 1.1					*/
/*----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/arcnet/RCS/arcnet.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: arcnet.c $
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
#include	<string.h>
#include	<stddef.h>
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"arcdrv.h"

extern struct com_ctxt	com_ctxt_tbl[];
extern struct comh	com_hd_tbl[];

/*======================================================================*/
/*	接続ノードを調べる（NEXT ID 以外）									*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*		   : id 		: 調べようとするノードID No. (1-255)			*/
/*	戻り値 : ノード検出 = ARC_TRUE(1)									*/
/*		   : ノードなし = ARC_FALSE(0)									*/
/*		   : 回線不正   = ARC_ILLEGAL(-1)								*/
/*----------------------------------------------------------------------*/
int _arc_node(int no, int id){
	struct com_ctxt	*pc;			/* pointer to context	*/

	pc = &com_ctxt_tbl[no];
	return(node_hunt(pc, (unsigned char)id));
}


/*======================================================================*/
/*	NEXT ID のノード番号を調べる										*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*	戻り値 : ノード検出 = NEXT-ID										*/
/*		   : ノードなし = ARC_ILLEGAL(-1)								*/
/*----------------------------------------------------------------------*/
int _arc_nextid(int no){
	struct com_ctxt	*pc;			/* pointer to context	*/

	pc = &com_ctxt_tbl[no];
	return(arc_nextid(pc));
}


/*======================================================================*/
/*	ノードの接続チェック												*/
/*----------------------------------------------------------------------*/
/*	単にマップテーブルとの照合を行う。									*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*			 int N_ID 接続チェックしたいノードID						*/
/*	戻り値 : 接続あり:ARC_TRUE(1), 接続なし:ARC_FALSE(0)				*/
/*----------------------------------------------------------------------*/
int _chk_arcnet_tbl(int no, int N_ID){
	struct comh	*ph;			/* pointer to tx header	*/
	struct com_ctxt	*pc;			/* pointer to context	*/

	pc = &com_ctxt_tbl[no];
	ph = &com_hd_tbl[no];

	if(ph->map_tbl[N_ID])
		return(ARC_TRUE);
	else
		return(ARC_FALSE);
}


/*======================================================================*/
/*	接続されているノード番号テーブルを作成する							*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*	戻り値 : next_id(1-255):正常, ARC_ILLEGAL(-1):nodeが無い			*/
/*----------------------------------------------------------------------*/
int _arc_map(int no){
	return (arc_map(no));
}


/*======================================================================*/
/*	参加ノードの変化をチェック											*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*	戻り値 ： 変化有り:ARC_FALSE(0), 無し:ARC_TRUE(1)					*/
/*----------------------------------------------------------------------*/
int _map_chk(int no)
{
	struct comh	*ph;			/* pointer to tx header	*/
	struct com_ctxt	*pc;			/* pointer to context	*/

	pc = &com_ctxt_tbl[no];
	ph = &com_hd_tbl[no];
	read_diagr(pc,(unsigned char)0xff,(unsigned char)0x00);
	if( (((pc->diagr) & (NEW_NXTID|MY_RECON)) == 0) && ((ph->recon_cntr) == 0) )
						return(ARC_TRUE);				/* Map not change */
	else
						return(ARC_FALSE);
}


/*======================================================================*/
/*	拡張 ARCNET 状態取得												*/
/*----------------------------------------------------------------------*/
/*	引き数 : int no		: デバイス番号									*/
/*======================================================================*/
int _arc_stat(int no, enum statspc cmd){
	return (arc_stat(no,cmd));
}

/* end of file */
