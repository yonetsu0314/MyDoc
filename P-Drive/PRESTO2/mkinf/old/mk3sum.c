#include <stdio.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/


#define     LENGTH_MAX      (1024)          /* ダウンロード　バイト数 */
#define     FROM_TRAN_BLOCK (LENGTH_MAX/2)
#define     FLASH_WRITE_BLOCK (0x2000)		/* for MCU3 */


long		entryadrs;
long		progsize;
unsigned long	sumttl,sumtmp,sumcnt;


int		transMOT(char *buf, long *maxpp, long *minpp);
int		htoi(char a);
void	put02hexaa(char *aa,int sum);
void	*Bin_Asc16( long value, unsigned char *asc, size_t keta );
void	rsstr(char *str);
void	put8hex(long i);
void	put4hex(long i);
long	calprogsize(long size, long block);


int
main(int argc,char **argv){
	FILE	*fp;
	char	buf[120];
	char	filename[100];
	int		c,i,j;
	long	maxp,minp;

	sumcnt = (unsigned long)0;
	sumtmp = (unsigned long)0;
	minp = 0x7fffffffL;
	maxp = 0x00000000L;

	if(argc<2){
		printf("\nEnter Filename.\n");
		return(0);
	}
	strcpy(filename,argv[1]);
	fp=fopen(filename,"rb");
	if(fp==NULL){
		printf("\nCan not open %s.\n",filename);
		return(0);
	}
	rsstr("\nSTART\n");
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

	rsstr("\nend");
	rsstr("\nbegin:"); put8hex(minp);
	rsstr("\n  end:"); put8hex(maxp);
	entryadrs = minp;
	progsize = maxp - minp;
//	progsize = calprogsize(progsize,FROM_TRAN_BLOCK);
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);
	rsstr("\n size:"); put8hex(progsize);
	sumttl  = (unsigned long)0x0ff * ((unsigned long)progsize - sumcnt);
	sumttl += sumtmp;
	rsstr("\n  sum:"); put4hex(sumttl);
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


