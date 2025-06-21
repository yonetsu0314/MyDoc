/*======================================================================
 * File name    : rzi017a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3�p�ُ픭���f�[�^�\��
 *----------------------------------------------------------------------
 *$Header: p:/PRESTO2/SHINC/RZCMD/RCS/rzi017a.h 1.4 2018-06-27 17:19:46+09 itami Exp $
 *$Log: rzi017a.h $
 *Revision 1.4  2018-06-27 17:19:46+09  itami
 *�l�܂�G���[�A���U���[�^�[�G���[�ǉ�
 *
 *Revision 1.3  2017-04-28 14:13:25+09  itami
 *��[�v�g�G���[,�Z���T�[���m�G���[�ǉ�
 *
 *Revision 1.2  2017-04-28 13:14:22+09  itami
 *�R���x���G���[�ǉ�
 *
 *Revision 1.1  2003-01-28 01:07:13+09  chimura
 *�������r�W����
 *
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi017a.h 1.14 1970/01/01 00:00:00 itami
 *Log: rzi017a.h
 * ���r�W���� 1.14  1970/01/01  00:00:00  itami
 * 2002/04/18 ��[�o�g�G���[�ǉ�
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI017_H)
#define	RZI017_H


enum rzi0170e {				/* �ُ���e			*/
	ERROR_ZERO,			/* ��_				*/
	ERROR_SPAN,			/* �X�p��			*/
	ERROR_WH,			/* �v�g				*/
	ERROR_PH,			/* �o�g				*/
	ERROR_BH,			/* �a�g				*/
	ERROR_RS,			/* �q�r				*/
	ERROR_DTH,			/* �c�s�g			*/
	ERROR_TH,			/* �s�g				*/
	ERROR_PW,			/* �P�d				*/
	ERROR_OVSCL,			/* �I�[�o�[�X�P�[��		*/
	ERROR_OVWGT,			/* �ߗ�				*/
	ERROR_AFD,			/* AFD				*/
	ERROR_CHT,			/* �㉺���V���[�g		*/
	ERROR_RCH,			/* �ă`�F�b�N�G���[�i���v�ʁj	*/
	ERROR_NETWORK,			/* �ʐM�G���[			*/
	ERROR_AD,			/* AD				*/
	ERROR_RAM,			/* RAM				*/
	ERROR_ROM,			/* ROM				*/
	ERROR_AUTO_DF,			/* ���U�d�ʎ����ύX		*/
	ERROR_METAL,			/* �������o			*/
	ERROR_AIR,			/* �G�A�[���ቺ			*/
	ERROR_DOOR,			/* ���J��			*/
	ERROR_DIS,			/* �r�o���܂�			*/
	ERROR_ILK,			/* �A���G���[			*/
	ERROR_CUP,			/* �J�b�v�G���[			*/
	ERROR_TRIP,			/* �g���b�v�G���[		*/
	ERROR_FATAL_ZERO,		/* �v���I��_�G���[		*/
        ERROR_COUNT,                    /* �����Z�G���[               */
        ERROR_DIVERTER,                 /* �U�蕪�����G���[             */
        ERROR_EMERGENCY_STOP,          	/* ����~		        */
	ERROR_UNDWGT,			/* �y�ʒ�~			*/
	ERROR_FDRV,			/* FDRV�G���[			*/
	ERROR_TB,			/* TB�G���[			*/
	ERROR_MB,			/* MB�G���[			*/
	ERROR_BB,			/* BB�G���[			*/
	ERROR_REJ,			/* �s�ǐU�蕪�����G���[		*/
	ERROR_TPH,			/* ��[�o�g�G���[		*/
	ERROR_TWH,			/* ��[�v�g�G���[		*/
	ERROR_DETECT,			/* �Z���T�[���m�G���[		*/
	ERROR_CONVEYOR,			/* �R���x���G���[		*/
	ERROR_JAM,			/* �l�܂�G���[			*/
	ERROR_SMP,			/* �T���v�����O�d�ʃG���[	*/
	ERROR_SYNC,			/* �����G���[			*/
	ERROR_DF_MOTOR,			/* ���U���[�^�[�G���[		*/
	ERROR_NON = 255,		/* �g�����G���[			*/
};

enum rzi0171e {				/* �c�t�b�ʐM�G���[���		*/
        ERR_D_CONNECT	= 0x01,  	/* ����f�G���[			*/
        ERR_D_NODE	= 0x02,		/* �m�[�h�����G���[		*/
};

enum rzi0172e {				/* �d�w�b�ʐM�G���[���		*/
        ERR_E_CONNECT	= 0x01,  	/* ����f�G���[			*/
        ERR_E_NODE	= 0x02,		/* �m�[�h�����G���[		*/
};

enum rzi0173e {				/* �c�t�b�G���[���		*/
        ERR_D_CALC	= 0x01,  	/* �쓮�p�^�[���v�Z�G���[	*/
        ERR_D_RECEPT	= 0x02,		/* �쓮�p�����[�^����M�G���[	*/
        ERR_D_STM	= 0x04,  	/* �쓮�G���[			*/
        ERR_D_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum rzi0174e {				/* �d�w�b�G���[���		*/
        ERR_E_STM	= 0x01,  	/* �ï��ݸ�Ӱ��ُ��~�G���[	*/
        ERR_E_OVERLAP	= 0x02,  	/* ����I�[�o�[���b�v�G���[	*/
        ERR_E_RECEPT	= 0x04,		/* �쓮�p�����[�^����M�G���[	*/
        ERR_E_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum rzi0175e {				/* �e�c�q�u�G���[���		*/
        ERR_F_COMERR	= 0x01,  	/* �ʐM�G���[			*/
        ERR_F_RECEPT	= 0x02,  	/* �쓮�p�����[�^����M�G���[	*/
        ERR_F_OVC	= 0x04,		/* �ߓd���G���[			*/
        ERR_F_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum rzi0176e {				/* �e�c�q�u�{�[�h�ԍ����	*/
        FDRV_BOARD_1	= 0x01,  	/* FDRV�{�[�h1			*/
        FDRV_BOARD_2	= 0x02,  	/* FDRV�{�[�h2			*/
        FDRV_BOARD_3	= 0x04,		/* FDRV�{�[�h3			*/
        FDRV_BOARD_4	= 0x08,		/* FDRV�{�[�h4			*/
};
/*------------------------------------------------------*/
/*	�ُ���̃p�����[�^�p�P�b�g			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzi0170e	error;		/* �ُ���			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned char	rs_ptn;		/* RS pattern			*/
	unsigned char	dth_ptn;	/* DTH pattern			*/
	unsigned char	th_ptn;		/* TH pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned char	duc_inf;	/* DUC error information	*/
	unsigned char	exc_inf;	/* EXC error information	*/
} RZI017T;

#if	defined(STORAGE)
struct rzi017t	rzi017s;
#else
#if	!defined(NOEXTERN)
extern struct rzi017t	rzi017s;
#endif
#endif

#define		rz_scan_error	rz_scan_error_v3
#define		rz_print_error	rz_print_error_v3

#endif
