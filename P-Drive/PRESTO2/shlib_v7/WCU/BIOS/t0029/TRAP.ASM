;*----------------------------------------------------------------------*;
;* File name	: trap.asm	/ 					*;
;*----------------------------------------------------------------------*;
;*$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/trap.asm 1.4 1970/01/01 00:00:00 chimura Exp $
;*$Log: trap.asm $
; リビジョン 1.4  1970/01/01  00:00:00  chimura
; 2003/09/23 12:00
; SSRとSPCを退避する。
; 
; リビジョン 1.3  1970/01/01  00:00:00  chimura
; 2003/09/22 20:00
; 前リビジョンの不具合修正
; 
; リビジョン 1.2  1970/01/01  00:00:00  chimura
; 2003/09/22 19:00
; trap中は割り込み禁止とする。
; 
; リビジョン 1.1  1970/01/01  00:00:00  sakaguti
; P5561WCUブートローダ
; 
;*----------------------------------------------------------------------
;*		(c) Copyright 2002, ISHIDA CO., LTD.
;*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
;*		(077) 553-4141
;*----------------------------------------------------------------------*;
;* Function								*;
;*----------------------------------------------------------------------*;
;
	.IMPORT	_vecttbl
	.SECTION TRP,CODE,ALIGN=4

__exp:
	NOP
	STC.L	SSR,@-R15 ;save SSR
	STC.L	SPC,@-R15 ;save SPC
        STS.L   PR,@-R15
	stc.l	r0_bank,@-r15		; R0_BANK0 退避
	mov.l	r0,@-r15		; R0_BANK1 退避
	mov.l	r1,@-r15		; R1_BANK1 退避
	mov.l	r2,@-r15		; R2_BANK1 退避

	MOV.L	EXPEVT,R0
	MOV.L	@R0,R0
	CMP/EQ	#0,R0 ; if (EXPEVT==H'000)
	BT/S	exit ; exit();
	NOP

	MOV	R0,R1
	MOV.L	VECTadr,R0
	SHLR2	R1
	SHLR	R1
	MOV.L	@(R0,R1),R2 ; vect CODE from 000
	mov.l	SRbase,r1		;

	mov	r2,r0			;copy
	mov.l	@r15+,r2		; R2_BANK1 復元
	ldc	r1,sr			;set SR
					; ここで、RB=0 に切り替わる
	stc	r0_bank,r0		; R0_BANK0 = R0_BANK1
	ldc.l	@r15+,r1_bank		; R1_BANK1 復元
	ldc.l	@r15+,r0_bank		; R0_BANK1 復元

	jsr	@r0			; trap へ
	mov.l	@r15+,r0		; R0_BANK0 復元

	mov.l	r1,@-r15		; R1 退避
	mov.l	SRbase,r1		;
	ldc	r1,sr			;set SR
	mov.l	@r15+,r1		; R1 復元

        LDS.L   @R15+,PR
	LDC.L	@R15+,SPC
	LDC.L	@R15+,SSR
	RTE
	NOP


	.ALIGN 4
exit:
	mov.l	@r15+,r2		; R2_BANK1 復元
	mov.l	@r15+,r1		; R1_BANK1 復元
	mov.l	@r15+,r0		; R0_BANK1 復元
	ldc.l	@r15+,r0_bank		; R0_BANK0 復元
        LDS.L   @R15+,PR
	LDC.L	@R15+,SPC
	LDC.L	@R15+,SSR
	RTE
	NOP


	.ALIGN 4
EXPEVT:		.DATA.L H'ffffffd4
VECTadr:	.DATA.L _vecttbl
SRbase:		.DATA.L	H'500000F0


;
; Software trap (TRAPA) handler Routine
;
	.EXPORT __trap
	.IMPORT _traptbl
	.SECTION TRP,CODE,ALIGN=4
	.ALIGN 4
__trap
        STS.L   PR,@-R15
	MOV.L	R0,@-R15 ;save work register
	MOV.L	R1,@-R15

	MOV.L	TRA,R0
	MOV.L	@R0,R0 ; get TRA no
	MOV.L	TRA_TOP,R1
	ADD	R1,R0

	MOV.L	TRAPTBL,R1
	MOV.L	@(R0,R1),R0 ; R0 = traptbl[TRA]

	MOV.L	@R15+,R1
	JSR	@R0 ; (*traptbl[TRA])(); /* call */
	MOV.L	@R15+,R0
        LDS.L   @R15+,PR
	RTS
	NOP


	.ALIGN 4
TRA:		.DATA.L H'ffffffd0
TRAPTBL:	.DATA.L _traptbl
TRA_TOP:	.DATA.L -H'200
	.END
