/******************************************************************************
* Header file for SUPER IO driver program for MC1543.                         *
*                                                                             *
*  File name : mc1543.h                                                       *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 27/July/2001     ; file created  by SP                                      *
* 31/Oct/2002      ; E_SIO_FAIL error code changed to (-110)            [SP]  *
******************************************************************************/
#ifndef MC1543_H
#define MC1543_H

#ifdef __cplusplus
extern "C" {
#endif

#define CFG_PORT    1
#define MC1543_DEVICE_ID    0x4315

#if CFG_PORT
#define CONFIG_PORT (SIOC_IO_BASE + 0x3f0)
#define INDEX_PORT  (SIOC_IO_BASE + 0x3f0)
#define DATA_PORT   (SIOC_IO_BASE + 0x3f1)
#else
#define CONFIG_PORT (SIOC_IO_BASE + 0x370)
#define INDEX_PORT  (SIOC_IO_BASE + 0x370)
#define DATA_PORT   (SIOC_IO_BASE + 0x371)
#endif

#define CFG_KEY1    0x51
#define CFG_KEY2    0x23
#define EXIT_KEY    0xbb

#define INDEX_0x02  (SIOC_IO_BASE + 0x02)
#define INDEX_0x07  (SIOC_IO_BASE + 0x07)   /* Logical device number    */
#define LOG_DEV_0   0x00
#define LOG_DEV_1   0x01
#define LOG_DEV_2   0x02
#define LOG_DEV_3   0x03
#define LOG_DEV_4   0x04
#define LOG_DEV_5   0x05
#define LOG_DEV_6   0x06
#define LOG_DEV_7   0x07
#define LOG_DEV_8   0x08
#define LOG_DEV_9   0x09
#define LOG_DEV_A   0x0a
#define LOG_DEV_B   0x0b
#define LOG_DEV_C   0x0c
#define LOG_DEV_D   0x0d

#define INDEX_0x1F  (SIOC_IO_BASE + 0x1f)
#define INDEX_0x20  (SIOC_IO_BASE + 0x20)
#define INDEX_0x21  (SIOC_IO_BASE + 0x21)
#define INDEX_0x22  (SIOC_IO_BASE + 0x22)
#define INDEX_0x23  (SIOC_IO_BASE + 0x23)
#define INDEX_0x2C  (SIOC_IO_BASE + 0x2c)
#define INDEX_0x2D  (SIOC_IO_BASE + 0x2d)
#define INDEX_0x30  (SIOC_IO_BASE + 0x30)   /* Logical device enable    */
#define DEV_EN      0x01

#define INDEX_0x60  (SIOC_IO_BASE + 0x60)
#define INDEX_0x61  (SIOC_IO_BASE + 0x61)
#define INDEX_0x70  (SIOC_IO_BASE + 0x70)
#define INDEX_0x72  (SIOC_IO_BASE + 0x72)
#define INDEX_0x74  (SIOC_IO_BASE + 0x74)
#define INDEX_0xF0  (SIOC_IO_BASE + 0xf0)
#define INDEX_0xF1  (SIOC_IO_BASE + 0xf1)
#define INDEX_0xF2  (SIOC_IO_BASE + 0xf2)
#define INDEX_0xF4  (SIOC_IO_BASE + 0xf4)


#define FDC_BASE    (SIOC_IO_BASE + 0x03f0)
#define PRN_BASE    (SIOC_IO_BASE + 0x0378)
#define UART1_BASE  (SIOC_IO_BASE + 0x03f8)
#define UART2_BASE  (SIOC_IO_BASE + 0x03e8)
#define UART3_BASE  (SIOC_IO_BASE + 0x02f8)

/* PCI space configuration registers for Super IO controller */
#define SIOCFG_PIC      0x40    /* PCI interface control */

/**** Error Codes ********************************************************/
#define E_SIO_FAIL  (-0x110)

#ifdef __cplusplus
}
#endif
#endif /* MC1543_H */
