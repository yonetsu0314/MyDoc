;****************************************************************************
;* SHC用PLL変更／H-UDI初期設定マクロ実行用コード                            *
;*                                                                          *
;*  Copyright (c) 2001 MiSPO Co., Ltd.                                      *
;*  All rights reserved.                                                    *
;*                                                                          *
;* 25/Apr/2001 Release                                                      *
;****************************************************************************

        .section P,CODE,ALIGN=4
_FMR_START:
        mov.l   WTCSR,r0
        mov.l   WTCSR_DATA,r1
        mov.w   r1,@r0                ; set WTCSR

        mov.l   DUMMY_DATA,r0
        mov.l   @r0,r0                ; read cache through(dummy read)

        mov.l   FMR,r0

; ★ PLL設定
; 下記から適応可能な１つを選択して、コンパイル＆リンク
; してください。デフォルトは、4:4:1 です。
;       mov     #h'00,r1              ; IΦ(x1),EΦ(x1),PΦ(x1)
;       mov     #h'04,r1              ; IΦ(x2),EΦ(x1),PΦ(x1)
;       mov     #h'05,r1              ; IΦ(x2),EΦ(x2),PΦ(x1)
;       mov     #h'06,r1              ; IΦ(x2),EΦ(x2),PΦ(x2)
;       mov     #h'08,r1              ; IΦ(x4),EΦ(x1),PΦ(x1)
;       mov     #h'09,r1              ; IΦ(x4),EΦ(x2),PΦ(x1)
;       mov     #h'0a,r1              ; IΦ(x4),EΦ(x2),PΦ(x2)
        mov     #h'0c,r1              ; IΦ(x4),EΦ(x4),PΦ(x1)
;       mov     #h'0e,r1              ; IΦ(x4),EΦ(x4),PΦ(x2)

        bra     _FMR_W
        nop

;* FMR への書きこみ命令を4nに配置する.

        .align 4

_FMR_W:
        mov.b   r1,@r0                ; modify FMR
        mov.b   @r0,r1                ; read FMR

        nop

        .align  4

WTCSR:          .data.l  h'fffffe80
FMR:            .data.l  h'fffffe90
DUMMY_DATA:     .data.l  h'20000000
WTCSR_DATA:     .data.l  h'0000a51f   ; WT/IT=0,TME=0,φ/16384

    .END
