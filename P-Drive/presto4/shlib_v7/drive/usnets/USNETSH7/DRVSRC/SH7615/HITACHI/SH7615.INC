;
; $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/hitachi/RCS/sh7615.inc 1.1 1970/01/01 00:00:00 masahiro Exp $
; $Log: sh7615.inc $
; リビジョン 1.1  1970/01/01  00:00:00  masahiro
; 初期リビジョン
; 
;
;----------------------------------------------------------------------
;		(C) Copyright 2002, ISHIDA CO., LTD.                           
;		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
;		PHONE (077) 553-4141                                           
;----------------------------------------------------------------------
;; SH7615 register difinitions

;**** Register address difinitions *************************************
;; Bit mask of Status Register (SR)
IMASK:		.equ	H'000000F0

;; Registers in Interrupt Control Unit
IPRA:		.equ	H'FFFFFEE2		; Interrupt Priority A (16bit)
IPRB:		.equ	H'FFFFFE60		; Interrupt Priority B (16bit)
IPRC:		.equ	H'FFFFFEE6		; Interrupt Priority C (16bit)
IPRD:		.equ	H'FFFFFE40		; Interrupt Priority D (16bit)
IPRE:		.equ	H'FFFFFEC0		; Interrupt Priority E (16bit)
VCRA:		.equ	H'FFFFFE62		; Interrupt Vector A (16bit)
VCRC:		.equ	H'FFFFFE66		; Interrupt Vector C (16bit)
VCRD:		.equ	H'FFFFFE68		; Interrupt Vector D (16bit)
VCRE:		.equ	H'FFFFFE42		; Interrupt Vector E (16bit)
VCRF:		.equ	H'FFFFFE44		; Interrupt Vector F (16bit)
VCRG:		.equ	H'FFFFFE46		; Interrupt Vector G (16bit)
VCRH:		.equ	H'FFFFFE48		; Interrupt Vector H (16bit)
VCRI:		.equ	H'FFFFFE4A		; Interrupt Vector I (16bit)
VCRJ:		.equ	H'FFFFFE4C		; Interrupt Vector J (16bit)
VCRK:		.equ	H'FFFFFE4E		; Interrupt Vector K (16bit)
VCRL:		.equ	H'FFFFFE50		; Interrupt Vector L (16bit)
VCRM:		.equ	H'FFFFFE52		; Interrupt Vector M (16bit)
VCRN:		.equ	H'FFFFFE54		; Interrupt Vector N (16bit)
VCRO:		.equ	H'FFFFFE56		; Interrupt Vector O (16bit)
VCRP:		.equ	H'FFFFFEC2		; Interrupt Vector P (16bit)
VCRQ:		.equ	H'FFFFFEC4		; Interrupt Vector Q (16bit)
VCRR:		.equ	H'FFFFFEC6		; Interrupt Vector R (16bit)
VCRS:		.equ	H'FFFFFEC8		; Interrupt Vector S (16bit)
VCRT:		.equ	H'FFFFFECA		; Interrupt Vector T (16bit)
VCRU:		.equ	H'FFFFFECC		; Interrupt Vector U (16bit)
VCRWDT:		.equ	H'FFFFFEE4		; Interrupt Vector WDT (16bit)
VCRDMA0:	.equ	H'FFFFFFA0		; Interrupt Vector DMA0 (32bit)
VCRDMA1:	.equ	H'FFFFFFA8		; Interrupt Vector DMA1 (32bit)
ICR:		.equ	H'FFFFFEE0		; Interrupt Control (16bit)
IRQCSR:		.equ	H'FFFFFEE8		; IRQ Control/Status (16bit)

;; Registers in Oscillator Unit
FMR:		.equ	H'FFFFFE90		; Frequency Control (8bit)

;; Registers in Bus Control Unit
BCR1:		.equ	H'FFFFFFE0		; Bus Control 1 (32bit)
BCR2:		.equ	H'FFFFFFE4		; Bus Control 2 (32bit)
BCR3:		.equ	H'FFFFFFFC		; Bus Control 3 (32bit)
WCR1:		.equ	H'FFFFFFE8		; Wait Control 1 (32bit)
WCR2:		.equ	H'FFFFFFC0		; Wait Control 2 (32bit)
WCR3:		.equ	H'FFFFFFC4		; Wait Control 3 (32bit)
MCR:		.equ	H'FFFFFFEC		; Memory Control (32bit)
RTCSR:		.equ	H'FFFFFFF0		; Refresh Timer Control (32bit)
RTCNT:		.equ	H'FFFFFFF4		; Refresh Timer Counter (32bit)
RTCOR:		.equ	H'FFFFFFF8		; Refresh Time Constant (32bit)

;; Registers in Cache Control Unit
CCR:		.equ	H'FFFFFE92		; Cache Control (8bit)

;; Registers in 16bit Free-running timer unit
TIER:		.equ	H'FFFFFE10		; Timer interrupt enable (8bit)
FTCSR:		.equ	H'FFFFFE11		; Free-running timer control/status (8bit)
FRC:		.equ	H'FFFFFE12		; Free-running count (16bit)
OCR:		.equ	H'FFFFFE14		; Output compare A/B (16bit)
TCR:		.equ	H'FFFFFE16		; Timer control (8bit)
TOCR:		.equ	H'FFFFFE17		; Timer output compare control (8bit)
FICR:		.equ	H'FFFFFE18		; Input capture (16bit)

;; Registers in serial communication interface with FIFO 1 (SCIF)
SCSMR1:		.equ	H'FFFFFCC0		; Serial mode (8bit)
SCBRR1:		.equ	H'FFFFFCC2		; Bit rate (8bit)
SCSCR1:		.equ	H'FFFFFCC4		; Serial Control (8bit)
SCFTDR1:	.equ	H'FFFFFCC6		; Transmit FIFO data (8bit)
SC1SSR1:	.equ	H'FFFFFCC8		; Serial status 1 (16bit)
SC2SSR1:	.equ	H'FFFFFCCA		; Serial status 2 (8bit)
SCFRDR1:	.equ	H'FFFFFCCC		; Receive FIFO data (8bit)
SCFCR1:		.equ	H'FFFFFCCE		; FIFO control (8bit)
SCFDR1:		.equ	H'FFFFFCD0		; FIFO data count (16bit)
SCFER1:		.equ	H'FFFFFCD2		; FIFO error (16bit)
SCIMR1:		.equ	H'FFFFFCD4		; IrDA mode (8bit)

;; Registers in serial communication interface with FIFO 2 (SCIF)
SCSMR2:		.equ	H'FFFFFCE0		; Serial mode (8bit)
SCBRR2:		.equ	H'FFFFFCE2		; Bit rate (8bit)
SCSCR2:		.equ	H'FFFFFCE4		; Serial Control (8bit)
SCFTDR2:	.equ	H'FFFFFCE6		; Transmit FIFO data (8bit)
SC1SSR2:	.equ	H'FFFFFCE8		; Serial status 1 (16bit)
SC2SSR2:	.equ	H'FFFFFCEA		; Serial status 2 (8bit)
SCFRDR2:	.equ	H'FFFFFCEC		; Receive FIFO data (8bit)
SCFCR2:		.equ	H'FFFFFCEE		; FIFO control (8bit)
SCFDR2:		.equ	H'FFFFFCF0		; FIFO data count (16bit)
SCFER2:		.equ	H'FFFFFCF2		; FIFO error (16bit)
SCIMR2:		.equ	H'FFFFFCF4		; IrDA mode (8bit)

;; Registers in 16bit timer pulse unit (TPU)
TCR0:		.equ	H'FFFFFC50		; Timer control 0 (8bit)
TMDR0:		.equ	H'FFFFFC51		; Timer mode 0 (8bit)
TIOR0H:		.equ	H'FFFFFC52		; Timer I/O control 0H (8bit)
TIOR0L:		.equ	H'FFFFFC53		; Timer I/O control 0L (8bit)
TIER0:		.equ	H'FFFFFC54		; Timer interrupt enable 0 (8bit)
TSR0:		.equ	H'FFFFFC55		; Timer status 0 (8bit)
TCNT0:		.equ	H'FFFFFC56		; Timer count 0 (16bit)
TGR0A:		.equ	H'FFFFFC58		; Timer general 0A (16bit)
TGR0B:		.equ	H'FFFFFC5A		; Timer general 0B (16bit)
TGR0C:		.equ	H'FFFFFC5C		; Timer general 0C (16bit)
TGR0D:		.equ	H'FFFFFC5E		; Timer general 0D (16bit)
;
TCR1:		.equ	H'FFFFFC60		; Timer control 1 (8bit)
TMDR1:		.equ	H'FFFFFC61		; Timer mode 1 (8bit)
TIOR1:		.equ	H'FFFFFC62		; Timer I/O control 1 (8bit)
TIER1:		.equ	H'FFFFFC64		; Timer interrupt enable 1 (8bit)
TSR1:		.equ	H'FFFFFC65		; Timer status 1 (8bit)
TCNT1:		.equ	H'FFFFFC66		; Timer count 1 (16bit)
TGR1A:		.equ	H'FFFFFC68		; Timer general 1A (16bit)
TGR1B:		.equ	H'FFFFFC6A		; Timer general 1B (16bit)
;
TCR2:		.equ	H'FFFFFC70		; Timer control 2 (8bit)
TMDR2:		.equ	H'FFFFFC71		; Timer mode 2 (8bit)
TIOR2:		.equ	H'FFFFFC72		; Timer I/O control 2 (8bit)
TIER2:		.equ	H'FFFFFC74		; Timer interrupt enable 2 (8bit)
TSR2:		.equ	H'FFFFFC75		; Timer status 2 (8bit)
TCNT2:		.equ	H'FFFFFC76		; Timer count 2 (16bit)
TGR2A:		.equ	H'FFFFFC78		; Timer general 2A (16bit)
TGR2B:		.equ	H'FFFFFC7A		; Timer general 2B (16bit)
;
TSTR:		.equ	H'FFFFFC40		; Timer start (8bit)
TSYR:		.equ	H'FFFFFC41		; Timer synchronize (8bit)

;; Registers in Pin Function Controller (PFC)
PACR:		.equ	H'FFFFFC80		; Port A Control (16bit)
PAIOR:		.equ	H'FFFFFC82		; Port A I/O (16bit)
PBCR:		.equ	H'FFFFFC88		; Port B Control (16bit)
PBIOR:		.equ	H'FFFFFC8A		; Port B I/O (16bit)
PBCR2:		.equ	H'FFFFFC8E		; Port B Control 2
