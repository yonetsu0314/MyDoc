;******************************************************************************
;* ���l�T�XC�p�x�N�^�e�[�u���ݒ�̗� (MiNET-7042�{�[�h)                       *
;*                                                                            *
;*  File name : vec7042h.asm                                                  *
;*                                                                            *
;* 27/Feb/1998 vec7040.asm�Ƃ��č쐬                                 MiSPO MI *
;* 16/Jan/2000 �����[�g�f�o�b�K�Ŏg����悤VECT��LOCATE=0���O��            MI *
;* 07/Mar/2001 MiNET-7042�p�̏�������ǉ�, SH7044����Flash�p�������Ή�     MI *
;* 08/Mar/2001 CS0(ROM),CS1(RAM)�̈�̃L���b�V��ON��ǉ�                   MI *
;* 25/Dec/2002  ROMICE�p��sp�ݒ���@���C��                                 KA *
;******************************************************************************

;* -def=_INIT_STACK="H'??????" ��SP�����l���w�肵�ăA�Z���u�����Ă��������B
;* �n�[�h�E�F�A�ɍ��킹�āA���̉ӏ����J�X�^�}�C�Y���Ă�������

;* �L���b�V���̗L��/������ݒ肵�܂��B

CACHE_ON: .define "1"               ; 0:����, 1:�L�� ��

;* �O���Q��

        .IMPORT __INIT              ; �����ݒ胋�[�`��

;* �����݃x�N�^

        .SECTION VECT,DATA          ; VECT�Z�N�V������ROM����0�Ԓn�Ƀ����N���邱��
        .export _RST_VECT
_RST_VECT:
        .DATA.L _RESET              ; �p���[�I�����Z�b�g PC�����l
        .DATA.L _INIT_STACK         ; �p���[�I�����Z�b�g SP�����l
        .DATA.L _RESET              ; �}�j���A�����Z�b�g PC�����l
        .DATA.L _INIT_STACK         ; �}�j���A�����Z�b�g SP�����l

        ; �����݃x�N�^��RAM��ɍĔz�u����O��̂��߁A�ȍ~�̃x�N�^�͒�`���܂�
        ; ��Bdef_inh �V�X�e���R�[����p�����A���ڃx�N�^��ROM�ɒ�`����ꍇ��
        ; �����ɏ��������Ă��������B

;* ���Z�b�g����

        .section P,CODE,ALIGN=4
        .export _RESET
_RESET:
       ; �o�X�R���g���[���̏�����

        mov.w   WCR1,r0
        mov.w   WCR1_DATA,r1
        mov.w   r1,@r0

        mov.w   WCR2,r0
        mov.w   WCR2_DATA,r1
        mov.w   r1,@r0

        mov.w   BCR1,r0
        mov.w   BCR1_DATA,r1
        mov.w   r1,@r0

        mov.w   BCR2,r0
        mov.w   BCR2_DATA,r1
        mov.w   r1,@r0

        ; �|�[�gA��������

        mov.w   PAIORH,r0
        mov.w   PAIORH_DATA,r1
        mov.w   r1,@r0

        mov.w   PAIORL,r0
        mov.w   PAIORL_DATA,r1
        mov.w   r1,@r0

        mov.w   PACRH,r0
        mov.w   PACRH_DATA,r1
        mov.w   r1,@r0

        mov.w   PACRL1,r0
        mov.w   PACRL1_DATA,r1
        mov.w   r1,@r0

        mov.w   PACRL2,r0
        mov.w   PACRL2_DATA,r1
        mov.w   r1,@r0

        ; �|�[�gB��������

        mov.w   PBIOR,r0
        mov.w   PBIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PBCR1,r0
        mov.w   PBCR1_DATA,r1
        mov.w   r1,@r0

        mov.w   PBCR2,r0
        mov.w   PBCR2_DATA,r1
        mov.w   r1,@r0

        ; �|�[�gC��������

        mov.w   PCIOR,r0
        mov.w   PCIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PCCR,r0
        mov.w   PCCR_DATA,r1
        mov.w   r1,@r0

        ; �|�[�gD��������

        mov.w   PDIORH,r0
        mov.w   PDIORH_DATA,r1
        mov.w   r1,@r0

        mov.w   PDIORL,r0
        mov.w   PDIORL_DATA,r1
        mov.w   r1,@r0

        mov.w   PDCRH1,r0
        mov.w   PDCRH1_DATA,r1
        mov.w   r1,@r0

        mov.w   PDCRH2,r0
        mov.w   PDCRH2_DATA,r1
        mov.w   r1,@r0

        mov.w   PDCRL,r0
        mov.w   PDCRL_DATA,r1
        mov.w   r1,@r0

        ; �|�[�gE��������

        mov.w   PEIOR,r0
        mov.w   PEIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PECR1,r0
        mov.w   PECR1_DATA,r1
        mov.w   r1,@r0

        mov.w   PECR2,r0
        mov.w   PECR2_DATA,r1
        mov.w   r1,@r0

        ; IRQOUT�@�\�R���g���[�����W�X�^��������

        mov.w   IFCR,r0
        mov.w   IFCR_DATA,r1
        mov.w   r1,@r0

        ; �L���b�V�����C�l�[�u���ɂ���

    .aif CACHE_ON ne 0
        mov.w   H_F000,r1           ; FFFFF000�`FFFFF3FF��0�N���A
        mov.w   H_F400,r2
        mov     #0,r0
L2:
        mov.l   r0,@r1
        add     #4,r1
        cmp/hs  r2,r1
        bf      L2
        mov     #3,r1               ; CECS1 = 1, CECS0 = 1
        mov.w   CCR,r0
        mov.w   r1,@r0
    .aendi

        ; �X�^�b�N�|�C���^���\�t�g�ł��Đݒ肵�A���������[�`���֔�ԁB

        mov.l   INIT,r2
        mov.l   INIT_STACK,sp
        jmp     @r2
        nop

;* ���W�X�^�����ݒ�l ��

BCR1_DATA:      .data.w h'200f  ; �o�X�R���g���[�����W�X�^1
                                ; MTURWE=1 MTU�A�N�Z�X����
                                ; IOE=0  CS3��Ԃ͒ʏ���
                                ; A3LG=0 A3SZ�l�ɏ]��
                                ; A2LG=0 A2SZ�l�ɏ]��
                                ; A1LG=0 A1SZ�l�ɏ]��
                                ; A0LG=0 A0SZ�l�ɏ]��
                                ; A3SZ=1 CS3���16�r�b�g(I/O,MB86964)
                                ; A2SZ=1 CS2���16�r�b�g(Flash)
                                ; A1SZ=1 CS1���16�r�b�g(SRAM)
                                ; A0SZ=1 CS0���16�r�b�g(ROM)

BCR2_DATA:      .data.w h'115f  ; �o�X�R���g���[�����W�X�^2
                                ; IW31,30=00 CS3��� 0�A�C�h���T�C�N��
                                ; IW21,20=01 CS2��� 1�A�C�h���T�C�N��
                                ; IW11,10=00 CS1��� 0�A�C�h���T�C�N��
                                ; IW01,00=01 CS0��� 1�A�C�h���T�C�N��
                                ; CW3=0 CS3��ԘA���A�N�Z�X���A�C�h���Ȃ�
                                ; CW2=1 CS2��ԘA���A�N�Z�X���A�C�h������
                                ; CW1=0 CS1��ԘA���A�N�Z�X���A�C�h���Ȃ�
                                ; CW0=1 CS0��ԘA���A�N�Z�X���A�C�h������
                                ; SW3=1 CS3���CS�A�T�[�g�g������
                                ; SW2=1 CS2���CS�A�T�[�g�g������
                                ; SW1=1 CS1���CS�A�T�[�g�g������
                                ; SW0=1 CS0���CS�A�T�[�g�g������


WCR1_DATA:      .data.w h'1102  ; �E�F�C�g�R���g���[�����W�X�^1
                                ; W33-30=0001 CS3��� 1�E�F�C�g
                                ; W23-20=0001 CS2��� 1�E�F�C�g
                                ; W13-10=0000 CS1��� 0�E�F�C�g
                                ; W03-00=0010 CS0��� 2�E�F�C�g

WCR2_DATA:      .data.w h'000f  ; �E�F�C�g�R���g���[�����W�X�^2
                                ; DDW1,0=00  (DMA���g�p,�����l�̂܂�)
                                ; DSW3-0=1111(DMA���g�p,�����l�̂܂�)

PAIORH_DATA:    .data.w h'0000  ; �|�[�gA IO���W�X�^H
PAIORL_DATA:    .data.w h'0012  ; �|�[�gA IO���W�X�^L
                                ; PA15IOR=0 CK(�V�X�e���N���b�N�o��)
                                ; PA14IOR=0 RD
                                ; PA13IOR=0 WRH
                                ; PA12IOR=0 WRL
                                ; PA11IOR=0 CS1
                                ; PA10IOR=0 CS0
                                ; PA9IOR =0 IRQ3(SIO_IRQ,���f�������������)
                                ; PA8IOR =0 IRQ2(DIPSW������)
                                ; PA7IOR =0 CS3
                                ; PA6IOR =0 CS2
                                ; PA5IOR =0 IRQ1(CTS1����)
                                ; PA4IOR =1 TXD1(Break�o�͎���PA4�Ƃ���)
                                ; PA3IOR =0 RXD1
                                ; PA2IOR =0 IRQ0(CTS0����)
                                ; PA1IOR =1 TXD0(Break�o�͎���PA1�Ƃ���)
                                ; PA0IOR =0 RXD0

PACRH_DATA:     .data.w h'5000  ; �|�[�gA�R���g���[�����W�X�^H
                                ; PA23MD= 1 WRHH (SH7042/44�ł͖���)
                                ; PA22MD= 1 WRHL (SH7042/44�ł͖���)
                                ; PA21MD=00 PA21 (SH7042/44�ł͖���)
                                ; PA20MD=00 PA20 (SH7042/44�ł͖���)
                                ; PA19MD=00 PA19 (SH7042/44�ł͖���)
                                ; PA18MD=00 PA18 (SH7042/44�ł͖���)
                                ; PA17MD=00 PA17 (SH7042/44�ł͖���)
                                ; PA16MD=00 PA16 (SH7042/44�ł͖���)

PACRL1_DATA:    .data.w h'555a  ; �|�[�gA�R���g���[�����W�X�^L1
                                ; PA15MD= 1 CK(�V�X�e���N���b�N�o��)
                                ; PA14MD= 1 RD
                                ; PA13MD= 1 WRH
                                ; PA12MD= 1 WRL
                                ; PA11MD= 1 CS1
                                ; PA10MD= 1 CS0
                                ; PA9MD =10 IRQ3(SIO_IRQ,���f�������������)
                                ; PA8MD =10 IRQ2(DIPSW������)

PACRL2_DATA:    .data.w h'ad75  ; �|�[�gA�R���g���[�����W�X�^L2
                                ; PA7MD=10 CS3
                                ; PA6MD=10 CS2
                                ; PA5MD=11 IRQ1(CTS1����)
                                ; PA4MD= 1 TXD1
                                ; PA3MD= 1 RXD1
                                ; PA2MD=11 IRQ0(CTS0����)
                                ; PA1MD= 1 TXD0
                                ; PA0MD= 1 RXD0

PBIOR_DATA:     .data.w h'0034  ; �|�[�gB IO���W�X�^
                                ; PB9IOR=0 IRQ7(MAX3100 IRQ����)
                                ; PB8IOR=0 WAIT
                                ; PB7IOR=0 A19
                                ; PB6IOR=0 A18
                                ; PB5IOR=1 PB5(MAX3100 CS�o��)
                                ; PB4IOR=1 PB4(MAX3100 SCLK�o��)
                                ; PB3IOR=0 PB3(MAX3100 DOUT����)
                                ; PB2IOR=1 PB2(MAX3100 DIN�o��)
                                ; PB1IOR=0 A17
                                ; PB0IOR=0 A16


PBCR1_DATA:     .data.w h'0007  ; �|�[�gB�R���g���[�����W�X�^1
                                ; PB9MD=01 IRQ7(MAX3100 IRQ����)
                                ; PB8MD=11 WAIT

PBCR2_DATA:     .data.w h'a005  ; �|�[�gB�R���g���[�����W�X�^2
                                ; PB7MD=10 A19
                                ; PB6MD=10 A18
                                ; PB5MD=00 PB5(MAX3100 CS�o��)
                                ; PB4MD=00 PB4(MAX3100 SCLK�o��)
                                ; PB3MD=00 PB3(MAX3100 DOUT����)
                                ; PB2MD=00 PB2(MAX3100 DIN�o��)
                                ; -----= 0
                                ; PB1MD= 1 A17
                                ; -----= 0
                                ; PB0MD= 1 A16

PCIOR_DATA:     .data.w h'0000  ; �|�[�gC IO���W�X�^
PCCR_DATA:      .data.w h'ffff  ; �|�[�gC �R���g���[�����W�X�^
                                ; PC15MD-PC0MD=1  A15-A0

PDIORH_DATA:    .data.w h'0000  ; �|�[�gD IO���W�X�^H
PDIORL_DATA:    .data.w h'0000  ; �|�[�gD IO���W�X�^L
PDCRH1_DATA:    .data.w h'5555  ; �|�[�gD �R���g���[�����W�X�^H1
                                ; PD31MD-PD24MD=01 D31-D24 (SH7042/44�ł͖���)
PDCRH2_DATA:    .data.w h'5555  ; �|�[�gD �R���g���[�����W�X�^H2
                                ; PD23MD-PD16MD=01 D23-D16 (SH7042/44�ł͖���)
PDCRL_DATA:     .data.w h'ffff  ; �|�[�gD �R���g���[�����W�X�^L
                                ; PD15MD-PD0MD=1 D15-D0

PEIOR_DATA:     .data.w h'ffea  ; �|�[�gE IO���W�X�^
                                ; PE15IOR=1 PE15(LED E8�o��)
                                ; PE14IOR=1 PE14(LED E7�o��, LAN DACK0�Ƃ��Ă͖��g�p)
                                ; PE13IOR=1 PE13(LED E6�o��)
                                ; PE12IOR=1 PE12(LED E5�o��)
                                ; PE11IOR=1 PE11(LED E4�o��)
                                ; PE10IOR=1 PE10(LED E3�o��)
                                ; PE9IOR =1 PE9 (LED E2�o��)
                                ; PE8IOR =1 PE8 (LED E1�o��)
                                ; PE7IOR =1 PE7 (RTC4513 CE�o��)
                                ; PE6IOR =1 PE6 (RTC4513 CLK�o��)
                                ; PE5IOR =1 PE5 (X24C16 SCL�o��)
                                ; PE4IOR =0 PE4 (X24C16 SDA���o��, RTC4513 DATA���o��)
                                ; PE3IOR =1 PE3 (RTS1�o��)
                                ; PE2IOR =0 PE2 (���g�p)
                                ; PE1IOR =1 PE1 (RTS0�o��)
                                ; PE0IOR =0 PE0 (LAN DREQ0�Ƃ��Ă͖��g�p)

PECR1_DATA:     .data.w h'0000  ; �|�[�gE �R���g���[�����W�X�^1
                                ; PE15MD=00 PE15(LED E8�o��)
                                ; PE14MD=00 PE14(LED E7�o��, LAN DACK0�Ƃ��Ă͖��g�p)
                                ; PE13MD=00 PE13(LED E6�o��)
                                ; PE12MD= 0 PE12(LED E5�o��)
                                ; PE11MD= 0 PE11(LED E4�o��)
                                ; PE10MD= 0 PE10(LED E3�o��)
                                ; PE9MD = 0 PE9 (LED E2�o��)
                                ; PE8MD = 0 PE8 (LED E1�o��)

PECR2_DATA:     .data.w h'0000  ; �|�[�gE �R���g���[�����W�X�^2
                                ; PE7MD = 0 PE7 (RTC4513 CE�o��)
                                ; PE6MD = 0 PE6 (RTC4513 CLK�o��)
                                ; PE5MD = 0 PE5 (X24C16 SCL�o��)
                                ; PE4MD =00 PE4 (X24C16 SDA���o��, RTC4513 DATA���o��)
                                ; PE3MD =00 PE3 (RTS1�o��)
                                ; PE2MD =00 PE2 (���g�p)
                                ; PE1MD =00 PE1 (RTS0�o��)
                                ; PE0MD =00 PE0 (LAN DREQ0�Ƃ��Ă͖��g�p)

IFCR_DATA:      .data.w h'0000  ; IRQOUT�@�\�R���g���[�����W�X�^
                                ; (IRQOUT�@�\���g�p)

;* ���W�X�^�A�h���X(����16�r�b�g)

BCR1:           .data.w h'8620
BCR2:           .data.w h'8622
WCR1:           .data.w h'8624
WCR2:           .data.w h'8626
PAIORH:         .data.w h'8384
PAIORL:         .data.w h'8386
PACRH:          .data.w h'8388
PACRL1:         .data.w h'838c
PACRL2:         .data.w h'838e
PBIOR:          .data.w h'8394
PBCR1:          .data.w h'8398
PBCR2:          .data.w h'839a
PCIOR:          .data.w h'8396
PCCR:           .data.w h'839c
PDIORH:         .data.w h'83a4
PDIORL:         .data.w h'83a6
PDCRH1:         .data.w h'83a8
PDCRH2:         .data.w h'83aa
PDCRL:          .data.w h'83ac
PEIOR:          .data.w h'83b4
PECR1:          .data.w h'83b8
PECR2:          .data.w h'83ba
IFCR:           .data.w h'83c8
CCR:            .data.w h'8740

;* �Q�ƃA�h���X/�f�[�^�l

        .align  4
INIT_STACK:     .data.l  _INIT_STACK
INIT:           .data.l  __INIT
H_F000:         .data.w  h'f000
H_F400:         .data.w  h'f400

;* �e�Z�N�V�����̃A�h���X

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
