/*======================================================================
 * File name    : pcom003.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: 予約交信ボーレートの設定
 *                RCU への予約コマンドの送信
 *		  予約コマンドメールの送信
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/pcom003.c 1.2 1970/01/01 00:00:00 ryoji Exp $
 *$Log: pcom003.c $
 * リビジョン 1.2  1970/01/01  00:00:00  ryoji
 * 2007/03/10 05:00:00
 * 予約３桁仕様でも使用できるよう 予約番号の桁数をディップスイッチ(SW3-4)で
 * 可変できるようにする。桁数は、OFFの場合 2 で、ONの場合 3 となる。
 * 
 * リビジョン 1.1  1998/09/01  14:14:19  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-1998, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#define _PCOM003_C_

#include	<string.h>
#include	<drive\serial.h>
#include	<drive\sh7615\scidrv.h>
#include	<drive\mpsdrva.h>
#include	<rxif\rx116.h>
#include	"rxdef.h"
#include	"mbx003.h"
#include	"decimal.h"
#include	"swi001.h"
#include	"adrs.h"
#include	"txfnc_id.h"
#include	"cmfhdl.h"
#include	"tagdef.h"
#include	"fnctbl.h"
#include	"hcmd.h"

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
struct mpspk *aux_baud_chg(
	int dev_no,
	struct mpspk *p		/* default pkt */
	)
{
	switch(dev_no) {
	    case AUX0_DN:
	    case AUX1_DN:
	    case AUX2_DN:
	    case AUX3_DN:
	    case AUX4_DN:
	    case AUX5_DN:
		if(devno_comp_mesid(dev_no,TX_EXT_C1_PRE) ||
			devno_comp_mesid(dev_no,TX_EXT_C2_PRE) ) {
			if(rd_sw(SW1) & 0x40)	p->baud = C9600BPS;
			else			p->baud = C4800BPS;
		}
	}
	return p;
}

struct scipk *con_baud_chg(
	int dev_no,
	struct scipk *p		/* default pkt */
	)
{
	switch(dev_no) {
	    case AUX0_DN:
	    case AUX1_DN:
	    case AUX2_DN:
	    case AUX3_DN:
	    case AUX4_DN:
	    case AUX5_DN:
		if(devno_comp_mesid(dev_no,TX_EXT_C1_PRE) ||
			devno_comp_mesid(dev_no,TX_EXT_C2_PRE) ) {
			if(rd_sw(SW1) & 0x40)	p->baud = C9600BPS;
			else			p->baud = C4800BPS;
		}
	}
	return p;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int tx_pre_rcu(
	int c1c2,
	int cmd,
	int pre_no
	)
{
	unsigned char rbuf[RCU_MES_MAX];
	struct	arc_rcu_pkt	*r = (struct arc_rcu_pkt *)rbuf;
	struct prechg_txt2_rcu 	*ptr2;
	struct prechg_txt3_rcu 	*ptr3;
	int	err;

	if(is_arcnet(TX_RCU_PRE)) { 	/* RCU との通信が arcnet */
		r->src = (unsigned char)get_rcu_my_adrs();
		r->dst = (unsigned char)get_rcu_adrs();
		strcpy((char *)r->src_memo,"HIC-");
		strcpy((char *)r->dst_memo,">RCU");
		r->tag = (unsigned char)(c1c2 + TAG3_C1);
		r->cmd = (unsigned char)cmd;
		strcpy((char *)r->ext_ptr,"0000");
		if (get_pno_digit() == 3) {
			ptr3 = (struct prechg_txt3_rcu *)&r->param;
		} else {
			ptr2 = (struct prechg_txt2_rcu *)&r->param;
		}
	}
	else {
		rbuf[0] = (unsigned char)(c1c2 + TAG3_C1);
		rbuf[1] = (unsigned char)cmd;
		if (get_pno_digit() == 3) {
			ptr3 = (struct prechg_txt3_rcu *)&rbuf[2];
		} else {
			ptr2 = (struct prechg_txt2_rcu *)&rbuf[2];
		}
	}

	if (get_pno_digit() == 3) {
		i2adec(0 ,sizeof(ptr3->pre_no),(char *)ptr3->pre_no);
		memset(ptr3->adrs,'0',sizeof(ptr3->adrs));
		ptr3->term = '\0';
		if(cmd == PRE_NUM_CMD) {
			i2adec(pre_no ,sizeof(ptr3->pre_no),(char *)ptr3->pre_no);
			ptr3->term = '\0';
		}
	} else {
		i2adec(0 ,sizeof(ptr2->pre_no),(char *)ptr2->pre_no);
		memset(ptr2->adrs,'0',sizeof(ptr2->adrs));
		ptr2->term = '\0';
		if(cmd == PRE_NUM_CMD) {
			i2adec(pre_no ,sizeof(ptr2->pre_no),(char *)ptr2->pre_no);
			ptr2->term = '\0';
		}
	}
	err = tx_func(TX_RCU_PRE,(int)strlen((char *)rbuf),rbuf);

	return err;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int tx_pre_mail(
	int no,
	struct pcom_msg *pre
	)
{
	union aap	mbxaa,mplaa;
	int		err = 0;

	pre->priority = 0;	/*priority */
	err = MPL_ADR( &mplaa.acadr, MPL_ID0 );
	if(err)			return err;
	err = MBX_ADR( &mbxaa.acadr, (unsigned int)(PRECOM_MBX_ID2+no));
	if(err)			return err;
	err = mbx_snd_msg( pre, sizeof(struct pcom_msg),(sizeof(struct pcom_msg)/16)+1, mbxaa, mplaa);
	return err;
}

