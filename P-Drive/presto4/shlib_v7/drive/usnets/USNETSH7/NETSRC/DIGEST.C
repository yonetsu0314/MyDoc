/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/digest.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: digest.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* DIGEST.C  Edition: 2  Last changed: 17-Feb-98,14:57:28  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    DIGEST.C -- Digestion Routines for USNET

    Copyright (C) 1994, 1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#include <string.h>

/* the MD5 digestion algorithm, as described in RFC 1321 */

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4. */
#define FF(a, b, c, d, x, s, ac) { \
    a += (((b) & (c)) | ((~b) & (d))) + (x) + (unsigned long)(ac); \
    a = ROTATE_LEFT(a, s) + (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    a += (((b) & (d)) | ((c) & (~d))) + (x) + (unsigned long)(ac); \
    a = ROTATE_LEFT(a, s) + (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    a += ((b) ^ (c) ^ (d)) + (x) + (unsigned long)(ac); \
    a = ROTATE_LEFT(a, s) + (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    a += ((c) ^ ((b) | (~d))) + (x) + (unsigned long)(ac); \
    a = ROTATE_LEFT(a, s) + (b); \
}

static void     Encode(unsigned char *output, unsigned long *input, int len)
{
    unsigned long   ul1;

    while (len--) {
        ul1 = *input++;
        *output++ = (unsigned char) ul1;
        *output++ = (unsigned char) (ul1 >> 8);
        *output++ = (unsigned char) (ul1 >> 16);
        *output++ = (unsigned char) (ul1 >> 24);
    }
}

/* MD5 basic transformation. Transforms state based on block. */

static void     MD5Transform(unsigned long state[4], unsigned char block[64])
{
    int             i,
                    j;
    unsigned long   a,
                    b,
                    c,
                    d,
                    x[16];

    a = state[0], b = state[1], c = state[2], d = state[3];
    for (i = j = 0; j < 64; i++, j += 4)
        x[i] = ((unsigned long) block[j]) | ((unsigned long) block[j + 1] << 8) |
            ((unsigned long) block[j + 2] << 16) |
            ((unsigned long) block[j + 3] << 24);

    FF(a, b, c, d, x[0], 7, 0xd76aa478);    /* 1 */
    FF(d, a, b, c, x[1], 12, 0xe8c7b756);   /* 2 */
    FF(c, d, a, b, x[2], 17, 0x242070db);   /* 3 */
    FF(b, c, d, a, x[3], 22, 0xc1bdceee);   /* 4 */
    FF(a, b, c, d, x[4], 7, 0xf57c0faf);    /* 5 */
    FF(d, a, b, c, x[5], 12, 0x4787c62a);   /* 6 */
    FF(c, d, a, b, x[6], 17, 0xa8304613);   /* 7 */
    FF(b, c, d, a, x[7], 22, 0xfd469501);   /* 8 */
    FF(a, b, c, d, x[8], 7, 0x698098d8);    /* 9 */
    FF(d, a, b, c, x[9], 12, 0x8b44f7af);   /* 10 */
    FF(c, d, a, b, x[10], 17, 0xffff5bb1);  /* 11 */
    FF(b, c, d, a, x[11], 22, 0x895cd7be);  /* 12 */
    FF(a, b, c, d, x[12], 7, 0x6b901122);   /* 13 */
    FF(d, a, b, c, x[13], 12, 0xfd987193);  /* 14 */
    FF(c, d, a, b, x[14], 17, 0xa679438e);  /* 15 */
    FF(b, c, d, a, x[15], 22, 0x49b40821);  /* 16 */

    GG(a, b, c, d, x[1], 5, 0xf61e2562);    /* 17 */
    GG(d, a, b, c, x[6], 9, 0xc040b340);    /* 18 */
    GG(c, d, a, b, x[11], 14, 0x265e5a51);  /* 19 */
    GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);   /* 20 */
    GG(a, b, c, d, x[5], 5, 0xd62f105d);    /* 21 */
    GG(d, a, b, c, x[10], 9, 0x2441453);    /* 22 */
    GG(c, d, a, b, x[15], 14, 0xd8a1e681);  /* 23 */
    GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);   /* 24 */
    GG(a, b, c, d, x[9], 5, 0x21e1cde6);    /* 25 */
    GG(d, a, b, c, x[14], 9, 0xc33707d6);   /* 26 */
    GG(c, d, a, b, x[3], 14, 0xf4d50d87);   /* 27 */
    GG(b, c, d, a, x[8], 20, 0x455a14ed);   /* 28 */
    GG(a, b, c, d, x[13], 5, 0xa9e3e905);   /* 29 */
    GG(d, a, b, c, x[2], 9, 0xfcefa3f8);    /* 30 */
    GG(c, d, a, b, x[7], 14, 0x676f02d9);   /* 31 */
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);  /* 32 */

    HH(a, b, c, d, x[5], 4, 0xfffa3942);    /* 33 */
    HH(d, a, b, c, x[8], 11, 0x8771f681);   /* 34 */
    HH(c, d, a, b, x[11], 16, 0x6d9d6122);  /* 35 */
    HH(b, c, d, a, x[14], 23, 0xfde5380c);  /* 36 */
    HH(a, b, c, d, x[1], 4, 0xa4beea44);    /* 37 */
    HH(d, a, b, c, x[4], 11, 0x4bdecfa9);   /* 38 */
    HH(c, d, a, b, x[7], 16, 0xf6bb4b60);   /* 39 */
    HH(b, c, d, a, x[10], 23, 0xbebfbc70);  /* 40 */
    HH(a, b, c, d, x[13], 4, 0x289b7ec6);   /* 41 */
    HH(d, a, b, c, x[0], 11, 0xeaa127fa);   /* 42 */
    HH(c, d, a, b, x[3], 16, 0xd4ef3085);   /* 43 */
    HH(b, c, d, a, x[6], 23, 0x4881d05);    /* 44 */
    HH(a, b, c, d, x[9], 4, 0xd9d4d039);    /* 45 */
    HH(d, a, b, c, x[12], 11, 0xe6db99e5);  /* 46 */
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8);  /* 47 */
    HH(b, c, d, a, x[2], 23, 0xc4ac5665);   /* 48 */

    II(a, b, c, d, x[0], 6, 0xf4292244);    /* 49 */
    II(d, a, b, c, x[7], 10, 0x432aff97);   /* 50 */
    II(c, d, a, b, x[14], 15, 0xab9423a7);  /* 51 */
    II(b, c, d, a, x[5], 21, 0xfc93a039);   /* 52 */
    II(a, b, c, d, x[12], 6, 0x655b59c3);   /* 53 */
    II(d, a, b, c, x[3], 10, 0x8f0ccc92);   /* 54 */
    II(c, d, a, b, x[10], 15, 0xffeff47d);  /* 55 */
    II(b, c, d, a, x[1], 21, 0x85845dd1);   /* 56 */
    II(a, b, c, d, x[8], 6, 0x6fa87e4f);    /* 57 */
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0);  /* 58 */
    II(c, d, a, b, x[6], 15, 0xa3014314);   /* 59 */
    II(b, c, d, a, x[13], 21, 0x4e0811a1);  /* 60 */
    II(a, b, c, d, x[4], 6, 0xf7537e82);    /* 61 */
    II(d, a, b, c, x[11], 10, 0xbd3af235);  /* 62 */
    II(c, d, a, b, x[2], 15, 0x2ad7d2bb);   /* 63 */
    II(b, c, d, a, x[9], 21, 0xeb86d391);   /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

/* Digests a string. */

void            MD5digest(unsigned char *ptr, int len, unsigned char *digest)
{
    struct {
        unsigned long   state[4],
                        count[2];
        unsigned char   buffer[64];
    }
                    context;

    memset(&context, 0, sizeof(context));
    context.state[0] = 0x67452301;
    context.state[1] = 0xefcdab89;
    context.state[2] = 0x98badcfe;
    context.state[3] = 0x10325476;
    context.count[0] = (unsigned long) len << 3;
    for (; len >= 64; len -= 64, ptr += 64)
        MD5Transform(context.state, ptr);
    memcpy(context.buffer, ptr, len);
    Encode(digest, context.count, 2);
    context.buffer[len] = 0x80;
    if (len >= 56) {
        MD5Transform(context.state, context.buffer);
        memset(context.buffer, 0, 56);
    }
    memcpy(context.buffer + 56, digest, 8);
    MD5Transform(context.state, context.buffer);
    Encode(digest, context.state, 4);
}
