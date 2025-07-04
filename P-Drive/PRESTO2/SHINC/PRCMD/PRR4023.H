/*======================================================================
 * File name	: prr4023.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	�g�����v�Z�d�l�g���p�����[�^�f�[�^�\��
 *	�`�s�`�@�r����������  �r������
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr4023.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr4023.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  kawamura
 * �������r�W����
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr423.h 1.2 1996/07/17 17:10:37 hiroki Exp
 *log: rzr423.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR4023_H)
#define	PRR4023_H

/*------------------------------------------------------*/
/*	�`�s�`�@�r����������  �r������			*/
/*------------------------------------------------------*/

typedef struct prr4023t {
	unsigned short	sample_size;	/*ATA SAMPLE SIZE	*/
	unsigned char	preset_num;	/*�v���Z�b�g�ԍ��@�@	*/
} PRR4023T;

#if	defined(STORAGE)
struct prr4023t	prr4023s;
#else
#if	!defined(NOEXTERN)
extern struct prr4023t	prr4023s;
#endif
#endif

#endif
