/******************************************************************************
* NORTi Network LAN Controller Driver (Fujitsu MB86964 for MiNET-704X)        *
*                                                                             *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Feb/1999 1.00                                                         MI *
* 24/Apr/1999 1.01 Waking up failure bug is fixed.                         MI *
* 19/Sep/2000 1.02 lan_intr関数にDLCR3とDLCR2のチェックを追加              MI *
* 19/Sep/2000 1.03 lan_get_end関数の8バイト境界処理を削除                  MI *
* 04/Oct/2000 1.04 16回コリジョンレジスタの設定値 04H → 06H               MI *
* 07/Dec/2002 1.05 lan_get_len見直し                                       MI *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "mb86964.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

/* ユーザー定義の関数/マクロ */

#ifndef lan_in
UB lan_in(unsigned);            /* バイトデータの入力 */
void lan_out(unsigned, UB);     /* バイトデータの出力 */
UH lan_inw(unsigned);           /* ワードデータの入力 */
void lan_outw(unsigned, UH);    /* ワードデータの出力 */
#endif
ER lan_def_int(void);           /* 割込み設定 */

/* グローバル変数 */

UB lan_loopback_test;           /* ループバックテスト */
UB lan_inter_nouse;             /* 割込み使わない */

/* 内部変数 */

static ID tx_wtskid;            /* 送信待ちタスクID */
static ID rx_wtskid;            /* 受信待ちタスクID */
static UB align_byte;

/* デバッグ用 */

#ifndef PKTCHK     /* 受信パケットステータスをチェックする(1)/しない(0) */
#define PKTCHK  0  /* ACPT BAD PKTビット＝0なので異常パケットは受信されるは */
#endif             /* ずないが、受信パケットステータスをチェックする場合'1' */

#if PKTCHK
int lan_error_count1;           /* 受信データ長異常回数 */
int lan_error_count2;           /* 受信パケットステータス異常回数 */
int lan_error_count3;           /* 受信オーバーフロー警告回数 */
int lan_error_count4;           /* GOOD PKTでCRCエラーが立つかの試験用 */
#endif

/*****************************************************************************
* 上位下位バイト入れ替え
*
******************************************************************************/

static UH swap(UH c)
{
    return (UH)((c >> 8) | (c << 8));
}

/*****************************************************************************
* LAN コントローラ初期化
*
******************************************************************************/

ER lan_ini(UB *macaddr)
{
    UB c;
    int i;

    /* コントロール１レジスタ */

    c =  DLCR6_NENA_DLC                 /* イニシャライズ */
        |DLCR6_100NS
     /* |DLCR6_SB */                    /* システムデータバス8ビット */
        |DLCR6_BB                       /* バッファメモリデータバス8ビット */
     /* |DLCR6_TX_BUF_SIZE1 */          /* 送信バッファサイズ 2kバイト */
     /* |DLCR6_TX_BUF_SIZE0 */
        |DLCR6_BUF_SIZE1                /* バッファメモリサイズ 32kバイト */
     /* |DLCR6_BUF_SIZE0 */
        ;
    lan_out(DLCR6, c);

    /* コントロール２レジスタ */

    c = 0x00
     /* |DLCR7_IDENT1 */                /* MB86964 では 01 固定(ﾘｰﾄﾞｵﾝﾘ) */
     /* |DLCR7_IDENT0 */
        |DLCR7_NSTBY                    /* スタンバイ状態でない */
        |DLCR7_RBS1                     /* レジスタバンク2（BMPR8～15）*/
     /* |DLCR7_RBS0 */
     /* |DLCR7_EOP */                   /* EOP 入力アクティブ Low */
     /* |DLCR7_BYTE_SWAP */             /* バイトスワップ */
        ;
    lan_out(DLCR7, c);

    /* 送受信割込みイネーブルレジスタ */

    lan_out(DLCR2, 0x00);               /* 送信割込み禁止 */
    lan_out(DLCR3, 0x00);               /* 受信割込み禁止 */

    /* 送受信ステータスレジスタ */

    lan_out(DLCR0, 0xff);               /* 送信ステータスクリア */
    lan_out(DLCR1, 0xff);               /* 受信ステータスクリア */

    /* 送信モードレジスタ */

    if (lan_loopback_test)
        lan_out(DLCR4, 0);              /* 強制ループバックモード */
    else
        lan_out(DLCR4, DLCR4_NLBC);     /* 通常送信モード */

    /* 受信モードレジスタ */

    if (macaddr[0] == 0 && macaddr[1] == 0
     && macaddr[2] == 0 && macaddr[3] == 0
     && macaddr[4] == 0 && macaddr[5] == 0)
        c = DLCR5_AM1|DLCR5_AM0;        /* アドレスマッチモード 11 */
    else
        c = DLCR5_AM0;                  /* アドレスマッチモード 10 */
    lan_out(DLCR5, c);

    /* レジスタバンク0 に切替 */

    c = (UB)lan_in(DLCR7);
    c &= ~(DLCR7_RBS1|DLCR7_RBS0);
    lan_out(DLCR7, c);

    /* ノード ID を設定 */

    for (i = 0; i < 6; i++)
    {   c = macaddr[i];
        lan_out(DLCR8 + i, c);
    }

    /* レジスタバンク2 に切替 */

    c = (UB)lan_in(DLCR7);
    c &= ~(DLCR7_RBS1|DLCR7_RBS0);
    c |= DLCR7_RBS1;
    lan_out(DLCR7, c);

    /* 16回コリジョンコントロールレジスタ */

    c = 0x00
        |BMPR11_16COL_CNT2              /* 自動的に再送信（Auto Mode）*/
        |BMPR11_16COL_CNT1
     /* |BMPR11_16COL_CNT0 */
        ;
    lan_out(BMPR11, c);

    /* その他 */

    lan_out(BMPR12, 0x00);
    lan_out(BMPR14, 0x00);
    lan_out(BMPR15, 0xfa);

    /* 動作許可 */

    c = (UB)lan_in(DLCR6);
    c &= ~DLCR6_NENA_DLC;
    lan_out(DLCR6, c);

    /* 割込み定義 */

    if (!lan_inter_nouse)
        lan_def_int();
    return E_OK;
}

/*****************************************************************************
* 送受信割込みハンドラ
*
******************************************************************************/

void lan_intr(void)
{
    UB c;

    /* 受信割込みか ? */

    c = lan_in(DLCR1) & lan_in(DLCR3);
    if (c & PKT_RDY)
    {   if (rx_wtskid)
        {   wup_tsk(rx_wtskid);         /* IP受信タスクを起床 */
            rx_wtskid = FALSE;
            lan_out(DLCR3, 0x00);       /* 受信割込み禁止 */
        }
    }

    /* TMT OK か ? */

    c = lan_in(DLCR0) & lan_in(DLCR2);
    if (c & DLCR0_TMT_OK)
    {   if (tx_wtskid)
        {   wup_tsk(tx_wtskid);         /* IP送信タスクを起床 */
            tx_wtskid = FALSE;
            lan_out(DLCR2, 0x00);       /* 送信割込み禁止 */
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
    UB c;
    UINT psw;

    if (lan_inter_nouse)
    {   for (;;)
        {   for (;;)
            {   c = lan_in(DLCR1);
                if (c & (PKT_RDY|OVRFLO))
                    break;
                if (tmout != TMO_FEVR && tmout-- == 0)
                    return E_TMOUT;
                tslp_tsk(1);
            }
            c = lan_in(DLCR5);
            if (!(c & DLCR5_BUF_EMP))
            {   lan_out(DLCR1, PKT_RDY);    /* PKT RDY ビットクリア */
                return E_OK;
            }
            lan_out(DLCR1, 0xff);           /* 全ステータスビットクリア */
        }
    }

    psw = vdis_psw();

    for (;;)
    {   c = lan_in(DLCR5);
        if (!(c & DLCR5_BUF_EMP))       /* BUF EMP = 0 となるまで */
            break;
        rx_wtskid = vget_tid();
        vcan_wup();
        lan_out(DLCR3, 0x80);           /* 受信割込み許可 */
        vset_psw(psw);
        ercd = tslp_tsk(tmout);         /* 受信割込みからの起床待ち */
        psw = vdis_psw();
        rx_wtskid = FALSE;
        lan_out(DLCR3, 0x00);           /* 受信割込み禁止 */
        lan_out(DLCR1, 0xff);           /* 全ステータスビットクリア */
        if (ercd != E_OK)               /* タイムアウトなら */
            break;                      /* エラー終了 */
    }
    vset_psw(psw);
    return ercd;
}

/*****************************************************************************
* 送信割込み待ち
*
******************************************************************************/

ER lan_wai_snd(TMO tmout)
{
    ER ercd = E_OK;
    UB c;
    UINT psw;
    static UB i;

    if (i == 0)         /* 最初だけ TMT OK 見ない */
    {   i = 1;
        return E_OK;
    }

    if (lan_inter_nouse)
    {   c = lan_in(DLCR0);
        if (c & DLCR0_TMT_OK)
        {   lan_out(DLCR0, DLCR0_TMT_OK);   /* TMT OK = 0 */
            return E_OK;
        }
        if (tmout != TMO_FEVR && tmout-- == 0)
            return E_TMOUT;
        tslp_tsk(1);
    }

    psw = vdis_psw();

    for (;;)
    {   c = lan_in(DLCR0);
        if (c & DLCR0_TMT_OK)
        {   lan_out(DLCR0, DLCR0_TMT_OK);   /* TMT OK = 0 */
            break;
        }
        tx_wtskid = vget_tid();
        vcan_wup();
        lan_out(DLCR2, 0x80);           /* 送信割込み許可 */
        vset_psw(psw);
        ercd = tslp_tsk(tmout);         /* 送信割込みからの起床待ち */
        psw = vdis_psw();
        tx_wtskid = FALSE;
        lan_out(DLCR2, 0x00);           /* 送信割込み禁止 */
        if (ercd != E_OK)               /* タイムアウトなら */
            break;                      /* エラー終了 */
    }
    vset_psw(psw);
    return ercd;
}

/*****************************************************************************
* 受信パケット長を得る
*
******************************************************************************/

ER lan_get_len(UH *len)
{
    UH c, sts;
    int n, i;

    /* 受信パケットステータス読み出し */

    c = lan_inw(BMPR8);
    sts = (UH)(swap(c) & 0x00ff);

    /* 受信データ長を読み出し */

    c = lan_inw(BMPR8);
    n = swap(c);
    *len = n;
    align_byte = (UB)((8 - ((n + 4) & 7)) & 7);

    /* 受信パケットステータスとデータ長をチェックする場合 */

 #if PKTCHK
    if ((n & 0xf800) != 0)              /* データ長上位5ビットは常に0 */
    {   lan_error_count1++;
        goto NG;
    }
    if (!(sts & GOOD_PKT))
    {   lan_error_count2++;
        goto NG;
    }
    if (sts & OVRFLO)                   /* OVRFLOは警告, このPKTは正常 */
        lan_error_count3++;
    if (sts & ~(GOOD_PKT|OVRFLO))       /* CRC ERRが立つことがあるが正常 */
        lan_error_count4++;
    goto OK;

    /* 正常パケットでない場合、読み飛ばし。 */

  NG:
    c = lan_inw(BMPR8);
    c = lan_inw(BMPR8);                 /* 少なくとも4回リードして SKIP */
    n -= 4;
    if (n <= 8)                         /* 残り8バイト以下での SKIP 禁止 */
    {   n += align_byte;                /* 8バイト境界まで空読みの必要あり */
        for (i = 0; i < n; i += 2)
            c = lan_inw(BMPR8);
    }
    else
        lan_out(BMPR14, BMPR14_SKIP_RX_PKT);
    return -1;
  OK:
 #endif
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
    UH *p;
    int i;
    UH c;

    p = (UH *)buf;
    for (i = 0; i < len; i += 2)
    {   c = lan_inw(BMPR8);
        *p++ = c;
    }
    return E_OK;
}

/*****************************************************************************
* 受信パケット読み出し終了
*
******************************************************************************/

ER lan_get_end()
{
    /* データが8ビットアクセスの場合、奇数長に対する1バイトのダミーリードを
       行ってください。*/

    return E_OK;
}

/*****************************************************************************
* 受信パケット読み飛ばし
*
* len   受信パケットの未読バイト数
******************************************************************************/

ER lan_skp_pkt(int len)
{
    int i;
    UH c;

    if (len <= 8)                       /* 残り8バイト以下での SKIP 禁止 */
    {   len += align_byte;              /* 8バイト境界まで空読みの必要あり */
        for (i = 0; i < len; i += 2)
            c = lan_inw(BMPR8);
    }
    else
        lan_out(BMPR14, BMPR14_SKIP_RX_PKT);
    return E_OK;
}

/*****************************************************************************
* 送信パケット長を設定
*
******************************************************************************/

ER lan_set_len(int len)
{
    lan_outw(BMPR8, swap(len));
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
    const UH *p;
    UH c;
    int i;

    p = (const UH *)data;
    for (i = 0; i < len; i += 2)
    {   c = *p++;
        lan_outw(BMPR8, c);
    }
    return E_OK;
}

/*****************************************************************************
* 送信パケットが60バイト未満の場合のダミー書き込み
*
* len       ダミーデータのバイト数（> 0）
******************************************************************************/

ER lan_put_dmy(int len)
{
    int i;

    for (i = 0; i < len; i += 2)
        lan_outw(BMPR8, 0);
    return E_OK;
}

/*****************************************************************************
* 送信パケット書き込み終了
*
******************************************************************************/

ER lan_put_end(void)
{
    lan_out(BMPR10, 0x81);                  /* TMST = 1, TX PKT CNT = 1 */
    return E_OK;
}

/* end */
