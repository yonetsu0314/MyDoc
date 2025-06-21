/******************************************************************************
* NORTi Network User's Hardware-dependent Functions (MiNET-MB86964)           *
*                                                                             *
*                                                                             *
* 23/Nov/1998                                                                 *
* 01/May/1999 lan_int を mb86964.c から移動                                   *
* 12/Sep/2001 制御系の判断を見直し                                         TA *
* 24/Dec/2002 SH7042/44/45で共通のためsh7042.h->sh7040.hにする             KA *
******************************************************************************/

#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "sh7040.h"

void lan_intr(void);

/* 割込み優先レベル */

#define IP          7

/* 割込みハンドラ定義情報 */

const T_DINT dint_lan = { TA_HLNG, lan_int };

/***************************************************************************
* LAN コントローラの割込み設定
*
* MB86964 /INT -> SH7040 /IRQ3
***************************************************************************/

ER lan_def_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_IRQ3, &dint_lan);    /* ベクタセット */

    psw = vdis_psw();
    sfr_setw(PFC_PACRL1,0x0008);    /* /IRQ3 LAN 割り込みポート初期化 */
    sfr_setw(INTC_ICR, 0x0100);     /* Lowレベルで割込み */
    sfr_setw(INTC_IPRA, IP);        /* 割込み優先レベル */
    vset_psw(psw);

    return ercd;
}

/***************************************************************************
* LAN コントローラ割込みハンドラ
*
***************************************************************************/

#if defined(__HITACHI__)            /* Hitach C */
#pragma interrupt(lan_int)
#endif

INTHDR lan_int(void)
{
    ent_int();
    lan_intr();
    ret_int();
}

/* end */
