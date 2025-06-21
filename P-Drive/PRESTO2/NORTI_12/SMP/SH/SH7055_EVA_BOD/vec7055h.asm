;****************************************************************************
;* SHCベクタテーブル設定の例（SH7055 EVA BOD)                               *
;*                                                                          *
;*  File name : vec7055h.asm                                                *
;*                                                                          *
;* 05/Feb/2001                                                              *
;****************************************************************************

        .IMPORT __INIT              ; 初期設定ルーチン

;* 割込みベクタ

        .SECTION VECT,DATA          ; VECTセクションは0番地にリンクすること
        .export _RST_VECT
_RST_VECT:
        .data.l (STARTOF PON)-(STARTOF VECT); パワーオンリセット PC 初期値 ★
        .data.l _INIT_STACK                 ; パワーオンリセット SP 初期値 ★
        .data.l (STARTOF PON)-(STARTOF VECT); マニュアルリセット PC 初期値 ★
        .data.l _INIT_STACK                 ; マニュアルリセット SP 初期値 ★

        ; 割込みベクタを RAM 上に再配置する前提のため、以降のベクタは定義し
        ; ません。
        ; def_inh システムコールを用いず、直接ベクタを ROM に定義する場合、
        ; ここへ書き加えてください。

;* リセット処理

        .section PON,CODE,ALIGN=4
        .export _RESET
_RESET:
        ; リスタートを考慮して割り込みマスクレベルを最高に

        stc     sr,r0
        or      #H'f0,r0
        ldc     r0,sr

        ; 必要な初期化を記述してください。

        mov.w   BCR1,r0
        mov.w   BCR1_DATA,r1
        mov.w   r1,@r0          ;ＢＣＲ１
        mov.w   BCR2,r0
        mov.w   BCR2_DATA,r1
        mov.w   r1,@r0          ;ＢＣＲ２
        mov.w   WCR,r0
        mov.w   WCR_DATA,r1
        mov.w   r1,@r0          ;ＷＣＲ
        mov.w   PCCR,r0
        mov.w   PCCR_DATA,r1
        mov.w   r1,@r0          ;ＰＣＣＲ
        mov.w   PECR,r0
        mov.w   PECR_DATA,r1
        mov.w   r1,@r0          ;ＰＥＣＲ
        mov.w   PFCRH,r0
        mov.w   PFCRH_DATA,r1
        mov.w   r1,@r0          ;ＰＦＣＲＨ
        mov.w   PFCRL,r0
        mov.w   PFCRL_DATA,r1
        mov.w   r1,@r0          ;ＰＦＣＲＬ
        mov.w   PHCR,r0
        mov.w   PHCR_DATA,r1
        mov.w   r1,@r0          ;ＰＨＣＲ

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

        ; スタックポインタをソフトでも再設定し、初期化ルーチンへ飛ぶ。

        mov.l   INIT,r0
        mov.l   INIT_STACK,sp       ; スタックポインタ初期化
        jmp     @r0
        nop

        .ALIGN  4
INIT_STACK: .DATA.L _INIT_STACK     ; パワーオンリセット SP 初期値 ★
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

BCR1_DATA:  .DATA.W H'000f  ;ｴﾘｱ0～3=16bit
;BCR2_DATA:  .DATA.W H'55ff
;WCR_DATA:   .DATA.W H'0012
BCR2_DATA:  .DATA.W H'f1dd  ;ｴﾘｱ0～3=3ｱｲﾄﾞﾙｻｲｸﾙ
WCR_DATA:   .DATA.W H'ff02  ;初期値

PCCR_DATA:  .DATA.W H'0155  ;ﾎﾟｰﾄCｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ
PECR_DATA:  .DATA.W H'ffff  ;ﾎﾟｰﾄEｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ
PFCRH_DATA: .DATA.W H'5555  ;ﾎﾟｰﾄFｺﾝﾄﾛｰﾙﾚｼﾞｽﾀH
PFCRL_DATA: .DATA.W H'5555  ;ﾎﾟｰﾄFｺﾝﾄﾛｰﾙﾚｼﾞｽﾀL
PHCR_DATA:  .DATA.W H'ffff  ;ﾎﾟｰﾄHｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ


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
