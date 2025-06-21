/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/driver.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: driver.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/
#include <machine.h>

#include "net.h"
#include "local.h"
#include "driver.h"
#include "..\sh7615.h"

#define LED	0x22200000	/* on-board LED */

typedef volatile unsigned short PORT16;

extern unsigned long vector_table[];

/* 実際の割り込みハンドラのエントリアドレスと、割り込みハンドラに渡す引数を */
/* 保持する。EtherC割り込み用 */
void (*irnew)(int);	/* 実際のハンドラのエントリアドレス */
int irarg;		/* ハンドラに渡す引数 */

/* 上記のテーブルのIRL割り込み用。 */
void (*irl_irnew[15])(int);
int irl_irarg[15];

#pragma interrupt (irstub,irl8_irstub)
/* 一次割り込みハンドラ。実際の割り込みハンドラに引数netnoを渡すために必要 */
/* ところでレジスタの退避についてだが、コンパイラのマニュアルでは#pragma interrupt */
/* 指定された関数では『関数内で使用する』全レジスタを退避するとなっているが、別の */
/* 関数を呼び出した場合にはどうなるのか？　呼び出された関数の方では当然R0～R7の */
/* 退避は行っていないので、この関数内でR0～R7を全て退避しないと正常動作しない。 */
/* ところが生成コードを見る限りR0～R7は全て退避している様であるが、これを信用 */
/* していいのか？ */
/* コンパイルリストを見て、必要ならＣではなくアセンブラで記述する事。 */
void irstub(void)
{
    (*irnew)(irarg);
}

/* IRL割り込みでは１つのベクタ番号に２つの割り込み事由が多重しているので */
/* 本来ならこれを識別しなければならないのだが、手抜きして行っていない */
/* FDC73C935のSerial 0用のIRL8用しか作成していない.... */
void irl8_irstub(void)
{
    (*irl_irnew[7])(irl_irarg[7]);
}

/* 指定された割り込みレベルのハンドラを設定する。E-DMAC専用 */
/* irno  - ベクタ番号。ベクタ番号は72～127の範囲で自由に設定可能である。 */
/* netno - この割り込みを使用するLAN I/Fを管理するネットワーク番号。実際の */
/*         割り込みハンドラに引数として渡される */
/* irhan - 実際の割り込みハンドラのエントリアドレス */
void IRinstall(int irno, int netno, void (*irhan) (int x))
{
    PORT16 *vcra =   (PORT16 *)VCRA;
    PORT16 *iprb =   (PORT16 *)IPRB;

    if ((irno < 68)||(irno > 127)) return;

    *iprb = (*iprb & 0x0fff) | (EDMAC_PRIOR << 12);
    *vcra = (*vcra & 0x007f) | (irno << 8);
    vector_table[irno] = (unsigned long)irstub;
    irnew = irhan;
    irarg = netno;
}

/* 指定された割り込みレベルのハンドラを設定する。IRL割り込み用 */
/* irlno  - 優先レベル(IRLnのn)。1～15の範囲 */
/* netno - この割り込みを使用するLAN I/Fを管理するネットワーク番号。実際の */
/*         割り込みハンドラに引数として渡される */
/* irhan - 実際の割り込みハンドラのエントリアドレス */
void IRLinstall(int irlno, int netno, void (*irhan) (int x))
{
    int vectno;

    if ((irlno < 1)||(irlno > 15)) {
        return;
    }
    if (irlno != 8) {
        return;		/* 他は一次割り込みハンドラを作っていない... */
    }

    vectno = (irlno/2) + 64;
    vector_table[vectno] = (unsigned long)irl8_irstub;
    irl_irnew[irlno-1] = irhan;
    irl_irarg[irlno-1] = netno;
}

/* 指定された割り込みレベルのハンドラを解除する。 */
/* irno  - ベクタ番号 */
/* 解除するといっても、デバイス側で割り込み要求を止める以外に方法がない。 */
/* 勝手にベクタをクリアすると暴走してしまう....。 */
void IRrestore(int irno)
{
}
