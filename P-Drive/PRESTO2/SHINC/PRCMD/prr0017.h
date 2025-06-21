/*======================================================================
 * File name	: prr0017.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	�^�C�~���O�f�[�^�\��
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prr0017.h 1.1 2003-11-28 23:55:29+09 kawamura Exp $
 *$Log: prr0017.h $
 *Revision 1.1  2003-11-28 23:55:29+09  kawamura
 *�������r�W����
 *
 *----------------------------------------------------------------------
 *header: rzr017.hv  2.1  93/11/02 16:57:10  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr017.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0017_H)
#define	PRR0017_H

typedef struct prr00170t {
	unsigned char	delay;		/* RS�x�ꎞ��			*/
	unsigned char	pool;		/* RS�v�[������			*/
} PRR00170T;

typedef struct prr00172t {
	unsigned char	delay;		/* �x�ꎞ��			*/
	unsigned char	on;		/* �I������			*/
} PRR00172T;

typedef struct prr00171t {
	unsigned long	assign_ptn;	/* Section assign pattern	*/
	unsigned char	wh_ph;		/* WH-PH�x�ꎞ��		*/
	unsigned char	ph_rf;		/* PH-RF�x�ꎞ��		*/
	unsigned char	wh1_bh;		/* WH1-MH�x�ꎞ��		*/
	unsigned char	bh_wh2;		/* MH-WH2�x�ꎞ��		*/
	unsigned char	stagger;	/* �Z�N�V�����������r�o����	*/
	unsigned char	sect_stagger;	/* �Z�N�V�����Ԏ����r�o����	*/
	unsigned char	bh_on;		/* BH�I������			*/
	unsigned char	wh_on;		/* WH�I������			*/
	unsigned char	ph_on;		/* PH�I������			*/
} PRR00171T;

/*------------------------------------------------------*/
/*	�^�C�~���O�p�����[�^�p�P�b�g			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	ds_delay;	/* dump confirm signal delay	*/
	unsigned char	ds_hold_tm;	/* dump confirm signal hold time*/
	unsigned char	op1_delay;	/* option signal 1 delay	*/
	unsigned char	op1_hold_tm;	/* option signal 1 hold time	*/
	unsigned char	op2_delay;	/* option signal 2 delay	*/
	unsigned char	op2_hold_tm;	/* option signal 2 hold time	*/
	unsigned char	wh_delay;	/* WH�x�ꎞ��			*/
	unsigned char	nof_round;	/* ���E���f�B���O�O���[�v��	*/
	unsigned char	round_base[8];	/* ���E���f�B���O�r�o��{����	*/
	unsigned char	min_th_no;	/* �ŏ�TH�ԍ�			*/
	unsigned char	nof_th;		/* �Y������ch��TH��		*/
	struct prr00172t	th[16];	/* TH parameter[nof_th]		*/
	unsigned char	min_dth_no;	/* �ŏ�DTH�ԍ�			*/
	unsigned char	nof_dth;	/* �Y������ch��DTH��		*/
	struct prr00172t	dth[16];/* DTH parameter[nof_dth]	*/
	unsigned char	min_rs_no;	/* �ŏ�RS�ԍ�			*/
	unsigned char	nof_rs;		/* �Y������ch��RS��		*/
	struct prr00170t	rs[16];	/* RS parameter[nof_rs]		*/
	unsigned char	nof_sect;	/* �Y������ch�̃Z�N�V������	*/
	struct prr00171t	sect[8];/* section parameter[nof_sect]	*/
} PRR0017T;

#if	defined(STORAGE)
struct prr0017t	prr0017s;
#else
#if	!defined(NOEXTERN)
extern struct prr0017t	prr0017s;
#endif
#endif

#endif
