/****************************************************************************
* NORTi4 サンプルプログラム  Computex SH3 EVA BOD (SH7729)                  *
*                                                                           *
*                                                                           *
* 18/Apr/2000                                                               *
* 18/Jun/2003 sioの設定を"38400 B8 PN S1"に統一                          KA *
* 26/Jun/2003 NORTiSIM対応                                               KA *
****************************************************************************/

#ifndef SH3
#define SH3                 /* SH3 を定義してkernel.hをincludeしてください */
#endif

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7729.h"
#endif

/* オブジェクトＩＤ */

#define TSK_MAIN    1       /* Main task ID */
#define TSK_SUB     2       /* Sub task ID */
#define TSK_RCV1    3       /* CH0 Receive task ID */
#define TSK_SND1    4       /* CH0 Send task ID */
#define TSK_LED     5       /* Led task ID */
#define FLG_TST     1       /* Event flag ID */
#define SEM_TST     1       /* Semaphore ID */
#define MBX_COM     1       /* Mail box ID */
#define MPF_COM     1       /* Memory pool ID */

/* コンフィグレーション */

#define TSKID_MAX   5       /* Maximum ID for TASK */
#define SEMID_MAX   1       /* Maximum ID for SEMAPHORE */
#define FLGID_MAX   1       /* Maximum ID for EVENT FLAG */
#define MBXID_MAX   1       /* Maximum ID for MAIL BOX */
#define MBFID_MAX   1       /* Maximum ID for MESSAGE BUFFER */
#define PORID_MAX   1       /* Maximum ID for RENDEVOUZ PORT */
#define MPLID_MAX   1       /* Maximum ID for VALIABLE LENGTH MEMORY POOL */
#define MPFID_MAX   1       /* Maximum ID for FIXED LENGTH MEMORY POOL */
#define CYCNO_MAX   1       /* Maximum ID for CYCRIC HANDLER */
#define ALMNO_MAX   1       /* Maximum ID for ALARM HANDLER  */
#define ISTKSZ      512     /* Stack size for Interval timer interrupt */
#define KNL_LEVEL   14      /* Kernel level */

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
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  5, 512, NULL, (B *)"led" };
const T_CTSK ctsk_rcv1 = { TA_HLNG, NULL, RcvTask,  1, 512, NULL, (B *)"rcvtask" };
const T_CTSK ctsk_snd1 = { TA_HLNG, NULL, SndTask,  2, 512, NULL, (B *)"sndtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };


/*****************************************************************************
* peripheral 初期化の例
*
******************************************************************************/

void ini_pio(void)
{
}

/*****************************************************************************
* 7セグLEDの点灯
*
******************************************************************************/

void out_7seg(unsigned int);

#if !defined(NORTiSIM)
#define PORT_LED 0xB8000000L
#define out_7seg(n) (void)(*(volatile unsigned long *)PORT_LED = ((unsigned long)(n)<<24))
#endif

void put_7seg(INT n)
{
    static const UB table[] = {
        ~0x3f,  /* 0: __fe dcba */
        ~0x06,  /* 1: ____ _cb_ */
        ~0x5b,  /* 2: _g_e d_ba */
        ~0x4f,  /* 3: _g__ dcba */
        ~0x66,  /* 4: _gf_ _cb_ */
        ~0x6d,  /* 5: _gf_ dc_a */
        ~0x7d,  /* 6: _gfe dc_a */
        ~0x07,  /* 7: ____ _cba */
        ~0x7f,  /* 8: _gfe dcba */
        ~0x6f,  /* 9: _gf_ dcba */
        ~0x77,  /* A: _gfe _cba */
        ~0x7c,  /* b: _gfe dc__ */
        ~0x39,  /* C: __fe d__a */
        ~0x5e,  /* d: _g_e dcb_ */
        ~0x79,  /* E: _gfe d__a */
        ~0x71,  /* F: _gfe ___a */
    };

    out_7seg(table[n]);
}

/*****************************************************************************
* メインタスク
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    sta_tsk(TSK_SUB, 0);
    sta_tsk(TSK_LED, 0);
    sta_tsk(TSK_RCV1, 0);
    sta_tsk(TSK_SND1, 0);

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

    chg_pri(TSK_SELF, 8);

    for (;;)
    {
        wup_tsk(TSK_MAIN);
        rel_wai(TSK_MAIN);
        tslp_tsk(20/MSEC);
        sig_sem(SEM_TST);
        set_flg(FLG_TST, 0x0001);
    }
}

/*****************************************************************************
* LEDタスク
*
******************************************************************************/

TASK LedTask(void)
{
    INT n;

    for (;;) {
        for (n = 0; n <= 15; n++) {
            put_7seg(n);
            dly_tsk(500/MSEC);
        }
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
* メイン
*
******************************************************************************/

int main(void)
{

    /* システム初期化 */

    sysini();                   /* マルチタスク初期化 */
    ini_pio();                  /* peripheral 初期化 */

    /* タスク生成 */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV1, &ctsk_rcv1);
    cre_tsk(TSK_SND1, &ctsk_snd1);
    cre_tsk(TSK_LED,  &ctsk_led );

    /* タスク以外のオブジェクト生成 */

    cre_sem(SEM_TST, &csem);    /* セマフォ生成 */
    cre_flg(FLG_TST, &cflg);    /* イベントフラグ生成 */
    cre_mpf(MPF_COM, &cmpf);    /* 固定長メモリプール生成 */
    cre_mbx(MBX_COM, &cmbx);    /* メイルボックス生成(Channel 0) */

    /* タスク起動 */

    sta_tsk(TSK_MAIN, 0);

    /* システム起動 */

    intsta();                   /* 周期タイマ割込みを起動 */
    syssta();                   /* マルチタスクへ移行 */
}

/* end */
