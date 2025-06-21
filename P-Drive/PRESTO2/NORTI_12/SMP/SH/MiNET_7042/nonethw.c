/******************************************************************************
* NORTi Network User's Hardware-dependent Functions (MiNET-MB86964)           *
*                                                                             *
*                                                                             *
* 23/Nov/1998                                                                 *
* 01/May/1999 lan_int �� mb86964.c ����ړ�                                   *
* 12/Sep/2001 ����n�̔��f��������                                         TA *
* 24/Dec/2002 SH7042/44/45�ŋ��ʂ̂���sh7042.h->sh7040.h�ɂ���             KA *
******************************************************************************/

#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "sh7040.h"

void lan_intr(void);

/* �����ݗD�惌�x�� */

#define IP          7

/* �����݃n���h����`��� */

const T_DINT dint_lan = { TA_HLNG, lan_int };

/***************************************************************************
* LAN �R���g���[���̊����ݐݒ�
*
* MB86964 /INT -> SH7040 /IRQ3
***************************************************************************/

ER lan_def_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_IRQ3, &dint_lan);    /* �x�N�^�Z�b�g */

    psw = vdis_psw();
    sfr_setw(PFC_PACRL1,0x0008);    /* /IRQ3 LAN ���荞�݃|�[�g������ */
    sfr_setw(INTC_ICR, 0x0100);     /* Low���x���Ŋ����� */
    sfr_setw(INTC_IPRA, IP);        /* �����ݗD�惌�x�� */
    vset_psw(psw);

    return ercd;
}

/***************************************************************************
* LAN �R���g���[�������݃n���h��
*
***************************************************************************/

#if defined(__HITACHI__)            /* Hitach C */
#pragma interrupt(lan_int)
#endif

INTHDR lan_int(void)
{
    ent_int();
    lan_intr();
    ret_int();
}

/* end */
