/******************************************************************************
* �V���A�����o�̓f�o�C�X��`�w�b�_�iSH7055�j                                  *
*                                                                             *
*  File name : nos7055.h                                                      *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 14/Jan/2001                                                                 *
******************************************************************************/

#ifndef NOS7055_H
#define NOS7055_H

#include "sh7055.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* �V���A�����[�h���W�X�^ */

#define SMR_CKS0    0x01    /* �N���b�N�Z���N�g0 */
#define SMR_CKS1    0x02    /* �N���b�N�Z���N�g1 */
#define SMR_MP      0x04    /* �}���`�v���Z�b�T���[�h */
#define SMR_STOP    0x08    /* �P�X�g�b�v�r�b�g(0), �Q�X�g�b�v�r�b�g(1) */
#define SMR_OE      0x10    /* �����p���e�B(0), ��p���e�B(1) */
#define SMR_PE      0x20    /* �p���e�B�C�l�[�u�� */
#define SMR_CHR     0x40    /* �W�r�b�g�f�[�^(0), �V�r�b�g�f�[�^(1) */
#define SMR_CA      0x80    /* �������������[�h(0), �N���b�N���������[�h(1) */

/* �V���A���R���g���[�����W�X�^ */

#define SCR_CKE0    0x01    /* �N���b�N�C�l�[�u��0 */
#define SCR_CKE1    0x02    /* �N���b�N�C�l�[�u��1 */
#define SCR_TEIE    0x04    /* ���M�I�����荞�݃C�l�[�u�� */
#define SCR_MPIE    0x08    /* �}���`�v���Z�b�T���荞�݃C�l�[�u�� */
#define SCR_RE      0x10    /* ��M�C�l�[�u�� */
#define SCR_TE      0x20    /* ���M�C�l�[�u�� */
#define SCR_RIE     0x40    /* ��M�f�[�^�t���^��M�G���[���荞�݃C�l�[�u�� */
#define SCR_TIE     0x80    /* ���M�f�[�^�G���v�e�B���荞�݃C�l�[�u�� */

/* �V���A���X�e�[�^�X���W�X�^ */

#define SSR_MPBT    0x01    /* �}���`�v���Z�b�T�r�b�g���M */
#define SSR_MPB     0x02    /* �}���`�v���Z�b�T�r�b�g��M */
#define SSR_TEND    0x04    /* ���M�I�� */
#define SSR_PER     0x08    /* �p���e�B�G���[ */
#define SSR_FER     0x10    /* �t���[�~���O�G���[ */
#define SSR_ORER    0x20    /* �I�[�o�����G���[ */
#define SSR_RDRF    0x40    /* ��M�f�[�^���W�X�^�t�� */
#define SSR_TDRE    0x80    /* ���M�f�[�^���W�X�^�G���v�e�B */

/* �|�[�g�iSCI ���o�͒[�q�Ƃ̌��p�|�[�g�j*/

#define PA_RXD0     0x8000  /* RxD0 ���͒[�q�iPA15�j*/
#define PA_TXD0     0x4000  /* TxD0 �o�͒[�q�iPA14�j*/
#define PC_RXD1     0x0002  /* RxD1 ���͒[�q�iPC2�j*/
#define PC_TXD1     0x0001  /* TxD1 �o�͒[�q�iPC1�j*/
#define PC_RXD2     0x0008  /* RxD2 ���͒[�q�iPC4�j*/
#define PC_TXD2     0x0004  /* TxD2 �o�͒[�q�iPC3�j*/
#define PB_RXD3     0x0200  /* RxD3 ���͒[�q�iPB9�j*/
#define PB_TXD3     0x0100  /* TxD3 �o�͒[�q�iPB8�j*/
#define PB_RXD4     0x0800  /* RxD4 ���͒[�q�iPB11�j*/
#define PB_TXD4     0x0400  /* TxD4 �o�͒[�q�iPB10�j*/

#ifdef __cplusplus
}
#endif
#endif /* NOS7055_H */
