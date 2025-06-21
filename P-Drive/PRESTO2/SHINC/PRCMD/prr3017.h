/*======================================================================
 * File name    : prr3017.h
 * Original	: p:/presto2/shinc/rzcmd/RCS/rzr317.h 1.1
 *----------------------------------------------------------------------
 * Function	:
 *	�^�C�~���O�f�[�^�\��
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prr3017.h 1.1 2015-05-28 10:54:33+09 itami Exp $
 *$Log: prr3017.h $
 *Revision 1.1  2015-05-28 10:54:33+09  itami
 *Initial revision
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR3017_H)
#define	PRR3017_H

/*------------------------------------------------------*/
/*	�^�C�~���O�p�����[�^�p�P�b�g			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr3017t {
	unsigned char	nof_round;	/* ���E���f�B���O�O���[�v��	*/
	unsigned char	round_base[8];	/* ���E���f�B���O�r�o��{����	*/
} PRR3017T;

#if	defined(STORAGE)
struct prr3017t	prr3017s;
#else
#if	!defined(NOEXTERN)
extern struct prr3017t	prr3017s;
#endif
#endif

#endif
