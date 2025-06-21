/*----------------------------------------------------------------------*
 * File name	: cnt.h													*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/rsh/RCS/cnt.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cnt.h $
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
/*		���t���b�V���R���g���[�����g�p�����J�E���^�[					*/
/*----------------------------------------------------------------------*/

#define BASE_CLOCK	196608L		/* 19.6608 Mhz */

struct counter  {
	int	enable_flag;
	int	*cnt;
	int *enable;
};

#ifdef	STORAGE
volatile struct counter syscnt = { 0, (int *)NULL, (int *)NULL };
#else
extern volatile struct counter syscnt;
#endif
