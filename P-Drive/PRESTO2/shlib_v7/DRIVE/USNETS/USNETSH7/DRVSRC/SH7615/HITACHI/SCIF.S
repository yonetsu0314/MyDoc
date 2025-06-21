;
; $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/hitachi/RCS/scif.s 1.1 1970/01/01 00:00:00 masahiro Exp $
; $Log: scif.s $
; リビジョン 1.1  1970/01/01  00:00:00  masahiro
; 初期リビジョン
; 
;
;----------------------------------------------------------------------
;		(C) Copyright 2002, ISHIDA CO., LTD.                           
;		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
;		PHONE (077) 553-4141                                           
;----------------------------------------------------------------------
;
;; SCIF communication routine - for MS7615ASE01 board
	.SECTION    P,CODE,ALIGN=4

	.INCLUDE "sh7615.inc"

	.EXPORT	_scifinit
	.EXPORT	_sendchar
	.EXPORT	_sendstr
	.EXPORT	_sendhex8
	.EXPORT	_sendhex16
	.EXPORT	_sendhex32

;** initialize scif **************************
;** void scifinit(void);
_scifinit:
	sts.l	PR,@-SP

	mov.l	#SCSCR1,R0
	mov.b	#H'0,R1			; disable receiver/transmitter
	mov.b	R1,@R0

	mov.l	#SCFCR1,R0
	mov.b	#H'6,R1			; reset FIFO
	mov.b	R1,@R0

	mov.l	#SCSCR1,R0
	mov.b	#H'2,R1			; external clock
	mov.b	R1,@R0

	mov.l	#SCSMR1,R0
	mov.b	#H'0,R1			; 8N1
	mov.b	R1,@R0

	mov.l   #100000,r0
scif1:	dt      r0
	bf      scif1

	mov.l	#SCFCR1,R0
	mov.b	#H'8,R1			; use RTS/CTS
	mov.b	R1,@R0

	mov.l	#SCSCR1,R0
	mov.b	#H'32,R1		; interrupts disabled / external clock
	mov.b	R1,@R0

	lds.l	@SP+,PR
	rts
	nop

; Send 1 char to SCIF
; void sendchar(int char);
; Input - R4 .... char

_sendchar:
	sts.l	PR,@-SP

	mov.l	#SC1SSR1,R1
send1001:
	mov.w	@R1,R2
	mov.l	#H'20,R3
	tst	R3,R2
	bt	send1001
	mov.l	#SCFTDR1,R1
	mov.b	R4,@R1

	mov.l	#SC1SSR1,R1
send1002:
	mov.w	@R1,R2
	mov.l	#H'20,R3
	tst	R3,R2
	bt	send1002
	mov.w	#H'df,R3	; clear TDFE
	and	R3,R2
	mov.w	R2,@R1

	lds.l	@SP+,PR
	rts
	nop

; Display null-terminated string to SCIF
; void sendstr(char *str);
; Input - R4 .... pointer to string
; **** Mar/3/2000 currently not working. Do not call. ****
_sendstr:
	sts.l	PR,@-SP

	mov.l	R4,R1
sends1:
	mov.b	@R1+,R0
	cmp/eq	#0,R0
	bt	sends2

	mov	R0,R4
	mov.l	#_sendchar,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	bra	sends1
	nop

sends2:
	lds.l	@SP+,PR
	rts
	nop

; Display 8bit value to SCIF
; void sendhex8(unsigned char value);
; Input - R4
_sendhex8:
	sts.l	PR,@-SP

	mov.l	R4,R1
	shlr2	R4
	shlr2	R4
	mov.l	#H'0f,R2
	and	R2,R4
	add	#H'30,R4
	mov.l	#H'3A,R2
	cmp/hs	R2,R4
	bf	s8001
	add	#7,R4
s8001:
	mov.l	#_sendchar,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	mov.l	R1,R4
	mov.l	#H'0f,R2
	and	R2,R4
	add	#H'30,R4
	mov.l	#H'3A,R2
	cmp/hs	R2,R4
	bf	s8002
	add	#7,R4
s8002:
	mov.l	#_sendchar,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	lds.l	@SP+,PR
	rts
	nop

; Display 16bit value to SCIF
; void sendhex16(unsigned short value);
; Input - R4
_sendhex16:
	sts.l	PR,@-SP

	mov.l	R4,R1
	shlr8	R4

	mov.l	#_sendhex8,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	mov.l	R1,R4

	mov.l	#_sendhex8,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	lds.l	@SP+,PR
	rts
	nop

; Display 32bit value to SCIF
; void sendhex32(unsigned long value);
; Input - R4
_sendhex32:
	sts.l	PR,@-SP

	mov.l	R4,R1
	shlr16	R4

	mov.l	#_sendhex16,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	mov.l	R1,R4

	mov.l	#_sendhex16,R2
	mov.l	R1,@-SP
	jsr	@R2
	nop
	mov.l	@SP+,R1

	lds.l	@SP+,PR
	rts
	nop

	.END
