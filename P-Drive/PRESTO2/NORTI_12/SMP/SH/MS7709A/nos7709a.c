/****************************************************************************
* シルアル入出力ドライバ（SH7709A）                                         *
*                                                                           *
*  File name : nos7709a.c                                                   *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -cp=sh3 -op=1 -def=CLK=? nos7709a.c         (Renesas C)    *
*            gcc -c -m3 -O2 -DCLK=? nos7709a.c               (exeGCC)       *
*            ccsh -c -cpu=sh_iii -ansi -O -DCLK=? nos7709a.c (GHS)          *
*                                                                           *
* 07/Sep/1998 作成                                                          *
* 22/Apr/2000 SH7709A 用として                                              *
* 28/Sep/2000 CH0 選択時の不具合修正                                        *
* 29/Sep/2000 CH0でSSR_RDF未定義エラーとなる不具合を修正                 MI *
* 31/Oct/2000 IAR EW 対応                                                TK *
* 13/Jul/2001 CH1,CH2のSSRアクセスをバイトからワードに修正               MI *
* 12/Sep/2001 制御系の判断を見直し                                       TA *
* 25/Sep/2001 CH2のSCFCR MCEビット誤り修正, 0x80→0x08                   MI *
* 25/Sep/2001 CH1,CH2の受信割込みでFERとPERを見ていた不具合修正他        MI *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え   KA *
* 25/Feb/2003 char/unsigned char のキャストを追加(ワーニングの削除)      KA *
* 25/Feb/2003 "Green Hills" コンパイラの判断を __ghs でするように修正    KA *
* 31/Jul/2003 BRR設定後 ウェイト挿入                                     KA *
****************************************************************************/

/*

最適化コンパイル

          割込みハンドラが含まれるので、必ず最適化オプションを付けてコンパイ
          ルしてください。

チャネル番号の指定

          CH マクロで、論理チャネル番号を指定できます。（無指定時は、CH=0）
          CH は 0～3 を指定できます。

          CH0, CH1, CH2 マクロで、物理チャネル番号を変更できます。
          CH0 を定義すると、SCI を使うようにコンパイルされます。
          CH1 を定義すると、IrDA の SCIF を使うようにコンパイルされます。
          CH2 を定義すると、FIFO 付き SCIF を使うようにコンパイルされます。

          チャネル0 とチャネル1 の両方をリンクする場合は、重複しないモジュー
          ル名となるようコンパイルしてください。

          (例) shc <options> nos7709a.c  ･･･････････････････････ チャネル0
               copy nos7709a.c nos7ch1.c
               shc <options> -def=CH=1 nos7ch1.c  ･････････････ チャネル1

          (例) gcc <options> nos7709a.c  ･･･････････････････････ チャネル0
               gcc <options> -DCH=1 -onos7ch1.o nos7709a.c ･････ チャネル1

          (例) ccsh <options> nos7709a.c  ･･････････････････････ チャネル0
               ccsh <options> -DCH=1 -o nos7ch1.o nos7709a.c ･･･ チャネル1

ボーレートジェネレータ

          ソースクロックのデフォルトは 33 MHz です。変更する場合 は CLK マク
          ロを定義してコンパイルしてください。値は [Hz] 単位の周波数です。

          (例) shc <options> -def=CLK=25000000 nos7709a.c ･･････ 25 MHz
                                  ~~~~~~~~~~~~
          (例) gcc <options> -DCLK=25000000 nos7709a.c
                               ~~~~~~~~~~~~
送受信バッファサイズの指定

          受信／送信バッファのサイズを、BUFSZ マクロにより定義してコンパイル
          してください。受信バッファと異なるサイズの送信バッファを用いる場合
          は、さらに TXBUFSZ マクロを定義してください。
          未指定時は、受信／送信バッファ共 1024 バイトとなります。

          (例) 受信バッファ, 送信バッファ共 128 バイト BUFSZ=128
          (例) 受信バッファ128, 送信バッファ 64 バイト BUFSZ=128, TXBUFSZ=64

ソフトウェアフロー制御

          XON/XOFF キャラクタによるフロー制御を用いない場合、NOXON マクロを定
          義してコンパイルしてください。コードサイズと処理速度を節約できます。

ハードウェアフロー制御（CH0 と CH1）

          チャネル0 /RTS 信号 を SCK0/SCPT[1] 端子に接続してください。
          チャネル0 /CTS 信号 を /IRQ0 端子に接続してください。
          チャネル1 /RTS 信号 を SCK1/SCPT[3] 端子に接続してください。
          チャネル1 /CTS 信号 を /IRQ1 端子に接続してください。

          ハードフローを用いない場合は、NOFLW マクロ定義してコンパイルしてく
          ださい。コードサイズと処理速度を節約できます。

ハードウェアフロー制御（CH2）

          SCIF 内蔵の RTS-CTS 機能を使い、ソフトウェアに依らず行います。

割込み優先順位

          本ハンドラの、デフォルトの割込み優先順位は、7 です。
          これを変更する場合、IP マクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。（IP ≦ KNL_LEVEL）
          本ハンドラより高優先度の割込みハンドラが有る場合、多重割込みが起こ
          ります。

          (例) shc <options> -def=IP=5 nos7709a.c ･･･ 割込み優先順位 5
                                  ~~~~
          (例) gcc <options> -DIP=5 nos7709a.c
                               ~~~~
未サポート機能

          調歩同期式モードのみで、クロック同期式モードはサポートしていません。
          内蔵ボーレートジェネレータのみで、外部クロック入出力はサポートして
          いません。
          DTR 信号出力、DSR 信号入力、CD 信号入力はサポートしていません。
          SH7709 特有のマルチプロセッサモードはサポートしていません。
*/

#include <stdlib.h>
#include <string.h>

#ifndef SH3
#define SH3
#endif

#ifdef V2
 #include "norti.h"
 #define PSW         UW
 #define tslp_tsk(p) wai_tsk(p)
#elif defined(V3)
 #include "norti3.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #ifndef iwup_tsk
  #define iwup_tsk(i) wup_tsk(i)
 #endif
 #define PSW         UINT
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #define PSW         UINT
#endif

#include "nosio.h"
#include "sh7709a.h"

/* 割込み優先順位の定義 */

#ifndef IP
#define IP          7           /* IP 未定義なら 7 とする */
#endif

/* クロックの定義 */

#ifndef CLK
#define CLK         33333333    /* 周辺モジュール用クロック[Hz] */
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

#if defined(GAIO)               /* XASSV */
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
#ifndef CH2
#if (CH==0)
#define CH0             /* 無指定の場合, 論理チャネル0 の物理チャネルは0 */
#elif (CH==1)
#define CH1             /* 無指定の場合, 論理チャネル1 の物理チャネルは1 */
#elif (CH==2)
#define CH2             /* 無指定の場合, 論理チャネル2 の物理チャネルは2 */
#else
#define CH0             /* 無指定の場合, 論理チャネル3 の物理チャネルは0 */
#endif
#endif
#endif
#endif
#include "nos7709a.h"

/* CTS 入力 IRQ 番号 */

#ifndef IRQ
#ifdef CH0
#define IRQ         0
#else
#define IRQ         1
#endif
#endif

#ifndef NOFLW
#ifndef CH2
#define PH_CTS  (0x01<<IRQ)     /* ポートH CTS 入力端子 */
#define INT_IRQ (INT_IRQ0+IRQ)  /* CTS 割込みベクタ番号 */
#endif
#endif

/* CHx に合わせた再定義 */

#ifdef CH0
#define SCI_SMR     SCI_SCSMR
#define SCI_BRR     SCI_SCBRR
#define SCI_SCR     SCI_SCSCR
#define SCI_TDR     SCI_SCTDR
#define SCI_SSR     SCI_SCSSR
#define SCI_RDR     SCI_SCRDR
#define SCP_TXD     SCP0DT      /* SCポート TxD 出力端子 */
#define SCP_RXD     SCP0DT      /* SCポート RxD 入力端子 */
#define SCP_RTS     SCP1DT      /* SCポート RTS 出力端子 */
#define SCPnMD0     SCP0MD0     /* SCポート TxD 出力モード0 */
#define SCPnMD1     SCP0MD1     /* SCポート TxD 出力モード1 */
#endif

#ifdef CH1
#define SCI_SMR     IrDA_SCSMR1
#define SCI_BRR     IrDA_SCBRR1
#define SCI_SCR     IrDA_SCSCR1
#define SCI_TDR     IrDA_SCFTDR1
#define SCI_SSR     IrDA_SCSSR1
#define SCI_RDR     IrDA_SCFRDR1
#define SCIF_SCFRDR IrDA_SCFRDR1
#define SCIF_SCFCR  IrDA_SCFCR1
#define SCIF_SCFDR  IrDA_SCFDR1
#undef  INT_ERI
#undef  INT_RXI
#undef  INT_TXI
#undef  INT_TEI
#define INT_ERI     INT_ERI1
#define INT_RXI     INT_RXI1
#define INT_TXI     INT_TXI1
#define INT_BRI     INT_BRI1
#define SCP_TXD     SCP2DT
#define SCP_RXD     SCP2DT
#define SCP_RTS     SCP3DT
#define SCPnMD0     SCP2MD0
#define SCPnMD1     SCP2MD1
#endif

#ifdef CH2
#define SCI_SMR     SCIF_SCSMR2
#define SCI_BRR     SCIF_SCBRR2
#define SCI_SCR     SCIF_SCSCR2
#define SCI_TDR     SCIF_SCFTDR2
#define SCI_SSR     SCIF_SCSSR2
#define SCI_RDR     SCIF_SCFRDR2
#define SCIF_SCFRDR SCIF_SCFRDR2
#define SCIF_SCFCR  SCIF_SCFCR2
#define SCIF_SCFDR  SCIF_SCFDR2
#undef  INT_ERI
#undef  INT_RXI
#undef  INT_TXI
#undef  INT_TEI
#define INT_ERI     INT_ERI2
#define INT_RXI     INT_RXI2
#define INT_TXI     INT_TXI2
#define INT_BRI     INT_BRI2
#define SCP_TXD     SCP4DT
#define SCP_RXD     SCP4DT
#define SCP_RTS     SCP6DT
#define SCP_CTS     SCP7DT
#define SCPnMD0     SCP4MD0
#define SCPnMD1     SCP4MD1
#endif

/* 割込みハンドラの再定義 */

#ifdef CH
#if (CH==0)
#define int_sio_rx  int_sio0_rx
#elif (CH==1)
#define int_sio_rx  int_sio1_rx
#elif (CH==2)
#define int_sio_rx  int_sio2_rx
#elif (CH==3)
#define int_sio_rx  int_sio3_rx
#endif

#if (CH==0)
#define int_sio_tx  int_sio0_tx
#elif (CH==1)
#define int_sio_tx  int_sio1_tx
#elif (CH==2)
#define int_sio_tx  int_sio2_tx
#elif (CH==3)
#define int_sio_tx  int_sio3_tx
#endif

#if (CH==0)
#define int_sio_te  int_sio0_te
#elif (CH==1)
#define int_sio_te  int_sio1_te
#elif (CH==2)
#define int_sio_te  int_sio2_te
#elif (CH==3)
#define int_sio_te  int_sio3_te
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

/* シリアル入出力制御ブロック */

extern T_SIO SIO[];

/* 内部変数 */

static UB txbuf[TXBUFSZ];       /* 送信バッファ */
static UB rxbuf[BUFSZ*2];       /* 受信バッファ, 受信ステータスバッファ */

#if (defined(INT_IRQ)||defined(CH2))
/*****************************************************************************
* RTS 信号 ON/OFF（内部関数）
*
******************************************************************************/

static void rts_on(void)
{
    PSW psw;

    psw = vdis_psw();
    sfr_clr(PORT_SCPDR, SCP_RTS);
    vset_psw(psw);
}

static void rts_off(void)
{
    PSW psw;

    psw = vdis_psw();
    sfr_set(PORT_SCPDR, SCP_RTS);
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

    /* ステータス入力 */

  #ifdef CH0
    ssr = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);
    ssr |= SIO[CH].rxsts;               /* 前回受信時のステータスとOR */
    if (sfr_in(PORT_SCPDR) & SCP_RXD)   /* ブレークOFF検出, RxD 端子"1" ? */
        ssr &= ~SSR_MPBT;               /* ブレーク検出は MPBT に割り当て */

    /* ステータス入力 */

  #else
    ssr = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER|SSR_RDF);
    ssr |= (SIO[CH].rxsts & ~SSR_BRK);  /* 受信時のステータスとOR */
  #endif

    /* ステータスビット編集 */

    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD 常時ON */

    if (ssr & SSR_TEND)                 /* 送信空 */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)                  /* パリティエラー */
        stat |= TSIO_PE;
  #ifdef CH0
    if (ssr & SSR_ORER)                 /* オーバランエラー */
  #else
    if (ssr & SSR_RDF)                  /* 受信FIFOフルはオーバランと同等 */
  #endif
        stat |= TSIO_OE;
    if (ssr & SSR_FER)                  /* フレーミングエラー */
        stat |= TSIO_FE;
  #ifdef CH0
    if (ssr & SSR_MPBT)                 /* ブレーク検出 */
  #else
    if (ssr & SSR_BRK)
  #endif
        stat |= TSIO_BD;

  #ifdef CH2
    if (sfr_in(PORT_SCPDR) & SCP_CTS)   /* /CTS 入力 */
        stat &= ~TSIO_CTS;
  #else
  #ifdef INT_IRQ
    if (sfr_in(PORT_PHDR) & PH_CTS)     /* /CTS 入力 */
        stat &= ~TSIO_CTS;
  #endif
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
  #ifndef NOFLW
    PSW psw;
  #endif

    /* RTS OFF */

  #if (defined(INT_IRQ)||defined(CH2))
    psw = vdis_psw();
    sfr_set(PORT_SCPDR, SCP_RTS);
  #ifdef CH0
    sfr_setw(PORT_SCPCR, SCP1MD0);
    sfr_clrw(PORT_SCPCR, SCP1MD1);
  #endif
  #ifdef CH1
    sfr_setw(PORT_SCPCR, SCP3MD0);
    sfr_clrw(PORT_SCPCR, SCP3MD1);
  #endif
  #ifdef CH2
    if (!(SIO[CH].flag & TSF_RTS))
    {   sfr_setw(PORT_SCPCR, SCP6MD0);
        sfr_clrw(PORT_SCPCR, SCP6MD1);
    }
  #endif
    vset_psw(psw);
  #endif

    /* リセット */

    sfr_out(SCI_SCR, 0x00);

    /* SCIF では FIFO 設定 */

  #ifndef CH0
    sfr_out(SCIF_SCFCR, 0x30);          /* 受信トリガ1, 送信トリガ1 */
  #endif
  #ifdef CH2
    if (SIO[CH].flag & TSF_RTS)
        sfr_set(SCIF_SCFCR, 0x08);      /* MCE = 1: モデム信号を許可 */
  #endif

    /* ボーレート設定値計算 */

    baud <<= 4; /* 64×2^(-1)×B の半分 */
    for (cks = 0; cks < 4; cks++, baud <<= 2)
    {   brr = ((CLK) / baud + 1) / 2; /* 四捨五入 */
        if (brr <= 0xffL)
            break;
    }
    if (cks == 4)
        return FALSE;

    /* モード／ボーレート設定 */

    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* ボーレートの1ビット期間経過待ち */
    tslp_tsk(2);

    /* フロー制御ある場合は、同割込みを初期化 */

  #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
    {   psw = vdis_psw();
      #if (IRQ <= 3)
        sfr_clrw(INTC_ICR1, 0x4000);        /* IRQLVL=0:IRQ3～0 独立 */
      #endif
        sfr_setw(INTC_ICR1, 2 << (IRQ * 2));/* IRQ Lowレベルセンス */
        vset_psw(psw);
    }
  #endif

    return TRUE;
}

#ifdef INT_IRQ
/*****************************************************************************
* CTS 割込み禁止（内部関数）
*
******************************************************************************/

static void dis_int_cts(void)
{
    PSW psw;

    if (SIO[CH].cmd[1] == 0)
        return;

    psw = vdis_psw();
  #if (IRQ < 4)
    sfr_clrw(INTC_IPRC, 0x000f << (IRQ * 4));
  #else
    sfr_clrw(INTC_IPRD, 0x000f << ((IRQ - 4) * 4));
  #endif
    SIO[CH].cmd[1] = 0;
    vset_psw(psw);
}

#endif
#ifdef INT_IRQ
/*****************************************************************************
* CTS 割込み許可（内部関数）
*
******************************************************************************/

static void ena_int_cts(void)
{
    PSW psw;

    if (SIO[CH].cmd[1] != 0)
        return;

    psw = vdis_psw();
  #if (IRQ < 4)
    sfr_setw(INTC_IPRC, IP << (IRQ * 4));
  #else
    sfr_setw(INTC_IPRD, IP << ((IRQ - 4) * 4));
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
    PSW psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_clrw(INTC_IPRB, 0x00f0);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRE, 0x0f00);
  #endif
  #ifdef CH2
    sfr_clrw(INTC_IPRE, 0x00f0);
  #endif
    vset_psw(psw);

  #ifdef INT_IRQ
    dis_int_cts();
  #endif
}

/*****************************************************************************
* シリアル割込み許可（内部関数）
*
******************************************************************************/

static void ena_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_setw(INTC_IPRB, IP << 4);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRE, IP << 8);
  #endif
  #ifdef CH2
    sfr_setw(INTC_IPRE, IP << 4);
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

  #if (defined(INT_IRQ)||defined(CH2))
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

  #ifdef CH0
    if ((SIO[CH].rxsts & SSR_MPBT) && (SIO[CH].oldsts & SSR_MPBT))
  #else
    if ((SIO[CH].rxsts & SSR_BRK) && (SIO[CH].oldsts & SSR_BRK))
  #endif
        return;

    /* バッファ満杯チェック */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
      #ifdef CH0
        SIO[CH].rxsts |= SSR_RDRF;      /* オーバフローは RDRF に割り当て */
      #else
        SIO[CH].rxsts |= SSR_TEND;      /* オーバフローは TEND に割り当て */
      #endif

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

  #if (defined(INT_IRQ)||defined(CH2))
    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_RTS))
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

  #if (defined(INT_IRQ)||defined(CH2))
    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_RTS))
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

    /* 受信ステータスと受信文字を入力 */
L1:
    chr = sfr_in(SCI_RDR);
  #ifdef CH0
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);
  #else
    sts = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER|SSR_RDF);
  #endif

    /* 受信割込みをクリア */

  #ifdef CH0
    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));
  #else
    sfr_outw(SCI_SSR, (UH)~(SSR_ER|SSR_BRK|SSR_RDF|SSR_DR));
  #endif

    /* ブレーク検出 */

  #ifdef CH0
    if ((sts & SSR_FER)                 /* フレーミングエラーで */
     && !(sfr_in(PORT_SCPDR) & SCP_RXD))/* RxD 端子"0" */
      sts |= SSR_MPBT;                  /* ブレーク検出は MPBT に割り当て */
  #endif

    SIO[CH].oldsts = SIO[CH].rxsts;     /* 前回の受信ステータス記憶 */
  #ifdef CH0
    SIO[CH].rxsts = (UB)sts;
  #else
    SIO[CH].rxsts = (UB)sts & ~SSR_RDF;
  #endif
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
    {   SIO[CH].rxtid = 0;
        iwup_tsk((ID)tid);
    }

  #ifndef CH0
    if (sfr_inw(SCI_SSR) & (SSR_RDF|SSR_DR))
        goto L1;
  #endif
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
  #ifndef CH0
    int cnt;
  #endif

    /* CTS 割込み禁止 */

  #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        dis_int_cts();
  #endif

    /* 送信ディセーブル中なら送信しない */

    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;

    /* RTSフロー制御ありなら CTS チェック */
    /* CTS OFF なら CTS 割込み許可 */

  #ifdef INT_IRQ
    if ((SIO[CH].flag & TSF_RTS) && (sfr_in(PORT_PHDR) & PH_CTS))
    {   ena_int_cts();
        goto L2;
    }
  #endif

    /* 送信レディでなければ終わり */

  #ifdef CH0
    if (!(sfr_in(SCI_SSR) & SSR_TDRE))
        return;
  #else
    if (!(sfr_inw(SCI_SSR) & SSR_TDFE))
        return;
    cnt = 15;
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
      #ifdef CH0
        sfr_clr(SCI_SSR, SSR_TDRE);
        return;
      #else
        cnt--;
      #endif
    }

    /* XOFF受信していたら送信不可 */

    if (SIO[CH].flag & TSF_RXOFF)
        goto L2;
  #endif

    /* 送信バッファ内文字数 - 1 */
    /* 送信バッファ空なら送信不可 */
L1:
    if (--SIO[CH].txcnt == (UH)-1)
    {
        SIO[CH].txcnt = 0;
      #ifdef CH0
        sfr_set(SCI_SCR, SCR_TEIE);     /* 送信終了割込み許可 */
      #else
        if (SIO[CH].tetid != 0)         /* SCIFではここで */
            iwup_tsk(SIO[CH].tetid);    /* 送信終了待ち解除 */
      #endif
L2:
      #ifndef CH0
        sfr_clrw(SCI_SSR, SSR_TDFE);
      #endif
        sfr_clr(SCI_SCR, SCR_TIE);      /* 送信割込み禁止 */
        return;
    }

    /* １文字送信 */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);
  #ifdef CH0
    sfr_clr(SCI_SSR, SSR_TDRE);
  #endif

    /* 取得ポインタを１つ進める */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* 送信待ち解除 */

    if ((tid = SIO[CH].txtid) != 0)
    {   SIO[CH].txtid = 0;
        iwup_tsk((ID)tid);
    }

  #ifndef CH0
    if (--cnt != 0)
        goto L1;
    sfr_clrw(SCI_SSR, SSR_TDFE);
  #endif
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

#ifdef CH0
/*****************************************************************************
* 割込みハンドラ送信終了処理（内部関数）
*
******************************************************************************/

static void te_int(void)
{
    /* 送信終了待ち解除 */

    if (SIO[CH].tetid != 0)
        iwup_tsk(SIO[CH].tetid);

    /* 送信終了割込み禁止 */

    sfr_clr(SCI_SSR, SSR_TEND);
    sfr_clr(SCI_SCR, SCR_TEIE);
}

#endif
#ifdef CH0
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

#endif
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

#ifdef V4
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio_rx, IP };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio_tx, IP };
#ifdef CH0
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio_te, IP };
#endif
#else
#ifdef V3
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio_rx, IP };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio_tx, IP };
#ifdef CH0
static const T_DINT dint_sio_te = { TA_HLNG, int_sio_te, IP };
#endif
#endif
#endif

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

    if (!(SIO[CH].flag & TSF_INIT))
    {
        SIO[CH].flag |= TSF_INIT;
      #ifdef V4
        def_inh(INT_ERI, &dinh_sio_rx);
        def_inh(INT_RXI, &dinh_sio_rx);
        def_inh(INT_TXI, &dinh_sio_tx);
       #ifdef CH0
        def_inh(INT_TEI, &dinh_sio_te);
       #else
        def_inh(INT_BRI, &dinh_sio_rx);
       #endif
       #ifdef INT_IRQ
        if (SIO[CH].flag & TSF_RTS)
            def_inh(INT_IRQ, &dinh_sio_tx);
       #endif
      #else
      #ifdef V3
        def_int(INT_ERI, &dint_sio_rx);
        def_int(INT_RXI, &dint_sio_rx);
        def_int(INT_TXI, &dint_sio_tx);
       #ifdef CH0
        def_int(INT_TEI, &dint_sio_te);
       #else
        def_int(INT_BRI, &dint_sio_rx);
       #endif
       #ifdef INT_IRQ
        if (SIO[CH].flag & TSF_RTS)
            def_int(INT_IRQ, &dint_sio_tx);
       #endif
      #else
        def_int(INT_ERI, int_sio_rx);
        def_int(INT_RXI, int_sio_rx);
        def_int(INT_TXI, int_sio_tx);
       #ifdef CH0
        def_int(INT_TEI, int_sio_te);
       #else
        def_int(INT_BRI, int_sio_rx);
       #endif
       #ifdef INT_IRQ
        if (SIO[CH].flag & TSF_RTS)
            def_int(INT_IRQ, int_sio_tx);
       #endif
      #endif
      #endif
    }

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

  #if (defined(INT_IRQ)||defined(CH2))
    rts_off();                          /* RTS OFF */
  #endif

    /* 割込み定義を取り消し */

  #ifdef V4 /* NORTI4 */
    def_inh(INT_ERI, (T_DINH *)NADR);
    def_inh(INT_RXI, (T_DINH *)NADR);
    def_inh(INT_TXI, (T_DINH *)NADR);
   #ifdef CH0
    def_inh(INT_TEI, (T_DINH *)NADR);
   #else
    def_inh(INT_BRI, (T_DINH *)NADR);
   #endif
   #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, (T_DINH *)NADR);
   #endif

  #else
  #ifdef V3 /* NORTI3 */
    def_int(INT_ERI, (T_DINT *)NADR);
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
   #ifdef CH0
    def_int(INT_TEI, (T_DINT *)NADR);
   #else
    def_int(INT_BRI, (T_DINT *)NADR);
   #endif
   #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (T_DINT *)NADR);
   #endif

  #else /* NORTi2 */
    def_int(INT_ERI, (INTHDRP)NADR);
    def_int(INT_RXI, (INTHDRP)NADR);
    def_int(INT_TXI, (INTHDRP)NADR);
   #ifdef CH0
    def_int(INT_TEI, (INTHDRP)NADR);
   #else
    def_int(INT_BRI, (INTHDRP)NADR);
   #endif
   #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (INTHDRP)NADR);
   #endif
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

          #ifdef CH0
            if (sts & (SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT))
          #else
            if (sts & (SSR_TEND|SSR_RDF|SSR_FER|SSR_PER|SSR_BRK))
          #endif
            {
              #ifdef CH0
                if (sts & SSR_RDRF)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_ORER)
                    ercd = EV_SIOORN;
                else if (sts & SSR_MPBT)
                    ercd = EV_SIOBRK;
              #else
                if (sts & SSR_TEND)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_RDF)
                    ercd = EV_SIOORN;
                else if (sts & SSR_BRK)
                    ercd = EV_SIOBRK;
              #endif
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
        ercd = wai_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* 複数回 iwup_tsk された場合の対策 */
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
        ercd = wai_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* 複数回 iwup_tsk された場合の対策 */
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
    PSW psw;

    chg_ims(IP);

    /* バッファクリア */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* ブレーク送信 */

    if (fncd & TSIO_SBON)
    {   fncd |= TSIO_TXD;
        psw = vdis_psw();
        sfr_clr(PORT_SCPDR, SCP_TXD);
        sfr_setw(PORT_SCPCR, SCPnMD0);
        vset_psw(psw);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        psw = vdis_psw();
        sfr_set(PORT_SCPDR, SCP_TXD);
        sfr_clrw(PORT_SCPCR, SCPnMD0);
        vset_psw(psw);
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

  #if (defined(INT_IRQ)||defined(CH2))
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
        {
          #ifdef CH0
            if (sfr_in(SCI_SSR) & SSR_TEND)
          #else
            if (sfr_inw(SCI_SSR) & SSR_TEND)
          #endif
            {   chg_ims(0);
                return E_OK;            /* 正常終了 */
            }
        }

        /* 送信終了割込み待ち */
      #ifdef CH0
        SIO[CH].tetid = vget_tid();
        chg_ims(0);
        ercd = wai_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* 複数回 iwup_tsk された場合の対策 */
        if (ercd)
            return ercd;                /* タイムアウト終了 */
      #else
        chg_ims(0);
        if (tmout == 0)
            return E_TMOUT;
        wai_tsk(1);
        if (tmout != TMO_FEVR)
            tmout--;
      #endif
    }
}

/* end */
