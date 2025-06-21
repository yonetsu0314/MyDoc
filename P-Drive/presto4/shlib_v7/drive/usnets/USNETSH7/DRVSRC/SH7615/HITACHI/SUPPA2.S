;
; $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/hitachi/RCS/suppa2.s 1.1 1970/01/01 00:00:00 masahiro Exp $
; $Log: suppa2.s $
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
;    SUPPA2.S -- assembly support code for HITACHI SH
;    Copyright (C) 1994,1997 United States Software Corporation

        .SECTION    P,CODE,ALIGN=4

;        .export        _Ndisable                        
;_Ndisable:
;        stc     sr,r0                
;        or      #h'f0,r0
;        or      #h'd0,r0
;        ldc     r0,sr
;        rts
;        nop

;        .export        _Nenable                        
;_Nenable:
;        stc     sr,r0                
;        mov.l   #h'ffffff0f,r1
;        and     r1,r0
;        ldc     r0,sr
;        rts
;        nop

        .export        _Nchksum
_Nchksum:                        ; function: Nchksum
        mov     #0,r0            ; Zero out r0
        mov     #1,r1            ; Use 1 to decrement r5
        mov.l   chmask,r6        ; Place FFFF in r6
chk3:   mov.w   @r4+,r7          ; Move word into r7
        and     r6,r7            ; Mask off lower 16bits
        add     r7,r0            ; Add to storage  
        sub     r1,r5            ; Decrement r5
        tst     r5,r5            ; Is count 0?
        bf      chk3             ; Branch if not 0
        mov     r0,r1            ; Place sum in r1 
        shll16  r1               ; Shift to upper two bytes
        clrt                     ; Clear T bit
        addc    r1,r0            ; Add sum and shifted sum
        shlr16  r0               ; Shift result right two bytes
        mov     #0,r7            ; Zero out r7
        addc    r7,r0            
        and    r6,r0             ; Return checksum
        rts
        nop

        .align        4
chmask:        .data.l        H'0000ffff

; replacement of memcpy()
; void Nmemcpy(char *dest, char *src, int len);
; 注意：リターン値は使用していないので返していない
;
; Arguments
; dest - R4
; src  - R5
; len  - R6
;
        .export  _Nmemcpy
_Nmemcpy:
        tst    r6,r6
        bt     memcend
        add    #H'ff,r6

        mov.b  @r5+,r0
        mov.b  r0,@r4
        bra    _Nmemcpy
        add    #1,r4
memcend:
        rts
        nop

;-----------------------------------------------------------
; ハーフワード単位のブロック入力
; void Ninhrep(unsigned short *buffer,unsigned long port,int len);
; R4 - ptr
; R5 - port
; R6 - len
        .export        _Ninhrep
_Ninhrep:              ; function: Ninhrep
        mov.l  #1,r1         ; len減算用の定数1
        tst    r1,r6         ; バイト数が奇数の場合の補正
        bt     inh01
        add    r1,r6
inh01:
        shlr   r6            ; バイト単位のlenをハーフワード単位に換算
inhlop:
        tst    r6,r6         ; len==0なら終了
        bt     inhend
        sub    r1,r6         ; len--

        mov.w  @(r5),r0      ; portから1ワード読み、
        mov.w  r0,@(r4)      ; バッファに書き込む

        bra    inhlop
        add    #2,r4         ; バッファポインタを進める
inhend:

        rts
        nop

;-----------------------------------------------------------
; ハーフワード単位のブロック出力
; void Nouthrep(unsigned short *buffer,unsigned long port,int len);
; R4 - ptr
; R5 - port
; R6 - len
        .export        _Nouthrep
_Nouthrep:              ; function: Nouthrep
        mov.l  #1,r1         ; len減算用の定数1
        tst    r1,r6         ; バイト数が奇数の場合の補正
        bt     outh01
        add    r1,r6
outh01:
        shlr   r6            ; バイト単位のlenをハーフワード単位に換算
outhlop:
        tst    r6,r6         ; len==0なら終了
        bt     outhend
        sub    r1,r6         ; len--

        mov.w  @(r4),r0      ; バッファから1ワード読み、
        mov.w  r0,@(r5)      ; portに書き込む

        bra    outhlop
        add    #2,r4         ; バッファポインタを進める
outhend:
        rts
        nop

        .end
