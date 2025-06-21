/****************************************************************************
* �V���A�����o�̓h���C�o�iSH7615�j                                          *
*                                                                           *
*  File name : nos7615.c                                                    *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                 *
*                                                                           *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=? nos7615.c        (Renesas C)      *
*            gcc -c -m2 -O2 -DCLK=? nos7615.c              (exeGCC)         *
*            ccsh -c -cpu=sh_ii -ansi -O -DCLK=? nos7615.c (GHS)            *
*                                                                           *
* 17/Apr/2000 �쐬                                                    MiSPO *
* 30/Oct/2000 IAR EW �Ή�                                                   *
* 12/Sep/2001 ����n�̔��f��������                                       TA *
* 27/Dec/2002 V4�}�N���̕s�v���A esle �� V4 �ƌ��Ȃ��悤���������ւ�   KA *
* 25/Feb/2003 "Green Hills" �R���p�C���̔��f�� __ghs �ł���悤�ɏC��    KA *
* 31/Jul/2003 BRR�ݒ�� �E�F�C�g�}��                                     KA *
****************************************************************************/

/*

�œK���R���p�C��

          �����݃n���h�����܂܂��̂ŁA�K���œK���I�v�V������t���ăR���p�C
          �����Ă��������B

�`���l���ԍ��̎w��

          CH �}�N���ŁA�_���`���l���ԍ����w��ł��܂��B�i���w�莞�́ACH=0�j
          CH �� 0�`3 ���w��ł��܂��B

          CH0, CH1 �}�N���ŁA�����`���l���ԍ���ύX�ł��܂��B
          CH0 ���`����ƁAFIFO �t�� SCIF1 ���g���悤�ɃR���p�C������܂��B
          CH1 ���`����ƁAFIFO �t�� SCIF2 ���g���悤�ɃR���p�C������܂��B

          �`���l��0 �ƃ`���l��1 �̗����������N����ꍇ�́A�d�����Ȃ����W���[
          �����ƂȂ�悤�R���p�C�����Ă��������B

          (��) shc <options> nos7615.c  ����������������������� �`���l��0
               shc <options> -def=CH=1 -ob=nos7ch1.o nos7615.c  �`���l��1

          (��) gcc <options> nos7615.c  ����������������������� �`���l��0
               gcc <options> -DCH=1 -o nos7ch1.o nos7615.c ���� �`���l��1

          (��) ccsh <options> nos7615.c  ���������������������� �`���l��0
               ccsh <options> -DCH=1 -o nos7ch1.o nos7615.c ��� �`���l��1

�{�[���[�g�W�F�l���[�^

          �\�[�X�N���b�N(P��)�̃f�t�H���g�� 15 MHz �ł��B�ύX����ꍇ ��
          CLK �}�N�����`���ăR���p�C�����Ă��������B�l�� [Hz] �P�ʂ̎�
          �g���ł��B

          (��) shc <options> -def=CLK=30000000 nos7615.c ������ 30 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=30000000 nos7615.c
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

�n�[�h�E�F�A�t���[����

          �n�[�h�t���[��p���Ȃ��ꍇ�́ANOFLW �}�N����`���ăR���p�C�����Ă�
          �������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B

          CH0�ł́ASCIF ������ RTS-CTS �@�\���g���A�\�t�g�E�F�A�Ɉ˂炸�s��
          �܂��B

          ���{�o�[�W�����ł�CH1�̃t���[����͖��m�F�ł��B

               |       �W �� �� �t
          -----+---------------------------
               |RTS   RTS/PB12(154pin)
               |CTS   CTS/PB11(156pin)
          CH0  |DTR --- Not Supported
               |DSR --- Not Supported
               |DCD --- Not Supported
          -----+---------------------------
               |RTS --- Not Supported
               |CTS --- Not Supported
          CH1  |DTR --- Not Supported
               |DSR --- Not Supported
               |DCD --- Not Supported
          -----+---------------------------

          ��SCIF�n�[�h�E�F�A�t���[����ɑ΂���⑫
            SCIF�̃n�[�h�E�F�A�t���[�����FIFO�o�b�t�@����̓ǂݏo������������
            ���邩�ǂ����ɂ��s���܂��B
            �ʐM���x�������Ƃ��A�ꎞ�I�Ɏ捞���~�߂Ȃ��ƁA��M�o�b�t�@�̕���
            �I�[�o�[�t���[���Ă��܂��܂��B���̂��߁A��M�o�b�t�@�̋󂫂����Ȃ�
            �Ȃ������_�ŁA�捞���̂��̂��~���܂��i��M�����֎~�j�B
            ��������ƁAFIFO����ǂݏo���Ȃ��̂�RTS ON�̂܂܂ŕێ�����܂��B
            ���̌��M�o�b�t�@����̓ǂݏo�����i��ŋ󂫂��������Ƃ��A������
            �����܂��B��M���ĊJ����AFIFO����ǂݏo������������ƁARTS��
            �����I��OFF�ƂȂ�܂��B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=5 nos7615.c ��� �����ݗD�揇�� 5
                                  ~~~~
          (��) gcc <options> -DIP=5 nos7615.c
                               ~~~~
�|�[�g�̏�����

          SCIF �ƌ��p�ɂȂ��Ă���|�[�gB �́A�O���ŁA���̗�̗l�ɏ��������Ă�
          �������B�i���L�̗�ł́APB �̑��̒[�q�͓��͂Ɖ���j

          #include "nos7615.h"
          sfr_setw(PFC_PBCR, PBCR_RXD1); ���� RxD1 �@�\
          sfr_setw(PFC_PBCR, PBCR_TXD1); ���� TxD1 �@�\
          sfr_setw(PFC_PBCR, PBCR_RTS1); ���� ~RTS �@�\
          sfr_setw(PFC_PBCR, PBCR_CTS1); ���� ~CTS �@�\
          sfr_setw(PFC_PBCR2, PBCR_RXD2); ��� RxD2 �@�\
          sfr_setw(PFC_PBCR2, PBCR_TXD2); ��� TxD2 �@�\

���T�|�[�g�@�\

          �������������[�h�݂̂ŁA�N���b�N���������[�h�̓T�|�[�g���Ă��܂���B
          �����{�[���[�g�W�F�l���[�^�݂̂ŁA�O���N���b�N���o�͂̓T�|�[�g����
          ���܂���B
          DTR �M���o�́ADSR �M�����́ACD �M�����͂̓T�|�[�g���Ă��܂���B
          SH7615 ���L�̃}���`�v���Z�b�T���[�h�̓T�|�[�g���Ă��܂���B

���얢�m�F

          CH1�́A���얢�m�F�ł��B

*/

#include <stdlib.h>
#include <string.h>

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
#include "sh7615.h"
#include "nos7615.h"

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP          7           /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         15*MHz      /* CPU�N���b�N[Hz] */
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

/* �_���`���l���ԍ��iCH=0�`3�j*/

#ifndef CH
#define CH          0           /* ���w��̏ꍇ, �_���`���l����0 */
#endif

/* �����n�̍��ق̒��� */

#if defined(GAIO)               /* XASSV */
 #include <ht1_inline.h>
#elif defined(__GNUC__)         /* exeGCC */
#elif defined(__ghs)            /* Green Hills */
#elif defined(__TID__)          /* IAR EW/ICC */
#else                           /* Renesas C */
 #ifndef __HITACHI__
  #define __HITACHI__
 #endif
 #include <machine.h>
#endif

/* �����`���l���ԍ��iCH0,CH1�j*/

#ifndef CH0
#ifndef CH1
#if (CH==0)
#define CH0             /* ���w��̏ꍇ, �_���`���l��0 �̕����`���l����0 */
#elif (CH==1)
#define CH1             /* ���w��̏ꍇ, �_���`���l��1 �̕����`���l����1 */
#elif (CH==2)
#define CH0             /* ���w��̏ꍇ, �_���`���l��2 �̕����`���l����0 */
#else
#define CH1             /* ���w��̏ꍇ, �_���`���l��3 �̕����`���l����1 */
#endif
#endif
#endif

/* CHx �ɍ��킹���Ē�` */

#ifdef CH0
#define SCI_SMR     SCIF_SCSMR1    /* �V���A�����[�h���W�X�^ */
#define SCI_BRR     SCIF_SCBRR1    /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR     SCIF_SCSCR1    /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR     SCIF_SCFTDR1   /* �g�����X�~�b�gFIFO�f�[�^���W�X�^ */
#define SCI_SSR     SCIF_SC1SSR1   /* �V���A���X�e�[�^�X1���W�X�^ */
#define SCI_SSR2    SCIF_SC2SSR1   /* �V���A���X�e�[�^�X2���W�X�^ */
#define SCI_RDR     SCIF_SCFRDR1   /* ���V�[�uFIFO�f�[�^���W�X�^ */
#define SCI_FCR     SCIF_SCFCR1    /* FIFO�R���g���[�����W�X�^ */
#define SCI_FDR     SCIF_SCFDR1    /* FIFO�f�[�^�����W�X�^ */
#define SCI_FER     SCIF_SCFER1    /* FIFO�G���[���W�X�^ */
#define PB_TXD      PB_TXD1        /* TxD �o�͒[�q */
#define PB_RXD      PB_RXD1        /* RxD ���͒[�q */
#define PB_CTS      PB_CTS1        /* CTS ���͒[�q */
#define INT_ERI     INT_ERI1       /* ��M�G���[�����݃x�N�^�ԍ� */
#define INT_RXI     INT_RXI1       /* ��M���������݃x�N�^�ԍ� */
#define INT_TXI     INT_TXI1       /* ���M�f�[�^�G���v�e�B�����݃x�N�^�ԍ� */
#define INT_BRI     INT_BRI1       /* �u���[�N���o�x�N�^�ԍ� */
#endif

#ifdef CH1
#define SCI_SMR     SCIF_SCSMR2    /* �V���A�����[�h���W�X�^ */
#define SCI_BRR     SCIF_SCBRR2    /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR     SCIF_SCSCR2    /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR     SCIF_SCFTDR2   /* �g�����X�~�b�gFIFO�f�[�^���W�X�^ */
#define SCI_SSR     SCIF_SC1SSR2   /* �V���A���X�e�[�^�X1���W�X�^ */
#define SCI_SSR2    SCIF_SC2SSR2   /* �V���A���X�e�[�^�X2���W�X�^ */
#define SCI_RDR     SCIF_SCFRDR2   /* ���V�[�uFIFO�f�[�^���W�X�^ */
#define SCI_FCR     SCIF_SCFCR2    /* FIFO�R���g���[�����W�X�^ */
#define SCI_FDR     SCIF_SCFDR2    /* FIFO�f�[�^�����W�X�^ */
#define SCI_FER     SCIF_SCFER2    /* FIFO�G���[���W�X�^ */
#define PB_TXD      PB_TXD2        /* TxD �o�͒[�q */
#define PB_RXD      PB_RXD2        /* RxD ���͒[�q */
#define INT_ERI     INT_ERI2       /* ��M�G���[�����݃x�N�^�ԍ� */
#define INT_RXI     INT_RXI2       /* ��M���������݃x�N�^�ԍ� */
#define INT_TXI     INT_TXI2       /* ���M�f�[�^�G���v�e�B�����݃x�N�^�ԍ� */
#define INT_BRI     INT_BRI2       /* �u���[�N���o�x�N�^�ԍ� */
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
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio0_tx };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio1_tx };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio2_tx };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio3_tx };
#endif
#else
#if (CH==0)
INTHDR int_sio0_rx(void);
INTHDR int_sio0_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio0_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio0_tx };
#elif (CH==1)
INTHDR int_sio1_rx(void);
INTHDR int_sio1_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio1_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio1_tx };
#elif (CH==2)
INTHDR int_sio2_rx(void);
INTHDR int_sio2_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio2_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio2_tx };
#elif (CH==3)
INTHDR int_sio3_rx(void);
INTHDR int_sio3_tx(void);
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio3_rx };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio3_tx };
#endif
#endif

#if defined(__HITACHI__)
#if (CH==0)
#pragma interrupt(int_sio0_rx)
#pragma interrupt(int_sio0_tx)
#elif (CH==1)
#pragma interrupt(int_sio1_rx)
#pragma interrupt(int_sio1_tx)
#elif (CH==2)
#pragma interrupt(int_sio2_rx)
#pragma interrupt(int_sio2_tx)
#elif (CH==3)
#pragma interrupt(int_sio3_rx)
#pragma interrupt(int_sio3_tx)
#endif
#endif

#ifndef NOFLW
/*****************************************************************************
* RTS �M�� ON/OFF�i�����֐��j
*
******************************************************************************/

static void rts_on(void)
{
  #ifdef CH1
    UINT psw;

    psw = vdis_psw();       /* ���荞�݋֎~ */

    /* RTS ON����R�[�h��������     */

    vset_psw(psw);          /* ���荞�݋֎~���� */
  #endif
}

static void rts_off(void)
{
  #ifdef CH1
    UINT psw;

    psw = vdis_psw();

    /* RTS OFF����R�[�h��������    */

    vset_psw(psw);
  #endif
}

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

    ssr = sfr_inw(SCI_SSR) & (SSR_FER|SSR_PER|SSR_BRK);
    if (sfr_in(SCI_SSR2) & (SSR2_ORER))  /* �I�[�o�����G���[���o */
        ssr |= SSR_DR;                   /* SSR_DR��SSR_ORER�����蓖�� */

    /* ��M���̃X�e�[�^�X��OR */

    ssr |= (SIO[CH].rxsts & ~SSR_BRK);

    /* �X�e�[�^�X�r�b�g�ҏW */

    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD �펞ON */

    if (ssr & SSR_TEND)             /* ���M�� */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)              /* �p���e�B�G���[ */
        stat |= TSIO_PE;
    if (ssr & SSR_DR /* as SSR2_ORER */)  /* �I�[�o�����G���[ */
        stat |= TSIO_OE;
    if (ssr & SSR_FER)              /* �t���[�~���O�G���[ */
        stat |= TSIO_FE;
    if (ssr & SSR_BRK)              /* �u���[�N���o */
        stat |= TSIO_BD;
  #ifndef NOFLW
   #ifdef CH0
    if (sfr_inw(IO_PBDR) & PB_CTS1)   /* ~CTS ���� */
        stat &= ~TSIO_CTS;
   #else
        stat &= ~TSIO_CTS;
   #endif
  #endif

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
    UINT psw;
    volatile int i;

    /* RTS OFF */

  #ifndef NOFLW
    rts_off();
  #endif

    /* ���Z�b�g */

    sfr_out(SCI_SCR, 0x00);                 /* Tx/Rx disable */
    sfr_out(SCI_FCR, FCR_RFRST|FCR_TFRST);  /* TxFIFO/RxFIFO reset */

    /* �{�[���[�g�ݒ�l�v�Z */

    cks = 0;
    for (brr = (CLK) / (baud * 32); brr > 0x100L; brr >>= 2)
    {   if (++cks > 3)
            return FALSE;
    }

    /* ���[�h�^�{�[���[�g�ݒ� */

    sfr_out(SCI_SSR2, SSR2_N1);  /* sampling = x16 */
    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));
    for ( i=0; i<1084; i++ ) ;  /* wait 1bit period for 9600bps / I��=60MHz */
                         /* 104[usec] / (1/60[MHz]) / 6[instruction] = 1084 */

    /* �{�[���[�g��1�r�b�g���Ԍo�ߑ҂� */
    tslp_tsk(2);

    /* �t���[���䂠��ꍇ�́A�������݂������� */

  #ifndef NOFLW
   #ifdef CH0
   #else
    if (SIO[CH].flag & TSF_RTS)
    {   psw = vdis_psw();

        /* �K�v�ȏ����������ɋL�q���܂��B */

        vset_psw(psw);
    }
   #endif
  #endif

    /* FIFO �ݒ� */

    sfr_out(SCI_FCR, 0x30);         /* ��M�g���K1, ���M�g���K1 */
    if (SIO[CH].flag & TSF_RTS)
        sfr_set(SCI_FCR, FCR_MCE);  /* MCE = 1: ���f���M�������� */

    /* ���荞�݃R���g���[���̃x�N�^No.��ݒ肷�� */

    psw = vdis_psw();
#ifdef CH0
    sfr_outw(INTC_VCRL, sfr_inw(INTC_VCRL) | (INT_ERI << 8)); /* ERI */
    sfr_outw(INTC_VCRL, sfr_inw(INTC_VCRL) | (INT_RXI << 0)); /* RXI */
    sfr_outw(INTC_VCRM, sfr_inw(INTC_VCRM) | (INT_TXI << 0)); /* TXI */
    sfr_outw(INTC_VCRM, sfr_inw(INTC_VCRM) | (INT_BRI << 8)); /* BRI */
#else
    sfr_outw(INTC_VCRN, sfr_inw(INTC_VCRN) | (INT_ERI << 8)); /* ERI */
    sfr_outw(INTC_VCRN, sfr_inw(INTC_VCRN) | (INT_RXI << 0)); /* RXI */
    sfr_outw(INTC_VCRO, sfr_inw(INTC_VCRO) | (INT_TXI << 0)); /* TXI */
    sfr_outw(INTC_VCRO, sfr_inw(INTC_VCRO) | (INT_BRI << 8)); /* BRI */
#endif
    vset_psw(psw);

    return TRUE;
}

/*****************************************************************************
* CTS �����݋֎~�i�����֐��j
*
******************************************************************************/
#ifndef NOFLW

static void dis_int_cts(void)
{
    UINT psw;

    if (SIO[CH].cmd[1] == 0)
        return;

    psw = vdis_psw();

    /* �b�s�r�����݂��}�X�N����R�[�h���L�q���邱�� */

    SIO[CH].cmd[1] = 0;
    vset_psw(psw);
}

#endif
/*****************************************************************************
* CTS �����݋��i�����֐��j
*
******************************************************************************/
#ifndef NOFLW

static void ena_int_cts(void)
{
    UINT psw;

    if (SIO[CH].cmd[1] != 0)
        return;

    psw = vdis_psw();

    /* �b�s�r�����݃}�X�N����������R�[�h���L�q���邱�� */

    SIO[CH].cmd[1] = 1;
    vset_psw(psw);
}

#endif
/*****************************************************************************
* �V���A�������݋֎~�i�����֐��j
*
******************************************************************************/

static void dis_int_sio(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    sfr_clrw(INTC_IPRD, 0x000f);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRE, 0xf000);
  #endif
    vset_psw(psw);

  #ifndef NOFLW
    dis_int_cts();
  #endif
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
    sfr_setw(INTC_IPRD, IP << 0);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRE, IP << 12);
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

    /* RTS�t���[����̉��� */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        rts_on();
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

    if (SIO[CH].txcnt >= TXBUFSZ)
    {
        return FALSE;
    }

    /* �o�b�t�@�֊i�[ */

    p = SIO[CH].txputp;
    *p = c;

    /* �i�[�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txputp = p;
    SIO[CH].txcnt++;

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

    if ((SIO[CH].rxsts & SSR_BRK) && (SIO[CH].oldsts & SSR_BRK))
        return;

    /* �o�b�t�@���t�`�F�b�N */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
        SIO[CH].rxsts |= SSR_TEND;      /* �I�[�o�t���[�� TEND �Ɋ��蓖�� */

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

    /* RTS�t���[���� */

  #ifndef NOFLW
   #ifdef CH0
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
        sfr_clr(SCI_SCR, SCR_RIE);      /* ��M���荞�݋֎~ */
   #else
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_off();
   #endif
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
    UINT psw;

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

    /* RTS�t���[���� */

  #ifndef NOFLW
   #ifdef CH0
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
    {   psw = vdis_psw();
        sfr_set(SCI_SCR, SCR_RIE);      /* ��M���荞�݋��� */
        vset_psw(psw);
    }
   #else
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
        rts_on();
   #endif
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
L1:
    chr = sfr_in(SCI_RDR);
    sts = sfr_inw(SCI_SSR) & (SSR_FER|SSR_PER|SSR_BRK);

    /* ��M�����݂��N���A */

    sfr_outw(SCI_SSR, (UB)~(SSR_ER|SSR_BRK|SSR_FER|SSR_PER|SSR_RDF|SSR_DR));
    if (sfr_in(SCI_SSR2) & (SSR2_ORER)){  /* �I�[�o�����G���[���o */
        sts |= SSR_DR;                    /* SSR_DR��SSR_ORER�����蓖�� */
        sfr_clr(SCI_SSR2, SSR2_ORER);
    }

    /* �u���[�N���o */

    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_inw(IO_PBDR) & PB_RXD1))  /* RxD �[�q"0" */
        sts |= SSR_BRK;

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

    if (sfr_inw(SCI_SSR) & (SSR_ER|SSR_BRK|SSR_RDF|SSR_DR))
        goto L1;
    if (sfr_in(SCI_SSR2) & SSR2_ORER )
        goto L1;
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
    int cnt;

    /* CTS �����݋֎~ */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        dis_int_cts();
  #endif

    /* ���M�f�B�Z�[�u�����Ȃ瑗�M���Ȃ� */

    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;

    /* RTS�t���[���䂠��Ȃ� CTS �`�F�b�N */
    /* CTS OFF �Ȃ� CTS �����݋��� */

  #ifndef NOFLW
   #ifdef CH0
    if ((SIO[CH].flag & TSF_RTS) && (sfr_inw(IO_PBDR) & PB_CTS1))
   #else
    if ((SIO[CH].flag & TSF_RTS))
   #endif
    {   ena_int_cts();
        goto L2;
    }
  #endif

    /* ���M���f�B�łȂ���ΏI��� */

    if (!(sfr_inw(SCI_SSR) & SSR_TDFE))
        return;
    cnt = 15;

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
        cnt--;
    }

    /* XOFF��M���Ă����瑗�M�s�� */

    if (SIO[CH].flag & TSF_RXOFF)
        goto L2;
  #endif

    /* ���M�o�b�t�@�������� - 1 */
    /* ���M�o�b�t�@��Ȃ瑗�M�s�� */
L1:
    if (--SIO[CH].txcnt == (UH)-1)
    {
        SIO[CH].txcnt = 0;

        if (SIO[CH].tetid != 0)        /* SCIF�ł͂�����       */
            wup_tsk(SIO[CH].tetid);    /* ���M�I���҂�����     */

        /* ���M�����݋֎~ */
L2:
        sfr_clrw(SCI_SSR, SSR_TDFE);
        sfr_clr(SCI_SCR, SCR_TIE);
        return;
    }

    /* �P�������M */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);

    /* �擾�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* ���M�҂����� */

    if ((tid = SIO[CH].txtid) != 0)
        wup_tsk((ID)tid);

    if (--cnt != 0 && (sfr_inw(SCI_SSR) & SSR_TDFE))
        goto L1;
    sfr_clrw(SCI_SSR, SSR_TDFE);
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
#endif
{
    ent_int();              /* �����݃n���h���̊J�n */
    tx_int();               /* ���M�����݃n���h���{�� */
    ret_int();              /* �����݃n���h�����畜�A���� */
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
    if      (strstr((char *)s, (char *)"PE") != NULL)   *mode |= SMR_PE;
    else if (strstr((char *)s, (char *)"PO") != NULL)   *mode |= SMR_PE|SMR_OE;
    if      (strstr((char *)s, (char *)"S2") != NULL)   *mode |= SMR_STOP;
    if      (strstr((char *)s, (char *)"B7") != NULL)   *mode |= SMR_CHR;

    /* �t���[����L������ */

    if (strstr((char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((char *)s, (char *)"RTS") != NULL)   SIO[CH].flag |= TSF_RTS;

    /* �I�[�����w�蔻�� */

    if (strstr((char *)s, (char *)"ETX") != NULL)   SIO[CH].eot = 0x03;
    if (strstr((char *)s, (char *)"EOT") != NULL)   SIO[CH].eot = 0x04;
    if (strstr((char *)s, (char *)"CR" ) != NULL)   SIO[CH].eot = 0x0d;
    if (strstr((char *)s, (char *)"LF" ) != NULL)   SIO[CH].eot = 0x0a;

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
    def_inh(INT_BRI, &dinh_sio_rx);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_inh(INT_IRQ, &dinh_sio_tx);  */ /* */
    }
    #endif
   #endif
  #else
    def_int(INT_ERI, &dint_sio_rx);
    def_int(INT_RXI, &dint_sio_rx);
    def_int(INT_TXI, &dint_sio_tx);
    def_int(INT_BRI, &dint_sio_rx);

   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_int(INT_IRQ, &dint_sio_tx);  */ /* */
    }
    #endif
   #endif
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
#endif
{
    if (!(SIO[CH].flag & TSF_INIT))     /* ���������Ȃ牽�����Ȃ� */
        return;
    dis_int_sio();                      /* �V���A�������݋֎~ */
    sfr_out(SCI_SCR, 0x00);             /* ����M�֎~ */
  #ifndef NOFLW
    rts_off();                          /* RTS OFF */
  #endif
  #ifdef V4
    def_inh(INT_ERI, (T_DINH *)NADR);   /* �����ݒ�`�������� */
    def_inh(INT_RXI, (T_DINH *)NADR);
    def_inh(INT_TXI, (T_DINH *)NADR);
    def_inh(INT_BRI, (T_DINH *)NADR);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_inh(INT_IRQ, (T_DINH *)NADR);  */ /* */
    }
    #endif
   #endif
  #else
    def_int(INT_ERI, (T_DINT *)NADR);   /* �����ݒ�`�������� */
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
    def_int(INT_BRI, (T_DINT *)NADR);
   #ifndef NOFLW
    #ifdef CH1
    if (SIO[CH].flag & TSF_RTS){
/*        def_int(INT_IRQ, (T_DINT *)NADR);  */ /* */
    }
    #endif
   #endif
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

            if (sts & (SSR_TEND|SSR_RDF|SSR_FER|SSR_PER|SSR_BRK))
            {   if (sts & SSR_TEND)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_RDF)
                    ercd = EV_SIOORN;
                else if (sts & SSR_BRK)
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
#endif
{
    UB c;
    UINT psw;

    chg_ims(IP);

    /* �o�b�t�@�N���A */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* �u���[�N���M */

    if (fncd & TSIO_SBON)
    {   fncd |= TSIO_TXD;
        psw = vdis_psw();
#ifdef CH0
        sfr_clrw(PFC_PBCR, PBCR_TXD1_M);  /* to generic */
#else
        sfr_clrw(PFC_PBCR2, PBCR_TXD2_M); /* to generic */
#endif
        sfr_setw(PFC_PBIOR, PB_TXD);    /* at output */
        sfr_clrw(IO_PBDR, PB_TXD);      /* as low */
        vset_psw(psw);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        psw = vdis_psw();
#ifdef CH0
        sfr_setw(PFC_PBCR, PBCR_TXD1);  /* to TxD */
#else
        sfr_setw(PFC_PBCR2, PBCR_TXD2); /* to RxD */
#endif
        vset_psw(psw);
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

    /* RTS �o�� */

  #ifndef NOFLW
    if (fncd & (TSIO_RTSON|TSIO_RTSOFF))
    {   if (fncd & TSIO_RTSON)      rts_on();
        if (fncd & TSIO_RTSOFF)     rts_off();
    }
  #endif

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
#endif
{
    ER ercd;

    for (;;)
    {
        chg_ims(IP);

        if (SIO[CH].txcnt == 0)
        {
            chg_ims(0);
            return E_OK;            /* ����I�� */
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
