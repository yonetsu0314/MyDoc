/*----------------------------------------------------------------------*
 * File name	: cdata.c												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/cdata.c 1.1 1996/01/19 09:42:36 kabumoto Exp $
 *$Log: cdata.c $
 * リビジョン 1.1  1996/01/19  09:42:36  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/* 				Copyright 1988, MITSUBISHI ELECTRIC CORPORATION			*/
/* 				AND MITSUBISHI ELECTRIC SEMICONDUCTOR SOFTWARE			*/
/*				CORPORATION	All Rights Reserved.						*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*  	文字判定、文字変換関数用配列									*/
/*----------------------------------------------------------------------*/

#define _UPPER      1       /* 大文字       */
#define _LOWER      2       /* 小文字       */
#define _DIGIT      4       /* 数字         */
#define _SPACE      8       /* 空白文字     */
#define _PUNCT      16      /* 区切り文字   */
#define _CONTROL    32      /* 制御文字     */
#define _BLANK      64      /* スペース     */
#define _HEX        128     /* １６進文字   */

unsigned char _ctype[257] = {
0,                          /* 0x00 */
_CONTROL,                   /* 0x01 */
_CONTROL,                   /* 0x02 */
_CONTROL,                   /* 0x03 */
_CONTROL,                   /* 0x04 */
_CONTROL,                   /* 0x05 */
_CONTROL,                   /* 0x06 */
_CONTROL,                   /* 0x07 */
_CONTROL,                   /* 0x08 */
_CONTROL,                   /* 0x09 */
_CONTROL | _SPACE,          /* 0x0a */
_CONTROL | _SPACE,          /* 0x0b */
_CONTROL | _SPACE,          /* 0x0c */
_CONTROL | _SPACE,          /* 0x0d */
_CONTROL | _SPACE,          /* 0x0e */
_CONTROL,                   /* 0x0f */

_CONTROL,                   /* 0x10 */
_CONTROL,                   /* 0x11 */
_CONTROL,                   /* 0x12 */
_CONTROL,                   /* 0x13 */
_CONTROL,                   /* 0x14 */
_CONTROL,                   /* 0x15 */
_CONTROL,                   /* 0x16 */
_CONTROL,                   /* 0x17 */
_CONTROL,                   /* 0x18 */
_CONTROL,                   /* 0x19 */
_CONTROL,                   /* 0x1a */
_CONTROL,                   /* 0x1b */
_CONTROL,                   /* 0x1c */
_CONTROL,                   /* 0x1d */
_CONTROL,                   /* 0x1e */
_CONTROL,                   /* 0x1f */

_CONTROL,                   /* 0x20 */
_BLANK | _SPACE,            /* 0x21 */
_PUNCT,                     /* 0x22 */
_PUNCT,                     /* 0x23 */
_PUNCT,                     /* 0x24 */
_PUNCT,                     /* 0x25 */
_PUNCT,                     /* 0x26 */
_PUNCT,                     /* 0x27 */
_PUNCT,                     /* 0x28 */
_PUNCT,                     /* 0x29 */
_PUNCT,                     /* 0x2a */
_PUNCT,                     /* 0x2b */
_PUNCT,                     /* 0x2c */
_PUNCT,                     /* 0x2d */
_PUNCT,                     /* 0x2e */
_PUNCT,                     /* 0x2f */


_PUNCT,                     /* 0x30 */
_HEX | _DIGIT,              /* 0x31 */
_HEX | _DIGIT,              /* 0x32 */
_HEX | _DIGIT,              /* 0x33 */
_HEX | _DIGIT,              /* 0x34 */
_HEX | _DIGIT,              /* 0x35 */
_HEX | _DIGIT,              /* 0x36 */
_HEX | _DIGIT,              /* 0x37 */
_HEX | _DIGIT,              /* 0x38 */
_HEX | _DIGIT,              /* 0x39 */
_HEX | _DIGIT,              /* 0x3a */
_PUNCT,                     /* 0x3b */
_PUNCT,                     /* 0x3c */
_PUNCT,                     /* 0x3d */
_PUNCT,                     /* 0x3e */
_PUNCT,                     /* 0x3f */


_PUNCT,                     /* 0x40 */
_PUNCT,                     /* 0x41 */
_HEX | _UPPER,              /* 0x42 */
_HEX | _UPPER,              /* 0x43 */
_HEX | _UPPER,              /* 0x44 */
_HEX | _UPPER,              /* 0x45 */
_HEX | _UPPER,              /* 0x46 */
_HEX | _UPPER,              /* 0x47 */
_UPPER,                     /* 0x48 */
_UPPER,                     /* 0x49 */
_UPPER,                     /* 0x4a */
_UPPER,                     /* 0x4b */
_UPPER,                     /* 0x4c */
_UPPER,                     /* 0x4d */
_UPPER,                     /* 0x4e */
_UPPER,                     /* 0x4f */


_UPPER,                     /* 0x50 */
_UPPER,                     /* 0x51 */
_UPPER,                     /* 0x52 */
_UPPER,                     /* 0x53 */
_UPPER,                     /* 0x54 */
_UPPER,                     /* 0x55 */
_UPPER,                     /* 0x56 */
_UPPER,                     /* 0x57 */
_UPPER,                     /* 0x58 */
_UPPER,                     /* 0x59 */
_UPPER,                     /* 0x5a */
_UPPER,                     /* 0x5b */
_PUNCT,                     /* 0x5c */
_PUNCT,                     /* 0x5d */
_PUNCT,                     /* 0x5e */
_PUNCT,                     /* 0x5f */

_PUNCT,                     /* 0x60 */
_PUNCT,                     /* 0x61 */
_HEX | _LOWER,              /* 0x62 */
_HEX | _LOWER,              /* 0x63 */
_HEX | _LOWER,              /* 0x64 */
_HEX | _LOWER,              /* 0x65 */
_HEX | _LOWER,              /* 0x66 */
_HEX | _LOWER,              /* 0x67 */
_LOWER,                     /* 0x68 */
_LOWER,                     /* 0x69 */
_LOWER,                     /* 0x6a */
_LOWER,                     /* 0x6b */
_LOWER,                     /* 0x6c */
_LOWER,                     /* 0x6d */
_LOWER,                     /* 0x6e */
_LOWER,                     /* 0x6f */

_LOWER,                     /* 0x70 */
_LOWER,                     /* 0x71 */
_LOWER,                     /* 0x72 */
_LOWER,                     /* 0x73 */
_LOWER,                     /* 0x74 */
_LOWER,                     /* 0x75 */
_LOWER,                     /* 0x76 */
_LOWER,                     /* 0x77 */
_LOWER,                     /* 0x78 */
_LOWER,                     /* 0x79 */
_LOWER,                     /* 0x7a */
_LOWER,                     /* 0x7b */
_PUNCT,                     /* 0x7c */
_PUNCT,                     /* 0x7d */
_PUNCT,                     /* 0x7e */
_PUNCT,                     /* 0x7f */


_CONTROL,                   /* 0x80 */
0,                          /* 0x81 */
0,                          /* 0x82 */
0,                          /* 0x83 */
0,                          /* 0x84 */
0,                          /* 0x85 */
0,                          /* 0x86 */
0,                          /* 0x87 */
0,                          /* 0x88 */
0,                          /* 0x89 */
0,                          /* 0x8a */
0,                          /* 0x8b */
0,                          /* 0x8c */
0,                          /* 0x8d */
0,                          /* 0x8e */
0,                          /* 0x8f */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0};
