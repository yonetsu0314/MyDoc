/*
 *$Header: p:/presto/mkinf/rcs/mkmcumot.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: mkmcumot.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期バージョン
 * 
 */

#include <stdio.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/


#define		FROM_TRAN_BLOCK   (0x0200)		/* for MCU1,2 */
#define     MAX_ITEM   5
#define     MCU1     1
#define     MCU2     2

#define     DEBUG

long		entryadrs;
long		progsize;
unsigned long	sumttl,sumtmp,sumcnt;

int		M1,M2,M1old,M2old,M1new,M2new,M1cnt,M2cnt;

char	mcuname[MAX_ITEM][16];
long	mcuadrs[MAX_ITEM];
long	mcusize[MAX_ITEM];

char	dummy[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

int		chk256(int m);
int		adddummy(int mcu, int inc);
void	mkmotstr(char buf[], char mem[], long ofs, int len);
int		mkmotsum(int mcu, char *filename);
int		transMOT(char *buf, long *maxpp, long *minpp);
int		htoi(char a);
char	*struprx(char *buf);
void	put02hexaa(char *aa,int sum);
void	*Bin_Asc16( long value, unsigned char *asc, size_t keta );
void	rsstr(char *str);
void	put8hex(long i);
void	put4hex(long i);
long	calprogsize(long size, long block);


/*********************************************************************
 *ＭＫＭＣＵＭＯＴ
 *　　開発０４－００１２７で報告した、ＭＣＵボードのブートローダの
 *　通信エラーを起こすバグに対処するチェック＆補正プログラム。
 *　
 *　　ＭＣＵ１，ＭＣＵ２のプログラム・サイズをＭ１，Ｍ２という値を用いて
 *　　　ＭＣＵ１のプログラム・サイズ　２５６　ｘ　Ｍ１　バイト
 *　　　ＭＣＵ２のプログラム・サイズ　２５６　ｘ　Ｍ２　バイト
 *　とするとき、番号
 *　　　10+M1,11+M1,14+M1,15+M1,17+M1,18+M1,23+M1+M2,24+M1+M2,14+M2,15+M2
 *　　　18+M1,19+M1,22+M1,23+M1,25+M1,26+M1,31+M1+M2,32+M1+M2（計１８個）
 *　が２５６の倍数の場合に、ＭＣＵ１，２のMOTﾌｧｲﾙにﾀﾞﾐｰﾃﾞｰﾀを付加して
 *　ﾌﾟﾛｸﾞﾗﾑｻｲｽﾞを修正して２５６の倍数にならないようにする。
 *　
 *　　ＭＫＩＮＦ２での　-mcu -rn と同じオプション設定で動作する。
 *　ＭＫＩＮＦ２の直前でＭＫＭＣＵＭＯＴを行う。
 *　ＭＣＵ１，２のＭＯＴファイル名はｉｎｆｉｎｆｏ．ｔｘｔから取り込む。
 *　
 *プログラムの流れ
 *　１．ＭＣＵ１，２のプログラムサイズを調べる。
 *　２．Ｍ１，Ｍ２を計算し、上記の１８個の番号が２５６の倍数にならないか
 *　　　チェックする。２５６の倍数になる場合、最小の増加で済むＭ１，Ｍ２
 *　　　を見つける
 *　　２．１　Ｍ１を決める
 *　　　　　　　　10+M1,11+M1,14+M1,15+M1,17+M1,18+M1,
 *　　　　　　　　18+M1,19+M1,22+M1,23+M1,25+M1,26+M1,
 *　　　　　　　が２５６の倍数の時、Ｍ１を＋１する。１．に戻る。
 *　　２．２　Ｍ２を決める
 *　　　　　　　　14+M2,15+M2
 *　　　　　　　が２５６の倍数の時、Ｍ２を＋１する。２．に戻る。
 *　　２．３　Ｍ１＋Ｍ２を確認する
 *　　　　　　　　23+M1+M2,24+M1+M2,
 *　　　　　　　　31+M1+M2,32+M1+M2,
 *　　　　　　　が２５６の倍数の時、Ｍ２を＋１する。２．に戻る。
 *　３．決定したＭ１，Ｍ２に対応するダミーデータを付加する。
 *　
 *********************************************************************/
int
main(void){
	FILE	*fpr;
	static char	buf[2][120];
	int		c,j,keynum,itemnum;

	printf("MKMCUMOT MOT file modifier for MCU board Version 1.1\n");
	printf("   Copyright (c) 2002-2004 Ishida Co.,Ltd.\n");

	/*　１．ＭＣＵ１，２のプログラムサイズを調べる。*/
	if((fpr = fopen("INFINFO.TXT", "rb")) == NULL){
		rsstr("file open error INFINFO.TXT.\n");
		return(-1);
	}

	buf[0][0] = buf[1][0] = (char)0;
	for(keynum = 0;keynum < 3;++keynum){
		itemnum = 0;
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
		strcpy(mcuname[keynum],buf[0]);

		if(keynum==0) continue;

		/* MOTファイルのファイルサイズを計算する */
		if(mkmotsum(keynum,buf[0]) < 0){
			fclose(fpr);
			rsstr("mkmotsum error.\n");
			return(-1);
		}
		mcuadrs[keynum] = entryadrs;
		mcusize[keynum] = progsize;
	}
	fclose(fpr);

	/*　２．Ｍ１，Ｍ２を計算し上記の１８個の番号が２５６の倍数にならないか
	 *　　　チェックする。２５６の倍数になる場合、最小の増加で済むＭ１，Ｍ２
	 *　　　を見つける*/
	M1cnt=M2cnt=0;
	M1 = (int)(mcusize[MCU1]/256L);
	M1old = M1;
	M2 = (int)(mcusize[MCU2]/256L);
	M2old = M2;
	printf("\nOld M1=%3d M2=%3d\n",M1,M2);

	/*　　２．１　Ｍ１を決める*/
loop1:
	if(M1cnt) printf("M1+%1d\n",M1cnt);
	M1cnt+=2;
	if(chk256(10+M1)==0){M1+=2; goto loop1;}
	if(chk256(11+M1)==0){M1+=2; goto loop1;}
	if(chk256(14+M1)==0){M1+=2; goto loop1;}
	if(chk256(15+M1)==0){M1+=2; goto loop1;}
	if(chk256(17+M1)==0){M1+=2; goto loop1;}
	if(chk256(18+M1)==0){M1+=2; goto loop1;}
	if(chk256(19+M1)==0){M1+=2; goto loop1;}
	if(chk256(22+M1)==0){M1+=2; goto loop1;}
	if(chk256(23+M1)==0){M1+=2; goto loop1;}
	if(chk256(25+M1)==0){M1+=2; goto loop1;}
	if(chk256(26+M1)==0){M1+=2; goto loop1;}

	M1new = M1;

	/*　　２．２　Ｍ２を決める*/
#ifdef DEBUG
	printf("\n");
#endif
loop2:
	if(M2cnt) printf("M2+%1d\n",M2cnt);
	M2cnt+=2;
	if(chk256(14+M2)==0){M2+=2; goto loop2;}
	if(chk256(15+M2)==0){M2+=2; goto loop2;}

	/*　　２．３　Ｍ１＋Ｍ２を確認する*/
	if(chk256(23+M1+M2)==0){M2+=2; goto loop2;}
	if(chk256(24+M1+M2)==0){M2+=2; goto loop2;}
	if(chk256(31+M1+M2)==0){M2+=2; goto loop2;}
	if(chk256(32+M1+M2)==0){M2+=2; goto loop2;}

	M2new = M2;

	/*　３．決定したＭ１，Ｍ２に対応するダミーデータを付加する。*/
	printf("\nNew M1=%3d M2=%3d\n",M1,M2);
	if(M1old != M1new){
		if(adddummy(MCU1,M1new-M1old)<0){
			rsstr("adddummy error.\n");
			return(-1);
		}
	}
	else printf("MCU1 no change.\n");

	if(M2old != M2new){
		if(adddummy(MCU2,M2new-M2old)<0){
			rsstr("adddummy error.\n");
			return(-1);
		}
	}
	else printf("MCU2 no change.\n");

	rsstr("\nOK\n");
	return(0);
}


int
chk256(int m){
#ifdef DEBUG
	if(m % 256) printf("%d ",m);
	else printf("*%d* ",m);
#endif
	return(m % 256);
}


int
adddummy(int mcu, int inc){
	FILE	*fp,*fp2;
	static char	buf[120],bufend[120],fname[120];
	long	adrs,size,ofs;

	adrs = mcuadrs[mcu];
	size = mcusize[mcu];
	strcpy(fname,mcuname[mcu]);
	strcat(fname,".mot");

	fp=fopen(fname,"rb");
	if(fp==NULL){
		printf("Can not open %s.\n",fname);
		return(-1);
	}
	fp2=fopen("TMP","wb");
	if(fp2==NULL){
		printf("Can not open TMP.\n");
		fclose(fp);
		return(-1);
	}
	for(;;){
		if(fgets(buf, (short)120, fp)==NULL) break;
		if(buf[1]=='7') break;
		if(buf[1]=='8') break;
		if(buf[1]=='9') break;
		if(fputs(buf,fp2)==EOF) goto err;
	}
	fclose(fp);
	strcpy(bufend,buf);

	ofs = adrs + size + 256*inc;
	mkmotstr(buf,dummy,ofs-4,4);

	if(fputs(buf,fp2)==EOF) goto err;
	if(fputs(bufend,fp2)==EOF) goto err;
	fclose(fp);
	fflush(fp2);
	fclose(fp2);

	if(remove(fname) !=0){
		printf("remove error %s.\n",fname);
		return(-1);
	}
	if(rename("TMP",fname) !=0){
		printf("rename error TMP->%s.\n",fname);
		return(-1);
	}
#ifdef DEBUG
	printf("MCU%d add dummy data.\n",mcu);
#endif
	return(0);

err:
	fclose(fp);
	fflush(fp2);
	fclose(fp2);
	printf("file write error TMP.\n");
	return(-1);
}


void
mkmotstr(char buf[], char mem[], long ofs, int len){
	char	*adr;
	int		i,j,idx,sum;
	union {
		unsigned long  lstr[2];
		unsigned short hstr[4];
		unsigned char  bstr[8];
	} intbuf;		/* Little Endian */

	intbuf.lstr[0] = ofs;
	if(ofs <= 0x0000ffffL){
		strcpy(buf,"S1");
		adr = (char *)(&intbuf.bstr[1]);
		j = 2;
	}
	else if(ofs <= 0x00ffffffL){
		strcpy(buf,"S2");
		adr = (char *)(&intbuf.bstr[2]);
		j = 3;
	}
	else{
		strcpy(buf,"S3");
		adr = (char *)(&intbuf.bstr[3]);
		j = 4;
	}
	sum = len+j+1;
	put02hexaa((char *)(&buf[2]),(int)sum);
	idx = 4;
	for(i=0;i<j;++i){
		sum += (int)*adr;
		put02hexaa((char *)(&buf[idx]),(int)(*adr));
		idx += 2;
		--adr;
	}
	adr = mem;
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

	buf[idx++] = (char)0x0d;
	buf[idx++] = (char)0x0a;	/* for ASCII mode */
	buf[idx]   = (char)0;
}


int
mkmotsum(int mcu, char *filename){
	FILE	*fp;
	static char	buf[120],fname[120];
	int		c,i,j;
	long	maxp,minp;

	sumcnt = (unsigned long)0;
	sumtmp = (unsigned long)0;
	minp = 0x7fffffffL;
	maxp = 0x00000000L;

	strcpy(fname,filename);
	strcat(fname,".mot");

	fp=fopen(fname,"rb");
	if(fp==NULL){
		printf("Can not open %s.\n",fname);
		return(-1);
	}
	printf("\nMCU%1d %s",mcu,fname);
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
			rsstr("Is Data strange,isn't it?\n");
			fclose(fp);
			return(-1);
		}
		if(i==2){
			rsstr("Check Sum Error !!\n");
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
	progsize = calprogsize(progsize,FROM_TRAN_BLOCK);
	rsstr("\n size:"); put8hex(progsize);
	sumttl  = (unsigned long)0x0ff * ((unsigned long)progsize - sumcnt);
	sumttl += sumtmp;
	rsstr("\n  sum:"); put4hex(sumttl);
	rsstr("\n");
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


