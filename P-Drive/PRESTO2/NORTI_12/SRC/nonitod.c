/*************************************************************************/
/* Integer-String Convertor Functions                                    */
/*                                                                       */
/*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                             */
/*  All rights reserved.                                                 */
/*                                                                       */
/* RTOS ���ɂ����� sprintf, sscanf �n�֐��̎g�p�͖]�܂�������܂���B  */
/* �����֐��̓X�^�b�N�̏���傫���A�܂��A���G���g�����g�łȂ��ꍇ    */
/* ������܂��B                                                          */
/* �ʓ|�ł͂���܂����A�{�t�@�C���ɗp�ӂ����l�ȁA�ʂ̐��l�|������ϊ�  */
/* �֐��̑g�ݍ����ő�ւ��Ă��������B                                    */
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

/* �����ϐ� */

static const char hexs[] = "0123456789abcdef";
static const char hexc[] = "0123456789ABCDEF";

/*************************************************************************/
/* ���l������ϊ��������̌��������߂�                                    */
/*                                                                       */
/* (��1) itolen(123); -> 3                                               */
/* (��2) ltolen(-123456); -> 7                                           */
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
/* 16�r�b�g���� -> 16�i��������i��s�[���t��, a�`f�������j              */
/*                                                                       */
/* d �� n ����16�i��������ɕϊ��� s �֕Ԃ��B                            */
/* 'a'�`'f' �͏��������g�p����B                                         */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* �����`�F�b�N�Ȃ��i��ꌅ�����j�B                                    */
/* s �T�C�Y�� n + 1 �ȏ�K�v�B                                           */
/*                                                                       */
/* (��1) ito0x(s, 0x12a, 4); -> s = "012a"                               */
/* (��2) ito0x(s, 0x12a, 2); -> s = "2a" �����                          */
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
/* 16�r�b�g���� -> 16�i��������i��s�[���t��, A�`F�啶���j              */
/*                                                                       */
/* d �� n ����16�i��������ɕϊ��� s �֕Ԃ��B                            */
/* 'A'�`'F' �͑啶�����g�p����B                                         */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* �����`�F�b�N�Ȃ��i��ꌅ�����j�B                                    */
/* s �T�C�Y�� n + 1 �ȏ�K�v�B                                           */
/*                                                                       */
/* (��1) ito0X(s, 0x12a, 4); -> s = "012A"                               */
/* (��2) ito0X(s, 0x12a, 2); -> s = "2A" �����                          */
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
/* 32�r�b�g���� -> 16�i��������i��s�[���t��, a�`f�������j              */
/*                                                                       */
/* d �� n ����16�i��������ɕϊ��� s �֕Ԃ��B                            */
/* 'a'�`'f' �͏��������g�p����B                                         */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* �����`�F�b�N�Ȃ��i��ꌅ�����j�B                                    */
/* s �T�C�Y�� n + 1 �ȏ�K�v�B                                           */
/*                                                                       */
/* (��1) lto0x(s, 0x1234ab, 8); -> s = "001234ab"                        */
/* (��2) lto0x(s, 0x1234ab, 4); -> s = "34ab" �����                     */
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
/* 32�r�b�g���� -> 16�i��������i��s�[���t��, A�`F�啶���j              */
/*                                                                       */
/* d �� n ����16�i��������ɕϊ��� s �֕Ԃ��B                            */
/* 'A'�`'F' �͏��������g�p����B                                         */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* s �T�C�Y�� n + 1 �ȏ�K�v�B                                           */
/*                                                                       */
/* (��1) lto0X(s, 0x1234ab, 8); -> s = "001234AB"                        */
/* (��2) lto0X(s, 0x1234ab, 4); -> s = "34AB" �����                     */
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
/* 16�r�b�g���� -> 10�i��������i���l��, ���̕����L��j                  */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* �}�C�i�X�����������Ɋ܂܂��B                                        */
/* n ���ɖ����Ȃ��ꍇ�͌��ɃX�y�[�X���t���B                            */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) itod(s, 123, 4);  -> s = "123 "                                 */
/* (��2) itod(s, -123, 5); -> s = "-123 "                                */
/* (��3) itod(s, -123, 3); -> s = "-99" �����                           */
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
/* 16�r�b�g���� -> 10�i��������i���l��, ���������j                      */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͌��ɃX�y�[�X���t���B                            */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) itou(s, 123, 4);  -> s = "123 "                                 */
/* (��2) itou(s, -123, 5); -> s = "65413"                                */
/* (��3) itou(s, 123, 2);  -> s = "99" �����                            */
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
/* 16�r�b�g���� -> 10�i��������i�E�l��, ���̕����L��j                  */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* �}�C�i�X�����������Ɋ܂܂��B                                        */
/* n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                              */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) ito_d(s, 123, 4);  -> s = " 123"                                */
/* (��2) ito_d(s, -123, 5); -> s = " -123"                               */
/* (��3) ito_d(s, -123, 3); -> s = "-99" �����                          */
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
/* 16�r�b�g���� -> 10�i��������i�E�l��, ���������j                      */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                              */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) ito_u(s, 123, 4);  -> s = " 123"                                */
/* (��2) ito_u(s, -123, 5); -> s = "65413"                               */
/* (��3) ito_u(s, 123, 2);  -> s = "99" �����                           */
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
/* 16�r�b�g���� -> 10�i��������i��s�[���t��, ���������j                */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) ito0u(s, 123, 4);  -> s = "0123"                                */
/* (��2) ito0u(s, -123, 5); -> s = "65413"                               */
/* (��3) ito0u(s, 123, 2);  -> s = "99" �����                           */
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
/* 32�r�b�g���� -> 10�i��������i���l��, ���̕����L��j                  */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* �}�C�i�X�����������Ɋ܂܂��B                                        */
/* n ���ɖ����Ȃ��ꍇ�͌��ɃX�y�[�X���t���B                            */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) ltod(s, 123456, 8);  -> s = "123456  "                          */
/* (��2) ltod(s, -123456, 8); -> s = "-123456 "                          */
/* (��3) ltod(s, -123456, 6); -> s = "-99999" �����                     */
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
/* 32�r�b�g���� -> 10�i��������i���l��, ���������j                      */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͌��ɃX�y�[�X���t���B                            */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) ltou(s, 123456, 8);   -> s = "123456  "                         */
/* (��2) ltou(s, -123456, 10); -> s = "4294843838"                       */
/* (��3) ltou(s, 123456, 5);   -> s = "99999" �����                     */
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
/* 32�r�b�g���� -> 10�i��������i�E�l��, ���̕����L��j                  */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* �}�C�i�X�����������Ɋ܂܂��B                                        */
/* n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                              */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) lto_d(s, 123456, 8);  -> s = "  123456"                         */
/* (��2) lto_d(s, -123456, 8); -> s = " -123456"                         */
/* (��3) lto_d(s, -123456, 6); -> s = "-99999" �����                    */
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
/* 32�r�b�g���� -> 10�i��������i�E�l��, ���������j                      */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                              */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) lto_u(s, 123456, 8);   -> s = "  123456"                        */
/* (��2) lto_u(s, -123456, 10); -> s = "4294843838"                      */
/* (��3) lto0u(s, 123456, 5);   -> s = "99999" �����                    */
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
/* 32�r�b�g���� -> 10�i��������i��s�[���t��, ���������j                */
/*                                                                       */
/* d �� n ����10�i��������ɕϊ��� s �֕Ԃ��B                            */
/* n ���ɖ����Ȃ��ꍇ�͑O�� '0' ���t���B                                 */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + 1 �ȏ�ł��邱�ƁB                                     */
/*                                                                       */
/* (��1) lto0u(s, 123456, 8);   -> s = "00123456"                        */
/* (��2) lto0u(s, -123456, 10); -> s = "4294843838"                      */
/* (��3) lto0u(s, 123456, 5);   -> s = "99999" �����                    */
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
/* 16�r�b�g�Œ菬���_ -> ����������i�E�l��, ���̕����L��j              */
/*                                                                       */
/* d ���A����n��.����m���̕�����ɕϊ��� s �֕Ԃ��B                      */
/* �������� n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                     */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + m + 2 �ȏ�K�v�B                                       */
/*                                                                       */
/* (��1) ito_f(s, 12345, 4, 2);  -> s = " 123.45"                        */
/* (��2) ito_f(s, -12345, 5, 2); -> s = " -123.45"                       */
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
/* 16�r�b�g�Œ菬���_ -> ����������i�E�l��, ���������j                  */
/*                                                                       */
/* d ���A����n��.����m���̕�����ɕϊ��� s �֕Ԃ��B                      */
/* �������� n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                     */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + m + 2 �ȏ�K�v�B                                       */
/*                                                                       */
/* (��1) ito_fu(s, 12345, 4, 2); -> s = " 123.45"                        */
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
/* 32�r�b�g�Œ菬���_ -> ����������i�E�l��, ���̕����L��j              */
/*                                                                       */
/* d ���A����n��.����m���̕�����ɕϊ��� s �֕Ԃ��B                      */
/* �������� n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                     */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + m + 2 �ȏ�K�v�B                                       */
/*                                                                       */
/* (��1) lto_f(s, 123456, 6, 2);  -> s = "  1234.56"                     */
/* (��2) lto_f(s, -123456, 6, 2); -> s = " -1234.56"                     */
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
/* 32�r�b�g�Œ菬���_ -> ����������i�E�l��, ���������j                  */
/*                                                                       */
/* d ���A����n��.����m���̕�����ɕϊ��� s �֕Ԃ��B                      */
/* �������� n ���ɖ����Ȃ��ꍇ�͑O�ɃX�y�[�X���t���B                     */
/* �����́A���̌����̏���l 9...9 �ɕϊ��B                             */
/* s �T�C�Y�� n + m + 2 �ȏ�K�v�B                                       */
/*                                                                       */
/* (��1) lto_fu(s, 123456, 6, 2); -> s = "  1234.56"                     */
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
/* 16�i�������� -> 16�r�b�g����                                          */
/*                                                                       */
/* ������ s �𐔒l�ϊ����ߒl�Ƃ��ĕԂ��Batoi ��16�i���ŁB                */
/* 16�i��������̑O�ɃX�y�[�X�������Ă��\��Ȃ��B�}�C�i�X�͕s�B        */
/* '0'�`'9','A'�`'F','a'�`'f' �ȊO�̕����ɏo��������ɕϊ��I���B         */
/* �I�[�o�t���[�`�F�b�N�Ȃ��B                                            */
/* ������ "0x1234" �̗l�� "0x" �Ŏn�܂�ꍇ�́A������΂����ʒu��    */
/* s �ɗ^���邱�ƁB                                                      */
/*                                                                       */
/* (��1) xtoi(" 12ab");  -> �ߒl = 0x12ab                                */
/* (��2) xtoi("0x12ab"); -> �ߒl = 0                                     */
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
/* 16�i�������� -> 32�r�b�g����                                          */
/*                                                                       */
/* ������ s �𐔒l�ϊ����ߒl�Ƃ��ĕԂ��Batol ��16�i���ŁB                */
/* 16�i��������̑O�ɃX�y�[�X�������Ă��\��Ȃ��B�}�C�i�X�͕s�B        */
/* '0'�`'9','A'�`'F','a'�`'f' �ȊO�̕����ɏo��������ɕϊ��I���B         */
/* �I�[�o�t���[�`�F�b�N�Ȃ��B                                            */
/* ������ "0x123456" �̗l�� "0x" �Ŏn�܂�ꍇ�́A������΂����ʒu��  */
/* s �ɗ^���邱�ƁB                                                      */
/*                                                                       */
/* (��1) xtol(" 1234ab");  -> �ߒl = 0x1234ab                            */
/* (��2) xtol("0x1234ab"); -> �ߒl = 0                                   */
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
/* �����_�t�������� -> 32�r�b�g�Œ菬���_                                */
/*                                                                       */
/* ������ s ���A������ m ���ƌ��Ȃ������ɕϊ����A�ߒl�Ƃ��ĕԂ��B        */
/* atof �̌Œ菬���_�ŁB(long)(atof(s) * pow(10, m)) �Ɠ����B            */
/* ���l������̑O�ɃX�y�[�X�������Ă��\��Ȃ��B�}�C�i�X���B            */
/* '0'�`'9','.' �ȊO�̕����ɏo��������ɕϊ��I���B                       */
/* �I�[�o�t���[�`�F�b�N�Ȃ��B                                            */
/*                                                                       */
/* (��1) atofl("1234.56", 2); -> �ߒl = 123456                           */
/* (��2) atofl("1234", 2);    -> �ߒl = 123400                           */
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
/* �f�o�b�O�p                                                            */
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
