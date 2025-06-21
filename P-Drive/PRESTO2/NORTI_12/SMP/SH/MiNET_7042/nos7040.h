/******************************************************************************
* �V���A�����o�̓f�o�C�X��`�w�b�_ (SH7040�V���[�Y)                           *
*                                                                             *
*  File name : nos7040.h                                                      *
*  Copyright (c) 1997-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Aug/1997 �V�K�쐬(nos7040.h)                                          MI *
* 24/Apr/2000 NORTi Ver.4 ���p�ɉ���(nos7040.h)                            FJ *
* 23/Dec/2002 SH7042/44/45�ŋ��ʂ̂���nos7040.h�ɖ߂�, �R�����g�C��        MI *
******************************************************************************/

#ifndef NOS7040_H
#define NOS7040_H

#include "sh7040.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ���[�U�[��`�֐� */

extern void rts0_on(void);      /* �����`���l��0 RTS�M��ON */
extern void rts0_off(void);     /* �����`���l��0 RTS�M��OFF */
extern void rts1_on(void);      /* �����`���l��1 RTS�M��ON */
extern void rts1_off(void);     /* �����`���l��1 RTS�M��OFF */

/*
   �n�[�h�E�F�A�ɍ��킹�āA�O���ɏ�L�̊֐����`���Ă��������B
   RTS�o�͂������ꍇ�Arts?_on, rts?_off�֐��͉����������^�[�����Ă��������B
   ���邢�́A�t���[���䖳��NOFLW�}�N�����`����nos7040.c���R���p�C������΁A
   ��L�̊֐��̒�`�͕K�v����܂���B
*/

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

/* �|�[�g�`(SCI ���o�͒[�q�Ƃ̌��p�|�[�g)*/

#define PA_RXD0     0x01    /* RxD0 ���͒[�q (PA0)*/
#define PA_TXD0     0x02    /* TxD0 �o�͒[�q (PA1)*/
#define PA_CTS0     0x04    /* /CTS0���͒[�q (PA2,/IRQ0)*/
#define PA_RXD1     0x08    /* RxD1 ���͒[�q (PA3)*/
#define PA_TXD1     0x10    /* TxD1 �o�͒[�q (PA4)*/
#define PA_CTS1     0x20    /* /CTS1���͒[�q (PA5,/IRQ1)*/

#ifdef __cplusplus
}
#endif
#endif /* NOS7040_H */
