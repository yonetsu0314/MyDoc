/******************************************************************************
* �V���A�����o�̓f�o�C�X��`�w�b�_�iSH7615�j                                  *
*                                                                             *
*  File name : nos7615.h                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 17/Apr/2000 �쐬                                                      MiSPO *
******************************************************************************/

#ifndef NOS7615_H
#define NOS7615_H

#include "sh7615.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* �V���A�����[�h���W�X�^(SCSMR) */

#define SMR_CKS0    0x01    /* �N���b�N�Z���N�g0 */
#define SMR_CKS1    0x02    /* �N���b�N�Z���N�g1 */
#define SMR_MP      0x04    /* �}���`�v���Z�b�T���[�h */
#define SMR_STOP    0x08    /* �P�X�g�b�v�r�b�g(0), �Q�X�g�b�v�r�b�g(1) */
#define SMR_OE      0x10    /* �����p���e�B(0), ��p���e�B(1) */
#define SMR_PE      0x20    /* �p���e�B�C�l�[�u�� */
#define SMR_CHR     0x40    /* �W�r�b�g�f�[�^(0), �V�r�b�g�f�[�^(1) */
#define SMR_CA      0x80    /* �������������[�h(0), �N���b�N���������[�h(1) */

/* �V���A���R���g���[�����W�X�^(SCSCR) */

#define SCR_CKE0    0x01    /* �N���b�N�C�l�[�u��0 */
#define SCR_CKE1    0x02    /* �N���b�N�C�l�[�u��1 */
#define SCR_MPIE    0x08    /* �}���`�v���Z�b�T���荞�݃C�l�[�u�� */
#define SCR_RE      0x10    /* ��M�C�l�[�u�� */
#define SCR_TE      0x20    /* ���M�C�l�[�u�� */
#define SCR_RIE     0x40    /* ��M�f�[�^�t���^��M�G���[���荞�݃C�l�[�u�� */
#define SCR_TIE     0x80    /* ���M�f�[�^�G���v�e�B���荞�݃C�l�[�u�� */

/* �V���A���X�e�[�^�X1���W�X�^(SC1SSR) */

#define SSR_DR      0x0001  /* ���V�[�u�f�[�^���f�B */
#define SSR_RDF     0x0002  /* ���V�[�u�f�[�^���W�X�^�t�� */
#define SSR_PER     0x0004  /* �p���e�B�G���[ */
#define SSR_FER     0x0008  /* �t���[�~���O�G���[ */
#define SSR_BRK     0x0010  /* �u���[�N���o */
#define SSR_TDFE    0x0020  /* �g�����X�~�b�gFIFO�f�[�^�G���v�e�B */
#define SSR_TEND    0x0040  /* �g�����X�~�b�g�G���h */
#define SSR_ER      0x0080  /* ���V�[�u�G���[ */
#define SSR_FER_MSK 0x0f00  /* �t���[�~���O�G���[�� */
#define SSR_PER_MSK 0xf000  /* �p���e�B�G���[�� */

/* �V���A���X�e�[�^�X1���W�X�^(SC2SSR) */

#define SSR2_ORER    0x01   /* �I�[�o�����G���[ */
#define SSR2_EI      0x02   /* ��M�f�[�^�G���[�����C�l�[�u�� */
#define SSR2_MPBT    0x04   /* �}���`�v���Z�b�T�r�b�g�g�����X�t�@�[ */
#define SSR2_MPB     0x08   /* �}���`�v���Z�b�T�r�b�g */
#define SSR2_N0      0x10   /* �N���b�N�r�b�g���[�g�� */
#define SSR2_N1      0x20   /* �N���b�N�r�b�g���[�g�� */
#define SSR2_RLM     0x40   /* ���V�[�uLSB/MSB�t�@�[�X�g�I�� */
#define SSR2_TLM     0x80   /* �g�����X�~�b�gLSB/MSB�t�@�[�X�g�I�� */

/* FIFO�R���g���[�����W�X�^(SCFCR) */

#define FCR_LOOP    0x01    /* ���[�v�o�b�N�e�X�g */
#define FCR_RFRST   0x02    /* ���V�[�uFIFO�f�[�^���W�X�^���Z�b�g */
#define FCR_TFRST   0x04    /* �g�����X�~�b�gFIFO�f�[�^���W�X�^���Z�b�g */
#define FCR_MCE     0x08    /* ���f���R���g���[���C�l�[�u�� */
#define FCR_TTRG0   0x10    /* �g�����X�~�b�gFIFO�f�[�^���g���K */
#define FCR_TTRG1   0x20    /* �g�����X�~�b�gFIFO�f�[�^���g���K */
#define FCR_RTRG0   0x40    /* ���V�[�uFIFO�f�[�^���g���K */
#define FCR_RTRG1   0x80    /* ���V�[�uFIFO�f�[�^���g���K */

/* �|�[�gB�iSCIF ���o�͒[�q�Ƃ̌��p�|�[�g�j*/

#define PB_RXD1     0x4000  /* RxD1 ���͒[�q�iPB14�j*/
#define PBCR_RXD1   0x2000  /* RxD1 �[�q�@�\ */
#define PBCR_RXD1_M 0x3000  /* RxD1 �[�q�@�\�}�X�N */

#define PB_TXD1     0x2000  /* TxD1 �o�͒[�q�iPB13�j*/
#define PBCR_TXD1   0x0800  /* TxD1 �[�q�@�\ */
#define PBCR_TXD1_M 0x0c00  /* TxD1 �[�q�@�\�}�X�N */

#define PB_RTS1     0x1000  /* ~RTS �o�͒[�q�iPB12�j*/
#define PBCR_RTS1   0x0200  /* ~RTS �[�q�@�\ */
#define PBCR_RTS1_M 0x0300  /* ~RTS �[�q�@�\�}�X�N */

#define PB_CTS1     0x0800  /* ~CTS ���͒[�q�iPB11�j*/
#define PBCR_CTS1   0x0080  /* ~CTS �[�q�@�\ */
#define PBCR_CTS1_M 0x00c0  /* ~CTS �[�q�@�\�}�X�N */

#define PB_RXD2     0x0020  /* RxD2 ���͒[�q�iPB5�j*/
#define PBCR_RXD2   0x0800  /* RxD2 �[�q�@�\ */
#define PBCR_RXD2_M 0x0c00  /* RxD2 �[�q�@�\�}�X�N */

#define PB_TXD2     0x0010  /* ~RTS �o�͒[�q�iPB4�j*/
#define PBCR_TXD2   0x0200  /* TxD2 �[�q�@�\ */
#define PBCR_TXD2_M 0x0300  /* TxD2 �[�q�@�\�}�X�N */

#ifdef __cplusplus
};
#endif
#endif /* NOS7615_H */
