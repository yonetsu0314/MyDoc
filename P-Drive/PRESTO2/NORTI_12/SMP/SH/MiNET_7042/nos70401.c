/****************************************************************************
* �V���A�����o�̓h���C�o�iSH7040�V���[�Y�j                                  *
*                                                                           *
*  File name : nos7040.c                                                    *
*  Copyright (c) 1998-2002, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                     *
*                                                                           *
*  Compile : shc -cp=sh2 -op=1 -def=CLK=? nos7040.c        (Renesas C)      *
*            gcc -c -m2 -O2 -DCLK=? nos7040.c              (exeGCC)         *
*            ccsh -c -cpu=sh_ii -ansi -O -DCLK=? nos7040.c (GHS)            *
*                                                                           *
* 27/Feb/1998 �V�K�쐬(nos7040.c)                                        MI *
* 17/Apr/2000 NORTi Ver.4 ���p�ɉ���(nos7040.c)                          FJ *
* 30/Oct/2000 IAR EW �Ή�                                                TK *
* 23/Dec/2002 XASS-V�̔��ʂ�ǉ�, �}�N���Ɋւ���R�����g�ǉ�             MI *
* 23/Dec/2002 SH7042/44/45�ŋ��ʂ̂���nos7040.c�ɖ߂�                    MI *
* 31/Jul/2003 BRR�ݒ�� �E�F�C�g�}��                                     KA *
****************************************************************************/

/*

�œK���R���p�C��

          �����݃n���h�����܂܂��̂ŁA�K���œK���I�v�V������t���ăR���p�C
          �����Ă��������B

�_���`���l���ԍ��̎w��

          CH�}�N���ŁA�_���`���l���ԍ����w��ł��܂��B(���w�莞�́ACH=0)
          SH7040�V���[�Y�̓����V���A����2�`���l���ł����A�O�t��UART�̑��݂�
          �l�����āACH�ɂ́A0�`3���w��ł���悤�ɂ��Ă��܂��B

          �`���l��0�ƃ`���l��1�̗����������N����ꍇ�́A�d�����Ȃ����W���[��
          ���ƂȂ�悤�R���p�C�����Ă��������B�{�\�[�X�t�@�C����ʖ��ŃR�s�[
          ����̂��ȒP�ł��B

          (��) shc <options> nos7040.c �c�c�c�c�c�c�c�c�c�c�c�c �`���l��0
               copy nos7040.c nos7ch1.c
               shc <options> -def=CH=1 nos7ch1.c �c�c�c�c�c�c�c �`���l��1

          (��) gcc <options> nos7040.c �c�c�c�c�c�c�c�c�c�c�c�c �`���l��0
               copy nos7040.c nos7ch1.c
               gcc <options> -DCH=1 nos7ch1.c  �c�c�c�c�c�c�c�c �`���l��1

�����`���l���ԍ��̎w��

          CH0, CH1�}�N���ŁA�����`���l���ԍ���ύX�ł��܂��B
          CH0���`����ƁASCI0���g���悤�ɃR���p�C������܂��B
          CH1���`����ƁASCI1���g���悤�ɃR���p�C������܂��B
          �{�}�N�����`���Ȃ��ꍇ�ACH=0/CH=2�ł�CH0�Ƃ��āACH=1/CH=3�ł�
          CH1�Ƃ��ăR���p�C�������̂ŁA�ʏ�́A�{�}�N���͒�`�s�v�ł����A
          SCI1�݂̂̃n�[�h�E�F�A�ŁASCI1��CH=0�Ɋ���t����ꍇ�Ȃǂ́A�{�}
          �N���𗘗p���Ă��������B

          (��) shc <options> -def=CH=0,CH1 nos7040.c �c�c�c SCI1���`���l��0
                                       ~~~
          (��) gcc <options> -DCH=0 -DCH1 nos7040.c
                                      ~~~
�{�[���[�g�W�F�l���[�^

          �\�[�X�N���b�N�̃f�t�H���g��24MHz �ł��B�ύX����ꍇ��CLK�}�N����
          ��`���ăR���p�C�����Ă��������B�l��[Hz]�P�ʂ̎��g���ł��B

          (��) shc <options> -def=CLK=24576000 nos7040.c �c�c�c 24.576 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=24576000 nos7040.c
                               ~~~~~~~~~~~~
����M�o�b�t�@�T�C�Y�̎w��

          ��M/���M�o�b�t�@�̃T�C�Y���ABUFSZ�}�N���ɂ���`���ăR���p�C����
          �Ă��������B��M�o�b�t�@�ƈقȂ�T�C�Y�̑��M�o�b�t�@��p����ꍇ�́A
          ����� TXBUFSZ�}�N�����`���Ă��������B
          ���w�莞�́A��M/���M�o�b�t�@��1024�o�C�g�ƂȂ�܂��B

          (��) ��M�o�b�t�@, ���M�o�b�t�@��128�o�C�g BUFSZ=128
          (��) ��M�o�b�t�@128, ���M�o�b�t�@64�o�C�g BUFSZ=128, TXBUFSZ=64

          �L�����N�^���̎�M�X�e�[�^�X���o�b�t�@�����O���Ă���̂ŁA���ۂ�
          ��M�o�b�t�@�̃T�C�Y�͏�L�̔{�ƂȂ�܂��B

�\�t�g�E�F�A�t���[����

          XON/XOFF�L�����N�^�ɂ��t���[�����p���Ȃ��ꍇ�ANOXON�}�N�����`
          ���ăR���p�C�����Ă��������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B

�n�[�h�E�F�A�t���[����

          RTS�M����ON/OFF���鎟�̊֐����A�O���ɒ�`���Ă��������B

          void rts0_on(void); �c�c� �����`���l��0 RTS�M�� ON
          void rts0_off(void); �c�c �����`���l��0 RTS�M�� OFF
          void rts1_on(void); �c�c� �����`���l��1 RTS�M�� ON
          void rts1_off(void); �c�c �����`���l��1 RTS�M�� OFF

          RTS�M���o�͂������ꍇ�Arts?_on/rts?_off �֐��͉����������^�[������
          ���������B
          RTS�M���|�[�g�̃r�b�g���삪���^�X�N�Ƌ�������ꍇ���l�����āA�{�h��
          �C�o����́A��L�֐������荞�݋֎~(���x��15)�ŌĂяo���Ă��܂��B��
          �����鑼�^�X�N������A���荞�݋֎~��Ԃœ��|�[�g�𑀍삵�Ă��������B

          RTS/CTS�M���ɂ��t���[������s���ꍇ�ACTS ON�ɂ�郌�x���Z���X����
          �݂��K�v�ł��B
          �`���l��0 /CTS �M���� /IRQ0(PA2)�[�q�ɐڑ����Ă��������B
          �`���l��1 /CTS �M���� /IRQ1(PA5)�[�q�ɐڑ����Ă��������B
          ����/IRQn��p����ꍇ�́A�{�\�[�X�t�@�C���̕ύX���K�v�ł��B

          �n�[�h�E�F�A�t���[��p���Ȃ��ꍇ�́ANOFLW�}�N����`���ăR���p�C����
          �Ă��������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B
          �܂��Arts?_on/rts?_off�֐��̒�`���s�v�ƂȂ�܂��B

�����ݗD�揇��

          �{�h���C�o�́A�f�t�H���g�̊����ݗD�揇�ʂ́A7�ł��B
          �����ύX����ꍇ�AIP�}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B(IP �� KNL_LEVEL)

          (��) shc <options> -def=IP=5 nos7040.c �c� �����ݗD�揇�� 5
                                  ~~~~
          (��) gcc <options> -DIP=5 nos7040.c
                               ~~~~
�|�[�g�̏�����

          SCI�ƌ��p�ɂȂ��Ă���|�[�gA �́A�O���ŁA���̗�̗l�ɏ��������Ă���
          �����B(���L�̗�ł́APA�̑��̒[�q�͓��͂Ɖ���)

          #include "nos7040.h"
          sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0); �c�c TXD1,TXD0�[�q"1"
          sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0); �c TXD1,TXD0�[�q�o�͕���
          sfr_setw(PFC_PACRL2, 0x0145); �c�c�c�c�c� TXD1,RXD1,TXD0,RXD0 �@�\

          RTS/CTS�M���ɂ��t���[������s���ꍇ�́A���̗�̗l�ɏ��������Ă�
          �������B

          #define PA_RTS0  0x40  �c�c�c�c�c�c�c�c�  PA6 �`���l��0 RTS�M��(��)
          #define PA_RTS1  0x80  �c�c�c�c�c�c�c�c�  PA7 �`���l��1 RTS�M��(��)
          sfr_setw(IO_PADRL, PA_TXD1|PA_TXD0|PA_RTS1|PA_RTS0);
          sfr_setw(PFC_PAIORL, PA_TXD1|PA_TXD0|PA_RTS1|PA_RTS0);
          sfr_setw(PFC_PACRL2, 0x0d75); �c�c�c�c�c IRQ1(CTS1),IRQ0(CTS0)�@�\

          ���̏ꍇ�� rts?_on/rts?_off�֐���͎��̒ʂ�ł��B

          void rts0_on(void)  { sfr_clrw(IO_PADRL, PA_RTS0); }
          void rts0_off(void) { sfr_setw(IO_PADRL, PA_RTS0); }
          void rts1_on(void)  { sfr_clrw(IO_PADRL, PA_RTS1); }
          void rts1_off(void) { sfr_setw(IO_PADRL, PA_RTS1); }

���T�|�[�g�@�\

          �������������[�h�݂̂ŁA�N���b�N���������[�h�̓T�|�[�g���Ă��܂���B
          �����{�[���[�g�W�F�l���[�^�݂̂ŁA�O���N���b�N���o�͂̓T�|�[�g����
          ���܂���B
          DTR�M���o�́ADSR�M�����́ACD�M�����͂̓T�|�[�g���Ă��܂���B
          SH�V���[�Y���L�̃}���`�v���Z�b�T���[�h�̓T�|�[�g���Ă��܂���B

NORTi�̃o�[�W����

          �{�t�@�C���́ANORTi2, NORTi3, NORTi Ver.4�Ō��p�ƂȂ��Ă��܂��B
          V2, V3, V4 �}�N���ŁANORTi�̃o�[�W�������w�肵�Ă��������B
          �ȗ������ꍇ�́AV4 �ƌ��Ȃ��܂��B
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
 #define V4
 #endif
 #include "kernel.h"
#endif

#include "nosio.h"
#include "sh7040.h"
#include "nos7040.h"

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
#define IP          7           /* IP ����`�Ȃ� 7 �Ƃ��� */
#endif

/* �N���b�N�̒�` */

#define MHz         1000000L
#ifndef CLK
#define CLK         24*MHz      /* CPU�N���b�N[Hz] */
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

#if defined(GAIO)               /* XASS-V */
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
#define SCI_SMR     SCI_SMR0    /* �V���A�����[�h���W�X�^ */
#define SCI_BRR     SCI_BRR0    /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR     SCI_SCR0    /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR     SCI_TDR0    /* �g�����X�~�b�g�f�[�^���W�X�^ */
#define SCI_SSR     SCI_SSR0    /* �V���A���X�e�[�^�X���W�X�^ */
#define SCI_RDR     SCI_RDR0    /* ���V�[�u�f�[�^���W�X�^ */
#define PA_TXD      PA_TXD0     /* TxD �o�͒[�q */
#define PA_RXD      PA_RXD0     /* RxD ���͒[�q */
#define PA_CTS      PA_CTS0     /* CTS ���͒[�q */
#define INT_ERI     INT_ERI0    /* ��M�G���[�����݃x�N�^�ԍ� */
#define INT_RXI     INT_RXI0    /* ��M���������݃x�N�^�ԍ� */
#define INT_TXI     INT_TXI0    /* ���M�f�[�^�G���v�e�B�����݃x�N�^�ԍ� */
#define INT_TEI     INT_TEI0    /* ���M���������݃x�N�^�ԍ� */
#define INT_IRQ     INT_IRQ0    /* CTS �����݃x�N�^�ԍ� */
#define IRQ         0           /* CTS �����ݗv���iIRQ0�j*/
#endif

#ifdef CH1
#define SCI_SMR     SCI_SMR1
#define SCI_BRR     SCI_BRR1
#define SCI_SCR     SCI_SCR1
#define SCI_TDR     SCI_TDR1
#define SCI_SSR     SCI_SSR1
#define SCI_RDR     SCI_RDR1
#define PA_TXD      PA_TXD1
#define PA_RXD      PA_RXD1
#define PA_CTS      PA_CTS1
#define INT_ERI     INT_ERI1
#define INT_RXI     INT_RXI1
#define INT_TXI     INT_TXI1
#define INT_TEI     INT_TEI1
#define INT_IRQ     INT_IRQ1
#define IRQ         1           /* CTS �����ݗv���iIRQ1�j*/
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

#ifndef NOFLW
/*****************************************************************************
* RTS �M�� ON/OFF�i�����֐��j
*
******************************************************************************/

static void rts_on(void)
{
    UINT psw;

    psw = vdis_psw();       /* ���荞�݋֎~ */
  #ifdef CH0
    rts0_on();
  #endif
  #ifdef CH1
    rts1_on();
  #endif
    vset_psw(psw);          /* ���荞�݋֎~���� */
}

static void rts_off(void)
{
    UINT psw;

    psw = vdis_psw();
  #ifdef CH0
    rts0_off();
  #endif
  #ifdef CH1
    rts1_off();
  #endif
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

    if (sfr_inw(IO_PADRL) & PA_RXD) /* RxD �[�q"1" */
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
  #ifndef NOFLW
    if (sfr_inw(IO_PADRL) & PA_CTS) /* /CTS ���� */
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

    /* RTS OFF */

  #ifndef NOFLW
    rts_off();
  #endif

    /* ���Z�b�g */

    sfr_out(SCI_SCR, 0x00);

    /* �{�[���[�g�ݒ�l�v�Z */

    cks = 0;
    for (brr = (CLK) / (baud * 32); brr > 0x100L; brr >>= 2)
    {   if (++cks > 3)
            return FALSE;
    }

    /* ���[�h�^�{�[���[�g�ݒ� */

    sfr_out(SCI_SMR, (UB)(mode | cks));
    sfr_out(SCI_BRR, (UB)(brr - 1));

    /* �{�[���[�g��1�r�b�g���Ԍo�ߑ҂� */
    tslp_tsk(2);

    /* �t���[���䂠��ꍇ�́A�������݂������� */

  #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        sfr_clr(INTC_ICR + 1, 0x80 >> IRQ); /* IRQ ���x���Z���X */
  #endif

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
  #if (IRQ<4)
    sfr_clrw(INTC_IPRA, 0x000f << (12 - (IRQ * 4)));
  #else
    sfr_clrw(INTC_IPRB, 0x000f << (12 - ((IRQ - 4) * 4)));
  #endif
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
  #if (IRQ<4)
    sfr_setw(INTC_IPRA, (IP) << (12 - (IRQ * 4)));
  #else
    sfr_setw(INTC_IPRB, (IP) << (12 - ((IRQ - 4) * 4)));
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
    sfr_clrw(INTC_IPRF, 0x00f0);
  #endif
  #ifdef CH1
    sfr_clrw(INTC_IPRF, 0x000f);
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
    sfr_setw(INTC_IPRF, IP << 4);
  #endif
  #ifdef CH1
    sfr_setw(INTC_IPRF, IP);
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

    /* RTS�t���[���� */

  #ifndef NOFLW
    if ((cnt == RTSOFF_LEN)
     && (SIO[CH].flag & TSF_RTS))
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

  #ifndef NOFLW
    if ((cnt == RTSON_LEN)
     && (SIO[CH].flag & TSF_RTS))
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

    /* ��M�����ƃX�e�[�^�X����� */

    chr = sfr_in(SCI_RDR);
    sts = sfr_in(SCI_SSR) & (SSR_ORER|SSR_FER|SSR_PER);

    /* ��M�����݂��N���A */

    sfr_out(SCI_SSR, (UB)~(SSR_RDRF|SSR_ORER|SSR_FER|SSR_PER|SSR_MPBT));

    /* �u���[�N���o */

    if ((sts & SSR_FER)                 /* �t���[�~���O�G���[�� */
     && !(sfr_inw(IO_PADRL) & PA_RXD))  /* RxD �[�q"0" */
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
    if ((SIO[CH].flag & TSF_RTS) && (sfr_inw(IO_PADRL) & PA_CTS))
    {   ena_int_cts();
        goto L2;
    }
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
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, &dinh_sio_tx);
   #endif
  #else
    def_int(INT_ERI, &dint_sio_rx);
    def_int(INT_RXI, &dint_sio_rx);
    def_int(INT_TXI, &dint_sio_tx);
    def_int(INT_TEI, &dint_sio_te);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, &dint_sio_tx);
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
    def_inh(INT_TEI, (T_DINH *)NADR);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_inh(INT_IRQ, (T_DINH *)NADR);
   #endif
  #else
    def_int(INT_ERI, (T_DINT *)NADR);   /* �����ݒ�`�������� */
    def_int(INT_RXI, (T_DINT *)NADR);
    def_int(INT_TXI, (T_DINT *)NADR);
    def_int(INT_TEI, (T_DINT *)NADR);
   #ifndef NOFLW
    if (SIO[CH].flag & TSF_RTS)
        def_int(INT_IRQ, (T_DINT *)NADR);
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

    chg_ims(IP);

    /* �o�b�t�@�N���A */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* �u���[�N���M */

    if (fncd & TSIO_SBON)
    {   fncd |= TSIO_TXD;
        sfr_clrw(IO_PADRL, PA_TXD);
    }
    if (fncd & TSIO_SBOFF)
    {   fncd |= TSIO_TXE;
        sfr_setw(IO_PADRL, PA_TXD);
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
