/******************************************************************************
* SH7727 Control Registers Definitions                                        *
*                                                                             *
*  Copyright (c)  2002-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 20/Jun/2001                                                                 *
* 23/Feb/2003 Compiler detection and extension definitions inclusion.      MI *
******************************************************************************/

#ifndef SH7727_H
#define SH7727_H

/* Compiler Detection */

#if defined(GAIO)               /* XASS-V */
#elif defined(__GNUC__)         /* exeGCC */
#elif defined(__ghs)            /* Green Hills */
#elif defined(__TID__)          /* IAR EW/ICC */
#elif (defined(WIN32)||defined(__WIN32__))
#else                           /* Renesas C */
#ifndef __HITACHI__
#define __HITACHI__
#endif
#endif

/* Inclusion of Compiler Dependence Extension Definitions for I/O Access */

#if defined(GAIO)
#include <ht1_inline.h>
#elif defined(__HITACHI__)
#include <machine.h>
#endif

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

#if !(defined(WIN32)||defined(__WIN32__))
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

#define CCN_PTEH        0xfffffff0L
#define CCN_PTEL        0xfffffff4L
#define CCN_TTB         0xfffffff8L
#define CCN_TEA         0xfffffffcL
#define CCN_MMUCR       0xffffffe0L
#define CCN_BASRA       0xffffffe4L
#define CCN_BASRB       0xffffffe8L
#define CCN_CCR         0xffffffecL
#define CCN_CCR2        0xa40000b0L
#define CCN_TRA         0xffffffd0L
#define CCN_EXPEVT      0xffffffd4L
#define CCN_INTEVT      0xffffffd8L

#define UBC_BARA        0xffffffb0L
#define UBC_BAMRA       0xffffffb4L
#define UBC_BBRA        0xffffffb8L
#define UBC_BARB        0xffffffa0L
#define UBC_BAMRB       0xffffffa4L
#define UBC_BBRB        0xffffffa8L
#define UBC_BDRB        0xffffff90L
#define UBC_BDMRB       0xffffff94L
#define UBC_BRCR        0xffffff98L
#define UBC_BETR        0xffffff9cL
#define UBC_BRSR        0xffffffacL
#define UBC_BRDR        0xffffffbcL

#define CPG_FRQCR       0xffffff80L
#define CPG_STBCR       0xffffff82L
#define CPG_STBCR2      0xffffff88L
#define CPG_WTCNT       0xffffff84L
#define CPG_WTCSR       0xffffff86L
#define CPG_CKIO2CR     0xa400023aL

#define BCN_BCR1        0xffffff60L
#define BCN_BCR2        0xffffff62L
#define BCN_WCR1        0xffffff64L
#define BCN_WCR2        0xffffff66L
#define BCN_MCR         0xffffff68L
#define BCN_PCR         0xffffff6cL
#define BCN_RTCSR       0xffffff6eL
#define BCN_RTCNT       0xffffff70L
#define BCN_RTCOR       0xffffff72L
#define BCN_RFCR        0xffffff74L
#define BCN_SDMR        0xffffd000L

#define RTC_R64CNT      0xfffffec0L
#define RTC_RSECCNT     0xfffffec2L
#define RTC_RMINCNT     0xfffffec4L
#define RTC_RHRCNT      0xfffffec6L
#define RTC_RWKCNT      0xfffffec8L
#define RTC_RDAYCNT     0xfffffecaL
#define RTC_RMONCNT     0xfffffeccL
#define RTC_RYRCNT      0xfffffeceL
#define RTC_RSECAR      0xfffffed0L
#define RTC_RMINAR      0xfffffed2L
#define RTC_RHRAR       0xfffffed4L
#define RTC_RWKAR       0xfffffed6L
#define RTC_RDAYAR      0xfffffed8L
#define RTC_RMONAR      0xfffffedaL
#define RTC_RCR1        0xfffffedcL
#define RTC_RCR2        0xfffffedeL

#define INTC_ICR0       0xfffffee0L
#define INTC_IPRA       0xfffffee2L
#define INTC_IPRB       0xfffffee4L

#define TMU_TSTR        0xfffffe92L
#define TMU_TCOR0       0xfffffe94L
#define TMU_TCNT0       0xfffffe98L
#define TMU_TCR0        0xfffffe9cL
#define TMU_TCOR1       0xfffffea0L
#define TMU_TCNT1       0xfffffea4L
#define TMU_TCR1        0xfffffea8L
#define TMU_TCOR2       0xfffffeacL
#define TMU_TCNT2       0xfffffeb0L
#define TMU_TCR2        0xfffffeb4L

#define SCI_SCSMR       0xfffffe80L
#define SCI_SCBRR       0xfffffe82L
#define SCI_SCSCR       0xfffffe84L
#define SCI_SCTDR       0xfffffe86L
#define SCI_SCSSR       0xfffffe88L
#define SCI_SCRDR       0xfffffe8aL
#define SCI_SCSCMR      0xfffffe8cL

#define INTC_INTEVT2    0xa4000000L
#define INTC_IRR0       0xa4000004L
#define INTC_IRR1       0xa4000006L
#define INTC_IRR2       0xa4000008L
#define INTC_ICR1       0xa4000010L
#define INTC_ICR2       0xa4000012L
#define INTC_INTER      0xa4000014L
#define INTC_IPRC       0xa4000016L
#define INTC_IPRD       0xa4000018L
#define INTC_IPRE       0xa400001aL

#define DMAC_SAR0       0xa4000020L
#define DMAC_DAR0       0xa4000024L
#define DMAC_DMATCR0    0xa4000028L
#define DMAC_CHCR0      0xa400002cL
#define DMAC_SAR1       0xa4000030L
#define DMAC_DAR1       0xa4000034L
#define DMAC_DMATCR1    0xa4000038L
#define DMAC_CHCR1      0xa400003cL
#define DMAC_SAR2       0xa4000040L
#define DMAC_DAR2       0xa4000044L
#define DMAC_DMATCR2    0xa4000048L
#define DMAC_CHCR2      0xa400004cL
#define DMAC_SAR3       0xa4000050L
#define DMAC_DAR3       0xa4000054L
#define DMAC_DMATCR3    0xa4000058L
#define DMAC_CHCR3      0xa400005cL
#define DMAC_DMAOR      0xa4000060L

#define CMT_CMSTR       0xa4000070L
#define CMT_CMCSR       0xa4000072L
#define CMT_CMCNT       0xa4000074L
#define CMT_CMCOR       0xa4000076L

#define AD_ADDRAH       0xa4000080L
#define AD_ADDRAL       0xa4000082L
#define AD_ADDRBH       0xa4000084L
#define AD_ADDRBL       0xa4000086L
#define AD_ADDRCH       0xa4000088L
#define AD_ADDRCL       0xa400008aL
#define AD_ADDRDH       0xa400008cL
#define AD_ADDRDL       0xa400008eL
#define AD_ADCSR        0xa4000090L
#define AD_ADCR         0xa4000092L

#define DA_DADR0        0xa40000a0L
#define DA_DADR1        0xa40000a2L
#define DA_DACR         0xa40000a4L

#define SIOF_SIMDR      0xa40000c0L
#define SIOF_SISCR      0xa40000c2L
#define SIOF_SITDAR     0xa40000c4L
#define SIOF_SIRDAR     0xa40000c6L
#define SIOF_SICDAR     0xa40000c8L
#define SIOF_SICTR      0xa40000ccL
#define SIOF_SIFCTR     0xa40000d0L
#define SIOF_SISTR      0xa40000d4L
#define SIOF_SIIER      0xa40000d6L
#define SIOF_SITDR      0xa40000e0L
#define SIOF_SIRDR      0xa40000e4L
#define SIOF_SITCR      0xa40000e8L
#define SIOF_SIRCR      0xa40000ecL
#define SIOF_SITMR      0xa40000fcL
#define SIOF_SIFPR      0xa40000feL

#define PORT_PACR       0xa4000100L
#define PORT_PBCR       0xa4000102L
#define PORT_PCCR       0xa4000104L
#define PORT_PDCR       0xa4000106L
#define PORT_PECR       0xa4000108L
#define PORT_PFCR       0xa400010aL
#define PORT_PGCR       0xa400010cL
#define PORT_PHCR       0xa400010eL
#define PORT_PJCR       0xa4000110L
#define PORT_PKCR       0xa4000112L
#define PORT_PLCR       0xa4000114L
#define PORT_SCPCR      0xa4000116L
#define PORT_PMCR       0xa4000118L
#define PORT_PADR       0xa4000120L
#define PORT_PBDR       0xa4000122L
#define PORT_PCDR       0xa4000124L
#define PORT_PDDR       0xa4000126L
#define PORT_PEDR       0xa4000128L
#define PORT_PFDR       0xa400012aL
#define PORT_PGDR       0xa400012cL
#define PORT_PHDR       0xa400012eL
#define PORT_PJDR       0xa4000130L
#define PORT_PKDR       0xa4000132L
#define PORT_PLDR       0xa4000134L
#define PORT_SCPDR      0xa4000136L
#define PORT_PMDR       0xa4000138L

#define SCIF_SCSMR2     0xa4000150L
#define SCIF_SCBRR2     0xa4000152L
#define SCIF_SCSCR2     0xa4000154L
#define SCIF_SCFTDR2    0xa4000156L
#define SCIF_SCSSR2     0xa4000158L
#define SCIF_SCFRDR2    0xa400015aL
#define SCIF_SCFCR2     0xa400015cL
#define SCIF_SCFDR2     0xa400015eL

#define PCC_PCC0ISR     0xa4000160L
#define PCC_PCC0GCR     0xa4000162L
#define PCC_PCC0CSCR    0xa4000164L
#define PCC_PCC0CSCIER  0xa4000166L

#define AFE_ACTR1       0xa4000180L
#define AFE_ACTR2       0xa4000182L
#define AFE_ASTR1       0xa4000184L
#define AFE_ASTR2       0xa4000186L
#define AFE_MRCR        0xa4000188L
#define AFE_MPCR        0xa400018aL
#define AFE_DPNQ        0xa400018cL
#define AFE_RCNT        0xa400018eL
#define AFE_ACDR        0xa4000190L
#define AFE_ASDR        0xa4000192L
#define AFE_TDFP        0xa4000194L
#define AFE_RDFP        0xa4000198L

#define HUDI_SDIR       0xa4000200L
#define HUDI_SDDR       0xa4000208L
#define HUDI_SDDRH      0xa4000208L
#define HUDI_SDDRL      0xa400020aL

#define PPCNT_IPRF      0xa4000220L
#define PPCNT_IPRG      0xa4000222L
#define PPCNT_IRR3      0xa4000224L
#define PPCNT_IRR4      0xa4000226L
#define PPCNT_ICR3      0xa4000228L
#define PPCNT_CHRAR     0xa400022aL
#define PPCNT_STBCR3    0xa4000230L
#define PPCNT_SRSTR     0xa4000232L
#define PPCNT_EXPFC     0xa4000234L
#define PPCNT_EXCPGCR   0xa4000236L

#define USBF_USBIFR0    0xa4000240L
#define USBF_USBIFR1    0xa4000241L
#define USBF_USBEPDR0I  0xa4000242L
#define USBF_USBEPDR0O  0xa4000243L
#define USBF_USBTRG     0xa4000244L
#define USBF_USBFCLR    0xa4000245L
#define USBF_USBEPSZ0O  0xa4000246L
#define USBF_USBEPDR0S  0xa4000247L
#define USBF_USBDASTS   0xa4000248L
#define USBF_USBEPDR2   0xa4000249L
#define USBF_USBISR0    0xa400024aL
#define USBF_USBEPSTL   0xa400024bL
#define USBF_USBIER0    0xa400024cL
#define USBF_USBIER1    0xa400024dL
#define USBF_USBEPDR1   0xa400024eL
#define USBF_USBEPSZ1   0xa400024fL
#define USBF_USBISR1    0xa4000250L
#define USBF_USBDMA     0xa4000251L
#define USBF_USBEPDR3   0xa4000252L

#define USBH_USBHR      0xa4000400L
#define USBH_USBHC      0xa4000404L
#define USBH_USBHCS     0xa4000408L
#define USBH_USBHIS     0xa400040cL
#define USBH_USBHIE     0xa4000410L
#define USBH_USBHID     0xa4000414L
#define USBH_USBHHCCA   0xa4000418L
#define USBH_USBHPCED   0xa400041cL
#define USBH_USBHCHED   0xa4000420L
#define USBH_USBHCCED   0xa4000424L
#define USBH_USBHBHED   0xa4000428L
#define USBH_USBHBCED   0xa400042cL
#define USBH_USBHDHED   0xa4000430L
#define USBH_USBHFI     0xa4000434L
#define USBH_USBHFR     0xa4000438L
#define USBH_USBHFN     0xa400043cL
#define USBH_USBHPS     0xa4000440L
#define USBH_USBHLST    0xa4000444L
#define USBH_USBHRDA    0xa4000448L
#define USBH_USBHRDB    0xa400044cL
#define USBH_USBHRS     0xa4000450L
#define USBH_USBHRPS1   0xa4000454L
#define USBH_USBHRPS2   0xa4000458L

#define LCDC_LDPR00     0xa4000800L
#define LCDC_LDICKR     0xa4000c00L
#define LCDC_LDMTR      0xa4000c02L
#define LCDC_LDDFR      0xa4000c04L
#define LCDC_LDSMR      0xa4000c06L
#define LCDC_LDSARU     0xa4000c08L
#define LCDC_LDSARL     0xa4000c0cL
#define LCDC_LDLAOR     0xa4000c10L
#define LCDC_LDPALCR    0xa4000c12L
#define LCDC_LDHCNR     0xa4000c14L
#define LCDC_LDHSYNR    0xa4000c16L
#define LCDC_LDVDLNR    0xa4000c18L
#define LCDC_LDVTLNR    0xa4000c1aL
#define LCDC_LDVSYNR    0xa4000c1cL
#define LCDC_LDACLNR    0xa4000c1eL
#define LCDC_LDINTR     0xa4000c20L
#define LCDC_LDPMMR     0xa4000c24L
#define LCDC_LDPSPR     0xa4000c26L
#define LCDC_LDCNTR     0xa4000c28L

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
#define INT_HUDI    47
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
#define INT_ERI2    72
#define INT_RXI2    73
#define INT_BRI2    74
#define INT_TXI2    75
#define INT_ADI     76
#define INT_LCDCI   77
#define INT_PC0     78
#define INT_USBHI   80
#define INT_USBFI0  81
#define INT_USBFI1  82
#define INT_AFEIFI  83
#define INT_SIFERI  88
#define INT_SIFTXI  89
#define INT_SIFRXI  90
#define INT_SIFCCI  91

#endif /* SH7727_H */
