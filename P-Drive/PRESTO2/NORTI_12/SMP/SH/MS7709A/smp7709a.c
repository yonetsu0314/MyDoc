/****************************************************************************
* NORTi4 �T���v���v���O���� (MS7709ASE01)                                   *
*                                                                           *
*                                                                           *
* 16/Apr/2000                                                               *
* 02/Sep/2002  �R�����g�C��                                              KA *
* 18/Jun/2003 sio�̐ݒ��"38400 B8 PN S1"�ɓ���                          KA *
* 25/Jun/2003 NORTiSIM�Ή�                                               KA *
****************************************************************************/

#ifndef SH3
#define SH3                 /* SH3 ���`����kernel.h��include���Ă������� */
#endif

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7709a.h"
#include "ms7709a.h"
#endif

/* �I�u�W�F�N�g�h�c */

#define TSK_MAIN    1       /* Main task ID */
#define TSK_SUB     2       /* Sub task ID */
#define TSK_RCV1    3       /* CH0 Receive task ID */
#define TSK_SND1    4       /* CH0 Send task ID */
#define TSK_RCV2    5       /* CH1 Receive task ID */
#define TSK_SND2    6       /* CH1 Send task ID */
#define TSK_LED     7       /* LED task ID */
#define FLG_TST     1       /* Event flag ID for test */
#define SEM_TST     1       /* Semaphore ID for test */
#define MBX_COM     1       /* Mail box ID */
#define MPF_COM     1       /* Memory pool ID */

/* �R���t�B�O���[�V���� */

#define TSKID_MAX   7       /* Maximum ID for TASK */
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
#define KNL_LEVEL   10      /* Kernel level */

#include "nocfg4.h"
#include "nosio.h"

/* ���b�Z�[�W�\���� */

#define COMBUFSZ    256
#define NCOMMPL     32

typedef struct t_commsg
{
    struct t_commsg *next;
    B buf[COMBUFSZ];

} T_COMMSG;

/* �I�u�W�F�N�g������� */

TASK MainTask(void);
TASK SubTask(void);
TASK RcvTask(INT);
TASK SndTask(INT);
TASK LedTask(void);

const T_CTSK ctsk_main = { TA_HLNG, NULL, MainTask, 7, 512, NULL, (B *)"main" };
const T_CTSK ctsk_sub  = { TA_HLNG, NULL, SubTask,  6, 512, NULL, (B *)"sub" };
const T_CTSK ctsk_rcv1 = { TA_HLNG, NULL, RcvTask,  1, 512, NULL, (B *)"rcvtask1" };
const T_CTSK ctsk_snd1 = { TA_HLNG, NULL, SndTask,  2, 512, NULL, (B *)"sndtask1" };
const T_CTSK ctsk_rcv2 = { TA_HLNG, NULL, RcvTask,  1, 512, NULL, (B *)"rcvtask2" };
const T_CTSK ctsk_snd2 = { TA_HLNG, NULL, SndTask,  2, 512, NULL, (B *)"sndtask2" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  2, 512, NULL, (B *)"ledtask" };
const T_CMBX cmbx      = { TA_TFIFO|TA_MFIFO, 0, NULL, (B *)"mbx1" };
const T_CMPF cmpf      = { TA_TFIFO, NCOMMPL, sizeof (T_COMMSG), NULL, (B *)"mpf1" };
const T_CSEM csem      = { TA_TFIFO, 0, 255, (B *)"sem1" };
const T_CFLG cflg      = { TA_WMUL, 0, (B *)"flag" };

/*****************************************************************************
* MPU �������̗�
*
* �n�[�h�E�F�A�ɍ��킹�ăJ�X�^�}�C�Y���Ă��������B
******************************************************************************/

void ini_mpu(void)
{
    /* ���Z�b�g����ɏ��������s���Ă��� */
}

/*****************************************************************************
* ���C���^�X�N
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    /* ���^�X�N�N�� */
    sta_tsk(TSK_SUB,  0);
    sta_tsk(TSK_RCV1, 0);
    sta_tsk(TSK_SND1, 0);
    sta_tsk(TSK_RCV2, 1);
    sta_tsk(TSK_SND2, 1);
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
* �T�u�^�X�N
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
/* ��M�^�X�N                                                                */
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
/* ���M�^�X�N                                                                */
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
* LED �^�X�N
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
* ���C��
*
******************************************************************************/

int main(void)
{

    /* �V�X�e�������� */

    ini_mpu();                  /* MPU ������ */
    sysini();                   /* �}���`�^�X�N������ */

    /* �^�X�N���� */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV1, &ctsk_rcv1);
    cre_tsk(TSK_SND1, &ctsk_snd1);
    cre_tsk(TSK_RCV2, &ctsk_rcv2);
    cre_tsk(TSK_SND2, &ctsk_snd2);
    cre_tsk(TSK_LED,  &ctsk_led);

    /* �^�X�N�ȊO�̃I�u�W�F�N�g���� */

    cre_sem(SEM_TST,   &csem);  /* �Z�}�t�H���� */
    cre_flg(FLG_TST,   &cflg);  /* �C�x���g�t���O���� */
    cre_mpf(MPF_COM,   &cmpf);  /* �Œ蒷�������v�[������ */
    cre_mbx(MBX_COM+0, &cmbx);  /* ���C���{�b�N�X����(Channel 0) */
    cre_mbx(MBX_COM+1, &cmbx);  /* ���C���{�b�N�X����(Channel 1) */

    /* �^�X�N�N�� */

    sta_tsk(TSK_MAIN, 0);

    /* �V�X�e���N�� */

    intsta();                   /* �����^�C�}�����݂��N�� */
    syssta();                   /* �}���`�^�X�N�ֈڍs */
}

/* end */
