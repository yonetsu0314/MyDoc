/******************************************************************************
* NORTi3 Network LAN Control Driver (SH7615 peripheral)                       *
*                                                                             *
*  File name : sh7615e.c                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 13/Apr/2000 �쐬                                                      MiSPO *
* 21/Oct/2000 EtherC/E-DMAC�̃\�t�greset��̑҂��ǉ�                 OK(MiSPO)*
* 25/Apr/2001 �C�x���g�t���O�̎g�p����߂�^���荞�ݕp�x�}��         OK       *
* 14/May/2002 Modified FIFO config for SH7616                        OK       *
******************************************************************************/

/*

1�t���[��/�}���`�o�b�t�@

          �{�T���v���́ASH7615 Ether�R���g���[��������1�t���[��/1�o�b�t�@
          �@�\�ɑΉ����Ă���܂��B1�t���[��/�}���`�o�b�t�@�@�\���g�p����
          �ꍇ�ɂ́A�J�X�^�}�C�Y���K�v�ł��B

�������[�v�o�b�N�̐ݒ�

          SH7615���� Ether�R���g���[���܂�Ԃ��̐ݒ��L���ɂ���ꍇ�ɂ�
          LOOP_BACK �}�N�����`���ăR���p�C�����Ă��������B

          (��) shc <option> -def=LOOP_BACK sh7615e.c
                            ~~~~~~~~~~~~~~
          (��) gcc <option> -DLOOP_BACK sh7615e.c
                            ~~~~~~~~~~~
          (��) ccsh <options> -DLOOP_BACK sh7615e.c
                              ~~~~~~~~~~~

Auto Negotiation�̐ݒ�

          Auto Negotiation(PHY������)�@�\��L���ɂ���ꍇ�ɂ́AAUTONEGO �}�N��
          ���`���ăR���p�C�����Ă��������BAUTONEGO �}�N�����`�����ꍇ�ɂ�
          SPEED(10/100Mbps)�̑I���͖����ł��B�f�t�H���g�ł� Auto Negotiation
          �@�\�͖����ł��B�S�Q�d�^���Q�d�̎����ݒ�͖{�T���v���ł̓T�|�[�g����
          �Ă��܂���B

          (��) shc <option> -def=AUTONEGO sh7615e.c
                            ~~~~~~~~~~~~~
          (��) gcc <option> -DAUTONEGO sh7615e.c
                            ~~~~~~~~~~
          (��) ccsh <options> -DAUTONEGO sh7615e.c
                              ~~~~~~~~~~

�X�s�[�h�̐ݒ�

          Auto Negotiation�@�\���g��Ȃ��ŌŒ胂�[�h�Ŏg�p����ꍇ�ɂ� SPEED
          �}�N���œ���X�s�[�h��ύX�ł��܂��B�f�t�H���g�ł� 10Mbps �ɂȂ��
          ���BSPEED �}�N���ɂ́A10 �܂��� 100 �̒l��ݒ肵�Ă��������B
          ����ȊO�̐ݒ�̓G���[�ƂȂ�܂��B�܂� AUTONEGO �}�N�����`�����ꍇ
          �ɂ� SPEED(10/100Mbps)�̑I���͖����ł��B
          ���̐ݒ�ł� 100Mbps �ɂȂ�܂��B

          (��) shc <option> -def=SPEED=100 sh7615e.c
                            ~~~~~~~~~~~~~~
          (��) gcc <option> -DSPEED=100 sh7615e.c
                            ~~~~~~~~~~~
          (��) ccsh <options> -DSPEED=100 sh7615e.c
                              ~~~~~~~~~~~

�S�Q�d�^���Q�d�̐ݒ�

          �S�Q�d��ݒ肷��ꍇ�ɂ́AFULL �}�N�����`���ăR���p�C�����Ă�����
          ���B�f�t�H���g�ł� ���Q�d�ɐݒ肳��܂��BAuto Negotiation�̑S�Q�d�^
          ���Q�d�I���͌Œ肳��܂��B

          (��) shc <option> -def=FULL sh7615e.c
                            ~~~~~~~~~
          (��) gcc <option> -DFULL sh7615e.c
                            ~~~~~~
          (��) ccsh <options> -DFULL sh7615e.c
                              ~~~~~~

*/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "dp83843.h"
#include "sh7615e.h"
#include "sh7615.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

#ifndef LOOP_BACK
#define LOOP_BACK 0   /* EtherC�̋������[�v�o�b�N�֎~ */
#else
#undef LOOP_BACK
#define LOOP_BACK 1   /* EtherC�̋������[�v�o�b�N���� */
#endif

#ifndef AUTONEGO
#define AUTONEGO  0   /* �f�t�H���g�ł́AAuto Negotiation�͖��� */
#else
#undef AUTONEGO
#define AUTONEGO  1
#endif

#ifndef SPEED
#define SPEED     10  /* �f�t�H���g�̃X�s�[�h�́A10Mbps */
#else
#if SPEED != 10 && SPEED != 100
#error NO SUPPORTED SPEED
#endif
#endif

#ifndef FULL
#define FULL      0   /* �f�t�H���g��DUPLEX�́A���Q�d(HALF)�ł� */
#else
#undef FULL
#define FULL      1
#endif

/* ���[�U�[��`�̊֐�/�}�N�� */

#ifndef ALIGN
#define ALIGN(p,n)  ((((UW)(p)) + (((UW)(n)) - 1)) & (~(((UW)(n)) - 1)))
                   /* �A���C�������g�����}�N��
                    *  p: �v�Z�O�A�h���X
                    *  n: ���E�l(2�ׂ̂���ł��邱��)
                    */
#endif

#define ETHER_TX_BUF_NUM    16    /* Tx buffer number */
#define ETHER_RX_BUF_NUM    16    /* Rx buffer number */
#define ETHER_TX_BUF_SIZ    0x600 /* =1520+@=1536 Tx buffer size(boundary is 16 & >1518) */
#define ETHER_RX_BUF_SIZ    0x600 /* =1520+@=1536 Rx buffer size(boundary is 16 & >1518) */

/* ����M���荞�݃X�e�[�^�X��` */

#define EV_ETH_RX (EESIPR_FRIP|EESIPR_RDEIP |EESIPR_RFOFIP|EESIPR_RMAFIP)
#define EV_ETH_TX (EESIPR_TCIP|EESIPR_TFUFIP|EESIPR_ITFIP |EESIPR_TROIP)

ER lan_def_int(void);       /* �����ݐݒ� */

/* �O���[�o���ϐ� */

UB lan_loopback_test=LOOP_BACK; /* ���[�v�o�b�N�e�X�g */
UB lan_inter_nouse=0;           /* �����ݎg��Ȃ� for DEBUG */

/* �����ϐ� */

static ETHER_BD  RxBD[ETHER_RX_BUF_NUM+1];  /* ��MBD(�A���C�����g�̂��� +1) */
static ETHER_BD  TxBD[ETHER_TX_BUF_NUM+1];  /* ���MBD(�A���C�����g�̂��� +1) */
static ETHER_BD *pRxBD;                     /* ��MBD�J�����g�A�h���X for handling */
static ETHER_BD *pRxBD2;                    /* ��MBD�J�����g�A�h���X for API */
static ETHER_BD *pTxBD;                     /* ���MBD�J�����g�A�h���X */

                                          /* 16byte���E�ɍ��킹�邽�� +16 */
static UB  RxBuf[ETHER_RX_BUF_NUM * ETHER_RX_BUF_SIZ +16];  /* ��Mbuffer */
static UB  TxBuf[ETHER_TX_BUF_NUM * ETHER_TX_BUF_SIZ +16];  /* ���Mbuffer */
static UB *pRxBuf;                          /* ��MBuffer�J�����g�A�h���X for working */
static UB *pTxBuf;                          /* ���MBuffer�J�����g�A�h���X for working */

static ID tx_wtskid;                        /* ���M�҂��^�X�NID */
static ID rx_wtskid;                        /* ��M�҂��^�X�NID */

/* ���s�}�N�� */

#define DI_RXINT()  /* ��M���荞�݋֎~ */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_clrl(EDMAC_EESIPR, EV_ETH_RX);\
    vset_psw(psw);\
}


#define EI_RXINT()  /* ��M���荞�݋��� */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_setl(EDMAC_EESIPR, EV_ETH_RX);\
    vset_psw(psw);\
}


#define DI_TXINT()  /* ���M���荞�݋֎~ */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_clrl(EDMAC_EESIPR, EV_ETH_TX);\
    vset_psw(psw);\
}

#define EI_TXINT()  /* ���M���荞�݋��� */\
if (!lan_inter_nouse){\
    UINT psw;\
    psw = vdis_psw();\
    sfr_setl(EDMAC_EESIPR, EV_ETH_TX);\
    vset_psw(psw);\
}

/* BD �I�[�o�t���[�Ŏ�M��~�̏ꍇ�̍ăX�^�[�g */
#define RX_RESTART() sfr_outl(EDMAC_EDRRR, EDRRR_RR)

/*****************************************************************************
* �o�b�t�@�f�B�X�N���v�^�̏�����
*
******************************************************************************/
static void BD_init(void)
{
    int i;
    ETHER_BD *current;

    /* BD & buffer �̈�́Acache through ���g�� */

    pTxBD  = (ETHER_BD *)(ALIGN(TxBD, sizeof(ETHER_BD)) + 0x20000000L);
    pRxBD  = (ETHER_BD *)(ALIGN(RxBD, sizeof(ETHER_BD)) + 0x20000000L);
    pTxBuf = (UB *)(ALIGN(TxBuf, 16) + 0x20000000L);
    pRxBuf = (UB *)(ALIGN(RxBuf, 16) + 0x20000000L);

    current = pTxBD;
    for (i = 0; i < ETHER_TX_BUF_NUM; i++, current++){
        current->status  = 0;
        current->bufsize = 0;
        current->rxsize  = 0;                    /* reserved area */
        current->buf_p   = pTxBuf;
        pTxBuf += ETHER_TX_BUF_SIZ;
        current->next    = current + 1;  /* LINK to next BD */
    }
    current--;
    current->status |= DESC_DL;  /* LAST descripter */
    current->next    = pTxBD;    /* LINK to 1st BD */

    current = pRxBD;
    for (i = 0; i < ETHER_RX_BUF_NUM; i++, current++){
        current->status  = DESC_ACT;
        current->bufsize = ETHER_RX_BUF_SIZ;
        current->rxsize  = 0;
        current->buf_p   = pRxBuf;
        pRxBuf += ETHER_RX_BUF_SIZ;
        current->next    = current + 1;  /* LINK to next BD */
    }
    current--;
    current->status |= DESC_DL;  /* LAST descripter */
    current->next    = pRxBD;    /* LINK to 1st BD */

    pTxBuf = NULL;
    pRxBD2 = NULL;
    pRxBuf = NULL;
}

/*****************************************************************************
* LAN �R���g���[��������
*
******************************************************************************/

ER lan_ini(UB *macaddr)
{
    ER ercd;
    UW *mach, *macl;

    /* PHY�̏����� */

#if AUTONEGO == 1
 #if FULL == 1
    PHY_init(TRUE, AFD10|AFD100);
            /* AutoNego,10M/Full or 100M/Full */
 #else
    PHY_init(TRUE, AHD10|AHD100);
            /* AutoNego,10M/Half or 100M/Half */
 #endif
#else
  #if FULL == 1
   #if SPEED == 10
    PHY_init(FALSE,FD10);             /* Fixed 10M/Full */
   #else
    PHY_init(FALSE,FD100);            /* Fixed 100M/Full */
   #endif
  #else
   #if SPEED == 10
    PHY_init(FALSE,HD10);             /* Fixed 10M/Half */
   #else
    PHY_init(FALSE,HD100);            /* Fixed 100M/Half */
   #endif
 #endif
#endif

    /* �o�b�t�@�f�B�X�N���v�^�̏����� */

    BD_init();

    /* E-DMAC ������ */

    sfr_outl(EDMAC_EDMR, EDMR_SWR);    /* reset EtherC/E-DMAC */
    dly_tsk(1);                        /* reset���A�܂�(min. 16system clock) */
    sfr_outl(EDMAC_TDLAR, (UW)pTxBD);  /* TxBD �擪�Ԓn�ݒ� */
    sfr_outl(EDMAC_RDLAR, (UW)pRxBD);  /* RxBD �擪�Ԓn�ݒ� */
    sfr_outl(EDMAC_EDMR, EDMR_DL16);

    sfr_outl(EDMAC_EESR, (EESR_ECI|EESR_TC|EESR_TFUF|EESR_ITF |EESR_TRO |
                                   EESR_FR|EESR_RDE |EESR_RFOF|EESR_RMAF)); /* clear */
    if (!lan_inter_nouse)
        sfr_outl(EDMAC_EESIPR, EESIPR_ECIP); /* ����M�ȊO�̊��荞�ݐݒ� */

    sfr_outl(EDMAC_TRSCER, 0);        /* status bit all copy */
    sfr_outl(EDMAC_TFTR, 0);          /* TxFIFO thresholf 0(store&foward) */
#ifdef SH7616
    /* for SH7616 */
    sfr_outl(EDMAC_FDR, FDR_TFD2048|FDR_RFD2048);   /* FIFO depth = 2048bytes */
#else
    /* for SH7615 */
    sfr_outl(EDMAC_FDR, FDR_TFD512|FDR_RFD512);     /* FIFO depth = 512bytes */
#endif
    sfr_outl(EDMAC_RCR, RCR_RNC);     /* multiple Recieve mode */
    sfr_outl(EDMAC_EDOCR, 0);         /* normal setup */

    /* Ether-C ������ */

    mach = (UW *)&macaddr[0];
    macl = (UW *)&macaddr[4];

    sfr_outl(ETHERC_RFLR, 1518);    /* RxMax = 1518 */

    sfr_outl(ETHERC_ECSR, ECSR_LCHNG|ECSR_MPR|ECSR_ICD); /* clear */
    if (!lan_inter_nouse)
        sfr_outl(ETHERC_ECSIPR, ECSIPR_LCHNGIP |
                                ECSIPR_MPRIP   |
                                ECSIPR_ICDIP); /* �����N��Ԍn�̊��荞�ݐݒ� */

    sfr_outl(ETHERC_MAHR, *mach);      /* MAC address High */
    sfr_outl(ETHERC_MALR, *macl>>16);  /* MAC address Low */

    /* �����ݒ�` */

    if (!lan_inter_nouse)
        lan_def_int();

    /* Tx/Rx enable */
    if (!lan_loopback_test)  /* NORMAL mode */
#if FULL == 0
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE);          /* Tx/Rx enable(DUPX=HALF) */
#else
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE|ECMR_DM);  /* Tx/Rx enable(DUPX=FULL) */
#endif
    else                     /* FORCE LOOP-BACK mode */
        sfr_outl(ETHERC_ECMR, ECMR_TE|ECMR_RE|ECMR_DM|ECMR_ILB);

    sfr_outl(EDMAC_EDRRR, EDRRR_RR);             /* E-DMAC Rx enable */

    return E_OK;
}

/*****************************************************************************
* ����M�����݃n���h��
*
******************************************************************************/

void lan_intr(void)
{
    ER ercd;
    UW eesr, ecsr;

    eesr = sfr_inl(EDMAC_EESR);
    sfr_outl(EDMAC_EESR, eesr);  /* clear */

    /* LINK status �ω������o */

    if (eesr & EESR_ECI){
        ecsr = sfr_inl(ETHERC_ECSR);
        sfr_outl(ETHERC_ECSR, ecsr);  /* clear */

        if (ecsr & ECSR_LCHNG){
            if (sfr_inl(ETHERC_PSR) & PSR_LMON){  /* LINK�m�����(Hard wire from PHY) */
            }
        }
        if (ecsr & ECSR_MPR){     /* Received Magic Packet */
        }
        if (ecsr & ECSR_ICD){     /* Detected Illegal Carrier */
        }
    }

    /* FR (Rx Complete) */

    if (eesr & (EESR_FR|EESR_RDE|EESR_RFOF|EESR_RMAF)){
        if (rx_wtskid){
            wup_tsk(rx_wtskid);         /* IP��M�^�X�N���N�� */
            rx_wtskid = FALSE;
            sfr_clrl(EDMAC_EESIPR, EV_ETH_RX);
        }
    }

    /* TC (Tx Complete) */

    if (eesr & (EESR_TC|EESR_TFUF|EESR_ITF|EESR_TRO)){
        if (tx_wtskid){
            wup_tsk(tx_wtskid);         /* IP���M�^�X�N���N�� */
            tx_wtskid = FALSE;
            sfr_clrl(EDMAC_EESIPR, EV_ETH_TX);
        }
    }
}

/*****************************************************************************
* ��M�����ݑ҂�
*
******************************************************************************/

ER lan_wai_rcv(TMO tmout)
{
    ER ercd = E_OK;

    for (;;){
        if (!(pRxBD->status & DESC_ACT)){

            /* 1�t���[��/1�o�b�t�@ �Œ�Ƃ��Ď��� */

            if ((!(pRxBD->status & DESC_FE) || (pRxBD->status & 0x29f) == EESR_RMAF)
                                            &&                     /* no Rx error */
                (pRxBD->status & (DESC_FP1|DESC_FP0)) == (DESC_FP1|DESC_FP0)){
                                                           /* 1�t���[��/1�o�b�t�@ */

                pRxBD2 = pRxBD;        /* set current RxBD for working */
                pRxBuf = pRxBD->buf_p; /* set current buffer for working */
                pRxBD  = pRxBD->next;  /* set next BD to current */
                return E_OK;
            }
            pRxBD->rxsize = 0;
            pRxBD->status = (pRxBD->status & DESC_DL)|DESC_ACT;
            pRxBD = pRxBD->next;
            RX_RESTART();
            continue;
        }

        /* wait for Rx by polling */

        if (lan_inter_nouse){
            if (tmout != TMO_FEVR && tmout-- == 0)
                return E_TMOUT;
            ercd = tslp_tsk(1);
            if (ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
        }

        /* wait for Interrupt events */

        else{
            rx_wtskid = vget_tid();
            vcan_wup();
            EI_RXINT();                     /* ��M�����݋��� */
            ercd = tslp_tsk(tmout);         /* ��M�����݂���̋N���҂� */
            DI_RXINT();                     /* ��M�����݋֎~ */
            rx_wtskid = FALSE;

            if (ercd != E_OK && ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
            if (ercd == E_TMOUT)
                break;
        }
    }
    return ercd;
}

/*****************************************************************************
* ���M�����ݑ҂�
*
******************************************************************************/

ER lan_wai_snd(TMO tmout)
{
    ER ercd = E_OK;

    for (;;){
        if (!(pTxBD->status & DESC_ACT)){  /* Tx OK */
            pTxBD->bufsize = 0;
            pTxBuf = pTxBD->buf_p;
            return E_OK;
        }

        /* wait for Tx READY by polling */

        if (lan_inter_nouse){
            if (tmout != TMO_FEVR && tmout-- == 0)
                return E_TMOUT;
            ercd = tslp_tsk(1);
            if (ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
        }

        /* wait for Interrupt events */

        else{
            tx_wtskid = vget_tid();
            vcan_wup();
            EI_TXINT();                     /* ���M�����݋��� */
            ercd = tslp_tsk(tmout);         /* ���M�����݂���̋N���҂� */
            DI_TXINT();                     /* ���M�����݋֎~ */
            tx_wtskid = FALSE;

            if (ercd != E_OK && ercd != E_TMOUT)  /* by forced wake up */
                return -1;        /* as error */
            if (ercd == E_TMOUT)
                break;
        }
    }
    return ercd;
}

/*****************************************************************************
* ��M�p�P�b�g���𓾂�
*
******************************************************************************/

ER lan_get_len(UH *len)
{
    if (!pRxBD2)
        *len = 0;
    else
        *len = pRxBD2->rxsize;
    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݏo��
*
* buf   �ǂݏo�����߂̃o�b�t�@
* len   �ǂݏo���o�C�g��
******************************************************************************/

ER lan_get_pkt(void *buf, int len)
{
    if (!pRxBuf)  /* no available RxBD */
        return -1;

    memcpy(buf, pRxBuf, len);
    pRxBuf += len;
    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݏo���I��
*
******************************************************************************/

ER lan_get_end(void)
{
    if (pRxBD2){
        pRxBD2->rxsize = 0;
        pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
        RX_RESTART();
    }
    pRxBD2 = NULL;
    pRxBuf = NULL;
    return E_OK;
}

/*****************************************************************************
* ��M�p�P�b�g�ǂݔ�΂�
*
* len   ��M�p�P�b�g�̖��ǃo�C�g��
******************************************************************************/

ER lan_skp_pkt(int len)
{
    int remain;

    if (!pRxBD2)  /* no available RxBD */
        return E_OK;

    remain = (int)((UW)pRxBuf - (UW)pRxBD2->buf_p);  /* �ǂݏo������ */
    remain = (int)((UINT)pRxBD2->rxsize) - remain;   /* �c�萔 */
    if (remain > 0){
        if (remain <= len){
            pRxBD2->rxsize = 0;
            pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
            RX_RESTART();
            pRxBD2 = NULL;
            pRxBuf = NULL;
        }
        else{
            pRxBuf += len;
        }
    }
    else{
        pRxBD2->rxsize = 0;
        pRxBD2->status = (pRxBD2->status & DESC_DL)|DESC_ACT;
        RX_RESTART();
        pRxBD2 = NULL;
        pRxBuf = NULL;
    }
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g����ݒ�
*
******************************************************************************/

ER lan_set_len(int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    pTxBD->bufsize = len;
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g��������
*
* data      �������ރf�[�^
* len       �f�[�^�̃o�C�g���i> 0�j
******************************************************************************/

ER lan_put_pkt(const void *data, int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    memcpy(pTxBuf, data, len);
    pTxBuf += len;
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g��60�o�C�g�����̏ꍇ�̃_�~�[��������
*
* len       �_�~�[�f�[�^�̃o�C�g���i> 0�j
******************************************************************************/

ER lan_put_dmy(int len)
{
    if (!pTxBuf)  /* no pass sequence [this value be set by "lan_wai_snd()" */
        return -1;

    memset(pTxBuf, 0, len);
    pTxBuf += len;
    return E_OK;
}

/*****************************************************************************
* ���M�p�P�b�g�������ݏI��
*
******************************************************************************/

ER lan_put_end(void)
{
    pTxBD->status = ((pTxBD->status & DESC_DL)|DESC_FP1|DESC_FP0|DESC_ACT);
                                       /* changed active of Fin. BD */
    pTxBD = pTxBD->next;               /* set next BD to current */
    sfr_outl(EDMAC_EDTRR, EDTRR_TR);   /* be transferrd */
    return E_OK;
}

/* end */
