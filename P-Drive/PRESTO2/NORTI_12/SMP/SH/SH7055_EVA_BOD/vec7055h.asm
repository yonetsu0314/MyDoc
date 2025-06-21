;****************************************************************************
;* SHC�x�N�^�e�[�u���ݒ�̗�iSH7055 EVA BOD)                               *
;*                                                                          *
;*  File name : vec7055h.asm                                                *
;*                                                                          *
;* 05/Feb/2001                                                              *
;****************************************************************************

        .IMPORT __INIT              ; �����ݒ胋�[�`��

;* �����݃x�N�^

        .SECTION VECT,DATA          ; VECT�Z�N�V������0�Ԓn�Ƀ����N���邱��
        .export _RST_VECT
_RST_VECT:
        .data.l (STARTOF PON)-(STARTOF VECT); �p���[�I�����Z�b�g PC �����l ��
        .data.l _INIT_STACK                 ; �p���[�I�����Z�b�g SP �����l ��
        .data.l (STARTOF PON)-(STARTOF VECT); �}�j���A�����Z�b�g PC �����l ��
        .data.l _INIT_STACK                 ; �}�j���A�����Z�b�g SP �����l ��

        ; �����݃x�N�^�� RAM ��ɍĔz�u����O��̂��߁A�ȍ~�̃x�N�^�͒�`��
        ; �܂���B
        ; def_inh �V�X�e���R�[����p�����A���ڃx�N�^�� ROM �ɒ�`����ꍇ�A
        ; �����֏��������Ă��������B

;* ���Z�b�g����

        .section PON,CODE,ALIGN=4
        .export _RESET
_RESET:
        ; ���X�^�[�g���l�����Ċ��荞�݃}�X�N���x�����ō���

        stc     sr,r0
        or      #H'f0,r0
        ldc     r0,sr

        ; �K�v�ȏ��������L�q���Ă��������B

        mov.w   BCR1,r0
        mov.w   BCR1_DATA,r1
        mov.w   r1,@r0          ;�a�b�q�P
        mov.w   BCR2,r0
        mov.w   BCR2_DATA,r1
        mov.w   r1,@r0          ;�a�b�q�Q
        mov.w   WCR,r0
        mov.w   WCR_DATA,r1
        mov.w   r1,@r0          ;�v�b�q
        mov.w   PCCR,r0
        mov.w   PCCR_DATA,r1
        mov.w   r1,@r0          ;�o�b�b�q
        mov.w   PECR,r0
        mov.w   PECR_DATA,r1
        mov.w   r1,@r0          ;�o�d�b�q
        mov.w   PFCRH,r0
        mov.w   PFCRH_DATA,r1
        mov.w   r1,@r0          ;�o�e�b�q�g
        mov.w   PFCRL,r0
        mov.w   PFCRL_DATA,r1
        mov.w   r1,@r0          ;�o�e�b�q�k
        mov.w   PHCR,r0
        mov.w   PHCR_DATA,r1
        mov.w   r1,@r0          ;�o�g�b�q

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

        ; �X�^�b�N�|�C���^���\�t�g�ł��Đݒ肵�A���������[�`���֔�ԁB

        mov.l   INIT,r0
        mov.l   INIT_STACK,sp       ; �X�^�b�N�|�C���^������
        jmp     @r0
        nop

        .ALIGN  4
INIT_STACK: .DATA.L _INIT_STACK     ; �p���[�I�����Z�b�g SP �����l ��
INIT:       .DATA.L __INIT

COPY_BGN    .data.l     (STARTOF VECT)
COPY_SIZE   .data.l     (STARTOF D)+(SIZEOF D)-(STARTOF VECT)
COPY_ORG    .data.l     (STARTOF VECT)-(STARTOF PON)-(copy_s-_RESET)

BCR1:       .DATA.W H'ec20
BCR2:       .DATA.W H'ec22
WCR:        .DATA.W H'ec24
PCCR        .DATA.W H'f73c
PECR:       .DATA.W H'f752
PFCRH:      .DATA.W H'f74a
PFCRL:      .DATA.W H'f74c
PHCR:       .DATA.W H'f72a

BCR1_DATA:  .DATA.W H'000f  ;�ر0�`3=16bit
;BCR2_DATA:  .DATA.W H'55ff
;WCR_DATA:   .DATA.W H'0012
BCR2_DATA:  .DATA.W H'f1dd  ;�ر0�`3=3����ٻ���
WCR_DATA:   .DATA.W H'ff02  ;�����l

PCCR_DATA:  .DATA.W H'0155  ;�߰�C���۰�ڼ޽�
PECR_DATA:  .DATA.W H'ffff  ;�߰�E���۰�ڼ޽�
PFCRH_DATA: .DATA.W H'5555  ;�߰�F���۰�ڼ޽�H
PFCRL_DATA: .DATA.W H'5555  ;�߰�F���۰�ڼ޽�L
PHCR_DATA:  .DATA.W H'ffff  ;�߰�H���۰�ڼ޽�


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
