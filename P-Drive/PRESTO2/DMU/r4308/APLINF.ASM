;	name	aplinf
;*======================================================================
;* File name    : aplinf.asm
;* Original	: 
;*----------------------------------------------------------------------
;* Function 	:
;*	BIOSにAPLの割り込みハンドラの情報を提供する為のROMテーブル。
;*	このモジュールはAPLのトップでリンクされる必要があり、テーブ
;*	ルの構造は以下の構造体にマッチしなければならない。
;*----------------------------------------------------------------------
;*$Header: p:/wcu/main/nz_wcu/rcs/aplinf.asm 1.1.6308.1.63.1 1970/01/01 00:00:00 chimura Exp $
;*$Log: aplinf.asm $
; リビジョン 1.1.6308.1.63.1  1970/01/01  00:00:00  chimura
; 2005/02/14
; セクションCVECTを追加（BIOSをAPPEND）
; 
; リビジョン 1.1.6308.1  1970/01/01  00:00:00  syuji
; 2002/12/23 14:40
; ＳＨＣコンパイラー使用によるバグフィックス。
; 	関数プロトタイプの不一致エラーを修正。
; 	ベクタテーブル変更。
; 
; リビジョン 1.1  1996/06/18  18:49:19  chimura
; 初期リビジョン
; 
;*----------------------------------------------------------------------
;*Header: p:/wcu/main/rz_wcu/rcs/aplinf.asm 2.1 1993/10/20 11:19:46 ryoji
;*Log: aplinf.asm
; リビジョン 2.1  1993/10/20  11:19:46  ryoji
;*----------------------------------------------------------------------
;*
;*
;*----------------------------------------------------------------------
;*		(c) Copyright 1993, ISHIDA CO., LTD.
;*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
;*		SHIGA JAPAN
;*		(0775) 53-4141
;*======================================================================

;extrn	__startup	:far	; MS-C startup module
;extrn	brdata		:far	; interrupt vector table
;extrn	_vector_strc	:far	; interrupt vector table


;*--------------------------------------------------------------*;
;*	ベクター･テーブルへのアドレス･テーブル			*;
;*								*;
;*--------------------------------------------------------------*;

;apl_info	struc
;	start_up	dd	?	; ｽﾀｰﾄ･ｱｯﾌﾟへのfarﾎﾟｲﾝﾀ
;	vec_struc	dw	?	; ﾍﾞｸﾀｰ･ﾃｰﾌﾞﾙ構造体へのfarﾎﾟｲﾝﾀ
;			dw	?	;	"
;apl_info	ends
;
;
;_tbl		segment	para public 'TBL'
;
;		public	_vector_tbl
;_vector_tbl	apl_info	<__startup, offset _vector_strc, seg brdata>
;
;_tbl		ends
;
;
;		end

	.global	_vector_tbl
	.global	_startup
	.global	_vector_strc

	.section CVECT,CODE,ALIGN=4

_vector_tbl:
	.DATA.L	_startup
	.DATA.L	_vector_strc
	.DATA.L	_vector_strc + 4

	.section P,CODE,ALIGN=4

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

