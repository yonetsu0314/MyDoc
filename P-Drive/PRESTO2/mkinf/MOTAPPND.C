/*----------------------------------------------------------------------*
 * File name	: motappnd.c						*
 *----------------------------------------------------------------------*
 *
 *$Header: p:/presto/mkinf/rcs/motappnd.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: motappnd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * hexappndのmot版
 * 
 *
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2004, ISHIDA CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY			*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
#include <stdio.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/

#define MAX_BUFF	120		/* 1ラインのバッファサイズ	*/

/*───────────────────────────────────*/
/* 機能説明								*/
/*	二つのMotorola S Format MOTファイルを結合し別のMOTファイルを生成する。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc	: 引き数の数(Min:1)					*/
/*	*argv[]	: ポインターの配列					*/
/*			argv[1] ･･･ 入力するMot File名1			*/
/*		  	argv[2] ･･･ 入力するMot File名2			*/
/*		  	argv[3] ･･･ 出力するMot File名			*/
/*									*/
/* 戻り値								*/
/*	int	: エラーコード						*/
/*───────────────────────────────────*/
int
main(int argc,char **argv){
	FILE	*fp,*fp2;
	char	buf[MAX_BUFF];

	if(argc != 4) {
		fprintf(stderr,"ERROR:Number of Parameter is not 3.\n");
		return(-2);
	}

	fp=fopen(argv[1],"rb");
	if(fp==NULL){
		fprintf(stderr,"Can not open %s.\n",argv[1]);
		return(-1);
	}
	fp2=fopen("TMP","wb");
	if(fp2==NULL){
		fprintf(stderr,"Can not open TMP.\n");
		fclose(fp);
		return(-1);
	}
	for(;;){
		if(fgets(buf, (short)MAX_BUFF, fp)==NULL) break;
		if(buf[1]=='7') break;
		if(buf[1]=='8') break;
		if(buf[1]=='9') break;
		if(fputs(buf,fp2)==EOF) goto err;
	}
	fclose(fp);

	fp=fopen(argv[2],"rb");
	if(fp==NULL){
		fprintf(stderr,"Can not open %s.\n",argv[2]);
		fflush(fp2);
		fclose(fp2);
		return(-1);
	}
	for(;;){
		if(fgets(buf, (short)MAX_BUFF, fp)==NULL) break;
		if(buf[1]=='0') continue;
		if(fputs(buf,fp2)==EOF) goto err;
		if(buf[1]=='7') break;
		if(buf[1]=='8') break;
		if(buf[1]=='9') break;
	}
	fclose(fp);
	fflush(fp2);
	fclose(fp2);

	remove(argv[3]);
	if(rename("TMP",argv[3]) !=0){
		fprintf(stderr,"rename error TMP->%s.\n",argv[3]);
		return(-1);
	}
	return(0);

err:
	fclose(fp);
	fflush(fp2);
	fclose(fp2);
	fprintf(stderr,"file write error TMP.\n");
	return(-1);
}


