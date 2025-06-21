/******************************************************************************
* �r�g�V�O�T�T �����h�^�n��`                                                 *
*                                                                             *
*  File name : sh7055.h                                                       *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 06/Feb/2001 �쐬                                                            *
* 25/Feb/2003 Green Hills ���胉�x����ύX(__SH7000 -> __ghs)              KA *
******************************************************************************/

#ifndef SH7055_H
#define SH7055_H

/* �����h�^�n�̃x�[�X�A�h���X */

#ifdef CPU_SH
#define IO_BASE ((volatile unsigned char *)0xffffe000L)
#endif

/* �����h�^�n���o�̓}�N�� */

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

#if defined(__GNUC__)
#elif defined(__ghs)
#else                   /* Renesas C */
#define sfr_iset(n,c)   do{set_gbr(IO_BASE+(n));gbr_or_byte(0,c);}while(0)
#define sfr_iclr(n,c)   do{set_gbr(IO_BASE+(n));gbr_and_byte(0,~(c));}while(0)
#endif
#endif

/* �o�X�E�R���g���[�� */

#define BSC_BCR1        0x0c20  /* �o�X�E�R���g���[���E���W�X�^1 */
#define BSC_BCR2        0x0c22  /* �o�X�E�R���g���[���E���W�X�^2 */
#define BSC_WCR         0x0c24  /* �E�F�C�g�E�X�e�[�g���䃌�W�X�^ */
#define BSC_RAMER       0x0c26  /* RAM �G�~�����[�V�������W�X�^ */

/* �v�c�s */

#define WDT_TCSR        0x0c10  /* �^�C�}�E�R���g���[���^�X�e�[�^�X�E���W�X�^ */
#define WDT_TCNT        0x0c10  /* �^�C�}�E�J�E���^�iwr�j*/
#define WDT_TCNT_R      0x0c11  /* �^�C�}�E�J�E���^�ird�j*/
#define WDT_RSTCSR      0x0c12  /* ���Z��g�E�R���g�����/�X�e��^�X�E���W�X�^�iwr�j*/
#define WDT_RSTCSR_R    0x0c13  /* ���Z��g�E�R���g�����/�X�e��^�X�E���W�X�^�ird�j*/

/* �c�l�`�b���� */

#define DMAC_DMAOR      0x0cb0  /* DMA �I�y���[�V�����E���W�X�^ */

/* �c�l�`�b�`���l���O */

#define DMAC_SAR0       0x0cc0  /* DMA �\�[�X�E�A�h���X�E���W�X�^0 */
#define DMAC_DAR0       0x0cc4  /* DMA �f�X�e�B�l�[�V�����E�A�h���X�E���W�X�^0 */
#define DMAC_DMATCR0    0x0cc8  /* DMA �g�����X�t�@�E�J�E���g�E���W�X�^0 */
#define DMAC_CHCR0      0x0ccc  /* DMA �`���l���E�R���g���[���E���W�X�^0 */

/* �c�l�`�b�`���l���P */

#define DMAC_SAR1       0x0cd0  /* DMA �\�[�X�E�A�h���X�E���W�X�^1 */
#define DMAC_DAR1       0x0cd4  /* DMA �f�X�e�B�l�[�V�����E�A�h���X�E���W�X�^1 */
#define DMAC_DMATCR1    0x0cd8  /* DMA �g�����X�t�@�E�J�E���g�E���W�X�^1 */
#define DMAC_CHCR1      0x0cdc  /* DMA �`���l���E�R���g���[���E���W�X�^1 */

/* �c�l�`�b�`���l���Q */

#define DMAC_SAR2       0x0ce0  /* DMA �\�[�X�E�A�h���X�E���W�X�^2 */
#define DMAC_DAR2       0x0ce4  /* DMA �f�X�e�B�l�[�V�����E�A�h���X�E���W�X�^2 */
#define DMAC_DMATCR2    0x0ce8  /* DMA �g�����X�t�@�E�J�E���g�E���W�X�^2 */
#define DMAC_CHCR2      0x0cec  /* DMA �`���l���E�R���g���[���E���W�X�^2 */

/* �c�l�`�b�`���l���R */

#define DMAC_SAR3       0x0cf0  /* DMA �\�[�X�E�A�h���X�E���W�X�^3 */
#define DMAC_DAR3       0x0cf4  /* DMA �f�X�e�B�l�[�V�����E�A�h���X�E���W�X�^3 */
#define DMAC_DMATCR3    0x0cf8  /* DMA �g�����X�t�@�E�J�E���g�E���W�X�^3 */
#define DMAC_CHCR3      0x0cfc  /* DMA �`���l���E�R���g���[���E���W�X�^3 */

/* �����݃R���g���[�� */

#define INTC_IPRA       0x0d00  /* �����ݗD�惌�x���ݒ背�W�X�^A */
#define INTC_IPRB       0x0d02  /* �����ݗD�惌�x���ݒ背�W�X�^B */
#define INTC_IPRC       0x0d04  /* �����ݗD�惌�x���ݒ背�W�X�^C */
#define INTC_IPRD       0x0d06  /* �����ݗD�惌�x���ݒ背�W�X�^D */
#define INTC_IPRE       0x0d08  /* �����ݗD�惌�x���ݒ背�W�X�^E */
#define INTC_IPRF       0x0d0a  /* �����ݗD�惌�x���ݒ背�W�X�^F */
#define INTC_IPRG       0x0d0c  /* �����ݗD�惌�x���ݒ背�W�X�^G */
#define INTC_IPRH       0x0d0e  /* �����ݗD�惌�x���ݒ背�W�X�^H */
#define INTC_IPRI       0x0d10  /* �����ݗD�惌�x���ݒ背�W�X�^I */
#define INTC_IPRJ       0x0d12  /* �����ݗD�惌�x���ݒ背�W�X�^J */
#define INTC_IPRK       0x0d14  /* �����ݗD�惌�x���ݒ背�W�X�^K */
#define INTC_IPRL       0x0d16  /* �����ݗD�惌�x���ݒ背�W�X�^L */
#define INTC_ICR        0x0d18  /* �����݃R���g���[�����W�X�^ */
#define INTC_ISR        0x0d1a  /* IRQ �X�e�[�^�X���W�X�^ */

/* �r�b�h�`���l���O */

#define SCI_SMR0        0x1000  /* �V���A���E���[�h�E���W�X�^ */
#define SCI_BRR0        0x1001  /* �r�b�g���[�g�E���W�X�^ */
#define SCI_SCR0        0x1002  /* �V���A���E�R���g���[���E���W�X�^ */
#define SCI_TDR0        0x1003  /* �g�����X�~�b�g�E�f�[�^�E���W�X�^ */
#define SCI_SSR0        0x1004  /* �V���A���E�X�e�[�^�X�E���W�X�^ */
#define SCI_RDR0        0x1005  /* ���V�[�u�E�f�[�^�E���W�X�^ */
#define SCI_SDCR0       0x1006  /* �V���A���f�B���N�V�����R���g���[�����W�X�^ */

/* �r�b�h�`���l���P */

#define SCI_SMR1        0x1008  /* �V���A���E���[�h�E���W�X�^ */
#define SCI_BRR1        0x1009  /* �r�b�g���[�g�E���W�X�^ */
#define SCI_SCR1        0x100a  /* �V���A���E�R���g���[���E���W�X�^ */
#define SCI_TDR1        0x100b  /* �g�����X�~�b�g�E�f�[�^�E���W�X�^ */
#define SCI_SSR1        0x100c  /* �V���A���E�X�e�[�^�X�E���W�X�^ */
#define SCI_RDR1        0x100d  /* ���V�[�u�E�f�[�^�E���W�X�^ */
#define SCI_SDCR1       0x100e  /* �V���A���f�B���N�V�����R���g���[�����W�X�^ */

/* �r�b�h�`���l���Q */

#define SCI_SMR2        0x1010  /* �V���A���E���[�h�E���W�X�^ */
#define SCI_BRR2        0x1011  /* �r�b�g���[�g�E���W�X�^ */
#define SCI_SCR2        0x1012  /* �V���A���E�R���g���[���E���W�X�^ */
#define SCI_TDR2        0x1013  /* �g�����X�~�b�g�E�f�[�^�E���W�X�^ */
#define SCI_SSR2        0x1014  /* �V���A���E�X�e�[�^�X�E���W�X�^ */
#define SCI_RDR2        0x1015  /* ���V�[�u�E�f�[�^�E���W�X�^ */
#define SCI_SDCR2       0x1016  /* �V���A���f�B���N�V�����R���g���[�����W�X�^ */

/* �r�b�h�`���l���R */

#define SCI_SMR3        0x1018  /* �V���A���E���[�h�E���W�X�^ */
#define SCI_BRR3        0x1019  /* �r�b�g���[�g�E���W�X�^ */
#define SCI_SCR3        0x101a  /* �V���A���E�R���g���[���E���W�X�^ */
#define SCI_TDR3        0x101b  /* �g�����X�~�b�g�E�f�[�^�E���W�X�^ */
#define SCI_SSR3        0x101c  /* �V���A���E�X�e�[�^�X�E���W�X�^ */
#define SCI_RDR3        0x101d  /* ���V�[�u�E�f�[�^�E���W�X�^ */
#define SCI_SDCR3       0x101e  /* �V���A���f�B���N�V�����R���g���[�����W�X�^ */

/* �r�b�h�`���l���S */

#define SCI_SMR4        0x1020  /* �V���A���E���[�h�E���W�X�^ */
#define SCI_BRR4        0x1021  /* �r�b�g���[�g�E���W�X�^ */
#define SCI_SCR4        0x1022  /* �V���A���E�R���g���[���E���W�X�^ */
#define SCI_TDR4        0x1023  /* �g�����X�~�b�g�E�f�[�^�E���W�X�^ */
#define SCI_SSR4        0x1024  /* �V���A���E�X�e�[�^�X�E���W�X�^ */
#define SCI_RDR4        0x1025  /* ���V�[�u�E�f�[�^�E���W�X�^ */
#define SCI_SDCR4       0x1026  /* �V���A���f�B���N�V�����R���g���[�����W�X�^ */

/* �`�s�t�|�U���� */

#define ATU_TSTR1       0x1401  /* �^�C�}�E�X�^�[�g�E���W�X�^1 */
#define ATU_TSTR2       0x1400  /* �^�C�}�E�X�^�[�g�E���W�X�^2 */
#define ATU_TSTR3       0x1402  /* �^�C�}�E�X�^�[�g�E���W�X�^3 */
#define ATU_PSCR1       0x1404  /* �v���X�P�[���E���W�X�^1 */
#define ATU_PSCR2       0x1406  /* �v���X�P�[���E���W�X�^2 */
#define ATU_PSCR3       0x1408  /* �v���X�P�[���E���W�X�^3 */
#define ATU_PSCR4       0x140a  /* �v���X�P�[���E���W�X�^4 */

/* �`�s�t�|�U�`���l���O */

#define ATU_TCNT0H      0x1430  /* �t���[�����j���O�E���W�X�^H */
#define ATU_TCNT0L      0x1430  /* �t���[�����j���O�E���W�X�^L */
#define ATU_ICR0AH      0x1434  /* �C���v�b�g�L���v�`���E���W�X�^AH */
#define ATU_ICR0AL      0x1434  /* �C���v�b�g�L���v�`���E���W�X�^AL */
#define ATU_ICR0BH      0x1438  /* �C���v�b�g�L���v�`���E���W�X�^BH */
#define ATU_ICR0BL      0x1438  /* �C���v�b�g�L���v�`���E���W�X�^BL */
#define ATU_ICR0CH      0x143c  /* �C���v�b�g�L���v�`���E���W�X�^CH */
#define ATU_ICR0CL      0x143c  /* �C���v�b�g�L���v�`���E���W�X�^CL */
#define ATU_ICR0DH      0x1420  /* �C���v�b�g�L���v�`���E���W�X�^DH */
#define ATU_ICR0DL      0x1420  /* �C���v�b�g�L���v�`���E���W�X�^DL */
#define ATU_ITVRR1      0x1424  /* �^�C�}�C���^�[�o���C���^���v�g���N�G�X�g�E���W�X�^1 */
#define ATU_ITVRR2A     0x1426  /* �^�C�}�C���^�[�o���C���^���v�g���N�G�X�g�E���W�X�^2A */
#define ATU_ITVRR2B     0x1428  /* �^�C�}�C���^�[�o���C���^���v�g���N�G�X�g�E���W�X�^2B */
#define ATU_TIOR0       0x142a  /* �^�C�}I/O�R���g���[���E���W�X�^ */
#define ATU_TSR0        0x142c  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER0       0x142e  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */

/* �`�s�t�|�U�`���l���P */

#define ATU_TCNT1A      0x1440  /* �t���[�����j���O�E���W�X�^A */
#define ATU_TCNT1B      0x1442  /* �t���[�����j���O�E���W�X�^B */
#define ATU_GR1A        0x1444  /* �W�F�l�����E���W�X�^A */
#define ATU_GR1B        0x1446  /* �W�F�l�����E���W�X�^B */
#define ATU_GR1C        0x1448  /* �W�F�l�����E���W�X�^C */
#define ATU_GR1D        0x144a  /* �W�F�l�����E���W�X�^D */
#define ATU_GR1E        0x144c  /* �W�F�l�����E���W�X�^E */
#define ATU_GR1F        0x144e  /* �W�F�l�����E���W�X�^F */
#define ATU_GR1G        0x1450  /* �W�F�l�����E���W�X�^G */
#define ATU_GR1H        0x1452  /* �W�F�l�����E���W�X�^H */
#define ATU_OCR1        0x1454  /* �A�E�g�v�b�g�R���y�A�E���W�X�^ */
#define ATU_OSBR1       0x1456  /* �I�t�Z�b�g�x�[�X�E���W�X�^ */
#define ATU_TIOR1A      0x1459  /* �^�C�}I/O�R���g���[���E���W�X�^A */
#define ATU_TIOR1B      0x1458  /* �^�C�}I/O�R���g���[���E���W�X�^B */
#define ATU_TIOR1C      0x145b  /* �^�C�}I/O�R���g���[���E���W�X�^C */
#define ATU_TIOR1D      0x145a  /* �^�C�}I/O�R���g���[���E���W�X�^D */
#define ATU_TCR1B       0x145c  /* �^�C�}�R���g���[���E���W�X�^B */
#define ATU_TSR1A       0x145e  /* �^�C�}�X�e�[�^�X�E���W�X�^A */
#define ATU_TSR1B       0x1460  /* �^�C�}�X�e�[�^�X�E���W�X�^B */
#define ATU_TIER1A      0x1462  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^A */
#define ATU_TIER1B      0x1464  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^B */
#define ATU_TRGMDR      0x1466  /* �g���K���[�h�E���W�X�^ */

/* �`�s�t�|�U�`���l���Q */

#define ATU_TCNT2A      0x1600  /* �t���[�����j���O�E���W�X�^A */
#define ATU_TCNT2B      0x1602  /* �t���[�����j���O�E���W�X�^B */
#define ATU_GR2A        0x1604  /* �W�F�l�����E���W�X�^A */
#define ATU_GR2B        0x1606  /* �W�F�l�����E���W�X�^B */
#define ATU_GR2C        0x1608  /* �W�F�l�����E���W�X�^C */
#define ATU_GR2D        0x160a  /* �W�F�l�����E���W�X�^D */
#define ATU_GR2E        0x160c  /* �W�F�l�����E���W�X�^E */
#define ATU_GR2F        0x160e  /* �W�F�l�����E���W�X�^F */
#define ATU_GR2G        0x1610  /* �W�F�l�����E���W�X�^G */
#define ATU_GR2H        0x1612  /* �W�F�l�����E���W�X�^H */
#define ATU_OCR2A       0x1614  /* �A�E�g�v�b�g�R���y�A�E���W�X�^A */
#define ATU_OCR2B       0x1616  /* �A�E�g�v�b�g�R���y�A�E���W�X�^B */
#define ATU_OCR2C       0x1618  /* �A�E�g�v�b�g�R���y�A�E���W�X�^C */
#define ATU_OCR2D       0x161a  /* �A�E�g�v�b�g�R���y�A�E���W�X�^D */
#define ATU_OCR2E       0x161c  /* �A�E�g�v�b�g�R���y�A�E���W�X�^E */
#define ATU_OCR2F       0x161e  /* �A�E�g�v�b�g�R���y�A�E���W�X�^F */
#define ATU_OCR2G       0x1620  /* �A�E�g�v�b�g�R���y�A�E���W�X�^G */
#define ATU_OCR2H       0x1622  /* �A�E�g�v�b�g�R���y�A�E���W�X�^H */
#define ATU_OSBR2       0x1624  /* �I�t�Z�b�g�x�[�X�E���W�X�^ */
#define ATU_TIOR2A      0x1627  /* �^�C�}I/O�R���g���[���E���W�X�^A */
#define ATU_TIOR2B      0x1626  /* �^�C�}I/O�R���g���[���E���W�X�^B */
#define ATU_TIOR2C      0x1629  /* �^�C�}I/O�R���g���[���E���W�X�^C */
#define ATU_TIOR2D      0x1628  /* �^�C�}I/O�R���g���[���E���W�X�^D */
#define ATU_TCR2A       0x162b  /* �^�C�}�R���g���[���E���W�X�^A */
#define ATU_TCR2B       0x162a  /* �^�C�}�R���g���[���E���W�X�^B */
#define ATU_TSR2A       0x162c  /* �^�C�}�X�e�[�^�X�E���W�X�^A */
#define ATU_TSR2B       0x162e  /* �^�C�}�X�e�[�^�X�E���W�X�^B */
#define ATU_TIER2A      0x1630  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^A */
#define ATU_TIER2B      0x1632  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^B */

/* �`�s�t�|�U�`���l���R�`�T���� */

#define ATU_TSR3        0x1480  /* �^�C�}�X�e�[�^�X�E���W�X�^3 */
#define ATU_TIER3       0x1482  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^3 */
#define ATU_TMDR3       0x1484  /* �^�C�}���[�h�E���W�X�^ */

/* �`�s�t�|�U�`���l���R */

#define ATU_TCNT3       0x14a0  /* �t���[�����j���O�E�J�E���^ */
#define ATU_GR3A        0x14a2  /* �W�F�l�����E���W�X�^A */
#define ATU_GR3B        0x14a4  /* �W�F�l�����E���W�X�^B */
#define ATU_GR3C        0x14a6  /* �W�F�l�����E���W�X�^C */
#define ATU_GR3D        0x14a8  /* �W�F�l�����E���W�X�^D */
#define ATU_TIOR3A      0x14ab  /* �^�C�}I/O�R���g���[���E���W�X�^A */
#define ATU_TIOR3B      0x14aa  /* �^�C�}I/O�R���g���[���E���W�X�^B */
#define ATU_TCR3        0x14ac  /* �^�C�}�R���g���[���E���W�X�^ */

/* �`�s�t�|�U�`���l���S */

#define ATU_TCNT4       0x14c0  /* �t���[�����j���O�E�J�E���^ */
#define ATU_GR4A        0x14c2  /* �W�F�l�����E���W�X�^A */
#define ATU_GR4B        0x14c4  /* �W�F�l�����E���W�X�^B */
#define ATU_GR4C        0x14c6  /* �W�F�l�����E���W�X�^C */
#define ATU_GR4D        0x14c8  /* �W�F�l�����E���W�X�^D */
#define ATU_TIOR4A      0x14cb  /* �^�C�}I/O�R���g���[���E���W�X�^A */
#define ATU_TIOR4B      0x14ca  /* �^�C�}I/O�R���g���[���E���W�X�^B */
#define ATU_TCR4        0x14cc  /* �^�C�}�R���g���[���E���W�X�^ */

/* �`�s�t�|�U�`���l���T */

#define ATU_TCNT5       0x14e0  /* �t���[�����j���O�E�J�E���^ */
#define ATU_GR5A        0x14e2  /* �W�F�l�����E���W�X�^A */
#define ATU_GR5B        0x14e4  /* �W�F�l�����E���W�X�^B */
#define ATU_GR5C        0x14e6  /* �W�F�l�����E���W�X�^C */
#define ATU_GR5D        0x14e8  /* �W�F�l�����E���W�X�^D */
#define ATU_TIOR5A      0x14eb  /* �^�C�}I/O�R���g���[���E���W�X�^A */
#define ATU_TIOR5B      0x14ea  /* �^�C�}I/O�R���g���[���E���W�X�^B */
#define ATU_TCR5        0x14ec  /* �^�C�}�R���g���[���E���W�X�^ */

/* �`�s�t�|�U�`���l���U */

#define ATU_TCNT6A      0x1500  /* �t���[�����j���O�E���W�X�^A */
#define ATU_TCNT6B      0x1502  /* �t���[�����j���O�E���W�X�^B */
#define ATU_TCNT6C      0x1504  /* �t���[�����j���O�E���W�X�^C */
#define ATU_TCNT6D      0x1506  /* �t���[�����j���O�E���W�X�^D */
#define ATU_CYLR6A      0x1508  /* �T�C�N���E���W�X�^A */
#define ATU_CYLR6B      0x150a  /* �T�C�N���E���W�X�^B */
#define ATU_CYLR6C      0x150c  /* �T�C�N���E���W�X�^C */
#define ATU_CYLR6D      0x150e  /* �T�C�N���E���W�X�^D */
#define ATU_BFR6A       0x1510  /* �o�b�t�@�E���W�X�^A */
#define ATU_BFR6B       0x1512  /* �o�b�t�@�E���W�X�^B */
#define ATU_BFR6C       0x1514  /* �o�b�t�@�E���W�X�^C */
#define ATU_BFR6D       0x1516  /* �o�b�t�@�E���W�X�^D */
#define ATU_DTR6A       0x1518  /* �f���[�e�B�E���W�X�^A */
#define ATU_DTR6B       0x151a  /* �f���[�e�B�E���W�X�^B */
#define ATU_DTR6C       0x151c  /* �f���[�e�B�E���W�X�^C */
#define ATU_DTR6D       0x151e  /* �f���[�e�B�E���W�X�^D */
#define ATU_TCR6A       0x1521  /* �^�C�}�R���g���[���E���W�X�^A */
#define ATU_TCR6B       0x1520  /* �^�C�}�R���g���[���E���W�X�^B */
#define ATU_TSR6        0x1522  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER6       0x1524  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */
#define ATU_PWMD        0x1526  /* PWM ���[�h�E���W�X�^ */

/* �`�s�t�|�U�`���l���V */

#define ATU_TCNT7A      0x1580  /* �t���[�����j���O�E���W�X�^A */
#define ATU_TCNT7B      0x1582  /* �t���[�����j���O�E���W�X�^B */
#define ATU_TCNT7C      0x1584  /* �t���[�����j���O�E���W�X�^C */
#define ATU_TCNT7D      0x1586  /* �t���[�����j���O�E���W�X�^D */
#define ATU_CYLR7A      0x1588  /* �T�C�N���E���W�X�^A */
#define ATU_CYLR7B      0x158a  /* �T�C�N���E���W�X�^B */
#define ATU_CYLR7C      0x158c  /* �T�C�N���E���W�X�^C */
#define ATU_CYLR7D      0x158e  /* �T�C�N���E���W�X�^D */
#define ATU_BFR7A       0x1590  /* �o�b�t�@�E���W�X�^A */
#define ATU_BFR7B       0x1592  /* �o�b�t�@�E���W�X�^B */
#define ATU_BFR7C       0x1594  /* �o�b�t�@�E���W�X�^C */
#define ATU_BFR7D       0x1596  /* �o�b�t�@�E���W�X�^D */
#define ATU_DTR7A       0x1598  /* �f���[�e�B�E���W�X�^A */
#define ATU_DTR7B       0x159a  /* �f���[�e�B�E���W�X�^B */
#define ATU_DTR7C       0x159c  /* �f���[�e�B�E���W�X�^C */
#define ATU_DTR7D       0x159e  /* �f���[�e�B�E���W�X�^D */
#define ATU_TCR7A       0x15a1  /* �^�C�}�R���g���[���E���W�X�^A */
#define ATU_TCR7B       0x15a0  /* �^�C�}�R���g���[���E���W�X�^B */
#define ATU_TSR7        0x15a2  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER7       0x15a4  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */

/* �`�s�t�|�U�`���l���W */

#define ATU_DCNT8A      0x1640  /* �_�E���J�E���^A */
#define ATU_DCNT8B      0x1642  /* �_�E���J�E���^B */
#define ATU_DCNT8C      0x1644  /* �_�E���J�E���^C */
#define ATU_DCNT8D      0x1646  /* �_�E���J�E���^D */
#define ATU_DCNT8E      0x1648  /* �_�E���J�E���^E */
#define ATU_DCNT8F      0x164a  /* �_�E���J�E���^F */
#define ATU_DCNT8G      0x164c  /* �_�E���J�E���^G */
#define ATU_DCNT8H      0x164e  /* �_�E���J�E���^H */
#define ATU_DCNT8I      0x1650  /* �_�E���J�E���^I */
#define ATU_DCNT8J      0x1652  /* �_�E���J�E���^J */
#define ATU_DCNT8K      0x1654  /* �_�E���J�E���^K */
#define ATU_DCNT8L      0x1656  /* �_�E���J�E���^L */
#define ATU_DCNT8M      0x1658  /* �_�E���J�E���^M */
#define ATU_DCNT8N      0x165a  /* �_�E���J�E���^N */
#define ATU_DCNT8O      0x165c  /* �_�E���J�E���^O */
#define ATU_DCNT8P      0x165e  /* �_�E���J�E���^P */
#define ATU_RLDR8       0x1660  /* �����[�h�E���W�X�^ */
#define ATU_TCNR        0x1662  /* �^�C�}�R�l�N�V�����E���W�X�^ */
#define ATU_OTR         0x1664  /* �����V���b�g�p���X�^�[�~�l�[�g�E���W�X�^ */
#define ATU_DSTR        0x1666  /* �_�E���J�E���g�X�^�[�g�E���W�X�^ */
#define ATU_TCR8        0x1668  /* �^�C�}�R���g���[���E���W�X�^ */
#define ATU_TSR8        0x166a  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER8       0x166c  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */
#define ATU_RLDENR      0x166e  /* �����[�h�C�l�[�u���E���W�X�^ */

/* �`�s�t�|�U�`���l���X */

#define ATU_ECNT9A      0x1680  /* �C�l�[�u���J�E���^A */
#define ATU_ECNT9B      0x1682  /* �C�l�[�u���J�E���^B */
#define ATU_ECNT9C      0x1684  /* �C�l�[�u���J�E���^C */
#define ATU_ECNT9D      0x1686  /* �C�l�[�u���J�E���^D */
#define ATU_ECNT9E      0x1688  /* �C�l�[�u���J�E���^E */
#define ATU_ECNT9F      0x168a  /* �C�l�[�u���J�E���^F */
#define ATU_GR9A        0x168c  /* �W�F�l�����E���W�X�^A */
#define ATU_GR9B        0x168e  /* �W�F�l�����E���W�X�^B */
#define ATU_GR9C        0x1690  /* �W�F�l�����E���W�X�^C */
#define ATU_GR9D        0x1692  /* �W�F�l�����E���W�X�^D */
#define ATU_GR9E        0x1694  /* �W�F�l�����E���W�X�^E */
#define ATU_GR9F        0x1696  /* �W�F�l�����E���W�X�^F */
#define ATU_TCR9A       0x1698  /* �^�C�}�R���g���[���E���W�X�^A */
#define ATU_TCR9B       0x169a  /* �^�C�}�R���g���[���E���W�X�^B */
#define ATU_TCR9C       0x169c  /* �^�C�}�R���g���[���E���W�X�^C */
#define ATU_TSR9        0x169e  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER9       0x16a0  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */

/* �`�s�t�|�U�`���l���P�O */

#define ATU_TCNT10AH    0x16c0  /* �t���[�����j���O�E���W�X�^AH */
#define ATU_TCNT10AL    0x16c0  /* �t���[�����j���O�E���W�X�^AL */
#define ATU_TCNT10B     0x16c4  /* �C�x���g�J�E���^B */
#define ATU_TCNT10C     0x16c6  /* �����[�h�J�E���^C */
#define ATU_TCNT10D     0x16c8  /* �␳�J�E���^D */
#define ATU_TCNT10E     0x16ca  /* �␳�J�E���^E */
#define ATU_TCNT10F     0x16cc  /* �␳�J�E���^F */
#define ATU_TCNT10G     0x16ce  /* �t���[�����j���O�E���W�X�^G */
#define ATU_ICR10AH     0x16d0  /* �C���v�b�g�L���v�`���E���W�X�^AH */
#define ATU_ICR10AL     0x16d0  /* �C���v�b�g�L���v�`���E���W�X�^AL */
#define ATU_OCR10AH     0x16d4  /* �A�E�g�v�b�g�R���y�A�E���W�X�^AH */
#define ATU_OCR10AL     0x16d4  /* �A�E�g�v�b�g�R���y�A�E���W�X�^AL */
#define ATU_OCR10B      0x16d8  /* �A�E�g�v�b�g�R���y�A�E���W�X�^B */
#define ATU_RLD10C      0x16da  /* �����[�h�E���W�X�^C */
#define ATU_GR10G       0x16dc  /* �W�F�l�����E���W�X�^G */
#define ATU_TCNT10H     0x16de  /* �m�C�Y�L�����Z���E�J�E���^H */
#define ATU_NCR10       0x16e0  /* �m�C�Y�L�����Z���E���W�X�^ */
#define ATU_TIOR10      0x16e2  /* �^�C�}I/O�R���g���[���E���W�X�^ */
#define ATU_TCR10       0x16e4  /* �^�C�}�R���g���[���E���W�X�^ */
#define ATU_TCCLR10     0x16e6  /* �␳�J�E���^�N���A�E���W�X�^ */
#define ATU_TSR10       0x16e8  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER10      0x16ea  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */

/* �`�s�t�|�U�`���l���P�P */

#define ATU_TCNT11      0x15c0  /* �t���[�����j���O�E���W�X�^ */
#define ATU_GR11A       0x15c2  /* �W�F�l�����E���W�X�^A */
#define ATU_GR11B       0x15c4  /* �W�F�l�����E���W�X�^B */
#define ATU_TIOR11      0x15c6  /* �^�C�}I/O�R���g���[���E���W�X�^ */
#define ATU_TCR11       0x15c8  /* �^�C�}�R���g���[���E���W�X�^ */
#define ATU_TSR11       0x15ca  /* �^�C�}�X�e�[�^�X�E���W�X�^ */
#define ATU_TIER11      0x15cc  /* �^�C�}�C���^���v�g�C�l�[�u���E���W�X�^ */

/* �p���X�R���g���[�� */

#define APC_POPCR       0x1700  /* �p���X�A�E�g�v�b�g�|�[�g�R���g���[���E���W�X�^ */

/* �ȓd�̓��[�h���� */

#define SYS_SBYCR       0x0c14  /* �X�^���o�C�R���g���[���E���W�X�^ */
#define SYS_SYSCR       0x1708  /* �V�X�e���R���g���[���E���W�X�^ */
#define SYS_MSTCR       0x170a  /* ���W���[���X�^���o�C�R���g���[���E���W�X�^(WR) */
#define SYS_MSTCR_R     0x170b  /* ���W���[���X�^���o�C�R���g���[���E���W�X�^(RD) */

/* �R���y�A�}�b�`�^�C�}���� */

#define CMT_CMSTR       0x1710  /* �R���y�A�}�b�`�^�C�}�X�^�[�g�E���W�X�^ */

/* �R���y�A�}�b�`�^�C�}�O */

#define CMT_CMCSR0      0x1712  /* �R���y�A�}�b�`�^�C�}�R���g���[��/�X�e�[�^�X�E���W�X�^ */
#define CMT_CMCNT0      0x1714  /* �R���y�A�}�b�`�^�C�}�E�J�E���^ */
#define CMT_CMCOR0      0x1716  /* �R���y�A�}�b�`�^�C�}�R���X�^���g�E���W�X�^ */

/* �R���y�A�}�b�`�^�C�}�P */

#define CMT_CMCSR1      0x1718  /* �R���y�A�}�b�`�^�C�}�R���g���[��/�X�e�[�^�X�E���W�X�^ */
#define CMT_CMCNT1      0x171a  /* �R���y�A�}�b�`�^�C�}�E�J�E���^ */
#define CMT_CMCOR1      0x171c  /* �R���y�A�}�b�`�^�C�}�R���X�^���g�E���W�X�^ */

/* �`�^�c */

#define AD_ADDR0H       0x1800  /* A/D �f�[�^�E���W�X�^0  H�iBit9�`2�j*/
#define AD_ADDR0L       0x1801  /* A/D �f�[�^�E���W�X�^0  L�iBit1�`0�j*/
#define AD_ADDR1H       0x1802  /* A/D �f�[�^�E���W�X�^1  H�iBit9�`2�j*/
#define AD_ADDR1L       0x1803  /* A/D �f�[�^�E���W�X�^1  L�iBit1�`0�j*/
#define AD_ADDR2H       0x1804  /* A/D �f�[�^�E���W�X�^2  H�iBit9�`2�j*/
#define AD_ADDR2L       0x1805  /* A/D �f�[�^�E���W�X�^2  L�iBit1�`0�j*/
#define AD_ADDR3H       0x1806  /* A/D �f�[�^�E���W�X�^3  H�iBit9�`2�j*/
#define AD_ADDR3L       0x1807  /* A/D �f�[�^�E���W�X�^3  L�iBit1�`0�j*/
#define AD_ADDR4H       0x1808  /* A/D �f�[�^�E���W�X�^4  H�iBit9�`2�j*/
#define AD_ADDR4L       0x1809  /* A/D �f�[�^�E���W�X�^4  L�iBit1�`0�j*/
#define AD_ADDR5H       0x180a  /* A/D �f�[�^�E���W�X�^5  H�iBit9�`2�j*/
#define AD_ADDR5L       0x180b  /* A/D �f�[�^�E���W�X�^5  L�iBit1�`0�j*/
#define AD_ADDR6H       0x180c  /* A/D �f�[�^�E���W�X�^6  H�iBit9�`2�j*/
#define AD_ADDR6L       0x180d  /* A/D �f�[�^�E���W�X�^6  L�iBit1�`0�j*/
#define AD_ADDR7H       0x180e  /* A/D �f�[�^�E���W�X�^7  H�iBit9�`2�j*/
#define AD_ADDR7L       0x180f  /* A/D �f�[�^�E���W�X�^7  L�iBit1�`0�j*/
#define AD_ADDR8H       0x1810  /* A/D �f�[�^�E���W�X�^8  H�iBit9�`2�j*/
#define AD_ADDR8L       0x1811  /* A/D �f�[�^�E���W�X�^8  L�iBit1�`0�j*/
#define AD_ADDR9H       0x1812  /* A/D �f�[�^�E���W�X�^9  H�iBit9�`2�j*/
#define AD_ADDR9L       0x1813  /* A/D �f�[�^�E���W�X�^9  L�iBit1�`0�j*/
#define AD_ADDR10H      0x1814  /* A/D �f�[�^�E���W�X�^10 H�iBit9�`2�j*/
#define AD_ADDR10L      0x1815  /* A/D �f�[�^�E���W�X�^10 L�iBit1�`0�j*/
#define AD_ADDR11H      0x1816  /* A/D �f�[�^�E���W�X�^11 H�iBit9�`2�j*/
#define AD_ADDR11L      0x1817  /* A/D �f�[�^�E���W�X�^11 L�iBit1�`0�j*/
#define AD_ADDR12H      0x1820  /* A/D �f�[�^�E���W�X�^12 H�iBit9�`2�j*/
#define AD_ADDR12L      0x1821  /* A/D �f�[�^�E���W�X�^12 L�iBit1�`0�j*/
#define AD_ADDR13H      0x1822  /* A/D �f�[�^�E���W�X�^13 H�iBit9�`2�j*/
#define AD_ADDR13L      0x1823  /* A/D �f�[�^�E���W�X�^13 L�iBit1�`0�j*/
#define AD_ADDR14H      0x1824  /* A/D �f�[�^�E���W�X�^14 H�iBit9�`2�j*/
#define AD_ADDR14L      0x1825  /* A/D �f�[�^�E���W�X�^14 L�iBit1�`0�j*/
#define AD_ADDR15H      0x1826  /* A/D �f�[�^�E���W�X�^15 H�iBit9�`2�j*/
#define AD_ADDR15L      0x1827  /* A/D �f�[�^�E���W�X�^15 L�iBit1�`0�j*/
#define AD_ADDR16H      0x1828  /* A/D �f�[�^�E���W�X�^16 H�iBit9�`2�j*/
#define AD_ADDR16L      0x1829  /* A/D �f�[�^�E���W�X�^16 L�iBit1�`0�j*/
#define AD_ADDR17H      0x182a  /* A/D �f�[�^�E���W�X�^17 H�iBit9�`2�j*/
#define AD_ADDR17L      0x182b  /* A/D �f�[�^�E���W�X�^17 L�iBit1�`0�j*/
#define AD_ADDR18H      0x182c  /* A/D �f�[�^�E���W�X�^18 H�iBit9�`2�j*/
#define AD_ADDR18L      0x182d  /* A/D �f�[�^�E���W�X�^18 L�iBit1�`0�j*/
#define AD_ADDR19H      0x182e  /* A/D �f�[�^�E���W�X�^19 H�iBit9�`2�j*/
#define AD_ADDR19L      0x182f  /* A/D �f�[�^�E���W�X�^19 L�iBit1�`0�j*/
#define AD_ADDR20H      0x1830  /* A/D �f�[�^�E���W�X�^20 H�iBit9�`2�j*/
#define AD_ADDR20L      0x1831  /* A/D �f�[�^�E���W�X�^20 L�iBit1�`0�j*/
#define AD_ADDR21H      0x1832  /* A/D �f�[�^�E���W�X�^21 H�iBit9�`2�j*/
#define AD_ADDR21L      0x1833  /* A/D �f�[�^�E���W�X�^21 L�iBit1�`0�j*/
#define AD_ADDR22H      0x1834  /* A/D �f�[�^�E���W�X�^22 H�iBit9�`2�j*/
#define AD_ADDR22L      0x1835  /* A/D �f�[�^�E���W�X�^22 L�iBit1�`0�j*/
#define AD_ADDR23H      0x1836  /* A/D �f�[�^�E���W�X�^23 H�iBit9�`2�j*/
#define AD_ADDR23L      0x1837  /* A/D �f�[�^�E���W�X�^23 L�iBit1�`0�j*/
#define AD_ADDR24H      0x1840  /* A/D �f�[�^�E���W�X�^24 H�iBit9�`2�j*/
#define AD_ADDR24L      0x1841  /* A/D �f�[�^�E���W�X�^24 L�iBit1�`0�j*/
#define AD_ADDR25H      0x1842  /* A/D �f�[�^�E���W�X�^25 H�iBit9�`2�j*/
#define AD_ADDR25L      0x1843  /* A/D �f�[�^�E���W�X�^25 L�iBit1�`0�j*/
#define AD_ADDR26H      0x1844  /* A/D �f�[�^�E���W�X�^26 H�iBit9�`2�j*/
#define AD_ADDR26L      0x1845  /* A/D �f�[�^�E���W�X�^26 L�iBit1�`0�j*/
#define AD_ADDR27H      0x1846  /* A/D �f�[�^�E���W�X�^27 H�iBit9�`2�j*/
#define AD_ADDR27L      0x1847  /* A/D �f�[�^�E���W�X�^27 L�iBit1�`0�j*/
#define AD_ADDR28H      0x1848  /* A/D �f�[�^�E���W�X�^28 H�iBit9�`2�j*/
#define AD_ADDR28L      0x1849  /* A/D �f�[�^�E���W�X�^28 L�iBit1�`0�j*/
#define AD_ADDR29H      0x184a  /* A/D �f�[�^�E���W�X�^29 H�iBit9�`2�j*/
#define AD_ADDR29L      0x184b  /* A/D �f�[�^�E���W�X�^29 L�iBit1�`0�j*/
#define AD_ADDR30H      0x184c  /* A/D �f�[�^�E���W�X�^30 H�iBit9�`2�j*/
#define AD_ADDR30L      0x184d  /* A/D �f�[�^�E���W�X�^30 L�iBit1�`0�j*/
#define AD_ADDR31H      0x184e  /* A/D �f�[�^�E���W�X�^31 H�iBit9�`2�j*/
#define AD_ADDR31L      0x184f  /* A/D �f�[�^�E���W�X�^31 L�iBit1�`0�j*/
#define AD_ADCSR0       0x1818  /* A/D �R���g���[���^�X�e�[�^�X�E���W�X�^0 */
#define AD_ADCR0        0x1819  /* A/D �R���g���[���E���W�X�^0 */
#define AD_ADTRGR0      0x176e  /* A/D �g���K�E���W�X�^0 */
#define AD_ADCSR1       0x1838  /* A/D �R���g���[���^�X�e�[�^�X�E���W�X�^1 */
#define AD_ADCR1        0x1839  /* A/D �R���g���[���E���W�X�^1 */
#define AD_ADTRGR1      0x172e  /* A/D �g���K�E���W�X�^1 */
#define AD_ADCSR2       0x1858  /* A/D �R���g���[���^�X�e�[�^�X�E���W�X�^2 */
#define AD_ADCR2        0x1859  /* A/D �R���g���[���E���W�X�^2 */
#define AD_ADTRGR2      0x172f  /* A/D �g���K�E���W�X�^2 */

/* �|�[�g�`�`�k */

#define PA_DR           0x1726  /* �|�[�gA �f�[�^�E���W�X�^ */
#define PB_DR           0x1738  /* �|�[�gB �f�[�^�E���W�X�^ */
#define PC_DR           0x173e  /* �|�[�gC �f�[�^�E���W�X�^ */
#define PD_DR           0x1746  /* �|�[�gD �f�[�^�E���W�X�^ */
#define PE_DR           0x1754  /* �|�[�gE �f�[�^�E���W�X�^ */
#define PF_DR           0x174e  /* �|�[�gF �f�[�^�E���W�X�^ */
#define PG_DR           0x1764  /* �|�[�gG �f�[�^�E���W�X�^ */
#define PH_DR           0x172c  /* �|�[�gH �f�[�^�E���W�X�^ */
#define PJ_DR           0x176c  /* �|�[�gJ �f�[�^�E���W�X�^ */
#define PK_DR           0x1778  /* �|�[�gK �f�[�^�E���W�X�^ */
#define PL_DR           0x175e  /* �|�[�gL �f�[�^�E���W�X�^ */

/* �s���E�t�@���N�V�����E�R���g���[�� */

#define PFC_PAIOR       0x1720  /* �|�[�gA �h�n���W�X�^ */
#define PFC_PACRH       0x1722  /* �|�[�gA �R���g���[���E���W�X�^H */
#define PFC_PACRL       0x1724  /* �|�[�gA �R���g���[���E���W�X�^L */
#define PFC_PBIOR       0x1730  /* �|�[�gB �h�n���W�X�^ */
#define PFC_PBCRH       0x1732  /* �|�[�gB �R���g���[���E���W�X�^H */
#define PFC_PBCRL       0x1734  /* �|�[�gB �R���g���[���E���W�X�^L */
#define PFC_PBIR        0x1736  /* �|�[�gB �C���o�[�g�E���W�X�^ */
#define PFC_PCIOR       0x173a  /* �|�[�gC �h�n���W�X�^ */
#define PFC_PCCR        0x173c  /* �|�[�gC �R���g���[���E���W�X�^ */
#define PFC_PDIOR       0x1740  /* �|�[�gD �h�n���W�X�^ */
#define PFC_PDCRH       0x1742  /* �|�[�gD �R���g���[���E���W�X�^H */
#define PFC_PDCRL       0x1744  /* �|�[�gD �R���g���[���E���W�X�^L */
#define PFC_PEIOR       0x1750  /* �|�[�gE �h�n���W�X�^ */
#define PFC_PECR        0x1752  /* �|�[�gE �R���g���[���E���W�X�^ */
#define PFC_PFIOR       0x1748  /* �|�[�gF �h�n���W�X�^ */
#define PFC_PFCRH       0x174a  /* �|�[�gF �R���g���[���E���W�X�^H */
#define PFC_PFCRL       0x174c  /* �|�[�gF �R���g���[���E���W�X�^L */
#define PFC_PGIOR       0x1760  /* �|�[�gG �h�n���W�X�^ */
#define PFC_PGCR        0x1762  /* �|�[�gG �R���g���[���E���W�X�^ */
#define PFC_PHIOR       0x1728  /* �|�[�gH �h�n���W�X�^ */
#define PFC_PHCR        0x172a  /* �|�[�gH �R���g���[���E���W�X�^ */
#define PFC_PJIOR       0x1766  /* �|�[�gJ �h�n���W�X�^ */
#define PFC_PJCRH       0x1768  /* �|�[�gJ �R���g���[���E���W�X�^H */
#define PFC_PJCRL       0x176a  /* �|�[�gJ �R���g���[���E���W�X�^L */
#define PFC_PKIOR       0x1770  /* �|�[�gK �h�n���W�X�^ */
#define PFC_PKCRH       0x1772  /* �|�[�gK �R���g���[���E���W�X�^H */
#define PFC_PKCRL       0x1774  /* �|�[�gK �R���g���[���E���W�X�^L */
#define PFC_PKIR        0x1776  /* �|�[�gK �C���o�[�g�E���W�X�^ */
#define PFC_PLIOR       0x1756  /* �|�[�gL �h�n���W�X�^ */
#define PFC_PLCRH       0x1758  /* �|�[�gL �R���g���[���E���W�X�^H */
#define PFC_PLCRL       0x175a  /* �|�[�gL �R���g���[���E���W�X�^L */
#define PFC_PLIR        0x175c  /* �|�[�gL �C���o�[�g�E���W�X�^ */

/* ���[�U�u���[�N�E�R���g���[�� */

#define UBC_UBARH       0x0c00  /* �u���[�N�E�A�h���X�E���W�X�^�g */
#define UBC_UBARL       0x0c02  /* �u���[�N�E�A�h���X�E���W�X�^�k */
#define UBC_UBAMRH      0x0c04  /* �u���[�N�E�A�h���X�E�}�X�N�E���W�X�^�g */
#define UBC_UBAMRL      0x0c06  /* �u���[�N�E�A�h���X�E�}�X�N�E���W�X�^�k */
#define UBC_UBBR        0x0c08  /* �u���[�N�E�o�X�T�C�N���E���W�X�^ */
#define UBC_UBCR        0x0c0a  /* �u���[�N�E�R���g���[���E���W�X�^ */

/* �t���b�V�������� */

#define FLASH_FLMCR1    0x0800  /* �t���b�V���������R���g���[���E���W�X�^1 */
#define FLASH_FLMCR2    0x0801  /* �t���b�V���������R���g���[���E���W�X�^2 */
#define FLASH_EBR1      0x0802  /* �u���b�N�w��E���W�X�^1 */
#define FLASH_EBR2      0x0803  /* �u���b�N�w��E���W�X�^2 */

/* �����݃x�N�^�ԍ� */

#define INT_NMI         11      /* NMI */
#define INT_UB          12      /* ���[�U�u���[�N */
#define INT_UDI         14      /* H-UDI �u���[�N */
#define INT_IRQ0        64      /* �O���[�q IRQ0 */
#define INT_IRQ1        65      /* �O���[�q IRQ1 */
#define INT_IRQ2        66      /* �O���[�q IRQ2 */
#define INT_IRQ3        67      /* �O���[�q IRQ3 */
#define INT_IRQ4        68      /* �O���[�q IRQ4 */
#define INT_IRQ5        69      /* �O���[�q IRQ5 */
#define INT_IRQ6        70      /* �O���[�q IRQ6 */
#define INT_IRQ7        71      /* �O���[�q IRQ7 */
#define INT_DEI0        72      /* DMAC �`���l��0 */
#define INT_DEI1        74      /* DMAC �`���l��1 */
#define INT_DEI2        76      /* DMAC �`���l��2 */
#define INT_DEI3        78      /* DMAC �`���l��3 */
#define INT_ITV1        80      /* ATU-II �`���l��0 INTV1 */
#define INT_ITV2A       81      /* ATU-II �`���l��0 INTV2A */
#define INT_ITV2A       81      /* ATU-II �`���l��0 INTV2B */
#define INT_ICI0A       84      /* ATU-II �`���l��0 ICI0A */
#define INT_ICI0B       86      /* ATU-II �`���l��0 ICI0B */
#define INT_ICI0C       88      /* ATU-II �`���l��0 ICI0C */
#define INT_ICI0D       90      /* ATU-II �`���l��0 ICI0D */
#define INT_OVI0        92      /* ATU-II �`���l��0 OVI0 */
#define INT_IMI1A       96      /* ATU-II �`���l��1 IMI1A */
#define INT_CMI1        96      /* ATU-II �`���l��1 CMI1 */
#define INT_IMI1B       97      /* ATU-II �`���l��1 IMI1B */
#define INT_IMI1C       98      /* ATU-II �`���l��1 IMI1C */
#define INT_IMI1D       99      /* ATU-II �`���l��1 IMI1D */
#define INT_IMI1E       100     /* ATU-II �`���l��1 IMI1E */
#define INT_IMI1F       101     /* ATU-II �`���l��1 IMI1F */
#define INT_IMI1G       102     /* ATU-II �`���l��1 IMI1G */
#define INT_IMI1H       103     /* ATU-II �`���l��1 IMI1H */
#define INT_OVI1A       104     /* ATU-II �`���l��1 OVI1A */
#define INT_OVI1B       104     /* ATU-II �`���l��1 OVI1B */
#define INT_IMI2A       108     /* ATU-II �`���l��2 IMI1A */
#define INT_CMI2A       108     /* ATU-II �`���l��2 CMI1A */
#define INT_IMI2B       109     /* ATU-II �`���l��2 IMI1B */
#define INT_CMI2B       109     /* ATU-II �`���l��2 CMI1B */
#define INT_IMI2C       110     /* ATU-II �`���l��2 IMI1C */
#define INT_CMI2C       110     /* ATU-II �`���l��2 CMI1C */
#define INT_IMI2D       111     /* ATU-II �`���l��2 IMI1D */
#define INT_CMI2D       111     /* ATU-II �`���l��2 CMI1D */
#define INT_IMI2E       112     /* ATU-II �`���l��2 IMI1E */
#define INT_CMI2E       112     /* ATU-II �`���l��2 CMI1E */
#define INT_IMI2F       113     /* ATU-II �`���l��2 IMI1F */
#define INT_CMI2F       113     /* ATU-II �`���l��2 CMI1F */
#define INT_IMI2G       114     /* ATU-II �`���l��2 IMI1G */
#define INT_CMI2G       114     /* ATU-II �`���l��2 CMI1G */
#define INT_IMI2H       115     /* ATU-II �`���l��2 IMI1H */
#define INT_CMI2H       115     /* ATU-II �`���l��2 CMI1H */
#define INT_OVI2A       116     /* ATU-II �`���l��2 OVI1A */
#define INT_OVI2B       116     /* ATU-II �`���l��2 OVI1B */
#define INT_IMI3A       120     /* ATU-II �`���l��3 IMI3A */
#define INT_IMI3B       121     /* ATU-II �`���l��3 IMI3B */
#define INT_IMI3C       122     /* ATU-II �`���l��3 IMI3C */
#define INT_IMI3D       123     /* ATU-II �`���l��3 IMI3D */
#define INT_OVI3        124     /* ATU-II �`���l��3 OVI3 */
#define INT_IMI4A       128     /* ATU-II �`���l��4 IMI4A */
#define INT_IMI4B       129     /* ATU-II �`���l��4 IMI4B */
#define INT_IMI4C       130     /* ATU-II �`���l��4 IMI4C */
#define INT_IMI4D       131     /* ATU-II �`���l��4 IMI4D */
#define INT_OVI4        132     /* ATU-II �`���l��4 OVI4 */
#define INT_IMI5A       136     /* ATU-II �`���l��5 IMI5A */
#define INT_IMI5B       137     /* ATU-II �`���l��5 IMI5B */
#define INT_IMI5C       138     /* ATU-II �`���l��5 IMI5C */
#define INT_IMI5D       139     /* ATU-II �`���l��5 IMI5D */
#define INT_OVI5        140     /* ATU-II �`���l��5 OVI5 */
#define INT_CMI6A       144     /* ATU-II �`���l��6 CMI6A */
#define INT_CMI6B       145     /* ATU-II �`���l��6 CMI6B */
#define INT_CMI6C       146     /* ATU-II �`���l��6 CMI6C */
#define INT_CMI6D       147     /* ATU-II �`���l��6 CMI6D */
#define INT_CMI7A       148     /* ATU-II �`���l��7 CMI7A */
#define INT_CMI7B       149     /* ATU-II �`���l��7 CMI7B */
#define INT_CMI7C       150     /* ATU-II �`���l��7 CMI7C */
#define INT_CMI7D       151     /* ATU-II �`���l��7 CMI7D */
#define INT_OSI8A       152     /* ATU-II �`���l��8 OSI8A */
#define INT_OSI8B       153     /* ATU-II �`���l��8 OSI8B */
#define INT_OSI8C       154     /* ATU-II �`���l��8 OSI8C */
#define INT_OSI8D       155     /* ATU-II �`���l��8 OSI8D */
#define INT_OSI8E       156     /* ATU-II �`���l��8 OSI8E */
#define INT_OSI8F       157     /* ATU-II �`���l��8 OSI8F */
#define INT_OSI8G       158     /* ATU-II �`���l��8 OSI8G */
#define INT_OSI8H       159     /* ATU-II �`���l��8 OSI8H */
#define INT_OSI8I       160     /* ATU-II �`���l��8 OSI8I */
#define INT_OSI8J       161     /* ATU-II �`���l��8 OSI8J */
#define INT_OSI8K       162     /* ATU-II �`���l��8 OSI8K */
#define INT_OSI8L       163     /* ATU-II �`���l��8 OSI8L */
#define INT_OSI8M       164     /* ATU-II �`���l��8 OSI8M */
#define INT_OSI8N       165     /* ATU-II �`���l��8 OSI8N */
#define INT_OSI8O       166     /* ATU-II �`���l��8 OSI8O */
#define INT_OSI8P       167     /* ATU-II �`���l��8 OSI8P */
#define INT_CMI9A       168     /* ATU-II �`���l��9 CMI9A */
#define INT_CMI9B       169     /* ATU-II �`���l��9 CMI9B */
#define INT_CMI9C       170     /* ATU-II �`���l��9 CMI9C */
#define INT_CMI9D       171     /* ATU-II �`���l��9 CMI9D */
#define INT_CMI9E       172     /* ATU-II �`���l��9 CMI9E */
#define INT_CMI9F       174     /* ATU-II �`���l��9 CMI9F */
#define INT_CMI10A      176     /* ATU-II �`���l��10 CMI10A */
#define INT_CMI10B      178     /* ATU-II �`���l��10 CMI10B */
#define INT_ICI10A      180     /* ATU-II �`���l��10 ICI10A */
#define INT_CMI10G      180     /* ATU-II �`���l��10 CMI10G */
#define INT_IMI11A      184     /* ATU-II �`���l��11 IMI11A */
#define INT_IMI11B      186     /* ATU-II �`���l��11 IMI11B */
#define INT_OVI11       187     /* ATU-II �`���l��11 OVI11 */
#define INT_CMTI0       188     /* CMT �`���l��0 CMTI0 */
#define INT_ADI0        190     /* A/D �`���l��0 ADI0 */
#define INT_CMTI1       192     /* CMT �`���l��1 CMTI1 */
#define INT_ADI1        194     /* A/D �`���l��1 ADI1 */
#define INT_ADI2        196     /* A/D �`���l��2 ADI2 */
#define INT_ERI0        200     /* SCI �`���l��0 ERI0 */
#define INT_RXI0        201     /* SCI �`���l��0 RXI0 */
#define INT_TXI0        202     /* SCI �`���l��0 TXI0 */
#define INT_TEI0        203     /* SCI �`���l��0 TEI0 */
#define INT_ERI1        204     /* SCI �`���l��1 ERI1 */
#define INT_RXI1        205     /* SCI �`���l��1 RXI1 */
#define INT_TXI1        206     /* SCI �`���l��1 TXI1 */
#define INT_TEI1        207     /* SCI �`���l��1 TEI1 */
#define INT_ERI2        208     /* SCI �`���l��2 ERI2 */
#define INT_RXI2        209     /* SCI �`���l��2 RXI2 */
#define INT_TXI2        210     /* SCI �`���l��2 TXI2 */
#define INT_TEI2        211     /* SCI �`���l��2 TEI2 */
#define INT_ERI3        212     /* SCI �`���l��3 ERI3 */
#define INT_RXI3        213     /* SCI �`���l��3 RXI3 */
#define INT_TXI3        214     /* SCI �`���l��3 TXI3 */
#define INT_TEI3        215     /* SCI �`���l��3 TEI3 */
#define INT_ERI4        216     /* SCI �`���l��4 ERI4 */
#define INT_RXI4        217     /* SCI �`���l��4 RXI4 */
#define INT_TXI4        218     /* SCI �`���l��4 TXI4 */
#define INT_TEI4        219     /* SCI �`���l��4 TEI4 */
#define INT_ERS0        220     /* HCAN �`���l��0 ERS0 */
#define INT_OVR0        221     /* HCAN �`���l��0 OVR0 */
#define INT_RM0         222     /* HCAN �`���l��0 RM0 */
#define INT_SLE0        223     /* HCAN �`���l��0 SLE0 */
#define INT_ITI         224     /* WDT ITI */
#define INT_ERS1        228     /* HCAN �`���l��1 ERS1 */
#define INT_OVR1        229     /* HCAN �`���l��1 OVR1 */
#define INT_RM1         230     /* HCAN �`���l��1 RM1 */
#define INT_SLE1        231     /* HCAN �`���l��1 SLE1 */

#endif /* SH7055_H */
