/*************************************************************************/
/* Integer-String Convertor Functions                                    */
/*                                                                       */
/*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                             */
/*  All rights reserved.                                                 */
/*                                                                       */
/* RTOS 環境において sprintf, sscanf 系関数の使用は望ましくありません。  */
/* これら関数はスタックの消費が大きく、また、リエントラントでない場合    */
/* もあります。                                                          */
/* 面倒ではありますが、本ファイルに用意した様な、個別の数値－文字列変換  */
/* 関数の組み合せで代替してください。                                    */
/*                                                                       */
/* 02/Feb/1999                                                           */
/* 21/Mar/1999 ?tolen -> ?todlen, ?toulen                                */
/* 12/Jun/1999 Auto length by 'n = 0', Return 's + n'                    */
/* 03/Sep/2003 memcpy -> memmove                                      KA */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "nonitod.h"

/* 内部変数 */

static const char hexs[] = "0123456789abcdef";
static const char hexc[] = "0123456789ABCDEF";

/*************************************************************************/
/* 数値文字列変換した時の桁数を求める                                    */
/*                                                                       */
/* (例1) itolen(123); -> 3                                               */
/* (例2) ltolen(-123456); -> 7                                           */
/*************************************************************************/

int itoulen(unsigned short d)
{
    if (d <= 9) return 1;
    if (d <= 99) return 2;
    if (d <= 999) return 3;
    if (d <= 9999) return 4;
    return 5;
}

int ltoulen(unsigned long d)
{
    if (d <= 9L) return 1;
    if (d <= 99L) return 2;
    if (d <= 999L) return 3;
    if (d <= 9999L) return 4;
    if (d <= 99999L) return 5;
    if (d <= 999999L) return 6;
    if (d <= 9999999L) return 7;
    if (d <= 99999999L) return 8;
    if (d <= 999999999L) return 9;
    return 10;
}

int itodlen(short d)
{
    if (d < 0)
        return itoulen((unsigned short)(-d)) + 1;
    else
        return itoulen((unsigned short)d);
}

int ltodlen(long d)
{
    if (d < 0L)
        return ltoulen((unsigned long)(-d)) + 1;
    else
        return ltoulen((unsigned long)d);
}

int itoxlen(unsigned short d)
{
    if (d <= 0x000f) return 1;
    if (d <= 0x00ff) return 2;
    if (d <= 0x0fff) return 3;
    return 4;
}

int ltoxlen(unsigned long d)
{
    if (d <= 0x0000000fL) return 1;
    if (d <= 0x000000ffL) return 2;
    if (d <= 0x00000fffL) return 3;
    if (d <= 0x0000ffffL) return 4;
    if (d <= 0x000fffffL) return 5;
    if (d <= 0x00ffffffL) return 6;
    if (d <= 0x0fffffffL) return 7;
    return 8;
}

/*************************************************************************/
/* 16ビット整数 -> 16進数文字列（先行ゼロ付き, a～f小文字）              */
/*                                                                       */
/* d を n 桁の16進数文字列に変換し s へ返す。                            */
/* 'a'～'f' は小文字を使用する。                                         */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* 桁溢れチェックなし（溢れ桁無視）。                                    */
/* s サイズは n + 1 以上必要。                                           */
/*                                                                       */
/* (例1) ito0x(s, 0x12a, 4); -> s = "012a"                               */
/* (例2) ito0x(s, 0x12a, 2); -> s = "2a" 桁溢れ                          */
/*************************************************************************/

char *ito0x(char *s, unsigned short d, int n)
{
    int i;

    if (n <= 0)
        n = itoxlen(d);
    i = n - 4;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 4;
    }
    s[n] = '\0';
    switch (n)
    {
    case 4:  i = d & 0x0f; s[3] = hexs[i]; d >>= 4;
    case 3:  i = d & 0x0f; s[2] = hexs[i]; d >>= 4;
    case 2:  i = d & 0x0f; s[1] = hexs[i]; d >>= 4;
    case 1:  i = d & 0x0f; s[0] = hexs[i];
    }
    return s + n;
}

/*************************************************************************/
/* 16ビット整数 -> 16進数文字列（先行ゼロ付き, A～F大文字）              */
/*                                                                       */
/* d を n 桁の16進数文字列に変換し s へ返す。                            */
/* 'A'～'F' は大文字を使用する。                                         */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* 桁溢れチェックなし（溢れ桁無視）。                                    */
/* s サイズは n + 1 以上必要。                                           */
/*                                                                       */
/* (例1) ito0X(s, 0x12a, 4); -> s = "012A"                               */
/* (例2) ito0X(s, 0x12a, 2); -> s = "2A" 桁溢れ                          */
/*************************************************************************/

char *ito0X(char *s, unsigned short d, int n)
{
    int i;

    if (n <= 0)
        n = itoxlen(d);
    i = n - 4;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 4;
    }
    s[n] = '\0';
    switch (n)
    {
    case 4:  i = d & 0x0f; s[3] = hexc[i]; d >>= 4;
    case 3:  i = d & 0x0f; s[2] = hexc[i]; d >>= 4;
    case 2:  i = d & 0x0f; s[1] = hexc[i]; d >>= 4;
    case 1:  i = d & 0x0f; s[0] = hexc[i];
    }
    return s + n;
}

/*************************************************************************/
/* 32ビット整数 -> 16進数文字列（先行ゼロ付き, a～f小文字）              */
/*                                                                       */
/* d を n 桁の16進数文字列に変換し s へ返す。                            */
/* 'a'～'f' は小文字を使用する。                                         */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* 桁溢れチェックなし（溢れ桁無視）。                                    */
/* s サイズは n + 1 以上必要。                                           */
/*                                                                       */
/* (例1) lto0x(s, 0x1234ab, 8); -> s = "001234ab"                        */
/* (例2) lto0x(s, 0x1234ab, 4); -> s = "34ab" 桁溢れ                     */
/*************************************************************************/

char *lto0x(char *s, unsigned long d, int n)
{
    int i;

    if (n <= 0)
        n = ltoxlen(d);
    i = n - 8;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 8;
    }
    s[n] = '\0';
    switch (n)
    {
    case 8:  i = (int)(d & 0x0f); s[7] = hexs[i]; d >>= 4;
    case 7:  i = (int)(d & 0x0f); s[6] = hexs[i]; d >>= 4;
    case 6:  i = (int)(d & 0x0f); s[5] = hexs[i]; d >>= 4;
    case 5:  i = (int)(d & 0x0f); s[4] = hexs[i]; d >>= 4;
    case 4:  i = (int)(d & 0x0f); s[3] = hexs[i]; d >>= 4;
    case 3:  i = (int)(d & 0x0f); s[2] = hexs[i]; d >>= 4;
    case 2:  i = (int)(d & 0x0f); s[1] = hexs[i]; d >>= 4;
    case 1:  i = (int)(d & 0x0f); s[0] = hexs[i];
    }
    return s + n;
}

/*************************************************************************/
/* 32ビット整数 -> 16進数文字列（先行ゼロ付き, A～F大文字）              */
/*                                                                       */
/* d を n 桁の16進数文字列に変換し s へ返す。                            */
/* 'A'～'F' は小文字を使用する。                                         */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* s サイズは n + 1 以上必要。                                           */
/*                                                                       */
/* (例1) lto0X(s, 0x1234ab, 8); -> s = "001234AB"                        */
/* (例2) lto0X(s, 0x1234ab, 4); -> s = "34AB" 桁溢れ                     */
/*************************************************************************/

char *lto0X(char *s, unsigned long d, int n)
{
    int i;

    if (n <= 0)
        n = ltoxlen(d);
    i = n - 8;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 8;
    }
    s[n] = '\0';
    switch (n)
    {
    case 8:  i = (int)(d & 0x0f); s[7] = hexc[i]; d >>= 4;
    case 7:  i = (int)(d & 0x0f); s[6] = hexc[i]; d >>= 4;
    case 6:  i = (int)(d & 0x0f); s[5] = hexc[i]; d >>= 4;
    case 5:  i = (int)(d & 0x0f); s[4] = hexc[i]; d >>= 4;
    case 4:  i = (int)(d & 0x0f); s[3] = hexc[i]; d >>= 4;
    case 3:  i = (int)(d & 0x0f); s[2] = hexc[i]; d >>= 4;
    case 2:  i = (int)(d & 0x0f); s[1] = hexc[i]; d >>= 4;
    case 1:  i = (int)(d & 0x0f); s[0] = hexc[i];
    }
    return s + n;
}

/*************************************************************************/
/* 16ビット整数 -> 10進数文字列（左詰め, 負の符号有り）                  */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* マイナス符号も桁数に含まれる。                                        */
/* n 桁に満たない場合は後ろにスペースが付く。                            */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) itod(s, 123, 4);  -> s = "123 "                                 */
/* (例2) itod(s, -123, 5); -> s = "-123 "                                */
/* (例3) itod(s, -123, 3); -> s = "-99" 桁溢れ                           */
/*************************************************************************/

char *itod(char *s, short d, int n)
{
    if (d >= 0)
        return itou(s, (unsigned short)d, n);
    else
    {   *s++ = '-';
        return itou(s, (unsigned short)-d, --n);
    }
}

/*************************************************************************/
/* 16ビット整数 -> 10進数文字列（左詰め, 符号無し）                      */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は後ろにスペースが付く。                            */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) itou(s, 123, 4);  -> s = "123 "                                 */
/* (例2) itou(s, -123, 5); -> s = "65413"                                */
/* (例3) itou(s, 123, 2);  -> s = "99" 桁溢れ                            */
/*************************************************************************/

char *itou(char *s, unsigned short d, int n)
{
    int i;
    char *ss;

    ss = ito0u(s, d, n);
    for (i = 0; i < n - 1; i++)
    {   if (s[i] != '0')
            break;
    }
    if (i != 0)
    {   memmove(s, s + i, n - i);
        memset(s + n - i, ' ', i);
    }
    return ss;
}

/*************************************************************************/
/* 16ビット整数 -> 10進数文字列（右詰め, 負の符号有り）                  */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* マイナス符号も桁数に含まれる。                                        */
/* n 桁に満たない場合は前にスペースが付く。                              */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) ito_d(s, 123, 4);  -> s = " 123"                                */
/* (例2) ito_d(s, -123, 5); -> s = " -123"                               */
/* (例3) ito_d(s, -123, 3); -> s = "-99" 桁溢れ                          */
/*************************************************************************/

char *ito_d(char *s, short d, int n)
{
    char *ss;

    if (d >= 0)
        return ito_u(s, (unsigned short)d, n);
    else
    {   *s++ = ' ';
        ss = ito0u(s, (unsigned short)-d, --n);
        while (--n > 0)
        {   if (*s != '0')
                break;
            *s++ = ' ';
        }
        *--s = '-';
        return ss;
    }
}

/*************************************************************************/
/* 16ビット整数 -> 10進数文字列（右詰め, 符号無し）                      */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は前にスペースが付く。                              */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) ito_u(s, 123, 4);  -> s = " 123"                                */
/* (例2) ito_u(s, -123, 5); -> s = "65413"                               */
/* (例3) ito_u(s, 123, 2);  -> s = "99" 桁溢れ                           */
/*************************************************************************/

char *ito_u(char *s, unsigned short d, int n)
{
    char *ss;

    ss = ito0u(s, d, n);
    while (--n > 0)
    {   if (*s != '0')
            break;
        *s++ = ' ';
    }
    return ss;
}

/*************************************************************************/
/* 16ビット整数 -> 10進数文字列（先行ゼロ付き, 符号無し）                */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) ito0u(s, 123, 4);  -> s = "0123"                                */
/* (例2) ito0u(s, -123, 5); -> s = "65413"                               */
/* (例3) ito0u(s, 123, 2);  -> s = "99" 桁溢れ                           */
/*************************************************************************/

char *ito0u(char *s, unsigned short d, int n)
{
    int i;

    if (n <= 0)
        n = itoulen(d);
    i = n - 5;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 5;
    }
    s[n] = '\0';

    switch (n)
    {
    case 4: if (d > 9999) d = 9999; break;
    case 3: if (d >  999) d =  999; break;
    case 2: if (d >   99) d =   99; break;
    case 1: if (d >    9) d =    9; break;
    }

    switch (n)
    {
    case 5: i = (d / 10000); *s++ = (char)('0' + i); d %= 10000;
    case 4: i = (d / 1000);  *s++ = (char)('0' + i); d %= 1000;
    case 3: i = (d / 100);   *s++ = (char)('0' + i); d %= 100;
    case 2: i = (d / 10);    *s++ = (char)('0' + i); d %= 10;
    case 1: i = (d);         *s++ = (char)('0' + i);
    }
    return s;
}

/*************************************************************************/
/* 32ビット整数 -> 10進数文字列（左詰め, 負の符号有り）                  */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* マイナス符号も桁数に含まれる。                                        */
/* n 桁に満たない場合は後ろにスペースが付く。                            */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) ltod(s, 123456, 8);  -> s = "123456  "                          */
/* (例2) ltod(s, -123456, 8); -> s = "-123456 "                          */
/* (例3) ltod(s, -123456, 6); -> s = "-99999" 桁溢れ                     */
/*************************************************************************/

char *ltod(char *s, long d, int n)
{
    if (d >= 0L)
        return ltou(s, (unsigned long)d, n);
    else
    {   *s++ = '-';
        return ltou(s, (unsigned long)-d, --n);
    }
}

/*************************************************************************/
/* 32ビット整数 -> 10進数文字列（左詰め, 符号無し）                      */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は後ろにスペースが付く。                            */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) ltou(s, 123456, 8);   -> s = "123456  "                         */
/* (例2) ltou(s, -123456, 10); -> s = "4294843838"                       */
/* (例3) ltou(s, 123456, 5);   -> s = "99999" 桁溢れ                     */
/*************************************************************************/

char *ltou(char *s, unsigned long d, int n)
{
    int i;
    char *ss;

    ss = lto0u(s, d, n);
    for (i = 0; i < n - 1; i++)
    {   if (s[i] != '0')
            break;
    }
    if (i != 0)
    {   memmove(s, s + i, n - i);
        memset(s + n - i, ' ', i);
    }
    return ss;
}

/*************************************************************************/
/* 32ビット整数 -> 10進数文字列（右詰め, 負の符号有り）                  */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* マイナス符号も桁数に含まれる。                                        */
/* n 桁に満たない場合は前にスペースが付く。                              */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) lto_d(s, 123456, 8);  -> s = "  123456"                         */
/* (例2) lto_d(s, -123456, 8); -> s = " -123456"                         */
/* (例3) lto_d(s, -123456, 6); -> s = "-99999" 桁溢れ                    */
/*************************************************************************/

char *lto_d(char *s, long d, int n)
{
    char *ss;

    if (d >= 0L)
        return lto_u(s, (unsigned long)d, n);
    else
    {   *s++ = ' ';
        ss = lto0u(s, (unsigned long)-d, --n);
        while (--n > 0)
        {   if (*s != '0')
                break;
            *s++ = ' ';
        }
        *--s = '-';
        return ss;
    }
}

/*************************************************************************/
/* 32ビット整数 -> 10進数文字列（右詰め, 符号無し）                      */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は前にスペースが付く。                              */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) lto_u(s, 123456, 8);   -> s = "  123456"                        */
/* (例2) lto_u(s, -123456, 10); -> s = "4294843838"                      */
/* (例3) lto0u(s, 123456, 5);   -> s = "99999" 桁溢れ                    */
/*************************************************************************/

char *lto_u(char *s, unsigned long d, int n)
{
    char *ss;

    ss = lto0u(s, d, n);
    while (--n > 0)
    {   if (*s != '0')
            break;
        *s++ = ' ';
    }
    return ss;
}

/*************************************************************************/
/* 32ビット整数 -> 10進数文字列（先行ゼロ付き, 符号無し）                */
/*                                                                       */
/* d を n 桁の10進数文字列に変換し s へ返す。                            */
/* n 桁に満たない場合は前に '0' が付く。                                 */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + 1 以上であること。                                     */
/*                                                                       */
/* (例1) lto0u(s, 123456, 8);   -> s = "00123456"                        */
/* (例2) lto0u(s, -123456, 10); -> s = "4294843838"                      */
/* (例3) lto0u(s, 123456, 5);   -> s = "99999" 桁溢れ                    */
/*************************************************************************/

char *lto0u(char *s, unsigned long d, int n)
{
    int i;

    if (n <= 0)
        n = ltoulen(d);
    i = n - 10;
    if (i > 0)
    {   memset(s, '0', i);
        s += i;
        n = 10;
    }

    switch (n)
    {
    case 9: if (d > 999999999L) d = 999999999L; break;
    case 8: if (d >  99999999L) d =  99999999L; break;
    case 7: if (d >   9999999L) d =   9999999L; break;
    case 6: if (d >    999999L) d =    999999L; break;
    case 5: if (d >     99999L) d =     99999L; break;
    case 4: if (d >      9999L) d =      9999L; break;
    case 3: if (d >       999L) d =       999L; break;
    case 2: if (d >        99L) d =        99L; break;
    case 1: if (d >         9L) d =         9L; break;
    }

    s[n] = '\0';
    switch (n)
    {
    case 10: i = (int)(d / 1000000000L); *s++ = (char)('0' + i); d %= 1000000000L;
    case 9:  i = (int)(d / 100000000L);  *s++ = (char)('0' + i); d %= 100000000L;
    case 8:  i = (int)(d / 10000000L);   *s++ = (char)('0' + i); d %= 10000000L;
    case 7:  i = (int)(d / 1000000L);    *s++ = (char)('0' + i); d %= 1000000L;
    case 6:  i = (int)(d / 100000L);     *s++ = (char)('0' + i); d %= 100000L;
    case 5:  i = (int)(d / 10000);       *s++ = (char)('0' + i); d %= 10000;
    case 4:  i = (int)(d / 1000);        *s++ = (char)('0' + i); d %= 1000;
    case 3:  i = (int)(d / 100);         *s++ = (char)('0' + i); d %= 100;
    case 2:  i = (int)(d / 10);          *s++ = (char)('0' + i); d %= 10;
    case 1:  i = (int)(d);               *s++ = (char)('0' + i);
    }
    return s;
}

/*************************************************************************/
/* 16ビット固定小数点 -> 小数文字列（右詰め, 負の符号有り）              */
/*                                                                       */
/* d を、整数n桁.小数m桁の文字列に変換し s へ返す。                      */
/* 整数部が n 桁に満たない場合は前にスペースが付く。                     */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + m + 2 以上必要。                                       */
/*                                                                       */
/* (例1) ito_f(s, 12345, 4, 2);  -> s = " 123.45"                        */
/* (例2) ito_f(s, -12345, 5, 2); -> s = " -123.45"                       */
/*************************************************************************/

char *ito_f(char *s, short d, int n, int m)
{
    char *ss;

    if (d >= 0)
        return ito_fu(s, (unsigned short)d, n, m);
    else
    {   *s++ = ' ';
        ss = ito_fu(s, -d, n - 1, m);
        for (;;)
        {   if (*s != ' ')
            {   *--s = '-';
                break;
            }
            s++;
        }
        return ss;
    }
}

/*************************************************************************/
/* 16ビット固定小数点 -> 小数文字列（右詰め, 符号無し）                  */
/*                                                                       */
/* d を、整数n桁.小数m桁の文字列に変換し s へ返す。                      */
/* 整数部が n 桁に満たない場合は前にスペースが付く。                     */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + m + 2 以上必要。                                       */
/*                                                                       */
/* (例1) ito_fu(s, 12345, 4, 2); -> s = " 123.45"                        */
/*************************************************************************/

char *ito_fu(char *s, unsigned short d, int n, int m)
{
    char c;
    char *ss;

    if (n == 0)
        n = itoulen(d);
    else
        n += m;
    ss = ito_u(s, d, n);
    if (m <= 0)
        return ss;

    s += n + 1;
    *s-- = '\0';
    c = *(s - 1);
    *s-- = c;
    while (--m > 0)
    {   c = *(s - 1);
        if (c == ' ')
            c = '0';
        *s-- = c;
    }
    *s-- = '.';
    if (*s == ' ')
        *s = '0';
    return ss + 1;
}

/*************************************************************************/
/* 32ビット固定小数点 -> 小数文字列（右詰め, 負の符号有り）              */
/*                                                                       */
/* d を、整数n桁.小数m桁の文字列に変換し s へ返す。                      */
/* 整数部が n 桁に満たない場合は前にスペースが付く。                     */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + m + 2 以上必要。                                       */
/*                                                                       */
/* (例1) lto_f(s, 123456, 6, 2);  -> s = "  1234.56"                     */
/* (例2) lto_f(s, -123456, 6, 2); -> s = " -1234.56"                     */
/*************************************************************************/

char *lto_f(char *s, long d, int n, int m)
{
    char *ss;

    if (d >= 0L)
        return lto_fu(s, (unsigned long)d, n, m);
    else
    {   *s++ = ' ';
        ss = lto_fu(s, -d, n - 1, m);
        for (;;)
        {   if (*s != ' ')
            {   *--s = '-';
                break;
            }
            s++;
        }
        return ss;
    }
}

/*************************************************************************/
/* 32ビット固定小数点 -> 小数文字列（右詰め, 符号無し）                  */
/*                                                                       */
/* d を、整数n桁.小数m桁の文字列に変換し s へ返す。                      */
/* 整数部が n 桁に満たない場合は前にスペースが付く。                     */
/* 桁溢れは、その桁数の上限値 9...9 に変換。                             */
/* s サイズは n + m + 2 以上必要。                                       */
/*                                                                       */
/* (例1) lto_fu(s, 123456, 6, 2); -> s = "  1234.56"                     */
/*************************************************************************/

char *lto_fu(char *s, unsigned long d, int n, int m)
{
    char c;
    char *ss;

    if (n == 0)
        n = ltoulen(d);
    else
        n += m;
    ss = lto_u(s, d, n);
    if (m <= 0)
        return ss;

    s += n + 1;
    *s-- = '\0';
    c = *(s - 1);
    *s-- = c;
    while (--m > 0)
    {   c = *(s - 1);
        if (c == ' ')
            c = '0';
        *s-- = c;
    }
    *s-- = '.';
    if (*s == ' ')
        *s = '0';
    return ss + 1;
}

/*************************************************************************/
/* 16進数文字列 -> 16ビット整数                                          */
/*                                                                       */
/* 文字列 s を数値変換し戻値として返す。atoi の16進数版。                */
/* 16進数文字列の前にスペースがあっても構わない。マイナスは不可。        */
/* '0'～'9','A'～'F','a'～'f' 以外の文字に出会った時に変換終了。         */
/* オーバフローチェックなし。                                            */
/* 文字列が "0x1234" の様に "0x" で始まる場合は、これを飛ばした位置を    */
/* s に与えること。                                                      */
/*                                                                       */
/* (例1) xtoi(" 12ab");  -> 戻値 = 0x12ab                                */
/* (例2) xtoi("0x12ab"); -> 戻値 = 0                                     */
/*************************************************************************/

unsigned short xtoi(const char *s)
{
    char c;
    int d;

    for (;;)
    {   c = *s++;
        if (c == ' ')
            continue;
        else if ('0' <= c && c <= '9')
        {   d = c - '0';
            break;
        }
        else if ('A' <= c && c <= 'F')
        {   d = c - 'A' + 10;
            break;
        }
        else if ('a' <= c && c <= 'f')
        {   d = c - 'a' + 10;
            break;
        }
        else
            return 0;
    }
    for (;;)
    {   c = *s++;
        if ('0' <= c && c <= '9')
            d = (d << 4) + (c - '0');
        else if ('A' <= c && c <= 'F')
            d = (d << 4) + (c - 'A' + 10);
        else if ('a' <= c && c <= 'f')
            d = (d << 4) + (c - 'a' + 10);
        else
            break;
    }
    return (unsigned short)d;
}

/*************************************************************************/
/* 16進数文字列 -> 32ビット整数                                          */
/*                                                                       */
/* 文字列 s を数値変換し戻値として返す。atol の16進数版。                */
/* 16進数文字列の前にスペースがあっても構わない。マイナスは不可。        */
/* '0'～'9','A'～'F','a'～'f' 以外の文字に出会った時に変換終了。         */
/* オーバフローチェックなし。                                            */
/* 文字列が "0x123456" の様に "0x" で始まる場合は、これを飛ばした位置を  */
/* s に与えること。                                                      */
/*                                                                       */
/* (例1) xtol(" 1234ab");  -> 戻値 = 0x1234ab                            */
/* (例2) xtol("0x1234ab"); -> 戻値 = 0                                   */
/*************************************************************************/

unsigned long xtol(const char *s)
{
    char c;
    long d;

    for (;;)
    {   c = *s++;
        if (c == ' ')
            continue;
        else if ('0' <= c && c <= '9')
        {   d = c - '0';
            break;
        }
        else if ('A' <= c && c <= 'F')
        {   d = c - 'A' + 10;
            break;
        }
        else if ('a' <= c && c <= 'f')
        {   d = c - 'a' + 10;
            break;
        }
        else
            return 0L;
    }
    for (;;)
    {   c = *s++;
        if ('0' <= c && c <= '9')
            d = (d << 4) + (c - '0');
        else if ('A' <= c && c <= 'F')
            d = (d << 4) + (c - 'A' + 10);
        else if ('a' <= c && c <= 'f')
            d = (d << 4) + (c - 'a' + 10);
        else
            break;
    }
    return (unsigned long)d;
}

/*************************************************************************/
/* 小数点付き文字列 -> 32ビット固定小数点                                */
/*                                                                       */
/* 文字列 s を、小数部 m 桁と見なす整数に変換し、戻値として返す。        */
/* atof の固定小数点版。(long)(atof(s) * pow(10, m)) と同等。            */
/* 数値文字列の前にスペースがあっても構わない。マイナスも可。            */
/* '0'～'9','.' 以外の文字に出会った時に変換終了。                       */
/* オーバフローチェックなし。                                            */
/*                                                                       */
/* (例1) atofl("1234.56", 2); -> 戻値 = 123456                           */
/* (例2) atofl("1234", 2);    -> 戻値 = 123400                           */
/*************************************************************************/

long atofl(const char *s, int m)
{
    char c, minus;
    int i;
    long d;

    minus = 0;
    for (;;)
    {   c = *s++;
        if (c == ' ')
            continue;
        else if (c == '-')
        {   d = 0L;
            minus = 1;
            break;
        }
        else if ('0' <= c && c <= '9')
        {   d = c - '0';
            break;
        }
        else
            return 0L;
    }
    i = -1;
    for (;;)
    {   c = *s++;
        if (c == '.')
        {   if (i >= 0 || m == 0)
                break;
            i = 0;
            continue;
        }
        else if ('0' <= c && c <= '9')
        {   d = (d * 10) + (c - '0');
            if (i >= 0)
            {   if (++i == m)
                    break;
            }
        }
        else
            break;
    }

    if (i < 0)
        i = 0;
    while (m > i)
    {   d *= 10;
        m--;
    }
    if (minus)
        return -d;
    else
        return d;
}

#ifdef DEBUG
/*************************************************************************/
/* デバッグ用                                                            */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <conio.h>

void main(void)
{
    char *p, *p2;
    char s[80];
    int n, m;
    short d;
    unsigned short u;
    long ld;
    unsigned long lu;

    for (;;)
    {
        putch('>');
        gets(s);
        if (stricmp(s, "Q") == 0)
            exit(0);
        p = strchr(s, ' ');
        if (p == NULL)
        {   puts("?");
            continue;
        }
        *p++ = '\0';

        if (strcmp(s, "ito0x") == 0 && sscanf(p, "%x %d", &u, &n) == 2)
            ito0x(s, u, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito0X") == 0 && sscanf(p, "%x %d", &u, &n) == 2)
            ito0X(s, u, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto0x") == 0 && sscanf(p, "%lx %d", &lu, &n) == 2)
            lto0x(s, lu, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto0X") == 0 && sscanf(p, "%lx %d", &lu, &n) == 2)
            lto0X(s, lu, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "itod")  == 0 && sscanf(p, "%d %d", &d, &n) == 2)
            ltod(s, d, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "itou")  == 0 && sscanf(p, "%u %d", &u, &n) == 2)
            itou(s, u, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito_d") == 0 && sscanf(p, "%d %d", &d, &n) == 2)
            ito_d(s, d, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito_u") == 0 && sscanf(p, "%u %d", &u, &n) == 2)
            ito_u(s, u, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito0u") == 0 && sscanf(p, "%u %d", &u, &n) == 2)
            ito0u(s, u, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ltod")  == 0 && sscanf(p, "%ld %d", &ld, &n) == 2)
            ltod(s, ld, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ltou")  == 0 && sscanf(p, "%lu %d", &lu, &n) == 2)
            ltou(s, lu, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto_d") == 0 && sscanf(p, "%ld %d", &ld, &n) == 2)
            lto_d(s, ld, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto_u") == 0 && sscanf(p, "%lu %d", &lu, &n) == 2)
            lto_u(s, lu, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto0u") == 0 && sscanf(p, "%lu %d", &lu, &n) == 2)
            lto0u(s, lu, n), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito_f") == 0 && sscanf(p, "%d %d %d", &d, &n, &m) == 3)
            ito_f(s, d, n, m), printf("\"%s\"\n", s);
        else if (strcmp(s, "ito_fu")== 0 && sscanf(p, "%u %d %d", &u, &n, &m) == 3)
            ito_fu(s, u, n, m), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto_f") == 0 && sscanf(p, "%ld %d %d", &ld, &n, &m) == 3)
            lto_f(s, ld, n, m), printf("\"%s\"\n", s);
        else if (strcmp(s, "lto_fu")== 0 && sscanf(p, "%lu %d %d", &lu, &n, &m) == 3)
            lto_fu(s, lu, n, m), printf("\"%s\"\n", s);
        else if (strcmp(s, "xtoi") == 0)
            printf("%x\n", xtoi(p));
        else if (strcmp(s, "xtol") == 0)
            printf("%lx\n", xtol(p));
        else if (strcmp(s, "atofl") == 0 && (p2 = strchr(p, ' ')) != NULL && sscanf(p2 + 1, "%d", &m) == 1)
            printf("%ld\n", atofl(p, m));
        else
            puts("?");
    }
}

#endif /* DEBUG */

/* end */
