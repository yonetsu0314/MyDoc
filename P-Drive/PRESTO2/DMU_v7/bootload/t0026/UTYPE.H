/*======================================================================
 * File name    : utype.h
 * Original		: p:/dacs_i/rcu/main/rcs/utype.h 1.3
 *----------------------------------------------------------------------
 * Function 	:
 *		組込みシステム用データ型定義
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/utype.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: utype.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#ifndef	_UTYPE_H_
#define	_UTYPE_H_

#ifndef _TYPEDEFS
typedef	unsigned char	BYTE;		/* 8-bit data */
typedef	unsigned short	WORD;		/* 16-bit data */
typedef	unsigned long	DWORD;		/* 32-bit data */

typedef union {						/* 64-bit data */
	BYTE	byte[8];
	WORD	word[4];
	DWORD	dword[2];
}
	QWORD;
typedef	QWORD  *	QWORD_PTR ;	/* Pointer to 64-bit data */
#endif	/* _TYPEDEFS */

#define	Low(word)		((word) & 0x00ff)
#define	High(word)		Low((word) >> 8)

#define	Lword(dword)	((WORD)((dword) & 0x0000ffff))
#define	Hword(dword)	Lword((dword) >> 16)

#define	QDL				0
#define	QDH				1

#define	Dim(x)			(sizeof(x) / sizeof(x[0]))

#endif	/* _UTYPE_H_ */
