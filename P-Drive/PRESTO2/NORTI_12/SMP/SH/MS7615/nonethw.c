/****************************************************************************
* NORTi3 Network User's Hardware-dependent Functions                        *
*                                                                           *
*  File name : nonethw.c                                                    *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                 *
*                                                                           *
* 17/Apr/2000 作成                                                    MiSPO *
* 12/Sep/2001 制御系の判断を見直し                                       TA *
****************************************************************************/

#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "sh7615.h"

void lan_intr(void);

/* 割込み優先レベル */

#define IP          7

/* 割込みハンドラ定義情報 */

const T_DINT dint_lan = { TA_HLNG, lan_int };

/***************************************************************************
* LAN コントローラの割込み設定
*
* internal peripheral
***************************************************************************/

ER lan_def_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_EINT, &dint_lan);   /* ベクタセット */

    psw = vdis_psw();
    sfr_outw(INTC_IPRB, sfr_inw(INTC_IPRB) | (IP << 12));
    sfr_outw(INTC_VCRA, sfr_inw(INTC_VCRA) | (INT_EINT << 8));	
    vset_psw(psw);

    return ercd;
}

/***************************************************************************
* LAN コントローラ割込みハンドラ
*
***************************************************************************/

#if defined(__HITACHI__)                /* Hitach C */
#pragma interrupt(lan_int)
#endif

INTHDR lan_int(void)
{
    ent_int();
    lan_intr();
    ret_int();
}

/* end */
