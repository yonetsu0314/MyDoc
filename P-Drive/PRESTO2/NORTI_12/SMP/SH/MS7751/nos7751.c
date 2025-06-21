/****************************************************************************
* �V���A�����o�̓h���C�o�iSH7751�j                                          *
*                                                                           *
*  File name : nos7751.c                                                    *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile :                                                                *
*      shc -cp=sh4 -op=1 -def=CLK=?,CH=?,CH1,V4 nos7751.c       (Renesas C) *
*      gcc -c -m4 -O2 -DCLK=? -DCH=? -DCH1 -DV4 nos7751.c          (exeGCC) *
*      ccsh -c -cpu=s4 -ansi -O -DCLK=? -DCH=? -DCH1 -DV4 nos7751.c   (GHS) *
*                                                                           *
* 19/Apr/2000                                                               *
* 06/Mar/2002     - Modified for RTS/CTS control           [SP]             *
* 18/Dec/2002      chg_ims() updated                                  [SP]  *
* 27/Dec/2002 V4�}�N���̕s�v���A esle �� V4 �ƌ��Ȃ��悤���������ւ�   KA *
* 25/Feb/2003 GAIO, IAR EW/ICC ��`�̒ǉ�                                KA *
* 25/Feb/2003 char/unsigned char �̃L���X�g��ǉ�(���[�j���O�̍폜)      KA *
* 07/May/2003 #pragma interrupt �̑O�� HITACHI -> __HITACHI__            MI *
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
          CH0 ���`����ƁASCI ���g���悤�ɃR���p�C������܂��B
          CH1 ���`����ƁAFIFO �t�� SCIF ���g���悤�ɃR���p�C������܂��B

          �`���l��0 �ƃ`���l��1 �̗����������N����ꍇ�́A�d�����Ȃ����W���[
          �����ƂȂ�悤�R���p�C�����Ă��������B

          (��) shc <options> nos7751.c  ����������������������� �`���l��0
               copy nos7751.c nos7ch1.c
               shc <options> -def=CH=1 nos7ch1.c  ������������� �`���l��1

          (��) gcc <options> nos7751.c  ����������������������� �`���l��0
               gcc <options> -DCH=1 -onos7ch1.o nos7751.c ����� �`���l��1

          (��) ccsh <options> nos7751.c  ���������������������� �`���l��0
               ccsh <options> -DCH=1 -o nos7ch1.o nos7751.c ��� �`���l��1

�{�[���[�g�W�F�l���[�^

          �\�[�X�N���b�N�̃f�t�H���g�� 33.3333 MHz �ł��B�ύX����ꍇ ��
          CLK �}�N�����`���ăR���p�C�����Ă��������B
          �l�� [Hz] �P�ʂ̎��g���ł��B

          (��) shc <options> -def=CLK=27000000 nos7751.c ������ 30 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=27000000 nos7751.c
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

�n�[�h�E�F�A�t���[����iCH0 �� CH1�j

          ���{�o�[�W�����ł�CH0�̃t���[����͖��m�F�ł��B

�n�[�h�E�F�A�t���[����iCH1�j

          SCIF ������ RTS-CTS �@�\���g���A�\�t�g�E�F�A�Ɉ˂炸�s���܂��B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A8 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=5 nos7751.c ��� �����ݗD�揇�� 5
                                  ~~~~
          (��) gcc <options> -DIP=5 nos7751.c
                               ~~~~
               |       �W �� �� �t
          -----+---------------------------
               |RTS --- Not Supported
               |CTS --- Not Supported
          CH0  |DTR --- Not Supported
               |DSR --- Not Supported
               |DCD --- Not Supported
          -----+---------------------------
               |RTS   RTS2/SCPT2(Hard)
               |CTS   CTS2/IRQ5/PINT7
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

���T�|�[�g�@�\

          �������������[�h�݂̂ŁA�N���b�N���������[�h�̓T�|�[�g���Ă��܂���B
          �����{�[���[�g�W�F�l���[�^�݂̂ŁA�O���N���b�N���o�͂̓T�|�[�g����
          ���܂���B
          SH7751 ���L�̃}���`�v���Z�b�T���[�h�̓T�|�[�g���Ă��܂���B
*/

#include <stdlib.h>
#include <string.h>

#ifndef SH4
 #define SH4
#endif

#ifdef V2
 #include "norti.h"
 #define PSW         UW
 #define tslp_tsk(p) wai_tsk(p)
#elif defined(V3)
 #include "norti3.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #ifndef iwup_tsk
  #define iwup_tsk(i) wup_tsk(i)
 #endif
 #define PSW         UINT
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #define PSW         UINT
#endif

#include "nosio.h"
#include "sh7751.h"

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
 #define IP          8           /* IP ����`�Ȃ� 8 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
 #define CLK        (27*MHz)  /* ���Ӄ��W���[���p�N���b�N[Hz] */
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

#include "nos7751.h"

/* CTS ���� IRQ �ԍ� */

#ifndef IRQ

  /* CTS�����݂�����΁AIRQ���`���邱��   */

#endif

#ifndef NOFLW

  /* CTS�����݂�����΁AINT_IRQ���`���邱��   */

#endif

/* CHx �ɍ��킹���Ē�` */

#ifdef CH0
 #define SCI_SMR     SCI_SCSMR1
 #define SCI_BRR     SCI_SCBRR1
 #define SCI_SCR     SCI_SCSCR1
 #define SCI_TDR     SCI_SCTDR1
 #define SCI_SSR     SCI_SCSSR1
 #define SCI_RDR     SCI_SCRDR1
 #define SCI_SCSPTR  SCI_SCSPTR1    /* 8bit */
 #define SCS_SPBIO   SPB1IO
 #define SCS_SPBDT   SPB1DT
#endif

#ifdef CH1
 #define SCI_SMR     SCIF_SCSMR2        /* 16bit */
 #define SCI_BRR     SCIF_SCBRR2        /*  8bit */
 #define SCI_SCR     SCIF_SCSCR2        /* 16bit */
 #define SCI_TDR     SCIF_SCFTDR2       /*  8bit */
 #define SCI_SSR     SCIF_SCFSR2        /* 16bit */
 #define SCI_RDR     SCIF_SCFRDR2       /*  8bit */
 #define SCIF_SCFRDR SCIF_SCFRDR2       /*  8bit */
 #define SCIF_SCFCR  SCIF_SCFCR2        /* 16bit */
 #define SCIF_SCFDR  SCIF_SCFDR2        /* 16bit */
 #define SCI_SCSPTR  SCIF_SCSPTR2       /* 16bit */
 #undef  INT_ERI
 #undef  INT_RXI
 #undef  INT_TXI
 #undef  INT_TEI
 #define INT_ERI     INT_FERI
 #define INT_RXI     INT_FRXI
 #define INT_TXI     INT_FTXI
 #define INT_BRI     INT_FBRI
 #define SCS_SPBIO   SPB2IO
 #define SCS_SPBDT   SPB2DT
 #undef  RTSOFF_LEN
 #undef  RTSON_LEN
 #define RTSON_LEN   (BUFSZ-48)         /* RTS�M��ON  �o�b�t�@�� */
 #define RTSOFF_LEN  (BUFSZ-32)         /* RTS�M��OFF �o�b�t�@�� */
#endif

/* �����݃n���h���̍Ē�` */

#ifdef CH
 #if (CH==0)
  #define int_sio_rx  int_sio0_rx
 #elif (CH==1)
  #define int_sio_rx  int_sio1_rx
 #elif (CH==2)
  #define int_sio_rx  int_sio2_rx
 #elif (CH==3)
  #define int_sio_rx  int_sio3_rx
 #endif

 #if (CH==0)
  #define int_sio_tx  int_sio0_tx
 #elif (CH==1)
  #define int_sio_tx  int_sio1_tx
 #elif (CH==2)
  #define int_sio_tx  int_sio2_tx
 #elif (CH==3)
  #define int_sio_tx  int_sio3_tx
 #endif

 #if (CH==0)
  #define int_sio_te  int_sio0_te
 #elif (CH==1)
  #define int_sio_te  int_sio1_te
 #elif (CH==2)
  #define int_sio_te  int_sio2_te
 #elif (CH==3)
  #define int_sio_te  int_sio3_te
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
 #endif
#endif

/* �V���A�����o�͐���u���b�N */

extern T_SIO SIO[];

/* �����ϐ� */

static UB txbuf[TXBUFSZ];       /* ���M�o�b�t�@ */
static UB rxbuf[BUFSZ*2];       /* ��M�o�b�t�@, ��M�X�e�[�^�X�o�b�t�@ */

#undef RTSFLG
#if (defined(INT_IRQ))
 #define RTSFLG
#endif

/* #ifdef RTSFLG */
/*****************************************************************************
* RTS �M�� ON/OFF�i�����֐��j
*
******************************************************************************/

static void rts_on(void)
{
#ifdef CH1
    PSW psw;

    psw = vdis_psw();
    sfr_clrw(SCI_SCSPTR, RTSDT);    /* Turn ON RTS signal */
    vset_psw(psw);
#endif
}

static void rts_off(void)
{
#ifdef CH1
    PSW psw;

    psw = vdis_psw();
    sfr_setw(SCI_SCSPTR, RTSDT);    /* Turn OFF RTS signal */
    vset_psw(psw);
#endif
}

/* #endif */

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
*
* DSR, CD �� �����̂ŏ펞 1 �Ƃ���
******************************************************************************/

static int get_stat(void)
{
    int stat;
    int ssr;

    /* �X�e�[�^�X���� */

#ifdef CH0
    ssr = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);
    ssr |= SIO[CH].rxsts;               /* �O���M���̃X�e�[�^�X��OR     */
    if (sfr_in(SCI_SCSPTR) & SCS_SPBDT) /* �u���[�NOFF���o, RxD �[�q"1" ? */
        ssr &= ~SSR_MPBT;               /* �u���[�N���o�� MPBT �Ɋ��蓖�� */
#else                                   /* SCIF��SSR��16Bit               */
    ssr = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER);
    ssr |= (SIO[CH].rxsts & ~SSR_BRK);  /* ��M���̃X�e�[�^�X��OR */
#endif

    /* �X�e�[�^�X�r�b�g�ҏW */
    stat = TSIO_DSR|TSIO_CD|TSIO_CTS;   /* DSR, CD �펞ON */
    if (ssr & SSR_TEND)                 /* ���M�� */
        stat |= TSIO_TXEMP;
    if (ssr & SSR_PER)                  /* �p���e�B�G���[ */
        stat |= TSIO_PE;
#ifdef CH0
    if (ssr & SSR_ORER)                 /* �I�[�o�����G���[ */
        stat |= TSIO_OE;
#endif
    if (ssr & SSR_FER)                  /* �t���[�~���O�G���[ */
        stat |= TSIO_FE;
#ifdef CH0
    if (ssr & SSR_MPBT)                 /* �u���[�N���o */
#else
    if (ssr & SSR_BRK)
#endif
        stat |= TSIO_BD;

#ifndef NOFLOW
#ifdef CH0
    stat &= ~TSIO_CTS;
#else
    if (sfr_inw(SCI_SCSPTR) & CTSDT)    /* /CTS ���� */
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
    PSW psw;

    /* RTS OFF */
#ifdef CH0
    /* ���Z�b�g */
    sfr_out(SCI_SCR, 0x00);
#else
    /* ���Z�b�g */
    sfr_outw(SCI_SCR, 0x00);

    /* SCIF �ł� FIFO �ݒ� */
    sfr_outw(SCIF_SCFCR, 0x30);         /* ��M�g���K1, ���M�g���K1 */
    if (SIO[CH].flag & TSF_RTS) {
        sfr_clrw(SCIF_SCFCR, MCR_EN);     /* MCE = 1: ���f���M�������� */
        sfr_setw(SCI_SCSPTR, RTSIO);
        sfr_clrw(SCI_SCSPTR, CTSIO);
        rts_on();
    }

#endif

    /* �{�[���[�g�ݒ�l�v�Z */

    baud <<= 4;                         /* 64�~2^(-1)�~B �̔��� */
    for (cks = 0; cks < 4; cks++, baud <<= 2)
    {   brr = ((CLK) / baud + 1) / 2;   /* �l�̌ܓ� */
        if (brr <= 0xffL)
            break;
    }
    if (cks == 4)
        return FALSE;

    /* ���[�h�^�{�[���[�g�ݒ� */
#ifdef  CH0
    sfr_out(SCI_SMR, (UB)(mode | cks));
#else
    sfr_outw(SCI_SMR, (mode | cks));
#endif
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* �{�[���[�g��1�r�b�g���Ԍo�ߑ҂� */
    tslp_tsk(2);

    return TRUE;
}


#ifdef INT_IRQ
/*****************************************************************************
* CTS �����݋֎~�i�����֐��j
*
******************************************************************************/

static void dis_int_cts(void)
{
    PSW psw;

    if (SIO[CH].cmd[1] == 0)
        return;

    psw = vdis_psw();

    /* �b�s�r�����݂��}�X�N����R�[�h���L�q���邱�� */

    SIO[CH].cmd[1] = 0;
    vset_psw(psw);
}

#endif
#ifdef INT_IRQ
/*****************************************************************************
* CTS �����݋��i�����֐��j
*
******************************************************************************/

static void ena_int_cts(void)
{
    PSW psw;

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
    PSW psw;

    psw = vdis_psw();
#ifdef CH0
    sfr_clrw(INTC_IPRB, 0x00f0);
#else
    sfr_clrw(INTC_IPRC, 0x00f0);
#endif
    vset_psw(psw);

#ifndef NOFLOW
#ifdef INT_IRQ
    dis_int_cts();
#endif
#endif
}

/*****************************************************************************
* �V���A�������݋��i�����֐��j
*
******************************************************************************/

static void ena_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
#ifdef CH0
    sfr_setw(INTC_IPRB, IP << 4);
#else
    sfr_setw(INTC_IPRC, IP << 4);
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

#ifndef NOXON
    /* XON/OFF�t���[����̉��� */

    if (SIO[CH].flag & TSF_TXOFF)
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF ���M�v���Z�b�g */
#ifdef CH0
        sfr_set(SCI_SCR, SCR_TIE);  /* ���M�����݋��� */
#else
        sfr_setw(SCI_SCR, SCR_TIE); /* ���M�����݋��� */
#endif
    }
#endif

#ifndef NOFLOW
    /* RTS�t���[����̉��� */

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
#ifdef  CH0
    sfr_set(SCI_SCR, SCR_TIE);
#else
    sfr_setw(SCI_SCR, SCR_TIE);
#endif
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

#ifdef CH0
    if ((SIO[CH].rxsts & SSR_MPBT) && (SIO[CH].oldsts & SSR_MPBT))
#else
    if ((SIO[CH].rxsts & SSR_BRK) && (SIO[CH].oldsts & SSR_BRK))
#endif
        return;

    /* �o�b�t�@���t�`�F�b�N */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (++cnt == BUFSZ)
#ifdef CH0
        SIO[CH].rxsts |= SSR_RDRF;      /* �I�[�o�t���[�� RDRF �Ɋ��蓖�� */
#else
        SIO[CH].rxsts |= SSR_TEND;      /* �I�[�o�t���[�� TEND �Ɋ��蓖�� */
#endif

    /* �o�b�t�@�������� + 1 */

    SIO[CH].rxcnt = (UH)cnt;

    /* XON/OFF�t���[���� */

#ifndef NOXON
    if ((cnt == XOFF_LEN)
     && ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON))
    {   SIO[CH].flag |= TSF_TXREQ;      /* XON/XOFF ���M�v���Z�b�g */
#ifdef CH0
        sfr_set(SCI_SCR, SCR_TIE);      /* ���M�����݋��� */
#else
        sfr_setw(SCI_SCR, SCR_TIE);     /* ���M�����݋��� */
#endif

    }
#endif

#ifndef NOFLOW
    /* RTS�t���[���� */

    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_off();
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
    PSW psw;

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

#ifndef NOFLOW
    /* RTS�t���[���� */

    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_on();
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
#ifndef CH0
    int sts2;
#endif

    /* ��M�X�e�[�^�X�Ǝ�M��������� */
L1:
    chr = sfr_in(SCI_RDR);
#ifdef CH0
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);
    /* ��M�����݂��N���A */
    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));
#else
    sts = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER);
    /* ��M�����݂��N���A */
    sfr_outw(SCI_SSR, (UB)~(SSR_ER|SSR_BRK|SSR_FER|SSR_PER|SSR_RDF|SSR_DR));
    sts2 = sfr_inw(SCIF_SCLSR2) & 0x01; /* Get Overrun              */
    sfr_outw(SCIF_SCLSR2,0);            /* Clear Overrun            */
    if ( sts2 ){
        sts |= SSR_DR;
    }
#endif

    /* �u���[�N���o */

#ifdef CH0
    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_in(SCI_SCSPTR) & SCS_SPBDT))  /* RxD �[�q"0" */
       sts |= SSR_MPBT;                 /* �u���[�N���o�� MPBT �Ɋ��蓖�� */
#else
    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_inw(SCI_SCSPTR) & SCS_SPBDT)) /* RxD �[�q"0" */
       sts |= SSR_BRK;
#endif

    SIO[CH].oldsts = SIO[CH].rxsts;     /* �O��̎�M�X�e�[�^�X�L�� */
    SIO[CH].rxsts = (UB)sts;
    SIO[CH].rxchr = (UB)chr;

#ifndef NOXON
    /* XON/OFF�t���[����L��̏ꍇ */
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
 #ifdef CH0
            sfr_set(SCI_SCR, SCR_TIE);      /* ���M�����݋��� */
 #else
            sfr_setw(SCI_SCR, SCR_TIE);
 #endif
            return;
        }
    }
#endif

    /* ��M�o�b�t�@�֊i�[ */

    put_rxbuf();

    /* ��M�҂����� */

    if ((tid = SIO[CH].rxtid) != 0)
    {   SIO[CH].rxtid = 0;
        iwup_tsk((ID)tid);
    }

#ifdef CH0
    if (sfr_in(SCI_SSR) & (SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT))
        goto L1;
#else
    if (sfr_inw(SCI_SSR) & (SSR_ER|SSR_BRK|SSR_FER|SSR_PER|SSR_RDF|SSR_DR))
        goto L1;
    if ( sfr_inw(SCIF_SCLSR2) & 0x01 )
        goto L1;
#endif
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
#ifndef CH0
    int cnt;
#endif

#ifndef NOFLOW
    /* CTS �����݋֎~ */
    if (SIO[CH].flag & TSF_RTS) {
#ifdef CH1
        if (sfr_inw(SCI_SCSPTR) & CTSDT) {
            goto L2;
            return;
        }
#endif
    }

#ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        dis_int_cts();
#endif
#endif

    /* ���M�f�B�Z�[�u�����Ȃ瑗�M���Ȃ� */
#ifdef CH0
    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;
#else
    if (!(sfr_inw(SCI_SCR) & SCR_TE))
        goto L2;
#endif

#ifndef NOFLOW
    /* RTS�t���[���䂠��Ȃ� CTS �`�F�b�N */
    /* CTS OFF �Ȃ� CTS �����݋��� */

#ifdef INT_IRQ
    if ((SIO[CH].flag & TSF_RTS) && (sfr_in(SCI_SCPTR) & PH_CTS))
    {   ena_int_cts();
        goto L2;
    }
#endif
#endif

    /* ���M���f�B�łȂ���ΏI��� */

#ifdef CH0
    if (!(sfr_in(SCI_SSR) & SSR_TDRE))
        return;
#else
    if (!(sfr_inw(SCI_SSR) & SSR_TDFE))
        return;
    cnt = 15;
#endif


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
      #ifdef CH0
        sfr_clr(SCI_SSR, SSR_TDRE);
        return;
      #else
        cnt--;
      #endif
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
#ifdef CH0
        sfr_set(SCI_SCR, SCR_TEIE);     /* ���M�I�������݋���   */
#else
        if (SIO[CH].tetid != 0)         /* SCIF�ł͂�����       */
            iwup_tsk(SIO[CH].tetid);    /* ���M�I���҂�����     */
#endif
L2:
#ifdef  CH0
        sfr_clr(SCI_SCR, SCR_TIE);      /* ���M�����݋֎~       */
#else
        sfr_clrw(SCI_SSR, SSR_TDFE);
        sfr_clrw(SCI_SCR, SCR_TIE);     /* ���M�����݋֎~       */
#endif
        return;
    }

    /* �P�������M */

    p = SIO[CH].txgetp;
    sfr_out(SCI_TDR, *p);
#ifdef CH0
    sfr_clr(SCI_SSR, SSR_TDRE);
#endif

    /* �擾�|�C���^���P�i�߂� */

    if (++p >= SIO[CH].txbuf + TXBUFSZ)
        p = SIO[CH].txbuf;
    SIO[CH].txgetp = p;

    /* ���M�҂����� */

    if ((tid = SIO[CH].txtid) != 0)
    {   SIO[CH].txtid = 0;
        iwup_tsk((ID)tid);
    }

#ifndef CH0
    if (--cnt != 0)
        goto L1;
    sfr_clrw(SCI_SSR, SSR_TDFE);
#endif
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
    ent_int();             /* �����݃n���h���̊J�n */
    tx_int();               /* ���M�����݃n���h���{�� */
    ret_int();              /* �����݃n���h�����畜�A���� */
}

#ifdef CH0
/*****************************************************************************
* �����݃n���h�����M�I�������i�����֐��j
*
******************************************************************************/

static void te_int(void)
{
    /* ���M�I���҂����� */

    if (SIO[CH].tetid != 0)
        iwup_tsk(SIO[CH].tetid);

    /* ���M�I�������݋֎~ */

    sfr_clr(SCI_SSR, SSR_TEND);
    sfr_clr(SCI_SCR, SCR_TEIE);
}

#endif
#ifdef CH0
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
 #endif
{
    ent_int();                  /* �����݃n���h���̊J�n */
    te_int();                   /* ���M�I������ */
    ret_int();                  /* �����݃n���h�����畜�A���� */
}

#endif
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

#ifdef V4
static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio_rx, IP };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio_tx, IP };
 #ifdef CH0
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio_te, IP };
 #endif
#else
#ifdef V3
static const T_DINT dint_sio_rx = { TA_HLNG, int_sio_rx, IP };
static const T_DINT dint_sio_tx = { TA_HLNG, int_sio_tx, IP };
 #ifdef CH0
static const T_DINT dint_sio_te = { TA_HLNG, int_sio_te, IP };
 #endif
#endif
#endif

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
#ifdef CH0
    sfr_clr(SCI_SCR, SCR_RIE );
#else
    sfr_clrw(SCI_SCR, SCR_RIE );
#endif
    if (!(SIO[CH].flag & TSF_INIT))
    {
        SIO[CH].flag |= TSF_INIT;
#ifdef V4
        def_inh(INT_ERI, &dinh_sio_rx);
        def_inh(INT_RXI, &dinh_sio_rx);
        def_inh(INT_TXI, &dinh_sio_tx);
 #ifdef CH0
        def_inh(INT_TEI, &dinh_sio_te);
 #else
        def_inh(INT_BRI, &dinh_sio_rx);
 #endif
 #ifdef INT_IRQ    /* RTS Interrupt */
        if (SIO[CH].flag & TSF_RTS)
            def_inh(INT_IRQ, &dinh_sio_tx);
 #endif
#else
#ifdef V3
        def_int(INT_ERI, &dint_sio_rx);
        def_int(INT_RXI, &dint_sio_rx);
        def_int(INT_TXI, &dint_sio_tx);
 #ifdef CH0
        def_int(INT_TEI, &dint_sio_te);
 #else
        def_int(INT_BRI, &dint_sio_rx);
 #endif
 #ifdef INT_IRQ    /* RTS Interrupt */
        if (SIO[CH].flag & TSF_RTS)
            def_int(INT_IRQ, &dint_sio_tx);
 #endif
#else
        def_int(INT_ERI, int_sio_rx);
        def_int(INT_RXI, int_sio_rx);
        def_int(INT_TXI, int_sio_tx);
 #ifdef CH0
        def_int(INT_TEI, int_sio_te);
 #else
        def_int(INT_BRI, int_sio_rx);
 #endif
 #ifdef INT_IRQ    /* RTS Interrupt */
        if (SIO[CH].flag & TSF_RTS)
            def_int(INT_IRQ, int_sio_tx);
 #endif
#endif
#endif
    }

    /* ��M�����݋��� */
#ifdef CH0
    sfr_set(SCI_SCR, SCR_RIE );
#else
    sfr_setw(SCI_SCR, SCR_RIE );
#endif
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
#ifdef CH0
    sfr_out(SCI_SCR, 0x00);             /* ����M�֎~ */
#else
    sfr_outw(SCI_SCR, 0x00);            /* ����M�֎~ */
#endif

#if (defined(INT_IRQ))
    rts_off();                          /* RTS OFF */
#endif

    /* �����ݒ�`�������� */

#ifdef V4 /* NORTI4 */
    def_inh(INT_ERI, (T_DINH *)NADR);
    def_inh(INT_RXI, (T_DINH *)NADR);
    def_inh(INT_TXI, (T_DINH *)NADR);
 #ifdef CH0
    def_inh(INT_TEI, (T_DINH *)NADR);
 #else
    def_inh(INT_BRI, (T_DINH *)NADR);
 #endif
 #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, (T_DINH *)NADR);
 #endif

#else
#ifdef V3 /* NORTI3 */
    def_int(INT_ERI, (T_DINT *)NADR);
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
 #ifdef CH0
    def_int(INT_TEI, (T_DINT *)NADR);
 #else
    def_int(INT_BRI, (T_DINT *)NADR);
 #endif
 #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (T_DINT *)NADR);
 #endif

#else /* NORTi2 */
    def_int(INT_ERI, (INTHDRP)NADR);
    def_int(INT_RXI, (INTHDRP)NADR);
    def_int(INT_TXI, (INTHDRP)NADR);
 #ifdef CH0
    def_int(INT_TEI, (INTHDRP)NADR);
 #else
    def_int(INT_BRI, (INTHDRP)NADR);
 #endif
 #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (INTHDRP)NADR);
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
    UINT imask;

    for (;;)
    {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);

        /* ��M�o�b�t�@����P�������� */

        sts = get_rxbuf(c);

        if (sts != -1)              /* ��M�����������ꍇ */
        {   chg_ims(imask);
            /* ��M�G���[���� */
#ifdef CH0
            if (sts & (SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT))
            {   if (sts & SSR_RDRF)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_ORER)
                    ercd = EV_SIOORN;
                else if (sts & SSR_MPBT)
                    ercd = EV_SIOBRK;
#else
            if (sts & (SSR_TEND|SSR_RDF|SSR_FER|SSR_PER|SSR_BRK))
            {   if (sts & SSR_TEND)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_RDF)
                    ercd = EV_SIOORN;
                else if (sts & SSR_BRK)
                    ercd = EV_SIOBRK;
#endif
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
        chg_ims(imask);
        ercd = wai_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
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
    UINT imask;

    for (;;) {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);
        /* ���M�o�b�t�@�ւP�����i�[ */
        if (put_txbuf(c))           /* �i�[�ł����ꍇ */
        {
            chg_ims(imask);
            return E_OK;            /* ����I�� */
        }

        /* ���M�����ݑ҂� */

        SIO[CH].txtid = vget_tid();
        chg_ims(imask);
        ercd = wai_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
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
    PSW psw;
    UINT imask;

#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
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
        sfr_clr(SCI_SCSPTR, SCS_SPBDT);
        sfr_set(SCI_SCSPTR, SCS_SPBIO);
#else
        sfr_clrw(SCI_SCSPTR, SCS_SPBDT);
        sfr_setw(SCI_SCSPTR, SCS_SPBIO);
#endif
        vset_psw(psw);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        psw = vdis_psw();
#ifdef CH0
        sfr_set(SCI_SCSPTR, SCS_SPBDT);
        sfr_clr(SCI_SCSPTR, SCS_SPBIO);
#else
        sfr_setw(SCI_SCSPTR, SCS_SPBDT);
        sfr_clrw(SCI_SCSPTR, SCS_SPBIO);
#endif
        vset_psw(psw);
    }

    /* �V���A�����[�h�ύX */

    if (fncd & (TSIO_RXE|TSIO_RXD|TSIO_TXE|TSIO_TXD))
    {
#ifdef CH0
        c = sfr_in(SCI_SCR);
#else
        c = (UB)sfr_inw(SCI_SCR);
#endif
        if (fncd & TSIO_RXE)        c |=  SCR_RE;
        if (fncd & TSIO_RXD)        c &= ~SCR_RE;
        if (fncd & TSIO_TXE)        c |=  SCR_TE;
        if (fncd & TSIO_TXD)        c &= ~SCR_TE;
#ifdef CH0
        sfr_out(SCI_SCR, c);
#else
        sfr_outw(SCI_SCR, c);
#endif
    }

    /* RTS �o�� */

#if (defined(INT_IRQ))
    if (fncd & (TSIO_RTSON|TSIO_RTSOFF))
    {   if (fncd & TSIO_RTSON)      rts_on();
        if (fncd & TSIO_RTSOFF)     rts_off();
    }
#endif
    chg_ims(imask);
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
    UINT imask;

#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    chg_ims(IP);

    stat = get_stat();
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = (UB)stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(imask);
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
    UINT imask;

    for (;;) {
#ifdef V4
        get_ims(&imask);
#else
        ref_ims(&imask);
#endif
        chg_ims(IP);

        if (SIO[CH].txcnt == 0)
        {
#ifndef CH0
            chg_ims(imask);
            return E_OK;                /* ����I�� */
#else
            if (sfr_in(SCI_SSR) & SSR_TEND)
            {   chg_ims(imask);
                return E_OK;            /* ����I�� */
            }
#endif
        }

        /* ���M�I�������ݑ҂� */

        SIO[CH].tetid = vget_tid();
        chg_ims(imask);
        ercd = wai_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
        if (ercd)
            return ercd;                /* �^�C���A�E�g�I�� */
    }
}

/* end */
