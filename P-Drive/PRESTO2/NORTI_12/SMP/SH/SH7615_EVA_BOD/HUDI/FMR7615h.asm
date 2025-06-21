;****************************************************************************
;* SHC�pPLL�ύX�^H-UDI�����ݒ�}�N�����s�p�R�[�h                            *
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

; �� PLL�ݒ�
; ���L����K���\�ȂP��I�����āA�R���p�C���������N
; ���Ă��������B�f�t�H���g�́A4:4:1 �ł��B
;       mov     #h'00,r1              ; I��(x1),E��(x1),P��(x1)
;       mov     #h'04,r1              ; I��(x2),E��(x1),P��(x1)
;       mov     #h'05,r1              ; I��(x2),E��(x2),P��(x1)
;       mov     #h'06,r1              ; I��(x2),E��(x2),P��(x2)
;       mov     #h'08,r1              ; I��(x4),E��(x1),P��(x1)
;       mov     #h'09,r1              ; I��(x4),E��(x2),P��(x1)
;       mov     #h'0a,r1              ; I��(x4),E��(x2),P��(x2)
        mov     #h'0c,r1              ; I��(x4),E��(x4),P��(x1)
;       mov     #h'0e,r1              ; I��(x4),E��(x4),P��(x2)

        bra     _FMR_W
        nop

;* FMR �ւ̏������ݖ��߂�4n�ɔz�u����.

        .align 4

_FMR_W:
        mov.b   r1,@r0                ; modify FMR
        mov.b   @r0,r1                ; read FMR

        nop

        .align  4

WTCSR:          .data.l  h'fffffe80
FMR:            .data.l  h'fffffe90
DUMMY_DATA:     .data.l  h'20000000
WTCSR_DATA:     .data.l  h'0000a51f   ; WT/IT=0,TME=0,��/16384

    .END
