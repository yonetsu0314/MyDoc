;
; $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/hitachi/RCS/start.s 1.1 1970/01/01 00:00:00 masahiro Exp $
; $Log: start.s $
; リビジョン 1.1  1970/01/01  00:00:00  masahiro
; 初期リビジョン
; 
;
;----------------------------------------------------------------------
;		(C) Copyright 2002, ISHIDA CO., LTD.                           
;		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
;		PHONE (077) 553-4141                                           
;----------------------------------------------------------------------
; Startup routine - for MS7615ASE01 board

STACK_SIZE:	.equ	16384	; Stack size. Must be set to desiable value.

		.INCLUDE "sh7615.inc"

		.IMPORT	_scifinit
		.IMPORT	_sendchar
		.IMPORT	_sendstr
		.IMPORT	_sendhex8
		.IMPORT	_sendhex16
		.IMPORT	_sendhex32

;; on-board LED
LED:		.equ	H'22200000		; on-board LED


;**** Code section *****************************************************

    .aif \&RAM EQ 0
;** Reset vector section. Must be located at H'00000000 ******
	.SECTION	Reset,DATA,LOCATE=H'00000000
	.data.l		__cold_start		; Power-on reset start vector
	.data.l		__initial_stack
	.data.l		__warm_start		; Manual reset start vector
	.data.l		__initial_stack
    .aendi
;
;** General vector section.
	.SECTION	Vector,DATA,ALIGN=4
	.EXPORT		_vector_table
_vector_table:
	.res.l		4			; dummy area for reset vector
	.res.l		128-4			; other vectors

;*************************************************************
;** Code section. ********************************************
;*************************************************************
    .aif \&RAM NE 0
;-------------------------------------------------------------
;---- entry point for warm-boot. -----------------------------
; モニタから実行する際に開始アドレスを固定するためのセクション
; RAM領域の先頭等解りやすいアドレスに配置する
;-------------------------------------------------------------
	.SECTION	Entry,CODE,ALIGN=4
	mov.l	#__warm_start,R0
	jmp	@R0
	nop
    .aendi

;-------------------------------------------------------------
;---- Power-on start -----------------------------------------
;-------------------------------------------------------------
	.SECTION    S,CODE,ALIGN=4
	.IMPORT     _main
	.EXPORT     __cold_start
	.EXPORT     __warm_start
;
__cold_start:
; initialize bus state controller
	mov.l	#BCR1,R1
	mov.l	#H'a55a63f1,R0	; CS3=SDRAM
	mov.l	R0,@R1

	mov.l	#BCR2,R1
	mov.l	#H'a55a02f8,R0	; CS4=16bit/CS3=32bit/CS2=32bit/CS1=16bit
	mov.l	R0,@R1

	mov.l	#BCR3,R1
	mov.l	#H'a55a0c01,R0	; SDRAM 4-bank,burst write
	mov.l	R0,@R1

	mov.l	#WCR1,R1
	mov.l	#H'a55aab7f,R0
	mov.l	R0,@R1

	mov.l	#WCR2,R1
	mov.l	#H'a55a000b,R0
	mov.l	R0,@R1

	mov.l	#WCR3,R1
	mov.l	#H'a55a0010,R0
	mov.l	R0,@R1

	mov.l	#MCR,R1
	mov.l	#H'a55a9ddb,R0
	mov.l	R0,@R1

	mov.l	#RTCOR,R1
	mov.l	#H'a55a003a,R0
	mov.l	R0,@R1

	mov.l	#RTCNT,R1
	mov.l	#H'a55a0039,R0
	mov.l	R0,@R1

;;; ここから
	mov.l	#RTCSR,R1
	mov.l	#H'a55a0000,R0		; stop refresh
	mov.l	R0,@R1

	mov.l	#H'1000,R0		; idle time
cold1:
	add	#-1,R0
	cmp/eq	#0,R0
	bf	cold1

	mov.l	#H'ffff8088,R1
	mov.w	#0,R0
	mov.w	R0,@R1

	mov.l	#RTCSR,R1
	mov.l	#H'a55a0008,R0
	mov.l	R0,@R1

	mov.l	#0,R2
	mov.w	#8,R3
cold2:
	mov.l	@R1,R0
	tst	#H'80,R0
	bt	cold2

	add	#1,R2
	cmp/eq	R2,R3
	bt	cold3
	mov.l	#H'a55a0008,R0
	mov.l	R0,@R1
	bra	cold2
	nop
cold3:
;;; ここまで

;;	mov.l	#H'ffff0088,R1
;;	mov.l	R0,@R1			; dummy write

__warm_start:
	mov.l	#LED,R1			; for Debugging.
	mov.w	#H'0100,R0
	mov.w	R0,@R1

; Setup Pin Function Controller
	mov.l	#PACR,R1
	mov.w	#H'0004,R0		; Select LNKSTA
	mov.w	R0,@R1

	mov.l	#PBCR,R1
	mov.w	#H'aaaa,R0		; Select SCIF,TPU1,TPU2
	mov.w	R0,@R1

	mov.l	#PBCR2,R1
	mov.w	#H'aaaa,R0		; Select SCIF,TPU0,TPU2
	mov.w	R0,@R1

; initialize cache
	mov.l	#CCR,R1
	mov.b	@R1,R0
	and	#H'FE,R0
	mov.b	R0,@R1		; cache disable
    .aif \&CACHE EQ 1		; if enabled in compiler.mak
	or	#H'10,R0
	mov.b	R0,@R1		; cache clear
	or	#H'01,R0
	mov.b	R0,@R1		; cache enable
    .aendi

; Setup Oscillator
	mov.l	#FMR,R1			; I/E/P = 4:4:2
	mov.b	#H'0e,R0
	mov.b	R0,@R1

; Setup SCIF (for Debug Console)
	mov.l	#_scifinit,R0
	jsr	@R0
	nop

	mov.l	#LED,R1			; for Debugging.
	mov.w	#H'0200,R0
	mov.w	R0,@R1

; Setup Interrupt Contoller (INTC)
	mov.l	#ICR,R1
	mov.w	#H'0000,R0		; NMI fall edge, IRL mode, auto vector
	mov.w	R0,@R1

; set vbr
	mov.l	#__istart,R3
	mov.l	@R3,R3
	ldc	R3,VBR

; set exception handler vector for debugging
;	mov.l	#_vector_table,R1
;	mov.l	@R1,R1
;	mov.l	#illegalinst,R2
;	mov.l	R2,@(D'16,R1)
;	mov.l	#cpuaddrerr,R2
;	mov.l	R2,@(D'36,R1)

; clear bss
	mov.l	#_bssclr,R0
	jsr	@R0
	nop

	mov.l	#LED,R1			; for Debugging.
	mov.w	#H'0300,R0
	mov.w	R0,@R1

; move initialized data to ram
    .aif \&RAM EQ 0
	mov.l	#_dmove,R0
	jsr	@R0
	nop
    .aendi

	mov.l	#LED,R1			; for Debugging.
	mov.w	#H'0400,R0
	mov.w	R0,@R1

; Enable interrupt (set mask level to 0)
	stc.l	SR,R0
	mov.l	#~IMASK,R1
	and	R1,R0
	ldc.l	R0,SR

	mov.l	#__stack,r15

; Jump to main()
main:
	mov.l	#1,R4		; argc
	mov.l	#argvtbl,R5	; argv
	mov.l	#_main,R0
	jsr	@R0
	nop

	mov.l	#LED,R1			; for Debugging.
	mov.w	#H'ff00,R0
	mov.w	R0,@R1

loop:                   ; Return from main() and enter 
sleep:                  ;   Loop forever
	sleep
	bra	loop
	nop

;
;  Zero out the bss.
;
;*bss clear**************************/

_bssclr:
	mov.l	#__b_begin,R0
	mov.l	@R0,R0
	mov.l	#__b_end,R1
	mov.l	@R1,R1
	mov.l	#0,R2
clr:
	cmp/ge	R1,R0
	bt	clrend
	bra	clr
	mov.b	R2,@-R1
clrend:
	rts
	nop

; move initialized data to ram
    .aif \&RAM EQ 0
;*data move**************************/
_dmove:
	mov.l	#__d_begin,R0
	mov.l	@R0,R0
	mov.l	#__r_begin,R1
	mov.l	@R1,R1
	mov.l	#__r_end,R2
	mov.l	@R2,R2
dmv:
	cmp/ge	R2,R1
	bt	dmvend
	mov.b	@R0+,R3
	mov.b	R3,@R1
	bra	dmv
	add	#1,R1
dmvend:
	rts
	nop
    .aendi

;-------------------------------------------------------------
;---- exception handlers -------------------------------------
;-------------------------------------------------------------
illegalinst:
	mov.l	@SP,R8
	mov.l	#mesillegalinst,R4
	mov.l	#_sendstr,R2
	jsr	@R2
	nop

genfault:
	mov.l	#meslocation,R4
	mov.l	#_sendstr,R2
	jsr	@R2
	nop

	mov.l	R8,R4
	mov.l	#_sendhex32,R2
	jsr	@R2
	nop

	mov.l	#mescrlf,R4
	mov.l	#_sendstr,R2
	jsr	@R2
	nop

genhalt:
	bra	genhalt
	nop

cpuaddrerr:
	mov.l	@SP,R8
	mov.l	#mescpuaddrerr,R4
	mov.l	#_sendstr,R2
	jsr	@R2
	nop

	bra	genfault
	nop

mesillegalinst:
	.sdata	"Illegal instruction "
	.data.b	0
mescpuaddrerr:
	.sdata	"CPU address error "
	.data.b	0
meslocation:
	.sdata	"at address "
	.data.b	0
mescrlf:
	.data.b	H'0d,H'0a
	.data.b	0
;*************************************************************
;** CONST section ********************************************
;*************************************************************
	.SECTION	C,DATA,ALIGN=4

	.EXPORT	__d_begin	; exportしているのはデバッグ時にシンボル名を
	.EXPORT	__b_begin	; 参照できるようにするため。外部参照はしていない。
	.EXPORT	__b_end
	.EXPORT	__istart

__d_begin	.DATA.L	(STARTOF D)
__b_begin	.DATA.L	(STARTOF B)
__b_end		.DATA.L	(STARTOF B) + (SIZEOF B)
__istart	.DATA.L	(STARTOF Vector)

    .aif \&RAM EQ 0
	.EXPORT	__r_begin
	.EXPORT	__r_end
__r_begin	.DATA.L	(STARTOF R)
__r_end		.DATA.L	(STARTOF R) + (SIZEOF R)
    .aendi

; dummy argment to main()
argvtbl		.data.l	H'0
		.data.l	H'0

;*************************************************************
;** DATA section *********************************************
;*************************************************************
	.SECTION	D,DATA,ALIGN=4

;*************************************************************
;** BSS section **********************************************
;*************************************************************
; Stack area
	.SECTION	B,DATA,ALIGN=4
	.SECTION	STK,DATA,ALIGN=4
	.res.l		100
__initial_stack:

	.SECTION	B,DATA,ALIGN=4
	.SECTION	STACK,DATA,ALIGN=4
	.res.l		STACK_SIZE
__stack		.DATA.L	(STARTOF STACK + SIZEOF STACK )

    .aif \&RAM EQ 0

;*************************************************************
;** R section ************************************************
;*************************************************************
	.SECTION	R,DATA,ALIGN=4
    .aendi

	.END	__warm_start
