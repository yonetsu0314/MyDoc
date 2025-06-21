/*----------------------------------------------------------------------*
 * File name	:	dmu5562.h											*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/dmu5562.h 1.10 1970/01/01 00:00:00 chimura Exp $
 *$Log: dmu5562.h $
 * リビジョン 1.10  1970/01/01  00:00:00  chimura
 * 2005/01/24 scuio,e2sromを追加する。
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  sakaguti
 * cfio,fromio追加
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  sakaguti
 * fpga_ioのﾌﾟﾛﾄﾀｲﾌﾟを追加
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/05/11 20:00
 * 起動用biosからアプリ用biosへの切り替え処理をソフトウェア割込みで
 * 行うようにしたことに伴う、宣言の追加。
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/02/19 19:20
 * INT_MASK_REG の定義を削除する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/02/05 10:00
 * WdtInit のプロトタイプ宣言を追加する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/14 20:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2003, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#include <machine.h>
//typedef     char            B;
//typedef     short           H;
//typedef     long            W;
//typedef     float           F;
//typedef     unsigned char   UB;
//typedef     unsigned short  UH;
//typedef     unsigned long   UW;
#include <kernel.h>
#include <sh7615\7615.h>
#include <sh7615\sh7615.h>


/******************************/
/* Interrupt Table definition */
/******************************/
#if	!defined(V_FUNC_T)
#define		V_FUNC_T
typedef void	(*V_FUNC)();
#endif
typedef struct	apl_info {
	void	*apl_start;
	void	*vect_no;
	V_FUNC	*vecter_adr;
};

/*************************/
/* STACK SIZE definition */
/*************************/
#define INIT_STACK	(void (*)(void))0x07000000	/* P-5562のSDRAMの終わり */
#define SPAdd		(0x07000000)		/* スタック アドレス	*/


#define	disable()		set_imask( 15 )
#define	enable()		set_imask( 0 )

/* アプリ開始アドレスの定義 */
#ifndef APL_SECT
#define APL_SECT      0x00200000    /* APL_SECT 未定義なら 0x00200000 とする */
#endif

/* 割込みテーブルの定義 */
#ifndef INTVEC
#define	INTVEC (0x06000000)    /* INTVEC 未定義なら 0x06000000 とする */
#endif
#ifndef BIOSVEC
#define	BIOSVEC (0x00200800)    /* BIOSVEC 未定義なら 0x00200800 とする */
#endif

#define RL1_P		(64-1)


/*******************/
/*     SDRAM       */
/*******************/
#define SD_BANK1 (*(volatile short*)0x06000000)
#define SD_BANK2 (*(volatile short*)0x06400000)
#define SD_BANK3 (*(volatile short*)0x06800000)
#define SD_BANK4 (*(volatile short*)0x06C00000)
#define SDMR	 (*(volatile short*)0xffff0888)

/***************************************/
/*     Software Wait Loop Parameter    */
/***************************************/
#define	WAIT1S	3000	/* 1mS loop counter for external ROM */
#define	WAIT01S	300		/* 0.1mS loop counter for external ROM */


extern const char	bios_id[];
extern const void (*const vector_table[])(void);
extern int *_D_ROM, *_D_BGN, *_D_END, *_B_BGN, *_B_END;

int *scif1_io_init(int,int,int,int);
void port_io(void);
void e2rom_io(void);
void icu_io(void);
void rtc_io(void);
void sram_io(void);
void tcu_io(void);
void from_io(void);
void com_io(void);
void bios_init(void);
void fpga_io(void);
void cf_io(void);
void e2srom_io(void);
void scu_io(void);


#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL	
#else
#define	GLOBAL	extern
#endif

void	InitPort( void );
void	init_FMR(void);
void	BscInit( void );
void	CacheInit( void );
void	WdtInit( void );

int		INITSCT(void);
void	INIT(void);
int		init_vector(void);
int		init_registers(void);
void	dmu_jump_to_reset(void);
void	halt(void);

void	rstwdt(void);
void	wait(int n);				/* wait 1mS * n */
void	wait0(int n);				/* wait 0.1mS * n */
void	wait1(void);				/* wait 1mS * n */

void	rsstr(char *str);
void	rsout(char c);

W    flash_write(H *adr,H data);
W    chip_erase(void);
W    sect_erase(UW sect);
void set_mode(UH cmd);
void set_fladr(UW flashadr);

