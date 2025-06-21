/****************************************************************************
* �m�n�q�s���T���v���v���O�����iMiNET-7042�j                                *
*                                                                           *
* 17/Apr/2000                                                               *
* 24/Dec/2002 sh7042.h->sh7040.h, nos7040.h->nos7040.h��                 KA *
* 18/Jun/2003 sio�̐ݒ��"38400 B8 PN S1"�ɓ���                          KA *
* 24/Jun/2003 NORTiSIM�Ή�                                               KA *
****************************************************************************/

#include "kernel.h"
#if !defined(NORTiSIM)
#include "sh7040.h"
#include "nos7040.h"
#endif

/* �I�u�W�F�N�g�h�c */

#define TSK_MAIN    1       /* ���C���^�X�N�h�c */
#define TSK_SUB     2       /* �T�u�^�X�N�h�c */
#define TSK_RCV1    3       /* CH0 ��M�^�X�N�h�c */
#define TSK_SND1    4       /* CH0 ���M�^�X�N�h�c */
#define TSK_RCV2    5       /* CH1 ��M�^�X�N�h�c */
#define TSK_SND2    6       /* CH1 ���M�^�X�N�h�c */
#define TSK_LED     7       /* LED task ID */
#define FLG_TST     1       /* �e�X�g�p�C�x���g�t���O�h�c */
#define SEM_TST     1       /* �e�X�g�p�Z�}�t�H�h�c */
#define MBX_COM     1       /* ���C���{�b�N�X�h�c */
#define MPF_COM     1       /* �������v�[���h�c */

/* �R���t�B�O���[�V���� */

#define TSKID_MAX   7       /* �^�X�N�h�c��� */
#define SEMID_MAX   1       /* �Z�}�t�H�h�c��� */
#define FLGID_MAX   1       /* �C�x���g�t���O�h�c��� */
#define MBXID_MAX   2       /* ���C���{�b�N�X�h�c��� */
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
* CPU ������
*
******************************************************************************/

void ini_mpu(void)
{
#if !defined(NORTiSIM)
    /* CS0 2wait, CS1 0wait, CS2 1wait, CS3 1wait */

    sfr_outw(BSC_WCR1,0x1102);

    /* CS3,CS1 0�A�C�h���T�C�N��, CS2 CS0 1�A�C�h���T�C�N�� */
    /* �S/CS �A�T�[�g�g������ */

    sfr_outw(BSC_BCR2,0x115f);

    /* IRQ7, WAIT �[�q���� */

    sfr_outw(PFC_PBCR1,0x0007);
#endif
}

/*****************************************************************************
* �|�[�g�������i�� �n�[�h�ɍ��킹�ăJ�X�^�}�C�Y���Ă��������j
*
******************************************************************************/

#define PE_RTS0     0x0002      /* PE1 �� �`���l��0 RTS �M���[�q�Ƃ��� */
#define PE_RTS1     0x0008      /* PE3 �� �`���l��1 RTS �M���[�q�Ƃ��� */

void ini_pio(void)
{
#if !defined(NORTiSIM)
    sfr_outw(IO_PEDR,  0x0000);     /* LED�p PE8..15 �����l */
    sfr_outw(PFC_PEIOR, 0xffe0);    /* LED�p PE8..15 �o�͐ݒ� */

    sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0);
    sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0);
    sfr_outw(PFC_PACRL2, 0xad75);
    sfr_setw(IO_PEDR, PE_RTS0|PE_RTS1);
    sfr_setw(PFC_PEIOR, PE_RTS0|PE_RTS1);
    sfr_clrw(PFC_PECR2, 0xa0cc);
#endif
}

/*****************************************************************************
* RTS �M��ON/OFF
*
******************************************************************************/

#if !defined(NORTiSIM)
void rts0_on(void)  { sfr_clrw(IO_PEDR, PE_RTS0); } /* �`���l��0 RTS �M�� ON */
void rts0_off(void) { sfr_setw(IO_PEDR, PE_RTS0); } /* �`���l��0 RTS �M�� OFF */
void rts1_on(void)  { sfr_clrw(IO_PEDR, PE_RTS1); } /* �`���l��1 RTS �M�� ON */
void rts1_off(void) { sfr_setw(IO_PEDR, PE_RTS1); } /* �`���l��1 RTS �M�� OFF */
#endif

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
* ���C��
*
******************************************************************************/

int main(void)
{

    /* �V�X�e�������� */

    ini_mpu();
    sysini();
    ini_pio();

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

    intsta();                   /* �C���^�[�o�����荞�݂��N�� */
    syssta();                   /* �}���`�^�X�N�ֈڍs */
}

/* end */
