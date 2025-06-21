/******************************************************************************
* EEPROM ���o��                                                               *
*                                                                             *
* 24/Mar/1998 �쐬                                                   MiSPO MI *
* 30/Nov/1998 MiNET-7042 �p�ɏC��                                          MI *
* 26/May/1998 ini_eeprom ��ǉ�                                            MI *
* 07/Mar/2001 ����Flash��������p��wai_5us���C��,�ǉ�                      MI *
* 24/Dec/2002 SH7042/44/45�ŋ��ʂ̂���sh7042.h->sh7040.h�ɂ���             KA *
******************************************************************************/

#include "norti3.h"
#include "sh7040.h"
#include "net7042.h"

/* EEPROM I/O �̃r�b�g */

#define EEP_SDA_BIT     0x0010
#define EEP_SCL_BIT     0x0020

/* ���o�̓}�N�� */

#define SDA_IN()        ((sfr_inw(IO_PEDR) & EEP_SDA_BIT) != 0)
#define SCL_HIGH()      sfr_setw(IO_PEDR, EEP_SCL_BIT)
#define SCL_LOW()       sfr_clrw(IO_PEDR, EEP_SCL_BIT)
#define SDA_HIGH()      sfr_setw(IO_PEDR, EEP_SDA_BIT)
#define SDA_LOW()       sfr_clrw(IO_PEDR, EEP_SDA_BIT)

#define SDA_DIR_IN()    sfr_clrw(PFC_PEIOR,EEP_SDA_BIT)
#define SDA_DIR_OUT()   sfr_setw(PFC_PEIOR,EEP_SDA_BIT)
#define SCL_DIR_OUT()   sfr_setw(PFC_PEIOR,EEP_SCL_BIT)

/*****************************************************************************/
/* 5��sec�ȏ�̑҂�                                                          */
/*                                                                           */
/* EEPROM �N���b�N�^�C�~���O���Ƃ邽�߁A5��sec �ȏ�̎��Ԃ�ׂ��B            */
/* CPU ���x�Ɉˑ�����̂ŁA�_�~�[�̎��s���ߐ��͒������K�v�B                  */
/*****************************************************************************/

static int wai_5us(void)
{
    int i;
    UH r;

    for (i = 0; i < 20; i++)
        r = *(volatile UH *)0xce0000;   /* ��L���b�V���̋�Ԃ��A�N�Z�X */
    return r;
}

/*****************************************************************************/
/* 10msec�ȏ�̑҂�                                                          */
/*                                                                           */
/* EEPROM �������݃^�C�~���O���Ƃ邽�߁A10msec �ȏ�̎��Ԃ�ׂ��B            */
/*****************************************************************************/

#define wait_10ms() dly_tsk(10/MSEC+1)

/*****************************************************************************/
/* �V���A���d�d�o�q�n�l�|�[�g������                                          */
/*                                                                           */
/* ������Ԃ� SCL=1 �o�͕���, SDA ���͕����Ƃ���B                           */
/* ��ɐ����� TRUE ��Ԃ��B                                                  */
/*****************************************************************************/

BOOL ini_eeprom(void)
{
    SCL_HIGH();
    SCL_DIR_OUT();
    SDA_DIR_IN();
    return TRUE;
}

/*****************************************************************************/
/* �V���A���d�d�o�q�n�l�P�o�C�g��������                                      */
/*                                                                           */
/* EEPROM �ɂP�o�C�g�̃f�[�^���������ށB                                     */
/* �������ݐ����� TRUE�A���s�� FALSE ��Ԃ��B                                */
/*****************************************************************************/

BOOL write_eeprom(UH addr,  /* EEPROM �̃A�h���X */
                  UB data)  /* �������ރf�[�^ */
{
    UB c;
    int t;

    /* START */ /* ������Ԃ� SCL=1 �o�͕���, SDA ���͕����Ƃ��� */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                           /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 */

    SDA_DIR_OUT();                          /* SDA �o�͕��� */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                           /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA WRITE */

    SDA_DIR_OUT();                          /* SDA �o�͕��� */
    c = data;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                      /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* STOP */

    SDA_LOW();
    SDA_DIR_OUT();                          /* SDA �o�͕��� */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();

    wait_10ms();                            /* tWR �҂�     */
    SDA_DIR_IN();                           /* SDA ���͕��� */
    return TRUE;
}

/*****************************************************************************/
/* �V���A���d�d�o�q�n�l�P�o�C�g�ǂݏo��                                      */
/*                                                                           */
/* EEPROM ����P�o�C�g�̃f�[�^��ǂݏo���B                                   */
/* �ǂݏo�������� TRUE�A���s�� FALSE ��Ԃ��B                                */
/*****************************************************************************/

BOOL read_eeprom(UH addr,       /* EEPROM �̃A�h���X */
                 UB *data)      /* �ǂݏo�����f�[�^ */
{
    UB c;
    int t;

    /* START (dummy write ) */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 (dummy write) */

    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* START */

    wai_5us();
    SCL_HIGH();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SDA_LOW();
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 1);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA READ */

    c = 0;
    wai_5us(); if (SDA_IN()) c |= 0x80; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x40; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x20; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x10; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x08; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x04; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x02; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x01; SCL_HIGH(); wai_5us(); SCL_LOW();
    *data = c;

    /* STOP */

    SCL_HIGH();
    wai_5us();
    SCL_LOW();

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    return TRUE;
}

/*****************************************************************************/
/* �V���A���d�d�o�q�n�l�y�[�W��������                                        */
/*                                                                           */
/* EEPROM �ɘA���P�U�o�C�g�P�ʂŃf�[�^���������ށB                           */
/* �P�U�o�C�g�������w�肵���ꍇ�́A�c��͂O���������ށB                      */
/* �������ݐ����� TRUE�A���s�� FALSE ��Ԃ��B                                */
/*****************************************************************************/

BOOL page_write_eeprom(UH addr,  /* EEPROM �̃A�h���X */
                       UB *buf,  /* �������ރf�[�^�ւ̃|�C���^ */
                       INT size)/* �������ރf�[�^�̃o�C�g�� */
{
    INT i, t, n;
    UB c;

    while (size > 0) {
        if (size >= 16)
            n = 16;
        else
            n = size;
        size -= n;

        /* START */ /* ������Ԃ� SCL=1 �o�͕���, SDA ���͕����Ƃ��� */

        SDA_LOW();
        SDA_DIR_OUT();                      /* SDA �o�͕��� */
        wai_5us();
        SCL_LOW();

        /* Device Type ID | Address A10-A8 | R/W */

        c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();                           /* SDA ���͕��� */

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();

        /* Address , A7-A0 */

        SDA_DIR_OUT();                          /* SDA �o�͕��� */
        c = (UB)(addr & 0xff) ;
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();                           /* SDA ���͕��� */

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();

        /* DATA WRITE */

        i = 0;
L1:
        SDA_DIR_OUT();                          /* SDA �o�͕��� */
        if (i < n)
            c = *buf++;
        else
            c = 0x00;
        wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
        SDA_HIGH();
        SDA_DIR_IN();

        /* ACK */

        for (t = 1000; t != 0; t--)
        {   if (!SDA_IN())                      /* SDA = 0 ? */
                break;
        }
        SCL_HIGH();
        wai_5us();
        if (t == 0)
            return FALSE;
        SCL_LOW();
        if (++i < 16)
            goto L1;

        /* STOP */

        SDA_LOW();
        SDA_DIR_OUT();                          /* SDA �o�͕��� */
        wai_5us();
        SCL_HIGH();
        wai_5us();
        SDA_HIGH();

        wait_10ms();                            /* tWR �҂�     */
        SDA_DIR_IN();                           /* SDA ���͕��� */

        addr += 16;
    }
    return TRUE;
}

/*****************************************************************************/
/* �V���A���d�d�o�q�n�l�A���o�C�g�ǂݏo��                                    */
/*                                                                           */
/* EEPROM ���畡���o�C�g�̃f�[�^��ǂݏo���B                                 */
/* �ǂݏo�������� TRUE�A���s�� FALSE ��Ԃ��B                                */
/*****************************************************************************/

BOOL seq_read_eeprom(UH addr,  /* EEPROM �̃A�h���X */
                     UB *buf,  /* Read�f�[�^�ւ̃|�C���^ */
                     INT size) /* Read�f�[�^�̃o�C�g�� */
{
    INT i, t;
    UB c;

    /* START (dummy write) */

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 0);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* Address , A7-A0 (dummy write) */

    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    c = (UB)(addr & 0xff) ;
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* START */

    wai_5us();
    SCL_HIGH();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SDA_LOW();
    wai_5us();
    SCL_LOW();

    /* Device Type ID | Address A10-A8 | R/W */

    c = (UB)(0xa0 | (UB)((addr >> 7) & 0x0e) | 1);
    wai_5us(); if (c & 0x80) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x40) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x20) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x10) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x08) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x04) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x02) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (c & 0x01) SDA_HIGH(); else SDA_LOW(); SCL_HIGH(); wai_5us(); SCL_LOW();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    /* ACK */

    for (t = 1000; t != 0; t--)
    {   if (!SDA_IN())                  /* SDA = 0 ? */
            break;
    }
    SCL_HIGH();
    wai_5us();
    if (t == 0)
        return FALSE;
    SCL_LOW();

    /* DATA READ */
    i = 0;
L1:
    c = 0;
    wai_5us(); if (SDA_IN()) c |= 0x80; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x40; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x20; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x10; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x08; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x04; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x02; SCL_HIGH(); wai_5us(); SCL_LOW();
    wai_5us(); if (SDA_IN()) c |= 0x01; SCL_HIGH(); wai_5us(); SCL_LOW();
    *buf++ = c;
    if (++i >= size)
        goto STOP;

    /* SEND ACK */
    wai_5us();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    SDA_LOW();
    wai_5us();
    SCL_HIGH();
    wai_5us();
    wai_5us();
    SCL_LOW();
    SDA_DIR_IN();
    SDA_HIGH();
    goto L1;

    /* STOP */
STOP:
    SCL_HIGH();
    wai_5us();
    SCL_LOW();

    SDA_LOW();
    SDA_DIR_OUT();                      /* SDA �o�͕��� */
    wai_5us();
    SCL_HIGH();
    wai_5us();
    SDA_HIGH();
    SDA_DIR_IN();                       /* SDA ���͕��� */

    return TRUE;
}

/* end */
