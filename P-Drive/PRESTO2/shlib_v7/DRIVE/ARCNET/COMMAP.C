/*----------------------------------------------------------------------*
 * File name	: commap.c	/ make network map			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcnet/RCS/commap.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: commap.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/04 ウェイト処理のループ数を変更する。(p-5561対応)
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * node_hunt,arc_mapのstaticを外した
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/commap.c 1.1 1995/11/07 17:12:38 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ネットワークのノードの参加状態をマッピングする。		*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<assert.h>			/*	"		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
/*#include	<custom\custom.h>*/		/*	"		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	<drive\rxdefd.h>		/*	"		*/
#include	"comctl.h"			/* Show current		*/
#include	"comctxt.h"			/* Show current		*/

/*----------------------------------------------------------------------*/
/*	node_hunt							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	接続ノードを調べる（NEXT ID 以外）				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*pc	: pointer to context					*/
/*	id 	: 調べようとするﾉｰﾄﾞID No. (1-255)			*/
/*									*/
/* 戻り値								*/
/*	int	: 1	･･･ ﾉｰﾄﾞ検出					*/
/*		  0	･･･ ﾉｰﾄﾞなし					*/
/*		 -1	･･･ 回線断					*/
/*----------------------------------------------------------------------*/

int
node_hunt(
	struct com_ctxt		*pc,
	unsigned char		id
	)
{
	int		i, j;
	long		time=10;

	_disable();
	_arcnet_outpb(pc, CONFGR, (unsigned char)SUBAD, (unsigned char)TENTIDA);
	_arcnet_outp(pc, TIDR, id);
	_enable();
	for(j=0; j<205*8; j++);				/* wait 52uS */
	read_diagr(pc, (unsigned char)(RCVACT|TOKEN|TENTID),
				(unsigned char)(RCVACT|TOKEN|TENTID));
	HLT_TSK(&time);
	if (~(i=read_diagr(pc, (unsigned char)(RCVACT|TOKEN|TENTID), (unsigned char)TENTID))&(RCVACT|TOKEN))
						return(-1);
	return ((i & TENTID)? 1: 0);
}


/*----------------------------------------------------------------------*/
/*	hunt_nextid							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	NEXT ID のノード番号を調べる(ﾘｺﾝﾌｨｸﾞのﾁｪｯｸ無し)			*/
/*		for COM20020 REV-B~ only				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*pc	: pointer to context					*/
/*									*/
/* 戻り値								*/
/*	int	: N	･･･ ﾉｰﾄﾞ検出(NEXT-ID)				*/
/*		  -1	･･･ ﾉｰﾄﾞなし					*/
/*----------------------------------------------------------------------*/

static int
hunt_nextid(
	struct com_ctxt		*pc
	)
{
	int		nextid;

	_disable();
	_arcnet_outpb(pc, CONFGR, SUBAD, NXTID);
	nextid = _arcnet_inp(pc, NXTIDR);
	_enable();
	if(nextid)					return(nextid);
	else						return(-1);
}

/*----------------------------------------------------------------------*/
/*	arc_nextid							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	NEXT ID のノード番号を調べる(ﾘｺﾝﾌｨｸﾞのﾁｪｯｸ有り)			*/
/*		for COM20020 REV-B~ only				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*pc	: pointer to context					*/
/*									*/
/* 戻り値								*/
/*	int	: N	･･･ ﾉｰﾄﾞ検出(NEXT-ID)				*/
/*		  -1	･･･ ﾉｰﾄﾞなし					*/
/*----------------------------------------------------------------------*/
#define		TOKEN_SEEN	36			/* wait 3mS */
int
arc_nextid(
	struct com_ctxt		*pc
	)
{
	int		i, j, c;

	read_diagr(pc, (unsigned char)(RCVACT|TOKEN|DUPID), 
				(unsigned char)(RCVACT|TOKEN|DUPID));
	c = TOKEN_SEEN;
	do {
		do {
			if(c-- == 0 )			return (-1);
			for(j=0; j<205*15; j++);		/* wait 86uS */
		} while (!((i=read_diagr(pc, (unsigned char)(RCVACT|TOKEN|DUPID),
					 (unsigned char)(RCVACT|TOKEN|DUPID))) & RCVACT));
	} while (!(i & (TOKEN|DUPID)));
	return(hunt_nextid(pc));
}

/*----------------------------------------------------------------------*/
/*	arc_map								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	接続されているノード番号テーブルを作成する			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*									*/
/* 戻り値								*/
/*	int	: Next node id	･･･ 正常				*/
/*		  -1		･･･ ﾉｰﾄﾞなし				*/
/*----------------------------------------------------------------------*/

int
arc_map(
	int	devno
	)
{
	struct com_ctxt	*pc;			/* pointer to context	*/
	struct comh	*ph;			/* pointer to tx header	*/
	unsigned char	node;
	int		nextid, myid;

	ph = &com_hd_tbl[devno];
	pc = &com_ctxt_tbl[devno];
	do {
arc_map_retry:
		memset((void *)ph->map_tbl, 0, 256UL);
		if((nextid = arc_nextid(pc)) == -1)	return (-1);
		disable_interrupt();
		_arcnet_outpb(pc, CONFGR, SUBAD, NODEIDA);
		myid = _arcnet_inp(pc, NIDR);
		enable_interrupt();

		for(node=(unsigned char)(nextid+1); node != (unsigned char)myid; node++) {
			if(node == 0)		continue;
			if((ph->map_tbl[node] = (unsigned char)node_hunt(pc, node)) == -1 )
							goto arc_map_retry;
		}
	} while (nextid != (unsigned char)arc_nextid(pc));

	(ph->map_tbl)[nextid] = (unsigned char)1;
	(ph->map_tbl)[myid] = (unsigned char)1;
	(ph->map_tbl)[0] = (unsigned char)1;

	return (nextid);
}

/*----------------------------------------------------------------------*/
/*	get_arc_map							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	接続されているノード番号テーブルの取得				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*									*/
/* 戻り値								*/
/*	char far *	: マップテーブルへのfarポインタ	･･･ 正常	*/
/*			  NULL pointer			･･･ ﾉｰﾄﾞなし	*/
/*----------------------------------------------------------------------*/

char *
(get_arc_map)(
	int	devno
	)
{
	if ( arc_map(devno) == -1 ) return ((char *)0);
	if ( devno < 0 && devno >= COM_DEV_MAX ) return ((char *)0);
	return ( (char *)com_hd_tbl[devno].map_tbl );
}

/*----------------------------------------------------------------------*/
/*	arc_contact							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	任意のﾉｰﾄﾞ･ｱﾄﾞﾚｼｽの接続確認					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*	node	: node id number					*/
/*									*/
/* 戻り値								*/
/*	int	: 2	･･･ 自ノード					*/
/*		  1	･･･ 正常					*/
/*		  0	･･･ ﾉｰﾄﾞなし					*/
/*		  -1	･･･ 回線断					*/
/*----------------------------------------------------------------------*/

int
arc_contact(
	int	devno,
	int	node
	)
{
	struct com_ctxt		*pc;		/* pointer to context	*/
	int			myid, nextid;

	pc = &com_ctxt_tbl[devno];

	/* MY node ID */
	disable_interrupt();
	_arcnet_outpb(pc, CONFGR, SUBAD, NODEIDA);
	myid = _arcnet_inp(pc, NIDR);
	enable_interrupt();
	if ( node == myid )			return (2);

	/* NEXT node ID */
	if ((nextid = arc_nextid(pc)) == -1 )	return (-1);
	if ( nextid == node )			return (1);

	/* Other node ID */
	return ( node_hunt(pc, (unsigned char)node) );
}
