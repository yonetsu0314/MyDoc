/******************************************************************************
* �V���A�����o�̓f�o�C�X��`�w�b�_ (SH7727����SCI/SCIF)                       *
*                                                                             *
*  File name : nos7727.h                                                      *
*  Copyright (c)  2001-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 18/Nov/2001 �쐬                                                            *
******************************************************************************/

#ifndef NOS7727_H
#define NOS7727_H

#include "sh7727.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* �V���A�����[�h���W�X�^ */

#define SMR_CKS0    0x01    /* �N���b�N�Z���N�g0 */
#define SMR_CKS1    0x02    /* �N���b�N�Z���N�g1 */
#ifdef CH0
#define SMR_MP      0x04    /* �}���`�v���Z�b�T���[�h */
#endif
#define SMR_STOP    0x08    /* �P�X�g�b�v�r�b�g(0), �Q�X�g�b�v�r�b�g(1) */
#define SMR_OE      0x10    /* �����p���e�B(0), ��p���e�B(1) */
#define SMR_PE      0x20    /* �p���e�B�C�l�[�u�� */
#define SMR_CHR     0x40    /* �W�r�b�g�f�[�^(0), �V�r�b�g�f�[�^(1) */
#ifdef CH0
#define SMR_CA      0x80    /* �������������[�h(0), �N���b�N���������[�h(1) */
#endif

/* �V���A���R���g���[�����W�X�^ */

#define SCR_CKE0    0x01    /* �N���b�N�C�l�[�u��0 */
#define SCR_CKE1    0x02    /* �N���b�N�C�l�[�u��1 */
#ifdef CH0
#define SCR_TEIE    0x04    /* ���M�I�����荞�݃C�l�[�u�� */
#define SCR_MPIE    0x08    /* �}���`�v���Z�b�T���荞�݃C�l�[�u�� */
#endif
#define SCR_RE      0x10    /* ��M�C�l�[�u�� */
#define SCR_TE      0x20    /* ���M�C�l�[�u�� */
#define SCR_RIE     0x40    /* ��M�f�[�^�t���^��M�G���[���荞�݃C�l�[�u�� */
#define SCR_TIE     0x80    /* ���M�f�[�^�G���v�e�B���荞�݃C�l�[�u�� */

/* �V���A���X�e�[�^�X���W�X�^(SCI) */

#ifdef CH0
#define SSR_MPBT    0x01    /* �}���`�v���Z�b�T�r�b�g���M */
#define SSR_MPB     0x02    /* �}���`�v���Z�b�T�r�b�g��M */
#define SSR_TEND    0x04    /* ���M�I�� */
#define SSR_PER     0x08    /* �p���e�B�G���[ */
#define SSR_FER     0x10    /* �t���[�~���O�G���[ */
#define SSR_ORER    0x20    /* �I�[�o�����G���[ */
#define SSR_RDRF    0x40    /* ��M�f�[�^���W�X�^�t�� */
#define SSR_TDRE    0x80    /* ���M�f�[�^���W�X�^�G���v�e�B */

/* �V���A���X�e�[�^�X���W�X�^(SCIF) */

#else
#define SSR_DR      0x01    /* ��M�f�[�^���f�B */
#define SSR_RDF     0x02    /* ��MFIFO�t�� */
#define SSR_PER     0x04    /* �p���e�B�G���[ */
#define SSR_FER     0x08    /* �t���[�~���O�G���[ */
#define SSR_BRK     0x10    /* �u���[�N���o */
#define SSR_TDFE    0x20    /* ���MFIFO�f�[�^�G���v�e�B */
#define SSR_TEND    0x40    /* ���M�I�� */
#define SSR_ER      0x80    /* ��M�G���[ */
#endif

/* �|�[�gSC�R���g���[�����W�X�^ */

#define SCP0MD0     0x0001
#define SCP0MD1     0x0002
#define SCP1MD0     0x0004
#define SCP1MD1     0x0008
#define SCP2MD0     0x0010
#define SCP2MD1     0x0020
#define SCP3MD0     0x0040
#define SCP3MD1     0x0080
#define SCP4MD0     0x0100
#define SCP4MD1     0x0200
#define SCP5MD0     0x0400
#define SCP5MD1     0x0800
#define SCP6MD0     0x1000
#define SCP6MD1     0x2000
#define SCP7MD0     0x4000
#define SCP7MD1     0x8000

/* �|�[�gSC�f�[�^���W�X�^ */

#define SCP0DT      0x01
#define SCP1DT      0x02
#define SCP2DT      0x04
#define SCP3DT      0x08
#define SCP4DT      0x10
#define SCP5DT      0x20
#define SCP6DT      0x40
#define SCP7DT      0x80

#ifdef __cplusplus
}
#endif
#endif /* NOS7727_H */
