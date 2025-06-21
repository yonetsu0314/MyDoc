/*----------------------------------------------------------------------*
 * File name	: tpuint.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/tpu/RCS/tpuint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tpuint.c $
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
/*		TCU �ɂ��������荞�݂̐ݒ�									*/
/*----------------------------------------------------------------------*/

#define	TPUINT_C

#include	<drive\tpudrv.h>
#include	"dtpu.h"

/*----------------------------------------------------------------------*/
/* TPU ���荞�݃n���h���[												*/
/*																		*/
/* ������																*/
/* �߂�l																*/
/*----------------------------------------------------------------------*/
#pragma interrupt(tgi0a_int_hdr)		/* TPU0 ���荞�������	*/
void	tgi0a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV0] != (void *)0)		interrupt_func[TPU_DEV0]();
	TPU0.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi1a_int_hdr)		/* TPU1 ���荞�������	*/
void	tgi1a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV1] != (void *)0)		interrupt_func[TPU_DEV1]();
	TPU1.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi2a_int_hdr)		/* TPU2 ���荞�������	*/
void	tgi2a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV2] != (void *)0)		interrupt_func[TPU_DEV2]();
	TPU2.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi3a_int_hdr)		/* TPU3 ���荞�������	*/
void	tgi3a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV3] != (void *)0)		interrupt_func[TPU_DEV3]();
	TPU3.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi4a_int_hdr)		/* TPU4 ���荞�������	*/
void	tgi4a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV4] != (void *)0)		interrupt_func[TPU_DEV4]();
	TPU4.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

#pragma interrupt(tgi5a_int_hdr)		/* TPU5 ���荞�������	*/
void	tgi5a_int_hdr(void)
{
	if(interrupt_func[TPU_DEV5] != (void *)0)		interrupt_func[TPU_DEV5]();
	TPU5.TSR.BIT.TGFA = 0;					/* Clear IREQ */
}

