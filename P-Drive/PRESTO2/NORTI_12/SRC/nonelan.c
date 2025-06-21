/******************************************************************************
* NORTi Network LAN Driver Caller                                             *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 28/Feb/1999                                                                 *
* 13/Jun/2001 �G���[�\����ǉ�                                             MI *
* 28/Nov/2002 SNMP�Ή������𔽉f                                           MI *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"

/* �p�P�b�g�\�����[�`�� */

#ifdef DUMP
void landump_len(VP *b, int len);
void landump_dat(VP b, const void *data, int len);
void landump_end(VP b);
void landump_msg(char *s);
#else
#define landump_len(b,l)
#define landump_dat(b,d,l)
#define landump_end(b)
#define landump_msg(s)
#endif

/* �����ϐ� */

static UH received_len;
VP rcv_dump;
VP snd_dump;

/***********************************************************************
* LAN �h���C�o�̃G���[����
*
* LAN �h���C�o�֐����G���[��Ԃ����Ƃ��ɁA���̊֐����Ă΂�܂��B
* �W���ł͉����������Ă��܂���B
************************************************************************/

ER lan_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

ER lan_errors(char *s, ER ercd)
{
    landump_msg(s);
    return tcpip_internal_error(ercd);
}

/***********************************************************************
* ��M�p�P�b�g���𓾂�
*
* ��M���� Ethernet �t���[���̃o�C�g����ߒl�Ƃ��ĕԂ��܂��B
* LAN �h���C�o�� lan_get_len �֐����Ăяo���Ă��܂��B
************************************************************************/

UH lan_received_len(void)
{
    ER ercd;

    ercd = lan_get_len(&received_len);
    if (ercd != E_OK)
    {   lan_errors((char*)"lan_get_len error\r\n", ercd);
        received_len = 0;
    }

    landump_len(&rcv_dump, received_len);
    SNMP_BYPCNT(snmp_mibbyp_if[0].ifInOctets, received_len);   /* ifInOctets */
    return received_len;
}

/***********************************************************************
* ��M�p�P�b�g�ǂݏo��
*
* ��M���� Ethernet �t���[�����w��o�C�g�������ǂݏo���܂��B
* LAN �h���C�o�� lan_get_pkt �֐����Ăяo���Ă��܂��B
************************************************************************/

BOOL lan_read_pkt(void *buf, int len)
{
    ER ercd;

    if (received_len == 0)
        return FALSE;

    if (len <= (int)received_len)
    {   ercd = lan_get_pkt(buf, len);
        if (ercd != E_OK)
        {   lan_errors((char*)"lan_get_pkt error", ercd);
            received_len = 0;
            landump_end(rcv_dump);
            return FALSE;
        }
        landump_dat(rcv_dump, buf, len);
        if ((received_len -= len) == 0)
        {   lan_get_end();
            landump_end(rcv_dump);
        }
        return TRUE;
    }
    else
    {   ercd = lan_get_pkt(buf, received_len);
        if (ercd != E_OK)
            lan_errors((char*)"lan_get_pkt error", ercd);
        landump_dat(rcv_dump, buf, received_len);
        received_len = 0;
        lan_get_end();
        landump_end(rcv_dump);
        return FALSE;
    }
}

/***********************************************************************
* ��M�p�P�b�g�j��
*
************************************************************************/

void lan_ignore_pkt(void)
{
    ER ercd;

    if (received_len != 0)
    {   ercd = lan_skp_pkt(received_len);
        if (ercd != E_OK)
            lan_errors((char*)"lan_skp_pkt error", ercd);
        received_len = 0;
    }
    landump_end(rcv_dump);
}

/***********************************************************************
* ��M�p�P�b�g�����܂œǂݏo��
*
************************************************************************/

BOOL lan_read_pkt_end(void *buf, int len, int bufsz)
{
    /* len �����Ȃ�ُ� */

    if ((int)received_len < len)
    {   SNMP_BYPCNT(snmp_mibbyp_if[0].ifInErrors, 1);   /* ifInErrors */
        lan_read_pkt(buf, received_len);
        return FALSE;
    }

    /* bufsz ���߂Ȃ�A���镪��j�� */

    if ((int)received_len > bufsz)
    {   if (!lan_read_pkt(buf, bufsz))
            return FALSE;
        lan_ignore_pkt();
        return TRUE;
    }

    /* ����ȓǂݏo�� */

    return lan_read_pkt(buf, received_len);
}

/***********************************************************************
* ���M�p�P�b�g��������
*
* head      �������ރw�b�_
* hlen      �w�b�_�̃o�C�g��
* data      �������ރf�[�^
* dlen      �f�[�^�̃o�C�g��
************************************************************************/

BOOL lan_write_pkt(const void *head, int hlen, const void *data, int dlen)
{
    int i, len;

    /* ���M���i�w�b�_���{�f�[�^���j��ݒ� */

    len = hlen + dlen;
    i = 60 - len;
    if (i > 0)
        len = 60;
    lan_set_len(len);
    landump_len(&snd_dump, len);
    SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutOctets, len);   /* ifOutOctets */

    /* �w�b�_�𑗐M�o�b�t�@�������� */

    if (hlen > 0)
    {   lan_put_pkt(head, hlen);
        landump_dat(snd_dump, head, hlen);
    }

    /* �f�[�^�𑗐M�o�b�t�@�������� */

    if (dlen > 0)
    {   lan_put_pkt(data, dlen);
        landump_dat(snd_dump, data, dlen);
    }

    /* 60�o�C�g�����̏ꍇ�̃_�~�[�������� */

    if (i > 0)
    {   lan_put_dmy(i);
        landump_dat(snd_dump, NULL, i);
    }

    /* ���M */

    lan_put_end();
    landump_end(snd_dump);

    return TRUE;
}

/* end */
