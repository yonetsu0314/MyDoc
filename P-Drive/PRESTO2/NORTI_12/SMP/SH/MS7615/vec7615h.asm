;****************************************************************************
;* SHC用ベクタテーブル設定の例（MS7615SE01 for SH7615）                     *
;*                                                                          *
;*  File name : vec7615h.asm                                                *
;*  Copyright (c) 2000-2002 MiSPO Co., Ltd.  All rights reserved.           *
;*                                                                          *
;* 11/Apr/2000 作成                                               OK(MiSPO) *
;* 11/Feb/2001 周波数変更時の処理設定値(WTCSR)の取り出しミス修正  OK(MiSPO) *
;* 26/Apr/2001 MCR設定変更                                               OK *
;* 16/Jan/2002 ローダによる ROM->RAMコピーに対応                         TA *
;* 09/Oct/2002 FMR初期化を最初に変更                                     KA *
;* 25/Dec/2002 ラベルを統一 stack_init->INIT_STACK                       KA *
;****************************************************************************

;* ★の箇所をカスタマイズしてください
;*   SP 初期値は、RAM の最後のアドレス+1 に設定するのが一般的です。
;*   MS7615SE01 board のSDRAMは、6000000～7FFFFFFの32MBです。

;* 本サンプルで使用するSDRAM構成
;*   6000000～6EFFFFF  for MAIN memory
;*   6F00000～6FFFFFF  for RAM DISK memory
;*   7000000～7FFFFFF  reserved

__INIT_STACK:  .equ    h'6f00000    ; スタック領域終わりのアドレス ★

;* キャッシュの有効／無効を設定します。

CACHE_ON: .define "1"               ; =0:無効／=1:有効

;* キャッシュのライトスルー／ライトバックを設定します。
;* キャッシュが有効の場合のみ有効です。また、キャッシュの設定とは別に
;* SDRAMの設定には、ライトスルーでは、SingleWrite
;* ライトバックでは、Burst Writeにします.

CACHE_WT: .define "0"               ; =0:ライトバック／=1:ライトスルー


;* 外部参照

        .IMPORT __INIT              ; 初期設定ルーチン

;* 割込みベクタ

        .SECTION VECT,DATA          ; VECTセクションは0番地にリンクすること
        .export _RST_VECT
_RST_VECT:
        .data.l (STARTOF PON)-(STARTOF VECT); パワーオンリセット PC 初期値 ★
        .data.l __INIT_STACK                ; パワーオンリセット SP 初期値 ★
        .data.l (STARTOF PON)-(STARTOF VECT); マニュアルリセット PC 初期値 ★
        .data.l __INIT_STACK                ; マニュアルリセット SP 初期値 ★

        ; 割込みベクタを RAM 上に再配置する前提のため、以降のベクタは定義し
        ; ません。def_inh システムコールを用いず、直接ベクタを ROM に定義す
        ; る場合、ここへ書き加えてください。

;* リセット処理

        .section PON,CODE,ALIGN=4
        .export _RESET
_RESET:
        ; リスタートを考慮して割り込みマスクレベルを最高に

        stc     sr,r0
        or      #H'f0,r0
        ldc     r0,sr

        mov.l   CCR,r0
        mov     #0,r1               ; disable cache
        mov.b   r1,@r0
        mov     #h'10,r1            ; force purge
        mov.b   r1,@r0

        ; バスコントローラの初期化

        mov.l   RTCSR,r1        ; 再初期化の禁止 test RTCSR or MCR
        add     #2,r1
        mov.w   @r1,r0
        cmp/eq  #0,r0
        bf      INI_SKIP

        ; システム動作クロックの設定(Iφ,Eφ,Pφ)

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

        ; Set Refresh timer ★

        mov.l   RTCOR,r0
        mov.l   RTCOR_DATA,r1
        mov.l   r1,@r0

        mov.l   RTCNT,r0
        mov.l   RTCNT_DATA,r1
        mov.l   r1,@r0

        mov.l   RTCSR,r0
        mov.l   RTCSR_DATA,r1
        mov.l   r1,@r0

        ; Set mode register in SDRAM ★

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

        ; キャッシュをイネーブルにする.

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

        ; 初期化ルーチンへ飛ぶ。

        mov.l   INIT,r0
        mov.l   INIT_STACK,sp       ; スタックポインタ初期化
        jmp     @r0
        nop

_FMR_ENTRY:
        mov.l   DIRECT_RW,r0
        jmp     @r0                 ; branch to
        nop


;* レジスタアドレスと初期設定値 ★

        .align  4

    .aif CACHE_WT eq 0
SDMR:           .data.l  h'ffff0088  ;SDRAM:レイテンシ=2/Burst /Length=4
    .aelse
SDMR:           .data.l  h'ffff0888  ;SDRAM:レイテンシ=2/Single/Length=4
    .aendi

SD_BANK1:       .data.l  h'06000000  ;SDRAM BANK1 アドレス
SD_BANK2:       .data.l  h'06800000  ;SDRAM BANK2 アドレス
SD_BANK3:       .data.l  h'07000000  ;SDRAM BANK3 アドレス
SD_BANK4:       .data.l  h'07800000  ;SDRAM BANK4 アドレス

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
                                     ;CS3=2 idel, wait SDRAMのレイテンシ=2

WCR2_DATA:      .data.l  h'a55a000b  ;初期値
WCR3_DATA:      .data.l  h'a55a0010  ;

MCR_DATA1:      .data.l  h'a55a5dd8  ;SDRAM=32bit､AMX=5(8Mx8bit)､Auto Refresh

RTCSR_DATA:     .data.l  h'a55a0008  ;Pφ/4(3.75MHz--0.27us)
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
COPY_SIZE       .data.l  (STARTOF D)+(SIZEOF D)-(STARTOF VECT)          ; ROM/RAM:初期化コード+データのサイズ
COPY_ORG        .data.l  (STARTOF VECT)-(STARTOF PON)-(copy_s-_RESET)   ; ROM

FMR_START:      .data.l  _FMR_START - _FMR_RDY
FMR_END:        .data.l  _FMR_END - _FMR_RDY


;* システム動作クロックの設定(Iφ,Eφ,Pφ)
;* SH7615 H/W マニュアル(暫定版)
;*   3.2.5 レジスタによる動作周波数の選択
;*     (2) 周波数変更方法
;* および、SH7612 H/W マニュアル P3-13..P3-17 を参考に作成

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

;* FMR への書きこみ命令を4nに配置する.

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
WTCSR_DATA:     .data.l  h'0000a51f   ; WT/IT=0,TME=0,φ/16384

_FMR_END:

;* 各セクションのアドレス

        .SECTION P,CODE,ALIGN=4     ; コード領域（ROM）
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
;          |  初期化処理(PON) |
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
