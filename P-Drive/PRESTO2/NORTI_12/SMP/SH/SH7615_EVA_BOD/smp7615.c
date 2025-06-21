/****************************************************************************
* NORTi4 �T���v���v���O���� (SH7615 EVA BOD+)                               *
*                                                                           *
*                                                                           *
* 08/Feb/2001 new Release                                                   *
* 18/Jun/2003 sio�̐ݒ��"38400 B8 PN S1"�ɓ���                          KA *
* 24/Jun/2003 NORTiSIM�Ή�                                               KA *
****************************************************************************/

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7615.h"
#include "nos7615.h"
#endif

/* Object ID */

#define TSK_MAIN    1       /* Main task ID */
#define TSK_SUB     2       /* Sub task ID */
#define TSK_RCV1    3       /* CH0 Receive task ID */
#define TSK_SND1    4       /* CH0 Send task ID */
#define TSK_LED     5       /* LED task ID */
#define FLG_TST     1       /* Event flag ID */
#define SEM_TST     1       /* Semaphore ID */
#define MBX_COM     1       /* Mail box ID */
#define MPF_COM     1       /* Memory pool ID */

/* System configuration */

#define TSKID_MAX   5       /* Maximum ID for TASK */
#define SEMID_MAX   1       /* Maximum ID for SEMAPHORE */
#define FLGID_MAX   1       /* Maximum ID for EVENT FLAG */
#define MBXID_MAX   2       /* Maximum ID for MAIL BOX */
#define MBFID_MAX   1       /* Maximum ID for MESSAGE BUFFER */
#define PORID_MAX   1       /* Maximum ID for RENDEVOUZ PORT */
#define MPLID_MAX   1       /* Maximum ID for VALIABLE LENGTH MEMORY POOL */
#define MPFID_MAX   1       /* Maximum ID for FIXED LENGTH MEMORY POOL */
#define CYCNO_MAX   1       /* Maximum ID for CYCRIC HANDLER */
#define ALMNO_MAX   1       /* Maximum ID for ALARM HANDLER  */

#define ISTKSZ      512     /* Stack size for Interval timer interrupt */

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
const T_CTSK ctsk_rcv1 = { TA_HLNG, NULL, RcvTask,  1, 1024, NULL, (B *)"rcvtask" };
const T_CTSK ctsk_snd1 = { TA_HLNG, NULL, SndTask,  2, 1024, NULL, (B *)"sndtask" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  8, 1024, NULL, (B *)"ledtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };

/*****************************************************************************
* Initialize peripherals on evaluation board�i�� Please customize !!�j
*
******************************************************************************/
void init_peripheral(void)
{
}

/*****************************************************************************
* Initialize port�i�� Please customize !!�j
*
******************************************************************************/

void ini_pio(void)
{
#if !defined(NORTiSIM)
  sfr_setw(PFC_PBCR2, PBCR_RXD2|PBCR_TXD2);
#endif
}

/*****************************************************************************
* Main task
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    /* ���^�X�N�N�� */
    sta_tsk(TSK_SUB,  0);
    sta_tsk(TSK_RCV1, 0);
    sta_tsk(TSK_SND1, 0);
    sta_tsk(TSK_LED,  0);

    /* SubTask �ƃV�X�e���R�[���̃e�X�g */
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
    /* MainTask ���D��x�������� */
    chg_pri(TSK_SELF, 8);

    /* MainTask �ƃV�X�e���R�[���̃e�X�g */
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
#define PORT_LED 0x28000003
#define out_7seg(n) (void)(*(volatile UB *)PORT_LED = (UB)(n))
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
    /* Initialize processor�i�� Please customize !!�j*/

    init_peripheral();

    /* Initialize system */

    sysini();
    ini_pio();

    /* Create tasks */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV1, &ctsk_rcv1);
    cre_tsk(TSK_SND1, &ctsk_snd1);
    cre_tsk(TSK_LED,  &ctsk_led);

    /* create objects */

    cre_sem(SEM_TST,   &csem);  /* Create semaphore */
    cre_flg(FLG_TST,   &cflg);  /* Create event flag */
    cre_mpf(MPF_COM,   &cmpf);  /* Create fixed memory pool */
    cre_mbx(MBX_COM+0, &cmbx);  /* Create mail box(Channel 0) */
    cre_mbx(MBX_COM+1, &cmbx);  /* Create mail box(Channel 1) */

    /* Start task */

    sta_tsk(TSK_MAIN, 0);

    /* Start multitask system */

    intsta();                   /* Start interval timer interrupt */
    syssta();                   /* enter into multi task */
}

/* end */
