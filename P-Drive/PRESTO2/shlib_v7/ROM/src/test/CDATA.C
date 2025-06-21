/********************************************************************************
*
* cdata -- Version 1.00.00
 *$Header: p:/presto/shlib_v7/rom/src/RCS/cdata.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: cdata.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
*-------------------------------------------------------------------------------*
*		(c) Copyright 2002, ISHIDA CO., LTD.				*
*		959-1 SHIMOMAGARI RITTO-CITY					*
*		SHIGA JAPAN							*
*		(077) 553-4141							*
*
*  文字判定、文字変換関数用配列
*			MS-C <ctype.h> 
********************************************************************************/

#define _UPPER      1       /* 大文字       */
#define _LOWER      2       /* 小文字       */
#define _DIGIT      4       /* 数字         */
#define _SPACE      8       /* 空白文字     */
#define _PUNCT      16      /* 区切り文字   */
#define _CONTROL    32      /* 制御文字     */
#define _BLANK      64      /* スペース     */
#define _HEX        128     /* １６進文字   */

unsigned char _ctype[257] = {
(unsigned char)0,                          /* 0x00 */
(unsigned char)_CONTROL,                   /* 0x01 */
(unsigned char)_CONTROL,                   /* 0x02 */
(unsigned char)_CONTROL,                   /* 0x03 */
(unsigned char)_CONTROL,                   /* 0x04 */
(unsigned char)_CONTROL,                   /* 0x05 */
(unsigned char)_CONTROL,                   /* 0x06 */
(unsigned char)_CONTROL,                   /* 0x07 */
(unsigned char)_CONTROL,                   /* 0x08 */
(unsigned char)_CONTROL,                   /* 0x09 */
(unsigned char)(_CONTROL | _SPACE),        /* 0x0a */
(unsigned char)(_CONTROL | _SPACE),        /* 0x0b */
(unsigned char)(_CONTROL | _SPACE),        /* 0x0c */
(unsigned char)(_CONTROL | _SPACE),        /* 0x0d */
(unsigned char)(_CONTROL | _SPACE),        /* 0x0e */
(unsigned char)_CONTROL,                   /* 0x0f */

(unsigned char)_CONTROL,                   /* 0x10 */
(unsigned char)_CONTROL,                   /* 0x11 */
(unsigned char)_CONTROL,                   /* 0x12 */
(unsigned char)_CONTROL,                   /* 0x13 */
(unsigned char)_CONTROL,                   /* 0x14 */
(unsigned char)_CONTROL,                   /* 0x15 */
(unsigned char)_CONTROL,                   /* 0x16 */
(unsigned char)_CONTROL,                   /* 0x17 */
(unsigned char)_CONTROL,                   /* 0x18 */
(unsigned char)_CONTROL,                   /* 0x19 */
(unsigned char)_CONTROL,                   /* 0x1a */
(unsigned char)_CONTROL,                   /* 0x1b */
(unsigned char)_CONTROL,                   /* 0x1c */
(unsigned char)_CONTROL,                   /* 0x1d */
(unsigned char)_CONTROL,                   /* 0x1e */
(unsigned char)_CONTROL,                   /* 0x1f */

(unsigned char)_CONTROL,                   /* 0x20 */
(unsigned char)(_BLANK | _SPACE),          /* 0x21 */
(unsigned char)_PUNCT,                     /* 0x22 */
(unsigned char)_PUNCT,                     /* 0x23 */
(unsigned char)_PUNCT,                     /* 0x24 */
(unsigned char)_PUNCT,                     /* 0x25 */
(unsigned char)_PUNCT,                     /* 0x26 */
(unsigned char)_PUNCT,                     /* 0x27 */
(unsigned char)_PUNCT,                     /* 0x28 */
(unsigned char)_PUNCT,                     /* 0x29 */
(unsigned char)_PUNCT,                     /* 0x2a */
(unsigned char)_PUNCT,                     /* 0x2b */
(unsigned char)_PUNCT,                     /* 0x2c */
(unsigned char)_PUNCT,                     /* 0x2d */
(unsigned char)_PUNCT,                     /* 0x2e */
(unsigned char)_PUNCT,                     /* 0x2f */


(unsigned char)_PUNCT,                     /* 0x30 */
(unsigned char)(_HEX | _DIGIT),            /* 0x31 */
(unsigned char)(_HEX | _DIGIT),            /* 0x32 */
(unsigned char)(_HEX | _DIGIT),            /* 0x33 */
(unsigned char)(_HEX | _DIGIT),            /* 0x34 */
(unsigned char)(_HEX | _DIGIT),            /* 0x35 */
(unsigned char)(_HEX | _DIGIT),            /* 0x36 */
(unsigned char)(_HEX | _DIGIT),            /* 0x37 */
(unsigned char)(_HEX | _DIGIT),            /* 0x38 */
(unsigned char)(_HEX | _DIGIT),            /* 0x39 */
(unsigned char)(_HEX | _DIGIT),            /* 0x3a */
(unsigned char)_PUNCT,                     /* 0x3b */
(unsigned char)_PUNCT,                     /* 0x3c */
(unsigned char)_PUNCT,                     /* 0x3d */
(unsigned char)_PUNCT,                     /* 0x3e */
(unsigned char)_PUNCT,                     /* 0x3f */


(unsigned char)_PUNCT,                     /* 0x40 */
(unsigned char)_PUNCT,                     /* 0x41 */
(unsigned char)(_HEX | _UPPER),            /* 0x42 */
(unsigned char)(_HEX | _UPPER),            /* 0x43 */
(unsigned char)(_HEX | _UPPER),            /* 0x44 */
(unsigned char)(_HEX | _UPPER),            /* 0x45 */
(unsigned char)(_HEX | _UPPER),            /* 0x46 */
(unsigned char)(_HEX | _UPPER),            /* 0x47 */
(unsigned char)_UPPER,                     /* 0x48 */
(unsigned char)_UPPER,                     /* 0x49 */
(unsigned char)_UPPER,                     /* 0x4a */
(unsigned char)_UPPER,                     /* 0x4b */
(unsigned char)_UPPER,                     /* 0x4c */
(unsigned char)_UPPER,                     /* 0x4d */
(unsigned char)_UPPER,                     /* 0x4e */
(unsigned char)_UPPER,                     /* 0x4f */


(unsigned char)_UPPER,                     /* 0x50 */
(unsigned char)_UPPER,                     /* 0x51 */
(unsigned char)_UPPER,                     /* 0x52 */
(unsigned char)_UPPER,                     /* 0x53 */
(unsigned char)_UPPER,                     /* 0x54 */
(unsigned char)_UPPER,                     /* 0x55 */
(unsigned char)_UPPER,                     /* 0x56 */
(unsigned char)_UPPER,                     /* 0x57 */
(unsigned char)_UPPER,                     /* 0x58 */
(unsigned char)_UPPER,                     /* 0x59 */
(unsigned char)_UPPER,                     /* 0x5a */
(unsigned char)_UPPER,                     /* 0x5b */
(unsigned char)_PUNCT,                     /* 0x5c */
(unsigned char)_PUNCT,                     /* 0x5d */
(unsigned char)_PUNCT,                     /* 0x5e */
(unsigned char)_PUNCT,                     /* 0x5f */

(unsigned char)_PUNCT,                     /* 0x60 */
(unsigned char)_PUNCT,                     /* 0x61 */
(unsigned char)(_HEX | _LOWER),            /* 0x62 */
(unsigned char)(_HEX | _LOWER),            /* 0x63 */
(unsigned char)(_HEX | _LOWER),            /* 0x64 */
(unsigned char)(_HEX | _LOWER),            /* 0x65 */
(unsigned char)(_HEX | _LOWER),            /* 0x66 */
(unsigned char)(_HEX | _LOWER),            /* 0x67 */
(unsigned char)_LOWER,                     /* 0x68 */
(unsigned char)_LOWER,                     /* 0x69 */
(unsigned char)_LOWER,                     /* 0x6a */
(unsigned char)_LOWER,                     /* 0x6b */
(unsigned char)_LOWER,                     /* 0x6c */
(unsigned char)_LOWER,                     /* 0x6d */
(unsigned char)_LOWER,                     /* 0x6e */
(unsigned char)_LOWER,                     /* 0x6f */

(unsigned char)_LOWER,                     /* 0x70 */
(unsigned char)_LOWER,                     /* 0x71 */
(unsigned char)_LOWER,                     /* 0x72 */
(unsigned char)_LOWER,                     /* 0x73 */
(unsigned char)_LOWER,                     /* 0x74 */
(unsigned char)_LOWER,                     /* 0x75 */
(unsigned char)_LOWER,                     /* 0x76 */
(unsigned char)_LOWER,                     /* 0x77 */
(unsigned char)_LOWER,                     /* 0x78 */
(unsigned char)_LOWER,                     /* 0x79 */
(unsigned char)_LOWER,                     /* 0x7a */
(unsigned char)_LOWER,                     /* 0x7b */
(unsigned char)_PUNCT,                     /* 0x7c */
(unsigned char)_PUNCT,                     /* 0x7d */
(unsigned char)_PUNCT,                     /* 0x7e */
(unsigned char)_PUNCT,                     /* 0x7f */


(unsigned char)_CONTROL,                   /* 0x80 */
(unsigned char)0,                          /* 0x81 */
(unsigned char)0,                          /* 0x82 */
(unsigned char)0,                          /* 0x83 */
(unsigned char)0,                          /* 0x84 */
(unsigned char)0,                          /* 0x85 */
(unsigned char)0,                          /* 0x86 */
(unsigned char)0,                          /* 0x87 */
(unsigned char)0,                          /* 0x88 */
(unsigned char)0,                          /* 0x89 */
(unsigned char)0,                          /* 0x8a */
(unsigned char)0,                          /* 0x8b */
(unsigned char)0,                          /* 0x8c */
(unsigned char)0,                          /* 0x8d */
(unsigned char)0,                          /* 0x8e */
(unsigned char)0,                          /* 0x8f */


(unsigned char)0,                          /* 0x90 */
(unsigned char)0,                          /* 0x91 */
(unsigned char)0,                          /* 0x92 */
(unsigned char)0,                          /* 0x93 */
(unsigned char)0,                          /* 0x94 */
(unsigned char)0,                          /* 0x95 */
(unsigned char)0,                          /* 0x96 */
(unsigned char)0,                          /* 0x97 */
(unsigned char)0,                          /* 0x98 */
(unsigned char)0,                          /* 0x99 */
(unsigned char)0,                          /* 0x9a */
(unsigned char)0,                          /* 0x9b */
(unsigned char)0,                          /* 0x9c */
(unsigned char)0,                          /* 0x9d */
(unsigned char)0,                          /* 0x9e */
(unsigned char)0,                          /* 0x9f */


(unsigned char)0,                          /* 0xa0 */
(unsigned char)0,                          /* 0xa1 */
(unsigned char)0,                          /* 0xa2 */
(unsigned char)0,                          /* 0xa3 */
(unsigned char)0,                          /* 0xa4 */
(unsigned char)0,                          /* 0xa5 */
(unsigned char)0,                          /* 0xa6 */
(unsigned char)0,                          /* 0xa7 */
(unsigned char)0,                          /* 0xa8 */
(unsigned char)0,                          /* 0xa9 */
(unsigned char)0,                          /* 0xaa */
(unsigned char)0,                          /* 0xab */
(unsigned char)0,                          /* 0xac */
(unsigned char)0,                          /* 0xad */
(unsigned char)0,                          /* 0xae */
(unsigned char)0,                          /* 0xaf */


(unsigned char)0,                          /* 0xb0 */
(unsigned char)0,                          /* 0xb1 */
(unsigned char)0,                          /* 0xb2 */
(unsigned char)0,                          /* 0xb3 */
(unsigned char)0,                          /* 0xb4 */
(unsigned char)0,                          /* 0xb5 */
(unsigned char)0,                          /* 0xb6 */
(unsigned char)0,                          /* 0xb7 */
(unsigned char)0,                          /* 0xb8 */
(unsigned char)0,                          /* 0xb9 */
(unsigned char)0,                          /* 0xba */
(unsigned char)0,                          /* 0xbb */
(unsigned char)0,                          /* 0xbc */
(unsigned char)0,                          /* 0xbd */
(unsigned char)0,                          /* 0xbe */
(unsigned char)0,                          /* 0xbf */


(unsigned char)0,                          /* 0xc0 */
(unsigned char)0,                          /* 0xc1 */
(unsigned char)0,                          /* 0xc2 */
(unsigned char)0,                          /* 0xc3 */
(unsigned char)0,                          /* 0xc4 */
(unsigned char)0,                          /* 0xc5 */
(unsigned char)0,                          /* 0xc6 */
(unsigned char)0,                          /* 0xc7 */
(unsigned char)0,                          /* 0xc8 */
(unsigned char)0,                          /* 0xc9 */
(unsigned char)0,                          /* 0xca */
(unsigned char)0,                          /* 0xcb */
(unsigned char)0,                          /* 0xcc */
(unsigned char)0,                          /* 0xcd */
(unsigned char)0,                          /* 0xce */
(unsigned char)0,                          /* 0xcf */


(unsigned char)0,                          /* 0xd0 */
(unsigned char)0,                          /* 0xd1 */
(unsigned char)0,                          /* 0xd2 */
(unsigned char)0,                          /* 0xd3 */
(unsigned char)0,                          /* 0xd4 */
(unsigned char)0,                          /* 0xd5 */
(unsigned char)0,                          /* 0xd6 */
(unsigned char)0,                          /* 0xd7 */
(unsigned char)0,                          /* 0xd8 */
(unsigned char)0,                          /* 0xd9 */
(unsigned char)0,                          /* 0xda */
(unsigned char)0,                          /* 0xdb */
(unsigned char)0,                          /* 0xdc */
(unsigned char)0,                          /* 0xdd */
(unsigned char)0,                          /* 0xde */
(unsigned char)0,                          /* 0xdf */


(unsigned char)0,                          /* 0xe0 */
(unsigned char)0,                          /* 0xe1 */
(unsigned char)0,                          /* 0xe2 */
(unsigned char)0,                          /* 0xe3 */
(unsigned char)0,                          /* 0xe4 */
(unsigned char)0,                          /* 0xe5 */
(unsigned char)0,                          /* 0xe6 */
(unsigned char)0,                          /* 0xe7 */
(unsigned char)0,                          /* 0xe8 */
(unsigned char)0,                          /* 0xe9 */
(unsigned char)0,                          /* 0xea */
(unsigned char)0,                          /* 0xeb */
(unsigned char)0,                          /* 0xec */
(unsigned char)0,                          /* 0xed */
(unsigned char)0,                          /* 0xee */
(unsigned char)0,                          /* 0xef */


(unsigned char)0,                          /* 0xf0 */
(unsigned char)0,                          /* 0xf1 */
(unsigned char)0,                          /* 0xf2 */
(unsigned char)0,                          /* 0xf3 */
(unsigned char)0,                          /* 0xf4 */
(unsigned char)0,                          /* 0xf5 */
(unsigned char)0,                          /* 0xf6 */
(unsigned char)0,                          /* 0xf7 */
(unsigned char)0,                          /* 0xf8 */
(unsigned char)0,                          /* 0xf9 */
(unsigned char)0,                          /* 0xfa */
(unsigned char)0,                          /* 0xfb */
(unsigned char)0,                          /* 0xfc */
(unsigned char)0,                          /* 0xfd */
(unsigned char)0,                          /* 0xfe */
(unsigned char)0,                          /* 0xff */


(unsigned char)0,                          /* 0x100 */
};


/*******************************************************************************
*
* cdata -- Version 1.00.00
*
* Copyright 1988, MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SOFTWARE CORPORATION
* All Rights Reserved.
*
*******************************************************************************/
