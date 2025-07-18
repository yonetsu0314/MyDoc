/*======================================================================
 * File name    : rzr423.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	�g�����v�Z�d�l�g���p�����[�^�i�S�j�f�[�^�\��
 *	�`�s�`�@�r����������  �r������
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr423.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr423.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  chimura
 * �������r�W����
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr423.h 1.2 1996/07/17 17:10:37 hiroki
 *Log: rzr423.h
 * ���r�W���� 1.2  1996/07/17  17:10:37  hiroki
 * �v���Z�b�g�ԍ��ǉ�
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR423_H)
#define	RZR423_H

/*------------------------------------------------------*/
/*	�`�s�`�@�r����������  �r������			*/
/*------------------------------------------------------*/

typedef struct rzr423t {
	unsigned short	sample_size;	/*ATA SAMPLE SIZE	*/
	unsigned char	preset_num;	/*�v���Z�b�g�ԍ��@�@	*/
} RZR423T;

#if	defined(STORAGE)
struct rzr423t	rzr423s;
#else
#if	!defined(NOEXTERN)
extern struct rzr423t	rzr423s;
#endif
#endif

#endif
