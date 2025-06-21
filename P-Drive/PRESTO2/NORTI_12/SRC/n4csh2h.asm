;******************************************************************************
;* NORTi4/SH CPU Interface (Renesas C,SH-2 with FPU)                          *
;*                                                                            *
;*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                 *
;*  All rights reserved.                                                      *
;*                                                                            *
;*  Assemble : asmsh n4csh4h.asm -cp=sh2e                                     *
;*                                                                            *
;* 22/Feb/2001                                                                *
;* 18/Jun/2001  bt,bf���߂��֎~                                         [TA]  *
;* 17/Jul/2001  STACK�Z�N�V�����Ƀ_�~�[�f�[�^���`                     [TA]  *
;* 17/Jan/2002  Hitachi C Ver.7.0 �ɑΉ�                                [TA]  *
;*              Ver.4.05.04 �Ƃ���                                            *
;* 15/Mar/2002  Hitachi C Ver.6.0B �ɑΉ�                               [KA]  *
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

CTX_SAVE:   .equ    156
    .AIF \&HVER GE H'700
CTX_FPSCR:  .equ    56
CTX_GBR:    .equ    44
    .AELSE
CTX_FPSCR:  .equ    56
CTX_GBR:    .equ    44
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
        .data.w h'0000          ; �`���ԍ��iNORTi�j
        .data.w h'5400          ; �d�l���o�[�W�����i��ITRON�d�l��Ver4.00�j
        .data.w h'0708          ; ���i�o�[�W�����iVer.4.07.08�j
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; �b�o�t���iSH2E�j
        .data.w h'0000          ; �o���G�[�V�����L�q

;******************************************************************************
;*                                                                            *
;*                           �V�X�e���R�[���֐�                               *
;*                                                                            *
;******************************************************************************
;************************************************
;* �V�X�e��������                               *
;************************************************

        .align  2
        .export _sysini
_sysini:                                ; void sysini(void)
        mov     r15,r5                  ; nowsp  = ���݃X�^�b�N�|�C���^�l;
        sts.l   pr,@-r15

        mov     #0,r0
        or      #h'f0,r0
        ldc     r0,sr                   ; SR = 0x00f0; �S�����݃}�X�N

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

;************************************************
;* �����݃n���h���̊J�n                         *
;************************************************

        .export _v4_ent_int
        .export _v3_ent_int

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;       |________|        ----->            -44|_(FR12)_|<- pTCB[RDQ[0]]->sp
;       |________|                          -40|_(FR13)_|
;       |________|                          -36|_(FR14)_|
;       |________|                          -32|_(FR15)_|
;       |________|                          -28|__(R8)__|
;       |________|                          -24|__(R9)__| R8�`R14, FR12�`FR15 �� ret_int �őޔ�
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_|
;       |________|                          -12|__(R12)_|
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR, MACH, MACL, PR,
;       |________|                          + 4|__MACH__| FPSCR, FPUL, FR0�`FR15,
;       |________|                          + 8|__MACL__| R0�`R7 ��
;       |________|                          +12|__FPSCR_| ent_int �őҔ�
;       |________|                          +16|__FPUL__|
;       |________|                          +20|__FR11__|
;       |________|                          +24|__FR10__|
;       |________|                          +28|__FR9___|
;       |________|                          +32|__FR8___|
;       |________|                          +36|__FR7___|
;       |________|                          +40|__FR6___|
;       |________|                          +44|__FR5___|
;       |________|                          +48|__FR4___|
;       |________|                          +52|__FR3___|
;       |________|                          +56|__FR2___|
;       |________|                          +60|__FR1___|
;       |________|                          +64|__FR0___|
;       |________|                          +68|___PR___|
;       |________|                          +72|___R7___|
;       |________|                          +76|___R6___|
;       |________|                          +80|___R5___|
;       |________|                          +84|___R4___|
;       |________|                          +88|___R3___|
;       |________|                          +92|___R2___|
;       |________|                          +96|___R1___|
;    + 0|___R7___|<- �����݃n���h���őҔ�  +100|___R0___|
;    + 4|___PC___|<- �����݂ɂ��ޔ�      +104|___PC___|
;    + 8|___SR___|<-  "                    +108|___SR___|






    .AIF \&HVER GE H'700
        .align  2
_v4_ent_int:
        mov.l   r1,@-r15
        mov.l   KNL_SR,r1               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.l   r2,@-r15
        mov.w   @r1,r2
        mov.l   r3,@-r15
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov.l   @(12,r15),r1
        mov.l   r0,@(12,r15)
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        mov.l   r1,@-r15
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
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
        bra     ent_int
        sts.l   fpscr,@-r15


;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|                          -44|_(FR12)_|
;       |________|                          -40|_(FR13)_|
;       |________|                          -36|_(FR14)_|
;       |________|                          -32|_(FR15)_|
;       |________|        ----->            -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__| R8�`R14 �� ret_int �őޔ�
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_|
;       |________|                          -12|__(R12)_|
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR, MACH, MACL, PR,
;       |________|                          + 4|__MACH__| FPSCR, FPUL, FR0�`FR11,
;       |________|                          + 8|__MACL__| R0�`R7 ��
;       |________|                          +12|__FPSCR_| ent_int �őҔ�
;       |________|                          +16|__FPUL__|
;       |________|                          +20|___PR___|
;       |________|                          +24|__FR0___|
;       |________|                          +28|__FR1___|
;       |________|                          +32|__FR2___|
;       |________|                          +36|__FR3___|
;       |________|                          +40|__FR4___|
;       |________|                          +44|__FR5___|
;       |________|                          +48|__FR6___|
;       |________|                          +52|__FR7___|
;       |________|                          +56|__FR8___|
;       |________|                          +60|__FR9___|
;       |________|                          +64|__FR10__|
;       |________|                          +68|__FR11__|
;       |________|                          +72|___R0___|
;       |________|                          +76|___R1___|
;       |________|                          +80|___R2___|
;       |________|                          +84|___R3___|
;       |________|                          +88|___R4___|
;       |________|                          +92|___R5___|
;       |________|                          +96|___R6___|
;    + 0|___R7___|<- �����݃n���h���őҔ�  +100|___R7___|
;    + 4|___PC___|<- �����݂ɂ��ޔ�      +104|___PC___|
;    + 8|___SR___|<-  "                    +108|___SR___|

    .AELIF \&HVER LE 6
        .align  2
_v4_ent_int:
        add     #-(4*3),r15
        mov.l   r3,@-r15
        mov.l   KNL_SR,r3               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.l   r2,@-r15
        mov.w   @r3,r2
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov.l   r6,@(16,r15)
        mov.l   r5,@(12,r15)
        mov.l   r4,@(08,r15)
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
        sts.l   fpul,@-r15
        bra     ent_int
        sts.l   fpscr,@-r15
    .AENDI
;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;       |________|        ----->            -44|_(FR12)_|<- pTCB[RDQ[0]]->sp
;       |________|                          -40|_(FR13)_|
;       |________|                          -36|_(FR14)_|
;       |________|                          -32|_(FR15)_|
;       |________|                          -28|__(R8)__|
;       |________|                          -24|__(R9)__| R8�`R14, FR12�`FR15
;       |________|                          -20|__(R10)_| �� ret_int �őޔ�
;       |________|                          -16|__(R11)_|
;       |________|                          -12|__(R12)_|
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR, MACH, MACL,
;       |________|                          + 4|__MACH__| �� ent_int �őҔ�
;       |________|                          + 8|__MACL__|
;    + 0|__FPSCR_|<- interrupt ����         +12|__FPSCR_|
;    + 4|__FPUL__|<-     �ɂ��ޔ�         +16|__FPUL__|
;    + 8|__FR11__|<-  "                     +20|__FR11__|
;    +12|__FR10__|<-  "                     +24|__FR10__|
;    +16|__FR9___|<-  "                     +28|__FR9___|
;    +20|__FR8___|<-  "                     +32|__FR8___|
;    +24|__FR7___|<-  "                     +36|__FR7___|
;    +28|__FR6___|<-  "                     +40|__FR6___|
;    +32|__FR5___|<-  "                     +44|__FR5___|
;    +36|__FR4___|<-  "                     +48|__FR4___|
;    +40|__FR3___|<-  "                     +52|__FR3___|
;    +44|__FR2___|<-  "                     +56|__FR2___|
;    +48|__FR1___|<-  "                     +60|__FR1___|
;    +52|__FR0___|<-  "                     +64|__FR0___|
;    +56|___PR___|<-  "                     +68|___PR___|
;    +60|___R7___|<-  "                     +72|___R7___|
;    +64|___R6___|<-  "                     +76|___R6___|
;    +68|___R5___|<-  "                     +80|___R5___|
;    +72|___R4___|<-  "                     +84|___R4___|
;    +76|___R3___|<-  "                     +88|___R3___|
;    +80|___R2___|<-  "                     +92|___R2___|
;    +84|___R1___|<-  "                     +96|___R1___|
;    +88|___R0___|<-  "                    +100|___R0___|
;    +92|___PC___|<- �����݂ɂ��ޔ�      +104|___PC___|
;    +96|___SR___|<-  "                    +108|___SR___|

    .AIF \&HVER GE H'700
        .align  2
_v3_ent_int:                            ; void v3_ent_int(void)

                                        ; R0�`R7, PR �͌Ăяo�����őޔ��ς�
        mov.l   KNL_SR,r3               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.w   @r3,r2
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov     r0,r1
        mov     #88,r0
        mov.l   r1,@(r0,r15)            ; R0,R5,R6 �͍X�V���ꂽ�\����
        mov     #64,r0
        mov.l   r6,@(r0,r15)            ; ����̂ŏ����߂�
        add     #4,r0
        mov.l   r5,@(r0,r15)

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|                          -44|_(FR12)_|<- pTCB[RDQ[0]]->sp
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
;       |________|                          + 4|__MACH__| �� ent_int �őҔ�
;       |________|                          + 8|__MACL__|
;    + 0|__FPSCR_|<- interrupt ����         +12|__FPSCR_|
;    + 4|__FPUL__|<-     �ɂ��ޔ�         +16|__FPUL__|
;    + 8|___PR___|<-  "                     +20|___PR___|
;    +12|__FR0___|<-  "                     +24|__FR0___|
;    +16|__FR1___|<-  "                     +28|__FR1___|
;    +20|__FR2___|<-  "                     +32|__FR2___|
;    +24|__FR3___|<-  "                     +36|__FR3___|
;    +28|__FR4___|<-  "                     +40|__FR4___|
;    +32|__FR5___|<-  "                     +44|__FR5___|
;    +36|__FR6___|<-  "                     +48|__FR6___|
;    +40|__FR7___|<-  "                     +52|__FR7___|
;    +44|__FR8___|<-  "                     +56|__FR8___|
;    +48|__FR9___|<-  "                     +60|__FR9___|
;    +52|__FR10__|<-  "                     +64|__FR10__|
;    +56|__FR11__|<-  "                     +68|__FR11__|
;    +60|___R0___|<-  "                     +72|___R0___|
;    +64|___R1___|<-  "                     +76|___R1___|
;    +68|___R2___|<-  "                     +80|___R2___|
;    +72|___R3___|<-  "                     +84|___R3___|
;    +76|___R4___|<-  "                     +88|___R4___|
;    +80|___R5___|<-  "                     +92|___R5___|
;    +84|___R6___|<-  "                     +96|___R6___|
;    +88|___R7___|<-  "                    +100|___R7___|
;    +92|___PC___|<- �����݂ɂ��ޔ�      +104|___PC___|
;    +96|___SR___|<-  "                    +108|___SR___|

    .AELIF \&HVER LE 6
        .align  2
_v3_ent_int:                            ; void v3_ent_int(void)

                                        ; R0�`R7, PR �͌Ăяo�����őޔ��ς�
        mov.l   KNL_SR,r3               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.w   @r3,r2
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov.l   r0,@(60,r15)            ; R0,R5,R6 �͍X�V���ꂽ�\����
        mov     #80,r0
        mov.l   r5,@(r0,r15)            ; ����̂ŏ����߂�
        add     #4,r0
        mov.l   r6,@(r0,r15)
    .AENDI
ent_int:
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15

        mov.l   INEST,r1
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
        mov.l   r3,@-r15
        sts.l   pr,@-r15
        jsr     @r0
        nop
        rte
        nop

        .align  4
ent_int_6:                              ; }
        mov.l   SND_ENT_INT,r0
        mov.l   r3,@-r15
        sts.l   pr,@-r15
        jsr     @r0
        nop
        rte
        nop
    .AELSE

ent_int_6:                              ; }
        rts
        ldc     r3,sr                   ; �����݃}�X�N�߂�
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
;       |________|                          +  0|__FR12__|<- pTCB[RDQ[0]]->sp
;       |________|                          +  4|__FR13__|
;       |________|                          +  8|__FR14__|
;       |________|                          + 12|__FR15__|
;       |________|                          + 16|___R8___|
;       |________|                          + 20|___R9___| R8�`R14, FR12�`FR15
;       |________|       �c��ޔ�           + 24|___R10__| �� ret_int �őޔ�
;       |________|        ----->            + 28|___R11__|
;       |________|                          + 32|___R12__|
;       |________|                          + 36|___R13__|
;       |________|                          + 40|___R14__|
;   +  0|___GBR__|<- ent_int �܂ł�         + 44|___GBR__|
;   +  4|__MACH__|<-  "    �Ҕ�             + 48|__MACH__|
;   +  8|__MACL__|<-  "                     + 52|__MACL__|
;   + 12|__FPSCR_|<-  "                     + 56|__FPSCR_|
;   + 16|__FPUL__|<-  "                     + 60|__FPUL__|
;   + 20|__FR11__|<-  "                     + 64|__FR11__|
;   + 24|__FR10__|<-  "                     + 68|__FR10__|
;   + 28|__FR9___|<-  "                     + 72|__FR9___|
;   + 32|__FR8___|<-  "                     + 76|__FR8___|
;   + 36|__FR7___|<-  "                     + 80|__FR7___|
;   + 40|__FR6___|<-  "                     + 84|__FR6___|
;   + 44|__FR5___|<-  "                     + 88|__FR5___|
;   + 48|__FR4___|<-  "                     + 92|__FR4___|
;   + 52|__FR3___|<-  "                     + 96|__FR3___|
;   + 56|__FR2___|<-  "                     +100|__FR2___|
;   + 60|__FR1___|<-  "                     +104|__FR1___|
;   + 64|__FR0___|<-  "                     +108|__FR0___|
;   + 68|___PR___|<-  "                     +112|___PR___|
;   + 72|___R7___|<-  "                     +116|___R7___|
;   + 76|___R6___|<-  "                     +120|___R6___|
;   + 80|___R5___|<-  "                     +124|___R5___|
;   + 84|___R4___|<-  "                     +128|___R4___|
;   + 88|___R3___|<-  "                     +132|___R3___|
;   + 92|___R2___|<-  "                     +136|___R2___|
;   + 96|___R1___|<-  "                     +140|___R1___|
;   +100|___R0___|<-  "                     +144|___R0___|
;   +104|___PC___|<-  "                     +148|___PC___|
;   +108|___SR___|<-  "                     +152|___SR___|

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|                          +  0|__FR12__|<- pTCB[RDQ[0]]->sp
;       |________|                          +  4|__FR13__| GBR,MACH,MACL,
;       |________|                          +  8|__FR14__| R8�`R14, FR12�`FR15 ��
;       |________|       �c��ޔ�           + 12|__FR15__| ret_int �őޔ�
;       |________|        ----->            + 16|___R8___|
;       |________|                          + 20|___R9___|
;       |________|                          + 24|___R10__|
;       |________|                          + 28|___R11__|
;       |________|                          + 32|___R12__|
;       |________|                          + 36|___R13__|
;       |________|                          + 40|___R14__|
;   +  0|___GBR__|<- ent_int �܂ł�         + 44|___GBR__|
;   +  4|__MACH__|<-  "    �Ҕ�             + 48|__MACH__|
;   +  8|__MACL__|<-  "                     + 52|__MACL__|
;   + 12|__FPSCR_|<-  "                     + 56|__FPSCR_|
;   + 16|__FPUL__|<-  "                     + 60|__FPUL__|
;   + 20|___PR___|<-  "                     + 64|___PR___|
;   + 24|__FR0___|<-  "                     + 68|__FR0___|
;   + 28|__FR1___|<-  "                     + 72|__FR1___|
;   + 32|__FR2___|<-  "                     + 76|__FR2___|
;   + 36|__FR3___|<-  "                     + 80|__FR3___|
;   + 40|__FR4___|<-  "                     + 84|__FR4___|
;   + 44|__FR5___|<-  "                     + 88|__FR5___|
;   + 48|__FR6___|<-  "                     + 92|__FR6___|
;   + 52|__FR7___|<-  "                     + 96|__FR7___|
;   + 56|__FR8___|<-  "                     +100|__FR8___|
;   + 60|__FR9___|<-  "                     +104|__FR9___|
;   + 64|__FR10__|<-  "                     +108|__FR10__|
;   + 68|__FR11__|<-  "                     +112|__FR11__|
;   + 72|___R0___|<-  "                     +116|___R0___|
;   + 76|___R1___|<-  "                     +120|___R1___|
;   + 80|___R2___|<-  "                     +124|___R2___|
;   + 84|___R3___|<-  "                     +128|___R3___|
;   + 88|___R4___|<-  "                     +132|___R4___|
;   + 92|___R5___|<-  "                     +136|___R5___|
;   + 96|___R6___|<-  "                     +140|___R6___|
;   +100|___R7___|<-  "                     +144|___R7___|
;   +104|___PC___|<-  "                     +148|___PC___|
;   +108|___SR___|<-  "                     +152|___SR___|

        .export _v3_ret_int
_v3_ret_int:                            ; void ret_int(void)

        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        mov.w   @r2,r0
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
        bt/s    not_save_tskid          ;;
        nop
        mov.l   TPRI_MAX,r4             ;;
        mov.l   @r4,r4                  ;;
        cmp/gt  r4,r2                   ;;
        bt/s    not_save_tskid          ;;
        nop
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

        mov     #(CTX_SAVE-CTX_GBR-4),r0
        mov.l   @(r0,r15),r0            ;    if ((�����ݎ��� SR & 0xf0) == 0
        tst     #h'f0,r0
        bf/s    ret_int_4
        nop

        mov.l   SDISP,r0                ;    if (SDISP)
        mov.b   @r0,r0
        tst     r0,r0                   ;       goto L1;
        bt/s    ret_int_1
        nop
        bra     ret_int_3
        mov     r0,r1
ret_int_1:
        mov.l   DDISP,r0
        mov.b   @r0,r0
        tst     r0,r0
        bf/s    ret_int_4
        nop

        tst     r1,r1                   ;     && DELAY)
        bt/s    ret_int_4
        nop

        mov     r3,r4                   ;    {
        mov.b   r5,@r2                  ;       DELAY = 0;
ret_int_2:                              ;       for (rdq = &RDQ[1];;)
        mov.b   @r4+,r1                 ;       {  tid = *rdq++;
        tst     r1,r1                   ;          if (tid != 0) break;
        bt/s    ret_int_2               ;       }
        nop
ret_int_3:
        mov.b   r1,@-r3                 ;       RDQ[0] = tid;

        mov.l   pTCB,r0                 ;       tcb = pTCB[tid];
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r1
        mov.l   NOWPRI,r4
        mov.b   @(TCB_PRI,r1),r0
        mov.b   r0,@r4                  ;       NOWPRI = tcb->pri;

        mov.l   r14,@-r15
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

        mov.l   @(TCB_SP,r1),r15        ;       SP = tcb->sp;
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
        mov.l   @r15+,r14               ;    }
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
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
    .AIF \&HVER GE H'700
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
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
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
;       |________|                         +  0|__FR12__|<- pTCB[RDQ[0]]->sp
;       |________|                         +  4|__FR13__|
;       |________|         �ޔ�            +  8|__FR14__|
;       |________|        ----->           + 12|__FR15__|
;       |________|                         + 16|___R8___|
;       |________|                         + 20|___R9___|
;       |________|                         + 24|___R10__|
;       |________|                         + 28|___R11__|
;       |________|                         + 32|___R12__|
;       |________|                         + 36|___R13__|
;       |________|                         + 40|___R14__|
;       |________|                         + 44|___GBR__|
;       |________|                         + 48|__MACH__|
;       |________|                         + 52|__MACL__|
;       |________|                         + 56|__FPSCR_|
;       |________|                         + 60|__FPUL__|
;       |________|                         + 64|_(FR11)_| FR0�`FR11 �ޔ�s�v
;       |________|                         + 68|_(FR10)_|
;       |________|                         + 72|_(FR9)__|
;       |________|                         + 76|_(FR8)__|
;       |________|                         + 80|_(FR7)__|
;       |________|                         + 84|_(FR6)__|
;       |________|                         + 88|_(FR5)__|
;       |________|                         + 92|_(FR4)__|
;       |________|                         + 96|_(FR3)__|
;       |________|                         +100|_(FR2)__|
;       |________|                         +104|_(FR1)__|
;       |________|                         +108|_(FR0)__|
;       |________|                         +112|___PR___|
;       |________|                         +116|__(R7)__| R7 �ޔ�s�v
;       |________|                         +120|___R6___|
;       |________|                         +124|___R5___|
;       |________|                         +128|___R4___|
;       |________|                         +132|__(R3)__| R1�`R3 �ޔ�s�v
;       |________|                         +136|__(R2)__|
;       |________|                         +140|__(R1)__|
;       |________|                         +144|___R0___|
;       |________|                         +148|___PC___|
;       |________|                         +152|___SR___|

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|                         +  0|__FR12__|<- pTCB[RDQ[0]]->sp
;       |________|                         +  4|__FR13__|
;       |________|                         +  8|__FR14__|
;       |________|         �ޔ�            + 12|__FR15__|
;       |________|        ----->           + 16|___R8___|
;       |________|                         + 20|___R9___|
;       |________|                         + 24|___R10__|
;       |________|                         + 28|___R11__|
;       |________|                         + 32|___R12__|
;       |________|                         + 36|___R13__|
;       |________|                         + 40|___R14__|
;       |________|                         + 44|___GBR__|
;       |________|                         + 48|__MACH__|
;       |________|                         + 52|__MACL__|
;       |________|                         + 56|__FPSCR_|
;       |________|                         + 60|__FPUL__|
;       |________|                         + 64|___PR___|
;       |________|                         + 68|_(FR0)__| FR0�`FR11 �ޔ�s�v
;       |________|                         + 72|_(FR1)__|
;       |________|                         + 76|_(FR2)__|
;       |________|                         + 80|_(FR3)__|
;       |________|                         + 84|_(FR4)__|
;       |________|                         + 88|_(FR5)__|
;       |________|                         + 92|_(FR6)__|
;       |________|                         + 96|_(FR7)__|
;       |________|                         +100|_(FR8)__|
;       |________|                         +104|_(FR9)__|
;       |________|                         +108|_(FR10)_|
;       |________|                         +112|_(FR11)_|
;       |________|                         +116|___R0___|
;       |________|                         +120|__(R1)__| R1�`R3 �ޔ�s�v
;       |________|                         +124|__(R2)__|
;       |________|                         +128|__(R3)__|
;       |________|                         +132|___R4___|
;       |________|                         +136|___R5___|
;       |________|                         +140|___R6___|
;       |________|                         +144|__(R7)__| R7 �ޔ�s�v
;       |________|                         +148|___PC___|
;       |________|                         +152|___SR___|

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

        mov.w   @r3,r1                  ; R1 = IMASK;�idispatch_8 �Ŏg���j
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
        bt/s    dispatch_1              ;    return E_OK;
        nop

; �����݃}�X�N�����݂̂Ń��^�[��

dispatch_8:
        rts                             ; return ercd;
        ldc     r1,sr                   ; SR = IMASK;

    .AIFDEF HOOK
dispatch_81:
        mov.l   r1,@-r15
        sts.l   pr,@-r15
        mov.l   r0,@-r15
        mov.l   SND_TASK_SWITCH_I,r0
        jsr     @r0
        nop
        mov.l   @r15+,r0
        rte
        nop
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
        bt/s    dispatch_8              ;    return ercd;
        nop

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
        bf/s    dispatch_4
        nop
        mov.l   DELAY,r2                ; DELAY = 0
        mov.b   r0,@r2
        mov     r3,r0

dispatch_3:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt/s    dispatch_3              ; }
        nop
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
        bt/s    dispatch_81
    .AELSE
        bt/s    dispatch_8
    .AENDI
        nop

; �R���e�L�X�g�ޔ�

    .AIF \&HVER GE H'700
        mov.l   r1,@-r15                ; (SR = 0)
        sts.l   pr,@-r15                ; (PC)
        mov.l   r0,@-r15
        add     #-4*3,r15               ; R1�`R3 �ޔ�s�v
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        add     #-4*1,r15               ; R7 �ޔ�s�v
        sts.l   pr,@-r15
        add     #-4*12,r15              ; FR0�`R11 �ޔ�s�v
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15
        mov.l   r14,@-r15
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
    .AELSE
        mov.l   r1,@-r15                ; (SR = 0)
        sts.l   pr,@-r15                ; (PC)
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
        sts.l   fpul,@-r15
        sts.l   fpscr,@-r15
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15
        mov.l   r14,@-r15
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

    .AIF \&HVER GE H'700
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
        mov.l   @r15+,r14
        ldc.l   @r15+,gbr
        lds.l   @r15+,mach
        lds.l   @r15+,macl
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
        lds.l   @r15+,pr
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
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
        mov.l   @r15+,r14
        ldc.l   @r15+,gbr
        lds.l   @r15+,mach
        lds.l   @r15+,macl
        lds.l   @r15+,fpscr
        lds.l   @r15+,fpul
    .AIF "\&HSTK" NE "60C"
        lds.l   @r15+,pr                ; PR ����
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
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte
        nop

; INEST/IMASK/DDISP �`�F�b�N����

        .align  2
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
        mov.w   @r3,r1

; �K���f�B�X�p�b�`�isyssta, chg_ims �p�j

        .align  2
_dispatch2:
        mov.l   DELAY,r2
        mov.l   RDQ,r3
        mov     #0,r0                   ; ercd = E_OK;
        bra     dispatch_2
        mov     #0,r1                   ; (SR) = 0x0000

; �K���f�B�X�p�b�`�iext_tsk, exd_tsk �p�j

        .align  4
_dispatch3:
        mov.l   RDQ,r3
        add     #1,r3
        mov     r3,r0
dispatch_6:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt/s    dispatch_6              ; }
        nop
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

;        .align  2
;        .export _dis_ims
;_dis_ims:                               ; void dis_ims(void)
;
;        mov.l   KNL_SR,r1               ; �����݃}�X�N���J�[�l�����x���ɂ���
;        stc     sr,r0
;        mov.w   @r1,r2
;        mov.l   IMASK,r1
;        ldc     r2,sr                   ; SR = KNL_SR;
;        rts
;        mov.w   r0,@r1                  ; IMASK = �}�X�N�O�� SR;

;************************************************
;* �V�X�e���R�[���o���ł̊����݃}�X�N����       *
;************************************************

;        .align  2
;        .export _ret_ims
;_ret_ims:                               ; ER ret_ims(void)
;
;        mov.l   IMASK,r1
;        mov     #0,r0
;        mov.w   @r1,r2
;        rts                             ; return E_OK;
;        ldc     r2,sr                   ; SR = IMASK;
;
;        .align  2
;        .export _ret_ims2
;_ret_ims2:                              ; void ret_ims2(void)
;
;        mov.l   IMASK,r1
;        mov.w   @r1,r2
;        rts
;        ldc     r2,sr                   ; SR = IMASK;

;************************************************
;* �V�X�e���R�[�������ł̊����݃}�X�N�ꎞ����   *
;************************************************

;        .align  4
;        .export _ope_ims
;_ope_ims:                               ; void ope_ims(void)
;
;        mov.l   IMASK,r1
;        stc     sr,r2
;        mov.w   @r1,r0
;        ldc     r0,sr                   ; SR = IMASK;
;        nop                             ; IMASK ��荂�D��̊����݂�����
;        ldc     r2,sr                   ; SR ����
;        rts
;        mov.w   r0,@r1                  ; ���D�抄���Ŕj�󂳂�邽�� IMASK ����

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

        mov.l   H_000000F0,r1
        stc     sr,r0
        rts                             ; �ߒl = �����݋֎~�O�� SR
        ldc     r1,sr                   ; SR = all mask

;************************************************
;* �X�e�[�^�X���W�X�^�̃Z�b�g                   *
;************************************************

        .align  4
        .export _v3_vset_psw
_v3_vset_psw:                           ; void v3_vset_psw(UW psw)

        rts
        ldc     r4,sr

;******************************************************************************
;*                                                                            *
;*                        �A�h���X/�f�[�^�̒�`                               *
;*                                                                            *
;******************************************************************************

        .align  4
KNL_SR:         .data.l _KNL_SR
c_sysini:       .data.l _c_sysini
wup_tsk:        .data.l _v4_wup_tsk
RDQ:            .data.l _RDQ
IMASK:          .data.l _IMASK
INEST:          .data.l _INEST
DELAY:          .data.l _DELAY
ISP:            .data.l _ISP
NOWPRI:         .data.l _NOWPRI
intini:         .data.l _intini
STARTOF_STACK:  .data.l (STARTOF STACK)
DDISP:          .data.l _DDISP
SDISP:          .data.l _SDISP
pTCB:           .data.l __pTCB-4
CTXPTN:         .data.l _CTXPTN
CTXPTR:         .data.l _CTXPTR
WID:            .data.l _WID
PREVTSK:        .data.l _PREVTSK
TPRI_MAX:       .data.l _CFG + CFG_TPRI_MAX
H_000000F0:     .data.l H'000000F0
        .AIFDEF HOOK
INIT_HOOK:          .data.l _vinit_hook
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
        mov.l   r7,@-r15
        mov.l   r1,@-r15
        mov.l   L1_ent_int,r1
        sts     pr,r7
        jsr     @r1
        mov.l   @r15+,r1

        mov.l   r14,@-r15
        mov.l   L1_isr,r14
        tst     r14,r14
        bt/s    L1_2
        nop
L1_1:
        mov.l   @r14,r2
        jsr     @r2
        mov.l   @(4,r14),r4
        mov.l   @(8,r14),r14
        tst     r14,r14
        bf/s    L1_1
        nop
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
