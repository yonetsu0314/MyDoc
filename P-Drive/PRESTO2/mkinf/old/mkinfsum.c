#include <stdio.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/


char	mcuname[5][16];
char	mcuver[5][8];
char	mcusum[5][8];

int		transINF(char *buf);
char	*struprx(char *buf);
void	*Bin_Asc16( long value, unsigned char *asc, size_t keta );
void	setmcuinf(int n, char *buf);
void	setinfend(int n, char *buf);
void	rsstr(char *str);


int
main(){
	FILE	*fp;
	static char	buf[120];
	unsigned short	sum,sumsub;
	int		c,i,j,keynum;

	if((fp = fopen("PROGRAM.INF", "rb")) == NULL){
		rsstr("file open error.");
		return(0);
	}
	sum = (unsigned short)0;
	keynum = 0;
	for(;;){
		sumsub = (unsigned short)0;
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
			sumsub += (unsigned short)c;
			continue;
		}

		i = transINF(buf);

		if(i < 0) break;
		sum += sumsub;
		++keynum;
	}
	if(keynum != 4) rsstr("item less than 4");
	Bin_Asc16( (long)sum, (unsigned char *)(&buf[0]), (size_t)4 );
	buf[4] = 0;
	rsstr("\nsum = ");
	rsstr(buf);
	if(strcmp(buf,mcusum[4]) != 0) rsstr("sum error");
	rsstr("\nend");
	fclose(fp);
	return(0);
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


