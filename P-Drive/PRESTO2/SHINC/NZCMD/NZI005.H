/*======================================================================
 * File name    : nzi005.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	���W�b�N�g�`�f�[�^�����f�[�^�\��
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi005.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi005.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  chimura
 * �������r�W����
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi005.h 1.1 1999/05/27 16:34:10 kawamura
 *Log: nzi005.h
 * ���r�W���� 1.1  1999/05/27  16:34:10  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI005_H)
#define	NZI005_H

enum nzi0050e {
        LOG_DCS1	= 0x0001,  	/* �r�o�����M���P		 */
        LOG_DCS2	= 0x0002,	/* �r�o�����M���Q		 */
        LOG_ERR1	= 0x0004,  	/* �G���[�M���P			 */
        LOG_ERR2	= 0x0008,	/* �G���[�M���Q			 */
        LOG_IFD1	= 0x0010,  	/* ��������M���P		 */
        LOG_IFD2	= 0x0020,	/* ��������M���Q		 */
        LOG_CTL1	= 0x0040,  	/* �R���g���[���M���P		 */
        LOG_CTL2	= 0x0080,	/* �R���g���[���M���Q		 */
        LOG_ILK1	= 0x0100,  	/* �A���M���P			 */
        LOG_ILK2	= 0x0200,	/* �A���M���Q			 */
        LOG_AUX1	= 0x0400,  	/* �`�t�w�T�i�m�y�̏ꍇ�`�t�w�P�j */
        LOG_AUX2	= 0x0800,	/* �`�t�w�U�i�m�y�̏ꍇ�`�t�w�Q�j */
        LOG_AUX3	= 0x1000,  	/* �`�t�w�P�i�m�y�̏ꍇ�`�t�w�R�j */
        LOG_AUX4	= 0x2000,	/* �`�t�w�Q�i�m�y�̏ꍇ�`�t�w�S�j */
        LOG_AUX5	= 0x4000,  	/* �`�t�w�R�i�m�y�̏ꍇ�`�t�w�T�j */
        LOG_AUX6	= (signed)0x8000,	/* �`�t�w�S�i�m�y�̏ꍇ�`�t�w�U�j */
};

/*------------------------------------------------------*/
/*	���W�b�N�g�`�f�[�^�̉����p�����[�^�p�P�b�g	*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi005t {
	unsigned char	unit_no;	/* �����[���j�b�g�ԍ�	*/
	unsigned char	nof_data;	/* �f�[�^�̐�		*/
	enum nzi0050e	data[32];	/* �f�[�^		*/
} NZI005T;

#if	defined(STORAGE)
struct nzi005t	nzi005s;
#else
#if	!defined(NOEXTERN)
extern struct nzi005t	nzi005s;
#endif
#endif

#endif
