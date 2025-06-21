/*----------------------------------------------------------------------*
 * File name	: ctime.c						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/ctime.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: ctime.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/11/12 11:30
 * locatime(),localdata()をビッグエンディアン仕様であるＤＭＵで
 * 使用できるように変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
#include	<time.h>
#include	<string.h>

#define	USA		1
#define	ENGLISH		2
#define	JAPANESE	3
#define	FRENCH		4
#define	GERMAN		5
#define	SPANISH		6
#define	SWEDISH		7
#define	DANISH		8
#define	DUTCH		9
#define	FINNISH		10

int	country_flag = 1;

void i2a(char *buff,int i);
void i4a(char *buff,int i);

/*----------------------------------------------*/
/*	country() function			*/
/*----------------------------------------------*/

int	country(num)
int	num;	/* read flag		0 		*/
		/* format number	1 ... U.S.A	*/
		/*			2 ... ENGLISH	*/
		/*			3 ... JAPANESE  */
		/*			4 ... FRENCH	*/
		/*			5 ... GERMAN	*/
		/*			6 ... SPANISH	*/
		/*			7 ... SWEDISH	*/
		/*			8 ... DANISH	*/
		/*			9 ... DUTCH	*/
		/*		       10 ... FINNISH	*/
{
	if(num)
		country_flag = num;
	else
		return(country_flag);
}

char *
ctime(clock)
	time_t *clock;
{
	struct tm *tm;

	tm = localtime(clock);
	return asctime(tm);
}

struct tm *
localtime(clock)
	time_t *clock;
{
	union {
		time_t l;
		unsigned short u[2];
	} un;
	static struct tm tm;

	un.l = *clock;
//	tm.tm_sec = (short)((un.u[0]) & 63);
//	tm.tm_min = (short)((un.u[0]>>8) & 63);
//	tm.tm_hour = (short)((un.u[1]) & 31);
	tm.tm_sec = (short)((un.u[1]) & 63);
	tm.tm_min = (short)((un.u[1]>>8) & 63);
	tm.tm_hour = (short)((un.u[0]) & 31);
	return &tm;
}

char *
asctime(tm)
	struct tm *tm;
{
	static char buffer[10];

	i2a(&buffer[0],(int)tm->tm_hour);
	buffer[2] = (unsigned char)':';
	i2a(&buffer[3],(int)tm->tm_min);
	buffer[5] = (unsigned char)':';
	i2a(&buffer[6],(int)tm->tm_sec);
	buffer[8] = (unsigned char)0;
	return buffer;
}


void i2a(buff,i)
	char	*buff;
	int	i;
{
	int	d_high,d_low;
	d_high = i/10;
	d_low  = i - d_high*10;
	*buff++ = (unsigned char)(d_high + '0');
	*buff++  = (unsigned char)(d_low + '0');
	*buff = (unsigned char)0;
}

void i4a(buff,i)
	char	*buff;
	int	i;
{
	*buff++ = (unsigned char)(i/1000 + '0');
	*buff++ = (unsigned char)(i/100 - (i/1000)*10 + '0');
	*buff++ = (unsigned char)(i/10 - (i/100)*10 + '0');
	*buff++ = (unsigned char)(i%10 + '0');
	*buff = (unsigned char)0;
}

char *
cdate(clock)
	date_t *clock;
{
	struct dt *tm;

	tm = localdate(clock);
	return ascdate(tm);
}                     

struct dt *
localdate(clock)
	date_t *clock;
{
	union {
		date_t l;
		unsigned short u[2];
	} un;
	static struct dt tm;

	un.l = *clock;
//	tm.tm_wday = (short)((un.u[0]) & 7);
//	tm.tm_mday = (short)((un.u[0]>>8) & 31);
//	tm.tm_mon =  (short)(((un.u[1]) & 15) - 1);
//	tm.tm_year = (short)((un.u[1]>>8) & 127);
	tm.tm_wday = (short)((un.u[1]) & 7);
	tm.tm_mday = (short)((un.u[1]>>8) & 31);
	tm.tm_mon =  (short)(((un.u[0]) & 15) - 1);
	tm.tm_year = (short)((un.u[0]>>8) & 127);
	return &tm;
}

char *
ascdate(tm)
	struct dt *tm;
{
	int	year;
	int	day;
	int	month;
	static char days[7][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static char days_jap[7][5] = {
		"(日)", "(月)", "(火)", "(水)", "(木)", "(金)", "(土)"
	};
	static char months_eng[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char months_fre[12][4] = {
		"Jan", "Fev", "Mar", "Avr", "Mai", "Jun", 
		"Jul", "Aov", "Sep", "Oct", "Nov", "Dec"
	};
	static char months_ger[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", 
		"Jul", "Aug", "Sep", "Okt", "Nov", "Dez"
	};
	static char months_spa[12][4] = {
		"Ene", "Feb", "Mar", "Abr", "May", "Jun",
		"Jul", "Ago", "Sep", "Oct", "Nov", "Dic"
	};
	static char months_swe_dan[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "Maj", "Jun",
		"Jul", "Aug", "Sep", "Okt", "Nov", "Dec"
	};
	static char months_dut[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "Mai", "Jun",
		"JUl", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char months_fin[12][4] = {
		"Tam", "Hel", "Maa", "Huh", "Tou", "Kes",
		"Hei", "Elo", "Syy", "Lok", "Mar", "Jou"
	};
	static char buffer[22];
        

	year = tm->tm_year + 1980;

	switch(country_flag) {
	case USA:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2];
       		buffer[3]=(unsigned char)' ';
       		buffer[4]=months_eng[tm->tm_mon][0];
       		buffer[5]=months_eng[tm->tm_mon][1];
       		buffer[6]=months_eng[tm->tm_mon][2];
       		buffer[7]=(unsigned char)'-';
       		day = tm->tm_mday;
       		i2a(&buffer[8],day);
       		buffer[10] = (unsigned char)'-';
       		i4a(&buffer[11],year);
		buffer[15] = (unsigned char)0;
               	break;
	case ENGLISH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
		buffer[7]=months_eng[tm->tm_mon][0];
		buffer[8]=months_eng[tm->tm_mon][1];
       		buffer[9]=months_eng[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
      		break;
	case JAPANESE:
		i4a(&buffer[0],year);
		strcpy(&buffer[4],"年");
       		month = (tm->tm_mon)+1;
       		i2a(&buffer[6],month);
       		strcpy(&buffer[8],"月");
       		day = tm->tm_mday;
       		i2a(&buffer[10],day);
       		strcpy(&buffer[12],"日");
       		buffer[14]=days_jap[tm->tm_wday][0];
       		buffer[15]=days_jap[tm->tm_wday][1];
       		buffer[16]=days_jap[tm->tm_wday][2];
       		buffer[17]=days_jap[tm->tm_wday][3];
       		buffer[18] = (unsigned char)0;
		break;
	case FRENCH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_fre[tm->tm_mon][0];
       		buffer[8]=months_fre[tm->tm_mon][1];
       		buffer[9]=months_fre[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	case GERMAN:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_ger[tm->tm_mon][0];
       		buffer[8]=months_ger[tm->tm_mon][1];
       		buffer[9]=months_ger[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	case SPANISH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_spa[tm->tm_mon][0];
       		buffer[8]=months_spa[tm->tm_mon][1];
       		buffer[9]=months_spa[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	case SWEDISH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_swe_dan[tm->tm_mon][0];
       		buffer[8]=months_swe_dan[tm->tm_mon][1];
       		buffer[9]=months_swe_dan[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	case DANISH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
		buffer[6]=(unsigned char)'-';
      		buffer[7]=months_swe_dan[tm->tm_mon][0];
		buffer[8]=months_swe_dan[tm->tm_mon][1];
       		buffer[9]=months_swe_dan[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	case DUTCH:
	      	buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_dut[tm->tm_mon][0];
       		buffer[8]=months_dut[tm->tm_mon][1];
       		buffer[9]=months_dut[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
 	case FINNISH:
		buffer[0]=days[tm->tm_wday][0];
       		buffer[1]=days[tm->tm_wday][1];
       		buffer[2]=days[tm->tm_wday][2]; 
       		buffer[3]=(unsigned char)' ';
       		day = tm->tm_mday;
       		i2a(&buffer[4],day);
       		buffer[6]=(unsigned char)'-';
       		buffer[7]=months_fin[tm->tm_mon][0];
       		buffer[8]=months_fin[tm->tm_mon][1];
       		buffer[9]=months_fin[tm->tm_mon][2];
       		buffer[10] = (unsigned char)'-';
		i4a(&buffer[11],year);
       		buffer[15] = (unsigned char)0;
		break;
	}
     	return(&buffer[0]);
}
