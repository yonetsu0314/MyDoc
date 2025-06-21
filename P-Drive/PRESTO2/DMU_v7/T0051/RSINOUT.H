/*======================================================================
 * File name    : rsinout.h
 * Original     : 
 *----------------------------------------------------------------------
 * Function     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/rsinout.h 2.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rsinout.h $
 * リビジョン 2.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/19 17:00
 * wait_answerを追加する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------
 *      (c) Copyright 2002, ISHIDA CO., LTD.
 *      959-1 SHIMOMAGARI RITTO-CITY
 *      SHIGA JAPAN
 *      (077) 553-4141
 *======================================================================
 */
#ifndef	_RSINOUT_H_
#define	_RSINOUT_H_

#define	WAIT1S	3000	/* 1mS loop counter for external ROM */
#define	WAIT01S	300		/* 0.1mS loop counter for external ROM */

#define INT_BUF_size  128*1024

#undef	GLOBAL	
#ifdef	STORAGE
#define	GLOBAL	
int		rsdispsw = 0;
int		rskeysw = 0;
#else
#define	GLOBAL	extern
extern int		rsdispsw;
extern int		rskeysw;
#endif

GLOBAL char		int_buf0[INT_BUF_size];
GLOBAL char		*int_buf0_in;
GLOBAL char		*int_buf0_out;
GLOBAL unsigned int	int_buf0_count;
GLOBAL unsigned int	int_buf0_cntout;
GLOBAL unsigned char	int_buf0_ovfl;

GLOBAL char		int_buf1[INT_BUF_size];
GLOBAL char		*int_buf1_in;
GLOBAL char		*int_buf1_out;
GLOBAL unsigned int	int_buf1_count;
GLOBAL unsigned int	int_buf1_cntout;
GLOBAL unsigned char	int_buf1_ovfl;


/*****************************************************************
                    コンソールとの通信処理
*****************************************************************/
void *Bin_Asc16( long value, unsigned char *asc, size_t keta );
long Asc16_Bin2( unsigned char *asc, size_t len );

void con0_buf_clear(void);
void con1_buf_clear(void);

void	rsstrdt(char *str);

int		getmes(char *str, char *adr, int n);
int htoi(char a);
int	chkin(char);
int	chkout(void);
int	chkrep(void);
int	chkesc(void);
void	put02hexaa(char *aa,int sum);

void	rstwdt(void);
void	wait(int n);				/* wait 1mS * n */
void	wait0(int n);				/* wait 0.1mS * n */
void	wait1(void);				/* wait 1mS */

void	rslf(void);
void	rsstr(char *str);
void	rsstrb(char *str);
void	rsout(char c);
void	rsstr232(char *str);
void	rsout232(char c);
void	rsstrdir(char *str);
void	rsoutdir(char c);
void	put4dir(int);
void	itoa1dir(int);

int		chkbuflen(void);
char	rschk(void);
char	rsin(void);
void	put2hex(int);
void	put4hex(int);
void	put6hex(int);
void	put8hex(int);
int		atoi1(char);
void	itoa1(int);
int	strcomp(char *,char *);

int	get2hex(void);
int	get4hex(void);
int	get6hex(void);
int	get8hex(void);

int wait_answer(void);

#endif
