/****************************************************************************
* ＮＯＲＴｉサンプルプログラム（MiNET-7042）                                *
*                                                                           *
* 17/Apr/2000                                                               *
* 24/Dec/2002 sh7042.h->sh7040.h, nos7040.h->nos7040.hに                 KA *
* 18/Jun/2003 sioの設定を"38400 B8 PN S1"に統一                          KA *
* 24/Jun/2003 NORTiSIM対応                                               KA *
****************************************************************************/

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7040.h"
#include "nos7040.h"
#endif

/* オブジェクトＩＤ */

#define TSK_MAIN    1       /* メインタスクＩＤ */
#define TSK_SUB     2       /* サブタスクＩＤ */
#define TSK_RCV1    3       /* CH0 受信タスクＩＤ */
#define TSK_SND1    4       /* CH0 送信タスクＩＤ */
#define TSK_RCV2    5       /* CH1 受信タスクＩＤ */
#define TSK_SND2    6       /* CH1 送信タスクＩＤ */
#define TSK_LED     7       /* LED task ID */
#define FLG_TST     1       /* テスト用イベントフラグＩＤ */
#define SEM_TST     1       /* テスト用セマフォＩＤ */
#define MBX_COM     1       /* メイルボックスＩＤ */
#define MPF_COM     1       /* メモリプールＩＤ */

/* コンフィグレーション */

#define TSKID_MAX   7       /* タスクＩＤ上限 */
#define SEMID_MAX   1       /* セマフォＩＤ上限 */
#define FLGID_MAX   1       /* イベントフラグＩＤ上限 */
#define MBXID_MAX   2       /* メイルボックスＩＤ上限 */
#define MBFID_MAX   1       /* メッセージバッファＩＤ上限 */
#define PORID_MAX   1       /* ランデブ用ポートＩＤ上限 */
#define MPLID_MAX   1       /* 可変長メモリプールＩＤ上限 */
#define MPFID_MAX   1       /* 固定長メモリプールＩＤ上限 */
#define CYCNO_MAX   1       /* 周期起動ハンドラ番号上限 */
#define ALMNO_MAX   1       /* アラームハンドラ番号上限 */
#define ISTKSZ      512     /* 割り込みハンドラのスタックサイズ */
#define KNL_LEVEL   10      /* カーネルの割り込みレベル */

#include "nocfg4.h"
#include "nosio.h"

/* メッセージ構造体 */

#define COMBUFSZ    256
#define NCOMMPL     32

typedef struct t_commsg
{
    struct t_commsg *next;
    B buf[COMBUFSZ];

} T_COMMSG;

/* オブジェクト生成情報 */

TASK MainTask(void);
TASK SubTask(void);
TASK RcvTask(INT);
TASK SndTask(INT);
TASK LedTask(void);

const T_CTSK ctsk_main = { TA_HLNG, NULL, MainTask, 7, 512, NULL, (B *)"main" };
const T_CTSK ctsk_sub  = { TA_HLNG, NULL, SubTask,  6, 512, NULL, (B *)"sub" };
const T_CTSK ctsk_rcv1 = { TA_HLNG, NULL, RcvTask,  1, 512, NULL, (B *)"rcvtask" };
const T_CTSK ctsk_snd1 = { TA_HLNG, NULL, SndTask,  2, 512, NULL, (B *)"sndtask" };
const T_CTSK ctsk_rcv2 = { TA_HLNG, NULL, RcvTask,  1, 512, NULL, (B *)"rcvtask" };
const T_CTSK ctsk_snd2 = { TA_HLNG, NULL, SndTask,  2, 512, NULL, (B *)"sndtask" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  8, 512, NULL, (B *)"ledtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };

/*****************************************************************************
* CPU 初期化
*
******************************************************************************/

void ini_mpu(void)
{
#if !defined(NORTiSIM)
    /* CS0 2wait, CS1 0wait, CS2 1wait, CS3 1wait */

    sfr_outw(BSC_WCR1,0x1102);

    /* CS3,CS1 0アイドルサイクル, CS2 CS0 1アイドルサイクル */
    /* 全/CS アサート拡張あり */

    sfr_outw(BSC_BCR2,0x115f);

    /* IRQ7, WAIT 端子許可 */

    sfr_outw(PFC_PBCR1,0x0007);
#endif
}

/*****************************************************************************
* ポート初期化（★ ハードに合わせてカスタマイズしてください）
*
******************************************************************************/

#define PE_RTS0     0x0002      /* PE1 を チャネル0 RTS 信号端子とする */
#define PE_RTS1     0x0008      /* PE3 を チャネル1 RTS 信号端子とする */

void ini_pio(void)
{
#if !defined(NORTiSIM)
    sfr_outw(IO_PEDR,  0x0000);     /* LED用 PE8..15 初期値 */
    sfr_outw(PFC_PEIOR, 0xffe0);    /* LED用 PE8..15 出力設定 */

    sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0);
    sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0);
    sfr_outw(PFC_PACRL2, 0xad75);
    sfr_setw(IO_PEDR, PE_RTS0|PE_RTS1);
    sfr_setw(PFC_PEIOR, PE_RTS0|PE_RTS1);
    sfr_clrw(PFC_PECR2, 0xa0cc);
#endif
}

/*****************************************************************************
* RTS 信号ON/OFF
*
******************************************************************************/

#if !defined(NORTiSIM)
void rts0_on(void)  { sfr_clrw(IO_PEDR, PE_RTS0); } /* チャネル0 RTS 信号 ON */
void rts0_off(void) { sfr_setw(IO_PEDR, PE_RTS0); } /* チャネル0 RTS 信号 OFF */
void rts1_on(void)  { sfr_clrw(IO_PEDR, PE_RTS1); } /* チャネル1 RTS 信号 ON */
void rts1_off(void) { sfr_setw(IO_PEDR, PE_RTS1); } /* チャネル1 RTS 信号 OFF */
#endif

/*****************************************************************************
* メインタスク
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    /* 他タスク起動 */
    sta_tsk(TSK_SUB,  0);
    sta_tsk(TSK_RCV1, 0);
    sta_tsk(TSK_SND1, 0);
    sta_tsk(TSK_RCV2, 1);
    sta_tsk(TSK_SND2, 1);
    sta_tsk(TSK_LED,  0);

    /* SubTask とシステムコールのテスト */
    for (;;)
    {
        slp_tsk();
        slp_tsk();
        dly_tsk(10/MSEC);
        wai_sem(SEM_TST);
        wai_flg(FLG_TST, 0x0001, TWF_ORW, &ptn);
        clr_flg(FLG_TST, 0x0000);
    }
}

/*****************************************************************************
* サブタスク
*
******************************************************************************/

TASK SubTask(void)
{
    /* MainTask より優先度を下げる */
    chg_pri(TSK_SELF, 8);

    /* MainTask とシステムコールのテスト */
    for (;;)
    {
        wup_tsk(TSK_MAIN);
        rel_wai(TSK_MAIN);
        tslp_tsk(20/MSEC);
        sig_sem(SEM_TST);
        set_flg(FLG_TST, 0x0001);
    }
}

/*****************************************************************************/
/* 受信タスク                                                                */
/*                                                                           */
/*****************************************************************************/

TASK RcvTask(INT ch)
{
    T_COMMSG *msg;
    T_SIOS sios;
    ER ercd;
    UINT i;
    UB c;

    /* initialize */

    ini_sio(ch, (B *)"38400 B8 PN S1");
    ctl_sio(ch, TSIO_RXE|TSIO_TXE|TSIO_RTSON);

    for (i = 0;;)
    {
        ref_sio(ch, &sios);
        if (sios.rxlen != 0)
            break;
        dly_tsk(1000/MSEC);
        put_sio(ch, (UB)('0' + i), 10/MSEC);
        if (++i > 9)
            i = 0;
    }

    for (;;)
    {
        /* Get memory block */

        get_mpf(MPF_COM, &msg);

        /* Received one line */

        for (i = 0;;)
        {
            ercd = get_sio(ch, &c, 1000/MSEC);
            if (ercd == E_TMOUT)
                break;
            if (c == '\n')
                continue;
            msg->buf[i] = c;
            if ((++i >= COMBUFSZ - 1) || (c == '\r'))
                break;
        }
        msg->buf[i] = '\0';

        /* Send message */

        snd_mbx(MBX_COM+ch, msg);
    }
}

/*****************************************************************************/
/* 送信タスク                                                                */
/*                                                                           */
/*****************************************************************************/

TASK SndTask(INT ch)
{
    T_COMMSG *msg;
    UINT i;
    UB c;

    for (;;)
    {
        /* Wait message */

        rcv_mbx(MBX_COM+ch, &msg);

        /* Send 1 line */

        for (i = 0;;)
        {
            c = msg->buf[i++];
            if (c == '\0')
                break;
            put_sio(ch, c, TMO_FEVR);
            if (c == '\r')
                put_sio(ch, '\n', TMO_FEVR);
        }

        /* Release memory block */

        rel_mpf(MPF_COM, msg);

        /* Wait completion */

        fls_sio(ch, TMO_FEVR);
    }
}

/*****************************************************************************
* LED タスク
*
******************************************************************************/

void out_led(unsigned int);

#if !defined(NORTiSIM)
#define out_led(n) (void)(*(volatile UB *)(IO_BASE+IO_PEDR) = (UB)(n))
#endif

TASK LedTask(void)
{
    UB c;

    for (c = 0;; c++)
    {
        out_led(c);
        dly_tsk(250/MSEC);
    }
}

/*****************************************************************************
* メイン
*
******************************************************************************/

int main(void)
{

    /* システム初期化 */

    ini_mpu();
    sysini();
    ini_pio();

    /* タスク生成 */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV1, &ctsk_rcv1);
    cre_tsk(TSK_SND1, &ctsk_snd1);
    cre_tsk(TSK_RCV2, &ctsk_rcv2);
    cre_tsk(TSK_SND2, &ctsk_snd2);
    cre_tsk(TSK_LED,  &ctsk_led);

    /* タスク以外のオブジェクト生成 */

    cre_sem(SEM_TST,   &csem);  /* セマフォ生成 */
    cre_flg(FLG_TST,   &cflg);  /* イベントフラグ生成 */
    cre_mpf(MPF_COM,   &cmpf);  /* 固定長メモリプール生成 */
    cre_mbx(MBX_COM+0, &cmbx);  /* メイルボックス生成(Channel 0) */
    cre_mbx(MBX_COM+1, &cmbx);  /* メイルボックス生成(Channel 1) */

    /* タスク起動 */

    sta_tsk(TSK_MAIN, 0);

    /* システム起動 */

    intsta();                   /* インターバル割り込みを起動 */
    syssta();                   /* マルチタスクへ移行 */
}

/* end */
