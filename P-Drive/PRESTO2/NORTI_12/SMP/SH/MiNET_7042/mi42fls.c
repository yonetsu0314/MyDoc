/******************************************************************************
* MiNET-7042 ボード フラッシュメモリ（29F200T ワードモード）                  *
*                                                                             *
*                                                                             *
* 14/Oct/1997 作成                                                            *
* 30/Nov/1998 MiNET-7042 用に修正                                       MiSPO *
* 24/Dec/2002 SH7042/44/45で共通のためsh7042.h->sh7040.hにする             KA *
******************************************************************************/

#include "norti3.h"
#include "sh7040.h"
#include "net7042.h"

UW Flush = FLUSH;

/* マクロ関数 */

#define flash_dat(addr,c)   (*((volatile UH *)Flush + addr) = c)
#define flash_cmd(addr,c)   (*((volatile UH *)Flush + addr) = c)
#define flash_sts(addr)     (*((volatile UH *)Flush + addr))

#if defined(NEW)
static int nop(int n) { while (n-- >= 0) ; return n; }
#endif


/*****************************************************************************
* フラッシュメモリＩＤリード
*
******************************************************************************/

BOOL flash_id(UH *maker, UH *device)
{
    /* コマンドシーケンス */

    flash_cmd(0x0555, 0x00aa);
    flash_cmd(0x02aa, 0x0055);
    flash_cmd(0x0555, 0x0090);
    *maker = flash_sts(0x0000);
    *device = flash_sts(0x0002);

    /* リセット */

    flash_cmd(0x0000, 0x00f0);
    return TRUE;
}

/*****************************************************************************
* フラッシュメモリを消去する（FF となる）
*
* 書式  BOOL flash_erase(UW addr, W size);
*
* 引数  addr    フラッシュメモリの先頭を 0 としたアドレス
*       size    消去するデータのバイト数
*
* 戻値  TREU:成功, FALSE:失敗
*
* 記事  ブロック単位でしか消去できない。すなわち任意のアドレスから任意の
*       サイズだけを消去することはできない。
*       アドレスとサイズは下表に従うこと。複数ブロックにまたがってもよい。
*
*         アドレス    サイズ
*         0x00000～   0x10000
*         0x10000～   0x10000
*         0x20000～   0x10000
*         0x30000～   0x08000
*         0x38000～   0x02000
*         0x3a000～   0x02000
*         0x3c000～   0x04000
******************************************************************************/

BOOL flash_erase(UW addr, W size)
{
    UW end;
    UH c;

    if (size & 1)
        return FALSE;

    end = (addr + size) / 2 - 1;
    addr /= 2;

    /* コマンドシーケンス */

    flash_cmd(0x0555, 0xaa);
    flash_cmd(0x02aa, 0x55);
    flash_cmd(0x0555, 0x80);
    flash_cmd(0x0555, 0xaa);
    flash_cmd(0x02aa, 0x55);
    if (addr < 0x08000)
        flash_cmd(0x00000, 0x0030);
    if (addr < 0x10000 && end >= 0x08000)
        flash_cmd(0x08000, 0x0030);
    if (addr < 0x18000 && end >= 0x10000)
        flash_cmd(0x10000, 0x0030);
    if (addr < 0x1c000 && end >= 0x18000)
        flash_cmd(0x18000, 0x0030);
    if (addr < 0x1d000 && end >= 0x1a000)
        flash_cmd(0x1c000, 0x0030);
    if (addr < 0x1e000 && end >= 0x1c000)
        flash_cmd(0x1d000, 0x0030);
    if (addr < 0x20000 && end >= 0x1e000)
        flash_cmd(0x1e000, 0x0030);

    /* DQ7 /DATA ポーリング */

    for (;;)
    {   c = flash_sts(addr);
        if ((c & 0x80) != 0)    /* DQ7 = 1 ? */
            break;
        if ((c & 0x20) == 0)    /* DQ5 = 0 ? */
            continue;
        c = flash_sts(addr);
        if ((c & 0x80) != 0)    /* DQ7 = 1 ? */
            break;
        flash_cmd(0x0000, 0x00f0);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
* フラッシュメモリをプログラムする
*
* 書式  BOOL flash_program(UW addr, void *data, W size);
*
* 引数  addr    フラッシュメモリの先頭を 0 としたアドレス（偶数）
*       data    プログラムするデータへのポインタ（偶数）
*       size    プログラムするデータのバイト数（偶数）
*
* 戻値  TRUE:成功, FALSE:失敗
*
* 記事  偶数であれば、任意のアドレスから、任意のサイズでプログラム可能。
*       すなわち、小さなサイズで何度も繰り返し呼び出し可能。
*       本関数でプログラムする前に消去が必要。
******************************************************************************/

BOOL flash_program(UW addr, void *data, W size)
{
    UW end;
    UH *p;
    UH c, cc;

    if ((UW)data & 1)
        return FALSE;
    if (size & 1)
        return FALSE;

    p = (UH *)data;
    end = (addr + size) / 2;
    addr /= 2;

    for (; addr < end; addr += 1, p++)
    {
        if (*p == 0xffff)
            continue;

        /* コマンドシーケンス */

        flash_cmd(0x0555, 0x00aa);
        flash_cmd(0x02aa, 0x0055);
        flash_cmd(0x0555, 0x00a0);
        flash_dat(addr, *p);

        /* DQ7 /DATA ポーリング */

        cc = *(UB *)p;
        for (;;)
        {
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            if ((c & 0x20) == 0)        /* DQ5 = 0 ? */
                continue;
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            flash_cmd(0x0000, 0x00f0);
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************
* フラッシュメモリをクリアする（00 にプログラムする）
*
* 書式  BOOL flash_clear(UW addr, W size);
*
* 引数  addr    フラッシュメモリの先頭を 0 としたアドレス（偶数）
*       data    プログラムするデータへのポインタ（偶数）
*
* 戻値  TREU:成功, FALSE:失敗
*
* 記事  偶数であれば、任意のアドレスから、任意のサイズでプログラム可能。
*       すなわち、小さなサイズで何度も繰り返し呼び出し可能。
*       本関数でプログラムする前に消去が必要。
******************************************************************************/

BOOL flash_clear(UW addr, W size)
{
    UW end;
    UH c, cc;

    if (size & 1)
        return FALSE;

    end = (addr + size) / 2;
    addr /= 2;

    for (; addr < end; addr += 1)
    {
        /* コマンドシーケンス */

        flash_cmd(0x0555, 0x00aa);
        flash_cmd(0x02aa, 0x0055);
        flash_cmd(0x0555, 0x00a0);
        flash_dat(addr, 0);

        /* DQ7 /DATA ポーリング */

        cc = 0;
        for (;;)
        {
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            if ((c & 0x20) == 0)        /* DQ5 = 0 ? */
                continue;
            c = flash_sts(addr);
            if (((c ^ cc) & 0x80) == 0) /* DQ7 = Data ? */
                break;
            flash_cmd(0x0000, 0xf0);
            return FALSE;
        }
    }
    return TRUE;
}

/* end */
