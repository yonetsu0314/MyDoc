;******************************************************************************
;* NORTi4/SH CPU Interface (Renesas C,SH-3)                                   *
;*                                                                            *
;*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                 *
;*  All rights reserved.                                                      *
;*                                                                            *
;*  Assemble : asmsh n3csh3h.asm -cp=sh3                                      *
;*  Assemble : asmsh n3csh3h.asm -cp=sh3 -def=HOOK=1  (include Hook-routines) *
;*                                                                            *
;* 24/Apr/2000                                                                *
;* 02/Oct/2000  �t�b�N���[�`���Ή��𐮗�                                      *
;* 01/Nov/2000  Ver.4.03.00 �J�[�l���ɑΉ��iext_tsk ras_tex�j                 *
;* 15/Jan/2001  Hitachi C Ver.6.0 �ɑΉ�                                      *
;* 17/Jul/2001  STACK�Z�N�V�����Ƀ_�~�[�f�[�^���`                           *
;* 07/Nov/2001  SR���W�X�^�̕ۑ��r�b�g��ݒ�                                  *
;* 16/Jan/2002  Hitachi C Ver.7.0 �ɑΉ�                                      *
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
      .AELSE
       .AIFDEF HV60C
HVER:   .ASSIGNA  6
       .AELSE
        .AIFDEF HV60B
HVER:    .ASSIGNA  6
        .AELSE
         .AIFDEF HV60A
HVER:     .ASSIGNA  6
         .AELSE
HVER:     .ASSIGNA  5
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
        .data.w h'0000          ; �`���ԍ��iNORTi/SH�j
        .data.w h'5400          ; �d�l���o�[�W�����i��ITRON�d�l��Ver4.00�j
        .data.w h'0708          ; ���i�o�[�W�����iVer.4.07.08�j
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; ���i�Ǘ����
        .data.w h'0000          ; �b�o�t���iSH3�j
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
        stc     sr,r0
        mov.l   SR_MASK,r1
        mov.l   H_400000F0,r2           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        and     r0,r1
        or      r2,r1
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

;************************************************
;* �����݃n���h���̊J�n                         *
;************************************************

        .export _v4_ent_int
        .export _v3_ent_int

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8�`R14 ��
;       |________|                          -12|__(R12)_| ret_int �őޔ�
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| R0�`R7,PR
;       |________|                          + 4|__MACH__| GBR,MACH,MACL ��
;       |________|                          + 8|__MACL__| ent_int �őҔ�
;       |________|                          +12|___PC___|
;       |________|                          +16|___SR___|
;       |________|                          +20|___PR___|
;       |________|                          +24|___R7___|
;       |________|                          +28|___R6___|
;       |________|                          +32|___R5___|
;       |________|                          +36|___R4___|
;       |________|                          +40|___R3___|
;       |________|                          +44|___R2___|
;       |________|                          +48|___R1___|
;    + 0|___R0___|<- �����݃n���h���őҔ�   +52|___R0___|

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
        mov.l   r0,@-r15                ; sts.l   pr,@-r15
        stc.l   ssr,@-r15
        bra     _v3_ent_int
        stc.l   spc,@-r15

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8�`R14 ��
;       |________|                          -12|__(R12)_| ret_int �őޔ�
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| R0�`R7,PR
;       |________|                          + 4|__MACH__| GBR,MACH,MACL ��
;       |________|                          + 8|__MACL__| ent_int �őҔ�
;       |________|                          +12|___PC___|
;       |________|                          +16|___SR___|
;       |________|                          +20|___PR___|
;       |________|                          +24|___R0___|
;       |________|                          +28|___R1___|
;       |________|                          +32|___R2___|
;       |________|                          +36|___R3___|
;       |________|                          +40|___R4___|
;       |________|                          +44|___R5___|
;       |________|                          +48|___R6___|
;    + 0|___R7___|<- �����݃n���h���őҔ�   +52|___R7___|

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
    .AIF \&HVER EQ 6
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
        stc.l   spc,@-r15
        bra     _v3_ent_int
        stc.l   ssr,@-r15
    .AELSE
        bra     _v3_ent_int
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
    .AENDI
    .AENDI

;       ���l�T�XC Ver.7.0 �ȍ~
;                        T_CTX �\����
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8�`R14 ��
;       |________|                          -12|__(R12)_| ret_int �őޔ�
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| PC,SR,
;       |________|                          + 4|__MACH__| GBR,MACH,MACL ��
;       |________|                          + 8|__MACL__| ent_int �őҔ�
;    + 0|___PC___|<- interrupt ����         +12|___PC___|
;    + 4|___SR___|<-  "                     +16|___SR___|
;    + 8|___PR___|<-  "                     +20|___PR___|
;    +12|___R7___|<-  "                     +24|___R7___|
;    +16|___R6___|<-  "                     +28|___R6___|
;    +20|___R5___|<-  "                     +32|___R5___|
;    +24|___R4___|<-  "                     +36|___R4___|
;    +28|___R3___|<-  "                     +40|___R3___|
;    +32|___R2___|<-  "                     +44|___R2___|
;    +36|___R1___|<-  "                     +48|___R1___|
;    +40|___R0___|<-  "                     +52|___R0___|

;       ���l�T�XC Ver.6.0x�ȑO
;                        T_CTX �\����
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8�`R14 ��
;       |________|                          -12|__(R12)_| ret_int �őޔ�
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| PC,SR,
;       |________|                          + 4|__MACH__| GBR,MACH,MACL ��
;       |________|                          + 8|__MACL__| ent_int �őҔ�
;       |__(SR)__|<- interrupt ����         +12|___PC___| new version ��
;       |__(PC)__|<-  new version �̂�      +16|___SR___| pc, sr ���t
;    + 0|___PR___|<- interrupt ����         +20|___PR___|
;    + 4|___R0___|<-  "                     +24|___R0___|
;    + 8|___R1___|<-  "                     +28|___R1___|
;    +12|___R2___|<-  "                     +32|___R2___|
;    +16|___R3___|<-  "                     +36|___R3___|
;    +20|___R4___|<-  "                     +40|___R4___|
;    +24|___R5___|<-  "                     +44|___R5___|
;    +28|___R6___|<-  "                     +48|___R6___|
;    +32|___R7___|<-  "                     +52|___R7___|

        .align  4
_v3_ent_int:                            ; void v3_ent_int(void)
    .AIF \&HVER LT 6
        stc.l   ssr,@-r15
        stc.l   spc,@-r15
    .AENDI
        sts.l   macl,@-r15
        mov.l   SR_MASK,r1
        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        stc     sr,r4
        mov.l   SR_BL,r3
        and     r4,r1
        mov.l   @r2,r0
        sts.l   mach,@-r15
        or      r1,r0
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
        mov.b   @r2,r1                  ;    pTCB[RDQ[0]]->head->sp = SP - 40;
        mov.l   pTCB,r0
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r2
        add     #-28,r15
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
;                              T_CTX �\����
;       |________|       �c��Ҕ�           +  0|___R8___|<- pTCB[RDQ[0]]->sp
;       |________|        ----->            +  4|___R9___|
;       |________|                          +  8|___R10__| R8�`R14 �� ret_int �őޔ�
;       |________|                          + 12|___R11__|
;       |________|                          + 16|___R12__|
;       |________|                          + 20|___R13__|
;       |________|                          + 24|___R14__|
;    + 0|___GBR__|< ent_int�܂łɑҔ�       + 28|___GBR__|
;    + 4|__MACH__|<   "                     + 32|__MACH__|
;    + 8|__MACL__|<   "                     + 36|__MACL__|
;    +12|___PC___|<-  "                     + 40|___PC___|
;    +16|___SR___|<-  "                     + 44|___SR___|
;    +20|___PR___|<-  "                     + 48|___PR___|
;    +24|___R7___|<-  "                     + 52|___R7___|
;    +28|___R6___|<-  "                     + 56|___R6___|
;    +32|___R5___|<-  "                     + 60|___R5___|
;    +36|___R4___|<-  "                     + 64|___R4___|
;    +40|___R3___|<-  "                     + 68|___R3___|
;    +44|___R2___|<-  "                     + 72|___R2___|
;    +48|___R1___|<-  "                     + 76|___R1___|
;    +52|___R0___|<-  "                     + 80|___R0___|

;       ���l�T�XC Ver.6.0x�ȑO
;                              T_CTX �\����
;       |________|       �c��Ҕ�           +  0|___R8___|<- pTCB[RDQ[0]]->sp
;       |________|        ----->            +  4|___R9___|
;       |________|                          +  8|___R10__| R8�`R14 �� ret_int �őޔ�
;       |________|                          + 12|___R11__|
;       |________|                          + 16|___R12__|
;       |________|                          + 20|___R13__|
;       |________|                          + 24|___R14__|
;    + 0|___GBR__|< ent_int�܂łɑҔ�       + 28|___GBR__|
;    + 4|__MACH__|<   "                     + 32|__MACH__|
;    + 8|__MACL__|<   "                     + 36|__MACL__|
;    +12|___PC___|<-  "                     + 40|___PC___| new version ��
;    +16|___SR___|<-  "                     + 44|___SR___| sr, pc ���t
;    +20|___PR___|<-  "                     + 48|___PR___|
;    +24|___R0___|<-  "                     + 52|___R0___|
;    +28|___R1___|<-  "                     + 56|___R1___|
;    +32|___R2___|<-  "                     + 60|___R2___|
;    +36|___R3___|<-  "                     + 64|___R3___|
;    +40|___R4___|<-  "                     + 68|___R4___|
;    +44|___R5___|<-  "                     + 72|___R5___|
;    +48|___R6___|<-  "                     + 76|___R6___|
;    +52|___R7___|<-  "                     + 80|___R7___|

        .export _v3_ret_int
_v3_ret_int:                            ; void v3_ret_int(void)

        stc     sr,r0
        mov.l   SR_MASK,r1
        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        and     r1,r0
        mov.l   @r2,r1
        or      r1,r0
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
        add     #28,r15                 ;    SP += 28;

    .AIF \&HVER EQ 6
        mov.l   @(40-28,r15),r0         ;    if ((�����ݎ��� SR & 0xf0) == 0
    .AELSE
        mov.l   @(44-28,r15),r0         ;    if ((�����ݎ��� SR & 0xf0) == 0
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

        mov.l   r14,@-r15
        mov.l   r13,@-r15               ;       GBR,MACH,MACL,R8�`R14 �ޔ�
        mov.l   r12,@-r15
        mov.l   r11,@-r15
        mov.l   r10,@-r15
        mov.l   r9,@-r15
        mov.l   r8,@-r15

        mov.l   @(TCB_SP,r1),r15        ;       SP = tcb->sp;
        mov.l   @r15+,r8                ;       R8�`R14 ����
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
        ldc.l   @r15+,gbr               ; GBR,MACH,MACL ����
        lds.l   @r15+,mach
        lds.l   @r15+,macl
    .AIF \&HVER GE H'700
        mov.l   @r15+,r2                ; r2 = PC
        mov.l   @r15+,r1                ; r1 = SR
        lds.l   @r15+,pr                ; R0�`R7, PR ����
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        stc     sr,r4
        mov.l   SR_MASK,r0
        mov.l   H_500000F0,r3           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        and     r4,r0
        mov.l   @r15+,r4
        or      r3,r0
        mov.l   @r15+,r3
        ldc     r0,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r2,spc                  ; SPC, SSR ����
        ldc     r1,ssr
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
    .AIF \&HVER EQ 6
        mov.l   @r15+,r6                ; r6 = SR
        mov.l   @r15+,r5                ; r5 = PC
    .AELSE
        mov.l   @r15+,r5                ; r5 = PC
        mov.l   @r15+,r6                ; r6 = SR
    .AENDI
        lds.l   @r15+,pr                ; R0�`R7, PR ����
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        stc     sr,r3
        mov.l   SR_MASK,r7
        mov.l   H_500000F0,r4           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        and     r3,r7
        mov.l   @r15+,r3
        or      r4,r7
        mov.l   @r15+,r4
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
;                              T_CTX �\����
;       |______|                     + 0|__R8__|<- pTCB[RDQ[0]]->sp
;       |______|         �ޔ�        + 4|__R9__|
;       |______|        ----->       + 8|__R10_|
;       |______|                     +12|__R11_|
;       |______|                     +16|__R12_|
;       |______|                     +20|__R13_|
;       |______|                     +24|__R14_|
;       |______|                     +28|__GBR_|
;       |______|                     +32|_MACH_|
;       |______|                     +36|_MACL_|
;       |______|                     +40|__PC__|
;       |______|                     +44|__SR__|
;       |______|                     +48|__PR__|
;       |______|                     +52|______| R7 �ޔ�s�v
;       |______|                     +56|__R6__| (CTXPTR)
;       |______|                     +60|__R5__| (CTXPTN)
;       |______|                     +64|__R4__| (WID)
;       |______|                     +68|______| R1�`R3 �ޔ�s�v
;       |______|                     +72|______|
;       |______|                     +76|______|
;       |______|                     +80|__R0__|

;       ���l�T�XC Ver.6.0x�ȑO
;                              T_CTX �\����
;       |______|                     + 0|__R8__|<- pTCB[RDQ[0]]->sp
;       |______|         �ޔ�        + 4|__R9__|
;       |______|        ----->       + 8|__R10_|
;       |______|                     +12|__R11_|
;       |______|                     +16|__R12_|
;       |______|                     +20|__R13_|
;       |______|                     +24|__R14_|
;       |______|                     +28|__GBR_|
;       |______|                     +32|_MACH_|
;       |______|                     +36|_MACL_|
;       |______|                     +40|__PC__| new version ��
;       |______|                     +44|__SR__| sr, pc ���t
;       |______|                     +48|__PR__|
;       |______|                     +52|__R0__|
;       |______|                     +56|______| R1�`R3 �ޔ�s�v
;       |______|                     +60|______|
;       |______|                     +64|______|
;       |______|                     +68|__R4__| (WID)
;       |______|                     +72|__R5__| (CTXPTN)
;       |______|                     +76|__R6__| (CTXPTR)
;       |______|                     +80|______| R7 �ޔ�s�v

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
        bf      dispatch_8              ;    return E_OK;

; ��U���荞�݋���

dispatch_1:
        stc     sr,r7                   ; old = SR;
        ldc     r1,sr                   ; SR MD=1,RB=0,IMASK=0000
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
        sts.l   pr,@-r15
        mov.l   r1,@-r15                ; (SR = 0x40000000)
        sts.l   pr,@-r15                ; (PC)
    .AELSE
        add     #-4*1,r15               ; R7 �ޔ�s�v
        mov.l   r6,@-r15
        mov.l   r5,@-r15
        mov.l   r4,@-r15
        add     #-4*3,r15               ; R1�`R3 �ޔ�s�v
        mov.l   r0,@-r15
        sts.l   pr,@-r15
    .AIF \&HVER EQ 6
        sts.l   pr,@-r15                ; (PC)
        mov.l   r1,@-r15                ; (SR = 0x40000000)
    .AELSE
        mov.l   r1,@-r15                ; (SR = 0x40000000)
        sts.l   pr,@-r15                ; (PC)
    .AENDI
    .AENDI
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
        mov.l   r15,@(TCB_SP,r7)        ; mytcb->sp = SP;

; �R���e�L�X�g����
dispatch_5:
        mov.l   @(TCB_SP,r2),r15        ; SP = tcb->sp;

    .AIFDEF HOOK
        mov.l   SND_TASK_SWITCH_D,r0
        jsr     @r0
        nop
    .AENDI

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
    .AIF \&HVER GE H'700
        mov.l   @r15+,r2                ; r2 = PC
        mov.l   @r15+,r1                ; r1 = SR
        lds.l   @r15+,pr                ; R0�`R7, PR ����
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        stc     sr,r4
        mov.l   SR_MASK,r0
        mov.l   H_500000F0,r3           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        and     r4,r0
        mov.l   @r15+,r4
        or      r3,r0
        mov.l   @r15+,r3
        ldc     r0,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r2,spc                  ; SPC, SSR ����
        ldc     r1,ssr
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
    .AIF \&HVER EQ 6
        mov.l   @r15+,r6
        mov.l   @r15+,r5
    .AELSE
        mov.l   @r15+,r5
        mov.l   @r15+,r6
    .AENDI
        lds.l   @r15+,pr
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        stc     sr,r3
        mov.l   SR_MASK,r7
        mov.l   H_500000F0,r4           ; SR MD=1,RB=0,BL=0,IMASK=1111;
        and     r3,r7
        mov.l   @r15+,r3
        or      r4,r7
        mov.l   @r15+,r4
        ldc     r7,sr                   ; SR MD=1,RB=0,BL=1;
        ldc     r5,spc
        ldc     r6,ssr
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte
        nop

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
        stc     sr,r0
        mov.l   SR_MASK,r1
        mov.l   H_40000000,r2           ; SR MD=1,RB=0,BL=0,IMASK=0000;
        and     r0,r1
        or      r2,r1
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

        stc     sr,r0
        mov.l   SR_MASK,r1
        mov.l   KNL_SR,r2               ; �����݃}�X�N���J�[�l�����x���ɂ���
        and     r0,r1
        mov.l   @r2,r3
        or      r1,r3
        mov.l   IMASK,r1
        ldc     r3,sr                   ; SR = KNL_SR;
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
        stc     sr,r0
        mov.l   SR_MASK,r1
        mov.l   H_500000F0,r2           ; SR MD=1,RB=0,BL=1,IMASK=1111;
        and     r0,r1
        or      r2,r1
        ldc     r1,sr                   ; ���W�X�^�o���N0, �S�����݃}�X�N
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
c_sysini:       .data.l _c_sysini
wup_tsk:        .data.l _v4_wup_tsk
RDQ:            .data.l _RDQ
IMASK:          .data.l _IMASK
INEST:          .data.l _INEST
DELAY:          .data.l _DELAY
ISP:            .data.l _ISP
NOWPRI:         .data.l _NOWPRI
intini:         .data.l _intini
DDISP:          .data.l _DDISP
SDISP:          .data.l _SDISP
pTCB:           .data.l __pTCB-4
CTXPTN:         .data.l _CTXPTN
CTXPTR:         .data.l _CTXPTR
H_400000F0:     .data.l H'400000F0      ; MD=1,RB=0,BL=0,IMASK=1111
H_40000000:     .data.l H'40000000      ; MD=1,RB=0,BL=0,IMASK=0000
H_500000F0:     .data.l H'500000F0      ; MD=1,RB=0,BL=1,IMASK=1111
SR_MASK:        .data.l H'8FFFFF0F
WID:            .data.l _WID
PREVTSK:        .data.l _PREVTSK
TPRI_MAX:       .data.l _CFG + CFG_TPRI_MAX
STARTOF_STACK:  .data.l (STARTOF STACK)
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
