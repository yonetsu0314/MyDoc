/****************************************************************************
* �V���A�����o�̓h���C�o�iSH7055�j                                          *
*                                                                           *
*  File name : nos7055.c                                                    *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -cp=sh2e -op=1 -def=SH2E,CLK=? nos7055.c       (Renesas C) *
*                                                                           *
* 05/Feb/2001  �쐬                                                         *
* 27/Mar/2001  GAIO/XASSV �Ή�                                           TA *
* 12/Sep/2001 ����n�̔��f��������                                       TA *
* 27/Dec/2002 V4�}�N���̕s�v���A esle �� V4 �ƌ��Ȃ��悤���������ւ�   KA *
* 26/Feb/2003 char/unsigned char �̃L���X�g��ǉ�(���[�j���O�̍폜)      KA *
* 31/Jul/2003 BRR�ݒ�� �E�F�C�g�}��                                     KA *
****************************************************************************/

/*

�œK���R���p�C��

          �����݃n���h�����܂܂��̂ŁA�K���œK���I�v�V������t���ăR���p�C
          �����Ă��������B

�`���l���ԍ��̎w��

          CH �}�N���ŁA�_���`���l���ԍ����w��ł��܂��B�i���w�莞�́ACH=0�j
          CH �� 0�`4 ���w��ł��܂��B

          CH0, CH1 �}�N���ŁA�����`���l���ԍ���ύX�ł��܂��B
          CH0 ���`����ƁA�����`���l��0 ���g���悤�ɃR���p�C������܂��B
          CH1 ���`����ƁA�����`���l��1 ���g���悤�ɃR���p�C������܂��B

          �`���l��0 �ƃ`���l��1 �̗����������N����ꍇ�́A�d�����Ȃ����W���[
          �����ƂȂ�悤�R���p�C�����Ă��������B

          (��) shc <options> nos7055.c  ----------------------- �`���l��0
               shc <options> -def=CH=1 -ob=nos7ch1.o nos7055.c  �`���l��1

�{�[���[�g�W�F�l���[�^

          �����N���b�N�̃f�t�H���g�� 40 MHz �ł��B�ύX����ꍇ �� CLK �}�N
          �����`���ăR���p�C�����Ă��������B�l�� [Hz] �P�ʂ̎��g���ł��B

          (��) shc <options> -def=CLK=24576000 nos7055.c ------ 24.576 MHz
                                  ~~~~~~~~~~~~
����M�o�b�t�@�T�C�Y�̎w��

          ��M�^���M�o�b�t�@�̃T�C�Y���ABUFSZ �}�N���ɂ���`���ăR���p�C��
          ���Ă��������B��M�o�b�t�@�ƈقȂ�T�C�Y�̑��M�o�b�t�@��p����ꍇ
          �́A����� TXBUFSZ �}�N�����`���Ă��������B
          ���w�莞�́A��M�^���M�o�b�t�@�� 1024 �o�C�g�ƂȂ�܂��B

          (��) ��M�o�b�t�@, ���M�o�b�t�@�� 128 �o�C�g BUFSZ=128
          (��) ��M�o�b�t�@128, ���M�o�b�t�@ 64 �o�C�g BUFSZ=128, TXBUFSZ=64

�\�t�g�E�F�A�t���[����

          XON/XOFF �L�����N�^�ɂ��t���[�����p���Ȃ��ꍇ�ANOXON �}�N�����
          �`���ăR���p�C�����Ă��������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B


�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=5 nos7055.c --- �����ݗD�揇�� 5
                                  ~~~~
�|�[�g�̏�����

          SCI �ƌ��p�ɂȂ��Ă���|�[�g�́A�O���ŁA���̗�̗l�ɏ��������Ă�
          �������B�i���L�̗�ł́A�|�[�gA �̑��̒[�q�͓��͂Ɖ���j

          #include "nos7055.h"
          sfr_setw(PA_DR, PA_TXD0); ---------------- TXD0�[�q"1"(��ڰ�OFF)
          sfr_setw(PFC_PAIORH, PA_TXD0); ----------- TXD0�[�q�o�͕���
          sfr_outw(PFC_PACRH, 0x5000); ------------- RXD0,TXD0�[�q�ɐݒ�

���T�|�[�g�@�\

          �������������[�h�݂̂ŁA�N���b�N���������[�h�̓T�|�[�g���Ă��܂���B
          �����{�[���[�g�W�F�l���[�^�݂̂ŁA�O���N���b�N���o�͂̓T�|�[�g����
          ���܂���B
          RTS �M���o��  CTS �M�����͂ɂ��n�[�h�E�F�A�t���[����̓T�|�[�g��
          �Ă��܂���B
          DTR �M���o�́ADSR �M�����́ACD �M�����͂̓T�|�[�g���Ă��܂���B
*/

#include <stdlib.h>
#include <string.h>

#ifndef SH2E
#define SH2E
#endif

#ifdef V2
 #include "norti.h"
 #define tslp_tsk(p) wai_tsk(p)
#elif defined(V3)
 #include "norti3.h"
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
#endif

#include "nosio.h"
#include "sh7055.h"
#include "nos7055.h"

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP          7           /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         40*MHz      /* CPU�N���b�N[Hz] */
#endif

/* ����M�o�b�t�@�T�C�Y�̒�` */

#ifndef BUFSZ
#define BUFSZ       1024        /* ��M�o�b�t�@�� */
#endif
#ifndef TXBUFSZ
#define TXBUFSZ     BUFSZ       /* ���M�o�b�t�@�� */
#endif

/* �t���[����̒萔 */

#define XOFF_LEN    (BUFSZ*3/4) /* XOFF���M�o�b�t�@�� */
#define XON_LEN     (BUFSZ*1/4) /* XON ���M�o�b�t�@�� */
#define RTSOFF_LEN  (BUFSZ-18)  /* RTS�M��OFF�o�b�t�@�� */
#define RTSON_LEN   (BUFSZ-19)  /* RTS�M��ON �o�b�t�@�� */

#define XOFF        0x13        /* ���M��~�L�����N�^�iCTRL-S�j*/
#define XON         0x11        /* ���M�ĊJ�L�����N�^�iCTRL-Q�j*/

/* �_���`���l���ԍ��iCH=0�`4�j*/

#ifndef CH
#define CH          0           /* ���w��̏ꍇ, �_���`���l����0 */
#endif

/* �����n�̍��ق̒��� */

#if defined(GAIO)               /* XASSV */
 #include <ht1_inline.h>
#elif defined(__HITACHI__)      /* Renesas C */
 #include <machine.h>
#endif

/* �����`���l���ԍ��iCH0�`CH4�j*/

#ifndef CH0
#ifndef CH1
#ifndef CH2
#ifndef CH3
#ifndef CH4
#if (CH==0)
#define CH0             /* ���w��̏ꍇ, �_���`���l��0 �̕����`���l����0 */
#elif (CH==1)
#define CH1             /* ���w��̏ꍇ, �_���`���l��1 �̕����`���l����1 */
#elif (CH==2)
#define CH2             /* ���w��̏ꍇ, �_���`���l��2 �̕����`���l����2 */
#elif (CH==3)
#define CH3             /* ���w��̏ꍇ, �_���`���l��3 �̕����`���l����3 */
#elif (CH==4)
#define CH4             /* ���w��̏ꍇ, �_���`���l��4 �̕����`���l����4 */
#else
#error Not Supported channel
#endif
#endif
#endif
#endif
#endif
#endif

/* CHx �ɍ��킹���Ē�` */

#ifdef CH0
#define SCI_SMR     SCI_SMR0    /* �V���A�����[�h���W�X�^ */
#define SCI_BRR     SCI_BRR0    /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR     SCI_SCR0    /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR     SCI_TDR0    /* �g�����X�~�b�g�f�[�^���W�X�^ */
#define SCI_SSR     SCI_SSR0    /* �V���A���X�e�[�^�X���W�X�^ */
#define SCI_RDR     SCI_RDR0    /* ���V�[�u�f�[�^���W�X�^ */
#define INT_ERI     INT_ERI0    /* ��M�G���[�����݃x�N�^�ԍ� */
#define INT_RXI     INT_RXI0    /* ��M���������݃x�N�^�ԍ� */
#define INT_TXI     INT_TXI0    /* ���M�f�[�^�G���v�e�B�����݃x�N�^�ԍ� */
#define INT_TEI     INT_TEI0    /* ���M���������݃x�N�^�ԍ� */
#define IO_PADRL    PA_DR       /* �V���A���M���̂���R���g���[�����W�X�^ */
#define PT_RXD      PA_RXD0     /* RxD ���͐M���̃r�b�g */
#define PT_TXD      PA_TXD0     /* TxD ���͐M���̃r�b�g */
#endif

#ifdef CH1
#define SCI_SMR     SCI_SMR1
#define SCI_BRR     SCI_BRR1
#define SCI_SCR     SCI_SCR1
#define SCI_TDR     SCI_TDR1
#define SCI_SSR     SCI_SSR1
#define SCI_RDR     SCI_RDR1
#define INT_ERI     INT_ERI1
#define INT_RXI     INT_RXI1
#define INT_TXI     INT_TXI1
#define INT_TEI     INT_TEI1
#define IO_PADRL    PC_DR
#define PT_RXD      PC_RXD1
#define PT_TXD      PC_TXD1
#endif

#ifdef CH2
#define SCI_SMR     SCI_SMR2
#define SCI_BRR     SCI_BRR2
#define SCI_SCR     SCI_SCR2
#define SCI_TDR     SCI_TDR2
#define SCI_SSR     SCI_SSR2
#define SCI_RDR     SCI_RDR2
#define INT_ERI     INT_ERI2
#define INT_RXI     INT_RXI2
#define INT_TXI     INT_TXI2
#define INT_TEI     INT_TEI2
#define IO_PADRL    PC_DR
#define PT_RXD      PC_RXD2
#define PT_TXD      PC_TXD2
#endif

#ifdef CH3
#define SCI_SMR     SCI_SMR3
#define SCI_BRR     SCI_BRR3
#define SCI_SCR     SCI_SCR3
#define SCI_TDR     SCI_TDR3
#define SCI_SSR     SCI_SSR3
#define SCI_RDR     SCI_RDR3
#define INT_ERI     INT_ERI3
#define INT_RXI     INT_RXI3
#define INT_TXI     INT_TXI3
#define INT_TEI     INT_TEI3
#define IO_PADRL    PB_DR
#define PT_RXD      PB_RXD3
#define PT_TXD      PB_TXD3
#endif

#ifdef CH4
#define SCI_SMR     SCI_SMR4
#define SCI_BRR     SCI_BRR4
#define SCI_SCR     SCI_SCR4
#define SCI_TDR     SCI_TDR4
#define SCI_SSR     SCI_SSR4
#define SCI_RDR     SCI_RDR4
#define INT_ERI     INT_ERI4
#define INT_RXI     INT_RXI4
#define INT_TXI     INT_TXI4
#define INT_TEI     INT_TEI4
#define IO_PADRL    PB_DR
#define PT_RXD      PB_RXD4
#define PT_TXD      PB_TXD4
#endif

/* �V���A�����o�͐���u���b�N */

extern T_SIO SIO[];

/* �����ϐ� */

static UB txbuf[TXBUFSZ];       /* ���M�o�b�t�@ */
static UB rxbuf[BUFSZ*2];       /* ��M�o�b�t�@, ��M�X�e�[�^�X�o�b�t�@ */

/* �����݃n���h����`��� */

#ifdef V4
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
INTHDR int_sio0_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio0_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio0_te };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
INTHDR int_sio1_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio1_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio1_te };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
INTHDR int_sio2_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio2_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio2_te };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
INTHDR int_sio3_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio3_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio3_te };
#elif (CH==4)
INTHDR int_sio4_rx(void);
INTHDR int_sio4_tx(void);
INTHDR int_sio4_te(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio4_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio4_tx };
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio4_te };
#endif
#else
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
INTHDR int_sio0_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio0_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio0_te };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
INTHDR int_sio1_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio1_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio1_te };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
INTHDR int_sio2_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio2_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio2_te };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
INTHDR int_sio3_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio3_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio3_te };
#elif (CH==4)
INTHDR int_sio4_rx(void);
INTHDR int_sio4_tx(void);
INTHDR int_sio4_te(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio4_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio4_tx };
static const T_DINT dint_sio_te = { TA_HLNG, int_sio4_te };
#endif
#endif

#if defined(__HITACHI__)
#if (CH==0)
#pragma interrupt(int_sio0_rx)
#pragma interrupt(int_sio0_tx)
#pragma interrupt(int_sio0_te)
#elif (CH==1)
#pragma interrupt(int_sio1_rx)
#pragma interrupt(int_sio1_tx)
#pragma interrupt(int_sio1_te)
#elif (CH==2)
#pragma interrupt(int_sio2_rx)
#pragma interrupt(int_sio2_tx)
#pragma interrupt(int_sio2_te)
#elif (CH==3)
#pragma interrupt(int_sio3_rx)
#pragma interrupt(int_sio3_tx)
#pragma interrupt(int_sio3_te)
#elif (CH==4)
#pragma interrupt(int_sio4_rx)
#pragma interrupt(int_sio4_tx)
#pragma interrupt(int_sio4_te)
#endif
#endif

/*****************************************************************************
* �V���A�����o�͐���u���b�N�������i�����֐��j
*
******************************************************************************/

static void init_buf(void)
{
    int init;

    /* ����u���b�N�N���A */

    init = SIO[CH].flag & TSF_INIT;
    memset(&SIO[CH], 0, sizeof SIO[CH]);
    SIO[CH].flag = (UB)init;
    SIO[CH].ch = CH;

    /* �|�C���^�ރZ�b�g�A�b�v */

    SIO[CH].txbuf = txbuf;          /* ���M�o�b�t�@ */
    SIO[CH].rxbuf = rxbuf;          /* ��M�o�b�t�@ */
    SIO[CH].txputp = SIO[CH].txbuf; /* ���M�o�b�t�@�i�[�|�C���^�����l */
    SIO[CH].txgetp = SIO[CH].txbuf; /* ���M�o�b�t�@�擾�|�C���^�����l */
    SIO[CH].rxputp = SIO[CH].rxbuf; /* ��M�o�b�t�@�i�[�|�C���^�����l */
    SIO[CH].rxgetp = SIO[CH].rxbuf; /* ��M�o�b�t�@�擾�|�C���^�����l */
}

/*****************************************************************************
* �X�e�[�^�X���� & �ҏW�i�����֐��j
*
*    bit7                                      bit0
*   [ DSR | BD  | FE  | OE  | PE  |TXEMP| CTS | CD  ]
*     (1)                                       (1)
*
* DSR, CD �� �����̂ŏ펞 1 �Ƃ���
******************************************************************************/

static int get_stat(void)
{
    int stat;
    int ssr;

    /* ���݂̃X�e�[�^�X���� */

    ssr = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);

    /* ��M���̃X�e�[�^�X��OR */

    ssr |= SIO[CH].rxsts;

    /* �u���[�NOFF���o */

    if (sfr_inw(IO_PADRL) & PT_RXD) /* RxD �[�q"1" */
        ssr &= ~SSR_MPBT;           /* �u���[�N���o�� MPBT �Ɋ��蓖�� */

    /* �X�e�[�^�X�r�b�g�ҏW */

    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD �펞ON */

    if (ssr & SSR_TEND)             /* ���M�� */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)              /* �p���e�B�G���[ */
        stat |= TSIO_PE;
    if (ssr & SSR_ORER)             /* �I�[�o�����G���[ */
        stat |= TSIO_OE;
    if (ssr & SSR_FER)              /* �t���[�~���O�G���[ */
        stat |= TSIO_FE;
    if (ssr & SSR_MPBT)             /* �u���[�N���o */
        stat |= TSIO_BD;

    return stat;
}

/*****************************************************************************
* �r�h�n�f�o�C�X�������i�����֐��j
*
******************************************************************************/

static BOOL init_sio(UB mode, UW baud)
{
    int cks;
    UW brr;

    /* ���Z�b�g */

    sfr_out(SCI_SCR, 0x00);

    /* �{�[���[�g�ݒ�l�v�Z */

    cks = 0;
    for (brr = (CLK) / (baud * 32 * 2); brr > 0x100L; brr >>= 2)
    {   if (++cks > 3)
            return FALSE;
    }

    /* ���[�h�^�{�[���[�g�ݒ� */

    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* �{�[���[�g��1�r�b�g���Ԍo�ߑ҂� */
    tslp_tsk(2);

    return TRUE;
}

/*****************************************************************************
* �V���A�������݋֎~�i�����֐��j
*
******************************************************************************/

static void dis_int_sio(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_clrw(INTC_IPRK, 0xf000);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRK, 0x0f00);
  #endif
  #ifdef CH2
    sfr_clrw(INTC_IPRK, 0x00f0);
  #endif
  #ifdef CH3
    sfr_clrw(INTC_IPRK, 0x000f);
  #endif
  #ifdef CH4
    sfr_clrw(INTC_IPRL, 0xf000);
  #endif
    vset_psw(psw);
}

/*****************************************************************************
* �V���A�������݋��i�����֐��j
*
******************************************************************************/

static void ena_int_sio(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_setw(INTC_IPRK, IP << 12);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRK, IP << 8);
  #endif
  #ifdef CH2
    sfr_setw(INTC_IPRK, IP << 4);
  #endif
  #ifdef CH3
    sfr_setw(INTC_IPRK, IP);
  #endif
  #ifdef CH4
    sfr_setw(INTC_IPRL, IP << 12);
  #endif
    vset_psw(psw);
}

/*****************************************************************************
* ���M�o�b�t�@�N���A�i�����֐��j
*
******************************************************************************/

static void clr_txbuf(void)
{
    SIO[CH].txcnt = 0;
    SIO[CH].txgetp = SIO[CH].txputp;
}

/*****************************************************************************
* ��M�o�b�t�@�N���A�i�����֐��j
*
******************************************************************************/

static void clr_rxbuf(void)
{
    /* ��M�o�b�t�@�N���A */

    SIO[CH].rxsts = 0;
    SIO[CH].oldsts = 0;
    SIO[CH].eotcnt = 0;
    SIO[CH].rxcnt = 0;
    SIO[CH].rxgetp = SIO[CH].rxputp;

    /* XON/OFF�t���[����̉��� */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_TXOFF)
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF ���M�v���Z�b�g */
        sfr_set(SCI_SCR, SCR_TIE);  /* ���M�����݋��� */
    }
  #endif
}

/*****************************************************************************
* ���M�o�b�t�@�ւP�����i�[�i�����֐��j
*
* �o�b�t�@���t�Ŋi�[�ł��Ȃ������ꍇ�́AFALSE ��Ԃ��B
******************************************************************************/

static BOOL put_txbuf(UB c)
{
    UB *p;

    /* �o�b�t�@�������� + 1 */
    /* �o�b�t�@���t�`�F�b�N */

    if (++SIO[CH].txcnt > TXBUFSZ)
    {   SIO[CH].txcnt = TXBUFSZ;
        return FALSE;
    }

    /* �o�b�t�@�֊i�[ */

    p = SIO[CH].txputp;
    *p = c;

    /* �i�[�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txputp = p;

    /* ���M�����݋��� */

    sfr_set(SCI_SCR, SCR_TIE);

    return TRUE;
}

/*****************************************************************************
* ��M�o�b�t�@�֎�M����/�X�e�[�^�X�i�[�i�����֐��j
*
******************************************************************************/

static void put_rxbuf(void)
{
    int cnt;
    UB *p;

    /* �A���u���[�N�Ȃ�i�[���Ȃ� */

    if ((SIO[CH].rxsts & SSR_MPBT)
     && (SIO[CH].oldsts & SSR_MPBT))
        return;

    /* �o�b�t�@���t�`�F�b�N */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
        SIO[CH].rxsts |= SSR_RDRF; /* �I�[�o�t���[�� RDRF �r�b�g�Ɋ��蓖�� */

    /* �o�b�t�@�������� + 1 */

    SIO[CH].rxcnt = (UH)cnt;

    /* XON/OFF�t���[���� */

  #ifndef NOXON
    if ((cnt == XOFF_LEN)
     && ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON))
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF ���M�v���Z�b�g */
        sfr_set(SCI_SCR, SCR_TIE);  /* ���M�����݋��� */
    }
  #endif

    /* �o�b�t�@�֊i�[ */
    /* �I�[�������o+1 */

    p = SIO[CH].rxputp;
    if ((*p = SIO[CH].rxchr) == SIO[CH].eot)
        SIO[CH].eotcnt++;
    *(p + BUFSZ) = SIO[CH].rxsts;

    /* �i�[�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].rxbuf + BUFSZ)
        p = SIO[CH].rxbuf;
    SIO[CH].rxputp = p;
}

/*****************************************************************************
* ��M�o�b�t�@����P�����擾�i�����֐��j
*
* �o�b�t�@��Ŏ擾�ł��Ȃ������ꍇ�́A-1 ��Ԃ��B
******************************************************************************/

static int get_rxbuf(UB *c)
{
    int cnt;
    int sts;
    UB *p;

    /* ��M�o�b�t�@��`�F�b�N */

    cnt = SIO[CH].rxcnt;
    if (--cnt == -1)
        return cnt;

    /* ��M�o�b�t�@�������� - 1 */

    SIO[CH].rxcnt = (UH)cnt;

    /* XON/OFF�t���[���� */

  #ifndef NOXON
    if ((cnt == XON_LEN)
     && (SIO[CH].flag & TSF_TXOFF))
    {   SIO[CH].flag |= TSF_TXREQ;      /* XON/XOFF ���M�v���Z�b�g */
        sfr_set(SCI_SCR, SCR_TIE);      /* ���M�����݋��� */
    }
  #endif

    /* ��M�o�b�t�@����擾 */
    /* �I�[�������o��-1 */

    p = SIO[CH].rxgetp;
    if ((*c = *p) == SIO[CH].eot)
        SIO[CH].eotcnt--;
    sts = *(p + BUFSZ);

    /* �擾�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].rxbuf + BUFSZ)
        p = SIO[CH].rxbuf;
    SIO[CH].rxgetp = p;

    return sts;
}

/*****************************************************************************
* ��M�����݃n���h���{�́i�����֐��j
*
******************************************************************************/

static void rx_int(void)
{
    int sts;
    int chr;
    int tid;

    /* ��M�����ƃX�e�[�^�X����� */

    chr = sfr_in(SCI_RDR);
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);

    /* ��M�����݂��N���A */

    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));

    /* �u���[�N���o */

    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_inw(IO_PADRL) & PT_RXD))  /* RxD �[�q"0" */
        sts |= SSR_MPBT;                /* �u���[�N���o�� MPBT �Ɋ��蓖�� */
    SIO[CH].oldsts = SIO[CH].rxsts;     /* �O��̎�M�X�e�[�^�X�L�� */
    SIO[CH].rxsts = (UB)sts;
    SIO[CH].rxchr = (UB)chr;

    /* XON/OFF�t���[����L��̏ꍇ */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_XON)
    {
        /* XOFF��M�Ȃ瑗�M���f������ */

        if (chr == XOFF)
        {   SIO[CH].flag |= TSF_RXOFF;
            return;
        }

        /* XON ��M�Ȃ瑗�M�ĊJ������ */

        if (chr == XON)
        {   SIO[CH].flag &= ~TSF_RXOFF;
            sfr_set(SCI_SCR, SCR_TIE);      /* ���M�����݋��� */
            return;
        }
    }
  #endif

    /* ��M�o�b�t�@�֊i�[ */

    put_rxbuf();

    /* ��M�҂����� */

    if ((tid = SIO[CH].rxtid) != 0)
        wup_tsk((ID)tid);
}

/*****************************************************************************
* ��M�����݃n���h��
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_rx(void)
#elif (CH==1)
INTHDR int_sio1_rx(void)
#elif (CH==2)
INTHDR int_sio2_rx(void)
#elif (CH==3)
INTHDR int_sio3_rx(void)
#elif (CH==4)
INTHDR int_sio4_rx(void)
#endif
{
    ent_int();              /* �����݃n���h���̊J�n */
    rx_int();               /* ��M�����݃n���h���{�� */
    ret_int();              /* �����݃n���h�����畜�A���� */
}

/*****************************************************************************
* ���M�����݃n���h���{�́i�����֐��j
*
******************************************************************************/

static void tx_int(void)
{
    UB *p;
    int tid;

    /* ���M�f�B�Z�[�u�����Ȃ瑗�M���Ȃ� */

    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;

    /* ��M������˗����ꂽXON/XOFF�̑��M��D�� */

  #ifndef NOXON
    if (SIO[CH].flag & TSF_TXREQ)
    {   SIO[CH].flag &= ~TSF_TXREQ;
        if (SIO[CH].flag & TSF_TXOFF)
        {   SIO[CH].flag &= ~TSF_TXOFF;
            sfr_out(SCI_TDR, XON);
        }
        else
        {   SIO[CH].flag |= TSF_TXOFF;
            sfr_out(SCI_TDR, XOFF);
        }
        sfr_clr(SCI_SSR, SSR_TDRE);
        return;
    }

    /* XOFF��M���Ă����瑗�M�s�� */

    if (SIO[CH].flag & TSF_RXOFF)
        goto L2;
  #endif

    /* ���M�o�b�t�@�������� - 1 */
    /* ���M�o�b�t�@��Ȃ瑗�M�s�� */

    if (--SIO[CH].txcnt == (UH)-1)
    {
        SIO[CH].txcnt = 0;

        /* ���M�I�������݋��� */

        sfr_set(SCI_SCR, SCR_TEIE);

        /* ���M�����݋֎~ */
    L2:
        sfr_clr(SCI_SCR, SCR_TIE);
        return;
    }

    /* �P�������M */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);
    sfr_clr(SCI_SSR, SSR_TDRE);

    /* �擾�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* ���M�҂����� */

    if ((tid = SIO[CH].txtid) != 0)
        wup_tsk((ID)tid);
}

/*****************************************************************************
* ���M�����݃n���h��
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_tx(void)
#elif (CH==1)
INTHDR int_sio1_tx(void)
#elif (CH==2)
INTHDR int_sio2_tx(void)
#elif (CH==3)
INTHDR int_sio3_tx(void)
#elif (CH==4)
INTHDR int_sio4_tx(void)
#endif
{
    ent_int();              /* �����݃n���h���̊J�n */
    tx_int();               /* ���M�����݃n���h���{�� */
    ret_int();              /* �����݃n���h�����畜�A���� */
}

/*****************************************************************************
* �����݃n���h�����M�I�������i�����֐��j
*
******************************************************************************/

static void te_int(void)
{
    /* ���M�I���҂����� */

    if (SIO[CH].tetid != 0)
        wup_tsk(SIO[CH].tetid);

    /* ���M�I�������݋֎~ */

    sfr_clr(SCI_SSR, SSR_TEND);
    sfr_clr(SCI_SCR, SCR_TEIE);
}

/*****************************************************************************
* ���M�I�������݃n���h��
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0_te(void)
#elif (CH==1)
INTHDR int_sio1_te(void)
#elif (CH==2)
INTHDR int_sio2_te(void)
#elif (CH==3)
INTHDR int_sio3_te(void)
#elif (CH==4)
INTHDR int_sio4_te(void)
#endif
{
    ent_int();                  /* �����݃n���h���̊J�n */
    te_int();                   /* ���M�I������ */
    ret_int();                  /* �����݃n���h�����畜�A���� */
}

/*****************************************************************************
* �������p�����[�^��́i�����֐��j
*
******************************************************************************/

static BOOL set_param(const B *s, UB *mode, UW *baud)
{
    B c;
    UW b;

    /* ���[�h���� */

    *mode = 0;
    if      (strstr((const char *)s, (char *)"PE") != NULL)   *mode |= SMR_PE;
    else if (strstr((const char *)s, (char *)"PO") != NULL)   *mode |= SMR_PE|SMR_OE;
    if      (strstr((const char *)s, (char *)"S2") != NULL)   *mode |= SMR_STOP;
    if      (strstr((const char *)s, (char *)"B7") != NULL)   *mode |= SMR_CHR;

    /* �t���[����L������ */

    if (strstr((const char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((const char *)s, (char *)"RTS") != NULL)   SIO[CH].flag |= TSF_RTS;

    /* �I�[�����w�蔻�� */

    if (strstr((const char *)s, (char *)"ETX") != NULL)   SIO[CH].eot = 0x03;
    if (strstr((const char *)s, (char *)"EOT") != NULL)   SIO[CH].eot = 0x04;
    if (strstr((const char *)s, (char *)"CR" ) != NULL)   SIO[CH].eot = 0x0d;
    if (strstr((const char *)s, (char *)"LF" ) != NULL)   SIO[CH].eot = 0x0a;

    /* �{�[���[�g���� */
    /* 3���ȏ�̐��l�������ĕϊ� */

    for (;;)
    {
        c = *s++;
        if (c == '\0')
        {   *baud = 9600L;
            break;
        }
        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;
        c = *s++;
        if (c < '0' || c > '9')
            continue;

        s -= 3;
        b = 0L;
        for (;;)
        {   c = *s++;
            if (c < '0' || c > '9')
                break;
            b = b * 10 + (c - '0');
        }
        *baud = b;
        break;
    }
    return TRUE;
}

/*****************************************************************************
* �V���A�����o�͏�����
*
******************************************************************************/

#if (CH==0)
ER ini_sio0(const B *param)
#elif (CH==1)
ER ini_sio1(const B *param)
#elif (CH==2)
ER ini_sio2(const B *param)
#elif (CH==3)
ER ini_sio3(const B *param)
#elif (CH==4)
ER ini_sio4(const B *param)
#endif
{
    UW baud;
    UB mode;

    dis_int_sio();

    /* ����u���b�N������ */

    init_buf();

    /* �p�����[�^��� */

    if (!set_param(param, &mode, &baud))
        return E_PAR;

    /* �f�o�C�X������ */

    if (!init_sio(mode, baud))
        return E_PAR;

    /* �����݃n���h���̒�` */

    SIO[CH].flag |= TSF_INIT;
  #ifdef V4
    def_inh(INT_ERI, &dinh_sio_rx);
    def_inh(INT_RXI, &dinh_sio_rx);
    def_inh(INT_TXI, &dinh_sio_tx);
    def_inh(INT_TEI, &dinh_sio_te);
  #else
    def_int(INT_ERI, &dint_sio_rx);
    def_int(INT_RXI, &dint_sio_rx);
    def_int(INT_TXI, &dint_sio_tx);
    def_int(INT_TEI, &dint_sio_te);
  #endif

    /* ��M�����݋��� */

    sfr_set(SCI_SCR, SCR_RIE);
    ena_int_sio();
    return E_OK;
}

/*****************************************************************************
* �V���A�����o�͏I��
*
******************************************************************************/

#if (CH==0)
void ext_sio0(void)
#elif (CH==1)
void ext_sio1(void)
#elif (CH==2)
void ext_sio2(void)
#elif (CH==3)
void ext_sio3(void)
#elif (CH==4)
void ext_sio4(void)
#endif
{
    if (!(SIO[CH].flag & TSF_INIT))     /* ���������Ȃ牽�����Ȃ� */
        return;
    dis_int_sio();                      /* �V���A�������݋֎~ */
    sfr_out(SCI_SCR, 0x00);             /* ����M�֎~ */
  #ifdef V4
    def_inh(INT_ERI, (T_DINH *)NADR);   /* �����ݒ�`�������� */
    def_inh(INT_RXI, (T_DINH *)NADR);
    def_inh(INT_TXI, (T_DINH *)NADR);
    def_inh(INT_TEI, (T_DINH *)NADR);
  #else
    def_int(INT_ERI, (T_DINT *)NADR);   /* �����ݒ�`�������� */
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
    def_int(INT_TEI, (T_DINT *)NADR);
  #endif
    SIO[CH].flag &= ~TSF_INIT;          /* �������ς݃t���O�N���A */
}

/*****************************************************************************
* �V���A���P��������
*
******************************************************************************/

#if (CH==0)
ER get_sio0(UB *c, TMO tmout)
#elif (CH==1)
ER get_sio1(UB *c, TMO tmout)
#elif (CH==2)
ER get_sio2(UB *c, TMO tmout)
#elif (CH==3)
ER get_sio3(UB *c, TMO tmout)
#elif (CH==4)
ER get_sio4(UB *c, TMO tmout)
#endif
{
    ER ercd;
    int sts;

    for (;;)
    {
        chg_ims(IP);

        /* ��M�o�b�t�@����P�������� */

        sts = get_rxbuf(c);

        if (sts != -1)              /* ��M�����������ꍇ */
        {
            chg_ims(0);

            /* ��M�G���[���� */

            if (sts & (SSR_RDRF|SSR_ORER|SSR_MPBT|SSR_FER|SSR_PER))
            {   if (sts & SSR_RDRF)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_ORER)
                    ercd = EV_SIOORN;
                else if (sts & SSR_MPBT)
                    ercd = EV_SIOBRK;
                else if (sts & SSR_FER)
                    ercd = EV_SIOFRM;
                else /* SSR_PER */
                    ercd = EV_SIOPTY;
            }
            else
                ercd = E_OK;
            return ercd;
        }

        /* ��M�����ݑ҂� */

        SIO[CH].rxtid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* ������ wup_tsk ���ꂽ�ꍇ�̑΍� */
        if (ercd)
            return ercd;            /* �^�C���A�E�g�I�� */
    }
}

/*****************************************************************************
* �V���A���P�����o��
*
******************************************************************************/

#if (CH==0)
ER put_sio0(UB c, TMO tmout)
#elif (CH==1)
ER put_sio1(UB c, TMO tmout)
#elif (CH==2)
ER put_sio2(UB c, TMO tmout)
#elif (CH==3)
ER put_sio3(UB c, TMO tmout)
#elif (CH==4)
ER put_sio4(UB c, TMO tmout)
#endif
{
    ER ercd;

    for (;;)
    {
        chg_ims(IP);

        /* ���M�o�b�t�@�ւP�����i�[ */

        if (put_txbuf(c))           /* �i�[�ł����ꍇ */
        {   chg_ims(0);
            return E_OK;            /* ����I�� */
        }

        /* ���M�����ݑ҂� */

        SIO[CH].txtid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* ������ wup_tsk ���ꂽ�ꍇ�̑΍� */
        if (ercd)
            return ercd;            /* �^�C���A�E�g�I�� */
    }
}

/*****************************************************************************
* �V���A�����o�͐���
*
******************************************************************************/

#if (CH==0)
ER ctl_sio0(UH fncd)
#elif (CH==1)
ER ctl_sio1(UH fncd)
#elif (CH==2)
ER ctl_sio2(UH fncd)
#elif (CH==3)
ER ctl_sio3(UH fncd)
#elif (CH==4)
ER ctl_sio4(UH fncd)
#endif
{
    UB c;

    chg_ims(IP);

    /* �o�b�t�@�N���A */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* �u���[�N���M */

    if (fncd & TSIO_SBON)
    {   fncd |= TSIO_TXD;
        sfr_clrw(IO_PADRL, PT_TXD);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        sfr_setw(IO_PADRL, PT_TXD);
    }

    /* �V���A�����[�h�ύX */

    if (fncd & (TSIO_RXE|TSIO_RXD|TSIO_TXE|TSIO_TXD))
    {   c = sfr_in(SCI_SCR);
        if (fncd & TSIO_RXE)        c |=  SCR_RE;
        if (fncd & TSIO_RXD)        c &= ~SCR_RE;
        if (fncd & TSIO_TXE)        c |=  SCR_TE;
        if (fncd & TSIO_TXD)        c &= ~SCR_TE;
        sfr_out(SCI_SCR, c);
    }

    chg_ims(0);
    return E_OK;
}

/*****************************************************************************
* �V���A�����o�͏�ԎQ��
*
******************************************************************************/

#if (CH==0)
ER ref_sio0(T_SIOS *pk_sios)
#elif (CH==1)
ER ref_sio1(T_SIOS *pk_sios)
#elif (CH==2)
ER ref_sio2(T_SIOS *pk_sios)
#elif (CH==3)
ER ref_sio3(T_SIOS *pk_sios)
#elif (CH==4)
ER ref_sio4(T_SIOS *pk_sios)
#endif
{
    int stat;

    chg_ims(IP);

    stat = get_stat();
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = (UB)stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(0);
    return E_OK;
}

/*****************************************************************************
* �V���A�����M�o�b�t�@�t���b�V��
*
******************************************************************************/

#if (CH==0)
ER fls_sio0(TMO tmout)
#elif (CH==1)
ER fls_sio1(TMO tmout)
#elif (CH==2)
ER fls_sio2(TMO tmout)
#elif (CH==3)
ER fls_sio3(TMO tmout)
#elif (CH==4)
ER fls_sio4(TMO tmout)
#endif
{
    ER ercd;

    for (;;)
    {
        chg_ims(IP);

        if (SIO[CH].txcnt == 0)
        {   if (sfr_in(SCI_SSR) & SSR_TEND)
            {   chg_ims(0);
                return E_OK;            /* ����I�� */
            }
        }

        /* ���M�I�������ݑ҂� */

        SIO[CH].tetid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* ������ wup_tsk ���ꂽ�ꍇ�̑΍� */
        if (ercd)
            return ercd;                /* �^�C���A�E�g�I�� */
    }
}

/* end */
