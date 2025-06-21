/*----------------------------------------------------------------------*
 * File name	: comtx.c	/ arcnet tx				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comtx.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: comtx.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2004/07/21 15:30:00
 * write関数arcnet_txについて、ARCNET IC が無い場合、
 * 送信長さ０の応答をするようにした。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/09/27 11:30
 * マルチパケット送信時のリトライ間隔を100msecから5msecに変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comtx.c 1.2 1996/10/08 12:11:50 syuji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ユーザーバッファの内容を、SHORTパケットで分割し送信を行う。	*/
/*	尚テキストをマルチパケットで転送を行うため、このドライバー	*/
/*	内でヘッダー部(struct pkth)がテキスト部の先頭に組み込まれ	*/
/*	る。								*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/* ANSI std		*/
#include	<string.h>			/* ANSI std		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
/*#include	<custom\custom.h>*/		/* Show bios		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"comctl.h"			/* Show current		*/
#include	"comctxt.h"			/* Show current		*/

#define	TX_RETRY	14
static long		retry_interval = 5;

/*----------------------------------------------------------------------*/
/*	ram_wr								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	COM20020の内部RAMへｱﾄﾞﾚｽ指定してﾃﾞｰﾀを書き込む。		*/
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

static void
ram_wr(
	struct com_ctxt	*pnt,
	unsigned int	adrs,
	unsigned int	data
	)
{
	_arcnet_outp2(pnt, ADRHR, (unsigned short)((AUTOINC<<8)|adrs));
	_arcnet_outp(pnt, DATR, (BYTE)data);

}


/*----------------------------------------------------------------------*/
/*	check_tx_pend							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	送信ペンディングの状態取得					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pc	: pointer to contxt					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ ペンディング無し			*/
/*			  -2	･･･ 回線断				*/
/*			  -3	･･･ 宛先ﾉｰﾄﾞが無い			*/
/*			  -4	･･･ ペンディング残留			*/
/*----------------------------------------------------------------------*/

int
check_tx_pend(
		int	devno			/* device number	*/
	)
{
	int		i;
	int		adr;
	struct com_ctxt	*pc;			/* pointer to context	*/

	pc = &com_ctxt_tbl[devno];
	if (arc_nextid(pc) == -1)			return (-2);
	if (!((i=_arcnet_inp(pc, STATR)) & TA))		return (-4);
	if (!(i & TMA)) {
		if (tx_init_flag[devno])		return (0);
		_disable();
		adr = _arcnet_inp2(pc, ADRHR)
		      &(((RDDATA|AUTOINC|ADRH_MAX)<<8)|(ADRL_MAX));
		_arcnet_outp2(pc, ADRHR, (unsigned short)((RDDATA<<8)|1));
		i = _arcnet_inp(pc, DATR);
		_arcnet_outp2(pc, ADRHR, (unsigned short)adr);
		_enable();
		if (i) {
			tx_init_flag[devno] = 1;	return (-3);
		}
	}
	return (0);
}


/*----------------------------------------------------------------------*/
/*	write_header							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃｷｽﾄのﾍｯﾀﾞｰ部をCOM20020のﾊﾞｯﾌｧへ書き込みを行う。		*/
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
write_header(
	struct com_ctxt	*pc,			/* pointer to context	*/
	struct pkth	*ph			/* packet ctl header	*/
	)
{
#if	defined(PKTH_T)
	    _arcnet_outp(pc, DATR, ph->all);
	    _arcnet_outp(pc, DATR, ph->rest);
#endif
}

/*----------------------------------------------------------------------*/
/*	arcnet_io_write							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	テキスト(バイナリー可)をデバイスに書き込み、送信コマンドを	*/
/*	発行する。							*/
/*	マルチパケット転送処理中は終了するまでリトライする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*u_buf		: string buffer					*/
/*	len		: number of byte				*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数			*/
/*			  -1	･･･ faital error			*/
/*			  -2	･･･ 回線断				*/
/*			  -3	･･･ 宛先ﾉｰﾄﾞが無い			*/
/*			  -4	･･･ ﾘﾄﾗｲ･ｴﾗｰ				*/
/*----------------------------------------------------------------------*/

int
arcnet_io_write(
	int			channel,	/* channel number	*/
	unsigned char		*u_buf,		/* string buffer pointer*/
	int			len		/* number of byte	*/
	)
{
	struct com_ctxt	*pc;			/* pointer to context	*/
	struct comh	*hp;			/* pointer to tx header	*/
	struct comfmt	*pt = (void *)u_buf;	/* pointer to text	*/
	struct pkth	pkh;			/* packet ctl header	*/
	unsigned char	*txt,		/* pointer to char data	*/
			did, cnt;
	int		err =0, siz, n, i;

	hp = &com_hd_tbl[channel];
	pc = &com_ctxt_tbl[channel];
	did = pt->dst_node;
	u_buf += sizeof(pt->src_node)+sizeof(pt->dst_node);
	ADV_WAI_SEM(err, com_tx_nest[channel], TIME_OUT,
		    com_tx_sem[channel].acadr, &com_time_out);
	if(err)							return(-1);
/*------------------------------------------------------*/
/*	送信バッファへの転送				*/
/*							*/
/*------------------------------------------------------*/

	siz = len;
	pkh.all = pkh.rest = (unsigned char)((len-1)/((SPKT_MAX-3)-SIZE_PKTH));
	do {
	    if(pkh.rest == 0)  cnt = (unsigned char)(SPKT_MAX-(SIZE_PKTH+siz));
	    else	       cnt = (unsigned char)(SPKT_MAX-(SPKT_MAX-3));
	    n = SPKT_MAX-1;

	    while (i=check_tx_pend(channel)) {
	    	if (i!=-4 || pkh.rest==pkh.all) {
			ADV_SIG_SEM(err, com_tx_nest[channel],
				com_tx_sem[channel].acadr);
							return (err?-1:i);
		}
		HLT_TSK(&retry_interval);
	    }

	    txt = u_buf;
	    ADV_WAI_SEM(err, com_nest[channel], TIME_OUT,
		com_sem[channel].acadr, &com_time_out);
	    if(err)						return(-1);
	    if(pkh.all == pkh.rest) {
		_arcnet_outpb(pc, CONFGR, (unsigned char)(TXEN|SUBAD), (unsigned char)(TXEN|NODEIDA));
		ram_wr(pc, (unsigned int)0, (unsigned int)_arcnet_inp(pc, NIDR));
		_arcnet_outp(pc, DATR, did);
	    }
	    _arcnet_outp2(pc, ADRHR, (unsigned short)((AUTOINC<<8)|2));
	    if(n > SPKT_MAX-1)  _arcnet_outp(pc, DATR, (unsigned char)0);
	    _arcnet_outp(pc, DATR, cnt);

	    _arcnet_outp2(pc, ADRHR, (unsigned short)((AUTOINC<<8)|cnt));
	    write_header(pc, &pkh);
	    for(n -= SIZE_PKTH ; n >= cnt ; n--)
	        _arcnet_outp(pc, DATR, *(txt++));
	    _arcnet_outp(pc, COMMR, (BYTE)(ENTX|PFNN(0)));
	    tx_init_flag[channel] = 0;
	    ADV_SIG_SEM(err, com_nest[channel], com_sem[channel].acadr);
	    if(err)						return(-1);
	    siz -= (txt-u_buf);
	    u_buf = txt;
	} while(pkh.rest--);

	ADV_SIG_SEM(err, com_tx_nest[channel], com_tx_sem[channel].acadr);
	if(err)							return(-1);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	arcnet_io_puts							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	テキストのストリングスにスタート/エンド･キャラクタを付加し、	*/
/*	デバイスに書き込み後、送信コマンドを発行する。			*/
/*	マルチパケット転送処理中は終了するまでリトライする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*u_buf		: string buffer					*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数			*/
/*			  -1	･･･ error				*/
/*			  -3	･･･ 宛先ﾉｰﾄﾞが無い			*/
/*			  -4	･･･ ﾘﾄﾗｲ･ｴﾗｰ				*/
/*----------------------------------------------------------------------*/

int
arcnet_io_puts(
	int			channel,	/* channel number	*/
	unsigned char		*u_buf		/* string buffer pointer*/
	)
{
	struct com_ctxt	*pc;			/* pointer to context	*/
	struct comh	*hp;			/* pointer to tx header	*/
	struct comfmt	*pt = (void *)u_buf;	/* pointer to text	*/
	struct pkth	pkh;			/* packet ctl header	*/
	unsigned char	*txt,		/* pointer to char data	*/
			did, cnt;
	int		err =0, len, siz, n, j, i;

	hp = &com_hd_tbl[channel];
	pc = &com_ctxt_tbl[channel];
	if((len = strlen((char *)&pt->text)) <= 0)			return(-1);
	did = pt->dst_node;
	u_buf += sizeof(pt->src_node)+sizeof(pt->dst_node);
	ADV_WAI_SEM(err, com_tx_nest[channel], TIME_OUT,
		    com_tx_sem[channel].acadr, &com_time_out);
	if(err)							return(-1);
/*------------------------------------------------------*/
/*	送信バッファへの転送				*/
/*							*/
/*------------------------------------------------------*/

	siz = len;
	pkh.all = pkh.rest = (unsigned char)((len+1)/((SPKT_MAX-3)-SIZE_PKTH));
	do {
	    j = 0;
	    if(pkh.all == pkh.rest)  ++j;
	    if(pkh.rest == 0)  cnt = (unsigned char)(SPKT_MAX-(SIZE_PKTH+siz+(++j)));
	    else	       cnt = (unsigned char)(SPKT_MAX-(SPKT_MAX-3));
	    n = SPKT_MAX-1;

	    while (i=check_tx_pend(channel)) {
	    	if (i!=-4 || pkh.rest==pkh.all) {
			ADV_SIG_SEM(err, com_tx_nest[channel],
				com_tx_sem[channel].acadr);
							return (err?-1:i);
		}
		HLT_TSK(&retry_interval);
	    }

	    txt = u_buf;
	    ADV_WAI_SEM(err, com_nest[channel], TIME_OUT,
		com_sem[channel].acadr, &com_time_out);
	    if(err)						return(-1);
	    if(pkh.all == pkh.rest) {
		_arcnet_outpb(pc, CONFGR, (unsigned char)(TXEN|SUBAD), (unsigned char)(TXEN|NODEIDA));
		ram_wr(pc, (unsigned int)0, (unsigned int)_arcnet_inp(pc, NIDR));
		_arcnet_outp(pc, DATR, did);
	    }
	    _arcnet_outp2(pc, ADRHR, (unsigned short)((AUTOINC<<8)|2));
	    if(n > SPKT_MAX-1)  _arcnet_outp(pc, DATR, (unsigned char)0);
	    _arcnet_outp(pc, DATR, cnt);

	    _arcnet_outp2(pc, ADRHR, (unsigned short)((AUTOINC<<8)|cnt));
	    write_header(pc, &pkh);
	    if(pkh.all == pkh.rest)  _arcnet_outp(pc, DATR, LF);
	    for(n -= (j+SIZE_PKTH) ; n >= cnt ; n--)
	    	_arcnet_outp(pc, DATR, *(txt++));
	    if(pkh.rest == 0)	     _arcnet_outp(pc, DATR, CR);
	    _arcnet_outp(pc, COMMR, (BYTE)(ENTX|PFNN(0)));
	    tx_init_flag[channel] = 0;
	    ADV_SIG_SEM(err, com_nest[channel], com_sem[channel].acadr);
	    if(err)						return(-1);
	    siz -= (txt-u_buf);
	    u_buf = txt;
	} while(pkh.rest--);

	ADV_SIG_SEM(err, com_tx_nest[channel], com_tx_sem[channel].acadr);
	if(err)							return(-1);
	return(len);
}

int
arcnet_tx(
	int		channel,		/* channel number	*/
	unsigned char	*u_buf,			/* string buffer pointer*/
	int		len			/* number of byte	*/
	)
{
	if(arc_exist_flag[channel]==0) return(0);/* ICが無い場合、length=0を返す */
	if(len)  return( arcnet_io_write( channel, u_buf, len));
	else	 return( arcnet_io_puts( channel, u_buf));
}
