/******************************************************************************
* ＮＯＲＴｉ対応 シルアル入出力関数ヘッダ                                     *
*                                                                             *
*  File name : nosio.h                                                        *
*  Copyright (c) 1993-2000, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
* 93-04-14 作成                                                               *
* 93-05-16 Ver1.13β                                                          *
* 94-01-05 Ver1.14                                                            *
* 95-06-15 Ver1.20                                                            *
* 95-08-14 T_SIOFP の定義を nosio.c へ移す                                    *
* 96-05-20 Ver1.21                                                            *
* 96-05-29 Ver1.30                                                            *
* 00-04-07 NORTi4対応 pascalをNORTiAPIに変更                                  *
******************************************************************************/

#ifndef NOSIO_H
#define NOSIO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LSI_C80
#ifndef const
#define const   /**/
#endif
#endif

/* エラーコード */

#define EV_SIOINI   1       /* 未初期化 */
#define EV_SIOOVF   2       /* 受信バッファオーバフロー */
#define EV_SIOPTY   3       /* パリティエラー */
#define EV_SIOORN   4       /* オーバーランエラー */
#define EV_SIOFRM   5       /* フレーミングエラー */
#define EV_SIOBRK   6       /* ブレーク検出 */

/* 機能コード */

#define TSIO_RXE    0x01    /* 受信イネーブル */
#define TSIO_RXD    0x02    /* 受信ディセーブル */
#define TSIO_TXE    0x04    /* 送信イネーブル */
#define TSIO_TXD    0x08    /* 送信ディセーブル */
#define TSIO_RTSON  0x10    /* RTS信号ON */
#define TSIO_RTSOFF 0x20    /* RTS信号OFF */
#define TSIO_DTRON  0x40    /* DTR信号ON */
#define TSIO_DTROFF 0x80    /* DTR信号OFF */
#define TSIO_RXCLR  0x0100  /* 受信バッファクリア */
#define TSIO_TXCLR  0x0200  /* 送信バッファクリア */
#define TSIO_SBON   0x0400  /* ブレーク送信ON */
#define TSIO_SBOFF  0x0800  /* ブレーク送信OFF */

/* シリアル入出力状態パケット */

typedef struct t_sios
{
    UB siostat;             /* シリアル入出力ステータス */

#define TSIO_CD     0x01    /* 受信キャリア検出 */
#define TSIO_CTS    0x02    /* CTS信号ON(1)/OFF(0) */
#define TSIO_TXEMP  0x04    /* 送信バッファ空 */
#define TSIO_PE     0x08    /* パリティエラー */
#define TSIO_OE     0x10    /* オーバランエラー */
#define TSIO_FE     0x20    /* フレーミングエラー */
#define TSIO_BD     0x40    /* ブレーク状態検出 */
#define TSIO_DSR    0x80    /* DSR信号ON(1)/OFF(0) */

    UB rxchr;               /* 受信バッファの先頭の文字（未使用）*/
    UH rxlen;               /* 受信バッファのデータ長 */
    UH frbufsz;             /* 送信バッファの空きサイズ */
    UH eotcnt;              /* 受信バッファの終端文字個数 */

} T_SIOS;

/* 関数プロトタイプ */

ER NORTiAPI ini_sio(INT, const B *);
void cdecl ext_sio(void);
ER NORTiAPI get_sio(INT, UB *, TMO);
ER NORTiAPI put_sio(INT, UB, TMO);
ER NORTiAPI ctl_sio(INT, UH);
ER NORTiAPI ref_sio(INT, T_SIOS *);
ER NORTiAPI fls_sio(INT, TMO);

/* シリアル入出力制御ブロック構造体 */

typedef struct t_sio
{
    UB ch;                  /* チャネル番号 */
    UB flag;                /* 制御フラグ */

#define TSF_INIT    0x01    /* 初期化済み */
#define TSF_TXREQ   0x02    /* XON/XOFF送信要求 */
#define TSF_RXOFF   0x04    /* XOFF受信した */
#define TSF_TXOFF   0x08    /* XOFF送信した */
#define TSF_XON     0x10    /* XON/OFFによるフロー制御有り */
#define TSF_DTR     0x20    /* DTRによるフロー制御有り */
#define TSF_RTS     0x40    /* RTSによるフロー制御有り */

    UB txchr;               /* 送信文字 */
    UB rxchr;               /* 受信文字 */
    UB rxsts;               /* 受信ステータス */
    UB oldchr;              /* 前回の受信文字 */
    UB oldsts;              /* 前回の受信ステータス */

    UB eot;                 /* 終端文字 */
    UH eotcnt;              /* 終端文字検出カウンタ */

    ID txtid;               /* 送信待ちタスクＩＤ */
    ID rxtid;               /* 受信待ちタスクＩＤ */
    ID tetid;               /* 送信終了待ちタスクＩＤ */

    UB cmd[6];              /* SIOコマンドバッファ */
    UB rsv[2];              /* 予備 */

    UH txcnt;               /* 送信バッファ内の文字数 */
    UH rxcnt;               /* 受信バッファ内の文字数 */

    UB *txbuf;              /* 送信バッファ */
    UB *rxbuf;              /* 受信バッファ（+BUFSZ:受信ステータスバッファ）*/
    UB *txputp;             /* 送信バッファ格納ポインタ */
    UB *txgetp;             /* 送信バッファ取得ポインタ */
    UB *rxputp;             /* 受信バッファ格納ポインタ */
    UB *rxgetp;             /* 受信バッファ取得ポインタ */

  #ifdef NORTi86
    H far *vram;            /* モニタ表示用テキストVRAMアドレス */
  #endif

} T_SIO;

#ifdef __cplusplus
}
#endif

#endif /* NOSIO_H */
