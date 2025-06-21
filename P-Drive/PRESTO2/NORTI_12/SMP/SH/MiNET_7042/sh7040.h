/******************************************************************************
* SH7040�V���[�Y�������Ӄ��W���[����`                                        *
*                                                                             *
*  File name : sh7040.h                                                       *
*  Copyright (c)  1997-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Aug/1997 �V�K�쐬 (sh7040.h)                                          MI *
* 25/Apr/2000 sh7042.h�ɉ���, MiNET-7042�{�[�h�ˑ���`��ǉ�               FJ *
* 31/Oct/2000 IAR EW �Ή�                                                  TK *
* 05/Mar/2001 sh7040.h�ɖ߂�, MiNET-7042�{�[�h�ˑ���`���폜               MI *
* 23/Dec/2002 sfr_iset, sfr_iclr���폜                                     MI *
******************************************************************************/

#ifndef SH7040_H
#define SH7040_H

/* �������Ӄ��W���[���̃x�[�X�A�h���X */

#ifdef CPU_SH /* SH�p�N���X�R���p�C���ŃR���p�C������Ă���ꍇ */
#define IO_BASE ((volatile unsigned char *)0xffff8000L)
#endif

/* �������Ӄ��W���[�����o�̓}�N�� */

UB sfr_in(unsigned n);              /* �o�C�g�f�[�^�̓��� */
void sfr_out(unsigned n, UB c);     /* �o�C�g�f�[�^�̏o�� */
UH sfr_inw(unsigned n);             /* ���[�h�f�[�^�̓��� */
void sfr_outw(unsigned n, UH c);    /* ���[�h�f�[�^�̏o�� */
UW sfr_inl(unsigned n);             /* �����O���[�h�f�[�^�̓��� */
void sfr_outl(unsigned n, UW c);    /* �����O���[�h�f�[�^�̏o�� */
void sfr_set(unsigned n, UB c);     /* �o�C�g�f�[�^�̃r�b�g�Z�b�g */
void sfr_clr(unsigned n, UB c);     /* �o�C�g�f�[�^�̃r�b�g�N���A */
void sfr_setw(unsigned n, UH c);    /* ���[�h�f�[�^�̃r�b�g�Z�b�g */
void sfr_clrw(unsigned n, UH c);    /* ���[�h�f�[�^�̃r�b�g�N���A */
void sfr_iset(unsigned n, UB c);    /* �o�C�g�f�[�^�̑��l�r�b�g�Z�b�g */
void sfr_iclr(unsigned n, UB c);    /* �o�C�g�f�[�^�̑��l�r�b�g�N���A */

#ifdef IO_BASE
#define sfr_in(n)       (*(IO_BASE+(n)))
#define sfr_out(n,c)    (*(IO_BASE+(n))=(c))
#define sfr_inw(n)      (*(volatile UH *)(IO_BASE+(n)))
#define sfr_outw(n,c)   (*(volatile UH *)(IO_BASE+(n))=(c))
#define sfr_inl(n)      (*(volatile UW *)(IO_BASE+(n)))
#define sfr_outl(n,c)   (*(volatile UW *)(IO_BASE+(n))=(c))
#define sfr_set(n,c)    (*(IO_BASE+(n))|=(c))
#define sfr_clr(n,c)    (*(IO_BASE+(n))&=~(c))
#define sfr_setw(n,c)   (*(volatile UH *)(IO_BASE+(n))|=(c))
#define sfr_clrw(n,c)   (*(volatile UH *)(IO_BASE+(n))&=~(c))
#endif

/* �V���A���R�~���j�P�[�V�����C���^�[�t�F�[�X(SCI) �`���l��0 */

#define SCI_SMR0    0x1a0   /* �V���A�����[�h���W�X�^ */
#define SCI_BRR0    0x1a1   /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR0    0x1a2   /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR0    0x1a3   /* �g�����X�~�b�g�f�[�^���W�X�^ */
#define SCI_SSR0    0x1a4   /* �V���A���X�e�[�^�X���W�X�^ */
#define SCI_RDR0    0x1a5   /* ���V�[�u�f�[�^���W�X�^ */

/* �V���A���R�~���j�P�[�V�����C���^�[�t�F�[�X(SCI) �`���l��1 */

#define SCI_SMR1    0x1b0   /* �V���A�����[�h���W�X�^ */
#define SCI_BRR1    0x1b1   /* �r�b�g���[�g���W�X�^ */
#define SCI_SCR1    0x1b2   /* �V���A���R���g���[�����W�X�^ */
#define SCI_TDR1    0x1b3   /* �g�����X�~�b�g�f�[�^���W�X�^ */
#define SCI_SSR1    0x1b4   /* �V���A���X�e�[�^�X���W�X�^ */
#define SCI_RDR1    0x1b5   /* ���V�[�u�f�[�^���W�X�^ */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) ���� */

#define MTU_TSTR    0x240   /* �^�C�}�X�^�[�g���W�X�^ */
#define MTU_TSYR    0x241   /* �^�C�}�V���N�����W�X�^ */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��0 */

#define MTU_TCR0    0x260   /* �^�C�}�R���g���[�����W�X�^0 */
#define MTU_TMDR0   0x261   /* �^�C�}���[�h���W�X�^0 */
#define MTU_TIOR0H  0x262   /* �^�C�} I/O �R���g���[�����W�X�^0H */
#define MTU_TIOR0L  0x263   /* �^�C�} I/O �R���g���[�����W�X�^0L */
#define MTU_TIER0   0x264   /* �^�C�}�C���^���v�g�C�l�[�u�����W�X�^0 */
#define MTU_TSR0    0x265   /* �^�C�}�X�e�[�^�X���W�X�^0 */
#define MTU_TCNT0   0x266   /* �^�C�}�J�E���^0 */
#define MTU_TGR0A   0x268   /* �W�F�l�������W�X�^0A */
#define MTU_TGR0B   0x26a   /* �W�F�l�������W�X�^0B */
#define MTU_TGR0C   0x26c   /* �W�F�l�������W�X�^0C */
#define MTU_TGR0D   0x26e   /* �W�F�l�������W�X�^0D */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��1 */

#define MTU_TCR1    0x280   /* �^�C�}�R���g���[�����W�X�^1 */
#define MTU_TMDR1   0x281   /* �^�C�}���[�h���W�X�^1 */
#define MTU_TIOR1   0x282   /* �^�C�} I/O �R���g���[�����W�X�^1 */
#define MTU_TIER1   0x284   /* �^�C�}�C���^���v�g�C�l�[�u�����W�X�^1 */
#define MTU_TSR1    0x285   /* �^�C�}�X�e�[�^�X���W�X�^1 */
#define MTU_TCNT1   0x286   /* �^�C�}�J�E���^1 */
#define MTU_TGR1A   0x288   /* �W�F�l�������W�X�^1A */
#define MTU_TGR1B   0x28a   /* �W�F�l�������W�X�^1B */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��2 */

#define MTU_TCR2    0x2a0   /* �^�C�}�R���g���[�����W�X�^2 */
#define MTU_TMDR2   0x2a1   /* �^�C�}���[�h���W�X�^2 */
#define MTU_TIOR2   0x2a2   /* �^�C�} I/O �R���g���[�����W�X�^2 */
#define MTU_TIER2   0x2a4   /* �^�C�}�C���^���v�g�C�l�[�u�����W�X�^2 */
#define MTU_TSR2    0x2a5   /* �^�C�}�X�e�[�^�X���W�X�^2 */
#define MTU_TCNT2   0x2a6   /* �^�C�}�J�E���^2 */
#define MTU_TGR2A   0x2a8   /* �W�F�l�������W�X�^2A */
#define MTU_TGR2B   0x2aa   /* �W�F�l�������W�X�^2B */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��3 */

#define MTU_TCR3    0x200   /* �^�C�}�R���g���[�����W�X�^3 */
#define MTU_TMDR3   0x202   /* �^�C�}���[�h���W�X�^3 */
#define MTU_TIOR3H  0x204   /* �^�C�} I/O �R���g���[�����W�X�^3H */
#define MTU_TIOR3L  0x205   /* �^�C�} I/O �R���g���[�����W�X�^3L */
#define MTU_TIER3   0x208   /* �^�C�}�C���^���v�g�C�l�[�u�����W�X�^3 */
#define MTU_TSR3    0x22c   /* �^�C�}�X�e�[�^�X���W�X�^3 */
#define MTU_TCNT3   0x210   /* �^�C�}�J�E���^3 */
#define MTU_TGR3A   0x218   /* �W�F�l�������W�X�^3A */
#define MTU_TGR3B   0x21a   /* �W�F�l�������W�X�^3B */
#define MTU_TGR3C   0x224   /* �W�F�l�������W�X�^3C */
#define MTU_TGR3D   0x226   /* �W�F�l�������W�X�^3D */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��4 */

#define MTU_TCR4    0x201   /* �^�C�}�R���g���[�����W�X�^4 */
#define MTU_TMDR4   0x203   /* �^�C�}���[�h���W�X�^4 */
#define MTU_TIOR4H  0x206   /* �^�C�} I/O �R���g���[�����W�X�^4H */
#define MTU_TIOR4L  0x207   /* �^�C�} I/O �R���g���[�����W�X�^4L */
#define MTU_TIER4   0x209   /* �^�C�}�C���^���v�g�C�l�[�u�����W�X�^4 */
#define MTU_TSR4    0x22d   /* �^�C�}�X�e�[�^�X���W�X�^4 */
#define MTU_TCNT4   0x212   /* �^�C�}�J�E���^4 */
#define MTU_TGR4A   0x21c   /* �W�F�l�������W�X�^4A */
#define MTU_TGR4B   0x21e   /* �W�F�l�������W�X�^4B */
#define MTU_TGR4C   0x228   /* �W�F�l�������W�X�^4C */
#define MTU_TGR4D   0x22a   /* �W�F�l�������W�X�^4D */

/* �}���`�t�@���N�V�����^�C�}�p���X���j�b�g(MTU) �`���l��3/4 ���� */

#define MTU_TOER    0x20a   /* �^�C�}o�}�X�^�C�l�[�u�����W�X�^ */
#define MTU_TOCR    0x20b   /* �^�C�}o�R���g���[�����W�X�^ */
#define MTU_TGCR    0x20d   /* �^�C�}�Q�[�g�R���g���[�����W�X�^ */
#define MTU_TCDR    0x214   /* �^�C�}�����f�[�^���W�X�^ */
#define MTU_TDDR    0x216   /* �^�C�}�f�b�h�^�C���f�[�^���W�X�^ */
#define MTU_TCNTS   0x220   /* �^�C�}�T�u�J�E���^ */
#define MTU_TCBR    0x222   /* �^�C�}�����o�b�t�@���W�X�^ */

/* MTU �|�[�g�A�E�g�v�b�g�C�l�[�u��(POE) */

#define MTU_ICSR    0x3c0   /* ���̓��x���R���g���[��/�X�e�[�^�X���W�X�^ */
#define MTU_OCSR    0x3c2   /* �o�̓��x���R���g���[��/�X�e�[�^�X���W�X�^ */

/* ���荞�݃R���g���[��(INTC) */

#define INTC_IPRA   0x348   /* ���荞�ݗD�惌�x���ݒ背�W�X�^A */
#define INTC_IPRB   0x34a   /* ���荞�ݗD�惌�x���ݒ背�W�X�^B */
#define INTC_IPRC   0x34c   /* ���荞�ݗD�惌�x���ݒ背�W�X�^C */
#define INTC_IPRD   0x34e   /* ���荞�ݗD�惌�x���ݒ背�W�X�^D */
#define INTC_IPRE   0x350   /* ���荞�ݗD�惌�x���ݒ背�W�X�^E */
#define INTC_IPRF   0x352   /* ���荞�ݗD�惌�x���ݒ背�W�X�^F */
#define INTC_IPRG   0x354   /* ���荞�ݗD�惌�x���ݒ背�W�X�^G */
#define INTC_IPRH   0x356   /* ���荞�ݗD�惌�x���ݒ背�W�X�^H */
#define INTC_ICR    0x358   /* ���荞�݃R���g���[�����W�X�^ */
#define INTC_ISR    0x35a   /* IRQ �X�e�[�^�X���W�X�^ */

/* I/O�|�[�g */

#define IO_PADRH    0x380   /* �|�[�gA �f�[�^���W�X�^H */
#define IO_PADRL    0x382   /* �|�[�gA �f�[�^���W�X�^L */
#define IO_PBDR     0x390   /* �|�[�gB �f�[�^���W�X�^ */
#define IO_PCDR     0x392   /* �|�[�gC �f�[�^���W�X�^ */
#define IO_PDDRH    0x3a0   /* �|�[�gD �f�[�^���W�X�^H */
#define IO_PDDRL    0x3a2   /* �|�[�gD �f�[�^���W�X�^L */
#define IO_PEDR     0x3b0   /* �|�[�gE �f�[�^���W�X�^ */
#define IO_PFDR     0x3b3   /* �|�[�gF �f�[�^���W�X�^ */

/* �s���t�@���N�V�����R���g���[��(PFC) */

#define PFC_PAIORH  0x384   /* �|�[�gA IO���W�X�^H */
#define PFC_PAIORL  0x386   /* �|�[�gA IO���W�X�^L */
#define PFC_PACRH   0x388   /* �|�[�gA �R���g���[�����W�X�^H */
#define PFC_PACRL1  0x38c   /* �|�[�gA �R���g���[�����W�X�^L1 */
#define PFC_PACRL2  0x38e   /* �|�[�gA �R���g���[�����W�X�^L2 */
#define PFC_PBIOR   0x394   /* �|�[�gB IO���W�X�^ */
#define PFC_PBCR1   0x398   /* �|�[�gB �R���g���[�����W�X�^1 */
#define PFC_PBCR2   0x39a   /* �|�[�gB �R���g���[�����W�X�^2 */
#define PFC_PCIOR   0x396   /* �|�[�gC IO���W�X�^ */
#define PFC_PCCR    0x39c   /* �|�[�gC �R���g���[�����W�X�^ */
#define PFC_PDIORH  0x3a4   /* �|�[�gD IO���W�X�^H */
#define PFC_PDIORL  0x3a6   /* �|�[�gD IO���W�X�^L */
#define PFC_PDCRH1  0x3a8   /* �|�[�gD �R���g���[�����W�X�^H1 */
#define PFC_PDCRH2  0x3aa   /* �|�[�gD �R���g���[�����W�X�^H2 */
#define PFC_PDCRL   0x3ac   /* �|�[�gD �R���g���[�����W�X�^L */
#define PFC_PEIOR   0x3b4   /* �|�[�gE IO���W�X�^ */
#define PFC_PECR1   0x3b8   /* �|�[�gE �R���g���[�����W�X�^1 */
#define PFC_PECR2   0x3ba   /* �|�[�gE �R���g���[�����W�X�^2 */
#define PFC_IFCR    0x3c8   /* IRQOUT �@�\�R���g���[�����W�X�^ */

/* �R���y�A�}�b�`�^�C�}(CMT) ���� */

#define CMT_CMSTR   0x3d0   /* �R���y�A�}�b�`�^�C�}�X�^�[�g���W�X�^ */

/* �R���y�A�}�b�`�^�C�}(CMT) �`���l��0 */

#define CMT_CMCSR0  0x3d2   /* �R���y�A�}�b�`�^�C�}�R���g���[��/�X�e�[�^�X���W�X�^0 */
#define CMT_CMCNT0  0x3d4   /* �R���y�A�}�b�`�^�C�}�J�E���^0 */
#define CMT_CMCOR0  0x3d6   /* �R���y�A�}�b�`�^�C�}�R���X�^���g���W�X�^0 */

/* �R���y�A�}�b�`�^�C�}(CMT) �`���l��1 */

#define CMT_CMCSR1  0x3d8   /* �R���y�A�}�b�`�^�C�}�R���g���[��/�X�e�[�^�X���W�X�^1 */
#define CMT_CMCNT1  0x3da   /* �R���y�A�}�b�`�^�C�}�J�E���^1 */
#define CMT_CMCOR1  0x3dc   /* �R���y�A�}�b�`�^�C�}�R���X�^���g���W�X�^1 */

/* A/D�ϊ��� */

#define AD_DRA      0x3f0   /* A/D �f�[�^���W�X�^A */
#define AD_DRB      0x3f2   /* A/D �f�[�^���W�X�^B */
#define AD_DRC      0x3f4   /* A/D �f�[�^���W�X�^C */
#define AD_DRD      0x3f6   /* A/D �f�[�^���W�X�^D */
#define AD_DRE      0x3f8   /* A/D �f�[�^���W�X�^E */
#define AD_DRF      0x3fa   /* A/D �f�[�^���W�X�^F */
#define AD_DRG      0x3fc   /* A/D �f�[�^���W�X�^G */
#define AD_DRH      0x3fe   /* A/D �f�[�^���W�X�^H */
#define AD_CSR      0x3e0   /* A/D �R���g���[��/�X�e�[�^�X���W�X�^ */
#define AD_CR       0x3e1   /* A/D �R���g���[�����W�X�^ */

/* ���[�U�u���[�N�R���g���[��(UBC) */

#define UBC_UBARH   0x600   /* ���[�U�u���[�N�A�h���X���W�X�^H */
#define UBC_UBARL   0x602   /* ���[�U�u���[�N�A�h���X���W�X�^L */
#define UBC_UBAMRH  0x604   /* ���[�U�u���[�N�A�h���X�}�X�N���W�X�^H */
#define UBC_UBAMRL  0x606   /* ���[�U�u���[�N�A�h���X�}�X�N���W�X�^L */
#define UBC_UBBR    0x608   /* ���[�U�u���[�N�o�X�T�C�N�����W�X�^ */

/* �E�H�b�`�h�b�O�^�C�}(WDT) */

#define WDT_TCSR    0x610   /* �^�C�}�R���g���[��/�X�e�[�^�X���W�X�^ */
#define WDT_TCNT    0x610   /* �^�C�}�J�E���^(��������) */
#define WDT_TCNT_R  0x611   /* �^�C�}�J�E���^(�ǂݏo��) */
#define WDT_RSTCSR  0x612   /* ���Z�b�g�R���g���[��/�X�e�[�^�X���W�X�^(��������) */
#define WDT_RSTCSR_R 0x613  /* ���Z�b�g�R���g���[��/�X�e�[�^�X���W�X�^(�ǂݏo��) */

/* �����d�͏�� */

#define SYS_SBYCR   0x614   /* �X�^���o�C�R���g���[�����W�X�^ */

/* �o�X�X�e�[�g�R���g���[��(BSC) */

#define BSC_BCR1    0x620   /* �o�X�R���g���[�����W�X�^1 */
#define BSC_BCR2    0x622   /* �o�X�R���g���[�����W�X�^2 */
#define BSC_WCR1    0x624   /* �E�F�C�g�R���g���[�����W�X�^1 */
#define BSC_WCR2    0x626   /* �E�F�C�g�R���g���[�����W�X�^2 */
#define BSC_DCR     0x62a   /* DRAM �G���A�R���g���[�����W�X�^ */
#define BSC_RTCSR   0x62c   /* ���t���b�V���^�C�}�R���g���[��/�X�e�[�^�X���W�X�^ */
#define BSC_RTCNT   0x62e   /* ���t���b�V���^�C�}�J�E���^ */
#define BSC_RTCOR   0x630   /* ���t���b�V���^�C���R���X�^���g���W�X�^ */

/* �_�C���N�g�������A�N�Z�X�R���g���[��(DMAC) �`���l��0 */

#define DMAC_SAR0   0x6c0   /* DMA �\�[�X�A�h���X���W�X�^0 */
#define DMAC_DAR0   0x6c4   /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^0 */
#define DMAC_TCR0   0x6c8   /* DMA �g�����X�t�@�J�E���g���W�X�^0 */
#define DMAC_CHCR0  0x6cc   /* DMA �`���l���R���g���[�����W�X�^0 */

/* �_�C���N�g�������A�N�Z�X�R���g���[��(DMAC) �`���l��1 */

#define DMAC_SAR1   0x6d0   /* DMA �\�[�X�A�h���X���W�X�^1 */
#define DMAC_DAR1   0x6d4   /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^1 */
#define DMAC_TCR1   0x6d8   /* DMA �g�����X�t�@�J�E���g���W�X�^1 */
#define DMAC_CHCR1  0x6dc   /* DMA �`���l���R���g���[�����W�X�^1 */

/* �_�C���N�g�������A�N�Z�X�R���g���[��(DMAC) �`���l��2 */

#define DMAC_SAR2   0x6e0   /* DMA �\�[�X�A�h���X���W�X�^2 */
#define DMAC_DAR2   0x6e4   /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^2 */
#define DMAC_TCR2   0x6e8   /* DMA �g�����X�t�@�J�E���g���W�X�^2 */
#define DMAC_CHCR2  0x6ec   /* DMA �`���l���R���g���[�����W�X�^2 */

/* �_�C���N�g�������A�N�Z�X�R���g���[��(DMAC) �`���l��3 */

#define DMAC_SAR3   0x6f0   /* DMA �\�[�X�A�h���X���W�X�^3 */
#define DMAC_DAR3   0x6f4   /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^3 */
#define DMAC_TCR3   0x6fa   /* DMA �g�����X�t�@�J�E���g���W�X�^3 */
#define DMAC_CHCR3  0x6fe   /* DMA �`���l���R���g���[�����W�X�^3 */

/* �_�C���N�g�������A�N�Z�X�R���g���[��(DMAC) ���� */

#define DMAC_DMAOR  0x6b0   /* DMA �I�y���[�V�������W�X�^ */

/* �f�[�^�g�����X�t�@�R���g���[��(DTC) */

#define DTC_DTEA    0x700   /* DTC �C�l�[�u�����W�X�^A */
#define DTC_DTEB    0x701   /* DTC �C�l�[�u�����W�X�^B */
#define DTC_DTEC    0x702   /* DTC �C�l�[�u�����W�X�^C */
#define DTC_DTED    0x703   /* DTC �C�l�[�u�����W�X�^D */
#define DTC_DTEE    0x704   /* DTC �C�l�[�u�����W�X�^E */
#define DTC_DTCSR   0x706   /* DTC �R���g���[��/�X�e�[�^�X���W�X�^ */
#define DTC_DTBR    0x708   /* DTC ���x�[�X���W�X�^ */

/* �L���b�V��������(CAC) */

#define CAC_CCR     0x740   /* �L���b�V���R���g���[�����W�X�^ */

/* ���荞�݃x�N�^�ԍ� */

#define INT_NMI     11      /* NMI */
#define INT_UB      12      /* ���[�U�u���[�N */
#define INT_IRQ0    64      /* �O���[�q IRQ0 */
#define INT_IRQ1    65      /* �O���[�q IRQ1 */
#define INT_IRQ2    66      /* �O���[�q IRQ2 */
#define INT_IRQ3    67      /* �O���[�q IRQ3 */
#define INT_IRQ4    68      /* �O���[�q IRQ4 */
#define INT_IRQ5    69      /* �O���[�q IRQ5 */
#define INT_IRQ6    70      /* �O���[�q IRQ6 */
#define INT_IRQ7    71      /* �O���[�q IRQ7 */
#define INT_DEI0    72      /* DMAC �`���l��0 */
#define INT_DEI1    76      /* DMAC �`���l��1 */
#define INT_DEI2    80      /* DMAC �`���l��2 */
#define INT_DEI3    84      /* DMAC �`���l��3 */
#define INT_TGI0A   88      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��0A */
#define INT_TGI0B   89      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��0B */
#define INT_TGI0C   90      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��0C */
#define INT_TGI0D   91      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��0D */
#define INT_TCI0V   92      /* MTU �`��l��0 �I�[�o�t���[ */
#define INT_TGI1A   96      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��1A */
#define INT_TGI1B   97      /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��1B */
#define INT_TCI1V   100     /* MTU �`��l��1 �I�[�o�t���[ */
#define INT_TCI1U   101     /* MTU �`��l��1 �A���_�t���[ */
#define INT_TGI2A   104     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��2A */
#define INT_TGI2B   105     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��2B */
#define INT_TCI2V   108     /* MTU �`��l��2 �I�[�o�t���[ */
#define INT_TCI2U   109     /* MTU �`��l��2 �A���_�t���[ */
#define INT_TGI3A   112     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��3A */
#define INT_TGI3B   113     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��3B */
#define INT_TGI3C   114     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��3C */
#define INT_TGI3D   115     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��3D */
#define INT_TCI3V   116     /* MTU �`��l��3 �I�[�o�t���[ */
#define INT_TGI4A   120     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��4A */
#define INT_TGI4B   121     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��4B */
#define INT_TGI4C   122     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��4C */
#define INT_TGI4D   123     /* MTU �R���y�A�}�b�`/�C���v�b�g�L���v�`��4D */
#define INT_TCI4V   124     /* MTU �`��l��4 �I�[�o�t���[/�A���_�t���[ */
#define INT_ERI0    128     /* SCI �`���l��0 ��M�G���[ */
#define INT_RXI0    129     /* SCI �`���l��0 ��M�f�[�^�t�� */
#define INT_TXI0    130     /* SCI �`���l��0 ���M�f�[�^�G���v�e�B */
#define INT_TEI0    131     /* SCI �`���l��0 ���M�I�� */
#define INT_ERI1    132     /* SCI �`���l��1 ��M�G���[ */
#define INT_RXI1    133     /* SCI �`���l��1 ��M�f�[�^�t�� */
#define INT_TXI1    134     /* SCI �`���l��1 ���M�f�[�^�G���v�e�B */
#define INT_TEI1    135     /* SCI �`���l��1 ���M�I�� */
#define INT_ADI     136     /* A/D �G���h */
#define INT_SWDTCE  140     /* DTC �\�t�g�E�F�A�N�� */
#define INT_CMI0    144     /* CMT �`���l��0 �R���y�A�}�b�` */
#define INT_CMI1    148     /* CMT �`���l��1 �R���y�A�}�b�` */
#define INT_ITI     152     /* �E�H�b�`�h�b�N�^�C�}�i�C���^�[�o���^�C�}�j */
#define INT_CMI     153     /* ���t���b�V���R���g���[���R���y�A�}�b�` */
#define INT_OEI     156     /* MTU POE */

#endif /* SH7040_H */
