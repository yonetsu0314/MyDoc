/*======================================================================
 * File name	: pri0017.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	�ُ픭���f�[�^�\��
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/pri0017.h 1.25 2019-03-28 14:49:39+09 itami Exp $
 *$Log: pri0017.h $
 *Revision 1.25  2019-03-28 14:49:39+09  itami
 *�����@�d���I�t,�����@�A���G���[�ǉ�
 *
 *Revision 1.24  2019-01-07 16:33:11+09  itami
 *�����e�X�g�G���[�ǉ�
 *
 *Revision 1.23  2017-06-14 15:41:30+09  itami
 *�^�]�s���A�K�X�[�U���u�ُ�ǉ�
 *
 *Revision 1.22  2017-02-06 11:04:47+09  satoruk
 *�����R���x���G���[�ǉ��B
 *
 *Revision 1.21  2016-11-02 09:37:49+09  hiroe
 *2016/11/02
 *i-SF�����ڑ�, i-SF�֑��M�s��, i-SF���M�o�b�t�@�t��, i-SF�֍đ����s, i-SF���牞���Ȃ��̒ǉ�
 *
 *Revision 1.20  2016-08-26 16:53:59+09  hiroe
 *2016.08.26
 *i-STATION LINK �A���G���[�̒ǉ�
 *
 *Revision 1.19  2016-06-08 17:46:33+09  hiroe
 *2016/06/08
 *�X�^�[�z�C�[���T�[�{�G���[,�X�C���O�����P�T�[�{�G���[,�X�C���O�����Q�T�[�{�G���[�ǉ�
 *
 *Revision 1.18  2016-04-07 14:04:05+09  hiroe
 *2016/04/07
 *RS�쓮�G���[
 *
 *Revision 1.17  2015-11-05 21:02:57+09  hiroe
 *2015/11/05
 *COM1�\���M��M�G���[,COM1�\���M���M�G���[�̒ǉ�
 *
 *Revision 1.16  2015-10-21 18:28:38+09  hiroe
 *2015.10.21
 *COM�\���M��M�G���[�̒ǉ�
 *
 *Revision 1.15  2015-10-19 20:43:11+09  hiroe
 *2015.10.19
 *COM�\���M���M�G���[�̒ǉ�
 *
 *Revision 1.14  2015-09-08 10:59:13+09  itami
 *RS���_���A�G���[�ǉ�
 *
 *Revision 1.13  2014-12-09 14:10:35+09  itami
 *�ݒ�l�G���[�ǉ�
 *
 *Revision 1.12  2014-10-17 15:14:46+09  itami
 *�t���[���ʒu�G���[�ǉ�
 *
 *Revision 1.11  2014-04-02 00:49:03+09  satoruk
 *2014/04/01
 *CTC�p�ɃC���^�[�t�F�C�X�G���[��ǉ�����B
 *
 * Revision 1.10  1970/01/01  00:00:00  itami
 * 2014/02/06 �X���C�hTH����ُ�ǉ��B
 * 
 * ���r�W���� 1.9  1970/01/01  00:00:00  hiroe
 * 2013/03/12
 * CTC�p��SHUTTER�G���[�AABS�G���[�AN2CTL�G���[��ǉ�����
 * 
 * ���r�W���� 1.8  1970/01/01  00:00:00  itami
 * 2011/08/23 ���U���[�^�[�G���[�ǉ��B
 * 
 * ���r�W���� 1.7  1970/01/01  00:00:00  itami
 * 2008/08/20 �����G���[�ǉ��B
 * 
 * ���r�W���� 1.6  1970/01/01  00:00:00  itami
 * 2007/10/15 �T���v�����O�d�ʃG���[
 * 
 * ���r�W���� 1.5  1970/01/01  00:00:00  kawamura
 * 2007/08/18  09:51
 * �l�܂�G���[��ǉ�����B
 * 
 * ���r�W���� 1.4  1970/01/01  00:00:00  ishidat
 * 2007/01/24 16:30
 * ������_�^�C���A�E�g�G���[�ǉ�
 * ERROR_AUTO_ZERO_TIMEOUT
 * 
 * ���r�W���� 1.3  1970/01/01  00:00:00  itami
 * 2005/08/12 �Z���T�[���m�G���[�ǉ��B
 * 
 * ���r�W���� 1.2  1970/01/01  00:00:00  chimura
 * 2005/03/15 �}���`�A���֘A�G���[�ǉ��@ERROR_MULT
 * 
 * ���r�W���� 1.1  1970/01/01  00:00:00  kawamura
 * �������r�W����
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi017a.h 1.14 1970/01/01 00:00:00 itami Exp
 *log: rzi017a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0017_H)
#define	PRI0017_H


enum pri00170e {			/* �ُ���e			*/
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
	ERROR_COUNT,			/* �����Z�G���[		*/
	ERROR_DIVERTER,			/* �U�蕪�����G���[		*/
	ERROR_EMERGENCY_STOP,		/* ����~			*/
	ERROR_UNDWGT,			/* �y�ʒ�~			*/
	ERROR_FDRV,			/* FDRV�G���[			*/
	ERROR_TB,			/* TB�G���[			*/
	ERROR_MB,			/* MB�G���[			*/
	ERROR_BB,			/* BB�G���[			*/
	ERROR_REJ,			/* �s�ǐU�蕪�����G���[		*/
	ERROR_TPH,			/* ��[�o�g�G���[		*/
	ERROR_MULT,			/* �}���`�A���֘A�G���[		*/
	ERROR_DETECT,			/* �Z���T�[���m�G���[		*/
	ERROR_AUTO_ZERO_TIMEOUT,	/* ������_�^�C���A�E�g�G���[	*/
	ERROR_JAM,			/* �l�܂�G���[			*/
	ERROR_SMP,			/* �T���v�����O�d�ʃG���[	*/
	ERROR_SYNC,			/* �����G���[			*/
	ERROR_DF_MOTOR,			/* ���U���[�^�[�G���[		*/
	ERROR_SHUTTER,			/* CTC�p�V���b�^�[�G���[	*/
	ERROR_ABS,			/* CTC�pABS�G���[		*/
	ERROR_N2CTL,			/* CTC�pN2CTL�G���[		*/
	ERROR_POKER,			/* CTC�pPOKER�G���[		*/
	ERROR_SLIDE_TH,			/* �X���C�hTH����ُ�		*/
	ERROR_CTC_IF,			/* CTC�p�C���^�[�t�F�C�X�G���[	*/
	ERROR_FRAME_POSITION,		/* �t���[���ʒu�G���[		*/
	ERROR_SETTING,			/* �ݒ�l�G���[			*/
	ERROR_RSRET,			/* RS���_���A�G���[		*/
	ERROR_COM_PRESND0,		/* COM0�\���M���M�G���[ */
	ERROR_COM_PRERCV0,		/* COM0�\���M��M�G���[ */
	ERROR_COM_PRESND1,		/* COM1�\���M���M�G���[ */
	ERROR_COM_PRERCV1,		/* COM1�\���M��M�G���[ */
	ERROR_RS_SRVERR,		/* RS�쓮�G���[ */
	ERROR_STRWILLERR,		/* �X�^�[�z�C�[���T�[�{�G���[ */
	ERROR_SWING1_ERR,		/* �X�C���O�����P�T�[�{�G���[ */
	ERROR_SWING2_ERR,		/* �X�C���O�����Q�T�[�{�G���[ */
	ERROR_ISLSND_ERR,		/* i-STATION LINK �A���G���[ */
	ERROR_ISFCON_ERR,		/* i-SF�����ڑ�              */
	ERROR_ISFSND_ERR,		/* i-SF�֑��M�s��            */
	ERROR_ISFFUL_ERR,		/* i-SF���M�o�b�t�@�t��      */
	ERROR_ISFRTY_ERR,		/* i-SF�֍đ����s            */
	ERROR_ISFRES_ERR,		/* i-SF���牞���Ȃ�          */
	ERROR_INFD_CONV,		/* �����R���x���G���[        */
	ERROR_PROD_DIS,			/* �^�]�s����                */
	ERROR_GAS_FILLER,		/* �K�X�[�U���u�ُ�          */
	ERROR_METAL_TEST,		/* �������o��e�X�g�G���[    */
	ERROR_IFD_PW_OFF,		/* �����@�d���I�t	     */
	ERROR_IFD_ILK,			/* �����@�A���G���[	     */

	ERROR_NON = 255,		/* �g�����G���[			*/
};

enum pri00171e {			/* �c�t�b�ʐM�G���[���		*/
	ERR_D_CONNECT	= 0x01,		/* ����f�G���[			*/
	ERR_D_NODE	= 0x02,		/* �m�[�h�����G���[		*/
};

enum pri00172e {			/* �d�w�b�ʐM�G���[���		*/
	ERR_E_CONNECT	= 0x01,		/* ����f�G���[			*/
	ERR_E_NODE	= 0x02,		/* �m�[�h�����G���[		*/
};

enum pri00173e {			/* �c�t�b�G���[���		*/
	ERR_D_CALC	= 0x01,		/* �쓮�p�^�[���v�Z�G���[	*/
	ERR_D_RECEPT	= 0x02,		/* �쓮�p�����[�^����M�G���[	*/
	ERR_D_STM	= 0x04,		/* �쓮�G���[			*/
	ERR_D_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum pri00174e {			/* �d�w�b�G���[���		*/
	ERR_E_STM	= 0x01,		/* �ï��ݸ�Ӱ��ُ��~�G���[	*/
	ERR_E_OVERLAP	= 0x02,		/* ����I�[�o�[���b�v�G���[	*/
	ERR_E_RECEPT	= 0x04,		/* �쓮�p�����[�^����M�G���[	*/
	ERR_E_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum pri00175e {			/* �e�c�q�u�G���[���		*/
	ERR_F_COMERR	= 0x01,		/* �ʐM�G���[			*/
	ERR_F_RECEPT	= 0x02,		/* �쓮�p�����[�^����M�G���[	*/
	ERR_F_OVC	= 0x04,		/* �ߓd���G���[			*/
	ERR_F_POWER	= 0x08,		/* �p���[�G���[			*/
};

enum pri00176e {			/* �e�c�q�u�{�[�h�ԍ����	*/
	FDRV_BOARD_1	= 0x01,		/* FDRV�{�[�h1			*/
	FDRV_BOARD_2	= 0x02,		/* FDRV�{�[�h2			*/
	FDRV_BOARD_3	= 0x04,		/* FDRV�{�[�h3			*/
	FDRV_BOARD_4	= 0x08,		/* FDRV�{�[�h4			*/
};
/*------------------------------------------------------*/
/*	�ُ���̃p�����[�^�p�P�b�g			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0017t {
	unsigned char	ch_id;		/* channel number		*/
	enum pri00170e	error;		/* �ُ���			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned short	rs_ptn;		/* RS pattern			*/
	unsigned short	dth_ptn;	/* DTH pattern			*/
	unsigned short	th_ptn;		/* TH pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned long	af_ptn;		/* AF pattern			*/
	unsigned char	duc_inf;	/* DUC error information	*/
	unsigned char	exc_inf;	/* EXC error information	*/
} PRI0017T;

#if	defined(STORAGE)
struct pri0017t	pri0017s;
#else
#if	!defined(NOEXTERN)
extern struct pri0017t	pri0017s;
#endif
#endif

#endif
