/*======================================================================
 * File name    : cmfhdl.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	通信デバイスファイルアクセスモジュール
 *	関数定義
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/cmfhdl.c 1.1.4102.1 1997/05/28 11:59:00 yonetsu Exp $
 *$Log: cmfhdl.c $
 * リビジョン 1.1.4102.1  1997/05/28  11:59:00  yonetsu
 * サムチェックの処理を外部の関数で行えるようにする。
 * sum_func()を外部で定義することが必要。
 * この関数は、他の送受信関数と共通のため、デバイス番号
 * 及び　送受信ステータスで区別し、処理すること。
 * 
 * リビジョン 1.1  1996/05/31  16:31:22  TGL
 * *** 空ログメッセージ ***
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<assert.h>			/* ANsi std		*/
#include	<fcntl.h>			/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show include	*/
#include	<rom\xio.h>			/* Show include	*/
#include	<drive\comdrva_.h>		/* Show include	*/

#include	"cmfhdl.h"			/* Show current	*/
#include	"fnctbl.h"

static unsigned char	*fname_tbl[] = {
		"con0",			/* STD I/O file name	*/
		"con1",			/* STD I/O file name	*/
		"lan0",			/* HST file name	*/
		"lan1",			/* HIC file name	*/
		"lan0",			/* RCU file name	*/
		"lan1",			/* ICU file name	*/
		"dpmem",		/* EXT file name	*/
		"tty0",			/* AUX0 file name	*/
		"tty1",			/* AUX1 file name	*/
		"tty2",			/* AUX2 file name	*/
		"tty3",			/* AUX3 file name	*/
		"tty4",			/* AUX4 file name	*/
		"tty5",			/* AUX5 file name	*/
};

static int	line_tbl[COM_FH_MAX] = {0};

#define	con_fh0	(line_tbl[CON_FH0])
#define	con_fh1	(line_tbl[CON_FH1])
#define	hst_fh	(line_tbl[HST_FH])
#define	hic_fh	(line_tbl[HIC_FH])
#define	rcu_fh	(line_tbl[RCU_FH])
#define	icu_fh	(line_tbl[ICU_FH])
#define	ext_fh	(line_tbl[EXT_FH])
#define	aux_fh0	(line_tbl[AUX_FH0])
#define	aux_fh1	(line_tbl[AUX_FH1])
#define	aux_fh2	(line_tbl[AUX_FH2])
#define	aux_fh3	(line_tbl[AUX_FH3])
#define	aux_fh4	(line_tbl[AUX_FH4])
#define	aux_fh5	(line_tbl[AUX_FH5])

#define	TIME_RTY	(2)

/*----------------------------------------------------------------------*/
/*	port_tbl_init							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	通信用ポートテーブルにファイル･ハンドルを設定する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	:  0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
port_tbl_init(void)
{
	int		i, *p;

	if(is_device_exist(CON_DN0)) {
		con_fh0 = open( fname_tbl[CON_DN0], O_RDWR);
		assert(con_fh0 != -1);
	}
	if(is_device_exist(CON_DN1)) {
		con_fh1 = open( fname_tbl[CON_DN1], O_RDWR);
		assert(con_fh1 != -1);
	}
	if(is_device_exist(RCU_DN)) {
		rcu_fh = open( fname_tbl[RCU_DN], O_RDWR);
		assert(rcu_fh != -1);
	}
/*	if(is_device_exist(HST_DN)) {
		hst_fh = open( fname_tbl[HST_DN], O_RDWR);
		assert(hst_fh != -1);
	}
	if(is_device_exist(ICU_DN)) {
		icu_fh = open( fname_tbl[ICU_DN], O_RDWR);
		assert(icu_fh != -1);
	}
*/	if(is_device_exist(HIC_DN)) {
		hic_fh = open( fname_tbl[HIC_DN], O_RDWR);
		assert(hic_fh != -1);
	}
/*
	ext_fh = open( fname_tbl[EXT_DN], O_RDWR);
	assert(ext_fh != -1);
*/

/*	if(is_device_exist(AUX0_DN)) {
		aux_fh0 = open( fname_tbl[AUX0_DN], O_RDWR);
		assert(aux_fh0 != -1);
	}
	if(is_device_exist(AUX1_DN)) {
		aux_fh1 = open( fname_tbl[AUX1_DN], O_RDWR);
		assert(aux_fh1 != -1);
	}
	if(is_device_exist(AUX2_DN)) {
		aux_fh2 = open( fname_tbl[AUX2_DN], O_RDWR);
		assert(aux_fh2 != -1);
	}
	if(is_device_exist(AUX3_DN)) {
		aux_fh3 = open( fname_tbl[AUX3_DN], O_RDWR);
		assert(aux_fh3 != -1);
	}
	if(is_device_exist(AUX4_DN)) {
		aux_fh4 = open( fname_tbl[AUX4_DN], O_RDWR);
		assert(aux_fh4 != -1);
	}
	if(is_device_exist(AUX5_DN)) {
		aux_fh5 = open( fname_tbl[AUX5_DN], O_RDWR);
		assert(aux_fh5 != -1);
	}
*/
	for(i= COM_FH_MAX, p= line_tbl; i ; i--, p++) {
		if(*p == -1)				return(-1);
	}
	return(0);
}

/*----------------------- Write Function Macro -------------------------*/
#define fh_write(func_x,fh_x) \
COMRV func_x( \
	unsigned char	*buf, \
	int		len \
	) \
{ \
	int		err; \
	long	retry_interval = TIME_RTY; \
\
	if(len <= 0)					return(COM_FAIL); \
	do { \
		err = write( fh_x, buf, len); \
		assert(err != -1); \
		if(err == COM_RETRY)	HLT_TSK( &retry_interval); \
	} while(err == COM_RETRY); \
	return(COM_COMPL); \
}

/*===== Write Function =====*/

fh_write(rcu_write,rcu_fh)
fh_write(icu_write,icu_fh)
fh_write(hic_write,hic_fh)
fh_write(hst_write,hst_fh)
fh_write(aux0_write,aux_fh0)
fh_write(aux1_write,aux_fh1)
fh_write(aux2_write,aux_fh2)
fh_write(aux3_write,aux_fh3)
fh_write(aux4_write,aux_fh4)
fh_write(aux5_write,aux_fh5)
fh_write(con0_write,con_fh0)
fh_write(con1_write,con_fh1)
/*----------------------------------------------------------------------*/
/*----------------------- Read  Function Macro -------------------------*/
#define	fh_read(func_x,fh_x) \
COMRV func_x( \
	unsigned char	*buf, \
	int		max \
	) \
{ \
	int	len; \
\
	if((len = read( fh_x, buf, max)) < 0) { \
		assert(len != -1);			return(COM_FAIL); \
	} \
	return(len); \
}

/*===== Read  Function =====*/

fh_read(rcu_read,rcu_fh)
fh_read(icu_read,icu_fh)
fh_read(hic_read,hic_fh)
fh_read(hst_read,hst_fh)
fh_read(aux0_read,aux_fh0)
fh_read(aux1_read,aux_fh1)
fh_read(aux2_read,aux_fh2)
fh_read(aux3_read,aux_fh3)
fh_read(aux4_read,aux_fh4)
fh_read(aux5_read,aux_fh5)
fh_read(con0_read,con_fh0)
fh_read(con1_read,con_fh1)
/*----------------------------------------------------------------------*/
/*----------------------- Puts  Function Macro -------------------------*/

#define fh_puts(func_x,fh_x,dev_no) \
COMRV func_x( \
	unsigned char	*buf, \
	int		len \
	) \
{ \
	int		err; \
	long		retry_interval = TIME_RTY; \
\
	err = sum_func(dev_no,&buf[sizeof(COMFMT)-1],	\
		       (int)(len-(sizeof(COMFMT)-1)),"tx");	\
	if(err < 0)					return(err); \
\
	do { \
		err = write( fh_x, buf, 0); \
		assert(err != -1); \
		if(err == COM_RETRY) 	HLT_TSK(&retry_interval); \
	} while(err == COM_RETRY); \
	if(err <= 0)					return(err); \
	return(COM_COMPL); \
}

/*===== Puts Function =====*/

fh_puts(rcu_puts,rcu_fh,RCU_DN)
fh_puts(icu_puts,icu_fh,ICU_DN)
fh_puts(hic_puts,hic_fh,HIC_DN)
fh_puts(hst_puts,hst_fh,HST_DN)
/*----------------------------------------------------------------------*/
/*----------------------- Gets  Function Macro -------------------------*/

#define fh_gets(func_x,fh_x,dev_no) \
COMRV func_x( \
	unsigned char	*buf, \
	int		max \
	) \
{ \
	int	len=max , err; \
\
	if((len = read( fh_x, buf, 0)) < 0) { \
		assert(len != -1); 	\
		return COM_FAIL; 	\
	} \
\
	err = sum_func(dev_no,&buf[sizeof(COMFMT)-1],   \
		       (int)(len-(sizeof(COMFMT)-1)),"rx"); 	\
	if(err < 0)				return err; 	  \
	else if(len - err < 0)			return RCV_ERROR; \
	else					return len - err; \
}

/*===== Gets Function =====*/

fh_gets(rcu_gets,rcu_fh,RCU_DN)
fh_gets(icu_gets,icu_fh,ICU_DN)
fh_gets(hic_gets,hic_fh,HIC_DN)
fh_gets(hst_gets,hst_fh,HST_DN)
/*----------------------------------------------------------------------*/

/*--------------------- Aux Puts  Function Macro -----------------------*/
/*----------------------------------------------------------------------*/
/*	aux#_puts							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	テキストを aux# にライトする。					*/
/*      ARCNET とは、チェックサムの方法が異なるので注意			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: pointer to text buff					*/
/*	len	: length of text					*/
/*									*/
/* 戻り値								*/
/*	COMRV	: COM_COMPL	･･･ complete				*/
/*	      	  COM_FAIL	･･･ error code				*/
/*----------------------------------------------------------------------*/

#define aux_fh_puts(func_x,fh_x,dev_no) \
COMRV func_x( \
	unsigned char	*buf, \
	int		len \
	) \
{ \
	int		err; \
	long		retry_interval = TIME_RTY; \
\
	err = sum_func(dev_no,buf,len,"tx");	  \
	if(err < 0)		return(COM_FAIL); \
\
	do { \
		err = write( fh_x, buf, len); \
		assert(err != -1); \
		if(err == COM_RETRY)	HLT_TSK(&retry_interval); \
	} while(err == COM_RETRY); \
	if(err <= 0)					return(err); \
	return(COM_COMPL); \
}

/*===== Puts Function =====*/

aux_fh_puts(aux0_puts,aux_fh0,AUX0_DN)
aux_fh_puts(aux1_puts,aux_fh1,AUX1_DN)
aux_fh_puts(aux2_puts,aux_fh2,AUX2_DN)
aux_fh_puts(aux3_puts,aux_fh3,AUX3_DN)
aux_fh_puts(aux4_puts,aux_fh4,AUX4_DN)
aux_fh_puts(aux5_puts,aux_fh5,AUX5_DN)
aux_fh_puts(con0_puts,con_fh0,CON_DN0)
aux_fh_puts(con1_puts,con_fh1,CON_DN1)
/*----------------------------------------------------------------------*/

/*--------------------- Aux Gets  Function Macro -----------------------*/
/*----------------------------------------------------------------------*/
/*	aux#_gets							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	aux# からリードする。						*/
/*      ARCNET とは、チェックサムの方法が異なるので注意			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buf	: pointer to text buff					*/
/*	max	: maximum sizeof buffer					*/
/*									*/
/* 戻り値								*/
/*	COMRV	: len		･･･ success				*/
/*		  RCV_ERROR	･･･ fail				*/
/*----------------------------------------------------------------------*/
#define aux_fh_gets(func_x,fh_x,dev_no) \
COMRV func_x( \
	unsigned char	*buf, \
	int		max \
	) \
{ \
	int 	len = max; \
	int	err;	   \
\
	if((len = read(fh_x, buf, 1)) < 0) { \
		assert(len != -1); \
		return(COM_FAIL); \
	} \
	err = sum_func(dev_no,buf,len,"rx");   \
	if(err < 0)				return err; \
	else if(len - err < 0)			return RCV_ERROR; \
	else					return len - err; \
}

/*===== Gets Function =====*/

aux_fh_gets(aux0_gets,aux_fh0,AUX0_DN)
aux_fh_gets(aux1_gets,aux_fh1,AUX1_DN)
aux_fh_gets(aux2_gets,aux_fh2,AUX2_DN)
aux_fh_gets(aux3_gets,aux_fh3,AUX3_DN)
aux_fh_gets(aux4_gets,aux_fh4,AUX4_DN)
aux_fh_gets(aux5_gets,aux_fh5,AUX5_DN)
aux_fh_gets(con0_gets,con_fh0,CON_DN0)
aux_fh_gets(con1_gets,con_fh1,CON_DN1)
