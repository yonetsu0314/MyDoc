/****************************************************************************
* NORTi �T���v���v���O���� Computex SH7055 EVA BOD                          *
*                                                                           *
*                                                                           *
* 07/Feb/2001                                                               *
* 18/Jun/2003 sio�̐ݒ��"38400 B8 PN S1"�ɓ���                          KA *
* 24/Jun/2003 NORTiSIM�Ή�                                               KA *
****************************************************************************/

#ifndef SH2E
#define SH2E                /* SH2E ���`����kernel.h��include���Ă������� */
#endif

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7055.h"
#include "nos7055.h"
#endif

/* �I�u�W�F�N�g�h�c */

#define TSK_MAIN    1       /* ���C���^�X�N�h�c */
#define TSK_SUB     2       /* �T�u�^�X�N�h�c */
#define TSK_RCV     3       /* ��M�^�X�N�h�c */
#define TSK_SND     4       /* ���M�^�X�N�h�c */
#define TSK_LED     5       /* LED task ID */
#define FLG_TST     1       /* �e�X�g�p�C�x���g�t���O�h�c */
#define SEM_TST     1       /* �e�X�g�p�Z�}�t�H�h�c */
#define MBX_COM     1       /* ���C���{�b�N�X�h�c */
#define MPF_COM     1       /* �������v�[���h�c */

/* �R���t�B�O���[�V���� */

#define TSKID_MAX   5       /* �^�X�N�h�c��� */
#define SEMID_MAX   1       /* �Z�}�t�H�h�c��� */
#define FLGID_MAX   1       /* �C�x���g�t���O�h�c��� */
#define MBXID_MAX   1       /* ���C���{�b�N�X�h�c��� */
#define MBFID_MAX   1       /* ���b�Z�[�W�o�b�t�@�h�c��� */
#define PORID_MAX   1       /* �����f�u�p�|�[�g�h�c��� */
#define MPLID_MAX   1       /* �ϒ��������v�[���h�c��� */
#define MPFID_MAX   1       /* �Œ蒷�������v�[���h�c��� */
#define CYCNO_MAX   1       /* �����N���n���h���ԍ���� */
#define ALMNO_MAX   1       /* �A���[���n���h���ԍ���� */
#define ISTKSZ      512     /* ���荞�݃n���h���̃X�^�b�N�T�C�Y */
#define KNL_LEVEL   10      /* �J�[�l���̊��荞�݃��x�� */

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
* MPU �������̗�
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
* ���C���^�X�N
*
******************************************************************************/

TASK MainTask(void)
{
    UINT ptn;

    /* ���^�X�N�N�� */
    sta_tsk(TSK_SUB, 0);
    sta_tsk(TSK_RCV, 0);
    sta_tsk(TSK_SND, 0);
    sta_tsk(TSK_LED, 0);

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
    /* �V�X�e�������� */

    ini_mpu();                  /* MPU ������ */
    sysini();                   /* �}���`�^�X�N������ */

    /* �^�X�N���� */

    cre_tsk(TSK_MAIN, &ctsk_main);
    cre_tsk(TSK_SUB,  &ctsk_sub );
    cre_tsk(TSK_RCV,  &ctsk_rcv );
    cre_tsk(TSK_SND,  &ctsk_snd );
    cre_tsk(TSK_LED,  &ctsk_led);

    /* �^�X�N�ȊO�̃I�u�W�F�N�g���� */

    cre_sem(SEM_TST, &csem);
    cre_flg(FLG_TST, &cflg);
    cre_mpf(MPF_COM, &cmpf);
    cre_mbx(MBX_COM, &cmbx);

    /* �^�X�N�N�� */

    sta_tsk(TSK_MAIN, 0);

    /* �V�X�e���N�� */

    intsta();                   /* �C���^�[�o�����荞�݂��N�� */
    syssta();                   /* �}���`�^�X�N�ֈڍs */
}

/* end */
