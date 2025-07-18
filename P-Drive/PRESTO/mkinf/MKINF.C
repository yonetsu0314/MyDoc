/*
 *$Header: p:/presto/mkinf/rcs/mkinf.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: mkinf.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * rename削除、version番号計算をオプション化
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＡＣＳ＿Ｚ用をＣＣＷＲ用に書き換え
 * 
 */

#include <stdio.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/


#define     FLASH_WRITE_BLOCK (0x2000)		/* for all */


long		entryadrs;
long		progsize;
unsigned long	sumttl,sumtmp,sumcnt;
unsigned short	suminf,sumsub;

char	mcuname[5][16];
char	mcuver[5][8];
char	mcusum[5][8];

int		optionz0,optionz1;

int		mkmotsum(int mcu, char *filename, char *verstr);
int		mkinfsum(void);
void	addsuff(char *filename, char *verstr);
int		transMOT(char *buf, long *maxpp, long *minpp);
int		transINF(char *buf);
void	setmcuinf(int n, char *buf);
void	setinfend(int n, char *buf);
int		htoi(char a);
char	*struprx(char *buf);
void	put02hexaa(char *aa,int sum);
void	*Bin_Asc16( long value, unsigned char *asc, size_t keta );
void	rsstr(char *str);
void	put8hex(long i);
void	put4hex(long i);
long	calprogsize(long size, long block);


/*******************************************************
 *　オプション：大文字、小文字可能
 *　　−ｚ０　：　バージョンに応じてMOT,EXOファイルをRENAMEする
 *　　　　　　　（デフォルト：RENAMEしない）
 *　　−ｚ１　：　バージョン記号I,O,Qを抜くとき番号を詰めて計算する
 *　　　　　　　（デフォルト：番号を詰めない）
 *******************************************************/
int
main(int argc,char **argv){
	FILE	*fpr,*fpw;
	static char	buf[2][120],str1[120],str2[120];
	int		c,i,j,keynum,itemnum;

	optionz0 = 0;
	optionz1 = 0;

	/* オプション設定のチェック */
	for(i=1;i<argc;++i){
		strcpy(str1,argv[i]);
		strupr(str1);
		if(strcmp("-Z0",str1)==0) optionz0 = 1;
		if(strcmp("-Z1",str1)==0) optionz1 = 1;
	}

	if((fpr = fopen("INFINFO.TXT", "rb")) == NULL){
		rsstr("file open error.");
		return(-1);
	}
	if((fpw = fopen("PROGRAM.INF", "w")) == NULL){
		rsstr("file open error.");
		return(-1);
	}
	keynum = 0;
	itemnum = 0;
	buf[0][0] = buf[1][0] = (char)0;
	for(;;){
		for(j=0;;){
			c  = fgetc(fpr);
			if(c==EOF) break;
			if(c==(int)0x0a) continue;
			if((c==(int)',')||(c==(int)0x09)||(c==(int)0x20)){
				if(itemnum==1) continue;
				buf[itemnum][j++] = (char)0;
				j = 0;
				itemnum=1;
				continue;
			}
			if(c==(int)0x0d){
				buf[itemnum][j++] = (char)0;
				break;
			}
			buf[itemnum][j++] = (char)c;
			continue;
		}
		if(c==EOF){
			rsstr("EOF");
			break;
		}

		struprx(buf[0]);
		struprx(buf[1]);

		if(keynum==0){
			addsuff(buf[0],buf[1]);
			printf("PACK %s",buf[0]);
			fprintf(fpw,"PACK,CCWR,I,%s,%s,\n",buf[0],buf[1]);
		}
		else{
			if(mkmotsum(keynum,buf[0],buf[1]) < 0){
				fclose(fpr);
				fflush(fpw);
				fclose(fpw);
				rsstr("\nmkmotsum error.\n");
				return(-1);
			}
			Bin_Asc16( (long)sumttl,  (unsigned char *)(&str1[0]), (size_t)4 );	str1[4] = 0;
			Bin_Asc16( (long)progsize,(unsigned char *)(&str2[0]), (size_t)6 );	str2[6] = 0;
			switch(keynum){
				case 1:
					fprintf(fpw,"CPU%1d,FPGA,E,%s,%s,%s,%s\n",keynum,buf[0],buf[1],str1,str2);
					break;
				case 2:
					fprintf(fpw,"CPU%1d,WCU,S,%s,%s,%s,%s\n",keynum,buf[0],buf[1],str1,str2);
					break;
				default:
					fprintf(fpw,"CPU%1d,DMU,S,%s,%s,%s,%s\n",keynum,buf[0],buf[1],str1,str2);
					break;
			}
		}
		++keynum;
		itemnum = 0;
		if(keynum > 3) break;
	}
	fclose(fpr);
	fflush(fpw);
	fclose(fpw);

	if(mkinfsum() < 0){
		rsstr("\nmkinfsum error.\n");
		return(-1);
	}
	if((fpw = fopen("PROGRAM.INF", "a")) == NULL){
		rsstr("file open error.");
		return(-1);
	}
	Bin_Asc16( (long)suminf,   (unsigned char *)(&str1[0]), (size_t)4 );	str1[4] = 0;
	fprintf(fpw,"INFEND,%s\n",str1);
	fflush(fpw);
	fclose(fpw);

	rsstr("\nOK");
	return(0);
}


int
mkmotsum(int mcu, char *filename, char *verstr){
	FILE	*fp;
	char	buf[120],tmp2[20],oname[120],fname[120];
	int		c,i,j,k;
	long	maxp,minp;

	sumcnt = (unsigned long)0;
	sumtmp = (unsigned long)0;
	minp = 0x7fffffffL;
	maxp = 0x00000000L;

	strcpy(fname,filename);
	strcpy(oname,filename);
	switch(mcu){
		case 1:
			strcat(oname,".exo");
			break;
		default:
			strcat(oname,".mot");
			break;
	}
	strcpy(buf,oname);
	fp=fopen(buf,"rb");
	if(fp==NULL){
		printf("\nCan not open %s.\n",buf);
		return(-1);
	}
	switch(mcu){
		case 1:
			printf("\nFPGA %s.EXO",fname);
			break;
		case 2:
			printf("\nWCU  %s.MOT",fname);
			break;
		default:
			printf("\nDMU  %s.MOT",fname);
			break;
	}
	for(;;){
		for(j=0;;){
			c  = fgetc(fp);
			if(c==EOF){
				rsstr("EOF");
				break;
			}
			if(c==(int)0x0a) continue;
			if(c==(int)0x09) continue;
			if(c==(int)0x20) continue;
			if(c==(int)0x0d){
				buf[j++] = (char)0;
				break;
			}
			buf[j++] = (char)c;
			continue;
		}

		i = transMOT(buf, &maxp, &minp);

		if(i==1){
			rsstr("\nIs Data strange,isn't it?\n");
			fclose(fp);
			return(-1);
		}
		if(i==2){
			rsstr("\nCheck Sum Error !!\n");
			fclose(fp);
			return(-1);
		}
		if(i < 0) break;
	}
	fclose(fp);

	rsstr("\nbegin:"); put8hex(minp);
	rsstr("\n  end:"); put8hex(maxp);
	entryadrs = minp;
	progsize = maxp - minp;
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);
	rsstr("\n size:"); put8hex(progsize);
	sumttl  = (unsigned long)0x0ff * ((unsigned long)progsize - sumcnt);
	sumttl += sumtmp;
	rsstr("\n  sum:"); put4hex(sumttl);

	for(i=0,k=0;i<20;++i){
		c = verstr[i];
		if(c=='.') break;
		if(c==(char)0) break;
		k *= 10;
		k += c & 0x0f;
	}

	if(k<=1) return(0);

	tmp2[0] = (char)(0x40 + k - 1);
	tmp2[1] = (char)0;
	if(optionz1!=0){
		if((k>=2)&&(k<=9)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k);
		if((k>=10)&&(k<=14)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k + 1);
		if((k>=15)&&(k<=15)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k + 2);
		if((k>=16)&&(k<=24)) strcat(fname,tmp2);
		tmp2[0] = (char)'_';
		if(k>=25) strcat(fname,tmp2);
	}
	else{
		strcat(fname,tmp2);
	}
	strcpy(filename,fname);

	if(optionz0==0) return(0);
	switch(mcu){
		case 1:
			strcat(fname,".exo");
			break;
		default:
			strcat(fname,".mot");
			break;
	}
	if(rename(oname,fname) !=0){
		printf("\nrename error\n");
		return(-1);
	}
	return(0);
}


void
addsuff(char *filename, char *verstr){
	char	tmp2[20],fname[120];
	int		c,i,k;

	strcpy(fname,filename);

	for(i=0,k=0;i<20;++i){
		c = verstr[i];
		if(c=='.') break;
		if(c==(char)0) break;
		k *= 10;
		k += c & 0x0f;
	}

	if(k<=1) return;

	tmp2[0] = (char)(0x40 + k - 1);
	tmp2[1] = (char)0;
	if(optionz1!=0){
		if((k>=2)&&(k<=9)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k);
		if((k>=10)&&(k<=14)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k + 1);
		if((k>=15)&&(k<=15)) strcat(fname,tmp2);
		tmp2[0] = (char)(0x40 + k + 2);
		if((k>=16)&&(k<=24)) strcat(fname,tmp2);
		tmp2[0] = (char)'_';
		if(k>=25) strcat(fname,tmp2);
	}
	else{
		strcat(fname,tmp2);
	}
	strcpy(filename,fname);
}


int
mkinfsum(){
	FILE	*fp;
	static char	buf[120];
	int		c,i,j,keynum;

	if((fp = fopen("PROGRAM.INF", "rb")) == NULL){
		rsstr("file open error.");
		return(-1);
	}
	suminf = (unsigned short)0;
	keynum = 0;
	for(;;){
		sumsub = (unsigned short)0;
		for(j=0;;){
			c  = fgetc(fp);
			if(c==EOF) break;
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
		if(c==EOF) break;

		i = transINF(buf);

		if(i < 0) break;
		suminf += sumsub;
		++keynum;
	}
	fclose(fp);
	if(keynum != 4) rsstr("item less than 4");
	Bin_Asc16( (long)suminf, (unsigned char *)(&buf[0]), (size_t)4 );
	buf[4] = 0;
	rsstr("\ninfsum = ");
	rsstr(buf);
	return(0);
}


int
transMOT(char *buf, long *maxpp, long *minpp){
	char	aa[20];
	long	k,p;
	int		i,j,a,sum;

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
		p = (long)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p < *minpp) *minpp = p;
		for(j=12;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			sumtmp += (unsigned long)a;		++sumcnt;
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
		p = (long)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p< *minpp)  *minpp = p;
		for(j=10;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			sumtmp += (unsigned long)a;		++sumcnt;
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
		p = (long)k;
		if((buf[0]!='S')||(i>32)) return 1;
		if(p< *minpp)  *minpp = p;
		for(j=8;i>0;--i){
			a = htoi(buf[j++])*16+htoi(buf[j++]);
			sumtmp += (unsigned long)a;		++sumcnt;
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
transINF(char *buf){
	static char	tmp1[80];

	strcpy(tmp1,buf);
	struprx(tmp1);
	if(strstr(buf,"PACK")!=((char *)NULL)){
		setmcuinf(0,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU1")!=((char *)NULL)){
		setmcuinf(1,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU2")!=((char *)NULL)){
		setmcuinf(2,tmp1);
		return 0;
	}
	if(strstr(buf,"CPU3")!=((char *)NULL)){
		setmcuinf(3,tmp1);
		return 0;
	}
	if(strstr(buf,"INFEND")!=((char *)NULL)){
		setinfend(4,tmp1);
		return -1;
	}
	return 0;
}


void
setmcuinf(int n, char *buf){
	int		i,j,k;
	char	a;

	mcuname[n][0] = 0;
	mcuver[n][0] = 0;
	mcusum[n][0] = 0;
	for(i=0,j=0,k=0;;++i){
		a = buf[i];
		if(a==','){
			switch(j){
				case 3:
					mcuname[n][k++] = 0;
					break;
				case 4:
					mcuver[n][k++] = 0;
					break;
				case 5:
					mcusum[n][k++] = 0;
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
				mcuname[n][k++] = a;
				break;
			case 4:
				mcuver[n][k++] = a;
				break;
			case 5:
				mcusum[n][k++] = a;
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

	mcuname[n][0] = (char)0;
	mcuver[n][0] = (char)0;
	mcusum[n][0] = (char)0;
	for(i=0,k=0;;++i){
		a = buf[i];
		if(a==','){
			mcusum[n][k++] = (char)0;
			k = 0;
			continue;
		}
		mcusum[n][k++] = a;
		if(a==0) break;
	}
}


int htoi(char a){
	char	b;

	b = (char)(a & 0x0f0);
	a &= 0x0f;
	if(b == 0x30) return((int)a);
	if(b == 0x40) return((int)(a+9));
	return(-1);
}


char *
struprx(char *buf){
	char	c;
	int		i;

	for(i=0;;++i){
		c = buf[i];
		if(c==(char)0) break;
		buf[i] = toupper(c);
	}
	return(buf);
}


void
put02hexaa(char *aa,int sum){
	int		k,ad;
	char	c;

	for (k=1;k>=0;--k){
		ad = sum;
		ad >>= 4*k;
		ad &= 0x0f;
		if(ad >= 10) c=(char)('A'+ad-10);
		else c = (char)('0' + ad);
		*aa++ = (char)c;
	}
	*aa++ = (char)0;
}


void *Bin_Asc16( long value, unsigned char *asc, size_t keta ){
	unsigned char	i, hi, lo;
	unsigned char *	p = asc + keta;

	for( i=(unsigned char)0; i<8; i++, value >>= 8 ) {
		if( keta == 0 ) break;
		keta--;
		p--;
		lo = (unsigned char)((unsigned char)value & 15);
		*p = (unsigned char)( lo >= 10 ? ('A' + lo - 10) : ('0' + lo) );
		if( keta == 0 ) break;
		keta--;
		p--;
		hi = (unsigned char)((unsigned char)value >> 4);
		*p = (unsigned char)( hi >= 10 ? ('A' + hi - 10) : ('0' + hi) );
	}
	return asc;
}


void
rsstr(char *str){
	printf("%s",str);
}


void
put8hex(long i){
	char	buf[16];

	Bin_Asc16( (long)i, (unsigned char *)(&buf[0]), (size_t)8 );
	buf[8] = 0;
	rsstr(buf);
}


void
put4hex(long i){
	char	buf[16];

	Bin_Asc16( (long)i, (unsigned char *)(&buf[0]), (size_t)4 );
	buf[4] = 0;
	rsstr(buf);
}


long
calprogsize(long size, long block){
	if((size % block)!=0) size = (size/block + 1) * block;
	return(size);
}


