/******************************************************************************
* �C���^�[�o�������݃n���h�� (SH7040�V���[�Y)                                 *
*                                                                             *
*  File name : n4i7040.c                                                      *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                   *
*                                                                             *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=?,CH=? n4i7040.c          (Renesas C) *
*            gcc -c -m2 -O2 -DCLK=? -DCH=? n4i7040.c              (exeGCC)    *
*            ccsh -c -cpu=sh_ii -ansi -noasmwarn -O -DCLK=?                   *
*                                               -DCH=? n4i7040.c  (GHS)       *
*                                                                             *
* 24/Jul/1998                                                                 *
* 30/Nov/1998 MiNET-7042 �p�ɏC��                                             *
* 12/Sep/2000 �t�b�N���[�`���p�̐ώZTick�N���b�N�ǂݏo����ǉ�             TA *
* 30/Oct/2000 IAR EW �Ή�                                                     *
* 19/Sep/2001 �����n�̔��f��������, �t�b�N���[�`���p�̃^�C���X�^���v��ǉ� TA *
* 23/Dec/2002 INTVSZ=0�Ή��R��C��, inthdr�������݃x�N�^(ROM)�ɋL�q��      MI *
* 23/Dec/2002 SH7042/44/45�ŋ��ʂ̂���n4i7040.c�Ƃ���, �R�����g�C��        MI *
******************************************************************************/

/*
�����݃x�N�^�e�[�u��

          def_inh�V�X�e���R�[���ɂ�銄���݃x�N�^�̐ݒ���\�ɂ��邽�߁A
          RAM�擪(400000H�Ԓn����1024�o�C�g)�ɁA�����݃x�N�^�e�[�u���������
          �z�肵�Ă��܂��B
          �����݃x�N�^�e�[�u���̗̈悪�A���̗̈�Əd�Ȃ�Ȃ��l�Ƀ����N���Ă�
          �������B(�d�Ȃ��Ă������J�Ōx���͏o�܂���!)
          �����݃x�N�^�e�[�u���̐擪�A�h���X��INTVEC�}�N���ŕύX�ł��܂��B
          �����݃x�N�^�e�[�u���̃T�C�Y(�o�C�g��)�́AINTVSZ�N���ŕύX�ł��܂��B

          (��) shc <op> -def=INTVEC=0,INTVSZ=336 n4i7040.c �c 0�Ԓn,336�o�C�g
                             ~~~~~~~~ ~~~~~~~~~~
          (��) gcc <op> -DINTVEC=0 -DINTVSZ=336 n4i7040.c
                          ~~~~~~~~   ~~~~~~~~~~
          �����݃x�N�^�e�[�u����ROM��ɂ���Adef_inh�Ɉ˂炸���ڃx�N�^���`
          ����ꍇ�AINTVEC=0, INTVSZ=0�Ƃ��Ă��������B���̏ꍇ�Adef_inh�V�X�e
          ���R�[���͋@�\���܂���B

dis_int, ena_int

          SH�V���[�Y�̊����݃R���g���[���ł́A�ėp�I��dis_int, ena_int�V�X�e
          ���R�[���̎��������G�ƂȂ��Ă��܂��܂��B
          ���̂Q�̃V�X�e���R�[���͗p�ӂ��Ă��܂���̂ŁA�ʂ̊����݋֎~��
          ���s�������ꍇ�́A�e���䃌�W�X�^�𒼐ڃA�N�Z�X���Ă��������B

_kernel_inROM

          �^����ꂽ�A�h���X��ROM�̈悩�ۂ���Ԃ��֐��ł��BROM�̈�ł����
          TRUE(1), ROM�̈�łȂ���� FALSE(0) ��Ԃ��܂��B
*/

#include "kernel.h"
#include "nosys4.h"
#include "sh7040.h"

/* �����݃x�N�^�e�[�u�� */

#ifndef INTVEC
#define INTVEC      0x400000L   /* �擪�A�h���X����`�̏ꍇ */
#endif
#ifndef INTVSZ
#define INTVSZ      1024        /* �T�C�Y����`�̏ꍇ */
#endif

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP      7               /* �����ݗD�揇�ʖ���`�̏ꍇ */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         24*MHz      /* �N���b�N����`�̏ꍇ */
#endif

/* �`���l���ԍ��̒�` */

#ifndef CH
#define CH      0               /* �`���l���ԍ�����`�̏ꍇ */
#endif

/* CH�ɍ��킹��CMT���W�X�^�̍Ē�` */

#if (CH==0)
#define CMT_CMCSR   CMT_CMCSR0
#define CMT_CMCNT   CMT_CMCNT0
#define CMT_CMCOR   CMT_CMCOR0
#define INT_CMT     INT_CMI0
#elif (CH==1)
#define CMT_CMCSR   CMT_CMCSR1
#define CMT_CMCNT   CMT_CMCNT1
#define CMT_CMCOR   CMT_CMCOR1
#define INT_CMT     INT_CMI1
#else
#error illegal CH number! (CH = 0 to 1)
#endif

/* �����n�̍��ق̒��� */

#if defined(__GNUC__)           /* exeGCC */
void disable(void);
void enable(void);
void set_cr(int cr);
int get_cr(void);
void set_vbr(void **base);
void **get_vbr(void);
extern char _srdata[], _erdata[];
extern char _stext[], _etext[];

#elif defined(__ghs)            /* Green Hills */
void disable(void);
void enable(void);
void set_cr(int cr);
int get_cr(void);
void set_vbr(void **base);
void **get_vbr(void);
extern char __ghsbegin_rodata[], __ghsend_rodata[];
extern char __ghsbegin_text[], __ghsend_text[];

#elif defined(__TID__)          /* IAR EW/ICC */
void set_vbr(void **base);
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;

#elif defined(GAIO)             /* XASS-V */
#include <ht1_inline.h>
extern UW CONST_TOP[], CONST_SIZE[];

#else                           /* Renesas C */
#ifndef __HITACHI__
#define __HITACHI__
#endif
#include <machine.h>
#define disable()   set_cr(15 << 4);
#define enable()    set_cr(0)
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;
#if (INTVSZ==0)
#pragma interrupt(inthdr)
#endif
#endif

/* Tick���͐ώZ�J�E���^ */

#ifdef HOOK
UW TOTAL_TICKCK;
UW TICK_RATE;
#endif

/* �O���Q�� */

extern int RST_VECT;
extern UB _KERNEL_pisrposi;
extern UB _KERNEL_hdrlen;
extern void _kernel_interr(void);

/* �����݃T�[�r�X���[�`��������� */

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_CMT, isritm, 0 };
#endif

/*****************************************************************************
* �����ݏ�����
*
* �`��  ER intini(void);
*
* �ߒl  ��� E_OK
*
* ���  �J�[�l���̏��������[�`�� sysini ���犄���݋֎~��ԂŌĂяo����܂��B
******************************************************************************/


ER intini(void)
{
  #if (INTVSZ!=0) /* �����݃e�[�u����ROM��ɒ�`�����ꍇ�� INTVSZ == 0 */

    INT i;
    int *p;

    /* �����݃x�N�^�e�[�u���̏����� */

    for (i = INT_IRQ0; i < (INTVSZ) / 4; i++)
        ((UW *)INTVEC)[i] = (UW)_kernel_interr;

    /* �x�N�^�x�[�X���W�X�^��ݒ� */
    /* �����[�g�f�o�b�K(���Z�b�g�x�N�^��0�Ԓn�łȂ��ꍇ)�͏��� */

    p = &RST_VECT;
    if (p == (int *)0L)
        set_vbr((void **)INTVEC);

  #endif
  #ifdef HOOK
    TOTAL_TICKCK = 0;
  #endif

    return E_OK;
}

/*****************************************************************************
* �萔���ۂ����`�F�b�N����
*
* �`��  BOOL _kernel_inROM(const void *p);
*
*       p     �`�F�b�N����̈�ւ̃|�C���^
*
* �ߒl  TRUE  �R�[�h�̈�܂��͒萔�̈�
*       FALSE ��L�ȊO
*
* ���  �I�u�W�F�N�g������񓙂��萔���ϐ����𔻒肷�邽�߂ɁA�J�[�l���̊e��
*       �V�X�e���R�[������Ăяo����܂��B
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

#elif defined(__TID__)          /* IAR EW/ICC */
    return((((UW)p >= (UW)_P_BGN) && ((UW)p <= (UW)_P_END)) || (((UW)p >= (UW)_C_BGN) && ((UW)p <= (UW)_C_END)));

#endif
}

/*****************************************************************************
* �����݃T�[�r�X���[�`���������̃`�F�b�N
*
* �`��  ER _kernel_check_cisr(const T_CISR *pk_cisr);
*
*       pk_cisr  �`�F�b�N���銄���݃T�[�r�X���[�`���������ւ̃|�C���^
*
* �ߒl  E_PAR �����ݔԍ��ƃ}�X�N���x���̂����ꂩ���͈͊O
*       E_OK  �����ݔԍ��ƃ}�X�N���x���͋��ɐ���
*
* ���  �����݃T�[�r�X���[�`���������̊����ݔԍ��ƃ}�X�N���x���͈̔͂�
*       �`�F�b�N���܂��B
*       cre_isr, acre_isr �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

ER _kernel_check_cisr(const T_CISR *pk_cisr)
{
    if (pk_cisr->intno >= (INTVSZ) / 4)
        return E_PAR;
    return E_OK;
}

/*****************************************************************************
* �����݃n���h���̃A�h���X�ݒ�
*
* �`��  void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr);
*
*       hdr       �����݃T�[�r�X���[�`�����Ăяo�������݃n���h���̃A�h���X
*       pk_cisr   �`�F�b�N���銄���݃T�[�r�X���[�`���������ւ̃|�C���^
*
* ���  �����݃T�[�r�X���[�`�����Ăяo�����߂ɁA�����Ő�������銄���݃n���h
*       ���̃A�h���X���A�����݃T�[�r�X���[�`���������̊����ݔԍ����Z�o��
*       �������݃x�N�^�֊i�[���܂��B
*       cre_isr, acre_isr �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    *p = (UW)hdr;
}

/*****************************************************************************
* �����݃n���h���̃A�h���X�擾
*
* �`��  FP _kernel_get_inthdr(const T_CISR *pk_cisr);
*
*       pk_cisr  �`�F�b�N���銄���݃T�[�r�X���[�`���������ւ̃|�C���^
*
* �ߒl  ���Y�����ݔԍ��̊����݃n���h���A�h���X
*
* ���  �����݃T�[�r�X���[�`���������̊����ݔԍ����Z�o���������݃x�N�^��
*       ��A�����݃n���h���A�h���X��ǂݏo���܂��B
*       cre_isr, acre_isr �V�X�e���R�[������Ăяo����܂��B
******************************************************************************/

FP _kernel_get_inthdr(const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    return (FP)*p;
}

/*****************************************************************************
* �����݃n���h�����`����
*
* �`��  ER def_inh(UINT inhno, const T_DINH *pk_dinh);
*
*       inhno    �����݃x�N�^�ԍ�
*       pk_dinh  �����݃n���h����`���
*
* �ߒl  �G���[�R�[�h
*
* ���  inhno �ɑΉ����銄���݃x�N�^�e�[�u���ɁAinthdr ��ݒ肵�܂��B
******************************************************************************/

ER v4_def_inh(INHNO inhno, const V4_T_DINH *pk_dinh)
{
  #if (INTVSZ==0)

    return E_NOSPT;

  #else

    UINT psw;
    UINT i;
    UW *p;
    FP fp;

    /* �����݃x�N�^�e�[�u���ւ̃|�C���^�Z�o */

    i = inhno;
    if (i >= (INTVSZ) / 4)
        return E_PAR;
    p = (UW *)INTVEC + i;

    /* ��`�����̏ꍇ�A����`�����݃n���h����ݒ� */

    if (pk_dinh == (T_DINH *)NADR)
        fp = (FP)_kernel_interr;
    else
        fp = pk_dinh->inthdr;

    /* CPU�����݋֎~ */

    psw = vdis_psw();

    /* �V���������݃x�N�^�ݒ� */

    *p = (UW)fp;

    /* CPU�����݋֎~�߂� */

    vset_psw(psw);
    return E_OK;

  #endif
}

/*****************************************************************************
* �����^�C�}�����݋N�� (����CMT)
*
* �`��  ER intsta(void);
*
* �ߒl  �G���[�R�[�h
*
* �⑫  �����݋֎~��ԂŎ��s���Ă��������B
******************************************************************************/

ER intsta(void)
{
    static UH const prescale[] = { 8, 32, 128, 512 };
    UW tc;
    UH cks;

    /* ���萔���v�Z */

    for (cks = 0; cks <= 3; cks++) {
        tc = ((UW)(CLK) * MSEC)/((UW)prescale[cks] * 1000);
        if (tc <= 0xffffL)
            break;
    }
    if (cks > 3 || tc == 0L)
        return E_PAR; /* �G���[ */

    /* �����ݒ�` */

  #if (INTVSZ!=0) /* ROM�ɒ�`����ꍇ�͕s�v */
    acre_isr(&disr_isritm);
  #endif

    /* �����ݗD�惌�x���ݒ� */

    sfr_setw(INTC_IPRG, (IP) << (4 - CH * 4));

    /* �^�C�}������ */

    sfr_outw(CMT_CMCOR, (H)(tc-1)); /* ��r�l */
    sfr_outw(CMT_CMCSR, 0x40        /* �R���y�A�}�b�`���荞�݂����� */
                       |cks);       /* �N���b�N�I�� */
    sfr_setw(CMT_CMSTR, 0x01 << CH);/* �J�E���g����X�^�[�g */
  #ifdef HOOK
    TICK_RATE = (UW)tc;
  #endif

    return E_OK;
}

/*****************************************************************************
* �����^�C�}�����T�[�r�X���[�`�� (���� CMT)
*
******************************************************************************/

void isritm(VP_INT exinf)
{
  #ifdef HOOK
    UINT psw;
    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
  #endif
    sfr_clrw(CMT_CMCSR, 0x80);      /* CMF �R���y�A�}�b�`�t���O�N���A */
  #ifdef HOOK
    vset_psw(psw);
  #endif
    sig_tim();                      /* �V�X�e���N���b�N���� */
}

/*****************************************************************************
* �����^�C�}�����݃n���h�� (���� CMT)
*
******************************************************************************/

#if (INTVSZ==0)
INTHDR inthdr(void)
{
    ent_int();                      /* �����݃n���h���̊J�n */
    isritm(NULL);                   /* �����T�[�r�X���[�`�����Ă� */
    ret_int();                      /* �����݃n���h�����畜�A���� */
}
#endif

/*****************************************************************************
* Tick���͐ώZ�l���Z�o (����CMT)
*
******************************************************************************/

#ifdef HOOK
#if (HOOK==1)
UW tickck_read(void)
#else
UW ctex_timer_read(void)
#endif
{
    UW clk;
    float wclk;

    if (!(sfr_inw(CMT_CMCSR) & 0x80))
        clk = sfr_inw(CMT_CMCNT) + TOTAL_TICKCK;
    if (sfr_inw(CMT_CMCSR) & 0x80)
        clk = sfr_inw(CMT_CMCNT) + TICK_RATE + TOTAL_TICKCK;

    /* ������L���ɂ���ΒP�ʒʂ�̃^�C���X�^���v�ɂȂ邪�A���������_�� */
    /* �g�p���邽�߃I�[�o�[�w�b�h���傫���Ȃ�B */
  #if 0
    wclk = clk * MSEC * 1000.0;
    clk = (UW)(wclk / TICK_RATE);
  #endif
    return clk;
}

#else

UW tickck_read(void)
{
    return 0L;
}
#endif

#if defined(__GNUC__)   /* exeGCC */
/*****************************************************************************
* �����݋֎~�^����, VBR�̐ݒ�^�擾
*
******************************************************************************/

asm("
        .align  2
        .global _disable
_disable:
        mov     #0xf0,r0
        rts
        ldc     r0,sr

        .align  2
        .global _enable
_enable:
        mov     #0,r0
        rts
        ldc     r0,sr

        .align  2
        .global _set_cr
_set_cr:
        rts
        ldc     r4,sr

        .align  2
        .global _get_cr
_get_cr:
        rts
        stc     sr,r0

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
#if defined(__ghs)   /* Green Hills */
/*****************************************************************************
* �����݋֎~�^����, �u�a�q�̐ݒ�^�擾
*
******************************************************************************/

void disable(void)
{
    asm("   mov     #0xf0,r0");
    asm("   ldc     r0,sr   ");
}

void enable(void)
{
    asm("   mov     #0,r0   ");
    asm("   ldc     r0,sr   ");
}

void set_cr(int cr)
{
    asm("   ldc     r4,sr   ");
}

int get_cr(void)
{
    asm("   stc     sr,r0   ");
}

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

void **get_vbr(void)
{
    asm("   stc     vbr,r0  ");
}

#endif
#if defined(__TID__)    /* IAR EW/ICC */
/*****************************************************************************
* VBR and GBR handling (IAR EW/ICC)
*
******************************************************************************/

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

#endif

/* end */
