/****************************************************************************
* �V���A�����o�̓h���C�o (SH7727����SCI/SCIF)                               *
*                                                                           *
*  File name : nos7727.c                                                    *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
* 07/Sep/1998 �쐬                                                       MI *
* 22/Apr/2000 SH7709A �p�Ƃ���                                              *
* 28/Sep/2000 CH0 �I�����̕s��C��                                        *
* 29/Sep/2000 CH0��SSR_RDF����`�G���[�ƂȂ�s����C��                 MI *
* 31/Oct/2000 IAR EW �Ή�                                                TK *
* 13/Jul/2001 CH1,CH2��SSR�A�N�Z�X���o�C�g���烏�[�h�ɏC��               MI *
* 12/Sep/2001 ����n�̔��f��������                                       TA *
* 25/Sep/2001 CH2��SCFCR MCE�r�b�g���C��, 0x80��0x08                   MI *
* 25/Sep/2001 CH1,CH2�̎�M�����݂�FER��PER�����Ă����s��C����        MI *
* 27/Dec/2002 V4�}�N���̕s�v���A esle �� V4 �ƌ��Ȃ��悤���������ւ�   KA *
* 23/Feb/2003 SH7727�p�Ƃ���(CH1�폜��)                                  MI *
* 23/Feb/2003 SCIF2�̊����݂ɂ��n�[�h�E�F�A�t���[���������            MI *
* 23/Feb/2003 NORTi2�Ή�(V2�}�N��)���폜                                 MI *
* 23/Feb/2003 SCIF��ref_sio��TEND�҂����������̂���rot_rdq�ōs��         MI *
* 31/Jul/2003 BRR�ݒ�� �E�F�C�g�}��                                     KA *
****************************************************************************/

/*

�œK���R���p�C��

          �����݃n���h�����܂܂��̂ŁA�K���œK���I�v�V������t���ăR���p�C
          �����Ă��������B

�`���l���ԍ��̎w��

          CH�}�N���ŁA�_���`���l���ԍ����w��ł��܂��B(���w�莞�́ACH=0)
          CH�́A0�`3���w��ł��܂��B

          CH0�ACH2�}�N���ŁA�����`���l���ԍ���ύX�ł��܂��B
          CH0���`����ƁASCI���g���悤�ɃR���p�C������܂��B
          CH2���`����ƁAFIFO�t����SCIF2���g���悤�ɃR���p�C������܂��B
          CH1�A���Ȃ킿�A�V���A��IO(SIOF)�ɂ͑Ή����Ă��܂���B

          �����̃`���l�����g�p����ꍇ�́A�d�����Ȃ����W���[�����ƂȂ�悤�R
          ���p�C�����Ă��������B

          (��) shc <options> nos7727.c  ------------------------ �`���l��0
               copy nos7727.c nos7ch1.c
               shc <options> -def=CH=1 nos7ch1.c  -------------- �`���l��1

�{�[���[�g�W�F�l���[�^

          �\�[�X�N���b�N�̃f�t�H���g��24MHz �ł��B�ύX����ꍇ�́ACLK�}�N
          �����`���ăR���p�C�����Ă��������B�l�́A[Hz]�P�ʂ̎��g���ł��B

          (��) shc <options> -def=CLK=25000000 nos7727.c  ------ 25 MHz
                                  ~~~~~~~~~~~~
����M�o�b�t�@�T�C�Y�̎w��

          ��M�^���M�o�b�t�@�̃T�C�Y���ABUFSZ�}�N���ɂ���`���ăR���p�C��
          ���Ă��������B��M�o�b�t�@�ƈقȂ�T�C�Y�̑��M�o�b�t�@��p����ꍇ
          �́A�����TXBUFSZ�}�N�����`���Ă��������B
          ���w�莞�́A��M�^���M�o�b�t�@��1024�o�C�g�ƂȂ�܂��B
          ��M�o�b�t�@�T�C�Y��4�o�C�g�ȏ�A�Ȃ�ׂ��傫�ȃT�C�Y�Ƃ��Ă�����
          ���B

          (��) ��M�o�b�t�@, ���M�o�b�t�@�� 128 �o�C�g BUFSZ=128
          (��) ��M�o�b�t�@128, ���M�o�b�t�@ 64 �o�C�g BUFSZ=128, TXBUFSZ=64

�\�t�g�E�F�A�t���[����

          XON/XOFF�L�����N�^�ɂ��t���[�����p���Ȃ��ꍇ�ANOXON�}�N�����
          �`���ăR���p�C�����Ă��������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B

�n�[�h�E�F�A�t���[����(CH0:SCI)

          �`���l��0 /RTS�M�����ASCK0/SCPT[1] �[�q�ɐڑ����Ă��������B
          �`���l��0 /CTS�M�����A/IRQn�[�q�ɐڑ����Ă��������B
          IRQn�́AIRQ�}�N���̒�`�őI���ł��܂�(0�`4, �f�t�H���g��0)�B

          IRQ3�`IRQ0���AIRL3�`IRL0�Ƃ��ăG���R�[�h����15���x���̊��荞�ݒ[�q��
          ���Ďg�p����ꍇ�̂��Ƃ͍l�����Ă��܂���B
          ����ȊO�̐ڑ��̏ꍇ�́A�{�\�[�X�t�@�C�����C�����Ă��������B
          �������A/CTS�M���̓��[���x�������݂Ō��o�ł���K�v������܂��B

�n�[�h�E�F�A�t���[����(CH2:SCIF2)

          SCIF2�ł́A�����̃��f���R���g���[���@�\(RTS/CTS�̎�������@�\)�𗘗p
          ���邱�Ƃ��ł��܂��B
          �������A���肪�n�[�h�E�F�A�̑��M��~�@�\�Ɉ˂炸�Ƀt���[������s����
          ����ꍇ�A��MFIFO���t�ł�RTS�M��OFF�ł́A����̑��M��~���Ԃɍ����
          �����Ƃ��S�z����܂��B

          �������f���R���g���[���@�\���g��Ȃ��ASCI�Ɠ��l�̊����݂��g�����n�[
          �h�E�F�A�t���[������s�����߂ɂ́ANOMCE�}�N�����`���ăR���p�C����
          �Ă��������B
          ���̏ꍇ�A�\�t�g�E�F�A�Ő���ł��Ȃ�RTS2/USB1d_TXENL�[�q�łȂ�����
          �|�[�g��RTS�M����ڑ�����K�v������A�{�h���C�o�ł́ASIOFSYNC/SCPT[6]
          �[�q��z�肵�Ă��܂��B����ȊO�̐ڑ��̏ꍇ�́A�{�\�[�X�t�@�C�����C��
          ���Ă��������B

          ������̃`���l�����A�n�[�h�E�F�A�t���[�����p���Ȃ��ꍇ�́ANOFLW�}
          �N�����`���ăR���p�C�����Ă��������B�R�[�h�T�C�Y�Ə������x��ߖ��
          ���܂��B�����������ȒʐM�ł́A��M�o�b�t�@�I�[�o�[�t���[�̊댯������
          �܂��B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7�ł��B
          �����ύX����ꍇ�AIP�}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B (IP �� KNL_LEVEL)
          �{�n���h���ƈقȂ鍂�D��x�̊����݃n���h��������ꍇ�A���d�����݂��N
          ����܂��B

���T�|�[�g�@�\

          �������������[�h�݂̂ŁA�N���b�N���������[�h�̓T�|�[�g���Ă��܂���B
          �����{�[���[�g�W�F�l���[�^�݂̂ŁA�O���N���b�N���o�͂̓T�|�[�g����
          ���܂���B
          RTS/CTS�ȊO�̃��f��������A���Ȃ킿�ADTR�M���o�́ADSR�M�����́ACD�M
          �����͓��̓T�|�[�g���Ă��܂���B
          �}���`�v���Z�b�T���[�h(MP)�̓T�|�[�g���Ă��܂���B

*/

#include <stdlib.h>
#include <string.h>

#ifdef V3 /* NORTi3 */
 #include "norti3.h"
 #include "nosys3.h"
 #define def_inh     def_int
 #define T_DINH      T_DINT
 #ifndef iwup_tsk
 #define iwup_tsk    wup_tsk
 #endif
#else /* NORTi Ver.4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
 #include "nosys4.h"
#endif

#include "nosio.h"
#include "sh7727.h"

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP          7           /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#ifndef CLK
#define CLK         33333333    /* ���Ӄ��W���[���p�N���b�N[Hz] */
#endif

/* ����M�o�b�t�@�T�C�Y�̒�` */

#ifndef BUFSZ
#define BUFSZ       1024        /* ��M�o�b�t�@�� */
#endif
#ifndef TXBUFSZ
#define TXBUFSZ     BUFSZ       /* ���M�o�b�t�@�� */
#endif

/* �t���[����̒萔 */

#define XOFF_LEN  ((BUFSZ*3)/4) /* XOFF���M�o�b�t�@�� */
#define XON_LEN   ((BUFSZ*1)/4) /* XON ���M�o�b�t�@�� */

#if (BUFSZ > 19)
#define RTSOFF_LEN  (BUFSZ-18)  /* RTS�M��OFF�o�b�t�@�� */
#define RTSON_LEN   (BUFSZ-19)  /* RTS�M��ON �o�b�t�@�� */
#else
#define RTSOFF_LEN  2           /* RTS�M��OFF�o�b�t�@�� */
#define RTSON_LEN   1           /* RTS�M��ON �o�b�t�@�� */
#endif

#define XOFF        0x13        /* ���M��~�L�����N�^�iCTRL-S�j*/
#define XON         0x11        /* ���M�ĊJ�L�����N�^�iCTRL-Q�j*/

/* �_���`���l���ԍ��iCH=0�`3�j*/

#ifndef CH
#define CH          0           /* ���w��̏ꍇ, �_���`���l����0 */
#endif

/* �����`���l���ԍ� */

#ifndef CH0
#ifndef CH2
#if (CH==0)                     /* ���w��̏ꍇ */
#define CH0                     /* �_���`���l��0�̕����`���l����0 */
#elif (CH==1)
#define CH2                     /* �_���`���l��1�̕����`���l����2 */
#elif (CH==2)
#define CH0                     /* �_���`���l��2�̕����`���l����0 */
#else
#define CH2                     /* �_���`���l��3�̕����`���l����2 */
#endif
#endif
#endif

#include "nos7727.h"

/* CTS0����IRQ�ԍ� */

#ifdef CH0
#ifndef NOFLW
#ifndef IRQ
#define IRQ         0           /* 0�`4�FIRQ0�`IRQ4��I���� */
#endif
#define INT_IRQ (INT_IRQ0+IRQ)  /* CTS�����݃x�N�^�ԍ� */
#define PH_CTS  (1<<(IRQ))      /* �|�[�gH CTS���̓r�b�g */
#endif
#endif

/* CTS2����IRQ�ԍ� */

#ifdef CH2
#ifndef NOFLW
#ifdef NOMCE                    /* �������f���R���g���[���@�\���g��Ȃ��ꍇ */
#define IRQ         5           /* RQ5�Œ� */
#define INT_IRQ     INT_IRQ5    /* CTS�����݃x�N�^�ԍ� */
#endif
#define SCP_CTS     SCP7DT      /* SC�|�[�g CTS2���̓r�b�g */
#endif
#endif

/* CHx �ɍ��킹���Ē�` */

#ifdef CH0
#define SCI_SMR     SCI_SCSMR
#define SCI_BRR     SCI_SCBRR
#define SCI_SCR     SCI_SCSCR
#define SCI_TDR     SCI_SCTDR
#define SCI_SSR     SCI_SCSSR
#define SCI_RDR     SCI_SCRDR
#define SCP_TXD     SCP0DT      /* SC�|�[�g TxD0�o�̓r�b�g */
#define SCP_RXD     SCP0DT      /* SC�|�[�g RxD0���̓r�b�g */
#define SCPnMD0     SCP0MD0     /* SC�|�[�g TxD0�o�̓��[�h0 */
#define SCPnMD1     SCP0MD1     /* SC�|�[�g TxD0�o�̓��[�h1 */
#ifndef NOFLW                   /* �n�[�h�E�F�A�t���[����͍s���ꍇ */
#define SCP_RTS     SCP1DT      /* SC�|�[�g RTS0�o�̓r�b�g */
#define SCPrMD0     SCP1MD0     /* SC�|�[�g RTS2�o�̓��[�h0 */
#define SCPrMD1     SCP1MD1     /* SC�|�[�g RTS2�o�̓��[�h1 */
#endif
#endif

#ifdef CH2
#define SCI_SMR     SCIF_SCSMR2
#define SCI_BRR     SCIF_SCBRR2
#define SCI_SCR     SCIF_SCSCR2
#define SCI_TDR     SCIF_SCFTDR2
#define SCI_SSR     SCIF_SCSSR2
#define SCI_RDR     SCIF_SCFRDR2
#define SCIF_SCFRDR SCIF_SCFRDR2
#define SCIF_SCFCR  SCIF_SCFCR2
#define SCIF_SCFDR  SCIF_SCFDR2
#undef  INT_ERI
#undef  INT_RXI
#undef  INT_TXI
#undef  INT_TEI
#define INT_ERI     INT_ERI2
#define INT_RXI     INT_RXI2
#define INT_TXI     INT_TXI2
#define INT_BRI     INT_BRI2
#define SCP_TXD     SCP4DT      /* SC�|�[�g TxD2�o�̓r�b�g */
#define SCP_RXD     SCP4DT      /* SC�|�[�g RxD2���̓r�b�g */
#define SCPnMD0     SCP4MD0     /* SC�|�[�g TxD2�o�̓��[�h0 */
#define SCPnMD1     SCP4MD1     /* SC�|�[�g TxD2�o�̓��[�h1 */
#ifndef NOFLW                   /* �n�[�h�E�F�A�t���[����͍s���� */
#ifdef NOMCE                    /* �������f���R���g���[���@�\���g��Ȃ��ꍇ */
#define SCP_RTS     SCP6DT      /* SC�|�[�g RTS2�o�͒[�q */
#define SCPrMD0     SCP6MD0     /* SC�|�[�g RTS2�o�̓��[�h0 */
#define SCPrMD1     SCP6MD1     /* SC�|�[�g RTS2�o�̓��[�h1 */
#endif
#endif
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

#ifdef SCP_RTS
/*****************************************************************************
* RTS�M��ON/OFF�i�����֐��j
*
******************************************************************************/

static void rts_ini(void)  /* RTS�|�[�g������(RTS OFF) */
{
    UINT psw;

    psw = vdis_psw();
    sfr_set(PORT_SCPDR, SCP_RTS);
    sfr_setw(PORT_SCPCR, SCPrMD0);
    sfr_clrw(PORT_SCPCR, SCPrMD1);
    vset_psw(psw);
}

static void rts_on(void)
{
    UINT psw;

    psw = vdis_psw();
    sfr_clr(PORT_SCPDR, SCP_RTS);
    vset_psw(psw);
}

static void rts_off(void)
{
    UINT psw;

    psw = vdis_psw();
    sfr_set(PORT_SCPDR, SCP_RTS);
    vset_psw(psw);
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

    SIO[CH].txbuf  = txbuf;         /* ���M�o�b�t�@ */
    SIO[CH].txputp = txbuf;         /* ���M�o�b�t�@�i�[�|�C���^�����l */
    SIO[CH].txgetp = txbuf;         /* ���M�o�b�t�@�擾�|�C���^�����l */
    SIO[CH].rxbuf  = rxbuf;         /* ��M�o�b�t�@ */
    SIO[CH].rxputp = rxbuf;         /* ��M�o�b�t�@�i�[�|�C���^�����l */
    SIO[CH].rxgetp = rxbuf;         /* ��M�o�b�t�@�擾�|�C���^�����l */
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

    /* �X�e�[�^�X���� */

  #ifdef CH0
    ssr = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER|SSR_TEND);
    ssr |= SIO[CH].rxsts;               /* �O���M���̃X�e�[�^�X��OR */
    if (sfr_in(PORT_SCPDR) & SCP_RXD)   /* �u���[�NOFF���o, RxD �[�q"1" ? */
        ssr &= ~SSR_MPBT;               /* �u���[�N���o�� MPBT �Ɋ��蓖�� */

    /* �X�e�[�^�X���� */

  #else
    ssr = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER|SSR_RDF|SSR_TEND);
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
  #else
    if (ssr & SSR_RDF)                  /* ��MFIFO�t���̓I�[�o�����Ɠ��� */
  #endif
        stat |= TSIO_OE;
    if (ssr & SSR_FER)                  /* �t���[�~���O�G���[ */
        stat |= TSIO_FE;
  #ifdef CH0
    if (ssr & SSR_MPBT)                 /* �u���[�N���o */
  #else
    if (ssr & SSR_BRK)
  #endif
        stat |= TSIO_BD;

  #ifdef PH_CTS /* CH0 */
    if (sfr_in(PORT_PHDR) & PH_CTS)     /* /CTS ���� */
        stat &= ~TSIO_CTS;
  #endif
  #ifdef SCP_CTS /* CH2 */
    if (sfr_in(PORT_SCPDR) & SCP_CTS)   /* /CTS ���� */
        stat &= ~TSIO_CTS;
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
  #ifndef NOFLW
    UINT psw;
  #endif

    /* RTS�|�[�g������(RTS OFF) */

  #ifdef SCP_RTS
    rts_ini();
  #endif

    /* ���Z�b�g */

    sfr_out(SCI_SCR, 0x00);

    /* SCIF �ł� FIFO �ݒ� */

  #ifndef CH0 /* SCIF */
    sfr_out(SCIF_SCFCR, 0x30);          /* ��M�g���K1, ���M�g���K1 */
    if (SIO[CH].flag & TSF_RTS)
        sfr_set(SCIF_SCFCR, 0x08);      /* MCE = 1: ���f���M�������� */
  #endif

    /* �{�[���[�g�ݒ�l�v�Z */

    baud <<= 4; /* 64�~2^(-1)�~B �̔��� */
    for (cks = 0; cks < 4; cks++, baud <<= 2)
    {   brr = ((CLK) / baud + 1) / 2; /* �l�̌ܓ� */
        if (brr <= 0xffL)
            break;
    }
    if (cks == 4)
        return FALSE;

    /* ���[�h�^�{�[���[�g�ݒ� */

    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* �{�[���[�g��1�r�b�g���Ԍo�ߑ҂� */
    tslp_tsk(2);

    /* �t���[���䂠��ꍇ�́A�������݂������� */

  #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
    {   psw = vdis_psw();
        sfr_setw(INTC_ICR1, 2 << (IRQ * 2));/* IRQ Low���x���Z���X */
        vset_psw(psw);
    }
  #endif

    return TRUE;
}

#ifdef INT_IRQ
/*****************************************************************************
* CTS �����݋֎~�i�����֐��j
*
******************************************************************************/

static void dis_int_cts(void)
{
    UINT psw;

    if (SIO[CH].cmd[1] == 0)
        return;

    psw = vdis_psw();
  #if (IRQ < 4)
    sfr_clrw(INTC_IPRC, 0x000f << (IRQ * 4));
  #else
    sfr_clrw(INTC_IPRD, 0x000f << ((IRQ - 4) * 4));
  #endif
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
    UINT psw;

    if (SIO[CH].cmd[1] != 0)
        return;

    psw = vdis_psw();
  #if (IRQ < 4)
    sfr_setw(INTC_IPRC, IP << (IRQ * 4));
  #else
    sfr_setw(INTC_IPRD, IP << ((IRQ - 4) * 4));
  #endif
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
    sfr_clrw(INTC_IPRB, 0x00f0);
  #endif
  #ifdef CH2
    sfr_clrw(INTC_IPRE, 0x00f0);
  #endif
    vset_psw(psw);

  #ifdef INT_IRQ
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
    sfr_setw(INTC_IPRB, IP << 4);
  #endif
  #ifdef CH2
    sfr_setw(INTC_IPRE, IP << 4);
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

  #ifdef SCP_RTS
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
    {   SIO[CH].flag |= TSF_TXREQ;  /* XON/XOFF ���M�v���Z�b�g */
        sfr_set(SCI_SCR, SCR_TIE);  /* ���M�����݋��� */
    }
  #endif

    /* RTS�t���[���� */

  #ifdef SCP_RTS
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

  #ifdef SCP_RTS
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

    /* ��M�X�e�[�^�X�Ǝ�M��������� */
L1:
    chr = sfr_in(SCI_RDR);
  #ifdef CH0
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);
  #else
    sts = sfr_inw(SCI_SSR) & (SSR_BRK|SSR_FER|SSR_PER|SSR_RDF);
  #endif

    /* ��M�����݂��N���A */

  #ifdef CH0
    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));
  #else
    sfr_outw(SCI_SSR, (UH)~(SSR_ER|SSR_BRK|SSR_RDF|SSR_DR));
  #endif

    /* �u���[�N���o */

  #ifdef CH0
    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_in(PORT_SCPDR) & SCP_RXD))/* RxD �[�q"0" */
        sts |= SSR_MPBT;                /* �u���[�N���o�� MPBT �Ɋ��蓖�� */
  #endif
    SIO[CH].oldsts = SIO[CH].rxsts;     /* �O��̎�M�X�e�[�^�X�L�� */
  #ifdef CH0
    SIO[CH].rxsts = (UB)sts;
  #else
    SIO[CH].rxsts = (UB)sts & ~SSR_RDF;
  #endif
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
    {   SIO[CH].rxtid = 0;
        iwup_tsk((ID)tid);
    }

  #ifndef CH0 /* SCIF */
    if (sfr_inw(SCI_SSR) & (SSR_RDF|SSR_DR))
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
  #ifndef CH0 /* SCIF */
    int cnt;
  #endif

    /* CTS �����݋֎~ */

  #ifdef INT_IRQ
    if (SIO[CH].flag & TSF_RTS)
        dis_int_cts();
  #endif

    /* ���M�f�B�Z�[�u�����Ȃ瑗�M���Ȃ� */

    if (!(sfr_in(SCI_SCR) & SCR_TE))
        goto L2;

    /* RTS�t���[���䂠��Ȃ� CTS �`�F�b�N */
    /* CTS OFF �Ȃ� CTS �����݋��� */

  #ifdef INT_IRQ
    if ((SIO[CH].flag & TSF_RTS) && (sfr_in(PORT_PHDR) & PH_CTS))
    {   ena_int_cts();
        goto L2;
    }
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
        sfr_set(SCI_SCR, SCR_TEIE);     /* ���M�I�������݋��� */
      #else
        if (SIO[CH].tetid != 0)         /* SCIF�ł͂����� */
            iwup_tsk(SIO[CH].tetid);    /* ���M�I���҂����� */
      #endif
L2:
      #ifndef CH0 /* SCIF */
        sfr_clrw(SCI_SSR, SSR_TDFE);
      #endif
        sfr_clr(SCI_SCR, SCR_TIE);      /* ���M�����݋֎~ */
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

  #ifndef CH0 /* SCIF */
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
    ent_int();              /* �����݃n���h���̊J�n */
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

static const T_DINH dinh_sio_rx = { TA_HLNG, int_sio_rx, IP };
static const T_DINH dinh_sio_tx = { TA_HLNG, int_sio_tx, IP };
#ifdef CH0
static const T_DINH dinh_sio_te = { TA_HLNG, int_sio_te, IP };
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

    if (!(SIO[CH].flag & TSF_INIT))
    {
        SIO[CH].flag |= TSF_INIT;
        def_inh(INT_ERI, &dinh_sio_rx);
        def_inh(INT_RXI, &dinh_sio_rx);
        def_inh(INT_TXI, &dinh_sio_tx);
       #ifdef CH0
        def_inh(INT_TEI, &dinh_sio_te);
       #else
        def_inh(INT_BRI, &dinh_sio_rx);
       #endif
       #ifdef INT_IRQ
        if (SIO[CH].flag & TSF_RTS)
            def_inh(INT_IRQ, &dinh_sio_tx);
       #endif
    }

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

  #ifdef SCP_RTS
    rts_off();                          /* RTS OFF */
  #endif

    /* �����ݒ�`�������� */

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

          #ifdef CH0
            if (sts & (SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT))
          #else
            if (sts & (SSR_TEND|SSR_RDF|SSR_FER|SSR_PER|SSR_BRK))
          #endif
            {
              #ifdef CH0
                if (sts & SSR_RDRF)
                    ercd = EV_SIOOVF;
                else if (sts & SSR_ORER)
                    ercd = EV_SIOORN;
                else if (sts & SSR_MPBT)
                    ercd = EV_SIOBRK;
              #else
                if (sts & SSR_TEND)
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
        chg_ims(0);
        ercd = tslp_tsk(tmout);
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
        sfr_clr(PORT_SCPDR, SCP_TXD);
        sfr_setw(PORT_SCPCR, SCPnMD0);
        vset_psw(psw);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        psw = vdis_psw();
        sfr_set(PORT_SCPDR, SCP_TXD);
        sfr_clrw(PORT_SCPCR, SCPnMD0);
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

  #ifdef SCP_RTS
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
  #ifndef CH0 /* SCIF */
    UW t = SYSCK.ltime;
  #endif

    for (;;)
    {
        chg_ims(IP);

        if (SIO[CH].txcnt == 0)
        {
          #ifdef CH0
            if (sfr_in(SCI_SSR) & SSR_TEND)
            {   chg_ims(0);
                return E_OK;            /* ����I�� */
            }
          #else /* SCIF */
            if (sfr_inw(SCI_SSR) & SSR_TEND)
            {   chg_ims(0);
                return E_OK;            /* ����I�� */
            }
            else
            {   chg_ims(0);
                if (tmout == 0)         /* �|�[�����O? */
                    return E_TMOUT;
                for (;;)
                {   rot_rdq(TPRI_RUN);
                    if (sfr_inw(SCI_SSR) & SSR_TEND)
                        return E_OK;    /* ����I�� */
                    if ((UW)(SYSCK.ltime - t) >= (UW)tmout)
                        return E_TMOUT; /* �^�C���A�E�g�I�� */
                }
            }
          #endif
        }

        /* ���M�I�������ݑ҂� */

        SIO[CH].tetid = vget_tid();
        chg_ims(0);
        ercd = tslp_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
        if (ercd)
            return ercd;                /* �^�C���A�E�g�I�� */
    }
}

/* end */
