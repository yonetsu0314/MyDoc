;*----------------------------------------------------------------------*;
;* File name	: inthd.asm	/ 					*;
;*----------------------------------------------------------------------*;
;*$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/inthd.asm 1.3 1970/01/01 00:00:00 chimura Exp $
;*$Log: inthd.asm $
; リビジョン 1.3  1970/01/01  00:00:00  chimura
; 2004/01/08 17:00
; NORTi Ver4.0に合わせる。
; 以下、MISPOからの回答
; 割込み発生から NORTi Ver.4 の割込みハンドラへ分岐するまで、どのよう
; な処理を行うか、次のようになっています。
; １．SRレジスタ以外は保存する。（使用したレジスタは復元する。）
; ２．SRレジスタの RBビットをクリアする。
; ３．SRレジスタの I0～I3ビットに割込み発生要因のレベルを設定する。
; 
; リビジョン 1.2  1970/01/01  00:00:00  chimura
; 2003/09/23 12:00
; 割り込み時、SSRとSPCを退避する。
; 
; リビジョン 1.1  1970/01/01  00:00:00  chimura
; 初期リビジョン
; 
;*----------------------------------------------------------------------
;*		(c) Copyright 2002, ISHIDA CO., LTD.
;*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
;*		(077) 553-4141
;*----------------------------------------------------------------------*;
;* Function								*;
;*----------------------------------------------------------------------*;
;
;  Interrupt handler Routine
;
INTEVT:	.equ	H'FFFFFFD8		; 割り込み事象レジスタ

	.IMPORT _hard_int_tbl
	.SECTION INT,CODE,ALIGN=4
__int:
;	STC.L	SSR,@-R15 ;save SSR
;	STC.L	SPC,@-R15 ;save SPC
	stc.l	r0_bank,@-r15		; R0_BANK0 退避
	mov.l	r0,@-r15		; R0_BANK1 退避
	mov.l	r1,@-r15		; R1_BANK1 退避
	mov.l	r2,@-r15		; R2_BANK1 退避
	mov.l	r3,@-r15		; R3_BANK1 退避
	stc.l	gbr,@-r15		;push GBR

	mov	#INTEVT,r0
	mov.l	@r0,r1			; R1_BANK1 = INTEVTレジスタの例外コード;
	tst	r1,r1			; if (例外コード == 0)
	bt/s	vect610_rte		;    割込みリターンへ
	nop

	mov.l	intevt2,r1		;set INTEVT2
	mov.l	@r1,r1			;
	shlr2	r1			;make address from INTEVT2
	shlr	r1			;

	MOV.L	#H'0,R0
	LDC	R0,GBR
	MOV.L	#H'78,R0
	CMP/HI	R0,R1
	BT	VECT_CK
	MOV.L	IREQ_OUT,R0
	AND.B	#H'EF,@(R0,GBR)
VECT_CK:

	mov.l	VECTadr02,r0		;get start address of jump table
	add	r1,r0			;get jump table address
	mov	r0,r3			;copy
	shlr2	r1			;make address of priority table address
	mova	tbl,r0			;
	add	r1,r0			;
	mov.b	@r0,r0			;get priority
	mov	r0,r2			;copy
	mov	r0,r1			;copy
	shlr	r2			;check bit0
	bt	set_sr			;

	and	#h'08,r0		;make IPR address
	mov	r0,r2			;
	mova	ipr_add,r0		;
	mov.l	@(r0,r2),r2		;
	mov	r1,r0			;
	and	#h'0f,r0		;
	add	r2,r0			;
	mov.w	@r0,r0			;IPRn -> r0
	mov	#-4,r2			;
	shad	r2,r1			;get shift number
	shld	r1,r0			;shift priority
set_sr:	
	and	#h'f0,r0		;
	mov.l	SRbase,r1		;
	or	r0,r1			;

;	ldc	r1,ssr			;set SSR
;	mova	ret_hnd,r0		;
;	ldc	r0,spc			;set SPC

	MOV.L	IREQ_OUT,R0
	OR.B	#H'10,@(R0,GBR)

	ldc.l	@r15+,gbr		;
	mov	r3,r0			;copy
	mov.l	@r15+,r3		; R3_BANK1 復元
	mov.l	@r15+,r2		; R2_BANK1 復元
	ldc	r1,sr			;set SR
					; ここで、RB=0 に切り替わる
	stc	r0_bank,r0		; R0_BANK0 = R0_BANK1
	ldc.l	@r15+,r1_bank		; R1_BANK1 復元
	ldc.l	@r15+,r0_bank		; R0_BANK1 復元

	mov.l	@r0,r0			; R0_BANK0 = _hard_int_tbl で登録の inthdr アドレス
	jmp	@r0			; inthdr へ
	mov.l	@r15+,r0		; R0_BANK0 復元

vect610_rte:				; 例外コード0の場合
	ldc.l	@r15+,gbr		;
	mov.l	@r15+,r3		; R3_BANK1 復元
	mov.l	@r15+,r2		; R2_BANK1 復元
	mov.l	@r15+,r1		; R1_BANK1 復元
	mov.l	@r15+,r0		; R0_BANK1 復元
	ldc.l	@r15+,r0_bank		; R0_BANK0 復元
;	LDC.L	@R15+,SPC
;	LDC.L	@R15+,SSR
	rte				; 割込みリターン
	nop

ret_hnd:
;	LDC.L	@R15+,SPC
;	LDC.L	@R15+,SSR
	rte				;
	nop				;

	.align	4
ipr_add:
		.data.l	h'fffffee2
		.data.l	0
		.data.l	h'a400000e
intevt2:	.data.l	h'a4000000
VECTadr02:	.DATA.L	_hard_int_tbl
SRbase:		.DATA.L	H'50000000
IREQ_OUT:	.DATA.L	H'A400012E

;code for priority(interrupt mask)
;  bit7 - 4	: shift or priority
;  bit3 - 0	: lower address
tbl:
	.data.b	0		;000	Power ON reset
	.data.b	0		;020	Manual reset
	.data.b	0		;040
	.data.b	0		;060
	.data.b	0		;080
	.data.b	0		;0a0
	.data.b	0		;0c0
	.data.b	0		;0e0
	.data.b	0		;100
	.data.b	0		;120
	.data.b	0		;140
	.data.b	0		;160
	.data.b	0		;180
	.data.b	0		;1a0
	.data.b	h'f1		;1c0	NMI
	.data.b	0		;1e0
	.data.b	h'f1		;200	IRL15
	.data.b	h'e1		;220	   14
	.data.b	h'd1		;240	   13
	.data.b	h'c1		;260	   12
	.data.b	h'b1		;280	   11
	.data.b	h'a1		;2a0	   10
	.data.b	h'91		;2c0	   9
	.data.b	h'81		;2e0	   8
	.data.b	h'71		;300	   7
	.data.b	h'61		;320	   6
	.data.b	h'51		;340	   5
	.data.b	h'41		;360	   4
	.data.b	h'31		;380	   3
	.data.b	h'21		;3a0	   2
	.data.b	h'11		;3c0	   1
	.data.b	0		;3e0
	.data.b	h'80		;400	tuni0
	.data.b	h'c0		;420	tuni1
	.data.b	h'00		;440	tuni2
	.data.b	h'00		;460	ticpi2
	.data.b	h'40		;480	ati
	.data.b	h'40		;4a0	pri
	.data.b	h'40		;4c0	cui
	.data.b	h'02		;4e0	eri
	.data.b	h'02		;500	rxi
	.data.b	h'02		;520	txi
	.data.b	h'02		;540	tei
	.data.b	h'82		;560	iti
	.data.b	h'c2		;580	rmci
	.data.b	h'c2		;5a0	rovi
	.data.b	0		;5c0
	.data.b	h'f1		;5e0	H-UDI
	.data.b	h'48		;600	IRQ0
	.data.b	h'08		;620	   1
	.data.b	h'c8		;640	   2
	.data.b	h'88		;660	   3
	.data.b	h'4a		;680	   4
	.data.b	h'0a		;6a0	   5
	.data.b	0		;6c0
	.data.b	0		;6e0
	.data.b	h'8a		;700	pint0-7
	.data.b	h'ca		;720	pint8-15
	.data.b	0		;740
	.data.b	0		;760
	.data.b	0		;780
	.data.b	0		;7a0
	.data.b	0		;7c0
	.data.b	0		;7e0
	.data.b	h'8c		;800	dei0
	.data.b	h'8c		;820	   1
	.data.b	h'8c		;840	   2
	.data.b	h'8c		;860	   3
	.data.b	h'cc		;880	eri1
	.data.b	h'cc		;8a0	rxi1
	.data.b	h'cc		;8c0	bri1
	.data.b	h'cc		;8e0	txi1
	.data.b	h'0c		;900	eri2
	.data.b	h'0c		;920	rxi2
	.data.b	h'0c		;940	bri2
	.data.b	h'0c		;960	txi2
	.data.b	h'4c		;980	adi

	.END
