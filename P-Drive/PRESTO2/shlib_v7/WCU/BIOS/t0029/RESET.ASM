;*----------------------------------------------------------------------*;
;* File name	: reset.asm	/ initial test				*;
;*----------------------------------------------------------------------*;
;*$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/reset.asm 1.1 1970/01/01 00:00:00 chimura Exp $
;*$Log: reset.asm $
; リビジョン 1.1  1970/01/01  00:00:00  chimura
; 初期リビジョン
; 
;*----------------------------------------------------------------------
;*		(c) Copyright 2002, ISHIDA CO., LTD.
;*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
;*		(077) 553-4141
;*----------------------------------------------------------------------*;
;* Function								*;
;*	CPUﾊｰﾄﾞｳｪｱのﾃｽﾄ、RAMのﾃｽﾄ及び初期化、RX116をOSとするｼｽﾃﾑ	*;
;*	の管理外のI/O初期設定を行う。					*;
;*----------------------------------------------------------------------*;
;
	.IMPORT _vecttbl
	.IMPORT _stacktbl
	.SECTION RES,CODE,ALIGN=4

	MOV.L	BSCR2,R0
	MOV.L	#H'15e0,R1
	MOV.W	R1,@R0
	MOV.L	WCR2,R0
	MOV.L	#H'ffff,R1
	MOV.W 	R1,@R0
	MOV.L	VECTadr,R1
	MOV.L	STACKadr,R2
	MOV.L	EXPEVT,R0
	MOV.L	@R0,R0
	CMP/EQ	#0,R0 ;POWER ON RESET
	BT	PON_RESET
	CMP/EQ	#H'20,R0
	BT	MANUAL_RESET
LOOP
	BRA	LOOP
	NOP
PON_RESET:
	MOV.L	@(0,R1),R1 ;set function
	MOV.L	@(0,R2),R15 ;set stack pointer
	JMP	@R1
	NOP
MANUAL_RESET:
	MOV.L	@(4,R1),R1 ;set function
	MOV.L	@(4,R2),R15 ;set stack pointer
	JMP	@R1
	NOP
;
	.ALIGN 4
VECTadr:	.DATA.L	_vecttbl
STACKadr:	.DATA.L _stacktbl
EXPEVT: 	.DATA.L H'ffffffd4
BSCR2:		.DATA.L H'ffffff62
WCR2: 		.DATA.L H'ffffff66
	.END
