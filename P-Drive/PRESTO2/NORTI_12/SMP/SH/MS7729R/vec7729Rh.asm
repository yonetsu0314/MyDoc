;****************************************************************************
;* SHC�x�N�^�ݒ�̗�iMS7729RSE01)                                          *
;*                                                                          *
;*  File name : vec7729Rh.asm                                               *
;*                                                                          *
;* 16/Jan/2002                                                              *
;* 02/Jun/2003  MS7729RSE01�p�ɏC��                                      KA *
;****************************************************************************

        .import __INIT          ; �����ݒ胋�[�`��
        .import _intdef         ; ���荞�݃n���h���e�[�u��
EXPEVT:  .equ   H'FFFFFFD4      ; ��O���ۃ��W�X�^
INTEVT:  .equ   H'FFFFFFD8      ; ���荞�ݎ��ۃ��W�X�^
INTEVT2: .equ   H'A4000000      ; ���荞�ݎ��ۃ��W�X�^2
INT_NMI: .equ   14              ; �m���}�X�J�u�����荞�ݔԍ�

;* �L���b�V���̗L���^������ݒ肵�܂��B

CACHE_ON: .define "1"           ; =0:�����^=1:�L��

;* �L���b�V���̃��C�g�X���[�^���C�g�o�b�N��ݒ肵�܂��B
;* �L���b�V�����L���̏ꍇ�̂ݗL���ł��B

CACHE_WT: .define "0"           ; =0:���C�g�o�b�N�^=1:���C�g�X���[

;* ��ʗ�O�����Ńz���g�����ꍇ�́A���̕ϐ��̒l�𒲂ׂĂ��������B

        .section B,DATA,ALIGN=4
_expspc: .res.l 1               ; ��ʗ�O�������� SPC
_expssr: .res.l 1               ; ��ʗ�O�������� SSR
_expevt: .res.l 1               ; ��ʗ�O�������� EXPEVT

;* ���Z�b�g

        .section VECT,CODE

        .export _RST_VECT
_RST_VECT:
        bra     _RESET          ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
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

        mov     #0,r0
        mov.l   expevt_4,r3
        mov.l   r0,@-r3         ; expevt = 0;
        mov.l   r0,@-r3         ; expssr = 0;
        mov.l   r0,@-r3         ; expspc = 0;

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
copy_0: mov.l   @r1+,r3
        mov.l   r3,@r2
        add     #-1,r0
        cmp/eq  #0,r0
        bf/s    copy_0
        add     #4,r2
copy_1:

        mov.l   INIT,r3         ; �����ݒ胋�[�`����
        jmp     @r3
        nop

        .align  4
INIT_STACK: .data.l _INIT_STACK
INIT:       .data.l __INIT

expevt_4:   .data.l _expevt+4

ADDR_MSK:   .data.l H'1FFFFFFF
COPY_BGN:   .data.l (STARTOF VECT)
COPY_SIZE:  .data.l (STARTOF D)+(SIZEOF D)-(STARTOF VECT)
COPY_ORG:   .data.l -(copy_s-_RST_VECT)


;* ��ʗ�O����

        .org    H'00000100
        bra     vect110         ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
        nop
        nop
        nop
        nop
        nop
        nop
vect110:
        mov.l   expevt_41,r1
        mov     #EXPEVT,r0
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


;* ��ʗ�O���ہiTLB�~�X�j

        .org    H'00000400

        mov.l   expevt_44,r1
        mov     #EXPEVT,r0
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

;* ���荞�ݗv��

        .org    H'00000600

        bra     vect610         ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
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

        mov.l   intevt2,r0
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
intevt2:    .data.l INTEVT2

        .align  4
_BOOT:
        mov.w   CCR,r0
        mov.w   CCR_D,r1
        mov.l   r1,@r0          ;�b�b�q

        ; �o�X�R���g���[���̏�����

        mov.w   RTCSR,r1        ; �ď������̋֎~
        mov.w   @r1,r0
        cmp/eq  #0,r0
        bf      INI_SKIP

        mov.w   BCR1,r0         ; Hitachi ULSI Systens MS7729RSE01 �p�ݒ�
        mov.w   BCR1_D,r1
        mov.w   r1,@r0
        mov.w   BCR2,r0
        mov.w   BCR2_D,r1
        mov.w   r1,@r0
        mov.w   WCR1,r0
        mov.w   WCR1_D,r1
        mov.w   r1,@r0
        mov.w   WCR2,r0
        mov.w   WCR2_D,r1
        mov.w   r1,@r0
        mov.w   MCR,r0
        mov.w   MCR_D,r1
        mov.w   r1,@r0
        mov.w   PCR,r0
        mov.w   PCR_D,r1
        mov.w   r1,@r0
        mov.w   RTCSR,r0
        mov.w   RTCSR_D,r1
        mov.w   r1,@r0
        mov.w   RTCNT,r0
        mov.w   RTCNT_D,r1
        mov.w   r1,@r0
        mov.w   RTCOR,r0
        mov.w   RTCOR_D,r1
        mov.w   r1,@r0
        mov.w   RFCR,r0
        mov.w   RFCR_D,r1
        mov.w   r1,@r0
BOOT_1:
        mov.w   RFCR,r0
        mov.w   @r0,r1
        extu.w  r1,r1
        mov     #8,r3
        cmp/ge  r3,r1
        bf      BOOT_1

        mov.w   SDMR,r0
        mov     #0,r1
        mov.b   r1,@r0
INI_SKIP:

        mov.w   WTCSR,r0
        mov.w   WTCSR_D,r1
        mov.w   r1,@r0
        mov.w   WTCNT,r0
        mov.w   WTCNT_D,r1
        mov.w   r1,@r0
        mov.w   FRQCR,r0
        mov.w   FRQCR_D,r1
        mov.w   r1,@r0

    .aif CACHE_ON ne 0
        mov.w   CCR,r0
        mov.w   CCR_D+2,r1
        mov.l   r1,@r0          ;�b�b�q
    .aendi

        rts
        nop

BCR1:       .data.w H'FF60
BCR2:       .data.w H'FF62
WCR1:       .data.w H'FF64
WCR2:       .data.w H'FF66
MCR:        .data.w H'FF68
PCR:        .data.w H'FF6C
RTCSR:      .data.w H'FF6E
RTCNT:      .data.w H'FF70
RTCOR:      .data.w H'FF72
RFCR:       .data.w H'FF74
SDMR:       .data.w H'E880

FRQCR:      .data.w H'FF80
WTCNT:      .data.w H'FF84
WTCSR:      .data.w H'FF86
CCR:        .data.w H'FFEC

BCR1_D:     .data.w H'0008
BCR2_D:     .data.w H'2EF0
WCR1_D:     .data.w H'0C30
WCR2_D:     .data.w H'BEDD
MCR_D:      .data.w H'002C
PCR_D:      .data.w H'0000
RTCSR_D:    .data.w H'A508
RTCNT_D:    .data.w H'A500
RTCOR_D:    .data.w H'A580
RFCR_D:     .data.w H'A400

FRQCR_D:    .data.w H'A101
WTCNT_D:    .data.w H'5A00
WTCSR_D:    .data.w H'A504
CCR_D:      .data.w H'0008
    .aif CACHE_WT eq 0
            .data.w H'0005
    .aelse
            .data.w H'0003
    .aendi

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
