/****************************************************************************
* �C���^�[�o�������݃n���h���iSH7055�j                                      *
*                                                                           *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -op=1 -def=CLK=?,CH=? n4i7055.c                  (SHC)     *
*                                                                           *
* 06/Feb/2001                                                               *
* 27/Mar/2001  GAIO/XASSV �Ή�                                           TA *
* 12/Sep/2001 ����n�̔��f��������                                       TA *
* 23/Dec/2002 INTVSZ=0�Ή��R��C��, inthdr�������݃x�N�^(ROM)�ɋL�q��    KA *
* 29/Jul/2003 �x�N�^ROM��`���̌�����                                    KA *
****************************************************************************/

/*

�œK���R���p�C��

          ���荞�݃n���h���̃R���p�C���ɂ́A�K���A�œK���I�v�V������t���A
          ent_int �Ăяo���̑O�ɁA�s�v�Ȗ��߂���������Ȃ��悤���ӂ��Ă�����
          ���B
          SHC�ł́A�����݃n���h���� #pragma interrupt �ƒ�`���Ă��������B

�g�p����n�[�h�E�F�A

          SH7055 ���� ATU-II �̃`���l�� 3�`7���C���^�[�o����^�C�}�Ƃ��Ďg�p
          ���Ă��܂��B
          ����ȊO�̃^�C�}��p����ꍇ�́A���[�U�[�ŉ������Ă��������B

�����ݎ����̎w��

          �����ݎ����́AMSEC �}�N���Ō��܂�܂��B�f�t�H���g�� 10 msec �ł��B
          �Ⴆ�΁A5 msec �ɕύX����ꍇ�́A���̗l�ɃR���p�C�����Ă��������B

          (��) shc -op=1 -def=MSEC=5 n4i7055.c
                              ~~~~~~
�N���b�N�̎w��

          �����N���b�N�̃f�t�H���g�� 40 MHz �ł��B�ύX����ꍇ �� CLK �}�N��
          ���`���ăR���p�C�����Ă��������B�l�� [Hz] �P�ʂ̎��g���ł��B

          (��) shc -op=1 -def=CLK=19660800 n4i7055.c ��� 19.6608 MHz
                              ~~~~~~~~~~~~
�`���l���ԍ��̎w��

          CH �}�N���ŁA�g�p���� ITU �`���l�����w��ł��܂��B�iCH=3�`7�j
          �f�t�H���g�� CH=3 �ł��B

          (��) shc -op=1 -def=CH=6 n4i7055.c  ������� ATU ����6
                              ~~~~
�����݃x�N�^�e�[�u��

          def_inh �V�X�e���R�[���ɂ�銄���݃x�N�^�̐ݒ���\�ɂ��邽�߁A
          RAM �擪�i9000000H �Ԓn���� 1024 �o�C�g�j�ɁA�����݃x�N�^�e�[�u����
          ����Ƒz�肵�Ă��܂��B
          �����݃x�N�^�e�[�u���̗̈悪�A���̗̈�Əd�Ȃ�Ȃ��l�Ƀ����N���Ă�
          �������B�i�d�Ȃ��Ă������J�Ōx���͏o�܂���!�j
          �����݃x�N�^�e�[�u���擪�A�h���X�� INTVEC �}�N���ŁA�T�C�Y�i�o�C�g
          ���j�́AINTVSZ �}�N���ŕύX�ł��܂��B

          (��) shc -op=1 -def=INTVEC=0,INTVSZ=464 n4i7055.c ��� 0�Ԓn,464�޲�
                              ~~~~~~~~ ~~~~~~~~~~
          �����݃x�N�^�e�[�u���� ROM ��ɂ���Adef_inh �Ɉ˂炸���ڃx�N�^���
          �`����ꍇ�AINTVSZ=0 �Ƃ��Ă��������B
          ���̏ꍇ�Adef_inh �V�X�e���R�[���͋@�\���܂���B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc -op=1 -def=IP=2 n4i7055.c ��� �����ݗD�揇�� 2
                              ~~~~
dis_int, ena_int

          ���荞�݃��x���ݒ�̃r�b�g�z�u���s�K���� SH7055 �̊����݃R���g���[
          ���ł́A�ėp�I�� dis_int, ena_int �V�X�e���R�[���̎��������G�ƂȂ�
          �Ă��܂��܂��B
          ���̂Q�̃V�X�e���R�[���͗p�ӂ��Ă��܂���̂ŁA�ʂ̊����݋֎~��
          ���s�������ꍇ�́A�e���䃌�W�X�^�𒼐ڃA�N�Z�X���Ă��������B
*/

#ifndef SH2E
#define SH2E
#endif

#include "kernel.h"
#include "nosys4.h"
#include "sh7055.h"

/* �����݃x�N�^�e�[�u�� */

#ifndef INTVEC
#define INTVEC      0x0400000L
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
#define CLK         40*MHz      /* CPU�N���b�N[Hz] */
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

#elif defined(__TID__)           /* IAR EW/ICC */
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
    static int *p;

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


/*****************************************************************************
* �C���^�[�o���^�C�}�N���iSH7055 ���� ATU�j
*
* �����݋֎~��ԂŎ��s���Ă��������B
******************************************************************************/

#if (CH==3)
#define ATU_TCR     ATU_TCR3
#define ATU_IMFD_b  0x0008
#define ATU_TSTR_b  0x10
#define ATU_TCNT    ATU_TCNT3
#define ATU_TIOR    ATU_TIOR3B
#define ATU_GRD     ATU_GR3D
#define ATU_TMDR_b  0x01
#define INT_IMID    INT_IMI3D
#elif (CH==4)
#define ATU_TCR     ATU_TCR4
#define ATU_IMFD_b  0x0100
#define ATU_TSTR_b  0x20
#define ATU_TCNT    ATU_TCNT4
#define ATU_TIOR    ATU_TIOR4B
#define ATU_GRD     ATU_GR4D
#define ATU_TMDR_b  0x02
#define INT_IMID    INT_IMI4D
#elif (CH==5)
#define ATU_TCR     ATU_TCR5
#define ATU_IMFD_b  0x2000
#define ATU_TSTR_b  0x40
#define ATU_TCNT    ATU_TCNT5
#define ATU_TIOR    ATU_TIOR5B
#define ATU_GRD     ATU_GR5D
#define ATU_TMDR_b  0x04
#define INT_IMID    INT_IMI5D
#elif (CH==6)
#define ATU_TCR     ATU_TCR6B
#define ATU_TSR     ATU_TSR6
#define ATU_TIER    ATU_TIER6
#define ATU_PSCR    ATU_PSCR2
#define ATU_CYLRD   ATU_CYLR6D
#define ATU_TCNT    ATU_TCNT6D
#define INT_CMID    INT_CMI6D
#define ATU_TSTR_b  0x08
#elif (CH==7)
#define ATU_TCR     ATU_TCR7B
#define ATU_TSR     ATU_TSR7
#define ATU_TIER    ATU_TIER7
#define ATU_PSCR    ATU_PSCR3
#define ATU_CYLRD   ATU_CYLR7D
#define ATU_TCNT    ATU_TCNT7D
#define INT_CMID    INT_CMI7D
#define ATU_TSTR_b  0x80
#else
#error illegal CH number! (CH = 3 to 7)
#endif

#if ((CH>=3)&&(CH<=5))

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_IMID, isritm, 0 };
#endif

ER intsta(void)
{
    static UB const prescale[] = { 1, 2, 4, 8, 16, 32 };
    UW clk,tc;
    INT i;
    UB tpsc;

    /* ���萔���v�Z */

    clk /= sfr_in(ATU_PSCR1)+1;
    for (i = 0; i < sizeof(prescale); i++)
    {   tc = ((UW)(CLK) * MSEC)/((UW)prescale[i] * 1000 * 2);
        if (tc <= 0x10000L)
            break;
    }
    if (i == sizeof(prescale) || tc == 0L)
        return E_SYS;
    tpsc = i;

    /* �����ݒ�` */

  #if (INTVSZ!=0)
    acre_isr(&disr_isritm);
  #endif

    /* �����ݗD�惌�x���ݒ� */

  #if (CH==3)
    sfr_outw(INTC_IPRF, sfr_inw(INTC_IPRF) | (IP << 12));
  #elif (CH==4)
    sfr_outw(INTC_IPRF, sfr_inw(INTC_IPRF) | (IP <<  4));
  #elif (CH==5)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | (IP << 12));
  #endif

    /* �^�C�}���j�b�g������ */

    sfr_outw(ATU_GRD, (H)(tc-1));       /* ��r�l */
    sfr_out(ATU_TCR, tpsc);             /* �N���b�N�I�� */
    sfr_setw(ATU_TIER3, ATU_IMFD_b);    /* IMID �����݋��� */
    sfr_outw(ATU_TCNT, 0x0000);         /* �t���[�����j���O�J�E���^���N���A */
    sfr_clr(ATU_TMDR3, ATU_TMDR_b);     /* PWM ���[�h�ɐݒ� */
    sfr_outw(ATU_TIOR, 0xb000);         /* GRxD �ƃR���y�A�}�b�`�A�g�O���o�� */
    sfr_set(ATU_TSTR1, ATU_TSTR_b);     /* TCNT�t���[�����J�E���^�n�� */
#ifdef HOOK
    TICK_RATE = tc;
#endif
    return E_OK;
}

/*****************************************************************************
* �C���^�[�o�������ݏ������[�`�i���� ATU�j
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clrw(ATU_TSR3, ATU_IMFD_b); /* IMID �R���y�A�}�b�`�t���O�N���A */
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
* �`�b�N���͐ώZ�l���Z�o�i���� ATU�j
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

    if (!(sfr_inw(ATU_TSR3) & ATU_IMFD_b))
    {   clk = sfr_inw(ATU_TCNT);
    }
    if (sfr_inw(ATU_TSR3) & ATU_IMFD_b)
    {   clk = sfr_inw(ATU_TCNT) + TICK_RATE;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#else

void isritm(VP_INT exinf);
#if (INTVSZ!=0)
const T_CISR disr_isritm = { TA_HLNG, NULL, INT_CMID, isritm, 0 };
#endif

UH calc_scale(UB *pscr, UH *tcr)
{
    static UB const prescale[] = { 1, 2, 4, 8, 16, 32 };
    UW clk,tc1, tc2;
    INT i, j;

    for (i = 0; i < sizeof(prescale); i++)
    {   tc1 = ((UW)(CLK) * MSEC)/((UW)prescale[i] * 1000 * 2);
        for(j = 0; j < 32; j++)
        {   tc2 = tc1 / (j+1);
            if (tc2 <= 0x10000L)
            {   *pscr = j;
                *tcr  = i << 12;
                return tc2;
            }
        }
    }
    return 0;
}

ER intsta(void)
{
    UH tc, tcr;
    UB pscr;

    /* ���萔���v�Z */

    tc = calc_scale(&pscr, &tcr);
    if (tc == 0)
        return E_SYS;

    /* �����ݒ�` */

  #if (INTVSZ!=0)
    acre_isr(&disr_isritm);
  #endif

    /* �����ݗD�惌�x���ݒ� */

  #if (CH==6)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | (IP << 4));
  #elif (CH==7)
    sfr_outw(INTC_IPRG, sfr_inw(INTC_IPRF) | IP);
  #endif

    /* �^�C�}���j�b�g������ */

    sfr_outw(ATU_CYLRD, tc);            /* ��r�l */
    sfr_out(ATU_PSCR, pscr);            /* ������I�� */
    sfr_outw(ATU_TCR, tcr);             /* �N���b�N�I�� */
    sfr_setw(ATU_TIER, 0x0008);         /* CMID �����݋��� */
    sfr_outw(ATU_TCNT, 0x0001);         /* �t���[�����j���O�J�E���^���N���A */
    sfr_set(ATU_TSTR2, ATU_TSTR_b);     /* TCNT�t���[�����J�E���^�n�� */
#ifdef HOOK
    TICK_RATE = tc;
#endif
    return E_OK;
}

/*****************************************************************************
* �C���^�[�o�������ݏ������[�`�i���� ATU�j
*
******************************************************************************/

void isritm(VP_INT exinf)
{
#ifdef HOOK
    UINT psw;

    vdis_psw();
    TOTAL_TICKCK += TICK_RATE;
#endif
    sfr_clrw(ATU_TSR, 0x0008);      /* CMID �R���y�A�}�b�`�t���O�N���A */
#ifdef HOOK
    vset_psw(psw);
#endif
    sig_tim();                      /* �V�X�e���N���b�N���� */
}

/*****************************************************************************
* �����^�C�}�����݃n���h�� (���� ATU)
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
* �`�b�N���͐ώZ�l���Z�o�i���� ATU�j
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

    if (!(sfr_inw(ATU_TSR) & 0x0008))
    {   clk = sfr_inw(ATU_TCNT) - 1;
    }
    if (sfr_inw(ATU_TSR) & 0x0008)
    {   clk = sfr_inw(ATU_TCNT) + TICK_RATE - 1;
    }
    return (((clk + TOTAL_TICKCK)*(MSEC*1000.0))/(TICK_RATE));
}
#else
UW tickck_read(void)
{
    return 0L;
}
#endif

#endif

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
* VBR handling (IAR EW/ICC)
*
******************************************************************************/

void set_vbr(void **base)
{
    asm("   ldc     r4,vbr  ");
}

#endif

/* end */
