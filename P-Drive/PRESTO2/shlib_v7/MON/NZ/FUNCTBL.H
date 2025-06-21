/*----------------------------------------------------------------------*
 * File name	: functbl.h	/ function table			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/functbl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: functbl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/functbl.h 1.1 1996/07/22 19:26:05
 *Log: functbl.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#define	INITIAL_FUNC	35
typedef void	(*VN_FUNC)();

#ifdef	STORAGE
VN_FUNC const	func_tbl[] = {
	__func_0,			/* No= 0	Key: 0		*/
	__func_1,			/* No= 1	Key: 1		*/
	__func_2,			/* No= 2	Key: 2		*/
	__func_3,			/* No= 3	Key: 3		*/
	__func_4,			/* No= 4	Key: 4		*/
	__func_5,			/* No= 5	Key: 5		*/
	__func_6,			/* No= 6	Key: 6		*/
	__func_7,			/* No= 7	Key: 7		*/
	__func_8,			/* No= 8	Key: 8		*/
	__func_9,			/* No= 9	Key: 9		*/
	__func_a,			/* No= 10	Key: a		*/
	__func_b,			/* No= 11	Key: b		*/
	__func_c,			/* No= 12	Key: c		*/
	__func_d,			/* No= 13	Key: d		*/
	__func_e,			/* No= 14	Key: e		*/
	__func_f,			/* No= 15	Key: f		*/
	__func_g,			/* No= 16	Key: g		*/
	__func_h,			/* No= 17	Key: h		*/
	__func_i,			/* No= 18	Key: i		*/
	__func_j,			/* No= 19	Key: j		*/
	__func_k,			/* No= 20	Key: k		*/
	__func_l,			/* No= 21	Key: l		*/
	__func_m,			/* No= 22	Key: m		*/
	__func_n,			/* No= 23	Key: n		*/
	__func_o,			/* No= 24	Key: o		*/
	__func_p,			/* No= 25	Key: p		*/
	__func_q,			/* No= 26	Key: q		*/
	__func_r,			/* No= 27	Key: r		*/
	__func_s,			/* No= 28	Key: s		*/
	__func_t,			/* No= 29	Key: t		*/
	__func_u,			/* No= 30	Key: u		*/
	__func_v,			/* No= 31	Key: v		*/
	__func_w,			/* No= 32	Key: w		*/
	__func_x,			/* No= 33	Key: x		*/
	__func_y,			/* No= 34	Key: y		*/
	__func_z			/* No= 35	Key: z		*/
};
#else
extern VN_FUNC const	func_tbl[];
#endif
