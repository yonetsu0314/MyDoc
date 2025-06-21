/******************************************************************************
* Fujitsu MB86964 Definitions                                                 *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Nov/1998                                                                 *
******************************************************************************/

/* MB86964 �������W�X�^ */

#define DLCR0       0x00        /* ���M�X�e�[�^�X */
#define DLCR1       0x01        /* ��M�X�e�[�^�X */
#define DLCR2       0x02        /* ���M�����݃C�l�[�u�� */
#define DLCR3       0x03        /* ��M�����݃C�l�[�u�� */
#define DLCR4       0x04        /* ���M���[�h */
#define DLCR5       0x05        /* ��M���[�h */
#define DLCR6       0x06        /* �R���g���[���P */
#define DLCR7       0x07        /* �R���g���[���Q */

#define DLCR8       0x08        /* �m�[�h ID 0 */
#define DLCR9       0x09        /* �m�[�h ID 1 */
#define DLCR10      0x0a        /* �m�[�h ID 2 */
#define DLCR11      0x0b        /* �m�[�h ID 3 */
#define DLCR12      0x0c        /* �m�[�h ID 4 */
#define DLCR13      0x0d        /* �m�[�h ID 5 */
#define DLCR14      0x0e        /* TDR 0 */
#define DLCR15      0x0f        /* TDR 1 */

#define MAR8        0x08        /* �}���`�X�L�����A�h���X 0 */
#define MAR9        0x09        /* �}���`�X�L�����A�h���X 1 */
#define MAR10       0x0a        /* �}���`�X�L�����A�h���X 2 */
#define MAR11       0x0b        /* �}���`�X�L�����A�h���X 3 */
#define MAR12       0x0c        /* �}���`�X�L�����A�h���X 4 */
#define MAR13       0x0d        /* �}���`�X�L�����A�h���X 5 */
#define MAR14       0x0e        /* �}���`�X�L�����A�h���X 6 */
#define MAR15       0x0f        /* �}���`�X�L�����A�h���X 7 */

#define BMPR8       0x08        /* �o�b�t�@�������|�[�g */
#define BMPR10      0x0a        /* ���M�p�P�b�g�� */
#define BMPR11      0x0b        /* 16 �R���W���� */
#define BMPR12      0x0c        /* DMA �C�l�[�u�� */
#define BMPR13      0x0d        /* DMA �o�[�X�g/�g�����V�[�o���[�h */
#define BMPR14      0x0e        /* ��M�R���g���[��/�g�����V�[�o������ */
#define BMPR15      0x0f        /* �g�����V�[�o�X�e�[�^�X */

/* ���M�X�e�[�^�X���W�X�^ */

#define DLCR0_TMT_OK        0x80    /* ���M OK */
#define DLCR0_NET_BSY       0x40    /* �l�b�g�r�W�[ */
#define DLCR0_TMT_REC       0x20    /* ���M�p�P�b�g��M */
#define DLCR0_SRT_PKT       0x10    /* �V���[�g�p�P�b�g */
#define DLCR0_JABBER        0x08    /* �W���o�[ */
#define DLCR0_COL           0x04    /* �R���W�����G���[ */
#define DLCR0_16COL         0x02    /* 16 �R���W�����G���[ */
#define DLCR0_BUS_WR_ERR    0x01    /* �o�X���C�g�G���[ */

/* ��M�X�e�[�^�X���W�X�^ */

#define DLCR1_PKT_RDY       0x80    /* �p�P�b�g���f�B */
#define DLCR1_BUS_RD_ERR    0x40    /* �o�X���[�h�G���[ */
#define DLCR1_DMA_EOP       0x20    /* DMA �]������ */
#define DLCR1_RMT_RST       0x10    /* �����[�g���Z�b�g */
#define DLCR1_RX_SRT_PKT    0x08    /* �V���[�g�p�P�b�g�G���[ */
#define DLCR1_ALG_ERR       0x04    /* �A���C�������g�G���[ */
#define DLCR1_CRC_ERR       0x02    /* CRC �G���[ */
#define DLCR1_OVRFLO        0x01    /* �I�[�o�t���[�G���[ */

/* ���M�����݃C�l�[�u�����W�X�^ */

#define DLCR2_ENA_TMT_OK    0x80    /* TMT OK �����ݔ������� */
#define DLCR2_ENA_JABBER    0x08    /* JABBER �����ݔ������� */
#define DLCR2_ENA_COL       0x04    /* COL �����ݔ������� */
#define DLCR2_ENA_16COL     0x02    /* 16COL �����ݔ������� */
#define DLCR2_ENA_BUS_WR_ERR 0x01   /* BUS WR ERR �����ݔ������� */

/* ��M�����݃C�l�[�u�����W�X�^ */

#define DLCR3_ENA_PKT_RDY   0x80    /* PKT RDY �����ݔ������� */
#define DLCR3_ENA_BUS_RD_ERR 0x40   /* BUS RD ERR �����ݔ������� */
#define DLCR3_ENA_DMA_EOP   0x20    /* DMA EOP �����ݔ������� */
#define DLCR3_ENA_RMT_RST   0x10    /* RMT RST �����ݔ������� */
#define DLCR3_ENA_RX_SRT_ERR 0x08   /* RX SRT ERR �����ݔ������� */
#define DLCR3_ENA_ALG_ERR   0x04    /* ALG ERR �����ݔ������� */
#define DLCR3_ENA_CRC_ERR   0x02    /* CRC ERR �����ݔ������� */
#define DLCR3_ENA_OVRFLO    0x01    /* OVRFLO �����ݔ������� */

/* ���M���[�h���W�X�^ */

#define DLCR4_COL3_COL0     0xf0    /* �R���W�����J�E���g */
#define DLCR4_TST1          0x08    /* �`�b�v�e�X�g1 */
#define DLCR4_NCNTRL        0x04    /* 0:CNTRL �[�q High, 1:Low */
#define DLCR4_NLBC          0x02    /* ���[�v�o�b�N�R���g���[�� */
#define DLCR4_DSC           0x01    /* �f�B�Z�[�u���L�����A�f�B�e�N�g */

/* ��M���[�h���W�X�^ */

#define DLCR5_TST2          0x80    /* �`�b�v�e�X�g2 */
#define DLCR5_BUF_EMP       0x40    /* ��M�o�b�t�@�� */
#define DLCR5_ACPT_BAD_PKT  0x20    /* �s�ǃp�P�b�g��M */
#define DLCR5_ADD_SIZE      0x10    /* �A�h���X�T�C�Y�i1:5�o�C�g�̂ݔ�r�j*/
#define DLCR5_ENA_SRT_PKT   0x08    /* �C�l�[�u���V���[�g�p�P�b�g */
#define DLCR5_ENA_RMT_RST   0x04    /* �C�l�[�u�������[�g���Z�b�g */
#define DLCR5_AM1           0x02    /* �A�h���X�}�b�`���[�h */
#define DLCR5_AM0           0x01    /* �V */

/* �R���g���[���P���W�X�^ */

#define DLCR6_NENA_DLC      0x80    /* 1:�C�j�V�����C�Y, 0:���싖�� */
#define DLCR6_100NS         0x40    /* �O�t��SRAM 100ns */
#define DLCR6_SB            0x20    /* �V�X�e���f�[�^�o�X8�r�b�g */
#define DLCR6_BB            0x10    /* �o�b�t�@�������f�[�^�o�X8�r�b�g */
#define DLCR6_TX_BUF_SIZE1  0x08    /* ���M�o�b�t�@�T�C�Y */
#define DLCR6_TX_BUF_SIZE0  0x04    /* �V */
#define DLCR6_BUF_SIZE1     0x02    /* �O�t���o�b�t�@�������T�C�Y */
#define DLCR6_BUF_SIZE0     0x01    /* �V */

/* �R���g���[���Q���W�X�^ */

#define DLCR7_IDENT1        0x80    /* �R���g���[����� */
#define DLCR7_IDENT0        0x40    /* 00:MB86960A, 01:MB86964, 11:MB86965A */
#define DLCR7_NSTBY         0x20    /* 0:�X�^���o�C���[�h */
#define DLCR7_RDYPNSEL      0x10    /* RDYPNSEL �[�q�l */
#define DLCR7_RBS1          0x08    /* ���W�X�^�o���N�I�� */
#define DLCR7_RBS0          0x04    /* �V */
#define DLCR7_EOP           0x02    /* 0:EOP ���̓N�e�B�u Low, 1:High */
#define DLCR7_BYTE_SWAP     0x01    /* �o�C�g�X���b�v */

/* 16��R���W�����R���g���[�����W�X�^ */

#define BMPR11_16COL_CNT2   0x04
#define BMPR11_16COL_CNT1   0x02
#define BMPR11_16COL_CNT0   0x01

/* DMA �C�l�[�u�����W�X�^ */

#define BMPR12_DMA_RENA     0x02    /* DMA ���[�h����C�l�[�u�� */
#define BMPR12_DMA_TENA     0x01    /* DMA ���C�g����C�l�[�u�� */

/* DMA �o�[�X�g�^�g�����V�[�o���[�h�R���g���[�����W�X�^ */

#define BMPR13_IO_BASE_UNLOCK 0x80
#define BMPR13_LOWER_SQUELCH  0x40
#define BMPR13_LINK_TEST_ENA  0x20
#define BMPR13_AUI            0x10
#define BMPR13_AUTO_PORT_SEL  0x08
#define BMPR13_STP            0x04
#define BMPR13_DMA_BRST1      0x02
#define BMPR13_DMA_BRST0      0x01

/* ��M�R���g���[���^�g�����V�[�o�����݃C�l�[�u�����W�X�^ */

#define BMPR14_ENA_RLD_INT  0x80
#define BMPR14_ENA_LLD_INT  0x40
#define BMPR14_ENA_RJAB_INT 0x20
#define BMPR14_SKIP_RX_PKT  0x04
#define BMPR14_ENA_SQE_INT  0x02
#define BMPR14_ENA_FILTER   0x01

/* �g�����V�[�o�X�e�[�^�X���W�X�^ */

#define BMPR15_RLD          0x80
#define BMPR15_LLD          0x40
#define BMPR15_RJAB         0x20
#define BMPR15_RMT_PORT     0x10
#define BMPR15_POL_REV      0x08
#define BMPR15_SQE          0x02

/* ��M�p�P�b�g�X�e�[�^�X */

#define PKT_RDY             0x80
#define GOOD_PKT            0x20
#define OVRFLO              0x01

/* end */
