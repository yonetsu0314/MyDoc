/*----------------------------------------------------------------------*
 * File name	: dtpu.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/tpudrv.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tpudrv.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  yonetsu
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		TCU �h���C�o�[�p�w�b�_�[�t�@�C��								*/
/*----------------------------------------------------------------------*/

#include	<bios2357\h8stpu.h>

/*----------------------------------------------------------------------*/
/* �^�C�}�[�̏�����														*/
/*																		*/
/* ������																*/
/*		TPU �ԍ� 0-5													*/
/*		���荞�ݎ���													*/
/*		���荞�ݏ����֐�												*/
/* �߂�l																*/
/*		 0	��� ����I��												*/
/*		 -1	��� ���������s												*/
/*----------------------------------------------------------------------*/

#define BASE_CLOCK	196608

enum {
	TPU_DEV0,
	TPU_DEV1,
	TPU_DEV2,
	TPU_DEV3,
	TPU_DEV4,
	TPU_DEV5,
	TPU_DEV_MAX
};

#ifdef STORAGE
void (*interrupt_func[TPU_DEV_MAX])(void) = { (void *)0 };
const long psc_tbl[] = { 10000L, 40000L, 1600000L, 640000L };
const void *tpu_adrs[] = { 	TPU0_ADRS, TPU1_ADRS, TPU2_ADRS, TPU3_ADRS, TPU4_ADRS, TPU5_ADRS };
#else
extern void (*interrupt_func[])(void);
extern const long psc_tbl[];
extern const void *tpu_adrs[];
#endif

#ifndef	TPUINT_C
void	tgi0a_int_hdr(void);
void	tgi1a_int_hdr(void);
void	tgi2a_int_hdr(void);
void	tgi3a_int_hdr(void);
void	tgi4a_int_hdr(void);
void	tgi5a_int_hdr(void);
#endif

int tpu_tmr_sys_init(int tpu_no,unsigned short ival_t, void (*i_func)(void));
int tpu_tmr_ival_chg(int tpu_no,unsigned short	ival_t);

