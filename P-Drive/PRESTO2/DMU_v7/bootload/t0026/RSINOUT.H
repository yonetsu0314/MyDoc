/*======================================================================
 * File name    : rsinout.h
 * Original     : 
 *----------------------------------------------------------------------
 * Function     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/rsinout.h 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rsinout.h $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * プログラム整理を行う
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

#include <stddef.h>
/*****************************************************************
                    コンソールとの通信処理
*****************************************************************/
void *Bin_Asc16( long value, unsigned char *asc, size_t keta );
long Asc16_Bin2( unsigned char *asc, size_t len );

void	rsstrdt(char *str);

int		getmes(char *str, char *adr, int n);
int htoi(char a);
int	chkin(char);
int	chkout(void);
int	chkrep(void);
int	chkesc(void);
void	put02hexaa(char *aa,int sum);

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
