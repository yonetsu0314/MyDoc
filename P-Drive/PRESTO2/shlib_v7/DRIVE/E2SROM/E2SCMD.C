/*----------------------------------------------------------------------*
 * File name	: e2scmd.c		/ e2s_rom_cmd_write						*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2scmd.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2scmd.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2scmd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		Serial EE-PROMにコマンドの書き込みを行う。この関数ではByte		*/
/*		でのアクセスは考慮していない(Wordのみ)。						*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		cmd			: command											*/
/*		adr			: Word Address(必要とする場合のみ)					*/
/*		*dat		: Pointer(ﾊﾟﾗﾒｰﾀを必要とする場合のみ)				*/
/*						WRITE … 書き込みﾃﾞｰﾀへのﾎﾟｲﾝﾀ					*/
/*						READ  … 読み出したﾃﾞｰﾀの格納先へのﾎﾟｲﾝﾀ		*/
/* 戻り値																*/
/*		int			: 0		… 正常終了									*/
/*					  -1	… 異常終了									*/
/*----------------------------------------------------------------------*/

#include	"limits.h"							/* ANSI std				*/
#include	"stdarg.h"							/*	"					*/
#include	"e2sctxt.h"							/* Show current			*/
#define		CMD_MASK	(READ & 0xe000)			/* Addr情報抜きのｺﾏﾝﾄﾞ	*/

#include	"kernel.h"

void		wait0(int);


int
e2s_rom_cmd_write(
	WORD	cmd, ...
	)
{
	va_list				ap;						/* 可変個引数集			*/
	unsigned short		inst[2];				/* command[0],(+dat[1])	*/
	unsigned short		adr, *dats, bp, i;
	unsigned char		*datc, bpc;
	int					k, j, cnt[2], yn;

	va_start(ap ,cmd);
	switch(cmd) {
		case READ:
		case WRITE:
		case ERASE:
				adr = (WORD)va_arg(ap, int);
				if(adr > (0x0ffff >>(e2sr_io.shf_cnt+3-BYTEACCESS)))	return(-1);
				cmd |= (adr << (e2sr_io.shf_cnt - BYTEACCESS) );
				break;
	}
	cnt[1] = 0;  cnt[0] = 17 - e2sr_io.shf_cnt + BYTEACCESS;
	if((inst[0] = cmd) & DATA) {
		if(BYTEACCESS!=0){
			cnt[1] = sizeof(bpc)*CHAR_BIT;
			datc = va_arg(ap, unsigned char *);
			bp = (unsigned short)(*datc);
			inst[1] = (WORD)(bp << CHAR_BIT);
		}
		else{
			cnt[1] = sizeof(bp)*CHAR_BIT;
			dats = va_arg(ap, unsigned short *);
			inst[1] = (*dats);
		}
	}
	inst[0] >>= 1;
	(*e2sr_io.fnc.out_handle_di)(0,0,0);				/* set DI = 0		*/
	(*e2sr_io.fnc.out_handle_sk)(0,0,0);				/* SK = Inactive	*/
	(*e2sr_io.fnc.out_handle_ce)(0,0,1);				/* CE = Active		*/
	for(k= 0; k < sizeof(cnt)/sizeof(cnt[0]); k++) {
		for(i= inst[k], j=cnt[k], bp= (WORD)0x8000; j; j--, bp>>= 1) {
			if(i & bp) 	yn =1;
			else		yn =0;
			(*e2sr_io.fnc.out_handle_di)(0,0,yn);		/* set DI			*/
			wait0(5);
			(*e2sr_io.fnc.out_handle_sk)(0,0,1);		/* SK = Active		*/
			wait0(5);
			(*e2sr_io.fnc.out_handle_sk)(0,0,0);		/* SK = Inactive	*/
		}
	}
	(*e2sr_io.fnc.out_handle_di)(0,0,0);		/* set DI			*/
	if((cmd & CMD_MASK) == READ) {
		j = 16 >> BYTEACCESS;
		bp = (WORD)0;
		do {
			wait0(5);
			(*e2sr_io.fnc.out_handle_sk)(0,0,1);		/* SK = Active		*/
			wait0(5);
			bp <<= 1;
			bp |= (*e2sr_io.fnc.in_handle_do)(0,0);	/* Read DO			*/
			(*e2sr_io.fnc.out_handle_sk)(0,0,0);		/* SK = Inactive	*/
		} while(--j);
		if(BYTEACCESS!=0){
			datc = va_arg(ap, unsigned char *);
			*datc = (unsigned char)bp;
		}
		else{
			dats = va_arg(ap, unsigned short *);
			*dats = bp;
		}
	}
	va_end(ap);
	(*e2sr_io.fnc.out_handle_ce)(0,0,0);		/* CE = Inactive	*/
	wait0(5);
	(*e2sr_io.fnc.out_handle_sk)(0,0,1);		/* SK = Active		*/
	wait0(5);
	(*e2sr_io.fnc.out_handle_sk)(0,0,0);		/* SK = Inactive	*/
	return(0);
}

/*----------------------------------------------------------------------*/
/*		e2s_rom_chk_ready												*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		書き込み後のデバイスのReady/Busyのチェックを行う。				*/
/*		tPR(2ms/93C46)を時間管理すれば、本関数は必要でない				*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		void	: 無し													*/
/*																		*/
/* 戻り値																*/
/*		int		: 0	…	成功											*/
/*----------------------------------------------------------------------*/

int
e2s_rom_chk_ready(void)
{
	int		i;

	(*e2sr_io.fnc.out_handle_ce)(0,0,1);				/* CE = Active		*/
	wait0(5);

	for(i=0;i<10;++i){
		if((*e2sr_io.fnc.in_handle_do)(0,0) != 0) break;
		(*e2sr_io.fnc.out_handle_sk)(0,0,1);		/* SK = Active		*/
		wait0(5);
		(*e2sr_io.fnc.out_handle_sk)(0,0,0);		/* SK = Inactive	*/
		wait0(5);
	}		/* Read DO			*/

	(*e2sr_io.fnc.out_handle_ce)(0,0,0);				/* CE = Inactive	*/
	wait0(5);
	return(0);
}
