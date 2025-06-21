/****************************************************************************
* シルアル入出力ドライバ（SH7615）                                          *
*                                                                           *
*  File name : nos7615.c                                                    *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                 *
*                                                                           *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=? nos7615.c        (Renesas C)      *
*            gcc -c -m2 -O2 -DCLK=? nos7615.c              (exeGCC)         *
*            ccsh -c -cpu=sh_ii -ansi -O -DCLK=? nos7615.c (GHS)            *
*                                                                           *
* 17/Apr/2000 作成                                                    MiSPO *
* 30/Oct/2000 IAR EW 対応                                                   *
* 12/Sep/2001 制御系の判断を見直し                                       TA *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え   KA *
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

          CH0, CH1 マクロで、物理チャネル番号を変更できます。
          CH0 を定義すると、FIFO 付き SCIF1 を使うようにコンパイルされます。
          CH1 を定義すると、FIFO 付き SCIF2 を使うようにコンパイルされます。

          チャネル0 とチャネル1 の両方をリンクする場合は、重複しないモジュー
          ル名となるようコンパイルしてください。

          (例) shc <options> nos7615.c  ･･･････････････････････ チャネル0
               shc <options> -def=CH=1 -ob=nos7ch1.o nos7615.c  チャネル1

          (例) gcc <options> nos7615.c  ･･･････････････････････ チャネル0
               gcc <options> -DCH=1 -o nos7ch1.o nos7615.c ････ チャネル1

          (例) ccsh <options> nos7615.c  ･･････････････････････ チャネル0
               ccsh <options> -DCH=1 -o nos7ch1.o nos7615.c ･･･ チャネル1

ボーレートジェネレータ

          ソースクロック(Pφ)のデフォルトは 15 MHz です。変更する場合 は
          CLK マクロを定義してコンパイルしてください。値は [Hz] 単位の周
          波数です。

          (例) shc <options> -def=CLK=30000000 nos7615.c ･･････ 30 MHz
                                  ~~~~~~~~~~~~
          (例) gcc <options> -DCLK=30000000 nos7615.c
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

ハードウェアフロー制御

          ハードフローを用いない場合は、NOFLW マクロ定義してコンパイルしてく
          ださい。コードサイズと処理速度を節約できます。

          CH0では、SCIF 内蔵の RTS-CTS 機能を使い、ソフトウェアに依らず行い
          ます。

          ※本バージョンではCH1のフロー制御は未確認です。

               |       標 準 割 付
          -----+---------------------------
               |RTS   RTS/PB12(154pin)
               |CTS   CTS/PB11(156pin)
          CH0  |DTR --- Not Supported
               |DSR --- Not Supported
               |DCD --- Not Supported
          -----+---------------------------
               |RTS --- Not Supported
               |CTS --- Not Supported
          CH1  |DTR --- Not Supported
               |DSR --- Not Supported
               |DCD --- Not Supported
          -----+---------------------------

          ※SCIFハードウェアフロー制御に対する補足
            SCIFのハードウェアフロー制御はFIFOバッファからの読み出しが完了して
            いるかどうかにより行われます。
            通信速度が早いとき、一時的に取込を止めないと、受信バッファの方が
            オーバーフローしてしまいます。そのため、受信バッファの空きが少なく
            なった時点で、取込そのものを停止します（受信割込禁止）。
            こうすると、FIFOから読み出さないのでRTS ONのままで保持されます。
            その後受信バッファからの読み出しが進んで空きが増えたとき、割込を
            許可します。受信が再開され、FIFOから読み出しが完了すると、RTSが
            自動的にOFFとなります。

割込み優先順位

          本ハンドラの、デフォルトの割込み優先順位は、7 です。
          これを変更する場合、IP マクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。（IP ≦ KNL_LEVEL）
          本ハンドラより高優先度の割込みハンドラが有る場合、多重割込みが起こ
          ります。

          (例) shc <options> -def=IP=5 nos7615.c ･･･ 割込み優先順位 5
                                  ~~~~
          (例) gcc <options> -DIP=5 nos7615.c
                               ~~~~
ポートの初期化

          SCIF と兼用になっているポートB は、外部で、次の例の様に初期化してく
          ださい。（下記の例では、PB の他の端子は入力と仮定）

          #include "nos7615.h"
          sfr_setw(PFC_PBCR, PBCR_RXD1); ････ RxD1 機能
          sfr_setw(PFC_PBCR, PBCR_TXD1); ････ TxD1 機能
          sfr_setw(PFC_PBCR, PBCR_RTS1); ････ ~RTS 機能
          sfr_setw(PFC_PBCR, PBCR_CTS1); ････ ~CTS 機能
          sfr_setw(PFC_PBCR2, PBCR_RXD2); ･･･ RxD2 機能
          sfr_setw(PFC_PBCR2, PBCR_TXD2); ･･･ TxD2 機能

未サポート機能

          調歩同期式モードのみで、クロック同期式モードはサポートしていません。
          内蔵ボーレートジェネレータのみで、外部クロック入出力はサポートして
          いません。
          DTR 信号出力、DSR 信号入力、CD 信号入力はサポートしていません。
          SH7615 特有のマルチプロセッサモードはサポートしていません。

動作未確認

          CH1は、動作未確認です。

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
 #define V4     1
 #endif
 #include "kernel.h"
#endif

#include "nosio.h"
#include "sh7615.h"
#include "nos7615.h"

/* 割込み優先順位の定義 */

#ifndef IP
#define IP          7           /* IP 未定義なら 7 とする */
#endif

/* クロックの定義 */

#define MHz         1000000L
#ifndef CLK
#define CLK         15*MHz      /* CPUクロック[Hz] */
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
#define SCI_SMR     SCIF_SCSMR1    /* シリアルモードレジスタ */
#define SCI_BRR     SCIF_SCBRR1    /* ビットレートレジスタ */
#define SCI_SCR     SCIF_SCSCR1    /* シリアルコントロールレジスタ */
#define SCI_TDR     SCIF_SCFTDR1   /* トランスミットFIFOデータレジスタ */
#define SCI_SSR     SCIF_SC1SSR1   /* シリアルステータス1レジスタ */
#define SCI_SSR2    SCIF_SC2SSR1   /* シリアルステータス2レジスタ */
#define SCI_RDR     SCIF_SCFRDR1   /* レシーブFIFOデータレジスタ */
#define SCI_FCR     SCIF_SCFCR1    /* FIFOコントロールレジスタ */
#define SCI_FDR     SCIF_SCFDR1    /* FIFOデータ数レジスタ */
#define SCI_FER     SCIF_SCFER1    /* FIFOエラーレジスタ */
#define PB_TXD      PB_TXD1        /* TxD 出力端子 */
#define PB_RXD      PB_RXD1        /* RxD 入力端子 */
#define PB_CTS      PB_CTS1        /* CTS 入力端子 */
#define INT_ERI     INT_ERI1       /* 受信エラー割込みベクタ番号 */
#define INT_RXI     INT_RXI1       /* 受信完了割込みベクタ番号 */
#define INT_TXI     INT_TXI1       /* 送信データエンプティ割込みベクタ番号 */
#define INT_BRI     INT_BRI1       /* ブレーク検出ベクタ番号 */
#endif

#ifdef CH1
#define SCI_SMR     SCIF_SCSMR2    /* シリアルモードレジスタ */
#define SCI_BRR     SCIF_SCBRR2    /* ビットレートレジスタ */
#define SCI_SCR     SCIF_SCSCR2    /* シリアルコントロールレジスタ */
#define SCI_TDR     SCIF_SCFTDR2   /* トランスミットFIFOデータレジスタ */
#define SCI_SSR     SCIF_SC1SSR2   /* シリアルステータス1レジスタ */
#define SCI_SSR2    SCIF_SC2SSR2   /* シリアルステータス2レジスタ */
#define SCI_RDR     SCIF_SCFRDR2   /* レシーブFIFOデータレジスタ */
#define SCI_FCR     SCIF_SCFCR2    /* FIFOコントロールレジスタ */
#define SCI_FDR     SCIF_SCFDR2    /* FIFOデータ数レジスタ */
#define SCI_FER     SCIF_SCFER2    /* FIFOエラーレジスタ */
#define PB_TXD      PB_TXD2        /* TxD 出力端子 */
#define PB_RXD      PB_RXD2        /* RxD 入力端子 */
#define INT_ERI     INT_ERI2       /* 受信エラー割込みベクタ番号 */
#define INT_RXI     INT_RXI2       /* 受信完了割込みベクタ番号 */
#define INT_TXI     INT_TXI2       /* 送信データエンプティ割込みベクタ番号 */
#define INT_BRI     INT_BRI2       /* ブレーク検出ベクタ番号 */
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
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio0_tx };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio1_tx };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio2_tx };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio3_tx };
#endif
#else
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio0_tx };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio1_tx };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio2_tx };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio3_tx };
#endif
#endif

#if defined(__HITACHI__)
#if (CH==0)
#pragma interrupt(int_sio0_rx)
#pragma interrupt(int_sio0_tx)
#elif (CH==1)
#pragma interrupt(int_sio1_rx)
#pragma interrupt(int_sio1_tx)
#elif (CH==2)
#pragma interrupt(int_sio2_rx)
#pragma interrupt(int_sio2_tx)
#elif (CH==3)
#pragma interrupt(int_sio3_rx)
#pragma interrupt(int_sio3_tx)
#endif
#endif

#ifndef NOFLW
/*****************************************************************************
* RTS 信号 ON/OFF（内部関数）
*
******************************************************************************/

static void rts_on(void)
{
  #ifdef CH1
    UINT psw;

    psw = vdis_psw();       /* 割り込み禁止 */

    /* RTS ONするコードをここへ     */

    vset_psw(psw);          /* 割り込み禁止復元 */
  #endif
}

static void rts_off(void)
{
  #ifdef CH1
    UINT psw;

    psw = vdis_psw();

    /* RTS OFFするコードをここへ    */

    vset_psw(psw);
  #endif
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

    ssr = sfr_inw(SCI_SSR) & (SSR_FER|SSR_PER|SSR_BRK);
    if (sfr_in(SCI_SSR2) & (SSR2_ORER))  /* オーバランエラー検出 */
        ssr |= SSR_DR;                   /* SSR_DRにSSR_ORERを割り当て */

    /* 受信時のステータスとOR */

    ssr |= (SIO[CH].rxsts & ~SSR_BRK);

    /* ステータスビット編集 */

    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD 常時ON */

    if (ssr & SSR_TEND)             /* 送信空 */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)              /* パリティエラー */
        stat |= TSIO_PE;
    if (ssr & SSR_DR /* as SSR2_ORER */)  /* オーバランエラー */
        stat |= TSIO_OE;
    if (ssr & SSR_FER)              /* フレーミングエラー */
        stat |= TSIO_FE;
    if (ssr & SSR_BRK)              /* ブレーク検出 */
        stat |= TSIO_BD;
  #ifndef NOFLW
   #ifdef CH0
    if (sfr_inw(IO_PBDR) & PB_CTS1)   /* ~CTS 入力 */
        stat &= ~TSIO_CTS;
   #else
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
    UINT psw;
    volatile int i;

    /* RTS OFF */

  #ifndef NOFLW
    rts_off();
  #endif

    /* リセット */

    sfr_out(SCI_SCR, 0x00);                 /* Tx/Rx disable */
    sfr_out(SCI_FCR, FCR_RFRST|FCR_TFRST);  /* TxFIFO/RxFIFO reset */

    /* ボーレート設定値計算 */

    cks = 0;
    for (brr = (CLK) / (baud * 32); brr > 0x100L; brr >>= 2)
    {   if (++cks > 3)
            return FALSE;
    }

    /* モード／ボーレート設定 */

    sfr_out(SCI_SSR2, SSR2_N1);  /* sampling = x16 */
    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));
    for ( i=0; i<1084; i++ ) ;  /* wait 1bit period for 9600bps / Iφ=60MHz */
                         /* 104[usec] / (1/60[MHz]) / 6[instruction] = 1084 */

    /* ボーレートの1ビット期間経過待ち */
    tslp_tsk(2);

    /* フロー制御ある場合は、同割込みを初期化 */

  #ifndef NOFLW
   #ifdef CH0
   #else
    if (SIO[CH].flag & TSF_RTS)
    {   psw = vdis_psw();

        /* 必要な処理をここに記述します。 */

        vset_psw(psw);
    }
   #endif
  #endif

    /* FIFO 設定 */

    sfr_out(SCI_FCR, 0x30);         /* 受信トリガ1, 送信トリガ1 */
    if (SIO[CH].flag & TSF_RTS)
        sfr_set(SCI_FCR, FCR_MCE);  /* MCE = 1: モデム信号を許可 */

    /* 割り込みコントローラのベクタNo.を設定する */

    psw = vdis_psw();
#ifdef CH0
    sfr_outw(INTC_VCRL, sfr_inw(INTC_VCRL) | (INT_ERI << 8)); /* ERI */
    sfr_outw(INTC_VCRL, sfr_inw(INTC_VCRL) | (INT_RXI << 0)); /* RXI */
    sfr_outw(INTC_VCRM, sfr_inw(INTC_VCRM) | (INT_TXI << 0)); /* TXI */
    sfr_outw(INTC_VCRM, sfr_inw(INTC_VCRM) | (INT_BRI << 8)); /* BRI */
#else
    sfr_outw(INTC_VCRN, sfr_inw(INTC_VCRN) | (INT_ERI << 8)); /* ERI */
    sfr_outw(INTC_VCRN, sfr_inw(INTC_VCRN) | (INT_RXI << 0)); /* RXI */
    sfr_outw(INTC_VCRO, sfr_inw(INTC_VCRO) | (INT_TXI << 0)); /* TXI */
    sfr_outw(INTC_VCRO, sfr_inw(INTC_VCRO) | (INT_BRI << 8)); /* BRI */
#endif
    vset_psw(psw);

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

    /* ＣＴＳ割込みをマスクするコードを記述すること */

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

    /* ＣＴＳ割込みマスクを解除するコードを記述すること */

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
    sfr_clrw(INTC_IPRD, 0x000f);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRE, 0xf000);
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
    sfr_setw(INTC_IPRD, IP << 0);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRE, IP << 12);
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

    if (SIO[CH].txcnt >= TXBUFSZ)
    {
        return FALSE;
    }

    /* バッファへ格納 */

    p = SIO[CH].txputp;
    *p = c;

    /* 格納ポインタを１つ進める */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txputp = p;
    SIO[CH].txcnt++;

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

    if ((SIO[CH].rxsts & SSR_BRK) && (SIO[CH].oldsts & SSR_BRK))
        return;

    /* バッファ満杯チェック */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
        SIO[CH].rxsts |= SSR_TEND;      /* オーバフローは TEND に割り当て */

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
   #ifdef CH0
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
        sfr_clr(SCI_SCR, SCR_RIE);      /* 受信割り込み禁止 */
   #else
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_off();
   #endif
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
    UINT psw;

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
   #ifdef CH0
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
    {   psw = vdis_psw();
        sfr_set(SCI_SCR, SCR_RIE);      /* 受信割り込み許可 */
        vset_psw(psw);
    }
   #else
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_on();
   #endif
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
L1:
    chr = sfr_in(SCI_RDR);
    sts = sfr_inw(SCI_SSR) & (SSR_FER|SSR_PER|SSR_BRK);

    /* 受信割込みをクリア */

    sfr_outw(SCI_SSR, (UB)~(SSR_ER|SSR_BRK|SSR_FER|SSR_PER|SSR_RDF|SSR_DR));
    if (sfr_in(SCI_SSR2) & (SSR2_ORER)){  /* オーバランエラー検出 */
        sts |= SSR_DR;                    /* SSR_DRにSSR_ORERを割り当て */
        sfr_clr(SCI_SSR2, SSR2_ORER);
    }

    /* ブレーク検出 */

    if ((sts & SSR_FER)                 /* フレーミングエラーで */
     && !(sfr_inw(IO_PBDR) & PB_RXD1))  /* RxD 端子"0" */
        sts |= SSR_BRK;

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

    if (sfr_inw(SCI_SSR) & (SSR_ER|SSR_BRK|SSR_RDF|SSR_DR))
        goto L1;
    if (sfr_in(SCI_SSR2) & SSR2_ORER )
        goto L1;
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
    int cnt;

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
   #ifdef CH0
    if ((SIO[CH].flag & TSF_RTS) && (sfr_inw(IO_PBDR) & PB_CTS1))
   #else
    if ((SIO[CH].flag & TSF_RTS))
   #endif
    {   ena_int_cts();
        goto L2;
    }
  #endif

    /* 送信レディでなければ終わり */

    if (!(sfr_inw(SCI_SSR) & SSR_TDFE))
        return;
    cnt = 15;

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
        cnt--;
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

        if (SIO[CH].tetid != 0)        /* SCIFではここで       */
            wup_tsk(SIO[CH].tetid);    /* 送信終了待ち解除     */

        /* 送信割込み禁止 */
L2:
        sfr_clrw(SCI_SSR, SSR_TDFE);
        sfr_clr(SCI_SCR, SCR_TIE);
        return;
    }

    /* １文字送信 */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);

    /* 取得ポインタを１つ進める */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* 送信待ち解除 */

    if ((tid = SIO[CH].txtid) != 0)
        wup_tsk((ID)tid);

    if (--cnt != 0 && (sfr_inw(SCI_SSR) & SSR_TDFE))
        goto L1;
    sfr_clrw(SCI_SSR, SSR_TDFE);
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
* 初期化パラメータ解析（内部関数）
*
******************************************************************************/

static BOOL set_param(const B *s, UB *mode, UW *baud)
{
    B c;
    UW b;

    /* モード判別 */

    *mode = 0;
    if      (strstr((char *)s, (char *)"PE") != NULL)   *mode |= SMR_PE;
    else if (strstr((char *)s, (char *)"PO") != NULL)   *mode |= SMR_PE|SMR_OE;
    if      (strstr((char *)s, (char *)"S2") != NULL)   *mode |= SMR_STOP;
    if      (strstr((char *)s, (char *)"B7") != NULL)   *mode |= SMR_CHR;

    /* フロー制御有無判別 */

    if (strstr((char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((char *)s, (char *)"RTS") != NULL)   SIO[CH].flag |= TSF_RTS;

    /* 終端文字指定判別 */

    if (strstr((char *)s, (char *)"ETX") != NULL)   SIO[CH].eot = 0x03;
    if (strstr((char *)s, (char *)"EOT") != NULL)   SIO[CH].eot = 0x04;
    if (strstr((char *)s, (char *)"CR" ) != NULL)   SIO[CH].eot = 0x0d;
    if (strstr((char *)s, (char *)"LF" ) != NULL)   SIO[CH].eot = 0x0a;

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
    def_inh(INT_BRI, &dinh_sio_rx);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_inh(INT_IRQ, &dinh_sio_tx);  */ /* */
    }
    #endif
   #endif
  #else
    def_int(INT_ERI, &dint_sio_rx);
    def_int(INT_RXI, &dint_sio_rx);
    def_int(INT_TXI, &dint_sio_tx);
    def_int(INT_BRI, &dint_sio_rx);

   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_int(INT_IRQ, &dint_sio_tx);  */ /* */
    }
    #endif
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
    def_inh(INT_BRI, (T_DINH *)NADR);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_inh(INT_IRQ, (T_DINH *)NADR);  */ /* */
    }
    #endif
   #endif
  #else
    def_int(INT_ERI, (T_DINT *)NADR);   /* 割込み定義を取り消し */
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
    def_int(INT_BRI, (T_DINT *)NADR);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_int(INT_IRQ, (T_DINT *)NADR);  */ /* */
    }
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

            if (sts & (SSR_TEND|SSR_RDF|SSR_FER|SSR_PER|SSR_BRK))
            {   if (sts & SSR_TEND)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_RDF)
                    ercd = EV_SIOORN;
                else if (sts & SSR_BRK)
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
    UINT psw;

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
#ifdef CH0
        sfr_clrw(PFC_PBCR, PBCR_TXD1_M);  /* to generic */
#else
        sfr_clrw(PFC_PBCR2, PBCR_TXD2_M); /* to generic */
#endif
        sfr_setw(PFC_PBIOR, PB_TXD);    /* at output */
        sfr_clrw(IO_PBDR, PB_TXD);      /* as low */
        vset_psw(psw);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        psw = vdis_psw();
#ifdef CH0
        sfr_setw(PFC_PBCR, PBCR_TXD1);  /* to TxD */
#else
        sfr_setw(PFC_PBCR2, PBCR_TXD2); /* to RxD */
#endif
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
        {
            chg_ims(0);
            return E_OK;            /* 正常終了 */
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
