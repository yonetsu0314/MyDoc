/*======================================================================
 * File name	: prcmd.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	�R�}���h�T�}���[
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prcmd.h 1.24 2015-05-28 10:55:50+09 itami Exp $
 *$Log: prcmd.h $
 *Revision 1.24  2015-05-28 10:55:50+09  itami
 *WH���E���f�B���O �^�C�~���O�g���e�L�X�g�ǉ�
 *(prr3017,prr3018)
 *
 *Revision 1.23  2014-06-26 20:51:52+09  hiroe
 *2014/06/26
 *���̃X�p���\���̒ǉ��Bprr1035
 *
 * ���r�W���� 1.22  1970/01/01  00:00:00  yonetsu
 * 2012/2/15 13:47
 * �t�B�[�_�쓮�����f�[�^�@�g���p�����[�^�ǉ�
 * 
 * ���r�W���� 1.21  1970/01/01  00:00:00  ohtani
 * 2007/10/12 13:50:06
 * �v�Z���ʊg���p�����[�^�R�}���h��ǉ�����B(pri1011,pri1012)
 * 
 * ���r�W���� 1.20  1970/01/01  00:00:00  chimura
 * 2007/03/22
 * 4-20mA�p�g���p�����[�^�R�}���h�̃\�[�X�ƍ\���̂̌^�̕s��v���C������B
 * (prr2005,prr2006)
 * 
 * ���r�W���� 1.19  1970/01/01  00:00:00  chimura
 * 2007/03/15 4-20mA�p�g���p�����[�^�R�}���h��ǉ�����B(prr2005,prr2006)
 * 
 * ���r�W���� 1.18  1970/01/01  00:00:00  itami
 * 2007/02/21 �g���p�����[�^�R�}���h��ǉ�����B(prr0137,prr0138)
 * 
 * ���r�W���� 1.17  1970/01/01  00:00:00  itami
 * 2007/01/11 ���U�V���b�^�[�p�R�}���h��ǉ�����B(prr0099)
 * 
 * ���r�W���� 1.16  1970/01/01  00:00:00  itami
 * 2006/10/26 ����d�l�f�[�^�p�R�}���h��ǉ�����B(prr135)
 * 
 * ���r�W���� 1.15  1970/01/01  00:00:00  kawamura
 * 2006/06/14  19:10
 * �����U�蕪�����u�Q�[�g�ʒu�g���p�����[�^��ǉ�����B�iprr1005�j
 * 
 * ���r�W���� 1.14  1970/01/01  00:00:00  chimura
 * 2005/09/09
 * FSV�p�ɏ�ԃR�}���h���g������B�ipri2013�j
 * 
 * ���r�W���� 1.13  1970/01/01  00:00:00  chimura
 * 2005/08/12
 * �g���㉺���l�f�[�^�ǉ��iprr5023�j
 * 
 * ���r�W���� 1.12  1970/01/01  00:00:00  ishidat
 * 2005/07/21 16:40
 * PLC�����݃f�[�^��ǉ�����
 * 
 * ���r�W���� 1.12  1970/01/01  00:00:00  ishidat
 * 2005/07/21 16:40
 * PLC�����݃f�[�^��ǉ�����
 * 
 * ���r�W���� 1.11  1970/01/01  00:00:00  kawamura
 * 2005/07/09  10:55
 * �O��̃��r�W�����ŁA�f�[�^�̃T�C�Y���ԈႦ�Ă����s����C������B
 * 
 * ���r�W���� 1.10  1970/01/01  00:00:00  kawamura
 * 2005/06/29  20:20
 * �⏕�g�ݍ��킹�̎Q�l�d�ʃf�[�^���M��ǉ�����B
 * 
 * ���r�W���� 1.9  1970/01/01  00:00:00  itami
 * 2005/03/22 ��ԕ\������ǉ�����B
 * 
 * ���r�W���� 1.8  1970/01/01  00:00:00  kawamura
 * 2005/03/01  13:30
 * ����A�x���[�W�R���g���[����ǉ�����B
 * 
 * ���r�W���� 1.7  1970/01/01  00:00:00  chimura
 * 2005/02/16 �V�t�g�f�[�^�֌W�̃R�}���h��ǉ�
 * 
 * ���r�W���� 1.6  1970/01/01  00:00:00  chimura
 * 2005/02/14 14:00
 * �}���`�A���֘A�̃p�����[�^��ǉ�����B
 * 
 * ���r�W���� 1.5  1970/01/01  00:00:00  kawamura
 * 2004/12/06  15:30
 * prr6023.h�ɂa�g�񓯊�������ON/OFF��ǉ�����B
 * 
 * ���r�W���� 1.4  1970/01/01  00:00:00  kawamura
 * 2004/08/10  15:41
 * �o�g�Z���֘A�y�уZ�~�I�[�g�֘A�̃R�}���h��ǉ�����B
 * 
 * ���r�W���� 1.3  1970/01/01  00:00:00  kawamura
 * 2004/05/08  16:00
 * �V�`�e�c�Ɋւ���R�}���h��ǉ�����B
 * 
 * ���r�W���� 1.2  1970/01/01  00:00:00  kawamura
 * 2004/05/03  15:00
 * �t���g���CATA�֘A�̃t�@�C����ǉ�����B
 * 
 * ���r�W���� 1.1  1970/01/01  00:00:00  kawamura
 * �������r�W����
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzcmd.h 2.95 1970/01/01 00:00:00 kawamura Exp
 *log: rzcmd.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#ifndef	__PRCMD_H__
#define	__PRCMD_H__

/* �ʐM�e�L�X�g�̃t�H�[�}�b�g */
/**/
/*             �|�C���^�v�Z�̋N�_                                       */
/*                ��0                      p1                           */
/*����������������������������������������������������������������������*/
/*��LF��DID ��SID ��CMD ��PTR0��PARAMETER0��PTR1��PARAMETER1��SUM ��CR��*/
/*�� 1��  4 ��  4 ��  1 ��   4��        n0��   4��        n1��  2 �� 1��*/
/*����������������������������������������������������������������������*/
/*    ��                                  ��                ��          */
/*    ��                               �g���p�����[�^       ��          */
/*    ��                                                    ��          */
/*    ��������������������������������������������������������          */
/*    ��SUM �v�Z�͈�		                            ��          */

/* PTRn �͎��̃p�����[�^�� CMD ���N�_�Ƃ����J�n�ʒu�������B             */
/* ���̃p�����[�^���Ȃ��ꍇ�� 0 �Ƃ���B                                */

#include	<custom\typedefs.h>		/* Show xlib		*/

typedef struct fmt_rzhd {			/* �擪�̃w�b�_�[��	*/
	BYTE	srcno[4];			/* �]�����v�ʋ@number	*/
	BYTE	dstno[4];			/* �]����	"	*/
} FMT_RZHD;

#define	PRMP	4				/* �|�C���^�[�̃T�C�Y	*/

/*#define		****_CMD	(' ')*/
#define		DCA_PRM_CMD	('!')
#define		ATA_DEV_CMD	('"')
#define		TIMER_PRM_CMD	('#')
#define		DMY_WEIGHT_CMD	('$')
#define		SHIFT_REQ_CMD	('%')
#define		SHIFT_MON_CMD	('&')
#define		PLC_WR_CMD	('\'')
#define		DCA_MON_CMD	('(')
#define		ATA_MON_CMD	(')')
#define		MLT_COND_CMD	('*')
#define		ADV_AFD_CMD	('+')
#define		PNUM_REQ_CMD	(',')
#define		MLT_P1_CMD	('-')
#define		INFO_REQ_CMD	('.')
#define		INFO_CMD	('/')
#define		LOGIC_REQ_CMD	('0')
#define		LOGIC_CMD	('1')
#define		ARC_REQ_CMD	('2')
#define		ARC_CMD		('3')
#define		RES_CYC_REQ_CMD	('4')
#define		RES_CYC_CMD	('5')
#define		RES_CYC_DET_CMD	('6')
#define		RES_CYC_STP_CMD	('7')
#define		DRV_CYC_PRM_CMD	('8')
#define		NAT_FRQ_PRM_CMD	('9')
#define		SAMPLE_DUMP_CMD	(':')
#define		RYIN_REQ_CMD	(';')
#define		MLT_EXT_CMD	('<')
#define		RXCOMP_CMD	('=')
#define		ACTCOMP_CMD	('>')
#define		SYSERR_CMD	('?')
#define		OPT_DEV_CMD	('@')
#define		SPAN_CMD	('A')
#define		SIG_ASSIGN_CMD	('B')
#define		MAN_COMBI_CMD	('C')
#define		ERREJ_CMD	('D')
#define		CBRSLT_CMD	('E')
#define		WRNG_CMD	('F')
#define		DRAIN_STRT_CMD	('G')
#define		DRAIN_STOP_CMD	('H')
#define		ERROR_CMD	('I')
#define		SPC_REQ_CMD	('J')
#define		SPAN_COE_CMD	('K')
#define		SCL_REQ_CMD	('L')
#define		MAN_CYCLE_CMD	('M')
#define		COND_CMD	('N')
#define		P_RSLT_CMD	('O')
#define		TDRV_STRT_CMD	('P')
#define		TDRV_STOP_CMD	('Q')
#define		PROD_STRT_CMD	('R')
#define		PROD_STOP_CMD	('S')
#define		TGT_PRM_CMD	('T')
#define		DIAGREQ_CMD	('U')
#define		AFVADJ_CMD	('V')
#define		PW_ON_CMD	('W')
#define		PW_OFF_CMD	('X')
#define		PNUM_CMD	('Y')
#define		ZERO_CMD	('Z')
#define		NETMAP_REQ_CMD	('[')
#define		SCALE_CMD	('\\')
#define		NETMAP_CMD	(']')
#define		MLT_P2_CMD	('^')
#define		MLT_P1REQ_CMD	('_')
/*#define		****_CMD	('`')*/
#define		WEI_SPEC_CMD	('a')
#define		BMIF_SPEC_CMD	('b')
#define		CBCAL_SPEC_CMD	('c')
#define		HOP_SPEC_CMD	('d')
#define		FD_SPEC_CMD	('e')
#define		RFDF_PRM_CMD	('f')
#define		CF_PRM_CMD	('g')
#define		AFDH_CMD	('h')
#define		PCUERR_CMD	('i')
#define		DZHOPSPEC_CMD	('j')
#define		CF_RSLT_CMD	('k')
#define		USR_LEVEL_CMD	('l')
#define		MEAN_CMD	('m')
/*#define		****_CMD	('n')*/
#define		P_TARG_CMD	('o')
#define		RNWPW_CMD	('p')
#define		RNWCOMP_CMD	('q')
#define		AFVRECALL_CMD	('r')
#define		AFDS_CMD	('s')
#define		TMG_PRM_CMD	('t')
#define		DIAGDATA_CMD	('u')
#define		AFVCOE_CMD	('v')
#define		MCH_SPEC_CMD	('w')
#define		INFD_SPEC_CMD	('x')
#define		COORD_CMD	('y')
#define		EEPROMCLR_CMD	('z')
#define		METAL_TEXT_CMD	('{')
#define		SH_CTL_CMD	('|')
#define		METAL_ANS_CMD	('}')
#define		RYIN_CMD	('~')


#define		HP_DRV_REQ_CMD	(0xa1)
#define		RYOUT_CMD	(0xa2)
#define		HP_DRV_CMD	(0xa3)
#define		HP_DRV_SET_CMD	(0xa4)
#define		FZERO_CMD	(0xa5)
#define		PRENUM_CMD	(0xa6)
#define		INFEED_CTL_CMD	(0xa7)
#define		DRAIN_ZERO_CMD	(0xa8)
#define		GOTO_BOOT_CMD	(0xa9)
#define		DSP_COND_CMD	(0xaa)
#define		SP_PARM_CMD	(0xab)
#define		EXT_PARM_CMD	(0xac)
/*#define		****_CMD	(0xad)*/
/*       		  �     	      */
/*       		  �     	      */
/*#define		****_CMD	(0xdf)*/


/*------------------------------------------------------*/
/*	�v�ʋ@�̍\���d�l�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0003,prr0004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* hopper assign pattern	*/
	BYTE	drv_type;		/* drive type			*/
	BYTE	direction;		/* ���s�ǁA�Q�������ʎq		*/
	BYTE	level;			/* �i��(WH����̈ʒu)		*/
	BYTE	port[2];		/* EXC�|�[�g�ԍ�		*/
} PR00030;

struct fmt_mch_construct {
	BYTE	cmd;			/* command = 'w'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	head_construct;		/* radial/linear		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	model_id;		/* model ID			*/
	BYTE	nof_df;			/* number of DF			*/
	BYTE	df_assign_ptn[8][8];	/* DF loacate pattern		*/
	BYTE	nof_af[2];		/* number of AF			*/
	BYTE	af_assign_ptn[32][8];	/* AF loacate pattern		*/
	BYTE	nof_rs[2];		/* number of RS			*/
	PR00030	rs[16];			/* PR00030 parameter of RS	*/
	BYTE	nof_dth[2];		/* number of DTH		*/
	PR00030	dth[16];		/* PR00030 parameter of DTH	*/
	BYTE	nof_th[2];		/* number of TH			*/
	PR00030	th[16];			/* PR00030 parameter of TH	*/
	BYTE	nof_duc_net;		/* number of duc net		*/
	BYTE	duc_assign_ptn[2][8];	/* DUC network pattern		*/
/*-------------------------------------*/
};

int	pr_scan_mch_construct( struct fmt_mch_construct *buf);
void *	pr_print_mch_construct( struct fmt_mch_construct *txt, struct prr0003t *pkt);


/*------------------------------------------------------*/
/*	�����@�̐���d�l�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0005,prr0006					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	sig_assign_ptn[8];	/* infeed signal assign pattern	*/
	BYTE	infd_detector;		/* level sensor type		*/
	BYTE	transient_int[2];	/* transient interrupt time	*/
	BYTE	feed_intrpt[2];		/* low product time		*/
	BYTE	targ[4];		/* �ڕW�d��			*/
	BYTE	upper[2];		/* ����΍�(%)			*/
	BYTE	lower[2];		/* �����΍�(%)			*/
} PR00050;

struct fmt_infd_spec {
	BYTE	cmd;			/* command = 'x'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sig;		/* number of infeed signal	*/
	PR00050	sig[8];			/* PR00050 paramter[nof_sig]	*/
	BYTE	nof_cf;			/* number of CF			*/
	BYTE	cf_assign_ptn[8][8];	/* CF assign pattern		*/
	BYTE	pconst[4];		/* proportion constant		*/
	BYTE	iconst[4];		/* integral constant		*/
	BYTE	dconst[4];		/* differrential constant	*/
	BYTE	filter_sw[4];		/* filter switch		*/
	BYTE	filter_const[4];	/* filter constant		*/
/*-------------------------------------*/
};

int	pr_scan_infd_spec( struct fmt_infd_spec *buf);
void *	pr_print_infd_spec( struct fmt_infd_spec *txt, struct prr0005t *pkt);


/*------------------------------------------------------*/
/*	��@�Ƃ̘A���d�l�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0007,prr0008					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	bm_assign_ptn[8];	/* bag maker assign pattern	*/
	BYTE	interface;		/* I/F type			*/
	BYTE	mul_dump_ini;		/* multi dump initiate mode	*/
	BYTE	mul_dump_conf;		/* multi dump confirm mode	*/
	BYTE	manu_dump_ini;		/* manual dump initiate mode	*/
	BYTE	manu_dump_conf;		/* manual dump confirm mode	*/
} PR00070;

struct fmt_bmif_spec {
	BYTE	cmd;			/* command = 'b'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_bm;			/* number of bag maker		*/
	PR00070	ch[4];			/* PR00070 paramter[nof_bm]	*/
/*-------------------------------------*/
};

int	pr_scan_bmif_spec( struct fmt_bmif_spec *buf);
void *	pr_print_bmif_spec( struct fmt_bmif_spec *txt, struct prr0007t *pkt);


/*------------------------------------------------------*/
/*	���̌v�ʓ����e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0009,prr0010					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	thin[4];		/* �Ԉ�����			*/
	BYTE	dig[4];			/* �L����������			*/
	BYTE	tap[4];			/* �^�b�v��			*/
	BYTE	coef[4];		/* �t�B���^�W��			*/
} PR00093;

typedef struct {
	BYTE	stable_count[2];	/* stable count			*/
	BYTE	zero_limit_cnt[4];	/* auto zero error count	*/
	BYTE	stable_cmp_cnt;		/* compare count		*/
	BYTE	range;			/* weigh range			*/
	BYTE	smpl_interval[2];	/* sampling interval		*/
	BYTE	iir_reset_cnt[2];	/* IIR filter reset count	*/
	BYTE	afv_mode;		/* AFV mode (off/auto/on)	*/
	BYTE	filter;			/* filter number		*/
	BYTE	span_wt;		/* �X�p������			*/
	BYTE	cell_type;		/* cell type			*/
	BYTE	nof_stage;		/* �t�B���^�X�e�[�W��		*/
	PR00093	dadfip[6];		/* �t�B���^�p�����[�^		*/
	BYTE	afv_wh_limit[4];	/* AFV����WH���E�l		*/
	BYTE	afv_afv_limit[4];	/* AFV����AFV���E�l		*/
	BYTE	afv_onoff_limit[4];	/* AFV�␳ON/OFF���E�l		*/
} PR00090;

typedef struct {
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	stick_weight[4];	/* stick weight			*/
} PR00091;

typedef struct {
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR00091	sect[8];		/* PR00091 paramter[nof_sect]	*/
} PR00092;

struct fmt_wei_spec {
	BYTE	cmd;			/* command = 'a'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_adc;		/* number of ADC board		*/
	PR00090	adc[4];			/* PR00090 paramter[nof_adc]	*/
	BYTE	nof_ch;			/* number of channel		*/
	PR00092	ch[4];			/* PR00092 paramter[nof_ch]	*/
};

int	pr_scan_wei_spec( struct fmt_wei_spec *buf);
void *	pr_print_wei_spec( struct fmt_wei_spec *txt, struct prr0009t *pkt);


/*------------------------------------------------------*/
/*	�g�ݍ��킹�v�Z�̓����e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0011,prr0012					*/
/*------------------------------------------------------*/

struct fmt_cbcal_spec {
	BYTE	cmd;			/* command = 'c'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	wh_active[8];		/* WH active head[nof_head/4]	*/
	BYTE	bh_active[8];		/* BH active head[nof_head/4]	*/
	BYTE	compensation[2];	/* compensation value		*/
	BYTE	renew_compensation;	/* renew compensation control	*/
	BYTE	calc_mode;		/* calculate control		*/
	BYTE	mix_mode;		/* mix calculate control	*/
	BYTE	over_scale;		/* over scale process control	*/
	BYTE	over_weight;		/* over weight process control	*/
	BYTE	hop_error;		/* hopper error process control	*/
	BYTE	az_error;		/* autozero error process control*/
	BYTE	stable;			/* stable head process control	*/
	BYTE	re_check;		/* re check process control	*/
	BYTE	th_error;		/* TH process control		*/
	BYTE	nof_stop_head[2];	/* number of stop head		*/
	BYTE	nof_stop_cycle[2];	/* number of stop cycle		*/
	BYTE	err_rej_type;		/* error reject process control	*/
	BYTE	multi_cal;		/* multi cal. process control	*/
	BYTE	approval_flag;		/* approval flag		*/
	BYTE	max_weight[4];		/* max. weight of approval	*/
	BYTE	min_weight[4];		/* min. weight of approval	*/
/*-------------------------------------*/
};

int	pr_scan_cbcal_spec( struct fmt_cbcal_spec *buf);
void *	pr_print_cbcal_spec( struct fmt_cbcal_spec *txt, struct prr0011t *pkt);


/*------------------------------------------------------*/
/*	�z�b�p�̊J�����̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0013,prr0014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	pulse_mltp[2];		/* ��{�p���X�{��		*/
	BYTE	repeat[2];		/* �J��Ԃ���			*/
} PR00130;

struct fmt_hop_spec {
	BYTE	cmd;			/* command = 'd'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	actuator_type;		/* �A�N�`���G�[�^�^�C�v		*/
	BYTE	phase_type;		/* �㎥���[�h	3:1-2��		*/
	BYTE	stop_mode;		/* ��~����			*/
	BYTE	speed[4];		/* ��]�� [�P�ʃp���X����0.4us] */
	BYTE	ignore_pulse[4];	/* �X���b�g�s���p���X��		*/
	BYTE	cam_slit[2];		/* �X���b�g��~�p���X��		*/
	BYTE	br_cl_delay[2];		/* BR-CL����x��p���X��	*/
	BYTE	cl_br_delay[2];		/* CL-BR����x��p���X��	*/
	BYTE	brake_time[2];		/* �u���[�L����[10msec]		*/
	BYTE	error_pulse[2];		/* �G���[���o�p���X��		*/
	BYTE	nof_pattern[2];		/* �p�^�[����			*/
	PR00130	ary[128];		/* PR00130 paramter[nof_pattern]*/
/*-------------------------------------*/
};

int	pr_scan_hop_spec( struct fmt_hop_spec *buf);
void *	pr_print_hop_spec( struct fmt_hop_spec *txt, struct prr0013t *pkt);


/*------------------------------------------------------*/
/*	�t�B�[�_�̋쓮�����̃e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0015,prr0016					*/
/*------------------------------------------------------*/

struct fmt_fd_spec {
	BYTE	cmd;			/* command = 'e'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	fd_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	feeder_id;		/* feeder id			*/
	BYTE	min_amp;		/* minimun aplitude		*/
	BYTE	max_amp;		/* maximun aplitude		*/
	BYTE	multiplier;		/* �t�B�[�_���Ԕ{��		*/
	BYTE	phase_type;		/* �ʑ�				*/
	BYTE	opt_sw_ctrl;		/* ���d�qSW�ɂ���~���[�h	*/
	BYTE	shutter_ctrl;		/* �V���b�^�A�����[�h		*/
	BYTE	shutter_off_delay[2];	/* �V���b�^��~�x�ꎞ��		*/
	BYTE	slow_start;		/* �X���[�X�^�[�g		*/
	BYTE	stop_ptn;		/* �X�g�b�v�p�^�[������		*/
/*-------------------------------------*/
};

int	pr_scan_fd_spec( struct fmt_fd_spec *buf);
void *	pr_print_fd_spec( struct fmt_fd_spec *txt, struct prr0015t *pkt);


/*------------------------------------------------------*/
/*	�^�C�~���O�̐ݒ�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0017,prr0018					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	delay[2];		/* RS�x�ꎞ��[nof_rs]		*/
	BYTE	pool[2];		/* RS�v�[������			*/
} PR00170;

typedef struct {
	BYTE	delay[2];		/* �x�ꎞ��[nof_rs]		*/
	BYTE	on[2];			/* �I������			*/
} PR00171;

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern	*/
	BYTE	wh_ph[2];		/* WH-PH�x�ꎞ��		*/
	BYTE	ph_rf[2];		/* PH-RF�x�ꎞ��		*/
	BYTE	wh1_bh[2];		/* WH1-MH�x�ꎞ��		*/
	BYTE	bh_wh2[2];		/* MH-WH2�x�ꎞ��		*/
	BYTE	stagger[2];		/* �Z�N�V�����������r�o����	*/
	BYTE	sect_stagger[2];	/* �Z�N�V�����Ԏ����r�o����	*/
	BYTE	bh_on[2];		/* BH�I������			*/
	BYTE	wh_on[2];		/* WH�I������			*/
	BYTE	ph_on[2];		/* PH�I������			*/
} PR00172;

struct fmt_tmg_parm {
	BYTE	cmd;			/* command = 't'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	ds_delay[2];		/* dump confirm signal delay	*/
	BYTE	ds_hold_tm[2];		/* dump confirm signal hold time*/
	BYTE	op1_delay[2];		/* option signal 1 delay	*/
	BYTE	op1_hold_tm[2];		/* option signal 1 hold time	*/
	BYTE	op2_delay[2];		/* option signal 2 delay	*/
	BYTE	op2_hold_tm[2];		/* option signal 2 hold time	*/
	BYTE	wh_delay[2];		/* WH�x�ꎞ��			*/
	BYTE	nof_round;		/* ���E���f�B���O�O���[�v��	*/
	BYTE	round_base[8][2];	/* ���E���f�B���O�r�o��{����	*/
	BYTE	min_th_no[2];		/* �ŏ�TH�ԍ�			*/
	BYTE	nof_th[2];		/* �Y������ch��TH��		*/
	PR00171	th[16];			/* TH�x�ꎞ��[nof_th]		*/
	BYTE	min_dth_no[2];		/* �ŏ�DTH�ԍ�			*/
	BYTE	nof_dth[2];		/* �Y������ch��DTH��		*/
	PR00171	dth[16];		/* DTH�x�ꎞ��[nof_dth]		*/
	BYTE	min_rs_no[2];		/* �ŏ�RS�ԍ�			*/
	BYTE	nof_rs[2];		/* �Y������ch��RS��		*/
	PR00170	rs[16];			/* PR00170 paramter[nof_rs]	*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR00172	sect[8];		/* PR00172 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_tmg_parm( struct fmt_tmg_parm *buf);
void *	pr_print_tmg_parm( struct fmt_tmg_parm *txt, struct prr0017t *pkt);


/*------------------------------------------------------*/
/*	�����ʂ̐ݒ�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0019,prr0020					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	amp[2];			/* �t�B�[�_�U�����x		*/
	BYTE	time[2];		/* �t�B�[�_�U������		*/
} PR00190;

struct fmt_rfdf_parm {
	BYTE	cmd;			/* command = 'f'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	backup;			/* backup			*/
	BYTE	nof_rf[2];		/* �Y������ch��RF��		*/
	PR00190	rf[32];			/* PR00190 paramter[nof_rf]	*/
	BYTE	nof_df;			/* �Y������ch��DF��		*/
	PR00190	df[8];			/* PR00190 paramter[nof_df]	*/
	BYTE	nof_af[2];		/* �Y������ch��AF��		*/
	PR00190	af[32];			/* PR00190 paramter[nof_af]	*/
/*-------------------------------------*/
};

int	pr_scan_rfdf_parm( struct fmt_rfdf_parm *buf);
void *	pr_print_rfdf_parm( struct fmt_rfdf_parm *txt, struct prr0019t *pkt);


/*------------------------------------------------------*/
/*	CF�̐U���ݒ�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0021,prr0022					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* ������Ԑ�			*/
	PR00190	at[8];			/* PR00190 paramter[nof_sig]	*/
} PR00210;

struct fmt_cf_parm {
	BYTE	cmd;			/* command = 'g'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ctrl;			/* control			*/
	BYTE	nof_cf;			/* CF��				*/
	PR00210	cf[8];			/* PR00210 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	pr_scan_cf_parm( struct fmt_cf_parm *buf);
void *	pr_print_cf_parm( struct fmt_cf_parm *txt, struct prr0021t *pkt);


/*------------------------------------------------------*/
/*	�`�e�c�Z�N�V�������̃e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0057,prr0058					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	ctrl_mode[2];		/* AFD���[�h			*/
	BYTE	ctrl_sw[2];		/* AFD����r�v			*/
	BYTE	amp_max[2];		/* �U�����x��Ώ��		*/
	BYTE	amp_min[2];		/* �U�����x��Ή���		*/
	BYTE	time_max[2];		/* �U�����Ԑ�Ώ��		*/
	BYTE	time_min[2];		/* �U�����Ԑ�Ή���		*/
} PR00570;

typedef struct {
	BYTE	amp_max[2];		/* �U�����x��Ώ��		*/
	BYTE	amp_min[2];		/* �U�����x��Ή���		*/
	BYTE	time_max[2];		/* �U�����Ԑ�Ώ��		*/
	BYTE	time_min[2];		/* �U�����Ԑ�Ή���		*/
} PR00571;

struct fmt_afd_sect {
	BYTE	cmd;			/* command = 's'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR00570	sect[8];		/* PR00570 paramter[nof_sect]	*/
	BYTE	nof_df;			/* �v�ʋ@��DF��			*/
	PR00571	df[8];			/* PR00571 paramter[nof_df]	*/
	BYTE	nof_af[2];		/* �v�ʋ@��AF��			*/
	PR00571	af[32];			/* PR00571 paramter[nof_df]	*/
/*-------------------------------------*/
};

int	pr_scan_afd_sect( struct fmt_afd_sect *buf);
void *	pr_print_afd_sect( struct fmt_afd_sect *txt, struct prr0057t *pkt);


/*------------------------------------------------------*/
/*	�v�ʖڕW�l�̐ݒ�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0023,prr0024					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern	*/
	BYTE	target_wt[8];		/* �v�ʖڕW�d��			*/
	BYTE	upper_wt[4];		/* ����d��			*/
	BYTE	upper_wt2[4];		/* ��Q����d��			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* �v�ʖڕW��			*/
	BYTE	upper_cnt[2];		/* �����			*/
	BYTE	upper_cnt2[2];		/* ��Q�����			*/
	BYTE	lower_cnt[2];		/* ������			*/
	BYTE	piece_wt[4];		/* �P�d				*/
	BYTE	target_head[2];		/* �ڕW�I���w�b�h��		*/
	BYTE	pw_ctrl;		/* �P�d�����X�V���[�h		*/
	BYTE	mix_ctrl;		/* �����␳���[�h		*/
	BYTE	cnt[2];			/* �D��r�o��			*/
} PR00230;

struct fmt_wei_parm {
	BYTE	cmd;			/* command = 'T'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR00230	sect[8];		/* PR00230 paramter[nof_sect]	*/
	BYTE	target_wt[8];		/* �v�ʖڕW�d��			*/
	BYTE	upper_wt[4];		/* ����d��			*/
	BYTE	upper_wt2[4];		/* ��Q����d��			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* �v�ʖڕW��			*/
	BYTE	upper_cnt[2];		/* �����			*/
	BYTE	upper_cnt2[2];		/* ��Q�����			*/
	BYTE	lower_cnt[2];		/* ������			*/
	BYTE	speed[4];		/* �v�ʔ\��			*/
	BYTE	dump_cnt[2];		/* �r�o��			*/
	BYTE	avg_ctrl;		/* ���ϒl���䃂�[�h		*/
	BYTE	second_upper_cycle[4];	/* ��Q����r�o�T�C�N��		*/
	BYTE	cyc_of_under[2];	/* �v�ʔ����p�x			*/
/*-------------------------------------*/
};

int	pr_scan_wei_parm( struct fmt_wei_parm *buf);
void *	pr_print_wei_parm( struct fmt_wei_parm *txt, struct prr0023t *pkt);


/*------------------------------------------------------*/
/*	�e�w�b�h�v�ʖڕW�l�̐ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0059,prr0060					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	no[2];			/* �w�b�h�ԍ�			*/
	BYTE	upper_wt[4];		/* ����d��			*/
	BYTE	lower_wt[4];		/* �����d��			*/
	BYTE	rf_ctrl;		/* �q�e�쓮			*/
	BYTE	ph_fd[2];		/* PH-�����M���x�ꎞ��		*/
} PR00590;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR00590	phead[2];		/* PR00590 paramter[nof_phead]	*/
	BYTE	on_off;			/* �e�q�v�ʂ���^���Ȃ�		*/
	BYTE	dump;			/* �����r�o�^���ݔr�o		*/
	BYTE	calc;			/* �v�Z�Q���^�s�Q��		*/
	BYTE	az_intvl[4];		/* ������_�����Ԋu		*/
	BYTE	over;			/* ����ȏ㎞�̏���		*/
	BYTE	under;			/* �����������̏���		*/
} PR00591;

struct fmt_pandc_parm {
	BYTE	cmd;			/* command = 'o'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR00591	sect[8];		/* PR00591 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	pr_scan_pandc_parm( struct fmt_pandc_parm *buf);
void *	pr_print_pandc_parm( struct fmt_pandc_parm *txt, struct prr0059t *pkt);


/*------------------------------------------------------*/
/*	�v���O�����ԍ��̗v���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0025,prr0026					*/
/*------------------------------------------------------*/

struct fmt_req_prgno {
	BYTE	cmd;			/* command = ','		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* �{�[�hID�ԍ�			*/
	BYTE	dev_no;			/* �f�o�C�X�ԍ�			*/
	BYTE	node_id[2];		/* �m�[�h�ԍ�			*/
/*-------------------------------------*/
};

int	pr_scan_req_prgno( struct fmt_req_prgno *buf);
void *	pr_print_req_prgno( struct fmt_req_prgno *txt, struct prr0025t *pkt);


/*------------------------------------------------------*/
/*	���f�[�^�̗v���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0027,prr0028					*/
/*------------------------------------------------------*/

struct fmt_req_weidat {
	BYTE	cmd;			/* command = 'L'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_req_weidat( struct fmt_req_weidat *buf);
void *	pr_print_req_weidat( struct fmt_req_weidat *txt, struct prr0027t *pkt);


/*------------------------------------------------------*/
/*	�쓮�n�d��ON�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_power_on {
	BYTE	cmd;			/* command = 'W'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};

/*------------------------------------------------------*/
/*	�쓮�n�d��OFF�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_power_off {
	BYTE	cmd;			/* command = 'X'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};

/*------------------------------------------------------*/
/*	�r�o��_�����e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0033,prr0034					*/
/*------------------------------------------------------*/

struct fmt_zero_adj {
	BYTE	cmd;			/* command = 'Z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* �����w�b�h�r�b�g�p�^�[��	*/
	BYTE	adj_df_ptn[2];		/* ����DF�r�b�g�p�^�[��		*/
	BYTE	init_zero_flag;		/* ������_�t���O		*/
	BYTE	ad_coef[4];		/* AD�W��			*/
	BYTE	da_coef[4];		/* DA�W��			*/
	BYTE	range[4];		/* ����				*/
/*-------------------------------------*/
};

int	pr_scan_zero_adj( struct fmt_zero_adj *buf);
void *	pr_print_zero_adj( struct fmt_zero_adj *txt, struct prr0033t *pkt);


/*------------------------------------------------------*/
/*	�X�p���Z���e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0035,prr0036					*/
/*------------------------------------------------------*/

struct fmt_span_adj {
	BYTE	cmd;			/* command = 'A'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* �����w�b�h�r�b�g�p�^�[��	*/
	BYTE	date[8];		/* �������t			*/
	BYTE	time[8];		/* ��������			*/
/*-------------------------------------*/
};

int	pr_scan_span_adj( struct fmt_span_adj *buf);
void *	pr_print_span_adj( struct fmt_span_adj *txt, struct prr0035t *pkt);


/*------------------------------------------------------*/
/*	�^�]�J�n�e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0037,prr0038					*/
/*------------------------------------------------------*/

struct fmt_prod_run {
	BYTE	cmd;			/* command = 'R'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_prod_run( struct fmt_prod_run *buf);
void *	pr_print_prod_run( struct fmt_prod_run *txt, struct prr0037t *pkt);


/*------------------------------------------------------*/
/*	�^�]��~�e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0039,prr0040					*/
/*------------------------------------------------------*/

struct fmt_prod_stop {
	BYTE	cmd;			/* command = 'S'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_prod_stop( struct fmt_prod_stop *buf);
void *	pr_print_prod_stop( struct fmt_prod_stop *txt, struct prr0039t *pkt);


/*------------------------------------------------------*/
/*	�S�r�o�J�n�e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0041,prr0042					*/
/*------------------------------------------------------*/

struct fmt_drain_run {
	BYTE	cmd;			/* command = 'G'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	head_ptn[8];		/* �r�o�w�b�h�r�b�g�p�^�[��	*/
/*-------------------------------------*/
};

int	pr_scan_drain_run( struct fmt_drain_run *buf);
void *	pr_print_drain_run( struct fmt_drain_run *txt, struct prr0041t *pkt);


/*------------------------------------------------------*/
/*	�S�r�o��~�e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0043,prr0044					*/
/*------------------------------------------------------*/

struct fmt_drain_stop {
	BYTE	cmd;			/* command = 'H'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_drain_stop( struct fmt_drain_stop *buf);
void *	pr_print_drain_stop( struct fmt_drain_stop *txt, struct prr0043t *pkt);


/*------------------------------------------------------*/
/*	�e�X�g�g�ݍ��킹�v�Z�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0045,prr0046					*/
/*------------------------------------------------------*/

struct fmt_test_calc {
	BYTE	cmd;			/* command = 'C'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_test_calc( struct fmt_test_calc *buf);
void *	pr_print_test_calc( struct fmt_test_calc *txt, struct prr0045t *pkt);


/*------------------------------------------------------*/
/*	�e�X�g�g�ݍ��킹�v�Z&�r�o�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0047,prr0048					*/
/*------------------------------------------------------*/

struct fmt_test_caldmp {
	BYTE	cmd;			/* command = 'M'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_test_caldmp( struct fmt_test_caldmp *buf);
void *	pr_print_test_caldmp( struct fmt_test_caldmp *txt, struct prr0047t *pkt);


/*------------------------------------------------------*/
/*	�G���[�����e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0049,prr0050					*/
/*------------------------------------------------------*/

struct fmt_error_cancel {
	BYTE	cmd;			/* command = 'D'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_error_cancel( struct fmt_error_cancel *buf);
void *	pr_print_error_cancel( struct fmt_error_cancel *txt, struct prr0049t *pkt);


/*------------------------------------------------------*/
/*	�e�X�g�쓮�J�n�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0051,prr0052					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_run {
	BYTE	cmd;			/* command = 'P'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	cf_ptn[2];		/* CF pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	rf_ptn[8];		/* RF pattern			*/
	BYTE	sht_ptn[8];		/* SHUTTER pattern		*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	wh2_ptn[8];		/* WH2 pattern(BH side)		*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	spa_ptn[8];		/* spare pattern		*/
	BYTE	drv_cyc;		/* Drive cycle			*/
	BYTE	sht_flg;		/* SHUTTER control flag		*/
	BYTE	spa_flg;		/* control flag			*/
/*-------------------------------------*/
};

int	pr_scan_tstdrv_run( struct fmt_tstdrv_run *buf);
void *	pr_print_tstdrv_run( struct fmt_tstdrv_run *txt, struct prr0051t *pkt);


/*------------------------------------------------------*/
/*	�e�X�g�쓮��~�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0053,prr0054					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_stop {
	BYTE	cmd;			/* command = 'Q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_tstdrv_stop( struct fmt_tstdrv_stop *buf);
void *	pr_print_tstdrv_stop( struct fmt_tstdrv_stop *txt, struct prr0053t *pkt);


/*------------------------------------------------------*/
/*	�d�d�o�q�n�l�[���N���A�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_eeprom_clr {
	BYTE	cmd;			/* command = 'z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*-------------------------------------*/
};


/*------------------------------------------------------*/
/*	�g�ݍ��킹�v�Z���ʃe�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0003,pri0004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	flag[2];		/* ���Z���ʃt���O		*/
	BYTE	weight[8];		/* �g�ݍ��킹�d��		*/
	BYTE	count[4];		/* �g�ݍ��킹��		*/
} PI00030;

struct fmt_cb_result {
	BYTE	cmd;			/* command = 'E'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PI00030	sect[8];		/* PI00030 paramter[nof_sect]	*/
	PI00030	ch;			/* PI00030   "			*/
	BYTE	multi_dump_seq_no[2];	/* �񐔌v�ʃJ�E���g		*/
	BYTE	slv_targ_wt[8];		/* �X���[�u�@�ڕW�d��		*/
	BYTE	slv_targ_cnt[4];	/* �X���[�u�@�ڕW��		*/
	BYTE	min_wh_no[2];		/* �ŏ�WH�ԍ�			*/
	BYTE	nof_wh[2];		/* �Y������ch��WH��		*/
	BYTE	wh_status[32][3];	/* WH�X�e�[�^�X[nof_wh]		*/
	BYTE	nof_bh[2];		/* �Y������ch��BH��		*/
	BYTE	bh_status[32][3];	/* WH�X�e�[�^�X[nof_bh]		*/
/*-------------------------------------*/
};

int	pr_scan_cb_result( struct fmt_cb_result *buf);
void *	pr_print_cb_result( struct fmt_cb_result *txt, struct pri0003t *pkt);


/*------------------------------------------------------*/
/*	�e�w�b�h�v�Z���ʃe�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0029,pri0030					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* �d��				*/
	BYTE	wh_status[3];		/* WH�X�e�[�^�X			*/
	BYTE	bh_status[3];		/* BH�X�e�[�^�X			*/
} PI00290;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PI00290	phead[2];		/* PI00290 paramter[nof_phead]	*/
} PI00291;

struct fmt_pandc_result {
	BYTE	cmd;			/* command = 'O'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PI00291	sect[8];		/* PI00291 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_pandc_result( struct fmt_pandc_result *buf);
void *	pr_print_pandc_result( struct fmt_pandc_result *txt, struct pri0029t *pkt);


/*------------------------------------------------------*/
/*	�P�d�����X�V���ʃe�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0005,pri0006					*/
/*------------------------------------------------------*/

struct fmt_piece_weight {
	BYTE	cmd;			/* command = 'p'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	BYTE	weight[8][4];		/* �P�d[nof_sect]		*/
/*-------------------------------------*/
};

int	pr_scan_piece_weight( struct fmt_piece_weight *buf);
void *	pr_print_piece_weight( struct fmt_piece_weight *txt, struct pri0005t *pkt);


/*------------------------------------------------------*/
/*	�␳�l�����X�V���ʃe�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0007,pri0008					*/
/*------------------------------------------------------*/

struct fmt_cmpn_value {
	BYTE	cmd;			/* command = 'q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	cmpn_val[2];		/* �␳�l			*/
/*-------------------------------------*/
};

int	pr_scan_cmpn_value( struct fmt_cmpn_value *buf);
void *	pr_print_cmpn_value( struct fmt_cmpn_value *txt, struct pri0007t *pkt);


/*------------------------------------------------------*/
/*	CF���䌋�ʃe�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0031,pri0032					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* ������Ԑ�			*/
	PR00190	at[8];			/* paramter[nof_sig]		*/
	BYTE	sig_stat[2];		/* �������			*/
	BYTE	df_wt[4];		/* �������U�d��			*/
} PI00310;

struct fmt_cf_rslt {
	BYTE	cmd;			/* command = 'k'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	seq_no;			/* �V�[�P���X�ԍ�		*/
	BYTE	nof_cf;			/* CF��				*/
	PI00310	cf[8];			/* PI00310 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	pr_scan_cf_rslt( struct fmt_cf_rslt *buf);
void *	pr_print_cf_rslt( struct fmt_cf_rslt *txt, struct pri0031t *pkt);


/*------------------------------------------------------*/
/*	�v���O�����ԍ��e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0009,pri0010					*/
/*------------------------------------------------------*/

struct fmt_prgno {
	BYTE	cmd;			/* command = 'Y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* �{�[�hID�ԍ�			*/
	BYTE	dev_no;			/* �f�o�C�X�ԍ�			*/
	BYTE	node_id[2];		/* �m�[�h�ԍ�			*/
	BYTE	len[2];			/* �h�c������̒���		*/
	BYTE	rom_id[128];		/* ROM ID������			*/
/*-------------------------------------*/
};

int	pr_scan_prgno( struct fmt_prgno *buf);
void *	pr_print_prgno( struct fmt_prgno *txt, struct pri0009t *pkt);


/*------------------------------------------------------*/
/*	���f�[�^�̃e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0011,pri0012					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* �w�b�h�d��			*/
	BYTE	status[3];		/* �w�b�h�X�e�[�^�X		*/
} PI00110;

struct fmt_head_weight {
	BYTE	cmd;			/* command = '\'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	flag;			/* real data or cal. data	*/
	BYTE	min_wh_no[2];		/* �ŏ�WH�ԍ�			*/
	BYTE	nof_wh[2];		/* �Y������ch��WH��		*/
	PI00110	wh[32];			/* WH�d��&�X�e�[�^�X[nof_wh]	*/
	BYTE	nof_bh[2];		/* �Y������ch��BH��		*/
	PI00110	bh[32];			/* WH�d��&�X�e�[�^�X[nof_bh]	*/
	BYTE	nof_ph[2];		/* �Y������ch��PH��		*/
	PI00110	ph[32];			/* WH�d��&�X�e�[�^�X[nof_ph]	*/
	BYTE	min_df_no;		/* �ŏ�DF�ԍ�			*/
	BYTE	nof_df;			/* �Y������ch��DF��		*/
	PI00110	df[8];			/* WH�d��&�X�e�[�^�X[nof_df]	*/
/*-------------------------------------*/
};

int	pr_scan_head_weight( struct fmt_head_weight *buf);
void *	pr_print_head_weight( struct fmt_head_weight *txt, struct pri0011t *pkt);


/*------------------------------------------------------*/
/*	�{�̂̏�ԃe�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0013,pri0014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	condition;		/* ���				*/
	BYTE	power_stat;		/* �쓮�n�d�����		*/
	BYTE	infeed_stat;		/* �������			*/
} PI00130;

struct fmt_condition {
	BYTE	cmd;			/* command = 'N'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	PI00130	ch[4];			/* PI00130 parameter of channel	*/
	BYTE	df_infeed_stat[2];	/* ���U�t�B�[�_�������		*/
/*-------------------------------------*/
};

int	pr_scan_condition( struct fmt_condition *buf);
void *	pr_print_condition( struct fmt_condition *txt, struct pri0013t *pkt);


/*------------------------------------------------------*/
/*	�x����̃e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0015,pri0016					*/
/*------------------------------------------------------*/

struct fmt_alarm {
	BYTE	cmd;			/* command = 'F'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	alarm[2];		/* �x����			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	afd[2];			/* AFD warning id		*/
/*-------------------------------------*/
};

int	pr_scan_alarm( struct fmt_alarm *buf);
void *	pr_print_alarm( struct fmt_alarm *txt, struct pri0015t *pkt);


/*------------------------------------------------------*/
/*	�ُ���̃e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0017,pri0018					*/
/*------------------------------------------------------*/

struct fmt_error {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	error[2];		/* �ُ���			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	duc_inf;		/* DUC error information	*/
	BYTE	exc_inf;		/* EXC error information	*/
/*-------------------------------------*/
};

int	pr_scan_error( struct fmt_error *buf);
void *	pr_print_error( struct fmt_error *txt, struct pri0017t *pkt);


/*------------------------------------------------------*/
/*	��M�����e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0019,pri0020					*/
/*------------------------------------------------------*/

struct fmt_echo {
	BYTE	cmd;			/* command = '='		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* ��M�R�}���h			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	pr_scan_echo( struct fmt_echo *buf);
void *	pr_print_echo( struct fmt_echo *txt, struct pri0019t *pkt);


/*------------------------------------------------------*/
/*	���슮���e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0021,pri0022					*/
/*------------------------------------------------------*/

struct fmt_complete {
	BYTE	cmd;			/* command = '>'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* ��M�R�}���h			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	pr_scan_complete( struct fmt_complete *buf);
void *	pr_print_complete( struct fmt_complete *txt, struct pri0021t *pkt);


/*------------------------------------------------------*/
/*	�V�X�e���G���[�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0023,pri0024					*/
/*------------------------------------------------------*/

struct fmt_sys_fault {
	BYTE	cmd;			/* command = '?'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	len[2];			/* ���b�Z�[�W�̃o�C�g��		*/
	BYTE	mes_str[64];		/* ���b�Z�[�W�E�X�g�����O�X	*/
/*-------------------------------------*/
};

int	pr_scan_sys_fault( struct fmt_sys_fault *buf);
void *	pr_print_sys_fault( struct fmt_sys_fault *txt, struct pri0023t *pkt);


/*------------------------------------------------------*/
/*	���ӈُ���̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0027,pri0028					*/
/*------------------------------------------------------*/

struct fmt_pcuerr {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	error;			/* �ُ���			*/
	BYTE	ptn[8];			/* pattern			*/
/*-------------------------------------*/
};

int	pr_scan_pcuerr( struct fmt_pcuerr *buf);
void *	pr_print_pcuerr( struct fmt_pcuerr *txt, struct pri0027t *pkt);


/*------------------------------------------------------*/
/*	�l�b�g���[�N�Q����Ԃ̗v���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0071,prr0072					*/
/*------------------------------------------------------*/

struct fmt_req_netmap {
	BYTE	cmd;			/* command = '['		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* �{�[�hID�ԍ�			*/
	BYTE	dev_no[2];		/* �f�o�C�X�ԍ�			*/
/*-------------------------------------*/
};

int	pr_scan_req_netmap( struct fmt_req_netmap *buf);
void *	pr_print_req_netmap( struct fmt_req_netmap *txt, struct prr0071t *pkt);


/*------------------------------------------------------*/
/*	�l�b�g���[�N�Q���󋵃e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0035,pri0036					*/
/*------------------------------------------------------*/

struct fmt_netmap {
	BYTE	cmd;			/* command = ']'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* �{�[�hID�ԍ�			*/
	BYTE	dev_no[2];		/* �f�o�C�X�ԍ�			*/
	BYTE	net_map[32][2];		/* �Q����ԃe�[�u��		*/
	BYTE	recon[4];		/* ���R���t�B�O������		*/
/*-------------------------------------*/
};

int	pr_scan_netmap( struct fmt_netmap *buf);
void *	pr_print_netmap( struct fmt_netmap *txt, struct pri0035t *pkt);


/*------------------------------------------------------*/
/*	�z�b�p�̊J�����̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0029,prr0030					*/
/*------------------------------------------------------*/

typedef struct {			/*----- mdp_section_data -------*/
	BYTE	amp_n[4];		/* �ŏI�J�ʒu(��]���)[%]	*/
	BYTE	sec_flag_power[4];	/* ����݂�ʰ���ܰ�ݒ�		*/
	BYTE	time_hold[4];		/* ΰ��ގ���[ms]		*/
	BYTE	num_ss_start[4];	/* �n���۰�ï�ߋ��(�ï�ߐ�)	*/
	BYTE	num_ss_stop[4];		/* �����۰�ï�ߋ��(�ï�ߐ�)	*/
	BYTE	max_pps[4];		/* ���x[pps]			*/
} PR00290;

typedef struct {			/* actuator_type == ACT_SM	*/
	BYTE	phase_type;		/* �㎥���[�h	3:1-2��		*/
	BYTE	full_pow_tm[4];		/* �ᑬ���t���p���[����(4000)	*/
	BYTE	ignore_pulse[4];	/* �X���b�g�s���p���X��		*/
	BYTE	cam_slit[2];		/* �X���b�g��~�p���X��		*/
	BYTE	hold_mode;		/* ��~�㎥����			*/
	BYTE	brake_time[2];		/* �u���[�L����[10msec]		*/
	BYTE	error_pulse[2];		/* �G���[���o�p���X��		*/
	BYTE	num_section[2];		/* �쓮����ݍ\����(����ݐ��j	*/
	BYTE	mode_flags[2];		/* �쓮�����Ӱ���׸�		*/
	BYTE	start_pps[4];		/* ���N���P����ٽ��[pps]	*/
	BYTE	fullstep[4];		/* �S�ï�ߐ�			*/
	BYTE	pitch[4];		/* (�\��)			*/
	PR00290	msd[10];		/* msd[num_section��]		*/
} PR00291;

typedef struct {			/* actuator_type == ACT_CB	*/
	BYTE	osc[4];			/* ��{��ٽ����(1us�P��)	*/
	BYTE	cl_drive_tm[4];		/* CL�쓮����			*/
	BYTE	cl_on_delay[2];		/* CL�I���x������		*/
	BYTE	br_on_delay[2];		/* BR�I���x������		*/
	BYTE	br_time[2];		/* BR�I������(10ms�P��)		*/
} PR00292;

typedef union {
	PR00291	sm;			/* �X�e�b�v����[�^�[�쓮�p	*/
	PR00292	cb;			/* �N���b�`�^�u���[�L�쓮�p	*/
} PR00293;

struct fmt_hop_spec2 {
	BYTE	cmd;			/* command = 'j'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	stop_mode;		/* ��~����			*/
	BYTE	actuator_type;		/* �A�N�`���G�[�^�^�C�v		*/
	PR00293	cpx;			/* �����쓮�p�����[�^		*/
/*-------------------------------------*/
};

int	pr_scan_hop_spec2( struct fmt_hop_spec2 *buf);
void *	pr_print_hop_spec2( struct fmt_hop_spec2 *txt, struct prr0029t *pkt);


/*------------------------------------------------------*/
/*	���f�f�f�[�^�v���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0031,prr0032					*/
/*------------------------------------------------------*/

struct fmt_diag_req {
	BYTE	cmd;			/* command = 'U'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_diag_req( struct fmt_diag_req *buf);
void *	pr_print_diag_req( struct fmt_diag_req *txt, struct prr0031t *pkt);


/*------------------------------------------------------*/
/*	�`�e�u�����e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0055,prr0056					*/
/*------------------------------------------------------*/

struct fmt_afv_adj {
	BYTE	cmd;			/* command = 'V'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	adj_mode[4];		/* adjust mode			*/
/*-------------------------------------*/
};

int	pr_scan_afv_adj( struct fmt_afv_adj *buf);
void *	pr_print_afv_adj( struct fmt_afv_adj *txt, struct prr0055t *pkt);


/*------------------------------------------------------*/
/*	�`�e�u�W���Ăяo���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_afv_recall {
	BYTE	cmd;			/* command = 'r'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*-------------------------------------*/
};


/*------------------------------------------------------*/
/*	�w�b�h�̏d�S�̍��W�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0061,prr0062					*/
/*------------------------------------------------------*/

struct fmt_afv_coord {
	BYTE	cmd;			/* command = 'y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* �`�c�b�{�[�h���ʔԍ�		*/
	BYTE	coord_x[28][4];		/* �v�ʊ��AFV�̏d�S�̂w���W	*/
	BYTE	coord_y[28][4];		/* �v�ʊ��AFV�̏d�S�̂x���W	*/
	BYTE	afv_coeffi[24][8];	/* �`�e�u�W��			*/
/*-------------------------------------*/
};

int	pr_scan_afv_coord( struct fmt_afv_coord *buf);
void *	pr_print_afv_coord( struct fmt_afv_coord *txt, struct prr0061t *pkt);


/*------------------------------------------------------*/
/*	���f�f�f�[�^�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0001,pri0002					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* �w�b�h�d��			*/
	BYTE	status[3];		/* �w�b�h�X�e�[�^�X		*/
} PI00010;

struct fmt_diag_data {
	BYTE	cmd;			/* command = 'u'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* �ŏ�WH�ԍ�			*/
	BYTE	nof_wh[2];		/* �Y������ch��WH��		*/
	PI00010	wh_3rd[32];		/* 3�i�ڏd��&�X�e�[�^�X[nof_wh]	*/
	PI00010	wh_4th[32];		/* 4�i�ڏd��&�X�e�[�^�X[nof_wh]	*/
	PI00010	wh_5th[32];		/* 5�i�ڏd��&�X�e�[�^�X[nof_wh]	*/
	BYTE	nof_ph[2];		/* �Y������ch��PH��		*/
	PI00010	ph_3rd[32];		/* 3�i�ڏd��&�X�e�[�^�X[nof_ph]	*/
	PI00010	ph_4th[32];		/* 4�i�ڏd��&�X�e�[�^�X[nof_ph]	*/
	PI00010	ph_5th[32];		/* 5�i�ڏd��&�X�e�[�^�X[nof_ph]	*/
	BYTE	min_df_no;		/* �ŏ�DF�ԍ�			*/
	BYTE	nof_df;			/* �Y������ch��DF��		*/
	PI00010	df[8];			/* WH�d��&�X�e�[�^�X[nof_df]	*/
	BYTE	nof_afv[2];		/* �`�e�u�Z����			*/
	BYTE	afv_out[16][4];		/* �`�e�u�Z���o��[nof_afv]	*/
/*-------------------------------------*/
};

int	pr_scan_diag_data( struct fmt_diag_data *buf);
void *	pr_print_diag_data( struct fmt_diag_data *txt, struct pri0001t *pkt);


/*------------------------------------------------------*/
/*	�`�e�u�W���e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* pri0047,pri0048					*/
/*------------------------------------------------------*/

struct fmt_afv_coeffi {
	BYTE	cmd;			/* command = 'v'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* �Y������WH�ԍ�		*/
	BYTE	new_coeffi[8];		/* �V�`�e�u�W��			*/
	BYTE	old_coeffi[8];		/* ���`�e�u�W��			*/
/*-------------------------------------*/
};

int	pr_scan_afv_coeffi( struct fmt_afv_coeffi *buf);
void *	pr_print_afv_coeffi( struct fmt_afv_coeffi *txt, struct pri0047t *pkt);


/*------------------------------------------------------*/
/*	�e��f�[�^�̗v���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0101,prr0102					*/
/*------------------------------------------------------*/

struct fmt_req_info {
	BYTE	cmd;			/* command = '.'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	info_no[2];		/* �f�[�^�̎��			*/
	BYTE	info_desc[2];		/* ���ʔԍ�			*/
/*-------------------------------------*/
};

int	pr_scan_req_info( struct fmt_req_info *buf);
void *	pr_print_req_info( struct fmt_req_info *txt, struct prr0101t *pkt);

/*------------------------------------------------------*/
/*	�e��f�[�^�̉����e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0025,pri0026					*/
/*------------------------------------------------------*/

struct fmt_info {
	BYTE	cmd;			/* command = '/'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	info_no[2];		/* �f�[�^�̎��			*/
	BYTE	info_desc[2];		/* ���ʔԍ�			*/
	BYTE	nof_data[2];		/* �f�[�^��			*/
	BYTE	data[32][8];		/* �f�[�^			*/
/*-------------------------------------*/
};

int	pr_scan_info( struct fmt_info *buf);
void *	pr_print_info( struct fmt_info *txt, struct pri0025t *pkt);


/*------------------------------------------------------*/
/*	���W�b�N�g�`�f�[�^�̗v���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0103,prr0104					*/
/*------------------------------------------------------*/

struct fmt_logic_parm {
	BYTE	cmd;			/* command = '0'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	logic_intvl[4];		/* �T���v�����O����		*/
	BYTE	unit_no;		/* �����[���j�b�g�ԍ�		*/
/*-------------------------------------*/
};

int	pr_scan_logic_parm( struct fmt_logic_parm *buf);
void *	pr_print_logic_parm( struct fmt_logic_parm *txt, struct prr0103t *pkt);


/*------------------------------------------------------*/
/*	���W�b�N�g�`�f�[�^�̉����e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0049,pri0050					*/
/*------------------------------------------------------*/

struct fmt_logic {
	BYTE	cmd;			/* command = '1'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	unit_no;		/* �f�[�^�̎��			*/
	BYTE	nof_data[2];		/* �f�[�^��			*/
	BYTE	data[32][4];		/* �f�[�^			*/
/*-------------------------------------*/
};

int	pr_scan_logic( struct fmt_logic *buf);
void *	pr_print_logic( struct fmt_logic *txt, struct pri0049t *pkt);


/*------------------------------------------------------*/
/*	�d�ʔg�`�f�[�^�̃p�����[�^�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0105,prr0106					*/
/*------------------------------------------------------*/

struct fmt_archive_parm {
	BYTE	cmd;			/* command = '2'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* WH�ԍ�			*/
	BYTE	type;			/* �d�ʃf�[�^�̃^�C�v		*/
	BYTE	hopper_id;		/* WH/PH���ʃt���O		*/
/*-------------------------------------*/
};

int	pr_scan_archive_parm( struct fmt_archive_parm *buf);
void *	pr_print_archive_parm( struct fmt_archive_parm *txt, struct prr0105t *pkt);


/*------------------------------------------------------*/
/*	�d�ʔg�`�f�[�^�̉����e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0053,pri0054					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE	weight[4];		/* �d�ʃf�[�^			*/
	BYTE	status;			/* �d�ʃX�e�[�^�X		*/
} PI00530;

struct fmt_archive {
	BYTE	cmd;			/* command = '3'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* WH/PH���ʃt���O		*/
	BYTE	wh_no[2];		/* WH�ԍ�			*/
	BYTE	index[4];		/* �f�[�^�̃C���f�b�N�X		*/
	BYTE	nof_data[2];		/* �f�[�^��			*/
	PI00530	data[32];		/* �f�[�^			*/
/*-------------------------------------*/
};

int	pr_scan_archive( struct fmt_archive *buf);
void *	pr_print_archive( struct fmt_archive *txt, struct pri0053t *pkt);


/*------------------------------------------------------*/
/*	�t�B�[�_���U�����v���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_res_cyc_req {
	BYTE	cmd;			/* command = '4'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};


/*------------------------------------------------------*/
/*	�t�B�[�_���U���������e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0051,pri0052					*/
/*------------------------------------------------------*/

struct fmt_resonance {
	BYTE	cmd;			/* command = '5'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RF�̐�			*/
	BYTE	rf_res_cycle[32][4];	/* RF�̋��U����			*/
	BYTE	nof_df[2];		/* DF�̐�			*/
	BYTE	df_res_cycle[8][4];	/* DF�̋��U����			*/
	BYTE	nof_af[2];		/* AF�̐�			*/
	BYTE	af_res_cycle[32][4];	/* AF�̋��U����			*/
	BYTE	nof_cf[2];		/* CF�̐�			*/
	BYTE	cf_res_cycle[8][4];	/* CF�̋��U����			*/
/*-------------------------------------*/
};

int	pr_scan_resonance( struct fmt_resonance *buf);
void *	pr_print_resonance( struct fmt_resonance *txt, struct pri0051t *pkt);


/*------------------------------------------------------*/
/*	�t�B�[�_���U�������o�J�n�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0107,prrr0108					*/
/*------------------------------------------------------*/

struct fmt_resonance_detect {
	BYTE	cmd;			/* command = '6'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	rf_detect_ptn[8];	/* RF���U�������o�p�^�[��	*/
	BYTE	df_detect_ptn[8];	/* DF���U�������o�p�^�[��	*/
	BYTE	af_detect_ptn[8];	/* AF���U�������o�p�^�[��	*/
	BYTE	cf_detect_ptn[8];	/* CF���U�������o�p�^�[��	*/
/*-------------------------------------*/
};

int	pr_scan_res_detect( struct fmt_resonance_detect *buf);
void *	pr_print_res_detect( struct fmt_resonance_detect *txt, struct prr0107t *pkt);


/*------------------------------------------------------*/
/*	�t�B�[�_���U�����v���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_res_cyc_stop {
	BYTE	cmd;			/* command = '7'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};


/*------------------------------------------------------*/
/*	�t�B�[�_�쓮�����ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0109,prr0110					*/
/*------------------------------------------------------*/

struct fmt_drv_cycle {
	BYTE	cmd;			/* command = '8'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RF�̐�			*/
	BYTE	rf_drv_cycle[32][4];	/* RF�̋쓮����			*/
	BYTE	nof_df[2];		/* DF�̐�			*/
	BYTE	df_drv_cycle[8][4];	/* DF�̋쓮����			*/
	BYTE	nof_af[2];		/* AF�̐�			*/
	BYTE	af_drv_cycle[32][4];	/* AF�̋쓮����			*/
	BYTE	nof_cf[2];		/* CF�̐�			*/
	BYTE	cf_drv_cycle[8][4];	/* CF�̋쓮����			*/
/*-------------------------------------*/
};

int	pr_scan_drv_cycle( struct fmt_drv_cycle *buf);
void *	pr_print_drv_cycle( struct fmt_drv_cycle *txt, struct prr0109t *pkt);


/*------------------------------------------------------*/
/*	�t�B�[�_���U�����ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0111,prr0112					*/
/*------------------------------------------------------*/

struct fmt_natural_freq {
	BYTE	cmd;			/* command = '9'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RF�̐�			*/
	BYTE	rf_natural_freq[32][4];	/* RF�̋��U����			*/
	BYTE	nof_df[2];		/* DF�̐�			*/
	BYTE	df_natural_freq[8][4];	/* DF�̋��U����			*/
	BYTE	nof_af[2];		/* AF�̐�			*/
	BYTE	af_natural_freq[32][4];	/* AF�̋��U����			*/
	BYTE	nof_cf[2];		/* CF�̐�			*/
	BYTE	cf_natural_freq[8][4];	/* CF�̋��U����			*/
/*-------------------------------------*/
};

int	pr_scan_natural_freq( struct fmt_natural_freq *buf);
void *	pr_print_natural_freq( struct fmt_natural_freq *txt, struct prr0111t *pkt);


/*------------------------------------------------------*/
/*	�T���v���r�o�̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0069,prr0070					*/
/*------------------------------------------------------*/

struct fmt_sample_dump {
	BYTE	cmd;			/* command = ':'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	sect_assign_ptn[2];	/* section pattern		*/
/*-------------------------------------*/
};

int	pr_scan_sample_dump( struct fmt_sample_dump *buf);
void *	pr_print_sample_dump( struct fmt_sample_dump *txt, struct prr0069t *pkt);


/*------------------------------------------------------*/
/*	�X�p���W���v���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0085,prr0086					*/
/*------------------------------------------------------*/

struct fmt_span_call {
	BYTE	cmd;			/* command = 'J'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_no;			/* �`�c�b�{�[�h���ʔԍ�		*/
	BYTE	range;			/* ����				*/
/*-------------------------------------*/
};

int	pr_scan_span_call( struct fmt_span_call *buf);
void *	pr_print_span_call( struct fmt_span_call *txt, struct prr0085t *pkt);


/*------------------------------------------------------*/
/*	�X�p���W�������e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0043,pri0044					*/
/*------------------------------------------------------*/

struct fmt_span_coeffi {
	BYTE	cmd;			/* command = 'K'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* �`�c�b�{�[�h���ʔԍ�		*/
	BYTE	range;			/* ����				*/
	BYTE	date[8];		/* �������t			*/
	BYTE	time[8];		/* ��������			*/
	BYTE	span_coeffi[24][4];	/* �r�o�`�m�W��			*/
/*-------------------------------------*/
};

int	pr_scan_span_coeffi( struct fmt_span_coeffi *buf);
void *	pr_print_span_coeffi( struct fmt_span_coeffi *txt, struct pri0043t *pkt);


/*------------------------------------------------------*/
/*	�t�����u�p�����[�^�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0063,prr0064					*/
/*------------------------------------------------------*/

struct updown_chute_parm {
	BYTE	cmd;			/* command = '@'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dev_no;			/* ���u�ԍ�			*/
	BYTE	nof_pos[2];		/* �|�W�V������			*/
	BYTE	err_detect_interval[2];	/* �G���[���o���ԁi*10ms�j	*/
	BYTE	position1[2];		/* �ݒ�ʒu�P			*/
	BYTE	position2[2];		/* �ݒ�ʒu�Q			*/
/*-------------------------------------*/
};

int	pr_scan_updown_chute(struct updown_chute_parm *buf);
void *	pr_print_updown_chute(struct updown_chute_parm *txt, struct prr0063t *pkt);


/*------------------------------------------------------*/
/*	��������M��ON/OFF�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0113,prr0114					*/
/*------------------------------------------------------*/

struct fmt_infonoff {
	BYTE	cmd;			/* command = 'l'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	onoff_key;		/* infeed ON/OFF		*/
/*-------------------------------------*/
};

int	pr_scan_infonoff( struct fmt_infonoff *buf);
void *	pr_print_infonoff( struct fmt_infonoff *txt, struct prr0113t *pkt);


/*------------------------------------------------------*/
/*	�S�r�o���̎�����_����e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0115,prr0116					*/
/*------------------------------------------------------*/

struct fmt_drain_zero {
	BYTE	cmd;			/* command = 'm'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	drain_zero;		/* drain auto zero flag		*/
/*-------------------------------------*/
};

int	pr_scan_drain_zero( struct fmt_drain_zero *buf);
void *	pr_print_drain_zero( struct fmt_drain_zero *txt, struct prr0115t *pkt);


/*------------------------------------------------------*/
/*	�M���o�͐�̃A�T�C���e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0119,prr0120					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE	sig_assign[2];		/* signal assign		*/
} PR01190;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR01190	phead[2];		/* parameter[nof_phead]		*/
} PR01191;

typedef struct {
	BYTE	nof_sect;		/* number of section		*/
	PR01191	sect[8];		/* parameter[nof_sect]		*/
} PR01192;

struct fmt_sig_assign {
	BYTE	cmd;			/* command = 'B'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_infsig;		/* number of infeed signal	*/
	BYTE	infsig_assign[8][2];	/* infeed signal assign		*/
	BYTE	lowp_sig_assign[8][2];	/* low product signal assign	*/
	BYTE	nof_df;			/* number of DF			*/
	BYTE	dfsync_sig_assign[8][2];/* DF sync. signal assign	*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	dumpsig_assign[4][2];	/* dump signal assign		*/
	BYTE	pr_dumpsig_assign[4][2];/* proper dump signal assign	*/
	BYTE	ng_dumpsig_assign[4][2];/* NG dump signal assign	*/
	BYTE	pr_readysig_assign[4][2];/* proper ready signal assign	*/
	BYTE	ng_readysig_assign[4][2];/* NG ready signal assign	*/
	BYTE	pr_each_dpsig_assign[4][2];/* proper each dump sig. assign*/
	BYTE	ng_each_dpsig_assign[4][2];/* NG each dump signal assign*/
	BYTE	pr_addsig_assign[4][2];	/* proper addition signal assign*/
	BYTE	ng_addsig_assign[4][2];	/* NG addition signal assign	*/
	BYTE	error_sig_assign[4][2];	/* error signal assign		*/
	BYTE	control_sig_assign[4][2];/* control signal assign	*/
	BYTE	prod_sig_assign[4][2];	/* production signal assign	*/
	BYTE	drain_sig_assign[4][2];	/* drain signal assign		*/
	BYTE	manuzero_sig_assign[4][2];/* manual zero adj. sig. assign*/
	BYTE	error_rej_sig_assign[4][2];/* error reject sig. assign	*/
	BYTE	lowp_stop_sig_assign[4][2];/* low product stop sig. assign*/
	BYTE	metal_det_sig_assign[4][2];/* metal detect signal assign*/
	PR01192	pandc[4];		/* parent parameter		*/
/*-------------------------------------*/
};

int	pr_scan_sig_assign_parm( struct fmt_sig_assign *buf);
void *	pr_print_sig_assign_parm( struct fmt_sig_assign *txt, struct prr0119t *pkt);


/*------------------------------------------------------*/
/*	�����[��̏o�̓`�F�b�N�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0121,prr0122					*/
/*------------------------------------------------------*/

struct fmt_ryout_check {
	BYTE	cmd;			/* command = '$'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* �����[��ԍ�		*/
	BYTE	drive_ptn[2];		/* �쓮�����[�p�^�[��		*/
	BYTE	drive_delay[2];		/* �쓮�x������[10msec]		*/
	BYTE	drive_time[2];		/* �쓮����[10msec,0=OFF,255=ON]*/
/*-------------------------------------*/
};

int	pr_scan_ryout_check( struct fmt_ryout_check *buf);
void *	pr_print_ryout_check( struct fmt_ryout_check *txt, struct prr0121t *pkt);


/*------------------------------------------------------*/
/*	�����[��̓��̓`�F�b�N�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0123,prr0124					*/
/*------------------------------------------------------*/

struct fmt_ryin_check {
	BYTE	cmd;			/* command = ';'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* �����[��ԍ�		*/
/*-------------------------------------*/
};

int	pr_scan_ryin_check( struct fmt_ryin_check *buf);
void *	pr_print_ryin_check( struct fmt_ryin_check *txt, struct prr0123t *pkt);


/*------------------------------------------------------*/
/*	�����[��̓��͏�ԉ����e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0055,pri0056					*/
/*------------------------------------------------------*/

struct fmt_ryin {
	BYTE	cmd;			/* command = ';'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* �����[��ԍ�		*/
	BYTE	input_ptn[2];		/* ���͏�ԃp�^�[��		*/
/*-------------------------------------*/
};

int	pr_scan_ryin( struct fmt_ryin *buf);
void *	pr_print_ryin( struct fmt_ryin *txt, struct pri0055t *pkt);


/*------------------------------------------------------*/
/*	�z�b�p�쓮�񐔗v���e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0125,prr0126					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_req {
	BYTE	cmd;			/* command = '#'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* �z�b�p���ʃt���O		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_req( struct fmt_hop_drive_req *buf);
void *	pr_print_hop_drive_req( struct fmt_hop_drive_req *txt, struct prr0125t *pkt);


/*------------------------------------------------------*/
/*	�z�b�p�쓮�񐔉����e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0057,pri0058					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_no {
	BYTE	cmd;			/* command = '%'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* �z�b�p���ʃt���O		*/
	BYTE	nof_hopper[2];		/* �z�b�p��			*/
	BYTE	nof_times[32][8];	/* �z�b�p�쓮��		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_no( struct fmt_hop_drive_no *buf);
void *	pr_print_hop_drive_no( struct fmt_hop_drive_no *txt, struct pri0057t *pkt);

/*------------------------------------------------------*/
/*	��ԕ\�����̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* pri0061,pri0062					*/
/*------------------------------------------------------*/

struct fmt_dspinf {
	BYTE	cmd;			/* command = '�'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	info[2];		/* �\�����			*/
	BYTE	hp_ptn[8];		/* Hopper pattern		*/
/*-------------------------------------*/
};

int	pr_scan_dsp_info( struct fmt_dspinf *buf);
void *	pr_print_dsp_info( struct fmt_dspinf *txt, struct pri0061t *pkt);


/*------------------------------------------------------*/
/*	�z�b�p�쓮�񐔐ݒ�e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0127,pry0128					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_set {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* �z�b�p���ʃt���O		*/
	BYTE	hopper_no[2];		/* �z�b�p�ԍ�			*/
	BYTE	nof_times[8];		/* �z�b�p�쓮��		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_set( struct fmt_hop_drive_set *buf);
void *	pr_print_hop_drive_set( struct fmt_hop_drive_set *txt, struct prr0127t *pkt);


/*--------------------------------------------------------------*/
/*	�^�]���Ƀz�b�p�����O���Ă̗�_�e�L�X�g�t�H�[�}�b�g	*/
/*								*/
/* prr0129,pry0130						*/
/*--------------------------------------------------------------*/

struct fmt_fzero {
	BYTE	cmd;			/* command = '\''		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_nonactive[8];	/* WH non active head	*/
	BYTE	ph_nonactive[8];	/* PH non active head	*/
/*-------------------------------------*/
};

int	pr_scan_fzero( struct fmt_fzero *buf);
void *	pr_print_fzero( struct fmt_fzero *txt, struct prr0129t *pkt);


/*------------------------------------------------------*/
/*	�\��ԍ��e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0131,prr0132					*/
/*------------------------------------------------------*/

struct fmt_preset_no {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	preset_no[4];		/* preset number		*/
/*-------------------------------------*/
};

int	pr_scan_preset_no( struct fmt_preset_no *buf);
void *	pr_print_preset_no( struct fmt_preset_no *txt, struct prr0131t *pkt);

/*------------------------------------------------------*/
/*	�������o�@�e�X�g�̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0067,prr0068					*/
/*------------------------------------------------------*/

struct fmt_metal_text {
	BYTE	cmd;			/* command = '{'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	function;		/* 0:�e�X�g���� 1:�e�X�g�J�n	*/
/*-------------------------------------*/
};

int	pr_scan_metal_text( struct fmt_metal_text *buf);
void *	pr_print_metal_text( struct fmt_metal_text *txt, struct prr0067t *pkt);

/*------------------------------------------------------*/
/*	�������o�@�e�X�g���ʂ̃e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* pri0033,pri0034					*/
/*------------------------------------------------------*/

struct fmt_metal_answer {
	BYTE	cmd;			/* command = '}'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	answer;			/* 0:���� 1:���s 2:���~ 3:SD���~*/
/*-------------------------------------*/
};

int	pr_scan_metal_answer( struct fmt_metal_answer *buf);
void *	pr_print_metal_answer( struct fmt_metal_answer *txt, struct pri0033t *pkt);

/*----------------------------------------------------------------------*/
/*	�e�w�b�h�̒x�ꎞ�Ԋg���^�C�~���O�̐ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*									*/
/* prr1017,prr1018							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	parent_delay[2];	/* �e�w�b�h�x�ꎞ��		*/
} PR10170;

typedef struct {
	BYTE	nof_parent;		/* �Y������ch�̃Z�N�V������	*/
	PR10170	pandc[2];		/* paramter[nof_parent]		*/
} PR10171;

struct fmt_parent_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR10171	sect[8];		/* P4 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_parent_tmg( struct fmt_parent_tmg *buf);
void *	pr_print_parent_tmg( struct fmt_parent_tmg *txt, struct prr1017t *pkt);

/*----------------------------------------------------------------------*/
/*  �c�b�`�p�����[�^�R�}���h�̃e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* prr0075,prr0076							*/
/*----------------------------------------------------------------------*/

struct fmt_dca_parm {
	BYTE	cmd;			/* command = '!'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_on_off;		/* DCA ON/OFF			*/
	BYTE	dca_cycle[4];		/* DCA cycle count		*/
	BYTE	dca_owf[2];		/* DCA OWF			*/
	BYTE	dca_sdm[2];		/* DCA SDM			*/
/*-------------------------------------*/
};

int	pr_scan_dca_parm( struct fmt_dca_parm *buf);
void *	pr_print_dca_parm( struct fmt_dca_parm *txt, struct prr0075t *pkt);


/*----------------------------------------------------------------------*/
/*  �`�s�`�΍��̃e�L�X�g�t�H�[�}�b�g					*/
/*									*/
/* prr0077,prr0078							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_dev {
	BYTE	cmd;			/* command = '"'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_dev[8];		/* ATA deviation		*/
/*-------------------------------------*/
};

int	pr_scan_ata_dev( struct fmt_ata_dev *buf);
void *	pr_print_ata_dev( struct fmt_ata_dev *txt, struct prr0077t *pkt);


/*----------------------------------------------------------------------*/
/*	�c�b�`���j�^�[���̃e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* pri0037,pri0038							*/
/*----------------------------------------------------------------------*/

struct fmt_dca_monitor {
	BYTE	cmd;			/* command = '('		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_sum_cnt[4];		/* DCA�T���v���J�E���g�l	*/
	BYTE	bf_cbwt[8];		/* �ă`�F�b�N�O�d��		*/
	BYTE	af_cbwt[8];		/* �ă`�F�b�N��d��		*/
/*-------------------------------------*/
};

int	pr_scan_dca_monitor( struct fmt_dca_monitor *buf);
void *	pr_print_dca_monitor( struct fmt_dca_monitor *txt, struct pri0037t *pkt);


/*----------------------------------------------------------------------*/
/*	�`�s�`���j�^�[���̃e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* pri0039,pri0040							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_monitor {
	BYTE	cmd;			/* command = ')'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_enable;		/* �`�s�`  enable		*/
	BYTE	ata_timer[4];		/* �`�s�`�^�C�}�[�l		*/
	BYTE	ata_mode;		/* ���݂̂`�s�`���[�h		*/
	BYTE	ata_tgt[8];		/* �`�s�`�^�[�Q�b�g		*/
	BYTE	ata_def_time[4];	/* ATA default time		*/
	BYTE	ata_cwb[4];		/* �b�v�a			*/
/*-------------------------------------*/
};

int	pr_scan_ata_monitor( struct fmt_ata_monitor *buf);
void *	pr_print_ata_monitor( struct fmt_ata_monitor *txt, struct pri0039t *pkt);


/*----------------------------------------------------------------------*/
/*    �g�ݍ��킹�v�Z�g���p�����[�^�e�L�X�g�t�H�[�}�b�g			*/
/*    �l�`�w  �r�b�`�k�d						*/
/* prr3011,prr3012							*/
/*----------------------------------------------------------------------*/

struct fmt_max_scale {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	max_scale;		/* max scale			*/
/*-------------------------------------*/
};

int	pr_scan_max_scale( struct fmt_max_scale *buf);
void *	pr_print_max_scale( struct fmt_max_scale *txt, struct prr3011t *pkt);


/*----------------------------------------------------------------------*/
/*   �{�̏�ԏ��g���p�����[�^�e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* pri1013,pri1014							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_target {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	ata_target[4][8];	/* ata target			*/
/*-------------------------------------*/
};

int	pr_scan_ata_target(struct fmt_ata_target *buf);
void *	pr_print_ata_target(struct fmt_ata_target *txt,struct pri1013t *pkt);


/*----------------------------------------------------------------------*/
/*	�`�s�`98���j�^�[���  �e�L�X�g�t�H�[�}�b�g			*/
/*	�X�e�C���E���b�N�^�C���E�e�L�X�g				*/
/*									*/
/* pri2039,pri2040							*/
/*----------------------------------------------------------------------*/
struct fmt_stale98_time {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stale98_time[4];	/* �X�e�C���E���b�N�E�^�C��	*/
	BYTE	stale98_mode;		/* ���݂̏�� (stale_mode)	*/
	BYTE	stale98_tmr[4];		/* �X�e�C���^�C�}�A�h���C���^�C�}*/
	BYTE	stale98_dcnt[2];	/* �_�E���J�E���g�l		*/
	BYTE	stale98_SDEnable;	/* �z�X�g����ݒ肳�ꂽStale Drain Enable */
	BYTE	stale98_drain_mode;	/* ���݂̏�ԁiDrain Mode)	*/
	BYTE	stale98_drain_tmr[4];	/* STALE DRAIN CLEAR TIME�̶����޳�*/
					/* ��ڲ�Ӱ�ނ�STALE_DRAIN_START���L��*/
/*-------------------------------------*/
};

int	pr_scan_stale98_time( struct fmt_stale98_time *buf);
void *	pr_print_stale98_time( struct fmt_stale98_time *txt, struct pri2039t *pkt);


/*----------------------------------------------------------------------*/
/*	�v�ʖڕW�l�g���p�����[�^�e�L�X�g�t�H�[�}�b�g			*/
/*	�`�s�`  �r�`�l�o�k�d  �r�h�y�d					*/
/* prr4023,prr4024							*/
/*----------------------------------------------------------------------*/

struct fmt_sample_size {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	sample_size[4];		/* ata sample size		*/
	BYTE	preset_num[2];		/* �v���Z�b�g�ԍ�		*/
/*-------------------------------------*/
};

int	pr_scan_sample_size( struct fmt_sample_size *buf);
void *	pr_print_sample_size( struct fmt_sample_size *txt, struct prr4023t *pkt);


/*------------------------------------------------------*/
/*	�V�`�e�c�̃e�L�X�g�t�H�[�}�b�g			*/
/*							*/
/* prr0133,prr0134					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	tbl[15][2];		/* fuzzy table			*/
	BYTE	zero_d_num[2];		/* �󌟏o�������		*/
	BYTE	p_os_c_num[2];		/* �I�[�o�[�X�P�[�����o�������	*/
	BYTE	trb_off_head[2];	/* �w�b�h���ቺ��~�w�b�h��	*/
	BYTE	cln_req_sw[2];		/* ���|�v�����m			*/
	BYTE	good_effi[4];		/* ���ғ�������l		*/
} PR01330;

typedef struct {
	BYTE	nof_sect;	/* �Y������ch�̃Z�N�V������		*/
	PR01330	sect[8];	/* AFD parameter[nof_sect]		*/
} PR01331;

typedef struct {
	BYTE	df_fuzzy_gain[4];	/* �t�@�W�[�e�[�u������Q�C��		*/
	BYTE	df_l_detect[4];		/* ���U���싖�e�T�C�N����		*/
	BYTE	df_ow_ctrl[2];		/* �����ߑ��Ԏ��ԑ���			*/
	BYTE	dfw_on_off[2];		/* ���U�d�ʐ���ON/OFF			*/
	BYTE	df_e_detect1[2];	/* ���U���싖�e��w�b�h���i�o���c�L���j	*/
	BYTE	df_e_detect2[2];	/* ���U���싖�e��w�b�h���i�o���c�L��j	*/
	BYTE	df_w_max[4];		/* ���U�d�ʐ���ő�l			*/
	BYTE	df_w_min[4];		/* ���U�d�ʐ���ŏ��l			*/
	BYTE	low_wt[2];		/* AFD�����~���U�d��			*/
} PR01332;

struct fmt_adv_afd {
	BYTE	cmd;			/* command = '+'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channnel		*/
	PR01331	ch[4];			/* channel parameter		*/
	BYTE	nof_infeed;		/* number of infeed		*/
	PR01332	ifd[8];			/* PR01332 paramter[nof_infeed]	*/
/*-------------------------------------*/
};

int	pr_scan_adv_afd( struct fmt_adv_afd *buf);
void *	pr_print_adv_afd( struct fmt_adv_afd *txt, struct prr0133t *pkt);


/*----------------------------------------------------------------------*/
/*    �o�g�Z���f�[�^�e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* prr2009,prr2010							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	stick_weight[4];	/* stick weight			*/
	BYTE	ph_weight_check;	/* PH�̏d�ʊĎ�			*/
} PR20091;

typedef struct {
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR20091	sect[8];		/* PR00091 paramter[nof_sect]	*/
} PR20090;

struct fmt_ph_cell {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ph_span_adj;		/* PH span adjustment		*/
	BYTE	nof_ch;			/* number of channnel		*/
	PR20090	ch[4];			/* channel parameter		*/
/*-------------------------------------*/
};

int	pr_scan_ph_cell( struct fmt_ph_cell *buf);
void *	pr_print_ph_cell( struct fmt_ph_cell *txt, struct prr2009t *pkt);


/*----------------------------------------------------------------------*/
/*    �Z�~�I�[�g�@�v�ʃf�[�^�e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* prr6023,prr6024							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	wh_async_supply;	/* WH�ւ̔񓯊�����		*/
	BYTE	only_bh_cal;		/* BH�݂̂őg�ݍ��킹�v�Z	*/
	BYTE	bh_async_supply;	/* BH�ւ̔񓯊�����		*/
} PR60230;

struct fmt_semi_auto {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	dump_dir;		/* dump direction		*/
	BYTE	conv_rev;		/* �W���R���x�����t�]����	*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR60230	sect[8];		/* PR60230 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_semi_auto( struct fmt_semi_auto *buf);
void *	pr_print_semi_auto( struct fmt_semi_auto *txt, struct prr6023t *pkt);

/*----------------------------------------------------------------------*/
/* �}���`�A���R���f�B�V�����e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* pri0045,pri0046							*/
/*----------------------------------------------------------------------*/

typedef union {
	BYTE		c_data[32];	/* �ėp�f�[�^			*/
	BYTE		i_data[32];	/* �ėp�f�[�^			*/
	BYTE		l_data[32];	/* �ėp�f�[�^			*/
} PI00450;

typedef struct {
	BYTE		data_cnt[2];	/* �f�[�^�o�C�g��		*/
	PI00450		dt;		/* �ėp�f�[�^			*/
} PI00451;

typedef struct {
	BYTE		disp_cnt[2];	/* �f�[�^�o�C�g��		*/
	BYTE		disp_str[128];	/* �\���f�[�^			*/
} PI00452;

typedef struct {
	BYTE		prn_cnt[2];	/* �f�[�^�o�C�g��		*/
	BYTE		prn_str[32];	/* �󎚃f�[�^			*/
} PI00453;

struct fmt_mlt_cond {
	BYTE	cmd;			/* command = '<'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	cond_inf[2];		/* ��ԓ��e			*/
	BYTE	act_inf[2];		/* �q�b�t������		*/
	BYTE	node_ptn[8];		/* error node pattern		*/
	BYTE	error_no[4];		/* �G���[���			*/
	PI00451	ex_d;			/* �ėp�f�[�^			*/
	PI00452	ds_d;			/* �\���f�[�^			*/
	PI00453	pr_d;			/* �󎚃f�[�^			*/
/*-------------------------------------*/
};

int	pr_scan_mlt_cond( struct fmt_mlt_cond *buf);
void *	pr_print_mlt_cond( struct fmt_mlt_cond *txt, struct pri0045t *pkt);

/*----------------------------------------------------------------------*/
/* �}���`�A���p�����[�^1���N�G�X�g�e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* prr0089,prr0090							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_p1req {
	BYTE	cmd;			/* command = '_'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_mlt_p1req( struct fmt_mlt_p1req *buf);
void *	pr_print_mlt_p1req( struct fmt_mlt_p1req *txt, struct prr0089t *pkt);

/*----------------------------------------------------------------------*/
/* �}���`�A���p�����[�^1�e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* prr0091,prr0092							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_parm1 {
	BYTE	cmd;			/* command = '-'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	neighbor_node[2];	/* �O�i�m�[�h�ԍ�		*/
	BYTE	shift_cnt[2];		/* �V�t�g��			*/
	BYTE	retry_time[2];		/* ���g���C����			*/
	BYTE	retry_span[2];		/* ���g���C�Ԋu			*/
	BYTE	retry_cnt[2];		/* ���g���C��			*/
	BYTE	master_slave[2];	/* �}�X�^�[�^�X���[�u�ݒ�	*/
	BYTE	ch_status[2];		/* �`�����l���ݒ�		*/
	BYTE	dsw[4];			/* NHIC DSW2 �r�b�g�t�B�[���h	*/
/*-------------------------------------*/
};

int	pr_scan_mlt_parm1( struct fmt_mlt_parm1 *buf);
void *	pr_print_mlt_parm1( struct fmt_mlt_parm1 *txt, struct prr0091t *pkt);

/*----------------------------------------------------------------------*/
/* �}���`�A���p�����[�^�Q�e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* prr0093,prr0094							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_parm2 {
	BYTE	cmd;			/* command = '^'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	multi_onoff[2];		/* �}���`�A�� �n�m�^�n�e�e�ݒ�	*/
	BYTE	multi_enter[2];		/* �}���`�A�� �Q���^�s�Q��	*/
	BYTE	combi_delay[2];		/* �v�ʒx�ꎞ��			*/
	BYTE	consec_mode[2];		/* �����␳���[�h		*/
	BYTE	mlt_dump_mode[2];	/* �񐔌v�ʔr�o���[�h		*/
	BYTE	interface[2];		/* ��@�A������		*/
	BYTE	speed[2];		/* ��������			*/
	BYTE	conv_delay[2];		/* ���ޱ�N���M���x�ꎞ��	*/
	BYTE	conv_on_time[2];	/* ���ޱ�N���M���I������	*/
	BYTE	ds_delay[2];		/* �r�o�����M���x�ꎞ��		*/
	BYTE	ds_on_time[2];		/* �r�o�����M���I������		*/
	BYTE	ds_sht_cnt[2];		/* �r�o�����M���V�t�g��		*/
	BYTE	mlt_dump_conf[2];	/* �񐔌v�ʔr�o�����M���o��Ӱ��	*/
	BYTE	p_ds_delay[2];		/* ���ʔr�o�����M���x�ꎞ��	*/
	BYTE	p_ds_on_time[2];	/* ���ʔr�o�����M���I������	*/
	BYTE	p_ds_sht_cnt[2];	/* ���ʔr�o�����M���V�t�g��	*/
/*-------------------------------------*/
};

int	pr_scan_mlt_parm2( struct fmt_mlt_parm2 *buf);
void *	pr_print_mlt_parm2( struct fmt_mlt_parm2 *txt, struct prr0093t *pkt);

/*----------------------------------------------------------------------*/
/* �}���`�A���g���p�����[�^�e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* prr0095,prr0096							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_ext {
	BYTE	cmd;			/* command = '<'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	s_cmd;			/* sub command			*/
	BYTE	p1[2];			/* parm1			*/
	BYTE	p2[2];			/* parm2			*/
	BYTE	p3[2];			/* parm3			*/
	BYTE	p4[2];			/* parm4			*/
/*-------------------------------------*/
};

int	pr_scan_mlt_ext( struct fmt_mlt_ext *buf);
void *	pr_print_mlt_ext( struct fmt_mlt_ext *txt, struct prr0095t *pkt);

/*----------------------------------------------------------------------*/
/*	�}���`�A���p�����[�^�Q�g���e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* prr1093,prr1094							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_parm2_ext {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_mch[2];		/* �v�ʋ@��			*/
	BYTE	node_id[32][2];		/* NHIC�m�[�h��			*/
/*-------------------------------------*/
};

int	pr_scan_mlt_parm2_ext( struct fmt_mlt_parm2_ext *buf);
void *	pr_print_mlt_parm2_ext( struct fmt_mlt_parm2_ext *txt, struct prr1093t *pkt);

/*----------------------------------------------------------------------*/
/*    �V�t�g�f�[�^�������ʃf�[�^�e�L�X�g�t�H�[�}�b�g    		*/
/*									*/
/* pri0041,pri0042							*/
/*----------------------------------------------------------------------*/
typedef struct {
	BYTE		sta_flag;	/* state flag			*/
	BYTE		weight[8];	/* �d��				*/
	BYTE		count[8];	/* �� 			*/
	BYTE         	ser_no[2];	/* �V���A���ԍ�			*/
} PI00410;

struct fmt_shift_data {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	all_num[2];		/* all data num 		*/
	BYTE	data_page[2];		/* data page          		*/
	BYTE	data_num;		/* �y�[�W���̃f�[�^��		*/
	PI00410 data[16];		/* �e�f�[�^���e			*/

/*-------------------------------------*/
};

int	pr_scan_shift_data( struct fmt_shift_data *buf);
void *	pr_print_shift_data( struct fmt_shift_data *txt, struct pri0041t *pkt);

/*----------------------------------------------------------------------*/
/*  �V�t�g�f�[�^�Q�Ɓi�v���j�d�l�e�L�X�g�t�H�[�}�b�g    		*/
/*									*/
/* prr0083,prr0084							*/
/*----------------------------------------------------------------------*/
struct fmt_shift_mon {
	BYTE	cmd;			/* command = '%'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	req_type;		/* reqest type   		*/
	BYTE	data_page[2];		/* data page          		*/
/*-------------------------------------*/
};

int	pr_scan_shift_mon( struct fmt_shift_mon *buf);
void *	pr_print_shift_mon( struct fmt_shift_mon *txt, struct prr0083t *pkt);


/*------------------------------------------------------*/
/*	�v�ʖڕW�l�g���p�����[�^�i�Q�j			*/
/*			�e�L�X�g�t�H�[�}�b�g		*/
/*	�A�x�R���E�T�C�N���^���V�I�^���Z�b�g�J�E���g	*/
/* prr2023,prr2024					*/
/*------------------------------------------------------*/

struct fmt_avg_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	avg_cycle[2];		/* average cycle		*/
	BYTE	avg_ratio[2];		/* average ratio		*/
	BYTE	avg_reset_count[4];	/* average reset count		*/
/*-------------------------------------*/
};

int	pr_scan_avg_parm( struct fmt_avg_parm *buf);
void *	pr_print_avg_parm( struct fmt_avg_parm *txt, struct prr2023t *pkt);


/*----------------------------------------------------------------------*/
/*	�⏕�g�ݍ��킹���d�ʃf�[�^�\���e�L�X�g�t�H�[�}�b�g		*/
/*									*/
/* pri1029,pri1030							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	nof_data;		/* number of data		*/
	BYTE	weight[32][4];		/* reference weight		*/
} PR10290;

struct fmt_ref_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR10290	sect[8];		/* paramter[nof_sect]		*/
/*-------------------------------------*/
};

int	pr_scan_ref_weight( struct fmt_ref_weight *buf);
void *	pr_print_ref_weight( struct fmt_ref_weight *txt, struct pri1029t *pkt);


/*------------------------------------------------------*/
/*	PLC�f�[�^�����݃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0087,prr0088					*/
/*------------------------------------------------------*/

struct fmt_plc_wr {
	BYTE	cmd;			/* command = '''		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	plc_wr_data[25][4];	/* PLC write data		*/
	BYTE	dump_mode[4];		/* dump mode			*/
/*-------------------------------------*/
};

int		pr_scan_plc_data( struct fmt_plc_wr *buf);
void *		pr_print_plc_data( struct fmt_plc_wr *txt, struct prr0087t *pkt);


/*------------------------------------------------------*/
/*	�v�ʖڕW�l�g���p�����[�^�i�T�j			*/
/*		�e�L�X�g�t�H�[�}�b�g			*/
/*	�g���㉺���l�f�[�^				*/
/* prr5023,prr5024					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	upper_wt[8];		/* ����d��			*/
	BYTE	lower_wt[8];		/* �����d��			*/
	BYTE	upper_cnt[4];		/* �����			*/
	BYTE	lower_cnt[4];		/* ������			*/
} PR50230;

struct fmt_ext_tgt {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* �Y������ch�̃Z�N�V������	*/
	PR50230	sect[8];		/* PR50230 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_ext_tgt(struct fmt_ext_tgt *buf);
void *	pr_print_ext_tgt(struct fmt_ext_tgt *txt, struct prr5023t *pkt);


/*----------------------------------------------------------------------*/
/*   �{�̏�ԏ��g���p�����[�^�e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* pri2013,pri2014							*/
/*----------------------------------------------------------------------*/

struct fmt_fsv_condition {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	cnd[4];			/* FSV condition		*/
/*-------------------------------------*/
};

int	pr_scan_fsv_condition(struct fmt_fsv_condition *buf);
void *	pr_print_fsv_condition(struct fmt_fsv_condition *txt,struct pri2013t *pkt);


/*----------------------------------------------------------------------*/
/*	�����U�蕪�����u�Q�[�g�ʒu�g���p�����[�^�e�L�X�g�t�H�[�}�b�g	*/
/*									*/
/* prr1005,prr1006							*/
/*----------------------------------------------------------------------*/

struct fmt_gate_position {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channnel		*/
	BYTE	gate_position[4];	/* �����U�蕪�����u�Q�[�g�ʒu	*/
/*-------------------------------------*/
};

int	pr_scan_gate_position( struct fmt_gate_position *buf);
void *	pr_print_gate_position( struct fmt_gate_position *txt, struct prr1005t *pkt);

/*------------------------------------------------------*/
/*	����d�l�f�[�^�̐ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0135,prr0136					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	spec_no[2];		/* �d�l�ԍ�			*/
	BYTE	nof_data;		/* �f�[�^��			*/
	BYTE	parm[8][8];		/* �f�[�^			*/
} PR01350;

struct fmt_sp_parm {
	BYTE	cmd;			/* command = 'T'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_parm[2];		/* number of paramter		*/
	PR01350	sp_parm[16];		/* PR01350 sp_parm[nof_parm]	*/
/*-------------------------------------*/
};

int	pr_scan_sp_parm( struct fmt_sp_parm *buf);
void *	pr_print_sp_parm( struct fmt_sp_parm *txt, struct prr0135t *pkt);

/*------------------------------------------------------*/
/*	���U�V���b�^�[�̐ݒ�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr0099,prr0100					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	on_off;			/* ���U�V���b�^�[�I���E�I�t	*/
	BYTE	fc_op_cnt[2];		/* ���U�V���b�^�[�����J�J�E���g	*/
	BYTE	fc_op_time[2];		/* ���U�V���b�^�[�����J����	*/
	BYTE	fc_cl_time[2];		/* ���U�V���b�^�[�����J����	*/
	BYTE	mode;			/* ���U�V���b�^�[���[�h		*/
	BYTE	nof_sh[2];		/* �Y������DF��SH��		*/
	BYTE	min_sh_no[2];		/* �ŏ�SH�ԍ�			*/
	BYTE	op_time[32][2];		/* ���U�V���b�^�[�J����		*/
} PR0990;


struct fmt_dfsh_parm {
	BYTE	cmd;			/* command = '|'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_df;			/* �Y������ch��DF��		*/
	BYTE	min_df_no;		/* �ŏ�DF�ԍ�			*/
	PR0990	sh[8];			/* ���U�V���b�^�[���		*/
/*-------------------------------------*/
};

int	pr_scan_sh_parm( struct fmt_dfsh_parm *buf);
void *	pr_print_sh_parm( struct fmt_dfsh_parm *txt, struct prr0099t *pkt);

/*------------------------------------------------------*/
/*	�g���p�����[�^�̃e�L�X�g�t�H�[�}�b�g		*/
/*							*/
/* prr0137,prr0138					*/
/*------------------------------------------------------*/

struct fmt_ext_parm {
	BYTE	cmd;			/* command = 0xac		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	sub_cmd[2];		/* sub command			*/
	BYTE	nof_parm[2];		/* number of paramter		*/
	BYTE	parm[40][8];		/* �f�[�^			*/
/*-------------------------------------*/
};

int	pr_scan_ext_parm( struct fmt_ext_parm *buf);
void *	pr_print_ext_parm( struct fmt_ext_parm *txt, struct prr0137t *pkt);

/*----------------------------------------------------------------------*/
/*	4-20mA����p�g���p�����[�^�e�L�X�g�t�H�[�}�b�g			*/
/*									*/
/* prr2005,prr2006							*/
/*----------------------------------------------------------------------*/

struct fmt_max_df_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sig[2];		/* �����@��			*/
	BYTE	max_df_weight[8][4];	/* 4-20mA�p�ő啪�U�d��		*/
/*-------------------------------------*/
};

int	pr_scan_max_df_weight( struct fmt_max_df_weight *buf);
void *	pr_print_max_df_weight( struct fmt_max_df_weight *txt, struct prr2005t *pkt);

/*----------------------------------------------------------------------*/
/*   �v�Z���ʊg���p�����[�^�e�L�X�g�t�H�[�}�b�g				*/
/*									*/
/* pri1011,pri1012							*/
/*----------------------------------------------------------------------*/

struct fmt_cb_caltm {
	BYTE	prm_pnt[4];		/* pointer to option parameter		*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel			*/
	BYTE	cb_caltm[4][8];		/* time of combinated calcuration	*/
/*-------------------------------------*/
};

int	pr_scan_cb_caltm(struct fmt_cb_caltm *buf);
void *	pr_print_cb_caltm(struct fmt_cb_caltm *txt, struct pri1011t *pkt);

/*----------------------------------------------------------------------*/
/*   �t�B�[�_�쓮�����f�[�^�\�� �g���p�����[�^�e�L�X�g�t�H�[�}�b�g 	*/
/*									*/
/* prr1015,prr1016							*/
/*----------------------------------------------------------------------*/
typedef struct  {
	BYTE	Ap[4];
	BYTE	Aon[4];
	BYTE	Bp[4];
	BYTE	Bon[4];
} PRR10150;

typedef struct {
	PRR10150 start;
	PRR10150 stop;
} PRR10151;

struct fmt_fd_spec_edge {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	PRR10151	mess[4];
/*-------------------------------------*/
};

int	pr_scan_fd_spec_edge( struct fmt_fd_spec_edge *buf);
void *	pr_print_fd_spec_edge( struct fmt_fd_spec_edge *txt, struct prr1015t *pkt);


/*----------------------------------------------------------------------*/
/*	���X�p���\���g���p�����[�^�e�L�X�g�t�H�[�}�b�g	*/
/*									*/
/* prr1035,prr1036							*/
/*----------------------------------------------------------------------*/

struct fmt_span2_adj {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	span_wt2[4];	/* ���X�p���d��	*/
/*-------------------------------------*/
};

int	pr_scan_span2_adj( struct fmt_span2_adj *buf);
void *	pr_print_span2_adj( struct fmt_span2_adj *txt, struct prr1035t *pkt);


/*------------------------------------------------------*/
/*	�v�g�p�̃��E���f�B���O�e�L�X�g�t�H�[�}�b�g	*/
/*							*/
/* prr3017,prr3018					*/
/*------------------------------------------------------*/

struct fmt_round_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_round;		/* ���E���f�B���O�O���[�v��	*/
	BYTE	round_base[8][2];	/* ���E���f�B���O�r�o��{����	*/
/*-------------------------------------*/
};

int	pr_scan_round_tmg( struct fmt_round_tmg *buf);
void *	pr_print_round_tmg( struct fmt_round_tmg *txt, struct prr3017t *pkt);

#endif	/* __PRCMD_H__	*/
