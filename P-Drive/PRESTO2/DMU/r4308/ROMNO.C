/*======================================================================
 * File name    : romno.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: ロム番号要求の応答
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/romno.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: romno.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-2000, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#define _ROMNO_C_

#include	<string.h>			/* ANSI std		*/

#include	<rxif\rx116.h>
#include	<rzcmd/rzcmd.h>
#include	<rzcmd/rzr025a.h>
#include	<rzcmd/rzi009a.h>

#include	"tagdef.h"
#include	"adrs.h"
#include	"fnctbl.h"
#include	"txfnc_id.h"
#include	"romno.h"

/*----------------------------------------------------------------------*/
/* void rom_no_req(int dev_no,int len,const unsigned char *buf,		*/
/*	const unsigned char *non_accpt_cmd,				*/
/*	const unsigned char *accpt_cmd )				*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*    RCU から受信した、ロム番号リクエストコマンドに応答する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/

void rom_no_req(
	int dev_no,
	int len,
	const unsigned char *buf,
	const unsigned char *non_accpt_cmd,
	const unsigned char *accpt_cmd
	)
{
	unsigned char 			rbuf[RCU_MES_MAX];
	struct	arc_rcu_pkt1		*r = (struct arc_rcu_pkt1 *)rbuf;
	const struct arc_rcu_pkt1	*p = (const struct arc_rcu_pkt1 *)buf;
	unsigned char 		cmd;
	RZI009T			rno;

	if(!len)			return;
	if(!is_arcnet(dev_no))		return;

	cmd = p->cmd;
	
	/* TAG の付いたコマンドは無視 */
	
	if( (cmd == TAG1_C1) || (cmd == TAG1_C2) )	return;
	if( (cmd == TAG2_C1) || (cmd == TAG2_C2) )	return;
	if( (cmd == TAG3_C1) || (cmd == TAG3_C2) )	return;
	if( (cmd == TAG4_C1) || (cmd == TAG4_C2) )	return;
	if(!is_accpt_cmd(cmd,non_accpt_cmd,accpt_cmd))	return;

	rz_scan_req_prgno((struct fmt_req_prgno_v3 *)&p->cmd);

	if(rzr025s.node_id != (unsigned char)get_rcu_my_adrs()) 	return;

	rno.brd_id = rzr025s.brd_id;			/* デバイスID番号 */
	rno.dev_no = rzr025s.dev_no;			/* デバイス番号	  */
	rno.node_id = rzr025s.node_id;			/* ノード番号	  */
	strcpy(rno.rom_id,get_lib_id());		/* ROM ID 文字列  */
	rno.len = (unsigned char)strlen(rno.rom_id);	/* 文字列の長さ	  */

	*(char *)rz_print_prgno((struct fmt_prgno_v3 *)&r->cmd,&rno) = '\0';
	r->src = (unsigned char)get_rcu_my_adrs();
	r->dst = (unsigned char)get_rcu_adrs();
	strcpy((char *)r->src_memo,"HIC-");
	strcpy((char *)r->dst_memo,">RCU");
	r->cmd = (unsigned char)PNUM_CMD;
	strncpy((char *)r->ext_ptr,"0000",4);

	tx_func(TX_RCU_NORMAL,(int)strlen((char *)rbuf),rbuf);
}
