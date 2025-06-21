/*======================================================================
 * File name    : initsk.c
 * Original		: p:/dacs_i/rcu/main/rcs/initsk.c 1.3
 *----------------------------------------------------------------------
 * Function 	:
 *		イニシャルタスク
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/setup.c 1.16 1970/01/01 00:00:00 kagatume Exp $
 *$Log: setup.c $
 * リビジョン 1.16  1970/01/01  00:00:00  kagatume
 * 2005/03/29 19:30
 * リビジョン 1.13 の変更を元に戻す。
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  chimura
 * 2005/01/24 シリアルの初期化の処理をbiosif経由で行う。
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  sakaguti
 * rsstr(\nBIOS:T0018
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  kagatume
 * 2004/05/11 20:00
 * 起動用biosからアプリ用biosへの切り替え処理をソフトウェア割込みで
 * 行うようにする。
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2004/02/19 19:30
 * INT_MASK_REGの定義を削除し、bootpara.h をインクルードする
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/02/05 10::00
 * WDT の初期化を追加する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_reset をライブラリに移動する
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/28 8:40
 * WDTを用いたリブートをやめ、wait時間を増加させることで
 * 3.3Vを安定させるように変更する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2003/11/21 19:40
 * ブートローダーへのジャンプ方法を変更する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2003/11/14 9:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * 電池電圧を越えるように１秒待つ
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * WDTでﾘﾌﾞｰﾄして3.3Vを安定させる
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * optimizeに依存しないようにスタックの再設定を削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_INITSK_C_
#include <sh7615\bootpara.h>
#include	<intrpt.h>
#include "dmu5562.h"

//#pragma inline_asm(setStack)
//void setStack(int sp){
//	mov	r4,r15
//}

int	scu_io_baud(int,int);
int	*scu_io_init(int, int, int,int);
void  bios_init(void);

#pragma	inline_asm(bios_init)
static void bios_init(void){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_init,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          init_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
init_exit:
          MOV.L       Lrsxxx,R0   ;bios_init_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _bios_init_err

          .ALIGN 4
jt_init:
          .DATA.L     _init_vector       ; r0 = 0
          .DATA.L     _INITSCT           ; r0 = 1
          .DATA.L     _init_registers    ; r0 = 2
          .DATA.L     _dmu_jump_to_reset ; r0 = 3
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
          .DATA.L     0                  ; none
}


int
bios_init_err(void)
{
	return(0);
}

void
dmu_jump_to_reset(){		/* ＷＤＴによる再起動 */
	disable_interrupt();

	*RSTCSR = (unsigned short)0x5a5e;		/* WDT Mode Set */
	*RSTCSR = (unsigned short)0xa500;		/* Reset WOVF */
	*WTCNT = (unsigned short)0x5a00;		/* Rest Watch Dog Timer */
	*WTCSR = (unsigned short)0xa57f;		/* WDT Start 70mS TimeUp */

	for(;;);
}


/*****************************************************************************
* ベクタテーブルの初期化
*
******************************************************************************/
int init_vector(void){
	int		i;

	for (i = 0; i <= RL1_P; i++) *((UW *)INTVEC+i) = *((UW *)BIOSVEC+i);
	return 0;
}

/*****************************************************************************
* セクションの初期化
*
******************************************************************************/
int INITSCT(void){
	int *p, *q;

	/* 未初期化データ領域をゼロで初期化 */
	for (p = _B_BGN; p < _B_END; p++)
        *p = 0;

	/* 初期化データを ROM 上から RAM 上へコピー */
	for (p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++)
		*p = *q;
		
	return 0;
}

/*****************************************************************************
* レジスタの初期化
*
******************************************************************************/
int init_registers(void){
	BscInit();					/* ＢＳＣの初期化		*/
	CacheInit();				/* キャッシュの初期化	*/
	init_FMR();					/* 周波数変更			*/
	InitPort();					/* ＩＯポート初期化		*/
	WdtInit();					/* WDTの初期化			*/
	return 0;
}

/*****************************************************************************
* スタートアップルーチン
*
******************************************************************************/
void INIT(void){
	int		i,*p;
	struct	apl_info	*ap;
	V_FUNC			*vec;
	void			(*apl)();
	
	*INT_MASK_REG0 = (char)0xff;		/* Disable Outernal Interrupt */
	*INT_MASK_REG1 = (char)0xff;		/* Disable Outernal Interrupt */

	BscInit();					/* ＢＳＣの初期化		*/
	CacheInit();				/* キャッシュの初期化	*/
	init_FMR();					/* 周波数変更			*/
	InitPort();					/* ＩＯポート初期化		*/
	WdtInit();					/* WDTの初期化			*/
	
	INITSCT();			/* セクションの初期化ルーチンの呼び出し */

	wait(1000);
	
	p = (int *)vector_table;
	for (i = 0; i < 128; i++) *((UW *)INTVEC+i) = *((UW *)p+i);

	/* 8bit 1stop none 57K */
	scu_io_baud(0,6);
	scu_io_init(0,1,0,0);

	disable();
	rsstr("\nBIOS:Starting Aplication ...");

	ap = (struct apl_info *)APL_SECT;
	vec = ap->vecter_adr;
	for(i = *(int *)ap->vect_no; i; i--) {
		*((V_FUNC *)INTVEC+i+RL1_P) = *vec++;
	}

	set_vbr((void **)INTVEC);	/* ＳＨ２のベクトル・テーブルに必須 */

//	setStack(SPAdd);			/* スタックにＥＤＯ－ＤＲＡＭ使用 */

	apl = *((void **)APL_SECT);

	(*apl)();
	
	for(;;);
}



