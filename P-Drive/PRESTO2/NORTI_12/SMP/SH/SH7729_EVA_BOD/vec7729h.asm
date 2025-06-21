;****************************************************************************
;* SHCベクタ設定の例  Computex SH3 EVA BOD (SH7729)                         *
;*                                                                          *
;*  File name : vec7729h.asm                                                *
;*                                                                          *
;* 23/Apr/2000                                                              *
;* 22/Sep/2000  キャッシュの初期化処理を追加                                *
;* 12/Sep/2001  VECTセクションをCODE属性に修正                           TA *
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
        bra     vect010         ; CSIDE デバッグ時の割込みエントリは +10
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        .export _RESET
_RESET:
vect010:
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

        mov.w   BCR1,r0         ; Computex SH3 EVA BOD (SH7729) 用設定
        mov.w   BCR1_D,r1
        mov.w   r1,@r0          ;ＢＣＲ１
        mov.w   BCR2,r0
        mov.w   BCR2_D,r1
        mov.w   r1,@r0          ;ＢＣＲ２
        mov.w   FRQCR,r0
        mov.w   FRQCR_D,r1
        mov.w   r1,@r0          ;ＦＲＱＣＲ
        mov.w   WCR1,r0
        mov.w   WCR1_D,r1
        mov.w   r1,@r0          ;ＷＣＲ１
        mov.w   WCR2,r0
        mov.w   WCR2_D,r1
        mov.w   r1,@r0          ;ＷＣＲ２
        mov.w   MCR,r0
        mov.w   MCR_D,r1
        mov.w   r1,@r0          ;ＭＣＲ
        mov.l   ADDSET,r0
        mov     #0,r1
        mov.w   r1,@r0          ;ＡＤＤＳＥＴ
        mov.w   SDMR,r0
        mov     #0,r1
        mov.b   r1,@r0          ;ＳＤＭＲ
        mov.l   ADDRST,r0
        mov     #0,r1
        mov.w   r1,@r0          ;ＡＤＤＲＳＴ
        mov.w   RTCSR,r0
        mov.w   RTCSR_D,r1
        mov.W   r1,@r0          ;ＲＴＣＳＲ
        mov.w   RTCOR,r0
        mov.w   RTCOR_D,r1
        mov.w   r1,@r0          ;ＲＴＣＯＲ
INI_SKIP:

    .aif CACHE_ON ne 0
        mov.w   CCR,r0
        mov.w   CCR_D+2,r1
        mov.l   r1,@r0          ;ＣＣＲ
    .aendi

        rts
        nop

        .align  4
ADDSET:     .data.l     h'1a000000
ADDRST:     .data.l     h'18000000
FRQCR:      .data.w     h'ff80
BCR1:       .data.w     h'ff60
BCR2:       .data.w     h'ff62
WCR1:       .data.w     h'ff64
WCR2:       .data.w     h'ff66
MCR:        .data.w     h'ff68
SDMR:       .data.w     h'e088      ;ｴﾘｱ3/32ﾋﾞｯﾄ/ﾚｲﾃﾝｼ2/Burst Length=4
RTCSR:      .data.w     h'ff6e
RTCNT:      .data.w     h'ff70
RTCOR:      .data.w     h'ff72
CCR:        .data.w     h'ffec

FRQCR_D:    .data.w     h'0122      ;PLL1on(×4),PLL2on(×1) 4:1:1
BCR1_D:     .data.w     h'0008      ;ｴﾘｱ3をSDRAM
BCR2_D:     .data.w     h'3fd0      ;ｴﾘｱ1を8ﾋﾞｯﾄ､ｴﾘｱ3を32ﾋﾞｯﾄ､ｴﾘｱ6を32ﾋﾞｯﾄ
;WCR1_D:     .data.w     h'3ff3
;WCR2_D:     .data.w     h'ffdf      ;ﾚｲﾃﾝｼ2
WCR1_D:     .data.w     h'3f00
WCR2_D:     .data.w     h'ffca      ;ﾚｲﾃﾝｼ2
MCR_D:      .data.w     h'0004      ;ﾓｰﾄﾞｾｯﾄｵﾝ､AMX=0､ｵｰﾄﾘﾌﾚｯｼｭ
RTCSR_D:    .data.w     h'a508      ;CKIO/4(8.25MHz--0.12us)
RTCNT_D:    .data.w     h'a500
RTCOR_D:    .data.w     h'a582      ;15.6us=0.12usx130---->HEX=82
CCR_D:      .data.w     h'0008
    .aif CACHE_WT eq 0
            .data.w     h'0005
    .aelse
            .data.w     h'0001
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
