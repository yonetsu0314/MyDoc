/******************************************************************************
* NORTi Network LAN Driver Caller                                             *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 28/Feb/1999                                                                 *
* 13/Jun/2001 エラー表示を追加                                             MI *
* 28/Nov/2002 SNMP対応部分を反映                                           MI *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"

/* パケット表示ルーチン */

#ifdef DUMP
void landump_len(VP *b, int len);
void landump_dat(VP b, const void *data, int len);
void landump_end(VP b);
void landump_msg(char *s);
#else
#define landump_len(b,l)
#define landump_dat(b,d,l)
#define landump_end(b)
#define landump_msg(s)
#endif

/* 内部変数 */

static UH received_len;
VP rcv_dump;
VP snd_dump;

/***********************************************************************
* LAN ドライバのエラー処理
*
* LAN ドライバ関数がエラーを返したときに、この関数が呼ばれます。
* 標準では何も処理していません。
************************************************************************/

ER lan_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

ER lan_errors(char *s, ER ercd)
{
    landump_msg(s);
    return tcpip_internal_error(ercd);
}

/***********************************************************************
* 受信パケット長を得る
*
* 受信した Ethernet フレームのバイト数を戻値として返します。
* LAN ドライバの lan_get_len 関数を呼び出しています。
************************************************************************/

UH lan_received_len(void)
{
    ER ercd;

    ercd = lan_get_len(&received_len);
    if (ercd != E_OK)
    {   lan_errors((char*)"lan_get_len error\r\n", ercd);
        received_len = 0;
    }

    landump_len(&rcv_dump, received_len);
    SNMP_BYPCNT(snmp_mibbyp_if[0].ifInOctets, received_len);   /* ifInOctets */
    return received_len;
}

/***********************************************************************
* 受信パケット読み出し
*
* 受信した Ethernet フレームを指定バイト数だけ読み出します。
* LAN ドライバの lan_get_pkt 関数を呼び出しています。
************************************************************************/

BOOL lan_read_pkt(void *buf, int len)
{
    ER ercd;

    if (received_len == 0)
        return FALSE;

    if (len <= (int)received_len)
    {   ercd = lan_get_pkt(buf, len);
        if (ercd != E_OK)
        {   lan_errors((char*)"lan_get_pkt error", ercd);
            received_len = 0;
            landump_end(rcv_dump);
            return FALSE;
        }
        landump_dat(rcv_dump, buf, len);
        if ((received_len -= len) == 0)
        {   lan_get_end();
            landump_end(rcv_dump);
        }
        return TRUE;
    }
    else
    {   ercd = lan_get_pkt(buf, received_len);
        if (ercd != E_OK)
            lan_errors((char*)"lan_get_pkt error", ercd);
        landump_dat(rcv_dump, buf, received_len);
        received_len = 0;
        lan_get_end();
        landump_end(rcv_dump);
        return FALSE;
    }
}

/***********************************************************************
* 受信パケット破棄
*
************************************************************************/

void lan_ignore_pkt(void)
{
    ER ercd;

    if (received_len != 0)
    {   ercd = lan_skp_pkt(received_len);
        if (ercd != E_OK)
            lan_errors((char*)"lan_skp_pkt error", ercd);
        received_len = 0;
    }
    landump_end(rcv_dump);
}

/***********************************************************************
* 受信パケット末尾まで読み出し
*
************************************************************************/

BOOL lan_read_pkt_end(void *buf, int len, int bufsz)
{
    /* len 未満なら異常 */

    if ((int)received_len < len)
    {   SNMP_BYPCNT(snmp_mibbyp_if[0].ifInErrors, 1);   /* ifInErrors */
        lan_read_pkt(buf, received_len);
        return FALSE;
    }

    /* bufsz 超過なら、溢れる分を破棄 */

    if ((int)received_len > bufsz)
    {   if (!lan_read_pkt(buf, bufsz))
            return FALSE;
        lan_ignore_pkt();
        return TRUE;
    }

    /* 正常な読み出し */

    return lan_read_pkt(buf, received_len);
}

/***********************************************************************
* 送信パケット書き込み
*
* head      書き込むヘッダ
* hlen      ヘッダのバイト数
* data      書き込むデータ
* dlen      データのバイト数
************************************************************************/

BOOL lan_write_pkt(const void *head, int hlen, const void *data, int dlen)
{
    int i, len;

    /* 送信長（ヘッダ長＋データ長）を設定 */

    len = hlen + dlen;
    i = 60 - len;
    if (i > 0)
        len = 60;
    lan_set_len(len);
    landump_len(&snd_dump, len);
    SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutOctets, len);   /* ifOutOctets */

    /* ヘッダを送信バッファ書き込み */

    if (hlen > 0)
    {   lan_put_pkt(head, hlen);
        landump_dat(snd_dump, head, hlen);
    }

    /* データを送信バッファ書き込み */

    if (dlen > 0)
    {   lan_put_pkt(data, dlen);
        landump_dat(snd_dump, data, dlen);
    }

    /* 60バイト未満の場合のダミー書き込み */

    if (i > 0)
    {   lan_put_dmy(i);
        landump_dat(snd_dump, NULL, i);
    }

    /* 送信 */

    lan_put_end();
    landump_end(snd_dump);

    return TRUE;
}

/* end */
