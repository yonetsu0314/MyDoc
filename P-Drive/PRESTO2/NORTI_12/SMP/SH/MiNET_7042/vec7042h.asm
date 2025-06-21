;******************************************************************************
;* ルネサスC用ベクタテーブル設定の例 (MiNET-7042ボード)                       *
;*                                                                            *
;*  File name : vec7042h.asm                                                  *
;*                                                                            *
;* 27/Feb/1998 vec7040.asmとして作成                                 MiSPO MI *
;* 16/Jan/2000 リモートデバッガで使えるようVECTのLOCATE=0を外す            MI *
;* 07/Mar/2001 MiNET-7042用の初期化を追加, SH7044内蔵Flash用初期化対応     MI *
;* 08/Mar/2001 CS0(ROM),CS1(RAM)領域のキャッシュONを追加                   MI *
;* 25/Dec/2002  ROMICE用にsp設定方法を修正                                 KA *
;******************************************************************************

;* -def=_INIT_STACK="H'??????" でSP初期値を指定してアセンブルしてください。
;* ハードウェアに合わせて、★の箇所をカスタマイズしてください

;* キャッシュの有効/無効を設定します。

CACHE_ON: .define "1"               ; 0:無効, 1:有効 ★

;* 外部参照

        .IMPORT __INIT              ; 初期設定ルーチン

;* 割込みベクタ

        .SECTION VECT,DATA          ; VECTセクションはROM化時0番地にリンクすること
        .export _RST_VECT
_RST_VECT:
        .DATA.L _RESET              ; パワーオンリセット PC初期値
        .DATA.L _INIT_STACK         ; パワーオンリセット SP初期値
        .DATA.L _RESET              ; マニュアルリセット PC初期値
        .DATA.L _INIT_STACK         ; マニュアルリセット SP初期値

        ; 割込みベクタをRAM上に再配置する前提のため、以降のベクタは定義しませ
        ; ん。def_inh システムコールを用いず、直接ベクタをROMに定義する場合は
        ; ここに書き加えてください。

;* リセット処理

        .section P,CODE,ALIGN=4
        .export _RESET
_RESET:
       ; バスコントローラの初期化

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

        ; ポートAを初期化

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

        ; ポートBを初期化

        mov.w   PBIOR,r0
        mov.w   PBIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PBCR1,r0
        mov.w   PBCR1_DATA,r1
        mov.w   r1,@r0

        mov.w   PBCR2,r0
        mov.w   PBCR2_DATA,r1
        mov.w   r1,@r0

        ; ポートCを初期化

        mov.w   PCIOR,r0
        mov.w   PCIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PCCR,r0
        mov.w   PCCR_DATA,r1
        mov.w   r1,@r0

        ; ポートDを初期化

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

        ; ポートEを初期化

        mov.w   PEIOR,r0
        mov.w   PEIOR_DATA,r1
        mov.w   r1,@r0

        mov.w   PECR1,r0
        mov.w   PECR1_DATA,r1
        mov.w   r1,@r0

        mov.w   PECR2,r0
        mov.w   PECR2_DATA,r1
        mov.w   r1,@r0

        ; IRQOUT機能コントロールレジスタを初期化

        mov.w   IFCR,r0
        mov.w   IFCR_DATA,r1
        mov.w   r1,@r0

        ; キャッシュをイネーブルにする

    .aif CACHE_ON ne 0
        mov.w   H_F000,r1           ; FFFFF000～FFFFF3FFを0クリア
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

        ; スタックポインタをソフトでも再設定し、初期化ルーチンへ飛ぶ。

        mov.l   INIT,r2
        mov.l   INIT_STACK,sp
        jmp     @r2
        nop

;* レジスタ初期設定値 ★

BCR1_DATA:      .data.w h'200f  ; バスコントロールレジスタ1
                                ; MTURWE=1 MTUアクセス許可
                                ; IOE=0  CS3空間は通常空間
                                ; A3LG=0 A3SZ値に従う
                                ; A2LG=0 A2SZ値に従う
                                ; A1LG=0 A1SZ値に従う
                                ; A0LG=0 A0SZ値に従う
                                ; A3SZ=1 CS3空間16ビット(I/O,MB86964)
                                ; A2SZ=1 CS2空間16ビット(Flash)
                                ; A1SZ=1 CS1空間16ビット(SRAM)
                                ; A0SZ=1 CS0空間16ビット(ROM)

BCR2_DATA:      .data.w h'115f  ; バスコントロールレジスタ2
                                ; IW31,30=00 CS3空間 0アイドルサイクル
                                ; IW21,20=01 CS2空間 1アイドルサイクル
                                ; IW11,10=00 CS1空間 0アイドルサイクル
                                ; IW01,00=01 CS0空間 1アイドルサイクル
                                ; CW3=0 CS3空間連続アクセス時アイドルなし
                                ; CW2=1 CS2空間連続アクセス時アイドルあり
                                ; CW1=0 CS1空間連続アクセス時アイドルなし
                                ; CW0=1 CS0空間連続アクセス時アイドルあり
                                ; SW3=1 CS3空間CSアサート拡張あり
                                ; SW2=1 CS2空間CSアサート拡張あり
                                ; SW1=1 CS1空間CSアサート拡張あり
                                ; SW0=1 CS0空間CSアサート拡張あり


WCR1_DATA:      .data.w h'1102  ; ウェイトコントロールレジスタ1
                                ; W33-30=0001 CS3空間 1ウェイト
                                ; W23-20=0001 CS2空間 1ウェイト
                                ; W13-10=0000 CS1空間 0ウェイト
                                ; W03-00=0010 CS0空間 2ウェイト

WCR2_DATA:      .data.w h'000f  ; ウェイトコントロールレジスタ2
                                ; DDW1,0=00  (DMA未使用,初期値のまま)
                                ; DSW3-0=1111(DMA未使用,初期値のまま)

PAIORH_DATA:    .data.w h'0000  ; ポートA IOレジスタH
PAIORL_DATA:    .data.w h'0012  ; ポートA IOレジスタL
                                ; PA15IOR=0 CK(システムクロック出力)
                                ; PA14IOR=0 RD
                                ; PA13IOR=0 WRH
                                ; PA12IOR=0 WRL
                                ; PA11IOR=0 CS1
                                ; PA10IOR=0 CS0
                                ; PA9IOR =0 IRQ3(SIO_IRQ,モデム制御線割込み)
                                ; PA8IOR =0 IRQ2(DIPSW割込み)
                                ; PA7IOR =0 CS3
                                ; PA6IOR =0 CS2
                                ; PA5IOR =0 IRQ1(CTS1入力)
                                ; PA4IOR =1 TXD1(Break出力時はPA4とする)
                                ; PA3IOR =0 RXD1
                                ; PA2IOR =0 IRQ0(CTS0入力)
                                ; PA1IOR =1 TXD0(Break出力時はPA1とする)
                                ; PA0IOR =0 RXD0

PACRH_DATA:     .data.w h'5000  ; ポートAコントロールレジスタH
                                ; PA23MD= 1 WRHH (SH7042/44では無し)
                                ; PA22MD= 1 WRHL (SH7042/44では無し)
                                ; PA21MD=00 PA21 (SH7042/44では無し)
                                ; PA20MD=00 PA20 (SH7042/44では無し)
                                ; PA19MD=00 PA19 (SH7042/44では無し)
                                ; PA18MD=00 PA18 (SH7042/44では無し)
                                ; PA17MD=00 PA17 (SH7042/44では無し)
                                ; PA16MD=00 PA16 (SH7042/44では無し)

PACRL1_DATA:    .data.w h'555a  ; ポートAコントロールレジスタL1
                                ; PA15MD= 1 CK(システムクロック出力)
                                ; PA14MD= 1 RD
                                ; PA13MD= 1 WRH
                                ; PA12MD= 1 WRL
                                ; PA11MD= 1 CS1
                                ; PA10MD= 1 CS0
                                ; PA9MD =10 IRQ3(SIO_IRQ,モデム制御線割込み)
                                ; PA8MD =10 IRQ2(DIPSW割込み)

PACRL2_DATA:    .data.w h'ad75  ; ポートAコントロールレジスタL2
                                ; PA7MD=10 CS3
                                ; PA6MD=10 CS2
                                ; PA5MD=11 IRQ1(CTS1入力)
                                ; PA4MD= 1 TXD1
                                ; PA3MD= 1 RXD1
                                ; PA2MD=11 IRQ0(CTS0入力)
                                ; PA1MD= 1 TXD0
                                ; PA0MD= 1 RXD0

PBIOR_DATA:     .data.w h'0034  ; ポートB IOレジスタ
                                ; PB9IOR=0 IRQ7(MAX3100 IRQ入力)
                                ; PB8IOR=0 WAIT
                                ; PB7IOR=0 A19
                                ; PB6IOR=0 A18
                                ; PB5IOR=1 PB5(MAX3100 CS出力)
                                ; PB4IOR=1 PB4(MAX3100 SCLK出力)
                                ; PB3IOR=0 PB3(MAX3100 DOUT入力)
                                ; PB2IOR=1 PB2(MAX3100 DIN出力)
                                ; PB1IOR=0 A17
                                ; PB0IOR=0 A16


PBCR1_DATA:     .data.w h'0007  ; ポートBコントロールレジスタ1
                                ; PB9MD=01 IRQ7(MAX3100 IRQ入力)
                                ; PB8MD=11 WAIT

PBCR2_DATA:     .data.w h'a005  ; ポートBコントロールレジスタ2
                                ; PB7MD=10 A19
                                ; PB6MD=10 A18
                                ; PB5MD=00 PB5(MAX3100 CS出力)
                                ; PB4MD=00 PB4(MAX3100 SCLK出力)
                                ; PB3MD=00 PB3(MAX3100 DOUT入力)
                                ; PB2MD=00 PB2(MAX3100 DIN出力)
                                ; -----= 0
                                ; PB1MD= 1 A17
                                ; -----= 0
                                ; PB0MD= 1 A16

PCIOR_DATA:     .data.w h'0000  ; ポートC IOレジスタ
PCCR_DATA:      .data.w h'ffff  ; ポートC コントロールレジスタ
                                ; PC15MD-PC0MD=1  A15-A0

PDIORH_DATA:    .data.w h'0000  ; ポートD IOレジスタH
PDIORL_DATA:    .data.w h'0000  ; ポートD IOレジスタL
PDCRH1_DATA:    .data.w h'5555  ; ポートD コントロールレジスタH1
                                ; PD31MD-PD24MD=01 D31-D24 (SH7042/44では無し)
PDCRH2_DATA:    .data.w h'5555  ; ポートD コントロールレジスタH2
                                ; PD23MD-PD16MD=01 D23-D16 (SH7042/44では無し)
PDCRL_DATA:     .data.w h'ffff  ; ポートD コントロールレジスタL
                                ; PD15MD-PD0MD=1 D15-D0

PEIOR_DATA:     .data.w h'ffea  ; ポートE IOレジスタ
                                ; PE15IOR=1 PE15(LED E8出力)
                                ; PE14IOR=1 PE14(LED E7出力, LAN DACK0としては未使用)
                                ; PE13IOR=1 PE13(LED E6出力)
                                ; PE12IOR=1 PE12(LED E5出力)
                                ; PE11IOR=1 PE11(LED E4出力)
                                ; PE10IOR=1 PE10(LED E3出力)
                                ; PE9IOR =1 PE9 (LED E2出力)
                                ; PE8IOR =1 PE8 (LED E1出力)
                                ; PE7IOR =1 PE7 (RTC4513 CE出力)
                                ; PE6IOR =1 PE6 (RTC4513 CLK出力)
                                ; PE5IOR =1 PE5 (X24C16 SCL出力)
                                ; PE4IOR =0 PE4 (X24C16 SDA入出力, RTC4513 DATA入出力)
                                ; PE3IOR =1 PE3 (RTS1出力)
                                ; PE2IOR =0 PE2 (未使用)
                                ; PE1IOR =1 PE1 (RTS0出力)
                                ; PE0IOR =0 PE0 (LAN DREQ0としては未使用)

PECR1_DATA:     .data.w h'0000  ; ポートE コントロールレジスタ1
                                ; PE15MD=00 PE15(LED E8出力)
                                ; PE14MD=00 PE14(LED E7出力, LAN DACK0としては未使用)
                                ; PE13MD=00 PE13(LED E6出力)
                                ; PE12MD= 0 PE12(LED E5出力)
                                ; PE11MD= 0 PE11(LED E4出力)
                                ; PE10MD= 0 PE10(LED E3出力)
                                ; PE9MD = 0 PE9 (LED E2出力)
                                ; PE8MD = 0 PE8 (LED E1出力)

PECR2_DATA:     .data.w h'0000  ; ポートE コントロールレジスタ2
                                ; PE7MD = 0 PE7 (RTC4513 CE出力)
                                ; PE6MD = 0 PE6 (RTC4513 CLK出力)
                                ; PE5MD = 0 PE5 (X24C16 SCL出力)
                                ; PE4MD =00 PE4 (X24C16 SDA入出力, RTC4513 DATA入出力)
                                ; PE3MD =00 PE3 (RTS1出力)
                                ; PE2MD =00 PE2 (未使用)
                                ; PE1MD =00 PE1 (RTS0出力)
                                ; PE0MD =00 PE0 (LAN DREQ0としては未使用)

IFCR_DATA:      .data.w h'0000  ; IRQOUT機能コントロールレジスタ
                                ; (IRQOUT機能未使用)

;* レジスタアドレス(下位16ビット)

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

;* 参照アドレス/データ値

        .align  4
INIT_STACK:     .data.l  _INIT_STACK
INIT:           .data.l  __INIT
H_F000:         .data.w  h'f000
H_F400:         .data.w  h'f400

;* 各セクションのアドレス

        .SECTION D,DATA,ALIGN=4     ; 初期化データ領域（ROM）
        .SECTION R,DATA,ALIGN=4     ; 初期化データ領域（RAM）
        .SECTION B,DATA,ALIGN=4     ; 未初期化データ領域（RAM）
        .SECTION STACK,DATA,ALIGN=4 ; スタック領域（RAM）
        .SECTION C,DATA,ALIGN=4     ; 定数領域（ROM）
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

; ROM 先頭 +------------------+
;          |割込みベクタ(VECT)| --- リセットベクタのみ
;          |------------------|
;          |  プログラム領域  |
;          |       (P)        |
;          |------------------|
;          |     定数領域     |
;          |       (C)        |
;          |------------------|
;          | 初期化データ領域 | ---+
;          |       (D)        |    |
;          |------------------|    | _INITSCT() でコピー
;         ～                  ～   |
; RAM 先頭 |------------------|    |
;          | 割込みベクタ領域 | ← | ---- ここは空けてリンク
;          |------------------|    |
;          | 初期化データ領域 | ←-+
;          |       (R)        |
;          |------------------|
;          |未初期化データ領域|
;          |       (B)        |
;          |------------------|
;          |   スタック領域   |
;          |     (STACK)      |

    .END
