/*----------------------------------------------------------------------*
 * File name	: cntget.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/rsh/RCS/cntget.c 1.2 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cntget.c $
 * ���r�W���� 1.2  1970/01/01  00:00:00  yonetsu
 * 2001.01.08 15:20
 * BIOS �� INCLUDE �t�H���_�[�̕ύX
 * 
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

#include	<bios2357\h8sbsc.h>
#include	<custom\custom.h>
#include	<drive\cntdrv.h>
#include	"cnt.h"

/*----------------------------------------------------------------------*/
/* �J�E���^�[�l�̎擾													*/
/*																		*/
/* ������																*/
/*		counter_no :	�J�E���^�[�̔ԍ� ( 1 - ���������̃J�E���^�[�̐�	*/
/* �߂�l																*/
/*		���݂̃J���^�[�l												*/
/*		�ǂݏo�����J���^�[�̓N���A�[����邱�Ƃɒ���					*/
/*----------------------------------------------------------------------*/
int get_scnt(
	int	counter_no
	)
{
	int	cnt = 0;

	if(syscnt.enable_flag == 0)				return cnt;
	if(counter_no > syscnt.enable_flag)		return cnt;
	counter_no--;

	_disable();
	cnt = *(syscnt.cnt + counter_no);
	*(syscnt.cnt + counter_no) = 0;
	_enable();

	return cnt;
}
