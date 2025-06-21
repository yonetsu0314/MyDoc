/****************************************************************************
* シルアル入出力ドライバ（SH7040シリーズ）                                  *
*                                                                           *
*  File name : nos7040.c                                                    *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=? nos7040.c        (Renesas C)      *
*            gcc -c -m2 -O2 -DCLK=? nos7040.c              (exeGCC)         *
*            ccsh -c -cpu=sh_ii -ansi -O -DCLK=? nos7040.c (GHS)            *
*                                                                           *
* 27/Feb/1998 新規作成(nos7040.c)                                        MI *
* 17/Apr/2000 NORTi Ver.4 兼用に改造(nos7040.c)                          FJ *
* 30/Oct/2000 IAR EW 対応                                                TK *
* 23/Dec/2002 XASS-Vの判別を追加, マクロに関するコメント追加             MI *
* 23/Dec/2002 SH7042/44/45で共通のためnos7040.cに戻す                    MI *
* 31/Jul/2003 BRR設定後 ウェイト挿入                                     KA *
****************************************************************************/

/*

最適化コンパイル

          割込みハンドラが含まれるので、必ず最適化オプションを付けてコンパイ
          ルしてください。

論理チャネル番号の指定

          CHマクロで、論理チャネル番号を指定できます。(無指定時は、CH=0)
          SH7040シリーズの内蔵シリアルは2チャネルですが、外付けUARTの存在も
          考慮して、CHには、0～3を指定できるようにしています。

          チャネル0とチャネル1の両方をリンクする場合は、重複しないモジュール
          名となるようコンパイルしてください。本ソースファイルを別名でコピー
          するのが簡単です。

          (例) shc <options> nos7040.c ……………………………… チャネル0
               copy nos7040.c nos7ch1.c
               shc <options> -def=CH=1 nos7ch1.c ………………… チャネル1

          (例) gcc <options> nos7040.c ……………………………… チャネル0
               copy nos7040.c nos7ch1.c
               gcc <options> -DCH=1 nos7ch1.c  …………………… チャネル1

物理チャネル番号の指定

          CH0, CH1マクロで、物理チャネル番号を変更できます。
          CH0を定義すると、SCI0を使うようにコンパイルされます。
          CH1を定義すると、SCI1を使うようにコンパイルされます。
          本マクロを定義しない場合、CH=0/CH=2ではCH0として、CH=1/CH=3では
          CH1としてコンパイルされるので、通常は、本マクロは定義不要ですが、
          SCI1のみのハードウェアで、SCI1をCH=0に割り付ける場合などは、本マ
          クロを利用してください。

          (例) shc <options> -def=CH=0,CH1 nos7040.c ……… SCI1をチャネル0
                                       ~~~
          (例) gcc <options> -DCH=0 -DCH1 nos7040.c
                                      ~~~
ボーレートジェネレータ

          ソースクロックのデフォルトは24MHz です。変更する場合はCLKマクロを
          定義してコンパイルしてください。値は[Hz]単位の周波数です。

          (例) shc <options> -def=CLK=24576000 nos7040.c ……… 24.576 MHz
                                  ~~~~~~~~~~~~
          (例) gcc <options> -DCLK=24576000 nos7040.c
                               ~~~~~~~~~~~~
送受信バッファサイズの指定

          受信/送信バッファのサイズを、BUFSZマクロにより定義してコンパイルし
          てください。受信バッファと異なるサイズの送信バッファを用いる場合は、
          さらに TXBUFSZマクロを定義してください。
          未指定時は、受信/送信バッファ共1024バイトとなります。

          (例) 受信バッファ, 送信バッファ共128バイト BUFSZ=128
          (例) 受信バッファ128, 送信バッファ64バイト BUFSZ=128, TXBUFSZ=64

          キャラクタ毎の受信ステータスもバッファリングしているので、実際の
          受信バッファのサイズは上記の倍となります。

ソフトウェアフロー制御

          XON/XOFFキャラクタによるフロー制御を用いない場合、NOXONマクロを定義
          してコンパイルしてください。コードサイズと処理速度を節約できます。

ハードウェアフロー制御

          RTS信号をON/OFFする次の関数を、外部に定義してください。

          void rts0_on(void); ……･ 物理チャネル0 RTS信号 ON
          void rts0_off(void); …… 物理チャネル0 RTS信号 OFF
          void rts1_on(void); ……･ 物理チャネル1 RTS信号 ON
          void rts1_off(void); …… 物理チャネル1 RTS信号 OFF

          RTS信号出力が無い場合、rts?_on/rts?_off 関数は何もせずリターンして
          ください。
          RTS信号ポートのビット操作が他タスクと競合する場合を考慮して、本ドラ
          イバからは、上記関数を割り込み禁止(レベル15)で呼び出しています。競
          合する他タスクからも、割り込み禁止状態で同ポートを操作してください。

          RTS/CTS信号によるフロー制御を行う場合、CTS ONによるレベルセンス割込
          みが必要です。
          チャネル0 /CTS 信号を /IRQ0(PA2)端子に接続してください。
          チャネル1 /CTS 信号を /IRQ1(PA5)端子に接続してください。
          他の/IRQnを用いる場合は、本ソースファイルの変更が必要です。

          ハードウェアフローを用いない場合は、NOFLWマクロ定義してコンパイルし
          てください。コードサイズと処理速度を節約できます。
          また、rts?_on/rts?_off関数の定義も不要となります。

割込み優先順位

          本ドライバの、デフォルトの割込み優先順位は、7です。
          これを変更する場合、IPマクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。(IP ≦ KNL_LEVEL)

          (例) shc <options> -def=IP=5 nos7040.c …･ 割込み優先順位 5
                                  ~~~~
          (例) gcc <options> -DIP=5 nos7040.c
                               ~~~~
ポートの初期化

          SCIと兼用になっているポートA は、外部で、次の例の様に初期化してくだ
          さい。(下記の例では、PAの他の端子は入力と仮定)

          #include "nos7040.h"
          sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0); …… TXD1,TXD0端子"1"
          sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0); … TXD1,TXD0端子出力方向
          sfr_setw(PFC_PACRL2, 0x0145); ……………･ TXD1,RXD1,TXD0,RXD0 機能

          RTS/CTS信号によるフロー制御を行う場合は、次の例の様に初期化してく
          ださい。

          #define PA_RTS0  0x40  ……………………･  PA6 チャネル0 RTS信号(例)
          #define PA_RTS1  0x80  ……………………･  PA7 チャネル1 RTS信号(例)
          sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0|PA_RTS1|PA_RTS0);
          sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0|PA_RTS1|PA_RTS0);
          sfr_setw(PFC_PACRL2, 0x0d75); …………… IRQ1(CTS1),IRQ0(CTS0)機能

          この場合の rts?_on/rts?_off関数例は次の通りです。

          void rts0_on(void)  { sfr_clrw(IO_PADRL, PA_RTS0); }
          void rts0_off(void) { sfr_setw(IO_PADRL, PA_RTS0); }
          void rts1_on(void)  { sfr_clrw(IO_PADRL, PA_RTS1); }
          void rts1_off(void) { sfr_setw(IO_PADRL, PA_RTS1); }

未サポート機能

          調歩同期式モードのみで、クロック同期式モードはサポートしていません。
          内蔵ボーレートジェネレータのみで、外部クロック入出力はサポートして
          いません。
          DTR信号出力、DSR信号入力、CD信号入力はサポートしていません。
          SHシリーズ特有のマルチプロセッサモードはサポートしていません。

NORTiのバージョン

          本ファイルは、NORTi2, NORTi3, NORTi Ver.4で兼用となっています。
          V2, V3, V4 マクロで、NORTiのバージョンを指定してください。
          省略した場合は、V4 と見なします。
*/

#include <stdlib.h>
#include <string.h>

#ifdef V2
 #include "norti.h"
 #define tslp_tsk(p) wai_tsk(p)
#elif defined(V3)
 #include "norti3.h"
#else /* V4 */
 #ifndef V4
 #define V4
 #endif
 #include "kernel.h"
#endif

#include "nosio.h"
#include "sh7040.h"
#include "nos7040.h"

/* 割込み優先順位の定義 */

#ifndef IP
#define IP          7           /* IP 未定義なら 7 とする */
#endif

/* クロックの定義 */

#define MHz         1000000L
#ifndef CLK
#define CLK         24*MHz      /* CPUクロック[Hz] */
#endif

/* 送受信バッファサイズの定義 */

#ifndef BUFSZ
#define BUFSZ       1024        /* 受信バッファ長 */
#endif
#ifndef TXBUFSZ
#define TXBUFSZ     BUFSZ       /* 送信バッファ長 */
#endif

/* フロー制御の定数 */

#define XOFF_LEN    (BUFSZ*3/4) /* XOFF送信バッファ長 */
#define XON_LEN     (BUFSZ*1/4) /* XON 送信バッファ長 */
#define RTSOFF_LEN  (BUFSZ-18)  /* RTS信号OFFバッファ長 */
#define RTSON_LEN   (BUFSZ-19)  /* RTS信号ON バッファ長 */

#define XOFF        0x13        /* 送信停止キャラクタ（CTRL-S）*/
#define XON         0x11        /* 送信再開キャラクタ（CTRL-Q）*/

/* 論理チャネル番号（CH=0～3）*/

#ifndef CH
#define CH          0           /* 無指定の場合, 論理チャネルは0 */
#endif

/* 処理系の差異の調整 */

#if defined(GAIO)               /* XASS-V */
#include <ht1_inline.h>
#elif defined(__GNUC__)         /* exeGCC */
#elif defined(__ghs)            /* Green Hills */
#elif defined(__TID__)          /* IAR EW/ICC */
#else                           /* Renesas C */
#ifndef __HITACHI__
#define __HITACHI__
#endif
#include <machine.h>
#endif

/* 物理チャネル番号（CH0,CH1）*/

#ifndef CH0
#ifndef CH1
#if (CH==0)
#define CH0             /* 無指定の場合, 論理チャネル0 の物理チャネルは0 */
#elif (CH==1)
#define CH1             /* 無指定の場合, 論理チャネル1 の物理チャネルは1 */
#elif (CH==2)
#define CH0             /* 無指定の場合, 論理チャネル2 の物理チャネルは0 */
#else
#define CH1             /* 無指定の場合, 論理チャネル3 の物理チャネルは1 */
#endif
#endif
#endif

/* CHx に合わせた再定義 */

#ifdef CH0
#define SCI_SMR     SCI_SMR0    /* シリアルモードレジスタ */
#define SCI_BRR     SCI_BRR0    /* ビットレートレジスタ */
#define SCI_SCR     SCI_SCR0    /* シリアルコントロールレジスタ */
#define SCI_TDR     SCI_TDR0    /* トランスミットデータレジスタ */
#define SCI_SSR     SCI_SSR0    /* シリアルステータスレジスタ */
#define SCI_RDR     SCI_RDR0    /* レシーブデータレジスタ */
#define PA_TXD      PA_TXD0     /* TxD 出力端子 */
#define PA_RXD      PA_RXD0     /* RxD 入力端子 */
#define PA_CTS      PA_CTS0     /* CTS 入力端子 */
#define INT_ERI     INT_ERI0    /* 受信エラー割込みベクタ番号 */
#define INT_RXI     INT_RXI0    /* 受信完了割込みベクタ番号 */
#define INT_TXI     INT_TXI0    /* 送信データエンプティ割込みベクタ番号 */
#define INT_TEI     INT_TEI0    /* 送信完了割込みベクタ番号 */
#define INT_IRQ     INT_IRQ0    /* CTS 割込みベクタ番号 */
#define IRQ         0           /* CTS 割込み要求（IRQ0）*/
#endif

#ifdef CH1
#define SCI_SMR     SCI_SMR1
#define SCI_BRR     SCI_BRR1
#define SCI_SCR     SCI_SCR1
#define SCI_TDR     SCI_TDR1
#define SCI_SSR     SCI_SSR1
#define SCI_RDR     SCI_RDR1
#define PA_TXD      PA_TXD1
#define PA_RXD      PA_RXD1
#define PA_CTS      PA_CTS1
#define INT_ERI     INT_ERI1
#define INT_RXI     INT_RXI1
#define INT_TXI     INT_TXI1
#define INT_TEI     INT_TEI1
#define INT_IRQ     INT_IRQ1
#define IRQ         1           /* CTS 割込み要求（IRQ1）*/
#endif

/* シリアル入出力制御ブロック */

extern T_SIO SIO[];

/* 内部変数 */

static UB txbuf[TXBUFSZ];       /* 送信バッファ */
static UB rxbuf[BUFSZ*2];       /* 受信バッファ, 受信ステータスバッファ */

/* 割込みハンドラ定義情報 */

#ifdef V4
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
INTHDR int_sio0_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio0_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio0_te };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
INTHDR int_sio1_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio1_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio1_te };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
INTHDR int_sio2_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio2_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio2_te };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
INTHDR int_sio3_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio3_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio3_te };
#endif
#else
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
INTHDR int_sio0_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio0_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio0_te };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
INTHDR int_sio1_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio1_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio1_te };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
INTHDR int_sio2_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio2_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio2_te };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
INTHDR int_sio3_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio3_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio3_te };
#endif
#endif

#if defined(__HITACHI__)
#if (CH==0)
#pragma interrupt(int_sio0_rx)
#pragma interrupt(int_sio0_tx)
#pragma interrupt(int_sio0_te)
#elif (CH==1)
#pragma interrupt(int_sio1_rx)
#pragma interrupt(int_sio1_tx)
#pragma interrupt(int_sio1_te)
#elif (CH==2)
#pragma interrupt(int_sio2_rx)
#pragma interrupt(int_sio2_tx)
#pragma interrupt(int_sio2_te)
#elif (CH==3)
#pragma interrupt(int_sio3_rx)
#pragma interrupt(int_sio3_tx)
#pragma interrupt(int_sio3_te)
#endif
#endif

#ifndef NOFLW
/*****************************************************************************
* RTS 信号 ON/OFF（内部関数）
*
******************************************************************************/

static void rts_on(void)
{
    UINT psw;

    psw = vdis_psw();       /* 割り込み禁止 */
  #ifdef CH0
    rts0_on();
  #endif
  #ifdef CH1
    rts1_on();
  #endif
    vset_psw(psw);          /* 割り込み禁止復元 */
}

static void rts_off(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    rts0_off();
  #endif
  #ifdef CH1
    rts1_off();
  #endif
    vset_psw(psw);
}

#endif
/*****************************************************************************
* シリアル入出力制御ブロック初期化（内部関数）
*
******************************************************************************/

static void init_buf(void)
{
    int init;

    /* 制御ブロッククリア */

    init = SIO[CH].flag & TSF_INIT;
    memset(&SIO[CH], 0, sizeof SIO[CH]);
    SIO[CH].flag = (UB)init;
    SIO[CH].ch = CH;

    /* ポインタ類セットアップ */

    SIO[CH].txbuf = txbuf;          /* 送信バッファ */
    SIO[CH].rxbuf = rxbuf;          /* 受信バッファ */
    SIO[CH].txputp = SIO[CH].txbuf; /* 送信バッファ格納ポインタ初期値 */
    SIO[CH].txgetp = SIO[CH].txbuf; /* 送信バッファ取得ポインタ初期値 */
    SIO[CH].rxputp = SIO[CH].rxbuf; /* 受信バッファ格納ポインタ初期値 */
    SIO[CH].rxgetp = SIO[CH].rxbuf; /* 受信バッファ取得ポインタ初期値 */
}

/*****************************************************************************
* ステータス入力 & 編集（内部関数）
*
*    bit7                                      bit0
*   [ DSR | BD  | FE  | OE  | PE  |TXEMP| CTS | CD  ]
*     (1)                                       (1)
*
* DSR, CD は 無いので常時 1 とする
******************************************************************************/

static int get_stat(void)
{
    int stat;
    int ssr;

    /* 現在のステータス入力 */

    ssr = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);

    /* 受信時のステータスとOR */

    ssr |= SIO[CH].rxsts;

    /* ブレークOFF検出 */

    if (sfr_inw(IO_PADRL) & PA_RXD) /* RxD 端子"1" */
        ssr &= ~SSR_MPBT;           /* ブレーク検出は MPBT に割り当て */

    /* ステータスビット編集 */

    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD 常時ON */

    if (ssr & SSR_TEND)             /* 送信空 */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)              /* パリティエラー */
        stat |= TSIO_PE;
    if (ssr & SSR_ORER)             /* オーバランエラー */
        stat |= TSIO_OE;
    if (ssr & SSR_FER)              /* フレーミングエラー */
        stat |= TSIO_FE;
    if (ssr & SSR_MPBT)             /* ブレーク検出 */
        stat |= TSIO_BD;
  #ifndef NOFLW
    if (sfr_inw(IO_PADRL) & PA_CTS) /* /CTS 入力 */
        stat &= ~TSIO_CTS;
  #endif

    return stat;
}

/*****************************************************************************
* ＳＩＯデバイス初期化（内部関数）
*
******************************************************************************/

static BOOL init_sio(UB mode, UW baud)
{
    int cks;
    UW brr;

    /* RTS OFF */

  #ifndef NOFLW
    rts_off();
  #endif

    /* リセット */

    sfr_out(SCI_SCR, 0x00);

    /* ボーレート設定値計算 */

    cks = 0;
    for (brr = (CLK) / (baud * 32); brr > 0x100L; brr >>= 2)
    {   if (++cks > 3)
            return FALSE;
    }

    /* モード／ボーレート設定 */

    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* ボーレートの1ビット期間経過待ち */
    tslp_tsk(2);

    /* フロー制御ある場合は、同割込みを初期化 */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        sfr_clr(INTC_ICR + 1, 0x80 >> IRQ); /* IRQ レベルセンス */
  #endif

    return TRUE;
}

/*****************************************************************************
* CTS 割込み禁止（内部関数）
*
******************************************************************************/
#ifndef NOFLW

static void dis_int_cts(void)
{
    UINT psw;

    if (SIO[CH].cmd[1] == 0)
        return;

    psw = vdis_psw();
  #if (IRQ<4)
    sfr_clrw(INTC_IPRA, 0x000f << (12 - (IRQ * 4)));
  #else
    sfr_clrw(INTC_IPRB, 0x000f << (12 - ((IRQ - 4) * 4)));
  #endif
    SIO[CH].cmd[1] = 0;
    vset_psw(psw);
}

#endif
/*****************************************************************************
* CTS 割込み許可（内部関数）
*
******************************************************************************/
#ifndef NOFLW

static void ena_int_cts(void)
{
    UINT psw;

    if (SIO[CH].cmd[1] != 0)
        return;

    psw = vdis_psw();
  #if (IRQ<4)
    sfr_setw(INTC_IPRA, (IP) << (12 - (IRQ * 4)));
  #else
    sfr_setw(INTC_IPRB, (IP) << (12 - ((IRQ - 4) * 4)));
  #endif
    SIO[CH].cmd[1] = 1;
    vset_psw(psw);
}

#endif
/*****************************************************************************
* シリアル割込み禁止（内部関数）
*
******************************************************************************/

static void dis_int_sio(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_clrw(INTC_IPRF, 0x00f0);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRF, 0x000f);
  #endif
    vset_psw(psw);

  #ifndef NOFLW
    dis_int_cts();
  #endif
}

/*****************************************************************************
* シリアル割込み許可（内部関数）
*
******************************************************************************/

static void ena_int_sio(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_setw(INTC_IPRF, IP << 4);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRF, IP);
  #endif
    vset_psw(psw);
}

/*****************************************************************************
* 送信バッファクリア（内部関数）
*
******************************************************************************/

static void clr_txbuf(void)
{
    SIO[CH].txcnt = 0;
    SIO[CH].txgetp = SIO[CH].txputp;
}

/*****************************************************************************
* 受信バッファクリア（内部関数）
*
******************************************************************************/

static void clr_rxbuf(void)
{
    /* 受信バッファクリア */

    SIO[CH].rxsts = 0;
    SIO[CH].oldsts = 0;
    SIO[CH].eotcnt = 0;
    SIO[CH].rxcnt = 0;
    SIO[CH].rxgetp = SIO[CH].rxputp;

    /* XON/OFFフロー制御の解除 */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_TXOFF)
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF 送信要求セット */
        sfr_set(SCI_SCR, SCR_TIE);  /* 送信割込み許可 */
    }
  #endif

    /* RTSフロー制御の解除 */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        rts_on();
  #endif
}

/*****************************************************************************
* 送信バッファへ１文字格納（内部関数）
*
* バッファ満杯で格納できなかった場合は、FALSE を返す。
******************************************************************************/

static BOOL put_txbuf(UB c)
{
    UB *p;

    /* バッファ内文字数 + 1 */
    /* バッファ満杯チェック */

    if (++SIO[CH].txcnt > TXBUFSZ)
    {   SIO[CH].txcnt = TXBUFSZ;
        return FALSE;
    }

    /* バッファへ格納 */

    p = SIO[CH].txputp;
    *p = c;

    /* 格納ポインタを１つ進める */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txputp = p;

    /* 送信割込み許可 */

    sfr_set(SCI_SCR, SCR_TIE);

    return TRUE;
}

/*****************************************************************************
* 受信バッファへ受信文字/ステータス格納（内部関数）
*
******************************************************************************/

static void put_rxbuf(void)
{
    int cnt;
    UB *p;

    /* 連続ブレークなら格納しない */

    if ((SIO[CH].rxsts & SSR_MPBT)
     && (SIO[CH].oldsts & SSR_MPBT))
        return;

    /* バッファ満杯チェック */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
        SIO[CH].rxsts |= SSR_RDRF; /* オーバフローは RDRF ビットに割り当て */

    /* バッファ内文字数 + 1 */

    SIO[CH].rxcnt = (UH)cnt;

    /* XON/OFFフロー制御 */

  #ifndef NOXON
    if ((cnt == XOFF_LEN)
     && ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON))
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF 送信要求セット */
        sfr_set(SCI_SCR, SCR_TIE);  /* 送信割込み許可 */
    }
  #endif

    /* RTSフロー制御 */

  #ifndef NOFLW
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_off();
  #endif

    /* バッファへ格納 */
    /* 終端文字検出+1 */

    p = SIO[CH].rxputp;
    if ((*p = SIO[CH].rxchr) == SIO[CH].eot)
        SIO[CH].eotcnt++;
    *(p + BUFSZ) = SIO[CH].rxsts;

    /* 格納ポインタを１つ進める */

    if (++p >= SIO[CH].rxbuf + BUFSZ)
        p = SIO[CH].rxbuf;
    SIO[CH].rxputp = p;
}

/*****************************************************************************
* 受信バッファから１文字取得（内部関数）
*
* バッファ空で取得できなかった場合は、-1 を返す。
******************************************************************************/

static int get_rxbuf(UB *c)
{
    int cnt;
    int sts;
    UB *p;

    /* 受信バッファ空チェック */

    cnt = SIO[CH].rxcnt;
    if (--cnt == -1)
        return cnt;

    /* 受信バッファ内文字数 - 1 */

    SIO[CH].rxcnt = (UH)cnt;

    /* XON/OFFフロー制御 */

  #ifndef NOXON
    if ((cnt == XON_LEN)
     && (SIO[CH].flag & TSF_TXOFF))
    {   SIO[CH].flag |= TSF_TXREQ;      /* XON/XOFF 送信要求セット */
        sfr_set(SCI_SCR, SCR_TIE);      /* 送信割込み許可 */
    }
  #endif

    /* RTSフロー制御 */

  #ifndef NOFLW
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_on();
  #endif

    /* 受信バッファから取得 */
    /* 終端文字検出数-1 */

    p = SIO[CH].rxgetp;
    if ((*c = *p) == SIO[CH].eot)
        SIO[CH].eotcnt--;
    sts = *(p + BUFSZ);

    /* 取得ポインタを１つ進める */

    if (++p >= SIO[CH].rxbuf + BUFSZ)
        p = SIO[CH].rxbuf;
    SIO[CH].rxgetp = p;

    return sts;
}

/*****************************************************************************
* 受信割込みハンドラ本体（内部関数）
*
******************************************************************************/

static void rx_int(void)
{
    int sts;
    int chr;
    int tid;

    /* 受信文字とステータスを入力 */

    chr = sfr_in(SCI_RDR);
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);

    /* 受信割込みをクリア */

    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));

    /* ブレーク検出 */

    if ((sts & SSR_FER)                 /* フレーミングエラーで */
     && !(sfr_inw(IO_PADRL) & PA_RXD))  /* RxD 端子"0" */
        sts |= SSR_MPBT;                /* ブレーク検出は MPBT に割り当て */
    SIO[CH].oldsts = SIO[CH].rxsts;     /* 前回の受信ステータス記憶 */
    SIO[CH].rxsts = (UB)sts;
    SIO[CH].rxchr = (UB)chr;

    /* XON/OFFフロー制御有りの場合 */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_XON)
    {
        /* XOFF受信なら送信中断させる */

        if (chr == XOFF)
        {   SIO[CH].flag |= TSF_RXOFF;
            return;
        }

        /* XON 受信なら送信再開させる */

        if (chr == XON)
        {   SIO[CH].flag &= ~TSF_RXOFF;
            sfr_set(SCI_SCR, SCR_TIE);      /* 送信割込み許可 */
            return;
        }
    }
  #endif

    /* 受信バッファへ格納 */

    put_rxbuf();

    /* 受信待ち解除 */

    if ((tid = SIO[CH].rxtid) != 0)
        wup_tsk((ID)tid);
}

/*****************************************************************************
* 受信割込みハンドラ
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_rx(void)
#elif (CH==1)
INTHDR int_sio1_rx(void)
#elif (CH==2)
INTHDR int_sio2_rx(void)
#elif (CH==3)
INTHDR int_sio3_rx(void)
#endif
{
    ent_int();              /* 割込みハンドラの開始 */
    rx_int();               /* 受信割込みハンドラ本体 */
    ret_int();              /* 割込みハンドラから復帰する */
}

/*****************************************************************************
* 送信割込みハンドラ本体（内部関数）
*
******************************************************************************/

static void tx_int(void)
{
    UB *p;
    int tid;

    /* CTS 割込み禁止 */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        dis_int_cts();
  #endif

    /* 送信ディセーブル中なら送信しない */

    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;

    /* RTSフロー制御ありなら CTS チェック */
    /* CTS OFF なら CTS 割込み許可 */

  #ifndef NOFLW
    if ((SIO[CH].flag & TSF_RTS) && (sfr_inw(IO_PADRL) & PA_CTS))
    {   ena_int_cts();
        goto L2;
    }
  #endif

    /* 受信側から依頼されたXON/XOFFの送信を優先 */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_TXREQ)
    {   SIO[CH].flag &= ~TSF_TXREQ;
        if (SIO[CH].flag & TSF_TXOFF)
        {   SIO[CH].flag &= ~TSF_TXOFF;
            sfr_out(SCI_TDR, XON);
        }
        else
        {   SIO[CH].flag |= TSF_TXOFF;
            sfr_out(SCI_TDR, XOFF);
        }
        sfr_clr(SCI_SSR, SSR_TDRE);
        return;
    }

    /* XOFF受信していたら送信不可 */

    if (SIO[CH].flag & TSF_RXOFF)
        goto L2;
  #endif

    /* 送信バッファ内文字数 - 1 */
    /* 送信バッファ空なら送信不可 */

    if (--SIO[CH].txcnt == (UH)-1)
    {
        SIO[CH].txcnt = 0;

        /* 送信終了割込み許可 */

        sfr_set(SCI_SCR, SCR_TEIE);

        /* 送信割込み禁止 */
    L2:
        sfr_clr(SCI_SCR, SCR_TIE);
        return;
    }

    /* １文字送信 */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);
    sfr_clr(SCI_SSR, SSR_TDRE);

    /* 取得ポインタを１つ進める */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* 送信待ち解除 */

    if ((tid = SIO[CH].txtid) != 0)
        wup_tsk((ID)tid);
}

/*****************************************************************************
* 送信割込みハンドラ
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_tx(void)
#elif (CH==1)
INTHDR int_sio1_tx(void)
#elif (CH==2)
INTHDR int_sio2_tx(void)
#elif (CH==3)
INTHDR int_sio3_tx(void)
#endif
{
    ent_int();              /* 割込みハンドラの開始 */
    tx_int();               /* 送信割込みハンドラ本体 */
    ret_int();              /* 割込みハンドラから復帰する */
}

/*****************************************************************************
* 割込みハンドラ送信終了処理（内部関数）
*
******************************************************************************/

static void te_int(void)
{
    /* 送信終了待ち解除 */

    if (SIO[CH].tetid != 0)
        wup_tsk(SIO[CH].tetid);

    /* 送信終了割込み禁止 */

    sfr_clr(SCI_SSR, SSR_TEND);
    sfr_clr(SCI_SCR, SCR_TEIE);
}

/*****************************************************************************
* 送信終了割込みハンドラ
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_te(void)
#elif (CH==1)
INTHDR int_sio1_te(void)
#elif (CH==2)
INTHDR int_sio2_te(void)
#elif (CH==3)
INTHDR int_sio3_te(void)
#endif
{
    ent_int();                  /* 割込みハンドラの開始 */
    te_int();                   /* 送信終了処理 */
    ret_int();                  /* 割込みハンドラから復帰する */
}

/*****************************************************************************
* 初期化パラメータ解析（内部関数）
*
******************************************************************************/

static BOOL set_param(const B *s, UB *mode, UW *baud)
{
    B c;
    UW b;

    /* モード判別 */

    *mode = 0;
    if      (strstr((const char *)s, (char *)"PE") != NULL)   *mode |= SMR_PE;
    else if (strstr((const char *)s, (char *)"PO") != NULL)   *mode |= SMR_PE|SMR_OE;
    if      (strstr((const char *)s, (char *)"S2") != NULL)   *mode |= SMR_STOP;
    if      (strstr((const char *)s, (char *)"B7") != NULL)   *mode |= SMR_CHR;

    /* フロー制御有無判別 */

    if (strstr((const char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((const char *)s, (char *)"RTS") != NULL)   SIO[CH].flag |= TSF_RTS;

    /* 終端文字指定判別 */

    if (strstr((const char *)s, (char *)"ETX") != NULL)   SIO[CH].eot = 0x03;
    if (strstr((const char *)s, (char *)"EOT") != NULL)   SIO[CH].eot = 0x04;
    if (strstr((const char *)s, (char *)"CR" ) != NULL)   SIO[CH].eot = 0x0d;
    if (strstr((const char *)s, (char *)"LF" ) != NULL)   SIO[CH].eot = 0x0a;

    /* ボーレート判別 */
    /* 3桁以上の数値検索して変換 */

    for (;;)
    {
        c = *s++;
        if (c == '\0')
        {   *baud = 9600L;
            break;
        }
        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;

        s -= 3;
        b = 0L;
        for (;;)
        {   c = *s++;
            if (c < '0' || c > '9')
                break;
            b = b * 10 + (c - '0');
        }
        *baud = b;
        break;
    }
    return TRUE;
}

/*****************************************************************************
* シリアル入出力初期化
*
******************************************************************************/

#if (CH==0)
ER ini_sio0(const B *param)
#elif (CH==1)
ER ini_sio1(const B *param)
#elif (CH==2)
ER ini_sio2(const B *param)
#elif (CH==3)
ER ini_sio3(const B *param)
#endif
{
    UW baud;
    UB mode;

    dis_int_sio();

    /* 制御ブロック初期化 */

    init_buf();

    /* パラメータ解析 */

    if (!set_param(param, &mode, &baud))
        return E_PAR;

    /* デバイス初期化 */

    if (!init_sio(mode, baud))
        return E_PAR;

    /* 割込みハンドラの定義 */

    SIO[CH].flag |= TSF_INIT;
  #ifdef V4
    def_inh(INT_ERI, &dinh_sio_rx);
    def_inh(INT_RXI, &dinh_sio_rx);
    def_inh(INT_TXI, &dinh_sio_tx);
    def_inh(INT_TEI, &dinh_sio_te);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, &dinh_sio_tx);
   #endif
  #else
    def_int(INT_ERI, &dint_sio_rx);
    def_int(INT_RXI, &dint_sio_rx);
    def_int(INT_TXI, &dint_sio_tx);
    def_int(INT_TEI, &dint_sio_te);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, &dint_sio_tx);
   #endif
  #endif

    /* 受信割込み許可 */

    sfr_set(SCI_SCR, SCR_RIE);
    ena_int_sio();
    return E_OK;
}

/*****************************************************************************
* シリアル入出力終了
*
******************************************************************************/

#if (CH==0)
void ext_sio0(void)
#elif (CH==1)
void ext_sio1(void)
#elif (CH==2)
void ext_sio2(void)
#elif (CH==3)
void ext_sio3(void)
#endif
{
    if (!(SIO[CH].flag & TSF_INIT))     /* 未初期化なら何もしない */
        return;
    dis_int_sio();                      /* シリアル割込み禁止 */
    sfr_out(SCI_SCR, 0x00);             /* 送受信禁止 */
  #ifndef NOFLW
    rts_off();                          /* RTS OFF */
  #endif
  #ifdef V4
    def_inh(INT_ERI, (T_DINH *)NADR);   /* 割込み定義を取り消し */
    def_inh(INT_RXI, (T_DINH *)NADR);
    def_inh(INT_TXI, (T_DINH *)NADR);
    def_inh(INT_TEI, (T_DINH *)NADR);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, (T_DINH *)NADR);
   #endif
  #else
    def_int(INT_ERI, (T_DINT *)NADR);   /* 割込み定義を取り消し */
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
    def_int(INT_TEI, (T_DINT *)NADR);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (T_DINT *)NADR);
   #endif
  #endif
    SIO[CH].flag &= ~TSF_INIT;          /* 初期化済みフラグクリア */
}

/*****************************************************************************
* シリアル１文字入力
*
******************************************************************************/

#if (CH==0)
ER get_sio0(UB *c, TMO tmout)
#elif (CH==1)
ER get_sio1(UB *c, TMO tmout)
#elif (CH==2)
ER get_sio2(UB *c, TMO tmout)
#elif (CH==3)
ER get_sio3(UB *c, TMO tmout)
#endif
{
    ER ercd;
    int sts;

    for (;;)
    {
        chg_ims(IP);

        /* 受信バッファから１文字得る */

        sts = get_rxbuf(c);

        if (sts != -1)              /* 受信文字あった場合 */
        {
            chg_ims(0);

            /* 受信エラー判別 */

            if (sts & (SSR_RDRF|SSR_ORER|SSR_MPBT|SSR_FER|SSR_PER))
            {   if (sts & SSR_RDRF)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_ORER)
                    ercd = EV_SIOORN;
                else if (sts & SSR_MPBT)
                    ercd = EV_SIOBRK;
                else if (sts & SSR_FER)
                    ercd = EV_SIOFRM;
                else /* SSR_PER */
                    ercd = EV_SIOPTY;
            }
            else
                ercd = E_OK;
            return ercd;
        }

        /* 受信割込み待ち */

        SIO[CH].rxtid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* 複数回 wup_tsk された場合の対策 */
        if (ercd)
            return ercd;            /* タイムアウト終了 */
    }
}

/*****************************************************************************
* シリアル１文字出力
*
******************************************************************************/

#if (CH==0)
ER put_sio0(UB c, TMO tmout)
#elif (CH==1)
ER put_sio1(UB c, TMO tmout)
#elif (CH==2)
ER put_sio2(UB c, TMO tmout)
#elif (CH==3)
ER put_sio3(UB c, TMO tmout)
#endif
{
    ER ercd;

    for (;;)
    {
        chg_ims(IP);

        /* 送信バッファへ１文字格納 */

        if (put_txbuf(c))           /* 格納できた場合 */
        {   chg_ims(0);
            return E_OK;            /* 正常終了 */
        }

        /* 送信割込み待ち */

        SIO[CH].txtid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* 複数回 wup_tsk された場合の対策 */
        if (ercd)
            return ercd;            /* タイムアウト終了 */
    }
}

/*****************************************************************************
* シリアル入出力制御
*
******************************************************************************/

#if (CH==0)
ER ctl_sio0(UH fncd)
#elif (CH==1)
ER ctl_sio1(UH fncd)
#elif (CH==2)
ER ctl_sio2(UH fncd)
#elif (CH==3)
ER ctl_sio3(UH fncd)
#endif
{
    UB c;

    chg_ims(IP);

    /* バッファクリア */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* ブレーク送信 */

    if (fncd & TSIO_SBON)
    {   fncd |= TSIO_TXD;
        sfr_clrw(IO_PADRL, PA_TXD);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        sfr_setw(IO_PADRL, PA_TXD);
    }

    /* シリアルモード変更 */

    if (fncd & (TSIO_RXE|TSIO_RXD|TSIO_TXE|TSIO_TXD))
    {   c = sfr_in(SCI_SCR);
        if (fncd & TSIO_RXE)        c |=  SCR_RE;
        if (fncd & TSIO_RXD)        c &= ~SCR_RE;
        if (fncd & TSIO_TXE)        c |=  SCR_TE;
        if (fncd & TSIO_TXD)        c &= ~SCR_TE;
        sfr_out(SCI_SCR, c);
    }

    /* RTS 出力 */

  #ifndef NOFLW
    if (fncd & (TSIO_RTSON|TSIO_RTSOFF))
    {   if (fncd & TSIO_RTSON)      rts_on();
        if (fncd & TSIO_RTSOFF)     rts_off();
    }
  #endif

    chg_ims(0);
    return E_OK;
}

/*****************************************************************************
* シリアル入出力状態参照
*
******************************************************************************/

#if (CH==0)
ER ref_sio0(T_SIOS *pk_sios)
#elif (CH==1)
ER ref_sio1(T_SIOS *pk_sios)
#elif (CH==2)
ER ref_sio2(T_SIOS *pk_sios)
#elif (CH==3)
ER ref_sio3(T_SIOS *pk_sios)
#endif
{
    int stat;

    chg_ims(IP);

    stat = get_stat();
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = (UB)stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(0);
    return E_OK;
}

/*****************************************************************************
* シリアル送信バッファフラッシュ
*
******************************************************************************/

#if (CH==0)
ER fls_sio0(TMO tmout)
#elif (CH==1)
ER fls_sio1(TMO tmout)
#elif (CH==2)
ER fls_sio2(TMO tmout)
#elif (CH==3)
ER fls_sio3(TMO tmout)
#endif
{
    ER ercd;

    for (;;)
    {
        chg_ims(IP);

        if (SIO[CH].txcnt == 0)
        {   if (sfr_in(SCI_SSR) & SSR_TEND)
            {   chg_ims(0);
                return E_OK;            /* 正常終了 */
            }
        }

        /* 送信終了割込み待ち */

        SIO[CH].tetid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* 複数回 wup_tsk された場合の対策 */
        if (ercd)
            return ercd;                /* タイムアウト終了 */
    }
}

/* end */
