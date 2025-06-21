;****************************************************************************
;* SHC�x�N�^�ݒ�̗�iComputex SH7750 EVA BOD)                              *
;*                                                                          *
;*  File name : vec7750h.asm                                                *
;*                                                                          *
;* 24/Apr/2000                                                              *
;* 15/Aug/2000 SH4�L���b�V���̃p�[�W��ǉ�                                  *
;* 12/Sep/2000  �L���b�V���̏�����������ǉ�                                *
;* 12/Sep/2001  �L���b�V���̃p�[�W�������I��P2�̈�Ŏ��s����悤�ɏC��      *
;*              VECT�Z�N�V������CODE�����ɏC��                           TA *
;* 23/Jan/2002 ���[�_�ɂ�� ROM->RAM�R�s�[�ɑΉ�                         TA *
;****************************************************************************

;* �������}�b�v�ɍ��킹�A����̃X�^�b�N�|�C���^�����l��ύX���Ă��������B

        .import __INIT          ; �����ݒ胋�[�`��
        .import _intdef         ; ���荞�݃n���h���e�[�u��
EXPEVT:     .equ   H'FF000024   ; ��O���ۃ��W�X�^
INTEVT:     .equ   H'FF000028   ; ���荞�ݎ��ۃ��W�X�^
INT_NMI:    .equ   14           ; �m���}�X�J�u�����荞�ݔԍ�

;* �L���b�V���̗L���^������ݒ肵�܂��B

CACHE_ON: .define "1"           ; =0:�����^=1:�L��

;* �L���b�V���̃��C�g�X���[�^���C�g�o�b�N��ݒ肵�܂��B
;* �L���b�V�����L���̏ꍇ�̂ݗL���ł��B

CACHE_WT: .define "1"           ; =0:���C�g�o�b�N�^=1:���C�g�X���[

;* ��ʗ�O�����Ńz���g�����ꍇ�́A���̕ϐ��̒l�𒲂ׂĂ��������B

        .section B,DATA,ALIGN=4
_expspc: .res.l 1               ; ��ʗ�O�������� SPC
_expssr: .res.l 1               ; ��ʗ�O�������� SSR
_expevt: .res.l 1               ; ��ʗ�O�������� EXPEVT

;* ���Z�b�g

        .section VECT,CODE


        .export _RST_VECT
_RST_VECT:
        nop
        bra     _RESET          ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
        nop
        nop
        nop
        nop
        nop
        .export _RESET
_RESET:
        mov.l   INIT_STACK,r15   ; �X�^�b�N�|�C���^�ݒ�

;* �������ݒ蓙�A�ŏ��ɕK�v�ȏ���������ꍇ�́A�ǉ����Ă��������B

        bsr     _BOOT
        nop

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
        mov.l   ADDR_MSK,r0
        and     r0,r1
        and     r0,r2
        add     r3,r1
        mov.l   COPY_SIZE,r0
        cmp/eq  r1,r2
        add     #3,r0
        bt      copy_1
        shlr2   r0
        mov.l   ADDR_P2,r3
        or      r3,r1
        or      r3,r2
copy_0: mov.l   @r1+,r3
        mov.l   r3,@r2
        add     #-1,r0
        cmp/eq  #0,r0
        bf/s    copy_0
        add     #4,r2
copy_1:

        mov     #0,r0
        mov.l   expevt_4,r3
        mov.l   r0,@-r3         ; expevt = 0;
        mov.l   r0,@-r3         ; expssr = 0;
        mov.l   r0,@-r3         ; expspc = 0;

        mov.l   INIT,r3         ; �����ݒ胋�[�`����
        jmp     @r3
        nop

        .align  4
INIT_STACK: .data.l _INIT_STACK ; �X�^�b�N�̈�I���̃A�h���X��
INIT:       .data.l __INIT

expevt_4:   .data.l _expevt+4

ADDR_MSK:   .data.l H'1FFFFFFF
ADDR_P2:    .data.l H'A0000000
COPY_BGN:   .data.l (STARTOF VECT)
COPY_SIZE:  .data.l (STARTOF D)+(SIZEOF D)-(STARTOF VECT)
COPY_ORG:   .data.l -(copy_s-_RST_VECT)


;* ��ʗ�O����

        .org    H'00000100
        nop
        bra     vect110         ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
        nop
        nop
        nop
        nop
        nop
vect110:
        mov.l   expevt_41,r1
        mov.l   expevt_42,r0
        mov.l   @r0,r0
        mov.l   r0,@-r1         ; expevt = EXPEVT���W�X�^�̗�O�R�[�h;
        stc     ssr,r0
        mov.l   r0,@-r1         ; expssr = SSR;
        stc     spc,r0
        mov.l   r0,@-r1         ; expspc = SPC;

        bra     $               ; �K�v�Ȃ烆�[�U�[�ŋL�q���Ă��������B
        nop

        .align  4
expevt_41:  .data.l _expevt+4
expevt_42:  .data.l EXPEVT


;* ��ʗ�O���ہiTLB�~�X�j

        .org    H'00000400
        mov.l   expevt_44,r1
        mov.l   expevt_45,r0
        mov.l   @r0,r0
        mov.l   r0,@-r1         ; expevt = EXPEVT���W�X�^�̗�O�R�[�h;
        stc     ssr,r0
        mov.l   r0,@-r1         ; expssr = SSR;
        stc     spc,r0
        mov.l   r0,@-r1         ; expspc = SPC;

        bra     $               ; �K�v�Ȃ烆�[�U�[�ŋL�q���Ă��������B
        nop

        .align  4
expevt_44:  .data.l _expevt+4
expevt_45:  .data.l EXPEVT


;* ���荞�ݗv��

        .org    H'00000600
        nop
        bra     vect610         ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
        nop
        nop
        nop
        nop
        nop
vect610:                        ; �����݂ł� BL=1,RB=1 �֐؂�ւ��
        stc.l   r0_bank,@-r15   ; R0_BANK0 �ޔ�
        mov.l   r0,@-r15        ; R0_BANK1 �ޔ�
        mov.l   r1,@-r15        ; R1_BANK1 �ޔ�

        mov.l   intevt,r0
        mov.l   @r0,r1          ; R1_BANK1 = INTEVT���W�X�^�̗�O�R�[�h;
        tst     r1,r1           ; if (��O�R�[�h == 0)
        bt/s    vect610_rte     ;    �����݃��^�[����
        shar    r1
        mov.l   intdef,r0       ; R0_BANK1 = def_inh �̓o�^�e�[�u��
        shar    r1
        add     r1,r0           ; R0_BANK0 = �e�[�u�����̑Ή��A�h���X
        mov.l   @r15+,r1        ; R1_BANK1 ����
        ldc.l   @r0+,sr         ; SR = def_inh �œo�^���ꂽ�l
                                ; �����ŁARB=0 �ɐ؂�ւ��
        stc     r0_bank,r0      ; R0_BANK0 = R0_BANK1
        ldc.l   @r15+,r0_bank   ; R0_BANK1 ����

        mov.l   @r0,r0          ; R0_BANK0 = def_inh �œo�^�� inthdr �A�h���X
        jmp     @r0             ; inthdr ��
        mov.l   @r15+,r0        ; R0_BANK0 ����

vect610_rte:                    ; ��O�R�[�h0�̏ꍇ
        mov.l   @r15+,r1        ; R1_BANK1 ����
        mov.l   @r15+,r0        ; R0_BANK1 ����
        ldc.l   @r15+,r0_bank   ; R0_BANK0 ����
        rte                     ; �����݃��^�[��
        nop

        .align  4
intdef:     .data.l _intdef-INT_NMI*8
intevt:     .data.l INTEVT


;* �n�[�h�E�F�A�̏�����

        .align  4
_BOOT:
        mov.l   CCR,r0
        mov.l   CCR_D,r1
        mov.l   r1,@r0          ; CCR (OC&IC disable)

        ; �o�X�R���g���[���̏�����

        mov.l   RTCSR,r1        ; �ď������̋֎~
        mov.w   @r1,r0
        cmp/eq  #0,r0
        bf      INI_SKIP

        mov.l   BCR1,r0         ; Computex SH7750 EVA BOD �p�ݒ�
        mov.l   BCR1_D,r1
        mov.l   r1,@r0          ; BCR1

        mov.l   BCR2,r0
        mov.w   BCR2_D,r1
        mov.w   r1,@r0          ; BCR2

        mov.l   WCR1,r0
        mov.l   WCR1_D,r1
        mov.l   r1,@r0          ; WCR1

        mov.l   WCR2,r0
        mov.l   WCR2_D,r1
        mov.l   r1,@r0          ; WCR2

        mov.l   WCR3,r0
        mov.l   WCR3_D,r1
        mov.l   r1,@r0          ; WCR3

        mov.l   MCR,r0
        mov.l   MCR_D,r1
        mov.l   r1,@r0          ; MCR

        mov.l   SDMR3,r0
        mov     #0,r1
        mov.b   r1,@r0          ; SDMR3

        mov.l   PCR,r0
        mov.w   PCR_D,r1
        mov.w   r1,@r0          ; PCR

        mov.l   RTCNT,r0
        mov.w   RTCNT_D,r1
        mov.w   r1,@r0          ; RTCNT

        mov.l   RTCOR,r0
        mov.w   RTCOR_D,r1
        mov.w   r1,@r0          ; RTCOR

        mov.l   RFCR,r0
        mov.w   RFCR_D,r1
        mov.w   r1,@r0          ; RFCR

        mov.l   RTCSR,r0
        mov.w   RTCSR_D,r1
        mov.w   r1,@r0          ; RTCSR

        mov.l   CCR,r0
        mov.l   CCR_D,r1
        mov.l   r1,@r0          ; CCR (OC&IC disable)

        nop
        nop
        nop
        nop

        mov.l   RFCR,r1
        mov     #8,r2
BOOT_1: mov.w   @r1,r0
        cmp/hs  r2,r0
        bf      BOOT_1

        mov.l   MCR,r0
        mov.l   MCR_D+4,r1
        mov.l   r1,@r0          ; MCR

        mov.l   SDMR3,r0
        mov     #0,r1
        mov.b   r1,@r0          ; SDMR3

        mov.l   RTCSR,r0
        mov.w   RTCSR_D+2,r1
        mov.w   r1,@r0          ; RTCSR
INI_SKIP:

    .aif CACHE_ON ne 0
        mov.l   CCR,r0
        mov.l   CCR_D+4,r1
        mov.l   r1,@r0          ; CCR (OC&IC enable, write through)
    .aendi

        rts
        nop

        .import __KERNEL_hdrlen
        .export __kernel_cash_purge
        .align  4

__kernel_cash_purge:
        mov.l   L1_0,r0
        mov.l   L1_1,r1
        mov.l   L1_2,r2
        and     r1,r0
        or      r2,r0
        jmp     @r0
        nop

        .align  4
L1_0:   .data.l kernel_cash_purge
L1_1:   .data.l H'1FFFFFFF
L1_2:   .data.l H'A0000000

kernel_cash_purge:
        mov.l   HDRLEN,r0
        mov     #32,r2
        mov.b   @r0,r1
        mov     r4,r3
        add     r1,r4
        add     #-4,r4
        clrt
L0_1:
        ocbp    @r3
        subc    r2,r1
        bf/s    L0_1
        add     r2,r3

        mov.l   CCR,r2
        mov.l   CCR_ICI,r3
        mov.l   @r2,r0
        ocbp    @r4
        or      r3,r0
        mov.l   r0,@r2
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        rts
        nop


        .align  4
BCR1:       .data.l H'FF800000
BCR2:       .data.l H'FF800004
WCR1:       .data.l H'FF800008
WCR2:       .data.l H'FF80000C
WCR3:       .data.l H'FF800010
MCR:        .data.l H'FF800014
PCR:        .data.l H'FF800018
RTCSR:      .data.l H'FF80001C
RTCNT:      .data.l H'FF800020
RTCOR:      .data.l H'FF800024
RFCR:       .data.l H'FF800028
SDMR3:      .data.l H'FF94008C
CCR:        .data.l H'FF00001C

BCR1_D:     .data.l H'00000008
WCR1_D:     .data.l H'07770777
WCR2_D:     .data.l H'FFFE4E17
WCR3_D:     .data.l H'07777777
MCR_D:      .data.l H'80090194
            .data.l H'C0090194
CCR_D:      .data.l H'00000808
    .aif CACHE_WT eq 0
            .data.l H'00000105
    .aelse
            .data.l H'00000103
    .aendi
CCR_ICI:    .data.l H'00000800

HDRLEN:     .data.l __KERNEL_hdrlen

BCR2_D:     .data.w H'3FF4
PCR_D:      .data.w H'0000
RTCNT_D:    .data.w H'A500
RTCOR_D:    .data.w H'A508
RTCSR_D:    .data.w H'A500
            .data.w H'A508
RFCR_D:     .data.w H'A482

;* �e�Z�N�V�����̃A�h���X

        .section P,CODE,ALIGN=4         ; �v���O�����̈�iROM�j
        .section D,DATA,ALIGN=4         ; �������f�[�^�̈�iROM�j
        .section R,DATA,ALIGN=4         ; �������f�[�^�̈�iRAM�j
        .section B,DATA,ALIGN=4         ; ���������f�[�^�̈�iRAM�j
        .section STACK,DATA,ALIGN=4     ; �X�^�b�N�̈�iRAM�j
        .section C,DATA,ALIGN=4         ; �萔�̈�iROM�j
__P_BGN .data.l (STARTOF P)
__P_END .data.l (STARTOF P)+(SIZEOF P)
__C_BGN .data.l (STARTOF C)
__C_END .data.l (STARTOF C)+(SIZEOF C)
__D_ROM .data.l (STARTOF D)
__D_BGN .data.l (STARTOF R)
__D_END .data.l (STARTOF R)+(SIZEOF R)
__B_BGN .data.l (STARTOF B)
__B_END .data.l (STARTOF B)+(SIZEOF B)
        .export __P_BGN
        .export __P_END
        .export __C_BGN
        .export __C_END
        .export __D_ROM
        .export __D_BGN
        .export __D_END
        .export __B_BGN
        .export __B_END

; ROM�擪 +--------------------+
;         | �����݃x�N�^(VECT) |
;         +--------------------+
;         |   �v���O�����̈�   |
;         |        (P)         |
;         +--------------------+
;         |      �萔�̈�      |
;         |        (C)         |
;         +--------------------+
;         |  �������f�[�^�̈�  |---+
;         |        (D)         |   |
;         +--------------------+   |_INITSCT() �ŃR�s�[
;         �`                  �`   |
; RAM�擪 +--------------------+   |
;         |  �������f�[�^�̈�  |<--+
;         |        (R)         |
;         +--------------------+
;         | ���������f�[�^�̈� |
;         |        (B)         |
;         +--------------------+
;         |    �X�^�b�N�̈�    |
;         |      (STACK)       |
;         �`                  �`
; SP�����l+--------------------+

        .END
