/****************************************************************************
* �C���^�[�o�������݃n���h���iSH7615�j                                      *
*                                                                           *
*  File name : n4i7615.c                                                    *
*  Copyright (c) 2000-2002 MiSPO Co., Ltd.  All rights reserved.            *
*                                                                           *
* Compile : shc -cp=sh2 -op=1 -def=CLK=?,CH=? n4i7615.c         (Renesas C) *
*           gcc -c -m2 -O2 -DCLK=? -DCH=? n4i7615.c             (exeGCC)    *
*           ccsh -c -cpu=sh_ii -ansi -noasmwarn -O -DCLK=? -DCH=? n4i7615.c *
*                                                               (GHS)       *
*                                                                           *
* 11/Apr/2000 �쐬                                                    MiSPO *
* 24/Sep/2000 �t�b�N���[�`���p�̃_�~�[�ώZ�`�b�N�N���b�N���C��              *
* 30/Oct/2000 IAR EW �Ή�                                                   *
* 19/Sep/2001 ����n�̔��f���������A�t�b�N���[�`���̃^�C���X�^���v��ǉ� TA *
* 27/Dec/2002 INTVSZ=0�Ή��R��C��, inthdr�������݃x�N�^(ROM)�ɋL�q��    KA *
* 29/Jul/2003 �x�N�^ROM��`���̌�����                                    KA *
****************************************************************************/

/*

�g�p����n�[�h�E�F�A

          SH7615 ���� FRT �܂��� TPU ���C���^�[�o����^�C�}�Ƃ��Ďg�p����
          ���܂��B
          ����ȊO�̃^�C�}��p����ꍇ�́A���[�U�[�ŉ������Ă��������B

�����ݎ����̎w��

          �����ݎ����́AMSEC �}�N���Ō��܂�܂��B�f�t�H���g�� 10 msec �ł��B
          �Ⴆ�΁A5 msec �ɕύX����ꍇ�́A���̗l�ɃR���p�C�����Ă��������B

          (��) shc -cp=sh2 -op=1 -def=MSEC=5 n4i7615.c
                                      ~~~~~~
          (��) gcc -c -m2 -O2 -DMSEC=5 n4i7615.c
                                ~~~~~~
�N���b�N�̎w��

          �\�[�X�N���b�N(P��)�̃f�t�H���g�� 15 MHz �ł��B�ύX����ꍇ ��
          CLK �}�N�����`���ăR���p�C�����Ă��������B�l�� [Hz] �P�ʂ�
          ���g���ł��B

          (��) shc <options> -def=CLK=28700000 n4i7615.c ����� 28.7 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=28700000 n4i7615.c
                               ~~~~~~~~~~~~
�`���l���ԍ��̎w��

          CH �}�N���ŁA�g�p���� TPU �`���l���iCH=0�`2�j
          �܂��́A              FRT         �iCH=3�j
          ���w��ł��܂��B
          �f�t�H���g�� CH=3 �ł��B

          (��) shc <options> -def=CH=1 n4i7615.c  ���������� TPU ����1
                                  ~~~~
          (��) gcc <options> -DCH=1 n4i7615.c
                               ~~~~
�����݃x�N�^�e�[�u��

          def_inh �V�X�e���R�[���ɂ�銄���݃x�N�^�̐ݒ���\�ɂ��邽�߁A
          RAM �擪�i6000000H �Ԓn���� 1024 �o�C�g�j�ɁA�����݃x�N�^�e�[�u����
          ����Ƒz�肵�Ă��܂��B
          �����݃x�N�^�e�[�u���̗̈悪�A���̗̈�Əd�Ȃ�Ȃ��l�Ƀ����N���Ă�
          �������B�i�d�Ȃ��Ă������J�Ōx���͏o�܂���!�j
          �����݃x�N�^�e�[�u���擪�A�h���X�� INTVEC �}�N���ŁA�T�C�Y�i�o�C�g
          ���j�́AINTVSZ �}�N���ŕύX�ł��܂��B

          (��) shc <op> -def=INTVEC=0,INTVSZ=336 n4i7615.c ��� 0�Ԓn,336�޲�
                             ~~~~~~~~ ~~~~~~~~~~
          (��) gcc <op> -DINTVEC=0 -DINTVSZ=336 n4i7615.c
                          ~~~~~~~~   ~~~~~~~~~~
          �����݃x�N�^�e�[�u���� ROM ��ɂ���Adef_inh �Ɉ˂炸���ڃx�N�^���
          �`����ꍇ�AINTVSZ=0 �Ƃ��Ă��������B
          ���̏ꍇ�Adef_inh �V�X�e���R�[���͋@�\���܂���B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=2 n4i7615.c ��� �����ݗD�揇�� 2
                                  ~~~~
          (��) gcc <options> -DIP=2 n4i7615.c
                               ~~~~
dis_int, ena_int

          ���荞�݃��x���ݒ�̃r�b�g�z�u���s�K���� SH7615 �̊����݃R���g���[
          ���ł́A�ėp�I�� dis_int, ena_int �V�X�e���R�[���̎��������G�ƂȂ�
          �Ă��܂��܂��B
          ���̂Q�̃V�X�e���R�[���͗p�ӂ��Ă��܂���̂ŁA�ʂ̊����݋֎~��
          ���s�������ꍇ�́A�e���䃌�W�X�^�𒼐ڃA�N�Z�X���Ă��������B

_kernel_inROM(const void *)

          �^����ꂽ�A�h���X��ROM�̈悩�ۂ���Ԃ��֐��ł��BROM�̈�ł����
          TRUE(1), ROM�̈�łȂ���� FALSE(0) ��Ԃ��Ă��������B

*/

#include "kernel.h"
#include "nosys4.h"
#include "sh7615.h"

/* �����݃x�N�^�e�[�u�� */

#ifndef INTVEC
#define INTVEC      0x6000000L
#endif
#ifndef INTVSZ
#define INTVSZ      1024
#endif

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP      7               /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         15*MHz      /* P�ӃN���b�N[Hz] */
#endif

/* �`���l���ԍ��̒�` */

#ifndef CH
#define CH      3               /* CH ����`�Ȃ� 3 �Ƃ��� */
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

#elif defined(GAIO)             /* XASSV */
#include <ht1_inline.h>
extern UW CONST_TOP[], CONST_SIZE[];

#elif defined(__HITACHI__)      /* Renesas C */
#include <machine.h>
#define disable()   set_cr(15 << 4);
#define enable()    set_cr(0)
extern int *_C_BGN, *_C_END, *_P_BGN, *_P_END;
#if (INTVSZ==0)
#pragma interrupt(inthdr)
#endif
#endif

extern int RST_VECT;

#ifdef HOOK
/* �`�b�N���͐ώZ�J�E���^ */

UW TOTAL_TICKCK;
UW TICK_RATE;
UH CM_FLAG;
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
{   /* �����݃e�[�u����ROM��ɒ�`�����ꍇ�� INTVSZ == 0 */
  #if (INTVSZ!=0)

    INT i;
    int *p;

    /* �����݃x�N�^�e�[�u���̏����� */

    for (i = INT_IRL1; i < (INTVSZ) / 4; i++)
        ((UW *)INTVEC)[i] = (UW)_kernel_interr;

    /* �x�N�^�x�[�X���W�X�^��ݒ� */
    /* �����[�g�f�o�b�K(���Z�b�g�x�N�^��0�Ԓn�łȂ��ꍇ)�͏��� */

    p = &RST_VECT;
    if (p == (int *)0L)
        set_vbr((void **)INTVEC);
    else{
      #ifdef REBOOT
        set_vbr((void **)INTVEC);
      #endif
    }

  #endif

  #ifdef HOOK
    TOTAL_TICKCK = 0;
    CM_FLAG = 0;
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

#elif defined(__TID__)          /* IAR EW/ICC */
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
    if (pk_cisr->intno >= (INTVSZ) / 4)
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

void _kernel_set_inthdr(FP hdr, const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    *p = (UW)hdr;
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

FP _kernel_get_inthdr(const T_CISR *pk_cisr)
{
    UW *p;

    p = (UW *)INTVEC + pk_cisr->intno;
    return (FP)*p;
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
*         UINT imask;    �����݃}�X�N���x���i���g�p�j
*       ER ercd;         �G���[�R�[�h
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

    /* ��`�����̏ꍇ */
    /* ����`�����݂�ݒ� */

    if (pk_dinh == (T_DINH *)NADR)
        fp = (FP)_kernel_interr;
    else
        fp = pk_dinh->inthdr;

    /* CPU�����݋֎~ */

    psw = vdis_psw();

    /* ��`�̏ꍇ */
    /* �V���������݃x�N�^�ݒ� */

    *p = (UW)fp;

    /* CPU�����݋֎~�߂� */
    vset_psw(psw);
    return E_OK;

  #endif
}

#if CH < 3
/*****************************************************************************
* �C���^�[�o���^�C�}�N���iSH7615 ���� TPU�j
*
* �����݋֎~��ԂŎ��s���Ă��������B
******************************************************************************/

#if (CH==0)
#define TPU_TCR   TPU_TCR0
#define TPU_TMDR  TPU_TMDR0
#define TPU_TIOR  TPU_TIOR0H
#define TPU_TIER  TPU_TIER0
#define TPU_TSR   TPU_TSR0
#define TPU_TCNT  TPU_TCNT0
#define TPU_TGRA  TPU_TGR0A
#define INT_TGIA  INT_TGI0A
#elif (CH==1)
#define TPU_TCR   TPU_TCR1
#define TPU_TMDR  TPU_TMDR1
#define TPU_TIOR  TPU_TIOR1
#define TPU_TIER  TPU_TIER1
#define TPU_TSR   TPU_TSR1
#define TPU_TCNT  TPU_TCNT1
#define TPU_TGRA  TPU_TGR1A
#define INT_TGIA  INT_TGI1A
#else
#define TPU_TCR   TPU_TCR2
#define TPU_TMDR  TPU_TMDR2
#define TPU_TIOR  TPU_TIOR2
#define TPU_TIER  TPU_TIER2
#define TPU_TSR   TPU_TSR2
#define TPU_TCNT  TPU_TCNT2
#define TPU_TGRA  TPU_TGR2A
#define INT_TGIA  INT_TGI2A
#endif

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_TGIA, isritm, 0 };
#endif

ER intsta(void)
{
    static UH const prescale[] = { 1, 4, 16, 64 };
    UW tc;
    UB cks;

    /* ���萔���v�Z */

    for (cks = 0; cks <= 3; cks++)
    {   tc = ((UW)(CLK) * MSEC)/((UW)prescale[cks] * 1000);
        if (tc <= 0xffffL)
            break;
    }
    if (cks > 3 || tc == 0L)
        return E_PAR; /* �G���[ */

    /* �����ݒ�` */

  #if (INTVSZ!=0)
    /* �����ݒ�`: ROM�ɒ�`����ꍇ�͕s�v */
    acre_isr(&disr_isritm);
  #endif

    /* �����ݗD�惌�x���ݒ� */

    sfr_setw(INTC_IPRD, (IP) << (12 - CH * 4));

    /* �����݃x�N�^�ԍ��ݒ� */

  #if INT_TGIA == INT_TGI0A
    sfr_outw(INTC_VCRE, sfr_inw(INTC_VCRE) | (INT_TGIA << 8));
  #elif INT_TGIA == INT_TGI1A
    sfr_outw(INTC_VCRH, sfr_inw(INTC_VCRH) | (INT_TGIA << 8));
  #else
    sfr_outw(INTC_VCRJ, sfr_inw(INTC_VCRJ) | (INT_TGIA << 8));
  #endif

    /* �^�C�}������ */

    sfr_out(TPU_TCR, 0x20+cks);     /* �N���b�N�I�����R���y�A�}�b�`�`�ŃN���A */
    sfr_outw(TPU_TGRA, (UH)tc);     /* �A�E�g�v�b�g�R���y�A�l */
    sfr_clr(TPU_TSR, 0x01);         /* �A�E�g�v�b�g�R���y�A�t���O�`�N���A*/
    sfr_outw(TPU_TCNT, 0x00);       /* �J�E���^�N���A */
    sfr_set(TPU_TIER, 0x01);        /* TGIEA ���荞�݋��� */
    sfr_clr(TPU_TSYR, 0x01 << CH);  /* �J�E���g�Ɨ� */
    sfr_set(TPU_TSTR, 0x01 << CH);  /* �J�E���g����X�^�[�g */
#ifdef HOOK
    TICK_RATE = (UW)tc;
#endif

    return E_OK;
}

/*****************************************************************************
* �C���^�[�o�������������[�`���i���� TPU�j
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clr(TPU_TSR, 0x01);         /* �A�E�g�v�b�g�R���y�A�t���O�`�N���A*/
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* �V�X�e���N���b�N���� */
}

/*****************************************************************************
* �����^�C�}�����݃n���h�� (���� TPU)
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

    if (!(sfr_in(TPU_TSR) & 0x01))
    {   clk = sfr_inw(TPU_TCNT);
    }
    if (sfr_in(TPU_TSR) & 0x01)
    {   clk = sfr_inw(TPU_TCNT) + TICK_RATE;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#elif CH == 3
/*****************************************************************************
* �C���^�[�o���^�C�}�N���iSH7615 ���� FRT�j
*
* �����݋֎~��ԂŎ��s���Ă��������B
******************************************************************************/

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_OCI, isritm, 0 };
#endif

ER intsta(void)
{
    UH tc;
    UB cks;

    /* ���萔���v�Z */

  #if (((CLK) * MSEC)/(1000L * 8) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 8));
    cks = 0x00;
  #elif (((CLK) * MSEC)/(1000L * 32) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 32));
    cks = 0x01;
  #elif (((CLK) * MSEC)/(1000L * 128) <= 0xffffL)
    tc = (UH)(((CLK) * MSEC)/(1000L * 128));
    cks = 0x02;
  #else
    #error time constant overflow!
  #endif

    /* �����ݒ�` */

  #if (INTVSZ!=0)
    /* �����ݒ�`: ROM�ɒ�`����ꍇ�͕s�v */
    acre_isr(&disr_isritm);
  #endif

    /* �����ݗD�惌�x���ݒ� */

    sfr_outw(INTC_IPRB, sfr_inw(INTC_IPRB) | (IP << 8));

    /* �����݃x�N�^�ԍ��ݒ� */

    sfr_outw(INTC_VCRC, sfr_inw(INTC_VCRC) | INT_OCI);

    /* �t���[������j���O��^�C�}�����ݒ� */

    sfr_clr(FRT_TOCR, 0x10);            /* OCRA �I�� */
    sfr_out(FRT_OCRABH, tc>>8);         /* �A�E�g�v�b�g�R���y�A�l */
    sfr_out(FRT_OCRABL, tc&0xff);
    sfr_out(FRT_TCR, cks);              /* �N���b�N�I�� */
    sfr_out(FRT_FTCSR, 0x01);           /* �R���y�A�}�b�`�`�ŃN���A */
    sfr_clr(FRT_FTCSR, 0x08);           /* �A�E�g�v�b�g�R���y�A�t���O�`�N���A*/
    sfr_out(FRT_FRCH, 0x00);            /* �J�E���^�N���A */
    sfr_out(FRT_FRCL, 0x00);
    sfr_out(FRT_TIER, 0x08);            /* OCIA �����݋��� */
#ifdef HOOK
    TICK_RATE = (UW)tc+1;
#endif
    return E_OK;
}

/*****************************************************************************
* �C���^�[�o�������������[�`���i���� FRT�j
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    psw = vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
    CM_FLAG = 0;
#endif
    sfr_clr(FRT_FTCSR, 0x08);       /* �A�E�g�v�b�g�R���y�A�t���O�`�N���A*/
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* �V�X�e���N���b�N���� */
}

/*****************************************************************************
* �����^�C�}�����݃n���h�� (���� FRT)
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
* �`�b�N���͐ώZ�l���Z�o�i���� FRT�j
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

    if (sfr_in(FRT_TIER) & 0x08)
    {
        if (!(sfr_in(FRT_FTCSR) & 0x08))
        {   clk  = sfr_in(FRT_FRCH) << 8;
            clk += sfr_in(FRT_FRCL);
        }
        if ((sfr_in(FRT_FTCSR) & 0x08) || (CM_FLAG))
        {   CM_FLAG = 1;
            clk  = sfr_in(FRT_FRCH) << 8;
            clk += sfr_in(FRT_FRCL);
            clk += TICK_RATE;
        }
        return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
    }
    else
    {
        return 0L;
    }
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#else
#error illegal CH number! (CH = 0 to 3)
#endif  /* #if CH == ?? */

#if defined(__GNUC__)   /* exeGCC */
/*****************************************************************************
* �����݋֎~�^����, �u�a�q�̐ݒ�^�擾
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
#if defined(__ghs)      /* Green Hills */
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
