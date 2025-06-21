;****************************************************************************
;* SHC�x�N�^�ݒ�̗�iMS7751SE01)                                           *
;*                                                                          *
;*  File name : vec7751h.asm                                                *
;*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                               *
;*  All rights reserved.                                                    *
;*                                                                          *
;* 24/Apr/2000                                                              *
;* 15/Aug/2000 SH4�L���b�V���̃p�[�W��ǉ�                                  *
;* 12/Sep/2000  �L���b�V���̏�����������ǉ�                                *
;* 05/Sep/2001  Modified for SH7751SE01 Eva Board  :SP                      *
;* 22/Oct/2002  ROM->RAM�R�s�[�ɑΉ�                                     KA *
;* 24/Feb/2003  __kernel_cash_purge ���L���b�V���G���A�Ŏ��s           KA *
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
        nop
        bra     vect010         ; CSIDE �f�o�b�O���̊����݃G���g���� +10
        nop
        nop
        nop
        nop
        nop
        nop
        .export _RESET
_RESET:
vect010:
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

        mov.l   FRQCR,r0        ; Hitachi ULSI Systems MS7751SE01 �p�ݒ�
        mov.w   FRQCR_D,r1
        mov.w   r1,@r0          ; FRQCR

        mov.l   BCR1,r0
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

        mov.l   PCTRA,r0
        mov.l   PCTRA_D,r1
        mov.l   r1,@r0          ; PCTRA

        mov.l   PCTRB,r0
        mov.l   PCTRB_D,r1
        mov.l   r1,@r0          ; PCTRB

        mov.l   GPIOIC,r0
        mov.w   GPIOIC_D,r1
        mov.w   r1,@r0          ; GPIOIC


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
BCR1:       .data.l H'FF800000      ; Bus Control Register-1
BCR2:       .data.l H'FF800004      ; Bus Control Register-2
WCR1:       .data.l H'FF800008      ; Wait Control Register-1
WCR2:       .data.l H'FF80000C      ; Wait Control Register-2
WCR3:       .data.l H'FF800010      ; Wait Control Register-3
MCR:        .data.l H'FF800014      ; Memory Control Register
PCR:        .data.l H'FF800018      ; PCMCIA Control Register.
RTCSR:      .data.l H'FF80001C      ; Refresh timer Control/Status Register.
RTCNT:      .data.l H'FF800020      ; Refresh timer counter.
RTCOR:      .data.l H'FF800024      ; Refresh time constant Register.
RFCR:       .data.l H'FF800028      ; Refresh count Register.
SDMR3:      .data.l H'FF940088      ; SDRAM mode Register.
CCR:        .data.l H'FF00001C      ; Cache Control Register.
PCTRA:      .data.l H'FF80002C      ; Port Control Register-A
PCTRB:      .data.l H'FF800040      ; Port Control Register-B
GPIOIC:     .data.l H'FF800048      ; GPIO Interrupt control Register.
FRQCR       .data.l H'FFC00000      ; Frequency Control Register.
FRQCR2      .data.l H'FFC00014      ; Frequency Control Register-2.

BCR1_D:     .data.l H'00080008
                    ; bit31    : ENDIAN    0   MD5 level monitor(Read only)
                    ; bit30    : MASTER    0   MD7 level monitor(Read only)
                    ; bit29    : A0MPX     0   MD6 level monitor(Read only)
                    ; bit28-27 : Reserved  00
                    ; bit26    : DPUP      0   Data pull-up ON
                    ; bit25    : IPUP      0   Control input pull-up ON
                    ; bit24    : OPUP      0   Control output pull-up ON
                    ; bit23-22 : Reserved  00
                    ; bit21    : A1MBC     0   CS1 area ordinary SRAM
                    ; bit20    : A4MBC     0   CS4 area ordinary SRAM
                    ; bit19    : BREQEN    1   BREQ enable
                    ; bit18    : Reserved  0
                    ; bit17    : MEMMPX    0   CS1 to CS6 normal interface
                    ; bit16    : DMABST    0   DMAC burst transfer OFF
                    ; bit15    : HIZMEM    0   Control line High-Z
                    ; bit14    : HIZCNT    0   RAS,CAS High-Z
                    ; bit13-11 : A0BST2-0  000 CS0 area ordinary memory
                    ; bit10- 8 : A5BST2-0  000 CS5 area ordinary memory
                    ; bit 7- 5 : A6BST2-0  000 CS6 area ordinary memory
                    ; bit 4- 2 : DRAMTP2-0 010 CS2 area ordinary memory
                    ;                          CS3 area SDRAM
                    ; bit 1    : Reserved  0
                    ; bit 0    : A56PCM    0   CS5,CS6 area normal memory

WCR1_D:     .data.l H'02770771
                    ; bit31    : Reserved  0
                    ; bit30-28 : DMAIW2-0  000 DMA cycle No idle cycles
                    ; bit27    : Reserved  0
                    ; bit26-24 : A6IW2-0   010 CS6 area  2 idle cycle inserted
                    ; bit23    : Reserved  0
                    ; bit22-20 : A5IW2-0   111 CS5 area 15 idle cycle inserted
                    ; bit19    : Reserved  0
                    ; bit18-16 : A4IW2-0   111 CS4 area 15 idle cycle inserted
                    ; bit15    : Reserved  0
                    ; bit14-12 : A3IW2-0   000 CS3 area No idle cycles
                    ; bit11    : Reserved  0
                    ; bit10- 8 : A2IW2-0   111 CS2 area 15 idle cycle inserted
                    ; bit 7    : Reserved  0
                    ; bit 6- 4 : A1IW2-0   111 CS1 area 15 idle cycle inserted
                    ; bit 3    : Reserved  0
                    ; bit 2- 0 : A0IW2-0   001 CS0 area  1 idle cycle inserted

WCR2_D:     .data.l H'7ffe4fe7
                    ; bit31-29 : A6W2-0    011 CS6 area  3 wait cycle inserted
                    ; bit28-26 : A6B2-0    111 CS6 area burst pitch 7
                    ; bit25-23 : A5W2-0    111 CS5 area 15 wait cycle inserted
                    ; bit22-20 : A5B2-0    111 CS5 area burst pitch 7
                    ; bit19-17 : A4W2-0    111 CS4 area 15 wait cycle inserted
                    ; bit16    : Reserved  0
                    ; bit15-13 : A3W2-0    010 CS3 area CAS latency 2-cycle
                    ; bit12    : Reserved  0
                    ; bit11- 9 : A2W2-0    111 CS2 area 15 wait cycle inserted
                    ; bit 8- 6 : A1W2-0    111 CS1 area 15 wait cycle inserted
                    ; bit 5- 3 : A0W2-0    100 CS0 area  6 wait cycle inserted
                    ; bit 2- 0 : A0B2-0    111 CS0 area burst pitch 7

WCR3_D:     .data.l H'00000000
                    ; bit31-27 : Reserved  00000
                    ; bit26    : A6S0      0   CS6 area setup wait cycle = 0
                    ; bit25-24 : A6H1-0    00  CS6 area hold wait cycle = 0
                    ; bit23    : Reserved  0
                    ; bit22    : A5S0      0   CS5 area setup wait cycle = 0
                    ; bit21-20 : A5H1-0    00  CS5 area hold wait cycle = 0
                    ; bit19    : Reserved  0
                    ; bit18    : A4S0      0   CS4 area setup wait cycle = 0
                    ; bit17-16 : A4H1-0    00  CS4 area hold wait cycle = 0
                    ; bit15    : Reserved  0
                    ; bit14    : A3S0      0   CS3 area setup wait cycle = 0
                    ; bit13-12 : A3H1-0    00  CS3 area hold wait cycle = 0
                    ; bit11    : Reserved  0
                    ; bit10    : A2S0      0   CS2 area setup wait cycle = 0
                    ; bit 9- 8 : A2H1-0    00  CS2 area hold wait cycle = 0
                    ; bit 7    : Reserved  0
                    ; bit 6    : A1S0      0   CS1 area setup wait cycle = 0
                    ; bit 5- 4 : A1H1-0    00  CS1 area hold wait cycle = 0
                    ; bit 3    : Reserved  0
                    ; bit 2    : A0S0      0   CS0 area setup wait cycle = 0
                    ; bit 1- 0 : A0H1-0    00  CS0 area hpld wait cycle = 0

MCR_D:      .data.l H'10110194
                    ; bit31    : RASD      0   �ʏ�Ӱ��
                    ; bit30    : MRSET     0   �S��ݸ��������
                    ; bit29-27 : TRC2-0    010
                    ;       Ref/Active to Ref/Active command period : 6-cycle(108nS)
                    ; bit26-24 : Reserved  000
                    ; bit23    : TCAS      0   SDRAM���͖��g�p
                    ; bit22    : Reserved  0
                    ; bit21-19 : TPC2-0    010
                    ;       Precharge to active command period : 3-cycle(54nS)
                    ; bit18    : Reserved  0
                    ; bit17-16 : RCD1-0    01
                    ;       Active command to column command(same bank) : 2-cycle(36nS)
                    ; bit15-13 : TWRL2-0   000
                    ;   Write recovery or data-in to precharge lead time : 1-cycle(18nS)
                    ; bit12-10 : TRAS2-0   000
                    ;   Ref/Active to Ref/Active command period : 10-cycle(180nS)
                    ; bit 9    : BE        0   SDRAM���͖��g�p
                    ; bit 8-7  : SZ1-0     11  Memory data size : 32bit
                    ; bit 6    : AMXEXT    0   SDRAM interface
                    ; bit 5- 3 : AMX2-0    010 (64M * 2)
                    ; bit 2    : RFSH      1   Refresh enable
                    ; bit 1    : RMODE     0   Refresh mode         : Auto refresh
                    ; bit 0    : EDOMODE   0   SDRAM���͖��g�p

            .data.l H'50110194
                    ; bit31    : RASD      0   �ʏ�Ӱ��
                    ; bit30    : MRSET     1   Ӱ��ڼ޽��ݒ�
                    ; bit29-27 : TRC2-0    010
                    ;       Ref/Active to Ref/Active command period : 6-cycle(108nS)
                    ; bit26-24 : Reserved  000
                    ; bit23    : TCAS      0   SDRAM���͖��g�p
                    ; bit22    : Reserved  0
                    ; bit21-19 : TPC2-0    010
                    ;       Precharge to active command period : 3-cycle(54nS)
                    ; bit18    : Reserved  0
                    ; bit17-16 : RCD1-0    01
                    ;       Active command to column command(same bank) : 2-cycle(36nS)
                    ; bit15-13 : TWRL2-0   000
                    ;   Write recovery or data-in to precharge lead time : 1-cycle(18nS)
                    ; bit12-10 : TRAS2-0   000
                    ;   Ref/Active to Ref/Active command period : 10-cycle(180nS)
                    ; bit 9    : BE        0   SDRAM���͖��g�p
                    ; bit 8-7  : SZ1-0     11  Memory data size : 32bit
                    ; bit 6    : AMXEXT    0   SDRAM interface
                    ; bit 5- 3 : AMX2-0    010 (64M * 2)
                    ; bit 2    : RFSH      1   Refresh enable
                    ; bit 1    : RMODE     0   Refresh mode         : Auto refresh
                    ; bit 0    : EDOMODE   0   SDRAM���͖��g�p

CCR_D:      .data.l H'00000808
    .aif CACHE_WT eq 0
            .data.l H'00000105
    .aelse
            .data.l H'00000103
    .aendi
CCR_ICI:    .data.l H'00000800

HDRLEN:     .data.l __KERNEL_hdrlen

PCTRA_D:    .data.l H'00000000
                    ; bit31    : PB15PUP   0   PORT15 pull-up
                    ; bit30    : PB15IO    0   PORT15 input
                    ; bit29    : PB14PUP   0   PORT14 pull-up
                    ; bit28    : PB14IO    0   PORT14 input
                    ; bit27    : PB13PUP   0   PORT13 pull-up
                    ; bit26    : PB13IO    0   PORT13 input
                    ; bit25    : PB12PUP   0   PORT12 pull-up
                    ; bit24    : PB12IO    0   PORT12 input
                    ; bit23    : PB11PUP   0   PORT11 pull-up
                    ; bit22    : PB11IO    0   PORT11 input
                    ; bit21    : PB10PUP   0   PORT10 pull-up
                    ; bit20    : PB10IO    0   PORT10 input
                    ; bit19    : PB9PUP    0   PORT9 pull-up
                    ; bit18    : PB9IO     0   PORT9 input
                    ; bit17    : PB8PUP    0   PORT8 pull-up
                    ; bit16    : PB8IO     0   PORT8 input
                    ; bit15    : PB7PUP    0   PORT7 pull-up
                    ; bit14    : PB7IO     0   PORT7 input
                    ; bit13    : PB6PUP    0   PORT6 pull-up
                    ; bit12    : PB6IO     0   PORT6 input
                    ; bit11    : PB5PUP    0   PORT5 pull-up
                    ; bit10    : PB5IO     0   PORT5 input
                    ; bit 9    : PB4PUP    0   PORT4 pull-up
                    ; bit 8    : PB4IO     0   PORT4 input
                    ; bit 7    : PB3PUP    0   PORT3 pull-up
                    ; bit 6    : PB3IO     0   PORT3 input
                    ; bit 5    : PB2PUP    0   PORT2 pull-up
                    ; bit 4    : PB2IO     0   PORT2 input
                    ; bit 3    : PB1PUP    0   PORT1 pull-up
                    ; bit 2    : PB1IO     0   PORT1 input
                    ; bit 1    : PB0PUP    0   PORT0 pull-up
                    ; bit 0    : PB0IO     0   PORT0 input

PCTRB_D:    .data.l H'00000000
                    ; bit31    : PB31PUP   0   PORT31 pull-up
                    ; bit30    : PB31IO    0   PORT31 input
                    ; bit29    : PB30PUP   0   PORT30 pull-up
                    ; bit28    : PB30IO    0   PORT30 input
                    ; bit27    : PB29PUP   0   PORT29 pull-up
                    ; bit26    : PB29IO    0   PORT29 input
                    ; bit25    : PB28PUP   0   PORT28 pull-up
                    ; bit24    : PB28IO    0   PORT28 input
                    ; bit23    : PB27PUP   0   PORT27 pull-up
                    ; bit22    : PB27IO    0   PORT27 input
                    ; bit21    : PB26PUP   0   PORT26 pull-up
                    ; bit20    : PB26IO    0   PORT26 input
                    ; bit19    : PB25PUP   0   PORT25 pull-up
                    ; bit18    : PB25IO    0   PORT25 input
                    ; bit17    : PB24PUP   0   PORT24 pull-up
                    ; bit16    : PB24IO    0   PORT24 input
                    ; bit15    : PB23PUP   0   PORT23 pull-up
                    ; bit14    : PB23IO    0   PORT23 input
                    ; bit13    : PB22PUP   0   PORT22 pull-up
                    ; bit12    : PB22IO    0   PORT22 input
                    ; bit11    : PB21PUP   0   PORT21 pull-up
                    ; bit10    : PB21IO    0   PORT21 input
                    ; bit 9    : PB20PUP   0   PORT20 pull-up
                    ; bit 8    : PB20IO    0   PORT20 input
                    ; bit 7    : PB19PUP   0   PORT19 pull-up
                    ; bit 6    : PB19IO    0   PORT19 input
                    ; bit 5    : PB18PUP   0   PORT18 pull-up
                    ; bit 4    : PB18IO    0   PORT18 input
                    ; bit 3    : PB17PUP   0   PORT17 pull-up
                    ; bit 2    : PB17IO    0   PORT17 input
                    ; bit 1    : PB16PUP   0   PORT16 pull-up
                    ; bit 0    : PB16IO    0   PORT16 input

BCR2_D:     .data.w H'2FFC
                    ; bit15-14 : A0SZ1-0   00  MD3,MD4 level monitor(Read only)
                    ; bit13-12 : A6SZ1-0   10  CS6 area 16bit bus size
                    ; bit11-10 : A5SZ1-0   11  CS5 area 32bit bus size
                    ; bit 9- 8 : A4SZ1-0   11  CS4 area 32bit bus size
                    ; bit 7- 6 : A3SZ1-0   11  CS3 area 32bit bus size
                    ; bit 5- 4 : A2SZ1-0   11  CS2 area 32bit bus size
                    ; bit 3- 2 : A1SZ1-0   11  CS1 area 32bit bus size
                    ; bit 1    : Reserved  0
                    ; bit 0    : PORTEN    0   Port not used


PCR_D:      .data.w H'0000
                    ; bit15-14 : A5PCW1-0  00  CS5 area PCMCIA wait  : 0-cycle
                    ; bit13-12 : A6PCW1-0  00  CS6 area PCMCIA wait  : 0-cycle
                    ; bit11- 9 : A5TED2-0  000
                    ;   CS5 area ADDRESS-OE/WE latency time : 0-cycle
                    ; bit 8- 6 : A6TED2-0  000
                    ;   CS6 area ADDRESS-OE/WE latency time : 0-cycle
                    ; bit 5- 3 : A5TEH2-0  000
                    ;   CS5 area OE/WE-ADDRESS hold time    : 0-cycle
                    ; bit 2- 0 : A6TEH2-0  000
                    ;   CS6 area OE/WE-ADDRESS hold time    : 0-cycle

RTCNT_D:    .data.w H'A500
                    ; RTCNT clear data

RTCOR_D:    .data.w H'A561
                    ; Refresh cycle

RTCSR_D:    .data.w H'a508
                    ; bit15- 8 : Reserved  00000000 Write code h'a5
                    ; bit 7    : CMF  0   Compare match flug                   : Clear
                    ; bit 6    : CMIE 0   Compare match interrupt              : Disable
                    ; bit 5- 3 : CKS2-0   001 Count clock                      : CKIO/4
                    ; bit 2    : OVF  0   Refresh count over flow flag         : Clear
                    ; bit 1    : OVIE 0   Refresh count over flow interrupt    : Disable
                    ; bit 0    : LMTS 0   Refresh count over flow limit select : 1024

            .data.w H'A518
                    ; bit15- 8 : Reserved  00000000 Write code h'a5
                    ; bit 7    : CMF  0   Compare match flug                   : Clear
                    ; bit 6    : CMIE 0   Compare match interrupt              : Disable
                    ; bit 5- 3 : CKS2-0   011 Count clock                      : CKIO/64
                    ; bit 2    : OVF  0   Refresh count over flow flag         : Clear
                    ; bit 1    : OVIE 0   Refresh count over flow interrupt    : Disable
                    ; bit 0    : LMTS 0   Refresh count over flow limit select : 1024

FRQCR_D     .data.w H'0E13
                    ; PLL1 Enabled
                    ; PLL2 Enabled
                    ; IFC = 000 CPU clk division ratio =1
                    ; BFC = 010 Bus clk division ratio =1/3
                    ; PFC = 011 Peripheral clk division ratio = 1/6.

RFCR_D:     .data.w H'A400
                    ; RFCR clear data

GPIOIC_D:   .data.w H'0000
                    ; bit15    : PTIREN15  0   PORT15 I/O port
                    ; bit14    : PTIREN14  0   PORT14 I/O port
                    ; bit13    : PTIREN13  0   PORT13 I/O port
                    ; bit12    : PTIREN12  0   PORT12 I/O port
                    ; bit11    : PTIREN11  0   PORT11 I/O port
                    ; bit10    : PTIREN10  0   PORT10 I/O port
                    ; bit 9    : PTIREN9   0   PORT9 I/O port
                    ; bit 8    : PTIREN8   0   PORT8 I/O port
                    ; bit 7    : PTIREN7   0   PORT7 I/O port
                    ; bit 6    : PTIREN6   0   PORT6 I/O port
                    ; bit 5    : PTIREN5   0   PORT5 I/O port
                    ; bit 4    : PTIREN4   0   PORT4 I/O port
                    ; bit 3    : PTIREN3   0   PORT3 I/O port
                    ; bit 2    : PTIREN2   0   PORT2 I/O port
                    ; bit 1    : PTIREN1   0   PORT1 I/O port
                    ; bit 0    : PTIREN0   0   PORT0 I/O port

;********* Memory map for SH7751SE01 evaluation board **************************
;           CS0(32bit)
;               h'00000000 - h'003fffff : EPROM area    (4MB)
;               h'01000000 - h'013fffff : Flash ROM area(4MB)
;               h'02000000 - h'03ffffff : External area (32MB)
;           CS1(32bit)
;               h'04000000 - h'07ffffff : External area(64MB)
;           CS2(32bit)
;               h'08000000 - h'0bffffff : External area(64MB)
;           CS3(32bit)
;               h'0c000000 - h'0fffffff : SDRAM area(64MB)
;           CS4(32bit)
;               h'10000000 - h'13ffffff : External area(64MB)
;           CS5(32bit)
;               h'14000000 - h'17ffffff ; External area(64MB)
;           CS6(16bit)
;               h'18000000 - h'18ffffff ; MR-SHPC-01 area
;               h'19000000 - h'19ffffff : Global SW area
;               h'1a000000 - h'1affffff : Debug LED
;               h'1b000000 - h'1bffffff : Board control register area
;*******************************************************************************
;* �e�Z�N�V�����̃A�h���X

        .section P,CODE,ALIGN=4         ; �v���O�����̈�iROM�j
        .section D,DATA,ALIGN=4         ; �������f�[�^�̈�iROM�j
        .section R,DATA,ALIGN=4         ; �������f�[�^�̈�iRAM�j
        .section B,DATA,ALIGN=4         ; ���������f�[�^�̈�iRAM�j
        .section STACK,DATA,ALIGN=4     ; �X�^�b�N�̈�iRAM�j
        .res.w  1
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
