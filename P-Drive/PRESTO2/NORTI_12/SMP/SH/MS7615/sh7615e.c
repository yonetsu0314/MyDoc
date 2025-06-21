/******************************************************************************
* NORTi3 Network LAN Control Driver (SH7615 peripheral)                       *
*                                                                             *
*  File name : sh7615e.c                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 13/Apr/2000 作成                                                      MiSPO *
* 21/Oct/2000 EtherC/E-DMACのソフトreset後の待ち追加                 OK(MiSPO)*
* 25/Apr/2001 イベントフラグの使用をやめる／割り込み頻度抑制         OK       *
* 14/May/2002 Modified FIFO config for SH7616                        OK       *
******************************************************************************/

/*

1フレーム/マルチバッファ

          本サンプルは、SH7615 Etherコントローラが持つ1フレーム/1バッファ
          機能に対応しております。1フレーム/マルチバッファ機能を使用する
          場合には、カスタマイズが必要です。

強制ループバックの設定

          SH7615内部 Etherコントローラ折り返しの設定を有効にする場合には
          LOOP_BACK マクロを定義してコンパイルしてください。

          (例) shc <option> -def=LOOP_BACK sh7615e.c
                            ~~~~~~~~~~~~~~
          (例) gcc <option> -DLOOP_BACK sh7615e.c
                            ~~~~~~~~~~~
          (例) ccsh <options> -DLOOP_BACK sh7615e.c
                              ~~~~~~~~~~~

Auto Negotiationの設定

          Auto Negotiation(PHYが持つ)機能を有効にする場合には、AUTONEGO マクロ
          を定義してコンパイルしてください。AUTONEGO マクロを定義した場合には
          SPEED(10/100Mbps)の選択は無効です。デフォルトでは Auto Negotiation
          機能は無効です。全２重／半２重の自動設定は本サンプルではサポートされ
          ていません。

          (例) shc <option> -def=AUTONEGO sh7615e.c
                            ~~~~~~~~~~~~~
          (例) gcc <option> -DAUTONEGO sh7615e.c
                            ~~~~~~~~~~
          (例) ccsh <options> -DAUTONEGO sh7615e.c
                              ~~~~~~~~~~

スピードの設定

          Auto Negotiation機能を使わないで固定モードで使用する場合には SPEED
          マクロで動作スピードを変更できます。デフォルトでは 10Mbps になりま
          す。SPEED マクロには、10 または 100 の値を設定してください。
          それ以外の設定はエラーとなります。また AUTONEGO マクロを定義した場合
          には SPEED(10/100Mbps)の選択は無効です。
          次の設定では 100Mbps になります。

          (例) shc <option> -def=SPEED=100 sh7615e.c
                            ~~~~~~~~~~~~~~
          (例) gcc <option> -DSPEED=100 sh7615e.c
                            ~~~~~~~~~~~
          (例) ccsh <options> -DSPEED=100 sh7615e.c
                              ~~~~~~~~~~~

全２重／半２重の設定

          全２重を設定する場合には、FULL マクロを定義してコンパイルしてくださ
          い。デフォルトでは 半２重に設定されます。Auto Negotiationの全２重／
          半２重選択は固定されます。

          (例) shc <option> -def=FULL sh7615e.c
                            ~~~~~~~~~
          (例) gcc <option> -DFULL sh7615e.c
                            ~~~~~~
          (例) ccsh <options> -DFULL sh7615e.c
                              ~~~~~~

*/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "dp83843.h"
#include "sh7615e.h"
#include "sh7615.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

#ifndef LOOP_BACK
#define LOOP_BACK 0   /* EtherCの強制ループバック禁止 */
#else
#undef LOOP_BACK
#define LOOP_BACK 1   /* EtherCの強制ループバック許可 */
#endif

#ifndef AUTONEGO
#define AUTONEGO  0   /* デフォルトでは、Auto Negotiationは無効 */
#else
#undef AUTONEGO
#define AUTONEGO  1
#endif

#ifndef SPEED
#define SPEED     10  /* デフォルトのスピードは、10Mbps */
#else
#if SPEED != 10 && SPEED != 100
#error NO SUPPORTED SPEED
#endif
#endif

#ifndef FULL
#define FULL      0   /* デフォルトのDUPLEXは、半２重(HALF)です */
#else
#undef FULL
#define FULL      1
#endif

/* ユーザー定義の関数/マクロ */

#ifndef ALIGN
#define ALIGN(p,n)  ((((UW)(p)) + (((UW)(n)) - 1)) & (~(((UW)(n)) - 1)))
                   /* アラインメント調整マクロ
                    *  p: 計算前アドレス
                    *  n: 境界値(2のべき乗であること)
                    */
#endif

#define ETHER_TX_BUF_NUM    16    /* Tx buffer number */
#define ETHER_RX_BUF_NUM    16    /* Rx buffer number */
#define ETHER_TX_BUF_SIZ    0x600 /* =1520+@=1536 Tx buffer size(boundary is 16 & >1518) */
#define ETHER_RX_BUF_SIZ    0x600 /* =1520+@=1536 Rx buffer size(boundary is 16 & >1518) */

/* 送受信割り込みステータス定義 */

#define EV_ETH_RX (EESIPR_FRIP|EESIPR_RDEIP |EESIPR_RFOFIP|EESIPR_RMAFIP)
#define EV_ETH_TX (EESIPR_TCIP|EESIPR_TFUFIP|EESIPR_ITFIP |EESIPR_TROIP)

ER lan_def_int(void);       /* 割込み設定 */

/* グローバル変数 */

UB lan_loopback_test=LOOP_BACK; /* ループバックテスト */
UB lan_inter_nouse=0;           /* 割込み使わない for DEBUG */

/* 内部変数 */

static ETHER_BD  RxBD[ETHER_RX_BUF_NUM+1];  /* 受信BD(アライメントのため +1) */
static ETHER_BD  TxBD[ETHER_TX_BUF_NUM+1];  /* 送信BD(アライメントのため +1) */
static ETHER_BD *pRxBD;                     /* 受信BDカレントアドレス for handling */
static ETHER_BD *pRxBD2;                    /* 受信BDカレントアドレス for API */
static ETHER_BD *pTxBD;                     /* 送信BDカレントアドレス */

                                          /* 16byte境界に合わせるため +16 */
static UB  RxBuf[ETHER_RX_BUF_NUM * ETHER_RX_BUF_SIZ +16];  /* 受信buffer */
static UB  TxBuf[ETHER_TX_BUF_NUM * ETHER_TX_BUF_SIZ +16];  /* 送信buffer */
static UB *pRxBuf;                          /* 受信Bufferカレントアドレス for working */
static UB *pTxBuf;                          /* 送信Bufferカレントアドレス for working */

static ID tx_wtskid;                        /* 送信待ちタスクID */
static ID rx_wtskid;                        /* 受信待ちタスクID */

/* 実行マクロ */

#define DI_RXINT()  /* 受信割り込み禁止 */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_clrl(EDMAC_EESIPR, EV_ETH_RX);\
    vset_psw(psw);\
}


#define EI_RXINT()  /* 受信割り込み許可 */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_setl(EDMAC_EESIPR, EV_ETH_RX);\
    vset_psw(psw);\
}


#define DI_TXINT()  /* 送信割り込み禁止 */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_clrl(EDMAC_EESIPR, EV_ETH_TX);\
    vset_psw(psw);\
}

#define EI_TXINT()  /* 送信割り込み許可 */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_setl(EDMAC_EESIPR, EV_ETH_TX);\
    vset_psw(psw);\
}

/* BD オーバフローで受信停止の場合の再スタート */
#define RX_RESTART() sfr_outl(EDMAC_EDRRR, EDRRR_RR)

/*****************************************************************************
* バッファディスクリプタの初期化
*
******************************************************************************/
static void BD_init(void)
{
    int i;
    ETHER_BD *current;

    /* BD & buffer 領域は、cache through を使う */

    pTxBD  = (ETHER_BD *)(ALIGN(TxBD, sizeof(ETHER_BD)) + 0x20000000L);
    pRxBD  = (ETHER_BD *)(ALIGN(RxBD, sizeof(ETHER_BD)) + 0x20000000L);
    pTxBuf = (UB *)(ALIGN(TxBuf, 16) + 0x20000000L);
    pRxBuf = (UB *)(ALIGN(RxBuf, 16) + 0x20000000L);

    current = pTxBD;
    for (i = 0; i < ETHER_TX_BUF_NUM; i++, current++){
        current->status  = 0;
        current->bufsize = 0;
        current->rxsize  = 0;                    /* reserved area */
        current->buf_p   = pTxBuf;
        pTxBuf += ETHER_TX_BUF_SIZ;
        current->next    = current + 1;  /* LINK to next BD */
    }
    current--;
    current->status |= DESC_DL;  /* LAST descripter */
    current->next    = pTxBD;    /* LINK to 1st BD */

    current = pRxBD;
    for (i = 0; i < ETHER_RX_BUF_NUM; i++, current++){
        current->status  = DESC_ACT;
        current->bufsize = ETHER_RX_BUF_SIZ;
        current->rxsize  = 0;
        current->buf_p   = pRxBuf;
        pRxBuf += ETHER_RX_BUF_SIZ;
        current->next    = current + 1;  /* LINK to next BD */
    }
    current--;
    current->status |= DESC_DL;  /* LAST descripter */
    current->next    = pRxBD;    /* LINK to 1st BD */

    pTxBuf = NULL;
    pRxBD2 = NULL;
    pRxBuf = NULL;
}

/*****************************************************************************
* LAN コントローラ初期化
*
******************************************************************************/

ER lan_ini(UB *macaddr)
{
    ER ercd;
    UW *mach, *macl;

    /* PHYの初期化 */

#if AUTONEGO == 1
 #if FULL == 1
    PHY_init(TRUE, AFD10|AFD100);
            /* AutoNego,10M/Full or 100M/Full */
 #else
    PHY_init(TRUE, AHD10|AHD100);
            /* AutoNego,10M/Half or 100M/Half */
 #endif
#else
  #if FULL == 1
   #if SPEED == 10
    PHY_init(FALSE,FD10);             /* Fixed 10M/Full */
   #else
    PHY_init(FALSE,FD100);            /* Fixed 100M/Full */
   #endif
  #else
   #if SPEED == 10
    PHY_init(FALSE,HD10);             /* Fixed 10M/Half */
   #else
    PHY_init(FALSE,HD100);            /* Fixed 100M/Half */
   #endif
 #endif
#endif

    /* バッファディスクリプタの初期化 */

    BD_init();

    /* E-DMAC 初期化 */

    sfr_outl(EDMAC_EDMR, EDMR_SWR);    /* reset EtherC/E-DMAC */
    dly_tsk(1);                        /* reset復帰まで(min. 16system clock) */
    sfr_outl(EDMAC_TDLAR, (UW)pTxBD);  /* TxBD 先頭番地設定 */
    sfr_outl(EDMAC_RDLAR, (UW)pRxBD);  /* RxBD 先頭番地設定 */
    sfr_outl(EDMAC_EDMR, EDMR_DL16);

    sfr_outl(EDMAC_EESR, (EESR_ECI|EESR_TC|EESR_TFUF|EESR_ITF |EESR_TRO |
                                   EESR_FR|EESR_RDE |EESR_RFOF|EESR_RMAF)); /* clear */
    if (!lan_inter_nouse)
        sfr_outl(EDMAC_EESIPR, EESIPR_ECIP); /* 送受信以外の割り込み設定 */

    sfr_outl(EDMAC_TRSCER, 0);        /* status bit all copy */
    sfr_outl(EDMAC_TFTR, 0);          /* TxFIFO thresholf 0(store&foward) */
#ifdef SH7616
    /* for SH7616 */
    sfr_outl(EDMAC_FDR, FDR_TFD2048|FDR_RFD2048);   /* FIFO depth = 2048bytes */
#else
    /* for SH7615 */
    sfr_outl(EDMAC_FDR, FDR_TFD512|FDR_RFD512);     /* FIFO depth = 512bytes */
#endif
    sfr_outl(EDMAC_RCR, RCR_RNC);     /* multiple Recieve mode */
    sfr_outl(EDMAC_EDOCR, 0);         /* normal setup */

    /* Ether-C 初期化 */

    mach = (UW *)&macaddr[0];
    macl = (UW *)&macaddr[4];

    sfr_outl(ETHERC_RFLR, 1518);    /* RxMax = 1518 */

    sfr_outl(ETHERC_ECSR, ECSR_LCHNG|ECSR_MPR|ECSR_ICD); /* clear */
    if (!lan_inter_nouse)
        sfr_outl(ETHERC_ECSIPR, ECSIPR_LCHNGIP |
                                ECSIPR_MPRIP   |
                                ECSIPR_ICDIP); /* リンク状態系の割り込み設定 */

    sfr_outl(ETHERC_MAHR, *mach);      /* MAC address High */
    sfr_outl(ETHERC_MALR, *macl>>16);  /* MAC address Low */

    /* 割込み定義 */

    if (!lan_inter_nouse)
        lan_def_int();

    /* Tx/Rx enable */
    if (!lan_loopback_test)  /* NORMAL mode */
#if FULL == 0
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE);          /* Tx/Rx enable(DUPX=HALF) */
#else
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE|ECMR_DM);  /* Tx/Rx enable(DUPX=FULL) */
#endif
    else                     /* FORCE LOOP-BACK mode */
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE|ECMR_DM|ECMR_ILB);

    sfr_outl(EDMAC_EDRRR, EDRRR_RR);             /* E-DMAC Rx enable */

    return E_OK;
}

/*****************************************************************************
* 送受信割込みハンドラ
*
******************************************************************************/

void lan_intr(void)
{
    ER ercd;
    UW eesr, ecsr;

    eesr = sfr_inl(EDMAC_EESR);
    sfr_outl(EDMAC_EESR, eesr);  /* clear */

    /* LINK status 変化を検出 */

    if (eesr & EESR_ECI){
        ecsr = sfr_inl(ETHERC_ECSR);
        sfr_outl(ETHERC_ECSR, ecsr);  /* clear */

        if (ecsr & ECSR_LCHNG){
            if (sfr_inl(ETHERC_PSR) & PSR_LMON){  /* LINK確立状態(Hard wire from PHY) */
            }
        }
        if (ecsr & ECSR_MPR){     /* Received Magic Packet */
        }
        if (ecsr & ECSR_ICD){     /* Detected Illegal Carrier */
        }
    }

    /* FR (Rx Complete) */

    if (eesr & (EESR_FR|EESR_RDE|EESR_RFOF|EESR_RMAF)){
        if (rx_wtskid){
            wup_tsk(rx_wtskid);         /* IP受信タスクを起床 */
            rx_wtskid = FALSE;
            sfr_clrl(EDMAC_EESIPR, EV_ETH_RX);
        }
    }

    /* TC (Tx Complete) */

    if (eesr & (EESR_TC|EESR_TFUF|EESR_ITF|EESR_TRO)){
        if (tx_wtskid){
            wup_tsk(tx_wtskid);         /* IP送信タスクを起床 */
            tx_wtskid = FALSE;
            sfr_clrl(EDMAC_EESIPR, EV_ETH_TX);
        }
    }
}

/*****************************************************************************
* 受信割込み待ち
*
******************************************************************************/

ER lan_wai_rcv(TMO tmout)
{
    ER ercd = E_OK;

    for (;;){
        if (!(pRxBD->status & DESC_ACT)){

            /* 1フレーム/1バッファ 固定として実装 */

            if ((!(pRxBD->status & DESC_FE) || (pRxBD->status & 0x29f) == EESR_RMAF)
                                            &&                     /* no Rx error */
                (pRxBD->status & (DESC_FP1|DESC_FP0)) == (DESC_FP1|DESC_FP0)){
                                                           /* 1フレーム/1バッファ */

                pRxBD2 = pRxBD;        /* set current RxBD for working */
                pRxBuf = pRxBD->buf_p; /* set current buffer for working */
                pRxBD  = pRxBD->next;  /* set next BD to current */
                return E_OK;
            }
            pRxBD->rxsize = 0;
            pRxBD->status = (pRxBD->status & DESC_DL)|DESC_ACT;
            pRxBD = pRxBD->next;
            RX_RESTART();
            continue;
        }

        /* wait for Rx by polling */

        if (lan_inter_nouse){
            if (tmout != TMO_FEVR && tmout-- == 0)
                return E_TMOUT;
            ercd = tslp_tsk(1);
            if (ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
        }

        /* wait for Interrupt events */

        else{
            rx_wtskid = vget_tid();
            vcan_wup();
            EI_RXINT();                     /* 受信割込み許可 */
            ercd = tslp_tsk(tmout);         /* 受信割込みからの起床待ち */
            DI_RXINT();                     /* 受信割込み禁止 */
            rx_wtskid = FALSE;

            if (ercd != E_OK && ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
            if (ercd == E_TMOUT)
                break;
        }
    }
    return ercd;
}

/*****************************************************************************
* 送信割込み待ち
*
******************************************************************************/

ER lan_wai_snd(TMO tmout)
{
    ER ercd = E_OK;

    for (;;){
        if (!(pTxBD->status & DESC_ACT)){  /* Tx OK */
            pTxBD->bufsize = 0;
            pTxBuf = pTxBD->buf_p;
            return E_OK;
        }

        /* wait for Tx READY by polling */

        if (lan_inter_nouse){
            if (tmout != TMO_FEVR && tmout-- == 0)
                return E_TMOUT;
            ercd = tslp_tsk(1);
            if (ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
        }

        /* wait for Interrupt events */

        else{
            tx_wtskid = vget_tid();
            vcan_wup();
            EI_TXINT();                     /* 送信割込み許可 */
            ercd = tslp_tsk(tmout);         /* 送信割込みからの起床待ち */
            DI_TXINT();                     /* 送信割込み禁止 */
            tx_wtskid = FALSE;

            if (ercd != E_OK && ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
            if (ercd == E_TMOUT)
                break;
        }
    }
    return ercd;
}

/*****************************************************************************
* 受信パケット長を得る
*
******************************************************************************/

ER lan_get_len(UH *len)
{
    if (!pRxBD2)
        *len = 0;
    else
        *len = pRxBD2->rxsize;
    return E_OK;
}

/*****************************************************************************
* 受信パケット読み出し
*
* buf   読み出すためのバッファ
* len   読み出すバイト数
******************************************************************************/

ER lan_get_pkt(void *buf, int len)
{
    if (!pRxBuf)  /* no available RxBD */
        return -1;

    memcpy(buf, pRxBuf, len);
    pRxBuf += len;
    return E_OK;
}

/*****************************************************************************
* 受信パケット読み出し終了
*
******************************************************************************/

ER lan_get_end(void)
{
    if (pRxBD2){
        pRxBD2->rxsize = 0;
        pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
        RX_RESTART();
    }
    pRxBD2 = NULL;
    pRxBuf = NULL;
    return E_OK;
}

/*****************************************************************************
* 受信パケット読み飛ばし
*
* len   受信パケットの未読バイト数
******************************************************************************/

ER lan_skp_pkt(int len)
{
    int remain;

    if (!pRxBD2)  /* no available RxBD */
        return E_OK;

    remain = (int)((UW)pRxBuf - (UW)pRxBD2->buf_p);  /* 読み出した数 */
    remain = (int)((UINT)pRxBD2->rxsize) - remain;   /* 残り数 */
    if (remain > 0){
        if (remain <= len){
            pRxBD2->rxsize = 0;
            pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
            RX_RESTART();
            pRxBD2 = NULL;
            pRxBuf = NULL;
        }
        else{
            pRxBuf += len;
        }
    }
    else{
        pRxBD2->rxsize = 0;
        pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
        RX_RESTART();
        pRxBD2 = NULL;
        pRxBuf = NULL;
    }
    return E_OK;
}

/*****************************************************************************
* 送信パケット長を設定
*
******************************************************************************/

ER lan_set_len(int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    pTxBD->bufsize = len;
    return E_OK;
}

/*****************************************************************************
* 送信パケット書き込み
*
* data      書き込むデータ
* len       データのバイト数（> 0）
******************************************************************************/

ER lan_put_pkt(const void *data, int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    memcpy(pTxBuf, data, len);
    pTxBuf += len;
    return E_OK;
}

/*****************************************************************************
* 送信パケットが60バイト未満の場合のダミー書き込み
*
* len       ダミーデータのバイト数（> 0）
******************************************************************************/

ER lan_put_dmy(int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    memset(pTxBuf, 0, len);
    pTxBuf += len;
    return E_OK;
}

/*****************************************************************************
* 送信パケット書き込み終了
*
******************************************************************************/

ER lan_put_end(void)
{
    pTxBD->status = ((pTxBD->status & DESC_DL)|DESC_FP1|DESC_FP0|DESC_ACT);
                                       /* changed active of Fin. BD */
    pTxBD = pTxBD->next;               /* set next BD to current */
    sfr_outl(EDMAC_EDTRR, EDTRR_TR);   /* be transferrd */
    return E_OK;
}

/* end */
