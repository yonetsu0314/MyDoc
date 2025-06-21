/*======================================================================
 * File name	: prr1035.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	�X�p�������f�[�^�\�� ���X�p���p
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prr1035.h 1.1 2014-06-26 20:52:15+09 hiroe Exp $
 *$Log: prr1035.h $
 *Revision 1.1  2014-06-26 20:52:15+09  hiroe
 *Initial revision
 *
 *----------------------------------------------------------------------
 *header: rzr035.hv  2.1  93/11/02 15:42:16  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr035.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR1035_H)
#define	PRR1035_H


/*------------------------------------------------------*/
/*	�X�p���Z���p�����[�^�p�P�b�g			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr1035t {
	unsigned short	span_wt2;	/* ���X�p���d��		*/
} PRR1035T;

#if	defined(STORAGE)
struct prr1035t	prr1035s;
#else
#if	!defined(NOEXTERN)
extern struct prr1035t	prr1035s;
#endif
#endif

#endif
