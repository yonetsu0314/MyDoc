;******************************************************************************
;* NORTi4/SH CPU Interface (Renesas C,SH-3&4 with FPU)                        *
;*                                                                            *
;*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                 *
;*  All rights reserved.                                                      *
;*                                                                            *
;*  Assemble : asmsh n4csh4h.asm -cp=sh3e                                     *
;*  Assemble : asmsh n4csh4h.asm -cp=sh4                                      *
;*  Assemble : asmsh n4csh4h.asm -cp=sh4 -def=GRX=1                           *
;*                                                                            *
;* 24/Apr/2000                                                                *
;* 02/Oct/2000  �t�b�N���[�`���Ή��𐮗�                                      *
;* 01/Nov/2000  Ver.4.03.00 �J�[�l���ɑΉ��iext_tsk ras_tex�j       4.03.02   *
;* 15/Jan/2001  Hitachi C Ver.6.0 �ɑΉ�                            4.03.03   *
;* 17/Jul/2001  STACK�Z�N�V�����Ƀ_�~�[�f�[�^���`                           *
;* 21/Jan/2002  Hitachi C Ver.7.0 �ɑΉ�                                [TA]  *
;*              Ver.4.05.04 �Ƃ���                                            *
;* 27/Feb/2002  Hitachi C Ver.6.0B �ɑΉ�                               [KA]  *
;*              Ver.4.05.05 �Ƃ���                                            *
;* 04/Apr/2002  Hitachi C Ver.7.0.02 �ɑΉ�                             [KA]  *
;*              Ver.4.05.06 �Ƃ���                                            *
;* 23/Apr/2002  FR12�`FR15���W�X�^�ޔ��~�X���C��                        [KA]  *
;*              Ver.4.05.07 �Ƃ���                                            *
;* 21/Jun/2002  Hitachi C Ver.7.0.03 �Ή��m�F                           [KA]  *
;*              Ver.4.06 �J�[�l���ɑΉ�                                       *
;*              Ver.4.06.07 �Ƃ���                                            *
;* 30/Aug/2002  Hitachi C Ver.7.1 �ɑΉ��m�F                            [KA]  *
;*              Ver.4.06.07 �Ƃ���                                            *
;* 10/Jul/2003  HV6���`�������̂ăt�H���g��HSTK="60C"�ɕύX           [KA]  *
;* 10/Jul/2003  Hitachi C Ver.7.1.02 �ɑΉ��m�F                         [KA]  *
;*              Ver.4.07.07 �Ƃ���                                            *
;* 29/Jul/2003  ���l�T�X SH C Ver.6.0x SH4 �̃��W�X�^�Q�ƃ~�X���C��     [TA]  *
;*              Ver.4.07.08 �Ƃ���                                            *
;******************************************************************************

;* ���l�T�XC�R���p�C���o�[�W������`

    .AIFDEF HVER
    .AELSE
     .AIFDEF HV7
HVER: .ASSIGNA  H'704
     .AELSE
      .AIFDEF HV6
HVER:  .ASSIGNA  6
HSTK:  .ASSIGNC  "60C"
      .AELSE
       .AIFDEF HV60C
HVER:   .ASSIGNA  6
HSTK:   .ASSIGNC  "60C"
       .AELSE
        .AIFDEF HV60B
HVER:    .ASSIGNA  6
HSTK:    .ASSIGNC  "60C"
        .AELSE
         .AIFDEF HV60A
HVER:     .ASSIGNA  6
HSTK:     .ASSIGNC  "60A"
         .AELSE
HVER:     .ASSIGNA  5
HSTK:     .ASSIGNC  "51C"
         .AENDI
        .AENDI
       .AENDI
      .AENDI
     .AENDI
    .AENDI

;* �\���̃����o�[�̈ʒu

TCB_SP:         .equ   0        ; TCB �̑ޔ��X�^�b�N�|�C���^�̈ʒu
TCB_PRI:        .equ   4        ; TCB �̃^�X�N�D��x�̈ʒu
CFG_TPRI_MAX:   .equ   4*10     ;

;* �R���e�L�X�g�̈�̕␳��

    .AIF \&HVER GE H'700
     .AIFDEF FPU
      .AIFDEF GRX
CTX_SAVE:   .equ    220
CTX_FPSCR:  .equ    128
CTX_GBR:    .equ    108
      .AELSE
CTX_SAVE:   .equ    156
CTX_FPSCR:  .equ    64
CTX_GBR:    .equ    44
      .AENDI
     .AELSE
      .AIFDEF GRX
CTX_SAVE:   .equ    220
CTX_FPSCR:  .equ    124
CTX_GBR:    .equ    104
      .AELSE
CTX_SAVE:   .equ    156
CTX_FPSCR:  .equ    60
CTX_GBR:    .equ    40
      .AENDI
     .AENDI
    .AELIF \&HVER EQ 6
      .AIFDEF GRX
CTX_SAVE:   .equ    220
CTX_FPSCR:  .equ    120
CTX_GBR:    .equ    108
      .AELSE
CTX_SAVE:   .equ    156
CTX_FPSCR:  .equ    56
CTX_GBR:    .equ    44
      .AENDI
    .AELSE
      .AIFDEF GRX
CTX_SAVE:   .equ    220
CTX_FPSCR:  .equ    128
CTX_GBR:    .equ    108
      .AELSE
CTX_SAVE:   .equ    156
CTX_FPSCR:  .equ    64
CTX_GBR:    .equ    44
      .AENDI
    .AENDI

;* �O���Q��

        .import _KNL_SR         ; �J�[�l���� SR �l

        .import _c_sysini       ; �V�X�e��������
        .import _v4_wup_tsk     ; �^�X�N���N������

        .import _RDQ            ; ���f�B�L���[�w�b�_
        .import _IMASK          ; �����݃}�X�N�t���O
        .import _INEST          ; �����݃l�X�g�J�E���^�i���J�E���g�j
        .import _DELAY          ; �x���f�B�X�p�b�`�v���t���O
        .import _ISP            ; �����݃X�^�b�N�|�C���^�����l
        .import _NOWPRI         ; ���݂̃^�X�N�D��x

        .import _DDISP          ; �f�B�X�p�b�`�֎~�t���O
        .import _SDISP          ; �V�X�e���^�X�N�f�B�X�p�b�`�v���t���O
        .import _CTXPTN
        .import _CTXPTR
        .import _WID            ; 10/27/99 T.F �҂��v���I�u�W�F�N�gID
        .import _PREVTSK        ; 11/20/99 T.F ��O�� Run Task ID
        .import _CFG            ; 11/20/99
        .import __pTCB          ; TCB �ւ̃|�C���^�z��

    .AIFDEF HOOK
        .import _vinit_hook     ;
        .import _visnd_debe     ;
        .import _visnd_debne    ;
        .import _visnd_debr     ;
        .import _visnd_debs     ;
        .import _visnd_debds    ;
        .import _visnd_debns    ;
    .AENDI

        .import _isritm
        .export __kernel_interr
        .export __KERNEL_pisrposi
        .export __KERNEL_hdrlen

;* �O���Q�Ɓi���[�U��`�j

        .import _intini         ; �����ݏ�����


;************************************************
;* �o�[�W�����̒�`                             *
;************************************************
        .section STACK,DATA,ALIGN=4
        .res.w  1
        .section P,CODE,ALIGN=4
        .sdataz "NORTi(c)MiSPO"
        .align  4
        .export __NORTI_VER
__NORTI_VER:                    ; T_VER _NORTI_VER;
        .data.w h'0108          ; ���[�J�[�ԍ��i(��)�~�X�|�j
        .data.w h'0000          ; �`���ԍ��iNORTi4/SH4�j
        .data.w h'5400          ; �d�l���o�[�W�����i��ITRON�d�l��Ver4.00�j
        .data.w h'0708          ; ���i�o�[�W�����iVer.4.07.08�j
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; �b�o�t���iSH4�j
        .data.w h'0000          ; �o���G�[�V�����L�q

;******************************************************************************
;*                                                                            *
;*                           �V�X�e���R�[���֐�                               *
;*                                                                            *
;******************************************************************************
;************************************************
;* �V�X�e��������                               *
;************************************************

        .align  4
        .export _sysini
_sysini:                                ; void sysini(void)
        mov.l   H_400000F0,r1           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        ldc     r1,sr                   ; ���W�X�^�o���N0, �S�����݃}�X�N
        nop

        mov     r15,r5                  ; nowsp  = ���݃X�^�b�N�|�C���^�l;
        sts.l   pr,@-r15

        mov.l   STARTOF_STACK,r4        ; stktop = �X�^�b�N�̈�擪�A�h���X;


        mov.l   c_sysini,r3             ; c_sysini(stktop, nowsp);
        jsr     @r3
        nop

        mov.l   intini,r3               ; intini();
    .AIFDEF HOOK
        jsr     @r3
        nop

        mov.l   INIT_HOOK,r3
    .AENDI
        jmp     @r3
        lds.l   @r15+,pr

        .align  4
H_400000F0:     .data.l H'400000F0      ; MD=1,RB=0,BL=0,IMASK=1111
STARTOF_STACK:  .data.l (STARTOF STACK)
intini:         .data.l _intini
c_sysini:       .data.l _c_sysini
        .AIFDEF HOOK
INIT_HOOK:      .data.l _vinit_hook
        .AENDI

;************************************************
;* �����݃n���h���̊J�n                         *
;************************************************

        .export _v4_ent_int
        .export _v3_ent_int

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;                                           (GRX)
;       |________|                      (-104)(-108)|_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                      (-100)(-104)|_(XF1)__|
;       |________|                       (-96)(-100)|_(XF2)__|
;       |________|                       (-92) (-96)|_(XF3)__|
;       |________|                       (-88) (-92)|_(XF4)__|
;       |________|                       (-84) (-88)|_(XF5)__|
;       |________|                       (-80) (-84)|_(XF6)__|
;       |________|                       (-76) (-80)|_(XF7)__|
;       |________|                       (-72) (-76)|_(XF8)__|
;       |________|                       (-68) (-72)|_(XF9)__|
;       |________|                       (-64) (-68)|_(XF10)_|
;       |________|                       (-60) (-64)|_(XF11)_|
;       |________|                       (-56) (-60)|_(XF12)_|
;       |________|                       (-52) (-56)|_(XF13)_|
;       |________|                       (-48) (-52)|_(XF14)_|
;       |________|                       (-44) (-48)|_(XF15)_|
;       |________|                       (-40) (-44)|_(FR12)_|
;       |________|                       (-36) (-40)|_(FR13)_|
;       |________|                       (-32) (-36)|_(FR14)_|
;       |________|                       (-28) (-32)|_(FR15)_|
;       |________|        ----->          -24   -28 |__(R8)__| R8�`R14, FR12�`FR15 ��
;       |________|                        -20   -24 |__(R9)__| ret_int �őޔ�
;       |________|                        -16   -20 |__(R10)_|
;       |________|                        -12   -16 |__(R11)_|
;       |________|                        - 8   -12 |__(R12)_|
;       |________|                        - 4   - 8 |__(R13)_|
;       |________|                              - 4 |__(R14)_|
;       |________|                        + 0   + 0 |___GBR__| GBR, MACH, MACL, PR,
;       |________|                        + 4   + 4 |__MACH__| PC, SR, FPSCR, FPUL,
;       |________|                        + 8   + 8 |__MACL__| FR0�`FR11, R0�`R7 ��
;       |________|                        +12   +12 |___PC___| ent_int �őҔ�
;       |________|                        +16   +16 |___SR___|
;       |________|                        +20   +20 |__FPSCR_|
;       |________|                        +24   +24 |__FPUL__|
;       |________|                        +28   +28 |__FR11__|
;       |________|                        +32   +32 |__FR10__|
;       |________|                        +36   +36 |__FR9___|
;       |________|                        +40   +40 |__FR8___|
;       |________|                        +44   +44 |__FR7___|
;       |________|                        +48   +48 |__FR6___|
;       |________|                        +52   +52 |__FR5___|
;       |________|                        +56   +56 |__FR4___|
;       |________|                        +60   +60 |__FR3___|
;       |________|                        +64   +64 |__FR2___|
;       |________|                        +68   +68 |__FR1___|
;       |________|                        +72   +72 |__FR0___|
;       |________|                        +76   +76 |___PR___|
;       |________|                        +80       |___R14__| -fpu�I�v�V��������
;       |________|                       ++84   +80 |___R7___|
;       |________|                       ++88   +84 |___R6___|
;       |________|                       ++92   +88 |___R5___|
;       |________|                       ++96   +92 |___R4___|
;       |________|                       +100   +96 |___R3___|
;       |________|                       +104  +100 |___R2___|
;       |________|                       +108  +104 |___R1___|
;    + 0|___R0___|<- �����݃n���h���őҔ�+112  +108 |___R0___|

    .AIF \&HVER GE H'700
        .align  4
_v4_ent_int:
        mov.l   r1,@-r15
        mov.l   r2,@-r15
        mov.l   r3,@-r15
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        mov.l   r7,@-r15
    .AIFDEF FPU
    .AELSE
        mov.l   r14,@-r15
    .AENDI
        mov.l   r0,@-r15                ; sts.l   pr,@-r15
        fmov.s  fr0,@-r15
        fmov.s  fr1,@-r15
        fmov.s  fr2,@-r15
        fmov.s  fr3,@-r15
        fmov.s  fr4,@-r15
        fmov.s  fr5,@-r15
        fmov.s  fr6,@-r15
        fmov.s  fr7,@-r15
        fmov.s  fr8,@-r15
        fmov.s  fr9,@-r15
        fmov.s  fr10,@-r15
        fmov.s  fr11,@-r15
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
        stc.l   ssr,@-r15
        bra     _v3_ent_int
        stc.l   spc,@-r15

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;                                         (GRX)
;       |________|                       (-108)|_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                       (-104)|_(XF1)__|
;       |________|                       (-100)|_(XF2)__|
;       |________|                       ( -96)|_(XF3)__|
;       |________|                       ( -92)|_(XF4)__|
;       |________|                       ( -88)|_(XF5)__|
;       |________|                       ( -84)|_(XF6)__|
;       |________|                       ( -80)|_(XF7)__|
;       |________|                       ( -76)|_(XF8)__|
;       |________|                       ( -72)|_(XF9)__|
;       |________|                       ( -68)|_(XF10)_|
;       |________|                       ( -64)|_(XF11)_|
;       |________|                       ( -60)|_(XF12)_|
;       |________|                       ( -56)|_(XF13)_|
;       |________|                       ( -52)|_(XF14)_|
;       |________|                       ( -48)|_(XF15)_|
;       |________|                       ( -44)|_(FR12)_|
;       |________|                       ( -40)|_(FR13)_| R8�`R14, FR12�`FR15 ��
;       |________|                       ( -36)|_(FR14)_| ret_int �őޔ�
;       |________|                       ( -32)|_(FR15)_|
;       |________|        ----->           -28 |__(R8)__|
;       |________|                         -24 |__(R9)__|
;       |________|                         -20 |__(R10)_|
;       |________|                         -16 |__(R11)_|
;       |________|                         -12 |__(R12)_|
;       |________|                         - 8 |__(R13)_|
;       |________|                         - 4 |__(R14)_|
;       |________|                         + 0 |___GBR__| GBR, MACH, MACL, PR, PC,
;       |________|                         + 4 |__MACH__| SR, FPSCR, FPUL, FR0�`FR11,
;       |________|                         + 8 |__MACL__| R0�`R7 �� ent_int �őҔ�
;       |________|                          +12|___PC___| FPSCR : version 6.0 �ł�
;       |________|                          +16|___SR___| FPUL  :
;       |________|                          +20|__FPSCR_| SR    :
;       |________|                          +24|__FPUL__| PC    :
;       |________|                         +28 |___PR___|
;       |________|                         +32 |__FR0___|
;       |________|                         +36 |__FR1___|
;       |________|                         +40 |__FR2___|
;       |________|                         +44 |__FR3___|
;       |________|                         +48 |__FR4___|
;       |________|                         +52 |__FR5___|
;       |________|                         +56 |__FR6___|
;       |________|                         +60 |__FR7___|
;       |________|                         +64 |__FR8___|
;       |________|                         +68 |__FR9___|
;       |________|                         +72 |__FR10__|
;       |________|                         +76 |__FR11__|
;       |________|                         +80 |___R0___|
;       |________|                         +84 |___R1___|
;       |________|                         +88 |___R2___|
;       |________|                         +92 |___R3___|
;       |________|                         +96 |___R4___|
;       |________|                        +100 |___R5___|
;       |________|                        +104 |___R6___|
;    + 0|___R7___|<- �����݃n���h���őҔ� +108 |___R7___|

    .AELIF \&HVER LE 6
        .align  4
_v4_ent_int:
        mov.l   r6,@-r15
        mov.l   r5,@-r15
        mov.l   r4,@-r15
        mov.l   r3,@-r15
        mov.l   r2,@-r15
        mov.l   r1,@-r15
        mov.l   r0,@-r15
    .AIF "\&HSTK" EQ "60C"
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
    .AENDI
        fmov.s  fr11,@-r15
        fmov.s  fr10,@-r15
        fmov.s  fr9,@-r15
        fmov.s  fr8,@-r15
        fmov.s  fr7,@-r15
        fmov.s  fr6,@-r15
        fmov.s  fr5,@-r15
        fmov.s  fr4,@-r15
        fmov.s  fr3,@-r15
        fmov.s  fr2,@-r15
        fmov.s  fr1,@-r15
        fmov.s  fr0,@-r15
    .AIF "\&HSTK" NE "60C"
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
    .AENDI
    .AIF \&HVER EQ 6
        stc.l   spc,@-r15
        stc.l   ssr,@-r15
    .AENDI
        sts.l   fpul,@-r15
        bra     _v3_ent_int
        sts.l   fpscr,@-r15
    .AENDI

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;                                           (GRX)
;       |________|                      (-104)(-108)|_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                      (-100)(-104)|_(XF1)__|
;       |________|                       (-96)(-100)|_(XF2)__|
;       |________|                       (-92) (-96)|_(XF3)__|
;       |________|                       (-88) (-92)|_(XF4)__|
;       |________|                       (-84) (-88)|_(XF5)__|
;       |________|                       (-80) (-84)|_(XF6)__|
;       |________|                       (-76) (-80)|_(XF7)__|
;       |________|                       (-72) (-76)|_(XF8)__|
;       |________|                       (-68) (-72)|_(XF9)__|
;       |________|                       (-64) (-68)|_(XF10)_|
;       |________|                       (-60) (-64)|_(XF11)_|
;       |________|                       (-56) (-60)|_(XF12)_|
;       |________|                       (-52) (-56)|_(XF13)_|
;       |________|                       (-48) (-52)|_(XF14)_|
;       |________|                       (-44) (-48)|_(XF15)_|
;       |________|                       (-40) (-44)|_(FR12)_|
;       |________|                       (-36) (-40)|_(FR13)_|
;       |________|                       (-32) (-36)|_(FR14)_|
;       |________|                       (-28) (-32)|_(FR15)_|
;       |________|        ----->          -24   -28 |__(R8)__| R8�`R14, FR12�`FR15 ��
;       |________|                        -20   -24 |__(R9)__| ret_int �őޔ�
;       |________|                        -16   -20 |__(R10)_|
;       |________|                        -12   -16 |__(R11)_|
;       |________|                        - 8   -12 |__(R12)_|
;       |________|                        - 4   - 8 |__(R13)_|
;       |________|                              - 4 |__(R14)_|
;       |________|                        + 0   + 0 |___GBR__| GBR, MACH, MACL, PR,
;       |________|                        + 4   + 4 |__MACH__| PC, SR, FPSCR, FPUL,
;       |________|                        + 8   + 8 |__MACL__| FR0�`FR11, R0�`R7 ��
;       |___PC___|                        +12   +12 |___PC___| ent_int �őҔ�
;       |___SR___|                        +16   +16 |___SR___|
;       |__FPSCR_|                        +20   +20 |__FPSCR_|
;       |__FPUL__|                        +24   +24 |__FPUL__|
;       |__FR11__|                        +28   +28 |__FR11__|
;       |__FR10__|                        +32   +32 |__FR10__|
;       |__FR9___|                        +36   +36 |__FR9___|
;       |__FR8___|                        +40   +40 |__FR8___|
;       |__FR7___|                        +44   +44 |__FR7___|
;       |__FR6___|                        +48   +48 |__FR6___|
;       |__FR5___|                        +52   +52 |__FR5___|
;       |__FR4___|                        +56   +56 |__FR4___|
;       |__FR3___|                        +60   +60 |__FR3___|
;       |__FR2___|                        +64   +64 |__FR2___|
;       |__FR1___|                        +68   +68 |__FR1___|
;       |__FR0___|                        +72   +72 |__FR0___|
;       |___PR___|                        +76   +76 |___PR___|
;       |___R14__|                        +80       |___R14__| -fpu�I�v�V��������
;       |___R7___|                       ++84   +80 |___R7___|
;       |___R6___|                       ++88   +84 |___R6___|
;       |___R5___|                       ++92   +88 |___R5___|
;       |___R4___|                       ++96   +92 |___R4___|
;       |___R3___|                       +100   +96 |___R3___|
;       |___R2___|                       +104  +100 |___R2___|
;       |___R1___|                       +108  +104 |___R1___|
;    + 0|___R0___|<- �����݃n���h���őҔ�+112  +108 |___R0___|

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;                                    (GRX) (NOR)
;       |________|                    -108     |_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                    -104     |_(XF1)__|
;       |________|                    -100     |_(XF2)__|
;       |________|                     -96     |_(XF3)__|
;       |________|                     -92     |_(XF4)__|
;       |________|                     -88     |_(XF5)__|
;       |________|                     -84     |_(XF6)__|
;       |________|                     -80     |_(XF7)__|
;       |________|                     -76     |_(XF8)__|
;       |________|                     -72     |_(XF9)__|
;       |________|                     -68     |_(XF10)_|
;       |________|                     -64     |_(XF11)_|
;       |________|                     -60     |_(XF12)_|
;       |________|                     -56     |_(XF13)_|
;       |________|                     -52     |_(XF14)_|
;       |________|                     -48     |_(XF15)_|
;       |________|                          -44|_(FR12)_|
;       |________|                          -40|_(FR13)_| R8�`R14, FR12�`FR15 ��
;       |________|                          -36|_(FR14)_| ret_int �őޔ�
;       |________|                          -32|_(FR15)_|
;       |________|        ----->            -28|__(R8)__|
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_|
;       |________|                          -12|__(R12)_|
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR, MACH, MACL,
;       |________|                          + 4|__MACH__| PC, SR �� ent_int �őҔ�
;       |________|                          + 8|__MACL__|
;       |________|FPSCR <-V6 interrupt ���� +12|___PC___| FPSCR : version 6.0 �ł�
;       |________|FPUL  <-   �ɂ��ޔ�     +16|___SR___| FPUL  :
;    + 0|__FPSCR_|SR    <-V5 interrupt ���� +20|__FPSCR_| SR    :
;    + 4|__FPUL__|PC    <-   �ɂ��ޔ�     +24|__FPUL__| PC    :
;    + 8|___PR___|<-  "                     +28|___PR___|
;    +12|__FR0___|<-  "                     +32|__FR0___|
;    +16|__FR1___|<-  "                     +36|__FR1___|
;    +20|__FR2___|<-  "                     +40|__FR2___|
;    +24|__FR3___|<-  "                     +44|__FR3___|
;    +28|__FR4___|<-  "                     +48|__FR4___|
;    +32|__FR5___|<-  "                     +52|__FR5___|
;    +36|__FR6___|<-  "                     +56|__FR6___|
;    +40|__FR7___|<-  "                     +60|__FR7___|
;    +44|__FR8___|<-  "                     +64|__FR8___|
;    +48|__FR9___|<-  "                     +68|__FR9___|
;    +52|__FR10__|<-  "                     +72|__FR10__|
;    +56|__FR11__|<-  "                     +76|__FR11__|
;    +60|___R0___|<-  "                     +80|___R0___|
;    +64|___R1___|<-  "                     +84|___R1___|
;    +68|___R2___|<-  "                     +88|___R2___|
;    +72|___R3___|<-  "                     +92|___R3___|
;    +76|___R4___|<-  "                     +96|___R4___|
;    +80|___R5___|<-  "                    +100|___R5___|
;    +84|___R6___|<-  "                    +104|___R6___|
;    +88|___R7___|<-  "                    +108|___R7___|

        .align  4
_v3_ent_int:                            ; void v3_ent_int(void)
    .AIF \&HVER LT 6
        stc.l   ssr,@-r15
        stc.l   spc,@-r15
    .AENDI
        sts.l   macl,@-r15
        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        stc     sr,r4
        mov.l   SR_BL,r3
        mov.l   @r2,r0
        sts.l   mach,@-r15
        and     r3,r4
        ldc     r0,sr                   ; SR = KNL_SR;
        mov.l   INEST,r1
        stc.l   gbr,@-r15

        mov.b   @r1,r0                  ; if (--INEST == -1)
        mov.l   RDQ,r2
        add     #-1,r0                  ; {
        cmp/eq  #-1,r0
        bf/s    ent_int_6
        mov.b   r0,@r1
        mov.b   @r2,r1                  ;    pTCB[RDQ[0]]->sp = SP - 40;
        mov.l   pTCB,r0
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r2
        add     #-CTX_GBR,r15
        mov.l   ISP,r1
        mov.l   r15,@(TCB_SP,r2)
        mov.l   @r1,r15                 ;    SP = ISP �����l

    .AIFDEF HOOK
        mov.l   SND_NOT_TASK,r0
        sts.l   pr,@-r15
        jsr     @r0
        mov.l   r4,@-r15
        ldc.l   @r15+,sr
        lds.l   @r15+,pr
        rts
        nop

        .align  4
ent_int_6:                              ; }
        mov.l   SND_ENT_INT,r0
        sts.l   pr,@-r15
        jsr     @r0
        mov.l   r4,@-r15
        ldc.l   @r15+,sr
        lds.l   @r15+,pr
        rts
        nop
    .AELSE

ent_int_6:                              ; }
        ldc     r4,sr                   ; �����݃}�X�N�߂�
        rts
        nop
    .AENDI

;************************************************
;* �����ݏ������A�ƃ^�X�N�N�����s��             *
;************************************************

        .align  4
        .export _v3_ret_wup
_v3_ret_wup:                            ; void v3_ret_wup(ID tskid)

        mov.l   wup_tsk,r3              ; wup_tsk(tskid);
        sts.l   pr,@-r15
        jsr     @r3
        nop
        lds.l   @r15+,pr                ; --> �ȉ� ret_int() �Ɠ���
        nop
;************************************************
;* �����݃n���h�����畜�A����                   *
;************************************************

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;                                        (GRX)
;       |________|                        + 0       |_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                        + 4       |_(XF1)__|
;       |________|                        + 8       |_(XF2)__|
;       |________|                        +12       |_(XF3)__|
;       |________|                        +16       |_(XF4)__|
;       |________|                        +20       |_(XF5)__|
;       |________|                        +24       |_(XF6)__|
;       |________|                        +28       |_(XF7)__|
;       |________|                        +32       |_(XF8)__|
;       |________|                        +36       |_(XF9)__|
;       |________|                        +40       |_(XF10)_|
;       |________|                        +44       |_(XF11)_|
;       |________|                        +48       |_(XF12)_|
;       |________|                        +52       |_(XF13)_|
;       |________|                        +56       |_(XF14)_|
;       |________|                        +60       |_(XF15)_|
;       |________|                        +64   + 0 |_(FR12)_|
;       |________|                        +68   + 4 |_(FR13)_|
;       |________|                        +72   + 8 |_(FR14)_|
;       |________|                        +76   +12 |_(FR15)_|
;       |________|        ----->          +80   +16 |__(R8)__|
;       |________|                        +84   +20 |__(R9)__|
;       |________|                        +88   +24 |__(R10)_|
;       |________|                        +92   +28 |__(R11)_|
;       |________|                        +96   +32 |__(R12)_|
;       |________|                       +100   +36 |__(R13)_|
;       |________|                       +104   +40 |__(R14)_|
;       |________|                       +108   +44 |___GBR__|
;       |________|                       +112   +48 |__MACH__|
;       |________|                       +116   +52 |__MACL__|
;       |___PC___|                       +120   +56 |___PC___|
;       |___SR___|                       +124   +60 |___SR___|
;       |__FPSCR_|                       +128   +64 |__FPSCR_|
;       |__FPUL__|                       +132   +68 |__FPUL__|
;       |__FR11__|                       +136   +72 |__FR11__|
;       |__FR10__|                       +140   +76 |__FR10__|
;       |__FR9___|                       +144   +80 |__FR9___|
;       |__FR8___|                       +148   +84 |__FR8___|
;       |__FR7___|                       +152   +88 |__FR7___|
;       |__FR6___|                       +156   +92 |__FR6___|
;       |__FR5___|                       +160   +96 |__FR5___|
;       |__FR4___|                       +164  +100 |__FR4___|
;       |__FR3___|                       +168  +104 |__FR3___|
;       |__FR2___|                       +172  +108 |__FR2___|
;       |__FR1___|                       +176  +112 |__FR1___|
;       |__FR0___|                       +180  +116 |__FR0___|
;       |___PR___|                       +184  +120 |___PR___|
;       |___R14__|                                  |___R14__|
;       |___R7___|                       +188  +124 |___R7___|
;       |___R6___|                       +192  +128 |___R6___|
;       |___R5___|                       +196  +132 |___R5___|
;       |___R4___|                       +200  +136 |___R4___|
;       |___R3___|                       +204  +140 |___R3___|
;       |___R2___|                       +208  +144 |___R2___|
;       |___R1___|                       +212  +148 |___R1___|
;       |___R0___|                       +216  +152 |___R0___|
;                                        +220  +156

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;                                     (GRX) (NOR)
;       |________|                     +  0     |__XF0___|<- pTCB[RDQ[0]]->sp
;       |________|                     +  4     |__XF1___|
;       |________|                     +  8     |__XF2___|
;       |________|                     + 12     |__XF3___|
;       |________|                     + 16     |__XF4___|
;       |________|                     + 20     |__XF5___|
;       |________|                     + 24     |__XF6___|
;       |________|                     + 28     |__XF7___|
;       |________|                     + 32     |__XF8___|
;       |________|                     + 36     |__XF9___|
;       |________|                     + 40     |__XF10__|
;       |________|                     + 44     |__XF11__|
;       |________|                     + 48     |__XF12__|
;       |________|                     + 52     |__XF13__|
;       |________|                     + 56     |__XF14__|
;       |________|                     + 60     |__XF15__|
;       |________|                     + 64 +  0|__FR12__| GBR,MACH,MACL,
;       |________|                     + 68 +  4|__FR13__| R8�`R14, FR12�`FR15 ��
;       |________|                     + 72 +  8|__FR14__| ret_int �őޔ�
;       |________|       �c��ޔ�      + 76 + 12|__FR15__|
;       |________|        ----->       + 80 + 16|___R8___|
;       |________|                     + 84 + 20|___R9___|
;       |________|                     + 88 + 24|___R10__|
;       |________|                     + 92 + 28|___R11__|
;       |________|                     + 96 + 32|___R12__|
;       |________|                     +100 + 36|___R13__|
;       |________|                     +104 + 40|___R14__|
;   +  0|___GBR__|<- ent_int �܂ł�    +108 + 44|___GBR__|
;   +  4|__MACH__|<-  "    �Ҕ�        +112 + 48|__MACH__|
;   +  8|__MACL__|<-  "                +116 + 52|__MACL__|
;   + 12|___PC___|<-  "                +120 + 56|___PC___| FPSCR : version 6.0
;   + 16|___SR___|<-  "                +124 + 60|___SR___| FPUL  :
;   + 20|__FPSCR_|<-  "                +128 + 64|__FPSCR_| SR    :
;   + 24|__FPUL__|<-  "                +132 + 68|__FPUL__| PC    :
;   + 28|___PR___|<-  "                +136 + 72|___PR___|
;   + 32|__FR0___|<-  "                +140 + 76|__FR0___|
;   + 36|__FR1___|<-  "                +144 + 80|__FR1___|
;   + 40|__FR2___|<-  "                +148 + 84|__FR2___|
;   + 44|__FR3___|<-  "                +152 + 88|__FR3___|
;   + 48|__FR4___|<-  "                +156 + 92|__FR4___|
;   + 52|__FR5___|<-  "                +160 + 96|__FR5___|
;   + 56|__FR6___|<-  "                +164 +100|__FR6___|
;   + 60|__FR7___|<-  "                +168 +104|__FR7___|
;   + 64|__FR8___|<-  "                +172 +108|__FR8___|
;   + 68|__FR9___|<-  "                +176 +112|__FR9___|
;   + 72|__FR10__|<-  "                +180 +116|__FR10__|
;   + 76|__FR11__|<-  "                +184 +120|__FR11__|
;   + 80|___R0___|<-  "                +188 +124|___R0___|
;   + 84|___R1___|<-  "                +192 +128|___R1___|
;   + 88|___R2___|<-  "                +196 +132|___R2___|
;   + 92|___R3___|<-  "                +200 +136|___R3___|
;   + 96|___R4___|<-  "                +204 +140|___R4___|
;   +100|___R5___|<-  "                +208 +144|___R5___|
;   +104|___R6___|<-  "                +212 +148|___R6___|
;   +108|___R7___|<-  "                +216 +152|___R7___|

        .export _v3_ret_int
_v3_ret_int:                            ; void ret_int(void)

        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.l   @r2,r0
        ldc     r0,sr                   ; SR = KNL_SR;

    .AIFDEF HOOK
        mov.l   SND_RET_INT,r0
        jsr     @r0
        nop
    .AENDI
        mov.l   INEST,r1
        mov.l   RDQ,r3

        mov.b   @r1,r5                  ; if (++INEST == 0)
        mov     #1,r2
        clrt
        addc    r2,r5                   ; �iR5 �� DELAY = 0 �Ŏg���j
        bf/s    ret_int_41              ; {
        mov.b   r5,@r1

        mov.b   @r3+,r1                 ; �iR3 �� rdq = &RDQ[1] �Ŏg���j

        mov.l   NOWPRI,r2               ; added by T.F 11/20/99
        mov.b   @r2,r2                  ;;
        tst     r2,r2                   ;;
        bt      not_save_tskid          ;;
        mov.l   TPRI_MAX,r4             ;;
        mov.l   @r4,r4                  ;;
        cmp/gt  r4,r2                   ;;
        bt      not_save_tskid          ;;
        mov.l   PREVTSK,r0              ;;
        mov.b   r1,@r0                  ;; save previous task ID

not_save_tskid:
        mov.l   pTCB,r0
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r1
        mov.l   DELAY,r2
        mov.l   @(TCB_SP,r1),r15        ;    SP = RDQ[0].head->sp;
        mov.b   @r2,r1
        add     #CTX_GBR,r15            ;    SP += xx;

    .AIF \&HVER EQ 6
        mov.l   @(4*5,r15),r0           ;    if ((�����ݎ��� SR & 0xf0) == 0
    .AELSE
        mov.l   @(4*4,r15),r0           ;    if ((�����ݎ��� SR & 0xf0) == 0
    .AENDI
        tst     #h'f0,r0
        bf      ret_int_4

        mov.l   SDISP,r0                ;    if (SDISP)
        mov.b   @r0,r0
        tst     r0,r0                   ;       goto L1;
        bt      ret_int_1
        bra     ret_int_3
        mov     r0,r1
ret_int_1:
        mov.l   DDISP,r0
        mov.b   @r0,r0
        tst     r0,r0
        bf      ret_int_4

        tst     r1,r1                   ;     && DELAY)
        bt      ret_int_4

        mov     r3,r4                   ;    {
        mov.b   r5,@r2                  ;       DELAY = 0;
ret_int_2:                              ;       for (rdq = &RDQ[1];;)
        mov.b   @r4+,r1                 ;       {  tid = *rdq++;
        tst     r1,r1                   ;          if (tid != 0) break;
        bt      ret_int_2               ;       }
ret_int_3:
        mov.b   r1,@-r3                 ;       RDQ[0] = tid;

        mov.l   pTCB,r0                 ;       tcb = pTCB[tid];
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r1
        mov.l   NOWPRI,r4
        mov.b   @(TCB_PRI,r1),r0
        mov.b   r0,@r4                  ;       NOWPRI = tcb->pri;

    .AIFDEF GRX
    .AIF \&HVER EQ 6
        mov.l   @(4*3,r15),r0
    .AELSE
        mov.l   @(4*5,r15),r0
    .AENDI
        lds     r0,fpscr
    .AENDI
    .AIF \&HVER GE H'700
      .AIFDEF FPU
        mov.l   r14,@-r15
      .AENDI
    .AELSE
        mov.l   r14,@-r15
    .AENDI
        mov.l   r13,@-r15               ;       GBR,MACH,MACL,R8�`R14 �ޔ�
        mov.l   r12,@-r15
        mov.l   r11,@-r15
        mov.l   r10,@-r15
        mov.l   r9,@-r15
        mov.l   r8,@-r15
        fmov.s  fr15,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr12,@-r15
    .AIFDEF GRX
        frchg
        fmov.s  fr15,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr12,@-r15
        fmov.s  fr11,@-r15
        fmov.s  fr10,@-r15
        fmov.s  fr9,@-r15
        fmov.s  fr8,@-r15
        fmov.s  fr7,@-r15
        fmov.s  fr6,@-r15
        fmov.s  fr5,@-r15
        fmov.s  fr4,@-r15
        fmov.s  fr3,@-r15
        fmov.s  fr2,@-r15
        fmov.s  fr1,@-r15
        fmov.s  fr0,@-r15
    .AENDI

        mov.l   @(TCB_SP,r1),r15        ;       SP = tcb->sp;
    .AIFDEF GRX
        mov     #CTX_FPSCR/4,r0
        shll2   r0
        mov.l   @(r0,r15),r1
        lds     r1,fpscr
        frchg
        fmov.s  @r15+,fr0
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr11
        fmov.s  @r15+,fr12
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr15
        frchg
    .AENDI
        fmov.s  @r15+,fr12
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr15
        mov.l   @r15+,r8
        mov.l   @r15+,r9
        mov.l   @r15+,r10
        mov.l   @r15+,r11
        mov.l   @r15+,r12
        mov.l   @r15+,r13
    .AIF \&HVER GE H'700
    .AIFDEF FPU
        mov.l   @r15+,r14               ;    }
    .AENDI
    .AELSE
        mov.l   @r15+,r14               ;    }
    .AENDI
ret_int_4:                              ;
    .AIFDEF HOOK
        mov.l   SND_TASK_SWITCH,r0
        jsr     @r0
        nop
    .AENDI
ret_int_41:                             ; }
        ldc.l   @r15+,gbr               ;       GBR,MACH,MACL,R8�`R14 ����
        lds.l   @r15+,mach
        lds.l   @r15+,macl
    .AIF \&HVER GE H'700
        mov.l   @r15+,r2                ; r2 = PC
        mov.l   @r15+,r1                ; r1 = SR
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
        fmov.s  @r15+,fr11
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr0
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AIFDEF FPU
    .AELSE
        mov.l   @r15+,r14
    .AENDI
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   H_500000F0,r0           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        ldc     r0,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r2,spc                  ; SPC, SSR ����
        ldc     r1,ssr
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
    .AIF \&HVER EQ 6
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
        mov.l   @r15+,r6                ; r6 = SR
        mov.l   @r15+,r5                ; r5 = PC
    .AELSE
        mov.l   @r15+,r5                ; r5 = PC
        mov.l   @r15+,r6                ; r6 = SR
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
    .AENDI
    .AIF "\&HSTK" NE "60C"
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AENDI
        fmov.s  @r15+,fr0
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr11
    .AIF "\&HSTK" EQ "60C"
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AENDI
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        mov.l   @r15+,r3
        mov.l   @r15+,r4

        mov.l   H_500000F0,r7
        ldc     r7,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r5,spc                  ; SPC, SSR ����
        ldc     r6,ssr
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte                             ; PC, SR ����
        nop


;******************************************************************************
;*                                                                            *
;*                            �J�[�l�������֐�                                *
;*                                                                            *
;******************************************************************************
;************************************************
;* �f�B�X�p�b�`                                 *
;************************************************

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;                                        (GRX)
;       |________|                        + 0       |_(XF0)__|<- pTCB[RDQ[0]]->sp
;       |________|                        + 4       |_(XF1)__|
;       |________|                        + 8       |_(XF2)__|
;       |________|                        +12       |_(XF3)__|
;       |________|                        +16       |_(XF4)__|
;       |________|                        +20       |_(XF5)__|
;       |________|                        +24       |_(XF6)__|
;       |________|                        +28       |_(XF7)__|
;       |________|                        +32       |_(XF8)__|
;       |________|                        +36       |_(XF9)__|
;       |________|                        +40       |_(XF10)_|
;       |________|                        +44       |_(XF11)_|
;       |________|                        +48       |_(XF12)_|
;       |________|                        +52       |_(XF13)_|
;       |________|                        +56       |_(XF14)_|
;       |________|                        +60       |_(XF15)_|
;       |________|                        +64   + 0 |_(FR12)_|
;       |________|                        +68   + 4 |_(FR13)_|
;       |________|                        +72   + 8 |_(FR14)_|
;       |________|                        +76   +12 |_(FR15)_|
;       |________|        ----->          +80   +16 |__(R8)__|
;       |________|                        +84   +20 |__(R9)__|
;       |________|                        +88   +24 |__(R10)_|
;       |________|                        +92   +28 |__(R11)_|
;       |________|                        +96   +32 |__(R12)_|
;       |________|                       +100   +36 |__(R13)_|
;       |________|                       +104   +40 |__(R14)_|
;       |________|                       +108   +44 |___GBR__|
;       |________|                       +112   +48 |__MACH__|
;       |________|                       +116   +52 |__MACL__|
;       |___PC___|                       +120   +56 |___PC___|
;       |___SR___|                       +124   +60 |___SR___|
;       |__FPSCR_|                       +128   +64 |__FPSCR_|
;       |__FPUL__|                       +132   +68 |__FPUL__|
;       |__FR11__|                       +136   +72 |__FR11__|
;       |__FR10__|                       +140   +76 |__FR10__|
;       |__FR9___|                       +144   +80 |__FR9___|
;       |__FR8___|                       +148   +84 |__FR8___|
;       |__FR7___|                       +152   +88 |__FR7___|
;       |__FR6___|                       +156   +92 |__FR6___|
;       |__FR5___|                       +160   +96 |__FR5___|
;       |__FR4___|                       +164  +100 |__FR4___|
;       |__FR3___|                       +168  +104 |__FR3___|
;       |__FR2___|                       +172  +108 |__FR2___|
;       |__FR1___|                       +176  +112 |__FR1___|
;       |__FR0___|                       +180  +116 |__FR0___|
;       |___PR___|                       +184  +120 |___PR___|
;       |___R14__|                                  |___R14__|
;       |___R7___|                       +188  +124 |___R7___|
;       |___R6___|                       +192  +128 |___R6___|
;       |___R5___|                       +196  +132 |___R5___|
;       |___R4___|                       +200  +136 |___R4___|
;       |___R3___|                       +204  +140 |___R3___|
;       |___R2___|                       +208  +144 |___R2___|
;       |___R1___|                       +212  +148 |___R1___|
;       |___R0___|                       +216  +152 |___R0___|
;                                        +220  +156

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;                                    (GRX) (NOR)
;       |________|                    +  0     |__XF0___|<- pTCB[RDQ[0]]->sp
;       |________|                    +  4     |__XF1___|
;       |________|                    +  8     |__XF2___|
;       |________|                    + 12     |__XF3___|
;       |________|                    + 16     |__XF4___|
;       |________|                    + 20     |__XF5___|
;       |________|                    + 24     |__XF6___|
;       |________|                    + 28     |__XF7___|
;       |________|                    + 32     |__XF8___|
;       |________|                    + 36     |__XF9___|
;       |________|                    + 40     |__XF10__|
;       |________|                    + 44     |__XF11__|
;       |________|                    + 48     |__XF12__|
;       |________|                    + 52     |__XF13__|
;       |________|                    + 56     |__XF14__|
;       |________|                    + 60     |__XF15__|
;       |________|                    + 64 +  0|__FR12__|
;       |________|                    + 68 +  4|__FR13__|
;       |________|                    + 72 +  8|__FR14__|
;       |________|         �ޔ�       + 76 + 12|__FR15__|
;       |________|        ----->      + 80 + 16|___R8___|
;       |________|                    + 84 + 20|___R9___|
;       |________|                    + 88 + 24|___R10__|
;       |________|                    + 92 + 28|___R11__|
;       |________|                    + 96 + 32|___R12__|
;       |________|                    +100 + 36|___R13__|
;       |________|                    +104 + 40|___R14__|
;       |________|                    +108 + 44|___GBR__|
;       |________|                    +112 + 48|__MACH__|
;       |________|                    +116 + 52|__MACL__|
;       |________|                    +120 + 56|___PC___| FPSCR : new version
;       |________|                    +124 + 60|___SR___| FPUL  :
;       |________|                    +128 + 64|__FPSCR_| SR    :
;       |________|                    +132 + 68|__FPUL__| PC    :
;       |________|                    +136 + 72|___PR___|
;       |________|                    +140 + 76|_(FR0)__| FR0�`FR11 �ޔ�s�v
;       |________|                    +144 + 80|_(FR1)__|
;       |________|                    +148 + 84|_(FR2)__|
;       |________|                    +152 + 88|_(FR3)__|
;       |________|                    +156 + 92|_(FR4)__|
;       |________|                    +160 + 96|_(FR5)__|
;       |________|                    +164 +100|_(FR6)__|
;       |________|                    +168 +104|_(FR7)__|
;       |________|                    +172 +108|_(FR8)__|
;       |________|                    +176 +112|_(FR9)__|
;       |________|                    +180 +116|_(FR10)_|
;       |________|                    +184 +120|_(FR11)_|
;       |________|                    +188 +124|___R0___|
;       |________|                    +192 +128|__(R1)__| R1�`R3 �ޔ�s�v
;       |________|                    +196 +132|__(R2)__|
;       |________|                    +200 +136|__(R3)__|
;       |________|                    +204 +140|___R4___|
;       |________|                    +208 +144|___R5___|
;       |________|                    +212 +148|___R6___|
;       |________|                    +216 +152|__(R7)__| R7 �ޔ�s�v

        .align  4
        .export _dispatch
        .export _dispatch1
        .export _dispatch2
        .export _dispatch3
_dispatch:                              ; ER dispatch(void)

; �x���f�B�X�p�b�`�v��

        mov.l   DELAY,r2
        mov     #1,r0
        mov.l   IMASK,r3
        mov.b   r0,@r2                  ; DELAY = 1;

; �����݋֎~���܂��͊����݃n���h�����Ȃ�
; �����݃}�X�N�߂��ďI��

        mov.l   @r3,r1                  ; R1 = IMASK;�idispatch_8 �Ŏg���j
        mov.l   INEST,r3
        mov     r1,r0
        tst     #h'f0,r0
        mov.b   @r3,r5                  ; if (IMASK & 0x00f0)
        bf/s    dispatch_8              ;    return E_OK;
        mov     #0,r0                   ; ercd = E_OK;
        mov.l   DDISP,r3
        tst     r5,r5                   ; if (INEST != 0)
        bf/s    dispatch_8              ;    return E_OK;
        mov.b   @r3,r5
        tst     r5,r5                   ; if (DDISP != 0)
        bt      dispatch_1              ;    return E_OK;

; �����݃}�X�N�����݂̂Ń��^�[��

dispatch_8:
        ldc     r1,sr                   ; SR = IMASK;
        rts                             ; return ercd;
        nop

    .AIFDEF HOOK
dispatch_81:
        mov.l   r0,@-r15
        mov.l   r1,@-r15
        sts.l   pr,@-r15
        mov.l   SND_TASK_SWITCH_I,r0
        jsr     @r0
        nop
        lds.l   @r15+,pr
        ldc.l   @r15+,sr
        rts
        mov.l   @r15+,r0
    .AENDI

; ��U���荞�݋���

dispatch_1:
        stc     sr,r7                   ; old = SR;
        ldc     r1,sr                   ; SR MD=1,RB=0,IMASK=0000
        nop
        nop
        ldc     r7,sr                   ; SR = old; (KNL_SR)

; �x���f�B�X�p�b�`�v����������Ă���ΏI��
; �����݋����� R0(ercd) �������������Ă��鎞������

        mov.b   @r2,r7
        mov.l   RDQ,r3
        tst     r7,r7                   ; if (DELAY == 0)
        bt      dispatch_8              ;    return ercd;

; ���s�^�X�N�I��

dispatch_2:
        mov.l   r0,@-r15
        mov.l   r1,@-r15

        mov.b   @r3+,r2                 ; mytcb = pTCB[RDQ[0]];
        mov.l   pTCB,r0
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r7

        mov.l   SDISP,r1
        mov     #0,r0
        mov.b   @r1,r2
        tst     r2,r2
        bf      dispatch_4
        mov.l   DELAY,r2                ; DELAY = 0
        mov.b   r0,@r2
        mov     r3,r0

dispatch_3:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt      dispatch_3              ; }
dispatch_4:
        mov.b   r2,@-r3                 ; RDQ[0] = tid;

        mov.l   pTCB,r0                 ; tcb = pTCB[tid];
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r2

        mov.l   NOWPRI,r3
        mov.b   @(TCB_PRI,r2),r0
        mov.b   r0,@r3                  ; NOWPRI = tcb->pri;

        mov.l   @r15+,r1
        mov.l   @r15+,r0
        cmp/eq  r2,r7
    .AIFDEF HOOK
        bt      dispatch_81
    .AELSE
        bt      dispatch_8
    .AENDI

; �R���e�L�X�g�ޔ�

    .AIF \&HVER GE H'700
        mov.l   r0,@-r15
        add     #-4*3,r15               ; R1�`R3 �ޔ�s�v
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        add     #-4*1,r15               ; R7 �ޔ�s�v
    .AIFDEF FPU
    .AELSE
        mov.l   r14,@-r15
    .AENDI
        sts.l   pr,@-r15
        add     #-4*12,r15              ; FR0�`R11 �ޔ�s�v
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
        mov.l   r1,@-r15                ; (SR = 0x40000000)
        sts.l   pr,@-r15                ; (PC)
    .AELSE
        add     #-4*1,r15               ; R7 �ޔ�s�v
        mov.l   r6,@-r15
        mov.l   r5,@-r15
        mov.l   r4,@-r15
        add     #-4*3,r15               ; R1�`R3 �ޔ�s�v
        mov.l   r0,@-r15
    .AIF "\&HSTK" EQ "60C"
        sts.l   pr,@-r15
    .AENDI
        add     #-4*12,r15              ; FR0�`R11 �ޔ�s�v
    .AIF "\&HSTK" NE "60C"
        sts.l   pr,@-r15
    .AENDI
    .AIF \&HVER EQ 6
        sts.l   pr,@-r15                ; (PC)
        mov.l   r1,@-r15                ; (SR)
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
    .AELSE
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
        mov.l   r1,@-r15                ; (SR)
        sts.l   pr,@-r15                ; (PC)
    .AENDI
    .AENDI
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15
    .AIF \&HVER GE H'700
    .AIFDEF FPU
        mov.l   r14,@-r15
    .AENDI
    .AELSE
        mov.l   r14,@-r15
    .AENDI
        mov.l   r13,@-r15
        mov.l   r12,@-r15
        mov.l   r11,@-r15
        mov.l   r10,@-r15
        mov.l   r9,@-r15
        mov.l   r8,@-r15
        fmov.s  fr15,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr12,@-r15
    .AIFDEF GRX
        frchg
        fmov.s  fr15,@-r15
        fmov.s  fr14,@-r15
        fmov.s  fr13,@-r15
        fmov.s  fr12,@-r15
        fmov.s  fr11,@-r15
        fmov.s  fr10,@-r15
        fmov.s  fr9,@-r15
        fmov.s  fr8,@-r15
        fmov.s  fr7,@-r15
        fmov.s  fr6,@-r15
        fmov.s  fr5,@-r15
        fmov.s  fr4,@-r15
        fmov.s  fr3,@-r15
        fmov.s  fr2,@-r15
        fmov.s  fr1,@-r15
        fmov.s  fr0,@-r15
    .AENDI
        mov.l   r15,@(TCB_SP,r7)        ; mytcb->sp = SP;

; �R���e�L�X�g����
dispatch_5:
        mov.l   @(TCB_SP,r2),r15        ; SP = tcb->sp;

    .AIFDEF HOOK
        mov.l   SND_TASK_SWITCH_D,r0
        jsr     @r0
        nop
    .AENDI

    .AIFDEF GRX
        mov     #CTX_FPSCR/4,r0
        shll2   r0
        mov.l   @(r0,r15),r1
        lds     r1,fpscr
        frchg
        fmov.s  @r15+,fr0
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr11
        fmov.s  @r15+,fr12
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr15
        frchg
    .AENDI
        fmov.s  @r15+,fr12
        fmov.s  @r15+,fr13
        fmov.s  @r15+,fr14
        fmov.s  @r15+,fr15
        mov.l   @r15+,r8
        mov.l   @r15+,r9
        mov.l   @r15+,r10
        mov.l   @r15+,r11
        mov.l   @r15+,r12
        mov.l   @r15+,r13
    .AIF \&HVER GE H'700
    .AIFDEF FPU
        mov.l   @r15+,r14
    .AENDI
    .AELSE
        mov.l   @r15+,r14
    .AENDI
        ldc.l   @r15+,gbr
        lds.l   @r15+,mach
        lds.l   @r15+,macl
    .AIF \&HVER GE H'700
        mov.l   @r15+,r2                ; r2 = PC
        mov.l   @r15+,r1                ; r1 = SR
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
        fmov.s  @r15+,fr11
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr0
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AIFDEF FPU
    .AELSE
        mov.l   @r15+,r14
    .AENDI
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   H_500000F0,r0           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        ldc     r0,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r2,spc                  ; SPC, SSR ����
        ldc     r1,ssr
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
    .AIF \&HVER EQ 6
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
        mov.l   @r15+,r6
        mov.l   @r15+,r5
    .AELSE
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
    .AENDI
    .AIF "\&HSTK" NE "60C"
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AENDI
        fmov.s  @r15+,fr0
        fmov.s  @r15+,fr1
        fmov.s  @r15+,fr2
        fmov.s  @r15+,fr3
        fmov.s  @r15+,fr4
        fmov.s  @r15+,fr5
        fmov.s  @r15+,fr6
        fmov.s  @r15+,fr7
        fmov.s  @r15+,fr8
        fmov.s  @r15+,fr9
        fmov.s  @r15+,fr10
        fmov.s  @r15+,fr11
    .AIF "\&HSTK" EQ "60C"
        lds.l   @r15+,pr                ; R0�`R7, PR ����
    .AENDI
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        mov.l   @r15+,r3
        mov.l   @r15+,r4
        mov.l   H_500000F0,r7
        ldc     r7,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r5,spc
        ldc     r6,ssr
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte
        nop

; INEST/IMASK/DDISP �`�F�b�N����

        .align  4
_dispatch1:
        mov.l   CTXPTN,r0
        mov.l   CTXPTR,r1
        mov.l   DELAY,r2
        mov.l   IMASK,r3
        mov.l   WID,r7
        mov.l   @r1,r6
        mov.l   @r0,r5
        mov     #1,r1
        mov     #0,r0                   ; ercd = E_OK;
        mov.b   r1,@r2                  ; DELAY = 1;
        mov.l   @r7,r4
        bra     dispatch_1
        mov.l   @r3,r1

; �K���f�B�X�p�b�`�isyssta, chg_ims �p�j

        .align  4
_dispatch2:
        mov.l   H_40000000,r1
        mov.l   DELAY,r2
        mov.l   RDQ,r3
        bra     dispatch_2
        mov     #0,r0                   ; ercd = E_OK;

; �K���f�B�X�p�b�`�iext_tsk, exd_tsk �p�j

        .align  4
_dispatch3:
        mov.l   RDQ,r3
        add     #1,r3
        mov     r3,r0
dispatch_6:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt      dispatch_6              ; }
        mov.b   r2,@-r3                 ; RDQ[0] = tid;

        mov.l   pTCB,r0                 ; tcb = pTCB[tid];
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r2

        mov.l   NOWPRI,r3
        mov.b   @(TCB_PRI,r2),r0
        bra     dispatch_5
        mov.b   r0,@r3                  ; NOWPRI = tcb->pri;

;************************************************
;* �V�X�e���R�[�������ł̊����݃}�X�N           *
;************************************************

        .align  4
        .export _dis_ims
_dis_ims:                               ; void dis_ims(void)

        mov.l   KNL_SR,r1               ; �����݃}�X�N���J�[�l�����x���ɂ���
        stc     sr,r0
        mov.l   @r1,r2
        mov.l   IMASK,r1
        ldc     r2,sr                   ; SR = KNL_SR;
        rts
        mov.l   r0,@r1                  ; IMASK = �}�X�N�O�� SR;

;************************************************
;* �V�X�e���R�[���o���ł̊����݃}�X�N����       *
;************************************************

        .align  4
        .export _ret_ims
_ret_ims:                               ; ER ret_ims(void)

        mov.l   IMASK,r1
        mov.l   @r1,r2
        ldc     r2,sr                   ; SR = IMASK;
        rts                             ; return E_OK;
        mov     #0,r0

        .align  4
        .export _ret_ims2
_ret_ims2:                              ; void ret_ims2(void)

        mov.l   IMASK,r1
        mov.l   @r1,r2
        ldc     r2,sr                   ; SR = IMASK;
        rts
        nop

;************************************************
;* �V�X�e���R�[�������ł̊����݃}�X�N�ꎞ����   *
;************************************************

        .align  4
        .export _ope_ims
_ope_ims:                               ; void ope_ims(void)

        mov.l   IMASK,r1
        stc     sr,r2
        mov.l   @r1,r0
        ldc     r0,sr                   ; SR = IMASK;
        nop                             ; IMASK ��荂�D��̊����݂�����
        ldc     r2,sr                   ; SR ����
        rts
        mov.l   r0,@r1                  ; ���D�抄���Ŕj�󂳂�邽�� IMASK ����

;******************************************************************************
;*                                                                            *
;*                         �Ǝ��V�X�e���R�[���֐�                             *
;*                                                                            *
;******************************************************************************
;********************************************************
;* �X�e�[�^�X���W�X�^�̊����݋֎~�iBL�r�b�g�j�Z�b�g     *
;********************************************************

        .align  4
        .export _v3_vdis_psw
_v3_vdis_psw:                           ; UW v3_vdis_psw(void)

        mov.l   H_500000F0,r1
        stc     sr,r0
        ldc     r1,sr                   ; SR MD=1,RB=0,BL=1,IMASK=15;
        rts                             ; �ߒl = �����݋֎~�O�� SR
        nop

;************************************************
;* �X�e�[�^�X���W�X�^�̃Z�b�g                   *
;************************************************

        .align  4
        .export _v3_vset_psw
_v3_vset_psw:                           ; void v3_vset_psw(UW psw)

        ldc     r4,sr
        rts
        nop

;******************************************************************************
;*                                                                            *
;*                        �A�h���X/�f�[�^�̒�`                               *
;*                                                                            *
;******************************************************************************

        .align  4
KNL_SR:         .data.l _KNL_SR
SR_BL:          .data.l H'EFFFFFFF
wup_tsk:        .data.l _v4_wup_tsk
RDQ:            .data.l _RDQ
IMASK:          .data.l _IMASK
INEST:          .data.l _INEST
DELAY:          .data.l _DELAY
ISP:            .data.l _ISP
NOWPRI:         .data.l _NOWPRI
DDISP:          .data.l _DDISP
SDISP:          .data.l _SDISP
pTCB:           .data.l __pTCB-4
CTXPTN:         .data.l _CTXPTN
CTXPTR:         .data.l _CTXPTR
H_40000000:     .data.l H'40000000      ; MD=1,RB=0,BL=0,IMASK=0000
H_500000F0:     .data.l H'500000F0      ; MD=1,RB=0,BL=1,IMASK=1111
WID:            .data.l _WID
PREVTSK:        .data.l _PREVTSK
TPRI_MAX:       .data.l _CFG + CFG_TPRI_MAX
        .AIFDEF HOOK
SND_ENT_INT:        .data.l _visnd_debe
SND_NOT_TASK:       .data.l _visnd_debne
SND_RET_INT:        .data.l _visnd_debr
SND_TASK_SWITCH:    .data.l _visnd_debs
SND_TASK_SWITCH_D:  .data.l _visnd_debds
SND_TASK_SWITCH_I:  .data.l _visnd_debns
        .AENDI

;************************************************
;* �N���񐔃L���[�C���O���� ext_tsk �ōď�����  *
;************************************************

        .align  2
        .import _c_ext_tsk
        .export _a_ext_tsk          ; void a_ext_tsk(T_CTX *ctx,
                                    ;                VP_INT stacd,
                                    ;                T_TCB NEAR *tcb,
                                    ;                ID tskid)
_a_ext_tsk:
        mov.l   c_ext_tsk,r3
        jmp     @r3
        mov     r4,r15              ; �R���e�L�X�g�ۑ�

        .align  4
c_ext_tsk:      .data.l _c_ext_tsk

;**********************************************************
;* �^�X��O�n���h���̑O�����ɃG���[�R�[�h�������Ƃ��ēn�� *
;**********************************************************

        .align  2
        .import _c_ext_exp
        .export _a_ext_exp

_a_ext_exp:
        mov.l   c_ext_exp,r3
        jmp     @r3
        mov     r0,r4               ; �R���e�L�X�g�ۑ�

        .align  4
c_ext_exp:      .data.l _c_ext_exp

;******************************************************************************
;*                                                                            *
;*                        �����݃T�[�r�X�����̒�`                            *
;*                                                                            *
;******************************************************************************

        .align  4

__kernel_interr:
    .AIF \&HVER GE H'700
        mov.l   r0,@-r15
        mov.l   r1,@-r15
        mov.l   L1_ent_int,r1
        sts     pr,r0
    .AELSE
        mov.l   r7,@-r15
        mov.l   r1,@-r15
        mov.l   L1_ent_int,r1
        sts     pr,r7
    .AENDI
        jsr     @r1
        mov.l   @r15+,r1

        mov.l   r14,@-r15
        mov.l   L1_isr,r14
        tst     r14,r14
        bt      L1_2
L1_1:
        mov.l   @r14,r2
        jsr     @r2
        mov.l   @(4,r14),r4
        mov.l   @(8,r14),r14
        tst     r14,r14
        bf      L1_1
L1_2:
        mov.l   L1_ret_int,r2
        jsr     @r2
        mov.l   @r15+,r14

        .align  4
L1_isr:
        .data.l 0
L1_ent_int:
        .data.l _v4_ent_int
L1_ret_int:
        .data.l _v3_ret_int
__kernel_interr_end:

__KERNEL_pisrposi:
        .data.b L1_isr - __kernel_interr
__KERNEL_hdrlen:
        .data.b __kernel_interr_end - __kernel_interr

        .end
