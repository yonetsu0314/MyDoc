/******************************************************************************
* シルアル入出力ドライバ  MS7709, MS7709A and MS7750 with NS16550(TL16C552)   *
*                                                                             *
*  File name : nos16550.c                                                     *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*  Compile : shc -cp=sh3 -op=1 -def=CLK=? nos16550.c         (Renesas C)      *
*            gcc -c -m3 -O2 -DCLK=? nos16550.c               (exeGCC)         *
*            ccsh -c -cpu=sh_iii -ansi -O -DCLK=? nos16550.c (GHS)            *
*                                                                             *
* 19/Apr/2000                                                                 *
* 12/Sep/2001 制御系の判断を見直し                                         TA *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え     KA *
* 25/Feb/2003 "Green Hills" コンパイラの判断を __ghs でするように修正      KA *
* 25/Jun/2003 SMSC Super I/O FDC37C935A 対応コード追加                     KA *
******************************************************************************/

/*

最適化コンパイル

          割込みハンドラが含まれるので、必ず最適化オプションを付けてコンパイ
          ルしてください。

チャネル番号の指定

          CH マクロで、論理チャネル番号を指定できます。（無指定時は、CH=0）
          CH は 0～3 を指定できます。

          CH0, CH1 マクロで、物理チャネル番号を変更できます。
          CH0 を定義すると、TL16C552 の SCI0 を使うようにコンパイルされます。
          CH1 を定義すると、TL16C552 の SCI1 を使うようにコンパイルされます。

          チャネル0 とチャネル1 の両方をリンクする場合は、重複しないモジュー
          ル名となるようコンパイルしてください。

          (例) shc <options> nos16550.c ･････････････････････････ チャネル0
               copy nos16550.c nos16ch1.c
               shc <options> -def=CH=1 nos16ch1.c  ･･････････････ チャネル1

          (例) gcc <options> nos16550.c ･････････････････････････ チャネル0
               gcc <options> -DCH=1 -onos16ch1.o nos16550.c ･････ チャネル1

          (例) ccsh <options> nos16550.c  ･･･････････････････････ チャネル0
               ccsh <options> -DCH=1 -o nos16ch1.o nos16550.c ･･･ チャネル1

ボーレートジェネレータ
          CLKTYPEマクロでクロックジェネレータに対する処理方法を切り替えます。
          CLKTYPE=0と定義すると、通常の分周比計算を行います。
          このときのソースクロックのデフォルトは 14.7456 MHz です。
          変更する場合 は CLKマクロを定義してコンパイルしてください。
          16550互換チップ等がそのまま搭載されているとき使用します。

          CLKTYPE=1と定義すると、あるボーレート以上はボーレート毎に固定値を
          セットします。
          SMSC FDC37C93x（Ultra I/O）は内部で1.8462MHzがデフォルトのソース
          クロックです。このICは115.2K以上では、分周比としてセットした値に
          よって、内蔵のソースクロックそのものを切り替えます。

                 分周比         実ボーレート
                    1             115200
                32770             230400
                32769             460800

          CLKマクロの値は [Hz] 単位の周波数です。
          ※TL16C552Aは16MHz迄のクロックが使用可能です。

          (例) shc <options> -def=CLK=8000000 nos16550.c ･･････ 8 MHz
                                  ~~~~~~~~~~~~
          (例) gcc <options> -DCLK=8000000 nos16550.c
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

          NS16550/TL16C552/TL16C552A のチャネルに対応する CTS,RTS の端子に
          /CTS , /RTS 信号を接続してください。

          ハードフローを用いない場合は、NOFLW マクロ定義してコンパイルしてく
          ださい。コードサイズと処理速度を節約できます。

割込み優先順位

          本ハンドラの、デフォルトの割込み優先順位は、7 です。
          これを変更する場合、IP マクロを定義してコンパイルしてください。
          ただし、カーネルより高優先にはできません。（IP ≦ KNL_LEVEL）
          本ハンドラより高優先度の割込みハンドラが有る場合、多重割込みが起こ
          ります。

          (例) shc <options> -def=IP=5 nos16550.c ･･･ 割込み優先順位 5
                                  ~~~~
          (例) gcc <options> -DIP=5 nos16550.c
                               ~~~~
          MS7750SE では、CH0 は 8、CH1 は 12 に、それぞれ固定されています。

モデム信号サポート

          16550バージョンではモデム制御信号やフロー制御信号は標準で搭載されて
          いますので、本ハンドラでも標準サポートとしました。
          ただし、RI（Ring Indicator)信号については、従来版のNOSIOとの互換性
          から、未サポートとしました。

未サポート機能


*/

#include <stdlib.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif

#ifdef V2
 #include "norti.h"
 #define PSW         UW
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
  #else
   #define CH0             /* 無指定の場合, 論理チャネル3 の物理チャネルは0 */
  #endif
 #endif
#endif

#include "nos16550.h"

/* CHx に合わせた再定義 */

#ifdef CH0
 #define UART_RBR   UART0_RBR
 #define UART_THR   UART0_THR
 #define UART_DLL   UART0_DLL
 #define UART_IER   UART0_IER
 #define UART_DLM   UART0_DLM
 #define UART_IIR   UART0_IIR
 #define UART_FCR   UART0_FCR
 #define UART_LCR   UART0_LCR
 #define UART_MCR   UART0_MCR
 #define UART_LSR   UART0_LSR
 #define UART_MSR   UART0_MSR
 #define UART_SCR   UART0_SCR
 #define INT_UART   INT_UART0
 #define IP_UART    IP_UART0
#endif

#ifdef CH1
 #define UART_RBR   UART1_RBR
 #define UART_THR   UART1_THR
 #define UART_DLL   UART1_DLL
 #define UART_IER   UART1_IER
 #define UART_DLM   UART1_DLM
 #define UART_IIR   UART1_IIR
 #define UART_FCR   UART1_FCR
 #define UART_LCR   UART1_LCR
 #define UART_MCR   UART1_MCR
 #define UART_LSR   UART1_LSR
 #define UART_MSR   UART1_MSR
 #define UART_SCR   UART1_SCR
 #define INT_UART   INT_UART1
 #define IP_UART    IP_UART1
#endif

#define INT_SIO     INT_UART
#define SIO_DR      UART_RBR
#define SIO_IER     UART_IER
#define SIO_IIR     UART_IIR
#define SIO_FCR     UART_FCR
#define SIO_LCR     UART_LCR
#define SIO_MCR     UART_MCR
#define SIO_MSR     UART_MSR
#define SIO_LSR     UART_LSR

/* 割込み優先順位の定義 */

#ifndef IP
 #if defined(CPU_H8S)
 #if INTM==2
 #define IP          IP_UART
 #else
 #define IP          1
 #endif
 #else
 #define IP          IP_UART
 #endif
#endif

/* クロックの定義 */
#ifndef CLKTYPE
 #define CLKTYPE 0
#endif

#if CLKTYPE == 0                /* 正直に、外部クロックを入れるタイプ   */
 #ifndef CLK
  #define CLK         14745600  /* CLK入力周波数[Hz] */
 #endif
#else                           /* スーパーＩ/Ｏ（SMSC FDC37C93X）      */
 #ifndef CLK
  #define CLK          1846200  /* 内蔵クロック                         */
 #endif
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

#ifndef NOFLW
 #define INT_IRQ   (INT_UART)       /* CTS 割込みベクタ番号 */
#endif

/* 割込みハンドラの再定義 */

#ifdef CH
 #if (CH==0)
  #define int_sio  int_sio0
 #elif (CH==1)
  #define int_sio  int_sio1
 #elif (CH==2)
  #define int_sio  int_sio2
 #elif (CH==3)
  #define int_sio  int_sio3
 #endif
#endif

#if defined(__HITACHI__)
 #if (CH==0)
  #pragma interrupt(int_sio0)
 #elif (CH==1)
  #pragma interrupt(int_sio1)
 #elif (CH==2)
  #pragma interrupt(int_sio2)
 #elif (CH==3)
  #pragma interrupt(int_sio3)
 #endif
#endif

/* シリアル入出力制御ブロック */

extern T_SIO SIO[];

/* 内部変数 */

static UB txbuf[TXBUFSZ];       /* 送信バッファ */
static UB rxbuf[BUFSZ*2];       /* 受信バッファ, 受信ステータスバッファ */

/* 制御文字 */

#define XOFF        0x13        /* 送信停止（CTRL-S）*/
#define XON         0x11        /* 送信再開（CTRL-Q）*/

#ifndef NOFLW
/*****************************************************************************
* RTS 信号 ON/OFF（内部関数）
*
******************************************************************************/

static void rts_on(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_RTS);
    vset_psw(psw);
}

static void rts_off(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_RTS);
    vset_psw(psw);
}

#endif
/*****************************************************************************
* DTR 信号 ON/OFF（内部関数）
*
******************************************************************************/

static void dtr_on(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_DTR);
    vset_psw(psw);
}

static void dtr_off(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_DTR);
    vset_psw(psw);
}

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
******************************************************************************/

static int get_stat(void)
{
    int stat;
    int ssr;
    int m;

    /* 現在のモデムステータス入力 */

    m = sci_in( SIO_MSR );

    /* モデムステータスビット編集 */

    stat = 0;
    if (m & SIO_CTS)
        stat |= TSIO_CTS;
    if (m & SIO_DSR)
        stat |= TSIO_DSR;
    if (m & SIO_CD)
        stat |= TSIO_CD;

    /* 現在のラインステータス入力 */

    ssr = sci_in(SIO_LSR);

    /* 受信時のステータスのエラービットとOR */

    ssr |= (SIO[CH].rxsts & (SIO_FE|SIO_OE|SIO_PE));

    /* ラインステータスビット編集 */

    if (ssr & SIO_PE)         /* パリティエラー       */
        stat |= TSIO_PE;
    if (ssr & SIO_OE)         /* オーバーランエラー   */
        stat |= TSIO_OE;
    if (ssr & SIO_FE)         /* オーバーランエラー   */
        stat |= TSIO_FE;
    if (ssr & SIO_BD)         /* ブレーク検出         */
        stat |= TSIO_BD;
    if (ssr & SIO_TxEMP)      /* 送信バッファ空       */
        stat |= TSIO_TXEMP;

    return stat;
}

/*****************************************************************************
* SMSC Super I/O FDC37C935A
*   Super I/O のConfiguration 16550
******************************************************************************/
#ifdef FDC_CONF_PORT

#define  FDC_INDEX_PORT FDC_CONF_PORT
#define  FDC_DATA_PORT  FDC_CONF_PORT+2

static void  fdc_cfgw(unsigned short  index, unsigned short  data)
{
    port_outw(FDC_INDEX_PORT, index);
    port_outw(FDC_DATA_PORT, data);
}

static void fdc_com_cfg(void)
{
#ifdef CH0
    unsigned short add = 0x03f8;    /* UART0 = 0xB04007F0   */
#else
    unsigned short add = 0x02f8;    /* UART1 = 0xB04005F0   */
#endif

unsigned short low_add;
unsigned short high_add;

    low_add = ((add << 8) & 0xff00);
    high_add = (add & 0xff00);

    port_outw(FDC_CONF_PORT, 0x5500);   /* Entering the configuration state */
    port_outw(FDC_CONF_PORT, 0x5500);

#ifdef CH0
    fdc_cfgw(0x0700, 0x0400);
#else
    fdc_cfgw(0x0700, 0x0500);
#endif
    fdc_cfgw(0x3000, 0x0100);
    fdc_cfgw(0x6000, high_add);
    fdc_cfgw(0x6100, low_add);
#ifdef CH0
    fdc_cfgw(0x7000, 0x0400);   /* IRQ4 */
#else
    fdc_cfgw(0x7000, 0x0300);   /* IRQ3 */
#endif
    fdc_cfgw(0xF000, 0x0000);
    fdc_cfgw(0x0700, 0x0000);

    port_outw(FDC_CONF_PORT, 0xAA00);   /* Exiting the configuration state */
}
#endif  /* FDC_CONF_PORT */


/*****************************************************************************
* ＳＩＯデバイス初期化（内部関数）
*
******************************************************************************/

static void init_sio(UB mode, UW baud)
{
    UH tc;

#ifdef FDC_CONF_PORT
    fdc_com_cfg();
#endif

    /* RTS OFF */

#ifndef NOFLW
    rts_off();
#endif

    /* リセット & FIFO 設定(受信トリガ1) */

    sci_out(SIO_FCR, (B)(SIO_TRG08|SIO_DMA|SIO_TXFRST|SIO_RXFRST|SIO_FIFOEN));

    /* ラインコントロール･レジスタ設定 */

    sci_out(SIO_LCR, SIO[CH].cmd[1] = mode);

    /* モデムコントロール･レジスタ･クリア */

    sci_out(SIO_MCR, 0);

    /* ボーレート分周レジスタ設定 */
#if CLKTYPE == 1
    /*
     * FDC37C93X の様に設定した値で、自動的にクロックソースを
     * 変更するタイプは、値できめうちにでもするしかない。
     */
    if ( baud >= 115200 ){
        if ( baud < 230400 )
            tc = 1;
        else if ( baud < 460800 )
            tc = 32770;
        else
            tc = 32769;
        sci_out(SIO_LCR, SIO[CH].cmd[1] |= SIO_DLAB);   /* DLAB = 1     */
        sci_out(SIO_DR, tc);
        tc >>= 8;
        sci_out(SIO_IER, tc);
        sci_out(SIO_LCR, SIO[CH].cmd[1] &= ~SIO_DLAB);  /* DLAB = 0     */
        return;
    }
#endif
    sci_out(SIO_LCR, SIO[CH].cmd[1] |= SIO_DLAB);   /* DLAB = 1     */
    tc = (UH)(CLK / (baud*16));                     /* *16 Clock    */
    sci_out(SIO_DR, tc);
    tc >>= 8;
    sci_out(SIO_IER, tc);
    sci_out(SIO_LCR, SIO[CH].cmd[1] &= ~SIO_DLAB);  /* DLAB = 0     */
}

/*****************************************************************************
* ＳＩＯへ送信文字を出力（内部関数）
*
* 出力できた場合 TRUE、出力できなかった場合 FALSE を返す。
******************************************************************************/

static BOOL tx_chr(UB c)
{
    /* 送信レディでなかったら送信割込み許可 */

    if (!(sci_in(SIO_LSR) & SIO_TxRDY))
    {   sci_out(SIO_IER, SIO[CH].cmd[0] |= SIO_TxIE);
        return FALSE;
    }

    /* フロー制御ありなら CTS チェック */
    /* CTS OFF なら モデムステータス割込み許可 */

    if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
    {   if (!(sci_in(SIO_MSR) & SIO_CTS))
        {   sci_out(SIO_IER, SIO[CH].cmd[0] |= SIO_MSIE);
            return FALSE;
        }
    }

    /* １文字送信 */

    sci_out(SIO_DR, SIO[CH].txchr = c);
    return TRUE;
}

/*****************************************************************************
* シリアル割込み禁止（内部関数）
*
******************************************************************************/

static void dis_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
    /*
     * MCRで割込禁止にしてしまう
     *    →   割込コントローラに依存しない＆CTSも禁止出来る
     */
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_OUT2);

    vset_psw(psw);

}

/*****************************************************************************
* シリアル割込み許可（内部関数）
*
******************************************************************************/

static void ena_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
    /*
     * MCRで割込許可にしてしまう
     *    →   割込コントローラに依存しない＆CTSも許可出来る
     */
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_OUT2);

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

    /* RTS-CTSフロー制御の解除 */
#ifndef NOFLW

    if (SIO[CH].flag & TSF_RTS)
        rts_on();

    /* DTR-CTSフロー制御の解除 */

    if (SIO[CH].flag & TSF_DTR)
        dtr_on();
#endif

    /* XON/OFFフロー制御の解除 */

#ifndef NOXON
    if (SIO[CH].flag & TSF_TXOFF)
    {   if (tx_chr(XON))
            SIO[CH].flag &= ~TSF_TXOFF;
        else
            SIO[CH].flag |= TSF_TXREQ;
    }
#endif
}

/*****************************************************************************
* 送信バッファへ１文字格納（内部関数）
*
* バッファ満杯で格納できなかった場合は、FALSE を返す。
******************************************************************************/

static BOOL put_txbuf(UB c)
{
    UH cnt;

    /* 送信バッファ空でXOFF受信していないなら直接出力 */

    if ((cnt = SIO[CH].txcnt) == 0)
    {
  #ifndef NOXON
        if (!(SIO[CH].flag & TSF_RXOFF))
  #endif
            if (tx_chr(c))
                return TRUE;
    }

    /* バッファ満杯チェック */

    if (cnt >= TXBUFSZ)
        return FALSE;

    /* バッファ内文字数 + 1 */

    SIO[CH].txcnt = cnt + 1;

    /* バッファへ格納 */

    *SIO[CH].txputp = c;

    /* 格納ポインタを１つ進める */

    if (++SIO[CH].txputp >= SIO[CH].txbuf + TXBUFSZ)
        SIO[CH].txputp = SIO[CH].txbuf;

    return TRUE;
}

/*****************************************************************************
* 受信バッファへ受信文字/ステータス格納（内部関数）
*
******************************************************************************/

static void put_rxbuf(void)
{
    int cnt;

    /* 連続ブレークなら格納しない */

    if ((SIO[CH].rxsts & SIO_BD) && (SIO[CH].oldsts & SIO_BD))
        return;

    /* バッファ満杯チェック */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (cnt == BUFSZ - 1)
        SIO[CH].rxsts |= SIO_RxRDY; /* オーバフローは RxRDY ビットに割り当て */

    /* バッファへ格納 */
    /* 終端文字検出+1 */

    if ((*SIO[CH].rxputp = SIO[CH].rxchr) == SIO[CH].eot)
        SIO[CH].eotcnt++;
    *(SIO[CH].rxputp + BUFSZ) = SIO[CH].rxsts;

    /* 格納ポインタを１つ進める */

    if (++SIO[CH].rxputp >= SIO[CH].rxbuf + BUFSZ)
        SIO[CH].rxputp = SIO[CH].rxbuf;

    /* バッファ内文字数 + 1 */

    SIO[CH].rxcnt = ++cnt;

    /* RTS-CTSフロー制御 */

#ifndef NOFLW
    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_off();

    /* DTR-CTSフロー制御 */

    else if (SIO[CH].flag & TSF_DTR)
    {   if (cnt == RTSOFF_LEN)
            dtr_off();
    }
#endif

    /* XON/OFFフロー制御 */

#ifndef NOXON
    if ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON)
    {   if (cnt == XOFF_LEN)
            SIO[CH].flag |= TSF_TXREQ;  /* 続く送信処理にXOFF送信要求セット */
    }
#endif
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
    UB  *p;

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
    {   if (tx_chr(XON))                    /* XON送信できたら */
            SIO[CH].flag &= ~TSF_TXOFF;     /* XOFF送信済みクリア */
        else                                /* 送信できなければ */
            SIO[CH].flag |= TSF_TXREQ;      /* 送信要求セット */
    }
#endif

    /* RTSフロー制御 */

#ifndef NOFLOW
    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_on();

    /* DTR-CTSフロー制御 */

    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_DTR))
        dtr_on();

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
* シリアル割込みハンドラ受信処理（内部関数）
*
******************************************************************************/

static void rx_int(void)
{
    int sts;
    int chr;
    int tid;

    /* 受信レディでなくなるまで繰り返し */

    tid = 0;
    while ((sts = sci_in(SIO_LSR)) & SIO_RxRDY)
    {
        /* 受信文字入力, ステータス記憶 */

        chr = sci_in(SIO_DR);
        SIO[CH].oldsts = SIO[CH].rxsts;     /* 前回の受信ステータス記憶 */
        SIO[CH].rxsts = (UB)(sts & (SIO_BD|SIO_FE|SIO_OE|SIO_PE));
        SIO[CH].rxchr = (UB)chr;

        /* XON/OFFフロー制御有りの場合 */

#ifndef NOXON
        if (SIO[CH].flag & TSF_XON)
        {
            /* XOFF受信なら送信中断させる */

            if (chr == XOFF)
            {   SIO[CH].flag |= TSF_RXOFF;
                continue;
            }

            /* XON 受信なら送信再開させる */

            if (chr == XON)
            {   SIO[CH].flag &= ~TSF_RXOFF;
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);
                continue;
            }
        }
#endif
        /* 受信バッファへ格納 */

        put_rxbuf();
        tid = SIO[CH].rxtid;
    }

    /* 受信待ち解除 */

    if (tid)
        iwup_tsk(tid);
}

/*****************************************************************************
* シリアル割込みハンドラ送信処理（内部関数）
*
******************************************************************************/

static void tx_int(void)
{
    UB *p;
    int cnt;
    int tid;

    /* 送信割込みとモデムステータス割込み禁止 */

    SIO[CH].cmd[0] &= ~(SIO_TxIE);
#ifdef  INT_IRQ
    if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
        SIO[CH].cmd[0] &= ~SIO_MSIE;
#endif

    sci_in(SIO_MSR);    /* モデムステータス割込みリセット */

    /* 送信レディでなくなるまで繰り返し */

    if (sci_in(SIO_LSR) & SIO_TxRDY)
    {   cnt = 0;
        /* フロー制御ありなら CTS チェック */
        /* CTS OFF なら モデムステータス割込み許可 */
#ifndef NOFLW
        if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
        {   if (!(sci_in(SIO_MSR) & SIO_CTS))
            {   SIO[CH].cmd[0] |= SIO_MSIE;
                return;
            }
        }
#endif
        /* 受信側から依頼されたXON/XOFFの送信 */
#ifndef NOXON
        if (SIO[CH].flag & TSF_TXREQ)
        {
            if (SIO[CH].flag & TSF_TXOFF)
            {   sci_out(SIO_DR, SIO[CH].txchr = XON);
                SIO[CH].flag &= ~TSF_TXOFF;
            }
            else
            {   sci_out(SIO_DR, SIO[CH].txchr = XOFF);
                SIO[CH].flag |= TSF_TXOFF;
            }
            SIO[CH].flag &= ~TSF_TXREQ;
            cnt++;
        }

        /* XOFF受信していたら送信不可 */

        if (SIO[CH].flag & TSF_RXOFF)
            return;
#endif

        for(; cnt < 16; cnt++)
        {
            /* 送信バッファ空なら送信不可 */

            if (--SIO[CH].txcnt == (UH)-1)
            {
                SIO[CH].txcnt = 0;
                if (tid = SIO[CH].tetid)
                    iwup_tsk(tid);          /* 送信終了待ち解除 */
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);          /* 送信待ち解除 */
                return;
            }

            /* １文字送信 */

            p = SIO[CH].txgetp;
            sci_out(UART_THR, *p);

            /* 取得ポインタを１つ進める */

            if (++p >= SIO[CH].txbuf + TXBUFSZ)
                p = SIO[CH].txbuf;
            SIO[CH].txgetp = p;
        }

        /* 送信待ち解除 */

        if (tid = SIO[CH].txtid)
            iwup_tsk(tid);
    }

    /* 送信割込み許可 */

    SIO[CH].cmd[0] |= SIO_TxIE;     /* 後で許可される   */

}

/*****************************************************************************
* シリアル割込みハンドラ本体（内部関数）
*
******************************************************************************/

static void int_sio_body(void)
{
    rx_int();
    tx_int();
    sci_out(SIO_IER, SIO[CH].cmd[0]);
}

/*****************************************************************************
* シリアル割込みハンドラ
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0(void)
#elif (CH==1)
INTHDR int_sio1(void)
#elif (CH==2)
INTHDR int_sio2(void)
#elif (CH==3)
INTHDR int_sio3(void)
#endif
{
    ent_int();              /* 割込み開始 */
    int_sio_body();         /* 割込みハンドラ本体 */
    ret_int();              /* 割込み終了 */
}

/*****************************************************************************
* 初期化パラメータ解析（内部関数）
*
******************************************************************************/

static BOOL set_param(const B *s, UB *mode, UW *baud)
{
    B c;
    UW b;
    UB md;

    /* モード判別 */

    md = 0;
    if      (strstr((const char *)s, (char *)"PE") != NULL)   md |= SIO_EVEN;
    else if (strstr((const char *)s, (char *)"PO") != NULL)   md |= SIO_ODD;
    else                                md |= SIO_NONE;
    if      (strstr((const char *)s, (char *)"S2") != NULL)   md |= SIO_S2;
    else                                md |= SIO_S1;

    /* データ長判別 */

    if      (strstr((const char *)s, (char *)"B7") != NULL)   md |= SIO_B7;
    else                                md |= SIO_B8;
    *mode = md;

    /* フロー制御有無判別 */

    if (strstr((const char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((const char *)s, (char *)"DTR") != NULL)   SIO[CH].flag |= TSF_DTR;
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
  #if (CH==0)
    static const T_DINH dinh_sio0 = { TA_HLNG, int_sio0 ,IP };
  #elif (CH==1)
    static const T_DINH dinh_sio1 = { TA_HLNG, int_sio1 ,IP };
  #elif (CH==2)
    static const T_DINH dinh_sio2 = { TA_HLNG, int_sio2 ,IP };
  #elif (CH==3)
    static const T_DINH dinh_sio3 = { TA_HLNG, int_sio3 ,IP };
  #endif
#else
#ifdef V3
  #if (CH==0)
    static const T_DINT dint_sio0 = { TA_HLNG, int_sio0 ,IP };
  #elif (CH==1)
    static const T_DINT dint_sio1 = { TA_HLNG, int_sio1 ,IP };
  #elif (CH==2)
    static const T_DINT dint_sio2 = { TA_HLNG, int_sio2 ,IP };
  #elif (CH==3)
    static const T_DINT dint_sio3 = { TA_HLNG, int_sio3 ,IP };
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
    UB mode;
    UW baud;

    /* 制御ブロック初期化 */

    init_buf();

    /* パラメータ解析 */

    if (!set_param(param, &mode, &baud))
        return E_PAR;

    /* デバイス初期化 */

    init_sio(mode, baud);

    /* 割込みハンドラの定義 */

    if (!(SIO[CH].flag & TSF_INIT))
    {
#ifdef V4
 #if (CH==0)
        def_inh(INT_SIO, &dinh_sio0);
 #elif (CH==1)
        def_inh(INT_SIO, &dinh_sio1);
 #elif (CH==2)
        def_inh(INT_SIO, &dinh_sio2);
 #elif (CH==3)
        def_inh(INT_SIO, &dinh_sio3);
 #endif
#else
#ifdef V3
 #if (CH==0)
        def_int(INT_SIO, &dint_sio0);
 #elif (CH==1)
        def_int(INT_SIO, &dint_sio1);
 #elif (CH==2)
        def_int(INT_SIO, &dint_sio2);
 #elif (CH==3)
        def_int(INT_SIO, &dint_sio3);
 #endif
#else
 #if (CH==0)
        def_int(INT_SIO, int_sio0);
 #elif (CH==1)
        def_int(INT_SIO, int_sio1);
 #elif (CH==2)
        def_int(INT_SIO, int_sio2);
 #elif (CH==3)
        def_int(INT_SIO, int_sio3);
 #endif
#endif
#endif
    }
    SIO[CH].flag |= TSF_INIT;       /* 初期化済みセット */

    /* シリアル割込み許可 */

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
    sci_out(SIO_IER, 0x00);
    sci_out(SIO_MCR, 0x00);             /* 送受信禁止, RTS/DTR OFF */

#if (defined(INT_IRQ))
    rts_off();                          /* RTS OFF */
#endif
    /* 割込み定義を取り消し */

#ifdef V4   /* NORTI4 */
    def_inh(INT_SIO, (T_DINH *)NADR);
#else
#ifdef V3   /* NORTI3 */
    def_int(INT_SIO, (T_DINT *)NADR);
#else       /* NORTi2 */
    def_int(INT_SIO, (INTHDRP)NADR);
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
    ER err;
    INT sts;

    for (;;)
    {
        chg_ims(IP);                /* シリアル割込み禁止 */

        /* 受信バッファから１文字得る */

        sts = get_rxbuf(c);

        if (sts != -1)              /* 受信文字あった場合 */
        {   chg_ims(0);             /* シリアル割込み許可 */

            /* 受信エラー判別 */

            if (sts & (SIO_RxRDY|SIO_BD|SIO_FE|SIO_OE|SIO_PE))
            {   if (sts & SIO_RxRDY)
                    err = EV_SIOOVF;
                else if (sts & SIO_OE)
                    err = EV_SIOORN;
                else if (sts & SIO_BD)
                    err = EV_SIOBRK;
                else if (sts & SIO_FE)
                    err = EV_SIOFRM;
                else /* SIO_PE */
                    err = EV_SIOPTY;
            }
            else
                err = E_OK;
            return err;
        }

        /* 受信割込み待ち */

        SIO[CH].rxtid = vget_tid();
        chg_ims(0);                 /* シリアル割込み許可 */
        err = wai_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* 複数回 iwup_tsk された場合の対策 */
        if (err)
            return err;             /* タイムアウト終了 */
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
    ER err;

    for (;;)
    {
        chg_ims(IP);                /* シリアル割込み禁止 */

        /* 送信バッファへ１文字格納 */

        if (put_txbuf(c))           /* 送信できた場合 */
        {   chg_ims(0);             /* シリアル割込み許可 */
            return E_OK;            /* 正常終了 */
        }

        /* 送信割込み待ち */

        SIO[CH].txtid = vget_tid();
        chg_ims(0);                 /* シリアル割込み許可 */
        err = wai_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* 複数回 iwup_tsk された場合の対策 */
        if (err)
            return err;             /* タイムアウト終了 */
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

    chg_ims(IP);                    /* シリアル割込み禁止 */

    /* バッファクリア */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* コマンド出力 */

    if (fncd & (TSIO_RXE|TSIO_RXD)) /* TSIO_TXE,TSIO_TXD 未サポート！ */
    {
        c = SIO[CH].cmd[0];
        if (fncd & TSIO_RXE)        c |= SIO_RxIE;
        if (fncd & TSIO_RXD)        c &= ~SIO_RxIE;
        sci_out(SIO_IER, SIO[CH].cmd[0] = c);
    }

    if (fncd & (TSIO_SBON|TSIO_SBOFF))
    {
        c = SIO[CH].cmd[1];
        if (fncd & TSIO_SBON)       c |= SIO_SB;
        else                        c &= ~SIO_SB;
        sci_out(SIO_LCR, SIO[CH].cmd[1] = c);
    }

    if (fncd & (TSIO_RTSON|TSIO_RTSOFF|TSIO_DTRON|TSIO_DTROFF))
    {
        c = SIO[CH].cmd[2];
        if (fncd & TSIO_RTSON)      c |=  SIO_RTS;
        if (fncd & TSIO_RTSOFF)     c &= ~SIO_RTS;
        if (fncd & TSIO_DTRON)      c |=  SIO_DTR;
        if (fncd & TSIO_DTROFF)     c &= ~SIO_DTR;
        sci_out(SIO_MCR, SIO[CH].cmd[2] = c);
    }

    chg_ims(0);                     /* シリアル割込み許可 */
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
    UB stat;

    chg_ims(IP);                        /* シリアル割込み禁止 */

    stat = get_stat();
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(0);                         /* シリアル割込み許可 */

    return E_OK;
}

/*****************************************************************************
* シリアル送信バッファ･フラッシュ
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
    ER err;

    for (;;)
    {
        chg_ims(IP);                    /* シリアル割込み禁止 */

        /* 送信バッファ空の場合 */

        if (SIO[CH].txcnt == 0)
        {
            chg_ims(0);                 /* シリアル割込み許可 */
            return E_OK;                /* 正常終了 */
        }

        /* 送信終了割込み待ち */

        SIO[CH].tetid = vget_tid();
        chg_ims(0);                     /* シリアル割込み許可 */
        err = wai_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* 複数回 iwup_tsk された場合の対策 */
        if (err)
            return err;                 /* タイムアウト終了 */
    }
}

/* end */
