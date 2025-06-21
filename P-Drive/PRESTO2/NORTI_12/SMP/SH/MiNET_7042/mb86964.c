/******************************************************************************
* NORTi Network LAN Controller Driver (Fujitsu MB86964 for MiNET-704X)        *
*                                                                             *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Feb/1999 1.00                                                         MI *
* 24/Apr/1999 1.01 Waking up failure bug is fixed.                         MI *
* 19/Sep/2000 1.02 lan_intr�֐���DLCR3��DLCR2�̃`�F�b�N��ǉ�              MI *
* 19/Sep/2000 1.03 lan_get_end�֐���8�o�C�g���E�������폜                  MI *
* 04/Oct/2000 1.04 16��R���W�������W�X�^�̐ݒ�l 04H �� 06H               MI *
* 07/Dec/2002 1.05 lan_get_len������                                       MI *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "mb86964.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

/* ���[�U�[��`�̊֐�/�}�N�� */

#ifndef lan_in
UB lan_in(unsigned);            /* �o�C�g�f�[�^�̓��� */
void lan_out(unsigned, UB);     /* �o�C�g�f�[�^�̏o�� */
UH lan_inw(unsigned);           /* ���[�h�f�[�^�̓��� */
void lan_outw(unsigned, UH);    /* ���[�h�f�[�^�̏o�� */
#endif
ER lan_def_int(void);           /* �����ݐݒ� */

/* �O���[�o���ϐ� */

UB lan_loopback_test;           /* ���[�v�o�b�N�e�X�g */
UB lan_inter_nouse;             /* �����ݎg��Ȃ� */

/* �����ϐ� */

static ID tx_wtskid;            /* ���M�҂��^�X�NID */
static ID rx_wtskid;            /* ��M�҂��^�X�NID */
static UB align_byte;

/* �f�o�b�O�p */

#ifndef PKTCHK     /* ��M�p�P�b�g�X�e�[�^�X���`�F�b�N����(1)/���Ȃ�(0) */
#define PKTCHK  0  /* ACPT BAD PKT�r�b�g��0�Ȃ̂ňُ�p�P�b�g�͎�M������ */
#endif             /* ���Ȃ����A��M�p�P�b�g�X�e�[�^�X���`�F�b�N����ꍇ'1' */

#if PKTCHK
int lan_error_count1;           /* ��M�f�[�^���ُ�� */
int lan_error_count2;           /* ��M�p�P�b�g�X�e�[�^�X�ُ�� */
int lan_error_count3;           /* ��M�I�[�o�[�t���[�x���� */
int lan_error_count4;           /* GOOD PKT��CRC�G���[�������̎����p */
#endif

/*****************************************************************************
* ��ʉ��ʃo�C�g����ւ�
*
******************************************************************************/

static UH swap(UH c)
{
    return (UH)((c >> 8) | (c << 8));
}

/*****************************************************************************
* LAN �R���g���[��������
*
******************************************************************************/

ER lan_ini(UB *macaddr)
{
    UB c;
    int i;

    /* �R���g���[���P���W�X�^ */

    c =  DLCR6_NENA_DLC                 /* �C�j�V�����C�Y */
        |DLCR6_100NS
     /* |DLCR6_SB */                    /* �V�X�e���f�[�^�o�X8�r�b�g */
        |DLCR6_BB                       /* �o�b�t�@�������f�[�^�o�X8�r�b�g */
     /* |DLCR6_TX_BUF_SIZE1 */          /* ���M�o�b�t�@�T�C�Y 2k�o�C�g */
     /* |DLCR6_TX_BUF_SIZE0 */
        |DLCR6_BUF_SIZE1                /* �o�b�t�@�������T�C�Y 32k�o�C�g */
     /* |DLCR6_BUF_SIZE0 */
        ;
    lan_out(DLCR6, c);

    /* �R���g���[���Q���W�X�^ */

    c = 0x00
     /* |DLCR7_IDENT1 */                /* MB86964 �ł� 01 �Œ�(ذ�޵��) */
     /* |DLCR7_IDENT0 */
        |DLCR7_NSTBY                    /* �X�^���o�C��ԂłȂ� */
        |DLCR7_RBS1                     /* ���W�X�^�o���N2�iBMPR8�`15�j*/
     /* |DLCR7_RBS0 */
     /* |DLCR7_EOP */                   /* EOP ���̓A�N�e�B�u Low */
     /* |DLCR7_BYTE_SWAP */             /* �o�C�g�X���b�v */
        ;
    lan_out(DLCR7, c);

    /* ����M�����݃C�l�[�u�����W�X�^ */

    lan_out(DLCR2, 0x00);               /* ���M�����݋֎~ */
    lan_out(DLCR3, 0x00);               /* ��M�����݋֎~ */

    /* ����M�X�e�[�^�X���W�X�^ */

    lan_out(DLCR0, 0xff);               /* ���M�X�e�[�^�X�N���A */
    lan_out(DLCR1, 0xff);               /* ��M�X�e�[�^�X�N���A */

    /* ���M���[�h���W�X�^ */

    if (lan_loopback_test)
        lan_out(DLCR4, 0);              /* �������[�v�o�b�N���[�h */
    else
        lan_out(DLCR4, DLCR4_NLBC);     /* �ʏ푗�M���[�h */

    /* ��M���[�h���W�X�^ */

    if (macaddr[0] == 0 && macaddr[1] == 0
     && macaddr[2] == 0 && macaddr[3] == 0
     && macaddr[4] == 0 && macaddr[5] == 0)
        c = DLCR5_AM1|DLCR5_AM0;        /* �A�h���X�}�b�`���[�h 11 */
    else
        c = DLCR5_AM0;                  /* �A�h���X�}�b�`���[�h 10 */
    lan_out(DLCR5, c);

    /* ���W�X�^�o���N0 �ɐؑ� */

    c = (UB)lan_in(DLCR7);
    c &= ~(DLCR7_RBS1|DLCR7_RBS0);
    lan_out(DLCR7, c);

    /* �m�[�h ID ��ݒ� */

    for (i = 0; i < 6; i++)
    {   c = macaddr[i];
        lan_out(DLCR8 + i, c);
    }

    /* ���W�X�^�o���N2 �ɐؑ� */

    c = (UB)lan_in(DLCR7);
    c &= ~(DLCR7_RBS1|DLCR7_RBS0);
    c |= DLCR7_RBS1;
    lan_out(DLCR7, c);

    /* 16��R���W�����R���g���[�����W�X�^ */

    c = 0x00
        |BMPR11_16COL_CNT2              /* �����I�ɍđ��M�iAuto Mode�j*/
        |BMPR11_16COL_CNT1
     /* |BMPR11_16COL_CNT0 */
        ;
    lan_out(BMPR11, c);

    /* ���̑� */

    lan_out(BMPR12, 0x00);
    lan_out(BMPR14, 0x00);
    lan_out(BMPR15, 0xfa);

    /* ���싖�� */

    c = (UB)lan_in(DLCR6);
    c &= ~DLCR6_NENA_DLC;
    lan_out(DLCR6, c);

    /* �����ݒ�` */

    if (!lan_inter_nouse)
        lan_def_int();
    return E_OK;
}

/*****************************************************************************
* ����M�����݃n���h��
*
******************************************************************************/

void lan_intr(void)
{
    UB c;

    /* ��M�����݂� ? */

    c = lan_in(DLCR1) & lan_in(DLCR3);
    if (c & PKT_RDY)
    {   if (rx_wtskid)
        {   wup_tsk(rx_wtskid);         /* IP��M�^�X�N���N�� */
            rx_wtskid = FALSE;
            lan_out(DLCR3, 0x00);       /* ��M�����݋֎~ */
        }
    }

    /* TMT OK �� ? */

    c = lan_in(DLCR0) & lan_in(DLCR2);
    if (c & DLCR0_TMT_OK)
    {   if (tx_wtskid)
        {   wup_tsk(tx_wtskid);         /* IP���M�^�X�N���N�� */
            tx_wtskid = FALSE;
            lan_out(DLCR2, 0x00);       /* ���M�����݋֎~ */
        }
    }
}

/*****************************************************************************
* ��M�����ݑ҂�
*
******************************************************************************/

ER lan_wai_rcv(TMO tmout)
{
    ER ercd = E_OK;
    UB c;
    UINT psw;

    if (lan_inter_nouse)
    {   for (;;)
        {   for (;;)
            {   c = lan_in(DLCR1);
                if (c & (PKT_RDY|OVRFLO))
                    break;
                if (tmout != TMO_FEVR && tmout-- == 0)
                    return E_TMOUT;
                tslp_tsk(1);
            }
            c = lan_in(DLCR5);
            if (!(c & DLCR5_BUF_EMP))
            {   lan_out(DLCR1, PKT_RDY);    /* PKT RDY �r�b�g�N���A */
                return E_OK;
            }
            lan_out(DLCR1, 0xff);           /* �S�X�e�[�^�X�r�b�g�N���A */
        }
    }

    psw = vdis_psw();

    for (;;)
    {   c = lan_in(DLCR5);
        if (!(c & DLCR5_BUF_EMP))       /* BUF EMP = 0 �ƂȂ�܂� */
            break;
        rx_wtskid = vget_tid();
        vcan_wup();
        lan_out(DLCR3, 0x80);           /* ��M�����݋��� */
        vset_psw(psw);
        ercd = tslp_tsk(tmout);         /* ��M�����݂���̋N���҂� */
        psw = vdis_psw();
        rx_wtskid = FALSE;
        lan_out(DLCR3, 0x00);           /* ��M�����݋֎~ */
        lan_out(DLCR1, 0xff);           /* �S�X�e�[�^�X�r�b�g�N���A */
        if (ercd != E_OK)               /* �^�C���A�E�g�Ȃ� */
            break;                      /* �G���[�I�� */
    }
    vset_psw(psw);
    return ercd;
}

/*****************************************************************************
* ���M�����ݑ҂�
*
******************************************************************************/

ER lan_wai_snd(TMO tmout)
{
    ER ercd = E_OK;
    UB c;
    UINT psw;
    static UB i;

    if (i == 0)         /* �ŏ����� TMT OK ���Ȃ� */
    {   i = 1;
        return E_OK;
    }

    if (lan_inter_nouse)
    {   c = lan_in(DLCR0);
        if (c & DLCR0_TMT_OK)
        {   lan_out(DLCR0, DLCR0_TMT_OK);   /* TMT OK = 0 */
            return E_OK;
        }
        if (tmout != TMO_FEVR && tmout-- == 0)
            return E_TMOUT;
        tslp_tsk(1);
    }

    psw = vdis_psw();

    for (;;)
    {   c = lan_in(DLCR0);
        if (c & DLCR0_TMT_OK)
        {   lan_out(DLCR0, DLCR0_TMT_OK);   /* TMT OK = 0 */
            break;
        }
        tx_wtskid = vget_tid();
        vcan_wup();
        lan_out(DLCR2, 0x80);           /* ���M�����݋��� */
        vset_psw(psw);
        ercd = tslp_tsk(tmout);         /* ���M�����݂���̋N���҂� */
        psw = vdis_psw();
        tx_wtskid = FALSE;
        lan_out(DLCR2, 0x00);           /* ���M�����݋֎~ */
        if (ercd != E_OK)               /* �^�C���A�E�g�Ȃ� */
            break;                      /* �G���[�I�� */
    }
    vset_psw(psw);
    return ercd;
}

/*****************************************************************************
* ��M�p�P�b�g���𓾂�
*
******************************************************************************/

ER lan_get_len(UH *len)
{
    UH c, sts;
    int n, i;

    /* ��M�p�P�b�g�X�e�[�^�X�ǂݏo�� */

    c = lan_inw(BMPR8);
    sts = (UH)(swap(c) & 0x00ff);

    /* ��M�f�[�^����ǂݏo�� */

    c = lan_inw(BMPR8);
    n = swap(c);
    *len = n;
    align_byte = (UB)((8 - ((n + 4) & 7)) & 7);

    /* ��M�p�P�b�g�X�e�[�^�X�ƃf�[�^�����`�F�b�N����ꍇ */

 #if PKTCHK
    if ((n & 0xf800) != 0)              /* �f�[�^�����5�r�b�g�͏��0 */
    {   lan_error_count1++;
        goto NG;
    }
    if (!(sts & GOOD_PKT))
    {   lan_error_count2++;
        goto NG;
    }
    if (sts & OVRFLO)                   /* OVRFLO�͌x��, ����PKT�͐��� */
        lan_error_count3++;
    if (sts & ~(GOOD_PKT|OVRFLO))       /* CRC ERR�������Ƃ����邪���� */
        lan_error_count4++;
    goto OK;

    /* ����p�P�b�g�łȂ��ꍇ�A�ǂݔ�΂��B */

  NG:
    c = lan_inw(BMPR8);
    c = lan_inw(BMPR8);                 /* ���Ȃ��Ƃ�4�񃊁[�h���� SKIP */
    n -= 4;
    if (n <= 8)                         /* �c��8�o�C�g�ȉ��ł� SKIP �֎~ */
    {   n += align_byte;                /* 8�o�C�g���E�܂ŋ�ǂ݂̕K�v���� */
        for (i = 0; i < n; i += 2)
            c = lan_inw(BMPR8);
    }
    else
        lan_out(BMPR14, BMPR14_SKIP_RX_PKT);
    return -1;
  OK:
 #endif
    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݏo��
*
* buf   �ǂݏo�����߂̃o�b�t�@
* len   �ǂݏo���o�C�g��
******************************************************************************/

ER lan_get_pkt(void *buf, int len)
{
    UH *p;
    int i;
    UH c;

    p = (UH *)buf;
    for (i = 0; i < len; i += 2)
    {   c = lan_inw(BMPR8);
        *p++ = c;
    }
    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݏo���I��
*
******************************************************************************/

ER lan_get_end()
{
    /* �f�[�^��8�r�b�g�A�N�Z�X�̏ꍇ�A����ɑ΂���1�o�C�g�̃_�~�[���[�h��
       �s���Ă��������B*/

    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݔ�΂�
*
* len   ��M�p�P�b�g�̖��ǃo�C�g��
******************************************************************************/

ER lan_skp_pkt(int len)
{
    int i;
    UH c;

    if (len <= 8)                       /* �c��8�o�C�g�ȉ��ł� SKIP �֎~ */
    {   len += align_byte;              /* 8�o�C�g���E�܂ŋ�ǂ݂̕K�v���� */
        for (i = 0; i < len; i += 2)
            c = lan_inw(BMPR8);
    }
    else
        lan_out(BMPR14, BMPR14_SKIP_RX_PKT);
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g����ݒ�
*
******************************************************************************/

ER lan_set_len(int len)
{
    lan_outw(BMPR8, swap(len));
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g��������
*
* data      �������ރf�[�^
* len       �f�[�^�̃o�C�g���i> 0�j
******************************************************************************/

ER lan_put_pkt(const void *data, int len)
{
    const UH *p;
    UH c;
    int i;

    p = (const UH *)data;
    for (i = 0; i < len; i += 2)
    {   c = *p++;
        lan_outw(BMPR8, c);
    }
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g��60�o�C�g�����̏ꍇ�̃_�~�[��������
*
* len       �_�~�[�f�[�^�̃o�C�g���i> 0�j
******************************************************************************/

ER lan_put_dmy(int len)
{
    int i;

    for (i = 0; i < len; i += 2)
        lan_outw(BMPR8, 0);
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g�������ݏI��
*
******************************************************************************/

ER lan_put_end(void)
{
    lan_out(BMPR10, 0x81);                  /* TMST = 1, TX PKT CNT = 1 */
    return E_OK;
}

/* end */
