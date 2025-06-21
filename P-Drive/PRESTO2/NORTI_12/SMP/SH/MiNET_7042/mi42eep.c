/******************************************************************************
* EEPROM 入出力                                                               *
*                                                                             *
* 24/Mar/1998 作成                                                   MiSPO MI *
* 30/Nov/1998 MiNET-7042 用に修正                                          MI *
* 26/May/1998 ini_eeprom を追加                                            MI *
* 07/Mar/2001 内蔵Flash高速動作用にwai_5usを修正,追加                      MI *
* 24/Dec/2002 SH7042/44/45で共通のためsh7042.h->sh7040.hにする             KA *
******************************************************************************/

#include "norti3.h"
#include "sh7040.h"
#include "net7042.h"

/* EEPROM I/O のビット */

#define EEP_SDA_BIT     0x0010
#define EEP_SCL_BIT     0x0020

/* 入出力マクロ */

#define SDA_IN()        ((sfr_inw(IO_PEDR) & EEP_SDA_BIT) != 0)
#define SCL_HIGH()      sfr_setw(IO_PEDR, EEP_SCL_BIT)
#define SCL_LOW()       sfr_clrw(IO_PEDR, EEP_SCL_BIT)
#define SDA_HIGH()      sfr_setw(IO_PEDR, EEP_SDA_BIT)
#define SDA_LOW()       sfr_clrw(IO_PEDR, EEP_SDA_BIT)

#define SDA_DIR_IN()    sfr_clrw(PFC_PEIOR,EEP_SDA_BIT)
#define SDA_DIR_OUT()   sfr_setw(PFC_PEIOR,EEP_SDA_BIT)
#define SCL_DIR_OUT()   sfr_setw(PFC_PEIOR,EEP_SCL_BIT)

/*****************************************************************************/
/* 5μsec以上の待ち                                                          */
/*                                                                           */
/* EEPROM クロックタイミングをとるため、5μsec 以上の時間を潰す。            */
/* CPU 速度に依存するので、ダミーの実行命令数は調整が必要。                  */
/*****************************************************************************/

static int wai_5us(void)
{
    int i;
    UH r;

    for (i = 0; i < 20; i++)
        r = *(volatile UH *)0xce0000;   /* 非キャッシュの空間をアクセス */
    return r;
}

/*****************************************************************************/
/* 10msec以上の待ち                                                          */
/*                                                                           */
/* EEPROM 書き込みタイミングをとるため、10msec 以上の時間を潰す。            */
/*****************************************************************************/

#define wait_10ms() dly_tsk(10/MSEC+1)

/*****************************************************************************/
/* シリアルＥＥＰＲＯＭポート初期化                                          */
/*                                                                           */
/* 初期状態は SCL=1 出力方向, SDA 入力方向とする。                           */
/* 常に成功で TRUE を返す。                                                  */
/*****************************************************************************/

BOOL ini_eeprom(void)
{
    SCL_HIGH();
    SCL_DIR_OUT();
    SDA_DIR_IN();
    return TRUE;
}

/*****************************************************************************/
/* シリアルＥＥＰＲＯＭ１バイト書き込み                                      */
/*                                                                           */
/* EEPROM に１バイトのデータを書き込む。                                     */
/* 書き込み成功で TRUE、失敗で FALSE を返す。                                */
/*****************************************************************************/

BOOL write_eeprom(UH addr,  /* EEPROM のアドレス */
                  UB data)  /* 書き込むデータ */
{
    UB c;
    int t;

    /* START */ /* 初期状態は SCL=1 出力方向, SDA 入力方向とする */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                           /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 */

    SDA_DIR_OUT();                          /* SDA 出力方向 */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                           /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA WRITE */

    SDA_DIR_OUT();                          /* SDA 出力方向 */
    c = data;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* STOP */

    SDA_LOW();
    SDA_DIR_OUT();                          /* SDA 出力方向 */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();

    wait_10ms();                            /* tWR 待ち     */
    SDA_DIR_IN();                           /* SDA 入力方向 */
    return TRUE;
}

/*****************************************************************************/
/* シリアルＥＥＰＲＯＭ１バイト読み出し                                      */
/*                                                                           */
/* EEPROM から１バイトのデータを読み出す。                                   */
/* 読み出し成功で TRUE、失敗で FALSE を返す。                                */
/*****************************************************************************/

BOOL read_eeprom(UH addr,       /* EEPROM のアドレス */
                 UB *data)      /* 読み出したデータ */
{
    UB c;
    int t;

    /* START (dummy write ) */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 (dummy write) */

    SDA_DIR_OUT();                      /* SDA 出力方向 */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* START */

    wai_5us();
    SCL_HIGH();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SDA_LOW();
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 1);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA READ */

    c = 0;
    wai_5us(); if (SDA_IN()) c |= 0x80; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x40; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x20; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x10; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x08; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x04; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x02; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x01; SCL_HIGH(); wai_5us(); SCL_LOW();
    *data = c;

    /* STOP */

    SCL_HIGH();
    wai_5us();
    SCL_LOW();

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    return TRUE;
}

/*****************************************************************************/
/* シリアルＥＥＰＲＯＭページ書き込み                                        */
/*                                                                           */
/* EEPROM に連続１６バイト単位でデータを書き込む。                           */
/* １６バイト未満を指定した場合は、残りは０を書き込む。                      */
/* 書き込み成功で TRUE、失敗で FALSE を返す。                                */
/*****************************************************************************/

BOOL page_write_eeprom(UH addr,  /* EEPROM のアドレス */
                       UB *buf,  /* 書き込むデータへのポインタ */
                       INT size)/* 書き込むデータのバイト数 */
{
    INT i, t, n;
    UB c;

    while (size > 0) {
        if (size >= 16)
            n = 16;
        else
            n = size;
        size -= n;

        /* START */ /* 初期状態は SCL=1 出力方向, SDA 入力方向とする */

        SDA_LOW();
        SDA_DIR_OUT();                      /* SDA 出力方向 */
        wai_5us();
        SCL_LOW();

        /* Device Type ID | Address A10-A8 | R/W */

        c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();                           /* SDA 入力方向 */

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();

        /* Address , A7-A0 */

        SDA_DIR_OUT();                          /* SDA 出力方向 */
        c = (UB)(addr & 0xff) ;
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();                           /* SDA 入力方向 */

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();

        /* DATA WRITE */

        i = 0;
L1:
        SDA_DIR_OUT();                          /* SDA 出力方向 */
        if (i < n)
            c = *buf++;
        else
            c = 0x00;
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();
        if (++i < 16)
            goto L1;

        /* STOP */

        SDA_LOW();
        SDA_DIR_OUT();                          /* SDA 出力方向 */
        wai_5us();
        SCL_HIGH();
        wai_5us();
        SDA_HIGH();

        wait_10ms();                            /* tWR 待ち     */
        SDA_DIR_IN();                           /* SDA 入力方向 */

        addr += 16;
    }
    return TRUE;
}

/*****************************************************************************/
/* シリアルＥＥＰＲＯＭ連続バイト読み出し                                    */
/*                                                                           */
/* EEPROM から複数バイトのデータを読み出す。                                 */
/* 読み出し成功で TRUE、失敗で FALSE を返す。                                */
/*****************************************************************************/

BOOL seq_read_eeprom(UH addr,  /* EEPROM のアドレス */
                     UB *buf,  /* Readデータへのポインタ */
                     INT size) /* Readデータのバイト数 */
{
    INT i, t;
    UB c;

    /* START (dummy write) */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 (dummy write) */

    SDA_DIR_OUT();                      /* SDA 出力方向 */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* START */

    wai_5us();
    SCL_HIGH();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SDA_LOW();
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 1);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA READ */
    i = 0;
L1:
    c = 0;
    wai_5us(); if (SDA_IN()) c |= 0x80; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x40; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x20; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x10; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x08; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x04; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x02; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x01; SCL_HIGH(); wai_5us(); SCL_LOW();
    *buf++ = c;
    if (++i >= size)
        goto STOP;

    /* SEND ACK */
    wai_5us();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    SDA_LOW();
    wai_5us();
    SCL_HIGH();
    wai_5us();
    wai_5us();
    SCL_LOW();
    SDA_DIR_IN();
    SDA_HIGH();
    goto L1;

    /* STOP */
STOP:
    SCL_HIGH();
    wai_5us();
    SCL_LOW();

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA 出力方向 */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA 入力方向 */

    return TRUE;
}

/* end */
