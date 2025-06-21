;****************************************************************************
;* SHCベクタ設定の例（MS7729RSE01)                                          *
;*                                                                          *
;*  File name : vec7729Rh.asm                                               *
;*                                                                          *
;* 16/Jan/2002                                                              *
;* 02/Jun/2003  MS7729RSE01用に修正                                      KA *
;****************************************************************************

        .import __INIT          ; 初期設定ルーチン
        .import _intdef         ; 割り込みハンドラテーブル
EXPEVT:  .equ   H'FFFFFFD4      ; 例外事象レジスタ
INTEVT:  .equ   H'FFFFFFD8      ; 割り込み事象レジスタ
INTEVT2: .equ   H'A4000000      ; 割り込み事象レジスタ2
INT_NMI: .equ   14              ; ノンマスカブル割り込み番号

;* キャッシュの有効／無効を設定します。

CACHE_ON: .define "1"           ; =0:無効／=1:有効

;* キャッシュのライトスルー／ライトバックを設定します。
;* キャッシュが有効の場合のみ有効です。

CACHE_WT: .define "0"           ; =0:ライトバック／=1:ライトスルー

;* 一般例外発生でホルトした場合は、この変数の値を調べてください。

        .section B,DATA,ALIGN=4
_expspc: .res.l 1               ; 一般例外発生時の SPC
_expssr: .res.l 1               ; 一般例外発生時の SSR
_expevt: .res.l 1               ; 一般例外発生時の EXPEVT

;* リセット

        .section VECT,CODE

        .export _RST_VECT
_RST_VECT:
        bra     _RESET          ; CSIDE デバッグ時の割込みエントリは +10
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        .export _RESET
_RESET:
        mov.l   INIT_STACK,r15   ; スタックポインタ設定

;* メモリ設定等、最初に必要な処理がある場合は、追加してください。

        bsr     _BOOT
        nop

        mov     #0,r0
        mov.l   expevt_4,r3
        mov.l   r0,@-r3         ; expevt = 0;
        mov.l   r0,@-r3         ; expssr = 0;
        mov.l   r0,@-r3         ; expspc = 0;

; FLASH/ROM -> RAM コピー部
; SDRAM上で実行するプログラムイメージをそのままオフセットを付けてFLASHに書き
; 込んだとき FLASH -> RAM コピーをするコードです。
; オフセット書込みが確認されると元のアドレス(SDRAM)にプログラムをコピーし元々
; のアドレス(__INIT)にジャンプしあたかも最初からSDRAMで動いていたかのように動
; き続けます。
; ☆移動(オフセット)がない場合はコピーは行われません。

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

        mov.l   INIT,r3         ; 初期設定ルーチンへ
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


;* 一般例外事象

        .org    H'00000100
        bra     vect110         ; CSIDE デバッグ時の割込みエントリは +10
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
        mov.l   r0,@-r1         ; expevt = EXPEVTレジスタの例外コード;
        stc     ssr,r0
        mov.l   r0,@-r1         ; expssr = SSR;
        stc     spc,r0
        mov.l   r0,@-r1         ; expspc = SPC;

        bra     $               ; 必要ならユーザーで記述してください。
        nop

        .align  4
expevt_41:  .data.l _expevt+4


;* 一般例外事象（TLBミス）

        .org    H'00000400

        mov.l   expevt_44,r1
        mov     #EXPEVT,r0
        mov.l   @r0,r0
        mov.l   r0,@-r1         ; expevt = EXPEVTレジスタの例外コード;
        stc     ssr,r0
        mov.l   r0,@-r1         ; expssr = SSR;
        stc     spc,r0
        mov.l   r0,@-r1         ; expspc = SPC;

        bra     $               ; 必要ならユーザーで記述してください。
        nop

        .align  4
expevt_44:  .data.l _expevt+4

;* 割り込み要求

        .org    H'00000600

        bra     vect610         ; CSIDE デバッグ時の割込みエントリは +10
        nop
        nop
        nop
        nop
        nop
        nop
        nop
vect610:                        ; 割込みでは BL=1,RB=1 へ切り替わる
        stc.l   r0_bank,@-r15   ; R0_BANK0 退避
        mov.l   r0,@-r15        ; R0_BANK1 退避
        mov.l   r1,@-r15        ; R1_BANK1 退避

        mov.l   intevt2,r0
        mov.l   @r0,r1          ; R1_BANK1 = INTEVTレジスタの例外コード;
        tst     r1,r1           ; if (例外コード == 0)
        bt/s    vect610_rte     ;    割込みリターンへ
        shar    r1
        mov.l   intdef,r0       ; R0_BANK1 = def_inh の登録テーブル
        shar    r1
        add     r1,r0           ; R0_BANK0 = テーブル内の対応アドレス
        mov.l   @r15+,r1        ; R1_BANK1 復元
        ldc.l   @r0+,sr         ; SR = def_inh で登録された値
                                ; ここで、RB=0 に切り替わる
        stc     r0_bank,r0      ; R0_BANK0 = R0_BANK1
        ldc.l   @r15+,r0_bank   ; R0_BANK1 復元

        mov.l   @r0,r0          ; R0_BANK0 = def_inh で登録の inthdr アドレス
        jmp     @r0             ; inthdr へ
        mov.l   @r15+,r0        ; R0_BANK0 復元

vect610_rte:                    ; 例外コード0の場合
        mov.l   @r15+,r1        ; R1_BANK1 復元
        mov.l   @r15+,r0        ; R0_BANK1 復元
        ldc.l   @r15+,r0_bank   ; R0_BANK0 復元
        rte                     ; 割込みリターン
        nop

        .align  4
intdef:     .data.l _intdef-INT_NMI*8
intevt2:    .data.l INTEVT2

        .align  4
_BOOT:
        mov.w   CCR,r0
        mov.w   CCR_D,r1
        mov.l   r1,@r0          ;ＣＣＲ

        ; バスコントローラの初期化

        mov.w   RTCSR,r1        ; 再初期化の禁止
        mov.w   @r1,r0
        cmp/eq  #0,r0
        bf      INI_SKIP

        mov.w   BCR1,r0         ; Hitachi ULSI Systens MS7729RSE01 用設定
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
        mov.l   r1,@r0          ;ＣＣＲ
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

;* 各セクションのアドレス

        .section P,CODE,ALIGN=4         ; プログラム領域（ROM）
        .section D,DATA,ALIGN=4         ; 初期化データ領域（ROM）
        .section R,DATA,ALIGN=4         ; 初期化データ領域（RAM）
        .section B,DATA,ALIGN=4         ; 未初期化データ領域（RAM）
        .section STACK,DATA,ALIGN=4     ; スタック領域（RAM）
        .section C,DATA,ALIGN=4         ; 定数領域（ROM）
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

; ROM先頭 +--------------------+
;         | 割込みベクタ(VECT) |
;         +--------------------+
;         |   プログラム領域   |
;         |        (P)         |
;         +--------------------+
;         |      定数領域      |
;         |        (C)         |
;         +--------------------+
;         |  初期化データ領域  |---+
;         |        (D)         |   |
;         +--------------------+   |_INITSCT() でコピー
;         ～                  ～   |
; RAM先頭 +--------------------+   |
;         |  初期化データ領域  |<--+
;         |        (R)         |
;         +--------------------+
;         | 未初期化データ領域 |
;         |        (B)         |
;         +--------------------+
;         |    スタック領域    |
;         |      (STACK)       |
;         ～                  ～
; SP初期値+--------------------+

        .END
