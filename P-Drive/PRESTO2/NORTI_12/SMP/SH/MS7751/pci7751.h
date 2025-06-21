/******************************************************************************
* Header file for PCI device regiter tableÅiSH7751Åj                          *
*                                                                             *
*  File name : pci7751.h                                                      *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 26/June/2001     ; file created  by [SP]                                    *
* 08/Apr/2002      ; Comments and beautification   [SP]                       *
******************************************************************************/
#ifndef PCI_H
#define PCI_H

#ifdef __cplusplus
extern "C" {
#endif

#define PCIERR_MASK     0x0080          /* PCI interrupt status */

/* Device classes and subclasses */

#define PCI_AMD_LANCE_ROMBASE   0x00000000  /* Mapping MEM area:0x00000000-0x000FFFFF   */
#define PCI_MAX_SLOT    2       /* Maximum 2 external PCI devices can be connected */

#define PCI_AMD_LANCE_APROM0    0x00
#define PCI_AMD_LANCE_APROM1    0x04
#define PCI_AMD_LANCE_APROM2    0x08
#define PCI_AMD_LANCE_APROM3    0x0c
#define PCI_AMD_LANCE_RDP       0x10
#define PCI_AMD_LANCE_RAP       0x14
#define PCI_AMD_LANCE_RESR      0x18
#define PCI_AMD_LANCE_BDP       0x1c


#define PCI_M1543_CFG_PORT      0x3f0
#define PCI_M1543_INDEX_PORT    0x3f0
#define PCI_M1543_DATA_PORT     0x3f1

#define PCI_M1543_CFG_KEY0      0x51
#define PCI_M1543_CFG_KEY1      0x23
#define PCI_M1543_CFG_EXIT      0xbb

/* PCI configuration space register definations */
#define PCI_ID                  0x00
#define PCI_CMD                 0x04
#define PCI_IOBASE              0x10
#define PCI_MEMBASE             0x14
#define PCI_ILR                 0x3c

#define PCI_VENDOR_ID_AMD       0x1022
#define PCI_DEVICE_ID_79C973    0x2000

#define PCI_VENDOR_ID_AL        0x10b9
#define PCI_DEVICE_ID_AL_M1533  0x1533
#define PCI_DEVICE_ID_AL_M5237  0x5237
#define PCI_DEVICE_ID_AL_M7101  0x7101

/* define structure for PCI card */
typedef struct pci_device {
    UH  vendor_id;          /* vendor ID of the PCI device */
    UH  device_id;          /* ID of the PCI device */
    UB  bus_no;             /* bus type for PCI card */
    UB  slot_no;            /* PCI slot no where card is fitted */
    UB  int_no;             /* interrupt no assigned by PCI arbiter */
    UW  io_addr;            /* IO map address for PCI card */
    UW  mem_addr;           /* MEM map address for PCI card */
} PCI_device;

#ifdef __cplusplus
}
#endif
#endif /* PCI_H */

