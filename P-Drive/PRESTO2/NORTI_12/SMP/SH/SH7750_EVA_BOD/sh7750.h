/******************************************************************************
* SH7750 ���䃌�W�X�^��`                                                     *
*                                                                             *
*  File name : sh7750.h                                                       *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 01/Feb/1999 created                                                         *
* 06/Dec/1999 �����ݔԍ��̕s������ǉ�                                        *
* 16/Apr/2000                                                                 *
* 16/Jan/2001 SCI_SCSPRT1 -> SCI_SCSPTR1                                      *
******************************************************************************/

#ifndef SH7750_H
#define SH7750_H

/* ���䃌�W�X�^�̃x�[�X�A�h���X */

#if (!defined(M_I86)&&!defined(__TURBOC__))
#define IO_BASE ((volatile unsigned char *)0xff000000)
#endif

/* ���䃌�W�X�^���o�̓}�N�� */

unsigned char sfr_in(int n);                /* �o�C�g�f�[�^�̓��� */
void sfr_out(int n, unsigned char c);       /* �o�C�g�f�[�^�̏o�� */
unsigned short sfr_inw(int n);              /* ���[�h�f�[�^�̓��� */
void sfr_outw(int n, unsigned short c);     /* ���[�h�f�[�^�̏o�� */
unsigned long sfr_inl(int n);               /* �����O���[�h�f�[�^�̓��� */
void sfr_outl(int n, unsigned long c);      /* �����O���[�h�f�[�^�̏o�� */
void sfr_set(int n, unsigned char c);       /* �o�C�g�f�[�^�̃r�b�g�Z�b�g */
void sfr_clr(int n, unsigned char c);       /* �o�C�g�f�[�^�̃r�b�g�N���A */
void sfr_setw(int n, unsigned short c);     /* ���[�h�f�[�^�̃r�b�g�Z�b�g */
void sfr_clrw(int n, unsigned short c);     /* ���[�h�f�[�^�̃r�b�g�N���A */

#ifdef IO_BASE
#define sfr_in(n)       (*(IO_BASE+(n)))
#define sfr_out(n,c)    (*(IO_BASE+(n))=(c))
#define sfr_inw(n)      (*(volatile unsigned short *)(IO_BASE+(n)))
#define sfr_outw(n,c)   (*(volatile unsigned short *)(IO_BASE+(n))=(c))
#define sfr_inl(n)      (*(volatile unsigned long *)(IO_BASE+(n)))
#define sfr_outl(n,c)   (*(volatile unsigned long *)(IO_BASE+(n))=(c))
#define sfr_set(n,c)    (*(IO_BASE+(n))|=(c))
#define sfr_clr(n,c)    (*(IO_BASE+(n))&=~(c))
#define sfr_setw(n,c)   (*(volatile unsigned short *)(IO_BASE+(n))|=(c))
#define sfr_clrw(n,c)   (*(volatile unsigned short *)(IO_BASE+(n))&=~(c))
#endif

/* ���䃌�W�X�^�̃I�t�Z�b�g�A�h���X */

#define CCN_PTEH        0x000000    /* �y�[�W�e�[�u���G���g����ʃ��W�X�^ */
#define CCN_PTEL        0x000004    /* �y�[�W�e�[�u���G���g�����ʃ��W�X�^ */
#define CCN_TTB         0x000008    /* �ϊ��e�[�u���x�[�X���W�X�^ */
#define CCN_TEA         0x00000c    /* TLB ��O�A�h���X���W�X�^ */
#define CCN_MMUCR       0x000010    /* MMU ���䃌�W�X�^ */
#define CCN_BASRA       0x000014    /* �u���[�N ASID ���W�X�^A */
#define CCN_BASRB       0x000018    /* �u���[�N ASID ���W�X�^B */
#define CCN_CCR         0x00001c    /* �L���b�V�����䃌�W�X�^ */
#define CCN_TRA         0x000020    /* TRAPA ��O���W�X�^ */
#define CCN_EXPEVT      0x000024    /* ��O���ۃ��W�X�^ */
#define CCN_INTEVT      0x000028    /* ���荞�ݎ��ۃ��W�X�^ */
#define CCN_PTEA        0x000034    /* �y�[�W�e�[�u���G���g���A�V�X�^���X���W�X�^ */
#define CCN_QACR0       0x000038    /* �L���[�A�h���X���䃌�W�X�^0 */
#define CCN_QACR1       0x00003c    /* �L���[�A�h���X���䃌�W�X�^1 */

#define UBC_BARA        0x200000    /* �u���[�N�A�h���X���W�X�^A */
#define UBC_BAMRA       0x200004    /* �u���[�N�A�h���X�}�X�N���W�X�^A */
#define UBC_BBRA        0x200008    /* �u���[�N�o�X�T�C�N�����W�X�^A */
#define UBC_BARB        0x20000c    /* �u���[�N�A�h���X���W�X�^B */
#define UBC_BAMRB       0x200010    /* �u���[�N�A�h���X�}�X�N���W�X�^B */
#define UBC_BBRB        0x200014    /* �u���[�N�o�X�T�C�N�����W�X�^B */
#define UBC_BDRB        0x200018    /* �u���[�N�f�[�^���W�X�^B */
#define UBC_BDMRB       0x20001c    /* �u���[�N�f�[�^�}�X�N���W�X�^B */
#define UBC_BRCR        0x200020    /* �u���[�N�R���g���[�����W�X�^ */

#define BSC_BCR1        0x800000    /* �o�X�R���g���[�����W�X�^1 */
#define BSC_BCR2        0x800004    /* �o�X�R���g���[�����W�X�^2 */
#define BSC_WCR1        0x800008    /* �E�F�C�g�X�e�[�g�R���g���[�����W�X�^1 */
#define BSC_WCR2        0x80000c    /* �E�F�C�g�X�e�[�g�R���g���[�����W�X�^2 */
#define BSC_WCR3        0x800010    /* �E�F�C�g�X�e�[�g�R���g���[�����W�X�^3 */
#define BSC_MCR         0x800014    /* �ʃ������R���g���[�����W�X�^ */
#define BSC_PCR         0x800018    /* PCMCIA ���W�X�^ */
#define BSC_RTCSR       0x80001c    /* ���t���b�V���^�C�}�R���g���[��/�X�e�[�^�X���W�X�^ */
#define BSC_RTCNT       0x800020    /* ���t���b�V���^�C�}�J�E���^ */
#define BSC_RTCOR       0x800024    /* ���t���b�V���^�C���R���X�^���g�J�E���^ */
#define BSC_RFCR        0x800028    /* ���t���b�V���J�E���g���W�X�^ */
#define BSC_PCTRA       0x80002c    /* �|�[�g�R���g�[�����W�X�^A */
#define BSC_PDTRA       0x800030    /* �|�[�g�f�[�^���W�X�^A */
#define BSC_PCTRB       0x800040    /* �|�[�g�R���g�[�����W�X�^B */
#define BSC_PDTRB       0x800044    /* �|�[�g�f�[�^���W�X�^B */
#define BSC_GPIOIC      0x800048    /* GPIO �����݃R���g���[�����W�X�^ */
#define BSC_SDMR2       0x900000    /* �V���N���i�XDRAM ���[�h���W�X�^2 */
#define BSC_SDMR3       0x940000    /* �V���N���i�XDRAM ���[�h���W�X�^3 */

#define DMAC_SAR0       0xa00000    /* DMA �\�[�X�A�h���X���W�X�^0 */
#define DMAC_DAR0       0xa00004    /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^0 */
#define DMAC_DMATCR0    0xa00008    /* DMA �g�����X�t�@�J�E���g���W�X�^0 */
#define DMAC_CHCR0      0xa0000c    /* DMA �`�����l���R���g���[�����W�X�^0 */
#define DMAC_SAR1       0xa00010    /* DMA �\�[�X�A�h���X���W�X�^1 */
#define DMAC_DAR1       0xa00014    /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^1 */
#define DMAC_DMATCR1    0xa00018    /* DMA �g�����X�t�@�J�E���g���W�X�^1 */
#define DMAC_CHCR1      0xa0001c    /* DMA �`�����l���R���g���[�����W�X�^1 */
#define DMAC_SAR2       0xa00020    /* DMA �\�[�X�A�h���X���W�X�^2 */
#define DMAC_DAR2       0xa00024    /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^2 */
#define DMAC_DMATCR2    0xa00028    /* DMA �g�����X�t�@�J�E���g���W�X�^2 */
#define DMAC_CHCR2      0xa0002c    /* DMA �`�����l���R���g���[�����W�X�^2 */
#define DMAC_SAR3       0xa00030    /* DMA �\�[�X�A�h���X���W�X�^3 */
#define DMAC_DAR3       0xa00034    /* DMA �f�X�e�B�l�[�V�����A�h���X���W�X�^3 */
#define DMAC_DMATCR3    0xa00038    /* DMA �g�����X�t�@�J�E���g���W�X�^3 */
#define DMAC_CHCR3      0xa0003c    /* DMA �`�����l���R���g���[�����W�X�^3 */
#define DMAC_DMATOR     0xa00040    /* DMA �I�y���[�V�������W�X�^ */

#define CPG_FRQCR       0xc00000    /* ���g�����䃌�W�X�^ */
#define CPG_STBCR       0xc00004    /* �X�^���o�C�R���g���[�����W�X�^ */
#define CPG_WTCNT       0xc00008    /* �E�H�b�`�h�b�O�^�C�}�J�E���^ */
#define CPG_WTCSR       0xc0000c    /* �E�H�b�`�h�b�O�^�C�}�R���g���[��/�X�e�[�^�X���W�X�^ */
#define CPG_STBCR2      0xc00010    /* �X�^���o�C�R���g���[�����W�X�^2 */

#define RTC_R64CNT      0xc80000    /* 64Hz �J�E���^ */
#define RTC_RSECCNT     0xc80004    /* �b�J�E���^ */
#define RTC_RMINCNT     0xc80008    /* ���J�E���^ */
#define RTC_RHRCNT      0xc8000c    /* ���J�E���^ */
#define RTC_RWKCNT      0xc80010    /* �j���J�E���^ */
#define RTC_RDAYCNT     0xc80014    /* ���J�E���^ */
#define RTC_RMONCNT     0xc80018    /* ���J�E���^ */
#define RTC_RYRCNT      0xc8001c    /* �N�J�E���^ */
#define RTC_RSECAR      0xc80020    /* �b�A���[�����W�X�^ */
#define RTC_RMINAR      0xc80024    /* ���A���[�����W�X�^ */
#define RTC_RHRAR       0xc80028    /* ���A���[�����W�X�^ */
#define RTC_RWKAR       0xc8002c    /* �j���A���[�����W�X�^ */
#define RTC_RDAYAR      0xc80030    /* ���A���[�����W�X�^ */
#define RTC_RMONAR      0xc80034    /* ���A���[�����W�X�^ */
#define RTC_RCR1        0xc80038    /* RTC �R���g���[�����W�X�^1 */
#define RTC_RCR2        0xc8003c    /* RTC �R���g���[�����W�X�^2 */

#define INTC_ICR        0xd00000    /* ���荞�݃R���g���[�����W�X�^ */
#define INTC_IPRA       0xd00004    /* ���荞�ݗD�惌�x���ݒ背�W�X�^A */
#define INTC_IPRB       0xd00008    /* ���荞�ݗD�惌�x���ݒ背�W�X�^B */
#define INTC_IPRC       0xd0000c    /* ���荞�ݗD�惌�x���ݒ背�W�X�^C */

#define TMU_TOCR        0xd80000    /* �^�C�}�A�E�g�v�b�g�R���g���[�����W�X�^ */
#define TMU_TSTR        0xd80004    /* �^�C�}�X�^�[�g���W�X�^ */
#define TMU_TCOR0       0xd80008    /* �^�C�}�R���X�^���g���W�X�^0 */
#define TMU_TCNT0       0xd8000c    /* �^�C�}�J�E���^0 */
#define TMU_TCR0        0xd80010    /* �^�C�}�R���g���[�����W�X�^0 */
#define TMU_TCOR1       0xd80014    /* �^�C�}�R���X�^���g���W�X�^1 */
#define TMU_TCNT1       0xd80018    /* �^�C�}�J�E���^1 */
#define TMU_TCR1        0xd8001c    /* �^�C�}�R���g���[�����W�X�^1 */
#define TMU_TCOR2       0xd80020    /* �^�C�}�R���X�^���g���W�X�^2 */
#define TMU_TCNT2       0xd80024    /* �^�C�}�J�E���^2 */
#define TMU_TCR2        0xd80028    /* �^�C�}�R���g���[�����W�X�^2 */
#define TMU_TCPR2       0xd8002c    /* �C���v�b�g�L���v�`�����W�X�^ */

#define SCI_SCSMR1      0xe00000    /* �V���A�����[�h���W�X�^ */
#define SCI_SCBRR1      0xe00004    /* �r�b�g���[�g���W�X�^ */
#define SCI_SCSCR1      0xe00008    /* �V���A���R���g���[�����W�X�^ */
#define SCI_SCTDR1      0xe0000c    /* �g�����X�~�b�g�f�[�^���W�X�^ */
#define SCI_SCSSR1      0xe00010    /* �V���A���X�e�[�^�X���W�X�^ */
#define SCI_SCRDR1      0xe00014    /* ���V�[�u�f�[�^���W�X�^ */
#define SCI_SCSCMR1     0xe00018    /* �X�}�[�g�J�[�h���[�h���W�X�^ */
#define SCI_SCSPTR1     0xe0001c    /* �V���A���|�[�g���W�X�^ */

#define SCIF_SCSMR2     0xe80000    /* �V���A�����[�h���W�X�^ */
#define SCIF_SCBRR2     0xe80004    /* �r�b�g���[�g���W�X�^ */
#define SCIF_SCSCR2     0xe80008    /* �V���A���R���g���[�����W�X�^ */
#define SCIF_SCFTDR2    0xe8000c    /* �g�����X�~�b�gFIFO�f�[�^���W�X�^ */
#define SCIF_SCFSR2     0xe80010    /* �V���A���X�e�[�^�X���W�X�^ */
#define SCIF_SCFRDR2    0xe80014    /* ���V�[�uFIFO�f�[�^���W�X�^ */
#define SCIF_SCFCR2     0xe80018    /* FIFO�R���g���[�����W�X�^ */
#define SCIF_SCFDR2     0xe8001c    /* FIFO�f�[�^�����W�X�^ */
#define SCIF_SCSPTR2    0xe80020    /* �V���A���|�[�g���W�X�^ */
#define SCIF_SCLSR2     0xe80024    /* ���C���X�e�[�^�X���W�X�^ */

    /* ���荞�ݔԍ��i��O�R�[�h��32�j*/

#define INT_NMI         14          /* �m���}�X�J�u�����荞�� */
#define INT_IRL15       16          /* ���x��15�O���n�[�h�E�F�A���荞�� */
#define INT_IRL14       17          /* ���x��14�O���n�[�h�E�F�A���荞�� */
#define INT_IRL13       18          /* ���x��13�O���n�[�h�E�F�A���荞�� */
#define INT_IRL12       19          /* ���x��12�O���n�[�h�E�F�A���荞�� */
#define INT_IRL11       20          /* ���x��11�O���n�[�h�E�F�A���荞�� */
#define INT_IRL10       21          /* ���x��10�O���n�[�h�E�F�A���荞�� */
#define INT_IRL9        22          /* ���x��9 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL8        23          /* ���x��8 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL7        24          /* ���x��7 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL6        25          /* ���x��6 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL5        26          /* ���x��5 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL4        27          /* ���x��4 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL3        28          /* ���x��3 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL2        29          /* ���x��2 �O���n�[�h�E�F�A���荞�� */
#define INT_IRL1        30          /* ���x��1 �O���n�[�h�E�F�A���荞�� */
#define INT_TUNI0       32          /* TMU0 �A���_�[�t���[���荞�� */
#define INT_TUNI1       33          /* TMU1 �A���_�[�t���[���荞�� */
#define INT_TUNI2       34          /* TMU2 �A���_�[�t���[���荞�� */
#define INT_TICPI2      35          /* TMU2 �C���v�b�g�L���v�`�����荞�� */
#define INT_ATI         36          /* RTC �A���[�����荞�� */
#define INT_PRI         37          /* RTC �������荞�� */
#define INT_CUI         38          /* RTC ���グ���荞�� */
#define INT_ERI         39          /* SCI ��M�G���[���荞�� */
#define INT_RXI         40          /* SCI ��M�f�[�^�t�����荞�� */
#define INT_TXI         41          /* SCI ���M�f�[�^�t�����荞�� */
#define INT_TEI         42          /* SCI ���M�f�[�^�G���v�e�B���荞�� */
#define INT_ITI         43          /* WDT �C���^�[�o���^�C�}���荞�� */
#define INT_RCMI        44          /* REF �R���y�A�}�b�`���荞�� */
#define INT_ROVI        45          /* REF ���t���b�V���J�E���g�I�[�o�t���[���荞�� */
#define INT_HUDI        48          /* HUDI ���荞�� */
#define INT_GPIO        49          /* GPIO ���荞�� */
#define INT_DMTE0       50          /* DMA CH0 �]���������荞�� */
#define INT_DMTE1       51          /* DMA CH1 �]���������荞�� */
#define INT_DMTE2       52          /* DMA CH2 �]���������荞�� */
#define INT_DMTE3       53          /* DMA CH3 �]���������荞�� */
#define INT_DMAE        54          /* DMA �A�h���X�G���[���荞�� */
#define INT_FERI        56          /* SCIF ��M�G���[���荞�� */
#define INT_FRXI        57          /* SCIF ��M�f�[�^�t�����荞�� */
#define INT_FBRI        58          /* SCIF �u���[�N���荞�� */
#define INT_FTXI        59          /* SCIF ���M�f�[�^�t�����荞�� */

#endif /* SH7750_H */
