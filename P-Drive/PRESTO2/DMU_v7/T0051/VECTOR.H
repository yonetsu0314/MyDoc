/*======================================================================
 * File name    : vector.h
 * Original		: p:/dacs_i/rcu/main/rcs/vector.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		ベクタテーブル用のプロトタイプ宣言ヘッダ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/vector.h 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: vector.h $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
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
#ifndef	_VECTOR_H_
#define	_VECTOR_H_

INTHDR pint0_int_hdr(void);
INTHDR pint1_int_hdr(void);
INTHDR pint2_int_hdr(void);
INTHDR pint3_int_hdr(void);

INTHDR arcnet0_int_hdr_(void);
INTHDR arcnet1_int_hdr_(void);

INTHDR INIT( void );
INTHDR UBrk(void);
INTHDR GnrlIlgl(void);
INTHDR ADDerror(void);
INTHDR Slot(void);
INTHDR DMAerror(void);
INTHDR scu_io(void);
INTHDR tcu_io(void);
INTHDR mpsc_io(void);
INTHDR port_io(void);
INTHDR icu_io(void);
INTHDR e2rom_io(void);
INTHDR rtc_io(void);
INTHDR sram_io(void);

INTHDR dum0(void);
INTHDR dum1(void);
INTHDR dum2(void);
INTHDR dum3(void);
INTHDR dum4(void);
INTHDR dum5(void);
INTHDR dum6(void);
INTHDR dum7(void);
INTHDR dum8(void);
INTHDR dum9(void);
INTHDR dum10(void);
INTHDR dum11(void);
INTHDR dum12(void);
INTHDR dum13(void);
INTHDR dum14(void);
INTHDR dum15(void);
INTHDR dum16(void);
INTHDR dum17(void);
INTHDR dum18(void);
INTHDR dum19(void);
INTHDR dum20(void);
INTHDR dum21(void);
INTHDR dum22(void);
INTHDR dum23(void);
INTHDR dum24(void);
INTHDR dum25(void);
INTHDR dum26(void);
INTHDR dum27(void);
INTHDR dum28(void);
INTHDR dum29(void);
INTHDR dum30(void);
INTHDR dum31(void);
INTHDR dum32(void);
INTHDR dum33(void);
INTHDR dum34(void);
INTHDR dum35(void);
INTHDR dum36(void);
INTHDR dum37(void);
INTHDR dum38(void);
INTHDR dum39(void);
INTHDR dum40(void);
INTHDR dum41(void);
INTHDR dum42(void);
INTHDR dum43(void);
INTHDR dum44(void);
INTHDR dum45(void);
INTHDR dum46(void);
INTHDR dum47(void);
INTHDR dum48(void);
INTHDR dum49(void);
INTHDR dum50(void);
INTHDR dum51(void);
INTHDR dum52(void);
INTHDR dum53(void);
INTHDR dum54(void);
INTHDR dum55(void);
INTHDR dum56(void);
INTHDR dum57(void);
INTHDR dum58(void);
INTHDR dum59(void);
INTHDR dum60(void);
INTHDR dum61(void);
INTHDR dum62(void);
INTHDR dum63(void);
INTHDR dum64(void);
INTHDR dum65(void);
INTHDR dum66(void);
INTHDR dum67(void);
INTHDR dum68(void);
INTHDR dum69(void);
INTHDR dum70(void);
INTHDR dum71(void);
INTHDR dum72(void);
INTHDR dum73(void);
INTHDR dum74(void);
INTHDR dum75(void);
INTHDR dum76(void);
INTHDR dum77(void);
INTHDR dum78(void);
INTHDR dum79(void);
INTHDR dum80(void);
INTHDR dum81(void);
INTHDR dum82(void);
INTHDR dum83(void);
INTHDR dum84(void);
INTHDR dum85(void);
INTHDR dum86(void);
INTHDR dum87(void);
INTHDR dum88(void);
INTHDR dum89(void);
INTHDR dum90(void);
INTHDR dum91(void);
INTHDR dum92(void);
INTHDR dum93(void);
INTHDR dum94(void);
INTHDR dum95(void);
INTHDR dum96(void);
INTHDR dum97(void);
INTHDR dum98(void);
INTHDR dum99(void);
INTHDR dum100(void);
INTHDR dum101(void);
INTHDR dum102(void);
INTHDR dum103(void);
INTHDR dum104(void);
INTHDR dum105(void);
INTHDR dum106(void);
INTHDR dum107(void);
INTHDR dum108(void);
INTHDR dum109(void);
INTHDR dum110(void);
INTHDR dum111(void);
INTHDR dum112(void);
INTHDR dum113(void);
INTHDR dum114(void);
INTHDR dum115(void);
INTHDR dum116(void);
INTHDR dum117(void);
INTHDR dum118(void);
INTHDR dum119(void);
INTHDR dum120(void);
INTHDR dum121(void);
INTHDR dum122(void);
INTHDR dum123(void);
INTHDR dum124(void);
INTHDR dum125(void);
INTHDR dum126(void);
INTHDR dum127(void);

#endif
