/******************************************************************************
* Driver file for Super IO controller MC1543                                  *
*                                                                             *
*  File name : mc1543.c                                                       *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 17/August/2001     ; file created by SP                                     *
* 20/Oct/2002  Board specific header file (ms7751.h) added      [SP]          *
******************************************************************************/
#ifndef SH4
 #define SH4
#endif
#include "norti3.h"
#include "sh7751.h"
#include "ms7751.h"
#include "pci7751.h"
#include "mc1543.h"

ER init_mc1543(void);               /* Super IO controller initialization */

/******** static function declaration ***********************/
static void set_sio_cfg(UB, UB);    /* Super IO configuration set function */
static void get_sio_cfg(UB, UB*);   /* Super IO configuration read function */
static void set_sio_pcicfg(void);   /* Super IO PCI space initialization */

static BOOL sio_fault;              /* Super IO controller fault flag */

extern void pci_write_config(UB, UB, UB, UB, UW);   /* PCI config reg write */
extern void pci_read_config(UB, UB, UB, UB, UW *);  /* PCI config reg read */
extern ER def_sio_int(void);        /* Super IO interrupt setup */
extern PCI_device sio_device;

/******************************************************************************
*   Routine to initialize Super IO controller for PCI configuration.
*
*******************************************************************************/
void set_sio_pcicfg(void)
{
    UW tmp;

    tmp = 0x8020c77f;           /* set for PIC, IORC, ISAC1 & ISAC2 */
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, 0x40, tmp);

    tmp = 0x00001b9d;           /* IDENR1, PIPM & BCSC setting */
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, 0x44, tmp);

    tmp = 0x00009315;           /* PIRT1 - PIRT4 setting */
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, 0x48, tmp);

    tmp = 0x0000000f;           /* PILET setting */
    pci_write_config(sio_device.bus_no, sio_device.slot_no, 0, 0x4c, tmp);


}
/******************************************************************************
*   Routine to initialize Super IO controller.
*
*******************************************************************************/
ER init_mc1543(void)
{
    UB data;
    UH tmp;

    set_sio_pcicfg();   /* Enable PCI device Interrupts */

    /* Enter the configuration data space */
    pci_iowrite_byte(CONFIG_PORT, CFG_KEY1, 0);
    pci_iowrite_byte(CONFIG_PORT, CFG_KEY2, 0);

    get_sio_cfg(INDEX_0x20, &data);
    tmp = data & 0xff;
    tmp <<= 8;
    get_sio_cfg(INDEX_0x21, &data);
    tmp |= (data & 0xff);       /* device ID */

    if (tmp != MC1543_DEVICE_ID) { /* Driver PCI Interface failed */
        return E_SIO_FAIL;
    }

    get_sio_cfg(INDEX_0x1F, &data);     /* device version */

    /* ISA Bus interface (logical device 2) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_2);
    set_sio_cfg(INDEX_0x30, DEV_EN);

    /* UART1 enable (logical device 4) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_4);
    set_sio_cfg(INDEX_0x30, DEV_EN);

    /* UART3 enable (logical device 5) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_5);
    set_sio_cfg(INDEX_0x30, DEV_EN);

#ifdef ALL_MC1543_DEVICE
    /* This part is not compiled since FDC, PRN, Keyboard and IDE controllers
        are not used in the sample program.
        Please modify this part of code as per the requirement.
    */

    /* FDC enable (logical device 0) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_0);
    set_sio_cfg(INDEX_0x30, DEV_EN);

    /* PRN enable (logical device 3) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_3);
    set_sio_cfg(INDEX_0x30, DEV_EN);

    /* Keyboard enable (logical device 7) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_7);
    set_sio_cfg(INDEX_0x30, DEV_EN);
    set_sio_cfg(INDEX_0x72, 0x0c);      /* Mouse IRQ12  */

    /* IDE enable (logical device 0xB) */
    set_sio_cfg(INDEX_0x07, LOG_DEV_B);
    set_sio_cfg(INDEX_0x30, DEV_EN);
    pci_iowrite_byte(CONFIG_PORT, EXIT_KEY, 0);

#endif

    /* define the event interrupt */
    if (def_sio_int() != E_OK) {
        sio_fault = TRUE;
    }

    return E_OK;
}
/******************************************************************************
*   Routine to write into Super IO configuration data space.
*
* Input -
*   index   - Configuration reg index number.
*   data    - Data to be written to configuration space.
*******************************************************************************/
static void set_sio_cfg(UB index, UB data)
{
    pci_iowrite_byte(INDEX_PORT, index, 0);
    pci_iowrite_byte(DATA_PORT, data, 0);
}
/******************************************************************************
*   Routine to read data from Super IO configuration data space.
*
* Input -
*   index   - Configuration reg index number.
* Output -
*   *data   - Data pointer to store data read from configuration space.
*******************************************************************************/
static void get_sio_cfg(UB index, UB* data)
{
    UB tmp;
    pci_iowrite_byte(INDEX_PORT, index, 0);
    pci_ioread_byte(DATA_PORT, &tmp, 0);
    *data = tmp;

}

/************************* End of file MC1543.C *******************************/

