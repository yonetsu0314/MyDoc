/******************************************************************************
* MiNET-7042 �{�[�h �t���b�V���������i29F200T ���[�h���[�h�j                  *
*                                                                             *
*                                                                             *
* 14/Oct/1997 �쐬                                                            *
* 30/Nov/1998 MiNET-7042 �p�ɏC��                                       MiSPO *
* 24/Dec/2002 SH7042/44/45�ŋ��ʂ̂���sh7042.h->sh7040.h�ɂ���             KA *
******************************************************************************/

#include "norti3.h"
#include "sh7040.h"
#include "net7042.h"

UW Flush = FLUSH;

/* �}�N���֐� */

#define flash_dat(addr,c)   (*((volatile UH *)Flush + addr) = c)
#define flash_cmd(addr,c)   (*((volatile UH *)Flush + addr) = c)
#define flash_sts(addr)     (*((volatile UH *)Flush + addr))

#if defined(NEW)
static int nop(int n) { while (n-- >= 0) ; return n; }
#endif


/*****************************************************************************
* �t���b�V���������h�c���[�h
*
******************************************************************************/

BOOL flash_id(UH *maker, UH *device)
{
    /* �R�}���h�V�[�P���X */

    flash_cmd(0x0555, 0x00aa);
    flash_cmd(0x02aa, 0x0055);
    flash_cmd(0x0555, 0x0090);
    *maker = flash_sts(0x0000);
    *device = flash_sts(0x0002);

    /* ���Z�b�g */

    flash_cmd(0x0000, 0x00f0);
    return TRUE;
}

/*****************************************************************************
* �t���b�V������������������iFF �ƂȂ�j
*
* ����  BOOL flash_erase(UW addr, W size);
*
* ����  addr    �t���b�V���������̐擪�� 0 �Ƃ����A�h���X
*       size    ��������f�[�^�̃o�C�g��
*
* �ߒl  TREU:����, FALSE:���s
*
* �L��  �u���b�N�P�ʂł��������ł��Ȃ��B���Ȃ킿�C�ӂ̃A�h���X����C�ӂ�
*       �T�C�Y�������������邱�Ƃ͂ł��Ȃ��B
*       �A�h���X�ƃT�C�Y�͉��\�ɏ]�����ƁB�����u���b�N�ɂ܂������Ă��悢�B
*
*         �A�h���X    �T�C�Y
*         0x00000�`   0x10000
*         0x10000�`   0x10000
*         0x20000�`   0x10000
*         0x30000�`   0x08000
*         0x38000�`   0x02000
*         0x3a000�`   0x02000
*         0x3c000�`   0x04000
******************************************************************************/

BOOL flash_erase(UW addr, W size)
{
    UW end;
    UH c;

    if (size & 1)
        return FALSE;

    end = (addr + size) / 2 - 1;
    addr /= 2;

    /* �R�}���h�V�[�P���X */

    flash_cmd(0x0555, 0xaa);
    flash_cmd(0x02aa, 0x55);
    flash_cmd(0x0555, 0x80);
    flash_cmd(0x0555, 0xaa);
    flash_cmd(0x02aa, 0x55);
    if (addr < 0x08000)
        flash_cmd(0x00000, 0x0030);
    if (addr < 0x10000 && end >= 0x08000)
        flash_cmd(0x08000, 0x0030);
    if (addr < 0x18000 && end >= 0x10000)
        flash_cmd(0x10000, 0x0030);
    if (addr < 0x1c000 && end >= 0x18000)
        flash_cmd(0x18000, 0x0030);
    if (addr < 0x1d000 && end >= 0x1a000)
        flash_cmd(0x1c000, 0x0030);
    if (addr < 0x1e000 && end >= 0x1c000)
        flash_cmd(0x1d000, 0x0030);
    if (addr < 0x20000 && end >= 0x1e000)
        flash_cmd(0x1e000, 0x0030);

    /* DQ7 /DATA �|�[�����O */

    for (;;)
    {   c = flash_sts(addr);
        if ((c & 0x80) != 0)    /* DQ7 = 1 ? */
            break;
        if ((c & 0x20) == 0)    /* DQ5 = 0 ? */
            continue;
        c = flash_sts(addr);
        if ((c & 0x80) != 0)    /* DQ7 = 1 ? */
            break;
        flash_cmd(0x0000, 0x00f0);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
* �t���b�V�����������v���O��������
*
* ����  BOOL flash_program(UW addr, void *data, W size);
*
* ����  addr    �t���b�V���������̐擪�� 0 �Ƃ����A�h���X�i�����j
*       data    �v���O��������f�[�^�ւ̃|�C���^�i�����j
*       size    �v���O��������f�[�^�̃o�C�g���i�����j
*
* �ߒl  TRUE:����, FALSE:���s
*
* �L��  �����ł���΁A�C�ӂ̃A�h���X����A�C�ӂ̃T�C�Y�Ńv���O�����\�B
*       ���Ȃ킿�A�����ȃT�C�Y�ŉ��x���J��Ԃ��Ăяo���\�B
*       �{�֐��Ńv���O��������O�ɏ������K�v�B
******************************************************************************/

BOOL flash_program(UW addr, void *data, W size)
{
    UW end;
    UH *p;
    UH c, cc;

    if ((UW)data & 1)
        return FALSE;
    if (size & 1)
        return FALSE;

    p = (UH *)data;
    end = (addr + size) / 2;
    addr /= 2;

    for (; addr < end; addr += 1, p++)
    {
        if (*p == 0xffff)
            continue;

        /* �R�}���h�V�[�P���X */

        flash_cmd(0x0555, 0x00aa);
        flash_cmd(0x02aa, 0x0055);
        flash_cmd(0x0555, 0x00a0);
        flash_dat(addr, *p);

        /* DQ7 /DATA �|�[�����O */

        cc = *(UB *)p;
        for (;;)
        {
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            if ((c & 0x20) == 0)        /* DQ5 = 0 ? */
                continue;
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            flash_cmd(0x0000, 0x00f0);
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************
* �t���b�V�����������N���A����i00 �Ƀv���O��������j
*
* ����  BOOL flash_clear(UW addr, W size);
*
* ����  addr    �t���b�V���������̐擪�� 0 �Ƃ����A�h���X�i�����j
*       data    �v���O��������f�[�^�ւ̃|�C���^�i�����j
*
* �ߒl  TREU:����, FALSE:���s
*
* �L��  �����ł���΁A�C�ӂ̃A�h���X����A�C�ӂ̃T�C�Y�Ńv���O�����\�B
*       ���Ȃ킿�A�����ȃT�C�Y�ŉ��x���J��Ԃ��Ăяo���\�B
*       �{�֐��Ńv���O��������O�ɏ������K�v�B
******************************************************************************/

BOOL flash_clear(UW addr, W size)
{
    UW end;
    UH c, cc;

    if (size & 1)
        return FALSE;

    end = (addr + size) / 2;
    addr /= 2;

    for (; addr < end; addr += 1)
    {
        /* �R�}���h�V�[�P���X */

        flash_cmd(0x0555, 0x00aa);
        flash_cmd(0x02aa, 0x0055);
        flash_cmd(0x0555, 0x00a0);
        flash_dat(addr, 0);

        /* DQ7 /DATA �|�[�����O */

        cc = 0;
        for (;;)
        {
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            if ((c & 0x20) == 0)        /* DQ5 = 0 ? */
                continue;
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            flash_cmd(0x0000, 0xf0);
            return FALSE;
        }
    }
    return TRUE;
}

/* end */
