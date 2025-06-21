;******************************************************************************
;* NORTi4/SH CPU Interface (Renesas C,SH-1,SH-2)                              *
;*                                                                            *
;*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                 *
;*  All rights reserved.                                                      *
;*                                                                            *
;*  Assemble : asmsh n4csh1h.asm                                              *
;*  Assemble : asmsh n4csh1h.asm -def=HOOK=1     (include Hook-routines)      *
;*                                                                            *
;* 25/Apr/2000                                                                *
;* 02/Oct/2000  フックルーチン対応を整理                                      *
;* 01/Nov/2000  Ver.4.03.00 カーネルに対応（ext_tsk ras_tex）                 *
;* 15/Jan/2001  Ver.4.03.03 として                                            *
;* 17/May/2001  STACKセクションにダミーデータを定義                           *
;* 16/Jan/2002  Hitachi C Ver.7.0 に対応                                      *
;*              Ver.4.05.04 として                                            *
;* 27/Feb/2002  Hitachi C Ver.6.0B に対応                               [KA]  *
;*              Ver.4.05.05 として                                            *
;* 04/Apr/2002  Hitachi C Ver.7.0.02 に対応                             [KA]  *
;*              Ver.4.05.06 として                                            *
;* 23/Apr/2002  FR12～FR15レジスタ退避ミスを修正                        [KA]  *
;*              Ver.4.05.07 として                                            *
;* 21/Jun/2002  Hitachi C Ver.7.0.03 対応確認                           [KA]  *
;*              Ver.4.06 カーネルに対応                                       *
;*              Ver.4.06.07 として                                            *
;* 30/Aug/2002  Hitachi C Ver.7.1 に対応確認                            [KA]  *
;*              Ver.4.06.07 として                                            *
;* 10/Jul/2003  HV6を定義した時のてフォルトをHSTK="60C"に変更           [KA]  *
;* 10/Jul/2003  Hitachi C Ver.7.1.02 に対応確認                         [KA]  *
;*              Ver.4.07.07 として                                            *
;* 29/Jul/2003  ルネサス SH C Ver.6.0x SH4 のレジスタ参照ミスを修正     [TA]  *
;*              Ver.4.07.08 として                                            *
;******************************************************************************

;* ルネサスCコンパイラバージョン定義

    .AIFDEF HVER
    .AELSE
     .AIFDEF HV7
HVER: .ASSIGNA  H'704
     .AELSE
      .AIFDEF HV6
HVER:  .ASSIGNA  6
      .AELSE
       .AIFDEF HV60C
HVER:   .ASSIGNA  6
       .AELSE
        .AIFDEF HV60B
HVER:    .ASSIGNA  6
        .AELSE
         .AIFDEF HV60A
HVER:     .ASSIGNA  6
         .AELSE
HVER:     .ASSIGNA  5
         .AENDI
        .AENDI
       .AENDI
      .AENDI
     .AENDI
    .AENDI

;* 構造体メンバーの位置

TCB_SP:         .equ   0        ; TCB の退避スタックポインタの位置
TCB_PRI:        .equ   4        ; TCB のタスク優先度の位置
CFG_TPRI_MAX:   .equ   4*10     ;

;* 外部参照

        .import _KNL_SR         ; カーネルの SR 値

        .import _c_sysini       ; システム初期化
        .import _v4_wup_tsk     ; タスクを起床する

        .import _RDQ            ; レディキューヘッダ
        .import _IMASK          ; 割込みマスクフラグ
        .import _INEST          ; 割込みネストカウンタ（減カウント）
        .import _DELAY          ; 遅延ディスパッチ要求フラグ
        .import _ISP            ; 割込みスタックポインタ初期値
        .import _NOWPRI         ; 現在のタスク優先度

        .import _DDISP          ; ディスパッチ禁止フラグ
        .import _SDISP          ; システムタスクディスパッチ要求フラグ
        .import _CTXPTN
        .import _CTXPTR
        .import _WID            ; 10/27/99 T.F 待ち要因オブジェクトID
        .import _PREVTSK        ; 11/20/99 T.F 一つ前の Run Task ID
        .import _CFG            ; 11/20/99
        .import __pTCB          ; TCB へのポインタ配列

    .AIFDEF HOOK
        .import _vinit_hook     ;
        .import _visnd_debe     ;
        .import _visnd_debne    ;
        .import _visnd_debr     ;
        .import _visnd_debs     ;
        .import _visnd_debds    ;
        .import _visnd_debns    ;
    .AENDI

        .import _isritm
        .export __kernel_interr
        .export __KERNEL_pisrposi
        .export __KERNEL_hdrlen

;* 外部参照（ユーザ定義）

        .import _intini         ; 割込み初期化


;************************************************
;* バージョンの定義                             *
;************************************************
        .section STACK,DATA,ALIGN=4
        .res.w  1
        .section P,CODE,ALIGN=4
        .sdataz "NORTi(c)MiSPO"
        .align  4
        .export __NORTI_VER
__NORTI_VER:                    ; T_VER _NORTI_VER;
        .data.w h'0108          ; メーカー番号（株式会社ミスポ）
        .data.w h'0000          ; 形式番号（NORTi4/SH）
        .data.w h'5400          ; 仕様書バージョン（μITRON仕様書Ver4.00）
        .data.w h'0708          ; 製品バージョン（Ver.4.07.08）
        .data.w h'0000          ; 製品管理情報
        .data.w h'0000          ; 製品管理情報
        .data.w h'0000          ; 製品管理情報
        .data.w h'0000          ; 製品管理情報
        .data.w h'0000          ; ＣＰＵ情報（SH1,SH2）
        .data.w h'0000          ; バリエーション記述

;******************************************************************************
;*                                                                            *
;*                           システムコール関数                               *
;*                                                                            *
;******************************************************************************
;************************************************
;* システム初期化                               *
;************************************************

        .align  2
        .export _sysini
_sysini:                                ; void sysini(void)
        mov     r15,r5                  ; nowsp  = 現在スタックポインタ値;
        sts.l   pr,@-r15

        mov     #0,r0
        or      #h'f0,r0
        ldc     r0,sr                   ; SR = 0x00f0; 全割込みマスク

        mov.l   STARTOF_STACK,r4        ; stktop = スタック領域先頭アドレス;


        mov.l   c_sysini,r3             ; c_sysini(stktop, nowsp);
        jsr     @r3
        nop

        mov.l   intini,r3               ; intini();
    .AIFDEF HOOK
        jsr     @r3
        nop

        mov.l   INIT_HOOK,r3
    .AENDI
        jmp     @r3
        lds.l   @r15+,pr

;************************************************
;* 割込みハンドラの開始                         *
;************************************************

        .align  2
        .export _v4_ent_int
        .export _v3_ent_int

;       ルネサスC Ver.7.0 以降
;                        T_CTX 構造体
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8～R14 は
;       |________|                          -12|__(R12)_| ret_int で退避
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| R0～R7,PR
;       |________|                          + 4|__MACH__| GBR,MACH,MACL は
;       |________|                          + 8|__MACL__| ent_int で待避
;       |________|                          +12|___PR___|
;       |________|                          +16|___R7___|
;       |________|                          +20|___R6___|
;       |________|                          +24|___R5___|
;       |________|                          +28|___R4___|
;       |________|                          +32|___R3___|
;       |________|                          +36|___R2___|
;       |________|                          +40|___R1___|
;    + 0|___R7___|<- 割込みハンドラで待避   +44|___R0___|
;    + 4|___PC___|<- 割込みによる退避       +48|___PC___|
;    + 8|___SR___|<-  "                     +52|___SR___|

    .AIF \&HVER GE H'700
_v4_ent_int:
        mov.l   r1,@-r15
        mov.l   KNL_SR,r1               ; 割込みマスクをカーネルレベルにする
        mov.l   r2,@-r15
        mov.w   @r1,r2
        mov.l   r3,@-r15
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov.l   @(12,r15),r1
        mov.l   r0,@(12,r15)
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        mov.l   r1,@-r15
        bra     ent_int
        mov.l   r7,@-r15                ; sts.l   pr,@-r15

;       ルネサスC Ver.6.0x以前
;                        T_CTX 構造体
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8～R14 は
;       |________|                          -12|__(R12)_| ret_int で退避
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| R0～R7,PR
;       |________|                          + 4|__MACH__| GBR,MACH,MACL は
;       |________|                          + 8|__MACL__| ent_int で待避
;       |________|                          +12|___PR___|
;       |________|                          +16|___R0___|
;       |________|                          +20|___R1___|
;       |________|                          +24|___R2___|
;       |________|                          +28|___R3___|
;       |________|                          +32|___R4___|
;       |________|                          +36|___R5___|
;       |________|                          +40|___R6___|
;    + 0|___R7___|<- 割込みハンドラで待避   +44|___R7___|
;    + 4|___PC___|<- 割込みによる退避       +48|___PC___|
;    + 8|___SR___|<-  "                     +52|___SR___|

    .AELIF \&HVER LE 6
_v4_ent_int:
        add     #-(4*3),r15
        mov.l   r3,@-r15
        mov.l   KNL_SR,r3               ; 割込みマスクをカーネルレベルにする
        mov.l   r2,@-r15
        mov.w   @r3,r2
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
        mov.l   r6,@(16,r15)
        mov.l   r5,@(12,r15)
        mov.l   r4,@(08,r15)
        mov.l   r1,@-r15                ; mov.l   r1,@-r15
        mov.l   r0,@-r15
        bra     ent_int
        mov.l   r7,@-r15                ; sts.l   pr,@-r15
    .AENDI

;       ルネサスC Ver.7.0 以降
;                        T_CTX 構造体
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8～R14 は
;       |________|                          -12|__(R12)_| ret_int で退避
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR,MACH,MACL は
;       |________|                          + 4|__MACH__| ent_int で待避
;       |________|                          + 8|__MACL__|
;    + 0|___PR___|<- interrupt 属性         +12|___PR___|
;    + 4|___R7___|<-  "                     +16|___R7___|
;    + 8|___R6___|<-  "                     +20|___R6___|
;    +12|___R5___|<-  "                     +24|___R5___|
;    +16|___R4___|<-  "                     +28|___R4___|
;    +20|___R3___|<-  "                     +32|___R3___|
;    +24|___R2___|<-  "                     +36|___R2___|
;    +28|___R1___|<-  "                     +40|___R1___|
;    +32|___R0___|<-  "                     +44|___R0___|
;    +36|___PC___|<- 割込みによる退避       +48|___PC___|
;    +40|___SR___|<-  "                     +52|___SR___|

;       ルネサスC Ver.6.0x以前
;                        T_CTX 構造体
;       |________|          ----->          -28|__(R8)__|<- pTCB[RDQ[0]]->sp
;       |________|                          -24|__(R9)__|
;       |________|                          -20|__(R10)_|
;       |________|                          -16|__(R11)_| R8～R14 は
;       |________|                          -12|__(R12)_| ret_int で退避
;       |________|                          - 8|__(R13)_|
;       |________|                          - 4|__(R14)_|
;       |________|                          + 0|___GBR__| GBR,MACH,MACL は
;       |________|                          + 4|__MACH__| ent_int で待避
;       |________|                          + 8|__MACL__|
;    + 0|___PR___|<- interrupt 属性         +12|___PR___|
;    + 4|___R0___|<-  "                     +16|___R0___|
;    + 8|___R1___|<-  "                     +20|___R1___|
;    +12|___R2___|<-  "                     +24|___R2___|
;    +16|___R3___|<-  "                     +28|___R3___|
;    +20|___R4___|<-  "                     +32|___R4___|
;    +24|___R5___|<-  "                     +36|___R5___|
;    +28|___R6___|<-  "                     +40|___R6___|
;    +32|___R7___|<-  "                     +44|___R7___|
;    +36|___PC___|<- 割込みによる退避       +48|___PC___|
;    +40|___SR___|<-  "                     +52|___SR___|

_v3_ent_int:                            ; void v3_ent_int(void)

                                        ; R0～R7, PR は呼び出し元で退避済み
        mov.l   KNL_SR,r3               ; 割込みマスクをカーネルレベルにする
        mov.w   @r3,r2
        stc     sr,r3
        ldc     r2,sr                   ; SR = KNL_SR;
    .AIF \&HVER GE H'700
        mov.l   r6,@(08,r15)            ; R0,R5,R6 は更新された可能性が
        mov.l   r5,@(12,r15)            ; あるので書き戻す
        mov.l   r0,@(32,r15)
    .AELSE
        mov.l   r6,@(28,r15)            ; R0,R5,R6 は更新された可能性が
        mov.l   r5,@(24,r15)            ; あるので書き戻す
        mov.l   r0,@(04,r15)
    .AENDI
ent_int:
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15

        mov.l   INEST,r1
        mov.b   @r1,r0                  ; if (--INEST == -1)
        mov.l   RDQ,r2
        add     #-1,r0                  ; {
        cmp/eq  #-1,r0
        mov.b   r0,@r1
        bf      ent_int_6
        mov.b   @r2,r1                  ;    pTCB[RDQ[0]]->sp = SP - 28;
        mov.l   pTCB,r0
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r2
        add     #-28,r15
        mov.l   ISP,r1
        mov.l   r15,@(TCB_SP,r2)
        mov.l   @r1,r15                 ;    SP = ISP 初期値

    .AIFDEF HOOK
        mov.l   SND_NOT_TASK,r0
        mov.l   r3,@-r15
        sts.l   pr,@-r15
        jsr     @r0
        nop
        rte
        nop

        .align  2
ent_int_6:                              ; }
        mov.l   SND_ENT_INT,r0
        mov.l   r3,@-r15
        sts.l   pr,@-r15
        jsr     @r0
        nop
        rte
        nop
    .AELSE
ent_int_6:                              ; }
        rts
        ldc     r3,sr                   ; 割込みマスク戻す
    .AENDI

;************************************************
;* 割込み処理復帰とタスク起床を行う             *
;************************************************

        .align  2
        .export _v3_ret_wup
_v3_ret_wup:                            ; void v3_ret_wup(ID tskid)

        mov.l   wup_tsk,r3              ; wup_tsk(tskid);
        sts.l   pr,@-r15
        jsr     @r3
        nop
        lds.l   @r15+,pr                ; --> 以下 ret_int() と同じ

;************************************************
;* 割込みハンドラから復帰する                   *
;************************************************

;       ルネサスC Ver.7.0 以降
;                              T_CTX 構造体
;       |________|       残り待避           +  0|___R8___|<- pTCB[RDQ[0]]->sp
;       |________|        ----->            +  4|___R9___|
;       |________|                          +  8|___R10__| R8～R14 は ret_int で退避
;       |________|                          + 12|___R11__|
;       |________|                          + 16|___R12__|
;       |________|                          + 20|___R13__|
;       |________|                          + 24|___R14__|
;    + 0|___GBR__|< ent_intまでに待避       + 28|___GBR__|
;    + 4|__MACH__|<   "                     + 32|__MACH__|
;    + 8|__MACL__|<   "                     + 36|__MACL__|
;    +12|___PR___|<-  "                     + 40|___PR___|
;    +16|___R7___|<-  "                     + 44|___R7___|
;    +20|___R6___|<-  "                     + 48|___R6___|
;    +24|___R5___|<-  "                     + 52|___R5___|
;    +28|___R4___|<-  "                     + 56|___R4___|
;    +32|___R3___|<-  "                     + 60|___R3___|
;    +36|___R2___|<-  "                     + 64|___R2___|
;    +40|___R1___|<-  "                     + 68|___R1___|
;    +44|___R0___|<-  "                     + 72|___R0___|
;    +48|___PC___|<-  "                     + 76|___PC___|
;    +52|___SR___|<-  "                     + 80|___SR___|

;       ルネサスC Ver.6.0x以前
;                              T_CTX 構造体
;       |________|       残り待避           +  0|___R8___|<- pTCB[RDQ[0]]->sp
;       |________|        ----->            +  4|___R9___|
;       |________|                          +  8|___R10__| R8～R14 は ret_int で退避
;       |________|                          + 12|___R11__|
;       |________|                          + 16|___R12__|
;       |________|                          + 20|___R13__|
;       |________|                          + 24|___R14__|
;    + 0|___GBR__|< ent_intまでに待避       + 28|___GBR__|
;    + 4|__MACH__|<   "                     + 32|__MACH__|
;    + 8|__MACL__|<   "                     + 36|__MACL__|
;    +12|___PR___|<-  "                     + 40|___PR___|
;    +16|___R0___|<-  "                     + 44|___R0___|
;    +20|___R1___|<-  "                     + 48|___R1___|
;    +24|___R2___|<-  "                     + 52|___R2___|
;    +28|___R3___|<-  "                     + 56|___R3___|
;    +32|___R4___|<-  "                     + 60|___R4___|
;    +36|___R5___|<-  "                     + 64|___R5___|
;    +40|___R6___|<-  "                     + 68|___R6___|
;    +44|___R7___|<-  "                     + 72|___R7___|
;    +48|___PC___|<-  "                     + 76|___PC___|
;    +52|___SR___|<-  "                     + 80|___SR___|

        .export _v3_ret_int
_v3_ret_int:                            ; void v3_ret_int(void)

        mov.l   KNL_SR,r2               ; 割込みマスクをカーネルレベルにする
        mov.w   @r2,r0
        ldc     r0,sr                   ; SR = KNL_SR;

    .AIFDEF HOOK
        mov.l   SND_RET_INT,r0
        jsr     @r0
        nop
    .AENDI
        mov.l   INEST,r1
        mov.l   RDQ,r3

        mov.b   @r1,r5                  ; if (++INEST == 0)
        mov     #1,r2
        clrt
        addc    r2,r5                   ; （R5 は DELAY = 0 で使う）
        mov.b   r5,@r1
        bf      ret_int_41              ; {

        mov.b   @r3+,r1                 ; （R3 は rdq = &RDQ[1] で使う）

        mov.l   NOWPRI,r2               ; added by T.F 11/20/99
        mov.b   @r2,r2                  ;;
        tst     r2,r2                   ;;
        bt      not_save_tskid          ;;
        mov.l   TPRI_MAX,r4             ;;
        mov.l   @r4,r4                  ;;
        cmp/gt  r4,r2                   ;;
        bt      not_save_tskid          ;;
        mov.l   PREVTSK,r0              ;;
        mov.b   r1,@r0                  ;; save previous task ID

not_save_tskid:
        mov.l   pTCB,r0
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r1
        mov.l   DELAY,r2
        mov.l   @(TCB_SP,r1),r15        ;    SP = RDQ[0].head->sp;
        mov.b   @r2,r1
        add     #28,r15                 ;    SP += 28;

        mov.l   @(80-28,r15),r0         ;    if ((割込み時の SR & 0xf0) == 0
        tst     #h'f0,r0
        bf      ret_int_4

        mov.l   SDISP,r0                ;    if (SDISP)
        mov.b   @r0,r0
        tst     r0,r0                   ;       goto L1;
        bt      ret_int_1
        bra     ret_int_3
        mov     r0,r1
ret_int_1:
        mov.l   DDISP,r0
        mov.b   @r0,r0
        tst     r0,r0
        bf      ret_int_4

        tst     r1,r1                   ;     && DELAY)
        bt      ret_int_4

        mov     r3,r4                   ;    {
        mov.b   r5,@r2                  ;       DELAY = 0;
ret_int_2:                              ;       for (rdq = &RDQ[1];;)
        mov.b   @r4+,r1                 ;       {  tid = *rdq++;
        tst     r1,r1                   ;          if (tid != 0) break;
        bt      ret_int_2               ;       }
ret_int_3:
        mov.b   r1,@-r3                 ;       RDQ[0] = tid;

        mov.l   pTCB,r0                 ;       tcb = pTCB[tid];
        extu.b  r1,r1
        shll2   r1
        mov.l   @(r0,r1),r1
        mov.l   NOWPRI,r4
        mov.b   @(TCB_PRI,r1),r0
        mov.b   r0,@r4                  ;       NOWPRI = tcb->pri;

        mov.l   r14,@-r15
        mov.l   r13,@-r15               ;       GBR,MACH,MACL,R8～R14 退避
        mov.l   r12,@-r15
        mov.l   r11,@-r15
        mov.l   r10,@-r15
        mov.l   r9,@-r15
        mov.l   r8,@-r15

        mov.l   @(TCB_SP,r1),r15        ;       SP = tcb->sp;
        mov.l   @r15+,r8                ;       R8～R14 復元
        mov.l   @r15+,r9
        mov.l   @r15+,r10
        mov.l   @r15+,r11
        mov.l   @r15+,r12
        mov.l   @r15+,r13
        mov.l   @r15+,r14               ;    }
ret_int_4:
    .AIFDEF HOOK
        mov.l   SND_TASK_SWITCH,r0
        jsr     @r0
        nop
    .AENDI
ret_int_41:                             ; }
        ldc.l   @r15+,gbr               ; GBR,MACH,MACL 復元
        lds.l   @r15+,mach
        lds.l   @r15+,macl
        lds.l   @r15+,pr                ; R0～R7, PR 復元
    .AIF \&HVER GE H'700
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        mov.l   @r15+,r3
        mov.l   @r15+,r4
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte                             ; PC, SR 復元
        nop

;******************************************************************************
;*                                                                            *
;*                            カーネル内部関数                                *
;*                                                                            *
;******************************************************************************
;************************************************
;* ディスパッチ                                 *
;************************************************

;       ルネサスC Ver.7.0 以降
;                              T_CTX 構造体
;       |______|                     + 0|__R8__|<- pTCB[RDQ[0]]->sp
;       |______|         退避        + 4|__R9__|
;       |______|        ----->       + 8|__R10_|
;       |______|                     +12|__R11_|
;       |______|                     +16|__R12_|
;       |______|                     +20|__R13_|
;       |______|                     +24|__R14_|
;       |______|                     +28|__GBR_|
;       |______|                     +32|_MACH_|
;       |______|                     +36|_MACL_|
;       |______|                     +40|__PR__|
;       |______|                     +44|______| R7 退避不要
;       |______|                     +48|__R6__| (CTXPTR)
;       |______|                     +52|__R5__| (CTXPTN)
;       |______|                     +56|__R4__| (WID)
;       |______|                     +60|______| R1～R3 退避不要
;       |______|                     +64|______|
;       |______|                     +68|______|
;       |______|                     +72|__R0__|
;       |______|                     +76|__PC__|
;       |______|                     +80|__SR__|

;       ルネサスC Ver.6.0x以前
;                              T_CTX 構造体
;       |______|                     + 0|__R8__|<- pTCB[RDQ[0]]->sp
;       |______|         退避        + 4|__R9__|
;       |______|        ----->       + 8|__R10_|
;       |______|                     +12|__R11_|
;       |______|                     +16|__R12_|
;       |______|                     +20|__R13_|
;       |______|                     +24|__R14_|
;       |______|                     +28|__GBR_|
;       |______|                     +32|_MACH_|
;       |______|                     +36|_MACL_|
;       |______|                     +40|__PR__|
;       |______|                     +44|__R0__|
;       |______|                     +48|______| R1～R3 退避不要
;       |______|                     +52|______|
;       |______|                     +56|______|
;       |______|                     +60|__R4__| (WID)
;       |______|                     +64|__R5__| (CTXPTN)
;       |______|                     +68|__R6__| (CTXPTR)
;       |______|                     +72|______| R7 退避不要
;       |______|                     +76|__PC__|
;       |______|                     +80|__SR__|

        .align  2
        .export _dispatch
        .export _dispatch1
        .export _dispatch2
        .export _dispatch3
_dispatch:                              ; ER dispatch(void)

; 遅延ディスパッチ要求

        mov.l   DELAY,r2
        mov     #1,r0
        mov.l   IMASK,r3
        mov.b   r0,@r2                  ; DELAY = 1;

; 割込み禁止中または割込みハンドラ内なら
; 割込みマスク戻して終了

        mov.w   @r3,r1                  ; R1 = IMASK;（dispatch_8 で使う）
        mov.l   INEST,r3
        mov     r1,r0
        tst     #h'f0,r0
        mov     #0,r0                   ; ercd = E_OK;
        mov.b   @r3,r5                  ; if (IMASK & 0x00f0)
        bf      dispatch_8              ;    return E_OK;
        mov.l   DDISP,r3
        tst     r5,r5                   ; if (INEST != 0)
        mov.b   @r3,r5
        bf      dispatch_8              ;    return E_OK;
        tst     r5,r5                   ; if (DDISP != 0)
        bf      dispatch_8              ;    return E_OK;

; 一旦割り込み許可

dispatch_1:
        stc     sr,r7                   ; old = SR;
        ldc     r1,sr                   ; SR = 0x0000;
        nop
        ldc     r7,sr                   ; SR = old; (KNL_SR)

; 遅延ディスパッチ要求解消されていれば終了
; 割込み許可中に R0(ercd) が書き換えられている時がある

        mov.b   @r2,r7
        mov.l   RDQ,r3
        tst     r7,r7                   ; if (DELAY == 0)
        bt      dispatch_8              ;    return ercd;

; 実行タスク選択

dispatch_2:
        mov.l   r0,@-r15
        mov.l   r1,@-r15

        mov.b   @r3+,r2                 ; mytcb = pTCB[RDQ[0]];
        mov.l   pTCB,r0
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r7

        mov.l   SDISP,r1
        mov     #0,r0
        mov.b   @r1,r2
        tst     r2,r2
        bf      dispatch_4
        mov.l   DELAY,r2                ; DELAY = 0
        mov.b   r0,@r2
        mov     r3,r0

dispatch_3:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt      dispatch_3              ; }
dispatch_4:
        mov.b   r2,@-r3                 ; RDQ[0] = tid;

        mov.l   pTCB,r0                 ; tcb = pTCB[tid];
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r2

        mov.l   NOWPRI,r3
        mov.b   @(TCB_PRI,r2),r0
        mov.b   r0,@r3                  ; NOWPRI = tcb->pri;

        mov.l   @r15+,r1
        mov.l   @r15+,r0
        cmp/eq  r2,r7
    .AIFDEF HOOK
        bt      dispatch_81
    .AELSE
        bt      dispatch_8
    .AENDI

; コンテキスト退避

    .AIF \&HVER GE H'700
        mov.l   r1,@-r15                ; (SR = 0)
        sts.l   pr,@-r15                ; (PC)
        mov.l   r0,@-r15
        add     #-4*3,r15               ; R1～R3 退避不要
        mov.l   r4,@-r15
        mov.l   r5,@-r15
        mov.l   r6,@-r15
        add     #-4*1,r15               ; R7 退避不要
    .AELSE
        mov.l   r1,@-r15                ; (SR = 0)
        sts.l   pr,@-r15                ; (PC)
        add     #-4*1,r15               ; R7 退避不要
        mov.l   r6,@-r15
        mov.l   r5,@-r15
        mov.l   r4,@-r15
        add     #-4*3,r15               ; R1～R3 退避不要
        mov.l   r0,@-r15
    .AENDI
        sts.l   pr,@-r15
        sts.l   macl,@-r15
        sts.l   mach,@-r15
        stc.l   gbr,@-r15
        mov.l   r14,@-r15
        mov.l   r13,@-r15
        mov.l   r12,@-r15
        mov.l   r11,@-r15
        mov.l   r10,@-r15
        mov.l   r9,@-r15
        mov.l   r8,@-r15
        mov.l   r15,@(TCB_SP,r7)        ; mytcb->sp = SP;

; コンテキスト復元
dispatch_5:
        mov.l   @(TCB_SP,r2),r15        ; SP = tcb->sp;

    .AIFDEF HOOK
        mov.l   SND_TASK_SWITCH_D,r0
        jsr     @r0
        nop
    .AENDI

        mov.l   @r15+,r8
        mov.l   @r15+,r9
        mov.l   @r15+,r10
        mov.l   @r15+,r11
        mov.l   @r15+,r12
        mov.l   @r15+,r13
        mov.l   @r15+,r14
        ldc.l   @r15+,gbr
        lds.l   @r15+,mach
        lds.l   @r15+,macl
        lds.l   @r15+,pr
    .AIF \&HVER GE H'700
        mov.l   @r15+,r7
        mov.l   @r15+,r6
        mov.l   @r15+,r5
        mov.l   @r15+,r4
        mov.l   @r15+,r3
        mov.l   @r15+,r2
        mov.l   @r15+,r1
        mov.l   @r15+,r0
    .AELSE
        mov.l   @r15+,r0
        mov.l   @r15+,r1
        mov.l   @r15+,r2
        mov.l   @r15+,r3
        mov.l   @r15+,r4
        mov.l   @r15+,r5
        mov.l   @r15+,r6
        mov.l   @r15+,r7
    .AENDI
        rte
        nop

; 割込みマスク復元のみでリターン

    .AIFDEF HOOK
dispatch_81:
        mov.l   r1,@-r15
        sts.l   pr,@-r15
        mov.l   r0,@-r15
        mov.l   SND_TASK_SWITCH_I,r0
        jsr     @r0
        nop
        mov.l   @r15+,r0
        rte
        nop
    .AENDI

dispatch_8:
        rts                             ; return ercd;
        ldc     r1,sr                   ; SR = IMASK;

; INEST/IMASK/DDISP チェック無し

        .align  2
_dispatch1:
        mov.l   CTXPTN,r0
        mov.l   CTXPTR,r1
        mov.l   DELAY,r2
        mov.l   IMASK,r3
        mov.l   WID,r7
        mov.l   @r1,r6
        mov.l   @r0,r5
        mov     #1,r1
        mov     #0,r0                   ; ercd = E_OK;
        mov.b   r1,@r2                  ; DELAY = 1;
        mov.l   @r7,r4
        bra     dispatch_1
        mov.w   @r3,r1

; 必ずディスパッチ（syssta, chg_ims 用）

        .align  2
_dispatch2:
        mov.l   DELAY,r2
        mov.l   RDQ,r3
        mov     #0,r0                   ; ercd = E_OK;
        bra     dispatch_2
        mov     #0,r1                   ; (SR) = 0x0000

; 必ずディスパッチ（ext_tsk, exd_tsk 用）

_dispatch3:
        mov.l   RDQ,r3
        add     #1,r3
        mov     r3,r0
dispatch_6:                             ; for (rdq = &RDQ[1];;)
        mov.b   @r0+,r2                 ; {  tid = *rdq++;
        tst     r2,r2                   ;    if (tid != 0) break;
        bt      dispatch_6              ; }
        mov.b   r2,@-r3                 ; RDQ[0] = tid;

        mov.l   pTCB,r0                 ; tcb = pTCB[tid];
        extu.b  r2,r2
        shll2   r2
        mov.l   @(r0,r2),r2

        mov.l   NOWPRI,r3
        mov.b   @(TCB_PRI,r2),r0
        bra     dispatch_5
        mov.b   r0,@r3                  ; NOWPRI = tcb->pri;

;************************************************
;* システムコール入口での割込みマスク           *
;************************************************

;       .align  2
;       .export _dis_ims
;_dis_ims:                              ; void dis_ims(void)
;
;       mov.l   KNL_SR,r1               ; 割込みマスクをカーネルレベルにする
;       stc     sr,r0
;       mov.w   @r1,r2
;       mov.l   IMASK,r1
;       ldc     r2,sr                   ; SR = KNL_SR;
;       rts
;       mov.w   r0,@r1                  ; IMASK = マスク前の SR;

;************************************************
;* システムコール出口での割込みマスク復元       *
;************************************************

;       .align  2
;       .export _ret_ims
;_ret_ims:                              ; ER ret_ims(void)
;
;       mov.l   IMASK,r1
;       mov     #0,r0
;       mov.w   @r1,r2
;       rts                             ; return E_OK;
;       ldc     r2,sr                   ; SR = IMASK;
;
;       .align  2
;       .export _ret_ims2
;_ret_ims2:                             ; void ret_ims2(void)
;
;       mov.l   IMASK,r1
;       mov.w   @r1,r2
;       rts
;       ldc     r2,sr                   ; SR = IMASK;

;************************************************
;* システムコール内部での割込みマスク一時解除   *
;************************************************

;       .align  2
;       .export _ope_ims
;_ope_ims:                              ; void ope_ims(void)
;
;       mov.l   IMASK,r1
;       stc     sr,r2
;       mov.w   @r1,r0
;       ldc     r0,sr                   ; SR = IMASK;
;       nop                             ; IMASK より高優先の割込みを入れる
;       ldc     r2,sr                   ; SR 復元
;       rts
;       mov.w   r0,@r1                  ; 高優先割込で破壊されるため IMASK 復元

;******************************************************************************
;*                                                                            *
;*                         独自システムコール関数                             *
;*                                                                            *
;******************************************************************************
;********************************************************
;* ステータスレジスタの割込みマスクセット               *
;********************************************************

        .align  2
        .export _v3_vdis_psw
_v3_vdis_psw:                           ; UINT vdis_psw(void)

        mov.l   H_000000F0,r1           ; SR IMASK=1111;
        stc     sr,r0
        rts                             ; 戻値 = 割込み禁止前の SR
        ldc     r1,sr                   ; SR = KNL_SR;

;************************************************
;* ステータスレジスタのセット                   *
;************************************************

        .align  2
        .export _v3_vset_psw
_v3_vset_psw:                           ; void vset_psw(UINT psw)

        rts
        ldc     r4,sr


;******************************************************************************
;*                                                                            *
;*                        アドレス/データの定義                               *
;*                                                                            *
;******************************************************************************

        .align  4
KNL_SR:         .data.l _KNL_SR
c_sysini:       .data.l _c_sysini
wup_tsk:        .data.l _v4_wup_tsk
RDQ:            .data.l _RDQ
IMASK:          .data.l _IMASK
INEST:          .data.l _INEST
DELAY:          .data.l _DELAY
ISP:            .data.l _ISP
NOWPRI:         .data.l _NOWPRI
intini:         .data.l _intini
STARTOF_STACK:  .data.l (STARTOF STACK)
DDISP:          .data.l _DDISP
SDISP:          .data.l _SDISP
pTCB:           .data.l __pTCB-4
CTXPTN:         .data.l _CTXPTN
CTXPTR:         .data.l _CTXPTR
WID:            .data.l _WID
PREVTSK:        .data.l _PREVTSK
TPRI_MAX:       .data.l _CFG + CFG_TPRI_MAX
H_000000F0:     .data.l H'000000F0
        .AIFDEF HOOK
INIT_HOOK:          .data.l _vinit_hook
SND_ENT_INT:        .data.l _visnd_debe
SND_NOT_TASK:       .data.l _visnd_debne
SND_RET_INT:        .data.l _visnd_debr
SND_TASK_SWITCH:    .data.l _visnd_debs
SND_TASK_SWITCH_D:  .data.l _visnd_debds
SND_TASK_SWITCH_I:  .data.l _visnd_debns
        .AENDI

;************************************************
;* 起動回数キューイング中の ext_tsk で再初期化  *
;************************************************

        .align  2
        .import _c_ext_tsk
        .export _a_ext_tsk          ; void a_ext_tsk(T_CTX *ctx,
                                    ;                VP_INT stacd,
                                    ;                T_TCB NEAR *tcb,
                                    ;                ID tskid)
_a_ext_tsk:
        mov.l   c_ext_tsk,r3
        jmp     @r3
        mov     r4,r15              ; コンテキスト保存

        .align  4
c_ext_tsk:      .data.l _c_ext_tsk

;**********************************************************
;* タス例外ハンドラの前処理にエラーコードを引数として渡す *
;**********************************************************

        .align  2
        .import _c_ext_exp
        .export _a_ext_exp

_a_ext_exp:
        mov.l   c_ext_exp,r3
        jmp     @r3
        mov     r0,r4               ; コンテキスト保存

        .align  4
c_ext_exp:      .data.l _c_ext_exp

;******************************************************************************
;*                                                                            *
;*                        割込みサービス処理の定義                            *
;*                                                                            *
;******************************************************************************

        .align  4

__kernel_interr:
        mov.l   r7,@-r15
        mov.l   r1,@-r15
        mov.l   L1_ent_int,r1
        sts     pr,r7
        jsr     @r1
        mov.l   @r15+,r1

        mov.l   r14,@-r15
        mov.l   L1_isr,r14
        tst     r14,r14
        bt      L1_2
L1_1:
        mov.l   @r14,r2
        jsr     @r2
        mov.l   @(4,r14),r4
        mov.l   @(8,r14),r14
        tst     r14,r14
        bf      L1_1
L1_2:
        mov.l   L1_ret_int,r2
        jsr     @r2
        mov.l   @r15+,r14

        .align  4
L1_isr:
        .data.l 0
L1_ent_int:
        .data.l _v4_ent_int
L1_ret_int:
        .data.l _v3_ret_int
__kernel_interr_end:

__KERNEL_pisrposi:
        .data.b L1_isr - __kernel_interr
__KERNEL_hdrlen:
        .data.b __kernel_interr_end - __kernel_interr

        .end
