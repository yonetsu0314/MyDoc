/******************************************************************************
* �l���m�d�s�|�V�O�S�Q ��`                                                   *
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
    int tm_sec;     /* �b */
    int tm_min;     /* �� */
    int tm_hour;    /* ���i0�`23�j*/
    int tm_mday;    /* ���i1�`31�j*/
    int tm_mon;     /* ���i0�`11, 1����0�Ƃ���j*/
    int tm_year;    /* �N�i1900�N��0�Ƃ���j*/
    int tm_wday;    /* �j�i0�`6, ���j����0�Ƃ���j*/
    int tm_yday;    /* �N������̒ʎZ�����i���T�|�[�g,0�j */
    int tm_isdst;   /* ��0:�Ď��ԗL��,��0:�����i0�̂݁j*/
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
