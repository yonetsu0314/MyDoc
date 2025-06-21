/****************************************************************************
* �C���^�[�o�������݃n���h���iMS7750SE01/Computex SH7750 EVA BOD)           *
*                                                                           *
*  File name : n4i7750.c                                                    *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -cp=sh4 -op=1 -def=CLK=?,CH=? n4i7750.c        (Renesas C) *
*            gcc -c -m4 -O2 -DCLK=? -DCH=? n4i7750.c              (exeGCC)  *
*            ccsh -c -ansi -noasmwarn -cpu=sh4 -O                           *
*                                        -DCLK=? -DCH=? n4i7750.c (GHS)     *
*                                                                           *
* 24/Apr/2000                                                               *
* 12/Sep/2000 �J�E���^�l�̌v�Z���C��                                        *
* 05/Seb/2001 v4_def_inh�ŁA�p�����[�^��NULL, NADR�̎��̏������C��          *
* 19/Sep/2001 ����n�̔��f��������                                          *
*             �^�C���X�^���v�p�̃N���b�N�̎��o����ǉ�                 TA *
* 27/Dec/2002 v4_def_inh�� pk_dinh->imask�`�F�b�N�C���ԈႢ���C��        KA *
* 25/Feb/2003 INTVEC �̃f�t�H���g�� 0xa0000000L ���� 0x80000000L �ɕύX  KA *
****************************************************************************/

/*

�œK���R���p�C��

          ���荞�݃n���h���̃R���p�C���ɂ́A�K���A�œK���I�v�V������t���A
          ent_int �Ăяo���O�ɁA�s�v�Ȗ��߂���������Ȃ��悤���ӂ��Ă��������B
          SHC�ł́A�����݃n���h���� #pragma interrupt �Œ�`���Ă��������B

�g�p����n�[�h�E�F�A

          SH7750 ���� TMU �������^�C�}�Ƃ��Ďg�p���Ă��܂��B
          ����ȊO�̃^�C�}��p����ꍇ�́A���[�U�[�ŉ������Ă��������B

�����ݎ����̎w��

          �����ݎ����́AMSEC �}�N���Ō��܂�܂��B�f�t�H���g�� 10 msec �ł��B
          �Ⴆ�΁A5 msec �ɕύX����ꍇ�́A���̗l�ɃR���p�C�����Ă��������B

          (��) shc <options> -def=MSEC=5 n4i7750.c
                                  ~~~~~~
          (��) gcc <options> -DMSEC=5 n4i7750.c
                               ~~~~~~
�N���b�N�̎w��

          �^�C�}�̃\�[�X�N���b�N�i���ӃN���b�NP�Ӂj�f�t�H���g�� 33.3 MHz �ł��B
          �ύX����ꍇ �� CLK �}�N�����`���ăR���p�C�����Ă��������B
          �l�� [Hz] �P�ʂ̎��g���ł��B

          (��) shc <options> -def=CLK=19660800 n4i7750.c ... 19.6608 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=19660800 n4i7750.c
                               ~~~~~~~~~~~~
�`���l���ԍ��̎w��

          CH �}�N���ŁA�g�p���� TMU �`���l�����w��ł��܂��B�iCH=0�`2�j
          �f�t�H���g�� CH=0 �ł��B

          (��) shc <options> -def=CH=1 n4i7750.c  .......... TMU CH1
                                  ~~~~
          (��) gcc <options> -DCH=2 n4i7750.c  ............. TMU CH2
                               ~~~~
���荞�݃n���h���ւ̕���

          ���荞�݂̓��������O�R�[�h�𔻕ʂ��āA�{�t�@�C���̊��荞�݃n���h
          ���֕���ijmp�j����܂ł̃��[�`���́A���[�U�ɂăA�Z���u���ŋL�q����
          ���������B�i�Ⴊ vec7750?.asm �ɂ���܂��j
          ���̃��[�`���ł́A���W�X�^��j�󂵂Ȃ��ŕ��򂵂Ă��������B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=2 n4i7750.c ... �����ݗD�揇�� 2
                                  ~~~~
          (��) gcc <options> -DIP=2 n4i7750.c
                               ~~~~
dis_int, ena_int

          dis_int, ena_int �V�X�e���R�[���͎�������Ă��܂���B
          �ʂ̊����݋֎~�����s�������ꍇ�́A�e���䃌�W�X�^�𒼐ڃA�N�Z�X
          ���Ă��������B
*/

#ifndef SH4
#define SH4
#endif

#include "kernel.h"
#include "nosys4.h"
#include "sh7750.h"


/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP      7               /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �`���l���ԍ��̒�` */

#ifndef CH
#define CH      0               /* CH ����`�Ȃ� 0 �Ƃ��� */
#endif

/* �����n�̍��ق̒��� */

#if defined(__GNUC__)           /* exeGCC */
void set_vbr(void **base);
extern char _srdata[], _erdata[];
extern char _stext[], _etext[];

#elif defined(__ghs)            /* Green Hills */
void set_vbr(void **base);
extern char __ghsbegin_rodata[], __ghsend_rodata[];
extern char __ghsbegin_text[], __ghsend_text[];

#elif defined(__TID__)          /* IAR EW/ICC */
void set_vbr(void **base);
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;

#elif defined(GAIO)             /* XASSV */
#include <ht1_inline.h>
extern UW CONST_TOP[], CONST_SIZE[];

#elif defined(__HITACHI__)      /* Renesas C */
#include <machine.h>
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         (100*MHz/3) /* ���ӃN���b�NP��[Hz] */
#endif

/* �����݃x�N�^�A�h���X */

#ifndef INTVEC
#define INTVEC      0x80000000L
#endif

/* ���荞�݃n���h���e�[�u�� */

UW intdef[INT_FTXI-INT_NMI+1][2];

extern int RST_VECT;

#ifdef HOOK
/* �`�b�N���͐ώZ�J�E���^ */

UW TOTAL_TICKCK;
UW TICK_RATE;
#endif

extern  UB _KERNEL_pisrposi;
extern  UB _KERNEL_hdrlen;
extern  void _kernel_interr(void);


/*****************************************************************************
* �����ݏ�����
*
* �`��  intini();
*
* ���  �J�[�l���̏��������[�`���isysini�j���犄���݋֎~��ԂŌĂяo����܂��B
******************************************************************************/

ER intini(void)
{
    INT i;
    int *p;

    /* �����݃x�N�^�e�[�u���̏����� */

    for (i = 0; i < sizeof (intdef) / 8; i++)
    {   intdef[i][0] = 0x500000f0;              /* SR MD=1, BL=1, IMASK=15 */
        intdef[i][1] = (UW)_kernel_interr;
    }

    /* �x�N�^�x�[�X���W�X�^��ݒ� */
    /* �����[�g�f�o�b�K(���Z�b�g�x�N�^��0�Ԓn�łȂ��ꍇ)�͏��� */

    p = &RST_VECT;
    if (((int)p & 0x1fffffff) == 0x00000000)
        set_vbr((void **)INTVEC);
    else
    {
      #ifdef REBOOT
        set_vbr((void **)INTVEC);
      #endif
    }
  #ifdef HOOK
    TOTAL_TICKCK = 0;
  #endif

    return E_OK;
}

/*****************************************************************************
* �萔���ۂ����`�F�b�N����
*
* �`��  result = _kernel_inROM(const void *p);
*
*       VP p;         �`�F�b�N����̈�ւ̃|�C���^
*       BOOL result;  TRUE .... �萔 or �R�[�h�Z�N�V����
*                     FALSE ... ��L�ȊO
*
* ���  �p�P�b�g������񓙂��萔���ϐ����𔻒肵�܂��B
*       �J�[�l���̊e��V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

BOOL _kernel_inROM(const void *p)
{
#if defined(__GNUC__)           /* exeGCC */
    return((((UW)p >= (UW)_stext) && ((UW)p <= (UW)_etext)) || (((UW)p >= (UW)_srdata) && ((UW)p <= (UW)_erdata)));

#elif defined(__ghs)            /* Green Hills */
    return((((UW)p >= (UW)__ghsbegin_text) && ((UW)p <= (UW)__ghsend_text)) || (((UW)p >= (UW)__ghsbegin_rodata) && ((UW)p <= (UW)__ghsend_rodata)));

#elif defined(GAIO)             /* XASSV */
    return(((UW)p >= (UW)CONST_TOP) && ((UW)p <= ((UW)CONST_TOP+(UW)CONST_SIZE)));

#elif defined(__HITACHI__)      /* Renesas C */
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));

#elif defined(__TID__)           /* IAR EW/ICC */
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));

#endif
}

/*****************************************************************************
* �����݃T�[�r�X���[�`����`���̃`�F�b�N
*
* �`��  ercd = _kernel_check_cisr(const T_CISR *pk_cisr);
*
*       T_CISR pk_cisr;  �`�F�b�N���銄���݃T�[�r�X���[�`����`���ւ̃|�C���^
*       ER ercd;         E_PAR ... �����ݔԍ��ƃ}�X�N���x���̂����ꂩ���͈͊O
*                        E_OK .... �����ݔԍ��ƃ}�X�N���x���͋��ɐ���
*
* ���  �����݃T�[�r�X���[�`����`���̊����ݔԍ��ƃ}�X�N���x���͈̔͂�
*       �`�F�b�N���܂��B
*       cre_isr(), acre_isr() �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

ER _kernel_check_cisr(const T_CISR *pk_cisr)
{
    UINT intno, imask;

    intno = pk_cisr->intno - INT_NMI;
    if (intno < 0 || intno >= sizeof (intdef) / 8)
        return E_PAR;
    imask = pk_cisr->imask;
    if (imask < 1 || imask > 15)
        return E_PAR;
    return E_OK;
}

/*****************************************************************************
* �����݃n���h���̏������ݏ���
*
* �`��  _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr);
*
*       FP hdr;          �����݃T�[�r�X���[�`���g�p���̊����݃n���h���A�h���X
*       T_CISR pk_cisr;  �`�F�b�N���銄���݃T�[�r�X���[�`����`���ւ̃|�C���^
*
* ���  �����݃T�[�r�X���[�`���g�p���̊����݃n���h���̃A�h���X�������݃T�[�r�X
*       ���[�`����`���̊����ݔԍ����Z�o�����A�h���X�Ɋi�[���܂��B
*       cre_isr(), acre_isr() �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

extern  _kernel_cash_purge(FP);

void _kernel_set_inthdr(FP hdr, T_CISR *pk_cisr)
{
    UINT intno;

    _kernel_cash_purge(hdr);
    intno = pk_cisr->intno - INT_NMI;
    intdef[intno][0] = 0x50000000 | (pk_cisr->imask << 4);
    intdef[intno][1] = (UW)hdr;
}

/*****************************************************************************
* �����݃n���h���̓ǂݏo������
*
* �`��  inthdr = _kernel_get_inthdr(const T_CISR *pk_cisr);
*
*       T_CISR pk_cisr;  �`�F�b�N���銄���݃T�[�r�X���[�`����`���ւ̃|�C���^
*       FP inthdr;       ���Y�����ݔԍ��̊����݃n���h���A�h���X
*
* ���  �����݃T�[�r�X���[�`����`���̊����ݔԍ����Z�o�����A�h���X����
*       �����݃n���h���A�h���X��ǂݏo���܂��B
*       cre_isr(), acre_isr() �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

FP _kernel_get_inthdr(T_CISR *pk_cisr)
{
    return (FP)intdef[pk_cisr->intno - INT_NMI][1];
}

/*****************************************************************************
* �����݃n���h�����`����
*
* �`��  ercd = def_inh(UINT inhno, const T_DINH *pk_dinh);
*
*       UINT inhno; �@    �����݃x�N�^�ԍ�
*       T_DINH *pk_dinh; �����݃n���h����`���
*         ATR intatr;    �����݃n���h�������iTA_HLNG �̂�, ���Q�Ɓj
*         FP inthdr;     �����݃n���h���A�h���X
*         UINT imask;    �����݃}�X�N���x��
*       ER ercd;         �G���[�R�[�h
*
* ���  inhno �ɑΉ����銄���݃x�N�^�e�[�u���ɁAinthdr ��ݒ肵�܂��B
******************************************************************************/

ER v4_def_inh(INHNO inhno, const V4_T_DINH *pk_dinh)
{
    UINT psw;
    UINT i, m;
    FP fp;

    i = inhno - INT_NMI;
    if (i < 0 || i >= sizeof (intdef) / 8)
        return E_PAR;

    /* ��`�����̏ꍇ */
    /* ����`�����݂�ݒ� */

    if (pk_dinh == (T_DINH *)NADR) {
        fp = (FP)_kernel_interr;
        m = 15;
    }
    else {
        fp = pk_dinh->inthdr;
        m = pk_dinh->imask;
        if (m < 1 || m > 15)
            return E_PAR;
    }

    /* CPU�����݋֎~ */

    psw = vdis_psw();

    /* ��`�̏ꍇ */
    /* �V���������݃x�N�^�ݒ� */

    intdef[i][0] = 0x50000000 | (m << 4);   /* SR MD=1, BL=1, IMASK=imask */
    intdef[i][1] = (UW)fp;

    /* CPU�����݋֎~�߂� */
    vset_psw(psw);
    return E_OK;
}


/*****************************************************************************
* �����^�C�}�N���iSH7750 ���� TMU�j
*
******************************************************************************/

#if (CH==0)
#define TMU_TCR     TMU_TCR0
#define TMU_TCNT    TMU_TCNT0
#define TMU_TCOR    TMU_TCOR0
#define INT_TUNI    INT_TUNI0
#elif (CH==1)
#define TMU_TCR     TMU_TCR1
#define TMU_TCNT    TMU_TCNT1
#define TMU_TCOR    TMU_TCOR1
#define INT_TUNI    INT_TUNI1
#elif (CH==2)
#define TMU_TCR     TMU_TCR2
#define TMU_TCNT    TMU_TCNT2
#define TMU_TCOR    TMU_TCOR2
#define INT_TUNI    INT_TUNI2
#else
#error illegal CH number! (CH = 0 to 2)
#endif

void isritm(VP_INT exinf);
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_TUNI, isritm, IP };

static UH tpsc;

ER intsta(void)
{
    UW tc;
    UW psw;

    /* ���萔���v�Z */

  #if   (((CLK) / 256000) * MSEC >= 0x0fffffff)
    tc = ((CLK) / 256000) * MSEC;
    tpsc = 3;
  #elif (((CLK) /  64000) * MSEC >= 0x0fffffff)
    tc = ((CLK) /  64000) * MSEC;
    tpsc = 2;
  #elif (((CLK) /  16000) * MSEC >= 0x0fffffff)
    tc = ((CLK) /  16000) * MSEC;
    tpsc = 1;
  #else
    tc = ((CLK) /   4000) * MSEC;
    tpsc = 0;
  #endif

  #ifdef HOOK
    TICK_RATE = tc;
  #endif

    /* �����ݒ�` */

    acre_isr(&disr_isritm);

    /* �����ݗD�惌�x���ݒ� */

    psw = vdis_psw();
  #if (CH==0)
    sfr_outw(INTC_IPRA, sfr_inw(INTC_IPRA) | (IP << 12));
  #elif (CH==1)
    sfr_outw(INTC_IPRA, sfr_inw(INTC_IPRA) | (IP <<  8));
  #elif (CH==2)
    sfr_outw(INTC_IPRA, sfr_inw(INTC_IPRA) | (IP <<  4));
  #endif

    /* �^�C�}���j�b�g������ */

    sfr_outl(TMU_TCOR, (tc-1));     /* �^�C�}�R���X�^���g�ݒ� */
    sfr_outl(TMU_TCNT, (tc-1));     /* �^�C�}�J�E���^�����l */
    sfr_outw(TMU_TCR, tpsc          /* �^�C�}�v���X�P�[���I�� */
                    | 0x20);        /* �A���_�[�t���[���荞�݂����� */
    sfr_set(TMU_TSTR, 0x01 << CH);  /* �J�E���g����X�^�[�g */
    vset_psw(psw);

    return E_OK;
}

/*****************************************************************************
* �����^�C�}�����݃T�[�r�X���[�`���i���� TMU�j
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_outw(TMU_TCR, tpsc | 0x20); /* �A���_�[�t���[�t���O���N���A */
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* �����^�C�}���� */
}

/*****************************************************************************
* �`�b�N���͐ώZ�l���Z�o�i�_�~�[�j
*
******************************************************************************/

#ifdef HOOK
#if HOOK==1
UW tickck_read(void)
#else
UW ctex_timer_read(void)
#endif
{
    UW clk;

    if (!(sfr_inw(TMU_TCR) & 0x0100))
    {   clk = TICK_RATE - sfr_inl(TMU_TCNT);
    }
    if (sfr_inw(TMU_TCR) & 0x0100)
    {   clk = TICK_RATE + TICK_RATE - sfr_inl(TMU_TCNT);
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#if defined(__GNUC__)   /* exeGCC */
/*****************************************************************************
* �u�a�q�̐ݒ�^�擾�iexeGCC�j
*
******************************************************************************/

asm("
        .align  2
        .global _set_vbr
_set_vbr:
        rts
        ldc     r4,vbr

        .align  2
        .global _get_vbr
_get_vbr:
        rts
        stc     vbr,r0

");

#endif
#if defined(__ghs)      /* Green Hills */
/*****************************************************************************
* �f�a�q�^�u�a�q�̐ݒ�^�擾�iGreen Hills�j
*
******************************************************************************/

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

void **get_vbr(void)
{
    asm("   stc     vbr,r0  ");
}

#endif

/* end */
