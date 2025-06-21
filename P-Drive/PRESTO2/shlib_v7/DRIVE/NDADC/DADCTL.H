/*----------------------------------------------------------------------*
 * File name	: dadctl.h	/ dadc control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto/shlib_v7/drive/ndadc/RCS/dadctl.h 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadctl.h $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/08/26 20:00
 * 符号有り整数をエンディアン変換する場合、符号拡張されないようにする。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/30  15:50
 * ・基板(P5561/P5554)に対応させた。
 *   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
 *   定義しないことでP5554(村田氏作成)に対応させた。
 *   
 * ・エンディアン(ビッグ/リトル)に対応させた。
 *   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
 *   定義しないことでリトルエンディアンに対応させた。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dadctl.h 1.1 1996/07/22 13:37:30 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#if	!defined(DADCTL_H)
#define	DADCTL_H

/* 論理反転マクロ */
#ifdef	MAS_P5561	/* ← メークファイルで定義：定義すれば坂口さんボード(P5561)用となり、定義しなければ村田さんボード(P5554)用となる。*/
#define	INVERSION(a)		a=(~a)
#ifdef	__DADINV_C__
int	verify_5561puti(unsigned short dat, volatile unsigned short *p);	/* DADINV.C */
long	verify_5561geti(volatile unsigned short *p);				/* DADINV.C */
void	inverse_dadc(char *buf, short len);					/* DADINV.C */
#else
#define	verify_puti	verify_5561puti
#define	verify_geti	verify_5561geti
extern int	verify_5561puti(unsigned short dat, volatile unsigned short *p);	/* DADINV.C */
extern long	verify_5561geti(volatile unsigned short *p);				/* DADINV.C */
extern void	inverse_dadc(char *buf, short len);					/* DADINV.C */
#endif	/* __DADINV_C__ */
#endif	/* MAS_P5561 */

/* エンディアン変換マクロ */
#ifdef	MAS_BIG_ENDIAN	/* ← メークファイルで定義：定義すればビッグエンディアンに、定義しなければリトルエンディアンになる。*/
#define	USS	unsigned short
#define	USL	unsigned long
#define	CHG_UL_SHORT(a)		a=(short)(((a&0x00ff)<<8)|((USS)(a&0xff00)>>8))
#define	CHG_UL_USHORT(a)	a=(unsigned short)(((a&0x00ff)<<8)|((a&0xff00)>>8))
#define	CHG_UL_LONG(a)		a=((a&0x000000ff)<<24)|((a&0x0000ff00)<<8)|((a&0x00ff0000)>>8)|(((USL)(a&0xff000000))>>24)
#define	CHG_UL_ULONG(a)		a=((a&0x000000ff)<<24)|((a&0x0000ff00)<<8)|((a&0x00ff0000)>>8)|((a&0xff000000)>>24)
#endif	/* MAS_BIG_ENDIAN */

void			abort(void);

int		init_dadi_tsk(DAD_ID pwb);
int		init_dad_dpm_read(DAD_ID pwb);
int		init_dad_dpm_write(DAD_ID pwb);
int		dad_dpm_read(DAD_ID pwb, void *buf, unsigned long max,
				unsigned short offset);
int		dad_dpm_write(DAD_ID pwb, void *ret, void *buf, short len);
int		wai_span_flg(DAD_ID pwb);
int		wai_afv_flg(DAD_ID pwb);
int		wai_afv_msg(DAD_ID *pwb);
int		wai_init_boot_flg(DAD_ID pwb, int bp);
int		wai_chg_boot_flg(DAD_ID pwb, int bp);
int		wai_zero_flg(DAD_ID pwb);
int		wai_mem_read_flg(DAD_ID pwb);
int		wai_mem_write_flg(DAD_ID pwb);
int		wai_sta_mon_flg(DAD_ID pwb);
int		wai_stp_mon_flg(DAD_ID pwb);

/*------------------------------------------------------*/
/*	APL側に関数経由で公開されるﾃﾞｰﾀ			*/
/*							*/
/*------------------------------------------------------*/

#ifdef	STORAGE
struct dadsck	dad_slf_chk[DAD_MAX] = {
			(short)0, (short)0	/* self checkの結果ﾃﾞｰﾀ		*/
};
struct dades10	dad_afv_err[DAD_MAX] = {
			0L, (short)0		/* AFV error status		*/
};
#else
extern struct dadsck	dad_slf_chk[];
extern struct dades10	dad_afv_err[];
#endif


#if	!defined(V_FUNC_T)
typedef	void	(*V_FUNC)();		/* voidを返す関数へのﾎﾟｲﾝﾀ	*/
#define	V_FUNC_T

#endif
typedef struct	ucsinf {
	short		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCSINF;

typedef struct base {
	short		wei_max;	/* DSP最大ｳｪｲ･ﾍｯﾄﾞ数		*/
	short		df_max;		/* DSP最大分散ﾌｨｰﾀﾞ数		*/
	short		afv_max;	/* DSP最大AFVｾﾙ数		*/
} BASE;

#ifdef	STORAGE
struct ucsinf	dad_icu_inf[DAD_MAX] = {/* ADC INTが接続されているICU	*/
			(short)-1,(unsigned char)-1,(unsigned char)-1,
			(short)-1,(unsigned char)-1,(unsigned char)-1,
};
V_FUNC const	dad_int_hdr[DAD_MAX] = {
			dadc0_int_hdr, dadc1_int_hdr
};
struct base	dad_base[] = {
			(short)DAD_WEI_MAX, (short)DAD_DF_MAX, (short)DAD_AFV_MAX,
			(short)DAD_WEI_MAX, (short)DAD_DF_MAX, (short)DAD_AFV_MAX
};
#else
extern struct ucsinf	dad_icu_inf[];
extern V_FUNC const	dad_int_hdr[];
extern struct base	dad_base[];
#endif

/*------------------------------------------------------*/
/*	DADCのコマンド定義				*/
/*							*/
/*------------------------------------------------------*/

enum dcmd {
/*-------------------------------------------	HOST to AFV-DSP	  ------*/
	SOFT_RESET	= 1,		/* software reset		*/
	INIT_BOOT0,			/* intial parameter boot	*/
	CHG_BOOT0,			/* change parameter boot	*/
	SPAN_ADJ,			/* span adjustment		*/
	AFV_ADJ,			/* AFV adjustment		*/
	ZERO_AD_ADJ,			/* zero A/D tracking		*/
	WEI_DAT_REQ,			/* weigh head data request	*/
	DF_DAT_REQ,			/* DF data convert		*/
	DSP_MEM_READ,			/* memory read			*/
	DSP_MEM_WRITE,			/* memory write			*/
	DSP_STA_MON,			/* monitor start		*/
	DSP_STP_MON,			/* monitor stop			*/
	DIAG_DAT_REQ,			/* diagnostic data request	*/
	HDCMD_MAX,

/*-------------------------------------------	AFV-DSP to HOST	  ------*/
	EOF_INIT_BOOT0	= 513,		/* End of initial parameter boot*/
	EOF_CHG_BOOT0,			/* End of change parameter boot	*/
	EOF_SPAN,			/* End of span adjustment	*/
	EOF_AFV,			/* End of AFV adjustment	*/
	AFV_ERR,			/* AFV error			*/
	DHCMD_MAX,

/*-------------------------------------------    HOST to ADC-DSP  ------*/
	INIT_BOOT1	= 257,		/* initial parameter boot	*/
	CHG_BOOT1,			/* change parameter boot	*/
	MANU_ZERO,			/* manual zero tracking		*/
	MEM_READ,			/* memory read			*/
	MEM_WRITE,			/* memory write			*/
	STA_MON,			/* monitor start		*/
	STP_MON,			/* monitor stop			*/
	HACMD_MAX,

/*-------------------------------------------    ADC-DSP to HOST  ------*/
	SELF_CHK	= 769,		/* self check result		*/
	EOF_INIT_BOOT1,			/* End of initial parameter boot*/
	EOF_CHG_BOOT1,			/* End of change parameter boot	*/
	EOF_ZERO,			/* zero tracking complete	*/
	EOF_MEM_READ,			/* memory read complete		*/
	EOF_MEM_WRITE,			/* memory write complete	*/
	EOF_STA_MON,			/* monitor start ehco		*/
	EOF_STP_MON,			/* monitor stop	echo		*/
	AHCMD_MAX,

/*----------------------------------------------------------------------*/
};
typedef enum dcmd	DCMD;

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#define	INIT_BOOT0_COMPL	0x1	/* AFV init. boot complete flag	*/
#define	INIT_BOOT1_COMPL	0x2	/* ADC	"			*/
#define	CHG_BOOT0_COMPL		0x4	/* AFV change	"		*/
#define	CHG_BOOT1_COMPL		0x8	/* ADC 	"			*/

#ifdef	STORAGE
unsigned int	dadi_tsk_id[DAD_MAX] = {
			DADI_TSK1_ID, DADI_TSK2_ID
		};
unsigned int	dadi_tsk_pri[DAD_MAX] = {
			DADI_TSK1_PRI, DADI_TSK2_PRI
		};
union aap	dadi_tsk[DAD_MAX];	/* INTRﾀｽｸ･ｱｸｾｽ･ｱﾄﾞﾚｽ		*/
long		dad_poll_time = 5;	/* ﾎﾟｰﾘﾝｸﾞ指定時間(ms)		*/
long		dad_tmout = 45000;	/* 零点ﾀｲﾑ･ｱｳﾄ指定時間(ms)	*/
union aap	dad_ds = { 0L };	/* adcﾄﾞﾗｲﾊﾞｰのseg情報		*/

#else
extern unsigned int	dadi_tsk_id[];
extern unsigned int	dadi_tsk_pri[];
extern union aap	dadi_tsk[];
extern long		dad_poll_time;
extern long		dad_tmout;
extern union aap	dad_ds;
#endif

#endif	/* DADCTL_H */
