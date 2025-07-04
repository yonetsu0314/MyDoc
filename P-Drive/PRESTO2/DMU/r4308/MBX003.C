/*======================================================================
 * File name    : mbx003.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	汎用メイルボックス用関数
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/mbx003.c 1.2 1998/05/15 10:19:23 yonetsu Exp $
 *$Log: mbx003.c $
 * リビジョン 1.2  1998/05/15  10:19:23  yonetsu
 * 関数 mbx_rcv_msg_clr を追加
 * 
 * リビジョン 1.1  1997/05/28  13:09:04  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1996, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<assert.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show include		*/
#include	<memory.h>			/* Show drive		*/

#include	"rxdef.h"			/* Show current dir.	*/

#ifndef CPU_SH
#define CPU_SH
#endif
#include	<kernel.h>

/*----------------------------------------------------------------------*/
/*	mbx_snd_msg							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	メッセージを送る。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*messege	: メッセージを示すポインター			*/
/*	msg_size	: メッセージの大きさ				*/
/*	blk_size	: メモリーブロックのサイズ（１６ビット単位）	*/
/*	mplaa		: メモリープールのアクセスアドレス		*/
/*	mbxaa		: メールボックスのアクセスアドレス		*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  !0	･･･ error code				*/
/*----------------------------------------------------------------------*/
int
mbx_snd_msg(
	void		*message,
	int		msg_size,
	int		blk_size,
	union	aap	mbxaa,
	union	aap	mplaa
	)
{
//	union	aap	blkaa = {0};
//	long		time_out = 0L;
	int		err;			/* error code		*/
	TT_MSG		*tmsg;
	
	err = get_mpl((ID)mplaa.acadr, (unsigned int)(blk_size*16), &tmsg);
/*	err = GET_BLK( ENDLESS, &blkaa.acadr, mplaa.acadr, blk_size
			, &time_out );*/
	if(err)							return(err);
	memcpy( &tmsg->buf.cbuf[0], message, (size_t)msg_size );
	err = snd_mbx( (ID)mbxaa.acadr, tmsg );
/*	err = SND_MSG( mbxaa.acadr, blkaa.acadr );*/
	return(err);
}

/*----------------------------------------------------------------------*/
/*	mbx_rev_msg							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	メッセージを受ける。（タイムアウトなし）			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*messege	: メッセージを示すポインター			*/
/*	msg_size	: メッセージの大きさ				*/
/*	mbxaa		: メールボックスのアクセスアドレス		*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  !0	･･･ error code				*/
/*----------------------------------------------------------------------*/
int
mbx_rcv_msg(
	void		*message,
	int		msg_size,
	union	aap	mbxaa
	)
{
//	union	aap	blkaa = {0};
//	long		time_out = 0L;
	int		err;			/* error code		*/
	TT_MSG		*tmsg;

	err = rcv_mbx((ID)mbxaa.acadr, &tmsg);
//	err = RCV_MSG( ENDLESS, &blkaa.acadr, mbxaa.acadr, &time_out );
	if(err)							return(err);
	memcpy( message, &tmsg->buf.cbuf[0], (size_t)msg_size );
	err = rel_mpl(1, tmsg);
//	err = REL_BLK(blkaa.acadr);
	return(err);
}

/*----------------------------------------------------------------------*/
/*	mbx_rev_msg_t							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	メッセージを受ける。（タイムアウト有無設定可能）		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*messege	: メッセージを示すポインター			*/
/*	msg_size	: メッセージの大きさ				*/
/*	mbxaa		: メールボックスのアクセスアドレス		*/
/*      time_out	:  0: タイムアウトなし (mbx_rcv_msg と同じ)	*/
/*			  !0: タイムアウトあり				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  !0	･･･ error code				*/
/*			            SVCE_TIMEOUT（タイムアウト）	*/
/*----------------------------------------------------------------------*/
int
mbx_rcv_msg_t(
	void		*message,
	int		msg_size,
	union	aap	mbxaa,
	long		time_out
	)
{
//	union	aap	blkaa = {0};
	int		err,mode;			/* error code		*/
	TT_MSG		*tmsg;
	
	mode = ENDLESS;
	if(time_out)	mode = TIME_OUT;
	err = trcv_mbx((ID)mbxaa.acadr, &tmsg, time_out);
//	err = RCV_MSG( mode, &blkaa.acadr, mbxaa.acadr, &time_out );
	if(err)							return(err);
	memcpy( message, &tmsg->buf.cbuf[0], (size_t)msg_size );
	err = rel_mpl(1, tmsg);
//	err = REL_BLK(blkaa.acadr);
	return(err);
}

/*----------------------------------------------------------------------*/
/*	mbx_rev_msg_clr							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	メールボックスのメッセージをクリアーする			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	mbxaa		: メールボックスのアクセスアドレス		*/
/*									*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
void
mbx_rcv_msg_clr(
	union	aap	mbxaa
	)
{
//	union	aap	blkaa = {0};
	int		err,err1;			/* error code		*/
	long		time_out = 1L;
	TT_MSG		*tmsg;
	
	do {
		err = trcv_mbx((ID)mbxaa.acadr, &tmsg, time_out);
//		err = RCV_MSG( TIME_OUT, &blkaa.acadr, mbxaa.acadr, &time_out );
		if(!err) {
			err1 = rel_mpl(1, tmsg);
//			err1 = REL_BLK(blkaa.acadr);
			assert(!err1);
		}
	} while(!err);
}
