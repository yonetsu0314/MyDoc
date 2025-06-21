/*----------------------------------------------------------------------*
 * File name	: comiorx.c	/					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comiorx.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: comiorx.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/07/22 18:25
 * 構造体cthのallをintからshortに変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comiorx.c 1.1 1995/11/07 17:12:34 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	割り込みハンドラより起床された、APL側の受信タスクに受信した	*/
/*	テキストを提供する。尚マルチパケット転送を可能にする行うため	*/
/*	、ヘッダー部(struct pkth)がテキスト部の先頭に組み込まれてい	*/
/*	るが、APL側に渡す時点で除去される。				*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<assert.h>			/*	"		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	<drive\rxdefd.h>		/*	"		*/
#include	"comctl.h"			/* Show current		*/
#include	"comctxt.h"			/* Show current		*/
#define		STORAGE
#include	"comrx.h"			/*	"		*/
#undef		STORAGE


#define	QUE_MAX	16				/* maximum no of que	*/

enum {						/* formal text		*/
	LEN,					/* text length		*/
	SID	= 2,				/* source ID		*/
	DID,					/* destination ID	*/
	TXT,					/* text			*/
	BUF_MAX	= LPKT_MAX,
};

struct cth {
	unsigned char	sid;			/* source ID		*/
	unsigned char	did;			/* destination ID	*/
	short		all;			/* total packet-1	*/
	unsigned char	*buf;			/* pointer to alloc. buf*/
}
static que[COM_DEV_MAX][QUE_MAX];

/*----------------------------------------------------------------------*/
/*	ram_rd								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	COM20020の内部RAMへｱﾄﾞﾚｽ指定してﾃﾞｰﾀを読み出す。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ctxt	: pointer contxt					*/
/*	adrs	: 内部RAMのｱﾄﾞﾚｽ(0-7ff)					*/
/*	data 	: ﾗｲﾄﾃﾞｰﾀ						*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

static int
ram_rd(
	struct com_ctxt	*pnt,
	unsigned int	adrs
	)
{
	_arcnet_outp2(pnt, ADRHR, (unsigned short)(((RDDATA|AUTOINC)<<8)|adrs));
	return(_arcnet_inp(pnt, DATR));
}

/*----------------------------------------------------------------------*/
/*	read_header							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃｷｽﾄのﾍｯﾀﾞｰ部をCOM20020のﾊﾞｯﾌｧから読み出す。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pc	: pointer to contxt					*/
/*	ph	: pointer to header					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

static void
read_header(
	struct com_ctxt	*pc,			/* pointer to context	*/
	struct pkth	*ph			/* packet ctl header	*/
	)
{
#if	defined(PKTH_T)
	int		i;
	char		*p;

	for(i =0, p =(void *)ph; i < sizeof(*ph) ; i++, p++)
		*p = (char)_arcnet_inp(pc, DATR);
#else
	memset(ph, 0, sizeof(struct pkth));
#endif
}

/*----------------------------------------------------------------------*/
/*	arc_dpm_read							*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	マルチパケットの受信テキストを連結し、1テキストに合成する。	*/
/*	通常のシングルパケットのテキストは、引数の**rtpに直接ストア	*/
/*	ーするが、マルチパケットのテキストでは、アロケートされたメモ	*/
/*	リーの内容を返すために、そのアドレスを*rtpに返す。従ってコー	*/
/*	リー側では、処理後メモリー資源の解放を必要とする。その判断は	*/
/*	引数として与えたrtpの値が変化したことで分かる。			*/
/*									*/
/* 注)	この受信モジュールでは、マルチパケット転送をサポートしている	*/
/*	が、従来の受信タスク(crxtsk.c)とソフトウェアのロジックが異な	*/
/*	るため、マルチパケットのテキストの合成中は、コーリー側に返す	*/
/*	値の無い空関数となる。						*/
/*									*/
/*			 <------------ struct comfmt  --------->	*/
/*			+2	+3	+4				*/
/*	+-------+-------+-------+-------+------- /// -----------+	*/
/*	|      len	| src	| dst	|	text		|	*/
/*	+-------+-------+-------+-------+------- /// -----------+	*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*	**rtp	: &pointer to rcv buffer				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ エラー					*/
/*----------------------------------------------------------------------*/

int
arc_dpm_read(
	int		devno,
	unsigned char	**rtp
	)
{
	struct com_ctxt	*pc;			/* pointer to context	*/
	struct rxh	*ph;			/* pointer to rx header	*/
/*	struct comfmt	*pt;*/			/* pointer to text	*/
	struct pkth	pkh;			/* packet ctl header	*/
	unsigned char	*txt, cnt,		/* pointer to char data	*/
			*tmp;
	unsigned int	adr;			/* RAM address		*/
	int		err =0, len, max, i;
	struct cth	*pq, cur;
	short		*tmp1;

	ph = &com_rxhd[devno];
	pc = &com_ctxt_tbl[devno];
	pq = &que[devno][0];
	tmp = *rtp;
/*------------------------------------------------------*/
/*	ARC-NET受信バッファからの転送			*/
/*							*/
/*------------------------------------------------------*/

	ADV_WAI_SEM(err, com_nest[devno], TIME_OUT,
		    com_sem[devno].acadr, &com_time_out);
	assert(!err); if (err)					return(-1);

	tmp[SID] = (unsigned char)ram_rd(pc, adr= ph->getp <<8);
	tmp[DID] = (unsigned char)_arcnet_inp(pc, DATR);
	if(cnt = (unsigned char)_arcnet_inp(pc, DATR))
		max = SPKT_MAX;
	else {
		max = LPKT_MAX;
		cnt = (unsigned char)_arcnet_inp(pc, DATR);
	}
	_arcnet_outp2(pc, ADRHR, (unsigned short)(((RDDATA|AUTOINC)<<8)|(adr+cnt)));
	read_header(pc, &pkh);
	txt = &tmp[TXT];
	for(i = cnt+SIZE_PKTH ; i < max ; i++)
		*(txt++) = (unsigned char)_arcnet_inp(pc, DATR);
	len = txt- &tmp[SID];

	i = max/SPKT_MAX;
	_disable();
	if((ph->getp += i)>= RXPAGE_MAX)  ph->getp =RXPAGE_MIN;
	if(!(pc->reg[INTMR] & RI))  arc_ena_rx(pc, ph);
	_enable();

	ADV_SIG_SEM(err, com_nest[devno], com_sem[devno].acadr);
	assert(!err); if (err)					return(-1);

	tmp1 = (short *)(tmp + LEN);
	*tmp1 = (short)len;
	if(pkh.all == 0)					return(0);
/*------------------------------------------------------*/
/*	マルチパケットのテキスト処理			*/
/*							*/
/*------------------------------------------------------*/

	*rtp = NULL;
	if(pkh.all == pkh.rest) {
	    for(i= 0; i < QUE_MAX; i++) {
	    	_disable();
	    	if(chrcmp((char *)(pq+i), (char)0, (int)(sizeof(*(pq+i)))) == 0) {
		    (pq+i)->sid = tmp[SID];
		    (pq+i)->did = tmp[DID];
		    (pq+i)->all = pkh.all;
		    _enable();
		    (pq+i)->buf = malloc((size_t)((pkh.all+1)*SPKT_MAX));
		    assert((pq+i)->buf != NULL);
		    memcpy((void *)((pq+i)->buf), (void *)&tmp[LEN],
			     (unsigned long)(*tmp1 += sizeof(short)));	return(0);
		}
		_enable();
	    }
	    assert(i < QUE_MAX);
	}
	else if(pkh.all > pkh.rest) {
	    cur.sid = tmp[SID];
	    cur.did = tmp[DID];
	    cur.all = pkh.all;
	    cur.buf = NULL;
	    for(i= 0; i < QUE_MAX; i++) {
		if(memcmp((void *)(pq+i), (void *)&cur, sizeof(cur)-sizeof(cur.buf))== 0) {
		    memcpy((void *)((pq+i)->buf+ *(short *)(pq+i)->buf), (void *)&tmp[TXT],
			     (unsigned long)(len -=(sizeof(tmp[SID])+sizeof(tmp[DID]))));
		    tmp1 = (short *)((pq+i)->buf);
		    *tmp1 += (short)len;

	    	    if(pkh.rest == 0) {
		    	*rtp = (pq+i)->buf;
			*tmp1 -= sizeof(short);
		    	_disable();
		    	memset((void *)(pq+i), 0, sizeof(*(pq+i)));
		    	_enable();
		    }						return(0);
	    	}
		assert(i < QUE_MAX);
	    }
	}
	else
	    assert(pkh.all > pkh.rest);

	return(-1);
}
