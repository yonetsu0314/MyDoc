/******************************************************************************
* ‚l‚‰‚m‚d‚s|‚V‚O‚S‚Q ’è‹`                                                   *
*                                                                             *
*  File name : net7042.h                                                      *
*  Copyright (c) 1997, MiSPO Co., Ltd.                                        *
*                                                                             *
* 30/Nov/1998                                                           MiSPO *
******************************************************************************/

#ifndef NET7042_H
#define NET7042_H

/* no3rtc.c */

struct tm {
    int tm_sec;     /* •b */
    int tm_min;     /* •ª */
    int tm_hour;    /* i0`23j*/
    int tm_mday;    /* “úi1`31j*/
    int tm_mon;     /* Œi0`11, 1Œ‚ğ0‚Æ‚·‚éj*/
    int tm_year;    /* ”Ni1900”N‚ğ0‚Æ‚·‚éj*/
    int tm_wday;    /* —ji0`6, “ú—j“ú‚ğ0‚Æ‚·‚éj*/
    int tm_yday;    /* ”N‰‚©‚ç‚Ì’ÊZ“ú”i–¢ƒTƒ|[ƒg,0j */
    int tm_isdst;   /* ‚0:‰ÄŠÔ—LŒø,0:–³Œøi0‚Ì‚İj*/
};

BOOL gettime(struct tm *time);
BOOL settime(struct tm *time);

/* no3flash.c */
                    /* Flash memory address */
#define FLUSH 0x00800000

BOOL flash_id(UH *maker, UH *device);
BOOL flash_erase(UW addr, W size);
BOOL flash_program(UW addr, void *data, W size);
BOOL flash_clear(UW addr, W size);

/* no3eepr.c */

BOOL ini_eeprom(void);
BOOL write_eeprom(UH addr, UB data);
BOOL read_eeprom(UH addr, UB *data);
BOOL page_write_eeprom(UH addr, UB *buf, INT size);
BOOL seq_read_eeprom(UH addr, UB *buf, INT size);

/* mi42tes.c */

void ini_mpu(void);
void ini_port(void);
BOOL read_ethernet_addr(void);
BOOL read_ip_addr(void);

#endif /* NET7042_H */
