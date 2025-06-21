/******************************************************************************
* SH7709A Control Registers Definitions                                       *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 11/Apr/2000                                                                 *
* 15/Aug/2000  corrected I/O address (0x04??????->0xa4??????)                 *
******************************************************************************/
/******************************************************************************
 *$Header: p:/presto2/shinc/sh7706/RCS/sh7709a.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sh7709a.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
******************************************************************************/

#ifndef SH7709A_H
#define SH7709A_H

/* Control Register Input/Output */

unsigned char sfr_in(long n);               /* input a byte data */
void sfr_out(long n, unsigned char c);      /* outut a byte data */
unsigned short sfr_inw(long n);             /* input a word data */
void sfr_outw(long n, unsigned short c);    /* output a word data */
unsigned long sfr_inl(long n);              /* input a long word data */
void sfr_outl(long n, unsigned long c);     /* output a long word data */
void sfr_set(long n, unsigned char c);      /* set bit of byte data */
void sfr_clr(long n, unsigned char c);      /* clear bit of byte data */
void sfr_setw(long n, unsigned short c);    /* set bit of word data */
void sfr_clrw(long n, unsigned short c);    /* clear bit of word data */

#if (!defined(M_I86)&&!defined(__TURBOC__))
#define sfr_in(n)       (*(volatile unsigned char *)(n))
#define sfr_out(n,c)    (*(volatile unsigned char *)(n)=(c))
#define sfr_inw(n)      (*(volatile unsigned short *)(n))
#define sfr_outw(n,c)   (*(volatile unsigned short *)(n)=(c))
#define sfr_inl(n)      (*(volatile unsigned long *)(n))
#define sfr_outl(n,c)   (*(volatile unsigned long *)(n)=(c))
#define sfr_set(n,c)    (*(volatile unsigned char *)(n)|=(c))
#define sfr_clr(n,c)    (*(volatile unsigned char *)(n)&=~(c))
#define sfr_setw(n,c)   (*(volatile unsigned short *)(n)|=(c))
#define sfr_clrw(n,c)   (*(volatile unsigned short *)(n)&=~(c))
#endif

/* Address of Control Registers */

#define CCN_PTEH     0xfffffff0L
#define CCN_PTEL     0xfffffff4L
#define CCN_TTB      0xfffffff8L
#define CCN_TEA      0xfffffffcL
#define CCN_MMUCR    0xffffffe0L
#define CCN_BASRA    0xffffffe4L
#define CCN_BASRB    0xffffffe8L
#define CCN_CCR      0xffffffecL
#define CCN_TRA      0xffffffd0L
#define CCN_EXPEVT   0xffffffd4L
#define CCN_INTEVT   0xffffffd8L

#define UBC_BARA     0xffffffb0L
#define UBC_BAMRA    0xffffffb4L
#define UBC_BBRA     0xffffffb8L
#define UBC_BARB     0xffffffa0L
#define UBC_BAMRB    0xffffffa4L
#define UBC_BBRB     0xffffffa8L
#define UBC_BDRB     0xffffff90L
#define UBC_BDMRB    0xffffff94L
#define UBC_BRCR     0xffffff98L

#define CPG_FRQCR    0xffffff80L
#define CPG_STBCR    0xffffff82L
#define CPG_STBCR2   0xffffff88L
#define CPG_WTCNT    0xffffff84L
#define CPG_WTCSR    0xffffff86L

#define BCN_BCR1     0xffffff60L
#define BCN_BCR2     0xffffff62L
#define BCN_WCR1     0xffffff64L
#define BCN_WCR2     0xffffff66L
#define BCN_MCR      0xffffff68L
#define BCN_DCR      0xffffff6aL
#define BCN_PCR      0xffffff6cL
#define BCN_RTCSR    0xffffff6eL
#define BCN_RTCNT    0xffffff70L
#define BCN_RTCOR    0xffffff72L
#define BCN_RFCR     0xffffff74L
#define BCN_BCR3     0xffffff7eL
#define BCN_SDMR     0xffffd000L

#define RTC_R64CNT   0xfffffec0L
#define RTC_RSECCNT  0xfffffec2L
#define RTC_RMINCNT  0xfffffec4L
#define RTC_RHRCNT   0xfffffec6L
#define RTC_RWKCNT   0xfffffec8L
#define RTC_RDAYCNT  0xfffffecaL
#define RTC_RMONCNT  0xfffffeccL
#define RTC_RYRCNT   0xfffffeceL
#define RTC_RSECAR   0xfffffed0L
#define RTC_RMINAR   0xfffffed2L
#define RTC_RHRAR    0xfffffed4L
#define RTC_RWKAR    0xfffffed6L
#define RTC_RDAYAR   0xfffffed8L
#define RTC_RMONAR   0xfffffedaL
#define RTC_RCR1     0xfffffedcL
#define RTC_RCR2     0xfffffedeL

#define INTC_ICR0    0xfffffee0L
#define INTC_IPRA    0xfffffee2L
#define INTC_IPRB    0xfffffee4L

#define TMU_TOCR     0xfffffe90L
#define TMU_TSTR     0xfffffe92L
#define TMU_TCOR0    0xfffffe94L
#define TMU_TCNT0    0xfffffe98L
#define TMU_TCR0     0xfffffe9cL
#define TMU_TCOR1    0xfffffea0L
#define TMU_TCNT1    0xfffffea4L
#define TMU_TCR1     0xfffffea8L
#define TMU_TCOR2    0xfffffeacL
#define TMU_TCNT2    0xfffffeb0L
#define TMU_TCR2     0xfffffeb4L
#define TMU_TCPR2    0xfffffeb8L

#define SCI_SCSMR    0xfffffe80L
#define SCI_SCBRR    0xfffffe82L
#define SCI_SCSCR    0xfffffe84L
#define SCI_SCTDR    0xfffffe86L
#define SCI_SCSSR    0xfffffe88L
#define SCI_SCRDR    0xfffffe8aL

#define INTC_INTEVT2 0xa4000000L
#define INTC_IRR0    0xa4000004L
#define INTC_IRR1    0xa4000006L
#define INTC_IRR2    0xa4000008L
#define INTC_ICR1    0xa4000010L
#define INTC_ICR2    0xa4000012L
#define INTC_INTER   0xa4000014L
#define INTC_IPRC    0xa4000016L
#define INTC_IPRD    0xa4000018L
#define INTC_IPRE    0xa400001aL

#define DMAC_SAR0    0xa4000020L
#define DMAC_DAR0    0xa4000024L
#define DMAC_DMATCR0 0xa4000028L
#define DMAC_CHCR0   0xa400002cL
#define DMAC_SAR1    0xa4000030L
#define DMAC_DAR1    0xa4000034L
#define DMAC_DMATCR1 0xa4000038L
#define DMAC_CHCR1   0xa400003cL
#define DMAC_SAR2    0xa4000040L
#define DMAC_DAR2    0xa4000044L
#define DMAC_DMATCR2 0xa4000048L
#define DMAC_CHCR2   0xa400004cL
#define DMAC_SAR3    0xa4000050L
#define DMAC_DAR3    0xa4000054L
#define DMAC_DMATCR3 0xa4000058L
#define DMAC_CHCR3   0xa400005cL
#define DMAC_DMAOR   0xa4000060L

#define CMT_CMSTR    0xa4000070L
#define CMT_CMCSR    0xa4000072L
#define CMT_CMCNT    0xa4000074L
#define CMT_CMCOR    0xa4000076L

#define AD_ADDRAH    0xa4000080L
#define AD_ADDRAL    0xa4000082L
#define AD_ADDRBH    0xa4000084L
#define AD_ADDRBL    0xa4000086L
#define AD_ADDRCH    0xa4000088L
#define AD_ADDRCL    0xa400008aL
#define AD_ADDRDH    0xa400008cL
#define AD_ADDRDL    0xa400008eL
#define AD_ADCSR     0xa4000090L
#define AD_ADCR      0xa4000092L

#define DA_DADR0     0xa40000a0L
#define DA_DADR1     0xa40000a2L
#define DA_DACR      0xa40000a4L

#define PORT_PACR    0xa4000100L
#define PORT_PBCR    0xa4000102L
#define PORT_PCCR    0xa4000104L
#define PORT_PDCR    0xa4000106L
#define PORT_PECR    0xa4000108L
#define PORT_PFCR    0xa400010aL
#define PORT_PGCR    0xa400010cL
#define PORT_PHCR    0xa400010eL
#define PORT_PJCR    0xa4000110L
#define PORT_PKCR    0xa4000112L
#define PORT_PLCR    0xa4000114L
#define PORT_SCPCR   0xa4000116L
#define PORT_PADR    0xa4000120L
#define PORT_PBDR    0xa4000122L
#define PORT_PCDR    0xa4000124L
#define PORT_PDDR    0xa4000126L
#define PORT_PEDR    0xa4000128L
#define PORT_PFDR    0xa400012aL
#define PORT_PGDR    0xa400012cL
#define PORT_PHDR    0xa400012eL
#define PORT_PJDR    0xa4000130L
#define PORT_PKDR    0xa4000132L
#define PORT_PLDR    0xa4000134L
#define PORT_SCPDR   0xa4000136L

#define IrDA_SCSMR1  0xa4000140L
#define IrDA_SCBRR1  0xa4000142L
#define IrDA_SCSCR1  0xa4000144L
#define IrDA_SCFTDR1 0xa4000146L
#define IrDA_SCSSR1  0xa4000148L
#define IrDA_SCFRDR1 0xa400014aL
#define IrDA_SCFCR1  0xa400014cL
#define IrDA_SCFDR1  0xa400014eL

#define SCIF_SCSMR2  0xa4000150L
#define SCIF_SCBRR2  0xa4000152L
#define SCIF_SCSCR2  0xa4000154L
#define SCIF_SCFTDR2 0xa4000156L
#define SCIF_SCSSR2  0xa4000158L
#define SCIF_SCFRDR2 0xa400015aL
#define SCIF_SCFCR2  0xa400015cL
#define SCIF_SCFDR2  0xa400015eL

/* Interrupt Number (INTEVT2 code / 0x20) */

#define INT_NMI     14
#define INT_IRL15   16              /* IRL mode only */
#define INT_IRL14   17              /* IRL mode only */
#define INT_IRL13   18              /* IRL mode only */
#define INT_IRL12   19              /* IRL mode only */
#define INT_IRL11   20              /* IRL mode only */
#define INT_IRL10   21              /* IRL mode only */
#define INT_IRL9    22              /* IRL mode only */
#define INT_IRL8    23              /* IRL mode only */
#define INT_IRL7    24              /* IRL mode only */
#define INT_IRL6    25              /* IRL mode only */
#define INT_IRL5    26              /* IRL mode only */
#define INT_IRL4    27              /* IRL mode only */
#define INT_IRL3    28              /* IRL mode only */
#define INT_IRL2    29              /* IRL mode only */
#define INT_IRL1    30              /* IRL mode only */
#define INT_TUNI0   32
#define INT_TUNI1   33
#define INT_TUNI2   34
#define INT_TICPI2  35
#define INT_ATI     36
#define INT_PRI     37
#define INT_CUI     38
#define INT_ERI     39
#define INT_RXI     40
#define INT_TXI     41
#define INT_TEI     42
#define INT_ITI     43
#define INT_RCMI    44
#define INT_ROVI    45
#define INT_IRQ0    48              /* IRQ mode only */
#define INT_IRQ1    49              /* IRQ mode only */
#define INT_IRQ2    50              /* IRQ mode only */
#define INT_IRQ3    51              /* IRQ mode only */
#define INT_IRQ4    52
#define INT_IRQ5    53
#define INT_PINT0   56
#define INT_PINT8   57
#define INT_DEI0    64
#define INT_DEI1    65
#define INT_DEI2    66
#define INT_DEI3    67
#define INT_ERI1    68
#define INT_RXI1    69
#define INT_BRI1    70
#define INT_TXI1    71
#define INT_ERI2    72
#define INT_RXI2    73
#define INT_BRI2    74
#define INT_TXI2    75
#define INT_ADI     76

#endif /* SH7709A_H */
