/****************************************************************************
* SHC�����ݒ胋�[�`���̗�iSH�j                                             *
*                                                                           *
*  File name : initsh.c                                                     *
*                                                                           *
* 24/May/1997 �쐬                                                          *
* 06/Dec/1999 -fpu=double�I�v�V�����ɑΉ�                             MiSPO *
****************************************************************************/

#include "machine.h"

extern int *_D_ROM, *_D_BGN, *_D_END, *_B_BGN, *_B_END;
extern void _INITSCT(void);
extern void main(void);
#pragma noregsave(_INIT)

void _INIT(void)
{
    _INITSCT();
#ifdef _FPD
    set_fpscr(((int)get_fpscr()&~0x00100000)|0x00080000);
#endif
    main();
    for (;;)
        ;
}

void _INITSCT(void)
{
    int *p, *q;

    for (p = _B_BGN; p < _B_END; p++)
        *p = 0;

    for (p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++)
        *p = *q;
}

/* end */
