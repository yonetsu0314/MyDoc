/****************************************************************************
* NORTi4 Sample Program for Hitachi (MS7751SE01) Evaluation board.          *
*                                                                           *
*                                                                           *
* 05/Sept/2001              :SP                                             *
* 13/Dec/2002  glb_func.h file removed                          [SP]        *
* 18/Jun/2003 sioの設定を"38400 B8 PN S1"に統一                          KA *
* 25/Jun/2003 NORTiSIM対応                                               KA *
****************************************************************************/

#ifndef SH4
#define SH4                 /* SH4 を定義してkernel.hをincludeしてください */
#endif

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7751.h"
#include "ms7751.h"
#endif

/* Object ID */

#define TSK_MAIN    1       /* Main task ID */
#define TSK_SUB     2       /* Sub task ID */
#define TSK_RCV1    3       /* CH0 Receive task ID */
#define TSK_SND1    4       /* CH0 Send task ID */
#define TSK_RCV2    5       /* CH1 Receive task ID */
#define TSK_SND2    6       /* CH1 Send task ID */
#define TSK_RCV3    7       /* CH2 Receive task ID */
#define TSK_SND3    8       /* CH2 Send task ID */
#define TSK_LED     9       /* LED task ID */
#define FLG_TST     1       /* Event flag ID */
#define SEM_TST     1       /* Semaphore ID */
#define MBX_COM     1       /* Mail box ID */
#define MPF_COM     1       /* Memory pool ID */

/* System configuration */

#define TSKID_MAX   9       /* Maximum ID for TASK */
#define SEMID_MAX   1       /* Maximum ID for SEMAPHORE */
#define FLGID_MAX   1       /* Maximum ID for EVENT FLAG */
#define MBXID_MAX   3       /* Maximum ID for MAIL BOX */
#define MBFID_MAX   1       /* Maximum ID for MESSAGE BUFFER */
#define PORID_MAX   1       /* Maximum ID for RENDEVOUZ PORT */
#define MPLID_MAX   1       /* Maximum ID for VALIABLE LENGTH MEMORY POOL */
#define MPFID_MAX   1       /* Maximum ID for FIXED LENGTH MEMORY POOL */
#define CYCNO_MAX   1       /* Maximum ID for CYCRIC HANDLER */
#define ALMNO_MAX   1       /* Maximum ID for ALARM HANDLER  */

#define ISTKSZ      1024     /* Stack size for Interval timer interrupt */

#include "nocfg4.h"
#include "nosio.h"

/* message structure */

#define COMBUFSZ    256
#define NCOMMPL     32

typedef struct t_commsg
{
    struct t_commsg *next;
    B buf[COMBUFSZ];

} T_COMMSG;

/* Information for creating task */

TASK MainTask(void);
TASK SubTask(void);
TASK RcvTask(INT);
TASK SndTask(INT);
TASK LedTask(void);

const T_CTSK ctsk_main = { TA_HLNG, NULL, MainTask, 7, 1024, NULL, (B *)"main" };
const T_CTSK ctsk_sub  = { TA_HLNG, NULL, SubTask,  6, 1024, NULL, (B *)"sub" };
const T_CTSK ctsk_rcv1 = { TA_HLNG, NULL, RcvTask,  5, 1024, NULL, (B *)"rcvtask1" };
const T_CTSK ctsk_snd1 = { TA_HLNG, NULL, SndTask,  6, 1024, NULL, (B *)"sndtask1" };
const T_CTSK ctsk_rcv2 = { TA_HLNG, NULL, RcvTask,  5, 1024, NULL, (B *)"rcvtask2" };
const T_CTSK ctsk_snd2 = { TA_HLNG, NULL, SndTask,  6, 1024, NULL, (B *)"sndtask2" };
const T_CTSK ctsk_rcv3 = { TA_HLNG, NULL, RcvTask,  5, 1024, NULL, (B *)"rcvtask3" };
const T_CTSK ctsk_snd3 = { TA_HLNG, NULL, SndTask,  6, 1024, NULL, (B *)"sndtask3" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  2, 1024, NULL, (B *)"ledtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };

const char title[] = "\n\n\r\
------------------------------------------------------------------------\r\n\
          NORTi Version 4 Sample Demonstration program                  \r\n\
                             for                                        \r\n\
               MS7751SE01 Hitachi Evaluation board                      \r\n\
                     Controller - SH7751                                \r\n\
                                                                        \r\n\
Program version 2.0                                                     \r\n\
------------------------------------------------------------------------\r\n";

/*****************************************************************************
* Initialize port（★ Please customize !!）
*
******************************************************************************/

void pci_check(void);   /* PCI bus driver upload */

void ini_pio(void)
{
    UW cnt;
    led_out(LED_ALL);
    for (cnt=60000; cnt > 0; cnt--);
    led_out(!LED_ALL);

    pci_check();            /* PCI bus driver upload */
    init_sio_device();      /* Initialize Super IO controller */
    if (init_mc1543() != E_OK) {
        led_out(LED_BOARD_ERROR);
        for (;;);           /* error hold */
    }
    led_out(LED_SIO_INIT);

    /*
        .... Add Customized Device Initialization program here...
    */

}

/*****************************************************************************
* Main task
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    sta_tsk(TSK_SUB,  0);
    sta_tsk(TSK_RCV1, 0);
    sta_tsk(TSK_SND1, 0);
    sta_tsk(TSK_RCV2, 1);
    sta_tsk(TSK_SND2, 1);
    sta_tsk(TSK_RCV3, 2);
    sta_tsk(TSK_SND3, 2);
    sta_tsk(TSK_LED,  0);

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

    ctl_sio(ch, TSIO_RXE|TSIO_TXE|TSIO_RTSON|TSIO_DTRON);

    for (i=0; title[i]!=0; i++) {
        put_sio(ch, (UB)(title[i]), 10/MSEC);
    }
    put_sio(ch, (UB)('\r'), 10/MSEC);

    for (i = 0;;) {
        ref_sio(ch, &sios);
        if (sios.rxlen != 0)
            break;
        dly_tsk(1000/MSEC);
        put_sio(ch, (UB)('0' + i), 10/MSEC);
        if (++i > 9)
            i = 0;
    }

    for (;;) {
        /* Get memory block */

        get_mpf(MPF_COM, &msg);

        /* Received one line */

        for (i = 0;;) {
            ercd = get_sio(ch, &c, 100/MSEC);
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
    ER ercd;

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
       /* Wait completion */

        fls_sio(ch, TMO_FEVR);

        /* Release memory block */

        rel_mpf(MPF_COM, msg);

     }
}

/*****************************************************************************
* LED タスク
*
******************************************************************************/

TASK LedTask(void)
{
    UB c;

    for (c = 0;; c++)
    {
        led_out(c<<8);
        dly_tsk(250/MSEC);
    }
}

/*****************************************************************************
* main
*
******************************************************************************/

int main(void)
{

    /* Initialize system */

    sysini();
    ini_pio();

    /* Create tasks */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV1, &ctsk_rcv1);
    cre_tsk(TSK_SND1, &ctsk_snd1);
    cre_tsk(TSK_RCV2, &ctsk_rcv2);
    cre_tsk(TSK_SND2, &ctsk_snd2);
    cre_tsk(TSK_RCV3, &ctsk_rcv3);
    cre_tsk(TSK_SND3, &ctsk_snd3);
    cre_tsk(TSK_LED,  &ctsk_led);

    /* create objects */

    cre_sem(SEM_TST,   &csem);  /* Create semaphore */
    cre_flg(FLG_TST,   &cflg);  /* Create event flag */
    cre_mpf(MPF_COM,   &cmpf);  /* Create fixed memory pool */
    cre_mbx(MBX_COM+0, &cmbx);  /* Create mail box(Channel 0) */
    cre_mbx(MBX_COM+1, &cmbx);  /* Create mail box(Channel 1) */
    cre_mbx(MBX_COM+2, &cmbx);  /* Create mail box(Channel 1) */

    /* Start task */

    sta_tsk(TSK_MAIN, 0);

    intsta();                   /* Start interval timer interrupt */
    syssta();                   /* enter into multi task */
}

/* end */
