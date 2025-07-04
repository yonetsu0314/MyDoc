/******************************************************************************
* 日立Ｃ初期設定ルーチンの例（SH）                                            *
*                                                                             *
*  File name : initsh.c                                                       *
*                                                                             *
* 97-05-24 作成                                                         MiSPO *
******************************************************************************/

extern int *_D_ROM, *_D_BGN, *_D_END, *_B_BGN, *_B_END;
extern void _INITSCT(void);
extern void main(void);
#pragma noregsave(_INIT)

void _INIT(void)
{
    _INITSCT();
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
