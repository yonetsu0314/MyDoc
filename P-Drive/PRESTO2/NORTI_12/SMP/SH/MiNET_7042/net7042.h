/******************************************************************************
* ＭｉＮＥＴ−７０４２ 定義                                                   *
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
    int tm_sec;     /* 秒 */
    int tm_min;     /* 分 */
    int tm_hour;    /* 時（0〜23）*/
    int tm_mday;    /* 日（1〜31）*/
    int tm_mon;     /* 月（0〜11, 1月を0とする）*/
    int tm_year;    /* 年（1900年を0とする）*/
    int tm_wday;    /* 曜（0〜6, 日曜日を0とする）*/
    int tm_yday;    /* 年初からの通算日数（未サポート,0） */
    int tm_isdst;   /* ≠0:夏時間有効,＝0:無効（0のみ）*/
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
