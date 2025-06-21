/******************************************************************************
* �V���A�����o�̓h���C�o  MS7709, MS7709A and MS7750 with NS16550(TL16C552)   *
*                                                                             *
*  File name : nos16550.c                                                     *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*  Compile : shc -cp=sh3 -op=1 -def=CLK=? nos16550.c         (Renesas C)      *
*            gcc -c -m3 -O2 -DCLK=? nos16550.c               (exeGCC)         *
*            ccsh -c -cpu=sh_iii -ansi -O -DCLK=? nos16550.c (GHS)            *
*                                                                             *
* 19/Apr/2000                                                                 *
* 12/Sep/2001 ����n�̔��f��������                                         TA *
* 27/Dec/2002 V4�}�N���̕s�v���A esle �� V4 �ƌ��Ȃ��悤���������ւ�     KA *
* 25/Feb/2003 "Green Hills" �R���p�C���̔��f�� __ghs �ł���悤�ɏC��      KA *
* 25/Jun/2003 SMSC Super I/O FDC37C935A �Ή��R�[�h�ǉ�                     KA *
******************************************************************************/

/*

�œK���R���p�C��

          �����݃n���h�����܂܂��̂ŁA�K���œK���I�v�V������t���ăR���p�C
          �����Ă��������B

�`���l���ԍ��̎w��

          CH �}�N���ŁA�_���`���l���ԍ����w��ł��܂��B�i���w�莞�́ACH=0�j
          CH �� 0�`3 ���w��ł��܂��B

          CH0, CH1 �}�N���ŁA�����`���l���ԍ���ύX�ł��܂��B
          CH0 ���`����ƁATL16C552 �� SCI0 ���g���悤�ɃR���p�C������܂��B
          CH1 ���`����ƁATL16C552 �� SCI1 ���g���悤�ɃR���p�C������܂��B

          �`���l��0 �ƃ`���l��1 �̗����������N����ꍇ�́A�d�����Ȃ����W���[
          �����ƂȂ�悤�R���p�C�����Ă��������B

          (��) shc <options> nos16550.c ������������������������� �`���l��0
               copy nos16550.c nos16ch1.c
               shc <options> -def=CH=1 nos16ch1.c  �������������� �`���l��1

          (��) gcc <options> nos16550.c ������������������������� �`���l��0
               gcc <options> -DCH=1 -onos16ch1.o nos16550.c ����� �`���l��1

          (��) ccsh <options> nos16550.c  ����������������������� �`���l��0
               ccsh <options> -DCH=1 -o nos16ch1.o nos16550.c ��� �`���l��1

�{�[���[�g�W�F�l���[�^
          CLKTYPE�}�N���ŃN���b�N�W�F�l���[�^�ɑ΂��鏈�����@��؂�ւ��܂��B
          CLKTYPE=0�ƒ�`����ƁA�ʏ�̕�����v�Z���s���܂��B
          ���̂Ƃ��̃\�[�X�N���b�N�̃f�t�H���g�� 14.7456 MHz �ł��B
          �ύX����ꍇ �� CLK�}�N�����`���ăR���p�C�����Ă��������B
          16550�݊��`�b�v�������̂܂ܓ��ڂ���Ă���Ƃ��g�p���܂��B

          CLKTYPE=1�ƒ�`����ƁA����{�[���[�g�ȏ�̓{�[���[�g���ɌŒ�l��
          �Z�b�g���܂��B
          SMSC FDC37C93x�iUltra I/O�j�͓�����1.8462MHz���f�t�H���g�̃\�[�X
          �N���b�N�ł��B����IC��115.2K�ȏ�ł́A������Ƃ��ăZ�b�g�����l��
          ����āA�����̃\�[�X�N���b�N���̂��̂�؂�ւ��܂��B

                 ������         ���{�[���[�g
                    1             115200
                32770             230400
                32769             460800

          CLK�}�N���̒l�� [Hz] �P�ʂ̎��g���ł��B
          ��TL16C552A��16MHz���̃N���b�N���g�p�\�ł��B

          (��) shc <options> -def=CLK=8000000 nos16550.c ������ 8 MHz
                                  ~~~~~~~~~~~~
          (��) gcc <options> -DCLK=8000000 nos16550.c
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

          NS16550/TL16C552/TL16C552A �̃`���l���ɑΉ����� CTS,RTS �̒[�q��
          /CTS , /RTS �M����ڑ����Ă��������B

          �n�[�h�t���[��p���Ȃ��ꍇ�́ANOFLW �}�N����`���ăR���p�C�����Ă�
          �������B�R�[�h�T�C�Y�Ə������x��ߖ�ł��܂��B

�����ݗD�揇��

          �{�n���h���́A�f�t�H���g�̊����ݗD�揇�ʂ́A7 �ł��B
          �����ύX����ꍇ�AIP �}�N�����`���ăR���p�C�����Ă��������B
          �������A�J�[�l����荂�D��ɂ͂ł��܂���B�iIP �� KNL_LEVEL�j
          �{�n���h����荂�D��x�̊����݃n���h�����L��ꍇ�A���d�����݂��N��
          ��܂��B

          (��) shc <options> -def=IP=5 nos16550.c ��� �����ݗD�揇�� 5
                                  ~~~~
          (��) gcc <options> -DIP=5 nos16550.c
                               ~~~~
          MS7750SE �ł́ACH0 �� 8�ACH1 �� 12 �ɁA���ꂼ��Œ肳��Ă��܂��B

���f���M���T�|�[�g

          16550�o�[�W�����ł̓��f������M����t���[����M���͕W���œ��ڂ����
          ���܂��̂ŁA�{�n���h���ł��W���T�|�[�g�Ƃ��܂����B
          �������ARI�iRing Indicator)�M���ɂ��ẮA�]���ł�NOSIO�Ƃ̌݊���
          ����A���T�|�[�g�Ƃ��܂����B

���T�|�[�g�@�\


*/

#include <stdlib.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif

#ifdef V2
 #include "norti.h"
 #define PSW         UW
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
  #else
   #define CH0             /* ���w��̏ꍇ, �_���`���l��3 �̕����`���l����0 */
  #endif
 #endif
#endif

#include "nos16550.h"

/* CHx �ɍ��킹���Ē�` */

#ifdef CH0
 #define UART_RBR   UART0_RBR
 #define UART_THR   UART0_THR
 #define UART_DLL   UART0_DLL
 #define UART_IER   UART0_IER
 #define UART_DLM   UART0_DLM
 #define UART_IIR   UART0_IIR
 #define UART_FCR   UART0_FCR
 #define UART_LCR   UART0_LCR
 #define UART_MCR   UART0_MCR
 #define UART_LSR   UART0_LSR
 #define UART_MSR   UART0_MSR
 #define UART_SCR   UART0_SCR
 #define INT_UART   INT_UART0
 #define IP_UART    IP_UART0
#endif

#ifdef CH1
 #define UART_RBR   UART1_RBR
 #define UART_THR   UART1_THR
 #define UART_DLL   UART1_DLL
 #define UART_IER   UART1_IER
 #define UART_DLM   UART1_DLM
 #define UART_IIR   UART1_IIR
 #define UART_FCR   UART1_FCR
 #define UART_LCR   UART1_LCR
 #define UART_MCR   UART1_MCR
 #define UART_LSR   UART1_LSR
 #define UART_MSR   UART1_MSR
 #define UART_SCR   UART1_SCR
 #define INT_UART   INT_UART1
 #define IP_UART    IP_UART1
#endif

#define INT_SIO     INT_UART
#define SIO_DR      UART_RBR
#define SIO_IER     UART_IER
#define SIO_IIR     UART_IIR
#define SIO_FCR     UART_FCR
#define SIO_LCR     UART_LCR
#define SIO_MCR     UART_MCR
#define SIO_MSR     UART_MSR
#define SIO_LSR     UART_LSR

/* �����ݗD�揇�ʂ̒�` */

#ifndef IP
 #if defined(CPU_H8S)
 #if INTM==2
 #define IP          IP_UART
 #else
 #define IP          1
 #endif
 #else
 #define IP          IP_UART
 #endif
#endif

/* �N���b�N�̒�` */
#ifndef CLKTYPE
 #define CLKTYPE 0
#endif

#if CLKTYPE == 0                /* �����ɁA�O���N���b�N������^�C�v   */
 #ifndef CLK
  #define CLK         14745600  /* CLK���͎��g��[Hz] */
 #endif
#else                           /* �X�[�p�[�h/�n�iSMSC FDC37C93X�j      */
 #ifndef CLK
  #define CLK          1846200  /* �����N���b�N                         */
 #endif
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

#ifndef NOFLW
 #define INT_IRQ   (INT_UART)       /* CTS �����݃x�N�^�ԍ� */
#endif

/* �����݃n���h���̍Ē�` */

#ifdef CH
 #if (CH==0)
  #define int_sio  int_sio0
 #elif (CH==1)
  #define int_sio  int_sio1
 #elif (CH==2)
  #define int_sio  int_sio2
 #elif (CH==3)
  #define int_sio  int_sio3
 #endif
#endif

#if defined(__HITACHI__)
 #if (CH==0)
  #pragma interrupt(int_sio0)
 #elif (CH==1)
  #pragma interrupt(int_sio1)
 #elif (CH==2)
  #pragma interrupt(int_sio2)
 #elif (CH==3)
  #pragma interrupt(int_sio3)
 #endif
#endif

/* �V���A�����o�͐���u���b�N */

extern T_SIO SIO[];

/* �����ϐ� */

static UB txbuf[TXBUFSZ];       /* ���M�o�b�t�@ */
static UB rxbuf[BUFSZ*2];       /* ��M�o�b�t�@, ��M�X�e�[�^�X�o�b�t�@ */

/* ���䕶�� */

#define XOFF        0x13        /* ���M��~�iCTRL-S�j*/
#define XON         0x11        /* ���M�ĊJ�iCTRL-Q�j*/

#ifndef NOFLW
/*****************************************************************************
* RTS �M�� ON/OFF�i�����֐��j
*
******************************************************************************/

static void rts_on(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_RTS);
    vset_psw(psw);
}

static void rts_off(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_RTS);
    vset_psw(psw);
}

#endif
/*****************************************************************************
* DTR �M�� ON/OFF�i�����֐��j
*
******************************************************************************/

static void dtr_on(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_DTR);
    vset_psw(psw);
}

static void dtr_off(void)
{
    PSW psw;

    psw = vdis_psw();
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_DTR);
    vset_psw(psw);
}

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
******************************************************************************/

static int get_stat(void)
{
    int stat;
    int ssr;
    int m;

    /* ���݂̃��f���X�e�[�^�X���� */

    m = sci_in( SIO_MSR );

    /* ���f���X�e�[�^�X�r�b�g�ҏW */

    stat = 0;
    if (m & SIO_CTS)
        stat |= TSIO_CTS;
    if (m & SIO_DSR)
        stat |= TSIO_DSR;
    if (m & SIO_CD)
        stat |= TSIO_CD;

    /* ���݂̃��C���X�e�[�^�X���� */

    ssr = sci_in(SIO_LSR);

    /* ��M���̃X�e�[�^�X�̃G���[�r�b�g��OR */

    ssr |= (SIO[CH].rxsts & (SIO_FE|SIO_OE|SIO_PE));

    /* ���C���X�e�[�^�X�r�b�g�ҏW */

    if (ssr & SIO_PE)         /* �p���e�B�G���[       */
        stat |= TSIO_PE;
    if (ssr & SIO_OE)         /* �I�[�o�[�����G���[   */
        stat |= TSIO_OE;
    if (ssr & SIO_FE)         /* �I�[�o�[�����G���[   */
        stat |= TSIO_FE;
    if (ssr & SIO_BD)         /* �u���[�N���o         */
        stat |= TSIO_BD;
    if (ssr & SIO_TxEMP)      /* ���M�o�b�t�@��       */
        stat |= TSIO_TXEMP;

    return stat;
}

/*****************************************************************************
* SMSC Super I/O FDC37C935A
*   Super I/O ��Configuration 16550
******************************************************************************/
#ifdef FDC_CONF_PORT

#define  FDC_INDEX_PORT FDC_CONF_PORT
#define  FDC_DATA_PORT  FDC_CONF_PORT+2

static void  fdc_cfgw(unsigned short  index, unsigned short  data)
{
    port_outw(FDC_INDEX_PORT, index);
    port_outw(FDC_DATA_PORT, data);
}

static void fdc_com_cfg(void)
{
#ifdef CH0
    unsigned short add = 0x03f8;    /* UART0 = 0xB04007F0   */
#else
    unsigned short add = 0x02f8;    /* UART1 = 0xB04005F0   */
#endif

unsigned short low_add;
unsigned short high_add;

    low_add = ((add << 8) & 0xff00);
    high_add = (add & 0xff00);

    port_outw(FDC_CONF_PORT, 0x5500);   /* Entering the configuration state */
    port_outw(FDC_CONF_PORT, 0x5500);

#ifdef CH0
    fdc_cfgw(0x0700, 0x0400);
#else
    fdc_cfgw(0x0700, 0x0500);
#endif
    fdc_cfgw(0x3000, 0x0100);
    fdc_cfgw(0x6000, high_add);
    fdc_cfgw(0x6100, low_add);
#ifdef CH0
    fdc_cfgw(0x7000, 0x0400);   /* IRQ4 */
#else
    fdc_cfgw(0x7000, 0x0300);   /* IRQ3 */
#endif
    fdc_cfgw(0xF000, 0x0000);
    fdc_cfgw(0x0700, 0x0000);

    port_outw(FDC_CONF_PORT, 0xAA00);   /* Exiting the configuration state */
}
#endif  /* FDC_CONF_PORT */


/*****************************************************************************
* �r�h�n�f�o�C�X�������i�����֐��j
*
******************************************************************************/

static void init_sio(UB mode, UW baud)
{
    UH tc;

#ifdef FDC_CONF_PORT
    fdc_com_cfg();
#endif

    /* RTS OFF */

#ifndef NOFLW
    rts_off();
#endif

    /* ���Z�b�g & FIFO �ݒ�(��M�g���K1) */

    sci_out(SIO_FCR, (B)(SIO_TRG08|SIO_DMA|SIO_TXFRST|SIO_RXFRST|SIO_FIFOEN));

    /* ���C���R���g���[������W�X�^�ݒ� */

    sci_out(SIO_LCR, SIO[CH].cmd[1] = mode);

    /* ���f���R���g���[������W�X�^��N���A */

    sci_out(SIO_MCR, 0);

    /* �{�[���[�g�������W�X�^�ݒ� */
#if CLKTYPE == 1
    /*
     * FDC37C93X �̗l�ɐݒ肵���l�ŁA�����I�ɃN���b�N�\�[�X��
     * �ύX����^�C�v�́A�l�ł��߂����ɂł����邵���Ȃ��B
     */
    if ( baud >= 115200 ){
        if ( baud < 230400 )
            tc = 1;
        else if ( baud < 460800 )
            tc = 32770;
        else
            tc = 32769;
        sci_out(SIO_LCR, SIO[CH].cmd[1] |= SIO_DLAB);   /* DLAB = 1     */
        sci_out(SIO_DR, tc);
        tc >>= 8;
        sci_out(SIO_IER, tc);
        sci_out(SIO_LCR, SIO[CH].cmd[1] &= ~SIO_DLAB);  /* DLAB = 0     */
        return;
    }
#endif
    sci_out(SIO_LCR, SIO[CH].cmd[1] |= SIO_DLAB);   /* DLAB = 1     */
    tc = (UH)(CLK / (baud*16));                     /* *16 Clock    */
    sci_out(SIO_DR, tc);
    tc >>= 8;
    sci_out(SIO_IER, tc);
    sci_out(SIO_LCR, SIO[CH].cmd[1] &= ~SIO_DLAB);  /* DLAB = 0     */
}

/*****************************************************************************
* �r�h�n�֑��M�������o�́i�����֐��j
*
* �o�͂ł����ꍇ TRUE�A�o�͂ł��Ȃ������ꍇ FALSE ��Ԃ��B
******************************************************************************/

static BOOL tx_chr(UB c)
{
    /* ���M���f�B�łȂ������瑗�M�����݋��� */

    if (!(sci_in(SIO_LSR) & SIO_TxRDY))
    {   sci_out(SIO_IER, SIO[CH].cmd[0] |= SIO_TxIE);
        return FALSE;
    }

    /* �t���[���䂠��Ȃ� CTS �`�F�b�N */
    /* CTS OFF �Ȃ� ���f���X�e�[�^�X�����݋��� */

    if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
    {   if (!(sci_in(SIO_MSR) & SIO_CTS))
        {   sci_out(SIO_IER, SIO[CH].cmd[0] |= SIO_MSIE);
            return FALSE;
        }
    }

    /* �P�������M */

    sci_out(SIO_DR, SIO[CH].txchr = c);
    return TRUE;
}

/*****************************************************************************
* �V���A�������݋֎~�i�����֐��j
*
******************************************************************************/

static void dis_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
    /*
     * MCR�Ŋ����֎~�ɂ��Ă��܂�
     *    ��   �����R���g���[���Ɉˑ����Ȃ���CTS���֎~�o����
     */
    sci_out(SIO_MCR, SIO[CH].cmd[2] &= ~SIO_OUT2);

    vset_psw(psw);

}

/*****************************************************************************
* �V���A�������݋��i�����֐��j
*
******************************************************************************/

static void ena_int_sio(void)
{
    PSW psw;

    psw = vdis_psw();
    /*
     * MCR�Ŋ������ɂ��Ă��܂�
     *    ��   �����R���g���[���Ɉˑ����Ȃ���CTS�����o����
     */
    sci_out(SIO_MCR, SIO[CH].cmd[2] |= SIO_OUT2);

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

    /* RTS-CTS�t���[����̉��� */
#ifndef NOFLW

    if (SIO[CH].flag & TSF_RTS)
        rts_on();

    /* DTR-CTS�t���[����̉��� */

    if (SIO[CH].flag & TSF_DTR)
        dtr_on();
#endif

    /* XON/OFF�t���[����̉��� */

#ifndef NOXON
    if (SIO[CH].flag & TSF_TXOFF)
    {   if (tx_chr(XON))
            SIO[CH].flag &= ~TSF_TXOFF;
        else
            SIO[CH].flag |= TSF_TXREQ;
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
    UH cnt;

    /* ���M�o�b�t�@���XOFF��M���Ă��Ȃ��Ȃ璼�ڏo�� */

    if ((cnt = SIO[CH].txcnt) == 0)
    {
  #ifndef NOXON
        if (!(SIO[CH].flag & TSF_RXOFF))
  #endif
            if (tx_chr(c))
                return TRUE;
    }

    /* �o�b�t�@���t�`�F�b�N */

    if (cnt >= TXBUFSZ)
        return FALSE;

    /* �o�b�t�@�������� + 1 */

    SIO[CH].txcnt = cnt + 1;

    /* �o�b�t�@�֊i�[ */

    *SIO[CH].txputp = c;

    /* �i�[�|�C���^���P�i�߂� */

    if (++SIO[CH].txputp >= SIO[CH].txbuf + TXBUFSZ)
        SIO[CH].txputp = SIO[CH].txbuf;

    return TRUE;
}

/*****************************************************************************
* ��M�o�b�t�@�֎�M����/�X�e�[�^�X�i�[�i�����֐��j
*
******************************************************************************/

static void put_rxbuf(void)
{
    int cnt;

    /* �A���u���[�N�Ȃ�i�[���Ȃ� */

    if ((SIO[CH].rxsts & SIO_BD) && (SIO[CH].oldsts & SIO_BD))
        return;

    /* �o�b�t�@���t�`�F�b�N */

    cnt = SIO[CH].rxcnt;
    if (cnt == BUFSZ)
        return;
    if (cnt == BUFSZ - 1)
        SIO[CH].rxsts |= SIO_RxRDY; /* �I�[�o�t���[�� RxRDY �r�b�g�Ɋ��蓖�� */

    /* �o�b�t�@�֊i�[ */
    /* �I�[�������o+1 */

    if ((*SIO[CH].rxputp = SIO[CH].rxchr) == SIO[CH].eot)
        SIO[CH].eotcnt++;
    *(SIO[CH].rxputp + BUFSZ) = SIO[CH].rxsts;

    /* �i�[�|�C���^���P�i�߂� */

    if (++SIO[CH].rxputp >= SIO[CH].rxbuf + BUFSZ)
        SIO[CH].rxputp = SIO[CH].rxbuf;

    /* �o�b�t�@�������� + 1 */

    SIO[CH].rxcnt = ++cnt;

    /* RTS-CTS�t���[���� */

#ifndef NOFLW
    if ((cnt == RTSOFF_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_off();

    /* DTR-CTS�t���[���� */

    else if (SIO[CH].flag & TSF_DTR)
    {   if (cnt == RTSOFF_LEN)
            dtr_off();
    }
#endif

    /* XON/OFF�t���[���� */

#ifndef NOXON
    if ((SIO[CH].flag & (TSF_XON|TSF_TXOFF)) == TSF_XON)
    {   if (cnt == XOFF_LEN)
            SIO[CH].flag |= TSF_TXREQ;  /* �������M������XOFF���M�v���Z�b�g */
    }
#endif
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
    UB  *p;

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
    {   if (tx_chr(XON))                    /* XON���M�ł����� */
            SIO[CH].flag &= ~TSF_TXOFF;     /* XOFF���M�ς݃N���A */
        else                                /* ���M�ł��Ȃ���� */
            SIO[CH].flag |= TSF_TXREQ;      /* ���M�v���Z�b�g */
    }
#endif

    /* RTS�t���[���� */

#ifndef NOFLOW
    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_RTS))
        rts_on();

    /* DTR-CTS�t���[���� */

    if ((cnt == RTSON_LEN) && (SIO[CH].flag & TSF_DTR))
        dtr_on();

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
* �V���A�������݃n���h����M�����i�����֐��j
*
******************************************************************************/

static void rx_int(void)
{
    int sts;
    int chr;
    int tid;

    /* ��M���f�B�łȂ��Ȃ�܂ŌJ��Ԃ� */

    tid = 0;
    while ((sts = sci_in(SIO_LSR)) & SIO_RxRDY)
    {
        /* ��M��������, �X�e�[�^�X�L�� */

        chr = sci_in(SIO_DR);
        SIO[CH].oldsts = SIO[CH].rxsts;     /* �O��̎�M�X�e�[�^�X�L�� */
        SIO[CH].rxsts = (UB)(sts & (SIO_BD|SIO_FE|SIO_OE|SIO_PE));
        SIO[CH].rxchr = (UB)chr;

        /* XON/OFF�t���[����L��̏ꍇ */

#ifndef NOXON
        if (SIO[CH].flag & TSF_XON)
        {
            /* XOFF��M�Ȃ瑗�M���f������ */

            if (chr == XOFF)
            {   SIO[CH].flag |= TSF_RXOFF;
                continue;
            }

            /* XON ��M�Ȃ瑗�M�ĊJ������ */

            if (chr == XON)
            {   SIO[CH].flag &= ~TSF_RXOFF;
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);
                continue;
            }
        }
#endif
        /* ��M�o�b�t�@�֊i�[ */

        put_rxbuf();
        tid = SIO[CH].rxtid;
    }

    /* ��M�҂����� */

    if (tid)
        iwup_tsk(tid);
}

/*****************************************************************************
* �V���A�������݃n���h�����M�����i�����֐��j
*
******************************************************************************/

static void tx_int(void)
{
    UB *p;
    int cnt;
    int tid;

    /* ���M�����݂ƃ��f���X�e�[�^�X�����݋֎~ */

    SIO[CH].cmd[0] &= ~(SIO_TxIE);
#ifdef  INT_IRQ
    if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
        SIO[CH].cmd[0] &= ~SIO_MSIE;
#endif

    sci_in(SIO_MSR);    /* ���f���X�e�[�^�X�����݃��Z�b�g */

    /* ���M���f�B�łȂ��Ȃ�܂ŌJ��Ԃ� */

    if (sci_in(SIO_LSR) & SIO_TxRDY)
    {   cnt = 0;
        /* �t���[���䂠��Ȃ� CTS �`�F�b�N */
        /* CTS OFF �Ȃ� ���f���X�e�[�^�X�����݋��� */
#ifndef NOFLW
        if (SIO[CH].flag & (TSF_RTS|TSF_DTR))
        {   if (!(sci_in(SIO_MSR) & SIO_CTS))
            {   SIO[CH].cmd[0] |= SIO_MSIE;
                return;
            }
        }
#endif
        /* ��M������˗����ꂽXON/XOFF�̑��M */
#ifndef NOXON
        if (SIO[CH].flag & TSF_TXREQ)
        {
            if (SIO[CH].flag & TSF_TXOFF)
            {   sci_out(SIO_DR, SIO[CH].txchr = XON);
                SIO[CH].flag &= ~TSF_TXOFF;
            }
            else
            {   sci_out(SIO_DR, SIO[CH].txchr = XOFF);
                SIO[CH].flag |= TSF_TXOFF;
            }
            SIO[CH].flag &= ~TSF_TXREQ;
            cnt++;
        }

        /* XOFF��M���Ă����瑗�M�s�� */

        if (SIO[CH].flag & TSF_RXOFF)
            return;
#endif

        for(; cnt < 16; cnt++)
        {
            /* ���M�o�b�t�@��Ȃ瑗�M�s�� */

            if (--SIO[CH].txcnt == (UH)-1)
            {
                SIO[CH].txcnt = 0;
                if (tid = SIO[CH].tetid)
                    iwup_tsk(tid);          /* ���M�I���҂����� */
                if (tid = SIO[CH].txtid)
                    iwup_tsk(tid);          /* ���M�҂����� */
                return;
            }

            /* �P�������M */

            p = SIO[CH].txgetp;
            sci_out(UART_THR, *p);

            /* �擾�|�C���^���P�i�߂� */

            if (++p >= SIO[CH].txbuf + TXBUFSZ)
                p = SIO[CH].txbuf;
            SIO[CH].txgetp = p;
        }

        /* ���M�҂����� */

        if (tid = SIO[CH].txtid)
            iwup_tsk(tid);
    }

    /* ���M�����݋��� */

    SIO[CH].cmd[0] |= SIO_TxIE;     /* ��ŋ������   */

}

/*****************************************************************************
* �V���A�������݃n���h���{�́i�����֐��j
*
******************************************************************************/

static void int_sio_body(void)
{
    rx_int();
    tx_int();
    sci_out(SIO_IER, SIO[CH].cmd[0]);
}

/*****************************************************************************
* �V���A�������݃n���h��
*
******************************************************************************/

#if (CH==0)
INTHDR int_sio0(void)
#elif (CH==1)
INTHDR int_sio1(void)
#elif (CH==2)
INTHDR int_sio2(void)
#elif (CH==3)
INTHDR int_sio3(void)
#endif
{
    ent_int();              /* �����݊J�n */
    int_sio_body();         /* �����݃n���h���{�� */
    ret_int();              /* �����ݏI�� */
}

/*****************************************************************************
* �������p�����[�^��́i�����֐��j
*
******************************************************************************/

static BOOL set_param(const B *s, UB *mode, UW *baud)
{
    B c;
    UW b;
    UB md;

    /* ���[�h���� */

    md = 0;
    if      (strstr((const char *)s, (char *)"PE") != NULL)   md |= SIO_EVEN;
    else if (strstr((const char *)s, (char *)"PO") != NULL)   md |= SIO_ODD;
    else                                md |= SIO_NONE;
    if      (strstr((const char *)s, (char *)"S2") != NULL)   md |= SIO_S2;
    else                                md |= SIO_S1;

    /* �f�[�^������ */

    if      (strstr((const char *)s, (char *)"B7") != NULL)   md |= SIO_B7;
    else                                md |= SIO_B8;
    *mode = md;

    /* �t���[����L������ */

    if (strstr((const char *)s, (char *)"XON") != NULL)   SIO[CH].flag |= TSF_XON;
    if (strstr((const char *)s, (char *)"DTR") != NULL)   SIO[CH].flag |= TSF_DTR;
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
  #if (CH==0)
    static const T_DINH dinh_sio0 = { TA_HLNG, int_sio0 ,IP };
  #elif (CH==1)
    static const T_DINH dinh_sio1 = { TA_HLNG, int_sio1 ,IP };
  #elif (CH==2)
    static const T_DINH dinh_sio2 = { TA_HLNG, int_sio2 ,IP };
  #elif (CH==3)
    static const T_DINH dinh_sio3 = { TA_HLNG, int_sio3 ,IP };
  #endif
#else
#ifdef V3
  #if (CH==0)
    static const T_DINT dint_sio0 = { TA_HLNG, int_sio0 ,IP };
  #elif (CH==1)
    static const T_DINT dint_sio1 = { TA_HLNG, int_sio1 ,IP };
  #elif (CH==2)
    static const T_DINT dint_sio2 = { TA_HLNG, int_sio2 ,IP };
  #elif (CH==3)
    static const T_DINT dint_sio3 = { TA_HLNG, int_sio3 ,IP };
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
    UB mode;
    UW baud;

    /* ����u���b�N������ */

    init_buf();

    /* �p�����[�^��� */

    if (!set_param(param, &mode, &baud))
        return E_PAR;

    /* �f�o�C�X������ */

    init_sio(mode, baud);

    /* �����݃n���h���̒�` */

    if (!(SIO[CH].flag & TSF_INIT))
    {
#ifdef V4
 #if (CH==0)
        def_inh(INT_SIO, &dinh_sio0);
 #elif (CH==1)
        def_inh(INT_SIO, &dinh_sio1);
 #elif (CH==2)
        def_inh(INT_SIO, &dinh_sio2);
 #elif (CH==3)
        def_inh(INT_SIO, &dinh_sio3);
 #endif
#else
#ifdef V3
 #if (CH==0)
        def_int(INT_SIO, &dint_sio0);
 #elif (CH==1)
        def_int(INT_SIO, &dint_sio1);
 #elif (CH==2)
        def_int(INT_SIO, &dint_sio2);
 #elif (CH==3)
        def_int(INT_SIO, &dint_sio3);
 #endif
#else
 #if (CH==0)
        def_int(INT_SIO, int_sio0);
 #elif (CH==1)
        def_int(INT_SIO, int_sio1);
 #elif (CH==2)
        def_int(INT_SIO, int_sio2);
 #elif (CH==3)
        def_int(INT_SIO, int_sio3);
 #endif
#endif
#endif
    }
    SIO[CH].flag |= TSF_INIT;       /* �������ς݃Z�b�g */

    /* �V���A�������݋��� */

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
    sci_out(SIO_IER, 0x00);
    sci_out(SIO_MCR, 0x00);             /* ����M�֎~, RTS/DTR OFF */

#if (defined(INT_IRQ))
    rts_off();                          /* RTS OFF */
#endif
    /* �����ݒ�`�������� */

#ifdef V4   /* NORTI4 */
    def_inh(INT_SIO, (T_DINH *)NADR);
#else
#ifdef V3   /* NORTI3 */
    def_int(INT_SIO, (T_DINT *)NADR);
#else       /* NORTi2 */
    def_int(INT_SIO, (INTHDRP)NADR);
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
    ER err;
    INT sts;

    for (;;)
    {
        chg_ims(IP);                /* �V���A�������݋֎~ */

        /* ��M�o�b�t�@����P�������� */

        sts = get_rxbuf(c);

        if (sts != -1)              /* ��M�����������ꍇ */
        {   chg_ims(0);             /* �V���A�������݋��� */

            /* ��M�G���[���� */

            if (sts & (SIO_RxRDY|SIO_BD|SIO_FE|SIO_OE|SIO_PE))
            {   if (sts & SIO_RxRDY)
                    err = EV_SIOOVF;
                else if (sts & SIO_OE)
                    err = EV_SIOORN;
                else if (sts & SIO_BD)
                    err = EV_SIOBRK;
                else if (sts & SIO_FE)
                    err = EV_SIOFRM;
                else /* SIO_PE */
                    err = EV_SIOPTY;
            }
            else
                err = E_OK;
            return err;
        }

        /* ��M�����ݑ҂� */

        SIO[CH].rxtid = vget_tid();
        chg_ims(0);                 /* �V���A�������݋��� */
        err = wai_tsk(tmout);
        SIO[CH].rxtid = 0;
        vcan_wup();                 /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
        if (err)
            return err;             /* �^�C���A�E�g�I�� */
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
    ER err;

    for (;;)
    {
        chg_ims(IP);                /* �V���A�������݋֎~ */

        /* ���M�o�b�t�@�ւP�����i�[ */

        if (put_txbuf(c))           /* ���M�ł����ꍇ */
        {   chg_ims(0);             /* �V���A�������݋��� */
            return E_OK;            /* ����I�� */
        }

        /* ���M�����ݑ҂� */

        SIO[CH].txtid = vget_tid();
        chg_ims(0);                 /* �V���A�������݋��� */
        err = wai_tsk(tmout);
        SIO[CH].txtid = 0;
        vcan_wup();                 /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
        if (err)
            return err;             /* �^�C���A�E�g�I�� */
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

    chg_ims(IP);                    /* �V���A�������݋֎~ */

    /* �o�b�t�@�N���A */

    if (fncd & TSIO_RXCLR)
        clr_rxbuf();
    if (fncd & TSIO_TXCLR)
        clr_txbuf();

    /* �R�}���h�o�� */

    if (fncd & (TSIO_RXE|TSIO_RXD)) /* TSIO_TXE,TSIO_TXD ���T�|�[�g�I */
    {
        c = SIO[CH].cmd[0];
        if (fncd & TSIO_RXE)        c |= SIO_RxIE;
        if (fncd & TSIO_RXD)        c &= ~SIO_RxIE;
        sci_out(SIO_IER, SIO[CH].cmd[0] = c);
    }

    if (fncd & (TSIO_SBON|TSIO_SBOFF))
    {
        c = SIO[CH].cmd[1];
        if (fncd & TSIO_SBON)       c |= SIO_SB;
        else                        c &= ~SIO_SB;
        sci_out(SIO_LCR, SIO[CH].cmd[1] = c);
    }

    if (fncd & (TSIO_RTSON|TSIO_RTSOFF|TSIO_DTRON|TSIO_DTROFF))
    {
        c = SIO[CH].cmd[2];
        if (fncd & TSIO_RTSON)      c |=  SIO_RTS;
        if (fncd & TSIO_RTSOFF)     c &= ~SIO_RTS;
        if (fncd & TSIO_DTRON)      c |=  SIO_DTR;
        if (fncd & TSIO_DTROFF)     c &= ~SIO_DTR;
        sci_out(SIO_MCR, SIO[CH].cmd[2] = c);
    }

    chg_ims(0);                     /* �V���A�������݋��� */
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
    UB stat;

    chg_ims(IP);                        /* �V���A�������݋֎~ */

    stat = get_stat();
    if (SIO[CH].txcnt != 0)
        stat &= ~TSIO_TXEMP;

    pk_sios->siostat = stat;
    pk_sios->rxlen   = SIO[CH].rxcnt;
    pk_sios->frbufsz = (UH)(TXBUFSZ - SIO[CH].txcnt);
    pk_sios->eotcnt  = SIO[CH].eotcnt;

    chg_ims(0);                         /* �V���A�������݋��� */

    return E_OK;
}

/*****************************************************************************
* �V���A�����M�o�b�t�@��t���b�V��
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
    ER err;

    for (;;)
    {
        chg_ims(IP);                    /* �V���A�������݋֎~ */

        /* ���M�o�b�t�@��̏ꍇ */

        if (SIO[CH].txcnt == 0)
        {
            chg_ims(0);                 /* �V���A�������݋��� */
            return E_OK;                /* ����I�� */
        }

        /* ���M�I�������ݑ҂� */

        SIO[CH].tetid = vget_tid();
        chg_ims(0);                     /* �V���A�������݋��� */
        err = wai_tsk(tmout);
        SIO[CH].tetid = 0;
        vcan_wup();                     /* ������ iwup_tsk ���ꂽ�ꍇ�̑΍� */
        if (err)
            return err;                 /* �^�C���A�E�g�I�� */
    }
}

/* end */
