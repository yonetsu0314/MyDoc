#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void main(void)
{
	int clnum, ofs;
	char buf[3];
	FILE *fp;
	
	fp = fopen("D:\\HOME_BORLAND\\FAT.txt", "r");
	if (fp == NULL) {
		printf("error\n");
		return;
	}

	printf("input number\n");
	scanf("%d", &clnum);

//	printf("%X", clnum);
	printf("%d", clnum);
	while (1) {
		ofs = clnum * 3;
			
		// Šï”
		if (clnum % 2) {
			fseek(fp, ofs-1, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum = (int)strtol(buf, (char **)NULL, 16);
			fseek(fp, ofs+1, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum += (int)strtol(buf, (char **)NULL, 16) * 16 * 16;
			fseek(fp, ofs+2, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum += (int)strtol(buf, (char **)NULL, 16) * 16;
		}
		// ‹ô”
		else {
			fseek(fp, ofs, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum = (int)strtol(buf, (char **)NULL, 16) * 16;
			fseek(fp, ofs+1, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum += (int)strtol(buf, (char **)NULL, 16);
			fseek(fp, ofs+3, SEEK_SET);
			fread(buf, 1, 1, fp);
			clnum += (int)strtol(buf, (char **)NULL, 16) * 16 * 16;
		}

//		printf(">%X", clnum);
		printf(">%d", clnum);
		if (clnum == 0x000 || clnum == 0xfff) {
			fclose(fp);
			return;
		}
	}
}

//		printf("%s%02X\n", mot, sum);
