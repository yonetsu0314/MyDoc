/******************************************************************************
* �m�n�q�s���Ή� �V���A�����o�͊֐��w�b�_                                     *
*                                                                             *
*  File name : nosio.h                                                        *
*  Copyright (c) 1993-2000, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
* 93-04-14 �쐬                                                               *
* 93-05-16 Ver1.13��                                                          *
* 94-01-05 Ver1.14                                                            *
* 95-06-15 Ver1.20                                                            *
* 95-08-14 T_SIOFP �̒�`�� nosio.c �ֈڂ�                                    *
* 96-05-20 Ver1.21                                                            *
* 96-05-29 Ver1.30                                                            *
* 00-04-07 NORTi4�Ή� pascal��NORTiAPI�ɕύX                                  *
******************************************************************************/

#ifndef NOSIO_H
#define NOSIO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LSI_C80
#ifndef const
#define const   /**/
#endif
#endif

/* �G���[�R�[�h */

#define EV_SIOINI   1       /* �������� */
#define EV_SIOOVF   2       /* ��M�o�b�t�@�I�[�o�t���[ */
#define EV_SIOPTY   3       /* �p���e�B�G���[ */
#define EV_SIOORN   4       /* �I�[�o�[�����G���[ */
#define EV_SIOFRM   5       /* �t���[�~���O�G���[ */
#define EV_SIOBRK   6       /* �u���[�N���o */

/* �@�\�R�[�h */

#define TSIO_RXE    0x01    /* ��M�C�l�[�u�� */
#define TSIO_RXD    0x02    /* ��M�f�B�Z�[�u�� */
#define TSIO_TXE    0x04    /* ���M�C�l�[�u�� */
#define TSIO_TXD    0x08    /* ���M�f�B�Z�[�u�� */
#define TSIO_RTSON  0x10    /* RTS�M��ON */
#define TSIO_RTSOFF 0x20    /* RTS�M��OFF */
#define TSIO_DTRON  0x40    /* DTR�M��ON */
#define TSIO_DTROFF 0x80    /* DTR�M��OFF */
#define TSIO_RXCLR  0x0100  /* ��M�o�b�t�@�N���A */
#define TSIO_TXCLR  0x0200  /* ���M�o�b�t�@�N���A */
#define TSIO_SBON   0x0400  /* �u���[�N���MON */
#define TSIO_SBOFF  0x0800  /* �u���[�N���MOFF */

/* �V���A�����o�͏�ԃp�P�b�g */

typedef struct t_sios
{
    UB siostat;             /* �V���A�����o�̓X�e�[�^�X */

#define TSIO_CD     0x01    /* ��M�L�����A���o */
#define TSIO_CTS    0x02    /* CTS�M��ON(1)/OFF(0) */
#define TSIO_TXEMP  0x04    /* ���M�o�b�t�@�� */
#define TSIO_PE     0x08    /* �p���e�B�G���[ */
#define TSIO_OE     0x10    /* �I�[�o�����G���[ */
#define TSIO_FE     0x20    /* �t���[�~���O�G���[ */
#define TSIO_BD     0x40    /* �u���[�N��Ԍ��o */
#define TSIO_DSR    0x80    /* DSR�M��ON(1)/OFF(0) */

    UB rxchr;               /* ��M�o�b�t�@�̐擪�̕����i���g�p�j*/
    UH rxlen;               /* ��M�o�b�t�@�̃f�[�^�� */
    UH frbufsz;             /* ���M�o�b�t�@�̋󂫃T�C�Y */
    UH eotcnt;              /* ��M�o�b�t�@�̏I�[������ */

} T_SIOS;

/* �֐��v���g�^�C�v */

ER NORTiAPI ini_sio(INT, const B *);
void cdecl ext_sio(void);
ER NORTiAPI get_sio(INT, UB *, TMO);
ER NORTiAPI put_sio(INT, UB, TMO);
ER NORTiAPI ctl_sio(INT, UH);
ER NORTiAPI ref_sio(INT, T_SIOS *);
ER NORTiAPI fls_sio(INT, TMO);

/* �V���A�����o�͐���u���b�N�\���� */

typedef struct t_sio
{
    UB ch;                  /* �`���l���ԍ� */
    UB flag;                /* ����t���O */

#define TSF_INIT    0x01    /* �������ς� */
#define TSF_TXREQ   0x02    /* XON/XOFF���M�v�� */
#define TSF_RXOFF   0x04    /* XOFF��M���� */
#define TSF_TXOFF   0x08    /* XOFF���M���� */
#define TSF_XON     0x10    /* XON/OFF�ɂ��t���[����L�� */
#define TSF_DTR     0x20    /* DTR�ɂ��t���[����L�� */
#define TSF_RTS     0x40    /* RTS�ɂ��t���[����L�� */

    UB txchr;               /* ���M���� */
    UB rxchr;               /* ��M���� */
    UB rxsts;               /* ��M�X�e�[�^�X */
    UB oldchr;              /* �O��̎�M���� */
    UB oldsts;              /* �O��̎�M�X�e�[�^�X */

    UB eot;                 /* �I�[���� */
    UH eotcnt;              /* �I�[�������o�J�E���^ */

    ID txtid;               /* ���M�҂��^�X�N�h�c */
    ID rxtid;               /* ��M�҂��^�X�N�h�c */
    ID tetid;               /* ���M�I���҂��^�X�N�h�c */

    UB cmd[6];              /* SIO�R�}���h�o�b�t�@ */
    UB rsv[2];              /* �\�� */

    UH txcnt;               /* ���M�o�b�t�@���̕����� */
    UH rxcnt;               /* ��M�o�b�t�@���̕����� */

    UB *txbuf;              /* ���M�o�b�t�@ */
    UB *rxbuf;              /* ��M�o�b�t�@�i+BUFSZ:��M�X�e�[�^�X�o�b�t�@�j*/
    UB *txputp;             /* ���M�o�b�t�@�i�[�|�C���^ */
    UB *txgetp;             /* ���M�o�b�t�@�擾�|�C���^ */
    UB *rxputp;             /* ��M�o�b�t�@�i�[�|�C���^ */
    UB *rxgetp;             /* ��M�o�b�t�@�擾�|�C���^ */

  #ifdef NORTi86
    H far *vram;            /* ���j�^�\���p�e�L�X�gVRAM�A�h���X */
  #endif

} T_SIO;

#ifdef __cplusplus
}
#endif

#endif /* NOSIO_H */
