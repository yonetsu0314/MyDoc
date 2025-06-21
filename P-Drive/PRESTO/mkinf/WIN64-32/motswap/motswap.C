#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define		READ_MAX	530



static char
trans_ascii(
	char	data
)
{
	if (data < 10) {
		data += '0';
	}
	else {
		data += 'A' - 10;
	}
	return(data);
}

/*----------------------------------------------------------------------*/
/*	itoh								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 4byte HEX to ASCII.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	i	: short value						*/
/*	*s	: pointer to string					*/
/*									*/
/* return value								*/
/*	void	:							*/
/*----------------------------------------------------------------------*/
void
itoh(
	unsigned short	i,			/* converted value	*/
	char		*s
)
{
	int	n;
	char	c;

	for (n = sizeof(i) * 8 - 4; n >= 0; n -= 4) {
		c = (char)(i >> n);
		*(s++) = trans_ascii(c);
		i -= (short)(c << n);
	}
}

/*----------------------------------------------------------------------*/
/*	ctoh								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 2byte HEX to ASCII.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	i	: char value 						*/
/*	*s	: pointer to string					*/
/*									*/
/* return value								*/
/*	void	: 							*/
/*----------------------------------------------------------------------*/
void
ctoh(
	unsigned char	i,
	char		*s
)
{
	s[0] = trans_ascii((char)(i >> 4));
	s[1] = trans_ascii((char)(i & 0x0f));
}

/*----------------------------------------------------------------------*/
/*	ltoh								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 4byte HEX to ASCII.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	l	: long value						*/
/*	*s	: pointer to string					*/
/*									*/
/* return value								*/
/*	void	: 							*/
/*----------------------------------------------------------------------*/
void
ltoh(
	unsigned long	l,
	char		*s
)
{
	int	n;
	char	c;

	for (n = sizeof(l) * 8 - 4; n >= 0; n -= 4) {
		c = (char)(l >> n);
		*(s++) = trans_ascii(c);
		l -= (unsigned long)c << n;
	}
}

/*----------------------------------------------------------------------*/
/*	htoi								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 4byte ASCII to short.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	*buff	: pointer of HEX ASCII string				*/
/*									*/
/* return value								*/
/*	int	:							*/
/*----------------------------------------------------------------------*/

unsigned short
htoi(
	char	*buff
)
{
	int		j;
	unsigned short	i;

	i = 0;
	for (j = 0; j<4; j++) {
		if (*buff >= 'A') 	i = (unsigned short)(i * 16 + *buff - 'A' + 10);
		else if ((*buff >= '0') && (*buff <= '9'))
			i = (unsigned short)(i * 16 + *buff - '0');
		buff++;
	}
	return(i);
}

/*----------------------------------------------------------------------*/
/*	htoc								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 2byte ASCII to char.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	*buff	: pointer of HEX ASCII string				*/
/*									*/
/* return value								*/
/*	char	:							*/
/*----------------------------------------------------------------------*/

unsigned char
htoc(
	char	*buff
)
{
	int		j;
	unsigned char	i;

	i = 0;
	for (j = 0; j<2; j++) {
		if (*buff >= 'A') 	i = (unsigned char)(i * 16 + *buff - 'A' + 10);
		else if ((*buff >= '0') && (*buff <= '9'))
			i = (unsigned char)(i * 16 + *buff - '0');
		buff++;
	}
	return(i);
}

/*----------------------------------------------------------------------*/
/*	htol								*/
/*----------------------------------------------------------------------*/
/* function								*/
/*	convert 8byte ASCII to char.					*/
/*----------------------------------------------------------------------*/
/* ˆø‚«”								*/
/*	*buff	: pointer of HEX ASCII string				*/
/*									*/
/* return value								*/
/*	long	:							*/
/*----------------------------------------------------------------------*/

unsigned long
htol(
	char	*buff
)
{
	int		j;
	unsigned long	i;

	i = 0;
	for (j = 0; j<8; j++) {
		if (*buff >= 'A') 	i = i * 16 + *buff - 'A' + 10;
		else if ((*buff >= '0') && (*buff <= '9'))
			i = i * 16 + *buff - '0';
		buff++;
	}
	return(i);
}

/*--------------------------------------------*/
/* main function                              */
/* swap upper byte  ,<--> lower byte          */
/*--------------------------------------------*/
int
main(
	int	argc,
	char	*argv[]
)
{
	long		addr, max_size, st_addr, offset;
	int		i, j, err, byte_cnt, sum, data_offset, nof_data;
	int		s1, s2, s3;
	FILE		*fp_bin, *fp_mot, *fp_checksum;
	unsigned char	str[READ_MAX], data[255], tmp[8];
	unsigned  short	checksum = 0;

	/* Show usage */
	if (argc != 4) {
		printf("USAGE: motswap \"FILE NAME\" \"ROM SIZE[Kbyte]\" \"OFFSET ADDRESS\"\n");
		return 0;
	}
	max_size = atoi(argv[2]) * 128L;
	if (max_size & 1) {
		printf("ROM SIZE IS NOT AN EVEN NUMBER!\n");
		return 0;
	}
	i = (int)strlen(argv[3]);
	if (i > 8) {
		printf("OFFSET ADDRESS IS OVERFLOW!\n");
		return 0;
	}
	memset(tmp, 0, sizeof(tmp));
	memcpy(&tmp[8 - i], argv[3], i);
	offset = htol(tmp);
	fp_bin = tmpfile();
	if (fp_bin == NULL) {
		printf("FILE \"temp.bin\" OPEN ERROR!\n");
		return -1;
	}
	fp_mot = fopen(argv[1], "r");
	if (fp_mot == NULL) {
		printf("FILE \"%s\" DOSE NOT EXIST!\n", argv[1]);
		fclose(fp_bin);
		return -1;
	}

	/* Fill 0xff */
	for (addr = 0; addr<max_size; addr++) {
		err = fseek(fp_bin, addr, SEEK_SET);
		if (err) {
			printf("CAN NOT WRITE BINARY DATA!\n");
			fclose(fp_bin);
			fclose(fp_mot);
			return -1;
		}
		fputc(0xff, fp_bin);
	}

	/* Read MOTOROLA file */
	while (fgets(str, READ_MAX - 1, fp_mot) != NULL) {
		if (str[0] == 'S') {
			switch (str[1]) {
			case '1':	/* S1 format(16bit address) */
				byte_cnt = htoc(&str[2]);
				st_addr = htoi(&str[4]);
				data_offset = 3;
				nof_data = byte_cnt - 3;
				break;
			case '2':	/* S2 format(24bit address) */
				byte_cnt = htoc(&str[2]);
				tmp[0] = tmp[1] = 0;
				memcpy(&tmp[2], &str[4], 6);
				st_addr = htol(tmp);
				data_offset = 4;
				nof_data = byte_cnt - 4;
				break;
			case '3':	/* S3 format(32bit address) */
				byte_cnt = htoc(&str[2]);
				st_addr = htol(&str[4]);
				data_offset = 5;
				nof_data = byte_cnt - 5;
				break;
			default:
				data_offset = 0;
				break;
			}
			if (data_offset > 0) {
				st_addr -= offset;
				if (st_addr < 0) {
					printf("OFFSET ADDRESS IS INCORRECT!\n");
					fclose(fp_mot);
					fclose(fp_bin);
					return -1;
				}
				for (sum = 0, i = 0, j = 2; i <= byte_cnt; i++, j += 2) {
					data[i] = htoc(&str[j]);
					sum += data[i];
				}
				if ((sum & 0xff) != 0xff) {
					printf("CHECKSUM ERROR !\n");
					fclose(fp_mot);
					fclose(fp_bin);
					return -1;
				}
			}
			else {
				continue;
			}
			/* Write binary data to file */
			for (i = 0, j = data_offset; i<nof_data; i++, j++) {
				err = fseek(fp_bin, st_addr, SEEK_SET);
				if (err) {
					printf("CAN NOT WRITE BINARY DATA!\n");
					fclose(fp_bin);
					fclose(fp_mot);
					return -1;
				}
				fputc(data[j], fp_bin);
				++st_addr;
			}
		}
		else {
			printf(" %s IS NOT MOTOROLA FILE!\n", argv[1]);
			fclose(fp_mot);
			fclose(fp_bin);
			return -1;
		}
	}
	/* Swap upper byte for lower byte */
	for (addr = 0; addr<max_size; addr += 2) {
		fseek(fp_bin, addr, SEEK_SET);
		i = fgetc(fp_bin);
		j = fgetc(fp_bin);
		if (i != j) {
			fseek(fp_bin, addr, SEEK_SET);
			fputc(j, fp_bin);
			fputc(i, fp_bin);
		}
	}
	fclose(fp_mot);
	fp_mot = fopen(argv[1], "w");
	/* Make new MOTOROLA file */
	s1 = s2 = s3 = 0;
	for (addr = 0; addr<max_size;) {
		st_addr = addr;
		j = 0;
		byte_cnt = 0;
		for (i = 0; i<16; i++) {
			++byte_cnt;
			fseek(fp_bin, addr, SEEK_SET);
			data[i] = (unsigned char)fgetc(fp_bin);
			if (data[i] != 0xff) {
				j = 1;
			}
			if (++addr >= max_size) {
				break;
			}
		}
		if (j) {
			sum = 0;
			if (st_addr < 0x10000) {
				s1 = 1;
				fprintf(fp_mot, "S1%02X%04lX", byte_cnt + 3, st_addr);
				sum = (byte_cnt + 3) +
					(int)(st_addr >> 8) +
					(int)(st_addr & 0xff);
			}
			else if (st_addr < 0x1000000) {
				s2 = 1;
				fprintf(fp_mot, "S2%02X%06lX", byte_cnt + 4, st_addr);
				sum = (byte_cnt + 4) +
					(int)(st_addr >> 16) +
					(int)((st_addr & 0x00ff00) >> 8) +
					(int)(st_addr & 0xff);
			}
			else {
				s3 = 1;
				fprintf(fp_mot, "S3%02X%08lX", byte_cnt + 5, st_addr);
				sum = (byte_cnt + 5) +
					(int)(st_addr >> 24) +
					(int)((st_addr & 0x00ff0000) >> 16) +
					(int)((st_addr & 0x0000ff00) >> 8) +
					(int)(st_addr & 0xff);
			}
			for (i = 0; i<byte_cnt; i++) {
				sum += data[i];
				fprintf(fp_mot, "%02X", data[i]);
			}
			sum = ~sum & 0xff;
			fprintf(fp_mot, "%02X\n", sum);
		}
	}
	/* Calculate checksum */
	for (addr = 0; addr<max_size; addr++) {
		err = fseek(fp_bin, addr, SEEK_SET);
		if (err) {
			printf("CAN NOT READ BINARY DATA!\n");
			fclose(fp_bin);
			fclose(fp_mot);
			return -1;
		}
		checksum += fgetc(fp_bin);
	}
	fp_checksum = fopen("checksum.dat", "w");
	if (fp_checksum != NULL) {
		fprintf(fp_checksum, "%04X\n", checksum);
	}
	fclose(fp_checksum);

	if (s1)	fprintf(fp_mot, "S9030000FC\n");
	if (s2)	fprintf(fp_mot, "S8040000FB\n");
	if (s3)	fprintf(fp_mot, "S7050000FA\n");
	fclose(fp_mot);
	fclose(fp_bin);
	return 0;
}
