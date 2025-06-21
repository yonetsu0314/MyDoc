/*
 *$Header: p:/presto/mkinf/rcs/adddummy.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: adddummy.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * mkmotstr関数の動作確認用
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

/*****************************************************
 *機能：
 *　MOTﾌｧｲﾙのｻｲｽﾞを256BYTE単位で増加させる
 *使用法：
 *　ADDDUMMY ファイル名　ブロック数
 *　　ファイル名：MOTﾌｧｲﾙ名。N0038.MOTの場合、N0036のみ入力
 *　　ブロック数：Ｎ：０～１０２３まで。２５６ｘＮバイト増加する
 *例：
 *　ADDDUMMY N0036 108
 *　　N0036.MOTを108x256ﾊﾞｲﾄ、ｻｲｽﾞを増加させる。
 *****************************************************/
int
main(int argc,char **argv){
	static char	buf[2][120];
	int		c,keynum;

	buf[0][0] = buf[1][0] = (char)0;
	keynum = 1;

	strcpy(buf[0],argv[1]);
	struprx(buf[0]);
	strcpy(mcuname[keynum],buf[0]);

	sscanf(argv[2],"%d",&c);

	if(mkmotsum(keynum,buf[0]) < 0){
		rsstr("mkmotsum error.\n");
		return(-1);
	}
	mcuadrs[keynum] = entryadrs;
	mcusize[keynum] = progsize;

	M1 = (int)(mcusize[MCU1]/256L);
	printf("\nM = %d + %d = %d\n",M1,c,M1+c);

	if(adddummy(MCU1,c)<0){
		rsstr("adddummy error.\n");
		return(-1);
	}

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
	printf("MCU add dummy data.\n");
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
	printf("\nMCU %s",fname);
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


