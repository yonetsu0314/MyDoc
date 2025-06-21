/****************************************************************************
* SH7751 controller specific hardware details                               *
*                                                                           *
*  File name : sh7751.h                                                     *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                *
*  All rights reserved.                                                     *
*                                                                           *
* 01/Feb/1999 created                                                       *
* 06/Dec/1999 äÑçûÇ›î‘çÜÇÃïsë´ï™Çí«â¡                                      *
* 16/Apr/2000                                                               *
* 16/Jan/2001 SCI_SCSPRT1 -> SCI_SCSPTR1                                    *
* 10/Aug/2001 modified for SH7751SE01 eva board       :SP                   *
* 11/Sept/2001 modified byte order defination.        :SP                   *
* 20/Oct/2002 External peripheral definations removed.        [SP]          *
* 30/Apr/2003 Add INT_GPIO definition                                    KA *
****************************************************************************/

#ifndef SH7751_H
#define SH7751_H

/* Control register Base Address */

#if (!defined(M_I86)&&!defined(__TURBOC__))
#define IO_BASE ((volatile unsigned char *)0xff000000)
#define IO_BASE2 ((volatile unsigned char *)0xfe000000)
#endif

#if (!defined (BIG_ENDIAN) && !defined(LITTLE_ENDIAN))
#define BIG_ENDIAN
#endif

/* Control Register data Input/Output functions */
unsigned char sfr_in(int n);              /* Byte data input */
void sfr_out(int n, unsigned char c);     /* Byte data output */
unsigned short sfr_inw(int n);            /* Word data input */
void sfr_outw(int n, unsigned short c);   /* Word data output */
unsigned long sfr_inl(int n);             /* Long Word data input */
void sfr_outl(int n, unsigned long c);    /* Long Word data output */
void sfr_set(int n, unsigned char c);     /* Bit-set control for Byte data */
void sfr_clr(int n, unsigned char c);     /* Bit-reset control for Byte data */
void sfr_setw(int n, unsigned short c);   /* Bit-set control for Word data */
void sfr_clrw(int n, unsigned short c);   /* Bit-reset control for Word data */

/* Extended Register data Input/Output functions */
unsigned char efr_in(int n);              /* Byte data input */
void efr_out(int n, unsigned char c);     /* Byte data output */
unsigned short efr_inw(int n);            /* Word data input */
void efr_outw(int n, unsigned short c);   /* Word data output */
unsigned long efr_inl(int n);             /* Long Word data input */
void efr_outl(int n, unsigned long c);    /* Long Word data output */
void efr_set(int n, unsigned char c);     /* Bit-set control for Byte data */
void efr_clr(int n, unsigned char c);     /* Bit-reset control for Byte data */
void efr_setw(int n, unsigned short c);   /* Bit-set control for Word data */
void efr_clrw(int n, unsigned short c);   /* Bit-reset control for Word data */

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

#ifdef IO_BASE2
#define efr_in(n)       (*(IO_BASE2+(n)))
#define efr_out(n,c)    (*(IO_BASE2+(n))=(c))
#define efr_inw(n)      (*(volatile UH *)(IO_BASE2+(n)))
#define efr_outw(n,c)   (*(volatile UH *)(IO_BASE2+(n))=(c))
#define efr_inl(n)      (*(volatile UW *)(IO_BASE2+(n)))
#define efr_outl(n,c)   (*(volatile UW *)(IO_BASE2+(n))=(c))
#define efr_set(n,c)    (*(IO_BASE2+(n))|=(c))
#define efr_clr(n,c)    (*(IO_BASE2+(n))&=~(c))
#define efr_setw(n,c)   (*(volatile UH *)(IO_BASE2+(n))|=(c))
#define efr_clrw(n,c)   (*(volatile UH *)(IO_BASE2+(n))&=~(c))
#endif

/* Offset address for Control Registers */
#define CCN_PTEH        0x000000    /* Page table entry higher register */
#define CCN_PTEL        0x000004    /* Page table entry low register */
#define CCN_TTB         0x000008    /* Translation table base register */
#define CCN_TEA         0x00000c    /* TLB exception address register */
#define CCN_MMUCR       0x000010    /* MMU Conrtol register */
#define CCN_BASRA       0x000014    /* Break ASID register-A */
#define CCN_BASRB       0x000018    /* Break ASID register-B */
#define CCN_CCR         0x00001c    /* Cache control register */
#define CCN_TRA         0x000020    /* TRAPA exception register */
#define CCN_EXPEVT      0x000024    /* Exception event register */
#define CCN_INTEVT      0x000028    /* Interrupt event register */
#define CCN_PTEA        0x000034    /* Page table entry assistance reg. */
#define CCN_QACR0       0x000038    /* Queue address control register0 */
#define CCN_QACR1       0x00003c    /* Queue address control register1 */

#define UBC_BARA        0x200000    /* Break address register-A */
#define UBC_BAMRA       0x200004    /* Break address register mask reg-A */
#define UBC_BBRA        0x200008    /* Break Bus cycle register-A */
#define UBC_BARB        0x20000c    /* Break address register-B */
#define UBC_BAMRB       0x200010    /* Break address register mask reg-B */
#define UBC_BBRB        0x200014    /* Break Bus cycle register-B */
#define UBC_BDRB        0x200018    /* Break data register-B */
#define UBC_BDMRB       0x20001c    /* Break data mask register-B */
#define UBC_BRCR        0x200020    /* Break control register */

#define BSC_BCR1        0x800000    /* Bus control register-1 */
#define BSC_BCR2        0x800004    /* Bus control register-2 */
#define BSC_WCR1        0x800008    /* Wait control register-1 */
#define BSC_WCR2        0x80000c    /* Wait control register-2 */
#define BSC_WCR3        0x800010    /* Wait control register-3 */
#define BSC_MCR         0x800014    /* Memory control register */
#define BSC_PCR         0x800018    /* PCMCIA control register */
#define BSC_RTCSR       0x80001c    /* Refresh timer control/status register */
#define BSC_RTCNT       0x800020    /* Refresh timer counter */
#define BSC_RTCOR       0x800024    /* Refresh time constant register */
#define BSC_RFCR        0x800028    /* refresh count register */
#define BSC_PCTRA       0x80002c    /* Port control register-A */
#define BSC_PDTRA       0x800030    /* Port data register-A */
#define BSC_PCTRB       0x800040    /* Port control register-B */
#define BSC_PDTRB       0x800044    /* Port data register-B */
#define BSC_GPIOIC      0x800048    /* GPIO interrupt control register */
#define BSC_SDMR2       0x900000    /* Sync DRAM mode register Area-2 */
#define BSC_SDMR3       0x940000    /* Sync DRAM mode register Area-3 */

#define DMAC_SAR0       0xa00000    /* DMA source address reg-0 */
#define DMAC_DAR0       0xa00004    /* DMA destination address reg-0 */
#define DMAC_DMATCR0    0xa00008    /* DMA transfer count reg-0 */
#define DMAC_CHCR0      0xa0000c    /* DMA channel control reg-0 */
#define DMAC_SAR1       0xa00010    /* DMA source address reg-1 */
#define DMAC_DAR1       0xa00014    /* DMA destination address reg-1 */
#define DMAC_DMATCR1    0xa00018    /* DMA transfer count reg-1 */
#define DMAC_CHCR1      0xa0001c    /* DMA channel control reg-1 */
#define DMAC_SAR2       0xa00020    /* DMA source address reg-2 */
#define DMAC_DAR2       0xa00024    /* DMA destination address reg-2 */
#define DMAC_DMATCR2    0xa00028    /* DMA transfer count reg-2 */
#define DMAC_CHCR2      0xa0002c    /* DMA channel control reg-2 */
#define DMAC_SAR3       0xa00030    /* DMA source address reg-3 */
#define DMAC_DAR3       0xa00034    /* DMA destination address reg-3 */
#define DMAC_DMATCR3    0xa00038    /* DMA transfer count reg-3 */
#define DMAC_CHCR3      0xa0003c    /* DMA channel control reg-3 */
#define DMAC_DMATOR     0xa00040    /* DMA operation register */

#define CPG_FRQCR       0xc00000    /* Frequency control register */
#define CPG_STBCR       0xc00004    /* Standby control register */
#define CPG_WTCNT       0xc00008    /* Watch dog timer-counter */
#define CPG_WTCSR       0xc0000c    /* Watch dog timer control-status reg */
#define CPG_STBCR2      0xc00010    /* Standby control register-2 */
#define CPG_FRQCR2      0xc00014    /* Frequency control register-2 */

#define RTC_R64CNT      0xc80000    /* 64Hz Counter */
#define RTC_RSECCNT     0xc80004    /* seconds counter */
#define RTC_RMINCNT     0xc80008    /* minute counter */
#define RTC_RHRCNT      0xc8000c    /* Hour counter */
#define RTC_RWKCNT      0xc80010    /* Day of week counter */
#define RTC_RDAYCNT     0xc80014    /* Day counter */
#define RTC_RMONCNT     0xc80018    /* Month counter */
#define RTC_RYRCNT      0xc8001c    /* Year counter */
#define RTC_RSECAR      0xc80020    /* Second alarm register */
#define RTC_RMINAR      0xc80024    /* Minute alarm register */
#define RTC_RHRAR       0xc80028    /* Hour alarm register */
#define RTC_RWKAR       0xc8002c    /* Day of week alarm register */
#define RTC_RDAYAR      0xc80030    /* Day alarm register */
#define RTC_RMONAR      0xc80034    /* Month alarm register */
#define RTC_RCR1        0xc80038    /* RTC Control register-1 */
#define RTC_RCR2        0xc8003c    /* RTC Control register-2 */

#define INTC_ICR        0xd00000    /* Interrupt control register */
#define INTC_IPRA       0xd00004    /* Interrupt priority register-A */
#define INTC_IPRB       0xd00008    /* Interrupt priority register-B */
#define INTC_IPRC       0xd0000c    /* Interrupt priority register-C */
#define INTC_IPRD       0xd00010    /* Interrupt priority register-D */

#define TMU_TOCR        0xd80000    /* Timer Output control register */
#define TMU_TSTR        0xd80004    /* Timer start register */
#define TMU_TCOR0       0xd80008    /* Timer constant register-0 */
#define TMU_TCNT0       0xd8000c    /* Timer counter-0 */
#define TMU_TCR0        0xd80010    /* Timer control register-0 */
#define TMU_TCOR1       0xd80014    /* Timer constant register-1 */
#define TMU_TCNT1       0xd80018    /* Timer counter-1 */
#define TMU_TCR1        0xd8001c    /* Timer control register-1 */
#define TMU_TCOR2       0xd80020    /* timer constant register-2 */
#define TMU_TCNT2       0xd80024    /* Timer counter-2 */
#define TMU_TCR2        0xd80028    /* Timer control register-2 */
#define TMU_TCPR2       0xd8002c    /* Input capture register */

#define SCI_SCSMR1      0xe00000    /* Serial mode register */
#define SCI_SCBRR1      0xe00004    /* Bit rate register */
#define SCI_SCSCR1      0xe00008    /* Serial control register */
#define SCI_SCTDR1      0xe0000c    /* Transmit data register */
#define SCI_SCSSR1      0xe00010    /* Serial status register */
#define SCI_SCRDR1      0xe00014    /* Receive data register*/
#define SCI_SCSCMR1     0xe00018    /* Smart card mode register */
#define SCI_SCSPTR1     0xe0001c    /* Serial port register */

#define SCIF_SCSMR2     0xe80000    /* Serial mode register */
#define SCIF_SCBRR2     0xe80004    /* Bit rate register */
#define SCIF_SCSCR2     0xe80008    /* Serial control register */
#define SCIF_SCFTDR2    0xe8000c    /* Transmit FIFO data register */
#define SCIF_SCFSR2     0xe80010    /* Serail status register */
#define SCIF_SCFRDR2    0xe80014    /* Receive FIFO data register */
#define SCIF_SCFCR2     0xe80018    /* FIFO control register */
#define SCIF_SCFDR2     0xe8001c    /* FIFO data count register */
#define SCIF_SCSPTR2    0xe80020    /* Serial port register */
#define SCIF_SCLSR2     0xe80024    /* Line status register */

    /* Interrupt sources and levels*/

#define INT_NMI         14          /* NMI interrupt */
#define INT_IRL15       16          /* External hardware Interrupt line-15 */
#define INT_IRL14       17          /* External hardware Interrupt line-14 */
#define INT_IRL13       18          /* External hardware Interrupt line-13 */
#define INT_IRL12       19          /* External hardware Interrupt line-12 */
#define INT_IRL11       20          /* External hardware Interrupt line-11 */
#define INT_IRL10       21          /* External hardware Interrupt line-10 */
#define INT_IRL9        22          /* External hardware Interrupt line-9 */
#define INT_IRL8        23          /* External hardware Interrupt line-8 */
#define INT_IRL7        24          /* External hardware Interrupt line-7 */
#define INT_IRL6        25          /* External hardware Interrupt line-6 */
#define INT_IRL5        26          /* External hardware Interrupt line-5 */
#define INT_IRL4        27          /* External hardware Interrupt line-4 */
#define INT_IRL3        28          /* External hardware Interrupt line-3 */
#define INT_IRL2        29          /* External hardware Interrupt line-2 */
#define INT_IRL1        30          /* External hardware Interrupt line-1 */
#define INT_TUNI0       32          /* Timer-0 underflow interrupt */
#define INT_TUNI1       33          /* Timer-1 underflow interrupt */
#define INT_TUNI2       34          /* Timer-2 underflow interrupt */
#define INT_TICPI2      35          /* Input capture interrupt */
#define INT_ATI         36          /* RTC alarm interrupt */
#define INT_PRI         37          /* RTC periodic interrupt */
#define INT_CUI         38          /* RTC carry-up interrupt */
#define INT_ERI         39          /* SCI Receive error interrupt */
#define INT_RXI         40          /* SCI receive data full interrupt */
#define INT_TXI         41          /* SCI transmit data empty interrupt */
#define INT_TEI         42          /* SCI transmit end interrupt */
#define INT_ITI         43          /* Watch Dog interval timer interrupt */
#define INT_RCMI        44          /* REF compare-match interrupt */
#define INT_ROVI        45          /* Refresh counter overflow interrupt */
#define INT_HUDI        48          /* HUDI interrupt */
#define INT_GPIO        49          /* GPIO äÑÇËçûÇ› */
#define INT_DMTE0       50          /* DMA CH0 transfer end interrupt */
#define INT_DMTE1       51          /* DMA CH1 transfer end interrupt */
#define INT_DMTE2       52          /* DMA CH2 transfer end interrupt */
#define INT_DMTE3       53          /* DMA CH3 transfer end interrupt */
#define INT_DMAE        54          /* DMA address error interrupt */
#define INT_FERI        56          /* SCIF receive error interrupt */
#define INT_FRXI        57          /* SCIF receive data full interrupt */
#define INT_FBRI        58          /* SCIF break interrupt request */
#define INT_FTXI        59          /* SCIF transmit data empty interrupt */

#define INT_PCISERR     80          /* PCI system error interrupt */
#define INT_PCIDMA3     81          /* DMA ch-3 transfer complete int */
#define INT_PCIDMA2     82          /* DMA ch-2 transfer complete int */
#define INT_PCIDMA1     83          /* DMA ch-1 transfer complete int */
#define INT_PCIDMA0     84          /* DMA ch-0 transfer complete int */
#define INT_PCIPWON     85          /* transition to normal power mode */
#define INT_PCIPWDWN    86          /* transition to power down mode */
#define INT_PCIERR      87          /* PCI error interrupt */

#define INT_TUNI3       88          /* Timer-3 underflow interrupt */
#define INT_TUNI4       92          /* Timer-4 underflow interrupt */

/* added offset adress for PCI control */
#define INTC_INTPRI00     0x080000  /* Interrupt priority level shifting reg */
#define INTC_INTREQ00     0x080020  /* Interrupt factor register */
#define INTC_INTMSK00     0x080040  /* Interrupt mask register */
#define INTC_INTMSKCLR00  0x080060  /* Interrupt factor register */

#define CPG_CLKSTP00      0x0A0000  /*  */
#define CPG_CLKSTPCLR00   0x0A0008  /*  */

#define TMU_TSTR2     0x100004  /* Timer start register-2 */
#define TMU_TCOR3     0x100008  /* Timer constant register-3 */
#define TMU_TCNT3     0x10000C  /* Timer counter-3 */
#define TMU_TCR3      0x100010  /* Timer control register-3 */
#define TMU_TCOR4     0x100014  /* Timer constant register-4 */
#define TMU_TCNT4     0x100018  /* Timer counter-4 */
#define TMU_TCR4      0x10001C  /* Timer control register-4 */

#define PCIC_PCICONF0     0x200000  /* PCI confiugration register-0 */
#define PCIC_PCICONF1     0x200004  /* PCI confiugration register-1 */
#define PCIC_PCICONF2     0x200008  /* PCI confiugration register-2 */
#define PCIC_PCICONF3     0x20000C  /* PCI confiugration register-3 */
#define PCIC_PCICONF4     0x200010  /* PCI confiugration register-4 */
#define PCIC_PCICONF5     0x200014  /* PCI confiugration register-5 */
#define PCIC_PCICONF6     0x200018  /* PCI confiugration register-6 */
#define PCIC_PCICONF7     0x20001C  /* PCI confiugration register-7 */
#define PCIC_PCICONF8     0x200020  /* PCI confiugration register-8 */
#define PCIC_PCICONF9     0x200024  /* PCI confiugration register-9 */
#define PCIC_PCICONF10    0x200028  /* PCI confiugration register-10 */
#define PCIC_PCICONF11    0x20002C  /* PCI confiugration register-11*/
#define PCIC_PCICONF12    0x200030  /* PCI confiugration register-12 */
#define PCIC_PCICONF13    0x200034  /* PCI confiugration register-13 */
#define PCIC_PCICONF14    0x200038  /* PCI confiugration register-14 */
#define PCIC_PCICONF15    0x20003C  /* PCI confiugration register-15 */
#define PCIC_PCICONF16    0x200040  /* PCI confiugration register-16 */
#define PCIC_PCICONF17    0x200044  /* PCI confiugration register-17 */

#define PCIC_PCICR        0x200100  /* PCI control register */
#define PCIC_PCILSR0      0x200104  /* PCI local space register-0 */
#define PCIC_PCILSR1      0x200108  /* PCI local space register-1 */
#define PCIC_PCILAR0      0x20010C  /* PCI local address register-0 */
#define PCIC_PCILAR1      0x200110  /* PCI local address register-1 */
#define PCIC_PCIINT       0x200114  /* PCI interrupt register */
#define PCIC_PCIINTM      0x200118  /* PCI interrupt mask register */
#define PCIC_PCICLR       0x20011C  /* PCI command data reg at error */
#define PCIC_PCIMBLR      0x200120  /* PCI arbiter bus master data register */
#define PCIC_PCIAINT      0x200130  /* PCI arbiter interrupt register */
#define PCIC_PCIAINTM     0x200134  /* PCI arbiter interrupt mask register */
#define PCIC_PCIALR       0x200138  /* PCI address data reg at error */
#define PCIC_PCIDMABT     0x200140  /* PCI DMA transfer arbitration master */

#define PCIC_PCIDPA0      0x200180  /* PCI DMA transfer address register-0 */
#define PCIC_PCIDLA0      0x200184  /* PCI DMA transfer local addr reg-0 */
#define PCIC_PCIDTC0      0x200188  /* PCI DMA transfer counter register-0 */
#define PCIC_PCIDCR0      0x20018C  /* PCI DMA control register-0 */
#define PCIC_PCIDPA1      0x200190  /* PCI DMA transfer address register-1 */
#define PCIC_PCIDLA1      0x200194  /* PCI DMA transfer local addr reg-1 */
#define PCIC_PCIDTC1      0x200198  /* PCI DMA transfer counter register-1 */
#define PCIC_PCIDCR1      0x20019C  /* PCI DMA control register-1 */
#define PCIC_PCIDPA2      0x2001A0  /* PCI DMA transfer address register-2 */
#define PCIC_PCIDLA2      0x2001A4  /* PCI DMA transfer local addr reg-2 */
#define PCIC_PCIDTC2      0x2001A8  /* PCI DMA transfer counter register-2 */
#define PCIC_PCIDCR2      0x2001AC  /* PCI DMA control register-2 */
#define PCIC_PCIDPA3      0x2001B0  /* PCI DMA transfer address register-3 */
#define PCIC_PCIDLA3      0x2001B4  /* PCI DMA transfer local addr reg-3 */
#define PCIC_PCIDTC3      0x2001B8  /* PCI DMA transfer counter register-3 */
#define PCIC_PCIDCR3      0x2001BC  /* PCI DMA control register-3 */
#define PCIC_PCIPAR       0x2001C0  /* PIO address register */
#define PCIC_PCIMBR       0x2001C4  /* Memory space base register */
#define PCIC_PCIIOBR      0x2001C8  /* IO space base register */
#define PCIC_PCIPINT      0x2001CC  /* PCI power management interrupt reg */
#define PCIC_PCIPINTM     0x2001D0  /* PCI power management int mask reg */
#define PCIC_PCICLKR      0x2001D4  /* PCI clock control register */

#define PCIC_PCIBCR1      0x2001E0  /* PCI bus control register-1 */
#define PCIC_PCIBCR2      0x2001E4  /* PCI bus control register-2 */
#define PCIC_PCIWCR1      0x2001E8  /* PCI wait control register-1 */
#define PCIC_PCIWCR2      0x2001EC  /* PCI wait control register-2 */
#define PCIC_PCIWCR3      0x2001F0  /* PCI wait control register-3 */
#define PCIC_PCIMCR       0x2001F4  /* PCI discrete memory control register */

#define PCIC_PCIPCTR      0x200200  /* PCI port control register */
#define PCIC_PCIPDTR      0x200204  /* PCI port data register */
#define PCIC_PCIPDR       0x200220  /* PIO data register */

#endif /* SH7751_H */
