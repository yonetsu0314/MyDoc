;*======================================================================
;* File name    : sect.asm
;* Original		: p:/dacs_i/rcu/main/rcs/sect.asm 1.2
;*----------------------------------------------------------------------
;* Function 	:
;*		FMRの初期設定とSECTION定義
;*----------------------------------------------------------------------
;*$Header: p:/presto2/dmu_v7/bootload/rcs/sect.asm 2.1 1970/01/01 00:00:00 kagatume Exp $
;*$Log: sect.asm $
; リビジョン 2.1  1970/01/01  00:00:00  kagatume
; 2004/03/09 17:00
; ブートローダーの書き換えに対応できるようにする
; 
; リビジョン 1.1  1970/01/01  00:00:00  sakaguti
; ＤＭＵ用ブートローダ
; 
;*----------------------------------------------------------------------
;*			(c) Copyright 1993, ISHIDA CO., LTD.
;*			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
;*			SHIGA JAPAN
;*			(0775) 53-4141
;*======================================================================
;
;*--------------------------------------------------------------*;
;*	ベクター･テーブルへのアドレス･テーブル			*;
;*								*;
;*--------------------------------------------------------------*;

	.global	_vector_tbl
	.global	_INIT
	.global	_vector_strc

	.AIFDEF BTWRITER
        .section VECT,CODE,ALIGN=4
	.AELSE
        .section P,CODE,ALIGN=4
	.AENDI
	
_vector_tbl:
	.DATA.L	_INIT
	.DATA.L	_vector_strc
	.DATA.L	_vector_strc + 4

	.AIFDEF BTWRITER
        .section P,CODE,ALIGN=4
	.AENDI

; ROM 先頭 +-------------------+
;          |割込みベクタ(CVECT)| --- リセットベクタのみ(BIOS)
;          |-------------------|
;          |   プログラム領域  |
;          |        (P)        |
;          |-------------------|
;          |      定数領域     |
;          |        (C)        |
;          |-------------------|
;          |  初期化データ領域 | ---+
;          |        (D)        |    |
;          |-------------------|    | INITSCT() でコピー
;         ～                   ～   |
; RAM 先頭 |-------------------|    |
;          |  割込みベクタ領域 | ← | ---- ここは空けてリンク
;          |-------------------|    |
;          |  初期化データ領域 | ←-+
;          |        (R)        |
;          |-------------------|
;          | 未初期化データ領域|
;          |        (B)        |
;          |-------------------|
;          |    スタック領域   |
;          |      (STACK)      |
;

PACR	.equ	h'fffffc80
XRAM	.equ	h'1000e000
FMR		.equ	h'fffffe90
WTCSR	.equ 	h'fffffe80


;	.SECTION FMRA,CODE,ALIGN=4
;	JTAG-ICE用にSECTIONを設ける場合は上記コメントを外すこと！
;
	.export _init_FMR

_init_FMR:
	mov.l	#XRAM,r1
	mov.l	r1,r5
	mov.l	#FREQUENCY,r2
	mov.l	#FREQUENCY_END,r3

program_move:
	mov.w	@r2,r0
	mov.w	r0,@r1
	add	#2,r1
	add	#2,r2			; ＸＲＡＭにプログラムをコピー
	cmp/eq	r2,r3
	bf	program_move
	nop

	mov.l	#PACR,r1
	mov.w	#h'0008,r0
	mov.w	r0,@r1

	MOV.L	#WTCSR,R1
	MOV.L	#H'A51F,R2
	MOV.L	#H'26200000,R3
	MOV.L	#FMR,R4

	jmp	@r5
	nop
	nop
	nop
	nop
	nop
clock4_err:
	bra	clock4_err
	nop
	nop
	nop
	nop
;
;	周波数変更プログラムの本体
;	これをXRAMへコピーしてからXRAM上で実行する
FREQUENCY:
	;<ｳｫｯﾁﾄﾞｯｸﾞﾀｲﾏｺﾝﾄﾛｰﾙ/ｽﾃｰﾀｽﾚｼﾞｽﾀ設定>
	;TMEﾋﾞｯﾄｸﾘｱ
	;WTCNTに入力するｸﾛｯｸ=φ/16384
	;(ｵｰﾊﾞｰﾌﾛｰ周期=278.528ms)
		MOV.W	R2,@R1

	;<外部ｷｬｯｼｭｽﾙｰ領域ﾘｰﾄﾞ>
	;外部ﾒﾓﾘ空間３のｷｬｯｼｭｽﾙｰ領域：H'26200000
		MOV.L	@R3,R0

	;PLL回路1&2 → 動作する
	;Iφ(×4)=60MHz、Eφ(×4)=60MHz、
	;Pφ(×2)=30MHz、CKIO(Eφ)=60MHz、
		MOV	#H'0e,R0
	;
	;	5V-EPROMへの対応暫定策（FLASH-ROMであれば#H'0e可能）
	;PLL回路1&2 → 動作する
	;Iφ(×4)=60MHz、Eφ(×2)=30MHz、
	;Pφ(×2)=30MHz、CKIO(Eφ)=60MHz、
;		MOV	#H'0a,R0

	MOV.B	R0,@R4
	rts
	nop
FREQUENCY_END:
	NOP


;* 各セクションのアドレス

	.SECTION D,DATA,ALIGN=4     ; 初期化データ領域（ROM）
	.SECTION R,DATA,ALIGN=4     ; 初期化データ領域（RAM）
	.SECTION B,DATA,ALIGN=4     ; 未初期化データ領域（RAM）
	.SECTION STACK,DATA,ALIGN=4 ; スタック領域（RAM）
	.SECTION C,DATA,ALIGN=4     ; 定数領域（ROM）
__P_BGN .DATA.L (STARTOF P)
__P_END .DATA.L (STARTOF P)+(SIZEOF P)
__C_BGN .DATA.L (STARTOF C)
__C_END .DATA.L (STARTOF C)+(SIZEOF C)
__D_ROM .DATA.L (STARTOF D)
__D_BGN .DATA.L (STARTOF R)
__D_END .DATA.L (STARTOF R)+(SIZEOF R)
__B_BGN .DATA.L (STARTOF B)
__B_END .DATA.L (STARTOF B)+(SIZEOF B)
	.EXPORT __P_BGN
	.EXPORT __P_END
	.EXPORT __C_BGN
	.EXPORT __C_END
	.EXPORT __D_ROM
	.EXPORT __D_BGN
	.EXPORT __D_END
	.EXPORT __B_BGN
	.EXPORT __B_END

    .END
