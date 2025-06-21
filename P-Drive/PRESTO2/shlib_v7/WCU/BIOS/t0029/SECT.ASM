;*----------------------------------------------------------------------*;
;* File name	: sect.asm	/ 					*;
;*----------------------------------------------------------------------*;
;*$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/sect.asm 1.1 1970/01/01 00:00:00 chimura Exp $
;*$Log: sect.asm $
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
	.SECTION B,DATA,ALIGN=4
	.SECTION R,DATA,ALIGN=4
	.SECTION D,DATA,ALIGN=4
; If other section are existed , Insert here “.SECTION XXX”,
	.SECTION C,DATA,ALIGN=4
__B_BGN: .DATA.L (STARTOF B)
__B_END: .DATA.L (STARTOF B)+(SIZEOF B)
__D_BGN: .DATA.L (STARTOF R)
__D_END: .DATA.L (STARTOF R)+(SIZEOF R)
__D_ROM: .DATA.L (STARTOF D)
	.EXPORT __B_BGN
	.EXPORT __B_END
	.EXPORT __D_BGN
	.EXPORT __D_END
	.EXPORT __D_ROM
	.END
