/****************************************************************************
* NORTi サンプルプログラム Computex SH7055 EVA BOD                          *
*                                                                           *
*                                                                           *
* 07/Feb/2001                                                               *
* 18/Jun/2003 sioの設定を"38400 B8 PN S1"に統一                          KA *
* 24/Jun/2003 NORTiSIM対応                                               KA *
****************************************************************************/

#ifndef SH2E
#define SH2E                /* SH2E を定義してkernel.hをincludeしてください */
#endif

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7055.h"
#include "nos7055.h"
#endif

/* オブジェクトＩＤ */

#define TSK_MAIN    1       /* メインタスクＩＤ */
#define TSK_SUB     2       /* サブタスクＩＤ */
#define TSK_RCV     3       /* 受信タスクＩＤ */
#define TSK_SND     4       /* 送信タスクＩＤ */
#define TSK_LED     5       /* LED task ID */
#define FLG_TST     1       /* テスト用イベントフラグＩＤ */
#define SEM_TST     1       /* テスト用セマフォＩＤ */
#define MBX_COM     1       /* メイルボックスＩＤ */
#define MPF_COM     1       /* メモリプールＩＤ */

/* コンフィグレーション */

#define TSKID_MAX   5       /* タスクＩＤ上限 */
#define SEMID_MAX   1       /* セマフォＩＤ上限 */
#define FLGID_MAX   1       /* イベントフラグＩＤ上限 */
#define MBXID_MAX   1       /* メイルボックスＩＤ上限 */
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

const T_CTSK ctsk_main = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (B *)"main" };
const T_CTSK ctsk_sub  = { TA_HLNG, NULL, SubTask,  6, 1024, NULL, (B *)"sub" };
const T_CTSK ctsk_rcv  = { TA_HLNG, NULL, RcvTask,  1, 1024, NULL, (B *)"rcvtask" };
const T_CTSK ctsk_snd  = { TA_HLNG, NULL, SndTask,  2, 1024, NULL, (B *)"sndtask" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  8, 1024, NULL, (B *)"ledtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };

/*****************************************************************************
* MPU 初期化の例
*
******************************************************************************/

void ini_mpu(void)
{
#if !defined(NORTiSIM)
    sfr_outw(PC_DR, PC_TXD1);
    sfr_outw(PFC_PCIOR, PC_TXD1);
    sfr_outw(PFC_PCCR, 0x0005);
#endif
}

/*****************************************************************************
* メインタスク
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    /* 他タスク起動 */
    sta_tsk(TSK_SUB, 0);
    sta_tsk(TSK_RCV, 0);
    sta_tsk(TSK_SND, 0);
    sta_tsk(TSK_LED, 0);

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
* Sub task
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
/* Receive task                                                              */
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
/* Send Task                                                                 */
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
* Led task
*
******************************************************************************/

void out_7seg(unsigned int);

#if !defined(NORTiSIM)
#define PORT_LED 0x00C00000
#define out_7seg(n) (void)(*(volatile UH *)PORT_LED = (UH)(n))
#endif

TASK LedTask(void)
{
    int i;

    /*                    0    1    2    3    4    5    6    7    8    9 */
    static led_ptn[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98};

    for (;;){
        for (i=0;i<10;i++){
            out_7seg(led_ptn[i]);
            dly_tsk(300/MSEC);
        }
    }
}

/*****************************************************************************
* main
*
******************************************************************************/

int main(void)
{
    /* システム初期化 */

    ini_mpu();                  /* MPU 初期化 */
    sysini();                   /* マルチタスク初期化 */

    /* タスク生成 */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV,  &ctsk_rcv );
    cre_tsk(TSK_SND,  &ctsk_snd );
    cre_tsk(TSK_LED,  &ctsk_led);

    /* タスク以外のオブジェクト生成 */

    cre_sem(SEM_TST, &csem);
    cre_flg(FLG_TST, &cflg);
    cre_mpf(MPF_COM, &cmpf);
    cre_mbx(MBX_COM, &cmbx);

    /* タスク起動 */

    sta_tsk(TSK_MAIN, 0);

    /* システム起動 */

    intsta();                   /* インターバル割り込みを起動 */
    syssta();                   /* マルチタスクへ移行 */
}

/* end */
