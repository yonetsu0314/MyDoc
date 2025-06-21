/*---------------------------------------------------------------------
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/ether/rcs/eth_init.c 1.2
 * $Header: p:/presto2/shlib_v7/drive/sh7615/ether/rcs/eth_init.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: eth_init.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

#include <kernel.h>
#include "sh7615\7615.h"
#include "drive\eth_def.h"
#include "drive\rxdefd.h"

#define EDMAC_VECTNO 73

/* Object ID */
//#define FLG_TST     1       /* Event flag ID */
#define SEM_TST     1      /* Semaphore ID */
#define MPF_COM     1       /* Memory pool ID */

/* message structure */
#define COMBUFSZ    256
#define NCOMMPL     32

typedef struct t_commsg
{
    struct t_commsg *next;
    B buf[COMBUFSZ];

} T_COMMSG;

const T_CMPF cmpf = { TA_TFIFO,			NCOMMPL,	sizeof(T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem = { TA_TFIFO,			0,			1,		(B *)"sem1" };
const T_CFLG cflg = { TA_WMUL,			0,			(B *)"flag" };

const T_DINT dint_etherhdr  = { TA_HLNG, (void *)irhan00 };

void init_sys_ether(void){

//saka	INTC.IPRB.WORD = (unsigned short)0xd000;	/* EDMAC/FRT 割り込みレベル設定 */
	INTC.IPRB.WORD = (unsigned short)0x1000;	/* EDMAC/FRT 割り込みレベル設定 */
	INTC.VCRA.WORD = (unsigned short)(EDMAC_VECTNO<<8);	/* EDMAC 割り込みベクタ番号７３ (0x4900)*//*999999999*/

	/* create objects */

//	cre_sem(SEM_TST,   &csem);  /* Create semaphore */
	cre_flg(ETHER_EVF_ID,   &cflg);  /* Create event flag */
//	cre_mpf(MPF_COM,   &cmpf);  /* Create fixed memory pool */

	def_int((INHNO)EDMAC_VECTNO, &dint_etherhdr);
}
