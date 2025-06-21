;****************************************************************************
;* SHC�p�x�N�^�e�[�u���ݒ�̗�iMS7615SE01 for SH7615�j                     *
;*                                                                          *
;*  File name : vec7615h.asm                                                *
;*  Copyright (c) 2000-2002 MiSPO Co., Ltd.  All rights reserved.           *
;*                                                                          *
;* 11/Apr/2000 �쐬                                               OK(MiSPO) *
;* 11/Feb/2001 ���g���ύX���̏����ݒ�l(WTCSR)�̎��o���~�X�C��  OK(MiSPO) *
;* 26/Apr/2001 MCR�ݒ�ύX                                               OK *
;* 16/Jan/2002 ���[�_�ɂ�� ROM->RAM�R�s�[�ɑΉ�                         TA *
;* 09/Oct/2002 FMR���������ŏ��ɕύX                                     KA *
;* 25/Dec/2002 ���x���𓝈� stack_init->INIT_STACK                       KA *
;****************************************************************************

;* ���̉ӏ����J�X�^�}�C�Y���Ă�������
;*   SP �����l�́ARAM �̍Ō�̃A�h���X+1 �ɐݒ肷��̂���ʓI�ł��B
;*   MS7615SE01 board ��SDRAM�́A6000000�`7FFFFFF��32MB�ł��B

;* �{�T���v���Ŏg�p����SDRAM�\��
;*   6000000�`6EFFFFF  for MAIN memory
;*   6F00000�`6FFFFFF  for RAM DISK memory
;*   7000000�`7FFFFFF  reserved

__INIT_STACK:  .equ    h'6f00000    ; �X�^�b�N�̈�I���̃A�h���X ��

;* �L���b�V���̗L���^������ݒ肵�܂��B

CACHE_ON: .define "1"               ; =0:�����^=1:�L��

;* �L���b�V���̃��C�g�X���[�^���C�g�o�b�N��ݒ肵�܂��B
;* �L���b�V�����L���̏ꍇ�̂ݗL���ł��B�܂��A�L���b�V���̐ݒ�Ƃ͕ʂ�
;* SDRAM�̐ݒ�ɂ́A���C�g�X���[�ł́ASingleWrite
;* ���C�g�o�b�N�ł́ABurst Write�ɂ��܂�.

CACHE_WT: .define "0"               ; =0:���C�g�o�b�N�^=1:���C�g�X���[


;* �O���Q��

        .IMPORT __INIT              ; �����ݒ胋�[�`��

;* �����݃x�N�^

        .SECTION VECT,DATA          ; VECT�Z�N�V������0�Ԓn�Ƀ����N���邱��
        .export _RST_VECT
_RST_VECT:
        .data.l (STARTOF PON)-(STARTOF VECT); �p���[�I�����Z�b�g PC �����l ��
        .data.l __INIT_STACK                ; �p���[�I�����Z�b�g SP �����l ��
        .data.l (STARTOF PON)-(STARTOF VECT); �}�j���A�����Z�b�g PC �����l ��
        .data.l __INIT_STACK                ; �}�j���A�����Z�b�g SP �����l ��

        ; �����݃x�N�^�� RAM ��ɍĔz�u����O��̂��߁A�ȍ~�̃x�N�^�͒�`��
        ; �܂���Bdef_inh �V�X�e���R�[����p�����A���ڃx�N�^�� ROM �ɒ�`��
        ; ��ꍇ�A�����֏��������Ă��������B

;* ���Z�b�g����

        .section PON,CODE,ALIGN=4
        .export _RESET
_RESET:
        ; ���X�^�[�g���l�����Ċ��荞�݃}�X�N���x�����ō���

        stc     sr,r0
        or      #H'f0,r0
        ldc     r0,sr

        mov.l   CCR,r0
        mov     #0,r1               ; disable cache
        mov.b   r1,@r0
        mov     #h'10,r1            ; force purge
        mov.b   r1,@r0

        ; �o�X�R���g���[���̏�����

        mov.l   RTCSR,r1        ; �ď������̋֎~ test RTCSR or MCR
        add     #2,r1
        mov.w   @r1,r0
        cmp/eq  #0,r0
        bf      INI_SKIP

        ; �V�X�e������N���b�N�̐ݒ�(I��,E��,P��)

        bsr     _FMR_RDY
        nop
_FMR_RDY:
        sts     pr,r2
        mov.l   FMR_START,r0
        add     r2,r0
        mov.l   DIRECT_RW,r1
        mov.l   FMR_END,r3
        add     r2,r3

_FMR_TRANS:
        mov.l   @r0+,r2
        mov.l   r2,@r1
        cmp/gt  r3,r0
        bf/s    _FMR_TRANS
        add     #4,r1

        bsr     _FMR_ENTRY
        nop

        mov.l   BCR1,r0
        mov.l   BCR1_DATA,r1
        mov.l   r1,@r0

        mov.l   BCR2,r0
        mov.l   BCR2_DATA,r1
        mov.l   r1,@r0

        mov.l   BCR3,r0
        mov.l   BCR3_DATA,r1
        mov.l   r1,@r0

        mov.l   WCR1,r0
        mov.l   WCR1_DATA,r1
        mov.l   r1,@r0

        mov.l   WCR2,r0
        mov.l   WCR2_DATA,r1
        mov.l   r1,@r0

        mov.l   WCR3,r0
        mov.l   WCR3_DATA,r1
        mov.l   r1,@r0

        mov.l   MCR,r0
        mov.l   MCR_DATA1,r1
        mov.l   r1,@r0

        ; Set Refresh timer ��

        mov.l   RTCOR,r0
        mov.l   RTCOR_DATA,r1
        mov.l   r1,@r0

        mov.l   RTCNT,r0
        mov.l   RTCNT_DATA,r1
        mov.l   r1,@r0

        mov.l   RTCSR,r0
        mov.l   RTCSR_DATA,r1
        mov.l   r1,@r0

        ; Set mode register in SDRAM ��

        mov.l   SDMR,r0
        mov     #h'ff,r1
        mov.w   r1,@r0              ; setting SDRAM

        mov.l   SD_BANK1,r0
        mov.l   @r0,r0              ; dummy read from SDRAM BANK1
        mov.l   SD_BANK2,r0
        mov.l   @r0,r0              ; dummy read from SDRAM BANK2
        mov.l   SD_BANK3,r0
        mov.l   @r0,r0              ; dummy read from SDRAM BANK3
        mov.l   SD_BANK4,r0
        mov.l   @r0,r0              ; dummy read from SDRAM BANK4
INI_SKIP:

    .aif CACHE_ON ne 0

        ; �L���b�V�����C�l�[�u���ɂ���.

        mov.l   CCR,r1
        mov.b   @r1,r0
        or      #h'10,r0            ; force purge
        mov.b   r0,@r1

    .aif CACHE_WT eq 0
        mov     #h'21,r0            ; as write back
    .aelse
        mov     #h'01,r0            ; as write through
    .aendi

        mov.b   r0,@r1              ; enable cache

    .aendi

; FLASH/ROM -> RAM �R�s�[��
; SDRAM��Ŏ��s����v���O�����C���[�W�����̂܂܃I�t�Z�b�g��t����FLASH�ɏ���
; ���񂾂Ƃ� FLASH -> RAM �R�s�[������R�[�h�ł��B
; �I�t�Z�b�g�����݂��m�F�����ƌ��̃A�h���X(SDRAM)�Ƀv���O�������R�s�[�����X
; �̃A�h���X(__INIT)�ɃW�����v�����������ŏ�����SDRAM�œ����Ă������̂悤�ɓ�
; �������܂��B
; ���ړ�(�I�t�Z�b�g)���Ȃ��ꍇ�̓R�s�[�͍s���܂���B

        bsr     copy_s
        nop
copy_s: sts     pr,r1
        mov.l   COPY_ORG,r3
        mov.l   COPY_BGN,r2
        add     r3,r1
        mov.l   COPY_SIZE,r0
        cmp/eq  r1,r2
        add     #3,r0
        bt      copy_1
        shlr2   r0
copy_0: mov.l   @r1+,r3
        mov.l   r3,@r2
        add     #-1,r0
        cmp/eq  #0,r0
        bf/s    copy_0
        add     #4,r2
copy_1:

        ; ���������[�`���֔�ԁB

        mov.l   INIT,r0
        mov.l   INIT_STACK,sp       ; �X�^�b�N�|�C���^������
        jmp     @r0
        nop

_FMR_ENTRY:
        mov.l   DIRECT_RW,r0
        jmp     @r0                 ; branch to
        nop


;* ���W�X�^�A�h���X�Ə����ݒ�l ��

        .align  4

    .aif CACHE_WT eq 0
SDMR:           .data.l  h'ffff0088  ;SDRAM:���C�e���V=2/Burst /Length=4
    .aelse
SDMR:           .data.l  h'ffff0888  ;SDRAM:���C�e���V=2/Single/Length=4
    .aendi

SD_BANK1:       .data.l  h'06000000  ;SDRAM BANK1 �A�h���X
SD_BANK2:       .data.l  h'06800000  ;SDRAM BANK2 �A�h���X
SD_BANK3:       .data.l  h'07000000  ;SDRAM BANK3 �A�h���X
SD_BANK4:       .data.l  h'07800000  ;SDRAM BANK4 �A�h���X

BCR1_DATA:      .data.l  h'a55a63f1  ;CS0=NOMAL ROM/CS2=Big/CS3=SDRAM/CS4=Big
BCR2_DATA:      .data.l  h'a55a02f8  ;CS1=16bit/CS2=32bit/CS3=32bit/CS4=16bit

    .aif CACHE_WT eq 0
BCR3_DATA:      .data.l  h'a55a0c01  ;Burst write
    .aelse
BCR3_DATA:      .data.l  h'a55a0c00  ;Single write
    .aendi

WCR1_DATA:      .data.l  h'a55aab7f  ;CS0=4 idel, wait depend on BCR1,BCR3
                                     ;CS1=2 idel, wait depend on BCR1,BCR3
                                     ;CS2=2 idel, wait depend on BCR1,BCR3
                                     ;CS3=2 idel, wait SDRAM�̃��C�e���V=2

WCR2_DATA:      .data.l  h'a55a000b  ;�����l
WCR3_DATA:      .data.l  h'a55a0010  ;

MCR_DATA1:      .data.l  h'a55a5dd8  ;SDRAM=32bit�AMX=5(8Mx8bit)�Auto Refresh

RTCSR_DATA:     .data.l  h'a55a0008  ;P��/4(3.75MHz--0.27us)
RTCOR_DATA:     .data.l  h'a55a0039  ;64ms/4096 = 15.6us =0.27us*57---->HEX=39
RTCNT_DATA:     .data.l  h'a55a0000

        .align  4

CCR:            .data.l  h'fffffe92
DIRECT_RW:      .data.l  h'1000e000   ; XRAM top address
BCR1:           .data.l  h'ffffffe0
BCR2:           .data.l  h'ffffffe4
BCR3:           .data.l  h'fffffffc
WCR1:           .data.l  h'ffffffe8
WCR2:           .data.l  h'ffffffc0
WCR3:           .data.l  h'ffffffc4
MCR:            .data.l  h'ffffffec
RTCSR:          .data.l  h'fffffff0
RTCNT:          .data.l  h'fffffff4
RTCOR:          .data.l  h'fffffff8

INIT_STACK:     .data.l  __INIT_STACK
INIT:           .data.l  __INIT

COPY_BGN        .data.l  (STARTOF VECT)                                 ; ROM:00000000  RAM:6000000  R/R:0000000
COPY_SIZE       .data.l  (STARTOF D)+(SIZEOF D)-(STARTOF VECT)          ; ROM/RAM:�������R�[�h+�f�[�^�̃T�C�Y
COPY_ORG        .data.l  (STARTOF VECT)-(STARTOF PON)-(copy_s-_RESET)   ; ROM

FMR_START:      .data.l  _FMR_START - _FMR_RDY
FMR_END:        .data.l  _FMR_END - _FMR_RDY


;* �V�X�e������N���b�N�̐ݒ�(I��,E��,P��)
;* SH7615 H/W �}�j���A��(�b���)
;*   3.2.5 ���W�X�^�ɂ�铮����g���̑I��
;*     (2) ���g���ύX���@
;* ����сASH7612 H/W �}�j���A�� P3-13..P3-17 ���Q�l�ɍ쐬

_FMR_START:
        mov.l   WTCSR,r0
        mov.l   WTCSR_DATA,r1
        mov.w   r1,@r0                ; set WTCSR

        mov.l   DUMMY_DATA,r0
        mov.l   @r0,r0                ; read cache through(dummy read)

        mov.l   FMR,r0
        mov     #h'0c,r1              ; I:E:P = 4:4:1
        bra     _FMR_W
        nop

;* FMR �ւ̏������ݖ��߂�4n�ɔz�u����.

        .align 4

_FMR_W:
        mov.b   r1,@r0                ; modify FMR
        mov.b   @r0,r1                ; read FMR

        rts
        nop

        .align  4

WTCSR:          .data.l  h'fffffe80
FMR:            .data.l  h'fffffe90
DUMMY_DATA:     .data.l  h'20000000
WTCSR_DATA:     .data.l  h'0000a51f   ; WT/IT=0,TME=0,��/16384

_FMR_END:

;* �e�Z�N�V�����̃A�h���X

        .SECTION P,CODE,ALIGN=4     ; �R�[�h�̈�iROM�j
        .SECTION D,DATA,ALIGN=4     ; �������f�[�^�̈�iROM�j
        .SECTION R,DATA,ALIGN=4     ; �������f�[�^�̈�iRAM�j
        .SECTION B,DATA,ALIGN=4     ; ���������f�[�^�̈�iRAM�j
        .SECTION STACK,DATA,ALIGN=4 ; �X�^�b�N�̈�iRAM�j
        .SECTION C,DATA,ALIGN=4     ; �萔�̈�iROM�j
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

; ROM �擪 +------------------+
;          |�����݃x�N�^(VECT)| --- ���Z�b�g�x�N�^�̂�
;          |------------------|
;          |  ����������(PON) |
;          |------------------|
;          |  �v���O�����̈�  |
;          |       (P)        |
;          |------------------|
;          |     �萔�̈�     |
;          |       (C)        |
;          |------------------|
;          | �������f�[�^�̈� | ---+
;          |       (D)        |    |
;          |------------------|    | _INITSCT() �ŃR�s�[
;         �`                  �`   |
; RAM �擪 |------------------|    |
;          | �����݃x�N�^�̈� | �� | ---- �����͋󂯂ă����N
;          |------------------|    |
;          | �������f�[�^�̈� | ��-+
;          |       (R)        |
;          |------------------|
;          |���������f�[�^�̈�|
;          |       (B)        |
;          |------------------|
;          |   �X�^�b�N�̈�   |
;          |     (STACK)      |

    .END
