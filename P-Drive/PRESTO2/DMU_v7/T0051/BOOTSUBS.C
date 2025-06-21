/*======================================================================
 * File name    : bootsubs.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/bootsubs.c 2.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: bootsubs.c $
 * リビジョン 2.4  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/18 14:00
 * upldinfにおいてFPGAのフォーマット形式をEに修正する。
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/18 12:30
 * インストール、アップロードするソフトはinfファイルによって判断
 * できるようにするため、recvINF、loadINFを変更。
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/10 16:30
 * filebkupにおいてエラーが生じた時は、エラーコードを返すようにする。
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:00
 * recvINFにおいてシリアルバッファをクリアする処理を
 * 関数化する
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/02/12 12:30
 * upldcfasc, upldmemasc 内のRCU>DISPコメントを
 * SENDで統一する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_reset をライブラリに移動する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/06 19:00
 * ワーニングをなくす
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 2002, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CITY
 *			SHIGA JAPAN
 *			(077) 553-4141
 *======================================================================
 */
#include <stdlib.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/
#include <dos_util.h>
#include <fsys.h>
#include <kernel.h>
#include <custom\typedefs.h>	/*	"		*/
#include <drive\atadrv.h>
#include <sh7615\bootpara.h>
#include <sh7615\7615.h>
#include "remmon.h"
#include "cmfhdl.h"
#include "download.h"
#include "bootcmds.h"
#include "rsinout.h"


void
mkmotstr(char buf[], char mem[], int ofs, int len, int mode){
	char	name[12],*adr,c;
	int		i,j,idx,sum;
	union {
		unsigned long  lstr[2];
		unsigned short hstr[4];
		unsigned char  bstr[8];
	} intbuf;

	switch(mode){
		case 0:			/* Header Record */
			for(i=strlen(mem)-1;i>0;--i){
				c = mem[i];
				if((c=='\\')||(c=='/')||(c==':')) break;
			}
			if(i>0) ++i;
			strcpy(name,"           ");
			for(j=0;j<11;){			/* ファイル名の切り出し */
				c = mem[i++];
				if(c=='.'){
					j = 8;
					continue;
				}
				if(c==(char)0) break;
				name[j++] = c;
			}
			strcpy(buf,"S00E0000");
			idx = 8;
			sum = 14;
			adr = name;
			for(i=0;i<11;++i){
				sum += (int)*adr;
				put02hexaa((char *)(&buf[idx]),(int)(*adr));
				idx += 2;
				++adr;
			}
			sum = ~sum;
			sum &= 0x00ff;
			put02hexaa((char *)(&buf[idx]),sum);
			idx += 2;
			break;
		case 1:			/* Data Record */
			intbuf.lstr[0] = ofs;
			if(ofs <= 0x0000ffff){
				strcpy(buf,"S1");
				adr = (char *)(&intbuf.bstr[2]);
				j = 2;
			}
			else if(ofs <= 0x00ffffff){
				strcpy(buf,"S2");
				adr = (char *)(&intbuf.bstr[1]);
				j = 3;
			}
			else{
				strcpy(buf,"S3");
				adr = (char *)(&intbuf.bstr[0]);
				j = 4;
			}
			sum = len+j+1;
			put02hexaa((char *)(&buf[2]),(int)sum);
			idx = 4;
			for(i=0;i<j;++i){
				sum += (int)*adr;
				put02hexaa((char *)(&buf[idx]),(int)(*adr));
				idx += 2;
				++adr;
			}
			adr = mem + ofs;
			for(i=0;i<len;++i){
				sum += (int)*adr;
				put02hexaa((char *)(&buf[idx]),(int)(*adr));
				idx += 2;
				++adr;
			}
			sum = ~sum;
			sum &= 0x00ff;
			put02hexaa((char *)(&buf[idx]),sum);
			idx += 2;
			break;
		default:		/* End Record */
			intbuf.lstr[0] = ofs;
			idx = 4;
			if(ofs <= 0x0000ffff){
				strcpy(buf,"S903");
				adr = (char *)(&intbuf.bstr[2]);
				sum = 3;
				j = 2;
			}
			else if(ofs <= 0x00ffffff){
				strcpy(buf,"S804");
				adr = (char *)(&intbuf.bstr[1]);
				sum = 4;
				j = 3;
			}
			else{
				strcpy(buf,"S705");
				adr = (char *)(&intbuf.bstr[0]);
				sum = 5;
				j = 4;
			}
			for(i=0;i<j;++i){
				sum += (int)*adr;
				put02hexaa((char *)(&buf[idx]),(int)(*adr));
				idx += 2;
				++adr;
			}
			sum = ~sum;
			sum &= 0x00ff;
			put02hexaa((char *)(&buf[idx]),sum);
			idx += 2;
			break;
	}
	buf[idx++] = (char)0x0d;
	buf[idx++] = (char)0x0a;	/* for ASCII mode */
	buf[idx]   = (char)0;
}


int
transMOT(char *buf, int ofs, char **maxpp, char **minpp){
	char	aa[20],*p;
	int		i,j,k,a,sum;

		if(buf[1]=='0') return 0;
		if(buf[1]=='7') return -1;
		if(buf[1]=='8') return -1;
		if(buf[1]=='9') return -1;
		if((buf[1]!='1')&&(buf[1]!='2')&&(buf[1]!='3')) return 1;
		if(buf[1]=='1') goto shrtadr;
		if(buf[1]=='2') goto mddladr;

		i=htoi(buf[2])*16+htoi(buf[3]);
		sum = i;
		i -= 5;
		j = htoi(buf[4])*16  + htoi(buf[5]);	k  = j;	sum += j;	k <<= 8;
		j = htoi(buf[6])*16  + htoi(buf[7]);	k |= j;	sum += j;	k <<= 8;
		j = htoi(buf[8])*16  + htoi(buf[9]);	k |= j;	sum += j;	k <<= 8;
		j = htoi(buf[10])*16 + htoi(buf[11]);	k |= j;	sum += j;
		p = (char *)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p < *minpp) *minpp = p;
		for(j=12;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			*(p + ofs) = (char)a;
			p++;
			sum += a;
		}
		if(p> *maxpp) *maxpp = p;
		sum = ~sum;
		sum &= 0x00ff;
		put02hexaa(aa,sum);
		if((buf[j++]!=aa[0])||(buf[j++]!=aa[1])) return 2;
		return 0;
mddladr:
		i=htoi(buf[2])*16+htoi(buf[3]);
		sum = i;
		i -= 4;
		j = htoi(buf[4])*16  + htoi(buf[5]);	k  = j;	sum += j;	k <<= 8;
		j = htoi(buf[6])*16  + htoi(buf[7]);	k |= j;	sum += j;	k <<= 8;
		j = htoi(buf[8])*16  + htoi(buf[9]);	k |= j;	sum += j;
		p = (char *)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p< *minpp)  *minpp = p;
		for(j=10;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			*(p + ofs) = (char)a;
			p++;
			sum += a;
		}
		if(p>*maxpp) *maxpp = p;
		sum = ~sum;
		sum &= 0x00ff;
		put02hexaa(aa,sum);
		if((buf[j++]!=aa[0])||(buf[j++]!=aa[1])) return 2;
		return 0;
shrtadr:
		i=htoi(buf[2])*16+htoi(buf[3]);
		sum = i;
		i -= 3;
		j = htoi(buf[4])*16 + htoi(buf[5]);	k  = j;	sum += j;	k <<= 8;
		j = htoi(buf[6])*16 + htoi(buf[7]);	k |= j;	sum += j;
		p = (char *)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p< *minpp)  *minpp = p;
		for(j=8;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			*(p + ofs) = (char)a;
			p++;
			sum += a;
		}
		if(p>*maxpp) *maxpp = p;
		sum = ~sum;
		sum &= 0x00ff;
		put02hexaa(aa,sum);
		if((buf[j++]!=aa[0])||(buf[j++]!=aa[1])) return 2;
		return 0;
}


int
uisdiv(int w,int v){
	return((int)((unsigned int)w / (unsigned short)v));
}


int		calprogsize(int size, int block){
	if((size % block)!=0) size = (size/block + 1) * block;
	return(size);
}


void
inxBAR(int dat){
	char	buf[8];

	barlen += dat;
	dat = barlen;
	if(dat>=100) strcpy(buf,"100");
	else if(dat>=10){
		buf[0] = (char)(0x30 + dat/10);
		buf[1] = (char)(0x30 + dat%10);
		buf[2] = (char)0;
	}
	else{
		buf[0] = (char)(0x30 + dat);
		buf[1] = (char)0;
	}
	rsstr("\nRCU>BAR "); rsstr(buf); rsstr("\n");
}


void
dispBAR(int dat){
	char	buf[8];

	if(dat>=100) strcpy(buf,"100");
	else if(dat>=10){
		buf[0] = (char)(0x30 + dat/10);
		buf[1] = (char)(0x30 + dat%10);
		buf[2] = (char)0;
	}
	else{
		buf[0] = (char)(0x30 + dat);
		buf[1] = (char)0;
	}
	rsstr("\nRCU>BAR "); rsstr(buf); rsstr("\n");
}


void
setBAR(int dat){
	barlen = dat;
	dispBAR(barlen);
}


void
incBAR(int dat){
	barlen += dat;
	dispBAR(barlen);
}


void
rscan(){
	int		c,i;

	rsstr("\nPlease Stop Tranfer...");
	for(i=0;;){
		if(rschk()){
			c  = rsin();
			i = 0;
			continue;
		}
		dly_tsk((RELTIM)1/MSEC);
		++i;
		if(i>3000) break;
	}
	rsstr("\nThank you.");
}


int
filebkup(char fname[],int n){
	ST_FILE	*fpr,*fpw;
	char	buf[120],c;
	int		i;

	rsstr("\nRCU>DISP 3:Backup "); rsstr(fname); rsstr(". (DMU-CF)\n");
	strcpy(buf,CFDIRNAME);	strcat(buf,fname);
	fpr = fopen( (char*)buf, "r" );
	if( fpr == NULL ){
		return (61);	/* File for Backup did not exist in CF */
	}
	strcpy(buf,DMUBKDIR);	strcat(buf,"\\");	strcat(buf,fname);
	fpw = fopen( (char*)buf, "w" );
	if( fpw == NULL ){
		return (62);	/* Can not Create File in CF */
	}

	for(i=0;;){
		if(fgets(buf, (short)120, fpr)==NULL) break;
		if(fputs(buf,fpw)==EOF) goto err;
		if(fputc(0x0d,fpw)==EOF) goto err;
		if(fputc(0x0a,fpw)==EOF) goto err;
		if(rschk()){
			c = rsin();
			if(c==(char)0x1b){
				fclose(fpr);
				fclose(fpw);
				return (63);	/* Backup Canceled */
			}
		}
		if(n!=0){
			++i;
			if(i>n){
				incBAR(1);
				i = 0;
			}
		}
	}
	fclose(fpr);
	fclose(fpw);
	return(0);
err:
	fclose(fpr);
	fclose(fpw);
	return (64);	/* CF Write Error */
}


int
filersm(char fname[]){
	ST_FILE	*fpr,*fpw;
	char	buf[120],c;
	int		i;

	rsstr("\nResume "); rsstr(fname); rsstr(" ...\n");
	strcpy(buf,DMUBKDIR);	strcat(buf,"\\");	strcat(buf,fname);
	fpr = fopen( (char*)buf, "r" );
	if( fpr == NULL ){
		rsstr("\nBackup File did not exist in CF\n");
		return(-1);
	}
	strcpy(buf,CFDIRNAME);	strcat(buf,fname);
	fpw = fopen( (char*)buf, "w" );
	if( fpw == NULL ){
		rsstr("\nCan not Create Resume File in CF\n");
		return(-1);
	}

	for(i=0;;++i){
		if(fgets(buf, (short)120, fpr)==NULL) break;
		if(fputs(buf,fpw)==EOF) goto err;
		if(fputc(0x0d,fpw)==EOF) goto err;
		if(fputc(0x0a,fpw)==EOF) goto err;
		if(rschk()){
			c = rsin();
			if(c==(char)0x1b){
				fclose(fpr);
				fclose(fpw);
				rsstr("\nBackup Canceled\n");
				return(-1);
			}
		}
		if((i%1000)==0){
			put2hex(i/1000);
			rsstr("\r");
		}
	}
	fclose(fpr);
	fclose(fpw);
	return(0);
err:
	fclose(fpr);
	fclose(fpw);
	rsstr("\nCF Write Error\n");
	return(-1);
}


int
rmbkall(){
	ST_DIRENTRY	*bp;
	char	buf[120],ext[4],ret,c;
	int		reti,i,j,a;
	unsigned char	uc;
	union llunion{
		unsigned long l;
		unsigned char  b[4];
	} ll;

	rsstr("\nRemove All Backup Boot Files ...\n");
	reti = chdir( (char*)DMUBKDIR );
	if( reti ) return(0);			/* /BOOTBAK not exist */

	for(i=0;i<16;++i) bkbuf[i][0] = (char)0;
	for(i=0;i<16;){
		bp = readdir( Cur_DirPtr );
		if( bp == NULL ) break;
		a = (int)bp->attr;
		if(a & 0x08){
			rsstr("\n#");
			rsstr((char *)bp->name);	rsout((char)'#');
			ll.l = (int)bp->size;
			uc = ll.b[0];	ll.b[0] = ll.b[3];	ll.b[3] = uc;
			uc = ll.b[1];	ll.b[1] = ll.b[2];	ll.b[2] = uc;
			put8hex((int)ll.l);
			continue;
		}
		else if(a & 0x10){
			rsstr("\n<");
			rsstr((char *)bp->name);
			rsout((char)'>');
			continue;
		}
		else{
			rsstr("\n ");
			rsstr((char *)bp->name);	rsout((char)' ');
			ll.l = (int)bp->size;
			uc = ll.b[0];	ll.b[0] = ll.b[3];	ll.b[3] = uc;
			uc = ll.b[1];	ll.b[1] = ll.b[2];	ll.b[2] = uc;
			put8hex((int)ll.l);
			strcpy(bkbuf[i],(char *)bp->name);
			++i;
			continue;
		}
	}
	for(i=0;i<16;++i){
		if(bkbuf[i][0]==(char)0) break;
		for(j=0;j<3;++j){
			c = bkbuf[i][j+8];
			if(bkbuf[i][j+8]==' ') break;
			ext[j] = c;
		}
		ext[j]=(char)0;
		for(j=0;j<8;++j){
			if(bkbuf[i][j]==' ') break;
		}
		bkbuf[i][j] = (char)'.';
		bkbuf[i][j+1] = (char)0;
		strcat(bkbuf[i],ext);
		rsstr("\nRemove "); rsstr(bkbuf[i]); rsstr(" ...");
		strcpy(buf,DMUBKDIR); strcat(buf,"\\"); strcat(buf,bkbuf[i]);
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr(" remove file error.");
	}
	reti = chdir( (char*)CFDIRNAME );
	if( reti ) rsstr("\nchdir error.");
	rmdir( (char*)DMUBKDIR );
	rsstr("\nRemove All Backup Boot Files ... Complete.\n");
	return(0);
}


int
upldinf(char fname[]){
	char	buf[120],tmp[8],c;
	unsigned short	sum;

	rsstr("\nPush CR to START !!!\n");
	rsstr("RCU>RECV ");	rsstr(fname+3);	rsstr("\n");
	c = rsin();
	if(c!=(char)0x0d){
		if(c!=(char)0x1b) rsstr("\nRCU>NG 51\n");	/* Receive Cancel with not ESC */
		return(0);
	}

	sum = (unsigned short)0;
	strcpy(buf,"PACK,DMU,I,");strcat(buf,"------");strcat(buf,",");strcat(buf,"--.--");strcat(buf,",");
	sum += calsubsum(buf);	rsstr(buf);	rslf();
	strcpy(buf,"CPU1,FPGA,E,");strcat(buf,prgname[1]);strcat(buf,",");strcat(buf,prgver[1]);
	strcat(buf,",");strcat(buf,prgsum[1]);strcat(buf,",");strcat(buf,prgsize[1]);
	sum += calsubsum(buf);	rsstr(buf);	rslf();
	strcpy(buf,"CPU2,WCU,S,");strcat(buf,prgname[2]);strcat(buf,",");strcat(buf,prgver[2]);
	strcat(buf,",");strcat(buf,prgsum[2]);strcat(buf,",");strcat(buf,prgsize[2]);
	sum += calsubsum(buf);	rsstr(buf);	rslf();
	strcpy(buf,"CPU2,DMU,S,");strcat(buf,prgname[3]);strcat(buf,",");strcat(buf,prgver[3]);
	strcat(buf,",");strcat(buf,prgsum[3]);strcat(buf,",");strcat(buf,prgsize[3]);
	sum += calsubsum(buf);	rsstr(buf);	rslf();
	Bin_Asc16( (long)sum, (unsigned char *)(&tmp[0]), (size_t)4 );
	tmp[4] = (char)0;
	strcpy(buf,"INFEND,");strcat(buf,tmp);
	rsstr(buf);	rslf();

	rsout((char)0x1a);		/* EOF */
	rsstr("\nRCU>OK\n");
	return(0);
}


/* ＩＮＦファイルは、ＣＦの１セクタ５１２ｂｙｔｅ以下の事 */
int
loadINF(char infname[]){
	ST_FILE	*fp;
	static char	buf[120];
	unsigned short	sum,sumsub;
	int		c,i,j/*,keynum*/;

	fp = fopen( infname, "r" );
	if( fp == NULL ) return(-1);

	sum = (unsigned short)0;
//	keynum = 0;
	memset(prgname, 0, sizeof(prgname));
	for(;;){
		sumsub = (unsigned short)0;
		for(j=0;;){
			c  = fgetc(fp);
			if(c==EOF){
				rsstr("EOF");
				goto err;
			}
			if(c==(int)0x0a) continue;
			if(c==(int)0x09) continue;
			if(c==(int)0x20) continue;
			if(c==(int)0x0d){
				buf[j++] = (char)0;
				break;
			}
			buf[j++] = (char)c;
			sumsub += (unsigned short)c;
			continue;
		}

		i = transINF(buf);

		if(i < 0) break;
		sum += sumsub;
//		++keynum;
	}
//	if(keynum != 4) goto err;
	Bin_Asc16( (long)sum, (unsigned char *)(&buf[0]), (size_t)4 );
	buf[4] = (char)0;
	rsstr("sum = ");
	rsstr(buf);
	if(strcmp(buf,prgsum[4]) != 0) goto err;
	rsstr("\nend");
	fclose(fp);
	memsize[1] = (int)Asc16_Bin2( (unsigned char *)prgsize[1], (size_t)6 );
	memsize[2] = (int)Asc16_Bin2( (unsigned char *)prgsize[2], (size_t)6 );
	memsize[3] = (int)Asc16_Bin2( (unsigned char *)prgsize[3], (size_t)6 );
//	rsstr("\nFPGA = ");put8hex(memsize[1]);
//	rsstr("\nWCU = ");put8hex(memsize[2]);
//	rsstr("\nDMU = ");put8hex(memsize[3]);
	return(0);
err:
	fclose(fp);
	return(-1);
}


unsigned short
calsubsum(char *buf){
	unsigned short	sumsub;
	int		i,c;

	sumsub = (unsigned short)0;
	for(i=0;;++i){
		c  = buf[i];
		if(c==(int)0x0a) continue;
		if(c==(int)0x09) continue;
		if(c==(int)0x20) continue;
		if(c==(int)0x0d) return(sumsub);
		if(c==(int)0x00) return(sumsub);
		sumsub += (unsigned short)c;
	}
}


int
recvINF(){
	ST_FILE	*fp;
	static char	buf[120];
	unsigned short	sum,sumsub;
	int		c,i,j/*,keynum*/;

	con0_buf_clear();

	for(i=0;;){
		if(rschk()) break;
		dly_tsk((RELTIM)1/MSEC);
		++i;
		if(i>60000) break;
	}
	if(i>60000){
		rsstr("\nTime out\n");
		return(-1);
	}

	if(tcpinput == 0){
		fp = fopen( CFINFNAME, "w" );
		if( fp == NULL ) return(-1);
	}

	rsstr("\nSTART\n");
	sum = (unsigned short)0;
//	keynum = 0;
	memset(prgname, 0, sizeof(prgname));
	for(;;){
		sumsub = (unsigned short)0;
		for(i=0,j=0;;){
			if(rschk()){
				c  = rsin();
				if(c==(int)0x0a) continue;
				if(c==(int)0x09) continue;
				if(c==(int)0x20) continue;
				if(c==(int)0x0d){
					buf[j++] = (char)0;
					break;
				}
				buf[j++] = (char)c;
				sumsub += (unsigned short)c;
				i = 0;
				continue;
			}
			dly_tsk((RELTIM)1/MSEC);
			++i;
			if(i>5000) break;
		}
		if(i>5000){
			rsstr("\nTime out\n");
			goto err;
		}

		if(tcpinput == 0){
			if(fputs(buf,fp)==EOF) goto err;
			if(fputc(0x0d,fp)==EOF) goto err;
			if(fputc(0x0a,fp)==EOF) goto err;
		}

		i = transINF(buf);

		if(i < 0) break;
		sum += sumsub;
//		++keynum;
	}
	if(tcpinput == 0) fclose(fp);
//	if(keynum != 4) return(-1);
	Bin_Asc16( (long)sum, (unsigned char *)(&buf[0]), (size_t)4 );
	buf[4] = (char)0;
	rsstr("\nsum = ");
	rsstr(buf);
	if(strcmp(buf,prgsum[4]) != 0) return(-1);
	rsstr("\nend");
	memsize[1] = (int)Asc16_Bin2( (unsigned char *)prgsize[1], (size_t)6 );
	memsize[2] = (int)Asc16_Bin2( (unsigned char *)prgsize[2], (size_t)6 );
	memsize[3] = (int)Asc16_Bin2( (unsigned char *)prgsize[3], (size_t)6 );
//	rsstr("\nFPGA = ");put8hex(memsize[1]);
//	rsstr("\nWCU = ");put8hex(memsize[2]);
//	rsstr("\nDMU = ");put8hex(memsize[3]);
	return(0);
err:
	if(tcpinput == 0) fclose(fp);
	return(-1);
}


int
transINF(char *buf){
	static char	tmp1[80];

	strcpy(tmp1,buf);
	strupr(tmp1);
	if(strstr(buf,"PACK")!=((char *)NULL)){
		setccwinf(0,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU1")!=((char *)NULL)){
		setccwinf(1,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU2")!=((char *)NULL)){
		setccwinf(2,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU3")!=((char *)NULL)){
		setccwinf(3,tmp1);
		return 0;
	}
	if(strstr(buf,"INFEND")!=((char *)NULL)){
		setinfend(4,tmp1);
		return -1;
	}
	return 0;
}


void
setccwinf(int n, char *buf){
	int		i,j,k;
	char	a;

	prgname[n][0] = (char)0;
	prgver[n][0] = (char)0;
	prgsum[n][0] = (char)0;
	prgsize[n][0] = (char)0;
	for(i=0,j=0,k=0;;++i){
		a = buf[i];
		if(a==','){
			switch(j){
				case 3:
					prgname[n][k++] = (char)0;
					break;
				case 4:
					prgver[n][k++] = (char)0;
					break;
				case 5:
					prgsum[n][k++] = (char)0;
					break;
				case 6:
					prgsize[n][k++] = (char)0;
					break;
				default:
					break;
			}
			++j;
			k = 0;
			continue;
		}
		switch(j){
			case 3:
				prgname[n][k++] = a;
				break;
			case 4:
				prgver[n][k++] = a;
				break;
			case 5:
				prgsum[n][k++] = a;
				break;
			case 6:
				prgsize[n][k++] = a;
				break;
			default:
				break;
		}
		if(a==0) break;
	}
}


void
setinfend(int n, char *buf){
	int		i,k;
	char	a;

	prgname[n][0] = (char)0;
	prgver[n][0] = (char)0;
	prgsum[n][0] = (char)0;
	prgsize[n][0] = (char)0;
	for(i=0,k=0;;++i){
		a = buf[i];
		if(a==','){
			prgsum[n][k++] = (char)0;
			k = 0;
			continue;
		}
		prgsum[n][k++] = a;
		if(a==0) break;
	}
}


char *
strupr(char *buf){
	char	c;
	int		i;

	for(i=0;;++i){
		c = buf[i];
		if(c==(char)0) break;
		buf[i] = (char)toupper(c);
	}
	return(buf);
}


void
pickversion(char *idptr){
	char	c;
	int		i,j;

	for(i=0;i<16;++i){
		c = idptr[i];
		tmpprogname[i] = c;						/* Load Boot Filename ignore */
		if(c==' '){
			tmpprogname[i] = (char)0;
			break;
		}
	}
	for(++i,j=0;j<8;++i,++j){
		c = idptr[i];
		tmpversion[j] = c;						/* Load Boot Version ignore */
		if(c==' '){
			tmpversion[j] = (char)0;
			break;
		}
	}
	for(++i,j=0;j<16;++i,++j){
		c = idptr[i];
		tmpprogname[j] = c;						/* Load Aplication Filename */
		if(c==' '){
			tmpprogname[j] = (char)0;
			break;
		}
	}
	for(++i,j=0;j<8;++i,++j){
		c = idptr[i];
		tmpversion[j] = c;						/* Load Aplication Version */
		if(c==' '){
			tmpversion[j] = (char)0;
			break;
		}
	}
	for(++i,j=0;j<8;++i,++j){
		c = idptr[i];
		tmpsumnum[j] = c;						/* Load Aplication Sum Number */
		if(c==' '){
			tmpsumnum[j] = (char)0;
			break;
		}
	}
	for(++i,j=0;j<8;++i,++j){
		c = idptr[i];
		tmpsizenum[j] = c;						/* Load Aplication Sum Number */
		if((c==(char)0)||(c==' ')){
			tmpsizenum[j] = (char)0;
			break;
		}
	}

	strupr(tmpprogname);
	strupr(tmpversion);
	strupr(tmpsumnum);
	strupr(tmpsizenum);
}


int
upldcfasc(char fname[],int barcnt){
	ST_FILE	*fp;
	char	buf[120],c;
	int		k;

	rsstr("\nRCU>DISP 102:SEND  Program ");	rsstr(fname+3);	rsstr(". (DMU-CF -> RCU)\n");
	rsstr("\nRCU>RECV ");	rsstr(fname+3);	rsstr("\n");
	c = rsin();
	if(c!=(char)0x0d){
		if(c!=(char)0x1b) rsstr("\nRCU>NG 51\n");	/* Receive Cancel with not ESC */
		return(0);
	}
	fp = fopen( (char*)fname, "r" );
	if( fp == NULL ){
		rsout((char)0x1a);		/* EOF */
		rsstr("\nRCU>NG 52\n");		/* Upload File did not exist in CF */
		return(-1);
	}

	for(k=0;;){
		if(fgets(buf, (short)120, fp)==NULL) break;
		rsstr(buf);
		rslf();
		if(rschk()){
			c = rsin();
			if(c==(char)0x1b){
				rsout((char)0x1a);		/* EOF */
				fclose(fp);
				rsstr("\nRCU>NG 53\n");		/* Upload Canceled */
				return(-1);
			}
		}
		if(barcnt!=0){
			++k;
			if(k > barcnt){
				inxBAR(5);
				k = 0;
			}
		}
	}
	rsout((char)0x1a);		/* EOF */
	fclose(fp);
	rsstr("\nRCU>OK\n");
	return(0);
}


int
upldmemasc(char *memadr, int sttofs, int size, char fname[],int barcnt){
	char	buf[120],c;
	int		i,j,k,ofs;

	rsstr("\nRCU>DISP 107:SEND  Program ");	rsstr(fname+3);	rsstr(" (DMU -> RCU)\n");
	rsstr("\nRCU>RECV ");	rsstr(fname+3);	rsstr("\n");
	c = rsin();
	if(c!=(char)0x0d){
		if(c!=(char)0x1b) rsstr("\nRCU>NG 51\n");	/* Receive Cancel with not ESC */
		return(0);
	}

	mkmotstr(buf,fname,0,0,0);
	rsstrb(buf);
	for(k=0,i=0;i<size;i+=16){
		if(rschk()){
			c = rsin();
			if(c==(char)0x1b){
				rsout((char)0x1a);		/* EOF */
				rsstr("\nRCU>NG 53\n");		/* Upload Canceled */
				return(-1);
			}
		}
		if(barcnt!=0){
			++k;
			if(k > barcnt){
				inxBAR(5);
				k = 0;
			}
		}
		ofs = sttofs + i;
		j = size - i;
		if(j>16) j = 16;
		mkmotstr(buf,memadr,ofs,j,1);
		rsstrb(buf);
	}
	mkmotstr(buf,(char *)NULL,sttofs,0,2);
	rsstrb(buf);
	rsout((char)0x1a);		/* EOF */
	rsstr("\nRCU>OK\n");
	return(0);
}


